#include "san.h"

#include <string>

#include "InvalidSANException.h"
#include "move_generator.h"

namespace {

char rankSAN(int rank)
{
    if(rank < 0 || rank >= Board::BOARD_SIZE){
        throw InvalidSanException("Invalid rank: " + std::to_string(rank));
    }
    return static_cast<char>('1' + rank);
}

char fileSAN(int file)
{
    if(file < 0 || file >= Board::BOARD_SIZE){
        throw InvalidSanException("Invalid rank: " + std::to_string(file));
    }
    return static_cast<char>('a' + file);
}

Coordinate coordinateFromString(const std::string &raw)
{
    if (raw.size() < 2) {
        throw InvalidSanException("Coordinate too short: " + raw);
    }
    if (raw[0] < 'a' || raw[0] > 'h' ||
        raw[1] < '1' || raw[1] > '8') {
        throw InvalidSanException("File must be between 'a' and 'h' and rank between '1' and '8'");
    }
    int file = raw[0] - 'a';
    int rank = raw[1] - '1';
    return {rank, file};
}

std::string stripCheckAnnotations(const std::string &raw)
{
    if (!raw.empty() && (raw.back() == '+' || raw.back() == '#'))
        return raw.substr(0, raw.size() - 1);
    return raw;
}

std::string coordinateToSAN(const Coordinate& coordinate)
{
    return {fileSAN(coordinate.file), rankSAN(coordinate.rank)};
}

Move resolveCastle(bool isKingSide, const GameState &gameState)
{
    int rank = gameState.currentPlayer == Color::WHITE ? 0 : 7;
    Coordinate source(rank, 4);

    if(isKingSide) {
        if(MoveGenerator::canCastleKingSide(gameState)) {
            auto move = Move{source, Coordinate{rank , 6}, gameState.currentPlayer};
            move.moveType = MoveType::CASTLE_KINGSIDE;
            move.movingPieceType = PieceType::KING;
            return move;
        }
        throw InvalidSanException("Can't castle king side.");
    }
    if(MoveGenerator::canCastleKingSide(gameState)) {
        auto move = Move{source, Coordinate{rank , 2}, gameState.currentPlayer};
        move.moveType = MoveType::CASTLE_KINGSIDE;
        move.movingPieceType = PieceType::KING;
        return move;
    }
    throw InvalidSanException("Can't castle queen side.");
}

char pieceTypeToChar(PieceType pieceType)
{
    switch (pieceType) {
    case PieceType::BISHOP: return 'B';
    case PieceType::KING: return 'K';
    case PieceType::KNIGHT: return 'N';
    case PieceType::ROOK: return 'R';
    case PieceType::QUEEN: return 'Q';
    default: return '\0';
    }
}

}

PieceType san::charToPieceType(char c)
{
    switch (std::toupper(c)) {
    case 'B': return PieceType::BISHOP;
    case 'K': return PieceType::KING;
    case 'N': return PieceType::KNIGHT;
    case 'P': return PieceType::PAWN;
    case 'Q': return PieceType::QUEEN;
    case 'R': return PieceType::ROOK;
    default: throw InvalidSanException(std::string("Unknown piece char: ") + c);
    }
}

std::string san::toSAN(const Move &move)
{
    if (move.moveType == MoveType::CASTLE_KINGSIDE)  return "O-O";
    if (move.moveType == MoveType::CASTLE_QUEENSIDE) return "O-O-O";

    std::string notation;
    if(move.movingPieceType != PieceType::PAWN) {
        notation += pieceTypeToChar(move.movingPieceType);
    }

    // if(move.isAmbiguous) {
    //     notation += coordinateToSAN(move.source);
    // }
    notation += coordinateToSAN(move.source);


    if (move.isCapture) {
        notation += 'x';
    }

    notation += coordinateToSAN(move.destination);


    if (move.moveType == MoveType::PROMOTION) {
        notation += '=';
        switch (move.promotionPieceType.value()) {
        case PieceType::QUEEN:  notation += 'Q'; break;
        case PieceType::ROOK:   notation += 'R'; break;
        case PieceType::BISHOP: notation += 'B'; break;
        case PieceType::KNIGHT: notation += 'N'; break;
        default: break;
        }
    }

    switch (move.gameStatusAfterMove) {
    case GameStatus::SINGLE_CHECK:
    case GameStatus::DOUBLE_CHECK:
        notation += '+'; break;
    case GameStatus::CHECK_MATE:
        notation += '#'; break;
    default: break;
    }

    return notation;
}

Move san::fromSAN(const std::string& san, const GameState& gameState)
{
    if (san.empty()) {
        throw InvalidSanException("Empty string");
    }
    auto s = stripCheckAnnotations(san);

    if (san == "0-0-0") {
        return resolveCastle(true, gameState);
    }
    if (san == "0-0") {
        return resolveCastle(false, gameState);
    }

    //parse promotion suffix
    std::optional<PieceType> promotionPiece;
    if (s.size() >= 2 && s[s.size()-2] == '=') {
        promotionPiece = charToPieceType(s.back());
        s.resize(s.size() - 2);
    }

    //moving piece type -> must be upper; no letter = pawn
    PieceType movingType = PieceType::PAWN;
    size_t idx = 0;
    if (!s.empty() && std::isupper(s[0]) && s[0] != 'x') {
        movingType = charToPieceType(s[0]);
        idx = 1;
    }

    // parse capture
    bool isCapture = false;
    s = s.substr(idx);
    {
        auto xpos = s.find('x');
        if (xpos != std::string::npos) {
            isCapture = true;
            s.erase(xpos, 1);
        }
    }

    if (s.size() < 2) throw InvalidSanException("SAN destination coord too short: " + san);

    const Coordinate destination = coordinateFromString(s.substr(s.size() - 2, s.size() -1));


    // Ambiguous source prefix: null/ file/ file + rank
    std::string disambig = s.substr(0, s.size() - 2);
    std::optional<int> disambigFile, disambigRank;
    for (char c : disambig) {
        if (c >= 'a' && c <= 'h') disambigFile = c - 'a';
        else if (c >= '1' && c <= '8') disambigRank = c - '1';
        else throw InvalidSanException("Unexpected ambiguous source char in: " + san);
    }
    Coordinate source{disambigRank.value(), disambigFile.value()};

    // matching legal moves
    std::vector<Move> candidates;
    // tod add disambig
    auto piece = gameState.board.at(source);
    if(!piece || piece->color() != gameState.currentPlayer || piece->type() != movingType){
        throw InvalidSanException("Source piece invalid");
    }
    for (auto moves = MoveGenerator::calculatePossibleMoves(source, gameState); auto& m : moves) {
        if (m.destination != destination) continue;
        if (m.isCapture != isCapture)
        if(promotionPiece) {
            if(m.moveType != MoveType::PROMOTION) continue;
            m.promotionPieceType = promotionPiece.value();

        }
    candidates.push_back(m);
    }

    if (candidates.empty())
        throw InvalidSanException("No legal move found for SAN: " + san);
    if (candidates.size() > 1)
        throw InvalidSanException("Ambiguous SAN (multiple matches): " + san);

    return candidates[0];
}


Coordinate san::coordinateFromString(const std::string &raw)
{
    if (raw.size() < 2) {
        throw InvalidSanException("Coordinate too short: " + raw);
    }
    if (raw[0] < 'a' || raw[0] > 'h' ||
        raw[1] < '1' || raw[1] > '8') {
        throw InvalidSanException("File must be between 'a' and 'h' and rank between '1' and '8'");
    }
    int file = raw[0] - 'a';
    int rank = raw[1] - '1';
    return {rank, file};
}
