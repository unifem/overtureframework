#ifdef USE_PPP
//   --- routines for the parallel Tridiagonal Solver ----


#include "Overture.h"
#include "TridiagonalSolver.h"
#include "display.h"
#include "ParallelUtility.h"

#ifndef OV_USE_DOUBLE
#define MPI_Real_Type MPI_FLOAT
#else
#define MPI_Real_Type MPI_DOUBLE
#endif

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


#define FOR_BLOCK(b1,b2)\
for( int b1=0; b1<blockSize; b1++)\
for( int b2=0; b2<blockSize; b2++) 


// ===============================================================================
/// \brief Return the list of processors that lie along an axis and includes p=pStart correspoding to array u.
/// This routine is used by the tridianginal solver to determine which processors are involved
///  in solving along a line.
///
/// \param pStart (input) :
/// \param axis (input) : 
/// \param numProcessorsThisAxis (ouptut) : number of processors along this axis
/// \param processorSet (output) : list of processors (this array will be allocated by this routine) You should delete when 
///                      finished.
/// \param u (input) : array defining the distribution.
///  
/// Example distribution: 
//            ------------
//            | p2 | p5  |
//            ------------
//            | p1 | p4  |
//            ------------
//            | p0 | p3  |
//            ------------
//            axis=0 --->
//
//   E.g. In the above distribution: 
//    If axis==0 : return the list of processors along axis=0 that include processor pStart.
//       pStart=0 or 3 then return processorSet=[0,3]
//       pStart=1 or 4 then return processorSet=[1,4]
//       pStart=2 or 5 then return processorSet=[2,5]
//   if axis==1 :
//       pStart=0,1,2 then return processorSet=[0,1,2]
//       pStart=3,4,5 then return processorSet=[3,4,5]
//  
// ===============================================================================
static
int 
getProcessorsThisAxis( const int pStart, const int axis, int & numProcessorsThisAxis, int *&processorSet, const realArray & u  )
{
#ifdef USE_PPP
#ifndef USE_PADRE
  DARRAY *uDArray = u.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
#else
  // Padre version:
  DARRAY *uDArray = u.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
    pPARTI_Representation->BlockPartiArrayDescriptor; 
#endif
  DECOMP *uDecomp = uDArray->decomp;

  const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
  const int baseProc = uDecomp->baseProc; 

  // for( int d=numDim-1; d>=0; d-- )
  // {
  //   printF("dimProc[%i]=%i\n",d,uDecomp->dimProc[d]);
  // }
  

  numProcessorsThisAxis=uDecomp->dimProc[axis];
  delete [] processorSet;
  processorSet = new int[numProcessorsThisAxis];

  int procFactor=1;
  for( int d=numDim-1; d>axis; d-- )
  {
    procFactor*=uDecomp->dimProc[d];
  }
  int firstProc = pStart % procFactor;
  for( int i=0; i<numProcessorsThisAxis; i++ )
  {
    processorSet[i]=firstProc + i*procFactor;
  }
  
#else
  OV_ABORT("error");
#endif
  return 0;
}

// ======================================================================================
/// \brief Get local array bounds for an array where the 3 partitioned dimensions I1,I2,I3
///  are offset from the usual positions [0,1,2] to be instead [indexOffset,indexOffset+1,indexOffset+2]
// 
// ======================================================================================
static bool
getLocalArrayBounds(realArray & a , realSerialArray & aLocal, Index & I1, Index & I2, Index & I3,
                    const int includeGhost, const int indexOffset=0 )
{
  bool ok=true;

  if( indexOffset==0 )
  {
    ok= ParallelUtility::getLocalArrayBounds(a,aLocal,I1,I2,I3,includeGhost);
  }
  else
  {
    for( int d=0; d<3; d++ )
    {
      Index &I = d==0 ? I1 : d==1 ? I2 : I3;
      int na,nb;
      if( includeGhost )
      {
	// include parallel ghost boundaries
	na = max(I.getBase() , aLocal.getBase(indexOffset+d));
	nb = min(I.getBound(),aLocal.getBound(indexOffset+d));
      }
      else
      {
	// do not include parallel ghost boundaries
	na = max(I.getBase() , aLocal.getBase(indexOffset+d)+a.getGhostBoundaryWidth(indexOffset+d));
	nb = min(I.getBound(),aLocal.getBound(indexOffset+d)-a.getGhostBoundaryWidth(indexOffset+d));
      }
      if( nb>=na )
      {
        I=Range(na,nb);
      }
      else
      {
	ok=false;
	break;
      }
      
    }
  }
  return ok;
}

  
#undef BB
#define BB(b1,b2) (b1+block*(b2))

