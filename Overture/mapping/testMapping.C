#include "Overture.h"  
#include "PlotStuff.h"
// include "RocketMapping.h"
#include "FilamentMapping.h"
#include "MappingInformation.h"
#include "HDF_DataBase.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" --------------------------------------------------------------------------------- \n");
  printf(" Test a new  Mapping \n");
  printf(" Use the checkMapping function to test derivatives, consistency of parameters etc. \n");
  printf(" Save the mapping to a data base and read back again   \n");
  printf(" --------------------------------------------------------------------------------- \n");

//  RocketMapping map;
  FilamentMapping map;

  // Make interactive changes to the mapping
  PlotStuff ps(TRUE,"testMapping");      // create a PlotStuff object
  MappingInformation mapInfo;              // parameters used by map.update
  mapInfo.graphXInterface=&ps;             // pass graphics interface
  map.update(mapInfo);

  printf("*********** Now test the operator = **************\n");
  FilamentMapping map3;
  map3=map;
  printf("*********** call interactiveUpdate **************\n");

  map3.debug=3;
  // map3.interactiveUpdate(ps);

  int i;
  for( i=0; i<100; i++ )
  {
    FilamentMapping map4;
    map4=map;
    if( i % 5 == 0 )
      printf("**** map4=map: number of A++ arrays = %i \n",Array_Domain_Type::getNumberOfArraysInUse());
  }
  


  RealArray r(1,2),x(1,2),xr(1,2,2);
  r=.5;
  map.map(r,x,xr);
  printf(" r=(%f,%f) x=(%f,%f) xr=(%f,%f,%f,%f)\n",r(0,0),r(0,1),x(0,0),x(0,1),
         xr(0,0,0),xr(0,1,0),xr(0,0,1),xr(0,1,1));

  // this function will check the mapping and it's derivatives etc.
// **  map.checkMapping();



  // Save the mapping in a data-base file
  HDF_DataBase dataBase;
  cout << "Mount a new database file...\n";
  dataBase.mount("map.dat","I");           // Initialize a database file

  map.put(dataBase,"my-map");       
  dataBase.unmount();
    
  // now mount the data-base and read in the mapping
  cout << "Mount an old data base file and read a mapping from it...\n";
  dataBase.mount("map.dat","R");  // mount a data base read-only
  for( i=0; i<100; i++ )
  {
    FilamentMapping map4;
    // map4.debug=3;
    map4.get(dataBase,"my-map");   
    if( i % 5 == 0 )
      printf("**** get map4: number of A++ arrays = %i \n",Array_Domain_Type::getNumberOfArraysInUse());
  }

  FilamentMapping map2;
  map2.get(dataBase,"my-map");   
  r=1.;
  map2.map(r,x,xr);
  printf(" r=(%f,%f) x=(%f,%f) xr=(%f,%f,%f,%f)\n",r(0,0),r(0,1),x(0,0),x(0,1),
         xr(0,0,0),xr(0,1,0),xr(0,0,1),xr(0,1,1));

  map2.update(mapInfo);


  Overture::finish();          
  return 0;
}
