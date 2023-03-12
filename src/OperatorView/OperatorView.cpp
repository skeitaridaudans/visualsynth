//
// Created by Guðmundur on 2/6/2023.
//

#include "OperatorView.h"
#include "src/Controller/Controller.h"

const int kCarrierLineTextLeftMargin = 20;
const int kCarrierLineLeftMargin = 10;
const int kCarrierLineRightMargin = 20;
const int kCarrierLineBottomMargin = 40;
const QString kCarrierText = "Carrier";
const QFont kCarrierFont("Noto Sans", 25);

OperatorView::OperatorView(QQuickItem *parent) : QQuickPaintedItem(parent),
                                                 newBox_(std::make_unique<AddOperatorBox>(const_cast<OperatorView *>(this))),
                                                 deleteOperatorBox_(std::make_unique<DeleteOperatorBox>(const_cast<OperatorView *>(this))),
                                                 operatorDrawer_(std::make_unique<OperatorDrawer>(const_cast<OperatorView *>(this))) {
}

void OperatorView::paint(QPainter *painter) {
    const auto& controller = Controller::instance;

    newBox_->update();
    deleteOperatorBox_->update();
    for (const auto& operator_: controller->operators()) {
        operatorDrawer_->update(operator_.second.get());
    }

    newBox_->draw(painter);
    deleteOperatorBox_->draw(painter);
    drawCarrierLine(painter);
    for (const auto& operator_: controller->operators()) {
        operatorDrawer_->draw(painter, operator_.second.get());
    }

    // Make a copy of all operator ids that should be deleted, then delete them in another loop.
    // Since it's not possible to delete elements from unordered_map while iterating over it
    std::vector<int> operatorToDelete;
    for (const auto& operator_ : controller->operators()) {
        if (operator_.second->scheduleForRemoval) {
            operatorToDelete.push_back(operator_.first);
        }
    }
    for (const auto& operatorId : operatorToDelete) {
        controller->removeOperator(operatorId);
    }

    update();
}

void OperatorView::drawCarrierLine(QPainter *painter) {
    // Calculate positions
    const auto carrierLineTextStartPos = QPointF(kCarrierLineTextLeftMargin, (int) height() - 40);

    const auto lineEndPoints = carrierLineEndPoints();
    const auto& carrierLineLeftPoint = lineEndPoints.first;
    const auto& carrierLineRightPoint = lineEndPoints.second;

    // Draw carrier line
    painter->setFont(kCarrierFont);
    painter->setPen(QColor(255, 255, 255));
    painter->drawText(carrierLineTextStartPos, kCarrierText);
    painter->drawLine(carrierLineLeftPoint, carrierLineRightPoint);
}

void OperatorView::addOperator(double x, double y) {
    const auto& controller = Controller::instance;

    if (const auto id = controller->addOperator()) {
        const auto& operator_ = controller->getOperatorById(*id);

        operator_->position = QPointF(x, y);
    }
    else {
        // TODO: Show error message if operator could not be added (due to operator limit reached)
    }
}

std::pair<QPointF, QPointF> OperatorView::carrierLineEndPoints() {
    const QFontMetrics fm(kCarrierFont);
    const auto textWidth = fm.horizontalAdvance(kCarrierText);
    const auto textHeight = fm.height();

    const auto carrierLineLeftPoint = QPointF(kCarrierLineTextLeftMargin + textWidth + kCarrierLineLeftMargin, (int) height() - kCarrierLineBottomMargin - ((double) textHeight / 4));
    const auto carrierLineRightPoint = QPointF((int) width() - kCarrierLineRightMargin, (int) height() - kCarrierLineBottomMargin - ((double) textHeight / 4));

    return {carrierLineLeftPoint, carrierLineRightPoint};
}

const std::unique_ptr<DeleteOperatorBox> &OperatorView::deleteOperatorBox() {
    return deleteOperatorBox_;
}
