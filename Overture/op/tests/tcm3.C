//===============================================================================
//  Coefficient Matrix Example
//    Using Oges to solve Poisson's equation on a CompositeGrid
//
// Usage: `tcm3 [<gridName>] [-solver=[yale][harwell][slap][petsc][mg]] [-debug=<value>] [-outputMatrix] ...
//              [-noTiming] [-check] [-plot] [-trig] [-tol=<value>] [-freq=<value>] [-dirichlet] [-neumann]' 
//
//   The -check option is used for regression testing -- it will test various solvers on a few grids
//
// NOTE:
// To get PETSc log info, compile PETScEquationSolver with the destructor calling PetscFinalize()
//   and use the command line arg -log_summary
//   memory usage: -trmalloc_log 
//
// Parallel examples:
//   mpirun -np 2 tcm3 square20.hdf -solver=petsc
//   mpirun -np 2 tcm3 cic.hdf -solver=petsc 
//   srun -N1 -n1 -ppdebug tcm3 square20.hdf -solver=petsc 
//   srun -N1 -n2 -ppdebug tcm3 sibe2.order2.hdf -solver=petsc 
//==============================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "SparseRep.h"
#include "display.h"
#include "Ogmg.h"
#include "Checker.h"
#include "PlotStuff.h"
#include "ParallelUtility.h"
#include "LoadBalancer.h"

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )

bool measureCPU=TRUE;
real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU();
  else
    return 0;
}

void
plotResults( PlotStuff & ps, Oges & solver, realCompositeGridFunction & u, realCompositeGridFunction & err )
// ==============================================================================================
// Plot results from Oges
// ==============================================================================================
{
      
  GraphicsParameters psp;

  aString answer;
  aString menu[]=
  {
    "solution",
    "error",
    "grid",
    "erase",
    "exit",
    ""
  };
    
  for( ;; )
  {
    ps.getMenuItem(menu,answer,"choose an option");
    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="solution" )
    {
      psp.set(GI_TOP_LABEL,"Solution u"); 
      PlotIt::contour(ps,u,psp);
    }
    else if( answer=="error" )
    {
      psp.set(GI_TOP_LABEL,"error"); 
      PlotIt::contour(ps,err,psp);
    }
    else if( answer=="grid" )
    {
      psp.set(GI_TOP_LABEL,"grid"); 
      PlotIt::plot(ps,*u.getCompositeGrid(),psp);
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
      
  }

}


int 
assignForcing(int option, CompositeGrid & cg, realCompositeGridFunction & f, OGFunction & exact,
              RealArray *varCoeff=NULL )
// ================================================================================================
//
/// \brief  Assign the right-hand-side. 
///
/// \param option (input) : 0 = dirichlet, 1=neumann, 2=mixed (variable coefficients)
// 
// ================================================================================================
{
  const int numberOfDimensions = cg.numberOfDimensions();
  
  Index I1,I2,I3;
  Index Ib1,Ib2,Ib3;
  Index Ig1,Ig2,Ig3;


  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    // mg.mapping().getMapping().getGrid();
    // printF(" signForJacobian=%e\n",mg.mapping().getMapping().getSignForJacobian());
#ifdef USE_PPP
    realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
#else
    realSerialArray & fLocal = f[grid]; 
#endif

    getIndex(mg.indexRange(),I1,I2,I3);  
    int includeGhost=1; // include parallel ghost pts in fLocal:
    bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1,I2,I3,includeGhost);
    if( !ok ) continue; // there are no points on this processor.

    realArray & x= mg.center();
#ifdef USE_PPP
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
#else
    const realSerialArray & xLocal = x;
