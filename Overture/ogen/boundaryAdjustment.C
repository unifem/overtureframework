#include "Ogen.h"
#include "conversion.h"
#include "display.h"
#include "ParallelUtility.h"

// /Notes:
//   There are potentially BoundaryAdjustment objects for each face (side,axis) of a grid wrt another grid2:
//   If BoundaryAdjustment & bA = cg.rcData->boundaryAdjustment(grid,grid2)(side,axis) then
//   bA will hold the adjustment info for adjusting face (side,axis) of "grid" when interpolating from grid2.



// we need to define these here for gcc
// typedef CompositeGridData_BoundaryAdjustment       BoundaryAdjustment;
typedef TrivialArray<BoundaryAdjustment,Range>     BoundaryAdjustmentArray;
typedef TrivialArray<BoundaryAdjustmentArray,Range>BoundaryAdjustmentArray2;

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

int Ogen::
oppositeBoundaryIndex(MappedGrid & g, const int & ks, const int & kd )
// choose the opposite boundary to be 1/3 the distance in index space, but at least 10 lines
// We could do better here
{
  const int numberOfLines = max(10,(int)(1./3.*(g.gridIndexRange(1,kd)-g.gridIndexRange(0,kd))+.5));
  if( ks==0 )
    return min(g.gridIndexRange(1,kd), g.gridIndexRange(0,kd)+numberOfLines);
  else
    return max(g.gridIndexRange(0,kd), g.gridIndexRange(1,kd)-numberOfLines);

}


int Ogen::
checkForBoundaryAdjustments(CompositeGrid & cg, int k1, int k2, IntegerArray & sidesShare,
                            bool & needAdjustment, int manualSharedBoundaryNumber[2][3] )
//=============================================================================================
// /Description:
//
//   Check to see if we need to make boundary adjustments for shared sides between 
//  grids k1 and k2. If so allocate space for the  boundaryAdjustment, acrossGrid, and 
//  oppositeBoundary arrays.
//    
//=============================================================================================
{
  
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfGrids = cg.numberOfComponentGrids();
  const int numberOfBaseGrids = cg.numberOfBaseGrids();

  MappedGrid &g1 = cg[k1];
  MappedGrid &g2 = cg[k2];

  const intArray & mask1d = g1.mask();
  GET_LOCAL_CONST(int,mask1d,mask1);

  const Range Rx(0,numberOfDimensions-1);
  
  BoundaryAdjustmentArray2 & boundaryAdjustment = cg.rcData->boundaryAdjustment;

  // ba12(side,axis) : holds boundary adjustments for grid=k1 interpolating from grid2=k2, (side,axis)
  TrivialArray<BoundaryAdjustment,Range>& bA12 = boundaryAdjustment(k1,k2);

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  
  if (k2 == cg.baseGridNumber(k1)) 
    bA12.redim(0);  // grid k2 is a refinement grid of grid k1, no need to adjust
  else 
  {
    if( (bA12.getBound(0)-bA12.getBase(0)+1) <2 )
      bA12.redim(2,numberOfDimensions); 

    if( false )
    {
      printf(" ====== boundaryAdjustment:\n");
      for( int n=0; n<numberOfManualSharedBoundaries; n++ )
      {
	printf(" n=%i, grid1=manualSharedBoundary(n,0)=%i grid2=manualSharedBoundary(n,3)=%i\n"
	       " ks1=manualSharedBoundary(n,1)=%i kd1=manualSharedBoundary(n,2)=%i\n",
	       n,manualSharedBoundary(n,0),manualSharedBoundary(n,3),manualSharedBoundary(n,1),
	       manualSharedBoundary(n,2));
      }
    }
    

    for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
    {
      for (Integer ks1=0; ks1<2; ks1++) 
      {

        // ***********************************************************************************
        // ************** check for adjustments for face (grid,ks1,kd1) **********************
        // ***********************************************************************************


	BoundaryAdjustment& bA = bA12(ks1,kd1);
	needAdjustment = false;
	// check manual shared sides 
	for( int n=0; n<numberOfManualSharedBoundaries; n++ )
	{
	  if( manualSharedBoundary(n,0)==k1 && manualSharedBoundary(n,3)==k2 &&
	      manualSharedBoundary(n,1)==ks1 && manualSharedBoundary(n,2)==kd1 )
	  {
            // we assume there is only one manual shared boundary for these values of (k1,k2,ks1,kd1)
            // assert( manualSharedBoundaryNumber[ks1][kd1]==-1 );
	    
	    manualSharedBoundaryNumber[ks1][kd1]=n;

            int ks2=manualSharedBoundary(n,4);
	    int kd2=manualSharedBoundary(n,5);
            sidesShare(ks1,kd1,ks2,kd2 )=1;

	    needAdjustment=true;
	    break; // do this for now
	  }
	}
        // printf("*** k1=%i k2=%i ks1=%i kd1=%i needAdjustment=%i\n",k1,k2,ks1,kd1,needAdjustment);
	
        if( !needAdjustment )
	{

	  for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
	  {
	    for (Integer ks2=0; ks2<2; ks2++)
	    {
	      if (g1.boundaryCondition(ks1,kd1) > 0 && g2.boundaryCondition(ks2,kd2) > 0 &&
		  g1.sharedBoundaryFlag(ks1,kd1) &&
		  g2.sharedBoundaryFlag(ks2,kd2) == g1.sharedBoundaryFlag(ks1,kd1) &&
		  cg.mayInterpolate(k1,k2,0) )  // *wdh* added 020127
		needAdjustment = LogicalTrue;
	    }
	  }
        }
      
	if (needAdjustment)
	{
	  // build an array that lives on a boundary

          bA.create(); // this will allocate arrays if they are not already there

	  getIndex(g1.dimension(),I1,I2,I3);
	  Iv[kd1]=g1.gridIndexRange(ks1,kd1);
	  
          int includeGhost=1;
          bool ok=ParallelUtility::getLocalArrayBounds(mask1d,mask1,I1,I2,I3,includeGhost);

          // *******************************
	  if( false )
	  {
	    printf("checkForBoundaryAdjustments: myid=%i k1=%i k2=%i maskLocal=[%i,%i][%i,%i][%i,%i]"
		   " Iv=[%i,%i][%i,%i][%i,%i]\n",
		   myid,k1,k2,
		   mask1.getBase(0),mask1.getBound(0),
		   mask1.getBase(1),mask1.getBound(1),
		   mask1.getBase(2),mask1.getBound(2),
		   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());

	    printf(" grid k1=%i : gridIndexRange=[%i,%i][%i,%i][%i,%i]\n",
		   k1,
		   g1.gridIndexRange(0,0),g1.gridIndexRange(1,0),
		   g1.gridIndexRange(0,1),g1.gridIndexRange(1,1),
		   g1.gridIndexRange(0,2),g1.gridIndexRange(1,2));
	  }
          // *******************************

	  if( !ok ) continue;  // No points in this processor **** is this ok *** 
	  

          // *wdh* 070313 -- new way: use RealArray's instead of boundary grid functions
          RealArray & boundaryAdjustment = bA.boundaryAdjustment();
	  RealArray & acrossGrid = bA.acrossGrid();
	  RealArray & oppositeBoundary = bA.oppositeBoundary();
	  
	  if( boundaryAdjustment.dimension(0)!=I1 || 
              boundaryAdjustment.dimension(1)!=I2 ||
              boundaryAdjustment.dimension(2)!=I3 )
	  {
 	    bA.computedGeometry &= ~THEinverseMap;    // mark as not computed yet

            boundaryAdjustment.redim(I1,I2,I3,Rx);  boundaryAdjustment=0.; 
	    acrossGrid.redim(I1,I2,I3,Rx);          acrossGrid=0.;
	    oppositeBoundary.redim(I1,I2,I3,Rx);    oppositeBoundary=0.;
	  }

// 	  const Integer side = ks1 ? RealMappedGridFunction::endingGridIndex :
// 	    RealMappedGridFunction::startingGridIndex;
// 	  const Range d1 = kd1==0 ? Range(side,side) : Range(),
// 	    d2 = kd1==1 ? Range(side,side) : Range(),
// 	    d3 = kd1==2 ? Range(side,side) : Range();

// 	  if (( bA.boundaryAdjustment().updateToMatchGrid(g1, d1, d2, d3, Rx) |
// 		bA.acrossGrid().updateToMatchGrid(g1, d1, d2, d3, Rx) |
// 		bA.oppositeBoundary().updateToMatchGrid(g1, d1, d2, d3, Rx) )
// 	      & RealMappedGridFunction::updateResized) 
// 	  {
// 	    bA.computedGeometry &= ~THEinverseMap;    // mark as not computed yet

//             bA.boundaryAdjustment()=0.; // *wdh* 020912  -- initialize for the dec
// 	    bA.acrossGrid()=0.;
// 	    bA.oppositeBoundary()=0.;
// 	  } 

          bA.sidesShare()=BoundaryAdjustment::unknown;  

	} 
	else 
	{
          bA.hasSharedSides()=BoundaryAdjustment::doNotShare;
          bA.destroy();
	  
// 	  bA.computedGeometry &= ~THEinverseMap;
// 	  bA.boundaryAdjustment.destroy();
// 	  bA.acrossGrid        .destroy();
// 	  bA.oppositeBoundary  .destroy();
	}
      } 
    }
  } // end if

  return 0;
}


int Ogen::
getAdjustmentVectors(CompositeGrid & cg, BoundaryAdjustment& bA, 
                     int grid, int grid2, bool & needAdjustment, int numberOfPoints,
                     int it, int ks1, int kd1, int ks2, int kd2, 
                     Index Iv[3], RealArray & x1 )
