//
// Created by Guðmundur on 4/7/2023.
//

#ifndef MAIN_QML_TWEENANIMATION_H
#define MAIN_QML_TWEENANIMATION_H

#include <optional>
#include <chrono>

enum class TweenAnimationState {
    Stop,
    Forward,
    Reverse
};

class TweenAnimation {
public:
    explicit TweenAnimation(double ms, double from = 0.0, double to = 1.0);
    void setForward();
    void setReverse();
    void stop();
    void update();
    double value();
    bool isRunning();
    bool isAtStart();
    bool isAtEnd();

private:
    TweenAnimationState animationState_ = TweenAnimationState::Stop;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> startTime_;
    double animTimeMs_;
    double fromValue_;
    double toValue_;
    double value_;
};


#endif //MAIN_QML_TWEENANIMATION_H
