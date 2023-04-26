//
// Created by Guðmundur on 4/25/2023.
//

#ifndef MAIN_QML_OPERATORWAVEVIEW_H
#define MAIN_QML_OPERATORWAVEVIEW_H

#include <QQuickWindow>
#include <QQuickPaintedItem>
#include <QPainter>
#include <deque>

class OperatorWaveView : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit OperatorWaveView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    Q_INVOKABLE void setColor(QColor color);

private:
    QColor color_ = QColor(0xFB00C4);
    double currentXOffset_;
    std::chrono::time_point<std::chrono::high_resolution_clock> moveOffsetUpdateTime_;

    double getValueAtOffset(double offset);
};


#endif //MAIN_QML_OPERATORWAVEVIEW_H
