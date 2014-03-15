#include "OB_MappedGridSolver.h"
#include "Parameters.h"
// include "OB_MappedGridFunction.h"
#include "MappedGridOperators.h"
#include "ParallelUtility.h"
#include "ArraySimple.h"
#include "SparseRep.h"

#define ICNSCF icnscf_
#define CNSNOSLIPWALLBCCOEFF cnsnoslipwallbccoeff_
extern "C" {
  void ICNSCF(const int *igdim, const int *igint, 
	      const real *vertex, 
	      const real *rx,
	      const real * det,
	      const int *mask,
	      const int *iparam,
	      const real *param, 
	      const real *uL, 
	      real *coeff); // output: the coefficients

  void CNSNOSLIPWALLBCCOEFF(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
			    const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
			    const real*coeff, const real *rhs,
			    const real*ul, const real*x, const real *aj, const real*rsxy,
			    const int*ipar, const real*rpar, const int*indexRange, const int*bc, const real*bd, const int*bt, int&nbv, const int&cfrhs);
}

// getLocalBoundsAndBoundaryConditions lives in cnsBC.C
extern void
getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                     IntegerArray & gidLocal, 
                                     IntegerArray & dimensionLocal, 
                                     IntegerArray & bcLocal );
int 
OB_MappedGridSolver::
formImplicitTimeSteppingMatrixCNS(realMappedGridFunction & coeff,
				  const real & dt0, 
				  realMappedGridFunction & uL,
				  const int & grid )
{
  // kkc 060304
  // This function manages the construction of the matrix used to solve
  // the linearized compressible Navier-Stokes equation.  The linearization
  // is performed about uL.  
  //
  // Right now, the boundary conditions are lagged to the previous time step so they get
  // dirichlet conditions in this routine.

  real t0=getCPU();
  MappedGridOperators &op = *coeff.getOperators();
  Range all;
  coeff=0.;

  // We need to compute I+dt\theta L where L is the u^{n+1} part of the linearized operator

  // // the next bunch of initialization stuff is taken from getUtCNS
  MappedGrid & mg = *(coeff.getMappedGrid());
  Index I1,I2,I3;
  getIndex(mg.extendedIndexRange(),I1,I2,I3);

  const real theta = parameters.dbase.get<real >("implicitFactor");
  const int numberOfDimensions=mg.numberOfDimensions();
  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");
  const int rc = parameters.dbase.get<int >("rc");
  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");
  const int tc = parameters.dbase.get<int >("tc");
  const bool gridIsMoving = parameters.gridIsMoving(grid);
  const int stencilSize=coeff.sparse->stencilSize;
  const int width = 2*parameters.dbase.get<int >("orderOfAccuracy")+1;
  //  const int width = parameters.dbase.get<int >("orderOfAccuracy")+1;
  const int halfWidth = (width-1)/2;

  ArraySimpleFixed<int,20,1,1,1> iparam;
  iparam[0] = numberOfDimensions;
  iparam[1] = numberOfComponents;
  iparam[2] = rc;
  iparam[3] = uc;
  iparam[4] = vc;
  iparam[5] = wc;
  iparam[6] = tc;
  iparam[7] = gridIsMoving;
  iparam[8] = parameters.isAxisymmetric();
  iparam[9] = parameters.dbase.get<bool >("axisymmetricWithSwirl");
  iparam[10] = stencilSize;
  iparam[11]= width;
  iparam[12]= halfWidth;
  iparam[15]= parameters.dbase.get<int >("debug");
  iparam[18]= parameters.dbase.get<int >("radialAxis"); 
  iparam[19]= grid;

  const real mu = parameters.dbase.get<real >("mu");
  const real gamma = parameters.dbase.get<real >("gamma");
  const real kThermal = parameters.dbase.get<real >("kThermal");
  const real Rg = parameters.dbase.get<real >("Rg");
  const real reynoldsNumber = parameters.dbase.get<real >("reynoldsNumber");
  const real prandtlNumber = parameters.dbase.get<real >("prandtlNumber");
  const real machNumber = parameters.dbase.get<real >("machNumber");

  ArraySimpleFixed<real,20,1,1,1> rparam;
  rparam[0]=reynoldsNumber;
  rparam[1]=prandtlNumber;
  rparam[2]=machNumber;
  rparam[3]=gamma;
  rparam[4]=parameters.dbase.get<real >("implicitFactor");
  rparam[5]=mg.gridSpacing(0);  
  rparam[6]=mg.gridSpacing(1);
  rparam[7]=mg.gridSpacing(2);
  rparam[8]=0; // not used for anything
  rparam[9]=parameters.dbase.get<real >("dt");
  rparam[13]= parameters.dbase.get<real >("av4");

  const realArray & u = uL;
  //  const realArray & gridVelocity = gridVelocity_;

  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    //    realSerialArray utLocal;  getLocalArrayWithGhostBoundaries(ut,utLocal);
    //    const realSerialArray & gridVelocityLocal= gridVelocity.getLocalArray();

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray & maskLocal= (*pmg2).mask().getLocalArray();
    const intSerialArray & maskLocal= mg.mask().getLocalArray();

    //    utLocal=0.; // ***** do this ****

  #else
    const realSerialArray & uLocal  = u;
    //    const realSerialArray & utLocal0 = ut; 
    //    realSerialArray & utLocal = (realSerialArray &)utLocal0;
    //    const realSerialArray & gridVelocityLocal = gridVelocity;

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray  & maskLocal = (*pmg2).mask(); 
    const intSerialArray  & maskLocal = mg.mask(); 

  #endif
  const int *pmask = maskLocal.getDataPointer();

  const int nGhost=2;
  const IntegerArray & gid = mg.gridIndexRange();
  const IntegerArray & indexRange = mg.indexRange();
  
  IntegerArray d(2,3),nr(2,3); 
  d = nr = 0;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    d(0,axis)=uLocal.getBase(axis);
    d(1,axis)=uLocal.getBound(axis);
    
    nr(0,axis)=max(d(0,axis)+nGhost,gid(0,axis));
    nr(1,axis)=min(d(1,axis)-nGhost,gid(1,axis));
    
  }

  mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
