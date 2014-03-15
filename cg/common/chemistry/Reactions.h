#ifndef REACTIONS
#define REACTIONS
//
// Define a base class for general Reactions.
//

#include "MaterialProperties.h"
#include "Overture.h"
#include "display.h"

class Reactions
{
 public:

  enum Mechanism
  {
    nonEquilibrium,
    equilibrium,
    frozen
  } reactionMechanism;

  Reactions();
  virtual ~Reactions();

  void useScaledVariables(bool trueOrFalse=TRUE );
  bool usingScaledVariables() const {return variablesScaled;} 

  // ---------These next functions must be defined by any derived class   --------------------
  virtual void reactionRates( const real & teS, const RealArray & kb, const RealArray & kf )=0;
  virtual void chemicalSource(const real & rhoS, const real & teS, const RealArray & y, 
                              const RealArray & sigma, const RealArray & sy=Overture::nullRealArray() )=0;
  // this next one must be defined to avoid hiding the base class, it can just call the base class
  // if there is no concern for efficiency
  virtual void chemicalSource(const RealArray & rhoS, const RealArray & teS, const RealArray & y, 
                              const RealArray & sigma, const RealArray & sy=Overture::nullRealArray() );
  // ------------------------------------------------------------------------------------------

  // ++++++ these next functions must be defined by any class not using the MaterialProperties Class +++++++++
  // molecular weight of a species (kg/mole)
  virtual real mw(const int & species ) const;
  // absolute enthalpy of a species
  virtual real h(const int species, const real teS );
  virtual RealArray & h(const int species, const RealArray & teS, const RealArray & h0 );
  // cp of a species
  virtual real cp(const int species, const real teS );
  virtual RealArray & cp(const int species, const RealArray & teS, const RealArray & cp0 );
  virtual RealArray & entropy(const RealArray & teS, const RealArray & pS, const RealArray & y, 
                              const RealArray & entropy );

  // new forms:
  virtual RealArray & h(const RealArray & teS, const RealArray & hi );
  virtual RealArray & cp(const RealArray & teS, const RealArray & cpi );
  virtual RealArray & sigmaFromPTY( const RealArray & pS,  
				    const RealArray & teS, 
				    const RealArray & y, 
				    const RealArray & sigmai );
  virtual RealArray & sigmaFromRPTY(const RealArray & rhoS, 
				    const RealArray & pS,  
				    const RealArray & teS,
				    const RealArray & y, 
				    const RealArray & sigmai );
  virtual void chemistrySource( const RealArray & pS, const RealArray & teS, const RealArray & y, 
				const RealArray & rhoS, const RealArray & source );

  RealArray & massFractionToMoleFraction( const RealArray & y, const RealArray & x );

  // mixture viscosity: (note: pass mole-fractions)
  virtual RealArray & viscosity(const RealArray & teS, const RealArray & x, const RealArray & eta );
  // mixture thermal conductivity (note: pass mole-fractions)
  virtual RealArray & thermalConductivity(const RealArray & teS, const RealArray & x, const RealArray & lambda );
  // mixture diffusion coefficients (note: pass mole-fractions)
  virtual RealArray & diffusion(const RealArray & pS, const RealArray & teS, const RealArray & x, 
                                const RealArray & lambda );

  // evaluate p from p=rho*R*T
  int pFromRTY( const RealArray & rho,const RealArray & teS,const RealArray & y, RealArray & pS ) const;
  // evaluate rho from rho=p/(R*T)
  int rFromPTY( const RealArray & pS, const RealArray & teS, const RealArray & y, RealArray & rho ) const;
  
  
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  int numberOfElements() const { return numberOfElements_; }
  int numberOfSpecies() const { return numberOfSpecies_; }
  int numberOfReactions() const { return numberOfReactions_; }
  aString getName( const int & species ) const;  // return the name of a a species

  // set scaling factors
  int setScaleForRho( real rho0 );
  int setScaleForT( real te0 );
  int setScaleForP( real p0, real pStatic );
  int setScaleForL( real l0 );
  int setScaleForU( real u0 );
  
  virtual void setScales(real rho0=1., 
			 real te0=1., 
			 real pStatic=1.,  
			 real l0=1.,  
			 real u0=1.,
                         real p0=-1. );

  real getScaleForRho() const;
  real getScaleForT() const;
  real getScaleForP() const;
  real getScaleForStaticP() const;
  real getScaleForL() const;
  real getScaleForU() const;
  real getScaleForR() const;
  void getScales(real & rho0, 
		 real & te0, 
		 real & pStatic,  
		 real & l0,  
		 real & u0,
		 real & p0 ) const;

  void setPressureLevel(const real & pressureLevel); // scaled pressure is (pScaled + pressureLevel)*p0 = p

  void setPressureIsConstant( const bool & trueOrFalse=TRUE );   // for constant pressure reactions
  bool getPressureIsConstant();
  
  // virtual int solveImplicitForRhoTemperatureAndSpecies();

  // Here are some utility functions applicable to all classes derived from this one.
  virtual void chemistrySourceAndJacobian( const RealArray & pS, 
					   const RealArray & teS, 
					   const RealArray & y, 
					   const RealArray & rhoS, 
					   const RealArray & source,
					   const RealArray & sy );

  // Solve backward Euler approximations to the Temperature and species equations, given the pressure
  virtual int solveImplicitForRTYGivenP(RealArray & u, 
					const RealArray & rhs,
					const int & rc, const int & pc, const int & tc, const int & sc,
					const Index & I1, 
					const Index & I2, 
					const Index & I3,
					const real & dt,
					bool equilibrium = FALSE );

  // Solve backward Euler approximations to the species equations, given rho, T and p
  virtual int solveImplicitForYGivenRTP(RealArray & u, 
					const RealArray & rhs,
					const int & rc, const int & pc, const int & tc, const int & sc,
					const Index & I1, 
					const Index & I2, 
					const Index & I3,
					const real & dt,
					bool equilibrium = FALSE  );

  // Solve backward Euler approximations to the T, species equations, given rho
  virtual int solveImplicitForPTYGivenR(RealArray & u, 
					const RealArray & rhs,
					const int & rc, const int & pc, const int & tc, const int & sc,
					const Index & I1, 
					const Index & I2, 
					const Index & I3,
					const real & dt,
					bool equilibrium = FALSE  );
  
  virtual void computeEigenvaluesOfTheChemicalSourceJacobian(const real rhoS, 
						     const RealArray & y, 
						     const RealArray & sigma, 
						     const RealArray & sy,
						     const RealArray & reLambda, 
						     const RealArray & imLambda  );

  // evaluate at an array of values
  void computeEigenvaluesOfTheChemicalSourceJacobian(const RealArray & rhoS, 
                                                     const RealArray & teS, 
						     const RealArray & y, 
						     const RealArray & reLambda, 
						     const RealArray & imLambda  );
  void checkChemicalSourceJacobian();
  
  MaterialProperties mp;

  static int debug;
 protected:
  int numberOfSpecies_, numberOfElements_, numberOfReactions_ ;
  RealArray molecularWeight;

  real rho0,pStatic,p0,te0,l0,u0,R0;           // for scaling
  real rho0Saved,pStaticSaved,p0Saved,te0Saved,l0Saved,u0Saved,R0Saved; // for saving a scaling

  aString *speciesName;  
  MaterialProperties::material *speciesNumber;
  bool pressureIsConstant;
  real pressureLevel;

  bool variablesScaled;
  
  
};


#endif // REACTIONS
