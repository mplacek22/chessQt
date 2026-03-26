#include "game.h"

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
    start();
}

void Game::executeMove(const Move& move) {
    board_.movePiece(move.source, move.destination);
    switchPlayer();
}

void Game::switchPlayer() {
    currentPlayer_ = currentPlayer_ == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void Game::updateGameStatus() {
    if (movesHistory_.size() == 50) {
        status_ = GameStatus::DRAW;
    }
}
