#include "game.h"
#include "pawn.h"

#include <qDebug>

Game::Game() {}


void Game::start() {
    status_ = GameStatus::IN_PROGRESS;

    // while (status_ == GameStatus::IN_PROGRESS) {
    //     auto moveInput = ui_->getPlayerMove();
    //     auto moveOptional = parseMove(moveInput);

    //     if (!moveOptional.has_value()) {
    //         ui_->showMessage("Invalid move format. Try again.");
    //         continue;
    //     }

    //     Move& move = moveOptional.value(); // Structured binding (C++17+)

    //     if (!handleMove(move)) {
    //         ui_->showMessage("Invalid move. Try again.");
    //         continue;
    //     }

    //     if (move.moveType == MoveType::PROMOTION) {
    //         ui_->showMessage("Choose promotion piece type:");
    //         move.promotionPieceType = parsePromotionPiece(ui_->getPlayerMove());
    //     }

    //     processMove(move);
    // }
}

void Game::restart() {
    board_.restart();
    currentPlayer_ = Color::WHITE;
    movesHistory_.clear();
    start();
}

void Game::executeMove(const Move& move) {
    board_.movePiece(move.source, move.destination);
    if (!move.movingPiece->hasMoved()) {
        move.movingPiece->setHasMoved(true);
    }
    // remove the captured piece from the board
    if(move.moveType == MoveType::ENPASSANT){
        auto movingPiece = std::dynamic_pointer_cast<Pawn>(move.movingPiece);
        int rank = move.destination.rank() - movingPiece->getNormalMoveDirections().front();
        board_.setPieceAt({rank, move.destination.file()}, nullptr);
    }
    else if(move.moveType == MoveType::PROMOTION) {
        pendingPromotion_ = true;
        promotionSquare_ = move.destination;
    }
}

void Game::switchPlayer() {
    currentPlayer_ = currentPlayer_ == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void Game::updateGameStatus() {
    if (movesHistory_.size() == 50) {
        status_ = GameStatus::DRAW;
    }
}

void Game::processMove(Move& move) {
    // move.movingPiece = board_.getPieceAt(move.source);
    // move.capturedPiece = board_.getPieceAt(move.destination);
    // todo: update move.moveType and move.promotionPieceType

    // auto possibleMoves = board_.getPieceAt(move.source)->calculatePossibleMoves(board_.board(), move.source);
    // std::ranges::any_of(possibleMoves, [&move.destination](const std::shared_ptr<Move>& m) {
    //     return m->destination == move.destination;
    // }
    executeMove(move);
    movesHistory_.push_back(move);
    if (pendingPromotion_) {
        return;
    }
    updateGameStatus();
    move.gameStatus = status_;
    switchPlayer();
}

void Game::calculatePossibleMovesForPiece(Coordinate &source)
{

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
