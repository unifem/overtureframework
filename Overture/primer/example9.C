#include "Overture.h"  
#include "HDF_DataBase.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ----------------------------------------------------------------------- \n");
  printf("This example shows how to save information in a data base file.          \n");
  printf("This could be a restart file for a PDE solver                            \n");
  printf("See also the documentation on ShowFileReader in the Ogshow documentation \n");
  printf("for how to read grids and grid functions from a show file. This would    \n"); 
  printf("be another way to get initial conditions for a solver.                   \n");
  printf(" ----------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "example>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  Range all;
  realCompositeGridFunction u(cg,all,all,all,2);           // create a grid function with 2 components

//  u.setName("Velocity Stuff");                             // give names to grid function ...
//  u.setName("u Stuff",0);                                  // ...and components
//  u.setName("v Stuff",1);
  Index I1,I2,I3;                                            
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    getIndex(cg[grid].dimension(),I1,I2,I3);                   // assign I1,I2,I3 from dimension
    const realArray & x =cg[grid].center()(I1,I2,I3,axis1);
    const realArray & y =cg[grid].center()(I1,I2,I3,axis2);
    
    u[grid](I1,I2,I3,0)=sin(Pi*x)*cos(Pi*y);                  // component 0 
    u[grid](I1,I2,I3,1)=cos(Pi*x)*sin(Pi*y);                  // component 1 
  }    

  // Here are some other things that we want to save:
  realArray v(20,20);         // distributed array (parallel)
  v=1.;

  realSerialArray drag(100);  // serial array
  drag=3.;

  real time=55.6;
  aString comment = "my restart file";

  HDF_DataBase db;   // make a data base
  
  printF("Mount file example9.hdf and save some data...\n");
  
  db.mount("example9.hdf","I");    // open the data base, I=initialize

  cg.put(db,"My Grid");            // save a grid
  u.put(db,"My Solution");         // save a grid function
  db.putDistributed(v,"v");        // save a distributed array of data
  db.put(drag,"drag");             // save an array of data
  db.put(time,"time");             // save a real number
  db.put(comment,"comment");       // save a string

  cout << "Close the file example9.hdf...\n";
  db.unmount();                    // close the data base


// Now mount the file and read back the data
  
  HDF_DataBase db2;
  cout << "Mount file example9.hdf and read back some data...\n";
  db2.mount("example9.hdf","R");   // mount, R=read-only

  // define new objects to read the data into
  CompositeGrid cg2;
  realCompositeGridFunction u2;

  realSerialArray drag2;
  realArray v2;
  real time2;
  aString comment2;

  // note that the data can be read back in any order
  db2.getDistributed(v2,"v");            // save an array of data
  db2.get(drag2,"drag");            // save an array of data
  db2.get(time2,"time");            // save a real number
  db2.get(comment2,"comment");      // save a string

  cg2.get(db2,"My Grid");

  u2.updateToMatchGrid(cg2);       // *** note: do an update before reading in the grid 
  u2.get(db2,"My Solution");
  u2.display("Here is u2");

  db2.unmount();                    // close the file

  // now check that we have read back the data properly

  if( max(fabs(u2-u))==0. && max(abs(drag2-drag))==0. && fabs(time2-time)==0. && comment==comment2 )
    printF("Objects were successfully read back in.\n");
  else
    printF("ERROR: Objects were NOT successfully read back in.\n");

  Overture::finish();          
  return 0;
}