#ifdef USE_PPP
  const realSerialArray & vertex = mg.vertex().getLocalArray();
  const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
  const realSerialArray & det = mg.centerJacobian().getLocalArray();
#else
  const realSerialArray & vertex = mg.vertex();
  const realSerialArray & rx = mg.inverseCenterDerivative();
  const realSerialArray & det = mg.centerJacobian();
#endif

  // // // end local variable initialization

  // // now actually build the coefficient matrix
  ICNSCF(d.getDataPointer(),nr.getDataPointer(),vertex.getDataPointer(), rx.getDataPointer(), 
	 det.getDataPointer(), pmask,// grid info
	 iparam.ptr(),rparam.ptr(), // solver paramters
	 uL.getDataPointer(), // state to linearize about
	 coeff.getDataPointer()); // output: the coefficients

  op.setOrderOfAccuracy(4); // do this to get the identity coeffs in the right place


  if ( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::steadyStateNewton)
    {
      if ( parameters.dbase.get<real >("implicitFactor")<10*REAL_EPSILON )
	coeff *= dt0;
      else
	coeff *= parameters.dbase.get<real >("implicitFactor")*dt0;
      
      for( int n=0; n<uL.getLength(3); n++ )
	coeff += op.identityCoefficients(all,all,all,n,n);
    }

  // BOUNDARY CONDITIONS

  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
  BoundaryConditionParameters bcParams;
  const IntegerArray & bc = mg.boundaryCondition();

  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  getLocalBoundsAndBoundaryConditions( uL,gidLocal,dimLocal,bcLocal ); 
  //  getTimeDependentBoundaryConditions( t,grid ); 

  BCTypes::BCNames 
    dirichlet             = BCTypes::dirichlet,
    neumann               = BCTypes::neumann,
    mixed                 = BCTypes::mixed,
    vectorSymmetry        = BCTypes::vectorSymmetry,
    extrapolate           = BCTypes::extrapolate,
    normalComponent       = BCTypes::normalComponent,
    evenSymmetry          = BCTypes::evenSymmetry;
  typedef Parameters::BoundaryCondition BoundaryCondition;
  const BoundaryCondition & noSlipWall                = Parameters::noSlipWall;
  const BoundaryCondition & slipWall                  = Parameters::slipWall;
  const BoundaryCondition & superSonicOutflow         = Parameters::superSonicOutflow;
  const BoundaryCondition & superSonicInflow          = Parameters::superSonicInflow;
  const BoundaryCondition & subSonicOutflow           = Parameters::subSonicOutflow;
  const BoundaryCondition & subSonicInflow            = Parameters::subSonicInflow;
  const BoundaryCondition & symmetry                  = Parameters::symmetry;
  const BoundaryCondition & inflowWithVelocityGiven   = Parameters::inflowWithVelocityGiven;
  const BoundaryCondition & outflow                   = Parameters::outflow;
  const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
                                                      = Parameters::inflowWithPressureAndTangentialVelocityGiven;
  const BoundaryCondition & dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
  const BoundaryCondition & neumannBoundaryCondition  = Parameters::neumannBoundaryCondition;
  const BoundaryCondition & axisymmetric              = Parameters::axisymmetric;
  const BoundaryCondition & farField                  = Parameters::farField;

  //  bcParams.ghostLineToAssign=0;
  // dirichlet conditions
  {
      for( int line=0; line<=2; line++ )
      {
	bcParams.lineToAssign=line;
	//	bcParams.ghostLineToAssign = line;
	for( int n=0; n<uL.getLength(3); n++ )
	  coeff.applyBoundaryConditionCoefficients(n,n,dirichlet,dirichletBoundaryCondition,bcParams);
      }
    bcParams.lineToAssign=0; // reset
    bcParams.extraInTangentialDirections=0;
  }

  // // fill in equations for no-slip wall density and velocity boundary conditions
  int cfrhs = 0; // fill in coeff 

  RealArray &ubv = parameters.dbase.get<RealArray >("userBoundaryConditionParameters");
  IntegerArray ubt;
  ubt = parameters.dbase.get<IntegerArray >("bcInfo") - Parameters::numberOfPredefinedBoundaryConditionTypes;
  int nbv = ubv.getLength(0);

  CNSNOSLIPWALLBCCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
			 uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
			 coeff.getDataPointer(), uL.getDataPointer(),// uL is just a dummy here
			 uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
			 iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), ubv.getDataPointer(), 
			 ubt.getDataPointer(),
			 nbv,cfrhs);

  //  op.setOrderOfAccuracy(2);
  bcParams.orderOfExtrapolation=2;
  bcParams.lineToAssign=1;
  if (parameters.dbase.get<bool >("axisymmetricWithSwirl"))
    coeff.applyBoundaryConditionCoefficients(wc,wc,dirichlet,noSlipWall,bcParams);

  // 
  // symmetry boundary conditions
  //
  bcParams.lineToAssign=0;
  coeff.applyBoundaryConditionCoefficients(tc,tc,neumann,symmetry,bcParams);

  Range V(uc,uc+numberOfDimensions-1);

  Range C(0,parameters.dbase.get<int >("numberOfComponents")-1);  // ***** is this correct ******
  coeff.applyBoundaryConditionCoefficients(V,V,vectorSymmetry,symmetry);
  coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,symmetry);
    
  if (parameters.dbase.get<bool >("axisymmetricWithSwirl"))
    coeff.applyBoundaryConditionCoefficients(wc,wc,neumann,symmetry);

  // 
  // inflow boundary conditions, dirichlet on u,v; dirchlet on either rho or T and neumann on either rho or T
  //
  Range VW(uc, uc+numberOfDimensions-1);
  for ( int c=VW.getBase(); c<=VW.getBound(); c++ )
    {
      coeff.applyBoundaryConditionCoefficients(c,c,dirichlet,subSonicInflow);
      coeff.applyBoundaryConditionCoefficients(c,c,neumann,subSonicInflow);
    }

  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      coeff.applyBoundaryConditionCoefficients(wc,wc,dirichlet,subSonicInflow);
      coeff.applyBoundaryConditionCoefficients(wc,wc,extrapolate,subSonicInflow);
    }
      


  int nb=BCTypes::boundary1;
  bcParams.lineToAssign=0;
  bool hasOutflow = false;
  for ( int a=0; a<numberOfDimensions; a++ )
    for ( int s=0; s<2; s++ )
      {
	if ( mg.boundaryCondition()(s,a)==Parameters::subSonicInflow )
	  {
	    if ( parameters.bcData(rc,s,a,grid)>0. )
	      {
		coeff.applyBoundaryConditionCoefficients(rc,rc,dirichlet,nb);
		coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,nb);
		coeff.applyBoundaryConditionCoefficients(tc,tc,neumann,nb);
	      }
	    else
	      {
		coeff.applyBoundaryConditionCoefficients(tc,tc,dirichlet,nb);
		coeff.applyBoundaryConditionCoefficients(tc,tc,neumann,nb);
		coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,nb);
	      }
	  }
	hasOutflow = hasOutflow || mg.boundaryCondition()(s,a)==Parameters::subSonicOutflow;
	nb++;
      }
  bcParams.lineToAssign=0;
  //
  // outflow conditions, mixed on T(input parsing only understands that and pressure), extrapolate on everything else??
  //
  bcParams.orderOfExtrapolation=1;
  for ( int c=VW.getBase(); c<=VW.getBound(); c++ )
    coeff.applyBoundaryConditionCoefficients(c,c,extrapolate,subSonicOutflow);
  nb=BCTypes::boundary1;
  // fill in the coefficients for the mixed derivative BC:
  bcParams.a.redim(3,2,bcData.getLength(2),1/*grid+1*/); bcParams.a=0.;

  for( int i=0; hasOutflow && i<=1; i++ )
    bcParams.a(i,all,all,0/*grid*/)=bcData(tc+parameters.dbase.get<int >("numberOfComponents")*(1+i),all,all,grid);

  coeff.applyBoundaryConditionCoefficients(tc,tc,mixed,subSonicOutflow,bcParams);
  coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,subSonicOutflow);

