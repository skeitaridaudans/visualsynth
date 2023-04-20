//
// Created by Guðmundur on 4/7/2023.
//

#include "ColorTweenAnimation.h"
#include "Utils.h"

ColorTweenAnimation::ColorTweenAnimation(double ms, QColor* color, QColor from, QColor to,
                                         std::function<double(double x)> animationCurve) : tweenAnimation_(ms, &fraction_,
                                                                                                           animationCurve),
                                                                                           fromColor_(from),
                                                                                           toColor_(to),
                                                                                           color_(color) {

}

ColorTweenAnimation::ColorTweenAnimation(const ColorTweenAnimation &colorTweenAnimation) :fraction_(colorTweenAnimation.fraction_),
                                                                                    tweenAnimation_(colorTweenAnimation.tweenAnimation_),
                                                                                    fromColor_(colorTweenAnimation.fromColor_),
                                                                                    toColor_(colorTweenAnimation.fromColor_),
                                                                                    color_(colorTweenAnimation.color_) {
    // Memory location of fraction_ changes when ColorTweenAnimation is copied, so we need to update it in tweenAnimation_
    tweenAnimation_.setValuePtr(&fraction_);
}

void ColorTweenAnimation::update() {
    tweenAnimation_.update();
    *color_ = colorLerp(fromColor_, toColor_, fraction_);
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