#endif

    // Assign the forcing : f = e.xx + e.yy + e.zz (e=exact solution)
    RealArray ed(I1,I2,I3);
    const int rectangularForTZ=0;
    fLocal=0.;
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      int ntd=0, nxd[3]={0,0,0}; //
      nxd[axis]=2;  // compute e.xx (axis=0), e.yy (axis=1), ...
      exact.gd( ed,xLocal,mg.numberOfDimensions(),rectangularForTZ,ntd,nxd[0],nxd[1],nxd[2],I1,I2,I3,0,0.);
      fLocal(I1,I2,I3)+=ed(I1,I2,I3);
    }
	

    ForBoundary(side,axis)
    {
      if( mg.boundaryCondition(side,axis) > 0 )
      {
        #ifdef USE_PPP
          const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
        #else
          const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
        #endif

	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue; // there are no points on this processor.
	if( option==0 )
	{
	  // Dirichlet BC's : assign the value of f on the boundary: 
	  RealArray ue(Ib1,Ib2,Ib3);
	  exact.gd( ue,xLocal,mg.numberOfDimensions(),rectangularForTZ,0,0,0,0,Ib1,Ib2,Ib3,0,0.);
	  fLocal(Ib1,Ib2,Ib3)=ue;
	}
	else
	{
          // Neumann or mixed BC's : assign the value of f on the ghost line: 

	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	  bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ig1,Ig2,Ig3,includeGhost);
	  if( !ok ) continue; // there are no points on this processor.

	  realSerialArray uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3);
	  exact.gd( uex,xLocal,numberOfDimensions,rectangularForTZ,0,1,0,0,Ib1,Ib2,Ib3,0,0.);
	  exact.gd( uey,xLocal,numberOfDimensions,rectangularForTZ,0,0,1,0,Ib1,Ib2,Ib3,0,0.);

	  fLocal(Ig1,Ig2,Ig3) = normal(Ib1,Ib2,Ib3,0)*uex + normal(Ib1,Ib2,Ib3,1)*uey;
	  if( numberOfDimensions==3 )
	  {
	    exact.gd( uex,xLocal,numberOfDimensions,rectangularForTZ,0,0,0,1,Ib1,Ib2,Ib3,0,0.);  // uex = T.z
	    fLocal(Ig1,Ig2,Ig3) +=normal(Ib1,Ib2,Ib3,2)*uex;
	  }
	  if( option==2 )
	  { // -- Mixed BC's ---
	    RealArray ue(Ib1,Ib2,Ib3);
	    exact.gd( ue,xLocal,mg.numberOfDimensions(),rectangularForTZ,0,0,0,0,Ib1,Ib2,Ib3,0,0.);
	    fLocal(Ig1,Ig2,Ig3) = (  varCoeff[grid](Ib1,Ib2,Ib3,0)*ue(Ib1,Ib2,Ib3)
				    +varCoeff[grid](Ib1,Ib2,Ib3,1)*fLocal(Ig1,Ig2,Ig3) );
	  }
	  
	}
	
      }
    }
  }
  // f.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);   
  // f.display("Here is f");

  return 0;
}

int 
computeTheError( int option, CompositeGrid & cg, realCompositeGridFunction & u,
		 realCompositeGridFunction & err, OGFunction & exact, real & error ) 
// ================================================================================================
//
//  Compute the error in the solution.
//
// /option (input) : 0 = dirichlet, 1=neumann, 2=mixed
// 
// ================================================================================================
{

  err=0.;
  error=0.;
  real errorWithGhostPoints=0;
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    realArray & x= mg.center();
#ifdef USE_PPP
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
    realSerialArray errLocal; getLocalArrayWithGhostBoundaries(err[grid],errLocal);
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);
#else
    const realSerialArray & xLocal = x;
    realSerialArray & uLocal = u[grid]; 
    realSerialArray & errLocal = err[grid]; 
    const intSerialArray & maskLocal = cg[grid].mask();
