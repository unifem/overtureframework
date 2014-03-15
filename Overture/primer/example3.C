#include "Overture.h"

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------ \n");
  printf(" Demonstrate how to interpolate a compositeGridFunction       \n");
  printf(" ------------------------------------------------------------ \n");

  aString nameOfOGFile;
  cout << "Enter the name of the overlapping grid data base file " << endl;
  cin >> nameOfOGFile;
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  realCompositeGridFunction u(cg);        // create a composite grid function
  u=0.;                                   // initialize to zero
  Index I1,I2,I3;                         // A++ Index object
  
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3);                    // assign I1,I2,I3
    where( cg[grid].mask()(I1,I2,I3) > 0 )                       // only assign points with mask>0
      u[grid](I1,I2,I3)=sin(cg[grid].vertex()(I1,I2,I3,axis1))   // do not assign interpolation points
                       *cos(cg[grid].vertex()(I1,I2,I3,axis2));  
  }    
  u.display("here is u=sin(x)*cos(y) before interpolation");

  // Interpolant interpolant(cg);      // Make an interpolant
  Interpolant & interpolant = *new Interpolant(cg);               // do this instead for now. 
  interpolant.interpolate(u);       // interpolate
  u.display("here is u after interpolation");

  u.interpolate();     // another way to interpolate, same result as above
  u.display("here is u after interpolate, version 2");
  
  Overture::finish();          
  return 0;  
}
