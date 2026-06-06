#include "move_generator.h"
#include <algorithm>
\
namespace  {

bool isSliding(const PieceType pieceType)
{
    return move_generator::SLIDING_PIECE_MASK & (1 << static_cast<int>(pieceType));
}

 std::vector<Move> calculatePawnMoves(const Coordinate& source, const GameState& state)
{
    std::vector<Move> moves;
    const auto movingPiece = state.board.at(source);
    const int moveDirection = state.currentPlayer == Color::WHITE ? 1 : -1;
    const int promotionRank = state.currentPlayer == Color::WHITE ? Board::BOARD_SIZE - 1 : 0;

    // forward move - 1 square
    int rank = source.rank + moveDirection;
    int file = source.file;
    Coordinate destination {rank, file};
    const bool singlePushClear = Board::inBounds(destination) && !state.board.at(destination).has_value();

    if (singlePushClear){

        Move move{source, destination, state.currentPlayer};
        move.movingPieceType = movingPiece->type;

        // promotion
        move.moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::NORMAL;

        moves.push_back(move);
    }

    // forward move - 2 squares
    rank += moveDirection;
    destination = {rank, file};
    if (!movingPiece->hasMoved && singlePushClear && Board::inBounds(destination) && !state.board.at(destination).has_value()) {
        Move move{source, destination, movingPiece->color};
        move.movingPieceType = movingPiece->type;
        move.moveType = MoveType::PAWN_DOUBLE_NORMAL;
        moves.push_back(move);
    }

    // capture
    rank = source.rank + moveDirection;
    for (const int f : {-1, 1}) {
        file = source.file + f;
        destination = {rank, file};
        if (Board::inBounds(destination)){
            Move move {source, destination, state.currentPlayer};
            move.movingPieceType = movingPiece->type;
            move.isCapture = true;

            if (state.board.isEnemy(destination, state.currentPlayer)) {
                move.moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::CAPTURE;
                moves.push_back(move);
            }
            // en passant
            if (state.lastMove && state.lastMove->moveType == MoveType::PAWN_DOUBLE_NORMAL){
                const int df = state.lastMove.value().destination.file - source.file;
                // is captured piece next to moving piece
                const bool next = state.lastMove.value().destination.rank == source.rank && (df == 1 || df == -1);
                // check if the destination of the last move matches the expected captured pawn position
                const bool isCapturableTarget = state.lastMove->destination.file == destination.file;
                if (!state.board.at(destination).has_value() && next && isCapturableTarget) {
                    move.moveType = MoveType::ENPASSANT;
                    moves.push_back(move);
                }
            }

        }
    }
    return moves;
}

 std::vector<Move> calculateRegularMoves(const Board& board, const Coordinate& source) {
    const auto piece = board.at(source);

    if (!piece) return {};

    std::vector<Move> moves;

    for (const auto& dir : move_generator::MOVE_DIRECTIONS.at(piece->type)) {
        int rank = source.rank;
        int file = source.file;

        do {
            rank += dir[0];
            file += dir[1];

            Coordinate destination {rank, file};

            if (!Board::inBounds(destination)) break;

            if (board.isFriendly(destination, piece->color)) break;

            Move move {source, destination, piece->color};
            move.movingPieceType = piece->type;
            if (board.at(destination)) {
                move.moveType = MoveType::CAPTURE;
                move.isCapture = true;
            }
            else {
                move.moveType = MoveType::NORMAL;
            }
            moves.push_back(move);

            if (board.isEnemy(destination, piece->color)) break;

        } while (isSliding(piece->type));
    }

    return moves;
}

// All moves ignoring whether they leave the king in check
 std::vector<Move> calculatePseudoLegalMoves(const Coordinate& source, const GameState& state)
{
    const auto piece = state.board.at(source);
    if (!piece) return {};
    switch (piece->type) {
    case PieceType::PAWN:   return calculatePawnMoves(source, state);
    default:                return calculateRegularMoves(state.board, source); // B, R, Q, N, K
    }
}

