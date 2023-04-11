
#ifndef LOVECOMMUNICATIONTCP_H
#define LOVECOMMUNICATIONTCP_H

#include <QString>
#include <QtNetwork>

class LoveCommunicationTcp
{
public:
    LoveCommunicationTcp();

    bool connectToServer(QString ip, int port);
    void disconnectFromServer();
    QString getMessageString();
    void sendMessageString(QString msg);

    QString sendOperatorValue(int operator_id, bool attack, int envelope_index, float frequency, float amplitude);
    QString addModulator(int operator_id, int modulator_id);
    QString removeModulator(int operator_id, int modulator_id);
    QString addCarrier(int operator_id);
    QString removeCarrier(int operator_id);
    QString setAttackAmpEnvelopePoint(int point_index, float value, float time);
    QString setReleaseAmpEnvelopePoint(int point_index, float value, float time);
    QString setAttackAmpEnvelopeSize(int size);
    QString setReleaseAmpEnvelopeSize(int size);


private:
    QTcpSocket socket;
    QByteArray getMessageBytes();
    void sendMessageBytes(QByteArray msg);

};

#endif // LOVECOMMUNICATIONTCP_H
