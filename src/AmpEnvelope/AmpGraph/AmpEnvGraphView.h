#ifndef AMPENVGRAPHVIEW_H
#define AMPENVGRAPHVIEW_H

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <QQuickPaintedItem>
#include "src/Controller/AmpEnvValue.h"
#include "DraggingTouchPoint.h"

class AmpEnvGraphView : public QQuickPaintedItem {
Q_OBJECT
public:
    explicit AmpEnvGraphView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

private:
    void paintGraphContainer(QPainter *painter);
    void paintParams(QPainter *painter);
    void paintLines(QPainter *painter);
    void paintParam(QPainter *painter, const AmpEnvValue &param);
    QPointF mapAmpEnvPointToView(const QPointF &point);
    QPointF mapViewPointToAmpEnvPoint(const QPointF &point);
    const AmpEnvValue* findTouchedAmpEnvPoint(const QPointF& touchPoint);
    const AmpEnvValue &getDraggingAmpEnvValue();

    QColor borderColor = QColor(Qt::gray);
    std::optional<DraggingTouchPoint> draggingTouchPoint_;

protected:
    void touchEvent(QTouchEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // AMPENVGRAPHVIEW_H
