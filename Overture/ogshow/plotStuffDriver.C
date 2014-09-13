#include "GL_GraphicsInterface.h"
#include "HDF_DataBase.h"

//===============================================================================================
//         This is the plotStuff driver program 
//
//  You can add to the driver any nonstandard Mapping's that you want to use.
//  See the example below where (if the macro ADD_USERMAPPINGS is defined) an AirfoilMapping
//  is created and added to a list. This allows the mapping to be created when it is 
//  read in from a data-base. 
//
//  Thus, for example, your compile line should look something like:
//      CC -DADD_USERMAPPINGS .... plotStuffDriver.C 
//
//===============================================================================================

// Here are some user defined mappings
#ifdef ADD_USER_MAPPINGS
#include "AirfoilMapping.h"
int addToMappingList(Mapping & map);
#endif

int plotStuff(int argc, char *argv[]);


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);
  
#ifdef ADD_USER_MAPPINGS
  AirfoilMapping airfoil;
  // Do this so we can create the airfoil mapping when it is read from a data-base file
  addToMappingList(airfoil);
#endif

  IntegerArray ia(1,1); // put this here for now to fix load problem on Mac's *wdh* 2014/08/29

  plotStuff(argc,argv);

  Overture::finish();
}
