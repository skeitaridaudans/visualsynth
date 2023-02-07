//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_SINWAVEITEM_H
#define QTQUICKTEST_SINWAVEITEM_H

#include <QQuickWindow>
#include <QQuickFramebufferObject>

class SinWaveItem : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    QQuickFramebufferObject::Renderer *createRenderer() const;
    qreal frequency();
    qreal height();
    Q_INVOKABLE void setFrequency(qreal frequency);
    Q_INVOKABLE void setHeight(qreal height);
private:
    qreal frequency_ = 15.0;
    qreal height_ = 0.9;
};


#endif //QTQUICKTEST_SINWAVEITEM_H
