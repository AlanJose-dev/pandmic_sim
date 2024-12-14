#ifndef RANDOM_WAL_MODEL_H
#define RANDOM_WAL_MODEL_H
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include "Individual.h"
#include "State.h"
#include "RandomNumberGenerator.h"

using namespace std;

class RandomWalkModel {

    private:

        RandomNumberGenerator* randomNumberGenerator;

        vector<vector<Individual>> population;

        vector<vector<Individual>> nextPopulation;

        vector<vector<double>> transitionProbabilities;

        double contagionFactor = 0.5;

        int gridSize;

        bool applySocialDistanceEffect;

        void initializePopulation() {
            population.resize(gridSize, std::vector<Individual>(gridSize, Individual(State::healthy)));
            nextPopulation = population;

            int startIndex = gridSize / 2;
            population[startIndex][startIndex].state = State::sick;
            nextPopulation[startIndex][startIndex].state = State::sick;
        }

    void initializeProbabilities() {
        transitionProbabilities = {
            {0.62, 0.3, 0.05, 0.0, 0.03}, // healthy
            {0.05, 0.64, 0.1, 0.01, 0.2}, // isolated
            {0.0,  0.1,  0.65, 0.1,  0.15}, // sick
            {0.0,  0.0,  0.0,  1.0,  0.0},  // dead
            {0.0,  0.05, 0.02, 0.0,  0.93}  // immune
        };
    }

    void computeSocialInteractions(int line, int column) {
        int initialLine = std::max(0, line - 1);
        int finalLine = std::min(line + 2, gridSize);

        int isolatedCount = 0;

        for (int i = initialLine; i < finalLine; ++i) {
            int initialColumn = std::max(0, column - 1);
            int finalColumn = std::min(column + 2, gridSize);

            for (int j = initialColumn; j < finalColumn; ++j) {
                Individual& neighbour = population[i][j];

                if (neighbour.state == State::isolated && applySocialDistanceEffect) {
                    isolatedCount++;
                }

                if (neighbour.state == State::sick) {
                    computeSickContact(nextPopulation[line][column], neighbour);
                }
            }
        }

        if (isolatedCount > 0 && applySocialDistanceEffect) {
            double reduction = 0.05 * isolatedCount;
            contagionFactor = std::max(contagionFactor * (1.0 - reduction), 0.1);
        }
    }

    void computeSickContact(Individual& individual, Individual& neighbour) {
        if (individual.state == State::dead) return;

        double number = this->randomNumberGenerator->getRandomNumber();

        if (number < contagionFactor) {
            individual.state = State::sick;
        }
    }

    void individualTransition(int line, int column) {
        Individual& individual = population[line][column];

        if (individual.state == State::dead) {
            return;
        }

        if (individual.state == State::healthy) {
            computeSocialInteractions(line, column);
        } else {
            const std::vector<double>& probabilities = transitionProbabilities[static_cast<int>(individual.state)];
            double number = this->randomNumberGenerator->getRandomNumber();

            double cumulativeProbability = 0.0;
            for (size_t i = 0; i < probabilities.size(); ++i) {
                cumulativeProbability += probabilities[i];
                if (number <= cumulativeProbability) {
                    nextPopulation[line][column].state = static_cast<State>(i);
                    break;
                }
            }
        }
    }

    void nextGeneration() {
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                individualTransition(i, j);
            }
        }
        population = nextPopulation;
    }

public:
    RandomWalkModel(int size, bool socialDistanceEffect)
        : gridSize(size), applySocialDistanceEffect(socialDistanceEffect)
    {
        this->randomNumberGenerator = new RandomNumberGenerator();    
        initializePopulation();
        initializeProbabilities();
    }

    int getStateCount(State state) {
        int cumulated = 0;
        for (const auto& row : population) {
            for (const auto& individual : row) {
                if (individual.state == state) {
                    cumulated++;
                }
            }
        }
        return cumulated;
    }

    void simulation(int generations) {
        for (int i = 0; i < generations; ++i) {
            nextGeneration();
        }
    }
};

#endif