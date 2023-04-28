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

const QColor kPointColor(128, 0, 128);
const double kPointHeight = 20;
const double kPointWidth = 20;
const double kPointDragAreaSize = 40;
const QColor kLineColor(128, 0, 128);
const double kViewAttackProportion = 0.8;
const double kViewReleaseProportion = 1.0 - kViewAttackProportion;

AmpEnvGraphView::AmpEnvGraphView(QQuickItem *parent) :
        QQuickPaintedItem(parent) {
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void AmpEnvGraphView::paintGraphContainer(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(borderColor, 3));
    QRect rect = QRect(0, 0, width(), height());
    painter->drawRect(rect);

}

void AmpEnvGraphView::paintParams(QPainter *painter) {
    const auto &controller = Controller::instance;

    // Only paint attack points, as they are the only ones that can be moved
    for (const auto &ampEnvValue: controller->attackAmpEnvValues()) {
        // Skip drawing the first amp envelope point
        if (ampEnvValue.index == 0) continue;

        paintParam(painter, ampEnvValue);
    }
}

void AmpEnvGraphView::paintParam(QPainter *painter, const AmpEnvValue &param) {
    const auto coords = getDrawingPosOfAmpEnvPoint(param);
    const auto rect = QRectF(coords.x() - (kPointWidth / 2), coords.y() - (kPointHeight / 2), kPointWidth,
                             kPointHeight);

    QBrush brush(kPointColor);
    painter->setBrush(brush);

    painter->setPen(Qt::PenStyle::SolidLine);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rect);
}

void AmpEnvGraphView::paintLines(QPainter *painter) {
    const auto &controller = Controller::instance;
    const auto &attackAmpEnvValues = controller->attackAmpEnvValues();
    const auto &releaseAmpEnvValues = controller->releaseAmpEnvValues();

    std::vector<QPointF> linePoints;
    for (const auto &ampEnvValue: attackAmpEnvValues) {
        const auto coords = getDrawingPosOfAmpEnvPoint(ampEnvValue);
        linePoints.emplace_back(coords);
    }

    // Only use the last point in the release envelope to draw the line
    const auto &releaseEnvLastPoint = releaseAmpEnvValues[releaseAmpEnvValues.size() - 1];
    const auto coords = mapAmpEnvPointToView(QPointF(releaseEnvLastPoint.time, releaseEnvLastPoint.value), false);
    linePoints.emplace_back(coords);

    painter->setPen(QPen(kLineColor, 5));
    painter->drawPolyline(linePoints.data(), linePoints.size());
}


void AmpEnvGraphView::paint(QPainter *painter) {

    painter->setRenderHint(QPainter::Antialiasing);

    //Drawing Rectangle
    this->paintLines(painter);
    this->paintGraphContainer(painter);
    this->paintParams(painter);

    update();
}

QPointF AmpEnvGraphView::mapAmpEnvPointToView(const QPointF &point, bool isAttack) {
    return isAttack
           ? QPointF(point.x() * width() * kViewAttackProportion, (1 - point.y()) * height())
           : QPointF(width() * kViewAttackProportion + point.x() * width() * kViewReleaseProportion,
                     (1 - point.y()) * height());
}

QPointF AmpEnvGraphView::mapViewPointToAmpEnvPoint(const QPointF &point, bool isAttack) {
    return isAttack
           ? QPointF(point.x() / (width() * kViewAttackProportion), 1 - (point.y() / height()))
           : QPointF(kViewAttackProportion + (point.x() / (width() * kViewReleaseProportion)),
                     1 - (point.y() / height()));
}

// Prevents a point in the amp envelope from going in front of or behind the previous and next point
// This is only intended to be used for attack points, as those are the only ones that can be moved
QPointF AmpEnvGraphView::clampBetweenAdjacentPoints(const AmpEnvValue &ampEnvValue, const QPointF &desiredPos) {
    const auto &controller = Controller::instance;
    const auto &attackAmpEnvValues = controller->attackAmpEnvValues();

    double minX = 0.0;
    if (ampEnvValue.index > 0) {
        minX = attackAmpEnvValues[ampEnvValue.index - 1].time;
    }

    double maxX = 1.0;
    if (ampEnvValue.index < attackAmpEnvValues.size() - 1) {
        maxX = attackAmpEnvValues[ampEnvValue.index + 1].time;
    }

    return {
            std::clamp(desiredPos.x(), minX, maxX),
            std::clamp(desiredPos.y(), 0.0, 1.0)
    };
}

