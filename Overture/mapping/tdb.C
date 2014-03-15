//
// test the new streaming data base
//

#include "SmoothedPolygon.h"
#include "Square.h"
#include "HDF_DataBase.h"
#include "PlotStuff.h"

void initializeMappingList();

int
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

  int option=0;
  cout << "Enter option (0=normal, 1=stream) \n";
  cin >> option;

  initializeMappingList();

  PlotStuff ps;
  SquareMapping square(2.,4.,3.,6.);
  SmoothedPolygon poly;

  Mapping & map = poly;

  PlotIt::plot(ps,map);

  HDF_DataBase db;
  db.mount("stream.hdf","I");
  if( option==1 )
    db.setMode(GenericDataBase::streamOutputMode);
  
  real time0=getCPU();

  db.put(map.getClassName(),"className");
  map.put(db,"my_map");
  char buff[40];
  for( int i=0; i<100; i++ )
  {
    sprintf(buff,"map%i",i);
    map.put(db,buff);
  }
  
  if( option==1 )
    db.setMode(GenericDataBase::normalMode);
  db.printStatistics();
  db.unmount();
  cout << "time to save and unmount file = " << getCPU()-time0 << endl;
  
  
  HDF_DataBase db2;
  time0=getCPU();
  db2.mount("stream.hdf","R");
  if( option==1 )
    db2.setMode(GenericDataBase::streamInputMode);


  aString mappingClassName;
  db2.get(mappingClassName,"className");
  cout << "mappingClassName=[" << mappingClassName << "]\n";
  
  Mapping *mapPointer = Mapping::makeMapping( mappingClassName );

  mapPointer->get(db2,"my_map");
  cout << "time to mount file and get = " << getCPU()-time0 << endl;
  // square2.display("");
  if( option==1 )
    db2.setMode(GenericDataBase::normalMode);
  db2.unmount();
  
  PlotIt::plot(ps,*mapPointer);

  return 0;
}
