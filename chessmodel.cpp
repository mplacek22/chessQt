#include "chessmodel.h"

ChessModel::ChessModel(QObject *parent) : QObject(parent) {}

bool ChessModel::whiteTurn() const {
    return m_whiteTurn;
}

int ChessModel::col(int piece) { return piece % 8; }
int ChessModel::row(int piece) { return piece / 8; }

bool ChessModel::canDrop(int col, int row) {
    return col >= 0 && col < 8 && row >= 0 && row < 8;
}

void ChessModel::setActivePiece(int piece) {
}

void ChessModel::release(int piece, int col, int row) {
    emit validMove(col, row);
}