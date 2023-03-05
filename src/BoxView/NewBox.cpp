//
// Created by Guðmundur on 2/6/2023.
//

#include "NewBox.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>

const double kBorderWidth = 0.02;
const double kBoxSize = 70.0;
const double kRightAnchor = 20.0;
const double kPositionY = 60.0;

NewBox::NewBox(BoxView *boxView) : boxView_(boxView) {}

void NewBox::update() {
    boxPos_.setX(boxView_->width() - kBoxSize - kRightAnchor);
    boxPos_.setY(kPositionY);

    const auto pos = boxView_->mapFromGlobal(QCursor::pos());

    if (isInsideBox(pos) && QGuiApplication::mouseButtons() == Qt::LeftButton && !boxCreated_) {
        boxView_->addOperator(pos.x() - kBoxSize / 2.0, pos.y() - kBoxSize / 2.0);
        boxCreated_ = true;
    }
    else if (!isInsideBox(pos) && QGuiApplication::mouseButtons() != Qt::LeftButton) {
        boxCreated_ = false;
    }
}

void NewBox::draw(QPainter* painter) {
    QBrush brush(QColor(0, 0, 0));

    const auto rect = QRectF(boxPos_, QSize(kBoxSize, kBoxSize));

    painter->setBrush(brush);
    painter->setPen(Qt::PenStyle::SolidLine);
    painter->setPen(QColor(255, 255, 255));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rect);

    painter->setFont(QFont("Arial", 45));
    painter->drawText(rect, Qt::AlignCenter, "+");
}

QPointF NewBox::widgetCoordsToGl(const QPointF &coords) {
    return QPointF(coords.x() / boxView_->width() * 2.0 - 1.0, coords.y() / boxView_->height() * 2.0 - 1.0);
}

bool NewBox::isInsideBox(const QPointF &coords) {
    return coords.x() >= boxPos_.x() && coords.x() < boxPos_.x() + kBoxSize && coords.y() >= boxPos_.y() && coords.y() < boxPos_.y() + kBoxSize;
}

