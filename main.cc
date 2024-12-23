#include "Headers/RandomWalkModel.h"
#include "Headers/RandomWalkModelParallel.h"
#include "Headers/State.h"
#include "Headers/ProgramInfoViewer.h"
#include <getopt.h>
#include <memory>

using namespace std;

int main(int argc, char* argv[]) {
    //Default params.
    int numberOfRuns = 1000;
    int populationMatrixSize = 100;
    int numberOfGenerations = 10;
    bool applySocialDistanceEffect = false;
    int threadCount = 1;
    bool generateImage = false;
    
    //Parse CLI options.
    //Don't move.
    static struct option longOptions[] = {
    {"runs", optional_argument, nullptr, 'r'},
    {"population", optional_argument, nullptr, 'p'},
    {"generations", optional_argument, nullptr, 'g'},
    {"social-distance-effect", no_argument, nullptr, 's'},
    {"threads", optional_argument, nullptr, 't'},
    {"image", no_argument, nullptr, 'i'},
    {"help", no_argument, nullptr, 'h'},
    {"version", no_argument, nullptr, 'v'},
    {nullptr, 0, nullptr, 0}
};

    const char* shortOptions = "r:p:g:st:ihv";

    int cliOption;
    while ((cliOption = getopt_long(argc, argv, shortOptions, longOptions, nullptr)) != -1) {
        switch (cliOption) {
            case 'r':
                numberOfRuns = stoi(optarg);
                break;
            case 'p':
                populationMatrixSize = stoi(optarg);
                break;
            case 'g':
                numberOfGenerations = stoi(optarg);
                break;
            case 's':
                applySocialDistanceEffect = true;
                break;
            case 't':
                threadCount = stoi(optarg);
                break;
            case 'i':
                generateImage = true;
                break;
            case 'h': // Handle both --help and -h
                printHelp();
                exit(EXIT_SUCCESS); // Terminate the program
            case 'v':
                printVersion();
                exit(EXIT_SUCCESS); // Terminate the program
            default:
                cerr << "Unknown option. Use -h or --help for usage information.\n";
                exit(EXIT_FAILURE);
        }
    }

    //Switch the probabilities as you need.
    vector<vector<double>> transitionProbabilities = {
        {0.62, 0.3, 0.05, 0.0, 0.03}, // healthy
        {0.05, 0.64, 0.1, 0.01, 0.2}, // isolated
        {0.0,  0.1,  0.65, 0.1,  0.15}, // sick
        {0.0,  0.0,  0.0,  1.0,  0.0},  // dead
        {0.0,  0.05, 0.02, 0.0,  0.93}  // immune
    };
    bool isMultiThreading = threadCount > 1;
    auto startTime = (chrono::high_resolution_clock::now());

    printHeaders(
        new int[4]{numberOfRuns, populationMatrixSize, numberOfGenerations, threadCount},
        new bool[2]{applySocialDistanceEffect, generateImage}
    );

    /**
     * Executes the model.
     */
    if(isMultiThreading) {
        try
        {
            unique_ptr<RandomWalkModelParallel> model;
            for(int i = 0; i < numberOfRuns; ++i) {
                model = make_unique<RandomWalkModelParallel>(populationMatrixSize, applySocialDistanceEffect, threadCount);
                model->setTransitionProbabilities(transitionProbabilities);
                model->parallelSimulation(numberOfGenerations);
                //Print the individuals count based on current state.
                cout << model->getStateCount(State::dead) << endl;
            }
            if(generateImage) {
                model->generateImage();
                cout << "\nImage generated" << endl;
            }
            return EXIT_SUCCESS;
        }
        catch(invalid_argument& e)
        {
            cerr << e.what() << endl;
            abort();
        }
    }
    else {
        unique_ptr<RandomWalkModel> model;
        for(int i = 0; i < numberOfRuns; ++i) {
            model = make_unique<RandomWalkModel>(populationMatrixSize, applySocialDistanceEffect);
            model->setTransitionProbabilities(transitionProbabilities);
            model->simulation(numberOfGenerations);
            //Print the individuals count based on current state.
            cout << model->getStateCount(State::dead) << endl;
        }
        if(generateImage) {
            model->generateImage();
            cout << "\nImage generated" << endl;
        }
        return EXIT_SUCCESS;
    }
}