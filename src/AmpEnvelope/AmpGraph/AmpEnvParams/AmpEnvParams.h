//
// Created by Sigurður on 3/25/2023.
//

#ifndef AMPENVPARAMS_H
#define AMPENVPARAMS_H

#include "src/AmpEnvelope/AmpGraph/AmpEnvGraphView.h"
#include "src/Controller/Operator.h"
#include <QQuickPaintedItem>
#include <QWidget>

class AmpEnvGraphView;

class AmpEnvParams : public QObject {
Q_OBJECT

public:

    AmpEnvParams(AmpEnvGraphView *GraphView);

    void draw(QPainter *painter );

    void assignX(int newX);
    void assignY(int newY);
    void assignRGBColor(int r, int g, int b);
    void assignText(QString newTxt);


    QPointF coords;
    int x_ = 5;
    int y_ = 5;
    int width_ = 20;
    int height_ = 20;
    int rgb_r = 0;
    int rgb_g = 0;
    int rgb_b = 0;

    QString text = "Stop";

    bool isBeingDragged;
    DraggingState draggingState = DraggingState::None;

    std::optional<QPointF> initialDragCursorPos;


private:

    QPointF dragOffset;
    AmpEnvGraphView *GraphView_;
};


#endif //AMPENVPARAMS_H
