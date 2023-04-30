#include "Controller.h"
#include <iostream>
#include "src/Alert/AlertController.h"
#include <QDebug>
#include <QTimer>
#include <fstream>
#include <utility>
#include <QQmlEngine>
#include <QQmlContext>
#include "src/Utils/Utils.h"
#include "src/Dialog/DialogController.h"

const int kMaxNumberOfOperators = 8;
const double kGraphicsFrequencyFactor = 160.0;
const QString kSynthIpSettingsKey = "synth_ip";
const QString kCompanyName = "Love Synthesizer";
const QString kApplicationName = "Visualsynth";

std::unique_ptr<Controller> Controller::instance = std::make_unique<Controller>();

Controller::Controller(QObject *parent) : QObject(parent), settings_(kCompanyName, kApplicationName) {
    api = std::make_unique<Api>([this](auto state) {
        onConnectionStateChanged(state);
    });

    resetAvailableOperatorIds();
    loadInitialPreset();

    if (settings_.contains(kSynthIpSettingsKey)) {
        synthIp_ = settings_.value(kSynthIpSettingsKey).toString();
        api->connect(synthIp_);
    }
    else {
        // TODO: Fix crash here (at least on Android)
        // showConnectDialog();
    }
}

void Controller::loadInitialPreset() {
    Operators defaultOperators;
    defaultOperators.insert(
            std::make_pair<int, Operator>(1, Operator(1, 100, 60, false, true, {0}, QPointF(0.4696, 0.894))));
    defaultOperators.insert(
            std::make_pair<int, Operator>(0, Operator(0, 70, 30, true, false, {}, QPointF(0.3251, 0.6075))));

    QString defaultname = "default";

    // TODO: Configure these values better
    const std::vector<AmpEnvValue> defaultAttackAmpEnv = {
            AmpEnvValue(0, 0.0, 0.0, true),
            AmpEnvValue(1, 0.8, 0.25, true),
            AmpEnvValue(2, 0.5, 0.5, true),
            AmpEnvValue(3, 0.5, 1.0, true),
    };

    const std::vector<AmpEnvValue> defaultReleaseAmpEnv {
            AmpEnvValue(0, 0.5, 0.0, false),
            AmpEnvValue(1, 0, 1, false),
    };

    const Preset defaultPreset = Preset(defaultOperators, defaultAttackAmpEnv, defaultReleaseAmpEnv, defaultname);
    changeToPreset(defaultPreset);
}

std::optional<int> Controller::addOperator() {
    if (operators_.size() >= kMaxNumberOfOperators) {
        AlertController::instance->showAlert("Error: Only 8 operators allowed", 1);
        return std::nullopt;
    }

    int id = *std::min_element(availableOperatorIds_.begin(), availableOperatorIds_.end());
    availableOperatorIds_.erase(id);

    auto op = Operator(id);
    operators_.insert(std::make_pair<int, Operator>((int) id, std::move(op)));

    return id;
}

void Controller::removeOperator(int operatorId) {
    removeAllModulatorsForOperator(operatorId);
    removeCarrier(operatorId);

    operators_.erase(operatorId);
    availableOperatorIds_.insert(operatorId);

    // Remove operator from all modulatedBy lists
    for (auto &op: operators_) {
        if (std::find(op.second.modulatedBy.begin(), op.second.modulatedBy.end(), operatorId) !=
            op.second.modulatedBy.end()) {
            op.second.modulatedBy.erase(
                    std::remove(op.second.modulatedBy.begin(), op.second.modulatedBy.end(), operatorId),
                    op.second.modulatedBy.end());
            api->removeModulator(op.second.id, operatorId);
        }
    }
}

void Controller::noteOn(int note) {
    api->noteOn(note);
}

void Controller::noteOff(int note) {
    api->noteOff(note);
}

void Controller::setAmpEnvelopeSize(int size) {
    api->setAmpEnvelopeSize(size);
}

void Controller::setAttackAmpEnvelopePoint(int index, float value, float time) {
    // It doesn't make sense to add a amp envelope point that is more than 2 spots ahead of any current point
    if (attackAmpEnvValues_.size() < index) {
        return;
    }

    api->setAmpEnvelopeAttackValue(index, value, time);

    const auto ampEnvValue = AmpEnvValue(index, value, time, true);
    if (attackAmpEnvValues_.size() == index) {
        attackAmpEnvValues_.emplace_back(ampEnvValue);
    }
    else {
        attackAmpEnvValues_[index] = ampEnvValue;
    }
}

