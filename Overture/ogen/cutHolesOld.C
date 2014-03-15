#include "Overture.h"
#include "Ogen.h"
#include "display.h"






int Ogen::
projectToBoundary( CompositeGrid & cg,
		   const int & grid, 
		   const RealArray & r,
		   const int iv[3], 
		   const int ivp[3], 
		   real rv[3] )
// ===============================================================================================
// /Description:
//   Determine the intersection of the line segment r(iv) --> r(ivp) with the rBound bounding box.
// or return the r(ivp) if there is no intersection
// ===============================================================================================
{

  // look for solutions to  rBound = r(iv) + s [ r(ivp) - r(iv) ]
  // Choose the root with a minimum value for s in [0,1]
  const real eps = REAL_EPSILON*10.;
  real sMin=2., s, rv0[3], rv1[3];
  int dir;
  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
  {
    rv0[dir]=r(iv[0],iv[1],iv[2],dir);
    rv1[dir]=r(ivp[0],ivp[1],ivp[2],dir);

    real rDiff=rv1[dir]-rv0[dir];
    if( fabs(rDiff) > eps )
    {
      rDiff=1./rDiff;
      s=(rBound(Start,dir,grid)-rv0[dir])*rDiff;
      if( abs(s-.5)<=.51 )
      {
	sMin=min(sMin,s);
        continue;     // this is a possible root. s in [0,1]. The other choice is not possible.
      }
      else
      {
	s=(rBound(End,dir,grid)-rv0[dir])*rDiff;
        if( abs(s-.5)<.51 )
	  sMin=min(sMin,s);
      }
    }
  }
  if( fabs(sMin-.5)>.51 )
    sMin=1.;  // *wdh* 990421
  
  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
    rv[dir]=rv0[dir]+sMin*(rv1[dir]-rv0[dir]);
  return 0;
}

/* ------
bool Ogen::
sharedBoundaryPoint( i,r, cg, g,side,axis, g2,map2 ) 
{
  bool canInterpolate=FALSE;
  tol = max(g2.sharedBoundaryTolerance()(Range(0,1),Rx))*max(g2.gridSpacing()(Rx));
  if( max(fabs(r(i,Rx)-.5))<=.5+tol ) 
  {
    int normalDirection=-1;
    // Check to see if we are close to a physical boundary of another grid.
    int dir;
    for( dir=0; dir<cg.numberOfDimensions(); dir++ )
    {
      for( int side2=Start; side2<=End; side2++ )
      {
	if( g2.boundaryCondition()(side2,dir)>0 &&
	    map2.getTypeOfCoordinateSingularity(side2,dir)!=Mapping::polarSingularity &&
	    ( fabs(r(i,dir)-side2) < boundaryEps || 
	      ( g.sharedBoundaryFlag()(side,axis)!=0 && 
		g.sharedBoundaryFlag()(side,axis)==g2.sharedBoundaryFlag()(side2,dir) &&
		fabs(r(i,dir)-side2) < g2.sharedBoundaryTolerance()(side2,dir)*g2.gridSpacing()(dir) )
	      ) )
	{
	  // double check that the normals to the surfaces are both in the same direction
                    
	  i3=g2.indexRange(Start,axis3);
	  for( int ax=0; ax<cg.numberOfDimensions(); ax++ )
	  {
	    // iv : nearest point
	    if( ax!=dir )
	    {
	      iv[ax]=r(i,ax)/g2.gridSpacing(ax) + g2.indexRange(Start,ax) + cvShift;
	      iv[ax]=max(g2.dimension(Start,ax),min(g2.dimension(End,ax),iv[ax]));
	    }
	    else
	      iv[dir]=g2.gridIndexRange(side2,dir);    // use gir for cell centered grids
	  }
	  const RealArray & normal2 = g2.vertexBoundaryNormal(side2,dir);
	  // ****** we may have to get a better approximation to the normal if a corner is nearby ??
	  real cosAngle = sum(normal(ia(i,0),ia(i,1),ia(i,2),Rx)*normal2(i1,i2,i3,Rx));
	  if( cosAngle>.7 )  // .8 // if cosine of the angle between normals > ?? 
	  {
	    canInterpolate=TRUE; 
	    normalDirection=dir;
	    break;
	  }
	  else
	  {
	    if( cosAngle>.3 )
	    {
	      printf("sharedBoundaryPoint:WARNING: a boundary point on grid %s can interpolate from the"
		     " boundary of grid %s,\n"
		     "   but the cosine of the angle between the surface normals is %e (too small).\n"
		     "   No interpolation assumed. r=(%e,%e,%e)\n",
		     (const char*)map1.getName(Mapping::mappingName),
		     (const char*)map2.getName(Mapping::mappingName),cosAngle,
		     r(i,0),r(i,1),(cg.numberOfDimensions()==2 ? 0. : r(i,2)));
	    }
	  }
	}
      }
    }
		  
    if( canInterpolate )
    { // tangential directions to the boundary have a stricter tolerance
      for( dir=0; dir<cg.numberOfDimensions(); dir++ )
      {
	if( dir!=normalDirection && fabs(r(i,dir)-.5) >= .5+boundaryEps )
	{
	  canInterpolate=FALSE;
	  break;
	}
      }
    }
    
  }
  return canInterpolate;
}

--- */

