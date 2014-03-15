// =================================================================================
//   Add interior equation forcing to the incompressible NS and related equations
// =================================================================================

#include "Cgins.h"
#include "Parameters.h"
#include "display.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "turbulenceModels.h"
#include "turbulenceParameters.h"

#include "kkcdefs.h"

// here is a macro for the AFS solver
#define IS_DIRICHLET(SIDE,AXIS) ( bcLocal(SIDE,AXIS)==Parameters::dirichletBoundaryCondition || bcLocal(SIDE,AXIS)==Parameters::noSlipWall || bcLocal(SIDE,AXIS)==InsParameters::inflowWithVelocityGiven || bcLocal(SIDE,AXIS)==InsParameters::outflow)

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


#define uv(m)   e   (mg,I1,I2,I3,m,t)
#define uvt(m)  e.t (mg,I1,I2,I3,m,t)
#define uvx(m)  e.x (mg,I1,I2,I3,m,t)
#define uvy(m)  e.y (mg,I1,I2,I3,m,t)
#define uvz(m)  e.z (mg,I1,I2,I3,m,t)
#define uvxx(m) e.xx(mg,I1,I2,I3,m,t)
#define uvxy(m) e.xy(mg,I1,I2,I3,m,t)
#define uvxz(m) e.xz(mg,I1,I2,I3,m,t)
#define uvyy(m) e.yy(mg,I1,I2,I3,m,t)
#define uvyz(m) e.yz(mg,I1,I2,I3,m,t)
#define uvzz(m) e.zz(mg,I1,I2,I3,m,t)


//\begin{>>MappedGridSolverInclude.tex}{\subsection{addForcingINS}}
void Cgins::
addForcing(realMappedGridFunction & dvdt, 
           const realMappedGridFunction & u,
	   int iparam[], real rparam[],
	   realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
           realMappedGridFunction *referenceFrameVelocity /* =NULL */ )
