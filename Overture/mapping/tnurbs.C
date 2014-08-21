#include "Mapping.h"
#include "MappingInformation.h"
#include "GL_GraphicsInterface.h"
#include "MappingRC.h"
#include "NurbsMapping.h"
#include "CylinderMapping.h"
#include "display.h"

extern real nurbTimeEvaluate;
extern real nurbsBasisTime;
extern real nurbApproximateInverseTime;
extern real nurbApproximateInverseTimeNew;

extern real nurbsEvalTime;

//---------------------------------------------------------
//   Test program for timing the NurbsMapping
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

  const int numberOfMapsToCheck=4;
  Mapping *mapPointer[numberOfMapsToCheck];

  NurbsMapping nurbs1(1,2);   // 2D curve
  nurbs1.setName(Mapping::mappingName,"2D nurbs curve");
  mapPointer[0]=&nurbs1;

  NurbsMapping nurbs2(1,3);   // 3D curve
  nurbs2.setName(Mapping::mappingName,"3D nurbs curve");
  mapPointer[1]=&nurbs2;

  NurbsMapping nurbs3(2,3);
  nurbs3.setName(Mapping::mappingName,"nurbs surface");
  mapPointer[2]=&nurbs3;

  CylinderMapping cyl;
  NurbsMapping nurbs4;
  nurbs4.interpolateVolume(cyl.getGrid());
  nurbs4.setName(Mapping::mappingName,"nurbs volume");
  mapPointer[3]=&nurbs4;
    

  for( int map=0; map<numberOfMapsToCheck; map++ )
  {
    Mapping & mapping = *mapPointer[map];
    printf("\n>>>>>>>>>>>>>>>>Check mapping %s <<<<<<<<<<<<<<\n\n",
             (const char*)mapping.getName(Mapping::mappingName));
      
    mapping.checkMapping();

    int domainDimension=mapping.getDomainDimension();
    int rangeDimension=mapping.getRangeDimension();
      
    n1=11;
    n2=11;
    n3=11;

    int i1,i2,i3=0;
    RealArray r(n1,n2,n3,domainDimension), x(n1,n2,n3,rangeDimension), 
              xr(n1*n2*n3,3,3), r2(n1*n2*n3,domainDimension);
    for( i3=0; i3<n3; i3++ )
    for( i2=0; i2<n2; i2++ )
      for( i1=0; i1<n1; i1++ )
      {
	r(i1,i2,i3,0)=(i1+1)/(n1+1.);
	if( domainDimension>1 )
	  r(i1,i2,i3,1)=(i2+1)/(n2+1.);
	if( domainDimension>2 )
	  r(i1,i2,i3,2)=(i3+1)/(n3+1.);
      }

    mapping.mapGrid(r,x);

    r.reshape(n1*n2*n3,domainDimension);
    x.reshape(n1*n2*n3,rangeDimension);
  
    // call first to get initialization stuff done -- so we have better timings
    mapping.map(r,x);
    r2=-1.;
    mapping.inverseMap(x,r2);
    printf("Maximum error in `r - inverseMap(map(r))' = %6.2e\n",max(fabs(r-r2)));

    Range I1(0,n1-1),I2(0,n2-1),I3(0,n3-1);
    RealArray xx(n1,n2,n3,rangeDimension), rr(n1*n2*n3,domainDimension), rrxx(n1*n2*n3,3,3);
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

    for( int it=0; it<3; it++ )
    {
      if( it==0 )
      {
        if( mapping.getDomainDimension()==3 ) continue;
	
        printf("\n ***** use vector evaluation *****\n");
        ((NurbsMapping&)mapping).use_kk_nrb_eval=false;
        NurbsMapping::useScalarEvaluation=false; 
	
      }
      else if( it==1 )
      {
        printf("\n ***** use scalar evaluation *****\n");
        ((NurbsMapping&)mapping).use_kk_nrb_eval=false;
        NurbsMapping::useScalarEvaluation=true;
      }
      else 
      {
        printf("\n ***** use scalar evaluation and Eleven evaluator *****\n");
        NurbsMapping::useScalarEvaluation=true;
        ((NurbsMapping&)mapping).use_kk_nrb_eval=true;
      }
      
      real time0=getCPU();  nurbTimeEvaluate=0.; nurbsBasisTime=0.; nurbsEvalTime=0.;
      mapping.map(r,x);
      real time=getCPU()-time0;
      printf("time for map(r,x) ............. %e (evaluate=%8.2e, basis=%8.2e,Eleven=%8.2e) \n",time,nurbTimeEvaluate,nurbsBasisTime,nurbsEvalTime);

      time0=getCPU(); nurbsEvalTime=0.;
      mapping.map(r,x,xr);
      real time1=getCPU()-time0;

      printf("time for map(r,x,xr) ............. %e (ratio to map(r,x)   =%5.1f)(Eleven=%8.2e)\n",
                   time1,(time>0. ? time1/time:0.),nurbsEvalTime);

      time0=getCPU();
      mapping.inverseMap(xx,rr);
      real time2=getCPU()-time0;

      time0=getCPU();
      mapping.inverseMap(xx,rr,rrxx);
      real time3=getCPU()-time0;

      time0=getCPU();
      mapping.map(rr,xx);
      time=getCPU()-time0;

      printf("time for inverseMap(xx,rr)........ %e (ratio to map(r,x)   =%5.1f, approx=%8.2e,%8.2e)\n",
              time2,(time>0.? time2/time : 0.),nurbApproximateInverseTime,nurbApproximateInverseTimeNew);
  
      time0=getCPU();
      mapping.map(rr,xx,rrxx);
      time1=getCPU()-time0;

      printf("time for inverseMap(x,r,rx) ...... %e (ratio to map(r,x,xr)=%5.1f, approx=%8.2e,%8.2e)\n",
          time3,(time1>0. ?time3/time1:0.),nurbApproximateInverseTime,nurbApproximateInverseTimeNew);
    }
    
  }

  ApproximateGlobalInverse::printStatistics();
  return 0;
}

