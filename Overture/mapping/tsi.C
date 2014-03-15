#include "Mapping.h"
#include "MappingInformation.h"
#include "GenericGraphicsInterface.h"
//#include "GL_GraphicsInterface.h"
#include "MappingRC.h"
#include "NurbsMapping.h"
#include "CylinderMapping.h"
#include "display.h"

//---------------------------------------------------------
//   Test program for inverting surfaces
//---------------------------------------------------------
int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    
  // This next call will allow the Mappings to be read in from the data-base file
//   initializeMappingList();

  Mapping *mapping;
  int n1,n2,n3;
  int domainDimension, rangeDimension;

  const int numberOfMapsToCheck=2;
  Mapping *mapPointer[numberOfMapsToCheck];

  CylinderMapping cyl(0.,1.,-1.,1.,1.,1.5,0.,0.,0., 2 ); // surface
  mapPointer[0]=&cyl;

  NurbsMapping nurbs(2,3);
  mapPointer[1]=&nurbs;
    
  if( FALSE )
  {
    CylinderMapping cyl2(0.,1.,-1.,1.,1.,1.5,0.,0.,0., 2 );

    cyl.setBasicInverseOption(Mapping::canDoNothing);

    RealArray x(1,3),r(1,3),r2(1,3), xr(1,3,3),x2(1,3);
    x=0.;
    r=0.;
    

    for( ;; )
    {
      printf("enter a point (x,y,z) to invert\n");
      cin >> x(0,0) >> x(0,1) >> x(0,2);
      
      cyl.inverseMap(x,r);
      cyl2.inverseMap(x,r2); // this is an analtic inverse
      cyl.map(r2,x2);
      printf(" x=(%6.2e,%6.2e,%6.2e) r=(%6.2e,%6.2e) rExact=(%6.2e,%6.2e) x2=(%9.4e,%9.4e,%9.4e)\n",
           x(0,0),x(0,1),x(0,2),r(0,0),r(0,1), r2(0,0),r2(0,1), x2(0,0),x2(0,1),x2(0,2));
    }
    
    return 0;
  }
    

  for( int map=0; map<numberOfMapsToCheck; map++ )
  {
    Mapping & mapping = *mapPointer[map];
    printf(">>>Check mapping %s \n",(const char*)mapping.getName(Mapping::mappingName));
      
    int domainDimension=mapping.getDomainDimension();
    int rangeDimension=mapping.getRangeDimension();
      
    n1=11;
    n2=11;
    n3=11;

    int i1,i2,i3=0;
    RealArray r(n1,n2,1,domainDimension), x(n1,n2,1,rangeDimension), xr(n1*n2,3,3), r2(n1*n2,domainDimension);
    for( i2=0; i2<n2; i2++ )
      for( i1=0; i1<n1; i1++ )
      {
	r(i1,i2,i3,0)=(i1+1)/(n1+1.);
	if( domainDimension>1 )
	  r(i1,i2,i3,1)=(i2+1)/(n2+1.);
      }

    mapping.mapGrid(r,x);

    r.reshape(n1*n2,domainDimension);
    x.reshape(n1*n2,rangeDimension);
  
    // call first to get initialization stuff done -- so we have better timings
    mapping.map(r,x);
    r2=-1.;
    mapping.inverseMap(x,r2);
    printf("Maximum error in `r - inverseMap(map(r))' = %6.2e\n",max(fabs(r-r2)));

    Range I1(0,n1-1),I2(0,n2-1),I3(0,n3-1);
    RealArray xx(n1,n2,n3,rangeDimension), rr(n1*n2*n3,2), rrxx(n1*n2*n3,3,3);
    real xMin[3], xMax[3];
    int axis;
    for( axis=0; axis<rangeDimension; axis++ )
    {
      xMin[axis]=min(x(nullRange,axis));
      xMax[axis]=max(x(nullRange,axis));
      xMin[axis]-=.2*(xMax[axis]-xMin[axis]);
      xMax[axis]+=.2*(xMax[axis]-xMin[axis]);
	
    }
    // choose some random points to invert
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	xx(I1,i2,i3,axis1).seqAdd(xMin[0],(xMax[0]-xMin[0])/(n1-1));
    if( rangeDimension>1 )
    {
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  xx(i1,I2,i3,axis2).seqAdd(xMin[1],(xMax[1]-xMin[1])/(n2-1));
    }
    if( rangeDimension>2 )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  xx(i1,i2,I3,axis3).seqAdd(xMin[2],(xMax[2]-xMin[2])/(n3-1));
    }
    xx.reshape(n1*n2*n3,rangeDimension);

    real time0=getCPU();
    mapping.map(r,x);
    real time=getCPU()-time0;
    printf("time for map(r,x) ............. %e \n",time);

    time0=getCPU();
    mapping.map(r,x,xr);
    real time1=getCPU()-time0;

    printf("time for map(r,x,xr) ............. %e (ratio to map(r,x)   =%5.1f)\n",time1,(time>0. ? time1/time:0.));

    time0=getCPU();
    mapping.inverseMap(xx,rr);
    real time2=getCPU()-time0;

    time0=getCPU();
    mapping.inverseMap(xx,rr,rrxx);
    real time3=getCPU()-time0;

    time0=getCPU();
    mapping.map(rr,xx);
    time=getCPU()-time0;

    time0=getCPU();
    mapping.map(rr,xx,rrxx);
    time1=getCPU()-time0;

    printf("time for inverseMap(xx,rr)........ %e (ratio to map(r,x)   =%5.1f)\n",time2,(time>0.? time2/time : 0.));
  
    printf("time for inverseMap(x,r,rx) ...... %e (ratio to map(r,x,xr)=%5.1f)\n",time3,(time1>0. ?time3/time1:0.));
    
  }

  ApproximateGlobalInverse::printStatistics();
  return 0;
}
