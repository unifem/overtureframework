#include "Overture.h"
#include "Ogen.h"
#include "display.h"
#include "conversion.h"
#include "ParallelUtility.h"

// define BOUNDS_CHECK 1

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


// int Ogen::
// projectToBoundary( CompositeGrid & cg,
// 		   const int & grid, 
// 		   const realArray & r,
// 		   const int iv[3], 
// 		   const int ivp[3], 
// 		   real rv[3] )
// // ===============================================================================================
// // /Description:
// //   Determine the intersection of the line segment r(iv) --> r(ivp) with the rBound bounding box.
// // or return the r(ivp) if there is no intersection
// // ===============================================================================================
// {

//   // look for solutions to  rBound = r(iv) + s [ r(ivp) - r(iv) ]
//   // Choose the root with a minimum value for s in [0,1]
//   const real eps = REAL_EPSILON*10.;
//   real sMin=2., s, rv0[3], rv1[3];
//   int dir;
//   for( dir=0; dir<cg.numberOfDimensions(); dir++ )
//   {
//     rv0[dir]=r(iv[0],iv[1],iv[2],dir);
//     rv1[dir]=r(ivp[0],ivp[1],ivp[2],dir);

//     real rDiff=rv1[dir]-rv0[dir];
//     if( fabs(rDiff) > eps )
//     {
//       rDiff=1./rDiff;
//       s=(rBound(Start,dir,grid)-rv0[dir])*rDiff;
//       if( fabs(s-.5)<=.51 )
//       {
// 	sMin=min(sMin,s);
//         continue;     // this is a possible root. s in [0,1]. The other choice is not possible.
//       }
//       else
//       {
// 	s=(rBound(End,dir,grid)-rv0[dir])*rDiff;
//         if( fabs(s-.5)<.51 )
// 	  sMin=min(sMin,s);
//       }
//     }
//   }
//   if( fabs(sMin-.5)>.51 )
//     sMin=1.;  // *wdh* 990421
  
//   for( dir=0; dir<cg.numberOfDimensions(); dir++ )
//     rv[dir]=rv0[dir]+sMin*(rv1[dir]-rv0[dir]);
//   return 0;
// }

int Ogen::
projectToParameterBoundary( const real rv0[3], const real rv1[3], real rv[3], 
                            const int numberOfDimensions, const int grid )
// ===============================================================================================
//  *new* version
// 
// /Description:
//   Determine the intersection of the line segment r(iv) --> r(ivp) with the rBound bounding box.
// or return the r(ivp) if there is no intersection
// ===============================================================================================
{

  // look for solutions to  rBound = r(iv) + s [ r(ivp) - r(iv) ]
  // Choose the root with a minimum value for s in [0,1]
  const real eps = REAL_EPSILON*10.;
  real sMin=2., s;
  int dir;
  for( dir=0; dir<numberOfDimensions; dir++ )
  {
    real rDiff=rv1[dir]-rv0[dir];
    if( fabs(rDiff) > eps )
    {
      rDiff=1./rDiff;
      s=(rBound(Start,dir,grid)-rv0[dir])*rDiff;
      if( fabs(s-.5)<=.51 )
      {
	sMin=min(sMin,s);
        continue;     // this is a possible root. s in [0,1]. The other choice is not possible.
      }
      else
      {
	s=(rBound(End,dir,grid)-rv0[dir])*rDiff;
        if( fabs(s-.5)<.51 )
	  sMin=min(sMin,s);
      }
    }
  }
  if( fabs(sMin-.5)>.51 )
    sMin=1.;  // *wdh* 990421
  
  for( dir=0; dir<numberOfDimensions; dir++ )
    rv[dir]=rv0[dir]+sMin*(rv1[dir]-rv0[dir]);
  return 0;
}


int Ogen::
getHoleWidth( CompositeGrid & cg,
              MappedGrid & g2, 
              int pHoleMarker[3], 
              IntegerArray & holeCenter, 
              IntegerArray & holeMask, 
              IntegerArray & holeWidth, 
              RealArray & r,
              RealArray & x,
              const int *pIndexRange2,
              const int *pExtendedIndexRange2,
              const int *plocalIndexBounds2,
              int iv[3], int jv[3], int jpv[3],
               bool isPeriodic2[3], bool isPeriodic2p[3], 
              const Index Iv[3], 
              const int & grid, const int & grid2,
              int & ib, int & ib2,
              int & skipThisPoint,
              int & initialPoint,
              const int & numberOfDimensions,
              const int & axisp1, const int & axisp2, const real&  cellCenterOffset,
              const int & maximumHoleWidth, int & numberOfHoleWidthWarnings )
