//
// Created by Guðmundur on 3/12/2023.
//

#ifndef MAIN_QML_ALERTCONTROLLER_H
#define MAIN_QML_ALERTCONTROLLER_H

#include <QObject>
#include <QColor>

const QColor kErrorColor (0xf44336);
const QColor kSuccessColor (0x66cc66);

class AlertController : public QObject {
    Q_OBJECT
public:
    static std::unique_ptr<AlertController> instance;
    AlertController (QObject* parent = 0);

    Q_INVOKABLE void showAlert(const QString& text, bool isError);
    Q_INVOKABLE void update();

    Q_PROPERTY(QString alertText READ alertText NOTIFY alertTextChanged)
    Q_PROPERTY(QString alertVisibleState READ alertVisibleState NOTIFY alertVisibleStateChanged)
    Q_PROPERTY(QColor alertColor READ alertColor NOTIFY alertColorChanged)

    QColor alertColor();
    QString alertVisibleState();
    QString alertText();
signals:

    void alertColorChanged(QColor value);
    void alertVisibleStateChanged(QString value);
    void alertTextChanged(QString value);
private:
    const QString kVisibleState = "visible";
    const QString kInvisibleState = "invisible";


    void updateAlertColor(bool failed);

    void setAlertVisibleState(const QString& state);
    void setAlertText(const QString& text);
    void setAlertColor(const QColor& color);

    QColor alertColor_ = kErrorColor;

    QString alertVisibleState_ = kInvisibleState;
    QString alertText_;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> alertShownTime_ = std::nullopt;
};


#endif //MAIN_QML_ALERTCONTROLLER_H
