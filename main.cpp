#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "BoardModel.h"
#include "gamecontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    QQmlApplicationEngine engine;

    GameController controller;

    engine.rootContext()->setContextProperty("gameController", &controller);

    engine.loadFromModule("Chess", "Main");
    return app.exec();
}
