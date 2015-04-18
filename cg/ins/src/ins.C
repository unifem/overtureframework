// ==============================================================================
//    Determine du/dt for the INS and related equations 
// ==============================================================================

#include "Cgins.h"
#include "Parameters.h"
#include "MappedGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"
#include "turbulenceModels.h"
#include "turbulenceParameters.h"
#include "GridMaterialProperties.h"

#define insdt EXTERN_C_NAME(insdt)
extern "C"
{
 void insdt(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b,
      const int&mask, const real& xy, const real& rx, const real& radiusInverse, 
      const real& u, const real& uu, real&ut, real&uti, const real&gv, const real & dw, 
      const int & ndMatProp, const int& matIndex, const real& matValpc, const real& matVal,
      const int&bc, const int&ipar, const real&rpar, const int&ierr );
}


//\begin{>>CginsInclude.tex}{\subsection{getUtINS}}
int Cgins::
getUt(const realMappedGridFunction & v, 
      const realMappedGridFunction & gridVelocity_, 
      realMappedGridFunction & dvdt, 
      int iparam[], real rparam[],
      realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
      MappedGrid *pmg2 /* =NULL */,
      const realMappedGridFunction *pGridVelocity2 /* = NULL */ )
//========================================================================================================
// /Description:
//   Compute $u_t$ on a component grid for the Incompressible NS: 2D, 2nd or 4th order
//
// /t=rparam[0] (input) : current time.
// /v (input) : current solution.
// /gridVelocity\_ (input) : grid velocity, used for moving grid problems only.
// /dvdt (output) : return $u_t$ in this grid function.
// /grid (input) : the component grid number if this MappedGrid is part of a GridCollection or CompositeGrid.
// /tForce=rparam[1] (input) : apply the forcing at this time (this could be $t+\Delta/2$ for example).
// /dvdtImplicit (input) : for implicit time stepping, the time derivative is split into two parts,
//     $u_t=u_t^E + u_t^I$. The explicit part, $u_t^E$, is returned in dvdt while the implicit part, $u_t^I$,
//   is returned in dvdtImplicit. 
// /tImplicit=rparam[2] (input) : for implicit time stepping, apply forcing for the implicit part at his 
//     time.
//  /pmg2 (input) : pointer to the grid at the next time level (this is needed by some methods for moving grids)
//  /pGridVelocity2 (input) : pointer to the grid velocity at the next time level (this is needed by some
//                             methods for moving grids)
//
// Implicit time-stepping notes: 
// ----------------------------
//    Suppose we are solving the PDE:
//           u_t = f(u,x,t)  + F(x,t)
//   that we have split into an explicit part, fe(u),  and implicit part, A*u:
//           u_t = fe(u) + A u  + F(x,t)
//
//   If the time stepping method is implicit then we compute
//           dvdt = fe(u)
//   When implicitOption==computeImplicitTermsSeparately we also compute:
//           dvdtImplicit = (1-alpha)*A*u 
//   where alpha is the implicit factor (= .5 for Crank-Nicolson). This is the part of the implicit
//   term that is treated explicitly. 
//   (if implicitOption==doNotComputeImplicitTerms then do not change dvdtImplicit).
// 
//\end{MappedGridSolverInclude.tex}  
//=======================================================================================================
{
  real cpu0=getCPU();
  
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return 0;


  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  const real & t=rparam[0];
  real tForce   =rparam[1];
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];
  const int level=iparam[1];
  const int numberOfStepsTaken = iparam[2];
  
  if( debug() & 8 )
    printF("Cgins::getUtINS: t=%9.3e, pde = %s \n",t,(const char*)parameters.pdeName);

  MappedGrid & mg = *(v.getMappedGrid());

  const int numberOfDimensions = mg.numberOfDimensions();
  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
  const Parameters::TurbulenceModel & turbulenceModel = 
                       parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  const Parameters::TimeSteppingMethod & timeSteppingMethod = 
                       parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");
  const Parameters::ImplicitMethod & implicitMethod = 
                    parameters.dbase.get<Parameters::ImplicitMethod>("implicitMethod");

  const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");


  // 
  // The new way to evaluate du/dt for some PDE models is to evaluate the RHS from the implicit implementation
  // 

  bool useNewEvaluation = !( (pdeModel==InsParameters::standardModel || pdeModel==InsParameters::BoussinesqModel ) && 
                             turbulenceModel==Parameters::noTurbulenceModel )
                          && turbulenceModel!=Parameters::SpalartAllmaras 
                          && turbulenceModel!=Parameters::LargeEddySimulation;

  // For now the LES option only uses insImplicitMatrix for implicit time-stepping and old way for explicit

  if( ( useNewEvaluation || parameters.dbase.get<int>("useNewImplicitMethod")==1 ) 
      && implicitMethod!=Parameters::approximateFactorization  // 100125 kkc
      && (timeSteppingMethod==Parameters::implicit || turbulenceModel!=Parameters::LargeEddySimulation) )
  {
    if( debug() & 4 )
    {
      printF(" ***** Cgins::getUt --> call insimp to eval the RHS ***********\n");
      fPrintF(debugFile," ***** Cgins::getUt --> call insimp to eval the RHS ***********\n");

      ::display(v,sPrintF("getUt: v BEFORE insimp for grid=%i",grid),pDebugFile,"%4.2f ");
    
      printF(" implicitOption = %i (0=all, 1=no implicit, 2=implicit-separate), useNewImplicitMethod=%i\n",
	     parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),
             parameters.dbase.get<int>("useNewImplicitMethod"));
    }
    
    if( orderOfAccuracy==4 )
    {
      printP("Cgins::getUt:ERROR: do NOT useNewImplicitMethod with orderOfAccuracy==4! This does not work yet\n");
      OV_ABORT("error");
    }
    


    // New way for implicit: evaluate the RHS
    realMappedGridFunction & coeffg = dvdt;  // not used
    real dt0=1.;  // not used 
    insImplicitMatrix(InsParameters::evalRightHandSide,coeffg,dt0,v,dvdt,dvdtImplicit,gridVelocity_,grid);

    if( debug() & 4 )
    {
      ::display(v,sPrintF("getUt: v after insimp for grid=%i",grid),pDebugFile,"%4.2f ");
      ::display(dvdt,sPrintF("getUt: dvdt after insimp for grid=%i",grid),pDebugFile,"%3.1f ");
      ::display(dvdtImplicit,sPrintF("getUt: dvdtImplicit after insimp for grid=%i",grid),pDebugFile,"%3.1f ");
    }

    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetUt"))+=getCPU()-cpu0;
    addForcing(dvdt,v,iparam,rparam,dvdtImplicit);

    return 0;
  }

  if( debug() & 4 )
    printF(" ***** Cgins::getUt --> use old evaluation ***********\n");

  Index I1,I2,I3;
