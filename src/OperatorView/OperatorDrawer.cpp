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
const double kMinDistanceBetweenOperators = kBoxSize + 10.0;
const double kFixOperatorPositionAnimTime = 100.0;

OperatorDrawer::OperatorDrawer(OperatorView *operatorView) : operatorView_(operatorView) {
}

void OperatorDrawer::update(Operator &operator_) {
    const auto touchPointPos = operatorView_->touchPoint().position;
    const auto &controller = Controller::instance;

    if (operator_.moveOperatorAnimation.has_value()) {
        operator_.moveOperatorAnimation->update();

        if (operator_.moveOperatorAnimation->isAtEnd()) {
            operator_.position = operator_.moveOperatorAnimation->value();
            operator_.moveOperatorAnimation = std::nullopt;
        }
    }

    // The initial click
    if (operator_.draggingState == DraggingState::None && isInsideBox(operator_, touchPointPos) &&
        operatorView_->touchPoint().isPressed && !isPointBeingDragged_) {
        operator_.initialDragCursorPos = touchPointPos;
        operator_.draggingState = DraggingState::Holding;
        operator_.moveOperatorAnimation = std::nullopt;
        // Is any point being dragged, used to make sure only one point can be dragged at once
        isPointBeingDragged_ = true;
        operatorView_->touchPressHandledState_ = TouchEventHandledState::Handled;
    }
        // While holding down when the operator hasn't been moved
    else if (operator_.draggingState == DraggingState::Holding) {
        const auto moveVector = operator_.initialDragCursorPos.value() - touchPointPos;
        const auto cursorMoveDistance = std::sqrt(QPointF::dotProduct(moveVector, moveVector));

        // Start dragging
        if (cursorMoveDistance >= kDragSensitivity) {
            operator_.draggingState = DraggingState::Dragging;
            operator_.initialDragCursorPos = std::nullopt;
            controller->deselectOperator();
        }

        // Click
        if (isInsideBox(operator_, touchPointPos) && !operatorView_->touchPoint().isPressed) {
            const auto selectedOperatorId = controller->selectedOperatorId();

            if (selectedOperatorId.has_value() && selectedOperatorId.value() == operator_.id) {
                controller->deselectOperator();
            } else if (selectedOperatorId.has_value()) {
                if (std::count(operator_.modulatedBy.begin(), operator_.modulatedBy.end(),
                               selectedOperatorId.value()) == 0) {
                    controller->addModulator(operator_.id, selectedOperatorId.value());
                } else {
                    controller->removeModulator(operator_.id, selectedOperatorId.value());
                }

                controller->deselectOperator();
            } else {
                controller->selectOperator(operator_.id);
            }

            operator_.draggingState = DraggingState::None;
            isPointBeingDragged_ = false;
        }
    }
    // While dragging the operator
    if (operator_.draggingState == DraggingState::Dragging && operatorView_->touchPoint().isPressed) {
        const auto operatorPos = operatorView_->fromViewCoords(
                QPointF(touchPointPos.x() - kBoxSize / 2.0, touchPointPos.y() - kBoxSize / 2.0));
        operator_.position.setX(operatorPos.x());
        operator_.position.setY(operatorPos.y());

        const auto carrierLinePoints = operatorView_->carrierLineEndPoints();
        if (isRectInsideLine(QRectF(operatorView_->toViewCoords(operator_.position), QSizeF(kBoxSize, kBoxSize)),
                             carrierLinePoints.first, carrierLinePoints.second)) {
            controller->addCarrier(operator_.id);
            operator_.position.setY((carrierLinePoints.first.y() - kBoxSize / 2.0) / operatorView_->height());
        } else {
            controller->removeCarrier(operator_.id);
        }
    }
        // When the operator is released
    else if (operator_.draggingState == DraggingState::Dragging) {
        operator_.draggingState = DraggingState::None;
        isPointBeingDragged_ = false;

        // If cursor is inside delete box
        if (operatorView_->deleteOperatorBox()->isInsideBox(touchPointPos)) {
            operator_.scheduleForRemoval = true;
        } else {
            fixOperatorPositionAfterDrop(operator_);
        }
    }
}

void OperatorDrawer::draw(QPainter *painter, const Operator &operator_) {
    drawBox(painter, operator_);

    // Draw modulator lines
    const auto&controller = Controller::instance;
    for (const auto opId: operator_.modulatedBy) {
        const auto &modulatedOp = controller->getOperatorById(opId);
        const auto modulatedOpPos = operatorView_->toViewCoords(
                !operator_.moveOperatorAnimation.has_value() ? operator_.position
                                                              : operator_.moveOperatorAnimation->value());
        const auto modulatorOpPos = operatorView_->toViewCoords(
                !modulatedOp.moveOperatorAnimation.has_value() ? modulatedOp.position
                                                                : modulatedOp.moveOperatorAnimation->value());

        const auto modulatorPosition = closestPointInBox(modulatorOpPos, modulatedOpPos, kBoxSize, kBoxSize);
        const auto modulatedPosition = closestPointInBox(modulatedOpPos, modulatorOpPos, kBoxSize, kBoxSize);

        painter->setPen(controller->getOperatorById(opId).getColorForOperator());
        painter->drawLine(modulatorPosition, modulatedPosition);
    }
}

