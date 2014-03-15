#include "Oges.h"
#include "SparseRep.h"
#include "OgesParameters.h"

int Oges::
printObsoleteMessage(const aString & routineName,
                     int option /* =0 */ )
{
  if( option==0 )
    printf("Oges::WARNING: routine %s is now obsolete. Use the new set function instead\n",
      (const char*)routineName);
  else
    printf("Oges::WARNING: routine %s is now obsolete. \n",(const char*)routineName);
  return 0;
}

void Oges::
setCoefficientType( const coefficientTypes coefficientType0 )
//======================================================================
//  Equations can be defined in a continuous or discrete manner
//
//=====================================================================
{
  // printf("setCoefficientType");
  coefficientType=coefficientType0;
}


/* ---
//\begin{>>OgesParametersInclude.tex}{\subsection{setAddBoundaryConditions}} 
void Oges::
setAddBoundaryConditions( const bool trueOrFalse )
//=====================================================================================
// /Purpose: If TRUE is passed then boundary conditions will be added to the matrix
//    according to the values found in the {\ff operators} member data.   
//    Use this option to add boundary conditions to a user supplied coefficient array.
// /trueOrFalse (input): A value of TRUE turns this option on, FALSE turns the option off
// /Notes:
//  For sides with Dirichlet BC's the ghostline equations are set to extrapolation
//  if the ghostlines are being used (ie. set with setGhostLineOption)
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  addBoundaryConditions=trueOrFalse;
}
--- */

//\begin{>>OgesParametersInclude.tex}{\subsection{setCompatibilityConstraint}}
void Oges::
setCompatibilityConstraint( const bool trueOrFalse )
//=====================================================================================
// /Purpose:
//   Add a compatibility constraint to de-singularize a singular matrix system.
// /trueOrFalse (input):   
//    If TRUE, add a compatibility equation to de-singularize a Neumann problem.
//    In this case Oges adds an extra equation to your system, and changes your
//    equations (see section ?? for more details). This option should be set
//    before you call {\ff initialize}.
//    Here is how to assign the  right hand side for this extra equation (can 
//    only be done after calling {\ff initialize})
//    {\footnotesize
//    \begin{verbatim}
//      Oges solver(...);
//      ...
//      solver.initialize();
//      ...
//      solver.equationToIndex( solver.extraEquationNumber(0),n,i1,i2,i3,grid );  // get n,i1,i2,i3,grid
//      f[grid](i1,i2,i3,n)=1.;     // assign the rhs for the compatibility equation
//      ..
//      solver.solve( u,f);
//    \end{verbatim}
//    }
//    The solution to the compatibility equation is returned at a ``unused'' grid point,
//    whose ``equation number'' is {\ff solver.EquationNumber(0)}.
//    Here is how to access the solution to the compatibility equation:
//    {\footnotesize
//    \begin{verbatim}
//      solver.equationToIndex( solver.extraEquationNumber(0),n,i1,i2,i3,grid );
//      cout << "solution to the compatibility equation is " << u[grid](i1,i2,i3,n) << endl;
//    \end{verbatim}
//    }
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  printObsoleteMessage("setCompatibilityConstraint");
  parameters.set(OgesParameters::THEcompatibilityConstraint,trueOrFalse);  

  // compatibilityConstraint=trueOrFalse;
  shouldBeInitialized=TRUE;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{setCompositeGrid}}
void Oges::
setCompositeGrid( CompositeGrid & cg0 )
//=====================================================================================
// /Purpose:
// Supply a CompositeGrid to Oges. Use this routine, for example,
// if an Oges object was created with the default constructor.
// Call this routine before calling initialize.
// /cg0 (input): Oges will keep a reference to this grid.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  cout << "***Oges::setCompositeGrid() : WARNING this routine will go away, use setGrid(cg) instead\n";
  cg.reference(cg0);
  privateUpdateToMatchGrid();
  shouldBeInitialized=TRUE;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{setConjugateGradientNumberOfIterations}}
