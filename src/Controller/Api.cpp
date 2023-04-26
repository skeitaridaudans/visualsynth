//
// Created by Guðmundur on 10-Feb-23.
//

#define USE_INTERSYNTH TRUE

#include <stdexcept>
#include <utility>
#include "Api.h"

#ifdef USE_INTERSYNTH

#include "Intersynth/intersynth.h"

#endif

Api::Api(std::function<void(QTcpSocket::SocketState state)> onConnectionStateChange) : loveCommunicationTcp_(
        std::move(onConnectionStateChange)) {
#ifdef USE_INTERSYNTH
    //loveCommunicationTcp_.connectToServer("10.121.101.205", 4893); // tengjast við syntha
    loveCommunicationTcp_.connectToServer("127.0.0.1", 4893); // tengjast locally

    loveCommunicationTcp_.removeCarrier(0); // All operators removed in the beginning
    loveCommunicationTcp_.removeCarrier(1); // All operators removed in the beginning
    loveCommunicationTcp_.removeCarrier(2); // All operators removed in the beginning
    loveCommunicationTcp_.removeCarrier(3); // All operators removed in the beginning
    loveCommunicationTcp_.removeCarrier(4); // All operators removed in the beginning
    loveCommunicationTcp_.removeCarrier(5); // All operators removed in the beginning
    loveCommunicationTcp_.removeCarrier(6); // All operators removed in the beginning
    loveCommunicationTcp_.removeCarrier(7); // All operators removed in the beginning


#endif
}

void Api::noteOn(unsigned char key) {
#ifdef USE_INTERSYNTH

    //loveCommunicationTcp_.noteOn(key, 120);
    //loveCommunicationTcp_.noteOn(120);

#endif
}

void Api::noteOff(unsigned char key) {
#ifdef USE_INTERSYNTH
    //loveCommunicationTcp_.noteOff(key, 0);
    //loveCommunicationTcp_.noteOff(0);
#endif
}

void Api::sendOperatorValue(unsigned char operator_, unsigned char alg_index, bool attack, float frequency_factor,
                            float amplitude) {
#ifdef USE_INTERSYNTH
    loveCommunicationTcp_.sendOperatorValue(operator_, attack, alg_index, frequency_factor, amplitude);
#endif
}

void Api::addModulator(int operatorId, int modulatorId) {
#ifdef USE_INTERSYNTH
    loveCommunicationTcp_.addModulator(operatorId, modulatorId);
#endif
}

void Api::removeModulator(int operatorId, int modulatorId) {
#ifdef USE_INTERSYNTH
    loveCommunicationTcp_.removeModulator(operatorId, modulatorId);
#endif
}

void Api::addCarrier(int operatorId) {
#ifdef USE_INTERSYNTH

    loveCommunicationTcp_.addCarrier(operatorId);
#endif
}

void Api::removeCarrier(int operatorId) {
#ifdef USE_INTERSYNTH
    //intersynth_remove_carrier(operatorId);
    loveCommunicationTcp_.removeCarrier(operatorId);

#endif
}

void Api::setAmpEnvelopeAttackValue(int index, float value, float time) {
#ifdef USE_INTERSYNTH
    loveCommunicationTcp_.setAttackAmpEnvelopePoint(index, value, time);
#endif
}

void Api::setAmpEnvelopeSize(int size) {
#ifdef USE_INTERSYNTH
    loveCommunicationTcp_.setAttackAmpEnvelopeSize(size);
#endif
}

void Api::setAmpReleaseEnvelopeSize(int size) {
#ifdef USE_INTERSYNTH
    loveCommunicationTcp_.setReleaseAmpEnvelopeSize(size);
#endif
}

void Api::setAmpReleaseEnvelopePoint(int index, float value, float time) {
#ifdef USE_INTERSYNTH
    loveCommunicationTcp_.setReleaseAmpEnvelopePoint(index, value, time);
#endif
}

