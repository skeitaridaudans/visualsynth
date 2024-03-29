//
// Created by Guðmundur on 2/6/2023.
//

#include "OperatorView.h"
#include "src/Controllers/Controller/Controller.h"
#include "src/Utils/FontAwesome.h"

const double kBoxSize = 70.0;
const int kCarrierLineTextLeftMargin = 20;
const int kCarrierLineLeftMargin = 10;
const int kCarrierLineRightMargin = 20;
const double kCarrierLinePosition = 0.9;
const double kCarrierLinePulseAnimTime = 450.0;
const double kCarrierLinePulseAnimMin = 0.4;
const double kCarrierLinePulseAnimMax = 1.0;
const QString kCarrierIcon (fa::fa_music);
const QColor kOperatorLineColor (0xFFFFFF);

OperatorView::OperatorView(QQuickItem *parent) : QQuickPaintedItem(parent),
                                                 newBox_(std::make_unique<AddOperatorBox>(
                                                         const_cast<OperatorView *>(this))),
                                                 deleteOperatorBox_(std::make_unique<DeleteOperatorBox>(
                                                         const_cast<OperatorView *>(this))),
                                                 operatorDrawer_(std::make_unique<OperatorDrawer>(
                                                         const_cast<OperatorView *>(this))),
                                                 carrierLinePulseAnim_(kCarrierLinePulseAnimTime,
                                                                       &carrierLineBoxOpacity_,
                                                                       AnimationCurves::easeInOut,
                                                                       kCarrierLinePulseAnimMin,
                                                                       kCarrierLinePulseAnimMax, true) {
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void OperatorView::paint(QPainter *painter) {
    auto &controller = Controller::instance;

    for (auto &operator_: controller->operators()) {
        operatorDrawer_->update(operator_.second);
    }
    deleteOperatorBox_->update();
    newBox_->update();

    if (operatorDrawer_->draggedOperatorId().has_value()) {
        if (!carrierLinePulseAnim_.isRunning()) {
            carrierLinePulseAnim_.setForward();
        }
        carrierLinePulseAnim_.update();
    }
    else {
        if (carrierLinePulseAnim_.isRunning()) {
            carrierLinePulseAnim_.stop();
            carrierLineBoxOpacity_ = 1.0;
        }
    }

    newBox_->draw(painter);
    deleteOperatorBox_->draw(painter);
    drawCarrierLine(painter);
    for (const auto &operator_: controller->operators()) {
        if (operator_.second.operatorViewState.draggingState != DraggingState::None) {
            continue;
        }

        operatorDrawer_->draw(painter, operator_.second);
    }
    // Draw the operator that is being dragged last so that it is on top of the other operators
    if (operatorDrawer_->draggedOperatorId().has_value()) {
        const auto &selectedOperator = controller->getOperatorById(operatorDrawer_->draggedOperatorId().value());
        operatorDrawer_->draw(painter, selectedOperator);
    }

    // Make a copy of all operator ids that should be deleted, then delete them in another loop.
    // Since it's not possible to delete elements from unordered_map while iterating over it
    std::vector<int> operatorToDelete;
    for (const auto &operator_: controller->operators()) {
        if (operator_.second.scheduleForRemoval) {
            operatorToDelete.push_back(operator_.first);
        }
    }
    for (const auto &operatorId: operatorToDelete) {
        controller->removeOperator(operatorId);
    }

    if (touchPressHandledState_ == TouchEventHandledState::Unhandled) {
        controller->deselectOperator();
        controller->hidePresets();

        touchPressHandledState_ = TouchEventHandledState::None;
    }

    update();
}

void OperatorView::drawCarrierLine(QPainter *painter) {
    // Calculate positions
    const auto carrierLineTextStartPos = QPointF(kCarrierLineTextLeftMargin, height() * kCarrierLinePosition + 20);

    const auto lineEndPoints = carrierLineEndPoints();
    const auto &carrierLineLeftPoint = lineEndPoints.first;
    const auto &carrierLineRightPoint = lineEndPoints.second;

    // Draw carrier line
    painter->setFont(fontAwesome()->font(fa::fa_solid, 45));
    painter->setPen(QPen(kOperatorLineColor, 3));
    painter->drawText(carrierLineTextStartPos, kCarrierIcon);
    painter->setOpacity(carrierLineBoxOpacity_);
    painter->drawLine(carrierLineLeftPoint, carrierLineRightPoint);
    painter->setOpacity(1.0);
}

void OperatorView::addOperator(double x, double y) {
    auto &controller = Controller::instance;

    if (const auto id = controller->addOperator()) {
        auto &operator_ = controller->getOperatorById(*id);

        operator_.position = QPointF(x, y);
    }
}

std::pair<QPointF, QPointF> OperatorView::carrierLineEndPoints() {
    const QFontMetrics fm(fontAwesome()->font(fa::fa_solid, 45));
    const auto textWidth = fm.horizontalAdvance(kCarrierIcon);
    const auto textHeight = fm.height();

    const auto carrierLineLeftPoint = QPointF(kCarrierLineTextLeftMargin + textWidth + kCarrierLineLeftMargin,
                                              height() * kCarrierLinePosition);
    const auto carrierLineRightPoint = QPointF(width() - kCarrierLineRightMargin,
                                               height() * kCarrierLinePosition);

    return {carrierLineLeftPoint, carrierLineRightPoint};
}

const std::unique_ptr<DeleteOperatorBox> &OperatorView::deleteOperatorBox() {
    return deleteOperatorBox_;
}

// Converts from the coords stored in Operator (0-1) to coords inside of the operator view
QPointF OperatorView::toViewCoords(const QPointF &pos) {
    return {pos.x() * width() - kBoxSize / 2.0, pos.y() * height() - kBoxSize / 2.0};
}

// Converts from coords inside the operator view to the ones stored in operator (0-1)
QPointF OperatorView::fromViewCoords(const QPointF &pos) {
    return {(pos.x() + kBoxSize / 2.0) / width() , (pos.y() + kBoxSize / 2.0) / height()};
}

void OperatorView::touchEvent(QTouchEvent *event) {
    QQuickItem::touchEvent(event);

    const auto &points = event->points();
    switch (event->type()) {
        case QEvent::TouchBegin:
            if (!points.empty()) {
                primaryTouchPoint_.id = points.first().id();
                primaryTouchPoint_.isPressed = true;
                primaryTouchPoint_.initialPressHandled = false;
                primaryTouchPoint_.position = points.first().position();
                touchPressHandledState_ = TouchEventHandledState::Unhandled;
                isUsingMouse_ = false;

                if (points.count() > 1) {
                    secondaryTouchPoint_.id = points[1].id();
                    secondaryTouchPoint_.isPressed = true;
                    secondaryTouchPoint_.initialPressHandled = false;
                    secondaryTouchPoint_.position = points[1].position();
                }

                event->accept();
            }
            break;
        case QEvent::TouchUpdate:
            // Cancel the event if the first touch point is no longer pressed_
            if (!points.empty() && points.first().id() == primaryTouchPoint_.id) {
                primaryTouchPoint_.position = points.first().position();

                // The second touch point should be able to change any time
                if (points.count() > 1 &&
                    (!secondaryTouchPoint_.isPressed || secondaryTouchPoint_.id != points[1].id())) {
                    secondaryTouchPoint_.id = points[1].id();
                    secondaryTouchPoint_.isPressed = true;
                    secondaryTouchPoint_.initialPressHandled = false;
                    secondaryTouchPoint_.position = points[1].position();
                } else if (points.count() == 1) {
                    secondaryTouchPoint_.isPressed = false;
                }
            }
            event->accept();
            break;
        case QEvent::TouchEnd:
            primaryTouchPoint_.isPressed = false;
            secondaryTouchPoint_.isPressed = false;
            event->accept();
            break;
        case QEvent::TouchCancel:
            primaryTouchPoint_.isPressed = false;
            secondaryTouchPoint_.isPressed = false;
            event->accept();
            break;
        default:
            break;
    }


}

const TouchPoint &OperatorView::primaryTouchPoint() {
    return primaryTouchPoint_;
}

TouchPoint &OperatorView::secondaryTouchPoint() {
    return secondaryTouchPoint_;
}

void OperatorView::mousePressEvent(QMouseEvent *event) {
    QQuickItem::mousePressEvent(event);

    primaryTouchPoint_.isPressed = true;
    primaryTouchPoint_.initialPressHandled = false;
    primaryTouchPoint_.position = event->position();
    touchPressHandledState_ = TouchEventHandledState::Unhandled;
    isUsingMouse_ = true;
    event->accept();
}

void OperatorView::mouseMoveEvent(QMouseEvent *event) {
    QQuickItem::mouseMoveEvent(event);

    primaryTouchPoint_.position = event->position();
    event->accept();
}

void OperatorView::mouseReleaseEvent(QMouseEvent *event) {
    QQuickItem::mouseReleaseEvent(event);

    primaryTouchPoint_.isPressed = false;
    primaryTouchPoint_.position = event->position();
    event->accept();
}

bool OperatorView::isUsingMouse() {
    return isUsingMouse_;
}
