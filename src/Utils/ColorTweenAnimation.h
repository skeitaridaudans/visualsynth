//
// Created by Guðmundur on 4/7/2023.
//

#ifndef MAIN_QML_COLORTWEENANIMATION_H
#define MAIN_QML_COLORTWEENANIMATION_H


#include <QColor>
#include "TweenAnimation.h"

class ColorTweenAnimation {
public:
    ColorTweenAnimation(double ms, QColor from, QColor to);
    void start();
    void startReverse();
    void stop();
    void update();
    const QColor& value();
    bool isRunning();
    bool isAtStart();
    bool isAtEnd();

private:
    TweenAnimation tweenAnimation_;
    QColor color_;
    QColor fromColor_;
    QColor toColor_;
};


#endif //MAIN_QML_COLORTWEENANIMATION_H
