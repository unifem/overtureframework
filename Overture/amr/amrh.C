// ====================================================================
//  amrh: Sample parallel amr hyperbolic solver 
// ====================================================================
// 
// Examples running in parallel: 
//
//   mpirun -np 2 amrh amrhtz
//   mpirun -np 2 amrh -noplot amrhtz
//   mpirun -np 2 -dbg=valgrindebug amrh
// mcr:
//   mpirun-wdh -np 1 amrh amrhtz
//   totalview srun -a -N2 -n2 -ppdebug amrh amrhtz
// valgrind:
//   srun -ppdebug -n 2 -N 2 memcheck_all amrh -noplot amrhtz
//         memcheckview amrh.????.p.mc   (for processor p)
// 
#include "Overture.h"  
#include "PlotStuff.h"
#include "AnnulusMapping.h"
#include "SquareMapping.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGPulseFunction.h"

#include "Regrid.h"
#include "ErrorEstimator.h"
#include "InterpolateRefinements.h"
#include "Ogen.h"

#include "interpPoints.h"

#include "util.h"

#include "ParentChildSiblingInfo.h"
#include "ParallelUtility.h"
#include "App.h"
#include "LoadBalancer.h"
#include "GridStatistics.h"

// int setMaskAtRefinements(GridCollection & gc );

FILE *debugFile;
FILE *checkFile;
static real a=1.,b=1.,c=1.,nu=-1.,anu=-1.;
static bool twilightZone=false;
OGFunction *tzFunction=NULL;
real timeForBoundaryConditions=0.;
real timeForSolveInterpolate=0.;
real timeForDudt=0.;

real
getDt(const real & cfl, 
      const real & a, 
      const real & b, 
      const real & nu, 
      MappedGrid & mg, 
      MappedGridOperators & op,
      const real alpha0 = -2.,
      const real beta0  = 1. );
real
getDt(const real & cfl, 
      const real & a, 
      const real & b, 
      const real & c, 
      const real & nu, 
      MappedGrid & mg, 
      MappedGridOperators & op,
      const real alpha0 = -2.,
      const real beta0  = 1. );

int 
rungeKutta1(real & t, 
	    real dt,
	    realCompositeGridFunction & u1, 
	    realCompositeGridFunction & u2 );

int 
rungeKutta2(real & t, 
	    real dt,
	    realCompositeGridFunction & u1, 
	    realCompositeGridFunction & u2, 
	    realCompositeGridFunction & u3 );

int 
rungeKutta4(real & t, 
	    real dt,
	    realCompositeGridFunction & u1, 
	    realCompositeGridFunction & u2, 
	    realCompositeGridFunction & u3, 
	    realCompositeGridFunction & u4 );

int 
applyBoundaryConditions( realCompositeGridFunction & u, real t )
// =================================================================================================
// /Description:
//      Apply boundary conditions for an overlapping grids with refinements.
// =================================================================================================
{
  CompositeGrid & cg = *u.getCompositeGrid();
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++)
  {
    // **** wdh 080410: Is this next line needed??? -> u.interpolate() does this at the start)
    u[grid].updateGhostBoundaries();  // update parallel ghost boundaries
  }
  
  checkArrayIDs("applyBC: before interpolate"); 

  real time0=getCPU();
  u.interpolate();          // this includes AMR interpolation.
  timeForSolveInterpolate+=getCPU()-time0;
  
  checkArrayIDs("applyBC: after interpolate"); 

  // apply true boundary conditions.
  time0=getCPU();
  u.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t);
  u.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t);
  u.finishBoundaryConditions();

  timeForBoundaryConditions+=getCPU()-time0;
  return 0;
}

int 
dudt( realCompositeGridFunction & u, realCompositeGridFunction & ut, real t )
{
  real time0=getCPU();
  
  CompositeGrid & cg = *u.getCompositeGrid();
  CompositeGridOperators & op = *u.getOperators();
  Index I1,I2,I3;
  
  if( cg.numberOfDimensions()==2 )
  {
    // ut=(-a)*u.x()+(-b)*u.y() + nu*u.laplacian();

    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.gridIndexRange(),I1,I2,I3);

      #ifdef USE_PPP
	realSerialArray utLocal; getLocalArrayWithGhostBoundaries(ut[grid],utLocal);
	realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u[grid],uLocal);

	bool ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3);
	if( !ok ) continue;
      #else
        realSerialArray & utLocal = ut[grid];
        realSerialArray & uLocal = u[grid]; 
      #endif

      realSerialArray ux(uLocal);
      realSerialArray uy(uLocal);
      realSerialArray lap(uLocal);

      op[grid].derivative(MappedGridOperators::xDerivative,uLocal,ux,I1,I2,I3);
      op[grid].derivative(MappedGridOperators::yDerivative,uLocal,uy,I1,I2,I3);
      op[grid].derivative(MappedGridOperators::laplacianOperator,uLocal,lap,I1,I2,I3);
      
      utLocal(I1,I2,I3)=(-a)*ux(I1,I2,I3)+(-b)*uy(I1,I2,I3) +nu*lap(I1,I2,I3);


    }
    
//     if( true )
//     {
//       ut.display(sPrintF("dudt: ut before TZ t=%9.3e",t),debugFile,"%9.2e ");
//     }

    if( twilightZone )
    {
      assert( tzFunction!=NULL );
      OGFunction & exact = *tzFunction;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	getIndex(mg.gridIndexRange(),I1,I2,I3);

//  	ut[grid](I1,I2,I3)+=exact.t(mg,I1,I2,I3,0,t) + a*exact.x(mg,I1,I2,I3,0,t)+ 
//  				b*exact.y(mg,I1,I2,I3,0,t) - nu*exact.laplacian(mg,I1,I2,I3,0,t);

        const bool isRectangular=false;  // do this for now
        realArray & utg = ut[grid];

        getIndex( mg.dimension(),I1,I2,I3);
        #ifdef USE_PPP
	  realSerialArray utLocal; getLocalArrayWithGhostBoundaries(utg,utLocal);
	  realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(mg.center(),xLocal);

	  bool ok=ParallelUtility::getLocalArrayBounds(utg,utLocal,I1,I2,I3,1);
	  if( !ok ) continue;
	  
        #else
	  realSerialArray & utLocal = utg;
	  const realSerialArray & xLocal = mg.center();
        #endif

        realSerialArray vLocal(I1,I2,I3);

        int ntd=1, nxd=0, nyd=0,nzd=0; // defines a derivative
	exact.gd( vLocal,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
        utLocal(I1,I2,I3)+=vLocal(I1,I2,I3);  // add e.t
	
        ntd=0; nxd=1;
	exact.gd( vLocal,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
        utLocal(I1,I2,I3)+=a*vLocal(I1,I2,I3);  // add a*e.x

        ntd=0; nxd=0; nyd=1;
	exact.gd( vLocal,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
        utLocal(I1,I2,I3)+=b*vLocal(I1,I2,I3);  // add b*e.y

        ntd=0; nxd=2; nyd=0; 
	exact.gd( vLocal,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
        utLocal(I1,I2,I3)-=nu*vLocal(I1,I2,I3);  // add -nu*e.xx

        ntd=0; nxd=0; nyd=2; 
	exact.gd( vLocal,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
        utLocal(I1,I2,I3)-=nu*vLocal(I1,I2,I3);  // add -nu*e.yy

        // should not be necessary to update ghost boundaries on utg
      }
    }
    if( anu>0. )
    {
      // add artificial viscosity
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
	realArray & utg = ut[grid];
	realArray & v = u[grid];
	  
        #ifdef USE_PPP
	  realSerialArray utLocal; getLocalArrayWithGhostBoundaries(utg,utLocal);
	  realSerialArray vLocal;  getLocalArrayWithGhostBoundaries(v,vLocal);

	  bool ok=ParallelUtility::getLocalArrayBounds(utg,utLocal,I1,I2,I3,0);
	  if( !ok ) continue;

        #else
	  realSerialArray & utLocal = utg;
	  realSerialArray & vLocal = v;
        #endif

	utLocal(I1,I2,I3)+=
           ((anu/cg[grid].gridSpacing(0))*( vLocal(I1+1,I2  ,I3)-2.*vLocal(I1,I2,I3)+vLocal(I1-1,I2  ,I3))+
	    (anu/cg[grid].gridSpacing(1))*( vLocal(I1  ,I2+1,I3)-2.*vLocal(I1,I2,I3)+vLocal(I1  ,I2-1,I3)));

      }
    }
  }
  else if( cg.numberOfDimensions()==3 )
  {
    // ut=(-a)*u.x()+(-b)*u.y()+(-c)*u.z() + nu*u.laplacian();

    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.gridIndexRange(),I1,I2,I3);
      
      // new way
      realArray & ug = u[grid];
	
      realSerialArray utLocal; getLocalArrayWithGhostBoundaries(ut[grid],utLocal);
      realSerialArray ugLocal; getLocalArrayWithGhostBoundaries(ug,ugLocal);

      bool ok = ParallelUtility::getLocalArrayBounds(ug,ugLocal,I1,I2,I3);  
      if( !ok ) continue;

      realSerialArray ux(I1,I2,I3);
	  
      op[grid].derivative(MappedGridOperators::xDerivative,ugLocal,ux,I1,I2,I3);
      utLocal(I1,I2,I3)=(-a)*ux;
      op[grid].derivative(MappedGridOperators::yDerivative,ugLocal,ux,I1,I2,I3);
      utLocal(I1,I2,I3)+=(-b)*ux;
      op[grid].derivative(MappedGridOperators::zDerivative,ugLocal,ux,I1,I2,I3);
      utLocal(I1,I2,I3)+=(-c)*ux;
      op[grid].derivative(MappedGridOperators::laplacianOperator,ugLocal,ux,I1,I2,I3);
      utLocal(I1,I2,I3)+=nu*ux;

    }
    
    if( twilightZone )
    {
      assert( tzFunction!=NULL );
      OGFunction & exact = *tzFunction;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	getIndex(mg.gridIndexRange(),I1,I2,I3);


        #ifdef USE_PPP
         realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
    	 realSerialArray utLocal; getLocalArrayWithGhostBoundaries(ut[grid],utLocal);
        #else
	  const realSerialArray & xLocal = mg.center(); 
    	  realSerialArray & utLocal=ut[grid]; 
        #endif

	bool ok = ParallelUtility::getLocalArrayBounds(ut[grid],utLocal,I1,I2,I3);  
	if( !ok ) continue;

	realSerialArray ue(I1,I2,I3);
	const bool isRectangular=false;  // do this for now

	int ntd=1, nxd=0, nyd=0, nzd=0; // defines a derivative
	exact.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
	utLocal(I1,I2,I3)+=ue(I1,I2,I3);  // add e.t

	ntd=0; nxd=1;
	exact.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
	utLocal(I1,I2,I3)+=a*ue(I1,I2,I3);  // add a*e.x

	ntd=0; nxd=0; nyd=1;
	exact.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
	utLocal(I1,I2,I3)+=b*ue(I1,I2,I3);  // add b*e.y

	ntd=0; nxd=0; nyd=0; nzd=1;
	exact.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
	utLocal(I1,I2,I3)+=c*ue(I1,I2,I3);  // add c*e.z

	ntd=0; nxd=2; nyd=0; nzd=0;
	exact.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
	utLocal(I1,I2,I3)-=nu*ue(I1,I2,I3);  // add -nu*e.xx

	ntd=0; nxd=0; nyd=2; nzd=0;
	exact.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
	utLocal(I1,I2,I3)-=nu*ue(I1,I2,I3);  // add -nu*e.yy

	ntd=0; nxd=0; nyd=0; nzd=2;
	exact.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,t);
	utLocal(I1,I2,I3)-=nu*ue(I1,I2,I3);  // add -nu*e.zz

      }
    }
    if( anu>0. )
    {
      // add artificial viscosity
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
	  
#ifdef USE_PPP
	realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	realSerialArray utLocal; getLocalArrayWithGhostBoundaries(ut[grid],utLocal);
#else
	realArray & uLocal = u[grid];
	realSerialArray & utLocal=ut[grid]; 
#endif

	bool ok = ParallelUtility::getLocalArrayBounds(ut[grid],utLocal,I1,I2,I3);  
	if( !ok ) continue;

	utLocal(I1,I2,I3)+=
	  ((anu/cg[grid].gridSpacing(0))*( uLocal(I1+1,I2  ,I3)-2.*uLocal(I1,I2,I3)+uLocal(I1-1,I2  ,I3))+
	   (anu/cg[grid].gridSpacing(1))*( uLocal(I1  ,I2+1,I3)-2.*uLocal(I1,I2,I3)+uLocal(I1  ,I2-1,I3))+
	   (anu/cg[grid].gridSpacing(2))*( uLocal(I1  ,I2,I3+1)-2.*uLocal(I1,I2,I3)+uLocal(I1  ,I2,I3-1)));

      }
    }
  }
  else if( cg.numberOfDimensions()==1 )
  {
    ut=(-a)*u.x() + nu*u.laplacian();
    if( twilightZone )
    {
      assert( tzFunction!=NULL );
      OGFunction & exact = *tzFunction;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	getIndex(mg.gridIndexRange(),I1,I2,I3);
	ut[grid](I1,I2,I3)+=exact.t(mg,I1,I2,I3,0,t)+a*exact.x(mg,I1,I2,I3,0,t)-nu*exact.laplacian(mg,I1,I2,I3,0,t);
      }
    }
  }

  if( false )
  {
    u.display(sPrintF("dudt: u at time t=%9.3e",t),debugFile,"%9.2e ");
    ut.display(sPrintF("dudt: ut at time t=%9.3e",t),debugFile,"%9.2e ");
  }

  timeForDudt+=getCPU()-time0;
  return 0;
}


