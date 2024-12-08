#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "State.h"

namespace Individual {
  
using namespace State;
using namespace std;

class Individual {
  public:
    State::State state;

    Individual(State::State initialState): state(initialState) {};
};

}

#endif