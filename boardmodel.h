#pragma once

#include <QAbstractListModel>

class GameController;
/**
 * @brief The BoardModel class is a QAbstractListModel exposing  64 chessboard squares to QML.
 *
 * It represents the chess board as a flat list of 64 items.
 * The list starts at the top left corner (a8 = 0) and ends at the bottom right corner (h1 = 63),
 * reading fromf left to right.
 *
 * It maps into a QML @c GridView with 8 columns.
 */
class BoardModel : public QAbstractListModel {
    Q_OBJECT
public:
    /**
     * @brief The Roles enum exposes custom roles to QML delegates.
     */
    enum Roles {
        IsHighlightedRole = Qt::UserRole + 1, /// Square is a legal move destination
        IsSelectedRole, /// Square holds the selected piece to be moved.
        SvgPathRole /// Resource path to the piece svg file.
    };

    /**
     * @brief BoardModel constructs the model with its data source
     * @param controller Nonowning pointer to the @c GameController, which is the data source.
     * @param parent Optional Qt parent object
     */
    explicit BoardModel(GameController* controller, QObject* parent = nullptr);

    /**
     * @brief rowCount returns the number of squares on the board.
     * @param parent Unused; exists to satisfy the @c QAbstractListModel interface.
     * @return 64 if parent is valid, 0 otherwise.
     */
    int rowCount(const QModelIndex& parent = {}) const override;

    /**
     * @brief data returns the data for givenn square and role
     * @param index Index of the queried square.
     * @param role One of the @c Roles enum.
     * @return @c QVariant containing:
     * - @c bool for @c IsHighlightedRole and @c IsSelectedRole
     * - @c QString for @c SvgPathRole
     */
    QVariant data(const QModelIndex& index, int role) const override;

    /**
     * @brief roleNames returns the mapping of role enum to QML-accessible names.
     * @return @c "isHighlighted", @c "isSelected", and @c "svgPath" exposed to QML.
     */
    QHash<int, QByteArray> roleNames() const override;

private:
    GameController* gameController_;
};