//========================================================================================================
// /Description:
//   Add the forcing term to $u_t$ on a component grid for the Incompressible NS.
// Here is where we added the analytic derivatives for twilight-zone flow.
//
// /mg (input) : grid
// /dvdt (intput/output) : return $u_t$ in this grid function.
// /t (input) : current time.
// /grid (input) : the component grid number if this MappedGrid is part of a GridCollection or CompositeGrid.
// /dvdtImplicit (input) : for implicit time stepping, the time derivative is split into two parts,
//     $u_t=u_t^E + u_t^I$. The explicit part, $u_t^E$, is returned in dvdt while the implicit part, $u_t^I$,
//   is returned in dvdtImplicit. This splitting does NOT depend on whether we are using backward Euler or
//   Crank-Nicolson since this weighting is applied elsewhere. 
// /tImplicit (input) : for implicit time stepping, apply forcing for the implicit part at his 
//     time.
// 
// Implicit time-stepping notes: 
// ----------------------------
//    Suppose we are solving the PDE:
//           u_t = f(u,x,t)  + F(x,t)
//   that we have split into an explicit part, fe(u),  and implicit part, A*u:
//           u_t = fe(u) + A u  + F(x,t)
//
//   Let the Twilight-zone function be ue(t). 
//   If the time stepping method is implicit then we compute
//           dvdt += F(x,t) - fe(ue(t))
//   When implicitOption==computeImplicitTermsSeparately we also compute:
//           dvdtImplicit +=  -[ alpha*A*ue(tImplicit) + (1-alpha)*A*ue(t) ] 
//   where alpha is the implicit factor (= .5 for Crank-Nicolson)
//   (if implicitOption==doNotComputeImplicitTerms then do not change dvdtImplicit).
// 
// 
//\end{MappedGridSolverInclude.tex}  
//=======================================================================================================
{

  // 111212 kkc add forcing for penalty terms.  Why is this here? Well, addForcing skips out if there 
  //                                            is no twilight zone or body forcing.
  IntegerArray &bcInfo = parameters.dbase.get<IntegerArray>("bcInfo");
  for ( int side=0; side<2; side++ )
    for ( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	real t         =rparam[1];   
	int grid = iparam[0];
	const bool gridIsMoving = parameters.gridIsMoving(grid);
	if (bcInfo(2,side,axis,grid))
	  {
	    Parameters::BCModifier *bcMod = parameters.bcModifiers[bcInfo(2,side,axis,grid)];
	    if ( bcMod->isPenaltyBC() )
	      {

		bcMod->addPenaltyForcing(parameters, 
					 t, dt,
					 u,
					 dvdt,
					 grid,
					 side,
					 axis,
					 (gridIsMoving ? referenceFrameVelocity : 0));
	      }
	  }
      }


  if( !parameters.dbase.get<bool >("twilightZoneFlow") && 
      !parameters.dbase.get<bool >("turnOnBodyForcing") &&
      (parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")!= // *kkc* 2013/10/04 -- added this
                                                    Parameters::approximateFactorization)
       )
  {
    // No forcing to add 
    return;
  }
  

  real cpu0=getCPU();

  if( debug() & 8 )
  {
    printF("Cgins::addForcing: ...\n");
  }
  // *wdh* 081207 MappedGrid & mg = *dvdt.getMappedGrid();
  MappedGrid & mg = *u.getMappedGrid();   // use this for moving grid cases since dvdt may not have the correct grid
  const real & t0=rparam[0];
  real t         =rparam[1];          // this is really tForce
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];
  const int level=iparam[1];
  const int numberOfStepsTaken = iparam[2];
  const bool gridIsMoving = parameters.gridIsMoving(grid);
  const int numberOfDimensions=mg.numberOfDimensions();

  const InsParameters::PDEModel pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
  const Parameters::TurbulenceModel & turbulenceModel = 
                 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");

  FILE *& debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
  
  const ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
  
  DataBase & pdeParameters = parameters.dbase.get<DataBase>("PdeParameters");
  
  // Note: steadyStateRungeKutta is considered implicit but we do not adjust the forcing 
  bool adjustForcingForImplicit = parameters.dbase.get<int>("adjustForcingForImplicit")==1 &&
                                  parameters.getGridIsImplicit(grid) && 
               parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::steadyStateRungeKutta ;

  const bool solveForTemperature = (pdeModel==InsParameters::BoussinesqModel || 
				    pdeModel==InsParameters::viscoPlasticModel);
  
  
  const Parameters::ImplicitOption & implicitOption = parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");


  realArray & ut = dvdt;
  OV_GET_SERIAL_ARRAY(real,ut,utLocal);

  // realArray & uti = dvdtImplicit;
  // OV_GET_SERIAL_ARRAY(real,uti,utiLocal);

  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & pc = parameters.dbase.get<int >("pc");
  const int & tc = parameters.dbase.get<int >("tc");
  const int & kc = parameters.dbase.get<int >("kc");
  const int & epsc = parameters.dbase.get<int >("epsc");
  const int & rc = parameters.dbase.get<int >("rc");
  const int & nc = parameters.dbase.get<int >("nc");

  // *wdh* 080204 const int nc=kc;
  const int ec = kc+1; 

  Index II[3], &I1=II[0], &I2=II[1], &I3=II[2];


  // --- Add on any body forcing (which includes user defined forcings) ---
  if( parameters.dbase.get<bool >("turnOnBodyForcing") )
  {
    // The body forcing has already been computed ( computeForcing should be called in the advance routine)

    assert( parameters.dbase.get<realCompositeGridFunction* >("bodyForce")!=NULL );
    realCompositeGridFunction & bodyForce = *(parameters.dbase.get<realCompositeGridFunction* >("bodyForce"));

    // Add the user defined force onto dvdt:
    OV_GET_SERIAL_ARRAY(real,bodyForce[grid],bodyForceLocal);


    getIndex(mg.gridIndexRange(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,I1,I2,I3); 
    
    const Range & Rt = parameters.dbase.get<Range >("Rt");       // time dependent components

    // printF("addForcing: add body force at t=%9.3e: (min,max)=(%g,%g), Rt=[%i,%i]\n",
    //	   t,min(bodyForceLocal),max(bodyForceLocal),Rt.getBase(),Rt.getBound());

    if( false && tc>= 0 )
    {
      printF("addForcing: add body force to grid=%i at t=%9.3e: T : (min,max)=(%g,%g), Rt=[%i,%i]\n",
	     grid,t,min(bodyForceLocal(I1,I2,I3,tc)),max(bodyForceLocal(I1,I2,I3,tc)),Rt.getBase(),Rt.getBound());
    }

    if (parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization)
      {
	IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);	
	ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( dvdt,gidLocal,dimLocal,bcLocal,CG_ApproximateFactorization::parallelBC ); 
	for ( int side=0; side<2; side++ )
	  for ( int axis=0; axis<numberOfDimensions; axis++ )
	    if ( IS_DIRICHLET(side,axis) )
	      {
		II[axis] = Index(II[axis].getBase()+1-side,II[axis].getLength()-1);
	      }
      }

    utLocal(I1,I2,I3,Rt) += bodyForceLocal(I1,I2,I3,Rt);

  }
  




  if( parameters.dbase.get<bool >("twilightZoneFlow") )
  {

    real nu = parameters.dbase.get<real >("nu");  // note: we make a local copy so we can scale it.
    real kThermal = parameters.dbase.get<real >("kThermal");  // note: we make a local copy so we can scale it.
    const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");

    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    const real ad21 = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad21") : 0.;
    const real cd22 = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad22")/SQR(mg.numberOfDimensions()) : 0.;

    const real ad41 = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad41") : 0.;
    const real cd42 = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad42")/SQR(mg.numberOfDimensions()) : 0.;

    const real ad21n = parameters.dbase.get<real >("ad21n");
    const real cd22n = parameters.dbase.get<real >("ad22n")/mg.numberOfDimensions();

    real adPsi=1., adPhi=1.;
    if( pdeModel==InsParameters::twoPhaseFlowModel )
    {
      if( pdeParameters.has_key("twoPhaseArtDisPsi")) 
       adPsi = pdeParameters.get<real>("twoPhaseArtDisPsi");
      else
      {
        printF("Cgins::addForcing: twoPhaseArtDisPsi not found for twoPhaseFlowModel\n");
	Overture::abort("error");
      }
      if( pdeParameters.has_key("twoPhaseArtDisPhi")) 
       adPhi = pdeParameters.get<real>("twoPhaseArtDisPhi");
      else
      {
        printF("Cgins::addForcing: twoPhaseArtDisPhi not found for twoPhaseFlowModel\n");
	Overture::abort("error");
      }
    }
    

    // ---add forcing for twlight-zone flow---

    OV_GET_LOCAL_ARRAY_CONDITIONAL(real,gridVelocity,!referenceFrameVelocity,utLocal,(*referenceFrameVelocity));

    const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

    if( !isRectangular )
      mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );

    realArray & x= mg.center();
    #ifdef USE_PPP
      realSerialArray xLocal; 
      if( !isRectangular ) 
        getLocalArrayWithGhostBoundaries(x,xLocal);
    #else
      const realSerialArray & xLocal = x;
    #endif

    const bool useVariableViscosity = (pdeModel==InsParameters::viscoPlasticModel ||
                                      pdeModel==InsParameters::twoPhaseFlowModel ||
                                      turbulenceModel==InsParameters::LargeEddySimulation ||
	  			      turbulenceModel==InsParameters::BaldwinLomax ||
				      turbulenceModel==InsParameters::kEpsilon );

    const bool useVariableDiffusivity = turbulenceModel==InsParameters::LargeEddySimulation;
    
    realMappedGridFunction *pnuT=NULL, *pue=NULL;
    if( useVariableViscosity )
    {
      // evaluate the coeff of viscosity using the exact solution
      Range all;
      Range N=parameters.dbase.get<int >("numberOfComponents");
      pue = new realMappedGridFunction(mg,all,all,all,N);
      realMappedGridFunction & ue = *pue;

      // assert( f.getOperators()!=NULL );
      assert( gf[0].u[grid].getOperators()!=NULL );
      ue.setOperators(*gf[0].u[grid].getOperators());

      getIndex(mg.dimension(),I1,I2,I3);

      e.gd(ue,0,0,0,0,I1,I2,I3,N,t);
    
      pnuT = new realMappedGridFunction(mg,all,all,all);
      pnuT->setOperators(*gf[0].u[grid].getOperators());

      ((InsParameters&)parameters).getModelVariables("viscosity", ue, *pnuT,grid, 0,t);

      if( pdeModel==InsParameters::twoPhaseFlowModel )
      { // save the density in ue(.,.,.,rc) 
        assert( rc>=0 );
        ((InsParameters&)parameters).getModelVariables("density", ue, ue,grid, rc,t);
      }
    }
    

    // **** note: the tzForcing arrays are shared with addForcingToPressureEquation
    const int numberOfTZArrays=mg.numberOfDimensions()==1 ? 1 : mg.numberOfDimensions()==2 ? 10 : 14;

    if( grid >= tzTimeVector1.size() ) 
    {
      tzTimeVector1.resize(grid+1,REAL_MAX);
      tzTimeVector2.resize(grid+1,REAL_MAX);
      tzForcingVector.resize(grid+1,NULL);
    }
    real & tzTimeStart1=tzTimeVector1[grid];
    realSerialArray *&tzForcing = tzForcingVector[grid];

    bool evaluateTZ=tzTimeStart1==REAL_MAX;  // set to true if we need to evaluate the TZ functions

    if( tzForcing==NULL )
    {
      evaluateTZ=true;  // evaluate the TZ functions
      
      tzForcing = new realSerialArray [numberOfTZArrays];
      int extra=1;
      getIndex(extendedGridIndexRange(mg),I1,I2,I3,extra);  // allocate space to hold  BC forcing in ghost points
      bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,I1,I2,I3); 
      if( ok )
      {
	tzForcing[0].redim(I1,I2,I3);
	tzForcing[1].redim(I1,I2,I3);
	tzForcing[2].redim(I1,I2,I3);
      }
    }
    // we cannot use the opt evaluation for moving grids since the grid points change
    if( gridIsMoving )
      evaluateTZ=true;  // we are forced to re-evaluate the TZ functions every time step
    
    getIndex(mg.extendedIndexRange(),I1,I2,I3);

   
    real scaleFactor=1., scaleFactorT=1.;
    if( evaluateTZ )
    {
      tzTimeStart1=t;  // save the time at which the TZ functions were evaluated
    }
    else 
    {
      // This is not the first time through -- compute scale factors for stored TZ values

      // Here we assume that the TZ function is a tensor product of a spatial function
      // times a function of time. In this case we just need to scale the TZ function
      // by the new value of the time function
      real xa=.123,ya=.456,za= mg.numberOfDimensions()==2 ? .789 : 0.;
      real ta=tzTimeStart1;
	
      real ua = e(xa,ya,za,uc,ta), va=e(xa,ya,za,vc,ta), pa=e(xa,ya,za,pc,ta), sfta=e.t(xa,ya,za,uc,ta);
      if( fabs(ua) > 1.e-3  && fabs(va) > 1.e-3 && fabs(pa) > 1.e-3 && fabs(sfta)>REAL_EPSILON*100. )
      {
	scaleFactor = e(xa,ya,za,uc,t)/ua; // we assume all time functions are the same

	real scaleFactorv = e(xa,ya,za,vc,t)/va; // we assume all time functions are the same
	real scaleFactorp = e(xa,ya,za,pc,t)/pa; // we assume all time functions are the same
	scaleFactorT=e.t(xa,ya,za,uc,t)/sfta;

	assert( fabs(scaleFactor)<1.e10 && fabs(scaleFactorv)<1.e10 && 
		fabs(scaleFactorp)<1.e10 && fabs(scaleFactorT)<1.e10 );

      }
      else  // we cannot scale with this value ...
      {
        evaluateTZ=true;
	tzTimeStart1=t;  // save the time at which the TZ functions were evaluated
      }
      
      // printf(" scaleFactoru=%e, scaleFactorv=%e, scaleFactorp=%e\n",scaleFactor,scaleFactorv,scaleFactorp);
	
    }


    real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0;
    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, 
                                             cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0);

    real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI;
    if( turbulenceModel==InsParameters::kEpsilon )
      getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI );

    // For Boussinesq: 
    real thermalExpansivity=1.;
    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
    const real kThermalLES = kThermal/nu; // for variable diffusivity (could use Prandtl number instead)
	    


    getIndex( mg.gridIndexRange(),I1,I2,I3);


    #ifdef USE_PPP
      bool useOpt=true;
    #else
      bool useOpt=false || pdeModel==InsParameters::BoussinesqModel || useVariableViscosity || parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization;
    #endif
    if( useOpt )
    {


      // loop bounds for this boundary:
      int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2];
      #ifdef USE_PPP
        n1a=max(I1.getBase(),utLocal.getBase(0)); n1b=min(I1.getBound(),utLocal.getBound(0));
        n2a=max(I2.getBase(),utLocal.getBase(1)); n2b=min(I2.getBound(),utLocal.getBound(1));
        n3a=max(I3.getBase(),utLocal.getBase(2)); n3b=min(I3.getBound(),utLocal.getBound(2));
      #else
        n1a=I1.getBase(); n1b=I1.getBound();
        n2a=I2.getBase(); n2b=I2.getBound();
        n3a=I3.getBase(); n3b=I3.getBound();
      #endif
  
  
      if( n1a>n1b || n2a>n2b || n3a>n3b ) return;
  
      I1=Range(n1a,n1b);
      I2=Range(n2a,n2b);
      I3=Range(n3a,n3b);
  
	
      if( mg.numberOfDimensions()==1 )
      {
        realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0xx(I1,I2,I3);
        realSerialArray p0x(I1,I2,I3); 

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);

	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);


	utLocal(I1,I2,I3,uc)+=u0t+advectionCoefficient*(u0*u0x) + p0x; // -nu*u0xx; 
	utLocal(I1,I2,I3,uc)-= nu*(u0xx);
	
      }
      else if( mg.numberOfDimensions()==2 )
      {

	realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3);
	realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	
        realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3);
        realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3);

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);
	if( pdeModel==InsParameters::twoPhaseFlowModel )
	{ // form grad(p)/rho 
          #ifdef USE_PPP
	   realSerialArray ue; getLocalArrayWithGhostBoundaries(*pue,ue);
          #else	    
	   realSerialArray & ue = *pue;
	  #endif          
	  p0x/=ue(I1,I2,I3,rc);
	  p0y/=ue(I1,I2,I3,rc);
	}
	

        RealArray te,tet,tex,tey, texx,teyy;
	if( solveForTemperature )
	{
          // Evaluate the derivative of T and save in te, tex, texx, ...
          te.redim(I1,I2,I3); tet.redim(I1,I2,I3); tex.redim(I1,I2,I3); tey.redim(I1,I2,I3); 
          texx.redim(I1,I2,I3); teyy.redim(I1,I2,I3);
          assert( tc>=0 );

          e.gd( te  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);
	  e.gd( tet ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
	  e.gd( tex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( tey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
  	  e.gd( texx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( teyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);
	}

        utLocal(I1,I2,I3,uc)+=u0t+advectionCoefficient*(u0*u0x+v0*u0y) + p0x; // -nu*(u0xx+u0yy);
        utLocal(I1,I2,I3,vc)+=v0t+advectionCoefficient*(u0*v0x+v0*v0y) + p0y; // -nu*(v0xx+v0yy);

	// p0x.display("p0x");
        RealArray nuUrOverR, nuVrOverR, kTrOverR, radiusInverse;
        if( turbulenceModel==InsParameters::noTurbulenceModel &&
            (pdeModel==InsParameters::standardModel ||
             pdeModel==InsParameters::BoussinesqModel) )
	{

	  if( parameters.isAxisymmetric() )
	  {
	    
	    // y corresponds to the radial direction
	    // y=0 is the axis of symmetry
	    radiusInverse=1./max(REAL_MIN,xLocal(I1,I2,I3,axis2));

	    nuUrOverR=nu*u0y*radiusInverse;
	    nuVrOverR=nu*(v0y-v0*radiusInverse)*radiusInverse;
	    if( pdeModel==InsParameters::BoussinesqModel )
	      kTrOverR=kThermal*tey*radiusInverse;

	    // fix points on axis
	    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
		if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
		{
		  Index Ib1,Ib2,Ib3;
		  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                  bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,Ib1,Ib2,Ib3); 

		  nuUrOverR(Ib1,Ib2,Ib3)=nu*u0yy(Ib1,Ib2,Ib3);
		  nuVrOverR(Ib1,Ib2,Ib3)=.5*nu*v0yy(Ib1,Ib2,Ib3);
		  if( pdeModel==InsParameters::BoussinesqModel )
		    kTrOverR(Ib1,Ib2,Ib3)=kThermal*teyy(Ib1,Ib2,Ib3);
		}
	      }
	    }
	  }  // end if isAxisymmetric 
	  

	  utLocal(I1,I2,I3,uc)-= nu*(u0xx+u0yy);
	  utLocal(I1,I2,I3,vc)-= nu*(v0xx+v0yy);
	  if( parameters.isAxisymmetric() )
	  {
	    utLocal(I1,I2,I3,uc)-= nuUrOverR;
	    utLocal(I1,I2,I3,vc)-= nuVrOverR;
	  }
	}
	
        if( useVariableViscosity ) 
	{ 	
	  // non-linear viscosity: viscoPlasticModel, BaldwinLomax, LargeEddySimulation, kEpsilon

          assert( pnuT!=NULL );
          #ifdef USE_PPP
            realSerialArray nuT; getLocalArrayWithGhostBoundaries(*pnuT,nuT);
          #else
            realSerialArray & nuT = *pnuT;
          #endif

	  realSerialArray nuTx(I1,I2,I3),nuTy(I1,I2,I3);

          // We just compute the derivatives of the exact nuT using differences. 
          assert( gf[0].u[grid].getOperators()!=NULL );
	  MappedGridOperators & op = *(gf[0].u[grid].getOperators()); // --------- fix this 
          op.derivative(MappedGridOperators::xDerivative ,nuT,nuTx,I1,I2,I3,0);
          op.derivative(MappedGridOperators::yDerivative ,nuT,nuTy,I1,I2,I3,0);

	  if( pdeModel==InsParameters::twoPhaseFlowModel )
	  {
	    // realMappedGridFunction & ue = *pue;
            #ifdef USE_PPP
	     realSerialArray ue; getLocalArrayWithGhostBoundaries(*pue,ue);
            #else	    
	     realSerialArray & ue = *pue;
	    #endif          
	    utLocal(I1,I2,I3,uc)-=( (nuT(I1,I2,I3)*(u0xx+u0yy)+nuTx*(2.*u0x )+nuTy*(u0y+v0x))/ue(I1,I2,I3,rc)
				    +gravity[0]);
	    utLocal(I1,I2,I3,vc)-=( (nuT(I1,I2,I3)*(v0xx+v0yy)+nuTx*(u0y+v0x)+nuTy*(2.*v0y ))/ue(I1,I2,I3,rc)
                                    +gravity[1]);
	  }
	  else
	  {
	    utLocal(I1,I2,I3,uc)-=nuT(I1,I2,I3)*(u0xx+u0yy)+nuTx*(2.*u0x )+nuTy*(u0y+v0x);
	    utLocal(I1,I2,I3,vc)-=nuT(I1,I2,I3)*(v0xx+v0yy)+nuTx*(u0y+v0x)+nuTy*(2.*v0y);
	  }
	  

	  if( turbulenceModel==InsParameters::kEpsilon )
	  {
            RealArray nut(I1,I2,I3), prod(I1,I2,I3);
	    RealArray k0(I1,I2,I3),k0t(I1,I2,I3),k0x(I1,I2,I3),k0y(I1,I2,I3),k0xx(I1,I2,I3),k0yy(I1,I2,I3);
	    e.gd( k0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,kc,t);
	    e.gd( k0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,kc,t);
	    e.gd( k0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,kc,t);
	    e.gd( k0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,kc,t);
	    e.gd( k0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,kc,t);
	    e.gd( k0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,kc,t);
	    RealArray e0(I1,I2,I3),e0t(I1,I2,I3),e0x(I1,I2,I3),e0y(I1,I2,I3),e0xx(I1,I2,I3),e0yy(I1,I2,I3);
	    e.gd( e0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,ec,t);
	    e.gd( e0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,ec,t);
	    e.gd( e0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,ec,t);
	    e.gd( e0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,ec,t);
	    e.gd( e0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,ec,t);
	    e.gd( e0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,ec,t);

            // printF("addForcing: cMu=%f, cEps1=%f, cEps2=%f, sigmaEpsI=%f, sigmaKI=%f\n",
            //        cMu,cEps1,cEps2,sigmaEpsI,sigmaKI);
	    
            nut=nuT(I1,I2,I3)-nu;  // recall: nuT is nuTotal 
	    prod = nut*( 2.*(u0x*u0x+v0y*v0y) + SQR(v0x+u0y) );

	    // ::display(utLocal(I1,I2,I3,kc),"addForcing: utLocal(I1,I2,I3,kc) Before","%8.2e ");
	    
	    utLocal(I1,I2,I3,kc)-=-k0t -u0*k0x-v0*k0y +prod -e0
	      +(nu+sigmaKI*nut)*(k0xx+k0yy)+sigmaKI*(nuTx*k0x+nuTy*k0y);
   
	    // ::display(utLocal(I1,I2,I3,kc),"addForcing: utLocal(I1,I2,I3,kc) After","%8.2e ");

	    utLocal(I1,I2,I3,ec)-=-e0t -u0*e0x-v0*e0y +cEps1*(e0/k0)*prod-cEps2*(e0*e0/k0)
	       +(nu+sigmaEpsI*nut)*(e0xx+e0yy)+sigmaEpsI*(nuTx*e0x+nuTy*e0y);

	  }
	  
          if( solveForTemperature && useVariableDiffusivity )
	  {
            // variable diffusivity: 
	    if( debug() & 2 && t<=0. )
	      printF("***Cgins:addForcing for T eqn and variable diffusivity ***\n");
	    
            utLocal(I1,I2,I3,tc)+=( tet+ advectionCoefficient*(u0*tex+v0*tey) 
				    - kThermalLES*( nuT(I1,I2,I3)*(texx+teyy) + nuTx*tex + nuTy*tey ) );
	  }

	} // end use variable viscosity


        if( solveForTemperature )
	{ // add Temperature equation terms for Boussinesq or other approximations

          if( !useVariableDiffusivity )
	  {
	    
	    if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0  )
	    {
	      // -- Variable material properties ---
	      if( debug() & 2 && t<=0. )
		printF("***Cgins:addForcing for T eqn -- Variable material properties --\n");

	      if( parameters.dbase.get<real>("thermalConductivity") < 0. )
	      {
		printF("Cgins::addForcing:ERROR: thermalConductivity=%e is less than zero!\n",
                       parameters.dbase.get<real>("thermalConductivity"));
		OV_ABORT("error");
	      }

	      const int rhoc = parameters.dbase.get<int>("rhoc");
	      const int Cpc = parameters.dbase.get<int>("Cpc");
	      const int thermalKc = parameters.dbase.get<int>("thermalConductivityc");
	      assert( rhoc>=0 && Cpc>=0 && thermalKc>=0 );


	      realSerialArray rho(I1,I2,I3), Cp(I1,I2,I3), K(I1,I2,I3), Kx(I1,I2,I3), Ky(I1,I2,I3);
	      // Evaluate the material parameters: 
	      e.gd(rho,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,rhoc     ,t);
	      e.gd(Cp ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,Cpc      ,t);
	      e.gd(K  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,thermalKc,t);
	      e.gd(Kx ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,thermalKc,t);
	      e.gd(Ky ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,thermalKc,t);

	      utLocal(I1,I2,I3,tc)+=tet+ advectionCoefficient*(u0*tex+v0*tey) 
		- ( K*(texx+teyy) + Kx*tex + Ky*tey )/(rho*Cp) ;

	    }
	    else
	    {
	      // constant material properties
	      utLocal(I1,I2,I3,tc)+=tet+ advectionCoefficient*(u0*tex+v0*tey) - kThermal*(texx+teyy);
	    }
	    
	  }
	  
	  if( parameters.isAxisymmetric() )
	    utLocal(I1,I2,I3,tc)-= kTrOverR;

          if( debug() & 8 )
            fprintf(pDebugFile,"Ins:add TZ forcing gravity=[%g,%g]\n",gravity[0],gravity[1]);

          utLocal(I1,I2,I3,uc)+=thermalExpansivity*gravity[0]*te; 
          utLocal(I1,I2,I3,vc)+=thermalExpansivity*gravity[1]*te; 
	}

	if( pdeModel==InsParameters::twoPhaseFlowModel )
	{
          // forcing for two-phase flow model advected scalars

          te.redim(I1,I2,I3),tet.redim(I1,I2,I3),tex.redim(I1,I2,I3),tey.redim(I1,I2,I3);
          texx.redim(I1,I2,I3),teyy.redim(I1,I2,I3);
          assert( tc>=0 );

          // Evaluate the derivatives of phi : tc 
          e.gd( te  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);
	  e.gd( tet ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
	  e.gd( tex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( tey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
  	  e.gd( texx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( teyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);

	  utLocal(I1,I2,I3,tc)+=tet+u0*tex+v0*tey - adPsi*(texx+teyy);

          // Evaluate the derivatives of psi : nc 
          e.gd( te  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,nc,t);
	  e.gd( tet ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,nc,t);
	  e.gd( tex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,nc,t);
	  e.gd( tey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,nc,t);
  	  e.gd( texx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,nc,t);
	  e.gd( teyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,nc,t);

	  utLocal(I1,I2,I3,nc)+=tet+ advectionCoefficient*(u0*tex+v0*tey) - adPhi*(texx+teyy);


	  assert( !parameters.isAxisymmetric() );
	}
	
	
      }
      else if( mg.numberOfDimensions()==3 )
      {

	realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3),u0z(I1,I2,I3);
	realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3),v0z(I1,I2,I3);
	realSerialArray w0(I1,I2,I3),w0t(I1,I2,I3),w0x(I1,I2,I3),w0y(I1,I2,I3),w0z(I1,I2,I3);
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3),p0z(I1,I2,I3);
	
        realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0zz(I1,I2,I3);
        realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0zz(I1,I2,I3);
        realSerialArray w0xx(I1,I2,I3),w0yy(I1,I2,I3),w0zz(I1,I2,I3);

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);
	e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,uc,t);

	e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	e.gd( v0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,vc,t);
	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);
	e.gd( v0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,vc,t);

	e.gd( w0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,wc,t);
	e.gd( w0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,wc,t);
	e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
	e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
	e.gd( w0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,wc,t);
	e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,t);
	e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,t);
	e.gd( w0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,wc,t);


	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);
	e.gd( p0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,pc,t);
	if( pdeModel==InsParameters::twoPhaseFlowModel )
	{ // form grad(p)/rho 
	  // realMappedGridFunction & ue = *pue;
          #ifdef USE_PPP
	   realSerialArray ue; getLocalArrayWithGhostBoundaries(*pue,ue);
          #else	    
	   realSerialArray & ue = *pue;
	  #endif          
	  p0x/=ue(I1,I2,I3,rc);
	  p0y/=ue(I1,I2,I3,rc);
	  p0z/=ue(I1,I2,I3,rc);
	}


        utLocal(I1,I2,I3,uc)+=u0t+advectionCoefficient*(u0*u0x+v0*u0y+w0*u0z) + p0x; // -nu*(u0xx+u0yy+u0zz);
        utLocal(I1,I2,I3,vc)+=v0t+advectionCoefficient*(u0*v0x+v0*v0y+w0*v0z) + p0y; // -nu*(v0xx+v0yy+v0zz);
        utLocal(I1,I2,I3,wc)+=w0t+advectionCoefficient*(u0*w0x+v0*w0y+w0*w0z) + p0z; // -nu*(w0xx+w0yy+w0zz);

        if( turbulenceModel==InsParameters::noTurbulenceModel &&
            (pdeModel==InsParameters::standardModel ||
             pdeModel==InsParameters::BoussinesqModel) )
	{
	  utLocal(I1,I2,I3,uc)-= nu*(u0xx+u0yy+u0zz);
	  utLocal(I1,I2,I3,vc)-= nu*(v0xx+v0yy+v0zz);
	  utLocal(I1,I2,I3,wc)-= nu*(w0xx+w0yy+w0zz);
	}

        RealArray te,tet,tex,tey,tez, texx,teyy,tezz;
	if( solveForTemperature )
	{
          // Evaluate the derivative of T and save in te, tex, texx, ...

          te.redim(I1,I2,I3); tet.redim(I1,I2,I3); tex.redim(I1,I2,I3); tey.redim(I1,I2,I3); tez.redim(I1,I2,I3);
          texx.redim(I1,I2,I3); teyy.redim(I1,I2,I3); tezz.redim(I1,I2,I3);
          assert( tc>=0 );

          e.gd( te  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);
	  e.gd( tet ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
	  e.gd( tex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( tey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
	  e.gd( tez ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,tc,t);
  	  e.gd( texx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( teyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);
	  e.gd( tezz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,tc,t);
	}


        if( useVariableViscosity )
	{ 	
	  // --- variable viscosity ---

          assert( pnuT!=NULL );
          #ifdef USE_PPP
            realSerialArray nuT; getLocalArrayWithGhostBoundaries(*pnuT,nuT);
          #else
            realSerialArray & nuT = *pnuT;
          #endif

	  realSerialArray nuTx(I1,I2,I3),nuTy(I1,I2,I3),nuTz(I1,I2,I3);

          // We just compute the derivatives of the exact nuT using differences. 
          assert( gf[0].u[grid].getOperators()!=NULL );
	  MappedGridOperators & op = *(gf[0].u[grid].getOperators()); // --------- fix this 
          op.derivative(MappedGridOperators::xDerivative ,nuT,nuTx,I1,I2,I3,0);
          op.derivative(MappedGridOperators::yDerivative ,nuT,nuTy,I1,I2,I3,0);
          op.derivative(MappedGridOperators::zDerivative ,nuT,nuTz,I1,I2,I3,0);


	  if( pdeModel==InsParameters::twoPhaseFlowModel )
	  {
	    // realMappedGridFunction & ue = *pue;
            #ifdef USE_PPP
	     realSerialArray ue; getLocalArrayWithGhostBoundaries(*pue,ue);
            #else	    
	     realSerialArray & ue = *pue;
	    #endif          
	    utLocal(I1,I2,I3,uc)-=( (nuT(I1,I2,I3)*(u0xx+u0yy+u0zz)+nuTx*(2.*u0x )+
				     nuTy*(u0y+v0x)+nuTz*(u0z+w0x))/ue(I1,I2,I3,rc)
				   +gravity[0]);
	    utLocal(I1,I2,I3,vc)-=( (nuT(I1,I2,I3)*(v0xx+v0yy+v0zz)+nuTx*(u0y+v0x)+
				     nuTy*(2.*v0y )+nuTz*(v0z+w0y))/ue(I1,I2,I3,rc)
                                   +gravity[1]);
	    utLocal(I1,I2,I3,wc)-=( (nuT(I1,I2,I3)*(w0xx+w0yy+w0zz)+nuTx*(u0z+w0x)+
				     nuTy*(v0z+w0y)+nuTz*(2.*w0z ))/ue(I1,I2,I3,rc)
				    +gravity[2]);
	  }
	  else
	  {
	    utLocal(I1,I2,I3,uc)-=nuT(I1,I2,I3)*(u0xx+u0yy+u0zz)+nuTx*(2.*u0x )+nuTy*(u0y+v0x)+nuTz*(u0z+w0x);
	    utLocal(I1,I2,I3,vc)-=nuT(I1,I2,I3)*(v0xx+v0yy+v0zz)+nuTx*(u0y+v0x)+nuTy*(2.*v0y )+nuTz*(v0z+w0y);
	    utLocal(I1,I2,I3,wc)-=nuT(I1,I2,I3)*(w0xx+w0yy+w0zz)+nuTx*(u0z+w0x)+nuTy*(v0z+w0y)+nuTz*(2.*w0z );
	  }

	  if( turbulenceModel==InsParameters::kEpsilon )
	  {
            RealArray nut(I1,I2,I3), prod(I1,I2,I3);
	    RealArray k0(I1,I2,I3),k0t(I1,I2,I3),k0x(I1,I2,I3),k0y(I1,I2,I3),k0xx(I1,I2,I3),k0yy(I1,I2,I3);
            RealArray k0z(I1,I2,I3),k0zz(I1,I2,I3);
	    e.gd( k0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,kc,t);
	    e.gd( k0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,kc,t);
	    e.gd( k0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,kc,t);
	    e.gd( k0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,kc,t);
	    e.gd( k0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,kc,t);
	    e.gd( k0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,kc,t);
	    e.gd( k0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,kc,t);
	    e.gd( k0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,kc,t);

	    RealArray e0(I1,I2,I3),e0t(I1,I2,I3),e0x(I1,I2,I3),e0y(I1,I2,I3),e0xx(I1,I2,I3),e0yy(I1,I2,I3);
            RealArray e0z(I1,I2,I3),e0zz(I1,I2,I3);
	    e.gd( e0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,ec,t);
	    e.gd( e0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,ec,t);
	    e.gd( e0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,ec,t);
	    e.gd( e0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,ec,t);
	    e.gd( e0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,ec,t);
	    e.gd( e0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,ec,t);
	    e.gd( e0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,ec,t);
	    e.gd( e0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,ec,t);


            nut=nuT(I1,I2,I3)-nu; // recall: nuT is nuTotal 
	    prod = nut*( 2.*(u0x*u0x+v0y*v0y+w0z*w0z) + SQR(v0x+u0y)+ SQR(w0y+v0z)+ SQR(u0z+w0x) );

	    utLocal(I1,I2,I3,kc)-=-k0t -u0*k0x-v0*k0y-w0*k0z +prod -e0
	      +(nu+sigmaKI*nut)*(k0xx+k0yy+k0zz)+sigmaKI*(nuTx*k0x+nuTy*k0y+nuTz*k0z);
   
	    utLocal(I1,I2,I3,ec)-=-e0t -u0*e0x-v0*e0y-w0*e0z +cEps1*(e0/k0)*prod -cEps2*(e0*e0/k0)
	       +(nu+sigmaEpsI*nut)*(e0xx+e0yy+e0zz)+sigmaEpsI*(nuTx*e0x+nuTy*e0y+nuTz*e0z);

	  }	  
	  
          if( solveForTemperature && useVariableDiffusivity )
	  {
            // variable diffusivity: 
            utLocal(I1,I2,I3,tc)+=( tet+ advectionCoefficient*(u0*tex+v0*tey+w0*tez) 
				    - kThermalLES*( nuT(I1,I2,I3)*(texx+teyy+tezz) +nuTx*tex +nuTy*tey +nuTz*tez ) );

	  }      

	} // end use variable viscosity
	

	

        if( solveForTemperature )
	{ // add Temperature terms for Boussinesq or other approximations

          assert( tc>=0 );

          if( !useVariableDiffusivity )
	  {
	    if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0  )
	    {
	      // -- Variable material properties ---
	      if( debug() & 2 && t<=0. )
		printF("***Cgins:addForcing for T eqn -- Variable material properties --\n");

	      const int rhoc = parameters.dbase.get<int>("rhoc");
	      const int Cpc = parameters.dbase.get<int>("Cpc");
	      const int thermalKc = parameters.dbase.get<int>("thermalConductivityc");
	      assert( rhoc>=0 && Cpc>=0 && thermalKc>=0 );


	      realSerialArray rho(I1,I2,I3), Cp(I1,I2,I3), K(I1,I2,I3), Kx(I1,I2,I3), Ky(I1,I2,I3), Kz(I1,I2,I3);
	      // Evaluate the material parameters: 
	      e.gd(rho,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,rhoc     ,t);
	      e.gd(Cp ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,Cpc      ,t);
	      e.gd(K  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,thermalKc,t);
	      e.gd(Kx ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,thermalKc,t);
	      e.gd(Ky ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,thermalKc,t);
	      e.gd(Kz ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,thermalKc,t);

	      utLocal(I1,I2,I3,tc)+=tet+ advectionCoefficient*(u0*tex+v0*tey+w0*tez)
		- ( K*(texx+teyy+tezz) + Kx*tex + Ky*tey + Kz*tez )/(rho*Cp) ;

	    }
	    else
	    {
	      // constant material properties
	      utLocal(I1,I2,I3,tc)+=tet+ advectionCoefficient*(u0*tex+v0*tey+w0*tez) - kThermal*(texx+teyy+tezz);
	    }
	    
	  }
	  
          utLocal(I1,I2,I3,uc)+=thermalExpansivity*gravity[0]*te; 
          utLocal(I1,I2,I3,vc)+=thermalExpansivity*gravity[1]*te; 
          utLocal(I1,I2,I3,wc)+=thermalExpansivity*gravity[2]*te; 
	}

	if( pdeModel==InsParameters::twoPhaseFlowModel )
	{
          // over-write u0t, u0x, ...
	  e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
	  e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
	  e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,tc,t);
	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);
	  e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,tc,t);

	  utLocal(I1,I2,I3,tc)+=u0t+ advectionCoefficient*(u0*u0x+v0*u0y+w0*u0z) - adPsi*(u0xx+u0yy+u0zz);

	  e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,nc,t);
	  e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,nc,t);
	  e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,nc,t);
	  e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,nc,t);
	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,nc,t);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,nc,t);
	  e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,nc,t);
	  
	  utLocal(I1,I2,I3,nc)+=u0t+u0*u0x+v0*u0y+w0*u0z - adPhi*(u0xx+u0yy+u0zz);
	  
	}
	
	
      }
      else
      {
	Overture::abort("error");
      }

      if ( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization )
	{
#ifdef USE_PPP
	Overture::abort("fix me kyle!");
#else

	  // ND useOpt==true
	  real dt = rparam[3];
	  int wc =  cg.numberOfDimensions()==2 ? vc : parameters.dbase.get<int >("wc");
	  int tc =  parameters.dbase.get<InsParameters::PDEModel >("pdeModel")!=InsParameters::BoussinesqModel ? wc : parameters.dbase.get<int >("tc");
	  
	  Range C(uc,tc); // !!! this line relies on tc=wc if not Boussinesq and wc=vc if 2D
	  // for dirichlet boundary conditions the boundary point equation sits on the ghost point line
	  //	  ut.display("ut before dirichlet bdy adjustment");
// 	  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
// 	    {
// 	      for( int side=Start; side<=End; side++ )
// 		{
// 		  if( mg.boundaryCondition(side,axis)==Parameters::dirichletBoundaryCondition ||
// 		      mg.boundaryCondition(side,axis)==Parameters::noSlipWall )
// 		    {
// 		      Index Ib1,Ib2,Ib3;
// 		      getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3);
// 		      Index Ig1,Ig2,Ig3;
// 		      getGhostIndex(mg.indexRange(),side,axis,Ig1,Ig2,Ig3);
		      
// 		      ut(Ig1,Ig2,Ig3,C) = 0.;
// 		    }
// 		}
// 	    }

	  // Notes from wdh: 
          ///  The AFC scheme solves the time derivative of the BC:
          //      u=g ->   Du/dt =Dg/Dt + f 
          // On a moving grid we need to add in the moving grid terms.
	  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
		{
		  if( mg.boundaryCondition(side,axis)==Parameters::dirichletBoundaryCondition ||
		      mg.boundaryCondition(side,axis)==Parameters::noSlipWall )
		    {
		      Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
		      getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3);

		      for ( int cc=uc; cc<=C.getBound(); cc++ )
			ut(Ib1,Ib2,Ib3,cc) = e.t(mg,Ib1,Ib2,Ib3,cc,t);

		      if ( referenceFrameVelocity )
			{
			  if ( mg.numberOfDimensions()==3 )
			    {
			      for ( int cc=uc; cc<=C.getBound(); cc++ )
				ut(Ib1,Ib2,Ib3,cc) += gridVelocityLocal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,cc,t)+gridVelocityLocal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,cc,t) + gridVelocityLocal(Ib1,Ib2,Ib3,2)*e.z(mg,Ib1,Ib2,Ib3,cc,t);
			    }
			  else
			    {
			      for ( int cc=uc; cc<=C.getBound(); cc++ )
				ut(Ib1,Ib2,Ib3,cc) += gridVelocityLocal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,cc,t)+gridVelocityLocal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,cc,t);
			    }
			  // kkc 120918, what the heck were the following 3 lines here for?
			  // kkc 120921, ohhhh, removing the grid acceleration in the tw case, but it is never added in the tw case now so it no longer needs to be here...
			  //real dtfac = t>t0 ? 2./dt : -2./dt;
			  //Range V(uc,uc+mg.numberOfDimensions()-1), R(0,mg.numberOfDimensions()-1);
			  //ut(Ib1,Ib2,Ib3,V) += dtfac*gridVelocityLocal(Ib1,Ib2,Ib3,R);
			}

		    }
