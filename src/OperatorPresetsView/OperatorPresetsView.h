//
// Created by Guðmundur on 4/2/2023.
//

#ifndef MAIN_QML_OPERATORPRESETSVIEW_H
#define MAIN_QML_OPERATORPRESETSVIEW_H


#include <QQuickPaintedItem>
#include "src/Controller/Operator.h"
#include "src/Controller/Controller.h"
#include "src/Utils/ColorTweenAnimation.h"
#include "OperatorPresetView.h"

class OperatorPresetView;

class OperatorPresetsView : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit OperatorPresetsView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

private:
    void paintAddPresetButton(QPainter *painter, const QPointF &pos);
    void loadPresets();
    void updateSizes();
    void addNewPreset();

    std::optional<std::vector<OperatorPresetView>> operatorPresetViews_;
    QSizeF presetBoxSize;
    ColorTweenAnimation addPresetBackgroundAnim_;
};


#endif //MAIN_QML_OPERATORPRESETSVIEW_H
