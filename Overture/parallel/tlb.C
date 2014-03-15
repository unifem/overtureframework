// 
//  Test code for the LoadBalancer
//
//  tlb -noplot load
// 
//  mpirun -np 2 -all-local tlb
//  mpirun-wdh -np 2 -all-local tlb


#include "LoadBalancer.h"
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)



int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);
  #ifdef USE_PPP
    aString fileName = "lbInputFile";
    ParallelUtility::getArgsFromFile(fileName,argc,argv );
  #endif


  int debug=0;

//   MappedGrid::setMinimumNumberOfDistributedGhostLines(2); 

  char buff[80];

  bool plotOption=true;
  aString commandFileName="", line;
  if( argc>1 )
  {
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( len=line.matches("-noplot") )
      {
	plotOption=false;
	printF(" Setting plotOption=false\n");
      }
      else if( commandFileName=="" )
      {
	commandFileName=line;    
	if( myid==0 ) printF("Using command file = [%s]\n",(const char*)commandFileName);
      }
    }
  }
  
  #ifdef USE_PPP
    ParallelUtility::deleteArgsFromFile(argc,argv);
  #endif


  LoadBalancer loadBalancer;
  int pStart=0, pEnd=7;
  #ifdef USE_PPP
    pEnd=np-1;
  #endif
  int maxProc=pEnd-pStart+1;

  loadBalancer.setProcessors(pStart,pEnd);

  PlotStuff ps(plotOption,"tlb");               // for plotting
  GraphicsParameters psp;

  aString logFile="tlb.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  aString menu[]=
  {
    "choose a grid",
    "number of processors",
    "get load balance",
    "assign load balance",
    "show array distributions",
    "grid plot",
    "contour plot",
    "erase",
    "exit",
    ""
  };
  aString answer,answer2;
  
  int currentGrid=0;
  CompositeGrid cg;
  realCompositeGridFunction u;
  
//   psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
//   psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,TRUE);
//   psp.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,1);
//   psp.set(GI_PLOT_INTERPOLATION_POINTS,TRUE);
//   psp.set(GI_COLOUR_INTERPOLATION_POINTS,TRUE);

  int grid=0, level=1;
  bool plotGrid=false;
  aString nameOfOGFile;
  
  GridDistributionList gridDistributionList;


  for( ;; )
  {

    if( plotOption && plotGrid )
    {
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      PlotIt::plot(ps,cg,psp);
    }
    
    ps.getMenuItem(menu,answer,"choose");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="number of processors" )
    {
      int np=1;
      ps.inputString(line,"Enter the number of processors to load balance over\n");
      sScanF(line,"%i",&np);
      loadBalancer.setProcessors(0,np-1);
      maxProc=np;
    }
    else if( answer=="get load balance" )
    {
       // Assign work loads for each grid based on the number of grid points
      loadBalancer.assignWorkLoads( cg,gridDistributionList );
  
      loadBalancer.determineLoadBalance( gridDistributionList );
    }
    else if( answer=="assign load balance" )
    {
      fflush(0);

      loadBalancer.assignLoadBalance( cg, gridDistributionList );
      cg.update(MappedGrid::THEcenter );
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
        realArray & x = mg.center();
	const realSerialArray & xLocal = x.getLocalArray();
	
	Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
	const intSerialArray & processorSet = partition.getProcessorSet();

	printf(" grid=%i: actual-processors=[%i,%i] myid=%i: bounds=[%i,%i][%i,%i] local bounds=[%i,%i][%i,%i]\n",
	       grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)),myid,
               x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),
               xLocal.getBase(0),xLocal.getBound(0),xLocal.getBase(1),xLocal.getBound(1));
	
      }
      fflush(0);
    }
    else if( answer=="show array distributions" )
    {
      int pStart,pEnd;
      int dimProc[3];
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        const IntegerArray & d = cg[grid].dimension();
	int dims[3]={1,1,1};//
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  dims[axis]=(d(1,axis)-d(0,axis)+1);
 
        gridDistributionList[grid].getProcessorRange(pStart,pEnd);
        int nProc=pEnd-pStart+1;

	gridDistributionList[grid].computeParallelArrayDistribution(dimProc);
	gridDistributionList[grid].getProcessorRange(pStart,pEnd);

	int minPts=INT_MAX;
	int maxPts=0;
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  if( dims[axis]>1 ) // some dimensions may not have many points 
	  {
	    int pointsPerProc=dims[axis]/dimProc[axis];
	    minPts = min(minPts,pointsPerProc);
	    maxPts = max(maxPts,pointsPerProc);
	  }
	}
	real ratio=real(maxPts)/real(minPts);

	printF("*** grid =%i : maxProc=%i, nProc=%i proc-decomp=[%i]x[%i]x[%i], grid-pts=[%i]x[%i]x[%i], "
	       "grid-pts/proc=[%i]x[%i]x[%i] ratio=%g\n",
	       grid,maxProc,pEnd-pStart+1,dimProc[0],dimProc[1],dimProc[2],
	       (d(1,0)-d(0,0)+1),
	       (d(1,1)-d(0,1)+1),
	       (d(1,2)-d(0,2)+1),
	       (d(1,0)-d(0,0)+1)/dimProc[0],
	       (d(1,1)-d(0,1)+1)/dimProc[1],
	       (d(1,2)-d(0,2)+1)/dimProc[2],ratio );

      }
    }
    else if( answer=="compute array distributions" )
    {
      // getProcessorRange( int & pStart, int & pEnd)
      int pStart,pEnd;
      int dimProc[3];
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        const IntegerArray & d = cg[grid].dimension();
	int dims[3]={1,1,1};//
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  dims[axis]=(d(1,axis)-d(0,axis)+1);
 
        const int minimumNumberOfPoints=7;  // we would like at least this many points in any direction, per processor
	int numIts=6;
        int npOpt;
        real ratioOpt=REAL_MAX/10.;
        gridDistributionList[grid].getProcessorRange(pStart,pEnd);
        int nProc=pEnd-pStart+1;

        for( int it=0; it<numIts; it++ )
	{
	  gridDistributionList[grid].computeParallelArrayDistribution(dimProc);

	  gridDistributionList[grid].getProcessorRange(pStart,pEnd);
	  printF(" it=%i: grid =%i : np=%i proc-decomp=[%i]x[%i]x[%i], grid-pts=[%i]x[%i]x[%i], "
		 "grid-pts/proc=[%i]x[%i]x[%i]\n",
		 it,grid,pEnd-pStart+1,dimProc[0],dimProc[1],dimProc[2],
		 (d(1,0)-d(0,0)+1),
		 (d(1,1)-d(0,1)+1),
		 (d(1,2)-d(0,2)+1),
		 (d(1,0)-d(0,0)+1)/dimProc[0],
		 (d(1,1)-d(0,1)+1)/dimProc[1],
		 (d(1,2)-d(0,2)+1)/dimProc[2]
	    );

	  // Now look for a better distribution:

	  int minPts=INT_MAX;
	  int maxPts=0;
	  for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  {
	    if( dimProc[axis]>1 || dims[axis]>=minimumNumberOfPoints ) // some dimensions may not have many points 
	    {
	      int pointsPerProc=dims[axis]/dimProc[axis];
	      minPts = min(minPts,pointsPerProc);
	      maxPts = max(maxPts,pointsPerProc);
	    }
	  }
          bool ok=false;
	  if( minPts > minimumNumberOfPoints )
	  {
            ok=true;
	  }
          real ratio=real(maxPts)/real(minPts);
          printF(" minPts=%i maxPts=%i ratio=%g   -> ok=%i \n",minPts,maxPts,ratio,(int)ok);
          if( ok && fabs(ratio-1.) < fabs(ratioOpt-1.) )
	  {
	    ratioOpt=ratio;
	    npOpt=nProc;
	  }
          nProc=max(1,nProc-1); // try one less processor
	  gridDistributionList[grid].setProcessors(pStart,pStart+nProc-1);
	}
        printF(" grid=%i: --> optimum processors:  np=%i, ratio=%g\n",grid,npOpt,ratioOpt);
      }
    }
    else if( answer=="choose a grid" )
    {

      ps.inputString(nameOfOGFile,"Enter the name of the grid");
      cout << "read grid " << nameOfOGFile << endl;
      getFromADataBase(cg,nameOfOGFile);
      cg.update(MappedGrid::THEmask);
      plotGrid=true;
    }
