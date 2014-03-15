#include "Cgcns.h"
#include "CompositeGridOperators.h"
#include "Ogshow.h"
#include "Ogen.h"
#include "Ogmg.h"
#include "Insbc4WorkSpace.h"
#include "App.h"
#include "CnsParameters.h"
#include "GridStatistics.h"



// ===================================================================================================================
/// \brief Perform initialization steps for Cgcns; build geometry arrays etc.
// ===================================================================================================================
int Cgcns::
setupGridFunctions()
{
  assert( current==0 );
  GridFunction & solution = gf[current];
  CompositeGrid & cg = *solution.u.getCompositeGrid();
  if( parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeWithArtificialDissipation )
    {
      cg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEinverseCenterDerivative | 
                MappedGrid::THEcenterJacobian |
                MappedGrid::THEvertex | MappedGrid::THEcenter );
    }
  else
    {
      int grid;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	if( mg.isRectangular() )
	  {
	    mg.update(MappedGrid::THEmask);
	  }
	else
	  {
	    mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEinverseCenterDerivative | 
		      MappedGrid::THEcenterJacobian );
	  }
      }
    }
  // kkc 060228
  if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit || 
      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton)
    {
      cg.update(MappedGrid::THEcenterBoundaryTangent |
		MappedGrid::THEvertexBoundaryNormal );
      buildImplicitSolvers(cg); // This will build the array of implicit solvers
    }

  if (parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
    parameters.dbase.put<bool>("amrNeedsTimeStepInfo",true);
  
  // --- check for negative volumes : this is usually bad news --- *wdh* 2013/09/26
  const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  const int numberOfGhost = orderOfAccuracyInSpace/2;
  int numberOfNegativeVolumes= GridStatistics::checkForNegativeVolumes( cg,numberOfGhost,stdout ); 
  if( numberOfNegativeVolumes>0 )
  {
    printF("Cgcns::FATAL Error: this grid has negative volumes (maybe only in ghost points).\n"
           "  This will normally cause severe or subtle errors. Please remake the grid.\n");
    OV_ABORT("ERROR");
  }
  else
  {
    printF("Cgcns:: No negative volumes were found\n.");
  }

  return DomainSolver::setupGridFunctions();

}

// ===================================================================================================================
/// \brief Initialize the solution, project velocity if required.
// ===================================================================================================================
int Cgcns::
initializeSolution()
{

  DomainSolver::initializeSolution();

  // *wdh* added 030916
  if( parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
  {
    // For the Godunov Method we first call getUt so that the eigenvalues needed to determine
    // the time step are computed.
    parameters.dbase.get<real >("dt")=1.e-5;  // we need an initial value
    DomainSolver::getUt( gf[current],gf[current].t,fn[0],gf[current].t);
    gf[current].conservativeToPrimitive();
  }
  // Get the initial time step *wdh* added 030805
  dt= getTimeStep( gf[current] ); 

  return 0;
}


