//
// Created by Guðmundur on 4/11/2023.
//

#ifndef MAIN_QML_POINTTWEENANIMATION_H
#define MAIN_QML_POINTTWEENANIMATION_H


#include <QPointF>
#include "TweenAnimation.h"

class PointTweenAnimation {
public:
    PointTweenAnimation(double ms, QPointF *point, QPointF from, QPointF to,
                        std::function<double(double x)> animationCurve = AnimationCurves::linear);
    PointTweenAnimation(const PointTweenAnimation& pointTweenAnimation);

    void setForward();
    void setReverse();
    void stop();
    void update();
    bool isRunning();
    bool isAtStart();
    bool isAtEnd();
    PointTweenAnimation& operator=(const PointTweenAnimation& other);

    QPointF fromPoint_;
    QPointF toPoint_;
private:
    TweenAnimation tweenAnimation_;
    double fraction_;
    QPointF *point_;
};


#endif //MAIN_QML_POINTTWEENANIMATION_H
