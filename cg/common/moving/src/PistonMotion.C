// ====================================================================================
///  \file PistonMotion.C
///  \brief Define piston motions for moving grid exact solutions.
// ===================================================================================

#include "PistonMotion.h"
#include "PlotStuff.h"
#include "NurbsMapping.h"
#include "GenericGraphicsInterface.h"
#include "display.h"

// =======================================================================================
//
//  The PistonMotion class is used to define the motion of a piston for rigid body
//  and FSI computations. This class can treat pistons with specified motions and
//  pistons whose motion is driven by a fluid pressure. In some cases the piston
//  motion is knwon analytically and in other cases the motion is computed by solving
//  some ODEs (and saving the result as a Nurbs that can be subsequently evaluated).
//
// =======================================================================================


// ************* FIX ME ****************
static real cc0,cc1,cc2,cc3,cc4,cc5,cc6,cc7,cc8,cc9;
static real ag,pg;

static PistonMotion::PistonOptionsEnum pistonMotionOption;
static PistonMotion *pistonMotion=NULL;

#define dfmin EXTERN_C_NAME(dfmin)
extern "C"
{
  real dfmin(real (*f) (const real&), real &x, real&a, real&b, const real&tol);
}

// Solution to the hydrodynamically forced piston problem and the time derivative:
#define G_FORCED_PISTON(t) \
      ( (-1./cc1)*( (t) - (cc2-1.)/(cc2-2.)/(cc3*cc5)*(pow( 1.+cc3*cc5*(t), (cc2-2.)/(cc2-1.) ) -1. ) ) )
#define GT_FORCED_PISTON(t) \
      ( (-1./cc1)*( 1. - pow( 1.+cc3*cc5*(t), (-1.)/(cc2-1.) ) ) )

// Characteristic function for the hydrodynamically forced piston
 //     H(tau) =   g(tau) + (a0 + gp1*.5* gDot(tau) )(t-tau) - x = 0
#define CHARACTERISTIC_FORCED_PISTON(tau) \
    ( G_FORCED_PISTON(tau) + (cc6+cc7*GT_FORCED_PISTON(tau))*(cc8-(tau))+cc9 )

namespace
{

extern "C"
{
 // evaluate the characteristic function used to define the boundary position
 //     H(tau) =   g(tau) + (a0 + gp1*.5* gDot(tau) )(t-tau) - x = 0
 //   expanding piston: write in the form :  H(tau)= cc0 + cc1*tau + cc2*tau^2 + cc3*tau^3
 //   ****NOTE:  Return the square of the function:  H^2(t) (instead  of H(t)) -- to which we find a minimum
 real pistonBoundaryFunctionPM( const real & tau )
 { 
   real hh;
   if( pistonMotionOption==PistonMotion::specifiedPistonMotion )
   {
     real g = ag*pow(tau,pg);
     real gt= ag*pg*pow(tau,pg-1.);
     hh = g + (cc6+cc7*gt)*(cc8-(tau))+cc9;
   }
   else if( pistonMotionOption==PistonMotion::pressureDrivenPistonMotion )
   {
     hh = CHARACTERISTIC_FORCED_PISTON(tau);
   }
   else if( pistonMotionOption==PistonMotion::pressureAndBodyForcedPistonMotion )
   {
     assert( pistonMotion!=NULL );
     real g,gt;
     pistonMotion->getPiston( tau,g,gt );
     hh = g + (cc6+cc7*gt)*(cc8-(tau))+cc9;
     // printF("pistonBoundaryFunctionPM: tau=%9.3e, cc8=%8.2e, hh =%9.3e\n",tau,cc8,hh);
   }
   
   return hh*hh;
 }
 
}
}


PistonMotion::
PistonMotion()
{
  pistonOption=pressureDrivenPistonMotion;

  mass=1.;
  gamma=1.4;
  rho0=gamma;
  u0=0.;
  p0=1.;
  a0=sqrt(gamma*p0/rho0);

  bf[0]=bf[1]=bf[2]=bf[3]=0.; // body force on the piston: bf[0]+bf[1]*t+bf[2]*t^2+bf[3]*t^3
  area=1.;

  pg=3.; ag=-1./pg;   // for specified motion:  g(t)= ag*t^pg  (piston with specfied motion)

  rtol=1.e-6;  // tolerance for ODE solver

  newtonTol=1.e-7;

  dt0=-1.;  // if dt0 is set then use this as the time step for the ODE solver
  cfl=.5;
  tFinal=1.;


  orderOfAccuracy=4;

  debug=3;
  nurbs=NULL;
  
  setGlobalConstants();
  
}


