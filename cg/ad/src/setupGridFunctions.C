#include "Cgad.h"
#include "CompositeGridOperators.h"
#include "Ogshow.h"
#include "Ogen.h"
#include "Ogmg.h"
#include "Insbc4WorkSpace.h"
#include "App.h"
// #include "turbulenceModels.h"


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{initialize}} 
int Cgad::
setupGridFunctions()
//=========================================================================================
// /Description:
//    initialize a CompositeGridSolver.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  assert( current==0 );
  GridFunction & solution = gf[current];

  CompositeGrid & cg = *solution.u.getCompositeGrid();
  cg.update(MappedGrid::THEcenter | MappedGrid::THEinverseVertexDerivative );


  const Parameters::TimeSteppingMethod & timeSteppingMethod= 
                         parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  
  if( timeSteppingMethod==Parameters::implicit  )
  {
    printF("\n ******* Cgad:INFO: Building an Oges object for implicit time-stepping *******\n\n");
    
    int numberOfImplicitSolversNeeded=1;
    
    delete [] implicitSolver; 
    implicitSolver=NULL;
    numberOfImplicitSolvers=numberOfImplicitSolversNeeded;
    if( numberOfImplicitSolvers>0 )
      implicitSolver= new Oges [numberOfImplicitSolvers];
  }
  
  return DomainSolver::setupGridFunctions();
}


