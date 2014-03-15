#include "Overture.h"
#include "Reactions.h"

class Chemkin : public Reactions
{
 public:
  Chemkin();
  Chemkin(const aString & chemistryFileName, const aString & transportFileName =nullString );
  ~Chemkin();
  
  // ------ these functions are derived from the base class -----------------------------
  virtual void reactionRates( const real & teS, 
			      const RealArray & kb, 
			      const RealArray & kf );
  virtual void chemicalSource(const real & rhoS, 
			      const real & teS, 
			      const RealArray & y, 
                              const RealArray & sigma, 
                              const RealArray & sy=Overture::nullRealArray() );
  virtual void chemicalSource(const RealArray & rhoS, const RealArray & teS, 
			      const RealArray & y, 
                              const RealArray & sigma, 
			      const RealArray & sy=Overture::nullRealArray() );

  // molecular weight of a species (kg/mole)
  virtual real mw(const int & species ) const;
  // absolute enthalpy of a species
  virtual real h(const int species, const real te );
  virtual RealArray & h(const int species, 
			const RealArray & teS, 
			const RealArray & h0 );

  // cp of a species
  virtual real cp(const int species, const real te );
  virtual RealArray & cp(const int species, 
			 const RealArray & teS,
			 const RealArray & cp0 );
  virtual RealArray & entropy(const RealArray & teS, 
			      const RealArray & pS, 
			      const RealArray & y,
			      const RealArray & entropy );

  // new forms:
  virtual RealArray & h(const RealArray & teS,
			const RealArray & hi );
  virtual RealArray & cp(const RealArray & teS, 
			 const RealArray & cpi );
  virtual RealArray & sigmaFromScaledPTY( const RealArray & pS,  
					  const RealArray & teS, 
					  const RealArray & y, 
					  const RealArray & sigmai );
  virtual RealArray & sigmaFromPTY( const RealArray & pS,  
				    const RealArray & teS, 
				    const RealArray & y, 
				    const RealArray & sigmai );

  virtual RealArray & sigmaFromRPTY(const RealArray & rhoS, 
				    const RealArray & pS,  
				    const RealArray & teS,
				    const RealArray & y, 
				    const RealArray & sigmai );
//   virtual void chemistrySource( const RealArray & p, 
// 				const RealArray & te, 
// 				const RealArray & y, 
// 				const RealArray & rho, 
// 				const RealArray & source );
  
  
  // mixture viscosity:
  virtual RealArray & viscosity(const RealArray & teS, 
				const RealArray & y, 
				const RealArray & eta );
  // mixture thermal conductivity
  virtual RealArray & thermalConductivity(const RealArray & teS, 
					  const RealArray & y, 
					  const RealArray & lambda );
  // mixture diffusion coefficients
  virtual RealArray & diffusion(const RealArray & pS, 
				const RealArray & teS, 
				const RealArray & y, 
				const RealArray & lambda );

  // ------------------------------------------------------------------------------------------

  int initialize(const aString & chemistryFileName,
                 const aString & transportFileName=nullString);  // supply a Chemkin binary file to read

  int rhoY( const RealArray & pS,
	    const RealArray & teS, 
	    const RealArray & yS, 
	    const RealArray & rhoS );
  int cpBS( const RealArray & teS, 
	    const RealArray & y, 
	    const RealArray & cp );
  int wYP( const RealArray & pS, 
	   const RealArray & teS,
	   const RealArray & y, 
	   const RealArray & wDot );
  

 protected:
  intArray iwork, imcwrk;   // work arrays for Chemkin and the transport routines
  RealArray rwork, rmcwrk;

  char *cwork;
  int numberOfCoefficientsInFit;

};


