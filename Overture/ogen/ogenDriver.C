//=========================================================================================
// Here is the driver program for `ogen' - the overlapping grid generator
//
//   Usage: type 
//       ogen
//  to run with graphics, or type
//       ogen noplot
//   to run without graphics, or
//       ogen file.cmd
//   to run ogen with graphics and read in a command file, or
//       ogen noplot file.cmd
//   to run ogen without graphics and read in a command file.
//
//  By default user commands will be saved in the file "ogen.cmd"
//
//  You can add to the driver any nonstandard Mapping's that you want to use.
//  See the example below where (if the macro ADD_USERMAPPINGS is defined) an AirfoilMapping
//  is created and added to a list. The list is then passed to ogen. The Mapping
//  can be subsequently changed within ogen, if required.
//
//  Thus, for example, your compile line should look something like:
//      CC -DADD_USERMAPPINGS .... ogenDriver.C 
//
//===========================================================================================

#include "Overture.h"
#include "MappingInformation.h"
#include "PlotStuff.h"

// Here are some user defined mappings
#ifdef ADD_USER_MAPPINGS
#include "AirfoilMapping.h"
int addToMappingList(Mapping & map);
#endif

int ogen(MappingInformation & mappingInfo, GenericGraphicsInterface & ps, const aString & commandFileName, CompositeGrid *cgp=0 );

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);
  // Index::setBoundsCheck(off); 

  Overture::printMemoryUsage("ogenDriver: after Overture::start");

  int numberOfParallelGhost=2;

  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="-nopause" || line=="-abortOnEnd" || line=="-nodirect" ||
          line=="-readCollective" || line=="-writeCollective" || line=="-multipleFileIO" ||
          line=="noplot" || line=="nopause" || line=="abortOnEnd" || line=="nodirect" ) // old way
      {
        continue; // these commands are processed by getGraphicsInterface below
      }
      else if( (len=line.matches("-numberOfParallelGhost=")) )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfParallelGhost);
	printF("ogenDriver: will use %i parallel ghost points.\n",numberOfParallelGhost);
      }
      else if( (len=line.matches("-numParallelGhost=")) )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfParallelGhost);
	printF("ogenDriver: will use %i parallel ghost points.\n",numberOfParallelGhost);
      }
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
  {
    printF("Usage: `ogen [-noplot][-nopause][-abortOnEnd][-numParallelGhost=<val>][file.cmd]' \n"
	   "          -noplot:   run without graphics \n" 
	   "          -nopause: do not pause \n" 
	   "          -abortOnEnd: abort if command file ends \n" 
	   "          file.cmd: read this command file \n");
    
  }
  
    // --- create user defined mappings ----
  MappingInformation mappingInfo;
#ifdef ADD_USER_MAPPINGS
  AirfoilMapping airfoil;
  mappingInfo.mappingList.addElement(airfoil);
  // Do this so we can read the airfoil mapping from a data-base file
  addToMappingList(airfoil);
#endif
  

  // Graphics interface:
  // Note: options "noplot", "nopause" and "abortOnEnd" are handled in the next call:
  // NOTE: this next call is important to get the command line arguments passed to the cmd file:
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("ogen: Overlapping Grid Generator",false,argc,argv);

  // By default start saving the command file called "ogen.cmd"
  aString logFile="ogen.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char*)logFile);
  fflush(0);

  #ifdef USE_PPP
    // Set the default number of parallel ghost lines. This can be changed in the ogen menu.
    // On Parallel machines always add at least this many ghost lines on local arrays
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numberOfParallelGhost);
  #endif

  Overture::printMemoryUsage("ogenDriver (2)");

  Overture::turnOnMemoryChecking(true);

  Overture::printMemoryUsage("ogenDriver (3)");

  // create more mappings and/or make an overlapping grid
  ogen( mappingInfo,ps,commandFileName);

  Overture::finish();
  return 0;
}




