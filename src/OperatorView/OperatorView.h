//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_BOXVIEW_H
#define QTQUICKTEST_BOXVIEW_H


#include <QtCore>
#include <QQuickWindow>
#include <QPainter>
#include <QQuickPaintedItem>
#include "AddOperatorBox.h"
#include "OperatorDrawer.h"
#include "DeleteOperatorBox.h"

class AddOperatorBox;

class DeleteOperatorBox;

class OperatorDrawer;

class OperatorView : public QQuickPaintedItem {
    Q_OBJECT
public:
    OperatorView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    void addOperator(double x, double y);
    std::pair<QPointF, QPointF> carrierLineEndPoints();

    const std::unique_ptr<DeleteOperatorBox>& deleteOperatorBox();
private:
    std::unique_ptr<OperatorDrawer> operatorDrawer_;
    std::unique_ptr<AddOperatorBox> newBox_;
    std::unique_ptr<DeleteOperatorBox> deleteOperatorBox_;

    void drawCarrierLine(QPainter *painter);
};


#endif //QTQUICKTEST_BOXVIEW_H
