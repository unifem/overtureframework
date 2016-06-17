// This file automatically generated from advanceLineSolve.bC with bpp.
// ****************************************************************************************************
// ********* INS Steady-State Line-Solver Routine : solve along lines in a given direction ************
// ****************************************************************************************************


#include "Cgins.h"
#include "TridiagonalSolver.h"
#include "MappedGridOperators.h"
#include "LineSolve.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

static int numberOfSlipWallErrorMessages=0;


// TridiagonalSolver **pTridiagonalSolvers=NULL;
#define tridiagonalSolver(c,axis,grid) lineSolve.pTridiagonalSolvers[c+maxNumberOfSystems*(axis+numberOfDimensions*(grid))]

//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)


// IntegerArray lineSolveIsInitialized;  // ****************************** fix *****************

// in common/src/getBounds.C : (should use new version in ParallelGridUtility.h)
// void
// getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                      IntegerArray & gidLocal, 
//                                      IntegerArray & dimensionLocal, 
//                                      IntegerArray & bcLocal );

#define insLineSetup EXTERN_C_NAME(inslinesetup)
#define insLineSetupNew EXTERN_C_NAME(inslinesetupnew)
#define insLineSolveBC EXTERN_C_NAME(inslinesolvebc)
#define computeResidual EXTERN_C_NAME(computeresidual)
#define computeResidualNew EXTERN_C_NAME(computeresidualnew)
extern "C"
{
void insLineSetup(const int&nd,
              		  const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
              		  const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,
                                    const int&nd3b,const int&nd4a,const int&nd4b,
              		  const int&md1a,const int&md1b,const int&md2a,const int&md2b,const int&md3a,const int&md3b,
              		  const int&mask,const real&rx,  const real&u,const real&gv,const real&dt,real&f,const real&dw,
                                    const int&dir, real&am, real&bm, real&cm, real&dm, real&em,  
                                    const int&bc, const int&boundaryCondition, const int&ndbcd1a,const int&ndbcd1b,const int&ndbcd2a,const int&ndbcd2b,
                                    const int&ndbcd3a,const int&ndbcd3b,const int&ndbcd4a,const int&ndbcd4b,const real&bcData,
                                    const int&ipar, const real&rpar, const int&ierr );

void insLineSetupNew(const int&nd,
              		  const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
              		  const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,
                                    const int&nd3b,const int&nd4a,const int&nd4b,
              		  const int&md1a,const int&md1b,const int&md2a,const int&md2b,const int&md3a,const int&md3b,
              		  const int&mask,const real&rx,  const real&u,const real&gv,const real&dt,real&f,const real&dw,
                                    const int&dir, real&am, real&bm, real&cm, real&dm, real&em,  
                                    const int&bc, const int&boundaryCondition, const int&ndbcd1a,const int&ndbcd1b,const int&ndbcd2a,const int&ndbcd2b,
                                    const int&ndbcd3a,const int&ndbcd3b,const int&ndbcd4a,const int&ndbcd4b,const real&bcData,
                                    const int&ipar, const real&rpar, const int&ierr );

void insLineSolveBC(const int&nd,
              		  const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
              		  const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,
                                    const int&nd3b,const int&nd4a,const int&nd4b,
              		  const int&md1a,const int&md1b,const int&md2a,const int&md2b,const int&md3a,const int&md3b,
              		  const int&mask,const real&rx,  const real&u,const real&gv,const real&dt,real&f,const real&dw,
                                    const int&dir, real&am, real&bm, real&cm, real&dm, real&em,  
                                    const int&bc, const int&boundaryCondition, const int&ndbcd1a,const int&ndbcd1b,const int&ndbcd2a,const int&ndbcd2b,
                                    const int&ndbcd3a,const int&ndbcd3b,const int&ndbcd4a,const int&ndbcd4b,const real&bcData,
                                    const int&ipar, const real&rpar, const int&ierr );

    void computeResidual(const int&nd,
                   		       const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
                   		       const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
                   		       const int&nd4a,const int&nd4b,
                   		       const int&mask,const real&rx,  const real&u,const real&gv,const real&dt,
                                              const real&f,const real&dw, real&residual,
                   		       const int&bc, const int&ipar, const real&rpar, const int&ierr );

    void computeResidualNew(const int&nd,
                   		       const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
                   		       const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
                   		       const int&nd4a,const int&nd4b,
                   		       const int&mask,const real&rx,  const real&u,const real&gv,const real&dt,
                                              const real&f,const real&dw, real&residual,
                   		       const int&bc, const int&ipar, const real&rpar, const int&ierr );
}


#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)



// ====================================================================================
// insLineBuildMatrix : 
//    Call insLineSetup to form the tridiagonal system, compute the RHS, or to compute the residual
// macro parameters:
//  Iv[] : fill in the interior equations at these values
// ====================================================================================

// ====================================================================================
// insLineMatrixBC: 
//    Fill in the matrix BC's for the line solver
// ====================================================================================

// ====================================================================================
//   setupParametersMacro :
//     Setup parameters for the call to insLineSetup
// ====================================================================================

// ====================================================================================
//   debugDisplayTridiagonalMatrices Macro:
// ====================================================================================

// ====================================================================================
//   debugDisplayFactoredMatrices
// ====================================================================================


// ----------------------------------------------------------------------------
//  getSystemIndexBounds: 
//   Return the array bounds for the tridiagonal systems
// ----------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// Macro: Determine the tridiagonal system type: 
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// Macro: Fill in and factor the tri-diagonal matrices for a system
// ------------------------------------------------------------------------------------------


// ===================================================================================================================
/// \brief INS Steady-State Line-Solver Routine : solve along lines in a given direction
/// \param lineSolve (input) : 
/// \param direction : solve along lines in this diresction
///
/// \notes advanceLineSolve:
///    o addForcing : assign body-forcing (e.g. TZ-forcing) to interior equations in f. 
///    o lineSolverBoundaryConditions : determine BC's for line solves, assign BC right-hand sides to f.
///    o insLineBuildMatrix(Ksv,am,bm,cm,dm,em) : calls insLineSetup to fill in the tri-diagonal matrix
///         am,b,,cm for a given equation. Optionally assign the RHS f for interior equations (all components)
//          (this does NOT include dirichlet boundaries) and sets dirichlet values for interpolation points. 
///    o insLineSolveBC : fill the matrix BC's into am,bm,cm and set dirichlet values for interpolation points
///          on the boundary (for dirichlet) or ghost line (for neumann).
// ===================================================================================================================
int Cgins::
advanceLineSolve(LineSolve & lineSolve,
                    const int grid, const int direction, 
                    realCompositeGridFunction & u0, 
                    realMappedGridFunction & f, 
                    realMappedGridFunction & residual,
                    const bool refactor,
                    const bool computeTheResidual /* =false */ )
{
    if( false )
    {
        return advanceLineSolveOld(lineSolve,grid,direction,u0,f,residual,refactor,computeTheResidual );
    }
    else
    {
        return advanceLineSolveNew(lineSolve,grid,direction,u0,f,residual,refactor,computeTheResidual );
    }
}



// ******************************************************************
// ***************** NEW way ****************************************
// ******************************************************************

int Cgins::
advanceLineSolveNew(LineSolve & lineSolve,
                    const int grid, const int direction, 
                    realCompositeGridFunction & u0, 
                    realMappedGridFunction & f, 
                    realMappedGridFunction & residual,
                    const bool refactor,
                    const bool computeTheResidual /* =false */ )
{
    real t=0.;  // ********************* what time should we use ? 

    CompositeGrid & cg= *u0.getCompositeGrid();
    realMappedGridFunction & u = u0[grid];
    MappedGrid & mg = cg[grid];
    const int numberOfDimensions=cg.numberOfDimensions();
    const intArray & mask = mg.mask();
    Range all;
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2]; 


//   Index Iav[3], &Ia1=Iav[0], &Ia2=Iav[1], &Ia3=Iav[2]; 

    const bool fourthOrder = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");
    if( debug() & 8 && fourthOrder )
        printF(" **** advanceLineSolve:INFO: solving penta-diagonal systems *****\n");
    

    InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
    const bool computeTemperature = (pdeModel==InsParameters::BoussinesqModel ||
                                                                      pdeModel==InsParameters::viscoPlasticModel);

  // For the INS, (u,v,w) have the same form for the implicit matrix equations, so that we can
  // can re-use the implicit systems:
  //     Dt + u*Dx + v*Dy - nu*Delta 
  // For the visco-plastic model, the implicit matrix equations are different for each component (u,v,w)
    bool momentumMatrixEquationsAreDifferent = pdeModel==InsParameters::viscoPlasticModel;

    bool isRectangular= mg.isRectangular();
  // turn this next stuff on to test the non-rectangular code even for a rectangular grid
  // ---------------
  //    isRectangular=false; // mg.isRectangular();
    
    if( !isRectangular )
    {
        mg.update(MappedGrid::THEinverseVertexDerivative);
    }
  // --------------

    bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    aString buff;

    const int pc=parameters.dbase.get<int >("pc");
    const int uc=parameters.dbase.get<int >("uc");
    const int vc=parameters.dbase.get<int >("vc");
    const int wc=parameters.dbase.get<int >("wc");
    const int nc=parameters.dbase.get<int >("kc");  // for SA turbulence model.
    const int tc=parameters.dbase.get<int >("tc");  // T

    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

   // The bcData array is used to access the mixed-derivative BC info for T
    const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");

    const real nu = parameters.dbase.get<real >("nu");
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
    RealArray & timing = parameters.dbase.get<RealArray>("timing");
    Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
    
    const bool useTurbulenceModel= turbulenceModel==Parameters::SpalartAllmaras;
    const int numberOfTimeDependentComponents= (useTurbulenceModel || computeTemperature ) ? numberOfDimensions+1 : numberOfDimensions;
    
    Range N(uc,uc+numberOfTimeDependentComponents-1);


    if( debug() & 4 )
    {
        fprintf(pDebugFile,"\n ************** advanceLineSolve: grid=%i, direction=%i ****************\n",grid,direction);
    }
    

#ifdef USE_PPP
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
    realSerialArray rLocal; getLocalArrayWithGhostBoundaries(residual,rLocal);
#else
    const intSerialArray & maskLocal = mask;

    realSerialArray & uLocal = u;
    realSerialArray & fLocal = f;
    realSerialArray & rLocal = residual;
#endif

//    // we first compute the smoothed artificial dissipation **** could save temporarily on residual array *****

//    realArray & artificialDissipation = mappedGridSolver[grid]->
//                      workSpace.get(MappedGridSolverWorkSpace::artificialDissipation);
//    getIndex(mg.dimension(),I1,I2,I3); 
//    const int nad= useTurbulenceModel ? 2 : 1;
//    mappedGridSolver[grid]->workSpace.resize(artificialDissipation,I1,I2,I3,nad);
    
