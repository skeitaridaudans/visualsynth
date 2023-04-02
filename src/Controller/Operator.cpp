//
// Created by Guðmundur on 10-Feb-23.
//

#include "Operator.h"
#include <iostream>

Operator::Operator(int id, QObject *parent)
        : id(id), QObject(parent), frequency(440), amplitude(20), isCarrier(false), isModulator(false) {}

Operator::Operator()
        : id(0), QObject(), frequency(440), amplitude(20), isCarrier(false), isModulator(false) {}

void Operator::setFrequency(long step) {
    if (((this->frequency + step) < 20000) && ((this->frequency + step) > 0)) {
        this->frequency = this->frequency + step;
    }
}

void Operator::setAmplitude(long step) {
    if (((this->amplitude + step) <= 60) && ((this->amplitude + step) >= 1)) {
        this->amplitude = this->amplitude + step;
    }
}

long Operator::getAmp() {
    return this->amplitude;
}

long Operator::getFreq() {
    return this->frequency;
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

void to_json(json &j, const std::unique_ptr<Operator> &o) {
    j = json{
            {"id",          o->id},
            {"frequency",   o->frequency},
            {"amplitude",   o->amplitude},
            {"isModulator", o->isModulator},
            {"isCarrier",   o->isCarrier},
            {"modulatedBy", o->modulatedBy},
            {"position",    {
                                    {"x", o->position.x()},
                                    {"y", o->position.y()}
                            }}
    };
}

void from_json(const json &j, std::unique_ptr<Operator> &o) {
    j["id"].get_to(o->id);
    j["frequency"].get_to(o->frequency);
    j["amplitude"].get_to(o->amplitude);
    j["isModulator"].get_to(o->isModulator);
    j["isCarrier"].get_to(o->isCarrier);
    j["modulatedBy"].get_to(o->modulatedBy);
    o->position.setX(j["position"]["x"].get<float>());
    o->position.setY(j["position"]["y"].get<float>());
}
