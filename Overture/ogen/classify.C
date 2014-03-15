#include "Ogen.h"
#include "Overture.h"
#include "display.h"
#include "conversion.h"
#include "ParallelUtility.h"
#include "CanInterpolate.h"


static const int ISneededPoint = CompositeGrid::ISreservedBit2;  // from Cgsh.h

// Define a macro to index an A++ array with 3 dimensions *NOTE* a legal macro  --> #define MASK
// #define DEF_ARRAY_MACRO_3D(int,mask,MASK) \
//   int * mask ## p = mask.Array_Descriptor.Array_View_Pointer2;\
//   const int mask ## Dim0=mask.getRawDataSize(0);\
//   const int mask ## Dim1=mask.getRawDataSize(1);\
// #define MASK(i0,i1,i2) mask ## p[i0+mask ## Dim0*(i1+mask ## Dim1*(i2))]

// Macro to extract a local array with ghost boundaries
//  type = int/float/double/real
//  xd = distributed array
//  xs = serial array 
#ifdef USE_PPP
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
 #define GET_LOCAL_CONST(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray & xs = xd
 #define GET_LOCAL_CONST(type,xd,xs)\
    const type ## SerialArray & xs = xd
#endif

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)


/* ---
int
whereIndex( const intArray & mask, intArray & ia1, intArray & ia2, intArray & ia3 )
// =======================================================================================
//  /Description:
//     Return indirect addressing arrays corresponding to TRUE points in a mask.
//  If R=Range(0,numberOfTruePoints-1) where numberOfTruePoints is the value returned by
//  this function then the values mask(ia1(R),ia2(R),ia3(R)) will all be TRUE, 
//  all other values in mask will be FALSE.
//
// /mask (input):  array of mask values, 
// /ia1,ia2,i3 (intput/output) : indirect addressing arrays marking all points where mask==TRUE. These
//   arrays should be dimensioned large enough to hold the result.
// /Return value: number of points in the indirect addressing arrays.
// =======================================================================================
{
  printF("whereIndex: mask.getBase(0)=%i \n",mask.getBase(0));
  
  int i=0;
  for( int i3=mask.getBase(2); i3<=mask.getBound(2); i3++ )
  {
    for( int i2=mask.getBase(1); i2<=mask.getBound(1); i2++ )
    {
      for( int i1=mask.getBase(0); i1<=mask.getBound(0); i1++ )
      {
	if( mask(i1,i2,i3)  )
	{                    
	  ia1(i)=i1;
	  ia2(i)=i2;
	  ia3(i)=i3;
	  i++;
	}
      }
    }
  }
  return i;
}
--- */

bool Ogen::
canDiscretize( MappedGrid & g, const int iv[3], bool checkOneSidedAtBoundaries /* =true */ )
//
//  Determine whether the given point on the grid can be used
//  for discretisation.
//
//  **** this function assumes the mask value is set at periodic points 
{
  const int numberOfDimensions=g.numberOfDimensions();

  int ab[3][2];
#define iab(side,axis) ab[(axis)][(side)]
  const int *boundaryConditionp = g.boundaryCondition().Array_Descriptor.Array_View_Pointer1;
#define boundaryCondition(i0,i1) boundaryConditionp[i0+2*(i1)]
  const int *boundaryDiscretizationWidthp = g.boundaryDiscretizationWidth().Array_Descriptor.Array_View_Pointer1;
#define boundaryDiscretizationWidth(i0,i1) boundaryDiscretizationWidthp[i0+2*(i1)]
  const int *eirp = g.extendedIndexRange().Array_Descriptor.Array_View_Pointer1;
#define extendedIndexRange(i0,i1) eirp[i0+2*(i1)]
  const int *dwp = g.discretizationWidth().Array_Descriptor.Array_View_Pointer0;
#define discretizationWidth(i0) dwp[i0]
  const int *isPeriodicp = g.isPeriodic().Array_Descriptor.Array_View_Pointer0;
#define isPeriodic(i0) isPeriodicp[i0]

//
//  Determine the stencil of points to check.
//
    for( int axis=0; axis<3; axis++ )
    {
      if( axis<numberOfDimensions )
      {
	iab(0,axis) = iv[axis] - discretizationWidth(axis) / 2;
	iab(1,axis) = iv[axis] + discretizationWidth(axis) / 2;
	if( !isPeriodic(axis) && checkOneSidedAtBoundaries ) 
	{
	  if( iab(0,axis) < extendedIndexRange(0,axis) ) 
	  {
	    if( boundaryCondition(0,axis) ) 
	    {
              // Point is a BC point.
	      iab(0,axis) = extendedIndexRange(0,axis);
	      iab(1,axis) = iab(0,axis) + (boundaryDiscretizationWidth(0,axis) - 1);
	    } 
	    else 
	    {
              // Point should have been an interpolation point.
	      return false;
	    } // end if
	  } // end if
	  if (iab(1,axis) > extendedIndexRange(1,axis)) 
	  {
	    if( boundaryCondition(1,axis) ) 
	    {
              // Point is a BC point.
	      iab(1,axis) = extendedIndexRange(1,axis);
	      iab(0,axis) = iab(1,axis) - (boundaryDiscretizationWidth(1,axis) - 1);
	    } 
	    else 
	    {
              // Point should have been an interpolation point.
	      return false;
	    } // endif
	  } // endif
	} // endif
      } 
      else 
      {
	iab(0,axis) = extendedIndexRange(0,axis);
	iab(1,axis) = extendedIndexRange(1,axis);
      } // end if
    } // end for_1

//
//  Check that all points in the discretization stencil are either
//  discretization points or interpolation points.
//
    intArray & maskgd = g.mask();
    GET_LOCAL(int,maskgd,maskg);

    const int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
    const int maskgDim0=maskg.getRawDataSize(0);
    const int maskgDim1=maskg.getRawDataSize(1);
#define mask(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]	
 
  const int j1b=iab(1,axis1), j2b=iab(1,axis2), j3b=iab(1,axis3);
  for( int j3=iab(0,axis3); j3<=j3b; j3++ )
    for( int j2=iab(0,axis2); j2<=j2b; j2++ )
      for( int j1=iab(0,axis1); j1<=j1b; j1++ )
        if( !(mask(j1,j2,j3) & MappedGrid::ISusedPoint) )
	{
	  return false;
	}

  return true;

#undef iab
#undef boundaryCondition
#undef boundaryDiscretizationWidth
#undef extendedIndexRange
#undef discretizationWidth
#undef isPeriodic
#undef mask
}


bool Ogen::
isNeededForDiscretization(MappedGrid& g, const int iv[3] )
//
//  See if the given point on grid "g" is needed for discretisation at any point.
//  A point is needed if there are any discretization points in the discretization stencil.
//
//  **** this function assumes the mask value is set at periodic points 
{
  const int numberOfDimensions=g.numberOfDimensions();
  int ab[3][2];
#define iab(side,axis) ab[(axis)][(side)]
  const int *boundaryConditionp = g.boundaryCondition().Array_Descriptor.Array_View_Pointer1;
#define boundaryCondition(i0,i1) boundaryConditionp[i0+2*(i1)]
  const int *boundaryDiscretizationWidthp = g.boundaryDiscretizationWidth().Array_Descriptor.Array_View_Pointer1;
#define boundaryDiscretizationWidth(i0,i1) boundaryDiscretizationWidthp[i0+2*(i1)]
  const int *eirp = g.extendedIndexRange().Array_Descriptor.Array_View_Pointer1;
#define extendedIndexRange(i0,i1) eirp[i0+2*(i1)]
  const int *dwp = g.discretizationWidth().Array_Descriptor.Array_View_Pointer0;
#define discretizationWidth(i0) dwp[i0]
  const int *isPeriodicp = g.isPeriodic().Array_Descriptor.Array_View_Pointer0;
#define isPeriodic(i0) isPeriodicp[i0]
  //
  //  Determine the range of points to check.
  //
  for( int axis=0; axis<3; axis++ )
  {
    if( axis<numberOfDimensions )
    {
      iab(0,axis) = iv[axis] - discretizationWidth(axis)/2;
      iab(1,axis) = iv[axis] + discretizationWidth(axis)/2;
      if( !isPeriodic(axis) ) 
      {
	if( iv[axis] < extendedIndexRange(0,axis) + boundaryDiscretizationWidth(0,axis) ) 
	{
          //                  See if the boundary discretization needs the given point.
	  if( boundaryCondition(0,axis) )
	  {
	    iab(0,axis) = extendedIndexRange(0,axis);
            //                  See if this interpolated edge needs
	    //                  no other points on the same grid.
	  }
	  else if( iv[axis] < extendedIndexRange(0,axis) + discretizationWidth(axis)/2 &&
		   boundaryCondition(0,axis) == 0 ) 
	    iab(0,axis) = iv[axis];
	} // end if
	if( iv[axis] > extendedIndexRange(1,axis) - boundaryDiscretizationWidth(1,axis) ) 
	{
          //                  See if the boundary discretization needs the given point.
	  if( boundaryCondition(1,axis) ) 
	  {
	    iab(1,axis) = extendedIndexRange(1,axis);
            //                  See if this interpolated edge needs
            //                  no other points on the same grid.
	  }
	  else if( iv[axis] > extendedIndexRange(1,axis) -  discretizationWidth(axis)/2 &&
		   boundaryCondition(1,axis) == 0 ) 
	    iab(1,axis) = iv[axis];
	} // end if
      } // end if
    } 
    else 
    {
      iab(0,axis) = extendedIndexRange(0,axis);
      iab(1,axis) = extendedIndexRange(1,axis);
    } // end if
  } // end for_1
//
//  Check that there are no discretizaton points in the discretization stencil.
//
    intArray & maskgd = g.mask();
    GET_LOCAL(int,maskgd,maskg);

    const int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
    const int maskgDim0=maskg.getRawDataSize(0);
    const int maskgDim1=maskg.getRawDataSize(1);
#define mask(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]	
 
  const int j1b=iab(1,axis1), j2b=iab(1,axis2), j3b=iab(1,axis3);
  for( int j3=iab(0,axis3); j3<=j3b; j3++ )
    for( int j2=iab(0,axis2); j2<=j2b; j2++ )
      for( int j1=iab(0,axis1); j1<=j1b; j1++ )
        if( mask(j1,j2,j3) & MappedGrid::ISdiscretizationPoint ) 
	{
	  return true;
	}

  return false;
}
#undef iab
#undef boundaryCondition
#undef boundaryDiscretizationWidth
#undef extendedIndexRange
#undef discretizationWidth
#undef isPeriodic
#undef mask

bool Ogen::
isOnInterpolationBoundary(MappedGrid& g, const int iv[3], const int & width /* = 1 */ )
//
// /Description:
//  Determine if a point is near the outer boundary of the set of interpolation points.
//  This would be true if there are any unused points in a stencil of half width equal
//  to `width'
//  /width (input) : halfwidth for the stencil.
//
//  **** this function assumes the mask value is set at periodic points 
{
  const int numberOfDimensions=g.numberOfDimensions();
  int ab[3][2];
#define iab(side,axis) ab[(axis)][(side)]
  const int *boundaryConditionp = g.boundaryCondition().Array_Descriptor.Array_View_Pointer1;
#define boundaryCondition(i0,i1) boundaryConditionp[i0+2*(i1)]
  const int *boundaryDiscretizationWidthp = g.boundaryDiscretizationWidth().Array_Descriptor.Array_View_Pointer1;
#define boundaryDiscretizationWidth(i0,i1) boundaryDiscretizationWidthp[i0+2*(i1)]
  const int *eirp = g.extendedIndexRange().Array_Descriptor.Array_View_Pointer1;
#define extendedIndexRange(i0,i1) eirp[i0+2*(i1)]
  const int *dwp = g.discretizationWidth().Array_Descriptor.Array_View_Pointer0;
#define discretizationWidth(i0) dwp[i0]
  const int *isPeriodicp = g.isPeriodic().Array_Descriptor.Array_View_Pointer0;
#define isPeriodic(i0) isPeriodicp[i0]
  //
  //  Determine the range of points to check.
  //
  for( int axis=0; axis<3; axis++ )
  {
    if( axis<numberOfDimensions )
    {
      iab(0,axis) = iv[axis] - width;
      iab(1,axis) = iv[axis] + width;
      if( !isPeriodic(axis) ) 
      {
	if( iab(0,axis) < extendedIndexRange(0,axis) )
	{
          //                  See if the boundary discretization needs the given point.
	  if( boundaryCondition(0,axis) )
	  {
	    iab(0,axis) = extendedIndexRange(0,axis);
            //                  See if this interpolated edge needs
	    //                  no other points on the same grid.
	  }
	  else 
	    return true;

	} // end if
	if( iab(1,axis) > extendedIndexRange(1,axis) )
	{
          //                  See if the boundary discretization needs the given point.
	  if( boundaryCondition(1,axis) ) 
	  {
	    iab(1,axis) = extendedIndexRange(1,axis);
            //                  See if this interpolated edge needs
            //                  no other points on the same grid.
	  }
	  else 
            return true;
	} // end if
      } // end if
    } 
    else 
    {
      iab(0,axis) = extendedIndexRange(0,axis);
      iab(1,axis) = extendedIndexRange(1,axis);
    } // end if
  } // end for_1
//
//  Check that there are no unused points in the discretization stencil.
//
    intArray & maskg = g.mask();
    const int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
    const int maskgDim0=maskg.getRawDataSize(0);
    const int maskgDim1=maskg.getRawDataSize(1);
#define mask(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]	
 
  const int j1b=iab(1,axis1), j2b=iab(1,axis2), j3b=iab(1,axis3);
  for( int j3=iab(0,axis3); j3<=j3b; j3++ )
    for( int j2=iab(0,axis2); j2<=j2b; j2++ )
      for( int j1=iab(0,axis1); j1<=j1b; j1++ )
        if( mask(j1,j2,j3)==0 ) 
	{
	  return true;
	}

  return false;
}
#undef iab
#undef boundaryCondition
#undef boundaryDiscretizationWidth
#undef extendedIndexRange
#undef discretizationWidth
#undef isPeriodic
#undef mask

bool Ogen::
isNeededForMultigridRestriction(CompositeGrid& c,
				const int & grid,
				const int & l,
				const int iv[3]) 
// ========================================================================================
// /Description:
//  Determine whether the given point on grid k at level l is needed for averaging at the next coarser level.
//  A discretization point on a coarser level needs to have a stencil of points on the finer level
//  from which to interpolate.
// ========================================================================================
{
  MappedGrid &gl = c.multigridLevel[l][grid], &gp = c.multigridLevel[l+1][grid];
  assert(c.multigridLevel[ l ].componentGridNumber(grid) == grid);
  assert(c.multigridLevel[l+1].componentGridNumber(grid) == grid);

  int ab[3][2];
#define iab(side,axis) ab[(axis)][(side)]
  const int lp1=l+1;
  const IntegerArray & coarseningRatio  = c.multigridCoarseningRatio(Range(0,2),grid,lp1);
  const IntegerArray & restrictionWidth = c.multigridRestrictionWidth(Range(0,2),grid,lp1);
  
  //
  //  Determine the stencil of points to check.
  //
  for( int axis=0; axis<3; axis++ )
  {
    if (axis < c.numberOfDimensions()) 
    {
      if (gl.isCellCentered(axis)) 
      {
        //  Must have c.multigridCoarseningRatio(axis,k,l+1) and
        //  c.multigridRestrictionWidth(axis,k,l+1) both odd or both even.
	iab(0,axis)=gp.indexRange(0,axis)+(iv[axis] - gl.indexRange(0,axis) + coarseningRatio(axis) +
			    (coarseningRatio(axis)-restrictionWidth(axis))/2)/coarseningRatio(axis) - 1;
	iab(1,axis)=gp.indexRange(0,axis)+(iv[axis] - gl.indexRange(0,axis) + coarseningRatio(axis) +
			   (restrictionWidth(axis)-coarseningRatio(axis))/2)/coarseningRatio(axis) - 1;
      } 
      else 
      {
        // Must have c.multigridRestrictionWidth(axis,k,l+1) odd.
	iab(0,axis)=gp.indexRange(0,axis)+(iv[axis] - gl.indexRange(0,axis) + coarseningRatio(axis) -
				      (restrictionWidth(axis)+1)/2)/coarseningRatio(axis);
	iab(1,axis)=gp.indexRange(0,axis)+(iv[axis]-gl.indexRange(0,axis)
                    +(restrictionWidth(axis)-1)/2)/coarseningRatio(axis);
      } 
      if (gp.boundaryCondition(0,axis) >= 0) 
	iab(0,axis) = max0(iab(0,axis), gp.extendedIndexRange(0,axis));
      if (gp.boundaryCondition(1,axis) >= 0) 
	iab(1,axis) =  min0(iab(1,axis), gp.extendedIndexRange(1,axis));
    } 
    else
    {
      iab(0,axis) = gp.extendedIndexRange(0,axis);
      iab(1,axis) = gp.extendedIndexRange(1,axis);
    }
  } 
  //
  //  Check that none of the points having the given point in their transfer
  //  stencils are discretization points. 
  //
  intArray & mask = gp.mask();
  if( TRUE || c.interpolationIsAllImplicit() )  // ****** why not do this all the time???
  {
    for( int j3=iab(0,axis3); j3<=iab(1,axis3); j3++ )
      for( int j2=iab(0,axis2); j2<=iab(1,axis2); j2++ )
	for( int j1=iab(0,axis1); j1<=iab(1,axis1); j1++ )
	  if( mask(j1,j2,j3) & MappedGrid::ISdiscretizationPoint ) 
	  {
	    return TRUE;
	  }

  } 
  else 
  {
    //
    //      Some interpolation points might need to become discretization points.
    //
    for( int j3=iab(0,axis3); j3<=iab(1,axis3); j3++ )
      for( int j2=iab(0,axis2); j2<=iab(1,axis2); j2++ )
	for( int j1=iab(0,axis1); j1<=iab(1,axis1); j1++ )
	  if( mask(j1,j2,j3) & MappedGrid::ISusedPoint ) 
	  {
	    return TRUE;
	  }

  } 

  return LogicalFalse;
#undef iab
}



int Ogen::
classifyRedundantPoints( CompositeGrid& cg, const int & grid, const int & level, CompositeGrid & cg0 )
// ====================================================================================================
// /Description:
//  Mark as unused any interpolated point that is not needed
//  for either discretisation or interpolation at the same level
//  or for interpolation at the next higher level.
//
// /level (input): multigrid level
// /cg0 (input) : "base" CompositeGrid with all multigrid levels
// ====================================================================================================
{
  MappedGrid & g = cg[grid];
  intArray & maskgd = g.mask();

  GET_LOCAL(int,maskgd,maskg);

  int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
  const int maskgDim0=maskg.getRawDataSize(0);
  const int maskgDim1=maskg.getRawDataSize(1);
#define MASK(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getIndex(extendedGridIndexRange(g),I1,I2,I3);

  if( debug & 8 )
    displayMask(maskgd,sPrintF(buff,"classifyRedundantPoints: mask for grid=%i at start",grid),logFile);
  

  if( true ) // **** wdh 001007
  {
    for( int axis=0; axis<g.numberOfDimensions(); axis++ )
    {
      // include ghost points on mixed boundaries.
      if( g.boundaryFlag(Start,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	Iv[axis]=Range(g.gridIndexRange(Start,axis)-1,Iv[axis].getBound());
      if( g.boundaryFlag(End,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	Iv[axis]=Range(Iv[axis].getBase(),g.gridIndexRange(End,axis)+1);
    }
  }
  

  int iv[3];
  int & i1=iv[0], & i2=iv[1], & i3=iv[2];
  bool ok=ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);
  if( ok )
  {
    if( cg0.numberOfMultigridLevels()==1 || level == cg0.numberOfMultigridLevels()-1 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	int & m = MASK(i1,i2,i3);
	// some compilers don't like the next statement since MappedGrid::ISinterpolationPoint is the sign bit.
	// if( (m & MappedGrid::ISinterpolationPoint) && !(m & ISneededPoint)) 
	if( (m <0) && !(m & ISneededPoint)) 
	{
	  // The point is an interpolated point but is not needed
	  // for interpolation of other points at this level.
	  // If it is not used for discretisation at some point, ...
	  // ** don't remove mixed boundary interpolation points
	  // if( !isNeededForDiscretization(g, iv) && !(m & MappedGrid::ISinteriorBoundaryPoint) )
	  if( !isNeededForDiscretization(g, iv) )
	  {
	    m = 0;   // mark as unused
	  }
	}
      }
    }
    else
    {
      // In the multigrid case we also need to check that a possible redundant interpolation
      // point is needed for interpolation (restriction) to the coarser level.
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	int & m = MASK(i1,i2,i3);
	// some compilers don't like the next statement since MappedGrid::ISinterpolationPoint is the sign bit.
	// if( (m & MappedGrid::ISinterpolationPoint) && !(m & ISneededPoint)) 
	if( (m <0) && !(m & ISneededPoint)) 
	{
	  // The point is an interpolated point but is not needed
	  // for interpolation of other points at this level.
	  // If it is not used for discretisation at some point, ...
	  if (!isNeededForDiscretization(g, iv) && !isNeededForMultigridRestriction(cg0,grid,level,iv) )
	  {
	    m = 0;   // mark as unused
	  }
	}
      }
    }
  }
  
  // *wdh* 000322 we need to do the following to get periodic boundaries correct -- cf. two-circle.cmd

  g.mask().periodicUpdate();
  maskgd.updateGhostBoundaries();  // is this correct here ?

  //  Mark as interior any interpolated point that could be used
  //  as an interior point.  Count the remaining interpolated points.
  cg.numberOfInterpolationPoints(grid)=0;
  if( ok )
  {
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      int & m = MASK(i1,i2,i3);
      if (m & MappedGrid::ISinterpolationPoint) 
      {
	//          This is an interpolated point.
	//          If it is not a boundary point in the interior of another grid...
	if (!(m & MappedGrid::ISinteriorBoundaryPoint)
	    //            and it can be used for discretisation...
	    && canDiscretize(g, iv) )
	{
	  //  then mark it as interior.
	  m = MappedGrid::ISdiscretizationPoint;
	}
	else
	{
	  // Otherwise, count it as an interpolation point.
	  cg.numberOfInterpolationPoints(grid)++;
	}
      }
    }
  }
  
  return 0;
}
#undef MASK

