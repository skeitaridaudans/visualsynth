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
#include "src/Utils/Animations/TweenAnimation.h"

class AmpEnvGraphView : public QQuickPaintedItem {
Q_OBJECT
public:
    explicit AmpEnvGraphView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    Q_PROPERTY(bool operatorEnvelope MEMBER operatorEnvelope_ NOTIFY operatorEnvelopeChanged);

signals:
    Q_SIGNAL void operatorEnvelopeChanged(bool operatorEnvelope);

private:
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
    double draggableAreaWidth();
    double draggableAreaHeight();
    QColor graphColor();
    QColor paramBorderColor();
    const std::vector<AmpEnvValue> &currentAttackEnvelope();
    const std::vector<AmpEnvValue> &currentReleaseEnvelope();
    void setAttackEnvelopeValue(int index, float value, float time);
    void setReleaseEnvelopeValue(int index, float value, float time);

    QColor borderColor = QColor(Qt::gray);
    std::optional<DraggingTouchPoint> draggingTouchPoint_;
    double draggingParamOpacity_ = 1.0;
    double draggingParamScale_ = 1.0;
    TweenAnimation paramOpacityAnim_;
    TweenAnimation paramScaleAnim_;
    // This is needed because draggingTouchPoint_ becomes null immediately after dragging stops,
    // but we want to finish the animation first
    int draggingAnimParamIndex_ = 0;
    bool operatorEnvelope_ = false;

protected:
    void touchEvent(QTouchEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // AMPENVGRAPHVIEW_H
