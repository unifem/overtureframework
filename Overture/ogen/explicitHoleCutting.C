#include "Overture.h"
#include "Ogen.h"
#include "display.h"
#include "ParallelUtility.h"
#include "ExplicitHoleCutter.h"

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// =================================================================================
/// \brief Cut holes with user defined explicit hole cutters
/// 
// =================================================================================
int Ogen::
explicitHoleCutting( CompositeGrid & cg )
{
  
  if( explicitHoleCutter.size()==0 )
  {
    return 0;
  }


  // ******************************************
  // --------- Explicit hole cutting ----------
  // -- Cut holes with user defined mappings --
  // ******************************************

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfDimensions = cg.numberOfDimensions();
  int maxNumberOfHolePoints = holePoint.getLength(0);

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int  iv[3], &i1 = iv[0],  &i2= iv[1], &i3 = iv[2];
  real xv[3]={0.,0.,0.};

  IntegerArray ia;
  RealArray xa,ra;

  // loop over explicit hole cutters
  for( int hc=0; hc<explicitHoleCutter.size(); hc++ )
  {
    ExplicitHoleCutter & holeCutter = explicitHoleCutter[hc];
    Mapping & cutMapping = holeCutter.holeCutterMapping.getMapping();
    const IntegerArray & mayCutHoles = holeCutter.mayCutHoles;

    // loop over grids
    for( int grid=numberOfBaseGrids-1; grid>=0; grid-- )
    {
      if( !mayCutHoles(grid) ) 
        continue;  // this hole cutter does not cut holes in this grid

      MappedGrid & g = cg[grid];
      Mapping & map = g.mapping().getMapping();

      if( cutMapping.intersects(map) )
      {
	printF("Explicit hole cutting in grid %i (%s) using cutter %i (%s).\n",
	       grid,(const char*)g.getName(),hc,(const char*)cutMapping.getName(Mapping::mappingName));
	  
	const bool isRectangular = g.isRectangular();
	OV_GET_SERIAL_ARRAY_CONDITIONAL(real,g.vertex(),vertexLocal,isRectangular);
	OV_GET_SERIAL_ARRAY_CONST(int,g.mask(),maskLocal);
	int * maskLocalp = maskLocal.Array_Descriptor.Array_View_Pointer2;
	const int maskLocalDim0=maskLocal.getRawDataSize(0);
	const int maskLocalDim1=maskLocal.getRawDataSize(1);
        #define MASK(i0,i1,i2) maskLocalp[i0+maskLocalDim0*(i1+maskLocalDim1*(i2))]	  

	// get parameters that define the Cartesian grid points (for rectangular grids)
	real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
	int iv0[3]={0,0,0}; //
	if( isRectangular )
	{
	  g.getRectangularGridParameters( dvx, xab );
	  for( int dir=0; dir<g.numberOfDimensions(); dir++ )
	    iv0[dir]=g.gridIndexRange(0,dir);
	}
        #define XAB(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

	// --- Make a list of points on grid that lie inside the bounding box of the cutMapping ---
	getIndex(g.gridIndexRange(),I1,I2,I3); // is gridIndexRange correct ? 
	int includeGhost=1;
	bool ok=ParallelUtility::getLocalArrayBounds(g.mask(),maskLocal,I1,I2,I3,includeGhost);

	// for IA, XA macros:
	int *iap; 
	real *xap;
	int iaDim0, xaDim0;

	int j=0; // counts points on grid to check for being inside the cutter mapping 
	if( ok )
	{
	  const int numPoints = I1.getLength()*I2.getLength()*I3.getLength();
	  int maxToCut = max(100,numPoints/4); // a guess, this will be increased as needed

	  ia.redim(maxToCut,3);
	  xa.redim(maxToCut,numberOfDimensions);
          #undef IA
	  iap = ia.Array_Descriptor.Array_View_Pointer1;
	  iaDim0=ia.getRawDataSize(0);
          #define IA(i0,i1) iap[i0+iaDim0*(i1)]
          #undef XA
	  xap = xa.Array_Descriptor.Array_View_Pointer1;
	  xaDim0=xa.getRawDataSize(0);
          #define XA(i0,i1) xap[i0+xaDim0*(i1)]

	  // Get the bounding box for the explicit hole cutter mapping
	  RealArray boundingBox;
	  boundingBox=cutMapping.getBoundingBox();   
	  const real *pbb=boundingBox.getDataPointer();
          #define cutterBoundingBox(side,axis) pbb[(side)+2*(axis)]

	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    bool inside=true;
	    for( int axis=0; axis<numberOfDimensions; axis++ )
	    {
	      if( isRectangular ) 
		xv[axis]= XAB(iv,axis);
	      else  
		xv[axis]= vertexLocal(i1,i2,i3,axis);
	      if( xv[axis]<cutterBoundingBox(0,axis) || xv[axis]>cutterBoundingBox(1,axis) )
	      {
		inside=false;
		break;
	      }
	    }
	    if( inside )
	    {
	      if( j>=maxToCut )
	      { // increase sizes of ia,xa arrays
		maxToCut = maxToCut*2;
		ia.resize(maxToCut,3);
		xa.resize(maxToCut,numberOfDimensions);

		iap = ia.Array_Descriptor.Array_View_Pointer1;
		iaDim0=ia.getRawDataSize(0);
		xap = xa.Array_Descriptor.Array_View_Pointer1;
		xaDim0=xa.getRawDataSize(0);
	      }
	      

	      IA(j,0)=i1; IA(j,1)=i2; IA(j,2)=i3;
	      for( int axis=0; axis<numberOfDimensions; axis++ )
	      {
		XA(j,axis)=xv[axis];
	      }
	      j++;
	    }
	  } //end for_3d
	    
	} // end if ok
	int numToCheck=j;
	  
	// Invert the points 
	if( numToCheck>0 )
	{
	  ra.redim(numToCheck,numberOfDimensions);
	  ra=-1.;
	}
	else
	{
	  xa.redim(0);
	  ra.redim(0);
	}
	Range R=numToCheck, Rx=numberOfDimensions;
	cutMapping.inverseMapS(xa(R,Rx),ra);

        #undef RA
	real * rap = ra.Array_Descriptor.Array_View_Pointer1;
	int raDim0=ra.getRawDataSize(0);
        #define RA(i0,i1) rap[i0+raDim0*(i1)]

	// cut holes at points that lie inside the explicit hole cutter mapping
	int numCut=0;
	for( int j=0; j<numToCheck; j++ )
	{
	  bool inside=true;
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	  {
	    // We cut points that are inside or on the boundary of the cutter mapping
	    // -- later we could allow for cutters that are just closed triangular surfaces, for example.
	    if( fabs(RA(j,axis)-.5)>.5 )
	    {
	      inside=false;
	      break;
	    }
	  }
	  if( inside )
	  {
	    MASK(IA(j,0),IA(j,1),IA(j,2))=0;
	      
	    if( debug & 2 )
	    {
	      fprintf(plogFile,"Cutting hole with explicit hole cutter %i in grid %i at (%i,%i,%i).\n",
		      hc,grid,IA(j,0),IA(j,1),IA(j,2));
	    }
	      
	    if( numberOfHolePoints>= maxNumberOfHolePoints )
	    {
	      maxNumberOfHolePoints*=2; 
	      holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
	      // printf(" ... increasing maxNumberOfHolePoints to %i\n",maxNumberOfHolePoints);
	    }

	    for( int axis=0; axis<numberOfDimensions; axis++ )
	      holePoint(numberOfHolePoints,axis)=XA(j,axis);

	    holePoint(numberOfHolePoints,numberOfDimensions)=grid;
	    numberOfHolePoints++;
	    numCut++;
	  }
	} // end for j 
	printF(" ... numToCheck=%i, number of holes cut=%i.\n",numToCheck,numCut);
	  
      } // end if cut map intersects grid 
    } // end for grid
      

  } // end loop over explicit hole cutters
    
  
  return 0;
  
}
