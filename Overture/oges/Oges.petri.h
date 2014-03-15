#ifndef OGES_H
#define OGES_H "Oges.h"



//===========================================================================
//  Overlapping Grid Equation Solver
//
//===========================================================================

#include "Overture.h"          
#include <wdhdefs.h>           // some useful defines and constants
#include <mathutil.h>          // define max, min,  etc
#include "aString.H"            // string Class
#include "OGFunction.h"
#include "CompositeGrid.h"
#include "CompositeGridFunction.h"
#include "CompositeGridOperators.h"

#include "ListOfIntSerialArray.h"
#include "ListOfFloatSerialArray.h"
#include "ListOfDoubleSerialArray.h"

#ifdef DOUBLE
  typedef ListOfDoubleSerialArray ListOfRealSerialArray;
#else
  typedef ListOfFloatSerialArray ListOfRealSerialArray;
#endif

class GenericGraphicsInterface;
class OgesParameters;

class Oges 
{
  public:

  Oges();	
  Oges( CompositeGrid & cg );
  Oges( MappedGrid & mg );
  Oges(const Oges & X);
  Oges & operator=( const Oges & x );
  ~Oges();	
   
  void setup();
  void cleanup();
  void reference(const Oges &); 

 // these should be in a derived class?
 public:
  realCompositeGridFunction uLinearized;  // linearized u for nonlinear problems
  realCompositeGridFunction rightNullVector;  
 
  enum equationTypes
  {
    LaplaceDirichlet=0,
    LaplaceNeumann=1,
    LaplaceMixed=2,
    Nonlinear1=3,
    Eigenvalue=4,
    Biharmonic=5,
    userSuppliedArray=6,
    Interpolation=7
  };

 equationTypes equationType;
 

  enum classifyTypes 
  {
    interior=1,
    boundary=2,
    ghost1=3,
    ghost2=4,
    ghost3=5,
    ghost4=6,
    interpolation=-1,
    periodic=-2,
    extrapolation=-3,
    unused=0
  };

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
    sor=4
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

  // options for ghostlines:
  enum ghostLineOptions
  {
    extrapolateGhostLine,
    useGhostLine,
    useGhostLineExceptCorner,
    useGhostLineExceptCornerAndNeighbours
  };

  enum GeneralOptions
  {
    minimumNumberOfIterations=0
    };
  

  int updateToMatchGrid( CompositeGrid & cg );
  int updateToMatchGrid( MappedGrid & mg );
  
  // ----------------Functions to set parameters -----------------------------
  int setOption( const GeneralOptions & option, const int & value );
  int setOption( const GeneralOptions & option, const real & value );

  void setAddBoundaryConditions( const bool trueOrFalse );
  void setCoefficientType( const coefficientTypes coefficientType );
  int  setCoefficientArray( realCompositeGridFunction & coeff );
  int  setCoefficientArray( realMappedGridFunction & coeff );
  void setCompositeGrid( CompositeGrid & cg );
  void setGrid( CompositeGrid & cg );
  void setGrid( MappedGrid & mg );
  void setConjugateGradientType( const conjugateGradientTypes conjugateGradientType );
  void setConjugateGradientPreconditioner( 
       const conjugateGradientPreconditioners conjugateGradientPreconditioner);
  void setConjugateGradientNumberOfIterations( const int conjugateGradientNumberOfIterations);  
  void setConjugateGradientNumberOfSaveVectors( 
       const int conjugateGradientNumberOfSaveVectors ); 
  void setConjugateGradientTolerance( const real conjugateGradientTolerance ); 
  void setCompatibilityConstraint( const bool trueOrFalse ); 
  void setEquationType( const equationTypes equationType );
  void setEvaluateJacobian( const int EvaluateJacobian );
  void setFillinRatio( const real fillinRatio );
  void setFillinRatio2( const real fillinRatio2 );
  void setFixupRightHandSide( const bool trueOrFalse );
  void setGhostLineOption( const int ghostLine, const ghostLineOptions option, 
            const int n=-1, const int grid=-1, const int side=-1, const int axis=-1 );
  void setHarwellTolerance( const real harwellTolerance);  // tolerance for harwell pivoting
  void setIterativeImprovement( const int trueOrFalse );  
  void setNumberOfComponents( const int numberOfComponents );
  void setNumberOfExtraEquations( const int numberOfExtraEquations );
  void setNumberOfGhostLines( const int numberOfGhostLines );
  void setNullVectorScaling(const real & scale );
  void setMatrixCutoff( const real matrixCutoff );

  int  setOgesParameters( const OgesParameters & opar );
  
