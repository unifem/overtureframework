#include "Parameters.h"

#include "turbulenceParameters.h"

extern "C" 
{

// ==============================================================================================
// Return the parameters in the Spalart-Allmaras TM
//   This function is also callable from fortran 
//
// Non-standard variables:
//
// cd0 (output): additional paramater to add to the distance from the wall. This is normally a small
//     parameter except for  twilightZone flow.
//
// cr0 (output): The largest value for "r" 
//                 r = min( U(nc)/( s*dSq*(kappa*kappa) ), cr0 );
//
// Call with:
//
// real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0;
// getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0)
//
// ==============================================================================================
void 
getSpalartAllmarasParameters( real & cb1, real & cb2, real & cv1, real & sigma, real & sigmai,
			      real & kappa, real & cw1, real & cw2, real & cw3,
			      real & cw3e6, real & cv1e3, real & cd0, real & cr0 )
{
   cb1=.1355, cb2=.622, cv1=7.1, sigma=2./3., sigmai=1./sigma, kappa=.41;

   // artificially scale cb1 to turn on the turbulence model at lower Reynolds number (for testing)
   cb1*=Parameters::spalartAllmarasScaleFactor;  // note: cw1 depends on cb1

   cw1=cb1/(kappa*kappa)+(1.+cb2)/sigma, cw2=.3, cw3=2., cw3e6=pow(cw3,6.), cv1e3=pow(cv1,3.);

   cd0=Parameters::spalartAllmarasDistanceScale; // 1.e-10;
   
   cr0 = 1.e3;  // largest value for r, for r sufficiently large fw -> 1 
}


// ==============================================================================================
// Return the parameters in the k-epsilon TM
//   This function is also callable from fortran 
//
// Call with:
//
// real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI;
// getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
//
// ==============================================================================================
void 
getKEpsilonParameters( real & cMu, real & cEps1, real & cEps2, real & sigmaEpsI, real & sigmaKI )
{
  cMu=.09;
  cEps1=1.44;
  cEps2=1.92;
  sigmaEpsI=1./1.3;  // NOTE: inverse of sigmaEps
  sigmaKI=1./1.;     // NOTE: inverse of sigmaK

  // for testing
  cMu=1.; 
  cEps1=1.5;
  cEps2=2.;
  sigmaEpsI=1./1.3;
  sigmaKI=1./1.;

  // cMu=0.; 
  // cEps1=0.;
  // cEps2=0.;
  // sigmaEpsI=0.;
  // sigmaKI=0.;

}


}

