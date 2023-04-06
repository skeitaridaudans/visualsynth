//
// Created by Sigurður on 3/25/2023.
//

#include "AmpEnvParams.h"
#include "QPainter"
#include "QRect"

AmpEnvParams::AmpEnvParams(AmpEnvGraphView *GraphView): GraphView_(GraphView){}



void AmpEnvParams:: assignX(int newX){
    this->coords.setX(newX+(width_/2));

}

void AmpEnvParams:: assignY(int newY){
    this->coords.setY(newY+(height_/2));
}

void AmpEnvParams::assignRGBColor(int r, int g, int b){
    this->rgb_r = r;
    this->rgb_g = g;
    this->rgb_b = b;
}

void AmpEnvParams::assignText(QString newTxt){
    this->text = newTxt;
}

void AmpEnvParams::draw(QPainter *painter) {
    QRect rect = QRect(coords.x()-(width_/2),coords.y()-(height_/2), width_, height_);

    QBrush brush(QColor(this->rgb_r,this->rgb_g,this->rgb_b));
    painter->setBrush(brush);

    painter->setPen(Qt::PenStyle::SolidLine);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rect);

    painter->setFont(QFont("Arial", 18));
    painter->drawText(rect, Qt::AlignCenter, text);

}

QPointF AmpEnvParams::myCoords() const{
    return this->coords;
}

void AmpEnvParams::setMyCoords(QPointF update_point) {
    bool emit_valid;

    if (this->coords.y() != update_point.y()) {
        this->coords.setY(update_point.y());

        emit_valid = true;
    }

    if (this->coords.x() != update_point.x()) {
        this->coords.setX(update_point.x());
        emit_valid = true;
    }


    if (emit_valid == true) {
        emit myCoordsChanged();
    }
}