void Controller::setReleaseAmpEnvelopePoint(int index, float value, float time) {
    // It doesn't make sense to add a amp envelope point that is more than 2 spots ahead of any current point
    if (releaseAmpEnvValues_.size() < index) {
        return;
    }

    api->setAmpReleaseEnvelopePoint(index, value, time);

    const auto ampEnvValue = AmpEnvValue(index, value, time, false);
    if (releaseAmpEnvValues_.size() == index) {
        releaseAmpEnvValues_.emplace_back(ampEnvValue);
    }
    else {
        releaseAmpEnvValues_[index] = ampEnvValue;
    }

}

void Controller::changeFrequency(int operatorId, float frequency) {
    operators_[operatorId].frequency = frequency;
    emit freqChanged(frequency);
    sendOperator(operatorId);
}

void Controller::changeAmplitude(int operatorId, long amplitude) {
    operators_[operatorId].amplitude = amplitude;
    emit ampChanged(amplitude);
    sendOperator(operatorId);
}

void Controller::setOperatorLfoFrequency(int operatorId, long amount) {
    auto &operator_ = getOperatorById(operatorId);
    operator_.frequencyLfoAmount = amount;

    sendOperatorLfoValuesToSynth(operatorId);
}

void Controller::setOperatorLfoAmplitude(int operatorId, long amount) {
    auto &operator_ = getOperatorById(operatorId);
    operator_.amplitudeLfoAmount = amount;

    sendOperatorLfoValuesToSynth(operatorId);
}

void Controller::sendOperatorLfoValuesToSynth(int operatorId) {
    const auto &operator_ = getOperatorById(operatorId);

    const auto frequencyAmount = static_cast<float>(operator_.frequencyLfoAmount) / 100.0f;
    const auto amplitudeAmount = static_cast<float>(operator_.amplitudeLfoAmount) / 100.0f;
    api->setOperatorLfoValues(operatorId, frequencyAmount, amplitudeAmount);
}

void Controller::setLfoEnabled(bool enabled) {
    isLfoEnabled_ = enabled;
    isLfoEnabledChanged(enabled);
    sendLfoGlobalOptionsToSynth();
}

void Controller::setLfoWaveType(LfoWaveType lfoWaveType) {
    lfoWaveType_ = lfoWaveType;
    lfoWaveTypeChanged(lfoWaveType);
    sendLfoGlobalOptionsToSynth();
}

void Controller::setLfoFrequency(long frequency) {
    lfoFrequency_ = frequency;
    lfoFrequencyChanged(frequency);
    sendLfoGlobalOptionsToSynth();
}

void Controller::sendLfoGlobalOptionsToSynth() {
    const float frequency = static_cast<float>(lfoFrequency_) / 10.0f;
    api->setLfoGlobalOptions(isLfoEnabled_, lfoWaveType_, frequency);
}

void Controller::addModulator(int operatorId, int modulatorId) {
    auto &op = operators_[operatorId];
    auto &mod = operators_[modulatorId];

    op.modulatedBy.push_back(modulatorId);
    mod.isModulator = true;

    sendAllOperatorInfo(operatorId);
}

void Controller::removeModulator(int operatorId, int modulatorId) {
    auto &op = operators_[operatorId];
    auto &mod = operators_[modulatorId];

    op.modulatedBy.erase(std::remove(op.modulatedBy.begin(), op.modulatedBy.end(), modulatorId), op.modulatedBy.end());
    api->removeModulator(operatorId, modulatorId);

    // Check if modulator is modulating any other operator before setting isModulator to false
    for (const auto &op_: operators_) {
        if (std::find(op_.second.modulatedBy.begin(), op_.second.modulatedBy.end(), modulatorId) ==
            op_.second.modulatedBy.end()) {
            return;
        }
    }
    mod.isModulator = false;
}

void Controller::addCarrier(int operatorId) {
    if (operators_[operatorId].isCarrier) return;

    operators_[operatorId].isCarrier = true;
    sendAllOperatorInfo(operatorId);
}

void Controller::removeCarrier(int operatorId) {
    if (!operators_[operatorId].isCarrier) return;

    operators_[operatorId].isCarrier = false;
    api->removeCarrier(operatorId);
}

void Controller::sendOperator(int operatorId) {
    const auto &op = operators_[operatorId];
    // Núverandi range 0 - 200 20 er lægsta nóta sem heyrist hæsta er 175 þá er 100 byrjunar nóta.

    float freq = std::pow(1.90366, (float) (op.frequency - 100) / 20.0);
    //float freq = (op.frequency - 1 + 0.001)/(100-1);
    float amp = std::pow(1.6, (float) (op.amplitude - 50) / 20.0) - 0.3;
    api->sendOperatorValue(op.id, 0, 1, freq, amp);

    //api.sendOperatorValue(op->id, 1, 0, std::pow(1.3, (((op->frequency/200.0*100.0)-50.0)/20.0)), std::pow(1.3, (((op->amplitude/60.0*100.0)-50)/20.0))-0.3);
    //api.sendOperatorValue(op->id, 1, 0, std::pow(1.90366, ((float)op->frequency / 20.0)), std::pow(1.6, (((float)(op->amplitude)-50)/20.0))-0.3);
    // api.sendOperatorValue(op->id, 1, 0, std::pow(1.3, (((op->frequency/20000.0*100.0)-50.0)/20.0)), std::pow(1.3, (((op->amplitude/60.0*100.0)-50)/20.0)));
}

