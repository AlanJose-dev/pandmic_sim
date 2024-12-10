#include "Headers/RandomWalkModel.h"
#include "Headers/State.h"
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
    //Default params.
    int numberOfRuns = 1000;
    int gridSize = 100;
    int numberOfGenerations = 10;
    bool applySocialDistanceEffect = false;
    
    //Don't move.
    int cliOption;
    while((cliOption = getopt(argc, argv, "r:g:n:s:")) != -1) {
        switch(cliOption) {
            case 'r':
                numberOfRuns = stoi(optarg);
                break;
            case 'g':
                gridSize = stoi(optarg);
                break;
            case 'n':
                numberOfGenerations = stoi(optarg);
                break;
            case 's':
                applySocialDistanceEffect = stoi(optarg) != 0;
                break;
            default:
                cerr << "Usage: " << argv[0] << 
                " [-r numberOfRuns] [-g gridSize] [-n numberOfGenerations] [-s applySocialDistanceEffect (1/0)]"
                << endl;
                return EXIT_FAILURE;
        }
    }

    //Ok, you can change here.
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
        //Print the individuals count based on current state.
        cout << model.getStateCount(State::dead) << endl;
    }

    return EXIT_SUCCESS;
}