// ===================================================================================================
// /Description:
//    Evaluate the adjustment vectors :
//           opposite-boundary, across-grid 
// 
// /numberOfPoints (input) : if zero on input then there are no boundary points to adjust.
// 
// This function is only called by updateBoundaryAdjustment.
// ===================================================================================================
{
  if( it!=0 ) return 0;

  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfGrids = cg.numberOfComponentGrids();
  const int numberOfBaseGrids = cg.numberOfBaseGrids();

  int k1=grid, k2=grid2;

  MappedGrid &g1 = cg[k1];
  MappedGrid &g2 = cg[k2];
  Mapping & map1 = g1.mapping().getMapping();
  
  const intArray & mask1d = g1.mask();
  GET_LOCAL_CONST(int,mask1d,mask1);

  const bool isRectangular1 = g1.isRectangular();
#ifdef USE_PPP
  realSerialArray vertex1; if( !isRectangular1 ) getLocalArrayWithGhostBoundaries(g1.vertex(),vertex1);
#else
  const realArray & vertex1 = g1.vertex();
#endif
  real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  int iv0[3]={0,0,0}; //
  if( isRectangular1 )
  {
    g1.getRectangularGridParameters( dvx, xab );
    for( int dir=0; dir<g1.numberOfDimensions(); dir++ )
      iv0[dir]=g1.gridIndexRange(0,dir);
  }
#define XV1(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))


  const Range Rx(0,numberOfDimensions-1);


  RealArray & ob = bA.oppositeBoundary();
  RealArray & ag = bA.acrossGrid();

  // NOTE: needAdjustment may be true on some processors and false on others 

  //   if( needAdjustment ) // ++++++++++++++++ do for it==0 for all processors ++++
  // ********************************************************************************
  // *** Some points need adjustment:                                            ****
  // ***    Compute the opposite boundary points  and the across-grid vectors.   ****
  // ********************************************************************************

  // choose the opposite boundary to be 1/3 the distance in index space, but at least 10 lines
  // We could do better here
  Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];


  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
  int kv[3];
  Index Im[3], &Im1=Im[0], &Im2=Im[1], &Im3=Im[2];
  Index Ib[3], &Ib1=Ib[0], &Ib2=Ib[1], &Ib3=Ib[2];

  Ib1=I1; Ib2=I2; Ib3=I3;
  Ib[kd1]=oppositeBoundaryIndex(g1,ks1,kd1);
  if( debug & 4 )
    fprintf(plogFile," grid=%i, (%i,%i) : oppositeBoundaryIndex=%i, gridIndex=%i \n",k1,ks1,kd1,
	   oppositeBoundaryIndex(g1,ks1,kd1),g1.gridIndexRange(ks1,kd1));
		
  int is[3] = {1,1,1};    // for computing face centered boundary point for CC grids.
  is[kd1]=0;
  if( numberOfDimensions==2 ) is[2]=0;

  // for cell centered case we need:
  for( int dir=0; dir<3; dir++ )
  {
    if( g1.isAllCellCentered() )
      Im[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound()-is[dir]);
    else
      Im[dir]=Iv[dir];
  }

  // ********** fix this for numToCheck==0 **********

  bool ok=true;
  int numOppositeBoundaryPoints=0;
  if( numberOfPoints>0 )
  {
    if( !isRectangular1 )
    {
      int includeGhost=1;
      ok = ParallelUtility::getLocalArrayBounds(mask1d,mask1,Ib1,Ib2,Ib3,includeGhost);
    }
    if( !ok ) // this is correct -- opp. bndry pts were not found so we need to compute them
      numOppositeBoundaryPoints = Ib1.getLength()*Ib2.getLength()*Ib3.getLength();
  }
    
  if( true ) // For parallel we must always do this next stuff
  {
    // Optionally evaluate the opposite-boundary points from the mapping, in the case when the 
    // opposite boundary points are not on this processor

    // printf("boundaryAdjustment:ERROR: opposite boundary points are not on this processor "
    //        "-- fix this Bill!\n");
    // Overture::abort("error");
    RealArray rb, xb;
    if( numOppositeBoundaryPoints>0 )
    {
      rb.redim(Ib1,Ib2,Ib3,numberOfDimensions), xb.redim(Ib1,Ib2,Ib3,numberOfDimensions);
      real *rbp = rb.Array_Descriptor.Array_View_Pointer3;
      const int rbDim0=rb.getRawDataSize(0);
      const int rbDim1=rb.getRawDataSize(1);
      const int rbDim2=rb.getRawDataSize(2);
      #define RB(i0,i1,i2,i3) rbp[i0+rbDim0*(i1+rbDim1*(i2+rbDim2*(i3)))]	

      real dr[3]={g1.gridSpacing(0),g1.gridSpacing(1),g1.gridSpacing(2)}; //
      int i1a=g1.gridIndexRange(0,0), i2a=g1.gridIndexRange(0,1), i3a=g1.gridIndexRange(0,2);
      if( numberOfDimensions==2 )
      {
	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
	  RB(i1,i2,i3,0)=(i1-i1a)*dr[0];
	  RB(i1,i2,i3,1)=(i2-i2a)*dr[1];
	}
      }
      else
      {
	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
	  RB(i1,i2,i3,0)=(i1-i1a)*dr[0];
	  RB(i1,i2,i3,1)=(i2-i2a)*dr[1];
	  RB(i1,i2,i3,2)=(i3-i3a)*dr[2];
	}
      }
    }
		
    #ifdef USE_PPP
     map1.mapGridS(rb,xb);                 // +++there may be communication here +++++++++
    #else
     if( numOppositeBoundaryPoints>0 ) 
       map1.mapGrid(rb,xb);
    #endif

    if( numberOfPoints==0 ) return 0;  // we can return here since there is no more communication
    if( numOppositeBoundaryPoints>0 )
    {
      if( g1.isAllVertexCentered() )
      {
	ob(I1,I2,I3,Rx)=xb(Ib1,Ib2,Ib3,Rx);
      }
      else
      {
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  if( dir!=kd1 )
	    Ib[dir]=Range(Ib[dir].getBase(),Ib[dir].getBound()-1);
	}
	ob(Im1,Im2,Im3,Rx)=.5*(xb(Ib1,Ib2,Ib3,Rx) + xb(Ib1+is[0],Ib2+is[1],Ib3+is[2],Rx));
      }
    }
    #undef RB
  }

  if( !ok )
  {
    // this case done above 
  }
  else if( g1.isAllVertexCentered() )
  {
    if( !isRectangular1 )
    {
      ob(I1,I2,I3,Rx)=vertex1(Ib1,Ib2,Ib3,Rx);
    }
    else
    {
      FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,Ib1,Ib2,Ib3)
      {
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  ob(i1,i2,i3,dir)=XV1(jv,dir);
	}
      }
    }
  }
  else
  {
    for( int dir=0; dir<numberOfDimensions; dir++ )
    {
      if( dir!=kd1 )
	Ib[dir]=Range(Ib[dir].getBase(),Ib[dir].getBound()-1);
    }
    if( !isRectangular1 )
    { // **** should this be Im1 here ??? why is this different than the above case??
      for( int dir=0; dir<numberOfDimensions; dir++ )
	ob(Im1,Im2,Im3,dir)=.5*(vertex1(Ib1,Ib2,Ib3,dir)+vertex1(Ib1+is[0],Ib2+is[1],Ib3+is[2],dir));
    }
    else
    {
      FOR_3IJD(i1,i2,i3,Im1,Im2,Im3,j1,j2,j3,Ib1,Ib2,Ib3)
      {
	kv[0]=j1+is[0], kv[1]=j2+is[1], kv[2]=j3+is[2];
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  ob(i1,i2,i3,dir)=.5*(XV1(jv,dir)+XV1(kv,dir));
	}
      }
    }
  }
  // printf(" Boundary adjustment for grid=%i, grid2=%i, kd1=%i, ks1=%i\n",k1,k2,kd1,ks1);
  // display(ba,"ba");
  ag(Im1,Im2,Im3,Rx) = ob(Im1,Im2,Im3,Rx) - x1(Im1,Im2,Im3,Rx);

  // Normalize ag to one over the SQUARE of its length.
  RealArray agg(Im1,Im2,Im3);
  if( numberOfDimensions==2 )
    agg=(ag(Im1,Im2,Im3,0)*ag(Im1,Im2,Im3,0) +
	 ag(Im1,Im2,Im3,1)*ag(Im1,Im2,Im3,1));
  else
    agg=(ag(Im1,Im2,Im3,0)*ag(Im1,Im2,Im3,0)+
	 ag(Im1,Im2,Im3,1)*ag(Im1,Im2,Im3,1)+
	 ag(Im1,Im2,Im3,2)*ag(Im1,Im2,Im3,2));

  if( min(agg(Im1,Im2,Im3))<=0. )
  {
    printf("updateBoundaryAdjustment: ERROR: min(agg)<=0 !\n");
    printf(" agg is the length of ag= 'across grid vector' \n");
	      
    printf(" grid=%i (%s) face=(ks1=%i,kd1=%i) grid2=%i (%s) face=(ks2=%i,kd2=%i)\n",
	   grid,(const char*)g1.getName(),ks1,kd1,
	   grid2,(const char*)g2.getName(),ks2,kd2);

    printf(" grid=%i, (%i,%i) : oppositeBoundaryIndex=%i, gridIndex=%i \n",k1,ks1,kd1,
	   oppositeBoundaryIndex(g1,ks1,kd1),g1.gridIndexRange(ks1,kd1));

    if( debug & 8 )
    {
      // display(xx,"xx : points on face of grid",logFile,"%5.2f ");
      display(ob(Im1,Im2,Im3,Rx),"ob: opposite boundary array",logFile,"%5.2f ");
      display(ag(Im1,Im2,Im3,Rx),"ag: across grid array      ",logFile,"%5.2f ");
    }
	      
    const real aggTol=max(agg(Im1,Im2,Im3))*REAL_EPSILON*100.;

    const int i3a=Im3.getBase(), i3b=Im3.getBound();
    const int i2a=Im2.getBase(), i2b=Im2.getBound();
    const int i1a=Im1.getBase(), i1b=Im1.getBound();
    for( int i3=i3a; i3<=i3b; i3++ )
    {
      for( int i2=i2a; i2<=i2b; i2++ )
      {
	for( int i1=i1a; i1<=i1b; i1++ )
	{
	  if( agg(i1,i2,i3)< aggTol )
	  {
	    bool found=false;
	    // look for a neighbouring value we can use instead
	    for( int m3=-1; m3<=1 && !found; m3++ )
	    {
	      int i3p=min(i3b,max(i3a,i3+m3));
	      for( int m2=-1; m2<=1 && !found; m2++ )
	      {
		int i2p=min(i2b,max(i2a,i2+m2));
		for( int m1=-1; m1<=1; m1++ )
		{
		  int i1p=min(i1b,max(i1a,i1+m1));
		  if( agg(i1p,i2p,i3p) > aggTol )
		  {
		    found=true;
		    agg(i1,i2,i3)=agg(i1p,i2p,i3p);
		    for( int dir=0; dir<g1.numberOfDimensions(); dir++)
		    {
		      ag(i1,i2,i3,dir)=ag(i1p,i2p,i3p,dir);
		      ob(i1,i2,i3,dir)=ob(i1p,i2p,i3p,dir);
		    }
		    break;
		  }
		}
	      }
	    }
	    if( !found )
	    {
	      printf("BoundaryAdjust: ERROR: unable to find a valid neighour for point (%i,%i,%i)\n",
		     i1,i2,i3);
	    }

	  }
	}
      }
    }
	      
    printf(" The adjustment will be zero for some points on this face. (Could be a singlarity?)\n");
  }
  where (agg(Im1,Im2,Im3) != (Real)0.)
    agg(Im1,Im2,Im3)=1./agg(Im1,Im2,Im3);
  for( int dir=0; dir<g1.numberOfDimensions(); dir++)
    ag(Im1,Im2,Im3,dir) *= agg;
                
  // RealArray t1;
  // t1=ag(I1,I2,I3,0)*(ob(I1,I2,I3,0) - x1(I1,I2,I3,0))+ag(I1,I2,I3,1)*(ob(I1,I2,I3,1) - x1(I1,I2,I3,1));
  // printf("grid=%i, grid2=%i, ks1=%i, kd1=%i \n",k1,k2,ks1,kd1);
  // display(t1,"Here is ag dot (ob-x1 ) after normalizing");
  // display(x1,"Here is x1 used ");

  // display(ob,"Here is ob used ",logFile,"%8.1e ");
  // display(ag,"Here is ag used ",logFile,"%8.1e ");
	    
  return 0;
}


