#pragma once
#include <QObject>
#include <memory>
#include "boardmodel.h"
#include "chessEnums.h"
#include "game_client.h"

using MovePairList = QList<QVariantMap>;
Q_DECLARE_METATYPE(MovePairList)
/**
 * @brief The GameController class is a QT/QML-facing controller that bridges @c IGameClient and the board model.
 *
 * GameController is the single object exposed to QML.
 *
 * It owns a @c BoardModel,implements @c IGameClient to receive push notifications from the game logic,
 * and exposes @c Q_INVOKABLE methods for player interactions (start, restart, move (selectSquare), promote).
 *
 */
class GameController : public QObject, public IGameClient {
    Q_OBJECT

    /// @brief Pointer to the board model.
    Q_PROPERTY(BoardModel* board READ board CONSTANT) // QML expects raw pointer for qt property

    /// @brief Color of the player whose turn it is.
    Q_PROPERTY(Chess::Enums::Color currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)

    /// @brief  Move list formatted for the QML move history panel.
    Q_PROPERTY(MovePairList movesList READ movesList NOTIFY movesChanged)

    /// @brief @c true while a pawn promotion choice is awaited from the player.
    Q_PROPERTY(bool pendingPromotion READ pendingPromotion NOTIFY pendingPromotionChanged)

    /// @brief Game status string
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

    /// @brief @c true while the game is in a playable state.
    Q_PROPERTY(bool isGameOngoing READ isGameOngoing NOTIFY statusChanged)

    /// @brief Integer representation of the winning @c Color, or -1 if game is not won.
    Q_PROPERTY(int winner READ winner NOTIFY statusChanged)

    /// @brief String draw cause, or empty string if the game is not drawn.
    Q_PROPERTY(QString drawCause READ drawCause  NOTIFY statusChanged)

public:
    explicit GameController(QObject* parent = nullptr);

    GameController(const GameController &) = delete;
    GameController(GameController &&) = delete;
    GameController &operator=(const GameController &) = delete;
    GameController &operator=(GameController &&) = delete;

    // Q_PROPERTY read
    /**
     * @brief board returns the board model for qml use
     * @return board model pointer
     */
    BoardModel *board() const;

    /**
     * @brief currentPlayer returns the color of the player to move.
     * @return Color of the player whose turn it is
     */
    Chess::Enums::Color currentPlayer() const;

    /**
     * @brief movesList returns the move history formatted for the QML move list
     * @return Move history formatted for the QML move list
     */
    MovePairList movesList() const;

    /**
     * @brief pendingPromotion returns @c true if a promotion piece selection is pending.
     * @return @c true if a promotion piece selection is pending.
     */
    bool pendingPromotion() const;

    /**
     * @brief status returns the current game status as a string.
     * @return Current game status as a display string.
     */
    QString status() const;

    /**
     * @brief winner returns the integer value of the winning player color.
     * @return Returns the integer value of the winning color, or -1 if game not won.
     */
    int winner() const;

    /**
     * @brief drawCause returns the draw cause as a display string.
     * @return Returns the draw cause as a display string, or empty string if game is not drawn.
     */
    QString drawCause() const;


    //Q_INVOKABLE actions

    /**
     * @brief startGame starts a new game.
     */
    Q_INVOKABLE void startGame();

    /**
     * @brief restartGame restarts the game, resetting all state.
     */
    Q_INVOKABLE void restartGame();

    /**
     * @brief promotePawn resolves a pending pawn promotion
     * @param pieceType Piece type chosen by the player.
     */
    Q_INVOKABLE void promotePawn(Chess::Enums::PieceType pieceType);

    /**
     * @brief Handles a click on a board square.
     *
     * On the first tap of a friendly piece, selects it and requests legal moves.
     * On the second tap of a highlighted square, submits the move.
     * Tapping twice on a same square clears selection.
     *
     * @param index Flat board index.
     */
    Q_INVOKABLE void selectSquare(int index);

    // data consumed by BoardModel

    /**
     * @brief selectedSquare returns the flat index of the currently selected square
     * @return The flat index of the currently selected square.
     */
    int selectedSquare() const;

    /**
     * @brief highlightedSquares the set of flat indices currently highlighted as legal moves.
     * @return The set of flat indices currently highlighted as legal moves.
     */
    QSet<int> highlightedSquares() const;

    /**
     * @brief svgPathForSquare Returns the SVG resource path for the piece on the given square.
     * @param sq Flat board index.
     * @return Resource path string, or empty string if the square is not occupied.
     */
    QString svgPathForSquare(int sq) const;

    // IGameClient

    /**
     * @brief possibleMovesCalculated Receives legal moves from the game and highlights them on the board.
     * @param moves Legal moves for the previously selected piece.
     */
    void possibleMovesCalculated(std::vector<Move> moves) override;

    /**
     * @brief onGameStateChanged Receives a full game state update from mediator,
     *  refreshes all bound properties and notifies QML.
     *
     * If there is a pending promotion choice -> early return.
     * Updates the cached state.
     * Appends the last move to moves history.
     * Notifies the board model.
     *
     * @param gameState Current game state after the latest move.
     */
    void onGameStateChanged(const GameState& gameState) override;

    /**
     * @brief onGameWon Receives a win notification,
     *  updates the status properties and notifies QML.
     * @param winner Color of the winning player.
     */
    void onGameWon(Color winner) override;

    /**
     * @brief onGameDrawn Receives a draw notification,
     * updates the status properties and notifies QML.
     * @param drawCause Cause of the draw.
     */
    void onGameDrawn(DrawCause drawCause) override;

    /**
     * @brief Receives a promotion-pending state change and notifies QML.
     * @param pendingPromotion @c true when a promotion choice is required.
     */
    void onPendingPromotionChanged(bool pendingPromotion) override;

signals:
    /// Emitted when the player's turn changes.
    void currentPlayerChanged();

    /// Emitted when the game status changes.
    void statusChanged();

    /// Emitted when the move history changes.
    void movesChanged();

    /// Emitted when promotion-pending state changes.
    void pendingPromotionChanged();

    /// Emitted when the board reference changes.
    void boardChanged();

    /// Emitted when the selected square changes.
    void squareSelectionChanged();


private:
    std::unique_ptr<BoardModel> boardModel_;
    int selectedSquare_ = -1;
    QSet<int> highlightedSquares_;
    MovePairList movesList_;

    void selectSourceSquare(int index);

    void selectDestinationSquare(int index);

    void clearSelection();

    void notifyBoardModel();

    void clearMoves();

    void appendMove(const Move& move);

    static Coordinate indexToCoordinate(int index);

    static int coordinateToIndex(const Coordinate& coordinate);
};