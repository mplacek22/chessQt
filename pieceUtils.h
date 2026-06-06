#pragma once
#include <QObject>
#include <QtQml/qqml.h>
#include "Color.h"
#include "PieceType.h"
#include "drawCause.h"
#include "gameStatus.h"

class PieceUtils : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

public:
    explicit PieceUtils(QObject* parent = nullptr);

    static PieceUtils* create(QQmlEngine*, QJSEngine*);

    Q_INVOKABLE QString imageSource(int color, int type) const;

    static QString imageSource(Color color, PieceType type);

    static QString gameStatusToString(GameStatus gameStatus);

    Q_INVOKABLE static QString drawCauseToString(DrawCause drawCause);

private:
    static QString colorToString(Color c);

    static QString pieceTypeToString(PieceType type);
};