// ======================================================================================
/// \brief Factor the tridiagonal system 
///
/// In this version we copy the full tridiagonal systems to the processors where
/// they need to be solved and then use the serial Tridiagonal factor (i.e. we factor duplicate times)
// 
/// A given processor generally only needs to solve a small subset of all systems and thus
/// this may be a reasonable first approach.
//  
/// NOTE: For parallel the block matrices are assumed to be dimensioned:
///      a(blockSize*blockSize, I1,I2,I3)
// ======================================================================================
int TridiagonalSolver::
factor( realArray & a, realArray & b, realArray & c, 
        const SystemType & type /* =normal */, 
	const int & systemAxis /* = 0 */ ,
	const int & block /* =1 */ )
{
  debug=3;
  if( debug !=0 )
    openDebugFiles();

  axis=systemAxis;
  
  bandWidth=3;  // 3=tridiagonal system : number of entries per grid point [a,b,c]
  blockSize=block;
  const int blockOffset= blockSize==1 ? 0 : 1;
  

  OV_GET_SERIAL_ARRAY(real,a,aLocal);
  if( aLocal.elementCount()==0 )
  { // There is nothing to be done on this processor.
    return 0;
  }
  

  OV_GET_SERIAL_ARRAY(real,b,bLocal);
  OV_GET_SERIAL_ARRAY(real,c,cLocal);


  // const intSerialArray & aProcessorSet = a.getPartition().getProcessorSet();
  // ::display(aProcessorSet,"factor: aProcessorSet");

  const int myid=max(0,Communication_Manager::My_Process_Number);
  int numProcessorsThisAxis=0;
  int *processorSet=NULL;
  getProcessorsThisAxis( myid, axis+blockOffset, numProcessorsThisAxis,processorSet,a );
  if( debug & 1 )
  {
    fprintf(debugFile,"\n --PTS-- factor: myid=%i, axis=%i blockSize=%i SystemType=%i, processors=[",myid,axis,blockSize,(int)type);
    for(int i=0; i<numProcessorsThisAxis; i++ ) fprintf(debugFile,"%i,",processorSet[i]);
    fprintf(debugFile,"]\n");
    fflush(debugFile);
  }
  

  Range D1=a.dimension(blockOffset+0), D2=a.dimension(blockOffset+1), D3=a.dimension(blockOffset+2);
  int includeGhost=0;  // do not send ghost points
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  I1=D1, I2=D2, I3=D3;
  bool ok;
  ok = getLocalArrayBounds(a,aLocal,I1,I2,I3,includeGhost,blockOffset);
  assert( ok );
  
  // Arrays a0,b0,c0 hold the entire set of tridiagonal systems that live on this processor
  RealArray a0,b0,c0;
  Range B=blockSize;
  Range BD=blockSize*blockSize;
  if( blockSize==1 )
  {
    if( axis==0 )
    {
      a0.redim(D1,I2,I3);
      b0.redim(D1,I2,I3);
      c0.redim(D1,I2,I3);
    }
    else if( axis==1 )
    {
      a0.redim(I1,D2,I3);
      b0.redim(I1,D2,I3);
      c0.redim(I1,D2,I3);
    }
    else if( axis==2 )
    {
      a0.redim(I1,I2,D3);
      b0.redim(I1,I2,D3);
      c0.redim(I1,I2,D3);
    }
    else
    {
      OV_ABORT("ERROR - invalid value for axis");
    }
  }
  else
  {
    if( axis==0 )
    {
      a0.redim(BD,D1,I2,I3);
      b0.redim(BD,D1,I2,I3);
      c0.redim(BD,D1,I2,I3);
    }
    else if( axis==1 )
    {
      a0.redim(BD,I1,D2,I3);
      b0.redim(BD,I1,D2,I3);
      c0.redim(BD,I1,D2,I3);
    }
    else if( axis==2 )
    {
      a0.redim(BD,I1,I2,D3);
      b0.redim(BD,I1,I2,D3);
      c0.redim(BD,I1,I2,D3);
    }
    else
    {
      OV_ABORT("ERROR - invalid value for axis");
    }
  }
  

  // Post receives for data from other proc's in the processor set
  int numReceive=numProcessorsThisAxis-1; // we receive data from this many other processors

  real **rBuff=NULL;   // buffers for receiving data
  real *buffs=NULL;     // buffer for sending data 
  MPI_Request *receiveRequest=NULL;
  MPI_Status *receiveStatus=NULL;
  MPI_Request *sendRequest= NULL;
  MPI_Status *sendStatus=NULL;

  const int tag1=729832;  // make a unique tag
  if( numReceive>0 )
  {
    // --- We must receive part of the tridiagonal system, from other processors ---

    IndexBox pBox[numReceive]; // holds sizes of arrays on other processors 

    receiveRequest= new MPI_Request[numReceive]; // remember to delete these
    receiveStatus= new MPI_Status[numReceive]; 

    rBuff = new real* [numReceive];
    int pCount=0;
    for( int pp=0; pp<numProcessorsThisAxis; pp++ )
    {
      // post receive from processor processorSet[pp]
      int pr=processorSet[pp];
      if( pr!=myid )
      {
	// Compute the size of the local array on processor pr: 
	CopyArray::getLocalArrayBox( pr,a,pBox[pCount] );
        
	int bufSize=bandWidth*pBox[pCount].size();  // pBox includes factor of blockSize^2

	rBuff[pCount]= new real [bufSize];

        int tag=tag1+pr;
	MPI_Irecv(rBuff[pCount],bufSize,MPI_Real_Type,pr,tag,Overture::OV_COMM,&receiveRequest[pCount] );

	pCount++;
      }
    }
    assert( pCount==numReceive );
    
    // Send my data to the other proc's in the processor set
    // ***** send all info ****
    sendRequest = new MPI_Request[numReceive]; 
    sendStatus = new MPI_Status[numReceive]; 

    const int numGridPointsToSend=I1.getLength()*I2.getLength()*I3.getLength();
    const int numDataToSend=SQR(blockSize)*bandWidth*numGridPointsToSend;
    buffs = new real [numDataToSend]; // buffer to hold send data
    // copy data into the buffer
    int i1,i2,i3;
    int k=0;
    if( blockSize==1 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	buffs[k]=aLocal(i1,i2,i3); k++;
	buffs[k]=bLocal(i1,i2,i3); k++;
	buffs[k]=cLocal(i1,i2,i3); k++;
      }
    }
    else
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	FOR_BLOCK(b1,b2)
	{
	  buffs[k]=aLocal(BB(b1,b2),i1,i2,i3); k++;
	  buffs[k]=bLocal(BB(b1,b2),i1,i2,i3); k++;
	  buffs[k]=cLocal(BB(b1,b2),i1,i2,i3); k++;
	}
	
      }
    }
    // fprintf(debugFile,"Send %i data values\n",k);
    assert( numDataToSend==k );

    pCount=0;
    for( int pp=0; pp<numProcessorsThisAxis; pp++ )
    {
      // send data to processor processorSet[pp]
      int ps=processorSet[pp];
      if( ps!=myid )
      {
	int tag=tag1+myid;
        // send the buffer
	MPI_Isend(buffs,numDataToSend,MPI_Real_Type,ps,tag,Overture::OV_COMM,&sendRequest[pCount] );
	pCount++;
      }
    }
    assert( pCount==numReceive );

    MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to recieve all messages

    // -- receive and unpack the data ---
    pCount=0;
    for( int pp=0; pp<numProcessorsThisAxis; pp++ )
    {
      int pr=processorSet[pp];
      if( pr!=myid )
      {
        // sanity check: make sure we received the expected data: 
	int numReceived=0;
	MPI_Get_count( &receiveStatus[pCount], MPI_Real_Type, &numReceived );
        int numExpected=bandWidth*pBox[pCount].size(); // NOTE: pBox includes factor of blockSize^2
	if( numReceived!=numExpected )
	{
	  fprintf(debugFile,"ERROR: myid=%i, pr=%i, pCount=%i : numReceived=%i, numExpected=%i, pBox[pCount].size()=%i\n",
		  myid,pr,pCount,numReceived,numExpected,pBox[pCount].size());
	  OV_ABORT("error");
	}
	

        // fill in the data from other processors into the local arrays
        Index J1=Range(pBox[pCount].base(blockOffset+0),pBox[pCount].bound(blockOffset+0));
        Index J2=Range(pBox[pCount].base(blockOffset+1),pBox[pCount].bound(blockOffset+1));
        Index J3=Range(pBox[pCount].base(blockOffset+2),pBox[pCount].bound(blockOffset+2));
	if( axis==0 )
	{
	  assert( J2==I2 && J3==I3 );
	}
	
	real *buff = rBuff[pCount];
	int k=0;
	if( blockSize==1 )
	{
	  FOR_3D(i1,i2,i3,J1,J2,J3)
	  {
	    a0(i1,i2,i3)=buff[k]; k++;
	    b0(i1,i2,i3)=buff[k]; k++;
	    c0(i1,i2,i3)=buff[k]; k++;
	  }
	}
	else
	{
	  FOR_3D(i1,i2,i3,J1,J2,J3)
	  {
	    FOR_BLOCK(b1,b2)
	    {
	      a0(BB(b1,b2),i1,i2,i3)=buff[k]; k++;
	      b0(BB(b1,b2),i1,i2,i3)=buff[k]; k++;
	      c0(BB(b1,b2),i1,i2,i3)=buff[k]; k++;
	    }
	  }
	}
	
	assert( k==numReceived );

	pCount++;
      }
    }
    assert( pCount==numReceive );
  
  } // end if numReceive>0 

  if( debug & 1 )
  {
    fprintf(debugFile,"\n --PTS-- factor: Before serial factor\n");
    fflush(debugFile);
  }


  // fill in local part of array 
  if( blockSize==1 )
  {
    a0(I1,I2,I3)=aLocal(I1,I2,I3);
    b0(I1,I2,I3)=bLocal(I1,I2,I3);
    c0(I1,I2,I3)=cLocal(I1,I2,I3);
  }
  else
  {
    a0(BD,I1,I2,I3)=aLocal(BD,I1,I2,I3);
    b0(BD,I1,I2,I3)=bLocal(BD,I1,I2,I3);
    c0(BD,I1,I2,I3)=cLocal(BD,I1,I2,I3);
  }
  
  if( debug & 2 )
  {
    fprintf(debugFile,"**** Here are the local tridiagonal systems that live on this processor***\n");
    ::display(aLocal,"aLocal",debugFile,"%5.2f ");
    ::display(bLocal,"bLocal",debugFile,"%5.2f ");
    ::display(cLocal,"cLocal",debugFile,"%5.2f ");
    fprintf(debugFile,"**** Here are the full tridiagonal systems that live on this processor***\n");
    ::display(a0,"a0",debugFile,"%5.2f ");
    ::display(b0,"b0",debugFile,"%5.2f ");
    ::display(c0,"c0",debugFile,"%5.2f ");
    fflush(debugFile);
  }
  

  // factor in serial 
  if( blockSize>1 )
  {
    a0.reshape(B,B,a0.dimension(1),a0.dimension(2),a0.dimension(3));
    b0.reshape(B,B,b0.dimension(1),b0.dimension(2),b0.dimension(3));
    c0.reshape(B,B,c0.dimension(1),c0.dimension(2),c0.dimension(3));
  }
  
  factor( a0,b0,c0,type,axis,blockSize);

  // clean up 
  MPI_Waitall( numReceive, sendRequest, sendStatus );  // wait to send all messages


  delete [] receiveRequest;
  delete [] receiveStatus;
  delete [] sendRequest;
  delete [] sendStatus;
  for( int pp=0; pp<numReceive; pp++ )
  {
    delete [] rBuff[pp];
  }

  delete [] rBuff;
  delete [] buffs;

  delete [] processorSet;
  return 0;
}

