#include "Overture.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "Ogen.h"

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

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )

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

static bool 
canDiscretize2( MappedGrid & g, const int iv[3], bool checkOneSidedAtBoundaries =true )
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
 
    printF("canDiscretize2: iab=[%i,%i][%i,%i][%i,%i]\n",
	   iab(0,0),iab(1,0),iab(0,1),iab(1,1),iab(0,2),iab(1,2));
    
  const int j1b=iab(1,axis1), j2b=iab(1,axis2), j3b=iab(1,axis3);
  for( int j3=iab(0,axis3); j3<=j3b; j3++ )
    for( int j2=iab(0,axis2); j2<=j2b; j2++ )
      for( int j1=iab(0,axis1); j1<=j1b; j1++ )
      {
	printF("mask(%i,%i,%i)=%i\n",j1,j2,j3,mask(j1,j2,j3));
        if( !(mask(j1,j2,j3) & MappedGrid::ISusedPoint) )
	{
	  return false;
	}
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


static bool 
myCanDiscretize( MappedGrid & g, const int iv[3], bool checkOneSidedAtBoundaries /* =true */ )
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

  //if( true )
  //  return true; // ***wdh* 2012/06/17
  


//
//  Check that all points in the discretization stencil are either
//  discretization points or interpolation points.
//
    // intArray & maskgd = g.mask();
    const intArray & maskgd = g.mask();

//   if( true )
//     return true; // ***wdh* 2012/06/17

    // *wdh* 2012/06/17 GET_LOCAL(int,maskgd,maskg);
    OV_GET_SERIAL_ARRAY_CONST(int,maskgd,maskg);

  if( true )
    return true; // ***wdh* 2012/06/17

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



int 
checkOverlappingGrid( const CompositeGrid & cg, const int & option /* =0 */, bool onlyCheckBaseGrids /* =true */ )
// ===================================================================================
// Check the validity of the Overlapping Grid
//
//   o check interpolation points
//     check for valid explicit or implicit interpolation
//
// /option (input) : a bit flag to turn out various options and outputs:
//       2 : check that discretization points are valid 
//       4 : print a list of interpolation points and where they interpolate from
//       8 : double check that interpolation points can interpolate (for parallel).
//      64 : print the mask
// /return value: 0 if no errors where found, otherwise the number of errors found.
//
// /onlyCheckBaseGrids (input) : if true only check interpolation points on base grids. If false, also
//   check interpolation points on AMR grids.
// ===================================================================================
{
  int ok=0;
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
//   #ifdef USE_PPP
//       // Copy the grid and all interpolation data to a single processor:
//     CompositeGrid cg;
// //     ParallelGridUtility::redistribute( cg_, cg, Range(0,0) );
//   #else
//     const CompositeGrid & cg = cg_;
//   #endif
  
  int iv[3]={0,0,0}, &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int iiv[3]={0,0,0};  // holds lower corner of interpolation stencil
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  
  const int ISneededPoint = CompositeGrid::ISreservedBit2;
  
  const int numberOfGrids = onlyCheckBaseGrids ? cg.numberOfBaseGrids() : cg.numberOfComponentGrids();

  realSerialArray rr(1,3); rr=0.;
  intSerialArray interpolates(1), useBackupRules(1);
  bool checkForOneSided=TRUE;
  useBackupRules=FALSE;

  const bool doubleCheckInterpolation = option & 8;
  if( doubleCheckInterpolation )
  {
    printF("Ogen:Parallel version: double check the interpolation...\n");
  }
      

  const int numberOfDimensions = cg.numberOfDimensions();
  RealArray epsilonForInterpolation(numberOfDimensions,numberOfGrids);
  Range all;

  for( int l=0; l<cg.numberOfMultigridLevels(); l++ ) // multigrid level
  {
    const CompositeGrid & cgl = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];
    
    // compute the allowable tolerance for interpolating near the boundary of a grid:
    
    for( int grid=0; grid<numberOfGrids; grid++ )
    {
      const RealArray & sharedBoundaryTolerance= cg[grid].sharedBoundaryTolerance();
      for( int axis=0; axis<numberOfDimensions; axis++ )
	epsilonForInterpolation(axis,grid)=max(cg.epsilon()*2.1/cg[grid].gridSpacing(axis),
					       max(sharedBoundaryTolerance(all,axis)));
    }
    

    for( int grid=0; grid<numberOfGrids; grid++ )
    {
      MappedGrid & mg = (MappedGrid&)cgl[grid]; // cast away const
      const intArray & maskgd = mg.mask();
      GET_LOCAL_CONST(int,maskgd,maskg);

      const intSerialArray & discretizationWidth= mg.discretizationWidth();

      if( option & 64 )
	mg.mask().display("checkOverlappingGrid: Here is the mask");
      // printf("CompositeGrid::ISusedPoint=%i, CompositeGrid::ISdiscretizationPoint=%i \n",
      //	   CompositeGrid::ISusedPoint,CompositeGrid::ISdiscretizationPoint);
    
      // *wdh* 070828 -- check that points on interpolation boundaries are unused or interpolation points
      const IntegerArray & eir = mg.extendedIndexRange(); // extendedGridIndexRange(mg);
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition(side,axis)==0 )
	{
	  getBoundaryIndex(eir,side,axis,I1,I2,I3);
	  // the number of interpolation layers is the half the discrization width
          const int numInterpLayers = (discretizationWidth(axis)-1)/2;
          int ia=eir(side,axis);
	  int ib = ia + (numInterpLayers -1 )*(1-2*side);
          Iv[axis] = side==0 ? Range(ia,ib) : Range(ib,ia);

	  bool hasPoints=ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);  
	  if( !hasPoints ) continue; 

	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( maskg(i1,i2,i3)>0 )
	    {
              printf("checkOverlappingGrid:ERROR: grid=%i, pt (i1,i2,i3)=(%i,%i,%i) on the interpolation "
                     "boundary (side,axis)=(%i,%i) is NOT valid, mask>0 !\n",grid,i1,i2,i3,side,axis);
	      ok++;
	      
	    }
	  }
	}
      }
      
      //if( true ) // *********** 2012/06/17
      //  continue;


      #ifdef USE_PPP
       const intSerialArray & numberOfInterpolationPoints = cgl->numberOfInterpolationPointsLocal;
       const intSerialArray & ip = cgl->interpolationPointLocal[grid];
       const intSerialArray & il = cgl->interpoleeLocationLocal[grid];
       const intSerialArray & ig = cgl->interpoleeGridLocal[grid];
       const realSerialArray & ci = cgl->interpolationCoordinatesLocal[grid];
       const intSerialArray & viw = cgl->variableInterpolationWidthLocal[grid];
      #else
       const intSerialArray & numberOfInterpolationPoints = cgl.numberOfInterpolationPoints;
       const intSerialArray & ip = cgl.interpolationPoint[grid];
       const intSerialArray & il = cgl.interpoleeLocation[grid];
       const intSerialArray & ig = cgl.interpoleeGrid[grid];
       const realSerialArray & ci = cgl.interpolationCoordinates[grid];
       const intSerialArray & viw = cgl.variableInterpolationWidth[grid];
      #endif

       if( false )
       {
	 for( int grid=0; grid<cgl.numberOfComponentGrids(); grid++ )
	 {
	   printF("checkOG: level=%i grid=%i niLocal=%i ci=[%i,%i]\n",l,grid,
		  numberOfInterpolationPoints(grid),ci.getBase(0),ci.getBound(0));
	 }
       }
       

      iv[2]=0;
      for( int i=0; i<numberOfInterpolationPoints(grid); i++ )
      {
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  iv[axis]=ip(i,axis);
	  iiv[axis]=il(i,axis);
	}
	// here is the donor grid:
        
	int gridi = ig(i);
	assert( gridi>=0 && gridi<numberOfGrids );
	
	MappedGrid & gi = (MappedGrid&)cgl[gridi];
	const intArray & maskd = gi.mask();
    
	GET_LOCAL_CONST(int,maskd,mask);
	

        if( iv[0]<maskg.getBase(0) || iv[0]>maskg.getBound(0) ||
            iv[1]<maskg.getBase(1) || iv[1]>maskg.getBound(1) )
	{
          printf("checkOverlappingGrid:ERROR: myid=%i, l=%i, grid=%i, interp. pt %i iv=(%i,%i,%i) "
                 "is outside local mask=[%i,%i][%i,%i]\n",myid,l,grid,i,iv[0],iv[1],iv[2],
		 maskg.getBase(0),maskg.getBound(0),maskg.getBase(1),maskg.getBound(1));
	  OV_ABORT("error");
	}
	

	if( !(maskg(iv[0],iv[1],iv[2]) & CompositeGrid::ISinterpolationPoint) )
	{
	  printf(" checkOverlappingGrid: ERROR mask wrong at an interpolation point\n");
	  printf(" -> grid=%i, donor=%i, interp pt i=%i iv=[%i,%i,%i] mask=%i\n",
		 grid,gridi,i,iv[0],iv[1],iv[2],maskg(iv[0],iv[1],iv[2]));
	  
	  ok++;
	}

	// width(Range(0,2))=cgl.interpolationWidth(Range(0,2),grid,gridi);  
	const int width = viw(i);

	if( option & 4 )
	{
	  printf("grid=%i, i=%i, p=(%i,%i,%i) from (%i,%i,%i) on grid %i, width=%i\n",
		 grid,i,iv[0],iv[1],iv[2],iiv[0],iiv[1],iiv[2],gridi,width);
	}
	

	for( int axis=axis1; axis<numberOfDimensions; axis++ ) 
	{
	  real rsb=ci(i,axis)/gi.gridSpacing(axis)+gi.indexRange(Start,axis);
	  real px= gi.isCellCentered(axis)  ? rsb-iiv[axis]-.5 : rsb-iiv[axis];

	  if( fabs( px - 0.5*( width-1)) >  0.5 + REAL_EPSILON*10. )
	  {
	    // Interpolation weight is too large, this could be an error
	    //..............okay if extrapolating near a boundary
	    //..............okay if we are exactly on the end points. (for refinement grids, 981030)
	    real pxmax=gi.isCellCentered(axis)  ? .5 : 0.; 
//	    const real eps=REAL_EPSILON*max(500.,100./gi.gridSpacing(axis));
//	    const real eps=cg.epsilon()*2.1/gi.gridSpacing(axis);
	    const real eps=epsilonForInterpolation(axis,gridi);
	    if( ! (
		  ( ( il(i,axis) < gi.indexRange(Start,axis)+width ) ||
		    ( il(i,axis) > gi.indexRange(End  ,axis)-width )  
		    ) &&
		  (px >= -eps-pxmax && px <= width-1.+eps+pxmax ) 
		  ) 
		&& fabs(px)>REAL_EPSILON*10. && fabs(px-width+1.)>REAL_EPSILON*10. )
	    {
	      ok++;
	      printf( " checkOG:ERROR: Invalid interp pt (%i,%i,%i) grid %i (%s) from (%i,%i,%i) grid %i (%s)\n"
		      "    r=(%6.2e,%6.2e,%6.2e)  interp weight =%7.3e (not in [%7.1e,%7.1e]) (eps=%6.3e) width=%i\n",
		      iv[0],iv[1],iv[2],grid,(const char*)mg.getName(),
		      il(i,axis1),il(i,axis2),(numberOfDimensions==3 ? il(i,axis3) : 0),
		      gridi,(const char*)gi.getName(),
		      ci(i,axis1),ci(i,axis2),(numberOfDimensions==3 ? ci(i,axis3) : 0.),
		      px,0.5*( width-1)-.5,0.5*( width-1)+.5,eps,width);
	    }
	  }
	}
	const int validMask = cgl.interpolationIsImplicit(grid,gridi) ? CompositeGrid::ISusedPoint
	  : CompositeGrid::ISdiscretizationPoint;
	bool pointOk=TRUE;
	const int width2 = numberOfDimensions>1 ? width : 1;
	const int width3 = numberOfDimensions>2 ? width : 1;

