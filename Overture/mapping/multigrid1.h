#include "Overture.h"
// #include "Mapping.h"
#include "MappedGridOperators.h"
//#include "PlotStuff.h"
#include "GenericGraphicsInterface.h"

class GenericGraphicsInterface;
class DataPointMapping;
//class PlotStuff;

class multigrid
{
  // A class to solve pde using the multigrid method

 public:

  int domainDimension, rangeDimension;

  enum BoundaryConditionTypes
  {
    dirichlet=1,
    slipOrthogonal=2,
    noSlipOrthogonalAndSpecifiedSpacing=3,
    noSlipOrthogonal,
    specifiedSpacing
  };

  enum SmoothingTypes
  {
    jacobiSmooth=1,
    redBlackSmooth,
    lineSmooth,
    zebraSmooth
  };
  

  int numberOfLevels;
  int maximumNumberOfLevels;
  int maximumNumberOfIterations;
  
  // directions
  SmoothingTypes smoothingMethod;  // 1: Jacobi   2: Red-Black   3: Line-Relaxation

 public:
  // The constructor
  multigrid();

  multigrid(const multigrid &old_multigrid); // The copy constructor

  ~multigrid();    // The destructor

 public:
  Mapping *map;    // for unit interval
  MappedGrid  *mg;
  MappedGridOperators  *operators;
  realMappedGridFunction *u;

  Range Rx;

  realArray *Source;
  RealMappedGridFunction *rhs, *w;   //The right hand side and linearized solution
  RealArray dx;
  int niter, iter;      //The number of iteration

  real residualTolerance;  // convergence criteria for the maximum residual

  //for the smoother
  real omega;                   // Optimal omega for underelaxed jacobi
  real lambda;                  // Interpolation power
  // when there is a gridBc=3
  real alpha, alphaPrev, omega1;
  int numberOfPeriods;          // For doing many sheets in the periodic case
  int useBlockTridiag;

  intArray boundaryCondition;
  realArray boundarySpacing;            //The specified boundary thickness
  Mapping *userMap;
  RealArray *rBoundary;        // holds unit square cooridnates of boundary values.

  IntegerArray gridIndex;     // holds interior points plus boundary where equations are applied.
  int numberOfLinesOfAttraction;
  IntegerArray lineAttractionDirection;
  RealArray lineAttractionParameters;

  int numberOfPointsOfAttraction;
  RealArray pointAttractionParameters;

  bool useNewStuff;

  int debug;
  
 public:

  int update(DataPointMapping & dpm,
             GenericGraphicsInterface *gi = NULL , 
	     GraphicsParameters & parameters =Overture::defaultGraphicsParameters() );
  

  // this periodic update knows about the derivativePeriodic case
  int periodicUpdate( RealMappedGridFunction & x, 
		      const Range & C=nullRange,
		      const bool & isAGridFunction = TRUE  );

  // get coefficients of the elliptic system
  int  getCoefficients(RealArray & coeff, 
		       const Index & J1, 
		       const Index & J2, 
		       const Index & J3,
		       const RealArray & ur, 
		       const RealArray & us,
                       const RealArray & ut = Overture::nullRealArray() );
  
  int smooth(const int & level, 
	     const SmoothingTypes & smoothingType,
	     const int & numberOfSubIterations =1,
             const int & ichange =1 );

  int jacobi(const int & level, 
	     RealMappedGridFunction & uu,
	     const int & ichange );

  int redBlack(const int & level, 
	       RealMappedGridFunction & uu,
	       const int & ichange );

  int lineSmoother(const int & direction,
                   const int & level,
		   RealMappedGridFunction & u );

  int applyBoundaryConditions(const int & level,
			      RealMappedGridFunction & uu );

  int getControlFunctions(const int & level );

  int defineBoundaryControlFunction();

  realArray SignOf(realArray & uarray);

  void getResidual(realArray &resid1, 
                   const int & level);

  void getResidual(realArray &resid1, 
                   const int & level,
		   Index Jv[3],
		   RealArray & coeff,
		   const bool & computeCoeffients =TRUE,
		   const bool & includeRightHandSide=TRUE );


  void getRHS(realArray &RH, int i );
	 
  int fineToCoarse(const int & level, 
                   const RealMappedGridFunction & uFine, 
                   RealMappedGridFunction & uCoarse,
		   const bool & isAGridFunction = FALSE );
  int coarseToFine(const int & level, 
                   const RealMappedGridFunction & uCoarse, 
                   RealMappedGridFunction & uFine,
		   const bool & isAGridFunction = FALSE );
  
  void Interpolate(int levelFrom, int levelTo, realArray &uFrom, 
		   realArray &uTo, int jmax);

  int multigridVcycle(const int & level );

  // supply a starting grid.
  int startingGrid( const RealArray & u0, const IntegerArray & indexBounds=Overture::nullIntArray() );

  int applyMultigrid();


  int stretchTheGrid(Mapping & mapToStretch);

//  int make2Power(int n);

  void setup(Mapping & map );

  void applyBC(realArray &u1, realArray &v1, int i);

  void updateRHS(int i );

  int plot( const RealMappedGridFunction & v, const aString & label );
  

 protected:

//  PlotStuff *ps;
  GenericGraphicsInterface *ps;
  GraphicsParameters psp;
  
  int initializeParameters();
  real maximumResidual, previousMaximumResidual;

  int numberOfCoefficients;  // number of coefficients int the coeff array
  char buff[80];
  FILE *debugFile;

};
