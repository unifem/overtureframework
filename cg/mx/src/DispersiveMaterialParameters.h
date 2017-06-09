// Class to define parameters of a dispersive material

#include "Overture.h"

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;


class DispersiveMaterialParameters
{
public:

  DispersiveMaterialParameters();
  DispersiveMaterialParameters(const DispersiveMaterialParameters& x);
  ~DispersiveMaterialParameters();

  DispersiveMaterialParameters& operator=( const DispersiveMaterialParameters& x);

  // return the and imaginary parts of "s" in the dispersion relation
  int 
  computeDispersionRelation( const real cc, const real eps, const real mu, const real k, 
                             real & reS, real & imS );
  //  *OLD*
  int 
  computeDispersivePlaneWaveParameters( const real cc, const real eps, const real mu, const real k, 
                                        real & omegar, real & omegai );

// Data members -- make public for now
public:
  real gamma, omegap;  // Drude-Lorentz model

  // general dispersive model parameters:
  //   modelParameters(i,k)  : i=0,1,2,3,numberOfModelParameters-1 are the parmeters in the equation 
  //                           for P_k , k=1,2,...,numberOfPolarizationVectors
  int numberOfPolarizationVectors;
  int numberOfModelParameters;
  RealArray modelParameters;

  // The database is a place to store parameters
  mutable DataBase dbase;

};
  
