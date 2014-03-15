#include "Cgins.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "InsParameters.h"

// #define SETEOS seteos_

// extern "C"
// {
//   void SETEOS( const int &m, 
//                const int &nd1a,const int &nd1b,const int &n1a,const int &n1b,
//                const int &nd2a,const int &nd2b,const int &n2a,const int &n2b,
//                const int &nd3a,const int &nd3b,const int &n3a,const int &n3b,
// 	       real & u,const int &mask,const int & nrparam, real & rparam,
//                const int &niparam, int & iparam, int & ier);
// }


// ===================================================================================================================
/// \brief Ths function is used to update state-variables. For example, the visco plastic viscosity. 
/// 
/// \param cgf (input/output)
/// \param stage (input) : -1, 0 or 1 
///
/// \details 
/// If stage equals -1 then update state variables at all points. 
/// 
/// This function is used at two different stages for each time step.  In the first
/// stage, (stage=0) the function is called after the solution has been advanced (but before boundary
/// conditions have been applied) to update any equilibrium state variables (and to limit any 
/// reacting species variables). Update all points of state variables 
/// that may be needed to apply the boundary conditions. 
///
/// In the second stage, (stage=1) the function is called after the boundary conditions have
/// been applied. Make sure that the state variables have been updated at all points after this step.
// ===================================================================================================================
int Cgins::
updateStateVariables(GridFunction & cgf, int stage /* = -1 */ )
{
  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
  if( pdeModel==InsParameters::viscoPlasticModel )
  {
    if( stage==-1 || stage==1 )
    {
      if( debug() & 2 )
        printF(" ****Cgins::updateStateVariables: compute nu for visco-plastic model, t=%9.3e ****\n",cgf.t);

      int & vsc = parameters.dbase.get<int >("vsc");
      assert( vsc>=0 );
      ((InsParameters&)parameters).getViscoPlasticVariables( "viscosity", cgf, cgf.u, vsc );

    }
  }
  if( pdeModel==InsParameters::twoPhaseFlowModel )
  {
    if( stage==-1 || stage==1 )
    {
      if( debug() & 2 )
      {
        printF(" ****Cgins::updateStateVariables for the twoPhaseFlowModel t=%9.3e ****\n",cgf.t);
        fPrintF(debugFile," ****Cgins::updateStateVariables for the twoPhaseFlowModel t=%9.3e ****\n",cgf.t);
      }
      
      int & rc = parameters.dbase.get<int >("rc");
      int & vsc = parameters.dbase.get<int >("vsc");
      assert( rc>=0 );
      assert( vsc==(rc+1) ); // we assume this in the next function 
      // evaluate the density and the viscosity: 
      ((InsParameters&)parameters).getTwoPhaseFlowVariables( "twoPhaseFlowVariables", cgf, cgf.u, rc );

    }
  }

  const Parameters::TurbulenceModel & turbulenceModel = 
                 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  
  if( turbulenceModel==Parameters::BaldwinLomax ||
      turbulenceModel==Parameters::kEpsilon ||
      turbulenceModel==Parameters::kOmega  ||
      turbulenceModel==Parameters::LargeEddySimulation )
  {
    const int & vsc = parameters.dbase.get<int >("vsc");
    assert( vsc>=0 );
    ((InsParameters&)parameters).getTurbulenceModelVariables( "viscosity", cgf, cgf.u, vsc );
  }
  else if( turbulenceModel!=Parameters::noTurbulenceModel &&
           turbulenceModel!=Parameters::SpalartAllmaras )
  {
    Overture::abort("updateStateVariables: unexpected value for turbulenceModel");
  }
  

  return 0;
}