void Controller::sendAllOperatorInfo(int operatorId, std::unordered_set<int> *visited) {
    auto visited_ = visited == nullptr ? new std::unordered_set<int>() : visited;

    // Operator has been visited before
    if (visited_->count(operatorId) != 0) {
        return;
    }
    visited_->insert(operatorId);

    const auto &operator_ = getOperatorById(operatorId);
    if (operator_.isCarrier) {
        api->addCarrier(operatorId);
    }

    sendOperator(operatorId);

    for (const auto &modulatorId: operator_.modulatedBy) {
        api->addModulator(operatorId, modulatorId);

        sendAllOperatorInfo(modulatorId, visited_);
    }

    // If this is the initial call
    if (visited == nullptr) {
        delete visited_;
    }
}

void Controller::selectOperator(int id) {
    selectedOperatorId_ = {id};
    auto &operator_ = operators_[selectedOperatorId_.value()];
    emit operatorSelected(&operator_);
}

void Controller::deselectOperator() {
    selectedOperatorId_ = std::nullopt;
    emit operatorDeselected(true);
}

Operators &Controller::operators() {
    return operators_;
}

const std::vector<AmpEnvValue>& Controller::attackAmpEnvValues() {
    return attackAmpEnvValues_;
}

const std::vector<AmpEnvValue>& Controller::releaseAmpEnvValues() {
    return releaseAmpEnvValues_;
}

Operator &Controller::getOperatorById(int id) {
    return operators_[id];
}

std::optional<int> Controller::selectedOperatorId() {
    return selectedOperatorId_;
}

Operator *Controller::getSelectedOperator() {
    if (selectedOperatorId_.has_value()) {
        auto &operator_ = operators_[selectedOperatorId_.value()];
        return &operator_;
    } else {
        return nullptr;
    }
}

void Controller::savePreset(const std::string &name) {
    const auto attackAmpEnvValues = attackAmpEnvValues_;
    const auto releaseAmpEnvValues = releaseAmpEnvValues_;
    json json(Preset(operators_, attackAmpEnvValues, releaseAmpEnvValues, QString::fromStdString(name)));

    std::ofstream file("presets/" + name + ".json");
    file << json.dump();
    file.close();


    //Alert that a preset has been saved
    QString str = QString("the preset '%1' has been saved to presets ").arg(QString::fromStdString(name));
    AlertController::instance->showAlert(str, 0);
}

void Controller::loadPreset(const std::string &name) {
    auto preset = loadJsonFileAsObject<Preset>("presets/" + name + ".json");

    resetAvailableOperatorIds();
    for (const auto &operator_: operators_) {
        availableOperatorIds_.erase(operator_.first);
    }
}

void Controller::changeToPreset(const Preset &preset) {
    removeAllModulators();
    removeAllCarriers();
    operators_ = preset.operators;

    resetAvailableOperatorIds();
    for (const auto &operator_: operators_) {
        availableOperatorIds_.erase(operator_.first);

        if (operator_.second.isCarrier) {
            sendAllOperatorInfo(operator_.first);
        }
    }

    for (const auto &ampEnvValue: preset.attackAmpEnvValues) {
        setAttackAmpEnvelopePoint(ampEnvValue.index, ampEnvValue.value, ampEnvValue.time);
    }
    for (const auto &ampEnvValue : preset.releaseAmpEnvValues) {
        setReleaseAmpEnvelopePoint(ampEnvValue.index, ampEnvValue.value, ampEnvValue.time);
    }

    // This is to avoid the program crashing when loading the first preset.
    if (!isFirst_) {
        //Alert that a preset has been loaded
        QString str = QString("the preset '%1' has been loaded!").arg(preset.name);
        AlertController::instance->showAlert(str, 0);
    } else {
        isFirst_ = false;
    }
}

void Controller::removeAllModulators() {
    for (const auto &operator_: operators_) {
        const auto modulatedBy = operator_.second.modulatedBy;

        for (const auto &modulator: modulatedBy) {
            removeModulator(operator_.first, modulator);
        }
    }
}

