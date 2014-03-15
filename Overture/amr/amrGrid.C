#include "Overture.h"  
#include "PlotStuff.h"
#include "AnnulusMapping.h"
#include "SquareMapping.h"
#include "GridCollectionOperators.h"
#include "Regrid.h"
#include "ErrorEstimator.h"
#include "InterpolateRefinements.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "LoadBalancer.h"
#include "Ogen.h"

int setMaskAtRefinements(GridCollection & gc );

int 
outputGridInfo( GridCollection & gc, 
		const aString & gridFileName, 
		const aString & fileName )
// =======================================================================================
// /Description:
//   This function will output a command file for building the AMR grid from scratch with
// grids added as base grids.
//\end{RegridInclude.tex} 
// ========================================================================================
{
  printF("*** outputing a command file %s for refine ****\n",(const char*)fileName);
  
  FILE *file=fopen(fileName,"w");

  int refinementRatio=2;
  if( gc.numberOfRefinementLevels()>1 )
    refinementRatio=gc.refinementLevel[1].refinementFactor(0,0);
  
  fprintf(file,"* Starting from: [%s] \n",(const char*)gridFileName);
  fprintf(file,
          "  create mappings\n"
          " * \n"
          "   rectangle\n"
          "     lines\n"
          "       33 33 \n"
          "    mappingName\n"
          "      square\n"
          "    exit\n"
          " * \n");

  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    int level=gc.refinementLevelNumber(grid);
    if( level>0 )
    {
      int baseGrid = gc.baseGridNumber(grid);
      MappedGrid & gb = gc[baseGrid];
      
      MappedGrid & mg = gc[grid];
//       Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
//       const intSerialArray & processorSet = partition.getProcessorSet();

      real ratio = pow(real(refinementRatio),real(level));
      RealArray rab(2,3);
      rab(0,2)=0.; rab(1,2)=1.;
      for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  rab(side,axis) = (mg.gridIndexRange(side,axis) - gb.gridIndexRange(0,axis)*ratio)/
	                    (ratio*(gb.gridIndexRange(1,axis) - gb.gridIndexRange(0,axis)));
	}
      }
      

      fprintf(file,
	      "reparameterize\n"
              "  transform which mapping?\n"
              "   %s\n"
              "  restrict parameter space\n"
              "    exit\n"
              "  set corners\n"
              "    %20.12e %20.12e %20.12e %20.12e %20.12e %20.12e  \n"
              "  lines\n"
              "    %i %i %i \n"
              "  boundary conditions\n"
              "    %i %i %i %i %i %i \n"
              "  share              \n"
              "     %i %i %i %i %i %i \n"
              "  mappingName\n"
              "    grid%i \n"
              "  exit\n",
	      (const char*)gb.getName(),
              rab(0,0),rab(1,0),rab(0,1),rab(1,1),rab(0,2),rab(1,2),
              mg.gridIndexRange(1,0)-mg.gridIndexRange(0,0)+1,
              mg.gridIndexRange(1,1)-mg.gridIndexRange(0,1)+1,
              mg.gridIndexRange(1,2)-mg.gridIndexRange(0,2)+1,
	      mg.boundaryCondition(0,0),mg.boundaryCondition(1,0),
	      mg.boundaryCondition(0,1),mg.boundaryCondition(1,1),
	      mg.boundaryCondition(0,2),mg.boundaryCondition(1,2),
	      mg.sharedBoundaryFlag(0,0),mg.sharedBoundaryFlag(1,0),
	      mg.sharedBoundaryFlag(0,1),mg.sharedBoundaryFlag(1,1),
	      mg.sharedBoundaryFlag(0,2),mg.sharedBoundaryFlag(1,2),
              grid);
    }
  }
  fprintf(file,
	  "exit this menu\n"
	  "generate an overlapping grid\n");
      

  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)==0 )
    {
      fprintf(file,"%s\n",(const char*)gc[grid].getName());
    }
    else
    {
      fprintf(file,"grid%i\n",grid);
    }
    
  }
  fprintf(file,"done choosing mappings\n"
               "compute overlap\n"
               "plot\n");
  


  fclose(file);
  return 0;
}