void Oges::
setConjugateGradientNumberOfIterations( 
      const int conjugateGradientNumberOfIterations0)
//=====================================================================================
// /Purpose:
// Maximum number of iterations.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setConjugateGradientNumberOfIterations");
  parameters.set(OgesParameters::THEmaximumNumberOfIterations,conjugateGradientNumberOfIterations0);

  // conjugateGradientNumberOfIterations=conjugateGradientNumberOfIterations0;
} 

//\begin{>>OgesParametersInclude.tex}{\subsection{setConjugateGradientNumberOfSaveVectors}}
void Oges::
setConjugateGradientNumberOfSaveVectors( 
     const int conjugateGradientNumberOfSaveVectors0 )
//=====================================================================================
// /Purpose:
//   Specify the number of save vectors for conjugate gradient (default=20).
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setConjugateGradientNumberOfSaveVectors");
  parameters.set(OgesParameters::THEgmresRestartLength,conjugateGradientNumberOfSaveVectors0);
// conjugateGradientNumberOfSaveVectors=conjugateGradientNumberOfSaveVectors0;
 } 

//\begin{>>OgesParametersInclude.tex}{\subsection{setConjugateGradientType}}
void Oges:: 
setConjugateGradientType( const conjugateGradientTypes
conjugateGradientType0 )
//=====================================================================================
// /Purpose: There are various flavours of conjugate gradient routines
//  that can be called based on the values of {\ff conjugateGradientType}.
//  In general larger values of {\ff conjugateGradientType} correspond to
//  methods which converge with fewer iterations (but faster?).
//  
//  The possibles values for {\ff conjugateGradientType} are
//  {
//  \footnotesize
//  \begin{verbatim}
//    enum conjugateGradientTypes
//    {
//      biConjugateGradient=0,
//      biConjugateGradientSquared=1,
//      GMRes=2,
//      CGStab=3
//    };
//  
//  \end{verbatim}
//  }
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setConjugateGradientType");
  switch (conjugateGradientType0 )
  {
  case biConjugateGradient:
    parameters.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradient);
    break;
  case biConjugateGradientSquared:
    parameters.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientSquared);
    break;
  case GMRes:
    parameters.set(OgesParameters::THEsolverMethod,OgesParameters::gmres);
    break;
  case CGStab:
    parameters.set(OgesParameters::THEsolverMethod,OgesParameters::gmres);
    break;
  default:
    parameters.set(OgesParameters::THEsolverMethod,OgesParameters::gmres);
  }
  // conjugateGradientType=conjugateGradientType0;
 }

//\begin{>>OgesParametersInclude.tex}{\subsection{setConjugateGradientPreconditioner}}
void Oges:: 
setConjugateGradientPreconditioner( const conjugateGradientPreconditioners conjugateGradientPreconditioner0)
//=====================================================================================
// /Purpose: There are various flavours of conjugate gradient routines
//  that can be called based on the values of {\ff
//  conjugate\-Gradient\-Type} and {\ff
//  conjugate\-Gradient\-Preconditioner}.  In general larger values of
//  {\ff conjugate\-Gradient\-Type} and {\ff
//  conjugate\-Gradient\-Preconditioner}.  correspond to methods which
//  converge with fewer iterations (but faster?).  The possibles values
//  for {\ff conjugate\-Gradient\-Preconditioner} are
//  
//  {
//  \footnotesize
//  \begin{verbatim}
//  
//    enum conjugateGradientPreconditioners
//    {
//      none=0,
//      diagonal=1,      // Diagonal Scaling 
//      incompleteLU=2,  // Incomplete LU
//      SSOR=3,          // Symmetric SOR
//    };
//  \end{verbatim}
//  }
// NOTE: Only {\tt diagonal} and {incompleteLU} are currently available.   
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setConjugateGradientPreconditioner");

  if( conjugateGradientPreconditioner0!=diagonal && conjugateGradientPreconditioner0!=incompleteLU )
  {
    cout << "Oges::setConjugateGradientPreconditioner:ERROR: invalid preconditioner \n";
    cout << " only diagonal scaling and incomplete LU are available \n";
  }
  else  
  {
    // conjugateGradientPreconditioner=conjugateGradientPreconditioner0;

    switch (conjugateGradientPreconditioner0 )
    {
    case diagonal:
      parameters.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
      break;
    case incompleteLU:
      parameters.set(OgesParameters::THEsolverMethod,OgesParameters::incompleteLUPreconditioner);
      break;
    }
  }

}


