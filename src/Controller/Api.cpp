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

void Api::sendOperatorValue(bool attack, int envelopeId, int operatorId, float frequency, float amplitude) {

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
