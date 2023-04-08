#ifndef QTQUICKTEST_CONTROLLER_H
#define QTQUICKTEST_CONTROLLER_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <QObject>
#include <iostream>
#include <QStringListModel>
#include "Operator.h"
#include "Api.h"

using Operators = std::unordered_map<int, std::unique_ptr<Operator>>;

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
    Q_INVOKABLE void addCarrier(int operatorId);
    Q_INVOKABLE void removeCarrier(int operatorId);
    void saveOperators(const std::string& name);
    void loadOperators(const std::string &name);
    const Operators& operators();
    const std::unique_ptr<Operator> &getOperatorById(int id);
    std::optional<int> selectedOperatorId();
    Q_INVOKABLE Operator* getSelectedOperator();
    void setOperators(const Operators& operators);

    Q_PROPERTY(bool showPresets MEMBER showPresets_ NOTIFY showPresetsChanged);

signals:
    // Signals for operators
    Q_SIGNAL void operatorSelected(Operator* op);
    Q_SIGNAL void operatorDeselected(bool deselected);
    Q_SIGNAL void ampChanged(long amp);
    Q_SIGNAL void freqChanged(long freq);

    Q_SIGNAL void showPresetsChanged(bool showPresets);
private:
    void sendOperator(int operatorId);
    void resetAvailableOperatorIds();

    Operators operators_;
    std::unordered_set<int> availableOperatorIds_;
    std::optional<int> selectedOperatorId_;
    Api api;
    bool showPresets_;
};

#endif
