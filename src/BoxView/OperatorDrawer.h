//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_OPERATORDRAWER_H
#define QTQUICKTEST_OPERATORDRAWER_H


#include "BoxView.h"
#include "src/Controller/Operator.h"

class BoxView;

class OperatorDrawer {
public:
    OperatorDrawer(BoxView *boxView);
    void update(Operator* operator_);
    void draw(QPainter *painter, Operator* operator_);
private:
    void drawBox(QPainter *painter, Operator *operator_);
    bool isInsideBox(Operator* operator_, const QPointF& coords);
    float randomColor();
    QColor getColorForOperator(Operator *operator_);

    BoxView *boxView_;
};


#endif //QTQUICKTEST_OPERATORDRAWER_H
