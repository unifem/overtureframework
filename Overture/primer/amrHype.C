#include "Overture.h"  
#include "PlotStuff.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "OGPulseFunction.h"

#include "Regrid.h"
#include "ErrorEstimator.h"
#include "InterpolateRefinements.h"
#include "Ogen.h"

#include "interpPoints.h"

// global variables for convenience:
static real a=1.,b=1.,nu=.005;

real
getDt(const real & cfl, 
      const real & a, 
      const real & b, 
      const real & nu, 
      MappedGrid & mg, 
      MappedGridOperators & op,
      const real alpha0 = -2.,
      const real beta0  = 1. );

int 
applyBoundaryConditions( realCompositeGridFunction & u, real t )
// =================================================================================================
// /Description:
//      Apply boundary conditions for an overlapping grids with refinements.
// =================================================================================================
{
  u.interpolate();  
  
  // apply true boundary conditions.
  u.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t);
  u.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t);
  u.finishBoundaryConditions();
  return 0;
}

int 
dudt( realCompositeGridFunction & u, realCompositeGridFunction & ut, real t )
{
  CompositeGrid & cg = *u.getCompositeGrid();

  if( cg.numberOfDimensions()==2 )
  {
    ut=(-a)*u.x()+(-b)*u.y() + nu*u.laplacian();
  }
  else if( cg.numberOfDimensions()==1 )
  {
    ut=(-a)*u.x() + nu*u.laplacian();
  }
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
  dudt( u1,u2,t );  // ... u2 <- k1=du1/dt(t)

  real dtb2=dt*.5;
  real dtb3=dt/3.;
  real dtb6=dt/6.;
  
  u3=u1+dtb2*u2;   // ...u3 <- yn+.5*k1
  u4=u1+dtb6*u2;   // ...u4 <- yn+1/6( k1 )   keep a running sum of the result (saves space)

  applyBoundaryConditions( u3,t+dtb2 );
  dudt( u3,u2,t+dtb2 );  //  ...u2 <- k2 = f(u3)
  
  u3=u1+dtb2*u2;   // ...yn+.5*k2
  u4+=dtb3*u2;     // ...yn+1/6( k1 +2*k2 )
  
  applyBoundaryConditions( u3,t+dtb2 );
  dudt( u3,u2,t+dtb2 ); // ...u2 <- k3 = f(u3)

  u3=u1+dt*u2;    // ...yn+k3
  u4+=dtb3*u2;    // ...yn+1/6( k1 +2*k2 +2*k3 )
 
  applyBoundaryConditions( u3,t+dt );
  dudt( u3,u2,t+dt ); //  ...u2 <- k4 = f(u3)

 
  u1=u4+dtb6*u2;
  applyBoundaryConditions( u1,t+dt );

  t+=dt;

  return 0;
}


real
getTimeStep( CompositeGrid & cg, CompositeGridOperators & op, real cfl, real cflr, real cfli )
// ==========================================================================================
//  Determine the time step
// =========================================================================================
{
  real dt=REAL_MAX;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    real dtGrid=0.;
    if( cg.numberOfDimensions()==2 )
    {
      cg.update(MappedGrid::THEinverseVertexDerivative );
      dtGrid = getDt(cfl,a,b,nu,cg[grid],op[grid],cflr,cfli);
    }
    else if( cg.numberOfDimensions()==1 )
    {
      dtGrid=.5*cfl/( 2.*fabs(a)/cg[grid].gridSpacing(axis1) +
		      4.*nu/SQR(cg[grid].gridSpacing(axis1)) );
    }
    else
    {
      throw "error";
    }
    dt=min(dt,dtGrid);
  }

  return dt;
}


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  Range all;

  printf(" -------------------------------------------------------------------- \n");
  printf(" Hyperbolic Adaptive Mesh Refinement Solver                           \n");
  printf("   Convection diffusion equation: u.t + a u.x + b u.y = nu Delta u    \n");
  printf("   Time Stepping: 4th order Runge Kutta                               \n");
  printf(" Usage: `amrHype [gridName]'  gridName=(optional) name of grid to use \n");
  printf(" -------------------------------------------------------------------- \n");

  aString nameOfOGFile="square20.hdf";
  if( argc>1 )
  {
    nameOfOGFile=argv[1];           // first command line arg can be the name of a grid.
  }

  PlotStuff ps(TRUE, "amrHype");       // create a PlotStuff object
  PlotStuffParameters psp;             // This object is used to change plotting parameters
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,TRUE);
  aString answer;
  char buff[80];
  
  // cga[2] : old and new Composite grids (ring buffer).
  CompositeGrid cga[2];
  CompositeGrid & cg = cga[0];
  getFromADataBase(cg,nameOfOGFile);
  cga[1]=cg; 
  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );

  Regrid regrid;  // this object knows how to build refinements from an error array
  InterpolateRefinements interp(cg.numberOfDimensions()); // interpolator for refinements
  interp.setOrderOfInterpolation(3);

  ErrorEstimator errorEstimator(interp);  // used to compute error estimates
  real errorThreshold=.1;   // error tolerance for regridding.
  // set the scale factors for the solution (estimate of solution size)
  RealArray scaleFactor(1);
  scaleFactor=1.;
  errorEstimator.setScaleFactor( scaleFactor );

  Ogen ogen;  // required when adapting overlapping grids.

  Interpolant interpolant;   // overlapping grid interpolator
  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
  interpolant.setInterpolateRefinements(interp);

  // AMR parameters:
  int baseLevel=0; // always regenerate to this level
  int numberOfRefinementLevels=3; 
  real efficiency=.7; 
  regrid.setEfficiency(efficiency);

  // assign the refinement ratio (4 is another possible value)
  int refinementRatio=2;
  regrid.setRefinementRatio(refinementRatio);

  // define initial condition as a pulse:
  OGPulseFunction pulse;
  real pulseCentre[3]={.25,.25,0.}, pulseRadius=.025;
