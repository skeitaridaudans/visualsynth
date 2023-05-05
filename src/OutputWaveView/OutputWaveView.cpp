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

    // This needs to be done every frame in case the height of the view changed (while resizing the window)
    std::vector<QPointF> renderWavePoints;
    for (const auto& waveValue : waveValues_) {
        renderWavePoints.emplace_back(waveValue.x(), waveValue.y() * (height() / 6.0) + (height() / 2.0));
    }

    painter->drawPolyline(renderWavePoints.data(), renderWavePoints.size());

    update();
}

double OutputWaveView::getValueAtOffset(double offset) {
    const auto &controller = Controller::instance;
    return controller->getCarrierOutput(currentXOffset_);
}
