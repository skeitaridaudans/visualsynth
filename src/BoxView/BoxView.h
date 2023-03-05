//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_BOXVIEW_H
#define QTQUICKTEST_BOXVIEW_H


#include <QtCore>
#include <QQuickWindow>
#include <QPainter>
#include <QQuickPaintedItem>
#include "NewBox.h"
#include "OperatorDrawer.h"

class NewBox;

class OperatorDrawer;

class BoxView : public QQuickPaintedItem {
    Q_OBJECT
public:
    BoxView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    void addOperator(double x, double y);
    std::pair<QPointF, QPointF> carrierLineEndPoints();
private:
    std::unique_ptr<OperatorDrawer> operatorDrawer_;
    std::unique_ptr<NewBox> newBox_;

    void drawCarrierLine(QPainter *painter);
};


#endif //QTQUICKTEST_BOXVIEW_H
