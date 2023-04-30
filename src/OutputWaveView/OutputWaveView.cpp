//
// Created by Guðmundur on 4/22/2023.
//

#include "OutputWaveView.h"
#include "src/Controller/Controller.h"

const QColor kWaveColor(0xFB00C4);
const double kOffsetUpdateTime = 1000.0 / 60.0; // 60 fps

OutputWaveView::OutputWaveView(QQuickItem *parent) : QQuickPaintedItem(parent),
                                                     moveOffsetUpdateTime_(std::chrono::high_resolution_clock::now()) {
}

void OutputWaveView::paint(QPainter *painter) {
    // Add the initial points when the view is first rendered
    if (waveValues_.empty()) {
        for (int x = 0; x < width() + 1; x++) {
            waveValues_.emplace_back(QPointF(static_cast<double>(x), getValueAtOffset(x)));
        }
        currentXOffset_ = (int) width();
    }

    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto timeSinceOffsetUpdate = std::chrono::duration<double, std::milli>(
            currentTime - moveOffsetUpdateTime_).count();
    if (timeSinceOffsetUpdate > kOffsetUpdateTime) {
        const auto offsetBy = (timeSinceOffsetUpdate / kOffsetUpdateTime) * 1.75;

        currentXOffset_ += offsetBy;
        waveValues_.pop_back();
        waveValues_.push_front(QPointF(0, getValueAtOffset(currentXOffset_)));

        for (auto& waveValue : waveValues_) {
            waveValue.setX(waveValue.x() + offsetBy);
        }

        moveOffsetUpdateTime_ = std::chrono::high_resolution_clock::now();
    }

    painter->setPen(QPen(kWaveColor, 3));

    painter->drawPolyline(QPolygonF(waveValues_));

    update();
}

double OutputWaveView::getValueAtOffset(double offset) {
    const auto &controller = Controller::instance;
    return controller->getCarrierOutput(currentXOffset_)  * (height() / 2.5) + (height() / 2.0);
}
