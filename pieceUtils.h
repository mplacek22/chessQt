#pragma once
#include <QObject>
#include <QtQml/qqml.h>
#include "Color.h"
#include "PieceType.h"
#include "drawCause.h"
#include "gameStatus.h"

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

    static QString gameStatusToString(GameStatus gameStatus) {
        switch (gameStatus) {
        case GameStatus::IN_PROGRESS: return "in_progress";
        case GameStatus::SINGLE_CHECK: return "check";
        case GameStatus::DOUBLE_CHECK: return "double_check";
        case GameStatus::DRAW: return "draw";
        case GameStatus::NEW: return "new";
        default: return "";
        }
    }

    Q_INVOKABLE static QString drawCauseToString(DrawCause drawCause)
    {
        switch (drawCause) {
            case DrawCause::STALEMATE: return "Stalemate";
            case DrawCause::AGREED: return "Agreed";
            case DrawCause::FIFTY_MOVE_RULE: return "50 moves rule";
            case DrawCause::THREEFOLD_REPETITION: return "threefold-repetition";
            case DrawCause::INSUFFICIENT_MATERIAL: return "insufficient material";
            default: return "";
        }
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