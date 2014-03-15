#ifndef BOUNDARY_CONDITION_PARAMETERS_H 
#define BOUNDARY_CONDITION_PARAMETERS_H 

#include "OvertureTypes.h"
#include "A++.h"

#ifndef OV_USE_DOUBLE
class floatMappedGridFunction;
#define REAL_MAPPED_GRID_FUNCTION floatMappedGridFunction
class floatGridCollectionFunction;
#define REAL_GRID_COLLECTION_FUNCTION floatGridCollectionFunction
#else
class doubleMappedGridFunction;
#define REAL_MAPPED_GRID_FUNCTION doubleMappedGridFunction
class doubleGridCollectionFunction;
#define REAL_GRID_COLLECTION_FUNCTION doubleGridCollectionFunction
#endif


// ===================================================================================
// This class is used to pass optional parameters to the boundary condition routines
// ===================================================================================
class BoundaryConditionParameters
{
public:

  enum CornerBoundaryConditionEnum
  {
    doNothingCorner=-1,  
    extrapolateCorner=0,
    symmetryCorner,  // should be replaced by the one of the odd,even below -- keep for compatibility
    taylor2ndOrder,  // should be replaced by the taylor2ndOrderOddCorner below -- keep for compatibility
    evenSymmetryCorner,
    oddSymmetryCorner,
    taylor2ndOrderEvenCorner,
    taylor4thOrderEvenCorner,
    vectorSymmetryAxis1Corner,       // even symmetry on all variables except normal component of the "velocity"
    vectorSymmetryAxis2Corner, 
    vectorSymmetryAxis3Corner
  };

  // Here are different ways we can assign the data for boundary conditions
  enum BoundaryConditionForcingOption
  {
    unSpecifiedForcing=-1,
    scalarForcing=0,
    vectorForcing,
    vectorByFaceForcing,
    arrayForcing,
    gridFunctionForcing
  };

  enum ExtrapolationOptionEnum
  {
    polynomialExtrapolation=0,
    extrapolateWithLimiter
  } extrapolationOption;
  real extrapolateWithLimiterParameters[2];


  BoundaryConditionParameters();
  ~BoundaryConditionParameters();
  
  int lineToAssign;          // apply Dirichlet BC on this line
  int orderOfExtrapolation;
  int orderOfInterpolation;
  int ghostLineToAssign;     // assign this ghost line (various bc's)
  int useMixedBoundaryMask;  // normally true 
  int extraInTangentialDirections; // extend the set of pts assigned by this many ps in the tangential directions
  int numberOfCornerGhostLinesToAssign; // assign this many lines at edges and corners, by default do all
  int cornerExtrapolationOption;  // for extrapolating corners along given directions instead of the diagonal


  IntegerArray components;       // hold components for various BC's
  IntegerArray uComponents,fComponents;
  RealArray a,b0,b1,b2,b3;

  int interpolateRefinementBoundaries;  // if true, interpolate all refinement boundaries
  int interpolateHidden;                // if true, interpolate hidden coarse grid points from higher level refinemnts

  int setUseMask(int trueOrFalse=TRUE);
  int getUseMask() const{ return useMask;}
  
  // Boundary conditions on mixed boundaries are normally NOT assigned at interior boundary points,
  //  unless you call the next function with "true"
  int assignAllPointsOnMixedBoundaries( bool trueOrFalse=true );

  intArray & mask();
  
  CornerBoundaryConditionEnum getCornerBoundaryCondition( int side1, int side2, int side3 = -1 ) const;
 
  int setCornerBoundaryCondition( CornerBoundaryConditionEnum bc );
  int setCornerBoundaryCondition( CornerBoundaryConditionEnum bc, int side1, int side2, int side3 = -1 );

  // Indicate which components form the "vector" for the vector symmetry corner BC (e.g. where the velocity
  // components start in the list of components
  int setVectorSymmetryCornerComponent( int component );
  int getVectorSymmetryCornerComponent() const;

  // supply an array for variable coefficients in a BC: (set to NULL to turn off) (*new* way 2011/09/03)
  int setVariableCoefficientsArray( realSerialArray *var=NULL ); 
  realSerialArray *getVariableCoefficientsArray() const; // *new* 2011/09/03

  // olde way: 
  void setVariableCoefficients( REAL_MAPPED_GRID_FUNCTION & var ); // supply a grid function for variable coefficients
  void setVariableCoefficients( REAL_GRID_COLLECTION_FUNCTION & var ); 
  REAL_MAPPED_GRID_FUNCTION *getVariableCoefficients() const;
  REAL_MAPPED_GRID_FUNCTION *getVariableCoefficients(const int & grid) const;

  void setRefinementLevelToSolveFor( int level );
  int getRefinementLevelToSolveFor() const { return refinementLevelToSolveFor;}

  int setBoundaryConditionForcingOption( BoundaryConditionForcingOption option );
  BoundaryConditionForcingOption getBoundaryConditionForcingOption() const;
  

  // This version is used by MappedGridOperators and expects 0 <= sideN <=2
  int getCornerBC(int side1, int side2, int side3) const{return (int)cornerBC[side1][side2][side3];} //

 protected:

  CornerBoundaryConditionEnum cornerBC[3][3][3]; // 0=extrapolate, 1=symmetry
  int vectorSymmetryCornerComponent;

  realSerialArray *variableCoefficientsArray; // new way 2011/09/03

  REAL_MAPPED_GRID_FUNCTION *variableCoefficients;
  REAL_GRID_COLLECTION_FUNCTION *variableCoefficientsGC;
  int useMask;
  intArray *maskPointer;   // for applying BC's selectively to points where mask != 0
  int refinementLevelToSolveFor;  // for refinement level solves.
  BoundaryConditionForcingOption boundaryConditionForcingOption;
  
};

#undef REAL_MAPPED_GRID_FUNCTION
#undef REAL_GRID_COLLECTION_FUNCTION

#endif
