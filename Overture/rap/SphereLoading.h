#ifndef SPHERE_LOADING_H
#define SPHERE_LOADING_H

#include "Overture.h"

// ==================================================================================================
//   This class holds information defining the spheres that fill a liner geometry. 
//   It is used as an argument to the ModelBuilder::linerGeometry function.
// ==================================================================================================

class SphereLoading
{
public:

  SphereLoading(int sizeOfDistribution=1, bool fromGUI=true);
  ~SphereLoading();

  //SphereLoading(int &);
  //SphereLoading(RealArray, RealArray, RealArray, RealArray, RealArray);

  void resize(int &);
  void updateDistribution(int, RealArray, RealArray);
  void printSpheres(aString &);
  void printTecPlotSpheres(aString &);
  void printDuneSpheres(aString &);

  RealArray sphereCenter;          // holds sphere centers, sphereCenter(i,0:2) 
  RealArray sphereRadius;          // sphere radius, sphereRadius(i)
  RealArray sphereDistribution;    // sphereDistribution(j,0) = radius, sphereDistribution(j,1)=probability distribution for radius

  RealArray sphereVelocity;        // sphereVelocity(i,0:2) : initial velocity of the sphere
  RealArray sphereStartTime;       // sphere starts to move at this time.
  double    volumeFraction;        // total volume fraction for all spheres
  bool      fromGUI;
  int       RNGSeed;               // initial seed for RNG
};

#endif
