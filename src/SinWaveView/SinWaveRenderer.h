//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_SINWAVERENDERER_H
#define QTQUICKTEST_SINWAVERENDERER_H


#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObjectFormat>
#include "SinWaveItem.h"

class SinWaveRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
    SinWaveItem *sinWaveItem;
public:
    SinWaveRenderer(SinWaveItem *sinWaveItem);
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void drawLine(QVector2D from, QVector2D to);

    void render() override;
protected:
    QOpenGLContext *m_context;
private:
    float sin_start_x = 0.0;
};


#endif //QTQUICKTEST_SINWAVERENDERER_H
