#include "king.h"
#include "Move.h"

std::vector<std::shared_ptr<Move> > King::calculatePossibleMoves(
    const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board,
    const Coordinate& source,
    std::optional<Move> lastMove)
{
    auto moves = Piece::calculatePossibleMoves(board, source, lastMove);;
    auto dangerSquares = computeKingDangerSquares(board, source);

    // Filter out moves to king danger squares
    std::erase_if(moves, [&](const auto& m) {
        return std::ranges::find(dangerSquares, m->destination) != dangerSquares.end();
    });

    return moves;
}

std::vector<Coordinate> King::computeKingDangerSquares(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board, const Coordinate &kingPosition) const
{
    std::array<std::array<std::shared_ptr<Piece>, 8>, 8> boardWithoutKing = board;
    boardWithoutKing[kingPosition.rank()][kingPosition.file()] = nullptr;

    std::vector<Coordinate> danger;
    for (int r = 0; r < board.size(); r++) {
        for (int f = 0; f < board.size(); f++) {
            const auto piece = boardWithoutKing[r][f];
            if (piece && piece->color() != color_) {
                for (const auto&  m : piece->calculatePseudoLegalMoves(boardWithoutKing, {r, f})) {
                    danger.push_back(m->destination);
                }
            }
        }
    }
    return danger;
}
