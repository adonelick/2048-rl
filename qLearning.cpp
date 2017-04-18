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
#include <future>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "state.hpp"
#include "game.hpp"
#include "ntnn.hpp"

using namespace std;

#define NUM_TUPLES 17
#define TUPLE_LENGTH 4

#define GAMES 100
#define ALPHA 0.01
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
 * The function chooses the action which maximizes value of the appropriate
 * value function.
 *
 * :param state: Reference to the current state
 * :param actions: Array of available actions in the current state
 * :param numActions: Number of actions in the actions array
 * :param V_up: Value function for the up state-action pairs
 * :param V_down: Value function for the down state-action pairs
 * :param V_left: Value function for the left state-action pairs
 * :param V_right: Value function for the right state-action pairs
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
 * This function runs the Q-learning algorithm on the 2048 games state-action
 * pairs. The scores and outcomes of the games which the agent plays are 
 * returned. Each set of games played by the agent is called an experiment.
 *
 * :param reportProcess: Whether to print the progress of the experiment
 *
 * :return: The results of the experiment (wins and scores as a function of
 *          number of games played)
 */
Results qLearning(bool reportProgress)
{
    /* Create the struct to store the experiment results */
    Results results;

    /* Declare the value functions (one for each action) */
    NTNN V_up(NUM_TUPLES, TUPLE_LENGTH, ALPHA, true);
    NTNN V_down(NUM_TUPLES, TUPLE_LENGTH, ALPHA, true);
    NTNN V_left(NUM_TUPLES, TUPLE_LENGTH, ALPHA, true);
    NTNN V_right(NUM_TUPLES, TUPLE_LENGTH, ALPHA, true);

    /* Add the tuples to the n-tuple regression networks */
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
    
    /* Run the training loop */
    for (unsigned int gameIndex = 0; gameIndex < GAMES; ++gameIndex)
    {
        Game game;
        numActions = game.getActions(actions);

        State state;
        State afterState;
        State nextState;

        Action bestAction;
        Action nextBestAction;
        unsigned int reward;
        double vNext;

        while (numActions > 0)
        {
            state = game.getState();

            /* Use the agent's policy to choose the next move to take */
            bestAction = getBestAction(state, actions, numActions, V_up, V_down, V_left, V_right);
            reward = game.takeAction(bestAction, afterState);

            /* Get the new state of the game */
            nextState = game.getState();
            numActions = game.getActions(actions);

            /* Start the learning part of the algorithm */
            if (numActions > 0) {

                nextBestAction = getBestAction(nextState, actions, numActions, V_up, V_down, V_left, V_right);

                /* Get the value of the next state, using the action selected above */
                if (nextBestAction == UP) {
                    vNext = V_up.evaluate(nextState);
                } else if (nextBestAction == DOWN) {
                    vNext = V_down.evaluate(nextState);
                } else if (nextBestAction == LEFT) {
                    vNext = V_left.evaluate(nextState);
                } else {
                    vNext = V_right.evaluate(nextState);
                }

                /* Use Q-Learning to update the value of the state-action pair */
                if (bestAction == UP) {
                    V_up.train(state, double(reward) + vNext);
                } else if (bestAction == DOWN) {
                    V_down.train(state, double(reward) + vNext);
                } else if (bestAction == LEFT) {
                    V_left.train(state, double(reward) + vNext);
                } else {
                    V_right.train(state, double(reward) + vNext);
                }
            }
        }

        /* If desired, print out the progress of the current experiment */
        if (reportProgress) {
            cout << "Percent Complete: ";
            cout << 100.0*double(gameIndex + 1) / double(GAMES) << "\r" << flush;
        }

        /* Record the results of the current game */
        results.scores.push_back(game.getScore());
        results.wins.push_back((game.getMaxTile() >= 2048));
    }

    /* Move the cursor to the next line */
    if (reportProgress) {
        cout << endl;
    }

    return results;
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

    for (int experiment = 1; experiment <= NUM_EXPERIMENTS; experiment += 4)
    {

        cout << "Experiment " << experiment << " / " << NUM_EXPERIMENTS << endl;

        /* Run four experiments in parallel to speed up the data gathering process.
         * First, we start the four training experiments in their own threads.
         */
        auto future1 = async(launch::async, qLearning, true);
        auto future2 = async(launch::async, qLearning, false);
        auto future3 = async(launch::async, qLearning, false);
        auto future4 = async(launch::async, qLearning, false);

        /* Wait for each experiment to finish execution */
        future1.wait();
        future2.wait();
        future3.wait();
        future4.wait();

        /* Collect the results from each of the experiments */
        Results experimentResults[4];
        experimentResults[0] = future1.get();
        experimentResults[1] = future2.get();
        experimentResults[2] = future3.get();
        experimentResults[3] = future4.get();

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

        for (unsigned int i = 0; i < 4; ++i) {
            for (unsigned int j = 0; j < GAMES; ++j) {

                scoresFile << experimentResults[i].scores[j];
                winsFile << experimentResults[i].wins[j];

                if (j != GAMES-1) {
                    scoresFile << ", ";
                    winsFile << ", ";
                }
            }

            scoresFile << '\n';
            winsFile << '\n';
        }

        scoresFile.close();
        winsFile.close();
    }

    return 0;
}
