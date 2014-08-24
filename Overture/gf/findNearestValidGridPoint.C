#include "Overture.h"
#include "display.h"
#include "MappingProjectionParameters.h"
#include "ParallelUtility.h"
#include "CanInterpolate.h"
#include "InterpolatePointsOnAGrid.h"


// =====================================================================================================
/// \brief Find the nearest valid grid point on a CompositeGrid to given points in space, x(i,.). 
///        A valid point is one with mask(i1,i2,i3)!=0 . 
/// \detail NOTE: This function currently assumes that the points x(i,.) could NOT interpolate and
///    so we are finding the best guess for points that are outside the grid.
///
/// \param cg (input) : CompositeGrid to check for closest valid point.
/// \param x (input) : x(i,0:r-1) (r=rangeDimension) list of points to check (in parallel, each processor
///               provides its own set of points).
/// \param il (output) : il(i,0:r) = (donor,i1,i2,i3) donor grid and index location of
///                      the closest valid point. 
/// \param ci (output) : ci(i,0:r-1) are the r-coordinates of the closest pt in the donor grid.
/// \return 0 for success. 
// =====================================================================================================
int InterpolatePointsOnAGrid::
findNearestValidGridPoint( CompositeGrid & cg, const RealArray & x, IntegerArray & il, RealArray & ci )
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();

  const int numberOfPointsToInterpolate=x.getLength(0);

  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int rangeDimension = numberOfDimensions;
  const int domainDimension = cg[0].domainDimension();
  Range Rx=rangeDimension;
  
  int debug=0; // 7;
  FILE *plogFile=NULL;

  if( debug>0 && plogFile==NULL )
  {
#ifdef USE_PPP
    plogFile = fopen(sPrintF("findNearestValidGridPointNP%i.p%i.log",np,myid),"w" ); 
#else
    plogFile = fopen("findNearestValidGridPoint.log","w" ); 
#endif
    
   fprintf(plogFile,
           " ********************************************************************************************* \n"
	   " ***************** findNearestValidGridPoint log file, myid=%i, NP=%i pts=%i ****************** \n"
	   " ********************************************************************************************* \n\n",
            myid,np,numberOfPointsToInterpolate);
  }



  const real *px = x.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=x.getRawDataSize(0);
  #undef X
  #define X(i0,i1) px[i0+xDim0*(i1)]  

  int *ilp = il.Array_Descriptor.Array_View_Pointer1;
  const int ilDim0=il.getRawDataSize(0);
  #define IL(i0,i1) ilp[i0+ilDim0*(i1)]

  real *cip = ci.Array_Descriptor.Array_View_Pointer1;
  const int ciDim0=ci.getRawDataSize(0);
  #define CI(i0,i1) cip[i0+ciDim0*(i1)]


  // ia : indirection array: 
  Range R = x.dimension(0);
  IntegerArray ia(R);
  int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]

  // dista(i) = the distance to the nearest grid point
  RealArray dista(R);  
  dista=REAL_MAX;

  real *pdista=dista.Array_Descriptor.Array_View_Pointer0;
