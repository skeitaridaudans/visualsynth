//
// Created by Guðmundur on 4/2/2023.
//

#include <QCursor>
#include <QPainter>
#include <filesystem>
#include <QGuiApplication>
#include "OperatorPresetsView.h"
#include "src/Controller/Operator.h"
#include "src/Utils/Utils.h"
#include "src/Controller/Controller.h"
#include "src/FontAwesome.h"
#include "src/Dialog/DialogController.h"
#include "src/Alert/AlertController.h"

const double kOuterPaddingX = 20.0;
const double kOuterPaddingY = 16.0;
const double kBetweenPaddingX = 16.0;
const double kBetweenPaddingY = 16.0;
const int kRowCount = 3;
const int kColumnCount = 3;
const QColor kAddPresetBackgroundColor = QColor(0x212121);
const QColor kAddPresetBackgroundHoverColor = QColor(0x161616);
const double kAddPresetBackgroundAnimTime = 100.0;

OperatorPresetsView::OperatorPresetsView(QQuickItem *parent) : QQuickPaintedItem(parent),
                                                               addPresetBackgroundAnim_(kAddPresetBackgroundAnimTime,
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
            const auto x = kOuterPaddingX + (j * (presetBoxSize.width() + kBetweenPaddingX));
            const auto y = kOuterPaddingY + (i * (presetBoxSize.height() + kBetweenPaddingX));
            operatorPresetViews_->at((i * 3) + j).update(QPointF(x, y), presetBoxSize);
            operatorPresetViews_->at((i * 3) + j).paint(painter, QPointF(x, y), presetBoxSize);
        }
    }

    const auto addButtonGridPosI = (int) std::ceil((double) (operatorPresetViews_->size() + 1) / kRowCount) - 1;
    const auto addButtonGridPosJ = std::min((int) operatorPresetViews_->size() - addButtonGridPosI * kColumnCount, kColumnCount);
    const auto x = kOuterPaddingX + (addButtonGridPosJ * (presetBoxSize.width() + kBetweenPaddingX));
    const auto y = kOuterPaddingY + (addButtonGridPosI * (presetBoxSize.height() + kBetweenPaddingX));
    paintAddPresetButton(painter, QPointF(x, y));

    update();
}

void OperatorPresetsView::paintAddPresetButton(QPainter *painter, const QPointF &pos) {
    const auto buttonRect = QRectF(pos, QSizeF(presetBoxSize.width(), (presetBoxSize.height() / 3.0) * 2.0));

    painter->setPen(Qt::PenStyle::NoPen);
    painter->setBrush(addPresetBackgroundAnim_.value());
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
    if (!std::filesystem::directory_entry("presets/").exists()) {
        qDebug() << "Presets directory does not exist";
        return;
    }

    std::vector<OperatorPresetView> operatorPresetViews;
    for (const auto &entry: std::filesystem::directory_iterator("presets/")) {
        if (!entry.is_directory()) {
            auto name = QString::fromStdString(entry.path().filename().string()).split(".").at(0);
            auto preset = loadJsonFileAsObject<Preset>(
                    entry.path().string());
            auto presetView = OperatorPresetView(const_cast<OperatorPresetsView *>(this), name, std::move(preset));
            operatorPresetViews.push_back(std::move(presetView));
        }
    }

    operatorPresetViews_ = std::move(operatorPresetViews);
}

void OperatorPresetsView::updateSizes() {
    presetBoxSize.setWidth((width() - (kOuterPaddingX * 2.0 + kBetweenPaddingX * kColumnCount)) / kColumnCount);
    presetBoxSize.setHeight((height() - (kOuterPaddingY * 2.0 + kBetweenPaddingY * kRowCount)) / kRowCount);
}

void OperatorPresetsView::addNewPreset() {
    if (Controller::instance->operators().empty()) {
        AlertController::instance->showAlert("Cannot create preset with no operators");
        return;
    }

    DialogController::instance->
            showDialog("Create preset", "Save current operators as a new preset", "Name", "Create",
                       "Cancel",
                       [this](const QString &presetName) {
                           const auto &controller = Controller::instance;
                           controller->savePreset(presetName.toStdString());

                           std::vector<AmpEnvValue> ampEnvValues (std::begin(controller->ampEnvValues()), std::end(controller->ampEnvValues()));
                           auto presetView = OperatorPresetView(const_cast<OperatorPresetsView *>(this), presetName,
                                                                Preset(controller->operators(), ampEnvValues));
                           operatorPresetViews_->push_back(std::move(presetView));
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