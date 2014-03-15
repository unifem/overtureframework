#include "Mapping.h"
#include "ParallelUtility.h"
#include "display.h"
#include "Inverse.h"           // defines global and local inverses
#include "DistributedInverse.h"
#include "SparseArray.h"

// *serial-array version*
void Mapping::
inverseMapS( const RealArray & x, 
             RealArray & r, 
	     RealArray & rx /* =Overture::nullRealArray() */,
	     MappingParameters & params /* =Overture::nullMappingParameters() */ )
// =====================================================================================
// /Description:
//  --- Here is the generic inverse ----
//
// /x (input) : invert these points. The dimensions of this array will determine which
//     points are inverted.
// /r (input/output) : On input this is an initial guess. If you know a reasonable initial
//   guess then supply it, If you don't know an initial guess
//    then set r=-1. for those points that you do not know a guess. If you do not know a guess
//     then do NOT specify some valid value like .5 since this will probably be slower than allowing
//     the value to be automatically generated.
// /rx (output): the derivatives of the inverse mapping.
//  /params (input) :
//   \begin{description}
//     \item[params.computeGlobalInverse] : TRUE means compute a full global inverse,
//       FALSE means only compute a local inverse using the initial guess supplied in r
//     \item[params.periodicityOfSpace] : 
//     \item[params.periodVector] : 
//   \end{description}
//\end{MappingInclude.tex}
//=================================================================================
{
  // NOTE: This code is similar to that in dpmMap (and findClosestGridPoint.C)


  int debug = 0; // 3; // Mapping::debug; // 0; // 3;
  
  if( debug & 8 )
    printF("inverseMapS: name=%s, params.computeGlobalInverse=%i\n",(const char*)getName(mappingName),params.computeGlobalInverse);

  if( basicInverseOption==canInvert && params.periodicityOfSpace==0 )
  {
    basicInverseS( x,r,rx,params );  // can use user supplied inverse
    return;
  }

#ifndef USE_PPP
  // *serial inverse*
  MappingWorkSpace workSpace; 
  if( params.computeGlobalInverse )
  {
    // first get the initial guess
    approximateGlobalInverse->inverse( x,r,rx,workSpace,params );

    // Now do Newton to Invert:
    exactLocalInverse->inverse( x,r,rx,workSpace,TRUE );   // TRUE means use results found in the
                                                           // workSpace
  }
  else
  {
    exactLocalInverse->inverse( x,r,rx,workSpace,FALSE );
  }
#else

  // ***** parallel inverse *****

//   int myid=max(0,Communication_Manager::My_Process_Number);
//   printf("inverseMapS: myid=%i usesDistributedInverse=%i x=[%i,%i]\n",myid,(int)usesDistributedInverse(),
//           x.getBase(0),x.getBound(0));

  if( !usesDistributedInverse() )
  {
    // ***** this mapping can be inverted with no parallel communication ****

    MappingWorkSpace workSpace; 
    if( params.computeGlobalInverse )
    {
      // first get the initial guess
      
      approximateGlobalInverse->inverse( x,r,rx,workSpace,params );

      // Now do Newton to Invert:
      exactLocalInverse->inverse( x,r,rx,workSpace,TRUE );   // TRUE means use results found in the
                                                             // workSpace
    }
    else
    {
      exactLocalInverse->inverse( x,r,rx,workSpace,FALSE );
    }
  }
  else
  {

    // ***** this Mapping used a distributed grid for the inverse *****

    if( debug >0 )
      openDebugFiles();
    

    MappingWorkSpace workSpace; 
    if( params.computeGlobalInverse )
    {
      approximateGlobalInverse->initialize(); // Warning: this will change the class variables base,bound
      
      int base,bound,computeMap=false,computeMapDerivative=false;
      Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

      const MPI_Comm & OV_COMM = Overture::OV_COMM;
  
      const int np= max(1,Communication_Manager::numberOfProcessors());
      const int myid=max(0,Communication_Manager::My_Process_Number);

      // for each pt x(i,.) find the processors whose local grid may hold the closest point

      real * rp = r.Array_Descriptor.Array_View_Pointer1;
      const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
#undef X
      const real * xp = x.Array_Descriptor.Array_View_Pointer1;
      const int xDim0=x.getRawDataSize(0);
#define X(i0,i1) xp[i0+xDim0*(i1)]
      real * rxp = rx.Array_Descriptor.Array_View_Pointer2;
      const int rxDim0=rx.getRawDataSize(0);
      const int rxDim1=rx.getRawDataSize(1);
#undef RX
#define RX(i0,i1,i2) rxp[i0+rxDim0*(i1+rxDim1*(i2))]

      if( debug & 1 )
      {
        fprintf(pDebugFile,"----------mapInverse:START myid=%i base=%i bound=%i computeMap=%i "
             "computeMapDerivative=%i domainDimension=%i rangeDimension=%i name=%s -------------\n",
		myid,base,bound,computeMap,computeMapDerivative,domainDimension,rangeDimension,
             (const char*)getName(mappingName));
      }
      

      int numPoints=bound-base+1;

      // NOTE: We know here which processors this Mapping's inverse grid is distributed over.
      //       We could use this info, but currently don't. 

      int *nqs = new int [np];  // nqs[p] : number of queries to send to processor p
      int *nqr = new int [np];  // nqs[p] : number of queries received from processor p
      for( int p=0; p<np; p++ ) nqs[p]=0;
    
      // ip(j,p) = i, j=0,..,nqs[p] : means x(i,.) is the j'th point sent to processor p 
      SparseArray<int> ip(numPoints,np);

//       int *pip=NULL;
//       if( numPoints>0 )
//          pip = new int [numPoints*np];    // ********* this is too large in general, use a SparseArray ? 
// #define ip(j,p) (pip[(p)+np*(j)])


      const RealArray & globalBoundingBox = getBoundingBox();
      const real extensionFactor=approximateGlobalInverse->stencilWalkBoundingBoxExtensionFactor;

      BoundingBox *& serialBoundingBox = approximateGlobalInverse->serialBoundingBox;
      assert( serialBoundingBox !=NULL );
      // IndexBox pBox;
      for( int p=0; p<np; p++ )
      {
        // Extend the bounding box for this processor in the same manner as findNearestGridPoint

        // CopyArray::getLocalArrayBox( p,grid,pBox );  // pBox : dimensions of gridSerial on processor p 

        real pbb[6]={0.,0.,0.,0.,0.,0.};
        #define bb(side,axis) pbb[(side)+2*((axis))]
        bool boxIsEmpty=false; // *wdh* 100323 -- check for empty boxes on processors
	real delta = 0; //kkc 110329
	for( int axis=0; axis<rangeDimension; axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
	    bb(side,axis)=serialBoundingBox[p].rangeBound(side,axis);
	  }
          if( bb(0,axis)>bb(1,axis) )
	  {
            boxIsEmpty=true;
	    break;
	  }
	  
	  //          real delta =extensionFactor*(globalBoundingBox(1,axis)-globalBoundingBox(0,axis));
// kkc 110329	    bb(0,axis)-=delta;
// kkc 110329	    bb(1,axis)+=delta;
	  delta = max(delta,(globalBoundingBox(1,axis)-globalBoundingBox(0,axis)));
	  //	  delta = max(delta,extensionFactor*(globalBoundingBox(1,axis)-globalBoundingBox(0,axis)));
	}

	for( int axis=0; !boxIsEmpty && axis<rangeDimension; axis++ )
	  { // kkc 110329
	    bb(0,axis)-=delta;
	    bb(1,axis)+=delta;
	  }

	if( boxIsEmpty ) continue;
	
	if( debug & 1 )
	  fprintf(pDebugFile," myid=%i p=%i extended-boundingBox = [%g,%g][%g,%g][%g,%g]\n",myid,p,
		 bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
	

	for( int i=base; i<=bound; i++ )
	{
	  // if x(i,.) is inside the bounding box for processor p 
          bool inside = true;
	  for( int axis=0; axis<rangeDimension; axis++ )
	  {
	    inside = inside && X(i,axis)>=bb(0,axis) && X(i,axis)<=bb(1,axis);
	    if( !inside ) break;
	  }
	  if( inside ) // x(i,.) is inside box[p] )
	  {
	    ip.get(nqs[p],p)=i;  // this point should be sent to processor p for an answer 
	    nqs[p]++;
	  }
	}
        if( debug & 1 ) fprintf(pDebugFile,"mapInverse: myid=%i send nqs[p]=%i queries to p=%i\n",myid,nqs[p],p);
      }

      // --- send/receive number of queries ---
      // we need to send computeMap and computeMapDerivative since these may not be the
      // same on all processors!

      int myInfo[3]={0,computeMap,computeMapDerivative}; //
      // info(0:2,p) : holds (nqr[p],computeMap[p],computeMapDerivative[p])
      int *pinfo = new int [3*np];
      #define info(i,p) pinfo[(i)+3*(p)]

      MPI_Status status;
      const int tag0=410389;
      int numberOfQueries=0;      // total number of queries that we will receieve 
      for( int p=0; p<np; p++ )
      {
        // *** send computeMap computeMapDerivative here too !

	int tags=tag0+p, tagr=tag0+myid;
        myInfo[0]=nqs[p];
	MPI_Sendrecv(myInfo,     3, MPI_INT, p, tags, 
		     &info(0,p), 3, MPI_INT, p, tagr, OV_COMM, &status ); 
// 	MPI_Sendrecv(&nqs[p], 1, MPI_INT, p, tags, 
// 		     &nqr[p], 1, MPI_INT, p, tagr, OV_COMM, &status ); 

        nqr[p]=info(0,p);
	
	if( debug & 1 ) 
	{
	  fprintf(pDebugFile,"mapInverse: myid=%i send %i queries to p=%i (map=%i,der=%i), "
                             "receive %i queries from p=%i, p: (map=%i der=%i)\n",
		 myid,nqs[p],p,computeMap,computeMapDerivative, nqr[p],p,info(1,p),info(2,p));
	  fflush(pDebugFile);
	}
	
	numberOfQueries += nqr[p];
      }

      // For now -- we compute r and rx for the queries if there is ANY processor wants these results,
      // but we do not return rx if the requesting processor does not want them.

      int computeMapQuery=false, computeMapDerivativeQuery=false;
      for( int p=0; p<np; p++ )
      {
	computeMapQuery = computeMapQuery || info(1,p);
	computeMapDerivativeQuery = computeMapDerivativeQuery || info(2,p);
      }

      // npr = number of processors that we will receive queries from 
      // nps = number of processors that we will send queries to
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
      //  rqb[p] : receieve answers buffer 

      const int numDataToSendPerPoint=rangeDimension+domainDimension;  // we send x and r 

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

      // --- post receives ---
      const int tag1=3610852;
      for( int p=0; p<npr; p++ )
      {  
        const int pp =mapr(p);
	int tag=tag1+myid;
	if( debug & 1 ) 
	{
	  fprintf(pDebugFile,"mapInverse: Post rec. for %i queries for myid=%i from p=%i with tag=%i"
                  "(numDataToSendPerPoint=%i) \n",nqr[pp],myid,pp,tag,numDataToSendPerPoint);
	  fflush(pDebugFile);
	}
	MPI_Irecv( rqb[p], nqr[pp]*numDataToSendPerPoint, MPI_Real, pp, tag, OV_COMM, &receiveRequest[p] );
      }

      // -- allocate query send buffers ---

      // pxs[p](i,m) : holds x and r values m=0,1,...,rangeDimension+domainDimension-1
      real **sqb = new real* [nps];
      for( int p=0; p<nps; p++ )
      {
        const int pp=maps(p);
	sqb[p] = new real [max(1,nqs[pp]*numDataToSendPerPoint)];
      }

      // --- fill in send buffers ---
// #define xs(i,n,p) sqb[p][(i)+nqs[p]*(n)]
// #define rs(i,m,p) sqb[p][(i)+nqs[p]*(m+rangeDimension)]
      for( int p=0; p<nps; p++ )
      {
	const int pp=maps(p);
	real *sbuff = sqb[p];
        int k=0;
	for( int j=0; j<nqs[pp]; j++ )
	{   
	  int i = ip(j,pp);
	  for( int n=0; n<rangeDimension; n++ ){ sbuff[k]=X(i,n); k++; } //  xs(j,n,p) = X(i,n); 
          if( computeMap )
  	    for( int m=0; m<domainDimension; m++ ){ sbuff[k]=R(i,m); k++; } // rs(j,m,p) = R(i,m);
	  else
            k+=domainDimension;
	}
        assert( k==nqs[pp]*numDataToSendPerPoint );
      }
    
      // --- send buffers ---
      for( int p=0; p<nps; p++ )
      {
        const int pp=maps(p);
	int tag=tag1+pp;
	if( debug & 1 ) 
	{
	  fprintf(pDebugFile,"mapInverse: Send %i queries from myid=%i to p=%i with tag=%i\n",nqs[pp],myid,pp,tag);
	  fflush(pDebugFile);
	}
	
	MPI_Isend(sqb[p], nqs[pp]*numDataToSendPerPoint, MPI_Real, pp, tag, OV_COMM, &sendRequest[p] );  
      }
  
      // --- wait for all the receives of queries to finish ---
      MPI_Waitall(npr,receiveRequest,receiveStatus);


      if( debug & 2 )
      { // sanity check:
        MPI_Barrier(OV_COMM);

	for( int p=0; p<npr; p++ )
	{
          const int pp=mapr(p);
	  int num=0;
	  MPI_Get_count( &receiveStatus[p], MPI_Real, &num );
	  assert( num==nqr[pp]*numDataToSendPerPoint );
	}
      }


      // --- post receives for answers (which have not even been computed yet) ---
      //     return r and maybe rx values 
      // For now make the next the same for all proc: 

      const int numDataToReceivePerPoint = domainDimension + domainDimension*rangeDimension*int(computeMapDerivative);

      real **rab = new real* [npra];  // rab = "answers" 
      for( int p=0; p<npra; p++ )
      {
        // return r, and optionally rx values
	const int pp=mapra(p);
	
        // we receive back the number we sent: nqs[pp] 
	rab[p] = new real [max(1,nqs[pp]*numDataToReceivePerPoint)];
      }

      const int tag2=2138652;
      for( int p=0; p<npra; p++ )
      {  
	const int pp=mapra(p);
	int tag=tag2+myid;
        if( debug & 1 )
        {
          fprintf(pDebugFile,"inverseMap: myid=%i, expect %i answers (%i values) from p=%i\n",myid,nqs[pp],nqs[pp]*numDataToReceivePerPoint,pp);
	  fflush(pDebugFile);
	}
	
	MPI_Irecv( rab[p], nqs[pp]*numDataToReceivePerPoint, MPI_Real, pp, tag, OV_COMM, &receiveRequestAnswer[p] );
      }

      // ------------------------------------------
      // --- un-pack the query values received ----
      // ------------------------------------------

      RealArray xc,rc,rxc;
      if( numberOfQueries>0 )
      {
	xc.redim(numberOfQueries,rangeDimension);
	rc.redim(numberOfQueries,domainDimension);

	if( computeMapDerivativeQuery )
          rxc.redim(numberOfQueries,domainDimension,rangeDimension); 
      }

      real * rcp = rc.Array_Descriptor.Array_View_Pointer1;
      const int rcDim0=rc.getRawDataSize(0);
#undef RC
#define RC(i0,i1) rcp[i0+rcDim0*(i1)]
#undef XC
      real * xcp = xc.Array_Descriptor.Array_View_Pointer1;
      const int xcDim0=xc.getRawDataSize(0);
#define XC(i0,i1) xcp[i0+xcDim0*(i1)]
      real * rxcp = rxc.Array_Descriptor.Array_View_Pointer2;
      const int rxcDim0=rxc.getRawDataSize(0);
      const int rxcDim1=rxc.getRawDataSize(1);
#undef RXC
#define RXC(i0,i1,i2) rxcp[i0+rxcDim0*(i1+rxcDim1*(i2))]


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
	  for( int n=0; n<rangeDimension;  n++ ){ XC(i,n) = rbuff[k]; k++; } //XC(i,n) = xrb(j,n,p); 
	  for( int m=0; m<domainDimension; m++ ){ RC(i,m) = rbuff[k]; k++; } //RC(i,m) = rrb(j,m,p);
	  i++;
	}
	if( k!=nqr[pp]*numDataToSendPerPoint )
	{
	  fprintf(pDebugFile,"ERROR: k=%i != nqr[pp]*numDataToReceivePerPoint=%i, pp=%i nqr[pp]=%i\n",
		  k,nqr[pp]*numDataToSendPerPoint,pp,nqr[pp]);
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
	  fprintf(pDebugFile,"inverseMap: myid=%i check point i=%i : xc=(%g,%g,%g), rc=(%g,%g,%g)\n",myid,i,
		 XC(i,0),(rangeDimension>1 ? XC(i,1) : 0.), (rangeDimension>2 ? XC(i,2) : 0.),
		 RC(i,0),(domainDimension>1 ? RC(i,1) : 0.), (domainDimension>2 ? RC(i,2) : 0.));

	}
        fflush(pDebugFile);
	Communication_Manager::Sync();
	MPI_Barrier(OV_COMM);
	
      }
	

      // *********************** ACTUAL INVERSE IS HERE *****************************

      // find the closest point
      if( numberOfQueries>0 )
        approximateGlobalInverse->inverse( xc,rc,rxc,workSpace,params );

      // When the exact local inverse is not a distributed operation we could reduce communication 
      // at this point by sending back just the closest points r -- then doing the exact local inverse
      // at the other processor 

      // For now we invert here -- this will handle the case of the DataPointMapping when the exact local
      // inverse uses the same distributed grid as the approximate global inverse

      // Now do Newton to Invert:
      if( false )
	printf("inverseMap: myid=%i mapping=%s usesDistributedInverse()=%i usesDistributedMap()=%i\n",myid,
	       (const char*)getName(mappingName),(int)usesDistributedInverse(),(int)usesDistributedMap());
      
      if( numberOfQueries>0 || usesDistributedMap() ) 
        exactLocalInverse->inverse( xc,rc,rxc,workSpace,true );

      if( debug & 2 )
	MPI_Barrier(OV_COMM);

      if( debug & 1 )
      {
	for( int i=0; i<numberOfQueries; i++ )
	{
	  fprintf(pDebugFile,"inverseMap: myid=%i after exactInverse, point i=%i : xc=(%g,%g,%g), rc=(%g,%g,%g)\n",myid,i,
		 XC(i,0),(rangeDimension>1 ? XC(i,1) : 0.), (rangeDimension>2 ? XC(i,2) : 0.),
		 RC(i,0),(domainDimension>1 ? RC(i,1) : 0.), (domainDimension>2 ? RC(i,2) : 0.));
	}
        fflush(pDebugFile);
      }


      // -----------------------------------------------
      // --- Send answers back : pack into a buffer  ---
      // -----------------------------------------------

      int *numDataToSendPerQueryPoint = new int [npsa];
      for( int p=0; p<npsa; p++ )
      {
	const int pp=mapsa(p);
        // we send back r and optionally rx depending on the processor that requested the answer
	numDataToSendPerQueryPoint[p]=domainDimension + info(2,pp)*domainDimension*rangeDimension;
      }
      

      real **sab = new real* [npsa];  // sab = send-answers buffer
      for( int p=0; p<npsa; p++ )
      {
        // we send back the number we received : nqr[p]
        const int pp=mapsa(p);
	sab[p] = new real [max(1,nqr[pp]*numDataToSendPerQueryPoint[p])];
      }

#undef rrb
// #define rrb(i,m,p) sab[p][(i)+nqr[p]*(m)]
// #define rxb(i,m,n,p) sab[p][(i)+nqr[p]*((m)+domainDimension*(n+1))]
      i=0;
      for( int p=0; p<npsa; p++ )
      {
	const int pp=mapsa(p);
	real *sbuff = sab[p];
	int k=0;
	for( int j=0; j<nqr[pp]; j++ )
	{
	  for( int m=0; m<domainDimension; m++ ){ sbuff[k]= RC(i,m); k++; } // rrb(j,m,p) = RC(i,m);
	  if( info(2,pp) ) // true if we are to computeMapDerivative for processor p
	  {
	    for( int m=0; m<domainDimension; m++ )for( int n=0; n<rangeDimension; n++ )
	    {
	      sbuff[k]= RXC(i,m,n); k++; // rxb(j,m,n,p) = RXC(i,m,n);
	    }
	  }
	  i++;
	}
        if( k!=nqr[pp]*numDataToSendPerQueryPoint[p] )
	{
          fprintf(pDebugFile,"inverseMap:ERROR: k=%i != nqr[pp]*numDataToSendPerQueryPoint[p]=%i, nqr[pp]=%i, numDataToSendPerQueryPoint[p]=%i\n"
                  " info(2,pp)=%i, pp=%i\n",k,nqr[pp]*numDataToSendPerQueryPoint[p],nqr[pp],numDataToSendPerQueryPoint[p],info(2,pp),pp);
	  OV_ABORT("error");
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
          fprintf(pDebugFile,"inverseMap: myid=%i, send %i answers (%i values) to p=%i\n",myid,nqr[pp],nqr[pp]*numDataToSendPerQueryPoint[p],pp);
	  fflush(pDebugFile);
	}
	
	MPI_Isend(sab[p], nqr[pp]*numDataToSendPerQueryPoint[p], MPI_Real, pp, tag, OV_COMM, &sendRequestAnswer[p] );  
      }

      if( debug & 4 )
	MPI_Barrier(OV_COMM);
      
      // --- wait for all the receives for answers to finish ---
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

      // --- assign results back into the r,rx arrays ---

// #undef rrb
// #define rrb(i,m) rbuff[(i)+nqs[pp]*(m)]
// #undef rxb 
// #define rxb(i,m,n) rbuff[(i)+nqs[pp]*((m)+domainDimension*(n+1))]

      bool *passigned = new bool [max(1,numPoints)];
#define assigned(i) passigned[(i)-base]
      for( int i=base; i<=bound; i++ )
	assigned(i)=false;
    
      real rv[3], rxv[3][3];
      for( int p=0; p<npra; p++ )
      {
	const int pp=mapra(p);
	real *rbuff=rab[p];
        int k=0;
	for( int j=0; j<nqs[pp]; j++ )
	{
	  i = ip(j,pp);  // the result goes here 
	  assert( i>=base && i<=bound );

	  for( int m=0; m<domainDimension; m++ ){ rv[m] = rbuff[k]; k++; }
          if( computeMapDerivative )
	  {
            for( int m=0; m<domainDimension; m++ )for( int n=0; n<rangeDimension; n++ )
	    {
	      rxv[m][n]= rbuff[k]; k++;
	    }
	  }

	  if( assigned(i) ) 
	  {
	    // this points has already been assigned, we should choose the best inverse

	    // For now: check the distance to the centre of the unit square and choose the
	    //   point with the least distance
	    real dist=0., distNew=0.;
	    for( int m=0; m<domainDimension; m++ )
	    {
	      dist += fabs(r(i,m)-.5);
	      distNew+= fabs(rv[m]-.5);
	    }
	    if( distNew<dist )
	    {
	      assigned(i)=false;  // this will cause the values to be assigned below
	    }
	  }
	  if( !assigned(i) )
	  {
	    assigned(i)=true;
	    if( computeMap )
	    {
	      for( int m=0; m<domainDimension; m++ ) R(i,m) = rv[m];

	      if( debug & 1 )
	      {
		fprintf(pDebugFile,"inverseMap: myid=%i RESULTS for point i=%i : x=(%g,%g,%g), r=(%g,%g,%g)\n",myid,i,
		       X(i,0),(rangeDimension>1 ? X(i,1) : 0.), (rangeDimension>2 ? X(i,2) : 0.),
		       R(i,0),(domainDimension>1 ? R(i,1) : 0.), (domainDimension>2 ? R(i,2) : 0.));
	      }
	    }
	  
	    if( computeMapDerivative )
	    {
	      for( int m=0; m<domainDimension; m++ )for( int n=0; n<rangeDimension; n++ )
	      {
	        RX(i,m,n) = rxv[m][n];
	      }
	    }
	  
	  }
	
	  i++;
	} // end for j
	assert( k==nqs[pp]*numDataToReceivePerPoint );
	
      }

      // any points not assigned but have been outside the bounding box. *wdh* 081002
      for( int i=base; i<=bound; i++ )
      {
	if( !assigned(i) )
	{
	  if( computeMap )
	  {
	    for( int m=0; m<domainDimension; m++ ) R(i,m) = Mapping::bogus;
	  }
	}
      }
      

      if( debug>0 )
      {
	fprintf(pDebugFile,"inverseMap:DONE: myid=%i\n",myid);
	fflush(pDebugFile);
      }
      
      // wait for ALL sends to finish on this processor before we can clean up
      MPI_Waitall(nps,sendRequest,sendStatus);
      MPI_Waitall(npsa,sendRequestAnswer,sendStatusAnswer);

      delete [] pinfo;
      delete [] numDataToSendPerQueryPoint;
      delete [] nqs;
      delete [] nqr;

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
    
      delete [] passigned;
      
    }
    else
    {
      exactLocalInverse->inverse( x,r,rx,workSpace,FALSE );
    }
    
  }
  
#endif

}

