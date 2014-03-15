#ifndef GODUNOV_H
#define GODUNOV_H

#include "OB_Parameters.h"

class Godunov
{
  public:

// constructor and destructor
    Godunov(OB_Parameters & p);
   ~Godunov();

// timings
//    real timeFlux;

// parameters for equations
    OB_Parameters & parameters;

// public member functions

    int getSlopeCorrection (const IntegerArray & nr, const real & dt, const RealArray & ds,
		            const realMappedGridFunction & rx, const realArray & det, 
                            const bool gridIsMoving, const realArray & vGrid,
                            const realArray & u, realArray & du, realArray & h);

    int getEigenvalues (const IntegerArray & nd,
                        const realArray & alpha, const realArray & beta,
                        const realArray & u, realArray & vn, realArray & c,
                        realArray & el, realArray & er);

    int getFlux (const IntegerArray & nr,
                 const realMappedGridFunction & rx, const realArray & det, 
                 const bool gridIsMoving, const realArray & vGrid,
                 const realArray & u, const realArray & du,
                 realArray & f, realArray & maxEigenvalue);

    int approximateRiemannSolver (const realArray & map, const realArray& ul, const realArray & ur,
                                  const realArray & f, real & maxEigenvalue);

    int getSource (const IntegerArray & nd, const real & dt,
                   const realArray & u, const realArray & v, realArray & up);

    realArray reactionRate (const realArray & v, const realArray & w);

    realArray & state (const realArray & energy, const realArray & lambda,
                       realArray & temperature, realArray & gamma, realArray & heat);

};

#endif
