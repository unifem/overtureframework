#include "DataPointMapping.h"
#include "ParallelUtility.h"
#include "display.h"
#include "Inverse.h"          
#include "DistributedInverse.h"
#include "SparseArray.h"
#include "NurbsMapping.h"

// ====================================================================================================
/// \brief Evaluate a DataPointMapping Mapping (in parallel)
/// \detail This function knows how to evaluate a DataPointMapping. In parallel, the grid associated
///   with the DataPointMapping may be distributed and thus the evaluation could require communication 
///   to the processor that owns the data points needed to evaluate the mapping.
// ===================================================================================================
void DataPointMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
{
  if( evalAsNurbs )
  {
    // --- Use a Nurbs to evaluate the mapping ---
    if( nurbsOutOfDate )
      generateNurbs();

    if( false )
      printF("--DPM-- mapS : eval as a NurbsMapping\n");
    
    NurbsMapping & nurbs = *dbase.get<NurbsMapping*>("nurbs");
    nurbs.mapS(r,x,xr,params);
    return;
  }



  if( params.coordinateType != cartesian )
    cerr << "DataPointMapping::map - coordinateType != cartesian " << endl;


#ifndef USE_PPP
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  mapScalar( r,x,xr,params,base,bound,computeMap,computeMapDerivative );
  return;
#else

  // ****************************************
  // ******* Parallel map function **********
  // ****************************************

  // ****** NOTE: this code is very similar to that in inverseMap.C *********

  int debug=0; // 1; // Mapping::debug; // 3;
  
  if( debug >0 )
    openDebugFiles();


  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  const MPI_Comm & OV_COMM = Overture::OV_COMM;
  
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);

  const real * rp = r.Array_Descriptor.Array_View_Pointer1;
  const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
#undef X
  real * xp = x.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=x.getRawDataSize(0);
#define X(i0,i1) xp[i0+xDim0*(i1)]
  real * xrp = xr.Array_Descriptor.Array_View_Pointer2;
  const int xrDim0=xr.getRawDataSize(0);
  const int xrDim1=xr.getRawDataSize(1);
#undef XR
#define XR(i0,i1,i2) xrp[i0+xrDim0*(i1+xrDim1*(i2))]


  if( debug & 1 )
  {
    fprintf(pDebugFile,"--------------- DPM:mapS: Start myid=%i base=%i bound=%i computeMap=%i computeDer=%i name=%s --------   \n",
	    myid,base,bound,computeMap,computeMapDerivative,
	    (const char*)getName(mappingName));
  }
      
  realArray & vertex = xy;  //   this array determines the parallel distribution 

  int numPoints=bound-base+1;

  int *nqs = new int [np];  // nqs[p] : number of queries to send to processor p
  int *nqr = new int [np];  // nqs[p] : number of queries received from processor p
  for( int p=0; p<np; p++ ) nqs[p]=0;
    
  // ip(j,p) = i, j=0,..,nqs[p] : means x(i,.) is the j'th point sent to processor p 
  SparseArray<int> ip(numPoints,np);

//   int *pip=NULL;
//   if( numPoints>0 )
//     pip = new int [numPoints*np];    // ********* this is too large in general, use a SparseArray ? 
// #define ip(j,p) (pip[(p)+np*(j)])

  real dr[3];
  for( int axis=0; axis<3; axis++ )
    dr[axis]=1./max(gridIndexRange(1,axis)-gridIndexRange(0,axis),1);

  int iv[3]={0,0,0}; //
  for( int i=base; i<=bound; i++ )
  {
    for( int axis=0; axis<domainDimension; axis++ )
    { // index = index of closest grid point on the donor grid -- is this correct ? 
      real rval =R(i,axis);
      if( isPeriodic[axis] ) rval=fmod(rval+1.,1.); // periodic wrap to [0,1]

      iv[axis] = int( rval/dr[axis]+ gridIndexRange(0,axis) +.5 );
    }

    int p = vertex.Array_Descriptor.findProcNum( iv );    // index iv[] is on processor p 

    ip.get(nqs[p],p)=i;  // this point should be sent to processor p for an answer 
    nqs[p]++;
  }
  if( debug & 1 )
  {
    for( int p=0; p<np; p++ ) 
      fprintf(pDebugFile,"DPM:mapS: myid=%i p=%i nqs[p]=%i\n",myid,p,nqs[p]);
  }
  

  // --- send/receive number of queries ---
  // we need to send computeMap and computeMapDerivative since these may not be the
  // same on all processors!

  int myInfo[3]={0,computeMap,computeMapDerivative}; //
  // info(0:2,p) : holds (nqr[p],computeMap[p],computeMapDerivative[p])
  int *pinfo = new int [3*np];