//   for ( int a=0; a<numberOfDimensions; a++ )
//     for ( int s=0; s<2; s++ )
//       {
// 	if ( mg.boundaryCondition()(s,a)==Parameters::subSonicOutflow )
// 	  {
// 	    // !!! it would be better to have a pressure condition, but that would require linearlization
// 	    //     and a new fortran function... work for later...
// 	  }
// 	nb++;
//       }
  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      //      coeff.applyBoundaryConditionCoefficients(wc,wc,dirichlet,subSonicOutflow);
      //      coeff.applyBoundaryConditionCoefficients(wc,wc,extrapolate,subSonicOutflow);
      coeff.applyBoundaryConditionCoefficients(wc,wc,extrapolate,subSonicOutflow);
      
    }

  //
  // extrapolate second ghost line
  //
  bcParams.ghostLineToAssign=2;
  //  bcParams.lineToAssign=2;
  bcParams.orderOfExtrapolation=2; 
  for( int n=0; n<uL.getLength(3); n++ )
    coeff.applyBoundaryConditionCoefficients(n,n,extrapolate,BCTypes::allBoundaries,bcParams);

  op.setOrderOfAccuracy(2);
  parameters.timing(Parameters::timeForUpdateOperators)+= getCPU()-t0;
  return 0;
}

