#ifndef RANDOM_WALK_MODEL_PARALLEL
#define RANDOM_WALK_MODEL_PARALLEL

#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include "RandomWalkModel.h"

using namespace std;

class RandomWalkModelParallel : public RandomWalkModel {
    
    private:
    
        // Attributes =>

        int threadCount;

        int currentProcessorAvailableThreads;

        // Methods =>

        void processChunk(int startRow, int endRow) {
            for (int i = startRow; i < endRow; ++i) {
                for (int j = 0; j < this->populationMatrixSize; ++j) {
                    this->individualTransition(i, j);
                }
            }
        }

        void throwIfMaximumThreadsIsExceeded()
        {
            if(this->threadCount > this->currentProcessorAvailableThreads) {
                throw invalid_argument("ERROR: THE REQUESTED THREADS COUNT EXCEEDS THE CURRENT PROCESSOR AVAILABLE THREADS.");
            }
        }

    public:

        using RandomWalkModel::RandomWalkModel; // Inherit constructor

        RandomWalkModelParallel(int populationMatrixSize, bool applySocialDistanceEffect, int threadCount):
         RandomWalkModel(populationMatrixSize, applySocialDistanceEffect), threadCount(threadCount)
        {
            this->currentProcessorAvailableThreads = thread::hardware_concurrency();
            this->throwIfMaximumThreadsIsExceeded();
        }

        void parallelSimulation(int generations) {
            for (int g = 0; g < generations; ++g) {
                // Create threads to process chunks of the population grid
                vector<thread> threads;
                int rowsPerThread = this->populationMatrixSize / this->threadCount;
                int remainingRows = this->populationMatrixSize % this->threadCount;

                for (int t = 0; t < this->threadCount; ++t) {
                    int startRow = t * rowsPerThread;
                    int endRow = startRow + rowsPerThread + (t < remainingRows ? 1 : 0);

                    threads.emplace_back([this, startRow, endRow]() {
                        processChunk(startRow, endRow);
                    });
                }

                // Wait for all threads to finish
                for (auto& t : threads) {
                    t.join();
                }

                // Synchronize population data
                this->population = this->nextPopulation;
            }
        }
};


#endif