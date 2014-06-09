#ifndef OGES_H
#define OGES_H "Oges.h"

#include "Overture.h"          

//===========================================================================
//  Overlapping Grid Equation Solver
//
//===========================================================================

#include "OGFunction.h"
#include "OgesParameters.h"
#include "MultigridCompositeGrid.h"

#include "ListOfIntSerialArray.h"
#include "ListOfFloatSerialArray.h"
#include "ListOfDoubleSerialArray.h"

#ifdef OV_USE_DOUBLE
  typedef ListOfDoubleSerialArray ListOfRealSerialArray;
#else
  typedef ListOfFloatSerialArray ListOfRealSerialArray;
#endif

// This macro will initialize the PETSc solver if OVERTURE_USE_PETSC is defined.
#ifdef OVERTURE_USE_PETSC
  #define INIT_PETSC_SOLVER() initPETSc();
#else
  #define INIT_PETSC_SOLVER() 
#endif



#ifdef OVERTURE_USE_PETSC
  void initPETSc();  // function to initialize the PETSc solver
#endif

class GenericGraphicsInterface;
class EquationSolver;

class Oges 
{
  public:


  enum SparseStorageFormatEnum
  {
    uncompressed,
    compressedRow,
    other
  } sparseStorageFormat;


  Oges();	
  Oges( CompositeGrid & cg );
  Oges( MappedGrid & mg );
  Oges(const Oges & X);
  Oges & operator=( const Oges & x );
  virtual ~Oges();	
   