  void setOrderOfAccuracy( const int order );
  void setPreconditionBoundary( const int preconditionBoundary );
  void setPreconditionRightHandSide( const int preconditionRightHandSide );
  void setRefactor( const int refactor );
  void setReorder( const int reorder );
  void setSolverJob( const int solverJob );
  void setSolverType( const solvers solverType );
  void setSorNumberOfIterations( const int sorNumberOfIterations );  
  void setSorTolerance( const real sorTolerance );
  void setSorOmega( const real sorOmega ); 
  void setSparseFormat( const int sparseFormat );
  void setTranspose( const int transpose );
  void setZeroRatio( const real zeroRatio );
  
  int update( GenericGraphicsInterface & gi ); // update parameters interactively
  
  bool isSolverIterative();  // TRUE if the solver chosen is an iterative method

 public:

  CompositeGrid cg;
  CompositeGridOperators operators;

  int numberOfComponentsForClassify;  
  intCompositeGridFunction classify;   // classify[grid](i1,i2,i3,n)

  int preconditionBoundary;
  int preconditionRightHandSide;
  int orderOfAccuracy;
  int maximumInterpolationWidth;
  coefficientTypes coefficientType;
  realCompositeGridFunction coeff;    // holds discrete coefficients
  intCompositeGridFunction equationNumber;
  bool compatibilityConstraint;
  bool addBoundaryConditions;

  int numberOfIterations; // number of iterations used by iterative solvers
  int minNumberOfIterations; // minimum Number Of Iterations
  real maximumResidual;  
  
  ListOfRealSerialArray ul,fl; // These are referenced to the user's u and f

  // These are for interpolation:
  IntegerArray interpWidth;           // actual width
  IntegerArray interpolationWidth;    // use this width

  IntegerArray kint;  

  int maximumNumberOfCoefficients;     // formally set by cgde
  int numberOfExtraEquations;          // formally nx
  IntegerArray extraEquationNumber;
  realCompositeGridFunction *coefficientsOfDenseExtraEquations;

  int realToIntegerRatio;   // lratio
  real machineEpsilon;      // epslon
  real matrixCutoff;        // epsz

  bool fixupRightHandSide; // zero out rhs at interp., extrap and periodic Points?

  int numberOfGhostLines;
  int maximumNumberOfGhostLines;
  ListOfIntSerialArray ghostLineOption;  // options for ghost lines

  solvers solverType;         // solver
  int solverJob;          // job
  int initialized;        // logical
  bool shouldBeInitialized;   // TRUE is initialize() should be called
  int numberOfComponentGrids;  // local copy
  int numberOfDimensions;
  int numberOfComponents; // nv
  int transpose;
  real zeroRatio;         // zratio
  real fillinRatio;       // fratio
  real fillinRatio2;      // fratio2
  real actualZeroRatio;         
  real actualFillinRatio;       
  real actualFillinRatio2;      
  real harwellTolerance;  // tolerance for harwell pivoting
  real nullVectorScaling;

  int refactor;
  int reorder;
  int evaluateJacobian;

  int conjugateGradientType;                  // icg
  int conjugateGradientPreconditioner;       // ipc
  int conjugateGradientNumberOfIterations;   // nit
  int conjugateGradientNumberOfSaveVectors;  // nsave
  real conjugateGradientTolerance;            // tol

  int sorNumberOfIterations;                  // nit
  real sorTolerance;                          // tol
  real sorOmega;                              // omega

  int numberOfEquations;       // neq
  int numberOfNonzerosBound;   // nqs : bound on number of nonzeros
  int numberOfNonzeros;        // nze


  //   sparseFormat = sparse storage format
  //         = 0 : ia() stored in compressed mode
  //         = 1 : ia() stored in uncompressed mode
  int sparseFormat;     // ispfmt

  int storageFormat;   // isf

  int iterativeImprovement;  // itimp

  IntegerArray eqnNo;
  IntegerArray peqn;
  int stencilLength; 
    
  IntegerArray i1v,kv;
  RealArray sol,rhs;
  IntegerArray perm,iperm;
  int ndia;
  int ndja;
  int nda;
  IntegerArray ia,ja;
  RealArray a;

  RealArray wh;        // for harwell
  IntegerArray ikeep,iwh;

  IntegerArray ias,jas;                // For iterative improvement
  RealArray rhsii,as,vii,resii; 

  int nsp;         // For Yale
  RealArray rsp;
  int yaleExcessWorkSpace;

  int ndiwk,ndwk;  // for CG, GMRES routines
  IntegerArray iwk;
  RealArray wk;

  // For boundary preconditioner:
  int bpcNumberOfEquations;  // neqp (Boundary Preconditioner)
  int bpcNumberOfNonzeros;   // nzep
  int bpciep;                // iep
  int bpciap;                // iap
  int bpcjap;                // jap
  int bpcap;                 // ap
  int bpcrhsp;               // rhsp
  IntegerArray iep,iap,jap;
  RealArray ap,rhsp;
  IntegerArray iabpc;
  IntegerArray iwkbpc;
  IntegerArray iab,jab,ia1,ja1,iep0,iap0,jap0,iepi;
  RealArray rwkbpc;
  RealArray ab,a1,ap0;