int Ogen::
unmarkBoundaryInterpolationPoints( CompositeGrid & cg, const int & grid )
// =====================================================================================
// /Description:
// First mark interp. points on physical boundaries that interpolate from other
// boundaries and prefer to be discretized
// We need this step as we found all possible interp points on boundaries. We do not
// want these un-needed interpolation points to be used when we mark points as needed.
// 
// =====================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  MappedGrid & c = cg[grid];
  Mapping & map = c.mapping().getMapping();
  realArray & rI = cg.inverseCoordinates[grid];
  intArray & maskgd = c.mask();
  intArray & inverseGridgd = cg.inverseGrid[grid];

  GET_LOCAL(int,maskgd,maskg);
  GET_LOCAL(int,inverseGridgd,inverseGridg);

  int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
  const int maskgDim0=maskg.getRawDataSize(0);
  const int maskgDim1=maskg.getRawDataSize(1);
#define MASK(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]
  int * inverseGridgp = inverseGridg.Array_Descriptor.Array_View_Pointer2;
  const int inverseGridgDim0=inverseGridg.getRawDataSize(0);
  const int inverseGridgDim1=inverseGridg.getRawDataSize(1);
#define INVERSEGRID(i0,i1,i2) inverseGridgp[i0+inverseGridgDim0*(i1+inverseGridgDim1*(i2))]

  int side,axis;
  int iv[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  for( axis=0; axis<cg.numberOfDimensions(); axis++ )
  {
    for( side=Start; side<=End; side++ )
    {
      if( c.boundaryCondition()(side,axis) > 0 &&
          map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity )
      {
        if( c.isAllVertexCentered() )
	{
    	  // getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);  // %%wdh
    	  getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);  // 980612: do not include periodic points

	  if( c.boundaryFlag(side,axis)==MappedGrid::physicalBoundary )
	    Iv[axis]=c.gridIndexRange(side,axis);
	}
	else
	{
    	  getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);  // %%wdh
/* ----- **wdh 980205
	  for( int dir=0; dir<c.numberOfDimensions(); dir++ )
	  { // include periodic image -- is this needed??
	    if( dir!=axis && c.isPeriodic()(dir) )
	      Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound()+1);
	  }
   ---- */
	}
	  
        bool ok= ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);
 	if( ok )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if(   MASK(i1,i2,i3) & MappedGrid::ISinterpolationPoint && 
		  !( MASK(i1,i2,i3) & MappedGrid::ISinteriorBoundaryPoint)  )   // *wdh* 980130
	    {
	      int & m=MASK(i1,i2,i3);
	      int grid2=INVERSEGRID(i1,i2,i3);
	      assert( grid2>=0 && grid2<numberOfBaseGrids);
	      if( grid2<grid && canDiscretize(c,iv) )// *************** fix this priority ****
	      {
		// This interpolation point on the boundary can be prefers to be discretized.

		m=MappedGrid::ISdiscretizationPoint;
		if( info & 4 )
		{
		  fprintf(plogFile,"Unmarking point (%i,%i,%i) on grid %s to be a discretization point\n",
			 i1,i2,i3,(const char *)c.mapping().getName(Mapping::mappingName));
		}
	      }
	    }
	  }
	} // end if ok

      }
    }
  }
  return 0;
}
#undef MASK
#undef INVERSEGRID

int Ogen::
unmarkInterpolationPoints( CompositeGrid & cg, const bool & unMarkAll /* = FALSE */ )
// =============================================================================================================
// /Description:
//    Un-mark interpolation points that interpolate from lower priority grids (if  {\tt unMarkAll=FALSE})
//    but can be discretization points instead. These points were marked by the new hole cutting algorithm.
// /unMarkAll (input) : If TRUE, unmark all interpolation points that can be used as discretization points.
//     This option is used when maximizing the overlap.
// =============================================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    
  int grid;
  for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
  {
    MappedGrid & c = cg[grid];
    intArray & maskgd = c.mask();
    intArray & inverseGridd = cg.inverseGrid[grid];

    GET_LOCAL(int,maskgd,maskg);
    GET_LOCAL(int,inverseGridd,inverseGridg);

    int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
    const int maskgDim0=maskg.getRawDataSize(0);
    const int maskgDim1=maskg.getRawDataSize(1);
#define MASK(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]
    int * inverseGridgp = inverseGridg.Array_Descriptor.Array_View_Pointer2;
    const int inverseGridgDim0=inverseGridg.getRawDataSize(0);
    const int inverseGridgDim1=inverseGridg.getRawDataSize(1);
#define INVERSEGRID(i0,i1,i2) inverseGridgp[i0+inverseGridgDim0*(i1+inverseGridgDim1*(i2))]

    // check points from interior points plus periodic boundaries
    getIndex(c.extendedIndexRange(),I1,I2,I3,-1);  // by default exclude boundaries
    int axis;
    for(axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      // include boundaries on periodic sides AND at polarSingularities
      if( c.isPeriodic(axis) )
	Iv[axis]=Range(c.gridIndexRange(Start,axis),c.gridIndexRange(End,axis));   // note: Iv[.] == I1,I2,I3
      else if( c.isAllVertexCentered() )
      {
	Mapping & map = c.mapping().getMapping();
	if( map.getTypeOfCoordinateSingularity(Start,axis)==Mapping::polarSingularity )
	  Iv[axis]=Range(c.gridIndexRange(Start,axis),Iv[axis].getBound());
	if( map.getTypeOfCoordinateSingularity(End  ,axis)==Mapping::polarSingularity )
	  Iv[axis]=Range(Iv[axis].getBase(),c.gridIndexRange(End,axis));
      }
    }

    bool ok=ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);
    if( !ok ) continue;
    
    if( !unMarkAll )
    {
      if( grid>0 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  // For points that interpolate from lower priority grids, try to turn into discretization points.
	  if( (MASK(i1,i2,i3)==MappedGrid::ISinterpolationPoint) && (INVERSEGRID(i1,i2,i3)<grid) )
	  {                               
	    if( canDiscretize(c,iv) )
	    {
	      MASK(i1,i2,i3)=MappedGrid::ISdiscretizationPoint;
	      INVERSEGRID(i1,i2,i3)=-1;
	    }
	  }
	}
      }
    }
    else if( unMarkAll )
    {
      // ** unmark all interpolation points ***
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( MASK(i1,i2,i3) & MappedGrid::ISinterpolationPoint )
	{                               
	  if( canDiscretize(c,iv) )
	  {
	    MASK(i1,i2,i3)=MappedGrid::ISdiscretizationPoint;
	    INVERSEGRID(i1,i2,i3)=-1;
	  }
	}
      }
    }
    
  }
  
  return 0;
}
#undef MASK
#undef INVERSEGRID


int Ogen::
markPointsNeededForInterpolation( CompositeGrid & cg, const int & grid, const int & lowerOrUpper /* =-1 */ )
// =============================================================================================================
// /Description:
//    Mark points that are needed for interpolation. For each interpolation point that has discretization
//  points in it's stencil (or that has already been marked as needed) mark its donor points.
//
// /lowerOrUpper (input) : if -1 check grids of lower priority, if +1 check grids of higher priority
//
// =============================================================================================================
{
  #ifdef USE_PPP
    return markPointsNeededForInterpolationNew(cg,grid,lowerOrUpper);
  #endif

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  const int numberOfDimensions = cg.numberOfDimensions();
  MappedGrid & c = cg[grid];
  intArray & maskgd = c.mask();
  intArray & inverseGridd = cg.inverseGrid[grid];
  realArray & rId = cg.inverseCoordinates[grid];
  RealArray & interpolationOverlap = cg.interpolationOverlap;

  GET_LOCAL(int,maskgd,maskg);
  GET_LOCAL(int,inverseGridd,inverseGridg);
  GET_LOCAL(real,rId,rI);
    
  Index I1,I2,I3;
  getIndex(c.extendedIndexRange(),I1,I2,I3); 

  bool ok=ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);
  if( !ok ) return 0;

  int axis;
  int iv[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];
  int l =0;  // multigrid level
  real rv[3]={0.,0.,0};
  
  int abi[3][2], abj[3][2];
#define iab(side,axis) abi[(axis)][(side)]
#define jab(side,axis) abj[(axis)][(side)]
  int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
  const int maskgDim0=maskg.getRawDataSize(0);
  const int maskgDim1=maskg.getRawDataSize(1);
#define MASK(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]
  int * inverseGridgp = inverseGridg.Array_Descriptor.Array_View_Pointer2;
  const int inverseGridgDim0=inverseGridg.getRawDataSize(0);
  const int inverseGridgDim1=inverseGridg.getRawDataSize(1);
#define INVERSEGRID(i0,i1,i2) inverseGridgp[i0+inverseGridgDim0*(i1+inverseGridgDim1*(i2))]


  // *********** NOTE: this is the serial version ***************

  FOR_3D(i1,i2,i3,I1,I2,I3)
  {
    if( MASK(i1,i2,i3) & MappedGrid::ISinterpolationPoint  ) 
    {
      const int grid2=INVERSEGRID(i1,i2,i3);
      if( grid2==grid && allowHangingInterpolation )
	continue;
	  
      assert( grid2>=0 && grid2<numberOfBaseGrids ); // && grid2!=grid );

      if( (lowerOrUpper==-1 && grid2<=grid) || (lowerOrUpper==+1 && grid2>=grid) ) // ******* move this up ******
      {
	int m=MASK(i1,i2,i3);
	if( m & ISneededPoint  ||                  // a needed point
	    isNeededForDiscretization(c,iv) )      // or it is needed for discretisation...
	{
	  // mark the interpolee points as needed
	  MappedGrid & g2 = cg[grid2];
	  intArray & mask2 = g2.mask();

	  real ov = interpolationOverlap(axis1,grid,grid2,l);  // *wdh 00015
          const bool explicitInterp = !cg.interpolationIsImplicit(grid,grid2,l);

	  if( m & MappedGrid::USESbackupRules )
	  {
	    const int backup=backupValues[grid](i1,i2,i3);
	    if( backup<0 )
	      ov-=1.;  // implicit interpolation
	    else
	      ov-=.5;  // lower order interpolation ** may not be right if order reduced by more than 1??
	  }
	      
	  real oneSidedShift = 2.*ov+1.;
	  if( explicitInterp )
	  { // for explicit interp, ov is increased by 1 in both directions -- for one sided we only
            // need to increase by 1 total:
            oneSidedShift = 2.*ov; // *wdh* 040718 I think this is correct (cf cicbug.cmd)
	  }

	  for( axis=0; axis<3; axis++ )
	  {
	    if( axis<cg.numberOfDimensions() )
	    {
	      real r = rI(i1,i2,i3,axis)/g2.gridSpacing(axis) + g2.indexRange(0,axis);
              rv[axis]=r;
	      iab(0,axis)=Integer(floor(r-ov - (g2.isCellCentered(axis) ? .5 : 0. )));
	      iab(1,axis)=Integer(floor(r+ov + (g2.isCellCentered(axis) ? .5 : 1. )));
	      if( !g2.isPeriodic(axis) )
	      {
		if( iab(0,axis) < g2.extendedIndexRange(0,axis) )
		{
		  if( g2.boundaryCondition(Start,axis)>0 ) 
		  {
		    // Point is close to a BC side. One-sided interpolation used.
		    iab(0,axis) = g2.extendedIndexRange(0,axis);
		    iab(1,axis) = min(g2.extendedIndexRange(1,axis),   // *wdh* added min 040327
                                      Integer(floor(iab(0,axis) + oneSidedShift )));
		  } // end if
		  else
		  {
		    printf("Ogen:MPNeeded:ERROR:grid=%i, donor=%i, ip=(i1,i2,i3)=(%i,%i,%i), g2.bc=%i"
			   "  donor location is invalid. (will shift to boundary)\n"
			   "  ov=%8.2e (default=%8.2e), rI(%i)=%8.2e, rI/dr=%8.2e -> iab=%i < g2.extendedIndexRange=%i\n",
			   grid,grid2,i1,i2,i3,g2.boundaryCondition(Start,axis),
			   ov, interpolationOverlap(axis1,grid,grid2,l),
                           axis,rI(i1,i2,i3,axis),r,iab(0,axis),g2.extendedIndexRange(0,axis));
		    iab(0,axis) = g2.extendedIndexRange(0,axis);
		    iab(1,axis) =  min(g2.extendedIndexRange(1,axis),   // *wdh* added min 040327
                                       Integer(floor(iab(0,axis) + oneSidedShift )));
		  }
		}
		if( iab(1,axis) > g2.extendedIndexRange(1,axis) )
		{
		  if( g2.boundaryCondition(End,axis)>0 ) 
		  {
		    // Point is close to a BC side. One-sided interpolation used.
		    iab(1,axis) = g2.extendedIndexRange(1,axis);
		    iab(0,axis) = max(g2.extendedIndexRange(0,axis),   // *wdh* added max 040327
                                       Integer(floor(iab(1,axis) - oneSidedShift )));
		  } // end if
		  else
		  {
		    printf("Ogen:MPNeeded:ERROR:grid=%i, donor=%i, ip=(i1,i2,i3)=(%i,%i,%i), g2.bc=%i"
			   "  donor location is invalid. (will shift to boundary)\n"
			   "  ov=%8.2e (default=%8.2e), rI(%i)=%8.2e, rI/dr=%8.2e -> iab=%i > g2.extendedIndexRange=%i\n",
			   grid,grid2,i1,i2,i3,g2.boundaryCondition(1,axis),
			   ov, interpolationOverlap(axis1,grid,grid2,l),
                           axis,rI(i1,i2,i3,axis),r,iab(1,axis),g2.extendedIndexRange(1,axis));
		    iab(1,axis) = g2.extendedIndexRange(1,axis);
		    iab(0,axis) = max(g2.extendedIndexRange(0,axis),   // *wdh* added max 040327
                                      Integer(floor(iab(1,axis) - oneSidedShift )));
		  }
		}
		    
		jab(0,axis)=iab(0,axis);
		jab(1,axis)=iab(1,axis);

	      } // end if
	      else
	      {
		jab(0,axis)=max(iab(0,axis),g2.extendedIndexRange(0,axis));
		jab(1,axis)=min(iab(1,axis),g2.extendedIndexRange(1,axis));
	      }
	    } 
	    else 
	    {
	      iab(0,axis) = jab(0,axis)=g2.extendedIndexRange(0,axis);
	      iab(1,axis) = jab(1,axis)=g2.extendedIndexRange(1,axis);
	    } // end if, end for_1
	  }

	  if( debug & 8 )
	  {
	    fprintf(logFile,"Ogen:markPtsNeeded:grid=%i, donor=%i, (i1,i2,i3)=(%i,%i,%i), "
		    "r=[%6.3f,%6.3f,%6.3f], iab=[%i,%i][%i,%i][%i,%i]\n",
		    grid,grid2,i1,i2,i3,rv[0],rv[1],rv[2],iab(0,0),iab(1,0),iab(0,1),iab(1,1),iab(0,2),iab(1,2));
	  }

	  // Mark interpolee points on grid2 as needed for interpolation.
	  // note that iab could go outside the extendedIndexRange on periodic edges so we need jab.

//            if( jab(0,axis1)<g2.extendedIndexRange(0,axis1) )  // for debugging
//  	  {
//  	    printf("markPointsNeededForInterpolation:ERROR: jab(0,0)=%i but g2.extendedIndexRange(0,0)=%i\n"
//                     " g2.isPeriodic(0)=%i g2.boundaryCondition(Start,0)=%i \n",
//                     jab(0,0),g2.extendedIndexRange(0,0),g2.isPeriodic(0),g2.boundaryCondition(Start,0));
//  	  }

	  Range J1(jab(0,axis1),jab(1,axis1)),J2(jab(0,axis2),jab(1,axis2)),J3(jab(0,axis3),jab(1,axis3));
	  mask2(J1,J2,J3) |= ISneededPoint;

	  // we need to mark the periodic images that lie inside the grid
	  if( g2.isPeriodic(axis1) || g2.isPeriodic(axis2) || (cg.numberOfDimensions()>2 && g2.isPeriodic(axis3)) )
	  {
	    bool needToMark=FALSE;
	    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	    {
	      if( g2.isPeriodic(axis) )
	      {
		if( iab(0,axis)<g2.indexRange(Start,axis) )
		{
		  needToMark=TRUE;
		  const int ndr =g2.gridIndexRange(End,axis)-g2.gridIndexRange(Start,axis); 
		  iab(0,axis)+=ndr;
		  iab(1,axis)=min(iab(1,axis)+ndr,g2.dimension(End,axis));
		}
		else if( iab(1,axis)>g2.indexRange(End,axis) )
		{
		  needToMark=TRUE;
		  const int ndr =g2.gridIndexRange(End,axis)-g2.gridIndexRange(Start,axis); 
		  iab(0,axis)=max(iab(0,axis)-ndr,g2.dimension(Start,axis));
		  iab(1,axis)-=ndr;
		}
	      }
	    }
	    if( needToMark )
	    {
	      Range J1(iab(0,axis1),iab(1,axis1)),J2(iab(0,axis2),iab(1,axis2)),J3(iab(0,axis3),iab(1,axis3));
	      mask2(J1,J2,J3) |= ISneededPoint;
/* ----
   for( int j3=iab(0,axis3); j3<=iab(1,axis3); j3++ )
   for( int j2=iab(0,axis2); j2<=iab(1,axis2); j2++ )
   for( int j1=iab(0,axis1); j1<=iab(1,axis1); j1++ )
   {
   mask2(j1,j2,j3) |= ISneededPoint;
   // printf("point is needed: grid=%i, (i1,i2,i3)=(%i,%i,%i) \n",grid2,j1,j2,j3);
   }
   ---- */
	    }
	  }
	}
      }
    }
  }
  return 0;
}

