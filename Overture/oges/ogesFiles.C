#include "Oges.h"
#include "EquationSolver.h"
#include "SparseRep.h"


//===================================================================================
// Overlapping Grid Equation Solver
//
//===================================================================================

int Oges::debug=0;

//\begin{>>OgesInclude.tex}{\subsection{solve}} 
int Oges::
solve( realCompositeGridFunction & u, realCompositeGridFunction & f )
//=====================================================================================
// /Purpose: 
//  Solve (first factoring if necessary) the system of equations.
// /u (output): Put the solution in this grid function.
// /f (input): This grid function is the right-hand-side (f and u can be the same)
// /Errors:  Some...
// /Return Values: An error number that can be decoded using {\ff getErrorMessage}.
//\end{OgesInclude.tex}
//=====================================================================================
{
  if( equationSolver[parameters.solver]==NULL )
    buildEquationSolvers(parameters.solver);  
  
  assert( (int)parameters.solver>0 && (int)parameters.solver<(int)maximumNumberOfEquationSolvers );
  if( equationSolver[parameters.solver]!=NULL )
  {
    if( u.getComponentDimension(0) < numberOfComponents )
    {
      printF("Oges::solve:ERROR: The solution realCompositeGridFunction u has too few components =%i\n"
	     "   The implicit system has %i components\n",u.getComponentDimension(0),numberOfComponents);
      Overture::abort("error");
    }
    if( f.getComponentDimension(0) < numberOfComponents )
    {
      printF("Oges::solve:ERROR: The RHS realCompositeGridFunction f has too few components =%i\n"
	     "   The implicit system has %i components\n",f.getComponentDimension(0),numberOfComponents);
      Overture::abort("error");
    }

    return equationSolver[parameters.solver]->solve(u,f);
  }
  else // if( parameters.solver!=SLAP )
  {
      printf("Oges::solve:ERROR: %s is not available \n",
         (const char *)parameters.getSolverTypeName());
      Overture::abort("error");
  }
  return -1;
}


//\begin{>>OgesInclude.tex}{\subsection{solve}} 
int Oges::
solve( realMappedGridFunction & u, realMappedGridFunction & f )
//=====================================================================================
// /Purpose: Solve the system Au=f
//\end{OgesInclude.tex}
//=====================================================================================
{
  Range R[4] = { nullRange,nullRange,nullRange,nullRange };
  R[u.positionOfComponent(0)]= Range(u.getComponentBase(0),u.getComponentBound(0));

  realCompositeGridFunction u0(cg,R[0],R[1],R[2],R[3]);
  u0[0].reference(u);
  realCompositeGridFunction f0(cg,R[0],R[1],R[2],R[3]);
  f0[0].reference(f);

  return solve(u0,f0);
  
}    


void Oges::
findExtraEquations()
{
  //====================================================================
  //  
  //   Purpose:
  //     Determine locations for extra equations, such as the
  //  constraint equation in the Laplace-Neumann problem or
  //  the eigenvalue in an eigen-problem
  //
  //
  //   Method
  //     Find unused points which can be used for the extra equations
  //  
  //   Output
  //     Assign : extraEquationNumber(i) i=0,...,numberOfExtraEquations-1
  //====================================================================

  if( numberOfExtraEquations <= 0 )
    return;

  extraEquationNumber.redim(numberOfExtraEquations);

  int i1,i2,i3;

  const int startingExtraEquationClassifyValue=10;
  
  int i=0;
  for( int grid=numberOfGrids-1; grid>=0; grid-- )
  {
    MappedGrid & c = cg[grid];
    IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;
    // kkc 060801 added another loop over components so that we get the right value when the number of components is >1
    //            !!! we can either choose the first equation of all the components or the last; but the code in generateMatrix only
    //                works if we use the very last equation so start from the end and work back
    for ( int nc = classifyX.getLocalBound(axis3+1); nc>=classifyX.getLocalBase(axis3+1)&&i<numberOfExtraEquations; nc-- )
      //    for ( int nc = classifyX.getLocalBase(axis3+1); nc<=classifyX.getLocalBound(axis3+1)&&i<numberOfExtraEquations; nc++ )
      {
	for( i3 =c.dimension()(End  ,axis3);
	     i3>=c.dimension()(Start,axis3) && i<numberOfExtraEquations; i3-- )
	  {
	    for( i2 =c.dimension()(End,  axis2);
		 i2>=c.dimension()(Start,axis2) && i<numberOfExtraEquations; i2-- )
	      {
		for( i1 =c.dimension()(End  ,axis1);
		     i1>=c.dimension()(Start,axis1) && i<numberOfExtraEquations; i1-- )
		  {
		    // classify already may have an extra eqn in it if, for example, initialize
		    // was called twice
		    // *wdh	  if( classifyX(i1,i2,i3)==unused && i<numberOfExtraEquations )  
		    if( (classifyX(i1,i2,i3,nc)==SparseRepForMGF::unused 
			 || classifyX(i1,i2,i3,nc) >= startingExtraEquationClassifyValue) && i<numberOfExtraEquations )
		      {
			// ...This point is not used yet, use it for an extra equation
			extraEquationNumber(i)=equationNo(nc,i1,i2,i3,grid);
			classifyX(i1,i2,i3,nc)=startingExtraEquationClassifyValue+i;
			i++;
		      }
		  } //end i1
	      } // end i2
	  } // end i3
      }// end nc
  }// end grid
  if( i < numberOfExtraEquations )
  {
    cerr << "Oges:findExtraEquations:ERROR unable to find locations for extra equations" << endl;
    cerr << "  This application is requesting numberOfExtraEquations ="
         << numberOfExtraEquations << endl;
    cerr << "  Extra equations are placed at unused points on the grid " << endl;
    cerr << "  You could add an extra ghostline to one of the grids " << endl;
    exit(1);
  }    
}

//================================================================
//  
//   Purpose:
//     Assign the right null vector
//  
//   Input
//  
//   Output
//================================================================
void Oges::
makeRightNullVector()
{
  if( parameters.compatibilityConstraint  && !parameters.userSuppliedCompatibilityConstraint )
  {
    if( Oges::debug & 2 )
      printF("--OGES-- makeRightNullVector...\n");

    rightNullVector.updateToMatchGrid(cg,nullRange,nullRange,nullRange,1);  // positionOfComponent=3

    Index I1,I2,I3;
    for( int grid=numberOfGrids-1; grid>=0; grid-- )
    {
      MappedGrid & c = cg[grid];
      IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;

      getIndex(c.dimension(),I1,I2,I3);
      rightNullVector[grid]=0.;
      // do not include the ghost line so that we retain u.n=0 as a BC!
      where( classifyX(I1,I2,I3,0)==SparseRepForMGF::interior || classifyX(I1,I2,I3,0)==SparseRepForMGF::boundary ) // kkc 090903 added the zero index so this works when we have a system that only needs one constraint
	rightNullVector[grid](I1,I2,I3)=parameters.nullVectorScaling;
    }
  }
  
}


