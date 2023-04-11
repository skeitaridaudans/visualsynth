//
// Created by Guðmundur on 4/7/2023.
//

#ifndef MAIN_QML_TWEENANIMATION_H
#define MAIN_QML_TWEENANIMATION_H

#include <optional>
#include <chrono>
#include <valarray>

enum class TweenAnimationState {
    Stop,
    Forward,
    Reverse
};

namespace AnimationCurves {
    inline double easeIn(double x) {
        return std::pow(x, 2.0);
    }

    inline double easeOut(double x) {
        return 1.0 - std::pow(1.0 - x, 2.0);
    }

    inline double linear(double x) {
        return x;
    }
}

class TweenAnimation {
public:
    explicit TweenAnimation(double ms, std::function<double (double x)> animationCurve = AnimationCurves::linear, double from = 0.0, double to = 1.0);
    void setForward();
    void setReverse();
    void stop();
    void update();
    double value();
    bool isRunning();
    bool isAtStart();
    bool isAtEnd();

private:
    std::function<double (double x)> animationCurve_;
    TweenAnimationState animationState_ = TweenAnimationState::Stop;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> startTime_;
    double animTimeMs_;
    double fromValue_;
    double toValue_;
    double value_;
};


#endif //MAIN_QML_TWEENANIMATION_H
