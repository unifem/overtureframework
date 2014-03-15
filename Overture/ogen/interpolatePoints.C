#include "Overture.h"
#include "Ogen.h"
// #include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"

// This function also appears in checkOverlap -- fix me 
static inline int 
decode(int mask )
// decode the mask ->  1=interior, 2=ghost, -2,3=interiorBoundaryPoint,  <0 =interp 
{
  int m=0;
  if( (mask & MappedGrid::ISdiscretizationPoint) && (mask & MappedGrid::ISinteriorBoundaryPoint) )
    m=3;
  else if( mask & MappedGrid::ISdiscretizationPoint )
    m=1;
  if( mask<0 && mask>-100 )
    m=mask;
  else if( mask<0 )
    m=-1;
  else if( mask & MappedGrid::ISghostPoint )
    m=2;

  if( mask<0 && (mask & MappedGrid::ISinteriorBoundaryPoint) )
    m=-2;

  return m;
}

//======================================================================================================
/// \brief Attempt to interpolate a list of points. For points that could interpolate the
/// interpolation data mask, inverseGrid and inverseCoordinates are changed.
///
/// \param cg (input) : 
/// \param grid (input) : points lie on this grid
/// \param numberToInterpolate (input) : number of points in the list ia
/// \param ia(i,0:2) (input) : index coordinates of the list of points
/// \param interpolates(i) (output) : true if the point was interpolated.
//======================================================================================================
int Ogen::
interpolatePoints(CompositeGrid & cg, int grid, int numberToInterpolate, const IntegerArray & ia, 
                        IntegerArray & interpolates )
{

  if( debug & 4 )
  {
    printF("++++interpolatePoints::Attempt to interpolate points on grid=%i\n",grid);
    fprintf(plogFile,"++++interpolatePoints:: Attempt to interpolate points on grid=%i numberToInterpolate=%i\n",
            grid,numberToInterpolate);
  }
  
  const int numberOfDimensions=cg.numberOfDimensions();
  const int numberOfBaseGrids=cg.numberOfBaseGrids();

  MappedGrid & g=cg[grid];
  intArray & mask = g.mask();
  OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
  
  Mapping & map = g.mapping().getMapping();
  Range Rx=numberOfDimensions;
  
  intArray & inverseGrid = cg.inverseGrid[grid];
  OV_GET_SERIAL_ARRAY(int,inverseGrid,inverseGridLocal);

  realArray & rI = cg.inverseCoordinates[grid];
  OV_GET_SERIAL_ARRAY(real,rI,rILocal);

  const bool isRectangular = g.isRectangular();
  real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  int iv0[3]={0,0,0}; //
  if( isRectangular )
  {
    g.getRectangularGridParameters( dvx, xab );
    for( int dir=0; dir<g.numberOfDimensions(); dir++ )
    {
      iv0[dir]=g.gridIndexRange(0,dir);
      if( g.isAllCellCentered() )
	xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
    }
		
  }
  #undef XVC
  #define XVC(i,axis) (xab[0][axis]+dvx[axis]*(i-iv0[axis]))

  realArray & center = g.center();
  OV_GET_SERIAL_ARRAY_CONDITIONAL(real,center,centerLocal,!isRectangular);  // we only need centerLocal if not rectangular

  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  i3=g.gridIndexRange(0,axis3);  // for 3D
  
  int numberToCheck=numberToInterpolate;

  // -- make a new list of the points to be interpolated ----
  //    This list will grow smaller as the points are interpolated
  //  ib(j,0:2) = sub-set of ia(i,0:2) of pts that cannot interpolate
  //  ja(j) : index back to the original ib list: ia(ja(j),0:2) ==  ib(j,0:2)
  IntegerArray ib, ja;
  if( numberToCheck>0 ) 
  {
    ib.redim(numberToCheck,3);
    ja.redim(numberToCheck);
  }
  
  int j=0;
  for( int i=0; i<numberToCheck; i++ )
  {
    ja(j)=i; // index back into the original list of points :  ia(ja(j),0:2) ==  ib(j,0:2) 
    ib(j,0)=ia(i,0); ib(j,1)=ia(i,1); ib(j,2)=ia(i,2);
    interpolates(j)=false;
    j++;
  }
  assert( j==numberToCheck );

  int totalNumberToCheck = ParallelUtility::getSum(numberToCheck);
  

  // x0(j,0:2) : (original) physical coordinates of the point j
  // x(j,0:2) : holds x0 values that may have a boundary adjustment (which may differ for each donor)
  // r(j,0:2) : inverse coordinates on a given donor grid
  RealArray x0, x, r;
  IntegerArray useBackupRules, cgInterpolates;
  bool tryBackupRules=false;  // we do NOT use backup rules here
  if( numberToCheck>0 ) 
  {
    x0.redim(numberToCheck,3);  
    x.redim(numberToCheck,3);
    r.redim(numberToCheck,3);
    useBackupRules.redim(numberToCheck);
    cgInterpolates.redim(numberToCheck);
    
    useBackupRules=tryBackupRules;
  }

  // --- evaluate the x-coordinates of the points to check : x0(i,0:nd-1) ---
  if( !isRectangular )
  {
#ifndef USE_PPP
    if( numberOfDimensions==2 )
    {
      for( int i=0; i<numberToCheck; i++ )
      {
	for( int axis=0; axis<numberOfDimensions; axis++ )
	  x0(i,axis)=centerLocal(ib(i,0),ib(i,1),i3,axis);
      }
    }
    else
    {
      for( int i=0; i<numberToCheck; i++ )
      {
	for( int axis=0; axis<numberOfDimensions; axis++ )
	  x0(i,axis)=centerLocal(ib(i,0),ib(i,1),ib(i,2),axis);
      }
    }
    
#else
    // In Parallel - evaluate the mapping -- we should maybe do this once at the top!
    for( int i=0; i<numberToCheck; i++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	r(i,axis) = (ib(i,axis)-g.gridIndexRange(0,axis))*g.gridSpacing(axis);
      }
    }
    map.mapS(r,x0);
#endif
  }
  else
  {
    for( int i=0; i<numberToCheck; i++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
	x0(i,axis)=XVC(ib(i,axis),axis);
    }
  }
  x=x0;

  // *************************************************
  // **** Try to interpolate from any other grid  ****
  // *************************************************

  for( int gg=1; gg<numberOfBaseGrids; gg++ )
  {
    int grid2 = numberOfBaseGrids-gg;
    if( grid2<=grid )
      grid2--;
	  
    if( !cg.mayInterpolate(grid,grid2,0) ) continue;

    MappedGrid & g2 = cg[grid2];
    Mapping & map2 = g2.mapping().getMapping();
    if( debug & 2 )
      fprintf(plogFile,"interpolatePoints:try to interpolate from grid2=%s \n",
              (const char*)map2.getName(Mapping::mappingName));

    if( map.intersects(map2,-1,-1,-1,-1,.1) )
    {
      // try to interpolate from grid2...

      Range R;
      if( numberToCheck>0 )
      {
	R=numberToCheck;
	if( numberToCheck!=x.getLength(0) )
	{
	  x.resize(numberToCheck,3);
	  r.redim(numberToCheck,3);
	  useBackupRules.redim(numberToCheck);
	  cgInterpolates.redim(numberToCheck);
          useBackupRules=tryBackupRules;
	}

	cgInterpolates=true;
        r=-1.;  // initial guess for inverseMap is "no guess"
      }
      else if( x.getLength(0)>0 )
      {
	x.redim(0); r.redim(0); useBackupRules.redim(0); cgInterpolates.redim(0);
      }
      


      // -- invert the points ---
      if( ib.getLength(0)!=numberToCheck )
      { // adjustBoundary looks at ib for the number of points to check -- we could pass numberToCheck?
	if( numberToCheck>0 )
	  ib.resize(numberToCheck,3);
	else
	  ib.redim(0);
      }
	

      if( useBoundaryAdjustment )
	adjustBoundary(cg,grid,grid2,ib,x ); 

      #ifdef USE_PPP
        map2.inverseMapS(x,r);  
      #else
        map2.inverseMap(x,r);
      #endif

      // -- determine if the point x with inverse coordinates r can interpolate from this donor grid --
      #ifdef USE_PPP
       checkCanInterpolate(cg, grid, grid2, numberToCheck, r, cgInterpolates, useBackupRules);
      #else
       checkForOneSided=false;
       cg.rcData->canInterpolate(grid, grid2, r, cgInterpolates, useBackupRules, checkForOneSided );
      #endif

      // -- adjust the interp data for points that could interpolate --
      int j=0;
      for( int i=0; i<numberToCheck; i++ )
      {
	if( cgInterpolates(i) )
	{
          // i1=ib(i,0); i2=ib(i,1); i3=ib(i,2);
          for( int axis=0; axis<numberOfDimensions; axis++ )
            iv[axis]=ib(i,axis);

	  interpolates(ja(i))=true;

	  if( debug & 2 ) 
            fprintf(plogFile,">>>interpPoints: The point (grid,i1,i2,i3)=(%i,%i,%i,%i) (%s) CAN "
                    "interpolate from grid2=%i\n",grid,i1,i2,i3,(const char*)g.getName(),grid2);

	  // we assume here that (i1,i2,i3) is in the local mask! -- is this always true?
	  maskLocal(i1,i2,i3)=MappedGrid::ISinterpolationPoint; 
	  inverseGridLocal(i1,i2,i3)= grid2;  
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    rILocal(i1,i2,i3,axis)=r(i,axis);
	  
	}
	else
	{
          // make a compressed list of points still to check
 	  if( i!=j )
 	  {
 	    ib(j,0)=ib(i,0); ib(j,1)=ib(i,1); ib(j,2)=ib(i,2);
            int ii=ja(i);
	    ja(j)=ii;
            for( int axis=0; axis<3; axis++ )
              x(j,axis)=x0(ii,axis); // reset x to non-boundary adjusted value 
 	  }
 	  j++;

	  if( debug & 2 ) 
	  {
	    for( int axis=0; axis<numberOfDimensions; axis++ )
	      iv[axis]=ib(i,axis);
            fprintf(plogFile,">>>interpPoints: The point (grid,i1,i2,i3)=(%i,%i,%i,%i) (%s) can NOT "
                    "interpolate from grid2=%i\n",grid,i1,i2,i3,(const char*)g.getName(),grid2);
	  }
	  
	}
      } // end for( i ) 
      
      numberToCheck=j;  // new number to check 
      totalNumberToCheck=ParallelUtility::getSum(numberToCheck);

      if( totalNumberToCheck==0 ) // note: in parallel we must only stop if all processors are done
        break;  // we are done
      
      
    } // if map.intersects(...)
  }  // for( int g=1; g


  return 0;
}

