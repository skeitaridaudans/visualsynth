//
// Created by gudmundur on 3.3.2023.
//

#ifndef QTQUICKTEST_UTILS_H
#define QTQUICKTEST_UTILS_H


#include <QPointF>
#include <QRect>

QPointF closestPointInBox(const QPointF& point, const QPointF& boxPosition, double width, double height);

bool isRectInsideLine(const QRectF& rect, const QPointF& lineStart, const QPointF& lineEnd);

#endif //QTQUICKTEST_UTILS_H