  // define a predefined equation
  int setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, 
                                        CompositeGridOperators & op,
                                        const IntegerArray & boundaryConditions,
                                        const RealArray & bcData, 
                                        RealArray & constantCoeff=Overture::nullRealArray(),
                                        realCompositeGridFunction *variableCoeff=NULL );

  // supply matrix coefficients and boundary conditions
  int setCoefficientsAndBoundaryConditions( realCompositeGridFunction & coeff,
                                            const IntegerArray & boundaryConditions,
					    const RealArray & bcData );


  void determineErrors(realCompositeGridFunction & u,  // is this needed?
                       OGFunction & exactSolution,
		       int & printOptions );

  aString getErrorMessage( const int errorNumber );    // is this needed?

  int get( OgesParameters::OptionEnum option, int & value ) const;
  int get( OgesParameters::OptionEnum option, real & value ) const;

  int getCompatibilityConstraint() const;
  int getNumberOfIterations();
  real getMaximumResidual() const;

  int initialize( );
  bool isSolverIterative() const;  // TRUE if the solver chosen is an iterative method
  bool canSolveInPlace() const;

  // return the matrix in compressed row format
  int getMatrix( IntegerArray & ia, IntegerArray& ja, RealArray & a, SparseStorageFormatEnum format=compressedRow );
  // compute y=A*x 
  int matrixVectorMultiply( int n, real *x, real *y );
  // compute r=A*x-b
  int computeResidual( int n, real *x, real *b, real *r );
  // assign the values of the grid function u into the vector x
  int assignVector( int n, real *x, realCompositeGridFunction & u );
  // store the vector x into the grid function u
  int storeVector( int n, real *x, realCompositeGridFunction & u );

  // output any relevant statistics 
  int printStatistics( FILE *file = stdout ) const;

  // ----------------Functions to set parameters -----------------------------
  int set( OgesParameters::SolverEnum option );
  int set( OgesParameters::SolverMethodEnum option );
  int set( OgesParameters::MatrixOrderingEnum option );
  int set( OgesParameters::PreconditionerEnum option );
	   
  int set( OgesParameters::OptionEnum option, int value=0 );
  int set( OgesParameters::OptionEnum option, float value );
  int set( OgesParameters::OptionEnum option, double value );
	   		      
  int setOgesParameters( const OgesParameters & opar );
  
  // assign values to rhs for the the extra equations
  int setExtraEquationValues( realCompositeGridFunction & f, real *value );

  // return solution values from the extra equations
  int getExtraEquationValues( const realCompositeGridFunction & u, real *value );

  // evaluate the dot product of an extra equation times u 
  int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, int extraEquation=0 );

  //  evaluate the dot product of an extra equation times u and return the sum of the coefficients of the
  //  extra equation (i.e. the dot product with the "1" vector)
  int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, 
                             real & sumOfExtraEquationCoefficients, int extraEquation=0 );

  virtual real sizeOf( FILE *file=NULL ) const ; // return number of bytes allocated by Oges, print info to a file
  
  // data base IO
  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  int outputSparseMatrix( const aString & fileName );

  int writeMatrixToFile( aString fileName );
  int writeMatrixGridInformationToFile( aString fileName );
  int writePetscMatrixToFile( aString filename,
			      realCompositeGridFunction & u,
			      realCompositeGridFunction & f);

  void reference(const Oges &); 

  // Supply the coefficient matrix (and optionnally boundary conditions, needed buy multigrid for e.g.): 
  int setCoefficientArray( realCompositeGridFunction & coeff,
			   const IntegerArray & boundaryConditions=Overture::nullIntArray(),
                           const RealArray & bcData=Overture::nullRealArray() );
  int setCoefficientArray( realMappedGridFunction & coeff,
			   const IntegerArray & boundaryConditions=Overture::nullIntArray(),
                           const RealArray & bcData=Overture::nullRealArray() );

  // only solve the equations on some grids:
  int setGridsToUse( const IntegerArray & gridsToUse ); 
  bool activeGrid( int grid ) const;  // return true if this grid is used
  const IntegerArray & getUseThisGrid() const; // useThisGrid(grid)=true if the grid is active

  // supply command line arguments used by some solvers such as PETSc
  int setCommandLineArguments( int argc, char **argv );

  void setGrid( CompositeGrid & cg, bool outOfDate=true );
  void setGrid( MappedGrid & mg, bool outOfDate=true );

  // Set the MultigridCompositeGrid to use: (for use with Ogmg)
  void set( MultigridCompositeGrid & mgcg );

  // Set the name of the composite grid (for info in files)
  void setGridName( const aString & name );

  // Set the name of this instance of Ogmg (for info in debug files)
  void setSolverName( const aString & name );

  int solve( realCompositeGridFunction & u, realCompositeGridFunction & f ); 
  int solve( realMappedGridFunction & u, realMappedGridFunction & f ); 

  int updateToMatchGrid( CompositeGrid & cg );
  int updateToMatchGrid( MappedGrid & mg );
  
  int update( GenericGraphicsInterface & gi, CompositeGrid & cg ); // update parameters interactively


  typedef EquationSolver* (*newPETScFunction)(Oges & oges); 
  

 public:
  // Here are the data and functions used by the EquationSolver classes

  int ndia;
  int ndja;
  int nda;

  RealArray sol,rhs;  // solution and rhs stored as a single long vector
  IntegerArray ia,ja; // rows and columns stored in a sparse format
  RealArray a;        // matrix entries stored in a sparse format

  
  int buildEquationSolvers(OgesParameters::SolverEnum solver);  // call this to build a sparse solver.

  int formMatrix(int & numberOfEquations, int & numberOfNonzeros,
                 SparseStorageFormatEnum storageFormat,
		 bool allocateSpace = TRUE,
		 bool factorMatrixInPlace = FALSE );

  int formRhsAndSolutionVectors(realCompositeGridFunction & u, 
				realCompositeGridFunction & f );
  int storeSolutionIntoGridFunction();


  
 public:

  CompositeGrid cg;
  OgesParameters parameters;    // This object holds parameters for Oges
  MultigridCompositeGrid mgcg;  // for Ogmg, holds multigrid hierarchy

  realCompositeGridFunction coeff;    // holds discrete coefficients

  intArray *classify;                 // holds classify arrays if we destroy the coeff grid function;

  realCompositeGridFunction uLinearized;       // linearized u for nonlinear problems

  ListOfRealSerialArray ul,fl; // These are referenced to the user's u and f

  // extra equation info, such as a compatibility constraint
  int numberOfExtraEquations;          
  IntegerArray extraEquationNumber;
  realCompositeGridFunction *coefficientsOfDenseExtraEquations;
  realCompositeGridFunction rightNullVector;   // right null vector used as a compatibility constraint

  int solvingSparseSubset; // set to true if we are solving equations on a sparse subset of the grid (e.g. interface)

  int solverJob;          // job
  int initialized;        // logical
  bool shouldBeInitialized;   // TRUE is initialize() should be called
  int numberOfGrids;  // local copy
  int numberOfDimensions;
  int numberOfComponents; // same as in coeff[0].sparse->numberOfComponents
  int stencilSize;

  int refactor;
  int reorder;
  int evaluateJacobian;
  bool recomputePreconditioner;

  // int matrixHasChanged;  // true if the matrix has changed. 
  
  int numberOfEquations;       // neq
  int numberOfNonzerosBound;   // nqs : bound on number of nonzeros
  int numberOfNonzeros;        // nze

    
  int preconditionBoundary;
  int preconditionRightHandSide;

  // Convert an Equation Number to a point on a grid (Inverse of equationNo)
  void equationToIndex( const int eqnNo0, int & n, int & i1, int & i2, int & i3, int & grid );
  

  int numberOfIterations; // number of iterations used by iterative solvers

  aString gridName;  // name of the composite grid (used to name gridCheckFile)
  aString solverName;  // name of this instance of Ogmg

  int argc;    // copy of command line arguments for PETSc
  char **argv;
  
  enum EquationSolverEnum
  {
    maximumNumberOfEquationSolvers=10
  };
  // Here is where we keep the objects that interface to various solvers: yale, harwell, slap, petsc..
  EquationSolver *equationSolver[maximumNumberOfEquationSolvers];
  
