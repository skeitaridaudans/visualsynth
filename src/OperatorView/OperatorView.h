//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_BOXVIEW_H
#define QTQUICKTEST_BOXVIEW_H


#include <QtCore>
#include <QQuickWindow>
#include <QPainter>
#include <QQuickPaintedItem>
#include "src/Utils/TouchPoint.h"
#include "AddOperatorBox.h"
#include "OperatorDrawer.h"
#include "DeleteOperatorBox.h"
#include "src/Utils/TweenAnimation.h"

class AddOperatorBox;

class DeleteOperatorBox;

class OperatorDrawer;

enum class TouchEventHandledState {
    None,
    Unhandled,
    Handled
};

class OperatorView : public QQuickPaintedItem {
    Q_OBJECT
public:
    OperatorView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    void addOperator(double x, double y);
    std::pair<QPointF, QPointF> carrierLineEndPoints();
    const std::unique_ptr<DeleteOperatorBox>& deleteOperatorBox();
    QPointF toViewCoords(const QPointF& pos);
    QPointF fromViewCoords(const QPointF& pos);
    const TouchPoint &primaryTouchPoint();
    TouchPoint &secondaryTouchPoint();
    bool isUsingMouse();

    TouchEventHandledState touchPressHandledState_ = TouchEventHandledState::None;
protected:
    void touchEvent(QTouchEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    std::unique_ptr<OperatorDrawer> operatorDrawer_;
    std::unique_ptr<AddOperatorBox> newBox_;
    std::unique_ptr<DeleteOperatorBox> deleteOperatorBox_;
    TweenAnimation carrierLinePulseAnim_;
    double carrierLineBoxOpacity_ = 1.0;
    TouchPoint primaryTouchPoint_;
    TouchPoint secondaryTouchPoint_;
    bool isUsingMouse_;

    void drawCarrierLine(QPainter *painter);
};


#endif //QTQUICKTEST_BOXVIEW_H
