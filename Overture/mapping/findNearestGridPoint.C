#include "Overture.h"
#include "display.h"
#include "MappingProjectionParameters.h"
#include "ParallelUtility.h"
#include "CanInterpolate.h"
#include "SparseArray.h"

// =============================================================================================================
/// \brief Find the nearest grid point to each of a given set of points x(i,.)
///
/// \param x (input) : x(i,0:r-1) find the nearest grid point to each of this set of points.
/// \param r (intput/output) : r(i,0:d-1) r-coordinates of the nearest grid points (r(i,0:d-1)==Mapping::bogus
///     if this value of r was not set since the distance was larger than dista(i)). On input these can
///     can provide an initial guess.
/// \param dista (input/output) : dista(i) Only find closest points that are smaller than this "distance" 
///      (which is actually defined as the SQUARE of the l2 norm) for each point. 
///       On output this value is set to the new smaller distance if one was found. 
/// \param xa (output) : xa(i,0:r-1) x-coordinates of the nearest grid points (for those points set).
//==============================================================================================================
int Mapping::
findNearestGridPoint( RealArray & x, RealArray & r, RealArray & dista, RealArray & xa )
{
  Mapping & map = *this;

//   const int domainDimension=map.getDomainDimension();
//   const int rangeDimension=map.getRangeDimension();

  int debug = Mapping::debug; // 3;
  
  if( debug & 2 )
    printF("findNearestGridPoint for mapping %s\n",(const char*)map.getName(Mapping::mappingName));

  int base=x.getBase(0), bound=x.getBound(0);
  const int numPoints=x.getLength(0);

  real *rp = r.Array_Descriptor.Array_View_Pointer1;
  const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
#undef X
  const real * xp = x.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=x.getRawDataSize(0);
#define X(i0,i1) xp[i0+xDim0*(i1)]

#undef XA
  real *xap = xa.Array_Descriptor.Array_View_Pointer1;
  const int xaDim0=xa.getRawDataSize(0);
#define XA(i0,i1) xap[i0+xaDim0*(i1)]

  real *pdista=dista.Array_Descriptor.Array_View_Pointer0;
#define DISTA(i0) pdista[i0]


  // If this Mapping has a basic inverse (usually means an analytic formula for the inverse)
  // then we can invert directly : 
  if( map.getBasicInverseOption()==Mapping::canInvert )
  {

    if( debug & 2 )
      printF("***findNearestGridPoint: use basicInverse for mapping %s\n",
	     (const char*)map.getName(Mapping::mappingName));

    map.basicInverseS( x,r );  // can use analytic inverse

    // we should double check that valid values for ra were returned since some basicInverse routines
    // may not work for points that are far away from the mapping (?)
    for( int i=base; i<=bound; i++ )
    {
      if( R(i,0)==Mapping::bogus )
      {
	printf("findNearestGridPoint:ERROR: basic inverse returned a bogus result! This should not happen.\n");
	OV_ABORT("error");
      }

      for( int axis=0; axis<domainDimension; axis++ )
      {
	// map ra ra(i,.) to closest point inside or on the unit square
	R(i,axis) = min(1.,max(0.,R(i,axis)));
      }
    }

    // evaluate the positions of the closest points
    map.mapS( r,xa );

    // Set the distance if it is smaller than the current value
    for( int i=base; i<=bound; i++ )
    {
      real dist=0.;
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	dist+= SQR(XA(i,axis)-X(i,axis));
      }
      if( dist<DISTA(i) )
      {
	DISTA(i)=dist;
      }
    }

    return 0;
  }

 #ifdef USE_PPP
  bool mapUsesDistributedInverse=map.usesDistributedInverse();
 #else
  bool mapUsesDistributedInverse = false;
 #endif

 if( !mapUsesDistributedInverse )
 {
    // If the mapping does not use a distributed inverse (e.g. NurbsMapping) then
    // we can find the nearest grid point using the serial function:

    // Do not check the bounding box when finding the nearest grid point:
    map.setParameter( MappingParameters::THEfindBestGuess,true );
	
    map.approximateGlobalInverse->findNearestGridPoint(base,bound,x,r );

    map.setParameter( MappingParameters::THEfindBestGuess,false );   // reset 

      
    // --- check the distance to the actual nearest point  ---

    // Here is the grid: 
    RealArray & grid = map.approximateGlobalInverse->grid;
    const int gDim0=grid.getRawDataSize(0);
    const int gDim1=grid.getRawDataSize(1);
    const int gDim2=grid.getRawDataSize(2);
    const real *gridp = grid.Array_Descriptor.Array_View_Pointer3;
    #undef GRID
    #define GRID(i1,i2,i3,axis) gridp[i1+gDim0*(i2+gDim1*(i3+gDim2*(axis)))]

    // gid(side,axis) = gridIndexRange for the global (distributed) grid
    // dr[axis] : grid spacing on global grid
    real dr[3] = {1.,1.,1.}; // 
    int pgid[6] = {0,0,0,0,0,0};  //
    #undef gid
    #define gid(side,axis) pgid[(side)+2*(axis)]
    for( int axis=0; axis<domainDimension; axis++ )
    {
      for( int side=0; side<=1; side++ )
	gid(side,axis)=map.gridIndexRange(side,axis);
      dr[axis]=1./(max(1,gid(1,axis)-gid(0,axis)));
    }

    real xv[3];
    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
    i2=i3=0;
    for( int i=base; i<=bound; i++ )
    {
      // Evaluate the x-coords of the actual closest point (save in xa if closer)
      for( int axis=0; axis<domainDimension; axis++ )
      {
	// real rr = (iv[axis]-gid(0,axis))*dr[axis]; // from stencil walk
	iv[axis] = int( R(i,axis)/dr[axis]+gid(0,axis)+.5 );
	iv[axis] = max(gid(0,axis),min(gid(1,axis),iv[axis]));
      }
      for( int axis=0; axis<rangeDimension; axis++ )
	xv[axis] = GRID(i1,i2,i3,axis);

      // compute the distance to this closest point 
      real dist=0.;
      for( int m=0; m<rangeDimension; m++ )
      {
	dist+= SQR(xv[m]-X(i,m));
      }

      if( dist<DISTA(i) )
      { // this point is closer, save the distance and x-coords
	DISTA(i)=dist;
	for( int m=0; m<rangeDimension;  m++ ) XA(i,m) = xv[m];
      }
      else
      {
        // if the point was not closer than dista then return bogus r-coordinates
        for( int m=0; m<domainDimension;  m++ ) R(i,m) = Mapping::bogus;
      }
      
    } // end for i

  }
  else // -- Mapping uses a distributed inverse ---
  {
#ifndef USE_PPP
    OV_ABORT("ERROR: this should not be used in serial");
#else

    // ---- Parallel version: Mapping uses a distributed inverse ----

    const int np= max(1,Communication_Manager::numberOfProcessors());
    const int myid=max(0,Communication_Manager::My_Process_Number);

    if( debug >0 )
    {
      openDebugFiles();
    }
    

    ApproximateGlobalInverse *approximateGlobalInverse = map.approximateGlobalInverse;

    approximateGlobalInverse->initialize(); // Warning: this will change the class variables base,bound
      
    const MPI_Comm & OV_COMM = Overture::OV_COMM;
  

    // for each pt x(i,.) find the processors whose local grid may hold the closest point


    if( debug & 1 )
    {
      fprintf(pDebugFile,"=============== findNearestGridPoint: mapping name =%s check %i points ===================\n",
	      (const char*)map.getName(Mapping::mappingName),numPoints);
    }
      

    int *nqs = new int [np];  // nqs[p] : number of queries to send to processor p
    int *nqr = new int [np];  // nqs[p] : number of queries received from processor p
    for( int p=0; p<np; p++ ) nqs[p]=0;
    
    // ip(j,p) = i, j=0,..,nqs[p] : means x(i,.) is the j'th point sent to processor p 
    SparseArray<int> ip(numPoints,np);

//     int *pip=NULL;
//     if( numPoints>0 )
//       pip = new int [numPoints*np];    // ********* this is too large in general, use a SparseArray ? 
//     #define ip(j,p) (pip[(p)+np*(j)])


    const RealArray & globalBoundingBox = map.getBoundingBox();
    BoundingBox *& serialBoundingBox = approximateGlobalInverse->serialBoundingBox;
    assert( serialBoundingBox !=NULL );
    real pbb[6]={0.,0.,0.,0.,0.,0.};
    #define bb(side,axis) pbb[(side)+2*((axis))]

    for( int p=0; p<np; p++ )
    {
      // Get the bounding box for this processor -- we should be able to access this directly from BoundingBox, fix me 
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  bb(side,axis)=serialBoundingBox[p].rangeBound(side,axis);
	}
      }
      if( debug & 1 )
	fprintf(pDebugFile,"parallelFindNearestGP: myid=%i boundingBox for p=%i is [%g,%g][%g,%g][%g,%g]\n",myid,p,
		bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
	
      if( bb(0,0)>bb(1,0) )
      {
        // bounding box is empty, skip the check
        continue;
      }

      for( int i=base; i<=bound; i++ )
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
	{ // there may be a closer point on the grid of proc. p so check this point

	  ip.get(nqs[p],p)=i;  // this point should be sent to processor p for an answer 
	  nqs[p]++;
	}
      }
      if( debug & 1 ) fprintf(pDebugFile," myid=%i send nqs[p]=%i queries to p=%i\n",myid,nqs[p],p);
    }



    // --- send/receive number of queries ---

    MPI_Status status;
    const int tag0=115730;
    int numberOfQueries=0;      // total number of queries that we will receive 
    for( int p=0; p<np; p++ )
    {
      int tags=tag0+p, tagr=tag0+myid;
      MPI_Sendrecv(&nqs[p], 1, MPI_INT, p, tags, 
                   &nqr[p], 1, MPI_INT, p, tagr, OV_COMM, &status ); 

      if( debug & 1 ) 
      {
	fprintf(pDebugFile,"myid=%i : send %i queries to p=%i, receive %i queries from p=%i\n",myid,nqs[p],p,nqr[p],p);
	fflush(pDebugFile);
      }
	
      numberOfQueries += nqr[p];
    }

    // npr = number of processors that we will receive data from 
    // nps = number of processors that we will send data to
    // npra = number of proc. that we will rec. answers from  (=nps)
    // npsa = number of proc. that we will send answer to  (=npr)

    int npr=0, nps=0;
    for( int p=0; p<np; p++ )
    {
      if( nqr[p]>0 ) npr++;
      if( nqs[p]>0 ) nps++;
    }
    const int npra=nps, npsa=npr;
    
    // Define the processor maps: 
    //   mapr(p) p=0,..,npr  : actual processor numbers we receive from (in assign stage)
    //   maps(p) p=0,..,nps  : actual processor numbers we send to      (in assign stage)
    int *pMapr = new int [npr];    
    int *pMaps = new int [nps];
