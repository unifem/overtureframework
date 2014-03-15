// **************************************************************************************************
// ******************** Test the InterpolatePoints Class ********************************************
// **************************************************************************************************

// Examples
//
// ++++++++++++++ test interpolating points that are outside the grid: ++++++++++++++
// mpirun -np 1 -all-local testip -in=valvee2.order2 -out=valvee -noplot -testFindNearest=1
// testip -in=joukowsky2de2.order2.hdf -out=square10 -testFindNearest=1  (try pt (-.4 .05 0.))
// ++++++++++++++++++++++++++++++++++
//
// testip -in=square20.hdf -out=square10.hdf
// testip -in=cice3.order2.hdf -out=cice2.order2.hdf -tz=trig
// 
// testip -tz=trig -testNew=0
//   cicStuff has a refinement patch 
// testip -in=cicStuffe3.order2.hdf -out=cice2.order2.hdf -tz=trig
//
// testip -noplot -in=square10 -out=square5 -infoLevel=3     NOTE: 2nd-line of ghost points are outside bounding-box of square10 
//
// testip -noplot -in=square10 -out=square8 -infoLevel=3 -degree=1
// testip -in=box10 -out=box8 -infoLevel=3 -degree=1
// testip -in=box10 -out=box5 -infoLevel=3 -degree=1
// testip -in=box64 -out=box100
// 
// testip -in=twoPipesi2.order2.hdf -out=twoPipesi1.5.order2.hdf -testOld=0 -degree=1
// testip -in=twoPipesi2.5.order2.hdf -out=twoPipesi1.5.order2.hdf -testOld=0 -degree=1
// 
// testip -in=twoPipese3.order2.hdf -out=twoPipese2.order2.hdf -testOld=0 -degree=1
// 
// testip -in=sibe3.order2.hdf -out=sibe2.order2.hdf
// testip -in=sibe3.order2.hdf -out=sibe2.order2.hdf -surf
// 
// testip -in=square128 -out=square40 -tz=trig
// 
// testip -noplot -in=/home/henshaw.0/runs/cgcns/quarterSphere1el2r4.show -out=/home/henshaw.0/runs/cgcns/quarterSphere1el2r2.show
//
// --- NEW InterpolatePointsOnAGrid (serial/parallel) version  ----
// mpirun -np 1 testip -in=square5 -out=square5 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=1 -debug=3
// mpirun -np 1 testip -in=square5 -out=square5 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0
// mpirun -np 1 testip -in=sise1.order2.hdf -out=sise2.order2.hdf -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1
// mpirun -np 2 testip -in=sise2.order2.hdf -out=sise1.order2.hdf -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0
// mpirun -np 1 testip -in=sise1.order2.hdf -out=sise2.order2.hdf -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=1
// mpirun -np 2 testip -in=sise1.order2.hdf -out=sise2.order2.hdf -iw=3 -noplot  -testOld=0 -testNew=0 -testParallel=1 -testPoints=1
//
// mpirun -np 1 testip -in=square20.hdf -out=square10.hdf
// mpirun -np 1 testip -in=cice3.order2 -out=cice2.order2 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0
// mpirun -np 4 testip -in=valvee -out=valvee2.order2 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1 -debug=3
// mpirun -np 1 testip -in=box10 -out=box20 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1 -debug=0
// mpirun -np 1 testip -in=rotatedBox2.order2 -out=rotatedBox4.order2 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1 -debug=0
// mpirun -np 4 testip -in=sibe1.order2 -out=sibe2.order2 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1 
// mpirun -np 1 testip -in=orthoSphere1.order2 -out=orthoSphere2.order2 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1 -debug=0
// mpirun -np 1 testip -in=quarterSphere1e -out=quarterSphere2e -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1 -debug=0
//
// 
// ** pts not assigned: 
//  mpirun -np 1 testip -in=cice1.order2 -out=square5 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=1 -debug=3
//  mpirun -np 1 testip -in=valve -out=square5 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=1 -debug=3
// mpirun -np 1 testip -in=valvee2.order2 -out=valvee -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1
// 
// srun -N1 -n1 -ppdebug testip -in=sise1.order2.hdf -out=sise2.order2.hdf -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=1
// srun -N1 -n1 -ppdebug testip -in=sise1.order2.hdf -out=sise2.order2.hdf -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=1
// totalview srun -a 
// srun -N1 -n8 -ppdebug memcheck_all testip -in=valvee -out=valvee2.order2 -iw=3 -noplot -testOld=0 -testNew=0 -testParallel=1 -testPoints=0 -numGhostToUse=1 -debug=3

