
#ifndef GAME_H
#define GAME_H 1

#include "state.hpp"


#define NUM_ACTIONS 4

enum Action {UP, DOWN, LEFT, RIGHT};



class Game
{

private:

    /* Current score of the game */
    unsigned int score;

    /* Current state of the game */
    State state;

public:

    Game();

    unsigned int getScore() const;

    unsigned int getMaxTile() const;

    unsigned int takeAction(Action a);

    unsigned int takeAction(Action a, State& afterState);

    unsigned int pretendTakeAction(Action a, State& afterState) const;

    State getState() const;

    unsigned int getActions(Action actions[NUM_ACTIONS]) const;
};


#endif