//
// Created by Guðmundur on 2/23/2023.
//

#include <QOpenGLFunctions>
#include <QColor>
#include "Utils.h"

void drawLine(const QPointF& from, const QPointF& to, const QColor& color) {
    const auto between = (to - from);
    const auto distance = sqrt(QPointF::dotProduct(between, between));
    const auto angle = between.x() >= 0 ? atan(between.y() / between.x()) : atan(between.y() / between.x()) - M_PI;

    glColor3d(color.redF(), color.greenF(), color.blueF());

    glPushMatrix();
    glTranslated(from.x(), from.y(), 0);
    glRotated(qRadiansToDegrees(angle), 0, 0, 1);
    glScaled(distance, 0.005, 1.0);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(0.0, 1.0);
    glVertex2f(1.0, 1.0);
    glEnd();

    glPopMatrix();
}