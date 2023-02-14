#ifndef QTQUICKTEST_CONTROLLER_H
#define QTQUICKTEST_CONTROLLER_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <QSet>
#include "Operator.h"
#include "Api.h"

class Controller {
public:
    static Controller instance;

    void addOperator();
    void removeOperator(int operatorId);
    void changeFrequency(int operatorId, long frequency);
    void changeAmplitude(int operatorId, long amplitude);
    void addModulator(int operatorId, int modulatorId);
    void removeModulator(int operatorId, int modulatorId);
    void noteOn(int note);
    void noteOff(int note);
    std::unordered_map<int, std::unique_ptr<Operator>>& operators();

private:
    Controller();
    void sendOperator(int operatorId);

    std::unordered_map<int, std::unique_ptr<Operator>> operators_;
    std::unordered_set<int> availableOperatorIds_;
    Api api;
};

#endif