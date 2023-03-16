#include "main.h"
#include "src/SinWaveView/SinWaveItem.h"
#include "src/OperatorView/OperatorView.h"
#include <QQmlContext>
#include "Controller/Controller.h"
#include "src/Alert/AlertController.h"

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);
    qmlRegisterType<SinWaveItem>("SinViewItem", 1, 0, "SinWaveItem");
    qmlRegisterType<OperatorView>("OperatorView", 1, 0, "OperatorView");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/VisualSynth/src/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("alertController", AlertController::instance.get());
    engine.rootContext()->setContextProperty("controller", Controller::instance.get());
    engine.load(url);

    return app.exec();
}
