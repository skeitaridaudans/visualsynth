//
// Created by Sigurður on 3/3/2023.
//

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <QQuickItem>
#include "AmpEnvGraphView.h"

AmpEnvGraphView::AmpEnvGraphView(QQuickItem *parent):QQuickPaintedItem(parent), release(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
                                                                                sustain(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
                                                                                attack(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
                                                                                decay(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
                                                                                start_to_a(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this))),
                                                                                a_to_d(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this))),
                                                                                d_to_s(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this))),
                                                                                s_to_r(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this))){

    double widthFromParams = x_/3;

    attack->assignText("A");
    attack->assignRGBColor(255,255,0);
    attack->assignX(widthFromParams*0 +(widthFromParams/2) );
    attack->assignY(center_);

    decay->assignText("D");
    decay->assignRGBColor(255,128,0);
    decay->assignX(widthFromParams*1 +(widthFromParams/2));
    decay->assignY(center_);

    sustain->assignText("S");
    sustain->assignRGBColor(255,229,180);
    sustain->assignX(widthFromParams*2 + (widthFromParams/2));
    sustain->assignY(center_);

    release->assignText("R");
    release->assignRGBColor(128,0,128);
    release->assignX(widthFromParams*3 + (widthFromParams/2));
    release->assignY(center_);


    start_to_a->assignP1(x_-width_,height_+y_);
    start_to_a->assignP2(attack->coords.x(),attack->coords.y());
    start_to_a->setColor(QColor(Qt::red));

    a_to_d->assignP1(attack->coords.x(),attack->coords.y());
    a_to_d->assignP2(decay->coords.x(),decay->coords.y());
    a_to_d->setColor(QColor(Qt::green));


    d_to_s->assignP1(decay->coords.x(),decay->coords.y());
    d_to_s->assignP2(sustain->coords.x(),sustain->coords.y());
    d_to_s->setColor(QColor(Qt::yellow));

    s_to_r->assignP1(sustain->coords.x(),sustain->coords.y());
    s_to_r->assignP2(release->coords.x(),release->coords.y());
    s_to_r->setColor(QColor(Qt::magenta));
}


void AmpEnvGraphView::initializeParams(std::unique_ptr <AmpEnvParams> param,double x,double y,int r,int g,int b,QString text){
    param->assignText(text);
    param->assignRGBColor(r,g,b);
    param->assignX(x);
    param->assignY(y);
}


void AmpEnvGraphView::paintGraphContainer(QPainter *painter){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(Qt::gray), 10));
    QRect rect = QRect(0, 0, this->width_, this->height_);
    painter->drawRect(rect);

}

void AmpEnvGraphView::paintParams(QPainter* painter){
    attack->draw(painter);
    decay->draw(painter);
    sustain->draw(painter);
    release->draw(painter);

}

void AmpEnvGraphView::paintLines(QPainter *painter){
    start_to_a->draw(painter);
    a_to_d->draw(painter);
    d_to_s->draw(painter);
    s_to_r->draw(painter);

}


void AmpEnvGraphView::paint(QPainter *painter) {

    painter->setRenderHint(QPainter::Antialiasing);

    //Drawing Rectangle
    this->paintLines(painter);
    this->paintGraphContainer(painter);
    this->paintParams(painter);

    update();
}