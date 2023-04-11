//
// Created by Guðmundur on 4/11/2023.
//

#ifndef MAIN_QML_TOUCHPOINT_H
#define MAIN_QML_TOUCHPOINT_H

#include <QPointF>

struct TouchPoint {
    bool isPressed;
    QPointF position;
};

#endif //MAIN_QML_TOUCHPOINT_H
