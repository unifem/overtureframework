#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "Overture.h"
#include "GridDistribution.h"


class LoadBalancer
{
public:

enum LoadBalancerTypeEnum
{
  defaultLoadBalancer=0,
  KernighanLin,
  sequentialAssignment, // grid g is placed on processor p= g % np;
  randomAssignment,     // grid g is placed in a random processor -- this is used for testing
  allToAll,             // grid g is given all processors
  userDefined,
  numberOfLoadBalanceTypes
};


LoadBalancer();
~LoadBalancer();

// Actually assign the load balance to the grids in the GridCollection:
int assignLoadBalance( GridCollection & gc, GridDistributionList & gridDistributionList,
                       int refinementLevel = 0 ) const;

// Assign work loads for each grid based on the number of grid points
int assignWorkLoads( GridCollection & gc, GridDistributionList & gridDistributionList,
                     int refinementLevel = 0 ) const;

// Assign work loads for each grid based on the number of grid points (this version takes a GridCollectionData)
int assignWorkLoads( GridCollectionData & gc, GridDistributionList & gridDistributionList,
                     int refinementLevel = 0 ) const;

// main function for load balancing - determine the load balance (but do not apply)
int determineLoadBalance( GridDistributionList & gridDistributionList,
                          int refinementLevel = 0, int mgStart=0, int mgEnd=INT_MAX ) const;

// get the load for a gridCollection using the default work-loads based on the number of grid points
int determineLoadBalance( GridCollection & gc, GridDistributionList & gridDistributionList,
                          int refinementLevel = 0, int mgStart=0, int mgEnd=INT_MAX ) const;

// implementations of different load balancing algorithms:
int determineLoadBalanceKernighanLin( GridDistributionList & gridDistributionList,
                                      int refinementLevel = 0, int mgStart=0, int mgEnd=INT_MAX ) const;
int determineLoadBalanceUserDefined( GridDistributionList & gridDistributionList,
                                     int refinementLevel = 0, int mgStart=0, int mgEnd=INT_MAX ) const;

LoadBalancerTypeEnum getLoadBalancerType() const;
aString getLoadBalancerTypeName() const;

// print statistics from the load balancing
int printStatistics(FILE *file= NULL);

// Specify which processors to load balance over:
int setProcessors(int pStart, int pEnd);

// specify which load balancing algorithm to use:
int setLoadBalancer(LoadBalancerTypeEnum loadBalancer );

// set target maximum realtive load imbalance:
int setTargetMaximumLoadImbalance( real target );

int update( GenericGraphicsInterface & gi );

protected:

// update statistics
int saveStatistics(GridDistributionList & gridDistributionList ) const;

int np;            // number of processors we load balance over
int *processorID;  // list of processor id's we load balance over

LoadBalancerTypeEnum loadBalancer;
real targetMaximumLoadImbalance;  // gives the target maximum relative load imbalance

static int debug;

// These are for statistics:
static int numberOfLoadBalances;
static real maximumImbalance, averageImbalance, averageNumberOfGrids, averageNumberOfBlocks, averageWorkPerBlock;

};


#endif
