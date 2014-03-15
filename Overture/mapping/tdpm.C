#include "Mapping.h"
#include "MappingInformation.h"
#include "Inverse.h"
#include "GenericGraphicsInterface.h"
#include "MappingRC.h"
#include "AnnulusMapping.h"
#include "SquareMapping.h"
//---------------------------------------------------------
//   Test program for the DataPointMapping
//
//
//  mpirun -np 2 -all-local tdpm 
//---------------------------------------------------------

#include "DataPointMapping.h"
#include "CylinderMapping.h"
#include "BoxMapping.h"
#include "LineMapping.h"
#include "display.h"
#include "ParallelUtility.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);
  // Index::setBoundsCheck(off); 

  const MPI_Comm & OV_COMM = Overture::OV_COMM;
  
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);

  Mapping *mapping;
  int n1,n2,n3;
  int domainDimension, rangeDimension;

  fflush(0);
  MPI_Barrier(OV_COMM);
    
  if( true )
  {
    Mapping::debug=3;
    
    DataPointMapping dpm;

    // SquareMapping map;
    AnnulusMapping map;
    // BoxMapping map;
    // CylinderMapping map;
    
    dpm.setMapping(map);
    // const RealArray & grid = dpm.getGrid();
    // display(grid,"grid");
    
    
    RealArray x(1,3),r(1,3),xr(1,3,3),x2(1,3),rx(1,3,3),t(1,3);
    x=0.;
    r=-1.;
    
    if( true )
    {
      // test parallel inverse 
      for(;;) 
      {
        fflush(0);
	if( myid==0 )
	{
	  if( map.getRangeDimension()==2 )
	  {
	    printf("enter a point (r0,r1) to map\n");
	    cin >> r(0,0) >> r(0,1);
	  }
	  else
	  {
	    printf("enter a point (r0,r1,r2) to map\n");
	    cin >> r(0,0) >> r(0,1) >> r(0,2);
	  }
	}
        const int fromProcessor=0;
	broadCast( r,fromProcessor);
	
	if( r(0,0)<-1. )
	  Overture::abort("done");

	dpm.mapS( r,x,xr );
	printf( "map results: myid=%i, r = (%f,%f,%f), x= (%6.3f,%6.3f,%6.3f)\n",
		myid,r(0,0),r(0,1),r(0,2),x(0,0),x(0,1),x(0,2));

        t=-1.;
	dpm.inverseMapS( x,t,rx );
	printf( "inverseMap results: myid=%i, x=(%6.3f,%6.3f,%6.3f), r=(%f,%f,%f) err=(%8.2e,%8.2e,%8.2e) \n\n",
		myid,x(0,0),x(0,1),x(0,2),t(0,0),t(0,1),t(0,2),
                fabs(t(0,0)-r(0,0)),fabs(t(0,1)-r(0,1)),fabs(t(0,2)-r(0,2)) );
      }
    }

    if( false )
    {
      int i,numberOfArrays=0;
      for( i=0; i<100; i++ )
      {
	dpm.inverseMapS(x,r);
// 	if( Array_Domain_Type::getNumberOfArraysInUse() > numberOfArrays )
// 	{
// 	  numberOfArrays=Array_Domain_Type::getNumberOfArraysInUse();
// 	  printf("**** WARNING: number of A++ arrays has increased to = %i \n",numberOfArrays);
// 	}
      }
    }
    

    for( ;; )
    {
      if( map.getRangeDimension()==2 )
      {
        printf("enter a point (x,y) to invert\n");
        cin >> x(0,0) >> x(0,1);
      }
      else
      {
        printf("enter a point (x,y,z) to invert\n");
        cin >> x(0,0) >> x(0,1) >> x(0,2);
      }
      
      // dpm.inverseMapS(x,r,xr);
      dpm.inverseMapS(x,r);
      dpm.mapS(r,x2);
      printf(" x=(%6.2e,%6.2e,%6.2e) r=(%6.2e,%6.2e,%6.2e) map(r)=(%6.2e,%6.2e,%6.2e)\n",
           x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),r(0,2), x2(0,0),x2(0,1),x2(0,2));
    }
    
    return 0;
  }



  // for( int it=0; it<=1; it++ )
  for( int orderOfAccuracy=2; orderOfAccuracy<=4; orderOfAccuracy+=2 )
  {
    printf("\n\n *******************orderOfAccuracy=%i ************* \n",orderOfAccuracy);
    
    for( int map=0; map<3; map++ )
    {
      DataPointMapping dpm;
      
      dpm.setOrderOfInterpolation(orderOfAccuracy);
      
      if( map==0 )
      {
	cout << "\n+++++++++++++++++ Checking a line+++++++++++++++++++++ \n";
	domainDimension=1; 
	rangeDimension=1;
	n1=101, n2=1, n3=1;
	mapping = new LineMapping;
        mapping->setGridDimensions(0,n1);
      }
      else if( map==1 )
      {
	cout << "\n+++++++++++++++++ Checking an Annulus+++++++++++++++++++++ \n";
	domainDimension=2; 
	rangeDimension=2;
	n1=121, n2=21, n3=1; // n1=61, n2=21, n3=1;  // n1=31, n2=11, n3=1;
	mapping = new AnnulusMapping;
        mapping->setGridDimensions(0,n1);
        mapping->setGridDimensions(1,n2);
      }
      else
      {
	cout << "\n+++++++++++++++++ Checking a Cylinder+++++++++++++++++++++ \n";
	domainDimension=3; 
	rangeDimension=3;
	n1=21, n2=11, n3=11;
	mapping = new CylinderMapping(0.,1.,-1.,1.,1.,1.5,0.,0.,0.,domainDimension );
        mapping->setGridDimensions(0,n1);
        mapping->setGridDimensions(1,n2);
        mapping->setGridDimensions(2,n3);
      }
  

      int i1,i2,i3;
      Range I1(0,n1-1),I2(0,n2-1),I3(0,n3-1);

      RealArray r(n1,n2,n3,domainDimension), x(n1,n2,n3,rangeDimension), xr(n1*n2*n3,3,3);

      Range all;
      int axis;

      dpm.setMapping( *mapping ) ;
      
//       const realArray& xy = dpm.getDataPoints(); 
//       display(xy(all,all,all,2),"xy");


//        if( it==1 )
//        {
//          dpm.setBasicInverseOption(Mapping::canDoNothing);
//  	// dpm.useScalarArrayIndexing(TRUE);
//        }

      if( true )
      {
        // double the number of points for evaluation

	n1=2*(n1-1)+1;
	n2=2*(n2-1)+1;
	n3=2*(n3-1)+1;
	
        I1=n1; I2=n2; I3=n3;
	
	r.redim(n1,n2,n3,domainDimension), x.redim(n1,n2,n3,rangeDimension), xr.redim(n1*n2*n3,3,3);
	for( i1=0; i1<n1; i1++ )
	  r(i1,I2,I3,0)=i1/(n1-1.);

	if( domainDimension>1 )
	  for( i2=0; i2<n2; i2++ )
	    r(I1,i2,I3,1)=i2/(n2-1.);

	if( domainDimension>2 )
	  for( i3=0; i3<n3; i3++ )
	    r(I1,I2,i3,2)=i3/(n3-1.);
	
      }
      
      r.reshape(n1*n2*n3,domainDimension);
      x.reshape(n1*n2*n3,rangeDimension);
  
      // call first to get initialization stuff done -- so we have better timings
      dpm.mapS(r,x);
      r=-1.;
      dpm.inverseMapS(x,r);

      RealArray xx(n1,n2,n3,rangeDimension);
      real xMin[3], xMax[3];
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


      // dpm.useScalarArrayIndexing(false);  // ***************************
      if( true )
	dpm.checkMapping();
      
      if( true )
      {
        RealArray x3(x),xr3(xr);
	mapping->mapS(r,x3,xr3);  // "true solution"

        RealArray x2(x), xr2(xr);
	dpm.useScalarArrayIndexing(false); 
	dpm.mapS(r,x,xr);
	dpm.useScalarArrayIndexing(true); 
	x2=-1.;
	xr2=-1.;
	dpm.mapS(r,x2,xr2);

        Range R=rangeDimension, D=domainDimension;
	
        real err =  max(fabs(x2(all,R)-x3(all,R)));
        real xrErr =  max(fabs(xr2(all,R,D)-xr3(all,R,D)));
        
	printf(" Scalar version: err(x)=%8.2e err(xr)=%8.2e\n",err,xrErr);
//  	::display(xr2(all,D,R),"xr2 (scalar)","%5.2f ");
//  	::display(xr3,(all,D,R)"xr3 (true)","%5.2f ");
//  	::display(fabs(xr3(all,D,R)-xr2(all,D,R)),"error","%8.2e ");

        real xDiff = max(fabs(x(all,R)-x2(all,R)));
        real xrDiff = max(fabs(xr(all,R,D)-xr2(all,R,D)));
        err = max(fabs(xr2(all,R,D)-xr3(all,R,D)));
	printf(" Compare new scalar version to non-scalar: xDiff=%8.2e, xrDiff=%8.2e, err=%8.2e\n",xDiff,xrDiff,err);

        real tol=.05; // 2;
	if( orderOfAccuracy==4 )
	{
          int count=0;
	  int maxPrint=10;
	  
	  RealArray & xrt = xr3; // xr
	  int num=n1*n2*n3;
	  for( int i=0; i<num && count<maxPrint; i++ )
	  {
	    for(int m=0; m<rangeDimension; m++ )
	    {
	      for( int n=0; n<domainDimension; n++ )
	      {
		if( fabs(xr2(i,m,n)-xrt(i,m,n))>tol )
		{
		  printf(" i=%i, r=(%9.3e,%9.3e,%9.3e) xr(%i,%i)=%9.3e xr2=(scalar)=%9.3e true=%9.3e error=%8.2e\n",
			 i,r(i,0),
			 (domainDimension>1 ? r(i,1) : 0.),(domainDimension>2 ?r(i,2) : 0.),m,n,
			 xr(i,m,n),xr2(i,m,n), xrt(i,m,n), fabs(xr2(i,m,n)-xrt(i,m,n)));
		}
	      }
	    }
	  }
	}

      }
      

      real time0=getCPU();
      dpm.mapS(r,x);
      real time=getCPU()-time0;

      time0=getCPU();
      mapping->mapS(r,x);
      real time2=getCPU()-time0;
      printf("time for dpm.map(r,x) ................. %e (ratio to Mapping =%5.1f)\n",time,(time2>0. ? time/time2 : 0.));
  
      time0=getCPU();
      dpm.mapS(r,x,xr);
//       if( rangeDimension==1 )
//       {
// 	display(xr,"xr for a line");
//       }
      
    
      time=getCPU()-time0;

      time0=getCPU();
      mapping->mapS(r,x,xr);
      time2=getCPU()-time0;

      printf("time for dpm.map(r,x,xr) .............. %e (ratio to Mapping =%5.1f)\n",time,(time2>0. ? time/time2 : 0.));


      time0=getCPU();
      dpm.inverseMapS(xx,r);
      time=getCPU()-time0;

      time0=getCPU();
      mapping->inverseMapS(xx,r);
      time2=getCPU()-time0;

      printf("time for dpm.inverseMap(x,r) .......... %e (ratio to Mapping =%5.1f)\n",time,(time2>0. ? time/time2 : 0.));
  
      time0=getCPU();
      dpm.inverseMapS(xx,r,xr);
      time=getCPU()-time0;

      time0=getCPU();
      mapping->inverseMapS(xx,r,xr);
      time2=getCPU()-time0;
      printf("time for dpm.inverseMap(x,r,rx) ....... %e (ratio to Mapping =%5.1f)\n",time,(time2>0. ? time/time2 : 0.));
    
    }
  }
  ApproximateGlobalInverse::printStatistics();

  Overture::finish();

  return 0;
}
