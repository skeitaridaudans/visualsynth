//
// Created by Guðmundur on 28-Apr-23.
//

#ifndef MAIN_QML_DRAGGINGTOUCHPOINT_H
#define MAIN_QML_DRAGGINGTOUCHPOINT_H

struct DraggingTouchPoint {
    int touchPointId;
    int ampEnvPointIndex;
    bool isAttack;

    DraggingTouchPoint(int touchPointId,
                       int ampEnvPointIndex,
                       bool isAttack) : touchPointId(touchPointId),
                                        ampEnvPointIndex(ampEnvPointIndex),
                                        isAttack(isAttack) {

    }
};

#endif //MAIN_QML_DRAGGINGTOUCHPOINT_H
