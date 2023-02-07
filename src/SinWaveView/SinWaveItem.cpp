//
// Created by Guðmundur on 2/6/2023.
//

#include "SinWaveItem.h"
#include "SinWaveRenderer.h"

QQuickFramebufferObject::Renderer *SinWaveItem::createRenderer() const
{
    window()->setPersistentGraphics(true);
    return new SinWaveRenderer(const_cast<SinWaveItem *>(this));
}
qreal SinWaveItem::frequency() {
    return frequency_;
}

void SinWaveItem::setFrequency(qreal frequency)  {
    frequency_ = frequency;
}

void SinWaveItem::setHeight(qreal height) {
    height_ = height;
}

qreal SinWaveItem::height() {
    return height_;
}