//     else if( answer=="add a refinement" )
//     {
//       if( false ) printInfo(cg);
//       ps.inputString(answer2,"Enter grid,level,range(0,0),range(1,0),...,range(1,2) refinementRatio");
//       grid=0;
//       level=1;
//       range=0;
//       int refinementRatio=2;
//       sScanF(answer2,"%i %i %i %i %i %i  %i %i %i",&grid,&level,&range(0,0),&range(1,0),
// 	     &range(0,1),&range(1,1),&range(0,2),&range(1,2),&refinementRatio);

//       printF("*** refinementRatio=%i \n",refinementRatio);

//       factor=refinementRatio;
//       cg.addRefinement(range, factor, level, grid); 

//       cg.update(GridCollection::THErefinementLevel);  // this seems to be needed.
//       // display(cg.interpolationWidth,"cg.interpolationWidth");
//     }
//     else if( answer=="update refinements" ||
//              answer=="update refinements new" )
//     {
//       #ifndef USE_PPP
//       if( answer=="update refinements" )
//         ogen.updateRefinement(cg);
//       else
//         ogen.updateRefinementNew(cg);
//       #else

//         // always call the new version in parallel:
//         const int numberOfSteps=2;  // check for leaks
//         for( int step=0; step<numberOfSteps; step++ )
//         {
//           ogen.updateRefinementNew(cg);
//           checkArrayIDs(sPrintF(buff,"regrid: after updateRefinementNew (step %i)",step)); // check for possible leaks
//         }
      
//       #endif

//       // cg.setMaskAtRefinements();

//       if( checkValidity )
//       {
	  
// 	printF("Checking validity of the overlapping grid...\n");
// 	int numberOfErrors=checkOverlappingGrid(cg);
// 	if( numberOfErrors==0 )
// 	{
// 	  printF("Overlapping grid is valid.\n");
// 	}
// 	else
// 	{
// 	  printF("Checking validity of the overlapping grid, Grid is not valid! Number of errors=%i\n",numberOfErrors);
// 	}
//       }
      
//       if( debug & 2 )
//       {
// 	for( int grid=0; grid<cg.numberOfGrids(); grid++ )
// 	  displayMask(cg[grid].mask(),"cg[grid].mask");
//       }
//     }
//    }
    else if( answer=="grid plot" )
    {
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(ps,cg,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="contour plot" )
    {
      if( u.isNull() )
      {
	u.updateToMatchGrid(cg);
	u=1.;
      }
      
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::contour(ps,u,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="erase" )
    {
      ps.erase();
      plotGrid=false;
    }
    else if( answer=="debug" )
    {
      ps.inputString(answer,"Enter debug");
//       sScanF(answer,"%i",&ogen.debug);
//       printF(" ogen.debug = %i\n",ogen.debug);
    }
    else
    {
      printF("Unknown response: [%s] \n",(const char*)answer);
      ps.stopReadingCommandFile();
    }
  }
  

  Overture::finish();          
  return 0;
}
