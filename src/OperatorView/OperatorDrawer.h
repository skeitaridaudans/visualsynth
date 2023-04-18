//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_OPERATORDRAWER_H
#define QTQUICKTEST_OPERATORDRAWER_H


#include "OperatorView.h"
#include "src/Controller/Operator.h"

class OperatorView;

class OperatorDrawer {
public:
    OperatorDrawer(OperatorView *operatorView);
    void update(Operator& operator_);
    void draw(QPainter *painter, const Operator& operator_);
private:
    void updateAnimations(Operator &operator_);
    void startDragging(Operator &operator_);
    void drawBox(QPainter *painter, const Operator &operator_);
    bool isInsideBox(const Operator& operator_, const QPointF& coords);
    void fixOperatorPositionAfterDrop(Operator& operator_, float moveMultiplier = 1.1f);


    OperatorView *operatorView_;
    DraggingState currentDraggingOperatorState_ = DraggingState::None;
    std::optional<int> draggedOperatorId_ = std::nullopt;

    void toggleModulator(Operator &operator_, int modulatorId);

    void onTouchDown(Operator &operator_);

    void onOperatorPressed(Operator &operator_);

    void updateOperatorDrag(Operator &operator_);

    void releaseOperator(Operator &operator_);
};


#endif //QTQUICKTEST_OPERATORDRAWER_H
