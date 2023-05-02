//
// Created by Guðmundur on 10-Feb-23.
//

#define USE_INTERSYNTH TRUE

#include <stdexcept>
#include <utility>
#include "Api.h"

const int kSynthPort = 4893;

Api::Api(std::function<void(QTcpSocket::SocketState state)> onConnectionStateChange) : loveCommunicationTcp_(
        std::move(onConnectionStateChange)) {
}

void Api::connect(const QString &ip) {
    loveCommunicationTcp_.connectToServer(ip, kSynthPort);
}

void Api::disconnect() {
    loveCommunicationTcp_.disconnectFromServer();
}

void Api::noteOn(unsigned char key) {
}

void Api::noteOff(unsigned char key) {
}

void Api::sendOperatorValue(unsigned char operator_, unsigned char alg_index, bool attack, float frequency_factor,
                            float amplitude) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.sendOperatorValue(operator_, attack, alg_index, frequency_factor, amplitude);
}

void Api::addModulator(int operatorId, int modulatorId) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.addModulator(operatorId, modulatorId);
}

void Api::removeModulator(int operatorId, int modulatorId) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.removeModulator(operatorId, modulatorId);
}

void Api::addCarrier(int operatorId) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.addCarrier(operatorId);
}

void Api::removeCarrier(int operatorId) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.removeCarrier(operatorId);
}

void Api::setAmpEnvelopeAttackValue(int index, float value, float time) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.setAttackAmpEnvelopePoint(index, value, time);
}

void Api::setAmpEnvelopeSize(int size) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.setAttackAmpEnvelopeSize(size);
}

void Api::setAmpReleaseEnvelopeSize(int size) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.setReleaseAmpEnvelopeSize(size);
}

void Api::setAmpReleaseEnvelopePoint(int index, float value, float time) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.setReleaseAmpEnvelopePoint(index, value, time);
}

void Api::setLfoGlobalOptions(bool enabled, LfoWaveType type, float frequency) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.setLfoGlobalOptions(enabled, type, frequency);
}

void Api::setOperatorLfoValues(int operatorId, float frequencyAmount, float amplitudeAmount) {
    if (!loveCommunicationTcp_.isConnected()) return;

    loveCommunicationTcp_.setOperatorLfoValues(operatorId, frequencyAmount, amplitudeAmount);
}
