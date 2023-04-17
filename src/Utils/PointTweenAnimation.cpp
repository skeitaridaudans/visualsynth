//
// Created by Guðmundur on 4/11/2023.
//

#include "PointTweenAnimation.h"

#include <utility>
#include "Utils.h"

PointTweenAnimation::PointTweenAnimation(double ms, QPointF from, QPointF to,
                                         std::function<double(double x)> animationCurve) : tweenAnimation_(ms,
                                                                                                           std::move(
                                                                                                                   animationCurve)),
                                                                                           fromPoint_(from),
                                                                                           toPoint_(to),
                                                                                           point_(from) {

}

void PointTweenAnimation::update() {
    tweenAnimation_.update();
    point_ = pointLerp(fromPoint_, toPoint_, tweenAnimation_.value());
}

const QPointF &PointTweenAnimation::value() const {
    return point_;
}

void PointTweenAnimation::setForward() {
    tweenAnimation_.setForward();
}

void PointTweenAnimation::setReverse() {
    tweenAnimation_.setReverse();
}

void PointTweenAnimation::stop() {
    tweenAnimation_.stop();
}

bool PointTweenAnimation::isRunning() {
    return tweenAnimation_.isRunning();
}

bool PointTweenAnimation::isAtStart() {
    return tweenAnimation_.isAtStart();
}

bool PointTweenAnimation::isAtEnd() {
    return tweenAnimation_.isAtEnd();
}
