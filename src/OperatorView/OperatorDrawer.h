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
    void drawBox(QPainter *painter, const Operator &operator_);
    bool isInsideBox(const Operator& operator_, const QPointF& coords);
    void fixOperatorPositionAfterDrop(Operator& operator_, float moveMultiplier = 1.1f);


    OperatorView *operatorView_;
    bool isPointBeingDragged_ = false;
};


#endif //QTQUICKTEST_OPERATORDRAWER_H
