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

int 
computeDispersivePlaneWaveParameters( const real cc, const real eps, const real mu, const real k, 
                                      real & omegar, real & omegai );

// Data members -- make public for now
  public:
  real gamma, omegap;  // Drude-Lorentz model

// The database is a place to store parameters
mutable DataBase dbase;

};
  
