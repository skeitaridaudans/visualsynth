//
// Created by Guðmundur on 10-Feb-23.
//

#include "Operator.h"
#include "qcolor.h"
#include <iostream>
#include <utility>

// Operator view state

OperatorViewState::OperatorViewState() {

}

OperatorViewState::OperatorViewState(const OperatorViewState &operatorViewState) : draggingState(
        operatorViewState.draggingState),
                                                                                   initialDragCursorPos(
                                                                                           operatorViewState.initialDragCursorPos) {

}

// Operator

Operator::Operator(int id, QObject *parent)
        : id(id), QObject(parent), frequency(100), amplitude(60), isCarrier(false), isModulator(false) {}

Operator::Operator()
        : id(0), QObject(), frequency(100), amplitude(60), isCarrier(false), isModulator(false) {}

Operator::Operator(const Operator &operator_)
        : id(operator_.id), frequency(operator_.frequency), amplitude(operator_.amplitude),
          isModulator(operator_.isModulator), isCarrier(operator_.isCarrier), modulatedBy(operator_.modulatedBy),
          position(operator_.position), operatorViewState(operator_.operatorViewState) {}

void Operator::setFrequency(long step) {
    if (((this->frequency + step) <= 200) && ((this->frequency + step) > 0)) {
        this->frequency = this->frequency + step;
    }
}

void Operator::setAmplitude(long step) {
    if (((this->amplitude + step) <= 100) && ((this->amplitude + step) >= 0)) {
        this->amplitude = this->amplitude + step;
    }
}

long Operator::getAmp() {
    return this->amplitude;
}

long Operator::getFreq() {
    return this->frequency;
}


QColor Operator::getColorForOperator() const {
    return QColor((int) ((log10((double) this->frequency) / 4.38) * 255.0), 20,
                  (int) ((log10((double) this->amplitude) / 4.38) * 255.0));
}

Operator::Operator(int id, long frequency, long amplitude, bool isModulator, bool isCarrier,
                   std::vector<int> modulatedBy, QPointF position, QObject *parent)
        : id(id), frequency(frequency), amplitude(amplitude), isModulator(isModulator), isCarrier(isCarrier),
          modulatedBy(std::move(modulatedBy)), position(position) {

}

void to_json(json &j, const Operator &o) {
    j = json{
            {"id",          o.id},
            {"frequency",   o.frequency},
            {"amplitude",   o.amplitude},
            {"isModulator", o.isModulator},
            {"isCarrier",   o.isCarrier},
            {"modulatedBy", o.modulatedBy},
            {"position",    {
                                    {"x", o.position.x()},
                                    {"y", o.position.y()}
                            }}
    };
}

void from_json(const json &j, Operator &o) {
    j["id"].get_to(o.id);
    j["frequency"].get_to(o.frequency);
    j["amplitude"].get_to(o.amplitude);
    j["isModulator"].get_to(o.isModulator);
    j["isCarrier"].get_to(o.isCarrier);
    j["modulatedBy"].get_to(o.modulatedBy);
    o.position.setX(j["position"]["x"].get<float>());
    o.position.setY(j["position"]["y"].get<float>());
}