#endif

    getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
    int includeGhost=1; // include parallel ghost pts in uLocal
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);

    real ueMax=0.; // holds the max value of the exact soln on this grid
    RealArray ue;
    if( ok )
    { // evaluate the exact solution
      ue.redim(I1,I2,I3);
      const int rectangularForTZ=0;
      exact.gd( ue,xLocal,mg.numberOfDimensions(),rectangularForTZ,0,0,0,0,I1,I2,I3,0,0.);
      ueMax=max(fabs(ue));
    }
    ueMax=ParallelUtility::getMaxValue(ueMax); // max value over all procs

    real gridErrWithGhost=0., gridErr=0.;
    if( ok )
    {
      where( maskLocal(I1,I2,I3)!=0 )
	errLocal(I1,I2,I3)=abs(uLocal(I1,I2,I3)-ue);

      gridErrWithGhost=max(errLocal(I1,I2,I3))/ueMax;

      getIndex(cg[grid].indexRange(),I1,I2,I3);  
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);
      if( !ok ) continue; // there are no points on this processor.

      where( maskLocal(I1,I2,I3)!=0 )
	errLocal(I1,I2,I3)=abs(uLocal(I1,I2,I3)-ue(I1,I2,I3));

      gridErr=max(errLocal(I1,I2,I3))/ueMax;
    }
    gridErr         =ParallelUtility::getMaxValue(gridErr); // max value over all procs
    gridErrWithGhost=ParallelUtility::getMaxValue(gridErrWithGhost); // max value over all procs

    error=max(error, gridErr );
    errorWithGhostPoints=max(errorWithGhostPoints, gridErrWithGhost);

    printF(" grid=%i (%s) max. rel. err=%e (%e with ghost)\n",grid,(const char*)cg[grid].getName(),
	   gridErr,gridErrWithGhost);

    if( Oges::debug & 8 )
    {
      display(u[grid],"solution u");
      display(err[grid],"abs(error on indexRange +1)");
      // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
    }
  }
  if( option==0 )
    printF("Maximum relative error with dirichlet bc's= %e (%e with ghost)\n",error,errorWithGhostPoints);  
  else if(option==1 )
    printF("Maximum relative error with neumann bc's= %e\n",error);  
  else if(option==1 )
    printF("Maximum relative error with mixed bc's= %e\n",error);  

  return 0;
}


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

   // This macro will initialize the PETSc solver if OVERTURE_USE_PETSC is defined.
  INIT_PETSC_SOLVER();

  printF("Usage: tcm3 [<gridName>] [-solver=[yale][harwell][slap][petsc][mg]] [-debug=<value>][-outputMatrix]\n" 
                     "[-noTiming] [-check] [-trig] [-tol=<value>] [-order=<value>] [-plot] [-ilu=] [-gmres] \n"
                     "[-freq=<value>] [-dirichlet] [-neumann] [-mixed] [-testCommunicator] [-hypre] \n");

  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
  // here are upper bounds on the errors we expect for each grid. This seems the only reliable
  // way to compare results from different machines, especially for iterative solvers.
  const real errorBound[maxNumberOfGridsToTest][2][2]=
    { 5.e-8,4.e-8,    5.e-7,9.e-7,  // square, dirichlet/neuman(DP) dir/neu(SP)
      7.e-4,2.e-3,    7.e-4,2.e-3, // cic
      6.e-3,7.e-3,    6.e-3,7.e-3  // sib
    };
  const int precision = REAL_EPSILON==DBL_EPSILON ? 0 : 1;
  int twilightZoneOption=0;
  
  int solverType=OgesParameters::yale; 
  aString solverName="yale";
  aString iterativeSolverType="bi-cg";
  bool check=false;
  real tol=1.e-8;
  int orderOfAccuracy=2;
  int plot=0;
  int iluLevels=-1; // -1 : use default
  int problemsToSolve=1+2;  // solve dirichlet=1 and neumann=2
  bool outputMatrix=false;
  bool testCommunicator=false;  // set to true to test PETSc when using only a subset of the processors.
  
  real fx=2., fy=2., fz=2.; // frequencies for trig TZ
  
  int len=0;
  if( argc >= 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
	measureCPU=FALSE;
      else if( (len=arg.matches("-debug=")) )
      {
	sScanF(arg(len,arg.length()-1),"%i",&Oges::debug);
	printF("Setting Oges::debug=%i\n",Oges::debug);
      }
      else if( (len=arg.matches("-tol=")) )
      {
	sScanF(arg(len,arg.length()-1),"%e",&tol);
	printF("Setting tol=%e\n",tol);
      }
      else if( (len=arg.matches("-freq=")) )
      {
	sScanF(arg(len,arg.length()-1),"%e",&fx);
	fy=fx; fz=fx;
	printF("Setting fx=fy=fz=%e\n",fx);
      }
      else if( (len=arg.matches("-ilu=")) )
      {
	sScanF(arg(len,arg.length()-1),"%i",&iluLevels);
	printF("Setting ilu levels =%i\n",iluLevels);
      }
      else if( (len=arg.matches("-gmres")) )
      {
	iterativeSolverType="gmres";
      }
      else if( (len=arg.matches("-hypre")) )
      {
	iterativeSolverType="hypre";
      }
      else if( (len=arg.matches("-testCommunicator")) )
      {
	testCommunicator=true;
	printF("Test the parallel PETSc solver using a subset of the processors\n");
      }
      else if( (len=arg.matches("-outputMatrix")) )
      {
	outputMatrix=true;
      }
      else if( (len=arg.matches("-dirichlet")) )
      {
	problemsToSolve=1; // just solve dirichlet problem
      }
      else if( (len=arg.matches("-neumann")) )
      {
	problemsToSolve=2; // just solve neumann problem
      }
      else if( (len=arg.matches("-mixed")) )
      {
	problemsToSolve=4; // just solve with mixed BC's (variable coeff)
      }
      else if( (len=arg.matches("-order=")) )
      {
	sScanF(arg(len,arg.length()-1),"%i",&orderOfAccuracy);
	if( orderOfAccuracy!=2 && orderOfAccuracy!=4 )
	{
	  printF("ERROR: orderOfAccuracy should be 2 or 4!\n");
	  Overture::abort();
	}
	printF("Setting orderOfAccuracy=%i\n",orderOfAccuracy);
      }
      else if( arg(0,7)=="-solver=" )
      {
	solverName=arg(8,arg.length()-1);
	if( solverName=="yale" )
	  solverType=OgesParameters::yale;
	else if( solverName=="harwell" )
	  solverType=OgesParameters::harwell;
	else if( solverName=="petsc" || solverName=="PETSc" )
#ifdef USE_PPP 
	{
	  solverType=OgesParameters::PETScNew;
	  // printF("tcm3: Setting solverType=PETScNew = %i\n",(int)solverType);
	}
#else
	solverType=OgesParameters::PETSc;
#endif
	else if( solverName=="slap" || solverName=="SLAP" )
	  solverType=OgesParameters::SLAP;
	else if( solverName=="mg" || solverName=="multigrid" )
	  solverType=OgesParameters::multigrid;
	else
	{
	  printF("Unknown solver=%s \n",(const char*)solverName);
	  throw "error";
	}
	
	printF("Setting solverType=%i\n",solverType);
      }
      else if( arg=="-plot" )
      {
	plot=true;
      }
      else if( arg=="-check" )
      {
	check=true;
      }
      else if( arg=="-trig" )
      {
	twilightZoneOption=1;
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[1];
      }
    }
  }

  if( Oges::debug > 3 )
    SparseRepForMGF::debug=3;  

  aString checkFileName;
  if( REAL_EPSILON == DBL_EPSILON )
    checkFileName="tcm3.dp.check.new";  // double precision
  else  
    checkFileName="tcm3.sp.check.new";
  Checker checker(checkFileName);  // for saving a check file.

  printF("=================================================================================\n"
         " --- tcm3 --- test coefficient matrices: scalar problem on an overlapping grid   \n"
         " \n"
         "  Equation: Poisson.\n");
  if( twilightZoneOption==0 )
    printF(" TwilightZone: polynomial, degree=%i.\n",orderOfAccuracy);
  else
    printF(" TwilightZone: trigonometric, fx=fy=fz=%e.\n",fx);
    
  printF("=================================================================================\n");

  PlotStuff ps(false,"tcm3");

  // make some shorter names for readability
  BCTypes::BCNames
    dirichlet           = BCTypes::dirichlet,
    neumann             = BCTypes::neumann,
    mixed               = BCTypes::mixed,
    extrapolate         = BCTypes::extrapolate,
    allBoundaries       = BCTypes::allBoundaries; 

  bool communicatorWasNewed=false;
  int pStart=-1, pEnd=-1;  // Use to distribute the grids in the CompositeGrid when we define a communicator
