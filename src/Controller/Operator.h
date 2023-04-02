//
// Created by Guðmundur on 10-Feb-23.
//

#ifndef QTQUICKTEST_OPERATOR_H
#define QTQUICKTEST_OPERATOR_H

#include <vector>
#include <QPoint>
#include <chrono>
#include <QObject>

enum class DraggingState {
    None,
    Holding,
    Dragging
};

struct Operator:public QObject {
    Q_OBJECT
public:
    Operator(int id, QObject* parent=0);

    int id;
    long frequency;
    long amplitude;
    bool isModulator;
    bool isCarrier;
    std::vector<int> modulatedBy;
    QPointF position;
    bool isBeingDragged;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> timeSinceClick = std::nullopt;
    Q_PROPERTY(int idProp MEMBER id)
    Q_PROPERTY(long freqProp MEMBER frequency)
    Q_PROPERTY(long ampProp MEMBER amplitude)
    Q_INVOKABLE long getFreq();
    Q_INVOKABLE long getAmp();
    Q_INVOKABLE void setFrequency(long step);
    Q_INVOKABLE void setAmplitude(long step);
    DraggingState draggingState = DraggingState::None;
    std::optional<QPointF> initialDragCursorPos;

    // Schedule the operator to be deleted since deleting it is not possible while iterating over the operators
    bool scheduleForRemoval = false;
    Q_INVOKABLE QColor getColorForOperator(); //Operator *operator_
};

#endif //QTQUICKTEST_OPERATOR_H
