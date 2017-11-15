// This file automatically generated from cutHolesNew.bC with bpp.
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
  #define GET_LOCAL(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
  #define GET_LOCAL_CONST(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
  #define GET_LOCAL(type,xd,xs)type ## SerialArray & xs = xd
  #define GET_LOCAL_CONST(type,xd,xs)const type ## SerialArray & xs = xd
#endif

#define  FOR_3(i1,i2,i3,I1,I2,I3)I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();for( i3=I3Base; i3<=I3Bound; i3++ )  for( i2=I2Base; i2<=I2Bound; i2++ )  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)int I1Base,I2Base,I3Base;int I1Bound,I2Bound,I3Bound;I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();for( i3=I3Base; i3<=I3Bound; i3++ )  for( i2=I2Base; i2<=I2Bound; i2++ )  for( i1=I1Base; i1<=I1Bound; i1++ )


// **********************************************************************************************************
//   if the point was NOT invertible then we double check to see it is outside of the boundary
// **********************************************************************************************************


// **********************************************************************************************************
//   *OLD* if the point was NOT invertible then we double check to see it is outside of the boundary
// **********************************************************************************************************





// ****************************************************************************************
// ************** Double check the points that we marked as holes  ************************
// ****************************************************************************************



int Ogen::
cutHolesNew(CompositeGrid & cg)
// =======================================================================================================
// NEW version for parallel
//
// /Description:
// For each physical boundary of each grid:
//     Find all points on other grids that are outside the boundary.
//
// Note: for a cell-centred grid we still use the vertex boundary values to cut holes.
//
// =======================================================================================================
{
    real time0=getCPU();
//  info |= 4;

  // When we cut holes, for each cutter point we form a region on the cuttee grid of points
  // to check. The maximum with of this region is the maxiumHoleWidth.
    const int maximumHoleWidth=10;  // 5 

    const int numberOfBaseGrids=cg.numberOfBaseGrids();
    const int numberOfDimensions = cg.numberOfDimensions();

    int numberOfExplicitHoleCutters=explicitHoleCutter.size();

  // We plot any suspicious discretization points that are too close to hole points as 'orphan' points
    numberOfOrphanPoints=0;
    plotOrphanPoints.redim(numberOfBaseGrids);
    plotOrphanPoints=2;  // colour orphan pts by grid number
    orphanPoint.redim(100,numberOfDimensions+1);

    if( numberOfBaseGrids==1 ) return 0;
    
    Range G(0,numberOfBaseGrids-1);
    const int maxNumberCutting= max(cg.mayCutHoles(G,G));
    if( maxNumberCutting==0 && numberOfManualHoles==0 && numberOfExplicitHoleCutters==0 )
    {
        return 0;
    }
    

    if( info & 4 ) printf("cutting holes with physical boundaries...\n");

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Range R, R1, Rx(0,numberOfDimensions-1);
    realSerialArray x,r,rr;
    realSerialArray x2(1,3), r2(1,3);
    intSerialArray ia,ia2;
    
    const real maxDistFactor=SQR(2.); // cut holes within 2*( cell diagonal length of cutee grid )


    int  iv[3], &i1 = iv[0],  &i2= iv[1], &i3 = iv[2];
    int  jv[3], &j1 = jv[0],  &j2= jv[1], &j3 = jv[2];
    int ipv[3], &i1p=ipv[0], &i2p=ipv[1], &i3p=ipv[2];
    int jpv[3], &j1p=jpv[0], &j2p=jpv[1], &j3p=jpv[2];

  // *wdh* 070413  int maxNumberOfHolePoints=10000*numberOfBaseGrids*numberOfDimensions;  // **** fix this 

    int numberOfLocalGridPoints=0;
    for( int grid=0; grid<numberOfBaseGrids; grid++ )
    {
        const intSerialArray & mask =cg[grid].mask().getLocalArray();
        numberOfLocalGridPoints+=mask.elementCount();
    }
  // estimate the maximum number of hole points as a fraction of the total number of points
    int maxNumberOfHolePoints = max(100, int(numberOfLocalGridPoints*.2) );
    

    numberOfHolePoints=0;
    holePoint.redim(maxNumberOfHolePoints,numberOfDimensions+1);
    
  // const real boundaryAngleEps=.01;
  // const real boundaryNormEps=0.; // *wdh* 980126 ** not needed since we double check//  1.e-2;
    int pHoleMarker[3];
    #define holeMarker(axis) pHoleMarker[axis]

    real xv[3]={0.,0.,0.};
    real rv[3]={0.,0.,0.};

    const real biggerBoundaryEps = sqrt(boundaryEps);


  // *****************************************************************************
  //   cutShare[grid](i1,i2,i3) : when a physical boundary cuts a hole in another grid
  //         we keep track of the share value of the cutter grid so that we can prevent
  //         shared boundaries from cutting holes where they shouldn't. Normally shared
  //         boundaries do not cut holes so this flag is not needed. It is needed for
  //         fillet/collar type grids when the use has specified that share boundaries
  //         may cut holes.
  // *****************************************************************************

    int grid,dir; 
    intSerialArray *cutShare= new intSerialArray [numberOfBaseGrids];
    
    int maximumSharedBoundaryFlag=0;
    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
        intArray & maskd =cg[grid].mask();
        GET_LOCAL(int,maskd,mask);

        cutShare[grid].redim(mask);
        cutShare[grid]=0;
        maximumSharedBoundaryFlag=max(maximumSharedBoundaryFlag,max(cg[grid].sharedBoundaryFlag()));
    }
    
  // ********************************************************
  // *** Mark non-cutting portions of physical boundaries ***
  // ********************************************************
    int n;
    for( n=0; n<numberOfNonCuttingBoundaries; n++ )
    {
        int grid=nonCuttingBoundaryPoints(n,0);
        int i1a=nonCuttingBoundaryPoints(n,1);
        int i1b=nonCuttingBoundaryPoints(n,2);
        int i2a=nonCuttingBoundaryPoints(n,3);
        int i2b=nonCuttingBoundaryPoints(n,4);
        int i3a=nonCuttingBoundaryPoints(n,5);
        int i3b=nonCuttingBoundaryPoints(n,6);

        Index I1=Range(i1a,i1b);
        Index I2=Range(i2a,i2b);
        Index I3=Range(i3a,i3b);
        MappedGrid & mg = cg[grid];
        intArray & maskd = mg.mask();
        GET_LOCAL(int,maskd,mask);

        bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
        if( !ok ) continue;

        where( mask(I1,I2,I3)!=0 )
        {
            mask(I1,I2,I3) |= ISnonCuttingBoundaryPoint;
        }
    }
    


  // *************************
  // *** Cut manual holes  ***
  // *************************
    if( numberOfManualHoles>0 )
    {
        for( int hole=0; hole<numberOfManualHoles; hole++ )
        {
            int grid=manualHole(hole,0);
            int i1a=manualHole(hole,1);
            int i1b=manualHole(hole,2);
            int i2a=manualHole(hole,3);
            int i2b=manualHole(hole,4);
            int i3a=manualHole(hole,5);
            int i3b=manualHole(hole,6);
            
            printf(" Cut the manual hole [%i,%i]x[%i,%i]x[%i,%i] in grid %s\n",
           	     i1a,i1b,i2a,i2b,i3a,i3b, (const char*)cg[grid].getName() );

            assert( grid>=0 && grid<numberOfBaseGrids );
            
            MappedGrid & g = cg[grid];
            intArray & maskd = g.mask();
      // const realArray & vertex = g.vertex();
            const bool isRectangular = g.isRectangular();

            GET_LOCAL(int,maskd,mask);
            #ifdef USE_PPP
                realSerialArray vertex; if( !isRectangular ) getLocalArrayWithGhostBoundaries(g.vertex(),vertex);
            #else
                const realSerialArray & vertex = g.vertex();
            #endif

            real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
            int iv0[3]={0,0,0}; //
            if( isRectangular )
            {
      	g.getRectangularGridParameters( dvx, xab );
      	for( int dir=0; dir<g.numberOfDimensions(); dir++ )
        	  iv0[dir]=g.gridIndexRange(0,dir);
            }
            #define XV(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

            I1=Range(i1a,i1b);
            I2=Range(i2a,i2b);
            I3=Range(i3a,i3b);

            bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
            if( !ok ) continue;

            i1a=I1.getBase(); i1b=I1.getBound();
            i2a=I2.getBase(); i2b=I2.getBound();
            i3a=I3.getBase(); i3b=I3.getBound();

            mask(I1,I2,I3)=0;
            
            int numHoles=(i1b-i1a+1)*(i2b-i2a+1)*(i3b-i3a+1);
            if( numberOfHolePoints+numHoles>= maxNumberOfHolePoints )
            {
      	maxNumberOfHolePoints*=2; 
                maxNumberOfHolePoints+=numHoles;
      	holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
      	printf(" ... increasing maxNumberOfHolePoints to %i\n",maxNumberOfHolePoints);
            }
            int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
            for( i3=i3a; i3<=i3b; i3++ )
            {
      	for( i2=i2a; i2<=i2b; i2++ )
      	{
        	  for( i1=i1a; i1<=i1b; i1++ )
        	  {
                        if( !isRectangular )
          	    {
            	      for( int axis=axis1; axis<numberOfDimensions; axis++ )
            		holePoint(numberOfHolePoints,axis)=vertex(i1,i2,i3,axis);
          	    }
          	    else
          	    {
            	      for( int axis=axis1; axis<numberOfDimensions; axis++ )
            		holePoint(numberOfHolePoints,axis)=XV(iv,axis);
          	    }
          	    
                        holePoint(numberOfHolePoints,numberOfDimensions)=grid;
          	    numberOfHolePoints++;
        	  }
      	}
            }
        }
    }  
    
    if( maxNumberCutting==0 && numberOfExplicitHoleCutters==0 )
    {
        return numberOfHolePoints;
    }


  // ******************************************
  // --------- Explicit hole cutting ----------
  // -- Cut holes with user defined mappings --
  // ******************************************
    explicitHoleCutting( cg );

    maxNumberOfHolePoints = holePoint.getLength(0);  // this may have changed

  // **********************************************************************
  //    Build boundaries that cut holes (build a copy on this processor)
  //
  //  xb(side,axis,grid)
  // **********************************************************************

    const int maxNumberOfBoundaries=numberOfBaseGrids*2*numberOfDimensions;
    RealArray **ppxBoundary = new RealArray* [maxNumberOfBoundaries];