#define mapr(p) pMapr[p]
#define maps(p) pMaps[p]
    // processor maps for answers: 
#define mapra(p) maps(p)
#define mapsa(p) mapr(p)

    int kr=0, ks=0;
    for( int p=0; p<np; p++ )
    {
      if( nqr[p]>0 ){ mapr(kr)=p; kr++; }
      if( nqs[p]>0 ){ maps(ks)=p; ks++; } 
    }
    assert( kr==npr && ks==nps );

    if( debug & 2 )
    {
      fprintf(pDebugFile," *** Queries: npr=%i, nps=%i, Answers: npra=%i, npsa=%i\n",npr,nps,npra,npsa);
      fprintf(pDebugFile," mapr=");
      for( int p=0; p<npr; p++ ) fprintf(pDebugFile,"%i, ",mapr(p));
      fprintf(pDebugFile,"\n");
      fprintf(pDebugFile," maps=");
      for( int p=0; p<nps; p++ ) fprintf(pDebugFile,"%i, ",maps(p));
      fprintf(pDebugFile,"\n");

      fflush(pDebugFile);
    }


    // ******* BUFFERS ************
    //  sqb[p] : send queries buffer -- send queries to processor p
    //  rqb[p] : receive queries buffer
    //
    //  sqb[p] : send answers buffer -- send answers to processor p
    //  rqb[p] : receive answers buffer 

    const int numDataToSendPerPoint=rangeDimension+domainDimension;  // we send x and r (r holds initial guess)
    const int numDataToReceivePerPoint = domainDimension + rangeDimension;

    // --- Allocate receive buffers ---
    real **rqb = new real* [npr];
    for( int p=0; p<npr; p++ )
    {
      const int pp =mapr(p);
      rqb[p] = new real [max(1,nqr[pp]*numDataToSendPerPoint)];
    }
  
    // -- allocate MPI buffers:
    MPI_Request *receiveRequest       = new MPI_Request[npr];  
    MPI_Status  *receiveStatus        = new MPI_Status [npr];

    MPI_Request *sendRequest          = new MPI_Request[nps];  
    MPI_Status  *sendStatus           = new MPI_Status [nps];

    MPI_Request *receiveRequestAnswer = new MPI_Request[npra];  
    MPI_Status  *receiveStatusAnswer  = new MPI_Status [npra];

    MPI_Request *sendRequestAnswer    = new MPI_Request[npsa];  
    MPI_Status  *sendStatusAnswer     = new MPI_Status [npsa];

    // --- post receives for queries ---
    const int tag1=8842157;
    for( int p=0; p<npr; p++ )
    {  
      const int pp =mapr(p);
      int tag=tag1+myid;
      MPI_Irecv( rqb[p], nqr[pp]*numDataToSendPerPoint, MPI_Real, pp, tag, OV_COMM, &receiveRequest[p] );
    }

    // -- allocate send buffers ---

    // pxs[p](i,m) : holds x and r values m=0,1,...,rangeDimension+domainDimension-1
    real **sqb = new real* [nps];
    for( int p=0; p<nps; p++ )
    {
      const int pp=maps(p);
      sqb[p] = new real [max(1,nqs[pp]*numDataToSendPerPoint)];
    }

    // --- fill in send buffers ---
