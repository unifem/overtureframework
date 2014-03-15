#include "Cgcns.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "CnsParameters.h"

#define SETEOS EXTERN_C_NAME(seteos)

extern "C"
{
  void SETEOS( const int &m, 
               const int &nd1a,const int &nd1b,const int &n1a,const int &n1b,
               const int &nd2a,const int &nd2b,const int &n2a,const int &n2b,
               const int &nd3a,const int &nd3b,const int &n3a,const int &n3b,
	       real & u,const int &mask,const int & nrparam, real & rparam,
               const int &niparam, int & iparam, int & ier);
}


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
int Cgcns::
updateStateVariables(GridFunction & cgf, int stage /* = -1 */)
{
  if( parameters.dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleNavierStokes && 
      parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
  {
    if( parameters.dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod")==CnsParameters::multiFluidVersion )
    {
      // nothing to do currently for the multifluid (cmfdu.f)
      return 0;
    }
    
      
    if( debug() & 4 )
      printf(" ++++++++++++++updateStateVariables called at t=%9.3e, form=%d ++++++++++++\n",cgf.t,cgf.form);

//    cgf.primitiveToConservative(parameters);

    CompositeGrid & cg = cgf.cg;
    
    const int nrparam=20;
    RealArray rparam(nrparam);
    rparam=0.;

    const int niparam=20;
    IntegerArray iparam(niparam);
    iparam(0)=parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
    iparam(1)=parameters.dbase.get<int >("numberOfSpecies");
    if( parameters.dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod")==CnsParameters::multiComponentVersion )
    {
      iparam(10)=1;
    }
    else
    {
      iparam(10)=0;
    }
    if( parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")==CnsParameters::igDesensitization )
    {
      iparam(15)=1;
    }
    else
    {
      iparam(15)=0;
    }

// check if cgf is in conserved or primitive variables
    if( cgf.form==GridFunction::primitiveVariables )
    {
      // assert( stage==1 ); // this may not be true when called from buildAmrGridsForInitialConditions -> interpolateAndApplyBoundaryConditions
      iparam(2)=2;                              // second stage
    }
    else
    {
      // assert( stage==0 );
      iparam(2)=1;                             // first stage
    }

    ListOfShowFileParameters & pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

    /*if( parameters.dbase.get<Parameters::EquationOfStateEnum >("equationOfState")==Parameters::jwlEOS &&
      parameters.dbase.get<Parameters::GodunovVariation >("conservativeGodunovMethod")!=Parameters::multiComponentVersion )
      {
      bool fdomeg1, fdajwl11, fdajwl21, fdrjwl11, fdrjwl21;
      bool fdomeg2, fdajwl12, fdajwl22, fdrjwl12, fdrjwl22;
      bool fdvs0, fdvg0, fdcgcs, fdheat;
      
      fdomeg1=pdeParameters.getParameter("omeg1",rparam(0));
      fdajwl11=pdeParameters.getParameter("ajwl11",rparam(1));
      fdajwl21=pdeParameters.getParameter("ajwl21",rparam(2));
      fdrjwl11=pdeParameters.getParameter("rjwl11",rparam(3));
      fdrjwl21=pdeParameters.getParameter("rjwl21",rparam(4));

      fdomeg2=pdeParameters.getParameter("omeg2",rparam(5));
      fdajwl12=pdeParameters.getParameter("ajwl12",rparam(6));
      fdajwl22=pdeParameters.getParameter("ajwl22",rparam(7));
      fdrjwl12=pdeParameters.getParameter("rjwl12",rparam(8));
      fdrjwl22=pdeParameters.getParameter("rjwl22",rparam(9));

      fdvs0=pdeParameters.getParameter("vs0",rparam(10));
      fdvg0=pdeParameters.getParameter("vg0",rparam(11));
      fdcgcs=pdeParameters.getParameter("cgcs",rparam(12));
      fdheat=pdeParameters.getParameter("heat",rparam(13));

      if( !fdomeg1 || !fdajwl11 || !fdajwl21 || !fdrjwl11 || !fdrjwl21 )
      {
      printf("Error (updateStateVariables) : undefined solid JWL EOS parameter(s)\n");
      exit(0);
      }
      if( !fdomeg2 || !fdajwl12 || !fdajwl22 || !fdrjwl12 || !fdrjwl22 )
      {
      printf("Error (updateStateVariables) : undefined gas JWL EOS parameter(s)\n");
      exit(0);
      }
      if( !fdvs0 || !fdvg0 || !fdcgcs || !fdheat )
      {
      printf("Error (updateStateVariables) : undefined equil. or heat EOS parameter(s)\n");
      exit(0);
      }
      }*/

    Index I1,I2,I3;
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];

      #ifdef USE_PPP
        realSerialArray u; getLocalArrayWithGhostBoundaries(cgf.u[grid],u);
        intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
      #else  
        const intSerialArray & mask = mg.mask();
        realSerialArray & u = cgf.u[grid];
      #endif

      // form the local bc values (parallel ghost boundaries appear as periodic, bc==-1)
      IntegerArray bcLocal(2,3);
      ParallelGridUtility::getLocalBoundaryConditions( cgf.u[grid],bcLocal );

      if( iparam(2)==2 )
      {
        // In this case seteos will assign boundary and ghost points
        getIndex(mg.extendedRange(),I1,I2,I3);
        iparam(3)=bcLocal(0,0);
        iparam(4)=bcLocal(1,0);
        iparam(5)=bcLocal(0,1);
        iparam(6)=bcLocal(1,1);
        iparam(7)=bcLocal(0,2);
        iparam(8)=bcLocal(1,2);
      }
      else
      {
        getIndex(mg.extendedRange(),I1,I2,I3);
      }
      
      iparam(9)=mg.numberOfDimensions();
      
      iparam(11)=parameters.dbase.get<int >("myid");

      bool ok = ParallelUtility::getLocalArrayBounds(cgf.u[grid],u,I1,I2,I3,1);   
      if( !ok ) continue;  // no points on this processor

      int ier=0;
      SETEOS( parameters.dbase.get<int >("numberOfComponents"),
              u.getBase(0),u.getBound(0),I1.getBase(),I1.getBound(),
              u.getBase(1),u.getBound(1),I2.getBase(),I2.getBound(),
              u.getBase(2),u.getBound(2),I3.getBase(),I3.getBound(),
              *u.getDataPointer(), *mask.getDataPointer(),
              nrparam, rparam(0), niparam, iparam(0), ier );
      
    }



  }
  
  if( false &&   // *wdh* this was added 060713 to track down a bug -- I don't think it is needed(?)
      parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov &&
      parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")==CnsParameters::jwlEOS )
  {
    printF("updateStateVariables: update parallel ghost after seteos\n");
    for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
    {
      realArray & un = cgf.u[grid];
      un.updateGhostBoundaries();
    }
  }
    
  return 0;
}