// 		  else if( mg.boundaryCondition(side,axis)==Parameters::neumannBoundaryCondition )
// 		    {
// 		      Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
// 		      getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3);
// 		      RealArray &normal = mg.vertexBoundaryNormal(side,axis);
// 		      for ( int cc=C.getBase(); cc<=C.getBound(); cc++ )
// 			ut(Ib1,Ib2,Ib3,cc) = (2*side-1)*(normal(Ib1,Ib2,Ib3,0)*(e.x(mg,Ib1,Ib2,Ib3,cc,t0+dt)-e.x(mg,Ib1,Ib2,Ib3,cc,t0)) +
// 					      normal(Ib1,Ib2,Ib3,1)*(e.y(mg,Ib1,Ib2,Ib3,cc,t0+dt)-e.y(mg,Ib1,Ib2,Ib3,cc,t0)))/dt;
// 		      if ( mg.numberOfDimensions()==3 )
// 			for ( int cc=C.getBase(); cc<=C.getBound(); cc++ )
// 			  ut(Ib1,Ib2,Ib3,cc) += (2*side-1)*normal(Ib1,Ib2,Ib3,2)*(e.z(mg,Ib1,Ib2,Ib3,cc,t0+dt)-e.z(mg,Ib1,Ib2,Ib3,cc,t0))/dt;
		      