  public:

  // -------Here are the public member functions-------------

  void determineErrors( realCompositeGridFunction & u, OGFunction & exactSolution,
		       int & printOptions );
  aString getErrorMessage( const int errorNumber );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  int initialize( );
  int solve( realCompositeGridFunction & u, realCompositeGridFunction & f ); 
  int solve( realMappedGridFunction & u, realMappedGridFunction & f ); 


  //These work as `solve' and form the matrix,storage etc, but no solve
  // USE these to extract the matrix for another solver  **pf** 12/8/98
  int formMatrix( realCompositeGridFunction & u, realCompositeGridFunction & f ); 
  int formMatrix( realMappedGridFunction & u, realMappedGridFunction & f ); 

  void formRhsAndSolutionVectors( int & ierr );
  void storeSolIntoGridFunction( int & ierr );

  void equationToIndex( const int eqnNo0, int & n, int & i1, int & i2, int & i3, int & grid )
  //=============================================================================
  // Convert an Equation Number to a point on a grid (Inverse of equationNo)
  // input -
  //  eqnNo0 : equation number
  // Output
  //  n : component number ( n=0,1,..,numberOfComponents-1 )
  //  i1,i2,i3 : grid indices
  //  grid : component grid number (grid=0,1,2..,numberOfCompoentGrids-1)   
  //=============================================================================
  {
    int eqn=eqnNo0;
    grid=numberOfComponentGrids-1;
    for( int grid1=1; grid1<numberOfComponentGrids; grid1++ )
    {
      if( eqn <= peqn(grid1+1) )
      {
	grid=grid1-1;
	break;
      }
    }
    eqn-=(peqn(grid+1)+1);
    n= (eqn % numberOfComponents);
    eqn/=numberOfComponents;
    i1=(eqn % arraySize(grid,axis1))+arrayDims(grid,Start,axis1);
    eqn/=arraySize(grid,axis1);
    i2=(eqn % arraySize(grid,axis2))+arrayDims(grid,Start,axis2);
    eqn/=arraySize(grid,axis2);
    i3=(eqn % arraySize(grid,axis3))+arrayDims(grid,Start,axis3);

  }

  // ---------functions for Integration weights:-------------
  void scaleIntegrationCoefficients( realCompositeGridFunction & u );
  
  inline real weightedArea( const int i1, const int i2, const int i3, RealDistributedArray & u, 
			     const RealDistributedArray & rsxy, const RealArray & gridSpacing );
  inline real jacobian( const int i1, const int i2, const int i3, const RealDistributedArray & rsxy );
  void surfaceArea( real & ds, real & dse, IntegerArray & iv, int & axis, 
		   const RealDistributedArray & rsxy, const RealArray & gridSpacing, const IntegerArray & arrayDimensions );
  void integrate( realCompositeGridFunction & ogic, realCompositeGridFunction & f , 
                  real & volumeIntegral, real & surfaceIntegral );
  void ogres( realCompositeGridFunction & u, realCompositeGridFunction & f, 
              realCompositeGridFunction & v, real & resmx );

  // -----------Here are protected member functions-------------------
  protected:

  virtual void assignDiscreteCoefficients( const int grid );
  

  // -----------Here are the private member functions---------------
  private:

  void addBoundaryConditionsToMatrix( const int grid );
  void allocate1();  // allocate work spaces
  void allocate2();  // allocate more work spaces
  void assignClassification();  //  assign the classify array etc.
  void getInterpolationCoefficients(const int grid);
  void getExtrapCoeff( const IntegerArray & iv, const int grid, const int n, const int order, 
		      RealDistributedArray & coeff, IntegerDistributedArray & equationNumber );
  void findExtraEquations();
  void initialize2( int & errorNumber );
  void makeRightNullVector();
  void solveEquations( int & errorNumber );
  void callSparseSolver( int & errorNumber );
  void setupBoundaryPC( int &  neqp, int & nzep, int & errorNumber );
  void setupBoundaryPC2( IntegerArray & ia0, int & nfict, int &  neqb, int & nzeb, 
			      int & nze1, int & neqp, int & nzep, IntegerArray & iepi0 );
  void generateMatrixError( const int nda, const int ieqn );
  void generateMatrix( int & errorNumber );
  void getSpecialCoefficients( const int grid, const IntegerArray & iv );

  void privateUpdateToMatchGrid();

  // --------Utility functions:-------------
  
