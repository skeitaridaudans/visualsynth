//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_DELETEOPERATORBOX_H
#define QTQUICKTEST_DELETEOPERATORBOX_H


#include "OperatorView.h"
#include "src/Utils/ColorTweenAnimation.h"

class OperatorView;

class DeleteOperatorBox {
public:
    DeleteOperatorBox(OperatorView *boxView);
    void update();
    void draw(QPainter* qPainter);

    bool isInsideDeleteArea(const QPointF& coords);
private:
    QPointF boxPos_;
    OperatorView *boxView_;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> startHoldingTime_ = std::nullopt;
    TweenAnimation scaleAnim_;
    double scale_;
    ColorTweenAnimation backgroundColorAnim_;
    QColor backgroundColor_;

    bool isInsideBox(const QPointF &coords);
    void deleteALlOperators();
};


#endif //QTQUICKTEST_DELETEOPERATORBOX_H
