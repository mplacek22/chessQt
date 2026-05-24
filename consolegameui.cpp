#include "consolegameui.h"
#include <iostream>
#include "InvalidSANException.h"
#include "algebraicNotationTranslator.h"

void ConsoleGameUI::run()
{
    game_.start();
    while(game_.isGameOngoing()) {
        displayCurrentPlayer();
        std::cout << "Game state: " << gameStatusToString(game_.status()) << std::endl;
        displayBoard();

        auto san = readSAN();
        if (!san.has_value())
            break;

        try {
            Move move = AlgebraicNotationTranslator::fromSAN(san.value(), game_.board(), game_.gameState());
            game_.processMove(move);
        }
        catch (const InvalidSanException& e) {
            std::cout << e.what() << std::endl;
                continue;
        }

        // Coordinate source{-1, -1};
        // Coordinate destination{-1, -1};
        // try {
        //     source = AlgebraicNotationTranslator::sourceFromSAN(san.value(), game_.currentPlayer(), game_.board());
        //     destination = AlgebraicNotationTranslator::destinationFromSAN(san.value(), game_.currentPlayer());
        // }
        // catch (const InvalidSanException& e) {
        //     std::cout << e.what() << std::endl;
        //     continue;
        // }

        // auto activeMoves = MoveGenerator::calculatePossibleMoves(game_.board(), source, game_.gameState());
        // auto it = std::find_if(activeMoves.begin(), activeMoves.end(),
        //                        [&](const std::shared_ptr<Move>& m) {
        //                            return m->destination == destination;
        //                        });
        // if (it == activeMoves.end()) {
        //     std::cout << "Move " + san.value() + " is not allowed." << std::endl;
        //     continue;
        // }

        // game_.processMove(**it);
    }

    gameOver();
}

void ConsoleGameUI::displayBoard() const
{
    std::cout << "   a b c d e f g h\n  -----------------" << std::endl;
    for (int r = 7; r >= 0; --r) {
        std::string line = std::to_string(r + 1) + "| ";
        for (int f = 0; f < 8; ++f) {
            auto piece = game_.board().getPieceAt({r, f});
            if (piece) {
                line += pieceChar(piece->type(), piece->color());
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
    std::cout << "Current player: " << (game_.currentPlayer() == Color::WHITE ? "White" : "Black") << std::endl;
}

std::optional<std::string> ConsoleGameUI::readSAN() const
{
    std::string input;
    while (true) {
        std::cout << "Enter move in SAN" << std::endl;
        if (!std::getline(std::cin, input))
            return std::nullopt;
        auto start = input.find_first_not_of(" \t");
        auto end   = input.find_last_not_of(" \t");

        if (start == std::string::npos)
            continue;
        input = input.substr(start, end - start + 1);

        if (input.size() < 2 || input.size() > 7) {
            std::cout << "Invalid input, try again.\n";
            continue;
        }

        return input;
    }
}

void ConsoleGameUI::gameOver() const
{
    if(game_.isGameOngoing()) return;

    std::cout << "Game over!" << std::endl;
    if(game_.winner()) {
        std::string winner = game_.winner().value() == Color::WHITE ? "white" : "black";
        std::cout << "The winner is: " << winner << std::endl;
    }
    if(game_.drawCause()) {
        std::cout << "It's a draw!" << std::endl;
        std::cout << "Draw cause: " << drawCauseToString(game_.drawCause().value()) << std::endl;
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
