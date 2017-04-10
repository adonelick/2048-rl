
#include "game.hpp"


Game::Game()
    : score(0),
      state()
{
    // Nothing to do here...
}

unsigned int Game::getScore() const
{
    return score;
}


unsigned int Game::getMaxTile() const
{
    return state.getMaxTile();
}


State Game::getState() const
{
    return state;
}


unsigned int Game::takeAction(Action a)
{
    unsigned int reward;

    if (a == UP) {
        reward = state.slideUp();
    } else if (a == DOWN) {
        reward = state.slideDown();
    } else if (a == LEFT) {
        reward = state.slideLeft();
    } else {
        reward = state.slideRight();
    }

    state.insertNewTile();
    score += reward;

    return reward;
}


unsigned int Game::takeAction(Action a, State& afterState)
{
    unsigned int reward;

    if (a == UP) {
        reward = state.slideUp();
    } else if (a == DOWN) {
        reward = state.slideDown();
    } else if (a == LEFT) {
        reward = state.slideLeft();
    } else {
        reward = state.slideRight();
    }

    afterState = State(state);
    state.insertNewTile();

    score += reward;
    return reward;
}


unsigned int Game::pretendTakeAction(Action a, State& afterState) const
{
    unsigned int reward;
    afterState = state;

    if (a == UP) {
        reward = afterState.slideUp();
    } else if (a == DOWN) {
        reward = afterState.slideDown();
    } else if (a == LEFT) {
        reward = afterState.slideLeft();
    } else {
        reward = afterState.slideRight();
    }

    return reward;
}

unsigned int Game::getActions(Action actions[NUM_ACTIONS]) const
{
    State upState(state);
    State downState(state);
    State leftState(state);
    State rightState(state);

    upState.slideUp();
    downState.slideDown();
    leftState.slideLeft();
    rightState.slideRight();

    unsigned int numActions = 0;

    if (upState != state) {
        actions[numActions++] = UP;
    }

    if (downState != state) {
        actions[numActions++] = DOWN;
    }

    if (leftState != state) {
        actions[numActions++] = LEFT;
    }

    if (rightState != state) {
        actions[numActions++] = RIGHT;
    }

    return numActions;
}
