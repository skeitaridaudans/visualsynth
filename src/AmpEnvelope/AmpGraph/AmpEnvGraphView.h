#ifndef AMPENVGRAPHVIEW_H
#define AMPENVGRAPHVIEW_H

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <QQuickPaintedItem>
#include <memory> // Added for std::unique_ptr

#include "src/AmpEnvelope/AmpGraph/AmpEnvParams/AmpEnvParams.h"
#include "src/AmpEnvelope/AmpGraph/LineBetweenParams/Line.h"

class AmpEnvParams;
class Line;

class AmpEnvGraphView : public QQuickPaintedItem {
Q_OBJECT
    Q_PROPERTY(double W READ W WRITE setW NOTIFY WChanged)
    Q_PROPERTY(double H READ H WRITE setH NOTIFY HChanged)
    Q_PROPERTY(double bW READ bW WRITE setbW NOTIFY bWChanged)

    Q_PROPERTY(QPointF attack READ attack WRITE setAttack NOTIFY attackChanged)
    Q_PROPERTY(QPointF decay READ decay WRITE setDecay NOTIFY decayChanged)
    Q_PROPERTY(QPointF sustain READ sustain WRITE setSustain NOTIFY sustainChanged)
    Q_PROPERTY(QPointF release READ release WRITE setRelease NOTIFY releaseChanged)


public:
    explicit AmpEnvGraphView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    double W() const;
    void setW(double val);

    double H() const;
    void setH(double val);

    double bW() const;
    void setbW(double val);

    QPointF attack();
    void setAttack(QPointF);

    QPointF decay();
    void setDecay(QPointF);

    QPointF sustain();
    void setSustain(QPointF);

    QPointF release();
    void setRelease(QPointF);

    std::unique_ptr<AmpEnvParams> attack_ = std::make_unique<AmpEnvParams>(this);
    std::unique_ptr<AmpEnvParams> decay_ = std::make_unique<AmpEnvParams>(this);
    std::unique_ptr<AmpEnvParams> sustain_ = std::make_unique<AmpEnvParams>(this);
    std::unique_ptr<AmpEnvParams> release_ = std::make_unique<AmpEnvParams>(this);

    std::unique_ptr<Line> start_to_attack_ = std::make_unique<Line>(this);
    std::unique_ptr<Line> attack_to_decay_ = std::make_unique<Line>(this);
    std::unique_ptr<Line> decay_to_sustain_ = std::make_unique<Line>(this);
    std::unique_ptr<Line> sustain_to_release_ = std::make_unique<Line>(this);

    double x_ = 450;
    double y_ = 340;

    double height_ = 316;
    double width_ = 628;

    QPointF center_ = QPointF(width_/2, height_/2);
    double borderwidth_ = 5;

signals:
    void WChanged();
    void HChanged();
    void bWChanged();

    void attackChanged();
    void decayChanged();
    void sustainChanged();
    void releaseChanged();


private:
    void paintGraphContainer(QPainter *painter);
    void paintParams(QPainter *painter);
    void paintLines(QPainter *painter);

    void initializeParams(std::unique_ptr<AmpEnvParams> param, double x, double y, int r, int g, int b, QString text);


    QPointF topRight_ = QPointF(this->x_+this->width_, this->y_+this->height_);
    QPointF topLeft_ = QPointF(this->x_, this->y_+this->height_);

    QPointF bottomRight_ = QPointF(this->x_+this->width_, this->y_);
    QPointF bottomLeft_ = QPointF(this->x_, this->y_);

};

#endif // AMPENVGRAPHVIEW_H