//   getIndex(extendedGridIndexRange(mg),I1,I2,I3);  // ***** 020902 *** WHY???
  getIndex(mg.gridIndexRange(),I1,I2,I3);  // ***** 030305 : we don't want to evaluate du/dt on ghost points

  const realArray & u = v;                               // **** array *****
  const realArray & gridVelocity = gridVelocity_;
  realArray & ut = dvdt;
  realArray & uti = dvdtImplicit;

  MappedGridOperators & op = *v.getOperators();

  int isRectangular=op.isRectangular();
  // isRectangular=false; // *** do this for testing  

  InsParameters::ImplicitVariation & implicitVariation = 
    parameters.dbase.get<InsParameters::ImplicitVariation>("implicitVariation");

  const real implicitFactor = parameters.dbase.get<real >("implicitFactor");
  real nu = parameters.dbase.get<real >("nu");              // this is a local copy
  real kThermal = parameters.dbase.get<real >("kThermal");  // this is a local copy
//   if( false && parameters.getGridIsImplicit(grid) )  // This is no longer done here *wdh* 071013
//   {
//     nu*=(1.-implicitFactor);        // coefficient of the explicit viscous terms (i.e. on the RHS)
//     kThermal*=(1.-implicitFactor);
//   }
  
  const real nuI = nu; // parameters.dbase.get<real >("nu");  // use this value for implicit terms

  const real adcPassiveScalar=1.; // coeff or linear artificial diffusion for the passive scalar ** add to params

  // only apply fourth-order AD here if it is explicit
  const bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
                                                !parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion");
  const bool & gridIsMoving = parameters.gridIsMoving(grid);

  int useWhereMask=false; // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 

  if( isRectangular && gridIsMoving && t>0. )  // *wdh* 100415 : getUt may be called for t<0 on a moving rectangular grid : is this ok?
  {
    printF("Cgins::getUt:ERROR: The grid is rectangular and moving! This should not happen!\n"
           " The operators for this grid have probably not been updated!\n"
           " grid=%i (name=%s) t=%9.3e\n",grid,(const char*)mg.getName(),t);
    OV_ABORT("error");
  }
  

  int *pMask;
  if( pmg2!=NULL )
  {
    // only need to evaluate du/dt at mask>0 in this case -- we have the mask at the new time level
    // *wdh* 080818 useWhereMask=true;
#ifdef USE_PPP
    pMask = pmg2->mask().getLocalArray().getDataPointer();
#else
    pMask = pmg2->mask().getDataPointer();
#endif

    // printf(" ***getUtINS: useWhereMask=%i \n",useWhereMask);
  }
  else
  {
#ifdef USE_PPP
    pMask = mg.mask().getLocalArray().getDataPointer(); 
#else
    pMask = mg.mask().getDataPointer();
#endif
  }

  real dx[3]={1.,1.,1.};
  if( isRectangular )
    mg.getDeltaX(dx);
  else
    mg.update(MappedGrid::THEinverseVertexDerivative);
   
  const int gridType= isRectangular ? 0 : 1;