int Ogen::
cutHolesNew(CompositeGrid & cg)
// =======================================================================================================
//
// For each physical boundary of each grid:
//     Find all points on other grids that are outside the boundary.
//
// Note: for a cell-centred grid we still use the vertex boundary values to cut holes.
//
// =======================================================================================================
{
  real time0=getCPU();
//  info |= 4;

  if( info & 4 ) printf("cutting holes with physical boundaries...\n");

  bool vectorize=TRUE;

  const int numberOfDimensions = cg.numberOfDimensions();
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range R, R1, Rx(0,numberOfDimensions-1);
  RealArray x,r,rr;
  RealArray x2(1,3), r2(1,3);
  IntegerArray ia,ia2;
  
  int i, jvOld[3], jpvOld[3];

  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int ipv[3], &i1p=ipv[0], &i2p=ipv[1], &i3p=ipv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
  int jpv[3], &j1p=jpv[0], &j2p=jpv[1], &j3p=jv[2];

  int maxNumberOfHolePoints=10000*cg.numberOfComponentGrids()*numberOfDimensions;
  numberOfHolePoints=0;
  holePoint.redim(maxNumberOfHolePoints,numberOfDimensions);
  
  const real boundaryAngleEps=.01;
  const real boundaryNormEps=0.; // *wdh* 980126 ** not needed since we double check//  1.e-2;
  IntegerArray holeOffset(numberOfDimensions);
  IntegerArray holeMarker(3);
  real rv[3]={0.,0.,0.};

  const real biggerBoundaryEps = sqrt(boundaryEps);

  int grid; 
  // for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  // cut holes with highest priority grids first since these will provide the first interpolation points
  for( grid=cg.numberOfComponentGrids()-1; grid>=0; grid-- )
  {
    MappedGrid & g = cg[grid];
    Mapping & map = g.mapping().getMapping();
    const RealArray & vertex = g.vertex();
    const RealArray & xr = g.vertexDerivative();
    IntegerArray & inverseGrid1 = cg.inverseGrid[grid];
    
    // shift this offset by epsilon to make sure we check the correct points in the k1,k2,k3 loop.
    const real cellCenterOffset= g.isAllCellCentered() ? .5-.5 : -.5;   // add -.5 to round to nearest point

    if( debug & 1 || info & 4 ) printf("cutting holes with grid: %s ...\n",
                   (const char*)g.mapping().getName(Mapping::mappingName));

    for( int axis=axis1; axis<numberOfDimensions; axis++ )
    {
      // axisp1 : must equal the most rapidly varying loop index of the triple (i1,i2,i3) loop below
      //          since we only save the holeWidth for the previous line.
      const int axisp1 = axis!=axis1 ? axis1 : axis2;  // we must make this axis1 if possible, otherwise axis2
      const int axisp2 = numberOfDimensions==2 ? axisp1 : (axis!=axis2 && axisp1!=axis2) ? axis2 : axis3;
      
      for( int side=Start; side<=End; side++ )
      {
        // Note: do not cut holes with singular sides
        if( g.boundaryCondition()(side,axis) > 0 && 
            map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity ) 
	{
	  // this side is a physical boundary
	  getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);   // note: use gridIndexRange
          Range R1(0,I1.length()*I2.length()*I3.length()-1);

	  // no: getBoundaryIndex(extendedGridIndexRange(g),side,axis,I1,I2,I3);   // note: use gridIndexRange
          bool firstTimeForThisBoundary=TRUE;
	  
          const RealDistributedArray & normal = g.vertexBoundaryNormal(side,axis); // *correct*
          // check all other grids  ********************************combine with interp on boundaries **********
          for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
	  {
            MappedGrid & g2 = cg[grid2];
	    if( grid2!=grid && cg.mayCutHoles(grid,grid2) && (isNew(grid) || isNew(grid2))
                &&  map.intersects( g2.mapping().getMapping(), side,axis,-1,-1,.1 ) )
	    {

              if( firstTimeForThisBoundary )
	      {
		firstTimeForThisBoundary=FALSE;
		r.redim(I1.length(),I2.length(),I3.length(),Rx);
		rr.redim(I1.length()*I2.length()*I3.length(),Rx);
		x.redim(I1.length()*I2.length()*I3.length(),Rx);
                ia.redim(I1.length()*I2.length()*I3.length(),7);  
	      }

	      RealArray & center2 = g2.center();
              IntegerArray & mask2 = g2.mask();
              const IntegerArray & indexRange2 = g2.indexRange();
              const IntegerArray & extendedIndexRange2 = g2.extendedIndexRange();
              IntegerArray & inverseGrid = cg.inverseGrid[grid2];
	      RealArray & rI = cg.inverseCoordinates[grid2];
	      
              // make a list of points in the bounding box of grid2
              // no need to cut holes with points that already interpolate from this grid!
              RealArray boundingBox;
              boundingBox=g2.mapping().getMapping().getBoundingBox(side,axis);    //   *** note: ghost lines not included
              real delta = .2*max( boundingBox(End,Rx)-boundingBox(Start,Rx) );
              for( dir=0; dir<numberOfDimensions; dir++ )
	      {
		boundingBox(Start,dir)-=delta;
		boundingBox(End  ,dir)+=delta;
	      }

              IntegerArray cutMask;
	      if( numberOfDimensions==2 )
                cutMask=(vertex(I1,I2,I3,axis1)>boundingBox(Start,axis1) && vertex(I1,I2,I3,axis1)<boundingBox(End,axis1)&&
			 vertex(I1,I2,I3,axis2)>boundingBox(Start,axis2) && vertex(I1,I2,I3,axis2)<boundingBox(End,axis2)&&
			  inverseGrid1(I1,I2,I3)!=grid2);
	      else
                cutMask=(vertex(I1,I2,I3,axis1)>boundingBox(Start,axis1) && vertex(I1,I2,I3,axis1)<boundingBox(End,axis1)&&
			 vertex(I1,I2,I3,axis2)>boundingBox(Start,axis2) && vertex(I1,I2,I3,axis2)<boundingBox(End,axis2)&&
			 vertex(I1,I2,I3,axis3)>boundingBox(Start,axis3) && vertex(I1,I2,I3,axis3)<boundingBox(End,axis3)&&
			 inverseGrid1(I1,I2,I3)!=grid2);


              int i=0;
              for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	      {
		for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		{
		  for( i1=I1Base; i1<=I1Bound; i1++ )
		  {
                    if( cutMask(i1,i2,i3) )
		    {
		      for( dir=numberOfDimensions; dir<3; dir++ )
                        x(i,dir)=vertex(i1,i2,i3,dir);
                      ia(i,0)=i1;
                      ia(i,1)=i2;
                      ia(i,2)=i3;
                      i++;
		    }
		  }
		}
	      }
              if( i==0 )
                continue;
	      int numberToCheck=i;
	      
              R1=Range(0,numberToCheck-1);

              real time1=getCPU();
	      if( useBoundaryAdjustment )
		adjustBoundary(cg,grid,grid2,ia(R1,Rx),x(R1,Rx));    // adjust boundary points on shared sides 

              g2.mapping().getMapping().inverseMap(x(R1,Rx),rr);

              r=Mapping::bogus;
              for( dir=0; dir<numberOfDimensions; dir++ )
  	        r(ia(R1,0),ia(R1,1),ia(R1,2),dir)=rr(R1,dir);
	      
              real time2=getCPU();

              if( debug & 1 || info & 4 ) printf("cut with (side,axis)=(%i,%i) : time to compute inverseMap on grid2=%s, "
                     "is %e (total=%e) (number of pts=%i)\n",
                     side,axis,(const char*)g2.mapping().getName(Mapping::mappingName),time2-time1,time2-totalTime,
                     numberToCheck);
	      
	      int dir;
	      for( dir=numberOfDimensions; dir<3; dir++ )
	      {
		jv[dir]=indexRange2(Start,dir);   // give default values 
		jpv[dir]=jv[dir];
	      }
              // we need to save the old boxes along the first tangential direction, axisp1
              Range It=Range(Iv[axisp1].getBase()-1,Iv[axisp1].getBound());
              IntegerArray holeCenter(Range(0,2),It), holeWidth(Range(0,2),It);
              holeWidth=-1;
	      
  	      for( dir=0; dir<3; dir++ )
	        holeCenter(dir,It)=indexRange2(Start,dir)-100; // bogus value means not a valid hole.

              const int indexRange00 = numberOfDimensions==2 ? indexRange2(Start,axisp1) :
		min( indexRange2(Start,axisp1),indexRange2(Start,axisp2));

              int numberCut=0;
	      
              // Compute the holeMask:
              //     holeMask(i1,i2,i3) = 0 : point is outside and not invertible
              //                        = 1 : point is inside
              //                        = 2 : point is outside but invertible
              //
              //                      -------------------------
              //                      |                       |      
              //                      |        grid2          |      
              //    holeMask          |                       |      
              //      --0---0---2---2---1---1---1---1---1---1---2---2---2---0---0---- cutting curve, grid
              //                      |                       |      
              //                      |                       |      
              //                      |                       |      
              //                      |                       |      
              //                      |                       |      
              //                      -------------------------


              IntegerArray holeMask(I1,I2,I3);
	      holeMask=0;
              real dr=0.;
              const int I1Base=I1.getBase(), I1Bound=I1.getBound();
              const int I2Base=I2.getBase(), I2Bound=I2.getBound();
              const int I3Base=I3.getBase(), I3Bound=I3.getBound();
              // loop over all points on the face that is cutting a hole
              for( i=0; i<numberToCheck; i++ )
	      {
		i1=ia(i,0);
		i2=ia(i,1);
		i3=ia(i,2);
		
		i1p=i1<I1Bound ? i1+1 : i1>I1Base ? i1-1 : i1;
		i2p=i2<I2Bound ? i2+1 : i2>I2Base ? i2-1 : i2;
		i3p=i3<I3Bound ? i3+1 : i3>I3Base ? i3-1 : i3;

		int ib = iv[axisp1];  // tangential marching direction
		// We need to include as 'inside' points that are close to the boundary
		// This is so we catch 'corners' that are cut off. Base the distance we
		// need to check on the tangential distance between cutting points.
		//               X
		//         o--o-/--o
		//         |  /
		//         |/
		//        /o 
		//      X  | 
		// avoid bogus points,
		if( numberOfDimensions==2 )
		{
		  if(r(i1p,i2p,i3p,axisp1)!=Mapping::bogus &&  rr(i,axisp1)!=Mapping::bogus )
		    dr=fabs(r(i1p,i2p,i3p,axisp1)-rr(i,axisp1));
		  else
		    dr=0.;
		}
		else
		{
		  if(r(i1p,i2p,i3p,axisp1)!=Mapping::bogus &&  rr(i,axisp1)!=Mapping::bogus &&
		     r(i1p,i2p,i3p,axisp2)!=Mapping::bogus &&  rr(i,axisp2)!=Mapping::bogus )
		    dr=max(fabs(r(i1p,i2p,i3p,axisp1)-rr(i,axisp1)),
			   fabs(r(i1p,i2p,i3p,axisp2)-rr(i,axisp2)));
		  else
		    dr=0.;
		}
		    
		if( rr(i,axis1)>rBound(Start,axis1,grid2)-dr && rr(i,axis1)<rBound(End,axis1,grid2)+dr &&
		    rr(i,axis2)>rBound(Start,axis2,grid2)-dr && rr(i,axis2)<rBound(End,axis2,grid2)+dr &&
		    ( g.numberOfDimensions()<3 || 
		      (rr(i,axis3)>rBound(Start,axis3,grid2)-dr && rr(i,axis3)<rBound(End,axis3,grid2)+dr )
		      )
		  )
		{

		  // check for shared sides **** why is this needed ??
		  // This is needed****
		  bool sharedSide=FALSE;
		  for( dir=0; dir<numberOfDimensions; dir++ )
		  {
		    if( ( g2.boundaryCondition()(Start,dir)>0 
			  && fabs(rr(i,dir)   )<g2.sharedBoundaryTolerance(Start,dir)*g2.gridSpacing(dir) 
			  && g2.sharedBoundaryFlag(0,dir)!=0 
			  && g2.sharedBoundaryFlag(0,dir)==g.sharedBoundaryFlag(side,axis) ) ||
			( g2.boundaryCondition()(End,dir)>0
			  && fabs(rr(i,dir)-1.)<g2.sharedBoundaryTolerance(Start,dir)*g2.gridSpacing(dir) 
			  && g2.sharedBoundaryFlag(1,dir)!=0 
			  && g2.sharedBoundaryFlag(1,dir)==g.sharedBoundaryFlag(side,axis) ) )
		    {
		      sharedSide=TRUE;
		      break;
		    }
		  }
		  if( sharedSide )
		  { 
		    // mark this point as "not inside" but invertible, we may need to use it for
		    // a neighbouring point.
		    holeMask(i1,i2,i3)=2;
		  }
		  else
		    holeMask(i1,i2,i3)=1;  // point is inside.

		  // holeMask(i1,i2,i3)=1;  // point is inside.

		}
		// else if( max(abs(r(i1,i2,i3,Rx)))<3. )
		else if( rr(i,0)!=Mapping::bogus )
		{
		  holeMask(i1,i2,i3)=2;  // mark this point as "not inside" but invertible
		}

	      } // end for i
	      

              // for any point on grid that can be interpolated, find points nearby on grid2 that are outside the
              // boundary and mark them as unused.

              if( info & 4 )
		display(holeMask,"holeMask (lives on the cutting face )","%2i");
		

              // loop over all points on the face that is cutting a hole
              for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
              for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
              for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
                int ib = iv[axisp1];  // tangential marching direction
                int ib2 = numberOfDimensions>2 ? iv[axisp2] : 0;  // second tangential marching direction (3D)
                if( holeMask(i1,i2,i3)==1 )
		{
                  if( info & 4 ) 
                    printf("------ cutHoles: process point ib=%i(ib2=%i) (%i,%i,%i) on (side,axis)=(%i,%i) holeMask==1 "
                           " r=(%6.2e,%6.2e,%6.2e)",ib,ib2,i1,i2,i3,side,axis,r(i1,i2,i3,0),r(i1,i2,i3,1),
                       numberOfDimensions==2 ? 0. : r(i1,i2,i3,0));
		  
                  // This point on the cutting curve is inside grid2
                
                  // Build a "box" of points on grid2 to check to see if they are inside or outside.
		  // jv : index of closest point to the interpolation point
                  //       (*NOTE* jv==(j1,j2,j3))
                  //             -----
                  //             |    |
                  //          -----1--------------------1------1
                  //             |    |
                  //             X-----
                  //           jv
		  for( dir=0; dir<numberOfDimensions; dir++ )
		  {
                    // note: cellCenterOffset will round to the nearest point. 
                    jv[dir]=(int)floor( r(i1,i2,i3,dir)/g2.gridSpacing(dir)+indexRange2(Start,dir)-cellCenterOffset );
                    holeMarker(dir)=jv[dir];
		  }
                  // 
                  // sequential boxes should overlap
                  //   o unless we cross a periodic boundary
                  //   o or unless we cross a boundary
                  // In 2D we just compare with the previous box.
                  // In 3D we also compare with the box for the point "below"
                  //          -----+----O----X---->
                  //               |    |    |
                  //          -----+----+----O----
		  
                  holeWidth(Rx,ib)=1; // by default check a stencil that extends by this amount in each direction.
                  bool skipThisPoint=0;
		  int initialPoint=0;
		  
                  // m : In 3D we need to check 2 possible previous points.
                  for( int m=0; m<g.numberOfDimensions()-1; m++ )
		  {
		    int axisT = m==0 ? axisp1 : axisp2;  // tangential direction.
		    int ibb =ib-1+m;   // i1-1 or i1

		    holeOffset=abs(holeMarker(Rx)-holeCenter(Rx,ibb));  // offset between this point and the last
		    if( max(holeOffset)==0 || max(holeOffset-holeWidth(Rx,ibb))<0  ) // ******
		    {
		      // skip this point, it is contained in the previous box (or boxes in 3D)
                      skipThisPoint++;
                      if( skipThisPoint>=g.numberOfDimensions()-1 )
		      {
                        if( info & 4 ) printf("  skip point ib=%i(%i)\n",ib,ib2);
		        break;
		      }
                      else
                        continue;
		    }
                    // ====================================================================================
                    // check the next point to see if it there
                    // If it is *NOT* there we must increase the size of this box.
                    int ivp[3]={i1,i2,i3};   // holds next point.
                    ivp[axisT]++;            // increment tangential direction.
                    if( ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])!=1 )
		    { // the next point is not in the grid
		      if( info & 4 ) 
			printf("  : m=%i, next point: r=(%6.2e,%6.2e) is not inside, holeMask(next)=%i\n",
			       m, r(ivp[0],ivp[1],ivp[2],0),r(ivp[0],ivp[1],ivp[2],1),holeMask(ivp[0],ivp[1],ivp[2]));
                      if( holeMask(ivp[0],ivp[1],ivp[2])!=0 )
		      {
                        // only use the next point if r does not change too much -- the next r value
                        // could be interpolating from another part of the grid
                        // projectToBoundary will find the closest intersection of the line segment
                        // with the rBound bounding box.
                        real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
                        if( dr<.3 && projectToBoundary(cg,grid2,r,iv,ivp,rv)==0 )
			{
			  for( dir=0; dir<numberOfDimensions; dir++ )
			  {
                            // note: cellCenterOffset will round to the nearest point. 
			    jpv[dir]=(int)floor( rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)-cellCenterOffset );
			    holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir])+1);
			  }
                          if( info & 4 ) 
                            printf("  : m=%i, next pt outside, intersection with bndy=(%6.2e,%6.2e,%6.2e) "
                                   "current holeWidth=(%i,%i,%i)\n",m,rv[0],rv[1],rv[2],
                                    holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 1 : holeWidth(2,ib));
			}
		      }
                      else
		      {
			if( info & 4 ) 
			  printf("  : holeMask=0 (far outside the grid) -- something is wrong here ? \n");
		      }
		      
                      // compute the distance in grid points to the nearest boundary. 
		      int bDist=min(extendedIndexRange2(End,Rx)-extendedIndexRange2(Start,Rx));
		      for( dir=0; dir<numberOfDimensions; dir++ )
		      {
			bDist=min(bDist,abs(holeMarker(dir)-extendedIndexRange2(Start,dir))
				  ,abs(holeMarker(dir)-extendedIndexRange2(End  ,dir)));
		      }
		      if( info & 4 ) 
                        printf("  : dist. to nearest boundary = %i grid points\n",bDist);
		      for( dir=0; dir<numberOfDimensions; dir++ )
		      {
			holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
			if( holeWidth(dir,ib) > max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
			{
			  if( info & 1 ) 
                            printf("cutHoles:WARNING: Final point holeWidth is very large for this point, "
                              "holeWidth=%i, for point ib=%i(ib2=%i), along axis=%i \n"
				   "  case: next pt on cutting surface not in grid, dist to nearest boundary=%i \n",
                                holeWidth(dir,ib),ib,ib2,dir,bDist);
			  holeWidth(dir,ib)=max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
			  if( info & 1 ) 
                            printf("        : something is wrong here. I am reducing the width to %i\n",
				 holeWidth(dir,ib));
			}
		      }
		    }

                    // ====================================================================================
                    // now check to see if the previous point is there
		    int ivm[3]={i1,i2,i3};   // holds previous point
		    ivm[axisT]--;
                    if( ivm[axisT] < Iv[axisT].getBase() || holeMask(ivm[0],ivm[1],ivm[2])!=1 )
		    {
                      // previous point is NOT inside this grid -- try to guess the box width in other ways.
                      if( ivm[axisT] < Iv[axisT].getBase() )
		      {
			// this is really the first point -- width=1 should do.
                        holeWidth(Rx,ib)=max(1,holeWidth(Rx,ib));
                        if( info & 4 ) 
			{
                          initialPoint++;
                          if( initialPoint>=numberOfDimensions-1 )
                            printf("  : m=%i, previous pt is outside , this is an INITIAL point, x=(%e,%e,%e)\n",
				   m,x(i1,i2,i3,0),x(i1,i2,i3,1),numberOfDimensions==2 ? 0. : x(i1,i2,i3,2));
			}
		      }
		      else
		      {
                        // this is not the first point, the boundary must have entered this grid.
 	                real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivm[0],ivm[1],ivm[2],Rx)));
                        if( ivm[axisT]>=Iv[axisT].getBase() && holeMask(ivm[0],ivm[1],ivm[2])==2 
                            && dr<.3 && projectToBoundary(cg,grid2,r,iv,ivm,rv)==0 )
			{
			  // previous point was invertible but must have been outside.
                          // determine its location index space.

                          if( info & 4 ) 
                            printf("  : m=%i, prev pt is outside but invert., intersect wth rBound =(%6.2e,%6.2e,%6.2e)\n",
			    m,rv[0],rv[1],rv[2]);
                          // only use the next point if r does not change too much -- the next r value
                          // could be interpolating from another part of the grid
			  for( dir=0; dir<numberOfDimensions; dir++ )
			  {
                            // note: cellCenterOffset will round to the nearest point. 
			    jpv[dir]=(int)floor( rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)-cellCenterOffset );
			    holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir])+1);
			    if( holeWidth(dir,ib) > max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
			    {
			      if( info & 1 ) 
                                printf("cutHoles:WARNING: holeWidth is very large for this point, holeWidth=%i,"
				     " for point ib=%i(%i),m=%i, along axis=%i \n",holeWidth(dir,ib),m,ib,ib2,dir);
			      holeWidth(dir,ib)=max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
			      if( info & 1 ) 
                                printf("        : something is wrong here. I am reducing the width to %i\n",
				     holeWidth(dir,ib));
			    }
			  }
			  if( info & 4 ) 
                            printf("  : m=%i, previous pt is useable, current holeWidth=(%i,%i,%i)\n",
				 m,holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 1 : holeWidth(2,ib));
			}
                        else 
			{
                          // There is no previous point. Base the width on the next point (if it is there) AND
                          // extend the width to the nearest boundary. *** this could go wrong maybe ?? ***
                          if( ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])!=0  )
			  {
                            real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
			    if( dr<.3 && projectToBoundary(cg,grid2,r,iv,ivp,rv)==0 )
			    { // the next point was invertible.
			      if( info & 4 ) 
				printf("  : m=%i, next pt is useable, next intersection with "
                                       "r-bndry =(%6.2e,%6.2e,%6.2e) \n",m,rv[0],rv[1],rv[2]);

			      for( dir=0; dir<numberOfDimensions; dir++ )
			      {
                                // note: cellCenterOffset will round to the nearest point. 
				jpv[dir] = (int)floor( rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)- 
                                         cellCenterOffset );
				holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir]));
			      }
			    }
			  }
                          // compute the distance in grid points to the nearest boundary. 
                          int bDist=min(extendedIndexRange2(End,Rx)-extendedIndexRange2(Start,Rx));
                          for( dir=0; dir<numberOfDimensions; dir++ )
			  {
                            bDist=min(bDist,abs(holeMarker(dir)-extendedIndexRange2(Start,dir))
				           ,abs(holeMarker(dir)-extendedIndexRange2(End  ,dir)));
			  }
			  if( info & 4 ) 
                            printf("  : (index) distance to nearest boundary = %i grid points\n",bDist);
                          for( dir=0; dir<numberOfDimensions; dir++ )
			  {
  			    holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
                            if( holeWidth(dir,ib) > max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
			    {
			      if( info & 1 ) 
                                printf("cutHoles:WARNING: holeWidth is very large for this point, holeWidth=%i,"
                                     " for point ib=%i(%i),m=%i, along axis=%i \n",holeWidth(dir,ib),ib,ib2,m,dir);
                              holeWidth(dir,ib)=max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
                              if( info & 1 ) 
                                printf("        : something is wrong here. I am reducing the width to %i\n",
				     holeWidth(dir,ib));
			    }
			  }
			}
		      }
		    }
		    else
		    {
		      real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivm[0],ivm[1],ivm[2],Rx)));
		      if( dr>.7 ) 
		      {
			// point jumps in r --- need to do something special
                        if( info & 4 ) 
                          printf("cutHoles:There has been a jump in r, ib=%(%i),im=%i,\n",ib,ib2,m);
                        for( dir=0; dir<numberOfDimensions; dir++ )
			{
			  if( fabs(r(i1,i2,i3,dir)-r(ivm[0],ivm[1],ivm[2],dir)) > .69 )
			  {
			    if( g2.isPeriodic(dir)==Mapping::functionPeriodic )
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
                                printf("cutHoles:jump in r but not on a periodic boundary!\n");
			      //  Base the width on the next point (if it is there) AND
			      // extend the width to the nearest boundary. *** this could go wrong maybe ?? ***
			      int ivp[3]={i1,i2,i3};   // holds next point.
                              ivp[axisT]=min(ivp[axisT]+1,Iv[axisT].getBound()); // increment tangential direction.
			      real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
			      if( holeMask(ivp[0],ivp[1],ivp[2])!=0 && dr<.3 && 
				  projectToBoundary(cg,grid2,r,iv,ivp,rv)==0 )
			      { // the next point was invertible.
				if( info & 4 ) 
				  printf("        : jump at ib=%i, next intersection with rBound =(%6.2e,%6.2e,%6.2e)\n",
					 ib, rv[0],rv[1],rv[2]);

				for( dir=0; dir<numberOfDimensions; dir++ )
				{
				  jpv[dir]=(int)floor(rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)-
                                    cellCenterOffset);
				  holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir]));
				}
			      }

			      // compute the distance in grid points to the nearest boundary. 
			      int bDist=min(extendedIndexRange2(End,Rx)-extendedIndexRange2(Start,Rx));
			      for( dir=0; dir<numberOfDimensions; dir++ )
			      {
				bDist=min(bDist,abs(holeMarker(dir)-extendedIndexRange2(Start,dir))
					  ,abs(holeMarker(dir)-extendedIndexRange2(End  ,dir)));
			      }
			      if( info & 4 ) 
                                 printf("cutHoles: Jump in r, ib=%i, distance to nearest boundary = %i grid points\n",
				        ib,bDist);
			      for( dir=0; dir<numberOfDimensions; dir++ )
			      {
				holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
				if( holeWidth(dir,ib) > max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
				{
				  if( info & 1 ) 
                                    printf("cutHoles:WARNING: holeWidth is very large for this point, holeWidth=%i,"
					 " for point ib=%i, along axis=%i \n",holeWidth(dir,ib),ib,dir);
				  holeWidth(dir,ib)=max(5,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
				  if( info & 1 ) 
                                    printf("        : something is wrong here. I am reducing the width to %i\n",
					 holeWidth(dir,ib));
				}
			      }
			      holeOffset(0)=-1;  // no need to compute holeWidth below
			    }
			  }
			}
		      }
                      if( holeOffset(0) >=0 )
		      {
			if( numberOfDimensions==2 )
			{
			  if( holeOffset(0)==0 )
			  { // boxes lie one above each other of each other
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
                              printf("    : holes are on a diagonal\n");
			    holeWidth(Rx,ib)=max(holeWidth(Rx,ib),holeOffset(Rx)+1);
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
			    holeWidth(Rx,ib)=max(holeWidth(Rx,ib),holeOffset(Rx)+1);
			  }
			}
		      }
		    }
		  } // end for m
                  holeCenter(Rx,ib)=holeMarker(Rx);
		  if( skipThisPoint>=g.numberOfDimensions()-1 )
                    continue;
		  if( info & 4 ) 
                    printf("  *** : grid2=%i, point ib=%i(%i), r=(%6.2e,%6.2e,%6.2e), holeCenter=(%i,%i,%i), "
			   "width=(%i,%i,%i)\n",grid2,ib,ib2,
                         r(i1,i2,i3,0),r(i1,i2,i3,1),numberOfDimensions==2 ? 0. : r(i1,i2,i3,2),
                         holeCenter(0,ib),holeCenter(1,ib),numberOfDimensions==2 ? 0 : holeCenter(2,ib),
                         holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 0 : holeWidth(2,ib));
		  for( dir=0; dir<numberOfDimensions; dir++ )
		  {
                    jpv[dir]=min(jv[dir]+holeWidth(dir,ib),extendedIndexRange2(End,dir));
                    jv[dir] =max(jv[dir]-holeWidth(dir,ib),extendedIndexRange2(Start,dir));
		  }
		   // make a list of boundary points and their inverseMap images
                  // first make sure there is enough space:
                  int numberOfNewPoints=(jpv[0]-j1+1)*(jpv[1]-j2+1)*(jpv[2]-j3+1);  
		  if( ia.getLength(0)<=numberCut+numberOfNewPoints )
		    ia.resize((ia.getLength(0)+numberOfNewPoints)*2,7);


                  for( int k3=j3; k3<=jpv[2]; k3++ )
                  for( int k2=j2; k2<=jpv[1]; k2++ )
                  for( int k1=j1; k1<=jpv[0]; k1++ )
 		  {
           	    if( mask2(k1,k2,k3)!=0 && 
                        inverseGrid(k1,k2,k3)!=grid &&  // *wdh* added 990426
                        !( mask2(k1,k2,k3) & MappedGrid::ISinteriorBoundaryPoint) )  // is this correct?
		    {
		      ia(numberCut,0)=k1;
		      ia(numberCut,1)=k2;
		      ia(numberCut,2)=k3;
		      ia(numberCut,3)=mask2(k1,k2,k3);

		      ia(numberCut,4)=i1;  // save these values for double checking points that cannot be inverted.
		      ia(numberCut,5)=i2;
		      ia(numberCut,6)=i3;

		      numberCut++;
		      mask2(k1,k2,k3)=0;

                      // printf(" Cutting a hole on grid2=%i at (%i,%i,%i) \n",grid2,k1,k2,k3);
		      
		    }
		  }
		}
                else 
		{
                  // *** this point is not inside.
                  // If the previous point was inside 
/* -----                  
                  for( int m=0; m<g.numberOfDimensions()-1; m++ )
		  {
		    int axisT = (axis +1+m) % numberOfDimensions;  // tangential direction.
		    int ibb =ib-1+m;   // i1-1 or i1
                    if( holeCenter(0,ibb)>=indexRange00 )
		    {
                      // previous 
		    }
		  }
---- */
		  holeCenter(0,ib)=indexRange00-1;  // mark this point as unused
		}
	      }  // for i1,i2,i3

              if( vectorize && numberCut > 0 )
	      {
		// now double check the points we cut out
		R=Range(0,numberCut-1);
		x2.redim(R,Rx);
		r2.redim(R,Rx);
	      
		for( dir=0; dir<numberOfDimensions; dir++ )
		  x2(R,dir)=center2(ia(R,0),ia(R,1),ia(R,2),dir);

                if( useBoundaryAdjustment )
		{
                  adjustBoundary(cg,grid2,grid,ia(R,Rx),x2(R,Rx));  // adjust boundary points on shared sides 
		}
		
		map.inverseMap(x2(R,Rx),r2(R,Rx));

                if( FALSE && debug & 1 )
		{
                  char buff[80];
		  display(r2(R,Rx),sPrintF(buff,"Here are the r2 coordinates on grid=%i\n",grid));
		}
		

		if( debug & 32 )
		{
                  for( i=R.getBase(); i<=R.getBound(); i++ )
		  {
		    if( fabs(r2(i,axis  )-.5) <= .5 +boundaryEps
			|| r2(i,axisp1)<rBound(Start,axisp1,grid) || r2(i,axisp1)>rBound(End,axisp1,grid) 
			|| r2(i,axisp2)<rBound(Start,axisp2,grid) || r2(i,axisp2)>rBound(End,axisp2,grid)  )
		    {
                      fprintf(logFile,"cutHoles: un-cutting a hole pt: grid2=%i (%s), ia=(%i,%i,%i), "
			      "r2(axis)=%7.3e, r2(axisp1)=%7.3e, r2(axisp2)=%7.3e\n",grid2,
 			      (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
                              r2(i,axis  ),r2(i,axisp1),r2(i,axisp2));
		    }
		  }
		}
		
                if( g.numberOfDimensions()==2 )
		{
		  // where( fabs(r2(R,axis1)-.5) <= .5+boundaryEps && fabs(r2(R,axis2)-.5) <= .5+boundaryEps  )
		  where( r2(R,axis1)>=rBound(Start,axis1,grid) && r2(R,axis1)<=rBound(End,axis1,grid) &&
                         r2(R,axis2)>=rBound(Start,axis2,grid) && r2(R,axis2)<=rBound(End,axis2,grid) )
		  { // we can interpolate
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=MappedGrid::ISinterpolationPoint; 
                    inverseGrid(ia(R,0),ia(R,1),ia(R,2))= grid;  
		    for( dir=0; dir<numberOfDimensions; dir++ )
		      rI(ia(R,0),ia(R,1),ia(R,2),dir)=r2(R,dir);
  		  }
  		  elsewhere( fabs(r2(R,axis))<10. &&       // point was invertible
                           ( fabs(r2(R,axis  )-.5) <= .5+boundaryEps
                             || (side==0 && r2(R,axis  )>=0. ) ||(side==1 && r2(R,axis  )<=1. )
  			   || r2(R,axisp1)<=rBound(Start,axisp1,grid) || r2(R,axisp1)>=rBound(End,axisp1,grid)) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=ia(R,3); // MappedGrid::ISdiscretizationPoint; // reset these values
  		  }
		}
		else  // 3d
		{
		  where( r2(R,axis1)>=rBound(Start,axis1,grid) && r2(R,axis1)<=rBound(End,axis1,grid) &&
                         r2(R,axis2)>=rBound(Start,axis2,grid) && r2(R,axis2)<=rBound(End,axis2,grid) &&
                         r2(R,axis3)>=rBound(Start,axis3,grid) && r2(R,axis3)<=rBound(End,axis3,grid) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=MappedGrid::ISinterpolationPoint; 
                    inverseGrid(ia(R,0),ia(R,1),ia(R,2))= grid;  
		    for( dir=0; dir<numberOfDimensions; dir++ )
		      rI(ia(R,0),ia(R,1),ia(R,2),dir)=r2(R,dir);
  		  }
		  elsewhere( fabs(r2(R,axis))<2. &&         // point was invertible
                         (fabs(r2(R,axis  )-.5) <= .5+boundaryEps 
                           || (side==0 && r2(R,axis  )>=0. ) ||(side==1 && r2(R,axis  )<=1. )
			   || r2(R,axisp1)<=rBound(Start,axisp1,grid) || r2(R,axisp1)>=rBound(End,axisp1,grid)
			   || r2(R,axisp2)<=rBound(Start,axisp2,grid) || r2(R,axisp2)>=rBound(End,axisp2,grid)) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=ia(R,3); // MappedGrid::ISdiscretizationPoint; // reset these values
  		  }
		}
                for( int i=0; i<numberCut; i++ )
		{
                  // need to check for shared sides here !!??
/* ------
                  if( sharedBoundaryPoint( r2(i,Rx), cg,grid,side,axis, grid2 ) )
		  {
		    printf("cutHoles: hole not cut as it is on a shared side\n");
                               "          No hole cut for point (%i,%i,%i) on grid=%i by grid=%i \n",
				 ia(i,0),ia(i,1),ia(i,2),grid2,grid);
		    mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3); 
		  }
                  else 
------ */
                  if( FALSE )
		  {
		    if( fabs(r2(i,axis  )-.5) <= .5+.02 && mask2(ia(i,0),ia(i,1),ia(i,2))==0 )
		    {
		      printf("**cutHoles: A point was cut near the boundary, (%i,%i,%i) on grid=%i by grid=%i "
			     " r=(%e,%e,%e) \n",
			     ia(i,0),ia(i,1),ia(i,2),grid2,grid,r2(i,0),r2(i,1),numberOfDimensions==2 ? 0. :
			     r2(i,2));
		      if( ia(i,3)<0 )
			mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3); 
		    }
		  }
                  if( FALSE && ia(i,3)<0 && inverseGrid(ia(i,0),ia(i,1),ia(i,2))==grid )
		  {
                    // do not cut a hole at a point that could already be interpolated from this grid!
                    mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3); 
                    printf("++cutHoles: un-cutting a hole (%i,%i,%i) on grid=%i by grid=%i. \n"
                           "This point could interpolate\n",ia(i,0),ia(i,1),ia(i,2),grid2,grid);
		  }
    
                  if( r2(i,axis)<10. && mask2(ia(i,0),ia(i,1),ia(i,2))==0 && 
                      fabs(r2(i,axis)-.5)<.5+biggerBoundaryEps &&
                      ( fabs( r2(i,axisp1)-.5) > .5 || (numberOfDimensions==3 && fabs( r2(i,axisp2)-.5) > .5 ) ) )
		  {
		    // invertible, outside [0,1] in tangential direction and close enough in the normal
                    // We need this since points outside [0,1] may not be inverted as accurately by Newton.
                    printf("++cutHoles: un-cutting a hole (%i,%i,%i) on grid=%i by grid=%i. r=(%e,%e,%e)  "
                           "This point is close to the boundary and outside [0,1] in tangent directions\n",
                           ia(i,0),ia(i,1),ia(i,2),grid2,grid, r2(i,0),r2(i,1),numberOfDimensions==2 ? 0. : r2(i,2));
  		    mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);  // reset this value
		  }
		  else if( r2(i,axis)>9. )  // NOT invertible
		  {
		    // if the point was NOT invertible then we double check to see it is outside of the boundary
                    // Estimate the r location for the point using
                    //        r = r(boundary) + dr
                    //        dr = [ dr/dx ] * dx       
                    //        dx = vector from boundary point jv to the point on the other grid, x2
                    jv[0]=ia(i,4); jv[1]=ia(i,5); jv[2]=ia(i,6);
                    real re[3], dx[3], det;
                    int ax;
                    for( ax=0; ax<numberOfDimensions; ax++ )
                      dx[ax]=x2(i,ax)-x(j1,j2,j3,ax);

#define XR(m,n) xr((j1),(j2),(j3),(m)+(n)*numberOfDimensions)

                    if( numberOfDimensions==2 )
		    {
                      real det = XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0);
                      if( det!=0. )
		      {
                        det=1./det;
    		        re[0]=(  XR(1,1)*dx[0]-XR(0,1)*dx[1] )*det;
		        re[1]=( -XR(1,0)*dx[0]+XR(0,0)*dx[1] )*det;
		      }
		      else
		      { // if the jacobian is singular
                        printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
                        re[0]=re[1]=0.;
                        re[axis]=.1*(2*side-1); // move point outside the grid
		      }
                      re[2]=0.;
		    }
		    else
		    {
		      det = (XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0))*XR(2,2) +
			    (XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1))*XR(2,0) +
			    (XR(0,2)*XR(1,0)-XR(0,0)*XR(1,2))*XR(2,1);
                      if( det!=0. )
		      {
                        det=1./det;
			re[0]=( (XR(1,1)*XR(2,2)-XR(2,1)*XR(1,2))*dx[0]+
				(XR(2,1)*XR(0,2)-XR(0,1)*XR(2,2))*dx[1]+
				(XR(0,1)*XR(1,2)-XR(1,1)*XR(0,2))*dx[2] )*det;
		      
			re[1]=( (XR(1,2)*XR(2,0)-XR(2,2)*XR(1,0))*dx[0]+
				(XR(2,2)*XR(0,0)-XR(0,2)*XR(2,0))*dx[1]+
				(XR(0,2)*XR(1,0)-XR(1,2)*XR(0,0))*dx[2] )*det;
		      
			re[2]=( (XR(1,0)*XR(2,1)-XR(2,0)*XR(1,1))*dx[0]+
				(XR(2,0)*XR(0,1)-XR(0,0)*XR(2,1))*dx[1]+
				(XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1))*dx[2] )*det;
			
		      }
		      else
		      { // if the jacobian is singular
                        printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
                        re[0]=re[1]=re[2]=0.;
                        re[axis]=.1*(2*side-1);
		      }
		      
		    }
