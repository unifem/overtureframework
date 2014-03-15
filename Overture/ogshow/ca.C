#include "ParallelUtility.h"

static FILE *debugFile=NULL;

#ifndef OV_USE_DOUBLE
#define MPI_Real MPI_FLOAT
#else
#define MPI_Real MPI_DOUBLE
#endif

#define FOR_BOX(i0,i1,i2,i3,box)\
      const int i0b=box.bound(0),i1b=box.bound(1),i2b=box.bound(2),i3b=box.bound(3);\
      for( int i3=box.base(3); i3<=i3b; i3++ )\
      for( int i2=box.base(2); i2<=i2b; i2++ )\
      for( int i1=box.base(1); i1<=i1b; i1++ )\
      for( int i0=box.base(0); i0<=i0b; i0++ )

int CopyArray::
copyArray( const realSerialArray & uLocal,
           const Index *Jv, 
           const intSerialArray & uProcessorSet,
           realDistributedArray & v,
           const Index *Iv )
// =======================================================================================
// /Description:
//    Perform a copy from a set of local arrays uLocal, to a distributed array v which loosely
//    speaking looks like:
//
//           v(Iv[0],Iv[1],...) = uLocal(Jv[0],Jv[1],...)
// 
//  This generalizes the usual P++ copy between arrays in that the uLocal serial arrays can
//  have a more general distribution than that current supported by block PARTI.
//
// /uLocal (input) : source array
// /Jv[d] (input) : defines the local rectangular region to copy from this processor.
//                  The actual data sent will be the intersection of these bounds with Iv.
// /uProcessorSet (input) : a list of source processors (i.e. the processors where uLocal has points)
// /Iv[d] (input) : defines the global rectangular region to copy. d=array dimension, d=0,1,..,5
// /v (input/output) : destination array On input this array must  be dimensioned to the correct size. 
// 
// /NOTES: 
//    1. If the uLocal boxes defined by Jv overlap then the value placed in v will depend
///     on the order v is assigned (i.e. not well defined)
///   2. This routine does NOT perform a ghost boundary update so it is up to the calling
///      program to do this if desired. 
// =======================================================================================
{
#ifdef USE_PPP
  MPI_Comm MY_COMM = Overture::OV_COMM;  // Communicator for the parallel interpolator

  const int myid = Communication_Manager::My_Process_Number;
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  const int maxDim=MAX_DISTRIBUTED_DIMENSIONS;
  assert( maxDim==4 );
  const int numDim=min(v.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
  assert( numDim<=4 );
  
  const bool uLocalIsEmpty = uLocal.getLength(0)==0;

  // Do this for now: (we don't accept a null-Index on input)
  for( int d=0; d<maxDim; d++ )
  {
    assert( Iv[d].getLength()>=0 );
    assert( uLocalIsEmpty || Jv[d].getLength()>=0 );
  }
  

  bool copyOnProcessor=true;  // if true do not send messages to the same processor

  int debug=0; // **************
  
  if( debug !=0 && debugFile==NULL )
  {
    char fileName[40];
    sprintf(fileName,"copyArray%i.debug",myid);
    debugFile= fopen(fileName,"w");
  }
  if( debug !=0 )
  {
    fprintf(debugFile,"++++ copyArray: v = uLocal,  *start* myid=%i +++++\n",myid);
    fprintf(debugFile," uLocal=[%i,%i][%i,%i][%i,%i], Jv=[%i,%i][%i,%i][%i,%i]\n",
	    uLocal.getBase(0),uLocal.getBound(0),
	    uLocal.getBase(1),uLocal.getBound(1),
	    uLocal.getBase(2),uLocal.getBound(2),
            Jv[0].getBase(),Jv[0].getBound(),
            Jv[1].getBase(),Jv[1].getBound(),
            Jv[2].getBase(),Jv[2].getBound(),
            Jv[3].getBase(),Jv[3].getBound() 
             );
    fflush(debugFile);
  }


  // v array lives on these processors:
  const intSerialArray & vProcessorSet = v.getPartition().getProcessorSet();
  const int npv=vProcessorSet.getLength(0);
  if( debug & 2 )
  {
    ::display(uProcessorSet,"uProcessorSet",debugFile);
    ::display(vProcessorSet,"vProcessorSet",debugFile);
  }
  

  // check if myid will rec. data for v 
  bool thisProcessorWillReceiveData=false;
  for( int p=0; p<npv; p++ )
  {
    if( vProcessorSet(p)==myid )
    {
      thisProcessorWillReceiveData=true;
      break;
    }
  }
  

  if( uLocalIsEmpty && !thisProcessorWillReceiveData )
  {
    // this processor is not involved in the copy -- we can return
    return 0;
  }

  // Header info to receive:
  //  rhbuff[p][0] : total size of data
  //  rhbuff[p][1] : base0
  //  rhbuff[p][2] : bound0
  //  rhbuff[p][3] : base1
  //  rhbuff[p][4] : bound1
  //     etc. 


  // We will only receive data if v has local data on myid
  // nprh = number of processors we will receive header info from
  // npr  = number of processors we will receive data from (npr<=nprh)
  int nprh=0;
  if( thisProcessorWillReceiveData )
    nprh=uProcessorSet.getLength(0);


  // numHeader = number of Header elements to expect from uProcessorSet(p)
  const int numHeader = 1 + 2*maxDim; // size of header data to receive
  
  // post receives for the header info (size, base, bound of data to be receieved)
  MPI_Request *receiveRequest = new MPI_Request[nprh];
  MPI_Status *receiveStatus   = new MPI_Status [nprh];

  // rhbuff : rec. header buffer
  int **rhbuff = new int* [nprh];

  const int tag1=272934; // make a unique tag
  for( int p=0; p<nprh; p++ )                       // -- NOTE: no need to send header info on-processor *fix me*
  {
    const int pp = uProcessorSet(p);
    int tag=tag1+myid;
    rhbuff[p] = new int [numHeader];
    MPI_Irecv( rhbuff[p],numHeader,MPI_INT ,pp,tag,MY_COMM,&receiveRequest[p] );
  }

  ListOfIndexBox::iterator iter; 

//   Index Iv[4], &I0=Iv[0], &I1=Iv[1], &I2=Iv[2], &I3=Iv[3];
//   Index Jv[4], &J0=Jv[0], &J1=Jv[1], &J2=Jv[2], &J3=Jv[3];

  // Step 1. Determine the information to send
  // make a list of boxes of where to send the data
  ListOfIndexBox sendBoxes;
  IndexBox mySendBox; // for on-processor copy
  int nps=0;  // counts number of non-empty boxes of data we send 
  if( !uLocalIsEmpty )
  {
    // we will send uLocal information

    // intersect Jv with Iv :
    IndexBox vBox(Iv[0].getBase(),Iv[0].getBound(),
                  Iv[1].getBase(),Iv[1].getBound(),
                  Iv[2].getBase(),Iv[2].getBound(),
                  Iv[3].getBase(),Iv[3].getBound());

    IndexBox uBox(Jv[0].getBase(),Jv[0].getBound(),
                  Jv[1].getBase(),Jv[1].getBound(),
                  Jv[2].getBase(),Jv[2].getBound(),
                  Jv[3].getBase(),Jv[3].getBound());

    IndexBox sourceBox; // sourceBox : send this box of data from uLocal 
    bool notEmpty = IndexBox::intersect(uBox,vBox,sourceBox);

    if( notEmpty )
    {
      // We need to send info from uLocal

      // for each v-processor p, intersect uBox with vBoxp
      IndexBox vBoxp;
      for( int p=0; p<npv; p++ )  // only need to loop over proc. in v 
      {
        const int pp = vProcessorSet(p);
        // get vBox: bounds on vLocal on proc. pp 
	getLocalArrayBox( pp, v, vBoxp );


        IndexBox pSendBox;
	notEmpty = IndexBox::intersect( sourceBox,vBoxp, pSendBox); 

	if( debug & 2 )
	{
	  fprintf(debugFile," pp=%i vBoxp=[%i,%i][%i,%i][%i,%i][%i,%i] notEmpty=%i, pSendBox=[%i,%i][%i,%i]\n",
		  pp,
		  vBoxp.base(0),vBoxp.bound(0),
		  vBoxp.base(1),vBoxp.bound(1),
		  vBoxp.base(2),vBoxp.bound(2),
		  vBoxp.base(3),vBoxp.bound(3),notEmpty,
		  pSendBox.base(0),pSendBox.bound(0),
		  pSendBox.base(1),pSendBox.bound(1));
	}

	if( copyOnProcessor && notEmpty && myid==pp )
	{
	  pSendBox.processor=myid;
	  mySendBox=pSendBox;  // save this for later 
	}

        // save the box (even if it is empty)
	pSendBox.processor=pp;
	sendBoxes.push_back(pSendBox);  // We need to send this box of data to processor p

	if( notEmpty && myid!=pp )
	  nps++;

      }
    
      if( debug & 2 )
      {
	for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
	{
	  IndexBox & pSendBox = *iter;
	  fprintf(debugFile,
		  ">>> myid=%i: Send box =[%i,%i][%i,%i][%i,%i][%i,%i] (empty=%i) to processor p=%i\n",
		  myid,
		  pSendBox.base(0),pSendBox.bound(0),
		  pSendBox.base(1),pSendBox.bound(1),
		  pSendBox.base(2),pSendBox.bound(2),
		  pSendBox.base(3),pSendBox.bound(3),(int)pSendBox.isEmpty(),pSendBox.processor);
	}
      }
    } // end if notEmpty
  } // end if uLocal.getLength(0)>0 )
  
  int npsh=0; // we will send this many header's 
  if( uLocal.getLength(0)>0 )
    npsh=npv; // we must send npv headers, not nps, since the rec. proc. expects this many 
   
  int **shbuff = new int* [npsh];

  MPI_Request *sendRequest    = new MPI_Request[npsh];   
  MPI_Status *sendStatus      = new MPI_Status [npsh];

  MPI_Request *sendRequestd    = new MPI_Request[npsh];   
  MPI_Status *sendStatusd      = new MPI_Status [npsh];

  // send sizes of send buffers
  if( npsh>0 ) 
    iter = sendBoxes.begin();
  for( int p=0; p<npsh; p++ ) 
  {
    const int pp=vProcessorSet(p);
    IndexBox & pSendBox = *iter;
    assert( pSendBox.processor==pp );
    int hbuffSize;
    if( pp!=myid && !pSendBox.isEmpty() )
    {
      if( debug & 2 )
      {
	fprintf(debugFile,"send header: p=%i, pp=%i pSendBox.processor=%i\n",p,pp,pSendBox.processor);
      }
      hbuffSize=numHeader;
      shbuff[p] = new int [hbuffSize];
      int k=0;
      shbuff[p][k]=pSendBox.size(); k++;
      for( int d=0; d<maxDim; d++ )
      {
	shbuff[p][k]=pSendBox.base(d);  k++;
	shbuff[p][k]=pSendBox.bound(d); k++;
      }
    }
    else
    {
      // no data to send to proc. pp : 
      if( debug & 2 )
      {
	fprintf(debugFile,"send header: p=%i, pp=%i Send to header to same proc.\n",p,pp);
      }
      hbuffSize=1; // we will send 1 "zero" since the rec. proc. expects a message
      shbuff[p] = new int [hbuffSize];
      for( int i=0; i<hbuffSize; i++ )
	shbuff[p][i]=0;
    }
    if( debug & 2 )
    {
      fprintf(debugFile,"myid=%i : send header info (%i ints) to p=%i\n",myid,hbuffSize,pp);
      fprintf(debugFile,"          shbuff=");
      for( int j=0; j<hbuffSize; j++ )
	fprintf(debugFile,"%i ",shbuff[p][j]);
      fprintf(debugFile,"\n");
    }

    const int sendTag = tag1 + pp;
    MPI_Isend(shbuff[p],hbuffSize,MPI_INT,pp,sendTag,MY_COMM,&sendRequest[p] );

    iter++;  // get next box 

  }
  

  MPI_Waitall( nprh, receiveRequest, receiveStatus );    // wait to receive all messages

  // rec. header data and allocate rec. buffers, and post receives.
  const int tag2=80207; // make a unique tag
  real **rbuff = new real* [nprh];
  int npr=0; // counts number of data receives to expect (only those with non-zero data to send)
  for( int p=0; p<nprh; p++ )
  {
    rbuff[p]=NULL;
    const int pp=receiveStatus[p].MPI_SOURCE;
    assert( pp==uProcessorSet(p) );
    int num = rhbuff[p][0];
    if( debug & 2 )
      fprintf(debugFile," -> received header info from p=%i, header-size=%i\n",pp,num);

    if( num>0 )
    { // post receives for data (only from proc's with non-zero amounts)
      if( debug & 2 )
	fprintf(debugFile,"myid=%i : post receive for %i data items from p=%i\n",myid,num,pp);

      rbuff[npr] = new real [ num ];
      int tag=tag2+myid;
      MPI_Irecv(rbuff[npr], num, MPI_Real ,pp,tag,MY_COMM,&receiveRequest[npr] );

      npr++;

    }
  }
  
  const real *uLocalp = uLocal.Array_Descriptor.Array_View_Pointer3;
  const int uLocalDim0=uLocal.getRawDataSize(0);
  const int uLocalDim1=uLocal.getRawDataSize(1);
  const int uLocalDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) uLocalp[i0+uLocalDim0*(i1+uLocalDim1*(i2+uLocalDim2*(i3)))]

  int i0,i1,i2,i3;
  // allocate, fill and send buffers 
  real **sbuff = new real* [nps];
  int p=0;
  for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
  {
    IndexBox & pSendBox = *iter;
    const int pp = pSendBox.processor;
    if( pp!=myid && !pSendBox.isEmpty()  )
    {
      // -- send this non-empty box of data --
      const int buffSize = pSendBox.size();
    
      sbuff[p] = new real [ max(1,buffSize) ];

      // fill buffers:
      int k=0;
      FOR_BOX(i0,i1,i2,i3,pSendBox)
      {
	sbuff[p][k] = U(i0,i1,i2,i3); k++;
      }

      if( debug & 2 )
      {
	fprintf(debugFile,"myid=%i : send %i data items to p=%i\n",myid,buffSize,pp);
	fprintf(debugFile,"          sbuff=");
	for( int j=0; j<buffSize; j++ )
	  fprintf(debugFile,"%8.2e ",sbuff[p][j]);
	fprintf(debugFile,"\n");
      }
    
      // send data
      int sendTag=tag2+pp;
      MPI_Isend(sbuff[p],buffSize,MPI_Real,pp,sendTag,MY_COMM,&sendRequestd[p] );

      p++;
    }
  }
  assert( p==nps );
  
  realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
  real *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
  const int vDim0=vLocal.getRawDataSize(0);
  const int vDim1=vLocal.getRawDataSize(1);
  const int vDim2=vLocal.getRawDataSize(2);
#undef V
#define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]

  if( debug & 2 )
  {
    fprintf(debugFile," vLocal=[%i,%i][%i,%i][%i,%i]\n",
	    uLocal.getBase(0),uLocal.getBound(0),
	    uLocal.getBase(1),uLocal.getBound(1),
	    uLocal.getBase(2),uLocal.getBound(2));
    fflush(debugFile);
  }

  // --- copy on processor here, while we wait for any messages ---
  if( copyOnProcessor && !mySendBox.isEmpty() )
  {
    // Do not send a message to the same processor -- just copy the data
    if( debug & 2 )
    {
      fprintf(debugFile,
	      ">>> myid=%i: Just copy data: [%i,%i][%i,%i][%i,%i][%i,%i] on the same processor p=%i\n",
	      myid,
	      mySendBox.base(0),mySendBox.bound(0),
	      mySendBox.base(1),mySendBox.bound(1),
	      mySendBox.base(2),mySendBox.bound(2),
	      mySendBox.base(3),mySendBox.bound(3),myid);
    }

    // assign points defined in mySendBox:
    FOR_BOX(i0,i1,i2,i3,mySendBox)
    { 
      V(i0,i1,i2,i3)=U(i0,i1,i2,i3);  
    }
  }


  MPI_Waitall( npr, receiveRequest, receiveStatus );    // wait to receive all messages


  int mpr=0;
  for( int p=0; p<nprh; p++ )
  {
    int num = rhbuff[p][0];
    if( num>0 )
    {
      const int pp=receiveStatus[mpr].MPI_SOURCE;
      // check me: assert( pp==uProcessorSet(p) ); 

      // for( int d=0; d<maxDim; d++ )
      // {
      //   Kv[d] = Range(rhbuff[p][i], rhbuff[p][i+1]); i+=2;
      // }

      IndexBox rBox(rhbuff[p][1],rhbuff[p][2],
		    rhbuff[p][3],rhbuff[p][4],
		    rhbuff[p][5],rhbuff[p][6],
		    rhbuff[p][7],rhbuff[p][8]);
    
      if( debug & 2 )
      {
        fprintf(debugFile," receive data from pp=%i (mpr=%i) data-count=%i\n",pp,mpr,rhbuff[p][0]);
	fprintf(debugFile,
		" -- fill v : [%i,%i][%i,%i][%i,%i][%i,%i] with data from p=%i\n",
		rBox.base(0),rBox.bound(0),
		rBox.base(1),rBox.bound(1),
		rBox.base(2),rBox.bound(2),
		rBox.base(3),rBox.bound(3),pp);

	fprintf(debugFile,"   rbuff=");
	for( int j=0; j<rhbuff[p][0]; j++ )
	  fprintf(debugFile,"%8.2e ",rbuff[mpr][j]);
	fprintf(debugFile,"\n");
      }
      
      int k=0;
      FOR_BOX(i0,i1,i2,i3,rBox)
      {
	V(i0,i1,i2,i3) = rbuff[mpr][k]; k++;
      }
      mpr++;
    }
  }
  assert( mpr==npr );

  if( debug & 2 )
  {
    fflush(debugFile);
  }
  
  // we must wait for all send's to complete before deleting the buffers
  MPI_Waitall( npsh, sendRequest, sendStatus );    // header sends
  MPI_Waitall( nps, sendRequestd, sendStatusd );  // data sends


  // clean up :     
  delete [] receiveRequest;
  delete [] receiveStatus;
  delete [] sendRequest;
  delete [] sendStatus;
  delete [] sendRequestd;
  delete [] sendStatusd;

  for( int p=0; p<nprh; p++ )
    delete [] rhbuff[p];
  delete [] rhbuff;

  for( int p=0; p<npsh; p++ ) 
    delete [] shbuff[p];
  delete [] shbuff;

  for( int p=0; p<nprh; p++ )
    delete [] rbuff[p];
  delete [] rbuff;

  for( int p=0; p<nps; p++ ) 
    delete [] sbuff[p];
  delete [] sbuff;

  return 0;

#else
  // serial case

  v(Iv[0],Iv[1],Iv[2],Iv[3])=uLocal(Jv[0],Jv[1],Jv[2],Jv[3]);

  return 0;

#endif
}
