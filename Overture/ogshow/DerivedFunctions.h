#ifndef DERIVED_FUNCTIONS_H
#define DERIVED_FUNCTIONS_H

#include "ShowFileReader.h"
#include "GenericGraphicsInterface.h"

// Derive new functions from old ones. This class is used by plotStuff to
// create derived quantities such as vorticity, mach-number, derivatives etc.

class DerivedFunctions
{
 public:

  enum DerivedItems
  {
    vorticity=0,
    xVorticity,
    yVorticity,
    zVorticity,
    enstrophy,    // || vorticity ||
    divergence,
    machNumber,
    pressure,
    temperature,
    speed,
    entropy,
    schlieren,
    minimumScale,
    r1MinimumScale,
    r2MinimumScale,
    r3MinimumScale,
    r1Velocity,
    r2Velocity,
    r3Velocity,
    xDerivative,
    yDerivative,
    zDerivative,
    xxDerivative,
    yyDerivative,
    zzDerivative,
    laplaceDerivative,
    gradientNorm,
    cellVolume,
    minimumEdgeLength,
    logarithm,
    userDefined,
    //    -- E&M variables
    energyDensity, // E*E + H*H 
    eFieldNorm, // || E || 
    hFieldNorm, //  || H ||
    // Solid mechanics
    displacementNorm,
    stressNorm,
    numberOfDerivedItems
  };


  DerivedFunctions(ShowFileReader & showFileReader );
  DerivedFunctions();
  ~DerivedFunctions();
  

  int add( int derivative, const aString & name_, int n1=0, int n2=0 );

  int getASolution(int & solutionNumber,
		   MappedGrid & cg,
		   realMappedGridFunction & u);

  int getASolution(int & solutionNumber,
		   CompositeGrid & cg,
		   realCompositeGridFunction & u);

  int getDisplacementComponents( int &u1c, int & u2c, int & u3c );

  int getVelocityComponents( int &v1c, int & v2c, int & v3c );

  int getStressComponents( int & s11c, int & s12c, int & s13c,
	  		   int & s21c, int & s22c, int & s23c,
			   int & s31c, int & s32c, int & s33c );


  int numberOfDerivedTypes() const { return numberOfDerivedFunctions; }
  
  int remove( int i );

  void set( ShowFileReader & showFileReader, GraphicsParameters *pgp=NULL  );

  // A user can define new derived functions using these functions 
  int setupUserDefinedDerivedFunction(GenericGraphicsInterface & gi, 
	  			      int numberOfComponents, 
				      aString *componentNames );

  int getUserDefinedDerivedFunction( int index,
                                     int indexOut, 
				     const aString & name, 
                                     const int numberOfComponents,
				     realCompositeGridFunction & uIn,
				     realCompositeGridFunction & uOut,
				     bool & interpolationRequired );

  // update current list of derived grid functions
  int update( GenericGraphicsInterface & gi, 
	      int numberOfComponents, 
	      aString *componentNames,
              GraphicsParameters *pgp=NULL );


 protected:

  int computeDerivedFunctions( realCompositeGridFunction & u );
  int getComponent( int & c, const aString & cName );
  void initialize();

  ShowFileReader *showFileReader;
  int numberOfDerivedFunctions;
  IntegerArray derived;
  aString *name;
  
  // Schlieren parameters
  real exposure, amplification;

  
  int velocityComponent[3];      // Array holding the velocity components
  int displacementComponent[3];  // displacement components
  int stressComponent[9];        // stress components

};

#endif