//    computeArtficialDissipation();
    

    bool alwaysComputeResidual=debug() & 4; //  true;  // false

  // ***************************************
  // ************* FORCING *****************
  // ***************************************

  // we fill in the forcing first since addForcing sets f everywhere.
  // const int fc=uc;  // put forcing for uc here, vc at fc+1, wc at fc+2
    real time0,time1;

    fLocal=0.;

    if( true ) // twilightZoneFlow )
    {
        time0=getCPU();
        int iparam[10];
        real rparam[10];
        rparam[0]=0.; // gf[mk].t;
        rparam[1]=0.; // gf[mk].t;
        rparam[2]=0.; // gf[mk].t; // tImplicit
        iparam[0]=grid;
        iparam[1]=cg.refinementLevelNumber(grid);

        addForcing(f,u,iparam,rparam); // this does not use the mask

        timing(parameters.dbase.get<int>("timeForForcing"))+=getCPU()-time0;

        if( computeTheResidual || alwaysComputeResidual )
            rLocal(all,all,all,N)=fLocal(all,all,all,N);  // save for residual computation below *****************
    }


    const IntegerArray & dim = mg.dimension();
    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  // NOTE: bcLocal(side,axis) == -1 for internal boundaries between processors
  // ** this next call is also done in lineSolverBC's 
    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal );  

  // isPeriodic[axis] : periodic and not split across processors
    bool isPeriodic[3]={false,false,false}; // 
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
        isPeriodic[axis]=mg.isPeriodic(axis)!=Mapping::notPeriodic && 
            dim(0,axis)==dimLocal(0,axis) && dim(1,axis)==dimLocal(1,axis);
    }
    

  // *************************************************
  // ***************** START *************************
  // *************************************************


    const int maxNumberOfComponents=6;  // [p], (u,v,w), n (TM), T
    const int maxNumberOfSystems=maxNumberOfComponents; // [p], [u,v,w], [turbulence-model], [Temperature]

    getIndex(mg.extendedIndexRange(),I1,I2,I3);  
    bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);  // do not include parallel ghost


    if( ok )  // there are points on this processor
    {
    // ==== there must be no-communication within this loop ====

        lineSolve.lineSolveIsInitialized=!refactor; 

        if( lineSolve.pTridiagonalSolvers==NULL )
        {
      // create pointers to Tridiagonal Systems
            lineSolve.lineSolveIsInitialized.redim(numberOfDimensions,cg.numberOfComponentGrids());
            lineSolve.lineSolveIsInitialized=false;
        
            lineSolve.pTridiagonalSolvers = 
      	new TridiagonalSolver* [maxNumberOfSystems*cg.numberOfComponentGrids()*numberOfDimensions];
            for( int g=0; g<cg.numberOfComponentGrids(); g++ )
            {
      	for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  for( int m=0; m<maxNumberOfSystems; m++ )
          	    tridiagonalSolver(m,axis,g)=NULL;
      	}
            }
        }
    


    // --------------------------------------------------------------------------------
    //    uSystem[component] = matrix system-number used by this component
    //                       =  -1  : this component is not solved for (e.g. p for INS)
    // --------------------------------------------------------------------------------
        int uSystem[maxNumberOfComponents]={-1,-1,-1,-1,-1,-1};

    // ----- by default all velocity components use the same tridiagonal system ----
    //       This will be changed if the BC's are not the same for all components ...
        uSystem[uc]=uc;
        uSystem[vc]=uc;
        uSystem[wc]=uc;
        if( momentumMatrixEquationsAreDifferent )
        {
            uSystem[vc]=vc;
            uSystem[wc]=wc;
        }

        if( nc>=0 ) 
            uSystem[nc]=nc;   // The SA turbulence model variable uses it's own tridiagonal solver
    
        if( tc>=0 ) 
            uSystem[tc]=tc; // The Temperature equation needs it's own tridiagonal solver 
        

    // Master Index's (These are altered by numGhost(side) to get the matrix dimensions  )
        Index Ixv[3], &Ix1=Ixv[0], &Ix2=Ixv[1], &Ix3=Ixv[2]; 

    // options: (to match insLineSolve.bf)
        const int assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3;

        real dx[3]={1.,1.,1.};
        if( isRectangular  )
            mg.getDeltaX(dx);
    
    // ************************************************************
    // ************ MATRIX Boundary conditions ********************
    // ************************************************************
    // 
    // bc(side,system) : holds "boundary conditions" for the tridiagonal system
    // 
    //                = dirichlet : the end condition for the line solver is a dicihlet like condition
    //                = neuman    :
    //                = extrapolate : 
    //                = interpolate: the line solve hits an interpolation boundary where the value is assumed given
    //
        const int interpolate=0, dirichlet=1, neumann=2, extrapolate=3;

        IntegerArray bc(2,maxNumberOfSystems), numGhost(2);

    // The INS momentum equations are the same for all components, so that we can
    // can re-use the implicit systems:
    //     Dt + u*Dx + v*Dy - nu*Delta 
    // ... but the boundary conditions may be different 
        int numberOfDifferentLineSolverBoundaryConditions=0;


    // --- set the order of extrapolation for outflow ---
        parameters.dbase.get<int >("orderOfExtrapolationForOutflow")=twilightZoneFlow ? 2 : 2; // 3;   // for outflow BC's


    // --- compute bc,numGhost Ixv -----
        getLineSolverBoundaryConditions( grid,direction,u,Ixv,maxNumberOfSystems,uSystem,
                             				     numberOfTimeDependentComponents,
                             				     bc,numGhost,numberOfDifferentLineSolverBoundaryConditions );


    // ************************************
    // ************ Setup *****************
    // ************************************
          int computeMatrix= !lineSolve.lineSolveIsInitialized(direction,grid);
          int computeMatrixBoundaryConditions=computeMatrix;
          int computeRHS=1;
          const int ndipar=60, ndrpar=30;
          int ipar[ndipar];
          real rpar[ndrpar];
          ipar[0] = pc;
          ipar[1] = uc;
          ipar[2] = vc;
          ipar[3] = wc;
          ipar[4] = grid;
          ipar[5] = parameters.dbase.get<int >("orderOfAccuracy");
          ipar[6] = parameters.gridIsMoving(grid);
          ipar[7] = true;
          ipar[8] = parameters.getGridIsImplicit(grid);
          ipar[9] = parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
          ipar[10]= parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
          ipar[11]= parameters.isAxisymmetric();
          ipar[12]= parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion");
          ipar[13]= parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");
          ipar[14]= isRectangular ? 0 : 1; // gridType;
          ipar[15]= computeMatrix;
          ipar[16]= computeRHS;
          ipar[17]= computeMatrixBoundaryConditions;
          ipar[18]= uc;
          ipar[19]=parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
          ipar[20]=0;   // (system) specifies which tridiagonal system to solve (index into the bc(0:1,system) array
          ipar[21]=assignINS; // option 
          ipar[22]=nc;
          ipar[23]= turbulenceModel;
          ipar[24]= twilightZoneFlow;
          ipar[25]= parameters.dbase.get<bool >("useSelfAdjointDiffusion");
          ipar[26]= (int)fourthOrder;
          ipar[27]=(int)pdeModel;
          ipar[28]=tc;
          ipar[29]=numberOfComponents;
          ipar[30]=-1;         // Form the tridiagonal matrix for this velocity component
          ipar[31]=mg.gridIndexRange(0,0);
          ipar[32]=mg.gridIndexRange(1,0);
          ipar[33]=mg.gridIndexRange(0,1);
          ipar[34]=mg.gridIndexRange(1,1);
          ipar[35]=mg.gridIndexRange(0,2);
          ipar[36]=mg.gridIndexRange(1,2);
          ipar[37]=parameters.dbase.get<int >("vsc");
          const int nTrip=50;
          ipar[nTrip]=ipar[nTrip+1]=ipar[nTrip+2]=-1; // turbulence trip location, i,j,k
          if ( parameters.dbase.get<IntegerArray >("turbulenceTripPoint").getLength(0) )
          {
              for ( int i=0; i<mg.numberOfDimensions(); i++ )
                  ipar[nTrip+i] = parameters.dbase.get<IntegerArray>("turbulenceTripPoint")(i+1);
          }
          rpar[0] = dx[0];
          rpar[1] = dx[1];
          rpar[2] = dx[2];
          rpar[3] = parameters.dbase.get<real >("nu");
     // the SA model always has AD in the equations so turn it off here by setting the coeff's to zero:
          rpar[4] = parameters.dbase.get<bool>("useSecondOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad21") : 0.;
          rpar[5] = parameters.dbase.get<bool>("useSecondOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad22") : 0.;
          rpar[6] = parameters.dbase.get<bool>("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad41") : 0.;
          rpar[7] = parameters.dbase.get<bool>("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad42") : 0.;
          rpar[8] = mg.gridSpacing(0);
          rpar[9] = mg.gridSpacing(1);
          rpar[10]= mg.gridSpacing(2);
          rpar[11]= parameters.dbase.get<real >("cfl");
          rpar[12]= parameters.dbase.get<real >("ad21n");
          rpar[13]= parameters.dbase.get<real >("ad22n");
          rpar[14]= parameters.dbase.get<real >("ad41n");
          rpar[15]= parameters.dbase.get<real >("ad42n");
          rpar[16]= parameters.dbase.get<real >("kThermal");
     // ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
     // get the gravity vector -- may be time dependent for a slow start
          real gravity[3];
          parameters.getGravityVector( gravity,t );  // ** CHECK ME FOR time dependence ***
          real thermalExpansivity=1.;
          parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
          rpar[17]=thermalExpansivity;
          rpar[18]=gravity[0];
          rpar[19]=gravity[1];
          rpar[20]=gravity[2];
     // declare and lookup visco-plastic parameters (macro)
     // declareViscoPlasticParameters;
     // rpar[21]=nuViscoPlastic;         
     // rpar[22]=etaViscoPlastic;        
     // rpar[23]=yieldStressViscoPlastic;
     // rpar[24]=exponentViscoPlastic;   
     // rpar[25]=epsViscoPlastic;           
        
        int ierr;
        const realArray & rsxy = isRectangular ? u :  mg.inverseVertexDerivative();
        const realSerialArray *gvp = &uLocal;   // fix this  -- grid-velocity 

        const real *prsxy = rsxy.getDataPointer();
        if( !isRectangular && prsxy==NULL )
        {
            Overture::abort("advanceLineSolver:ERROR: array rx is not there!");
        }
        #ifdef USE_PPP
        if( false )
        {
            Overture::abort("advanceLineSolver:finish me!");
        }
        
        #endif


        realCompositeGridFunction *& pDistanceToBoundary =parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary");
    // *wdh* 081214 const realSerialArray *dwp = pDistanceToBoundary==NULL ? &uLocal : &((*pDistanceToBoundary)[grid]).getLocalArray();
        
        realSerialArray *dwp = &uLocal;
        if( pDistanceToBoundary !=NULL )
        {
            realSerialArray dtb; getLocalArrayWithGhostBoundaries((*pDistanceToBoundary)[grid],dtb);
            dwp = &dtb;
        }

        if ( turbulenceModel==Parameters::BaldwinLomax )
        {
      // ------------------------------------------------------------------------------
      // ------------- BaldwinLomax has a setup stage (zero-equation model) -----------
      // ------------------------------------------------------------------------------

            RealArray a,b,c,d,e;   // fix this ***************

            int option = ipar[21];
            ipar[21] = setupSweep; // option:  set up the sweep (compute BL eddy viscosity)
            ipar[30]=-1;        // form equations for this component (if equationsAreDifferent==true)

              K1=Iv[0], K2=Iv[1], K3=Iv[2]; 
      //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
              insLineSetupNew(numberOfDimensions,
                                        K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                         		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                         		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                        a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1), 
                         		 a.getBase(2),a.getBound(2),                  
                         		 *maskLocal.getDataPointer(), *prsxy,   
                                        *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                        *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                        direction, *a.getDataPointer(),*b.getDataPointer(),*c.getDataPointer(), 
                                        *d.getDataPointer(),*e.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                        bcData.getBase(0),bcData.getBound(0),
                                        bcData.getBase(1),bcData.getBound(1),
                                        bcData.getBase(2),bcData.getBound(2),
                                        bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                        ipar[0],rpar[0], ierr );

            ipar[21]=option;  // reset 
        }

    // ***********************************************************************************
    //  In the normal case we are solving the systems and not just computing the residual
    // ***********************************************************************************
        if( !computeTheResidual )  
        {
      // ============ loop here to only factor and solve a sub-set of the equations at a time ======
      // Isv, Jsv : s=subset


            Index Ixsv[3], &Ixs1=Ixsv[0], &Ixs2=Ixsv[1], &Ixs3=Ixsv[2]; // master bounds (when solving a sub-set of equations)      
            Index Dsv[3], &Ds1=Dsv[0], &Ds2=Dsv[1], &Ds3=Dsv[2]; // dimensions of (tridiagonal) systems

            bool done=false;

      // Here are the max number of parallel tridiagonal lines we solve at one time, in each direction:
            int maximumNumberOfLinesToSolveAtOneTime=100;
            parameters.dbase.get<ListOfShowFileParameters>("pdeParameters").getParameter("maximumNumberOfLinesToSolveAtOneTime",maximumNumberOfLinesToSolveAtOneTime); 
            if( maximumNumberOfLinesToSolveAtOneTime!=100 )
      	printF("LineSolver: solve at most %i lines at a time\n",maximumNumberOfLinesToSolveAtOneTime);
      	
            const int numberOfLinesPerSolve[3]={maximumNumberOfLinesToSolveAtOneTime,maximumNumberOfLinesToSolveAtOneTime,
                                                                                    maximumNumberOfLinesToSolveAtOneTime}; 

      // ******************************************************************************************************
      // ************* This next loop over (ns1,ns2) is used to split up the number of line solves  ***********
      // ************* so that we solve fewer simultaneously (this avoids using too much memory)    ***********
      // ******************************************************************************************************
            int nsv[2], &ns1=nsv[0], &ns2=nsv[1];
            ns2=0;                     // second tangential direction
            for( ns1=0; !done; ns1++ ) // first tangential direction
            {
            
      	time0=getCPU();

      	Ixs1=Ix1, Ixs2=Ix2, Ixs3=Ix3;  // fill in "subset" Index values 

        // Set bounds to solve a sub-set of points at a time: 
                bool solveForMultipleSubsets=false;
      	for( int dirs=0; dirs<numberOfDimensions-1; dirs++ ) // tangential directions
      	{
        	  const int dirp= (direction+dirs+1)%numberOfDimensions;

        	  int base=Ixv[dirp].getBase()+nsv[dirs]*numberOfLinesPerSolve[dirp];

        	  if( base>Ixv[dirp].getBound() )
        	  {
          	    if( dirs==numberOfDimensions-2 )
          	    {
            	      done=true;
            	      break;
          	    }
          	    else
          	    { // in 3D we reset ns1 to zero and keep going
            	      ns1=0;
            	      ns2++;
            	      base=Ixv[dirp].getBase();
          	    }
        	  }
        	  int bound=min(base+(numberOfLinesPerSolve[dirp]-1),Ixv[dirp].getBound());

        	  Ixsv[dirp]=Range(base,bound);

        	  if( base!=Ixv[dirp].getBase() || bound!=Ixv[dirp].getBound() )
                        solveForMultipleSubsets=true;
      	}
    

      	if( done ) break;

      	if( debug() & 2 )
      	{
        	  fprintf(pDebugFile,
                                  "\n ++++++ direction=%i (ns1,ns2)=(%i,%i) : solve for points Ixsv=[%i,%i][%i,%i][%i,%i] "
             		 "+++++++++++++++\n\n",
             		 direction,ns1,ns2,
              		  Ixs1.getBase(),Ixs1.getBound(),Ixs2.getBase(),Ixs2.getBound(),Ixs3.getBase(),Ixs3.getBound());
      	}
      	

        // add on extra ghost lines to Ixsv to get the matrix dimensions Dsv: 
                  for( int axis=0; axis<3; axis++ )
                  {
                      if( axis==direction )
                          Dsv[direction]=Range(Ixsv[direction].getBase()-numGhost(0),Ixsv[direction].getBound()+numGhost(1));
                      else
                          Dsv[axis]=Ixsv[axis];
                  }

      	if( !lineSolve.lineSolveIsInitialized(direction,grid) )
      	{
          // *****************************************************************************
	  // ************* form and factor the matrix, compute the rhs *******************
          // *****************************************************************************

        	  computeMatrix= !lineSolve.lineSolveIsInitialized(direction,grid);
        	  computeMatrixBoundaryConditions=computeMatrix;
        	  computeRHS=!solveForMultipleSubsets;  

        	  ipar[15]= computeMatrix;
        	  ipar[16]= computeRHS;
        	  ipar[17]= computeMatrixBoundaryConditions;

        	  if( turbulenceModel==Parameters::SpalartAllmaras )
        	  {
	    // ****************************************************************************
	    // ************** Build the SpalartAllmaras Tridiagonal System ****************
	    // ****************************************************************************

	    // *** do this first so that the RHS is filled in before the RHS-BC's are assigned ??

	    // The SA turbulence variable should have the same BC's as the tangential component
	    //     dirichlet at no slip walls, neumann at slip walls

                        const int systemTM=uSystem[nc]; 
                          RealArray am(Ds1,Ds2,Ds3),bm(Ds1,Ds2,Ds3),cm(Ds1,Ds2,Ds3),dm,em;
                          if( fourthOrder )
                          { // penta-diagonal:
                              dm.redim(Ds1,Ds2,Ds3);
                              em.redim(Ds1,Ds2,Ds3);
                          }
             // printf(" Build the tridiagonal matrix for the Temperature equation, systemTemperature=%i\n",systemTemperature);
                          ipar[20]=systemTM;
                          ipar[21]=assignSpalartAllmaras;
                          ipar[30]=nc;        // form equations for this nc (if equationsAreDifferent==true)
             // bcData.display("bcData before setup T eqn's");
             // here we assign the tridiagonal systemTM for the Temperature equation
                            K1=Ixsv[0], K2=Ixsv[1], K3=Ixsv[2]; 
             //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
                            insLineSetupNew(numberOfDimensions,
                                                      K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                       		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                       		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                      am.getBase(0),am.getBound(0),am.getBase(1),am.getBound(1), 
                                       		 am.getBase(2),am.getBound(2),                  
                                       		 *maskLocal.getDataPointer(), *prsxy,   
                                                      *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                      *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                      direction, *am.getDataPointer(),*bm.getDataPointer(),*cm.getDataPointer(), 
                                                      *dm.getDataPointer(),*em.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                      bcData.getBase(0),bcData.getBound(0),
                                                      bcData.getBase(1),bcData.getBound(1),
                                                      bcData.getBase(2),bcData.getBound(2),
                                                      bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                      ipar[0],rpar[0], ierr );
                          if( solveForMultipleSubsets && ns1==0 && ns2==0 )
                          { // Evaluate the RHS once for ALL points here (if we are solving for multiple subsets)
                              ipar[15]= computeMatrix = false;
                              ipar[16]= computeRHS    = true; 
                                K1=Ixv[0], K2=Ixv[1], K3=Ixv[2]; 
               //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
                                insLineSetupNew(numberOfDimensions,
                                                          K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                           		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                           		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                          am.getBase(0),am.getBound(0),am.getBase(1),am.getBound(1), 
                                           		 am.getBase(2),am.getBound(2),                  
                                           		 *maskLocal.getDataPointer(), *prsxy,   
                                                          *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                          *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                          direction, *am.getDataPointer(),*bm.getDataPointer(),*cm.getDataPointer(), 
                                                          *dm.getDataPointer(),*em.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                          bcData.getBase(0),bcData.getBound(0),
                                                          bcData.getBase(1),bcData.getBound(1),
                                                          bcData.getBase(2),bcData.getBound(2),
                                                          bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                          ipar[0],rpar[0], ierr );
                          }
             // fill in the matrix BC's
                          {
                          K1=Ixsv[0], K2=Ixsv[1], K3=Ixsv[2]; 
             // adjust bcLocal for solving on am sub-set of points -- set the bcLocal to "periodic" on the
             // interior boundaries so that we do not change the matrix BC's there
                          IntegerArray bcLocalSave=bcLocal;
                          for( int dirs=0; dirs<numberOfDimensions-1; dirs++ ) // tangential directions
                          {
                              const int dirp= (direction+dirs+1) % numberOfDimensions;  
                              if( Ixsv[dirp].getBase()!=Ixv[dirp].getBase() ) 
                                  bcLocal(0,dirp)=-1;
                              if( Ixsv[dirp].getBound()!=Ixv[dirp].getBound() )
                                  bcLocal(1,dirp)=-1;
                          }
              // Note: Fill in BC coefficients on one line outside bounds Kv:
                            if( am.getLength(0)<K1.getLength() )
                            {
                                printF(" lineSolve: grid=%i : ERROR: insLineSolveBC: matrix am too small?? am=[%i,%i] K=[%i,%i]\n",
                                  	  grid,am.getBase(0),am.getBound(0),K1.getBase(),K1.getBound());
                                Overture::abort("error");
                            }
                            insLineSolveBC(numberOfDimensions,
                                                      K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                       		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                       		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                      am.getBase(0),am.getBound(0),am.getBase(1),am.getBound(1), 
                                       		 am.getBase(2),am.getBound(2),                  
                                       		 *maskLocal.getDataPointer(), *prsxy,   
                                                      *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                      *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                      direction, *am.getDataPointer(),*bm.getDataPointer(),*cm.getDataPointer(), 
                                                      *dm.getDataPointer(),*em.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                      bcData.getBase(0),bcData.getBound(0),
                                                      bcData.getBase(1),bcData.getBound(1),
                                                      bcData.getBase(2),bcData.getBound(2),
                                                      bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                      ipar[0],rpar[0], ierr );
                            bcLocal=bcLocalSave;
                          }
                          const char systemString[]="SPAL";
                            if( debug() & 8 )
                            {
                                fprintf(pDebugFile," +++grid %i (%s) direction=%i +++\n",
                                    	    grid,(const char*)mg.getName(),direction);
                                if( debug() & 16 )
                                    displayMask(mask,"Here is the mask",pDebugFile);
                                display(am,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), am",systemString),pDebugFile,"%6.2f ");
                                display(bm,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), bm",systemString),pDebugFile,"%6.2f ");
                                display(cm,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), cm",systemString),pDebugFile,"%6.2f ");
                                if( fourthOrder )
                                {
                                    display(dm,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), dm",systemString),pDebugFile,"%6.2f ");
                                    display(em,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), em",systemString),pDebugFile,"%6.2f ");
                                }
                                fflush(pDebugFile);
                            }
                          if( tridiagonalSolver(systemTM,direction,grid)==NULL )
                              tridiagonalSolver(systemTM,direction,grid)=new TridiagonalSolver;
                            TridiagonalSolver::SystemType type = (isPeriodic[direction] ? TridiagonalSolver::periodic :
                                        	        (bc(0,nc)!=dirichlet || bc(1,nc)!=dirichlet) ? TridiagonalSolver::extended :
                                                                						  TridiagonalSolver::normal);
                          if( debug() & 4 )
                                fprintf(pDebugFile," Tridiagonal %s systemTM type=%i (normal=%i,extended=%i,periodic=%i)\n",
                                                systemString,(int)type,(int)TridiagonalSolver::normal,(int)TridiagonalSolver::extended,
                                                (int) TridiagonalSolver::periodic);
                          if( !fourthOrder )
                              tridiagonalSolver(systemTM,direction,grid)->factor(am,bm,cm,type,direction);
                          else
                              tridiagonalSolver(systemTM,direction,grid)->factor(am,bm,cm,dm,em,type,direction);

        	  } // end SPAL TM


          //  ------------- optionally create the tridiagonal system for the temperature here -----------------
        	  if( computeTemperature )
        	  {
          	    const int systemTemperature=tc;
                          RealArray am(Ds1,Ds2,Ds3),bm(Ds1,Ds2,Ds3),cm(Ds1,Ds2,Ds3),dm,em;
                          if( fourthOrder )
                          { // penta-diagonal:
                              dm.redim(Ds1,Ds2,Ds3);
                              em.redim(Ds1,Ds2,Ds3);
                          }
             // printf(" Build the tridiagonal matrix for the Temperature equation, systemTemperature=%i\n",systemTemperature);
                          ipar[20]=systemTemperature;
                          ipar[21]=assignTemperature;
                          ipar[30]=tc;        // form equations for this tc (if equationsAreDifferent==true)
             // bcData.display("bcData before setup T eqn's");
             // here we assign the tridiagonal systemTemperature for the Temperature equation
                            K1=Ixsv[0], K2=Ixsv[1], K3=Ixsv[2]; 
             //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
                            insLineSetupNew(numberOfDimensions,
                                                      K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                       		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                       		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                      am.getBase(0),am.getBound(0),am.getBase(1),am.getBound(1), 
                                       		 am.getBase(2),am.getBound(2),                  
                                       		 *maskLocal.getDataPointer(), *prsxy,   
                                                      *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                      *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                      direction, *am.getDataPointer(),*bm.getDataPointer(),*cm.getDataPointer(), 
                                                      *dm.getDataPointer(),*em.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                      bcData.getBase(0),bcData.getBound(0),
                                                      bcData.getBase(1),bcData.getBound(1),
                                                      bcData.getBase(2),bcData.getBound(2),
                                                      bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                      ipar[0],rpar[0], ierr );
                          if( solveForMultipleSubsets && ns1==0 && ns2==0 )
                          { // Evaluate the RHS once for ALL points here (if we are solving for multiple subsets)
                              ipar[15]= computeMatrix = false;
                              ipar[16]= computeRHS    = true; 
                                K1=Ixv[0], K2=Ixv[1], K3=Ixv[2]; 
               //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
                                insLineSetupNew(numberOfDimensions,
                                                          K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                           		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                           		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                          am.getBase(0),am.getBound(0),am.getBase(1),am.getBound(1), 
                                           		 am.getBase(2),am.getBound(2),                  
                                           		 *maskLocal.getDataPointer(), *prsxy,   
                                                          *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                          *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                          direction, *am.getDataPointer(),*bm.getDataPointer(),*cm.getDataPointer(), 
                                                          *dm.getDataPointer(),*em.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                          bcData.getBase(0),bcData.getBound(0),
                                                          bcData.getBase(1),bcData.getBound(1),
                                                          bcData.getBase(2),bcData.getBound(2),
                                                          bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                          ipar[0],rpar[0], ierr );
                          }
             // fill in the matrix BC's
                          {
                          K1=Ixsv[0], K2=Ixsv[1], K3=Ixsv[2]; 
             // adjust bcLocal for solving on am sub-set of points -- set the bcLocal to "periodic" on the
             // interior boundaries so that we do not change the matrix BC's there
                          IntegerArray bcLocalSave=bcLocal;
                          for( int dirs=0; dirs<numberOfDimensions-1; dirs++ ) // tangential directions
                          {
                              const int dirp= (direction+dirs+1) % numberOfDimensions;  
                              if( Ixsv[dirp].getBase()!=Ixv[dirp].getBase() ) 
                                  bcLocal(0,dirp)=-1;
                              if( Ixsv[dirp].getBound()!=Ixv[dirp].getBound() )
                                  bcLocal(1,dirp)=-1;
                          }
              // Note: Fill in BC coefficients on one line outside bounds Kv:
                            if( am.getLength(0)<K1.getLength() )
                            {
                                printF(" lineSolve: grid=%i : ERROR: insLineSolveBC: matrix am too small?? am=[%i,%i] K=[%i,%i]\n",
                                  	  grid,am.getBase(0),am.getBound(0),K1.getBase(),K1.getBound());
                                Overture::abort("error");
                            }
                            insLineSolveBC(numberOfDimensions,
                                                      K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                       		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                       		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                      am.getBase(0),am.getBound(0),am.getBase(1),am.getBound(1), 
                                       		 am.getBase(2),am.getBound(2),                  
                                       		 *maskLocal.getDataPointer(), *prsxy,   
                                                      *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                      *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                      direction, *am.getDataPointer(),*bm.getDataPointer(),*cm.getDataPointer(), 
                                                      *dm.getDataPointer(),*em.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                      bcData.getBase(0),bcData.getBound(0),
                                                      bcData.getBase(1),bcData.getBound(1),
                                                      bcData.getBase(2),bcData.getBound(2),
                                                      bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                      ipar[0],rpar[0], ierr );
                            bcLocal=bcLocalSave;
                          }
                          const char systemString[]="T";
                            if( debug() & 8 )
                            {
                                fprintf(pDebugFile," +++grid %i (%s) direction=%i +++\n",
                                    	    grid,(const char*)mg.getName(),direction);
                                if( debug() & 16 )
                                    displayMask(mask,"Here is the mask",pDebugFile);
                                display(am,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), am",systemString),pDebugFile,"%6.2f ");
                                display(bm,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), bm",systemString),pDebugFile,"%6.2f ");
                                display(cm,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), cm",systemString),pDebugFile,"%6.2f ");
                                if( fourthOrder )
                                {
                                    display(dm,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), dm",systemString),pDebugFile,"%6.2f ");
                                    display(em,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), em",systemString),pDebugFile,"%6.2f ");
                                }
                                fflush(pDebugFile);
                            }
                          if( tridiagonalSolver(systemTemperature,direction,grid)==NULL )
                              tridiagonalSolver(systemTemperature,direction,grid)=new TridiagonalSolver;
                            TridiagonalSolver::SystemType type = (isPeriodic[direction] ? TridiagonalSolver::periodic :
                                        	        (bc(0,tc)!=dirichlet || bc(1,tc)!=dirichlet) ? TridiagonalSolver::extended :
                                                                						  TridiagonalSolver::normal);
                          if( debug() & 4 )
                                fprintf(pDebugFile," Tridiagonal %s systemTemperature type=%i (normal=%i,extended=%i,periodic=%i)\n",
                                                systemString,(int)type,(int)TridiagonalSolver::normal,(int)TridiagonalSolver::extended,
                                                (int) TridiagonalSolver::periodic);
                          if( !fourthOrder )
                              tridiagonalSolver(systemTemperature,direction,grid)->factor(am,bm,cm,type,direction);
                          else
                              tridiagonalSolver(systemTemperature,direction,grid)->factor(am,bm,cm,dm,em,type,direction);
        	  }
        	  

          // ---------------------------------------------------------
	  // -------------- Momentum Equations -----------------------
          // ---------------------------------------------------------

                    RealArray av[3], bv[3], cv[3], dv[3], ev[3];

          // system0 : equations for u ( and maybe w)
          // system1 : equations for v ( and maybe w)
          //const int system0=uSystem[uc]; 
          //const int system1= uSystem[uc]!=uSystem[vc] ? vc : numberOfDimensions>2 ? wc : uc;

        	  for( int m=0; m<numberOfDimensions; m++ ) // m : momentum components u,v,w 
        	  {
          	    int velocityComponent=uc+m;
          	    int system=-1;
            // bool computeMatrix=false,computeRHS=false,computeMatrixBoundaryConditions=false;
          	    
          	    if( velocityComponent==uc )
          	    {
            	      system=uSystem[uc];
            	      computeMatrix=true;   // compute the matrix 
            	      computeRHS= !solveForMultipleSubsets;   // compute RHS (for all components)
                	      computeMatrixBoundaryConditions=true;   // we DO need to fill in the BC's
          	    }
          	    else if( velocityComponent==vc )
          	    {
                            system=uSystem[vc];
            	      computeMatrix=momentumMatrixEquationsAreDifferent;   // compute the matrix only sometimes
                            computeRHS=false;  
                            computeMatrixBoundaryConditions=uSystem[vc] != uSystem[uc];
          	    }
          	    else
          	    {
                            system=uSystem[wc];
            	      computeMatrix=momentumMatrixEquationsAreDifferent;   // compute the matrix only sometimes
                            computeRHS=false;  
                            computeMatrixBoundaryConditions= (uSystem[wc] != uSystem[uc]) || (uSystem[wc] != uSystem[vc]);
          	    }
          	    assert( system>=0 );
          	    
          	    ipar[15]= computeMatrix;
          	    ipar[16]= computeRHS;
          	    ipar[17]= computeMatrixBoundaryConditions;

          	    ipar[20]=system;    // system to solve
          	    ipar[21]=assignINS; // option: fill in the INS momentum equations
          	    ipar[30]=velocityComponent;  // form equations for this component (if equationsAreDifferent==true)

          	    RealArray & a = av[m];
          	    RealArray & b = bv[m];
          	    RealArray & c = cv[m];
          	    RealArray & d = dv[m];
          	    RealArray & e = ev[m];

          	    if( computeMatrix )
          	    {
                            if( debug() & 4 )
                                printF(" *** Fill in the matrix for system=%i, velocityComponent=%i \n",system,velocityComponent);

            	      
            	      assert( a.getLength(0)==0 );
            	      
            	      a.redim(Ds1,Ds2,Ds3); b.redim(Ds1,Ds2,Ds3); c.redim(Ds1,Ds2,Ds3);
            	      if( fourthOrder )
            	      { // penta-diagonal:
            		d.redim(Ds1,Ds2,Ds3); e.redim(Ds1,Ds2,Ds3);
            	      }
            	      
	      // fill in the matrix system  and assign the interior-equations rhs (for u,v,w)
	      // (Note: this will fill in the interior equations on the boundary too -> 
              //          may be needed if we copy matrix for v or w)
                          K1=Ixsv[0], K2=Ixsv[1], K3=Ixsv[2]; 
            //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
                          insLineSetupNew(numberOfDimensions,
                                                    K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                     		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                     		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                    a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1), 
                                     		 a.getBase(2),a.getBound(2),                  
                                     		 *maskLocal.getDataPointer(), *prsxy,   
                                                    *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                    *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                    direction, *a.getDataPointer(),*b.getDataPointer(),*c.getDataPointer(), 
                                                    *d.getDataPointer(),*e.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                    bcData.getBase(0),bcData.getBound(0),
                                                    bcData.getBase(1),bcData.getBound(1),
                                                    bcData.getBase(2),bcData.getBound(2),
                                                    bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                    ipar[0],rpar[0], ierr );
            	      if( m==0 && solveForMultipleSubsets && ns1==0 && ns2==0 )
            	      { // Evaluate the RHS once for ALL points here (if we are solving for multiple subsets)
                                ipar[15]= computeMatrix = false;
                                ipar[16]= computeRHS    = true; 
                                  K1=Ixv[0], K2=Ixv[1], K3=Ixv[2]; 
                //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
                                  insLineSetupNew(numberOfDimensions,
                                                            K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                             		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                             		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                            a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1), 
                                             		 a.getBase(2),a.getBound(2),                  
                                             		 *maskLocal.getDataPointer(), *prsxy,   
                                                            *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                            *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                            direction, *a.getDataPointer(),*b.getDataPointer(),*c.getDataPointer(), 
                                                            *d.getDataPointer(),*e.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                            bcData.getBase(0),bcData.getBound(0),
                                                            bcData.getBase(1),bcData.getBound(1),
                                                            bcData.getBase(2),bcData.getBound(2),
                                                            bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                            ipar[0],rpar[0], ierr );
            	      }
            	      
            	      
          	    } // end computeMatrix

            // In some cases we make a copy of the original matrix (to avoid recomputing the same thing)
            // since there are different BC's to be filled in to the matrix
          	    if( m==0 && numberOfDifferentLineSolverBoundaryConditions>0 && !momentumMatrixEquationsAreDifferent   )
          	    {
	      // printF(" lineSolve: grid=%i : numberOfDifferentLineSolverBoundaryConditions=%i : "
              //        "copy a,b,c uSystem=[%i,%i,%i]\n",grid,
              //             numberOfDifferentLineSolverBoundaryConditions,uSystem[uc],uSystem[vc],uSystem[wc]); // @@@

                            int ms=1;
            	      if( uSystem[vc]!=uSystem[uc] )
            	      {
            		av[ms]=av[0]; bv[ms]=bv[0]; cv[ms]=cv[0];
            		if( fourthOrder )
            		{
              		  dv[ms]=dv[0]; ev[ms]=ev[0];
            		}
		// ms++; 
            	      }
            	      ms++;  // *wdh* 090210 -- component w always uses av[2] 
            	      if( numberOfDimensions==3 && (uSystem[wc]!=uSystem[uc] || uSystem[wc]!=uSystem[vc]) )
            	      {
            		av[ms]=av[0]; bv[ms]=bv[0]; cv[ms]=cv[0];
            		if( fourthOrder )
            		{
              		  dv[ms]=dv[0]; ev[ms]=ev[0];
            		}
            	      }
          	    }
          	    
          	    if( computeMatrixBoundaryConditions )
          	    {
                            if( debug() & 4 )
                                printF(" --- Fill in the matrix BC's for system=%i, velocityComponent=%i \n",system,velocityComponent);

	      // fill in the matrix BC's for u
                        {
                        K1=Ixsv[0], K2=Ixsv[1], K3=Ixsv[2]; 
            // adjust bcLocal for solving on a sub-set of points -- set the bcLocal to "periodic" on the
            // interior boundaries so that we do not change the matrix BC's there
                        IntegerArray bcLocalSave=bcLocal;
                        for( int dirs=0; dirs<numberOfDimensions-1; dirs++ ) // tangential directions
                        {
                            const int dirp= (direction+dirs+1) % numberOfDimensions;  
                            if( Ixsv[dirp].getBase()!=Ixv[dirp].getBase() ) 
                                bcLocal(0,dirp)=-1;
                            if( Ixsv[dirp].getBound()!=Ixv[dirp].getBound() )
                                bcLocal(1,dirp)=-1;
                        }
             // Note: Fill in BC coefficients on one line outside bounds Kv:
                          if( a.getLength(0)<K1.getLength() )
                          {
                              printF(" lineSolve: grid=%i : ERROR: insLineSolveBC: matrix a too small?? a=[%i,%i] K=[%i,%i]\n",
                                	  grid,a.getBase(0),a.getBound(0),K1.getBase(),K1.getBound());
                              Overture::abort("error");
                          }
                          insLineSolveBC(numberOfDimensions,
                                                    K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                                     		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                                     		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                                    a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1), 
                                     		 a.getBase(2),a.getBound(2),                  
                                     		 *maskLocal.getDataPointer(), *prsxy,   
                                                    *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                                    *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                                    direction, *a.getDataPointer(),*b.getDataPointer(),*c.getDataPointer(), 
                                                    *d.getDataPointer(),*e.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                                    bcData.getBase(0),bcData.getBound(0),
                                                    bcData.getBase(1),bcData.getBound(1),
                                                    bcData.getBase(2),bcData.getBound(2),
                                                    bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                                    ipar[0],rpar[0], ierr );
                          bcLocal=bcLocalSave;
                        }

                            char systemString[2];
                            sprintf(systemString,"%i",system);
            	      
                          if( debug() & 8 )
                          {
                              fprintf(pDebugFile," +++grid %i (%s) direction=%i +++\n",
                                  	    grid,(const char*)mg.getName(),direction);
                              if( debug() & 16 )
                                  displayMask(mask,"Here is the mask",pDebugFile);
                              display(a,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), a",systemString),pDebugFile,"%6.2f ");
                              display(b,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), b",systemString),pDebugFile,"%6.2f ");
                              display(c,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), c",systemString),pDebugFile,"%6.2f ");
                              if( fourthOrder )
                              {
                                  display(d,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), d",systemString),pDebugFile,"%6.2f ");
                                  display(e,sPrintF(buff,"Here is the tridiagonal INS matrix (system %s), e",systemString),pDebugFile,"%6.2f ");
                              }
                              fflush(pDebugFile);
                          }

            	      if( tridiagonalSolver(system,direction,grid)==NULL )
            		tridiagonalSolver(system,direction,grid)=new TridiagonalSolver;

	      // Parallel: fix (bool)mg.isPeriodic(direction) ********************************************************

              // *** Factor the matricies ***

                          TridiagonalSolver::SystemType type = (isPeriodic[direction] ? TridiagonalSolver::periodic :
                                      	        (bc(0,system)!=dirichlet || bc(1,system)!=dirichlet) ? TridiagonalSolver::extended :
                                                              						  TridiagonalSolver::normal);

            	      if( debug() & 4 )
            		fprintf(pDebugFile," Tridiagonal system %i : type=%i (normal=%i,extended=%i,periodic=%i)\n",
                  			system,(int)type,(int)TridiagonalSolver::normal,(int)TridiagonalSolver::extended,
                  			(int) TridiagonalSolver::periodic);


            	      if( !fourthOrder )
            		tridiagonalSolver(system,direction,grid)->factor(a,b,c,type,direction);
            	      else
            		tridiagonalSolver(system,direction,grid)->factor(a,b,c,d,e,type,direction);

                          if( debug() & 16 )
                          {
                              display(a,sPrintF(buff,"AFTER FACTOR: Here is the tridiagonal INS matrix (system %s), a",systemString),pDebugFile,"%6.0f ");
                              display(b,sPrintF(buff,"AFTER FACTOR: Here is the tridiagonal INS matrix (system %s), b",systemString),pDebugFile,"%6.0f ");
                              display(c,sPrintF(buff,"AFTER FACTOR: Here is the tridiagonal INS matrix (system %s), c",systemString),pDebugFile,"%6.0f ");
                              if( fourthOrder )
                              {
                                  display(d,sPrintF(buff,"AFTER FACTOR: Here is the tridiagonal INS matrix (system %s), d",systemString),pDebugFile,"%6.0f ");
                                  display(e,sPrintF(buff,"AFTER FACTOR: Here is the tridiagonal INS matrix (system %s), e",systemString),pDebugFile,"%6.0f ");
                              }
                              fflush(pDebugFile);
                          }

          	    } // end if computeMatrixBoundaryConditions

                    } // end for m  (velocity components)
        	  
        
	  // add the size computation to LineSolve ?
                    if( ns1==0 && ns2==0 )
        	  {
	    // *** for statistics we remember how much memory we needed ***
          	    real size=0;
          	    for( int c=0; c<maxNumberOfSystems; c++ )
          	    {
            	      if( tridiagonalSolver(c,direction,grid)!=NULL )
            		size+=tridiagonalSolver(c,direction,grid)->sizeOf();
          	    }
          	    lineSolve.maximumSizeAllocated=max(lineSolve.maximumSizeAllocated,size);
        	  }
        	  
      	}
      	else 
      	{
          // ********* lineSolve is already initialized  **********
	  // ********* just compute the RHS              **********

          // --- check this ---

                    const int system0=uSystem[uc];
                    const int system1= uSystem[uc]!=uSystem[vc] ? vc : numberOfDimensions>2 ? wc : uc;

          // getSystemIndexBounds(Isv,system0);

        	  RealArray a,b,c,d,e;   // fix this ***************
                    ipar[30]=-1;        // form equations for this component (if equationsAreDifferent==true)
                  K1=Ixsv[0], K2=Ixsv[1], K3=Ixsv[2]; 
        //  // Note: Fill in the interior coefficients for points (K1,K2,K3), 
                  insLineSetupNew(numberOfDimensions,
                                            K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(), 
                             		 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
                             		 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
                                            a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1), 
                             		 a.getBase(2),a.getBound(2),                  
                             		 *maskLocal.getDataPointer(), *prsxy,   
                                            *uLocal.getDataPointer(), *gvp->getDataPointer(),  
                                            *((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),  
                                            direction, *a.getDataPointer(),*b.getDataPointer(),*c.getDataPointer(), 
                                            *d.getDataPointer(),*e.getDataPointer(),  bc(0,0), bcLocal(0,0), 
                                            bcData.getBase(0),bcData.getBound(0),
                                            bcData.getBase(1),bcData.getBound(1),
                                            bcData.getBase(2),bcData.getBound(2),
                                            bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
                                            ipar[0],rpar[0], ierr );

      	}
      	timing(parameters.dbase.get<int>("timeForLineImplicitFactor"))+=getCPU()-time0;

      	time1=getCPU();

      	if( debug() & 8 )
      	{
        	  display(fLocal,"Here is fLocal before assign line solver BC's",pDebugFile,"%6.2f ");
        	  fflush(pDebugFile);
      	}

        // ----------------------------------------
	// --- assign the RHS for BC's into f -----
        // ----------------------------------------
                if( ns1==0 && ns2==0 ) // we only need to assign the first time through
                	  assignLineSolverBoundaryConditions( grid,direction,u,f,numberOfTimeDependentComponents,isPeriodic );


      	if( debug() & 8 )
      	{
        	  display(fLocal,"Here is fLocal before solve",pDebugFile,"%6.2f ");
        	  fflush(pDebugFile);
      	}
      	

	// **************************************************************
	// *****************     Solve     ******************************
	// **************************************************************
      	for( int m=0; m<maxNumberOfSystems; m++ )
      	{
                    if( uSystem[m]==-1 ) continue;  // nothing to do for this system

          // getComponentIndexBounds(Kv,m);

                  for( int axis=0; axis<3; axis++ )
                  {
                      if( axis==direction )
                          Kv[direction]=Range(Ixsv[direction].getBase()-numGhost(0),Ixsv[direction].getBound()+numGhost(1));
                      else
                          Kv[axis]=Ixsv[axis];
                  }
        	  
        
        	  if( debug() & 8 )
        	  {
          	    fprintf(pDebugFile,"$$$ direction=%i component m=%i uSystem=%i solve for K=[%i,%i][%i,%i]\n",
                		    direction,m,uSystem[m],  K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound());

          	    display(fLocal(K1,K2,K3,m),sPrintF("Here is the RHS BEFORE triSolver for component m=%i",m),pDebugFile,"%8.1e ");
              	    fflush(pDebugFile);

        	  }
        	  if( debug() & 64 )
        	  { 
          	    Index I1,I2,I3;
          	    getIndex(mg.gridIndexRange(),I1,I2,I3);
          	    I1=Range(-1,1);
          	    display(fLocal(I1,I2,I3,Range(uc,vc)),sPrintF("\n RHS before triSolver for component m=%i",m),
                		    pDebugFile,"%7.4f ");
        	  }

          // ************************************************
          // ************** solve the system ****************
          // ********** (solve for u,v,w,n, or T )***********
          // ************************************************

        	  tridiagonalSolver(uSystem[m],direction,grid)->solve(fLocal(all,all,all,m),K1,K2,K3);


        	  if( debug() & 8 )
        	  {
          	    fprintf(pDebugFile,"$$$ direction=%i component m=%i uSystem=%i solve for K=[%i,%i][%i,%i]\n",
                		    direction,m,uSystem[m],  K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound());
          	    display(fLocal(K1,K2,K3,m),sPrintF("Here is the solution from the triSolver for component m=%i",m),pDebugFile,"%8.1e ");
              	    fflush(pDebugFile);
        	  }
        	  if( debug() & 64 )
        	  { 
          	    Index I1,I2,I3;
          	    getIndex(mg.gridIndexRange(),I1,I2,I3);
          	    I1=Range(-1,1);
          	    display(fLocal(I1,I2,I3,Range(uc,vc)),sPrintF("\n solution from triSolver for component m=%i",m),
                		    pDebugFile,"%7.4f ");
        	  }
      	
      	}  // for m (velocity component)
            
      	timing(parameters.dbase.get<int>("timeForLineImplicitSolve"))+=getCPU()-time1;
      	
            
            } // end for( ns ) -- loop that split up the number of line-solves done at one time. 
        

            time1=getCPU();

      // For now: always delete tridiagonal solvers after use
            for( int c=0; c<maxNumberOfSystems; c++ )
            {
      	delete tridiagonalSolver(c,direction,grid);
      	tridiagonalSolver(c,direction,grid)=NULL;
            }

            if( debug() & 8 )
      	display(fLocal,"AFTER ALL SOLVES Here is the solution from the tridiagonalSolver",pDebugFile,"%8.1e ");
        
    
            const int * maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
            const int maskDim0=maskLocal.getRawDataSize(0);
            const int maskDim1=maskLocal.getRawDataSize(1);
