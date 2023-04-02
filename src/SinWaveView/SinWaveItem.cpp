//
// Created by Guðmundur on 2/6/2023.
//

#include "SinWaveItem.h"

SinWaveItem::SinWaveItem(QQuickItem *parent) : QQuickPaintedItem(parent) {

}

void SinWaveItem::paint(QPainter *painter) {
    painter->setPen(QPen(QColor(0xFB00C4), 3));

    const auto incrementStep = 0.001;
    for (float p = 0.0; p < 1.0; p += incrementStep) {
        const auto x = p * width();
        const auto nextX = (p + incrementStep) * width();
        painter->drawLine(QPointF(x, qSin(sinStartX_ + p * frequency()) * amplitude() * 40 + 40), QPointF(nextX, qSin(sinStartX_ + (p + incrementStep) * frequency()) * amplitude() * 40 + 40));
    }
    sinStartX_ += 0.1;

    update();
}

qreal SinWaveItem::frequency() {
    return frequency_;
}

void SinWaveItem::setFrequency(qreal frequency)  {
    if(frequency*0.05 < 22){
        frequency_ = 20;
    } else if (frequency*0.05 > 452){
        frequency_ = frequency*0.02;
    } else{
        frequency_ = frequency*0.05;
    }
}

void SinWaveItem::setAmplitude(qreal amplitude) {
    amplitude_ = amplitude*0.05;
}

qreal SinWaveItem::amplitude() {
    return amplitude_;
}