int Ogen::
updateBoundaryAdjustment( CompositeGrid & cg, 
			  const int & grid, 
			  const int & grid2,
                          intSerialArray *iag,
                          realSerialArray *rg,
                          realSerialArray *xg,
                          IntegerArray & sidesShare )
// ==============================================================================
// /Description:
//   Compute boundary-adjustment data. Here is where we compute the data that is
// required to make adjustments for mis-matched boundaries. 
//
// 
// /grid,grid2 (input) : compute the boundary adjustments for shared sides between grid and grid2.
// /iag[side+2*axis] : indirect array holding a list of (i1,i2,i3) values for possible points on the face
//   (side,axis) of grid that may interpolate from grid2.
// /sharedSides (output) : true if there were any shared sides between these grids.
// /sidesShare(ks1,kd1,ks2,kd2) (output) =  < 0 : do not share ,   > 0 : do share => compute an adjustment
//
// /ba := cg.rcData->boundaryAdjustment(grid1,grid2) : holds boundary adjustment info between grid grid1 and grid2.
// /ba(side,axis).boundaryAdjustment(I1,I2,I3,0:r-1) : adjustments for a given side. 
// /ba(side,axis).acrossGrid(I1,I2,I3,0:r-1) : 
// /ba(side,axis).oppositeBoundary((I1,I2,I3,0:r-1) : a vector pointing from the boundary point into the interior; 
//  used to  blend the correction at points away from the boundary.
// 
// /Notes:
// The basic correction mechanism takes any grid points y(i1(R),i2(R),i3(R),Rx) on grid1 and shifts
// them to match the boundary on grid2. These shifted values are only used when computing interpolation
// stencils.
// \begin{verbatim}
//   x_b(i1,i2,i3,Rx) = boundary points on grid1
//   x_p(i1,i2,i3,Rx) = boundary points on grid1 projected onto the boundary of grid2
//   boundaryAdjustment(i1,i2,i3,Rx) = x_p(i1,i2,i3,Rx) ) - x_b(i1,i2,i3,Rx)
//   x_1(j1,j2,j3) = points on grid1 on a line some distance away from the boundary. These mark
//      the end of the region where the boundary adjustment is made.
//   oppositeBoundary = x_1(j1,j2,j3) - x_b(i1,i2,i3,Rx)
//   acrossGrid = [ oppositeBoundary - x_b(.,.,.,,Rx) ] / \| oppositeBoundary - x_b(.,.,.,,Rx) \|^2
//   y(i1(R),i2(R),i3(R),Rx) += boundaryAdjustment(i1(R),i2(R),i3(R),Rx) \cdot [ 
//       acrossGrid(i1(R),i2(R),i3(R),Rx) \cdot ( oppositeBoundary(i1(R),i2(R),i3(R),Rx)- y(i1(R),i2(R),i3(R),Rx)x) ) ]
// \end{verbatim}
//  If the points y(i1(R),i2(R),i3(R),Rx) lie on the boundary of grid1 then they will be shifted
//  to lie exactly on the boundary of grid2.
//
// /Algorithm:
// \begin{verbatim}
//    for( it=[0..nd-1] )  // iterate in case there are multiple shared sides.
//    {
//      for( ks1=[0..1] kd1=[0..nd-1] ) // check each face of grid
//      {
//        bA = boundaryAdjustment(grid,grid2)(ks1,kd1);
//        x = grid1.center()(Boundary)
//        first = true;  
// 
//        for( ks2=[0..1] kd2=[0..nd-1] ) // check for sharing with faces of grid2
//        {
//          if( !bA.sidesShare(ks2,kd2) ) continue;        
//
//          if( first || numberAdjusted>0 )
//          {
//            first=false;
//            if( numberAdjusted>0 ) 
//              adjustBoundary: x -> x2;
//
//            map2.inverseMap: x2 -> r
//          }
//
//          ok = | r(.,kd2)-ks2| < eps && | r(.,kd2p1)-.5 | < delta ;
//
//          project onto boundary: r -> r2
//          map2: r2 -> x3
//
//          // check normals, should be nearly in the same direction.
//          ok = ok &&  |n1.n2|/| D+D- n2| < eps;
//          
//          where( ok )
//             bA.boundaryAdjustment += x3-x2; 
//
//        } // end for( ks2...)
//        if( needAdjustment && it ==0 )
//        {
//           bA.oppositeBoundary=...
//           bA.acrossGrid = ...
//        } 
//      
//        adjust boundary: x -> x2
//        map2.inverseMap: x2 -> r2
//
//      } // end for( ks1..
//      
//      if( !wasAdjusted or it>= number of directions adjusted -1 )
//        break;  
//
//    } // end for( it...
//     
//    where( |r-rBoundary| < eps )
//      project r onto the boundary: r <- rBoundary.
//     
// \end{verbatim}
// ==============================================================================
{
  assert( grid!=grid2 );
  
  sidesShare=0;

  if( !isNew(grid) && !isNew(grid2) )
    return 0;

#ifdef USE_PPP
  if( false )
  {
    printF("updateBoundaryAdjustment::Not implemented yet for parallel. Do nothing...\n");
    return 0;
  }
#endif

  if( debug & 4 ) 
  {
    fprintf(plogFile,
      "\n ***** updateBoundaryAdjustment(%i,%i): adjust pts on grid=%i that interp from grid2=%i ****\n\n",
            grid,grid2,grid,grid2);
  }
  if( info & 2 )
  {
    // Overture::printMemoryUsage(sPrintF("updateBoundaryAdjustment (start) grid=%i grid2=%i",grid,grid2));
    Overture::checkMemoryUsage(sPrintF("updateBoundaryAdjustment (start) grid=%i grid2=%i",grid,grid2));
  }
  
//      The minimum allowed cosine of the angle between
//      normal vectors of shared boundary surfaces:
//  const Real degrees = atan(1.) / 45., minimumNormalCosine = cos(18. * degrees);

  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfGrids = cg.numberOfComponentGrids();
  const int numberOfBaseGrids = cg.numberOfBaseGrids();
  const Range Rx(0,numberOfDimensions-1);
  
  int k1,k2;
  k1=grid;
  k2=grid2;
  if( cg.baseGridNumber(k2) != k2 )
  {
    printF("Ogen::updateBoundaryAdjustment:ERROR: cg.baseGridNumber(k2) != k2, cg.baseGridNumber(k2)=%i, k2=%i\n",
           cg.baseGridNumber(k2),k2);
    Overture::abort("error");
  }
  

  BoundaryAdjustmentArray2 & boundaryAdjustment = cg.rcData->boundaryAdjustment;

  int side,axis;
  if( (boundaryAdjustment.getBound(0)-boundaryAdjustment.getBase(0)+1) == numberOfGrids &&
      (boundaryAdjustment(k1,k2).getBound(0)-boundaryAdjustment(k1,k2).getBase(0)+1) ==2 )
  {
    // The boundaryAdjustment array has already been created -- check to see if there
    // are any shared sides. 
    TrivialArray<BoundaryAdjustment,Range>& bA12 = boundaryAdjustment(k1,k2);

    int sharedSidesExist=0;
    for( axis=0; axis<cg.numberOfDimensions() && !sharedSidesExist; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( bA12(side,axis).hasSharedSides() )
	{
          // these sides share but it could be that we know from a previous computation
          // that no adjustment is needed (this could be a moving grid case, for example).
	  if( bA12(side,axis).hasSharedSides()!=BoundaryAdjustment::shareButNoAdjustmentNeeded )
	  {
	    sharedSidesExist=BoundaryAdjustment::share;
	  
	    break;
	  }
	  else
	    sharedSidesExist=BoundaryAdjustment::shareButNoAdjustmentNeeded;
	}
      }
    }
    if( sharedSidesExist!=BoundaryAdjustment::share )
    {
      if( sharedSidesExist==BoundaryAdjustment::shareButNoAdjustmentNeeded )
      {
        // always compute shared sides *****************************************************
        // There is trouble in pibMove.cmd since the grids on the share boundary may
        // not exactly match after they have moved some number of steps.

        if( debug & 2 )
	{
	  printF("updateBoundaryAdjustment: no adjustment needed for shared sides between grid=%i and grid2=%i\n"
		 "  but I am going to allow for adjustments\n", grid,grid2);
	}
	sharedSidesExist=BoundaryAdjustment::share;

      }
      else
      {
	if( debug & 2 )
	{
	  if(sharedSidesExist==0 ) 
	  {
	    fprintf(plogFile,"updateBoundaryAdjustment: no shared sides between grid=%i and grid2=%i\n",grid,grid2);
	  }
	  else
	  {
	    fprintf(plogFile,"updateBoundaryAdjustment: no adjustment needed for shared sides between "
                    " grid=%i and grid2=%i\n",grid,grid2);
	  }
	}
	return 0;
      }
    }
    
  }
  
  
  if( useBoundaryAdjustment &&
      (boundaryAdjustment.getBound(0)-boundaryAdjustment.getBase(0)+1) <numberOfGrids )
  {
    boundaryAdjustment.redim(numberOfGrids,numberOfBaseGrids);   // ************************** fix this ****
  }
  

  TrivialArray<BoundaryAdjustment,Range>& bA12 = boundaryAdjustment(k1,k2);

  // *wdh* boundaryAdjustment.redim(numberOfGrids,numberOfBaseGrids);

