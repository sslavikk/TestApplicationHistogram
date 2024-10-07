#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <uimanager.h>
#include <QQmlContext>
#include <qmlplot.h>
#include "defines.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    UiManager manager;
    QQmlApplicationEngine engine;
    qRegisterMetaType<QVector<DataGraph>>("QVector<DataGraph>");
    qmlRegisterType<CustomPlotItem>("CustomPlot", 1, 0, "CustomPlotItem");
    engine.rootContext()->setContextProperty(QStringLiteral("UiManager"), &manager);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
