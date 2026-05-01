#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "BoardModel.h"
#include "gamecontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    // Register types so QML knows about them
    // Must register BEFORE engine is created
    qmlRegisterType<BoardModel>("Chess", 1, 0, "BoardModel");

    qmlRegisterType<GameController>("Chess", 1, 0, "GameController");

    QQmlApplicationEngine engine;

    // Game* game = new Game();

    // ChessBoard* chess = new ChessBoard(&app);
    // engine.rootContext()->setContextProperty("game", game);

    GameController controller;
    engine.rootContext()->setContextProperty("gameController", &controller);


    engine.loadFromModule("Chess", "Main");
    return app.exec();
}