PistonMotion::
~PistonMotion()
{
  delete nurbs;
}

// ======================================================================================
/// \brief copy constructor
// ======================================================================================
PistonMotion::
PistonMotion( const PistonMotion & x )
{
 pistonOption=x.pistonOption;
 mass=x.mass;
 gamma=x.gamma;
 rho0=x.rho0;
 u0=x.u0;
 p0=x.p0;
 a0=x.a0;
 for( int j=0; j<4; j++ ) bf[j]=x.bf[j];
 area=x.area;
 pg=x.pg;
 ag=x.ag;
 rtol=x.rtol;
 newtonTol=x.newtonTol;
 dt0=x.dt0;
 cfl=x.cfl;
 tFinal=x.tFinal;
 orderOfAccuracy=x.orderOfAccuracy;
 debug=x.debug;

 assert( x.nurbs==NULL );  // what should we do if x.nurbs is not NULL ??
 nurbs=NULL;

 setGlobalConstants();

}


void PistonMotion::
setGlobalConstants() const
{
  // global constants for the hydrodynamically forced piston:
  cc1=(gamma-1.)/(2.*a0);
  cc2=2.*gamma/(gamma-1.);
  cc3=p0*area/mass;  // p0*A/M // cc3=p0*H/Mass;
  cc4=0.;
  cc5=cc1*(cc2-1.);
}

  

// ======================================================================================
/// \brief Compute the fluid solution at a point (t,x)
///
// ======================================================================================
int PistonMotion::
getFlow( const real t, const real xx, real & rhoTrue, real & uTrue, real & pTrue ) const
{
  const real gp1 = gamma+1., gm1=gamma-1.;
  const real tol=REAL_EPSILON*100.;
  
  // set global variables:
  pistonMotion=(PistonMotion*)this;
  
  pistonMotionOption=pistonOption;  
  ::ag=ag;
  ::pg=pg;

  // global constants for the hydrodynamically forced piston:
  setGlobalConstants();

  if( xx>a0*t )
  {
    uTrue = 0.; // -gt;
  }
  else
  {
    // note: these next (global) variables are used in the definition of the "pistonBoundaryFunction" 
    cc6=a0, cc7=gp1*.5, cc8=t, cc9=-xx;
		
    // *********************************************************************
    // Find the value of tau where the following function is zero : 
    //     g(tau) + (a0 + gp1*.5* gDot(tau) )(t-tau) - x = 0 

    // real tau = zeroin(a,b,pistonBoundaryFunctionPM,tol);

    // It seems easier to find a minimum to g(tau)^2: 

    // Look for a minimum in the interval [a,b]
    real a=0.;
    //b= i1==I1Base ? t-xx/a0 : tau;  // use last solution as upper bound
    real b = t-xx/a0;
    real tau=0.; 
    real ggmin = dfmin(pistonBoundaryFunctionPM,tau,a,b,tol);

    if( pistonOption==specifiedPistonMotion )
    {
      // g(t) = ag*tau^pg
      uTrue = ag*pg*pow(tau,pg-1.);   // here is the velocity
    }
    else if( pistonOption==PistonMotion::pressureDrivenPistonMotion )
    {
      uTrue = GT_FORCED_PISTON(tau);
    }
    else
    {
      real g,gt;
      getPiston( tau,g,gt );
      uTrue=gt;
      // printF("getFlow: t=%9.3e x=%9.3e tau=%9.3e g=%9.3e gt=%9.3e, a0=%9.3e, [a,b]=[%9.3e,%9.3e]\n",t,xx,tau,g,gt,a0,a,b);
      // printF("       : gamma=%9.3e ggmin=%9.3e cc1=%9.3e cc2=%9.3e cc3=%9.3e, cc4=%9.3e\n",gamma,ggmin,cc1,cc2,cc3,cc4);
      // // hh = g + (cc6+cc7*gt)*(cc8-(tau))+cc9;
      // printF(" hh =%9.3e\n",g + (cc6+cc7*gt)*(cc8-(tau))+cc9);
      // getPiston( t,g,gt );
      // printF("       : g(t)=%9.3e,  gt(1)=%9.3e\n",g,gt);


    }
	
  }
  real aTrue = a0+.5*gm1*uTrue;
  pTrue = p0*pow(aTrue/a0,2.*gamma/gm1);
  rhoTrue = rho0*pow(aTrue/a0,2./gm1 );
  
}

