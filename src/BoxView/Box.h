//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_BOX_H
#define QTQUICKTEST_BOX_H


#include "BoxView.h"

class Box {
public:
    Box(BoxView *boxView, double x, double y);
    void update();
    void draw();
private:
    void drawOuter();
    void drawInner();
    QPointF widgetCoordsToGl(const QPointF& coords);
    bool isInsideBox(const QPointF& coords);

    double boxPosX_ = 0.0;
    double boxPosY_ = 0.0;
    BoxView *boxView_;
};


#endif //QTQUICKTEST_BOX_H