//  real angleDiff = 1.-maximumAngleDifferenceForNormalsOnSharedBoundaries;

//  angleDiff=.9;

  // Determine which pairs of sides may need adjustment.
  //      bc >0 on both sides and share == share with share>0
  

  MappedGrid &g1 = cg[k1];
  Mapping & map1 = g1.mapping().getMapping();
  const intArray & mask1d = g1.mask();
  const bool isRectangular1 = g1.isRectangular();

  GET_LOCAL_CONST(int,mask1d,mask1);
  
#ifdef USE_PPP
  realSerialArray vertex1; if( !isRectangular1 ) getLocalArrayWithGhostBoundaries(g1.vertex(),vertex1);
#else
  const realArray & vertex1 = g1.vertex();
#endif

  MappedGrid &g2 = cg[k2];
  Mapping & map2 = g2.mapping().getMapping();

  real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  int iv0[3]={0,0,0}; //
  if( isRectangular1 )
  {
    g1.getRectangularGridParameters( dvx, xab );
    for( int dir=0; dir<g1.numberOfDimensions(); dir++ )
      iv0[dir]=g1.gridIndexRange(0,dir);
  }
#define XV1(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))


  // ********************************************************************************************************
  // **** Check to see if we need to make boundary adjustments for shared sides between grids k1 and k2  ****
  // ********************************************************************************************************
  bool needAdjustment=false;
  int manualSharedBoundaryNumber[2][3]={-1,-1,-1,-1,-1,-1}; 

  checkForBoundaryAdjustments(cg,k1,k2,sidesShare,needAdjustment,manualSharedBoundaryNumber);



  int dir;
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
  int kv[3];
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Im[3], &Im1=Im[0], &Im2=Im[1], &Im3=Im[2];
  Range R;
  real shareTol[3][2]={0.,0.,0.,0.,0.,0.};  //

  // **********************************************
  // ***** Determine the boundary adjustments *****
  // **********************************************

  if( bA12.getNumberOfElements() )
  {
    Integer kd1, kd2, ks1, ks2;
    // Extend the bounding box by a bit 
    RealArray boundingBox;
    boundingBox = g2.boundingBox();
    for( dir=0; dir<numberOfDimensions; dir++ )
    {
      real delta=(boundingBox(End  ,dir)-boundingBox(Start,dir))*.05;
      boundingBox(Start,dir)-=delta;
      boundingBox(End  ,dir)+=delta;
    }

    // sidesShare(ks1,kd1,ks2,kd2) = 0 : unknown
    //                             < 0 : do not share
    //                             > 0 : do share => compute an adjustment
	
      

    //  *****************************************
    //  *** Compute the boundary adjustments. ***
    //  *****************************************

    RealArray rOkg[6];
    int numberOfDirectionsAdjusted=0;
    // *** we have to iterate if there are adjacent faces on grid1 that share boundaries ****
    // *** because the adjustment on one face can affect the adjustment on the other face ***

    // ------------------------------START ITERATIONS---------------------------------------------------
    for( int it=0; it<numberOfDimensions; it++ )
    {
      bool wasAdjusted=FALSE;
      for( kd1=0; kd1<numberOfDimensions; kd1++)
      {
	const int kd1p1 = (kd1+1) % numberOfDimensions;
	const int kd1p2 = (kd1+2) % numberOfDimensions;

	int is[3] = {1,1,1};    // for computing face centered boundary point for CC grids.
	is[kd1]=0;
	if( numberOfDimensions==2 ) is[2]=0;
	    
	bool directionAdjusted=false;  // set to true if we adjust for this value of kd1
	for( ks1=0; ks1<2; ks1++ ) 
	{
          // =====================================================
          // =========== check face (kd1,ks1) ====================
          // =====================================================

          #ifdef USE_PPP
	    const realSerialArray & normal1 = g1.vertexBoundaryNormalArray(ks1,kd1);
          #else
 	    const realSerialArray & normal1 = g1.vertexBoundaryNormal(ks1,kd1);
          #endif

	  BoundaryAdjustment& bA = bA12(ks1,kd1);

	  if( it==0 )
	  {
	    bA.computedGeometry &= ~(THEmask | THEinverseMap);  // mark at not computed and ??
	    if( bA.wasCreated()) 
	      bA.boundaryAdjustment() = (Real)0.;     // initialize for the iteration
	  }

	  if( !bA.wasCreated() )
	  {
	    bA.hasSharedSides()=BoundaryAdjustment::doNotShare; 
	    continue;
	  }
	  
	  RealArray & ba = bA.boundaryAdjustment();

	  RealArray & xx =xg[ks1+2*kd1];
	  RealArray & r =rg[ks1+2*kd1];
	  IntegerArray & ia = iag[ks1+2*kd1];

	  RealArray & rOk =rOkg[ks1+2*kd1];  // local to this function

	  RealArray x1;

	  RealArray r2,r3, x2,x3;

	  // First count up the number of manual shared sides for this face (grid1,ks1,kd1)
          IntegerArray manual;
          int numberOfManualSharedBoundariesOnThisFace=0;
          if( numberOfManualSharedBoundaries>0 )
	  {
	    manual.redim(numberOfManualSharedBoundaries);
	    manual=-1;
	    for( int n=0; n<numberOfManualSharedBoundaries; n++ )
	    {
	      if( manualSharedBoundary(n,0)==k1 && manualSharedBoundary(n,3)==k2 &&
		  manualSharedBoundary(n,1)==ks1 && manualSharedBoundary(n,2)==kd1 )
	      {
		manual(numberOfManualSharedBoundariesOnThisFace)=n;
		numberOfManualSharedBoundariesOnThisFace++;
	      }
	    }
	  }
	  
          // **********************************************************
	  // ***** Loop here for multiple manual shared boundaries ****
          // *****     OR for the standard case                    ****
	  // **********************************************************

          // iStart[ks1,kd1] : if there are multiple manual shared sides on a given face (grid1,ks1,kd1) then
          //                   iStart[ks1][kd1] equals the number of points on the face that we have checked so far. 

	  int iStart[2][3]={0,0,0,0,0,0};
	  for( int numberOfManual=0; numberOfManual<max(1,numberOfManualSharedBoundariesOnThisFace); numberOfManual++)
	  {
            // NOTE: There are either points specified manually to check, or we check all points on the face

            // **********************************************************************
	    // ****** Determine list of points on grid to check for adjustment   ****
	    // **********************************************************************
	    //        xx(R,Rx) == vertex1(ia(R,0),ia(R,1),ia(R,2),Rx) : points to adjust

            int kd2a, kd2b, ks2a, ks2b;  // bounds on kd2 and ks2
	    
	    if( numberOfManualSharedBoundariesOnThisFace==0 )
	    {
	      // ***************************************************
	      // *** standard case: no manual shared boundaries ****
	      // ***************************************************

              // --- check all faces of grid2 ---
              kd2a=0; kd2b=numberOfDimensions-1;
	      ks2a=0; ks2b=1;

	      getBoundaryIndex(g1.dimension(),ks1,kd1,I1,I2,I3);
	      Iv[kd1]=Index(g1.gridIndexRange(ks1,kd1),1);
	    
              //      angleDiff=.9; // **** fix this ***
	      
	    }
	    else
	    {
	      //  *************************************
	      //  ***** manual shared boundary  *******
	      //  *************************************

              const int m=manual(numberOfManual);
              assert( m>=0 && m<numberOfManualSharedBoundaries );

	      ks2=manualSharedBoundary(m,4);  // we share a boundary with this face of grid2
	      kd2=manualSharedBoundary(m,5);
	      sidesShare(ks1,kd1,ks2,kd2 )=1;

              // --- we only need to check one face of grid2: ---
	      ks2a=ks2; ks2b=ks2;
              kd2a=kd2; kd2b=kd2;

              //       angleDiff=1.-manualSharedBoundaryValue(m,2);
              //	      printf(" ****** angleDiff=%e ***** \n",angleDiff );

              if( Ogen::debug & 2 )
		fprintf(plogFile,"UBA: ** k1=%i, k2=%i, ks1=%i, kd1=%i, ks2=%i, kd2=%i, numberOfManual=%i m=%i\n",
		       k1,k2,ks1,kd1,ks2,kd2,numberOfManual,m);
		

	      I1=Range(manualSharedBoundary(m, 6),manualSharedBoundary(m, 7));
	      I2=Range(manualSharedBoundary(m, 8),manualSharedBoundary(m, 9));
	      I3=Range(manualSharedBoundary(m,10),manualSharedBoundary(m,11));

	      if( debug & 1 ) printf(" boundaryAdjust:INFO: Manual shared boundary:"
				     " pts [i1a,i1b]x[i2a,i2b]x[i3a,i3b]=[%i,%i]x[%i,%i]x[%i,%i] "
				     " for grid %i (%s) from grid %i (%s) \n",
				     I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
				     k1,(const char*)g1.getName(),k2,(const char*)g2.getName());
	      

	      const IntegerArray & d = g1.dimension();
	      if( I1.getBase()<d(0,0) || I1.getBound()>d(1,0) ||
		  I2.getBase()<d(0,1) || I2.getBound()>d(1,1) ||
		  I3.getBase()<d(0,2) || I3.getBound()>d(1,2) ) 
	      {
		printf("boundaryAdjustment:ERROR: manual shared boundary points are out of range!\n");
		Overture::abort("error");
	      }
	      
	    } // end else manual shared boundary

	    // for cell centered case we need:
	    for( dir=0; dir<3; dir++ )
	      if( g1.isAllCellCentered() )
		Im[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound()-is[dir]);
	      else
		Im[dir]=Iv[dir];

            int includeGhost=1;
            bool ok=ParallelUtility::getLocalArrayBounds(mask1d,mask1,I1,I2,I3,includeGhost);
            ok= ok && ParallelUtility::getLocalArrayBounds(mask1d,mask1,Im1,Im2,Im3,includeGhost);


            // **************************************************************************************
            // *** numberOfPoints = number of points of face (grid1,ks1,kd1) to project onto one ***
            // ***                  or more faces of grid 2                                      ***
            // *** numToCheck = number of points of face (grid1,ks1,kd1) that are inside the     ***
            // ***              bounding box of grid2                                            ***
            // *************************************************************************************

	    int numberOfPoints=0;


	    if( ok )
	      numberOfPoints = I1.length()*I2.length()*I3.length();

            // If numberOfPoints==0 at this point then there is no more work to do, but in parallel we
            //     must continue since the mapS and inverseMapS used below are collective 
            #ifndef USE_PPP
   	    // if( !ok ) continue;  // No points in this processor **** is this ok ***  +++++++++++++++++++++
            #endif

	    Range p = numberOfPoints;

	    int numToCheck=0;
            R=numToCheck;
	    
	    if( numberOfPoints>0  )
	    {
	      if( it==0  ) // IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
	      {
		// We only need to compute the points xx on the first time through...

		// Get the geometry at vertices or center of the face for cell centered grids.
		x1.redim(I1,I2,I3,Rx);
		if( g1.isAllVertexCentered() )
		{
		  if( !isRectangular1 )
		    x1=vertex1(I1,I2,I3,Rx);      // **** could be a reference in this case
		  else
		  {
		    FOR_3D(i1,i2,i3,I1,I2,I3)
		    {
		      for( dir=0; dir<numberOfDimensions; dir++ )
			x1(i1,i2,i3,dir)=XV1(iv,dir);
		    }
		  }
		}
		else // cell-centred case 
		{
		  x1=0.;
		  if( !isRectangular1 )
		  {
		    for( dir=0; dir<numberOfDimensions; dir++ )
		      x1(Im1,Im2,Im3,dir)=.5*(vertex1(Im1,Im2,Im3,dir)+
					      vertex1(Im1+is[0],Im2+is[1],Im3+is[2],dir));
		  }
		  else
		  {
		    FOR_3D(i1,i2,i3,Im1,Im2,Im3)
		    {
		      jv[0]=i1+is[0], jv[1]=i2+is[1], jv[2]=i3+is[2];
		      for( dir=0; dir<numberOfDimensions; dir++ )
			x1(i1,i2,i3,dir)=.5*(XV1(iv,dir)+XV1(jv,dir));
		    }
		  }
		}

		if( numberOfManual==0 )
		{
		  // first time:
		  iStart[ks1][kd1]=0;
		  ia.redim(p,3);  // this is needed to apply adjustments already made
		}
		else
		{
		  iStart[ks1][kd1]=r.getBound(0)+1;
		  ia.resize(iStart[ks1][kd1]+numberOfPoints,3);
		}
	      
                // ----------------------------------------------------------------------------------------------
                // --- Make a sub-list of points of grid1 (ks1,kd1) that are inside the bounding box of grid2 ---
                // ----------------------------------------------------------------------------------------------

		int i1,i2,i3;
		int i=iStart[ks1][kd1];
		for( i3=Im3.getBase(); i3<=Im3.getBound(); i3++ )
		{
		  for( i2=Im2.getBase(); i2<=Im2.getBound(); i2++ )
		  {
		    for( i1=Im1.getBase(); i1<=Im1.getBound(); i1++ )
		    {
		      // *** should use bounding box for the side of the grid?? *****
		      if(x1(i1,i2,i3,0)>= boundingBox(Start,axis1) && x1(i1,i2,i3,0)<= boundingBox(End,axis1) &&
			 x1(i1,i2,i3,1)>= boundingBox(Start,axis2) && x1(i1,i2,i3,1)<= boundingBox(End,axis2) &&
			 ( cg.numberOfDimensions()==2 || 
			   x1(i1,i2,i3,2)>= boundingBox(Start,axis3) && x1(i1,i2,i3,2)<= boundingBox(End,axis3) ) )
		      {
			ia(i,0)=i1;
			ia(i,1)=i2;
			ia(i,2)=i3;
			i++;
		      }
		    }
		  }
		}

                // if there are multiple manual shared sides on a given face (grid1,ks1,kd1) then iStart[ks1,kd1]
                // keeps track of previous points we have checked.
                numToCheck=i-iStart[ks1][kd1];
		
		R=Range(iStart[ks1][kd1],i-1);

// 		if( i<=iStart[ks1][kd1] )
// 		  continue;
	      
                if( numToCheck>0 )
		{
		  if( numberOfManual==0 )
		  {
		    xx.redim(R,Rx);
		    r.redim(R,Rx);   r=-1.;
		    rOk.redim(R,Rx); rOk=-1.;  // if 0 or 1 this holds the projected r values.
		  }
		  else
		  {
		    xx.resize(R.getBound()+1,Rx);
		    r.resize(R.getBound()+1,Rx);   r(R,Rx)=-1.;
		    rOk.resize(R.getBound()+1,Rx); rOk(R,Rx)=-1.;
		  }
	      
		  for( dir=0; dir<numberOfDimensions; dir++ )
		    xx(R,dir)=x1(ia(R,0),ia(R,1),ia(R,2),dir);
		  
		}
		
	      } // end if it==0  
	      else
	      {
                numToCheck=r.getBound(0)-iStart[ks1][kd1]+1;
		
// 		if( r.getBound(0)<iStart[ks1][kd1] ) // r.getLength(0)<=0 )
// 		  continue;

                if( numToCheck>0 )
  		  R=Range(iStart[ks1][kd1],r.getBound(0));
	      }  // end else if it IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
	      
	      // Compute the boundary parameters on grid g2.
	      if( numToCheck>0 )
	      {
		r2.redim(R,Rx); x3.redim(R,Rx);
	      }
	      
	    } // end if numberOfPoints>0 
	    
	    assert( numToCheck==R.getLength() );
	    if( numberOfPoints==0 ) assert( numToCheck==0 );

	    int dim =0;
            if( numberOfPoints>0 )
              dim= ba.getLength(0)*ba.getLength(1)*ba.getLength(2);
	    if( manualSharedBoundaryNumber[ks1][kd1]<0 )
	      assert( numberOfPoints==dim);

            // *************************************************************************************
	    // ***** Check whether any boundaries of g2 share with side (ks1,kd1) of grid g1. ******
            // *************************************************************************************
	    IntegerArray isOk;
	    if( numToCheck>0 ) isOk.redim(R);
	    needAdjustment = false;
	    bool first=true;

	    for( kd2=kd2a; kd2<=kd2b; kd2++)
	    {
	      for( ks2=ks2a; ks2<=ks2b; ks2++)
	      {

		determineBoundaryPointsToAdjust(cg, grid, grid2,sidesShare, ks1, kd1, ks2,kd2, bA ,
						first,needAdjustment,
						numberOfDirectionsAdjusted, directionAdjusted,wasAdjusted,
						R, ia, isOk, it,shareTol,
						r, r2, r3, rOk, 
						xx, x2, x3  );
	      } // end for ks2
	    }  // end for kd2
	    
            // *************
            // if( numToCheck==0 ) continue;

            // ******************************************************
            // Evaluate the adjustment vectors :
            //    opposite-boundary, across-grid 
            // ******************************************************

            getAdjustmentVectors(cg,bA,grid,grid2,needAdjustment,numberOfPoints, it,ks1,kd1,ks2,kd2,Iv,x1 );
 

	    bA.computedGeometry |= THEinverseMap;
	    bA.hasSharedSides()=BoundaryAdjustment::share;
            
	    // ************************************************************************
	    // **** Now apply the adjustment and recompute grid2 r-coordinates ********
	    // ************************************************************************

	    if( numToCheck>0 )
	    {
	      x2=xx(R,Rx);
	      adjustBoundary(cg,k1,k2,ia(R,Rx),x2);
	    }
	    else
	    {
	      adjustBoundary(cg,k1,k2,Overture::nullIntArray(),Overture::nullRealArray());
	    }
	    if( numToCheck>0 )
	    {
	      r2(R,Rx)=r(R,Rx);
	      r(R,Rx)=-1.;  // ***wdh***
	    }
	    
            #ifdef USE_PPP
  	     if( numToCheck>0 )
	      map2.inverseMapCS(x2(R,Rx), r(R,Rx));  // +++++++++++++++++++++++++++++++++++++++
             else
	      map2.inverseMapCS(Overture::nullRealArray(),Overture::nullRealArray());  
            #else
             if( numToCheck>0 )
	      map2.inverseMapC(x2(R,Rx), r(R,Rx));
            #endif
	    if( needAdjustment && debug & 4 )
	    {
	      checkBoundaryAdjustment(cg, grid, grid2, ks1,kd1, bA , numberOfDirectionsAdjusted, 
				      R,ia,isOk, r, r2, r3, rOk, xx, x2, x3);
	    }
	    
	  } // end for numberOfManual...


	} // end for ks1
	if( it==0 && directionAdjusted )
	  numberOfDirectionsAdjusted++;
      } // end for kd1

      if( it>=numberOfDirectionsAdjusted-1 || !wasAdjusted )
	break;
      
    } // for it

 
    int numberOfFacesAdjusted=0;  // count the number of faces where adjustments were really needed.
    for (axis=0; axis<numberOfDimensions; axis++)
    {
      for (side=Start; side<=End; side++ )
      {
	BoundaryAdjustment& bA = bA12(side,axis);
	// *wdh* 2012/04/19 if( !bA.wasCreated()  || min(abs(bA.sidesShare()-BoundaryAdjustment::share)) !=0 )
        // We must check that there are NO values of bA.sidesShare() > 0 :
	if( !bA.wasCreated()  || max(abs(bA.sidesShare()-BoundaryAdjustment::share)) ==0 )
	{
	  // no shared sides needing adjustments remain 
	  bA.hasSharedSides()=BoundaryAdjustment::shareButNoAdjustmentNeeded;

	  if( debug & 8 ) 
	    fprintf(plogFile,"++++++++ UBA: ---NO boundary adjustments for (grid,side,axis)=(%i,%i,%i) "
                   " interpolating from grid2=%i, deleting adjustment arrays. ++++++++++++ \n",
		   k1,side,axis,k2);

	  bA.destroy(); // *wdh* 070410
	}
	else
	{
	  numberOfFacesAdjusted++;
	}
	
      }
    }
    // now set the r values to be exactly on the boundaries for points deemed to be matching.
    for (kd1=0; kd1<numberOfDimensions; kd1++)
    {
      for (ks1=0; ks1<2; ks1++) 
      {
	RealArray & r =rg[ks1+2*kd1];
	RealArray & rOk =rOkg[ks1+2*kd1];
	Range R=r.dimension(0);
	if( R.getLength()>0 )
	{
	  for( dir=0; dir<numberOfDimensions; dir++)
	  {
	    where( fabs(rOk(R,dir)-r(R,dir)) < boundaryEps*100. ) // *******************************  
	      r(R,dir)=rOk(R,dir);
	    // where( fabs(rOk(R,dir)-r(R,dir)) < max(shareTol[kd1][ks1],boundaryEps*100.) ) // ****************
	    //  r(R,dir)=rOk(R,dir);
	  }
	}
      }
    }

    if( numberOfFacesAdjusted==0 ) // *wdh* 070410
    {
      
      // printF("---NO boundary adjustments needed for grid=%i interpolating from grid2=%i, "
      // 	     "deleting boundaryAdjustment(%i,%i)\n",k1,k2,k1,k2);
      // bA12.redim(0);  -- this causes trouble, why??
    }
  } // end if( bA12.getNumberOfElements()

  cg.rcData->computedGeometry &= THEinverseMap;
  
  if( info & 2 )
  {
    // Overture::printMemoryUsage(sPrintF("updateBoundaryAdjustment (end) grid=%i grid2=%i",grid,grid2));
    Overture::checkMemoryUsage(sPrintF("updateBoundaryAdjustment (end) grid=%i grid2=%i",grid,grid2));
  }
  if( debug > 1 )
  {
    fflush(plogFile);
  }
  
  return 0;
}



