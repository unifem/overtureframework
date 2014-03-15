#include "Cgcns.h"
#include "PlotStuff.h"
#include "Ogshow.h"
#include "ParallelUtility.h"
#include "display.h"

#include "Cgcns.h"
#include "CgSolverUtil.h"

int 
getLineFromFile( FILE *file, char s[], int lim);


int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture and A++/P++
  // Optimization_Manager::setForceVSG_Update(Off);
  const int myid=Communication_Manager::My_Process_Number;

  Overture::turnOnMemoryChecking(true);

  int plotOption=true;
  bool smartRelease=false;
  bool reportMemory=false;
  bool loadBalance=false;

  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
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
      else if( line=="release" )
      {
        smartRelease=true;
        printF("*** turn on smart release of memory ***\n");
        Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );
      }
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printF("cgcns: reading commands from file [%s]\n",(const char*)commandFileName);
      }
      
    }
  }
  else
    printF("Usage: `cgcns [options][file.cmd]' \n"
            "     options:                            \n" 
            "          noplot:   run without graphics \n" 
            "          nopause: do not pause \n" 
            "          abortOnEnd: abort if command file ends \n" 
            "          memory:   run with A++ memory tracking\n" 
            "          release:  run with A++ smart release of memeory\n"
            "     file.cmd: read this command file \n");


  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("cgcns",false,argc,argv);

  // By default start saving the command file called "cgcns.cmd"
  aString logFile="cgcns.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  ps.appendToTheDefaultPrompt("cgcns>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    ps.readCommandFile(commandFileName);
  }

  aString nameOfShowFile;

  CompositeGrid cg;
  int numParallelGhost=2;
  int maxWidthForExtrapInterpNeighbours=3; // 
  aString nameOfGridFile="";
  nameOfGridFile = readOrBuildTheGrid(ps, cg, loadBalance, numParallelGhost, maxWidthForExtrapInterpNeighbours );

  Interpolant & interpolant = *new Interpolant(cg); interpolant.incrementReferenceCount();
  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);


  // cout << "cgcns: numberOfGrids = " << cg.numberOfGrids() << endl;
  // cout << "cgcns: numberOfComponentGrids = " << cg.numberOfComponentGrids() << endl;

  // cout << "cgcns: nameOfShowFile = " << nameOfShowFile << endl;
  
   Ogshow *show=NULL;
//  ** old way:
//   ps.inputString(nameOfShowFile,"Enter the name of the (new) show file (enter `none' for none):");
//   if( nameOfShowFile=="none" )
//     nameOfShowFile="";
//   if( nameOfShowFile!="" )
//     show = new Ogshow( nameOfShowFile,".",false );
  

  // CgSolver solver(cg,&ps,show,plotOption);
  // CgSolver & solver = *new CgSolver(cg,&ps,show,plotOption);

  Cgcns & solver = *new Cgcns(cg,&ps,show,plotOption);

//   pDomainSolver = new Cgcns(solver.parameters,solver.u,solver.parameters.dbase.get<real >("tInitial"));
//   pDomainSolver->updateToMatchGrid(cg);

  solver.setNameOfGridFile(nameOfGridFile);

  solver.setParametersInteractively();

//   assert( solver.compositeGridSolver!=NULL && solver.compositeGridSolver[0]!=NULL );
//   pDomainSolver = solver.compositeGridSolver[0]; // do this for now

  solver.solve();
  solver.printStatistics();

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

  delete &solver;  // do this so we shutdown PETSc (if used).

  printF("cgcnsMain: interpolant.getReferenceCount=%i\n",interpolant.getReferenceCount());
  if( interpolant.decrementReferenceCount()==0 )
  {
    printF("cgcnsMain: delete Interpolant\n");
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
  
  return 0;
}
