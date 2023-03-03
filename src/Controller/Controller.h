#ifndef QTQUICKTEST_CONTROLLER_H
#define QTQUICKTEST_CONTROLLER_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <QObject>
#include <iostream>
#include "Operator.h"
#include "Api.h"

class Controller : public QObject {
    Q_OBJECT
public:
    static std::unique_ptr<Controller> instance;
    Controller (QObject* parent = 0);

    Q_INVOKABLE int addOperator();
    Q_INVOKABLE void removeOperator(int operatorId);
    Q_INVOKABLE void changeFrequency(int operatorId, long frequency);
    Q_INVOKABLE void changeAmplitude(int operatorId, long amplitude);
    Q_INVOKABLE void addModulator(int operatorId, int modulatorId);
    Q_INVOKABLE void removeModulator(int operatorId, int modulatorId);
    Q_INVOKABLE void noteOn(int note);
    Q_INVOKABLE void noteOff(int note);
    Q_INVOKABLE void selectOperator(int operatorId);
    Q_INVOKABLE void deselectOperator();
    const std::unordered_map<int, std::unique_ptr<Operator>>& operators();
    const std::unique_ptr<Operator> &getOperatorById(int id);
    std::optional<int> selectedOperatorId();
    std::optional<std::reference_wrapper<std::unique_ptr<Operator>>>  selectedOperator();

private:
    void sendOperator(int operatorId);

    std::unordered_map<int, std::unique_ptr<Operator>> operators_;
    std::unordered_set<int> availableOperatorIds_;
    std::optional<int> selectedOperatorId_;
    Api api;
};

#endif