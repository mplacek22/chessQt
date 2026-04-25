#include "pawn.h"

std::vector<std::shared_ptr<Move> > Pawn::calculatePossibleMoves(
    const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board,
    const Coordinate &source,
    GameStatus gameStatus,
    std::optional<Move> lastMove)
{
    std::vector<std::shared_ptr<Move>> moves;
    int moveDirection = color_ == Color::WHITE ? 1 : -1;
    int promotionRank = color_ == Color::WHITE ? board.size() - 1 : 0;

    // forward move - 1 square
    int rank = source.rank() + moveDirection;
    int file = source.file();
    if (inBounds(rank, file) && !board[rank][file]){

        Coordinate destination {rank, file};
        auto move = std::make_shared<Move>(source, destination, color_);
        move->movingPiece = board[source.rank()][source.file()];

        // promotion
        move->moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::NORMAL;

        moves.push_back(std::move(move));
    }

    // forward move - 2 squares
    rank += moveDirection;
    if (!hasMoved_ && inBounds(rank, file) && !board[rank][file]) {
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

            if (isEnemy(board, destination)) {
                move->moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::CAPTURE;
                move->capturedPiece = board[rank][file];
                moves.push_back(std::move(move));
            }
            // en passant
            if (lastMove && lastMove->moveType == MoveType::PAWN_DOUBLE_NORMAL){
                int df = lastMove.value().destination.file() - source.file();
                // is captured piece next to moving piece
                bool next = lastMove.value().destination.rank() == source.rank() && (df == 1 || df == -1);
                // check if the destination of the last move matches the expected captured pawn position
                bool isCapturableTarget = lastMove->destination.file() == destination.file();
                if (!board[rank][file] && next && isCapturableTarget) {
                    move->moveType = MoveType::ENPASSANT;
                    move->capturedPiece = board[source.rank()][file];
                    moves.push_back(std::move(move));
                }
            }

        }
    }
    return moves;
}
