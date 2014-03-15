#ifndef EQUATION_SOLVER_H
#define EQUATION_SOLVER_H

#include "Oges.h"

// base class for equations solvers such as yale, harwell, slap, petsc etc.
class EquationSolver
{
 public:
  EquationSolver(Oges & oges_);
  virtual ~EquationSolver();
  virtual int solve(realCompositeGridFunction & u,
		    realCompositeGridFunction & f)=0;

  virtual int saveBinaryMatrix(aString filename00,
			       realCompositeGridFunction & u,
			       realCompositeGridFunction & f);

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

  // call this function when the grid changes (and before setCoefficientsAndBoundaryConditions)
  virtual int setGrid( CompositeGrid & cg );

  // Set the MultigridCompositeGrid to use: (for use with Ogmg)
  virtual int set( MultigridCompositeGrid & mgcg );

  const aString & getName() const;

  virtual real getMaximumResidual(); 

  virtual int printStatistics( FILE *file = stdout ) const;   // output any relevant statistics 

  // assign values to rhs for the the extra equations 
  virtual int setExtraEquationValues( realCompositeGridFunction & f, real *value );

  // return solution values from the extra equations
  virtual int getExtraEquationValues( const realCompositeGridFunction & u, real *value );

  // evaluate the dot product of an extra equation times u 
  virtual int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, int extraEquation=0 );

  virtual int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, 
                                     real & sumOfExtraEquationCoefficients, int extraEquation=0 );

  virtual int allocateMatrix(int,int,int,int);
  virtual int setMatrixElement(int,int,int,real);
  virtual int displayMatrix();

// So far a common data structure is used by all vector types, so there is no need to have these:
//  virtual void setRHSVectorElement(int,real);
//  virtual void setSolVectorElement(int,real);

  virtual real sizeOf( FILE *file=NULL ); // return number of bytes allocated 

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