// ======================================================================================
/// \brief Return the piston position.
// ======================================================================================
real PistonMotion::
getPosition( const real t ) const
{
  real g=0.;
  if( pistonOption==specifiedPistonMotion )
  {
    // g(t) = ag*tau^pg
    g  = ag*pow(t,pg);
  }
  else if( pistonOption==PistonMotion::pressureDrivenPistonMotion )
  {
    g  = G_FORCED_PISTON(t);
  }
  else
  {
    if( t>tFinal )
    {
      printF("PistonMotion::getPosition:WARNING: t>tFinal - solution will be extrapolated.\n");
    }
    
    if( nurbs==NULL )
    {
      printF("PistonMotion::getPosition:ERROR: the solution has not been computed yet!\n");
      OV_ABORT("ERROR");
    }

    RealArray ra(1), ga(1,2);
    ra(0)=t/tFinal; // scale to [0,1]
    nurbs->mapS( ra,ga );

    g=ga(0,0);
  }

  return g;
}


// ======================================================================================
/// \brief Return the piston velocity.
// ======================================================================================
real PistonMotion:: 
getVelocity( const real t ) const
{
  real gt=0.;
  if( pistonOption==specifiedPistonMotion )
  {
    // g(t) = ag*tau^pg
    gt = ag*pg*pow(t,pg-1.);   
  }
  else if( pistonOption==PistonMotion::pressureDrivenPistonMotion )
  {
    gt = GT_FORCED_PISTON(t);
  }
  else
  {
    if( t>tFinal )
    {
      printF("PistonMotion::getVelocity:WARNING: t>tFinal - solution will be extrapolated.\n");
    }
    if( nurbs==NULL )
    {
      printF("PistonMotion::getVelocity:ERROR: the solution has not been computed yet!\n");
      OV_ABORT("ERROR");
    }

    RealArray ra(1), ga(1,2);
    ra(0)=t/tFinal; // scale to [0,1]
    nurbs->mapS( ra,ga );

    gt=ga(0,1);
  }

  return gt;
}


// ======================================================================================
/// \brief Get the piston position and velocity.
// ======================================================================================
int PistonMotion::
getPiston( const real t, real & g, real & gt ) const
{

  if( pistonOption==specifiedPistonMotion )
  {
    // g(t) = ag*tau^pg
    g  = ag*pow(t,pg);
    gt = ag*pg*pow(t,pg-1.);   
    
  }
  else if( pistonOption==PistonMotion::pressureDrivenPistonMotion )
  {
    g  = G_FORCED_PISTON(t);
    gt = GT_FORCED_PISTON(t);
  }
  else
  {
    if( t>tFinal )
    {
      printF("PistonMotion::getPiston:WARNING: t>tFinal - solution will be extrapolated, t=%8.2e, tFinal=%8.2e.\n",
	     t,tFinal);
    }
    if( nurbs==NULL )
    {
      printF("PistonMotion::getPiston:ERROR: the solution has not been computed yet!\n");
      OV_ABORT("ERROR");
    }

    RealArray ra(1), ga(1,2);
    ra(0)=t/tFinal; // scale to [0,1]
    nurbs->mapS( ra,ga );

    g=ga(0,0);
    gt=ga(0,1);
  }
  
  return 0;
}



