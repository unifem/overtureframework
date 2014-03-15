#include "ModelBuilder.h"
#include "MappingInformation.h"
#include "GL_GraphicsInterface.h"

#define SC (const char *)

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  bool openGraphicsWindow=false; // do not open the graphics window.
  GL_GraphicsInterface gi(openGraphicsWindow,"buildLiner");  


  ModelBuilder modelBuilder;
  CompositeSurface model;
  PointList points;
  
  
  // list of commands, null terminated
  aString commands[] = 
  {
    "ql point 4 0.7000 0.0000",
    "ql point 1 0.400 0.0000",
    "revolve around x-axis",
    "radius for spheres .05",
    "fill with spheres",
    "exit",
    ""  
  };
  
  gi.readCommandsFromStrings(commands);

  
  SphereLoading sphereLoading;

  // specify the radii and volume fractions of the spheres we load into the liner:

  // sphereDistribution(j,0) = radius, sphereDistribution(j,1)=volume fraction
  RealArray & sphereDistribution = sphereLoading.sphereDistribution;

  const int numberOfSphereRadii=3;
  sphereDistribution.redim(numberOfSphereRadii,3);
  // radius, volume-fraction and mass fraction
  sphereDistribution(0,0)=.05;     sphereDistribution(0,1)=.2;  sphereDistribution(0,2)=.2;   
  sphereDistribution(0,0)=.025;    sphereDistribution(0,1)=.3;	sphereDistribution(0,2)=.3;
  sphereDistribution(0,0)=.0125;   sphereDistribution(0,1)=.4;	sphereDistribution(0,2)=.4;
   
  modelBuilder.linerGeometry(model,gi,points,sphereLoading);

  RealArray & sphereCenter = sphereLoading.sphereCenter;
  RealArray & sphereRadius = sphereLoading.sphereRadius;
  RealArray & sphereVelocity=sphereLoading.sphereVelocity; // sphereVelocity(i,0:2) : initial velocity of the sphere
  RealArray & sphereStartTime=sphereLoading.sphereStartTime;
  const int numberOfSpheres=sphereCenter.getLength(0);
  for( int i=0; i<numberOfSpheres; i++ )
  {
    printf(" sphere i=%i, center=(%10.3e,%10.3e,%10.3e) radius=%10.3e velocity=(%10.3e,%10.3e,%10.3e) t0=%10.3e\n",i,
           sphereCenter(i,0),sphereCenter(i,1),sphereCenter(i,2),sphereRadius(i),
           sphereVelocity(i,0),sphereVelocity(i,1),sphereVelocity(i,2),sphereStartTime(i));
  }

  Overture::finish();          
  return 0;
}



