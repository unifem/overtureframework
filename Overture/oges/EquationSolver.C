#include "EquationSolver.h"
#include "ParallelUtility.h"


EquationSolver::
EquationSolver(Oges & oges_) : oges(oges_), parameters(oges_.parameters) 
{
  name="EquationSolver";
  
  numberOfEquations=0;
  numberOfNonzeros=0;
  maximumResidual=0.;   // after solve, this is the maximumResidual (if computed)
  numberOfIterations=0; 
}

EquationSolver::
~EquationSolver()
{
}


// =====================================================================================
// \brief Return the maximum residual.
// =====================================================================================
real EquationSolver::
getMaximumResidual()
{
  return maximumResidual;
} 

// =====================================================================================
// \brief Return the number of iterations used in the last solve.
// =====================================================================================
int EquationSolver::
getNumberOfIterations() const
{
  // By default return this value:
  return oges.numberOfIterations;
} 


const aString & EquationSolver::
getName() const
{
  return name;
}


// ============================================================================
/// \brief Print a description of the solver and options used.
// ============================================================================
int EquationSolver::
printSolverDescription( const aString & label, FILE *file /* = stdout */ ) const
{
  return 0;
}

//\begin{>>EquationSolverInclude.tex}{\subsection{printStatistics}} 
int EquationSolver::
printStatistics(FILE *file /* = stdout */ ) const
//===================================================================================
// /Description:
//   Output any relevant statistics
//\end{>>EquationSolverInclude.tex}
//===================================================================================
{
  // printF("EquationSolver::printStatistics in base class called!\n");
  return 0;
}

real EquationSolver::
sizeOf( FILE * file /* =NULL */ )
  // return number of bytes allocated 
{
  return 0;
}

int EquationSolver::
saveBinaryMatrix(aString filename00,
		 realCompositeGridFunction & u,
		 realCompositeGridFunction & f)
{
  cout << "EquationSolver::saveBinaryMatrix -- not saving, only available with PETSc\n";

  return(-1);
}

int EquationSolver::
displayMatrix()
{
  return 0;
}

int EquationSolver::
setMatrixElement(int nzcounter,int i,int j,real value)
{
  return(0);
}

int EquationSolver::
allocateMatrix(int ndia,int ndja,int nda,int N)
{
  return(0);
}


int EquationSolver::
setGrid( CompositeGrid & cg )
{
  printF("EquationSolver::setGrid:ERROR: base class function called\n");
  return -1;
}

  // Set the MultigridCompositeGrid to use: (for use with Ogmg)
int EquationSolver::
set( MultigridCompositeGrid & mgcg )
{
  printF("EquationSolver::set( MultigridCompositeGrid):ERROR: base class function called\n");
  return -1;
}


int EquationSolver::
setCoefficientsAndBoundaryConditions( realCompositeGridFunction & coeff,
				      const IntegerArray & boundaryConditions,
				      const RealArray & bcData )
{
  printF("EquationSolver::setCoefficientsAndBoundaryConditions:ERROR: base class function called\n");
  return -1;
}


int EquationSolver::
setCoefficientArray( realCompositeGridFunction & coeff,
		     const IntegerArray & boundaryConditions /* =Overture::nullIntArray() */,
		     const RealArray & bcData /* =Overture::nullRealArray() */ )
{
  printF("EquationSolver::setCoefficientArray:ERROR: base class function called\n");
  return -1;
}



int EquationSolver::
setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, CompositeGridOperators & op,
                                  const IntegerArray & boundaryConditions,
				  const RealArray & bcData, 
				  RealArray & constantCoeff,
                                  realCompositeGridFunction *variableCoeff )
{
  return -1;
}

int EquationSolver::
setExtraEquationValues( realCompositeGridFunction & f, real *value )
//==================================================================================
// /Description:
//   Assign values to the right-hand-side for the extra equations
//
// /f (input/output) : fill in rhs values here
// /values[i] (input) : values for each extra equation, i=0,1,2,...,
//
// /Return values: 0=success
//==================================================================================
{
  // here is the default implementation 
  CompositeGrid & cg = *f.getCompositeGrid();
  assert( oges.extraEquationNumber.getLength(0)>=oges.numberOfExtraEquations );
  for( int i=0; i<oges.numberOfExtraEquations; i++ )
  {
    int ne,i1e,i2e,i3e,gride;
    oges.equationToIndex( oges.extraEquationNumber(i),ne,i1e,i2e,i3e,gride);
    f[gride](i1e,i2e,i3e,ne)=value[i];

    if( Oges::debug & 4 )
      printF("EquationSolver::setExtraEquationValues: f[%i](%i,%i,%i,%i)= %14.10e (eqn-number=%i)\n",
	     gride,i1e,i2e,i3e,ne,f[gride](i1e,i2e,i3e,ne),oges.extraEquationNumber(i));

    if( gride!=cg.numberOfComponentGrids()-1 )
    {
      printf("EquationSolver::setExtraEquationValues:ERROR: The extra equation for the singular equation \n"
             "  is at f[%i](%i,%i,%i,%i) "
             "  BUT this point NOT on the last grid -- this can cause problems.  \n"
             "  Add an extra ghost line to the last grid to overcome this problem.\n",gride,i1e,i2e,i3e,ne);
      Overture::abort("error");
    }

  }
  return 0;
}


