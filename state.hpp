
#ifndef STATE_H
#define STATE_H 1


#define GRID_SIZE 4
#define TWO_PROBABILITY 0.9


class State {

private:

    /* The GRID_SIZE x GRID_SIZE array that defines a state */
    unsigned int grid[GRID_SIZE][GRID_SIZE];

public:

    State();

    State(const State& otherState);

    State& operator=(const State& otherState);

    void insertNewTile();

    unsigned int getMaxTile() const;

    unsigned int slideUp();

    unsigned int slideDown();

    unsigned int slideLeft();

    unsigned int slideRight();

    void print() const;

    unsigned int getTile(unsigned int row, unsigned int col) const;

    /* Overloaded relational operators for equality comparison */
    bool operator==(const State& otherState) const;

    /* Overloaded relational operators for inequality comparison */
    bool operator!=(const State& otherState) const;

};

#endif