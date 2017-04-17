/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digial Signal Processing
 * Final Project
 */

#ifndef NTNN_H
#define NTNN_H 1

#include <unordered_map>
#include "state.hpp"

#define INITIAL_WEIGHTS 10.0


/**
 * This class implements an n-tuple regression network which
 * serves as the value functions for the reinforcement learning problem.
 * In this case, the value function maps a state to a numeric value.
 *
 * We use the following indexing scheme for the board's tiles:
 * -------------------------
 * |  0  |  1  |  2  |  3  |
 * -------------------------
 * |  4  |  5  |  6  |  7  |
 * -------------------------
 * |  8  |  9  |  10 |  11 |
 * -------------------------
 * |  12 |  13 |  14 |  15 |
 * -------------------------
 
 * The tuples of the n-tuple network are literally tuples of these
 * indices, say, for example (0, 1, 2, 3). This would be a 4-tuple
 * across the top row of the game board. 
 */
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
    unsigned int currentNumTuples = 0;

    /* The learning rate */
    double alpha;

    /* Whether the weights are initially zero, or the value INITIAL_WEIGHTS */
    bool initializeWeights;

    /* An array of weight maps (one map per tuple) */
    std::unordered_map<unsigned int, double>* weights;

public:

    /**
     * This is the standard constructor for the n-tuple network. With this
     * constructor, you specify the number of tuples the network will use, 
     * the length of each tuple, and the network's learning rate. When using 
     * this constructor, the initial weights are all set to zero.
     *
     * :param num: Number of tuples to be in the network
     * :param length: Length of each individual tuple
     * :param alpha: Learning rate of the network
     *
     * :return: New n-tuple neural network
     */
    NTNN(unsigned int num, unsigned int length, double alpha);

    /**
     * This is the standard constructor for the n-tuple network. With this
     * constructor, you specify the number of tuples the network will use, 
     * the length of each tuple, and the network's learning rate.
     *
     * :param num: Number of tuples to be in the network
     * :param length: Length of each individual tuple
     * :param alpha: Learning rate of the network
     * :param initializeWeights: Whether the network's weights are initially nonzero
     *
     * :return: New n-tuple neural network
     */
    NTNN(unsigned int num, unsigned int length, double alpha, bool initializeWeights);

    /**
    * This is simply the object destructor.
    */
    ~NTNN();

    /**
     * This member function allows a user to add a tuple to the network.
     * If the tuple is added successfully, the function returns true.
     * If the tuple cannot be added (if it is too long, or the network 
     * is full), then the function returns false. 
     *
     * :param tuple: Array containing the tuple to be added
     * :param length: Length of the tuple array
     *
     * :return: Whether the tuple was added successfully or not 
     */
    bool addTuple(unsigned int* tuple, unsigned int length);

    /**
     * This function evaluates a given state and returns its value
     * based on the weights of the network. 
     *
     * :param state: State to be evaluated
     *
     * :return: Value of the given state
     */
    double evaluate(const State& state) const;

    /**
     * This member function allows the user to present the network with 
     * a training example. The user provides a state with a corresponding
     * value, and the network updates its weights accordingly.
     *
     * :param state: State on which to train the network
     * :param value: Corresponding value of the given state
     *
     * :return: (None)
     */
    void train(const State& state, double value);

private:

    /**
     * This function is a helper function that calculates the index 
     * to the weight map for a given state and tuple. 
     *
     * :param state: State for which to compute the weight index
     * :param tuple: Index of the tuple for which we wish to calculate the weight index
     *
     * :return: Weight index
     */
    unsigned int getWeightIndex(const State& state, unsigned int tuple) const;

};

#endif
