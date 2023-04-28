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
#include "src/Utils/TweenAnimation.h"

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
    QPointF mapAmpEnvPointToView(const QPointF &point, bool isAttack);
    QPointF mapViewPointToAmpEnvPoint(const QPointF &point, bool isAttack);
    QPointF clampBetweenAdjacentPoints(const AmpEnvValue& ampEnvValue, const QPointF& desiredPos);
    QPointF getDrawingPosOfAmpEnvParam(const AmpEnvValue& param);
    const AmpEnvValue *findTouchedAmpEnvPoint(const QPointF& touchPoint);
    const AmpEnvValue &getDraggingAmpEnvValue();
    bool startDragging(int touchPointId, const QPointF& pos);
    void updateDragging(QPointF draggingPos);

    QColor borderColor = QColor(Qt::gray);
    std::optional<DraggingTouchPoint> draggingTouchPoint_;
    double draggingParamOpacity_ = 1.0;
    double draggingParamScale_ = 1.0;
    TweenAnimation paramOpacityAnim_;
    TweenAnimation paramScaleAnim_;

protected:
    void touchEvent(QTouchEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // AMPENVGRAPHVIEW_H
