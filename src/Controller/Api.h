//
// Created by Guðmundur on 10-Feb-23.
//

#ifndef QTQUICKTEST_API_H
#define QTQUICKTEST_API_H


class Api {
public:
    Api();

    void sendOperatorValue(bool attack, int envelopeId, int operatorId, float frequency, float amplitude);
    void noteOn(int midiNote);
    void noteOff(int midiNote);
};


#endif //QTQUICKTEST_API_H