#undef MASK
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
            const real *fp = fLocal.Array_Descriptor.Array_View_Pointer3;
            const int fDim0=fLocal.getRawDataSize(0);
            const int fDim1=fLocal.getRawDataSize(1);
            const int fDim2=fLocal.getRawDataSize(2);
#undef F
#define F(i0,i1,i2,i3) fp[i0+fDim0*(i1+fDim1*(i2+fDim2*(i3)))]
            real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
            const int uDim0=uLocal.getRawDataSize(0);
            const int uDim1=uLocal.getRawDataSize(1);
            const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]


            int i1,i2,i3;
            real omega=1.,  omegam=1.-omega;

      // Update all points, including ghost points. 
              for( int axis=0; axis<3; axis++ )
              {
                  if( axis==direction )
                      Iv[direction]=Range(Ixv[direction].getBase()-numGhost(0),Ixv[direction].getBound()+numGhost(1));
                  else
                      Iv[axis]=Ixv[axis];
              }

            if( debug() & 4 )
      	fprintf(pDebugFile,"\n *** AFTER SOLVES: Update pts Iv=[%i,%i][%i,%i][%i,%i] of u from the tridiag. soln f ***\n\n",
                                Iv[0].getBase(),Iv[0].getBound(),Iv[1].getBase(),Iv[1].getBound(),Iv[2].getBase(),Iv[2].getBound());

            bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,1);
            if( ok )
            {
                assert( !useTurbulenceModel || !computeTemperature );  // both these cannot currently be on
      	
                const int cc = useTurbulenceModel ? nc : tc;  // assign nc or tc 
      	if( numberOfDimensions==2 )
      	{
        	  if( !useTurbulenceModel && !computeTemperature )
        	  {
          	    FOR_3D(i1,i2,i3,I1,I2,I3)
          	    {
            	      if( MASK(i1,i2,i3)>0 )
            	      {           
            		U(i1,i2,i3,uc)=omegam*U(i1,i2,i3,uc)+omega*F(i1,i2,i3,uc);
            		U(i1,i2,i3,vc)=omegam*U(i1,i2,i3,vc)+omega*F(i1,i2,i3,vc);
            	      }
          	    }
        	  }
        	  else
        	  {
          	    FOR_3D(i1,i2,i3,I1,I2,I3)
          	    {
            	      if( MASK(i1,i2,i3)>0 )
            	      {           
            		U(i1,i2,i3,uc)=omegam*U(i1,i2,i3,uc)+omega*F(i1,i2,i3,uc);
            		U(i1,i2,i3,vc)=omegam*U(i1,i2,i3,vc)+omega*F(i1,i2,i3,vc);
            		U(i1,i2,i3,cc)=omegam*U(i1,i2,i3,cc)+omega*F(i1,i2,i3,cc);
            	      }
          	    }
        	  }
      	}
      	else
      	{
        	  if( !useTurbulenceModel && !computeTemperature )
        	  {
          	    FOR_3D(i1,i2,i3,I1,I2,I3)
          	    {
            	      if( MASK(i1,i2,i3)>0 )
            	      {           
            		U(i1,i2,i3,uc)=omegam*U(i1,i2,i3,uc)+omega*F(i1,i2,i3,uc);
            		U(i1,i2,i3,vc)=omegam*U(i1,i2,i3,vc)+omega*F(i1,i2,i3,vc);
            		U(i1,i2,i3,wc)=omegam*U(i1,i2,i3,wc)+omega*F(i1,i2,i3,wc);
            	      }
          	    }
        	  }
          	    
        	  else
        	  {
          	    FOR_3D(i1,i2,i3,I1,I2,I3)
          	    {
            	      if( MASK(i1,i2,i3)>0 )
            	      {           
            		U(i1,i2,i3,uc)=omegam*U(i1,i2,i3,uc)+omega*F(i1,i2,i3,uc);
            		U(i1,i2,i3,vc)=omegam*U(i1,i2,i3,vc)+omega*F(i1,i2,i3,vc);
            		U(i1,i2,i3,wc)=omegam*U(i1,i2,i3,wc)+omega*F(i1,i2,i3,wc);
            		U(i1,i2,i3,cc)=omegam*U(i1,i2,i3,cc)+omega*F(i1,i2,i3,cc);
            	      }
          	    }
        	  }
      	}
            }
        
      // -----------------------------------------------------------------------
      // fill in div(u) BC for a slip wall
      // -----------------------------------------------------------------------
            for( int side=0; side<=1; side++ )
            {
      	const int bc0=mg.boundaryCondition(side,direction);

      	int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
      	is1=is2=is3=0;
      	isv[direction]=1-2*side;

        // NOTE: for curvilinear grids, div(u)=0 is set on slip walls in insBC
      	if( isRectangular && bc0==Parameters::slipWall )
      	{
        	  const int is=1-2*side;
        	  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
        	  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
        	  Index Jgv[3], &Jg1=Jgv[0], &Jg2=Jgv[1], &Jg3=Jgv[2];

                    int extra=-1;  // *wdh* 070921 do not include end points -- but what about where multiple slip walls meet ??

        	  getGhostIndex(mg.gridIndexRange(),side,direction,Ig1,Ig2,Ig3,1,extra);
        	  getBoundaryIndex(mg.gridIndexRange(),side,direction,Ib1,Ib2,Ib3,extra);

        	  bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,1);
        	  if( !ok ) continue;
        	  ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,1);
      	
          // *wdh* 070814
	  // in parallel -- watch out for bounds in tangential directions: we use: Ib2-1, Ib2+1 etc. 
        	  for( int axis=1; axis<numberOfDimensions; axis++ )
        	  {
          	    int dir = (direction+axis) % numberOfDimensions;  // tangential direction
          	    if( Ibv[dir].getBase()==uLocal.getBase(dir) )
          	    {
            	      Ibv[dir]=Range(Ibv[dir].getBase()+1,Ibv[dir].getBound());
            	      Igv[dir]=Range(Igv[dir].getBase()+1,Igv[dir].getBound());
          	    }
          	    if( Ibv[dir].getBound()==uLocal.getBound(dir) )
          	    {
            	      Ibv[dir]=Range(Ibv[dir].getBase(),Ibv[dir].getBound()-1);
            	      Igv[dir]=Range(Igv[dir].getBase(),Igv[dir].getBound()-1);
          	    }
        	  }
        	  

        	  if( numberOfDimensions==2 )
        	  {
	    // u.x+v.y=0
          	    if( direction==0 )
            	      uLocal(Ig1,Ig2,Ig3,uc)=uLocal(Ib1+is,Ib2,Ib3,uc)+(is*dx[0]/dx[1])*(uLocal(Ib1,Ib2+1,Ib3,vc)-uLocal(Ib1,Ib2-1,Ib3,vc));
          	    else 
            	      uLocal(Ig1,Ig2,Ig3,vc)=uLocal(Ib1,Ib2+is,Ib3,vc)+(is*dx[1]/dx[0])*(uLocal(Ib1+1,Ib2,Ib3,uc)-uLocal(Ib1-1,Ib2,Ib3,uc));
        	  }
        	  else // 3d 
        	  {
	    // u.x+v.y+w.z=0
          	    if( direction==0 )
            	      uLocal(Ig1,Ig2,Ig3,uc)=(uLocal(Ib1+is,Ib2,Ib3,uc)+
                              				      (is*dx[0]/dx[1])*(uLocal(Ib1,Ib2+1,Ib3,vc)-uLocal(Ib1,Ib2-1,Ib3,vc))+
                              				      (is*dx[0]/dx[2])*(uLocal(Ib1,Ib2,Ib3+1,wc)-uLocal(Ib1,Ib2,Ib3-1,wc)));
          	    else if( direction==1 )
            	      uLocal(Ig1,Ig2,Ig3,vc)=(uLocal(Ib1,Ib2+is,Ib3,vc)+
                              				      (is*dx[1]/dx[0])*(uLocal(Ib1+1,Ib2,Ib3,uc)-uLocal(Ib1-1,Ib2,Ib3,uc))+
                              				      (is*dx[1]/dx[2])*(uLocal(Ib1,Ib2,Ib3+1,wc)-uLocal(Ib1,Ib2,Ib3-1,wc)));
          	    else
          	    {
            	      uLocal(Ig1,Ig2,Ig3,wc)=(uLocal(Ib1,Ib2,Ib3+is,wc)+
                              				      (is*dx[2]/dx[0])*(uLocal(Ib1+1,Ib2,Ib3,uc)-uLocal(Ib1-1,Ib2,Ib3,uc))+
                              				      (is*dx[2]/dx[1])*(uLocal(Ib1,Ib2+1,Ib3,vc)-uLocal(Ib1,Ib2-1,Ib3,vc)));
          	    }
        	  }

          // Now extrapolate the "ends"
                    for( int a=1; a<numberOfDimensions; a++ )
        	  {
          	    int axis= (direction+a) % numberOfDimensions; // tangential direction
          	    Jg1=Ig1, Jg2=Ig2, Jg3=Ig3;
          	    for( int side2=0; side2<=1; side2++ )
          	    {
              // make sure the "end" is on this processor
                            if( mg.boundaryCondition(side2,axis)>=0 &&
                                    (side2==0 && Igv[axis].getBase()  == mg.gridIndexRange(side2,axis)+1 ) ||
                                    (side2==1 && Igv[axis].getBound() == mg.gridIndexRange(side2,axis)-1 ) )
            	      {
            		Jgv[axis]=mg.gridIndexRange(side2,axis);

            		int cc = uc+direction;
          	    
		// uLocal(Jg1,Jg2,Jg3,cc)=2.*uLocal(Jg1+is1,Jg2+is2,Jg3+is3,cc)-uLocal(Jg1+2*is1,Jg2+2*is2,Jg3+2*is3,cc);
                // printF("... Extrap slipWall end: (side,direction)(%i,%i), (side2,axis)=(%i,%i) "
                //        "Jgv=[%i,%i][%i,%i][%i,%i]\n",
		//        side,direction, side2,axis, Jgv[0].getBase(),Jgv[0].getBound(),
		//        Jgv[1].getBase(),Jgv[1].getBound(),Jgv[2].getBase(),Jgv[2].getBound());
            		
            		uLocal(Jg1,Jg2,Jg3,cc)=( 3.*uLocal(Jg1+  is1,Jg2+  is2,Jg3+  is3,cc)
                                                                                -3.*uLocal(Jg1+2*is1,Jg2+2*is2,Jg3+2*is3,cc)
                                					  + uLocal(Jg1+3*is1,Jg2+3*is2,Jg3+3*is3,cc) );
            		
            	      }
          	    }
        	  }

      	}
            }// end for side
        


            timing(parameters.dbase.get<int>("timeForLineImplicitSolve"))+=getCPU()-time1;

        } // end if( !computeTheResidual )
    
    

        if( computeTheResidual || alwaysComputeResidual )
        {
      // *************************************************************
      // ***********Compute the Residual******************************
      // *************************************************************

            time1=getCPU();
        
            if( twilightZoneFlow )
            {
	// In this case we saved the RHS forcing in the residual array
      	fLocal(all,all,all,N)=rLocal(all,all,all,N);
            }
            else
            {
      	fLocal(all,all,all,N)=0.;
            }
        
            rLocal=0.;  // **************
    
            getIndex(mg.indexRange(),I1,I2,I3,-1);  // interior points, ignore residuals on boundaries
            int includeGhost=0;
            bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
            if( ok )
            {
      	computeResidualNew(numberOfDimensions,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(), 
                  			I3.getBase(),I3.getBound(), 
                  			uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                  			uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
                  			*maskLocal.getDataPointer(), *prsxy,
                  			*uLocal.getDataPointer(), *gvp->getDataPointer(),
                  			*((*pdtVar)[grid].getDataPointer()), *fLocal.getDataPointer(), *dwp->getDataPointer(),
                  			*rLocal.getDataPointer(), bc(0,0), ipar[0],rpar[0], ierr );
            }
        

      // ** residual.getOperators()->setTwilightZoneFlow(false);
      // ** real time=0.;
      // ** residual.applyBoundaryCondition(N,BCTypes::dirichlet,BCTypes::allBoundaries,0.,time);
      // ** residual.interpolate();  // interpolate the residual for plotting 
      // ** residual.getOperators()->setTwilightZoneFlow(twilightZoneFlow);

      // RealArray maxRes(numberOfTimeDependentComponents);
            timing(parameters.dbase.get<int>("timeForLineImplicitResidual"))+=getCPU()-time1;
        
        }  // end if compute the residual
    
    }  // end if( ok ) // there are points on this processor
    

  // Update parallel ghost boundaries -- should this be done here ? -- only for direction==0 ??
    u.updateGhostBoundaries();
  // this may be needed if the periodic direction is split in parallel:
    u.periodicUpdate();

    if( !computeTheResidual )
    {
        if( debug() & 8 )
        {
            fflush(pDebugFile);
      // MPI_Barrier(Overture::OV_COMM);

            if( twilightZoneFlow )
            {
      	OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
      	Index I1,I2,I3;
      	getIndex(mg.dimension(),I1,I2,I3);

                bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
                if( ok )
      	{
                    const realArray & x= mg.center();
                    #ifdef USE_PPP
                        realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
                    #else
                        const realSerialArray & xLocal = x;
                    #endif

                    realSerialArray ue(I1,I2,I3);
        	  bool isRectangularForTZ=false;
                    e.gd( ue,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,I1,I2,I3,uc,t);
        	  display(fabs(uLocal(I1,I2,I3,uc)-ue),"ERROR in u at end",pDebugFile,"%8.2e ");
                    e.gd( ue,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,I1,I2,I3,vc,t);
        	  display(fabs(uLocal(I1,I2,I3,vc)-ue),"ERROR in v at end",pDebugFile,"%8.2e ");
      	}
      	

      	fprintf(pDebugFile," lineSolve: errors after solve, direction=%i t=%e \n",direction,gf[current].t);
      	determineErrors( gf[current] ) ;
            }
        }
        
    } // end if( !computeTheResidual )

    
    if( ( computeTheResidual || alwaysComputeResidual ) && debug() & 4 )
    {
        real time2=getCPU();
        printF("lineSolve: grid=%i max residuals from computeResidual are: [",grid);
        getIndex(mg.gridIndexRange(),I1,I2,I3);
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
        for(int n=0; n<numberOfTimeDependentComponents; n++ )
        {
            real resMax=0.;
            if( ok )
      	resMax=max(fabs(rLocal(I1,I2,I3,n+uc)));

            resMax=ParallelUtility::getMaxValue(resMax);
            printF(" %8.2e,",  resMax);
        }
        printF("]\n");
        timing(parameters.dbase.get<int>("timeForLineImplicitResidual"))+=getCPU()-time2;
    }

    return 0;
}