//\begin{>>OgesParametersInclude.tex}{\subsection{setConjugateGradientTolerance}}
void Oges::
setConjugateGradientTolerance( const real conjugateGradientTolerance0 )
//=====================================================================================
// /Purpose: Specify the convergence criteria.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setConjugateGradientTolerance");
  parameters.set(OgesParameters::THEtolerance, conjugateGradientTolerance0);

  // conjugateGradientTolerance=conjugateGradientTolerance0; 
}

  
// // ============== this should be in the derived Class
// //\begin{>>OgesParametersInclude.tex}{\subsection{setEquationType}}
// void Oges::
// setEquationType( const equationTypes equationType0 )
// //=====================================================================================
// // /Purpose: Specify the type of equation that you would like to solve.
// // If you would like to supply the coefficients yourself then
// // choose {\ff userSuppliedArray}, and see {\ff setCoefficientArray}
// // /equationType0 (input): Choose a value from
// //  {
// //  \footnotesize
// //  \begin{verbatim}
// //    enum equationTypes
// //    {
// //      LaplaceDirichlet=0,
// //      LaplaceNeumann=1,
// //      LaplaceMixed=2,
// //      Nonlinear1=3,
// //      Eigenvalue=4,
// //      Biharmonic=5,
// //      userSuppliedArray=6,
// //      Interpolation=7
// //    };
// //  \end{verbatim}
// //  }
// //\end{PlotStuffInclude.tex} 
// //=====================================================================================
// {
//   printf("Oges:setEquationType:ERROR:using internal Oges equations is no longer supported. \n"
//          "     Use coefficient matrices and operators instead\n");

//   printObsoleteMessage("setEquationType");

//   equationType=equationType0; 
//   int compatibilityConstraint=FALSE;
//   int i;
//   switch (equationType)
//   {
//   case LaplaceNeumann:
//     compatibilityConstraint=TRUE;
//     break;
//   case Biharmonic:
//     setNumberOfComponents(2);
//     break;
//   case Interpolation:
//     // use all ghostlines for implicit interpolation
//     for( i=1; i<=maximumNumberOfGhostLines; i++ )
//       setGhostLineOption( i,useGhostLine );
//     break;    
//   }
//   parameters.set(OgesParameters::THEcompatibilityConstraint,compatibilityConstraint);
//   shouldBeInitialized=TRUE;
// }


