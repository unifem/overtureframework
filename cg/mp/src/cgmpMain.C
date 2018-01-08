// ==================================================================================
//   cgmp : main program for the multi-phsyics solver
// ==================================================================================

#include "Cgmp.h"
#include "PlotStuff.h"
#include "ParallelUtility.h"

#include "CgSolverUtil.h"
#include "Oges.h"

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture and A++/P++
  // Optimization_Manager::setForceVSG_Update(Off);
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
  { // look at arguments for "noplot" or some other name
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
      else if( line=="memory" )
      {
	reportMemory=true;
        Diagnostic_Manager::setTrackArrayData(TRUE);
      }
      else if( line=="loadBalance" || line=="-loadBalance" )
      {
	loadBalance=true;
      }
      else if( len=line.matches("-numberOfParallelGhost=") )
      {
	sScanF(line(len,line.length()-1),"%i",&numberOfParallelGhost);
        if( numberOfParallelGhost<0 || numberOfParallelGhost>10 )
	{
	  printF("ERROR: numberOfParallelGhost=%i is no valid!\n",numberOfParallelGhost);
	  OV_ABORT("error");
	}
	printF("Setting numberOfParallelGhost=%i\n",numberOfParallelGhost);
      }
      else if( line=="release" )
      {
        smartRelease=true;
        printF("*** turn on smart release of memory ***\n");
        Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );
      }
      else if( commandFileName=="" )
      {
        commandFileName=line;
        printF("cgmp: reading commands from file [%s]\n",(const char*)commandFileName);
      }

    }
  }
  else
  {
    printF("Usage: `cgmp [options][file.cmd]' \n"
	   "     options:                            \n"
	   "          noplot:   run without graphics \n"
	   "          nopause: do not pause \n"
	   "          abortOnEnd: abort if command file ends \n"
	   "          memory:   run with A++ memory tracking\n"
	   "          release:  run with A++ smart release of memeory\n"
	   "     file.cmd: read this command file \n");

  }


  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("cgmp",false,argc,argv);

  // By default start saving the command file called "cgmp.cmd"
  aString logFile="cgmp.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  ps.appendToTheDefaultPrompt("cgmp>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    ps.readCommandFile(commandFileName);
  }

  aString nameOfShowFile;

  CompositeGrid cg;
  aString nameOfGridFile="";

  // *wdh* 2014/01/12 nameOfGridFile =readOrBuildTheGrid(ps, cg, loadBalance);

  const int maxWidthExtrapInterpNeighbours=4;  // This means we support 3rd-order extrap, (1,-3,3,-1)
  nameOfGridFile = readOrBuildTheGrid(ps, cg, loadBalance, numberOfParallelGhost,maxWidthExtrapInterpNeighbours );

  cg.update(GridCollection::THEdomain);

  const int numberOfDomains=cg.numberOfDomains();
  printF(" >>> numberOfDomains=%i\n",numberOfDomains);

  Ogshow *show=NULL;

  Cgmp & mpSolver = *new Cgmp(cg,&ps,show,plotOption);

  mpSolver.setNameOfGridFile(nameOfGridFile);
  mpSolver.setParametersInteractively();

  mpSolver.solve();

  mpSolver.printStatistics();

  ps.unAppendTheDefaultPrompt();
//  delete show;

  if( reportMemory )
    Diagnostic_Manager::report();

  if(  myid==0 && false )
  {
    printf("\n +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
    printf(" +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  }


//  delete pDomainSolver;
//   for( int d=0; d<numberOfDomains; d++ )
//   {
//     delete solver[d];  // do this so we shutdown PETSc (if used).
//   }
//   delete [] solver;
//   delete [] interpolant;


  delete &mpSolver;  // do this so we shutdown PETSc before P++  (if PETSc is being used).

  Overture::finish();
  if( smartRelease )
  {
    int totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    if( totalNumberOfArrays>0 )
    {
      printf("\n**** WARNING: Number of A++ is %i >0 \n",totalNumberOfArrays);
    }
  }

  return 0;
}
