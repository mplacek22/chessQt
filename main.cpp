#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "chessboard.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ChessBoard* chess = new ChessBoard(&app);
    engine.rootContext()->setContextProperty("chessController", chess);

    engine.loadFromModule("Chess", "Main");
    return app.exec();
}