#undef XR
                    for( ax=0; ax<numberOfDimensions; ax++ )
                      re[ax]+=jv[ax]*g.gridSpacing(ax);
		    
                    // do not cut a hole if the point is
                    //     1. inside in the normal direction
                    // or  2. on the wrong side in the normal direction
                    // or  3. outside in the tangential direction.
                    if( fabs(re[axis]-.5) <= .5+boundaryEps 
			 || (side==0 && re[axis  ]>=0. ) ||(side==1 && re[axis  ]<=1. )
			 || re[axisp1]<=rBound(Start,axisp1,grid) || re[axisp1]>=rBound(End,axisp1,grid)
			 || re[axisp2]<=rBound(Start,axisp2,grid) || re[axisp2]>=rBound(End,axisp2,grid) )
		    {
                      if( debug & 4 )
		      {
			printf("cutHoles: Non-invertible point: estimated r=(%7.2e,%7.2e,%7.2e) is",
			       re[0],re[1],re[2]);
			if(fabs(re[axis]-.5) <= .5+boundaryEps ) 
			  printf("   inside in the normal direction! No hole cut. ****** \n");
			else
			  printf(" outside in the tangential tangential direction. No hole cut. \n");
			
		      }
		      mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
		    }
                    else
		    {
		      real cosAngle =0.;
		      real dist1=0., dist2=0.;
		      jpv[0]=j1; jpv[1]=j2; jpv[2]=j3;
		      jpv[axis]=g.gridIndexRange(1-side,axis);  // opposite boundary
		      for( int ax=0; ax<numberOfDimensions; ax++ )
		      {
			cosAngle += normal(j1,j2,j3,ax)*dx[ax]; //  (x2(i,ax)-x(j1,j2,j3,ax));
			dist1+=SQR(x(j1,j2,j3,ax)-vertex(j1p,j2p,j3p,ax));    // approx distance to opposite boundary
			dist2+=SQR(dx[ax]);            // distance from boundary to potential hole point
		      }
                      cosAngle/= SQRT(max(REAL_MIN,dist2));
		      const real maxCosAngle=.0;  
                      const real maxDistFactor=.9;
		      // we do not cut a hole if the cosine of the angle between normals < maxCosAngle or if
		      // the distance to the potential hole point is greater than the distance to the opposite boundary
		      // 
		      if( cosAngle < maxCosAngle || dist2>dist1*maxDistFactor )  // if cosine of the angle between normals > ?? 
		      {
			if( debug & 1 )
			{
			  printf("cutHoles: no hole cut for the non-invertible point (%i,%i,%i) on grid=%i by grid=%i ",
				 ia(i,0),ia(i,1),ia(i,2),grid2,grid);
			  if( cosAngle < maxCosAngle )
			    printf("since n.n=%7.2e <%7.2e \n",cosAngle,maxCosAngle);
			  else
			    printf("since dist2=%7.2e > dist1*.9=%7.2e\n",dist2,dist1*maxDistFactor);
			}
			mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
		      }
/* -----
                      else if( cosAngle<.1 )
		      {
                         printf("cutHoles:WARNING: hole cut for the non-invertible point (%i,%i,%i) "
                                "on grid=%i by grid=%i \n"
                                "                   BUT cosine of angle between normals=%e \n",
                           ia(i,0),ia(i,1),ia(i,2),grid2,grid,cosAngle);
		      }
---- */		      
		    }
		  }
                  if( debug & 16 && numberOfDimensions==3 )
		  {
		    fprintf(logFile,"cutHoles: grid=%s, grid2=%s pt=(%i,%i,%i) r2=(%6.2e,%6.2e,%6.2e) mask=%i\n",
                       (const char*)g.mapping().getName(Mapping::mappingName),
                       (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
			    r2(i,axis1),r2(i,axis2),r2(i,axis3),mask2(ia(i,0),ia(i,1),ia(i,2)));
                    fprintf(logFile,"          x=(%6.2e,%6.2e,%6.2e) boundary shift=(%6.2e,%6.2e,%6.2e) \n",
                            x2(i,axis1),x2(i,axis2),x2(i,axis3),
                            fabs(x2(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)),
                            fabs(x2(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)),
                            fabs(x2(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)) );
		  }
		  if( mask2(ia(i,0),ia(i,1),ia(i,2))==0 )
		  {
		    
		    for( dir=0; dir<numberOfDimensions; dir++ )
		    {
		      holePoint(numberOfHolePoints,dir) = x2(i,dir);
		    }
		    numberOfHolePoints++;
		    assert( numberOfHolePoints < maxNumberOfHolePoints );
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    
  }
  // we need to set values in the inverseGrid array to -1 at all unused points *** can this be done else where ????
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & g = cg[grid];
    const IntegerArray & mask = g.mask();

    g.mask().periodicUpdate();   // ****

    IntegerArray & inverseGrid = cg.inverseGrid[grid];
    getIndex(extendedGridIndexRange(g),I1,I2,I3);  
    where( mask(I1,I2,I3)==0 )
      inverseGrid(I1,I2,I3)=-1;
  }
  

  real time=getCPU();
  if( info & 2 ) 
    printf(" time to cut holes........................................%e (total=%e)\n",time-time0,time-totalTime);
  timeCutHoles=time-time0;
  
  return numberOfHolePoints;
}