int 
OB_MappedGridSolver::
applyBoundaryConditionsForImplicitTimeSteppingCNS(realMappedGridFunction & rhs, 
						  realMappedGridFunction & uL,
						  realMappedGridFunction & gridVelocity,
						  real t,
						  int grid )
{
  MappedGrid & mg = *(rhs.getMappedGrid());
  Index I1,I2,I3;
  getIndex(mg.extendedIndexRange(),I1,I2,I3);

  const real theta = parameters.dbase.get<real >("implicitFactor");
  const int numberOfDimensions=mg.numberOfDimensions();
  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");
  const int rc = parameters.dbase.get<int >("rc");
  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");
  const int tc = parameters.dbase.get<int >("tc");
  const bool gridIsMoving = parameters.gridIsMoving(grid);

  ArraySimpleFixed<int,20,1,1,1> iparam;
  iparam[0] = numberOfDimensions;
  iparam[1] = numberOfComponents;
  iparam[2] = rc;
  iparam[3] = uc;
  iparam[4] = vc;
  iparam[5] = max(wc,vc);
  iparam[6] = tc;
  iparam[7] = gridIsMoving;
  iparam[8] = parameters.isAxisymmetric();
  iparam[9] = parameters.dbase.get<bool >("axisymmetricWithSwirl");
  iparam[15]= parameters.dbase.get<int >("debug");
  iparam[18]= parameters.dbase.get<int >("radialAxis"); 
  iparam[19]= grid;

  const real mu = parameters.dbase.get<real >("mu");
  const real gamma = parameters.dbase.get<real >("gamma");
  const real kThermal = parameters.dbase.get<real >("kThermal");
  const real Rg = parameters.dbase.get<real >("Rg");
  const real reynoldsNumber = parameters.dbase.get<real >("reynoldsNumber");
  const real prandtlNumber = parameters.dbase.get<real >("prandtlNumber");
  const real machNumber = parameters.dbase.get<real >("machNumber");

  ArraySimpleFixed<real,20,1,1,1> rparam;
  rparam[0]=reynoldsNumber;
  rparam[1]=prandtlNumber;
  rparam[2]=machNumber;
  rparam[3]=gamma;
  rparam[4]=parameters.dbase.get<real >("implicitFactor");
  rparam[5]=mg.gridSpacing(0);  
  rparam[6]=mg.gridSpacing(1);
  rparam[7]=mg.gridSpacing(2);
  rparam[8]=0; // not used for anything
  rparam[9]=parameters.dbase.get<real >("dt");

  const realArray & u = uL;
  //  const realArray & gridVelocity = gridVelocity_;

  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    //    realSerialArray utLocal;  getLocalArrayWithGhostBoundaries(ut,utLocal);
    //    const realSerialArray & gridVelocityLocal= gridVelocity.getLocalArray();

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray & maskLocal= (*pmg2).mask().getLocalArray();
    const intSerialArray & maskLocal= mg.mask().getLocalArray();

    //    utLocal=0.; // ***** do this ****

  #else
    const realSerialArray & uLocal  = u;
    //    const realSerialArray & utLocal0 = ut; 
    //    realSerialArray & utLocal = (realSerialArray &)utLocal0;
    //    const realSerialArray & gridVelocityLocal = gridVelocity;

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray  & maskLocal = (*pmg2).mask(); 
    const intSerialArray  & maskLocal = mg.mask(); 

  #endif
  const int *pmask = maskLocal.getDataPointer();

  const int nGhost=2;
  const IntegerArray & gid = mg.gridIndexRange();
  const IntegerArray & indexRange = mg.indexRange();
  
  IntegerArray d(2,3),nr(2,3); 
  d = nr = 0;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    d(0,axis)=uLocal.getBase(axis);
    d(1,axis)=uLocal.getBound(axis);
    
    nr(0,axis)=max(d(0,axis)+nGhost,gid(0,axis));
    nr(1,axis)=min(d(1,axis)-nGhost,gid(1,axis));
    
  }

  mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
