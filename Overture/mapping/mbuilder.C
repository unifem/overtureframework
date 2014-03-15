#include "Mapping.h"
#include "MappingInformation.h"
#include "PlotStuff.h"
#include "MappingRC.h"
#include "AnnulusMapping.h"
#include "SquareMapping.h"
#include "DataPointMapping.h"
#include "CylinderMapping.h"
#include "BoxMapping.h"
#include "LineMapping.h"
#include "Inverse.h"

int display( realArray & x )
{
  ::display(x);
  return 0;
}

int display( intArray & x )
{
  ::display(x);
  return 0;
}

// MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end
// Here are some private mappings
#include "AirfoilMapping.h"

int createMappings( MappingInformation & mapInfo );
int initializeMappingList();
int destructMappingList();

//---------------------------------------------------------
//   mbuild : mapping builder program
//---------------------------------------------------------
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);

 int numberOfParallelGhost=2;

//    std::string aa="hi";
//    printf("aa=[%s]\n",aa.c_str());
  
  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="-nopause" || line=="-abortOnEnd" ||
          line=="noplot" || line=="nopause" || line=="abortOnEnd" )
        continue;
      else if( len=line.matches("-numParallelGhost=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfParallelGhost);
	printF("ogenDriver: will use %i parallel ghost points.\n",numberOfParallelGhost);
      }
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
    cout << "Usage: `mbuilder [noplot][nopause][abortOnEnd][file.cmd]' \n"
            "          noplot:   run without graphics \n" 
            "          nopause: do not pause \n" 
            "          abortOnEnd: abort if command file ends \n" 
            "          file.cmd: read this command file \n";

  #ifdef USE_PPP
    // Set the default number of parallel ghost lines. This can be changed in the ogen menu.
    // On Parallel machines always add at least this many ghost lines on local arrays
    Mapping::setMinimumNumberOfDistributedGhostLines(numberOfParallelGhost);
  #endif

  // This next call will allow the Mappings to be read in from the data-base file
  initializeMappingList();

  // Note: options "noplot", "nopause" and "abortOnEnd" are handled in the next call:
  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface("mbuilder",false,argc,argv);

  GraphicsParameters gp;             // create an object that is used to pass parameters
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  gi.appendToTheDefaultPrompt("mbuild>"); // set the default prompt
    
  // By default start saving the command file called "mbuilder.cmd"
  aString logFile="mbuilder.cmd";
  gi.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  if( commandFileName!="" )
    gi.readCommandFile(commandFileName);

  if( FALSE )
  {
    // create some private mappings and add to the generic list of mappings
    Mapping *mapPointer;
    mapPointer=new AirfoilMapping(); 
    mapInfo.mappingList.addElement(*mapPointer);
  }

  int domainDimension=2;
  int rangeDimension=2;

#define DIMENSION 4

#if ( DIMENSION == 1 )
  domainDimension=1;
  rangeDimension=1;
  int n1=11, n2=1, n3=1;
  LineMapping a;
#elif ( DIMENSION == 2 )

  domainDimension=2;
  rangeDimension=2;
  int n1=31, n2=11, n3=1;
//  AnnulusMapping a(.5,1.,0.,0.,.1,.9);
  AnnulusMapping a;
#elif ( DIMENSION == 3 )

  domainDimension=2;
  rangeDimension=3;
  int n1=21, n2=11;
  int n3= domainDimension==3 ? 11 : 1;
  CylinderMapping a(0.,1.,-1.,1.,1.,1.5,0.,0.,0.,domainDimension );
//  BoxMapping a;
#endif    
//  int n1=5, n2=5;
//  SquareMapping a;


  createMappings( mapInfo );
    
  // delete any mappings made here
  destructMappingList();

  if( Mapping::debug & 2 )
    ApproximateGlobalInverse::printStatistics();

  Overture::finish();

  return 0;
}
