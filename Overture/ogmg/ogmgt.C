// #define BOUNDS_CHECK
//===========================================================
//   Test Program for Ogmg: Overlapping Grid Multigrid Solver
//   --------------------------------------------------------
//===========================================================

#include "Overture.h"  
#include "Ogmg.h"
#include "CompositeGridOperators.h"
#include "PlotStuff.h"
#include "NameList.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "Integrate.h"
#include <time.h>
#include "HDF_DataBase.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"

// OGFunction *pExactSolution=NULL;
int 
getLineFromFile( FILE *file, char s[], int lim);

void 
pauseForInput(const aString & comment, real total, bool checkMemoryUsage=false)
{
//     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       cg[grid].displayComputedGeometry();

  real mem=Overture::getCurrentMemoryUsage();
  real maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
  real minMem=ParallelUtility::getMinValue(mem);  // min over all processors

  printF(" ===>%s my total = %5.1f M,  memory per-processor: [min,max]=[%g,%g] Mb \n\n",
         (const char*)comment,total/1.e6,minMem,maxMem);

//   printF(" ===>%s my total = %5.1f M, A++ array=%5.1f M A++ total=%5.1f M \n\n",
//          (const char*)comment,total/1.e6,
// 	 Diagnostic_Manager::getTotalArrayMemoryInUse()/1.e6,Diagnostic_Manager::getTotalMemoryInUse()/1.e6);

  if( checkMemoryUsage )
  {
    char buff[8];
//  cout << comment << endl;
    printF("pause: hit enter to continue\n");
  
    getLine(buff,sizeof(buff));
    printF("..continuing\n");
  }
  
}

#undef ForBoundary
#define ForBoundary(side,axis)  \
       for( int axis=0; axis<cg.numberOfDimensions(); axis++ ) \
         for( int side=0; side<=1; side++ )

static int totalNumberOfArrays=0;
static void 
checkArrays(const aString & label) 
//==============================================================================
// /Description:
// Output a warning messages if the number of arrays has increased
//\end{CompositeGridSolverInclude.tex}  
//==============================================================================
{
  if(GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
  {
    totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    printF("\n**** %s: Number of A++ arrays has increased to %i \n\n",(const char*)label,GET_NUMBER_OF_ARRAYS);
  }
}


int 
main(int argc, char *argv[])
{
  // Diagnostic_Manager::setTrackArrayData(TRUE);
  
  int checkMemoryUsage=false; // true; // false;   // set true so we can pause and look at 'top'

  Overture::start(argc,argv);  // initialize Overture
  Optimization_Manager::setForceVSG_Update(Off);

  // This macro will initialize the PETSc solver if OVERTURE_USE_PETSC is defined.
  INIT_PETSC_SOLVER();

  const int maxBuff=300;
  char buff[maxBuff];
    
  const int myid=Communication_Manager::My_Process_Number;

  int plotOption=false;
  aString commandFileName="";
  int maximumNumberOfLevels=10;
  int maximumNumberOfExtraLevels=10;
  int maximumNumberOfIterations=10;
  
  Ogmg::debug=1;
  bool checkGeometryArrays=false;
  bool runTests=false;
  int degreex=-1;  // degreex of space polynomial can be changed on the command line
  int numParallelGhost=2;

  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      //  int len=strlen(argv[i]);
      //        printF(" argv[%i]=%s len=%i\n",i,argv[i],len);
      
      line=argv[i];
      if( line=="plot" || line=="-plot" )
        plotOption=true;
      else if( line=="noplot" || line=="-noplot" )
        plotOption=false;
      else if( (len=line.matches("levels=")) || (len=line.matches("-levels=")) )
      {
        int length=line.length();
	sScanF(line(len,length-1),"%i",&maximumNumberOfLevels);
	maximumNumberOfExtraLevels=maximumNumberOfLevels-1;
        printF("maximumNumberOfExtraLevels=%i \n",maximumNumberOfExtraLevels);
      }
      else if( line=="-test" )
      {
        runTests=true;
      }
      else if( (len=line.matches("debug=")) || (len=line.matches("-debug=")) )
      {
        int length=line.length();
	sScanF(line(len,length-1),"%i",&Ogmg::debug);
        printF("Ogmg::debug=%i \n",Ogmg::debug);
      }
      else if( (len=line.matches("-degreex=")) )
      {
        int length=line.length();
	sScanF(line(len,length-1),"%i",&degreex);
        printF("Ogmg::degree of the spatial polynomial is degreex=%i \n",degreex);
      }
      else if( (len=line.matches("-numParallelGhost=")) )
      {
	sScanF(line(len,line.length()-1),"%i",&numParallelGhost);
        printF("Ogmg::number of parallel ghost lines is=%i \n",numParallelGhost);
      }
      else if( (len=line.matches("-numberOfParallelGhost=")) )
      {
	sScanF(line(len,line.length()-1),"%i",&numParallelGhost);
        printF("Ogmg::number of parallel ghost lines is=%i \n",numParallelGhost);
      }
      else if( (len=line.matches("-maxits=")) )
      {
        int length=line.length();
	sScanF(line(len,length-1),"%i",&maximumNumberOfIterations);
        printF("Ogmg::maximumNumberOfIterations=%i \n",maximumNumberOfIterations);
      }
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
  {
    printF( "Usage: `ogmgt -plot file.cmd] -levels=max number -debug= -test -numParallelGhost=<>' \n"
            "          -plot:     run with graphics \n" 
            "          -test:     run tests of smoother, coarse grid solve, coarse-to-fine etc. \n" 
            "          -degreex=<> : degree of the spatial polynomial\n"
            "          -maxits=<>  : maximum number of iterations (cycles)\n"
            "          -numParallelGhost=<>  : number of parallel ghost lines\n"
      "          file.cmd: read this command file \n" );
  }
  
  // Graphics interface:
  // *wdh* 091102 PlotStuff ps(plotOption,"ogmgt: Multigrid test routine"); 
  // The GetOptions line is processed here: 
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("ogmgt",false,argc,argv);
  PlotStuffParameters psp;

  printF(" ------------------------------------------------------------ \n");
  printF(" Test routine for the multigrid solver Ogmg                   \n");
  printF(" ------------------------------------------------------------ \n");

  // By default start saving a command file 
  aString logFile="ogmg.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file [%s]\n",(const char *)logFile);

  ps.appendToTheDefaultPrompt("ogmgt>");
  printF(" ps.graphicsIsOn()=%i isGraphicsWindowOpen()=%i\n",ps.graphicsIsOn(),ps.isGraphicsWindowOpen());
  
  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file = %s\n",(const char*)commandFileName);
    ps.readCommandFile(commandFileName);
  }

  aString nameOfOGFile="mg.hdf";
  ps.inputString(nameOfOGFile,"Enter the name of the (old) overlapping grid file:");
  
  // create and read in a CompositeGrid
  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numParallelGhost);
  #endif


  // Optionally change the min number of points per processor
  if( false )
  {
    // int minNumberPerProc[3]={21,9,7};  // default in GridDistribution.C
    // Set the min number of points per proc:
    int minNumberPerProc[3]={128,32,16};  
    // int minNumberPerProc[3]={128,64,32};  
    //   2d: 128 means smallest grid will be 128x128 = 16,384
    // int minNumberPerProc[3]={256,128,48};  
    // int minNumberPerProc[3]={1024,512,128};  
    for( int nd=1; nd<=3; nd++ )
      GridDistribution::setMinimumNumberOfPointsPerDimensionPerProcessor(nd,minNumberPerProc[nd-1]);
  }
    
  // create and read in a CompositeGrid
  bool loadBalance=true;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile,loadBalance);
  if( Communication_Manager::Number_Of_Processors >1 )
  { // display the parallel distribution
    cg.displayDistribution("ogmgt",stdout);
  }

  // cg.update(MappedGrid::THEcenter);
  cg.update(MappedGrid::THEmask);

  // Oges::debug=63;

  // Ogmg mgSolver(cg,&ps);  
  Ogmg & mgSolver = *new Ogmg;  // ********* Here is the multigrid solver ***********

  mgSolver.setGridName(nameOfOGFile);
  mgSolver.setSolverName("ogmgt");

  OgmgParameters & par = mgSolver.parameters;
  // set this before update?
  par.set(OgmgParameters::THEmaximumNumberOfExtraLevels,maximumNumberOfExtraLevels);
  par.set(OgmgParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);

  // build parameters that depend on the number of grids and levels so that we can then
  // specify the smoothers etc. 
  par.updateToMatchGrid(cg,max(1,maximumNumberOfExtraLevels));
  

  const int orderOfAccuracy = cg[0].discretizationWidth(0)==5 ? 4 : 2;
  
  real total;
  pauseForInput("Before mgSolver.updateToMatchGrid(cg)",cg.sizeOf()+mgSolver.sizeOf(),checkMemoryUsage);

  mgSolver.set(&ps);
