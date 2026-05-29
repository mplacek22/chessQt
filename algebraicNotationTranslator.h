#ifndef ALGEBRAICNOTATIONTRANSLATOR_H
#define ALGEBRAICNOTATIONTRANSLATOR_H

#include "Board.h"
#include "game_state.h"

namespace AlgebraicNotationTranslator
{
std::string toSAN(const Move& move);

Move fromSAN (const std::string& san, const Board& board, const GameState& gameState);

Coordinate destinationFromSAN(const std::string& raw, const Color color);

Coordinate sourceFromSAN(const std::string& raw, const Color color, const Board& board);

Coordinate coordinateFromString(const std::string& raw);

std::string stripCheckAnnotations(const std::string& raw);

std::string coordinateToSAN(const Coordinate& coordinate);

char rankSAN(int rank);

char fileSAN(int file);

Move resolveCastle(bool isKingSide, const Board& board, const GameState& gameState);

PieceType charToPieceType(char c);

char pieceTypeToChar(PieceType pieceType);
};

#endif // ALGEBRAICNOTATIONTRANSLATOR_H
