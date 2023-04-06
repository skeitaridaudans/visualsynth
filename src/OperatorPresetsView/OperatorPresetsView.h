//
// Created by Guðmundur on 4/2/2023.
//

#ifndef MAIN_QML_OPERATORPRESETSVIEW_H
#define MAIN_QML_OPERATORPRESETSVIEW_H


#include <QQuickPaintedItem>
#include "src/Controller/Operator.h"

class OperatorPresetsView : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit OperatorPresetsView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

private:
    void paintAddPresetButton(QPainter *painter, const QPointF &pos);
    void paintPreset(QPainter *painter, const QPointF &pos, const QString& name, const std::unordered_map<int, std::unique_ptr<Operator>>& preset);
    void paintPresetPreview(QPainter *painter, const QPointF& pos, const QSizeF& size, const std::unordered_map<int, std::unique_ptr<Operator>>& preset);
    void loadPresets();
    void updateSizes();

    std::pair<QPointF, QPointF> findMinMaxOfOperators(const std::unordered_map<int, std::unique_ptr<Operator>>& operators);
    QPointF moveBetweenRects(const QPointF& point, const QRectF& from, const QRectF& to);
    std::optional<QStringList> presetNames_ = std::nullopt;
    std::optional<std::vector<std::unordered_map<int, std::unique_ptr<Operator>>>> presets_;
    QSizeF presetBoxSize;
};


#endif //MAIN_QML_OPERATORPRESETSVIEW_H
