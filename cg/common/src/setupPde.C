#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "MaterialProperties.h"
#include "Chemkin.h"

#include "EquationDomain.h"
#include "SurfaceEquation.h"


int readRestartFile(GridFunction & cgf, Parameters & parameters,
                    const aString & restartFileName =nullString );



// ===================================================================================================================
/// \brief A virtual function to setup the PDE to be solved.
/// \details This function is called at the very start in order to setup the equations
///   to be solved etc. 
/// \param reactionName (input) : 
/// \param restartChosen (input) : 
/// \param originalBoundaryCondition (input) : 
///
// ===================================================================================================================
int DomainSolver::
setupPde(aString & reactionName, bool restartChosen, IntegerArray & originalBoundaryCondition)
{
  if( true )
  {
    Overture::abort("DomainSolver::setupPde:ERROR: base class called");
    return 1;
  }

  return 0;
}

// ===================================================================================================================
/// \brief Set the plot titles for interactive plotting.
/// \param t (input) : current time
/// \param dt (input) : current time step
///
// ===================================================================================================================
int DomainSolver::
setPlotTitle(const real &t, const real &dt)
{
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  aString buff;
  psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at time %f, dt=%4.1e",t,dt));

  return 0;
}