void OperatorDrawer::drawBox(QPainter *painter, const Operator& operator_) {
    auto color = operator_.getColorForOperator();
    const auto operatorPos = operatorView_->toViewCoords(
            !operator_.moveOperatorAnimation.has_value() ? operator_.position
                                                          : operator_.moveOperatorAnimation->value());

    painter->setBrush(QBrush(color));

    const auto &controller = Controller::instance;
    if (controller->selectedOperatorId().has_value() && controller->selectedOperatorId().value() == operator_.id) {
        painter->setPen(QPen(QColor(255, 255, 255), 1));
    } else {
        painter->setPen(Qt::PenStyle::NoPen);
    }

    painter->drawRoundedRect(QRectF(operatorPos, QSize(kBoxSize, kBoxSize)),
                             kCornerRadius, kCornerRadius);
}

bool OperatorDrawer::isInsideBox(const Operator &operator_, const QPointF &coords) {
    const auto operatorPos = operatorView_->toViewCoords(operator_.position);
    return coords.x() >= operatorPos.x() &&
           coords.x() < operatorPos.x() + kBoxSize && // coords.x is within x range of operator
           coords.y() >= operatorPos.y() &&
           coords.y() < operatorPos.y() + kBoxSize; // coords.y is within y range of operator
}

void OperatorDrawer::fixOperatorPositionAfterDrop(Operator &operator_, float moveMultiplier) {
    const auto &controller = Controller::instance;

    const auto operatorPos = operator_.moveOperatorAnimation.has_value() ? operator_.moveOperatorAnimation->toPoint_
                                                                          : operator_.position;
    for (const auto &otherOp: controller->operators()) {
        if (otherOp.second.id == operator_.id) continue;

        // X and y coordinates have different scales,
        // since both are between 0 and 1 but the width and height are different,
        // which means that the distance the box needs to be moved in x and y coordinates
        // is different and needs to be calculated separately
        const auto minDistanceX = kMinDistanceBetweenOperators / operatorView_->width();
        const auto minDistanceY = kMinDistanceBetweenOperators / operatorView_->height();
        const auto betweenPoints = vectorBetweenPoints(otherOp.second.position, operatorPos);
        if (abs(betweenPoints.x()) < minDistanceX && abs(betweenPoints.y()) < minDistanceY) {
            const auto expectedX = betweenPoints.normalized().x() * minDistanceX;
            const auto expectedY = betweenPoints.normalized().y() * minDistanceY;
            // Multiply the difference to make sure it moves more than the distance that is checked
            // and to prevent infinite recursion by always moving further in every recursion
            const auto expectedVector = QVector2D(expectedX, expectedY) * moveMultiplier;
            const auto difference = expectedVector - betweenPoints;

            const auto newPosition = operatorPos + difference;

            if (operator_.moveOperatorAnimation.has_value()) {
                operator_.moveOperatorAnimation->toPoint_ = newPosition;
            } else {
                operator_.moveOperatorAnimation = PointTweenAnimation(kFixOperatorPositionAnimTime,
                                                                       operatorPos, newPosition,
                                                                       AnimationCurves::easeOutBack);
                operator_.moveOperatorAnimation->setForward();
            }

            fixOperatorPositionAfterDrop(operator_, moveMultiplier + 0.1f);
            return;
        }
    }

    const auto operatorCoordsWidth = kBoxSize / operatorView_->width();
    const auto operatorCoordsHeight = kBoxSize / operatorView_->height();
    auto updatedPosition = operatorPos;
    bool positionUpdated = false;
    if (operatorPos.x() < 0) {
        updatedPosition.setX(0);
        positionUpdated = true;
    } else if (operatorPos.x() > 1.0 - operatorCoordsWidth) {
        updatedPosition.setX(1.0 - operatorCoordsWidth);
        positionUpdated = true;
    }

    if (operatorPos.y() < 0) {
        updatedPosition.setY(0);
        positionUpdated = true;
    } else if (operatorPos.y() > 1.0 - operatorCoordsHeight) {
        updatedPosition.setY(1.0 - operatorCoordsHeight);
        positionUpdated = true;
    }

    if (positionUpdated) {
        if (operator_.moveOperatorAnimation.has_value()) {
            operator_.moveOperatorAnimation->toPoint_ = updatedPosition;
        } else {
            operator_.moveOperatorAnimation = PointTweenAnimation(kFixOperatorPositionAnimTime, operatorPos,
                                                                   updatedPosition, AnimationCurves::easeOutBack);
            operator_.moveOperatorAnimation->setForward();
        }
    }
}
