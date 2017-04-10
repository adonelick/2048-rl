
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "state.hpp"
#include "game.hpp"

using namespace std;

int main(int argc, char **argv)
{

    // Initialize the random seed
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

    while (numActions > 0) 
    {
        currentState = game.getState();
        currentState.print();

        cout << "Next action: ";
        cin >> nextAction;
        cout << "" << endl;

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

    return 0;
}