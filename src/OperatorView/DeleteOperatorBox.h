//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_DELETEOPERATORBOX_H
#define QTQUICKTEST_DELETEOPERATORBOX_H


#include "OperatorView.h"

class OperatorView;

class DeleteOperatorBox {
public:
    DeleteOperatorBox(OperatorView *boxView);
    void update();
    void draw(QPainter* qPainter);

    bool isInsideBox(const QPointF& coords);
private:

    QPointF boxPos_;
    OperatorView *boxView_;
    bool boxCreated_ = false;
};


#endif //QTQUICKTEST_DELETEOPERATORBOX_H
