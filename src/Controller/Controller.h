#ifndef QTQUICKTEST_CONTROLLER_H
#define QTQUICKTEST_CONTROLLER_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <QObject>
#include <iostream>
#include <QStringListModel>
#include <QSettings>
#include "Operator.h"
#include "Api.h"
#include "AmpEnvValue.h"
#include "Preset.h"

using Operators = std::unordered_map<int, Operator>;

class Controller : public QObject {
Q_OBJECT
public:
    static std::unique_ptr<Controller> instance;

    Controller(QObject *parent = 0);

    Q_INVOKABLE std::optional<int> addOperator();
    Q_INVOKABLE void removeOperator(int operatorId);
    Q_INVOKABLE void changeFrequency(int operatorId, float frequency);
    Q_INVOKABLE void changeAmplitude(int operatorId, long amplitude);
    Q_INVOKABLE void setOperatorLfoFrequency(int operatorId, long amount);
    Q_INVOKABLE void setOperatorLfoAmplitude(int operatorId, long amount);
    Q_INVOKABLE void setLfoEnabled(bool enabled);
    Q_INVOKABLE void setLfoWaveType(LfoWaveType lfoWaveType);
    Q_INVOKABLE void setLfoFrequency(long frequency);
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
    const std::vector<AmpEnvValue>& attackAmpEnvValues();
    const std::vector<AmpEnvValue>& releaseAmpEnvValues();
    Operator &getOperatorById(int id);
    std::optional<int> selectedOperatorId();
    Q_INVOKABLE Operator *getSelectedOperator();
    void changeToPreset(const Preset& preset);
    Q_INVOKABLE void hidePresets();
    double getSingleOperatorValue(int operatorId, double offset);
    double getOperatorModulationValue(int operatorId, double offset);
    double getCarrierOutput(double offset);
    Q_INVOKABLE void showConnectDialog();
    Q_INVOKABLE void disconnect();
    bool showPresets();

    Q_PROPERTY(bool showPresets MEMBER showPresets_ NOTIFY showPresetsChanged);
    Q_PROPERTY(bool isConnected MEMBER isConnected_ NOTIFY isConnectedChanged);
    Q_PROPERTY(bool isConnecting MEMBER isConnecting_ NOTIFY isConnectingChanged);
    Q_PROPERTY(QString connectionStateText MEMBER connectionStateText_ NOTIFY connectionStateTextChanged);
    Q_PROPERTY(bool isLfoEnabled MEMBER isLfoEnabled_ NOTIFY isLfoEnabledChanged);
    Q_PROPERTY(long lfoFrequency MEMBER lfoFrequency_ NOTIFY lfoFrequencyChanged);

signals:
    // Signals for operators
    Q_SIGNAL void operatorSelected(Operator* op);
    Q_SIGNAL void operatorDeselected(bool deselected);
    Q_SIGNAL void ampChanged(long amp);
    Q_SIGNAL void freqChanged(float freq);
    Q_SIGNAL void showPresetsChanged(bool showPresets);
    Q_SIGNAL void isConnectedChanged(bool isConnected);
    Q_SIGNAL void isConnectingChanged(bool isConnecting);
    Q_SIGNAL void connectionStateTextChanged(QString connectionStateText);
    Q_SIGNAL void isLfoEnabledChanged(bool isLfoEnabled);
    Q_SIGNAL void lfoWaveTypeChanged(LfoWaveType lfoWaveType);
    Q_SIGNAL void lfoFrequencyChanged(long lfoFrequency);


private:
    void sendOperator(int operatorId);
    void sendAllOperatorInfo(int operatorId, std::unordered_set<int> *visited = nullptr);
    void resetAvailableOperatorIds();
    void removeAllModulators();
    void removeAllCarriers();
    void removeAllModulatorsForOperator(int operatorId);
    void onConnectionStateChanged(QTcpSocket::SocketState state);
    void loadInitialPreset();
    void sendOperatorLfoValuesToSynth(int operatorId);
    void sendLfoGlobalOptionsToSynth();
    void onConnected();
    void setConnectionStateText(QString connectionStateText);
    void sendAmpEnvelopeToSynth();

    bool isFirst_ = true; // This is used to avoid the program crashing while loading the initial preset
    Operators operators_;
    std::unordered_set<int> availableOperatorIds_;
    std::optional<int> selectedOperatorId_;
    std::vector<AmpEnvValue> attackAmpEnvValues_;
    std::vector<AmpEnvValue> releaseAmpEnvValues_;
    std::unique_ptr<Api> api;
    bool showPresets_ = false;
    bool isConnected_ = false;
    bool isConnecting_ = false;
    bool isLfoEnabled_ = false;
    LfoWaveType lfoWaveType_ = LfoWaveType::Sine;
    // 60 is 6 Hz
    long lfoFrequency_ = 60;
    QString synthIp_;
    QString connectionStateText_ = "Not connected";
    QSettings settings_;
};

#endif