int Ogen::
markPointsReallyNeededForInterpolation( CompositeGrid & cg )
// =============================================================================================================
// /Description:
//    Mark points that are needed for interpolation. For each interpolation point that has discretization
//  points in it's stencil (or that has already been marked as needed) mark its interpolee points.
//
// /lowerOrUpper (input) : if -1 check grids of lower priority, if +1 check grids of higher priority
//
// =============================================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  for( int grid=0; grid<numberOfBaseGrids; grid++ )
  {

    MappedGrid & c = cg[grid];
    intArray & mask = c.mask();
    intArray & inverseGrid = cg.inverseGrid[grid];
    realArray & rI = cg.inverseCoordinates[grid];
    RealArray & interpolationOverlap = cg.interpolationOverlap;

    Index I1,I2,I3;
    getIndex(c.extendedIndexRange(),I1,I2,I3); 
    int axis;
    int iv[3];
    int & i1 = iv[0];
    int & i2 = iv[1];
    int & i3 = iv[2];
    int l =0;  // multigrid level
  
    int abi[3][2], abj[3][2];
#define iab(side,axis) abi[(axis)][(side)]
#define jab(side,axis) abj[(axis)][(side)]

    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  if( mask(i1,i2,i3) & MappedGrid::ISinterpolationPoint  ) 
	  {
	    const int grid2=inverseGrid(i1,i2,i3);
	    assert( grid2>=0 && grid2<numberOfBaseGrids && grid2!=grid );

	    if( grid2!=grid )
	    {
	      int m=mask(i1,i2,i3);
	      if( isOnInterpolationBoundary(c,iv)  ) // or it is needed for discretisation...
	      {
                printf("markReallyNeeded: point (%i,%i) on grid %i is really needed\n",i1,i2,grid);
		
		// mark the interpolee points as needed
		MappedGrid & g2 = cg[grid2];
		intArray & mask2 = g2.mask();

		real ov = interpolationOverlap(axis1,grid,grid2,l);  // *wdh 00015
		if( m & MappedGrid::USESbackupRules )
		{
		  const int backup=backupValues[grid](i1,i2,i3);
		  if( backup<0 )
		    ov-=1.;  // implicit interpolation
		  else
		    ov-=.5;
		}

		for( axis=0; axis<3; axis++ )
		{
		  if( axis<cg.numberOfDimensions() )
		  {
		    real r = rI(i1,i2,i3,axis)/g2.gridSpacing(axis) + g2.indexRange(0,axis);
		    iab(0,axis)=Integer(floor(r-ov - (g2.isCellCentered(axis) ? .5 : 0. )));
		    iab(1,axis)=Integer(floor(r+ov + (g2.isCellCentered(axis) ? .5 : 1. )));
		    if( !g2.isPeriodic(axis) )
		    {
		      if( (iab(0,axis) < g2.extendedIndexRange(0,axis)) && (g2.boundaryCondition(Start,axis)>0) ) 
		      {
			// Point is close to a BC side. One-sided interpolation used.
			iab(0,axis) = g2.extendedIndexRange(0,axis);
			iab(1,axis) = Integer(floor(iab(0,axis) + ((Real)2. * ov + (Real)1.)));
		      } // end if
		      if( (iab(1,axis) > g2.extendedIndexRange(1,axis)) && (g2.boundaryCondition(End,axis)>0) ) 
		      {
			// Point is close to a BC side. One-sided interpolation used.
			iab(1,axis) = g2.extendedIndexRange(1,axis);
			iab(0,axis) = Integer(floor(iab(1,axis) - ((Real)2. * ov + (Real)1.)));
		      } // end if
		      jab(0,axis)=iab(0,axis);
		      jab(1,axis)=iab(1,axis);

		    } // end if
		    else
		    {
		      jab(0,axis)=max(iab(0,axis),g2.extendedIndexRange(0,axis));
		      jab(1,axis)=min(iab(1,axis),g2.extendedIndexRange(1,axis));
		    }
		  } 
		  else 
		  {
		    iab(0,axis) = jab(0,axis)=g2.extendedIndexRange(0,axis);
		    iab(1,axis) = jab(1,axis)=g2.extendedIndexRange(1,axis);
		  } // end if, end for_1
		}
		// Mark interpolee points on grid2 as needed for interpolation.
		// note that iab could go outside the extendedIndexRange on periodic edges so we need jab.
		Range J1(jab(0,axis1),jab(1,axis1)),J2(jab(0,axis2),jab(1,axis2)),J3(jab(0,axis3),jab(1,axis3));
		mask2(J1,J2,J3) |= ISneededPoint;

		// we need to mark the periodic images that lie inside the grid
		if( g2.isPeriodic(axis1) || g2.isPeriodic(axis2) || (cg.numberOfDimensions()>2 && g2.isPeriodic(axis3)) )
		{
		  bool needToMark=FALSE;
		  for( axis=0; axis<cg.numberOfDimensions(); axis++ )
		  {
		    if( g2.isPeriodic(axis) )
		    {
		      if( iab(0,axis)<g2.indexRange(Start,axis) )
		      {
			needToMark=TRUE;
			const int ndr =g2.gridIndexRange(End,axis)-g2.gridIndexRange(Start,axis); 
			iab(0,axis)+=ndr;
			iab(1,axis)=min(iab(1,axis)+ndr,g2.dimension(End,axis));
		      }
		      else if( iab(1,axis)>g2.indexRange(End,axis) )
		      {
			needToMark=TRUE;
			const int ndr =g2.gridIndexRange(End,axis)-g2.gridIndexRange(Start,axis); 
			iab(0,axis)=max(iab(0,axis)-ndr,g2.dimension(Start,axis));
			iab(1,axis)-=ndr;
		      }
		    }
		  }
		  if( needToMark )
		  {
		    Range J1(iab(0,axis1),iab(1,axis1)),J2(iab(0,axis2),iab(1,axis2)),J3(iab(0,axis3),iab(1,axis3));
		    mask2(J1,J2,J3) |= ISneededPoint;
/* ----
		    for( int j3=iab(0,axis3); j3<=iab(1,axis3); j3++ )
		      for( int j2=iab(0,axis2); j2<=iab(1,axis2); j2++ )
			for( int j1=iab(0,axis1); j1<=iab(1,axis1); j1++ )
			{
			  mask2(j1,j2,j3) |= ISneededPoint;
			  // printf("point is needed: grid=%i, (i1,i2,i3)=(%i,%i,%i) \n",grid2,j1,j2,j3);
			}
----- */
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  return 0;
}


int Ogen::
updateCanInterpolate( CompositeGrid & cg, CompositeGrid & cg0, RealArray & removedPointBound )
// =============================================================================================================
// /Description:
//    After points have been removed to improve the quality of the interpolation we need to recheck points
//  to see if they can still interpolate
//
// =============================================================================================================
{
#ifdef USE_PPP
  printF("updateCanInterpolate::Not implemented yet for parallel. Do nothing...\n");
#else

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  LogicalDistributedArray interpolates(1), useBackupRules(1);
  useBackupRules=FALSE;
  // 
  // If checkForOneSided=TRUE then canInterpolate will not allow a one-sided interpolation
  // stencil to use ANY interiorBoundaryPoint's -- this is actually too strict. We really
  // only want to disallow interpolation that has less than the minimum overlap distance
  //
  checkForOneSided=FALSE;  // check for one side interp from non-conforming grids *******
//  bool ok=TRUE;
//  int numberOfInvalidPoints=0;
//  RealArray invalidPoint;

  realArray rr(1,3),x(1,3);
  
  rr=-1.;
  Range Rx(0,cg.numberOfDimensions()-1);
  
  for( int grid=numberOfBaseGrids-1; grid>=0; grid-- )
  {

    MappedGrid & c = cg[grid];
    intArray & mask = c.mask();
    const realArray & vertex = c.vertex();
    intArray & inverseGrid = cg.inverseGrid[grid];
    realArray & rI = cg.inverseCoordinates[grid];
    Mapping & map = cg[grid].mapping().getMapping();

    Index I1,I2,I3;
    getIndex(c.extendedIndexRange(),I1,I2,I3); 
    int axis;
    int iv[3];
    int & i1 = iv[0];
    int & i2 = iv[1];
    int & i3 = iv[2];
    // int l =0;  // multigrid level

    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  if( mask(i1,i2,i3) & MappedGrid::ISinterpolationPoint  ) 
	  {
	    int grid2=inverseGrid(i1,i2,i3);
	    assert( grid2>=0 && grid2<numberOfBaseGrids ); //  && grid2!=grid );

            bool checkThisPoint=TRUE;
	    for( axis=0; axis<c.numberOfDimensions(); axis++ )
	    {
	      rr(0,axis)=rI(i1,i2,i3,axis);
              if( rr(0,axis) < removedPointBound(Start,axis,grid2) || 
                  rr(0,axis) > removedPointBound(End  ,axis,grid2) )
	      {
		checkThisPoint=FALSE;
		break;
	      }
	    }
	    if( !checkThisPoint )
              continue;

            interpolates(0)=TRUE;
	    cg0.rcData->canInterpolate(cg.gridNumber(grid),cg.gridNumber(grid2), rr, interpolates, useBackupRules, 
				       checkForOneSided );

            const int grid2Old=grid2;
	    if( !interpolates(0) )
	    {
              // try to interpolate from another grid
	      intArray ia(1,3);
	      ia(0,0)=iv[0]; ia(0,1)=iv[1]; ia(0,2)=iv[2];
	      // lastChanceInterpolation(cg,cg0,grid,iv,ok,interpolates,numberOfInvalidPoints,invalidPoint,FALSE);
	      for( int g=1; g<numberOfBaseGrids; g++ )
	      {
		grid2 = grid2Old-g;  // only check grids with a lower priority ????
		if( grid2==grid )
                  continue;
		if( grid2<0 )
                  break;
	  
		Mapping & map2 = cg[grid2].mapping().getMapping();
		if( debug & 3 )
		  fprintf(plogFile,"point (%i,%i,%i) on grid %i, try to interpolate from grid2=%i \n",
                          i1,i2,i3,grid,grid2);

		if( map.intersects(map2,-1,-1,-1,-1,.1) )
		{

		  for( axis=0; axis<c.numberOfDimensions(); axis++ )
		    x(0,axis)=vertex(i1,i2,i3,axis);
		  if( useBoundaryAdjustment )
		    adjustBoundary(cg,grid,grid2,ia,x(0,Rx)); 

		  map2.inverseMap(x(0,Rx),rr);

                  // If the r values are outside [0,1] but within [-eps1,1+eps2] where eps1 and eps2 
                  // are determined by the shared boundary tolerance then project r back into [0,1]
		  const RealArray & shareTol = cg[grid2].sharedBoundaryTolerance();
		  for( axis=0; axis<c.numberOfDimensions(); axis++ )
		  {
		    if( rr(0,axis)<0. && rr(0,axis)>= -shareTol(Start,axis)*cg[grid2].gridSpacing()(axis) )
		    {
		      rr(0,axis)=0.;
		    }
		    else if( rr(0,axis)>1. && rr(0,axis)<= 1.+shareTol(End,axis)*cg[grid2].gridSpacing()(axis) )
		    {
		      rr(0,axis)=1.; 
		    }
		  }
		  interpolates(0)=TRUE;
		  cg0.rcData->canInterpolate(cg.gridNumber(grid),cg.gridNumber(grid2), rr, interpolates, 
                                             useBackupRules, checkForOneSided );

                  if( interpolates(0) )
		  {
		    inverseGrid(i1,i2,i3)=grid2;
		    for( axis=0; axis<c.numberOfDimensions(); axis++ )
		      rI(i1,i2,i3,axis)=rr(0,axis);
		    
                    break;
		  }
		}
	      } // end for g
	      
              if( interpolates(0) )
	      {
                if( debug & 4 )
		  printf(" updateCanInterpolate:point (%i,%i,%i) on grid %i now interpolates from grid %i\n",
                       i1,i2,i3,grid,inverseGrid(i1,i2,i3)); 
	      }
	      else
	      {
		if( canDiscretize(c,iv) )
		{
		  mask(i1,i2,i3)=MappedGrid::ISdiscretizationPoint;
		}
		// else if( !isNeededForDiscretization(c,iv) )
		//   mask(i1,i2,i3)=0;
		else
		{ // ***** should force the points we removed to be put back in.
		  printf("updateCanInterpolate:ERROR: point (%i,%i,%i) on grid %i cannot discretize\n",i1,i2,i3,grid);
		}
	      }
	    }
	  }
	}
      }
    }
  }
#endif
  return 0;
}





// int Ogen::
// updateMaskPeriodicity(MappedGrid & c, const int & i1, const int & i2, const int & i3 )
// // This routine will update c.mask() to be periodic given that the point c.mask()(i1,i2,i3)
// // has just been changed.
// {
//   // **** this routine is not used and is not correct ****

//   // ***** fix for doubly periodic ****

//   intArray & mask = c.mask();

//   const int iv[3] = {i1,i2,i3};  
//   int jv[3]= {i1,i2,i3};
//   int &j1=jv[0], &j2=jv[1], &j3=jv[2];
//   const int numberOfGhostPoints=1;
//   for( int axis=0; axis<c.numberOfDimensions(); axis++ )
//   {
//     if( c.isPeriodic()(axis) )
//     {
//       if( iv[axis]>=c.gridIndexRange()(End,axis)-numberOfGhostPoints )
//       {
// 	jv[axis]=c.gridIndexRange()(Start,axis)+(iv[axis]-c.gridIndexRange()(End,axis));
//         mask(j1,j2,j3)=mask(i1,i2,i3);
//         jv[axis]=iv[axis];
//       }
//       if( iv[axis]<=c.gridIndexRange()(Start,axis)+numberOfGhostPoints )
//       {
// 	jv[axis]=c.gridIndexRange()(End,axis)+(iv[axis]-c.gridIndexRange()(Start,axis));
//         mask(j1,j2,j3)=mask(i1,i2,i3);
//         jv[axis]=iv[axis];
//       }
//     }
//   }
//   return 0;
// }



int Ogen::
findBestGuess(CompositeGrid & cg, 
	      const int & grid, 
	      const int & numberToCheck, 
	      intSerialArray & ia, 
	      realSerialArray & x, 
	      realSerialArray & r,
              realSerialArray & rI,
              intSerialArray & inverseGrid,
              const realSerialArray & center )
// ===================================================================================
//  Given a list of points that should nearly be inside of the grids, find a best guess
// as to which grid they are closest to.
// ===================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  MappedGrid & c = cg[grid];
  const bool isRectangular = c.isRectangular();
  real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  int iv0[3]={0,0,0}; //
  if( isRectangular )
  {
    c.getRectangularGridParameters( dvx, xab );
    for( int dir=0; dir<c.numberOfDimensions(); dir++ )
    {
      iv0[dir]=c.gridIndexRange(0,dir);
      if( c.isAllCellCentered() )
	xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
    }
		
  }
  #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

  int axis;
  Range R, Rx;
  if( numberToCheck>0 )
  {
    
    x.redim(numberToCheck,3);
    r.redim(numberToCheck,3);  r=-1.;
    // R=numberToCheck-1;  *wdh* 081117
    R=numberToCheck; 
    Rx=cg.numberOfDimensions();
    if( !isRectangular )
    {
      for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	x(R,axis)=center(ia(R,0),ia(R,1),ia(R,2),axis);
    }
    else
    {
      int iv[3];
      for( int i=R.getBase(); i<=R.getBound(); i++ )
      {
	iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
	for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	  x(i,axis)=XC(iv,axis);
      }
    }
  }
  else
  {
    x.redim(0);
    r.redim(0);
  }
  
  // check grids: grid+1,...,cg.numberOfComponentGrids()-1,  grid-1,...,0
    
  Mapping & map = cg[grid].mapping().getMapping();

  realSerialArray distanceToCenter(R);
  distanceToCenter=.5 + .1;  // look for points where the max r distance from .5 is less than this value.
  for( int g=1; g<numberOfBaseGrids; g++ )
  {
    int grid2 = grid+g < numberOfBaseGrids ? grid+g : numberOfBaseGrids-g-1;
    assert( grid2!=grid );
    Mapping & map2 = cg[grid2].mapping().getMapping();

    if( map.intersects(map2,-1,-1,-1,-1,.1) )
    {
      if( info & 4 ) 
        printf("findBestGuess: try to interpolate grid %s from grid %s \n",
               (const char *)c.mapping().getName(Mapping::mappingName),
	       (const char *)map2.getName(Mapping::mappingName));

      // these mappings may intersect
      // if( useBoundaryAdjustment )           
      //  cg.rcData->adjustBoundary(grid,grid2,ia,x);    // adjust boundary points on shared sides ****** use this?

      #ifdef USE_PPP
        if( numberToCheck>0 )
          map2.inverseMapS(x(R,Rx),r);
        else
          map2.inverseMapS(x,r);
      #else
        map2.inverseMap(x(R,Rx),r);
      #endif
	
      if( numberToCheck>0 )
      {
	if( cg.numberOfDimensions()==2 )   
	{
	  where( max(fabs(r(R,0)-.5),fabs(r(R,1)-.5)) <= distanceToCenter(R) )
	  {
	    // these points are closer than the previous best (or else the first time we have a good enough guess)
	    distanceToCenter(R)=max(fabs(r(R,0)-.5),fabs(r(R,1)-.5));
	    inverseGrid(ia(R,0),ia(R,1),ia(R,2))=grid2;
	    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	      rI(ia(R,0),ia(R,1),ia(R,2),axis)=r(R,axis);
	  }
	}
	else
	{
	  where( max(max(fabs(r(R,0)-.5),fabs(r(R,1)-.5)),fabs(r(R,2)-.5)) <= distanceToCenter(R) )
	  {
	    // these points are closer than the previous best (or else the first time we have a good enough guess)
	    distanceToCenter(R)=max(max(fabs(r(R,0)-.5),fabs(r(R,1)-.5)),fabs(r(R,2)-.5));
	    inverseGrid(ia(R,0),ia(R,1),ia(R,2))=grid2;
	    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	      rI(ia(R,0),ia(R,1),ia(R,2),axis)=r(R,axis);
	  }
	}
      }
    }
  }
  return 0;
}


int Ogen::
adjustForNearbyBoundaries(CompositeGrid & cg,
                          IntegerArray & numberOfInterpolationPoints,
                          intSerialArray *iInterp )
// ===========================================================================================
// /Description:
//   When two boundaries are close together (see drop2.cmd) we may have to make sure
//   that there are enough interior points kept adjacent to the boundaries for an equation
//   to be applied on the boundary. Points are added back to accomplish this -- EVEN if these
//   points were cut by the hole-cutting algorithm. This may result in points outside the domain
//   needing to be interpolated.
//
//  /Algorithm:
//    May sure that every boundary point with mask(Ib1,Ib2,Ib3)!=0 has enough adjacent interior
//  points for discretization. The number of lines is given by the boundaryDiscretizationWidth.
// ===========================================================================================
{
#ifdef USE_PPP
  printF("adjustForNearbyBoundaries::Not implemented yet for parallel. Do nothing...\n");
  return 0;
#else

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  int grid;
  int numberOfInterpolationPointsAdded=0;
  Range Rx=cg.numberOfDimensions();
  
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & mg = cg[grid];
    intArray & mask = mg.mask();
    intArray & inverseGrid = cg.inverseGrid[grid];
    realArray & rI = cg.inverseCoordinates[grid];
    realArray & center = mg.center();
    intArray & ia2 = iInterp[grid];
    
    const bool isRectangular = mg.isRectangular();
    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      mg.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
	iv0[dir]=mg.gridIndexRange(0,dir);
	if( mg.isAllCellCentered() )
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
		
    }
    #undef XC
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

    int side,axis;
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
        if( mg.boundaryCondition(side,axis)>0 )
	{
	  Index Ib1,Ib2,Ib3, Ip1,Ip2,Ip3, Im1,Im2,Im3;
	  getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3);

          const int numberOfLines=(mg.boundaryDiscretizationWidth(0,axis) - 1);

          const intArray & boundaryMask = mask(Ib1,Ib2,Ib3);
          // mb= mask(Ib1,Ib2,Ib3) & CompositeGrid::ISinterpolationPoint;
          for( int g=1; g<=numberOfLines; g++ ) 
	  {
	    getGhostIndex(mg.indexRange(),side,axis,Ip1,Ip2,Ip3,-g);  // g'th line inside.
	    getGhostIndex(mg.indexRange(),side,axis,Im1,Im2,Im3,1-g);  // (g-1)'th line inside.
            intArray m(Ip1,Ip2,Ip3);
            m = boundaryMask && mask(Ip1,Ip2,Ip3)==0;
	    int num=sum(m);
	    
	    if( num>0 )
	    {
	      numberOfInterpolationPointsAdded+=num;

	      where( m )
	      {
		mask(Ip1,Ip2,Ip3) = CompositeGrid::ISinterpolationPoint;
		inverseGrid(Ip1,Ip2,Ip3)=inverseGrid(Im1,Im2,Im3);

                // need to set inverse coordinates rI()=...
                for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
                  rI(Ip1,Ip2,Ip3,dir)=rI(Im1,Im2,Im3,dir); // used as an initial guess

//                 if( g==1 )
// 		{ // last time through, reset boundary points to be interior points ??
// 		  // mask(Ib1,Ib2,Ib3)=MappedGrid::ISdiscretizationPoint;
// 		  // inverseGrid(Ib1,Ib2,Ib3)=-1;
// 		}
	      }

              // make a list of points to change, we need to find their inverse coordinates
              intArray ia(num,4);
              int i1,i2,i3;
              int i=0;
              for( i3=Ip3.getBase(); i3<=Ip3.getBound(); i3++ )
              for( i2=Ip2.getBase(); i2<=Ip2.getBound(); i2++ )
	      for( i1=Ip1.getBase(); i1<=Ip1.getBound(); i1++ )
	      {
                if( m(i1,i2,i3) )
		{
		  ia(i,0)=i1;
		  ia(i,1)=i2;
		  ia(i,2)=i3;
                  ia(i,3)=inverseGrid(i1,i2,i3);
                  assert( ia(i,3)>=0 && ia(i,3)<numberOfBaseGrids );
		  
		  i++;
		}
	      }
              // add new interp points to the list
              Range R=num, R3(0,2);
              ia2(R+numberOfInterpolationPoints(grid),R3)=ia(R,R3);
  	      numberOfInterpolationPoints(grid)+=num;
	      
	      for( i=0; i<num; i++ )
	      {
                int j=i;
                int grid2=ia(j,3);
                j++;
		while( j<num && ia(j,3)==grid2 ) // make a list of pts that all interpolate from grid2.
		{
		  j++;
		}
                R=Range(i,j-1);
		realArray r(R,Rx),x(R,Rx);
                int dir;
                if( !isRectangular )
		{
		  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		  {
		    x(R,dir)=center(ia(R,0),ia(R,1),ia(R,2),dir); // interpolate these points.
		    r(R,dir)=rI(ia(R,0),ia(R,1),ia(R,2),dir);
		  }
		}
		else
		{
		  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		  {
		    r(R,dir)=rI(ia(R,0),ia(R,1),ia(R,2),dir);
		  }
		  for( int i=R.getBase(); i<=R.getBound(); i++ )
		  {
                    int iv[3];
		    iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
		    for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		    {
		      x(dir)=XC(iv,dir); // interpolate these points.
		    }
		  }
		  
		}
		
		Mapping & map2 = cg[grid2].mapping().getMapping();
		map2.inverseMap(x,r);
                 
                if( max(fabs(r)) > 9. )  // 
		{
                  // for points that couldn't interpolate just use initial values ** could do better here
                  for( j=R.getBase(); j<=R.getBound(); j++ )
		  {
		    if( abs(r(j,0)>9. ) )
		    {
		      for( dir=0; dir<cg.numberOfDimensions(); dir++ )
			r(j,dir)=rI(ia(j,0),ia(j,1),ia(j,2),dir);
		    }
		  }
		}
                // project points outside grid2 onto the boundary
                for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		{
		  r(R,dir)=min(rBound(End,dir,grid2)-boundaryEps,max(rBound(Start,dir,grid2)+boundaryEps,r(R,dir)));
		}
                for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		  rI(ia(R,0),ia(R,1),ia(R,2),dir)=r(R,dir);
		
                i=j;
	      } // end for i
	      for( i=0; i<num; i++ )
	      {
		printf("adjustForNearbyBoundaries: grid=%i pt (%i,%i,%i) interps from grid2=%i at r=(%8.2e,%8.2e)\n",
		       grid,ia(i,0),ia(i,1),ia(i,2),inverseGrid(ia(i,0),ia(i,1),ia(i,2)),
                       rI(ia(i,0),ia(i,1),ia(i,2),0),rI(ia(i,0),ia(i,1),ia(i,2),1));
	      }

	    }
	  }
	}
      }
    }
    
  }
  return numberOfInterpolationPointsAdded;
#endif
}

// Old calling sequence for backward compatibility:
int Ogen::
checkCanInterpolate(CompositeGrid & cg ,
		    int grid, int donor, RealArray & r, IntegerArray & interpolates,
		    IntegerArray & useBackupRules )
{
  int numberToCheck= r.getLength(0);
  return checkCanInterpolate(cg , grid, donor, numberToCheck, r, interpolates, useBackupRules );
}

int Ogen::
checkCanInterpolate(CompositeGrid & cg ,
		    int grid, int donor, 
                    int numberToCheck,
                    RealArray & r, IntegerArray & interpolates,
		    IntegerArray & useBackupRules )