//     #define xs(i,n,p) sqb[p][(i)+nqs[p]*(n)]
//     #define rs(i,m,p) sqb[p][(i)+nqs[p]*(m+rangeDimension)]
    for( int p=0; p<nps; p++ )
    {
      const int pp=maps(p);
      real *sbuff = sqb[p];
      int k=0;
      for( int j=0; j<nqs[pp]; j++ )
      {   
	int i = ip(j,pp);
	for( int n=0; n<rangeDimension;  n++ ){ sbuff[k]=X(i,n); k++; } //
	for( int m=0; m<domainDimension; m++ ){ sbuff[k]=R(i,m); k++; } //
      }
      assert( k==nqs[pp]*numDataToSendPerPoint );
    }
    
    // --- send buffers ---
    for( int p=0; p<nps; p++ )
    {
      const int pp=maps(p);
      if( debug & 1 ) 
      {
	fprintf(pDebugFile," Send %i queries from myid=%i to p=%i\n",nqs[pp],myid,pp);
	fflush(pDebugFile);
      }
	
      int tag=tag1+pp;
      MPI_Isend(sqb[p], nqs[pp]*numDataToSendPerPoint, MPI_Real, pp, tag, OV_COMM, &sendRequest[p] );  
    }
  
    // --- wait for all the receives of queries to finish ---
    MPI_Waitall(npr,receiveRequest,receiveStatus);


    if( debug & 2 )
    { // sanity check:

      for( int p=0; p<npr; p++ )
      {
	const int pp=mapr(p);
	int num=0;
	MPI_Get_count( &receiveStatus[p], MPI_Real, &num );
	assert( num==nqr[pp]*numDataToSendPerPoint );
      }
    }


    // --- post receives for answers (which have not even been computed yet) ---

    real **rab = new real* [npra];  // rab = "answers" 
    for( int p=0; p<npra; p++ )
    {
      // return r, and xc (x-coords of closest pt)
      const int pp=mapra(p);

      // we receive back the number we sent: nqs[p] 
      rab[p] = new real [max(1,nqs[pp]*numDataToReceivePerPoint)];
    }

    const int tag2=8721027;
    for( int p=0; p<npra; p++ )
    {  
      const int pp=mapra(p);
      int tag=tag2+myid;
      if( debug & 1 )
      {
	fprintf(pDebugFile,"myid=%i, expect %i answers from p=%i\n",myid,nqs[pp],pp);
	fflush(pDebugFile);
      }
	
      MPI_Irecv( rab[p], nqs[pp]*numDataToReceivePerPoint, MPI_Real, pp, tag, OV_COMM, &receiveRequestAnswer[p] );
    }

    // ------------------------------------------
    // --- un-pack the query values received ----
    // ------------------------------------------

    RealArray xc,rc;
    if( numberOfQueries>0 )
    {
      xc.redim(numberOfQueries,rangeDimension);
      rc.redim(numberOfQueries,domainDimension);
    }

    real * rcp = rc.Array_Descriptor.Array_View_Pointer1;
    const int rcDim0=rc.getRawDataSize(0);
