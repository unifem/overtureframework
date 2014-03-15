// ====================================================================================================================
//  This file contains the functions that evaluate model variables (such as the visco-plastic viscosity
// or the turbulent viscosity).
//
// ====================================================================================================================

#include "Cgins.h"
#include "InsParameters.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"


// ===================================================================================================================
/// \brief Compute model variables, such as a nonlinear coefficient of viscosity
/// \details This routine is used to compute the coefficient of viscosity some turbulence models or the
///        visco-plastic viscosity for examle.
/// \param name (input) : evaluate this quantity. Example, "viscosity"
/// \param cgf (input) : use this solution 
/// \param r (output) : save results here.
/// \param component (input) : save results in this component of "r".
// ==================================================================================================================
int InsParameters::
getModelVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, 
                             const int component )
{

  const InsParameters::PDEModel pdeModel = dbase.get<InsParameters::PDEModel >("pdeModel");
  const Parameters::TurbulenceModel & turbulenceModel = 
                 dbase.get<Parameters::TurbulenceModel >("turbulenceModel");

  if( pdeModel==InsParameters::viscoPlasticModel )
  {
    return getViscoPlasticVariables(name,cgf,r,component);
  }
  else if( pdeModel==InsParameters::twoPhaseFlowModel )
  {
    return getTwoPhaseFlowVariables(name,cgf,r,component);
  }
  else if( pdeModel==standardModel || 
           turbulenceModel==LargeEddySimulation )
  {
    return getTurbulenceModelVariables(name,cgf,r,component);
  }
  else if( pdeModel!=BoussinesqModel )
  {
    printF("InsParameters::getModelVariables:ERROR: unknown pdeModel=%i\n",(int)pdeModel);
    Overture::abort("error");
  }
  
}

int InsParameters::
getModelVariables( const aString & name, const realMappedGridFunction & u, realMappedGridFunction & v,
		   const int grid,
		   const int component,
		   const real t )
{
  const InsParameters::PDEModel pdeModel = dbase.get<InsParameters::PDEModel >("pdeModel");
  const Parameters::TurbulenceModel & turbulenceModel = 
                 dbase.get<Parameters::TurbulenceModel >("turbulenceModel");

  if( pdeModel==InsParameters::viscoPlasticModel )
  {
    return getViscoPlasticVariables(name,u,v,grid,component,t);
  }
  else if( pdeModel==InsParameters::twoPhaseFlowModel )
  {
    return getTwoPhaseFlowVariables(name,u,v,grid,component,t);
  }
  else if( pdeModel==standardModel || 
           turbulenceModel==LargeEddySimulation )
  {
    return getTurbulenceModelVariables(name,u,v,grid,component,t);
  }
  else if( pdeModel!=BoussinesqModel )
  {
    printF("InsParameters::getModelVariables:ERROR: unknown pdeModel=%i\n",(int)pdeModel);
    Overture::abort("error");
  }

}
