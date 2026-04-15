#include "pawn.h"

std::vector<std::shared_ptr<Move> > Pawn::calculatePossibleMoves(
    const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board,
    const Coordinate &source,
    std::optional<Move> lastMove)
{
    std::vector<std::shared_ptr<Move>> moves;
    int moveDirection = color_ == Color::WHITE ? 1 : -1;

    // forward move - 1 square
    int rank = source.rank() + moveDirection;
    int file = source.file();
    if (inBounds(rank, file) && !board[rank][file]){

        Coordinate destination {rank, file};
        auto move = std::make_shared<Move>(source, destination, color_);
        move->movingPiece = board[source.rank()][source.file()];

        // promotion
        int promotionRank = color_ == Color::WHITE ? board.size() - 1 : 0;
        move->moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::NORMAL;

        moves.push_back(std::move(move));
    }

    // forward move - 2 squares
    if (!hasMoved_ && inBounds(rank, file) && !board[rank][file]) {
        rank += moveDirection;
        Coordinate destination {rank, file};
        auto move = std::make_shared<Move>(source, destination, color_);
        move->movingPiece = board[source.rank()][source.file()];
        move->moveType = MoveType::PAWN_DOUBLE_NORMAL;
        moves.push_back(std::move(move));
    }

    // capture
    rank = source.rank() + moveDirection;
    for (int f : {-1, 1}) {
        file = source.file() + f;
        if (inBounds(rank, file)){
            Coordinate destination {rank, file};
            auto move = std::make_shared<Move>(source, destination, color_);
            move->movingPiece = board[source.rank()][source.file()];

            // en passant
            // is captured piece next to moving piece

            if (lastMove){
                int df = lastMove.value().destination.file() - source.file();
                bool next = lastMove.value().destination.rank() == source.rank() && (df == 1 || df == -1);
                // check if the destination of the last move matches the expected captured pawn position
                bool isCapturableTarget = lastMove->destination.file() == destination.file();
                if (!board[rank][file] && lastMove->moveType == MoveType::PAWN_DOUBLE_NORMAL && next && isCapturableTarget) {
                    move->moveType = MoveType::ENPASSANT;
                    move->capturedPiece = board[source.rank()][file];
                    moves.push_back(std::move(move));
                }
            }
            else if (isEnemy(board, destination)) {
                move->moveType = MoveType::CAPTURE;
                move->capturedPiece = board[rank][file];
                moves.push_back(std::move(move));
            }
        }
    }
    return moves;
}
