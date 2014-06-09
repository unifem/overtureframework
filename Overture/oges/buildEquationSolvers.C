//
// Here we create EquationSolvers that Oges can use
// 
// We can always build
//    o yale
//    o harwell
//    o slap
// We can optionally build by compiling with the flag -DOVERTURE_USE_PETSC
//    o PETSc
// #ifdef OVERTURE_USE_PETSC
// #include "PETScEquationSolver.h"
// #include "PETScSolver.h"
// #endif
#include "YaleEquationSolver.h"
#include "HarwellEquationSolver.h"
#include "SlapEquationSolver.h"
#include "MultigridEquationSolver.h"



//\begin{>>OgesParametersInclude.tex}{\subsection{isAvailable(SolverEnum) }} 
int OgesParameters::
isAvailable( SolverEnum solverType )
//==================================================================================
// /Description:
//   Return TRUE if a given solver (esp. PETSc) is available.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  if( solverType==yale || solverType==harwell || solverType==SLAP || solverType==multigrid )
  {
    return 1;
  }
  else if( solverType==PETSc ) // we need to figure out a way to see if PETSc is available
  {
    return Oges::petscIsAvailable;
  }
  else if( solverType==PETScNew )
  {
    // parallel version of PETSc solver: 
   #ifdef USE_PPP
    return Oges::petscIsAvailable;
   #else
    return 0;
   #endif
  }
  else
  {
    printf("OgesParameters::isAvailable: ERROR unknown solverType.\n");
  }
  return TRUE;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{isSolverIterative}} 
bool Oges::
isSolverIterative() const
//=====================================================================================
// /Description:
//   Return TRUE if the solver chosen is an iterative method
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  return parameters.solver==OgesParameters::SLAP || 
         parameters.solver==OgesParameters::PETSc || 
         parameters.solver==OgesParameters::PETScNew ||
         parameters.solver==OgesParameters::multigrid;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{buildEquationSolvers}} 
int Oges::
buildEquationSolvers(OgesParameters::SolverEnum solver)
//=====================================================================================
// /Description:
//    This function will build an equation solver of a particular type.
//  This function is found in the {\tt Oges/buildEquationSolvers.C} file. It is this
// file that you may have to copy and edit in order to turn on the availability solvers
// that are not distributed with Overture (such as PETSc).
//\end{OgesParametersInclude.tex}
//=====================================================================================
{
  if( !parameters.isAvailable(solver) )
  {
    printf("Oges::buildEquationSolvers:ERROR: solver %s is not currently available\n"
           " You may have to copy and edit the file Overture/Oges/buildEquationSolvers.C\n"
           " and a file like PETScEquationSolver.C (if you are trying to use PETSc)\n"
           " and then link the files to your application in order to get a non-standard solver\n"
           " See the Oges documentation for further details\n",
           (const char*)parameters.getSolverTypeName(solver));
    OV_ABORT("error");
  }
  
  if( solver==OgesParameters::yale )
  {
    const int yaleES = OgesParameters::yale;
    if( equationSolver[yaleES]==NULL )
      equationSolver[yaleES]=new YaleEquationSolver(*this);
  }
  else if( solver==OgesParameters::harwell )
  {
    const int harwellES = OgesParameters::harwell;
    if( equationSolver[harwellES]==NULL )
      equationSolver[harwellES]=new HarwellEquationSolver(*this);
  }
  else if( solver==OgesParameters::SLAP )
  {
    const int slapES = OgesParameters::SLAP;
    if( equationSolver[slapES]==NULL )
      equationSolver[slapES]=new SlapEquationSolver(*this);
  }
  else if( solver==OgesParameters::PETSc )
  {
    const int petsc = OgesParameters::PETSc;
    // Overture::shutDownPETSc = &finalizePETSc;  // set the function that will shut down PETSc

    assert( createPETSc!=NULL );

    if( equationSolver[petsc]==NULL )
      equationSolver[petsc]=(*createPETSc)(*this);
    

    // if( equationSolver[petsc]==NULL )
    //   equationSolver[petsc]=new PETScEquationSolver(*this);

  }
  #ifdef USE_PPP
  else if( solver==OgesParameters::PETScNew )
  {
    // Overture::shutDownPETSc = &finalizePETSc;  // set the function that will shut down PETSc

    const int petsc = OgesParameters::PETScNew;

    if( equationSolver[petsc]==NULL )
      equationSolver[petsc]=(*createPETSc)();

    // if( equationSolver[petsc]==NULL )
    //   equationSolver[petsc]=new PETScSolver(*this);

  }
  #endif
  else if( solver==OgesParameters::multigrid )
  {
    const int multigridES = OgesParameters::multigrid;
    if( equationSolver[multigridES]==NULL )
    {
      equationSolver[multigridES]=new MultigridEquationSolver(*this);
      equationSolver[multigridES]->set( mgcg );  // supply the MultigridCompositeGrid to use (holds multigrid hierarchy and can be shared)
    }
  }
  else
  {
    printF("Oges::buildEquationSolvers:ERROR:Unknown solver %s to build\n",
         (const char*)parameters.getSolverTypeName(solver));
    OV_ABORT("error");
  }

  return 0;
}