// //\begin{>>OgesParametersInclude.tex}{\subsection{setGhostLineOption}}
// void Oges::
// setGhostLineOption( const int ghostLine, const ghostLineOptions option, 
//              const int n, const int grid, const int side, const int axis )
// //=====================================================================================
// // /Purpose:
// //  Specify whether a 
// //  ghost line should be used 
// //  for discretization or should be extrapolated. This option must be specified
// //  before the call to {\ff initialize}.
// //  By default ghost line values are extrapolated.
// //  There are three choices when using the ghostline. Either
// //  use the entire ghostline, or use the ghostline but extrapolate
// //  the corner or use the ghostline but extrapolate the corner
// //  and the neighbours to the corner. These choices are described
// //  in more detail in section \ref{secGhostLineOptions}.
// //   Ghost line options are specified for
// //  each {\ff ghostLine=1,2,...}, and each component {\ff n=0},
// //  on each {\ff side=0,1}
// //  and {\ff axis=0,1,2} of each grid.
// //  {\footnotesize
// //  \begin{verbatim}
// //    void setGhostLineOption( const int ghostLine, const ghostLineOptions option, 
// //            const int n=-1, const int grid=-1, const int side=-1, const int axis=-1 )
// //  
// //    enum ghostLineOptions  // enum found in Oges
// //    {
// //      extrapolateGhostLine,                   // default
// //      useGhostLine,
// //      useGhostLineExceptCorner,               // i.e. extrapolate the corner value
// //      useGhostLineExceptCornerAndNeighbours
// //    };
// //  \end{verbatim}
// //  }
// //  If you omit the arguments {\ff n,grid,side,axis} then the option will
// //  apply to all components of all sides of all grids. 
// //  For example:
// //  {\footnotesize
// //  \begin{verbatim}
// //    Oges solver( ... );
// //    ...
// //    solver.setGhostLineOption( 1, Oges::useGhostLine );  // use first ghostline for discretization
// //    ...                                                
// //    solver.initialize();
// //    ...
// //  \end{verbatim}
// //  }
// //\end{OgesParametersInclude.tex}
// //=====================================================================================
// {
//   if( numberOfGrids < 1 )
//   {
//     cout << "Oges:setGhostLineOption:error you must supply a grid before setting"
//           " ghost line options \n";
//     cout << "Supply a grid with the constructor or use setGrid \n";
//     if( this )
//       throw "Oges:setGhostLineOption:error";
//     return;
//   }
//   if( ghostLine<1 || ghostLine > maximumNumberOfGhostLines )
//   {
//     cout << "Oges:setGhostLineOption:error invalid ghostLine= " << ghostLine << endl;
//     if( this )
//       throw "Oges:setGhostLineOption:error";
//     return;
//   }
//   if( option < 0 || option > 3 )
//   {
//     cout << "Oges:setGhostLineOption:error invalid option= " << option << endl;
//     if( this )
//       throw "Oges:setGhostLineOption:error";
//     return;
//   }
//   int nStart= n==-1 ? 0 : n;
//   int nEnd  = n==-1 ? numberOfComponents-1 : n;
//   int gridStart= grid==-1 ? 0 : grid;
//   int gridEnd  = grid==-1 ? numberOfGrids-1 : grid;
//   int sideStart= side==-1 ? Start : side;
//   int sideEnd  = side==-1 ? End   : side;
//   int axisStart= axis==-1 ? axis1 : axis;
//   int axisEnd  = axis==-1 ? numberOfDimensions-1 : axis;
  
//   for( int n0=nStart; n0<=nEnd; n0++ )
//   for( int grid0=gridStart; grid0<=gridEnd; grid0++ )
//   for( int side0=sideStart; side0<=sideEnd; side0++ )
//   for( int axis0=axisStart; axis0<=axisEnd; axis0++ )
//   {
//     ghostLineOption[grid0](side0,axis0,ghostLine,n0)=option;
//   }
//   initialized=FALSE;    // we need to initialize again
//   shouldBeInitialized=TRUE;
// }


//\begin{>>OgesParametersInclude.tex}{\subsection{setHarwellTolerance}}
void Oges::
setHarwellTolerance( const real harwellTolerance0) // tolerance for harwell pivoting
//=====================================================================================
// /Purpose:
// Tolerance for Harwell that determines the level of threshold
// pivotting. A value of zero corresponds to no pivoting and
// a value of $1.$ corresponds to partial pivotting.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setHarwellTolerance");
  Overture::abort("error");
}

    
//\begin{>>OgesParametersInclude.tex}{\subsection{setIterativeImprovement}}
void Oges::
setIterativeImprovement( const int iterativeImprovement0 )
//=====================================================================================
// /Purpose: Set to TRUE if you want to perform iterative improvement
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setIterativeImprovement");
  Overture::abort("error");
