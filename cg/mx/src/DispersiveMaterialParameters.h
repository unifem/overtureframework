// Class to define parameters of a dispersive material
//
// Generalized Dispersion Model:
//       E_tt - c^2 Delta(E) = -alphaP P_tt
//       P_tt + b1 P_1 + b0 = a0*E + a1*E_t 
// 
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
  evaluateDispersionRelation( const real c, const real k, real & sr, real & si, real & psir, real & psii  );

  // *old* return the and imaginary parts of "s" in the dispersion relation
  int 
  computeDispersionRelation( const real cc, const real eps, const real mu, const real k, 
                             real & reS, real & imS );
  //  *OLD*
  int 
  computeDispersivePlaneWaveParameters( const real cc, const real eps, const real mu, const real k, 
                                        real & omegar, real & omegai );
  int setParameters( const real a0, const real a1, const real b0, const real b1 );

// Data members -- make public for now
public:

  real alphaP;
  real gamma, omegap;  // Drude-Lorentz model

  // general dispersive model parameters:
  //   modelParameters(i,k)  : i=0,1,2,3,numberOfModelParameters-1 are the parmeters in the equation 
  //                           for P_k , k=1,2,...,numberOfPolarizationVectors
  //   modelParameter(0,k) : a0
  //   modelParameter(1,k) : a1
  //   modelParameter(2,k) : b0
  //   modelParameter(3,k) : b1
  // 
  int numberOfPolarizationVectors;
  int numberOfModelParameters;
  RealArray modelParameters;

  // Save the root after it has been computed to save computations
  bool rootComputed;
  real ck0,sr0,si0,psir0,psii0;

  // The database is a place to store parameters
  mutable DataBase dbase;

};
  
