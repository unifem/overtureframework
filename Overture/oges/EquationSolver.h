#ifndef EQUATION_SOLVER_H
#define EQUATION_SOLVER_H

#include "Oges.h"

// base class for equations solvers such as yale, harwell, slap, petsc etc.
class EquationSolver
{
 public:
  EquationSolver(Oges & oges_);
  virtual ~EquationSolver();

  virtual int allocateMatrix(int,int,int,int);

  virtual int displayMatrix();

  // Convert an Equation Number to a point on a grid (inverse of equationNo)
  virtual void equationToIndex( const int eqnNo0, int & n, int & i1, int & i2, int & i3, int & grid );

  // convert a component and grid point into an equation number
  virtual int equationNo( const int n, const int i1, const int i2, const int i3, const int grid );

  // evaluate the dot product of an extra equation times u 
  virtual int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, int extraEquation=0 );

  virtual int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, 
                                     real & sumOfExtraEquationCoefficients, int extraEquation=0 );
  // Find the locations for extra equations: 
  virtual int findExtraEquations();


  // return solution values from the extra equations
  virtual int getExtraEquationValues( RealArray & values ) const;
  // Old way: 
  virtual int getExtraEquationValues( const realCompositeGridFunction & u, real *value, const int maxNumberToReturn=1 );

  // return right-hand side values from the extra equations
  int getExtraEquationRightHandSideValues( RealArray & values ) const;

  virtual real getMaximumResidual(); 

  const aString & getName() const;

  virtual int getNumberOfIterations() const; 

  // initialize solver when the equations have changed.
  virtual int initialize();

  // -- print a description of the solver and options used
  virtual int printSolverDescription( const aString & label, FILE *file = stdout ) const; 

  virtual int printStatistics( FILE *file = stdout ) const;   // output any relevant statistics 

  virtual int saveBinaryMatrix(aString filename00,
			       realCompositeGridFunction & u,
			       realCompositeGridFunction & f);

  // Set the MultigridCompositeGrid to use: (for use with Ogmg)
  virtual int set( MultigridCompositeGrid & mgcg );

  // new way to set coefficients:
  virtual int setCoefficientsAndBoundaryConditions( realCompositeGridFunction & coeff,
                                                    const IntegerArray & boundaryConditions,
					            const RealArray & bcData );
  // old way to set coefficients: 
  virtual int setCoefficientArray( realCompositeGridFunction & coeff,
			           const IntegerArray & boundaryConditions=Overture::nullIntArray(),
                                   const RealArray & bcData=Overture::nullRealArray() );

  virtual int setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, 
                                                CompositeGridOperators & op, const IntegerArray & boundaryConditions,
						const RealArray & bcData, 
                                                RealArray & constantCoeff = Overture::nullRealArray(),
                                                realCompositeGridFunction *variableCoeff=NULL );

  // assign values to rhs for the the extra equations 
  virtual int setExtraEquationRightHandSideValues( realCompositeGridFunction & f, real *value );

  // assign initial guess to extra equation values (for iterative solvers)
  virtual int setExtraEquationValuesInitialGuess( real *value );

  // call this function when the grid changes (and before setCoefficientsAndBoundaryConditions)
  virtual int setGrid( CompositeGrid & cg );

  virtual int setMatrixElement(int,int,int,real);

  virtual real sizeOf( FILE *file=NULL ); // return number of bytes allocated 

  // solve the equations
  virtual int solve(realCompositeGridFunction & u,
		    realCompositeGridFunction & f)=0;

 protected:

  Oges & oges;
  OgesParameters & parameters;

  aString name;
  int numberOfEquations;
  int numberOfNonzeros;
  real maximumResidual;   // after solve, this is the maximumResidual (if computed)
  int numberOfIterations; // number of iterations required for the solve.
  
  // here we save the values of the current state so we can compare for any changes
  // with oges.parameters
  int solverMethod;
  int preconditioner;
  int matrixOrdering;
  int numberOfIncompleteLULevels;
  int gmresRestartLength;

};


#endif