int Ogen::
removeExteriorPointsNew(CompositeGrid & cg, 
		     const bool boundariesHaveCutHoles /* = FALSE */ )
// ============================================================================================================
// /Description:
//   Once the hole boundary has been determined sweep out all remaining hole points.
//  This routine assumes that the boundary of the hole partitions the domain into
//  separate regions. Points inside should be bounded by a layer of interpolation points, mask(i1,i2,i3)<0,
//   and points outside should have a layer of holes, mask(i1,i2,i3)==0. Thus this routine
//  will look for places where interpolation points are next to hole points. This will signal
//  the start or end of the hole region.
//  
// ============================================================================================================
{
  real time0=getCPU();
  
  if( info & 4 ) printf("removing exterior points by sweeping...\n");

  int grid;
  

  Index Iv[3];
  Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range R,Rx(0,cg.numberOfDimensions()-1), I;
  RealArray x;
  IntegerArray ia, crossings;
  int i;
  int i1,i2,i3;  

  // *wdh* 981001 int maxNumberOfHolePoints=10000*cg.numberOfComponentGrids()*cg.numberOfDimensions();
  // estimate the total number of hole points in terms of the total number of grid points.
  // guess that the number of holes points is proportional to the surface area of the grid boundaries.  
  int numberOfSurfacePoints=0;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    numberOfSurfacePoints+=pow(cg[grid].mask().elementCount(),(cg.numberOfDimensions()-1.)/cg.numberOfDimensions());
  
  int maxNumberOfHolePoints=numberOfSurfacePoints*cg.numberOfDimensions()*2;

  if( holePoint.getLength(0)<maxNumberOfHolePoints )
    holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
  
  
  // For cell centred grids copy the mask from the last cell to the first ghost cell as
  // this info is used by the ray-tracing algorithm
  if( cg[0].isAllCellCentered() )
  {
    Index I1g,I2g,I3g;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & g = cg[grid];
      IntegerArray & mask = g.mask();
      if( g.isAllCellCentered() )
      {
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
          if( g.boundaryCondition()(End,axis)>0 )
	  {
	    getGhostIndex(g.gridIndexRange(),End,axis,I1 ,I2 ,I3 ,-1);  // last row of interior cells
	    getGhostIndex(g.gridIndexRange(),End,axis,I1g,I2g,I3g,0);   // first row of ghost cells
	    mask(I1g,I2g,I3g)=mask(I1,I2,I3);
	  }
	}
      }
    }
  }


  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & g = cg[grid];
    IntegerArray & mask = g.mask();
    RealArray & center = g.center();

    if( info & 4 ) 
      printf("removing points on grid %s... \n",(const char*)g.mapping().getName(Mapping::mappingName));

    int axis;
    getIndex(extendedGridIndexRange(g),I1,I2,I3);  
    // =====================================================================================
	  
    ia.redim(SQR(max(I1.length(),I2.length(),I3.length())),3);
	  
    bool done=FALSE;
	  
    const int I1Base=I1.getBase(), I1Bound=I1.getBound();
    const int I2Base=I2.getBase(), I2Bound=I2.getBound();
    const int I3Base=I3.getBase(), I3Bound=I3.getBound();

    int mask0;
    
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	for( i1=I1Base+1; i1<=I1Bound; i1++ )
	{
          mask0=mask(i1-1,i2,i3);
          while( i1<I1Bound && mask(i1,i2,i3)==mask0 )
            i1++;
	  if( mask(i1-1,i2,i3)<0 && mask(i1,i2,i3)==0 )  
	  { // sweep to the right
	    i1++;
	    while( i1<=I1Bound && mask(i1,i2,i3)>=0 )
	    {
	      if( mask(i1,i2,i3)>0 )
	      {
		mask(i1,i2,i3)=0;
		for( axis=0; axis<cg.numberOfDimensions(); axis++ )
		  holePoint(numberOfHolePoints,axis) = center(i1,i2,i3,axis);
		numberOfHolePoints++;
		if( numberOfHolePoints>=maxNumberOfHolePoints )
		{
		  maxNumberOfHolePoints*=1.5;
		  holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
		}
	      }
	      i1++;
	    }
	  }
	  else if( mask(i1-1,i2,i3)==0 && mask(i1,i2,i3)<=0 )
	  { // sweep to left
	    // there may be initial points that we need to sweep out from right to left
	    int i=i1-2;
	    while( i>=I1Base && mask(i,i2,i3)>=0 )
	    {
	      if( mask(i,i2,i3)>0 )
	      {
		mask(i,i2,i3)=0;
		for( axis=0; axis<cg.numberOfDimensions(); axis++ )
		  holePoint(numberOfHolePoints,axis) = center(i,i2,i3,axis);
		numberOfHolePoints++;
		if( numberOfHolePoints>=maxNumberOfHolePoints )
		{
		  maxNumberOfHolePoints*=1.5;
		  holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
		  printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			 maxNumberOfHolePoints);
		}
	      }
	      i--;
	    }
	  }
	}
      }
    }

    // sweep in the i2 direction
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )   // sweep i2
    {
      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
	for( i2=I2Base+1; i2<=I2Bound; i2++ )
	{
          mask0=mask(i1,i2-1,i3);
          while( i2<I2Bound && mask(i1,i2,i3)==mask0 )
            i2++;
	  if( mask(i1,i2-1,i3)==0 && mask(i1,i2,i3)>0 )
	  { // sweep up
	    while( i2<=I2Bound && mask(i1,i2,i3)>0 )
	    {
	      mask(i1,i2,i3)=0;
	      for( axis=0; axis<cg.numberOfDimensions(); axis++ )
		holePoint(numberOfHolePoints,axis) = center(i1,i2,i3,axis);
	      numberOfHolePoints++;
	      if( numberOfHolePoints>=maxNumberOfHolePoints )
	      {
		maxNumberOfHolePoints*=1.5;
		holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
		printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
		       maxNumberOfHolePoints);
	      }
	      i2++;
	    }
	  }
	  else if( mask(i1,i2-1,i3)>0 && mask(i1,i2,i3)==0 )
	  {
	    // sweep down
	    int i=i2-1;
	    while( i>=I2Base && mask(i1,i,i3)>0 )
	    {
	      mask(i1,i,i3)=0;
	      for( axis=0; axis<cg.numberOfDimensions(); axis++ )
		holePoint(numberOfHolePoints,axis) = center(i1,i,i3,axis);
	      numberOfHolePoints++;
	      if( numberOfHolePoints>=maxNumberOfHolePoints )
	      {
		maxNumberOfHolePoints*=1.5;
		holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
		printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
		       maxNumberOfHolePoints);
	      }
	      i--;
	    }
	  }
	}
      }
    }
    
    // sweep in the i3 direction
    if( cg.numberOfDimensions()==3 )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )   // sweep i3
      {
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  for( i3=I3Base+1; i3<=I3Bound; i3++ )
	  {
	    mask0=mask(i1,i2,i3-1);
	    while( i3<I3Bound && mask(i1,i2,i3)==mask0 )
	      i3++;
	    if( mask(i1,i2,i3-1)==0 && mask(i1,i2,i3)>0 )
	    { // sweep up
	      while( i3<=I3Bound && mask(i1,i2,i3)>0 )
	      {
		mask(i1,i2,i3)=0;
		for( axis=0; axis<cg.numberOfDimensions(); axis++ )
		  holePoint(numberOfHolePoints,axis) = center(i1,i2,i3,axis);
		numberOfHolePoints++;
		if( numberOfHolePoints>=maxNumberOfHolePoints )
		{
		  maxNumberOfHolePoints*=1.5;
		  holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
		  printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			 maxNumberOfHolePoints);
		}
		i3++;
	      }
	    }
	    else if( mask(i1,i2,i3-1)>0 && mask(i1,i2,i3)==0 )
	    {
	      // sweep down
	      int i=i3-1;
	      while( i>=I3Base && mask(i1,i2,i)>0 )
	      {
		mask(i1,i2,i)=0;
		for( axis=0; axis<cg.numberOfDimensions(); axis++ )
		  holePoint(numberOfHolePoints,axis) = center(i1,i2,i,axis);
		numberOfHolePoints++;
		if( numberOfHolePoints>=maxNumberOfHolePoints )
		{
		  maxNumberOfHolePoints*=1.5;
		  holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
		  printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			 maxNumberOfHolePoints);
		}
		i--;
	      }
	    }
	  }
	}
      }
    }
    

  }  // for grid
  