//   mgSolver.updateToMatchGrid(cg); // this will build extra levels
  
//   pauseForInput("After mgSolver.updateToMatchGrid(cg)",cg.sizeOf()+mgSolver.sizeOf(),checkMemoryUsage);
//   if( checkGeometryArrays )
//   {
//     printF("***GEOMETRY After mgSolver.updateToMatchGrid(cg)\n");
//     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       cg[grid].displayComputedGeometry();
//   }
  
  par.setSmootherType(OgmgParameters::redBlack);  // default
  par.setResidualTolerance(1.e-10);
  par.setErrorTolerance(1.e-7);
//  par.set(OgmgParameters::THEmaximumNumberOfIterations,7);
  Range all;

//   fprintf(mgSolver.getInfoFile(),"\\clearpage\n \\section{%s}\n",(const char*)commandFileName);

//   int level;
//   printF("cg.numberOfComponentGrids()=%i, cg.numberOfGrids()=%i , cg.numberOfBaseGrids()=%i \n",
//          cg.numberOfComponentGrids(),cg.numberOfGrids(),cg.numberOfBaseGrids());
//   if( cg.numberOfMultigridLevels() > 1 )
//   {
//     for( level=0; level<cg.numberOfMultigridLevels(); level++ )
//     {
//       CompositeGrid & m =cg.multigridLevel[level];
//       printF("level=%i, numberOfComponentGrids()=%i, numberOfGrids()=%i, numberOfBaseGrids()=%i\n",level,
// 	     m.numberOfComponentGrids(),m.numberOfGrids(),m.numberOfBaseGrids());
//       m.numberOfInterpolationPoints.display(sPrintF(buff,"level=%i, numberOfInterpolationPoints",level));
      
//       // for( int grid=0; grid<m.numberOfComponentGrids(); grid++ )
//       //   m.variableInterpolationWidth[grid].display("variableInterpolationWidth");
      
//     }
//   }
  

  NameList nl;
  intArray numberOfSmooths(cg.numberOfMultigridLevels());
  numberOfSmooths=2;  


  // Here are the parameters for this test routine
  bool plotResults=FALSE;
  int twilightZone=1;  // 1=trig, 2=poly
  int printOptions = 1;  // 1:print max errors
  real fx=1., fy=1., fz=1.;
  // real fx=8.3, fy=7.8, fz=1.; 

  int numberOfComponents=1;

  // Here are the parameters for oges:
  // Default values:  *** use get ***
//  int conjugateGradientType=0;
//  int conjugateGradientPreconditioner=0;
//  int conjugateGradientNumberOfIterations=0;
//  int solverType=Oges::yale;
//  real harwellTolerance=.1;
//  int transpose=FALSE;

  enum ProblemTypes
  {
    dirichlet=OgmgParameters::dirichlet,  // this is also the value for a BC
    neumann=OgmgParameters::neumann,
    mixed=OgmgParameters::mixed,
    extrapolate=OgmgParameters::extrapolate,
    neumann2=mixed+1
  } problem = dirichlet;
  
  bool adjustSingularEquations=false;

  IntegerArray bc(2,3,cg.numberOfComponentGrids());
  bc=dirichlet;
  const int numBcData=3;
  RealArray bcData(numBcData,2,3,cg.numberOfComponentGrids());
  bcData=0.;

  aString name,answer,answer2, option="s";
	 
  aString mainMenu[]=
  {
    "!ogmgt",
    ">option",
      "solve",
      "test smoother",
      "test coarse to fine",
      "test fine to coarse",
      "test bc",
      "test coarse grid solver",
    "<>problem",
      "laplace (predefined)",
      "heat equation (predefined)",
      "divScalarGrad (predefined)", // div( s(x) grad )
      "divScalarGradHeatEquation (predefined)",  // I + div( s(x) grad )
      "laplace",
      "heat equation",
      "divScalarGrad", // div( s(x) grad )
      "divScalarGradHeatEquation",  // I + div( s(x) grad )
      "dirichlet",
      "neumann",
      "mixed",
      "neumann2",
    "<bc(side,axis,grid)=[1=dirichlet][2=neumann][3=mixed]",
    "bcNumber<num>=[d|n|m|e]",
    "change parameters",
    ">twilight zone",
      "turn off twilight zone",
      "turn on trigonometric",
      "set trigonometric frequencies",
      "turn on polynomial",
      "set exact initial conditions",
    "<debug",
    "adjust singular equations",
    "nuDt",
// too late    "order of accuracy",
    "exit",
    ""
  };
  

  mgSolver.parameters.updateToMatchGrid(cg);  // ***
  bool solvePredefined=false;

  
  OgesParameters::EquationEnum equationToSolve=OgesParameters::laplaceEquation; 
  RealArray equationCoefficients(2,cg.numberOfComponentGrids());
  Range G=cg.numberOfComponentGrids();
  real nuDt=.1;  // for heat equation solve I - nuDt* Delta

  bool initialConditionsExact=false;
  //  equationCoefficients(1,0)=0.;  // Grid 0 has the identity operator
  

  // ==========Loop for changing parameters========================
  int len=0;
  for( ;; ) 
  {
    ps.getMenuItem(mainMenu,answer,"change a parameter");
    if( answer=="exit" ) break;

    if( answer=="solve" )
      option=answer;
    else if( answer=="test smoother" )
      option="smooth";
    else if( answer=="test coarse to fine" )
      option="cf";
    else if( answer=="test fine to coarse" )
      option="fc";
    else if( answer=="test bc" )
      option="bc";
    else if( answer=="test coarse grid solver" )
      option="coarseGrid";
    else if( answer=="dirichlet" )
    {
      problem=dirichlet;
    }
    else if( answer=="neumann" )
    {
      problem=neumann;
      bc=neumann;
    }
    else if( answer=="neumann2" )
    {
      problem=neumann2;
      bc=neumann;
    }
    else if( answer=="laplace (predefined)" )
    {
      equationToSolve=OgesParameters::laplaceEquation;
      solvePredefined=true;
    }
    else if( answer=="heat equation (predefined)" )
    {
      equationToSolve=OgesParameters::heatEquationOperator;
      solvePredefined=true;
      printF("Use the heat equation operator I - Delta\n");
    }
    else if( answer=="divScalarGrad (predefined)" )
    {
      equationToSolve=OgesParameters::divScalarGradOperator;
      solvePredefined=true;
      printF("Use the div( s(x) grad ) operator\n");
    }
    else if( answer=="divScalarGradHeatEquation (predefined)" )
    {
      equationToSolve=OgesParameters::divScalarGradHeatEquationOperator;
      solvePredefined=true;
      printF("Use the I + div( s(x) grad ) operator\n");
    }
    else if( answer=="laplace" )
    {
      equationToSolve=OgesParameters::laplaceEquation;
      solvePredefined=false;
    }
    else if( answer=="heat equation" )
    {
      equationToSolve=OgesParameters::heatEquationOperator;
      solvePredefined=false;
      printF("Use the heat equation operator I - Delta\n");
    }
    else if( answer=="divScalarGrad" )
    {
      equationToSolve=OgesParameters::divScalarGradOperator;
      solvePredefined=false;
      printF("Use the div( s(x) grad ) operator\n");
    }
    else if( answer=="divScalarGradHeatEquation" )
    {
      equationToSolve=OgesParameters::divScalarGradHeatEquationOperator;
      solvePredefined=false;
      printF("Use the I + div( s(x) grad ) operator\n");
    }
    else if( answer=="bc=neumann" )
    {
      bc=neumann;
    }
    else if( answer=="adjust singular equations" )
    {
      adjustSingularEquations=true;
      printF("adjustSingularEquations=true\n");
    }
    else if( (len=answer.matches("bcNumber")) )
    {
      // BC of the form
      //    bcNumber3=[d|n|m|e]
      int bcNumber=-1;
      sScanF(answer(len,answer.length()-1),"%i",&bcNumber);
      // The bc name follows an '=' :   
      int length=answer.length();
      while( len<length-1 && answer[len]!='=') len++;
      aString bcName=answer(len+1,length-1);
      printF("setting BC number %i to be %s\n",bcNumber,(const char*)bcName);
      int bcType=dirichlet;
      if( bcName=="d" )
        bcType=dirichlet;
      else if( bcName=="n" )
	bcType=neumann;
      else if( bcName=="m" )
	bcType=mixed;
      else if( bcName=="e" )
	bcType=extrapolate;
      else 
      {
	printF("ERROR: unknown bcName=[%s]. Will set to dirichlet\n",(const char*)bcName);
	bcName="d";
        bcType=dirichlet;
      }
      
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	ForBoundary(side,axis) 
	{
	  if( cg[grid].boundaryCondition(side,axis)==bcNumber )
	  {
	    bc(side,axis,grid)=bcType;
	    printF(" Setting bc=%s for grid =%i (%s) (side,axis)=(%i,%i)\n",(const char*)bcName,grid,
		   (const char*)cg[grid].getName(),side,axis);
	  }
	}
      }
    }
    else if( answer(0,1)=="bc" )
    {
      nl.getIntArray( answer,bc );
    }
    else if( answer=="mixed" )
    {
      problem=mixed;
    }
    else if( answer=="change parameters" )
    {
      mgSolver.update(ps,cg);
    }
    else if( answer=="turn off twilight zone" )
      twilightZone=0;
    else if( answer=="turn on trigonometric" )
      twilightZone=1;
    else if( answer=="turn on polynomial" )
      twilightZone=2;
    else if( answer=="set trigonometric frequencies" )
    {
      ps.inputString(answer2,sPrintF(buff,"Enter fx,fy,fz (default =%f,%f,%f)",fx,fy,fz));
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e",&fx,&fy,&fz);
        printF(" fx=%f, fy=%f, fz=%f\n",fx,fy,fz);
      }
    }
    else if( answer=="set exact initial conditions" )
    {
      initialConditionsExact=true;
    }
    else if( answer=="debug" )
    {
      ps.inputString(answer2,sPrintF(buff,"Enter the debug parameter (default =%i)",Ogmg::debug));
      if( answer2!="" )
      {
	sScanF(answer2,"%i",&Ogmg::debug);
        printF(" Ogmg::debug=%i\n",Ogmg::debug);
      }
    } 
