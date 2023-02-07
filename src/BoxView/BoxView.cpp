//
// Created by Guðmundur on 2/6/2023.
//

#include "BoxView.h"
#include "BoxViewRenderer.h"

QQuickFramebufferObject::Renderer *BoxView::createRenderer() const {
    window()->setPersistentGraphics(true);
    return new BoxViewRenderer(const_cast<BoxView *>(this));
}