//  real pulseCentre[3]={-.7,-.7,0.}, pulseRadius=.025;
  real pulseRadiusX=0., pulseRadiusY=0., pulseRadiusZ=0.;
  pulse.setRadius(pulseRadius);
  pulse.setCentre(pulseCentre[0],pulseCentre[1],pulseCentre[2]);
      
  
  // define solution and operators:
  realCompositeGridFunction u(cg), error;
  CompositeGridOperators op(cg);

  u=pulse(cg,Range(0,0));
  u.setOperators(op);
      
  int currentGrid=0;
  // =================== build the initial AMR grid =========================
  for( int level=1; level<numberOfRefinementLevels; level++ )
  {
    int nextGrid= (currentGrid+1) % 2;

    CompositeGrid & cgc = cga[currentGrid];
    CompositeGrid & cgNew = cga[nextGrid];

    error.updateToMatchGrid(cgc);
    interpolant.updateToMatchGrid(cgc,level-1);  // error estimator needs to interpolate
        
    op.updateToMatchGrid(cgc);
    error.setOperators(op);

    errorEstimator.computeAndSmoothErrorFunction( u,error );  // compute an error estimate

    printf("Error: min=%e, max=%e \n",min(error),max(error));

    regrid.regrid(cgc,cgNew, error, errorThreshold, level, baseLevel);

    ogen.updateRefinement(cgNew);

    cgNew.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
	
    u.updateToMatchGrid(cgNew);
    u=pulse(cgNew,Range(0,0));   // re-evaluate the initial conditions on the new grid.

    currentGrid = (currentGrid+1) % 2;
  }

  CompositeGrid & cgc = cga[currentGrid];
      
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  psp.set(GI_TOP_LABEL,"initial conditions on refined grid");
  PlotIt::contour(ps,u,psp);
  PlotIt::plot(ps,cgc,psp);

  op.updateToMatchGrid(cgc);
  // define work-space grid functions for Runge-Kutta
  realCompositeGridFunction u1(cgc),u2(cgc),u3(cgc);
  u.setOperators(op);
  u1.setOperators(op);
  u2.setOperators(op);
  u3.setOperators(op);

  int step;
  int maximumNumberOfSteps=1000;
      
  const int baseLevelToUpdate=1;
  interpolant.updateToMatchGrid(cgc,baseLevelToUpdate);
	
  // =========== compute the time step =================
  real dt=.01, t=0.;
  real cfl=.9;
  real cflr=2.7; // stability bounds for RK4
  real cfli=2.7;
  dt = getTimeStep( cgc,op,cfl,cflr,cfli );
  printf("*** dt=%9.3e\n",dt);
      
  aString menu2[]=
  {
    "step",
    "movie mode",
    "final time",
    "anu",
    "cfl",
    "plot interval",
    "contour",
    "grid",
    "erase",
    "exit",
    "" 
  };

  real tFinal=.3;
  real tPrint=.025;
  int regridInterval=refinementRatio;
  int plotInterval=max(4,int(tPrint/dt+.5));
  int movieModePlotInterval=plotInterval;

  bool movieMode=false;
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      
  bool finished=false;
      
  bool plotOption=1+2; // plot contour and grid.

  real cpu0=getCPU();

  for( step=0; step<maximumNumberOfSteps; step++ )
  {
    if( !movieMode || t>tFinal-dt*1.e-4 )
    {
      for( ;; )
      {
	ps.getMenuItem(menu2,answer,"choose");
	if( answer=="contour" )
	{
	  plotOption|=1;
	  ps.erase();
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  PlotIt::contour(ps,u,psp);
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	}
	else if( answer=="grid" )
	{
	  plotOption|=2;
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  PlotIt::plot(ps,cga[currentGrid],psp);
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	}
	else if( answer=="step" )
	{
	  plotInterval=1;
	  break;
	}
	else if( answer=="final time" )
	{
	  ps.inputString(answer,"Enter the final time");
	  sScanF(answer,"%e",&tFinal);
	  printf("tFinal=%e\n",tFinal);
	}
	else if( answer=="plot interval" )
	{
	  ps.inputString(answer,"Enter plot interval");
	  sScanF(answer,"%i",&plotInterval);
	  printf("plot interval=%i\n",plotInterval);
	  movieModePlotInterval=plotInterval;
	}
	else if( answer=="movie mode" )
	{
	  plotInterval=movieModePlotInterval;
	      
	  movieMode=true;
	  break;
	}
	else if( answer=="erase" )
	{
	  plotOption=0;
	  ps.erase();
	}
	else
	{
	  finished=true;
	  break;
	}
      }
    }
	
    if( finished )
      break;
	
    if( numberOfRefinementLevels>1 && step>0 && (step % regridInterval == 0) )
    {
      // ****** regrid *************

      printf(" ***** regrid currentGrid=%i step=%i **********\n",currentGrid,step);
      int nextGrid= (currentGrid+1) % 2;
      CompositeGrid & cgc = cga[currentGrid];
      CompositeGrid & cgNew = cga[nextGrid];
      error.updateToMatchGrid(cgc);

      errorEstimator.computeAndSmoothErrorFunction(u,error );     // compute an error estimate
      printf("Error: min=%e, max=%e \n",min(error),max(error));

      regrid.regrid(cgc,cgNew, error, errorThreshold, numberOfRefinementLevels-1, baseLevel);
      regrid.printStatistics(cgNew);

      ogen.updateRefinement(cgNew);
      cgNew.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
 
      // Interpolate the old solution (u) onto the new grid (u1)
      u.applyBoundaryCondition(0,BCTypes::extrapolateInterpolationNeighbours);
      u1.updateToMatchGrid(cgNew);
      interp.interpolateRefinements( u,u1 );
 	  
      // update the operators and Interpolant
      op.updateToMatchGrid(cgNew);
      interpolant.updateToMatchGrid(cgNew,baseLevelToUpdate);

      u.updateToMatchGrid(cgNew);
      u.setOperators(op);
      u.dataCopy(u1);
      applyBoundaryConditions(u,t);

      printf(" old dt=%9.3e, ",dt);
      dt = getTimeStep( cgNew,op,cfl,cflr,cfli );
      printf("new dt=%9.3e\n",dt);

      // redimension work functions:
      u2.updateToMatchGrid(cgNew); 
      u3.updateToMatchGrid(cgNew); 
      
      u1.setOperators(op);
      u2.setOperators(op);
      u3.setOperators(op);


      currentGrid = (currentGrid+1) % 2;
    }
           
    real dtSave=dt;
    if( t+dt > tFinal ) // adjust last time step to reach tFinal
    {
      dt=tFinal-t;
    }
	
    CompositeGrid & cgc = cga[currentGrid];

    // ===== Take a time step ======
    rungeKutta4( t,dt,u,u1,u2,u3 );

    dt=dtSave;
	
    if( step % plotInterval == 0 || ( t > tFinal-.5*dt ) )
    {
      printf("***** t=%8.2e, dt=%8.2e, cpu=%8.2e\n",t,dt,getCPU()-cpu0);

      psp.set(GI_TOP_LABEL,sPrintF(buff,"u t=%8.2e, dt=%8.2e, nu=%8.2e",t,dt,nu));
      ps.erase();
      if( plotOption & 2 )
	PlotIt::plot(ps,cgc,psp);
      if( plotOption & 1 )
	PlotIt::contour(ps,u,psp);
      ps.redraw(movieMode);
    }
    
  }  // for( step ..
  


  Overture::finish();          
  return 0;
}


