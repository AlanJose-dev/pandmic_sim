#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <random>
#include <chrono>

std::mt19937& getGenerator()
{
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

double getRandomNumber()
{
    const double minValue = 0.0;
    const double maxValue = 1.0;
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(getGenerator());
}

#endif