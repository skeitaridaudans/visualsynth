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
const double kDragSensitivity = 2.0;

OperatorDrawer::OperatorDrawer(OperatorView *operatorView) : operatorView_(operatorView) {
}

void OperatorDrawer::update(Operator* operator_) {
    const auto cursorPos = operatorView_->mapFromGlobal(QCursor::pos());

    const auto& controller = Controller::instance;
    if (operator_->draggingState == DraggingState::None && isInsideBox(operator_, cursorPos) && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        operator_->initialDragCursorPos = cursorPos;
        operator_->draggingState = DraggingState::Holding;
    }
    else if (operator_->draggingState == DraggingState::Holding) {
        const auto moveVector = operator_->initialDragCursorPos.value() - cursorPos;
        const auto cursorMoveDistance = std::sqrt(QPointF::dotProduct(moveVector, moveVector));

        // Start dragging
        if (cursorMoveDistance >= kDragSensitivity) {
            operator_->draggingState = DraggingState::Dragging;
            operator_->initialDragCursorPos = std::nullopt;
            controller->deselectOperator();
        }

        // Click
        if (isInsideBox(operator_, cursorPos) && QGuiApplication::mouseButtons() != Qt::LeftButton) {
            const auto selectedOperatorId = controller->selectedOperatorId();

            if (selectedOperatorId.has_value()) {
                controller->addModulator(operator_->id, selectedOperatorId.value());
            }
            else {
                controller->selectOperator(operator_->id);
            }

            operator_->draggingState = DraggingState::None;
        }
    }


    if (operator_->draggingState == DraggingState::Dragging && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        operator_->position.setX(cursorPos.x() - kBoxSize / 2.0);
        operator_->position.setY(cursorPos.y() - kBoxSize / 2.0);

        const auto carrierLinePoints = operatorView_->carrierLineEndPoints();
        if (isRectInsideLine(QRectF(operator_->position, QSizeF(kBoxSize, kBoxSize)), carrierLinePoints.first, carrierLinePoints.second)) {
            operator_->isCarrier = true;
            operator_->position.setY(carrierLinePoints.first.y() - kBoxSize / 2.0);
        }
        else {
            operator_->isCarrier = false;
        }
    }
    else if (operator_->draggingState == DraggingState::Dragging) {
        operator_->draggingState = DraggingState::None;

        // If cursor is inside delete box
        if (operatorView_->deleteOperatorBox()->isInsideBox(cursorPos)) {
            operator_->scheduleForRemoval = true;
        }
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
