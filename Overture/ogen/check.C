#include "Overture.h"
#include "Ogen.h"

int Ogen::
checkForOrphanPointsOnBoundaries(CompositeGrid & cg )
//
// This function assumes that the boubdary points have been interpolated already.
// For each physical boundary of each grid:
//
{
  if( info & 4 ) printf("checking for orphan points on physcial boundaries...\n");

  const int numberOfBaseGrids = cg.numberOfBaseGrids();

  Index I1,I2,I3;
  Range R(0,cg.numberOfDimensions()-1);
  int iv[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];

  int maxNumberOfOrphanPoints=100*cg.numberOfComponentGrids()*cg.numberOfDimensions();
  numberOfOrphanPoints=0;
  orphanPoint.redim(maxNumberOfOrphanPoints,cg.numberOfDimensions()+1);
  

  int grid;
  IntegerArray faceIndexRange(2,3);
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    const realArray & center = g.center();
    
    if( info & 4 ) printf("checking boundaries of grid: %s for orphan points\n",
                   (const char*)g.mapping().getName(Mapping::mappingName));

    for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
        if( g.boundaryCondition()(side,axis) > 0 )
	{
	  // this side is a physical boundary: 
          // check the edges whose neighbouring faces are interpolation boundaries

	  faceIndexRange=g.extendedIndexRange();  // gridIndexRange for this face
	  faceIndexRange(Start,axis )=g.extendedIndexRange()(side,axis);
	  faceIndexRange(End  ,axis )=g.extendedIndexRange()(side,axis);

          for( int edge=0; edge<cg.numberOfDimensions()-1; edge++ )
	  {
            int eAxis = (axis+edge+1) % cg.numberOfDimensions();
	    for( int eSide=Start; eSide<=End; eSide++ )
	    {
              if( g.boundaryCondition()(eSide,eAxis) == 0 )
	      {
		getBoundaryIndex(faceIndexRange,eSide,eAxis,I1,I2,I3);  // this is an edge
              
                const intArray & mask = g.mask();
                // look for orphan points
		for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    {
		      if( !( mask(i1,i2,i3) & MappedGrid::ISinterpolationPoint) )
		      {
                        if( cg.numberOfDimensions()==3 )
                          printf("orphan point %i: grid=%i, i=(%i,%i,%i) mask=%i, (interp=%i,discretize=%i) \n",
                               numberOfOrphanPoints,
                               grid,i1,i2,i3,mask(i1,i2,i3),MappedGrid::ISinterpolationPoint,
                               MappedGrid::ISdiscretizationPoint);
			
                        for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
                          orphanPoint(numberOfOrphanPoints,dir)=center(i1,i2,i3,dir);

                        orphanPoint(numberOfOrphanPoints,cg.numberOfDimensions())=grid;

                        numberOfOrphanPoints++;
                        if( numberOfOrphanPoints>= maxNumberOfOrphanPoints )
			{
			  maxNumberOfOrphanPoints*=2;
			  orphanPoint.resize(numberOfOrphanPoints,orphanPoint.getLength(1));
			}
			
		      }
		    }
	      }
	    }
	  }
	}
      }
    }
  }
  if( numberOfOrphanPoints>0 )
    orphanPoint.resize(numberOfOrphanPoints,orphanPoint.getLength(1));
  else
    orphanPoint.redim(0);
  
  return numberOfOrphanPoints;
}


