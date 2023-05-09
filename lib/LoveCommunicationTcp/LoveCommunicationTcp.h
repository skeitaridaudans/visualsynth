#ifndef LOVECOMMUNICATIONTCP_H
#define LOVECOMMUNICATIONTCP_H

#include <QString>
#include <QtNetwork>
#include "src/Controllers/Controller/LfoWaveType.h"

class LoveCommunicationTcp
{
public:
    LoveCommunicationTcp(std::function<void (QTcpSocket::SocketState state)> onStateChange);

    bool connectToServer(QString ip, int port);
    void connectToServerAsync(QString ip, int port);
    void disconnectFromServer();
    QString getMessageString();
    void sendMessageString(QString msg);
    bool isConnected();

    QString sendOperatorValue(int operator_id, bool attack, int envelope_index, float frequency, float amplitude);
    QString addModulator(int operator_id, int modulator_id);
    QString removeModulator(int operator_id, int modulator_id);
    QString addCarrier(int operator_id);
    QString removeCarrier(int operator_id);
    QString setAttackAmpEnvelopePoint(int point_index, float value, float time);
    QString setReleaseAmpEnvelopePoint(int point_index, float value, float time);
    QString setAttackAmpEnvelopeSize(int size);
    QString setReleaseAmpEnvelopeSize(int size);
    QString setLfoGlobalOptions(bool enabled, LfoWaveType type, float frequency);
    QString setOperatorLfoValues(int operatorId, float frequencyAmount, float amplitudeAmount);
    QString setOperatorAmpEnvelopeValue(int operatorId, int index, bool attack, float value, float time);

private:
    QTcpSocket socket;
    std::function<void (QTcpSocket::SocketState state)> onStateChange;
    QByteArray getMessageBytes();
    void sendMessageBytes(QByteArray msg);
};

#endif // LOVECOMMUNICATIONTCP_H
