#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "MediaAppMain.h"
#include "MyThread.h"
#include "../common/header/message/HmiData.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    MediaAppMain MediaAppMainCore;    // Create the application core with signals and slots

    MediaAppMain* pMediaAppMain = new MediaAppMain();
    pMediaAppMain->AppInit();
    qmlRegisterType<MediaAppMain>("com.MediaAppMain", 1, 0, "MediaAppMain");
    //qmlRegisterType<MY_IPC>("com.MY_IPC", 1, 0, "MY_IPC");

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("MediaAppMainCore", &MediaAppMainCore);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    MyThread* mThread = new MyThread();
   // zmThread->run();
//    mThread->getApp(pMediaAppMain);
    mThread->getApp(&MediaAppMainCore);

    mThread->start(QThread::IdlePriority);
//mThread->wait();
    return app.exec();
}
