//
// Created by Guðmundur on 2/6/2023.
//

#include "OperatorDrawer.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>
#include "src/Controller/Controller.h"
#include "src/GlUtils/Utils.h"

const double kBorderWidth = 0.02;
const double kBoxSize = 0.1;

OperatorDrawer::OperatorDrawer(BoxView *boxView) : boxView_(boxView) {
}

void OperatorDrawer::update(Operator* operator_) {
    const auto pos = widgetCoordsToGl(boxView_->mapFromGlobal(QCursor::pos()));

    if (!operator_->isBeingDragged && isInsideBox(operator_, pos) && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        operator_->isBeingDragged = true;
    }
    if (operator_->isBeingDragged && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        operator_->position.setX(pos.x() - kBoxSize / 2.0);
        operator_->position.setY(pos.y() - kBoxSize / 2.0);
    }
    else if (operator_->isBeingDragged) {
        operator_->isBeingDragged = false;
    }
}

void OperatorDrawer::draw(Operator* operator_) {
    glPushMatrix();
    glTranslated(operator_->position.x(), operator_->position.y(), 0.0);
    glScaled(kBoxSize, kBoxSize, 1.0);

    drawBox(operator_);

    glPopMatrix();

    // Draw modulator lines
    auto const& controller = Controller::instance;
    for (const auto opId : operator_->modulatedBy) {
        const auto& position = controller->getOperatorById(opId)->position;

        drawLine(position, operator_->position);
    }
}

void OperatorDrawer::drawBox(Operator* operator_) {
    auto color = getColorForOperator(operator_);
    glColor3d(color.redF(), color.greenF(), color.blueF());

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(0.0, 0.0);
    glVertex2d(1.0, 0.0);
    glVertex2d(0.0, 1.0);
    glVertex2d(1.0, 1.0);
    glEnd();
}

QPointF OperatorDrawer::widgetCoordsToGl(const QPointF &coords) {
    return QPointF(coords.x() / boxView_->width() * 2.0 - 1.0, coords.y() / boxView_->height() * 2.0 - 1.0);
}

bool OperatorDrawer::isInsideBox(Operator* operator_, const QPointF &coords) {
    return coords.x() >= operator_->position.x() &&
            coords.x() < operator_->position.x()  + kBoxSize && // coords.x is within x range of operator
            coords.y() >= operator_->position.y() &&
            coords.y() < operator_->position.y() + kBoxSize; // coords.y is within y range of operator
}

QColor OperatorDrawer::getColorForOperator(Operator *operator_) {
    // TODO: Calculate color from operator frequency/amplitude
    return QColor(255, 0, 0);
}