#define info(i,p) pinfo[(i)+3*(p)]

  MPI_Status status;
  const int tag0=309278;
  int numberOfQueries=0;      // total number of queries that we will receieve 
  for( int p=0; p<np; p++ )
  {
    // *** send computeMap computeMapDerivative here too !

    int tags=tag0+p, tagr=tag0+myid;
    myInfo[0]=nqs[p];
    MPI_Sendrecv(myInfo,     3, MPI_INT, p, tags, 
		 &info(0,p), 3, MPI_INT, p, tagr, OV_COMM, &status ); 

    nqr[p]=info(0,p);
    if( debug & 1 ) 
    {
      fprintf(pDebugFile,"DPM:mapS: myid=%i send %i queries to p=%i (map=%i,der=%i), "
	      "receive %i queries from p=%i, p: (map=%i der=%i)\n",
	      myid,nqs[p],p,computeMap,computeMapDerivative, nqr[p],p,info(1,p),info(2,p));
      fflush(pDebugFile);
    }
	
    numberOfQueries += nqr[p];
  }

  // For now -- we compute r and rx for the queries if there is ANY processor wants these results,
  // but we do not return rx if the requesting processor does not want them.

  int computeMapAnswer=false, computeMapDerivativeAnswer=false;
  for( int p=0; p<np; p++ )
  {
    computeMapAnswer = computeMapAnswer || info(1,p);
    computeMapDerivativeAnswer = computeMapDerivativeAnswer || info(2,p);
  }

  // --------------------------------------------------------------------
  // npr  = number of processors that we will receive queries from 
  // nps  = number of processors that we will send queries to
  // npra = number of proc. that we will rec. answers from  (=nps)
  // npsa = number of proc. that we will send answer to     (=npr)
  // --------------------------------------------------------------------

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

  MPI_Request *receiveRequest       = new MPI_Request[npr];  
  MPI_Status  *receiveStatus        = new MPI_Status [npr];  

  MPI_Request *sendRequest          = new MPI_Request[nps];  
  MPI_Status  *sendStatus           = new MPI_Status [nps];  

  MPI_Request *receiveRequestAnswer = new MPI_Request[npra];  
  MPI_Status  *receiveStatusAnswer  = new MPI_Status [npra];

  MPI_Request *sendRequestAnswer    = new MPI_Request[npsa];  
  MPI_Status  *sendStatusAnswer     = new MPI_Status [npsa];  


  // ******* BUFFERS ************
  //  sqb[p] : send queries buffer -- send queries to processor p
  //  rqb[p] : receive queries buffer
  //
  //  sqb[p] : send answers buffer -- send answers to processor p
  //  rqb[p] : receieve answers buffer 

  const int numDataToSendPerPoint=domainDimension;  // we send r 

  // --- Allocate receive buffers ---
  real **rqb = new real* [npr];
  for( int p=0; p<npr; p++ )
  {
    const int pp =mapr(p);
    rqb[p] = new real [max(1,nqr[pp]*numDataToSendPerPoint)];
  }
  
  // --- post receives for queries ---
  const int tag1=2529743;
  for( int p=0; p<npr; p++ )
  {  
    const int pp =mapr(p);
    int tag=tag1+myid;
    MPI_Irecv( rqb[p], nqr[pp]*numDataToSendPerPoint, MPI_Real, pp, tag, OV_COMM, &receiveRequest[p] );
  }

  // -- allocate send buffers for queries ---

  // pxs[p](i,m) : holds r values m=0,1,...,domainDimension-1
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
      for( int m=0; m<domainDimension; m++ )
      {
	real rval =R(i,m);
	if( isPeriodic[m] ) rval=fmod(rval+1.,1.); // periodic wrap to [0,1]
	sbuff[k]=rval; k++; // rs(j,m,p) = rval;
      }
    }
    assert( k==nqs[pp]*numDataToSendPerPoint );
  }
    
  // --- send buffers for queries ---
  for( int p=0; p<nps; p++ )
  {
    const int pp=maps(p);
    if( debug & 1 ) 
    {
      fprintf(pDebugFile,"DPM:mapS: Send %i queries from myid=%i to p=%i\n",nqs[pp],myid,pp);
      if( debug & 2 ) 
      {
	fprintf(pDebugFile,"data=");
	for( int i=0; i<nqs[pp]*numDataToSendPerPoint; i++ ) fprintf(pDebugFile,"%g ",sqb[p][i]);
	fprintf(pDebugFile,"\n");
      }
      fflush(pDebugFile);
    }
	
    int tag=tag1+pp;
    MPI_Isend(sqb[p], nqs[pp]*numDataToSendPerPoint, MPI_Real, pp, tag, OV_COMM, &sendRequest[p] );  
  }
  
  // --- wait for all the receives to finish ---
  MPI_Waitall(npr,receiveRequest,receiveStatus);


  if( debug & 2 )
  { // sanity check:
    // fflush(0);
    // Communication_Manager::Sync();

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
  const int numDataToReceivePerPoint = rangeDimension + domainDimension*rangeDimension*int(computeMapDerivative);

  real **rab = new real* [npra];  // rab = "answers" 
  for( int p=0; p<npra; p++ )
  {
    // return x, and optionally xr values
    const int pp=mapra(p);

    // we receive back the number we sent: nqs[pp] 
    rab[p] = new real [max(1,nqs[pp]*numDataToReceivePerPoint)];
  }

  const int tag2=1329743;
  for( int p=0; p<npra; p++ )
  {  
    const int pp=mapra(p);
    int tag=tag2+myid;
    if( debug & 1 )
    {
      fprintf(pDebugFile,"DPM:mapS: myid=%i, expect %i answers from p=%i\n",myid,nqs[pp],pp);
      fflush(pDebugFile);
    }
	
    MPI_Irecv( rab[p], nqs[pp]*numDataToReceivePerPoint, MPI_Real, pp, tag, OV_COMM, &receiveRequestAnswer[p] );
  }

  // ------------------------------------
  // --- un-pack the values received ----
  // ------------------------------------

  RealArray xc,rc,xrc;
  if( numberOfQueries>0 )
  {
    xc.redim(numberOfQueries,rangeDimension);
    rc.redim(numberOfQueries,domainDimension);

    if( computeMapDerivativeAnswer )
      xrc.redim(numberOfQueries,rangeDimension,domainDimension); 
  }

  real * rcp = rc.Array_Descriptor.Array_View_Pointer1;
  const int rcDim0=rc.getRawDataSize(0);
#undef RC
#define RC(i0,i1) rcp[i0+rcDim0*(i1)]
#undef XC
  real * xcp = xc.Array_Descriptor.Array_View_Pointer1;
  const int xcDim0=xc.getRawDataSize(0);
#define XC(i0,i1) xcp[i0+xcDim0*(i1)]
  real * xrcp = xrc.Array_Descriptor.Array_View_Pointer2;
  const int xrcDim0=xrc.getRawDataSize(0);
  const int xrcDim1=xrc.getRawDataSize(1);
#undef XRC
#define XRC(i0,i1,i2) xrcp[i0+xrcDim0*(i1+xrcDim1*(i2))]


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
      // for( int n=0; n<rangeDimension; n++ )  XC(i,n) = xrb(j,n,p); 
      for( int m=0; m<domainDimension; m++ ){ RC(i,m) = rbuff[k]; k++; } //  RC(i,m) = rrb(j,m,p);
      i++;
    }
    assert( k==nqr[pp]*numDataToSendPerPoint );
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
      fprintf(pDebugFile,"DPM:mapS: myid=%i eval point i=%i : rc=(%g,%g,%g) on this processor.\n",myid,i,
	      RC(i,0),(domainDimension>1 ? RC(i,1) : 0.), (domainDimension>2 ? RC(i,2) : 0.));

    }
    fflush(pDebugFile);
    Communication_Manager::Sync();
    MPI_Barrier(OV_COMM);
	
  }
	
  Index J=Range(numberOfQueries);


  // pass base,bound,computeMap,computeMapDerivative 

  // **********************************
  // ****** Evaluate the Mapping ******
  // **********************************
  if( numberOfQueries>0 )
  {
    mapScalar( rc,xc,xrc,params, 0,numberOfQueries-1,computeMapAnswer,computeMapDerivativeAnswer );
//    mapScalar( rc,xc,xrc,params, 0,numberOfQueries-1,computeMapAnswer,computeMapDerivativeAnswer );

    if( false )
    {
      ::display(rc,"--DPM-- rc INPUT for mapScalar",pDebugFile,"%6.3f ");
      ::display(xc,"--DPM-- xc OUTPUT from mapScalar",pDebugFile,"%6.3f ");
    }
    
  }
  


  if( debug & 1 )
  {
    for( int i=0; i<numberOfQueries; i++ )
    {
      fprintf(pDebugFile,"DPM:mapS: myid=%i after mapScalar, point i=%i : xc=(%g,%g,%g), rc=(%g,%g,%g)\n",myid,i,
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
    // we send back x and optionally xr depending on the processor that requested the answer
    numDataToSendPerQueryPoint[p]=rangeDimension + info(2,pp)*domainDimension*rangeDimension;
  }

  real **sab = new real* [npsa];  // sab = send-answers buffer
  for( int p=0; p<npsa; p++ )
  {
    // we send back the number we received : nqr[pp]
    const int pp=mapsa(p);
    sab[p] = new real [max(1,nqr[pp]*numDataToSendPerQueryPoint[p])];
  }


  i=0;
  for( int p=0; p<npsa; p++ )
  {
    const int pp=mapsa(p);
    real *sbuff = sab[p];
    int k=0;
    for( int j=0; j<nqr[pp]; j++ )
    {
      for( int m=0; m<rangeDimension; m++ ){ sbuff[k]= XC(i,m); k++; } //  rrb(j,m,p) = XC(i,m);
      if( info(2,pp) ) // true if we are to computeMapDerivative for processor p  *wdh* fixed p -> pp 2011/08/22
      {
	for( int m=0; m<domainDimension; m++ )for( int n=0; n<rangeDimension; n++ ) 
	{
	  sbuff[k]= XRC(i,m,n); k++; //   rxb(j,m,n,p) = XRC(i,m,n);
	}
      }
      i++;
    }
    if( k!=nqr[pp]*numDataToSendPerQueryPoint[p] )
    {
      if( pDebugFile!=NULL )
      {
	fprintf(pDebugFile,"DPM::mapS:ERROR: k=%i != nqr[pp]*numDataToSendPerQueryPoint[p]=%i, nqr[pp]=%i, numDataToSendPerQueryPoint[p]=%i\n"
		" info(2,pp)=%i, pp=%i\n",k,nqr[pp]*numDataToSendPerQueryPoint[p],nqr[pp],numDataToSendPerQueryPoint[p],info(2,pp),pp);
      }
      else
      {
	printf("DPM::mapS:ERROR: myid=%i, k=%i != nqr[pp]*numDataToSendPerQueryPoint[p]=%i, nqr[pp]=%i, numDataToSendPerQueryPoint[p]=%i\n"
		" info(2,pp)=%i, pp=%i\n",myid,k,nqr[pp]*numDataToSendPerQueryPoint[p],nqr[pp],numDataToSendPerQueryPoint[p],info(2,pp),pp);
      }
      fflush(0);
      
      OV_ABORT("error");
    }
  }

  // -------------------------------------------
  // --- Send buffers with query results     ---
  // -------------------------------------------

  for( int p=0; p<npsa; p++ )
  {
    const int pp=mapsa(p);
    int tag=tag2+pp;
    if( debug & 1 )
    {
      fprintf(pDebugFile,"DPM:mapS: myid=%i, send %i answers to p=%i\n",myid,nqr[pp],pp);
      fflush(pDebugFile);
    }
	
    MPI_Isend(sab[p], nqr[pp]*numDataToSendPerQueryPoint[p], MPI_Real, pp, tag, OV_COMM, &sendRequestAnswer[p] );  
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

  // --- assign results back into the r,xr arrays ---

  real xv[3], xrv[3][3];;
  for( int p=0; p<npra; p++ )
  {
    const int pp=mapra(p);
    real *rbuff=rab[p];
    int k=0;
    for( int j=0; j<nqs[pp]; j++ )
    {
      i = ip(j,pp);  // the result goes here 
      assert( i>=base && i<=bound );
	  
      for( int m=0; m<domainDimension; m++ ){ xv[m] = rbuff[k]; k++; }
      if( computeMapDerivative )
      {
	for( int m=0; m<domainDimension; m++ )for( int n=0; n<rangeDimension; n++ )
	{
	  xrv[m][n]= rbuff[k]; k++;
	}
      }
      if( computeMap ) // only fill in result for x if requested 
      {
	for( int m=0; m<rangeDimension; m++ ) X(i,m) = xv[m];
	if( debug & 1 )
	{
	  fprintf(pDebugFile,"DPM:mapS: myid=%i RESULTS for point i=%i : x=(%g,%g,%g), r=(%g,%g,%g)\n",myid,i,
		  X(i,0),(rangeDimension >1 ? X(i,1) : 0.), (rangeDimension >2 ? X(i,2) : 0.),
		  R(i,0),(domainDimension>1 ? R(i,1) : 0.), (domainDimension>2 ? R(i,2) : 0.));
	}
      }
      if( computeMapDerivative ) // only fill in result for xr if requested 
      {
	for( int m=0; m<domainDimension; m++ )for( int n=0; n<rangeDimension; n++ )
	{
	  XR(i,m,n) = xrv[m][n];
	}
      }
      i++;
    } // end for j
    assert( k==nqs[pp]*numDataToReceivePerPoint );   
  }

  if( debug>0 )
  {
    fprintf(pDebugFile,"DPM:mapS:DONE: myid=%i\n",myid);
    fflush(pDebugFile);
  }
  if( false )
  {
    ::display(r,"--DPM-- dpmMap: r","%6.3f ");
    ::display(x,"--DPM-- dpmMap: x","%6.3f ");
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

    
  return;

#endif
}
