//
// Created by Guðmundur on 4/7/2023.
//

#ifndef MAIN_QML_DIALOGCONTROLLER_H
#define MAIN_QML_DIALOGCONTROLLER_H

#include <QObject>

class DialogController : public QObject {
Q_OBJECT
public:
    static std::unique_ptr<DialogController> instance;

    explicit DialogController(QObject *parent = 0);

    void showDialog(const QString &title, const QString &text, const QString &defaultValue = "",
                    const QString &placeholderText = "", const QString &confirmButtonText = "Ok",
                    const QString &cancelButtonText = "Cancel", std::function<bool(const QString& value)> onSubmit = nullptr);

    Q_INVOKABLE void submit();

    Q_INVOKABLE void cancel();

    Q_PROPERTY(QString title MEMBER title_ NOTIFY titleChanged)
    Q_PROPERTY(QString text MEMBER text_ NOTIFY textChanged)
    Q_PROPERTY(QString placeholderText MEMBER placeholderText_ NOTIFY placeholderTextChanged)
    Q_PROPERTY(QString value MEMBER value_ NOTIFY valueChanged)
    Q_PROPERTY(QString confirmButtonText MEMBER confirmButtonText_ NOTIFY confirmButtonTextChanged)
    Q_PROPERTY(QString cancelButtonText MEMBER cancelButtonText_ NOTIFY cancelButtonTextChanged)
    Q_PROPERTY(bool isVisible MEMBER isVisible_ NOTIFY isVisibleChanged)

signals:
    Q_SIGNAL void titleChanged(QString title);

    Q_SIGNAL void textChanged(QString text);

    Q_SIGNAL void placeholderTextChanged(QString placeholderName);

    Q_SIGNAL void valueChanged(QString value);

    Q_SIGNAL void confirmButtonTextChanged(QString acceptButtonText);

    Q_SIGNAL void cancelButtonTextChanged(QString cancelButtonText);

    Q_SIGNAL void isVisibleChanged(bool isVisible);

private:
    QString title_;
    QString text_;
    QString placeholderText_;
    QString value_;
    QString confirmButtonText_;
    QString cancelButtonText_;
    bool isVisible_ = false;
    std::function<bool(const QString& value)> onSubmit_;
};


#endif //MAIN_QML_DIALOGCONTROLLER_H