#undef RC
#define RC(i0,i1) rcp[i0+rcDim0*(i1)]
#undef XC
    real * xcp = xc.Array_Descriptor.Array_View_Pointer1;
    const int xcDim0=xc.getRawDataSize(0);
#define XC(i0,i1) xcp[i0+xcDim0*(i1)]

// #define xrb(i,n,p) rqb[p][(i)+nqr[p]*(n)]
// #define rrb(i,m,p) rqb[p][(i)+nqr[p]*(m+rangeDimension)]
    int i=0;
    for( int p=0; p<npr; p++ )
    {
      const int pp=mapr(p);
      real *rbuff=rqb[p];
      int k=0;
      for( int j=0; j<nqr[pp]; j++ )  
      {
	for( int n=0; n<rangeDimension;  n++ ){ XC(i,n) = rbuff[k]; k++; } //  
	for( int m=0; m<domainDimension; m++ ){ RC(i,m) = rbuff[k]; k++; } //
	i++;
      }
      if( k!=nqr[pp]*numDataToReceivePerPoint )
      {
	fprintf(pDebugFile,"ERROR: k=%i != nqr[pp]*numDataToReceivePerPoint=%i\n",
		k,nqr[pp]*numDataToReceivePerPoint);
        fflush(pDebugFile);
	OV_ABORT("error");
      }
      
    }
    assert( i==numberOfQueries );

    // we can delete rqb[p]  now 
    for( int p=0; p<npr; p++ )
      delete [] rqb[p];
    delete [] rqb;

    if( debug & 2 )
    {
      for( int i=0; i<numberOfQueries; i++ )
      {
	fprintf(pDebugFile,"parallelFindNearestGP: myid=%i check point i=%i : xc=(%g,%g,%g), rc=(%g,%g,%g)\n",myid,i,
		XC(i,0),(rangeDimension>1 ? XC(i,1) : 0.), (rangeDimension>2 ? XC(i,2) : 0.),
		RC(i,0),(domainDimension>1 ? RC(i,1) : 0.), (domainDimension>2 ? RC(i,2) : 0.));

      }
      fflush(pDebugFile);
      Communication_Manager::Sync();
      MPI_Barrier(OV_COMM);
	
    }
	

    // *********************** ACTUAL SERIAL FIND CLOSEST POINT IS HERE *****************************

    // find the closest point
    if( numberOfQueries>0 )
    {

      // inverseMap: approximateGlobalInverse->inverse( xc,rc,rxc,workSpace,params );

      // Do not check the bounding box when finding the nearest grid point:
      map.setParameter( MappingParameters::THEfindBestGuess,true );
	
      map.approximateGlobalInverse->findNearestGridPoint(0,numberOfQueries-1,xc,rc );

      map.setParameter( MappingParameters::THEfindBestGuess,false );   // reset       


      // Evaluate the x-coords of the actual closest point (save in xc)

      RealArray & grid = map.approximateGlobalInverse->grid;
      
      // Here is the grid: 
      const int gDim0=grid.getRawDataSize(0);
      const int gDim1=grid.getRawDataSize(1);
      const int gDim2=grid.getRawDataSize(2);
      const real *gridp = grid.Array_Descriptor.Array_View_Pointer3;
#undef GRID
#define GRID(i1,i2,i3,axis) gridp[i1+gDim0*(i2+gDim1*(i3+gDim2*(axis)))]

      // gid(side,axis) = gridIndexRange for the global (distributed) grid
      // dr[axis] : grid spacing on global grid
      real dr[3] = {1.,1.,1.}; // 
      int pgid[6] = {0,0,0,0,0,0};  //
      #undef gid
      #define gid(side,axis) pgid[(side)+2*(axis)]
      for( int axis=0; axis<domainDimension; axis++ )
      {
	for( int side=0; side<=1; side++ )
	  gid(side,axis)=map.gridIndexRange(side,axis);
	dr[axis]=1./(max(1,gid(1,axis)-gid(0,axis)));
      }


      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      i2=i3=0;
      for( int i=0; i<numberOfQueries; i++ )
      {
	for( int axis=0; axis<domainDimension; axis++ )
	{
          // real rr = (iv[axis]-gid(0,axis))*dr[axis]; // from stencil walk
          iv[axis] = int( RC(i,axis)/dr[axis]+gid(0,axis)+.5 );
	  iv[axis] = max(gid(0,axis),min(gid(1,axis),iv[axis]));
	}
        for( int axis=0; axis<rangeDimension; axis++ )
	  XC(i,axis)= GRID(i1,i2,i3,axis);
      }

      if( debug & 1 )
      {
	for( int i=0; i<numberOfQueries; i++ )
	{
	  fprintf(pDebugFile,"parallelFindNearestGP: myid=%i after local find nearest pt, point i=%i : xc=(%g,%g,%g), rc=(%g,%g,%g)\n",myid,i,
		 XC(i,0),(rangeDimension>1 ? XC(i,1) : 0.), (rangeDimension>2 ? XC(i,2) : 0.),
		 RC(i,0),(domainDimension>1 ? RC(i,1) : 0.), (domainDimension>2 ? RC(i,2) : 0.));
	}
        fflush(pDebugFile);
      }
    }
    

    // -----------------------------------------------
    // --- Send answers back : pack into a buffer  ---
    // -----------------------------------------------

    real **sab = new real* [npsa];  // sab = send-answers buffer
    for( int p=0; p<npsa; p++ )
    {
      // we send back the number we received : nqr[p]
      const int pp=mapsa(p);
      sab[p] = new real [max(1,nqr[pp]*numDataToReceivePerPoint)];
    }