//! Determine the points to adjust on a shared boundary when interpolating points from grid side (ks1,kd1)
//  to grid2 side (ks2,kd2).
int Ogen::
determineBoundaryPointsToAdjust(CompositeGrid & cg, 
				const int grid, 
				const int grid2,
				IntegerArray & sidesShare, 
				const int ks1, const int kd1, const int ks2, const int kd2, 
				BoundaryAdjustment & bA , bool & first, bool & needAdjustment,
				int numberOfDirectionsAdjusted, bool & directionAdjusted, bool & wasAdjusted,
				Range & R, IntegerArray & ia, IntegerArray & ok, const int it, real shareTol[3][2],
				RealArray & r, RealArray & r2, RealArray & r3, RealArray & rOk, 
				RealArray & xx, RealArray & x2, RealArray & x3  )
{
  int numToCheck = R.getLength();

  const int k1=grid;
  const int k2=grid2;
  
  MappedGrid & g1 = cg[grid];
  MappedGrid & g2= cg[grid2];
  
  Mapping & map1 = g1.mapping().getMapping();
  Mapping & map2 = g2.mapping().getMapping();

  #ifdef USE_PPP
    const realSerialArray & normal1 = g1.vertexBoundaryNormalArray(ks1,kd1);
  #else
    const realSerialArray & normal1 = g1.vertexBoundaryNormal(ks1,kd1);
  #endif

  const int numberOfDimensions=cg.numberOfDimensions();
  Range Rx=numberOfDimensions;
  
  RealArray & ba = bA.boundaryAdjustment();
  

  int dir;
  
  const int kd1p1 = (kd1+1) % numberOfDimensions;
  const int kd1p2 = (kd1+2) % numberOfDimensions;

  const int kd2p1 = (kd2+1) % numberOfDimensions;
  const int kd2p2 = (kd2+2) % numberOfDimensions;
		

  if( sidesShare(ks1,kd1,ks2,kd2 )<0 )
    return 0;

  if( sidesShare(ks1,kd1,ks2,kd2) > 0 || (
    g1.boundaryCondition(ks1,kd1) > 0 && g2.boundaryCondition(ks2,kd2) > 0 &&
    g1.sharedBoundaryFlag(ks1,kd1) &&
    g2.sharedBoundaryFlag(ks2,kd2) == g1.sharedBoundaryFlag(ks1,kd1) &&
    map1.intersects( map2, ks1,kd1, ks2,kd2, .1 ) ) )
  {
    // This side could be sharing
    sidesShare(ks1,kd1,ks2,kd2 )=1;
    bA.sidesShare()(ks2,kd2)=BoundaryAdjustment::share;  // may change if no adjustments are needed.

    needAdjustment=true;

    #ifdef USE_PPP
      const realSerialArray & normal2 = g2.vertexBoundaryNormalArray(ks2,kd2);
    #else
      const realSerialArray & normal2 = g2.vertexBoundaryNormal(ks2,kd2);
    #endif

    
    // Get the tolerances for shared boundaries
    real rTol,xTol,nTol;
    getSharedBoundaryTolerances( cg,k1,ks1,kd1,k2,ks2,kd2,rTol,xTol,nTol );

    real angleDiff=1.-nTol;
//      printf(" **** k1,ks1,kd2=(%i,%i,%i) k2=ks2,kd2=(%i,%i,%i) rTol=%8.2e xTol=%8.2e nTol=%8.2e angleDiff=%8.2e ****\n",
//          k1,ks1,kd1,k2,ks2,kd2,rTol,xTol,nTol,angleDiff );


    if( first || numberOfDirectionsAdjusted>0 )
    {
      first=false;
		      
      if( numToCheck>0 )
      {
	x2.redim(R,Rx);
	x2=xx(R,Rx);  // x2 will be the adjusted version of x1
      }
     
      if( numberOfDirectionsAdjusted>0 )
      {
	// apply adjustment so far if we are adjusting more than one direction
	if( debug & 2 )
	{
	  fprintf(plogFile,"\n determineBoundaryPointsToAdjust: grid=%i, (%i,%i),  grid2=%i, (%i,%i) it=%i, "
		  " apply adjustment so far",k1,ks1,kd1,k2,ks2,kd2,it);
	}
	// we need to reshape ba since it is used by adjustBoundary
	if( numToCheck>0 )
          adjustBoundary(cg,k1,k2,ia(R,Rx),x2);
        else
          adjustBoundary(cg,k1,k2,Overture::nullIntArray(),Overture::nullRealArray());
	if( debug & 2 )
	{
	  fprintf(plogFile,"determineBoundaryPointsToAdjust : max adjustment=%e\n",max(fabs(x2(R,Rx)-xx(R,Rx))));
	}
      }
      if( numToCheck>0 ) r(R,Rx)=-1.;  // ***wdh***
      #ifdef USE_PPP
        if( numToCheck>0 )
          map2.inverseMapCS(x2(R,Rx), r(R,Rx));                // +++++++++++++++++++++++++++++++++++++++
        else
          map2.inverseMapCS(Overture::nullRealArray(),Overture::nullRealArray()); 
      #else
        if( numToCheck>0 )
          map2.inverseMapC(x2(R,Rx), r(R,Rx));
      #endif
    }

/* --------
   // *** note *** here we assume that we are within .4 of the boundary --- fix this ----
   if( numberOfDimensions==2 )
   ok=fabs(r(R,kd2)-(real)ks2) < .4 && abs(r(R,kd2p1)-.5) < .9;
   else
   ok=fabs(r(R,kd2)-(real)ks2) < .4 && fabs(r(R,kd2p1)-.5) < .9 && fabs(r(R,kd2p2)-.5) < .9;
   ----------- */

	// *****  const real bbEps=.3; // max(.01,.25*g2.gridSpacing(kd2)); // ***** should estimate this properly ****

	// *****************************************************************
	// ***** determine which points to adjust based on the position ****
	// *****************************************************************
	// In the normal direction we must be within bbEps
	// In the tangential direction(s) we must be within delta1 (delta2)

    // const real bbEps=rTol; // .8; // max(.01,.25*g2.gridSpacing(kd2)); // ***** should estimate this properly ****
    const real bbEps=rTol*g2.gridSpacing(kd2); // *wdh* 2012/04/20 TRY THIS 

    // only consider points that are close in the tangential direction -- one ghost line
    const real delta1=.5+g2.gridSpacing(kd2p1);  //  *wdh* 990701 : used to be .4 !
    const real delta2=.5+g2.gridSpacing(kd2p2);
    if( numToCheck>0 )
    {
      if( numberOfDimensions==2 )
	ok=fabs(r(R,kd2)-(real)ks2) < bbEps && abs(r(R,kd2p1)-.5) < delta1;
      else
	ok=fabs(r(R,kd2)-(real)ks2) < bbEps && fabs(r(R,kd2p1)-.5) < delta1 && fabs(r(R,kd2p2)-.5) < delta2;

      if( debug & 2)
      {
	fprintf(plogFile,"\n DBPTA: Project grid=%i(%s), face=(%i,%i) onto grid2=%i(%s), face=(%i,%i)\n"
		" ",k1,(const char*)g1.getName(),ks1,kd1,k2,(const char*)g2.getName(),ks2,kd2);
// 	display(x2,"Here is x2, points to be projected",logFile,"%5.2f ");
// 	display(r,"Here is r, the projected parametric boundary points",logFile,"%5.2f ");
// 	display(ok,"Here is ok: points that should be projected",logFile,"%2i");
        fprintf(plogFile,"DBPTA: x2 = pts to be projected, r=projected parameter values, ok=pts that should be projected\n");
	
	for( int i=R.getBase(); i<=R.getBound(); i++ )
	{
	  fprintf(plogFile,"DBPTA: i=%i ia=(%i,%i,%i) x2=(%8.2e,%8.2e,%8.2e) r=(%8.2e,%8.2e,%8.2e) ok=%i\n",
		  i,
                  ia(i,0),ia(i,1),(numberOfDimensions==3 ? ia(i,2) : 0),
                  x2(i,0),x2(i,1),(numberOfDimensions==3 ? x2(i,2) : 0.),
                  r(i,0),r(i,1),(numberOfDimensions==3 ? r(i,2) : 0.),ok(i));
	}

      }
		
      //   Project the results onto the boundary of g2.
      // *** we could build a sub-list here of ok points ****
      r2(R,Rx)=r(R,Rx);
      where( ok )
      {
	r2(R,kd2)=(real)ks2;
      }
      otherwise()
      {
	for( dir=0; dir<numberOfDimensions; dir++ ) 
	  r2(R,dir)=0.;
      }
    }
    
    // Compute the projected boundary points.
 
    // *** ---> Evaluate derivatives x2r and then we can compute the normals2  <- ********

    RealArray xr2; 
    if( numToCheck>0 ) xr2.redim(R,Rx,Rx);

    #ifdef USE_PPP
      if( numToCheck>0 )
        map2.mapS(r2, x3, xr2);                                       // +++++++++++++++++++++++++++++++++++++++
      else
        map2.mapS(Overture::nullRealArray(),Overture::nullRealArray());
    #else
      if( numToCheck>0 ) map2.map(r2, x3, xr2);
    #endif

    // -------------------------
    if( debug & 4 )
    {
      if( numToCheck>0 ) 
      {
	r3.redim(R,Rx);
	r3=-1;
      }
      #ifdef USE_PPP
       if( numToCheck>0 ) 
         map2.inverseMapCS( x3,r3 );
       else
         map2.inverseMapCS( Overture::nullRealArray(),Overture::nullRealArray());
      #else
        if( numToCheck>0 ) map2.inverseMapC( x3,r3 );
      #endif
    }
    // -------------------------

    // **************************************
    // **** Check for matching of normals ***
    // **************************************
    const real cvOffset = g1.isAllVertexCentered() ? .5 : 0;
    RealArray n1, n2;
    if( numToCheck>0 ) 
    {
      n1.redim(R,Rx); n2.redim(R,Rx);
      for( dir=0; dir<numberOfDimensions; dir++ )
      {
	n1(R,dir) = normal1(ia(R,0),ia(R,1),ia(R,2),dir);
      }
    }
    
    // compute the normals on map2 at the points that were projected onto the boundary (*wdh* 070313)
    if( true )
    {
      // new way
      const real signForJacobian2=map2.getSignForJacobian();
      getNormal(numberOfDimensions,ks2,kd2,xr2,signForJacobian2, n2);    // no communication here
    }
    else
    {
      // old way
      IntegerArray ia2(R,3);
      if( numberOfDimensions<3 )
        ia2(R,axis3)=g2.indexRange(Start,axis3);
      for( dir=0; dir<numberOfDimensions; dir++ )
      {
	if( dir!=kd2 )
	{
	  equals(ia2(R,dir),evaluate(r2(R,dir)*(1./g2.gridSpacing(dir))
				     +g2.indexRange(Start,dir)+cvOffset));
	  ia2(R,dir)=max(g2.dimension(Start,dir),min(g2.dimension(End,dir),ia2(R,dir)));
	}
	else
	  ia2(R,dir)=g2.gridIndexRange(ks2,kd2);
      }
      for( dir=0; dir<numberOfDimensions; dir++ )
      {
	n2(R,dir)=normal2(ia2(R,0),ia2(R,1),ia2(R,2),dir);  // normal at closest point
      }
    }
    
    if( debug & 2 && numToCheck>0 )
    {
      fprintf(plogFile,"\n DBPTA: Project grid=%i, face=(%i,%i) onto grid2=%i, face=(%i,%i)\n"
	      " ",k1,ks1,kd1,k2,ks2,kd2);
      display(r2,"DBPTA:Here is r2, the projected parametric boundary points (only for ok pts)",plogFile,"%5.2f ");
      display(xx(R,Rx),"DBPTA:Here is xx ",plogFile,"%5.2f ");
      display(x3(R,Rx),"DBPTA:Here is x3 (xx projected onto the boundary) ",plogFile,"%5.2f ");
      display(x3(R,Rx)-xx(R,Rx),"DBPTA:Here is x3-xx the shift",plogFile,"%5.2f ");
      display(n1,"DBPTA:Here n1 -- the normal on grid 1",plogFile,"%5.2f ");
      display(n2,"DBPTA:Here n2 -- the normal on grid 2",plogFile,"%5.2f ");
    }
		

    if( debug & 16 && numToCheck>0 )
    {
      fprintf(plogFile,"DBPTA: *** boundaryEps=%e **** \n",boundaryEps);
      display(ok,"DBPTA: ok before normal test",plogFile);
    }
		    
    // ***check***
    RealArray nDot;
    if( numToCheck>0 )
    {
      nDot.redim(R);
      if( numberOfDimensions==2 )
	nDot = n1(R,0)*n2(R,0)+n1(R,1)*n2(R,1);
      else
	nDot = n1(R,0)*n2(R,0)+n1(R,1)*n2(R,1)+n1(R,2)*n2(R,2);

      if( debug & 8 ) 
      {
	display(r(R,kd2),"DBPTA: r(R,kd2)",plogFile,"%8.2e ");
	display(nDot,"DBPTA: nDot",plogFile,"%8.2e ");
      }
    }
    
    // nDiff can be very large!!
    // **** 990701 nDot+=min(.5,nDiff*(.5/numberOfDimensions));

//    real angleDiff=.9;
		
    if( debug & 2 && numToCheck>0 )
    {
      int numberRemovedByNormalCheck=0;
      numberRemovedByNormalCheck = sum( ok && fabs(r(R,kd2)-(real)ks2) <.01 && (nDot < angleDiff) );
      if( numberRemovedByNormalCheck>0 )
      {
	fprintf(plogFile,
               "DBPTA: WARNING:Grid %s (%i,%i) shares a boundary with grid %s (%i,%i) and %i points were\n"
	       "  close but not adjusted since cos(angle) between the normals was < %e \n",
	       (const char*)map1.getName(Mapping::mappingName),ks1,kd1,
	       (const char*)map2.getName(Mapping::mappingName),ks2,kd2,
	       numberRemovedByNormalCheck,angleDiff );
	if( numberOfDimensions==3 )
	{
	  for( int i=R.getBase(); i<=R.getBound(); i++ )
	  {
	    if( ok(i) && fabs(r(i,kd2)-(real)ks2) <.01 && nDot(i) < angleDiff )
	    {
	      fprintf(plogFile,"DBPTA:  pt (%4i,%4i,%4i) on grid %i has cos(angle) = %e with grid2=%i, "
		      "r=(%6.2e,%6.2e,%6.2e) n1=(%6.2e,%6.2e,%6.2e) n2=(%6.2e,%6.2e,%6.2e) \n",
		      ia(i,0),ia(i,1),ia(i,2),k1,nDot(i),k2,r(i,0),r(i,1),r(i,2),
                      n1(i,0),n1(i,1),n1(i,2),n2(i,0),n2(i,1),n2(i,2));
	    }
	  }
	}
      }
    }
		    
    // *****************************************************************
    // ***** determine which points to adjust based on the Normals *****
    // *****************************************************************
    // There is no need to adjust 
    //          1) points that are very close (these will be shared points)
    //          2) points where nDot is small (these will NOT be shared points)
                
    if( numToCheck>0 )
    {
      where( !ok ||  fabs(r(R,kd2)-(real)ks2) <boundaryEps || (nDot < angleDiff) )
	ok = LogicalFalse;
    }

    if( debug & 16 &&  numToCheck>0 )
      display(ok,"DBPTA: ok after normal test",plogFile);

    // *****************************************
    // **** Check for x-distance between pts ***
    // *****************************************

    if( xTol< .05*REAL_MAX &&  numToCheck>0 )
    {
      // dist = distance between the points xx on grid k1 and the points projected onto the boundary of grid k2
      RealArray dist(R);
      if( numberOfDimensions==2 )
        dist=SQR(x3(R,0)-xx(R,0)) + SQR(x3(R,1)-xx(R,1));
      else
        dist=SQR(x3(R,0)-xx(R,0)) + SQR(x3(R,1)-xx(R,1)) + SQR(x3(R,2)-xx(R,2));

      dist=SQRT(dist);
      
      where( dist>xTol )
      {
	ok=false;
      }
      // display(ok,"ok after xTol check","%2i");
    }


    // Compute the boundary adjustment.
    int numToAdjust = sum(ok);
    // fix me : this requires communication 
    numToAdjust = ParallelUtility::getMaxValue(numToAdjust); // in parallel we must adjust if any proc needs to adjust
    if( numToAdjust>0 ) 
    {
      // some points require adjustment
      if( numToCheck>0 )
      {
	where (ok)
	{
	  rOk(R,kd2)=(real)ks2;  // holds projected r values
		    
	  for ( dir=0; dir<numberOfDimensions; dir++)
	  {
	    ba(ia(R,0),ia(R,1),ia(R,2),dir) += x3(R,dir) - x2(R,dir);
	    // const RealArray & t1 = evaluate(x3(R,dir) - x2(R,dir));
	    // ba(ia(R,0),ia(R,1),ia(R,2),dir) += t1;
	    // diff=max(diff,max(fabs(ba(ia(R,0),ia(R,1),ia(R,2),dir))));
	  }
	  shareTol[kd1][ks1]=max(fabs(r(R,kd2)-(real)ks2));
	}
      }
      if( debug & 1 )
	fprintf(plogFile,"DBPTA: Grid %s (%i,%i) shares a bndry with grid %s (%i,%i) boundaryEps=%e "
		"shareTol=%8.1e it=%i\n",
		(const char*)map1.getName(Mapping::mappingName),ks1,kd1,
		(const char*)map2.getName(Mapping::mappingName),ks2,kd2,boundaryEps,shareTol[kd1][ks1],it);

      // display(ok,"Here is ok");
      // display(r,"Here is r, the inverse of x2");
      // display(ba,"Here is the boundaryAdjustment");
      // display(x3(R,Rx)-x2(R,Rx),"Here is the correction to ba");
		      
      bA.computedGeometry |= THEmask;
      directionAdjusted=true;
      wasAdjusted=true;
    }
    else
    {
      if( debug & 2 )
	fprintf(plogFile,"DBPTA: Grid %s (%i,%i) shares a boundary with grid %s (%i,%i) but no adjustments needed\n",
	       (const char*)map1.getName(Mapping::mappingName),ks1,kd1,
	       (const char*)map2.getName(Mapping::mappingName),ks2,kd2);

      bA.sidesShare()(ks2,kd2)=BoundaryAdjustment::shareButNoAdjustmentNeeded;

      if( debug & 8 )
      {
	real maxDiff=0.;
	where( fabs(r(R,kd2))<2. )
	{
	  maxDiff=max(fabs(r(R,kd2)-(real)ks2));
	}
	fprintf(plogFile,"DBPTA: boundaryEps = %e, max(fabs(r(R,kd2)-(real)ks2)=%e \n",boundaryEps,maxDiff);
	display(x2,"DBPTA: Here is x2",plogFile);
      }
      // *wdh* 990718 break; // The adjustment is zero.
      return 0;  // we cannot break since an adjacent side may have adjustments
    } 
  }
  else
  {
    sidesShare(ks1,kd1,ks2,kd2 )=-1;   // sides do not share
    bA.sidesShare()(ks2,kd2)=BoundaryAdjustment::doNotShare;
  }


  return 0;
}






