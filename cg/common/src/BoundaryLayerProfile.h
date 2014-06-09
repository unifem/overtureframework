#ifndef BOUNDARY_LAYER_PROFILE_H
#define BOUNDARY_LAYER_PROFILE_H

#include "Overture.h"

// ========================================================================================================
/// \brief Class to evaluate a boundary layer profile
// ========================================================================================================
class BoundaryLayerProfile
{

public:

BoundaryLayerProfile();
~BoundaryLayerProfile();

int setParameters( real nu, real U );

// Evaluate the solution u(x,y) and v(x,y)
int eval( const real x, const real y, real & u, real & v );


// Evaluate the Blasius flat-plate profile
int evalBlasius( const real eta, real & f , real & fp );

static int debug;

protected:

int initializeProfile();

int blasiusFunc( real *fv, real *fvp );

real nu;  // kinematic viscosity
real U;   // free-stream velocity

bool initialized;
int nEta;  // number of eta points in spline
RealArray eta,f,fp,bcd;  // for spline fit 
real etaMin, etaMax;     // evaluate Blasius function over this range


};
#endif
