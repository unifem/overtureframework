#include "Overture.h"
#include "Integrate.h"
#include "OGTrigFunction.h"  // Trigonometric function
#include "OGPolyFunction.h"  // polynomial function
#include "display.h"

//================================================================================
//  Integrate a function on an overlapping grid.
//================================================================================
int 
main(int argc, char **argv)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile="/home/henshaw/res/ogen/cic.hdf"
    
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
    
  Integrate integrate(cg);

  RealCompositeGridFunction u(cg);
  u=1;
    
  real volume, surfaceArea;
  volume = integrate.volumeIntegral(u);
  surfaceArea = integrate.surfaceIntegral(u);
  printf("Error in volume = %e \n", fabs(volume-( 4.*4.-Pi*SQR(.5) )) );
  printf("Error in surface area = %e \n",fabs(surfaceArea-( 4.*4 + Pi)));
    
  // compute the integral on a part of the boundary.
  int surfaceID=0;    // this number identifies the surface
  int numberOfFaces=1;
  IntegerArray boundary(3,numberOfFaces);
  int side=0, axis=axis2, grid=1;
  boundary(0,0)=side;
  boundary(1,0)=axis;
  boundary(2,0)=grid;
  integrate.defineSurface( surfaceID,numberOfFaces,boundary ); // define the surface
      
  surfaceArea = integrate.surfaceIntegral(u,surfaceID);        

  printf("Grid cic: surfaceArea for cylinder = %e, error=%e \n",surfaceArea,fabs(surfaceArea-Pi));

  return 0;
}
