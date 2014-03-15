#include "Overture.h"
  
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------ \n");
  printf(" Read an overlapping grid from a data base file               \n");
  printf(" Make a grid function, assign it values and then display it   \n");
  printf(" ------------------------------------------------------------ \n");

  aString nameOfOGFile;
  cout << "Enter the name of the overlapping grid data base file " << endl;
  cin >> nameOfOGFile;
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  realCompositeGridFunction u(cg);                      // create a composite grid function
  u=0.;                                                 // initialize to zero
  Index I1,I2,I3;                                       // A++ Index object
  
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3);                  // assign I1,I2,I3 from indexRange
    u[grid](I1,I2,I3)=sin(cg[grid].vertex()(I1,I2,I3,axis1))   // assign all interior points on this
                     *cos(cg[grid].vertex()(I1,I2,I3,axis2));  // component grid
  }    
  u.display("here is u=sin(x)*cos(y)");

  Overture::finish();          
  return 0;
}
