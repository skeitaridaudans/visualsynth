//
// Created by star on 13.2.2023.
//

#ifndef MIDI_SYNTH_FLOATING_FIX_2_H
#define MIDI_SYNTH_FLOATING_FIX_2_H
//
// Created by star on 13.2.2023.
//

// function to store a floating point value in 5 bytes with each byte containing 7 bits
void store_float_in_buffer(unsigned char *buffer, float value) {
    uint32_t float_bits = *((uint32_t*) &value);  // get the bits of the floating point value
    buffer[0] = (char) (float_bits & 0x7F);
    buffer[1] = (char) ((float_bits >> 7) & 0x7F);
    buffer[2] = (char) ((float_bits >> 14) & 0x7F);
    buffer[3] = (char) ((float_bits >> 21) & 0x7F);
    buffer[4] = (char) ((float_bits >> 28) & 0x7F);
}

// function to reconstruct a floating point value from a 5-byte buffer with each byte containing 7 bits
float reconstruct_float_from_buffer(const unsigned char *buffer) {
    uint32_t float_bits = ((uint32_t) buffer[0] & 0x7F) |
                          (((uint32_t) buffer[1] & 0x7F) << 7) |
                          (((uint32_t) buffer[2] & 0x7F) << 14) |
                          (((uint32_t) buffer[3] & 0x7F) << 21) |
                          (((uint32_t) buffer[4] & 0x7F) << 28);
    return *((float*) &float_bits);  // convert the bits back to a floating point value
}


#endif //MIDI_SYNTH_FLOATING_FIX_2_H