#ifdef USE_PPP
  MPI_Comm myComm=MPI_COMM_WORLD, myCommWorld=MPI_COMM_WORLD;  // Have PETSc use these communicators
  if( testCommunicator && solverType==OgesParameters::PETScNew )
  {
    // Here we test the PETSc solver when only some processors are involved in the parallel solve.
    // We build an MPI communicator that only includes some processors.
    communicatorWasNewed=true;

    const int np=max(1,Communication_Manager::Number_Of_Processors);
    const int myid=max(0,Communication_Manager::My_Process_Number);

    MPI_Group worldGroup, myGroup;
    MPI_Comm_group( MPI_COMM_WORLD,&worldGroup ); //get world group

    const int numRanks=min(2,np);
    int ranks[4]={1,2,3,4};  // include these ranks in myGroup
    printF("--TCM3-- Active processors = [%i",ranks[0]);
    for( int r=1; r<numRanks; r++ ){ printF(",%i",ranks[r]); }  // 
    printF("]\n");

    MPI_Group_incl(worldGroup, numRanks, ranks, &myGroup );   // myGgroup includes some ranks 
    MPI_Comm_create( MPI_COMM_WORLD, myGroup, &myComm ); // construct myComm
    MPI_Comm_create( MPI_COMM_WORLD, myGroup, &myCommWorld ); // construct myCommWorld
    MPI_Group_free(&worldGroup);
    MPI_Group_free(&myGroup);

    pStart=ranks[0]; pEnd=ranks[numRanks-1];

    // int colour = myid==(np-1);  // communicator includes this processor
    // // int colour = myid==(np-1);  // communicator includes this processor
    // // int colour = 1; 
    // int key=0;
    // MPI_Comm_split(MPI_COMM_WORLD,colour,key,&myComm );

    // if( myComm!=MPI_COMM_NULL )
    // {
    //   int size=-1; 
    //   MPI_Comm_size(myComm,&size);
    //   printf(" --TCM3-- myComm: myid=%i size=%i.\n",myid,size);
    // }
    // else
    // {
    //   printf(" --TCM3-- myComm: myid=%i myComm==NULL.\n",myid);
    // }
  }
  else
  {

  }
