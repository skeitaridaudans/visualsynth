//
// Created by Guðmundur on 2/6/2023.
//

#include "SinWaveRenderer.h"

SinWaveRenderer::SinWaveRenderer(SinWaveItem *sinWaveItem) : sinWaveItem(sinWaveItem) {}

QOpenGLFramebufferObject *SinWaveRenderer::createFramebufferObject(const QSize &size) {
    initializeOpenGLFunctions();

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    // optionally enable multisampling by doing format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

void SinWaveRenderer::drawLine(QVector2D from, QVector2D to) {
    glPushMatrix();
    glColor3d(0.0, 0.0, 0.7);
    glTranslated(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(from.x(), from.y());
    glVertex2d(from.x(), from.y() + 0.05);
    glVertex2d(to.x(), to.y());
    glVertex2d(to.x(), to.y() + 0.05);
    glEnd();
    glPopMatrix();
}

void SinWaveRenderer::render() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    for (float x = -1.0; x < 1.0; x += 0.001) {
        drawLine(QVector2D(x, qSin(sin_start_x + x * sinWaveItem->frequency()) * sinWaveItem->height()), QVector2D(x + 0.001, qSin(sin_start_x + (x + 0.001) * sinWaveItem->frequency()) * sinWaveItem->height()));
    }

    update();

    sin_start_x += 0.1;
}
