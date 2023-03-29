//
// Created by Sigurður on 3/29/2023.
//

#include <QPoint>
#include "Line.h"

Line::Line(AmpEnvGraphView *GraphView): GraphView_(GraphView){}

void Line::assignP1(double x, double y) {
    this->point1_.setX(x);
    this->point1_.setY(y);
}

void Line::assignP2(double x, double y) {
    this->point2_.setX(x);
    this->point2_.setY(y);
}

void Line::setColor(QColor c){
    this->color = c;
}

void Line::draw(QPainter *painter) {
    painter->setPen(QPen(color, 5));
    painter->drawLine(point1_, point2_);
}