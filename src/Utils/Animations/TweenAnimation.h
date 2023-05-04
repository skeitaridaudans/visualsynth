//
// Created by Guðmundur on 4/7/2023.
//

#ifndef MAIN_QML_TWEENANIMATION_H
#define MAIN_QML_TWEENANIMATION_H

#include <optional>
#include <chrono>
#include <valarray>
#include <functional>

enum class TweenAnimationState {
    Stop,
    Forward,
    Reverse
};

namespace AnimationCurves {
    // Animation curve formulas are from https://easings.net

    inline double linear(double x) {
        return x;
    }

    inline double easeIn(double x) {
        return std::pow(x, 2.0);
    }

    inline double easeOut(double x) {
        return 1.0 - std::pow(1.0 - x, 2.0);
    }

    inline double easeInOut(double x) {
        return x < 0.5 ? 2 * std::pow(x, 2.0) : 1 - std::pow(-2 * x + 2, 2) / 2;
    }

    inline double easeOutBack(double x) {
        const auto c1 = 1.70158;
        const auto c3 = c1 + 1.0;

        return 1.0 + c3 * std::pow(x - 1.0, 3.0) + c1 * std::pow(x - 1.0, 2.0);
    }
}

class TweenAnimation {
public:
    TweenAnimation(double ms, double* value, std::function<double(double x)> animationCurve = AnimationCurves::linear,
                   double from = 0.0, double to = 1.0, bool loop = false);

    void setForward();
    void setReverse();
    void stop();
    void update();
    bool isRunning();
    bool isAtStart();
    bool isAtEnd();
    void setValuePtr(double *value);
    void setOnFinished(std::function<void()> onFinished);

private:
    std::function<double(double x)> animationCurve_;
    TweenAnimationState animationState_ = TweenAnimationState::Stop;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> startTime_;
    double animTimeMs_;
    double fromValue_;
    double toValue_;
    double* value_;
    bool loop_;
    std::function<void()> onFinished_;
};


#endif //MAIN_QML_TWEENANIMATION_H
