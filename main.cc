#include "Headers/RandomWalkModel.h"
#include "Headers/State.h"

using namespace std;

int main() {
    int numberOfRuns = 1000;
    int gridSize = 100;
    int numberOfGenerations = 52;
    bool applySocialDistanceEffect = true;
    
    vector<vector<double>> transitionProbabilities = {
        {0.62, 0.3, 0.05, 0.0, 0.03}, // healthy
        {0.05, 0.64, 0.1, 0.01, 0.2}, // isolated
        {0.0,  0.1,  0.65, 0.1,  0.15}, // sick
        {0.0,  0.0,  0.0,  1.0,  0.0},  // dead
        {0.0,  0.05, 0.02, 0.0,  0.93}  // immune
    };

    for(int i = 0; i < numberOfRuns; ++i) {
        RandomWalkModel model(gridSize, applySocialDistanceEffect);
        model.simulation(numberOfGenerations);
        cout << model.getStateCount(State::dead) << endl;
    }

    return EXIT_SUCCESS;
}