// ========================================================================================
// /Description:
//    Here is the new (parallel) canInterpolate function
//
// /grid, donor (input): check for interpolation of points on "grid" from donor
// /numberToCheck (input) : check this many points 
// /r (input) : r(i,0:nd-1) -- coordinates of the points to check, i=0,1,...,  numberToCheck-1
// /interpolates (output) : return true if point i can be interpolated
// / useBackupRules (input) : if useBackupRules(i)==true then allow backup interpolation rules.
// /Return value: true if all points could interpolate, false otherwise.
// ========================================================================================
{
  // NO: if( numberToCheck==0 ) return 0;
  bool allPointsCouldInterpolate=true;

  const int interpWidth = cg.interpolationWidth(0,grid,donor,0); // assumes the same width in all directions
  const int numberOfDimensions = cg.numberOfDimensions();
  
  using namespace CanInterpolate;
  
  CanInterpolateQueryData *cid = new CanInterpolateQueryData[max(1,numberToCheck)];
  const int rBase0=r.getBase(0);
  for( int n=0; n<numberToCheck; n++ )
  {
    int i =n+rBase0;
    cid[n].id=n; cid[n].i=i; cid[n].grid=grid; cid[n].donor=donor;
    for( int axis=0; axis<numberOfDimensions; axis++ )
      cid[n].rv[axis]=r(i,axis);

  }
  
  // Allocate space for results
  CanInterpolateResultData *cir =new CanInterpolateResultData[max(1,numberToCheck)];

  // --------------------------------
  // -------check canInterpolate-----
  // --------------------------------

  // this function will find any valid interpolation by default (i.e. backup results too)
  // this function also computes the interpolation stencil
  CanInterpolate::canInterpolate( cg, numberToCheck,cid, cir );

  // process the canInterpolate results
  const int interpolatesBase0=interpolates.getBase(0);
  for( int n=0; n<numberToCheck; n++ )
  {
    int width = cir[n].width;   // interpolation width (=0 if invalid)
    // int i=cid[n].i;
    // int grid=cid[n].grid;
    // cir[i].il[0];
    // cir[i].il[1];
    // cir[i].il[2];

    int i0=n+interpolatesBase0;
    if( width == interpWidth )
    {
      interpolates(i0)=true;
      // if( true )
      // {
      // 	printf("Ogen:checkCanInterp: n=%i i=%i grid=%i donor=%i width=%i interpWidth=%i "
      //          " r=(%5.3f,%5.3f,%5.3f) -> il=(%i,%i,%i) CAN interp \n",
      // 	       n,cid[n].i,cid[n].grid,cid[n].donor,width,interpWidth,cid[n].rv[0],cid[n].rv[1],cid[n].rv[2],
      //         cir[n].il[0],cir[n].il[1],cir[n].il[2]);
      // }
    }
    else if( useBackupRules(i0) && width>0 ) // what to do here ? 
    {
      interpolates(i0)=true;
      // if( true )
      // {
      // 	printf("Ogen:checkCanInterp: n=%i i=%i grid=%i donor=%i width=%i interpWidth=%i "
      //          " r=(%5.3f,%5.3f,%5.3f) -> il=(%i,%i,%i) CAN interp BACKUP\n",
      // 	       n,cid[n].i,cid[n].grid,cid[n].donor,width,interpWidth,cid[n].rv[0],cid[n].rv[1],cid[n].rv[2],
      //          cir[n].il[0],cir[n].il[1],cir[n].il[2]);
      // }
    }
    else
    {
      interpolates(i0)=false;
      allPointsCouldInterpolate=false;
      
      // if( true )
      // {
      // 	printf("Ogen:checkCanInterp: n=%i i=%i grid=%i donor=%i width=%i interpWidth=%i "
      //          " r=(%5.3f,%5.3f,%5.3f) -> il=(%i,%i,%i) CAN NOT interp\n",
      // 	       n,cid[n].i,cid[n].grid,cid[n].donor,width,interpWidth,cid[n].rv[0],cid[n].rv[1],cid[n].rv[2],
      //          cir[n].il[0],cir[n].il[1],cir[n].il[2]);
      // }
      

    }
  }

//     intSerialArray & variableInterpolationWidth = cg->variableInterpolationWidthLocal[grid]; 
//     // Choose the donor point with largest width (and use the first one with that width) : 
//     if( width>variableInterpolationWidth(i) )
//     {
//       int interpolee=cid[n].donor;
//     }

  delete [] cid;
  delete [] cir;
  
  return allPointsCouldInterpolate;
}


int Ogen::
classifyPoints(CompositeGrid & cg,
	       realSerialArray & invalidPoint, 
	       int & numberOfInvalidPoints,
               const int & level,
               CompositeGrid & cg0 )
// ======================================================================================
// 
// /Description:
//   Given a grid with exterior points removed and boundary interpolation marked, 
//    determine the interpolation points and hole points.
//
// /cg (input) : grid for this level. 
// /invalidPoints (output) : return a list of invalid points
// /numberOfInvalidPoints (output) : the number of invalid points
// /level (input) : multigrid level
// /cg0 (input) : grid on level 0
// ======================================================================================
{
  real time,time0=getCPU();
  
  if( info & 4 ) printf("classify points...\n");
  if( info & 2 )
  {
    Overture::printMemoryUsage("classifyPoints (start)");
    // Overture::checkMemoryUsage("classifyPoints (start)");
  }


  assert( ps!=NULL );
  // PlotStuff & gi = *ps;

  const int numberOfDimensions = cg.numberOfDimensions();

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  numberOfInvalidPoints=0;

  int grid, grid2, side, axis;
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  // const real eps = REAL_EPSILON*20.;   // **** is this the same as cg.epsilon ?
  intSerialArray ia;
  intSerialArray *iInterp = new intSerialArray [ numberOfBaseGrids ];

  delete [] backupValues; // these could have been created by moving update
  backupValues = new intSerialArray [ numberOfBaseGrids ];  // to hold backup values
  // backupValuesUsed(grid) = true is some backup values have been used
  backupValuesUsed.redim(numberOfBaseGrids);
  backupValuesUsed=false;


  realSerialArray r,x;
  
  intSerialArray interpolates(1), useBackupRules(1), interpolatesArray, useBackupRulesArray;
  useBackupRules=FALSE;
  // 
  // If checkForOneSided=TRUE then canInterpolate will not allow a one-sided interpolation
  // stencil to use ANY interiorBoundaryPoint's -- this is actually too strict. We really
  // only want to disallow interpolation that has less than the minimum overlap distance
  //
  checkForOneSided=FALSE;  // check for one side interp from non-conforming grids *******

  Range R, Rx(0,numberOfDimensions-1), R3(0,2), all, Gb=numberOfBaseGrids;
  IntegerArray numberOfInterpolationPoints(numberOfBaseGrids);
  
  // if( true && numberOfBaseGrids>7 ) // TEMP
  // {
  //   int grid=7, i1=170,i2=52,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Classify:START: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }
  // if( true && numberOfBaseGrids>9 ) // TEMP
  // {
  //   int grid=9, i1=158,i2=43,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Classify:START: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }


  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    intArray & maskd = c.mask();
    intArray & inverseGridd = cg.inverseGrid[grid];
    realArray & rId = cg.inverseCoordinates[grid];
    intSerialArray & ia2 = iInterp[grid];
    const bool isRectangular = c.isRectangular();
    
    GET_LOCAL(int,maskd,mask);
    GET_LOCAL(int,inverseGridd,inverseGrid);
    GET_LOCAL(real,rId,rI);
    
    #ifdef USE_PPP
      realSerialArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(c.center(),center);
    #else
      const realSerialArray & center = c.center();
    #endif


    const int bogusInverseGrid=cg.numberOfGrids()+100;  // also appears in checkHoleCutting

    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      c.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<numberOfDimensions; dir++ )
      {
	iv0[dir]=c.gridIndexRange(0,dir);
	if( c.isAllCellCentered() )
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
		
    }
    #undef XC
    #define XC0(i1,i2,i3) (xab[0][0]+dvx[0]*(i1-iv0[0]))
    #define XC1(i1,i2,i3) (xab[0][1]+dvx[1]*(i2-iv0[1]))
    #define XC2(i1,i2,i3) (xab[0][2]+dvx[2]*(i3-iv0[2]))
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

    if( debug & 8 )
    {
      displayMask(maskd,sPrintF("Mask for grid %i at start of classify points",grid),logFile);
      display(inverseGridd,sPrintF("inverseGrid for grid %i at start",grid),logFile,"%3i");
    }
    

    intSerialArray initialInverseGrid;
    bool useNewImproperInterpolation=FALSE;
    if( useNewImproperInterpolation )       // **** this doesn't seem to work. maybe priority?
    {
      // no need to check existing interpolation points for proper interpolation.
      // Save them to be added later.
      initialInverseGrid=inverseGrid;
      inverseGrid=-1;
    }

    // ===== mark points on interpolation boundaries that need to be interpolated ======
    //   o interpolation boundaries
    //   o points next to exterior points
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( c.boundaryCondition()(side,axis)==0 )
	{
          // getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);
          const int halfWidth = c.discretizationWidth(axis)/2;
          for( int ghost=0; ghost>-halfWidth; ghost-- )
	  {
	    getGhostIndex(c.extendedIndexRange(),side,axis,I1,I2,I3,ghost);

	    bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
	    if( !ok ) continue;

	    where( mask(I1,I2,I3) > 0 )  // != 0
	    {
	      mask(I1,I2,I3)= MappedGrid::ISinterpolationPoint;
	      inverseGrid(I1,I2,I3)=bogusInverseGrid;
	    }
	  }
	}
      }
    }
    //  mark points next to exterior points (mask==0). The number of points we mark
    // depends on how wide the discretization width is.
    getIndex(c.extendedIndexRange(),I1,I2,I3);
    const int halfWidth1 = c.discretizationWidth(axis1)/2;
    const int halfWidth2 = numberOfDimensions>1 ? c.discretizationWidth(axis2)/2 : 0;
    const int halfWidth3 = numberOfDimensions>2 ? c.discretizationWidth(axis3)/2 : 0;
    const int hw[3] = { halfWidth1,halfWidth2,halfWidth3};  
    
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      if( min(c.numberOfGhostPoints()(Range(0,1),axis))<hw[axis] )
      {
	printf("Ogen::ERROR: the number of ghost points must be at least %i if the discretization width =%i\n",
	       hw[axis],c.discretizationWidth(axis));
	throw "error";
      }
      Iv[axis] = Range(max(Iv[axis].getBase() ,c.dimension()(Start,axis)+hw[axis]),
                       min(Iv[axis].getBound(),c.dimension()(End  ,axis)-hw[axis]));
    }
    

    // The mask "m(i1,i2,i3)" will be true if the discr. stencil for point (i1,i2,i3) has
    // a hole point in it (mask=0)
    bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
    if( ok )
    {
      intSerialArray m(I1,I2,I3);
      m=0;
    
      for( int m3=-halfWidth3; m3<=halfWidth3; m3++ )
	for( int m2=-halfWidth2; m2<=halfWidth2; m2++ )
	  for( int m1=-halfWidth1; m1<=halfWidth1; m1++ )
	  {
	    if( m1!=0 || m2!=0 || m3!=0 )
	      m = m || mask(I1+m1,I2+m2,I3+m3)==0;
	  }
      where( mask(I1,I2,I3) > 0 && m )
      {
	mask(I1,I2,I3)= MappedGrid::ISinterpolationPoint;
	inverseGrid(I1,I2,I3)=bogusInverseGrid;
      }
    }
    
    // make a list of points to check
    getIndex(c.extendedRange(),I1,I2,I3);
    ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);  // *wdh* 090309 
    if( ok )
      ia2.redim(I1.length()*I2.length()*I3.length(),3);  // *** this is too big
    else
      ia2.redim(0);

    getIndex(c.extendedIndexRange(),I1,I2,I3);
    // mask.display("mask after marking interpolation points");
    if( TRUE )
    {
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	if( c.boundaryFlag(Start,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	  Iv[axis]=Range(c.gridIndexRange(Start,axis)-1,Iv[axis].getBound());
	if( c.boundaryFlag(End,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	  Iv[axis]=Range(Iv[axis].getBase(),c.gridIndexRange(End,axis)+1);
      }
    }

    // ----------------------------------------------------------------------
    // ====== check for (improper) interpolation from grid2 > grid: =========
    //        improper interpolation : point is inside grid2
    // ----------------------------------------------------------------------

    // ***** NOTE**** we should not have to recheck interpolation points already computed   *******




    int i=0;
    ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
    if( ok )
    {
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    // ** if( inverseGrid(i1,i2,i3)==grid )
	    if( inverseGrid(i1,i2,i3) >=0  )  // This will also add already existing interp points to the list, from
	    {                                 // interp on the boundary
	      ia2(i,0)=i1;
	      ia2(i,1)=i2;
	      ia2(i,2)=i3;
	      i++;
	    }
	  }
	}
      }
    }
    
    numberOfInterpolationPoints(grid)=i;

    int numberToCheck=numberOfInterpolationPoints(grid);
    x.redim(numberToCheck,3);
    r.redim(numberToCheck,3); r=-1.;
    ia.redim(numberToCheck,3);
    R=Range(0,numberToCheck-1);
    ia(R,R3)=ia2(R,R3);

    // check grids from highest priority first: numberOfBaseGrids-1,numberOfBaseGrids-2,...
    Mapping & map = cg[grid].mapping().getMapping();
    for( int g=1; g<numberOfBaseGrids; g++ )
    {
      grid2 = numberOfBaseGrids-g;
      if( grid2<=grid )
        grid2--;
      
      if( (!isNew(grid) && !isNew(grid2)) || !cg0.mayInterpolate(grid,grid2,0)   )
      {
        if( info & 4 )
          printf("skip improper interpolation for grids: grid=%i, grid2=%i myid=%i\n",grid,grid2,myid);
        continue;
      }

      Mapping & map2 = cg[grid2].mapping().getMapping();

      int mapIntersects= map.intersects(map2,-1,-1,-1,-1,.1);
#ifdef USE_PPP
      if( debug & 2 )
      {
	int iMin = ParallelUtility::getMinValue(mapIntersects);
	int iMax = ParallelUtility::getMaxValue(mapIntersects);
	if( iMin!=iMax )
	{
	  printf("ERROR: grid=%i grid2=%i mapIntersects=%i for myid=%i\n",grid,grid2,mapIntersects,myid);
	  const RealArray & bb = map.getBoundingBox(-1,-1);
	  const RealArray & bb2 = map2.getBoundingBox(-1,-1);
	  printf(" myid=%i grid =%i: boundingBox=[%16.10e,%16.10e][%16.10e,%16.10e][%16.10e,%16.10e]\n",
		 myid,grid,bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
	  printf(" myid=%i grid2=%i: boundingBox=[%16.10e,%16.10e][%16.10e,%16.10e][%16.10e,%16.10e]\n",
		 myid,grid2,bb2(0,0),bb2(1,0),bb2(0,1),bb2(1,1),bb2(0,2),bb2(1,2));
	
	  Overture::abort("error");
	}
      }
#endif

      if( !mapIntersects && info & 4 ) 
	printf("Improper: grid %i (%s) does NOT intersect grid %i (%s) (myid=%i)\n",
	       grid,(const char *)cg[grid].getName(),grid2,(const char *)cg[grid2].getName(),myid);

      if( mapIntersects ) // map.intersects(map2,-1,-1,-1,-1,.1) )
      {
        if( info & 4 ) 
	{
	  
          printf(" try to interpolate grid %i (%s) from grid %i (%s) (myid=%i)\n",
                 grid,(const char *)cg[grid].getName(),
		 grid2,(const char *)cg[grid2].getName(),myid);
	  
          fflush(0);
          // #ifdef USE_PPP
 	  // MPI_Barrier(Overture::OV_COMM);  // Add this for testing
          // #endif	
        }
	
	// these mappings may intersect

	if(  TRUE || g!=1  )  // ***wdh make a sub list *wdh* for cgrid
	{
	  // make a new list of points that remain to be checked    **** could check bounding box too *************
	  i=0;
	  for( int j=0; j<numberToCheck; j++ )
	  {
	    if( inverseGrid(ia(j,0),ia(j,1),ia(j,2)) == bogusInverseGrid 
		|| inverseGrid(ia(j,0),ia(j,1),ia(j,2))<grid2  ) // ***wdh make sure highest priority grid is chosen
	    {
	      ia(i,0)=ia(j,0);
	      ia(i,1)=ia(j,1);
	      ia(i,2)=ia(j,2);
	      i++;
	    }
	  }
	  numberToCheck=i;
	}
        #ifndef USE_PPP
   	if( numberToCheck==0 ) // in serial we can break here
	  break;
        #endif
	R=numberToCheck; 
	if( numberToCheck>0 )
	{
	  if( !isRectangular )
	  {
	    for( axis=0; axis<numberOfDimensions; axis++ )
	      x(R,axis)=center(ia(R,0),ia(R,1),ia(R,2),axis);
	  }
	  else
	  {
	    for( int i=R.getBase(); i<=R.getBound(); i++ )
	    {
	      iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
	      for( axis=0; axis<numberOfDimensions; axis++ )
		x(i,axis)=XC(iv,axis);
	    }
	  }
	}
	
        if( useBoundaryAdjustment )
          if( numberToCheck>0 )
            adjustBoundary(cg,grid,grid2,ia(R,Rx),x);    // adjust boundary points on shared sides *** only do at end?
          else
            adjustBoundary(cg,grid,grid2,Overture::nullIntArray(),Overture::nullRealArray());

        #ifdef USE_PPP
          if( numberToCheck>0 )
           map2.inverseMapS(x(R,Rx),r);
          else
           map2.inverseMapS(Overture::nullRealArray(),Overture::nullRealArray());
        #else
  	  if( numberToCheck>0 )
            map2.inverseMap(x(R,Rx),r);
        #endif
        if( debug & 16 )
          display(r(R,Rx),"Here are the inverseMap coordinates",plogFile);

 	if( numberToCheck==0 ) // no communication after this 
 	  continue;            // *wdh* 070413 -- we cannot break here in parallel, we must continue to next grid2

	if( numberOfDimensions==2 )
	{
	  // where( fabs(r(R,0)-.5) <= .5+eps && fabs(r(R,1)-.5) <= .5+eps )
	  where( r(R,0)>rBound(Start,0,grid2) && r(R,0)<rBound(End,0,grid2) &&
                 r(R,1)>rBound(Start,1,grid2) && r(R,1)<rBound(End,1,grid2) )
	  {
	    inverseGrid(ia(R,0),ia(R,1),ia(R,2)) = grid2;   // can interpolate from grid2
	    for( axis=0; axis<numberOfDimensions; axis++ )
              rI(ia(R,0),ia(R,1),ia(R,2),axis)=r(R,axis);   // save coordinates
	  }
          // rI.display("here is rI");
	}
	else if( numberOfDimensions==3 )
	{
	  // where( fabs(r(R,0)-.5) <= .5+eps && fabs(r(R,1)-.5) <= .5+eps && fabs(r(R,2)-.5) <= .5+eps )
	  where( r(R,0)>rBound(Start,0,grid2) && r(R,0)<rBound(End,0,grid2) &&
                 r(R,1)>rBound(Start,1,grid2) && r(R,1)<rBound(End,1,grid2) &&
                 r(R,2)>rBound(Start,2,grid2) && r(R,2)<rBound(End,2,grid2) )
	  {
	    inverseGrid(ia(R,0),ia(R,1),ia(R,2)) = grid2;
	    for( axis=0; axis<numberOfDimensions; axis++ )
              rI(ia(R,0),ia(R,1),ia(R,2),axis)=r(R,axis);   // save coordinates
	  }
	  
	}
        else
	{
	  where( r(R,0)>rBound(Start,0,grid2) && r(R,0)<rBound(End,0,grid2) )
	  {
	    inverseGrid(ia(R,0),ia(R,1),ia(R,2)) = grid2;   // can interpolate from grid2
            rI(ia(R,0),ia(R,1),ia(R,2),0)=r(R,0);   // save coordinates
	  }
          
	}
	
      }
      
    }
    if( debug & 2 )
    {
      fprintf(plogFile," grid: %s, numberOfInterpolationPoints=%i [ISinteriorBoundaryPoint=%i"
             " ISdiscretizationPoint=%i]\n",
           (const char *)c.mapping().getName(Mapping::mappingName),
	   numberOfInterpolationPoints(grid),MappedGrid::ISinteriorBoundaryPoint,
          MappedGrid::ISdiscretizationPoint);
      for( i=0; i<numberOfInterpolationPoints(grid); i++ )
      {
	fprintf(plogFile," i=%i, interpolationPoint=(%i,%i,%i), interpoleeGrid=%i, rI=(%e,%e,%e), mask=%i \n",i,
	       ia2(i,0),ia2(i,1),ia2(i,2),inverseGrid(ia2(i,0),ia2(i,1),ia2(i,2)), 
	       rI(ia2(i,0),ia2(i,1),ia2(i,2),0),rI(ia2(i,0),ia2(i,1),ia2(i,2),1),
		( numberOfDimensions==2 ? 0. : rI(ia2(i,0),ia2(i,1),ia2(i,2),2)),
                 mask(ia2(i,0),ia2(i,1),ia2(i,2)));
      }
    }
    
    // Look for points that could not interpolate from any other grid
    i=0;
    for( int j=0; j<numberToCheck; j++ )
    {
      if( inverseGrid(ia(j,0),ia(j,1),ia(j,2)) == bogusInverseGrid )
      {
	ia(i,0)=ia(j,0);
	ia(i,1)=ia(j,1);
	ia(i,2)=ia(j,2);
	i++;
      }
    }
    numberToCheck=i;
    int maxNumberToCheck=ParallelUtility::getMaxValue(numberToCheck); // *wdh* 081001
    if( maxNumberToCheck > 0 ) 
    {
      // These points should be almost inside one grid --- they may be slightly outside 
      // Therefore check again find the next best guess
      findBestGuess(cg,grid,numberToCheck,ia,x,r,rI,inverseGrid,center);

      IntegerArray ialc(1,3), isOk(1,3);
      isOk=true;
      for( i=0; i<numberToCheck; i++ )
      {
	if( inverseGrid(ia(i,0),ia(i,1),ia(i,2)) == bogusInverseGrid )
	{
          if( allowHangingInterpolation )
	  {
	    // allow "hanging" interpolation which we treat as interpolating from the same grid
            printf("INFO: Hanging interpolation for point on grid %s, i=%i, i=(%i,%i,%i), "
		   "x=(%6.2e,%6.2e,%6.2e)\n",
		   (const char *)c.mapping().getName(Mapping::mappingName),i,ia(i,0),ia(i,1),ia(i,2),
		   x(i,0),x(i,1),x(i,2));
            mask(ia(i,0),ia(i,1),ia(i,2)) |= MappedGrid::USESbackupRules;
	    continue;
	  }

          // the algorithm failed! redo lastChance and output diagnostics
	  // iv[0]=ia(i,0); iv[1]=ia(i,1); iv[2]=ia(i,2);
	  ialc(0,0)=ia(i,0); ialc(0,1)=ia(i,1); ialc(0,2)=ia(i,2);
          interpolates(0)=false;
          int lastChanceOption=0; // -1 = do not remove un-needed at this stage
	  lastChanceInterpolation(cg,cg0,grid,ialc,isOk,interpolates,numberOfInvalidPoints,invalidPoint,true,
				  false,true,lastChanceOption);

          if( interpolates(0) )
            continue;
	  
	  isOk=false;

	  printf(" ***FATAL ERROR*** unable to interpolate point on grid %s, i=%i, i=(%i,%i,%i), "
		 "x=(%6.2e,%6.2e,%6.2e)\n",
		 (const char *)c.mapping().getName(Mapping::mappingName),i,ia(i,0),ia(i,1),ia(i,2),
		 x(i,0),x(i,1),x(i,2));
	  if( invalidPoint.getLength(0) <= numberOfInvalidPoints )
	  {
	    invalidPoint.resize(invalidPoint.getLength(0)*2+100,numberOfDimensions+1);
	  }
          if( !isRectangular )
	  {
	    for( axis=0; axis<numberOfDimensions; axis++ )
	      invalidPoint(numberOfInvalidPoints,axis)=center(ia(i,0),ia(i,1),ia(i,2),axis);
	  }
	  else
	  {
            iv[0]=ia(i,0); iv[1]=ia(i,1); iv[2]=ia(i,2);
	    for( axis=0; axis<numberOfDimensions; axis++ )
	      invalidPoint(numberOfInvalidPoints,axis)=XC(iv,axis);
	  }
	  
          invalidPoint(numberOfInvalidPoints,numberOfDimensions)=grid;
	  numberOfInvalidPoints++;
	}
        else
	{
          if( info & 2 ) 
	    printf("**WARNING** findBestGuess was used for point on grid %s, i=%i, i=(%i,%i,%i), "
                  " r=(%6.2e,%6.2e,%6.2e), grid2=%i\n",
                 (const char *)c.mapping().getName(Mapping::mappingName),i,ia(i,0),ia(i,1),ia(i,2),
                 rI(ia(i,0),ia(i,1),ia(i,2),0),rI(ia(i,0),ia(i,1),ia(i,2),1),
		 ( numberOfDimensions==2 ? 0. : rI(ia(i,0),ia(i,1),ia(i,2),2)),
                 inverseGrid(ia(i,0),ia(i,1),ia(i,2)) );
          // project the point onto the boundary
	  rI(ia(i,0),ia(i,1),ia(i,2),Rx)=max(0.,min(1.,rI(ia(i,0),ia(i,1),ia(i,2),Rx)));
	}
      }
      if( !isOk(0) )
	return 1;   // *********************************  fix in parallel ****
    }

    if( useNewImproperInterpolation )
    {
      // now add the interpolation points found by interp on boundaries and cutting holes to the list
      i=numberOfInterpolationPoints(grid);
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    if( initialInverseGrid(i1,i2,i3) >=0  )  // add points from interp on the boundary
	    {
	      inverseGrid(i1,i2,i3)=initialInverseGrid(i1,i2,i3);
	      ia2(i,0)=i1;
	      ia2(i,1)=i2;
	      ia2(i,2)=i3;
	      i++;
	    }
	  }
	}
      }
      numberOfInterpolationPoints(grid)=i;
    }

    Overture::checkMemoryUsage(sPrintF("classifyPoints (improper, grid=%i)",grid));

  } // end for grid
  
  

