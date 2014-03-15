// ====================================================================================================================
//  This file contains functions that define the viscoPlastic model.
//
//  See also
//    viscoPlasticMacrosCpp.h - defines the model
//    setupPde.C - looks up parameters to set the plot title.
//    (Note used anymore: getViscosity.bf - optimized fortran routines to evaluate the viscosity and other variables.)
// ====================================================================================================================

#include "Cgins.h"
#include "InsParameters.h"
#include "CompositeGridOperators.h"
#include "viscoPlasticMacrosCpp.h"
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
/// \param name (input) : evaluate this quantity: "viscosity", "viscoPlasticYield", 
///     "viscoPlasticStrainRate", "sigmaxx", "sigmaxy", "sigmaxz", "sigmayy", "sigmayz", "sigmazz"
///
///     Note: if name=="viscoPlasticVariables" then evaluate both "viscoPlasticYield", "viscoPlasticStrainRate" and
///      and save these in component and component+1. 
/// \param cgf (input) : use this solution 
/// \param r (output) : save results here.
/// \param component (input) : save results in this component of "r".
// ==================================================================================================================
int InsParameters::
getViscoPlasticVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, const int component )
{

  const CompositeGrid & cg = cgf.cg;
  const realCompositeGridFunction & u = cgf.u;
  
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getViscoPlasticVariables( name,u[grid],r[grid],grid,component,cgf.t );
  }

  // we optionally interpolate the variable here in case we need values at interpolation points.
  if( name=="viscosity" )
  {
    // we could smooth the viscosity here 

    r.interpolate(Range(component,component));

    // *wdh* 080416 -- we need to extrapolate ghost values of the viscosity AFTER interpolation
    //   so that we get proper values at ghost points where the boundary pt is interpolation. 
    //   These values are used by the RHS to the pressure eqn, for example. 
    BoundaryConditionParameters extrapParams;
    extrapParams.orderOfExtrapolation=2;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      r.applyBoundaryCondition(component,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 
      r.finishBoundaryConditions();
    }
    
  }

  return 0;
}

