//
// Created by Guðmundur on 4/11/2023.
//

#ifndef MAIN_QML_POINTTWEENANIMATION_H
#define MAIN_QML_POINTTWEENANIMATION_H


#include <QPointF>
#include "TweenAnimation.h"

class PointTweenAnimation {
public:
    PointTweenAnimation(double ms, QPointF from, QPointF to,
                        std::function<double(double x)> animationCurve = AnimationCurves::linear);
    void setForward();
    void setReverse();
    void stop();
    void update();
    const QPointF &value();
    bool isRunning();
    bool isAtStart();
    bool isAtEnd();

    QPointF fromPoint_;
    QPointF toPoint_;
private:
    TweenAnimation tweenAnimation_;
    QPointF point_;
};


#endif //MAIN_QML_POINTTWEENANIMATION_H
