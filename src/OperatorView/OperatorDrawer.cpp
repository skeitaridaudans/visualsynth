//
// Created by Guðmundur on 2/6/2023.
//

#include "OperatorDrawer.h"
#include <QCursor>
#include <QGuiApplication>
#include "src/Controller/Controller.h"
#include "src/Utils/Utils.h"
#include "src/FontAwesome.h"

// Operator appearance
const double kBoxSize = 70.0;
const double kCornerRadius = 5.0;

// Dragging options
const double kDragSensitivity = 5.0;
const double kOperatorHoldToDragTime = 400.0;
const double kMinDistanceBetweenOperators = kBoxSize + 10.0;
const double kFixOperatorPositionAnimTime = 100.0;
const double kOperatorSizeMultiplier = 1.0;
const double kOperatorDraggingSizeMultiplier = 1.2;
const double kOperatorOpacity = 1.0;
const double kOperatorDraggingOpacity = 0.6;
const double kOperatorDragStateChangeAnimTime = 300.0;

// Plus/minus icon on operator
const double kOperatorConnectIconPulseAnimationTime = 300.0;
const double KOperatorConnectIconMinOpacity = 0.3;
const double KOperatorConnectIconMaxOpacity = 1.0;

// Modulator line
const double kModulatorArrowTriangleSize = 8.0;
const double kModulatorLineWidth = 4.0;
const int kModulatorCurvedLinePointCount = 20;
const double kModulatorLineCurve = 30.0;
const double kCurvedLineMargin = 6.0;


OperatorDrawer::OperatorDrawer(OperatorView *operatorView) : operatorView_(operatorView) {
}

void OperatorDrawer::update(Operator &operator_) {
    const auto primaryTouchPointPos = operatorView_->primaryTouchPoint().position;

    updateAnimations(operator_);

    // Only update animations if freezeInteraction is enabled
    if (operator_.operatorViewState.freezeInteraction) {
        return;
    }

    // Handle adding modulator by pressing with secondary touch point
    if (currentDraggingOperatorState_ == DraggingState::Dragging) {
        auto &secondaryTouchPoint = operatorView_->secondaryTouchPoint();

        if (secondaryTouchPoint.isPressed && !secondaryTouchPoint.initialPressHandled &&
            isInsideBox(operator_, secondaryTouchPoint.position)) {
            toggleModulator(operator_, draggedOperatorId_.value());

            secondaryTouchPoint.initialPressHandled = true;
        }
    }

    // When the operator is initially pressed
    if (operator_.operatorViewState.draggingState == DraggingState::None &&
        isInsideBox(operator_, primaryTouchPointPos) &&
        operatorView_->primaryTouchPoint().isPressed && currentDraggingOperatorState_ == DraggingState::None) {
        onTouchDown(operator_);
    }

    // While holding down when the operator hasn't been moved
    else if (operator_.operatorViewState.draggingState == DraggingState::Holding) {
        const auto moveVector = operator_.operatorViewState.initialDragCursorPos.value() - primaryTouchPointPos;
        const auto cursorMoveDistance = std::sqrt(QPointF::dotProduct(moveVector, moveVector));

        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto timeElapsedSinceClick = std::chrono::duration<double, std::milli>(
                currentTime - operator_.operatorViewState.initialClickTime.value()).count();

        // Start dragging
        if (cursorMoveDistance >= kDragSensitivity || timeElapsedSinceClick > kOperatorHoldToDragTime) {
            startDragging(operator_);
        }

        // When operator is pressed (touch is released before dragging starts)
        else if (isInsideBox(operator_, primaryTouchPointPos) && !operatorView_->primaryTouchPoint().isPressed) {
            onOperatorPressed(operator_);
        }
    }

    // While dragging the operator
    if (operator_.operatorViewState.draggingState == DraggingState::Dragging &&
        operatorView_->primaryTouchPoint().isPressed) {
        updateOperatorDrag(operator_);
    }

    // When the operator is released
    else if (operator_.operatorViewState.draggingState == DraggingState::Dragging) {
        releaseOperator(operator_);
    }
}

