/** 
 * Written by Andrew Donelick
 * EELE 577 - Advanced Digital Signal Processing
 * Final Project
 */

#include "ntnn.hpp"
 
#include <iostream>
#include <fstream>
#include <sstream>
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


void NTNN::train(const State& state, double update)
{
    double weightChange = alpha*(update - evaluate(state));
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
        row = tuples[tuple][i] / GRID_SIZE;
        col = tuples[tuple][i] % GRID_SIZE;

        if (state.getTile(row, col) != 0) {
            weightIndex += pow(100, i)*log2(state.getTile(row, col));
        }
    }

    return weightIndex;
}


void NTNN::load(const string& agentFile)
{
    fstream agent;
    agent.open(agentFile, ios::in);

    string line;
    unsigned int tuple = 0;
    unsigned int weightIndex;
    double value;

    if (agent.is_open()) {

        while (getline(agent, line))
        {
            if (line == "-1, -1") {
                tuple++;
            } else {

                string stringKey = line.substr(0, line.find(", "));
                string stringValue = line.erase(0, line.find(", ") + 2);

                stringstream keyConversion{stringKey};
                stringstream valueConversion{stringValue};

                keyConversion >> weightIndex;
                valueConversion >> value;

                weights[tuple][weightIndex] = value;
            }
        }
    }
}


void NTNN::save(const string& agentFile)
{
    fstream agent;
    agent.open(agentFile, ios::out | ios::trunc);

    /* Loop throough each tuple, and dump all weights associated with
     * that tuple to the file.
     */
    for (unsigned int i = 0; i < currentNumTuples; ++i) {

        for (auto const& element : weights[i]) {

            agent << element.first;
            agent << ", ";
            agent << element.second;
            agent << "\n";
        }

        /* We assume that this line represents the division between tuples */
        agent << "-1, -1\n";
    }

    agent.close();
}
