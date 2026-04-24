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
    };

}