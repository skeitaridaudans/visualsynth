//
// Created by Sigurður on 3/3/2023.
//

// AmpEnvGraphView.cpp
#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <QQuickItem>
#include <QMouseEvent>
#include "AmpEnvGraphView.h"


AmpEnvGraphView::AmpEnvGraphView(QQuickItem *parent) :
        QQuickPaintedItem(parent),
        release_(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
        sustain_(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
        attack_(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
        decay_(std::make_unique<AmpEnvParams>(const_cast<AmpEnvGraphView *>(this))),
        start_to_attack_(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this))),
        attack_to_decay_(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this))),
        decay_to_sustain_(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this))),
        sustain_to_release_(std::make_unique<Line>(const_cast<AmpEnvGraphView *>(this)))
{
    // Assign variables to attack
    attack_->assignText("");
    attack_->assignRGBColor(128, 0, 128);
    attack_->assignX(minWidth_);
    attack_->assignY(minHeight_);

    // Assign variables to decay
    decay_->assignText("");
    decay_->assignRGBColor(128, 0, 128);
    decay_->assignX(minWidth_);
    decay_->assignY(minHeight_);

    // Assign variables to sustain
    sustain_->assignText("");
    sustain_->assignRGBColor(128, 0, 128);
    sustain_->assignX(minWidth_);
    sustain_->assignY(minHeight_);

    // Assign variables to release
    release_->assignText("");
    release_->assignRGBColor(128, 0, 128);
    release_->assignX(maxWidth_+borderwidth_);
    release_->assignY(maxHeight_+borderwidth_);

    // Create a line between start and attack
    start_to_attack_->assignP1(0, height_);
    start_to_attack_->assignP2(attack_->coords.x(), attack_->coords.y());
    start_to_attack_->setColor(QColor(128,0,128));

    // Create a line between attack and decay
    attack_to_decay_->assignP1(attack_->coords.x(), attack_->coords.y());
    attack_to_decay_->assignP2(decay_->coords.x(), decay_->coords.y());
    attack_to_decay_->setColor(QColor(128,0,128));

    // Create a line between decay and sustain
    decay_to_sustain_->assignP1(decay_->coords.x(), decay_->coords.y());
    decay_to_sustain_->assignP2(sustain_->coords.x(), sustain_->coords.y());
    decay_to_sustain_->setColor(QColor(128,0,128));

    // Create a line between sustain and release
    sustain_to_release_->assignP1(sustain_->coords.x(), sustain_->coords.y());
    sustain_to_release_->assignP2(release_->coords.x(), release_->coords.y());
    sustain_to_release_->setColor(QColor(128,0,128));
}

void AmpEnvGraphView::paintGraphContainer(QPainter *painter){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(borderColor, 3));
    QRect rect = QRect(0, 0, this->width_, this->height_);
    painter->drawRect(rect);

}

void AmpEnvGraphView::paintParams(QPainter* painter){
    attack_->draw(painter);
    decay_->draw(painter);
    sustain_->draw(painter);
    release_->draw(painter);

}

void AmpEnvGraphView::paintLines(QPainter *painter){
    start_to_attack_->draw(painter);
    attack_to_decay_->draw(painter);
    decay_to_sustain_->draw(painter);
    sustain_to_release_->draw(painter);

}


void AmpEnvGraphView::paint(QPainter *painter) {

    painter->setRenderHint(QPainter::Antialiasing);

    //Drawing Rectangle
    this->paintLines(painter);
    this->paintGraphContainer(painter);
    this->paintParams(painter);

    update();
}

double AmpEnvGraphView::W()const
{
    return this->width_;
}

void AmpEnvGraphView::setW(double val)
{
    if (width_ != val) {
        width_ = val;
        emit WChanged();
    }
}

double AmpEnvGraphView::H() const
{
    return this->height_;
}

void AmpEnvGraphView::setH(double val)
{
    if (height_ != val) {
        height_ = val;
        emit HChanged();
    }
}

double AmpEnvGraphView::bW() const
{
    return this->borderwidth_;
}

void AmpEnvGraphView::setbW(double val)
{
    if (borderwidth_ != val) {
        borderwidth_ = val;
        emit bWChanged();
    }
}

QPointF AmpEnvGraphView::attack() {
    return attack_->coords;
}

void  AmpEnvGraphView::setAttack(QPointF new_point){

    attack_->coords.setX(new_point.x() + attack_->x_);
    start_to_attack_->assignP2(attack_->coords.x(),attack_->coords.y());
    attack_to_decay_->assignP1(attack_->coords.x(),attack_->coords.y());

    emit attackChanged();


}

QPointF AmpEnvGraphView::decay() {
    return decay_->coords;
}

void  AmpEnvGraphView::setDecay(QPointF new_point){

    decay_->coords.setX(new_point.x() + decay_->x_);
    attack_to_decay_->assignP2(decay_->coords.x(),decay_->coords.y());
    decay_to_sustain_->assignP1(decay_->coords.x(),decay_->coords.y());

    emit decayChanged();


}

QPointF AmpEnvGraphView::sustain() {
    return sustain_->coords;
}

void  AmpEnvGraphView::setSustain(QPointF new_point){

    sustain_->coords.setY(new_point.y() + sustain_->y_);
    decay_->coords.setY(new_point.y()+sustain_->y_);

    decay_to_sustain_->assignP2(sustain_->coords.x(),sustain_->coords.y());
    decay_to_sustain_->assignP1(decay_->coords.x(),sustain_->coords.y());
    attack_to_decay_->assignP2(decay_->coords.x(),decay_->coords.y());

    sustain_to_release_->assignP1(sustain_->coords.x(),sustain_->coords.y());

    emit sustainChanged();


}

QPointF AmpEnvGraphView::release() {
    return sustain_->coords;
}

void  AmpEnvGraphView::setRelease(QPointF new_point){

    sustain_->coords.setX(new_point.x() + sustain_->x_);
    decay_to_sustain_->assignP2(sustain_->coords.x(),sustain_->coords.y());
    sustain_to_release_->assignP1(sustain_->coords.x(),sustain_->coords.y());

    emit releaseChanged();

}

QColor AmpEnvGraphView::bColor() {
    return borderColor;
}

void AmpEnvGraphView::setbColor(QColor c) {
    borderColor = c;

    emit bColorChanged();
}



double AmpEnvGraphView::graphMinW() const{
    return this->minWidth_;
};


void AmpEnvGraphView::setGraphMinW(double val){
    this->minWidth_ = val;
    emit graphMinWChanged();
};


double AmpEnvGraphView::graphMaxW() const{
    return this->maxWidth_;
};

void AmpEnvGraphView::setGraphMaxW(double val){
    this->maxWidth_ = val;
    emit graphMaxWChanged();
}

double AmpEnvGraphView::graphMinH() const{
    return this->minHeight_;
}

void AmpEnvGraphView::setGraphMinH(double val){
    this->minHeight_ = val;
    emit graphMinHChanged();
}


double AmpEnvGraphView::graphMaxH() const{
    return this->maxHeight_;
}

void AmpEnvGraphView::setGraphMaxH(double val){

    this->maxHeight_ = val;
    emit graphMaxHChanged();
}
