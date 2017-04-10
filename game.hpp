/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digial Signal Processing
 * Final Project
 */

#ifndef GAME_H
#define GAME_H 1

#include "state.hpp"

#define NUM_ACTIONS 4


/**
 * This enum defines the possible actions which can be 
 * taken on the game of 2048. 
 */
enum Action {UP, DOWN, LEFT, RIGHT};

/**
 * Class which contains everything you need to run a 2048 game.
 */ 
class Game
{

private:

    /* Current score of the game */
    unsigned int score;

    /* Current state of the game */
    State state;

public:

    /**
     * Constructor for the 2048 game
     *
     * :return: New 2048 game
     */
    Game();

    /**
     * Gets the current score for the game.
     *
     * :return: Current score of the game
     */
    unsigned int getScore() const;

    /**
     * Searches the entire game state and returns the value of the tile
     * with the largest value.
     *
     * :return: Value of the largest tile on the board
     */
    unsigned int getMaxTile() const;

    /**
     * This function allows the player to execute a move on the game.
     * The action should be one of the actions defined for the game 
     * (see the enum above). Otherwise, the game state will stay the same.
     * Finally, this function returns the reward for making the given move.
     *
     * :param a: Action (move) to take on the game
     *
     * :return: Reward for executing the given action (move)
     */
    unsigned int takeAction(Action a);

    /**
     * This function allows the player to execute a move on the game.
     * The action should be one of the actions defined for the game 
     * (see the enum above). Otherwise, the game state will stay the same.
     * Finally, this function returns the reward for making the given move.
     * Additionally, this function returns the "afterstate" for the given 
     * action, that is, the state after the move, but before the random 
     * tile insert.
     *
     * :param a: Action (move) to take on the game
     * :param afterState: Afterstate corresponding to the given move (return value)
     *
     * :return: Reward for executing the given action (move)
     */
    unsigned int takeAction(Action a, State& afterState);

    /**
     * This function allows the player to pretend to execute a move on 
     * the game. That means that the function computes the reward that would
     * be returned for taking this move, and the corresponding afterstate, 
     * but the state of the game DOES NOT change. 
     *
     * :param a: Action (move) to pretend to take on the game
     * :param afterState: Afterstate corresponding to the given move (return value)
     *
     * :return: Reward for executing the given action (move)
     */
    unsigned int pretendTakeAction(Action a, State& afterState) const;

    /**
     * Gets the current state for the game.
     *
     * :return: Current state of the game
     */
    State getState() const;

    /**
     * Gets the possible actions for the current game state. The actions
     * are stored in the array supplied to the function, and the number
     * of possible actions are returned.
     *
     * :param actions: Array that will store the possible actions
     *
     * :return: The number of possible actions for the given game state
     */
    unsigned int getActions(Action actions[NUM_ACTIONS]) const;
};


#endif
