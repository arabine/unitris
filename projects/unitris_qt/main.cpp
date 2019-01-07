#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWindow>

#include "UnitrisWrapper.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);

    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("D8S");
    QCoreApplication::setOrganizationDomain("d8s.eu");
    QCoreApplication::setApplicationName("Unitris - Qt version");

    UnitrisWrapper unitris;
    QQmlApplicationEngine engine;

    QQmlContext * ctx = engine.rootContext();
    ctx->setContextProperty("unitris", &unitris);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