#define DISTA(i0) pdista[i0]

  real timeCanInterpolate=0.;
  
  RealArray xa,ra,xc,da;
  IntegerArray ib;

  for( int grid=numberOfComponentGrids-1; grid>=0; grid-- )
  {
    
    const MappedGrid & mg = cg[grid];
    Mapping & map = mg.mapping().getMapping();


    // Make a list of points that may have a closest point on this mapping
    const RealArray & boundingBox = mg.boundingBox();
    const real *pbb = &boundingBox(0,0);
    #undef bb
#define bb(side,axis) pbb[(side)+2*(axis)]  
    
    int j=0;
    for( int i=0; i<numberOfPointsToInterpolate; i++ )
    {
      // Compute the square of the L2 distance to the bounding box (distance is zero if we are inside the box)
      //         |
      //         |
      //  x......|  box 
      //         |
      //         +----------
      //         .
      //         .
      ///   x.....


      real distToBox =0.;
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	real x0=X(i,axis);
	if( x0<bb(0,axis) ) distToBox+=SQR(x0-bb(0,axis));
	if( x0>bb(1,axis) ) distToBox+=SQR(x0-bb(1,axis));
      }
      if( distToBox<= DISTA(i) )
      { // there may be a closer point in this grid so check this point
	IA(j)=i;
	j++;
      }
    }
    
    const int numberToCheck=j;
    Range I=numberToCheck;

    // xa : holds x-coords of points to check
    xa.redim(I,rangeDimension);
    real *pxa = xa.Array_Descriptor.Array_View_Pointer1;
    const int xaDim0=xa.getRawDataSize(0);
    #undef XA
    #define XA(i0,i1) pxa[i0+xaDim0*(i1)] 

    // ra : holds r-coords of points to check
    ra.redim(I,domainDimension);
    real *pra = ra.Array_Descriptor.Array_View_Pointer1;
    const int raDim0=ra.getRawDataSize(0);
    #define RA(i0,i1) pra[i0+raDim0*(i1)] 

    // da : holds the current closest distance and later the new closest distance (if it really is closer)
    da.redim(I);
    real *pda=da.Array_Descriptor.Array_View_Pointer0;
    #define DA(i0) pda[i0]

    // --- xc : holds the x-coordinates of the closest points  ---
    xc.redim(I,rangeDimension);
    const real *pxc = xc.Array_Descriptor.Array_View_Pointer1;
    const int xcDim0=xc.getRawDataSize(0);
    #undef XC
    #define XC(i0,i1) pxc[i0+xcDim0*(i1)] 

    ra=-1.;
    for( int i=0; i<numberToCheck; i++ )
    {
      int iai=IA(i);
      for( int axis=0; axis<rangeDimension; axis++ )
	XA(i,axis)=X(iai,axis);
      DA(i)=DISTA(iai);
    }
    

    // Here is the new parallel find nearest grid point: 
    //  Finds a new closest point, xc(i,.), and new square-distance da(i) but only if it is closer than the current squared-distance da(i)
    map.findNearestGridPoint( xa, ra, da, xc );


//     // If this Mapping has a basic inverse (usually means an analytic formula for the inverse)
//     // then we can invert directly : 
//     if( map.getBasicInverseOption()==Mapping::canInvert )
//     {
//       map.basicInverseS( xa,ra );  // can use user supplied inverse

//       // we should double check that valid values for ra were returned since some basicInverse routines
//       // may not work for points that are far away from the mapping.

//       for( int i=0; i<numberToCheck; i++ )
//       {
// 	if( RA(i,0)==Mapping::bogus )
// 	{
// 	  printf("findNearestValidGridPoint:ERROR: basic inverse returned a bogus result!\n");
// 	  OV_ABORT("error");
// 	}

// 	for( int axis=0; axis<domainDimension; axis++ )
// 	{
// 	  // map ra ra(i,.) to closest point inside or on the unit square
// 	  RA(i,axis) = min(1.,max(0.,RA(i,axis)));
// 	}
//       }
//     }
//     else if( !map.usesDistributedInverse() )
//     {
//       // If the mapping does not use a distributed inverse (e.g. NurbsMapping) then
//       // we can find the nearest grid point using the serial function:

//       // Do not check the bounding box when finding the nearest grid point:
//       map.setParameter( MappingParameters::THEfindBestGuess,true );
	
//       map.approximateGlobalInverse->findNearestGridPoint(0,numberToCheck-1,xa,ra );

//       map.setParameter( MappingParameters::THEfindBestGuess,false );   // reset 

//     }
//     else
//     {
//       // Mapping uses a distributed inverse ...

//     #ifndef USE_PPP

//       // Do not check the bounding box when finding the nearest grid point:
//       map.setParameter( MappingParameters::THEfindBestGuess,true );
	
//       map.approximateGlobalInverse->findNearestGridPoint(0,numberToCheck-1,xa,ra );

//       map.setParameter( MappingParameters::THEfindBestGuess,false );   // reset 