void OperatorDrawer::toggleModulator(Operator &operator_, int modulatorId) {
    const auto &controller = Controller::instance;

    if (std::count(operator_.modulatedBy.begin(), operator_.modulatedBy.end(),
                   modulatorId) == 0) {
        controller->addModulator(operator_.id, modulatorId);
    } else {
        controller->removeModulator(operator_.id, modulatorId);
    }
}

void OperatorDrawer::onTouchDown(Operator &operator_) {
    operator_.operatorViewState.initialDragCursorPos = operatorView_->primaryTouchPoint().position;
    operator_.operatorViewState.draggingState = DraggingState::Holding;
    operator_.operatorViewState.moveOperatorAnimation = std::nullopt;
    operator_.operatorViewState.initialClickTime = std::chrono::high_resolution_clock::now();

    // This is so that other operators can know that this operator is being dragged
    currentDraggingOperatorState_ = DraggingState::Holding;
    draggedOperatorId_ = operator_.id;

    operatorView_->touchPressHandledState_ = TouchEventHandledState::Handled;
}

void OperatorDrawer::startDragging(Operator &operator_) {
    const auto &controller = Controller::instance;

    operator_.operatorViewState.draggingState = DraggingState::Dragging;
    operator_.operatorViewState.initialDragCursorPos = std::nullopt;
    operator_.operatorViewState.initialClickTime = std::nullopt;

    currentDraggingOperatorState_ = DraggingState::Dragging;

    if (!operator_.operatorViewState.sizeMultiplierAnim.has_value()) {
        operator_.operatorViewState.sizeMultiplierAnim = TweenAnimation(kOperatorDragStateChangeAnimTime,
                                                                        &operator_.operatorViewState.sizeMultiplier,
                                                                        AnimationCurves::easeOutBack,
                                                                        kOperatorSizeMultiplier,
                                                                        kOperatorDraggingSizeMultiplier);
    }
    if (!operator_.operatorViewState.opacityAnim.has_value()) {
        operator_.operatorViewState.opacityAnim = TweenAnimation(kOperatorDragStateChangeAnimTime,
                                                                 &operator_.operatorViewState.opacity,
                                                                 AnimationCurves::easeOut,
                                                                 kOperatorOpacity,
                                                                 kOperatorDraggingOpacity);
    }

    operator_.operatorViewState.sizeMultiplierAnim->setForward();
    operator_.operatorViewState.opacityAnim->setForward();

    controller->deselectOperator();
}

void OperatorDrawer::onOperatorPressed(Operator &operator_) {
    const auto &controller = Controller::instance;
    const auto selectedOperatorId = controller->selectedOperatorId();

    if (selectedOperatorId.has_value() && selectedOperatorId.value() == operator_.id) {
        controller->deselectOperator();
    } else if (operatorView_->isUsingMouse() && selectedOperatorId.has_value()) {
        // Allow adding modulator when using a mouse, if we are using touch screen,
        // then adding modulator should be done while dragging
        toggleModulator(operator_, selectedOperatorId.value());

        controller->deselectOperator();
    } else {
        controller->selectOperator(operator_.id);
    }

    operator_.operatorViewState.draggingState = DraggingState::None;
    currentDraggingOperatorState_ = DraggingState::None;
    draggedOperatorId_ = std::nullopt;
}

void OperatorDrawer::updateOperatorDrag(Operator &operator_) {
    const auto &controller = Controller::instance;
    const auto touchPointPos = operatorView_->primaryTouchPoint().position;
    const auto scaledBoxSize = kBoxSize * operator_.operatorViewState.sizeMultiplier;
    const auto operatorPos = operatorView_->fromViewCoords(
            QPointF(touchPointPos.x() - scaledBoxSize / 2.0, touchPointPos.y() - scaledBoxSize / 2.0));
    operator_.position.setX(operatorPos.x());
    operator_.position.setY(operatorPos.y());

    const auto carrierLinePoints = operatorView_->carrierLineEndPoints();
    if (isRectInsideLine(QRectF(operatorView_->toViewCoords(operator_.position), QSizeF(kBoxSize, kBoxSize)),
                         carrierLinePoints.first, carrierLinePoints.second)) {
        controller->addCarrier(operator_.id);
        operator_.position.setY(carrierLinePoints.first.y() / operatorView_->height());
    } else {
        controller->removeCarrier(operator_.id);
    }
}