#include "Overture.h"
#include "PlotStuff.h"
#include "interpPoints.h"
#include "display.h"
#include "InterpolatePoints.h"
#include "UnstructuredMapping.h"
#include "SurfaceStitcher.h"
#include "BodyDefinition.h"
#include "gridFunctionNorms.h"
#include "ParallelUtility.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"

// *new* parallel version 090814
#include "InterpolatePointsOnAGrid.h"

// *new* 100321: 
int
findNearestValidGridPoint( CompositeGrid & cg, const RealArray & x, IntegerArray & il, RealArray & ci );


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



void
plotResults( PlotStuff & ps, realCompositeGridFunction & u, realCompositeGridFunction & err,
             CompositeGrid & cgDonor,  realCompositeGridFunction & uDonor )
// ==============================================================================================
// Plot results from 
// ==============================================================================================
{
      
  GraphicsParameters psp;

  aString answer;
  aString menu[]=
  {
    "solution",
    "error",
    "grid",
    "donor solution",
    "donor grid",
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
     ps.erase();
      psp.set(GI_TOP_LABEL,"Solution"); 
      PlotIt::contour(ps,u,psp);
    }
    else if( answer=="error" )
    {
      ps.erase();
      psp.set(GI_TOP_LABEL,"error"); 
      PlotIt::contour(ps,err,psp);
    }
    else if( answer=="grid" )
    {
      ps.erase();
      psp.set(GI_TOP_LABEL,"grid"); 
      PlotIt::plot(ps,*u.getCompositeGrid(),psp);
    }
    else if( answer=="donor solution" )
    {
     ps.erase();
      psp.set(GI_TOP_LABEL,"Donor solution"); 
      PlotIt::contour(ps,uDonor,psp);
    }
    else if( answer=="donor grid" )
    {
      ps.erase();
      psp.set(GI_TOP_LABEL,"donor grid"); 
      PlotIt::plot(ps,cgDonor,psp);
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
      
  }

}


int 
computeTheError( CompositeGrid & cg, realCompositeGridFunction & u,
		 realCompositeGridFunction & err, OGFunction & exact, real & error ) 
// ================================================================================================
//
//  Compute the error in the solution.
//

// ================================================================================================
{
  int debug =0;
  const int numberOfDimensions = cg.numberOfDimensions();
  
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

      if( true )
      {
        int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3) 
	{
	  if( errLocal(i1,i2,i3)>.1 )
	  {
	    printF(" *** grid=%i (%s) i=(%i,%i,%i) x=(%9.3e,%9.3e,%9.3e) u=%9.3e true=%9.3e err=%e  \n",
                   grid,(const char*)cg[grid].getName(),
		   i1,i2,i3,xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1),
                      (numberOfDimensions==2 ? 0. : xLocal(i1,i2,i3,2)),
                   uLocal(i1,i2,i3),ue(i1,i2,i3),errLocal(i1,i2,i3));
	  }
	}
      }

    }
    gridErr=ParallelUtility::getMaxValue(gridErr); // max value over all procs
    gridErrWithGhost=ParallelUtility::getMaxValue(gridErrWithGhost); // max value over all procs

    error=max(error, gridErr );
    errorWithGhostPoints=max(errorWithGhostPoints, gridErrWithGhost);

    printF(" grid=%i (%s) max. rel. err=%e (%e with ghost)\n",grid,(const char*)cg[grid].getName(),
	   gridErr,gridErrWithGhost);

   

    if( debug & 8 )
    {
      display(u[grid],"solution u");
      display(err[grid],"abs(error on indexRange +1)");
      // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
    }
  }
  printF("Maximum relative error = %e (%e with ghost)\n",error,errorWithGhostPoints);  

  return 0;
}