#ifdef USE_PPP
  const realSerialArray & vertex = mg.vertex().getLocalArray();
  const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
  const realSerialArray & det = mg.centerJacobian().getLocalArray();
#else
  const realSerialArray & vertex = mg.vertex();
  const realSerialArray & rx = mg.inverseCenterDerivative();
  const realSerialArray & det = mg.centerJacobian();
#endif

  // // // end local variable initialization
  // BOUNDARY CONDITIONS
  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
  BoundaryConditionParameters bcParams;
  const IntegerArray & bc = mg.boundaryCondition();
  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  getLocalBoundsAndBoundaryConditions( uL,gidLocal,dimLocal,bcLocal ); 
  // determine time dependent conditions:
  getTimeDependentBoundaryConditions( t,grid ); 

  BCTypes::BCNames 
    dirichlet             = BCTypes::dirichlet,
    neumann               = BCTypes::neumann,
    mixed                 = BCTypes::mixed,
    vectorSymmetry        = BCTypes::vectorSymmetry,
    extrapolate           = BCTypes::extrapolate,
    normalComponent       = BCTypes::normalComponent,
    evenSymmetry          = BCTypes::evenSymmetry;
  typedef Parameters::BoundaryCondition BoundaryCondition;
  const BoundaryCondition & noSlipWall                = Parameters::noSlipWall;
  const BoundaryCondition & slipWall                  = Parameters::slipWall;
  const BoundaryCondition & superSonicOutflow         = Parameters::superSonicOutflow;
  const BoundaryCondition & superSonicInflow          = Parameters::superSonicInflow;
  const BoundaryCondition & subSonicOutflow           = Parameters::subSonicOutflow;
  const BoundaryCondition & subSonicInflow            = Parameters::subSonicInflow;
  const BoundaryCondition & symmetry                  = Parameters::symmetry;
  const BoundaryCondition & inflowWithVelocityGiven   = Parameters::inflowWithVelocityGiven;
  const BoundaryCondition & outflow                   = Parameters::outflow;
  const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
                                                      = Parameters::inflowWithPressureAndTangentialVelocityGiven;
  const BoundaryCondition & dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
  const BoundaryCondition & neumannBoundaryCondition  = Parameters::neumannBoundaryCondition;
  const BoundaryCondition & axisymmetric              = Parameters::axisymmetric;
  const BoundaryCondition & farField                  = Parameters::farField;

  int cfrhs = 1; // fill in rhs
  //  bcParams.ghostLineToAssign = 1;
  //  bcParams.lineToAssign = 1;
  //  rhs.applyBoundaryCondition(tc,dirichlet,noSlipWall,bcData,pBoundaryData,t,Overture::defaultBoundaryConditionParameters(),grid);

  RealArray &ubv = parameters.dbase.get<RealArray >("userBoundaryConditionParameters");
  IntegerArray ubt;
  ubt = parameters.dbase.get<IntegerArray >("bcInfo") - Parameters::numberOfPredefinedBoundaryConditionTypes;
  int nbv = ubv.getLength(0);
  CNSNOSLIPWALLBCCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
			 uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
 /* uL is just a dummy here*/ uL.getDataPointer(), rhs.getDataPointer(),
			 uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
			 iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), ubv.getDataPointer(), ubt.getDataPointer(),
			 nbv,
			 cfrhs);

  //  Range Vc(uc,max(wc,vc));
  Range C(uc,vc);
  Range V(rc,tc);
  //  bcParams.ghostLineToAssign = 1;
  bcParams.lineToAssign = 1;
  rhs.applyBoundaryCondition(V,dirichlet,symmetry,0,t,bcParams);

  // 
  // inflow boundary conditions, dirichlet on u,v; dirchlet on either rho or T and mixed on either rho or T
  //
  Range VW(uc, uc+numberOfDimensions-1);
  rhs.applyBoundaryCondition(VW,dirichlet,subSonicInflow, bcData, pBoundaryData,t,
			     Overture::defaultBoundaryConditionParameters(),grid);
  bcParams.lineToAssign = 0;
  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      rhs.applyBoundaryCondition(wc,dirichlet,subSonicInflow,bcData, pBoundaryData,t,
				 Overture::defaultBoundaryConditionParameters(),grid);//0,t,bcParams);
      bcParams.lineToAssign = 1;
      rhs.applyBoundaryCondition(wc,dirichlet,subSonicInflow,0,t,bcParams);
    }

  
  int nb=BCTypes::boundary1;
  for ( int a=0; a<numberOfDimensions; a++ )
    for ( int s=0; s<2; s++ )
      {
	if ( mg.boundaryCondition()(s,a)==Parameters::subSonicInflow )
	  {
	    if ( parameters.bcData(rc,s,a,grid)>0. )
	      {
		bcParams.lineToAssign=0;
		rhs.applyBoundaryCondition(rc,dirichlet, subSonicInflow, bcData, pBoundaryData,t,
					   Overture::defaultBoundaryConditionParameters(),grid);
		
		bcParams.lineToAssign=1;
		rhs.applyBoundaryCondition(tc,dirichlet,subSonicInflow,0,t,bcParams);
		rhs.applyBoundaryCondition(rc,dirichlet,subSonicInflow,0,t,bcParams);

		
		//		coeff.applyBoundaryConditionCoefficients(tc,tc,mixed,nb,bcParams);
	      }
	    else
	      {
		bcParams.lineToAssign=0;
		rhs.applyBoundaryCondition(tc,dirichlet, subSonicInflow, bcData, pBoundaryData,t,
					   Overture::defaultBoundaryConditionParameters(),grid);
		bcParams.lineToAssign=1;
		rhs.applyBoundaryCondition(rc,dirichlet,subSonicInflow,0,t,bcParams);
		rhs.applyBoundaryCondition(tc,dirichlet,subSonicInflow,0,t,bcParams);
	      }
	  }
	nb++;
      }
  bcParams.lineToAssign = 0;

  //
  // outflow conditions, mixed on either rho or T, neumann on everything else??
  //
  bcParams.lineToAssign=1;
  rhs.applyBoundaryCondition(VW,dirichlet,subSonicOutflow,0.,t,bcParams);
  nb=BCTypes::boundary1;
  for ( int a=0; a<numberOfDimensions; a++ )
    for ( int s=0; s<2; s++ )
      {
	if ( mg.boundaryCondition()(s,a)==Parameters::subSonicOutflow )
	  {
	    rhs.applyBoundaryCondition(tc,dirichlet,nb,bcData(tc,s,a,grid),t,bcParams);
	    rhs.applyBoundaryCondition(rc,dirichlet,nb,0.,t,bcParams);
	  }
	nb++;
      }
  bcParams.lineToAssign = 0;
  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      //      rhs.applyBoundaryCondition(wc,dirichlet,subSonicOutflow,0.,t);
      //      rhs.applyBoundaryCondition(wc,dirichlet,subSonicOutflow,bcData, pBoundaryData,t,
      //				 Overture::defaultBoundaryConditionParameters(),grid);//0,t,bcParams);
      bcParams.lineToAssign = 1;
      rhs.applyBoundaryCondition(wc,dirichlet,subSonicOutflow,0.,t,bcParams);
    }


  bcParams.lineToAssign = 2;
  //  bcParams.ghostLineToAssign = 2;
  if ( false ) {
  rhs.applyBoundaryCondition(V,extrapolate,BCTypes::allBoundaries,0,t,bcParams);
  }
  // dirichlet conditions
  {
    //    bcParams.extraInTangentialDirections=2;  // *wdh* 050611 -- assign extended boundary
    Range C(rc,tc);
    for( int line=0; line<=2; line++ )
      {
	bcParams.lineToAssign=line;
	rhs.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,0.,t,bcParams);
      }
    bcParams.lineToAssign=0; // reset
    bcParams.extraInTangentialDirections=0;
  }

  return 0;
}