//  iterativeImprovement=iterativeImprovement0;
}  

//\begin{>>OgesParametersInclude.tex}{\subsection{setNullVectorScaling}}
void Oges::
setNullVectorScaling(const real & scale )
//=====================================================================================
// /Purpose: Set the scale for the null vector (default is 1)
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  printObsoleteMessage("setNullVectorScaling");
  Overture::abort("error");
//  nullVectorScaling=scale;
//  shouldBeInitialized=TRUE;
}



//\begin{>>OgesParametersInclude.tex}{\subsection{setNumberOfComponents}}
void Oges::
setNumberOfComponents( const int numberOfComponents0 )
//=====================================================================================
// /Purpose:
// This defines the number of vector components
// in the problem. For example, {\ff numberOfComponents==3} for
// the 2D incompressible Navier-Stokes
// equations which consists of three equations for $\uv=[u,v,p]^T$.
// /numberOfComponents0 (input): number of components in the matrix system.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printf("Oges::setNumberOfComponents:WARNING: This function is now obsolete. No need to call\n");
//  numberOfComponents=numberOfComponents0;
//  shouldBeInitialized=TRUE;
}

// //\begin{>>OgesParametersInclude.tex}{\subsection{setNumberOfExtraEquations}}
// void Oges:: 
// setNumberOfExtraEquations( const int numberOfExtraEquations0 )
// //=====================================================================================
// // /Purpose: Set the number of extra equations. These are extra
// //  unknowns in your problem that do not correspond to a solution value at
// //  a grid point.  For example, this could be the eigenvalue in an
// //  eigenvalue problem, or the extra unknown that is sometimes added to
// //  the Neumann problem for the Laplace opertor.  After calling {\ff
// //  setNumberOfExtraEquations(m)} the array {\ff extraEquation(i),
// //  i=0,...,m-1} will be assigned. These are the equation numbers for the
// //  extra equations.
// //\end{OgesParametersInclude.tex}
// //=====================================================================================
// { 
//   // Define the number of extra Equations
//   // Allocate space to save coefficients in **** free this space  ****
//   numberOfExtraEquations=numberOfExtraEquations0; 
//   shouldBeInitialized=TRUE;

// }

// //\begin{>>OgesParametersInclude.tex}{\subsection{setNumberOfGhostLines}}
// void Oges::
// setNumberOfGhostLines( const int numberOfGhostLines0 )
// //=====================================================================================
// // /Purpose:
// // Specify the number of ghost lines to be used in the computation.
// // See also {\ff ghostLineOptions}.
// // /numberOfGhostLines0 (input): non-negative integer.
// //\end{OgesParametersInclude.tex}
// //=====================================================================================
// { if( numberOfGhostLines0 > maximumNumberOfGhostLines )
//   {
//     cout << "Oges:setNumberOfGhostLines: numberOfGhostLines is too large" << endl;
//   }
//   numberOfGhostLines=min(numberOfGhostLines0,maximumNumberOfGhostLines); 
//   shouldBeInitialized=TRUE;
// }


//\begin{>>OgesParametersInclude.tex}{\subsection{setMatrixCutoff}}
void Oges::
setMatrixCutoff( const real matrixCutoff0 )
//=====================================================================================
// /Purpose:
//  This is a cutoff parameter that eliminates small
//  entries in the sparse matrix. For each row, any entry that is smaller
//  than {\ff matrixCutoff} times the largest entry in the row is thrown away:
//  $$
//     \mbox{Throw away element $a(i,j)$ if }
//           | a(i,j) |  < {\ff matrixCutoff}~\max_{j} | a(i,j) |    ~.
//  $$
// /matrixCutoff0 (input): A non-negative real value.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setMatrixCutoff");
  Overture::abort("error");
//  matrixCutoff=matrixCutoff0;   
//  shouldBeInitialized=TRUE;
}