#ifndef USE_PPP	
        // ---- serial check ----
	for( int w3=0; w3<width3 && pointOk; w3++ )    
	{
	  for( int w2=0; w2<width2 && pointOk; w2++ )    
	  {
	    for( int w1=0; w1<width; w1++ )    
	    {
	      const int & m = mask(iiv[0]+w1,iiv[1]+w2,iiv[2]+w3);
	      if( !(m & validMask) )
	      {
		pointOk=FALSE;
		printf("checkOverlappingGrid:ERROR: Some of the donor points cannot be used for interpolation\n");
		printf(" grid=%s (%i), point %i = (%i,%i,%i), donor grid=%s (%i), donor=(%i,%i,%i), "
		       "offset=(%i,%i,%i), width=%i, mask=%i, "
		       "isImplicit=%i, \n mask&discretization=%i, mask&interpolation=%i mask&ghost=%i"
                       "mask&USESbackupRules=%i mask&ISinteriorBoundaryPoint=%i mask&ISneededPoint=%i\n",
		       (const char*)mg.getName(),grid,
		       i,iv[0],iv[1],iv[2],
		       (const char*)gi.getName(),gridi,
		       iiv[0]+w1,iiv[1]+w2,iiv[2]+w3,w1,w2,w3,width,m,
		       cgl.interpolationIsImplicit(grid,gridi),m & CompositeGrid::ISdiscretizationPoint,
		       m & CompositeGrid::ISinterpolationPoint, m & CompositeGrid::ISghostPoint,
                       m & CompositeGrid::USESbackupRules, m &CompositeGrid::ISinteriorBoundaryPoint,
                       m & ISneededPoint );

		if( true )
		{
		  printf("Here are the bits of the mask[31...0]=");
		  for( int b=31; b>=0; b-- )
		    printf("%i",m & (1<<b) ? 1 : 0);
		  printf("\n");
		}
		
		for( int axis=axis1; axis<numberOfDimensions; axis++ ) 
		  rr(0,axis)=ci(i,axis);
		interpolates=TRUE;  useBackupRules=FALSE;
		cg.rcData->canInterpolate(cg.gridNumber(grid),cg.gridNumber(gridi), rr, interpolates,
					  useBackupRules, checkForOneSided );
 		printf(" canInterpolate: interpolate=%i, ISdiscretizationPoint=%i "
 		       "ISinterpolationPoint=%i\n",interpolates(0),
 		       MappedGrid::ISdiscretizationPoint,CompositeGrid::ISinterpolationPoint);


// #ifndef USE_PPP
// 		cg.rcData->canInterpolate(grid,gridi, rr, interpolates, useBackupRules, checkForOneSided );
// #endif

		ok++;
		break;
	      }
	    }
	  }
	}
