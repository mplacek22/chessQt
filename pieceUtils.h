#pragma once
#include <QObject>
#include <QtQml/qqml.h>
#include "Color.h"
#include "PieceType.h"

class PieceUtils : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

public:
    Q_INVOKABLE static QString imageSource(int color, int type)
    {
        return imageSource(static_cast<Color>(color), static_cast<PieceType>(type));
    }

    static QString imageSource(Color color, PieceType type) {
        return QString("pieces/%1_%2.svg").arg(colorToString(color), pieceTypeToString(type));
    }


private:
    static QString colorToString(Color c) {
        return c == Color::WHITE ? "w" : "b";
    }
    static QString pieceTypeToString(PieceType type) {
        switch (type) {
            case PieceType::KING: return "king";
            case PieceType::QUEEN: return "queen";
            case PieceType::ROOK: return "rook";
            case PieceType::BISHOP: return "bishop";
            case PieceType::KNIGHT: return "knight";
            case PieceType::PAWN: return "pawn";
            default: return "";
        }
    }
};