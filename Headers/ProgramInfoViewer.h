#ifndef PROGRAM_INFO_VIWER_H
#define PROGRAM_INFO_VIWER_H

#include <iostream>
#include "State.h"

using namespace std;

const char VERSION[] = "0.4.2";

inline const char * const boolToString(bool boolean)
{
  return boolean ? "Yes" : "No";
}

void printHeaders(int intParams[4], bool boolParams[2])
{
    cout << "---------- PANDEMIC_SIM ----------" << endl;
    cout << "-- Number of runs: " << intParams[0] << endl;
    cout << "-- Population matrix size: " << intParams[1] << endl;
    cout << "-- Number of generations: " << intParams[2] << endl;
    cout << "-- Social distance effect applyied: " << boolToString(boolParams[0]) << endl;
    cout << "-- Threads: " << intParams[3] << endl;
    cout << "-- Generate visual example image on finish: " << boolToString(boolParams[1]) << endl;
    cout << "----------------------------------" << endl;
    delete[] intParams;
    delete[] boolParams;
}

void printHelp()
{
    cout << "Usage: simulator [-v | --version] [-h | --help] [-r | --runs] [-p | --population <value>]" << endl;
    cout << "                 [-g | --generations <value>] [-s | --social-distance-effect] [-t | --threads <value>]" << endl;
    cout << "                 [-i | --image]" << endl;
    cout << "\n" << endl;
    cout << "Params description:" << endl;
    cout << "-v | --version:                      Show the program version" << endl;
    cout << "-h | --help:                         Show this message" << endl;
    cout << "-r | --runs:                         Defines how many times the model will be executed, determining the results count" << endl;
    cout << "-p | --population:                   Define the population matrix side length, use the square root, e.g 100 corresponds 10000 (integer)" << endl;
    cout << "-g | --generations:                  Specify the generations number in weeks (integer)" << endl;
    cout << "-s | --social-distance-effect:       Run the simmulations with the social distance/lockdown effect applyied, it reduces the disease contagion factor" << endl;
    cout << "-t | --threads:                      Run the simmulations with multi-thread profile, specifies the threads count that the program may use. The maximum value is the current processor available threads (integer)" << endl;
    cout << "-i | --image:                        Generates a visual disease spread example as a .png image" << endl;
}

void printVersion()
{
    cout << "Pandemic Sim version " << VERSION << endl;
    cout << "Author: Alan José <alanjsdelima@gmail.com>"<< endl;
    cout << "Public Domain Code, feel free to use, modify and redistribute it."<< endl;
}

#endif