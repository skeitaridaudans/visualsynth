//
// Created by Guðmundur on 10-Feb-23.
//

#ifndef QTQUICKTEST_OPERATOR_H
#define QTQUICKTEST_OPERATOR_H

#include "src/Utils/Animations/PointTweenAnimation.h"
#include <vector>
#include <QPoint>
#include <chrono>
#include <QObject>
#include "src/Utils/Utils.h"
#include "AmpEnvValue.h"

enum class DraggingState {
    None,
    Holding,
    Dragging
};

// All state that is specifically just for the operator view
struct OperatorViewState {
    OperatorViewState();

    OperatorViewState(const OperatorViewState &operatorViewState);

    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> initialClickTime = std::nullopt;
    DraggingState draggingState = DraggingState::None;
    std::optional<QPointF> initialDragCursorPos;
    std::optional<PointTweenAnimation> moveOperatorAnimation;
    double sizeMultiplier = 1.0;
    std::optional<TweenAnimation> sizeMultiplierAnim;
    double opacity = 1.0;
    std::optional<TweenAnimation> opacityAnim;
    double connectIconOpacity = 0.3;
    std::optional<TweenAnimation> connectIconOpacityAnim;
    bool freezeInteraction = false;
};

struct Operator : public QObject {
Q_OBJECT
public:
    Operator();
    Operator(int id, QObject *parent = 0);
    Operator(int id, float frequency, long amplitude, bool isModulator, bool isCarrier, std::vector<int> modulatedBy,
             QPointF position, std::vector<AmpEnvValue> attackEnvValues, std::vector<AmpEnvValue> releaseEnvValues,
             QObject *parent = 0);
    Operator(const Operator &operator_);

    Operator &operator=(const Operator &operator_);

    int id;
    float frequency;
    long amplitude;
    bool isModulator;
    bool isCarrier;
    float currentSemiTone;

    // Fraction, 0-1
    double frequencyLfoAmount = 0;
    double amplitudeLfoAmount = 0;

    std::vector<int> modulatedBy;
    QPointF position;
    OperatorViewState operatorViewState;
    std::vector<AmpEnvValue> attackEnvValues;
    std::vector<AmpEnvValue> releaseEnvValues;
    int visitedCount = 0;

    Q_PROPERTY(int idProp MEMBER id)
    Q_PROPERTY(float freqProp MEMBER frequency)
    Q_PROPERTY(long ampProp MEMBER amplitude)
    Q_PROPERTY(double frequencyLfoAmount MEMBER frequencyLfoAmount)
    Q_PROPERTY(double amplitudeLfoAmount MEMBER amplitudeLfoAmount)
    Q_PROPERTY(float semiTone MEMBER currentSemiTone)
    Q_INVOKABLE float getFreq();
    Q_INVOKABLE long getAmp();
    Q_INVOKABLE void setFrequency(float step);
    Q_INVOKABLE void setAmplitude(long step);
    Q_INVOKABLE void updateFrequency(float frequency);
    Q_INVOKABLE void setSemiTone(float value);
    Q_INVOKABLE float getSemiTone();

    // Schedule the operator to be deleted since deleting it is not possible while iterating over the operators
    bool scheduleForRemoval = false;
    Q_INVOKABLE QColor getColorForOperator() const; //Operator *operator_

};

void to_json(json &j, const Operator &o);

void from_json(const json &j, Operator &o);

#endif //QTQUICKTEST_OPERATOR_H