int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int numProc= max(1,Communication_Manager::numberOfProcessors());

  Range all;

  printF(" ------------------------------------------------------------------- \n");
  printF(" Test the amr regrid function                                        \n");
  printF("    Usage:                                                           \n");
  printF("    amrGrid [file.cmd]                                               \n");
  printF(" ------------------------------------------------------------------- \n");

  aString nameOfOGFile="square20.hdf";
  aString commandFileName = "amrGrid1.cmd";

  int len;
  aString arg;
  for( int i=1; i<argc; i++ )
  {
    arg=argv[i];
    commandFileName=arg;
  }
  

  #ifndef USE_PPP
  if( argc > 1 )
  { 
    nameOfOGFile=argv[1];
  }
  #endif
  
  FILE *debugFile = fopen("amrGrid.debug","w" ); 


  PlotStuff ps(TRUE, "amrGrid");       // create a PlotStuff object
  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,TRUE);

  aString logFile="amrGrid.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  ps.inputString(nameOfOGFile,"Enter the name of the grid");

  CompositeGrid cg;
  bool loadBalance=true;
  getFromADataBase(cg,nameOfOGFile,loadBalance);

  Regrid regrid;
  InterpolateRefinements interpolateRefinements(cg.numberOfDimensions());
  ErrorEstimator errorEstimator(interpolateRefinements);

  regrid.debug=0; // *****************

  int numberOfDimensions = cg.numberOfDimensions();
  bool addGridsAsBaseGrids=false;

  //...plot the grid
  psp.set(GI_TOP_LABEL,"Initial grid");
  PlotIt::plot(ps,cg,psp);

  aString menu[]=
  {
    "compute grid",
    "compute grid with nesting",
    ">error function",
      "two solid circles",
      "diagonal",
      "cross",
      "plus",
      "hollow circle",
    "<number of refinement levels",
    "maximum number of splits",
    "minimum box width",
    "minimum refinement size",
    "maximum refinement size",
    "grid efficiency",
    "refinement ratio",
    "number of buffer zones",
    "width of proper nesting",
    "index coarsening factor",
    "use smart bisection",
    "do not use smart bisection",
    "add new grids as refinements",
    "add new grids as base grids",
    "set zero base level",
    "set base level",
    "allow rotated grids",
    "aligned grids",
    "do not merge boxes",
    "use error function",
    "do not use error function",
    "change the plot",
    "plot points",
    "do not plot points",
    "smooth error function",
    "save the grid",
    "debug",
    "exit",
    ""
  };
  aString answer;
  int numberOfRefinementLevels=3;

  ErrorEstimator::ErrorFunctionEnum errorType=ErrorEstimator::diagonal; // cross; // diagonal;

  real efficiency=.7; 
  int flaggedRegionGrowthSize=3;
  int minimumRefinementSize=16;
  int maximumRefinementSize=16;
  int baseLevel=-1; // 0; // -1;  // this needs to be zero for level 3 grids to extend to the interp-boundaries!

  bool useErrorFunction=true;
  bool plotPoints=true;
  bool smoothErrorFunction=false;
  
  CompositeGrid cgOld,cgNew;
  
  for( ;; )
  {
    ps.getMenuItem(menu,answer,"choose" );
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="two solid circles" )
    {
      errorType=ErrorEstimator::twoSolidCircles;
    }
    else if( answer=="diagonal" )
    {
      errorType=ErrorEstimator::diagonal;
    }
    else if( answer=="cross" )
    {
      errorType=ErrorEstimator::cross;
    }
    else if( answer=="plus" )
    {
      errorType=ErrorEstimator::plus;
    }
    else if( answer=="hollow circle" )
    {
      errorType=ErrorEstimator::hollowCircle;
    }
    else if( answer=="number of refinement levels" )
    {
      ps.inputString(answer,"Enter the number of refinement levels");
      sScanF(answer,"%i",&numberOfRefinementLevels);
      printf("set numberOfRefinementLevels=%i\n",numberOfRefinementLevels);
      
    }
    else if( answer=="minimum box width" )
    {
      int minBoxWidth=5;
      ps.inputString(answer,"Enter the minimum box width");
      sScanF(answer,"%i",&minBoxWidth);
      printf("set the minimum box width=%i\n",minBoxWidth);
      regrid.setMinimumBoxWidth(minBoxWidth);
    }
    else if( answer=="maximum number of splits" )
    {
      int maximumNumberOfSplits=1000000;
      ps.inputString(answer,"Enter the maximum number of splits");
      sScanF(answer,"%i",&maximumNumberOfSplits);
      printf("set maximumNumberOfSplits=%i\n",maximumNumberOfSplits);
      regrid.setMaximumNumberOfSplits(maximumNumberOfSplits);
    }
    else if( answer=="minimum refinement size" )
    {
      ps.inputString(answer,"Enter the minimum refinement size");
      sScanF(answer,"%i",&minimumRefinementSize);
      printf("set minimumRefinementSize=%i\n",minimumRefinementSize);
      
    }
    else if( answer=="maximum refinement size" )
    {
      ps.inputString(answer,"Enter the maximum refinement size");
      sScanF(answer,"%i",&maximumRefinementSize);
      printf("set maximumRefinementSize=%i\n",maximumRefinementSize);
    }
    else if( answer=="grid efficiency" )
    {
      ps.inputString(answer,"Enter the grid efficiency 0< eff < 1");
      sScanF(answer,"%e",&efficiency);
      printf("set efficiency=%e\n",efficiency);

      regrid.setEfficiency(efficiency);
      
    }
    else if( answer=="refinement ratio" )
    {
      int refinementRatio=2;
      ps.inputString(answer,"Enter the refinement ratio");
      sScanF(answer,"%i",&refinementRatio);

      printf("set refinementRatio=%i\n",refinementRatio);

      regrid.setRefinementRatio(refinementRatio);
      
    }
    else if( answer=="number of buffer zones" )
    {
      ps.inputString(answer,"Enter the number of buffer zones, (flagged region growth size)");
      sScanF(answer,"%i",&flaggedRegionGrowthSize);
      printf("set number of buffer zones=%i\n",flaggedRegionGrowthSize);
      
      regrid.setNumberOfBufferZones(flaggedRegionGrowthSize);  // expansion of tagged error points
      // regrid.setWidthOfProperNesting(flaggedRegionGrowthSize); // distance between levels
    }
    else if( answer=="width of proper nesting" )
    {
      int widthOfProperNesting=1;
      ps.inputString(answer,"Enter the width of proper nesting (min-dist between amr levels)");
      sScanF(answer,"%i",&widthOfProperNesting);
      printf("set width of proper nesting=%i\n",widthOfProperNesting);
      
      regrid.setWidthOfProperNesting(widthOfProperNesting); // distance between levels
    }
    else if( answer=="index coarsening factor" )
    {
      int factor=1;
      ps.inputString(answer,"Enter the index coarsening factor");
      sScanF(answer,"%i",&factor);
      printF("set index coarsening factor=%i\n",factor);
      
      regrid.setIndexCoarseningFactor(factor);
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
      addGridsAsBaseGrids=false;
    }
    else if( answer=="add new grids as base grids" )
    {
      regrid.setGridAdditionOption(Regrid::addGridsAsBaseGrids);
      addGridsAsBaseGrids=true;
    }
    else if( answer=="set base level" )
    {
      ps.inputString(answer,"Enter the base level (-1 = only build a new level, 0=rebuild all levels)");
      sScanF(answer,"%i",&baseLevel);
      printf("set baseLevel=%i\n",baseLevel);
    }
    else if( answer=="set zero base level" )
    {
      baseLevel=0;
      printf("set baseLevel=%i\n",baseLevel);
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
    else if( answer=="use error function" )
    {
      useErrorFunction=true;
    }
    else if( answer=="do not use error function" )
    {
      useErrorFunction=false;
      printf("do not use error function\n");
    }
    else if( answer=="debug" )
    {
      regrid.debug=7;
      printf("Setting regrid.debug=7\n");
    }
    else if( answer=="plot points" )
    {
      plotPoints=true;
    }
    else if( answer=="do not plot points" )
    {
      plotPoints=false;
    }
    else if( answer=="smooth error function" )
    {
      smoothErrorFunction=true; 
    }
    else if( answer=="save the grid" )
    {
      aString fileName,gridName;
      ps.inputString(fileName,"Enter the name of the file");
      for( ;; )
      {
	ps.inputString(gridName,"Save the grid under which name?");
	if( gridName=="." )
	  ps.outputString("Error: do not choose `.' as a name");
	else
	  break;
      }      
      // printF("grid before\n");
      // printInfo(cg,1);

      real time=getCPU();
      cgNew.saveGridToAFile(fileName,gridName);
      time=getCPU()-time;
      printF(" amrGrid: time to save the grid in a file = %8.2e(s)\n",time);
    }
    else if( answer=="change the plot" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp.set(GI_TOP_LABEL,"Refined grid");
      if( cgNew.numberOfComponentGrids()>0 )
        PlotIt::plot(ps,cgNew,psp);
      else
        PlotIt::plot(ps,cg,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="compute grid" || answer=="compute grid with nesting" )
    {
      // bool nestedGrids= addGridsAsBaseGrids || answer=="compute grid with nesting";
      bool nestedGrids= answer=="compute grid with nesting";
      
      cgOld.destroy();
      cgNew.destroy();
      cgOld=cg;
      
      bool debug=true;
  
      Index I1,I2,I3;                                            
      realCompositeGridFunction error;

      int numberOfOldBaseGrids=0;
      
      // ---- add levels 1 at a time ----
      for( int level=1; level<numberOfRefinementLevels; level++ ) 
      {
	
        CompositeGrid & cg0 = ((level%2)==1) ?  cgOld : cgNew;
        CompositeGrid & cg1 = ((level%2)==1) ?  cgNew : cgOld;
	
        CompositeGridOperators cgop(cg0);
	Interpolant interpolant(cg0);
	
  	error.updateToMatchGrid(cg0);
	
        errorEstimator.computeErrorFunction( error, errorType );

        if( smoothErrorFunction )
	{
	  error.setOperators(cgop);
	  errorEstimator.smoothErrorFunction(error); // (error,numberOfSmooths)
	}
	if( addGridsAsBaseGrids )
	{
          printf("*** zero error on %i old base grids\n",numberOfOldBaseGrids);
	  
          for( int grid=0; grid<numberOfOldBaseGrids; grid++ )
	    error[grid]=0.;

          numberOfOldBaseGrids=cg0.numberOfBaseGrids();

	}
	
	printF("amrGrid: adaptively refining grid, level=%i, ...\n",level);

	real errorThreshhold=.1;

	ps.erase();
        if( plotPoints )
	{
	  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  errorEstimator.plotErrorPoints( error, errorThreshhold,ps,psp );
	  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	}
	
        if( debug && level>1 )
	{
          for( int ll=0; ll<min(level,cg0.numberOfRefinementLevels()); ll++ )
	  {
	    printf(" cg0.numberOfRefinementLevels()=%i\n",cg0.numberOfRefinementLevels());
	    printf(" BEFORE REGRID cg0.refinementLevel[%i].numberOfGrids()=%i \n", ll,
		   cg0.refinementLevel[ll].numberOfGrids());
	  }
	}
	
        // We either recompute all levels (nestedGrids==true) or just compute the next level.
	
        // if we add grids as base grids then we always are adding level 1
        int regridLevel = addGridsAsBaseGrids ? 1: level;

        if( useErrorFunction )
  	  regrid.regrid(cg0,cg1, error, errorThreshhold, regridLevel, (nestedGrids ? 0 : baseLevel));
        else
	{
          printf("build errorFlag array...\n");

          intCompositeGridFunction errorFlag(cg0);
          for( int grid=0; grid<cg0.numberOfGrids(); grid++ )
  	    errorFlag[grid]=error[grid]>errorThreshhold;
	  regrid.regrid(cg0,cg1, errorFlag, regridLevel, (nestedGrids ? 0 : baseLevel));
	}
	
	// cg1.update(GridCollection::THErefinementLevel);
        for( int ll=0; ll<cg1.numberOfRefinementLevels(); ll++ )
	{
	  printf("AFTER regrid cg1.refinementLevel[%i].numberOfGrids()=%i \n", ll,cg1.refinementLevel[ll].numberOfGrids());
	  // cg1.refinementLevel[ll].refinementFactor.display("cg1.refinementLevel[.].refinementFactor");
	}
	if( false )
	{
	  cg1.update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex );
	  for( int grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg1[grid];
	    realArray & x = mg.center();
	    const realSerialArray & xLocal = x.getLocalArray();
	
	    Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
	    const intSerialArray & processorSet = partition.getProcessorSet();

	    printf(" grid=%i: actual-proc=[%i,%i] myid=%i: x: bounds=[%i,%i][%i,%i] local bounds=[%i,%i][%i,%i]\n",
		   grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)),myid,
		   x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),
		   xLocal.getBase(0),xLocal.getBound(0),xLocal.getBase(1),xLocal.getBound(1));
	
	    intArray & mask = mg.mask();
	    const intSerialArray & maskLocal = mask.getLocalArray();
	    printf(" grid=%i: actual-proc=[%i,%i] myid=%i: mask: bounds=[%i,%i][%i,%i] local bounds=[%i,%i][%i,%i]\n",
		   grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)),myid,
		   mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1),
		   maskLocal.getBase(0),maskLocal.getBound(0),maskLocal.getBase(1),maskLocal.getBound(1));

	  }
	  fflush(0);
	}

	cg1.update(MappedGrid::THEmask );

	if( !addGridsAsBaseGrids )
	{
	  // cg1.setMaskAtRefinements();
          Ogen ogen(ps);
	  ogen.updateRefinement(cg1);
	  
	}
	else
	{

          Ogen ogen(ps);

	  // ogen.debug=3;
          // ogen.info=3;

          cg1.update(MappedGrid::THEmask );
	  for( int grid=0; grid<cg1.numberOfBaseGrids(); grid++ )
	  {
	    intArray & mask = cg1[grid].mask();
	    mask=MappedGrid::ISdiscretizationPoint;  
	  }
	  cg1.update(CompositeGrid::THEinverseMap, CompositeGrid::COMPUTEnothing);
	  cg1.inverseCoordinates=0;
	  cg1.inverseGrid=-1;

	  // ogen.resetGrid(cg1);
	  if( false )
	  {
	    CompositeGrid cg3;
	    for( int grid=0; grid<cg1.numberOfGrids(); grid++ )
	    {
	      cg3.add(cg1[grid].mapping().getMapping());
	    }
            cg3.updateReferences();
	    ogen.updateOverlap(cg3);
            ps.erase();
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	    psp.set(GI_TOP_LABEL,"Refined grid cg3 ");
	    PlotIt::plot(ps,cg3,psp);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }

          ogen.updateOverlap(cg1);
	  
	  // cg1.inverseCoordinates=0;
	  //cg1.inverseGrid=-1;
          cg0=cg1;

	  // cg0.inverseCoordinates=0;
	  //cg0.inverseGrid=-1;

	}
	
