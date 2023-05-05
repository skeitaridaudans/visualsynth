//
// Created by Guðmundur on 2/6/2023.
//

#include "AddOperatorBox.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>
#include "src/FontAwesome.h"
#include "src/Controller/Controller.h"

const double kBorderWidth = 0.02;
const double kBoxSize = 70.0;
const double kRightAnchor = 20.0;
const double kPositionY = 60.0;
const double kCornerRadius = 5.0;

AddOperatorBox::AddOperatorBox(OperatorView *operatorView) : operatorView_(operatorView) {}

void AddOperatorBox::update() {
    boxPos_.setX(operatorView_->width() - kBoxSize - kRightAnchor);
    boxPos_.setY(kPositionY);

    const auto pos = operatorView_->primaryTouchPoint().position;

    if (isInsideBox(pos) && operatorView_->primaryTouchPoint().isPressed && !operatorCreated_ && !isAnyOperatorBeingDragged()) {
        const auto newOperatorPos = operatorView_->fromViewCoords(QPointF(pos.x() - kBoxSize / 2.0, pos.y() - kBoxSize / 2.0));
        operatorView_->addOperator(newOperatorPos.x(), newOperatorPos.y());
        operatorCreated_ = true;
        operatorView_->touchPressHandledState_ = TouchEventHandledState::Handled;
    }
    else if (!isInsideBox(pos) && !operatorView_->primaryTouchPoint().isPressed) {
        operatorCreated_ = false;
    }
}

void AddOperatorBox::draw(QPainter* painter) {
    QBrush brush(QColor(0x212121));

    const auto rect = QRectF(boxPos_, QSizeF(kBoxSize, kBoxSize));

    painter->setBrush(brush);
    painter->setPen(Qt::PenStyle::SolidLine);
    painter->setPen(QColor(255, 255, 255));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(rect, kCornerRadius, kCornerRadius);

    painter->setFont(fontAwesome()->font(fa::fa_solid, 45));
    painter->drawText(rect, Qt::AlignCenter, QString(fa::fa_plus));
}

bool AddOperatorBox::isInsideBox(const QPointF &coords) {
    return coords.x() >= boxPos_.x() && coords.x() < boxPos_.x() + kBoxSize && coords.y() >= boxPos_.y() && coords.y() < boxPos_.y() + kBoxSize;
}

bool AddOperatorBox::isAnyOperatorBeingDragged() {
    const auto& controller = Controller::instance;
    return q20::ranges::any_of(controller->operators().begin(), controller->operators().end(), [] (const auto& kv) { return kv.second.operatorViewState.draggingState != DraggingState::None; });
}