void Controller::removeAllModulatorsForOperator(int operatorId) {
    const auto modulatedBy = getOperatorById(operatorId).modulatedBy;

    for (const auto &modulator: modulatedBy) {
        removeModulator(operatorId, modulator);
    }
}

void Controller::removeAllCarriers() {
    for (const auto &operator_: operators_) {
        if (operator_.second.isCarrier) {
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
    if(showPresets_ == true){
        showPresets_ = false;
        emit showPresetsChanged(false);
    }
}

double Controller::getSingleOperatorValue(int operatorId, double offset) {
    auto &operator_ = getOperatorById(operatorId);

    double frequency = std::pow(1.90366, (double) (operator_.frequency - 100) / 20.0);
    double amplitude = std::pow(1.6, (double) (operator_.amplitude - 50) / 20.0) - 0.3;
    return sin(-(double) (offset) * M_PI * 2 * frequency / kGraphicsFrequencyFactor) * amplitude;
}

double Controller::getOperatorModulationValue(int operatorId, double offset) {
    auto &operator_ = getOperatorById(operatorId);

    operator_.visitedCount++;

    double modulationSum = 0.0;
    for (const auto modulatorId: operator_.modulatedBy) {
        const auto &modulator = getOperatorById(modulatorId);
        if (modulator.visitedCount < 2) {
            modulationSum += getOperatorModulationValue(modulatorId, offset);
        }
    }
    operator_.visitedCount--;

    double frequency = std::pow(1.90366, (double) (operator_.frequency - 100) / 20.0);
    double amplitude = std::pow(1.6, (double) (operator_.amplitude - 50) / 20.0) - 0.3;
    return sin(modulationSum - (double) (offset) * M_PI * 2 * frequency / kGraphicsFrequencyFactor) * amplitude;
}

double Controller::getCarrierOutput(double offset) {
    double totalSample = 0.0;

    for (const auto &operator_: operators_) {
        if (operator_.second.isCarrier) {
            totalSample += getOperatorModulationValue(operator_.first, offset);
        }
    }

    return totalSample;
}

void Controller::showConnectDialog() {
    DialogController::instance->showDialog(
            "Connect to synthesizer",
            "Type the IP address of the synthesizer that you want to connect to",
            synthIp_, "IP address", "Connect", "Cancel",
            [this](const auto &ip) {
                if (ip.isEmpty()) {
                    AlertController::instance->showAlert("Invalid IP address", true);
                    return false;
                }

                synthIp_ = ip;
                settings_.setValue(kSynthIpSettingsKey, ip);
                api->connect(synthIp_);
                return true;
            });
}

void Controller::disconnect() {
    api->disconnect();
}

void Controller::onConnectionStateChanged(QTcpSocket::SocketState state) {
    if (isConnecting_ && state == QTcpSocket::SocketState::UnconnectedState) {
        AlertController::instance->showAlert("Failed to connect to synth", true);
    }

    isConnecting_ = state == QTcpSocket::SocketState::ConnectingState;
    isConnectingChanged(isConnecting_);

    isConnected_ = state == QTcpSocket::SocketState::ConnectedState;
    isConnectedChanged(isConnected_);

    if (isConnected_) {
        onConnected();
        setConnectionStateText("Connected");
    }
    else if (isConnecting_) {
        setConnectionStateText("Connecting");
    }
    else {
        setConnectionStateText("Not connected");
    }
}

void Controller::onConnected() {
    // Remove all operators when we connect
    for (int i = 0; i < 8; i++) {
        api->removeCarrier(i);
    }

    // Send all operators that have been created in the UI before connecting
    for (const auto &operator_: operators_) {
        if (operator_.second.isCarrier) {
            sendAllOperatorInfo(operator_.first);
        }
    }

    sendAmpEnvelopeToSynth();
}

void Controller::setConnectionStateText(QString connectionStateText) {
    connectionStateText_ = std::move(connectionStateText);
    connectionStateTextChanged(connectionStateText_);
}

void Controller::sendAmpEnvelopeToSynth() {
    // For some reason, the synth always creates n+1 points when you set size to n
    api->setAmpEnvelopeSize(attackAmpEnvValues_.size() - 1);
    for (const auto &ampEnvValue : attackAmpEnvValues_) {
        api->setAmpEnvelopeAttackValue(ampEnvValue.index, ampEnvValue.value, ampEnvValue.time);
    }

    api->setAmpReleaseEnvelopeSize(releaseAmpEnvValues_.size());
    for (const auto &ampEnvValue : releaseAmpEnvValues_) {
        api->setAmpReleaseEnvelopePoint(ampEnvValue.index, ampEnvValue.value, ampEnvValue.time);
    }
}
