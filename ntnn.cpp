/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digial Signal Processing
 * Final Project
 */

#include "ntnn.hpp"
 
#include <iostream>
#include <cmath>

using namespace std;

NTNN::NTNN(unsigned int num, unsigned int length, double alpha)
    : numTuples{num},
      tupleLength{length},
      alpha{alpha},
      initializeWeights{false}
{
    /* Declare the array of pointers for the tuples */
    tuples = new unsigned int*[numTuples];

    /* Create the arrays for each individual tuple */
    for (int i = 0; i < numTuples; ++i) {
        tuples[i] = new unsigned int[tupleLength];
    }

    /* Dynamically create the array of hashmaps for the weights */
    weights = new unordered_map<unsigned int, double>[numTuples];
}


NTNN::NTNN(unsigned int num, unsigned int length, double alpha, bool initializeWeights)
    : numTuples{num},
      tupleLength{length},
      alpha{alpha},
      initializeWeights{initializeWeights}
{
    /* Declare the array of pointers for the tuples */
    tuples = new unsigned int*[numTuples];

    /* Create the arrays for each individual tuple */
    for (int i = 0; i < numTuples; ++i) {
        tuples[i] = new unsigned int[tupleLength];
    }

    /* Dynamically create the array of hashmaps for the weights */
    weights = new unordered_map<unsigned int, double>[numTuples];
}


NTNN::~NTNN()
{
    for (int i = 0; i < numTuples; ++i) {
        delete[] tuples[i];
    }

    delete[] tuples;
    delete[] weights;
}


bool NTNN::addTuple(unsigned int* tuple, unsigned int length)
{   
    /* Check if the given tuple has the proper length */
    if (length != tupleLength) {
        return false;
    }

    /* If the tuple has the right size, check if we have room for it. */
    if (currentNumTuples == numTuples) {
        return false;
    }

    /* If we have room, add the tuple to the tuples array */
    for (unsigned int i = 0; i < length; ++i) {
        tuples[currentNumTuples][i] = tuple[i];
    }

    currentNumTuples++;
    return true;
}


double NTNN::evaluate(const State& state) const
{
    double value = 0.0;
    unsigned int weightIndex;

    for (unsigned int i = 0; i < currentNumTuples; ++i) {
        weightIndex = getWeightIndex(state, i);

        if (initializeWeights && (weights[i][weightIndex] == 0)) {
            weights[i][weightIndex] = INITIAL_WEIGHTS;
        }

        /* If the weightIndex has never been seen before, the []
         * operator adds it in for us, with default value 0.0.
         */
        value += weights[i][weightIndex];
    }

    return value;
}


void NTNN::train(const State& state, double value)
{
    double weightChange = alpha*(value - evaluate(state));
    unsigned int weightIndex;

    for (unsigned int i = 0; i < currentNumTuples; ++i) {

        weightIndex = getWeightIndex(state, i);

        /* If the weightIndex has never been seen before, the []
         * operator adds it in for us, with default value 0.0.
         */
        weights[i][weightIndex] += weightChange;
    }
}


unsigned int NTNN::getWeightIndex(const State& state, unsigned int tuple) const
{
    unsigned int weightIndex = 0;
    unsigned int row;
    unsigned int col;

    for (int i = 0; i < tupleLength; ++i) {

        /* Convert the index given in the tuple to row/column for the grid */
        row = tuples[tuple][i] / tupleLength;
        col = tuples[tuple][i] % tupleLength;

        if (state.getTile(row, col) != 0) {
            weightIndex += pow(100, i)*log2(state.getTile(row, col));
        }
    }

    return weightIndex;
}
