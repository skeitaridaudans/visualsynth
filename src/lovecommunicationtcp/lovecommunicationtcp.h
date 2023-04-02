
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
    void addModulator(int operator_id, int modulator_id);
    void removeModulator(int operator_id, int modulator_id);
    void addCarrier(int operator_id);
    void removeCarrier(int operator_id);


private:
    QTcpSocket socket;
    QByteArray getMessageBytes();
    void sendMessageBytes(QByteArray msg);

};

#endif // LOVECOMMUNICATIONTCP_H