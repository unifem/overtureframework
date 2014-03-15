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

//    ************************************
//    ******** Parallel version **********
//    ************************************

namespace
{
// This struct holds the data needed to make a boundary adjustment query

struct AdjustBoundaryQueryData
{

real xv[3]; // point to adjust -- note: put doubles first for memory alignment
int i;      // index into original list -- use this for checking, can eventually be removed.
int ip[3];  // index space coordinates

AdjustBoundaryQueryData& operator=(const AdjustBoundaryQueryData& x)
{ xv[0]=x.xv[0]; xv[1]=x.xv[1]; xv[2]=x.xv[2]; i=x.i; 
  ip[0]=x.ip[0]; ip[1]=x.ip[1]; ip[2]=x.ip[2]; return *this; }


};

// here is the info that we send back from a boundary adjustment query
struct AdjustBoundaryResultData
{
  real dx[3]; // corrections to the positions
  int i;      // index into original list -- use this for checking, can eventually be removed.
};


}


int Ogen::
adjustBoundary(CompositeGrid & cg,
               const Integer&      k1,
	       const Integer&      k2,
	       const intSerialArray&     ip,
	       const realSerialArray&    x) 
// ==========================================================================================
// /Description:
//
//    Adjust the position x of points ip of grid k1 interpolated from
//  base grid k2 to take into account mismatch between shared boundaries.
// 
//
// The basic correction mechanism is defined by
// \begin{verbatim}
//   x(R,Rx) += boundaryAdjustment(i(R),i2(R),i3(R),Rx) \cdot [ 
//            acrossGrid(ip(R),i2(R),i3(R),Rx) \cdot ( oppositeBoundary(ip(R),i2(R),i3(R),Rx)- x(R,Rx) )  ]
//   
// \end{verbatim}
//
// ==========================================================================================
{

#ifndef USE_PPP
  return adjustBoundarySerial(cg,k1,k2,ip,x);

#else

  // if( true ) return 0; // *****************
  
  if( debug & 4 )
  {
    fprintf(plogFile,"\n ***** adjustBoundary:START(%i,%i): adjust pts on grid=%i that interp from grid=%i ****\n\n",
            k1,k2,k1,k2);
  }

  BoundaryAdjustmentArray2 & boundaryAdjustment = cg.rcData->boundaryAdjustment;

  if (boundaryAdjustment.getNumberOfElements()) 
  {
    BoundaryAdjustmentArray& bA12 = boundaryAdjustment(k1,k2);
    if( bA12.getNumberOfElements()>0 ) 
    {

      const int np=Communication_Manager::numberOfProcessors();
      const MPI_Comm & OV_COMM = Overture::OV_COMM;
  
      const int numberOfDimensions = cg.numberOfDimensions();
      const int numberOfGrids = cg.numberOfComponentGrids();

      const IntegerArray & gridIndexRange = cg[k1].gridIndexRange();
      const int i3a=gridIndexRange(0,2);

      // adjustFace(side,axis) = true if this face causes adjustments
      // obIndex(side,axis) : index value for "opposite boundary" -- indicates whcih pts get an adjustment

      int padjustFace[6], pobIndex[6];
      #define adjustFace(side,axis) padjustFace[(side)+2*(axis)]
      #define obIndex(side,axis) pobIndex[(side)+2*(axis)]
    
      int numberOfSidesAdjusted=0;
      for( int kd1=0; kd1<numberOfDimensions; kd1++ )
      {
	for( int ks1=0; ks1<2; ks1++ )
	{
	  BoundaryAdjustment& bA = bA12(ks1,kd1);
	  if ((bA.computedGeometry & (THEinverseMap | THEmask)) == 
              (THEinverseMap | THEmask) && bA.hasSharedSides()==BoundaryAdjustment::share ) 
	  {
            adjustFace(ks1,kd1)=true;
	    obIndex(ks1,kd1)=oppositeBoundaryIndex( cg[k1],ks1,kd1 ); 
	    numberOfSidesAdjusted++;

	  }
          else
	  {
            adjustFace(ks1,kd1)=false;
	  }
	  
	}
      }
      // We should check in parallel that all processors agree with this: 
      // Who sets bA.hasSharedSides() ? 
      if( debug & 2 )
      {
        int numAdjusted = ParallelUtility::getSum(numberOfSidesAdjusted);
	if( numAdjusted != numberOfSidesAdjusted*np )
	{
	  printF("adjustBoundary:ERROR:  not all processors agree on numberOfSidesAdjusted!\n");
          fprintf(plogFile,"adjustBoundary:ERROR:  not all processors agree on numberOfSidesAdjusted=%i!\n",
              numberOfSidesAdjusted);
          fflush(0);
	  Overture::abort("error");
	}
      }
      if( numberOfSidesAdjusted==0 )
      {
        if( debug & 4 )
	{
  	  printF("adjustBoundary:WARNING: There are no sides to be adjusted but boundaryAdjustment(k1=%i,k2=%i) is non-empty\n",
                 k1,k2);
          fprintf(plogFile,
            "adjustBoundary:WARNING: There are no sides to be adjusted but boundaryAdjustment(k1=%i,k2=%i) is non-empty\n",
                    k1,k2);
	}
	return 0;
      }

      // If multiple sides on the same grid are adjusted, we need to keep a copy of 
      // the original un-adjusted values

      Range Rx=numberOfDimensions;
      Range R=ip.dimension(0);

      RealArray x0;
      if( numberOfSidesAdjusted>1 )
      {
	x0.redim(R,Rx); x0=x(R,Rx);  // copy of original 
      }
      else
	x0.reference(x(R,Rx));             // just reference 

      const int rBase=R.getBase(), rBound=R.getBound();
      real  *xp = x.Array_Descriptor.Array_View_Pointer1; // x.getDataPointer(x);
      const int xDim0=x.getRawDataSize(0);
#define   X(i,m)   xp[i+xDim0*(m)]

      const int *ipp = ip.Array_Descriptor.Array_View_Pointer1; // getDataPointer(ip);
      const int ipDim0=ip.getRawDataSize(0); 
#define  IP(i,m)  ipp[i+ipDim0*(m)]

      const real *x0p =x0.Array_Descriptor.Array_View_Pointer1; // getDataPointer(x0); 
      const int x0Dim0=x0.getRawDataSize(0);
#define  X0(i,m)  x0p[i+x0Dim0*(m)]

//       const int rBase=R.getBase(), rBound=R.getBound(), rDim=rBound-rBase+1;
//       const int xBase=x.getBase(0), xDim=x.getRawDataSize(0);
//       const int ipBase=ip.getBase(0), ipDim=ip.getRawDataSize(0);

//       const real *x0p =x0.Array_Descriptor.Array_View_Pointer1; // getDataPointer(x0);
//       real  *xp       = x.Array_Descriptor.Array_View_Pointer1; // getDataPointer(x);
//       const int *ipp = ip.Array_Descriptor.Array_View_Pointer1; //getDataPointer(ip);
// #define  X0(i,m)  x0p[i-rBase+rDim*(m)]
// #define   X(i,m)   xp[i-xBase+xDim*(m)]
// #define  IP(i,m)  ipp[i-ipBase+ipDim*(m)]

      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];

      // We locate the processor a point is on using the mask array: 
      const intArray & mask = cg[k1].mask();

      int *nqs = new int [np];  // nqs[p] : number of queries to send to processor p
      int *nqr = new int [np];  // nqs[p] : number of queries received from processor p
    
      // ============================================
      // ===== define the MPI derived datatypes =====
      // ============================================
      // --NOTE: put doubles first in struct to align in memory

      MPI_Datatype ABQueryDataType, ABResultDataType, oldTypes[2];
      MPI_Aint offsets[2], extent;
      int blockCounts[2];

      // --- define the mpi QUERY data type ---
      offsets[0]    = 0;
      oldTypes[0]   = MPI_Real;            // NOTE: Use MPI_Real  (not MPI_REAL == MPI_FLOAT)
      blockCounts[0]= 3;                   // number of real's

      MPI_Type_extent(oldTypes[0], &extent);

      offsets[1]    = blockCounts[0]*extent;
      oldTypes[1]   = MPI_INT;
      blockCounts[1]= 4;      // number of int's in ABQueryData

      MPI_Type_struct(2, blockCounts, offsets, oldTypes, &ABQueryDataType);
      MPI_Type_commit(&ABQueryDataType);

      // --- define the mpi RESULT data type ---
      offsets[0]    = 0;
      oldTypes[0]   = MPI_Real;            // NOTE: Use MPI_Real  (not MPI_REAL == MPI_FLOAT)
      blockCounts[0]= 3;                   // number of real's

      MPI_Type_extent(oldTypes[0], &extent);

      offsets[1]    = blockCounts[0]*extent;
      oldTypes[1]   = MPI_INT;
      blockCounts[1]= 1;     

      MPI_Type_struct(2, blockCounts, offsets, oldTypes, &ABResultDataType);
      MPI_Type_commit(&ABResultDataType);


      // ia(j) = i : indirection array for points that actually need adjustment (points far from the
      //             the boundary don't need adjustment)
      IntegerArray ia(ip.getLength(0)); // indirection array


      MPI_Request *receiveRequest = new MPI_Request[np];  
      MPI_Request *sendQueryRequest = new MPI_Request[np];  
      MPI_Request *sendResultsRequest = new MPI_Request[np];  
      MPI_Status *receiveStatus= new MPI_Status[np];  


      // ==========================================================
      // ============ Adjustments for FACE (ks1,kd1) ==============
      // ==========================================================

      // NOTE: we could probably do adjustments for all faces at once to avoid extra communication
      //   but this is probably a rare event when multiple sides need adjustment. It is a bit tricky 
      //   to do all at faces at once since some points will go to multiple processors for adjustments

      for( int kd1=0; kd1<numberOfDimensions; kd1++ )for( int ks1=0; ks1<2; ks1++ )if( adjustFace(ks1,kd1) )
      {
        const int ibkd1=cg[k1].gridIndexRange(ks1,kd1); // index on the boundary
	const int is1=2*ks1-1;

	// -- Count the number of queries that should be sent to processor p for any face ---

	int numberToCheck=0;
	for( int i=rBase; i<=rBound; i++ )
	{
	  if( (IP(i,kd1)-obIndex(ks1,kd1))*is1>0  ) // only pts close to the boundary are adjusted
	  {
	    ia(numberToCheck)=i;
	    numberToCheck++;
	  }
	}
      
	// processorToCheck[j] = p if point i=ia(j) should be sent to processor p: 
	int *processorToCheck = new int[max(1,numberToCheck)];    

	// **** BUFFERS ****
	//   sendQueries     : queries to be sent
	//   receiveQueries  : queries recieved here
	//   sendResults     : results saved here for sending 
	//   receiveResults  : results returned here

	// queries are put here for sending:
	AdjustBoundaryQueryData *sendQueries = new AdjustBoundaryQueryData[max(1,numberToCheck)];
	// results are returned here: 
	AdjustBoundaryResultData *receiveResults = new AdjustBoundaryResultData[max(1,numberToCheck)];

	// Count the number of queries (for ANY faces) that should be sent to processor p 
	for( int p=0; p<np; p++ ) nqs[p]=0;

	for( int j=0; j<numberToCheck; j++ )
	{
	  int i=ia(j);
	  AdjustBoundaryQueryData & send = sendQueries[j];

	  send.i=i;
	  send.ip[0]=IP(i,0); send.ip[1]=IP(i,1); 
	  send.xv[0]= X0(i,0); send.xv[1]= X0(i,1); // pass ORIGINAL location 
          if( numberOfDimensions==3 )
	  {
            send.ip[2]=IP(i,2); send.xv[2]= X0(i,2); 
	  }
	  else
	  {
            send.ip[2]=i3a; send.xv[2]=0.;
	  }
	  

	  send.ip[kd1]=ibkd1;  // project to the boundary 
          int ivv[4]={ send.ip[0],send.ip[1],send.ip[2],0};  // 
          // int p = mask.Array_Descriptor.findProcNum( send.ip );    // index is on processor p 
          int p = mask.Array_Descriptor.findProcNum( ivv );    // index is on processor p 
	  send.ip[kd1]=IP(i,kd1); // reset 

	  processorToCheck[j] = p;
	  nqs[p]++;
	}
      
	if( debug & 4 )
	{
	  for( int p=0; p<np; p++ ) 
	    fprintf(plogFile,"AB: (grid1,grid2)=(%i,%i) (ks1,kd1)=(%i,%i) : Send to p=%i nqs[p]=%i\n",
                    k1,k2,ks1,kd1,p,nqs[p]);
	}

	// --- send/receive number of queries ---

	MPI_Status status;
	const int tag0=104288;
	int numberOfQueries=0;      // total number of queries that we will receive 
	for( int p=0; p<np; p++ )
	{
	  int tags=tag0+p, tagr=tag0+myid;
	  MPI_Sendrecv(&nqs[p], 1, MPI_INT, p, tags, 
		       &nqr[p], 1, MPI_INT, p, tagr, OV_COMM, &status ); 

	  if( debug & 4 )
	  {
	    fprintf(plogFile,"AB: myid=%i send %i queries to p=%i, receive %i queries from p=%i\n",
		    myid,nqs[p],p, nqr[p],p);
	    fflush(plogFile);
	  }
	  numberOfQueries += nqr[p];
	}

	//   receiveQueries  : queries recieved here
	//   sendResults     : results saved here for sending 

	AdjustBoundaryQueryData *receiveQueries = new AdjustBoundaryQueryData[max(1,numberOfQueries)];
	AdjustBoundaryResultData *sendResults = new AdjustBoundaryResultData[max(1,numberOfQueries)];


	// --- Post receives for the queries that we can perform on this processor ----
	const int tag1=113623;
	int loc=0;
        int numQueriesReceived=0;
	for( int p=0; p<np; p++ )
	{  
	  if( nqr[p] > 0 )
	  {
	    int tag=tag1+myid;
	    MPI_Irecv( &(receiveQueries[loc]), nqr[p], ABQueryDataType, p, tag, OV_COMM, 
                       &receiveRequest[numQueriesReceived] );
	    loc+=nqr[p];
            numQueriesReceived++;
	  }
	}
	
	// ---- Send data to the processor where it can be checked ----

        int numQueriesSent=0;  // count the number of different processors that we will send queries to.
	for( int p=0; p<np; p++ )  // Send to processor p 
	{
	  if( nqs[p]>0 )
	  {
	    numQueriesSent++;
	  }
	}
	
	// Send data as an index type from the single array of queries
	MPI_Datatype IndexedQueryDataType;

	// Here is the indexed return data type: (to return the data in the order it was sent)
	MPI_Datatype *IndexedResultDataType = NULL;
	if( numQueriesSent>0 )
           IndexedResultDataType = new MPI_Datatype [numQueriesSent];

        int numSent=0;
	for( int p=0; p<np; p++ )  // Send to processor p 
	{
	  if( nqs[p]>0 )
	  {
	    int num=-1;
	    int *blocklen = new int [max(1,nqs[p])];
	    int *displacement = new int [max(1,nqs[p])];
	    if( nqs[p]>0 )
	    {
	      for( int n=0; n<numberToCheck; n++ )
	      {
		// collect all queries that go to processor p (collect blocks of queries that all go to the same p)
		if( processorToCheck[n]==p )
		{
		  if( n==0 || processorToCheck[n]!=processorToCheck[n-1] )
		  {
		    num++;
		    displacement[num]=n;  // index of first entry in this block
		    blocklen[num]=1;      // number of entries in this block
		  }
		  else
		    blocklen[num]++;
		}
	      }
	    }
	    num++;
    
	    // MPI_Datatype IndexedQueryDataType;
	    MPI_Type_indexed(num, blocklen, displacement, ABQueryDataType, &IndexedQueryDataType);
	    MPI_Type_commit(&IndexedQueryDataType);     

	    // also create the indexed return type which uses the same block structure as the query data
	    MPI_Type_indexed(num, blocklen, displacement, ABResultDataType, &IndexedResultDataType[numSent]);
	    MPI_Type_commit(&IndexedResultDataType[numSent]);
    
	    if( debug & 4 ) fprintf(plogFile,"AB>> myid=%i send %i queries to p=%i \n"
				    "     (number of blocks=%i, blocklen[0]=%i displacement[0]=%i)\n",
				    myid,nqs[p],p,num,blocklen[0],displacement[0]);
    
	    int tag=tag1+p;
	    // , 1, -- means we send one indexed type: 
	    MPI_Isend(sendQueries, 1, IndexedQueryDataType, p, tag, OV_COMM, &sendQueryRequest[numSent] );  

	    delete [] blocklen;                      // is this ok to do here?
	    delete [] displacement; 
	    MPI_Type_free( &IndexedQueryDataType );  // This IS OK according to the MPI  documentation

            numSent++;
	  }
	} // end for p
	assert( numSent==numQueriesSent );
  
	if( debug & 2 )
	{
	  fflush(plogFile);
	  Communication_Manager::Sync();
	}
  
  
	// --- wait for all the receives to finish ---
	if( numQueriesReceived>0 )
  	  MPI_Waitall(numQueriesReceived,receiveRequest,receiveStatus);
  
	if( debug & 4 )
	{
	  loc=0;
	  for( int p=0; p<np; p++ )
	  {  
	    if( nqr[p]>0 )
	    {
	      fprintf(plogFile,"AB: received the following %i queries from processor p=%i\n",nqr[p],p);
	      for( int i=0; i<nqr[p]; i++ )
	      {
		AdjustBoundaryQueryData & q = receiveQueries[loc+i];
		fprintf(plogFile,"AB: i=%i ip=(%i,%i,%i) x=(%8.2e,%8.2e,%8.2e,) \n",q.i,q.ip[0],q.ip[1],q.ip[2],
			q.xv[0],q.xv[1],q.xv[2]);
	      }
	      loc+=nqr[p];
	    }
	  }
	  fflush(plogFile);
	  Communication_Manager::Sync();
	}
  

	// --- post receives for the return data  ---
	const int tag2=370615;
        numSent=0;
	for( int p=0; p<np; p++ )
	{
	  if( nqs[p]>0 )
	  {
	    // return the results in the same order as they were posed:
	    int tag=tag2+myid;
	    MPI_Irecv( receiveResults, 1, IndexedResultDataType[numSent], p, tag, OV_COMM, &receiveRequest[numSent] );
	    numSent++;
	  }
	}

      // ------------ apply the boundary adjustement -------------

	BoundaryAdjustment& bA = bA12(ks1,kd1);
	    
	const RealArray & ba = bA.boundaryAdjustment();
	const RealArray & ob = bA.oppositeBoundary();
	const RealArray & ag = bA.acrossGrid();
	    
	const int nd1a=ba.getBase(0), nd1b=ba.getBound(0), nd1=nd1b-nd1a+1;
	const int nd2a=ba.getBase(1), nd2b=ba.getBound(1), nd2=nd2b-nd2a+1;
	const int nd3a=ba.getBase(2), nd3b=ba.getBound(2), nd3=nd3b-nd3a+1;
	const real *bap=getDataPointer(ba);
	const real *agp=getDataPointer(ag);
	const real *obp=getDataPointer(ob);
#define BA(i1,i2,i3,m) bap[i1-nd1a+nd1*(i2-nd2a+nd2*(i3-nd3a+nd3*(m)))]	    
#define AG(i1,i2,i3,m) agp[i1-nd1a+nd1*(i2-nd2a+nd2*(i3-nd3a+nd3*(m)))]	    
#define OB(i1,i2,i3,m) obp[i1-nd1a+nd1*(i2-nd2a+nd2*(i3-nd3a+nd3*(m)))]	    

	if( debug & 4 )
	{
	  fprintf(plogFile,"AB: grid=%i face=(%i,%i), grid2=%i \n",k1,ks1,kd1,k2);
	}
		  
	real phi;
	loc=0;
	for( int p=0; p<np; p++ ) // process queries from processor p 
	{  
	  if( numberOfDimensions==2 )
	  {
	    for( int i=0; i<nqr[p]; i++ )
	    {
	      AdjustBoundaryQueryData & q = receiveQueries[loc+i];
	      AdjustBoundaryResultData & r = sendResults[loc+i];

	      i1=q.ip[0]; i2=q.ip[1]; i3=q.ip[2];
	      iv[kd1]=ibkd1;  // project to the boundary 

              #ifdef USE_PPP
	        assert( i1>=nd1a && i1<=nd1b && i2>=nd2a && i2<=nd2b  && i3>=nd3a && i3<=nd3b );
              #endif
	      phi   =(AG(i1,i2,i3,0)*(OB(i1,i2,i3,0) - q.xv[0])+
		      AG(i1,i2,i3,1)*(OB(i1,i2,i3,1) - q.xv[1]));
		  
	      if( debug & 4 )
	      {
		fprintf(plogFile,"AB: i=%i (i1,i2,i3)=(%i,%i,%i) x0=(%8.2e,%8.2e) adjust=(%8.2e,%8.2e)\n",
			i,i1,i2,i3,q.xv[0],q.xv[1],phi*BA(i1,i2,i3,0),phi*BA(i1,i2,i3,1));
	      }

	      r.dx[0]=phi*BA(i1,i2,i3,0);
	      r.dx[1]=phi*BA(i1,i2,i3,1);
              r.dx[2]=0.;
	      r.i=q.i;  // pass this back for double checking

	    }
	  }
	  else if( numberOfDimensions==3 )
	  {
	    for( int i=0; i<nqr[p]; i++ )
	    {
	      AdjustBoundaryQueryData & q = receiveQueries[loc+i];
	      AdjustBoundaryResultData & r = sendResults[loc+i];

	      i1=q.ip[0]; i2=q.ip[1]; i3=q.ip[2];
	      iv[kd1]=ibkd1; // project to the boundary

              #ifdef USE_PPP
	      if( !( i1>=nd1a && i1<=nd1b && i2>=nd2a && i2<=nd2b && i3>=nd3a && i3<=nd3b ) )
	      {
                printf("adjustBoundary: ERROR: myid=%i query for pt (i1,i2,i3)=(%i,%i,%i) from p=%i is outside "
                       "local bounds=[%i,%i][%i,%i][%i,%i]\n",
		       myid,i1,i2,i3,p,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b);
                printf(" k1=%i, k2=%i, kd1=%i, ibkd1=%i (boundary index)\n",k1,k2,kd1,ibkd1);

		int ivv[4]={ i1,i2,i3,0};  // 
                int pp = mask.Array_Descriptor.findProcNum( ivv );
                printf(" ---> pt iv=(%i,%i,%i) is on proc %i\n",iv[0],iv[1],iv[2],pp);
                intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
		printf(" maskLocal=[%i,%i][%i,%i][%i,%i]\n",
		       maskLocal.getBase(0),maskLocal.getBound(0),
		       maskLocal.getBase(1),maskLocal.getBound(1),
		       maskLocal.getBase(2),maskLocal.getBound(2));
                    
		Overture::abort("error");
	      }
              #endif
	      phi=(AG(i1,i2,i3,0)*(OB(i1,i2,i3,0) - q.xv[0])+
		   AG(i1,i2,i3,1)*(OB(i1,i2,i3,1) - q.xv[1])+
		   AG(i1,i2,i3,2)*(OB(i1,i2,i3,2) - q.xv[2]));

	      r.dx[0]=phi*BA(i1,i2,i3,0);
	      r.dx[1]=phi*BA(i1,i2,i3,1);
	      r.dx[2]=phi*BA(i1,i2,i3,2);
	      r.i=q.i;  // pass this back for double checking

	    }
	      
	  }
	  else
	  {
	    Overture::abort("ERROR: in numberOfDimensions");
	  }

	  loc+=nqr[p];
	}  // end for p=0...


	// --- send the answers back to the appropriate processor  ---
	loc=0;
        int numResultsSent=0;
	for( int p=0; p<np; p++ )
	{
	  if( nqr[p]>0 )
	  {
	    int tag=tag2+p;
	    MPI_Isend( &(sendResults[loc]), nqr[p], ABResultDataType, p, tag, OV_COMM, 
                       &sendResultsRequest[numResultsSent] ); 
	    loc+= nqr[p];  
            numResultsSent++;
	  }
	}

	if( numSent>0 )
          MPI_Waitall(numSent,receiveRequest,receiveStatus);
  
	if( debug & 4 )
	{ // sanity check:
	  for( int p=0, q=0; p<np; p++ )
	  {
	    if( nqs[p]>0 )
	    {
	      int num=0;
	      MPI_Get_count( &receiveStatus[q], ABResultDataType, &num );
	      assert( num==nqs[p] );
              q++;
	    }
	  }
	  for( int j=0; j<numberToCheck; j++ )
	  {
	    AdjustBoundaryResultData & r = receiveResults[j];
	    int i=ia(j);
	    assert( i==r.i );
	    fprintf(plogFile,"AB:results i=%i, dx=(%8.2e,%8.2e,%8.2e)\n", r.i,r.dx[0],r.dx[1],r.dx[2]);
	    
	    //  fabs(r.xv[0]-X(i,0)),fabs(r.xv[1]-X(i,1)),(numberOfDimensions==3 : fabs(r.xv[2]-X(i,2)) : 0.));
	  }
    
	}
  
	// ---- fill in results ----

	for( int j=0; j<numberToCheck; j++ )
	{
	  AdjustBoundaryResultData & r = receiveResults[j];
	
	  int i = ia(j);
	  assert( i==r.i );
	
	  if( debug & 4 )
	  {
            i1=IP(i,0), i2=IP(i,1), i3=IP(i,2);
	    fprintf(plogFile,"AB: i=%i (i1,i2,i3)=(%i,%i,%i) x=(%8.2e,%8.2e) adjust=(%8.2e,%8.2e)\n",
		    i,i1,i2,i3,X(i,0),X(i,1),r.dx[0],r.dx[1]);
	  }
	  for( int m=0; m<numberOfDimensions; m++ )
	    X(i,m) += r.dx[m];  // add on the adjustment 
	}


	// wait for sends to finish on this processor before we can clean up
	if( numQueriesSent>0 )
  	  MPI_Waitall(numQueriesSent,sendQueryRequest,receiveStatus);
        if( numResultsSent>0 )
	  MPI_Waitall(numResultsSent,sendResultsRequest,receiveStatus);

	// cleanup for this face:
	for( int n=0; n<numQueriesSent; n++ ) 
          MPI_Type_free( &IndexedResultDataType[n] );

	delete [] IndexedResultDataType;

	delete [] sendQueries;
	delete [] receiveQueries;
	delete [] sendResults;
	delete [] receiveResults;

	delete [] processorToCheck;
	
      } // ========== end this face  ==================


      // cleanup:
      MPI_Type_free( &ABQueryDataType );
      MPI_Type_free( &ABResultDataType );

      delete [] nqr;
      delete [] nqs;

      delete [] receiveStatus;
      delete [] receiveRequest;
      delete [] sendQueryRequest;
      delete [] sendResultsRequest;


    }
  }

  return 0;


