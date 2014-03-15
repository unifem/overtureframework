//
//  NULL version if Chemkin is not available
//

#include "Chemkin.h"

Chemkin::
Chemkin() : Reactions()
{
  cwork=NULL;
}

Chemkin::
Chemkin(const aString & chemistryFileName, 
        const aString & transportFileName /* = nullString */ ) : Reactions()
// ============================================================================================
//  /fileName (input) : name of a Chemkin generated binary file to read
// ============================================================================================
{
  printf("Chemkin::Sorry, Chemkin is not available. Talk to Bill Henshaw \n");
  if( this )
    Overture::abort("error");
}


Chemkin::
~Chemkin()
{
  delete [] cwork;
}

  
int Chemkin::
initialize(const aString & chemistryFileName, 
           const aString & transportFileName /* = nullString */ )
// ============================================================================================
//  /fileName (input) : name of a Chemkin generated binary file to read
// ============================================================================================
{
  // Initialize CHEMKIN 
  printf("Chemkin::Sorry, Chemkin is not available. Talk to Bill Henshaw \n");
  if( this )
    Overture::abort("error");
  return 0;
}

void Chemkin::
reactionRates( const real & teS, const RealArray & kb, const RealArray & kf )
{
  cout << "Chemkin::reactionRates: ERROR \n";
}

void Chemkin::
chemicalSource(const real & rhoS, 
	       const real & teS, 
	       const RealArray & y, 
	       const RealArray & sigma, 
	       const RealArray & sy /* =Overture::nullRealArray() */ )
// ===========================================================================================
//  /Description: Compute the chemical source term, sigma, and/or the jacobian of sigma/rho
//
// /rhoS (input): density (scaled by rho0)
// /teS (input): temperature (scaled)
// /y (input): mass fractions
// /sigma (output): source term (if sigma!=nullArray)
// /sy (output): d(sigma/rho)/dy (if sy!=nullArray)  Unscaled!
// ===========================================================================================
{
}

void Chemkin::
chemicalSource(const RealArray & rhoS, const RealArray & teS, const RealArray & y, 
	       const RealArray & sigma, const RealArray & sy /* =Overture::nullRealArray() */ )
// ===========================================================================================
//  /Description: Compute the chemical source term, sigma, and/or the jacobian of sigma/rho
//
// /rhoS (input): density (scaled by rho0)
// /teS (input): temperature (scaled)
// /y (input): mass fractions
// /sigma (output): source term (if sigma!=nullArray)
// /sy (output): d(sigma/rho)/dy (if sy!=nullArray)
// ===========================================================================================
{
}



real Chemkin::
mw(const int & species) const
// molecular weight of a species (kg/mole)
{
  return 0.;
}

// absolute enthalpy of a species
real Chemkin::
h(const int species, const real te )
//====================================================================================
// return the ABSOLUTE enthalpy of a species at an array of values
// /h0 (output) : cp in Mass units J/Kg
//====================================================================================
{
  return 0.;
}

RealArray & Chemkin::
h(const int species, const RealArray & teS, const RealArray & h0 )
//====================================================================================
// return the ABSOLUTE enthalpy of a species at an array of values
// /h0 (output) : cp in Mass units J/Kg
//====================================================================================
{
  return Overture::nullRealArray();
}

// cp of a species
real Chemkin::
cp(const int species, const real te )
//====================================================================================
// return the cp of a species
// /cp0 (output) : cp in Mass units J/(Kg-K)
//====================================================================================
{
  return 0.;
}

RealArray & Chemkin::
cp(const int species, const RealArray & teS, const RealArray & cp0 )
//====================================================================================
// return the cp of a species
// /cp0 (output) : cp in Mass units J/(Kg-K)
//====================================================================================
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
entropy(const RealArray & teS, const RealArray & pS, const RealArray & y, const RealArray & entropy )
{
  return Overture::nullRealArray();
}



int Chemkin::
rhoY( const RealArray & p, const RealArray & te, const RealArray & y, const RealArray & rho )
//
//    Returns the mass density of the gas mixture given the pressure, 
//    temperature and mass fractions;  see Eq. (2). 
// 
{
  return 0;
}

int Chemkin::
cpBS( const RealArray & te, const RealArray & y, const RealArray & cp )
//     CPBS  - Mean specific heat at constant pressure in mass units. 
//                   cgs units - ergs/(gm*K) 
//                   Data type - real scalar 
{
  return 0;
}

int Chemkin::
wYP( const RealArray & p, const RealArray & te, const RealArray & y, const RealArray & wDot ) 
//   Returns the molar production rates of the species given the 
//   pressure, temperature and mass fractions;  see Eq. (49). 
{
  return 0;
}


RealArray & Chemkin::
viscosity(const RealArray & teS, const RealArray & x, const RealArray & eta )
// =================================================================================
//     mixture viscosity:
// /teS (input) : temperature, (scaled)
// /x (input) : mole fractions
// =================================================================================
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
thermalConductivity(const RealArray & teS, const RealArray & x, const RealArray & lambda )
// =================================================================================
// mixture thermal conductivity
// /te (input) : temperature, (scaled)
// /x (input) : mole fractions
// =================================================================================
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
diffusion(const RealArray & pS, const RealArray & teS, const RealArray & x, const RealArray & d )
// =================================================================================
// mixture diffusion coefficients
// /p (input) : pressure (scaled)
// /te (input) : temperature, (scaled)
// /x (input) : mole fractions
// =================================================================================
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
h(const RealArray & te, const RealArray & hi )
// /te(I1,I2,I3) (input) : temperature, unscaled
// /hi(I1,I2,I3,S) (output) : MKS, J/Kg
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
cp(const RealArray & te, const RealArray & cpi )
// /te(I1,I2,I3) (input) : temperature, unscaled
// /cpi(I1,I2,I3,S) (output) : MKS, J/(Kg K)
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
sigmaFromPTY( const RealArray & p,  const RealArray & te, const RealArray & y, const RealArray & sigmai )
// /p(I1,I2,I3) (input) : temperature, unscaled
// /te(I1,I2,I3) (input) : temperature, unscaled
// /sigma(I1,I2,I3,S) (output) (MKS) Kg/m^3 
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
sigmaFromScaledPTY( const RealArray & pS,  const RealArray & teS, const RealArray & y, const RealArray & sigmai )
// /pS(I1,I2,I3) (input) : temperature, scaled
// /teS(I1,I2,I3) (input) : temperature, scaled
// /sigma(I1,I2,I3,S) (output) (MKS) Kg/m^3 
{
  return Overture::nullRealArray();
}

RealArray & Chemkin::
sigmaFromRPTY(const RealArray & rhoS,  
              const RealArray & pS,  
	      const RealArray & teS, 
	      const RealArray & y, 
	      const RealArray & sigma )
//====================================================================================
// /Description:
//   Compute $\sigma$ for all species from $\rho,p,T,Y_i)$.
//====================================================================================
{
  return sigmaFromPTY(pS,teS,y,sigma);
}
