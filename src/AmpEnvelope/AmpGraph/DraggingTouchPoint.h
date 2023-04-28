//
// Created by Guðmundur on 28-Apr-23.
//

#ifndef MAIN_QML_DRAGGINGTOUCHPOINT_H
#define MAIN_QML_DRAGGINGTOUCHPOINT_H

struct DraggingTouchPoint {
    int touchPointId;
    int ampEnvPointIndex;
    bool isLastPoint;

    DraggingTouchPoint(int touchPointId,
                       int ampEnvPointIndex,
                       bool isLastPoint) : touchPointId(touchPointId),
                                        ampEnvPointIndex(ampEnvPointIndex),
                                        isLastPoint(isLastPoint) {

    }
};

#endif //MAIN_QML_DRAGGINGTOUCHPOINT_H