void OperatorDrawer::releaseOperator(Operator &operator_) {
    operator_.operatorViewState.draggingState = DraggingState::None;
    currentDraggingOperatorState_ = DraggingState::None;
    draggedOperatorId_ = std::nullopt;

    // If cursor is inside delete box
    if (operatorView_->deleteOperatorBox()->isInsideDeleteArea(operatorView_->primaryTouchPoint().position)) {
        operator_.scheduleForRemoval = true;
    } else {
        fixOperatorPositionAfterDrop(operator_);
    }

    operator_.operatorViewState.sizeMultiplierAnim->setReverse();
    operator_.operatorViewState.opacityAnim->setReverse();
}

void OperatorDrawer::updateAnimations(Operator &operator_) {
    const auto &controller = Controller::instance;

    // Move operator animation (used when correcting the operator position)
    if (operator_.operatorViewState.moveOperatorAnimation.has_value()) {
        operator_.operatorViewState.moveOperatorAnimation->update();

        if (operator_.operatorViewState.moveOperatorAnimation->isAtEnd()) {
            operator_.operatorViewState.moveOperatorAnimation = std::nullopt;
        }
    }

    // Dragging start/end animation
    if (operator_.operatorViewState.sizeMultiplierAnim.has_value()) {
        operator_.operatorViewState.sizeMultiplierAnim->update();
    }
    if (operator_.operatorViewState.opacityAnim.has_value()) {
        operator_.operatorViewState.opacityAnim->update();
    }

    // Pulse animation
    if ((!operatorView_->isUsingMouse() &&
         currentDraggingOperatorState_ == DraggingState::Dragging &&
         operator_.operatorViewState.draggingState == DraggingState::None) ||
        (operatorView_->isUsingMouse() &&
         controller->selectedOperatorId().has_value() &&
         controller->selectedOperatorId().value() != operator_.id)) {
        if (!operator_.operatorViewState.connectIconOpacityAnim.has_value()) {
            operator_.operatorViewState.connectIconOpacityAnim = TweenAnimation(
                    kOperatorConnectIconPulseAnimationTime, &operator_.operatorViewState.connectIconOpacity,
                    AnimationCurves::easeInOut, KOperatorConnectIconMinOpacity,
                    KOperatorConnectIconMaxOpacity, true);
            operator_.operatorViewState.connectIconOpacityAnim->setForward();
        }
        operator_.operatorViewState.connectIconOpacityAnim->update();
    } else if (operator_.operatorViewState.connectIconOpacityAnim.has_value()) {
        operator_.operatorViewState.connectIconOpacityAnim = std::nullopt;
    }
}

void OperatorDrawer::draw(QPainter *painter, const Operator &operator_) {
    drawBox(painter, operator_);

    // Draw modulator lines
    const auto &controller = Controller::instance;
    for (const auto opId: operator_.modulatedBy) {
        const auto &modulatorOp = controller->getOperatorById(opId);
        const auto modulatorOpPos = operatorView_->toViewCoords(modulatorOp.position);
        // Subtract 0.05x the size of the operator from the position so the arrow isn't touching the box from the top/left
        const auto modulatedOpPos = operatorView_->toViewCoords(operator_.position) -
                                    QPointF(1, 1) * (kBoxSize * operator_.operatorViewState.sizeMultiplier * 0.05);

        const auto modulatorBoxSize = kBoxSize * modulatorOp.operatorViewState.sizeMultiplier;
        // Multiply by 1.1 so that the arrow isn't fully touching the box
        const auto modulatedBoxSize = kBoxSize * operator_.operatorViewState.sizeMultiplier * 1.1;
        const auto modulatorPos = closestPointInBox(modulatedOpPos, modulatorOpPos, modulatorBoxSize, modulatorBoxSize);
        const auto modulatedPos = closestPointInBox(modulatorOpPos, modulatedOpPos, modulatedBoxSize, modulatedBoxSize);

        if (std::count(modulatorOp.modulatedBy.begin(), modulatorOp.modulatedBy.end(), operator_.id) != 0) {
            drawCurvedModulatorLine(painter, modulatorOp, modulatorPos, modulatedPos);
        }
        else {
            drawModulatorLine(painter, modulatorOp, modulatorPos, modulatedPos);
        }
    }
}

