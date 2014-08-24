#ifdef OV_HAVE_GOOGLE_PROFILE
#include "google/profiler.h"
#endif

#include "Cgins.h"
#include "PlotStuff.h"
#include "Ogshow.h"
#include "ParallelUtility.h"
#include "display.h"
#include "CgSolverUtil.h"
#include "Oges.h"


int 
getLineFromFile( FILE *file, char s[], int lim);

int
main(int argc, char *argv[])
{
#ifdef OV_HAVE_GOOGLE_PROFILE
  ProfilerStart("ins.gprofile");
#endif

  Overture::start(argc,argv);  // initialize Overture and A++/P++
  const int myid=Communication_Manager::My_Process_Number;

  // This macro will initialize the PETSc solver if OVERTURE_USE_PETSC is defined.
  INIT_PETSC_SOLVER();

  int plotOption=true;
  bool smartRelease=false;
  bool reportMemory=false;
  bool loadBalance=false;
  int numberOfParallelGhost=2;
  
  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "-noplot" or some other name
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="noplot" )
        plotOption=false;
      else if( line=="-nopause" || line=="-abortOnEnd" || line=="-nodirect" ||
               line=="-readCollective" || line=="-writeCollective" ||
               line=="nopause" || line=="abortOnEnd" || line=="nodirect" )
        continue; // these commands are processed by getGraphicsInterface below 
      else if( line=="-memory" || line=="memory" )
      {
	reportMemory=true;
        Diagnostic_Manager::setTrackArrayData(TRUE);
      }
      else if( line=="loadBalance" || line=="-loadBalance" ) // *old way*
      {
	loadBalance=true;
      }
      else if( len=line.matches("-loadBalance=") ) // *new* way
      {
	int boolValue=0;
	
       sScanF(line(len,line.length()-1),"%i",&boolValue);
       loadBalance = boolValue!=0;
       printF("cginsMain: setting: loadBalance=%i.\n",(int)loadBalance);
      }
      
      else if( line=="-release" || line=="release" )
      {
        smartRelease=true;
        printF("*** turn on smart release of memory ***\n");
        Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );
      }
      else if( len=line.matches("-numberOfParallelGhost=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfParallelGhost);
	printF("cginsMain: will use %i parallel ghost points.\n",numberOfParallelGhost);
      }
      else if( len=line.matches("-numParallelGhost=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfParallelGhost);
	printF("cginsMain: will use %i parallel ghost points.\n",numberOfParallelGhost);
      }
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printF("cginsMain: reading commands from file [%s].\n",(const char*)commandFileName);
      }
      
    }
  }
  else
    printF("Usage: `cgins [options][file.cmd]' \n"
            "     options:                            \n" 
            "          -noplot:   run without graphics \n" 
            "          -nopause: do not pause \n" 
            "          -abortOnEnd: abort if command file ends \n" 
            "          -memory:   run with A++ memory tracking\n" 
            "          -numberOfParallelGhost=<val>:  set the number of parallel ghost lines.\n" 
            "          -loadBalance=[0|1]: 1=load balance grids.\n" 
            "          -release:  run with A++ smart release of memeory\n"
            "     file.cmd: read this command file \n");


  
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("cgins",false,argc,argv);


  char *cgenv = getenv("CG");
  if ( cgenv )
  {
    aString cmd = aString("use lib \""+aString(cgenv)+"/common/src\"; use CgUtilities;");
    ps.parseAnswer(cmd);
    cmd = aString("use lib \""+aString(cgenv)+"/ins/src\"; use CgINS;");
    ps.parseAnswer(cmd);
  }

  // By default start saving the command file called "cgins.cmd"
  aString logFile="cgins.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  ps.appendToTheDefaultPrompt("cgins>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    ps.readCommandFile(commandFileName);
  }

  aString nameOfShowFile;
  CompositeGrid cg;
  aString nameOfGridFile="";
  nameOfGridFile = readOrBuildTheGrid(ps, cg, loadBalance, numberOfParallelGhost);

  // Interpolant interpolant(cg); 
  Interpolant & interpolant = *new Interpolant(cg); interpolant.incrementReferenceCount();
  // Interpolant & interpolant = *new Interpolant; interpolant.incrementReferenceCount();
  // interpolant.updateToMatchGrid(cg);

  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);

//   #ifdef USE_PPP
//   if( !interpolant.interpolationIsExplicit() )
//   {
//     printF("cgins:INFO: This grid has implicit interpolation. Explicit interpolation is faster.\n");
//     //     printF("cgins:ERROR: The parallel composite grid interpolator needs explicit interpolation ****\n");
//     //     Overture::abort();
//   }
//   #endif

  Ogshow *show=NULL;

  Cgins & solver = *new Cgins(cg,&ps,show,plotOption);

  solver.setNameOfGridFile(nameOfGridFile);
  solver.setParametersInteractively();

//   assert( solver.compositeGridSolver!=NULL && solver.compositeGridSolver[0]!=NULL );
//   pDomainSolver = solver.compositeGridSolver[0]; // do this for now

  solver.solve();
  solver.printStatistics();

  ps.unAppendTheDefaultPrompt();

  if( reportMemory )
    Diagnostic_Manager::report();

  if(  myid==0 && false ) 
  {
    printf("\n +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
    printf(" +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  }

  delete &solver;  // do this so we shutdown PETSc (if used).

  printF("cginsMain: interpolant.getReferenceCount=%i\n",interpolant.getReferenceCount());
  if( interpolant.decrementReferenceCount()==0 )
  {
    printF("cginsMain: delete Interpolant\n");
    delete &interpolant;
  }
  
  Overture::finish();          
  if( smartRelease )
  {
    int totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    if( totalNumberOfArrays>0 )
    {
      printf("\n**** WARNING: Number of A++ is %i >0 \n",totalNumberOfArrays);
    }
  }

#ifdef OV_HAVE_GOOGLE_PROFILE
  ProfilerStop();
#endif  

  return 0;

}


