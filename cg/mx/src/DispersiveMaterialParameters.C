#include "DispersiveMaterialParameters.h"

#define evalDispersionRelation EXTERN_C_NAME(evaldispersionrelation)

extern "C"
{
  void evalDispersionRelation( const real& cc, const real& eps, const real& gam, const real& omegap, const real& k, 
                               real& reS, real& imS );
}


// ============================================================================
/// \brief Class to define parameters of a dispersive material.
// ============================================================================
DispersiveMaterialParameters::
DispersiveMaterialParameters()
{

  // general dispersive model parameters:
  //   modelParameters(i,k)  : i=0,1,2,3,4 are the parmeters in the equation 
  //                           for P_k , k=1,2,...,numberOfPolarizationVectors
  // Polarization equation for vector P_k
  //   (P_k)_tt + a_k (P_k)_t + b_k P_k = c_k E + d_k E_t
  // modelParameters(0:3,k) = [a_k,b_k,c_k,d_k] 

  numberOfPolarizationVectors=0; // by default a domain is non-dispersive
  numberOfModelParameters=4;     // [a_k,b_k,c_k,d_k] 
  // modelParameters.redim(numberOfModelParameters,numberOfPolarizationVectors); 

  // **OLD WAY: 
  // Drude-Lorentz model:  
  //     P_tt + gamma P_t = omegap^2 E 
  gamma=1.;  // damping 
  omegap=1.; // plasma frequency

  if( false )
  {
    // **TEST***
    real c=1., eps=1., mu=1., k=1.;
    real omegar,omegai;
  
    computeDispersivePlaneWaveParameters( c,eps,mu,k, omegar, omegai );
  }
  
}

// ============================================================================
/// \brief Copy constructor
// ============================================================================
DispersiveMaterialParameters::
DispersiveMaterialParameters(const DispersiveMaterialParameters& x)
{
  *this=x;  
}

// ============================================================================
/// \brief Destructor.
// ============================================================================
DispersiveMaterialParameters::
~DispersiveMaterialParameters()
{
}


DispersiveMaterialParameters & DispersiveMaterialParameters::
operator =( const DispersiveMaterialParameters & x )
{
  numberOfPolarizationVectors=x.numberOfPolarizationVectors;
  numberOfModelParameters=x.numberOfModelParameters;
  modelParameters.redim(0);
  modelParameters=x.modelParameters;

  gamma=x.gamma;
  omegap=x.omegap;

  return *this;
}


// ==========================================================================================
/// \brief Compute the real and imaginary parts of the disperion relation parameter "s"
///
/// \param c,eps,mu,k (input) 
/// \param  reS,imS (ouptut) : real and imaginary parts of s omega in exp(s*t)*exp(i k*x )
// ==========================================================================================
int DispersiveMaterialParameters::
computeDispersionRelation( const real c, const real eps, const real mu, const real k, 
                           real & reS, real & imS )
{
  evalDispersionRelation( c, eps, gamma, omegap, k,  reS, imS );
  
  printF("--DispersiveMaterialParameters-- dispersion-relation: c=%g eps=%g mu=%g gamma=%g omegap=%g"
         " -> real(s)=%g, Im(s)=%g\n",
	 c,eps,mu,gamma,omegap, reS,imS );

  return 0;
}


// ==========================================================================================
/// \brief Compute the real and imaginary parts of the dispersive plane wave "omega"
///
/// \param c,eps,mu,k (input) 
/// \param  omegar,omegai (ouptut) : real and imaginary parts of omega in exp(i(k*x-omega*t))
// ==========================================================================================
int DispersiveMaterialParameters::
computeDispersivePlaneWaveParameters( const real c, const real eps, const real mu, const real k, 
                                      real & omegar, real & omegai )
{
  real reS, imS;
  evalDispersionRelation( c, eps, gamma, omegap, k,  reS, imS );
  omegar=imS;
  omegai=reS;
  
  printF("--DispersiveMaterialParameters-- dispersion-relation: c=%g eps=%g mu=%g gamma=%g omegap=%g -> omegar=%g, omegai=%g\n",
	 c,eps,mu,gamma,omegap, omegar,omegai);

  return 0;
}
