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

    Q_INVOKABLE bool isConnected();     //Breki - checks if connection to the synth is working
    Q_INVOKABLE std::optional<int> addOperator();
    Q_INVOKABLE void removeOperator(int operatorId);
    Q_INVOKABLE void changeFrequency(int operatorId, long frequency);
    Q_INVOKABLE void changeAmplitude(int operatorId, long amplitude);
    Q_INVOKABLE void addModulator(int operatorId, int modulatorId);
    Q_INVOKABLE void removeModulator(int operatorId, int modulatorId);
    Q_INVOKABLE void noteOn(int note);
    Q_INVOKABLE void noteOff(int note);
    Q_INVOKABLE void selectOperator(int operatorId);
    Q_INVOKABLE void deselectOperator();
    Q_INVOKABLE void showAlert(const QString& text);
    const std::unordered_map<int, std::unique_ptr<Operator>>& operators();
    const std::unique_ptr<Operator> &getOperatorById(int id);
    std::optional<int> selectedOperatorId();
    std::optional<std::reference_wrapper<std::unique_ptr<Operator>>>  selectedOperator();

    Q_PROPERTY(QString alertText READ alertText NOTIFY alertTextChanged)
    Q_PROPERTY(QString alertVisibleState READ alertVisibleState NOTIFY alertVisibleStateChanged)

    QString alertVisibleState();
    QString alertText();
signals:
    void alertVisibleStateChanged(QString value);
    void alertTextChanged(QString value);
private:
    void sendOperator(int operatorId);

    std::unordered_map<int, std::unique_ptr<Operator>> operators_;
    std::unordered_set<int> availableOperatorIds_;
    std::optional<int> selectedOperatorId_;
    Api api;

    QString alertVisibleState_ = "invisible";
    QString alertText_;
};

#endif