#endif
}

#undef BA
#undef AG
#undef OP
#undef X0
#undef X
#undef IP
	

#ifdef USE_PPP
int Ogen::
adjustBoundary(CompositeGrid & cg,
               const Integer&      k1,
	       const Integer&      k2,
	       const intArray&     ip,
	       const realArray&    x) 
// ==========================================================================================
// /Description:
//    Adjust the position x of points ip of grid k1 interpolated from
//  base grid k2 to take into account mismatch between shared boundaries.
//
// The basic correction mechanism is defined by
// \begin{verbatim}
//   x(R,Rx) += boundaryAdjustment(i(R),i2(R),i3(R),Rx) \cdot [ 
//            acrossGrid(ip(R),i2(R),i3(R),Rx) \cdot ( oppositeBoundary(ip(R),i2(R),i3(R),Rx)- x(R,Rx) )  ]
//   
// \end{verbatim}
//
// ==========================================================================================
{

  printF("adjustBoundary::Not implemented yet for parallel. Do nothing...\n");
  return 0;
}
#endif


int Ogen::
adjustBoundarySerial(CompositeGrid & cg,
		     const Integer&      k1,
		     const Integer&      k2,
		     const intSerialArray&     ip,
		     const realSerialArray&    x) 
// ==========================================================================================
// /Description:
//    Adjust the position x of points ip of grid k1 interpolated from
//  base grid k2 to take into account mismatch between shared boundaries.
//
// The basic correction mechanism is defined by
// \begin{verbatim}
//   x(R,Rx) += boundaryAdjustment(i(R),i2(R),i3(R),Rx) \cdot [ 
//            acrossGrid(ip(R),i2(R),i3(R),Rx) \cdot ( oppositeBoundary(ip(R),i2(R),i3(R),Rx)- x(R,Rx) )  ]
//   
// \end{verbatim}
//
// ==========================================================================================
{
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfGrids = cg.numberOfComponentGrids();

  BoundaryAdjustmentArray2 & boundaryAdjustment = cg.rcData->boundaryAdjustment;

  if (boundaryAdjustment.getNumberOfElements()) 
  {
    BoundaryAdjustmentArray& bA12 = boundaryAdjustment(k1,k2);
    if( bA12.getNumberOfElements()>0 ) 
    {
      if( debug & 4 ) 
	printF("BA: adjustBoundary: for grid=%i interpolating from grid2=%i \n",k1,k2);

      const IntegerArray & gridIndexRange = cg[k1].gridIndexRange();
      Integer kd1, ks1, dir;
      
      // If multiple sides on the same grid are adjusted, we need to keep a copy of 
      // the original un-adjusted values
      int numberOfSidesAdjusted=0;
      for (kd1=0; kd1<numberOfDimensions && numberOfSidesAdjusted<2; kd1++)
      {
	for (ks1=0; ks1<2; ks1++)
	{
	  BoundaryAdjustment& bA = bA12(ks1,kd1);
	  if ((bA.computedGeometry & (THEinverseMap | THEmask)) == 
              (THEinverseMap | THEmask) && bA.hasSharedSides()==BoundaryAdjustment::share ) 
	  {
	    numberOfSidesAdjusted++;
	  }
	}
      }

      if( numberOfSidesAdjusted==0 )
      {
	if( debug & 4 ) 
          printF("BA:WARNING: There are no sides to be adjusted but boundaryAdjustment(k1,k2) is non-empty\n");
	return 0;
      }
      

      Range R(ip.getBase(0),ip.getBound(0));
      Range Rx(0,numberOfDimensions-1);
      // If multiple sides on the same grid are adjusted, we need to keep a copu of 
      // the original un-adjusted values
      RealArray x0;
      if( numberOfSidesAdjusted>1 )
      {
	x0.redim(R,Rx);  x0=x(R,Rx);   // copy of the original x
      }
      else
      {
	x0.reference(x(R,Rx));      // we don't need a copy 
      }
      

      const int rBase=R.getBase(), rBound=R.getBound();
//      const int rBase=R.getBase(), rBound=R.getBound(), rDim=rBound-rBase+1;
//      const int xBase=x.getBase(0), xDim=x.getRawDataSize(0);
//      const int ipBase=ip.getBase(0), ipDim=ip.getRawDataSize(0);

      real  *xp = x.Array_Descriptor.Array_View_Pointer1; // x.getDataPointer(x);
      const int xDim0=x.getRawDataSize(0);
#define   X(i,m)   xp[i+xDim0*(m)]

      const int *ipp = ip.Array_Descriptor.Array_View_Pointer1; // getDataPointer(ip);
      const int ipDim0=ip.getRawDataSize(0); 
#define  IP(i,m)  ipp[i+ipDim0*(m)]

      const real *x0p =x0.Array_Descriptor.Array_View_Pointer1; // getDataPointer(x0); 
      const int x0Dim0=x0.getRawDataSize(0);
#define  X0(i,m)  x0p[i+x0Dim0*(m)]

      for (kd1=0; kd1<numberOfDimensions; kd1++)
      {
	for (ks1=0; ks1<2; ks1++)
	{
	  BoundaryAdjustment& bA = bA12(ks1,kd1);

	  if( debug & 4 ) 
	    printF("*** BA: adjustBoundary: for (grid,side,axis)=(%i,%i,%i) donor grid2=%i  bA.hasSharedSides()=%i \n",
		   k1,ks1,kd1,k2,(int)bA.hasSharedSides());


	  if ((bA.computedGeometry & (THEinverseMap | THEmask)) == (THEinverseMap | THEmask) &&
              bA.hasSharedSides()==BoundaryAdjustment::share ) 
	  {

	    if( debug & 4 )
	      printF("*** BA: adjustBoundary: for (grid,side,axis)=(%i,%i,%i) interpolating from grid2=%i \n",
                      k1,ks1,kd1,k2);


            const RealArray & ba = bA.boundaryAdjustment();
            const RealArray & ob = bA.oppositeBoundary();
            const RealArray & ag = bA.acrossGrid();
	    
            int ibkd1=cg[k1].gridIndexRange(ks1,kd1);
            const int kd1p1=(kd1+1)%numberOfDimensions;
            const int kd1p2=(kd1+2)%numberOfDimensions;
	    
            // only change points where obMask is TRUE.
            const int obIndex = oppositeBoundaryIndex(cg[k1],ks1,kd1 );

            // printf(" grid=%i, grid2=%i adjust side (%i,%i) obIndex=%i, sum(obMask)=%i max(ba)=%e \n",k1,k2,ks1,kd1,
	    //        obIndex,sum(obMask),max(abs(ba(ib(R,0),ib(R,1),ib(R,2),1))) );
            // display(ip(R,kd1),"Here is ip(R,kd1)");	    
            // display(x(R,Rx),"Here is x(R,Rx)");	    
            // display(obMask,"Here is obMask");	    

            const int nd1a=ba.getBase(0), nd1b=ba.getBound(0), nd1=nd1b-nd1a+1;
            const int nd2a=ba.getBase(1), nd2b=ba.getBound(1), nd2=nd2b-nd2a+1;
            const int nd3a=ba.getBase(2), nd3b=ba.getBound(2), nd3=nd3b-nd3a+1;
            const real *bap=getDataPointer(ba);
            const real *agp=getDataPointer(ag);
            const real *obp=getDataPointer(ob);
#define BA(i1,i2,i3,m) bap[i1-nd1a+nd1*(i2-nd2a+nd2*(i3-nd3a+nd3*(m)))]	    
#define AG(i1,i2,i3,m) agp[i1-nd1a+nd1*(i2-nd2a+nd2*(i3-nd3a+nd3*(m)))]	    
#define OB(i1,i2,i3,m) obp[i1-nd1a+nd1*(i2-nd2a+nd2*(i3-nd3a+nd3*(m)))]	    

	    if( debug & 4 )
	    {
	      fprintf(plogFile,"AB: grid=%i face=(%i,%i), grid2=%i \n",k1,ks1,kd1,k2);
	    }
		  
            int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
            int is1=2*ks1-1;
            real phi;
	    if( numberOfDimensions==2 )
	    {
	      for( int i=rBase; i<=rBound; i++ )
	      {
		if( (IP(i,kd1)-obIndex)*is1>0  )
		{
                  // (i1,i2,i3) = point on boundary next to (IP(i,0:2))
                  i1=IP(i,0); i2=IP(i,1); i3=IP(i,2);
                  iv[kd1]=ibkd1;

                  #ifdef USE_PPP
		  assert( i1>=nd1a && i1<=nd1b && i2>=nd2a && i2<=nd2b );
                  #endif
		  phi   =(AG(i1,i2,i3,0)*(OB(i1,i2,i3,0) - X0(i,0))+
			  AG(i1,i2,i3,1)*(OB(i1,i2,i3,1) - X0(i,1)));

                  if( debug & 4 )
		  {
                    fprintf(plogFile,"AB: i=%i (i1,i2,i3)=(%i,%i,%i) x=(%8.2e,%8.2e) adjust=(%8.2e,%8.2e)\n",
			    i,i1,i2,i3,X(i,0),X(i,1),phi*BA(i1,i2,i3,0),phi*BA(i1,i2,i3,1));
		  }
		  
		  X(i,0)+=phi*BA(i1,i2,i3,0);
		  X(i,1)+=phi*BA(i1,i2,i3,1);
		}
	      }
	    }
	    else if( numberOfDimensions==3 )
	    {
	      for( int i=rBase; i<=rBound; i++ )
	      {
		if( (IP(i,kd1)-obIndex)*is1>0 )
		{
                  i1=IP(i,0); i2=IP(i,1); i3=IP(i,2);
                  iv[kd1]=ibkd1;
                  #ifdef USE_PPP
		  assert( i1>=nd1a && i1<=nd1b && i2>=nd2a && i2<=nd2b && i3>=nd3a && i3<=nd3b  );
                  #endif
//                if( i>50 )
//  		  {
//  		    printf(" i=%i, i1,i2,i3=%i,%i,%i ag=%8.2e,%8.2e,%8.2e, ob=%8.2e,%8.2e,%8.2e "
//                             "x0=%8.2e,%8.2e,%8.2e\n",i,i1,i2,i3,ag(i1,i2,i3,0),ag(i1,i2,i3,1),ag(i1,i2,i3,2),
//  			   ob(i1,i2,i3,0),ob(i1,i2,i3,1),ob(i1,i2,i3,2),x0(i,0),x0(i,1),x0(i,2));
//  		  }
                  //if( true || k1==2 ) // TEMP ************* 120419
		  //{
		  //  printF("AAA adjustBoundary: k1=%i (i1,i2,i3)=(%i,%i,%i) from k2=%i dx=(%e,%e,%e)\n",
		  //	   k1,i1,i2,i3,k2,phi*BA(i1,i2,i3,0),phi*BA(i1,i2,i3,1),phi*BA(i1,i2,i3,2));
		  //}
		  

		  phi=(AG(i1,i2,i3,0)*(OB(i1,i2,i3,0) - X0(i,0))+
			  AG(i1,i2,i3,1)*(OB(i1,i2,i3,1) - X0(i,1))+
                          AG(i1,i2,i3,2)*(OB(i1,i2,i3,2) - X0(i,2)));
		  X(i,0)+=phi*BA(i1,i2,i3,0);
		  X(i,1)+=phi*BA(i1,i2,i3,1);
		  X(i,2)+=phi*BA(i1,i2,i3,2);
		}
	      }
	    }
	    else
	    {
	      Overture::abort("ERROR: in numberOfDimensions");
	    }
#undef BA
#undef AG
#undef OP
#undef X0
#undef X
#undef IP

	    // printf("**After shifting x for ks1=%i, kd1=%i, grid1=%i,grid2=%i \n",ks1,kd1,k1,k2);
	    // display(t3,"shift");
            // printf("grid=%i, grid2=%i, ks1=%i, kd1=%i \n",k1,k2,ks1,kd1);
            // display(phi," adjust boundary : phi");
	    // display(x0," adjust boundary : x0");
	    // display(ob," adjust boundary : ob");
	    // display(ag," adjust boundary : ag");
            // display(ib,"here is ib");
	  } 
          else
	  {
            // *wdh* 010303 phi=0;
	  }
	  
	} // for for ks1
      }  // end for kd1
    } 
  } 

  return 0;
}
