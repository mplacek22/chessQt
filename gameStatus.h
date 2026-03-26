#pragma once

enum class GameStatus {
    NEW,
    IN_PROGRESS,
    CHECK,
    CHECK_MATE,
    STALE_MATE,
    DRAW,
    WHITE_WIN,
    BLACK_WIN
};