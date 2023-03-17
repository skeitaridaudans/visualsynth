//
// Created by Guðmundur on 2/6/2023.
//

#include "DeleteOperatorBox.h"
#include <QOpenGLFunctions>
#include <QCursor>
#include <QGuiApplication>
#include "src/FontAwesome.h"

const double kBorderWidth = 0.02;
const double kBoxSize = 70.0;
const double kLeftAnchor = 20.0;
const double kPositionY = 60.0;
const double kCornerRadius = 5.0;

DeleteOperatorBox::DeleteOperatorBox(OperatorView *boxView) : boxView_(boxView) {}

void DeleteOperatorBox::update() {
    boxPos_.setX(kLeftAnchor);
    boxPos_.setY(kPositionY);
}

void DeleteOperatorBox::draw(QPainter* painter) {
    QBrush brush(QColor(0x212121));

    const auto rect = QRectF(boxPos_, QSize(kBoxSize, kBoxSize));

    painter->setBrush(brush);
    painter->setPen(Qt::PenStyle::SolidLine);
    painter->setPen(QColor(255, 255, 255));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(rect, kCornerRadius, kCornerRadius);

    painter->setFont(fontAwesome()->font(fa::fa_solid, 45));
    painter->drawText(rect, Qt::AlignCenter, QString(fa::fa_trash_can));
}

bool DeleteOperatorBox::isInsideBox(const QPointF &coords) {
    return coords.x() >= boxPos_.x() && coords.x() < boxPos_.x() + kBoxSize
        && coords.y() >= boxPos_.y() && coords.y() < boxPos_.y() + kBoxSize;
}

