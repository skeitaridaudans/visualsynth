//
// Created by Guðmundur on 10-Feb-23.
//

#include "Operator.h"

Operator::Operator(int id, QObject* parent)
    : id(id), QObject(parent), frequency(440), amplitude(60), isCarrier(false), isModulator(false) {}

void Operator::setFrequency(long step) {
    if(((this->frequency + step) < 20000) && ((this->amplitude + step) > 0)){
        this->frequency = this->frequency+step;
    }
}

void Operator::setAmplitude(long step) {

    if(((this->amplitude + step) < 20) && ((this->amplitude + step) > -20)){
        this->amplitude = this->amplitude+step;
    }
}

long Operator::getAmp() {
    return this->amplitude;
}

long Operator::getFreq() {
    return this->frequency;
}
