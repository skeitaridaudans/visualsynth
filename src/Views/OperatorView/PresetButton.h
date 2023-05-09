//
// Created by Sigurður on 4/25/2023.
//

#ifndef MAIN_QML_PRESETBUTTON_H
#define MAIN_QML_PRESETBUTTON_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QQuickPaintedItem>
#include "src/Utils/FontAwesome.h"

class PresetButton : public QQuickPaintedItem{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

    Q_PROPERTY(double width READ width )
    Q_PROPERTY(double height READ height)
    Q_PROPERTY(QColor color READ color)

public:
    PresetButton(QQuickItem *parent = nullptr);

    QString title() const;
    void setTitle(const QString newTitle);
    Q_INVOKABLE void updateOpen();


    void paint(QPainter *painter);

    double width();
    double height();

    QColor color();

    signals:
    void clicked();
    void titleChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    QString icon_;

    bool pressed_ = false ;
    bool open_ = false;
    QString text_ = "Presets";

    double width_ = 100;
    double height_ = 25;


    QColor colorOpen_ = QColor(84,44,83);
    QColor colorClosed_ = QColor(Qt::transparent);

    QColor color_  = colorClosed_;

    QColor borderColor_ = QColor(Qt::darkGray);


};


#endif //MAIN_QML_PRESETBUTTON_H