// #undef rrb
// #define rrb(i,m,p) sab[p][(i)+nqr[p]*(m)]
// #define rxb(i,m,p) sab[p][(i)+nqr[p]*((m)+domainDimension)]
    i=0;
    for( int p=0; p<npsa; p++ )
    {
      const int pp=mapsa(p);
      real *sbuff = sab[p];
      int k=0;
      for( int j=0; j<nqr[pp]; j++ )
      {
	for( int m=0; m<domainDimension; m++ ){ sbuff[k]= RC(i,m); k++; } //
	for( int m=0; m<rangeDimension;  m++ ){ sbuff[k]= XC(i,m); k++; } //
	i++;
      }
    }

    // --------------------
    // --- Send buffers ---
    // --------------------

    for( int p=0; p<npsa; p++ )
    {
      const int pp=mapsa(p);
      int tag=tag2+pp;
      if( debug & 1 )
      {
	fprintf(pDebugFile,"parallelFindNearestGP: myid=%i, send %i answers to p=%i\n",myid,nqr[pp],pp);
	fflush(pDebugFile);
      }
	
      MPI_Isend(sab[p], nqr[pp]*numDataToReceivePerPoint, MPI_Real, pp, tag, OV_COMM, &sendRequestAnswer[p] );  
    }

    if( debug & 4 )
      MPI_Barrier(OV_COMM);
      
    // --- wait for all the receives to finish ---
    MPI_Waitall(npra,receiveRequestAnswer,receiveStatusAnswer);  

    if( debug & 2 )
    { // sanity check:
      for( int p=0; p<npra; p++ )
      {
        const int pp=mapra(p);
	int num=0;
	MPI_Get_count( &receiveStatusAnswer[p], MPI_Real, &num );
	assert( num==nqs[pp]*numDataToReceivePerPoint );
      }
    }

    // --- assign results back into the r,xa arrays ---

    // Any points not assigned are given a bogus value
    for( int i=base; i<=bound; i++ )
    {
      for( int m=0; m<domainDimension; m++ ) R(i,m) = Mapping::bogus;
    }

