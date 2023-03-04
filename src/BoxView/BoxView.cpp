//
// Created by Guðmundur on 2/6/2023.
//

#include "BoxView.h"
#include "src/Controller/Controller.h"

BoxView::BoxView(QQuickItem *parent) : QQuickPaintedItem(parent),
    newBox_(std::make_unique<NewBox>(const_cast<BoxView *>(this))),
    operatorDrawer_(std::make_unique<OperatorDrawer>(const_cast<BoxView *>(this))) {
}

void BoxView::paint(QPainter *painter) {
    newBox_->update();
    const auto& controller = Controller::instance;

    for (const auto& operator_: controller->operators()) {
        operatorDrawer_->update(operator_.second.get());
    }

    newBox_->draw(painter);
    for (const auto& operator_: controller->operators()) {
        operatorDrawer_->draw(painter, operator_.second.get());
    }

    update();
}

void BoxView::addOperator(double x, double y) {
    const auto& controller = Controller::instance;

    auto id = controller->addOperator();
    const auto& operator_ = controller->getOperatorById(id);

    operator_->position = QPoint(x, y);
}