/*  -------------------
int Ogen::
cutHoles(CompositeGrid & cg)
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
  if( info & 4 ) printf("cutting holes with physical boundaries...\n");

  bool vectorize=TRUE;

  Index I1,I2,I3;
  Range R, Rx(0,cg.numberOfDimensions()-1);
  RealArray x,r;
  RealArray x2(1,3), r2(1,3);
  IntegerArray ia;
  
  int i, iv[3], jv[3], jvp[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];
  int & j1 = jv[0];
  int & j2 = jv[1];
  int & j3 = jv[2];

  int maxNumberOfHolePoints=10000*numberOfBaseGrids*cg.numberOfDimensions();
  numberOfHolePoints=0;
  holePoint.redim(maxNumberOfHolePoints,cg.numberOfDimensions());
  
  const real boundaryAngleEps=.01;
  const real boundaryNormEps=0.; // *wdh* 980126 ** not needed since we double check//  1.e-2;

  int grid;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    Mapping & map = g.mapping().getMapping();
    const real cellCenterOffset= g.isAllCellCentered() ? .5 : 0.; 

    if( info & 4 ) printf("cutting holes with grid: %s ...\n",
                   (const char*)g.mapping().getName(Mapping::mappingName));

    for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
    {
      const int axisp1 = (axis +1) % cg.numberOfDimensions();
      const int axisp2 = cg.numberOfDimensions()==2 ? axisp1 : (axis +2) % cg.numberOfDimensions();
      
      for( int side=Start; side<=End; side++ )
      {
        // Note: do not cut holes with singular sides
        if( g.boundaryCondition()(side,axis) > 0 && 
            map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity ) 
	{
	  // this side is a physical boundary
	  getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);   // note: use gridIndexRange
	  // no: getBoundaryIndex(extendedGridIndexRange(g),side,axis,I1,I2,I3);   // note: use gridIndexRange
          bool firstTimeForThisBoundary=TRUE;
	  
          const RealDistributedArray & normal = g.vertexBoundaryNormal(side,axis); // *correct*
          // check all other grids  ********************************combine with interp on boundaries **********
          for( int grid2=0; grid2<numberOfBaseGrids; grid2++ )
	  {
            MappedGrid & g2 = cg[grid2];
	    if( grid2!=grid && cg.mayCutHoles(grid,grid2)
                &&  map.intersects( g2.mapping().getMapping(), side,axis,-1,-1,.1 ) )
	    {
              if( firstTimeForThisBoundary )
	      {
		firstTimeForThisBoundary=FALSE;
		r.redim(I1,I2,I3,Rx);
		x.redim(I1,I2,I3,Rx);
		x=g.vertex()(I1,I2,I3,Rx);    // ** note these are the vertices on the boundary ***
	      }

	      RealArray & center2 = g2.center();
              IntegerArray & mask2 = g2.mask();
              const IntegerArray & indexRange2 = g2.indexRange();
              const IntegerArray & extendedIndexRange2 = g2.extendedIndexRange();
	      
              real time1=getCPU();
              g2.mapping().getMapping().inverseMapGrid(x,r);
              real time2=getCPU();
              if( info & 4 ) printf("cut with (side,axis)=(%i,%i) : time to compute inverseMap on grid2=%s, "
                     "is %e (total=%e)\n",
                     side,axis,(const char*)g2.mapping().getName(Mapping::mappingName),time2-time1,time2-totalTime);
	      
	      int dir;
	      for( dir=cg.numberOfDimensions(); dir<3; dir++ )
	      {
		jv[dir]=indexRange2(Start,dir);   // give default values 
		jvp[dir]=jv[dir];
	      }
              int numberCut=0;
              ia.redim(I1.length()*I2.length()*I3.length(),4);   // holds hole points
	      
              // for any point that can be interpolated, find points nearby that are outside the
              // boundary and mark them as unused.
              for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
              for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
              for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
                if( r(i1,i2,i3,axis1)>rBound(Start,axis1,grid2) && r(i1,i2,i3,axis1)<rBound(End,axis1,grid2) &&
                    r(i1,i2,i3,axis2)>rBound(Start,axis2,grid2) && r(i1,i2,i3,axis2)<rBound(End,axis2,grid2) &&
                    ( g.numberOfDimensions()<3 || 
                      ( r(i1,i2,i3,axis3)>rBound(Start,axis3,grid2) && r(i1,i2,i3,axis3)<rBound(End,axis3,grid2) )
                    )
                  )
		  // *wdh* 980426 if( max(fabs(r(i1,i2,i3,Rx)-.5))<.51 )  // **** fix arbitrary tolerance ***
		{
                  // this interpolee point is near the the boundary
                  // *** find a point on grid2 that is outside ****************** to do *****

		  // check for shared sides
                  bool sharedSide=FALSE;
		  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		  {
		    if( ( g2.boundaryCondition()(Start,dir)>0 
                          && fabs(r(i1,i2,i3,dir)   )<g2.sharedBoundaryTolerance()(Start,dir)*g2.gridSpacing()(dir) 
                          && g2.sharedBoundaryFlag()(0,dir)!=0 
                          && g2.sharedBoundaryFlag()(0,dir)==g.sharedBoundaryFlag()(side,axis) ) ||
		        ( g2.boundaryCondition()(End,dir)>0
                           && fabs(r(i1,i2,i3,dir)-1.)<g2.sharedBoundaryTolerance()(Start,dir)*g2.gridSpacing()(dir) 
                           && g2.sharedBoundaryFlag()(1,dir)!=0 
                           && g2.sharedBoundaryFlag()(1,dir)==g.sharedBoundaryFlag()(side,axis) ) )
		    {
                      sharedSide=TRUE;
                      break;
		    }
		  }
                  if( sharedSide )
		  { 
		    continue;   // don't cut holes on two sides that are shared
		  }
		  // jv : index of lower left corner of the cell that contains
                  //      the point. (*NOTE* jv==(j1,j2,j3))
		  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		  {
                    jv[dir] = (int)floor( r(i1,i2,i3,dir)/g2.gridSpacing()(dir)+indexRange2(Start,dir) - cellCenterOffset );
                    // jv[dir] = int( r(i1,i2,i3,dir)/g2.gridSpacing()(dir)+indexRange2(Start,dir) - cellCenterOffset );
                    jvp[dir]=min(jv[dir]+1,extendedIndexRange2(End,dir));
                    // *wdh* 980608 jv[dir] =max(jv[dir]  ,extendedIndexRange2(Start,dir));
                    jv[dir] =max(jv[dir]-1,extendedIndexRange2(Start,dir));
		  }
		  
                  // mark points near jv that are outside  **** is this needed?? -- yes  ***
                  for( int k3=j3; k3<=jvp[2]; k3++ )
                  for( int k2=j2; k2<=jvp[1]; k2++ )
                  for( int k1=j1; k1<=jvp[0]; k1++ )
		  {
                    // take the dot product of the vector with the outward normal
                    //           \         
                    //            \      
                    //            +\+ +  
                    //            + x +  
                    //            + +\+  
                    //                \
                    //  ****NOTE**** do not mark a hole point at points that are already interpolation
		    if( mask2(k1,k2,k3)!=0  
                        && !( mask2(k1,k2,k3) & MappedGrid::ISinterpolationPoint)
                        && ( sum( (center2(k1,k2,k3,Rx)-x(i1,i2,i3,Rx))*normal(i1,i2,i3,Rx) ) >= 
                              0. ) 
// *wdh* 980427              (boundaryAngleEps*sum(fabs(center2(k1,k2,k3,Rx)-x(i1,i2,i3,Rx))) 
//			     + boundaryNormEps))  // ************ fix this, needed to be scaled ***********
                      )
		    {
                      if( vectorize )
		      {
                        // save this point so we can later double check it.
                        if( ia.getLength(0)<=numberCut )
			  ia.resize(ia.getLength(0)*2,4);

			ia(numberCut,0)=k1;
			ia(numberCut,1)=k2;
			ia(numberCut,2)=k3;
			ia(numberCut,3)=mask2(k1,k2,k3);
                        numberCut++;

                        mask2(k1,k2,k3)=0; // not used (may be reset later)

			if( debug & 4 && (TRUE || cg.numberOfDimensions()==3) )
			  fprintf(logFile,"cutting hole point (k1,k2,k3)=(%i,%i,%i) on grid %i using grid %i, "
				  "r=(%e,%e,%e) \n", k1,k2,k3,grid2,grid,r(i1,i2,i3,0),r(i1,i2,i3,1),r(i1,i2,i3,2));
		      }
		      else
		      {
			// double check that the point is outside the other grid
			for( dir=0; dir<cg.numberOfDimensions(); dir++ )
			  x2(0,dir)=center2(k1,k2,k3,dir);

			map.inverseMap(x2,r2); // slow --- set mask=0 and then recheck all points at once

			// point should be outside in the normal direction but inside in the tangential
			// ( we need the tangential condition for the test to make sense where grids overlap)
			// *** if we fail to interpolate, r=10 is returned. ---> assume this means we are outside
			if( max(fabs(r2(0,Rx)))>2. || (
			  fabs(r2(0,axis  )-.5) > .5 
			  && fabs(r2(0,axisp1)-.5)<=.51       // ***** check for shared sides here
			  && fabs(r2(0,axisp2)-.5)<=.51 ) )
			{

			  mask2(k1,k2,k3)=0; // not used

			  if( debug & 4 && (TRUE || cg.numberOfDimensions()==3) )
			    fprintf(logFile,"cutting hole point %i : grid=%i, (i1,i2,i3)=(%i,%i,%i), x=(%e,%e,%e),"
                               " grid2=%i, r=(%4.1f,%4.1f,%4.1f) x2=(%e,%e,%e) \n",numberOfHolePoints,
				   grid,i1,i2,i3,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),
				   grid2,r(i1,i2,i3,0),r(i1,i2,i3,1),r(i1,i2,i3,2),
				   center2(k1,k2,k3,0),center2(k1,k2,k3,1),center2(k1,k2,k3,2));

			  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
			  {
			    holePoint(numberOfHolePoints,dir) = center2(k1,k2,k3,dir);
			  }
			  numberOfHolePoints++;
			  assert( numberOfHolePoints < maxNumberOfHolePoints );
			}
			else
			{
			  if( debug & 4 )
			  {
			    printf("INFO:cutHoles: not cutting a hole, refined test failed"
				   ", r=(%8.2e,%8.2e,%8.2e), r2=(%8.2e,%8.2e,%8.2e) sharedSide=%i\n",
				   r(i1,i2,i3,0),r(i1,i2,i3,1),(cg.numberOfDimensions()>2 ? r(i1,i2,i3,axis3) : 0.),
				   r2(0,axis1),r2(0,axis2),(cg.numberOfDimensions()>2 ? r2(0,axis3) : 0.),sharedSide);
			  }
			}
		      }
		    }
		  }
		}
	      }  // for i3

              if( vectorize && numberCut > 0 )
	      {
		// now double check the points we cut out
		R=Range(0,numberCut-1);
		x2.redim(R,Rx);
		r2.redim(R,Rx);
	      
		for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		  x2(R,dir)=center2(ia(R,0),ia(R,1),ia(R,2),dir);

		map.inverseMap(x2(R,Rx),r2(R,Rx));

		if( debug & 4 )
		{
                  for( i=R.getBase(); i<=R.getBound(); i++ )
		  {
		    if( fabs(r2(i,axis  )-.5) <= .5 
			|| r2(i,axisp1)<rBound(Start,axisp1,grid) || r2(i,axisp1)>rBound(End,axisp1,grid) 
			|| r2(i,axisp2)<rBound(Start,axisp2,grid) || r2(i,axisp2)>rBound(End,axisp2,grid)  )
		    {
                      fprintf(logFile,"cutHoles: un-cutting a hole point: grid2=%i (%s), ia=(%i,%i,%i), "
			      "r2(axis)=%e, r2(axisp1)=%e, r2(axisp2)=%e\n",grid2,
 			      (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
                              r2(i,axis  ),r2(i,axisp1),r2(i,axisp2));
		    }
		  }
		}
		
                if( g.numberOfDimensions()==2 )
		{
		  where( fabs(r2(R,axis))<2. &&       // point was invertible
                         (fabs(r2(R,axis  )-.5) <= .5 // this is correct
			   || r2(R,axisp1)<rBound(Start,axisp1,grid) || r2(R,axisp1)>rBound(End,axisp1,grid)) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=ia(R,3); // MappedGrid::ISdiscretizationPoint; // reset these values
  		  }
		}
		else  // 3d
		{
		  where( fabs(r2(R,axis))<2. &&         // point was invertible
                         (fabs(r2(R,axis  )-.5) <= .5 
			   || r2(R,axisp1)<rBound(Start,axisp1,grid) || r2(R,axisp1)>rBound(End,axisp1,grid)
			   || r2(R,axisp2)<rBound(Start,axisp2,grid) || r2(R,axisp2)>rBound(End,axisp2,grid)) )
		  {
  		    mask2(ia(R,0),ia(R,1),ia(R,2))=ia(R,3); // MappedGrid::ISdiscretizationPoint; // reset these values
  		  }
		}
                for( int i=0; i<numberCut; i++ )
		{
		  if( mask2(ia(i,0),ia(i,1),ia(i,2))==0 )
		  {
		    
		    for( dir=0; dir<cg.numberOfDimensions(); dir++ )
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
  real time=getCPU();
  if( info & 2 ) 
    printf(" time to cut holes........................................%e (total=%e)\n",time-time0,time-totalTime);
  timeCutHoles=time-time0;
  
  return numberOfHolePoints;
}
------ */


