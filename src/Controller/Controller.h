#ifndef QTQUICKTEST_CONTROLLER_H
#define QTQUICKTEST_CONTROLLER_H

#include <QMap>
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
    QMap<int, std::unique_ptr<Operator>>& operators();

private:
    Controller();
    void sendOperator(int operatorId);

    QMap<int, std::unique_ptr<Operator>> operators_;
    QSet<int> availableOperatorIds_;
    Api api;
};

#endif