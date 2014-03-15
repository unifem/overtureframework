#include "Overture.h"
#include "ParallelUtility.h"
#include "App.h"

int applyBoundaryConditions( realCompositeGridFunction & u, real t );
int dudt( realCompositeGridFunction & u, realCompositeGridFunction & ut, real t );

int 
rungeKutta1(real & t, 
           real dt,
           realCompositeGridFunction & u1, 
           realCompositeGridFunction & u2 )
// ================================================================================
// /Description:
//  Advance some time steps - forward Euler.
//
//       y(n+1) = yn + k1
//           k1 = dt*f(t,yn)
//
//   /t (input) : current time
//   /dt (input) : time step.
//   /u1 (input/output) : solution at time t on input, solution at time t+dt at output
//   /u2  : work spaces
//======================================================================
{
  CompositeGrid & cg = *u1.getCompositeGrid();
  dudt( u1,u2,t ); 
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    #ifdef USE_PPP
      realSerialArray u1g; getLocalArrayWithGhostBoundaries(u1[grid],u1g);
      realSerialArray u2g; getLocalArrayWithGhostBoundaries(u2[grid],u2g);
    #else
      realSerialArray & u1g = u1[grid];
      realSerialArray & u2g = u2[grid];
    #endif
    u1g+=dt*u2g; 
  }
  applyBoundaryConditions( u1,t+dt );

  t+=dt;

  return 0;
}

int 
rungeKutta2(real & t, 
           real dt,
           realCompositeGridFunction & u1, 
           realCompositeGridFunction & u2, 
           realCompositeGridFunction & u3 )
// ================================================================================
// /Description:
//  Advance some time steps - Second Order Runge-Kutta
//
//       y(n+1) = yn + 1/2( k1 + k2 )
//           k1 = dt*f(t,yn)
//           k2 = dt*f(t+h,yn+k1)
//
//   /t (input) : current time
//   /dt (input) : time step.
//   /u1 (input/output) : solution at time t on input, solution at time t+dt at output
//   /u2,u3  : work spaces
//======================================================================
{
  CompositeGrid & cg = *u1.getCompositeGrid();
  dudt( u1,u2,t ); 

  checkArrayIDs("rk2: after dudt"); 

  real dtb2=dt*.5;
  
  int grid;
  Index I1,I2,I3;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    if( !hasSameDistribution(u1[grid],u2[grid]) ||
        !hasSameDistribution(u1[grid],u3[grid]) )
    {
      printf("rungeKutta2:ERROR: u1,u2,u3 do NOT have the same parallel distribution!\n");
      Overture::abort("error");
    }
    testConsistency(u1[grid],"rungeKutta2");
    testConsistency(u2[grid],"rungeKutta2");
    testConsistency(u3[grid],"rungeKutta2");
    

    #ifdef USE_PPP
      realSerialArray u1g; getLocalArrayWithGhostBoundaries(u1[grid],u1g);
      realSerialArray u2g; getLocalArrayWithGhostBoundaries(u2[grid],u2g);
      realSerialArray u3g; getLocalArrayWithGhostBoundaries(u3[grid],u3g);
    #else
      realSerialArray & u1g = u1[grid];
      realSerialArray & u2g = u2[grid];
      realSerialArray & u3g = u3[grid];
    #endif
    getIndex(cg[grid].dimension(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1g,I1,I2,I3,1);      
    if( !ok ) continue;
    
    u3g=u1g+dt*u2g;
    u1g+=dtb2*u2g;
  }
  
  checkArrayIDs("rk2: before applyBC(1)"); 
  applyBoundaryConditions( u3,t+dt );
  checkArrayIDs("rk2: after applyBC(1)"); 

  dudt( u3,u2,t+dt );  
  
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    #ifdef USE_PPP
      realSerialArray u1g; getLocalArrayWithGhostBoundaries(u1[grid],u1g);
      realSerialArray u2g; getLocalArrayWithGhostBoundaries(u2[grid],u2g);
    #else
      realSerialArray & u1g = u1[grid];
      realSerialArray & u2g = u2[grid];
    #endif
    getIndex(cg[grid].dimension(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1g,I1,I2,I3,1);      
    if( !ok ) continue;

    u1g+=dtb2*u2g;
  }
  
  applyBoundaryConditions( u1,t+dt );

  t+=dt;

  return 0;
}


int 
rungeKutta4(real & t, 
	    real dt,
	    realCompositeGridFunction & u1, 
	    realCompositeGridFunction & u2, 
	    realCompositeGridFunction & u3, 
	    realCompositeGridFunction & u4 )
