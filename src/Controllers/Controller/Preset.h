//
// Created by Guðmundur on 4/16/2023.
//

#ifndef MAIN_QML_PRESET_H
#define MAIN_QML_PRESET_H

#include <utility>

#include "Controller.h"

using Operators = std::unordered_map<int, Operator>;

struct Preset {
    QString name;
    Operators operators;
    std::vector<AmpEnvValue> attackAmpEnvValues;
    std::vector<AmpEnvValue> releaseAmpEnvValues;
    bool lfoEnabled;
    double lfoFrequency;

    Preset() : operators(), attackAmpEnvValues(), releaseAmpEnvValues(), lfoEnabled(false), lfoFrequency(6.0) {

    }

    Preset(Operators operators, std::vector<AmpEnvValue> attackAmpEnvValues,
           std::vector<AmpEnvValue> releaseAmpEnvValues, QString name, bool lfoEnabled, double lfoFrequency)
            : operators(std::move(operators)), attackAmpEnvValues(std::move(attackAmpEnvValues)),
              releaseAmpEnvValues(std::move(releaseAmpEnvValues)), name(std::move(name)), lfoEnabled(lfoEnabled),
              lfoFrequency(lfoFrequency) {

    }
};

inline void to_json(json &j, const Preset &o) {
    j = json{
            {"operators",           o.operators},
            {"attackAmpEnvValues",  o.attackAmpEnvValues},
            {"releaseAmpEnvValues", o.releaseAmpEnvValues},
            {"lfoEnabled", o.lfoEnabled},
            {"lfoFrequency", o.lfoFrequency},
    };
}

inline void from_json(const json &j, Preset &o) {
    j["operators"].get_to(o.operators);
    j["attackAmpEnvValues"].get_to(o.attackAmpEnvValues);
    j["releaseAmpEnvValues"].get_to(o.releaseAmpEnvValues);
    j["lfoEnabled"].get_to(o.lfoEnabled);
    j["lfoFrequency"].get_to(o.lfoFrequency);
}

#endif //MAIN_QML_PRESET_H
