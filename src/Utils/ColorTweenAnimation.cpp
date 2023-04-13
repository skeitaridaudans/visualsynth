//
// Created by Guðmundur on 4/7/2023.
//

#include "ColorTweenAnimation.h"
#include "Utils.h"

ColorTweenAnimation::ColorTweenAnimation(double ms, QColor from, QColor to,
                                         std::function<double(double x)> animationCurve) : tweenAnimation_(ms,
                                                                                                           animationCurve),
                                                                                           fromColor_(from),
                                                                                           toColor_(to),
                                                                                           color_(from) {

}

void ColorTweenAnimation::update() {
    tweenAnimation_.update();
    color_ = colorLerp(fromColor_, toColor_, tweenAnimation_.value());
}

const QColor &ColorTweenAnimation::value() {
    return color_;
}

void ColorTweenAnimation::setForward() {
    tweenAnimation_.setForward();
}

void ColorTweenAnimation::setReverse() {
    tweenAnimation_.setReverse();
}

void ColorTweenAnimation::stop() {
    tweenAnimation_.stop();
}

bool ColorTweenAnimation::isRunning() {
    return tweenAnimation_.isRunning();
}

bool ColorTweenAnimation::isAtStart() {
    return tweenAnimation_.isAtStart();
}

bool ColorTweenAnimation::isAtEnd() {
    return tweenAnimation_.isAtEnd();
}