int Ogen::
countCrossingsWithRealBoundary(CompositeGrid & cg, const realArray & x, IntegerArray & crossings )
//
// Count the number of times a ray crosses the real boundary of the domain
//
// /x (input) : x(I,0:r-1) - points to check (starting points for the rays)
// /crossings (output) : number of distinct crossings, even number means outside.
//
{
  Range I = x.dimension(0);
  Range R = x.dimension(1);

  const int numberOfBaseGrids = cg.numberOfBaseGrids();
  const int numberOfDimensions = cg.numberOfDimensions();
  
  crossings.redim(I);
  crossings=0;
    
  int maxNumberOfCrossings=numberOfBaseGrids*2+5;   // this is only a guess
  RealArray xCross;
  // xCross holds crossing points (x,y,z), (i1,i2,i3) grid number
  // 
  xCross.redim(I,Range(0,2*numberOfDimensions),maxNumberOfCrossings); 
  xCross=-1.;

  const int xi1=numberOfDimensions; // position of the index i1 in xCross.
  const int xi2=xi1+1;
  const int xi3=xi2+1;
  const int xGrid = 2*numberOfDimensions;  // position of the grid number in xCross.
  Range Rx(0,numberOfDimensions-1);

  // count crossings with all physical boundaries
  const int ng=numberOfBaseGrids;
  for( int grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    Mapping & map = g.mapping().getMapping();
    const intArray & mask = g.mask();

    for( int axis=axis1; axis<numberOfDimensions; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
        // On non-conforming grids, interpolation boundaries count as part of the boundary
        // Hopefully there are no hole points in the region of overlap between
        // the interpolation boundary and the other physical boundary

        // **** g.mayCutHoles(grid,grid)==100 **** means the grid is non-conforming

	if( (g.boundaryCondition()(side,axis) > 0 && 
             map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity )
            || g.isPeriodic()(axis)==Mapping::derivativePeriodic   // these boundaries count for hole cutting
            || (cg.mayCutHoles(grid,grid)==100 && g.boundaryCondition()(side,axis)==0 ) )  
	{
          if( debug & 4 )
            printf("countCrossingsWithRealBoundaries: check (side,axis)=(%i,%i) \n",side,axis);
	  
          // Cells must have corners that are discretization or interpolation in order to be considered
          // for crossings.
          // ***** NOTE ***** here we assume that the number of points in the Mapping is the same as
          // the number of points in the mask (i.e. the MappedGrid)
#ifndef USE_PPP
	  map.approximateGlobalInverse->
            countCrossingsWithPolygon(x, crossings, side,axis,xCross, 
				      mask, MappedGrid::ISdiscretizationPoint | MappedGrid::ISinterpolationPoint,
                                      maskRatio[0],maskRatio[1],maskRatio[2] );
#else
          throw "error";
#endif
          // fill in the grid number  *** fix this somehow ***
	  for( int i=I.getBase(); i<=I.getBound(); i++ )
	  {
	    for( int c1= 0; c1<crossings(i); c1++ )
	    {
              if( xCross(i,xGrid,c1)<0. )
                xCross(i,xGrid,c1)=grid+ ng*( side+2*axis);
	    }
	  }
	  

	}
      }
    }
  }
    
  // remove counts of multiple crossings across a boundary where two grids overlap


  // Use this relative tolerance as the initial bound to consider that two points of crossings may be the same.
  real tolerance = 1.e-2*max(gridScale); 
  // this tolerance is used for unshared boundaries
  real unsharedTol = REAL_EPSILON*max(gridScale)*10.; 
    
  for( int i=I.getBase(); i<=I.getBound(); i++ )
  {
    int numberOfCrossings=crossings(i);
    int numberOfDistinctCrossings=numberOfCrossings;
    if( numberOfCrossings>1 )
    {
      // check that we haven't crossed the same point twice on different grids
      for( int c1= 0; c1<numberOfCrossings; c1++ )
      {
	for( int c2=c1+1; c2<numberOfCrossings; c2++ )
	{
          const int grid1=int(xCross(i,xGrid,c1)+.5) % ng;
          const int grid2=int(xCross(i,xGrid,c2)+.5) % ng;
          real dist = fabs(xCross(i,axis2,c1)-xCross(i,axis2,c2));   // *note* y-direction only
	  
          if( grid1!=grid2 && dist<tolerance )
	  {
            bool multipleCrossing=FALSE;
            if( dist<unsharedTol )
	    { // distance between points is very small, no need for sides to be shared
              multipleCrossing=TRUE;
	    }
	    else 
	    {
              // extract (side,axis) info for each point
	      int sideAxis = int(xCross(i,xGrid,c1)+.5)/ng;
	      int side1=sideAxis % 2;
	      int dir1 =sideAxis/2;
	      sideAxis = int(xCross(i,xGrid,c2)+.5)/ng;
	      int side2=sideAxis % 2;
	      int dir2 =sideAxis/2;

	      if( cg[grid1].sharedBoundaryFlag()(side1,dir1)!=0 &&
		  cg[grid1].sharedBoundaryFlag()(side1,dir1)==cg[grid2].sharedBoundaryFlag()(side2,dir2) )
	      {
                // points belong to boundaries that are shared
		// *** check the mask too ??? ****************************
		int iv[3], jv[3], ivp[3], jvp[3];    
		int &i1=iv[0], &i2=iv[1], &i3=iv[2];
		int &j1=jv[0], &j2=jv[1], &j3=jv[2];
	      
		iv[0]=int(xCross(i,xi1,c1)+.5);  
		iv[1]=int(xCross(i,xi2,c1)+.5);
		iv[2]= numberOfDimensions>2 ? int(xCross(i,xi3,c1)+.5) : cg[grid1].extendedIndexRange()(Start,axis3);

		jv[0]=int(xCross(i,xi1,c2)+.5);
		jv[1]=int(xCross(i,xi2,c2)+.5);
		jv[2]=numberOfDimensions>2 ? int(xCross(i,xi3,c2)+.5) : cg[grid2].extendedIndexRange()(Start,axis3);
		// The dot product of the boundary normals should be positive
		real nDot = sum(cg[grid1].vertexBoundaryNormal(side1,dir1)(i1,i2,i3,Rx)*  // vertex correct
				cg[grid2].vertexBoundaryNormal(side2,dir2)(j1,j2,j3,Rx));

		if( nDot > 0. ) // normals point in the same half plane
		{
		  const realArray & x1 = cg[grid1].center();
		  const realArray & x2 = cg[grid2].center();
	      
		  // estimate the expected error in the normal direction :
                  //   (grid spacing off the boundary) * sharedBoundaryTolerance
		  real dn1=0., dn2=0.;
		  ivp[0]=iv[0]; ivp[1]=iv[1]; ivp[2]=iv[2];
		  jvp[0]=jv[0]; jvp[1]=jv[1]; jvp[2]=jv[2];
		  ivp[dir1]+=1-2*side1;
		  dn1=sum(fabs(x1(ivp[0],ivp[1],ivp[2],Rx) - x1(iv[0],iv[1],iv[2],Rx)))
		    *cg[grid1].sharedBoundaryTolerance()(side1,dir1);

		  jvp[dir2]+=1-2*side2;
		  dn2=sum(fabs(x2(jvp[0],jvp[1],jvp[2],Rx) - x2(jv[0],jv[1],jv[2],Rx)))
		    *cg[grid2].sharedBoundaryTolerance()(side2,dir2);
		  real dn=max(dn1,dn2);

		  if( dist < dn )
		  {
		    // points match
		    multipleCrossing=TRUE;
		  }
		  else
		  {
		    // check for a tangential crossing, we allow a greater tolerance in this case

		    ivp[dir1]=iv[dir1]; // reset from above
		    jvp[dir2]=jv[dir2];
		    // alpha = |cos(theta)| = angle between the ray and the normal , 
                    //     |(x2-x1).n| / /| x2-x1 \| == | normal(axis2)|   (since x2-x2 = (0,dy,0))
		    // choose one normal, could average?
		    real alpha = fabs( cg[grid1].vertexBoundaryNormal(side1,dir1)(i1,i2,i3,axis2)); 

		    // find the maximum length of the cell edges (tangential direction only)
		    real dt1=0., dt2=0.;
		    for( int dir=0; dir<numberOfDimensions-2; dir++ )
		    {
		      int axisp = (dir1+dir+1) % numberOfDimensions; // tangential direction
		      ivp[axisp]++;
		      dt1=max(dt1, sum(fabs(x1(ivp[0],ivp[1],ivp[2],Rx) - x1(iv[0],iv[1],iv[2],Rx))) );
		      ivp[axisp]--;

		      axisp = (dir2+dir+1) % numberOfDimensions;
		      jvp[axisp]++;
		      dt2=max(dt2, sum(fabs(x2(jvp[0],jvp[1],jvp[2],Rx) - x2(jv[0],jv[1],jv[2],Rx))) );
		      jvp[axisp]--;
		    }
                    real dt=max(dt1,dt2);
		    if( dist < alpha*dn+(1.-alpha)*dt )
		    {
		      // points match
		      multipleCrossing=TRUE;
		    }
		  }
		}  // if nDot
	      }  // if share
	    }
	    if( multipleCrossing )
	    {
	      numberOfDistinctCrossings--; // do not count one of the multiple crossings
	      if( debug & 4 )
	      {
		printf("countCrossingsWithRealBoundaries:removing a duplicate crossing \n");
		if( numberOfDimensions==3 )
		{
		  printf("crossing 1: grid=%i, x=(%e,%e,%e), (i1,i2,i3)=(%i,%i,%i)\n",
			 grid1,xCross(i,0,c1),xCross(i,1,c1),xCross(i,2,c1), 
			 int(xCross(i,3,c1)+.5),int(xCross(i,4,c1)+.5),int(xCross(i,5,c1)+.5));
		  printf("crossing 2: grid=%i, x=(%e,%e,%e), (i1,i2,i3)=(%i,%i,%i)\n",
			 grid2,xCross(i,0,c2),xCross(i,1,c2),xCross(i,2,c2), 
			 int(xCross(i,3,c2)+.5),int(xCross(i,4,c2)+.5),int(xCross(i,5,c2)+.5));
		}
		else
		{
		  printf("crossing 1: grid=%i, x=(%e,%e), (i1,i2)=(%i,%i)\n",
			 grid1,xCross(i,0,c1),xCross(i,1,c1),
			 int(xCross(i,2,c1)+.5),int(xCross(i,3,c1)+.5));
		  printf("crossing 2: grid=%i, x=(%e,%e), (i1,i2)=(%i,%i)\n",
			 grid2,xCross(i,0,c2),xCross(i,1,c2),
			 int(xCross(i,2,c2)+.5),int(xCross(i,3,c2)+.5));
		}
	      }
	    }
	    else 
	    {
              // no multiple crossing found but points are reasonable close
	      // -- double check and print some info ---
	      int sideAxis = int(xCross(i,xGrid,c1)+.5)/ng;
	      int side1=sideAxis % 2;
	      int dir1 =sideAxis/2;
	      sideAxis = int(xCross(i,xGrid,c2)+.5)/ng;
	      int side2=sideAxis % 2;
	      int dir2 =sideAxis/2;
	      if( cg[grid1].sharedBoundaryFlag()(side1,dir1)!=0 &&
		  cg[grid1].sharedBoundaryFlag()(side1,dir1)==cg[grid2].sharedBoundaryFlag()(side2,dir2) )
	      {
		if( info & 2 ) 
		{
                  printf("countCrossings:WARNING: There is a potential double crossing on shared sides not caught\n");
		  printf(" i=%i, grid1=%i, x1=(%e,%e,%e),  grid2=%i, x2=(%e,%e,%e), distance=%e, tolerance=%e\n",i,
			 grid1,xCross(i,0,c1),xCross(i,1,c1),xCross(i,2,c1),
			 grid2,xCross(i,0,c2),xCross(i,1,c2),xCross(i,2,c2),
			 dist,tolerance*max(gridScale(grid1),gridScale(grid2)) );
		}
		// determine the grid spacing in the tangential direction as a measure of how close
		// the co-incident points should be -- this actually depends on (x2-x1).normal

		// we can also check that n1.n2 > 0 to avoid the thin wing problem
		int iv[3], jv[3], ivp[3], jvp[3];    
		int &i1=iv[0], &i2=iv[1], &i3=iv[2];
		int &j1=jv[0], &j2=jv[1], &j3=jv[2];
	      
		iv[0]=int(xCross(i,xi1,c1)+.5);  
		iv[1]=int(xCross(i,xi2,c1)+.5);
		iv[2]=numberOfDimensions>2 ? int(xCross(i,xi3,c1)+.5) : cg[grid1].extendedIndexRange()(Start,axis3);

		jv[0]=int(xCross(i,xi1,c2)+.5);
		jv[1]=int(xCross(i,xi2,c2)+.5);
		jv[2]=numberOfDimensions>2 ? int(xCross(i,xi3,c2)+.5) : cg[grid2].extendedIndexRange()(Start,axis3);
		// The dot product of the boundary normals should be positive
		real nDot = sum(cg[grid1].vertexBoundaryNormal(side1,dir1)(i1,i2,i3,Rx)*
				cg[grid2].vertexBoundaryNormal(side2,dir2)(j1,j2,j3,Rx));

		// alpha = |cos(theta)| = angle between the ray and the normal , |(x2-x1).n| / /| x2-x1 \|
		// choose one normal, could average?
		real alpha = fabs( cg[grid1].vertexBoundaryNormal(side1,dir1)(i1,i2,i3,axis2)); 
	      
		const realArray & x1 = cg[grid1].center();
		const realArray & x2 = cg[grid2].center();
		const intArray & mask1 = cg[grid1].mask();
		const intArray & mask2 = cg[grid2].mask();
	      
		// estimate the expected error in the normal direction
		real dn1=0., dn2=0.;
		ivp[0]=iv[0]; ivp[1]=iv[1]; ivp[2]=iv[2];
		jvp[0]=jv[0]; jvp[1]=jv[1]; jvp[2]=jv[2];
		ivp[dir1]+=1-2*side1;
		dn1=sum(fabs(x1(ivp[0],ivp[1],ivp[2],Rx) - x1(iv[0],iv[1],iv[2],Rx)))
		  *cg[grid1].sharedBoundaryTolerance()(side1,dir1);
		ivp[dir1]=iv[dir1];

		jvp[dir2]+=1-2*side2;
		dn2=sum(fabs(x2(jvp[0],jvp[1],jvp[2],Rx) - x2(jv[0],jv[1],jv[2],Rx)))
		  *cg[grid2].sharedBoundaryTolerance()(side2,dir2);
		jvp[dir2]=jv[dir2];

		real dn=max(dn1,dn2);
	      
		// find the maximum length of the cell edges (tangential direction only)
		real dt1=0., dt2=0.;
		for( int dir=0; dir<numberOfDimensions-2; dir++ )
		{
		  int axisp = (dir1+dir+1) % numberOfDimensions; // tangential direction
		  ivp[axisp]++;
		  dt1=max(dt1, sum(fabs(x1(ivp[0],ivp[1],ivp[2],Rx) - x1(iv[0],iv[1],iv[2],Rx))) );
		  ivp[axisp]--;

		  axisp = (dir2+dir+1) % numberOfDimensions;
		  jvp[axisp]++;
		  dt2=max(dt2, sum(fabs(x2(jvp[0],jvp[1],jvp[2],Rx) - x2(jv[0],jv[1],jv[2],Rx))) );
		  jvp[axisp]--;
		}
		real dt=max(dt1,dt2);
		// check that at least one corner of the cell is interpolated
		bool interp1=(mask1(i1,i2,i3) & MappedGrid::ISinterpolationPoint) !=0 ;
		if( !interp1 )
		{
		  int axisp1 = (dir1+1) % numberOfDimensions;
		  int axisp2 = (dir1+2) % numberOfDimensions;
		
		  for( int m=1; m<numberOfDimensions*2-2; m++ )
		  {
		    ivp[axisp1]+=m % 2;
		    ivp[axisp2]+=m/2;
		    interp1 = interp1 || (mask1(ivp[0],ivp[1],ivp[2])& MappedGrid::ISinterpolationPoint );
		    ivp[axisp1]=iv[axisp1];
		    ivp[axisp2]=iv[axisp2];
		    if( interp1 )
		      break;
		  }
		}
		// check that at least one corner of the cell is interpolated
		bool interp2=(mask2(j1,j2,j3) & MappedGrid::ISinterpolationPoint) !=0 ;
		if( !interp2 )
		{
		  int axisp1 = (dir2+1) % numberOfDimensions;
		  int axisp2 = (dir2+2) % numberOfDimensions;
		
		  for( int m=1; m<numberOfDimensions*2-2; m++ )
		  {
		    jvp[axisp1]+=m % 2;
		    jvp[axisp2]+=m/2;
		    interp2 = interp2 || (mask2(jvp[0],jvp[1],jvp[2])& MappedGrid::ISinterpolationPoint );
		    jvp[axisp1]=jv[axisp1];
		    jvp[axisp2]=jv[axisp2];
		    if( interp2 )
		      break;
		  }
		}
                if( info & 2 ) 
		{
		  printf(" n1.n2=%6.1e, dn=%7.2e, dt=%7.2e, alpha=%7.2e, dist=%e <? alpha*dn+(1-alpha)*dt=%8.3e \n",
			 nDot,dn,dt,alpha,dist,alpha*dn+(1.-alpha)*dt);
		  printf(" (i1,i2,i3)=(%i,%i,%i), (side,axis)=(%i,%i) n1=(%7.2e,%7.2e,%7.2e), interp1=%i \n",
			 i1,i2,i3,side1,dir1,
			 cg[grid1].vertexBoundaryNormal(side1,dir1)(i1,i2,i3,0),
			 cg[grid1].vertexBoundaryNormal(side1,dir1)(i1,i2,i3,1),
			 cg[grid1].vertexBoundaryNormal(side1,dir1)(i1,i2,i3,2),interp1);
		  printf(" (j1,j2,j3)=(%i,%i,%i), (side,axis)=(%i,%i) n2=(%7.2e,%7.2e,%7.2e), interp2=%i\n",
			 j1,j2,j3,side2,dir2,
			 cg[grid2].vertexBoundaryNormal(side2,dir2)(j1,j2,j3,0),
			 cg[grid2].vertexBoundaryNormal(side2,dir2)(j1,j2,j3,1),
			 cg[grid2].vertexBoundaryNormal(side2,dir2)(j1,j2,j3,2),interp2);
		  
		}
	      }
	    }  // else if
	  }
	  
	  //if( FALSE )
	  //  printf("multiple crossing: point %i : (%e,%e) distict crossings =%i \n",
	  //	   c1,xCross(i,0,c1),xCross(i,1,c1),numberOfDistinctCrossings);
	}
      }
    }
    crossings(i)=numberOfDistinctCrossings;
  }
    return 0;
}

