//
// Created by Guðmundur on 4/2/2023.
//

#ifndef MAIN_QML_OPERATORPRESETSVIEW_H
#define MAIN_QML_OPERATORPRESETSVIEW_H


#include <QQuickPaintedItem>
#include "src/Controllers/Controller/Operator.h"
#include "src/Controllers/Controller/Controller.h"
#include "src/Utils/Animations/ColorTweenAnimation.h"
#include "src/Utils/TouchPoint.h"
#include "OperatorPresetView.h"

class OperatorPresetView;

class OperatorPresetsView : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit OperatorPresetsView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    const TouchPoint& touchPoint();

    Q_PROPERTY(double calculatedContentHeight MEMBER calculatedContentHeight_ NOTIFY calculatedContentHeightChanged);
    Q_PROPERTY(double containerWidth MEMBER containerWidth_ NOTIFY containerWidthChanged);
    Q_PROPERTY(double containerHeight MEMBER containerHeight_ NOTIFY containerHeightChanged);

signals:
    Q_SIGNAL void calculatedContentHeightChanged(double calculatedContentHeight);
    Q_SIGNAL void containerWidthChanged(double containerWidth);
    Q_SIGNAL void containerHeightChanged(double containerHeight);

protected:
    void touchEvent(QTouchEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void paintAddPresetButton(QPainter *painter, const QPointF &pos);
    void loadPresets();
    void updateSizes();
    void addNewPreset();

    std::optional<std::vector<std::unique_ptr<OperatorPresetView>>> operatorPresetViews_;
    QSizeF presetBoxSize_;
    ColorTweenAnimation addPresetBackgroundAnim_;
    TouchPoint lastTouchPoint_;
    QColor addPresetBackgroundColor_;
    double calculatedContentHeight_;
    double containerWidth_;
    double containerHeight_;
};


#endif //MAIN_QML_OPERATORPRESETSVIEW_H
