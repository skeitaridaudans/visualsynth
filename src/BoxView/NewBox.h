//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_NEWBOX_H
#define QTQUICKTEST_NEWBOX_H


#include "BoxView.h"

class BoxView;

class NewBox {
public:
    NewBox(BoxView *boxView);
    void update();
    void draw(QPainter* qPainter);
private:
    QPointF widgetCoordsToGl(const QPointF& coords);
    bool isInsideBox(const QPointF& coords);

    QPointF boxPos_;
    BoxView *boxView_;
    bool boxCreated_ = false;
};


#endif //QTQUICKTEST_NEWBOX_H
