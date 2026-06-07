#pragma once

#include "i_game_mediator.h"

/**
 * @class Colleague
 * @brief Base class for objects participating in mediator communication.
 */
class Colleague {
public:
    virtual ~Colleague() = default;

    /**
    * @brief Assigns the mediator used by this object.
    * @param mediator Pointer to the game mediator.
     */
    void setMediator(IGameMediator *mediator);

protected:
    /// Pointer to the associated game mediator.
    IGameMediator *mediator_ = nullptr;
};
