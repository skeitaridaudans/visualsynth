//
// Created by Guðmundur on 10-Feb-23.
//

#ifndef QTQUICKTEST_OPERATOR_H
#define QTQUICKTEST_OPERATOR_H

#include <vector>
#include <QPoint>
#include <chrono>

struct Operator {
    Operator(int id);
    int id;
    long frequency;
    long amplitude;
    bool isModulator;
    bool isCarrier;
    std::vector<int> modulatedBy;
    QPoint position;
    bool isBeingDragged;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> timeSinceClick = std::nullopt;
};


#endif //QTQUICKTEST_OPERATOR_H
