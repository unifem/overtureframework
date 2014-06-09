#include "BoundaryLayerProfile.h"
#include "display.h"

#define CSGEN  EXTERN_C_NAME(csgen)
#define CSEVAL EXTERN_C_NAME(cseval)

// These are the old spline routines from FMM
extern "C"
{
  void CSGEN (int & n, real & x, real & y, real & bcd, int & iopt  );
  void CSEVAL(int & n, real & x, real & y, real & bcd, real & u, real & s, real & sp );
}

int BoundaryLayerProfile::debug=0;

// ===================================================================================
/// \brief Constructor 
// ===================================================================================
BoundaryLayerProfile::
BoundaryLayerProfile()
{
  initialized=false;
  nu=1.e-3;
  U=1.;
  nEta=-1;
  etaMin=0.; etaMax=10.; // evaluate Blasius function over this range
  
}

// ===================================================================================
/// \brief Destructor.
// ===================================================================================
BoundaryLayerProfile::
~BoundaryLayerProfile()
{
}
    
// ===================================================================================
/// \brief Assign the parameters in the solution
/// \param nu (input) : kinematic viscosity
/// \param U (input) : free-stream velocity
// ===================================================================================
int BoundaryLayerProfile::
setParameters( real nu_, real U_ )
{
  nu=nu_;
  U=U_;

  return 0;
}


// ===================================================================================
/// \brief Evaluate the boundary layer solution u(x,y) and v(x,y)
/// \Note:  For x<=0 the solution is the free stream
/// \Note: v only makes sense if sqrt(nu*U/x) is small -- thus nu should be small and
///     x order 1
// ===================================================================================
int BoundaryLayerProfile::
eval( const real x, const real y, real & u, real & v )
{

  if( !initialized )
    initializeProfile(); 

  if( x<=0 )
  {
    if( x<0 )
      printF("BoundaryLayerProfile:WARNING: x<0.\n");
    u=U;
    v=0.;
    return 0;
  }
  
  real eta0 = y*sqrt(U/(nu*x));

  if( eta0<etaMax ) // the spline only covers etaMin <= eta <= etaMax
  {
    real f0,fp0;
    evalBlasius( eta0, f0 ,fp0 );
  

    u = U*fp0;
    v = .5*sqrt(nu*U/x)*(eta0*fp0-f0);
  }
  else
  {
    // asymptotic form
    //  eta=10 :  eta*fp-f=1.72078750
    // Blasius: eta=  9.9500 f=  8.22921 fp=  1.00000  ( eta*fp-f=1.72078762)                                                         // Blasius: eta= 10.0000 f=  8.27921 fp=  1.00000  ( eta*fp-f=1.72078763)        
    u=U;
    real Rex = U*x/nu;
    v = .5*1.72078763*U/sqrt(Rex);   
  }
  
  return 0;
}


// ===================================================================================
/// \brief Evaluate the Blasius flat-plate profile f and the derivative f'
/// \param eta0 (input) : similarity variable eta0 = y*sqrt(U/(nu*x))
/// \param f0, fp0 (output) : f and f'
// ===================================================================================
int BoundaryLayerProfile::
evalBlasius( const real eta0, real & f0 , real & fp0 )
{
  if( !initialized )
    initializeProfile(); 

  assert( nEta>0 );

  // --- evaluate the splines --- 
  real fp1,fpp;
  real eta1=eta0;
  CSEVAL(nEta,eta(0), f(0),bcd(0,0,0), eta1,f0,fp1 ); 

  CSEVAL(nEta,eta(0),fp(0),bcd(0,0,1), eta1,fp0,fpp ); 

   // printF("evalBlasius: eta=%8.4f, f=%9.5f, fp=%9.5f (fp1=%9.5f)\n",eta0,f0,fp0,fp1);
  
  return 0;
}

// ===================================================================================
/// \brief Define the "slope" function for the Blasius equation written as a first-order system
//   f''' = -.5*f*f'' . This is used by the RK4 solver.
// ===================================================================================
int BoundaryLayerProfile::
blasiusFunc( real *fv, real *fvp )
{
  fvp[0] = fv[1];    // f' 
  fvp[1] = fv[2];    // f'' 
  fvp[2] = -.5*fv[0]*fv[2];  // f''' 
}


// ===================================================================================
/// \brief This function will initialize the solution.
///
/// \details This function solves the Blasius ODE for f(eta) and f'(eta) and then
///   fits a Spline to the resulting data.
///
// ===================================================================================
int BoundaryLayerProfile::
initializeProfile()
{
  printF("BoundaryLayerProfile::initializeProfile: nu=%9.3e, U=%8.2e\n",nu,U);
  initialized=true;
  

  // Solve Blasius ODE with RK4:

  const real fpp0 = 0.3320573362151946;   // value of f''(0) that leads to f'(infinity)=1


  nEta=201;   // number of eta values 
  real deta=(etaMax-etaMin)/(nEta-1);  // time-step

  eta.redim(nEta); f.redim(nEta); fp.redim(nEta);  // save f and f' here 

  real fv[3], df[3], k1[3], k2[3], k3[3], k4[3]; 

  fv[0]=0.; fv[1]=0.; fv[2]=fpp0; // initial values 
  eta(0)=etaMin;
  f(0)=fv[0];  // save f 
  fp(0)=fv[1]; // save fp 
  for( int i=1; i<nEta; i++ ) // time-step loop 
  {
    // RK4:
    blasiusFunc(fv,k1);

    for( int j=0; j<3; j++ ){ df[j] = fv[j] + .5*deta*k1[j]; }
    blasiusFunc(df,k2);

    for( int j=0; j<3; j++ ){ df[j] = fv[j] + .5*deta*k2[j]; }
    blasiusFunc(df,k3);

    for( int j=0; j<3; j++ ){ df[j] = fv[j] +   deta*k3[j]; }
    blasiusFunc(df,k4);

    for( int j=0; j<3; j++ ){ fv[j] +=  (deta/6.)*( k1[j] + 2.*(k2[j]+k3[j]) + k4[j] );  } // RK4 
       
    eta(i) = etaMin + i*deta;
    f(i)=fv[0];  // save f 
    fp(i)=fv[1]; // save fp 

    if( debug & 1 )
      printF("Blasius: eta=%8.4f f=%9.5f fp=%9.5f  ( eta*fp-f=%10.8f) \n",eta(i),f(i),fp(i),eta(i)*fp(i)-f(i));
  }
  

  // --- fit splines ---
  int option= 0; // 0 = not-periodic
  int numberOfSplinePoints=nEta;
  bcd.redim(3,numberOfSplinePoints,2);
  CSGEN (numberOfSplinePoints, eta(0),  f(0), bcd(0,0,0), option ); // spline for f 
  CSGEN (numberOfSplinePoints, eta(0), fp(0), bcd(0,0,1), option ); // spline for f'


  return 0;
}


