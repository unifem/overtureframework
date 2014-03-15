//
// Define a branching reaction
//
#include "BranchingReaction.h"
#include "Overture.h"


BranchingReaction::
BranchingReaction()
//====================================================================================
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  rho0=p0=te0=1.;   // scaling values 

  numberOfSpecies_=2;
  speciesName = new aString[numberOfSpecies_];
  speciesName[0]="f";
  speciesName[1]="y";

}


BranchingReaction::
~BranchingReaction()
{
  delete speciesName;
}



void BranchingReaction::
reactionRates( const real & teS, const RealArray & kb_, const RealArray & kf_)
{
//   RealArray & kb = (RealArray&) kb_;
//   RealArray & kf = (RealArray&) kf_;

//   real te=teS*te0;   // scale the temperature

//   kb(0)=1.10e18*pow(te,-1.5)*1.e-12;   // convert from cm^6 to m^6
//   kf(0)= kb(0) * pow(mp.Kp(MP::F,te),2)/(mp.R*te) * mp.newtonMeterSquaredPerAtmosphere; // convert atmos -> N/m^2

//   kb(1)=7.50e18*(1./te)*1.e-12;
//   kf(1)= kb(1) * pow(mp.Kp(MP::H,te),2)/(mp.R*te) * mp.newtonMeterSquaredPerAtmosphere;

//   kb(2)=7.50e18*(1./te)*1.e-12;
//   kf(2)= kb(2) * mp.Kp(MP::H,te)*mp.Kp(MP::F,te)/mp.Kp(MP::HF,te)/(mp.R*te) * mp.newtonMeterSquaredPerAtmosphere;

//   kb(3)=5.28e12*pow(te,.5)*exp(-4000./te)*1.e-12;
//   kf(3)= kb(3) * mp.Kp(MP::H,te)/(mp.Kp(MP::HF,te)*mp.Kp(MP::F,te));

//   kb(4)=5.00e12*exp(-5700./te)*1.e-12;
//   kf(4)= kb(4) * mp.Kp(MP::F,te)/(mp.Kp(MP::HF,te)*mp.Kp(MP::H,te));

//   kb(5)=1.75e10*pow(te,.5)*exp(-19997./te)*1.e-12;
//   kf(5)= kb(5) / pow(mp.Kp(MP::HF,te),2.);
}


void BranchingReaction::
chemicalSource(const RealArray & rhoS, const RealArray & teS, const RealArray & y, 
	       const RealArray & sigma, const RealArray & sy )
// ===========================================================================================
//  /Description: Compute the chemical source term, sigma, and/or the jacobian of sigma/rho
//
// /rhoS (input): density (scaled by rho0)
// /teS (input): temperature (scaled)
// /y (input): mass fractions
// /sigma (output): source term (if sigma!=nullArray)
// /sy (output): d(sigma/rho)/dy (if sy!=nullArray)
// ===========================================================================================
// ======================================================================================================
//  Evaluate the chemical source terms at an array of points
// ======================================================================================================
//====================================================================================
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  Reactions::chemicalSource(rhoS,teS,y,sigma,sy);
}


void BranchingReaction::
chemicalSource( const real & rhoS, const real & teS, const RealArray & y, 
               const RealArray & sigma_, const RealArray & sy_ )
// ===========================================================================================
//  /Description: Compute the chemical source term, sigma, and/or the jacobian of sigma/rho
//
// /rhoS (input): density (scaled by rho0)
// /teS (input): temperature (scaled)
// /y (input): mass fractions
// /sigma (output): source term (if sigma!=nullArray)
// /sy (output): d(sigma/rho)/dy (if sy!=nullArray)
// ===========================================================================================
//====================================================================================
//\end{ReactionsInclude.tex}  
//====================================================================================
{

}

#undef MP



//\begin{>>ReactionsInclude.tex}{\subsection{sigmaFromPTY}}
RealArray & BranchingReaction::
sigmaFromRPTY(const RealArray & rhoS,  
              const RealArray & pS,  
	      const RealArray & teS, 
	      const RealArray & y, 
	      const RealArray & sigmai )
//====================================================================================
// /Description:
//\end{ReactionsInclude.tex}  
//====================================================================================
{
 chemicalSource(rhoS,teS,y,sigmai);
 return (RealArray&)sigmai;

}