// 		    }
		}
	    }
	  Index I1,I2,I3;
	  getIndex(mg.dimension(),I1,I2,I3);
	  for ( int cc=C.getBase(); cc<=C.getBound(); cc++ )
	    {
	      where( mg.mask()(I1,I2,I3) <= 0 )
		{
		  ut(I1,I2,I3,cc) = 0.;
		}
	    }
#endif
	} // approximate factorization bc forcing
      

      delete pnuT;
      delete pue;
      return;

    }  // end useOpt ---------------------------------------------------------------
    

    // *** old way ***

    if( mg.numberOfDimensions()==1 )
    {
      // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 
      where( mg.mask()(I1,I2,I3) != 0 )
      {
	ut(I1,I2,I3,uc)+= uvt(uc)+advectionCoefficient*(uv(uc)*uvx(uc)) +uvx(pc)-nu*(uvxx(uc));
      }
    }
    else if( mg.numberOfDimensions()==2 )
    {
      if( evaluateTZ )
      { // evaluate the TZ functions
        
	realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3);
	realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	
        realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3);
        realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3);

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);


	tzForcing[0](I1,I2,I3) = u0t; 
	tzForcing[1](I1,I2,I3) = u0*u0x+v0*u0y;
	tzForcing[2](I1,I2,I3) = p0x;

	tzForcing[3] = v0t; 
	tzForcing[4] = u0*v0x+v0*v0y;
	tzForcing[5] = p0y;
       

	tzForcing[6] = u0xx+u0yy; 
	tzForcing[7] = v0xx+v0yy; 

      }
      

      // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 
      // where( mg.mask()(I1,I2,I3) != 0 )