int Ogen::
checkCrossings(CompositeGrid & cg,
	       const int & numToCheck, 
	       const IntegerArray & ia, 
	       intArray & mask,
	       realArray & x,
	       realArray & center,
	       IntegerArray & crossings,
	       const Range & Rx,
               const int & usedPoint )
// ==============================================================================
// 
// This is a utility routine for removeExteriorPoints
// 
// ==============================================================================
{
	  
  if( numToCheck<=0 )
    return 0;

  Range R(0,numToCheck-1);
  x.redim(R,Rx);
  crossings.redim(R);
  int dir;
  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
    x(R,dir)=center(ia(R,0),ia(R,1),ia(R,2),dir);
                  
  countCrossingsWithRealBoundary( cg,x,crossings );
  // a point found inside is marked as a used point
  where( crossings % 2 == 0 )
  {
    mask(ia(R,0),ia(R,1),ia(R,2))=0;
  }
  otherwise()
  {
    mask(ia(R,0),ia(R,1),ia(R,2))=usedPoint;
  }
  if( holePoint.getLength(0) <= numberOfHolePoints+numToCheck )
    holePoint.resize(holePoint.getLength(0)*2+numToCheck,Rx);
    
  for( int i=0; i<numToCheck; i++ )
  {
    if( mask(ia(i,0),ia(i,1),ia(i,2))==0 )
    {
      for( dir=0; dir<cg.numberOfDimensions(); dir++ )
        holePoint(numberOfHolePoints++,dir) = x(i,dir);
    }
  }
  return 0;
}



