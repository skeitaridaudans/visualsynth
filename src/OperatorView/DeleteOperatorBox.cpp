//
// Created by Guðmundur on 2/6/2023.
//

#include "DeleteOperatorBox.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>
#include "src/FontAwesome.h"
#include "src/Controller/Controller.h"
#include "src/Alert/AlertController.h"

const double kBorderWidth = 0.02;
const double kBoxSize = 70.0;
const double kDeleteAreaSize = 100.0;
const double kLeftAnchor = 20.0;
const double kPositionY = 60.0;
const double kCornerRadius = 5.0;
const double kDeleteAllAnimTime = 400.0;
const double kHoldToDeleteTime = 800.0;
const QColor kDefaultBackgroundColor(0xB7, 0x1C, 0x1C, 0);
const QColor kHoldToDeleteBackgroundColor(0xB7, 0x1C, 0x1C, 100);

DeleteOperatorBox::DeleteOperatorBox(OperatorView *boxView)
        : boxView_(boxView),
          scale_(1.0),
          scaleAnim_(kHoldToDeleteTime, &scale_, AnimationCurves::easeOut, 1.0, 1.4),
          backgroundColor_(kDefaultBackgroundColor),
          backgroundColorAnim_(kHoldToDeleteTime, &backgroundColor_, kDefaultBackgroundColor,
                               kHoldToDeleteBackgroundColor) {

}

void DeleteOperatorBox::update() {
    boxPos_.setX(kLeftAnchor);
    boxPos_.setY(kPositionY);

    scaleAnim_.update();
    backgroundColorAnim_.update();

    if (boxView_->primaryTouchPoint().isPressed &&
        isInsideBox(boxView_->primaryTouchPoint().position)) {

        // The initial press
        if (boxView_->touchPressHandledState_ == TouchEventHandledState::Unhandled) {
            boxView_->touchPressHandledState_ = TouchEventHandledState::Handled;
            startHoldingTime_ = std::chrono::high_resolution_clock::now();
            scaleAnim_.setForward();
            backgroundColorAnim_.setForward();
        }
            // Check if the button has been held down long enough
        else if (startHoldingTime_.has_value()) {
            const auto currentTime = std::chrono::high_resolution_clock::now();
            const auto timeElapsed = std::chrono::duration<double, std::milli>(
                    currentTime - startHoldingTime_.value()).count();

            if (timeElapsed >= kHoldToDeleteTime) {
                deleteALlOperators();
                startHoldingTime_ = std::nullopt;
                scaleAnim_.setReverse();
                backgroundColorAnim_.setReverse();
            }
        }
    }

        // Reset the timer holding if it was started previously
    else if (startHoldingTime_.has_value()) {
        startHoldingTime_ = std::nullopt;
        scaleAnim_.setReverse();
        backgroundColorAnim_.setReverse();
    }
}

void DeleteOperatorBox::draw(QPainter *painter) {
    QBrush brush(backgroundColor_);

    const auto rect = QRectF(boxPos_ - (QPointF(kBoxSize, kBoxSize) * ((scale_ - 1) / 2)), QSizeF(kBoxSize, kBoxSize) * scale_);

    painter->setBrush(brush);
    painter->setPen(Qt::PenStyle::SolidLine);
    painter->setPen(QColor(255, 255, 255));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(rect, kCornerRadius, kCornerRadius);

    painter->setFont(fontAwesome()->font(fa::fa_solid, 45));
    painter->drawText(rect, Qt::AlignCenter, QString(fa::fa_trash_can));
}

bool DeleteOperatorBox::isInsideDeleteArea(const QPointF &coords) {
    const auto halfDeleteAreaSize = kDeleteAreaSize / 2.0;
    return coords.x() >= boxPos_.x() - halfDeleteAreaSize && coords.x() < boxPos_.x() + kDeleteAreaSize
           && coords.y() >= boxPos_.y() - halfDeleteAreaSize && coords.y() < boxPos_.y() + kDeleteAreaSize;
}

bool DeleteOperatorBox::isInsideBox(const QPointF &coords) {
    return coords.x() >= boxPos_.x() && coords.x() < boxPos_.x() + kBoxSize
           && coords.y() >= boxPos_.y() && coords.y() < boxPos_.y() + kBoxSize;
}

void DeleteOperatorBox::deleteALlOperators() {
    auto &controller = Controller::instance;

    if (controller->operators().empty()) {
        AlertController::instance->showAlert("No operators to delete", true);
        return;
    }

    controller->deselectOperator();
    for (auto &operator_: controller->operators()) {
        if (operator_.second.operatorViewState.freezeInteraction) continue;

        auto &operatorViewState = operator_.second.operatorViewState;
        operatorViewState.freezeInteraction = true;
        operatorViewState.moveOperatorAnimation = PointTweenAnimation(kDeleteAllAnimTime,
                                                                      &operator_.second.position,
                                                                      operator_.second.position,
                                                                      boxView_->fromViewCoords(
                                                                              boxPos_),
                                                                      AnimationCurves::easeOut);
        operatorViewState.moveOperatorAnimation->setForward();

        operatorViewState.opacityAnim = TweenAnimation(kDeleteAllAnimTime, &operatorViewState.opacity,
                                                       AnimationCurves::easeOut, 1.0, 0.0);
        operatorViewState.opacityAnim->setForward();

        operatorViewState.sizeMultiplierAnim = TweenAnimation(kDeleteAllAnimTime, &operatorViewState.sizeMultiplier,
                                                              AnimationCurves::easeOut, 1.0, 0.4);
        operatorViewState.sizeMultiplierAnim->setForward();

        operatorViewState.moveOperatorAnimation->setOnFinished([&operator_]() mutable {
            operator_.second.scheduleForRemoval = true;
        });
    }
}

