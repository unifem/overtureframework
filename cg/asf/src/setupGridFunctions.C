#include "Cgasf.h"
#include "CompositeGridOperators.h"
#include "Ogshow.h"
#include "Ogen.h"
#include "Ogmg.h"
#include "AsfParameters.h"
#include "App.h"
#include "GridStatistics.h"


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{initialize}} 
int Cgasf::
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

  cg.update(MappedGrid::THEcenter | MappedGrid::THEvertex | 
	    MappedGrid::THEinverseVertexDerivative ); // **** fix this for rectangular ****
  
  if( pp==NULL )
    pp=new realCompositeGridFunction;
  if( ppx==NULL )
    ppx=new realCompositeGridFunction;

  Range all;
  CompositeGridOperators & operators = *gf[current].u.getOperators();

  p().updateToMatchGrid(cg,all,all,all); p()=0.;
  p().setOperators(operators);
  pressureRightHandSide.updateToMatchGrid(cg,all,all,all); pressureRightHandSide=0.;
  pressureRightHandSide.setOperators(operators);
  px().updateToMatchGrid(cg,all,all,all);
  px().setOperators(operators);
  int stencilSize=int( pow(3,cg.numberOfDimensions())+1 ); 
  coeff.updateToMatchGrid(cg,stencilSize,all,all,all);  // add one for interpolation
  
  coeff.setOperators(operators); 
  
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
  operators.setStencilSize(stencilSize);
  
  if( implicitSolver==NULL )
    {
      numberOfImplicitSolvers=1;
      implicitSolver= new Oges [numberOfImplicitSolvers];
    }
  implicitSolver[0].setGrid(cg);
  implicitSolver[0].setOrderOfAccuracy(2);
  implicitSolver[0].setCoefficientArray(coeff);
  
  if( parameters.dbase.get<AsfParameters::TestProblems >("testProblem")!=AsfParameters::standard )
    assignTestProblem(gf[current]);   // ************************ is this ok ? *****

  // --- check for negative volumes : this is usually bad news --- *wdh* 2013/09/26
  const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  const int numberOfGhost = orderOfAccuracyInSpace/2;
  int numberOfNegativeVolumes= GridStatistics::checkForNegativeVolumes( cg,numberOfGhost,stdout ); 
  if( numberOfNegativeVolumes>0 )
  {
    printF("Cgasf::FATAL Error: this grid has negative volumes (maybe only in ghost points).\n"
           "  This will normally cause severe or subtle errors. Please remake the grid.\n");
    OV_ABORT("ERROR");
  }
  else
  {
    printF("Cgasf:: No negative volumes were found\n.");
  }

  return DomainSolver::setupGridFunctions();
}



