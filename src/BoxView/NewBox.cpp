//
// Created by Guðmundur on 2/6/2023.
//

#include "NewBox.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>

const double kBorderWidth = 0.02;
const double kBoxSize = 0.1;

NewBox::NewBox(BoxView *boxView, BoxViewRenderer *boxViewRenderer) : boxView_(boxView), boxViewRenderer_(boxViewRenderer) {}

void NewBox::update() {
    const auto pos = widgetCoordsToGl(boxView_->mapFromGlobal(QCursor::pos()));

    if (isInsideBox(pos) && QGuiApplication::mouseButtons() == Qt::LeftButton && !boxCreated_) {
        boxViewRenderer_->addOperator(pos.x() - kBoxSize / 2.0, pos.y() - kBoxSize / 2.0);
        boxCreated_ = true;
    }
    else if (!isInsideBox(pos) && QGuiApplication::mouseButtons() != Qt::LeftButton) {
        boxCreated_ = false;
    }
}

void NewBox::draw() {
    glPushMatrix();
    glTranslated(boxPosX_, boxPosY_, 0.0);
    glScaled(kBoxSize, kBoxSize, 1.0);

    drawOuter();
    drawInner();
    drawPlus();

    glPopMatrix();
}

void NewBox::drawOuter() {
    glColor3d(1.0, 1.0, 1.0);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(0.0, 0.0);
    glVertex2d(1.0, 0.0);
    glVertex2d(0.0, 1.0);
    glVertex2d(1.0, 1.0);
    glEnd();
}

void NewBox::drawInner() {
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

void NewBox::drawPlus() {
    glColor3d(1.0, 1.0, 1.0);

    glTranslated(0.25, 0.25, 0.0);
    glScaled(0.5, 0.5, 1.0);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(0.45, 0.0);
    glVertex2d(0.55, 0.0);
    glVertex2d(0.45, 1.0);
    glVertex2d(0.55, 1.0);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(0.0, 0.45);
    glVertex2d(1.0, 0.45);
    glVertex2d(0.0, 0.55);
    glVertex2d(1.0, 0.55);
    glEnd();
}

QPointF NewBox::widgetCoordsToGl(const QPointF &coords) {
    return QPointF(coords.x() / boxView_->width() * 2.0 - 1.0, coords.y() / boxView_->height() * 2.0 - 1.0);
}

bool NewBox::isInsideBox(const QPointF &coords) {
    return coords.x() >= boxPosX_ && coords.x() < boxPosX_ + kBoxSize && coords.y() >= boxPosY_ && coords.y() < boxPosY_ + kBoxSize;
}