void OperatorDrawer::drawModulatorLine(QPainter *painter, const Operator &modulatorOp, const QPointF &modulatorPos,
                                       const QPointF &modulatedPos) {
    const auto backwardVector = vectorBetweenPoints(modulatedPos, modulatorPos).normalized();
    const auto perpToLine = QVector2D(-backwardVector.y(), backwardVector.x());
    const auto pointA = pointToVector(modulatedPos) + (backwardVector * kModulatorArrowTriangleSize) +
                        (perpToLine * kModulatorArrowTriangleSize);
    const auto pointB = pointToVector(modulatedPos) + (backwardVector * kModulatorArrowTriangleSize) -
                        (perpToLine * kModulatorArrowTriangleSize);

    const QPointF arrowEndTrianglePoints[3] = {
            modulatedPos,
            vectorToPoint(pointA),
            vectorToPoint(pointB),
    };

    painter->setPen(QPen(modulatorOp.getColorForOperator(), kModulatorLineWidth));
    painter->setBrush(modulatorOp.getColorForOperator());
    painter->drawLine(modulatorPos, modulatedPos);
    painter->drawPolygon(arrowEndTrianglePoints, 3);
}

void
OperatorDrawer::drawCurvedModulatorLine(QPainter *painter, const Operator &modulatorOp, const QPointF &modulatorPos,
                                        const QPointF &modulatedPos) {
    const auto lineVector = vectorBetweenPoints(modulatorPos, modulatedPos);
    const auto perpToLine = QVector2D(-lineVector.normalized().y(), lineVector.normalized().x());
    const auto curveControlPoint = pointToVector(modulatorPos) + lineVector * 0.5 + perpToLine * (lineVector.length() / 4);

    const auto startPoint = pointToVector(modulatorPos) + perpToLine * kCurvedLineMargin;
    const auto endPoint = pointToVector(modulatedPos) + perpToLine * kCurvedLineMargin;

    std::vector<QPointF> linePoints;
    for (int i = 0; i < kModulatorCurvedLinePointCount; i++) {
        // Divide by number of points - 1, since that is the index of the last point
        const double fraction = static_cast<double>(i) / static_cast<double>(kModulatorCurvedLinePointCount-1);
        linePoints.push_back(vectorToPoint(bezierCurve(startPoint, curveControlPoint, endPoint, fraction)));
    }

    painter->setPen(QPen(modulatorOp.getColorForOperator(), kModulatorLineWidth));
    painter->setBrush(modulatorOp.getColorForOperator());
    painter->drawPolyline(linePoints.data(), kModulatorCurvedLinePointCount);

    // Draw arrow at end of line
    const auto backwardVector = vectorBetweenPoints(linePoints[linePoints.size()-1], linePoints[linePoints.size()-2]).normalized();
    const auto perpToEndOfLine = QVector2D(-backwardVector.y(), backwardVector.x());
    const auto pointA = endPoint + (backwardVector * kModulatorArrowTriangleSize) +
                        (perpToEndOfLine * kModulatorArrowTriangleSize);
    const auto pointB = endPoint + (backwardVector * kModulatorArrowTriangleSize) -
                        (perpToEndOfLine * kModulatorArrowTriangleSize);

    const QPointF arrowEndTrianglePoints[3] = {
            vectorToPoint(endPoint),
            vectorToPoint(pointA),
            vectorToPoint(pointB),
    };
    painter->drawPolygon(arrowEndTrianglePoints, 3);
}