//\begin{>>OgesParametersInclude.tex}{\subsection{setOrderOfAccuracy}}
void Oges::
setOrderOfAccuracy( const int orderOfAccuracy0 )
//=====================================================================================
// /Purpose:
//  Define the order of accuracy. This will define the order
//  of accuracy of the discretization of the predefined problems
//  and will determine the order of extrapolation used at
//  extrapolation points.
//  By setting this parameter to 2 or to 4 you can solve a predefined
//  problem to
//  second or fourth-order accuracy.  For fourth order accuracy you will
//  need a grid that has been created, using CMPGRD, with {\ff dw=5}
//  (Discretization Width), {\ff iw=5} (Interpolation Width) and {\ff
//  nxtra=2} (2 lines of fictitious points).  For second order accuracy
//  you will only need to set {\ff nxtra=1} in CMPGRD as the defaults for
//  {\ff dw (=3)} and {\ff iw (=3)} are fine.
// /orderOfAccuracy0 (input): a value of 2 or 4.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  printf("Oges::setOrderOfAccuracy:WARNING: This function is now obsolete. No need to call\n");
/* ---  
  if( orderOfAccuracy0 !=2 && orderOfAccuracy0 !=4 )
  {
    cout << " Oges:setOrderOfAccuracy: invalid order of accuracy = " 
      << orderOfAccuracy0 << endl;
  }
  else
  {
    orderOfAccuracy=orderOfAccuracy0;
    shouldBeInitialized=TRUE;
  }
----- */
}

//\begin{>>OgesParametersInclude.tex}{\subsection{setPreconditionBoundary}}
void Oges::
setPreconditionBoundary( const int preconditionBoundary0 )
//=====================================================================================
// /Purpose:
//  Set this parameter to TRUE
//  in order to precondition the
//  equations at the boundary. Use this option when there are Neumann
//  boundary conditions or a mixture of Neumann and Dirichlet boundary
//  conditions to prevent small or zero pivots in many solvers. When
//  {\ff preconditionBoundary==TRUE} Oges will, for each boundary point,
//   take the equations for the boundary point
//  and its corresponding fictitious point(s) and solve these equations
//  for the unknowns at the
//  boundary point and fictitious point(s) in terms of the unknowns at the
//  other points.
// /preconditionBoundary0 (input): TRUE or FALSE value
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  preconditionBoundary=preconditionBoundary0;
  shouldBeInitialized=TRUE;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{setPreconditionRightHandSide}}
void Oges::
setPreconditionRightHandSide( const int preconditionRightHandSide0 )
//=====================================================================================
// /Purpose: ??
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  preconditionRightHandSide=preconditionRightHandSide0;
  shouldBeInitialized=TRUE;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{setRefactor}}
void Oges::
setRefactor( const int refactor0 )
//=====================================================================================
// /Purpose:
// Refactor the matrix.
// Set this option if the matrix coefficients have changed.  See the
// example in section \ref{sec:refactor} for an example of how to
// refactor a matrix.
// /refactor0 (input): TRUE or FALSE value 
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  refactor=refactor0; 
// ***   if( refactor )
// ***    shouldUpdateMatrix=TRUE; 
}

//\begin{>>OgesParametersInclude.tex}{\subsection{setReorder}}
void Oges::
setReorder( const int reorder0 )
//=====================================================================================
// /Purpose: 
// Reorder the matrix to reduce
// fillin. Use this option when you re-factor the matrix. Not all solvers
// will re-order.
// /reorder0 (input):  TRUE or FALSE value.
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  reorder=reorder0; 
}

void Oges::
setSolverJob( const int solverJob0 )
//=====================================================================================
//=====================================================================================
{ solverJob=solverJob0; }

