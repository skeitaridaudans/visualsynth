//
// Created by Guðmundur on 4/11/2023.
//

#include "PointTweenAnimation.h"

#include <utility>
#include "Utils.h"

PointTweenAnimation::PointTweenAnimation(double ms, QPointF *point, QPointF from, QPointF to,
                                         std::function<double(double x)> animationCurve) : fraction_(0.0),
                                                                                            tweenAnimation_(ms, &fraction_,
                                                                                                           std::move(
                                                                                                                   animationCurve)),
                                                                                           fromPoint_(from),
                                                                                           toPoint_(to),
                                                                                           point_(point) {

}

PointTweenAnimation::PointTweenAnimation(const PointTweenAnimation &pointTweenAnimation) : fraction_(pointTweenAnimation.fraction_),
                                                                                     tweenAnimation_(pointTweenAnimation.tweenAnimation_),
                                                                                     fromPoint_(pointTweenAnimation.fromPoint_),
                                                                                     toPoint_(pointTweenAnimation.toPoint_),
                                                                                     point_(pointTweenAnimation.point_) {
    // Memory location of fraction_ changes when PointTweenAnimation is copied, so we need to update it in tweenAnimation_
    tweenAnimation_.setValuePtr(&fraction_);
}

void PointTweenAnimation::update() {
    if (!isRunning()) return;

    tweenAnimation_.update();
    *point_ = pointLerp(fromPoint_, toPoint_, fraction_);
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

PointTweenAnimation& PointTweenAnimation::operator=(const PointTweenAnimation& other) {
    if (this != &other) {
        fromPoint_ = other.fromPoint_;
        toPoint_ = other.toPoint_;
        tweenAnimation_ = other.tweenAnimation_;
        fraction_ = other.fraction_;
        point_ = other.point_;
    }
    return *this;
}

void PointTweenAnimation::setOnFinished(std::function<void()> onFinished) {
    tweenAnimation_.setOnFinished(std::move(onFinished));
}
