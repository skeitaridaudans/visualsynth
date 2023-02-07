//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_BOXVIEW_H
#define QTQUICKTEST_BOXVIEW_H


#include <QtCore>
#include <QQuickWindow>
#include <QQuickFramebufferObject>

class BoxView : public QQuickFramebufferObject {
    Q_OBJECT
public:
    QQuickFramebufferObject::Renderer *createRenderer() const override;
};


#endif //QTQUICKTEST_BOXVIEW_H