//================================================================================
//
//  Test the interpolatePoints function
//
//================================================================================
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();

  aString inFile="cice2.order2.hdf", outFile="cice4.order2.hdf";

  bool interpolateSurface=false;

  printF("Usage: testip -noplot -in=file1 -out=file2 -surf -tz=[poly|trig] -iw=<num> -degree=<> -debug=<> \n"
         "             -testNew=[0|1] -testOld=[0|1] -infoLevel=<> -numGhost=<> -numGhostToUse=<> \n"
	 "      -surf : interpolate a surface CompositeGrid\n");

  int twilightZoneOption=0;
  int degreeOfSpacePolynomial = 2;
  real fx=1., fy=1., fz=1.; // frequencies for trig TZ

  int testOld=true, testNew=true, testParallel=false, testPoints=false, testFindNearest=false;
  #ifdef USE_PPP
   testOld=false, testNew=false, testParallel=true, testPoints=false;
  #endif
  int infoLevel=1;
  int numGhost=1;       // number of ghost points to interpolate on the target grid function
  int numGhostToUse=0;  // number of ghost points we can use when interpolating from the donor grid function
  
  int interpolationWidth=2;
  int debug=1;

  int plot=true;
  aString line;
  int len=0;
  for( int i=1; i<argc; i++ )
  {
    line=argv[i];
    if( line=="-noplot" )
    {
      plot=false;
    }
    else if( len=line.matches("-in=") )
    {
      inFile=line(len,line.length()-1);
    }
    else if( len=line.matches("-out=") )
    {
      outFile=line(len,line.length()-1);
    }
    else if( len=line.matches("-infoLevel=") )
    {
      sScanF(line(len,line.length()-1),"%i",&infoLevel);
    }
    else if( len=line.matches("-debug=") )
    {
      sScanF(line(len,line.length()-1),"%i",&debug);
    }
    else if( len=line.matches("-iw=") )
    {
      sScanF(line(len,line.length()-1),"%i",&interpolationWidth);
    }
    else if( len=line.matches("-testOld=") )
    {
      sScanF(line(len,line.length()-1),"%i",&testOld);
    }
    else if( len=line.matches("-testNew=") )
    {
      sScanF(line(len,line.length()-1),"%i",&testNew);
    }
    else if( len=line.matches("-testParallel=") )
    {
      sScanF(line(len,line.length()-1),"%i",&testParallel);
    }
    else if( len=line.matches("-testFindNearest=") )
    {
      sScanF(line(len,line.length()-1),"%i",&testFindNearest);
      if( testFindNearest )
      {
	testOld=0;
	testNew=0;
	testParallel=0;
      }
    }
    else if( len=line.matches("-testPoints=") )
    {
      sScanF(line(len,line.length()-1),"%i",&testPoints);
    }
    else if( len=line.matches("-degree=") )
    {
      sScanF(line(len,line.length()-1),"%i",&degreeOfSpacePolynomial);
    }
    else if( len=line.matches("-numGhost=") )
    {
      sScanF(line(len,line.length()-1),"%i",&numGhost);
    }
    else if( len=line.matches("-numGhostToUse=") )
    {
      sScanF(line(len,line.length()-1),"%i",&numGhostToUse);
    }
    else if( line=="-surf" )
    {
      interpolateSurface=true;
      printF(" *** interpolateSurface=true ****\n");
    }
    else if( line=="-tz=poly" )
    {
      twilightZoneOption=0;
    }
    else if( line=="-tz=trig" )
    {
      twilightZoneOption=1;
    }
     
  }

  Range all;                  // a null Range is used when constructing grid functions, it indicates
                              // the positions of the coordinate axes

  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    const int numParallelGhost=2;
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numParallelGhost);
  #endif

  InterpolatePoints::debug=debug;
  InterpolatePointsOnAGrid::debug=debug;
    

  if( !interpolateSurface )
  {
    // *******************************************
    // ****** interpolate volume grids ***********
    // *******************************************


    CompositeGrid cg;
    getFromADataBase(cg,inFile);
    cg.update( MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex );
    // cg.update(MappedGrid::THEinverseVertexDerivative);


    CompositeGrid cg2;
    getFromADataBase(cg2,outFile);
    cg2.update( MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex );
    // cg2.update(MappedGrid::THEinverseVertexDerivative);

    const int numberOfDimensions=cg.numberOfDimensions();

    // create a twilight-zone function for checking the errors
    OGFunction *exactPointer;
    if( twilightZoneOption==1 )
    {
      printF("TwilightZone: trigonometric polynomial, fx=%9.3e, fy=%9.3e, fz=%9.3e\n",fx,fy,fz);
      exactPointer = new OGTrigFunction(fx,fy,fz); 
    }
    else
    {
      printF("TwilightZone: algebraic polynomial\n");
      // cg.changeInterpolationWidth(2);

      int degreeOfTimePolynomial = 1;
      int numberOfComponents = cg.numberOfDimensions();
      exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					degreeOfTimePolynomial);
    
      
    }
    OGFunction & exact = *exactPointer;

    printF("\n*** Interpolate points on %s from %s ***\n",(const char*)outFile,(const char*)inFile);
    if( twilightZoneOption==0 )
    {
      printF("          Polynomial Exact Solution, degree=%i \n",degreeOfSpacePolynomial);
    }
    else
    {
      printF("          Trigonometric Exact Solution \n");
    }
    
    // Range R=Range(0,cg.numberOfDimensions()-1);
    Range R=Range(0,0);
    realCompositeGridFunction u(cg,all,all,all,R);

    Index I1,I2,I3;
    

    exact.assignGridFunction(u);

    // Assign bogus values to unused points to make sure they are note used 
    // when interpolating
    if( true )
    {
      const real bogusValue=-9999.;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++)
      {
	MappedGrid & mg = cg[grid];
#ifdef USE_PPP
	realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);