// //\begin{>>OgesParametersInclude.tex}{\subsection{setTranspose}}
// void Oges::
// setTranspose( const int transpose0 )
// //=====================================================================================
// // /Purpose:
// // Solve the the system with the matrix replaced by its transpose.
// // /transpose0 (input):  TRUE or FALSE value.
// //\end{OgesParametersInclude.tex}
// //=====================================================================================
// { transpose=transpose0; }



//\begin{>>OgesParametersInclude.tex}{\subsection{setSolverType}}
void Oges::
setSolverType( const solvers solverType0 )
//=====================================================================================
// /Purpose: Specify which sparse matrix solver to use.
//
// /solverType0 (input): A value from:
//  {\footnotesize
//  \begin{verbatim}
//    enum solvers
//    {
//      yale=1,
//      harwell=2,
//      bcg=3,      /* for conjugate Gradient, GMRes */
//      sor=4
//    };
//  \end{verbatim} 
//  }
//
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setSolverType");

  switch (solverType0)
  {
  case yale:
    set(OgesParameters::THEsolverType,OgesParameters::yale);
    break;
  case harwell:
    set(OgesParameters::THEsolverType,OgesParameters::harwell);
    break;
  case bcg:
  case SLAP:
    set(OgesParameters::THEsolverType,OgesParameters::SLAP);
    break;
  case PETSc:
    set(OgesParameters::THEsolverType,OgesParameters::PETSc);
    break;
  default:
    set(OgesParameters::THEsolverType,OgesParameters::SLAP);
  }

  shouldBeInitialized=TRUE;
}

// void Oges::
// setSparseFormat( const int sparseFormat0 )
// //=====================================================================================
// // /Purpose: ?
// //=====================================================================================
// { 
// sparseFormat=sparseFormat0;
//  }


void Oges::
setZeroRatio( const real zeroRatio0 )
//=====================================================================================
// /Purpose: Is this needed anymore?
//=====================================================================================
{ 
  printObsoleteMessage("setZeroRatio");
  Overture::abort("error");
}


void Oges::
setFillinRatio( const real fillinRatio0 )
//=====================================================================================
// /Purpose: Is this needed anymore?
//=====================================================================================
{ 
  printObsoleteMessage("setFillinRatio");
  Overture::abort("error");
}


void Oges::
setFillinRatio2( const real fillinRatio20 )
//=====================================================================================
// /Purpose: Is this needed anymore?
//=====================================================================================
{ 
  printObsoleteMessage("setFillinRatio2");
  Overture::abort("error");
}

// void Oges::
// setFixupRightHandSide( const bool trueOrFalse )
// //=====================================================================================
// // /Purpose: ?
// //\end{OgesParametersInclude.tex}
// //=====================================================================================
// {
//   fixupRightHandSide=trueOrFalse;
// }

//\begin{>>OgesParametersInclude.tex}{\subsection{setSorNumberOfIterations}}
void Oges::
setSorNumberOfIterations( const int sorNumberOfIterations0 )
//=====================================================================================
// /Purpose: Specify the number of iterations for SOR
//\end{OgesParametersInclude.tex}
//=====================================================================================
{   
  printObsoleteMessage("setSorNumberOfIterations");
  Overture::abort("error");
}


//\begin{>>OgesParametersInclude.tex}{\subsection{setSorTolerance}}
void Oges::
setSorTolerance( const real sorTolerance0 )
//=====================================================================================
// /Purpose: Set tolerance for SOR
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setSorTolerance");
  Overture::abort("error");
}

//\begin{>>OgesParametersInclude.tex}{\subsection{setSorOmega}}
void Oges::
setSorOmega( const real sorOmega0 )
//=====================================================================================
// /Purpose: Set relaxation parameter for SOR
//\end{OgesParametersInclude.tex}
//=====================================================================================
{ 
  printObsoleteMessage("setSorOmega");
  Overture::abort("error");
}

  

