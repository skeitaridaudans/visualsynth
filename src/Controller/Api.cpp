//
// Created by Guðmundur on 10-Feb-23.
//

#include <stdexcept>
#include "Api.h"
#include "Intersynth/intersynth.h"

Api::Api() {
    intersynth_init();
    bool selected = intersynth_select_port(1);
    if (!selected) throw std::runtime_error("Failed to select port");
}

void Api::sendOperatorValue(bool attack, int envelopeId, int operatorId, float frequency, float amplitude) {

}

void Api::noteOn(unsigned char key) {
    intersynth_send_note(key, 120);
}

void Api::noteOff(unsigned char key) {
    intersynth_send_note(key, 0);
}
