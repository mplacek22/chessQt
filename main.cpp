#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Game.h"
#include "game_mediator.h"
#include "gamecontroller.h"
#include "pieceUtils.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<PieceUtils>(
        "Chess", 1, 0, "PieceUtils",
        [](QQmlEngine* e, QJSEngine* j) -> QObject* {
            return PieceUtils::create(e, j);
        }
        );

    Game game;
    GameController controller;
    GameMediator mediator(&game, &controller);

    engine.rootContext()->setContextProperty("gameController", &controller);

    engine.loadFromModule("Chess", "Main");
    return app.exec();
}