// ========================================================================================================
/// \brief Try to interpolate a point
///  If interpolatePoint==true then attempt to interpolate the point.
///   Return true if the point was interpolated.
/// \param cg (input) : CompositeGrid
/// \param grid (input) : point to interpolate is on this grid.
/// \param iv[3] (input) : index of the point to interpolate.
/// \param interpolatePoint (input) : if true and the point (grid,iv) can interpolate from another donor grid
///      then adjust the mask, inverseGrid and inverseCoordinates to indicate this.
/// \param checkInterpolationCoords (input) : 
/// \param checkBoundaryPoint (input) : if true then project the point (grid,iv) onto the nearest boundary
///       before attempting to interpolate. Also check the normals between the interpolation point and donor.
/// \param infoLevel (input) : bit flag, infoLevel = 0 + 1 + 2 to output additional information.
///
// ========================================================================================================
bool Ogen::
interpolateAPoint(CompositeGrid & cg, int grid, int iv[3], bool interpolatePoint, 
                  bool checkInterpolationCoords, bool checkBoundaryPoint, int infoLevel )
{
  bool wasInterpolated=false;

  if( debug & 4 )
  {
    printF("++++interpolateAPoint::Attempt to interpolate the pt (grid,i1,i2,i3)=(%i,%i,%i,%i)\n",grid,iv[0],iv[1],iv[2]);
  }
  

  int &i1=iv[0], &i2=iv[1], &i3=iv[2];
  const int numberOfDimensions=cg.numberOfDimensions();
  MappedGrid & g=cg[grid];
  realArray & center = g.center();
  intArray & mask = g.mask();
  OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
  
  Mapping & map = g.mapping().getMapping();
  Range Rx=numberOfDimensions;
  
  intArray & inverseGrid = cg.inverseGrid[grid];
  OV_GET_SERIAL_ARRAY(int,inverseGrid,inverseGridLocal);

  realArray & rI = cg.inverseCoordinates[grid];
  OV_GET_SERIAL_ARRAY(real,rI,rILocal);



  const bool isRectangular = g.isRectangular();
  real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  int iv0[3]={0,0,0}; //
  if( isRectangular )
  {
    g.getRectangularGridParameters( dvx, xab );
    for( int dir=0; dir<g.numberOfDimensions(); dir++ )
    {
      iv0[dir]=g.gridIndexRange(0,dir);
      if( g.isAllCellCentered() )
	xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
    }
		
  }
  #undef XC
  #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

  OV_GET_SERIAL_ARRAY_CONDITIONAL(real,center,centerLocal,!isRectangular);  // we only need centerLocal if not rectangular


  RealArray x(1,3), r(1,3), r2(1,3), x2(1,3), xr2(1,3,3);
  r=-1.;  r2=-1.; x=0.; x2=0.; xr2=0.;

  if( !(i1>=g.dimension(0,0) && i1<=g.dimension(1,0) &&
        i2>=g.dimension(0,1) && i2<=g.dimension(1,1) &&
        i3>=g.dimension(0,2) && i3<=g.dimension(1,2)) )
  {
    printF("interpolateAPoint:ERROR: (i1,i2,i3) should be in the ranges [%i,%i]x[%i,%i]x[%i,%i]\n",
	   g.dimension(0,0),g.dimension(1,0),g.dimension(0,1),g.dimension(1,1),
	   g.dimension(0,2),g.dimension(1,2));
    return wasInterpolated;
  }

  #ifdef USE_PPP
   const int proc = mask.Array_Descriptor.findProcNum( iv );  // point this on this processor
  #else
   const int proc =0 ;
  #endif

  int sidec=-1, axisc=-1;  // closest face
  if( checkBoundaryPoint )
  {
    // find the closest boundary point -- use x-distance!
    int ivb[3];
    findClosestBoundaryPoint( g, &x(0,0), iv, ivb, sidec, axisc );

    if( sidec>=0 )
    {
      iv[axisc]=g.gridIndexRange(sidec,axisc);
      if( infoLevel & 1 ) printF(" --> closest boundary point: iv=(%i,%i,%i) face=(%i,%i)\n",i1,i2,i3,sidec,axisc);
    }
    else
    {
      if( infoLevel & 1 ) printF(" --> no closest boundary point (?)\n");
    }
  }



  
  // x0 = location of point (grid,iv) (before any boundary adjustment)
  RealArray x0(1,3); x0=0.; 
  if( !isRectangular )
  {
#ifndef USE_PPP
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      x0(0,axis)=center(iv[0],iv[1],iv[2],axis);      // need to start from original pt.
    }
#else
    // In Parallel - evaluate the mapping
    RealArray ri(1,3); ri=0.;
    for( int axis=0; axis<numberOfDimensions; axis++ )
      ri(0,axis) = (iv[axis]-g.gridIndexRange(0,axis))*g.gridSpacing(axis);
    map.mapS(ri,x0);
#endif
  }
  else
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
      x0(0,axis)=XC(iv,axis);      // need to start from original pt.
  }


  if( infoLevel & 1 )
  {
    if( myid==proc )
    {
      printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      printf(" **** interpolateAPoint: (grid,i1,i2,i3)=(%i,%i,%i,%i) (%s) :  x=(%11.5e,%11.5e,%11.5e), mask=%i decode=%i\n",
	     grid,i1,i2,i3,(const char*)g.getName(),
	     x0(0,0),x0(0,1),x0(0,2),maskLocal(i1,i2,i3),decode(maskLocal(i1,i2,i3)));
      fflush(0);
    }
    
  }
  // ---------------------------------------------------------------------------------
  // -- loop over other grids and determine if we can interpolate from any of them ---
  // ---------------------------------------------------------------------------------

  for( int gg=1; gg<cg.numberOfComponentGrids(); gg++ )
  {
    int grid2 = cg.numberOfComponentGrids()-gg;
    if( grid2<=grid )
      grid2--;
	  
    if( !cg.mayInterpolate(grid,grid2,0) ) continue;

    MappedGrid & g2 = cg[grid2];
    // *wdh* 110621 g2.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
    
    Mapping & map2 = g2.mapping().getMapping();
    if( infoLevel & 2 ) 
      printF("  Try to interpolate from grid2=%s \n",(const char*)map2.getName(Mapping::mappingName));

    if( map.intersects(map2,-1,-1,-1,-1,.1) )
    {
      x=x0;  // x may be adjusted for nearby boundaries below
	 
      if( useBoundaryAdjustment )
      {
	IntegerArray ia(1,3);
	ia(0,0)=iv[0]; ia(0,1)=iv[1]; ia(0,2)=iv[2];
	adjustBoundary(cg,grid,grid2,ia,x(0,Rx)); 
      }
	    
      Mapping::debug=infoLevel;  // *** turn on info for mapping inverse
      if( infoLevel & 2 ) printF("  >>>>>>>>>>>>>>call inverseMap for grid2=%s with debug info on:\n",
				 (const char*)g2.getName());
      r=-1.;
      map2.inverseMapS(x(0,Rx),r);
      if( infoLevel & 2 ) printF("  <<<<<<<<<<<<<<< return from inverseMap\n");
      Mapping::debug=0;
	    

      IntegerArray interpolates(1); interpolates=true;
      IntegerArray useBackupRules(1);  useBackupRules=false;

      // *wdh* 2012/05/31 checkForOneSided=false;
      checkForOneSided=true;
      #ifdef USE_PPP
        checkCanInterpolate( cg ,grid, grid2, r, interpolates, useBackupRules );
      #else
        cg.rcData->canInterpolate(cg.gridNumber(grid),cg.gridNumber(grid2), r, interpolates, 
				  useBackupRules, checkForOneSided );
      #endif
      if( interpolates(0) )
      {
	if( infoLevel & 1 ) printF("\n>>> The point (grid,i1,i2,i3)=(%i,%i,%i,%i) (%s) CAN interpolate",grid,i1,i2,i3,(const char*)g.getName());
        if( !wasInterpolated )
	{
	  wasInterpolated=true;
	  if( interpolatePoint )
	  {
            // we assume here that (i1,i2,i3) is in the local mask! -- is this always true?
	    maskLocal(i1,i2,i3)=MappedGrid::ISinterpolationPoint; 
	    inverseGridLocal(i1,i2,i3)= grid2;  
	    for( int axis=0; axis<numberOfDimensions; axis++ )
	      rILocal(i1,i2,i3,axis)=r(0,axis);
	  }
	}
      }
      else
      {
	if( infoLevel & 1 ) printF("\n>>> The point (grid,i1,i2,i3)=(%i,%i,%i,%i) can NOT interpolate",
                                    grid,i1,i2,i3,(const char*)g.getName());
      }
      

      if( infoLevel & 1 ) 
      {

	printF(" from grid2=%i (%s): \n"
	       "    The point to interpolate is: x=(%7.3e,%7.3e,%7.3e), x(after bndry adjust)=(%7.3e,%7.3e,%7.3e)  adjust=(%7.1e,%7.1e,%7.1e)\n"
	       "    grid2=%i unit square coords: r=(%6.2e,%6.2e,%6.2e) (a value of 1.00e+01 means the inverse failed)\n",
	       grid2,(const char*)map2.getName(Mapping::mappingName),
	       x0(0,0),x0(0,1),x0(0,2), x(0,0),x(0,1),x(0,2),x0(0,0)-x(0,0),x0(0,1)-x(0,1),x0(0,2)-x(0,2),
	       grid2,r(0,0),r(0,1),r(0,2) );

	int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
	j1=j2=j3=0;
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  jv[axis]=int(r(0,axis)/g2.gridSpacing(axis)+g2.gridIndexRange(0,axis)+.5);
	}
	const intArray & mask2 = g2.mask();
        OV_GET_SERIAL_ARRAY_CONST(int,mask2,mask2Local);
	
        const bool isRectangular2 = g2.isRectangular();
	realArray & vertex2 = g2.vertex();
        OV_GET_SERIAL_ARRAY_CONDITIONAL(real,vertex2,vertex2Local,!isRectangular2);

	const int iw = cg.interpolationWidth(0,grid,grid2); // interpolation width 
	const int hw = iw/2;  // interpolation half width 

	if( j1>=g2.dimension(0,0) && j1<=g2.dimension(1,0) &&
	    j2>=g2.dimension(0,1) && j2<=g2.dimension(1,1) &&
	    j3>=g2.dimension(0,2) && j3<=g2.dimension(1,2) )
	{
          #ifdef USE_PPP
          const int proc2 = mask2.Array_Descriptor.findProcNum( jv );
          #else
          const int proc2 = 0;
          #endif
	  if( myid==proc2 )
	  {
	    if( isRectangular2 )  // *wdh* 110621
	    {
	      real dvx2[3]={1.,1.,1.}, xab2[2][3]={{0.,0.,0.},{0.,0.,0.}};
	      g2.getRectangularGridParameters( dvx2, xab2 );
              #undef XC2
              #define XC2(jv,axis) (xab2[0][axis]+dvx2[axis]*(jv[axis]-g2.gridIndexRange(0,axis)))
	      for( int axis=0; axis<numberOfDimensions; axis++ )
                x2(0,axis)=XC2(jv,axis);
	    }
	    else
	    {
              for( int axis=0; axis<numberOfDimensions; axis++ )
		x2(0,axis)=vertex2Local(j1,j2,j3,axis);
	    }
	    
	    printf("    The closest point on grid2=%i (%s) is (j1,j2,j3)=(%i,%i,%i) with coords x=(%11.5e,%11.5e,%11.5e), mask=%i decode=%i\n",
		   grid2,(const char*)g2.getName(), j1,j2,j3, x2(0,0),x2(0,1),x2(0,2),mask2Local(j1,j2,j3),decode(mask2Local(j1,j2,j3)));
	    printf("    Here is the mask on grid2=%i (mask: 1=interior, 2=ghost, -2,3=interiorBoundaryPoint,  <0 =interp) \n",grid2);
	    for( int k3=j3-hw; k3<=j3+hw; k3++ )
	    {
	      if( k3<g2.dimension(0,2) || k3>g2.dimension(1,2) )
		continue;
	      for( int k2=j2-hw; k2<=j2+hw; k2++ )
	      {
		if( k2<g2.dimension(0,1) || k2>g2.dimension(1,1) )
		  continue;
		printf("      mask2(%i:%i,%i,%i) =",j1-hw,j1+hw,k2,k3);

		for( int k1=j1-hw; k1<=j1+hw; k1++ )
		{
		  if( k1>=g2.dimension(0,0) && k1<=g2.dimension(1,0) )
		  {
		    printf(" %3i ",decode(mask2Local(k1,k2,k3)));
		  }
		}
		printf("\n");
	      }
	    }	
	    fflush(0);
	  }
	}
	
      }
      
      if( wasInterpolated ) break;
      
      int iv2[3]={0,0,0};
      if( checkInterpolationCoords && !interpolates(0) )
      {
        // --- double check the interpolation coordinates ---
        //   (1) find the distance between the inverse point and the nearest grid point
        //   
	assert( map2.approximateGlobalInverse!=NULL );
	r2=-1.;

        // This next function only finds the nearest grid point on this processor:
        // map2.approximateGlobalInverse->findNearestGridPoint(0,0,x,r2 );
	// printf(" myid=%i : nearest grid point on grid2=% is at r2=(%8.2e,%8.2e,%8.2e)\n",myid,grid2,r2(0,0),r2(0,1),r2(0,2));


	RealArray dista(1);  dista=REAL_MAX;
	RealArray xa(1,3);  xa=0.; // coordinates of nearest grid pt.
        map2.findNearestGridPoint( x,r2,dista,xa );
	// printf(" myid=%i : nearest grid point on grid2=%i is at r2=(%8.2e,%8.2e,%8.2e) xa=(%8.2e,%8.2e,%8.2e)\n",
        //          myid,grid2,r2(0,0),r2(0,1),r2(0,2),xa(0,0),xa(0,1),xa(0,2));


	
	map2.mapS(r2,x2,xr2);  // get xr2 so we can estimate the grid spacing in physical space

	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  iv2[axis]=int(r2(0,axis)/g2.gridSpacing(axis)+.5)+g2.gridIndexRange(0,axis);

	// --- estimate the local grid spacing ---
        //    dx = (dx/dr)*dr
	real dx2=0.;
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
          real dxa=0.;
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    dxa += xr2(0,axis,dir)*g2.gridSpacing(dir);
	  }
          dx2 += SQR(dxa);
	}
	dx2=sqrt(dx2);

	if( infoLevel & 1 ) 
	{
	  real dist;
	  if( numberOfDimensions==2 )
	  {
	    dist=sqrt( SQR(x2(0,0)-x(0,0))+SQR(x2(0,1)-x(0,1)) );
	  }
	  else
	  {
	    dist=sqrt( SQR(x2(0,0)-x(0,0))+SQR(x2(0,1)-x(0,1))+SQR(x2(0,2)-x(0,2)) );
	  }
	  
          printF(" --- Check the interpolation coordinates with the nearest grid point: ---\n");
	  printF(" Closest grid pt on grid2=%s is at r2=(%6.2e,%6.2e,%6.2e) x2=(%6.2e,%6.2e,%6.2e) i2=(%i,%i,%i)\n"
		 "   dist=%8.2e,  dist(x-x2)/(local grid2 spacing)=%8.2e\n",
		 (const char*)g2.getName(),r2(0,0),r2(0,1),r2(0,2),  x2(0,0),x2(0,1),x2(0,2), 
		 iv2[0],iv2[1],iv2[2], dist, dist/dx2);
	}
	
      
	if( numberOfDimensions==3 && max(fabs(r2-.5))<.6 )
	{
          // **** if the grid2 inverse failed, but we are close to grid2, we double check the default
          // **** inverse by applying a damped Newton method 


	  if( infoLevel & 1 ) printF("This inverse for this point failed to converge but it is close\n");
	  // Look for a closer point
	  const realArray & center2 = g2.center();

	  RealArray xx(1,3),dx(3),dr(1,3),rr(1,3);
	  RealArray rx(1,3,3);
	  rr=r2;
	      
	  // xx=current guess -- starting from nearest grid point 
	  xx=x2;

	  real drMax;
	  for( int step=0; step<50; step++ )
	  {
	    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	      dx(axis)=xx(0,axis)-x0(0,axis);

	    map2.inverseMapS(xx,r2,rx); // compute rx

	    real det=
	      (rx(0,axis1,axis2)*rx(0,axis2,axis3)-rx(0,axis2,axis2)*rx(0,axis1,axis3))*rx(0,axis3,axis1)+
	      (rx(0,axis1,axis3)*rx(0,axis2,axis1)-rx(0,axis2,axis3)*rx(0,axis1,axis1))*rx(0,axis3,axis2)+
	      (rx(0,axis1,axis1)*rx(0,axis2,axis2)-rx(0,axis2,axis1)*rx(0,axis1,axis2))*rx(0,axis3,axis3);

	    drMax=0.; 
	    real drMaxRelative=0.;
	    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	    {
	      dr(0,axis)=rx(0,axis,0)*dx(0)+rx(0,axis,1)*dx(1)+rx(0,axis,2)*dx(2);
	      drMaxRelative=max(drMaxRelative,fabs(dr(0,axis)/g2.gridSpacing(axis)));
	    }
	    drMax=max(fabs(dr));
	    real damping=1.;
	    if( drMaxRelative>.5 )
	    {
	      damping=.5/drMaxRelative;
	    }
	    real dist=max(fabs(dx));
	    real rxMax=max(fabs(rx));
		
	    if( infoLevel & 4 )
	      printF("My Newton it=%i: xx=(%9.3e,%9.3e,%9.3e) rr=(%7.1e,%7.1e,%7.1e) dr=(%7.1e,%7.1e,%7.1e) \n"
		     "         dx=(%7.1e,%7.1e,%7.1e) drMax=%7.1e, "
		     "drMaxRelative=%7.1e damping=%7.1e, rxMax=%7.1e det=%8.2e |x-xx|=%8.2e\n",
		     step,xx(0,0),xx(0,1),xx(0,2),rr(0,0),rr(0,1),rr(0,2),dr(0,0),dr(0,1),dr(0,2),
		     dx(0),dx(1),dx(2), drMax,drMaxRelative,damping,rxMax,det, dist);

	    if( drMax<REAL_EPSILON*500. )
	      break;
		
	    rr-=dr*damping;

	    map2.mapS(rr,xx);
		
	  }
	  if(  drMax<REAL_EPSILON*500. )
	  {
	    printF("My Newton converged: xx=(%9.3e,%9.3e,%9.3e) rr=(%7.1e,%7.1e,%7.1e) dr=(%7.1e,%7.1e,%7.1e) \n",
		   xx(0,0),xx(0,1),xx(0,2),rr(0,0),rr(0,1),rr(0,2),dr(0,0),dr(0,1),dr(0,2));
	  }
	  else
	  {
	    printF("My Newton diverged: xx=(%9.3e,%9.3e,%9.3e) rr=(%7.1e,%7.1e,%7.1e) dr=(%7.1e,%7.1e,%7.1e) \n",
		   xx(0,0),xx(0,1),xx(0,2),rr(0,0),rr(0,1),rr(0,2),dr(0,0),dr(0,1),dr(0,2));
	  }
	      
	      
	  r=rr;
	}
      }
      
      if( checkBoundaryPoint && numberOfDimensions==3 )
      {
        // *** compare the normals at the boundary points *******
        // *** the normals on shared sides should be in wroughly the same direction ****

        #ifdef USE_PPP
    	  printF("interpolateAPoint: check normals -- WARNING: finish me for parallel\n");
          continue;
        #endif

	int ks1=0, kd1=2;  
	int ks2=0, kd2=2;
	if( sidec>=0 )
	{
	  ks1=sidec, kd1=axisc;
	}
	      
	RealArray r2p(1,3), x2p(1,3);
	int ivb[3];
	for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
	{
	  iv2[dir]=int(r(0,dir)/g2.gridSpacing(dir)+.5)+g2.gridIndexRange(0,dir);
	  x2p(0,dir)=center(i1,i2,i3,dir);
	}
	      
	findClosestBoundaryPoint( g2, &x2p(0,0), iv2, ivb, ks2,kd2 );
	if( ks2<0 )
	{
	  ks2=0, kd2=2;  // set these if not found
	}

	// project the point onto the boundary of grid2
	r2p=r;
	r2p(0,kd2)=real(ks2);
	map2.mapS(r2p,x2p);
	iv2[kd2]=g2.gridIndexRange(ks2,kd2);
// 	      for( axis=0; axis<cg.numberOfDimensions(); axis++ )
// 		iv2[axis]=int(r2p(0,axis)/g2.gridSpacing(axis)+.5)+g2.gridIndexRange(0,axis);
	      
	printF(" Closest point on boundary is (%i,%i,%i) x2p=(%9.3e,%9.3e,%9.3e) face=(%i,%i)\n",
	       iv2[0],iv2[1],iv2[2],x2p(0,0),x2p(0,1),x2p(0,2),ks2,kd2);
	      
//        findClosestBoundaryPoint( g2, &x2p(0,0), iv, ivb, sidec, axisc );

	realArray & normal  = g.vertexBoundaryNormal(ks1,kd1);
	realArray & normal2 = g2.vertexBoundaryNormal(ks2,kd2);
	      

	RealArray n1(3), n2(3);
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  n1(axis)=normal(iv[0],iv[1],iv[2],axis);
	  n2(axis)=normal2(iv2[0],iv2[1],iv2[2],axis);
	}
	real nDot=n1(0)*n2(0)+n1(1)*n2(1)+n1(2)*n2(2);
	printF(" normal1=(%8.2e,%8.2e,%8.2e) normal2=(%8.2e,%8.2e,%8.2e).",
	       n1(0),n1(1),n1(2),n2(0),n2(1),n2(2));
	printF(" dot product of normals = %8.2e\n",nDot);
   
      }

    }
    else
    {
      if( infoLevel & 2 ) 
         printF("interpolateAPoint: Grid %s does not intersect grid %s\n",(const char*)g.getName(),
             (const char*)g2.getName());
    }

  }  // end for gg 
  if( infoLevel & 1 )
    printF("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  
  return wasInterpolated;
}

