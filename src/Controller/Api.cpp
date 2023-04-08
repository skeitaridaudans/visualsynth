//
// Created by Guðmundur on 10-Feb-23.
//

#define USE_INTERSYNTH TRUE

#include <stdexcept>
#include "Api.h"
#ifdef USE_INTERSYNTH
#include "Intersynth/intersynth.h"

#endif

Api::Api() {
#ifdef USE_INTERSYNTH

    loveCommunicationTcp_.connectToServer("10.121.101.239", 4893);

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

void Api::sendOperatorValue(unsigned char operator_, unsigned char alg_index, bool attack, float frequency_factor, float amplitude) {
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