//   if( true ) // 2012/06/18 --------------------------- TEST ------------------------------------------
//   {
//     delete [] iInterp; iInterp=NULL;
//     delete [] backupValues; backupValues=NULL;
//     return 0;
    
//   }
  



  time=getCPU();
  if( info & 2 ) 
    printF(" time to compute improper interpolation...................%e (total=%e)\n",time-time0,time-totalTime);
  timeImproperInterpolation=time-time0;
  time0=time;

  if( info & 2 )
    Overture::printMemoryUsage("classifyPoints (after improper)");

  if( Ogen::debug & 1 )
  {
    generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
    cg.numberOfInterpolationPoints(Gb)=numberOfInterpolationPoints(Gb); // *wdh* 2012/10/25 
    
    plot( "After marking (improper) interpolation",cg);
  }

  // make adjustments for two boundaries that get very close to each other to make sure
  // we can discretize boundary conditions.
  if( makeAdjustmentsForNearbyBoundaries )
  {
    int numberAdjusted = adjustForNearbyBoundaries( cg,numberOfInterpolationPoints,iInterp );
    if( numberAdjusted>0 )
    {
      printf("**** %i points changed by adjustForNearbyBoundaries\n",numberAdjusted);
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );  // this is needed for some reason

      if( debug & 1) 
        plot( "After adding back points for nearby boundaries",cg);
    }
  }
  
  if( debug & 8 )
  {
    for( grid=0; grid<numberOfBaseGrids; grid++ )
      displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after improper",grid),logFile);
  }

  // if( true && numberOfBaseGrids>7 ) // TEMP
  // {
  //   int grid=7, i1=170,i2=52,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Classify:After improper: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }
  // if( true && numberOfBaseGrids>9 ) // TEMP
  // {
  //   int grid=9, i1=158,i2=43,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Classify:After improper: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }


  // =================================================
  //   Now enforce that the interpolation be proper
  // =================================================


//   if( true ) // 2012/06/18 --------------------------- TEST ------------------------------------------
//   {
//     delete [] iInterp; iInterp=NULL;
//     delete [] backupValues; backupValues=NULL;
//     return 0;
 
//   }

  // *** now make sure the periodicity of the mask is enforced from now on
  //     This is used by 
  //        canInterpolate: ??
  //        canDiscretize : checks for MappedGrid::ISusedPoint
  //        isNeededForDiscretization : checks for MappedGrid::ISdiscretizationPoint

  // Now we check for one sided interpolation from near boundaries of nonconforming grids that
  // are interpolating
  checkForOneSided=TRUE;  

  realSerialArray rr(1,3); rr=-1.;
  
  real timeForCanInterpolate=0.;
  real timeForFixingProperInterpolation=0.;
  
  int numberOfProperInterpolationErrors=0;
  bool ok=true; // for testing
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    Mapping & map = c.mapping().getMapping();
    intArray & maskd = c.mask();
    intArray & inverseGridd = cg.inverseGrid[grid];
    realArray & rId = cg.inverseCoordinates[grid];
    intSerialArray & ia2 = iInterp[grid];
    // const realArray & center = c.center();
    const bool isRectangular = c.isRectangular();


    GET_LOCAL(int,maskd,mask);
    GET_LOCAL(int,inverseGridd,inverseGrid);
    GET_LOCAL(real,rId,rI);
    
    #ifdef USE_PPP
      realSerialArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(c.center(),center);
    #else
      const realSerialArray & center = c.center();
    #endif


    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      c.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<numberOfDimensions; dir++ )
      {
	iv0[dir]=c.gridIndexRange(0,dir);
	if( c.isAllCellCentered() )
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
		
    }
    #undef XC0
    #undef XC1
    #undef XC2
    #undef XC
    #define XC0(i1,i2,i3) (xab[0][0]+dvx[0]*(i1-iv0[0]))
    #define XC1(i1,i2,i3) (xab[0][1]+dvx[1]*(i2-iv0[1]))
    #define XC2(i1,i2,i3) (xab[0][2]+dvx[2]*(i3-iv0[2]))
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))


    c.mask().periodicUpdate();
    c.mask().updateGhostBoundaries();

    if( debug & 8 )
      displayMask(maskd,sPrintF("Mask for grid %i at start of proper interpolation",grid),logFile);


    // we can't do this in parallel since checkCanInterpolate is a collective operation
    
    if( false && numberOfInterpolationPoints(grid)<=0 )   
      continue;

    // ===  sort the interpolation points by interpolee grid so we can optimize the checking.
    //   ia( ni(Start,grid2): ni(End,grid2),Rx) : these pts interpolate from grid2 
    IntegerArray ng(numberOfBaseGrids), ni(2,numberOfBaseGrids);
    ng=0;
    const int numberOfBaseGrids=cg.numberOfBaseGrids();
    const int numberOfInterp = numberOfInterpolationPoints(grid);
    // first we count:
    int i;
    for( i=0; i<numberOfInterp; i++ )
    {
      grid2=inverseGrid(ia2(i,0),ia2(i,1),ia2(i,2));
      if( grid2>=0 && grid2<numberOfBaseGrids )
      {
	ng(grid2)++;
      }
    }
    ni(0,0)=0;
    ni(1,0)=ng(0)-1;
    ng(0)=0;           // reset ng(grid) to point to ni(Start,grid)
    for( grid2=1; grid2<numberOfBaseGrids; grid2++ )
    {
      ni(0,grid2)=ni(1,grid2-1)+1;
      ni(1,grid2)=ni(0,grid2)+ng(grid2)-1;
      ng(grid2)=ni(0,grid2); // reset ng(grid) to point to ni(Start,grid)
    }
    // now fill in the sorted array:
    if( numberOfInterp>0 )
      ia.redim(numberOfInterp,3);
    else
      ia.redim(0);
    
    for( i=0; i<numberOfInterp; i++ )
    {
      grid2=inverseGrid(ia2(i,0),ia2(i,1),ia2(i,2));
      if( grid2>=0 && grid2<numberOfBaseGrids )
      {
	ia(ng(grid2),R3)=ia2(i,R3);
	ng(grid2)++;
      }
    }

//    if( true ) // 2012/06/18 ----------OK ---------------- TEST ------------------------------------------
//    {
//      continue;
//    }

    Range R,R1;
    realSerialArray r2;

    for( grid2=0; grid2<numberOfBaseGrids; grid2++ )
    {
      // if( grid2==grid || (ni(1,grid2)-ni(0,grid2) <0) )
      if( grid2==grid )
	continue;

      int numInterp = ni(1,grid2)-ni(0,grid2)+1;
      
      if( numInterp>0 )
      {
	if( debug & 2 )
	  fprintf(plogFile,"proper: grid=%i, grid2=%i ni(0,grid2)=%5i ni(1,grid2)=%5i number=%i\n",grid,grid2,
		  ni(0,grid2),ni(1,grid2),ni(1,grid2)-ni(0,grid2)+1);

        R=Range(0,ni(1,grid2)-ni(0,grid2));
        R1=Range(ni(0,grid2),ni(1,grid2));

	r2.redim(R,Rx);
	for( axis=0; axis<numberOfDimensions; axis++ )
	  r2(R,axis)=rI(ia(R1,0),ia(R1,1),ia(R1,2),axis);

	interpolatesArray.redim(R);
	interpolatesArray(R)=TRUE;
	useBackupRulesArray.redim(R);
	useBackupRulesArray=FALSE;
      }
      else
      {
	r2.redim(0);
        interpolatesArray.redim(0);
	useBackupRulesArray.redim(0);
      }

//    if( true ) // 2012/06/18 ---------OK ------------------ TEST ------------------------------------------
//    {
//      continue;
//    }

      // This will check if we have a valid interpolation point. It uses the cg.interpolationOverlap
      // array to decide the size of the stencil required.
      // **NOTE** use cg0 as canInterpolate checks the multigrid level

      // ** *wdh* 110621 : We should really replace the calls below to a new function findBestInterpolation
      // that will find the best (if any) interpolation width, interpolation type.

      real time2=getCPU();
#ifdef USE_PPP
        checkCanInterpolate(cg0,cg.gridNumber(grid),cg.gridNumber(grid2), r2, interpolatesArray,
			    useBackupRulesArray);
#else
        cg0.rcData->canInterpolate(cg.gridNumber(grid),cg.gridNumber(grid2), r2, interpolatesArray,
 	   			   useBackupRulesArray, checkForOneSided );
#endif
      
      timeForCanInterpolate+=getCPU()-time2;

      time2=getCPU();

 //   if( true ) // 2012/06/18 ----LEAK HERE ----------------------- TEST ------------------------------------------
//    {
//      continue;
//    }



      // if( numInterp<=0 ) continue;  // ********
       
      const int numberOfPointsToCheck=numInterp; // R.getLength();


      if( numberOfPointsToCheck>0 && debug & 16 )
      {
        fprintf(plogFile,"\n ----- proper interpolation: first check validity of improper points, grid=%i, grid2=%i --- \n",
                grid,grid2);
	for( int i=R.getBase(); i<=R.getBound(); i++ )
	{
	  fprintf(plogFile," proper: i=%i grid=%i grid2=%i iv=(%i,%i), canInterpolate=%i\n",i,grid,grid2,
		  ia(ni(0,grid2)+i,0),ia(ni(0,grid2)+i,1),interpolatesArray(i));
	}
      }
      

      intSerialArray ib(R);
      const int i0=ni(0,grid2);
	
      int iStart=0, iEnd=numberOfPointsToCheck-1;  // marks range of points including pts left to check
	
      for( int g2=grid2-1; g2>=0; g2-- )
      {
	// *wdh* 090422 if( g2==grid || !cg0.mayInterpolate(grid,grid2,0) )
	if( g2==grid || !cg0.mayInterpolate(grid,g2,0) )
	  continue;       // skip this one. we cannot interpolate from the same grid
	  
	Mapping & map2 = cg[g2].mapping().getMapping();

	if( map.intersects(map2,-1,-1,-1,-1,.1) )
	{

	  // make a list of points that still need to be fixed
	  int j=0, iEndNew=iStart-1;
	  for( int i=iStart; i<=iEnd; i++ )
	  {
	    if( !interpolatesArray(i) )  // pt couldn't interpolate
	    {
	      ib(j)=i;
	      if( j==0 ) 
		iStart=i;
	      iEndNew=i;
	      j++;
	    }
	  }
	  iEnd=iEndNew;
	    
	  int numberOfPointsLeftToCheck=j;
          #ifndef USE_PPP
	  if( numberOfPointsLeftToCheck==0 )
	    break;                           // in serial we can break here
          #endif
	  R=numberOfPointsLeftToCheck;

	  intSerialArray iaa;
	  if( numberOfPointsLeftToCheck>0 ) 
	  {
	    iaa.redim(R,R3);
	    rr.redim(R,Rx);  rr=-1.;
	    for( axis=0; axis<3; axis++ ) 
	      iaa(R,axis)=ia(ib(R)+i0,axis);
	    
	    x.redim(R,Rx);
	    if( !isRectangular )
	    {
	      for( axis=0; axis<numberOfDimensions; axis++ )
		x(R,axis)=center(iaa(R,0),iaa(R,1),iaa(R,2),axis);
	    }
	    else
	    {
	      for( int i=R.getBase(); i<=R.getBound(); i++ )
	      {
		iv[0]=iaa(i,0), iv[1]=iaa(i,1), iv[2]=iaa(i,2);
		for( axis=0; axis<numberOfDimensions; axis++ )
		  x(i,axis)=XC(iv,axis);
	      }
	    }
	  }
	  else
	  {
	    x.redim(0);
	  }
	  
	  if( useBoundaryAdjustment )
	    adjustBoundary(cg,grid,g2,iaa,x); 
	  // adjustBoundary(cg,grid,g2,iaa(R,Rx),x); 

#ifdef USE_PPP
	  map2.inverseMapS(x,rr);
#else
          if( numberOfPointsLeftToCheck>0 ) 
	    map2.inverseMap(x,rr);
#endif
	  if( numberOfPointsLeftToCheck>0 ) 
	  {
	    interpolates.redim(R);
	    interpolates=FALSE;
	
	    if( numberOfDimensions==2 )
	    {
	      where( rr(R,0)>rBound(Start,0,g2) && rr(R,0)<rBound(End,0,g2) &&
		     rr(R,1)>rBound(Start,1,g2) && rr(R,1)<rBound(End,1,g2) )
	      {
		inverseGrid(iaa(R,0),iaa(R,1),iaa(R,2)) = g2;   // can interpolate from g2  // ****** ia2
		for( axis=0; axis<numberOfDimensions; axis++ )
		  rI(iaa(R,0),iaa(R,1),iaa(R,2),axis)=rr(R,axis);   // save coordinates
		interpolates(R)=TRUE;
	      }
	    }
	    else if( numberOfDimensions==3 )
	    {
	      where( rr(R,0)>rBound(Start,0,g2) && rr(R,0)<rBound(End,0,g2) &&
		     rr(R,1)>rBound(Start,1,g2) && rr(R,1)<rBound(End,1,g2) &&
		     rr(R,2)>rBound(Start,2,g2) && rr(R,2)<rBound(End,2,g2) )
	      {
		inverseGrid(iaa(R,0),iaa(R,1),iaa(R,2)) = g2;   // can interpolate from g2  // ****** ia2
		for( axis=0; axis<numberOfDimensions; axis++ )
		  rI(iaa(R,0),iaa(R,1),iaa(R,2),axis)=rr(R,axis);   // save coordinates
		interpolates(R)=TRUE;
	      }
	    }
	    else 
	    {
	      where( rr(R,0)>rBound(Start,0,g2) && rr(R,0)<rBound(End,0,g2) )
	      {
		inverseGrid(iaa(R,0),iaa(R,1),iaa(R,2)) = g2;   // can interpolate from g2  // ****** ia2
		for( axis=0; axis<numberOfDimensions; axis++ )
		  rI(iaa(R,0),iaa(R,1),iaa(R,2),axis)=rr(R,axis);   // save coordinates
		interpolates(R)=TRUE;
	      }
	    }
	  }
	  else // no points to check
	  {
	    rr.redim(0);
	  }
	  
	  // now re-check interpolation conditions
          #ifdef USE_PPP
            checkCanInterpolate(cg0,cg.gridNumber(grid),cg.gridNumber(g2),rr,interpolates,useBackupRulesArray);
          #else
	    cg0.rcData->canInterpolate(cg.gridNumber(grid),cg.gridNumber(g2), rr, interpolates, useBackupRulesArray,
		  		     checkForOneSided );
          #endif
	  if( numberOfPointsLeftToCheck>0 )
	    interpolatesArray(ib(R))=interpolates(R);
	}
      } // for g2
	
      // Count the number of points that could not interpolate *wdh* 110621
      int numToCheck=0;
      for( int i=iStart; i<=iEnd; i++ )
      {
	if( !interpolatesArray(i) )
          numToCheck++;
      }
      int totalNumToCheck=ParallelUtility::getSum(numToCheck);
      
  // if( true && numberOfBaseGrids>7 ) // TEMP
  // {
  //   int grid=7, i1=170,i2=52,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Classify:Before lastChance: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }
  // if( true && numberOfBaseGrids>9 ) // TEMP
  // {
  //   int grid=9, i1=158,i2=43,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Classify:Before lastChance: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }

      if( true && totalNumToCheck>0 )
      {
        // --- call the last chance interpolation to see if we can interpolate with backup rules ---

	IntegerArray ialc, isOk, interpolateslc;  // "lc" = last chance
        if( numToCheck>0 )
	{
          ialc.redim(numToCheck,3); isOk.redim(numToCheck); interpolateslc.redim(numToCheck);
          interpolateslc=false;
	}
        int j=0;
	for( int i=iStart; i<=iEnd; i++ )
	{
	  if( !interpolatesArray(i) )
	  {
            for( int axis=0; axis<3; axis++ )
              ialc(j,axis)=ia(i+i0,axis); 
	    j++;
    	  }
	}
        assert( j==numToCheck );
        int lastChanceOption=0; // -1 : do not remove un-needed at this stage
	lastChanceInterpolation( cg,cg0,grid,ialc,isOk,interpolateslc,numberOfInvalidPoints,invalidPoint,false,
                                 false,true,lastChanceOption);

        int k=0; // counts the number of points that could still not be interpolated
        for( int j=0; j<numToCheck; j++ )
	{
	  if( !interpolateslc(j) )
	  {
	    for( int axis=0; axis<3; axis++ )
              ialc(k,axis)=ialc(j,axis);  // compress this array
            k++;
	  }
	}
        numToCheck=k;
        totalNumToCheck=ParallelUtility::getSum(numToCheck);
        numberOfProperInterpolationErrors+=numToCheck;  // is this correct ? or use total over all processors?
	if( totalNumToCheck>0 )
	{
          // the algorithm failed! redo lastChance and output diagnostics
	  if( numToCheck>0 )
	  {
	    ialc.resize(numToCheck,3); isOk.resize(numToCheck); interpolateslc.resize(numToCheck);
	    interpolateslc=false;  
	  }
	  
          lastChanceInterpolation( cg,cg0,grid,ialc,isOk,interpolateslc,numberOfInvalidPoints,invalidPoint,true,false,false );

	}

      }
      else if( totalNumToCheck>0 )
      {
	// Old way -- this is wrong in parallel --- *wdh* 110621
	IntegerArray ialc(1,3), isOk(1,3);
	for( int i=iStart; i<=iEnd; i++ )
	{
	  if( !interpolatesArray(i) )
	  {
	    ialc(0,0)=ia(i+i0,0); ialc(0,1)=ia(i+i0,1); ialc(0,2)=ia(i+i0,2); 
	    
	    interpolates(0)=FALSE;

	    // This point could not be interpolated. Let's go back and try again. The problem could be
	    // one of the following:
	    //   1. A point that can no longer interpolate but that can be used as a discretization point
	    //   2. An interior point of a grid that is deemed inside the grid by hole-cutting
	    //      but outside the grid by the inverseMap -- in this case declare the point to be inside.
          
	    lastChanceInterpolation(cg,cg0,grid,ialc,isOk,interpolates,numberOfInvalidPoints,invalidPoint,false);
	    if( !interpolates(0) )
	    {
	      // the algorithm failed! redo lastChance and output diagnostics
	      lastChanceInterpolation(cg,cg0,grid,ialc,isOk,interpolates,numberOfInvalidPoints,invalidPoint,true,false,false);
	      numberOfProperInterpolationErrors++;
	    }
	  }
	}  // for int i
      }
      
  // if( true && numberOfBaseGrids>7 ) // TEMP
  // {
  //   int grid=7, i1=170,i2=52,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("proper:After lastChance: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }
  // if( true && numberOfBaseGrids>9 ) // TEMP
  // {
  //   int grid=9, i1=158,i2=43,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("proper:After lastChance: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }

 
      timeForFixingProperInterpolation+=getCPU()-time2;
    }

    Overture::checkMemoryUsage(sPrintF("classifyPoints (proper, grid=%i)",grid));

  }

  
  time=getCPU();
  if( info & 2 ) 
  {
    printF(" time to compute proper interpolation.....................%e (total=%e)\n",time-time0,time-totalTime);
    printF("      (includes time for canInterpolate...................%e )\n",timeForCanInterpolate);
    printF("      (includes time for fix up...........................%e )\n",timeForFixingProperInterpolation);
  }
  timeProperInterpolation=time-time0;
  if( info & 2 )
    Overture::printMemoryUsage("classifyPoints (after proper)");

