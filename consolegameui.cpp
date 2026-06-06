#include "InvalidSANException.h"
#include "consolegameui.h"
#include "san.h"
#include <iostream>
#include <algorithm>

void ConsoleGameUI::run()
{
    GameClient::startGame();

    while (isGameOngoing()) {
        std::cout << "\n\n";
        displayCurrentPlayer();
        displayStatus();
        displayBoard();

        auto source = promptCoordinate("Source square (e.g. e2): ");
        if (!source) break;

        mediator_->onPossibleMovesRequested(source.value());
        if (cachedPossibleMoves_.empty()) {
            std::cout << "No legal moves from that square. Try again.\n";
            continue;
        }

        auto destination = promptCoordinate("Destination square (e.g. e4): ");
        if (!destination) break;

        auto it = std::find_if(cachedPossibleMoves_.begin(), cachedPossibleMoves_.end(),
                               [&](const Move& m) {
                                   return m.destination == destination;
                               });
        if (it == cachedPossibleMoves_.end()) {
            std::cout << "Move is not allowed. Try again.\n";
            continue;
        }

        mediator_->onMoveRequested(*it);
    }
}

void ConsoleGameUI::possibleMovesCalculated(std::vector<Move> moves)
{
    GameClient::possibleMovesCalculated(moves);
}

void ConsoleGameUI::onGameStateChanged(const GameState &gameState)
{
    GameClient::onGameStateChanged(gameState);
}

void ConsoleGameUI::onGameWon(Color winner)
{
    GameClient::onGameWon(winner);
    std::cout << "Game over!" << std::endl;
    std::string winnerStr = winner == Color::WHITE ? "white" : "black";
    std::cout << "The winner is: " << winnerStr << std::endl;
}

void ConsoleGameUI::onGameDrawn(DrawCause drawCause)
{
    GameClient::onGameDrawn(drawCause);
    std::cout << "Game over!" << std::endl;
    std::cout << "It's a draw!" << std::endl;
    std::cout << "Draw cause: " << drawCauseToString(drawCause_.value()) << std::endl;
}

void ConsoleGameUI::onPendingPromotionChanged(bool pendingPromotion)
{
    GameClient::onPendingPromotionChanged(pendingPromotion);
    if (!pendingPromotion) return;

    std::cout << "Pawn promotion! Choose piece [Q, R, B, N]: ";
    while (true) {
        std::string input;
        if (!std::getline(std::cin, input)) {
            GameClient::promotePawn(PieceType::QUEEN);  // default
            return;
        }
        if (input.empty()) continue;

        try {
            PieceType type = san::charToPieceType(toupper(input[0]));
            if (std::find(PROMOTION_PIECES.begin(), PROMOTION_PIECES.end(), type)
                != PROMOTION_PIECES.end()) {
                GameClient::promotePawn(type);
                return;
            }
        } catch (const InvalidSanException&) {}

        std::cout << "Invalid choice. Enter Q, R, B or N: ";
    }
}

void ConsoleGameUI::displayBoard() const
{
    std::cout << "   a b c d e f g h\n  -----------------" << std::endl;
    for (int r = 7; r >=0 ; --r) {
        std::string line = std::to_string(r + 1) + "| ";
        for (int f = 0; f < 8; ++f) {
            auto piece = cachedBoard_.at({r, f});
            if (piece) {
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

char ConsoleGameUI::pieceChar(PieceType type, Color color) const
{
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
    if(color == Color::WHITE){
        c = toupper(c);
    }
    return c;
}

void ConsoleGameUI::displayCurrentPlayer() const
{
    std::cout << "Current player: " << (cachedCurrentPlayer_ == Color::WHITE ? "White" : "Black") << std::endl;
}

void ConsoleGameUI::displayStatus() const
{
    std::cout << "Game state: " << gameStatusToString(cachedGameStatus_) << std::endl;
}

std::optional<Coordinate> ConsoleGameUI::promptCoordinate(const std::string& prompt) const
{
    while (true) {
        std::cout << prompt << std::endl;
        std::string input;
        if (!std::getline(std::cin, input)) return std::nullopt;

        auto start = input.find_first_not_of(" \t");
        auto end   = input.find_last_not_of(" \t");
        if (start == std::string::npos) continue;
        input = input.substr(start, end - start + 1);

        // if (input == "q" || input == "Q") return std::nullopt;

        try {
            return san::coordinateFromString(input);
        } catch (const InvalidSanException& e) {
            std::cout << "Invalid coordinate: " << e.what() << ". Try again.\n";
        }
    }
}

std::string ConsoleGameUI::drawCauseToString(DrawCause drawCause) const
{
    switch (drawCause) {
    case DrawCause::STALEMATE: return "Stalemate";
    case DrawCause::AGREED: return "Agreed";
    case DrawCause::FIFTY_MOVE_RULE: return "50 moves rule";
    case DrawCause::THREEFOLD_REPETITION: return "threefold-repetition";
    case DrawCause::INSUFFICIENT_MATERIAL: return "insufficient material";
    default: return "";
    }
}

std::string ConsoleGameUI::gameStatusToString(GameStatus gameStatus) const
{
    switch (gameStatus) {
    case GameStatus::IN_PROGRESS: return "in_progress";
    case GameStatus::SINGLE_CHECK: return "check";
    case GameStatus::DOUBLE_CHECK: return "double_check";
    case GameStatus::DRAW: return "draw";
    case GameStatus::NEW: return "new";
    default: return "";
    }
}