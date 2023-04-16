//
// Created by Guðmundur on 4/7/2023.
//

#include "OperatorPresetView.h"
#include <QPainter>
#include <QCursor>
#include <QGuiApplication>
#include "src/Utils/Utils.h"

const QFont kPresetNameFont("Noto Sans", 12);
const double kOperatorSize = 16.0;
const double kOperatorBoxPadding = 0.1;
const QColor kPresetBackgroundColor = QColor(0x212121);
const QColor kPresetBackgroundHoverColor = QColor(0x161616);
const double kPresetBackgroundAnimTime = 100.0;

OperatorPresetView::OperatorPresetView(OperatorPresetsView *operatorPresetView, QString name, Preset preset)
        : operatorPresetsView_(operatorPresetView), name_(std::move(name)), preset_(std::move(preset)),
          presetBackgroundAnim_(kPresetBackgroundAnimTime, kPresetBackgroundColor, kPresetBackgroundHoverColor, AnimationCurves::easeOut) {
    operatorsMinMax_ = findMinMaxOfOperators(preset_.operators);
}

void OperatorPresetView::update(const QPointF &pos, const QSizeF &size) {
    const auto touchPointPos = operatorPresetsView_->touchPoint().position;

    if (isPointInsideRect(touchPointPos, QRectF(pos, size)) && operatorPresetsView_->touchPoint().isPressed) {
        presetBackgroundAnim_.setForward();
        const auto &controller = Controller::instance;
        controller->changeToPreset(preset_);
    }
    else if (presetBackgroundAnim_.isAtEnd()) {
        presetBackgroundAnim_.setReverse();
    }

    presetBackgroundAnim_.update();
}

void OperatorPresetView::paint(QPainter *painter, const QPointF &pos, const QSizeF &size) {
    paintPreview(painter, pos, QSizeF(size.width(), (size.height() / 3.0) * 2.0));

    painter->setFont(kPresetNameFont);
    painter->setPen(QColor(255, 255, 255));
    painter->drawText(QRectF(QPointF(pos.x(), pos.y() + (size.height() / 3.0) * 2.0),
                             QSizeF(size.width(), size.height() / 3.0)), name_,
                      QTextOption(Qt::AlignCenter));
}

void OperatorPresetView::paintPreview(QPainter *painter, const QPointF &pos, const QSizeF &size) {
    const auto sourceRect = QRectF(operatorsMinMax_.first,
                                   QSizeF(operatorsMinMax_.second.x() - operatorsMinMax_.first.x(),
                                          operatorsMinMax_.second.y() - operatorsMinMax_.first.y()));
    const auto destRect = QRectF(pos + QPointF(size.width() * kOperatorBoxPadding, size.height() * kOperatorBoxPadding),
                                 size - (size * kOperatorBoxPadding * 2.0) - QSizeF(kOperatorSize, kOperatorSize));

    painter->setPen(Qt::PenStyle::NoPen);
    painter->setBrush(QColor(presetBackgroundAnim_.value()));
    painter->drawRoundedRect(QRectF(pos, size), 8, 8);

    // Draw the modulator lines
    for (const auto &operator_: preset_.operators) {
        for (const auto opId: operator_.second.modulatedBy) {
            const auto operatorCenterOffset = QPointF(kOperatorSize / 2.0, kOperatorSize / 2.0);
            const auto modulatorPos =
                    moveBetweenRects(preset_.operators.at(opId).position, sourceRect, destRect) +
                    operatorCenterOffset;
            const auto modulatedPos =
                    moveBetweenRects(operator_.second.position, sourceRect, destRect) + operatorCenterOffset;

            painter->setPen(preset_.operators.at(opId).getColorForOperator());
            painter->drawLine(modulatorPos, modulatedPos);
        }
    }

    // Draw the operators
    painter->setPen(Qt::PenStyle::NoPen);
    for (const auto &operator_: preset_.operators) {
        painter->setBrush(QBrush(operator_.second.getColorForOperator()));

        const auto operatorRectPos = moveBetweenRects(operator_.second.position, sourceRect, destRect);
        painter->drawRoundedRect(QRectF(operatorRectPos, QSizeF(kOperatorSize, kOperatorSize)), 4, 4);
    }
}

std::pair<QPointF, QPointF>
OperatorPresetView::findMinMaxOfOperators(const Operators &operators) {
    std::optional<QPointF> min = std::nullopt;
    std::optional<QPointF> max = std::nullopt;

    for (const auto &operator_: operators) {
        if (min == std::nullopt) {
            min = operator_.second.position;
            max = QPointF(min->x() + (kOperatorSize / operatorPresetsView_->width()),
                          min->y() + (kOperatorSize / operatorPresetsView_->height()));
        } else {
            const auto operatorPos = operator_.second.position;
            if (operatorPos.x() < min->x()) {
                min->setX(operatorPos.x());
            }
            if (operatorPos.y() < min->y()) {
                min->setY(operatorPos.y());
            }

            if (operatorPos.x() > max->x()) {
                max->setX(operatorPos.x() + (kOperatorSize / operatorPresetsView_->width()));
            }
            if (operatorPos.y() > max->y()) {
                max->setY(operatorPos.y() + (kOperatorSize / operatorPresetsView_->height()));
            }
        }
    }

    return std::make_pair(min.value(), max.value());
}


