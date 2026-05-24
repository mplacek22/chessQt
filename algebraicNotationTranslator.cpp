#include "algebraicNotationTranslator.h"

#include "InvalidSANException.h"
#include "move_generator.h"

std::string AlgebraicNotationTranslator::toSAN(const Move &move)
{
    if (move.moveType == MoveType::CASTLE_KINGSIDE)  return "O-O";
    if (move.moveType == MoveType::CASTLE_QUEENSIDE) return "O-O-O";

    std::string notation;
    if (move.movingPiece->type() != PieceType::PAWN) {
        notation += move.movingPiece->name();
    }

    // if(move.isAmbiguous) {
    //     notation += coordinateToSAN(move.source);
    // }
    notation += coordinateToSAN(move.source);


    if (move.capturedPiece) {
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

    switch (move.gameStatus) {
    case GameStatus::SINGLE_CHECK: notation += '+'; break;
    case GameStatus::DOUBLE_CHECK: notation += '+'; break;
    case GameStatus::CHECK_MATE: notation += '#'; break;
    default: break;
    }

    return notation;
}

Move AlgebraicNotationTranslator::fromSAN(const std::string& san, const Board& board, const GameState& gameState)
{
    if (san.empty()) {
        throw InvalidSanException("Empty string");
    }
    auto s = stripCheckAnnotations(san);

    if (san == "0-0-0") {
        return resolveCastle(true, board, gameState);
    }
    if (san == "0-0") {
        return resolveCastle(false, board, gameState);
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

    Coordinate destination = coordinateFromString(s.substr(s.size() - 2, s.size() -1));


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
    std::vector<std::shared_ptr<Move>> candidates;
    // tod add disambig
    auto piece = board.getPieceAt(source);
    if(!piece || piece->color() != gameState.currentPlayer || piece->type() != movingType){
        throw InvalidSanException("Source piece invalid");
    }
    auto moves = MoveGenerator::calculatePossibleMoves(board, source, gameState);
    for (auto& m : moves) {
        if (m->destination != destination) continue;
        // if (m->capturedPiece !=)
        if(promotionPiece) {
            if(m->moveType != MoveType::PROMOTION) continue;
            m->promotionPieceType = promotionPiece.value();

        }
        candidates.push_back(m);
    }

    // for (int rank = 0; rank < 8; ++rank) {
    //     for (int file = 0; file < 8; ++file) {
    //         Coordinate src(rank, file);
    //         auto piece = board.getPieceAt(src);
    //         if (!piece) continue;
    //         if (piece->color() != gameState.currentPlayer) continue;
    //         if (piece->type() != movingType) continue;

    //         // Apply disambiguation
    //         if (disambigFile && file != *disambigFile) continue;
    //         if (disambigRank && rank != *disambigRank) continue;

    //         auto moves = MoveGenerator::calculatePossibleMoves(board, src, gameState);
    //         for (auto& m : moves) {
    //             if (m->destination.rank != destination.rank) continue;
    //             if (m->destination.file != destination.file) continue;
    //             if (promotionPiece && m->promotionPieceType != promotionPiece) continue;
    //             candidates.push_back(m);
    //         }
    //     }
    // }

    if (candidates.empty())
        throw InvalidSanException("No legal move found for SAN: " + san);
    if (candidates.size() > 1)
        throw InvalidSanException("Ambiguous SAN (multiple matches): " + san);

    return *candidates[0];

    throw InvalidSanException("Invalid value");
}

Coordinate AlgebraicNotationTranslator::destinationFromSAN(const std::string &raw, const Color color)
{
    if (raw.empty()) {
        throw InvalidSanException("Empty string");
    }
    std::string san = raw;

    // Strip check/checkmate suffix
    if (san.back() == '+' || san.back() == '#'){
        san.pop_back();
    }

    // Castling
    if (san == "0-0") {
        return { color == Color::WHITE ? 0 : 7, 6 };
    }
    if (san == "0-0-0"){
        return { color == Color::WHITE ? 0 : 7, 2 };
    }

    // Strip promotion suffix
    if (san.size() >= 2 && san[san.size()-2] == '=') {
        san.resize(san.size() - 2);
    }

    if (san.size() < 2) {
        throw InvalidSanException("SAN too short: " + raw);
    }

    return coordinateFromString(san.substr(san.size() - 2));
}

Coordinate AlgebraicNotationTranslator::coordinateFromString(const std::string &raw)
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

Coordinate AlgebraicNotationTranslator::sourceFromSAN(const std::string &raw, const Color color, const Board &board)
{
    if (raw.empty()) {
        throw InvalidSanException("Empty string");
    }
    std::string san = raw;
    // Strip check/checkmate suffix
    if (!san.empty() && (san.back() == '+' || san.back() == '#'))
        san.pop_back();

    // Castling
    if (san == "0-0" || san == "0-0-0")
        return { color == Color::WHITE ? 0 : 7, 4 };

    // Strip promotion suffix
    if (san.size() >= 2 && san[san.size()-2] == '=') {
        san.resize(san.size() - 2);
    }

    if (san.size() < 2) {
        throw InvalidSanException("SAN too short: " + raw);
    }

    PieceType pieceType = PieceType::PAWN;
    if (!san.empty() && std::isupper(static_cast<unsigned char>(san[0]))) {
        switch (san[0]) {
        case 'N': pieceType = PieceType::KNIGHT; break;
        case 'B': pieceType = PieceType::BISHOP; break;
        case 'R': pieceType = PieceType::ROOK;   break;
        case 'Q': pieceType = PieceType::QUEEN;  break;
        case 'K': pieceType = PieceType::KING;   break;
        default:  throw InvalidSanException(std::string("Unknown piece '") + san[0] + "'");
        }
        san.erase(san.begin());
    }

    // Strip capture mark
    if (!san.empty() && san.back() == 'x')
        san.pop_back();

    // Parse disambiguation hint what remains is "", "f", "r", or "fr"
    int hintFile = -1, hintRank = -1;
    //strip desination
    san.resize(san.size() - 2);
    if (san.size() == 2) {
        // Full source square encoded in SAN — no board scan needed
        return coordinateFromString(san);
    } else if (san.size() == 1) {
        char c = san[0];
        if      (c >= 'a' && c <= 'h') hintFile = c - 'a';
        else if (c >= '1' && c <= '8') hintRank = c - '1';
        else throw InvalidSanException(std::string("Unexpected disambiguation char '") + c + "'");
    } else if (!san.empty()) {
        throw InvalidSanException("Unexpected leftover '" + san + "' in: '" + raw + "'");
    }

    // Scan board for candidates
    std::vector<Coordinate> candidates;
    for (int r = 0; r < Board::BOARD_SIZE; ++r) {
        for (int f = 0; f < Board::BOARD_SIZE; ++f) {
            if (hintFile != -1 && f != hintFile) continue;
            if (hintRank != -1 && r != hintRank) continue;

            auto piece = board.getPieceAt({ r, f });
            if (!piece || piece->type() != pieceType || piece->color() != color) continue;
            // if (!board({ r, f }, dest)) continue;
            // if (MoveGenerator::calculatePseudoLegalMoves(board, {r,f}, GameStatus::IN_PROGRESS))
            candidates.push_back({ r, f });
        }
    }

    // Resolve
    if (candidates.empty())
        throw InvalidSanException("Source piece not found");
    if (candidates.size() > 1)
        throw InvalidSanException("Ambiguous move");

    return candidates[0];
}

std::string AlgebraicNotationTranslator::stripCheckAnnotations(const std::string &raw)
{
    if (!raw.empty() && (raw.back() == '+' || raw.back() == '#'))
        return raw.substr(0, raw.size() - 1);
    return raw;
}

std::string AlgebraicNotationTranslator::coordinateToSAN(const Coordinate& coordinate)
{
    return {fileSAN(coordinate.file), rankSAN(coordinate.rank)};
}

char AlgebraicNotationTranslator::rankSAN(int rank)
{
    if(rank < 0 || rank >= Board::BOARD_SIZE){
        throw InvalidSanException("Invalid rank: " + std::to_string(rank));
    }
    return static_cast<char>('1' + rank);
}

char AlgebraicNotationTranslator::fileSAN(int file)
{
    if(file < 0 || file >= Board::BOARD_SIZE){
        throw InvalidSanException("Invalid rank: " + std::to_string(file));
    }
    return static_cast<char>('a' + file);
}

Move AlgebraicNotationTranslator::resolveCastle(bool isKingSide, const Board &board, const GameState &gameState)
{
    int rank = gameState.currentPlayer == Color::WHITE ? 0 : 7;
    Coordinate source(rank, 4);

    if(isKingSide) {
        if(MoveGenerator::canCastleKingSide(board, gameState)) {
            auto move = Move{source, Coordinate{rank , 6}, gameState.currentPlayer};
            move.moveType = MoveType::CASTLE_KINGSIDE;
            move.movingPiece = board.getPieceAt(source);
            return move;
        }
        throw InvalidSanException("Can't castle king side.");
    }
    if(MoveGenerator::canCastleKingSide(board, gameState)) {
        auto move = Move{source, Coordinate{rank , 2}, gameState.currentPlayer};
        move.moveType = MoveType::CASTLE_KINGSIDE;
        move.movingPiece = board.getPieceAt(source);
        return move;
    }
    throw InvalidSanException("Can't castle queen side.");
}

PieceType AlgebraicNotationTranslator::charToPieceType(char c)
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
