#pragma once
#include <QObject>
#include <memory>
#include "game.h"
#include "boardmodel.h"
#include "chessEnums.h"
#include "IGameObserver.h"

using MovePairList = QList<QVariantMap>;
Q_DECLARE_METATYPE(MovePairList)

class GameController : public QObject, public IGameObserver {
    Q_OBJECT
    Q_PROPERTY(BoardModel* board READ board CONSTANT) // QML expects raw pointer for qt property
    Q_PROPERTY(Chess::Enums::Color currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(MovePairList movesList READ movesList NOTIFY movesChanged)
    Q_PROPERTY(bool pendingPromotion READ pendingPromotion NOTIFY pendingPromotionChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool isGameOngoing READ isGameOngoing NOTIFY statusChanged)
    Q_PROPERTY(int winner READ winner NOTIFY statusChanged)
    Q_PROPERTY(QString drawCause READ drawCause  NOTIFY statusChanged)

public:
    explicit GameController(QObject* parent = nullptr);

    // Q_PROPERTY read
    BoardModel* board() const;
    Chess::Enums::Color currentPlayer() const;
    MovePairList movesList() const;
    bool pendingPromotion() const;
    QString status() const;
    bool isGameOngoing() const;
    int winner() const;
    QString drawCause() const;

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void restartGame();
    Q_INVOKABLE void promotePawn(Chess::Enums::PieceType pieceType);
    Q_INVOKABLE void selectSquare(int index);

    int selectedSquare() const;
    QList<QPair<int,int>> highlightedSquares() const;
    QString svgPathForSquare(int sq) const;

signals:
    void currentPlayerChanged();
    void statusChanged();
    void movesChanged();
    void pendingPromotionChanged();
    void boardChanged();
    void squareSelectionChanged();

private:
    std::unique_ptr<Game> game_;
    std::unique_ptr<BoardModel> boardModel_;

    int selectedSquare_ = -1;
    QList<QPair<int,int>> highlightedSquares_; //todo: change to QSet<int>
    std::vector<Move> activeMoves_;
    MovePairList movesList_;

    // IGameObserver interface
    void onMoveExecuted();

    void selectSourceSquare(int index);
    void selectDestinationSquare(int index);
    void clearSelection();
    void notifyBoardModel();
    void appendMove();
    void clearMoves();

    static Coordinate indexToCoordinate(int index)
    {
        int file = index % 8;
        int rank = Board::BOARD_SIZE - 1 - (index / 8);
        return {rank, file};
    }
};