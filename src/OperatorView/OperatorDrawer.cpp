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
const double kCornerRadius = 5.0;

OperatorDrawer::OperatorDrawer(OperatorView *operatorView) : operatorView_(operatorView) {
}

void OperatorDrawer::update(Operator* operator_) {
    const auto touchPointPos = operatorView_->touchPoint().position;

    const auto& controller = Controller::instance;
    if (operator_->draggingState == DraggingState::None && isInsideBox(operator_, touchPointPos) && operatorView_->touchPoint().isPressed) {
        operator_->initialDragCursorPos = touchPointPos;
        operator_->draggingState = DraggingState::Holding;
    }
    else if (operator_->draggingState == DraggingState::Holding) {
        const auto moveVector = operator_->initialDragCursorPos.value() - touchPointPos;
        const auto cursorMoveDistance = std::sqrt(QPointF::dotProduct(moveVector, moveVector));

        // Start dragging
        if (cursorMoveDistance >= kDragSensitivity) {
            operator_->draggingState = DraggingState::Dragging;
            operator_->initialDragCursorPos = std::nullopt;
            controller->deselectOperator();
        }

        // Click
        if (isInsideBox(operator_, touchPointPos) && !operatorView_->touchPoint().isPressed) {
            const auto selectedOperatorId = controller->selectedOperatorId();

            if (selectedOperatorId.has_value() && selectedOperatorId.value() == operator_->id) {
                controller->deselectOperator();
            }
            else if (selectedOperatorId.has_value()) {
                controller->addModulator(operator_->id, selectedOperatorId.value());
                controller->deselectOperator();
            }
            else {
                controller->selectOperator(operator_->id);
            }

            operator_->draggingState = DraggingState::None;
        }
    }


    if (operator_->draggingState == DraggingState::Dragging && operatorView_->touchPoint().isPressed) {
        const auto operatorPos = operatorView_->fromViewCoords(QPointF(touchPointPos.x() - kBoxSize / 2.0, touchPointPos.y() - kBoxSize / 2.0));
        operator_->position.setX(operatorPos.x());
        operator_->position.setY(operatorPos.y());

        const auto carrierLinePoints = operatorView_->carrierLineEndPoints();
        if (isRectInsideLine(QRectF(operatorView_->toViewCoords(operator_->position), QSizeF(kBoxSize, kBoxSize)), carrierLinePoints.first, carrierLinePoints.second)) {
            controller->addCarrier(operator_->id);
            operator_->position.setY((carrierLinePoints.first.y() - kBoxSize / 2.0) / operatorView_->height());
        }
        else {
            controller->removeCarrier(operator_->id);
        }
    }
    else if (operator_->draggingState == DraggingState::Dragging) {
        operator_->draggingState = DraggingState::None;

        // If cursor is inside delete box
        if (operatorView_->deleteOperatorBox()->isInsideBox(touchPointPos)) {
            operator_->scheduleForRemoval = true;
        }
    }
}

void OperatorDrawer::draw(QPainter *painter, Operator* operator_) {
    drawBox(painter, operator_);

    // Draw modulator lines
    auto const& controller = Controller::instance;
    for (const auto opId : operator_->modulatedBy) {
        const auto operatorPos = operatorView_->toViewCoords(operator_->position);
        const auto modulatorPosition = closestPointInBox(operatorView_->toViewCoords(controller->getOperatorById(opId)->position), operatorPos, kBoxSize, kBoxSize);
        const auto modulatedPosition = closestPointInBox(operatorPos, operatorView_->toViewCoords(controller->getOperatorById(opId)->position), kBoxSize, kBoxSize);

        painter->setPen(getColorForOperator(controller->getOperatorById(opId).get()));
        painter->drawLine(modulatorPosition, modulatedPosition);
    }
}

void OperatorDrawer::drawBox(QPainter *painter, Operator* operator_) {
    auto color = getColorForOperator(operator_);

    painter->setBrush(QBrush(color));

    const auto& controller = Controller::instance;
    if (controller->selectedOperatorId().has_value() && controller->selectedOperatorId().value() == operator_->id) {
        painter->setPen(QPen(QColor(255, 255, 255), 1));
    }
    else {
        painter->setPen(Qt::PenStyle::NoPen);
    }

    painter->drawRoundedRect(QRectF(operatorView_->toViewCoords(operator_->position), QSize(kBoxSize, kBoxSize)), kCornerRadius, kCornerRadius);
}

bool OperatorDrawer::isInsideBox(Operator* operator_, const QPointF &coords) {
    const auto operatorPos = operatorView_->toViewCoords(operator_->position);
    return coords.x() >= operatorPos.x() &&
            coords.x() < operatorPos.x()  + kBoxSize && // coords.x is within x range of operator
            coords.y() >= operatorPos.y() &&
            coords.y() < operatorPos.y() + kBoxSize; // coords.y is within y range of operator
}

QColor OperatorDrawer::getColorForOperator(Operator *operator_) {
    return QColor((int) ((log10((double) operator_->frequency) / 4.38) * 255.0), 20, (int) ((log10((double) operator_->amplitude) / 4.38) * 255.0));
}