#endif
      } // end for i 

      // if( true ) // *********** 2012/06/17
      //  continue;


#ifdef USE_PPP
      
      // --- parallel version : double check interpolation ---
      if( doubleCheckInterpolation )
      {
	IntegerArray interpolates, useBackupRules;
	RealArray rr;
	// *wdh* 091212 const IntegerArray & ise = cg.interpolationStartEndIndex;
	const IntegerArray & ise = cgl.interpolationStartEndIndex;
	for( int gridi=0; gridi<numberOfGrids; gridi++ )
	{
	  if( grid==gridi ) continue;
	
	  MappedGrid & gi = (MappedGrid&)cgl[gridi];
	
	  // printf("checkOverlappingGrid: myid=%i grid=%i gridi=%i interpolationStartEndIndex()=%i %i \n",
	  //        myid,grid,gridi,
	  //        ise(0,grid,gridi),
	  //        ise(1,grid,gridi));
	  const int numi = ise(0,grid,gridi)>=0 ? ise(1,grid,gridi)-ise(0,grid,gridi)+1 : 0;

	  if( numi>0 )
	  {
	    Range R=numi;
	    interpolates.redim(R);  interpolates=false;
	    useBackupRules.redim(R); useBackupRules=true;
	    rr.redim(R,numberOfDimensions); 
	    Range Ri=Range(ise(0,grid,gridi),ise(1,grid,gridi));
	    Range Rx=numberOfDimensions;
	    rr=ci(Ri,Rx);
	  }
	  else
	  {
	    interpolates.redim(0);
	    useBackupRules.redim(0);
	    rr.redim(0);
	  }

	  // 
	  // Ogen::checkCanInterpolate((CompositeGrid&)cg,cg.gridNumber(grid),cg.gridNumber(gridi), rr, interpolates,
	  //			  useBackupRules);
	  // *wdh* 091212 (for Ogmg::buildExtraLevelsNew) -- use cgl 
	  Ogen::checkCanInterpolate((CompositeGrid&)cgl,grid,gridi, rr, interpolates,useBackupRules);

	  for( int j=0; j<numi; j++ )
	  {
	    if( !interpolates(j) )
	    {
	      const int i=ise(0,grid,gridi)+j;
	      for( int axis=0; axis<numberOfDimensions; axis++ )
	      {
		iv[axis]=ip(i,axis);
		iiv[axis]=il(i,axis);
	      }
	      const int width = viw(i);
	      printf("checkOverlappingGrid:ERROR: Unable to interpolate a point, myid=%i\n",myid);
	      printf(" grid=%s (%i), j=%i point %i = (%i,%i,%i), donor grid=%s (%i), donor=(%i,%i,%i),"
		     " width=%i, isImplicit=%i, ci=(%5.3f,%5.3f,%5.3f).\n",
		     (const char*)mg.getName(),grid,
		     j,i,iv[0],iv[1],iv[2],
		     (const char*)gi.getName(),gridi,iiv[0],iiv[1],iiv[2],
		     width,cgl.interpolationIsImplicit(grid,gridi),
		     rr(j,0),rr(j,1),(numberOfDimensions==2 ? 0. : rr(j,2)));
	      ok++;
	    }
	    else
	    {
	      if( false )
	      {
		const int i=ise(0,grid,gridi)+j;
		printf(" checkOverlappingGrid: myid=%i, i=%i grid=%i gridi=%i interpolates=%i\n",myid,i,
		       grid,gridi,interpolates(j));
	      }
	    }
	  }
	}
      }
      
