//
// Created by Sigurður on 3/25/2023.
//

#ifndef AMPENVPARAMS_H
#define AMPENVPARAMS_H

#include "src/AmpEnvelope/AmpGraph/AmpEnvGraphView.h"
#include <QQuickPaintedItem>

class AmpEnvGraphView;

class AmpEnvParams {
public:

    AmpEnvParams(AmpEnvGraphView *GraphView);

    void draw(QPainter *painter );
    int getWidth();
    int getHeight();

    void assignWidth(int new_width);
    void assignHeight(int new_height);
    void assignX(int newX);
    void assignY(int newY);
    void assignRGBColor(int r, int g, int b);
    void assignText(QString newTxt);

    QPointF coords;
    
private:
    QPoint paramPos_ = QPoint(10,15);
    int x_ = 5;
    int y_ = 5;

    int width_ = 27;
    int height_ = 27;
    int rgb_r = 0;
    int rgb_g = 0;
    int rgb_b = 0;

    QString text = "Stop";


    AmpEnvGraphView *GraphView_;

};


#endif //AMPENVPARAMS_H
