#include "game.h"
#include "move_generator.h"
#include "pawn.h"

Game::Game() {}

void Game::start() {
    status_ = GameStatus::IN_PROGRESS;
}

void Game::restart() {
    board_.restart();
    currentPlayer_ = Color::WHITE;
    movesHistory_.clear();
    start();
}

void Game::executeMove(const Move& move) {
    auto piece = board_.getPieceAt(move.source);
    board_.movePiece(move.source, move.destination);
    if (!piece->hasMoved()) {
        piece->setHasMoved(true);
    }

    switch (move.moveType) {
        case MoveType::ENPASSANT: {
            // remove the captured piece from the board
            auto movingPiece = std::dynamic_pointer_cast<Pawn>(piece);
            int rank = move.destination.rank - movingPiece->getNormalMoveDirections().front();
            board_.setPieceAt({rank, move.destination.file}, nullptr);
            break;
        }
        case MoveType::PROMOTION: {
            promotionSquare_ = move.destination;
            pendingPromotion_ = true;
            if(move.promotionPieceType) {
                promotePawn(move.promotionPieceType.value());
            }
            break;
        }
        case MoveType::CASTLE_KINGSIDE: {
            int rank = move.destination.rank;
            Coordinate rookSource = {rank, 7};
            Coordinate rookDestination = {rank, 5}; //f1 (white), f8 (black)
            board_.movePiece(rookSource, rookDestination);
            board_.getPieceAt(rookDestination)->setHasMoved(true);
            break;
        }
        case MoveType::CASTLE_QUEENSIDE: {
            int rank = move.destination.rank;
            Coordinate rookSource = {rank, 0};
            Coordinate rookDestination = {rank, 3}; //d1 (white), d8 (black)
            board_.movePiece(rookSource, rookDestination);
            board_.getPieceAt(rookDestination)->setHasMoved(true);
            break;
        }
        default:
            break;
    }
}

void Game::switchPlayer() {
    currentPlayer_ = currentPlayer_ == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void Game::updateGameStatus() {
    if (isFiftyMoveRule()) {
        setDraw(DrawCause::FIFTY_MOVE_RULE);
        return;
    }

    if(isInsufficientMaterial()) {
        setDraw(DrawCause::INSUFFICIENT_MATERIAL);
        return;
    }

    // todo: repetition

    Color enemyColor = oppositeColor(currentPlayer_);
    GameState state = gameState();
    bool canEnemyMove = MoveGenerator::canPlayerMove(board_, state, enemyColor);
    int numCheckers = MoveGenerator::computeCheckers(board_, state).size();

    if (numCheckers == 0) {
        if(canEnemyMove) {
            status_ = GameStatus::IN_PROGRESS;
        }
        else {
            setDraw(DrawCause::STALEMATE);
        }
        return;
    }
    else if (numCheckers > 0 && !canEnemyMove) {
        status_ = GameStatus::CHECK_MATE;
        updateWinner();
        return;
    }
    else if (numCheckers == 1) {
        status_ = GameStatus::SINGLE_CHECK;
    }
    else if (numCheckers == 2) {
        status_ = GameStatus::DOUBLE_CHECK;
    }
}

void Game::updateWinner()
{
    if (status_ == GameStatus::CHECK_MATE){
        winner_ = currentPlayer_;
    }
}

bool Game::isFiftyMoveRule() const
{
    return movesHistory_.size() >= 50;
}

bool Game::isRepetition() const
{
    return false;
}

bool Game::isInsufficientMaterial() const
{
    struct PieceCounts {
        int knights = 0;
        int total   = 0;
        bool hasBishopOnLight = false;
        bool hasBishopOnDark  = false;
    };

    PieceCounts white, black;

    for (int r = 0; r < Board::BOARD_SIZE; ++r) {
        for (int f = 0; f < Board::BOARD_SIZE; ++f) {
            auto piece = board_.getPieceAt({r, f});
            if (!piece) continue;

            auto& counts = (piece->color() == Color::WHITE) ? white : black;

            switch (piece->type()) {
            case PieceType::PAWN:
            case PieceType::ROOK:
            case PieceType::QUEEN:
                return false; // any major piece (P, R, Q) -> sufficient material
            case PieceType::KNIGHT:
                ++counts.total;
                ++counts.knights;
                break;
            case PieceType::BISHOP:
                ++counts.total;
                (board_.isLightSquare(r, f) ? counts.hasBishopOnLight : counts.hasBishopOnDark) = true;
                break;
            default:
                break;
            }
        }
    }

    // K vs K
    if (white.total == 0 && black.total == 0) return true;

    // K + minor vs K
    if ((white.total == 1 && black.total == 0) ||
        (black.total == 1 && white.total == 0)) return true;

    // KB vs KB on the same square color
    const bool sameColorBishops =
        (white.hasBishopOnLight && black.hasBishopOnLight) ||
        (white.hasBishopOnDark  && black.hasBishopOnDark);

    if (white.total == 1 && black.total == 1 && sameColorBishops) return true;

    return false;
}

Color Game::oppositeColor(Color color) const
{
    return color == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void Game::setDraw(DrawCause drawCause)
{
    status_ = GameStatus::DRAW;
    drawCause_ = drawCause;
}
void Game::processMove(Move& move) {
    executeMove(move);
    if (pendingPromotion_) {
        return;
    }
    updateGameStatus();
    move.gameStatusAfterMove = status_;
    movesHistory_.push_back(move);
    switchPlayer();
}

void Game::promotePawn(PieceType type)
{
    if (!promotionSquare_ || !pendingPromotion_) {
        return;
    }
    board_.setPieceAt(promotionSquare_.value(), Piece::create(currentPlayer_, type));
    pendingPromotion_ = false;
    promotionSquare_ = std::nullopt;
    Move& lastMove = movesHistory_.back();
    lastMove.promotionPieceType = type;
    updateGameStatus();
    switchPlayer();
}

const GameState Game::gameState() const
{
    return {currentPlayer_, status_, movesHistory_.empty() ? std::nullopt : std::optional(movesHistory_.back())};
}

bool Game::isGameOngoing() const
{
    return ONGOING_GAME_STATUSES_MASK & (1 << static_cast<int>(status_));
}

std::vector<Move> Game::calculatePossibleMovesFromCoord(const Coordinate &source)
{
    return MoveGenerator::calculatePossibleMoves(board_, source, gameState());
}