//        ut(I1,I2,I3,uc)+= uvt(uc)+advectionCoefficient*(u0*u0x+v0*u0y) +p0x;
//        ut(I1,I2,I3,vc)+= uvt(vc)+advectionCoefficient*(u0*v0x+v0*v0y) +p0y;

      if( debug() & 64 )
      {
	fprintf(parameters.dbase.get<FILE* >("debugFile")," *** evaluateTZ=%i *****\n",evaluateTZ);
	display(mg.vertex()(I1,I2,I3,0),sPrintF(" mg.vertex(I1,I2,I3,0) at t=%e",t),parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");

	display(ut(I1,I2,I3,uc),sPrintF("ut(I1,I2,I3,uc) before adding TZ at t=%e\n",t),parameters.dbase.get<FILE* >("debugFile"),"%7.4f ");
	display(tzForcing[1](I1,I2,I3)+tzForcing[2](I1,I2,I3),
	          sPrintF("u*ux+px (TZ) at t=%e\n",t),parameters.dbase.get<FILE* >("debugFile"),"%6.4f ");
      }
      
      utLocal(I1,I2,I3,uc)+= scaleFactorT*tzForcing[0](I1,I2,I3)+
                        (advectionCoefficient*SQR(scaleFactor))*tzForcing[1](I1,I2,I3)+
                        scaleFactor*tzForcing[2](I1,I2,I3);
      utLocal(I1,I2,I3,vc)+= scaleFactorT*tzForcing[3]
                       +(advectionCoefficient*SQR(scaleFactor))*tzForcing[4]+scaleFactor*tzForcing[5];

// *********************** start 123
#ifndef USE_PPP
      if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ||
          parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
      {
	const realArray & u0  = e   (mg,I1,I2,I3,uc,t); // this could be optimized
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);

//  	realArray adc;
//          adc = ad21+cd22*(fabs(u0x)+fabs(u0y)+fabs(v0x)+fabs(v0y)); // ** coeff of art. dissipation

//  	ut(I1,I2,I3,uc)-=adc*(e(mg,I1-1,I2,I3,uc,t)+e(mg,I1+1,I2,I3,uc,t)+
//                                e(mg,I1,I2-1,I3,uc,t)+e(mg,I1,I2+1,I3,uc,t)-4.*u0);
	
//  	ut(I1,I2,I3,vc)-=adc*(e(mg,I1-1,I2,I3,vc,t)+e(mg,I1+1,I2,I3,vc,t)+
//                                e(mg,I1,I2-1,I3,vc,t)+e(mg,I1,I2+1,I3,vc,t)-4.*v0);
	realArray adc;
        adc = fabs(u0x)+fabs(u0y)+fabs(v0x)+fabs(v0y);
        if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
	{
#undef ADTZ2
#define ADTZ2(cc,q0) adc2*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		           e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t)+\
			    -4.*q0 )
          realArray adc2;
	  adc2 = ad21+cd22*adc;
	  ut(I1,I2,I3,uc)-=ADTZ2(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ2(vc,v0);
	}
        if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
	{
#undef ADTZ4
#define ADTZ4(cc,q0) adc4*(-(e(mg,I1-2,I2,I3,cc,t)+e(mg,I1+2,I2,I3,cc,t)+\
		 	     e(mg,I1,I2-2,I3,cc,t)+e(mg,I1,I2+2,I3,cc,t))\
		        +4.*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		             e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t))\
			    -12.*q0 )

          realArray adc4;
	  adc4 = ad41+cd42*adc;
	  ut(I1,I2,I3,uc)-=ADTZ4(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ4(vc,v0);
	}
      }
      

      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
      {
	realArray radiusInverse;
	realArray & vertex = mg.vertex();
	if( nu!=0. )
	{
	  realArray nuUrOverR, nuVrOverR;
	  if( parameters.isAxisymmetric() )
	  {
	    const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);

	    const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	    const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);

  	    const realArray & u0yy= e.yy(mg,I1,I2,I3,uc,t);
  	    const realArray & v0yy= e.yy(mg,I1,I2,I3,vc,t);

	    // y corresponds to the radial direction
	    // y=0 is the axis of symmetry
	    realArray radiusInverse;
	    radiusInverse=1./max(REAL_MIN,vertex(I1,I2,I3,axis2));

	    nuUrOverR=nu*u0y*radiusInverse;
	    nuVrOverR=nu*(v0y-v0*radiusInverse)*radiusInverse;

	    // fix points on axis
	    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
		if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
		{
		  Index Ib1,Ib2,Ib3;
		  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  nuUrOverR(Ib1,Ib2,Ib3)=nu*u0yy(Ib1,Ib2,Ib3);
		  nuVrOverR(Ib1,Ib2,Ib3)=.5*nu*v0yy(Ib1,Ib2,Ib3);
	  
		}
	      }
	    }
	  }

	  ut(I1,I2,I3,uc)-= (nu*scaleFactor)*tzForcing[6];
	  ut(I1,I2,I3,vc)-= (nu*scaleFactor)*tzForcing[7];
	  if( parameters.isAxisymmetric() )
	  {
	    ut(I1,I2,I3,uc)-= nuUrOverR;
	    ut(I1,I2,I3,vc)-= nuVrOverR;
	  }
	}
      }
      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
        // here are the SA TM equations 

        assert( !parameters.isAxisymmetric() );
	assert( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL );
	const realArray & d = (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];
	
        const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
        const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
        const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);

        const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
        const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
        const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);

	const realArray & n0   = e  (mg,I1,I2,I3,nc,t);
	const realArray & n0x  = e.x(mg,I1,I2,I3,nc,t);
	const realArray & n0y  = e.y(mg,I1,I2,I3,nc,t);
        

        realArray nuT,chi3,nuTx,nuTy,nuTd;
        chi3 = pow(n0/nu,3.);

        // chi3=0.;  // *******************

	nuT = nu+n0*(chi3/(chi3+cv1e3)); // ******************
        nuTd=chi3*(chi3+4.*cv1e3)/pow(chi3+cv1e3,2.);
	nuTx= n0x*nuTd;// ******************
	nuTy= n0y*nuTd;// ******************
	
	ut(I1,I2,I3,uc)-=nuT*(e.laplacian(mg,I1,I2,I3,uc,t))+nuTx*(2.*u0x )+nuTy*(u0y+v0x);
	ut(I1,I2,I3,vc)-=nuT*(e.laplacian(mg,I1,I2,I3,vc,t))+nuTx*(u0y+v0x)+nuTy*(2.*v0y);
	
	realArray s, r,g,fw,fnu1(I1,I2,I3),fnu2(I1,I2,I3), chi, dSq(I1,I2,I3);

	chi=n0/nu;
	fnu1=chi*chi*chi/( chi*chi*chi+cv1e3);
	fnu2=1.-chi/(1.+chi*fnu1);

	if( mg.numberOfDimensions()==2 )
	{
	  s= fabs(u0y-v0x); // turbulence source term
	}
	else
	{
	  // s=SQRT( SQR(u0y-v0x) + SQR(v0z-w0y) + SQR(w0x-u0z) );
	}
    
	// const real epsD=1.e-20;
	// d(I1,I2,I3)=max(d(I1,I2,I3),epsD);
	dSq=(d(I1,I2,I3)+cd0)*(d(I1,I2,I3)+cd0);
      
	s+= n0*fnu2/( dSq*(kappa*kappa) );
      
        // we could assume that d is set to a nonzero value on the boundary.
	// ** r= n0/( max( s*dSq*(kappa*kappa), cr0 ) );

	// we assume that g reaches a constant for r large enough
	r = min( n0/( s*dSq*(kappa*kappa) ), cr0 );

	g=r+cw2*(pow(r,6.)-r);
	fw=g*pow( (1.+cw3e6)/(pow(g,6.)+cw3e6), 1./6.);

	realArray nSqBydSq;
	nSqBydSq=cw1*fw*(n0*n0/dSq);

        if( debug() & 8 )
	{
	  printf("addTZ: max(cb1*s*n0)=%9.2e, max(nSqBydSq)=%9.2e max(ut(nc))=%9.2e (before)",
                max(fabs(cb1*s*n0)),max(fabs(nSqBydSq)),max(fabs(ut(I1,I2,I3,nc))));
	}
	
	
	ut(I1,I2,I3,nc)+=e.t(mg,I1,I2,I3,nc,t) +u0*n0x+v0*n0y
	  - sigmai*(nu+n0)*(e.laplacian(mg,I1,I2,I3,nc,t))
          - ((1.+cb2)*sigmai)*(n0x*n0x+n0y*n0y)
          - cb1*s*n0 + nSqBydSq 
          -( (ad21n+cd22n*( fabs(n0x)+fabs(n0y) ))*(
	    e(mg,I1-1,I2,I3,nc,t)+e(mg,I1+1,I2,I3,nc,t)+e(mg,I1,I2-1,I3,nc,t)+e(mg,I1,I2+1,I3,nc,t)-4*n0) );
	

        if( debug() & 8 )
	{
	  printf("  max(ut(nc))=%9.2e (after)\n",max(fabs(ut(I1,I2,I3,nc))));
	}

      }
