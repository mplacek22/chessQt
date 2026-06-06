#include "pieceUtils.h"

PieceUtils::PieceUtils(QObject *parent) : QObject(parent) {}

PieceUtils *PieceUtils::create(QQmlEngine *, QJSEngine *)
{
    static PieceUtils instance;
    return &instance;
}



QString PieceUtils::imageSource(int color, int type) const
{
    return imageSource(static_cast<Color>(color), static_cast<PieceType>(type));
}



QString PieceUtils::imageSource(Color color, PieceType type) {
    return QString("pieces/%1_%2.svg").arg(colorToString(color), pieceTypeToString(type));
}



QString PieceUtils::gameStatusToString(GameStatus gameStatus) {
    switch (gameStatus) {
    case GameStatus::IN_PROGRESS: return "in_progress";
    case GameStatus::SINGLE_CHECK: return "check";
    case GameStatus::DOUBLE_CHECK: return "double_check";
    case GameStatus::DRAW: return "draw";
    case GameStatus::NEW: return "new";
    default: return "";
    }
}



QString PieceUtils::drawCauseToString(DrawCause drawCause)
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



QString PieceUtils::colorToString(Color c) {
    return c == Color::WHITE ? "w" : "b";
}



QString PieceUtils::pieceTypeToString(PieceType type) {
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

