//
// Created by Guðmundur on 4/25/2023.
//

#include "OperatorWaveView.h"

#include "src/Controller/Controller.h"

const double kOffsetUpdateTime = 1000.0 / 60.0; // 60 fps

OperatorWaveView::OperatorWaveView(QQuickItem *parent) : QQuickPaintedItem(parent),
                                                     moveOffsetUpdateTime_(std::chrono::high_resolution_clock::now()),
                                                     currentXOffset_(0.0) {
}

void OperatorWaveView::paint(QPainter *painter) {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto timeSinceOffsetUpdate = std::chrono::duration<double, std::milli>(
            currentTime - moveOffsetUpdateTime_).count();
    if (timeSinceOffsetUpdate > kOffsetUpdateTime) {
        const auto offsetBy = (timeSinceOffsetUpdate / kOffsetUpdateTime) * 1.75;

        currentXOffset_ -= offsetBy;
        moveOffsetUpdateTime_ = std::chrono::high_resolution_clock::now();
    }

    std::vector<QPointF> points;
    for (int x = 0; x < width(); x+=2) {
        points.emplace_back(static_cast<double>(x), getValueAtOffset(currentXOffset_ + static_cast<double>(x)));
    }

    painter->setPen(QPen(color_, 3));
    painter->drawPolyline(points.data(), points.size());

    update();
}

double OperatorWaveView::getValueAtOffset(double offset) {
    const auto &controller = Controller::instance;
    const auto selectedOperatorId = controller->selectedOperatorId();
    if (!selectedOperatorId.has_value()) {
        return height() / 2.0;
    }

    return controller->getSingleOperatorValue(selectedOperatorId.value(), offset)  * (height() / 7.0) + (height() / 2.0);
}

void OperatorWaveView::setColor(QColor color) {
    color_ = color;
}