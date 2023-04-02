//
// Created by Sigurður on 3/25/2023.
//

#include "AmpEnvParams.h"
#include "QPainter"
#include "QRect"

AmpEnvParams::AmpEnvParams(AmpEnvGraphView *GraphView): GraphView_(GraphView){}


void AmpEnvParams::assignHeight(int new_height){
    this->height_ = new_height;
}

void AmpEnvParams::assignWidth(int new_width){
    this->width_ = new_width;
}

int AmpEnvParams::getHeight() {
    return this->height_;
}

int AmpEnvParams::getWidth() {
    return this->width_;
}


void AmpEnvParams:: assignX(int newX){
    this->coords.setX(newX+(width_/2));
    this->x_ = newX;
}

void AmpEnvParams:: assignY(int newY){
    this->coords.setY(newY+(height_/2));
    this->y_ = newY;
}

void AmpEnvParams::assignRGBColor(int r, int g, int b){
    this->rgb_r = r;
    this->rgb_g = g;
    this->rgb_b = b;
}

void AmpEnvParams::assignText(QString newTxt){
    this->text = newTxt;
}

void AmpEnvParams::draw(QPainter *painter) {

//    qDebug() << "im drawing... ";

    //qDebug() << "b in rgb: " << rgb_b <<"";

    QRect rect = QRect(x_,y_, width_, height_);

    QBrush brush(QColor(this->rgb_r,this->rgb_g,this->rgb_b));
    painter->setBrush(brush);

    painter->setPen(Qt::PenStyle::SolidLine);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rect);

    painter->setFont(QFont("Arial", 18));
    painter->drawText(rect, Qt::AlignCenter, text);

}
