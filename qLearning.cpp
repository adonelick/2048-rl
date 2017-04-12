/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digial Signal Processing
 * Final Project
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
#define NUM_EXPERIMENTS 10


/**
 * This function computes the best action to take given the current game
 * state, an array of possible actions, and the current value function. 
 * The function chooses the action which maximizes value of the appropriate
 * value function.
 *
 * :param state: Reference to the current state
 * :param actions: Array of available actions in the current state
 * :param numActions: Number of actions in the actions array
 * :param valueFunctions: Value functions (one for each action)
 *
 * :return: The best action to take in the current state
 */
Action getBestAction(const State& state, Action* actions, int numActions, const NTNN& V_up, const NTNN& V_down, const NTNN& V_left, const NTNN& V_right)
{
    Action bestAction;
    Action a;
    double bestValue = -numeric_limits<double>::infinity();
    double value;

    for (int i = 0; i < numActions; ++i) {
        
        a = actions[i];

        /* Compute the value of the action, and check if 
         * the action compares favorably to previous results.
         */
        if (a == UP) {
            value = V_up.evaluate(state);
        } else if (a == DOWN) {
            value = V_down.evaluate(state);
        } else if (a == LEFT) {
            value = V_left.evaluate(state);
        } else {
            value = V_right.evaluate(state);
        }

        if (value > bestValue) {
            bestValue = value;
            bestAction = a;
        }

    }

    return bestAction;
}


/**
 * This function runs the temporal difference learning algorithm on 
 * the 2048 game afterstates. The scores and outcomes of the games which
 * the algorithms played are stored in the give, pre-allocated arrays.
 *
 * :param scores: Array in which to scores of the games played
 * :param wins: Array in which to store the games' outcomes (win/loss)
 *
 * :return: (None)
 */
void qLearning(unsigned int* scores, bool* wins)
{
    /* Declare the value functions (one for each action) */
    NTNN V_up(NUM_TUPLES, TUPLE_LENGTH, ALPHA);
    NTNN V_down(NUM_TUPLES, TUPLE_LENGTH, ALPHA);
    NTNN V_left(NUM_TUPLES, TUPLE_LENGTH, ALPHA);
    NTNN V_right(NUM_TUPLES, TUPLE_LENGTH, ALPHA);

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
        V_up.addTuple(tuples[i], TUPLE_LENGTH);
        V_down.addTuple(tuples[i], TUPLE_LENGTH);
        V_left.addTuple(tuples[i], TUPLE_LENGTH);
        V_right.addTuple(tuples[i], TUPLE_LENGTH);
    }

    Action actions[4];
    unsigned int numActions;
    
    for (unsigned int gameIndex = 0; gameIndex < GAMES; ++gameIndex)
    {
        Game game;
        numActions = game.getActions(actions);

        State state;
        State nextState;

        Action bestAction;
        Action nextBestAction;
        unsigned int reward;

        while (numActions > 0)
        {
            state = game.getState();
            bestAction = getBestAction(state, actions, numActions, V_up, V_down, V_left, V_right);

            reward = game.takeAction(bestAction);
            nextState = game.getState();
            numActions = game.getActions(actions);

            /* Start the learning part of the algorithm */
            if (numActions > 0) {

                bestAction = getBestAction(nextState, actions, numActions, V_up, V_down, V_left, V_right);

                if (bestAction == UP) {
                    V_up.train(state, double(reward) + V_up.evaluate(nextState));
                } else if (bestAction == DOWN) {
                    V_down.train(state, double(reward) + V_down.evaluate(nextState));
                } else if (bestAction == LEFT) {
                    V_left.train(state, double(reward) + V_left.evaluate(nextState));
                } else {
                    V_right.train(state, double(reward) + V_right.evaluate(nextState));
                }
            }
        }

        scores[gameIndex] = game.getScore();
        wins[gameIndex] = (game.getMaxTile() >= 2048);
    }
}


/**
 * This is the function which runs the program. In this program.
 * we train an agent to play the game 2048 using Q-learning
 * applied to the game's state-action pairs.
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

    unsigned int scores[GAMES];
    bool wins[GAMES];

    for (int experiment = 1; experiment <= NUM_EXPERIMENTS; ++experiment)
    {

        cout << "Experiment " << experiment << " / " << NUM_EXPERIMENTS << endl;
        qLearning(scores, wins);

        /* Create the names of the results files */
        ostringstream scoresFileName;
        scoresFileName << "results/"; 
        scoresFileName << "Q_S_" << GAMES << "_" << int(1000*ALPHA) << "_scores.csv";

        ostringstream winsFileName;
        winsFileName << "results/"; 
        winsFileName << "Q_S_" << GAMES << "_" << int(1000*ALPHA) << "_wins.csv";

        /* Save the data to a csv file in the results folder */
        fstream scoresFile;
        fstream winsFile;
        scoresFile.open(scoresFileName.str(), ios::out | ios::app);
        winsFile.open(winsFileName.str(), ios::out | ios::app);

        for (unsigned int i = 0; i < GAMES; ++i) {

            scoresFile << scores[i];
            winsFile << wins[i];

            if (i != GAMES-1) {
                scoresFile << ", ";
                winsFile << ", ";
            }
        }

        scoresFile << '\n';
        scoresFile.close();

        winsFile << '\n';
        winsFile.close();
    }

    return 0;
}
