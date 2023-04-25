//
// Created by Guðmundur on 4/22/2023.
//

#ifndef MAIN_QML_OUTPUTWAVEVIEW_H
#define MAIN_QML_OUTPUTWAVEVIEW_H

#include <QQuickWindow>
#include <QQuickPaintedItem>
#include <QPainter>
#include <deque>

class OutputWaveView : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit OutputWaveView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

private:
    QList<QPointF> waveValues_;
    double currentXOffset_;
    std::chrono::time_point<std::chrono::high_resolution_clock> moveOffsetUpdateTime_;

    double getValueAtOffset(double offset);
};


#endif //MAIN_QML_OUTPUTWAVEVIEW_H