int Ogen::
removeExteriorPoints(CompositeGrid & cg, 
		     const bool boundariesHaveCutHoles /* = FALSE */ )
//
// /Description:
//   Use a ray tracing algorithm to remove points outside the domain.
//
// /numberOfHolePoints_ (input) : current number of entries in the array holePoint
//
{
#ifndef USE_PPP
  real time0=getCPU();
  
  if( info & 4 ) printf("removing exterior points by ray tracing...\n");

  const int numberOfBaseGrids = cg.numberOfBaseGrids();

  int grid;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    // make sure the bounding boxes are initialized  // *********************** this is fixed, remove ******
    cg[grid].mapping().getMapping().approximateGlobalInverse->initialize();
  }
  

  Index Iv[3];
  Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range R,Rx(0,cg.numberOfDimensions()-1), I;
  realArray x;
  IntegerArray ia, crossings;

  int i1,i2,i3;  

  int maxNumberOfHolePoints=10000*numberOfBaseGrids*cg.numberOfDimensions();
  if( holePoint.getLength(0)<maxNumberOfHolePoints )
    holePoint.resize(maxNumberOfHolePoints,cg.numberOfDimensions());
  
  
  // For cell centred grids copy the mask from the last cell to the first ghost cell as
  // this info is used by the ray-tracing algorithm
  if( cg[0].isAllCellCentered() )
  {
    Index I1g,I2g,I3g;
    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
      MappedGrid & g = cg[grid];
      intArray & mask = g.mask();
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


  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    intArray & mask = g.mask();

    if( info & 4 ) 
      printf("removing points on grid %s... \n",(const char*)g.mapping().getName(Mapping::mappingName));

    if( info & 8  )
    {
      // *** for testing ****
      getIndex(g.extendedIndexRange(),I1,I2,I3,-1);  // by default exclude boundaries
      for(;;)
      {
	printf("grid=%i, (%i,%i)x(%i,%i)x(%i,%i) \n",grid,I1.getBase(),I1.getBound(),
	       I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	cout << "Enter a point to check (i1,i2,i3) \n";
	cin >> i1 >> i2 >> i3;
        if( i1<0 )
          break;
	x.redim(1,cg.numberOfDimensions());
        for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
    	  x(0,dir)=g.center()(i1,i2,i3,dir);

        countCrossingsWithRealBoundary( cg,x,crossings );

        printf("x=(%e,%e,%e) crossings=%i \n",x(0,0),x(0,1),x(0,2),crossings(0));
	
      }
    }

    // remove points from interior points plus periodic/interpolation boundaries
    getIndex(g.extendedIndexRange(),I1,I2,I3,-1);  // by default exclude boundaries
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      if( g.isPeriodic()(axis) )
	Iv[axis]=Range(g.gridIndexRange()(Start,axis),g.gridIndexRange()(End,axis));   // note: Iv[.] == I1,I2,I3
      else
      {
	if( g.boundaryCondition()(Start,axis) == 0 )
	  Iv[axis]=Range(g.extendedIndexRange()(Start,axis),Iv[axis].getBound());   // note: Iv[.] == I1,I2,I3
        if( g.boundaryCondition()(End,axis) == 0 )
	  Iv[axis]=Range(Iv[axis].getBase(),g.extendedIndexRange()(End,axis));
      }
    }

    // Check points to see if they are outside the domain
    if( boundariesHaveCutHoles )
    {
      // =====================================================================================
      // only check points near hole points
      // Successively check points to the left, right, bottom, top, front, back of any
      // hole point for new hole points. Normally this should find all points on one pass.
      // but we try passes until there are no more changes.

	  
      bool vectorize=TRUE;
      if( vectorize )
      {
	ia.redim(SQR(max(I1.length(),I2.length(),I3.length())),3);
	  
	bool done=FALSE;
	// We mark points that we find inside with "usedPoint", this value should just be
	// different from MappedGrid::ISdiscretizationPoint (and positive)
	const int usedPoint = MappedGrid::ISdiscretizationPoint | MappedGrid::ISreservedBit0;
	  
	while( !done )
	{
	  done=TRUE; // set to FALSE if any changes are made.

	  // scan left to right
          int numToCheck=0;
	  for( i1=I1.getBase()+1; i1<=I1.getBound(); i1++ )
	  {
	    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    {
	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      {
		if( mask(i1-1,i2,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
                  ia(numToCheck,0)=i1;
                  ia(numToCheck,1)=i2;
                  ia(numToCheck,2)=i3;
		  numToCheck++;
		}
	      }
	    }
	  }
          checkCrossings( cg,numToCheck, ia, mask,x,g.center(),crossings,Rx,usedPoint );
	  
	  // scan right to left
          numToCheck=0;
	  for( i1=I1.getBound()-1; i1>=I1.getBase(); i1-- )
	  {
	    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    {
	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      {
		if( mask(i1+1,i2,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
                  ia(numToCheck,0)=i1;
                  ia(numToCheck,1)=i2;
                  ia(numToCheck,2)=i3;
		  numToCheck++;
		}
	      }
	    }
	  }
          checkCrossings( cg,numToCheck, ia, mask,x,g.center(),crossings,Rx,usedPoint );

	  // scan bottom to top
          numToCheck=0;
	  for( i2=I2.getBase()+1; i2<=I2.getBound(); i2++ )
	  {
	    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    {
	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
		if( mask(i1,i2-1,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
                  ia(numToCheck,0)=i1;
                  ia(numToCheck,1)=i2;
                  ia(numToCheck,2)=i3;
		  numToCheck++;
		}
	      }
	    }
	  }
          checkCrossings( cg,numToCheck, ia, mask,x,g.center(),crossings,Rx,usedPoint );

	  // scan top to bottom
          numToCheck=0;
	  for( i2=I2.getBound()-1; i2>=I2.getBase(); i2-- )
	  {
	    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    {
	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
		if( mask(i1,i2+1,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
                  ia(numToCheck,0)=i1;
                  ia(numToCheck,1)=i2;
                  ia(numToCheck,2)=i3;
		  numToCheck++;
		}
	      }
	    }
	  }
          checkCrossings( cg,numToCheck, ia, mask,x,g.center(),crossings,Rx,usedPoint );

	  if( cg.numberOfDimensions()>2 )
	  {
	    // scan front to back
            numToCheck=0;
	    for( i3=I3.getBase()+1; i3<=I3.getBound(); i3++ )
	    {
	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      {
		for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		{
		  if( mask(i1,i2,i3-1)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		  {
		    done=FALSE;
		    ia(numToCheck,0)=i1;
		    ia(numToCheck,1)=i2;
		    ia(numToCheck,2)=i3;
		    numToCheck++;
		  }
		}
	      }
	    }
	    checkCrossings( cg,numToCheck, ia, mask,x,g.center(),crossings,Rx,usedPoint );
	    // scan back to front
            numToCheck=0;
	    for( i3=I3.getBound()-1; i3>=I3.getBase(); i3-- )
	    {
	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      {
		for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		{
		  if( mask(i1,i2,i3+1)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		  {
		    done=FALSE;
		    ia(numToCheck,0)=i1;
		    ia(numToCheck,1)=i2;
		    ia(numToCheck,2)=i3;
		    numToCheck++;
		  }
		}
	      }
	    }
	    checkCrossings( cg,numToCheck, ia, mask,x,g.center(),crossings,Rx,usedPoint );

	  }
	}

      }
      else
      {
	x.redim(1,cg.numberOfDimensions());
	  
	bool done=FALSE;
	// We mark points that we find inside with "usedPoint", this value should just be
	// different from MappedGrid::ISdiscretizationPoint (and positive)
	const int usedPoint = MappedGrid::ISdiscretizationPoint | MappedGrid::ISreservedBit0;
	  
	while( !done )
	{
	  done=TRUE; // set to FALSE if any changes are made.
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	  {

	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    {
	      // scan left to right
	      for( i1=I1.getBase()+1; i1<=I1.getBound(); i1++ )
	      {
		if( mask(i1-1,i2,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
		  for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		    x(0,dir)=g.center()(i1,i2,i3,dir);
		  countCrossingsWithRealBoundary( cg,x,crossings );
		  // a point found inside is marked as a used point
		  mask(i1,i2,i3)=  crossings(0) % 2 == 0 ? 0 : usedPoint;
		  if( mask(i1,i2,i3)==0 ) holePoint(numberOfHolePoints++,Rx) = x(0,Rx);
		  assert( numberOfHolePoints < maxNumberOfHolePoints );
		}
	      }
	      // scan right to left

	      for( i1=I1.getBound()-1; i1>=I1.getBase(); i1-- )
	      {
		if( mask(i1+1,i2,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
		  for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		    x(0,dir)=g.center()(i1,i2,i3,dir);
		  countCrossingsWithRealBoundary( cg,x,crossings );
		  // a point found inside is marked as a used point
		  mask(i1,i2,i3)=  crossings(0) % 2 == 0 ? 0 : usedPoint; 
		  if( mask(i1,i2,i3)==0 ) holePoint(numberOfHolePoints++,Rx) = x(0,Rx);
		  assert( numberOfHolePoints < maxNumberOfHolePoints );
		}
	      }
	    }

	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      // scan bottom to top
	      for( i2=I2.getBase()+1; i2<=I2.getBound(); i2++ )
	      {
		if( mask(i1,i2-1,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
		  for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		    x(0,dir)=g.center()(i1,i2,i3,dir);
		  countCrossingsWithRealBoundary( cg,x,crossings );
		  // a point found inside is marked as a used point
		  mask(i1,i2,i3)=  crossings(0) % 2 == 0 ? 0 : usedPoint; 
		  if( mask(i1,i2,i3)==0 ) holePoint(numberOfHolePoints++,Rx) = x(0,Rx);
		  assert( numberOfHolePoints < maxNumberOfHolePoints );
		}
	      }
	      // scan top to bottom
	      for( i2=I2.getBound()-1; i2>=I2.getBase(); i2-- )
	      {
		if( mask(i1,i2+1,i3)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		{
		  done=FALSE;
		  for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		    x(0,dir)=g.center()(i1,i2,i3,dir);
		  countCrossingsWithRealBoundary( cg,x,crossings );
		  // a point found inside is marked as a used point
		  mask(i1,i2,i3)=  crossings(0) % 2 == 0 ? 0 : usedPoint; 
		  if( mask(i1,i2,i3)==0 ) holePoint(numberOfHolePoints++,Rx) = x(0,Rx);
		  assert( numberOfHolePoints < maxNumberOfHolePoints );
		}
	      }
	    }
	  }
	  if( cg.numberOfDimensions()>2 )
	  {
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    {
	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
		// scan front to back
		for( i3=I3.getBase()+1; i3<=I3.getBound(); i3++ )
		{
		  if( mask(i1,i2,i3-1)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		  {
		    done=FALSE;
		    for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		      x(0,dir)=g.center()(i1,i2,i3,dir);
		    countCrossingsWithRealBoundary( cg,x,crossings );
		    // a point found inside is marked as a used point
		    mask(i1,i2,i3)=  crossings(0) % 2 == 0 ? 0 : usedPoint; 
		    if( mask(i1,i2,i3)==0 ) holePoint(numberOfHolePoints++,Rx) = x(0,Rx);
		    assert( numberOfHolePoints < maxNumberOfHolePoints );
		  }
		}
		// scan back to front
		for( i3=I3.getBound()-1; i3>=I3.getBase(); i3-- )
		{
		  if( mask(i1,i2,i3+1)==0 && mask(i1,i2,i3)==MappedGrid::ISdiscretizationPoint )
		  {
		    done=FALSE;
		    for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		      x(0,dir)=g.center()(i1,i2,i3,dir);
		    countCrossingsWithRealBoundary( cg,x,crossings );
		    // a point found inside is marked as a used point
		    mask(i1,i2,i3)=  crossings(0) % 2 == 0 ? 0 : usedPoint; 
		    if( mask(i1,i2,i3)==0 ) holePoint(numberOfHolePoints++,Rx) = x(0,Rx);
		    assert( numberOfHolePoints < maxNumberOfHolePoints );
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    else
    {
      // ***** for testing, boundaries have not cut holes ******

      x.redim(I1,I2,I3,Rx);
      x(I1,I2,I3,Rx)=g.center()(I1,I2,I3,Rx);
      I=Range(I1.length()*I2.length()*I3.length());
      x.reshape(I,Rx);
      countCrossingsWithRealBoundary( cg,x,crossings );
      for( int i=I.getBase(); i<=I.getBound(); i++ )
      {
	if( crossings(i) % 2 == 0 )
	{
	  // This point is outside
	  holePoint(numberOfHolePoints,Rx) = x(i,Rx);
	  numberOfHolePoints++;
	  assert( numberOfHolePoints < maxNumberOfHolePoints );
	}
      }
    }
  }
  real time=getCPU();
  if( info & 2 ) 
    printf(" time to remove exterior points...........................%e (total=%e)\n",time-time0,time-totalTime);
  timeRemoveExteriorPoints=time-time0;
  

#endif // USE_PPP
  return numberOfHolePoints;
}

int Ogen::
findTrueBoundary(CompositeGrid & cg)
//
// Mark points on physical boundaries that are not really part of the true boundary.
//
// As an example this routine will zero out all the points marked D below:
//
//                                           bc>0
//             --------I---I--I--I--I-I-I-O--D--D--D----D---D--+     0 = holes cut by boundary
//            grid 1   |   |  |  |  |   |        grid 1        |     I = interpolation points on the boundary
//                     |   |  |  |  |   |0                     D
//                     |----------------|                      |
//                     |   |  |  |  |   |0                     |<---- this boundary should be removed
//                     |   |  |  |  |   |                      D           (points marked D)
//                     |----------------|                      |
//                     |                |<- bc>0               |bc>0
//                     |                |                      |
//                     |   grid 2       |                      D
//                     |                |                      |
//
// This routine assumes the following have been done:
//   (1) Find where boundary points of one grid can interpolate from the boundary 
//       other grids. 
//   (2) Mark points that are outside near these points
//
//  This routine will then `zero' out discretization points (D) that are outside the domain.
//  Theses are recognized as being discretization points that are next to hole points.
//
//    
{
  real time0=getCPU();
  
  if( info & 4 ) printf("finding the true boundary...\n");

  Index Iv[3];
  Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int i1,i2,i3;  
  const int numberOfBaseGrids = cg.numberOfBaseGrids();

  int grid;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    intArray & mask = g.mask();

    if( info & 4 ) 
      printf("checking boundaries of grid: %s,...\n",(const char*)g.mapping().getName(Mapping::mappingName));

    for( int iteration=0; iteration<=1; iteration++ )
    {
      // we may have to iterate twice to get boundaries like the one on the right in the
      // above figure since we first sweep out the points on the top and then sweep the
      // out the points on the right   *** could try and avoid the second iteration ****

      for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( g.boundaryCondition()(side,axis) > 0 &&
              g.mapping().getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity )
	  {
	    // this side is a physical boundary
	    // mark all discretization (D) points that are next to hole points or any
	    // D points that can reach a hole point through D points.

	    getBoundaryIndex(g.extendedIndexRange(),side,axis,I1,I2,I3);  // %%wdh
            // include periodic edges ??
            for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
	    {
	      if( dir!=axis && g.isPeriodic()(dir) )
		Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound()+1);
	    }

	    if( cg.numberOfDimensions()==2 )
	    {
	      int is1 = axis==axis1 ? 0 : 1;
	      int is2 = axis==axis2 ? 0 : 1;
	      i3=I3.getBase();
	      // sweep left to right (or bottom to top)
	      for( i2=I2.getBase(); i2<=I2.getBound()-is2; i2++ )
		for( i1=I1.getBase(); i1<=I1.getBound()-is1; i1++ )
		{
		  if( mask(i1,i2,i3)==0 && ( mask(i1+is1,i2+is2,i3) & MappedGrid::ISdiscretizationPoint ))
		    mask(i1+is1,i2+is2,i3)=0;
		}
	      // sweep right to left to right (or top to bottom)
	      for( i2=I2.getBound(); i2>=I2.getBase()+is2; i2-- )
		for( i1=I1.getBound(); i1>=I1.getBase()+is1; i1-- )
		{
		  if( mask(i1,i2,i3)==0 && ( mask(i1-is1,i2-is2,i3) & MappedGrid::ISdiscretizationPoint ))
		    mask(i1-is1,i2-is2,i3)=0;
		}
	    }
	    else
	    {
	      // 3D : first sweep back and forth in i1, then back and forth in i2 and finally i3
	      if( axis!=axis1 )
	      {
		for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  {
		    // sweep right
		    for( i1=I1.getBase(); i1<=I1.getBound()-1; i1++ )
		    {
		      if( mask(i1,i2,i3)==0 && ( mask(i1+1,i2,i3) & MappedGrid::ISdiscretizationPoint ))
			mask(i1+1,i2,i3)=0;
		    }
		    // sweep left
		    for( i1=I1.getBound(); i1>=I1.getBase()+1; i1-- )
		    {
		      if( mask(i1,i2,i3)==0 && ( mask(i1-1,i2,i3) & MappedGrid::ISdiscretizationPoint ))
			mask(i1-1,i2,i3)=0;
		    }
		  }
	      }
	      if( axis!=axis2 )
	      {
		for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		  {
		    // sweep bottom to top
		    for( i2=I2.getBase(); i2<=I2.getBound()-1; i2++ )
		    {
		      if( mask(i1,i2,i3)==0 && ( mask(i1,i2+1,i3) & MappedGrid::ISdiscretizationPoint ))
			mask(i1,i2+1,i3)=0;
		    }
		    // sweep top top bottom
		    for( i2=I2.getBound(); i2>=I2.getBase()+1; i2-- )
		    {
		      if( mask(i1,i2,i3)==0 && ( mask(i1,i2-1,i3) & MappedGrid::ISdiscretizationPoint ))
			mask(i1,i2-1,i3)=0;
		    }
		  }
	      }
	      if( axis!=axis3 )
	      {
		for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		  {
		    // sweep bottom to top
		    for( i3=I3.getBase(); i3<=I3.getBound()-1; i3++ )
		    {
		      if( mask(i1,i2,i3)==0 && ( mask(i1,i2,i3+1) & MappedGrid::ISdiscretizationPoint ))
			mask(i1,i2,i3+1)=0;
		    }
		    // sweep top top bottom
		    for( i3=I3.getBound(); i3>=I3.getBase()+1; i3-- )
		    {
		      if( mask(i1,i2,i3)==0 && ( mask(i1,i2,i3-1) & MappedGrid::ISdiscretizationPoint ))
			mask(i1,i2,i3-1)=0;
		    }
		  }

	      }
	    }
	  }
	}
      }
    }
  }
  
  real time=getCPU();
  if( info & 2 ) 
    printf(" time to find the true boundary...........................%e (total=%e)\n",time-time0,time-totalTime);
  timeFindTrueBoundary=time-time0;
  
  return 0;
}
