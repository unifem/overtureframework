#ifndef TRAVELING_WAVE_FSI_H
#define TRAVELING_WAVE_FSI_H

// ===========================================================
// This class defines some exact solutions for FSI
// ===========================================================

#include "Overture.h"

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

class GraphicsParameters;



class TravelingWaveFsi
{
public:

enum TwlightZoneOptionEnum
{
  polynomial=0,
  trigonometric=1
};


TravelingWaveFsi();
~TravelingWaveFsi();


int 
advance( real t );

int
advanceInsShell( real t );

int 
applyBoundaryConditions( RealArray & us, RealArray & uf, const real t );

int 
assignInitialConditions( const real t, const real dt );

int 
computeErrors( real t );

int 
getExactFluidSolution( realArray & u, real t, MappedGrid & mg, const Index & I1, const Index & I2, const Index & I3 );

int 
getExactSolidSolution( realArray & u, real t, MappedGrid & mg, const Index & I1, const Index & I2, const Index & I3 );

int 
getExactShellSolution( const realArray & x, realArray & ue, realArray & ve, realArray & ae, real t, 
		       const Index & I1, const Index & I2, const Index & I3 );
real 
getTimeStep( real t );

int 
getUt( RealArray & us, RealArray & u, 
       RealArray & ust, RealArray & ut, real t );

int 
getUtInsShell( RealArray & us, RealArray & u, 
               RealArray & ust, RealArray & ut, real t );

int 
plot( GenericGraphicsInterface & gi, realCompositeGridFunction & uPlot, GraphicsParameters & psp );

int 
rungeKutta4(real t, 
	    real dt,
	    realArray & us1, realArray & uf1,
	    realArray & usNew, realArray & ufNew );

int 
setup( CompositeGrid & cg, CompositeGrid & cgSolid );

int 
update( GenericGraphicsInterface & gi );

// The database is used to hold parameters
mutable DataBase dbase; 

};

#endif
