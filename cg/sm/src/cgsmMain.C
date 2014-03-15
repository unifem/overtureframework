// ---------------------------------------------------------------------------
// Solve the equations of Solid Mechanics
//
//
// ---------------------------------------------------------------------------

#include "MappedGridOperators.h"
#include "PlotStuff.h"
// #include "SquareMapping.h"
// #include "AnnulusMapping.h"
// #include "MatrixTransform.h"
// #include "DataPointMapping.h"

// #include "OGTrigFunction.h"
// #include "OGPolyFunction.h"
#include "display.h"

#include "Cgsm.h"
#include "ParallelUtility.h"

#include "CgSolverUtil.h"

int 
getLineFromFile( FILE *file, char s[], int lim);

void display(realArray & u )
{
  printF("u.getlength(0)=%i\n",u.getLength(0));
  
  ::display(u,"u");
}



int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);
  // Use this to avoid un-necessary communication: 
  Optimization_Manager::setForceVSG_Update(Off);
  const int myid=Communication_Manager::My_Process_Number;

  int plotOption=true;
  bool smartRelease=false;
  bool reportMemory=false;
  bool loadBalance=false;
  int numberOfParallelGhost=2;
  
  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    int len=0;
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
        printF("cgsmMain: reading commands from file [%s]\n",(const char*)commandFileName);
      }
      
    }
  }
  else
    printF("Usage: `cgsm [options][file.cmd]' \n"
            "     options:                            \n" 
            "          -noplot:   run without graphics \n" 
            "          -nopause: do not pause \n" 
            "          -abortOnEnd: abort if command file ends \n" 
            "          -numberOfParallelGhost=<num> : number of parallel ghost lines \n" 
            "          memory:   run with A++ memory tracking\n" 
            "          release:  run with A++ smart release of memory\n"
            "     file.cmd: read this command file \n");

  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("cgsm",false,argc,argv);

  // By default start saving the command file called "cgsm.cmd"
  aString logFile="cgsm.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  ps.appendToTheDefaultPrompt("cgsm>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    ps.readCommandFile(commandFileName);
  }


  CompositeGrid cg;
  const int maxWidthExtrapInterpNeighbours=4;  // This means we support 3rd-order extrap, (1,-3,3,-1)
  aString nameOfGridFile="";
  nameOfGridFile = readOrBuildTheGrid(ps, cg, loadBalance, numberOfParallelGhost,maxWidthExtrapInterpNeighbours );


  Interpolant & interpolant = *new Interpolant(cg); interpolant.incrementReferenceCount();
  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);

//   #ifdef USE_PPP
//   if( !interpolant.interpolationIsExplicit() )
//   {
//     printf("cgsm:ERROR: The parallel composite grid interpolator needs explicit interpolation ****\n");
//     Overture::abort();
//   }
//   #endif

  Ogshow *show=NULL;
  Cgsm & solver = *new Cgsm(cg,&ps,show,plotOption); 

  solver.setNameOfGridFile(nameOfGridFile);
  solver.setParametersInteractively();

  solver.solve();

  delete &solver;  // do this so we close the show file and shutdown PETSc (if used).

  printF("cgsmMain: interpolant.getReferenceCount=%i\n",interpolant.getReferenceCount());
  if( interpolant.decrementReferenceCount()==0 )
  {
    printF("cgsmMain: delete Interpolant\n");
    delete &interpolant;
  }

  Overture::finish();          
  return 0;
}
