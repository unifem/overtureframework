#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "ProjectVelocity.h"
#include "Ogshow.h"
#include "Ogen.h"
#include "Ogmg.h"
#include "FileOutput.h"
// #include "turbulenceModels.h"
// #include "Insbc4WorkSpace.h"
#include "LineSolve.h"
#include "ParallelUtility.h"

ProjectVelocity projector;  // ******************* fix this *******************

void DomainSolver::
smoothVelocity(GridFunction & cgf,
               const int numberOfSmooths )
// ============================================================================================
// /Description:
//  Smooth the velocity
// ============================================================================================
{
  FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");

  realCompositeGridFunction & u = cgf.u;
  CompositeGrid & cg = *u.getCompositeGrid();
  
  const int orderOfAccuracy = min(4,parameters.dbase.get<int >("orderOfAccuracy")); //kkc 101115 add min

  BoundaryConditionParameters bcParams;
  bcParams.orderOfExtrapolation=2;  // *wdh* 100816
  // *wdh* 100816 bcParams.ghostLineToAssign= orderOfAccuracy/2;  // *note*

  if( Parameters::checkForFloatingPointErrors )
    checkSolution(cgf.u,"DomainSolver::smoothVelocity:start");

  //  ---Use a Jacobi smoother, under-relaxed
  real omega0=.9;
  real omo=1.-omega0, ob4=omega0/4., ob6=omega0/6.;
  
  Index I1,I2,I3;
  int extra[3] = { 0,0,0 };  
  Index N(parameters.dbase.get<int >("uc"),cg.numberOfDimensions());
  for( int it=0; it<numberOfSmooths; it++ )
  {
    if( debug() & 4 )
    {
      printF(" smoothVelocity>>> iteration=%i\n",it);
      fPrintF(debugFile," smoothVelocity>>> iteration=%i\n",it);
    }
    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      realArray & v = u[grid];
//      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
//        extra[axis] = cg[grid].isPeriodic()(axis) ? 0 : -1;
      // *wdh* getIndex(extendedGridIndexRange(cg[grid]),I1,I2,I3,extra[0],extra[1],extra[2]);  // *wdh* 030313
      getIndex(cg[grid].gridIndexRange(),I1,I2,I3,extra[0],extra[1],extra[2]); 
      
      #ifdef USE_PPP
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
      #else
        realSerialArray & vLocal = v;
      #endif    

      bool ok = ParallelUtility::getLocalArrayBounds(v,vLocal,I1,I2,I3);
      if( ok )
      {
	if( cg.numberOfDimensions()==2 || parameters.dbase.get<int >("compare3Dto2D") )
	{
	  // where( cg[grid).mask()(I1,I2,I3) >0 ) ** add this ?
	  vLocal(I1,I2,I3,N)=omo*vLocal(I1,I2,I3,N)
	    +ob4*( vLocal(I1+1,I2,I3,N)+vLocal(I1-1,I2,I3,N)
		   +vLocal(I1,I2+1,I3,N)+vLocal(I1,I2-1,I3,N));
	}
	else
	{
	  vLocal(I1,I2,I3,N)=omo*vLocal(I1,I2,I3,N)
	    +ob6*( vLocal(I1+1,I2,I3,N)+vLocal(I1-1,I2,I3,N)
		   +vLocal(I1,I2+1,I3,N)+vLocal(I1,I2-1,I3,N)
		   +vLocal(I1,I2,I3+1,N)+vLocal(I1,I2,I3-1,N) );
	}
      }
    }

    if( Parameters::checkForFloatingPointErrors )
      checkSolution(cgf.u,sPrintF("smoothVelocity: before applyBC it=%i",it));

    // assign bc's 
    applyBoundaryConditions(cgf); 

    if( orderOfAccuracy==4 ) // trouble at outflow with standard BC's
    {
      // On boundaries that set div(u)=0 it is better to extrapolate the ghost points instead, since
      // we are looking for regions where div(u) is large
      if( false ) // *wdh* 2013/09/25 +++++ switch back for AFS problems ++++++
      { // *wdh* 2012/09/14
        const int nbc=2;  // noSlipWall and outflow
	for( int ibc=0; ibc<nbc; ibc++ )
	{
          const int outflow=5;  // fix me -- this is from InsParameters
          int bcToExtrap = ibc==0 ? Parameters::noSlipWall : outflow;
	  u.applyBoundaryCondition(N,BCTypes::extrapolate,bcToExtrap,0.,0.,bcParams);
	  bcParams.ghostLineToAssign=2;
	  u.applyBoundaryCondition(N,BCTypes::extrapolate,bcToExtrap,0.,0.,bcParams); 
	  bcParams.ghostLineToAssign=1;
	}
	
      }
      else
      {
	u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,bcParams);
	bcParams.ghostLineToAssign=2;
	u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,bcParams); // **** 990106
	bcParams.ghostLineToAssign=1;
      }
      
      u.finishBoundaryConditions();
    }

    if( Parameters::checkForFloatingPointErrors )
      checkSolution(cgf.u,sPrintF("smoothVelocity: after applyBC it=%i",it));

    // *wdh* this extrap condition causes trouble at slipWall/outflow corners 000124
    // *** undo the div(u)=0 BC so the projection will work better on the boundary.
    // *wdh*  u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries); // **** 990106

    if( debug() & 32 )
      cgf.u.display("smooth velocity solution after sub-smooth, before interpolate",debugFile,"%9.6f ");
    // interpolate first, needed for extrapolate-neighbours
    interpolate(cgf,N);
    if( debug() & 32 )
      cgf.u.display("smooth velocity solution after sub-smooth, after interpolate",debugFile,"%9.6f ");

    // assign bc's to get ghost point values
    if( orderOfAccuracy==2 )
      applyBoundaryConditions(cgf);

    // *wdh* this extrap condition causes trouble at slipWall/outflow corners 000124
    // *** undo the div(u)=0 BC so the projection will work better on the boundary.
    // *wdh* u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries); // **** 990106

    if( debug() & 4 || debug() & 32 )
      cgf.u.display("smooth velocity solution after sub-smooth, after applyBC",debugFile,"%6.3f ");
  }  
}



