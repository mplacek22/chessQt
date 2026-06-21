#include "game.h"
#include "move_generator.h"

Game::Game() = default;

void Game::start() {
    status_ = GameStatus::IN_PROGRESS;
    mediator_->onGameStateChanged(gameState());
}

void Game::restart() {
    board_.restart();
    currentPlayer_ = Color::WHITE;
    winner_ = std::nullopt;
    drawCause_ = std::nullopt;
    movesHistory_.clear();
    halfMoveClock = 0;
    start();
}

void Game::executeMove(const Move &move) {
    board_.move(move.source, move.destination);
    board_.at(move.destination)->hasMoved = true;
    updateHalfMoveClock(move);
    switch (move.moveType) {
        case MoveType::ENPASSANT: {
            // remove the captured piece from the board
            const int rank = move.destination.rank - (board_.at(move.source)->color == Color::WHITE ? 1 : -1);
            board_.set({rank, move.destination.file}, std::nullopt);
            break;
        }
        case MoveType::PROMOTION: {
            setPendingPromotionMove(move);
            if (move.promotionPieceType) {
                promotePawn(move.promotionPieceType.value());
            }
            break;
        }
        case MoveType::CASTLE_KINGSIDE: {
            int rank = move.destination.rank;
            Coordinate rookSource = {rank, 7};
            Coordinate rookDestination = {rank, 5}; //f1 (white), f8 (black)
            board_.move(rookSource, rookDestination);
            board_.at(rookDestination)->hasMoved = true;
            break;
        }
        case MoveType::CASTLE_QUEENSIDE: {
            const int rank = move.destination.rank;
            const Coordinate rookSource = {rank, 0};
            const Coordinate rookDestination = {rank, 3}; //d1 (white), d8 (black)
            board_.move(rookSource, rookDestination);
            board_.at(rookDestination)->hasMoved = true;
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
        setDraw(FIFTY_MOVE_RULE);
        return;
    }

    if (isInsufficientMaterial()) {
        setDraw(INSUFFICIENT_MATERIAL);
        return;
    }

    // todo: repetition

    const Color enemyColor = oppositeColor(currentPlayer_);
    const GameState currentState = gameState();
    GameState enemyState = currentState;
    enemyState.currentPlayer = enemyColor;
    const bool canEnemyMove = move_generator::canCurrentPlayerMove(enemyState);
    const auto numCheckers = move_generator::computeCheckers(currentState).size();

    if (numCheckers == 0) {
        if (canEnemyMove) {
            status_ = GameStatus::IN_PROGRESS;
        } else {
            setDraw(DrawCause::STALEMATE);
        }
        return;
    }
    if (!canEnemyMove) {
        status_ = GameStatus::CHECK_MATE;
        updateWinner();
        return;
    }
    if (numCheckers == 1) {
        status_ = GameStatus::SINGLE_CHECK;
    } else if (numCheckers == 2) {
        status_ = GameStatus::DOUBLE_CHECK;
    }
}

void Game::updateWinner() {
    if (status_ == GameStatus::CHECK_MATE) {
        winner_ = currentPlayer_;
        mediator_->onGameWon(winner_.value());
    }
}

bool Game::isFiftyMoveRule() const {
    return halfMoveClock >= 100;
}

bool Game::isRepetition() const {
    return false;
}

bool Game::isInsufficientMaterial() const {
    struct PieceCounts {
        int knights = 0;
        int total = 0;
        bool hasBishopOnLight = false;
        bool hasBishopOnDark = false;
    };

    PieceCounts white, black;

    for (int r = 0; r < Board::BOARD_SIZE; ++r) {
        for (int f = 0; f < Board::BOARD_SIZE; ++f) {
            const auto piece = board_.at({r, f});
            if (!piece) continue;

            auto &[knights, total, hasBishopOnLight, hasBishopOnDark] = piece->color == Color::WHITE ? white : black;

            switch (piece->type) {
                case PieceType::PAWN:
                case PieceType::ROOK:
                case PieceType::QUEEN:
                    return false; // any major piece (P, R, Q) -> sufficient material
                case PieceType::KNIGHT:
                    ++total;
                    ++knights;
                    break;
                case PieceType::BISHOP:
                    ++total;
                    (Board::isLightSquare(r, f) ? hasBishopOnLight : hasBishopOnDark) = true;
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
        (black.total == 1 && white.total == 0))
        return true;

    // KB vs KB on the same square color
    const bool sameColorBishops =
            (white.hasBishopOnLight && black.hasBishopOnLight) ||
            (white.hasBishopOnDark && black.hasBishopOnDark);

    if (white.total == 1 && black.total == 1 && sameColorBishops) return true;

    return false;
}

Color Game::oppositeColor(const Color color) {
    return color == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void Game::setDraw(DrawCause drawCause) {
    status_ = GameStatus::DRAW;
    drawCause_ = drawCause;
    mediator_->onGameDrawn(drawCause);
}


void Game::setPendingPromotionMove(const std::optional<Move> &moveOpt) {
    pendingPromotionMove_ = moveOpt;
    mediator_->onPromotionPending(moveOpt.has_value());
}

void Game::updateHalfMoveClock(const Move &move)
{
    if (move.movingPieceType == PieceType::PAWN || move.isCapture){
        halfMoveClock = 0;
    }
    else {
        ++halfMoveClock;
    }
}

void Game::processMove(const Move &currentMove) {
    executeMove(currentMove);
    if (pendingPromotion()) {
        return;
    }
    movesHistory_.push_back(currentMove);
    updateGameStatus();
    movesHistory_.back().gameStatusAfterMove = status_;
    switchPlayer();
    mediator_->onGameStateChanged(gameState());
}

bool Game::pendingPromotion() const { return pendingPromotionMove_.has_value(); }

void Game::promotePawn(PieceType type) {
    if (!pendingPromotion()) {
        return;
    }
    board_.set(pendingPromotionMove_.value().destination, {type, currentPlayer_});
    updateGameStatus();
    if (pendingPromotionMove_.has_value()) {
        pendingPromotionMove_.value().gameStatusAfterMove = status_;
        pendingPromotionMove_.value().promotionPieceType = type;
        movesHistory_.push_back(pendingPromotionMove_.value());
        setPendingPromotionMove(std::nullopt);
    }
    switchPlayer();
    mediator_->onGameStateChanged(gameState());
}

GameState Game::gameState() const {
    return {
        currentPlayer_, status_, board_, movesHistory_.empty() ? std::nullopt : std::optional(movesHistory_.back())
    };
}

void Game::requestPossibleMoves(const Coordinate &coord) const {
    const auto moves = move_generator::calculatePossibleMoves(coord, gameState());
    mediator_->onPossibleMovesCalculated(moves);
}
