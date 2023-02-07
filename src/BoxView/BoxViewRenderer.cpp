//
// Created by Guðmundur on 2/6/2023.
//

#include <QOpenGLFramebufferObjectFormat>
#include "BoxViewRenderer.h"

BoxViewRenderer::BoxViewRenderer(BoxView *boxView) : boxView(boxView), newBox_(std::make_unique<NewBox>(boxView, const_cast<BoxViewRenderer *>(this))) {
}

QOpenGLFramebufferObject *BoxViewRenderer::createFramebufferObject(const QSize &size) {
    initializeOpenGLFunctions();

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    // optionally enable multisampling by doing format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

void BoxViewRenderer::render() {
    newBox_->update();
    for (const auto& box : boxes_) {
        box->update();
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    newBox_->draw();
    for (const auto& box : boxes_) {
        box->draw();
    }

    update();
}

void BoxViewRenderer::addBox(double x, double y) {
    boxes_.push_back(std::make_unique<Box>(boxView, x, y));
}