//   if( true ) // 2012/06/18 --------------------------- TEST ------------------------------------------
//   {
//     delete [] iInterp; iInterp=NULL;
//     delete [] backupValues; backupValues=NULL;
//     return 0;
    
//   }


  // if( true && numberOfBaseGrids>7 ) // TEMP
  // {
  //   int grid=7, i1=170,i2=52,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("proper:After proper: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }
  // if( true && numberOfBaseGrids>9 ) // TEMP
  // {
  //   int grid=9, i1=158,i2=43,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("proper:After proper: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }


  #ifdef USE_PPP

    if( numberOfProperInterpolationErrors>0 ) 
      printf("classify:proper:ERROR: myid=%i numberOfProperInterpolationErrors=%i\n",
            myid,numberOfProperInterpolationErrors);
    numberOfProperInterpolationErrors=ParallelUtility::getMaxValue(numberOfProperInterpolationErrors);

    // do this for now: (some cases get fixed later)
    if( true ) 
    {
      if( numberOfProperInterpolationErrors>0 ) 
	printF(" +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
	       "classify:There were errors finding proper interpolation. This is a parallel run and \n"
	       " I am going to continue anyway for now. The problem may be resolved later...\n"
	       " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
  
      numberOfProperInterpolationErrors=0; // *********
    }
  #endif

  if( numberOfProperInterpolationErrors>0 )
  {
    return numberOfProperInterpolationErrors;
  }
  if( !ok ) // for testing ...
    return 1;

  time0=time;
  if( Ogen::debug & 1 )
  {
    generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
    plot( "After marking (proper) interpolation",cg);
  }
//   if( FALSE && (debug & 4 && info & 1) )
//   {
//     printf("Checking validity of the overlapping grid after  marking (proper) interpolation...\n");
//     generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
//     int numberOfErrors=checkOverlappingGrid(cg);
//     printf("... numberOfErrors=%i \n",numberOfErrors);
//   }
  if( debug & 8 )
  {
    for( grid=0; grid<numberOfBaseGrids; grid++ )
      displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after proper",grid),logFile);
  }

  if( minimizeTheOverlap )
  {
    // =================================================================================
    // now try to interpolate discretization points from grids with a higher priority.
    // =================================================================================

    interpolateAll( cg,numberOfInterpolationPoints,cg0 ); 
  
    time=getCPU();
    if( info & 2 ) 
      printF(" time to compute all interpolation points.................%e (total=%e)\n",time-time0,time-totalTime);
    timeAllInterpolation=time-time0;
    if( info & 2 )
      Overture::printMemoryUsage("classifyPoints (after all)");

    time0=time;
    if( Ogen::debug & 1 )
    {
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
      plot( "After marking all interpolation",cg);
    
    }

    if( debug & 8 )
    {
      for( grid=0; grid<numberOfBaseGrids; grid++ )
        displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after all interpolation",grid),logFile);
    }
//     if( FALSE && (debug & 4 && info & 1) )
//     {
//       printf("Checking validity of the overlapping grid after interpolateAll...\n");
//       generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
//       int numberOfErrors=checkOverlappingGrid(cg);
//       printf("... numberOfErrors=%i \n",numberOfErrors);
//     }

    if( improveQualityOfInterpolation )
    {
/* ----
   for( grid=0; grid<numberOfBaseGrids; grid++ )
   {
   // mark points on higher priority grids needed by lower priority grids
   markPointsReallyNeededForInterpolation( cg );
   }
   ---- */

      for( grid=0; grid<numberOfBaseGrids; grid++ )
      {
	if( cg[grid].isRectangular() )
	{
	  // *** fix this Bill! ***
	  printf("*** WARNING *** improveQuality: creating the vertex array"
		 " for a rectangular grid\n");
	  cg[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter);
	}
      }
      

    // try to improve the quality of the interpolation
      if( Ogen::debug & 1 )
      {
	generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
	plot( sPrintF(buff,"Before improve quality\n"),cg );
      }
      RealArray removedPointBound(2,3,numberOfBaseGrids);
      removedPointBound(0,nullRange,nullRange)=1.e6;
      removedPointBound(1,nullRange,nullRange)=-1.e6;
    
//       for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
//         cg[grid].mask().periodicUpdate();
       
      for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
	improveQuality(cg,grid,removedPointBound);

      if( Ogen::debug & 1 )
      {
	generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
	plot( sPrintF(buff,"After improve quality\n"),cg );
      }
    // display(removedPointBound,"removedPointBound");
    
      updateCanInterpolate( cg,cg0,removedPointBound );
      time=getCPU();
      if( info & 2 ) 
	printF(" time to improve quality of interpolation.................%e (total=%e)\n",time-time0,time-totalTime);
      timeImproveQuality=time-time0;
      time0=time;
    
    
      if( Ogen::debug & 1 )
      {
	generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
	plot( sPrintF(buff,"After updateCanInterpolate\n"),cg );
      }
      
    }


    // if backup rules were used at any points we need to change some parameters to
    // indicate that the interpolation is now implicit (if it wasn't already) and the
    // overlap should be decreased (so the mark needed points works)
//     if( max(cg.mayBackupInterpolate) > 0 )  // *************************** is this needed anymore ? ******
//     {
//       psp.set(GI_PLOT_BACKUP_INTERPOLATION_POINTS,TRUE);
    
//       for( grid=0; grid<numberOfBaseGrids; grid++ )
//       {
// 	for( grid2=0; grid2<numberOfBaseGrids; grid2++ )
// 	{
// 	  if( cg.mayBackupInterpolate(grid,grid2,0) )
// 	  {
// 	    if( !cg0.interpolationIsImplicit(grid,grid2,level) )
// 	    {
// 	      printf("WARNING: Interpolation is now implicit betweens grid %i and grid %i  (level=%i)\n",
// 		     grid,grid2,level);
// 	      cg0.interpolationIsImplicit(grid,grid2,level)=TRUE;
// 	      cg.interpolationIsImplicit(grid,grid2,0)=TRUE;
// 	    }
// 	  }
// 	}
//       }
//       updateParameters(cg0,level);  // update overlap etc.
//       updateParameters(cg);  // update overlap etc.
//     }
  

    if( debug & 8 )
    {
      for( grid=0; grid<numberOfBaseGrids; grid++ )
	displayMask(cg[grid].mask(),sPrintF("Mask for grid %i before remove redundant",grid),logFile);
    }

    if( info & 4 ) printF("*** remove redundant interpolation points\n");
  
  // This step was added 980614 -- needed by the ellipsoid grid
  //             1---1------------------- grid=2
  //          0---2---2------------------ grid=1
  //   ---------1--2--3-----------------  grid=0
  //   Point 0 needs points 1,2 on grid 0, 
  //   Point 1 on grid=0 needs point 0 *** this needs to be imposed **** or else point 2 on grid=0 will be removed
    if( numberOfBaseGrids>1 )
    {
      // umark interpolation points on the highest priority grid. The new hole cutting algorithm
      // will mark too many points.
      unmarkInterpolationPoints( cg );
      if( Ogen::debug & 4 )
      {
	generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
	plot( sPrintF(buff,"After unmarkInterpolationPoints\n"),cg );
      }
      if( debug & 8 )
      {
	for( grid=0; grid<numberOfBaseGrids; grid++ )
	  displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after unmarkInterpolationPoints",grid),logFile);
      }
      for( grid=0; grid<numberOfBaseGrids; grid++ )
      {
        // if( true )
	// {
        //   // markPointsNeeded checks if points can discretized -- needs neighbours -- 081001 ---
        //   cg[grid].mask().updateGhostBoundaries();
	// }
	// mark points on higher priority grids needed by lower priority grids
	markPointsNeededForInterpolation( cg,grid,+1 );
      }
      if( debug & 8 )
      {
	for( grid=0; grid<numberOfBaseGrids; grid++ )
	  displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after markPointsNeededForInterpolation",grid),logFile);
      }
    }

    for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
    {
      // first mark interp. points on boundaries that prefer to be discretized
      // we need this step as we found all possible interp points on boundaries
      unmarkBoundaryInterpolationPoints( cg, grid );
    }


    if( Ogen::debug & 8 )
    {
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
      plot( sPrintF(buff,"after unmarkBoundaryInterpolationPoints\n"),cg );
    }
  
    if( debug & 8 )
    {
      for( grid=0; grid<numberOfBaseGrids; grid++ )
	displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after unmarkBoundaryInterpolationPoints",grid),logFile);
    }

    for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
    {
      // mark points on lower grids needed for interpolation
      markPointsNeededForInterpolation( cg,grid,-1 );
    }
    if( debug & 8 )
    {
      for( grid=0; grid<numberOfBaseGrids; grid++ )
	displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after markPointsNeededForInterpolation on lower",grid),
                    logFile);
    }
    if( Ogen::debug & 8 )
    {
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );

      printF("Checking validity of the overlapping grid after markPointsNeededForInterpolation on lower...\n");
      int numberOfErrors=checkOverlappingGrid(cg);
      printF("... numberOfErrors=%i \n",numberOfErrors);

      plot( sPrintF(buff,"after markPointsNeededForInterpolation\n"),cg );
    }

    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
      cg[grid].mask().periodicUpdate();
      cg[grid].mask().updateGhostBoundaries();
      if( !classifyHolesForHybrid ) 
      {
	classifyRedundantPoints( cg,grid,level,cg0 );
	cg[grid].mask().periodicUpdate(); // wdh 980903
	cg[grid].mask().updateGhostBoundaries();
      }

      if( Ogen::debug & 8 )
      {
	generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
	printF("Checking validity of the overlapping grid after classifyRedundant grid=%i...\n",grid);
	int numberOfErrors=checkOverlappingGrid(cg);
	printF("... numberOfErrors=%i \n",numberOfErrors);

	plot( sPrintF(buff,"After classifyRedundant grid=%i...",grid),cg );
      }

      markPointsNeededForInterpolation( cg,grid,+1 );
      if( Ogen::debug & 8  )
      {
	generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
	printF("Checking validity of the overlapping grid after markPointsNeededForInterpolation grid=%i...\n",grid);
	int numberOfErrors=checkOverlappingGrid(cg);
	printF("... numberOfErrors=%i \n",numberOfErrors);

	plot( sPrintF(buff,"After markPointsNeededForInterpolation for grid %i",grid),cg);
      }

    }
  
    if( debug & 8 )
    {
      for( grid=0; grid<numberOfBaseGrids; grid++ )
	displayMask(cg[grid].mask(),sPrintF("Mask for grid %i after classifyRedundant",grid),logFile);
    }
    
    if( debug & 8 )
    {
      for( grid=0; grid<numberOfBaseGrids; grid++ )
      {
	cg[grid].mask().periodicUpdate();
	cg[grid].mask().updateGhostBoundaries();
      }
      
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
      printF("Checking validity of the overlapping grid after classifyRedundant ...\n");
      int numberOfErrors=checkOverlappingGrid(cg);
      printF("... numberOfErrors=%i \n",numberOfErrors);

      plot( sPrintF(buff,"after classifyRedundant (all)\n"),cg );
    }

    time=getCPU();
    if( info & 2 ) 
      printF(" time to remove redundant points..........................%e (total=%e)\n",time-time0,time-totalTime);
    timeRemoveRedundant=time-time0;
    time0=time;
    if( info & 2 )
      Overture::printMemoryUsage("classifyPoints (remove redundant)");    
  }
  else
  {
    // if we maximize the overlap we need to unmark any extra interpolation points that were marked
    unmarkInterpolationPoints( cg,TRUE );

    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
      cg[grid].mask().periodicUpdate();
      cg[grid].mask().updateGhostBoundaries();	
      if (!classifyHolesForHybrid)
      {
	classifyRedundantPoints( cg,grid,level,cg0 );
	cg[grid].mask().periodicUpdate();
        cg[grid].mask().updateGhostBoundaries();
      }
    }
    
    if( Ogen::debug & 4 )
    {
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
      plot( sPrintF(buff,"After unmarkInterpolationPoints\n"),cg );
    }

  }
  
  if( improveQualityOfInterpolation )
  {
    // we may need to redo the mixed boundaries since the improve quality could have removed
    // the interpolation points at mixed boundaries
    for( int n=0; n<numberOfMixedBoundaries; n++ )
    {
      interpolateMixedBoundary(cg,n);
    }
    if( Ogen::debug & 1 )
    {
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
      plot( sPrintF(buff,"After re-interpolate mixed boundary\n"),cg );
    }
 
  }
  
  // fill in the interpolation arrays  

  generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );
  
  // In parallel we need to be consistent across all processors  *wdh* 2012/06/14
  IntegerArray backupValuesUsedOnSomeProcessors(numberOfBaseGrids);
  ParallelUtility::getMinValues(&backupValuesUsed(0),&backupValuesUsedOnSomeProcessors(0),numberOfBaseGrids);

  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    // *wdh* 2012/06/14 if( backupValuesUsed(grid)<0 )
    if( backupValuesUsedOnSomeProcessors(grid)<0 )
    {
      // If backup values of implicit interpolation were used then we need
      // to change the type of interpolation.
      for( int grid2=0; grid2<numberOfBaseGrids; grid2++ )
      {
	if( cg.interpolationStartEndIndex(2,grid,grid2)>=0 )
	{
	  cg0.interpolationIsImplicit(grid,grid2,level)=true;  // mark all interpolee grids for now **** fix ***
	  cg.interpolationIsImplicit(grid,grid2,0)=true;
	}
      }
    }
  }

  // --- For each used point on the boundary, mark its ghost points. ---
  markMaskAtGhost( cg );
  

//   for( grid=0; grid<numberOfBaseGrids; grid++ )
//   {
//     //
//     // For each used point on the boundary, mark its ghost points.
//     //
//     MappedGrid & g = cg[grid];
//     intArray & maskd = g.mask();

//     GET_LOCAL(int,maskd,mask);
//     int * maskp = mask.Array_Descriptor.Array_View_Pointer2;
//     const int maskDim0=mask.getRawDataSize(0);
//     const int maskDim1=mask.getRawDataSize(1);
//     #undef MASK
//     #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

//     // g.mask().display("Mask before marking ghost values");
//     if( debug & 8 )
//       displayMask(maskd,sPrintF("Mask for grid %i before marking ghost values",grid),logFile);



//     Index I[3], &I1=I[0], &I2=I[1], &I3=I[2], J[3], &J1=J[0], &J2=J[1], &J3=J[2];
//     int side,axis;
//     for( axis=0; axis<3; axis++ )
//     {
//       for( side=Start; side<=End; side++ )
//       {
//         getBoundaryIndex(g.dimension(),side,axis,I1,I2,I3);
//         // *wdh* 980626 I[axis]=g.indexRange(side,axis);
//         I[axis]=g.gridIndexRange(side,axis);

// 	bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);  
// 	if( !ok ) continue;  // is this correct ?
	
//         getGhostIndex(g.dimension(),side,axis,J1,J2,J3);

// 	const Integer pm1 = 2 * side - 1;
// 	// if( g.boundaryCondition(side,axis)!=0 ) // do not change periodic edges, these may be needed for interp.
//         int lastGhost= g.extendedIndexRange(side,axis);
	  
// 	if( g.boundaryFlag(side,axis)==MappedGrid::physicalBoundary )
// 	{
// 	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
// 	  { 
// 	    J[axis] = k; 
//             bool ok= ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
//             if( ok )
// 	    {
//               int j1,j2,j3;
// 	      FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3)
// 	      {
// 		if( MASK(i1,i2,i3) )
// 		{
// 		  MASK(j1,j2,j3) = MappedGrid::ISghostPoint;
// 		}
// 		else
// 		{
// 		  MASK(j1,j2,j3) = 0;
// 		}
// 	      }
// 	    }
	    
// 	  }

//           // *wdh* 991106 : for non-conforming grids, set ghost point mask to include ISinteriorBoundaryPoint
//           getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);
//           getGhostIndex(g.gridIndexRange(),side,axis,J1,J2,J3);

// 	  bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
// 	  ok=ok && ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
// 	  if( ok )
// 	  {
// 	    where( mask(I1,I2,I3) & MappedGrid::ISinteriorBoundaryPoint ) 
// 	    {
// 	      mask(J1,J2,J3) = MappedGrid::ISghostPoint | MappedGrid::ISinteriorBoundaryPoint;
// 	    }
// 	  }
	  

// 	}
// 	else if( g.boundaryCondition()(side,axis)==0 )
// 	{
// 	  // set ghost lines outside interpolation edges to zero.
// 	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
// 	  { 
// 	    J[axis] = k; 
//             ok=ok && ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
//   	    if( ok )
// 	      mask(J1,J2,J3) = 0;
// 	  }
// 	}
// 	else if( g.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
// 	{
//           lastGhost=g.gridIndexRange(side,axis);
// 	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
// 	  { 
// 	    J[axis] = k; 
//             ok=ok && ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
//             if( !ok ) continue;

// 	    where( mask(I1,I2,I3) & MappedGrid::ISinteriorBoundaryPoint  && mask(J1,J2,J3)>0 ) 
// 	    {
//               //  we need to mark the ghost line too for the BC mask which looks at the first
//               // ghost line for ISinteriorBoundaryPoint.
//               mask(J1,J2,J3) = MappedGrid::ISghostPoint | MappedGrid::ISinteriorBoundaryPoint ;
// 	    }
// 	    elsewhere( mask(I1,I2,I3) && mask(J1,J2,J3)>0  )
// 	    {
// 	      mask(J1,J2,J3) = MappedGrid::ISghostPoint;
// 	    }
//             elsewhere(mask(I1,I2,I3)==0 )
// 	    {
// 	      mask(J1,J2,J3) = 0;
// 	    }
// 	  }
// 	}

//       }
//     }

//     g.mask().periodicUpdate();
//     g.mask().updateGhostBoundaries();
    
//     if(  debug& 8 )
//       displayMask(g.mask(),sPrintF("Mask for grid %i afer marking ghost values",grid),logFile);
//   }

  delete [] iInterp; iInterp=NULL;
  delete [] backupValues; backupValues=NULL;
  
  return 0;
}