#define pxBoundary(side,axis,grid) (ppxBoundary[(side)+2*((axis)+numberOfDimensions*(grid))])
#define xBoundary(side,axis,grid) (*pxBoundary(side,axis,grid))
    for( int b=0; b<maxNumberOfBoundaries; b++ )
        ppxBoundary[b]=NULL;
        
    
    for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
    {
        MappedGrid & g = cg[grid];
        Mapping & map = g.mapping().getMapping();
        const bool isRectangular = g.isRectangular();
        #ifdef USE_PPP
            realSerialArray vertex; if( !isRectangular ) getLocalArrayWithGhostBoundaries(g.vertex(),vertex);
        #else
            const realSerialArray & vertex = g.vertex();
        #endif
        for( int axis=axis1; axis<numberOfDimensions; axis++ )
        {
            for( int side=Start; side<=End; side++ )
            {
        // Note: do not cut holes with singular sides
                if( g.boundaryCondition(side,axis) > 0 && 
                        map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity ) 
      	{

        	  getIndex(g.dimension(),I1,I2,I3); 
                    Iv[axis]=g.gridIndexRange(side,axis);

        	  if( !isRectangular )
        	  {
          	    pxBoundary(side,axis,grid) = new RealArray(I1,I2,I3,numberOfDimensions);
          	    
                        Range Rx= numberOfDimensions;
          	    if( false )
          	    {
            	      xBoundary(side,axis,grid)(I1,I2,I3,Rx)=vertex(I1,I2,I3,Rx); // do this for now
          	    }
          	    else
          	    {
            	      int numPoints = I1.getLength()*I2.getLength()*I3.getLength();
	      // xBoundary(side,axis,grid).redim(numPoints,Rx);

            	      RealArray rb(I1,I2,I3,numberOfDimensions);
            	      real *rbp = rb.Array_Descriptor.Array_View_Pointer3;
            	      const int rbDim0=rb.getRawDataSize(0);
            	      const int rbDim1=rb.getRawDataSize(1);
            	      const int rbDim2=rb.getRawDataSize(2);
#define RB(i0,i1,i2,i3) rbp[i0+rbDim0*(i1+rbDim1*(i2+rbDim2*(i3)))]	

            	      real dr[3]={g.gridSpacing(0),g.gridSpacing(1),g.gridSpacing(2)}; //
            	      int i1a=g.gridIndexRange(0,0), i2a=g.gridIndexRange(0,1), i3a=g.gridIndexRange(0,2);
            	      if( numberOfDimensions==2 )
            	      {
            		FOR_3D(i1,i2,i3,I1,I2,I3)
            		{
              		  RB(i1,i2,i3,0)=(i1-i1a)*dr[0];
              		  RB(i1,i2,i3,1)=(i2-i2a)*dr[1];
            		}
            	      }
            	      else
            	      {
            		FOR_3D(i1,i2,i3,I1,I2,I3)
            		{
              		  RB(i1,i2,i3,0)=(i1-i1a)*dr[0];
              		  RB(i1,i2,i3,1)=(i2-i2a)*dr[1];
              		  RB(i1,i2,i3,2)=(i3-i3a)*dr[2];
            		}
            	      }
                            #ifdef USE_PPP
              	        map.mapGridS(rb,xBoundary(side,axis,grid));
                            #else
              	        map.mapGrid(rb,xBoundary(side,axis,grid));
                            #endif
	      // x.reshape(I1,I2,I3,Rx);

            	      if( debug & 2 )
            	      {
              	        RealArray & xb = xBoundary(side,axis,grid);
            		fprintf(plogFile,"After mapGridS for boundary points grid=%i (side,axis)=(%i,%i):\n",grid,side,axis);
            		FOR_3D(i1,i2,i3,I1,I2,I3)
            		{
              		  fprintf(plogFile," (i1,i2,i3)=(%i,%i,%i) rb=(%8.2e,%8.2e) xb=(%8.2e,%8.2e)\n",i1,i2,i3,
                    			  RB(i1,i2,i3,0),RB(i1,i2,i3,1),xb(i1,i2,i3,0),xb(i1,i2,i3,1));
            		}
            	      }
#undef RB
          	    }
          	    
        	  }
      	}
            }
        }
        
    }
    