// ======================================================================================
/// \brief Solve the ODEs for the piston position and velocity and save as a Nurbs.
// ======================================================================================
int PistonMotion::
computePistonMotion()
{
  if( pistonOption!=PistonMotion::pressureAndBodyForcedPistonMotion )
  {
    printF("PistonMotion::computePistonMotion: INFO: computePistonMotion only makes sense for pressureAndBodyForcedPistonMotion,\n"
           " when the solution is determined by solving an ODE\n");
    return 0;
  }


  real dt;
  if( dt0 > 0. )
  {
    dt=dt0;
  }
  else 
  {
    //  Guess dt: err = K T dt^2
    dt = .5*cfl*pow( (rtol/tFinal), (1./orderOfAccuracy) ); 
  }
    
  //  number of steps:
  int numSteps  = int( tFinal/dt + 1.5 );
  if( dt0 <0. )
  {
    dt = tFinal/(numSteps-1); // alter dt so we exact reach tFinal
  }

  printF("Piston::computePistonMotion: orderOfAccuracy=%d, n=%d, dt=%8.2e\n",orderOfAccuracy,numSteps,dt);

  //  allocate arrays to hold the solution
  Range R=2;
  RealArray ta(numSteps), ya(Range(numSteps),R);
  
  RealArray yv(R), yvn(R);
  yv=0.; // initial conditions  *FIX ME*

  real t=0.; 

  // save the initial solution:
  ta(0)=t;  
  for( int i=R.getBase(); i<=R.getBound(); i++ )
    ya(0,i)=yv(i); 

  for( int step=1; step<numSteps; step ++ )
  {

    timeStep( yvn, yv, t, dt );

    t = t+dt;
    yv=yvn;

    // save the solution:
    assert( step<=ta.getBound(0) );
    ta(step)=t;  
    for( int i=R.getBase(); i<=R.getBound(); i++ )
      ya(step,i)=yv(i); 

  }
  printF("...done: t=%9.3e, t-tFinal=%8.2e, numSteps=%i.\n",t,t-tFinal,numSteps);

  // --- Construct a Nurbs to represent the solution (g,g') as a function of time
  if( nurbs==NULL )
  {
    nurbs = new NurbsMapping(1,2);
  }
  
  int option=0;
  int degree=3;
  ta *= 1./tFinal;  // scale time to the unit square

  nurbs->interpolate( ya,option,ta,degree );


  return 0;
}


// ======================================================================================
/// \brief Solve the ODEs for the piston position and velocity 
// ======================================================================================
int PistonMotion::
timeStep( RealArray & yvn, RealArray & yv, real t, real dt )
{
  
  real t0=t;

  Range R=2;
  RealArray yv0(R), kv1(R);
  
  if( orderOfAccuracy==1 )
  {
    // ---- DIRK order 1 : Back Euler: -----
    //     y' = f(y,t)
    //     k1 = f(y(n)+dt*k1,t+dt) : implicit equation for k1
    //     y(n+1) = y(n) + dt*k1
    //
    // To solve k1 = f(y(n)+dt*k1,t+dt), let y=y(n)+dt*k1, then
    //      y + dt*f(y,t+dt) = y(n) , k1=(y-yn)/dt 
    //
    //
    real aii=1., ci=1., b1=1.; // Backward Euler
    yv0=yv;  // yv0=yn + dt*sum_j=0^{i-1} k_j
    dirkImplicitSolve( dt,aii,t0+ci*dt,yv,yv0, kv1);
    
    yvn = yv + dt*b1*( kv1 );

  }
  else if( orderOfAccuracy==2 )
  {
    //  ---- DIRK order 2 : Implicit mid-point --

    real aii=.5, ci=.5, b1=1.; // Implicit mid-point

    yv0=yv;  //  yv0=yn + dt*sum_j=0^{i-1} k_j
    dirkImplicitSolve( dt,aii,t0+ci*dt,yv,yv0, kv1);  // yv=initial guess

    yvn = yv + dt*b1*( kv1 );

  }
  else if( orderOfAccuracy==3 )
  {
    // ---- DIRK two-stage order 3, A-stable 
    //   Formula from Crouzeix, cf. Alexander 1977 SIAM J. Anal. 14, no 6. 

    RealArray kv2(R);
    
    real sqi3 = 1./sqrt(3.);

    real a11=.5*(1.+sqi3),                    c1=.5*(1.+sqi3), b1=.5; 
    real a21=-sqi3,        a22=.5*(1.+sqi3),  c2=.5*(1.-sqi3), b2=.5; 

    yv0=yv;  // yv0=yn + dt*sum_j=0^{i-1} k_j
    dirkImplicitSolve( dt,a11,t0+c1*dt,yv,yv0, kv1);  // yv=initial guess

    yv0=yv + dt*( a21*kv1 );
    dirkImplicitSolve( dt,a22,t0+c2*dt,yv,yv0, kv2);  // yv=initial guess  *FIX ME*

    yvn = yv + dt*( b1*kv1 + b2*kv2 );

  }
  else if( orderOfAccuracy==4 )
  {  

    // ---- DIRK 4-stage order 4, A0-stable 
    //   Formula from Jackson and Norsett (1990)   -- See: for A and L stable methods? 
    // Iserles, A. and Nørsett, S. P. (1990). On the Theory of Parallel Runge-Kutta Methods. IMA
    //   Journal of Numerical Analysis 10: 463-488.
 
    RealArray kv2(R), kv3(R), kv4(R);

    real a11=1.,                                        c1=1.,     b1=11./72.; 
    real a21=0.,       a22=3./5.,                       c2=3./5.,  b2=25./72.;
    real a31=171./44., a32=-215/44., a33=1.,            c3=0.,     b3=11./72.;
    real a41=-43./20., a42=39./20.,  a43=0., a44=3./5., c4=2./5.,  b4=25./72.;

    yv0=yv;  // yv0=yn + dt*sum_j=0^{i-1} k_j
    dirkImplicitSolve( dt,a11,t0+c1*dt,yv,yv0, kv1);  // yv=initial guess

    yv0=yv + dt*( a21*kv1 );
    dirkImplicitSolve( dt,a22,t0+c2*dt,yv,yv0, kv2);  // yv=initial guess  *FIX ME*

    yv0=yv + dt*( a31*kv1 + a32*kv2 );
    dirkImplicitSolve( dt,a33,t0+c3*dt,yv,yv0, kv3);  // yv=initial guess  *FIX ME*

    yv0=yv + dt*( a41*kv1 + a42*kv2 + a43*kv3 );
    dirkImplicitSolve( dt,a44,t0+c4*dt,yv,yv0, kv4);  // yv=initial guess  *FIX ME*

    yvn = yv + dt*( b1*kv1 + b2*kv2 + b3*kv3 + b4*kv4 );


  }
  else
  {
    OV_ABORT("finish me");
  }

  return 0;
}


