#include "GL_GraphicsInterface.h"
#include "LineMapping.h"

GL_GraphicsInterface *psPointer;               // create a GL_GraphicsInterface object

#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

//===============================================================================================
//     Example routine demonstrating the use of the GL_GraphicsInterface Class
//
//  This example shows the use of:
//    o prompting for a menu
//    o plotting grids functions and grids
//===============================================================================================


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  psPointer = new GL_GraphicsInterface;
  GL_GraphicsInterface & ps = *psPointer;

  aString nameOfOGFile(80), nameOfDirectory(80);

  real xa=0., xb=1.;
  int numberOfGridPoints=21;
  LineMapping line(xa,xb,numberOfGridPoints);   // mapping for unit interval, [0,1] with 11 grid points
    
  MappedGrid c(line);   // a grid
  c.update();           // compute usual geometry arrays
  c.update(MappedGrid::THEinverseVertexDerivative);           // compute usual geometry arrays

  int i0=c.indexRange()(Start,0);  // get index values for x=0 and x=1
  int i1=c.indexRange()(End,0); 
  Index I1=Range(i0,i1);
  Index I1g = Range(i0-1,i1+1);

  c.mask()=0;
  c.mask()(I1)=1; // **************************************

  Range all;
  realMappedGridFunction u(c,all,2);

  u.setName("rup");               
  u.setName("r",0);                          
  u.setName("u",1);

  real scaleFactor=1.;
//  cout << "Enter the scale factor \n";
//  cin >> scaleFactor;

  u(I1,0)=scaleFactor*sin(2.*Pi*c.center()(I1,Range(0,0),Range(0,0),axis1));
  u(I1,1)=2.*scaleFactor*cos(2.*Pi*c.center()(I1,Range(0,0),Range(0,0),axis1));

  GraphicsParameters psp;               // create an object that is used to pass parameters
    
  aString answer,answer2;
  aString menu[] = { "contour",
		    "grid",
		    "erase",
		    "exit",
                    "" };
  aString menu2[]= { "params","plot","exit","" };

  for(;;)
  {
    ps.getMenuItem(menu,answer);
    if( answer=="contour" )
    {
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      PlotIt::contour(ps,u,psp);  // contour/surface plots
    }
    else if( answer=="grid" )
    {
      psp.set(GI_TOP_LABEL,"My Grid");  // set title
      ps.plot(c,psp);   // plot the composite grid
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  Overture::finish();          
  return 0;
}
