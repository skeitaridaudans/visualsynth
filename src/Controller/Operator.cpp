//
// Created by Guðmundur on 10-Feb-23.
//

#include "Operator.h"
#include "qcolor.h"
#include <iostream>



Operator::Operator(int id, QObject* parent)
    : id(id), QObject(parent), frequency(10), amplitude(20), isCarrier(false), isModulator(false) {}

void Operator::setFrequency(long step) {
    if(((this->frequency + step) <= 100) && ((this->frequency + step) > 0)){
        this->frequency = this->frequency+step;
    }
}

void Operator::setAmplitude(long step) {
    if(((this->amplitude + step) <= 100) && ((this->amplitude + step) >= 0)){
        this->amplitude = this->amplitude+step;
    }
}

long Operator::getAmp() {
    return this->amplitude;
}

long Operator::getFreq() {
    return this->frequency;
}


QColor Operator::getColorForOperator() {
    return QColor((int) ((log10((double) this->frequency) / 4.38) * 255.0), 20, (int) ((log10((double) this->amplitude) / 4.38) * 255.0));
}
