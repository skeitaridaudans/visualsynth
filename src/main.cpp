#include "main.h"
#include "src/Views/OperatorView/OperatorView.h"
#include "src/Views/AmpEnvelope/AmpGraph/AmpEnvGraphView.h"

#include <QQmlContext>

#ifdef ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif

#include "src/Controllers/Controller/Controller.h"
#include "src/Controllers/Alert/AlertController.h"
#include "src/Controllers/Dialog/DialogController.h"
#include "src/Views/OperatorPresetsView/OperatorPresetsView.h"
#include "src/Views/OutputWaveView/OutputWaveView.h"
#include "src/Views/OperatorView/PresetButton.h"
#include "src/Views/OperatorWaveView/OperatorWaveView.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<OperatorView>("OperatorView", 1, 0, "OperatorView");
    qmlRegisterType<PresetButton>("PButton", 1,0, "PresetButton");

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

#ifdef ANDROID
    // Ask for storage permission on android so that presets work
    const auto res = QtAndroidPrivate::checkPermission(QtAndroidPrivate::Storage).result();
    if (res == QtAndroidPrivate::Denied) {
        const auto res = QtAndroidPrivate::requestPermission(QtAndroidPrivate::Storage).result();
    }
#endif

    return app.exec();
}
