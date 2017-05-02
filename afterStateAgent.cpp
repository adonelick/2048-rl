/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digital Signal Processing
 * Final Project
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <unistd.h>

#include "state.hpp"
#include "game.hpp"
#include "ntnn.hpp"

using namespace std;

#define NUM_TUPLES 17
#define TUPLE_LENGTH 4

#define GAMES 10000000
#define ALPHA 0.0001
#define SHOW_GAME false
#define NEW_AGENT false
#define AGENT_FILE "agents/TD_AS_AGENT_MODIFIED.csv"
#define SCORES_FILE "results/TD_AS_0_0_scores.csv"
#define WINS_FILE "results/TD_AS_0_0_wins.csv"
#define SAVE_INTERVAL 1000

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
        State afterState{state};

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

        if (reward != 0)
        {
            reward = log2(reward);
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
 * This function saves the scores and wins to the file specified at 
 * at the top of this file. We assume that this function is called 
 * every SAVE_INTERVAL games, so there are SAVE_INTERVAL elements in
 * each of the provided arrays.
 *
 * :param scores: Array of scores to save to the scores file
 * :param wins: Array of wins to save to the wins file
 *
 * :return: (None)
 */
void saveScores(unsigned int* scores, bool* wins)
{
    /* Save the data to a csv file in the results folder */
    fstream scoresFile;
    fstream winsFile;
    scoresFile.open(SCORES_FILE, ios::out | ios::app);
    winsFile.open(WINS_FILE, ios::out | ios::app);

    for (unsigned int i = 0; i < SAVE_INTERVAL; ++i) {

        scoresFile << scores[i] << '\n';
        winsFile << wins[i] << '\n';

    }

    scoresFile.close();
    winsFile.close();
}


/**
 * This function runs the temporal difference learning algorithm on 
 * the 2048 game afterstates. The scores and outcomes of the games which
 * the algorithms played are stored in the given, pre-allocated arrays.
 *
 * :return: (None)
 */
void playGame(bool showGame)
{
    /* Declare the value function */
    NTNN V(NUM_TUPLES, TUPLE_LENGTH, ALPHA);

    /* Declare the arrays to hold the scores and wins */
    unsigned int scores[SAVE_INTERVAL];
    bool wins[SAVE_INTERVAL];

    if (!NEW_AGENT) {
        V.load(AGENT_FILE);
    }

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
        double valueUpdate;

        while (numActions > 0)
        {
            /* Get the current state of the game. If desired, show the game
             * as the agent plays, including current score.
             */
            state = game.getState();

            if (showGame) {
                state.print();
                cout << "Current Score: " << game.getScore() << endl;
                usleep(250000);
            }

            bestAction = getBestAction(state, actions, numActions, V);
            reward = game.takeAction(bestAction, afterState);
            nextState = game.getState();
            numActions = game.getActions(actions);

            /* Start the learning part of the algorithm */
            if (numActions > 0) {
                nextBestAction = getBestAction(nextState, actions, numActions, V);
                rNext = game.pretendTakeAction(nextBestAction, nextAfterState);

                if (rNext != 0)
                {
                    rNext = log2(rNext);
                }

                valueUpdate = double(rNext) + V.evaluate(nextAfterState);
                V.train(afterState, valueUpdate);
                
            } else if (game.getScore() < 25000) {
                valueUpdate = -50.0;
                V.train(afterState, valueUpdate);
            }

        }

        /* Print out the progress of the current experiment */
        if ((!showGame) && (gameIndex % 10 == 0))
        {
            cout << "Percent Complete: ";
            cout << 100.0*double(gameIndex + 1) / double(GAMES);
            cout << "; Game score: " << game.getScore() << "   ";
            cout << "\r" << flush;
        }

        /* Save the current values in the agent's value network,
         * as well as the scores, to disk for later viewing.
         */
        if ((gameIndex % SAVE_INTERVAL == 0) && (gameIndex > 0)) 
        {
            saveScores(scores, wins);
            V.save(AGENT_FILE);
        }

        /* Save the current game's score */
        scores[gameIndex % SAVE_INTERVAL] = game.getScore();
        wins[gameIndex % SAVE_INTERVAL] = (game.getMaxTile() >= 2048);
    }

    /* Move the cursor to the next line */
    cout << endl;
}


/**
 * This is the function which runs the program. In this program.
 * we train an agent to play the game 2048 using Temporal Difference
 * learning applied to the game's afterstates. The afterstates are 
 * the game states after a move has been executed, but before a 
 * new tile has been inserted.
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

    cout << "Learning Rate: " << ALPHA << endl;
    cout << "Number of Games per Experiment: " << GAMES << endl;

    /* Train the agent using temporal difference learning applied to 
     * game afterstates. As we go, save the agent to disk.
     */
    playGame(SHOW_GAME);

    return 0;
}