//      else if( answer=="order of accuracy" )
//      {
//        ps.inputString(answer2,sPrintF(buff,"Enter the order of accuracy (2/4) (default =%i)",orderOfAccuracy));
//        if( answer2!="" )
//        {
//  	sScanF(answer2,"%i",&orderOfAccuracy);
//          mgSolver.setOrderOfAccuracy(orderOfAccuracy);
//        }
//      }
    else if( (len=answer.matches("nuDt")) )
    {
      sScanF(answer(len,answer.length()-1),"%e",&nuDt);
      printF("Setting nuDt=%9.2e for the heat equation option\n",nuDt);
    }
    else
    {
      printF("unknown response: [%s]\n",(const char*)answer);
      ps.stopReadingCommandFile();
    }
    printF("---- solvePredefined=%i\n",(int)solvePredefined);
  }
  
  printF("............... solvePredefined=%i\n",(int)solvePredefined);

  equationCoefficients(0,G)= 1.;  
  equationCoefficients(1,G)=-nuDt;

  // Now build the multigrid levels
  printF("\n >>>>> Build the multigrid levels ...\n");
  mgSolver.updateToMatchGrid(cg); // this will build extra levels

  // mgSolver.displaySmoothers("After mgSolver.updateToMatchGrid(cg)");
  
  pauseForInput("After mgSolver.updateToMatchGrid(cg)",cg.sizeOf()+mgSolver.sizeOf(),checkMemoryUsage);
  if( checkGeometryArrays )
  {
    printF("***GEOMETRY After mgSolver.updateToMatchGrid(cg)\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }
  fPrintF(mgSolver.getInfoFile(),"\\clearpage\n \\section{%s}\n",(const char*)commandFileName);

  int level;
  if( false )
  {
    printF("cg.numberOfComponentGrids()=%i, cg.numberOfGrids()=%i , cg.numberOfBaseGrids()=%i \n",
	   cg.numberOfComponentGrids(),cg.numberOfGrids(),cg.numberOfBaseGrids());
    if( cg.numberOfMultigridLevels() > 1 )
    {
      for( level=0; level<cg.numberOfMultigridLevels(); level++ )
      {
	CompositeGrid & m =cg.multigridLevel[level];
	printF("level=%i, numberOfComponentGrids()=%i, numberOfGrids()=%i, numberOfBaseGrids()=%i\n",level,
	       m.numberOfComponentGrids(),m.numberOfGrids(),m.numberOfBaseGrids());
	m.numberOfInterpolationPoints.display(sPrintF(buff,"level=%i, numberOfInterpolationPoints",level));
      
	// for( int grid=0; grid<m.numberOfComponentGrids(); grid++ )
	//   m.variableInterpolationWidth[grid].display("variableInterpolationWidth");
      
      }
    }
  }


  // assign BC's for the problem type
  bool problemIsSingular=true;
  if( adjustSingularEquations ) 
    problemIsSingular=false;
  
  CompositeGrid & mgcg = mgSolver.getCompositeGrid();
  for( level=-1; level<mgcg.numberOfMultigridLevels(); level++ )
  {
    // CompositeGrid & m =level==-1 ? mgcg : mgcg.multigridLevel[level];
    // If we read in an existing MG grid with levels then cg is currently not referenced to mgcg[0]
    CompositeGrid & m =level==-1 ? cg : mgcg.multigridLevel[level];
    for( int grid=0; grid<m.numberOfComponentGrids(); grid++ )  
    {
      ForBoundary(side,axis)
      {
	if( m[grid].boundaryCondition(side,axis) > 0 )
	{
          m[grid].boundaryCondition()(side,axis)=bc(side,axis,grid);
	  if( m[grid].boundaryCondition(side,axis)!=neumann )
	    problemIsSingular=false;
	}
	
      }
      if( Ogmg::debug & 8 && myid==0 )
        display(m[grid].boundaryCondition(),
               sPrintF(buff,"level=%i boundary condition on grid=%i, dirichlet=%i neumann=%i",
		       level,grid,dirichlet,neumann));
    }
  }
  if( Ogmg::debug & 8 && myid==0 )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ ) 
    {
      display(cg[grid].boundaryCondition(),
	      sPrintF(buff,"cg[grid]: boundary condition on grid=%i, dirichlet=%i neumann=%i",
		      grid,dirichlet,neumann));
    }
  }
  
  if( problemIsSingular )
  {
    printF(" ** mgSolver.setProblemIsSingular(true); ** \n");
    par.setProblemIsSingular(true);
  }

  CompositeGridOperators cgop(cg);
  const int width = orderOfAccuracy+1;  // 3 or 5
  const int stencilSize=int(pow(width,cg.numberOfDimensions())+1);
  cgop.setStencilSize( stencilSize );
  cgop.setOrderOfAccuracy(orderOfAccuracy);

  total=cg.sizeOf()+mgSolver.sizeOf();
  pauseForInput("Before build coeff",cg.sizeOf()+mgSolver.sizeOf());


  if( twilightZone==1 )
     fPrintF(mgSolver.getInfoFile(),"%% trigonometric TZ, fx=%9.3e, fy=%9.3e, fz=%9.3e\n",fx,fy,fz);
  else if( twilightZone==2 )
     fPrintF(mgSolver.getInfoFile(),"%% polynomial TZ\n");

  real time0=getCPU();

  // Build coefficient matrices for each multigrid level
  realCompositeGridFunction coeff;
  realCompositeGridFunction *variableCoefficients=NULL;
  
  if( !solvePredefined )
  {
    //  coeff.setDataAllocationOption(1);  // do not allocate on rectangular grids
    coeff.updateToMatchGrid(cg,stencilSize,all,all,all);
    coeff=0.;
    total=coeff.sizeOf()+cg.sizeOf()+mgSolver.sizeOf();
    pauseForInput("After declare coeff",total);
  }

  // These are used to adjust an equation for singular problems:
  int i1s, i2s, i3s, grids, mDiag;
  real alphas;
  bool setDirichlet=false; // true;
  if( cg.numberOfDimensions()==2 )
  {
    i1s=10, i2s=10, i3s=0;  // do this for now ---
    mDiag = (width*width/2);
  }
  else
  {
    i1s=5, i2s=5, i3s=5;
    mDiag = (width*width*width/2);
  }

  if( !solvePredefined )
  {
    const int numberOfGhostLines=orderOfAccuracy/2; // *wdh* 2013/11/30
    coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
    coeff.setOperators(cgop);

    pauseForInput("After declare cgop",cgop.sizeOf()+coeff.sizeOf()+cg.sizeOf()+mgSolver.sizeOf(),checkMemoryUsage);

    // *note* the next assigments apply to all grids on all levels
    // printF(" cg.numberOfComponentGrids()=%i\n",cg.numberOfComponentGrids());

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      cgop[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid]); // efficient version

      if( equationToSolve==OgesParameters::laplaceEquation )
      {
	if( adjustSingularEquations && grid==grids )
	{
	  // Change the equation at one point:
	  //     Delta(u) + alphas*u = f + alphas*uExact

	  alphas = coeff[grids](mDiag,i1s,i2s,i3s);

	  printF("\n *** Adjust the singular problem at grid=%i, pt (i1,i2,i3)=(%i,%i,%i) mDiag=%i "
		 " alphas=%e (setDirichlet=%i) ***\n\n",
		 grids,i1s,i2s,i3s,mDiag,alphas,(int)setDirichlet);
	  
	
	  if( !setDirichlet )
	  {
	    // Adjust the equation: 
	    coeff[grids](mDiag,i1s,i2s,i3s) += alphas; // change the diagonal
	  }
	  else
	  { // set a dirichlet equation at one point
	    coeff[grids](all,i1s,i2s,i3s)=0.;
	    coeff[grids](mDiag,i1s,i2s,i3s) = alphas;
	  }

	}
	
      }
      else if( equationToSolve==OgesParameters::heatEquationOperator )
      {
        // equationCoefficients(0,grid)*ue+equationCoefficients(1,grid)*uLap;

        #ifdef USE_PPP
  	  realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff[grid],coeffLocal);
        #else
  	  realSerialArray & coeffLocal = coeff[grid];
        #endif


	coeffLocal *= equationCoefficients(1,grid);

	int md; // diagonal term
	if( cg.numberOfDimensions()==2 )
	  md=(width*width)/2; // 4 or 12 ;
	else if( cg.numberOfDimensions()==3 )
	  md=(width*width*width)/2; // 13 or 62;
	else
	  md=width/2; // 1;

        coeffLocal(md,all,all,all) += equationCoefficients(0,grid);
	
      }
      else
      {
	printF("ogmgt:ERROR: equationToSolve=%i not defined yet for non-predefined!\n",(int)equationToSolve);
	OV_ABORT("error");
      }
      

    }
    


    total=cgop.sizeOf()+coeff.sizeOf()+cg.sizeOf()+mgSolver.sizeOf();
    // printF(" ***coeff.sizeOf()=%12.0f\n",coeff.sizeOf());
    pauseForInput("After assign laplacianCoefficients:",total,checkMemoryUsage);
  }

  BoundaryConditionParameters bcParams;
  RealArray & a = bcParams.a;
  a.redim(2);
  a(0)=1.; a(1)=1.;  // coefficients of any mixed boundary conditions: a0 + a1*u.n 
  // a(0)=1.e-5;
   
  real timeForSettingUpCoefficients=0.;
  const int orderOfExtrapolation = orderOfAccuracy==2 ? 3 : 4; 
  
  // For now the "extrapolation" BC only works for order=2 : 
  const int extrapolationBoundaryOrderOfExtrapolation = 2; 

  // --- Fill in the bcData array ---
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    for( int side=0; side<=1; side++ )for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      if( cg[grid].boundaryCondition(side,axis)==mixed ||
          cg[grid].boundaryCondition(side,axis)==neumann )
      {
	if( !cg[grid].isRectangular() )
	{
	  cg[grid].update(MappedGrid::THEvertexBoundaryNormal);
	}
      }
      

      if( cg[grid].boundaryCondition(side,axis)==mixed)
      { // set the coefficients of the mixed BC: a(0)*u + a(1)*u.n
	bcData(0,side,axis,grid)=a(0);
	bcData(1,side,axis,grid)=a(1);
      }
      else if( cg[grid].boundaryCondition(side,axis)==extrapolate )
      {
	bcData(0,side,axis,grid)=extrapolationBoundaryOrderOfExtrapolation;  // supply the order of extrapolation
      }
	
    }
  }

  if( !solvePredefined )
  {
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,dirichlet);
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,dirichlet);
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,extrapolate);
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,neumann);
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,mixed,bcParams);
    
    if( orderOfAccuracy==4 )
    {
      BoundaryConditionParameters extrapParams;
      extrapParams.ghostLineToAssign=2;
      extrapParams.orderOfExtrapolation=orderOfExtrapolation; // orderOfAccuracy+1;
      coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries,extrapParams); // extrap 2nd ghost line
    }

    if( cg.numberOfMultigridLevels()>1 )
    {
      printF("*******ogmgt:  coeff.finishBoundaryConditions();\n");
      coeff.finishBoundaryConditions();
    }
    else
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        coeff[grid].updateGhostBoundaries();  // *wdh* 100605
      }
      

      // if we build extra levels we do not call finish boundary conditions.
      printF("*******ogmgt: DO NOT CALL coeff.finishBoundaryConditions();\n");
    }
    
    total=cgop.sizeOf()+coeff.sizeOf()+cg.sizeOf()+mgSolver.sizeOf();
    pauseForInput("After assign BC's",total,checkMemoryUsage);
    printF("...done\n");

    if( Ogmg::debug & 32 && cg.numberOfMultigridLevels()>1 )
      coeff.multigridLevel[cg.numberOfMultigridLevels()-1].display("ogmgt:: here is the coeff array on the coarse grid");

    timeForSettingUpCoefficients=getCPU()-time0;

    // this next call will also set boundary conditions
    mgSolver.setCoefficientArray(coeff,bc,bcData);  

    //::display(mgSolver.boundaryCondition,"ogmgt: mgSolver.boundaryCondition: after setCoefficientArray");
    
    total=coeff.sizeOf()+cgop.sizeOf()+cg.sizeOf()+mgSolver.sizeOf();
    pauseForInput("After mgSolver.setCoefficientArray(coeff)",total,checkMemoryUsage);
  }
  else
  {
    if( checkGeometryArrays )
    {
      printF("***GEOMETRY before buildPredefinedEquations\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	cg[grid].displayComputedGeometry();
    }
    
//     int numData=3;
//     RealArray bcData(numData,2,3,cg.numberOfComponentGrids());
//     bcData(0,all,all,all)=1.;  // for mixed bc: a(0)*u + a(1)*u.n
//     bcData(1,all,all,all)=1.;
    
    if( equationToSolve==OgesParameters::divScalarGradOperator ||
        equationToSolve==OgesParameters::variableHeatEquationOperator ||
	equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
    {
      variableCoefficients = new realCompositeGridFunction(cg);

      *variableCoefficients=1.;   // use this for now.
    }
    mgSolver.setEquationAndBoundaryConditions(equationToSolve, cgop, bc, bcData, equationCoefficients,
                          variableCoefficients );

  }
  

  realCompositeGridFunction u(cg), f(cg);
//  u.setOperators(cgop);  
  
  #ifdef USE_PPP
  for( int grid=0; grid<cg.numberOfGrids(); grid++ )
  {
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
    uLocal=0.;
  }
  #else
  u=0.;   // initial guess
  #endif

  // --- adjust the trig frequencies for a derivative periodic grid ---
  // --> MG will not converge if the solution is not periodic too
  int gridp=0;  // assume grid=0 is the periodic one
  for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
  {
    if( cg[gridp].isPeriodic(axis)==Mapping::derivativePeriodic )
    {
      // --- compute length along axis
      real length=-1.;
      MappedGrid & mg = cg[gridp];
      if( mg.isRectangular() )
      {
        real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
        mg.getRectangularGridParameters( dvx, xab );
        length=xab[1][axis]-xab[0][axis];
      }
      else
      {
        Mapping & map = mg.mapping().getMapping();
	RealArray r(2,3),x(2,3);
	r=0.; x=0.;
	r(1,axis)=1.;
	map.mapS(r,x);
	length = sqrt( SQR(x(1,0)-x(0,0)) + SQR(x(1,1)-x(0,1))+ SQR(x(1,2)-x(0,2)) );
      }
      assert( length>0. );

      fx=max(2.,fx)/length;
      fy=max(2.,fy)/length;
      fz=max(2.,fz)/length;
      
      printF("ogmgt:INFO: axis=%i is derivative periodic with length=%9.3e,\n"
             " I will adjust the trig function to match: fx=%9.3e, fy=%9.3e, fz=%9.3e.\n",axis,length,
	     fx,fy,fz);
      
      if( twilightZone==2 )
      {
	printF("ogmgt:ERROR: polynomial TZ function will not work with derivative periodic grid\n");
	OV_ABORT("error");
      }
      
    }
  }
  

  OGTrigFunction tzTrig(fx,fy,fz);  // create an exact solution

  int degreeOfSpacePolynomial=degreex>=0 ? degreex : orderOfAccuracy;
  
  OGPolyFunction tzPoly(degreeOfSpacePolynomial,cg.numberOfDimensions());      // create an exact solution
  OGFunction & tz = twilightZone==1 ? (OGFunction&)tzTrig : (OGFunction&)tzPoly;

  if( twilightZone!=0 )
    mgSolver.pExactSolution=&tz;  // For debugging -- this pointer may be used by ogmg to get the exact solution

  
  RealArray spatialCoefficientsForTZ(5,5,5,numberOfComponents);  
  spatialCoefficientsForTZ=0.;
  RealArray timeCoefficientsForTZ(5,numberOfComponents);      
  timeCoefficientsForTZ=0.;
  timeCoefficientsForTZ(0,0)=1.;
  if( degreeOfSpacePolynomial==0 )
  {
    spatialCoefficientsForTZ(0,0,0)=1.; 
  }
  else if( degreeOfSpacePolynomial==1 )
  {
    spatialCoefficientsForTZ(1,0,0)=1.; 
    spatialCoefficientsForTZ(0,1,0)=1.;
    if( cg.numberOfDimensions()==3 )
      spatialCoefficientsForTZ(0,0,1)=-.25;
  }
  else if( degreeOfSpacePolynomial==2 )
  {
    spatialCoefficientsForTZ(1,0,0)=-.5;
    spatialCoefficientsForTZ(0,1,0)=-.5;
    spatialCoefficientsForTZ(2,0,0)=1.;  // x^2
    spatialCoefficientsForTZ(0,2,0)=1.;
    if( cg.numberOfDimensions()==3 )
    {
      spatialCoefficientsForTZ(0,0,1)=-.5;
      spatialCoefficientsForTZ(0,0,2)=.5;
    }
  }
  else if( degreeOfSpacePolynomial==3 )
  {
    spatialCoefficientsForTZ(1,0,0)=-.5;
    spatialCoefficientsForTZ(0,1,0)=-.5;
    spatialCoefficientsForTZ(2,0,0)=1.;  // x^2
    spatialCoefficientsForTZ(0,2,0)=1.;

    spatialCoefficientsForTZ(3,0,0)=.7;  // x^3
    spatialCoefficientsForTZ(0,3,0)=.9;
    spatialCoefficientsForTZ(2,1,0)=-.5;
    if( cg.numberOfDimensions()==3 )
    {
      spatialCoefficientsForTZ(0,0,1)=-.5;
      spatialCoefficientsForTZ(0,0,2)=.5;
      spatialCoefficientsForTZ(0,0,3)=.8;
    }
  }
  else if( degreeOfSpacePolynomial==4 )
  {
    if( true)
    {
      // here is a poly that is symmetric in x-y
      spatialCoefficientsForTZ(1,0,0)= .5;
      spatialCoefficientsForTZ(0,1,0)= .5;

      spatialCoefficientsForTZ(2,0,0)= .3;  // x^2
      spatialCoefficientsForTZ(0,2,0)= .3;

      spatialCoefficientsForTZ(3,1,0)= .1;  // x^2
      spatialCoefficientsForTZ(1,3,0)= .1;

      spatialCoefficientsForTZ(4,0,0)=.2;  // x^4
      spatialCoefficientsForTZ(0,4,0)=.2;  // y^4
      spatialCoefficientsForTZ(2,2,0)=-.1;  // x^2 y^2

      if( cg.numberOfDimensions()==3 )
      {
	spatialCoefficientsForTZ(0,0,1)= .6;   // z 
	spatialCoefficientsForTZ(0,0,2)=-.3;   // z^2
	spatialCoefficientsForTZ(0,0,4)= .1;   // z^4

      }
    }
    else
    {
      spatialCoefficientsForTZ(1,0,0)=-.5;
      spatialCoefficientsForTZ(0,1,0)= .5;

      spatialCoefficientsForTZ(2,0,0)= .3;  // x^2
      spatialCoefficientsForTZ(0,2,0)=-.3;

      spatialCoefficientsForTZ(3,1,0)= .1;  // x^2
      spatialCoefficientsForTZ(1,3,0)=-.1;

      spatialCoefficientsForTZ(4,0,0)=.2;  // x^4
      spatialCoefficientsForTZ(0,4,0)=.1;  // y^4
      spatialCoefficientsForTZ(2,2,0)=-.1;  // x^2 y^2

      if( cg.numberOfDimensions()==3 )
      {
	spatialCoefficientsForTZ(0,0,1)= .6;
	spatialCoefficientsForTZ(0,0,2)=-.3;
	spatialCoefficientsForTZ(0,0,4)= .1;

      }
    }

  }
  else
  {
    spatialCoefficientsForTZ(0,0,0)=1.; 
  }
    
  tzPoly.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  


  aString bcLabel="BC: ";
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
        if( mg.boundaryCondition(side,axis) > 0 )
	{
          // bc(side,axis,grid)=[1=dirichlet][2=neumann][3=mixed]
          if( bc(side,axis,grid)==dirichlet )
	  {
	    bcLabel+="D";
	  }
	  else if( bc(side,axis,grid)==neumann )
	  {
	    bcLabel+="N";
	  }
	  else
	  {
	    bcLabel+="M";
	  }
	}
	else if( mg.boundaryCondition(side,axis)==0 )
	{
          bcLabel+="I";   // interpolation
	}
	else
	{
          bcLabel+="P";   // periodic
	}
      }
    }
    if( grid<cg.numberOfComponentGrids()-1 )
      bcLabel+="+";
    if( bcLabel.length()>20 )
    {
      // label is getting too long
      bcLabel+="...";  
      break;
    }
  }
  

  // Assign the captions for the table in the info file.
  time_t *tp= new time_t;
  time(tp);
  tm *ptm=localtime(tp);

  sPrintF(Ogmg::infoFileCaption[0],"Grid: %s. %2.2i/%2.2i/%2.2i",(const char*)nameOfOGFile,
           1900+ptm->tm_year-2000,ptm->tm_mon+1,ptm->tm_mday);
  delete tp;
  
  sPrintF(Ogmg::infoFileCaption[1],"%s.",(const char*)bcLabel);
  Ogmg::infoFileCaption[2]=orderOfAccuracy==2 ? "Second-order accurate." : "Fourth-order accurate.";
  sPrintF(Ogmg::infoFileCaption[3],"%s.",
	  (twilightZone==1 ? "Trigonometric solution" :
	   twilightZone==2 ? "Polynomial solution" : "Constant forcing"));

  real cpu0=getCPU();
  if( Ogmg::debug & 2 )
    printF(" ogmgt: assign the RHS...\n");
  Index I1,I2,I3;
  if( twilightZone )
  {
    cg.update(MappedGrid::THEcenter | MappedGrid::THEvertex);

//    cgop.setTwilightZoneFlow(TRUE);           // this will set twilight-zone flow for level 0 only
//    cgop.setTwilightZoneFlowFunction(tz);

    if( problemIsSingular )  // **** turn off for now
    { // set mean value for a singular problem 
      if( u.numberOfMultigridLevels()>1 )
        tz.assignGridFunction( u.multigridLevel[0] );
      else
        tz.assignGridFunction( u );
      par.setMeanValueForSingularProblem( mgSolver.getMean(u));
      if( u.numberOfMultigridLevels()>1 )
        u.multigridLevel[0]=0.;  // reset to zero
      else
        u=0.;
    }
    
    if( initialConditionsExact )
    { // start with the exact initial conditions
      printF("** Ogmg: setting the initial conditions to be exact\n");
      tz.assignGridFunction( u );
    }
    


    // assign the rhs for all the equations (including BC's)
    realCompositeGridFunction & ff = cg.numberOfMultigridLevels()>1 ? f.multigridLevel[0] : f;
    CompositeGrid & c = cg.numberOfMultigridLevels()>1 ? cg.multigridLevel[0] : cg;    
    bool ok;
    for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
    {
      getIndex(c[grid].dimension(),I1,I2,I3);
      #ifdef USE_PPP
	realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	realSerialArray ffLocal; getLocalArrayWithGhostBoundaries(ff[grid],ffLocal);
	realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(c[grid].center(),xLocal);

	ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
	if( !ok ) continue;
	  
      #else
	realSerialArray & uLocal = u[grid];
	realSerialArray & ffLocal = ff[grid];
	const realSerialArray & xLocal = c[grid].center();
      #endif

      const bool isRectangular=false;  // do this for now
      realSerialArray ue(I1,I2,I3), uLap(I1,I2,I3); 
      int ntd=0, nxd=2, nyd=0, nzd=0; 
      tz.gd( uLap,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.xx
      nxd=0; nyd=2; nzd=0;
      tz.gd( ue  ,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.yy 
      uLap+=ue;
      if( c.numberOfDimensions()==3 )
      {
	nxd=0; nyd=0; nzd=2;
	tz.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.zz
	uLap+=ue;
      }
      nxd=0; nyd=0; nzd=0;
      tz.gd( ue,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.);	 // e
      
      if( equationToSolve==OgesParameters::heatEquationOperator )
      {
        ffLocal(I1,I2,I3)=equationCoefficients(0,grid)*ue+equationCoefficients(1,grid)*uLap;
      }
      else
      {
        ffLocal(I1,I2,I3)=uLap;
      }
      

      if( false )
        ff[grid]=1.;
      
      if( false && problemIsSingular )
      {
	f+=1.;   // for testing
      }
      
      if( adjustSingularEquations && grid==grids )
      {
        #ifdef USE_PPP
	  OV_ABORT("finish me");
        #endif
        real xs = xLocal(i1s,i2s,i3s,0), ys=xLocal(i1s,i2s,i3s,1);
	real zs = cg.numberOfDimensions()==2 ? 0. : xLocal(i1s,i2s,i3s,2);
	
        real ue = tz(xs,ys,zs,0,0.);
	if( !setDirichlet )
	  ffLocal(i1s,i2s,i3s) += alphas*ue;
	else	
	  ffLocal(i1s,i2s,i3s) = alphas*ue;
      }
      

      // Assign Boundary Conditions 
      ForBoundary(side,axis)
      {
	if( c[grid].boundaryCondition(side,axis)<=0 ) 
          continue;
	
        Index I1b,I2b,I3b,I1g,I2g,I3g;
	if( c[grid].boundaryCondition(side,axis)==dirichlet )
	{
          int extra=orderOfAccuracy/2;
	  getBoundaryIndex(c[grid].gridIndexRange(),side,axis,I1b,I2b,I3b,extra);
	  getGhostIndex   (c[grid].gridIndexRange(),side,axis,I1g,I2g,I3g,1,extra);

	  ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1b,I2b,I3b,1);
	  ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1g,I2g,I3g,1);
	  if( !ok ) continue;

	  ffLocal(I1b,I2b,I3b)=ue(I1b,I2b,I3b);
	  uLocal(I1b,I2b,I3b)=ffLocal(I1b,I2b,I3b);    // added 030330
	  
          if( orderOfAccuracy==2 )
	  {
  	    ffLocal(I1g,I2g,I3g)=0.; // extrap
	  }
          else
	  {
            // for 4th order we fill in the eqn at the bndry as the rhs at the ghost point
	    if( equationToSolve==OgesParameters::heatEquationOperator )
	      ffLocal(I1g,I2g,I3g)=equationCoefficients(0,grid)*ue(I1b,I2b,I3b)+
 		                   equationCoefficients(1,grid)*uLap(I1b,I2b,I3b);
            else
	      ffLocal(I1g,I2g,I3g)=uLap(I1b,I2b,I3b); // tz.laplacian(cg[grid],I1b,I2b,I3b);
	  }
	}
	else if( c[grid].boundaryCondition(side,axis)==extrapolate )
	{
          int extra=orderOfAccuracy/2;
	  getGhostIndex(c[grid].gridIndexRange(),side,axis,I1g,I2g,I3g,1,extra);
	  ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1g,I2g,I3g,1);
	  if( !ok ) continue;

          ffLocal(I1g,I2g,I3g)=0.; // extrap
          if( orderOfAccuracy==4 )
	  {
            // what should we do here ??
	    getGhostIndex(c[grid].gridIndexRange(),side,axis,I1g,I2g,I3g,2,extra);  // 2nd ghost line 
	    ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1g,I2g,I3g,1);
	    if( !ok ) continue;
	    ffLocal(I1g,I2g,I3g)=0.;

	  }
	}
        else if( c[grid].boundaryCondition(side,axis)==neumann || c[grid].boundaryCondition(side,axis)==mixed )
	{
	  getBoundaryIndex(c[grid].gridIndexRange(),side,axis,I1b,I2b,I3b);
	  getGhostIndex   (c[grid].gridIndexRange(),side,axis,I1g,I2g,I3g);

	  ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1b,I2b,I3b,1);
	  ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1g,I2g,I3g,1);
	  if( !ok ) continue;

	  realSerialArray uex(I1b,I2b,I3b), uey(I1b,I2b,I3b), uez;
	  nxd=1; nyd=0; nzd=0;
          tz.gd( uex,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.x
	  nxd=0; nyd=1; nzd=0;
          tz.gd( uey,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.y
	  if( c.numberOfDimensions()==3 )
	  {
	    uez.redim(I1b,I2b,I3b);
	    nxd=0; nyd=0; nzd=1;
	    tz.gd( uez,xLocal,cg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.zz
	  }
          // ***** neumann BC *****
          if( c[grid].isRectangular() )
	  {
	    if( axis==axis1 )
	      ffLocal(I1g,I2g,I3g)=uex(I1b,I2b,I3b)*(2*side-1.); // normal is 2*side-1
	    else if( axis==axis2 )
	      ffLocal(I1g,I2g,I3g)=uey(I1b,I2b,I3b)*(2*side-1.);
            else
	      ffLocal(I1g,I2g,I3g)=uez(I1b,I2b,I3b)*(2*side-1.);
	  }
	  else
	  {
	    c[grid].update(MappedGrid::THEvertexBoundaryNormal );
            #ifdef USE_PPP
              const realSerialArray & normal = c[grid].vertexBoundaryNormalArray(side,axis);
            #else
              const realSerialArray & normal = c[grid].vertexBoundaryNormal(side,axis);
            #endif

	    if( c.numberOfDimensions()==2 )
	      ffLocal(I1g,I2g,I3g)=(normal(I1b,I2b,I3b,0)*uex(I1b,I2b,I3b)+
				    normal(I1b,I2b,I3b,1)*uey(I1b,I2b,I3b));
	    else 
	      ffLocal(I1g,I2g,I3g)=(normal(I1b,I2b,I3b,0)*uex(I1b,I2b,I3b)+
				    normal(I1b,I2b,I3b,1)*uey(I1b,I2b,I3b)+
				    normal(I1b,I2b,I3b,2)*uez(I1b,I2b,I3b));
	  }
	  
          if( c[grid].boundaryCondition(side,axis)==mixed )
	  {
	    ffLocal(I1g,I2g,I3g)=bcParams.a(0)*ue(I1b,I2b,I3b)+bcParams.a(1)*ffLocal(I1g,I2g,I3g);
	  }

	  if( orderOfAccuracy==4 )
	  { // *wdh* 2013/11/29 -- set 2nd ghost to zero
	    printF("ogmgt:INFO: set f at 2nd ghost line to zero...\n");
	    
	    getGhostIndex(c[grid].gridIndexRange(),side,axis,I1g,I2g,I3g,2); // 2nd ghost line

            ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1g,I2g,I3g,1);
	    if( !ok ) continue;
	    ffLocal(I1g,I2g,I3g)=0.;
	    
	  }
	  


	  if( false && orderOfAccuracy==4 ) // This is not needed anymore
	  {
	    getGhostIndex(c[grid].gridIndexRange(),side,axis,I1g,I2g,I3g,2); // 2nd ghost line

            const real a0=bcParams.a(0), a1=bcParams.a(1), nsign=2*side-1.;
            
            if( c.numberOfDimensions()==2 && c[grid].isRectangular() )
	    {
	      if( axis==axis1 )
	      {
		realArray uxxx; uxxx = tz.gd(0,3,0,0,c[grid],I1b,I2b,I3b);
		realArray uxyy; uxyy = tz.gd(0,1,2,0,c[grid],I1b,I2b,I3b);
		ff[grid](I1g,I2g,I3g)=a0*f[grid](I1b,I2b,I3b)+a1*nsign*( uxxx+uxyy ) - nsign*uxyy;
	      }
	      else if( axis==axis2 )
	      {
		realArray uxxy; uxxy = tz.gd(0,2,1,0,c[grid],I1b,I2b,I3b);
		realArray uyyy; uyyy = tz.gd(0,0,3,0,c[grid],I1b,I2b,I3b);
		ff[grid](I1g,I2g,I3g)=a0*f[grid](I1b,I2b,I3b)+a1*nsign*( uxxy+uyyy ) - nsign*uxxy;
	      }
	      else
	      {
		Overture::abort();
	      }
	    }
	    else
	    {
	      // Overture::abort();
              // we don't fill in this info in this case
	    }
	  } // end if orderOfAccuracy==4
	  
	}
	else if( c[grid].boundaryCondition(side,axis)>0 )
	{
	  printF("ERROR: unknown bc=%i for grid=%i\n",c[grid].boundaryCondition(side,axis),grid);
	  OV_ABORT("error");
	}
	
      }
    }
    if( false && problemIsSingular )
    {
      // ****** explicitly compute the left null vector

      Integrate integrate(c);
      RealCompositeGridFunction & leftNullVector = integrate.leftNullVector();
      real alpha=0., leftNorm=0.;
      for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);
        alpha+=sum(leftNullVector[grid](I1,I2,I3)*ff[grid](I1,I2,I3));
        leftNorm+=sum(leftNullVector[grid](I1,I2,I3)*leftNullVector[grid](I1,I2,I3));
      }
      alpha/=sqrt(leftNorm);
      printF("\n\n========================================================\n"
                 "*** Compatibility value: alpha = l.f/l.l = %8.2e ****\n"
                "========================================================= \n\n",alpha);

      // Oges::debug=1;

