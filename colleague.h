#ifndef COLLEAGUE_H
#define COLLEAGUE_H

#include "i_game_mediator.h"


class Colleague
{
public:
    void setMediator(IGameMediator* mediator);
protected:
    IGameMediator* mediator_ = nullptr;
};

#endif // COLLEAGUE_H
