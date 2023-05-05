//
// Created by Sigurður on 3/3/2023.
//

// AmpEnvGraphView.cpp
#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <QQuickItem>
#include <QMouseEvent>
#include "AmpEnvGraphView.h"
#include "src/Controller/Controller.h"

const double kPointHeight = 20;
const double kPointWidth = 20;
const double kPointDragAreaSize = 40;
const QColor kAmpEnvelopeGraphColor(128, 0, 128);
const QColor kNoOperatorSelectedGraphColor(0x424242);
const QColor kParamBorderColor(0x212121);
const double kViewAttackProportion = 0.8;
const double kViewReleaseProportion = 1.0 - kViewAttackProportion;
const double kDraggingStartAnimTime = 300.0;
const double kPointMargin = (kPointHeight / 2.0) + 4.0;
const std::vector<AmpEnvValue> kPlaceholderAttackEnvelope = {
        AmpEnvValue(0, 0.0, 0.0, true),
        AmpEnvValue(1, 0.9, 0.1, true),
        AmpEnvValue(2, 0.5, 0.3, true),
        AmpEnvValue(3, 0.5, 1.0, true),
};

const std::vector<AmpEnvValue> kPlaceholderReleaseEnvelope = {
        AmpEnvValue(0, 0.5, 0.0, false),
        AmpEnvValue(1, 0.0, 1.0, false),
};

