#include "Controller.h"
#include <iostream>
#include "src/Alert/AlertController.h"
#include <QDebug>
#include <QTimer>
#include <fstream>
#include <utility>
#include "src/Utils/Utils.h"

const int kMaxNumberOfOperators = 8;

std::unique_ptr<Controller> Controller::instance = std::make_unique<Controller>();

Controller::Controller(QObject *parent) : QObject(parent){
    resetAvailableOperatorIds();
}

bool Controller::isConnected(){
    //Breki - checks if connection to the synth is working
    return true;
}

std::optional<int> Controller::addOperator() {
    if (operators_.size() >= kMaxNumberOfOperators) {
        AlertController::instance->showAlert("Error: Only 8 operators allowed");
        return std::nullopt;
    }

    int id = *std::min_element(availableOperatorIds_.begin(), availableOperatorIds_.end());
    availableOperatorIds_.erase(id);

    auto op = std::make_unique<Operator>(id);
    operators_.insert(std::make_pair<int, std::unique_ptr<Operator>>((int)id, std::move(op)));

    return id;
}

void Controller::removeOperator(int operatorId) {
    removeAllModulatorsForOperator(operatorId);
    removeCarrier(operatorId);

    operators_.erase(operatorId);
    availableOperatorIds_.insert(operatorId);

    // Remove operator from all modulatedBy lists
    for (const auto& op : operators_) {
        if (std::find(op.second->modulatedBy.begin(), op.second->modulatedBy.end(), operatorId) != op.second->modulatedBy.end()) {
            op.second->modulatedBy.erase(std::remove(op.second->modulatedBy.begin(), op.second->modulatedBy.end(), operatorId),op.second->modulatedBy.end());
            api.removeModulator(op.second->id, operatorId);
        }
    }
}

void Controller::noteOn(int note) {
    api.noteOn(note);
}

void Controller::noteOff(int note) {
    api.noteOff(note);
}

void Controller::setAmpEnvelopeSize(int size) {
    api.setAmpEnvelopeSize(size);
}
void Controller::setAttackAmpEnvelopePoint(int index, float value, float time){
    api.setAmpEnvelopeAttackValue(index, value, time);
}

void Controller::setReleaseAmpEnvelopePoint(int index, float value, float time){
    api.setAmpReleaseEnvelopePoint(index, value, time);
}

void Controller::changeFrequency(int operatorId, long frequency) {
    operators_[operatorId]->frequency = frequency;
    emit freqChanged(frequency);
    sendOperator(operatorId);
}

void Controller::changeAmplitude(int operatorId, long amplitude) {
    operators_[operatorId]->amplitude = amplitude;
    emit ampChanged(amplitude);
    sendOperator(operatorId);
}

void Controller::addModulator(int operatorId, int modulatorId) {
    const auto& op = operators_[operatorId];
    const auto& mod = operators_[modulatorId];

    op->modulatedBy.push_back(modulatorId);
    mod->isModulator = true;

    sendAllOperatorInfo(operatorId);
}

void Controller::removeModulator(int operatorId, int modulatorId) {
    const auto& op = operators_[operatorId];
    const auto& mod = operators_[modulatorId];

    op->modulatedBy.erase(std::remove(op->modulatedBy.begin(), op->modulatedBy.end(), modulatorId), op->modulatedBy.end());
    api.removeModulator(operatorId, modulatorId);

    // Check if modulator is modulating any other operator before setting isModulator to false
    for (const auto& op_ : operators_) {
        if (std::find(op_.second->modulatedBy.begin(), op_.second->modulatedBy.end(), modulatorId) == op_.second->modulatedBy.end()) {
            return;
        }
    }
    mod->isModulator = false;
}

void Controller::addCarrier(int operatorId) {
    if (operators_[operatorId]->isCarrier) return;

    operators_[operatorId]->isCarrier = true;
    sendAllOperatorInfo(operatorId);
}

void Controller::removeCarrier(int operatorId) {
    if (!operators_[operatorId]->isCarrier) return;

    operators_[operatorId]->isCarrier = false;
    api.removeCarrier(operatorId);
}

