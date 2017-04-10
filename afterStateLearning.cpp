/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digial Signal Processing
 * Final Project
 */

#include <iostream>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include <time.h>
#include "state.hpp"
#include "game.hpp"
#include "ntnn.hpp"

using namespace std;

#define NUM_TUPLES 17
#define TUPLE_LENGTH 4

#define GAMES 10000
#define ALPHA 0.01


/**
 * This function computes the best action to take given the current game
 * state, an array of possible actions, and the current value function. 
 * The function chooses the action which maximizes the sum of the value 
 * of the next afterstate and the obtained reward.
 *
 * :param state: Reference to the current state
 * :param actions: Array of available actions in the current state
 * :param numActions: Number of actions in the actions array
 * :param V: Current value function
 *
 * :return: The best action to take in the current state
 */
Action getBestAction(const State& state, Action* actions, int numActions, const NTNN& V)
{
    Action bestAction;
    Action a;
    double bestValue = -numeric_limits<double>::infinity();

    unsigned int reward;
    double value;

    for (int i = 0; i < numActions; ++i) {
        
        a = actions[i];
        State afterState(state);

        /* Compute the afterstate based on the action */
        if (a == UP) {
            reward = afterState.slideUp();
        } else if (a == DOWN) {
            reward = afterState.slideDown();
        } else if (a == LEFT) {
            reward = afterState.slideLeft();
        } else {
            reward = afterState.slideRight();
        }

        /* Compute the value of the action, and check if 
         * the action compares favorably to previous results.
         */
        value = double(reward) + V.evaluate(afterState);
        if (value > bestValue) {
            bestValue = value;
            bestAction = a;
        }

    }

    return bestAction;
}


/**
 * This is the function which runs the program.
 *
 * :param argc: Number of command line arguments
 * :param argv: Command line arguments
 *
 * :return: Error code (0 = no error)
 */
int main(int argc, char **argv)
{
    /* Initialize the random seed */
    srand(time(NULL));

    /* Declare the value function */
    NTNN V(NUM_TUPLES, TUPLE_LENGTH, ALPHA);

    /* Add the tuples to the n-tuple regression network */
    unsigned int tuples[NUM_TUPLES][TUPLE_LENGTH] = {
                                                      {0, 1, 2, 3}, {4, 5, 6, 7}, 
                                                      {8, 9, 10, 11}, {12, 13, 14, 15},
                                                      {0, 4, 8, 12}, {1, 5, 9, 13}, 
                                                      {2, 6, 10, 14}, {3, 7, 11, 15},
                                                      {0, 1, 4, 5}, {1, 2, 5, 6}, 
                                                      {2, 3, 6, 7}, {4, 5, 8, 9}, 
                                                      {5, 6, 9, 10}, {6, 7, 10, 11},
                                                      {8, 9, 12, 13}, {9, 10, 13, 14},
                                                      {10, 11, 14, 15}
                                                    };
    for (int i = 0; i < NUM_TUPLES; ++i) {
        V.addTuple(tuples[i], TUPLE_LENGTH);
    }

    
    Action actions[4];
    unsigned int numActions;
    
    for (unsigned int gameIndex = 0; gameIndex < GAMES; ++gameIndex)
    {
        Game game;
        numActions = game.getActions(actions);

        State state;
        State afterState;
        State nextState;
        State nextAfterState;
        Action bestAction;
        Action nextBestAction;
        unsigned int reward;
        unsigned int rNext;

        while (numActions > 0)
        {
            state = game.getState();
            bestAction = getBestAction(state, actions, numActions, V);

            reward = game.takeAction(bestAction, afterState);
            nextState = game.getState();
            numActions = game.getActions(actions);

            /* Start the learning part of the algorithm */
            if (numActions > 0) {
                nextBestAction = getBestAction(nextState, actions, numActions, V);
                rNext = game.pretendTakeAction(nextBestAction, nextAfterState);

                V.train(afterState, double(rNext) + V.evaluate(nextAfterState));
            }
        }

        cout << game.getScore() << endl;

        if (game.getMaxTile() >= 2048) {
            cout << "Win! " << game.getMaxTile() << endl;
        }

    }

    return 0;
}