  inline int arraySize(const int grid, const int axis )
  {
    return cg[grid].dimension(End,axis)-
           cg[grid].dimension(Start,axis)+1;
  }
  inline int arrayDims(const int grid, const int side, const int axis )
  {
    return cg[grid].dimension(side,axis);
  }
  inline int gridIndex(const int grid, const int side, const int axis )
  {
    return cg[grid].gridIndexRange(side,axis);
  }
  inline int eqn2( const int n, const int i1, const int i2, const int i3, const int grid);
  inline int equationNo( const int n, const int i1, const int i2, const int i3, 
                         const int grid )
  //=============================================================================
  // Return the equation number for given indices
  //  n : component number ( n=0,1,..,numberOfComponents-1 )
  //  i1,i2,i3 : grid indices
  //  grid : component grid number (grid=0,1,2..,numberOfCompoentGrids-1)   
  //=============================================================================
  {
    return n+1+   numberOfComponents*(i1-arrayDims(grid,Start,axis1)+
               arraySize(grid,axis1)*(i2-arrayDims(grid,Start,axis2)+
               arraySize(grid,axis2)*(i3-arrayDims(grid,Start,axis3)))) + peqn(grid+1);
  }
  IntegerDistributedArray equationNo(const int n, const Index & I1, const Index & I2, const Index & I3, 
		      const int grid );

  // --- These are for the discrete coefficient functions----
  int dum;                 // these are for the merge0 macro
  Range aR0,aR1,aR2,aR3;
  int width,halfWidth,halfWidth3;
  RealArray d12,d22,d14,d24;
  RealArray delta;
  RealArray Dr,Ds,Dt,Drr,Dss,Dtt,Drs,Drt,Dst;
  RealArray Dr4,Ds4,Dt4,Drr4,Dss4,Dtt4,Drs4,Drt4,Dst4;

  void defineVertexDifferences( MappedGrid & c );

  void userSuppliedCoefficients( const int grid );

  void laplaceDirichletVertexNonConservative( const int grid );

  void laplaceNeumannVertexNonConservative( const int grid );

  void laplaceMixedVertexNonConservative( const int grid );

  void biharmonicDirichletVertexNonConservative( const int grid );

  void laplaceDirichletCellConservative( const int grid );

  void laplaceNeumannCellConservative( const int grid );

  void implicitInterpolation( const int grid );


 public:
  static int debug;

 

};

#define RSXY(i1,i2,i3,m,n) rsxy(i1,i2,i3,m+numberOfDimensions*(n))
inline real Oges::
weightedArea( const int i1, const int i2, const int i3, RealDistributedArray & u, 
  const RealDistributedArray & rsxy, const RealArray & gridSpacing )
{
  return numberOfDimensions==2 ? 
     ( (RSXY(i1,i2,i3,axis1,axis1)*RSXY(i1,i2,i3,axis2,axis2)-
        RSXY(i1,i2,i3,axis1,axis2)*RSXY(i1,i2,i3,axis2,axis1))
       *u(i1,i2,i3)/(gridSpacing(axis1)*gridSpacing(axis2)))
   : ( numberOfDimensions==3 ?
     ( RSXY(i1,i2,i3,axis1,axis1)*
          (RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis2))
     -RSXY(i1,i2,i3,axis1,axis2)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis1))
     +RSXY(i1,i2,i3,axis1,axis3)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis2)
          -RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis1)) )
       *u(i1,i2,i3)/(gridSpacing(axis1)*gridSpacing(axis2)*gridSpacing(axis3))
      : 
         RSXY(i1,i2,i3,axis1,axis1)*u(i1,i2,i3)/gridSpacing(axis1)
      );
}	 

inline real Oges::jacobian( const int i1, const int i2, const int i3, const RealDistributedArray & rsxy )
{
  return numberOfDimensions==2 ? 
     ( (RSXY(i1,i2,i3,axis1,axis1)*RSXY(i1,i2,i3,axis2,axis2)-
        RSXY(i1,i2,i3,axis1,axis2)*RSXY(i1,i2,i3,axis2,axis1)))
   :
     ( RSXY(i1,i2,i3,axis1,axis1)*
          (RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis2))
     -RSXY(i1,i2,i3,axis1,axis2)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis1))
     +RSXY(i1,i2,i3,axis1,axis3)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis2)
	  -RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis1)) );
}
#undef RSXY

inline int Oges::
eqn2( const int  n, const int  i1, const int  i2, const int  i3, 
                         const int grid)
{
  return                (i1-cg[grid].dimension(Start,axis1)+
   arraySize(grid,axis1)*(i2-cg[grid].dimension(Start,axis2)+
   arraySize(grid,axis2)*(i3-cg[grid].dimension(Start,axis3)+
   arraySize(grid,axis3)*n)));
}  

#endif // OGES_H

