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
        : id(id), QObject(parent), frequency(100), amplitude(60), isCarrier(false), isModulator(false), currentSemiTone(0) {}

Operator::Operator()
        : id(0), QObject(), frequency(100), amplitude(60), isCarrier(false), isModulator(false), currentSemiTone(0) {}

Operator::Operator(const Operator &operator_)
        : id(operator_.id), frequency(operator_.frequency), amplitude(operator_.amplitude),
          isModulator(operator_.isModulator), isCarrier(operator_.isCarrier), modulatedBy(operator_.modulatedBy),
          position(operator_.position), operatorViewState(operator_.operatorViewState),
          frequencyLfoAmount(operator_.frequencyLfoAmount), amplitudeLfoAmount(operator_.amplitudeLfoAmount),
          attackEnvValues(operator_.attackEnvValues), releaseEnvValues(operator_.releaseEnvValues), currentSemiTone(operator_.currentSemiTone) {}

Operator &Operator::operator=(const Operator &operator_) {
    this->id = operator_.id;
    this->frequency = operator_.frequency;
    this->amplitude = operator_.amplitude;
    this->isModulator = operator_.isModulator;
    this->isCarrier = operator_.isCarrier;
    this->modulatedBy = operator_.modulatedBy;
    this->position = operator_.position;
    this->operatorViewState = operator_.operatorViewState;
    this->frequencyLfoAmount = operator_.frequencyLfoAmount;
    this->amplitudeLfoAmount = operator_.amplitudeLfoAmount;
    this->attackEnvValues = operator_.attackEnvValues;
    this->releaseEnvValues = operator_.releaseEnvValues;

    return *this;
}

void Operator::setFrequency(float step) {
    if (((this->frequency + step) <= 200) && ((this->frequency + step) > 0.9)) {
        this->frequency = this->frequency + step;
    }
}

void Operator::setSemiTone(float value){
    this->currentSemiTone = value;
}

float Operator::getSemiTone() {
    return this->currentSemiTone;
}

void Operator::updateFrequency(float frequency){
    this->frequency = frequency;
}

void Operator::setAmplitude(long step) {
    if(((this->amplitude + step) <= 200) && ((this->amplitude + step) >= 0)){
        this->amplitude = this->amplitude+step;
    }
}

long Operator::getAmp() {
    return this->amplitude;
}

float Operator::getFreq() {
    return this->frequency;
}


QColor Operator::getColorForOperator() const {
    return {
        (int) (this->frequency / 200.0 * 240.0),
        20,
        (int) (this->amplitude / 200.0 * 240.0)
    };
}

Operator::Operator(int id, float frequency, long amplitude, bool isModulator, bool isCarrier,
                   std::vector<int> modulatedBy, QPointF position, std::vector<AmpEnvValue> attackEnvValues,
                   std::vector<AmpEnvValue> releaseEnvValues, QObject *parent)
        : id(id), frequency(frequency), amplitude(amplitude), isModulator(isModulator), isCarrier(isCarrier),
          modulatedBy(std::move(modulatedBy)), position(position), attackEnvValues(attackEnvValues), releaseEnvValues(releaseEnvValues), currentSemiTone(0) {

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
                            }},
            {"frequencyLfoAmount", o.frequencyLfoAmount},
            {"amplitudeLfoAmount", o.amplitudeLfoAmount},
            {"attackEnvValues", o.attackEnvValues},
            {"releaseEnvValues", o.releaseEnvValues}
    };
}

void from_json(const json &j, Operator &o) {
    j["id"].get_to(o.id);
    j["frequency"].get_to(o.frequency);
    j["amplitude"].get_to(o.amplitude);
    j["isModulator"].get_to(o.isModulator);
    j["isCarrier"].get_to(o.isCarrier);
    j["modulatedBy"].get_to(o.modulatedBy);
    j["frequencyLfoAmount"].get_to(o.frequencyLfoAmount);
    j["amplitudeLfoAmount"].get_to(o.amplitudeLfoAmount);
    j["attackEnvValues"].get_to(o.attackEnvValues);
    j["releaseEnvValues"].get_to(o.releaseEnvValues);
    o.position.setX(j["position"]["x"].get<float>());
    o.position.setY(j["position"]["y"].get<float>());
}