//   #ifdef USE_PPP
//     MPI_Barrier(Overture::OV_COMM);  // Add this for testing
//   #endif

    int numberOfWarningMessages=0;
    int numberOfHoleWidthWarnings=0;
    int numberOfNonInvertibleWarnings=0;
    
  // **********************************************************************
  //     cut holes with highest priority grids first since these will 
  //     provide the first interpolation points
  // **********************************************************************
    for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
    {
        MappedGrid & g = cg[grid];
        Mapping & map = g.mapping().getMapping();
    // const realArray & vertex = g.vertex();
        const realArray & xr = g.vertexDerivative();  // xr is used for non-invertible points -- just eval as needed?
        const intArray & maskd = g.mask();
        const bool isRectangular = g.isRectangular();
        
    // NOTE: mask used for if( mask(i1,i2,i3) & ISnonCuttingBoundaryPoint ) // could be a mixed-boundary pt
        GET_LOCAL_CONST(int,maskd,mask);
        #ifdef USE_PPP
            realSerialArray vertex; if( !isRectangular ) getLocalArrayWithGhostBoundaries(g.vertex(),vertex);
        #else
            const realSerialArray & vertex = g.vertex();
        #endif

    // shift this offset by epsilon to make sure we check the correct points in the k1,k2,k3 loop.
        const real cellCenterOffset= g.isAllCellCentered() ? .5-.5 : -.5;   // add -.5 to round to nearest point

        real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
        int iv0[3]={0,0,0}; //
        if( isRectangular )
        {
            g.getRectangularGridParameters( dvx, xab );
            for( int dir=0; dir<g.numberOfDimensions(); dir++ )
      	iv0[dir]=g.gridIndexRange(0,dir);
        }
        #define XV(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))


        if( debug & 1 || info & 4 )
        {
            printf("cutting holes with grid: %s ...\n",(const char*)g.getName());
            fprintf(plogFile,"\n ===========================================================================\n"
                                              "   ************* cutting holes with grid: %s ...\n\n",(const char*)g.getName());
        }
        
        for( int axis=axis1; axis<numberOfDimensions; axis++ )
        {
      // axisp1 : must equal the most rapidly varying loop index of the triple (i1,i2,i3) loop below
      //          since we only save the holeWidth for the previous line.
            const int axisp1 = axis!=axis1 ? axis1 : axis2;  // we must make this axis1 if possible, otherwise axis2
            const int axisp2 = numberOfDimensions==2 ? axisp1 : (axis!=axis2 && axisp1!=axis2) ? axis2 : axis3;
            
            for( int side=Start; side<=End; side++ )
            {
        // Note: do not cut holes with singular sides
                if( g.boundaryCondition(side,axis) > 0 && 
                        map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity ) 
      	{
	  // this side is a physical boundary
        	  getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);   // note: use gridIndexRange

                    real boundaryEpsilon=boundaryEps;
                    if( debug & 2 )
                        printf(" cg.maximumHoleCuttingDistance(%i,%i,%i)=%e\n",
                                    side,axis,grid,cg.maximumHoleCuttingDistance(side,axis,grid));
        	  
                    const real maximumHoleCuttingDistanceSquared=SQR(cg.maximumHoleCuttingDistance(side,axis,grid));
                    if( debug & 4 )
          	    printf(" (side,axis,grid)=(%i,%i,%i) maximumHoleCuttingDistance=%e\n",
               		   side,axis,grid,cg.maximumHoleCuttingDistance(side,axis,grid));

//           if( FALSE && g.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
// 	  {
// 	    // choose boundaryEps to match tolerance on c-grid since we don't want to cut holes
//             //  inside the c-grid
// 	    for( int n=0; n<numberOfMixedBoundaries; n++ )
// 	    {
// 	      if( mixedBoundary(n,0)==grid && side==mixedBoundary(n,1) && axis==mixedBoundary(n,2))
// 	      {
// 		boundaryEpsilon=mixedBoundaryValue(n,0);
//                 assert( boundaryEpsilon>=0. );
// 	      }
// 	    }
// 	  }
        	  

                    Range R1(0,I1.length()*I2.length()*I3.length()-1);

	  // no: getBoundaryIndex(extendedGridIndexRange(g),side,axis,I1,I2,I3);   // note: use gridIndexRange
                    bool firstTimeForThisBoundary=TRUE;

          // ************************************************************************************************
          // *** share : when a point on grid2 interp's from grid, we mark cutShare[grid2](i1,i2,i3)=share
          //             so we can prevent other boundaries with the same value for share from cutting
          // *** next line assume only positive share values ***
          // ************************************************************************************************
        	  const int share=g.sharedBoundaryFlag(side,axis)>0 ? g.sharedBoundaryFlag(side,axis) : 
          	    maximumSharedBoundaryFlag+grid+1 ; // a unique (bogus) value for this grid.
        	  
                    #ifdef USE_PPP
          	    const realSerialArray & normal = g.vertexBoundaryNormalArray(side,axis);
                    #else
            	    const realSerialArray & normal = g.vertexBoundaryNormal(side,axis);
                    #endif


          // **********************************
          //     Cut Holes in other grids  
          // **********************************
                    for( int grid2=0; grid2<numberOfBaseGrids; grid2++ )
        	  {
                        MappedGrid & g2 = cg[grid2];
          	    if( grid2!=grid && 
                // *wdh* 990504 cg.mayCutHoles(grid,grid2) &&   // we need to check for interp points in this case
            		(cg.mayCutHoles(grid,grid2) || cg.mayInterpolate(grid,grid2,0)) && // *wdh* 020127
                                (isNew(grid) || isNew(grid2))
                                &&  map.intersects( g2.mapping().getMapping(), side,axis,-1,-1,.1 ) )
          	    {

                            bool mayCutHoles = cg.mayCutHoles(grid,grid2);
                            const bool phantomHoleCutting=cg.mayCutHoles(grid,grid2)==2;
            	      
                            if( debug & 4 )
            		fprintf(plogFile,"\n +++++ grid %i (%s) (side,axis)=(%i,%i) try to cut holes in grid2=%i (%s)\n\n",
                   		       grid,(const char*)g.getName(),side,axis,grid2,(const char*)g2.getName());
            		
              // printf(" (side,axis,grid)=(%i,%i,%i) mayCutHoles=%i grid2=%i sharedSidesMayCutHoles=%i\n",
	      //     side,axis,grid,mayCutHoles,grid2,cg.sharedSidesMayCutHoles(grid,grid2));
            	      
                            if( mayCutHoles && !cg.sharedSidesMayCutHoles(grid,grid2) && !phantomHoleCutting )
            	      {
		// shared sides should not cut holes
                                const int shareFlag=g.sharedBoundaryFlag(side,axis);
                // printf(" (side,axis,grid)=(%i,%i,%i) share=%i grid2=%i\n",
		//       side,axis,grid,shareFlag,grid2);
            		
            		if( shareFlag!=0 && min(abs(g2.sharedBoundaryFlag()-shareFlag))==0 )
            		{
		  // grid2 has the same share flag as this face of grid.
                                    mayCutHoles=FALSE;
                                    if( debug & 2 )
                    		    fprintf(plogFile,"*** hole cutting prevented for (side,axis,grid)=(%i,%i,%i) on grid2=%i since"
                                                      " grids share a boundary.\n",side,axis,grid,grid2);
            		}
            	      }
            	      
                            if( firstTimeForThisBoundary )
            	      {
            		firstTimeForThisBoundary=FALSE;
            		r.redim(I1,I2,I3,Rx);
            		rr.redim(I1.length()*I2.length()*I3.length(),Rx); rr=-1.;
            		x.redim(I1.length()*I2.length()*I3.length(),Rx);
                                ia.redim(I1.length()*I2.length()*I3.length(),7);  
            	      }

            	      real *rp = r.Array_Descriptor.Array_View_Pointer3;
            	      const int rDim0=r.getRawDataSize(0);
            	      const int rDim1=r.getRawDataSize(1);
            	      const int rDim2=r.getRawDataSize(2);
                            #define R(i0,i1,i2,i3) rp[i0+rDim0*(i1+rDim1*(i2+rDim2*(i3)))]	

                            #undef RR
            	      real * rrp = rr.Array_Descriptor.Array_View_Pointer1;
            	      const int rrDim0=rr.getRawDataSize(0);
                            #define RR(i0,i1) rrp[i0+rrDim0*(i1)]

                            #undef X
            	      real * xp = x.Array_Descriptor.Array_View_Pointer1;
            	      const int xDim0=x.getRawDataSize(0);
                            #define X(i0,i1) xp[i0+xDim0*(i1)]

                            #undef IA
            	      int * iap = ia.Array_Descriptor.Array_View_Pointer1;
            	      const int iaDim0=ia.getRawDataSize(0);
                            #define IA(i0,i1) iap[i0+iaDim0*(i1)]

                            const bool isRectangular2 = g2.isRectangular();
            	      real dvx2[3]={1.,1.,1.}, xab2[2][3]={{0.,0.,0.},{0.,0.,0.}};
                            int iv20[3]={0,0,0}; //
            	      if( isRectangular2 )
            	      {
            		g2.getRectangularGridParameters( dvx2, xab2 );
                                for( int dir=0; dir<numberOfDimensions; dir++ )
            		{
              		  iv20[dir]=g2.gridIndexRange(0,dir);
                                    if( g2.isAllCellCentered() )
              		  {
                                        xab2[0][dir]+=.5*dvx2[dir];  // offset for cell centered
              		  }
            		}
            		
            	      }
                            #undef XC2
                            #define XC2(iv,axis) (xab2[0][axis]+dvx2[axis]*(iv[axis]-iv20[axis]))

              // const IntegerArray & indexRange2 = g2.indexRange();
              // const IntegerArray & extendedIndexRange2 = g2.extendedIndexRange();

                            const int *pIndexRange2=g2.indexRange().Array_Descriptor.Array_View_Pointer1;
                            const int *pExtendedIndexRange2=g2.extendedIndexRange().Array_Descriptor.Array_View_Pointer1;
                            #define indexRange2(side,axis) pIndexRange2[(side)+2*(axis)]
                            #define extendedIndexRange2(side,axis) pExtendedIndexRange2[(side)+2*(axis)]

            	      const real *pGridSpacing2 = g2.gridSpacing().Array_Descriptor.Array_View_Pointer0;
                            #define gridSpacing2(axis) pGridSpacing2[axis]

                            intArray & mask2d = g2.mask();
                            intArray & inverseGridd = cg.inverseGrid[grid2];
            	      realArray & rId = cg.inverseCoordinates[grid2];

                            GET_LOCAL(int,mask2d,mask2);
                            GET_LOCAL(int,inverseGridd,inverseGrid);
                            GET_LOCAL(real,rId,rI);
                            #ifdef USE_PPP
                                realSerialArray center2; if( !isRectangular2 ) getLocalArrayWithGhostBoundaries(g2.center(),center2);
                            #else
                                const realSerialArray & center2 = g2.center();
                            #endif

                            int plocalIndexBounds2[6];
            	      #define localIndexBounds2(side,axis) plocalIndexBounds2[(side)+2*(axis)]
            	      for( dir=0; dir<3; dir++ )
            	      {
            		localIndexBounds2(0,dir)=max(extendedIndexRange2(0,dir),mask2.getBase(dir));
            		localIndexBounds2(1,dir)=min(extendedIndexRange2(1,dir),mask2.getBound(dir));
            	      }

              // isPeriodic2[dir]  : function periodic for grid2 
              // isPeriodic2p[dir] : function periodic for grid2 AND the parallel direction is NOT distributed.
              //                   : If the parallel direction is split across processors then we cannot
              //                     perform a periodic wrap of the points. 
                            bool isPeriodic2[3]={false,false,false};       
                            bool isPeriodic2p[3]={false,false,false};       

                            bool isDerivativePeriodic2[3]={false,false,false};
                            real derivativePeriod2[3]={0.,0.,0.};
                    

                            for( dir=0; dir<numberOfDimensions; dir++ )
            	      {
              // isPeriodic2[dir]=g2.isPeriodic(dir)==Mapping::functionPeriodic;
                                isPeriodic2[dir]=g2.isPeriodic(dir)==Mapping::functionPeriodic ||
                                                                  g2.isPeriodic(dir)==Mapping::derivativePeriodic;  // *wdh* Nov 14, 2017
                                isPeriodic2p[dir]= (isPeriodic2[dir] && 
                           				   localIndexBounds2(0,dir)==extendedIndexRange2(0,dir) &&
                           				   localIndexBounds2(1,dir)==extendedIndexRange2(1,dir) );

                                isDerivativePeriodic2[dir]=g2.isPeriodic(dir)==Mapping::derivativePeriodic;
                // here is the length of the periodic direction (e.g. for periodic box)
                                if( isDerivativePeriodic2[dir] )
                                {
                                    derivativePeriod2[dir]=g2.mapping().getMapping().getPeriodVector(dir,dir);
                  // printF(" grid2=%i dir=%i isDerivativePeriodic: derivativePeriod2[dir]=%g\n",
                  //       grid2,dir,derivativePeriod2[dir]);

                                    assert( derivativePeriod2[dir]>0. );
                                }
                                
                                
            	      }
                            printF(" grid2=%i: isPeriodic2=[%i,%i,%i] isPeriodic2p=[%i,%i,%i]\n",grid2,
                                isPeriodic2[0],isPeriodic2[1],isPeriodic2[2],
                                isPeriodic2p[0],isPeriodic2p[1],isPeriodic2p[2]);
                            
            	      

            	      intSerialArray & cutShare2 = cutShare[grid2];
                            intSerialArray cut;
            	      cut=cutShare2;   // ** make a copy ** why?
            	      
            	      
              // ***************************************************************************
              //   Make a list of points on grid in the bounding box of grid2
              // no need to cut holes with points that already interpolate from this grid!
              // ***************************************************************************

                            RealArray & xb = xBoundary(side,axis,grid);

                            RealArray boundingBox;
                            boundingBox=g2.mapping().getMapping().getBoundingBox();    //   *** note: ghost lines not included
                            real delta = .2*max( boundingBox(End,Rx)-boundingBox(Start,Rx) );
                            for( dir=0; dir<numberOfDimensions; dir++ )
            	      {
            		boundingBox(Start,dir)-=delta;
            		boundingBox(End  ,dir)+=delta;
            	      }

                            intSerialArray cutMask(I1,I2,I3);
              // cutMask(i1,i2,i3)==1 : if point (i1,i2,i3) is inside the bounding box of grid2
                            if( !isRectangular )
            	      {
            		if( numberOfDimensions==2 )
              		  cutMask=(xb(I1,I2,I3,axis1)>boundingBox(0,axis1) && xb(I1,I2,I3,axis1)<boundingBox(1,axis1)&&
                     			   xb(I1,I2,I3,axis2)>boundingBox(0,axis2) && xb(I1,I2,I3,axis2)<boundingBox(1,axis2));
            		else
              		  cutMask=(xb(I1,I2,I3,axis1)>boundingBox(0,axis1) && xb(I1,I2,I3,axis1)<boundingBox(1,axis1)&&
                     			   xb(I1,I2,I3,axis2)>boundingBox(0,axis2) && xb(I1,I2,I3,axis2)<boundingBox(1,axis2)&&
                     			   xb(I1,I2,I3,axis3)>boundingBox(0,axis3) && xb(I1,I2,I3,axis3)<boundingBox(1,axis3));
            	      }
            	      else
            	      {
            		if( numberOfDimensions==2 )
            		{
                                    FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                		    cutMask(i1,i2,i3)=(XV(iv,axis1)>boundingBox(0,axis1) && XV(iv,axis1)<boundingBox(1,axis1)&&
                                 			               XV(iv,axis2)>boundingBox(0,axis2) && XV(iv,axis2)<boundingBox(1,axis2));
              		  }
            		}
            		else
            		{
                                    FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                		    cutMask(i1,i2,i3)=(XV(iv,axis1)>boundingBox(0,axis1) && XV(iv,axis1)<boundingBox(1,axis1)&&
                               				       XV(iv,axis2)>boundingBox(0,axis2) && XV(iv,axis2)<boundingBox(1,axis2)&&
                               				       XV(iv,axis3)>boundingBox(0,axis3) && XV(iv,axis3)<boundingBox(1,axis3));
              		  }
            		}
            	      }
            	      

            	      if( map.getTopology(side,axis)==Mapping::topologyIsPartiallyPeriodic )
            	      {
                                fprintf(plogFile,"grid=%s (side,axis)=(%i,%i) is a c-grid side\n",(const char*)g.getName(),side,axis);
            		
                                intArray & topologyMaskd=map.topologyMask();
            		GET_LOCAL(int,topologyMaskd,topologyMask);
            		
            		cutMask=cutMask && topologyMask(I1,I2,I3)==0;
            	      }
//               if( FALSE && numberOfMixedBoundaries>0  ) // fix this ***************
// 	      {
//                 // do not cut holes with interpolation parts of mixed boundaries
// 		// **wdh*  990927 cutMask=cutMask && !(mask(I1,I2,I3) & MappedGrid::ISinteriorBoundaryPoint);
// 		cutMask=cutMask && !(mask(I1,I2,I3) & ISnonCuttingBoundaryPoint);
// 	      }
            	      
                            int i=0;
                            int I1Base=I1.getBase(), I1Bound=I1.getBound();
                            int I2Base=I2.getBase(), I2Bound=I2.getBound();
                            int I3Base=I3.getBase(), I3Bound=I3.getBound();
            	      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
            	      {
            		for( i2=I2.getBase(); i2<=I2Bound; i2++ )
            		{
              		  for( i1=I1Base; i1<=I1Bound; i1++ )
              		  {
                		    if( cutMask(i1,i2,i3) )
                		    {
		      // ***** ia(i,.) : list of potential cutting points ********
		      // for( dir=0; dir<numberOfDimensions; dir++ )
		      //  x(i,dir)=vertex(i1,i2,i3,dir);
                  		      ia(i,0)=i1;
                  		      ia(i,1)=i2;
                  		      ia(i,2)=i3;
                  		      i++;
                		    }
              		  }
            		}
            	      }
            	      
                            #ifndef USE_PPP
                            if( i==0 )
                                continue;
                            #endif
            	      int numberToCheck=i;
            	      
                            R1=numberToCheck; // this is ok if numberToCheck==0

                            const int r1Bound=R1.getBound();
                            if( !isRectangular )
            	      {
            		for( dir=0; dir<numberOfDimensions; dir++ )
                                    for( int i=R1.getBase(); i<=r1Bound; i++ )
                		    x(i,dir)=xb(ia(i,0),ia(i,1),ia(i,2),dir);
            	      }
            	      else
            	      {
            		for( int i=R1.getBase(); i<=r1Bound; i++ )
            		{
              		  iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
              		  for( dir=0; dir<numberOfDimensions; dir++ )
                		    x(i,dir)=XV(iv,dir);
            		}
            	      }

            	      if( debug & 2 )
            	      {
            		fprintf(plogFile,"BEFORE adjustBoundary and inverseMap for boundary cutting points:\n");
            		for( int i=R1.getBase(); i<=r1Bound; i++ )
            		{
              		  fprintf(plogFile," i=%i x=(%8.2e,%8.2e)\n",i,x(i,0),x(i,1));
            		}
            	      }
            	      
                            real time1=getCPU();
              // adjust boundary points on shared sides *** x is changed **
            	      if( useBoundaryAdjustment )
            	      {
                                if( numberToCheck>0 )
                  		  adjustBoundary(cg,grid,grid2,ia(R1,Rx),x(R1,Rx));   
                                else
                                    adjustBoundary(cg,grid,grid2,Overture::nullIntArray(),Overture::nullRealArray()); 
            	      }
            	      
            	      if( debug & 2 )
            	      {
            		fprintf(plogFile,"AFTER adjustBoundary and BEFORE inverseMap for boundary cutting points:\n");
            		for( int i=R1.getBase(); i<=r1Bound; i++ )
            		{
              		  fprintf(plogFile," i=%i x=(%8.2e,%8.2e)\n",i,x(i,0),x(i,1));
            		}
            	      }

                            #ifdef USE_PPP
                                if( numberToCheck>0 )
                                    g2.mapping().getMapping().inverseMapS(x(R1,Rx),rr);
                                else
                                    g2.mapping().getMapping().inverseMapS(Overture::nullRealArray(),Overture::nullRealArray());
                            #else
                                g2.mapping().getMapping().inverseMap(x(R1,Rx),rr);
                            #endif

            	      if( numberToCheck>0 )
            	      {
                                r=Mapping::bogus;
            		for( dir=0; dir<numberOfDimensions; dir++ )
              		  r(ia(R1,0),ia(R1,1),ia(R1,2),dir)=rr(R1,dir);
            	      }
            		
                            real time2=getCPU();

                            if( debug & 1 || info & 4 ) 
            	      {
            		fprintf(plogFile,"grid=%s: cut with (side,axis)=(%i,%i) :\n time for inverseMap grid2=%s, "
                  			"is %e (total=%e) (number of pts=%i)\n",
                  			(const char*)g.mapping().getName(Mapping::mappingName),
                  			side,axis,(const char*)g2.mapping().getName(Mapping::mappingName),time2-time1,time2-totalTime,
                  			numberToCheck);
            		if( debug & 2 )
            		{
                                    fprintf(plogFile,"After inverseMap for boundary cutting points:\n");
              		  for( int i=R1.getBase(); i<=r1Bound; i++ )
              		  {
                		    fprintf(plogFile," i=%i ia=(%i,%i) x=(%8.2e,%8.2e) r=(%8.2e,%8.2e)\n",i,
                                                  ia(i,0),ia(i,1),x(i,0),x(i,1),rr(i,0),rr(i,1));
              		  }
            		}
            	      }
            	      
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
            	      
              // ********************************************************************************************
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
              // ********************************************************************************************


                            intSerialArray holeMask;
            	      if( numberToCheck>0 )
            	      {
            		holeMask.redim(I1,I2,I3);
            		holeMask=0;
            	      }
                            real dr=0.;
              // loop over all points on the face that is cutting a hole
                            for( i=0; i<numberToCheck; i++ )
            	      {
            		i1=ia(i,0);
            		i2=ia(i,1);
            		i3=ia(i,2);
            		
            		i1p=i1<I1Bound ? i1+1 : i1>I1Base ? i1-1 : i1;
            		i2p=i2<I2Bound ? i2+1 : i2>I2Base ? i2-1 : i2;
            		i3p=i3<I3Bound ? i3+1 : i3>I3Base ? i3-1 : i3;

		// ** int ib = iv[axisp1];  // tangential marching direction
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
                                dr=0.;
            		if( numberOfDimensions==2 )
            		{
              		  if( fabs(rr(i,axisp1)-.5)<.6 && fabs(r(i1p,i2p,i3p,axisp1)-.5)<.6 ) // watch out for periodic bndry's
              		  {
                		    dr=fabs(r(i1p,i2p,i3p,axisp1)-rr(i,axisp1));
                                        dr=min(.2,1.-dr);  // this should handle the case when we cross a periodic boundary
              		  }
            		}
            		else
            		{
              		  if( fabs(rr(i,axisp1)-.5)<.6 && fabs(r(i1p,i2p,i3p,axisp1)-.5)<.6  )
              		  {
                		    dr=max(fabs(r(i1p,i2p,i3p,axisp1)-rr(i,axisp1)),fabs(r(i1p,i2p,i3p,axisp2)-rr(i,axisp2)));
                                        dr=min(.2,1.-dr);  // this should handle the case when we cross a periodic boundary
              		  }
              		  
            		}
                //  if( dr>.5 )
		//  {
		//    printf("WARNING: i=%i, rr=(%e,%e,%e), rp=(%e,%e,%e) (i1,i2,i3)=(%4i,%4i,%4i) and dr=%e\n",
		//	   i,rr(i,0),rr(i,1),rr(i,2), r(i1p,i2p,i3p,0),r(i1p,i2p,i3p,1),r(i1p,i2p,i3p,2), i1,i2,i3,dr);
		//  }
                		    
            		if( rr(i,axis1)>rBound(Start,axis1,grid2)-dr && rr(i,axis1)<rBound(End,axis1,grid2)+dr &&
                		    rr(i,axis2)>rBound(Start,axis2,grid2)-dr && rr(i,axis2)<rBound(End,axis2,grid2)+dr &&
                		    ( numberOfDimensions<3 || 
                  		      (rr(i,axis3)>rBound(Start,axis3,grid2)-dr && rr(i,axis3)<rBound(End,axis3,grid2)+dr )
                  		      )
              		  )
            		{

		  // check for shared sides **** why is this needed ??
                  // *wdh* 990927 if( mask(i1,i2,i3) & MappedGrid::ISinteriorBoundaryPoint )

                  // **** fix this -- local mask is not defined everywhere!
                                    #ifndef USE_PPP
                                      if( mask(i1,i2,i3) & ISnonCuttingBoundaryPoint ) // could be a mixed-boundary pt
                  	 	     holeMask(i1,i2,i3)=2;
                	 	   else
                 		     holeMask(i1,i2,i3)=1;  // point is inside.
                                    #else
               		   if( numberOfWarningMessages<1 )
               		   {
                 		     numberOfWarningMessages++; printF("Ogen::cutHoles: fix me for parallel and mixed-boundaries\n");
               		   }
                                        holeMask(i1,i2,i3)=1;  // point is inside.
                                    #endif

            		}
		// else if( max(fabs(r(i1,i2,i3,Rx)))<3. )
            		else if( rr(i,0)!=Mapping::bogus )
            		{
              		  holeMask(i1,i2,i3)=2;  // mark this point as "not inside" but invertible
            		}

            	      } // end for i=0,..,numberToCheck
            	      

              // for any point on grid that can be interpolated, find points nearby on grid2 that are outside the
              // boundary and mark them as unused.

            	      if( numberToCheck>0 )
            	      {

            		if( info & 4 )
              		  display(holeMask,"holeMask (on cutting face ) 1=inside of g2, 0=outside, 2=out (but invertible)",
                    			  plogFile,"%2i");
            		

		// loop over all points on the face that is cutting a hole
            		FOR_3(i1,i2,i3,I1,I2,I3)
            		{
              		  int ib = iv[axisp1];  // tangential marching direction
              		  int ib2 = numberOfDimensions>2 ? iv[axisp2] : 0;  // second tangential marching direction (3D)
              		  if( holeMask(i1,i2,i3)==1 )
              		  {
                		    if( info & 4 ) 
                  		      fprintf(plogFile,"------ cutHoles: process point ib=%i(ib2=%i) (%i,%i,%i) on (side,axis)=(%i,%i) holeMask==1 "
                        			      " r=(%6.2e,%6.2e,%6.2e)\n",ib,ib2,i1,i2,i3,side,axis,r(i1,i2,i3,0),r(i1,i2,i3,1),
                        			      numberOfDimensions==2 ? 0. : r(i1,i2,i3,2));
              		  
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
                  		      jv[dir]=(int)floor( r(i1,i2,i3,dir)/gridSpacing2(dir)+indexRange2(Start,dir)-cellCenterOffset );
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
              		  
                		    int skipThisPoint=0;
                		    int initialPoint=0;

                		    getHoleWidth( cg,g2, 
                          				  pHoleMarker, 
                          				  holeCenter, 
                          				  holeMask, 
                          				  holeWidth, 
                          				  r,
                          				  x,
                          				  pIndexRange2,
                          				  pExtendedIndexRange2,
                                                                    plocalIndexBounds2,
                          				  iv,jv,jpv,
                          				  isPeriodic2,isPeriodic2p, 
                          				  Iv, 
                          				  grid, grid2,
                          				  ib, ib2,
                          				  skipThisPoint,
                          				  initialPoint,
                          				  numberOfDimensions,
                          				  axisp1, axisp2, cellCenterOffset,
                          				  maximumHoleWidth, numberOfHoleWidthWarnings );
  

                		    for( dir=0; dir<numberOfDimensions; dir++ )
                  		      holeCenter(dir,ib)=holeMarker(dir);
                		    if( skipThisPoint>=numberOfDimensions-1 )
                  		      continue;
                		    if( info & 4 ) 
                  		      fprintf(plogFile,"  *** : grid2=%i, point ib=%i(%i), r=(%6.2e,%6.2e,%6.2e), holeCenter=(%i,%i,%i), "
                        			      "width=(%i,%i,%i)\n",grid2,ib,ib2,
                        			      r(i1,i2,i3,0),r(i1,i2,i3,1),numberOfDimensions==2 ? 0. : r(i1,i2,i3,2),
                        			      holeCenter(0,ib),holeCenter(1,ib),numberOfDimensions==2 ? 0 : holeCenter(2,ib),
                        			      holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 0 : holeWidth(2,ib));
                		    for( dir=0; dir<numberOfDimensions; dir++ )
                		    {
                                        
		      // if( g2.isPeriodic(dir)!=Mapping::functionPeriodic ) // ************ 060325 *
                  		      if( !isPeriodic2p[dir] ) 
                  		      {
                  			jpv[dir]=min(jv[dir]+holeWidth(dir,ib),localIndexBounds2(End,dir));
                  			jv[dir] =max(jv[dir]-holeWidth(dir,ib),localIndexBounds2(Start,dir));
                  		      }
                  		      else
                  		      {
                  			jpv[dir]=jv[dir]+holeWidth(dir,ib);             // ** fix for parallel **
                  			jv[dir] =jv[dir]-holeWidth(dir,ib); 
                  		      }
                		    
                		    }
		    // make a list of boundary points and their inverseMap images
		    // first make sure there is enough space:
                		    int numberOfNewPoints=(jpv[0]-j1+1)*(jpv[1]-j2+1)*(jpv[2]-j3+1);  
                		    if( ia.getLength(0)<=numberCut+numberOfNewPoints )
                  		      ia.resize((ia.getLength(0)+numberOfNewPoints)*2,7);


		    // ***********************************************************
		    // *** Fill-in the ia array with potential points to cut *****
		    // ***********************************************************
		    // At this stage we include points that are both inside and outside the grid
                		    int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];
                		    for( int k3a=j3; k3a<=jpv[2]; k3a++ )
                  		      for( int k2a=j2; k2a<=jpv[1]; k2a++ )
                  			for( int k1a=j1; k1a<=jpv[0]; k1a++ )
                  			{
                                        
			  // ************ 060325 ***************
                    			  k1=k1a; k2=k2a; k3=k3a;
                    			  for( dir=0; dir<numberOfDimensions; dir++ )
                    			  {
			    // **** fix this for parallel ****
                      			    if( isPeriodic2p[dir] )
                      			    {
                        			      if( kv[dir]<extendedIndexRange2(Start,dir) )
                        			      {
                        				kv[dir]+=indexRange2(1,dir)-indexRange2(0,dir)+1;
                        			      }
                        			      else if( kv[dir]>extendedIndexRange2(End,dir) )
                        			      {
                        				kv[dir]-=indexRange2(1,dir)-indexRange2(0,dir)+1;
                        			      }
                      			    }
                    			  }
                		    

                    			  if(
			    // *0 mask2(k1,k2,k3)!=0 &&           // this point already cut
                      			    cut(k1,k2,k3) >=0 &&  // this point not in the list yet
                      			    (mask2(k1,k2,k3)!=0 || cut(k1,k2,k3)==share) && 
                      			    inverseGrid(k1,k2,k3)!=grid &&  // *wdh* added 990426
			    !( mask2(k1,k2,k3) & ISnonCuttingBoundaryPoint) )  // is this correct?
			    // *wdh*  990927 !( mask2(k1,k2,k3) & MappedGrid::ISinteriorBoundaryPoint) )  
                    			  {
                      			    ia(numberCut,0)=k1;
                      			    ia(numberCut,1)=k2;
                      			    ia(numberCut,2)=k3;
                      			    ia(numberCut,3)=mask2(k1,k2,k3);

//                       if( true )
// 			printf("cut hole: myid=%i i=%i ia=(%i,%i,%i)\n",
// 			       myid,numberCut,k1,k2,k3);

                      			    if( debug & 4 )
                        			      fprintf(plogFile,"---- grid=%i cutting a hole on grid2=%i at (%i,%i,%i) mask=%i cut=%i, share=%i \n",
                              				      grid,grid2,k1,k2,k3,mask2(k1,k2,k3),cut(k1,k2,k3),share);


                      			    if( mayCutHoles )
                      			    {
                        			      ia(numberCut,4)=i1;  // save these values for double checking points that cannot be inverted.
                        			      ia(numberCut,5)=i2;
                        			      ia(numberCut,6)=i3;

			      // 1. do not cut a hole at a pt that could already interp from inside a 
			      //    grid with the same share value:
			      // 2. Do not cut holes with a phantom hole cutter.
                        			      if( cut(k1,k2,k3)!=share && !phantomHoleCutting )
                        				mask2(k1,k2,k3)=0;   

			      // cutShare2(k1,k2,k3)=share;
                  			
                      			    }

                      			    cut(k1,k2,k3)=-1;
                      			    numberCut++;

                  		      
                    			  }
                  			} // end for k1a, k2a,k3a
              		  }
              		  else // holeMask(i1,i2,i3)!=1 : 
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
            	      } // end if( numberToCheck > 0 )
            	      

                            bool doubleCheck= numberCut > 0;
            	      #ifdef USE_PPP
              // In parallel we must check on all processors since there are calls to inverseMap and map 
              	        doubleCheck=true;  
                            #endif

                            if( doubleCheck )
            	      {
                // =========================================================
		// ====== now double check the points we cut out  ==========
                // =========================================================

            // 		checkPointsMarkedAsHoles();
                            R=numberCut;
                            if( numberCut>0 )
                            {
                                x2.redim(R,Rx);
                                r2.redim(R,Rx); r2=-2.;
                            }
                            const int bound=R.getBound();
                            if( !isRectangular2 )
                            {
                                if( true )
                                { // sanity check on the values in the ia array
                                    for( int i=R.getBase(); i<=bound; i++ )
                                    {
                              	i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);  // fill in iv[0..2]
                              	if( i1<center2.getBase(0) )
                              	{
                                	  printf("ERROR: myid=%i i=%i ia=(%i,%i,%i) center2=[%i,%i][%i,%i][%i,%i]\n"
                                     		 "                                    mask2=[%i,%i][%i,%i][%i,%i]\n",
                                     		 myid,i,i1,i2,i3,center2.getBase(0),center2.getBound(0),
                                     		 center2.getBase(1),center2.getBound(1),center2.getBase(2),center2.getBound(2),
                                     		 mask2.getBase(0),mask2.getBound(0),
                                     		 mask2.getBase(1),mask2.getBound(1),mask2.getBase(2),mask2.getBound(2));
                                	  Overture::abort("error");
                              	}
                                    }
                                }
                                if( numberCut>0 )
                                {
                                    for( dir=0; dir<numberOfDimensions; dir++ )
                              	x2(R,dir)=center2(ia(R,0),ia(R,1),ia(R,2),dir);
                                }
                            }
                            else
                            {
                                for( int i=R.getBase(); i<=R.getBound(); i++ )
                                {
                                    i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);  // fill in iv[0..2]
                                    for( dir=0; dir<numberOfDimensions; dir++ )
                                    {
                              	x2(i,dir)=XC2(iv,dir);
                                    }
                                }
                            }
                            if( useBoundaryAdjustment )
                            {
                                if( numberCut>0 )
                                    adjustBoundary(cg,grid2,grid,ia(R,Rx),x2);  // adjust boundary points on shared sides 
                                else
                                    adjustBoundary(cg,grid2,grid,Overture::nullIntArray(),Overture::nullRealArray());
                            }
                        #ifdef USE_PPP
                            if( numberCut>0 )
                                map.inverseMapS(x2,r2);
                            else
                                map.inverseMapS(Overture::nullRealArray(),Overture::nullRealArray());
                        #else
                            map.inverseMapC(x2,r2);
                        #endif
              // determine xB -- the closest pt on the boundary 
                            realSerialArray rB, xB;
                            if(numberCut>0 )
                            {
                                rB.redim(R,Rx); xB.redim(R,Rx);
                                rB=(real)side; // project inverse pt to the boundary
                                where( r2(R,0)!=Mapping::bogus )
                                {
                                    rB(R,axisp1)=r2(R,axisp1);
                                    if( numberOfDimensions==3 )
                              	rB(R,axisp2)=r2(R,axisp2);
                                }
                            }
                            if( debug & 4 )
                            {
                                fprintf(plogFile,"\n *** grid=%i grid2=%i side=%i axis=%i Call mapS for xB numberCut=%i***\n\n",
                                  	    grid,grid2,side,axis,numberCut);
                            }
                        #ifdef USE_PPP
                            if( numberCut>0 )
                                map.mapS(rB,xB);
                            else 
                                map.mapS(Overture::nullRealArray(),Overture::nullRealArray());
                        #else
                            map.mapC(rB,xB);
                        #endif
                            if( debug & 4 )
                            {
                                fprintf(plogFile,"\n *** grid=%i grid2=%i side=%i axis=%i AFTER mapS for xB numberCut=%i***\n\n",
                                  	    grid,grid2,side,axis,numberCut);
                                fflush(plogFile);
                            }
            //   #ifdef USE_PPP
            //     MPI_Barrier(Overture::OV_COMM);  // Add this for testing
            //   #endif
            //                 if( FALSE && debug & 4 )
            // 		{
            //                   char buff[80];
            // 		  display(ia(R,Rx),sPrintF(buff,"Here is ia on grid=%i\n",grid),plogFile," %3i");
            // 		  display(x2(R,Rx),sPrintF(buff,"Here are the x2 coordinates on grid=%i\n",grid),plogFile," %9.2e");
            // 		  display(r2(R,Rx),sPrintF(buff,"Here are the r2 coordinates on grid=%i\n",grid),plogFile," %9.2e");
            // 		}
                            if( numberCut >0  )   // the remaining code in this section has no communication
                            {
                                if( debug & 4 )
                                {
                                    for( i=R.getBase(); i<=R.getBound(); i++ )
                                    {
                              	fprintf(plogFile,"potential hole pt: grid2=%i ia=(%3i,%3i,%3i), r2=(%8.1e,%8.1e,%8.1e) mask=%i\n",
                                    		grid2,ia(i,0),ia(i,1),ia(i,2),
                                    		r2(i,0),r2(i,1),(numberOfDimensions==2 ? 0. : r2(i,2)),mask2(ia(i,0),ia(i,1),ia(i,2)));
                                    }
                                    for( i=R.getBase(); i<=R.getBound(); i++ )
                                    {
                              	fprintf(plogFile,"cutHoles: invert grid=%i, i=%i ia=(%i,%i,%i), x2=(%9.2e,%9.2e,%9.2e)"
                                    		" r2=(%5.2e,%5.2e,%5.2e) axis=%i\n",grid,i,
                                    		ia(i,0),ia(i,1),ia(i,2),
                                    		x2(i,0),x2(i,1),(numberOfDimensions==2? 0. : x2(i,2)),
                                    		r2(i,0),r2(i,1),(numberOfDimensions==2? 0. : r2(i,2)),axis);
                                    }
                                    for( i=R.getBase(); i<=R.getBound(); i++ )
                                    {
                              	if(  r2(i,axis1)>=rBound(Start,axis1,grid) && r2(i,axis1)<=rBound(End,axis1,grid) &&
                                   	     r2(i,axis2)>=rBound(Start,axis2,grid) && r2(i,axis2)<=rBound(End,axis2,grid) &&
                                   	     ( numberOfDimensions==2 || 
                                     	       (r2(i,axis3)>=rBound(Start,axis3,grid) && r2(i,axis3)<=rBound(End,axis3,grid)) ) )
                              	{
                                	  fprintf(plogFile,"cutHoles: un-cutting a hole: grid2=%i (%s), ia=(%i,%i,%i), "
                                      		  "r=(%7.1e,%7.1e,%7.1e) axis=%i **can interpolate\n",grid2,
                                      		  (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
                                      		  r2(i,0),r2(i,1),(numberOfDimensions==2 ? 0. : r2(i,2)),axis);
                              	}
                              	else if(  r2(i,axis)!=Mapping::bogus &&       // point was invertible
                                      		  ( fabs(r2(i,axis  )-.5) <= .5+boundaryEpsilon
                                        		    || (side==0 && r2(i,axis  )>=0. ) ||(side==1 && r2(i,axis  )<=1. )
                                        		    || r2(i,axisp1)<=rBound(Start,axisp1,grid) || r2(i,axisp1)>=rBound(End,axisp1,grid)
                                        		    || r2(i,axisp2)<=rBound(Start,axisp2,grid) || r2(i,axisp2)>=rBound(End,axisp2,grid) ) )
                              	{
                                	  fprintf(plogFile,"cutHoles: un-cutting a hole: grid2=%i (%s), ia=(%i,%i,%i), "
                                      		  "r=(%7.1e,%7.1e,%7.1e) grid=%i side=%i axis=%i **reset mask to %i\n",grid2,
                                      		  (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
                                      		  r2(i,0),r2(i,1),(numberOfDimensions==2 ? 0. : r2(i,2)),grid,side,axis,ia(i,3));
                              	}
                                    }
                                } // end if( debug )
                // *****************************************
                // ****** Mark interpolation points ********
                // *****************************************
                //################### optimise these scalar indexing loops.############
                                const int bound=R.getBound();
                                if( numberOfDimensions==2 )
                                {
                                    for( int i=0; i<=bound; i++ )
                                    {
                              	i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);
            	// r2(i,dir) : position of the potential hole point on the cutter grid.
                              	if( r2(i,axis1)>=rBound(Start,axis1,grid) && r2(i,axis1)<=rBound(End,axis1,grid) &&
                                  	    r2(i,axis2)>=rBound(Start,axis2,grid) && r2(i,axis2)<=rBound(End,axis2,grid) )
                              	{ 
            	  // we can interpolate: the potential hole point on grid2 is actually inside grid
            	  // *wdh* 011027 only change interp if this is a higher priority
                                            mask2(i1,i2,i3) = ia(i,3); // *wdh* 2012/04/19  RESET the mask
                                	  if( inverseGrid(i1,i2,i3)<grid || mask2(i1,i2,i3)!=MappedGrid::ISinterpolationPoint) 
                                	  {
                                  	    mask2(i1,i2,i3)=MappedGrid::ISinterpolationPoint; 
                                  	    inverseGrid(i1,i2,i3)= grid;  
                                  	    for( dir=0; dir<numberOfDimensions; dir++ )
                                    	      rI(i1,i2,i3,dir)=r2(i,dir);
                                                cutShare2(i1,i2,i3)=share;  // *wdh* 2012/03/17
                                	  }
                              	}
                              	else if( r2(i,axis)!=Mapping::bogus &&       // point was invertible
                                     		 ( fabs(r2(i,axis  )-.5) <= .5+boundaryEpsilon
                                       		   || (side==0 && r2(i,axis  )>=0. ) ||(side==1 && r2(i,axis  )<=1. )
                                       		   || r2(i,axisp1)<=rBound(Start,axisp1,grid) || r2(i,axisp1)>=rBound(End,axisp1,grid)) )
                              	{
            	  // the potential hole point is not inside the cutter grid but it is
            	  //     1) almost inside in the normal direction 
            	  // OR  2) outside the opposite boundary in the normal direction
            	  // OR  3) outside in the tangential directions of the cutter grid.
            	  // (The case of a non-invertible point is treated later)
            	  // *** we should mark this point as UNKNOWN status so we check it later *****
                                	  mask2(i1,i2,i3)=ia(i,3); // reset these values
                              	}
                                    }
                                }
                                else  // 3d
                                {
                                    for( int i=0; i<=bound; i++ )
                                    {
                              	i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);
                              	if( r2(i,axis1)>=rBound(Start,axis1,grid) && r2(i,axis1)<=rBound(End,axis1,grid) &&
                                  	    r2(i,axis2)>=rBound(Start,axis2,grid) && r2(i,axis2)<=rBound(End,axis2,grid) &&
                                  	    r2(i,axis3)>=rBound(Start,axis3,grid) && r2(i,axis3)<=rBound(End,axis3,grid) )
                              	{
            	  // *wdh* 011027 only change interp if this is a higher priority
                                            mask2(i1,i2,i3) = ia(i,3); // *wdh* 2012/04/19  RESET the mask
                                	  if( inverseGrid(i1,i2,i3)<grid || mask2(i1,i2,i3)!=MappedGrid::ISinterpolationPoint) 
                                	  {
                                  	    mask2(i1,i2,i3)=MappedGrid::ISinterpolationPoint; 
                                  	    inverseGrid(i1,i2,i3)= grid;  
                                  	    for( dir=0; dir<numberOfDimensions; dir++ )
                                    	      rI(i1,i2,i3,dir)=r2(i,dir);
                                	  }
                                            cutShare2(i1,i2,i3)=share;  // *wdh* 2012/03/17
                              	}
                              	else if( r2(i,axis)!=Mapping::bogus &&         // point was invertible
                                     		 (fabs(r2(i,axis  )-.5) <= .5+boundaryEpsilon 
                                      		  || (side==0 && r2(i,axis  )>=0. ) ||(side==1 && r2(i,axis  )<=1. )
                                      		  || r2(i,axisp1)<=rBound(Start,axisp1,grid) || r2(i,axisp1)>=rBound(End,axisp1,grid)
                                      		  || r2(i,axisp2)<=rBound(Start,axisp2,grid) || r2(i,axisp2)>=rBound(End,axisp2,grid)) )
                              	{
                                	  mask2(i1,i2,i3)=ia(i,3); // MappedGrid::ISdiscretizationPoint; // reset these values
                              	}
                                    }
                                }
                                if( !mayCutHoles )
                                    numberCut=0;
                                if( g.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
                                {
                  //  mixedBoundary -- don't cut holes where there is a non-cutting portion of the boundary
                                    for( int i=0; i<numberCut; i++ )
                                    {
                              	if( mask2(ia(i,0),ia(i,1),ia(i,2))==0 && r2(i,axis)!=Mapping::bogus )
                              	{
                                	  bool okToCheck=TRUE;
                                	  j3=g.gridIndexRange(Start,axis3);
                                	  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
                                	  {
                                  	    jv[dir]=int( rB(i,dir)/g.gridSpacing(dir)+g.gridIndexRange(Start,dir) );
                                  	    okToCheck=okToCheck && (jv[dir]>=g.gridIndexRange(Start,dir) && 
                                                    				    jv[dir]<=g.gridIndexRange(End  ,dir));
                                	  }
                                	  if( okToCheck && mask(j1,j2,j3) & ISnonCuttingBoundaryPoint )
                                	  {
            	    // reset this point
                                  	    if( debug & 4 )
                                    	      fprintf(plogFile,"Uncut point (%i,%i,%i) on grid %i on a mixed boundary\n",
                                          		      ia(i,0),ia(i,1),ia(i,2),grid2);
                                  	    mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3); // reset these values
                                	  }
                              	}
                                    }
                                }
                                for( int i=0; i<numberCut; i++ )
                                {
                                    if( mask2(ia(i,0),ia(i,1),ia(i,2))!=0 )
                                    {
                              	continue; // this point was not cut
                                    }
                                    else if( r2(i,axis)!=Mapping::bogus )
                                    {
            	// point was invertible
                              	if( fabs(r2(i,axis)-.5)<.5+biggerBoundaryEps &&
                                  	    ( fabs( r2(i,axisp1)-.5) > .5 || (numberOfDimensions==3 && fabs( r2(i,axisp2)-.5) > .5 ) ) )
                              	{
            	  // *** wasn't this checked above ??  *****
            	  // invertible, outside [0,1] in tangential direction and close enough in the normal
            	  // We need this since points outside [0,1] may not be inverted as accurately by Newton.
                                	  if( debug & 2 )
                                  	    fprintf(plogFile,"++cutHoles: un-cutting a hole (%i,%i,%i) on grid=%i by grid=%i. "
                                        		    "r=(%7.1e,%7.1e,%7.1e)  "
                                        		    "This pt is close to the boundary and outside [0,1] in tangent directions\n",
                                        		    ia(i,0),ia(i,1),ia(i,2),grid2,grid,r2(i,0),r2(i,1),numberOfDimensions==2 ? 0. : r2(i,2));
                                	  mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);  // reset this value
                              	}
                              	else
                              	{
            	  // estimate the distance to the boundary -- only cut points that are close to the cutting surface
            	  // dx2 = square of the length of the diagonal of the cell on grid2
                                	  real distToBndry=0.,dx2=0.;
                                	  i3p=g2.dimension(End,axis3);
            	  // *wdh* 990918 use diagonal ipv[axis]=ia(i,axis);
                                	  ipv[axis]=ia(i,axis)+1;
                                	  if( ipv[axis] > g2.dimension(End,axis) )
                                  	    ipv[axis]=ia(i,axis)-1;
                                	  ipv[axisp1]=ia(i,axisp1)+1; 
                                	  if( ipv[axisp1] > g2.dimension(End,axisp1) )
                                  	    ipv[axisp1]=ia(i,axisp1)-1;
                                	  ipv[axisp2]=ia(i,axisp2)+1; 
                                	  if( ipv[axisp2] > g2.dimension(End,axisp2) )
                                  	    ipv[axisp2]=ia(i,axisp2)-1;
                                	  int ax;		      
                                	  if( !isRectangular2 )
                                	  {
                                  	    for( ax=0; ax<numberOfDimensions; ax++ )
                                  	    {
                                                    real distx = SQR(x2(i,ax)-xB(i,ax));
                                                    if( isDerivativePeriodic2[ax] )
                                                    { // For derivative periodic problems (e.g. periodic box) 
                            // we may need to shift to the periodic image  *wdh* Nov 15, 2017
                                                        if( sqrt(distx) > .5*derivativePeriod2[ax] )
                                                        {
                              // printF("cutHoles: ax=%i isDerivativePeriodic2=%i distx=%g derivativePeriod2=%g\n",
                              //       ax,(int)isDerivativePeriodic2[ax],distx,derivativePeriod2[ax]);
                                                            distx = fabs(1.-distx);
                                                            assert( distx <= .5*derivativePeriod2[ax] );
                                                        }
                                                    }
                                    	      distToBndry+=distx;
                                    	      dx2+=SQR(center2(ia(i,0),ia(i,1),ia(i,2),ax)-center2(i1p,i2p,i3p,ax));
                                  	    }
                                	  }
                                	  else
                                	  {
                                  	    i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);  // fill in iv[0..2]
                                  	    for( ax=0; ax<numberOfDimensions; ax++ )
                                  	    {
                                    	      real distx = SQR(x2(i,ax)-xB(i,ax));
                                                    if( isDerivativePeriodic2[ax] )
                                                    { // For derivative periodic problems (e.g. periodic box) 
                            // we may need to shift to the periodic image  *wdh* Nov 15, 2017
                                                        if( sqrt(distx) > .5*derivativePeriod2[ax] )
                                                        {
                                                            distx = fabs(1.-distx);
                                                            assert( distx <= .5*derivativePeriod2[ax] );
                                                        }
                                                    }
                                    	      distToBndry+=distx;
                                    	      dx2+=SQR(XC2(iv,ax)-XC2(ipv,ax));
                                  	    }
                                	  }
                                	  if( distToBndry > maxDistFactor*dx2 || distToBndry>maximumHoleCuttingDistanceSquared )
                                	  {
                                  	    if( debug & 2 )
                                  	    {
                                    	      fprintf(plogFile,"++cutHoles: un-cutting a hole (%i,%i,%i) on grid=%i by grid=%i. "
                                          		      " r=(%7.1e,%7.1e,%7.1e) pt is too far from boundary. \n"
                                          		      " distToBndry=%8.1e dx2=%8.1e x2=(%9.2e,%9.2e,%9.2e) xB=(%9.2e,%9.2e,%9.2e)"
                                                                    " rB=(%9.2e,%9.2e,%9.2e) derivativePeriod2=(%g,%g,%g)\n",
                                          		      ia(i,0),ia(i,1),ia(i,2),grid2,grid,r2(i,0),r2(i,1),
                                          		      numberOfDimensions==2 ? 0. : r2(i,2), sqrt(distToBndry),sqrt(dx2),
                                          		      x2(i,0),x2(i,1),numberOfDimensions==2 ? 0. : x2(i,2),
                                                                    xB(i,0),xB(i,1),numberOfDimensions==2 ? 0. : xB(i,2),
                                                                    rB(i,0),rB(i,1),numberOfDimensions==2 ? 0. : rB(i,2),
                                                                    derivativePeriod2[0],derivativePeriod2[1],derivativePeriod2[2]
                                                                          );
                                    	      if( distToBndry>maximumHoleCuttingDistanceSquared )
                                    		fprintf(plogFile,"since distToBndry=%7.2e >maximumHoleCuttingDistance=%7.2e\n",
                                          			SQRT(distToBndry),SQRT(maximumHoleCuttingDistanceSquared));
                                  	    }
                                  	    mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);  // reset this value
                                	  }
                              	}
                                    }
                                    else if( r2(i,axis)==Mapping::bogus )  // NOT invertible
                                    {
            	// if the point was NOT invertible then we double check to see it is outside of the boundary
                    // checkHolePointWithBogusInverse()
                      // if the point was NOT invertible then we double check to see it is outside of the boundary
                      // Estimate the r location for the point using
                      //        r = r(boundary) + dr
                      //        dr = [ dr/dx ] * dx       
                      //        dx = vector from boundary point jv to the point on the other grid, x2
                      //
                      // We compute: re[0:2] : the estimated r location for the pt in the cutting grid=grid 
                      // 
                      // the point (j1,j2,j3) was used to cut this hole -- we need the closest bndry pt.
                                            jv[0]=ia(i,4); jv[1]=ia(i,5); jv[2]=ia(i,6);
                                            real distToBndry=REAL_MAX;
                                            int ax;
                                            for( ax=0; ax<numberOfDimensions; ax++ )
                                                xv[ax]=x2(i,ax);
                                            map.approximateGlobalInverse->binarySearchOverBoundary( xv,distToBndry,jv,side,axis ); 
                                            jpv[0]=j1; jpv[1]=j2; jpv[2]=j3;
                                            jpv[axisp1]=jv[axisp1]+1;   
                                            if( jpv[axisp1] > g.dimension(End,axisp1) )
                                                jpv[axisp1]=jv[axisp1]-1;
                      // compute the actual distance between x2 to the boundary segment
                      // jv ---> jpv
                                            real xvj[3], xvp[3];
                                            for( ax=0; ax<numberOfDimensions; ax++ )
                                            {
                                                xvj[ax]=!isRectangular ? xb(j1,j2,j3,ax)    : XV(jv,ax);
                                                xvp[ax]=!isRectangular ? xb(j1p,j2p,j3p,ax) : XV(jpv,ax);
                                            }
                                            if( numberOfDimensions==2 )
                                            {
                                                real dot = 0., norm=0.;
                                                for( ax=0; ax<numberOfDimensions; ax++ )
                                                {
                                                    dot+=(x2(i,ax)-xvj[ax])*(xvp[ax]-xvj[ax]);
                                                    norm+=SQR(xvp[ax]-xvj[ax]);
                                                }
                                                distToBndry-=dot*dot/max(REAL_MIN,norm);
                                            }
                                            real re[3], dx[3], det;
                                            for( ax=0; ax<numberOfDimensions; ax++ )
                                                dx[ax]=x2(i,ax)-xvj[ax];
                    // define XR(m,n) xr((j1),(j2),(j3),(m)+(n)*numberOfDimensions)
                                        #define XR(m,n) xra[n][m]
                      // *wdh* 080519 ***This next section is not correct : xb(i1,i2,i3,ax) only lives on the boundary
                      //  : use vertex for now -- fix for parallel ---
                                        #ifdef USE_PPP
                                            printF("cutHoles:checkHolePointWithBogusInverse:\n");
                                            printF(" grid2=%i is being cut by the boundary of grid=%i (side,axis)=(%i,%i)\n",grid2,grid,side,axis);
                                            printF(" The pt we are checking is xv=x2=(%8.2e,%8.2e,%8.2e) on grid2=%i\n",xv[0],xv[1],xv[2],grid2);
                                            printF(" The hole was cut with ia(%i,0:2)=(%i,%i,%i), xb=(%8.2e,%8.2e,%8.2e) \n",i,ia(i,4),ia(i,5),ia(i,6),
                                                              xb(jv[0],jv[1],jv[2],0),xb(jv[0],jv[1],jv[2],1),xb(jv[0],jv[1],jv[2],2));
                                            printF(" Closest pts on the boundary are jv=(%i,%i,%i), jpv=(%i,%i,%i) \n", jv[0],jv[1],jv[2], jpv[0],jpv[1],jpv[2]);
                                            printF(" distance to pt jv is distToBndry=%8.2e\n",distToBndry);
                      // just evaluate xr on the boundary from the mapping 
                                            real dr[3]={g.gridSpacing(0),g.gridSpacing(1),g.gridSpacing(2)}; //
                                            int jva[3]={g.gridIndexRange(0,0),g.gridIndexRange(0,1),g.gridIndexRange(0,2)};
                                            RealArray rr(1,3), xx(1,3), xxr(1,3,3);
                                            for(ax=0; ax<numberOfDimensions; ax++ )
                                            {
                                                rr(0,ax)=(jv[ax]-jva[ax])*dr[ax];
                                            }
                                            map.mapS(rr,xx,xxr);
                                            printF(" point jv -> rr=(%8.2e,%8.2e,%8.2e) -> xx=(%8.2e,%8.2e,%8.2e)\n",rr(0,0),rr(0,1),rr(0,2), xx(0,0),xx(0,1),xx(0,2));
                                            det = xxr(0,0,0)*xxr(0,1,1)-xxr(0,0,1)*xxr(0,1,0);
                                            if( det!=0. )
                                            {
                                                det=1./det;
                                                re[0]=rr(0,0) + (  xxr(0,1,1)*dx[0]-xxr(0,0,1)*dx[1] )*det;
                                                re[1]=rr(0,1) + ( -xxr(0,1,0)*dx[0]+xxr(0,0,0)*dx[1] )*det;
                                            }
                                            printF(" Estimated location on grid : re=%8.2e,%8.2e,%8.2e)\n",re[0],re[1],re[2]);
                      // Overture::abort("cutHoles:checkHolePointWithBogusInverse:ERROR: finish me for parallel");
                                        #else
                    // serial version 
                                            int kv[3];
                                            if( numberOfDimensions==2 )
                                            {
                                                real xra[2][2];
                                                if( (j1+1) <= g.dimension(End,0) )
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                                	  xra[0][ax]=(vertex(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
                                              	else
                                              	{
                                                	  kv[0]=j1+1, kv[1]=j2, kv[2]=j3;
                                                	  xra[0][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(0);
                                              	}
                                                    }
                                                }
                                                else
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                                	  xra[0][ax]=(xvj[ax]-vertex(j1-1,j2,j3,ax))/g.gridSpacing(0);
                                              	else
                                              	{
                                                	  kv[0]=j1-1, kv[1]=j2, kv[2]=j3;
                                                	  xra[0][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(0);
                                              	}
                                                    }
                                                }
                                                if( (j2+1) <= g.dimension(End,1) )
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                                	  xra[1][ax]=(vertex(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
                                              	else
                                              	{
                                                	  kv[0]=j1, kv[1]=j2+1, kv[2]=j3;
                                                	  xra[1][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(1);
                                              	}
                                                    }
                                                }
                                                else
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                                	  xra[1][ax]=(xvj[ax]-vertex(j1,j2-1,j3,ax))/g.gridSpacing(1);
                                              	else
                                              	{
                                                	  kv[0]=j1, kv[1]=j2-1, kv[2]=j3;
                                                	  xra[1][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(1);
                                              	}
                                                    }
                                                }
                        // assert( (j1+1) <= g.dimension(End,0) );
                        // assert( (j2+1) <= g.dimension(End,1) );
                        // for(ax=0; ax<numberOfDimensions; ax++ ) 
                        // {
                        //   xra[0][ax]=(xb(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
                        //   xra[1][ax]=(xb(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
                        // }
                                                real det = XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0);
                                                if( det!=0. )
                                                {
                                                    det=1./det;
                                                    re[0]=(  XR(1,1)*dx[0]-XR(0,1)*dx[1] )*det;
                                                    re[1]=( -XR(1,0)*dx[0]+XR(0,0)*dx[1] )*det;
                                                }
                                                else
                                                { // if the jacobian is singular
                                                    if( debug & 1 )
                                                    {
                                              	printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
                                              	fprintf(plogFile,"cutHoles:WARNING: non-invertible point and jacobian=0. "
                                                    		"for estimating location\n");
                                              	fprintf(plogFile,"cutHoles:WARNING: grid=%i grid2=%i XR=(%8.2e,%8.2e,%8.2e,%8.2e)"
                                                    		" isRectangular=%i isRectangular2=%i\n",
                                                    		grid,grid2,XR(0,0),XR(1,0),XR(0,1),XR(1,1),isRectangular,isRectangular2);
                                              	fprintf(plogFile,"        : i=%i, ia=(%i,%i,%i) distToBndry=%8.2e\n",
                                                    		i,ia(i,0),ia(i,1),ia(i,2),distToBndry);
                                              	fprintf(plogFile,"        : jv=(%i,%i,%i) jpv=(%i,%i,%i) (jv= cutting point on grid=%i)\n",
                                                    		jv[0],jv[1],jv[2],jpv[0],jpv[1],jpv[2],grid);
                                              	fprintf(plogFile,"        : xvj=(%8.2e,%8.2e,%8.2e) xvp=(%8.2e,%8.2e,%8.2e)\n",
                                                    		xvj[0],xvj[1],xvj[2],xvp[0],xvp[1],xvp[2]);
                                              	fprintf(plogFile,"        : xb(j1,j2+1,j3,ax)=(%8.2e,%8.2e,%8.2e)\n",
                                                    		xb(j1,j2+1,j3,0),xb(j1,j2+1,j3,1),xb(j1,j2+1,j3,2));
                                                    }
                                                    re[0]=re[1]=0.;
                                                    re[axis]=.1*(2*side-1); // move point outside the grid
                                                }
                                                re[2]=0.;
                                            }
                                            else
                                            {
                        // *** 3D ***
                                                real xra[3][3];
                                                if( (j1+1) <= g.dimension(End,0) )
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                              	{
                              // *wdh* 090207 : xb only lies on (side,axis) -- use vertex instead
                    	  // *wdh* 090207 xra[0][ax]=(xb(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
                                                	  xra[0][ax]=(vertex(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
                                              	}
                                              	else
                                              	{
                                                	  kv[0]=j1+1, kv[1]=j2, kv[2]=j3;
                                                	  xra[0][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(0);
                                              	}
                                                    }
                                                }
                                                else
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                              	{
                    	  // *wdh* 090207 xra[0][ax]=(xvj[ax]-xb(j1-1,j2,j3,ax))/g.gridSpacing(0);
                                                	  xra[0][ax]=(xvj[ax]-vertex(j1-1,j2,j3,ax))/g.gridSpacing(0);
                                              	}
                                              	else
                                              	{
                                                	  kv[0]=j1-1, kv[1]=j2, kv[2]=j3;
                                                	  xra[0][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(0);
                                              	}
                                                    }
                                                }
                                                if(  (j2+1) <= g.dimension(End,1) )
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                              	{
                    	  // *wdh* 090207 xra[1][ax]=(xb(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
                                                	  xra[1][ax]=(vertex(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
                                              	}
                                              	else
                                              	{
                                                	  kv[0]=j1, kv[1]=j2+1, kv[2]=j3;
                                                	  xra[1][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(1);
                                              	}
                                                    }
                                                }
                                                else
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                              	{
                    	  // *wdh* 090207 xra[1][ax]=(xvj[ax]-xb(j1,j2-1,j3,ax))/g.gridSpacing(1);
                                                	  xra[1][ax]=(xvj[ax]-vertex(j1,j2-1,j3,ax))/g.gridSpacing(1);
                                              	}
                                              	else
                                              	{
                                                	  kv[0]=j1, kv[1]=j2-1, kv[2]=j3;
                                                	  xra[1][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(1);
                                              	}
                                                    }
                                                }
                                                if( (j3+1) <= g.dimension(End,2) )
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                              	{
                              // if( j3+1 > xb.getBound(2) )  // *wdh* 090207
                    	  // {
                    	  //   printF("ERROR: j3+1 = %i, xb.getBound(2)=%i, g.dimension(1,2)=%i\n",j3+1,xb.getBound(2),g.dimension(1,2));
                    	  //   printF("ERROR: side=%i axis=%i\n",side,axis);
                    	  //   OV_ABORT("error");
                    	  // }
                    	  // *wdh* 090207 xra[2][ax]=(xb(j1,j2,j3+1,ax)-xvj[ax])/g.gridSpacing(2);
                                                	  xra[2][ax]=(vertex(j1,j2,j3+1,ax)-xvj[ax])/g.gridSpacing(2);
                                              	}
                                              	else
                                              	{
                                                	  kv[0]=j1, kv[1]=j2, kv[2]=j3+1;
                                                	  xra[2][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(2);
                                              	}
                                                    }
                                                }
                                                else
                                                {
                                                    for(ax=0; ax<numberOfDimensions; ax++ ) 
                                                    {
                                              	if( !isRectangular )
                                              	{
                    	  // *wdh* 090207 xra[2][ax]=(xvj[ax]-xb(j1,j2,j3-1,ax))/g.gridSpacing(2);
                                                	  xra[2][ax]=(xvj[ax]-vertex(j1,j2,j3-1,ax))/g.gridSpacing(2);
                                              	}
                                              	else
                                              	{
                                                	  kv[0]=j1, kv[1]=j2, kv[2]=j3-1;
                                                	  xra[2][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(2);
                                              	}
                                                    }
                                                }
                                        /* --------------------		      
                                              assert( (j1+1) <= g.dimension(End,0) );
                                              assert( (j2+1) <= g.dimension(End,1) );
                                              assert( (j3+1) <= g.dimension(End,2) );
                                              for(ax=0; ax<numberOfDimensions; ax++ ) 
                                              {
                                              xra[0][ax]=(xb(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
                                              xra[1][ax]=(xb(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
                                              xra[2][ax]=(xb(j1,j2,j3+1,ax)-xvj[ax])/g.gridSpacing(2);
                                              }
                                              ----------------------------- */
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
                                                    if( debug & 1 )
                                              	printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
                                                    re[0]=re[1]=re[2]=0.;
                                                    re[axis]=.1*(2*side-1);
                                                }
                                            }
                                        #undef XR
                                            for( ax=0; ax<numberOfDimensions; ax++ )
                                                re[ax]+=jv[ax]*g.gridSpacing(ax);
                                        #endif
                                            if( debug & 4 )
                                            {
                                                fprintf(plogFile,"cutHoles: Non-invert pt: ia=(%i,%i,%i) est. r=(%7.2e,%7.2e,%7.2e) distToBndry=%8.2e is",
                                                  	    ia(i,0),ia(i,1),ia(i,2),re[0],re[1],re[2],distToBndry);
                                                if(fabs(re[axis]-.5) <= .5+boundaryEpsilon ) 
                                                    fprintf(plogFile," inside in the normal direction (axis=%i)!\n",axis);
                                                else
                                                    fprintf(plogFile," outside in the normal direction (axis=%i)!\n",axis);
                                            }
                      // do not cut a hole if the point is
                      //     1. inside in the normal direction
                      // or  2. on the wrong side in the normal direction
                      // or  3. outside in the tangential direction.
                                            if( fabs(re[axis]-.5) <= .5+boundaryEpsilon 
                                                    || (side==0 && re[axis  ]>=0. ) ||(side==1 && re[axis  ]<=1. )
                                                    || re[axisp1]< -g.gridSpacing(axisp1) || re[axisp1]>1.+g.gridSpacing(axisp1)
                                                    || re[axisp2]< -g.gridSpacing(axisp2) || re[axisp2]>1.+g.gridSpacing(axisp2) )
                        // *wdh* 990702 || re[axisp1]<=0. || re[axisp1]>=1.
                        // *wdh* 990702 || re[axisp2]<=0. || re[axisp2]>=1. )
                        // *wdh* 990502 || re[axisp1]<=rBound(Start,axisp1,grid) || re[axisp1]>=rBound(End,axisp1,grid)
                        // *wdh* 990502 || re[axisp2]<=rBound(Start,axisp2,grid) || re[axisp2]>=rBound(End,axisp2,grid) )
                                            {
                                                if( debug & 4 )
                                                {
                                                    fprintf(plogFile,"cutHoles: Non-invertible point: ia=(%i,%i,%i) est. r=(%7.2e,%7.2e,%7.2e) is",
                                                    	      ia(i,0),ia(i,1),ia(i,2),re[0],re[1],re[2]);
                                                    if(fabs(re[axis]-.5) <= .5+boundaryEpsilon ) 
                                              	fprintf(plogFile," inside in the normal direction (axis=%i)! No hole cut. ****** \n",axis);
                                                    else
                                              	fprintf(plogFile," outside in the tangential direction. No hole cut. \n");
                                                }
                                                mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
                                            }
                                            else
                                            {
                                                real cosAngle =0.;
                                                jpv[0]=j1; jpv[1]=j2; jpv[2]=j3;
                                                jpv[axis]=g.gridIndexRange(1-side,axis);  // opposite boundary
                                                for( ax=0; ax<numberOfDimensions; ax++ )
                                                    cosAngle += normal(j1,j2,j3,ax)*dx[ax]; //  (x2(i,ax)-xb(j1,j2,j3,ax));
                        // in 3d we should probably take the minimum of all edge lengths or use
                        // the normal distance in both 2d and 3d
                                                ipv[axis]=ia(i,axis);
                                                ipv[axisp1]=ia(i,axisp1)+1;
                        // *wdh* 080519 ipv[axisp2]=ia(i,axisp2); // ia(i,axisp2)+1; *wdh* 021230
                                                if( numberOfDimensions==2 )
                                                    ipv[2]=ia(i,2);
                                                else
                                                    ipv[axisp2]=ia(i,axisp2);        // axisp2==axisp1 in 2D ! do not over-write ipv[axisp1] !
                                                real dx2=0.;
                                                if( ipv[axis]<=g2.dimension(1,axis) )
                                                {
                                                    if( !isRectangular2 )
                                                    {
                                              	for( ax=0; ax<numberOfDimensions; ax++ )
                                                	  dx2+=SQR(center2(ia(i,0),ia(i,1),ia(i,2),ax)-center2(i1p,i2p,i3p,ax));
                                                    }
                                                    else
                                                    {
                                              	iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
                                              	for( ax=0; ax<numberOfDimensions; ax++ )
                                                	  dx2+=SQR(XC2(iv,ax)-XC2(ipv,ax));
                                                    }
                                                }
                                                cosAngle/= SQRT(max(REAL_MIN,distToBndry));
                                                const real maxCosAngle=.0;  
                        // we do not cut a hole if the cosine of the angle between normals < maxCosAngle or if
                        // the distance to the potential hole point is greater than the distance to the opposite boundary
                        // 
                                                if( debug & 4 )
                                                {
                                                    fprintf(plogFile,"cutHoles: INFO non-invert pt (%i,%i,%i) on grid=%i by grid=%i "
                                                    	      "distToBndry=%8.2e, dx2=%8.2e dx2*maxDistFactor=%8.2e\n",
                                                    	      ia(i,0),ia(i,1),ia(i,2),grid2,grid,distToBndry,dx2,dx2*maxDistFactor );
                          // fprintf(plogFile,"cutHoles: INFO: iv=(%i,%i,%i) ipv=(%i,%i,%i) dvx2=(%8.2e,%8.2e,%8.2e)"
                          //        " axisp1=%i axisp2=%i\n",
                          //      iv[0],iv[1],iv[2],ipv[0],ipv[1],ipv[2],dvx2[0],dvx2[1],dvx2[2],axisp1,axisp2);
                                                }
                                                if( cosAngle < maxCosAngle ||  // if cosine of the angle between normals > ?? 
                                              	distToBndry>dx2*maxDistFactor  || distToBndry>maximumHoleCuttingDistanceSquared ) 
                                                {
                                                    if( debug & 1 )
                                                    {
                                              	fprintf(plogFile,"cutHoles: no hole cut for non-invert pt (%i,%i,%i) on grid=%i by grid=%i ",
                                                    		ia(i,0),ia(i,1),ia(i,2),grid2,grid);
                                              	if( cosAngle < maxCosAngle )
                                                	  fprintf(plogFile,"since n.n=%7.2e <%7.2e \n",cosAngle,maxCosAngle);
                                              	else if(distToBndry>maximumHoleCuttingDistanceSquared )
                                                	  fprintf(plogFile,"since distToBndry=%7.2e >maximumHoleCuttingDistance=%7.2e\n",
                                                      		  SQRT(distToBndry),SQRT(maximumHoleCuttingDistanceSquared));
                                              	else 
                                                	  fprintf(plogFile,"since distToBndry=%7.2e > dx*2.=%7.2e j=(%i,%i,%i) jp=(%i,%i,%i)\n",
                                                      		  SQRT(distToBndry),SQRT(dx2*maxDistFactor),j1,j2,j3,j1p,j2p,j3p);
                                                    }
                                                    mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
                                                }
                                            }
                                    } 
                                    if( debug & 16 && numberOfDimensions==3 )
                                    {
                              	fprintf(plogFile,"cutHoles: grid=%s, grid2=%s pt=(%i,%i,%i) r2=(%6.2e,%6.2e,%6.2e) mask=%i\n",
                                    		(const char*)g.mapping().getName(Mapping::mappingName),
                                    		(const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
                                    		r2(i,axis1),r2(i,axis2),r2(i,axis3),mask2(ia(i,0),ia(i,1),ia(i,2)));
                              	fprintf(plogFile,"          x=(%6.2e,%6.2e,%6.2e) boundary shift=(%6.2e,%6.2e,%6.2e) \n",
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
                              	holePoint(numberOfHolePoints,numberOfDimensions)=grid;
                              	numberOfHolePoints++;
                              	if( numberOfHolePoints >= maxNumberOfHolePoints )
                              	{
                                	  maxNumberOfHolePoints*=2;
                                	  holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
                                	  printf(" ... increasing maxNumberOfHolePoints to %i\n",maxNumberOfHolePoints);
                              	}
                                    }
                                }
                // *wdh* 2012/03/17 -- this next is NOT correct if some other "grid" has set mask2<0 !
                // where( mask2(ia(R,0),ia(R,1),ia(R,2)) <= 0 )
                //   cutShare2(ia(R,0),ia(R,1),ia(R,2))=share;
                            } // end if numberCut>0 
            		
            	      }
          	    }
        	  }
      	}
            }
        }
    } // end for grid 
    

  // now double check the hole cutting
    checkHoleCutting(cg);


  // we need to set values in the inverseGrid array to -1 at all unused points *** can this be done else where ????
    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
        MappedGrid & g = cg[grid];
        const intArray & maskd = g.mask();

        g.mask().periodicUpdate();   // ****

        intArray & inverseGridd = cg.inverseGrid[grid];
        GET_LOCAL_CONST(int,maskd,mask);
        GET_LOCAL(int,inverseGridd,inverseGrid);

        getIndex(extendedGridIndexRange(g),I1,I2,I3);  
        int includeGhost=1;
        bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3,includeGhost);
        if( !ok ) continue;
        where( mask(I1,I2,I3)==0 )
            inverseGrid(I1,I2,I3)=-1;
    }
    
    if( debug & 1 && numberOfBaseGrids>2 )
    {
    // recompute the hole points for **plotting purposes only** since it is possible that some
    // hole points have been converted back into interpolation points
        numberOfHolePoints=0;
        for( grid=0; grid<numberOfBaseGrids; grid++ )
        {
            MappedGrid & g = cg[grid];
            const intArray & maskd = g.mask();
      // realArray & center = g.center();
            const bool isRectangular = g.isRectangular();

            GET_LOCAL_CONST(int,maskd,mask);
            #ifdef USE_PPP
                realSerialArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(g.center(),center);
            #else
                const realSerialArray & center = g.center();
            #endif


            real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
            int iv0[3]={0,0,0}; //
            if( isRectangular )
            {
      	g.getRectangularGridParameters( dvx, xab );
      	for( int dir=0; dir<numberOfDimensions; dir++ )
      	{
        	  iv0[dir]=g.gridIndexRange(0,dir);
        	  if( g.isAllCellCentered() )
          	    xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      	}
            		
            }
            #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))




            getIndex(extendedGridIndexRange(g),I1,I2,I3);  
            bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
            if( ok )
            {
      	for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      	{
        	  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
        	  {
          	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
          	    {
            	      if( mask(i1,i2,i3)==0 )
            	      {
            		if( !isRectangular )
            		{
              		  for( dir=0; dir<numberOfDimensions; dir++ )
                		    holePoint(numberOfHolePoints,dir) = center(i1,i2,i3,dir);
            		}
            		else
            		{
              		  for( dir=0; dir<numberOfDimensions; dir++ )
                		    holePoint(numberOfHolePoints,dir) = XC(iv,dir);
            		}
            	      
            		holePoint(numberOfHolePoints,numberOfDimensions)=grid;
            	      
            		numberOfHolePoints++;
            		assert( numberOfHolePoints < maxNumberOfHolePoints );
            	      }
          	    }
        	  }
      	}
            }
            
      // printf("*** number of hole points = %i \n",numberOfHolePoints);
            
        }
        
    }
    
    delete [] cutShare;

  // ***** It seems that we can delete these sooner -- no need to save as "grid" changes *********
    for( int b=0; b<maxNumberOfBoundaries; b++ )
    {
        delete ppxBoundary[b];
    }
    delete [] ppxBoundary;
    
    real time=getCPU();
    timeCutHoles=time-time0;
    if( info & 2 ) 
    {
        Overture::checkMemoryUsage("Ogen::cut holes (new)");
        printF(" time to cut holes (new)..................................%e (total=%e)\n",time-time0,time-totalTime);
        printF("   includes time to check hole cutting.......................%e (total=%e)\n",timeCheckHoleCutting,time-totalTime);
    }
    
    return numberOfHolePoints;
}