AmpEnvGraphView::AmpEnvGraphView(QQuickItem *parent) :
        QQuickPaintedItem(parent),
        paramOpacityAnim_(kDraggingStartAnimTime, &draggingParamOpacity_, AnimationCurves::easeOut, 1.0, 0.6),
        paramScaleAnim_(kDraggingStartAnimTime, &draggingParamScale_, AnimationCurves::easeOutBack, 1.0, 1.4) {
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void AmpEnvGraphView::paintParams(QPainter *painter) {
    for (const auto &ampEnvParam: currentAttackEnvelope()) {
        // Skip drawing the first point in amp envelope (as it is at 0,0 and cannot be moved)
        if (ampEnvParam.index == 0 && !operatorEnvelope_) continue;

        paintParam(painter, ampEnvParam);
    }

    for (const auto &ampEnvParam: currentReleaseEnvelope()) {
        paintParam(painter, ampEnvParam);
    }

    // Reset opacity after painting the params
    painter->setOpacity(1.0);
}

void AmpEnvGraphView::paintParam(QPainter *painter, const AmpEnvValue &param) {
    const auto isBeingDragged = draggingAnimParamIndex_ == param.index;

    const auto coords = getDrawingPosOfAmpEnvParam(param);
    const auto pointWidth = kPointWidth * (isBeingDragged ? draggingParamScale_ : 1.0);
    const auto pointHeight = kPointHeight * (isBeingDragged ? draggingParamScale_ : 1.0);
    const auto rect = QRectF(coords.x() - (pointWidth / 2), coords.y() - (pointHeight / 2), pointWidth,
                             pointHeight);

    QBrush brush(graphColor());
    painter->setBrush(brush);
    painter->setPen(QPen(paramBorderColor(), 1));
    painter->setOpacity(isBeingDragged ? draggingParamOpacity_ : 1.0);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawRect(rect);
}

void AmpEnvGraphView::paintLines(QPainter *painter) {
    const auto &attackAmpEnvParams = currentAttackEnvelope();
    const auto &releaseAmpEnvParams = currentReleaseEnvelope();

    std::vector<QPointF> linePoints;
    for (const auto &ampEnvParam: attackAmpEnvParams) {
        const auto coords = getDrawingPosOfAmpEnvParam(ampEnvParam);
        linePoints.emplace_back(coords);
    }

    for (const auto &ampEnvParam: releaseAmpEnvParams) {
        const auto coords = getDrawingPosOfAmpEnvParam(ampEnvParam);
        linePoints.emplace_back(coords);
    }

    painter->setPen(QPen(graphColor(), 5));
    painter->drawPolyline(linePoints.data(), linePoints.size());
}


void AmpEnvGraphView::paint(QPainter *painter) {
    paramOpacityAnim_.update();
    paramScaleAnim_.update();

    painter->setRenderHint(QPainter::Antialiasing);

    this->paintLines(painter);
    this->paintParams(painter);

    update();
}

QPointF AmpEnvGraphView::mapAmpEnvPointToView(const QPointF &point, bool isAttack) {
    // Calculates the position of a point inside the view based on coordinates from the amp envelope (0-1)
    return isAttack
           ? QPointF(kPointMargin + point.x() * draggableAreaWidth() * kViewAttackProportion,
                     kPointMargin + (1 - point.y()) * draggableAreaHeight())
           : QPointF(kPointMargin + draggableAreaWidth() * kViewAttackProportion +
                     point.x() * draggableAreaWidth() * kViewReleaseProportion,
                     kPointMargin + (1 - point.y()) * draggableAreaHeight());
}

// Calculate coordinates to use in the amp envelope (0-1) from coordinates inside the view
QPointF AmpEnvGraphView::mapViewPointToAmpEnvPoint(const QPointF &point, bool isAttack) {
    return isAttack
           ? QPointF((point.x() - kPointMargin) / (draggableAreaWidth() * kViewAttackProportion),
                     1 - ((point.y() - kPointMargin) / draggableAreaHeight()))
           : QPointF(((point.x() - kPointMargin - draggableAreaWidth() * kViewAttackProportion) /
                      (draggableAreaWidth() * kViewReleaseProportion)),
                     1 - ((point.y() - kPointMargin) / draggableAreaHeight()));
}

// Prevents a point in the amp envelope from going in front of or behind the previous and next point
// This is only intended to be used for attack points, as those are the only ones that can be moved
QPointF AmpEnvGraphView::clampBetweenAdjacentPoints(const AmpEnvValue &ampEnvValue, const QPointF &desiredPos) {
    const auto &envParams = ampEnvValue.attack ? currentAttackEnvelope() : currentReleaseEnvelope();

    double minX = 0.0;
    if (ampEnvValue.index > 0) {
        minX = envParams[ampEnvValue.index - 1].time;
    }

    double maxX = 1.0;
    if (ampEnvValue.index < envParams.size() - 1) {
        maxX = envParams[ampEnvValue.index + 1].time;
    }

    return {
            std::clamp(desiredPos.x(), minX, maxX),
            std::clamp(desiredPos.y(), 0.0, 1.0)
    };
}

QPointF AmpEnvGraphView::getDrawingPosOfAmpEnvParam(const AmpEnvValue &param) {
    return mapAmpEnvPointToView(QPointF(param.time, param.value), param.attack);
}

const AmpEnvValue &AmpEnvGraphView::getDraggingAmpEnvValue() {
    return draggingTouchPoint_->isAttack
           ? currentAttackEnvelope()[draggingTouchPoint_->ampEnvPointIndex]
           : currentReleaseEnvelope()[draggingTouchPoint_->ampEnvPointIndex];
}

const AmpEnvValue *AmpEnvGraphView::findTouchedAmpEnvPoint(const QPointF &touchPoint) {
    for (const auto &ampEnvValue: currentAttackEnvelope()) {
        const auto valueCoords = getDrawingPosOfAmpEnvParam(ampEnvValue);

        if (vectorBetweenPoints(touchPoint, valueCoords).length() < kPointDragAreaSize) {
            return &ampEnvValue;
        }
    }

    const auto &releaseAmpEnvParams = currentReleaseEnvelope();
    if (!releaseAmpEnvParams.empty()) {
        const auto &releaseEnvLastParam = releaseAmpEnvParams[releaseAmpEnvParams.size() - 1];
        const auto valueCoords = getDrawingPosOfAmpEnvParam(releaseEnvLastParam);

        if (vectorBetweenPoints(touchPoint, valueCoords).length() < kPointDragAreaSize) {
            return &releaseEnvLastParam;
        }
    }

    return nullptr;
}

bool AmpEnvGraphView::startDragging(int touchPointId, const QPointF &pos) {
    const auto &controller = Controller::instance;
    const auto *touchedAmpEnvPoint = findTouchedAmpEnvPoint(pos);

    // Dragging the first point is not allowed in attack amp envelope (but is allowed in operator envelope)
    if (touchedAmpEnvPoint != nullptr &&
        (operatorEnvelope_ || touchedAmpEnvPoint->index != 0 || !touchedAmpEnvPoint->attack) &&
        (!operatorEnvelope_ || controller->selectedOperatorId().has_value())) {
        const bool isLast = touchedAmpEnvPoint->attack
                            ? touchedAmpEnvPoint->index == currentAttackEnvelope().size() - 1
                            : touchedAmpEnvPoint->index == currentReleaseEnvelope().size() - 1;
        draggingTouchPoint_ = DraggingTouchPoint(touchPointId, touchedAmpEnvPoint->index, touchedAmpEnvPoint->attack,
                                                 isLast);
        draggingAnimParamIndex_ = touchedAmpEnvPoint->index;
        paramOpacityAnim_.setForward();
        paramScaleAnim_.setForward();
        return true;
    }

    return false;
}

void AmpEnvGraphView::updateDragging(QPointF draggingPos) {
    const auto &attackEnvelope = currentAttackEnvelope();
    auto &ampEnvValue = getDraggingAmpEnvValue();
    const auto updatedPos = mapViewPointToAmpEnvPoint(draggingPos, ampEnvValue.attack);
    const auto clampedUpdatedPos = clampBetweenAdjacentPoints(ampEnvValue, updatedPos);

    if (ampEnvValue.attack) {
        // Last attack point can only be moved along the y-axis
        if (draggingTouchPoint_->isLastPoint) {
            setAttackEnvelopeValue(ampEnvValue.index, clampedUpdatedPos.y(), 1.0);
            setAttackEnvelopeValue(ampEnvValue.index-1, clampedUpdatedPos.y(), attackEnvelope[ampEnvValue.index-1].time);
        }
        // Move y value of last point with the second last point
        else if (ampEnvValue.index == attackEnvelope.size() - 2) {
            setAttackEnvelopeValue(ampEnvValue.index, clampedUpdatedPos.y(),
                                                  clampedUpdatedPos.x());
            setAttackEnvelopeValue(attackEnvelope.size()-1, clampedUpdatedPos.y(), 1.0);
        }
        else if (ampEnvValue.index == 0) {
            setAttackEnvelopeValue(ampEnvValue.index, clampedUpdatedPos.y(), 0.0);
        }
        else {
            setAttackEnvelopeValue(ampEnvValue.index, clampedUpdatedPos.y(),
                                                  clampedUpdatedPos.x());
        }
    } else {
        // Only the last point can be moved, and it can only be moved on the X axis
        if (draggingTouchPoint_->isLastPoint && !operatorEnvelope_) {
            setReleaseEnvelopeValue(ampEnvValue.index, ampEnvValue.value, clampedUpdatedPos.x());
        }
        else if (draggingTouchPoint_->isLastPoint ) {
            setReleaseEnvelopeValue(ampEnvValue.index, clampedUpdatedPos.y(), clampedUpdatedPos.x());
        }
    }
}

void AmpEnvGraphView::touchEvent(QTouchEvent *event) {
    QQuickItem::touchEvent(event);

    const auto &points = event->points();
    switch (event->type()) {
        case QEvent::TouchBegin:
            if (!points.empty() && startDragging(points.first().id(), points.first().position())) {
                event->accept();
            }
            break;
        case QEvent::TouchUpdate:
            if (!points.empty()) {
                const auto &draggingPoint = points.first();

                if (draggingPoint.id() == draggingTouchPoint_->touchPointId) {
                    updateDragging(draggingPoint.position());
                }
            }
            break;
        case QEvent::TouchEnd:
        case QEvent::TouchCancel:
            draggingTouchPoint_ = std::nullopt;
            paramOpacityAnim_.setReverse();
            paramScaleAnim_.setReverse();
            break;
        default:
            break;
    }
}

void AmpEnvGraphView::mousePressEvent(QMouseEvent *event) {
    QQuickItem::mousePressEvent(event);

    if (startDragging(-1, event->position())) {
        event->accept();
    }
}

void AmpEnvGraphView::mouseMoveEvent(QMouseEvent *event) {
    QQuickItem::mouseMoveEvent(event);

    updateDragging(event->position());
}

void AmpEnvGraphView::mouseReleaseEvent(QMouseEvent *event) {
    QQuickItem::mouseReleaseEvent(event);

    draggingTouchPoint_ = std::nullopt;
    paramOpacityAnim_.setReverse();
    paramScaleAnim_.setReverse();
}

double AmpEnvGraphView::draggableAreaWidth() {
    return width() - kPointMargin * 2.0;
}

double AmpEnvGraphView::draggableAreaHeight() {
    return height() - kPointMargin * 2.0;
}

QColor AmpEnvGraphView::graphColor() {
    const auto &controller = Controller::instance;

    if (!operatorEnvelope_) {
        return kAmpEnvelopeGraphColor;
    }
    else {
        const auto operatorId = controller->selectedOperatorId();
        return operatorId.has_value()
            ? controller->getOperatorById(operatorId.value()).getColorForOperator()
            : kNoOperatorSelectedGraphColor;
    }
}

const std::vector<AmpEnvValue> &AmpEnvGraphView::currentAttackEnvelope() {
    const auto& controller = Controller::instance;

    if (!operatorEnvelope_) {
        return controller->attackAmpEnvValues();
    }
    else {
        return controller->selectedOperatorId().has_value()
            ? controller->getOperatorById(controller->selectedOperatorId().value()).attackEnvValues
            : kPlaceholderAttackEnvelope;
    }
}

const std::vector<AmpEnvValue> &AmpEnvGraphView::currentReleaseEnvelope() {
    const auto& controller = Controller::instance;

    if (!operatorEnvelope_) {
        return controller->releaseAmpEnvValues();
    }
    else {
        return controller->selectedOperatorId().has_value()
               ? controller->getOperatorById(controller->selectedOperatorId().value()).releaseEnvValues
               : kPlaceholderReleaseEnvelope;
    }
}

void AmpEnvGraphView::setAttackEnvelopeValue(int index, float value, float time) {
    const auto& controller = Controller::instance;

    if (!operatorEnvelope_) {
        controller->setAttackAmpEnvelopePoint(index, value, time);
    }
    else {
        if (!controller->selectedOperatorId().has_value()) return;
        controller->setAttackOpEnvelopePoint(controller->selectedOperatorId().value(), index, value, time);
    }
}

void AmpEnvGraphView::setReleaseEnvelopeValue(int index, float value, float time) {
    const auto& controller = Controller::instance;

    if (!operatorEnvelope_) {
        controller->setReleaseAmpEnvelopePoint(index, value, time);
    }
    else {
        if (!controller->selectedOperatorId().has_value()) return;
        controller->setReleaseOpEnvelopePoint(controller->selectedOperatorId().value(), index, value, time);
    }
}

QColor AmpEnvGraphView::paramBorderColor() {
    if (operatorEnvelope_ && !Controller::instance->selectedOperatorId().has_value()) {
        return {0, 0, 0, 0};
    }

    return kParamBorderColor;
}