//\begin{>>CompositeGridSolverInclude.tex}{\subsection{project}} 
int DomainSolver::
project(GridFunction & cgf)
// ===================================================================================
// /Description:
//   Project the solution to be divergence free (approximately)
// The projection uses the Oges object "poisson" from this class -- it thus will
// use the same parameter values set in cg. The projection Poisson equation uses
// Neumann BC's all around so that it may have different BC's from the NS equations.
//\end{CompositeGridSolverInclude.tex}  
// ===================================================================================
{
  real time0=getCPU();
    
  if( parameters.dbase.get<bool >("projectInitialConditions") )
  {
    printF(">>>>>DomainSolver::project: project the initial conditions <<<<\n");
    // *** old way ****
    // *** parameters.dbase.get<bool >("projectInitialConditions")=FALSE;
    
    if( debug() & 16 ) 
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," \n ****Solution before projectVelocity**** \n");
      outputSolution( cgf.u,0. );
    }
    // project the velocity
    int numberOfSmoothsPerProjection=5;
    projector.setNumberOfSmoothsPerProjectionIteration(numberOfSmoothsPerProjection);
    projector.setVelocityComponent(parameters.dbase.get<int >("uc"));
    projector.setPoissonSolver(poisson);
    projector.setCompare3Dto2D(parameters.dbase.get<int >("compare3Dto2D"));
    projector.setIsAxisymmetric(parameters.isAxisymmetric());

    projector.projectVelocity(cgf.u,*(cgf.u.getOperators()));
    if( debug() & 16 ) 
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," \n ****Solution after projectVelocity**** \n");
      outputSolution( cgf.u,0. );
    }


    // assign bc's to get ghost point values
    applyBoundaryConditions(cgf);

    // build pressure equation
    updateToMatchGrid(cgf.cg); 
  }
  else
  {
    updateToMatchGrid(cgf.cg); 
  }
    
  // *wdh* 020512 : should only fixup before interpolate and applyBC
  //     printf(">>>>>>> fixup unused points after projection...\n");
  //     fixupUnusedPoints(cgf.u);

  if( debug() & 16 ) 
  {
    fprintf(parameters.dbase.get<FILE* >("debugFile")," \n ****Solution after projectVelocity and BC's**** \n");
    outputSolution( cgf.u,0. );
  }
  
  real time=getCPU()-time0;
  printF(">>>>>Time to project = %8.2e s <<<<\n",time);
  return 0;
}

