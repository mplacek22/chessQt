#pragma once

#include <Move.h>

class IGameObserver{
public:
    virtual ~IGameObserver() = default;

private:
    virtual void onMoveExecuted() {}
    // virtual void onGameStarted() {}
    // virtual void onGameOver() {}
    // virtual void onBoardChanged() {}
};
