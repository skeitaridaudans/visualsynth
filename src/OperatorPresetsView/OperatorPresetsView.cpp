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

const QFont kPresetNameFont("Noto Sans", 12);
const double kOuterPaddingX = 20.0;
const double kOuterPaddingY = 16.0;
const double kBetweenPaddingX = 16.0;
const double kBetweenPaddingY = 16.0;
const double kRowCount = 3.0;
const double kColumnCount = 3.0;
const double kOperatorSize = 16.0;
const double kOperatorBoxPadding = 0.1;

OperatorPresetsView::OperatorPresetsView(QQuickItem *parent) : QQuickPaintedItem(parent) {

}

void OperatorPresetsView::paint(QPainter *painter) {
    if (presetNames_ == std::nullopt) {
        loadPresets();
    }
    updateSizes();

    for (int i = 0; i < (int) std::ceil((double) presetNames_->size() / 3.0); i++) {
        for (int j = 0; j < std::min((int) presetNames_->size() - i * 3, 3); j++) {
            const auto x = kOuterPaddingX + (j * (presetBoxSize.width() + kBetweenPaddingX));
            const auto y = kOuterPaddingY + (i * (presetBoxSize.height() + kBetweenPaddingX));
            paintPreset(painter, QPointF(x, y), presetNames_->at(i + j), presets_.value()[i + j]);
        }
    }

    const auto addButtonGridPosI = (int) std::ceil((double) (presetNames_->size() + 1) / 3.0) - 1;
    const auto addButtonGridPosJ = std::min((int) presetNames_->size() - addButtonGridPosI * 3, 3);
    const auto x = kOuterPaddingX + (addButtonGridPosJ * (presetBoxSize.width() + kBetweenPaddingX));
    const auto y = kOuterPaddingY + (addButtonGridPosI * (presetBoxSize.height() + kBetweenPaddingX));
    paintAddPresetButton(painter, QPointF(x, y));

    update();
}

void OperatorPresetsView::paintAddPresetButton(QPainter *painter, const QPointF &pos) {
    const auto buttonRect = QRectF(pos, QSizeF(presetBoxSize.width(), (presetBoxSize.height() / 3.0) * 2.0));

    painter->setPen(Qt::PenStyle::NoPen);
    painter->setBrush(QColor(0x212121));
    painter->drawRoundedRect(buttonRect, 8, 8);

    painter->setPen(QColor(255, 255, 255));
    painter->setFont(fontAwesome()->font(fa::fa_solid, 35));
    painter->drawText(buttonRect, Qt::AlignCenter, QString(fa::fa_plus));

    const auto cursorPos = mapFromGlobal(QCursor::pos());
    if (isPointInsideRect(cursorPos, buttonRect) && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        const auto& controller = Controller::instance;
        controller->saveOperators("test2");
    }
}

void OperatorPresetsView::paintPreset(QPainter *painter, const QPointF &pos, const QString &name,
                                      const std::unordered_map<int, std::unique_ptr<Operator>> &preset) {
    paintPresetPreview(painter, pos, QSizeF(presetBoxSize.width(), (presetBoxSize.height() / 3.0) * 2.0), preset);

    painter->setFont(kPresetNameFont);
    painter->setPen(QColor(255, 255, 255));
    painter->drawText(QRectF(QPointF(pos.x(), pos.y() + (presetBoxSize.height() / 3.0) * 2.0),
                             QSizeF(presetBoxSize.width(), presetBoxSize.height() / 3.0)), name,
                      QTextOption(Qt::AlignCenter));

    const auto cursorPos = mapFromGlobal(QCursor::pos());
    if (isPointInsideRect(cursorPos, QRectF(pos, presetBoxSize)) && QGuiApplication::mouseButtons() == Qt::LeftButton) {
        const auto& controller = Controller::instance;
        controller->setOperators(preset);
    }
}