// ======================================================================================================
// 
//  /Description:
//     This function is called by cutHolesNew as part of the hole cutting process to compute the
//  "holeWidth" between consecutive "hole" points. 
//  
// ======================================================================================================
{

  int &i1=iv[0], &i2=iv[1], &i3=iv[2];
  real rv[3]={0.,0.,0.};  // r(i1,i2,i3,.)
  real rvm[3]={0.,0.,0.};  // r(ivm[],.
  real rvp[3]={0.,0.,0.};  // r(ivp[],.

  real rp[3]={0.,0.,0.};  // holds point projected onto the parameter bounding box
  
  skipThisPoint=0;
  initialPoint=0;
		  
  const int maxHoleWidthWarnings=10;

  #define indexRange2(side,axis) pIndexRange2[(side)+2*(axis)]
  #define extendedIndexRange2(side,axis) pExtendedIndexRange2[(side)+2*(axis)]
  #define localIndexBounds2(side,axis) plocalIndexBounds2[(side)+2*(axis)]

  int pHoleOffset[3]={0,0,0}; 
  #define holeOffset(axis) pHoleOffset[axis]
  #define holeMarker(axis) pHoleMarker[axis]

  const real *pGridSpacing2 = g2.gridSpacing().Array_Descriptor.Array_View_Pointer0;
  #define gridSpacing2(axis) pGridSpacing2[axis]

  for( int dir=0; dir<numberOfDimensions; dir++ )
  {
    rv[dir]=r(i1,i2,i3,dir);
    holeWidth(dir,ib)=1; // by default check a stencil that extends by this amount in each direction.
  }
  
  // *********************************************************************************
  // m : In 3D we need to check 2 possible previous points.
  for( int m=0; m<numberOfDimensions-1; m++ )
  {
    int axisT = m==0 ? axisp1 : axisp2;  // tangential direction.
    int ibb =ib-1+m;   // i1-1 or i1


    // **********************************
    // *** ivp = holds the next point ***
    // **********************************
    int ivp[3]={i1,i2,i3};   // holds next point.
    ivp[axisT]++;            // increment tangential direction.


    // ************************************************************************
    // **** holeOffset = max index-distance between the new hole and old ******
    // ************************************************************************

    // ******************************************************************************
    // *** The box-width we use for checking will be max(holeWidth,holeOffset-1) ****
    // ******************************************************************************
    int maxHoleOffset=0, maxHoleOffsetWidth=0;
    for(int dir=0; dir<numberOfDimensions; dir++ )
    {
      holeOffset(dir)=abs(holeMarker(dir)-holeCenter(dir,ibb));  // offset between this point and the last
      maxHoleOffset=max(maxHoleOffset,holeOffset(dir));
      maxHoleOffsetWidth=max(maxHoleOffsetWidth,holeOffset(dir)-holeWidth(dir,ibb));
    }
    
    if( maxHoleOffset==0 || maxHoleOffsetWidth<0 )
    {
      // New hole point is in the SAME location as the previous hole, or is inside the previous box

      // *wdh* 040912 : only skip this pt if the next pt is there 
      if(ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])==1)
      {
	// skip this point, it is contained in the previous box (or boxes in 3D)
	skipThisPoint++;
	if( skipThisPoint>=numberOfDimensions-1 )
	{
	  if( info & 4 ) fprintf(plogFile,"  skip point ib=%i(%i) (inside previous box)\n",ib,ib2);
	  break;
	}
	else
	  continue;
      }
    }


    // ====================================================================================
    // check the next point to see if it there
    // If it is *NOT* there we must increase the size of this box.
    bool nextPointIsNotInside=ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])!=1;
    if( nextPointIsNotInside )
    { // the next point is not inside (not in the grid or was not a cutting pt)
      if( info & 4 ) 
	fprintf(plogFile,"  : m=%i, next point: r=(%6.2e,%6.2e) is not inside, holeMask(next)=%i\n",
		m, r(ivp[0],ivp[1],ivp[2],0),r(ivp[0],ivp[1],ivp[2],1),holeMask(ivp[0],ivp[1],ivp[2]));
      bool widthFound=false;
      if( holeMask(ivp[0],ivp[1],ivp[2])!=0 )
      {
	// only use the next point if r does not change too much -- the next r value
	// could be interpolating from another part of the grid
	// projectToParameterBoundary will find the closest intersection of the line segment
	// with the rBound bounding box.

	// **NOTE: rp[axis] is computed by projectToParameterBoundary
	real dr=0.; // max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
          rvp[dir]=r(ivp[0],ivp[1],ivp[2],dir);
          real dri = fabs(rv[dir]-rvp[dir]);
	  if( isPeriodic2[dir] && dri>.5 )
	  { // shift rvp[dir] by + or - 1 to get the closer periodic image
	    if( rvp[dir]>rv[dir] ){ rvp[dir]-=1.; } else { rvp[dir]+=1.; } //
            dri = fabs(rv[dir]-rvp[dir]);
	  }
	  dr=max(dr,dri);
	}
	
        // if( dr<.3 && projectToParameterBoundary(cg,grid2,r,iv,ivp,rp)==0 )
	if( dr<.3 && projectToParameterBoundary(rv,rvp,rp,numberOfDimensions,grid2)==0 )
	{
	  widthFound=true;
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    // note: cellCenterOffset will round to the nearest point. 
	    jpv[dir]=(int)floor( rp[dir]/gridSpacing2(dir)+indexRange2(Start,dir)-cellCenterOffset );
	    holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir])+1);
	  }
	  if( info & 4 ) 
	    fprintf(plogFile,"  : m=%i, next pt outside, intersection with bndy=(%6.2e,%6.2e,%6.2e) "
		    "current holeWidth=(%i,%i,%i)\n",m,rp[0],rp[1],rp[2],
		    holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 1 : holeWidth(2,ib));
	}
      }
      else
      {
	if( info & 4 ) 
	  fprintf(plogFile,"  : holeMask=0 (far outside the grid) -- something is wrong here ? \n");
      }
      if( !widthFound ) // 990914 wdh
      {
	// compute the distance in grid points to the nearest boundary. 
        // *wdh* 081004 -- use localIndexBounds in parallel
	int bDist=INT_MAX;
	for( int dir=0; dir<numberOfDimensions; dir++ )
	  bDist=min(bDist, localIndexBounds2(End,dir)-localIndexBounds2(Start,dir));
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  bDist=min(bDist,abs(holeMarker(dir)-localIndexBounds2(Start,dir))
		    ,abs(holeMarker(dir)-localIndexBounds2(End  ,dir)));
	}
	if( info & 4 ) 
	  fprintf(plogFile,"  : dist. to nearest boundary = %i grid points\n",bDist);
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
	  if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
	  {
	    if( info & 1 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
	    {
	      printf("cutHoles:WARNING: Final point holeWidth is very large for this point, "
		     "holeWidth=%i, for point ib=%i(ib2=%i), along axis=%i \n"
		     "  case: next pt on cutting surface not in grid, dist to nearest boundary=%i \n",
		     holeWidth(dir,ib),ib,ib2,dir,bDist);
	      fprintf(plogFile,"cutHoles:WARNING: Final point holeWidth is very large for this point, "
		     "holeWidth=%i, for point ib=%i(ib2=%i), along axis=%i \n"
		     "  case: next pt on cutting surface not in grid, dist to nearest boundary=%i \n",
		     holeWidth(dir,ib),ib,ib2,dir,bDist);
	    }
	    holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
	    if( info & 1 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
	    {
	      numberOfHoleWidthWarnings++;
	      printf("        : something is wrong here. I am reducing the width to %i\n",holeWidth(dir,ib));
	      fprintf(plogFile,"        : something is wrong here. I am reducing the width to %i\n",holeWidth(dir,ib));
	      if( numberOfHoleWidthWarnings==maxHoleWidthWarnings ) 
		printf("cutHoles:INFO: I will stop printing `holeWidth very large' warnings!\n");
	    }
	  }
	}
      }
    } // end nextPointIsNotInside 
    // ====================================================================================

                   
    // -----------------------------------------------------------------------------------
    // now check to see if the previous point is there

    // **********************************
    // *** ivm = holds previous point ***
    // **********************************
    int ivm[3]={i1,i2,i3};  
    ivm[axisT]--;

    bool previousPointIsNotInside=ivm[axisT] < Iv[axisT].getBase() || holeMask(ivm[0],ivm[1],ivm[2])!=1;
		      
    if( previousPointIsNotInside )
    { // the previous point is NOT inside this grid -- try to guess the box width in other ways.

      if( ivm[axisT] < Iv[axisT].getBase() ) // first point
      {
	// this is really the first point -- width=1 should do.
	for( int dir=0; dir<numberOfDimensions; dir++ )
  	  holeWidth(dir,ib)=max(1,holeWidth(dir,ib));
	if( info & 4 ) 
	{
	  initialPoint++;
	  if( initialPoint>=numberOfDimensions-1 )
	  {
            // *wdh* 2012/03/17 -- this next use of "x" is wrong -- I think it is now a 1-d array
	    // fprintf(plogFile,"  : m=%i, previous pt is outside , this is an INITIAL point, x=(%e,%e,%e)\n",
	    // 	    m,x(i1,i2,i3,0),x(i1,i2,i3,1),numberOfDimensions==2 ? 0. : x(i1,i2,i3,2));
	    fprintf(plogFile,"  : m=%i, previous pt is outside , this is an INITIAL point\n",m);
	  }
	  
	}
      }
      else // not the first point 
      {
	// this is not the first point, the boundary must have entered this grid.

	// ---- *wdh* 070401
	real dr=0.;
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
          rvm[dir]=r(ivm[0],ivm[1],ivm[2],dir);
	  real dra = fabs(rv[dir]-rvm[dir]);
	  if( isPeriodic2[dir] && dra>.5 && dra<1.5 ) 
	  {
            // dra=fabs(1.-dra);
            if( rvm[dir]>rv[dir] ){ rvm[dir]-=1.; } else { rvm[dir]+=1.; } //
	    dra = fabs(rv[dir]-rvm[dir]);
	  }
	  dr=max(dr,dra);
	}
			  

	// ................................................................
	// **NOTE: rp[axis] is computed by projectToParameterBoundary
	bool previousPointIsOutsideButInvertible=ivm[axisT]>=Iv[axisT].getBase() && 
	  holeMask(ivm[0],ivm[1],ivm[2])==2  &&
	  dr<.3 && 
	  projectToParameterBoundary(rv,rvm,rp,numberOfDimensions,grid2)==0;
//	  projectToParameterBoundary(cg,grid2,r,iv,ivm,rp)==0;
			  
	if( previousPointIsOutsideButInvertible )
	{
	  // previous point was invertible but must have been outside (or a non-cutting point).
	  // determine its location index space.

	  if( info & 4 ) 
	    fprintf(plogFile,"  : m=%i, prev pt is outside but invert., intersect wth rBound =(%6.2e,%6.2e,%6.2e)\n",
		    m,rp[0],rp[1],rp[2]);
	  // only use the next point if r does not change too much -- the next r value
	  // could be interpolating from another part of the grid
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    // note: cellCenterOffset will round to the nearest point. 
	    jpv[dir]=(int)floor( rp[dir]/gridSpacing2(dir)+indexRange2(Start,dir)-cellCenterOffset );
	    int jwidth=abs(jpv[dir]-jv[dir])+1;
	    if( isPeriodic2[dir] )
	    { // correct the case when jpv[dir] crosses a branch cut.
	      if( jwidth> (indexRange2(1,dir)-indexRange2(Start,dir))/2 )
	      {
		jwidth=max(1,abs(jwidth-(indexRange2(1,dir)-indexRange2(Start,dir)+1)));
	      }
	    }
			    
	    holeWidth(dir,ib)=max(holeWidth(dir,ib),jwidth); 

	    if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
	    {
	      if( info & 2 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
	      {
		real dr0=0.;
                for( int dir2=0; dir2<numberOfDimensions; dir2++ )
                  dr0=max(dr0, fabs(rv[dir2]-r(ivm[0],ivm[1],ivm[2],dir2)) );
		printf("cutHoles:WARNING: myid=%i holeWidth=%i is very large"
		       " grid=%i grid2=%i m=%i pt ib=%i(%i) axis=%i \n"
		       " dr0=%g dr=%g (i1,i2,i3)=(%i,%i,%i) ivm=(%i,%i,%i) "
		       "holeMask(i)=%i holeMask(im)=%i\n"
		       "   jv=(%i,%i,%i), jpv=(%i,%i,%i) isPeriodic2=[%i,%i,%i] "
		       "isPeriodic2p=[%i,%i,%i]\n"
		       "  r(i1,i2,i3,.)=[%g,%g,%g] r(ivm,.)=[%g,%g,%g]  rp=[%g,%g,%g}\n",
		       myid,holeWidth(dir,ib),grid,grid2,m,ib,ib2,dir,
		       dr0,dr,i1,i2,i3,ivm[0],ivm[1],ivm[2],holeMask(i1,i2,i3),
		       holeMask(ivm[0],ivm[1],ivm[2]),
		       jv[0],jv[1],jv[2],jpv[0],jpv[1],jpv[2],
		       isPeriodic2[0],isPeriodic2[1],isPeriodic2[2],
		       isPeriodic2p[0],isPeriodic2p[1],isPeriodic2p[2],
		       rv[0],rv[1],rv[2],
		       r(ivm[0],ivm[1],ivm[2],0),r(ivm[0],ivm[1],ivm[2],1),
		       (numberOfDimensions==3 ? r(ivm[0],ivm[1],ivm[2],2) : 0.),
		       rp[0],rp[1],rp[2]);
	      }
				
	      holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
	      if( info & 2 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
	      {
		numberOfHoleWidthWarnings++;
		printf(" -> Reducing width to %i\n", holeWidth(dir,ib));
		if( numberOfHoleWidthWarnings==maxHoleWidthWarnings ) 
		  printf("cutHoles:INFO: I will stop printing `holeWidth very large' warnings!\n");
	      }
	    }
	  } // end for dir
	  if( info & 4 ) 
	    fprintf(plogFile,"  : m=%i, previous pt is useable, current holeWidth=(%i,%i,%i)\n",
		    m,holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 1 : holeWidth(2,ib));
	}
	else // !previousPointIsOutsideButInvertible
	{
	  // ---- There is no previous point. ----

	  // Base the width on the next point (if it is there) AND
	  // extend the width to the nearest boundary. *** this could go wrong maybe ?? ***
	  if( ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])!=0  )
	  {
 
	    real dr=0.; // max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
	    for( int dir=0; dir<numberOfDimensions; dir++ )
	    {
	      rvp[dir]=r(ivp[0],ivp[1],ivp[2],dir);
	      // dr=max(dr, fabs(rv[dir]-rvp[dir]));
	      real dri = fabs(rv[dir]-rvp[dir]);
	      if( isPeriodic2[dir] && dri>.5 )
	      { // shift rvp[dir] by + or - 1 to get the closer periodic image
		if( rvp[dir]>rv[dir] ){ rvp[dir]-=1.; } else { rvp[dir]+=1.; } //
		dri = fabs(rv[dir]-rvp[dir]);
	      }
	      dr=max(dr,dri);
	    }
	    
	    // if( dr<.3 && projectToParameterBoundary(cg,grid2,r,iv,ivp,rp)==0 )
	    if( dr<.3 && projectToParameterBoundary(rv,rvp,rp,numberOfDimensions,grid2)==0 )
	    { // the next point was invertible.
	      if( info & 4 ) 
		fprintf(plogFile,"  : m=%i, next pt is useable, next intersection with "
			"r-bndry =(%6.2e,%6.2e,%6.2e) \n",m,rp[0],rp[1],rp[2]);

	      for( int dir=0; dir<numberOfDimensions; dir++ )
	      {
		// note: cellCenterOffset will round to the nearest point. 
		jpv[dir] = (int)floor( rp[dir]/gridSpacing2(dir)+indexRange2(Start,dir)- 
				       cellCenterOffset );
		holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir]));
	      }
	    }
	  }
	  // compute the distance in grid points to the nearest boundary. 
	// compute the distance in grid points to the nearest boundary. 
	  int bDist=INT_MAX;
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	    bDist=min(bDist, localIndexBounds2(End,dir)-localIndexBounds2(Start,dir));
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    bDist=min(bDist,abs(holeMarker(dir)-localIndexBounds2(Start,dir))
		      ,abs(holeMarker(dir)-localIndexBounds2(End  ,dir)));
	  }
	  if( info & 4 ) 
	    fprintf(plogFile,"  : (index) distance to nearest boundary = %i grid points\n",bDist);
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
	    if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
	    {
	      if( info & 2 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
		printf("cutHoles:WARNING: holeWidth very large, holeWidth=%i,"
		       " grid=%i, grid2=%i, pt ib=%i(%i),m=%i, axis=%i",holeWidth(dir,ib),grid,grid2,
		       m,ib,ib2,dir);
	      holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
	      if( info & 2 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
	      {
		numberOfHoleWidthWarnings++;
		printf(" -> reducing width to %i\n",holeWidth(dir,ib));
		if( numberOfHoleWidthWarnings==maxHoleWidthWarnings ) 
		  printf("cutHoles:INFO: I will stop printing `holeWidth very large' warnings!\n");
	      }
	    }
	  }
	} // end -- no previous point
      } // end -- not the first point 


    } 
    else  // previous point is INSIDE the grid
    {

      real dr=0.;
      for( int dir=0; dir<numberOfDimensions; dir++ )
	dr=max(dr, fabs(rv[dir]-r(ivm[0],ivm[1],ivm[2],dir)) );

      if( dr>.7 ) 
      {
	// point jumps in r --- need to do something special
	if( info & 4 ) 
	  fprintf(plogFile,"cutHoles:There has been a jump in r, ib=%i(%i),im=%i,\n",ib,ib2,m);
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  if( fabs(rv[dir]-r(ivm[0],ivm[1],ivm[2],dir)) > .69 )
	  {
	    // if( g2.isPeriodic(dir)==Mapping::functionPeriodic )
	    if( isPeriodic2[dir] )
	    { // crossed a periodic boundary, 
	      //       +---+---++---+---+
	      //  ... n-2 n-1  n,0  1   2 .... 
	      //           <-  2   ->
	      // holeOffset = |a-(n-b)| = n-a-b : should be a+b
	      holeOffset(dir)=indexRange2(End,dir)-indexRange2(Start,dir)+1-holeOffset(dir);
	      assert( holeOffset(dir) >=0 );
	    }
	    else
	    {
	      if( info & 4 ) 
		fprintf(plogFile,"cutHoles:jump in r but not on a periodic boundary!\n");
	      //  Base the width on the next point (if it is there) AND
	      // extend the width to the nearest boundary. *** this could go wrong maybe ?? ***
	      int ivp[3]={i1,i2,i3};   // holds next point.
	      ivp[axisT]=min(ivp[axisT]+1,Iv[axisT].getBound()); // increment tangential direction.

	      real dr=0.; 
	      for( int dir2=0; dir2<numberOfDimensions; dir2++ )
	      {
		rvp[dir2]=r(ivp[0],ivp[1],ivp[2],dir2);
                // dr=max(dr,fabs(rv[dir2]-rvp[dir2]));

		real dri = fabs(rv[dir2]-rvp[dir2]);
		if( isPeriodic2[dir2] && dri>.5 )
		{ // shift rvp[dir2] by + or - 1 to get the closer periodic image
		  if( rvp[dir2]>rv[dir2] ){ rvp[dir2]-=1.; } else { rvp[dir2]+=1.; } //
		  dri = fabs(rv[dir2]-rvp[dir2]);
		}
		dr=max(dr,dri);
	      }
	      
	      if( holeMask(ivp[0],ivp[1],ivp[2])!=0 && dr<.3 && 
		  projectToParameterBoundary(rv,rvp,rp,numberOfDimensions,grid2)==0 )
//		  projectToParameterBoundary(cg,grid2,r,iv,ivp,rp)==0 )
	      { // the next point was invertible.
		if( info & 4 ) 
		  fprintf(plogFile,"        : jump at ib=%i, next intersection with rBound =(%6.2e,%6.2e,%6.2e)\n",
			  ib, rp[0],rp[1],rp[2]);

		for( int dir=0; dir<numberOfDimensions; dir++ )
		{
		  jpv[dir]=(int)floor(rp[dir]/gridSpacing2(dir)+indexRange2(Start,dir)-cellCenterOffset);
		  holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir]));
		}
	      }

	      // compute the distance in grid points to the nearest boundary. 
	      int bDist=INT_MAX;
	      for( int dir=0; dir<numberOfDimensions; dir++ )
		bDist=min(bDist, localIndexBounds2(End,dir)-localIndexBounds2(Start,dir));
	      for( int dir=0; dir<numberOfDimensions; dir++ )
	      {
		bDist=min(bDist,abs(holeMarker(dir)-localIndexBounds2(Start,dir))
			  ,abs(holeMarker(dir)-localIndexBounds2(End  ,dir)));
	      }
	      if( info & 4 ) 
		fprintf(plogFile,"cutHoles: Jump in r, ib=%i, distance to nearest boundary = %i grid points\n",
			ib,bDist);
	      for( int dir=0; dir<numberOfDimensions; dir++ )
	      {
		holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
		if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
		{
		  if( info & 1 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
		    printf("cutHoles:WARNING: holeWidth is very large for this point, holeWidth=%i,"
			   " for point ib=%i(%i),m=%i, along axis=%i",holeWidth(dir,ib),ib,ib2,m,dir);
		  holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
		  if( info & 1 && numberOfHoleWidthWarnings<maxHoleWidthWarnings ) 
		  {
		    numberOfHoleWidthWarnings++;
		    printf(" -> reducing to %i\n",holeWidth(dir,ib));
		    if( numberOfHoleWidthWarnings==maxHoleWidthWarnings ) 
		      printf("cutHoles:INFO: I will stop printing `holeWidth very large' warnings!\n");
		  }
		}
	      }
	      holeOffset(0)=-1;  // no need to compute holeWidth below
	    }
	  }
	}
      } // end if  dr > .7 
			

 
      // **************************************************************************
      // ****** The holeWidth(.,ib) and holeOffset have NOW been assigned *********
      // ******  compute the actual holeWidth(.,ib)                       *********
      // **************************************************************************

      if( holeOffset(0) >=0 )
      {
	if( numberOfDimensions==2 )
	{
	  if( holeOffset(0)==0 )
	  { // boxes lie one above each other
	    holeWidth(1,ib)=max(holeWidth(1,ib),holeOffset(1)-1);
	  }
	  else if( holeOffset(1)==0 )
	  { // boxes are horizontal
	    holeWidth(0,ib)=max(holeWidth(0,ib),holeOffset(0)-1);
	  }
	  else
	  {
	    // holes are on a diagonal
	    if( info & 4 ) 
	      fprintf(plogFile,"    : holes are on a diagonal\n");

            for( int dir=0; dir<numberOfDimensions; dir++ )
	      holeWidth(dir,ib)=max(holeWidth(dir,ib),holeOffset(dir)+1);
	  }
	}
	else
	{
	  // 3D:
	  if( holeOffset(1)==0 && holeOffset(2)==0 )
	  {// boxes are horizontal
	    holeWidth(0,ib)=max(holeWidth(0,ib),holeOffset(0)-1);
	  }
	  else if( holeOffset(2)==0 && holeOffset(0)==0 )
	  {// boxes are vertical  
	    holeWidth(1,ib)=max(holeWidth(1,ib),holeOffset(1)-1);
	  }
	  else if( holeOffset(0)==0 && holeOffset(1)==0 )
	  {
	    holeWidth(2,ib)=max(holeWidth(2,ib),holeOffset(2)-1);
	  }
	  else
	  {
	    // holes are on a diagonal
	    for( int dir=0; dir<numberOfDimensions; dir++ )
              holeWidth(dir,ib)=max(holeWidth(dir,ib),holeOffset(dir)+1);
	  }
	}
      } // end if holeOffset(0) >=0 
			

    } // end -- previous point is inside the grid
		      

  } // end for m
  // *********************************************************************************

  return 0;
}
