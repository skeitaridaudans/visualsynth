//
// Created by gudmundur on 3.3.2023.
//

#include "Utils.h"
#include <algorithm>
#include <qdebug.h>

QPointF closestPointInBox(const QPointF& point, const QPointF& boxPosition, const double width, const double height) {
    const auto minX = boxPosition.x();
    const auto maxX = boxPosition.x() + width;
    const auto minY = boxPosition.y();
    const auto maxY = boxPosition.y() + height;

    return QPointF(std::clamp(point.x(), minX, maxX), std::clamp(point.y(), minY, maxY));
}

bool isRectInsideLine(const QRectF& rect, const QPointF& lineStart, const QPointF& lineEnd) {
    // This assumes the line is horizontal, so lineStart.y has to be the same as lineEnd.y
    return rect.left() >= lineStart.x() && rect.right() <= lineEnd.x() &&
        rect.top() <= lineStart.y() && rect.bottom() >= lineStart.y();
}