//         displayMask(cg1[0].mask(),"Mask before");
	
//         setMaskAtRefinements( cg1 );
//         displayMask(cg1[0].mask(),"Mask after");
	

	if( true )
	{
          int minBoxWidth=INT_MAX, minGrid=0;
	  for( int grid=cg1.numberOfBaseGrids(); grid<cg1.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg1[grid];
            const IntegerArray & gid = mg.gridIndexRange();
            for( int axis=0; axis<cg1.numberOfDimensions(); axis++ )
	    {
              int width=gid(1,axis)-gid(0,axis)+1;
	      if( width<minBoxWidth )
	      {
		minBoxWidth=width; minGrid=grid;
	      }
	    }
	  }
          const IntegerArray & gid = cg1[minGrid].gridIndexRange();
          printF("\n ***** number of grids =%i, smallest (AMR) width = %i pts. "
                 "grid=%i, baseGrid=%i bounds=[%i,%i][%i,%i][%i,%i]\n\n",
                       cg1.numberOfComponentGrids(),minBoxWidth,minGrid,cg1.baseGridNumber(minGrid),
                       gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2));
	}
	

        aString amrFileName;
	amrFileName=sPrintF("amrGridDebugNP%i.cmd",numProc);

        printF("Saving AMR grid info to the file %s\n",(const char*)amrFileName);
        regrid.outputRefinementInfo( cg1,nameOfOGFile,amrFileName);

	amrFileName="amrBaseGrid.cmd";
        printF("Saving base grid info to the file %s\n",(const char*)amrFileName);
	outputGridInfo( cg1,nameOfOGFile,amrFileName);
	

	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	psp.set(GI_TOP_LABEL,"Refined grid");
	PlotIt::plot(ps,cg1,psp);
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	
      }
      
      if( (numberOfRefinementLevels%2) == 1 )
        cgNew=cgOld;
      

/* ------
      error.updateToMatchGrid(cgNew);

      computeErrorFunction( error, errorType );
      psp.set(GI_TOP_LABEL,"Error function");
      PlotIt::contour(ps,error,psp);

      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      ps.erase();
      psp.set(GI_TOP_LABEL,"Refined grid");
      PlotIt::plot(ps,cgNew,psp);
----- */
    }
    else
    {
      printf("unknown response\n");
    }
  }
  
  fclose(debugFile);

  Overture::finish();          
  return 0;
}


