//
// Created by Guðmundur on 10-Feb-23.
//

#ifndef QTQUICKTEST_OPERATOR_H
#define QTQUICKTEST_OPERATOR_H

#include <vector>
#include <QPoint>
#include <chrono>

enum class DraggingState {
    None,
    Holding,
    Dragging
};

struct Operator {
    Operator(int id);
    int id;
    long frequency;
    long amplitude;
    bool isModulator;
    bool isCarrier;
    std::vector<int> modulatedBy;
    QPointF position;
    DraggingState draggingState = DraggingState::None;
    std::optional<QPointF> initialDragCursorPos;

    // Schedule the operator to be deleted since deleting it is not possible while iterating over the operators
    bool scheduleForRemoval = false;
};


#endif //QTQUICKTEST_OPERATOR_H
