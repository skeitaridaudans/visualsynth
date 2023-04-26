#include "main.h"
#include "src/OperatorView/OperatorView.h"
#include "src/AmpEnvelope/AmpGraph/AmpEnvGraphView.h"

#include <QQmlContext>
#include "Controller/Controller.h"
#include "src/Alert/AlertController.h"
#include "src/Dialog/DialogController.h"
#include "src/OperatorPresetsView/OperatorPresetsView.h"
#include "src/OutputWaveView/OutputWaveView.h"
#include "src/OperatorWaveView/OperatorWaveView.h"

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QGuiApplication app(argc, argv);
    qmlRegisterType<OperatorView>("OperatorView", 1, 0, "OperatorView");
    qmlRegisterType<AmpEnvGraphView>("AmpEnvGraphView",1,0,"AmpEnvGraphItem");
    qmlRegisterType<OperatorPresetsView>("OperatorPresetsView", 1, 0, "OperatorPresetsView");
    qmlRegisterType<OutputWaveView>("OutputWaveView", 1, 0, "OutputWaveView");
    qmlRegisterType<OperatorWaveView>("OperatorWaveView", 1, 0, "OperatorWaveView");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/VisualSynth/src/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("alertController", AlertController::instance.get());
    engine.rootContext()->setContextProperty("dialogController", DialogController::instance.get());
    engine.rootContext()->setContextProperty("controller", Controller::instance.get());
    engine.load(url);

    return app.exec();
}
