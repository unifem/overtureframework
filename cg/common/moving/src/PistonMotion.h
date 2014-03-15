#ifndef PISTON_MOTION
#define PISTON_MOTION

// =======================================================================================
//
//  The PistonMotion class is used to define the motion of a piston for rigid body
//  and FSI computations. This class can treat pistons with specified motions and
//  pistons whose motion is driven by a fluid pressure. In some cases the piston
//  motion is knwon analytically and in other cases the motion is computed by solving
//  some ODEs (and saving the result as a Nurbs that can be subsequently evaluated).
//
// =======================================================================================

#include "Overture.h"

class NurbsMapping;
class GenericGraphicsInterface;

class PistonMotion
{
public:

enum PistonOptionsEnum
{
  specifiedPistonMotion=0,
  pressureDrivenPistonMotion,
  pressureAndBodyForcedPistonMotion  
};
  

PistonMotion();
~PistonMotion();

// copy constructor
PistonMotion( const PistonMotion & x );

// Return piston position:
real getPosition( const real t ) const;

// Return piston velocity:
real getVelocity( const real t ) const;

// get the flow solution at a point (t,x)
int 
getFlow( const real t, const real x, real & rhoTrue, real & uTrue, real & pTrue ) const;

// Get piston position and velocity
int
getPiston( const real t, real & g, real & gt ) const;

int update( GenericGraphicsInterface & gi );

protected:

int
computePistonMotion();

int 
dirkImplicitSolve( const real dt, const real aii, const real tc, const RealArray & yv, const RealArray &yv0, 
                   RealArray & kv );

void 
setGlobalConstants() const;

int 
timeStep( RealArray & yNew, RealArray & y, real t, real dt );

PistonOptionsEnum pistonOption;

real mass;  // mass of the piston
real rho0, u0, p0, gamma, a0;  // fluid properties

real bf[4]; // body force on the piston: bf[0]+bf[1]*t+bf[2]*t^2+bf[3]*t^3
real area; // cross-sectional area of the piston

real rtol;  // tolerance for the ODE solver

real newtonTol; // tolerance for Newton solve
int orderOfAccuracy;
int debug;

real dt0, cfl, tFinal;

real ag,pg;  // for specified motion

NurbsMapping *nurbs;

};

#endif
