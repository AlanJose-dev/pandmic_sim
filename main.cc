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

    for(int i = 0; i < numberOfRuns; ++i) {
        RandomWalkModel model(gridSize, applySocialDistanceEffect);
        model.simulation(numberOfGenerations);
        //Print the individuals count based on current state.
        cout << model.getStateCount(State::dead) << endl;
    }

    return EXIT_SUCCESS;
}