// ================================================================================
// /Description:
//  Advance some time steps - Fourth Order Runge-Kutta
//
//       y(n+1) = yn + 1/6( k1 + 2*k2 + 2*k3 + k4 )
//           k1 = dt*f(t,yn)
//           k2 = dt*f(t+.5*h,yn+.5*k1)
//           k3 = dt*f(t+.5*h,yn+.5*k2)
//           k4 = dt*f(t+h,yn+k3)
//
//   /t (input) : current time
//   /dt (input) : time step.
//   /u1 (input/output) : solution at time t on input, solution at time t+dt at output
//   /u2,u3,u4  : work spaces
//======================================================================
{
  CompositeGrid & cg = *u1.getCompositeGrid();
  
  dudt( u1,u2,t );  // ... u2 <- k1=du1/dt(t)

  real dtb2=dt*.5;
  real dtb3=dt/3.;
  real dtb6=dt/6.;
  
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    #ifdef USE_PPP
      realSerialArray u1g; getLocalArrayWithGhostBoundaries(u1[grid],u1g);
      realSerialArray u2g; getLocalArrayWithGhostBoundaries(u2[grid],u2g);
      realSerialArray u3g; getLocalArrayWithGhostBoundaries(u3[grid],u3g);
      realSerialArray u4g; getLocalArrayWithGhostBoundaries(u4[grid],u4g);
    #else
      realSerialArray & u1g = u1[grid];
      realSerialArray & u2g = u2[grid];
      realSerialArray & u3g = u3[grid];
      realSerialArray & u4g = u4[grid];
    #endif
    u3g=u1g+dtb2*u2g;   // ...u3 <- yn+.5*k1
    u4g=u1g+dtb6*u2g;   // ...u4 <- yn+1/6( k1 )   keep a running sum of the result (saves space)
  }
  
  applyBoundaryConditions( u3,t+dtb2 );
  dudt( u3,u2,t+dtb2 );  //  ...u2 <- k2 = f(u3)
  
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    #ifdef USE_PPP
      realSerialArray u1g; getLocalArrayWithGhostBoundaries(u1[grid],u1g);
      realSerialArray u2g; getLocalArrayWithGhostBoundaries(u2[grid],u2g);
      realSerialArray u3g; getLocalArrayWithGhostBoundaries(u3[grid],u3g);
      realSerialArray u4g; getLocalArrayWithGhostBoundaries(u4[grid],u4g);
    #else
      realSerialArray & u1g = u1[grid];
      realSerialArray & u2g = u2[grid];
      realSerialArray & u3g = u3[grid];
      realSerialArray & u4g = u4[grid];
    #endif
    u3g=u1g+dtb2*u2g;   // ...yn+.5*k2
    u4g+=dtb3*u2g;     // ...yn+1/6( k1 +2*k2 )
  }
  
  applyBoundaryConditions( u3,t+dtb2 );
  dudt( u3,u2,t+dtb2 ); // ...u2 <- k3 = f(u3)

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    #ifdef USE_PPP
      realSerialArray u1g; getLocalArrayWithGhostBoundaries(u1[grid],u1g);
      realSerialArray u2g; getLocalArrayWithGhostBoundaries(u2[grid],u2g);
      realSerialArray u3g; getLocalArrayWithGhostBoundaries(u3[grid],u3g);
      realSerialArray u4g; getLocalArrayWithGhostBoundaries(u4[grid],u4g);
    #else
      realSerialArray & u1g = u1[grid];
      realSerialArray & u2g = u2[grid];
      realSerialArray & u3g = u3[grid];
      realSerialArray & u4g = u4[grid];
    #endif
    u3g=u1g+dt*u2g;    // ...yn+k3
    u4g+=dtb3*u2g;    // ...yn+1/6( k1 +2*k2 +2*k3 )
  }
 
  applyBoundaryConditions( u3,t+dt );
  dudt( u3,u2,t+dt ); //  ...u2 <- k4 = f(u3)

 
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    #ifdef USE_PPP
      realSerialArray u1g; getLocalArrayWithGhostBoundaries(u1[grid],u1g);
      realSerialArray u2g; getLocalArrayWithGhostBoundaries(u2[grid],u2g);
      realSerialArray u4g; getLocalArrayWithGhostBoundaries(u4[grid],u4g);
    #else
      realSerialArray & u1g = u1[grid];
      realSerialArray & u2g = u2[grid];
      realSerialArray & u4g = u4[grid];
    #endif
    u1g=u4g+dtb6*u2g;
  }
  
  applyBoundaryConditions( u1,t+dt );

  t+=dt;

  return 0;
}

