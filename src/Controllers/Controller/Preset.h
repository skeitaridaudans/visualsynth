//
// Created by Guðmundur on 4/16/2023.
//

#ifndef MAIN_QML_PRESET_H
#define MAIN_QML_PRESET_H

#include "Controller.h"

using Operators = std::unordered_map<int, Operator>;

struct Preset {
    QString name;
    Operators operators;
    std::vector<AmpEnvValue> attackAmpEnvValues;
    std::vector<AmpEnvValue> releaseAmpEnvValues;

    Preset() : operators(), attackAmpEnvValues(), releaseAmpEnvValues() {

    }

    Preset(Operators operators, std::vector<AmpEnvValue> attackAmpEnvValues, std::vector<AmpEnvValue> releaseAmpEnvValues, QString name) : operators(std::move(operators)), attackAmpEnvValues(std::move(attackAmpEnvValues)), releaseAmpEnvValues(std::move(releaseAmpEnvValues)), name(name) {

    }
};

inline void to_json(json& j, const Preset& o) {
    j = json{
            { "operators", o.operators },
            { "attackAmpEnvValues", o.attackAmpEnvValues},
            { "releaseAmpEnvValues", o.releaseAmpEnvValues},
    };
}

inline void from_json(const json& j, Preset& o) {
    j["operators"].get_to(o.operators);
    j["attackAmpEnvValues"].get_to(o.attackAmpEnvValues);
    j["releaseAmpEnvValues"].get_to(o.releaseAmpEnvValues);
}

#endif //MAIN_QML_PRESET_H