//       alpha/=sqrt(leftNorm);
//       for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
//       {
// 	getIndex(cg[grid].dimension(),I1,I2,I3);
//         ff[grid]-=alpha*leftNullVector[grid];
//       }
      
    }

    if( problem==neumann2 )
    {
      printF("problem==neumann2 : de-singularize at one point\n");
      int grid=0;
      int i1=1,i2=1,i3=0;
      ff[grid](i1,i2,i3)-=1000.*tz(c[grid],i1,i2,i3)(i1,i2,i3);
      // ff[grid](i1,i2,i3)=tz(c[grid],i1,i2,i3)(i1,i2,i3);
      
      printF(" before: coeff[grid](4,i1,i2,i3) = %e \n",coeff[0](4,i1,i2,i3));
      for( int level=0; level<cg.numberOfMultigridLevels(); level++ )
      {
	assert( cg[grid].mask()(i1,i2,i3)>0 );
	coeff[grid](4,i1,i2,i3)-=1000.;
        // coeff.multigridLevel[level][grid](all,i1,i2,i3)=0.;
	// coeff.multigridLevel[level][grid](4,i1,i2,i3)=1.;
      }
    }
  }  
  else // not TZ 
  {
    realCompositeGridFunction & ff = cg.numberOfMultigridLevels()>1 ? f.multigridLevel[0] : f;
    CompositeGrid & c = cg.numberOfMultigridLevels()>1 ? cg.multigridLevel[0] : cg;    
    bool ok;
    for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
    {
      #ifdef USE_PPP
	realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	realSerialArray ffLocal; getLocalArrayWithGhostBoundaries(ff[grid],ffLocal);
	ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
	if( !ok ) continue;
	  
      #else
	realSerialArray & uLocal = u[grid];
	realSerialArray & ffLocal = ff[grid];
      #endif

      const real lapValue=1.;
      ffLocal=lapValue;

      // Assign Boundary Conditions 
      ForBoundary(side,axis)
      {
	Index I1b,I2b,I3b,I1g,I2g,I3g;
	getBoundaryIndex(c[grid].gridIndexRange(),side,axis,I1b,I2b,I3b);
	getGhostIndex   (c[grid].gridIndexRange(),side,axis,I1g,I2g,I3g);

	ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1b,I2b,I3b,1);
	ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1g,I2g,I3g,1);
	if( !ok ) continue;

	if( c[grid].boundaryCondition(side,axis)==dirichlet )
	{
	  ffLocal(I1b,I2b,I3b)=0.;
	  uLocal(I1b,I2b,I3b)=ffLocal(I1b,I2b,I3b);    // added 030330

          if( orderOfAccuracy==2 )
	  {
  	    ffLocal(I1g,I2g,I3g)=0.; // extrap
	  }
          else
	  {
            // for 4th order we fill in the eqn at the bndry as the rhs at the ghost point *wdh* 100723
	    if( equationToSolve==OgesParameters::heatEquationOperator )
	      ffLocal(I1g,I2g,I3g)=(equationCoefficients(0,grid)*uLocal(I1b,I2b,I3b)+
				    equationCoefficients(1,grid)*lapValue);
            else
	      ffLocal(I1g,I2g,I3g)=lapValue;
	  }
	}
	else
	{
	  ffLocal(I1g,I2g,I3g)=0.; // extrap
	}
      }
    }
    
  }
  if( Ogmg::debug & 2 )
  {
    cpu0=getCPU()-cpu0;
    printF(" ogmgt: time to assign the RHS =%8.2e\n",cpu0);
  }
  
