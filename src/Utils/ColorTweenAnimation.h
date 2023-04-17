//
// Created by Guðmundur on 4/7/2023.
//

#ifndef MAIN_QML_COLORTWEENANIMATION_H
#define MAIN_QML_COLORTWEENANIMATION_H


#include <QColor>
#include "TweenAnimation.h"

class ColorTweenAnimation {
public:
    ColorTweenAnimation(double ms, QColor *color, QColor from, QColor to,
                        std::function<double(double x)> animationCurve = AnimationCurves::linear);
    ColorTweenAnimation(const ColorTweenAnimation &colorTweenAnimation);

    void setForward();
    void setReverse();
    void stop();
    void update();
    bool isRunning();
    bool isAtStart();
    bool isAtEnd();

private:
    TweenAnimation tweenAnimation_;
    double fraction_;
    QColor *color_;
    QColor fromColor_;
    QColor toColor_;
};


#endif //MAIN_QML_COLORTWEENANIMATION_H
