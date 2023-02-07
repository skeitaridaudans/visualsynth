//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_NEWBOX_H
#define QTQUICKTEST_NEWBOX_H


#include "BoxView.h"
#include "BoxViewRenderer.h"

class BoxViewRenderer;

class NewBox {
public:
    NewBox(BoxView *boxView, BoxViewRenderer *boxViewRenderer);
    void update();
    void draw();
private:
    void drawOuter();
    void drawInner();
    void drawPlus();
    QPointF widgetCoordsToGl(const QPointF& coords);
    bool isInsideBox(const QPointF& coords);

    double boxPosX_ = 0.85;
    double boxPosY_ = -0.8;
    BoxView *boxView_;
    BoxViewRenderer *boxViewRenderer_;
    bool boxCreated_ = false;
};


#endif //QTQUICKTEST_NEWBOX_H
