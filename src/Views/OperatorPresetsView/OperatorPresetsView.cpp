//
// Created by Guðmundur on 4/2/2023.
//

#include <QCursor>
#include <QPainter>
#include <filesystem>
#include <QGuiApplication>
#include "OperatorPresetsView.h"
#include "src/Utils/Utils.h"
#include "src/Controllers/Controller/Controller.h"
#include "src/Utils/FontAwesome.h"
#include "src/Controllers/Dialog/DialogController.h"
#include "src/Controllers/Alert/AlertController.h"

const double kOuterPaddingX = 20.0;
const double kOuterPaddingY = 16.0;
const double kBetweenPaddingX = 16.0;
const double kBetweenPaddingY = 16.0;
const int kRowCount = 3;
const int kColumnCount = 3;
const QColor kAddPresetBackgroundColor = QColor(0x212121);
const QColor kAddPresetBackgroundHoverColor = QColor(0x161616);
const double kAddPresetBackgroundAnimTime = 100.0;
const QString kPresetDirName = "presets";

OperatorPresetsView::OperatorPresetsView(QQuickItem *parent) : QQuickPaintedItem(parent),
                                                               addPresetBackgroundColor_(kAddPresetBackgroundColor),
                                                               addPresetBackgroundAnim_(kAddPresetBackgroundAnimTime,
                                                                                        &addPresetBackgroundColor_,
                                                                                        kAddPresetBackgroundColor,
                                                                                        kAddPresetBackgroundHoverColor) {
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void OperatorPresetsView::paint(QPainter *painter) {
    if (!isEnabled()) {
        update();
        return;
    }

    if (operatorPresetViews_ == std::nullopt) {
        loadPresets();
    }
    updateSizes();

    for (int i = 0; i < (int) std::ceil((double) operatorPresetViews_->size() / (double) kRowCount); i++) {
        for (int j = 0; j < std::min((int) operatorPresetViews_->size() - i * kColumnCount, kColumnCount); j++) {
            const auto x = kOuterPaddingX + (j * (presetBoxSize_.width() + kBetweenPaddingX));
            const auto y = kOuterPaddingY + (i * (presetBoxSize_.height() + kBetweenPaddingX));
            operatorPresetViews_->at((i * 3) + j)->update(QPointF(x, y), presetBoxSize_);
            operatorPresetViews_->at((i * 3) + j)->paint(painter, QPointF(x, y), presetBoxSize_);
        }
    }

    const auto addButtonGridPosI = (int) std::ceil((double) (operatorPresetViews_->size() + 1) / kRowCount) - 1;
    const auto addButtonGridPosJ = std::min((int) operatorPresetViews_->size() - addButtonGridPosI * kColumnCount, kColumnCount);
    const auto x = kOuterPaddingX + (addButtonGridPosJ * (presetBoxSize_.width() + kBetweenPaddingX));
    const auto y = kOuterPaddingY + (addButtonGridPosI * (presetBoxSize_.height() + kBetweenPaddingX));
    paintAddPresetButton(painter, QPointF(x, y));

    update();
}

void OperatorPresetsView::paintAddPresetButton(QPainter *painter, const QPointF &pos) {
    const auto buttonRect = QRectF(pos, QSizeF(presetBoxSize_.width(), (presetBoxSize_.height() / 3.0) * 2.0));

    painter->setPen(Qt::PenStyle::NoPen);
    painter->setBrush(addPresetBackgroundColor_);
    painter->drawRoundedRect(buttonRect, 8, 8);

    painter->setPen(QColor(255, 255, 255));
    painter->setFont(fontAwesome()->font(fa::fa_solid, 35));
    painter->drawText(buttonRect, Qt::AlignCenter, QString(fa::fa_plus));

    if (isPointInsideRect(touchPoint().position, buttonRect) && touchPoint().isPressed) {
        addPresetBackgroundAnim_.setForward();
        addNewPreset();
    } else if (addPresetBackgroundAnim_.isAtEnd()) {
        addPresetBackgroundAnim_.setReverse();
    }

    addPresetBackgroundAnim_.update();
}

void OperatorPresetsView::loadPresets() {
    if (!QDir(kPresetDirName).exists() && !QDir().mkdir(kPresetDirName)) {
        return;
    }

    operatorPresetViews_ = std::vector<std::unique_ptr<OperatorPresetView>>();
    for (const auto &entry: std::filesystem::directory_iterator(kPresetDirName.toStdString())) {
        if (!entry.is_directory()) {
            auto name = QString::fromStdString(entry.path().filename().string()).split(".").at(0);
            auto preset = loadJsonFileAsObject<Preset>(
                    entry.path().string());
            preset.name = name;
            auto presetView = std::make_unique<OperatorPresetView>(const_cast<OperatorPresetsView *>(this), name, std::move(preset));
            operatorPresetViews_->push_back(std::move(presetView));
        }
    }
}

void OperatorPresetsView::updateSizes() {
    presetBoxSize_.setWidth((containerWidth_ - (kOuterPaddingX * 2.0 + kBetweenPaddingX * kColumnCount)) / kColumnCount);
    presetBoxSize_.setHeight((containerHeight_ - (kOuterPaddingY * 2.0 + kBetweenPaddingY * kRowCount)) / kRowCount);

    const auto contentHeight = ceil(static_cast<double>(operatorPresetViews_->size() + 1) / 3.0) * (presetBoxSize_.height() + kBetweenPaddingY) + kOuterPaddingY * 2.0;
    if (std::abs(contentHeight - calculatedContentHeight_) > 0.1) {
        calculatedContentHeight_ = contentHeight;
        calculatedContentHeightChanged(calculatedContentHeight_);
    }
}

void OperatorPresetsView::addNewPreset() {
    if (Controller::instance->operators().empty()) {
        AlertController::instance->showAlert("Cannot create preset with no operators",1);
        return;
    }

    DialogController::instance->
            showDialog("Create preset", "Save current operators as a new preset", "", "Name of preset", "Create",
                       "Cancel",
                       [this](const QString &presetName) {
                            if (presetName.isEmpty()) {
                                AlertController::instance->showAlert("Preset name cannot be empty", true);
                                return false;
                            }
                            if (presetName.length() > 10) {
                                AlertController::instance->showAlert("Name of preset cannot be longer than 10 characters", true);
                                return false;
                            }
                           if (QFile(kPresetDirName + "/" + presetName + ".json").exists()) {
                               AlertController::instance->showAlert("Preset of the same name already exists", true);
                               return false;
                           }

                           const auto &controller = Controller::instance;
                           controller->savePreset(presetName.toStdString());

                           auto presetView = std::make_unique<OperatorPresetView>(const_cast<OperatorPresetsView *>(this), presetName,
                                                                Preset(controller->operators(), controller->attackAmpEnvValues(),
                                                                       controller->releaseAmpEnvValues(),presetName,
                                                                       controller->isLfoEnabled(), controller->lfoFrequency()));
                           operatorPresetViews_->push_back(std::move(presetView));

                           return true;
                       });
}

const TouchPoint &OperatorPresetsView::touchPoint() {
    return lastTouchPoint_;
}

void OperatorPresetsView::touchEvent(QTouchEvent *event) {
    QQuickItem::touchEvent(event);

    const auto& points = event->points();
    switch (event->type()) {
        case QEvent::TouchBegin:
            if (!points.empty()) {
                lastTouchPoint_.isPressed = true;
                lastTouchPoint_.position = points.first().position();
                event->accept();
            }
            break;
        case QEvent::TouchUpdate:
            if (!points.empty()) {
                lastTouchPoint_.position = points.first().position();
            }
            event->accept();
            break;
        case QEvent::TouchEnd:
            lastTouchPoint_.isPressed = false;
            event->accept();
            break;
        case QEvent::TouchCancel:
            lastTouchPoint_.isPressed = false;
            event->accept();
            break;
        default:
            break;
    }
}

void OperatorPresetsView::mousePressEvent(QMouseEvent *event) {
    QQuickItem::mousePressEvent(event);

    lastTouchPoint_.isPressed = true;
    lastTouchPoint_.position = event->position();
    event->accept();
}

void OperatorPresetsView::mouseMoveEvent(QMouseEvent *event) {
    QQuickItem::mouseMoveEvent(event);

    lastTouchPoint_.position = event->position();
    event->accept();
}

void OperatorPresetsView::mouseReleaseEvent(QMouseEvent *event) {
    QQuickItem::mouseReleaseEvent(event);

    lastTouchPoint_.isPressed = false;
    lastTouchPoint_.position = event->position();
    event->accept();
}
