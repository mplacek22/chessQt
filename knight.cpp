// #include "knight.h"

// Knight::Knight() {}

// std::vector<std::shared_ptr<Move> > Knight::calculatePossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board, const Coordinate &source)
// {
//     return Piece::calculatePossibleMoves(board, source);;

//     std::vector<std::shared_ptr<Move>> moves;

//     for (const auto& dir : getMoveDirections()) {
//         int rank = source.rank() + dir[0];
//         int file = source.file() + dir[1];

//         if (inBounds(rank, file)) {
//             Coordinate destination  {rank, file};
//             if(!isFriendly(board, destination)) {
//                 auto move = std::make_shared<Move>(source, destination, color_);
//                 move->movingPiece = board[source.rank()][source.file()];
//                 const auto& capturedPiece = board[rank][file];
//                 if (capturedPiece) {
//                     move->moveType = MoveType::CAPTURE;
//                     move->capturedPiece = capturedPiece;
//                 }
//                 else {
//                     move->moveType = MoveType::NORMAL;
//                 }
//                 moves.push_back(move);
//             }
//         }

//     }
//     return moves;
// }