//       else  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
//       {
//         assert( !parameters.isAxisymmetric() );
// 	assert( kc>0 && ec>0 );

//         const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
//         const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
//         const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
//         const realArray & u0Lap= e.laplacian(mg,I1,I2,I3,uc,t);

//         const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
//         const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
//         const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
//         const realArray & v0Lap= e.laplacian(mg,I1,I2,I3,vc,t);

// 	const realArray & k0   = e  (mg,I1,I2,I3,kc,t);
// 	const realArray & k0x  = e.x(mg,I1,I2,I3,kc,t);
// 	const realArray & k0y  = e.y(mg,I1,I2,I3,kc,t);
//         const realArray & k0Lap= e.laplacian(mg,I1,I2,I3,kc,t);

// 	const realArray & e0   = e  (mg,I1,I2,I3,ec,t);
// 	const realArray & e0x  = e.x(mg,I1,I2,I3,ec,t);
// 	const realArray & e0y  = e.y(mg,I1,I2,I3,ec,t);
//         const realArray & e0Lap= e.laplacian(mg,I1,I2,I3,ec,t);
        

//         realArray nuT,nuTx,nuTy,nuP,e02,prod;

// 	e02=e0*e0;
	
// 	nuT = cMu*k0*k0/e0;
// 	nuP=nu+nuT;
// 	nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e02;
// 	nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e02;


