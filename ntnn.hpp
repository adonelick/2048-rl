
#ifndef NTNN_H
#define NTNN_H 1


#include <unordered_map>
#include "state.hpp"


class NTNN
{

private:

    /* Structure to hold the tuples. Will be a 2d array */
    unsigned int** tuples;

    /* The number of tuples in the network */
    unsigned int numTuples;

    /* The length of each tuple */
    unsigned int tupleLength;

    /* Number of tuples currently in the network */
    unsigned int currentNumTuples;

    /* The learning rate */
    double alpha;

    /* An array of weight maps (one map per tuple) */
    std::unordered_map<unsigned int, double>* weights;

public:

    NTNN(unsigned int num, unsigned int length, double alpha);

    /* Object destructor */
    ~NTNN();

    /**
     *
     */
    bool addTuple(unsigned int* tuple, unsigned int length);

    double evaluate(const State& state) const;

    void train(const State& state, double value);

private:

    unsigned int getWeightIndex(const State& state, unsigned int tuple) const;

};

#endif
