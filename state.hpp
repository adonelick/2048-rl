/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digial Signal Processing
 * Final Project
 */

#ifndef STATE_H
#define STATE_H 1

#define GRID_SIZE 4
#define TWO_PROBABILITY 0.9

/**
 * This class represents a state for the game 2048.
 */
class State {

private:

    /* The GRID_SIZE x GRID_SIZE array that defines a state */
    unsigned int grid[GRID_SIZE][GRID_SIZE];

public:

    /**
     * The constructor for a State object.
     *
     * :return: New State object
     */
    State();

    /**
     * The copy constructor for a State object.
     *
     * :param otherState: The existing state to copy
     *
     * :return: New State object
     */
    State(const State& otherState);

    /**
     * Assignment operator for a State object.
     *
     * :param otherState: The existing state to copy
     *
     * :return: New State object
     */
    State& operator=(const State& otherState);

    /**
     * This function adds a new tile to the game state.
     * The tile takes value 2 with probability 0.9, and a value 4 
     * with probability 0.1. Regardless of its value, the tile is placed
     * at random in any of the free spaces in the state.
     *
     * :return: (None)
     */
    void insertNewTile();

    /**
     * Searches the entire game state and returns the value of the tile
     * with the largest value.
     *
     * :return: Value of the largest tile on the board
     */
    unsigned int getMaxTile() const;

    /**
     * This member function moves and combines the tiles in the game state
     * as if the player made an "UP" move. This function only moves and 
     * combines the tiles; it does not add a new tile to complete the move.
     *
     * :return: Reward for executing the UP move
     */
    unsigned int slideUp();

    /**
     * This member function moves and combines the tiles in the game state
     * as if the player made an "DOWN" move. This function only moves and 
     * combines the tiles; it does not add a new tile to complete the move.
     *
     * :return: Reward for executing the DOWN move
     */
    unsigned int slideDown();

    /**
     * This member function moves and combines the tiles in the game state
     * as if the player made an "LEFT" move. This function only moves and 
     * combines the tiles; it does not add a new tile to complete the move.
     *
     * :return: Reward for executing the LEFT move
     */
    unsigned int slideLeft();

    /**
     * This member function moves and combines the tiles in the game state
     * as if the player made an "RIGHT" move. This function only moves and 
     * combines the tiles; it does not add a new tile to complete the move.
     *
     * :return: Reward for executing the RIGHT move
     */
    unsigned int slideRight();

    /**
     * Prints the current values of the state's grid in a pretty way.
     * 
     * :return: (None)
     */
    void print() const;

    /**
     * Returns the value of the tile located in the specified row and column.
     * If no tile exists at the location specified, then the function returns
     * a value of zero. 
     *
     * :param row: Row of the tile of interest
     * :param col: Column of the tile of interest
     *
     * :return: Value of the tile located at the specified location
     */
    unsigned int getTile(unsigned int row, unsigned int col) const;


    /**
     * Sets the value of a state's tile using the given row, column,
     * and desired new value. The new value should be a power of 2 and
     * an integer.
     *
     * :param row: Row of the tile of interest
     * :param col: Column of the tile of interest
     * :param value: New value of the tile located at (row, col)
     *
     * :return: Whether the value at (row, col) was updated
     */
    bool setTile(unsigned int row, unsigned int col, unsigned int value);

    /**
     * Gets the locations and numbers of all empty tiles in the current
     * state. Locations are returned in the supplied arrays.
     *
     * :param rows: Row locations of the empty tiles
     * :param cols: Column locations of the empty tiles
     *
     * :return: Number of empty tiles
     */
     unsigned int getEmptyTiles(unsigned int* rows, unsigned int* cols) const;

    /**
     * Gets the possible next states that can be reached from the current
     * game state. The function also computes the probability of reaching
     * each of the next possible states.
     * 
     * :param nextStates: Array of the possible next states
     * :param probabilities: Probabilities corresponsing to each possible state
     *
     * :return: Number of possible next states
     */
    unsigned int getNextStates(State** nextStates, double* probabilities) const;

    /**
     * Overloaded relational operators for equality comparison
     *
     * :param otherState: State to compare against
     *
     * :return: Whether or not this state and the other are equal 
     */
    bool operator==(const State& otherState) const;

    /**
     * Overloaded relational operators for inequality comparison
     *
     * :param otherState: State to compare against
     *
     * :return: Whether or not this state and the other are not equal 
     */
    bool operator!=(const State& otherState) const;

};

#endif