#endif

  int numberOfSolvers = check ? 2 : 1;
  real worstError=0.;
  for( int sparseSolver=0; sparseSolver<numberOfSolvers; sparseSolver++ )
  {
    if( check )
    {
      if( sparseSolver==0 )
      {
	solverName="yale";
	solverType=OgesParameters::yale;
      }
      else
      {
	solverName="slap";
	solverType=OgesParameters::SLAP;
      }
    }

    checker.setLabel(solverName,0);


    for( int it=0; it<numberOfGridsToTest; it++ )
    {
      aString nameOfOGFile=gridName[it];
      checker.setLabel(nameOfOGFile,1);

      printF("\n *****************************************************************\n"
              " ******** Checking grid: %s                 ************ \n"
              " *****************************************************************\n\n",(const char*)nameOfOGFile);

      CompositeGrid cg;
      if( pStart>=0  )
      {
	LoadBalancer loadBalancer;
        loadBalancer.setLoadBalancer(LoadBalancer::allToAll);
	printF("loadBalancer: pStart=%i, pEnd=%i\n",pStart,pEnd);
	
        loadBalancer.setProcessors(pStart, pEnd);

	// loadBalancer.setLoadBalancer(LoadBalancer::allToAll);
	getFromADataBase(cg,nameOfOGFile,loadBalancer);
      }
      else
      {
	getFromADataBase(cg,nameOfOGFile);
      }
      cg.displayDistribution("cg after reading.");
      
      cg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal);

      if( Oges::debug >3 )
      {
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  displayMask(cg[grid].mask(),"mask");
      }

      const int inflow=1, outflow=2, wall=3;
  
      // create a twilight-zone function for checking the errors
      OGFunction *exactPointer;
      if( twilightZoneOption==1 ||
	  min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
      {
	// this grid is probably periodic in space, use a trig function
	printF("TwilightZone: trigonometric polynomial, fx=%9.3e, fy=%9.3e, fz=%9.3e\n",fx,fy,fz);
	exactPointer = new OGTrigFunction(fx,fy,fz); 
      }
      else
      {
	printF("TwilightZone: algebraic polynomial\n");
	// cg.changeInterpolationWidth(2);

	int degreeOfSpacePolynomial = orderOfAccuracy; 
	int degreeOfTimePolynomial = 1;
	int numberOfComponents = cg.numberOfDimensions();
	exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					  degreeOfTimePolynomial);
    
      
      }
      OGFunction & exact = *exactPointer;

      // make a grid function to hold the coefficients
      Range all;
      Index I1,I2,I3, Ia1,Ia2,Ia3;

      const int width=orderOfAccuracy+1;
      int stencilSize=int(pow(width,cg.numberOfDimensions())+1);  // add 1 for interpolation equations

      realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 

      const int numberOfGhostLines=orderOfAccuracy/2;
      coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
      coeff=0.;
    
      // create grid functions: 
      realCompositeGridFunction u(cg),f(cg);
      realCompositeGridFunction err(cg);

      real error;

      CompositeGridOperators op(cg);                            // create some differential operators
      op.setStencilSize(stencilSize);
      op.setOrderOfAccuracy(orderOfAccuracy);
      //   op.setTwilightZoneFlow(TRUE);
      // op.setTwilightZoneFlowFunction(exact);

      f.setOperators(op); // for apply the BC
      coeff.setOperators(op);
  
      // cout << "op.laplacianCoefficients().className: " << (op.laplacianCoefficients()).getClassName() << endl;
      // cout << "-op.laplacianCoefficients().className: " << (-op.laplacianCoefficients()).getClassName() << endl;
    
      if( false )
      {
	coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
      }
      else
      { // new way for parallel -- this avoids all communication
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
	  op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid],I1,I2,I3);
	}
      }

      // fill in the coefficients for the boundary conditions
      coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
      coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
      BoundaryConditionParameters bcParams;
      if( orderOfAccuracy==4 )
      {
	bcParams.ghostLineToAssign=2;
	coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries,bcParams); // extrap 2nd ghost line
      }
      coeff.finishBoundaryConditions();
      // coeff.display("Here is coeff after finishBoundaryConditions");

      if( false )
      {
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  displayCoeff(coeff[grid],sPrintF("Coeff matrix for grid %i",grid));
	  
	  // coeff[grid].sparse->classify.display("the classify matrix after applying finishBoundaryConditions()");
	  //	  coeff[grid].display("this is the coefficient matrix");
	}
      }
    

      Oges solver( cg );                     // create a solver
      
      #ifdef USE_PPP
      if( solverType==OgesParameters::PETScNew )
      {
        Oges::OGES_COMM_WORLD=myCommWorld;
	solver.setCommunicator( myComm );
      }
      #endif 
      
      solver.set(OgesParameters::THEsolverType,solverType); 

      
      if( outputMatrix )
	solver.set(OgesParameters::THEkeepSparseMatrix,true);
      
      if( solver.isSolverIterative() ) 
      {
	solver.setCommandLineArguments( argc,argv );

	if( iterativeSolverType=="gmres" )
	{
	  solver.set(OgesParameters::THEsolverMethod,OgesParameters::generalizedMinimalResidual);
	  solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
	}
	else if( iterativeSolverType=="hypre" )
	{
          // NOTE: hypre is called through PETSc
          // NOTE: Hypre AMG is a PC type within a Kyrlov solver such as gmres or bcgs, 
          solver.set(OgesParameters::THEparallelSolverMethod,OgesParameters::gmres);
	  solver.set(OgesParameters::THEparallelPreconditioner,OgesParameters::hyprePreconditioner);
	  solver.set(OgesParameters::THEpreconditioner,OgesParameters::hyprePreconditioner);
          solver.set(OgesParameters::THEparallelExternalSolver,OgesParameters::hypre);

          solver.parameters.setPetscOption("-ksp_type","gmres");
          solver.parameters.setPetscOption("-pc_type","hypre");
          solver.parameters.setPetscOption("-pc_hypre_type","boomeramg");
          solver.parameters.setPetscOption("-pc_hypre_boomeramg_strong_threshold",".5");
          solver.parameters.setPetscOption("-pc_hypre_boomeramg_max_levels","20");
          solver.parameters.setPetscOption("-pc_hypre_boomeramg_coarsen_type","Falgout");

	}
      	else
	{
	  if( solverType==OgesParameters::PETSc )
	    solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
	  else if( solverType==OgesParameters::PETScNew )
	  { // parallel: -- NOTE: in parallel the solveMethod should be preonly and the parallelSolverMethod bicgs etc.
	    solver.set(OgesParameters::THEbestIterativeSolver);
	    // solver.set(OgesParameters::THEparallelSolverMethod,OgesParameters::biConjugateGradient);
	    // solver.set(OgesParameters::THEparallelSolverMethod,OgesParameters::gmres);
            // Use an LU solver on each processor:
            // solver.set(OgesParameters::THEpreconditioner,OgesParameters::luPreconditioner);
            // This also works: Use an LU on each processor:
            // solver.parameters.setPetscOption("-sub_pc_type","lu");
	  }
	  else
	    solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradient);
	}
	
	solver.set(OgesParameters::THErelativeTolerance,max(tol,REAL_EPSILON*10.));
	solver.set(OgesParameters::THEmaximumNumberOfIterations,10000);
	if( iluLevels>=0 )
	  solver.set(OgesParameters::THEnumberOfIncompleteLULevels,iluLevels);
      }    

      printF("\n === Solver:\n %s\n =====\n",(const char*)solver.parameters.getSolverName());

      if( false )
	solver.parameters.display();
      

      // ---------------------------------
      // --------- Dirichlet BC's --------
      // ---------------------------------
      if( problemsToSolve % 2 ==1 )
      {

	solver.setCoefficientArray( coeff );   // supply coefficients
	// Assign the right-hand-side f  
	assignForcing( 0,cg,f,exact );
       
  
	u=0.;  // initial guess for iterative solvers
	real time0=CPU();
	solver.solve( u,f );   // solve the equations
	real time= ParallelUtility::getMaxValue(CPU()-time0);
	printF("\n*** max residual=%8.2e, time for 1st solve of the Dirichlet problem = %8.2e (iterations=%i) ***\n",
	       solver.getMaximumResidual(),time,solver.getNumberOfIterations());

	// solve again
	if( true )
	{
	  // u=0.;
	  time0=CPU();
	  solver.solve( u,f );   // solve the equations
	  time= ParallelUtility::getMaxValue(CPU()-time0);
	  printF("\n*** max residual=%8.2e, time for 2nd solve of the Dirichlet problem = %8.2e (iterations=%i) ***\n\n",
		 solver.getMaximumResidual(),time,solver.getNumberOfIterations());
	}
	if( outputMatrix )
	{
	  printF("tcm3:INFO: save the matrix to file tcm3Matrix.out (using writeMatrixToFile). \n");
	  solver.writeMatrixToFile("tcm3Matrix.out");

	  aString fileName = "sparseMatrix.dat";
	  printF("tcm3:INFO: save the matrix to file %s (using outputSparseMatrix)\n",(const char*)fileName);
	  solver.outputSparseMatrix( fileName );
	}
      
      
	// ---- check the errors in the solution ---

	const int numberOfGridsPoints=max(1,cg.numberOfGridPoints());
	const real solverSize=solver.sizeOf();
	printF(".....solver: size = %8.2e (bytes), grid-pts=%i, reals/grid-pt=%5.2f \n",
	       solverSize,numberOfGridsPoints,solverSize/(numberOfGridsPoints*sizeof(real)));

	// u.display("Here is the solution to Laplacian(u)=f");
	computeTheError( 0,cg,u,err,exact, error );
	worstError=max(worstError,error);

	checker.setCutOff(errorBound[it][precision][0]); checker.printMessage("dirichlet: error",error,time);
  
	if( plot )
	{
	  ps.createWindow("tcm3");
	  plotResults( ps,solver,u,err );
	}
	
      }


      // ------------------------------------------
      // --------- Neumann or Mixed BC's ----------
      // ------------------------------------------
      if( (problemsToSolve/2) % 2 ==1 ||
          (problemsToSolve/4) % 2 ==1 )
      {
        bool neumannBCs =(problemsToSolve/2) % 2 ==1;
        bool mixedBCs   =(problemsToSolve/4) % 2 ==1;
	aString optionName = neumannBCs ? "neumann" : "mixed";

	coeff=0.;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
	  op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid],I1,I2,I3);
	}

	// fill in the coefficients for the boundary conditions
        RealArray *varCoeff=NULL;  // holds variable coefficients
	if( neumannBCs )
	{
  	  coeff.applyBoundaryConditionCoefficients(0,0,neumann,allBoundaries);
	}
	else
	{
          // -- mixed BC's with variable coefficients --

	  bcParams.setVariableCoefficientOption(  BoundaryConditionParameters::spatiallyVaryingCoefficients );
	  varCoeff = new RealArray [cg.numberOfComponentGrids()];
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg[grid];
            int numGhost=1;
	    getIndex(mg.gridIndexRange(),I1,I2,I3,numGhost);
            realArray & vertex = mg.vertex();
	    OV_GET_SERIAL_ARRAY_CONST(real,vertex,x);
	    int includeGhost=1;
	    bool ok = ParallelUtility::getLocalArrayBounds(vertex,x,I1,I2,I3,includeGhost);
	    if( ok ) 
	    {
	      // varCoeff only needs to be allocated on the boundary allocate on entire grid 
              // so we can assign all boundaries in one call (below)
	      RealArray & vc = varCoeff[grid];
	      vc.redim(I1,I2,I3,2);  // holds variable coefficients
	      bcParams.setVariableCoefficientsArray( &vc );        

	      // coeff of u 
	      vc(I1,I2,I3,0)=1.+ .025*SQR(x(I1,I2,I3,0)) + .03*SQR(x(I1,I2,I3,1));   
	      // coeff of u.n : (this value must not be zero)
	      vc(I1,I2,I3,1)=2. + .1*SQR(x(I1,I2,I3,0)) + .05*SQR(x(I1,I2,I3,1)); 
	    }
	    
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,mixed,allBoundaries,bcParams);

	    // reset:
	    bcParams.setVariableCoefficientsArray( NULL ); 
	  }
          // reset: 
          bcParams.setVariableCoefficientOption( BoundaryConditionParameters::spatiallyConstantCoefficients );
	  
	}
	
	if( orderOfAccuracy==4 )
	  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries,bcParams); // extrap 2nd ghost line

	coeff.finishBoundaryConditions();
	if( false )
	{
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	    ::displayCoeff(coeff[grid],sPrintF("coeff on grid=%i",grid));
	}
	

        solver.setCoefficientArray( coeff );   // supply coefficients

	bool singularProblem=neumannBCs; 
	// for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	// { // this loop does nothing for now 
	//   MappedGrid & mg = cg[grid];
	//   ForBoundary(side,axis)
	//   {
	//     if( mg.boundaryCondition(side,axis) > 0  )
	//     { 
	//     }
	//     else if( mg.boundaryCondition(side,axis) ==inflow ||  mg.boundaryCondition(side,axis) ==outflow )
	//     {
	//       singularProblem=false;
	//     }
	//   }
	// }

	// Assign the right-hand-side f  
	const int option = neumannBCs ? 1 : 2;
	assignForcing( option,cg,f,exact,varCoeff );

        delete [] varCoeff;

	// if the problem is singular Oges will add an extra constraint equation to make the system nonsingular
	if( singularProblem )
	  solver.set(OgesParameters::THEcompatibilityConstraint,TRUE);

	// Tell the solver to refactor the matrix since the coefficients have changed
	solver.setRefactor(TRUE);
	// we need to reorder too because the matrix changes a lot for the singular case
	solver.setReorder(TRUE);
	
	if( singularProblem )
	{
	  // we need to first initialize the solver before we can fill in the rhs for the compatibility equation
	  solver.initialize();
	  realCompositeGridFunction ue(cg);
	  exact.assignGridFunction(ue,0.);
	  real value=0.;
	  solver.evaluateExtraEquation(ue,value);

	  solver.setExtraEquationValues(f,&value );
	}
	
	u=0.;  // initial guess for iterative solvers
	real time0=CPU();
	solver.solve( u,f );   // solve the equations
	real time= ParallelUtility::getMaxValue(CPU()-time0);
	printF("\n*** residual=%8.2e, time for 1st solve of the %s problem = %8.2e (iterations=%i)\n",
	       solver.getMaximumResidual(),(const char*)optionName,time,solver.getNumberOfIterations());

	// turn off refactor for the 2nd solve
	solver.setRefactor(FALSE);
	solver.setReorder(FALSE);
	// u=0.;  // initial guess for iterative solvers
	time0=CPU();
	solver.solve( u,f );   // solve the equations
	time= ParallelUtility::getMaxValue(CPU()-time0);
	printF("\n*** residual=%8.2e, time for 2nd solve of the %s problem = %8.2e (iterations=%i)\n\n",
	       solver.getMaximumResidual(),(const char*)optionName, time,solver.getNumberOfIterations());

	computeTheError( option,cg,u,err,exact, error );

	worstError=max(worstError,error);
      
	checker.setCutOff(errorBound[it][precision][1]);
	aString buff;
	checker.printMessage(sPrintF(buff,"%s: error",(const char*)optionName),error,time);
      
      }
      
      if( plot )
      {
	if( !( problemsToSolve % 2 ==1 ))
	  ps.createWindow("tcm3");
	plotResults( ps,solver,u,err );
      }

      delete exactPointer; exactPointer=0;// kkc 090902, this was a memory leak making new OGFunction's for each grid w/o releasing the previous one

    }  // end it (number of grids)
    

  }  // end sparseSolver

#ifdef USE_PPP
  if( communicatorWasNewed )
  {
    if( myComm !=MPI_COMM_NULL )
      MPI_Comm_free(&myComm);
    if( myCommWorld !=MPI_COMM_NULL )
      MPI_Comm_free(&myCommWorld);
  }
#endif
  
  fflush(0);
  printF("\n\n ************************************************************************************************\n");
  if( worstError > .025 )
    printF(" ************** Warning, there is a large error somewhere, worst error =%e ******************\n",
	   worstError);
  else
    printF(" ************** Test apparently successful, worst error =%e ******************\n",worstError);
  printF(" **************************************************************************************************\n\n");

  fflush(0);

  Overture::finish();          

  return(0);
}