// #undef rrb
// #define rrb(i,m,p) rab[p][(i)+nqs[p]*(m)]
// #undef rxb 
// #define rxb(i,m,p) rab[p][(i)+nqs[p]*((m)+domainDimension)]

    real rv[3]={0.,0.,0.}, xv[3]={0.,0.,0.};
    for( int p=0; p<npra; p++ )
    {
      const int pp=mapra(p);
      real *rbuff=rab[p];
      int k=0;
      for( int j=0; j<nqs[pp]; j++ )
      {
	i = ip(j,pp);  // the result goes here 
	assert( i>=base && i<=bound );
	  
	for( int m=0; m<domainDimension; m++ ){ rv[m] = rbuff[k]; k++; } //
	for( int m=0; m<rangeDimension;  m++ ){ xv[m] = rbuff[k]; k++; }

	if( debug & 1 )
	{
	  fprintf(pDebugFile," received answer i=%i (j=%i) from pp=%i : r=(%g,%g,%g) xc=(%g,%g,%g)\n",
                  i,j,pp,rv[0],rv[1],rv[2],xv[0],xv[1],xv[2]);
	  
	}
	  
        // compute the distance to this closest point 
	real dist=0.;
	for( int m=0; m<rangeDimension; m++ )
	{
	  dist+= SQR(xv[m]-X(i,m));
	}

	if( dist<DISTA(i) )
	{
	  DISTA(i)=dist;
	  for( int m=0; m<domainDimension; m++ ) R(i,m) = rv[m];
	  for( int m=0; m<rangeDimension;  m++ ) XA(i,m) = xv[m];
	  if( debug & 1 )
	  {
	    fprintf(pDebugFile," -- new closest pt found for pt i=%i : x=(%g,%g,%g), r=(%g,%g,%g) xa=(%g,%g,%g) dist=%8.2e\n",i,
		    X(i,0),(rangeDimension>1 ? X(i,1) : 0.), (rangeDimension>2 ? X(i,2) : 0.),
		    R(i,0),(domainDimension>1 ? R(i,1) : 0.), (domainDimension>2 ? R(i,2) : 0.), 
                    XA(i,0),(rangeDimension>1 ? XA(i,1) : 0.), (rangeDimension>2 ? XA(i,2) : 0.),
                    DISTA(i));
	  }
	}
	  
      }
    }


    if( debug>0 )
    {
      fprintf(pDebugFile,"parallelFindNearestGP:DONE\n"
              " ===============================================================================\n");
      fflush(pDebugFile);
    }
      
    // wait for ALL sends to finish on this processor before we can clean up
    MPI_Waitall(nps,sendRequest,sendStatus);
    MPI_Waitall(npsa,sendRequestAnswer,sendStatusAnswer);

    delete [] nqs;
    delete [] nqr;
