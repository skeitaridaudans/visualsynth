//
// Created by Guðmundur on 10-Feb-23.
//

#ifndef QTQUICKTEST_API_H
#define QTQUICKTEST_API_H

#include "lib/LoveCommunicationTcp/LoveCommunicationTcp.h"

class Api {
public:
    Api(std::function<void (QTcpSocket::SocketState state)> onConnectionStateChange);

    void connect(const QString& ip);
    void disconnect();
    void sendOperatorValue(unsigned char operator_, unsigned char alg_index, bool attack, float frequency_factor, float amplitude);
    void noteOn(unsigned char key);
    void noteOff(unsigned char key);
    void addModulator(int operatorId, int modulatorId);
    void removeModulator(int operatorId, int modulatorId);
    void addCarrier(int operatorId);
    void removeCarrier(int operatorId);
    void setAmpEnvelopeAttackValue(int index, float value, float time);
    void setAmpEnvelopeSize(int size);
    void setAmpReleaseEnvelopePoint(int index, float value, float time);
    void setAmpReleaseEnvelopeSize(int size);
    void setLfoGlobalOptions(bool enabled, LfoWaveType type, float frequency);
    void setOperatorLfoValues(int operatorId, float frequencyAmount, float amplitudeAmount);
    void setOperatorEnvelopeAttackValue(int operatorId, int index, float value, float time);
    void setOperatorEnvelopeReleaseValue(int operatorId, int index, float value, float time);

private:
    LoveCommunicationTcp loveCommunicationTcp_;
};


#endif //QTQUICKTEST_API_H
