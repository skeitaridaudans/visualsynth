//
// Created by Guðmundur on 10-Feb-23.
//

#include <stdexcept>
#include "Api.h"
#ifdef USE_INTERSYNTH
#include "Intersynth/intersynth.h"
#endif

Api::Api() {
#ifdef USE_INTERSYNTH
    intersynth_init();
    bool selected = intersynth_select_port(1);
    if (!selected) throw std::runtime_error("Failed to select port");
#endif
}

void Api::noteOn(unsigned char key) {
#ifdef USE_INTERSYNTH
    intersynth_send_note(key, 120);
#endif
}

void Api::noteOff(unsigned char key) {
#ifdef USE_INTERSYNTH
    intersynth_send_note(key, 0);
#endif
}

void Api::sendOperatorValue(unsigned char operator_, unsigned char alg_index, bool attack, float frequency_factor, float amplitude) {
#ifdef USE_INTERSYNTH
    intersynth_change_operator_values(operator_, alg_index, attack, frequency_factor, amplitude);
#endif
}

void Api::addModulator(int operatorId, int modulatorId) {
#ifdef USE_INTERSYNTH
    intersynth_add_modulator(operatorId, modulatorId);
#endif
}

void Api::removeModulator(int operatorId, int modulatorId) {
#ifdef USE_INTERSYNTH
    intersynth_remove_modulator(operatorId, modulatorId);
#endif
}

void Api::addCarrier(int operatorId) {
#ifdef USE_INTERSYNTH
    intersynth_add_carrier(operatorId);
#endif
}

void Api::removeCarrier(int operatorId) {
#ifdef USE_INTERSYNTH
    intersynth_remove_carrier(operatorId);
#endif
}