#include "InvalidSANException.h"
#include "consolegameui.h"
#include "san.h"
#include <iostream>
#include <string>

void ConsoleGameUI::run() {
    startGame();

    while (true) {
        const auto action = isGameOngoing() ? runGameLoop() : promptEndGame();
        if (action == LoopAction::QUIT) return;
        restartGame();
    }
}

void ConsoleGameUI::possibleMovesCalculated(std::vector<Move> moves) {
    IGameClient::possibleMovesCalculated(moves);
}

void ConsoleGameUI::onGameStateChanged(const GameState &gameState) {
    IGameClient::onGameStateChanged(gameState);
}

void ConsoleGameUI::onGameWon(Color winner) {
    IGameClient::onGameWon(winner);
    std::cout << "Game over!" << std::endl;
    std::string winnerStr = winner == Color::WHITE ? "white" : "black";
    std::cout << "The winner is: " << winnerStr << std::endl;
}

void ConsoleGameUI::onGameDrawn(DrawCause drawCause) {
    IGameClient::onGameDrawn(drawCause);
    std::cout << "Game over!" << std::endl;
    std::cout << "It's a draw!" << std::endl;
    std::cout << "Draw cause: " << drawCauseToString(drawCause_.value()) << std::endl;
}

void ConsoleGameUI::onPendingPromotionChanged(bool pendingPromotion) {
    IGameClient::onPendingPromotionChanged(pendingPromotion);
    if (!pendingPromotion) return;

    std::cout << "Pawn promotion! Choose piece [Q, R, B, N]: " << std::endl;
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        const auto start = input.find_first_not_of(" \t");
        const auto end   = input.find_last_not_of(" \t");
        if (start != std::string::npos)
            input = input.substr(start, end - start + 1);

        if (input.length() == 1) {
            try {
                if (PieceType type = san::charToPieceType(toupper(input[0]));
                    std::ranges::find(PROMOTION_PIECES, type) != PROMOTION_PIECES.end()) {
                    IGameClient::promotePawn(type);
                    return;
                }
            } catch (const InvalidSanException &) {}
        }
        std::cout << "Invalid choice. Enter Q, R, B or N: " << std::endl;
    }
}

void ConsoleGameUI::displayBoard() const {
    std::cout << "   a b c d e f g h\n  -----------------" << std::endl;
    for (int r = 7; r >= 0; --r) {
        std::string line = std::to_string(r + 1) + "| ";
        for (int f = 0; f < 8; ++f) {
            if (const auto piece = cachedBoard_.at({r, f})) {
                line += pieceChar(piece->type, piece->color);
            } else
                line += '.';
            line += ' ';
        }
        line += '|' + std::to_string(r + 1);
        std::cout << line << std::endl;
    }
    std::cout << "  -----------------\n   a b c d e f g h" << std::endl;
}

void ConsoleGameUI::displayCurrentPlayer() const {
    std::cout << "Current player: " << (cachedCurrentPlayer_ == Color::WHITE ? "White" : "Black") << std::endl;
}

auto ConsoleGameUI::pieceChar(const PieceType type, Color color) -> char {
    char c = [&]() -> char {
        switch (type) {
            case PieceType::PAWN: return 'p';
            case PieceType::BISHOP: return 'b';
            case PieceType::KING: return 'k';
            case PieceType::KNIGHT: return 'n';
            case PieceType::QUEEN: return 'q';
            case PieceType::ROOK: return 'r';
        }
        throw std::invalid_argument("Unknown PieceType");
    }();
    if (color == Color::WHITE) {
        c = toupper(c);
    }
    return c;
}

void ConsoleGameUI::displayStatus() const {
    std::cout << "Game state: " << gameStatusToString(cachedGameStatus_) << std::endl;
}

void ConsoleGameUI::displayEndGameOptions() {
    std::cout << "Enter 'r' to restart game or 'q' to quit." << std::endl;
}

ConsoleGameUI::PromptOutcome ConsoleGameUI::promptCoordinate(const std::string &prompt) {
    while (true) {
        std::cout << prompt << std::endl;
        std::string input;
        if (!std::getline(std::cin, input)) return {PromptResult::QUIT, {}};

        const auto start = input.find_first_not_of(" \t");
        const auto end   = input.find_last_not_of(" \t");
        if (start != std::string::npos)
            input = input.substr(start, end - start + 1);

        if (input == "q" || input == "quit") return {PromptResult::QUIT, {}};
        if (input == "r" || input == "restart") return {PromptResult::RESTART, {}};

        try {
            return {PromptResult::COORDINATE, san::coordinateFromString(input)};
        } catch (const InvalidSanException &e) {
            std::cout << "Invalid coordinate: " << e.what() << ". Try again." << std::endl;
        }
    }
}

std::string ConsoleGameUI::drawCauseToString(DrawCause drawCause) {
    switch (drawCause) {
        case DrawCause::STALEMATE: return "Stalemate";
        case DrawCause::AGREED: return "Agreed";
        case DrawCause::FIFTY_MOVE_RULE: return "50 moves rule";
        case DrawCause::THREEFOLD_REPETITION: return "threefold-repetition";
        case DrawCause::INSUFFICIENT_MATERIAL: return "insufficient material";
        default: return "";
    }
}

std::string ConsoleGameUI::gameStatusToString(GameStatus gameStatus) {
    switch (gameStatus) {
        case GameStatus::IN_PROGRESS: return "in_progress";
        case GameStatus::SINGLE_CHECK: return "check";
        case GameStatus::DOUBLE_CHECK: return "double_check";
        case GameStatus::DRAW: return "draw";
        case GameStatus::NEW: return "new";
        default: return "";
    }
}

ConsoleGameUI::LoopAction ConsoleGameUI::runGameLoop() {
    while (isGameOngoing()) {
        std::cout << "\n\n";
        displayCurrentPlayer();
        displayStatus();
        displayBoard();

        const auto [srcResult, source] = promptCoordinate("Source square (e.g. e2) [r=restart, q=quit]: ");
        if (srcResult == PromptResult::QUIT) return LoopAction::QUIT;
        if (srcResult == PromptResult::RESTART) return LoopAction::RESTART;

        mediator_->onPossibleMovesRequested(*source);
        if (cachedPossibleMoves_.empty()) {
            std::cout << "No legal moves from that square. Try again." << std::endl;
            continue;
        }

        const auto [dstResult, destination] = promptCoordinate("Destination square (e.g. e4) [r=restart, q=quit]: ");
        if (dstResult == PromptResult::QUIT) return LoopAction::QUIT;
        if (dstResult == PromptResult::RESTART) return LoopAction::RESTART;

        const auto it = std::ranges::find_if(cachedPossibleMoves_,
                                             [&](const Move &m) {
                                                 return m.destination == destination;
                                             });
        if (it == cachedPossibleMoves_.end()) {
            std::cout << "Move is not allowed. Try again." << std::endl;
            continue;
        }

        mediator_->onMoveRequested(*it);
    }

    return promptEndGame();
}

ConsoleGameUI::LoopAction ConsoleGameUI::promptEndGame() const {
    displayBoard();
    displayEndGameOptions();

    while (true) {
        std::cout << "[r=restart, q=quit]: ";
        std::string input;
        if (!std::getline(std::cin, input)) return LoopAction::QUIT;
        if (input == "q" || input == "quit") return LoopAction::QUIT;
        if (input == "r" || input == "restart") return LoopAction::RESTART;
        std::cout << "Unknown command." << std::endl;
    }
}
