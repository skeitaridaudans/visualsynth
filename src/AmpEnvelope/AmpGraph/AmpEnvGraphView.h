//
// Created by Sigurður on 3/3/2023.
//

#ifndef AMPENVGRAPHVIEW_H
#define AMPENVGRAPHVIEW_H

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <QQuickPaintedItem>
#include "src/AmpEnvelope/AmpGraph/AmpEnvParams/AmpEnvParams.h"
#include "src/AmpEnvelope/AmpGraph/LineBetweenParams/Line.h"

class AmpEnvParams;
class Line;

class AmpEnvGraphView : public QQuickPaintedItem {
    Q_OBJECT

public:
    explicit AmpEnvGraphView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

private:
    void paintGraphContainer(QPainter *painter);
    void paintParams(QPainter *painter);
    void paintLines(QPainter *painter);

    void initializeParams(std::unique_ptr <AmpEnvParams> param,double x,double y,int r,int g,int b,QString text);

    double x_ = 450;
    double y_ = 340;
    double width_ = 628;
    double height_ = 316;
    double center_ = 140;

    QPointF topRight_ = QPointF(this->x_+this->width_,this->y_+this->height_);
    QPointF topLeft_ = QPointF(this->x_,this->y_+this->height_);

    QPointF bottomRight_ = QPointF(this->x_+this->width_,this->y_);
    QPointF bottomLeft_ = QPointF(this->x_,this->y_);


    std::unique_ptr <AmpEnvParams> attack;
    std::unique_ptr <AmpEnvParams> decay;
    std::unique_ptr <AmpEnvParams>sustain;
    std::unique_ptr <AmpEnvParams> release;

    std::unique_ptr <Line> start_to_a;
    std::unique_ptr <Line> a_to_d;
    std::unique_ptr <Line> d_to_s;
    std::unique_ptr <Line> s_to_r;




};

#endif //AMPENVGRAPHVIEW_H