 std::vector<Coordinate> computeKingDangerSquares(const Coordinate& kingPosition, const GameState& gameState)
{
    const Color color = gameState.board.at(kingPosition)->color;
    auto boardWithoutKing = gameState.board;
    boardWithoutKing.set(kingPosition, std::nullopt);

    std::vector<Coordinate> danger;
    for (int r = 0; r < Board::BOARD_SIZE; r++) {
        for (int f = 0; f < Board::BOARD_SIZE; f++) {
            const auto piece = boardWithoutKing.at({r, f});
            if (piece && piece->color != color) {
                GameState tmpState = {gameState.currentPlayer, gameState.gameStatus, boardWithoutKing, gameState.lastMove};
                for (const auto&  m : calculatePseudoLegalMoves({r, f}, tmpState)) {
                    danger.push_back(m.destination);
                }
            }
        }
    }
    return danger;
}

 std::vector<Move> calculateKingMoves(const Coordinate& source, const GameState& state)
{
    auto moves = calculateRegularMoves(state.board, source);
    auto dangerSquares = computeKingDangerSquares(source, state);

    // Filter out moves to king danger squares
    std::erase_if(moves, [&](const auto& m) {
        return std::ranges::find(dangerSquares, m.destination) != dangerSquares.end();
    });

    if(move_generator::canCastleKingSide(state)) {
        Move move {source, Coordinate{source.rank, 6}, state.currentPlayer};
        move.moveType = MoveType::CASTLE_KINGSIDE;
        move.movingPieceType = state.board.at(source)->type;
        moves.push_back(move);
    }
    if(move_generator::canCastleQueenSide(state)) {
        Move move {source, Coordinate{source.rank, 2}, state.currentPlayer};
        move.moveType = MoveType::CASTLE_QUEENSIDE;
        move.movingPieceType = state.board.at(source)->type;
        moves.push_back(move);
    }

    return moves;
}


 bool isSquareAttackedBy(const Coordinate& target, const Color attackerColor, const GameState& gameState)
{
    const GameState nextMoveState{attackerColor, gameState.gameStatus, gameState.board, gameState.lastMove};

    for (int r = 0; r < Board::BOARD_SIZE; ++r) {
        for (int f = 0; f < Board::BOARD_SIZE; ++f) {
            Coordinate source = {r, f};
            auto piece = gameState.board.at(source);
            if (!piece || piece->color != attackerColor) continue;

            for (auto moves = calculatePseudoLegalMoves(source, nextMoveState); const auto& move : moves) {
                if (move.destination == target && move.isCapture)
                    return true;
            }
        }
    }
    return false;
}

 bool canCastle(const GameState& state, const bool kingSide) {
    // no check
    if(state.gameStatus == GameStatus::SINGLE_CHECK || state.gameStatus == GameStatus::DOUBLE_CHECK){
        return false;
    }
    // king hasn't moved
    const Coordinate kingStartPosition(state.currentPlayer == Color::WHITE ? 0 : 7, 4);
    Coordinate kingPosition = state.board.findKing(state.currentPlayer);
    const auto king = state.board.at(kingPosition);
    if(kingPosition != kingStartPosition || king->hasMoved) {
        return false;
    }
    // rook hasn't moved
    const int rookFileOffset = kingSide ? 3 : -4;
    const Coordinate rookStartPosition = {kingPosition.rank, kingPosition.file + rookFileOffset};
    const auto rook = state.board.at(rookStartPosition);
    if (!rook || rook->type != PieceType::ROOK || rook->hasMoved) {
        return false;
    }
    // clear path
    if (!state.board.isPathClear(kingPosition, rookStartPosition)){
        return false;
    }

    // path squares cant be attacked
    const Color attackerColor = state.currentPlayer == Color::WHITE ? Color::BLACK : Color::WHITE;
    const int direction = kingSide ? 1 : -1;
    for (int step = 0; step <= 2; ++step) {
        const Coordinate square = {kingPosition.rank, kingPosition.file + direction * step};
        if (isSquareAttackedBy(square, attackerColor, state)) {
            return false;
        }
    }

    return true;
}

