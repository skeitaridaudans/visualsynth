//
// Created by Sigurður on 4/25/2023.
//
#include <iostream>
#include "PresetButton.h"
#include "QPainter"
#include "src/Controller/Controller.h"


const QString iconOn_ (fa::fa_folder_open);
const QString iconOff_ (fa::fa_folder_closed);

PresetButton::PresetButton(QQuickItem *parent) : QQuickPaintedItem(parent){
    setAcceptedMouseButtons(Qt::LeftButton);
    setAntialiasing(true);
    icon_ = iconOff_;
}



QString PresetButton::title() const{

    return text_ + " " + icon_;
}

void PresetButton::setTitle(QString newTitle) {
    text_ = newTitle;
    emit titleChanged();
}


void PresetButton::paint(QPainter *painter) {

    painter->setRenderHint(QPainter::Antialiasing);

    //Set background color of button
    painter->setBrush(pressed ? color_ : color_);

    //set line around button
    painter->setPen(QPen(borderColor_, 5));

    painter->drawRect(boundingRect());


    //set text and icon coords
    QPointF text_location = QPointF(width_*3/4,height_*5/4);
    QPointF icon_location = QPointF(text_location.x()+(text_.length()*11),text_location.y());

    // Draw text
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial",15));
    painter->drawText(text_location, text_);


    //Draw icon
    painter->setPen(Qt::white);
    painter->setFont(fontAwesome()->font(fa::fa_regular, 21));
    painter->drawText(icon_location, icon_);
}



void PresetButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = true;
        update();
    }
}

void PresetButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;
        update();


        // Emit the clicked signal
        emit clicked();
    }
}

void PresetButton::updateOpen() {
    open_ = Controller::instance->showPresets();
    if (open_) { //if the button displays all available presets then display a certain icon
        icon_ = iconOn_;
        color_ = colorOpen_;
    }else {
        icon_ = iconOff_;
        color_ = colorClosed_;
    }
    update();
}

double PresetButton::width() {
    return width_;
}

double PresetButton::height() {
    return height_;
}

QColor PresetButton::color() {
    return color_;
}
