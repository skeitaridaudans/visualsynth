#include "main.h"
#include "src/SinWaveView/SinWaveItem.h"
#include "src/BoxView/BoxView.h"
#include <QQmlContext>
#include "Controller/Controller.h"

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);
    qmlRegisterType<SinWaveItem>("SinViewItem", 1, 0, "SinWaveItem");
    qmlRegisterType<BoxView>("BoxView", 1, 0, "BoxView");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/QtQuickTest/src/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    engine.rootContext()->setContextProperty("controller", Controller::instance.get());

    return app.exec();
}
