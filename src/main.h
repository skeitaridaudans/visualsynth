//
// Created by Guðmundur on 1/25/2023.
//

#ifndef QTQUICKTEST_MAIN_H
#define QTQUICKTEST_MAIN_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQuickFramebufferObject>
#include <QSize>
#include <QtQuick/QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include<QDebug>
#include <QVector2D>
#include <QtMath>

class FbItem : public QQuickFramebufferObject
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

#endif //QTQUICKTEST_MAIN_H