//    if( option=="cf" )
//    {
//      mgSolver.coarseToFineTest(u,f);  // test prologation
//    }
//    if( option=="fc" )
//    {
//      mgSolver.fineToCoarseTest(u,f);  // test restriction
//    }
//    else if( option=="smooth" )
//    {
//      mgSolver.smoothTest(u,f);       // test a smoother
//    }
//    else if( option=="bc" )
//    {
//      mgSolver.bcTest(u,f);  // test restriction
//    }


  if( FALSE )
  {
    // call twice in a row to make sure there nothing funny happens.
    par.setResidualTolerance(1.e-4);
    par.setErrorTolerance(1.e-3);
    mgSolver.solve(u,f);            // **** solve the problem with multigrid *****

    printF("Call solve again..\n");
    par.setResidualTolerance(1.e-12);
    par.setErrorTolerance(1.e-6);
    mgSolver.solve(u,f);            // **** solve the problem with multigrid *****
  }
  else // if( option=="solve" || option=="s" )
  {
    if( checkGeometryArrays )
    {
      printF("***GEOMETRY before solve\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	cg[grid].displayComputedGeometry();
    }
      
    // mgSolver.solve(u,f);  
    if( cg.numberOfMultigridLevels()==1 )
    {
      mgSolver.solve(u,f);  

      if( false )
      { // for testing solve again
	u=0.;
	mgSolver.solve(u,f);
      }
      

      if( false )
      {
	printF("\n **************Call mgSolver.updateToMatchGrid(cg) and solve again ...\n");
	
	mgSolver.updateToMatchGrid(cg);

	if( true )
	{
	  ::display(cg.numberOfInterpolationPoints(),"ogmgt: cg.numberOfInterpolationPoints()");
	}

        // u.updateToMatchGrid(cg);
	
	mgSolver.solve(u,f);  
      }
      
    }
    else 
    {
      mgSolver.solve(u.multigridLevel[0],f.multigridLevel[0]);  
    }
    
  }

  printF("\n\n ******************************************************************************\n");
  if( solvePredefined )
  {
    if( equationToSolve==OgesParameters::laplaceEquation )
    {
      printF(" *************** ogmgt: Solve predefined Laplace Equation *****************\n");
    }
    else if( equationToSolve==OgesParameters::heatEquationOperator )
    {
      printF(" ****** ogmgt: Solve predefined Heat Equation I - nuDt*Delta (nuDt=%9.2e)********\n",nuDt);
    }
    else if( equationToSolve==OgesParameters::divScalarGradOperator )
    {
      printF(" *************** ogmgt: Solve predefined div-scalar-grad Equation *****************\n");
    }
    else
    {
      printF(" *************** ogmgt: Solve predefined UNKNOWN Equation *****************\n");
    }
    
  }
  else
  {
    if( equationToSolve==OgesParameters::laplaceEquation )
    {
      printF(" *************** ogmgt: Solve Laplace Equation (NOT predefined) *****************\n");
    }
    else if( equationToSolve==OgesParameters::heatEquationOperator )
    {
      printF(" *************** ogmgt: Solve predefined Heat Equation  (NOT predefined) *****************\n");
    }
    else if( equationToSolve==OgesParameters::divScalarGradOperator )
    {
      printF(" *************** ogmgt: Solve predefined div-scalar-grad Equation  (NOT predefined) *****************\n");
    }
    else
    {
      printF(" *************** ogmgt: Solve predefined UNKNOWN Equation  (NOT predefined) *****************\n");
    }
  }
  printF(" ******************************************************************************\n");
  

  printF("\n *****After solve: max residual = %8.2e (%i cycles)****\n\n",mgSolver.getMaximumResidual(),
	 mgSolver.getNumberOfIterations());
  mgSolver.printStatistics();
    
  // printF(" +++++ time for setting up the coefficients = %e \n",timeForSettingUpCoefficients);
    
  if( Ogmg::debug & 32 ) 
    u.display("here is u on the fine grid after solve");


  total=coeff.sizeOf()+cgop.sizeOf()+cg.sizeOf()+mgSolver.sizeOf()+u.sizeOf()+f.sizeOf();
  pauseForInput("After solve",total,checkMemoryUsage);
  if( Ogmg::debug & 4 )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }
    

  if( false )
  {
    BoundaryConditionParameters extrapParams;
    extrapParams.orderOfExtrapolation=7;
    cgop.applyBoundaryCondition(u,0,BCTypes::extrapolate,BCTypes::allBoundaries,0,0.,extrapParams );
  }
    
  CompositeGrid & cg0 = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[0];
  RealCompositeGridFunction e(cg0);
  e=0.;
  RealArray error(cg0.numberOfComponentGrids());
  error=0.;
  real uMax=0.;
  real maximumError=0.;
  FILE *checkFile=mgSolver.getCheckFile();
  if( twilightZone )
  {
    const int numGhostToCheck=orderOfAccuracy/2;
    for( int numGhost=0; numGhost<=numGhostToCheck; numGhost++ )
    {
      for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )     
      {
	MappedGrid & mg = cg0[grid];
      
	getIndex(mg.gridIndexRange(),I1,I2,I3,numGhost);         // include ghost points 
        bool ok=true;
#ifdef USE_PPP
	realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	realSerialArray eLocal; getLocalArrayWithGhostBoundaries(e[grid],eLocal);
	realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
	intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);

	ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
#else
	realSerialArray & uLocal = u[grid];
	realSerialArray & eLocal = e[grid];
	const realSerialArray & xLocal = mg.center();
	const intSerialArray & maskLocal = mg.mask();
#endif

	uMax=REAL_MIN*100.;
	error(grid)=0.;
        if( ok )
	{
	  realSerialArray uExact(I1,I2,I3);
	  int ntd=0, nxd=0, nyd=0, nzd=0;
	  const bool isRectangular=false;  // do this for now
	  tz.gd( uExact,xLocal,cg0.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e

	  where( maskLocal(I1,I2,I3)!=0 )
	  {
	    uMax=max(fabs(uExact));
	
	    eLocal(I1,I2,I3)=fabs(uLocal(I1,I2,I3)-uExact);
	    error(grid)=max(fabs(eLocal(I1,I2,I3)));
	  }
	}
	uMax=ParallelUtility::getMaxValue(uMax);
	error(grid)=ParallelUtility::getMaxValue(error(grid));
	  
	maximumError=max(maximumError,error(grid));
	printF("Maximum error on grid: %15s = %8.2e, max relative error=%8.2e (includes %i ghost)\n",
	       (const char *)cg0[grid].mapping().getName(Mapping::mappingName),error(grid),error(grid)/uMax,
	       numGhost );
// 	if( numGhost==0 && checkFile!=NULL )
// 	{
// 	  fprintf(checkFile,"Maximum error on grid: %15s = %8.2e, max relative error=%8.2e (includes %i ghost)\n",
// 		  (const char *)cg0[grid].mapping().getName(Mapping::mappingName),
// 		  error(grid),error(grid)/uMax,numGhost);
// 	}
 
      }

      if( numGhost==0 && checkFile!=NULL )
      {
	// The last line in the check file is used by the convergence tests
	int numberOfComponents=1, component=0;
        real t=mgSolver.getNumberOfIterations();
	fprintf(checkFile,"%8.1e %i %i %8.2e %8.2e\n",t,numberOfComponents,component,maximumError,uMax);
      }
    }
      
    if( Ogmg::debug & 8 )
    {
      if( u.numberOfMultigridLevels()>1 )
      {
	realCompositeGridFunction & u0 = u.multigridLevel[0];
	u0.display("soln including ghost points","%8.2e ");
      }
      else
      {
	u.display("soln including ghost points","%8.2e ");
      }
	
      e.display("error including ghost points","%6.2e ");
    }
      
  }
    

  if( runTests || option=="coarseGrid" )
  {
    if( false )
    {
      // save the coarse grid for testing 
      CompositeGrid & mgcg = mgSolver.getCompositeGrid();
      int numLevels=mgcg.numberOfMultigridLevels();
      // CompositeGrid & cgCoarse = mgcg.multigridLevel[numLevels-1];
      CompositeGrid & cgCoarse = mgcg.multigridLevel[1];

//          for( int grid=0; grid<cgCoarse.numberOfComponentGrids(); grid++ )
//  	{
//  	  MappedGrid & mg = cgCoarse[grid];
//  	  displayMask(mg.mask(),sPrintF(buff,"Ogmg:: mask for grid %i, level=%i ",grid,level));
//  	}
	
      aString fileName="coarseLevel.hdf";
      aString gridName="coarseLevel";
      HDF_DataBase dataFile;
      dataFile.mount(fileName,"I");
      // int streamMode=1; // save in compressed form.
      // dataFile.put(streamMode,"streamMode");
      // dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
      printF("Saving the coarse level=%i as file=%s\n",numLevels,(const char*)fileName);
      // cgCoarse.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
      cgCoarse.put(dataFile,gridName);
      dataFile.unmount();
    }

    printF("\n\n**** test the accuracy of the coarse grid solver\n");
    mgSolver.coarseGridSolverTest(plotOption);
  }
  if( runTests || option=="cf" )
  {
    printF("\n\n***************** test coarse to fine operator ******************\n");
    mgSolver.coarseToFineTest();  // test prologation
  }
  if( runTests || option=="fc" )
  {
    printF("\n\n***************** test fine to coarse operator ******************\n");
    mgSolver.fineToCoarseTest(); 
  }
  if( runTests || option=="smooth" )
  {
    int plotOption=0;
    mgSolver.smoothTest(ps,plotOption);       // test a smoother
  }
  if( option=="bc" )
  {
    mgSolver.bcTest();
  }
    

  if( plotOption )
  {
      
    // -- combine the solution, error and defect into one grid function for easier plotting ---
    int numComponentsToPlot=3;
    realCompositeGridFunction v(cg,all,all,all,numComponentsToPlot);
    
    v.setName("solution",0);
    v.setName("error",1);
    v.setName("defect",2);
    
    realCompositeGridFunction & defect = mgSolver.getDefect().multigridLevel[0];
    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      OV_GET_SERIAL_ARRAY(real,v[grid],vLocal);
      OV_GET_SERIAL_ARRAY(real,u[grid],uLocal);
      OV_GET_SERIAL_ARRAY(real,e[grid],eLocal);
      OV_GET_SERIAL_ARRAY(real,defect[grid],defectLocal);
      
      getIndex(cg[grid].dimension(),I1,I2,I3);
      bool ok =ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
      if( ok )
      {
	vLocal(I1,I2,I3,0)=uLocal(I1,I2,I3);
	vLocal(I1,I2,I3,1)=eLocal(I1,I2,I3);
	vLocal(I1,I2,I3,2)=defectLocal(I1,I2,I3);
      }
    }
    
    aString answer;
    aString menu[]=
      {
        "contour",
	// "solution",
	// "error",
	"defect",
	"grid",
        "rhs (for Ogmg)",
        "plot parallel dist.",
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
      else if( answer=="contour" )
      {
	psp.set(GI_TOP_LABEL,"Ogmg:"); 
	PlotIt::contour(ps,v,psp);
      }
//       else if( answer=="solution" )
//       {
// 	psp.set(GI_TOP_LABEL,"Solution u"); 
// 	PlotIt::contour(ps,u,psp);
//       }
//       else if( answer=="error" )
//       {
// 	psp.set(GI_TOP_LABEL,"error"); 
// 	PlotIt::contour(ps,e,psp);
//       }
      else if( answer=="defect" )
      {
	psp.set(GI_TOP_LABEL,"defect"); 
	mgSolver.computeDefect(0);
	PlotIt::contour(ps,mgSolver.getDefect().multigridLevel[0],psp);
      }
      else if( answer=="rhs (for Ogmg)" )
      {
	psp.set(GI_TOP_LABEL,"rhs"); 
	PlotIt::contour(ps,mgSolver.getRhs(),psp);
      }
      else if( answer=="grid" )
      {
	psp.set(GI_TOP_LABEL,"grid"); 
	PlotIt::plot(ps,mgSolver.getCompositeGrid(),psp);
      }
      else if( answer=="plot parallel dist." )
      {
	ps.erase();
	psp.set(GI_TOP_LABEL,"Parallel distribution");
	PlotIt::plotParallelGridDistribution(mgSolver.getCompositeGrid(),ps,psp);
	ps.erase();
      }
      
    }
  }
  
  ps.unAppendTheDefaultPrompt();
  printF("\n *** Summary of results written to file ogmg.info **** \n");
  
  delete variableCoefficients;
  // Diagnostic_Manager::report();

  fflush(0);
  // delete solver here so that PETSc objects are cleaned up before MPI is shut down
  delete &mgSolver; 
  
  Overture::finish();          
  return(0);
}