//\begin{>ogenInclude.tex}{\subsubsection{markGhost}}
int Ogen::
markMaskAtGhost( CompositeGrid & cg )
// =================================================================================================
// /Description:
//     Mark the mask at ghost points to match the values of the mask on the boundary.
// /cg (input/output):
// /author: wdh 090804 : make this a separate routine that can be called from movingUpdate too.
//\end{ogenInclude.tex}
// =================================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  int i1,i2,i3;
  for( int grid=0; grid<numberOfBaseGrids; grid++ )
  {
    //
    // For each used point on the boundary, mark its ghost points.
    //
    MappedGrid & g = cg[grid];
    intArray & maskd = g.mask();

    GET_LOCAL(int,maskd,mask);
    int * maskp = mask.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=mask.getRawDataSize(0);
    const int maskDim1=mask.getRawDataSize(1);
#undef MASK
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

    // g.mask().display("Mask before marking ghost values");
    if( debug & 8 )
      displayMask(maskd,sPrintF("Ogen:markMaskAtGhost: Mask for grid %i before marking ghost values",grid),logFile);



    Index I[3], &I1=I[0], &I2=I[1], &I3=I[2], J[3], &J1=J[0], &J2=J[1], &J3=J[2];
    int side,axis;
    for( axis=0; axis<3; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
        getBoundaryIndex(g.dimension(),side,axis,I1,I2,I3);
        // *wdh* 980626 I[axis]=g.indexRange(side,axis);
        I[axis]=g.gridIndexRange(side,axis);

	bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);  
	if( !ok ) continue;  // is this correct ?
	
        getGhostIndex(g.dimension(),side,axis,J1,J2,J3);

	const Integer pm1 = 2 * side - 1;
	// if( g.boundaryCondition(side,axis)!=0 ) // do not change periodic edges, these may be needed for interp.
        int lastGhost= g.extendedIndexRange(side,axis);
	  
	if( g.boundaryFlag(side,axis)==MappedGrid::physicalBoundary )
	{
	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
	  { 
	    J[axis] = k; 
            bool ok= ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
            if( ok )
	    {
              int j1,j2,j3;
	      FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3)
	      {
		if( MASK(i1,i2,i3) )
		{
		  MASK(j1,j2,j3) = MappedGrid::ISghostPoint;
		}
		else
		{
		  MASK(j1,j2,j3) = 0;
		}
	      }
	    }
	    
	  }

          // *wdh* 991106 : for non-conforming grids, set ghost point mask to include ISinteriorBoundaryPoint
          getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);
          getGhostIndex(g.gridIndexRange(),side,axis,J1,J2,J3);

	  bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
	  ok=ok && ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
	  if( ok )
	  {
	    where( mask(I1,I2,I3) & MappedGrid::ISinteriorBoundaryPoint ) 
	    {
	      mask(J1,J2,J3) = MappedGrid::ISghostPoint | MappedGrid::ISinteriorBoundaryPoint;
	    }
	  }
	  

	}
	else if( g.boundaryCondition()(side,axis)==0 )
	{
	  // set ghost lines outside interpolation edges to zero.
	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
	  { 
	    J[axis] = k; 
            ok=ok && ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
  	    if( ok )
	      mask(J1,J2,J3) = 0;
	  }
	}
	else if( g.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	{
          lastGhost=g.gridIndexRange(side,axis);
	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
	  { 
	    J[axis] = k; 
            ok=ok && ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3);
            if( !ok ) continue;

	    where( mask(I1,I2,I3) & MappedGrid::ISinteriorBoundaryPoint  && mask(J1,J2,J3)>0 ) 
	    {
              //  we need to mark the ghost line too for the BC mask which looks at the first
              // ghost line for ISinteriorBoundaryPoint.
              mask(J1,J2,J3) = MappedGrid::ISghostPoint | MappedGrid::ISinteriorBoundaryPoint ;
	    }
	    elsewhere( mask(I1,I2,I3) && mask(J1,J2,J3)>0  )
	    {
	      mask(J1,J2,J3) = MappedGrid::ISghostPoint;
	    }
            elsewhere(mask(I1,I2,I3)==0 )
	    {
	      mask(J1,J2,J3) = 0;
	    }
	  }
	}

      }
    }

    g.mask().periodicUpdate();
    g.mask().updateGhostBoundaries();
    
    if(  debug& 8 )
      displayMask(g.mask(),sPrintF("Ogen:markMaskAtGhost:Mask for grid %i afer marking ghost values",grid),logFile);
  }

  return 0;
}


// int Ogen::
// markPartiallyPeriodicBoundaries( CompositeGrid & cg,
//                                  intArray *iInterp )
// // ================================================================================
// // /Description: ****NOT USED ANYMORE ****
// //   Mark C-grid interpolation points. These are ghost points on the c-grid boundary
// //   that interpolate from the first line of the same boundary (at the other end).
// // ==============================================================================
// {
//   if( cg.numberOfDimensions()!=2 )
//     return 0;
  
//   const int numberOfBaseGrids=cg.numberOfBaseGrids();

//   Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
//   int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
//   int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
//   is1=is2=is3=0;
//   int grid;
//   for( grid=0; grid<numberOfBaseGrids; grid++ )
//   {
//     MappedGrid & g = cg[grid];
//     Mapping & map = g.mapping().getMapping();
//     intArray & mask = g.mask();
//     intArray & inverseGrid = cg.inverseGrid[grid];
//     realArray & rI = cg.inverseCoordinates[grid];
    
//     int numberOfPartiallyPeriodicPoints=0;
    
//     int side,axis;
//     for( axis=0; axis<cg.numberOfDimensions(); axis++ )
//     {
//       for( side=Start; side<=End; side++ )
//       {
//         if( map.getTopology(side,axis)==Mapping::topologyIsPartiallyPeriodic )
// 	{
//           is[axis]=2*side-1;
//           const int axisp1 = (axis+1) % cg.numberOfDimensions();
//           intArray & topo = map.topologyMask();

//           getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3); // what about ghost points!

// 	  const int I1Base=I1.getBase(), I1Bound=I1.getBound();
// 	  const int I2Base=I2.getBase(), I2Bound=I2.getBound();
// 	  const int I3Base=I3.getBase(), I3Bound=I3.getBound();
// 	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
// 	  {
// 	    for( i2=I2.getBase(); i2<=I2Bound; i2++ )
// 	    {
// 	      for( i1=I1Base; i1<=I1Bound; i1++ )
// 	      {
// 		if( mask(i1,i2,i3)!=0 && topo(i1,i2,i3)!=0 )  // *** is this right ??
// 		{
//                   mask(i1,i2,i3)=MappedGrid::ISinteriorBoundaryPoint;  // ????????????

//                   mask(i1+is1,i2+is2,i3+is3)=MappedGrid::ISinterpolationPoint;
// 		  inverseGrid(i1+is1,i2+is2,i3+is3)=grid;
// 		  rI(i1+is1,i2+is2,i3+is3,axis)=g.gridSpacing(axis);  // we need to enforce this
//                   real rr=(iv[axisp1]-g.gridIndexRange(Start,axisp1))*g.gridSpacing(axisp1);
// 		  rI(i1+is1,i2+is2,i3+is3,axisp1)=1.-rr;

// 		  numberOfPartiallyPeriodicPoints++;
		  
// 		}
// 	      }
// 	    }
// 	  }

//           is[axis]=0;
// 	}
//       }
//     }
//     if( numberOfPartiallyPeriodicPoints>0 )
//     {
// // **** needed ??      ia.redim(ia.getLength(0)+numberOfPartiallyPeriodicPoints,3);
//     }
//   }
//   return 0;
// }


//\begin{>ogenInclude.tex}{\subsubsection{generateInterpolationArrays}}
int Ogen::
generateInterpolationArrays( CompositeGrid & cg, 
                             const IntegerArray & numberOfInterpolationPoints,
                             intSerialArray *iInterp )
// =================================================================================================
// /Description:
//  Fill in the interpolation arrays. Order the interpolation points by the interpoleeGrid
// /cg (input/output):
// /numberOfInterpolationPoints (input):
// /iInterp : 
//\end{ogenInclude.tex}
// =================================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfDimensions=cg.numberOfDimensions();
  

  // cg.numberOfInterpolationPoints must be correct or else cg.update() recomputes the data!
  // Thus we make temporary arrays to hold the data in
  intSerialArray *interpoleeGrid =           new intSerialArray [numberOfBaseGrids];
  intSerialArray *interpolationPoint=        new intSerialArray [numberOfBaseGrids];
  intSerialArray *interpoleeLocation=        new intSerialArray [numberOfBaseGrids];
  intSerialArray *variableInterpolationWidth=new intSerialArray [numberOfBaseGrids];
  realSerialArray *interpolationCoordinates=  new realSerialArray [numberOfBaseGrids];

  Index I1,I2,I3;
  Range R,Rx(0,cg.numberOfDimensions()-1);
  Range Rg=numberOfBaseGrids;

  IntegerArray ng(numberOfBaseGrids);
  IntegerArray gridStart(numberOfBaseGrids);

  cg.interpolationStartEndIndex=-1;
  
  // cg.numberOfInterpolationPoints.redim(numberOfBaseGrids);

  cg.numberOfInterpolationPoints=0;  // *wdh* 040311
  #ifdef USE_PPP
    cg->numberOfInterpolationPointsLocal.redim(cg.numberOfGrids());
    cg->numberOfInterpolationPointsLocal=0;
    IntegerArray & cgni = cg->numberOfInterpolationPointsLocal;
  #else
    IntegerArray & cgni = cg.numberOfInterpolationPoints;
  #endif

  
  int grid,grid2;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    intArray & maskd = c.mask();
    intArray & inverseGridd = cg.inverseGrid[grid];
    intSerialArray & ia = iInterp[grid];
    realArray & rId = cg.inverseCoordinates[grid];

    if( true ) maskd.updateGhostBoundaries();

    GET_LOCAL(int,maskd,mask);
    GET_LOCAL(int,inverseGridd,inverseGrid);
    GET_LOCAL(real,rId,rI);

    // The extendedRange includes an extra line outside of mixed boundaries.
    // Mixed boundary interp points will be: MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint
    getIndex(c.extendedRange(),I1,I2,I3); 
    
    bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);

    int i1,i2,i3, i=0;
    if( ok )
    {
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    if( mask(i1,i2,i3) & MappedGrid::ISinterpolationPoint )
	    {                               
	      ia(i,0)=i1;
	      ia(i,1)=i2;
	      ia(i,2)=i3;
	      i++;
	    }
	  }
	}
      }
    }
    
/* ----
    // Add in any points from mixed boundaries *wdh* 020705
    // Do this here because sometimes we get ghost points marked as interpolation pts (bjet) for some reason
    const IntegerArray & extendedRange = c.extendedRange();
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
        if( c.boundaryFlag(side,axis)==MappedGridData::mixedPhysicalInterpolationBoundary )
	{
          getBoundaryIndex( extendedRange,side,axis,I1,I2,I3);
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
          for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    if( mask(i1,i2,i3) & (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint) )
  	    {                               
	      ia(i,0)=i1;
	      ia(i,1)=i2;
	      ia(i,2)=i3;
	      i++;
	    }
	}
      }
    }
    ---- */    

    if( i>ia.getLength(0) )
    {
      printF("\n **** Ogen:generateInterpolationArrays::ERROR: i=%i > ia.getLength(0)=%i\n\n ",i,ia.getLength(0));
      aString gridFileName="gridFailed.hdf", gridName="gridFailed";
      printF("Saving the current grids in the file %s (option outputGridOnFailure=true)\n",(const char*)gridFileName);
      saveGridToAFile( cg,gridFileName,gridName );
      Overture::abort("error");
    }

    assert( i<=ia.getLength(0) );
    
    numberOfInterpolationPoints(grid)=i;
    cgni(grid)=i;
    if( cgni(grid)==0 )
      continue;
    interpoleeGrid[grid].redim(cgni(grid));
    interpolationPoint[grid].redim(cgni(grid),Rx);
    interpoleeLocation[grid].redim(cgni(grid),Rx);
    interpolationCoordinates[grid].redim(cgni(grid),Rx);
    variableInterpolationWidth[grid].redim(cgni(grid));

    intSerialArray & vWidth = variableInterpolationWidth[grid];

    int l=0;  // multigrid level

    // *wdh* 100201 vWidth=max(cg.interpolationWidth(Rx,grid,Rg,l));
    vWidth=-1;  // set this below
			      
    // fill in the ng array
    // ng(g2) = number of interpolation from grid that interpolate from g2
    //if( FALSE && numberOfComponentGrids<=2 )
    //{
    //  ng(grid)=0; ng((grid+1)%numberOfComponentGrids)=cgni(grid);
    //}
    //else
    // {
    ng=0;
    for( i=0; i<numberOfInterpolationPoints(grid); i++ )
    {
      grid2=inverseGrid(ia(i,0),ia(i,1),ia(i,2));
      if( grid2<0 || grid2>numberOfBaseGrids)
      {
	printf("ERROR: grid2<0 : grid=%i, grid2=%i, (i1,i2,i3)=(%i,%i,%i) mask=%i \n"
	       "       numberOfInterpolationPoints=%i \n",
	       grid,grid2,ia(i,0),ia(i,1),ia(i,2),mask(ia(i,0),ia(i,1),ia(i,2)),numberOfInterpolationPoints(grid));
	display(c.boundaryCondition(),"Here are the boundary conditions");
	  
	throw "error";
      }
      ng(grid2)++;
    }
    // }

    // count the number of implicit backup-interpolation points when backup rules are used
    IntegerArray ngi(numberOfBaseGrids);
    ngi=0;
    if( backupValuesUsed.getLength(0)>0 && backupValuesUsed(grid)<0 )
    {
      for( i=0; i<numberOfInterpolationPoints(grid); i++ )
      {
	if( backupValues[grid](ia(i,0),ia(i,1),ia(i,2))<0 )
	{
          grid2=inverseGrid(ia(i,0),ia(i,1),ia(i,2));
	  ngi(grid2)++;
	}
      }
      if( sum(ngi)>0 )
        display(ngi,sPrintF(buff,"backup values needed. Here is ngi for grid %i",grid));
      
    }
    

    R=Range(0,numberOfInterpolationPoints(grid)-1);
    realSerialArray & interpolationCoord = interpolationCoordinates[grid];
    if( numberOfInterpolationPoints(grid) > 0 )
    {
      gridStart(0)=0;
      for( grid2=1; grid2<numberOfBaseGrids; grid2++ )
	gridStart(grid2)=gridStart(grid2-1)+ng(grid2-1);
	
      
      for( grid2=0; grid2<numberOfBaseGrids; grid2++ )
      {

        if( ng(grid2)>0 )
	{
	  cg.interpolationStartEndIndex(0,grid,grid2)=gridStart(grid2);              // start value
	  cg.interpolationStartEndIndex(1,grid,grid2)=gridStart(grid2)+ng(grid2)-1;  // end value
	  if( cg.interpolationIsImplicit(grid,grid2,0) )
	    cg.interpolationStartEndIndex(2,grid,grid2)= cg.interpolationStartEndIndex(1,grid,grid2);
	  else if( ngi(grid2)>0 )
	    cg.interpolationStartEndIndex(2,grid,grid2)=gridStart(grid2)+ngi(grid2)-1; // end value for implicit pts.
	}
      }
      
      IntegerArray gridStartImplicit(numberOfBaseGrids);
      gridStartImplicit=gridStart;
      
      gridStart+=ngi;  // explicit points start here

      // order all interpolation points by interpolee grid,  
      // **** put implicit backup-points first so that when we iterate to solve the interpolation equations
      // we can iterate on just the implicit points.

      int varWidth;
      for( int i=0; i<numberOfInterpolationPoints(grid); i++ )
      {
	grid2=inverseGrid(ia(i,0),ia(i,1),ia(i,2));
	int j=gridStart(grid2);
	
        // move below *wdh* 100318 since j may be out of bounds here if there are backup interp:
        // vWidth(j)=cg.interpolationWidth(0,grid,grid2,l);   // *wdh* 100210 -- set vWidth to default for these grids

	if( mask(ia(i,0),ia(i,1),ia(i,2)) & MappedGrid::USESbackupRules ) 
	{
          if( true )
	  {
	    assert( backupValuesUsed(grid)!=0 );
            int i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);
            const intSerialArray & bv = backupValues[grid];
	    if( i1<bv.getBase(0) || i1>bv.getBound(0) ||
                i2<bv.getBase(1) || i2>bv.getBound(1) ||
                i3<bv.getBase(2) || i3>bv.getBound(2) )
	    {
	      printf("getInterpArrays:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) bv:[%i,%i][%i,%i][%i,%i]\n",
		     grid,i1,i2,i3,bv.getBase(0),bv.getBound(0),
		     bv.getBase(1),bv.getBound(1),
		     bv.getBase(2),bv.getBound(2));
	      Overture::abort("error");
	    }
	  }
	  
	  varWidth=backupValues[grid](ia(i,0),ia(i,1),ia(i,2));
          
          if( varWidth<0 )   // this point is implicit interpolation
	  {
            j=gridStartImplicit(grid2);
	    gridStartImplicit(grid2)++;
            cg.interpolationIsImplicit(grid,grid2,l)=true;  // *wdh* 000826
	    
	  }
          else
	  {
    	    gridStart(grid2)++;
	  }
	  if( j>=vWidth.getLength(0) )
	  {
	    printf("ERROR: j=%i > vWidth.getLength(0)=%i\n", j,vWidth.getLength(0));
	    display(cgni,"(numberOfInterpolationPoints");
	    display(gridStart,"gridStart");
	    throw "error";
	  }
	  vWidth(j)=abs(varWidth);

	  // printf("Point (%i,%i,%i) on grid %i has interp width %i\n",ia(i,0),ia(i,1),ia(i,2),grid,vWidth(j));
	}
	else
	{
          vWidth(j)=cg.interpolationWidth(0,grid,grid2,l);   // *wdh* 100318 -- set vWidth to default for these grids
  	  gridStart(grid2)++;
	}
	interpoleeGrid[grid](j)=grid2;
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
  	  interpolationPoint[grid](j,axis)=ia(i,axis);
	  interpolationCoord(j,axis)=rI(ia(i,0),ia(i,1),ia(i,2),axis);
	}
	
      }
      // }
    } //  if( numberOfInterpolationPoints(grid) > 0 )
    

    // Fill in the interpoleeLocation -- lower left corner of the stencil
    intSerialArray & interpoleeLoc = interpoleeLocation[grid];
    
   // ** could vectorize this I think if we put gridSpacing into an array: spacing(grid,axis)   

    for( i=0; i<numberOfInterpolationPoints(grid); i++ )
    {
      int grid2=interpoleeGrid[grid](i);
      // *wdh8 000115 const IntegerArray & interpolationWidth = cg.interpolationWidth(Rx,grid,grid2,l);

      const int width = vWidth(i);

      MappedGrid & g2 = cg[grid2];
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	// Get the lower-left corner of the interpolation cube.
	int intLoc=int(floor(interpolationCoord(i,axis)/g2.gridSpacing()(axis) + g2.indexRange()(0,axis) -
			 .5 * width + (g2.isCellCentered()(axis) ? .5 : 1.)));
	if (!g2.isPeriodic()(axis)) 
	{
	  if( (intLoc < g2.extendedIndexRange()(0,axis)) && (g2.boundaryCondition()(Start,axis)>0) )
	  {
            //                        Point is close to a BC side.
            //                        One-sided interpolation used.
	    intLoc = g2.extendedIndexRange()(0,axis);
	  }
	  if( (intLoc + width - 1 > g2.extendedIndexRange()(1,axis))
              && (g2.boundaryCondition()(End,axis)>0) )
	  {
            //                        Point is close to a BC side.
            //                        One-sided interpolation used.
	    intLoc = g2.extendedIndexRange()(1,axis) - width + 1;
	  }
	} // end if
	interpoleeLoc(i,axis) = intLoc;
      } // end for_1
      
    }

    if( info & 32 )
    {
      ::display(interpolationCoordinates[grid],sPrintF(buff,"interpolationCoordinates[%i]",grid),"%5.2f ");
      ::display(interpolationPoint[grid],sPrintF(buff,"interpolationPoint[%i]",grid));
      ::display(interpoleeGrid[grid],sPrintF(buff,"interpoleeGrid[%i]",grid));
      ::display(interpoleeLocation[grid],sPrintF(buff,"interpoleeLocation[%i]",grid));
    }
    
  } // end for( grid )

 #ifdef USE_PPP
  // In parallel we need to make sure that the cg.interpolationIsImplicit is consistent across all processors
  // *wdh* 2012/06/14
  const int l=0; // MG level
  int numVal=0;
  int *ival = new int [numberOfBaseGrids*numberOfBaseGrids];
  for( int grid=0; grid<numberOfBaseGrids; grid++ )for( int grid2=0; grid2<numberOfBaseGrids; grid2++ )
  {
    ival[numVal++]=cg.interpolationIsImplicit(grid,grid2,l);
  }
  
  ParallelUtility::getMaxValues(ival,ival,numVal);

  numVal=0;
  for( int grid=0; grid<numberOfBaseGrids; grid++ )for( int grid2=0; grid2<numberOfBaseGrids; grid2++ )
  {
    cg.interpolationIsImplicit(grid,grid2,l) = ival[numVal++];
  }
  delete [] ival;
 #endif  



  // now we know how many interpolation points there are so we can create the arrays in the cg.
  #ifndef USE_PPP

    // serial version 

  cg.update(
    CompositeGrid::THEinterpolationPoint       |
    CompositeGrid::THEinterpoleeGrid           |
    CompositeGrid::THEinterpoleeLocation       |
    CompositeGrid::THEinterpolationCoordinates,
    CompositeGrid::COMPUTEnothing);

  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    // cg.numberOfInterpolationPoints(grid)=numberOfInterpolationPoints(grid);

    cg.interpoleeGrid[grid]=interpoleeGrid[grid];
    cg.variableInterpolationWidth[grid]=variableInterpolationWidth[grid]; // ** max(cg.interpolationWidth(Rx,Rg,Rg,0));

    cg.interpolationPoint[grid]=interpolationPoint[grid];
    cg.interpoleeLocation[grid]=interpoleeLocation[grid];
    cg.interpolationCoordinates[grid]=interpolationCoordinates[grid];
  }
  #else

    // parallel case -- save interp data in local array  

  // dimension new serial array interpolation data arrays
#define adjustSizeMacro(x,n)\
  while( x.getLength() < n )\
    x.addElement();\
  while( x.getLength() > n )\
    x.deleteElement()

  // tell the CompositeGrid that we are storing the interp data in a local serial form:
  cg->localInterpolationDataState=CompositeGridData::localInterpolationDataForAll;

  const int numberOfGrids=cg.numberOfGrids();
  
  adjustSizeMacro(cg->interpolationPointLocal,numberOfGrids);
  adjustSizeMacro(cg->interpoleeGridLocal,numberOfGrids);
  adjustSizeMacro(cg->variableInterpolationWidthLocal,numberOfGrids);
  adjustSizeMacro(cg->interpoleeLocationLocal,numberOfGrids);
  adjustSizeMacro(cg->interpolationCoordinatesLocal,numberOfGrids);
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    // cg->numberOfInterpolationPointsLocal(grid)=numberOfInterpolationPoints(grid);
    cg->interpoleeGridLocal[grid].redim(0);
    cg->interpoleeGridLocal[grid]=interpoleeGrid[grid];

    cg->variableInterpolationWidthLocal[grid].redim(0);
    cg->variableInterpolationWidthLocal[grid]=variableInterpolationWidth[grid]; 

    cg->interpolationPointLocal[grid].redim(0);
    cg->interpolationPointLocal[grid]=interpolationPoint[grid];

    cg->interpoleeLocationLocal[grid].redim(0);
    cg->interpoleeLocationLocal[grid]=interpoleeLocation[grid];

    cg->interpolationCoordinatesLocal[grid].redim(0);
    cg->interpolationCoordinatesLocal[grid]=interpolationCoordinates[grid];
  }

  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    cg.numberOfInterpolationPoints(grid)=ParallelUtility::getSum(cgni(grid));
  }
  
  #endif