int Cgins::
getLineSolverBoundaryConditions(const int grid, const int direction, 
                                                                realMappedGridFunction & u,
                        				Index *Iv,
                        				const int maxNumberOfSystems, int *uSystem, 
                        				const int numberOfTimeDependentComponents,
                        				IntegerArray & bc, IntegerArray & numGhost,
                        				int & numberOfDifferentLineSolverBoundaryConditions )
//===========================================================================================================
// /Description:
//    Determine the matrix (line solver) boundary conditions,  and the number of ghost lines to include,
//    and the Index bounds, Iv[3], for the tridiangonal systems, and the system specification uSystem[.]. 
// 
// /grid, direction (input) : grid and line solver direction
// /Iv (output) : master index bounds (derive bounds for components using this and the numGhost array)
// /uSystem[] (input/output) : uSystem[component] is the system number of a given component
// 
// /numberOfTimeDependentComponents (input) :  the number of time dependent components 
// 
// /bc(0:1,system) (output) : sets the BC for the line solver. 
//                   periodic=-1, interpolate=0, dirichlet=1, neumann=2, extrapolate=3;
// /numGhost(0:1) (output) : Adjust the range in the line solver direction by numGhost(side). 
//       numGhost(side)=0 for interpolation (and parallel) boundaries.
// 
//===========================================================================================================
{
  // printF(" *new* getLineSolverBoundaryConditions \n");
    
    real t=0.;  // ********************* what time should we use ? 

    MappedGrid & mg = *u.getMappedGrid();

    const int numberOfDimensions=cg.numberOfDimensions();
    const intArray & mask = mg.mask();
    Range all;

    Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2]; // for first system (tangential components for slip wall case)

    const bool fourthOrder = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");
    if( debug() & 8 && fourthOrder )
        printF(" **** advanceLineSolve:INFO: solving penta-diagonal systems *****\n");
    

    bool isRectangular= mg.isRectangular();
    bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    
    const int pc=parameters.dbase.get<int >("pc");
    const int uc=parameters.dbase.get<int >("uc");
    const int vc=parameters.dbase.get<int >("vc");
    const int wc=parameters.dbase.get<int >("wc");
    const int nc=parameters.dbase.get<int >("kc");  // for SA turbulence model.
    const int tc=parameters.dbase.get<int >("tc");  // Temperature

    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

    const real nu = parameters.dbase.get<real >("nu");
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
    RealArray & timing = parameters.dbase.get<RealArray>("timing");
    Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
    
    const bool useTurbulenceModel= turbulenceModel==Parameters::SpalartAllmaras;
    InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
    const bool computeTemperature = pdeModel==InsParameters::BoussinesqModel ||
                                                                    pdeModel==InsParameters::viscoPlasticModel;

    bool momentumMatrixEquationsAreDifferent = pdeModel==InsParameters::viscoPlasticModel;

    Range N(uc,uc+numberOfTimeDependentComponents-1);
    Range V(uc,uc+numberOfDimensions-1);

    const int fc=uc;   // put forcing for uc here, vc at fc+1, wc at fc+2

  // The bcData array is used to access the mixed-derivative BC info for T
    const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");


    bool alwaysComputeResidual=debug() & 4; //  true;  // false


    #ifdef USE_PPP
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    #else
        const intSerialArray & maskLocal = mask;
        realSerialArray & uLocal = u;
    #endif

    getIndex(mg.extendedIndexRange(),I1,I2,I3);  // include boundary  -- holds boundary conditions

    bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);  // do NOT include parallel ghost

    if( debug() & 4 ) fprintf(pDebugFile," XXXX lineSolverBC: start: I1=[%i,%i] I2=[%i,%i]\n",
                     			   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());


  // *******************************************************
  // *** boundary conditions for the tridiagonal system ****
  // *******************************************************
  //    interpolate: the line solve hits an interpolation boundary where the value is assumed given
  //
    const int periodic=-1, interpolate=0, dirichlet=1, neumann=2, extrapolate=3;


    numGhost=0;
    
    bc=dirichlet;  // ** set the default boundary condition **

    const int numberOfGhostLines= fourthOrder ? 2 : 1;

