/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digial Signal Processing
 * Final Project
 */

#include "game.hpp"



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
    bool validAction = true;

    /* Slide and combine the tiles based on the current move */
    if (a == UP) {
        reward = state.slideUp();
    } else if (a == DOWN) {
        reward = state.slideDown();
    } else if (a == LEFT) {
        reward = state.slideLeft();
    } else if (a == RIGHT) {
        reward = state.slideRight();
    } else {
        reward = 0;
        validAction = false;
    }

    /* Only insert a tile if the action was valid */
    if (validAction) {
        state.insertNewTile();
    }

    score += reward;
    return reward;
}


unsigned int Game::takeAction(Action a, State& afterState)
{
    unsigned int reward;
    bool validAction = true;

    /* Slide and combine the tiles based on the current move */
    if (a == UP) {
        reward = state.slideUp();
    } else if (a == DOWN) {
        reward = state.slideDown();
    } else if (a == LEFT) {
        reward = state.slideLeft();
    } else if (a == RIGHT) {
        reward = state.slideRight();
    } else {
        reward = 0;
        validAction = false;
    }

    /* Copy the state to the afterState variable (for return), and 
     * insert a tile if the action we took was a valid one.
     */
    afterState = State{state};
    if (validAction) {
        state.insertNewTile();
    }

    score += reward;
    return reward;
}


unsigned int Game::pretendTakeAction(Action a, State& afterState) const
{
    unsigned int reward;
    afterState = state;

    /* Slide and combine the tiles based on the current move.
     * Note that we execute the move on the afterstate variable, not 
     * the current game state. This way, we don't affect the game state.
     */
    if (a == UP) {
        reward = afterState.slideUp();
    } else if (a == DOWN) {
        reward = afterState.slideDown();
    } else if (a == LEFT) {
        reward = afterState.slideLeft();
    } else if (a == RIGHT) {
        reward = afterState.slideRight();
    } else {
        reward = 0;
    }

    /* We do not update the game's score here, as we are only 
     * pretending to take this action.
     */
    return reward;
}


unsigned int Game::getActions(Action actions[NUM_ACTIONS]) const
{
    /* Create some new variables to create and store afterstates 
     * for each of the game's four actions.
     */
    State upState{state};
    State downState{state};
    State leftState{state};
    State rightState{state};

    upState.slideUp();
    downState.slideDown();
    leftState.slideLeft();
    rightState.slideRight();

    unsigned int numActions = 0;

    /* If the afterstate for an action is different than the current 
     * state, then the action leading to that afterstate is a valid
     * action to take. Otherwise, we omit it from our action array.
     */
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