// -----------Here are protected member functions-------------------
 protected:

  OgesParameters::EquationEnum equationToSolve;
  RealArray constantCoefficients;    // for second order constant coefficients predefined equation
  IntegerArray boundaryConditions;  // bc's for predefined equations
  RealArray bcData;                 // data for bc's such as the coeff's of a mixed BC


  IntegerArray gridEquationBase;  // gridEquationBase(grid) = first eqn number on a grid.

  bool useAllGrids;            //  false if we only solve on a subset of grids.
  IntegerArray useThisGrid;    // only solve on this list of grids.


  void setup();
  void findExtraEquations();
  void makeRightNullVector();
  void generateMatrixError( const int nda, const int ieqn );
  void generateMatrix( int & errorNumber );

  void privateUpdateToMatchGrid();

  // --------Utility functions:-------------
  
  inline int arraySize(const int grid, const int axis );
  inline int arrayDims(const int grid, const int side, const int axis );

  // Return the equation number for given indices
  inline int equationNo( const int n, const int i1, const int i2, const int i3, const int grid );
  
  IntegerDistributedArray equationNo(const int n, const Index & I1, const Index & I2, const Index & I3, 
		      const int grid );

 public:
  static int debug;

  static int petscIsAvailable;  // set to true if PETSc is available
  static newPETScFunction createPETSc;  // pointer to a function that can "new" a PETSc instance

 public:

  int printObsoleteMessage(const aString & routineName, int option =0 );

// ************************************************************************************************
// ----------all the remaining stuff in the class is obsolete-----------------------------------------
// ************************************************************************************************

 void setCompositeGrid( CompositeGrid & cg );

 enum coefficientTypes // coefficients can be supplied in continous or discrete form
 {
   continuous=0,
   discrete=1
 };

 public:
  // enumerators for available solvers 

   enum solvers
   {
     yale=1,
     harwell=2,
     bcg=3,
     sor=4,
     SLAP,
     PETSc
   };

   enum conjugateGradientTypes
   {
     biConjugateGradient=0,
     biConjugateGradientSquared=1,
     GMRes=2,
     CGStab=3
   };

   enum conjugateGradientPreconditioners
   {
     none=0,
     diagonal=1,
     incompleteLU=2,
     SSOR=3
   };


  void setConjugateGradientType( const conjugateGradientTypes conjugateGradientType );
  void setConjugateGradientPreconditioner( 
     const conjugateGradientPreconditioners conjugateGradientPreconditioner);
  void setConjugateGradientNumberOfIterations( const int conjugateGradientNumberOfIterations);  
  void setConjugateGradientNumberOfSaveVectors( 
     const int conjugateGradientNumberOfSaveVectors ); 
   void setConjugateGradientTolerance( const real conjugateGradientTolerance ); 
   void setCompatibilityConstraint( const bool trueOrFalse ); 
   void setEvaluateJacobian( const int EvaluateJacobian );
   void setFillinRatio( const real fillinRatio );
   void setFillinRatio2( const real fillinRatio2 );
   void setFixupRightHandSide( const bool trueOrFalse );
   void setHarwellTolerance( const real harwellTolerance);  // tolerance for harwell pivoting
   void setIterativeImprovement( const int trueOrFalse );  

  void setNumberOfComponents( const int numberOfComponents );  // **** this is needed or get from SparseRep!

  void setNullVectorScaling(const real & scale );
  void setMatrixCutoff( const real matrixCutoff );

  void setOrderOfAccuracy( const int order ); // **** this is needed or get from SparseRep!

  void setPreconditionBoundary( const int preconditionBoundary );
  void setPreconditionRightHandSide( const int preconditionRightHandSide );
  void setRefactor( const int refactor );
  void setReorder( const int reorder );
  void setSolverJob( const int solverJob );
  void setSolverType( const solvers solverType );
  void setSorNumberOfIterations( const int sorNumberOfIterations );  
  void setSorTolerance( const real sorTolerance );
  void setSorOmega( const real sorOmega ); 
//  void setSparseFormat( const int sparseFormat );
  void setTranspose( const int transpose );
  void setZeroRatio( const real zeroRatio );

  void setCoefficientType( const coefficientTypes coefficientType );


//  solvers solverType;         // solver
  coefficientTypes coefficientType;

  real actualZeroRatio;         
  real actualFillinRatio;       
  real actualFillinRatio2;      
  real maximumResidual;  
  
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------


};


inline int Oges::
arraySize(const int grid, const int axis )
{
  return cg[grid].dimension(End,axis)-cg[grid].dimension(Start,axis)+1;
}

inline int Oges::
arrayDims(const int grid, const int side, const int axis )
{
  return cg[grid].dimension(side,axis);
}


inline int Oges::
equationNo( const int n, const int i1, const int i2, const int i3, 
	    const int grid )
//=============================================================================
  // Return the equation number for given indices
  //  n : component number ( n=0,1,..,numberOfComponents-1 )
  //  i1,i2,i3 : grid indices
  //  grid : component grid number (grid=0,1,2..,numberOfCompoentGrids-1)   
  //=============================================================================
{
  return n+1+   numberOfComponents*(i1-cg[grid].dimension(Start,axis1)+
	(cg[grid].dimension(End,axis1)-cg[grid].dimension(Start,axis1)+1)*(i2-cg[grid].dimension(Start,axis2)+
        (cg[grid].dimension(End,axis2)-cg[grid].dimension(Start,axis2)+1)*(i3-cg[grid].dimension(Start,axis3)
							 ))) + gridEquationBase(grid);
}


#endif // OGES_H