#else
	realSerialArray & uLocal = u[grid]; 
	const intSerialArray & maskLocal = cg[grid].mask();
#endif

	// getIndex(mg.dimension(),I1,I2,I3);
	getIndex(mg.gridIndexRange(),I1,I2,I3);
	int includeGhost=1; // include parallel ghost pts in uLocal
	bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);

	if( ok )
	{
	  where( maskLocal(I1,I2,I3)==0 )
	  {
	    uLocal(I1,I2,I3,R)=bogusValue;
	  }
	}
      }
    }
    
  


    // **** for testing ***
    // Interpolant interp(cg);
    // u.interpolate();
    

    realCompositeGridFunction v(cg2,all,all,all,R), err(cg2,all,all,all,R);
    v.setName("u",0);
    err.setName("err-u",0);
  

    if( testOld )
    {
      v=-1.;
      real time0=getCPU();
      bool useNewWay=false;
      int num=interpolateAllPoints(u,v,useNewWay);    // interpolate v from u
      real time=getCPU()-time0;

      printf("\n***Old: Time to interpolateAllPoints: %8.2e \n",time);
      cout << "      After interpolating a grid function: (number of extrapolated points =" << num << ")\n";  

      // computeError(cg,cg2,v,err);
      real error=0.;
      computeTheError(cg2,v,err,exact,error);
    }
    
    if( testNew )
    {
      InterpolatePoints interpolator;
      interpolator.setInfoLevel( infoLevel );
      
      v=-1.;
      real time0=getCPU();
      int num=interpolator.interpolateAllPoints(u,v,R,R,numGhost);    // interpolate v from u
      real time=getCPU()-time0;

      printF("\n***New: Time to interpolateAllPoints: %8.2e \n",time);
      printF(" After interpolating a grid function: (number of extrapolated points =%i\n",num);
  
      if( false ) // fix me -- we need an optimised version of interpolateAllPoints with a setup phase
      {
	v=-1.;
	time0=getCPU();
	num=interpolator.interpolateAllPoints(u,v,R,R,numGhost);    // interpolate v from u
	time=getCPU()-time0;

	printf("*     : Time to interpolate again:    %8.2e \n",time);
	
      }
      
      // u.display("Here is u (holds x and y):");
      // v.display("Here is v:");
      // computeError(cg,cg2,v,err);
      real error=0.;
      computeTheError(cg2,v,err,exact,error);
      
      
      // Now interpolate a few points
      int numPoints=5;
      RealArray xi(numPoints,3), ui(Range(numPoints),R);
      xi=0.;
      ui=-99.;
      // Interpolate on the line between xa and xb : 
      real xa[3]={.25,.25,.25}, xb[3]={.75,.75,.75}; 
      for( int i=0; i<   numPoints; i++ )
      {
	for( int axis=0; axis<numberOfDimensions; axis++ )
  	  xi(i,axis)=xa[axis]+(i-1)*(xb[axis]-xa[axis])/(numPoints-1.);
      }

      interpolator.interpolatePoints(xi,u,ui);
      real t=0.;
      for( int i=0; i<numPoints; i++ )
      {
	real uTrue = exact(xi(i,0),xi(i,1),xi(i,2),0,t);
	real err = fabs( uTrue-ui(i,0) );
	printF(" Point %i : xi=(%8.2e,%8.2e,%8.2e), ui=%8.2e, uTrue=%8.2e err=%8.2e\n",
               i,xi(i,0),xi(i,1),xi(i,2),ui(i,0),uTrue,err);
      }
    }

    if( testParallel )
    {
      // ---- test NEW (serial/parallel) version ----

      InterpolatePointsOnAGrid interpolator;
      interpolator.setInfoLevel( infoLevel );
      interpolator.setInterpolationWidth(interpolationWidth);
      // Set the number of valid ghost points that can be used when interpolating from a grid function: 
      interpolator.setNumberOfValidGhostPoints( numGhostToUse );
      
      // Assign all points, extrapolate pts if necessary:
      interpolator.setAssignAllPoints(true);
      

      Communication_Manager::Sync();

      if( testPoints )
      {
	// Interpolate a few points:

        RealArray xi;

        int numPoints = 3+myid;
        xi.redim(numPoints,3); xi=0.;
	
        real ds = 1./numPoints;
	for( int i=0; i<numPoints; i++ )
	{
          int j=i+myid;
	  xi(i,0) = j*.6*ds +.1;
	  xi(i,1) = j*.4*ds + .05;
	  if( cg.numberOfDimensions()==3 )
	    xi(i,2) = j*.2*ds + .15;
	}

	if( false )
	{
	  // *** grid=1 (stopper) i=(66,0,0) x=(5.782e-01,6.718e-01,0.000e+00) u=-2.692e+03 true=3.036e+00 err=2.695326e+03
          int i=0;
          //xi(i,0)=5.782e-01;
	  //xi(i,1)=6.718e-01;
          xi(i,0)=5.783e-01;
	  xi(i,1)=6.719e-01;
          // 5.744e-01,6.681e-01
          xi(i,0)=5.744e-01;
	  xi(i,1)=6.681e-01;

          xi(i,0)=2.;
	  xi(i,1)=3.;

	}

	if( cg.numberOfDimensions()==3 )
	{
	  // sibe1.order2
	  numPoints =2;
	  xi.redim(numPoints,3); xi=0.;
	  
          // *** grid=1 (north-pole) i=(2,9,4) x=(-7.518e-01,-2.734e-01,7.719e-03) u=2.520e+00 true=6.225e-01 err=1.897463e+00  
          //  *** grid=1 (north-pole) i=(3,9,4) x=(-7.396e-01,-2.958e-01,7.406e-02) u=2.550e+00 true=6.786e-01 err=1.871365e+00  
	  int i=0;
          xi(i,0)=-7.518e-01; xi(i,1)=-2.734e-01; xi(i,2)=7.719e-03;
          i=1;
          xi(i,0)=-7.396e-01; xi(i,1)=-2.958e-01; xi(i,2)=7.406e-02;
	  

        }
	

	int rt=interpolator.buildInterpolationInfo(xi,cg);
        const IntegerArray & status = interpolator.getStatus();
        if( rt!=0 )
	{
	  int num=abs(rt);
	  printF("testip: Error return from InterpolatePointsOnAGrid::buildInterpolationInfo could not interpolate \n"
                 "%i points.\n",num);
	  for( int i=0; i<numPoints; i++ )
	  {
	    printF(" pt %i : x=(%8.2e,%8.2e,%8.2e) status=%i (0=unable to interp)\n",i,xi(i,0),xi(i,1),xi(i,2),status(i));
	  }
	  OV_ABORT("ERROR");
	}
	

        RealArray ui(numPoints);
        ui=-999.;

        interpolator.interpolatePoints(u,ui);

        // check the errors
        real t=0.;
	for( int i=0; i<numPoints; i++ )
	{
	  real uTrue = exact(xi(i,0),xi(i,1),xi(i,2),0,t);
	  real err = fabs( uTrue-ui(i,0) );
	  printf("myid=%i :  Point %i : xi=(%8.2e,%8.2e,%8.2e), ui=%8.2e, uTrue=%8.2e err=%8.2e (status=%i)\n",
		 myid,i,xi(i,0),xi(i,1),xi(i,2),ui(i,0),uTrue,err,status(i));
	}

      }
      else
      {

	v=-1.;
	real time0=getCPU();
	int num=interpolator.interpolateAllPoints(u,v,R,R,numGhost);    // interpolate v from u
	real time=getCPU()-time0;

        int numNotAssigned=interpolator.getNumberUnassigned();
        numNotAssigned=ParallelUtility::getSum(numNotAssigned);
        int numExtrapolated=interpolator.getNumberExtrapolated();
	numExtrapolated=ParallelUtility::getSum(numExtrapolated);

	printF("\n***testip InterpolatePointsOnAGrid: np=%i, Time to interpolateAllPoints : cpu=%8.2e(s) \n",np,time);
	printF(" After interpolating a grid function: (number of extrapolated points =%i)\n",numExtrapolated);
	if( numNotAssigned>0 )
	{
          printF(" **** testip:WARNING %i pts NOT assigned ****\n",numNotAssigned);
	}
	
  
	if( true ) 
	{
	  v=-1.;
	  time0=getCPU();
	  num=interpolator.interpolateAllPoints(u,v,R,R,numGhost);    // interpolate v from u
	  time=getCPU()-time0;

	  printF("*     : Time to interpolate again:    %8.2e (s)\n",time);
	
	}
      
	// u.display("Here is u (holds x and y):");
	// v.display("Here is v:");
	// computeError(cg,cg2,v,err);
	real error=0.;
	computeTheError(cg2,v,err,exact,error);
      }
      
      
    } // end if testParallel
    
    
    if( testFindNearest )
    {
      PlotStuff gi;
      GraphicsParameters gip;
      
      
      gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,true); 
      PlotIt::plot(gi, cg, gip );
      gip.set(GI_USE_PLOT_BOUNDS,true);

      RealArray x(1,3), ci(1,3);
      IntegerArray il(1,4);
      for( ;; )
      {

	gi.inputString(line,"Enter a point x,y,z (to find the nearest grid point to, `done' to finish)) \n");
	if( line=="done" ) break;
	
	sScanF(line,"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
	if( numberOfDimensions==2 ) x(0,2)=0.;
	
	int rt = findNearestValidGridPoint( cg, x, il, ci );
	
        // plot results
        int donor=il(0,numberOfDimensions);
	assert( donor>=0 && donor<cg.numberOfComponentGrids() );
	
	RealArray rc(1,3),xc(1,3);
	rc=0.;
	for( int axis=0; axis<numberOfDimensions; axis++ )
	  rc(0,axis)=ci(0,axis);

	MappedGrid & mg = cg[donor];
	Mapping & map = mg.mapping().getMapping();

	map.mapS(rc,xc);

        RealArray points(2,3); points=0.;
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  points(0,axis)=x(0,axis);
	  points(1,axis)=xc(0,axis);
	}
	

        gi.erase();

        int ptSize=5;
	gip.set(GI_POINT_SIZE,ptSize*gi.getLineWidthScaleFactor());
        gi.plotPoints(points,gip);
        PlotIt::plot(gi, cg, gip );

      }
      
    }
    




    if( plot )
    {
      PlotStuff ps;
      plotResults(  ps,v,err, cg,u );
      
    }
    
  }
  else
  {

    // *******************************************
    // ****** interpolate surface grids **********
    // *******************************************

    aString gridName="sibe.hdf";
    CompositeGrid cg;
    getFromADataBase(cg,gridName);
    cg.update(MappedGrid::THEmask);

    // Here is the object that knows how to stitch surfaces.
    SurfaceStitcher stitcher;

    // Here is the object used to define which surfaces to stitch -- by default stitch all surfaces.
    BodyDefinition bd;

    if( gridName.matches("sib") )
    {
      int surface=0;
      int numberOfFaces=2;
      IntegerArray boundary(3,numberOfFaces);
      int side=0, axis=axis3, grid=1;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      grid=2;
      boundary(0,1)=side;
      boundary(1,1)=axis;
      boundary(2,1)=grid;
      bd.defineSurface( 0,numberOfFaces,boundary ); 
    }
    

    if( bd.totalNumberOfSurfaces()>0 )
      stitcher.defineSurfaces( cg,&bd );
    else
      stitcher.defineSurfaces(cg);  // choose all boundary surfaces

    // stitcher.enlargeGap(gapWidth);

    assert( stitcher.getSurfaceCompositeGrid()!=NULL );
    CompositeGrid & cgSurf = *stitcher.getSurfaceCompositeGrid();
    
    PlotStuff ps;
    PlotStuffParameters psp;
    psp.set(GI_PLOT_UNS_EDGES,true);
    psp.set(GI_PLOT_UNS_FACES,true);

    bool plotSurfaceGrid=true;
    if( false && plotSurfaceGrid )
    {
      psp.set(GI_TOP_LABEL,"Surface CompositeGrid");
      PlotIt::plot(ps,*stitcher.getSurfaceCompositeGrid(),psp);
    }

    // stitcher.stitchSurfaceCompositeGrid(interactiveStitcher);

    // cg.setSurfaceStitching( stitcher.getUnstructuredGrid() );
	
//     UnstructuredMapping &umap = *stitcher.getUnstructuredGrid();
//     int numberOfTriangles=umap.size(UnstructuredMapping::Face);
//     printF("computeStitchedSurfaceWeights: Number of triangles on the unstructured stitcher grid = %i\n",
// 	   numberOfTriangles);
    
//     numberOfTriangles=min(2,numberOfTriangles); // *************************

//     realArray & verts = (realArray &)umap.getNodes();
//     intArray & tris = (intArray &) umap.getEntities(UnstructuredMapping::Face);

//     realArray tCenters(numberOfTriangles, cg.numberOfDimensions());
//     ArraySimpleFixed<real,3,1,1,1> v0,v1,v2;
  
//     for ( int e=0; e<numberOfTriangles; e++ )
//     {
//       for ( int a=0; a<3; a++ )
//       {
// 	v0[a] = verts(tris(e,0),a);
// 	v1[a] = verts(tris(e,1),a);
// 	v2[a] = verts(tris(e,2),a);
// 	tCenters(e,a) = (v0[a] + v1[a] + v2[a])/3;
//       }
//     }

//      stitcher.setMask(SurfaceStitcher::originalMask);
//      stitcher.setMask(SurfaceStitcher::enlargedHoleMask);

    int numPoints=2;
    Range R= numPoints;
    
    RealArray x(numPoints,3);
    x(0,0)=.50; x(0,1)=0.1; x(0,2)=0.;
    x(1,0)=.42; x(1,1)=-.1; x(1,2)=+.3;

    InterpolatePoints interpolate;

    // Interpolate from the surface grid:
    RealArray xp; // holds projected points
    interpolate.buildInterpolationInfo(x, cgSurf, &xp);

    // ::display(xp,"Projected points after buildInterpolationInfo","%6.3f ");


    RealArray interpCoeff(numPoints,4);
    interpCoeff = 0;

    interpolate.interpolationCoefficients(cgSurf,interpCoeff);
    // ::display(interpCoeff,"interpCoeff's","%5.2f ");


    // ===== Interpolate a function and check the error =====
    cgSurf.update( MappedGrid::THEcenter );
    
    Range Rx=Range(0,cgSurf.numberOfDimensions()-1);
    realCompositeGridFunction u(cgSurf,all,all,all,Rx);

    Index I1,I2,I3;
    
    // cg.numberOfInterpolationPoints.display("Here is og.numberOfInterpolationPoints");
    for( int grid=0; grid<cgSurf.numberOfComponentGrids(); grid++)
    {
      getIndex(cgSurf[grid].dimension(),I1,I2,I3);
      u[grid](I1,I2,I3,Rx)=cgSurf[grid].center()(I1,I2,I3,Rx);
    } 


    RealArray uInterpolated(R,Rx);
    interpolate.interpolatePoints(u,uInterpolated,Rx);
     
    for( int i=0; i<numPoints; i++ )
    {
      printF(" i=%i xp=(%g,%g,%g) uInterpolated=(%g,%g,%g) err=(%8.2e,%8.2e,%8.2e)\n",
	     xp(i,0),xp(i,1),xp(i,1),
	     uInterpolated(i,0),uInterpolated(i,1),uInterpolated(i,1),
	     fabs(xp(i,0)-uInterpolated(i,0)),
	     fabs(xp(i,1)-uInterpolated(i,1)),
	     fabs(xp(i,2)-uInterpolated(i,2)));
    }


    if( true )
    {
      // Now plot the two points and a line joining them

      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      ps.erase();

      psp.set(GI_MAPPING_COLOUR,"black");
      psp.set(GraphicsParameters::curveLineWidth,3.);

      RealArray segments(R,3,2);
      Range R3=3;
      segments(R,R3,0)=x(R,R3);  // start point of line segment
      segments(R,R3,1)=xp(R,R3);  // end pt of line segment
      #ifndef USE_PPP
      ps.plotLines(segments, psp);
      #endif

      psp.set(GI_POINT_SIZE, (real) 6.0);
      psp.set(GI_POINT_COLOUR, "green");
      ps.plotPoints(x,psp);
      psp.set(GI_POINT_COLOUR, "red");
      ps.plotPoints(xp,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
       
      PlotIt::plot(ps,*stitcher.getSurfaceCompositeGrid(),psp);
    }

     



//     IntegerArray indexValues,interpoleeGrid;
//     interpolate.getInterpolationInfo(cg,indexValues,interpoleeGrid);


  }
  

//     if( false )
//   {

//     int numberOfPointsToInterpolate=1;
//     RealArray positionToInterpolate(numberOfPointsToInterpolate,3), 
//       uInterpolated(numberOfPointsToInterpolate,2);
//     uInterpolated=0.;
//     for(;;)
//     {
//       cout << "Enter a point to interpolate (x,y) \n";
//       cin >> positionToInterpolate(0,0) >> positionToInterpolate(0,1) ;
//       int extrap = interpolatePoints(positionToInterpolate,u,uInterpolated);
//       cout << " extrap = " << extrap << endl;
//       uInterpolated.display("Here is uInterpolated: (should be = to (x,y)");
//     }
//   }
  
  Overture::finish();          
  return 0;
}

