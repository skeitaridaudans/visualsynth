//
// Created by Guðmundur on 2/6/2023.
//

#include "Box.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>

const double kBorderWidth = 0.02;
const double kBoxSize = 0.1;

Box::Box(BoxView *boxView, double x, double y) : boxView_(boxView), boxPosX_(x), boxPosY_(y) {}

void Box::update() {
    const auto pos = widgetCoordsToGl(boxView_->mapFromGlobal(QCursor::pos()));

    if (isInsideBox(pos) && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        boxPosX_ = pos.x() - kBoxSize / 2.0;
        boxPosY_ = pos.y() - kBoxSize / 2.0;
    }
}

void Box::draw() {
    glPushMatrix();
    glTranslated(boxPosX_, boxPosY_, 0.0);
    glScaled(kBoxSize, kBoxSize, 1.0);

    drawOuter();
    drawInner();

    glPopMatrix();
}

void Box::drawOuter() {
    glColor3d(1.0, 1.0, 1.0);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(0.0, 0.0);
    glVertex2d(1.0, 0.0);
    glVertex2d(0.0, 1.0);
    glVertex2d(1.0, 1.0);
    glEnd();
}

void Box::drawInner() {
    const double innerBoxSize = 1.0 - (kBorderWidth * 2);

    glColor3d(0.0, 0.0, 0.0);
    glTranslated(kBorderWidth, kBorderWidth, 0.0);
    glScaled(innerBoxSize, innerBoxSize, 1.0);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(0.0, 0.0);
    glVertex2d(1.0, 0.0);
    glVertex2d(0.0, 1.0);
    glVertex2d(1.0, 1.0);
    glEnd();
}

QPointF Box::widgetCoordsToGl(const QPointF &coords) {
    return QPointF(coords.x() / boxView_->width() * 2.0 - 1.0, coords.y() / boxView_->height() * 2.0 - 1.0);
}

bool Box::isInsideBox(const QPointF &coords) {
    return coords.x() >= boxPosX_ && coords.x() < boxPosX_ + kBoxSize && coords.y() >= boxPosY_ && coords.y() < boxPosY_ + kBoxSize;
}


