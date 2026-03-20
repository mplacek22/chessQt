#include "chessboard.h"

ChessBoard::ChessBoard(QObject* parent) : QAbstractListModel(parent), m_board(64) {
    setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

int ChessBoard::rowCount(const QModelIndex&) const { return 64; }

QVariant ChessBoard::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= 64) return {};
    int i = index.row();
    int file = i % 8;
    int rank = i / 8;
    switch (role) {
    case PieceRole:  return m_board[i];
    case FileRole:   return file;
    case RankRole:   return rank;
    }
    return {};
}

QHash<int, QByteArray> ChessBoard::roleNames() const {
    return {
            { PieceRole, "piece" },
            { FileRole,  "file"  },
            { RankRole,  "rank"  },
            };
}

QString ChessBoard::pieceAt(int file, int rank) const {
    if (file < 0 || file > 7 || rank < 0 || rank > 7) return "";
    return m_board[toIndex(file, rank)];
}

QString ChessBoard::fen() const { return m_fen; }

void ChessBoard::setFen(const QString& fen) {
    if (m_fen == fen) return;
    m_fen = fen;
    loadFen(fen);
    emit fenChanged();
    emit dataChanged(createIndex(0,0), createIndex(63,0));
}

void ChessBoard::loadFen(const QString& fen) {
    m_board.fill("", 64);
    QStringList parts = fen.split(' ');
    QString placement = parts[0];
    m_whiteTurn = (parts.size() > 1 && parts[1] == "b") ? false : true;

    int rank = 7; // FEN starts from rank 8
    int file = 0;
    static const QMap<QChar, QString> fenMap = {
                                                {'K',"wK"},{'Q',"wQ"},{'R',"wR"},{'B',"wB"},{'N',"wN"},{'P',"wP"},
                                                {'k',"bK"},{'q',"bQ"},{'r',"bR"},{'b',"bB"},{'n',"bN"},{'p',"bP"},
                                                };
    for (QChar c : placement) {
        if (c == '/') { rank--; file = 0; }
        else if (c.isDigit()) { file += c.digitValue(); }
        else {
            if (fenMap.contains(c)) m_board[toIndex(file, rank)] = fenMap[c];
            file++;
        }
    }
}

QString ChessBoard::buildFen() const {
    static const QMap<QString, QChar> pieceMap = {
                                                  {"wK",'K'},{"wQ",'Q'},{"wR",'R'},{"wB",'B'},{"wN",'N'},{"wP",'P'},
                                                  {"bK",'k'},{"bQ",'q'},{"bR",'r'},{"bB",'b'},{"bN",'n'},{"bP",'p'},
                                                  };
    QString fen;
    for (int rank = 7; rank >= 0; rank--) {
        int empty = 0;
        for (int file = 0; file < 8; file++) {
            QString p = m_board[toIndex(file, rank)];
            if (p.isEmpty()) { empty++; }
            else {
                if (empty > 0) { fen += QString::number(empty); empty = 0; }
                fen += pieceMap.value(p, '?');
            }
        }
        if (empty > 0) fen += QString::number(empty);
        if (rank > 0) fen += '/';
    }
    fen += m_whiteTurn ? " w" : " b";
    fen += " KQkq - 0 1"; // simplified: no castling/en-passant tracking
    return fen;
}

bool ChessBoard::isValidMove(int ff, int fr, int tf, int tr) const {
    const QString& moving = m_board[toIndex(ff, fr)];
    const QString& target = m_board[toIndex(tf, tr)];
    if (moving.isEmpty()) return false;
    // Can't capture own piece
    if (!target.isEmpty() && moving[0] == target[0]) return false;
    // Turn check
    if (m_whiteTurn && !isWhitePiece(moving)) return false;
    if (!m_whiteTurn && !isBlackPiece(moving)) return false;

    QString type = moving.mid(1);
    int df = tf - ff, dr = tr - fr;
    int adf = qAbs(df), adr = qAbs(dr);

    if (type == "P") {
        int dir = isWhitePiece(moving) ? 1 : -1;
        int startRank = isWhitePiece(moving) ? 1 : 6;
        // Forward move
        if (df == 0 && dr == dir && target.isEmpty()) return true;
        // Double push
        if (df == 0 && dr == 2*dir && fr == startRank && target.isEmpty()
            && m_board[toIndex(ff, fr+dir)].isEmpty()) return true;
        // Capture
        if (adf == 1 && dr == dir && !target.isEmpty()) return true;
        return false;
    }
    if (type == "N") return (adf==1 && adr==2) || (adf==2 && adr==1);
    if (type == "K") return adf <= 1 && adr <= 1;
    if (type == "R") {
        if (df != 0 && dr != 0) return false;
        // Check path clear
        int stepF = (df == 0) ? 0 : df/qAbs(df);
        int stepR = (dr == 0) ? 0 : dr/qAbs(dr);
        for (int f=ff+stepF, r=fr+stepR; f!=tf||r!=tr; f+=stepF, r+=stepR)
            if (!m_board[toIndex(f,r)].isEmpty()) return false;
        return true;
    }
    if (type == "B") {
        if (adf != adr) return false;
        int stepF = df/adf, stepR = dr/adr;
        for (int f=ff+stepF, r=fr+stepR; f!=tf||r!=tr; f+=stepF, r+=stepR)
            if (!m_board[toIndex(f,r)].isEmpty()) return false;
        return true;
    }
    if (type == "Q") {
        // Queen = R + B
        if (df == 0 || dr == 0) {
            int stepF = (df==0)?0:df/adf, stepR = (dr==0)?0:dr/adr;
            for (int f=ff+stepF, r=fr+stepR; f!=tf||r!=tr; f+=stepF, r+=stepR)
                if (!m_board[toIndex(f,r)].isEmpty()) return false;
            return true;
        }
        if (adf == adr) {
            int stepF = df/adf, stepR = dr/adr;
            for (int f=ff+stepF, r=fr+stepR; f!=tf||r!=tr; f+=stepF, r+=stepR)
                if (!m_board[toIndex(f,r)].isEmpty()) return false;
            return true;
        }
        return false;
    }
    return false;
}

void ChessBoard::selectSquare(int file, int rank) {
    int idx = toIndex(file, rank);
    if (m_selectedIndex == -1) {
        // Select if own piece
        const QString& p = m_board[idx];
        if (p.isEmpty()) return;
        if (m_whiteTurn && !isWhitePiece(p)) return;
        if (!m_whiteTurn && !isBlackPiece(p)) return;
        m_selectedIndex = idx;
        emit selectedIndexChanged();
    } else {
        int fromFile = m_selectedIndex % 8;
        int fromRank = m_selectedIndex / 8;
        if (isValidMove(fromFile, fromRank, file, rank)) {
            beginResetModel();
            m_board[idx] = m_board[m_selectedIndex];
            m_board[m_selectedIndex] = "";
            m_whiteTurn = !m_whiteTurn;
            m_fen = buildFen();
            endResetModel();
            emit fenChanged();
            emit moveMade(fromFile, fromRank, file, rank);
        }
        m_selectedIndex = -1;
        emit selectedIndexChanged();
    }
}