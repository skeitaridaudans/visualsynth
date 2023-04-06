//
// Created by Sigurður on 3/25/2023.
//

#ifndef AMPENVPARAMS_H
#define AMPENVPARAMS_H

#include "src/AmpEnvelope/AmpGraph/AmpEnvGraphView.h"
#include <QQuickPaintedItem>

class AmpEnvGraphView;

class AmpEnvParams : public QObject {
Q_OBJECT

    Q_PROPERTY(QPointF myCoords READ myCoords WRITE setMyCoords NOTIFY myCoordsChanged)

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

    QString text = "None";
    QPointF myCoords() const;

    void setMyCoords(QPointF update_point);

signals:
    void myCoordsChanged();

private:




    AmpEnvGraphView *GraphView_;

};


#endif //AMPENVPARAMS_H