// common/src/getBounds.C --> getLocalBoundsAndBoundaryConditions
// From mp/src/assignInterfaceBoundaryConditions.C

    const IntegerArray & dim = mg.dimension();
    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal );


    Range S = maxNumberOfSystems;

  // ********* do NOT include ghost lines at interpolation or parallel boundaries **************
    for( int side=0; side<=1; side++ )
    {
        int bc0=mg.boundaryCondition(side,direction);

        if( dimLocal(side,direction)!=dim(side,direction) ||
                ( bc0<0 && dimLocal(1-side,direction)!=dim(1-side,direction)) )  // periodic boundary that is "split" 
        {
      // parallel boundaries -- for now assume values are given (i.e. lag values)
            bc(side,S)=dirichlet;
      // The dirichlet BC is applied at the first parallel ghost: 
//        Iv[direction]= side==0 ? Range(Iv[direction].getBase()-1,Iv[direction].getBound()  ) :
// 	 Range(Iv[direction].getBase()  ,Iv[direction].getBound()+1);
            
              numGhost(side)=numberOfGhostLines; 
            
        }
        else 
        {

            if( bc0==0 )
            {
      	bc(side,S)=interpolate;
            }
            else if( bc0<0 )
            {
      	bc(side,S)=periodic;
            }
            else if( bc0==Parameters::noSlipWall || 
             	       bc0==InsParameters::inflowWithVelocityGiven ||
                              bc0==Parameters::dirichletBoundaryCondition )
            {
      	numGhost(side)=numberOfGhostLines; 
            }
            else if( bc0==Parameters::slipWall || ( bc0==InsParameters::outflow && isRectangular) )
            {
      	numGhost(side)=numberOfGhostLines; 

                
      	if( !isRectangular && numberOfSlipWallErrorMessages<10 )
      	{
          // what we could do is form a linear combination of the interior equation and the n.u BC 
          //        BC:    (n.u)n + (tau.E) tau  =  g n + f tau   (this implies n.u=g, tau.E=f 
          //           ->  (n.u)n + E - (n.E)n   =  g n + f - (n.f)n 
        	  numberOfSlipWallErrorMessages++;
        	  printF("LineSolver:WARNING: The line solver using scalar tridiagonal matrices cannot handle "
                                  "curved slip walls!\n"
                                  "This may still work if the grid faces are flat and match those of a rectangle\n");
      	}
      	if( bc0==Parameters::slipWall )
      	{
                    for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    if( axis!=direction )
          	    {
            	      bc(side,uc+axis)=neumann; // the tangential velocity components are Neumann BC for slip wall
          	    }
        	  }
      	}
      	else if( bc0==InsParameters::outflow )
      	{
          // extrapolate all variables at outflow except Neumann BC on the normal component 
                    bc(side,S)=extrapolate;
          // Rectangular : use neumann on the normal component:
        	  bc(side,uc+direction)=neumann;

        	  numGhost(side)=numberOfGhostLines;
      	}
      	else
      	{
                    Overture::abort("lineSolverBoundaryConditions:ERROR");
      	}

            }
            else if( bc0==InsParameters::outflow )
            {
      	assert( !isRectangular );
            
      	bc(side,S)=extrapolate;  // extrap all components in this case
      	numGhost(side)=numberOfGhostLines;
            }
            else
            {
      	printF("lineSolverBoundaryConditions:ERROR: unexpected BC, bc=%i for (side,direction,grid)=(%i,%i,%i)\n",
             	       bc0,side,direction,grid);
                Overture::abort("lineSolverBoundaryConditions:ERROR");
            }
            
            if( computeTemperature )
            {

      	if( bc0==Parameters::noSlipWall || 
          	    bc0==InsParameters::inflowWithVelocityGiven ||
          	    bc0==Parameters::slipWall  )
      	{
	  // we need to check for a mixed-BC on T 
        	  int axis=direction;
            
        	  if( bc0==Parameters::slipWall )  // ** fix this ***
        	  {
	    // Set coefficients of mixed BC -> neumann for T : (should we do this somewhere else?)
          	    mixedCoeff(tc,side,direction,grid)=0.; 
          	    mixedNormalCoeff(tc,side,direction,grid)=1.; 
          	    mixedRHS(tc,side,direction,grid)=0.;
        	  }
        	  
        	  assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
        	  
        	  if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
        	  {
	    // adiabaticNoSlipWall=true;
          	    if( bc0==InsParameters::inflowWithVelocityGiven || debug() & 4 )
            	      printF("++++lineSolverBC: Mixed BC for T: (grid,side,axis)=(%i,%i,%i), %3.2f*T+%3.2f*T.n=%3.2f,  \n",
                 		     grid,side,axis, 
                 		     mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid));

          	    bc(side,tc)=neumann;   

	    // Overture::abort("ERROR: finish me!");
        	  }
      	}
      	else if( bc0==InsParameters::dirichletBoundaryCondition )
      	{
        	  bc(side,tc)=dirichlet;
      	}
      	else if( bc0==InsParameters::outflow )
      	{  
          // -- set the coefficients for the T BC to be Neumann at outflow 
        	  int axis=direction;

        	  mixedCoeff(tc,side,axis,grid)=0.;
        	  mixedNormalCoeff(tc,side,axis,grid)=1.;
        	  mixedRHS(tc,side,axis,grid)=0.;

        	  if( false )
        	  {
          	    printF("+lineSolverBC: Outflow: Mixed BC for T: (grid,side,axis)=(%i,%i,%i), %3.2f*T+%3.2f*T.n=%3.2f,  \n",
               		   grid,side,axis, 
               		   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid));
        	  }
        	  
                    bc(side,tc)=neumann; // use Neumann BC for T at outflow

	  // bc(side,tc)=extrapolate; // T is extrapolated
      	}
      	else if( bc0>0 )
      	{
        	  printF("getLineSolverBC: Set T BC: ERROR: unexpected BC =%i\n",bc0);
        	  Overture::abort("error");
      	}
            }
  
            
        } // end else not parallel boundary
    }

  // *new* way 
    numberOfDifferentLineSolverBoundaryConditions=0;
    for( int side=0; side<=1; side++ )for( int axis=0; axis<numberOfDimensions; axis++ )
    {
        if( bcLocal(side,axis)==Parameters::noSlipWall ||
                bcLocal(side,axis)==InsParameters::inflowWithVelocityGiven ||
                (bcLocal(side,axis)==InsParameters::outflow && !isRectangular) ||
                bcLocal(side,axis)==Parameters::dirichletBoundaryCondition ||
                bcLocal(side,axis)<=0 )
        {
      // These BC's are the same condition for [u,v,w]
        }
        else if( bcLocal(side,axis)==Parameters::slipWall ||
                (bcLocal(side,axis)==InsParameters::outflow && isRectangular) )
        {
            if( numberOfDifferentLineSolverBoundaryConditions==0 )
            {
      	if( axis==0 )
      	{ // uc=normal component, (vc,wc) = tangential 
        	  uSystem[uc]=uc; uSystem[vc]=vc;  if( numberOfDimensions>2 ) uSystem[wc]=vc;
      	}
      	else if( axis==1 )
      	{
        	  uSystem[uc]=uc; uSystem[vc]=vc;  if( numberOfDimensions>2 ) uSystem[wc]=uc;
      	}
      	else
      	{
        	  uSystem[uc]=uc; uSystem[vc]=uc;  uSystem[wc]=wc;  // (u,v) are both tangential
      	}
            }
            else
            {
                uSystem[uc]=uc; uSystem[vc]=vc; uSystem[wc]=wc;
                break;
            }

            numberOfDifferentLineSolverBoundaryConditions++;

        }
        else
        {
            printF("getLineSolverBC: ERROR: unexpected BC: bc(%i,%i)=%i\n",side,axis,bcLocal(side,axis));
            Overture::abort("error");
        }
        
    }
    

    if( debug() & 4 ) 
        fprintf(pDebugFile," XXXX lineSolverBC: DONE: I1=[%i,%i] I2=[%i,%i], bc=[%i,%i][%i,%i][%i,%i]\n",
                      			    I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
                                            bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2));

    return 0;
}


