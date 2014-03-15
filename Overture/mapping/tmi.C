//---------------------------------------------------------
// Test program for inverting mappings.
//
//   o check that the inverse appears to be working
//   o Time the mapping and inverse.
//
// srun -N1 -n1 -ppdebug tmi -numPoints=1001 -mapToCheck=sphere >! tmi.NP1.out
// srun -N1 -n2 -ppdebug tmi -numPoints=1001 -mapToCheck=sphere >! tmi.NP2.out
// srun -N1 -n4 -ppdebug tmi -numPoints=1001 -mapToCheck=sphere >! tmi.NP4.out
//---------------------------------------------------------


#include "Mapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "SmoothedPolygon.h"
#include "CylinderMapping.h"
#include "SphereMapping.h"
#include "display.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
    
  int len=0;
  int numberOfPoints=11;
  aString mapToCheck="all";
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( len=arg.matches("-debug=") )
      {
	sScanF(arg(len,arg.length()-1),"%i",&Mapping::debug);
	printF("Setting Mapping::debug=%i\n",Mapping::debug);
      }
      else if( len=arg.matches("-numPoints=") )
      {
	sScanF(arg(len,arg.length()-1),"%i",&numberOfPoints);
	printF("Setting numberOfPoints=%i\n",numberOfPoints);
      }
      else if( len=arg.matches("-mapToCheck=") )
      {
	mapToCheck=arg(len,arg.length()-1);
	printF("Setting mapToCheck=%s\n",(const char*)mapToCheck);
      }
      else
      {
	printF("Unknown option = [%s]\n",(const char*)arg);
      }
    }
  }


  // Mapping::debug=31;
  
  Mapping *mapping;
  int n1,n2,n3;
  int domainDimension, rangeDimension;

  const int numberOfMapsToCheck=5;
  Mapping *mapPointer[numberOfMapsToCheck];

  int mapCount=0;
  SquareMapping square;    
  AnnulusMapping annulus;  
  SmoothedPolygon sp;      
  CylinderMapping cyl;     

  SphereMapping sphere; 
  sphere.setGridDimensions(0,31); // phi 
  sphere.setGridDimensions(1,61); // theta
  sphere.setGridDimensions(2,11); // r     

  if( mapToCheck=="all" || mapToCheck=="square" ) mapPointer[mapCount++]=&square;
  if( mapToCheck=="all" || mapToCheck=="annulus" ) mapPointer[mapCount++]=&annulus;
  if( mapToCheck=="all" || mapToCheck=="sp" ) mapPointer[mapCount++]=&sp;
  if( mapToCheck=="all" || mapToCheck=="cyl" ) mapPointer[mapCount++]=&cyl;
  if( mapToCheck=="all" || mapToCheck=="sphere" ) mapPointer[mapCount++]=&sphere;   




  RealArray rr(1,3), xx(1,3), xxr(1,3,3);
  xx=0.;
  rr=0.;
  
  for( int it=0; it<2; it++ )
  {
    // second time through turn off the Analytic inverse (if it exists)

    if( it==0 )
      printF(" -------------------------------------------------------------------\n");
    else
      printF(" ------------------Turn off the analytic inverse--------------------\n");

    for( int map=0; map<mapCount; map++ )
    {
      Mapping & mapping = *mapPointer[map];

      if( it==1 )
        mapping.setBasicInverseOption(Mapping::canDoNothing);
      
      int domainDimension=mapping.getDomainDimension();
      int rangeDimension=mapping.getRangeDimension();
      
      n1=numberOfPoints;
      n2=domainDimension>1 ? 11 : 1;
      n3=domainDimension>2 ? 11 : 1;

      int i1,i2,i3=0;

      RealArray r(n1,n2,n3,domainDimension), x(n1*n2*n3,rangeDimension), xr(n1*n2*n3,3,3);
      RealArray r2(n1*n2*n3,domainDimension);
      for( i3=0; i3<n3; i3++ )
      {
	for( i2=0; i2<n2; i2++ )
	{
	  for( i1=0; i1<n1; i1++ )
	  {
	    r(i1,i2,i3,0)=(i1+1)/(n1+1.);  // avoid periodic boundary so we can compare r and r2 below
	    if( domainDimension>1 )
	      r(i1,i2,i3,1)=(i2+1)/(n2+1.);
	    if( domainDimension>2 )
	      r(i1,i2,i3,2)=(i3+1)/(n3+1.);
	  }
	}
      }

      r.reshape(n1*n2*n3,domainDimension);

      
      mapping.inverseMapS(xx,rr,xxr);   // initial call so timings are more accurate below

      real time0=getCPU();
      mapping.mapS(r,x,xr);
      real time=getCPU()-time0;

      time0=getCPU();
      r2=-1.;
      mapping.inverseMapS(x,r2,xr);
      real time1=getCPU()-time0;
      // display(r,"r");
      // display(r2,"r2");

      real maxError=max(fabs(r-r2));

      printF(">>>Check mapping %s, %i points, inverseIsDistributed=%i\n",
             (const char*)mapping.getName(Mapping::mappingName),r.getLength(0),
	     (int)mapping.usesDistributedInverse());

      printF("time for map(r,x,xr) .............. %e \n",time);
      printF("time for inverseMap(x,r,rx)........ %e (ratio to map(r,x,xr)=%5.1f) error=%6.1e\n",
               time1,(time>0.? time1/time : 0.),maxError);
    
    }
  }

  ApproximateGlobalInverse::printStatistics();

  Overture::finish();          
  return 0;
}
