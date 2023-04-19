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
#include "src/AmpEnvelope/AmpGraph/AmpEnvParams/AmpEnvParams.h"
#include "AmpEnvValue.h"
#include "Preset.h"

using Operators = std::unordered_map<int, Operator>;

class Controller : public QObject {
Q_OBJECT
public:
    static std::unique_ptr<Controller> instance;

    Controller(QObject *parent = 0);

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
    Q_INVOKABLE void setAttackAmpEnvelopePoint(int index, float value, float time);
    Q_INVOKABLE void setReleaseAmpEnvelopePoint(int index, float value, float time);
    Q_INVOKABLE void setAmpEnvelopeSize(int size);
    void savePreset(const std::string &name);
    void loadPreset(const std::string &name);
    Operators &operators();
    AmpEnvValue (&ampEnvValues())[4];
    Operator &getOperatorById(int id);
    std::optional<int> selectedOperatorId();
    Q_INVOKABLE Operator *getSelectedOperator();
    void changeToPreset(const Preset& preset);
    Q_INVOKABLE void hidePresets();
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
    void sendAllOperatorInfo(int operatorId, std::unordered_set<int> *visited = nullptr);
    void resetAvailableOperatorIds();
    void removeAllModulators();
    void removeAllCarriers();
    void removeAllModulatorsForOperator(int operatorId);

    Operators operators_;
    std::unordered_set<int> availableOperatorIds_;
    std::optional<int> selectedOperatorId_;
    AmpEnvValue ampEnvValues_[4] = {AmpEnvValue(0, true), AmpEnvValue(1, true), AmpEnvValue(2, true), AmpEnvValue(3, false)};
    Api api;
    bool showPresets_;

    void loadInitialPreset();
};

#endif