//! Check the boundary adjustment for debugging purposes
int Ogen::
checkBoundaryAdjustment(CompositeGrid & cg, 
			const int grid, 
			const int grid2,
			const int ks1, const int kd1,
			BoundaryAdjustment & bA ,
			int numberOfDirectionsAdjusted, 
			Range & R, IntegerArray & ia, IntegerArray & ok,
			RealArray & r, RealArray & r2, RealArray & r3, RealArray & rOk, 
			RealArray & xx, RealArray & x2, RealArray & x3)
{
  // Compute the actual boundary adjustment for testing

  const int k1=grid;
  const int k2=grid2;
  
  const int numberOfDimensions=cg.numberOfDimensions();

  RealArray & ba = bA.boundaryAdjustment();

  int numToCheck=R.getLength();

  real tolerance=0., maximumBoundaryAdjustment=0.;
  if( numberOfDimensions==2 )
  {
    if( numToCheck>0 )
    {
      ok=( fabs(ba(ia(R,0),ia(R,1),ia(R,2),0))+fabs(ba(ia(R,0),ia(R,1),ia(R,2),1)) ) > 0.;
      where( ok )
      {
	tolerance=max(min(fabs(r(R,axis1)),min(
			    fabs(r(R,axis1)-1.),min(fabs(r(R,axis2)),fabs(r(R,axis2)-1.)))));
	maximumBoundaryAdjustment=max(max(fabs(ba(ia(R,0),ia(R,1),ia(R,2),0))),
				      max(fabs(ba(ia(R,0),ia(R,1),ia(R,2),1))));
      }
    }
    if( debug & 4 )
    {
      fprintf(plogFile,"updateBoundaryAdjustment: grid=%i,ks1=%i,kd1=%i grid2=%i numToCheck=%i : \n",
              grid,ks1,kd1,grid2,numToCheck);
      for(int i=0; i<=R.getBound(); i++ )
      {
	// if( ok(i) )
	fprintf(plogFile,"ok=%i (i1,i2)=(%3i,%3i), ba=(%8.1e,%8.1e) r=(%8.1e,%8.1e) rp=(%8.1e,%8.1e)\n"
		"      x=(%e,%e), xp=(%e,%e) \n",
		ok(i),ia(i,0),ia(i,1), ba(ia(i,0),ia(i,1),ia(i,2),0),  ba(ia(i,0),ia(i,1),ia(i,2),1),
		r2(i,0),r2(i,1),r(i,0),r(i,1),
		xx(i,0),xx(i,1),x2(i,0),x2(i,1));
      }
    }
  }
  else
  {
    if( numToCheck>0 )
    {
      ok=( fabs(ba(ia(R,0),ia(R,1),ia(R,2),0))+
	   fabs(ba(ia(R,0),ia(R,1),ia(R,2),1))+
	   fabs(ba(ia(R,0),ia(R,1),ia(R,2),2)) ) > 0.;
      where( ok )
      {
	tolerance=max( min(fabs(r(R,axis1)),
			   min(fabs(r(R,axis1)-1.),
			       min(fabs(r(R,axis2)),
				   min(fabs(r(R,axis2)-1.),
				       min(fabs(r(R,axis3)),fabs(r(R,axis3)-1.)))))) );
	maximumBoundaryAdjustment=max(max(fabs(ba(ia(R,0),ia(R,1),ia(R,2),0))),
				      max(fabs(ba(ia(R,0),ia(R,1),ia(R,2),1))),
				      max(fabs(ba(ia(R,0),ia(R,1),ia(R,2),2))));
		      
      }
    }
    if( debug & 4 )
    {
      fprintf(plogFile,"updateBoundaryAdjustment: grid=%i,ks1=%i,kd1=%i grid2=%i numToCheck=%i : \n",
              grid,ks1,kd1,grid2,numToCheck);
      for(int i=0; i<=R.getBound(); i++ )
      {
	fprintf(plogFile,"ok=%i i=(%3i,%3i,%3i), ba=(%8.1e,%8.1e,%8.1e) "
		"r=(%8.1e,%8.1e,%8.1e) rp=(%8.1e,%8.1e,%8.1e)\n"
		"      x=(%e,%e,%e), xp=(%e,%e,%e) x3-xp=(%8.1e,%8.1e,%8.1e) r3=(%8.1e,%8.1e,%8.1e)\n",
		ok(i),ia(i,0),ia(i,1),ia(i,2),
		ba(ia(i,0),ia(i,1),ia(i,2),0),  ba(ia(i,0),ia(i,1),ia(i,2),1), 
		ba(ia(i,0),ia(i,1),ia(i,2),2),  r2(i,0),r2(i,1),r2(i,2),  r(i,0),r(i,1),r(i,2),
		xx(i,0),xx(i,1),xx(i,2),x2(i,0),x2(i,1),x2(i,2),
		x3(i,0)-x2(i,0),x3(i,1)-x2(i,1),x3(i,2)-x2(i,2),r3(i,0),r3(i,1),r3(i,2)
	  );
      }
    }
  }
  fprintf(plogFile,"\n **** grid=%i, grid2=%i, ks1=%i, kd1=%i max(ba)=%e, tolerance for adjusted boundary is %e\n",
	 k1,k2,ks1,kd1,maximumBoundaryAdjustment,tolerance);

  return 0;
}

  
