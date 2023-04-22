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
    std::vector<AmpEnvValue> empEnvValues;

    Preset() : operators(), empEnvValues() {

    }

    Preset(Operators operators, std::vector<AmpEnvValue> ampEnvValues, QString pname) : operators(std::move(operators)), empEnvValues(std::move(ampEnvValues)), name(pname) {

    }
};

inline void to_json(json& j, const Preset& o) {
    j = json{
            { "operators", o.operators },
            { "empEnvValues", o.empEnvValues },
    };
}

inline void from_json(const json& j, Preset& o) {
    j["operators"].get_to(o.operators);
    j["empEnvValues"].get_to(o.empEnvValues);
}

#endif //MAIN_QML_PRESET_H
