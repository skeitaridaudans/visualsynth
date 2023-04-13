//
// Created by Guðmundur on 4/7/2023.
//

#include "DialogController.h"
#include <QDebug>

std::unique_ptr<DialogController> DialogController::instance = std::make_unique<DialogController>();

DialogController::DialogController(QObject *parent) : QObject(parent) {

}

void DialogController::showDialog(const QString &title, const QString &text, const QString &defaultValue,
                                  const QString &confirmButtonText, const QString &cancelButtonText,
                                  std::function<void(const QString& value)> onSubmit) {
    isVisible_ = true;
    isVisibleChanged(isVisible_);
    title_ = title;
    titleChanged(title_);
    text_ = text;
    textChanged(text_);
    value_ = defaultValue;
    valueChanged(value_);
    confirmButtonText_ = confirmButtonText;
    confirmButtonTextChanged(confirmButtonText_);
    cancelButtonText_ = cancelButtonText;
    cancelButtonTextChanged(cancelButtonText_);

    onSubmit_ = onSubmit;
}

void DialogController::submit() {
    isVisible_ = false;
    isVisibleChanged(isVisible_);

    if (onSubmit_ != nullptr) {
        onSubmit_(value_);
    }
}

void DialogController::cancel() {
    isVisible_ = false;
    isVisibleChanged(isVisible_);
}

