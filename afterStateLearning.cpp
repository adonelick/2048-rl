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
#include <future>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include "state.hpp"
#include "game.hpp"
#include "ntnn.hpp"

using namespace std;

#define NUM_TUPLES 17
#define TUPLE_LENGTH 4

#define GAMES 200000
#define ALPHA 0.002
#define NUM_EXPERIMENTS 4



/* Declare a struct which is used to collect experiment results */
struct Results
{
    vector<unsigned int> scores;
    vector<bool> wins;
};


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
 * This function runs the temporal difference learning algorithm on 
 * the 2048 game afterstates. The scores and outcomes of the games which
 * the algorithms played are stored in the given, pre-allocated arrays.
 *
 * :return: The results of the experiment (wins and scores as a function of
 *          number of games played)
 */
Results afterStateLearning()
{
    /* Create the struct to store the experiment results */
    Results results;

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
        double valueUpdate;

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

                if (rNext != 0)
                {
                    rNext = log2(rNext);
                }

                valueUpdate = double(rNext) + V.evaluate(nextAfterState);
                V.train(afterState, valueUpdate);
            } else {
                V.train(afterState, -50.0);
            }
        }

        /* Print out the progress of the current experiment */
        if (gameIndex % 10 == 0)
        {
            cout << "Percent Complete: ";
            cout << 100.0*double(gameIndex + 1) / double(GAMES);
            cout << "; Game score: " << game.getScore() << "   ";
            cout << "\r" << flush;
        }

        /* Record the results of the current game */
        results.scores.push_back(game.getScore());
        results.wins.push_back((game.getMaxTile() >= 2048));
    }

    /* Move the cursor to the next line */
    cout << endl;

    return results;
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

    unsigned int scores[GAMES];
    bool wins[GAMES];

    for (int experiment = 1; experiment <= NUM_EXPERIMENTS; ++experiment)
    {

        cout << "Experiment " << experiment << " / " << NUM_EXPERIMENTS << endl;

        /* Collect the results from each of the experiments */
        Results experimentResults = afterStateLearning();

        /* Create the names of the results files */
        ostringstream scoresFileName;
        scoresFileName << "results/"; 
        scoresFileName << "TD_AS_" << GAMES << "_" << int(1000*ALPHA) << "_scores.csv";

        ostringstream winsFileName;
        winsFileName << "results/"; 
        winsFileName << "TD_AS_" << GAMES << "_" << int(1000*ALPHA) << "_wins.csv";

        /* Save the data to a csv file in the results folder */
        fstream scoresFile;
        fstream winsFile;
        scoresFile.open(scoresFileName.str(), ios::out | ios::app);
        winsFile.open(winsFileName.str(), ios::out | ios::app);

        for (unsigned int i = 0; i < GAMES; ++i) {

            scoresFile << experimentResults.scores[i];
            winsFile << experimentResults.wins[i];

            if (i != GAMES-1) {
                scoresFile << ", ";
                winsFile << ", ";
            }
        }

        scoresFile << '\n';
        winsFile << '\n';

        scoresFile.close();
        winsFile.close();
    }

    return 0;
}