int EquationSolver::
getExtraEquationValues( const realCompositeGridFunction & u, real *value )
//==================================================================================
// /Description:
//   Return solution values from the extra equations
//
// /u(input) : grid function holding the solution.
// /value[i] (output) : values for each extra equation, i=0,1,2,...,
// 
// /Return values: 0=success
// /Author: wdh
//==================================================================================
{
  // here is the default implementation 
  const CompositeGrid & cg = *u.getCompositeGrid();
  assert( oges.extraEquationNumber.getLength(0)>=oges.numberOfExtraEquations );
  for( int i=0; i<oges.numberOfExtraEquations; i++ )
  {
    int ne,i1e,i2e,i3e,gride;
    oges.equationToIndex( oges.extraEquationNumber(i),ne,i1e,i2e,i3e,gride);
    value[i]=u[gride](i1e,i2e,i3e,ne);

    if( Oges::debug & 4 )
      printF("EquationSolver::getExtraEquationValues: value[%i] = u[%i](%i,%i,%i,%i)= %14.10e \n",
           i,gride,i1e,i2e,i3e,ne,u[gride](i1e,i2e,i3e,ne));

  }
  return 0;
}

int EquationSolver::
evaluateExtraEquation( const realCompositeGridFunction & u, real & value, int extraEquation /* =0 */ )
//==================================================================================
// /Description:
//    Evaluate the dot product of the coefficients of an extra equation times u 
//
// /u (input) : grid function to dot with the extra equation
// /value (output) : the dot product
// /extraEquation (input) : the number of the extra equation (0,1,...,numberOfExtraEquations-1)
// 
// /Return values: 0=success
// /Author: wdh
//==================================================================================
{
  real sumOfExtraEquationCoefficients=0.;
  return evaluateExtraEquation(u,value,sumOfExtraEquationCoefficients,extraEquation);
}


int EquationSolver::
evaluateExtraEquation( const realCompositeGridFunction & u, real & value, real & sumOfExtraEquationCoefficients,
                       int extraEquation /* =0 */ )
//==================================================================================
// /Description:
//    Evaluate the dot product of the coefficients of an extra equation times u 
//  Also return the sum of the coefficients of the extra equation (i.e. the dot product with the "1" vector)
//
// /u (input) : grid function to dot with the extra equation
// /value (output) : the dot product
// /extraEquation (input) : the number of the extra equation (0,1,...,numberOfExtraEquations-1)
// 
// /Return values: 0=success
// /Author: wdh
// /Changes: kkc 090903, removed the assertion for extraEquation==0 and added the extra equation as an index
//==================================================================================
{
  assert( oges.extraEquationNumber.getLength(0)>=oges.numberOfExtraEquations );
  Index I1,I2,I3;
  //kkc 090903  assert( extraEquation==0 );
  assert(extraEquation>=0 && extraEquation<oges.numberOfExtraEquations );

  const CompositeGrid & cg = *u.getCompositeGrid();
  value=0.;
  sumOfExtraEquationCoefficients=0.;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const MappedGrid & mg = cg[grid];
    getIndex(mg.dimension(),I1,I2,I3);

    // *** fix this for parallel ***
    const realSerialArray & uLocal = u[grid].getLocalArray();
    const realSerialArray & nullVector = oges.rightNullVector[grid].getLocalArray();
            
    int includeGhost=0;  // do NOT include ghost since we dont want to sum redundant pts
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);
    if( !ok ) continue;      

    value+=sum(nullVector(I1,I2,I3,extraEquation)*uLocal(I1,I2,I3,extraEquation));
    sumOfExtraEquationCoefficients+=sum(nullVector(I1,I2,I3,extraEquation));

//      if( debug() & 32 )
//        display(poisson->rightNullVector[grid],"-- right null vector",debugFile,"%10.4e ");

  }

  return 0;
}
