#include "Overture.h"  
#include "HDF_DataBase.h"
#include "SquareMapping.h"
#include "display.h"
#include "ParallelUtility.h"

int 
main(int argc, char *argv[])
{
  int debug=0;
  
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);

  printF(" ---------------------------------------------------- \n");
  printF(" Test saving a grid function an HDf file              \n");
  printF(" ---------------------------------------------------- \n");

  int readCollective=true;
  int writeCollective=true;
  if( readCollective )
  {
    if( myid==0 ) printf("***Set DataBase READ mode to collective ***\n");
    GenericDataBase::setParallelReadMode(GenericDataBase::collectiveIO);
  }
  if( writeCollective )
  {
    if( myid==0 ) printf("***Set DataBase WRITE mode to collective ***\n");
    GenericDataBase::setParallelWriteMode(GenericDataBase::collectiveIO);
  }


  const int nx=256, ny=256;
//  const int nx=64, ny=64;
//  const int nx=7, ny=7;

  char buff[80];
  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,nx);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,ny);                  // axis2==1, set no. of grid points
  MappedGrid mg(square);                               // MappedGrid for a square
  mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );       
  
  Range all;                                   // a null Range is used as a place-holder below for the coordinates
  realMappedGridFunction u(mg,all,all,all,2);  // create a grid function with 2 components: u(0:10,0:10,0:0,0:1)
  u.setName("Velocity Stuff");                 // give names to grid function ...
  u.setName("u Stuff",0);                      // ...and components
  u.setName("v Stuff",1);
  Index I1,I2,I3;                                            

  // mg.dimension()(2,3) : start/end index values for all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);                        // assign I1,I2,I3 from dimension
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray x; getLocalArrayWithGhostBoundaries(mg.vertex(),x);
  #else
    realSerialArray & uLocal = u;
    realSerialArray & xLocal = mg.vertex();
  #endif

  const int includeGhost=1;
  bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
  if( ok )
  {
    uLocal(I1,I2,I3,0)=sin(Pi*x(I1,I2,I3,axis1))*cos(Pi*x(I1,I2,I3,axis2));        
    uLocal(I1,I2,I3,1)=cos(Pi*x(I1,I2,I3,axis1))*sin(Pi*x(I1,I2,I3,axis2));
  }
  u.updateGhostBoundaries();
  
  if( debug & 1 )
  {
    display(u,"u before writing","%5.2f ");
  }

  // Save the grid-function in a data-base file
  HDF_DataBase dataBase;

  dataBase.mount("gf.hdf","I");           // Initialize a database file

  bool stream=false;
  if ( stream )
    dataBase.setMode(GenericDataBase::streamOutputMode);
  else
    dataBase.setMode(HDF_DataBase::noStreamMode);


  aString name="u";
  u.put(dataBase,name);


  if ( stream )
    dataBase.setMode(GenericDataBase::normalMode);  
  dataBase.unmount();
    
  // now mount the data-base and read in the mapping
  printF("Mount an old data base file and read a mapping from it...\n");
  
  dataBase.mount("gf.hdf","R");  // mount a data base read-only
  if ( stream ) 
    dataBase.setMode(GenericDataBase::streamInputMode);

  realMappedGridFunction v(mg,all,all,all,2);
  real time0=getCPU();
  v.get(dataBase,name);
  real time=ParallelUtility::getMaxValue(getCPU()-time0);
  printF("nx=%i, ny=%i, v.elementCount()=%i, time to read = %8.2e(s)\n",nx,ny,v.elementCount(),time);

  if( debug & 1 )
    display(v,"v after reading","%5.2f ");
  
  #ifdef USE_PPP
    realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
  #else
    realSerialArray & vLocal = v;
  #endif

  vLocal-=uLocal;
  if( debug & 1 )
  {
    display(v,"error","%5.2f ");
    // display(vLocal,"vLocal : error","%5.2f ");
  }
  
  real err = 0.;
  if( ok )
    err=max(fabs(vLocal(I1,I2,I3,nullRange)));
  err=ParallelUtility::getMaxValue(err);
  printF("Maximum difference between u and v is %8.2e\n",err);

  if ( stream )
    dataBase.setMode(GenericDataBase::normalMode);  
  dataBase.unmount();

  Overture::finish();          
  return 0;
}
