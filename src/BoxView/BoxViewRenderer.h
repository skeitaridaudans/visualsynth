//
// Created by Guðmundur on 2/6/2023.
//

#ifndef QTQUICKTEST_BOXVIEWRENDERER_H
#define QTQUICKTEST_BOXVIEWRENDERER_H


#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "BoxView.h"
#include "OperatorDrawer.h"
#include "NewBox.h"

class NewBox;

class BoxViewRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions {
    BoxView *boxView;
public:
    explicit BoxViewRenderer(BoxView *boxView);
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

    void render() override;
    void addOperator(double x, double y);
protected:
    QOpenGLContext *m_context;
private:
    std::unique_ptr<OperatorDrawer> operatorDrawer_;
    std::unique_ptr<NewBox> newBox_;
};


#endif //QTQUICKTEST_BOXVIEWRENDERER_H