#endif
      if( option & 2 )
      {
        // --- check that discretization points are valid --

	getIndex(mg.gridIndexRange(),I1,I2,I3);
	bool hasPoints=ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);  
	if( hasPoints ) 
	{
          // NOTE: it is very slow to check every pt so only check most likely problem pts.

          Range Rx=numberOfDimensions;
          // const int dw = max(discretizationWidth(Rx));
          const int dw = discretizationWidth(0);
	  const int hw=(dw-1)/2;

          int * maskp = maskg.Array_Descriptor.Array_View_Pointer2;
          const int maskDim0=maskg.getRawDataSize(0);
          const int maskDim1=maskg.getRawDataSize(1);
          #undef MASK
          #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

#define MASK3A(i1,i2,i3) \
  MASK(i1-1,i2,i3)==0 || MASK(i1,i2,i3)==0 || MASK(i1+1,i2,i3)==0 
#define MASK3B(i1,i2,i3) \
  MASK3A(i1,i2-1,i3)==0 || MASK3A(i1,i2,i3)==0 || MASK3A(i1,i2+1,i3)==0 
#define MASK3C(i1,i2,i3) \
 MASK3B(i1,i2,i3-1)==0 || MASK3B(i1,i2,i3)==0 || MASK3B(i1,i2,i3+1)==0

#define MASK5A(i1,i2,i3) \
 MASK(i1-2,i2,i3)==0 || MASK(i1-1,i2,i3)==0 || MASK(i1,i2,i3)==0 || MASK(i1+1,i2,i3)==0 || MASK(i1+2,i2,i3)==0
#define MASK5B(i1,i2,i3) \
 MASK5A(i1,i2-2,i3)==0 || MASK5A(i1,i2-1,i3)==0 || MASK5A(i1,i2,i3)==0 || MASK5A(i1,i2+1,i3)==0 || MASK5A(i1,i2+2,i3)==0
#define MASK5C(i1,i2,i3) \
 MASK5B(i1,i2,i3-2)==0 || MASK5B(i1,i2,i3-1)==0 || MASK5B(i1,i2,i3)==0 || MASK5B(i1,i2,i3+1)==0 || MASK5B(i1,i2,i3+2)==0

#define MASK7A(i1,i2,i3) \
 MASK(i1-3,i2,i3)==0 || MASK(i1-2,i2,i3)==0 || MASK(i1-1,i2,i3)==0 || MASK(i1,i2,i3)==0 || MASK(i1+1,i2,i3)==0 || MASK(i1+2,i2,i3)==0 || MASK(i1+3,i2,i3)==0
#define MASK7B(i1,i2,i3) \
 MASK7A(i1,i2-3,i3)==0 || MASK7A(i1,i2-2,i3)==0 || MASK7A(i1,i2-1,i3)==0 || MASK7A(i1,i2,i3)==0 || MASK7A(i1,i2+1,i3)==0 || MASK7A(i1,i2+2,i3)==0 || MASK7A(i1,i2+3,i3)==0
#define MASK7C(i1,i2,i3) \
 MASK7B(i1,i2,i3-3)==0 || MASK7B(i1,i2,i3-2)==0 || MASK7B(i1,i2,i3-1)==0 || MASK7B(i1,i2,i3)==0 || MASK7B(i1,i2,i3+1)==0 || MASK7B(i1,i2,i3+2)==0 || MASK7B(i1,i2,i3+3)==0

#define MASK9A(i1,i2,i3) \
 MASK(i1-4,i2,i3)==0 || MASK(i1-3,i2,i3)==0 || MASK(i1-2,i2,i3)==0 || MASK(i1-1,i2,i3)==0 || MASK(i1,i2,i3)==0 || MASK(i1+1,i2,i3)==0 || MASK(i1+2,i2,i3)==0 || MASK(i1+3,i2,i3)==0 || MASK(i1+4,i2,i3)==0
#define MASK9B(i1,i2,i3) \
 MASK9A(i1,i2-4,i3)==0 || MASK9A(i1,i2-3,i3)==0 || MASK9A(i1,i2-2,i3)==0 || MASK9A(i1,i2-1,i3)==0 || MASK9A(i1,i2,i3)==0 || MASK9A(i1,i2+1,i3)==0 || MASK9A(i1,i2+2,i3)==0 || MASK9A(i1,i2+3,i3)==0 || MASK9A(i1,i2+4,i3)==0
#define MASK9C(i1,i2,i3) \
 MASK9B(i1,i2,i3-4)==0 || MASK9B(i1,i2,i3-3)==0 || MASK9B(i1,i2,i3-2)==0 || MASK9B(i1,i2,i3-1)==0 || MASK9B(i1,i2,i3)==0 || MASK9B(i1,i2,i3+1)==0 || MASK9B(i1,i2,i3+2)==0 || MASK9B(i1,i2,i3+3)==0 || MASK9B(i1,i2,i3+4)==0

	  if( numberOfDimensions==2 )
	  {
	    if( dw==3 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		// if( MASK(i1,i2,i3)>0 && 
		//     (MASK(i1-1,i2-1,i3)<=0 || MASK(i1  ,i2-1,i3)<=0 || MASK(i1+1,i2-1,i3)<=0 ||
		//      MASK(i1-1,i2  ,i3)<=0 ||                           MASK(i1+1,i2  ,i3)<=0 ||
		//      MASK(i1-1,i2+1,i3)<=0 || MASK(i1  ,i2+1,i3)<=0 || MASK(i1+1,i2+1,i3)<=0 ) )
		if( MASK(i1,i2,i3)>0 && ( MASK3B(i1,i2,i3) ) )
		{
		  // if( !myCanDiscretize(mg,iv,true) )
		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	    }
	    else if( dw==5 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 && ( MASK5B(i1,i2,i3) ) )
		{
		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	    }
	    else if( dw==7 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 && ( MASK7B(i1,i2,i3) ) )
		{
		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	    }
	    else if( dw==9 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 && ( MASK9B(i1,i2,i3) ) )
		{
		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	    }
	    else 
	    { // general case for arbitrary DW
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 )
		{
		  bool ok=true;
		  for( int j2=i2-hw; j2<=i2+hw && ok; j2++ )
		  {
		    for( int j1=i1-hw; j1<=i1+hw; j1++ )
		    {
		      if( MASK(j1,j2,i3)==0 )
		      {
			ok=false;
			break;
		      }
		    }
		  }
		  if( !ok )
		  {

		    if( !Ogen::canDiscretize(mg,iv) )
		    {
		      printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			     "but point can NOT be discretized\n",grid,i1,i2,i3);
		      // ok++;
		    }
		  }
		}
	      }
	    }
	    
	  }
	  else // ========== Three Dimensions ===============
	  {
	    if( dw==3 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
// 		if( MASK(i1,i2,i3)>0 && 
// 		    (MASK(i1-1,i2-1,i3-1)<=0 || MASK(i1  ,i2-1,i3-1)<=0 || MASK(i1+1,i2-1,i3-1)<=0 ||
// 		     MASK(i1-1,i2  ,i3-1)<=0 || MASK(i1  ,i2  ,i3-1)<=0 || MASK(i1+1,i2  ,i3-1)<=0 ||
// 		     MASK(i1-1,i2+1,i3-1)<=0 || MASK(i1  ,i2+1,i3-1)<=0 || MASK(i1+1,i2+1,i3-1)<=0 || 
// 		     MASK(i1-1,i2-1,i3  )<=0 || MASK(i1  ,i2-1,i3  )<=0 || MASK(i1+1,i2-1,i3  )<=0 ||
// 		     MASK(i1-1,i2  ,i3  )<=0 ||                             MASK(i1+1,i2  ,i3  )<=0 ||
// 		     MASK(i1-1,i2+1,i3  )<=0 || MASK(i1  ,i2+1,i3  )<=0 || MASK(i1+1,i2+1,i3  )<=0 || 
// 		     MASK(i1-1,i2-1,i3+1)<=0 || MASK(i1  ,i2-1,i3+1)<=0 || MASK(i1+1,i2-1,i3+1)<=0 ||
// 		     MASK(i1-1,i2  ,i3+1)<=0 || MASK(i1  ,i2  ,i3+1)<=0 || MASK(i1+1,i2  ,i3+1)<=0 ||
// 		     MASK(i1-1,i2+1,i3+1)<=0 || MASK(i1  ,i2+1,i3+1)<=0 || MASK(i1+1,i2+1,i3+1)<=0 ) )
                if( MASK(i1,i2,i3)>0 && ( MASK3C(i1,i2,i3) ) )
		{
		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	    }
	    else if( dw==5 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 && ( MASK5C(i1,i2,i3) ) )
		{
// 		  if( grid==1 && i1==3 && i2==10 && i3==1 )
// 		  {
// 		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
// 			   "but point can NOT be discretized\n",grid,i1,i2,i3);
//                     bool ok = canDiscretize2(mg,iv);
//                     printf(" canDiscretize = %i\n",(int)ok);
//                     printf(" discretizationWidth= [%i,%i,%i]\n",mg.discretizationWidth(0),mg.discretizationWidth(1),
// 			   mg.discretizationWidth(2));
// 		    ::display(mg.boundaryDiscretizationWidth(),"g.boundaryDiscretizationWidth()");

// 		    printf(" Here is the mask surrounding this point (grid,i1,i2,i3)=(%i,%i,%i,%i)\n", grid,i1,i2,i3);
// 		    printf("   mask: 1=interior, 2=ghost, -2,3=interiorBoundaryPoint,  <0 =interp \n");
// 		    for( int j3=i3-hw; j3<=i3+hw; j3++ )
// 		    {
// 		      if( j3<mg.dimension(0,2) || j3>mg.dimension(1,2) )
// 			continue;
// 		      for( int j2=i2-hw; j2<=i2+hw; j2++ )
// 		      {
// 			if( j2<mg.dimension(0,1) || j2>mg.dimension(1,1) )
// 			  continue;
// 			printf("   mask(%i:%i,%i,%i) =",i1-hw,i1+hw,j2,j3);

// 			for( int j1=i1-hw; j1<=i1+hw; j1++ )
// 			{
// 			  if( j1>=mg.dimension(0,0) && j1<=mg.dimension(1,0) )
// 			  {
// 			    printf(" %i ",MASK(j1,j2,j3));
// 			  }
// 			}
// 			printf("\n");
// 		      }
// 		    }

// 		  }
		  

		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	    }
	    else if( dw==7 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 && ( MASK7C(i1,i2,i3) ) )
		{
		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	    }
	    else if( dw==9 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 && ( MASK9C(i1,i2,i3) ) )
		{
		  if( !Ogen::canDiscretize(mg,iv) )
		  {
		    printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			   "but point can NOT be discretized\n",grid,i1,i2,i3);
		    // ok++;
		  }
		}
	      }
	      
	    }
	    else 
	    { // general case for arbitrary DW
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 )
		{
		  bool ok=true;
		  for( int j3=i3-hw; j3<=i3+hw && ok; j3++ )
		  {
		    for( int j2=i2-hw; j2<=i2+hw && ok; j2++ )
		    {
		      for( int j1=i1-hw; j1<=i1+hw; j1++ )
		      {
			if( MASK(j1,j2,j3)==0 )
			{
			  ok=false;
			  break;
			}
		      }
		    }
		  }
		  
		  if( !ok )
		  {

		    if( !Ogen::canDiscretize(mg,iv) )
		    {
		      printf("checkOverlappingGrid:ERROR: grid=%i (i1,i2,i3)=(%i,%i,%i) has mask>0 "
			     "but point can NOT be discretized\n",grid,i1,i2,i3);
		      // ok++;
		    }
		  }
		}
	      }
	    }

	  }
	}
      }
      
    } // for for grid 


  } // end for multigrid levels
  

//   int fromProcessor=0;
//   broadCast(ok,fromProcessor);

  ParallelUtility::getSum(ok);
  
  return ok;
}