// 	ut(I1,I2,I3,uc)-=nuP*u0Lap+nuTx*(2.*u0x )+nuTy*(u0y+v0x);
// 	ut(I1,I2,I3,vc)-=nuP*v0Lap+nuTx*(u0y+v0x)+nuTy*(2.*v0y);
	
// 	prod = nuT*( 2.*(u0x*u0x+v0y*v0y) + SQR(v0x+u0y) );

// //          printf("insTZ: kc,ec,epsc=%2i,%2i,%2i cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,\n",
// //  	       kc,ec,epsc,cMu,cEps1,cEps2,sigmaEpsI,sigmaKI);
	
// 	ut(I1,I2,I3,kc)-=-e.t(mg,I1,I2,I3,kc,t) -u0*k0x-v0*k0y +prod -e0
//             +(nu+sigmaKI*nuT)*k0Lap+sigmaKI*(nuTx*k0x+nuTy*k0y);
   
// 	ut(I1,I2,I3,ec)-=-e.t(mg,I1,I2,I3,ec,t) -u0*e0x-v0*e0y
//            +cEps1*(e0/k0)*prod-cEps2*(e02/k0) +(nu+sigmaEpsI*nuT)*e0Lap+sigmaEpsI*(nuTx*e0x+nuTy*e0y);
   
//       }
      else 
      {
	if( parameters.dbase.get<int >("myid")==0 )
          printf("Unknown turbulenceModel!\n");
	Overture::abort();
      }

      if ( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization )
	{
	  Overture::abort("ApproximateFactorization should always use useOpt section of ins addForcing");
	} // approximate factorization bc forcing
    }
    else if( mg.numberOfDimensions()==3 )
    {
      if( evaluateTZ )
      {
	const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);

	const realArray & p0x = e.x (mg,I1,I2,I3,pc,t);
	const realArray & p0y = e.y (mg,I1,I2,I3,pc,t);
	const realArray & p0z = e.z (mg,I1,I2,I3,pc,t);

	tzForcing[0](I1,I2,I3) = e.t (mg,I1,I2,I3,uc,t);
	tzForcing[1](I1,I2,I3) = u0*u0x+v0*u0y+w0*u0z;
	tzForcing[2](I1,I2,I3) = p0x;

	tzForcing[3] = e.t (mg,I1,I2,I3,vc,t);
	tzForcing[4] = u0*v0x+v0*v0y+w0*v0z;
	tzForcing[5] = p0y;

	tzForcing[6] = e.t (mg,I1,I2,I3,wc,t);
	tzForcing[7] = u0*w0x+v0*w0y+w0*w0z;
	tzForcing[8] = p0z;
       
	tzForcing[ 9] = e.laplacian(mg,I1,I2,I3,uc,t);
	tzForcing[10] = e.laplacian(mg,I1,I2,I3,vc,t);
	tzForcing[11] = e.laplacian(mg,I1,I2,I3,wc,t);

      }
      
//        ut(I1,I2,I3,uc)+= uvt(uc)+advectionCoefficient*(u0*u0x+v0*u0y+w0*u0z) +p0x;
//        ut(I1,I2,I3,vc)+= uvt(vc)+advectionCoefficient*(u0*v0x+v0*v0y+w0*v0z) +p0y;
//        ut(I1,I2,I3,wc)+= uvt(wc)+advectionCoefficient*(u0*w0x+v0*w0y+w0*w0z) +p0z;

      ut(I1,I2,I3,uc)+= scaleFactorT*tzForcing[0](I1,I2,I3)+
                       (advectionCoefficient*SQR(scaleFactor))*tzForcing[1](I1,I2,I3)+
                        scaleFactor*tzForcing[2](I1,I2,I3);
      ut(I1,I2,I3,vc)+= scaleFactorT*tzForcing[3]
                       +(advectionCoefficient*SQR(scaleFactor))*tzForcing[4]+scaleFactor*tzForcing[5];
      ut(I1,I2,I3,wc)+= scaleFactorT*tzForcing[6]
                       +(advectionCoefficient*SQR(scaleFactor))*tzForcing[7]+scaleFactor*tzForcing[8];



      if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ||
          parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
      {
	const realArray & u0  = e   (mg,I1,I2,I3,uc,t); // this could be optimized
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);

	realArray adc;
        adc = (fabs(u0x)+fabs(u0y)+fabs(u0z)+
	       fabs(v0x)+fabs(v0y)+fabs(v0z)+
	       fabs(w0x)+fabs(w0y)+fabs(w0z));
        if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
	{
#undef ADTZ2
#define ADTZ2(cc,q0) adc2*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		           e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t)+\
			   e(mg,I1,I2,I3-1,cc,t)+e(mg,I1,I2,I3+1,cc,t)\
			    -6.*q0 )
          realArray adc2;
	  adc2 = ad21+cd22*adc;
	  ut(I1,I2,I3,uc)-=ADTZ2(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ2(vc,v0);
	  ut(I1,I2,I3,wc)-=ADTZ2(wc,w0);
	  
	}
        if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
	{
#undef ADTZ4
#define ADTZ4(cc,q0) adc4*(-(e(mg,I1-2,I2,I3,cc,t)+e(mg,I1+2,I2,I3,cc,t)+\
		 	     e(mg,I1,I2-2,I3,cc,t)+e(mg,I1,I2+2,I3,cc,t)+\
			     e(mg,I1,I2,I3-2,cc,t)+e(mg,I1,I2,I3+2,cc,t))\
		        +4.*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		             e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t)+\
			     e(mg,I1,I2,I3-1,cc,t)+e(mg,I1,I2,I3+1,cc,t))\
			    -18.*q0 )

          realArray adc4;
	  adc4 = ad41+cd42*adc;
	  ut(I1,I2,I3,uc)-=ADTZ4(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ4(vc,v0);
	  ut(I1,I2,I3,wc)-=ADTZ4(wc,w0);
	}
      }
      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
      {
	if( nu!=0. )
	{
	  ut(I1,I2,I3,uc)-= (nu*scaleFactor)*tzForcing[ 9];
	  ut(I1,I2,I3,vc)-= (nu*scaleFactor)*tzForcing[10];
	  ut(I1,I2,I3,wc)-= (nu*scaleFactor)*tzForcing[11];
	}
      }
      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
        // here are the SA TM equations without all the source terms -- these are just turned off for TZ
	assert( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL );
	const realArray & d = (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];

	const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);

	const realArray & n0   = e  (mg,I1,I2,I3,nc,t);
	const realArray & n0x  = e.x(mg,I1,I2,I3,nc,t);
	const realArray & n0y  = e.y(mg,I1,I2,I3,nc,t);
	const realArray & n0z  = e.z(mg,I1,I2,I3,nc,t);

        realArray nuT,chi3,nuTx,nuTy,nuTz,nuTd;
        chi3 = pow(n0/nu,3.);
	
	nuT = nu+n0*(chi3/(chi3+cv1e3));
        nuTd=chi3*(chi3+4.*cv1e3)/pow(chi3+cv1e3,2.);

	nuTx= n0x*nuTd;
	nuTy= n0y*nuTd;
	nuTz= n0z*nuTd;
	
	ut(I1,I2,I3,uc)-=nuT*(e.laplacian(mg,I1,I2,I3,uc,t))+nuTx*(2.*u0x )+nuTy*(u0y+v0x)+nuTz*(u0z+w0x);
	ut(I1,I2,I3,vc)-=nuT*(e.laplacian(mg,I1,I2,I3,vc,t))+nuTx*(u0y+v0x)+nuTy*(2.*v0y )+nuTz*(v0z+w0y);
	ut(I1,I2,I3,wc)-=nuT*(e.laplacian(mg,I1,I2,I3,wc,t))+nuTx*(u0z+w0x)+nuTy*(v0z+w0y)+nuTz*(2.*w0z );

	realArray s, r,g,fw,fnu1(I1,I2,I3),fnu2(I1,I2,I3), chi, dSq(I1,I2,I3);

	chi=n0/nu;
	fnu1=chi*chi*chi/( chi*chi*chi+cv1e3);
	fnu2=1.-chi/(1.+chi*fnu1);

	s=SQRT( SQR(u0y-v0x) + SQR(v0z-w0y) + SQR(w0x-u0z) );
    
