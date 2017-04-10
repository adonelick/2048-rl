/* Written by Andrew Donelicl
 * EELE 577 Final Project
 */

#include <iostream>
#include <iomanip>
#include <random>
#include <stdlib.h>
#include "state.hpp"


using namespace std;


State::State()
{
    /* Initialize the grid to zero */
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            grid[row][col] = 0;
        }
    }

    /* Start by inserting two tiles into the grid */
    insertNewTile();
    insertNewTile();
}


State::State(const State& otherState)
{
    /* Copy over the data from one grid to the other */
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            grid[row][col] = otherState.grid[row][col];
        }
    }
}


State& State::operator=(const State& otherState)
{
    /* Copy over the data from one grid to the other */
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            grid[row][col] = otherState.grid[row][col];
        }
    }
}


void State::insertNewTile()
{
    random_device rd;
    mt19937 e2(rd());
    uniform_real_distribution<> dist(0, 1);

    /* Get the possible indices where we can insert a tile */
    int rowIndices[GRID_SIZE*GRID_SIZE];
    int colIndices[GRID_SIZE*GRID_SIZE];
    int numZeros = 0;

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {

            if (grid[row][col] == 0) {
                rowIndices[numZeros] = row;
                colIndices[numZeros] = col;
                numZeros++;
            }
        }
    }

    /* Choose whether to insert a 2 or a 4 */
    unsigned int insertValue;
    if (dist(e2) < TWO_PROBABILITY) {
        insertValue = 2;
    } else {
        insertValue = 4;
    }

    /* Insert the value into one of the free spaces on the board */
    int index = rand() % numZeros;

    int row = rowIndices[index];
    int col = colIndices[index];
    grid[row][col] = insertValue;

}


unsigned int State::getMaxTile() const
{

    unsigned int maxTile = 0;

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {

            if (grid[row][col] > maxTile) {
                maxTile = grid[row][col];
            }
        }
    }

    return maxTile;
}


unsigned int State::slideUp()
{

    unsigned int reward = 0;

    for (int col = 0; col < 4; ++col) {

        int firstFreeRow = 0;
        bool alreadyAggregated = false;

        for (int row = 0; row < 4; ++row) {

            if (grid[row][col] == 0) {
                continue;
            }

            if ((firstFreeRow > 0) && (!alreadyAggregated) && (grid[firstFreeRow-1][col] == grid[row][col])) {
                grid[firstFreeRow - 1][col] *= 2;
                grid[row][col] = 0;
                reward += grid[firstFreeRow-1][col];
                alreadyAggregated = true;
            } else {
                unsigned int temp = grid[row][col];
                grid[row][col] = 0;
                grid[firstFreeRow++][col] = temp;
                alreadyAggregated = false;
            }
        }
    }

    return reward;
}


unsigned int State::slideDown()
{
    unsigned int reward = 0;

    for (int col = 0; col < GRID_SIZE; ++col) {

        int firstFreeRow = 3;
        bool alreadyAggregated = false;

        for (int row = GRID_SIZE-1; row >= 0; --row) {

            if (grid[row][col] == 0) {
                continue;
            }

            if ((firstFreeRow < 3) && (!alreadyAggregated) && (grid[firstFreeRow+1][col] == grid[row][col])) {
                grid[firstFreeRow + 1][col] *= 2;
                grid[row][col] = 0;
                reward += grid[firstFreeRow+1][col];
                alreadyAggregated = true;
            } else {
                unsigned int temp = grid[row][col];
                grid[row][col] = 0;
                grid[firstFreeRow--][col] = temp;
                alreadyAggregated = false;
            }
        }
    }

    return reward;
}


unsigned int State::slideRight()
{
    unsigned int reward = 0;

    for (int row = 0; row < GRID_SIZE; ++row) {

        int firstFreeCol = 3;
        bool alreadyAggregated = false;

        for (int col = GRID_SIZE-1; col >= 0; --col) {

            if (grid[row][col] == 0) {
                continue;
            }

            if ((firstFreeCol < 3) && (!alreadyAggregated) && (grid[row][firstFreeCol+1] == grid[row][col])) {
                grid[row][firstFreeCol+1] *= 2;
                grid[row][col] = 0;
                reward += grid[row][firstFreeCol+1];
                alreadyAggregated = true;
            } else {
                unsigned int temp = grid[row][col];
                grid[row][col] = 0;
                grid[row][firstFreeCol--] = temp;
                alreadyAggregated = false;
            }
        }
    }

    return reward;
}


unsigned int State::slideLeft()
{
    unsigned int reward = 0;

    for (int row = 0; row < 4; ++row) {

        int firstFreeCol = 0;
        bool alreadyAggregated = false;

        for (int col = 0; col < 4; ++col) {

            if (grid[row][col] == 0) {
                continue;
            }

            if ((firstFreeCol > 0) && (!alreadyAggregated) && (grid[row][firstFreeCol-1] == grid[row][col])) {
                grid[row][firstFreeCol-1] *= 2;
                grid[row][col] = 0;
                reward += grid[row][firstFreeCol-1];
                alreadyAggregated = true;
            } else {
                unsigned int temp = grid[row][col];
                grid[row][col] = 0;
                grid[row][firstFreeCol++] = temp;
                alreadyAggregated = false;
            }
        }
    }

    return reward;
}


void State::print() const
{
    cout << "-----------------------------\n";
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            if (grid[row][col] != 0) {
                cout << '|' << setfill(' ') << setw(6) << grid[row][col];
            } else {
                cout << "|      ";
            }
            
        }
        cout << "|\n-----------------------------\n";
    }
}


unsigned int State::getTile(unsigned int row, unsigned int col) const
{
    return grid[row][col];
}


bool State::operator==(const State& otherState) const
{
    bool equal = true;

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {

            equal = equal && (grid[row][col] == otherState.grid[row][col]);
        }
    }

    return equal;
}


bool State::operator!=(const State& otherState) const
{
    return !(*this == otherState);
}