//     #else
//       // ---- Parallel version: Mapping uses a distributed inverse ----


//       // finish me -- xc is assigned here and not below...

//       // watch out: dista may be changed by the next routine

//       // here is the new parallel find nearest grid point:
//       map.findNearestGridPoint( xa, ra, dista, xc );
      

//       OV_ABORT("findNearestValidGridPoint: map.usesDistributedInverse() : finish me");
//     #endif      
//     }
    

//     // We now have the r coordinates of the closest points on this grid: RA(i,.) 
//     // Make a sub-list of points whose distance is less than the current best distance 


//     // Todo: for Cartesian grids we can evaluate XC directly 
//     map.mapS( ra,xc );

    // ib : indirection array that holds subset of points that we should check the mask for 
    ib.redim(I);
    int *ibp = ib.Array_Descriptor.Array_View_Pointer0;
    #define IB(i0) ibp[i0]    

    j=0;
    for( int i=0; i<numberToCheck; i++ )
    {
//       real dist=0.;
//       for( int axis=0; axis<rangeDimension; axis++ )
//       {
// 	dist+= SQR(XA(i,axis)-XC(i,axis));
//       }
//      if( dist<DISTA(IA(i)) )

      if( RA(i,0)!=Mapping::bogus ) // this point must be closer... check the mask
      {
	IB(j)=i;   // IB(j) indexes the the IA, RA, XA, XC arrays
	j++;
      }
    }
    // -- Here is the number of points that we need to check the mask value for since they may be closer:
    const int numToCheckMask=j;

    // **** Now check the mask value for these points ***

    // NOTE: checkCanInterpolate will use cg.interpolationWidth and interpolationOverlap 
    //   We need to define a "grid-number" for the receptor grid. 
    const int donor = grid;
    const int receptor = 0; // Is this OK if donor=receptor ? 

    // temporarily change these for the canInterpolate function:
    const int widthOld = cg.interpolationWidth(0,grid,donor,0);
    const real ovOld =  cg.interpolationOverlap(0,grid,donor,0);
      
    // Can we be sure that cgCanInterpolate is consistent with getInterpolationStencil??
    // cgCanInterpolate:         ia=rr-ov,      ib=rr+ov+1  -> width = 2*ov+1 -> ov=(width-1)/2
    // getInterpolationStencil : ia=rr-.5*iw+1  ib=rr+.5*iw -> ov=.5*iw-1 = .5*(width-2)
    // Ogen: changeParameters: ov = .5*(iw-2) for implicit interp

    // for now we assume that the original point x(i,.) cannot interpolate so we just check the closest pt (ra,xc)
    int interpolationWidth=1;  

    real ov=max(0., (interpolationWidth-2.)/2. ) ; // Is this correct?
    cg.interpolationWidth(Rx,receptor,donor,0)=interpolationWidth;  // target interp. width
    cg.interpolationOverlap(Rx,receptor,donor,0)=ov;

    real timeb=getCPU();
    
    using namespace CanInterpolate;
    // Allocate space for queries: 
    CanInterpolateQueryData *cid = new CanInterpolateQueryData[max(1,numToCheckMask)];

    // fill in the query data:
    for( int n=0; n<numToCheckMask; n++ )
    {
      int i = IB(n); // index into IA, RA, XA, XC
      cid[n].id=n; cid[n].i=i; 
      cid[n].grid=receptor; 
      cid[n].donor=donor;  // NOTE: we could process multiple donor grids at once as an optimization
      for( int axis=0; axis<numberOfDimensions; axis++ )
	cid[n].rv[axis]=RA(i,axis);

    }
    // Allocate space for results
    CanInterpolateResultData *cir =new CanInterpolateResultData[max(1,numToCheckMask)];

    // new: this version is for serial and parallel:
    // this function will find any valid interpolation by default (i.e. backup results too)
    // this function also computes the interpolation stencil

    int numberOfValidGhostPoints=0;  // do this for now 
    const int numValidGhost = numberOfValidGhostPoints>0 ? numberOfValidGhostPoints : 0;
    CanInterpolate::canInterpolate( cg, numToCheckMask,cid, cir, numValidGhost );

    timeCanInterpolate+=getCPU()-timeb;

    // reset
    cg.interpolationWidth(Rx,receptor,donor,0)=widthOld;
    cg.interpolationOverlap(Rx,receptor,donor,0)=ovOld;

    if( debug & 4 )
    {
      if( numToCheckMask>0 )
      {
	fprintf(plogFile,"--- findNearestValidGridPoint: attempt to interp from donor grid=%i : \n",donor);
	for( int n=0; n<numToCheckMask; n++ )
	{
          int i=IB(n), iai=IA(i);
	  fprintf(plogFile,
                  " pt %i interpolates=%i width=%i from donor=%i il=(%i,%i,%i) r=(%8.2e,%8.2e,%8.2e),"
                  " xc=(%8.2e,%8.2e,%8.2e) (xc is closest pt to xa=(%8.2e,%8.2e,%8.2e))\n",
		  iai,int(cir[n].width>0),cir[n].width,donor,cir[n].il[0],cir[n].il[1],cir[n].il[2],
		  RA(i,0),RA(i,1),(numberOfDimensions==2 ? 0. : RA(i,2)),
		  XC(i,0),XC(i,1),(numberOfDimensions==2 ? 0. : XC(i,2)),
		  XA(i,0),XA(i,1),(numberOfDimensions==2 ? 0. : XA(i,2)));
	}
      }
    }
    
    // *************** Assign points that were valid *****************
    for( int n=0; n<numToCheckMask; n++ )
    {
      int i = IB(n);
      int width = cir[n].width;   // interpolation width (=0 if invalid)

      // if( width>STATUS(IA(i)) ) // Is this a better interpolation width ? 
      if( width>0 ) // Is this pt valid ?
      {
        // we could have saved this dist above:
// 	real dist=0.;
// 	for( int axis=0; axis<rangeDimension; axis++ )
// 	{
// 	  dist+= SQR(XA(i,axis)-XC(i,axis));
// 	}

        real dist = DA(i);  // check me **

	int iai=IA(i);
	real oldDist=DISTA(iai);
	DISTA(iai) = dist;  // here is the new closest distance 

        IL(iai,0)=grid;
	for( int axis=0; axis<domainDimension; axis++ )
	{
	  CI(iai,axis)=RA(i,axis);          // r-coordinates of closest pt
	  IL(iai,axis+1)=cir[n].il[axis];   // lower left corner of the interpolation stencil
	}
        if( debug & 2 )
	{ 
          fprintf(plogFile,"**** improved point found, pt=%i, donor=%i, new-dist=%8.2e (old-dist=%8.2e)****\n",
                  iai,grid,sqrt(dist),sqrt(oldDist));
	}
	
	// STATUS(IA(i)) = width;  // if staus==interpolationWidth then we have the best possible donor

      }
    } // end for( n )
    delete [] cid;
    delete [] cir;


  } // end for( grid )
  
  if( debug & 4 )
  {
    fprintf(plogFile,"--- findNearestValidGridPoint: FINAL results ---\n");
    for( int i=0; i<numberOfPointsToInterpolate; i++ )
    {
      fprintf(plogFile," pt %i x=(%8.2e,%8.2e,%8.2e) closest: donor=%i il=(%i,%i,%i) ci=(%8.2e,%8.2e,%8.2e)\n",
              i,
              X(i,0),X(i,1),(numberOfDimensions==2 ? 0. : X(i,2)),
	      IL(i,0),IL(i,1),IL(i,2),(numberOfDimensions==2 ? 0 : IL(i,3)),
              CI(i,0),CI(i,1),(numberOfDimensions==2 ? 0. : CI(i,2)));
    }
    fprintf(plogFile,"------------------------------------------------\n");
  }

  if( plogFile!=NULL )
  {
    fflush(plogFile);
    fclose(plogFile);
  }
  
  return 0;
}
