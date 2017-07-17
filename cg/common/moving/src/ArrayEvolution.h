#ifndef ARRAY_EVOLUTION_H
#define ARRAY_EVOLUTION_H "ArrayEvolution.h"

#include "Overture.h"

// OR InterfaceEvolution

class ArrayEvolution
{
public:

ArrayEvolution();
~ArrayEvolution();

// add a new time level
int add( real t, RealArray & x );

// Evaluate the array at a given time t
int eval( real t, RealArray & x, int numberOfDerivatives=0, int orderOfAccuracy=-1 );

// return the number of time levels currently stored:
int getNumberOfTimeLevels() const;

int setMaximumNumberOfTimeLevels(int maxLevels );

int setOrderOfAccuracy(int order );

public:

int current;  // current time level
int maximumNumberOfTimeLevels;
int orderOfTimeAccuracy;

std::vector<real> times;
std::vector<RealArray> timeHistory;

};

  

#endif
