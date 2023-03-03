//
// Created by gudmundur on 3.3.2023.
//

#include "Utils.h"
#include <algorithm>

QPointF closestPointInBox(const QPointF& point, const QPointF& boxPosition, const double width, const double height) {
    const auto minX = boxPosition.x();
    const auto maxX = boxPosition.x() + width;
    const auto minY = boxPosition.y();
    const auto maxY = boxPosition.y() + height;

    return QPointF(std::clamp(point.x(), minX, maxX), std::clamp(point.y(), minY, maxY));
}