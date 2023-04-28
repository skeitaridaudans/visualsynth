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

const QColor kPointColor (128, 0, 128);
const double kPointHeight = 20;
const double kPointWidth = 20;
const double kPointDragAreaSize = 40;
const QColor kLineColor (128, 0, 128);

AmpEnvGraphView::AmpEnvGraphView(QQuickItem *parent) :
        QQuickPaintedItem(parent)
{
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void AmpEnvGraphView::paintGraphContainer(QPainter *painter){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(borderColor, 3));
    QRect rect = QRect(0, 0, width(), height());
    painter->drawRect(rect);

}

void AmpEnvGraphView::paintParams(QPainter* painter){
    const auto &controller = Controller::instance;

    for (const auto& ampEnvValue : controller->attackAmpEnvValues()) {
        paintParam(painter, ampEnvValue);
    }

    for (const auto& ampEnvValue : controller->releaseAmpEnvValues()) {
        paintParam(painter, ampEnvValue);
    }
}

void AmpEnvGraphView::paintParam(QPainter *painter, const AmpEnvValue& param) {
    const auto coords = mapAmpEnvPointToView(QPointF(param.time, param.value));
    const auto rect = QRectF(coords.x()-(kPointWidth/2),coords.y()-(kPointHeight/2), kPointWidth, kPointHeight);

    QBrush brush(kPointColor);
    painter->setBrush(brush);

    painter->setPen(Qt::PenStyle::SolidLine);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rect);
}

void AmpEnvGraphView::paintLines(QPainter *painter){
    const auto &controller = Controller::instance;
    const auto &attackAmpEnvValues = controller->attackAmpEnvValues();
    const auto &releaseAmpEnvValues = controller->releaseAmpEnvValues();

    std::vector<QPointF> linePoints;
    for (const auto& ampEnvValue : attackAmpEnvValues) {
        const auto coords = mapAmpEnvPointToView(QPointF(ampEnvValue.time, ampEnvValue.value));
        linePoints.emplace_back(coords);
    }
    for (const auto& ampEnvValue : releaseAmpEnvValues) {
        const auto coords = mapAmpEnvPointToView(QPointF(ampEnvValue.time, ampEnvValue.value));
        linePoints.emplace_back(coords);
    }

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

QPointF AmpEnvGraphView::mapAmpEnvPointToView(const QPointF &point) {
    return QPointF(point.x() * width(), (1 - point.y()) * height());
}

QPointF AmpEnvGraphView::mapViewPointToAmpEnvPoint(const QPointF &point) {
    return QPointF(point.x() / width(), 1 - (point.y() / height()));
}

const AmpEnvValue &AmpEnvGraphView::getDraggingAmpEnvValue() {
    const auto &controller = Controller::instance;

    if (draggingTouchPoint_->isAttack) {
        return controller->attackAmpEnvValues()[draggingTouchPoint_->ampEnvPointIndex];
    }
    else {
        return controller->releaseAmpEnvValues()[draggingTouchPoint_->ampEnvPointIndex];
    }
}

const AmpEnvValue* AmpEnvGraphView::findTouchedAmpEnvPoint(const QPointF& touchPoint) {
    const auto &controller = Controller::instance;
    for (auto& ampEnvValue : controller->attackAmpEnvValues()) {
        const auto valueCoords = mapAmpEnvPointToView(QPointF(ampEnvValue.time, ampEnvValue.value));

        if (vectorBetweenPoints(touchPoint, valueCoords).length() < kPointDragAreaSize) {
            return &ampEnvValue;
        }
    }

    for (auto& ampEnvValue : controller->releaseAmpEnvValues()) {
        const auto valueCoords = mapAmpEnvPointToView(QPointF(ampEnvValue.time, ampEnvValue.value));

        if (vectorBetweenPoints(touchPoint, valueCoords).length() < kPointDragAreaSize) {
            return &ampEnvValue;
        }
    }

    return nullptr;
}

void AmpEnvGraphView::touchEvent(QTouchEvent *event) {
    QQuickItem::touchEvent(event);

    const auto& points = event->points();
    switch (event->type()) {
        case QEvent::TouchBegin:
            if (!points.empty()) {
                const auto *touchedAmpEnvPoint = findTouchedAmpEnvPoint(points.first().position());

                if (touchedAmpEnvPoint != nullptr) {
                    draggingTouchPoint_ = DraggingTouchPoint(points.first().id(), touchedAmpEnvPoint->index, touchedAmpEnvPoint->attack);
                    event->accept();
                }
            }
            break;
        case QEvent::TouchUpdate:
            if (!points.empty()) {
                const auto &draggingPoint = points.first();

                if (draggingPoint.id() == draggingTouchPoint_->touchPointId) {
                    auto& ampEnvValue = getDraggingAmpEnvValue();
                    const auto updatedPos = mapViewPointToAmpEnvPoint(draggingPoint.position());

                    const auto &controller = Controller::instance;
                    if (ampEnvValue.attack) {
                        controller->setAttackAmpEnvelopePoint(ampEnvValue.index, updatedPos.y(), updatedPos.y());
                    }
                    else {
                        controller->setReleaseAmpEnvelopePoint(ampEnvValue.index, updatedPos.y(), updatedPos.y());
                    }
                }
            }
            break;
        case QEvent::TouchEnd:
            draggingTouchPoint_ = std::nullopt;
            break;
        case QEvent::TouchCancel:
            draggingTouchPoint_ = std::nullopt;
            break;
        default:
            break;
    }
}

void AmpEnvGraphView::mousePressEvent(QMouseEvent *event) {
    QQuickItem::mousePressEvent(event);

    const auto *touchedAmpEnvPoint = findTouchedAmpEnvPoint(event->position());

    if (touchedAmpEnvPoint != nullptr) {
        draggingTouchPoint_ = DraggingTouchPoint(-1, touchedAmpEnvPoint->index, touchedAmpEnvPoint->attack);
        event->accept();
    }
}

void AmpEnvGraphView::mouseMoveEvent(QMouseEvent *event) {
    QQuickItem::mouseMoveEvent(event);

    auto& ampEnvValue = getDraggingAmpEnvValue();
    const auto updatedPos = mapViewPointToAmpEnvPoint(event->position());

    const auto &controller = Controller::instance;
    if (ampEnvValue.attack) {
        controller->setAttackAmpEnvelopePoint(ampEnvValue.index, updatedPos.y(), updatedPos.x());
    }
    else {
        controller->setReleaseAmpEnvelopePoint(ampEnvValue.index, updatedPos.y(), updatedPos.x());
    }
}

void AmpEnvGraphView::mouseReleaseEvent(QMouseEvent *event) {
    QQuickItem::mouseReleaseEvent(event);

    draggingTouchPoint_ = std::nullopt;
}
