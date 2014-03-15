#include "Overture.h"  
#include "PlotStuff.h"
#include "ChannelMapping.h"
#include "MappingInformation.h"
#include "HDF_DataBase.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" --------------------------------------------------------------------------------- \n");
  printf(" Test a user-written Mapping, ChannelMapping.{h,C} \n");
  printf(" Use the checkMapping function to test derivatives, consistency of parameters etc. \n");
  printf(" Save the mapping to a data base and read back again   \n");
  printf(" --------------------------------------------------------------------------------- \n");

  ChannelMapping channel;

  realArray r(1,2),x(1,2),xr(1,2,2);
  r=.5;
  channel.map(r,x,xr);
  printf(" r=(%f,%f) x=(%f,%f) xr=(%f,%f,%f,%f)\n",r(0,0),r(0,1),x(0,0),x(0,1),
         xr(0,0,0),xr(0,1,0),xr(0,0,1),xr(0,1,1));

  // this function will check the mapping and it's derivatives etc.
  channel.checkMapping();


  // Make interactive changes to the mapping
  PlotStuff ps(TRUE,"mappedGridExample4");      // create a PlotStuff object
  MappingInformation mapInfo;              // parameters used by map.update
  mapInfo.graphXInterface=&ps;             // pass graphics interface
  channel.update(mapInfo);

  // Save the mapping in a data-base file
  HDF_DataBase dataBase;
  cout << "Mount a new database file...\n";
  dataBase.mount("map.dat","I");           // Initialize a database file

  channel.put(dataBase,"my-channel");       
  dataBase.unmount();
    
  // now mount the data-base and read in the mapping
  cout << "Mount an old data base file and read a mapping from it...\n";
  dataBase.mount("map.dat","R");  // mount a data base read-only
  ChannelMapping channel2;
  channel2.get(dataBase,"my-channel");   


  r=1.;
  channel2.map(r,x,xr);
  printf(" r=(%f,%f) x=(%f,%f) xr=(%f,%f,%f,%f)\n",r(0,0),r(0,1),x(0,0),x(0,1),
         xr(0,0,0),xr(0,1,0),xr(0,0,1),xr(0,1,1));

  Overture::finish();          
  return 0;
}