void OperatorDrawer::drawBox(QPainter *painter, const Operator &operator_) {
    auto color = operator_.getColorForOperator();
    const auto operatorPos = operatorView_->toViewCoords(operator_.position);

    painter->setBrush(QBrush(color));

    const auto &controller = Controller::instance;
    if (controller->selectedOperatorId().has_value() && controller->selectedOperatorId().value() == operator_.id) {
        painter->setPen(QPen(QColor(255, 255, 255), 1));
    } else {
        painter->setPen(Qt::PenStyle::NoPen);
    }

    painter->setOpacity(operator_.operatorViewState.opacity);
    const auto operatorBoxSize = kBoxSize * operator_.operatorViewState.sizeMultiplier;
    const auto operatorBoxRect = QRectF(operatorPos, QSizeF(operatorBoxSize, operatorBoxSize));
    painter->drawRoundedRect(operatorBoxRect, kCornerRadius, kCornerRadius);
    painter->setOpacity(1.0);

    if (!operatorView_->isUsingMouse() &&
        currentDraggingOperatorState_ == DraggingState::Dragging &&
        operator_.operatorViewState.draggingState == DraggingState::None) {
        drawOperatorConnectIcon(painter, operatorBoxRect, operator_, draggedOperatorId_.value());
    } else if (operatorView_->isUsingMouse() &&
               controller->selectedOperatorId().has_value() &&
               controller->selectedOperatorId().value() != operator_.id) {
        drawOperatorConnectIcon(painter, operatorBoxRect, operator_, controller->selectedOperatorId().value());
    }
}

void
OperatorDrawer::drawOperatorConnectIcon(QPainter *painter, const QRectF &operatorBoxRect, const Operator &operator_,
                                        const int selectedOperatorId) {
    const auto isModulatedByDragged =
            std::count(operator_.modulatedBy.begin(), operator_.modulatedBy.end(), selectedOperatorId) != 0;

    painter->setPen(QPen(QColor(255, 255, 255)));
    painter->setOpacity(operator_.operatorViewState.connectIconOpacity);
    painter->setFont(fontAwesome()->font(fa::fa_solid, 24));
    painter->drawText(operatorBoxRect, Qt::AlignCenter,
                      QString(!isModulatedByDragged ? fa::fa_plus_circle : fa::fa_minus_circle));
    painter->setOpacity(1.0);
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

    const auto operatorPos = operator_.operatorViewState.moveOperatorAnimation.has_value()
                             ? operator_.operatorViewState.moveOperatorAnimation->toPoint_
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

            if (operator_.operatorViewState.moveOperatorAnimation.has_value()) {
                operator_.operatorViewState.moveOperatorAnimation->toPoint_ = newPosition;
            } else {
                operator_.operatorViewState.moveOperatorAnimation.emplace(
                        PointTweenAnimation(kFixOperatorPositionAnimTime, &operator_.position,
                                            operatorPos, newPosition,
                                            AnimationCurves::easeOutBack));
                operator_.operatorViewState.moveOperatorAnimation->setForward();
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
        if (operator_.operatorViewState.moveOperatorAnimation.has_value()) {
            operator_.operatorViewState.moveOperatorAnimation->toPoint_ = updatedPosition;
        } else {
            operator_.operatorViewState.moveOperatorAnimation.emplace(
                    PointTweenAnimation(kFixOperatorPositionAnimTime, &operator_.position, operatorPos,
                                        updatedPosition, AnimationCurves::easeOutBack));
            operator_.operatorViewState.moveOperatorAnimation->setForward();
        }
    }
}

const std::optional<int> &OperatorDrawer::draggedOperatorId() {
    return draggedOperatorId_;
}