//    delete [] pip;

    delete [] receiveRequest;
    delete [] receiveStatus;
    delete [] sendRequest;
    delete [] sendStatus;

    delete [] receiveRequestAnswer;
    delete [] receiveStatusAnswer;
    delete [] sendRequestAnswer;
    delete [] sendStatusAnswer;

    for( int p=0; p<nps; p++ )
    {
      delete [] sqb[p];
    }
    delete [] sqb;
    
    for( int p=0; p<npra; p++ )
    {
      delete [] rab[p];
    }
    delete [] rab;
    for( int p=0; p<npsa; p++ )
    {
      delete [] sab[p];
    }
    delete [] sab;
    
    delete [] pMapr;
    delete [] pMaps;
    

#endif

  } // end mapping uses a distributed inverse
  
  return 0;
  
}


// =====================================================================================================
/// \brief Find the nearest valid grid point on a CompositeGrid to given points in space, x(i,.). 
///        A valid point is one with mask(i1,i2,i3)!=0 . 
/// \detail NOTE: This function currently assumes that the points x(i,.) could NOT interpolate and
///    so we are finding the best guess for points that are outside the grid.
///
/// \param cg (input) : CompositeGrid to check for closest valid point.
/// \param x (input) : x(i,0:r-1) (r=rangeDimension) list of points to check (in parallel each processor
///               provides its own set of points).
/// \param il (output) : ip(i,0:r) = (i1,i2,i3,donor) index location and grid number of
///                      the closest valid point. 
/// \param ci (output) : ci(i,0:r-1) are the r-coordinates of the closest pt in the donor grid.
/// \return 0 for success. 
// =====================================================================================================
int
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
  
  int debug=7;
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
	   " **** findNearestValidGridPoint log file, myid=%i, NP=%i pts=%i ********* \n"
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
  
  RealArray xa,ra,xc;
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

    ra=-1.;
    for( int i=0; i<numberToCheck; i++ )
    {
      int iai=IA(i);
      for( int axis=0; axis<rangeDimension; axis++ )
	XA(i,axis)=X(iai,axis);
    }
    

    // If this Mapping has a basic inverse (usually means an analytic formula for the inverse)
    // then we can invert directly : 
    if( map.getBasicInverseOption()==Mapping::canInvert )
    {
      map.basicInverseS( xa,ra );  // can use user supplied inverse

      // we should double check that valid values for ra were returned since some basicInverse routines
      // may not work for points that are far away from the mapping.

      for( int i=0; i<numberToCheck; i++ )
      {
	if( RA(i,0)==Mapping::bogus )
	{
	  printf("findNearestValidGridPoint:ERROR: basic inverse returned a bogus result!\n");
	  OV_ABORT("error");
	}

	for( int axis=0; axis<rangeDimension; axis++ )
	{
	  // map ra ra(i,.) to closest point inside or on the unit square
	  RA(i,axis) = min(1.,max(0.,RA(i,axis)));
	}
      }
    }
    else if( !map.usesDistributedInverse() )
    {
      // If the mapping does not use a distributed inverse (e.g. NurbsMapping) then
      // we can find the nearest grid point using the serial function:

      // Do not check the bounding box when finding the nearest grid point:
      map.setParameter( MappingParameters::THEfindBestGuess,true );
	
      map.approximateGlobalInverse->findNearestGridPoint(0,numberToCheck-1,xa,ra );

      map.setParameter( MappingParameters::THEfindBestGuess,false );   // reset 

    }
    else
    {
      // Mapping uses a distributed inverse ...

    #ifndef USE_PPP

      // Do not check the bounding box when finding the nearest grid point:
      map.setParameter( MappingParameters::THEfindBestGuess,true );
	
      map.approximateGlobalInverse->findNearestGridPoint(0,numberToCheck-1,xa,ra );

      map.setParameter( MappingParameters::THEfindBestGuess,false );   // reset 

    #else
      // ---- Parallel version: Mapping uses a distributed inverse ----


      


      OV_ABORT("findNearestValidGridPoint: map.usesDistributedInverse() : finish me");
    #endif      
    }
    

    // We now have the r coordinates of the closest points on this grid: RA(i,.) 
    // Make a sub-list of points whose distance is less than the current best distance 

    // --- xc : holds the x-coordinates of the closest points  ---
    xc.redim(I,rangeDimension);
    const real *pxc = xc.Array_Descriptor.Array_View_Pointer1;
    const int xcDim0=xc.getRawDataSize(0);
    #undef XC
    #define XC(i0,i1) pxc[i0+xcDim0*(i1)] 

    // Todo: for Cartesian grids we can evaluate XC directly 
    map.mapS( ra,xc );

    // ib : indirection array that holds subset of points that we should check the mask for 
    ib.redim(I);
    int *ibp = ib.Array_Descriptor.Array_View_Pointer0;
    #define IB(i0) ibp[i0]    

    j=0;
    for( int i=0; i<numberToCheck; i++ )
    {
      real dist=0.;
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	dist+= SQR(XA(i,axis)-XC(i,axis));
      }
      if( dist<DISTA(IA(i)) )
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
	real dist=0.;
	for( int axis=0; axis<rangeDimension; axis++ )
	{
	  dist+= SQR(XA(i,axis)-XC(i,axis));
	}
	int iai=IA(i);
	real oldDist=DISTA(iai);
	DISTA(iai) = dist;  // here is the new closest distance 

	for( int axis=0; axis<domainDimension; axis++ )
	{
	  CI(iai,axis)=RA(i,axis);        // r-coordinates of closest pt
	  IL(iai,axis)=cir[n].il[axis];   // lower left corner of the interpolation stencil
	}
        IL(iai,numberOfDimensions)=grid;
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
              i,IL(i,numberOfDimensions),
              X(i,0),X(i,1),(numberOfDimensions==2 ? 0. : X(i,2)),
	      IL(i,0),IL(i,1),(numberOfDimensions==2 ? 0 : IL(i,2)),
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