// =============================================================================================
/// \brief parallel penta-diagonal solver
/// 
// =============================================================================================
int TridiagonalSolver::
factor( realArray & a, 
	realArray & b, 
	realArray & c, 
	realArray & d, 
	realArray & e, 
	const SystemType & type /* =normal */, 
	const int & axis /* =0 */,
	const int & block /* =1 */ )
{

  OV_ABORT("finish me");
  return 0;
}




// =============================================================================================
/// \brief Solve the tridiagonal system (in parallel)
/// 
// =============================================================================================
int TridiagonalSolver::
solve( realArray & r, 
       const Range & R1 /*=nullRange */, 
       const Range & R2 /*=nullRange */, 
       const Range & R3 /*=nullRange3 */ )
{
  if( false )
  {
    OV_GET_SERIAL_ARRAY(real,r,rLocal);
    rLocal=1.;
    return 0;
  }
  

  bandWidth=3;  // 3=tridiagonal system : number of entries per grid point [a,b,c]
  Range B=blockSize;
  const int blockOffset= blockSize==1 ? 0 : 1;

  debug=3;

  OV_GET_SERIAL_ARRAY(real,r,rLocal);
  if( rLocal.elementCount()==0 )
  { // There is nothing to be done on this processor.
    return 0;
  }
  

  const int myid=max(0,Communication_Manager::My_Process_Number);
  int numProcessorsThisAxis=0;
  int *processorSet=NULL;
  getProcessorsThisAxis( myid, axis+blockOffset, numProcessorsThisAxis,processorSet,r );
  if( true )
  {
    fprintf(debugFile,"\n --PTS-- solve: myid=%i, axis=%i: processors=[",myid,axis);
    for(int i=0; i<numProcessorsThisAxis; i++ ) fprintf(debugFile,"%i,",processorSet[i]);
    fprintf(debugFile,"]\n");
  }
  
  Range D1=r.dimension(blockOffset+0), D2=r.dimension(blockOffset+1), D3=r.dimension(blockOffset+2);

  int includeGhost=0;  // do not send ghost points
  Index I1,I2,I3;
  I1=D1, I2=D2, I3=D3;
  bool ok = getLocalArrayBounds(r,rLocal,I1,I2,I3,includeGhost,blockOffset);


  // Arrays r0 holds the entire solution of the tridiagonal systems that live on this processor
  RealArray r0;
  if( blockSize==1 )
  {
    if( axis==0 )
    {
      r0.redim(D1,I2,I3);
    }
    else if( axis==1 )
    {
      r0.redim(I1,D2,I3);
    }
    else if( axis==2 )
    {
      r0.redim(I1,I2,D3);
    }
    else
    {
      OV_ABORT("ERROR");
    }
  }
  else
  {
    if( axis==0 )
    {
      r0.redim(B,D1,I2,I3);
    }
    else if( axis==1 )
    {
      r0.redim(B,I1,D2,I3);
    }
    else if( axis==2 )
    {
      r0.redim(B,I1,I2,D3);
    }
    else
    {
      OV_ABORT("ERROR");
    }
  }
  
  // Post receives for data from other proc's in the processor set
  int numReceive=numProcessorsThisAxis-1; // we receive data from this many other processors

  real **rBuff=NULL;   // buffers for receiving data
  real *buffs=NULL;     // buffer for sending data 
  MPI_Request *receiveRequest=NULL;
  MPI_Status *receiveStatus=NULL;
  MPI_Request *sendRequest= NULL;
  MPI_Status *sendStatus=NULL;

  const int tag1=630721;  // make a unique tag
  if( numReceive>0 )
  {
    // --- We must receive part of the tridiagonal system, from other processors ---

    IndexBox pBox[numReceive]; // holds sizes of arrays on other processors 

    receiveRequest= new MPI_Request[numReceive]; // remember to delete these
    receiveStatus= new MPI_Status[numReceive]; 

    rBuff = new real* [numReceive];
    int pCount=0;
    for( int pp=0; pp<numProcessorsThisAxis; pp++ )
    {
      // post receive from processor processorSet[pp]
      int pr=processorSet[pp];
      if( pr!=myid )
      {
	// Compute the size of the local array on processor pr: 
	CopyArray::getLocalArrayBox( pr,r,pBox[pCount] );
        
	int bufSize=pBox[pCount].size(); // NOTE: pBox includes factor of blockSize

	rBuff[pCount]= new real [bufSize];

        int tag=tag1+pr;
	MPI_Irecv(rBuff[pCount],bufSize,MPI_Real_Type,pr,tag,Overture::OV_COMM,&receiveRequest[pCount] );

	pCount++;
      }
    }
    assert( pCount==numReceive );
    
    // Send my data to the other proc's in the processor set
    // ***** send all info ****
    sendRequest = new MPI_Request[numReceive]; 
    sendStatus = new MPI_Status[numReceive]; 

    const int numGridPointsToSend=I1.getLength()*I2.getLength()*I3.getLength();
    const int numDataToSend=blockSize*numGridPointsToSend;
    buffs = new real [numDataToSend]; // buffer to hold send data
    // copy data into the buffer
    int i1,i2,i3;
    int k=0;
    if( blockSize==1 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	buffs[k]=rLocal(i1,i2,i3); k++;
      }
    }
    else
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	for( int b1=0; b1<blockSize; b1++ )
	{
	  buffs[k]=rLocal(b1,i1,i2,i3); k++;
	}
      }
      
    }
    
    assert( numDataToSend==k );

    pCount=0;
    for( int pp=0; pp<numProcessorsThisAxis; pp++ )
    {
      // send data to processor processorSet[pp]
      int ps=processorSet[pp];
      if( ps!=myid )
      {
	int tag=tag1+myid;
        // send the buffer
	MPI_Isend(buffs,numDataToSend,MPI_Real_Type,ps,tag,Overture::OV_COMM,&sendRequest[pCount] );
	pCount++;
      }
    }
    assert( pCount==numReceive );

    MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to recieve all messages

    // -- receive and unpack the data ---
    pCount=0;
    for( int pp=0; pp<numProcessorsThisAxis; pp++ )
    {
      int pr=processorSet[pp];
      if( pr!=myid )
      {
        // sanity check: make sure we received the expected data: 
	int numReceived=0;
	MPI_Get_count( &receiveStatus[pCount], MPI_Real_Type, &numReceived );
        int numExpected=pBox[pCount].size();
	if( numReceived!=numExpected )
	{
	  fprintf(debugFile,"solve:ERROR: myid=%i, pr=%i, pCount=%i : numReceived=%i, numExpected=%i, pBox[pCount].size()=%i\n",
		  myid,pr,pCount,numReceived,numExpected,pBox[pCount].size());
	  OV_ABORT("error");
	}
	

        // fill in the data from other processors into the local arrays
        Index J1=Range(pBox[pCount].base(blockOffset+0),pBox[pCount].bound(blockOffset+0));
        Index J2=Range(pBox[pCount].base(blockOffset+1),pBox[pCount].bound(blockOffset+1));
        Index J3=Range(pBox[pCount].base(blockOffset+2),pBox[pCount].bound(blockOffset+2));

	// fprintf(debugFile,"receive: J1=[%i,%i] J2=[%i,%i] J3=[%i,%i]\n",J1.getBase(),J1.getBound(),
	// 	J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound());
	
	if( axis==0 )
	{
	  assert( J2==I2 && J3==I3 );
	}
	
	real *buff = rBuff[pCount];
	int k=0;
	if( blockSize==1 )
	{
	  FOR_3D(i1,i2,i3,J1,J2,J3)
	  {
	    r0(i1,i2,i3)=buff[k]; k++;
	  }
	}
	else
	{
	  FOR_3D(i1,i2,i3,J1,J2,J3)
	  {
	    for( int b1=0; b1<blockSize; b1++ )
	    {
	      // fprintf(debugFile,"receive: (b1,i2,i3,i3)=(%i,%i,%i,%i) buff=%8.2e\n",b1,i1,i2,i3,buff[k]);
	      r0(b1,i1,i2,i3)=buff[k]; k++;
	    }
	  }
	}
	
	assert( k==numReceived );

	pCount++;
      }
    }
    assert( pCount==numReceive );
  
  } // end if numReceive>0 

  // fill in local part of array 
  if( blockSize==1 )
    r0(I1,I2,I3)=rLocal(I1,I2,I3);
  else
    r0(B,I1,I2,I3)=rLocal(B,I1,I2,I3);
  
  if( debug & 2 )
  {
    fprintf(debugFile,"**** Here is the local RHS that live on this processor***\n");
    ::display(rLocal,"rLocal",debugFile,"%5.2f ");
    fprintf(debugFile,"**** Here is the full RHS that live on this processor***\n");
    ::display(r0,"r0",debugFile,"%5.2f ");
  }
  
  const int na = blockSize==1 ? 0 : 2;
  Index R1r=R1, R2r=R2, R3r=R3;
  Index I1r,I2r,I3r;
  I1r= R1==nullRange ? I1 : R1r;
  I2r= R2==nullRange ? I2 : R2r;
  I3r= R3==nullRange ? I3 : R3r;

  // --- solve in serial ---
  solve( r0,I1r,I2r,I3r );

  if( debug & 2 )
  {
    fprintf(debugFile,"**** Here is the full SOLUTION that lives on this processor***\n");
    ::display(r0,"r0",debugFile,"%5.2f ");
  }


  // Fill in the local solution plus ghost: *FIX FOR STRIDE AND GHOST*
  I1=D1, I2=D2, I3=D3;
  includeGhost=0;
  ok = getLocalArrayBounds(r,rLocal,I1,I2,I3,includeGhost,blockOffset);

  if( blockSize==1 )
    rLocal(I1,I2,I3)=r0(I1,I2,I3);
  else
    rLocal(B,I1,I2,I3)=r0(B,I1,I2,I3);
  
  if( debug & 2 )
  {
    fprintf(debugFile,"**** Here is the LOCAL SOLUTION that lives on this processor***\n");
    ::display(rLocal,"rLocal",debugFile,"%5.2f ");
  }

  // clean up 
  MPI_Waitall( numReceive, sendRequest, sendStatus );  // wait to send all messages


  delete [] receiveRequest;
  delete [] receiveStatus;
  delete [] sendRequest;
  delete [] sendStatus;
  for( int pp=0; pp<numReceive; pp++ )
  {
    delete [] rBuff[pp];
  }

  delete [] rBuff;
  delete [] buffs;

  delete [] processorSet;
  return 0;

  // r=1.;
  // return 0;
}


  
#endif /* USE_PPP */