 bool leavesKingInCheck(const Move& move, const GameState& state)
{
    auto copy = state.board;
    const auto movingPiece = copy.at(move.source);
    const Color movingColor = movingPiece->color;
    const Color enemy = (movingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

    // remove the en-passant captured pawn before checking for check
    if (move.moveType == MoveType::ENPASSANT) {
        const int capturedPawnRank = move.source.rank;
        const int capturedPawnFile = move.destination.file;
        copy.set({capturedPawnRank, capturedPawnFile}, std::nullopt);
    }

    // simulate the move
    copy.move(move.source, move.destination);

    const Coordinate kingSquare = copy.findKing(movingColor);
    const GameState tmpState = {state.currentPlayer, state.gameStatus, copy, move};

    return isSquareAttackedBy(kingSquare, enemy, tmpState);
}

}

std::vector<Move> move_generator::calculatePossibleMoves(const Coordinate &source, const GameState &state)
{
    const auto piece = state.board.at(source);
    if (!piece) return {};

    // in double check only the king can move
    if(state.gameStatus == GameStatus::DOUBLE_CHECK && state.board.at(source)->type != PieceType::KING) {
        return {};
    }

    std::vector<Move> pseudo;
    switch (piece->type) {
    case PieceType::PAWN:
        pseudo = calculatePawnMoves(source, state);
        break;
    case PieceType::KING:
        pseudo = calculateKingMoves(source, state);
        break;
    default:
        pseudo = calculateRegularMoves(state.board, source); // B, R, Q, N
        break;
    }

    std::vector<Move> legal;
    legal.reserve(pseudo.size());

    for (const auto& move : pseudo) {
        if (!leavesKingInCheck(move, state))
            legal.push_back(move);
    }

    return legal;
}


std::vector<Coordinate> move_generator::computeCheckers(const GameState &gameState)
{
    const Color enemy = gameState.currentPlayer == Color::WHITE ? Color::BLACK : Color::WHITE;

    const Coordinate kingPosition = gameState.board.findKing(enemy);

    std::vector<Coordinate> checkers;

    for (int r = 0; r < Board::BOARD_SIZE; ++r) {
        for (int f = 0; f < Board::BOARD_SIZE; ++f) {
            Coordinate source {r, f};
            const auto piece = gameState.board.at(source);
            if (!piece || piece->color != gameState.currentPlayer) continue;

            auto moves = calculatePseudoLegalMoves(source, gameState);
            for (const auto& move : moves) {
                if (move.destination == kingPosition) {
                    checkers.push_back(source);
                    if (checkers.size() == 2) {
                        return checkers;
                    }
                    break;
                }
            }
        }
    }
    return checkers;
}

bool move_generator::canPlayerMove(const GameState &gameState, const Color player) {
    std::vector<Coordinate> pieces_coords;

    for (int r = 0; r < Board::BOARD_SIZE; ++r) {
        for (int f = 0; f < Board::BOARD_SIZE; ++f) {
            Coordinate coord {r, f};
            const auto piece = gameState.board.at(coord);
            if(piece && piece->color == player) {
                pieces_coords.push_back(coord);
            }
        }
    }
    return std::ranges::any_of(pieces_coords,
                               [&](const auto& source) {
                                   return !calculatePossibleMoves(source, gameState).empty();
                               }
                               );
}

bool move_generator::canCastleKingSide(const GameState &state) {
    return canCastle(state, true);
}

bool move_generator::canCastleQueenSide(const GameState &state) {
    return canCastle(state, false);
}