QPointF AmpEnvGraphView::getDrawingPosOfAmpEnvPoint(const AmpEnvValue &ampEnvValue) {
    const auto &controller = Controller::instance;

    auto coords = mapAmpEnvPointToView(QPointF(ampEnvValue.time, ampEnvValue.value), ampEnvValue.attack);

    // The last point essentially represents both the last in attack envelope and the first in release envelope
    // So the (X) position that we want to display it in is the sum of the positions of both the last in attack
    // and the first in release (relative to the start of the release proportion)
    if (ampEnvValue.index == controller->attackAmpEnvValues().size() - 1) {
        const auto firstReleasePoint = controller->releaseAmpEnvValues().front();
        const auto firstReleasePointPos = mapAmpEnvPointToView(QPointF(firstReleasePoint.time, firstReleasePoint.value),
                                                               firstReleasePoint.attack);
        coords.setX(coords.x() + firstReleasePointPos.x() - width() * kViewAttackProportion);
    }

    return coords;
}

const AmpEnvValue &AmpEnvGraphView::getDraggingAmpEnvValue() {
    const auto &controller = Controller::instance;

    return controller->attackAmpEnvValues()[draggingTouchPoint_->ampEnvPointIndex];
}

const AmpEnvValue *AmpEnvGraphView::findTouchedAmpEnvPoint(const QPointF &touchPoint) {
    const auto &controller = Controller::instance;
    for (auto &ampEnvValue: controller->attackAmpEnvValues()) {
        const auto valueCoords = mapAmpEnvPointToView(QPointF(ampEnvValue.time, ampEnvValue.value), true);

        if (vectorBetweenPoints(touchPoint, valueCoords).length() < kPointDragAreaSize) {
            return &ampEnvValue;
        }
    }

    return nullptr;
}

bool AmpEnvGraphView::startDragging(int touchPointId, const QPointF &pos) {
    const auto &controller = Controller::instance;
    const auto *touchedAmpEnvPoint = findTouchedAmpEnvPoint(pos);

    // Only allow dragging attack points, and dragging the first point is not allowed
    if (touchedAmpEnvPoint != nullptr && touchedAmpEnvPoint->attack && touchedAmpEnvPoint->index != 0) {
        draggingTouchPoint_ = DraggingTouchPoint(touchPointId, touchedAmpEnvPoint->index,
                                                 touchedAmpEnvPoint->index ==
                                                 controller->attackAmpEnvValues().size() - 1);
        return true;
    }

    return false;
}

void AmpEnvGraphView::updateDragging(QPointF draggingPos) {
    const auto &controller = Controller::instance;
    auto &ampEnvValue = getDraggingAmpEnvValue();
    const auto updatedPos = mapViewPointToAmpEnvPoint(draggingPos, ampEnvValue.attack);
    const auto clampedUpdatedPos = clampBetweenAdjacentPoints(ampEnvValue, updatedPos);

    controller->setAttackAmpEnvelopePoint(ampEnvValue.index, clampedUpdatedPos.y(),
                                          clampedUpdatedPos.x());
    if (draggingTouchPoint_->isLastPoint) {
        const auto releasePointX = std::max(
                (updatedPos.x() - clampedUpdatedPos.x()) / kViewReleaseProportion * kViewAttackProportion,
                0.0);

        controller->setReleaseAmpEnvelopePoint(0, clampedUpdatedPos.y(), releasePointX);
    }
}

void AmpEnvGraphView::touchEvent(QTouchEvent *event) {
    QQuickItem::touchEvent(event);
    const auto &controller = Controller::instance;

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
            break;
        default:
            break;
    }
}

void AmpEnvGraphView::mousePressEvent(QMouseEvent *event) {
    QQuickItem::mousePressEvent(event);
    const auto &controller = Controller::instance;

    if (startDragging(-1, event->position())) {
        event->accept();
    }
}

void AmpEnvGraphView::mouseMoveEvent(QMouseEvent *event) {
    QQuickItem::mouseMoveEvent(event);
    const auto &controller = Controller::instance;

    updateDragging(event->position());
}

void AmpEnvGraphView::mouseReleaseEvent(QMouseEvent *event) {
    QQuickItem::mouseReleaseEvent(event);

    draggingTouchPoint_ = std::nullopt;
}
