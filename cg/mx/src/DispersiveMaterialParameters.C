#include "DispersiveMaterialParameters.h"

#define evalDispersionRelation EXTERN_C_NAME(evaldispersionrelation)

extern "C"
{
  void evalDispersionRelation( const real& cc, const real& eps, const real& gam, const real& omegap, const real& k, 
                               real& omegar, real& omegai );
}


// ============================================================================
/// \brief Class to define parameters of a dispersive material.
// ============================================================================
DispersiveMaterialParameters::
DispersiveMaterialParameters()
{
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
  gamma=x.gamma;
  omegap=x.omegap;
  
  return *this;
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
  evalDispersionRelation( c, eps, gamma, omegap, k,  omegar, omegai );
  
  printF("--DispersiveMaterialParameters-- dispersion-relation: c=%g eps=%g mu=%g gamma=%g omegap=%g -> omegar=%g, omegai=%g\n",
	 c,eps,mu,gamma,omegap, omegar,omegai);

  return 0;
}
