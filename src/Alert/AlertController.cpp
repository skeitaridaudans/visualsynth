//
// Created by Guðmundur on 3/12/2023.
//

#include "AlertController.h"

const double kAlertTime = 1000;

std::unique_ptr<AlertController> AlertController::instance = std::make_unique<AlertController>();

AlertController::AlertController(QObject *parent) : QObject(parent) {

}

void AlertController::showAlert(const QString &text) {
    setAlertText(text);
    setAlertVisibleState(kVisibleState);
    alertShownTime_ = std::chrono::high_resolution_clock::now();
}

void AlertController::update() {
    if (alertShownTime_.has_value()) {
        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto timeElapsed = std::chrono::duration<double, std::milli>(currentTime-alertShownTime_.value()).count();

        if (timeElapsed >= kAlertTime) {
            setAlertVisibleState(kInvisibleState);
            alertShownTime_ = std::nullopt;
        }
    }
}

QString AlertController::alertVisibleState() {
    return alertVisibleState_;
}

QString AlertController::alertText() {
    return alertText_;
}

void AlertController::setAlertVisibleState(const QString &state) {
    alertVisibleState_ = state;
    alertVisibleStateChanged(state);
}

void AlertController::setAlertText(const QString &text) {
    alertText_ = text;
    alertTextChanged(text);
}
