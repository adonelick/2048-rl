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
#include <random>
#include <stdlib.h>
#include <time.h>

#include "state.hpp"
#include "game.hpp"

using namespace std;

#define NUM_TUPLES 17
#define TUPLE_LENGTH 4

#define GAMES 200000
#define EPSILON 0.0
#define NUM_EXPERIMENTS 30



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
Action getBestAction(const State& state, Action* actions, int numActions)
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

        /* Compute the value of the action, and check if 
         * the action compares favorably to previous results.
         */
        if (value > bestValue) {
            bestValue = value;
            bestAction = a;
        }
    }

    random_device rd;
    mt19937 e2(rd());
    uniform_real_distribution<> dist(0, 1);

    if (dist(e2) < EPSILON) {
        bestAction = static_cast<Action>(int(rand()));
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
Results epsilonGreedyPlaying()
{
    /* Create the struct to store the experiment results */
    Results results;
    
    Action actions[4];
    unsigned int numActions;
    
    for (unsigned int gameIndex = 0; gameIndex < GAMES; ++gameIndex)
    {
        Game game;
        numActions = game.getActions(actions);

        State state;
        Action bestAction;
        unsigned int reward;

        while (numActions > 0)
        {
            state = game.getState();
            bestAction = getBestAction(state, actions, numActions);

            reward = game.takeAction(bestAction);
            numActions = game.getActions(actions);
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

    cout << "Learning Rate: " << ALPHA << endl;
    cout << "Number of Games per Experiment: " << GAMES << endl;

    for (int experiment = 1; experiment <= NUM_EXPERIMENTS; ++experiment)
    {

        cout << "Experiment " << experiment << " / " << NUM_EXPERIMENTS << endl;

        /* Collect the results from each of the experiments */
        Results experimentResults = epsilonGreedyPlaying();

        /* Create the names of the results files */
        ostringstream scoresFileName;
        scoresFileName << "results/"; 
        scoresFileName << "EG_S_" << GAMES << "_" << int(1000*EPSILON) << "_scores.csv";

        ostringstream winsFileName;
        winsFileName << "results/"; 
        winsFileName << "EG_S_" << GAMES << "_" << int(1000*EPSILON) << "_wins.csv";

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
