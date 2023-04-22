//
// Created by Guðmundur on 4/22/2023.
//

#include "OutputWaveView.h"
#include "src/Controller/Controller.h"

const QColor kWaveColor(0xFB00C4);
const double kOffsetUpdateTime = 1000.0 / 60.0; // 60 fps

OutputWaveView::OutputWaveView(QQuickItem *parent) : QQuickPaintedItem(parent),
                                                     moveOffsetUpdateTime_(std::chrono::high_resolution_clock::now()) {
    const auto &controller = Controller::instance;

}

void OutputWaveView::paint(QPainter *painter) {
    // Add the initial points when the view is first rendered
    if (waveValues_.empty()) {
        for (int x = 0; x < width() + 1; x++) {
            waveValues_.emplace_back(getValueAtOffset(x));
        }
        currentXOffset_ = (int) width();
    }

    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto timeSinceOffsetUpdate = std::chrono::duration<double, std::milli>(
            currentTime - moveOffsetUpdateTime_).count();
    if (timeSinceOffsetUpdate > kOffsetUpdateTime) {
        currentXOffset_++;
        moveOffsetUpdateTime_ = std::chrono::high_resolution_clock::now();

        waveValues_.pop_front();
        waveValues_.push_back(getValueAtOffset(currentXOffset_));
    }

    painter->setPen(QPen(kWaveColor, 3));

    std::vector<QPointF> wavePoints;
    for (int x = 0; x < width() + 1; x++) {
        wavePoints.emplace_back((double) x, waveValues_[x]);
    }

    painter->drawPolyline(wavePoints.data(), wavePoints.size());

    update();
}

double OutputWaveView::getValueAtOffset(int offset) {
    const auto &controller = Controller::instance;
    return controller->getCarrierOutput(currentXOffset_)  * (height() / 2.5) + (height() / 2.0);
}