// =======================================================================================================
/// \brief: Solve the implicit DIRK equation for kv. This is a protected routine.
/// \details This code is based on the matlab code in ??
// 
// --- Solve:
//      mass*(yv-yv0) - aii*dt*f(yv,tc) = 0 
//
//   kv = (yv-yv0)/(aii*dt) 
// Input:
//   aii, tc : diagonal weight and time
//   y = initial guess 
//   y0 
// =======================================================================================================
int PistonMotion::
dirkImplicitSolve( const real dt, const real aii, const real tc, const RealArray & yv, const RealArray &yv0, 
                   RealArray & kv )
{

  // Initial guess:
  real yk = yv(1);  // we only need to solve for y = g'
  real y0 = yv0(1);
  
  const real adt = aii*dt; 
  const real bodyForce = bf[0]+tc*(bf[1]+tc*(bf[2]+tc*(bf[3])));

  const real gPow = 2.*gamma/(gamma-1.);
  const real cp0 = (gamma-1.)/(2.*a0);

  int maxIterations=20;
  real maxCorrection=REAL_MAX;
  int k=0;
  for( k=0; k<maxIterations; k++ )
  {

    // Pressure: P = p0*( 1+(gam-1)/(2*a0) * g' )^(2*gam/(gam-1))
    real p = p0*pow( 1.+ cp0*yk , gPow );
    real rk = mass*(yk-y0) - adt*area*( -( p - bodyForce ) );  // residual

    // Jacobian: df/dy 
    real fp = mass + adt*area*( p0*gPow*cp0*pow( 1.+ cp0*yk , gPow-1. ) );

    // solve for the correction:
    real dy =  rk/fp;
    maxCorrection = abs(dy);
    if( debug & 2 )
    {
      printF("PM:DIRK: k=%d, correction: max(|dy|)=%8.2e (tc=%9.3e)\n",k,maxCorrection,tc); 
    }

    // update the current solution
    yk -= dy;
    if( maxCorrection<newtonTol ) 
      break;
  }
  
  if( maxCorrection>newtonTol )
  {
    printF("PM:dirkImplicitSolve:WARNING: No convergence in Newton after %d iterations,. maxCorrection=%8.2e, tol=%8.2e\n",
	   maxIterations,maxCorrection,newtonTol);
  }
  else
  {
    
    if( debug & 1 )
    {
      printF("PM:DIRK%i: %i Newton iterations required.\n",orderOfAccuracy,k+1);
    }
  }

  // Fill in the results
  kv(0) = yk;            // g' = y
  kv(1) = (yk-y0)/adt;   // y'
   
  return 0;
}

