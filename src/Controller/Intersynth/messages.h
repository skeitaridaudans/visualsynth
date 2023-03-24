//
// Created by gudmundur on 24.3.2023.
//

#ifndef MAIN_QML_MESSAGES_H
#define MAIN_QML_MESSAGES_H

#define MIDI_NOTE_MESSAGE_SIZE 3

#define OPERATOR_VALUES 0x10
#define EXPRESSIVE_CHANGE 0x20  // is applied to single operator throughout all envelopes
#define AMP_ENVELOPE_VALUE 0x30
#define AMP_ATTACK 0x01
#define AMP_RELEASE 0x02
#define MODULATED_BY 0x40
#define MODULATOR_ON 0x10
#define MODULATOR_OFF 0x00
#define CARRIER_ON 0x50
#define CARRIER_OFF 0x60
#define CLEAR_OPERATORS 0x70
#define ALGORITHM_ENVELOPE_INFO 0x80
#define INSERT 0x01
#define REMOVE 0x02
#define ENVELOPE_SIZE 0x03
#define UPDATE_TIME 0x04
#define SOLO_POINT 0x05
#define FREQ_SYNCED_BY 0x90
#define AMP_FIXED 0xA0
#define IGNORE_ENVELOPE 0x01
#define IGNORE_VELOCITY 0x02
#define NOTE_INFO 0xB0
#define NOTE_ON 0x01
#define NOTE_OFF 0x02
#define NOTE_UPDATE 0x03
#define MIDI_CHANNEL 0xC0
#define CHANGE_BY_PARAMETER 0xD0
#define SET_FREQ_PER_T 0x02
#define SET_AMPS_PER_T 0x03
#define LINEAR_T 0x04
#define FREQ_T 0x05
#define AMP_T 0x06

#endif //MAIN_QML_MESSAGES_H
