#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "State.h"

class Individual {
public:
    State state;

    Individual(State initialState = State::healthy) : state(initialState) {}
};

#endif