void OperatorPresetsView::paintPresetPreview(QPainter *painter, const QPointF &pos, const QSizeF &size,
                                             const std::unordered_map<int, std::unique_ptr<Operator>> &preset) {
    const auto operatorMinMax = findMinMaxOfOperators(preset);
    const auto sourceRect = QRectF(operatorMinMax.first, QSizeF(operatorMinMax.second.x() - operatorMinMax.first.x(),
                                                                operatorMinMax.second.y() - operatorMinMax.first.y()));
    const auto destRect = QRectF(pos + QPointF(size.width() * kOperatorBoxPadding, size.height() * kOperatorBoxPadding),
                                 size - (size * kOperatorBoxPadding * 2.0) - QSizeF(kOperatorSize, kOperatorSize));

    painter->setPen(Qt::PenStyle::NoPen);
    painter->setBrush(QColor(0x212121));
    painter->drawRoundedRect(QRectF(pos, size), 8, 8);

    // Draw the modulator lines
    for (const auto &operator_: preset) {
        for (const auto opId: operator_.second->modulatedBy) {
            const auto operatorCenterOffset = QPointF(kOperatorSize / 2.0, kOperatorSize / 2.0);
            const auto modulatorPos =
                    moveBetweenRects(preset.at(opId)->position, sourceRect, destRect) +
                    operatorCenterOffset;
            const auto modulatedPos =
                    moveBetweenRects(operator_.second->position, sourceRect, destRect) + operatorCenterOffset;

            painter->setPen(preset.at(opId)->getColorForOperator());
            painter->drawLine(modulatorPos, modulatedPos);
        }
    }

    // Draw the operators
    painter->setPen(Qt::PenStyle::NoPen);
    for (const auto &operator_: preset) {
        painter->setBrush(QBrush(operator_.second->getColorForOperator()));

        const auto operatorRectPos = moveBetweenRects(operator_.second->position, sourceRect, destRect);
        painter->drawRoundedRect(QRectF(operatorRectPos, QSizeF(kOperatorSize, kOperatorSize)), 4, 4);
    }
}

void OperatorPresetsView::loadPresets() {
    if (!std::filesystem::directory_entry("presets/").exists()) {
        qDebug() << "Presets directory does not exist";
        return;
    }

    auto presetFileNames = QStringList();
    std::vector<std::unordered_map<int, std::unique_ptr<Operator>>> operatorPresets;

    for (const auto &entry: std::filesystem::directory_iterator("presets/")) {
        if (!entry.is_directory()) {
            presetFileNames.push_back(QString::fromStdString(entry.path().filename().string()).split(".").at(0));

            auto preset = loadJsonFileAsObject<std::unordered_map<int, std::unique_ptr<Operator>>>(
                    entry.path().string());
            operatorPresets.push_back(std::move(preset));
        }
    }

    presetNames_ = std::move(presetFileNames);
    presets_ = std::move(operatorPresets);
}

void OperatorPresetsView::updateSizes() {
    presetBoxSize.setWidth((width() - (kOuterPaddingX * 2.0 + kBetweenPaddingX * kColumnCount)) / kColumnCount);
    presetBoxSize.setHeight((height() - (kOuterPaddingY * 2.0 + kBetweenPaddingY * kRowCount)) / kRowCount);
}

std::pair<QPointF, QPointF>
OperatorPresetsView::findMinMaxOfOperators(const std::unordered_map<int, std::unique_ptr<Operator>> &operators) {
    std::optional<QPointF> min = std::nullopt;
    std::optional<QPointF> max = std::nullopt;

    for (const auto &operator_: operators) {
        if (min == std::nullopt) {
            min = operator_.second->position;
            max = QPointF(min->x() + (kOperatorSize / width()), min->y() + (kOperatorSize / height()));
        } else {
            const auto operatorPos = operator_.second->position;
            if (operatorPos.x() < min->x()) {
                min->setX(operatorPos.x());
            }
            if (operatorPos.y() < min->y()) {
                min->setY(operatorPos.y());
            }

            if (operatorPos.x() > max->x()) {
                max->setX(operatorPos.x() + (kOperatorSize / width()));
            }
            if (operatorPos.y() > max->y()) {
                max->setY(operatorPos.y() + (kOperatorSize / height()));
            }
        }
    }

    return std::make_pair(min.value(), max.value());
}

QPointF OperatorPresetsView::moveBetweenRects(const QPointF &point, const QRectF &from, const QRectF &to) {
    const auto x = ((point.x() - from.x()) / from.width()) * to.width() + to.x();
    const auto y = ((point.y() - from.y()) / from.height()) * to.height() + to.y();

    return {x, y};
}
