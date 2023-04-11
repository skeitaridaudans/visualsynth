//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_NEWBOX_H
#define QTQUICKTEST_NEWBOX_H


#include "OperatorView.h"

class OperatorView;

class AddOperatorBox {
public:
    AddOperatorBox(OperatorView *operatorView);
    void update();
    void draw(QPainter* qPainter);
private:
    bool isInsideBox(const QPointF& coords);

    QPointF boxPos_;
    OperatorView *operatorView_;
    bool operatorCreated_ = false;
};


#endif //QTQUICKTEST_NEWBOX_H
