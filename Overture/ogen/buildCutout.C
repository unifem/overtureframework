#include "Ogen.h"
#include "MappingInformation.h"

int Ogen::
buildCutout(CompositeGrid & cg, MappingInformation & cutMapInfo2 )
// ========================================================================
// Description:
//    Cut holes in an existing overlapping grid. The holes are are cut my
// the Mappings conatined in the cutMapInfo2.mappingList.
// ========================================================================
{
  
  real time0=getCPU();
  if( info & 4 ) printf("build a cutout grid...\n");

  Index I1,I2,I3;
  Range R, Rx(0,cg.numberOfDimensions()-1);
  realArray x;
  IntegerArray ia, crossings;
  
  int i, iv[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];
  real x0,x1,x2;

  Mapping & map = cutMapInfo2.mappingList[0].getMapping();
  MappedGrid g(map);
  g.update(MappedGrid::THEboundingBox);
  
  // use this face of the Mapping:
  int cutSide=0;
  int cutAxis=cg.numberOfDimensions()-1;
  

  if( TRUE || info & 4 ) printf("cutting holes with mapping: %s ...\n",(const char*)map.getName(Mapping::mappingName));

  for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
  {
    MappedGrid & g2 = cg[grid2];
    Mapping & map2 = g2.mapping().getMapping();
    if( map.intersects( map2, cutSide,cutAxis,-1,-1,.1 ) )  // check this ****
    {
      const realArray & center = g2.center();   // *** do we want center or vertex here ????
      intArray & mask = g2.mask();
      getIndex(extendedGridIndexRange(g2),I1,I2,I3); 
      // make a list of the grid points that lie withing the bounding box of the cut-mapping.
      
      R=Range(0,I1.length()*I2.length()*I3.length());
      ia.redim(R,3);
      int numberToCheck=0;
      const RealArray & boundingBox = g.boundingBox();  
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    if( mask(i1,i2,i3)!=0 )
	    {
	      x0=center(i1,i2,i3,axis1);
	      x1=center(i1,i2,i3,axis2);
	      if( cg.numberOfDimensions() > 2 )
		x2=center(i1,i2,i3,axis3);
	      if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) &&
		  x1 >= boundingBox(Start,axis2) && x1 <= boundingBox(End,axis2) &&
		  ( cg.numberOfDimensions()==2 || 
		    (x2 >= boundingBox(Start,axis3) && x2 <= boundingBox(End,axis3)) ) )
	      {
		ia(numberToCheck,0)=i1;
		ia(numberToCheck,1)=i2;
		ia(numberToCheck,2)=i3;
		numberToCheck++;
	      }
	    }
	  }
	}
      }
      if( numberToCheck>0 )
      {
	R=Range(0,numberToCheck-1);
	x.redim(R,Rx);
	for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
	  x(R,axis)=center(ia(R,0),ia(R,1),ia(R,2),axis);
	x.display("check these points");
	crossings.redim(R);
	crossings=0;
        #ifndef USE_PPP
	map.approximateGlobalInverse->
	  countCrossingsWithPolygon(x, crossings, cutSide,cutAxis
				    // ,xCross, 
				    // mask, MappedGrid::ISdiscretizationPoint | MappedGrid::ISinterpolationPoint,
				    // maskRatio[0],maskRatio[1],maskRatio[2] 
	    );
        #else
	  Overture::abort("ERROR: fix me for parallel");
        #endif        
      
	crossings.display("Here are the crossings");
      
	where( crossings % 2 == 1 )
	{
	  mask(ia(R,0),ia(R,1),ia(R,2))=0;
	}
	if( holePoint.getLength(0) <= numberOfHolePoints+numberToCheck )
	  holePoint.resize(holePoint.getLength(0)*2+numberToCheck,Rx);
    
	for( i=0; i<numberToCheck; i++ )
	{
	  if( mask(ia(i,0),ia(i,1),ia(i,2))==0 )
	  {
            for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
	      holePoint(numberOfHolePoints++,axis) = x(i,axis);
	  }
	}
      }
      

    }
  }
  return 0;
}