real time=getCPU();
  if( info & 2 ) 
    printf(" time to remove exterior points...........................%e (total=%e)\n",time-time0,time-totalTime);
  timeRemoveExteriorPoints=time-time0;
  

  return numberOfHolePoints;
}




int Ogen::
cutHoles(CompositeGrid & cg)
// =======================================================================================================
// **** this was a new try at cutting holes ****
//
// /Description:
//    Here is the newest hole cutting algorithm.
//
// For each physical boundary of each grid:
//     Find all points on other grids that are outside the boundary.
//
// Note: for a cell-centred grid we still use the vertex boundary values to cut holes.
//
// =======================================================================================================
{
  real time0=getCPU();
//  info |= 4;

  if( info & 4 ) printf("cutting holes with physical boundaries...\n");

  const int numberOfDimensions = cg.numberOfDimensions();
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  Range R, R1, Rx(0,numberOfDimensions-1);
//  RealArray x,r;
  RealArray xx(1,3), r2(1,3);
  IntegerArray ia,ia2;
  
  int i, jvOld[3], jvpOld[3];
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int ipv[3], &i1p=ipv[0], &i2p=ipv[1], &i3p=ipv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
  int jpv[3], &j1p=jpv[0], &j2p=jpv[1], &j3p=jv[2];
  int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];
  real xv[3], &x0=xv[0], &x1=xv[1], &x2=xv[2];
  
  int maxNumberOfHolePoints=10000*cg.numberOfComponentGrids()*numberOfDimensions;
  numberOfHolePoints=0;
  holePoint.redim(maxNumberOfHolePoints,numberOfDimensions);
  
  const real boundaryAngleEps=.01;
  const real boundaryNormEps=0.; // *wdh* 980126 ** not needed since we double check//  1.e-2;
  IntegerArray holeOffset(numberOfDimensions);
  IntegerArray holeMarker(3);
  real rv[3]={0.,0.,0.};
  

  const real biggerBoundaryEps = sqrt(boundaryEps);

  int grid,dir;
  real timeForBinarySearch=0.;
  real timeForCollection=0.;
  real timeForInverseMap=0.;
  int numberOfBinarySearches=0;
  
  // cut holes with highest priority grids first since these will provide the first interpolation points
  for( grid=cg.numberOfComponentGrids()-1; grid>=0; grid-- )
  {
    MappedGrid & g = cg[grid];
    Mapping & map = g.mapping().getMapping();
    const RealArray & vertex = g.vertex();
    const RealArray & xr = g.vertexDerivative();
    
    for( dir=numberOfDimensions; dir<3; dir++ )
    {
      jv[dir]=g.indexRange(Start,dir);   // give default values 
      jpv[dir]=jv[dir];
    }
    // shift this offset by epsilon to make sure we check the correct points in the k1,k2,k3 loop.
    const real cellCenterOffset= g.isAllCellCentered() ? .5-.5 : -.5;   // add -.5 to round to nearest point

    if( debug & 1 || info & 4 ) printf("cutting holes with grid: %s ...\n",
                   (const char*)g.mapping().getName(Mapping::mappingName));

    for( int axis=axis1; axis<numberOfDimensions; axis++ )
    {
      // axisp1 : must equal the most rapidly varying loop index of the triple (i1,i2,i3) loop below
      //          since we only save the holeWidth for the previous line.
      const int axisp1 = axis!=axis1 ? axis1 : axis2;  // we must make this axis1 if possible, otherwise axis2
      const int axisp2 = numberOfDimensions==2 ? axisp1 : (axis!=axis2 && axisp1!=axis2) ? axis2 : axis3;
      
      for( int side=Start; side<=End; side++ )
      {
        // Note: do not cut holes with singular sides
        if( g.boundaryCondition()(side,axis) > 0 && 
            map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity ) 
	{
	  // this side is a physical boundary
	  getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);   // note: use gridIndexRange
          Range R1(0,I1.length()*I2.length()*I3.length()-1);

          // encode closest pt as :  nI0+i1+nI1*i2+nI2*i3
          int nI1=I1.length();
          int nI2=I1.length()*I2.length();
          int nI0=1-(I1.getBase()+nI1*I2.getBase()+nI2*I3.getBase());  // offset to make code positive
	  

	  // no: getBoundaryIndex(extendedGridIndexRange(g),side,axis,I1,I2,I3);   // note: use gridIndexRange
          bool firstTimeForThisBoundary=TRUE;
	  
          const RealDistributedArray & normal = g.vertexBoundaryNormal(side,axis); // *correct*
          // check all other grids  ********************************combine with interp on boundaries **********
          for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
	  {
            MappedGrid & g2 = cg[grid2];
	    if( grid2!=grid && cg.mayCutHoles(grid,grid2) && (isNew(grid) || isNew(grid2))
                &&  map.intersects( g2.mapping().getMapping(), side,axis,-1,-1,.1 ) )
	    {

              // This side intersects with g2
              if( debug & 1 || info & 4 ) printf("cut with (side,axis)=(%i,%i) in grid2=%s \n",
                     side,axis,(const char*)g2.mapping().getName(Mapping::mappingName));


	      RealArray & center2 = g2.center();
              IntegerArray & mask2 = g2.mask();
              const IntegerArray & indexRange2 = g2.indexRange();
              const IntegerArray & extendedIndexRange2 = g2.extendedIndexRange();
              IntegerArray & inverseGrid = cg.inverseGrid[grid2];
	      RealArray & rI = cg.inverseCoordinates[grid2];
	      

              real timeC=getCPU();

              // make a list of points on g2 that lie near the cutting face
              
              getIndex(extendedGridIndexRange(g2),J1,J2,J3);
              int numberCut=J1.getLength()*J2.getLength()*J3.getLength();
	      
              RealArray boundingBox;
              boundingBox=map.getBoundingBox(side,axis);    //   *** note: ghost lines not included
              // BoundingBox & tree = getBoundingBoxTree(side,axis);
	      // int numberOfBoxesToCheck=0;
              // int numberOfLevels=0;

              real delta = .2*max( boundingBox(End,Rx)-boundingBox(Start,Rx) );
              for( dir=0; dir<numberOfDimensions; dir++ )
	      {
		boundingBox(Start,dir)-=delta;
		boundingBox(End  ,dir)+=delta;
	      }
	      
	      // display(boundingBox,"Bounding box for this side");

              getIndex(extendedGridIndexRange(g2),J1,J2,J3,1);
              RealArray db(J1,J2,J3), minDist(J1,J2,J3);
	      //db =-1.;
              IntegerArray closeToBoundary(J1,J2,J3); // 0=unknown, >0 : close ,  <0 : not close
	      closeToBoundary=0;

              getIndex(extendedGridIndexRange(g2),J1,J2,J3);
	      

#define DIST(x, i1,i2,i3, j1,j2,j3) ( numberOfDimensions==2 ? \
                                 SQRT( SQR(x(i1,i2,i3,0)-x(j1,j2,j3,0))+ \
                                       SQR(x(i1,i2,i3,1)-x(j1,j2,j3,1)) ) : \
                                 SQRT( SQR(x(i1,i2,i3,0)-x(j1,j2,j3,0))+ \
                                       SQR(x(i1,i2,i3,1)-x(j1,j2,j3,1))+  \
                                       SQR(x(i1,i2,i3,2)-x(j1,j2,j3,2)) ) )
                                 
              ia.redim(numberCut,4);

	      const real distanceFactor=1.5;  // check points this many grid spacings away from the boundary
              for( dir=0; dir<3; dir++ )
                iv[dir]=g.gridIndexRange(side,dir);  // initial guess for closest pt on the face.
	      
	      int j=0;
              const int J1Bound = J1.getBound();
              for( j3=J3.getBase(); j3<=J3.getBound(); j3++ )
              for( j2=J2.getBase(); j2<=J2.getBound(); j2++ )
              for( j1=J1.getBase(); j1<=J1Bound; j1++ )
	      {
                x0 = center2(j1,j2,j3,0);
                x1 = center2(j1,j2,j3,1);
                x2 = center2(j1,j2,j3,numberOfDimensions-1);
		
                if( mask2(j1,j2,j3)!=0 && inverseGrid(j1,j2,j3)!=grid &&
                    x0>=boundingBox(Start,axis1) && x0<=boundingBox(End,axis1) &&
                    x1>=boundingBox(Start,axis2) && x1<=boundingBox(End,axis2) && (numberOfDimensions==2 || (
		    x2>=boundingBox(Start,axis3) && x2<=boundingBox(End,axis3))) )
		{
                  // point is inside the global bounding box. 
                  // Now check more refined boxes.
/* -----
                  bool checkThisPoint=numberOfBoxesToCheck>0;
		  for( int b=0; b<numberOfBoxesToCheck; b++ )
		  {
                    if( x0>=boundingBox[b](Start,axis1) && x0<=boundingBox[b](End,axis1) &&
			x1>=boundingBox[b](Start,axis2) && x1<=boundingBox[b](End,axis2) && (numberOfDimensions==2 || (
			x2>=boundingBox[b](Start,axis3) && x2<=boundingBox[b](End,axis3))) )
		    {
		      checkThisPoint=TRUE;
		      break;
		    }
		  }
		  if( !checkThisPoint )
                    break;
----- */
                  // fix bounds: and 3D

		  // j1m = j1>J1.getBase() ? j1-1 : j1;
                  // j1p = j1m+2< J1.getBound() ? j1m+2 :  
                  // dist1=dist(j1-1,j2,j3, j1+1,j2,j3)
                  minDist(j1,j2,j3)=0.;
                  int m;
                  k1=j1, k2=j2, k3=j3;
                  for( m=0; m<numberOfDimensions; m++ )
                  {
		    kv[m]= jv[m] < Jv[m].getBound() ? jv[m]+1 : jv[m]-1; 
                    minDist(j1,j2,j3)=max(minDist(j1,j2,j3),distanceFactor*DIST(center2, k1,k2,k3, j1,j2,j3));
                    kv[m]=jv[m];
		  }
		  
                  for( m=0; m<numberOfDimensions; m++ )
		  {
                    // When checking the distance of X to the cutting face we first try
                    // to use the info from points A and B.
                    //
                    //    i2
                    //    |   |   |   |   |
                    //    o---A---X---o---o---  
                    //    |   |   |   |   |
                    //    o---o---B---o---o---  i1

                    // k1=j1, k2=j2, k3=j3;
		    kv[m]=jv[m]-1; // compare to a previous point in each coordinate direction.
                    if( kv[m]<Jv[m].getBase() )
		      continue;
		    
		    real dx = DIST(center2, k1,k2,k3, j1,j2,j3) ;  // distance to the neighbour
		    if( closeToBoundary(k1,k2,k3)>0 )
		    {
                      // neighbour is close to the cutting face
		      if( db(k1,k2,k3)+dx < minDist(j1,j2,j3) )
		      {
			closeToBoundary(j1,j2,j3)=closeToBoundary(k1,k2,k3);
                        db(j1,j2,j3)=db(k1,k2,k3)+dx;
                        if( debug & 2  )
			  printf(" (%i,%i,%i): neighbour is close, db=%e, dx=%e, minDist=%e\n",j1,j2,j3,
			      db(k1,k2,k3),dx,minDist(j1,j2,j3));
			
			ia(j,0)=j1;
			ia(j,1)=j2;
			ia(j,2)=j3;
			ia(j,3)=mask2(j1,j2,j3);
			j++;
                        break;
			
		      }
                      else
		      {
                        // decode closest pt on boundary (taken from neighbour)
                        int cb=closeToBoundary(k1,k2,k3)-nI0;
			i3=cb/nI2; cb-=i3*nI2;
                        i2=cb/nI1;
			i1=cb-i2*nI1;
		      }
		    }
		    else if( closeToBoundary(k1,k2,k3)<0  )
		    {
                      // neighbour is not close to the cutting face
		      if( db(k1,k2,k3)-dx > minDist(j1,j2,j3) )
		      {
                        db(j1,j2,j3)=db(k1,k2,k3)-dx;
			closeToBoundary(j1,j2,j3)=closeToBoundary(k1,k2,k3) ;
                        if( debug & 2 )
			  printf(" (%i,%i,%i): neighbour is far away, db=%e, dx=%e, minDist=%e\n",j1,j2,j3,
			       db(k1,k2,k3),dx,minDist(j1,j2,j3));

			break;
		      }
                      else
		      {
                        int cb=-closeToBoundary(k1,k2,k3)-nI0;  // decode closest pt on boundary
			i3=cb/nI2; cb-=i3*nI2;
                        i2=cb/nI1;
			i1=cb-i2*nI1;
		      }
		    }
                    kv[m]=jv[m];
		  }
		  
                  if( closeToBoundary(j1,j2,j3)==0 )
		  {
		    // find the actual distance to the boundary
                    // we know the distance to the previously computed closest point on the boundary
                    i1=max(I1.getBase(),min(i1,I1.getBound()));
                    i2=max(I2.getBase(),min(i2,I2.getBound()));
                    i3=max(I3.getBase(),min(i3,I3.getBound()));
		    
                    real minimumDistance=numberOfDimensions==2 ?
		      SQR(vertex(i1,i2,i3,0)-x0)+SQR(vertex(i1,i2,i3,1)-x1) :
                      SQR(vertex(i1,i2,i3,0)-x0)+SQR(vertex(i1,i2,i3,1)-x1)+SQR(vertex(i1,i2,i3,2)-x2);
                    real timeA=getCPU();
		    map.approximateGlobalInverse->binarySearchOverBoundary( xv, minimumDistance, iv, side,axis );
                    timeForBinarySearch+=getCPU()-timeA;
                    numberOfBinarySearches++;
		    
                    minimumDistance=SQRT(minimumDistance);
		    if( minimumDistance > minDist(j1,j2,j3) )
		    {
                      // correct the estimate for the min. distance since we only find the
                      // distance to the nearest grid point
                      //                     X
                      //                    /
                      //                   /
                      //        --o-------o---------o--------o---
                      // take the length of the normal component 
                      minimumDistance= numberOfDimensions==2 ?
			fabs( (vertex(i1,i2,i3,0)-x0)*normal(i1,i2,i3,0)+(vertex(i1,i2,i3,1)-x1)*normal(i1,i2,i3,1)) :
                        fabs( (vertex(i1,i2,i3,0)-x0)*normal(i1,i2,i3,0)+(vertex(i1,i2,i3,1)-x1)*normal(i1,i2,i3,1)+
                              (vertex(i1,i2,i3,2)-x2)*normal(i1,i2,i3,2));
		    }
		    
                    db(j1,j2,j3)=minimumDistance;
                    if( debug & 2 )
                       printf(" cut: point (%i,%i,%i), distance to boundary=%e, minDist=%e\n",j1,j2,j3,minimumDistance,
		         minDist(j1,j2,j3));
		    
		    if( minimumDistance < minDist(j1,j2,j3) )
		    {
		      closeToBoundary(j1,j2,j3)=nI0+i1+nI1*i2+nI2*i3; // encode closest bndry pt (i1,i2,i3)
                      assert( closeToBoundary(j1,j2,j3)>0 );
		      ia(j,0)=j1;
		      ia(j,1)=j2;
		      ia(j,2)=j3;
		      ia(j,3)=mask2(j1,j2,j3);
		      j++;
		    }
		    else
		    {
		      closeToBoundary(j1,j2,j3)=-(nI0+i1+nI1*i2+nI2*i3);
                      assert( closeToBoundary(j1,j2,j3)<0 );
		    }
		  }
		}
	      }
              numberCut=j;
              printf("Number of points to check = %i \n",numberCut);
	      timeForCollection+=getCPU()-timeC;
	      
              if( numberCut > 0 )
	      {
		// now double check the points we cut out
		R=Range(0,numberCut-1);
		xx.redim(R,Rx);
		r2.redim(R,Rx);
	      
		for( dir=0; dir<numberOfDimensions; dir++ )
		  xx(R,dir)=center2(ia(R,0),ia(R,1),ia(R,2),dir);

                if( useBoundaryAdjustment )
		{
                  adjustBoundary(cg,grid2,grid,ia(R,Rx),xx(R,Rx));  // adjust boundary points on shared sides 
		}
		real timeI=getCPU();
		map.inverseMap(xx(R,Rx),r2(R,Rx));
                timeForInverseMap+=getCPU()-timeI;

                if( FALSE && debug & 1 )
		{
                  char buff[80];
		  display(r2(R,Rx),sPrintF(buff,"Here are the r2 coordinates on grid=%i\n",grid));
		}
		

		if( debug & 32 )
		{
                  for( i=R.getBase(); i<=R.getBound(); i++ )
		  {
		    if( fabs(r2(i,axis  )-.5) <= .5 +boundaryEps
			|| r2(i,axisp1)<rBound(Start,axisp1,grid) || r2(i,axisp1)>rBound(End,axisp1,grid) 
			|| r2(i,axisp2)<rBound(Start,axisp2,grid) || r2(i,axisp2)>rBound(End,axisp2,grid)  )
		    {
                      fprintf(logFile,"cutHoles: un-cutting a hole pt: grid2=%i (%s), ia=(%i,%i,%i), "
			      "r2(axis)=%7.3e, r2(axisp1)=%7.3e, r2(axisp2)=%7.3e\n",grid2,
 			      (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
                              r2(i,axis  ),r2(i,axisp1),r2(i,axisp2));
		    }
		  }
		}
		
                if( g.numberOfDimensions()==2 )
		{
		  where( r2(R,axis1)>=rBound(Start,axis1,grid) && r2(R,axis1)<=rBound(End,axis1,grid) &&
                         r2(R,axis2)>=rBound(Start,axis2,grid) && r2(R,axis2)<=rBound(End,axis2,grid) )
		  { // we can interpolate
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=MappedGrid::ISinterpolationPoint; 
                    inverseGrid(ia(R,0),ia(R,1),ia(R,2))= grid;  
		    for( dir=0; dir<numberOfDimensions; dir++ )
		      rI(ia(R,0),ia(R,1),ia(R,2),dir)=r2(R,dir);
  		  }
  		  elsewhere( fabs(r2(R,axis))!=Mapping::bogus &&       // point was invertible
                           !( fabs(r2(R,axis  )-.5) <= .5+boundaryEps
                             || (side==0 && r2(R,axis  )>=0. ) || (side==1 && r2(R,axis  )<=1. )
  			   || r2(R,axisp1)<=rBound(Start,axisp1,grid) || r2(R,axisp1)>=rBound(End,axisp1,grid)) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=0;
  		  }
		}
		else  // 3d
		{
		  where( r2(R,axis1)>=rBound(Start,axis1,grid) && r2(R,axis1)<=rBound(End,axis1,grid) &&
                         r2(R,axis2)>=rBound(Start,axis2,grid) && r2(R,axis2)<=rBound(End,axis2,grid) &&
                         r2(R,axis3)>=rBound(Start,axis3,grid) && r2(R,axis3)<=rBound(End,axis3,grid) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=MappedGrid::ISinterpolationPoint; 
                    inverseGrid(ia(R,0),ia(R,1),ia(R,2))= grid;  
		    for( dir=0; dir<numberOfDimensions; dir++ )
		      rI(ia(R,0),ia(R,1),ia(R,2),dir)=r2(R,dir);
  		  }
		  elsewhere( fabs(r2(R,axis))!=Mapping::bogus &&         // point was invertible
                         !(fabs(r2(R,axis  )-.5) <= .5+boundaryEps 
                           || (side==0 && r2(R,axis  )>=0. ) ||(side==1 && r2(R,axis  )<=1. )
			   || r2(R,axisp1)<=rBound(Start,axisp1,grid) || r2(R,axisp1)>=rBound(End,axisp1,grid)
			   || r2(R,axisp2)<=rBound(Start,axisp2,grid) || r2(R,axisp2)>=rBound(End,axisp2,grid)) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=0;
  		  }
		}
                for( int i=0; i<numberCut; i++ )
		{
                  if( r2(i,axis)!=Mapping::bogus && mask2(ia(i,0),ia(i,1),ia(i,2))==0 && 
                      fabs(r2(i,axis)-.5)<.5+biggerBoundaryEps &&
                      ( fabs( r2(i,axisp1)-.5) > .5 || (numberOfDimensions==3 && fabs( r2(i,axisp2)-.5) > .5 ) ) )
		  {
		    // invertible, outside [0,1] in tangential direction and close enough in the normal
                    // We need this since points outside [0,1] may not be inverted as accurately by Newton.
                    printf("++cutHoles: un-cutting a hole (%i,%i,%i) on grid=%i by grid=%i. r=(%e,%e,%e)  "
                           "This point is close to the boundary and outside [0,1] in tangent directions\n",
                           ia(i,0),ia(i,1),ia(i,2),grid2,grid, r2(i,0),r2(i,1),numberOfDimensions==2 ? 0. : r2(i,2));
  		    mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);  // reset this value
		  }
		  else if( r2(i,axis)==Mapping::bogus )  // NOT invertible
		  {
                    // If a neighbouring point interpolates then we expect to cut a hole here
                    k1 = ia(i,0), k2=ia(i,1), k3=ia(i,2);
                    const int width3=numberOfDimensions==2 ? 0 : 1;
                    bool skipThisPoint=TRUE;
                    for( int m3=-width3; m3<=width3 && skipThisPoint ; m3++ )
		    {
		      for( int m2=-1; m2<=1 && skipThisPoint; m2++ )
		      {
			for( int m1=-1; m1<=1; m1++ )
			{
			  if( mask2(k1+m1,k2+m2,k3+m3)<0 && inverseGrid(k1+m1,k2+m2,k3+m3)==grid )
			  {
			    skipThisPoint=FALSE;
                            j3=g.gridIndexRange(Start,axis3);// default
			    for( dir=0; dir<numberOfDimensions; dir++ )
			    {
			      // note: cellCenterOffset will round to the nearest point. 
                              rv[dir]=rI(k1+m1,k2+m2,k3+m3,dir);
			      jv[dir]=(int)floor( rv[dir]/g.gridSpacing(dir)+g.indexRange(Start,dir)-cellCenterOffset );
			    }
                            jv[axis]=g.gridIndexRange(side,axis);  // project onto the boundary
			    
                            break;
			  } 
			}
		      }
		    }
		    if( !skipThisPoint )
		    {
                      mask2(k1,k2,k3)=0;
                      if( debug & 4 )
                        printf("Trying to cut a hole at the non-invertible pt (%i,%i,%i) jv=(%i,%i,%i)\n",
                           k1,k2,k3,j1,j2,j3);
		      // if the point was NOT invertible then we double check to see it is outside of the boundary
		      // Estimate the r location for the point using
		      //        r = r(boundary) + dr
		      //        dr = [ dr/dx ] * dx       
		      //        dx = vector from boundary point jv to the point on the other grid, xx
               
		      real re[3], dx[3], det;
		      int ax;
		      for( ax=0; ax<numberOfDimensions; ax++ )
			dx[ax]=xx(i,ax)-vertex(j1,j2,j3,ax);

#define XR(m,n) xr((j1),(j2),(j3),(m)+(n)*numberOfDimensions)

		      if( numberOfDimensions==2 )
		      {
			real det = XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0);
			if( det!=0. )
			{
			  det=1./det;
			  re[0]=(  XR(1,1)*dx[0]-XR(0,1)*dx[1] )*det;
			  re[1]=( -XR(1,0)*dx[0]+XR(0,0)*dx[1] )*det;
			}
			else
			{ // if the jacobian is singular
			  printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
			  re[0]=re[1]=0.;
			  re[axis]=.1*(2*side-1); // move point outside the grid
			}
			re[2]=0.;
		      }
		      else
		      {
			det = (XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0))*XR(2,2) +
			  (XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1))*XR(2,0) +
			  (XR(0,2)*XR(1,0)-XR(0,0)*XR(1,2))*XR(2,1);
			if( det!=0. )
			{
			  det=1./det;
			  re[0]=( (XR(1,1)*XR(2,2)-XR(2,1)*XR(1,2))*dx[0]+
				  (XR(2,1)*XR(0,2)-XR(0,1)*XR(2,2))*dx[1]+
				  (XR(0,1)*XR(1,2)-XR(1,1)*XR(0,2))*dx[2] )*det;
		      
			  re[1]=( (XR(1,2)*XR(2,0)-XR(2,2)*XR(1,0))*dx[0]+
				  (XR(2,2)*XR(0,0)-XR(0,2)*XR(2,0))*dx[1]+
				  (XR(0,2)*XR(1,0)-XR(1,2)*XR(0,0))*dx[2] )*det;
		      
			  re[2]=( (XR(1,0)*XR(2,1)-XR(2,0)*XR(1,1))*dx[0]+
				  (XR(2,0)*XR(0,1)-XR(0,0)*XR(2,1))*dx[1]+
				  (XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1))*dx[2] )*det;
			
			}
			else
			{ // if the jacobian is singular
			  printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
			  re[0]=re[1]=re[2]=0.;
			  re[axis]=.1*(2*side-1);
			}
		      
		      }
#undef XR
		      for( ax=0; ax<numberOfDimensions; ax++ )
			re[ax]+=jv[ax]*g.gridSpacing(ax);
		    
                    // do not cut a hole if the point is
                    //     1. inside in the normal direction
                    // or  2. on the wrong side in the normal direction
                    // or  3. outside in the tangential direction.
		      if( fabs(re[axis]-.5) <= .5+boundaryEps 
			  || (side==0 && re[axis  ]>=0. ) ||(side==1 && re[axis  ]<=1. )
			  || re[axisp1]<=rBound(Start,axisp1,grid) || re[axisp1]>=rBound(End,axisp1,grid)
			  || re[axisp2]<=rBound(Start,axisp2,grid) || re[axisp2]>=rBound(End,axisp2,grid) )
		      {
			if( debug & 4 )
			{
			  printf("cutHoles: Non-invertible point: estimated r=(%7.2e,%7.2e,%7.2e) is",
				 re[0],re[1],re[2]);
			  if(fabs(re[axis]-.5) <= .5+boundaryEps ) 
			    printf("   inside in the normal direction! No hole cut. ****** \n");
			  else
			    printf(" outside in the tangential tangential direction. No hole cut. \n");
			
			}
			mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
		      }
		      else
		      {
			real cosAngle =0.;
			real dist1=0., dist2=0.;
			jpv[0]=j1; jpv[1]=j2; jpv[2]=j3;
			jpv[axis]=g.gridIndexRange(1-side,axis);  // opposite boundary
			for( int ax=0; ax<numberOfDimensions; ax++ )
			{
			  cosAngle += normal(j1,j2,j3,ax)*dx[ax]; //  (xx(i,ax)-x(j1,j2,j3,ax));
			  dist1+=SQR(vertex(j1,j2,j3,ax)-vertex(j1p,j2p,j3p,ax));    // approx distance to opposite boundary
			  dist2+=SQR(dx[ax]);            // distance from boundary to potential hole point
			}
			cosAngle/= SQRT(max(REAL_MIN,dist2));
			const real maxCosAngle=.0;  
			const real maxDistFactor=.9;
			// we do not cut a hole if the cosine of the angle between normals < maxCosAngle or if
			// the distance to the potential hole point is greater than the distance to the opposite boundary
			// 
			if( cosAngle < maxCosAngle || dist2>dist1*maxDistFactor )  // if cosine of the angle between normals > ?? 
			{
			  if( debug & 1 )
			  {
			    printf("cutHoles: no hole cut for the non-invertible point (%i,%i,%i) on grid=%i by grid=%i ",
				   ia(i,0),ia(i,1),ia(i,2),grid2,grid);
			    if( cosAngle < maxCosAngle )
			      printf("since n.n=%7.2e <%7.2e \n",cosAngle,maxCosAngle);
			    else
			      printf("since dist2=%7.2e > dist1*.9=%7.2e\n",dist2,dist1*maxDistFactor);
			  }
			  mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
			}
/* -----
                        else if( cosAngle<.1 )
			{
			printf("cutHoles:WARNING: hole cut for the non-invertible point (%i,%i,%i) "
			"on grid=%i by grid=%i \n"
			"                   BUT cosine of angle between normals=%e \n",
			ia(i,0),ia(i,1),ia(i,2),grid2,grid,cosAngle);
			}
   ---- */		      
		      }
		    } // end if( !skipThis )
		  } // end else if( r2(i,axis)==Mapping::bogus )  
		  
                  if( debug & 16 && numberOfDimensions==3 )
		  {
		    fprintf(logFile,"cutHoles: grid=%s, grid2=%s pt=(%i,%i,%i) r2=(%6.2e,%6.2e,%6.2e) mask=%i\n",
                       (const char*)g.mapping().getName(Mapping::mappingName),
                       (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
			    r2(i,axis1),r2(i,axis2),r2(i,axis3),mask2(ia(i,0),ia(i,1),ia(i,2)));
                    fprintf(logFile,"          x=(%6.2e,%6.2e,%6.2e) boundary shift=(%6.2e,%6.2e,%6.2e) \n",
                            xx(i,axis1),xx(i,axis2),xx(i,axis3),
                            fabs(xx(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)),
                            fabs(xx(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)),
                            fabs(xx(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)) );
		  }
		  if( mask2(ia(i,0),ia(i,1),ia(i,2))==0 )
		  {
		    
		    for( dir=0; dir<numberOfDimensions; dir++ )
		    {
		      holePoint(numberOfHolePoints,dir) = xx(i,dir);
		    }
		    numberOfHolePoints++;
		    assert( numberOfHolePoints < maxNumberOfHolePoints );
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  // we need to set values in the inverseGrid array to -1 at all unused points *** can this be done else where ????
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & g = cg[grid];
    const IntegerArray & mask = g.mask();

    g.mask().periodicUpdate();   // ****

    IntegerArray & inverseGrid = cg.inverseGrid[grid];
    getIndex(extendedGridIndexRange(g),I1,I2,I3);  
    where( mask(I1,I2,I3)==0 )
      inverseGrid(I1,I2,I3)=-1;
  }
  

  real time=getCPU();
  if( info & 2 ) 
  {
    printf(" time for binary %i searches = %e , timeForCollection=%e, timeForInverseMap=%e\n",
           numberOfBinarySearches,timeForBinarySearch,
             timeForCollection,timeForInverseMap);
    printf(" time to cut holes........................................%e (total=%e)\n",time-time0,time-totalTime);
  }
  
  timeCutHoles=time-time0;
  
  return numberOfHolePoints;
}



#undef DIST