//  Tell the CompositeGrid that the interpolation data have been computed:
  cg->computedGeometry |=
    CompositeGrid::THEmask                     |
    CompositeGrid::THEinterpolationCoordinates |
    CompositeGrid::THEinterpolationPoint       |
    CompositeGrid::THEinterpoleeLocation       |
    CompositeGrid::THEinterpoleeGrid;

  delete [] interpoleeGrid;
  delete [] interpolationPoint;
  delete [] interpoleeLocation;
  delete [] interpolationCoordinates;
  delete [] variableInterpolationWidth;

  return 0;
}




int Ogen::
interpolateAll(CompositeGrid & cg, IntegerArray & numberOfInterpolationPoints,CompositeGrid& cg0 )
// =================================================================================
// /Description:
//   Now try to interpolate discretization points from grids with a higher priority.
// 
// =================================================================================
{
  Overture::checkMemoryUsage("classifyPoints interpolateAll START");

  const int numberOfBaseGrids=cg.numberOfBaseGrids();

  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int i;
  real x0,x1,x2;
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  intSerialArray ia, ia2, interpolates, useBackupRules;
  realSerialArray x,r;
  Range R, Rx(0,cg.numberOfDimensions()-1);
  const int numberOfDimensions=cg.numberOfDimensions();
  
  doubleLengthInt numberOfGridPoints=0;
  for( int grid=0; grid<numberOfBaseGrids; grid++ )
  {
    const IntegerArray & d = cg[grid].dimension();
    doubleLengthInt numGridPoints = (d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);
    numberOfGridPoints+=numGridPoints;
    #ifdef USE_PPP
    if( debug & 2 )
      printF("Ogen::interpolateAll: grid=%i numberOfGridPoints=%lli\n",grid,numGridPoints);
    #endif
  }

  for( int grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    intArray & maskd = c.mask();
    intArray & inverseGridd = cg.inverseGrid[grid];
    // realArray & center = c.center();
    realArray & rId = cg.inverseCoordinates[grid];
    const bool isRectangular = c.isRectangular();
    

    GET_LOCAL(int,maskd,mask);
    GET_LOCAL(int,inverseGridd,inverseGrid);
    GET_LOCAL(real,rId,rI);
    
    #ifdef USE_PPP
      realSerialArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(c.center(),center);
    #else
      const realSerialArray & center = c.center();
    #endif

    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      c.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<numberOfDimensions; dir++ )
      {
	iv0[dir]=c.gridIndexRange(0,dir);
	if( c.isAllCellCentered() )
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
		
    }
    #define XC0(i1,i2,i3) (xab[0][0]+dvx[0]*(i1-iv0[0]))
    #define XC1(i1,i2,i3) (xab[0][1]+dvx[1]*(i2-iv0[1]))
    #define XC2(i1,i2,i3) (xab[0][2]+dvx[2]*(i3-iv0[2]))
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

    // make a list of all discretization points that we will check
    c.mask().periodicUpdate();
    c.mask().updateGhostBoundaries();    

    // check points from interior points plus periodic boundaries
    getIndex(c.extendedIndexRange(),I1,I2,I3,-1);  // by default exclude boundaries
    bool noMixedBoundaries=true;
    int axis;
    for(axis=0; axis<numberOfDimensions; axis++ )
    {
      // include boundaries on periodic sides AND at polarSingularities
      if( c.isPeriodic(axis) )
	Iv[axis]=Range(c.gridIndexRange(Start,axis),c.gridIndexRange(End,axis));   // note: Iv[.] == I1,I2,I3
      else if( c.isAllVertexCentered() )
      {
        Mapping & map = c.mapping().getMapping();
        if( map.getTypeOfCoordinateSingularity(Start,axis)==Mapping::polarSingularity )
          Iv[axis]=Range(c.gridIndexRange(Start,axis),Iv[axis].getBound());
	if( map.getTypeOfCoordinateSingularity(End  ,axis)==Mapping::polarSingularity )
	  Iv[axis]=Range(Iv[axis].getBase(),c.gridIndexRange(End,axis));
      }

      // include boundary and ghost lines lines on mixed-boundaries. *****
      // **** why do we include ghost lines ? they should have already been marked ****
      if( false ) // *wdh* 020705
      {
	if( c.boundaryFlag(Start,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	{
	  Iv[axis]=Range(c.gridIndexRange(Start,axis)-1,Iv[axis].getBound());
	  noMixedBoundaries=false;
	}
	if( c.boundaryFlag(End,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	{
	  Iv[axis]=Range(Iv[axis].getBase(),c.gridIndexRange(End,axis)+1);
	  noMixedBoundaries=false;
	}
      }
      
    }

    bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
    int maxNumberToCheckOnThisGrid = I1.length()*I2.length()*I3.length();
    if( maxNumberToCheckOnThisGrid <0 )
    {
      printf("Ogen:interpolateAll:ERROR: maxNumberToCheckOnThisGrid<0 : Maybe this needs to be an int64!\n");
      OV_ABORT("error");
    }
    ia.redim(maxNumberToCheckOnThisGrid,3);  

    i=0;
    if( ok )
    {
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    if( mask(i1,i2,i3) & MappedGrid::ISdiscretizationPoint 
		|| mask(i1,i2,i3) & MappedGrid::ISinteriorBoundaryPoint   // redo mixed-boundary interp points
	      )
	    {                               
	      ia(i,0)=i1;
	      ia(i,1)=i2;
	      ia(i,2)=i3;
	      i++;
	    }
	  }
	}
      }
    }
    
    int numberLeftToCheck=i;
    R=Range(0,numberLeftToCheck-1);

    if( info & 4 ) 
      printf("*** All interpolate: grid %i (%s) There are numberLeftToCheck=%i points to check for interpolation\n",
	     grid,(const char *)c.getName(),numberLeftToCheck);

    Mapping & map = cg[grid].mapping().getMapping();
    bool firstGridChecked=TRUE;
    int numberOfGridsToCheck = numberOfBaseGrids-1-grid;
    if( improveQualityOfInterpolation )
      numberOfGridsToCheck=numberOfBaseGrids-1;  // interpolate from lower grids too.

    for( int g2=0; g2<numberOfGridsToCheck; g2++ )
    {
      int grid2=numberOfBaseGrids-1-g2;
      if( grid2<=grid )
        grid2--;

      if( ( !isNew(grid) && !isNew(grid2) ) || !cg0.mayInterpolate(grid,grid2,0) )
      {
        if( info & 4 )
	  printf("skip all interpolation grids: grid=%i (isNew=%i), grid2=%i (isNew=%i), mayInterp=%i \n",
                  grid,isNew(grid),grid2,isNew(grid2),cg0.mayInterpolate(grid,grid2,0));
	continue;
      }

      Mapping & map2 = cg[grid2].mapping().getMapping();

       
      // This is correct: we need to use the bounding box for the whole of grid2 since we are checking which
      // points of "grid" are inside grid2: 
      const RealArray & boundingBox = cg[grid2].boundingBox();  


      // const RealArray & bb = map2.getBoundingBox();  
      bool mapsIntersect=map.intersects(map2,-1,-1,-1,-1,.1);
      if( !mapsIntersect && info & 4 ) 
	printf("*** All interpolate: grid %s does NOT intersect grid %s \n",
	       (const char *)c.mapping().getName(Mapping::mappingName),
	       (const char *)map2.getName(Mapping::mappingName));
      
      if( mapsIntersect )
      {
	// these mappings may intersect
        if( info & 4 ) 
          fprintf(plogFile,"*** interpolateAll: try to interpolate interior points of grid %s from grid %s "
                 "boundingBox=[%g,%g][%g,%g][%g,%g] \n",
           (const char *)c.getName(),(const char *)cg[grid2].getName(),
              boundingBox(0,0),boundingBox(1,0),boundingBox(0,1),boundingBox(1,1),
		 boundingBox(0,2),boundingBox(1,2));

	// ia : make a new list of points remaining to check
        if( !firstGridChecked )
	{
    	  i=0;
	  for( int j=0; j<numberLeftToCheck; j++ )
	  {
	    if( mask(ia(j,0),ia(j,1),ia(j,2)) & MappedGrid::ISdiscretizationPoint
                || mask(ia(j,0),ia(j,1),ia(j,2)) & MappedGrid::ISinteriorBoundaryPoint  
              )
	    {
	      ia(i,0)=ia(j,0);
	      ia(i,1)=ia(j,1);
	      ia(i,2)=ia(j,2);
	      i++;
	    }
	  }
	  numberLeftToCheck=i;
          #ifndef USE_PPP
	  if( numberLeftToCheck==0 ) 
	    break;                  // in serial we are done, no more points to check
          #endif
	}
        else
          firstGridChecked=false;

        // **** We could fill in x(i,.) at the same time as ia2 -- fill in at most
        //      maxNumberAllowedToCheck at a time
        //      We could use a more accurate "inside" check

        // ia2: make a list of points to check on grid2 (those points inside the bounding box)
        ia2.redim(numberLeftToCheck,3);  

        int k=0;
        if( numberOfDimensions==2 )
	{
	  for( i=0; i<numberLeftToCheck; i++ )
	  {
            if( !isRectangular )
	    {
	      x0=center(ia(i,0),ia(i,1),ia(i,2),axis1);
	      x1=center(ia(i,0),ia(i,1),ia(i,2),axis2);
	    }
	    else
	    {
	      x0=XC0(ia(i,0),ia(i,1),ia(i,2));
	      x1=XC1(ia(i,0),ia(i,1),ia(i,2));
	    }
	    if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) &&
		x1 >= boundingBox(Start,axis2) && x1 <= boundingBox(End,axis2) )
	    {
	      ia2(k,0)=ia(i,0);
	      ia2(k,1)=ia(i,1);
	      ia2(k,2)=ia(i,2);
	      k++;
	    }
	  }
	}
	else if( numberOfDimensions==3 )
	{
	  for( i=0; i<numberLeftToCheck; i++ )
	  {
            if( !isRectangular )
	    {
	      x0=center(ia(i,0),ia(i,1),ia(i,2),axis1);
	      x1=center(ia(i,0),ia(i,1),ia(i,2),axis2);
	      x2=center(ia(i,0),ia(i,1),ia(i,2),axis3);
	    }
	    else
	    {
	      x0=XC0(ia(i,0),ia(i,1),ia(i,2));
	      x1=XC1(ia(i,0),ia(i,1),ia(i,2));
	      x2=XC2(ia(i,0),ia(i,1),ia(i,2));
	    }
	    if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) &&
		x1 >= boundingBox(Start,axis2) && x1 <= boundingBox(End,axis2) &&
		x2 >= boundingBox(Start,axis3) && x2 <= boundingBox(End,axis3) )
	    {
	      ia2(k,0)=ia(i,0);
	      ia2(k,1)=ia(i,1);
	      ia2(k,2)=ia(i,2);
	      k++;
	    }
	  }
	}
	else
	{
	  for( i=0; i<numberLeftToCheck; i++ )
	  {
	    if( !isRectangular )
              x0=center(ia(i,0),ia(i,1),ia(i,2),axis1);
            else
              x0=XC0(ia(i,0),ia(i,1),ia(i,2));
	    if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) )
	    {
	      ia2(k,0)=ia(i,0);
	      ia2(k,1)=ia(i,1);
	      ia2(k,2)=ia(i,2);
	      k++;
	    }
	  }
	}
        int numberToCheck=k; 
	// NO: if( numberToCheck==0 )
        //   continue;               // no points to check on this grid

        R=numberToCheck;
	if( numberToCheck>0 )
	{
	  x.redim(R,Rx);
	  r.redim(R,Rx); r=-1.;
	  if( !isRectangular )
	  {
	    for( axis=0; axis<numberOfDimensions; axis++ )
	      x(R,axis)=center(ia2(R,0),ia2(R,1),ia2(R,2),axis);
	  }
	  else
	  {
	    for( int i=R.getBase(); i<=R.getBound(); i++ )
	    {
	      iv[0]=ia2(i,0), iv[1]=ia2(i,1),iv[2]=ia2(i,2);
	      for( axis=0; axis<numberOfDimensions; axis++ )
		x(i,axis)=XC(iv,axis);
	    }
	  }
	}
	
	if( useBoundaryAdjustment )
	  if( numberToCheck>0 )
            adjustBoundary(cg,grid,grid2,ia2(R,Rx),x);    // adjust boundary points on shared sides
          else
             adjustBoundary(cg,grid,grid2,Overture::nullIntArray(),Overture::nullRealArray());

        int maxNumberToCheck=ParallelUtility::getMaxValue(numberToCheck); 
        int sumNumberToCheck=ParallelUtility::getSum(numberToCheck); 

	if( info & 2 ) 
	  printF("*** interpolateAll: try to interpolate %i points of grid %s from grid %s \n",
		 sumNumberToCheck,(const char *)c.getName(),(const char *)cg[grid2].getName());

	// the dpm inverse requires quite a bit of temporary storage. Therefore we reduce
	// the number of points that we invert at any one time. This seems to be faster too.

  	// *wdh* 080207 const int maxNumberAllowedToCheck=10000; // *wdh* 070408 // 5000;     
  	// const int maxNumberAllowedToCheck=(int)min((doubleLengthInt)500000,max(numberOfGridPoints/10,(doubleLengthInt)10000)); 
  	const int maxNumberAllowedToCheck=(int)min((doubleLengthInt)maximumNumberOfPointsToInvertAtOneTime,
                                                   max(numberOfGridPoints/10,(doubleLengthInt)10000)); 
	
	if( maxNumberToCheck<maxNumberAllowedToCheck )
	{
#ifdef USE_PPP
	  if( numberToCheck>0 )
	    map2.inverseMapS(x,r); 
	  else
	    map2.inverseMapS(Overture::nullRealArray(),Overture::nullRealArray()); 
#else
	  if( numberToCheck>0 ) 
            map2.inverseMap(x,r);
#endif
	}
	else
	{ // in parallel we must take the same number of sub-check steps on all processors
          const int numberOfSubChecks = (maxNumberToCheck+maxNumberAllowedToCheck-1)/maxNumberAllowedToCheck;
          realSerialArray rr, xx;
          Range R0,S;
	  for( int i=0; i<numberOfSubChecks; i++ )
	  {
            int iStart=i*maxNumberAllowedToCheck;
	    int iEnd  =min((i+1)*maxNumberAllowedToCheck-1,numberToCheck-1);
	    int num = iEnd-iStart+1;
            int iEndMax = min((i+1)*maxNumberAllowedToCheck-1,maxNumberToCheck-1);
            if( info & 2 )
	      printF("  ...sub-invert step %i: invert points S=[%i,%i] \n",i,iStart,iEndMax);
	    // map2.inverseMap(x(S,Rx),r(S,Rx));
	    if( num>0 )
	    {
              S=Range(iStart,iEnd);
	      R0=S-S.getBase();                 // work around for A++ bug.
              if( rr.dimension(0)!=R0 )
	      {
		rr.redim(R0,Rx); xx.redim(R0,Rx);
	      }
	      xx(R0,Rx)=x(S,Rx);
	      rr=-1.;
	    }
	    
#ifdef USE_PPP
	    if( num>0 )
              map2.inverseMapS(xx,rr);
            else 
              map2.inverseMapS(Overture::nullRealArray(),Overture::nullRealArray());
#else
            if( num>0 )
  	      map2.inverseMap(xx,rr);
#endif
	    if( num>0 )
              r(S,Rx)=rr(R0,Rx);
	    // display(r,"r","%4.1f ");
	  }
	}

	if( numberToCheck>0 )
	{
	  if( debug & 16 )
	    ::display(r,"Here are the inverseMap coordinates",plogFile);

	  interpolates.redim(numberToCheck); interpolates=TRUE;
	  useBackupRules.redim(numberToCheck);  useBackupRules=FALSE;

	} // end if numberToCheck>0
	else
	{
          r.redim(0);  // do this for canInterpolate
	}
	
        #ifdef USE_PPP
          checkCanInterpolate(cg0,cg.gridNumber(grid),cg.gridNumber(grid2), r, interpolates, useBackupRules);
        #else
    	  cg0.rcData->canInterpolate(cg.gridNumber(grid),cg.gridNumber(grid2), r, interpolates, 
                                   useBackupRules, checkForOneSided );
        #endif
        if( numberToCheck>0 )
	{
	  if( debug & 8 )
	  {
	    fprintf(plogFile,"--- interpolateAll: interpolate pts on grid=%i from grid2=%i : \n",grid,grid2);
	    for( int i=R.getBase(); i<=R.getBound(); i++ )
	    {
	      fprintf(plogFile," pt (%i,%i,%i) on grid %i interpolates=%i from grid2=%i "
                     " r=(%8.2e,%8.2e,%8.2e), x=(%8.2e,%8.2e,%8.2e)\n",
                     ia2(i,0),ia2(i,1),ia2(i,2),grid,interpolates(i),grid2,
		      r(i,0),r(i,1),(numberOfDimensions==2 ? 0. : r(i,2)),
                     x(i,0),x(i,1),(numberOfDimensions==2 ? 0. : x(i,2)));
	    }
	  }
	  if( noMixedBoundaries )
	  {
	    where( interpolates(R) )
	    {
	      mask(ia2(R,0),ia2(R,1),ia2(R,2))=MappedGrid::ISinterpolationPoint;
	      inverseGrid(ia2(R,0),ia2(R,1),ia2(R,2)) = grid2;   // can interpolate from grid2
	      for( axis=0; axis<numberOfDimensions; axis++ )
		rI(ia2(R,0),ia2(R,1),ia2(R,2),axis)=r(R,axis);   // save coordinates
	    }
	  }
	  else
	  {
	    // when there are mixed boundaries we should not over-write interpolation points
	    // that are already there since these will be marked as interiorBoundaryPoints.
	    where( interpolates(R) && inverseGrid(ia2(R,0),ia2(R,1),ia2(R,2))!=grid2  )
	    {
	      mask(ia2(R,0),ia2(R,1),ia2(R,2))=MappedGrid::ISinterpolationPoint;
	      inverseGrid(ia2(R,0),ia2(R,1),ia2(R,2)) = grid2;   // can interpolate from grid2
	      for( axis=0; axis<numberOfDimensions; axis++ )
		rI(ia2(R,0),ia2(R,1),ia2(R,2),axis)=r(R,axis);   // save coordinates
	    }
  
	  }
	
	  numberOfInterpolationPoints(grid)+=sum(interpolates(R));
	}
	
	// rI.display("here is rI");
      }
    }
    c.mask().periodicUpdate();
    c.mask().updateGhostBoundaries();    

    Overture::checkMemoryUsage(sPrintF("classifyPoints (interpolateAll, grid=%i)",grid));

  }

  return 0;
}


int Ogen::
computeInterpolationStencil(CompositeGrid & cg, 
			    const int & grid, 
			    const int & gridI, 
			    const real r[3], 
			    int stencil[3][2],
			    bool useOneSidedAtBoundaries /* = true */,
			    bool useOddInterpolationWidth /* = false */ )
// =========================================================================================
//  /grid (input): the point to interpolate is on this grid.
//  /gridI (input): the interpolee grid, the interpolation stencil lies on this grid.
//  /r (input): coordinates of the interpolation point. 
//  /stencil[axis][side] (output) : index values of the stencil
// /useOneSidedAtBoundaries (input) : at physical boundaries use one side interpolation.
//  /useOddInterpolationWidth (input) : if true, increase the interpolation width by
//    one if it is an even width (used by moving update)
// =========================================================================================
{
  Range Rx(0,cg.numberOfDimensions()-1);
  int l=0;  // multigrid level *********
  const IntegerArray & interpolationWidth = cg.interpolationWidth(Rx,grid,gridI,l);
  int width[3];
  int axis;
  for( axis=0; axis<cg.numberOfDimensions(); axis++ )
  {
    width[axis]=interpolationWidth(axis);
    if( useOddInterpolationWidth && (width[axis]%2)==0 )
      width[axis]+=1;
  }
  MappedGrid & g2 = cg[gridI];
  for( axis=0; axis<cg.numberOfDimensions(); axis++ )
  {
    // Get the lower-left corner of the interpolation cube.
    int intLoc=int(floor(r[axis]/g2.gridSpacing(axis) + g2.indexRange(0,axis) -
			 .5 * width[axis] + (g2.isCellCentered(axis) ? .5 : 1.)));
    if( !g2.isPeriodic(axis) && useOneSidedAtBoundaries ) 
    {
      if( (intLoc < g2.extendedIndexRange(0,axis)) && (g2.boundaryCondition(Start,axis)>0) )
      {
	//  Point is close to a BC side. One-sided interpolation used.
	intLoc = g2.extendedIndexRange(0,axis);
      }
      if( (intLoc + width[axis] - 1 > g2.extendedIndexRange(1,axis)) 
           && (g2.boundaryCondition(End,axis)>0) )
      {
	// Point is close to a BC side.  One-sided interpolation used.
	intLoc = g2.extendedIndexRange(1,axis) - width[axis] + 1;
      }
    } // end if
    stencil[axis][0] = intLoc;
    stencil[axis][1] = intLoc+width[axis]-1;
  } // end for_1
  for( axis=cg.numberOfDimensions(); axis<3; axis++ )
  {
    stencil[axis][0] = stencil[axis][1] = g2.extendedIndexRange(0,axis);
  }
  return 0;
}


 
 
