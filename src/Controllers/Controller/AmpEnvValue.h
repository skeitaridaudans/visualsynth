//
// Created by Guðmundur on 4/16/2023.
//

#ifndef MAIN_QML_AMPENVVALUE_H
#define MAIN_QML_AMPENVVALUE_H

#include "src/Utils/Utils.h"

struct AmpEnvValue {
    int index;
    float value;
    float time;
    bool attack;

    AmpEnvValue() : index(0), value(0.0f), time(0.0f), attack(false) {

    }

    explicit AmpEnvValue(int index, bool attack) : index(index), value(0.0f), time(0.0f), attack(attack) {

    }

    AmpEnvValue(int index, float value, float time, bool attack) : index(index), value(value), time(time), attack(attack) {

    }
};

inline void to_json(json& j, const AmpEnvValue& o) {
    j = json{
            { "index", o.index },
            { "value", o.value },
            { "time", o.time },
            { "attack", o.attack },
    };
}

inline void from_json(const json& j, AmpEnvValue& o) {
    j["index"].get_to(o.index);
    j["value"].get_to(o.value);
    j["time"].get_to(o.time);
    j["attack"].get_to(o.attack);
}

#endif //MAIN_QML_AMPENVVALUE_H
