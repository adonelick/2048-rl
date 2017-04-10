/**
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digital Signal Processing
 * Final Project
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "state.hpp"
#include "game.hpp"

using namespace std;

/**
 * This function is the main function which runs this program. 
 * In this program, you are able to play a game of 2048 from start
 * to finish. The game is programmed to stop only when no more moves
 * remain for the current game state. However, it does notify you of
 * a win if you reach the 2048 tile.
 *
 * :param argc: Number of input arguments
 * :param argv: Command line arguments
 *
 * :return: Error code
 */
int main(int argc, char **argv)
{

    /* Initialize the random seed */
    srand(time(NULL));

    /* Print the rules of the game */
    cout << "Game rules:" << endl;
    cout << "Up = 5" << endl;
    cout << "Down = 2" << endl;
    cout << "Left = 1" << endl;
    cout << "Right = 3\n" << endl;

    /* Start the game */
    Game game = Game();
    Action actions[NUM_ACTIONS];

    State currentState;
    State afterState;
    unsigned int numActions = game.getActions(actions);
    unsigned int nextAction;

    /* Run the main game loop, as long as there are moves available */
    while (numActions > 0) 
    {
        currentState = game.getState();
        currentState.print();

        cout << "Next action: ";
        cin >> nextAction;
        cout << "" << endl;

        /* Act upon the player's action. Remember the move encoding
         * as described in the game rules above!
         */
        if (nextAction == 5) {
            game.takeAction(UP);
        } else if (nextAction == 2) {
            game.takeAction(DOWN);
        } else if (nextAction == 1) {
            game.takeAction(LEFT);
        } else if (nextAction == 3) {
            game.takeAction(RIGHT);
        } else {
            cout << "Invalid action!" << endl;
        }

        cout << "Current score: " << game.getScore() << endl;
        numActions = game.getActions(actions);

        if (game.getMaxTile() >= 2048) {
            cout << "You Win!" << endl;
        }
    }

    cout << "No more moves. Game over!" << endl;

    return 0;
}