/* ----
//=====================================================================
//   Initialization routine for oges
//
//=====================================================================
void Oges::
initialize2( int & errorNumber )
{
  
  errorNumber=0;
  
  if( parameters.solver==OgesParameters::yale )
  {
    sparseFormat=0;  // remove set for this variable
  }
  else if( parameters.solver==OgesParameters::harwell )
  {
    //    Harwell
    sparseFormat=1;        // uncompressed ia()
    
  }
  else if( parameters.solver==OgesParameters::SLAP )
  {
    
    //         Conjugate Gradient
    //       ...SLAP:
    //         icg : = 0 : SLAP: bi-conjugate gradient
    //               = 1 : SLAP: bi-conjugate gradient squared
    //               = 2 : SLAP: GMRES
    //         ipc : =0 : diagonal scaled preconditioner
    //               =1 : incomplete LU preconditioner
    //       ...ESSL:
    //         icg =11 : conjugate gradient
    //             =12 : bi-conjugate gradient squared
    //             =13 : GMRES
    //             =14 : CGSTAB, smoothly converging CGS
    //         ipc =11 : no preconditioning
    //             =12 : Diagonal
    //             =13 : SSOR
    //             =14 : ILU

    // if( conjugateGradientType <= 10 )
    sparseFormat=1; // SLAP stores uncompressed ia()

    //  conjugateGradientNumberOfIterations=0; these should be defaults
    //  conjugateGradientTolerance=0.;

  }
  else if( parameters.solver==OgesParameters::sor )
  {
    //        SOR
  }
  
  //    ...default for fratio depends on 2nd or 4th order
  //       and number of space dimensions

  const int stencilSize = coeff[0].sparse->stencilSize;

  if( parameters.fillinRatio<=0. )
    parameters.fillinRatio=stencilSize+10;  // +5; *wdh* 971024
  if( parameters.compatibilityConstraint && numberOfDimensions==3 )
    parameters.fillinRatio+=5;

  //   ...fratio2 is another fill-in ratio for Harwell:
  if( parameters.fillinRatio2<=0. )
    parameters.fillinRatio2=2.;


  // --- count the number of equations
  numberOfEquations=0;
  int grid;
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    numberOfEquations+=arraySize(grid,axis1)*arraySize(grid,axis2)*arraySize(grid,axis3);
  }
  
  //c     ...allocate space for the boundary preconditioner variables

  bpcNumberOfEquations=0;
  bpcNumberOfNonzeros=0;

  bpciep=1;
  bpciap=1;
  bpcjap=1;
  bpcap=1;
  bpcrhsp=1;

  //c       Number of equations:
  numberOfEquations=numberOfEquations*numberOfComponents;

  // .. get epslon=r1mach(4) = largest relative spacing
  //    IBM Single Prec.: epslon=.95367E-06
  machineEpsilon=R1MACH(4);
  realToIntegerRatio=1;

  //    The following fudge tries to detect whether the code has been
  //    compiled with an automatic double precision option in which
  //    case we want to call D1MACH
  //     IBM Double Prec.: epslon=.222E-15

  real oneps=1.+machineEpsilon/1000.;
  if( oneps != 1. )
  {
    machineEpsilon=D1MACH(4);
    realToIntegerRatio=2;
  }

  //  ...parameter for throwing away small matrix elements:
  if( parameters.matrixCutoff <= 0. )
    parameters.matrixCutoff=machineEpsilon;

  //   storage format
  //         = 0 : allocate local space to store solution for sparse solvers
  //         = 1 : no need to allocate local storage
  storageFormat=0;
  if( parameters.solver == OgesParameters::SLAP )
  {
    // storage for u and f will be in compact form
    storageFormat=0;   // ***** used to be 1 *****
    if( storageFormat==1 && numberOfComponents > 1 )
    {
      cerr << "oges:initialize2:ERROR: Compact storage but numberOfComponents>1 " << endl;
      cerr << "This option not implemented." << endl;
      cerr << "Cannot use conjugate Gradient with numberOfComponents > 1 " << endl;
      exit(1);
    }
  }
  
}
----- */