#ifdef USE_PPP
  realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
#else
  const realSerialArray & uLocal = u;
#endif
  const real *pu = u.getDataPointer();

  // For now we need the center array for the axisymmetric case:
  const bool vertexNeeded=parameters.isAxisymmetric();
  
  const realArray & xy = vertexNeeded ? mg.center() : u;
  if( vertexNeeded ) 
  {
    assert( mg.center().getLength(0)>0 );
  }
  const realArray & rsxy = isRectangular ? u :  mg.inverseVertexDerivative();
  // For non-moving grids u==uu, otherwise uu is a temp space to hold (u-gv)

  //  uw is a workspace that is only used for moving grids.
  realSerialArray uw;  // *wdh* 091128 -- make this a serial array
  if( parameters.gridIsMoving(grid) )
  {
    uw.redim(uLocal);
  }
  const realSerialArray & uu = parameters.gridIsMoving(grid) ? uw : uLocal;

  const realArray & dw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? u : 
    (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];
    

#ifdef USE_PPP
  RealArray xLocal; getLocalArrayWithGhostBoundaries(xy,xLocal);
  const real *pxy = xy.getLocalArray().getDataPointer(); 
  const real *prsxy = rsxy.getLocalArray().getDataPointer(); 
  real *put = ut.getLocalArray().getDataPointer(); 
  real *puti = uti.getLocalArray().getDataPointer(); 
  const real *pgv = gridIsMoving ? gridVelocity.getLocalArray().getDataPointer() : pu;
  const real *puu = uu.getDataPointer(); 
  const real *pdw = dw.getLocalArray().getDataPointer();
#else
  const RealArray & xLocal = xy;
  const real *pxy = xy.getDataPointer(); 
  const real *prsxy = rsxy.getDataPointer(); 
  real *put = ut.getDataPointer(); 
  real *puti = uti.getDataPointer(); 
  const real *pgv = gridIsMoving ? gridVelocity.getDataPointer() : pu;
  const real *puu = uu.getDataPointer(); 
  const real *pdw = dw.getDataPointer();
#endif 

  getIndex(mg.gridIndexRange(),I1,I2,I3);  // *wdh* 030220  - evaluate du/dt here

#ifdef USE_PPP
  // loop bounds for this boundary:
  const int n1a = max(I1.getBase() , uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
  const int n1b = min(I1.getBound(),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));
  const int n2a = max(I2.getBase() , uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
  const int n2b = min(I2.getBound(),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));
  const int n3a = max(I3.getBase() , uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
  const int n3b = min(I3.getBound(),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));