real
getTimeStep( CompositeGrid & cg, CompositeGridOperators & op, real cfl, real cflr, real cfli )
{
  real dt=REAL_MAX;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    real dtGrid=0.;
    if( cg.numberOfDimensions()==2 )
    {
      dtGrid = getDt(cfl,a,b,nu,cg[grid],op[grid],cflr,cfli);
    }
    else if( cg.numberOfDimensions()==3 )
    {
      dtGrid = getDt(cfl,a,b,c,nu,cg[grid],op[grid],cflr,cfli);
    }
    else if( cg.numberOfDimensions()==1 )
    {
      dtGrid=.5*cfl/( 2.*fabs(a)/cg[grid].gridSpacing(axis1) +
		      4.*nu/SQR(cg[grid].gridSpacing(axis1)) );
    }
    else
    {
      Overture::abort("error");
    }
    dt=min(dt,dtGrid);
    if( false )
    {
      printF("Get time step: grid=%i (baseGrid=%i) dtGrid=%8.2e\n",grid,cg.baseGridNumber(grid),dtGrid);
    }
  }
  
  if( false && anu>0. )  // ***************************
  {
    if( nu>0. )
      dt*=.25;
    else
      dt*=.5;
  }

  return dt;
}



int
checkParallelConsistency(realCompositeGridFunction & u, const aString & label )
// ------------------------------------------------------------------
// Perform some consistency checks in parallel
// ------------------------------------------------------------------
{
  #ifndef USE_PPP
    return 0;
  #endif

  fflush(0);
  Communication_Manager::Sync();

  const int myid=max(0,Communication_Manager::My_Process_Number);
  // check the grid for validity of the mask
  CompositeGrid & cg = *u.getCompositeGrid();
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u[grid],uLocal);
    intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);

    testConsistency(u[grid], label+"(u)" );
    testConsistency(cg[grid].mask(),label+"(mask)" );

    if( u[grid].grid != cg[grid].rcData )
    {
       printf("checkParallelConsistency:ERROR:myid=%i, %s, u[grid].grid != cg[grid].rcData!! grid=%i\n",
	     myid,(const char*)label,grid);
    }
    

    if( maskLocal.dimension(0)!=uLocal.dimension(0) || maskLocal.dimension(1)!=uLocal.dimension(1) || 
        maskLocal.dimension(2)!=uLocal.dimension(2) )
    {
      printf("checkParallelConsistency:ERROR:myid=%i, %s, maskLocal does not match uLocal!! grid=%i\n",
	     myid,(const char*)label,grid);
      printf(" maskLocal: [%i,%i][%i,%i][%i,%i]\n",maskLocal.getBase(0),maskLocal.getBound(0),
	     maskLocal.getBase(1),maskLocal.getBound(1),maskLocal.getBase(2),maskLocal.getBound(2));
      printf(" uLocal: [%i,%i][%i,%i][%i,%i]\n",uLocal.getBase(0),uLocal.getBound(0),
	     uLocal.getBase(1),uLocal.getBound(1),uLocal.getBase(2),uLocal.getBound(2));
		
      fflush(0);
      Communication_Manager::Sync();

      Overture::abort("error");
    }
  }


  return 0;
}



int 
main(int argc, char *argv[])
{

  Overture::start(argc,argv);  // initialize Overture
  // Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );

  Optimization_Manager::setForceVSG_Update(Off);

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());

  Overture::turnOnMemoryChecking(true);

  MappedGrid::setMinimumNumberOfDistributedGhostLines(2);  // need two ghost lines for amr interp

  // Set the maximum width needed to extrapolate interpolation neighbours: 
  // GenericMappedGridOperators::setDefaultMaximumWidthForExtrapolateInterpolationNeighbours(3);
  
  int debug=0;
  bool plotPoints=true; 
#ifdef USE_PPP
  plotPoints=false;  // this needs to be fixed in parallel
#endif  
  printF(" ------------------------------------------------------------------- \n");
  printF(" Hyperbolic AMR Solver                                               \n");
  printF("    output written to amrh.debug (also amrh.check)                   \n");
  printF(" Usage: amrh [-cmd=file.cmd] [options]                                    \n");
  printF("  options: -grid=file.hdf                                            \n");
  printF("         : -rr=[2,4]   : refinement ratio                            \n");
  printF("         : -rl=[1,2...]: refinement levels                           \n");
  printF("         : -nb=[1,2...]: number of buffer zones                      \n");
  printF("         : -et=error threshold                                       \n");
  printF("         : -tf=final time                                            \n");
  printF("         : -noplot                                                   \n");
  printF("         : -cf=checkFileName                                         \n");
  printF(" ------------------------------------------------------------------- \n");
  
  aString commandFileName="";
  aString arg, gridName="", method="";
  
  int numberOfRefinementLevels=3; // 3;
  int refinementRatio=2;
  int numberOfBufferZones=-1; // -1 : use default value

  int plotResults=true;
  aString checkFileName="amrh.check";
  real errorThreshold=.01;
  int regridInterval=-1; // -1 : use default value  
  
  // Somehow the partition in an array can be wrong if we do not destroy the grid function before an update
  bool destroyBeforeUpdate=false;  // for a bug 
  bool destroyGridBeforeRegrid=false; // true; // false;

  real tFinal=.1;
  int len=0;
  for( int i=1; i<argc; i++ )
  {
    arg=argv[i];
    if( arg(0,5)=="-grid=" )
    {
      gridName=arg(6,arg.length()-1);
      printF("grid=[%s]\n",(const char*)gridName);
    }
    else if( arg(0,3)=="-rr=" )
    {
      sScanF(arg(4,arg.length()-1),"%i",&refinementRatio);
      assert( refinementRatio==2 || refinementRatio==4 );
      printF("refinementRatio=%i\n",refinementRatio);
    }
    else if( arg(0,3)=="-rl=" )
    {
      sScanF(arg(4,arg.length()-1),"%i",&numberOfRefinementLevels);
      assert( numberOfRefinementLevels>=1 && numberOfRefinementLevels<100 );
      printF("numberOfRefinementLevels=%i\n",numberOfRefinementLevels);
    }
    else if( arg(0,3)=="-nb=" )
    {
      sScanF(arg(4,arg.length()-1),"%i",&numberOfBufferZones);
      printF("numberOfBufferZones=%i\n",numberOfBufferZones);
      assert( numberOfBufferZones>=1 && numberOfBufferZones<100 );
    }
    else if( arg(0,3)=="-et=" )
    {
      sScanF(arg(4,arg.length()-1),"%e",&errorThreshold);
      printF("errorThreshold=%e\n",errorThreshold);
      assert( errorThreshold>=0. );
    }
    else if( arg(0,3)=="-tf=" )
    {
      sScanF(arg(4,arg.length()-1),"%e",&tFinal);
      printF("tFinal=%e\n",tFinal);
      assert( tFinal>=0. );
    }
    else if( arg(0,6)=="-noplot" )
    {
      plotResults=false;
    }
    else if( arg(0,3)=="-cf=" )
    {
      checkFileName=arg(4,arg.length()-1);
      printF("checkFileName=[%s]\n",(const char*)checkFileName);
    }
    else if( len=arg.matches("-cmd=") )
    {
      commandFileName=arg(len,arg.length()-1);
      printF("Use command file =%s\n",(const char*)commandFileName);
    }
    else if( i==1 )
    {  // for backward compatibility
      commandFileName=arg;
      printF("Use command file =%s\n",(const char*)commandFileName);
    }
  }


  Range all;

  if( np==1 )
    debugFile = fopen("amrh.debug","w" ); 
  else
    debugFile = fopen(sPrintF("amrh%i.debug",myid),"w" ); 

  checkFile = fopen((const char*)checkFileName,"w" ); 


  fPrintF(checkFile,"amrh: commandFile=%s \n",(const char *)commandFileName);

  // NOTE: this next call is important to get the command line arguments passed to the cmd file:
  GL_GraphicsInterface & ps = (GL_GraphicsInterface&)(*Overture::getGraphicsInterface("amrh",plotResults,argc,argv));