void Controller::sendOperator(int operatorId) {
    const auto& op = operators_[operatorId];
    // Núverandi range 0 - 200 20 er lægsta nóta sem heyrist hæsta er 175 þá er 100 byrjunar nóta.
    float freq = std::pow(1.90366, (float)(op->frequency - 100)/20.0);
    std::cout << "Ayy: " << freq << std::endl;
    float amp = std::pow(1.6, (float)(op->amplitude - 50)/20.0) - 0.3;
    api.sendOperatorValue(op->id, 0, 1, freq, amp);
    //api.sendOperatorValue(op->id, 1, 0, std::pow(1.3, (((op->frequency/200.0*100.0)-50.0)/20.0)), std::pow(1.3, (((op->amplitude/60.0*100.0)-50)/20.0))-0.3);
    //api.sendOperatorValue(op->id, 1, 0, std::pow(1.90366, ((float)op->frequency / 20.0)), std::pow(1.6, (((float)(op->amplitude)-50)/20.0))-0.3);
   // api.sendOperatorValue(op->id, 1, 0, std::pow(1.3, (((op->frequency/20000.0*100.0)-50.0)/20.0)), std::pow(1.3, (((op->amplitude/60.0*100.0)-50)/20.0)));
}

void Controller::sendAllOperatorInfo(int operatorId) {
    const auto& operator_ = getOperatorById(operatorId);
    if (operator_->isCarrier) {
        api.addCarrier(operatorId);
    }

    sendOperator(operatorId);

    for (const auto& modulatorId : operator_->modulatedBy) {
        api.addModulator(operatorId, modulatorId);
        // TODO: Fix possible infinite recursion here
        sendAllOperatorInfo(modulatorId);
    }
}

void Controller::selectOperator(int id) {
    selectedOperatorId_ = {id};
    const auto& operator_ = operators_[selectedOperatorId_.value()];
    emit operatorSelected(operator_.get());
}

void Controller::deselectOperator() {
    selectedOperatorId_ = std::nullopt;
    emit operatorDeselected(true);
}

const Operators &Controller::operators() {
    return operators_;
}

const std::unique_ptr<Operator> &Controller::getOperatorById(int id) {
    return operators_[id];
}

std::optional<int> Controller::selectedOperatorId() {
    return selectedOperatorId_;
}

Operator* Controller::getSelectedOperator() {
    if (selectedOperatorId_.has_value()) {
        const auto& operator_ = operators_[selectedOperatorId_.value()];
        return operator_.get();
    }
    else {
        return nullptr;
    }
}

void Controller::saveOperators(const std::string& name) {
    json json(operators_);

    std::ofstream file("presets/" + name + ".json");
    file << json.dump();
    file.close();
}

void Controller::loadOperators(const std::string& name) {
    operators_ = loadJsonFileAsObject<Operators>("presets/" + name + ".json");

    resetAvailableOperatorIds();
    for (const auto& operator_ : operators_) {
        availableOperatorIds_.erase(operator_.first);
    }
}

void Controller::setOperators(const Operators& operators) {
    removeAllModulators();
    removeAllCarriers();
    operators_.clear();

    for (const auto& operator_ : operators) {
        auto op = std::make_unique<Operator>(*operator_.second);
        operators_.insert(std::make_pair((int) operator_.first, std::move(op)));
    }

    resetAvailableOperatorIds();
    for (const auto& operator_ : operators_) {
        availableOperatorIds_.erase(operator_.first);

        if (operator_.second->isCarrier) {
            sendAllOperatorInfo(operator_.first);
        }
    }
}

void Controller::removeAllModulators() {
    for (const auto& operator_ : operators_) {
        const auto modulatedBy = operator_.second->modulatedBy;

        for (const auto& modulator : modulatedBy) {
            removeModulator(operator_.first, modulator);
        }
    }
}

void Controller::removeAllModulatorsForOperator(int operatorId) {
    const auto modulatedBy = getOperatorById(operatorId)->modulatedBy;

    for (const auto& modulator : modulatedBy) {
        removeModulator(operatorId, modulator);
    }
}

void Controller::removeAllCarriers() {
    for (const auto& operator_ : operators_) {
        if (operator_.second->isCarrier) {
            removeCarrier(operator_.first);
        }
    }
}

void Controller::resetAvailableOperatorIds() {
    availableOperatorIds_.clear();
    for (int i = 0; i < 8; i++) {
        availableOperatorIds_.insert(i);
    }
}

void Controller::hidePresets() {
    showPresets_ = false;
    showPresetsChanged(false);
}
