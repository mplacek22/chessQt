#pragma once
#include <QObject>
#include <QtQml/qqml.h>

namespace Chess {

    class Enums : public QObject
    {
        Q_OBJECT
        QML_SINGLETON
        QML_ELEMENT

    public:
        enum class PieceType {
            BISHOP = 0,
            KING = 1,
            KNIGHT = 2,
            PAWN = 3,
            QUEEN = 4,
            ROOK = 5
        };
        Q_ENUM(PieceType)

        enum class Color {
            WHITE = 0,
            BLACK = 1
        };
        Q_ENUM(Color)

        enum class GameStatus {
            NEW,
            IN_PROGRESS,
            SINGLE_CHECK,
            DOUBLE_CHECK,
            CHECK_MATE,
            STALE_MATE,
            DRAW,
        };
        Q_ENUM(GameStatus)

        enum DrawCause {
            STALEMATE,
            INSUFFICIENT_MATERIAL,
            FIFTY_MOVE_RULE,
            THREEFOLD_REPETITION,
            FIVEFOLD_REPETITION,
            AGREED
        };
        Q_ENUM(DrawCause)


    };

}