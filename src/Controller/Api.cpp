//
// Created by Guðmundur on 10-Feb-23.
//

#define USE_INTERSYNTH TRUE

#include <stdexcept>
#include <utility>
#include "Api.h"

Api::Api(std::function<void(QTcpSocket::SocketState state)> onConnectionStateChange) : loveCommunicationTcp_(
        std::move(onConnectionStateChange)) {
    loveCommunicationTcp_.connectToServer("10.121.101.205", 4893); // tengjast við syntha
    //loveCommunicationTcp_.connectToServer("127.0.0.1", 4893); // tengjast locally

    // All operators removed in the beginning
    for (int i = 0; i < 8; i++) {
        loveCommunicationTcp_.removeCarrier(i);
    }
}

void Api::noteOn(unsigned char key) {
}

void Api::noteOff(unsigned char key) {
}

void Api::sendOperatorValue(unsigned char operator_, unsigned char alg_index, bool attack, float frequency_factor,
                            float amplitude) {
    loveCommunicationTcp_.sendOperatorValue(operator_, attack, alg_index, frequency_factor, amplitude);
}

void Api::addModulator(int operatorId, int modulatorId) {
    loveCommunicationTcp_.addModulator(operatorId, modulatorId);
}

void Api::removeModulator(int operatorId, int modulatorId) {
    loveCommunicationTcp_.removeModulator(operatorId, modulatorId);
}

void Api::addCarrier(int operatorId) {
    loveCommunicationTcp_.addCarrier(operatorId);
}

void Api::removeCarrier(int operatorId) {
    loveCommunicationTcp_.removeCarrier(operatorId);
}

void Api::setAmpEnvelopeAttackValue(int index, float value, float time) {
    loveCommunicationTcp_.setAttackAmpEnvelopePoint(index, value, time);
}

void Api::setAmpEnvelopeSize(int size) {
    loveCommunicationTcp_.setAttackAmpEnvelopeSize(size);
}

void Api::setAmpReleaseEnvelopeSize(int size) {
    loveCommunicationTcp_.setReleaseAmpEnvelopeSize(size);
}

void Api::setAmpReleaseEnvelopePoint(int index, float value, float time) {
    loveCommunicationTcp_.setReleaseAmpEnvelopePoint(index, value, time);
}

void Api::setLfoGlobalOptions(bool enabled, LfoWaveType type, float frequency) {
    loveCommunicationTcp_.setLfoGlobalOptions(enabled, type, frequency);
}

void Api::setOperatorLfoValues(int operatorId, float frequencyAmount, float amplitudeAmount) {
    loveCommunicationTcp_.setOperatorLfoValues(operatorId, frequencyAmount, amplitudeAmount);
}