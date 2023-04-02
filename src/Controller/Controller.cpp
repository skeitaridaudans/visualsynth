#include "Controller.h"
#include <iostream>
#include "src/Alert/AlertController.h"
#include <QDebug>
#include <QTimer>

const int kMaxNumberOfOperators = 8;

std::unique_ptr<Controller> Controller::instance = std::make_unique<Controller>();

Controller::Controller(QObject *parent) : QObject(parent){
    for (int i = 0; i < 8; i++) {
        availableOperatorIds_.insert(i);
    }
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
    operators_.erase(operatorId);
    availableOperatorIds_.insert(operatorId);

    // Remove operator from all modulatedBy lists
    for (const auto& op : operators_) {
        if (std::find(op.second->modulatedBy.begin(), op.second->modulatedBy.end(), operatorId) != op.second->modulatedBy.end()) {
            op.second->modulatedBy.erase(std::remove(op.second->modulatedBy.begin(), op.second->modulatedBy.end(), operatorId),op.second->modulatedBy.end());
        }
    }
}

void Controller::noteOn(int note) {
    api.noteOn(note);
}

void Controller::noteOff(int note) {
    api.noteOff(note);
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

    api.addModulator(operatorId, modulatorId);
}

void Controller::removeModulator(int operatorId, int modulatorId) {
    const auto& op = operators_[operatorId];
    const auto& mod = operators_[modulatorId];

    op->modulatedBy.erase(std::remove(op->modulatedBy.begin(), op->modulatedBy.end(), modulatorId), op->modulatedBy.end());

    // Check if modulator is modulating any other operator before setting isModulator to false
    for (const auto& op_ : operators_) {
        if (std::find(op_.second->modulatedBy.begin(), op_.second->modulatedBy.end(), modulatorId) == op_.second->modulatedBy.end()) {
            return;
        }
    }
    mod->isModulator = false;

    api.removeModulator(operatorId, modulatorId);
}

void Controller::addCarrier(int operatorId) {
    if (operators_[operatorId]->isCarrier) return;

    operators_[operatorId]->isCarrier = true;
    api.addCarrier(operatorId);
}

void Controller::removeCarrier(int operatorId) {
    if (!operators_[operatorId]->isCarrier) return;

    operators_[operatorId]->isCarrier = false;
    api.removeCarrier(operatorId);
}

void Controller::sendOperator(int operatorId) {
    const auto& op = operators_[operatorId];
    std::cout << op->amplitude << std::endl;
    api.sendOperatorValue(op->id, op->frequency, op->amplitude, true, 0);
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

const std::unordered_map<int, std::unique_ptr<Operator>> &Controller::operators() {
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
