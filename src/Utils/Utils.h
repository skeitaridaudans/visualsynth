//
// Created by gudmundur on 3.3.2023.
//

#ifndef QTQUICKTEST_UTILS_H
#define QTQUICKTEST_UTILS_H


#include <QPointF>
#include <QRect>
#include <QColor>
#include <QVector2D>
#include <algorithm>
#include <qdebug.h>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

inline QPointF closestPointInBox(const QPointF& point, const QPointF& boxPosition, const double width, const double height) {
    const auto minX = boxPosition.x();
    const auto maxX = boxPosition.x() + width;
    const auto minY = boxPosition.y();
    const auto maxY = boxPosition.y() + height;

    return QPointF(std::clamp(point.x(), minX, maxX), std::clamp(point.y(), minY, maxY));
}

inline bool isRectInsideLine(const QRectF& rect, const QPointF& lineStart, const QPointF& lineEnd) {
    // This assumes the line is horizontal, so lineStart.y has to be the same as lineEnd.y
    return rect.left() >= lineStart.x() && rect.right() <= lineEnd.x() &&
           rect.top() <= lineStart.y() && rect.bottom() >= lineStart.y();
}

inline bool isPointInsideRect(const QPointF& point, const QRectF& rect) {
    return point.x() >= rect.x() &&
            point.x() <= rect.x() + rect.width() && // point.x is within x range of rect
            point.y() >= rect.y() &&
            point.y() <= rect.y() + rect.height(); // point.y is within y range of rect
}

inline QPointF moveBetweenRects(const QPointF& point, const QRectF& from, const QRectF& to) {
    const auto x = ((point.x() - from.x()) / from.width()) * to.width() + to.x();
    const auto y = ((point.y() - from.y()) / from.height()) * to.height() + to.y();

    return {x, y};
}

inline QVector2D pointToVector(const QPointF& point) {
    return {(float) point.x(), (float) point.y()};
}

inline QVector2D vectorBetweenPoints(const QPointF& from, const QPointF& to) {
    return pointToVector(to) - pointToVector(from);
}

inline void addVectorToPoint(QPointF& point, const QVector2D& vector) {
    point.setX(point.x() + vector.x());
    point.setY(point.y() + vector.y());
}

inline QPointF operator +(const QPointF& point, const QVector2D& vector) {
    return { point.x() + vector.x(), point.y() + vector.y() };
}

inline QColor colorLerp(const QColor& from, const QColor& to, const double fraction) {
    const auto red = (int) ((to.red() - from.red()) * fraction + from.red());
    const auto green = (int) ((to.green() - from.green()) * fraction + from.green());
    const auto blue = (int) ((to.blue() - from.blue()) * fraction + from.blue());

    return {red, green, blue};
}

inline QPointF pointLerp(const QPointF& from, const QPointF& to, const double fraction) {
    const auto x = ((to.x() - from.x()) * fraction + from.x());
    const auto y = ((to.y() - from.y()) * fraction + from.y());

    return {x, y};
}

inline json loadJsonFile(const std::string& path) {
    std::ifstream file(path);
    return json::parse(file);
}

template <class T>
inline T loadJsonFileAsObject(const std::string& path) {
    const auto json = loadJsonFile(path);
    return json.get<T>();
}

#endif //QTQUICKTEST_UTILS_H