//  PlotStuff ps(plotResults, "amrh");       // create a PlotStuff object

  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];
  aString logFile="amrh.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);


  if( gridName=="" )
    ps.inputString(gridName,"Enter the grid to use");

  // cg0 : holds original grid (so we can rerun from scratch)
  // cga[2] : old and new Composite grids (ring buffer).
  CompositeGrid cga[2], cg0;
  bool loadBalance=true;
  getFromADataBase(cg0,gridName,loadBalance);

  Overture::checkMemoryUsage("amrh after getFromADataBase");  

  cga[1]=cg0;  // *****
  
  if( debug &2 )
  {
    cg0.displayDistribution("cg0 (start)");
    cga[1].displayDistribution("cga[1] (start)");
  }

  Regrid regrid;
  regrid.turnOnLoadBalacing( true ); // for load balancing AMR grids
  LoadBalancer & lb = regrid.getLoadBalancer(); // here is the load balancer used by Regrid
  // lb.setLoadBalancer(LoadBalancer::sequentialAssignment);

  regrid.setRefinementRatio(refinementRatio);
  if( numberOfBufferZones!=-1 )
  {
    regrid.setNumberOfBufferZones(numberOfBufferZones);  // expansion of tagged error points
    regrid.setWidthOfProperNesting(numberOfBufferZones); // distance between levels
  }

  InterpolateRefinements interp(cg0.numberOfDimensions());

//   IntegerArray ratio(3);
//   ratio=refinementRatio;
//   interp.setRefinementRatio( ratio );

  ErrorEstimator errorEstimator(interp);

  RealArray scaleFactor(2);
  scaleFactor=1.;
  errorEstimator.setScaleFactor( scaleFactor );
  Ogen ogen;
  
//   ogen.debug=3;  // ***********

  int numberOfDimensions = cg0.numberOfDimensions();

  int numberOfGridPoints=0, maxNumberOfGridPoints=0, averageNumberOfGridPoints=0, numberOfRegrids=0;
  int averageNumberOfGrids=0, maxNumberOfGrids=0, minNumberOfGrids=0;
  minNumberOfGrids=cg0.numberOfComponentGrids();

  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  if( numberOfDimensions==1 )
    psp.set(GI_COLOUR_LINE_CONTOURS,true);
  if( numberOfDimensions==2 )
  {
    psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,true);
  }
  if( numberOfDimensions==3 )
  {
    // psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,true);
    // psp.set(GI_PLOT_SHADED_SURFACE_GRIDS,false);
    // psp.set(GI_PLOT_SHADED_SURFACE,false);
    // psp.set(GI_PLOT_BLOCK_BOUNDARIES,true);
    // psp.set(GI_PLOT_GRID_LINES,false);
  }
  
  
  //...plot the grid
  if( false )
  {
    psp.set(GI_TOP_LABEL,"Initial grid");
    PlotIt::plot(ps,cg0,psp);
  }
  enum TimeSteppingMethodEnum
    {
      forwardEuler,
      rungeKuttaOrder2,
      rungeKuttaOrder4
    } timeSteppingMethod=rungeKuttaOrder2;

  
  aString menu[]=
    {
      "solve",
      ">initial condition",
      "top hat",
      "top hat parameters",
      "<>time stepping",
      "forward Euler",
      "2nd order Runge Kutta",
      "4th order Runge Kutta",
      "<>parameters",
      "nu",
      "cfl",
      "<number of refinement levels",
      "minimum refinement size",
      "maximum refinement size",
      "grid efficiency",
      "refinement ratio",
      "number of buffer zones",
      "regrid interval",
      ">options",
      "error threshold",
      "use smart bisection",
      "do not use smart bisection",
      "add new grids as refinements",
      "add new grids as base grids",
      "set zero base level",
      "set base level",
      "allow rotated grids",
      "aligned grids",
      "do not merge boxes",
      "order in space",
      "order in time",
      "<change the plot",
      "use twilight zone",
      "use pulse function",
      "use poly",
      "use top hat for error estimator",
      "do not use twilight zone",
      "use 2 point interpolation",
      "use 3 point interpolation",
      "use iterative implicit interpolation",
      "debug",
      "interp debug",
      "ogen debug",
      "error estimator debug",
      "turn off load balancer",
      "turn on load balancer",
      "exit",
      "finish",
      ""
    };
  aString answer;
  char buff[80];
  
  // bool twilightZone=false;

  real efficiency=.7; 
  int minimumRefinementSize=16;
  int maximumRefinementSize=16;
  int baseLevel=-1;

  enum TwilightZoneFunctionEnum
    {
      polyTZ,
      trigTZ,
      pulseTZ
    } twilightZoneFunction=polyTZ;

  bool useTopHatForErrorEstimator=false;

  OGPulseFunction pulse(cg0.numberOfDimensions());
  pulse.setRadius(.25);
      
      

