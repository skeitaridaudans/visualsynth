//
// Created by Guðmundur on 4/11/2023.
//

#ifndef MAIN_QML_TOUCHPOINT_H
#define MAIN_QML_TOUCHPOINT_H

#include <QPointF>

struct TouchPoint {
    int id = -1;
    bool isPressed = false;
    QPointF position;
    bool initialPressHandled = false;
};

#endif //MAIN_QML_TOUCHPOINT_H