//	const real epsD=1.e-20;
//	d(I1,I2,I3)=max(d(I1,I2,I3),epsD);
//	dSq=d(I1,I2,I3)*d(I1,I2,I3);
	dSq=(d(I1,I2,I3)+cd0)*(d(I1,I2,I3)+cd0);
	
	s+= n0*fnu2/( dSq*(kappa*kappa) );
      
        // we could assume that d is set to a nonzero value on the boundary.
	// ** r= n0/( max( s*dSq*(kappa*kappa), cr0 ) );

       	// we assume that g reaches a constant for r large enough
        r = min( n0/( s*dSq*(kappa*kappa) ), cr0 );

	g=r+cw2*(pow(r,6.)-r);
	fw=g*pow( (1.+cw3e6)/(pow(g,6.)+cw3e6), 1./6.);

	realArray nSqBydSq;
	nSqBydSq=cw1*fw*(n0*n0/dSq);

        if( debug() & 2 )
	{
	  printf("addTZ: max(cb1*s*n0)=%9.2e, max(nSqBydSq)=%9.2e max(ut(nc))=%9.2e (before)",
                max(fabs(cb1*s*n0)),max(fabs(nSqBydSq)),max(fabs(ut(I1,I2,I3,nc))));
	}
	
	ut(I1,I2,I3,nc)+=e.t(mg,I1,I2,I3,nc,t) +u0*n0x+v0*n0y+w0*n0z
	  - sigmai*(nu+n0)*(e.xx(mg,I1,I2,I3,nc,t)+e.yy(mg,I1,I2,I3,nc,t)+e.zz(mg,I1,I2,I3,nc,t))
          - ((1.+cb2)*sigmai)*(n0x*n0x+n0y*n0y+n0z*n0z)
          - cb1*s*n0 + nSqBydSq
          -( (ad21n+cd22n*( fabs(n0x)+fabs(n0y)+fabs(n0z) ))*(
	    e(mg,I1-1,I2,I3,nc,t)+e(mg,I1+1,I2,I3,nc,t)+
            e(mg,I1,I2-1,I3,nc,t)+e(mg,I1,I2+1,I3,nc,t)+
            e(mg,I1,I2,I3-1,nc,t)+e(mg,I1,I2,I3-1,nc,t)
                                          -8.*n0) );


        if( debug() & 2 )
	{
	  printf("  max(ut(nc))=%9.2e (after)",max(fabs(ut(I1,I2,I3,nc))));
	}

      }
//       else  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
//       {
//         assert( !parameters.isAxisymmetric() );
// 	assert( kc>0 && ec>0 );

//         const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
//         const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
//         const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
//         const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);
//         const realArray & u0Lap= e.laplacian(mg,I1,I2,I3,uc,t);

//         const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
//         const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
//         const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
//         const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);
//         const realArray & v0Lap= e.laplacian(mg,I1,I2,I3,vc,t);

//         const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
//         const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
//         const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
//         const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);
//         const realArray & w0Lap= e.laplacian(mg,I1,I2,I3,wc,t);

// 	const realArray & k0   = e  (mg,I1,I2,I3,kc,t);
// 	const realArray & k0x  = e.x(mg,I1,I2,I3,kc,t);
// 	const realArray & k0y  = e.y(mg,I1,I2,I3,kc,t);
// 	const realArray & k0z  = e.z(mg,I1,I2,I3,kc,t);
//         const realArray & k0Lap= e.laplacian(mg,I1,I2,I3,kc,t);

// 	const realArray & e0   = e  (mg,I1,I2,I3,ec,t);
// 	const realArray & e0x  = e.x(mg,I1,I2,I3,ec,t);
// 	const realArray & e0y  = e.y(mg,I1,I2,I3,ec,t);
// 	const realArray & e0z  = e.z(mg,I1,I2,I3,ec,t);
//         const realArray & e0Lap= e.laplacian(mg,I1,I2,I3,ec,t);
        

//         realArray nuT,nuTx,nuTy,nuTz,nuP,e02,prod;

// 	e02=e0*e0;
	
// 	nuT = cMu*k0*k0/e0;
// 	nuP=nu+nuT;
// 	nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e02;
// 	nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e02;
// 	nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e02;


// 	ut(I1,I2,I3,uc)-=nuP*u0Lap+nuTx*(2.*u0x )+nuTy*(u0y+v0x)+nuTz*(u0z+w0x);
// 	ut(I1,I2,I3,vc)-=nuP*v0Lap+nuTx*(u0y+v0x)+nuTy*(2.*v0y )+nuTz*(v0z+w0y);
// 	ut(I1,I2,I3,wc)-=nuP*w0Lap+nuTx*(u0z+w0x)+nuTy*(v0z+w0y)+nuTz*(2.*w0z );

// 	prod = nuT*( 2.*(u0x*u0x+v0y*v0y+w0z*w0z) + SQR(v0x+u0y)+ SQR(w0y+v0z)+ SQR(u0z+w0x) );
	
//   	ut(I1,I2,I3,kc)-=-e.t(mg,I1,I2,I3,kc,t) -u0*k0x-v0*k0y-w0*k0z+prod -e0
//   	  +(nu+sigmaKI*nuT)*k0Lap+sigmaKI*(nuTx*k0x+nuTy*k0y+nuTz*k0z);
   
//   	ut(I1,I2,I3,ec)-=-e.t(mg,I1,I2,I3,ec,t) -u0*e0x-v0*e0y-w0*e0z+cEps1*(e0/k0)*prod-cEps2*(e02/k0)
//   	  +(nu+sigmaEpsI*nuT)*e0Lap+sigmaEpsI*(nuTx*e0x+nuTy*e0y+nuTz*e0z);
   
//       }
      else 
      {
	printf("Unknown turbulenceModel!\n");
	Overture::abort();
      }

      if ( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization )
	{
	  Overture::abort("ApproximateFactorization should always use useOpt section of ins addForcing");
	} // approximate factorization bc forcing

#endif
// *********************** end 123
    }
    else
    {
      printF("Cgins::addForcing:ERROR: unknown dimension\n");
      OV_ABORT("error");
    }

    delete pnuT;
    
  } // if twilight zone
  else if ( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization )
  {
    realArray & ut = dvdt;
    realArray & uti = dvdtImplicit;

    OV_GET_LOCAL_ARRAY(real,ut);
    OV_GET_LOCAL_ARRAY(real,uti);
    OV_GET_LOCAL_ARRAY_CONDITIONAL(real,gridVelocity,!referenceFrameVelocity,utLocal,(*referenceFrameVelocity));

    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");

    Range V(uc,uc+mg.numberOfDimensions()-1), R(0,mg.numberOfDimensions()-1);
    real dt = rparam[3];

    real dtfac = t>t0 ? 2./dt : -2./dt;

    if ( referenceFrameVelocity )
      {
	for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    for( int side=Start; side<=End; side++ )
	      {
		if( mg.boundaryCondition(side,axis)==Parameters::noSlipWall )
		  {
		    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
		    getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3);
		    bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,Ib1,Ib2,Ib3); 

		    if ( ok )
		      {
			if ( dtfac<0. ) 
			  {
			    utLocal(Ib1,Ib2,Ib3,V) = dtfac*gridVelocityLocal(Ib1,Ib2,Ib3,R);
			  }
			else
			  {
			    utLocal(Ib1,Ib2,Ib3,V) += dtfac*gridVelocityLocal(Ib1,Ib2,Ib3,R);
			  }
		      }
		  }
	      }
	  }
      }
  } // bc forcing for approximate factorization without twilight zone
  

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForForcing"))+=getCPU()-cpu0;

}

