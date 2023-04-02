//
// Created by Sigurður on 3/29/2023.
//

#ifndef MAIN_QML_LINE_H
#define MAIN_QML_LINE_H

#include <QPainter>
#include <QQuickPaintedItem>

class AmpEnvGraphView;

class Line {

public:

    Line(AmpEnvGraphView *GraphView);
    void draw(QPainter* painter);
    void assignP1(double x,double y);
    void assignP2(double x,double y);
    void setColor(QColor c);


private:
    AmpEnvGraphView *GraphView_;
    QPointF point1_;
    QPointF point2_;
    QColor color;
};


#endif //MAIN_QML_LINE_H
