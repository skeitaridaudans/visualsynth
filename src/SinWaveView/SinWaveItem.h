//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_SINWAVEITEM_H
#define QTQUICKTEST_SINWAVEITEM_H

#include <QQuickWindow>
#include <QQuickPaintedItem>
#include <QPainter>

class SinWaveItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit SinWaveItem(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    qreal frequency();
    qreal amplitude();
    Q_INVOKABLE void setFrequency(qreal frequency);
    Q_INVOKABLE void setAmplitude(qreal amplitude);
private:
    qreal frequency_ = 0;
    qreal amplitude_ = 0;
    float sinStartX_ = 0.0;
};


#endif //QTQUICKTEST_SINWAVEITEM_H