// ----------------------------------------------------------------------------------------
// Macro:  Assign the RHS for the Temperature
// ----------------------------------------------------------------------------------------


int Cgins::
assignLineSolverBoundaryConditions(const int grid, const int direction, 
                           				   realMappedGridFunction & u, 
                           				   realMappedGridFunction & f, 
                           				   const int numberOfTimeDependentComponents,
                                                                      bool isPeriodic[3] )
//===========================================================================================================
// /Description:
//    Fill in the RHS f with the boundary conditions for the line solver.
// 
// /grid, direction (input) : grid and line solver direction
// /f (input/output) : The BC's are assigned here
// /numberOfTimeDependentComponents (input) :  the number of time dependent components 
// 
// 
//===========================================================================================================
{
  // printF(" *new* assignLineSolverBoundaryConditions \n");
    
    real t=0.;  // ********************* what time should we use ? 

    MappedGrid & mg = *u.getMappedGrid();

    const int numberOfDimensions=cg.numberOfDimensions();
    const intArray & mask = mg.mask();
    Range all;

    const bool fourthOrder = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");
    if( debug() & 8 && fourthOrder )
        printF(" **** advanceLineSolve:INFO: solving penta-diagonal systems *****\n");
    

    bool isRectangular= mg.isRectangular();
    bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    
    const int pc=parameters.dbase.get<int >("pc");
    const int uc=parameters.dbase.get<int >("uc");
    const int vc=parameters.dbase.get<int >("vc");
    const int wc=parameters.dbase.get<int >("wc");
    const int nc=parameters.dbase.get<int >("kc");  // for SA turbulence model.
    const int tc=parameters.dbase.get<int >("tc");  // Temperature

    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

    const real nu = parameters.dbase.get<real >("nu");
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
    RealArray & timing = parameters.dbase.get<RealArray>("timing");
    Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
    
    const bool useTurbulenceModel= turbulenceModel==Parameters::SpalartAllmaras;
    InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
    const bool computeTemperature = pdeModel==InsParameters::BoussinesqModel ||
                                                                    pdeModel==InsParameters::viscoPlasticModel;

    bool momentumMatrixEquationsAreDifferent = pdeModel==InsParameters::viscoPlasticModel;

    Range N(uc,uc+numberOfTimeDependentComponents-1);
    Range V(uc,uc+numberOfDimensions-1);

//  const int fc=uc;   // put forcing for uc here, vc at fc+1, wc at fc+2

  // The bcData array is used to access the mixed-derivative BC info for T
    const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");


    #ifdef USE_PPP
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
    #else
        const intSerialArray & maskLocal = mask;
        realSerialArray & uLocal = u;
        realSerialArray & fLocal = f;
    #endif

    const IntegerArray & dim = mg.dimension();
    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal );


  // *******************************************************
  // *** boundary conditions for the tridiagonal system ****
  // *******************************************************
  //    interpolate: the line solve hits an interpolation boundary where the value is assumed given
  //
  // const int periodic=-1, interpolate=0, dirichlet=1, neumann=2, extrapolate=3;


    real dx[3]={1.,1.,1.};
    if( isRectangular  )
          mg.getDeltaX(dx);

    if( twilightZoneFlow )
    {
        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal );
    }
    
      
  // -------------------------------------------
  // ------------ fill in BC's  ----------------
  // -------------------------------------------

    Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
    Index Igv[3], &Ig1=Igv[0],&Ig2=Igv[1],&Ig3=Igv[2];
    Index Ipv[3], &Ip1=Ipv[0],&Ip2=Ipv[1],&Ip3=Ipv[2];

  //  Index Jgv[3], &Jg1=Jgv[0],&Jg2=Jgv[1],&Jg3=Jgv[2];

    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
    is1=is2=is3=0;
        
  // --- first assign tangential boundaries ---
  //   (fill in dirichlet BC values)
    for( int axis=0; axis<numberOfDimensions; axis++ )for( int side=0; side<=1; side++ )
    {
        if( axis==direction ) continue;
        
        const int is=1-2*side;
        isv[direction]=is;
            
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,1);
        if( !ok ) continue;

        if( bcLocal(side,axis)==Parameters::noSlipWall || 
      	bcLocal(side,axis)==InsParameters::inflowWithVelocityGiven ||
      	bcLocal(side,axis)==Parameters::dirichletBoundaryCondition )
        {
            fLocal(Ib1,Ib2,Ib3,V)=uLocal(Ib1,Ib2,Ib3,V); // assign velocity components
      	
            if( computeTemperature )
            {
	// set T if there is a dirichlet BC (but not mixed BC)
      	real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid), rhs=mixedRHS(tc,side,axis,grid);
      
      	assert( a0!=0. || a1!=0. );
        	if( a1==0. ) // coeff of T.n is zero -> Dirichlet BC
      	{
        	  if( a0!=1. ) printF("WARNING: dirichlet BC for T but a0=%e\n",a0);
        	  
        	  fLocal(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc); 
      	}

            }

        }
        else if( bcLocal(side,axis)==InsParameters::outflow )
        {
        }
        else if( bcLocal(side,axis)==Parameters::slipWall )
        {
      // the normal component has a dirichlet BC: 
            int cc= uc+axis;
            fLocal(Ib1,Ib2,Ib3,cc)=uLocal(Ib1,Ib2,Ib3,cc);  
        }
        else if( bcLocal(side,axis)>0 )
        {
            Overture::abort("insLineSolve BC : finish me");
            
        }
        
        if( useTurbulenceModel && 
      	( bcLocal(side,axis)==InsParameters::inflowWithVelocityGiven ||
        	  bcLocal(side,axis)==Parameters::dirichletBoundaryCondition ) )
        {
      // turbulence n is given at inflow and dirichlet boundaries
            fLocal(Ib1,Ib2,Ib3,nc)=uLocal(Ib1,Ib2,Ib3,nc);
        }


    }
    



    for( int side=0; side<=1; side++ )
    {
        int bc0=mg.boundaryCondition(side,direction);

        if( dimLocal(side,direction)!=dim(side,direction) || 
              ( bc0<0 && dimLocal(1-side,direction)!=dim(1-side,direction) ))
        {
      // parallel boundaries : assign the ghost value as dirichlet 
            getGhostIndex(gidLocal,side,direction,Ig1,Ig2,Ig3);   // first parallel ghost line
      // For periodic boundaries, we assign the boundary value on the far right.
            if( bc0<0 && side==1 && dimLocal(side,direction)==dim(side,direction) )
      	Igv[direction]=Igv[direction].getBase()-1;
      	
            fLocal(Ig1,Ig2,Ig3,V)=uLocal(Ig1,Ig2,Ig3,V); // assign velocity components

            if( useTurbulenceModel )
      	fLocal(Ig1,Ig2,Ig3,nc)=uLocal(Ig1,Ig2,Ig3,nc);

            if( computeTemperature )
                fLocal(Ig1,Ig2,Ig3,tc)=uLocal(Ig1,Ig2,Ig3,tc);

            continue;
        }
            
        const int is=1-2*side;
        isv[direction]=is;
            
     // *wdh* 090209 -- do not include ghost for outflow BC -- we don't solve on lines here anyway
        const int includeGhost=0; 
        getBoundaryIndex(mg.gridIndexRange(),side,direction,Ib1,Ib2,Ib3);
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
        if( !ok ) continue;

        getGhostIndex(mg.gridIndexRange(),side,direction,Ig1,Ig2,Ig3);   // first ghost line
        ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
        assert( ok );
        
        getGhostIndex(mg.gridIndexRange(),side,direction,Ip1,Ip2,Ip3,-1); // first line inside
        ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ip1,Ip2,Ip3,includeGhost);
        assert( ok );
        

        if( bc0==Parameters::noSlipWall || 
      	bc0==InsParameters::inflowWithVelocityGiven ||
      	bc0==Parameters::dirichletBoundaryCondition )
        {
      // fprintf(debugFile," FILL noSlipWall: [ia,ib]=[%i,%i] [ja,jb]=[%i,%i] \n",ia,ib,ja,jb);

      // -- combine these:
            fLocal(Ib1,Ib2,Ib3,V)=uLocal(Ib1,Ib2,Ib3,V); // assign velocity components
            fLocal(Ig1,Ig2,Ig3,V)=uLocal(Ig1,Ig2,Ig3,V); // set ghost line values too
      	
            if( useTurbulenceModel )
            {
      	fLocal(Ib1,Ib2,Ib3,nc)=uLocal(Ib1,Ib2,Ib3,nc);
      	fLocal(Ig1,Ig2,Ig3,nc)=uLocal(Ig1,Ig2,Ig3,nc);
            }
        	  

      // Assign the rhs for the Temperature
              if( computeTemperature )
              { // Look for mixed BC for T
                  int axis=direction;
                  real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid), rhs=mixedRHS(tc,side,axis,grid);
                  assert( a0!=0. || a1!=0. );
                  if( a1==0. ) // coeff of T.n is zero -> Dirichlet BC
                  {
                      fLocal(Ib1,Ib2,Ib3,tc)=uLocal(Ib1,Ib2,Ib3,tc); 
                      fLocal(Ig1,Ig2,Ig3,tc)=uLocal(Ig1,Ig2,Ig3,tc); 
                  }
                  else  // coeff of T.n is non-zero
                  {
                      if( debug() & 4 )
                  	printF("++++assign line solver mixed BC for T: (grid,side,axis)=(%i,%i,%i), %3.2f*T+%3.2f*T.n=%3.2f,  \n",
                         	       grid,side,axis, a0, a1, rhs );
           // The line solver matrix holds the coefficients of the mixed-BC along a line:
           // a0*u + a1*u.n = 
           // a0*u + a1*( n1*( rx*ur + sx*us ) + n2*( ry*ur + sy*us ) )
           //  n1 = rsxy(dir,0), n2=rsxy(dir,1)
                      if( twilightZoneFlow )
                          rhs=0.;  // turn off user defined RHS for TZ 
                      if( isRectangular )
                      {
                  	fLocal(Ig1,Ig2,Ig3,tc)=rhs;
                      }
                      else
                      {
             // --- we should optimize this ---
                          #ifdef USE_PPP
                            const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
                   	 realSerialArray rsxy; getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rsxy);
                          #else
                   	 const realSerialArray & rsxy = mg.inverseVertexDerivative();
                            const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
                          #endif
      	 // RSXY: index the rsxy array as a 5D array: 
                          #define RSXY(I1,I2,I3,m1,m2) rsxy(I1,I2,I3,(m1)+numberOfDimensions*(m2))
                          MappedGridOperators & op = *u.getOperators();
             // a1*( an1*( rsxy(dir,0)*u_d - ux2()) + an2*( rsxy(dir,1)*u_d - uy2() ) + ...
                          RealArray ur(Ib1,Ib2,Ib3);
                          MappedGridOperators::derivativeTypes rDeriv = MappedGridOperators::derivativeTypes(MappedGridOperators::r1Derivative+direction); // "normal" r-derivative
                          if( false )
                  	{
               // ** fix this ** the parameter derivatives are NOT defined!
                    	  op.derivative(rDeriv,uLocal,ur ,Ib1,Ib2,Ib3,tc); // ur <- T.r1 or T.r2 or T.r3 
                  	}
                  	else
                  	{
                    	  getGhostIndex(mg.gridIndexRange(),side,direction,Ip1,Ip2,Ip3,-1); // first line inside
                    	  ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ip1,Ip2,Ip3,1);
                    	  ur(Ib1,Ib2,Ib3)=(uLocal(Ip1,Ip2,Ip3,tc)-uLocal(Ig1,Ig2,Ig3,tc))/(is*2.*mg.gridSpacing(direction));
                  	}
      	// ::display(ur," mixed BC: ur");
                          RealArray ux(Ib1,Ib2,Ib3),uy(Ib1,Ib2,Ib3);
                          op.derivative(MappedGridOperators::xDerivative,uLocal,ux ,Ib1,Ib2,Ib3,tc);
                          op.derivative(MappedGridOperators::yDerivative,uLocal,uy ,Ib1,Ib2,Ib3,tc);
             // RHS = LHS*u - [ a0*u + a1*T.n ]
             // LHS*u = a0*u + a1*( an1*( rsxy(dir,0)*ur ) + an2*( rsxy(dir,1)*ur ) ) 
                  	fLocal(Ig1,Ig2,Ig3,tc) = rhs + a1*( 
                                    normal(Ib1,Ib2,Ib3,0)*( RSXY(Ib1,Ib2,Ib3,direction,0)*ur-ux ) + 
                                    normal(Ib1,Ib2,Ib3,1)*( RSXY(Ib1,Ib2,Ib3,direction,1)*ur-uy ) );
                          if( numberOfDimensions==3 )
                  	{
                              op.derivative(MappedGridOperators::zDerivative,uLocal,ux ,Ib1,Ib2,Ib3,tc); // ux <- T.z 
                    	  fLocal(Ig1,Ig2,Ig3,tc) += a1*( normal(Ib1,Ib2,Ib3,2)*( RSXY(Ib1,Ib2,Ib3,direction,2)*ur-ux ) );
                  	}
      	// ::display(ux," mixed BC: ux");
      	// ::display(uy," mixed BC: uy");
      	// ::display(fLocal(Ig1,Ig2,Ig3,tc)," fLocal(Ig1,Ig2,Ig3,tc)");
                          #undef RSXY
                      }
                      if( twilightZoneFlow )
                      {
                          OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                          #ifdef USE_PPP
                            const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
                   	 realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
                          #else
                   	 const realSerialArray & xLocal = mg.center();
                            const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
                          #endif
                          realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3);
                          bool isRectangularForTZ=false;
                          e.gd( ue ,xLocal,numberOfDimensions,isRectangularForTZ,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                          e.gd( uex,xLocal,numberOfDimensions,isRectangularForTZ,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                          e.gd( uey,xLocal,numberOfDimensions,isRectangularForTZ,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                  	fLocal(Ig1,Ig2,Ig3,tc) += a0*ue + normal(Ib1,Ib2,Ib3,0)*uex + normal(Ib1,Ib2,Ib3,1)*uey;
                  	if( numberOfDimensions==3 )
                  	{
                    	  e.gd( uex,xLocal,numberOfDimensions,isRectangularForTZ,0,0,0,1,Ib1,Ib2,Ib3,tc,t);  // uex = T.z
                              fLocal(Ig1,Ig2,Ig3,tc) +=normal(Ib1,Ib2,Ib3,2)*uex;
                  	}
                      }
                  }
              } // end if computeTemperature

        }
        else if( bc0==Parameters::slipWall )
        {

            int fc1= direction==0 ? uc : direction==1 ? vc : wc;  // normal component
            int fc2= direction==0 ? vc : direction==1 ? uc : uc;  // tangential component 
            int fc3= direction==0 ? wc : direction==1 ? wc : vc;  // another tangential component

            if( twilightZoneFlow )
            {
      	OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

#ifdef USE_PPP
      	realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
#else
      	const realSerialArray & xLocal = mg.center();
#endif

//            printf("slipWall: Assign dirichlet BCs to component fc1=%i Jgv=[%i,%i][%i,%i]\n",fc1,
//   		 Jg1.getBase(),Jg1.getBound(),Jg2.getBase(),Jg2.getBound());
//            printf("slipWall: Assign nemann BCs to component fc2=%i Igv=[%i,%i][%i,%i]\n",fc2,
//   		 Ig1.getBase(),Ig1.getBound(),Ig2.getBase(),Ig2.getBound());
        	  
	// 030819 f(Jg1,Jg2,Jg3,fc1)=e(mg,Jg1,Jg2,Jg3,fc1);  // normal component: dirichlet BC on the boundary
	// 030819 f(Ig1,Ig2,Ig3,fc1)=e(mg,Ig1,Ig2,Ig3,fc1);  // give a value on the ghost line too

      	fLocal(Ib1,Ib2,Ib3,fc1)=uLocal(Ib1,Ib2,Ib3,fc1); // normal component BC: give dirichlet BC on the boundary
      	fLocal(Ig1,Ig2,Ig3,fc1)=uLocal(Ig1,Ig2,Ig3,fc1); // fill in ghost value solution here 

      	realSerialArray uge(Ig1,Ig2,Ig3), upe(Ip1,Ip2,Ip3);
      	bool isRectangularForTZ=false;
      	e.gd( uge,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ig1,Ig2,Ig3,fc2,t);
      	e.gd( upe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ip1,Ip2,Ip3,fc2,t);

      	fLocal(Ig1,Ig2,Ig3,fc2)=uge-upe;  // tangential component, ghost value

      	if( numberOfDimensions==3 )
      	{
	  // f(Ig1,Ig2,Ig3,fc3)=e(mg,Ig1,Ig2,Ig3,fc3)-e(mg,Ip1,Ip2,Ip3,fc3);
        	  e.gd( uge,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ig1,Ig2,Ig3,fc3,t);
        	  e.gd( upe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ip1,Ip2,Ip3,fc3,t);
        	  fLocal(Ig1,Ig2,Ig3,fc3)=uge-upe;
      	}
        	  
      	if( useTurbulenceModel )
      	{
	  // f(Ig1,Ig2,Ig3,nc)=e(mg,Ig1,Ig2,Ig3,nc)-e(mg,Ip1,Ip2,Ip3,nc);
        	  e.gd( uge,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ig1,Ig2,Ig3,nc,t);
        	  e.gd( upe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ip1,Ip2,Ip3,nc,t);
        	  fLocal(Ig1,Ig2,Ig3,nc)=uge-upe;
      	}

      	if( computeTemperature )
      	{
        	  e.gd( uge,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ig1,Ig2,Ig3,tc,t);
        	  e.gd( upe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ip1,Ip2,Ip3,tc,t);
        	  fLocal(Ig1,Ig2,Ig3,tc)=uge-upe;
      	}
        	  
      	if( fourthOrder )
      	{
        	  Igv[direction] = side==0 ? Igv[direction].getBase()-1 : Igv[direction].getBound()+1;
        	  fLocal(Ig1,Ig2,Ig3,N)=0.;  // second ghost for tangential variables
      	}
            }
            else
            {
	// slip wall: 
      	fLocal(Ib1,Ib2,Ib3,fc1)=uLocal(Ib1,Ib2,Ib3,fc1); // normal compponent BC: give dirichlet BC on the boundary
      	fLocal(Ig1,Ig2,Ig3,fc1)=uLocal(Ig1,Ig2,Ig3,fc1); // fill in ghost value solution here (assigned below)

      	fLocal(Ig1,Ig2,Ig3,fc2)=0.;                 // tangential component: neumann BC is applied on the ghost line
      	if( numberOfDimensions==3 )
        	  fLocal(Ig1,Ig2,Ig3,fc3)=0.;

      	if( useTurbulenceModel )
        	  fLocal(Ig1,Ig2,Ig3,nc)=0.;

      	if( fourthOrder )
      	{
        	  Igv[direction] = side==0 ? Igv[direction].getBase()-1 : Igv[direction].getBound()+1;
        	  fLocal(Ig1,Ig2,Ig3,N)=0.;
      	}
            }

      // Assign the rhs for the Temperature
      // **** assignTemperatureRHS();

        }
    // *************************************************************************************************
        else if( bc0==InsParameters::outflow )
        {
      // normal component: use u.x+v.y=0
      // **** to fix: if there is an  adjacent slipWall --> use u.xx=0 for the line on the wall since
      //              we have already used div(u)=0 for the slipWall ghost point
            if( twilightZoneFlow )
            {

      	OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
      	const real cex1= parameters.dbase.get<int >("orderOfExtrapolationForOutflow")==2 ? -2. : -3.;
      	const real cex2= parameters.dbase.get<int >("orderOfExtrapolationForOutflow")==2 ?  1. :  3.;
        	  
      	const realArray & x= mg.center();
#ifdef USE_PPP
      	realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
#else
      	const realSerialArray & xLocal = x;
#endif

      	Range V(uc,uc+numberOfDimensions-1);
      	realSerialArray uge(Ig1,Ig2,Ig3,V), upe(Ip1,Ip2,Ip3,V), ube(Ib1,Ib2,Ib3,V);
      	bool isRectangularForTZ=false;
      	e.gd( uge,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ig1,Ig2,Ig3,V,t);
      	e.gd( upe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ip1,Ip2,Ip3,V,t);
      	e.gd( ube,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ib1,Ib2,Ib3,V,t);

      	bool useNew=true;
        	  
	// *********** fix this for 3rd-order extrapolation !!

      	if( direction==0 )
      	{ 
//    	    printf(" Assign outflow BCs to component uc=%i Igv=[%i,%i][%i,%i] Ipv=[%i,%i][%i,%i]\n",uc,
//    		 Ig1.getBase(),Ig1.getBound(),Ig2.getBase(),Ig2.getBound(),
//                   Ip1.getBase(),Ip1.getBound(),Ip2.getBase(),Ip2.getBound() );
        	  if( !useNew )
        	  {
          	    if( isRectangular )
            	      f(Ig1,Ig2,Ig3,uc)=e(mg,Ig1,Ig2,Ig3,uc)-e(mg,Ip1,Ip2,Ip3,uc);  // u.x+v.y=0
          	    else
            	      f(Ig1,Ig2,Ig3,uc)=e(mg,Ig1,Ig2,Ig3,uc)+cex1*e(mg,Ig1+is1,Ig2+is2,Ig3+is3,uc)+cex2*e(mg,Ip1,Ip2,Ip3,uc); 
            	      
          	    f(Ig1,Ig2,Ig3,vc)=e(mg,Ig1,Ig2,Ig3,vc)+cex1*e(mg,Ig1+is1,Ig2+is2,Ig3+is3,vc)+cex2*e(mg,Ip1,Ip2,Ip3,vc);
          	    if( numberOfDimensions==3 )
            	      f(Ig1,Ig2,Ig3,wc)=e(mg,Ig1,Ig2,Ig3,wc)+cex1*e(mg,Ig1+is1,Ig2+is2,Ig3+is3,wc)+cex2*e(mg,Ip1,Ip2,Ip3,wc);
        	  }
        	  else
        	  {
          	    if( isRectangular )
          	    {
            	      fLocal(Ig1,Ig2,Ig3,uc)=uge(Ig1,Ig2,Ig3,uc)-upe(Ip1,Ip2,Ip3,uc);
          	    }
          	    else
          	    {
            	      fLocal(Ig1,Ig2,Ig3,uc)=uge(Ig1,Ig2,Ig3,uc)+cex1*ube(Ib1,Ib2,Ib3,uc)+cex2*upe(Ip1,Ip2,Ip3,uc); 
          	    }
          	    
          	    fLocal(Ig1,Ig2,Ig3,vc)=uge(Ig1,Ig2,Ig3,vc)+cex1*ube(Ib1,Ib2,Ib3,vc)+cex2*upe(Ip1,Ip2,Ip3,vc);
          	    if( numberOfDimensions==3 )
            	      fLocal(Ig1,Ig2,Ig3,wc)=uge(Ig1,Ig2,Ig3,wc)+cex1*ube(Ib1,Ib2,Ib3,wc)+cex2*upe(Ip1,Ip2,Ip3,wc);
        	  }
          	    
      	}
      	else if( direction==1 )
      	{
        	  if( !useNew )
        	  {
          	    if( isRectangular )
            	      f(Ig1,Ig2,Ig3,vc)=e(mg,Ig1,Ig2,Ig3,vc)-e(mg,Ip1,Ip2,Ip3,vc);  // u.x+v.y=0
          	    else
            	      f(Ig1,Ig2,Ig3,vc)=e(mg,Ig1,Ig2,Ig3,vc)+cex1*e(mg,Ig1+is1,Ig2+is2,Ig3+is3,vc)+cex2*e(mg,Ip1,Ip2,Ip3,vc);
	    // f(Ig1,Ig2,Ig3,uc)=e(mg,Ig1,Ig2,Ig3,uc)-e(mg,Ip1,Ip2,Ip3,uc);
	    // f(Ig1,Ig2,Ig3,uc)=0.;// this is assigned already
          	    f(Ig1,Ig2,Ig3,uc)=e(mg,Ig1,Ig2,Ig3,uc)+cex1*e(mg,Ig1+is1,Ig2+is2,Ig3+is3,uc)+cex2*e(mg,Ip1,Ip2,Ip3,uc);
          	    if( numberOfDimensions==3 )
            	      f(Ig1,Ig2,Ig3,wc)=e(mg,Ig1,Ig2,Ig3,wc)+cex1*e(mg,Ig1+is1,Ig2+is2,Ig3+is3,wc)+cex2*e(mg,Ip1,Ip2,Ip3,wc);
        	  }
        	  else
        	  {
          	    if( isRectangular )
            	      fLocal(Ig1,Ig2,Ig3,vc)=uge(Ig1,Ig2,Ig3,vc)-upe(Ip1,Ip2,Ip3,vc);  // u.x+v.y=0
          	    else
            	      fLocal(Ig1,Ig2,Ig3,vc)=uge(Ig1,Ig2,Ig3,vc)+cex1*ube(Ib1,Ib2,Ib3,vc)+cex2*upe(Ip1,Ip2,Ip3,vc);
          	    fLocal(Ig1,Ig2,Ig3,uc)=uge(Ig1,Ig2,Ig3,uc)+cex1*ube(Ib1,Ib2,Ib3,uc)+cex2*upe(Ip1,Ip2,Ip3,uc);
          	    if( numberOfDimensions==3 )
            	      fLocal(Ig1,Ig2,Ig3,wc)=uge(Ig1,Ig2,Ig3,wc)+cex1*ube(Ib1,Ib2,Ib3,wc)+cex2*upe(Ip1,Ip2,Ip3,wc);
        	  }
          	    
      	}
      	else 
      	{
	  // if( isRectangular )
	  //   f(Ig1,Ig2,Ig3,wc)=e(mg,Ig1,Ig2,Ig3,wc)-e(mg,Ip1,Ip2,Ip3,wc);
	  // else
	  //   f(Ig1,Ig2,Ig3,wc)=e(mg,Ig1,Ig2,Ig3,wc)+cex1*e(mg,Ib1,Ib2,Ib3,wc)+cex2*e(mg,Ip1,Ip2,Ip3,wc);
	  // f(Ig1,Ig2,Ig3,uc)=e(mg,Ig1,Ig2,Ig3,uc)+cex1*e(mg,Ib1,Ib2,Ib3,uc)+cex2*e(mg,Ip1,Ip2,Ip3,uc);
	  // f(Ig1,Ig2,Ig3,vc)=e(mg,Ig1,Ig2,Ig3,vc)+cex1*e(mg,Ib1,Ib2,Ib3,vc)+cex2*e(mg,Ip1,Ip2,Ip3,vc);

        	  if( isRectangular )
          	    fLocal(Ig1,Ig2,Ig3,wc)=uge(Ig1,Ig2,Ig3,wc)-upe(Ip1,Ip2,Ip3,wc);
        	  else
          	    fLocal(Ig1,Ig2,Ig3,wc)=uge(Ig1,Ig2,Ig3,wc)+cex1*ube(Ib1,Ib2,Ib3,wc)+cex2*upe(Ip1,Ip2,Ip3,wc);
        	  fLocal(Ig1,Ig2,Ig3,uc)=uge(Ig1,Ig2,Ig3,uc)+cex1*ube(Ib1,Ib2,Ib3,uc)+cex2*upe(Ip1,Ip2,Ip3,uc);
        	  fLocal(Ig1,Ig2,Ig3,vc)=uge(Ig1,Ig2,Ig3,vc)+cex1*ube(Ib1,Ib2,Ib3,vc)+cex2*upe(Ip1,Ip2,Ip3,vc);
      	}

      	if( turbulenceModel==Parameters::SpalartAllmaras )
      	{
	  // if( parameters.dbase.get<int >("orderOfExtrapolationForOutflow")==2 )
	  //    f(Ig1,Ig2,Ig3,nc)=e(mg,Ig1,Ig2,Ig3,nc)-2.*e(mg,Ib1,Ib2,Ib3,nc)+e(mg,Ip1,Ip2,Ip3,nc);
	  // else
	  //    f(Ig1,Ig2,Ig3,nc)=e(mg,Ig1,Ig2,Ig3,nc)-3.*e(mg,Ib1,Ib2,Ib3,nc)+3.*e(mg,Ip1,Ip2,Ip3,nc);

        	  realSerialArray nge(Ig1,Ig2,Ig3), npe(Ip1,Ip2,Ip3), nbe(Ib1,Ib2,Ib3);
        	  bool isRectangularForTZ=false;
        	  e.gd( nge,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ig1,Ig2,Ig3,nc,t);
        	  e.gd( npe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ip1,Ip2,Ip3,nc,t);
        	  e.gd( nbe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ib1,Ib2,Ib3,nc,t);

        	  if( parameters.dbase.get<int >("orderOfExtrapolationForOutflow")==2 )
          	    fLocal(Ig1,Ig2,Ig3,nc)=nge-2.*nbe+npe;
        	  else
          	    fLocal(Ig1,Ig2,Ig3,nc)=nge-3.*nbe+3.*npe;
      	}
      	else if( turbulenceModel!=Parameters::noTurbulenceModel )
      	{
        	  Overture::abort();
      	}
      	if( computeTemperature )
      	{
          // Assign the rhs for the Temperature
                      if( computeTemperature )
                      { // Look for mixed BC for T
                          int axis=direction;
                          real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid), rhs=mixedRHS(tc,side,axis,grid);
                          assert( a0!=0. || a1!=0. );
                          if( a1==0. ) // coeff of T.n is zero -> Dirichlet BC
                          {
                              fLocal(Ib1,Ib2,Ib3,tc)=uLocal(Ib1,Ib2,Ib3,tc); 
                              fLocal(Ig1,Ig2,Ig3,tc)=uLocal(Ig1,Ig2,Ig3,tc); 
                          }
                          else  // coeff of T.n is non-zero
                          {
                              if( debug() & 4 )
                          	printF("++++assign line solver mixed BC for T: (grid,side,axis)=(%i,%i,%i), %3.2f*T+%3.2f*T.n=%3.2f,  \n",
                                 	       grid,side,axis, a0, a1, rhs );
               // The line solver matrix holds the coefficients of the mixed-BC along a line:
               // a0*u + a1*u.n = 
               // a0*u + a1*( n1*( rx*ur + sx*us ) + n2*( ry*ur + sy*us ) )
               //  n1 = rsxy(dir,0), n2=rsxy(dir,1)
                              if( twilightZoneFlow )
                                  rhs=0.;  // turn off user defined RHS for TZ 
                              if( isRectangular )
                              {
                          	fLocal(Ig1,Ig2,Ig3,tc)=rhs;
                              }
                              else
                              {
                 // --- we should optimize this ---
                                  #ifdef USE_PPP
                                    const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
                           	 realSerialArray rsxy; getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rsxy);
                                  #else
                           	 const realSerialArray & rsxy = mg.inverseVertexDerivative();
                                    const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
                                  #endif
          	 // RSXY: index the rsxy array as a 5D array: 
                                  #define RSXY(I1,I2,I3,m1,m2) rsxy(I1,I2,I3,(m1)+numberOfDimensions*(m2))
                                  MappedGridOperators & op = *u.getOperators();
                 // a1*( an1*( rsxy(dir,0)*u_d - ux2()) + an2*( rsxy(dir,1)*u_d - uy2() ) + ...
                                  RealArray ur(Ib1,Ib2,Ib3);
                                  MappedGridOperators::derivativeTypes rDeriv = MappedGridOperators::derivativeTypes(MappedGridOperators::r1Derivative+direction); // "normal" r-derivative
                                  if( false )
                          	{
                   // ** fix this ** the parameter derivatives are NOT defined!
                            	  op.derivative(rDeriv,uLocal,ur ,Ib1,Ib2,Ib3,tc); // ur <- T.r1 or T.r2 or T.r3 
                          	}
                          	else
                          	{
                            	  getGhostIndex(mg.gridIndexRange(),side,direction,Ip1,Ip2,Ip3,-1); // first line inside
                            	  ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ip1,Ip2,Ip3,1);
                            	  ur(Ib1,Ib2,Ib3)=(uLocal(Ip1,Ip2,Ip3,tc)-uLocal(Ig1,Ig2,Ig3,tc))/(is*2.*mg.gridSpacing(direction));
                          	}
          	// ::display(ur," mixed BC: ur");
                                  RealArray ux(Ib1,Ib2,Ib3),uy(Ib1,Ib2,Ib3);
                                  op.derivative(MappedGridOperators::xDerivative,uLocal,ux ,Ib1,Ib2,Ib3,tc);
                                  op.derivative(MappedGridOperators::yDerivative,uLocal,uy ,Ib1,Ib2,Ib3,tc);
                 // RHS = LHS*u - [ a0*u + a1*T.n ]
                 // LHS*u = a0*u + a1*( an1*( rsxy(dir,0)*ur ) + an2*( rsxy(dir,1)*ur ) ) 
                          	fLocal(Ig1,Ig2,Ig3,tc) = rhs + a1*( 
                                            normal(Ib1,Ib2,Ib3,0)*( RSXY(Ib1,Ib2,Ib3,direction,0)*ur-ux ) + 
                                            normal(Ib1,Ib2,Ib3,1)*( RSXY(Ib1,Ib2,Ib3,direction,1)*ur-uy ) );
                                  if( numberOfDimensions==3 )
                          	{
                                      op.derivative(MappedGridOperators::zDerivative,uLocal,ux ,Ib1,Ib2,Ib3,tc); // ux <- T.z 
                            	  fLocal(Ig1,Ig2,Ig3,tc) += a1*( normal(Ib1,Ib2,Ib3,2)*( RSXY(Ib1,Ib2,Ib3,direction,2)*ur-ux ) );
                          	}
          	// ::display(ux," mixed BC: ux");
          	// ::display(uy," mixed BC: uy");
          	// ::display(fLocal(Ig1,Ig2,Ig3,tc)," fLocal(Ig1,Ig2,Ig3,tc)");
                                  #undef RSXY
                              }
                              if( twilightZoneFlow )
                              {
                                  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                                  #ifdef USE_PPP
                                    const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
                           	 realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
                                  #else
                           	 const realSerialArray & xLocal = mg.center();
                                    const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
                                  #endif
                                  realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3);
                                  bool isRectangularForTZ=false;
                                  e.gd( ue ,xLocal,numberOfDimensions,isRectangularForTZ,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                                  e.gd( uex,xLocal,numberOfDimensions,isRectangularForTZ,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                                  e.gd( uey,xLocal,numberOfDimensions,isRectangularForTZ,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                          	fLocal(Ig1,Ig2,Ig3,tc) += a0*ue + normal(Ib1,Ib2,Ib3,0)*uex + normal(Ib1,Ib2,Ib3,1)*uey;
                          	if( numberOfDimensions==3 )
                          	{
                            	  e.gd( uex,xLocal,numberOfDimensions,isRectangularForTZ,0,0,0,1,Ib1,Ib2,Ib3,tc,t);  // uex = T.z
                                      fLocal(Ig1,Ig2,Ig3,tc) +=normal(Ib1,Ib2,Ib3,2)*uex;
                          	}
                              }
                          }
                      } // end if computeTemperature

// 	  realSerialArray nge(Ig1,Ig2,Ig3), npe(Ip1,Ip2,Ip3), nbe(Ib1,Ib2,Ib3);
// 	  bool isRectangularForTZ=false;
// 	  e.gd( nge,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ig1,Ig2,Ig3,tc,t);
// 	  e.gd( npe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ip1,Ip2,Ip3,tc,t);
// 	  e.gd( nbe,xLocal,mg.numberOfDimensions(),isRectangularForTZ,0,0,0,0,Ib1,Ib2,Ib3,tc,t);

// 	  if( parameters.dbase.get<int >("orderOfExtrapolationForOutflow")==2 )
// 	    fLocal(Ig1,Ig2,Ig3,tc)=nge-2.*nbe+npe;
// 	  else
// 	    fLocal(Ig1,Ig2,Ig3,tc)=nge-3.*nbe+3.*npe;

      	}
      	if( fourthOrder )
      	{
        	  Igv[direction] = side==0 ? Igv[direction].getBase()-1 : Igv[direction].getBound()+1;
        	  f(Ig1,Ig2,Ig3,N)=0.;
      	}
        	  
            }
            else // not twilightzone
            {
	// outflow 

	// ********************************************** Just give u.xx=0 too for non-rectangular
      	if( isRectangular )
      	{
        	  fLocal(Ig1,Ig2,Ig3,N)=0.;
          	    
        	  if( numberOfDimensions==2 )
        	  {
	    // u.x+v.y=0
          	    if( direction==0 )
            	      fLocal(Ig1,Ig2,Ig3,uc)= (is*dx[0]/dx[1])*(uLocal(Ib1,Ib2+1,Ib3,vc)-uLocal(Ib1,Ib2-1,Ib3,vc));
          	    else if( direction==1 )
            	      fLocal(Ig1,Ig2,Ig3,vc)= (is*dx[1]/dx[0])*(uLocal(Ib1+1,Ib2,Ib3,uc)-uLocal(Ib1-1,Ib2,Ib3,uc));
        	  }
        	  else
        	  {
	    // u.x+v.y+w.z=0
          	    if( direction==0 )
            	      fLocal(Ig1,Ig2,Ig3,uc)=((is*dx[0]/dx[1])*(uLocal(Ib1,Ib2+1,Ib3,vc)-uLocal(Ib1,Ib2-1,Ib3,vc))+
                              				      (is*dx[0]/dx[2])*(uLocal(Ib1,Ib2,Ib3+1,wc)-uLocal(Ib1,Ib2,Ib3-1,wc)));
          	    else if( direction==1 )
            	      fLocal(Ig1,Ig2,Ig3,vc)=((is*dx[1]/dx[0])*(uLocal(Ib1+1,Ib2,Ib3,uc)-uLocal(Ib1-1,Ib2,Ib3,uc))+
                              				      (is*dx[1]/dx[2])*(uLocal(Ib1,Ib2,Ib3+1,wc)-uLocal(Ib1,Ib2,Ib3-1,wc)));
          	    else
          	    {
            	      fLocal(Ig1,Ig2,Ig3,wc)=((is*dx[2]/dx[0])*(uLocal(Ib1+1,Ib2,Ib3,uc)-uLocal(Ib1-1,Ib2,Ib3,uc))+
                              				      (is*dx[2]/dx[1])*(uLocal(Ib1,Ib2+1,Ib3,vc)-uLocal(Ib1,Ib2-1,Ib3,vc)));
          	    }
          	    
        	  }
        	  
	  // **** corners ********
        	  for( int dir=0; dir<numberOfDimensions-1; dir++ )
          	    for( int sideAdjacent=0; sideAdjacent<=1; sideAdjacent++ )
          	    {
            	      int axis= (direction+dir+1) % numberOfDimensions; // tangential direction
            	      int bcAdjacent = mg.boundaryCondition(sideAdjacent,axis);
            	      if( bcAdjacent==Parameters::slipWall )
            	      {
		// outflow is next to a slipWall -- replace div(u)=0 BC with u.x=0 (for now, uxx=0 better)...
            		Ip1=Ig1, Ip2=Ig2, Ip3=Ig3;

            		Ipv[direction]=side==0 ? Igv[direction].getBase() : Igv[direction].getBound();
            		Ipv[axis]=sideAdjacent==0 ? Igv[axis].getBase() : Igv[axis].getBound();

            		fLocal(Ip1,Ip2,Ip3,uc+direction)=0.;
            		

            	      }
          	    }
      	}
      	else // not rectangular
      	{
        	  fLocal(Ig1,Ig2,Ig3,N)=0.;
        	  if( fourthOrder )
        	  {
          	    Igv[direction] = side==0 ? Igv[direction].getBase()-1 : Igv[direction].getBound()+1;
          	    fLocal(Ig1,Ig2,Ig3,N)=0.;
        	  }
      	}
        	  
        	  
            }
        }
        else if( bc0>0 )
        {
            printf("advanceLineSolver: Unknown boundary condition: bc0=%i\n",bc0);
            Overture::abort();
      	
        }
        
    } // end for side

    
    if( debug() & 64 )
    { 
        aString buff;
        Index I1,I2,I3;
        getIndex(mg.gridIndexRange(),I1,I2,I3);
        I1=Range(-1,1);
        display(f(I1,I2,I3,Range(uc,vc)),"\n RHS after fill BC's",debugFile,"%7.4f ");
    }

    return 0;
}


