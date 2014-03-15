// ====================================================================================================================
//  This file contains functions that define the two-phase flow model
//
//    Define the density the viscosity from the other variables.
// 
//  See also
//    insImpTP.bf
//    setupPde.C - looks up parameters to set the plot title.
// ====================================================================================================================

#include "Cgins.h"
#include "InsParameters.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
    int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
    int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
    for(i3=I3Base; i3<=I3Bound; i3++) \
    for(i2=I2Base; i2<=I2Bound; i2++) \
    for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
    I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
    I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
    for(i3=I3Base; i3<=I3Bound; i3++) \
    for(i2=I2Base; i2<=I2Bound; i2++) \
    for(i1=I1Base; i1<=I1Bound; i1++)


// ===================================================================================================================
/// \brief Define variables for the visco-plastic model.
/// \details This routine is used to compute the coefficient of viscosity for the visco-plastic model.
/// \param name (input) : evaluate this quantity: "density", "viscosity", 
///     Note: if name=="twoPhaseFlowVariables" then evaluate both "density" and the "viscosity"
///      and save these in component and component+1. 
/// \param cgf (input) : use this solution 
/// \param r (output) : save results here.
/// \param component (input) : save results in this component of "r".
// ==================================================================================================================
int InsParameters::
getTwoPhaseFlowVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, const int component )
{

  const CompositeGrid & cg = cgf.cg;
  const realCompositeGridFunction & u = cgf.u;
  
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getTwoPhaseFlowVariables( name,u[grid],r[grid],grid,component, cgf.t );
  }

  // we optionally interpolate the variable here in case we need values at interpolation points.
  if( name=="viscosity" )
  {
    // we could optionally smooth and/or interpolate variables here.

    // r.interpolate(Range(component,component));
  }

  return 0;
}

int InsParameters::
getTwoPhaseFlowVariables( const aString & name, const realMappedGridFunction & uIn, realMappedGridFunction & vIn, 
                          const int grid,
                          const int component,
                          const real t )
{

  MappedGrid & mg = *vIn.getMappedGrid();

#ifdef USE_PPP
  realSerialArray v; getLocalArrayWithGhostBoundaries(vIn,v);
  realSerialArray u; getLocalArrayWithGhostBoundaries(uIn,u);
#else
  realSerialArray & v = vIn;
  const realSerialArray & u = uIn;
#endif


  const int uc=dbase.get<int >("uc");
  const int vc=dbase.get<int >("vc");
  const int wc=dbase.get<int >("wc");
  const int tc=dbase.get<int >("tc");
  const int nc=dbase.get<int >("nc");
  const int rc=dbase.get<int >("rc");

  Index I1,I2,I3;
  getIndex(mg.dimension(),I1,I2,I3);
      
  const int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(uIn,u,I1,I2,I3,includeGhost);
  if( !ok ) return 0;

  real nu = dbase.get<real >("nu");

  DataBase & pdeParameters = dbase.get<DataBase>("PdeParameters");

  real rho1=1., rho2=1., mu1=.1, mu2=.1;

  if( pdeParameters.has_key("twoPhaseRho1")) rho1 = pdeParameters.get<real>("twoPhaseRho1");
  if( pdeParameters.has_key("twoPhaseRho2")) rho2 = pdeParameters.get<real>("twoPhaseRho2");
  if( pdeParameters.has_key("twoPhaseMu1" ))  mu1 = pdeParameters.get<real>("twoPhaseMu1");
  if( pdeParameters.has_key("twoPhaseMu2" ))  mu2 = pdeParameters.get<real>("twoPhaseMu2");
  
  // for testing we give rho an explicit time dependence: 
  real rhot=0.;
  if( pdeParameters.has_key("twoPhaseRhot")) rhot = pdeParameters.get<real>("twoPhaseRhot");
  
  int twoPhaseOption =0; // 0=smooth, 1=Heaviside for rho and mu 
  if( pdeParameters.has_key("twoPhaseOption")) twoPhaseOption = pdeParameters.get<real>("twoPhaseOption");

  int & debug = dbase.get<int >("debug");
  if( debug & 4 )
    printF("getTwoPhaseFlowVariables: rho1=%g rho2=%g mu1=%g mu2=%g rhot=%g\n",rho1,rho2,mu1,mu2,rhot);

//   if( true )
//   {
//     rho1=2.;
//     rho2=1.;
//     mu1=nu;
//     mu2=2.*nu;
//   }
//   else if( false )
//   { // const. rho 
//     rho1=2.;
//     rho2=2.;
//     mu1=nu;
//     mu2=.5*nu;
//   }
//   else if( false )
//   { // const nu 
//     rho1=2.;
//     rho2=1.;
//     mu1=nu;
//     mu2=nu;
//   }
//   else if( true )
//   {
//     rho1=2.;
//     rho2=2.;
//     mu1=nu;
//     mu2=nu;
//   }
  
  int n=component;  // put result into this component

  if( name=="density" || name=="twoPhaseFlowVariables" )
  {
    // do this for now: 
    if( twoPhaseOption==1 )
    {
      where( u(I1,I2,I3,tc)>=.5 )
        v(I1,I2,I3,n)= rho1;
      otherwise()
        v(I1,I2,I3,n)= rho2;
    }
    else
    {
      v(I1,I2,I3,n)= rho1*u(I1,I2,I3,tc) + rho2*(1.-u(I1,I2,I3,tc))  + rhot*t;
    }
    n++;  // increment for next component to be saved
  }

  if( name=="viscosity" || name=="twoPhaseFlowVariables" )
  {
    if( twoPhaseOption==1 )
    {
      where( u(I1,I2,I3,tc)>=.5 )
        v(I1,I2,I3,n)= mu1;
      otherwise()
        v(I1,I2,I3,n)= mu2;
    }
    else
    {
      v(I1,I2,I3,n)= mu1*u(I1,I2,I3,tc) + mu2*(1.-u(I1,I2,I3,tc));
    }
    
    n++;  // increment for next component to be saved
  }
    


  return 0;
}