// =========================================================================================================
/// \brief Define the piston motion parameters.
// =========================================================================================================
int PistonMotion::
update( GenericGraphicsInterface & gi )
{


  GUIState dialog;
  dialog.setWindowTitle("PistonMotion");
  dialog.setExitCommand("exit", "exit");


  // Option menu
  aString opCommand2[] = {"specified motion piston",
			  "pressure driven piston",
			  "pressure and body forced piston",
 			  ""};
  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Motion:", opCommand2, opCommand2, (int)pistonOption );


//   aString colourBoundaryCommands[] = { "colour by bc",
// 			               "colour by share",
// 			               "" };
//   // dialog.addRadioBox("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );
//   dialog.addOptionMenu("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );

   aString cmds[] = {"compute solution",
                     "plot",
		     "compute errors",
                     "plot flow",
		     "help",
 		    ""};
   int numberOfPushButtons=3;  // number of entries in cmds
   int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
   dialog.setPushButtons( cmds, cmds, numRows ); 

//   aString tbCommands[] = {"relax correction steps",
//                           "added mass",
//  			  ""};
//   int tbState[10];

//   tbState[0] = relaxCorrectionSteps;
//   tbState[1] = dbase.get<bool>("includeAddedMass");
//   int numColumns=1;
//   dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;

  textLabels[nt] = "mass:";  sPrintF(textStrings[nt],"%g",mass);  
  nt++; 
  textLabels[nt] = "area:";  sPrintF(textStrings[nt],"%g",area);  
  nt++; 
  textLabels[nt] = "rho0:";  sPrintF(textStrings[nt],"%g",rho0); 
  nt++; 

  textLabels[nt] = "p0:";  sPrintF(textStrings[nt],"%g",p0); 
  nt++; 
  textLabels[nt] = "gamma:";  sPrintF(textStrings[nt],"%g",gamma); 
  nt++; 
  textLabels[nt] = "body force:";  sPrintF(textStrings[nt],"%g, %g, %g, %g (bf0,bf1,bf2,bf3)",
                bf[0],bf[1],bf[2],bf[3]); 
  nt++; 

  textLabels[nt] = "tFinal:";  sPrintF(textStrings[nt],"%g",tFinal); 
  nt++; 

  textLabels[nt] = "ag, pg:";  sPrintF(textStrings[nt],"%g, %g (specified motion: g=ag*t^pg)",ag,pg); 
  nt++; 

  textLabels[nt] = "order of accuracy:"; 
  sPrintF(textStrings[nt],"%i",orderOfAccuracy);  nt++; 

  textLabels[nt] = "tolerance:"; 
  sPrintF(textStrings[nt],"%8.2e",rtol);  nt++; 

  textLabels[nt] = "newtonTol:"; 
  sPrintF(textStrings[nt],"%8.2e",newtonTol);  nt++; 

  textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",debug);  
  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(dialog);
  gi.appendToTheDefaultPrompt("PistonMotion>");

  bool recomputeSolution=true; // for pressureAndBodyForcedPistonMotion

  int len=0;
  aString answer,buff;
  for( ;; )
  {

    gi.getAnswer(answer,"");  
 
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="help" )
    {
      printF("\n"
             "The PistonMotion class is used to define the motion of a piston for rigid body\n"
	     "and FSI computations. This class can treat pistons with specified motions and\n"
	     "pistons whose motion is driven by a fluid pressure. In some cases the piston\n"
	     "motion is known analytically and in other cases the motion is computed by solving\n"
	     "some ODEs (and saving the result as a Nurbs that can be subsequently evaluated).\n"
             "\n"
             "Parameters:\n"
             "  ag,pg : parameters for specified piston motion.\n"
             "  mass : mass of the piston.\n"
             "  area : cross-sectional area of the piston (needed to compute the force from the pressure)\n"
             "  rho0,p0,gamma : density, pressure and gas law constant for adjacent gas at t=0.\n"
             "  bf0,bf1,bf2,bf3 : parameters for body force = bf0 + bf1*t + bf2*t^2 + bf3*t^3.\n"
             "  order of accuracy: for the ODE solver (a diagonally implicit Runge-Kutta method).\n"
             "  tolerance: approximate error tolerance for the ODE solver.\n"
             "  newtonTol: error tol for the Newton iteration in the ODE (implicit) solver.\n"
             "\n"
             "specified motion piston:  g(t) = ag*t^pg : parameters: ag,pg  (exact solution is known).\n"
             "\n"
             "pressure driven piston: a piston of mass `mass' is driven by the pressure from an ideal gas at rest,\n"
             "      the gas is defined by the parameters: rho0,p0,gamma. (exact solution is known).\n"
             "\n"
             "pressure and body force piston: a piston  of mass `mass' is driven by an ideal gas at rest "
             "and a body force,\n"
             "      the gas is defined by the parameters: rho0,p0,gamma. The body force is defined by\n"
             "             body force = bf0 + bf1*t + bf2*t^2 + bf3*t^3.\n"
             "      NOTE: this solution must be computed approximately by solving an ODE. Thus you should also \n"
             "      specify tFinal (the largest time over which the solution will be needed).\n"
             "\n");
    }

    else if( answer=="specified motion piston" ||
	     answer=="pressure driven piston" ||
	     answer=="pressure and body forced piston" )
    {
      pistonOption = (answer=="specified motion piston" ? specifiedPistonMotion :
		      answer=="pressure driven piston" ? pressureDrivenPistonMotion : 
		      pressureAndBodyForcedPistonMotion );
      dialog.getOptionMenu("Motion:").setCurrentChoice(answer);
    }

    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){}//
    else if( dialog.getTextValue(answer,"mass:","%g",mass) ){ recomputeSolution=true; }//
    else if( dialog.getTextValue(answer,"area:","%g",area) ){ recomputeSolution=true; }//
    else if( dialog.getTextValue(answer,"tFinal:","%g",tFinal) ){ recomputeSolution=true; }//
    else if( dialog.getTextValue(answer,"rho0:","%g",rho0) ){ recomputeSolution=true; }//
    else if( dialog.getTextValue(answer,"p0:","%g",p0) ){ recomputeSolution=true; }//
    else if( dialog.getTextValue(answer,"gamma:","%g",gamma) ){ recomputeSolution=true; }//
    else if( dialog.getTextValue(answer,"order of accuracy:","%i",orderOfAccuracy) ){ recomputeSolution=true; } //
    else if( dialog.getTextValue(answer,"tolerance:","%e",rtol) ){ recomputeSolution=true; } //
    else if( dialog.getTextValue(answer,"newtonTol:","%e",newtonTol) ){ recomputeSolution=true; } //

    else if( len=answer.matches("body force:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&bf[0],&bf[1],&bf[2],&bf[3]);
      printF("The body force is bf0 + bf1*t + bf2*t^2 + bf3*t^3\n");
      printF("Setting bf0=%8.2e, bf1=%8.2e, bf2=%8.2e, bf3=%8.2e\n",bf[0],bf[1],bf[2],bf[3]);
      printF("NOTE: The body force only applies to the `pressure and body force piston' motion.\n");

      dialog.setTextLabel("body force:",sPrintF(buff,"%g, %g, %g, %g (bf0,bf1,bf2,bf3)",bf[0],bf[1],bf[2],bf[3]));

      recomputeSolution=true;
    }
    else if( len=answer.matches("ag, pg:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&ag,&pg);
      printF("Setting ag=%8.2e, pg=%8.2e for the specified motion: g(t) = ag*t^pg\n",ag,pg);

      dialog.setTextLabel("ag, pg:",sPrintF(buff,"%g, %g (specified motion: g=ag*t^pg)",ag,pg));
    }
    
    else if( answer=="compute solution" )
    {

      computePistonMotion();
      recomputeSolution=false;
    }
    else if( answer=="compute errors" )
    {
      if( pistonOption!=PistonMotion::pressureAndBodyForcedPistonMotion )
      {
	printF("INFO: compute errors only makes sense for pressureAndBodyForcedPistonMotion, when the solution\n"
               " is determined by solving an ODE\n");
	continue;
      }
      

      // When bodyForce==0 the exact solution is known (see mog)
      if( nurbs==NULL )
      {
	printF("You must first compute the solution before the errors can be computed.\n");
	continue;
      }
      if( bf[0]!=0. || bf[1]!=0. || bf[2]!=0. || bf[3]!=0. )
      {
	printF("WARNING: The errors cannot be computed for bodyForce!=0 \n");
	continue;
      }
      
      int numPoints=101;
      RealArray ta(numPoints), xa(numPoints,2);
      real dta=1./(numPoints-1);
      for( int i=0; i<numPoints; i++ )
	ta(i)=i*dta;
      nurbs->mapS( ta,xa );
      ta*=tFinal;  // scale time<

      RealArray xe(numPoints,2);
      const real twoByGamP1=2./(gamma+1.);
      const real ch =area*p0/(mass*a0);  // coeff for tHat def'n
      real cp0 = (2./(gamma-1.))*(mass*a0*a0/(area*p0));
      
      
      real gErrMax=0., gDotErrMax=0.;
      for( int i=0; i<numPoints; i++ )
      {
	real t= ta(i);
	real tHat = t*ch;
        real g = cp0*( - 1. -tHat + pow(1.+.5*(gamma+1.)*tHat, twoByGamP1) ) ;
        real gDot =  cp0*( - ch + ch*pow(1.+(gamma+1.)*.5*tHat, twoByGamP1-1.)  );

        printF(" i=%i t=%9.3e (g,gDot)=(%9.3e,%9.3e) (ge,gDote)=(%9.3e,%9.3e) err=(%8.2e,%8.2e)\n",i,t,
	       xa(i,0),xa(i,1), g,gDot,fabs(g-xa(i,0)),fabs(gDot-xa(i,1)));
	
	gErrMax=max(gErrMax,fabs(g-xa(i,0)));
	gDotErrMax=max(gDotErrMax,fabs(gDot-xa(i,1)));
      }
      printF(" Max. err : g=%8.2e, gDot=%8.2e\n",gErrMax, gDotErrMax);

    }
    else if( answer=="plot" )
    {
      if( pistonOption==PistonMotion::pressureAndBodyForcedPistonMotion && nurbs==NULL )
      {
	printF("You must first compute the pressureAndBodyForcedPistonMotion solution before it can be plotted.\n");
	continue;
      }
      
      GraphicsParameters parameters;
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,"PistonMotion");
      gi.erase();

      // PlotIt::plot(gi,*nurbs,parameters); 

      int numPoints=101;
      realArray ta(numPoints), xa(numPoints,2);
      real dta=1./(numPoints-1);
      for( int i=0; i<numPoints; i++ )
      {
	ta(i)=i*dta;
        getPiston( ta(i),xa(i,0),xa(i,1) );
      }
      // nurbs->map( ta,xa );
      
      aString title="PistonMotion";
      aString tName="time";
      aString xName[2];
      xName[0]="g";
      xName[1]="gDot";
      
      ta*=tFinal;  // scale time
      PlotIt::plot(gi,ta,xa,title,tName,xName,parameters);

      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      
    }
    else if( answer== "plot flow" )
    {
      if( false )
      {
	real t=0., x=0.;
	for( ;; )
	{
	  gi.getAnswer(answer,"Enter t,x (enter t=-1 to finish)");
	  sScanF(answer,"%e %e",&t,&x);
	  if( t==-1. ) break;
	  real rhoTrue, uTrue, pTrue;
	  getFlow( t, x,rhoTrue,uTrue, pTrue );
	  printF(" t=%9.3e, x=%9.3e, (rho,u,p)=(%9.3e,%9.3e,%9.3e)\n",t, x,rhoTrue,uTrue, pTrue);
	}
      }
      
      GraphicsParameters parameters;
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      // parameters.set(GI_TOP_LABEL,"PistonMotion");
      gi.erase();

      // PlotIt::plot(gi,*nurbs,parameters); 

      real t=tFinal*.5;
      real g,gt;
      getPiston( t,g,gt ); // piston at time t
      
      int numPoints=101;
      realArray xa(numPoints), ua(numPoints,3);
      real length=1.25*(a0*t);
      real dx=length/(numPoints-1);
      for( int i=0; i<numPoints; i++ )
      {
	xa(i)=g+i*dx;
        getFlow( t, xa(i),ua(i,0),ua(i,1),ua(i,2) );
      }
      // nurbs->map( ta,xa );
      
      aString title; sPrintF(title,"Flow Solution t=%8.2e",t);
      aString xName="x";
      aString uName[3];
      uName[0]="rho";
      uName[1]="u";
      uName[2]="p";
      
      PlotIt::plot(gi,xa,ua,title,xName,uName,parameters);

      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);



    }
    else
    {
      printF("PistonMotion::update:ERROR: unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    // global constants for the hydrodynamically forced piston:
    setGlobalConstants();
  }

  // Make sure the piston motion has been computed
  if( pistonOption==pressureAndBodyForcedPistonMotion && recomputeSolution )
  {
    computePistonMotion();
    recomputeSolution=false;
  }


  gi.unAppendTheDefaultPrompt();
  gi.popGUI(); // restore the previous GUI
  


  return 0;
}

