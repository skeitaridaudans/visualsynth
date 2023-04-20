//
// Created by Guðmundur on 10-Feb-23.
//

#ifndef QTQUICKTEST_API_H
#define QTQUICKTEST_API_H


#include "src/lovecommunicationtcp/lovecommunicationtcp.h"

class Api {
public:
    Api();

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
    LoveCommunicationTcp loveCommunicationTcp_; //prófa að hafa public til að kalla í frá qml

private:
    //LoveCommunicationTcp loveCommunicationTcp_;
};


#endif //QTQUICKTEST_API_H