#else
  // loop bounds for this boundary:
  const int n1a=I1.getBase(); const int n1b=I1.getBound();
  const int n2a=I2.getBase(); const int n2b=I2.getBound();
  const int n3a=I3.getBase(); const int n3b=I3.getBound();
#endif

  bool ok=true;
  if( n1a>n1b || n2a>n2b || n3a>n3b ) ok=false;

  if( ok )
  {
    
    I1=Range(n1a,n1b);
    I2=Range(n2a,n2b);
    I3=Range(n3a,n3b);

    // For axisymmetric problems define:
    //    radiusInverse(i1,i2,i3) = 1/y  : off the axis of symmetry
    //                            = 0    : on the axis of symmetry
    RealArray radiusInverse;
    if( parameters.isAxisymmetric() )
    {
      radiusInverse.redim(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2));
      radiusInverse(I1,I2,I3) = 1./max(REAL_MIN,xLocal(I1,I2,I3,axis2));
      Index Ib1,Ib2,Ib3;
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
	  {
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
            bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
            if( ok )
  	      radiusInverse(Ib1,Ib2,Ib3)=0.;
	  }
	}
      }
	
      if( debug() & 8 )
      {
	display(radiusInverse,sPrintF("Cgins::getUt: radiusInverse, grid=%i, before assignOPT",grid),pDebugFile,"%8.5f ");
      }
    }


    const int gridIsImplicit=parameters.getGridIsImplicit(grid);
    real adcBoussinesq=0.; // coefficient of artificial diffusion for Boussinesq T equation 
    real thermalExpansivity=1.;

    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("adcBoussinesq",adcBoussinesq);

    // --- Variable material properies ---
    GridMaterialProperties::MaterialFormatEnum materialFormat = GridMaterialProperties::constantMaterialProperties;
    int ndMatProp=1;  // for piecewise constant materials, this is the leading dimension of the matVal array
    int *matIndexPtr=pMask;  // if not used, point to mask
    real*matValPtr=put;       // if not used, point to ut
    if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
    {
      // Material properties do vary 

      // printF("Cgins::getUt: Material properties do vary\n");

      std::vector<GridMaterialProperties> & materialProperties = 
	parameters.dbase.get<std::vector<GridMaterialProperties> >("materialProperties");

      GridMaterialProperties & matProp = materialProperties[grid];
      materialFormat = matProp.getMaterialFormat();
      
      if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
      {
	IntegerArray & matIndex = matProp.getMaterialIndexArray();
        matIndexPtr = matIndex.getDataPointer();
      }
      
      RealArray & matVal = matProp.getMaterialValuesArray();
      matValPtr = matVal.getDataPointer();
      ndMatProp = matVal.getLength(0);  

      // ::display(matVal,"Cgins::getUt: matVal");
    }

    int ipar[] ={parameters.dbase.get<int >("pc"), // 0
		 parameters.dbase.get<int >("uc"),
		 parameters.dbase.get<int >("vc"),
		 parameters.dbase.get<int >("wc"),
		 parameters.dbase.get<int >("kc"),
		 parameters.dbase.get<int >("sc"),
		 parameters.dbase.get<int >("tc"),
		 grid,
		 orderOfAccuracy,
		 (int)parameters.gridIsMoving(grid), 
		 useWhereMask,                       // 10 
		 (int)gridIsImplicit,
		 (int)parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod"),
		 (int)parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),
		 (int)parameters.isAxisymmetric(),
		 (int)parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion"),
		 (int)useFourthOrderArtificialDiffusion,
		 (int)parameters.dbase.get<bool >("advectPassiveScalar"),
		 gridType,
		 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),
		 (int)parameters.dbase.get<InsParameters::PDEModel >("pdeModel"),  // 20 
                 parameters.dbase.get<int >("vsc"), // 21 
                 parameters.dbase.get<int >("rc"),   // 22
                 debug(),
                 materialFormat
    };

    real rpar[]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),
		 dx[0],dx[1],dx[2],
		 nu,
		 parameters.dbase.get<real >("ad21"),
		 parameters.dbase.get<real >("ad22"),
		 parameters.dbase.get<real >("ad41"),
		 parameters.dbase.get<real >("ad42"),
		 parameters.dbase.get<real >("nuPassiveScalar"),
		 adcPassiveScalar,
		 parameters.dbase.get<real >("ad21n"),
		 parameters.dbase.get<real >("ad22n"),
		 parameters.dbase.get<real >("ad41n"),
		 parameters.dbase.get<real >("ad42n"),
		 0.,                    // 17 : was yEps, no longer used 
		 parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],
		 parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1],
		 parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2],
		 thermalExpansivity,
		 adcBoussinesq,    // ipar[22]
		 kThermal,         // ipar[23]
                 t 
    };

    int ierr=0;
    insdt(mg.numberOfDimensions(),
	  I1.getBase(),I1.getBound(),
	  I2.getBase(),I2.getBound(),
	  I3.getBase(),I3.getBound(),
	  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
	  uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
	  *pMask, *pxy, *prsxy,*radiusInverse.getDataPointer(),
	  *pu, *puu, *put, 
	  *puti,*pgv, *pdw,
          ndMatProp,*matIndexPtr,*matValPtr,*matValPtr, 
	  *mg.boundaryCondition().getDataPointer(), ipar[0], rpar[0], ierr );

    
  }
  
  if( debug() & 64 )
  {
    fPrintF(debugFile,"\n ++getUt: t=%9.3e, grid=%i, gridIsMoving=%i gridIsImplicit=%i isRectangular=%i nu=%9.3e\n",
            t,grid,(int)parameters.gridIsMoving(grid),(int)parameters.getGridIsImplicit(grid),(int)isRectangular,nu);
    ::display(dvdt,sPrintF("getUt: dvdt after insdt for grid=%i, t=%9.3e",grid,t),pDebugFile,"%3.1f ");
    // if( parameters.gridIsMoving(grid) )
    //  ::display(gridVelocity,"getUt: gridVelocity",pDebugFile,"%3.1f ");
    if( parameters.getGridIsImplicit(grid) )
     ::display(dvdtImplicit,sPrintF("getUt: dvdtImplicit after insdt for grid=%i, t=%9.3e",grid,t),pDebugFile,"%3.1f ");
  }


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetUt"))+=getCPU()-cpu0;

  // printF("timeForGetUt=%8.2e\n",parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetUt")));
  
  addForcing(dvdt,v,iparam,rparam,dvdtImplicit);

  if( debug() & 64 )
  {
    ::display(dvdt,sPrintF("getUt: dvdt after addForcing grid=%i, t=%9.3e",grid,t),pDebugFile,"%3.1f ");
  }

