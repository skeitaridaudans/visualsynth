#include "main.h"

class FbItem;

class FbItemRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
    FbItem *fbItem;
public:
    FbItemRenderer(FbItem *fbItem) : fbItem(fbItem) {}
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size)
    {
        initializeOpenGLFunctions();

        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        // optionally enable multisampling by doing format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }
    void drawLine(QVector2D from, QVector2D to)
    {
        glPushMatrix();
        glColor3d(0.0, 0.0, 0.7);
        glTranslated(0.0, 0.0, 0.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2d(from.x(), from.y());
        glVertex2d(from.x(), from.y() + 0.05);
        glVertex2d(to.x(), to.y());
        glVertex2d(to.x(), to.y() + 0.05);
        glEnd();
        glPopMatrix();
    }

    void render()
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        for (float x = -1.0; x < 1.0; x += 0.001) {
            drawLine(QVector2D(x, qSin(sin_start_x + x * fbItem->frequency()) * fbItem->height()), QVector2D(x + 0.001, qSin(sin_start_x + (x + 0.001) * fbItem->frequency()) * fbItem->height()));
        }

        update();

        sin_start_x += 0.1;
    }
protected:
    QOpenGLContext *m_context;
private:
    float sin_start_x = 0.0;
};

QQuickFramebufferObject::Renderer *FbItem::createRenderer() const
{
    window()->setPersistentGraphics(true);
    return new FbItemRenderer(const_cast<FbItem *>(this));
}
qreal FbItem::frequency() {
    return frequency_;
}

void FbItem::setFrequency(qreal frequency)  {
    frequency_ = frequency;
}

void FbItem::setHeight(qreal height) {
    height_ = height;
}

qreal FbItem::height() {
    return height_;
}

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);
    qmlRegisterType<FbItem>("fbitem", 1, 0, "FbItem");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/QtQuickTest/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
