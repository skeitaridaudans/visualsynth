//
// Created by Guðmundur on 4/7/2023.
//

#include "ColorTweenAnimation.h"
#include "Utils.h"

ColorTweenAnimation::ColorTweenAnimation(double ms, QColor from, QColor to) : tweenAnimation_(ms), fromColor_(from),
                                                                              toColor_(to) {

}

void ColorTweenAnimation::update() {
    tweenAnimation_.update();
    color_ = colorLerp(fromColor_, toColor_, tweenAnimation_.value());
}

const QColor &ColorTweenAnimation::value() {
    return color_;
}

void ColorTweenAnimation::start() {
    tweenAnimation_.setForward();
}

void ColorTweenAnimation::startReverse() {
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
