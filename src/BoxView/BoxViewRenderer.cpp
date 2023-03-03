//
// Created by Guðmundur on 2/6/2023.
//

#include <QOpenGLFramebufferObjectFormat>
#include "BoxViewRenderer.h"
#include "src/Controller/Controller.h"

BoxViewRenderer::BoxViewRenderer(BoxView *boxView) : boxView(boxView),
                                                     newBox_(std::make_unique<NewBox>(boxView,
                                                                                      const_cast<BoxViewRenderer *>(this))),
                                                     operatorDrawer_(std::make_unique<OperatorDrawer>(boxView)) {
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
    const auto& controller = Controller::instance;

    for (const auto& operator_: controller->operators()) {
        operatorDrawer_->update(operator_.second.get());
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    newBox_->draw();
    for (const auto& operator_: controller->operators()) {
        operatorDrawer_->draw(operator_.second.get());
    }

    update();
}

void BoxViewRenderer::addOperator(double x, double y) {
    const auto& controller = Controller::instance;

    // TODO: Remove this when selecting modulator has been implemented
    // If there is another operator, make it a modulator of the new operator
    /*std::optional<int> modulatedBy = std::nullopt;
    if (!controller->operators().empty()) {
        const auto& first = *controller->operators().begin();
        modulatedBy = std::make_optional<int>(first.first);
    }*/

    auto id = controller->addOperator();
    const auto& operator_ = controller->getOperatorById(id);

    /*if (modulatedBy.has_value()) {
        operator_->modulatedBy.push_back(modulatedBy.value());
    }*/

    operator_->position = QPointF(x, y);
}