//   if( bugCheck )
//   {
//     const int uc = parameters.dbase.get<int >("uc");
//     const int tc = parameters.dbase.get<int >("tc");
    
//     int c0=uc, c1=uc+mg.numberOfDimensions()-1;
//     if( pdeModel==InsParameters::BoussinesqModel )
//     {
//       assert( tc==c1+1 );
//       c1=tc;
//     }
//     for( int c=c0; c<=c1; c++ )
//     {
//       real maxErr = 0.;
//       real maxErri = 0.;
//       getIndex( mg.gridIndexRange(),I1,I2,I3);
//       where( mg.mask()(I1,I2,I3)>0 )
//       {
// 	maxErr = max(maxErr, max(fabs(dvdtSave(I1,I2,I3,c)-dvdt(I1,I2,I3,c))));
// 	maxErri = max( maxErri, max(fabs(dvdtImplicitSave(I1,I2,I3,c)-dvdtImplicit(I1,I2,I3,c))));
//       }

//       real eps=1.e-10;
//       if( maxErr > eps || maxErri > eps )
//       {
// 	printF(" ********** getUt: impOption=%i grid=%i, component c=%i max diff in  dvdt = %8.2e, dvdtImplicit=%8.2e \n",
// 	       parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),grid,c,maxErr,maxErri);
//       }
//     }
  
//     dvdt=dvdtSave;
//     dvdtImplicit=dvdtImplicitSave;

//   }

  return 0;

}