int InsParameters::
getViscoPlasticVariables( const aString & name, const realMappedGridFunction & uIn, realMappedGridFunction & vIn, 
                          const int grid,
                          const int component,
                          const real t )
{

  // evaluate variables for the visco-plastic model

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

  Index I1,I2,I3;
  int extra=1;  
  getIndex(mg.gridIndexRange(),I1,I2,I3,extra); // only compute derivatives here 
      
  const int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(uIn,u,I1,I2,I3,includeGhost);
  if( !ok ) return 0;

  // This is in-efficient but ok for now --
  RealArray esr(I1,I2,I3),nuT(I1,I2,I3);

  InsParameters & parameters = *this;
  
  // declare and lookup visco-plastic parameters (macro)
  declareViscoPlasticParameters;

  epsViscoPlastic=0.;  // we can set this to zero now.

  MappedGridOperators & op = *vIn.getOperators(); 

  RealArray u0x(I1,I2,I3),u0y(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
  RealArray u0z,v0z,w0x,w0y,w0z;
  op.derivative(MappedGridOperators::xDerivative,u,u0x,I1,I2,I3,uc);
  op.derivative(MappedGridOperators::yDerivative,u,u0y,I1,I2,I3,uc);
  op.derivative(MappedGridOperators::xDerivative,u,v0x,I1,I2,I3,vc);
  op.derivative(MappedGridOperators::yDerivative,u,v0y,I1,I2,I3,vc);
    
  if( mg.numberOfDimensions()==2 )
  {
    esr = strainRate2d(u0x,u0y,v0x,v0y);
  }
  else
  {
    u0z.redim(I1,I2,I3),v0z.redim(I1,I2,I3),w0x.redim(I1,I2,I3),w0y.redim(I1,I2,I3),w0z.redim(I1,I2,I3);

    op.derivative(MappedGridOperators::zDerivative,u,u0z,I1,I2,I3,uc);
    op.derivative(MappedGridOperators::zDerivative,u,v0z,I1,I2,I3,vc);

    op.derivative(MappedGridOperators::xDerivative,u,w0x,I1,I2,I3,wc);
    op.derivative(MappedGridOperators::yDerivative,u,w0y,I1,I2,I3,wc);
    op.derivative(MappedGridOperators::zDerivative,u,w0z,I1,I2,I3,wc);

    esr = strainRate3d(u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z);
  }


  const real mp=exponentViscoPlastic;  // shorter names
  const real ys=yieldStressViscoPlastic;
  // Choose the value for tol from :
  //   error in (1-exp(-mp*x)) =  O(eps)    (for mp*x small, eps=machine epsilon)
  //   error in series expansion to (1-exp(-mp*x)) = (mp*x)^5/5!
  //     switch to series when : (mp*x)^5/5! < eps 
  //        tol = ( eps*5!/mp )^(1/5) 
  // Then
  //   error in (1/x)*(1-exp(-mp*x)) = eps/tol
  //   For double precision, mp=50, eps/tol = 2.5e-13 
  const real tol = pow( 120.*REAL_EPSILON/max(REAL_MIN,mp), 1./5.);

//  printF(" --- getViscoPlasticVariables: tol=%8.2e, eps/tol=%8.2e ---\n",tol,REAL_EPSILON/tol);

  
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,I1,I2,I3) 
  {
    real sr = esr(i1,i2,i3);
    real exp0 = exp(-mp*sr); 
    real te= u(i1,i2,i3,tc); // here is the temperature

//     if( sr<1.e-2 )
//     {
//       real expr1=(etaViscoPlastic + (ys/max(1.e-15,sr))*(1.-exp0));
//       real expr2=etaViscoPlastic + ys*mp*( 1.+mp*sr*( -.5 + mp*sr*(1./6. + mp*sr*(-1./24.))));
//       printF(" sr=%9.3e, nuT=%10.4e, series=%10.4e, err=%8.2e \n",sr,expr1,expr2,fabs(expr1-expr2));
//     }
    
    if( sr>tol )
    {
      nuT(i1,i2,i3) = (etaViscoPlastic + (ys/sr)*(1.-exp0));
    }
    else
    {
      // use a Taylor series for small values of mp*sr, error =  O( (ys/sr)*(mp*sr)^5 )
      nuT(i1,i2,i3) = etaViscoPlastic + ys*mp*( 1.+mp*sr*( -.5 + mp*sr*(1./6. + mp*sr*(-1./24.))));
    }
    
  }
  
  Range all;
  int n=component;  // put result into this component
  if( name=="viscosity" )
  {
    v(all,all,all,n)=0.;
    v(I1,I2,I3,n)=nuT;  


    // assign ghost point values too
    BoundaryConditionParameters extrapParams;
    extrapParams.orderOfExtrapolation=2;
    op.applyBoundaryCondition(vIn, n,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 
    op.finishBoundaryConditions(vIn);

    n++;  // increment for next component to be saved
  }
    
  if( name=="viscoPlasticYield" || name=="yield" || name=="viscoPlasticVariables"  )
  {
    // yield-surface = 1 : this marks the boundary between the solid and plastic regimes

    // evaluate || sigma || / ( yieldStress )
    //  use: || sigma || = |eta|*|| eDot ||
    //      || eDot || = effectiveYieldStress  (?? is this right )       
    real yieldStress=yieldStressViscoPlastic;
    if( yieldStressViscoPlastic==0. ) yieldStress=1.;
    
    v(all,all,all,n)=0.;
    v(I1,I2,I3,n)=(2./(yieldStress))*nuT*esr;  // *wdh* **check this** -> factor turns to 1. ???

    n++;  // increment for next component to be saved
  }

  if( name=="viscoPlasticStrainRate" || name=="eDot" || name=="viscoPlasticVariables" )
  {
    // The effective strain rate is a norm of the rate rate tensor.
    v(all,all,all,n)=0.;
    v(I1,I2,I3,n)=esr;  

    n++;  // increment for next component to be saved
  }

  if( name.matches("sigma") )
  {
    // Here are the components of the stress tensor, sigma'_ij = eta*( D_j u_i + D_i u_j ) 
    if( name=="sigmaxx" )
    {
      v(I1,I2,I3,n)=nuT*2.*u0x;     n++;     // sigma'_xx 
    }
    if( name=="sigmaxy")
    {
      v(I1,I2,I3,n)=nuT*(u0y+v0x);  n++;     // sigma'_xy
    }
    if( name=="sigmaxz")
    {
      if( mg.numberOfDimensions()==3 )
        v(I1,I2,I3,n)=nuT*(u0z+w0x);        // sigma'_xz
      else
        v(I1,I2,I3,n)=0.;
      n++;
    }
    if( name=="sigmayy")
    {
      v(I1,I2,I3,n)=nuT*2.*v0y;     n++;     // sigma'_yy
    }
    if( name=="sigmayz")
    {
      if( mg.numberOfDimensions()==3 )
	v(I1,I2,I3,n)=nuT*(v0z+w0y);           // sigma'_yz
      else
        v(I1,I2,I3,n)=0.;
      n++;
    }
    if( name=="sigmazz")
    {
      if( mg.numberOfDimensions()==3 )
	v(I1,I2,I3,n)=nuT*2.*w0z;              // sigma'_zz
      else
        v(I1,I2,I3,n)=0.;
      n++;
    }
    
  }
  

  return 0;
}
