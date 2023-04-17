//
// Created by Guðmundur on 4/7/2023.
//

#include "TweenAnimation.h"

#include <utility>

TweenAnimation::TweenAnimation(double ms, double *value, std::function<double(double x)> animationCurve, double from,
                               double to, bool loop) : animTimeMs_(ms), animationCurve_(std::move(animationCurve)), value_(value),
                                            fromValue_(from), toValue_(to), loop_(loop) {

}

void TweenAnimation::setForward() {
    if (animationState_ == TweenAnimationState::Forward || isAtEnd()) {
        return;
    }

    if (animationState_ == TweenAnimationState::Reverse) {
        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto timeElapsed = std::chrono::duration<double, std::milli>(currentTime - startTime_.value()).count();
        const auto timeRemaining = (int) (animTimeMs_ - timeElapsed);
        startTime_ = std::chrono::high_resolution_clock::now() - std::chrono::duration<int, std::milli>(timeRemaining);
    } else {
        startTime_ = std::chrono::high_resolution_clock::now();
    }

    animationState_ = TweenAnimationState::Forward;
}

void TweenAnimation::setReverse() {
    if (animationState_ == TweenAnimationState::Reverse || isAtStart()) {
        return;
    }

    if (animationState_ == TweenAnimationState::Forward) {
        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto timeElapsed = std::chrono::duration<double, std::milli>(currentTime - startTime_.value()).count();
        const auto timeRemaining = (int) (animTimeMs_ - timeElapsed);
        startTime_ = std::chrono::high_resolution_clock::now() - std::chrono::duration<int, std::milli>(timeRemaining);
    } else {
        startTime_ = std::chrono::high_resolution_clock::now();
    }

    animationState_ = TweenAnimationState::Reverse;
}

void TweenAnimation::stop() {
    animationState_ = TweenAnimationState::Stop;
}

void TweenAnimation::update() {
    if (animationState_ == TweenAnimationState::Stop || startTime_ == std::nullopt) {
        return;
    }

    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto timeElapsed = std::chrono::duration<double, std::milli>(currentTime - startTime_.value()).count();

    if (timeElapsed >= animTimeMs_) {
        const auto previousAnimationState = animationState_;

        *value_ = animationState_ == TweenAnimationState::Forward ? toValue_ : fromValue_;
        animationState_ = TweenAnimationState::Stop;

        if (loop_) {
            if (previousAnimationState == TweenAnimationState::Forward) {
                setReverse();
            }
            else {
                setForward();
            }
        }
        return;
    }

    *value_ = animationCurve_(timeElapsed / animTimeMs_) * (toValue_ - fromValue_) + fromValue_;

    if (animationState_ == TweenAnimationState::Reverse) {
        *value_ = toValue_ - (*value_ - fromValue_);
    }
}

bool TweenAnimation::isRunning() {
    return animationState_ != TweenAnimationState::Stop;
}

bool TweenAnimation::isAtStart() {
    return *value_ == fromValue_;
}

bool TweenAnimation::isAtEnd() {
    return *value_ == toValue_;
}

void TweenAnimation::setValuePtr(double *value) {
    value_ = value;
}
