//
// Created by Guðmundur on 2/6/2023.
//

#include "OperatorDrawer.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>
#include "src/Controller/Controller.h"
#include "src/GlUtils/Utils.h"
#include "src/Utils/Utils.h"

const double kBoxSize = 70.0;

OperatorDrawer::OperatorDrawer(OperatorView *boxView) : boxView_(boxView) {
}

void OperatorDrawer::update(Operator* operator_) {
    const auto pos = boxView_->mapFromGlobal(QCursor::pos());

    const auto& controller = Controller::instance;
    if (!operator_->isBeingDragged && isInsideBox(operator_, pos) && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        if (!operator_->timeSinceClick.has_value()) {
            operator_->timeSinceClick = std::chrono::high_resolution_clock::now();
        }
        else {
            const auto endTime = std::chrono::high_resolution_clock::now();
            const auto timeElapsed = std::chrono::duration<double, std::milli>(endTime-operator_->timeSinceClick.value()).count();

            // Start dragging
            if (timeElapsed >= 200) {
                operator_->isBeingDragged = true;
                operator_->timeSinceClick = std::nullopt;
                controller->deselectOperator();
            }
        }
    }
    // Click
    else if (!operator_->isBeingDragged && isInsideBox(operator_, pos) && operator_->timeSinceClick.has_value()) {
        const auto selectedOperatorId = controller->selectedOperatorId();

        if (selectedOperatorId.has_value()) {
            controller->addModulator(operator_->id, selectedOperatorId.value());
        }
        else {
            controller->selectOperator(operator_->id);
        }
    }


    if (operator_->isBeingDragged && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        operator_->position.setX(pos.x() - kBoxSize / 2.0);
        operator_->position.setY(pos.y() - kBoxSize / 2.0);

        const auto carrierLinePoints = boxView_->carrierLineEndPoints();
        if (isRectInsideLine(QRectF(operator_->position, QSizeF(kBoxSize, kBoxSize)), carrierLinePoints.first, carrierLinePoints.second)) {
            operator_->isCarrier = true;
            operator_->position.setY(carrierLinePoints.first.y() - kBoxSize / 2.0);
        }
        else {
            operator_->isCarrier = false;
        }
    }
    else if (operator_->isBeingDragged) {
        operator_->isBeingDragged = false;
    }
}

void OperatorDrawer::draw(QPainter *painter, Operator* operator_) {
    drawBox(painter, operator_);

    // Draw modulator lines
    auto const& controller = Controller::instance;
    for (const auto opId : operator_->modulatedBy) {
        const auto modulatorPosition = closestPointInBox(controller->getOperatorById(opId)->position, operator_->position, kBoxSize, kBoxSize);
        const auto modulatedPosition = closestPointInBox(operator_->position, controller->getOperatorById(opId)->position, kBoxSize, kBoxSize);

        painter->setPen(getColorForOperator(controller->getOperatorById(opId).get()));
        painter->drawLine(modulatorPosition, modulatedPosition);
    }
}

void OperatorDrawer::drawBox(QPainter *painter, Operator* operator_) {
    auto color = getColorForOperator(operator_);

    painter->setBrush(QBrush(color));
    painter->setPen(Qt::PenStyle::NoPen);
    painter->drawRect(QRectF(operator_->position, QSize(kBoxSize, kBoxSize)));
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
