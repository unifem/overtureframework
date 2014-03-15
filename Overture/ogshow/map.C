#include "Mapping.h"
#include "MappingInformation.h"
#include "GL_GraphicsInterface.h"
#include "MappingRC.h"
#include "Annulus.h"
#include "Square.h"
#include "DataPointMapping.h"
#include "CylinderMapping.h"
#include "BoxMapping.h"
#include "LineMapping.h"

// MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end
// Here are some private mappings
#include "AirfoilMapping.h"

int createMappings( MappingInformation & mapInfo );

//---------------------------------------------------------
//   Test program for the createMappings routine
//---------------------------------------------------------
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  int plotOption=TRUE;
  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="noplot" )
        plotOption=FALSE;
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
    cout << "Usage: `pm [noplot][file.cmd]' \n"
            "          noplot:   run without graphics \n" 
            "          file.cmd: read this command file \n";

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    
  // This next call will allow the Mappings to be read in from the data-base file
  initializeMappingList();


  GL_GraphicsInterface gi(plotOption); // create a GL_GraphicsInterface object
  GraphicsParameters gp;               // create an object that is used to pass parameters
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  gi.appendToTheDefaultPrompt("pm>"); // set the default prompt
    
  // By default start saving the command file called "ogen.cmd"
  aString logFile="pm.cmd";
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

#define DIMENSION 3

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
#else

  domainDimension=2;
  rangeDimension=3;
  int n1=21, n2=11;
  int n3= domainDimension==3 ? 11 : 1;
  CylinderMapping a(0.,1.,-1.,1.,1.,1.5,0.,0.,0.,domainDimension );
//  BoxMapping a;
#endif    
//  int n1=5, n2=5;
//  SquareMapping a;

/* ---
  RealArray r(n1,n2,n3,domainDimension), x(n1,n2,n3,rangeDimension);
  for( int i3=0; i3<n3; i3++ )
  for( int i2=0; i2<n2; i2++ )
  for( int i1=0; i1<n1; i1++ )
  {
    r(i1,i2,i3,0)=i1/(n1-1.);
    if( domainDimension>1 )
      r(i1,i2,i3,1)=i2/(n2-1.);
    if( domainDimension>2 )
      r(i1,i2,i3,2)=i3/(n3-1.);
  }

  r.reshape(n1*n2*n3,domainDimension);
  x.reshape(n1*n2*n3,rangeDimension);
  a.map(r,x);
  r.reshape(n1,n2,n3,domainDimension);
  x.reshape(n1,n2,n3,rangeDimension);
---- */

/* ---
  // ** DataPointMapping dpm(x);  // can also do this
  DataPointMapping dpm;
    
// *****
//  dpm.setOrderOfInterpolation(4);
// *****

  for( int axis=axis1; axis<domainDimension; axis++ )
  {
    if( a.getIsPeriodic(axis) )
    {
      dpm.setIsPeriodic(axis,Mapping::functionPeriodic);
      dpm.setBoundaryCondition(Start,axis,-1);
      dpm.setBoundaryCondition(End  ,axis,-1);
    }
  }
  dpm.setDataPoints(x,3,domainDimension);

//  gi.plot(dpm);    
  mapInfo.mappingList.addElement(dpm);
---- */

  createMappings( mapInfo );
    
  // delete any mappings made here
  destructMappingList();

  if( Mapping::debug & 2 )
    ApproximateGlobalInverse::printStatistics();

  Overture::finish();          
  return 0;
}
