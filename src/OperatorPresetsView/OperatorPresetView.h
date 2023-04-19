//
// Created by Guðmundur on 4/7/2023.
//

#ifndef MAIN_QML_OPERATORPRESETVIEW_H
#define MAIN_QML_OPERATORPRESETVIEW_H

#include "OperatorPresetsView.h"
#include "src/Utils/ColorTweenAnimation.h"

class OperatorPresetsView;

class OperatorPresetView {
public:
    OperatorPresetView(OperatorPresetsView *operatorPresetView, QString name, Preset preset);

    void update(const QPointF &pos, const QSizeF &size);
    void paint(QPainter *painter, const QPointF &pos, const QSizeF &size);
private:
    void paintPreview(QPainter *painter, const QPointF &pos, const QSizeF &size);
    std::pair<QPointF, QPointF> findMinMaxOfOperators(const Operators& operators);

    OperatorPresetsView* operatorPresetsView_;
    Preset preset_;
    QString name_;
    // The range of coordinates that the operators are inside
    std::pair<QPointF, QPointF> operatorsMinMax_;
    ColorTweenAnimation presetBackgroundAnim_;
    QColor presetBackgroundColor_;
};


#endif //MAIN_QML_OPERATORPRESETVIEW_H