//   RealArray topHatCentre(3),topHatVelocity(3);
//   topHatCentre(0)=.35;
//   topHatCentre(1)=.35;
//   topHatCentre(2)=.0;
//   topHatVelocity(0)=1.;
//   topHatVelocity(1)=1.;
//   topHatVelocity(2)=1.;
//   real topHatRadius=.15;

  int degreeOfSpacePolynomial = 1;
  int degreeOfTimePolynomial = 1;

  bool useIterativeImplicitInterpolation=false;

  // real nu=-1.;
  real cfl=-1.;

  int currentGrid=0;
  
  bool finished=false;
  
  for( ;; )
  {
    ps.getMenuItem(menu,answer,"choose" );
    
    if( answer=="exit" || answer=="done" || answer=="finish" )
    {
      break;
    }
    else if( answer=="top hat" )
    {
    }
    else if( answer=="top hat parameters" )
    {
      real topHatCentre[3]={0.,0.,0.}, topHatVelocity[3]={1.,1.,1.}, topHatRadius=.25;
      real topHatRadiusX=0., topHatRadiusY=0., topHatRadiusZ=0.;
      
      ps.inputString(answer,"Enter the centre");
      sScanF(answer,"%e %e %e",&topHatCentre[0],&topHatCentre[1],&topHatCentre[2]);
      printF("centre = (%e,%e,%e)\n",topHatCentre[0],topHatCentre[1],topHatCentre[2]);
      ps.inputString(answer,"Enter the radius");
      sScanF(answer,"%e %e %e  %e ",&topHatRadius,&topHatRadiusX,&topHatRadiusY,&topHatRadiusZ);
      printF("radius = %e\n",topHatRadius);
      if( topHatRadiusX>0. )
        printF("topHatRadiusX = %e\n",topHatRadiusX);
      if( topHatRadiusY>0. )
        printF("topHatRadiusY = %e\n",topHatRadiusY);
      ps.inputString(answer,"Enter the top hat velocity vector");
      sScanF(answer,"%e %e %e",&topHatVelocity[0],&topHatVelocity[1],&topHatVelocity[2]);
      printF("velocity = %e %e %e\n",topHatVelocity[0],topHatVelocity[1],topHatVelocity[2]);

      errorEstimator.setTopHatParameters( topHatCentre, topHatVelocity,topHatRadius,
					  topHatRadiusX,topHatRadiusY,topHatRadiusZ);         

      pulse.setRadius(topHatRadius);
      pulse.setCentre(topHatCentre[0],topHatCentre[1],topHatCentre[2]);
      pulse.setVelocity(topHatVelocity[0],topHatVelocity[1],topHatVelocity[2]);

    }
    else if( answer=="turn off load balancer" || answer=="turn on load balancer" )
    {
      bool loadBalance = answer=="turn off load balancer"  ? false : true;
      regrid.turnOnLoadBalacing( loadBalance ); // for load balancing AMR grids
    }
    else if( answer=="number of refinement levels" )
    {
      ps.inputString(answer,"Enter the number of refinement levels");
      sScanF(answer,"%i",&numberOfRefinementLevels);
      printF("set numberOfRefinementLevels=%i\n",numberOfRefinementLevels);
      
    }
    else if( answer=="minimum refinement size" )
    {
      ps.inputString(answer,"Enter the minimum refinement size");
      sScanF(answer,"%i",&minimumRefinementSize);
      printF("set minimumRefinementSize=%i\n",minimumRefinementSize);
      
    }
    else if( answer=="maximum refinement size" )
    {
      ps.inputString(answer,"Enter the maximum refinement size");
      sScanF(answer,"%i",&maximumRefinementSize);
      printF("set maximumRefinementSize=%i\n",maximumRefinementSize);
    }
    else if( answer=="grid efficiency" )
    {
      ps.inputString(answer,"Enter the grid efficiency 0< eff < 1");
      sScanF(answer,"%e",&efficiency);
      printF("set efficiency=%e\n",efficiency);

      regrid.setEfficiency(efficiency);
      
    }
    else if( answer=="refinement ratio" )
    {
      ps.inputString(answer,"Enter the refinement ratio");
      sScanF(answer,"%i",&refinementRatio);

      printF("set refinementRatio=%i\n",refinementRatio);

      regrid.setRefinementRatio(refinementRatio);
//       IntegerArray ratio(3);
//       ratio=refinementRatio;
//       interp.setRefinementRatio( ratio );
    }
    else if( answer=="number of buffer zones" )
    {
      ps.inputString(answer,"Enter the flagged region growth size");
      sScanF(answer,"%i",&numberOfBufferZones);
      printF("set numberOfBufferZones=%i\n",numberOfBufferZones);
      
      regrid.setNumberOfBufferZones(numberOfBufferZones);  // expansion of tagged error points
      regrid.setWidthOfProperNesting(numberOfBufferZones); // distance between levels
    }
    else if( answer=="regrid interval" )
    {
      ps.inputString(answer,"Enter the regrid interval");
      sScanF(answer,"%i",&regridInterval);
      printF("set regridInterval=%i\n",regridInterval);
      
//       regrid.setNumberOfBufferZones(numberOfBufferZones);  // expansion of tagged error points
//       regrid.setWidthOfProperNesting(numberOfBufferZones); // distance between levels
    }
    else if( answer=="minimum box size" )
    {
      int minimumBoxSize=16;
      ps.inputString(answer,"Enter the min number of points on a refinement grid");
      sScanF(answer,"%i",&minimumBoxSize);
      printF("set minimumBoxSize=%i\n",minimumBoxSize);
      
      regrid.setMinimumBoxSize(minimumBoxSize);

    }
    else if( answer=="order in space" )
    {
      ps.inputString(answer,"Enter the order in space of the tz polynomial");
      sScanF(answer,"%i",&degreeOfSpacePolynomial);
      printF("set degreeOfSpacePolynomial=%i\n",degreeOfSpacePolynomial);

      interp.setOrderOfInterpolation(degreeOfSpacePolynomial+1);
      
    }
    else if( answer=="order in time" )
    {
      ps.inputString(answer,"Enter the order in time of the tz polynomial");
      sScanF(answer,"%i",&degreeOfTimePolynomial);
      printF("set degreeOfTimePolynomial=%i\n",degreeOfTimePolynomial);
    }
    else if( answer=="error threshold" )
    {
      ps.inputString(answer,"Enter errorThreshold");
      sScanF(answer,"%e",&errorThreshold);
    }
    else if( answer=="use smart bisection" )
    {
      regrid.setUseSmartBisection(true);
    }
    else if( answer=="do not use smart bisection" )
    {
      regrid.setUseSmartBisection(false);
    }
    else if( answer=="add new grids as refinements" )
    {
      regrid.setGridAdditionOption(Regrid::addGridsAsRefinementGrids);
    }
    else if( answer=="add new grids as base grids" )
    {
      regrid.setGridAdditionOption(Regrid::addGridsAsBaseGrids);
    }
    else if( answer=="set base level" )
    {
      ps.inputString(answer,"Enter the base level (-1 = only build new level)");
      sScanF(answer,"%i",&baseLevel);
      printF("set baseLevel=%i\n",baseLevel);
    }
    else if( answer=="set zero base level" )
    {
      baseLevel=0;
      printF("set baseLevel=%i\n",baseLevel);
    }
    else if( answer=="allow rotated grids" )
    {
      regrid.setGridAlgorithmOption( Regrid::rotated );
    }
    else if( answer=="aligned grids" )
    {
      regrid.setGridAlgorithmOption( Regrid::aligned );
    }
    else if( answer=="do not merge boxes" )
    {
      regrid.setMergeBoxes(false);
    }
    else if( answer=="debug" )
    {
      ps.inputString(answer,"Enter debug");
      sScanF(answer,"%i",&debug);
      printF("set debug=%i\n",debug);

      if( debug >3 )
      {
	regrid.debug=debug;
	ogen.debug=debug;
      }
      
    }
    else if( answer=="interp debug" )
    {
      interp.debug=7;
    }
    else if( answer=="ogen debug" )
    {
      ps.inputString(answer,"Enter ogen.debug");
      sScanF(answer,"%i",&ogen.debug);
      printF("set ogen.debug=%i\n",ogen.debug);
    }
    else if( answer=="error estimator debug" )
    {
      errorEstimator.debug=7;
    }
    else if( answer=="use 2 point interpolation" )
    {
      interp.setOrderOfInterpolation(2);
    }
    else if( answer=="use 3 point interpolation" )
    {
      interp.setOrderOfInterpolation(3);
    }
    else if( answer=="use twilight zone" )
    {
      twilightZone=true;
    }
    else if( answer=="use pulse function" )
    {
      twilightZoneFunction=pulseTZ;
    }
    else if( answer=="use poly" )
    {
      twilightZoneFunction=polyTZ;
    }
    else if( answer=="use top hat for error estimator" )
    {
      useTopHatForErrorEstimator=true;
    }
    else if( answer=="do not use top hat for error estimator" )
    {
      useTopHatForErrorEstimator=false;
    }
    else if( answer=="do not use twilight zone" )
    {
      twilightZone=false;
    }
    else if( answer=="use iterative implicit interpolation" )
    {
      useIterativeImplicitInterpolation=true;
    }
    else if( answer=="change the plot" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp.set(GI_TOP_LABEL,"Refined grid");
      PlotIt::plot(ps,cga[currentGrid],psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="forward Euler" )
    {
      timeSteppingMethod=forwardEuler;
    }
    else if( answer=="2nd order Runge Kutta" )
    {
      timeSteppingMethod=rungeKuttaOrder2;
    }
    else if( answer=="4th order Runge Kutta" )
    {
      timeSteppingMethod=rungeKuttaOrder4;
    }
    else if( answer=="nu" )
    {
      ps.inputString(answer,"Enter nu");
      sScanF(answer,"%e",&nu);
      printF("nu=%e\n",nu);
    }
    else if( answer=="cfl" )
    {
      ps.inputString(answer,"Enter cfl");
      sScanF(answer,"%e",&cfl);
    }
    else if( answer=="change load balancer" )
    {
      // change load balancer options
      lb.update(ps);
    }
    else if( answer=="solve" )
    {
      Overture::checkMemoryUsage("amrh solve...");  


      errorEstimator.setMaximumNumberOfRefinementLevels(numberOfRefinementLevels);
   
      CompositeGrid & cgi =cga[currentGrid];
      
      cgi.destroy();
      cgi=cg0;
      // cgi.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
      int grid;
      for( grid=0; grid<cgi.numberOfComponentGrids(); grid++ )
      {
	if( cgi[grid].isRectangular() )
	  cgi[grid].update(MappedGrid::THEmask );
	else
	  cgi[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
      }
      // display(cgi.numberOfInterpolationPoints,"cgi.numberOfInterpolationPoints");
      
      Index I1,I2,I3;                                            
      realCompositeGridFunction u(cgi), ue(cgi), error;
      CompositeGridOperators op(cgi);
      // Interpolant interpolant; 
      Interpolant & interpolant = *new Interpolant(cgi);               // do this instead for now. 
      if( useIterativeImplicitInterpolation )
        interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
      
      interpolant.setInterpolateRefinements(interp);

      int baseLevel=0; // always regenerate to this level

      int nComp = 1;
      OGPolyFunction poly(degreeOfSpacePolynomial,cg0.numberOfDimensions(),nComp,
			   degreeOfTimePolynomial);

      OGFunction & exact = twilightZoneFunction==polyTZ ? (OGFunction &) poly : (OGFunction &) pulse;
      tzFunction=&exact;

      op.setTwilightZoneFlow( twilightZone );  
      op.setTwilightZoneFlowFunction( exact );

      // =================== build the initial AMR grid =========================
      // 

      if( twilightZone )
      {
        // u=exact(cgi,Range(0,0));

        exact.assignGridFunction(u,0.);
	
      }
      else
      {
	// getTrueSolution(u,0.,topHatCentre,topHatVelocity,topHatRadius);
        errorEstimator.computeFunction( u,ErrorEstimator::topHat,0. );

      }
      
      u.setOperators(op);
      
      Overture::checkMemoryUsage("amrh after u.setOperators(op);...");  

      if( debug & 4 )
      {
	cgi.update(GridCollection::THErefinementLevel);

	printf(" ** Initial grid ** \n");
	printInfo( cgi );
      }
      for( int level=1; level<numberOfRefinementLevels; level++ )
      {
	
        int nextGrid= (currentGrid+1) % 2;

        CompositeGrid & cgc = cga[currentGrid];
	CompositeGrid & cgNew = cga[nextGrid];
	

        error.updateToMatchGrid(cgc);
        error=0.; // *wdh* 040427
        interpolant.updateToMatchGrid(cgc,level-1);
        
        op.updateToMatchGrid(cgc);
	
        // u.setOperators(*u.getOperators());
        error.setOperators(*u.getOperators());
        // error.setOperators(op);

        Overture::checkMemoryUsage("amrh after error.setOperators()...");  

        if( !useTopHatForErrorEstimator ) // !twilightZone || twilightZoneFunction==pulseTZ )
	{
          if( false )
	  {
            int numberOfSmooths=0; // * for testing *
  	    errorEstimator.computeAndSmoothErrorFunction(u,error,numberOfSmooths );
	  }
	  else
	  {
	    errorEstimator.computeAndSmoothErrorFunction(u,error );
	  }
	  
	}
        else
	{
          // Base error estimator on the top-hat function:
	  ue.updateToMatchGrid(cgc);
          errorEstimator.computeFunction( ue,ErrorEstimator::topHat,0. );

	  if( debug & 4 )
	  {
            ue.display("top hat solution","%4.1f ");
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	    psp.set(GI_TOP_LABEL,"top hat solution");
	    ps.erase();
	    PlotIt::contour(ps,ue,psp);
	  }

          // int numberOfSmooths=0; // ********************
  	  // errorEstimator.computeAndSmoothErrorFunction(ue,error,numberOfSmooths );
  	  errorEstimator.computeAndSmoothErrorFunction(ue,error);
	}
      
        Overture::checkMemoryUsage("amrh after errorEstimator.computeAndSmoothErrorFunction...");  

        if( debug & 2 )
	{
          if( debug & 4 )
  	    error.display(sPrintF("Regrid initial grid, level=%i, error:",level),debugFile,"%10.4e ");
	
	  printF("Regrid initial conditions: error est.: min=%8.2e, max=%8.2e (tol=%8.2e)\n",
		 min(error),max(error),errorThreshold);

	}
	if( debug & 4 )
	{
          // error.display("error","%4.1f ");
  	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  psp.set(GI_TOP_LABEL,sPrintF(buff,"error pts (level=%i)",level));
	  ps.erase();
	  psp.set(GI_TOP_LABEL,"error");
	  PlotIt::contour(ps,error,psp);
	  if( plotPoints )
	    errorEstimator.plotErrorPoints( error, errorThreshold,ps,psp );
	}
	
        Overture::checkMemoryUsage("amrh before regrid.regrid...");  

        if( destroyGridBeforeRegrid )
  	  cgNew.destroy();   

	regrid.regrid(cgc,cgNew, error, errorThreshold, level, baseLevel);
        numberOfRegrids++;

	minNumberOfGrids=cgc.numberOfComponentGrids();
	
        ogen.updateRefinement(cgNew);

	for( grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
	{
	  // printF(" cgNew[%i].isRectangular()=%i\n",grid,cgNew[grid].isRectangular());
	  
	  if( cgNew[grid].isRectangular() )
	    cgNew[grid].update(MappedGrid::THEmask );
	  else
	    cgNew[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
	}
	  
	
	if( debug & 2 )
	  regrid.outputRefinementInfo( cgNew, gridName,"amrhDebug.cmd" );

        // display(cgNew.interpolationIsImplicit,"interpolationIsImplicit after regrid");

        if( debug & 4 )
	{
          psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
          ps.erase();
	  psp.set(GI_TOP_LABEL,sPrintF(buff,"Refined grid (level=%i)",level));
	  PlotIt::plot(ps,cgNew,psp);
	}
	
        // u.destroy();  // *wdh* 061022 -- is this needed in parallel?

        printF("\n ==== amrh: u.updateToMatchGrid(cgNew);\n");
	
        u.updateToMatchGrid(cgNew);
	if( true )
	{
          printF("*** regrid: after u.updateToMatchGrid(cgNew) :  ****\n");
	  for( int grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cgNew[grid];
	    Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
            const Partitioning_Type & uPartition = u[grid].getPartition();
             
            if( !hasSameDistribution(partition,uPartition) )
	    {
              printF("\n ** ERROR: grid=%i : u[grid] has a different partition from cgNew[grid] !! **\n",grid);
	      const intSerialArray & processorSet = partition.getProcessorSet();
	      printF("  grid=%i: cgNew[grid]: actual-processors=[%i,%i]\n",
		     grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));
	      const intSerialArray & uProcessorSet  = ((Partitioning_Type &)uPartition).getProcessorSet();
	      printF("  grid=%i: u[grid]: actual-processors=[%i,%i]\n",
		     grid,uProcessorSet(uProcessorSet.getBase(0)),uProcessorSet(uProcessorSet.getBound(0)));
	    
	      if( true )
	      {
		CompositeGrid & cg = *u.getCompositeGrid();
		MappedGrid & mg = cg[grid];
		Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
		const intSerialArray & processorSet = partition.getProcessorSet();
		printF("  grid=%i: cg[grid]: actual-processors=[%i,%i]\n",
		       grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));
	      }

              fflush(0);
              Communication_Manager::Sync();
	      Overture::abort("Error");
	    }
	  }
	}
	

	if( twilightZone )
	{
          // u=exact(cgNew,Range(0,0));
          // This next stuff should be put into a TZ function:
          // exact.gd(u,Range(0,0));
          for( int grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cgNew[grid];
            mg.update(MappedGrid::THEcenter);
	    
            const bool isRectangular=false;  // do this for now
	    
            realArray & ug = u[grid];
            #ifdef USE_PPP
	      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(ug,uLocal);
	      realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
            #else
	      realSerialArray & uLocal = ug;
	      const realSerialArray & xLocal = mg.center();
            #endif
            real t=0.;
            getIndex( mg.dimension(),I1,I2,I3);
	    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);  
	    if( ok )
	    {
              if( xLocal.getDataPointer()==NULL )
	      {
                printf("ERROR: myid=%i, xLocal.getDataPointer()==NULL but uLocal is not NULL !\n");

		if( true )
		{
		  Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
		  const intSerialArray & processorSet = partition.getProcessorSet();
		  printf("  grid=%i: cg[grid]: actual-processors=[%i,%i]\n",
			  grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));
		}
		if( true )
		{
		  // Partitioning_Type & partition = (Partitioning_Type &)u[grid].getPartition();
		  // const intSerialArray & processorSet = partition.getProcessorSet();
                  const Partitioning_Type & uPartition = ug.getPartition();
                  const intSerialArray & processorSet  = ((Partitioning_Type &)uPartition).getProcessorSet();
		  printf("  grid=%i: u[grid]: actual-processors=[%i,%i]\n",
			  grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));
		}
                

		Overture::abort("error");
	      }
	      exact.gd( uLocal,xLocal,cgNew.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,0,t);
	    }
	  }
	}
        else
          errorEstimator.computeFunction( u,ErrorEstimator::topHat,0. );
	// getTrueSolution( u,0.,topHatCentre,topHatVelocity,topHatRadius );
        if( debug & 2 )
	{
	  psp.set(GI_TOP_LABEL,"u");
	  PlotIt::contour(ps,u,psp);
	}

        if( debug & 4 )
	{
	  printF(" ** New grid ** \n");
          printInfo( cgNew );
	}

        currentGrid = (currentGrid+1) % 2;

      }
      
      CompositeGrid & cg =cga[currentGrid];


      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      psp.set(GI_TOP_LABEL,"initial conditions on refined grid");
      
      if( cg.numberOfDimensions()==1 )
      {
        real yLevel= twilightZone ? exact(0.,0.,0.,0,0.) : 0.;
	psp.set(GI_Y_LEVEL_FOR_1D_GRIDS,yLevel);
	PlotIt::plot(ps,cg,psp);
      }
      PlotIt::contour(ps,u,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      realCompositeGridFunction u1(cg),u2,u3;
      u1=0.;

      int step;
      int maximumNumberOfSteps=1000000;
      
      real maxError=0.;
      // real a=1., b=1.;
      if( nu<0. )
      {
	nu=.04/pow(2,numberOfRefinementLevels);
	if( cg.numberOfDimensions()==1 )
	{
	  nu=1.*cg[0].gridSpacing(0)/pow(2,numberOfRefinementLevels);
	}
      }
      real anu=max(fabs(a),fabs(b))*.5;
      anu=0.;
      
      op.updateToMatchGrid(cg);

      interpolant.updateToMatchGrid(cg,1);

      u.setOperators(op);
	
      // =========== compute the time step =================
      real dt=.01, t=0.;

      dt=REAL_MAX;
      real cflr=2., cfli=1.;
      if( timeSteppingMethod==rungeKuttaOrder2 )
      {
        if( cfl<0. ) cfl=.9;
        cflr=2.;  // are these right??
        cfli=1.;  
      }
      else if( timeSteppingMethod==rungeKuttaOrder4 )
      {
        if( cfl<0. ) cfl=.9;
        cflr=2.7; // are these right??
        cfli=2.7;
      }
      else if( timeSteppingMethod==forwardEuler )
      {
        if( cfl<0. ) cfl=.5;
        cflr=2.;  // are these right??
        cfli=1.;  
      }
      

      dt = getTimeStep( cg,op,cfl,cflr,cfli );
      printF("*** dt=%9.3e\n",dt);
      
      if( debug & 2 )
	Overture::printMemoryUsage(sPrintF("amrh after get time step, step=%i",step),stdout);

      aString menu2[]=
      {
	"step",
        "movie mode",
        "go",
        "final time",
        "plot time interval",
        "anu",
        "cfl",
        "plot interval",
        "contour",
        "plot a refinement",
        "grid",
        "plot error",
        "plot regrid error function",
        "plot error points",
        "plot parallel distribution",
        "output a .cmd file",
        "debug",
        "interp debug",
        "error estimator debug",
        "interpolate a refinement",
        "erase",
        "exit",
	"" 
      };

      real tPlot=.1;
      if( regridInterval<0 ) 
        regridInterval=refinementRatio;
      
      int plotInterval=max(4,int(tPlot/dt+.5));
      int movieModePlotInterval=plotInterval;

      bool movieMode=false;
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      
      real cpu0 = getCPU();
      real timeForRegrid=0.;
      real timeForRegridUpdate=0.;
      real timeForSolve=0.;
      real timeForPlotting=0.;
      real timeForGridCopy=0.;
      real timeForInterpolateRefinements=0;
      real timeForUpdate=0.;
      real timeForOgen=0.;
      real timeForInterpolate=0.;
      real timeForErrorEstimator=0.;
      real timeForPCS=0.;
      
      bool computeErrorOnFinestLevel=true;  // compute error on the finest level for plotting
      
      int plotOption=1+2; // plot contour and grid.
      
      for( step=0; step<maximumNumberOfSteps; step++ )
      {
        if( !movieMode || t>tFinal-dt*1.e-4 )
	{
          for( ;; )
	  {
	    ps.getMenuItem(menu2,answer,"choose");
	    if( answer=="contour" )
	    {
              plotOption|=1;
	      
	      ps.erase();
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	      PlotIt::contour(ps,u,psp);
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	    }
	    else if( answer=="grid" )
	    {
              plotOption|= 2;
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	      PlotIt::plot(ps,cga[currentGrid],psp);
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	    }
	    else if( answer=="step" )
	    {
              plotInterval=1;
              break;
	    }
            else if( answer=="final time" )
	    {
              ps.inputString(answer,"Enter the final time");
	      sScanF(answer,"%e",&tFinal);
	      printF("tFinal=%e\n",tFinal);
	    }
            else if( answer=="anu" )
	    {
              ps.inputString(answer,"Enter anu");
	      sScanF(answer,"%e",&anu);
	      printF("anu=%e\n",anu);
	    }
            else if( answer=="cfl" )
	    {
              real cflOld=cfl;
              ps.inputString(answer,"Enter cfl");
	      sScanF(answer,"%e",&cfl);
	      printF("cfl=%e\n",cfl);
              dt*=cfl/cflOld;
	    }
            else if( answer=="plot time interval" )
	    {
              ps.inputString(answer,"Enter tPlot"); 
              sScanF(answer,"%e",&tPlot);
	      plotInterval=max(4,int(tPlot/dt+.5));
	      movieModePlotInterval=plotInterval;
	      printF("tPlot=%e (plotInterval=%i steps)\n",tPlot,plotInterval);
	    }
            else if( answer=="plot interval" )
	    {
              ps.inputString(answer,"Enter plot interval");
	      sScanF(answer,"%i",&plotInterval);
	      printF("plot interval=%i\n",plotInterval);
              movieModePlotInterval=plotInterval;
	    }
	    else if( answer=="movie mode" || answer=="go" )
	    {
              plotInterval=movieModePlotInterval;
	      
	      movieMode=true;
              break;
	    }
            else if( answer=="plot error" )
	    {
              CompositeGrid & cgc = cga[currentGrid];

  	      error.updateToMatchGrid(cgc);
              if( !twilightZone )
	      {
		psp.set(GI_TOP_LABEL,sPrintF(buff,"error estimate at t=%8.2e",t));
                errorEstimator.computeAndSmoothErrorFunction(u,error );
                printF("Error: min=%e, max=%e \n",min(error),max(error));

	      }
              else
	      {
		psp.set(GI_TOP_LABEL,sPrintF(buff,"error at t=%8.2e",t));

	        checkError(u,t,exact,"plot error: ",debugFile,0,&error);
	      }
              ps.erase();
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	      psp.set(GI_TOP_LABEL,sPrintF(buff,"error at t=%8.2e",t));
              PlotIt::contour(ps,error,psp);
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

	    }
            else if( answer=="plot regrid error function" )
	    {
              CompositeGrid & cgc = cga[currentGrid];
         
  	      error.updateToMatchGrid(cgc);

 	      if( computeErrorOnFinestLevel )
   	        errorEstimator.setMaximumNumberOfRefinementLevels(numberOfRefinementLevels+1);

	      if( !twilightZone  || twilightZoneFunction==pulseTZ )
		errorEstimator.computeAndSmoothErrorFunction(u,error );
	      else
	      {
		// With TZ flow, base error estimator on the top-hat function:
		ue.updateToMatchGrid(cgc);
		// getTrueSolution(ue,t,topHatCentre,topHatVelocity,topHatRadius);
		errorEstimator.computeFunction( ue,ErrorEstimator::topHat,t );

		ue.setOperators(*u.getOperators());
		errorEstimator.computeAndSmoothErrorFunction(ue,error );
	      }

              ps.erase();
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	      psp.set(GI_TOP_LABEL,sPrintF(buff,"regrid error at t=%8.2e",t));
              PlotIt::contour(ps,error,psp);
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
              ps.erase();

 	      if( computeErrorOnFinestLevel )
 	        errorEstimator.setMaximumNumberOfRefinementLevels(numberOfRefinementLevels);
	    }
            else if( answer=="plot error points" )
	    {
              CompositeGrid & cgc = cga[currentGrid];

	      if( computeErrorOnFinestLevel )
   	        errorEstimator.setMaximumNumberOfRefinementLevels(numberOfRefinementLevels+1);

  	      error.updateToMatchGrid(cgc);
	      if( !twilightZone || twilightZoneFunction==pulseTZ  )
		errorEstimator.computeAndSmoothErrorFunction(u,error );
	      else
	      {
		// With TZ flow, base error estimator on the top-hat function:
		ue.updateToMatchGrid(cgc);
		// getTrueSolution(ue,t,topHatCentre,topHatVelocity,topHatRadius);
		errorEstimator.computeFunction( ue,ErrorEstimator::topHat,t );

		ue.setOperators(*u.getOperators());
		errorEstimator.computeAndSmoothErrorFunction(ue,error );
	      }

	      if( plotPoints )
		errorEstimator.plotErrorPoints( error, errorThreshold,ps,psp );

 	      if( computeErrorOnFinestLevel )
 	        errorEstimator.setMaximumNumberOfRefinementLevels(numberOfRefinementLevels);
	    }
            else if( answer=="plot a refinement" )
	    {
              CompositeGrid & cgc = cga[currentGrid];
	      realCompositeGridFunction w(cgc);
	      w=1.;
              for( int level=0; level<cgc.numberOfRefinementLevels(); level++ )
	      {
		ps.erase();
		psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
		PlotIt::contour(ps,w.refinementLevel[level],psp);
		psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	      }
	      
	    }
            else if( answer=="plot parallel distribution" )
	    {
              CompositeGrid & cg = cga[currentGrid];

  	      realCompositeGridFunction pd(cg);
	      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	      {
		MappedGrid & mg = cg[grid];
		getIndex(mg.dimension(),I1,I2,I3);

                #ifdef USE_PPP
	  	  realSerialArray pdLocal; getLocalArrayWithGhostBoundaries(pd[grid],pdLocal);
		  bool ok=ParallelUtility::getLocalArrayBounds(pd[grid],pdLocal,I1,I2,I3);
		  if( !ok ) continue;
                #else
		  realSerialArray & pdLocal = pd[grid];
                #endif
		pdLocal=myid;
	      }
	      
              ps.erase();
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	      psp.set(GI_TOP_LABEL,sPrintF(buff,"Parallel distribution at t=%8.2e",t));
              PlotIt::contour(ps,pd,psp);
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

	    }
            else if( answer=="output a .cmd file" )
	    {
               regrid.outputRefinementInfo( cga[currentGrid], gridName,"amrhDebug.cmd" );
	    }
	    else if( answer=="debug" )
	    {
	      ps.inputString(answer,"Enter debug");
	      sScanF(answer,"%i",&debug);
	      printF("set debug=%i\n",debug);

	      regrid.debug=debug;
	      ogen.debug=debug;
	    }
	    else if( answer=="interp debug" )
	    {
	      interp.debug=7;
	    }
	    else if( answer=="error estimator debug" )
	    {
	      errorEstimator.debug=7;
	    }
            else if( answer=="interpolate a refinement" )
	    {
              // display(cg.interpolationStartEndIndex,"cg.interpolationStartEndIndex");
	      
              interpolant.debug=1;
              for( int level=0; level<cg.numberOfRefinementLevels(); level++ )
	      {
		printf("Intepolate level %i...\n",level);
		u.getInterpolant()->interpolateRefinementLevel(level,u);
	      }
	    }
            else if( answer=="erase" )
	    {
	      plotOption=0;
	      ps.erase();
	    }
	    else if( answer=="exit" )
	    {
              finished=true;
	      break;
	    }
            else
	    {
              printF("Unknown command=[%s]\n",(const char*)answer);
	      ps.stopReadingCommandFile();
	    }
	  }
	}
	
        if( finished )
          break;
	
        if( step==0 )
	{
	  // cga[currentGrid].updateParentChildSiblingInfo();
	  if( twilightZone )
	  {
	    maxError=checkError(u,t,exact,"Start of step",debugFile);
	  }

	}
	
        if( (numberOfRefinementLevels>1 && step>0 && (step % regridInterval == 0)) )
	{
          // ****** regrid *************

          if( debug & 1 ) printF(" ***** regrid currentGrid=%i step=%i **********\n",currentGrid,step);
	  real timeStart=getCPU();
	  
          int nextGrid= (currentGrid+1) % 2;

          CompositeGrid & cgc = cga[currentGrid];
          CompositeGrid & cgNew = cga[nextGrid];

          if( destroyBeforeUpdate ) error.destroy(); 
	  error.updateToMatchGrid(cgc);
          error=0.;  // *wdh* 040427

	  if( computeErrorOnFinestLevel )
	    errorEstimator.setMaximumNumberOfRefinementLevels(numberOfRefinementLevels+1);

          real time0=getCPU();
	  if( !twilightZone || twilightZoneFunction==pulseTZ  )
	    errorEstimator.computeAndSmoothErrorFunction(u,error );
	  else
	  {
	    // With TZ flow, base error estimator on the top-hat function:
            if( destroyBeforeUpdate ) ue.destroy(); 
	    ue.updateToMatchGrid(cgc);
	    // getTrueSolution(ue,t,topHatCentre,topHatVelocity,topHatRadius);
            errorEstimator.computeFunction( ue,ErrorEstimator::topHat,t );

            ue.setOperators(*u.getOperators());
	    errorEstimator.computeAndSmoothErrorFunction(ue,error );
	  }

	  if( computeErrorOnFinestLevel )
	    errorEstimator.setMaximumNumberOfRefinementLevels(numberOfRefinementLevels);

          timeForErrorEstimator+=getCPU()-time0;
	  
          if( debug & 4 )
            printF("step=%i, Error estimate: min=%e, max=%e (tol=%8.2e) \n",step,min(error),max(error),errorThreshold);

	  if( debug & 4 )
	  {
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
            psp.set(GI_TOP_LABEL,sPrintF(buff,"error estimate before regrid, t=%8.2e",t));
	    ps.erase();
	    PlotIt::contour(ps,error,psp);
            psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }

	  if( destroyBeforeUpdate )
	  {
            // destroy the grids and gf's associated with cgNew, so we can redistribute in parallel 
            // ***check this****
	    u1.destroy();
	    u2.destroy();
	    u3.destroy();
            ue.destroy(); 
	    
	  }
	  if( destroyGridBeforeRegrid )
	  {
            // cgNew.destroy(); // *********************************
            for( int grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
	    {
	      // if( cgNew.refinementLevelNumber(grid)>0 )
              cgNew[grid].destroy();
	    }
	  }
	  
          time0=getCPU();

	  regrid.regrid(cgc,cgNew, error, errorThreshold, numberOfRefinementLevels-1, baseLevel);

	  numberOfRegrids++;
          timeForRegrid+=getCPU()-time0;
	  
          // display(cgNew.interpolationIsImplicit,"interpolationIsImplicit after regrid");
  
          checkArrayIDs("amrh: after regrid"); // check for possible leaks
          if( debug & 2 )
            Overture::printMemoryUsage(sPrintF("amrh after regrid step=%i",step),stdout);
	  
          if( debug & 2 )
            regrid.printStatistics(cgNew,NULL,&numberOfGridPoints);
	  else
	  {
            GridStatistics::getNumberOfPoints(cgNew,numberOfGridPoints);
	  }
	  
          maxNumberOfGridPoints=max(maxNumberOfGridPoints,numberOfGridPoints);
          averageNumberOfGridPoints+=numberOfGridPoints;
          averageNumberOfGrids+=cgNew.numberOfComponentGrids();
	  maxNumberOfGrids=max(maxNumberOfGrids,cgNew.numberOfComponentGrids());

          if( debug & 2 )
	  {
	    regrid.outputRefinementInfo( cgNew, gridName,"amrhDebug.cmd" );

	    printF("---After regrid:\n");
	    for( int grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
	    {
              const IntegerArray & d = cgNew[grid].dimension();
              printF(" grid=%i : dimension=[%i,%i][%i,%i]\n",grid,d(0,0),d(1,0),d(0,1),d(1,1));
	    }
            cgNew.displayDistribution(sPrintF(buff,"cgNew after regrid, step=%i",step));

            // if( step>=248 ) Overture::abort("stop here for testing");
	  }
	  
// 	  if( step==32 ) 
// 	  {
// 	    ogen.debug=7; // *********************
// 	    // cgNew.interpolationOverlap.display("Before ogen.updateRefinement: interpolationOverlap(grid,grid2)");
//             printF(" Before ogen.updateRefinement:\n");
//             for( int grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
// 	      for( int grid2=0; grid2<cgNew.numberOfComponentGrids(); grid2++ )
// 	      {
//                 printF(" grid=%i grid2=%i interpolationOverlap=%5.2f\n",grid,grid2,
//                        cgNew.interpolationOverlap(0,grid,grid2,0));
// 	      }
// 	  }
	  

          time0=getCPU();
          ogen.updateRefinement(cgNew);
          timeForOgen+=getCPU()-time0;
          if( debug & 1 ) printF("step=%i, time for ogen.updateRefinement = %8.2e\n",step,getCPU()-time0);
	  
          #ifndef USE_PPP
          if( debug & 1 )
	  {
            int numErrors=Ogen::checkUpdateRefinement( cg );  // this doesn't work in parallel
            printF(" checkUpdateRefinement: number of errors=%i\n",numErrors);
	  }
          #endif


// 	  if( step==32 ) ogen.debug=0; // *********************


          if( debug & 4 )
	  {
	    int numberOfErrors=checkOverlappingGrid(cgNew);
	    if( numberOfErrors==0 )
	      printF("step=%i: Overlapping grid is valid.\n",step);
	    else
	    {
	      printF("Checking validity of the overlapping grid, Grid is not valid! Number of errors=%i\n",
		     numberOfErrors);
	      regrid.outputRefinementInfo( cgNew, gridName,"amrhDebug.cmd" );
	      ps.stopReadingCommandFile();
	    }
	    
	  }
	  
          time0=getCPU();

	  // cgNew.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
	  for( grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
	  {
	    if( cgNew[grid].isRectangular() )
	      cgNew[grid].update(MappedGrid::THEmask);
	    else
	      cgNew[grid].update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter);
	  }
 
	  if( false )
	  {
	    cgNew.displayDistribution(sPrintF(buff,"cgNew after regrid,  step=%i",step));
	    
            fflush(0);
    	    Communication_Manager::Sync();

	    CompositeGrid & cg = cgNew;
	    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	    {
	      intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);

	      testConsistency(cg[grid].mask(),"cgNew: mask");
	      if( grid==1 )
	      {
		Partitioning_Type & partition = (Partitioning_Type &)cg[grid].rcData->partition;
		const intSerialArray & processorSet = partition.getProcessorSet();

		printf("cgNew:  myid=%i grid=%i maskLocal: [%i,%i][%i,%i][%i,%i] proc=[%i,%i] cg[grid].rcData=%p\n",
                       myid,grid,
                       maskLocal.getBase(0),maskLocal.getBound(0),
                       maskLocal.getBase(1),maskLocal.getBound(1),
                       maskLocal.getBase(2),maskLocal.getBound(2),
                       processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)),
                       cg[grid].rcData);
		fflush(0);
		Communication_Manager::Sync();
	      }
	    }
	  }
	  

//           ListOfParentChildSiblingInfo listOfPCSInfo;
//           ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( cgNew, listOfPCSInfo );

          if( false )
	  {
            cgNew.updateParentChildSiblingInfo();
	    ListOfParentChildSiblingInfo & listOfPCSInfo = *cgNew.getParentChildSiblingInfo();
	  
	    for( int grid=0; grid<listOfPCSInfo.getLength(); grid++ )
	    {
	      cout << "\n *****grid " << grid << ": PCSInfo:\n";
	      cout << listOfPCSInfo[grid];
	    }
//             aString answer;
// 	    cout << "enter a char to continue\n";
// 	    cin >> answer;

	    timeForPCS+=getCPU()-time0;
	  }
	  

          if( destroyBeforeUpdate ) u1.destroy();  

          u1.updateToMatchGrid(cgNew);
	  if( false )
	  {
	    for( int grid=0; grid<cgNew.numberOfComponentGrids(); grid++ )
	    {
              if( grid==1 )
                printf("After u1.update, myid=%i, grid=%i u1[grid].grid=%p, cgNew[grid].rcData=%p\n",
                       myid,grid,u1[grid].grid,cgNew[grid].rcData);
	      
	      if( u1[grid].grid != cgNew[grid].rcData )
	      {
		printf("After u1.updateToMatchGrid(cgNew):ERROR:myid=%i u[grid].grid != cg[grid].rcData!! grid=%i\n",
		       myid,grid);
                Overture::abort("error");
	      }
	    }
	  }
	  
	  
	  u1=-1.;

	  if( true && refinementRatio >2 )
	  { // is this needed for sis with rf=4? ---> YES
	    u.applyBoundaryCondition(0,BCTypes::extrapolateInterpolationNeighbours);
	  }
          timeForUpdate+=getCPU()-time0;

          if( debug & 4 )
	  {
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
  	    psp.set(GI_TOP_LABEL,"after regrid");
            ps.erase();
            PlotIt::plot(ps,cgNew,psp);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }

	  
          time0=getCPU();
          if( debug & 2 )
            printF("interpolate new refinements from old ...\n");

	  if( true )
	  { // check the grid for validity of the mask
            checkParallelConsistency(u,"Before interpRef: u");
            checkParallelConsistency(u1,"Before interpRef: u1");
	  }

          // this will interp interior points and call interpolateRefinementBoundaries
 	  interp.interpolateRefinements( u,u1 );
          timeForInterpolateRefinements+=getCPU()-time0;
 	  
	  // printF(" after regrid: cgNew.numberOfComponentGrids=%i \n",cgNew.numberOfComponentGrids());
          // printInfo( cgNew );
          if( debug & 2 )
	  {
            real err = checkError(u1,t,exact,sPrintF("step %i after regrid: interpolate new refinements from old",
                                 step),debugFile,debug);
	  }
	  

          time0=getCPU();
	  op.updateToMatchGrid(cgNew);

          if( true ) 
   	    interpolant.updateToMatchGrid(cgNew,1);

          if( destroyBeforeUpdate ) u.destroy();
          u.updateToMatchGrid(cgNew);
	  u.dataCopy(u1);  // ***************** 060308 
	  // u=u1; 
          u.setOperators(op);
        
          timeForUpdate+=getCPU()-time0;
	  
          time0=getCPU();
	  applyBoundaryConditions(u,t);

          timeForInterpolate+=getCPU()-time0;
	  
	  if( debug & 2 )
	  {
	    error.updateToMatchGrid(cgNew);
            real err = checkError(u,t,exact,sPrintF("step %i after regrid: after apply BC:",step),
                       debugFile,debug,&error);

            if( false )
	    {
              // cgNew.interpolationOverlap.display("interpolationOverlap(grid,grid2)");
	      // cgNew.interpolationIsImplicit.display("interpolationIsImplicit(grid,grid2)");
	      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	      psp.set(GI_TOP_LABEL,sPrintF(buff,"Error after regrid, t=%8.2e",t));
	      ps.erase();
	      PlotIt::contour(ps,error,psp);
	      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	    }
	    
	  }

          if( false )
	  {
            psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	    psp.set(GI_TOP_LABEL,sPrintF(buff,"After regrid. u1 t=%8.2e",t));
	    ps.erase();
	    PlotIt::contour(ps,u1,psp);
	    psp.set(GI_TOP_LABEL,sPrintF(buff,"After regrid. u t=%8.2e",t));
	    ps.erase();
	    PlotIt::contour(ps,u,psp);
            psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }
	  
	  if( twilightZone && debug & 4)
	  {
            real err = checkError(u,t,exact,"After regrid: ");
            if( err > 1.e-4 && twilightZoneFunction!=pulseTZ )
	    {
              regrid.outputRefinementInfo( cgNew, gridName,"amrhDebug.new.cmd" );
	      
	      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

  	      error.updateToMatchGrid(cgNew);
	      psp.set(GI_TOP_LABEL,sPrintF(buff,"error at t=%8.2e",t));
	      error=u-exact(cgNew,0,t);

              ps.erase();
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	      psp.set(GI_TOP_LABEL,sPrintF(buff,"error at t=%8.2e",t));
              PlotIt::contour(ps,error,psp);
              psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

	    }
	    
	  }

          if( debug & 1 ) printF(" old dt=%9.3e, ",dt);
          dt = getTimeStep( cgNew,op,cfl,cflr,cfli );
          if( debug & 1 ) printF("new dt=%9.3e\n",dt);

          currentGrid = (currentGrid+1) % 2;

          timeForRegridUpdate+=getCPU()-timeStart;
	  
	}  // end regrid 
	
           
        real dtSave=dt;
        if( t+dt > tFinal ) // adjust last time step to reach tFinal
	{
	  dt=tFinal-t;
	}
	

        CompositeGrid & cgc = cga[currentGrid];

	if( (step==0 || numberOfRefinementLevels>1) && (step % regridInterval == 0) )
	{
          // printF("=============== step=%i regridInterval=%i update u1,u2,u3,... ===========\n",step,regridInterval);
	  
          real time0=getCPU();
	  if( timeSteppingMethod==rungeKuttaOrder2 )
	  {
            if( destroyBeforeUpdate ) u2.destroy(); 
	    u2.updateToMatchGrid(cgc);  u2=0.;
	    for( int grid=0; grid<cgc.numberOfComponentGrids(); grid++)
	    {
	      if(!hasSameDistribution(u1[grid],u2[grid]) )
	      {
		printf("amrh:update u2:ERROR: u1,u2 (grid=%i) do NOT have the same parallel distribution!\n",grid);
   	        testConsistency(u2[grid],"amrh:update u2");
		Overture::abort("error");
	      }
	    }
	    

	    u1.setOperators(*u.getOperators());
	    u2.setOperators(*u.getOperators());
	  }
	  else if( timeSteppingMethod==rungeKuttaOrder4 )
	  {
	    u2.updateToMatchGrid(cgc); u2=0.;
	    u3.updateToMatchGrid(cgc); u3=0.;
	    u1.setOperators(*u.getOperators());
	    u2.setOperators(*u.getOperators());
	    u3.setOperators(*u.getOperators());
	  }
	  else if( timeSteppingMethod==forwardEuler )
	  { 
	    u1.setOperators(*u.getOperators());
	  }
	  timeForUpdate+=getCPU()-time0;
          timeForRegridUpdate+=getCPU()-time0;

	}
	
	if( debug & 4 )
	{
	  cgc.displayDistribution(sPrintF(buff,"cgc=cga[%i] at start of step=%i",currentGrid,step));
          ps.erase();
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid cgc at start of step, t=%8.2e",t));
	  PlotIt::plot(ps,cgc,psp);
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	}

	if( debug & 2 )
	  Overture::printMemoryUsage(sPrintF("amrh before RK steps, step=%i",step),stdout);

        real time=getCPU();
	

	if( timeSteppingMethod==rungeKuttaOrder2 )
	{
	  rungeKutta2( t,dt,u,u1,u2 );
	}
	else if( timeSteppingMethod==rungeKuttaOrder4 )
	{
	  rungeKutta4( t,dt,u,u1,u2,u3 );
	}
	else if( timeSteppingMethod==forwardEuler )
	{ 
	  // forward euler:
	  rungeKutta1( t,dt,u,u1 );
	}
	else
	{
	  Overture::abort("error");
	}
	
        dt=dtSave;
	
	timeForSolve+=getCPU()-time;

        checkArrayIDs(sPrintF("amrh: after time step %i t=%8.2e",step,t)); // check for possible leaks

	if( debug &4 )
	{
	  cgc.displayDistribution(sPrintF(buff,"cgc (2) step=%i, before check err",step));
          ps.erase();
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid cgc before check err, t=%8.2e",t));
	  PlotIt::plot(ps,cgc,psp);
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	}

	
	if( debug &4 )
	{
	  cgc.displayDistribution(sPrintF(buff,"cgc (2) step=%i, AFTER check err",step));
          ps.erase();
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid cgc AFTER check err, t=%8.2e",t));
	  PlotIt::plot(ps,cgc,psp);
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	}


        if( ((step % plotInterval) == 0) || ( t > tFinal-.5*dt ) )
	{
          if( debug & 2 )
	  {
            Overture::printMemoryUsage(sPrintF(">>>amrh step=%i",step),stdout);
	  }
	  
          real maxMem=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());
	  
          printF(">>> step=%i, t=%8.2e, dt=%8.2e, grids=%i, cpu=%8.2e, timeForSolve=%8.2e, timeForRegrid=%8.2e"
                  " max-mem=%g (Mb)\n",
		 step,t,dt,cgc.numberOfComponentGrids(),getCPU()-cpu0,timeForSolve,timeForRegrid,maxMem);

	  maxError=checkError(u,t,exact,sPrintF(">>> step=%i: ",step),debugFile,debug);


          time=getCPU();
	  psp.set(GI_TOP_LABEL,sPrintF(buff,"u t=%8.2e, dt=%8.2e, nu=%8.2e, anu=%8.2e",t,dt,nu,anu));
	  ps.erase();
	  if( cgc.numberOfDimensions()==1 )
	  {
            real yLevel= twilightZone ? exact(0.,0.,0.,0,t) : 0.;
	    psp.set(GI_Y_LEVEL_FOR_1D_GRIDS,yLevel);
	  }
	  
          if( plotOption & 2 )
	    PlotIt::plot(ps,cgc,psp);
          if( plotOption & 1 )
	    PlotIt::contour(ps,u,psp);
	  ps.redraw(movieMode);
          timeForPlotting+=getCPU()-time;
	}
	else if( twilightZone && debug & 2 )
	{
	  maxError=checkError(u,t,exact,"After time step: ",debugFile,debug);
	}

	
      } // end for step
      

      if( numberOfRefinementLevels==1 )
      {
	regrid.printStatistics(cga[currentGrid],NULL,&numberOfGridPoints);
	maxNumberOfGridPoints=numberOfGridPoints;
	averageNumberOfGridPoints=numberOfGridPoints;
	numberOfRegrids=1;
      }

      averageNumberOfGridPoints/=max(1,numberOfRegrids);
      averageNumberOfGrids/=max(1,numberOfRegrids);
      
      real totalTime=max(REAL_MIN*100.,getCPU()-cpu0);

      totalTime=ParallelUtility::getMaxValue(totalTime);
      timeForDudt=ParallelUtility::getMaxValue(timeForDudt);
      timeForRegrid=ParallelUtility::getMaxValue(timeForRegrid);
      timeForRegridUpdate=ParallelUtility::getMaxValue(timeForRegridUpdate);
      timeForSolve=ParallelUtility::getMaxValue(timeForSolve);
      timeForPlotting=ParallelUtility::getMaxValue(timeForPlotting);
      timeForGridCopy=ParallelUtility::getMaxValue(timeForGridCopy);
      timeForInterpolateRefinements=ParallelUtility::getMaxValue(timeForInterpolateRefinements);
      timeForUpdate=ParallelUtility::getMaxValue(timeForUpdate);
      timeForInterpolate=ParallelUtility::getMaxValue(timeForInterpolate);
      timeForErrorEstimator=ParallelUtility::getMaxValue(timeForErrorEstimator);
      timeForPCS=ParallelUtility::getMaxValue(timeForPCS);
      timeForOgen=ParallelUtility::getMaxValue(timeForOgen);
      aString methodName;
      methodName = timeSteppingMethod==forwardEuler ? "forward Euler" : timeSteppingMethod==rungeKuttaOrder2 ?
	"2nd order Runge-Kutta" : "4th order Runge-Kutta";

      const int numProc= max(1,Communication_Manager::numberOfProcessors());
      fPrintF(checkFile,
             " %8.1e %9.2e %i %9.1f %9.2e %i (t, error, gridPoints, cpu, errorThreshold, numBuffer)\n"
             "=======================================================================================\n"
             " grid=%s, numberOfSteps=%i, levels=%i, refine ratio=%i, regrid interval=%i, processors=%i\n"
             " time stepping method = %s\n"
             " error threshold =%9.3e, number of buffer zones=%i   \n"
             " error at t=%8.1e is %8.1e   \n"
             "=======================================================================================\n",
              t,maxError,averageNumberOfGridPoints,totalTime-timeForPlotting,errorThreshold,numberOfBufferZones,
	      (const char*)gridName,step,numberOfRefinementLevels,refinementRatio,regridInterval,numProc,
	      (const char *)methodName,errorThreshold,numberOfBufferZones,t,maxError);
      
      real maxMem=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());
      for( int io=0; io<=1; io++ )
      {
	FILE *file = io==0 ? checkFile : stdout;

        fPrintF(file,
             "=======================================================================================\n"
             " grid=%s, numberOfSteps=%i, levels=%i, refine ratio=%i, regrid interval=%i, processors=%i \n"
             " time stepping method = %s\n"
             " error threshold =%9.3e, number of buffer zones=%i, efficiency=%5.2f  \n"
             " numberOfGridPoints: %i (ave) %i (max)  max-mem-usage=%g (Mb)      \n"
		" numberOfGrids (%i,%i,%i) (min,ave,max)\n",
             (const char*)gridName,step,numberOfRefinementLevels,refinementRatio,regridInterval,numProc,
		(const char *)methodName,errorThreshold,numberOfBufferZones,efficiency,
		averageNumberOfGridPoints,maxNumberOfGridPoints,maxMem,
		minNumberOfGrids,averageNumberOfGrids,maxNumberOfGrids);

        
        fPrintF(file,
		" twilightZone = %s",
                (twilightZoneFunction==polyTZ ? "poly" : twilightZoneFunction==trigTZ ? "trig" : "pulse"));
	if( twilightZoneFunction==polyTZ )
	  fPrintF(file,", degreeSpace=%i degreeTime=%i\n",degreeOfSpacePolynomial,degreeOfTimePolynomial);
        else if( twilightZoneFunction==trigTZ )
	  fPrintF(file,"\n");
	else
          fPrintF(file,"\n");
	
	if( useTopHatForErrorEstimator ) 
	  fPrintF(file," use top hat for error estimator\n");

        fPrintF(file,
             "                                        cpu   cpu/step     %%\n"
             " timeStep............................%8.2e  %8.2e  %5.1f\n"
             "     dudt............................%8.2e  %8.2e (%5.1f)\n"
             "     interpolate.....................%8.2e  %8.2e (%5.1f)\n"
             "     boundary conditions.............%8.2e  %8.2e (%5.1f)\n"
             " regrid and update...................%8.2e  %8.2e  %5.1f\n"
             "     regrid only.....................%8.2e  %8.2e (%5.1f)\n"
             "     grid copy.......................%8.2e  %8.2e (%5.1f)\n"
             "     interpolate refinements.........%8.2e  %8.2e (%5.1f)\n"
             "     update grid and operators.......%8.2e  %8.2e (%5.1f)\n"
             "     time for ogen.updateRefinement..%8.2e  %8.2e (%5.1f)\n"
             "     interpolate.....................%8.2e  %8.2e (%5.1f)\n"
             "     error estimator.................%8.2e  %8.2e (%5.1f)\n"
             "     compute parent/child/sibling....%8.2e  %8.2e (%5.1f)\n"
             " plotting............................%8.2e  %8.2e  %5.1f\n"
             " total (without plotting)............%8.2e  %8.2e (%5.1f)\n"
             " total...............................%8.2e  %8.2e  %5.1f\n"
             "=======================================================================================\n",
             timeForSolve,timeForSolve/step,100.*timeForSolve/totalTime,
             timeForDudt,timeForDudt/step,100.*timeForDudt/totalTime,
             timeForSolveInterpolate,timeForSolveInterpolate/step,100.*timeForSolveInterpolate/totalTime,
             timeForBoundaryConditions,timeForBoundaryConditions/step,100.*timeForBoundaryConditions/totalTime,
             timeForRegridUpdate,timeForRegridUpdate/step,100.*timeForRegridUpdate/totalTime,
             timeForRegrid,timeForRegrid/step,100.*timeForRegrid/totalTime,
             timeForGridCopy,timeForGridCopy/step,100.*timeForGridCopy/totalTime,
             timeForInterpolateRefinements,timeForInterpolateRefinements/step,100.*timeForInterpolateRefinements/totalTime,
             timeForUpdate,timeForUpdate/step,100.*timeForUpdate/totalTime,
             timeForOgen,timeForOgen/step,100.*timeForOgen/totalTime,
             timeForInterpolate,timeForInterpolate/step,100.*timeForInterpolate/totalTime,
             timeForErrorEstimator,timeForErrorEstimator/step,100.*timeForErrorEstimator/totalTime,
             timeForPCS,timeForPCS/step,100.*timeForPCS/totalTime,
             timeForPlotting,timeForPlotting/step,100.*timeForPlotting/totalTime,
             totalTime-timeForPlotting,(totalTime-timeForPlotting)/step,
                                     100.*(totalTime-timeForPlotting)/totalTime,
	     totalTime,totalTime/step,100.);

	interp.printStatistics(file);
        regrid.printStatistics(cga[currentGrid],file,&numberOfGridPoints);

        regrid.getLoadBalancer().printStatistics(file);

	if( false )
	{
	  for( int grid=0; grid<cga[currentGrid].numberOfComponentGrids(); grid++ )
	    cga[currentGrid][grid].displayComputedGeometry();
	}

        // print results formatted to go in a table 

        // %    grid       levels   ratio  NP  loadBalance  steps regrids  grids     max-error
        // %    square40     3        2     2    random      200    50    (1,8,10)     1.e-14 

        aString lbName="all-to-all";
        if( regrid.loadBalancingIsOn() )
	{
	  lbName=regrid.getLoadBalancer().getLoadBalancerTypeName();
	  if( lbName=="sequentialAssignment" ) lbName="sequential";
	  else if( lbName=="randomAssignment" ) lbName="random";
	}
      
        aString gName=gridName;
	int lastChar = gName.length()-1;  
	if( lastChar>3 && gName(lastChar-3,lastChar)==".hdf"  )
	  gName = gName(0,lastChar-4);  // remove ".hdf"
        fPrintF(file,
		"grid         levels   ratio   NP  loadBalance  steps  regrids  grids(min,ave,max)  "
		"  grid-pts(ave,max)  max-error\n"
		"%s      &    %i   &  %i   &  %i  & %s        & %i  & %i    & (%i,%i,%i) & (%7.1e,%7.1e) & %8.2e \\\\\n",
		(const char*)gName,numberOfRefinementLevels,refinementRatio,
		np,(const char*)lbName,step,numberOfRegrids,minNumberOfGrids,
		averageNumberOfGrids,maxNumberOfGrids,
		real(averageNumberOfGridPoints+.5),real(maxNumberOfGridPoints+.5),
		maxError);
      }
      
    }
    else
    {
      printF("unknown response\n");
      ps.stopReadingCommandFile();
    }

    if( finished ) 
      break;
    
  }
  
  
  
  Overture::finish();          
  return 0;
}


