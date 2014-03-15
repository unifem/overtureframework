// This file automatically generated from CopyArray.bC with bpp.
#include "ParallelUtility.h"

static FILE *debugFile=NULL;


#ifndef OV_USE_DOUBLE
#define MPI_Real MPI_FLOAT
#else
#define MPI_Real MPI_DOUBLE
#endif

#define FOR_BOX(i0,i1,i2,i3,box)const int i0b=box.bound(0),i1b=box.bound(1),i2b=box.bound(2),i3b=box.bound(3);for( int i3=box.base(3); i3<=i3b; i3++ )for( int i2=box.base(2); i2<=i2b; i2++ )for( int i1=box.base(1); i1<=i1b; i1++ )for( int i0=box.base(0); i0<=i0b; i0++ )

#define FOR_BOX_BOX_WITH_STRIDE(i0,i1,i2,i3,ibox,j0,j1,j2,j3,jbox)const int i0b=ibox.bound(0),i1b=ibox.bound(1),i2b=ibox.bound(2),i3b=ibox.bound(3);const int j0b=jbox.bound(0),j1b=jbox.bound(1),j2b=jbox.bound(2),j3b=jbox.bound(3);const int is0=D[0].getStride(), is1=D[1].getStride(), is2=D[2].getStride(), is3=D[3].getStride();const int js0=S[0].getStride(), js1=S[1].getStride(), js2=S[2].getStride(), js3=S[3].getStride();for( int i3=ibox.base(3),j3=jbox.base(3); i3<=i3b; i3+=is3, j3+=js3 )for( int i2=ibox.base(2),j2=jbox.base(2); i2<=i2b; i2+=is2, j2+=js2 )for( int i1=ibox.base(1),j1=jbox.base(1); i1<=i1b; i1+=is1, j1+=js1 )for( int i0=ibox.base(0),j0=jbox.base(0); i0<=i0b; i0+=is0, j0+=js0 )

#define FOR_BOX_WITH_STRIDE(i0,i1,i2,i3,stride0,stride1,stride2,stride3,box)const int i0b=box.bound(0),i1b=box.bound(1),i2b=box.bound(2),i3b=box.bound(3);const int is0=stride0, is1=stride1, is2=stride2, is3=stride3;for( int i3=box.base(3); i3<=i3b; i3+=is3 )for( int i2=box.base(2); i2<=i2b; i2+=is2 )for( int i1=box.base(1); i1<=i1b; i1+=is1 )for( int i0=box.base(0); i0<=i0b; i0+=is0 )

// **********************************************************
// *************** defineCopyMacro **************************
// **********************************************************

// defineCopyMacro(int,intArray,intSerialArray,MPI_INT);
int CopyArray::
copyArray( intArray & dest, Index *D0, 
         	   const intArray &  src, Index *S0, int nd /* =4 */ )
// ===================================================================================
//   Make the copy:
//            dest(D[0],D[1], ... D[nd-1]) = src( S[0],...,S[nd-1] ) 
//
//  This function supports strides, e.g. "dest(2:14:3) = src(1:9:2)". 
//  This function will assign parallel ghost values in dest. 
//  This function does not use parallel ghost values from src. 
//
//  This function is a replacement for the "ParallelUtility::copy" function that uses PARTI
// 
// /dest (input/output) : destination array
// /D0[] (input) : destination Index values (un-specified dimensions of dest will default to "all")
// /src (input) : source array
// /S0[] (input) : source Index values (un-specified dimensions of dest will default to "all")
// /nd (input) : number of Index's supplied (must be 4 for now)
//
//
// ===================================================================================
{
#ifdef USE_PPP
    int debug=0; // =7 
    const int myid = Communication_Manager::My_Process_Number;
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    const int numDim=4; // min(src.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    assert( numDim<=4 && nd==4 );
    Index D[4], S[4];
    for( int axis=0; axis<4; axis++ )
    {
        if( D0[axis].getLength()>0 )
        {
            D[axis]=D0[axis];
        }
        else
        {
            D[axis]=Range(dest.getBase(axis),dest.getBound(axis));
        }
        if( S0[axis].getLength()>0 )
        {
            S[axis]=S0[axis];
        }
        else
        {
            S[axis]=Range(src.getBase(axis),src.getBound(axis));
        }
    }
    for( int axis=0; axis<numDim; axis++ )
    {
        if( S[axis].getBase() <src.getBase(axis) -src.getGhostBoundaryWidth(axis) || 
                S[axis].getBound()>src.getBound(axis)+src.getGhostBoundaryWidth(axis) )
        {
            printf(" CopyArray::copy:ERROR Source index values are out of bounds!\n"
           	     "   axis=%i S=[%i,%i] but src [base,bound]=[%i,%i]\n",
           	     axis,S[axis].getBase(),S[axis].getBound(),src.getBase(axis),src.getBound(axis));
            OV_ABORT("error");
        }
        if( D[axis].getBase() <dest.getBase(axis) -dest.getGhostBoundaryWidth(axis) || 
                D[axis].getBound()>dest.getBound(axis)+dest.getGhostBoundaryWidth(axis) )
        {
            printf(" CopyArray::copyArray:ERROR Destination index values are out of bounds!\n"
           	     "   axis=%i D=[%i,%i] but dest [base,bound]=[%i,%i]\n",
           	     axis,D[axis].getBase(),D[axis].getBound(),dest.getBase(axis),dest.getBound(axis));
            OV_ABORT("CopyArray::error");
        }
        if( (D[axis].getBound()-D[axis].getBase())/D[axis].getStride() != 
                (S[axis].getBound()-S[axis].getBase())/S[axis].getStride() )
        {
            printf(" ParallelUtility::copy:ERROR non-conformable operation!\n"
           	     "   axis=%i src=[%i,%i,%i] with count=%i, but dest=[%i,%i,%i] with count=%i\n",
           	     axis,
                          D[axis].getBase(),D[axis].getBound(),D[axis].getStride(),
           	     (D[axis].getBound()-D[axis].getBase())/D[axis].getStride(),
                          S[axis].getBase(),S[axis].getBound(),S[axis].getStride(),
           	     (S[axis].getBound()-S[axis].getBase())/S[axis].getStride());
            OV_ABORT("CopyArray::error");
        }
    }
    bool copyOnProcessor=true;  // if true do not send messages to the same processor
    if( debug !=0 && debugFile==NULL )
    {
        char fileName[40];
        sprintf(fileName,"copyArray%i.debug",myid);
        debugFile= fopen(fileName,"w");
    }
    if( debug !=0 )
    {
        fprintf(debugFile,"++++ copyArray *start* myid=%i +++++\n",myid);
        fflush(debugFile);
    }
  // Example with strides
  //        dest(2:14:3) = src(1:9:2)
  //
  //      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+       src  : S=[1,9,2]  (stride=2)
  //            1     3     5     7     9   
  // 
  //      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ dest : D=[2,14,3] (stride=3)
  //         2        5        8       11       14
  // 
  // src : source array
  //   srcSubArray : box for [ S[0],S[1],S[2],S[3] ] = points to copy
  //   srcLocalArrayBox : box for srcLocal
  //   localSrcSubArray : srcSubArray .intersect. srcLocalArrayBox = local pts to copy 
  // Step 1. Determine the information to send
  // Here is the global src sub-array that we want to send:
    IndexBox srcSubArray(S[0].getBase(),S[0].getBound(),
                   		       S[1].getBase(),S[1].getBound(),
                   		       S[2].getBase(),S[2].getBound(),
                   		       S[3].getBase(),S[3].getBound());
  // Here is the global dest sub-array that we want to assign
    IndexBox destSubArray(D[0].getBase(),D[0].getBound(),
                  			D[1].getBase(),D[1].getBound(),
                  			D[2].getBase(),D[2].getBound(),
                  			D[3].getBase(),D[3].getBound());
    intSerialArray srcLocal; getLocalArrayWithGhostBoundaries(src,srcLocal);
    intSerialArray destLocal; getLocalArrayWithGhostBoundaries(dest,destLocal);
  // Here are the boxes for srcLocal and destLocal arrays:
    IndexBox srcLocalArrayBox, destLocalArrayBox;
    getLocalArrayBox( myid, src, srcLocalArrayBox ); 
    getLocalArrayBoxWithGhost( myid, dest, destLocalArrayBox ); // include ghost points in dest
    if( debug!=0 )
    {
        fprintf(debugFile,"copyArray: S=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n"
                    	              "           D=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
          	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
          	    S[1].getBase(),S[1].getBound(),S[1].getStride(),
          	    S[2].getBase(),S[2].getBound(),S[2].getStride(),
          	    S[3].getBase(),S[3].getBound(),S[3].getStride(),
          	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
          	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
          	    D[2].getBase(),D[2].getBound(),D[2].getStride(),
          	    D[3].getBase(),D[3].getBound(),D[3].getStride());
        fprintf(debugFile,"copyArray: myid=%i srcLocal bounds=[%i,%i][%i,%i][%i,%i][%i,%i] (no ghost)\n",
          	    myid,
          	    srcLocalArrayBox.base(0),srcLocalArrayBox.bound(0),
          	    srcLocalArrayBox.base(1),srcLocalArrayBox.bound(1),
          	    srcLocalArrayBox.base(2),srcLocalArrayBox.bound(2),
          	    srcLocalArrayBox.base(3),srcLocalArrayBox.bound(3)); 
        fprintf(debugFile,"copyArray: myid=%i destLocal bounds=[%i,%i][%i,%i][%i,%i][%i,%i] (with ghost)\n",
          	    myid,
          	    destLocalArrayBox.base(0),destLocalArrayBox.bound(0),
          	    destLocalArrayBox.base(1),destLocalArrayBox.bound(1),
          	    destLocalArrayBox.base(2),destLocalArrayBox.bound(2),
          	    destLocalArrayBox.base(3),destLocalArrayBox.bound(3)); 
        fflush(debugFile);
    }
    const int *srcp = srcLocal.Array_Descriptor.Array_View_Pointer3;
    const int srcDim0=srcLocal.getRawDataSize(0);
    const int srcDim1=srcLocal.getRawDataSize(1);
    const int srcDim2=srcLocal.getRawDataSize(2);
#undef SRC
#define SRC(i0,i1,i2,i3) srcp[i0+srcDim0*(i1+srcDim1*(i2+srcDim2*(i3)))]
    int *destp = destLocal.Array_Descriptor.Array_View_Pointer3;
    const int destDim0=destLocal.getRawDataSize(0);
    const int destDim1=destLocal.getRawDataSize(1);
    const int destDim2=destLocal.getRawDataSize(2);
#undef DEST
#define DEST(i0,i1,i2,i3) destp[i0+destDim0*(i1+destDim1*(i2+destDim2*(i3)))]
// ------------- round up or round down to the nearest stride-point -------------------------
#define roundUp(i,base,stride)   ( (((i)-base +stride-1)/stride)*stride+base )
#define roundDown(i,base,stride) ( (((i)-base          )/stride)*stride+base )
#define roundBound(box,axis,S) roundUp(box.base(axis) ,S[axis].getBase(),S[axis].getStride()),roundDown(box.bound(axis),S[axis].getBase(),S[axis].getStride()) 
#define roundBox(box,S) box.setBounds(roundBound(box,0,S),roundBound(box,1,S),roundBound(box,2,S),roundBound(box,3,S))
// --------------------- Conversion between src and dest index spaces -------------------------------------------
// srcToDest: convert an index i in the src to the corresponding index j in the dest, dest(j)=scr(i)
#define srcToDest(i,axis) ((((i) -S[axis].getBase())/S[axis].getStride())*D[axis].getStride()+D[axis].getBase())
#define destToSrc(i,axis) ((((i) -D[axis].getBase())/D[axis].getStride())*S[axis].getStride()+S[axis].getBase())
#define boundsSrcToDest(box,axis) srcToDest(box.base(axis),axis),srcToDest(box.bound(axis),axis)
#define boundsDestToSrc(box,axis) destToSrc(box.base(axis),axis),destToSrc(box.bound(axis),axis)
#define boxDestFromSrc(boxd,boxs) boxd.setBounds(boundsSrcToDest(boxs,0),boundsSrcToDest(boxs,1),boundsSrcToDest(boxs,2),boundsSrcToDest(boxs,3))
#define boxSrcFromDest(boxs,boxd) boxs.setBounds(boundsDestToSrc(boxd,0),boundsDestToSrc(boxd,1),boundsDestToSrc(boxd,2),boundsDestToSrc(boxd,3))
// ---------------------------------------------------------------------------------------------------------------
  // Here is the local src sub-array that we need to send
    IndexBox localSrcSubArray;
    bool notEmpty = IndexBox::intersect(srcLocalArrayBox, srcSubArray, localSrcSubArray );
  // adjust box to match stride-points 
    roundBox(localSrcSubArray,S);
    notEmpty = !localSrcSubArray.isEmpty();
  // ***********************************************************
  // **** make a list of boxes of where to send the data *******
  // ***********************************************************
    ListOfIndexBox sendBoxes;
    if( notEmpty )
    {
    // We need to send info from srcLocal.
    // For each processor p, intersect localSrcSubArray with destArrayLocalBoxOnProcessorP
        for( int p=0; p<np; p++ )
        {
      // destArrayLocalBoxOnProcessorP : bounds on local dest array on processor p 
            IndexBox destArrayLocalBoxOnProcessorP;
            getLocalArrayBoxWithGhost( p, dest, destArrayLocalBoxOnProcessorP );
      // Convert localSrcSubArray from src index-space to the dest index-space
            IndexBox srcInDest;
            boxDestFromSrc(srcInDest,localSrcSubArray);
            IndexBox destSendBox;
            notEmpty = IndexBox::intersect( srcInDest, destArrayLocalBoxOnProcessorP, destSendBox ); 
      // round box to stride-pts: 
            roundBox(destSendBox,D);
            notEmpty = !destSendBox.isEmpty();
            if( debug!=0 )
            {
                fprintf(debugFile,
                                " check for send to p=%i: localSrcSubArray=[%i,%i][%i,%i] srcInDest=[%i,%i][%i,%i]"
                                " dest-local-to-p=[%i,%i][%i,%i] destSendBox=[%i,%i][%i,%i] --> notEmpty=%i\n",
                                p,
            		localSrcSubArray.base(0),localSrcSubArray.bound(0),
            		localSrcSubArray.base(1),localSrcSubArray.bound(1),
                                srcInDest.base(0),srcInDest.bound(0),
                                srcInDest.base(1),srcInDest.bound(1),
                                destArrayLocalBoxOnProcessorP.base(0),destArrayLocalBoxOnProcessorP.bound(0),
                                destArrayLocalBoxOnProcessorP.base(1),destArrayLocalBoxOnProcessorP.bound(1),
                                destSendBox.base(0),destSendBox.bound(0),
                                destSendBox.base(1),destSendBox.bound(1),notEmpty);
            }
            if( notEmpty )
            {
        // convert destSendBox from dest index-space to src index-space, srcSendBox
                IndexBox srcSendBox;
                boxSrcFromDest(srcSendBox,destSendBox);
      	if( copyOnProcessor && myid==p )
      	{
	  // Do not send a message to the same processor -- just copy the data
        	  if( debug!=0 )
        	  {
          	    fprintf(debugFile,"****copyArray: myid=%i Just copy data on the same processor: *****\n"
                		    "   src=[%i,%i][%i,%i][%i,%i][%i,%i] \n"
                                        "  dest=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
                		    myid,
                		    srcSendBox.base(0),srcSendBox.bound(0),
                		    srcSendBox.base(1),srcSendBox.bound(1),
                		    srcSendBox.base(2),srcSendBox.bound(2),
                		    srcSendBox.base(3),srcSendBox.bound(3),
                                        destSendBox.base(0),destSendBox.bound(0),
                		    destSendBox.base(1),destSendBox.bound(1),
                		    destSendBox.base(2),destSendBox.bound(2),
                		    destSendBox.base(3),destSendBox.bound(3));
        	  }
	  // assign points defined in srcSendBox
        	  FOR_BOX_BOX_WITH_STRIDE(i0,i1,i2,i3,destSendBox, j0,j1,j2,j3,srcSendBox )  
        	  { 
          	    DEST(i0,i1,i2,i3)=SRC(j0,j1,j2,j3); 
        	  }
      	}
      	else 
      	{
        	  srcSendBox.processor=p;
        	  sendBoxes.push_back(srcSendBox);  // We need to send this box of data to processor p
      	}
            }
        }
        ListOfIndexBox::iterator iter; 
        for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
        {
            IndexBox & pSendBox = *iter;
            if( debug!=0 )
            {
      	fprintf(debugFile,"****copyArray:*****\n"
            		">>> myid=%i: Send box =[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i] to processor p=%i\n",
            		myid,
            		pSendBox.base(0),pSendBox.bound(0),S[0].getStride(),
            		pSendBox.base(1),pSendBox.bound(1),S[1].getStride(),
            		pSendBox.base(2),pSendBox.bound(2),S[2].getStride(),
            		pSendBox.base(3),pSendBox.bound(3),S[3].getStride(),
                                pSendBox.processor);
            }
        }
    }
  // ********************************************************************
  // *********** Step 2. Determine the information to receive ***********
  // ********************************************************************
    if( debug!=0 )
    {
        fprintf(debugFile,"copyArray: myid=%i receive data for : destLocalArrayBox=[%i,%i][%i,%i][%i,%i][%i,%i], "
                        "isEmpty=%i\n",
          	    myid,
          	    destLocalArrayBox.base(0),destLocalArrayBox.bound(0),
          	    destLocalArrayBox.base(1),destLocalArrayBox.bound(1),
          	    destLocalArrayBox.base(2),destLocalArrayBox.bound(2),
          	    destLocalArrayBox.base(3),destLocalArrayBox.bound(3),(int)destLocalArrayBox.isEmpty());
        fflush(debugFile);
    }
    ListOfIndexBox receiveBoxes;
  // Here is the local dest sub-array that we need to receive
    IndexBox localDestSubArray;
    notEmpty = IndexBox::intersect(destLocalArrayBox, destSubArray, localDestSubArray );
  // round box to stride-pts: 
    roundBox(localDestSubArray,D);
    notEmpty = !localDestSubArray.isEmpty();
    if( notEmpty )
    {
    // make a list of boxes of where to receive the data from
        if( debug!=0 )
        {
            fprintf(debugFile," Receive points: localDestSubArray=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
            	      localDestSubArray.base(0),localDestSubArray.bound(0),D[0].getStride(),
            	      localDestSubArray.base(1),localDestSubArray.bound(1),D[1].getStride(),
            	      localDestSubArray.base(2),localDestSubArray.bound(2),D[2].getStride(),
            	      localDestSubArray.base(3),localDestSubArray.bound(3),D[3].getStride());
        }
    // for each processor p, intersect localDestSubArray with srcLocalArrayBoxOnProcessorP
        for( int p=0; p<np; p++ )
        {
            if( copyOnProcessor && myid==p )
      	continue;  // the data has already been transfered between the processor and itself.
            IndexBox srcLocalArrayBoxOnProcessorP;  // defines the bounds of src on processor p
            CopyArray::getLocalArrayBox( p,src,srcLocalArrayBoxOnProcessorP );  // this is without ghost pts 
            if( debug!=0 )
            {
      	fprintf(debugFile,"copyArray: myid=%i srcLocalArrayBox=[%i,%i][%i,%i][%i,%i][%i,%i] on p=%i\n",
            		myid,
            		srcLocalArrayBoxOnProcessorP.base(0),srcLocalArrayBoxOnProcessorP.bound(0),
            		srcLocalArrayBoxOnProcessorP.base(1),srcLocalArrayBoxOnProcessorP.bound(1),
            		srcLocalArrayBoxOnProcessorP.base(2),srcLocalArrayBoxOnProcessorP.bound(2),
            		srcLocalArrayBoxOnProcessorP.base(3),srcLocalArrayBoxOnProcessorP.bound(3),p);
            }
      // convert localDestSubArray from dest index-space to src-index-space
            IndexBox destInSrc;
            boxSrcFromDest(destInSrc,localDestSubArray);
            IndexBox destReceiveBox;
            notEmpty = IndexBox::intersect(destInSrc,srcLocalArrayBoxOnProcessorP, destReceiveBox ); 
      // round box to stride-pts: 
            roundBox(destReceiveBox,S);
            notEmpty = !destReceiveBox.isEmpty();
            if( notEmpty )
            {
      	if( debug!=0 )
      	{
        	  fprintf(debugFile," myid=%i: Expecting to receive box =[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]"
                                    "from processor p=%i\n",
              		  myid,
              		  destReceiveBox.base(0),destReceiveBox.bound(0),S[0].getStride(),
              		  destReceiveBox.base(1),destReceiveBox.bound(1),S[1].getStride(),
              		  destReceiveBox.base(2),destReceiveBox.bound(2),S[2].getStride(),
              		  destReceiveBox.base(3),destReceiveBox.bound(3),S[3].getStride(),p);
      	}
        // convert destReceiveBox from src index-space to dest index-space
                boxDestFromSrc(destReceiveBox,destReceiveBox);
      	destReceiveBox.processor=p;
                receiveBoxes.push_back(destReceiveBox);
      	if( debug!=0 )
      	{
        	  fprintf(debugFile,"      --> in dest index-space box =[%i,%i][%i,%i][%i,%i][%i,%i] \n",
              		  destReceiveBox.base(0),destReceiveBox.bound(0),D[0].getStride(),
              		  destReceiveBox.base(1),destReceiveBox.bound(1),D[1].getStride(),
              		  destReceiveBox.base(2),destReceiveBox.bound(2),D[2].getStride(),
              		  destReceiveBox.base(3),destReceiveBox.bound(3),D[3].getStride());
      	}
            }
        }
    }
// Return the number points in a box accounting for the stride:
#define getStridedBoxSize(box,S)( ((box.bound(0)-box.base(0))/S[0].getStride()+1)* ((box.bound(1)-box.base(1))/S[1].getStride()+1)* ((box.bound(2)-box.base(2))/S[2].getStride()+1)* ((box.bound(3)-box.base(3))/S[3].getStride()+1) )
  // ************** MPI calls *****************
    if( debugFile!=NULL ) fflush(debugFile);
    const int numReceive=receiveBoxes.size();
    int **rBuff=NULL;   // buffers for receiving data
    MPI_Request *receiveRequest=NULL;
    MPI_Status *receiveStatus=NULL;
    int *receiveBoxIndex = new int [np];  // maps processor number to index in receiveBoxes
    for( int p=0; p<np; p++ ) receiveBoxIndex[p]=-1;
    if( numReceive>0 )
    {
    // ---- post receives first ----
        receiveRequest= new MPI_Request[numReceive]; // remember to delete these
        receiveStatus= new MPI_Status[numReceive]; 
        rBuff = new int* [numReceive];
    // int sendingProc = new int [numReceive];
        for(int m=0; m<numReceive; m++ )
        {
            IndexBox & pReceiveBox = receiveBoxes[m];
      // sendingProc[m]=pReceiveBox.processor;  // this processor will be sending the data
            int bufSize=getStridedBoxSize(pReceiveBox,D);  // we receive the strided data
            rBuff[m]= new int [bufSize];
            assert( pReceiveBox.processor>=0 && pReceiveBox.processor<np );
            receiveBoxIndex[pReceiveBox.processor]=m;  // maps processor number to index in receiveBoxes
            if( debug!=0 )
            {
      	fprintf(debugFile,">>> myid=%i: post a receive for buffer of size %i from p=%i (m=%i,numReceive=%i) \n",
                                myid,bufSize,pReceiveBox.processor,m,numReceive);
            }
            MPI_Irecv(rBuff[m],bufSize,MPI_INT,pReceiveBox.processor,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
        }
    }
  // ----- Now send the data -----
    const int numSend=sendBoxes.size();
    MPI_Request *sendRequest=NULL;
    int **sBuff=NULL;
    if( numSend>0 )
    {
        sendRequest= new MPI_Request[numSend]; // remember to delete these
        sBuff = new int* [numSend];
        for(int m=0; m<numSend; m++ )
        {
      // NOTE: we only send the strided data 
            IndexBox & pSendBox = sendBoxes[m]; 
            int bufSize=getStridedBoxSize(pSendBox,S);
            sBuff[m]= new int [bufSize];
            int *buff=sBuff[m];
            int i=0;
            FOR_BOX_WITH_STRIDE(i0,i1,i2,i3,S[0].getStride(),S[1].getStride(),S[2].getStride(),S[3].getStride(),pSendBox)
            {
      	buff[i]=SRC(i0,i1,i2,i3); 
      	i++;
            }
            if( debug!=0 )
            {
      	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
            }
            MPI_Isend(sBuff[m],bufSize,MPI_INT,pSendBox.processor,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
        }
    }
    if( numReceive>0 )
    {
        MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to receive all messages
        for(int m=0; m<numReceive; m++  )
        {
            int bufSize=receiveStatus[m].MPI_TAG;
            int p = receiveStatus[m].MPI_SOURCE;
            assert( p>=0 && p<np );
            if( debug!=0 )
            {
      	fprintf(debugFile,">>> myid=%i: received buffer of size %i from p=%i (m=%i,numReceive=%i)\n myid=%i: buff=",
                        myid,bufSize,p,m,numReceive,myid);
      	for( int j=0; j<bufSize; j++ ) fprintf(debugFile,"%3.1f ",rBuff[m][j]);
      	fprintf(debugFile,"\n");
            }
      // fill in the entries of vLocal
            int n = receiveBoxIndex[p];
            assert( n>=0 && n<numReceive );
      // Question: is n==m always???
            IndexBox & rBox = receiveBoxes[n];
            assert( rBox.processor==p );
      // assign dest(rBox) = rBuff[m][0...]
            const int *buff = rBuff[m];
            int i=0;
            FOR_BOX_WITH_STRIDE(i0,i1,i2,i3,D[0].getStride(),D[1].getStride(),D[2].getStride(),D[3].getStride(),rBox)
            {
      	DEST(i0,i1,i2,i3)=buff[i];   
      	i++;
            }
        }
    }
//   if( debug & 2 )
//   {
//     ListOfIndexBox::iterator iter; 
//     for(int m=0; iter = receiveBoxes.begin(); m++, iter!=receiveBoxes.end(); iter++ )
//     {
//       IndexBox & pReceiveBox = *iter;
//       int bufSize=receiveStatus[m].MPI_TAG;
//       fprintf(debugFile,"<- processor %i: received msg from processor %i, tag=%i p=%i values=",myID,
// 	      receiveStatus[m].MPI_SOURCE,receiveStatus[m].MPI_TAG,p);
//       for( j=0; j<nivd; j++ ) fprintf(debugFile,"%8.2e ",dbuff[p][j]);
//       fprintf(debugFile,"\n");
//       }
//     }
//   }
  // wait to send messages before deleting buffers
    if( numSend>0 )
    {
        if( debug!=0 )
        {
            fprintf(debugFile,"+++ myid=%i: wait for all messges to be sent, numSend=%i\n",myid,numSend); 
            fflush(debugFile);
        }
        MPI_Status *sendStatus = new MPI_Status[numSend]; 
        MPI_Waitall( numSend, sendRequest, sendStatus );   
        delete [] sendStatus;
    }
    if( debug!=0 )
    {
        fprintf(debugFile,"+++ myid=%i: cleanup buffers...\n",myid); 
        fflush(debugFile);
    }
    for (int i=0; i<numReceive; i++ )
    {
        delete [] rBuff[i];
    }
    delete [] rBuff;
    delete [] receiveStatus;
    delete [] receiveRequest;
    delete [] receiveBoxIndex;
    for (int i=0; i<numSend; i++ )
    {
        delete [] sBuff[i];
    }
    delete [] sBuff;
    delete [] sendRequest;
    if( debugFile!=NULL )
    {
        fprintf(debugFile,"**** myid=%i finished in copyArray ****\n\n",myid);
        fflush(debugFile);
    }
    return 0;
#else
  // **** Here is the serial version  ****
    Index *D=D0, *S=S0;
    switch (nd)
    {
    case 1:
        dest(D[0]) = src(S[0]);
        break;
    case 2:
        dest(D[0],D[1]) = src(S[0],S[1]);
        break;
    case 3:
        dest(D[0],D[1],D[2]) = src(S[0],S[1],S[2]);
        break;
    case 4:
        dest(D[0],D[1],D[2],D[3]) = src(S[0],S[1],S[2],S[3]);
        break;
    case 5:
        dest(D[0],D[1],D[2],D[3],D[4]) = src(S[0],S[1],S[2],S[3],S[4]);
        break;
    case 6:
        dest(D[0],D[1],D[2],D[3],D[4],D[5]) = src(S[0],S[1],S[2],S[3],S[4],S[5]);
        break;
//    case 7:
//      dest(D[0],D[1],D[2],D[3],D[4],D[5],D[6]) = src(S[0],S[1],S[2],S[3],S[4],S[5],S[6]);
//      break;
//    case 8:
//      dest(D[0],D[1],D[2],D[3],D[4],D[5],D[6],D[7]) = src(S[0],S[1],S[2],S[3],S[4],S[5],S[6],S[7]);
//      break;
    default:
        Overture::abort("ERROR: nd to large");
        break;
    }
#endif
    return 0;
}

// defineCopyMacro(float,floatArray,floatSerialArray,MPI_FLOAT);
int CopyArray::
copyArray( floatArray & dest, Index *D0, 
         	   const floatArray &  src, Index *S0, int nd /* =4 */ )
// ===================================================================================
//   Make the copy:
//            dest(D[0],D[1], ... D[nd-1]) = src( S[0],...,S[nd-1] ) 
//
//  This function supports strides, e.g. "dest(2:14:3) = src(1:9:2)". 
//  This function will assign parallel ghost values in dest. 
//  This function does not use parallel ghost values from src. 
//
//  This function is a replacement for the "ParallelUtility::copy" function that uses PARTI
// 
// /dest (input/output) : destination array
// /D0[] (input) : destination Index values (un-specified dimensions of dest will default to "all")
// /src (input) : source array
// /S0[] (input) : source Index values (un-specified dimensions of dest will default to "all")
// /nd (input) : number of Index's supplied (must be 4 for now)
//
//
// ===================================================================================
{
#ifdef USE_PPP
    int debug=0; // =7 
    const int myid = Communication_Manager::My_Process_Number;
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    const int numDim=4; // min(src.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    assert( numDim<=4 && nd==4 );
    Index D[4], S[4];
    for( int axis=0; axis<4; axis++ )
    {
        if( D0[axis].getLength()>0 )
        {
            D[axis]=D0[axis];
        }
        else
        {
            D[axis]=Range(dest.getBase(axis),dest.getBound(axis));
        }
        if( S0[axis].getLength()>0 )
        {
            S[axis]=S0[axis];
        }
        else
        {
            S[axis]=Range(src.getBase(axis),src.getBound(axis));
        }
    }
    for( int axis=0; axis<numDim; axis++ )
    {
        if( S[axis].getBase() <src.getBase(axis) -src.getGhostBoundaryWidth(axis) || 
                S[axis].getBound()>src.getBound(axis)+src.getGhostBoundaryWidth(axis) )
        {
            printf(" CopyArray::copy:ERROR Source index values are out of bounds!\n"
           	     "   axis=%i S=[%i,%i] but src [base,bound]=[%i,%i]\n",
           	     axis,S[axis].getBase(),S[axis].getBound(),src.getBase(axis),src.getBound(axis));
            OV_ABORT("error");
        }
        if( D[axis].getBase() <dest.getBase(axis) -dest.getGhostBoundaryWidth(axis) || 
                D[axis].getBound()>dest.getBound(axis)+dest.getGhostBoundaryWidth(axis) )
        {
            printf(" CopyArray::copyArray:ERROR Destination index values are out of bounds!\n"
           	     "   axis=%i D=[%i,%i] but dest [base,bound]=[%i,%i]\n",
           	     axis,D[axis].getBase(),D[axis].getBound(),dest.getBase(axis),dest.getBound(axis));
            OV_ABORT("CopyArray::error");
        }
        if( (D[axis].getBound()-D[axis].getBase())/D[axis].getStride() != 
                (S[axis].getBound()-S[axis].getBase())/S[axis].getStride() )
        {
            printf(" ParallelUtility::copy:ERROR non-conformable operation!\n"
           	     "   axis=%i src=[%i,%i,%i] with count=%i, but dest=[%i,%i,%i] with count=%i\n",
           	     axis,
                          D[axis].getBase(),D[axis].getBound(),D[axis].getStride(),
           	     (D[axis].getBound()-D[axis].getBase())/D[axis].getStride(),
                          S[axis].getBase(),S[axis].getBound(),S[axis].getStride(),
           	     (S[axis].getBound()-S[axis].getBase())/S[axis].getStride());
            OV_ABORT("CopyArray::error");
        }
    }
    bool copyOnProcessor=true;  // if true do not send messages to the same processor
    if( debug !=0 && debugFile==NULL )
    {
        char fileName[40];
        sprintf(fileName,"copyArray%i.debug",myid);
        debugFile= fopen(fileName,"w");
    }
    if( debug !=0 )
    {
        fprintf(debugFile,"++++ copyArray *start* myid=%i +++++\n",myid);
        fflush(debugFile);
    }
  // Example with strides
  //        dest(2:14:3) = src(1:9:2)
  //
  //      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+       src  : S=[1,9,2]  (stride=2)
  //            1     3     5     7     9   
  // 
  //      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ dest : D=[2,14,3] (stride=3)
  //         2        5        8       11       14
  // 
  // src : source array
  //   srcSubArray : box for [ S[0],S[1],S[2],S[3] ] = points to copy
  //   srcLocalArrayBox : box for srcLocal
  //   localSrcSubArray : srcSubArray .intersect. srcLocalArrayBox = local pts to copy 
  // Step 1. Determine the information to send
  // Here is the global src sub-array that we want to send:
    IndexBox srcSubArray(S[0].getBase(),S[0].getBound(),
                   		       S[1].getBase(),S[1].getBound(),
                   		       S[2].getBase(),S[2].getBound(),
                   		       S[3].getBase(),S[3].getBound());
  // Here is the global dest sub-array that we want to assign
    IndexBox destSubArray(D[0].getBase(),D[0].getBound(),
                  			D[1].getBase(),D[1].getBound(),
                  			D[2].getBase(),D[2].getBound(),
                  			D[3].getBase(),D[3].getBound());
    floatSerialArray srcLocal; getLocalArrayWithGhostBoundaries(src,srcLocal);
    floatSerialArray destLocal; getLocalArrayWithGhostBoundaries(dest,destLocal);
  // Here are the boxes for srcLocal and destLocal arrays:
    IndexBox srcLocalArrayBox, destLocalArrayBox;
    getLocalArrayBox( myid, src, srcLocalArrayBox ); 
    getLocalArrayBoxWithGhost( myid, dest, destLocalArrayBox ); // include ghost points in dest
    if( debug!=0 )
    {
        fprintf(debugFile,"copyArray: S=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n"
                    	              "           D=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
          	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
          	    S[1].getBase(),S[1].getBound(),S[1].getStride(),
          	    S[2].getBase(),S[2].getBound(),S[2].getStride(),
          	    S[3].getBase(),S[3].getBound(),S[3].getStride(),
          	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
          	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
          	    D[2].getBase(),D[2].getBound(),D[2].getStride(),
          	    D[3].getBase(),D[3].getBound(),D[3].getStride());
        fprintf(debugFile,"copyArray: myid=%i srcLocal bounds=[%i,%i][%i,%i][%i,%i][%i,%i] (no ghost)\n",
          	    myid,
          	    srcLocalArrayBox.base(0),srcLocalArrayBox.bound(0),
          	    srcLocalArrayBox.base(1),srcLocalArrayBox.bound(1),
          	    srcLocalArrayBox.base(2),srcLocalArrayBox.bound(2),
          	    srcLocalArrayBox.base(3),srcLocalArrayBox.bound(3)); 
        fprintf(debugFile,"copyArray: myid=%i destLocal bounds=[%i,%i][%i,%i][%i,%i][%i,%i] (with ghost)\n",
          	    myid,
          	    destLocalArrayBox.base(0),destLocalArrayBox.bound(0),
          	    destLocalArrayBox.base(1),destLocalArrayBox.bound(1),
          	    destLocalArrayBox.base(2),destLocalArrayBox.bound(2),
          	    destLocalArrayBox.base(3),destLocalArrayBox.bound(3)); 
        fflush(debugFile);
    }
    const float *srcp = srcLocal.Array_Descriptor.Array_View_Pointer3;
    const int srcDim0=srcLocal.getRawDataSize(0);
    const int srcDim1=srcLocal.getRawDataSize(1);
    const int srcDim2=srcLocal.getRawDataSize(2);
#undef SRC
#define SRC(i0,i1,i2,i3) srcp[i0+srcDim0*(i1+srcDim1*(i2+srcDim2*(i3)))]
    float *destp = destLocal.Array_Descriptor.Array_View_Pointer3;
    const int destDim0=destLocal.getRawDataSize(0);
    const int destDim1=destLocal.getRawDataSize(1);
    const int destDim2=destLocal.getRawDataSize(2);
#undef DEST
#define DEST(i0,i1,i2,i3) destp[i0+destDim0*(i1+destDim1*(i2+destDim2*(i3)))]
// ------------- round up or round down to the nearest stride-point -------------------------
#define roundUp(i,base,stride)   ( (((i)-base +stride-1)/stride)*stride+base )
#define roundDown(i,base,stride) ( (((i)-base          )/stride)*stride+base )
#define roundBound(box,axis,S) roundUp(box.base(axis) ,S[axis].getBase(),S[axis].getStride()),roundDown(box.bound(axis),S[axis].getBase(),S[axis].getStride()) 
#define roundBox(box,S) box.setBounds(roundBound(box,0,S),roundBound(box,1,S),roundBound(box,2,S),roundBound(box,3,S))
// --------------------- Conversion between src and dest index spaces -------------------------------------------
// srcToDest: convert an index i in the src to the corresponding index j in the dest, dest(j)=scr(i)
#define srcToDest(i,axis) ((((i) -S[axis].getBase())/S[axis].getStride())*D[axis].getStride()+D[axis].getBase())
#define destToSrc(i,axis) ((((i) -D[axis].getBase())/D[axis].getStride())*S[axis].getStride()+S[axis].getBase())
#define boundsSrcToDest(box,axis) srcToDest(box.base(axis),axis),srcToDest(box.bound(axis),axis)
#define boundsDestToSrc(box,axis) destToSrc(box.base(axis),axis),destToSrc(box.bound(axis),axis)
#define boxDestFromSrc(boxd,boxs) boxd.setBounds(boundsSrcToDest(boxs,0),boundsSrcToDest(boxs,1),boundsSrcToDest(boxs,2),boundsSrcToDest(boxs,3))
#define boxSrcFromDest(boxs,boxd) boxs.setBounds(boundsDestToSrc(boxd,0),boundsDestToSrc(boxd,1),boundsDestToSrc(boxd,2),boundsDestToSrc(boxd,3))
// ---------------------------------------------------------------------------------------------------------------
  // Here is the local src sub-array that we need to send
    IndexBox localSrcSubArray;
    bool notEmpty = IndexBox::intersect(srcLocalArrayBox, srcSubArray, localSrcSubArray );
  // adjust box to match stride-points 
    roundBox(localSrcSubArray,S);
    notEmpty = !localSrcSubArray.isEmpty();
  // ***********************************************************
  // **** make a list of boxes of where to send the data *******
  // ***********************************************************
    ListOfIndexBox sendBoxes;
    if( notEmpty )
    {
    // We need to send info from srcLocal.
    // For each processor p, intersect localSrcSubArray with destArrayLocalBoxOnProcessorP
        for( int p=0; p<np; p++ )
        {
      // destArrayLocalBoxOnProcessorP : bounds on local dest array on processor p 
            IndexBox destArrayLocalBoxOnProcessorP;
            getLocalArrayBoxWithGhost( p, dest, destArrayLocalBoxOnProcessorP );
      // Convert localSrcSubArray from src index-space to the dest index-space
            IndexBox srcInDest;
            boxDestFromSrc(srcInDest,localSrcSubArray);
            IndexBox destSendBox;
            notEmpty = IndexBox::intersect( srcInDest, destArrayLocalBoxOnProcessorP, destSendBox ); 
      // round box to stride-pts: 
            roundBox(destSendBox,D);
            notEmpty = !destSendBox.isEmpty();
            if( debug!=0 )
            {
                fprintf(debugFile,
                                " check for send to p=%i: localSrcSubArray=[%i,%i][%i,%i] srcInDest=[%i,%i][%i,%i]"
                                " dest-local-to-p=[%i,%i][%i,%i] destSendBox=[%i,%i][%i,%i] --> notEmpty=%i\n",
                                p,
            		localSrcSubArray.base(0),localSrcSubArray.bound(0),
            		localSrcSubArray.base(1),localSrcSubArray.bound(1),
                                srcInDest.base(0),srcInDest.bound(0),
                                srcInDest.base(1),srcInDest.bound(1),
                                destArrayLocalBoxOnProcessorP.base(0),destArrayLocalBoxOnProcessorP.bound(0),
                                destArrayLocalBoxOnProcessorP.base(1),destArrayLocalBoxOnProcessorP.bound(1),
                                destSendBox.base(0),destSendBox.bound(0),
                                destSendBox.base(1),destSendBox.bound(1),notEmpty);
            }
            if( notEmpty )
            {
        // convert destSendBox from dest index-space to src index-space, srcSendBox
                IndexBox srcSendBox;
                boxSrcFromDest(srcSendBox,destSendBox);
      	if( copyOnProcessor && myid==p )
      	{
	  // Do not send a message to the same processor -- just copy the data
        	  if( debug!=0 )
        	  {
          	    fprintf(debugFile,"****copyArray: myid=%i Just copy data on the same processor: *****\n"
                		    "   src=[%i,%i][%i,%i][%i,%i][%i,%i] \n"
                                        "  dest=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
                		    myid,
                		    srcSendBox.base(0),srcSendBox.bound(0),
                		    srcSendBox.base(1),srcSendBox.bound(1),
                		    srcSendBox.base(2),srcSendBox.bound(2),
                		    srcSendBox.base(3),srcSendBox.bound(3),
                                        destSendBox.base(0),destSendBox.bound(0),
                		    destSendBox.base(1),destSendBox.bound(1),
                		    destSendBox.base(2),destSendBox.bound(2),
                		    destSendBox.base(3),destSendBox.bound(3));
        	  }
	  // assign points defined in srcSendBox
        	  FOR_BOX_BOX_WITH_STRIDE(i0,i1,i2,i3,destSendBox, j0,j1,j2,j3,srcSendBox )  
        	  { 
          	    DEST(i0,i1,i2,i3)=SRC(j0,j1,j2,j3); 
        	  }
      	}
      	else 
      	{
        	  srcSendBox.processor=p;
        	  sendBoxes.push_back(srcSendBox);  // We need to send this box of data to processor p
      	}
            }
        }
        ListOfIndexBox::iterator iter; 
        for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
        {
            IndexBox & pSendBox = *iter;
            if( debug!=0 )
            {
      	fprintf(debugFile,"****copyArray:*****\n"
            		">>> myid=%i: Send box =[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i] to processor p=%i\n",
            		myid,
            		pSendBox.base(0),pSendBox.bound(0),S[0].getStride(),
            		pSendBox.base(1),pSendBox.bound(1),S[1].getStride(),
            		pSendBox.base(2),pSendBox.bound(2),S[2].getStride(),
            		pSendBox.base(3),pSendBox.bound(3),S[3].getStride(),
                                pSendBox.processor);
            }
        }
    }
  // ********************************************************************
  // *********** Step 2. Determine the information to receive ***********
  // ********************************************************************
    if( debug!=0 )
    {
        fprintf(debugFile,"copyArray: myid=%i receive data for : destLocalArrayBox=[%i,%i][%i,%i][%i,%i][%i,%i], "
                        "isEmpty=%i\n",
          	    myid,
          	    destLocalArrayBox.base(0),destLocalArrayBox.bound(0),
          	    destLocalArrayBox.base(1),destLocalArrayBox.bound(1),
          	    destLocalArrayBox.base(2),destLocalArrayBox.bound(2),
          	    destLocalArrayBox.base(3),destLocalArrayBox.bound(3),(int)destLocalArrayBox.isEmpty());
        fflush(debugFile);
    }
    ListOfIndexBox receiveBoxes;
  // Here is the local dest sub-array that we need to receive
    IndexBox localDestSubArray;
    notEmpty = IndexBox::intersect(destLocalArrayBox, destSubArray, localDestSubArray );
  // round box to stride-pts: 
    roundBox(localDestSubArray,D);
    notEmpty = !localDestSubArray.isEmpty();
    if( notEmpty )
    {
    // make a list of boxes of where to receive the data from
        if( debug!=0 )
        {
            fprintf(debugFile," Receive points: localDestSubArray=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
            	      localDestSubArray.base(0),localDestSubArray.bound(0),D[0].getStride(),
            	      localDestSubArray.base(1),localDestSubArray.bound(1),D[1].getStride(),
            	      localDestSubArray.base(2),localDestSubArray.bound(2),D[2].getStride(),
            	      localDestSubArray.base(3),localDestSubArray.bound(3),D[3].getStride());
        }
    // for each processor p, intersect localDestSubArray with srcLocalArrayBoxOnProcessorP
        for( int p=0; p<np; p++ )
        {
            if( copyOnProcessor && myid==p )
      	continue;  // the data has already been transfered between the processor and itself.
            IndexBox srcLocalArrayBoxOnProcessorP;  // defines the bounds of src on processor p
            CopyArray::getLocalArrayBox( p,src,srcLocalArrayBoxOnProcessorP );  // this is without ghost pts 
            if( debug!=0 )
            {
      	fprintf(debugFile,"copyArray: myid=%i srcLocalArrayBox=[%i,%i][%i,%i][%i,%i][%i,%i] on p=%i\n",
            		myid,
            		srcLocalArrayBoxOnProcessorP.base(0),srcLocalArrayBoxOnProcessorP.bound(0),
            		srcLocalArrayBoxOnProcessorP.base(1),srcLocalArrayBoxOnProcessorP.bound(1),
            		srcLocalArrayBoxOnProcessorP.base(2),srcLocalArrayBoxOnProcessorP.bound(2),
            		srcLocalArrayBoxOnProcessorP.base(3),srcLocalArrayBoxOnProcessorP.bound(3),p);
            }
      // convert localDestSubArray from dest index-space to src-index-space
            IndexBox destInSrc;
            boxSrcFromDest(destInSrc,localDestSubArray);
            IndexBox destReceiveBox;
            notEmpty = IndexBox::intersect(destInSrc,srcLocalArrayBoxOnProcessorP, destReceiveBox ); 
      // round box to stride-pts: 
            roundBox(destReceiveBox,S);
            notEmpty = !destReceiveBox.isEmpty();
            if( notEmpty )
            {
      	if( debug!=0 )
      	{
        	  fprintf(debugFile," myid=%i: Expecting to receive box =[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]"
                                    "from processor p=%i\n",
              		  myid,
              		  destReceiveBox.base(0),destReceiveBox.bound(0),S[0].getStride(),
              		  destReceiveBox.base(1),destReceiveBox.bound(1),S[1].getStride(),
              		  destReceiveBox.base(2),destReceiveBox.bound(2),S[2].getStride(),
              		  destReceiveBox.base(3),destReceiveBox.bound(3),S[3].getStride(),p);
      	}
        // convert destReceiveBox from src index-space to dest index-space
                boxDestFromSrc(destReceiveBox,destReceiveBox);
      	destReceiveBox.processor=p;
                receiveBoxes.push_back(destReceiveBox);
      	if( debug!=0 )
      	{
        	  fprintf(debugFile,"      --> in dest index-space box =[%i,%i][%i,%i][%i,%i][%i,%i] \n",
              		  destReceiveBox.base(0),destReceiveBox.bound(0),D[0].getStride(),
              		  destReceiveBox.base(1),destReceiveBox.bound(1),D[1].getStride(),
              		  destReceiveBox.base(2),destReceiveBox.bound(2),D[2].getStride(),
              		  destReceiveBox.base(3),destReceiveBox.bound(3),D[3].getStride());
      	}
            }
        }
    }
// Return the number points in a box accounting for the stride:
#define getStridedBoxSize(box,S)( ((box.bound(0)-box.base(0))/S[0].getStride()+1)* ((box.bound(1)-box.base(1))/S[1].getStride()+1)* ((box.bound(2)-box.base(2))/S[2].getStride()+1)* ((box.bound(3)-box.base(3))/S[3].getStride()+1) )
  // ************** MPI calls *****************
    if( debugFile!=NULL ) fflush(debugFile);
    const int numReceive=receiveBoxes.size();
    float **rBuff=NULL;   // buffers for receiving data
    MPI_Request *receiveRequest=NULL;
    MPI_Status *receiveStatus=NULL;
    int *receiveBoxIndex = new int [np];  // maps processor number to index in receiveBoxes
    for( int p=0; p<np; p++ ) receiveBoxIndex[p]=-1;
    if( numReceive>0 )
    {
    // ---- post receives first ----
        receiveRequest= new MPI_Request[numReceive]; // remember to delete these
        receiveStatus= new MPI_Status[numReceive]; 
        rBuff = new float* [numReceive];
    // int sendingProc = new int [numReceive];
        for(int m=0; m<numReceive; m++ )
        {
            IndexBox & pReceiveBox = receiveBoxes[m];
      // sendingProc[m]=pReceiveBox.processor;  // this processor will be sending the data
            int bufSize=getStridedBoxSize(pReceiveBox,D);  // we receive the strided data
            rBuff[m]= new float [bufSize];
            assert( pReceiveBox.processor>=0 && pReceiveBox.processor<np );
            receiveBoxIndex[pReceiveBox.processor]=m;  // maps processor number to index in receiveBoxes
            if( debug!=0 )
            {
      	fprintf(debugFile,">>> myid=%i: post a receive for buffer of size %i from p=%i (m=%i,numReceive=%i) \n",
                                myid,bufSize,pReceiveBox.processor,m,numReceive);
            }
            MPI_Irecv(rBuff[m],bufSize,MPI_FLOAT,pReceiveBox.processor,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
        }
    }
  // ----- Now send the data -----
    const int numSend=sendBoxes.size();
    MPI_Request *sendRequest=NULL;
    float **sBuff=NULL;
    if( numSend>0 )
    {
        sendRequest= new MPI_Request[numSend]; // remember to delete these
        sBuff = new float* [numSend];
        for(int m=0; m<numSend; m++ )
        {
      // NOTE: we only send the strided data 
            IndexBox & pSendBox = sendBoxes[m]; 
            int bufSize=getStridedBoxSize(pSendBox,S);
            sBuff[m]= new float [bufSize];
            float *buff=sBuff[m];
            int i=0;
            FOR_BOX_WITH_STRIDE(i0,i1,i2,i3,S[0].getStride(),S[1].getStride(),S[2].getStride(),S[3].getStride(),pSendBox)
            {
      	buff[i]=SRC(i0,i1,i2,i3); 
      	i++;
            }
            if( debug!=0 )
            {
      	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
            }
            MPI_Isend(sBuff[m],bufSize,MPI_FLOAT,pSendBox.processor,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
        }
    }
    if( numReceive>0 )
    {
        MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to receive all messages
        for(int m=0; m<numReceive; m++  )
        {
            int bufSize=receiveStatus[m].MPI_TAG;
            int p = receiveStatus[m].MPI_SOURCE;
            assert( p>=0 && p<np );
            if( debug!=0 )
            {
      	fprintf(debugFile,">>> myid=%i: received buffer of size %i from p=%i (m=%i,numReceive=%i)\n myid=%i: buff=",
                        myid,bufSize,p,m,numReceive,myid);
      	for( int j=0; j<bufSize; j++ ) fprintf(debugFile,"%3.1f ",rBuff[m][j]);
      	fprintf(debugFile,"\n");
            }
      // fill in the entries of vLocal
            int n = receiveBoxIndex[p];
            assert( n>=0 && n<numReceive );
      // Question: is n==m always???
            IndexBox & rBox = receiveBoxes[n];
            assert( rBox.processor==p );
      // assign dest(rBox) = rBuff[m][0...]
            const float *buff = rBuff[m];
            int i=0;
            FOR_BOX_WITH_STRIDE(i0,i1,i2,i3,D[0].getStride(),D[1].getStride(),D[2].getStride(),D[3].getStride(),rBox)
            {
      	DEST(i0,i1,i2,i3)=buff[i];   
      	i++;
            }
        }
    }
//   if( debug & 2 )
//   {
//     ListOfIndexBox::iterator iter; 
//     for(int m=0; iter = receiveBoxes.begin(); m++, iter!=receiveBoxes.end(); iter++ )
//     {
//       IndexBox & pReceiveBox = *iter;
//       int bufSize=receiveStatus[m].MPI_TAG;
//       fprintf(debugFile,"<- processor %i: received msg from processor %i, tag=%i p=%i values=",myID,
// 	      receiveStatus[m].MPI_SOURCE,receiveStatus[m].MPI_TAG,p);
//       for( j=0; j<nivd; j++ ) fprintf(debugFile,"%8.2e ",dbuff[p][j]);
//       fprintf(debugFile,"\n");
//       }
//     }
//   }
  // wait to send messages before deleting buffers
    if( numSend>0 )
    {
        if( debug!=0 )
        {
            fprintf(debugFile,"+++ myid=%i: wait for all messges to be sent, numSend=%i\n",myid,numSend); 
            fflush(debugFile);
        }
        MPI_Status *sendStatus = new MPI_Status[numSend]; 
        MPI_Waitall( numSend, sendRequest, sendStatus );   
        delete [] sendStatus;
    }
    if( debug!=0 )
    {
        fprintf(debugFile,"+++ myid=%i: cleanup buffers...\n",myid); 
        fflush(debugFile);
    }
    for (int i=0; i<numReceive; i++ )
    {
        delete [] rBuff[i];
    }
    delete [] rBuff;
    delete [] receiveStatus;
    delete [] receiveRequest;
    delete [] receiveBoxIndex;
    for (int i=0; i<numSend; i++ )
    {
        delete [] sBuff[i];
    }
    delete [] sBuff;
    delete [] sendRequest;
    if( debugFile!=NULL )
    {
        fprintf(debugFile,"**** myid=%i finished in copyArray ****\n\n",myid);
        fflush(debugFile);
    }
    return 0;
#else
  // **** Here is the serial version  ****
    Index *D=D0, *S=S0;
    switch (nd)
    {
    case 1:
        dest(D[0]) = src(S[0]);
        break;
    case 2:
        dest(D[0],D[1]) = src(S[0],S[1]);
        break;
    case 3:
        dest(D[0],D[1],D[2]) = src(S[0],S[1],S[2]);
        break;
    case 4:
        dest(D[0],D[1],D[2],D[3]) = src(S[0],S[1],S[2],S[3]);
        break;
    case 5:
        dest(D[0],D[1],D[2],D[3],D[4]) = src(S[0],S[1],S[2],S[3],S[4]);
        break;
    case 6:
        dest(D[0],D[1],D[2],D[3],D[4],D[5]) = src(S[0],S[1],S[2],S[3],S[4],S[5]);
        break;
//    case 7:
//      dest(D[0],D[1],D[2],D[3],D[4],D[5],D[6]) = src(S[0],S[1],S[2],S[3],S[4],S[5],S[6]);
//      break;
//    case 8:
//      dest(D[0],D[1],D[2],D[3],D[4],D[5],D[6],D[7]) = src(S[0],S[1],S[2],S[3],S[4],S[5],S[6],S[7]);
//      break;
    default:
        Overture::abort("ERROR: nd to large");
        break;
    }
#endif
    return 0;
}

// defineCopyMacro(double,doubleArray,doubleSerialArray,MPI_DOUBLE);
int CopyArray::
copyArray( doubleArray & dest, Index *D0, 
         	   const doubleArray &  src, Index *S0, int nd /* =4 */ )
// ===================================================================================
//   Make the copy:
//            dest(D[0],D[1], ... D[nd-1]) = src( S[0],...,S[nd-1] ) 
//
//  This function supports strides, e.g. "dest(2:14:3) = src(1:9:2)". 
//  This function will assign parallel ghost values in dest. 
//  This function does not use parallel ghost values from src. 
//
//  This function is a replacement for the "ParallelUtility::copy" function that uses PARTI
// 
// /dest (input/output) : destination array
// /D0[] (input) : destination Index values (un-specified dimensions of dest will default to "all")
// /src (input) : source array
// /S0[] (input) : source Index values (un-specified dimensions of dest will default to "all")
// /nd (input) : number of Index's supplied (must be 4 for now)
//
//
// ===================================================================================
{
#ifdef USE_PPP
    int debug=0; // =7 
    const int myid = Communication_Manager::My_Process_Number;
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    const int numDim=4; // min(src.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    assert( numDim<=4 && nd==4 );
    Index D[4], S[4];
    for( int axis=0; axis<4; axis++ )
    {
        if( D0[axis].getLength()>0 )
        {
            D[axis]=D0[axis];
        }
        else
        {
            D[axis]=Range(dest.getBase(axis),dest.getBound(axis));
        }
        if( S0[axis].getLength()>0 )
        {
            S[axis]=S0[axis];
        }
        else
        {
            S[axis]=Range(src.getBase(axis),src.getBound(axis));
        }
    }
    for( int axis=0; axis<numDim; axis++ )
    {
        if( S[axis].getBase() <src.getBase(axis) -src.getGhostBoundaryWidth(axis) || 
                S[axis].getBound()>src.getBound(axis)+src.getGhostBoundaryWidth(axis) )
        {
            printf(" CopyArray::copy:ERROR Source index values are out of bounds!\n"
           	     "   axis=%i S=[%i,%i] but src [base,bound]=[%i,%i]\n",
           	     axis,S[axis].getBase(),S[axis].getBound(),src.getBase(axis),src.getBound(axis));
            OV_ABORT("error");
        }
        if( D[axis].getBase() <dest.getBase(axis) -dest.getGhostBoundaryWidth(axis) || 
                D[axis].getBound()>dest.getBound(axis)+dest.getGhostBoundaryWidth(axis) )
        {
            printf(" CopyArray::copyArray:ERROR Destination index values are out of bounds!\n"
           	     "   axis=%i D=[%i,%i] but dest [base,bound]=[%i,%i]\n",
           	     axis,D[axis].getBase(),D[axis].getBound(),dest.getBase(axis),dest.getBound(axis));
            OV_ABORT("CopyArray::error");
        }
        if( (D[axis].getBound()-D[axis].getBase())/D[axis].getStride() != 
                (S[axis].getBound()-S[axis].getBase())/S[axis].getStride() )
        {
            printf(" ParallelUtility::copy:ERROR non-conformable operation!\n"
           	     "   axis=%i src=[%i,%i,%i] with count=%i, but dest=[%i,%i,%i] with count=%i\n",
           	     axis,
                          D[axis].getBase(),D[axis].getBound(),D[axis].getStride(),
           	     (D[axis].getBound()-D[axis].getBase())/D[axis].getStride(),
                          S[axis].getBase(),S[axis].getBound(),S[axis].getStride(),
           	     (S[axis].getBound()-S[axis].getBase())/S[axis].getStride());
            OV_ABORT("CopyArray::error");
        }
    }
    bool copyOnProcessor=true;  // if true do not send messages to the same processor
    if( debug !=0 && debugFile==NULL )
    {
        char fileName[40];
        sprintf(fileName,"copyArray%i.debug",myid);
        debugFile= fopen(fileName,"w");
    }
    if( debug !=0 )
    {
        fprintf(debugFile,"++++ copyArray *start* myid=%i +++++\n",myid);
        fflush(debugFile);
    }
  // Example with strides
  //        dest(2:14:3) = src(1:9:2)
  //
  //      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+       src  : S=[1,9,2]  (stride=2)
  //            1     3     5     7     9   
  // 
  //      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ dest : D=[2,14,3] (stride=3)
  //         2        5        8       11       14
  // 
  // src : source array
  //   srcSubArray : box for [ S[0],S[1],S[2],S[3] ] = points to copy
  //   srcLocalArrayBox : box for srcLocal
  //   localSrcSubArray : srcSubArray .intersect. srcLocalArrayBox = local pts to copy 
  // Step 1. Determine the information to send
  // Here is the global src sub-array that we want to send:
    IndexBox srcSubArray(S[0].getBase(),S[0].getBound(),
                   		       S[1].getBase(),S[1].getBound(),
                   		       S[2].getBase(),S[2].getBound(),
                   		       S[3].getBase(),S[3].getBound());
  // Here is the global dest sub-array that we want to assign
    IndexBox destSubArray(D[0].getBase(),D[0].getBound(),
                  			D[1].getBase(),D[1].getBound(),
                  			D[2].getBase(),D[2].getBound(),
                  			D[3].getBase(),D[3].getBound());
    doubleSerialArray srcLocal; getLocalArrayWithGhostBoundaries(src,srcLocal);
    doubleSerialArray destLocal; getLocalArrayWithGhostBoundaries(dest,destLocal);
  // Here are the boxes for srcLocal and destLocal arrays:
    IndexBox srcLocalArrayBox, destLocalArrayBox;
    getLocalArrayBox( myid, src, srcLocalArrayBox ); 
    getLocalArrayBoxWithGhost( myid, dest, destLocalArrayBox ); // include ghost points in dest
    if( debug!=0 )
    {
        fprintf(debugFile,"copyArray: S=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n"
                    	              "           D=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
          	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
          	    S[1].getBase(),S[1].getBound(),S[1].getStride(),
          	    S[2].getBase(),S[2].getBound(),S[2].getStride(),
          	    S[3].getBase(),S[3].getBound(),S[3].getStride(),
          	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
          	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
          	    D[2].getBase(),D[2].getBound(),D[2].getStride(),
          	    D[3].getBase(),D[3].getBound(),D[3].getStride());
        fprintf(debugFile,"copyArray: myid=%i srcLocal bounds=[%i,%i][%i,%i][%i,%i][%i,%i] (no ghost)\n",
          	    myid,
          	    srcLocalArrayBox.base(0),srcLocalArrayBox.bound(0),
          	    srcLocalArrayBox.base(1),srcLocalArrayBox.bound(1),
          	    srcLocalArrayBox.base(2),srcLocalArrayBox.bound(2),
          	    srcLocalArrayBox.base(3),srcLocalArrayBox.bound(3)); 
        fprintf(debugFile,"copyArray: myid=%i destLocal bounds=[%i,%i][%i,%i][%i,%i][%i,%i] (with ghost)\n",
          	    myid,
          	    destLocalArrayBox.base(0),destLocalArrayBox.bound(0),
          	    destLocalArrayBox.base(1),destLocalArrayBox.bound(1),
          	    destLocalArrayBox.base(2),destLocalArrayBox.bound(2),
          	    destLocalArrayBox.base(3),destLocalArrayBox.bound(3)); 
        fflush(debugFile);
    }
    const double *srcp = srcLocal.Array_Descriptor.Array_View_Pointer3;
    const int srcDim0=srcLocal.getRawDataSize(0);
    const int srcDim1=srcLocal.getRawDataSize(1);
    const int srcDim2=srcLocal.getRawDataSize(2);
#undef SRC
#define SRC(i0,i1,i2,i3) srcp[i0+srcDim0*(i1+srcDim1*(i2+srcDim2*(i3)))]
    double *destp = destLocal.Array_Descriptor.Array_View_Pointer3;
    const int destDim0=destLocal.getRawDataSize(0);
    const int destDim1=destLocal.getRawDataSize(1);
    const int destDim2=destLocal.getRawDataSize(2);
#undef DEST
#define DEST(i0,i1,i2,i3) destp[i0+destDim0*(i1+destDim1*(i2+destDim2*(i3)))]
// ------------- round up or round down to the nearest stride-point -------------------------
#define roundUp(i,base,stride)   ( (((i)-base +stride-1)/stride)*stride+base )
#define roundDown(i,base,stride) ( (((i)-base          )/stride)*stride+base )
#define roundBound(box,axis,S) roundUp(box.base(axis) ,S[axis].getBase(),S[axis].getStride()),roundDown(box.bound(axis),S[axis].getBase(),S[axis].getStride()) 
#define roundBox(box,S) box.setBounds(roundBound(box,0,S),roundBound(box,1,S),roundBound(box,2,S),roundBound(box,3,S))
// --------------------- Conversion between src and dest index spaces -------------------------------------------
// srcToDest: convert an index i in the src to the corresponding index j in the dest, dest(j)=scr(i)
#define srcToDest(i,axis) ((((i) -S[axis].getBase())/S[axis].getStride())*D[axis].getStride()+D[axis].getBase())
#define destToSrc(i,axis) ((((i) -D[axis].getBase())/D[axis].getStride())*S[axis].getStride()+S[axis].getBase())
#define boundsSrcToDest(box,axis) srcToDest(box.base(axis),axis),srcToDest(box.bound(axis),axis)
#define boundsDestToSrc(box,axis) destToSrc(box.base(axis),axis),destToSrc(box.bound(axis),axis)
#define boxDestFromSrc(boxd,boxs) boxd.setBounds(boundsSrcToDest(boxs,0),boundsSrcToDest(boxs,1),boundsSrcToDest(boxs,2),boundsSrcToDest(boxs,3))
#define boxSrcFromDest(boxs,boxd) boxs.setBounds(boundsDestToSrc(boxd,0),boundsDestToSrc(boxd,1),boundsDestToSrc(boxd,2),boundsDestToSrc(boxd,3))
// ---------------------------------------------------------------------------------------------------------------
  // Here is the local src sub-array that we need to send
    IndexBox localSrcSubArray;
    bool notEmpty = IndexBox::intersect(srcLocalArrayBox, srcSubArray, localSrcSubArray );
  // adjust box to match stride-points 
    roundBox(localSrcSubArray,S);
    notEmpty = !localSrcSubArray.isEmpty();
  // ***********************************************************
  // **** make a list of boxes of where to send the data *******
  // ***********************************************************
    ListOfIndexBox sendBoxes;
    if( notEmpty )
    {
    // We need to send info from srcLocal.
    // For each processor p, intersect localSrcSubArray with destArrayLocalBoxOnProcessorP
        for( int p=0; p<np; p++ )
        {
      // destArrayLocalBoxOnProcessorP : bounds on local dest array on processor p 
            IndexBox destArrayLocalBoxOnProcessorP;
            getLocalArrayBoxWithGhost( p, dest, destArrayLocalBoxOnProcessorP );
      // Convert localSrcSubArray from src index-space to the dest index-space
            IndexBox srcInDest;
            boxDestFromSrc(srcInDest,localSrcSubArray);
            IndexBox destSendBox;
            notEmpty = IndexBox::intersect( srcInDest, destArrayLocalBoxOnProcessorP, destSendBox ); 
      // round box to stride-pts: 
            roundBox(destSendBox,D);
            notEmpty = !destSendBox.isEmpty();
            if( debug!=0 )
            {
                fprintf(debugFile,
                                " check for send to p=%i: localSrcSubArray=[%i,%i][%i,%i] srcInDest=[%i,%i][%i,%i]"
                                " dest-local-to-p=[%i,%i][%i,%i] destSendBox=[%i,%i][%i,%i] --> notEmpty=%i\n",
                                p,
            		localSrcSubArray.base(0),localSrcSubArray.bound(0),
            		localSrcSubArray.base(1),localSrcSubArray.bound(1),
                                srcInDest.base(0),srcInDest.bound(0),
                                srcInDest.base(1),srcInDest.bound(1),
                                destArrayLocalBoxOnProcessorP.base(0),destArrayLocalBoxOnProcessorP.bound(0),
                                destArrayLocalBoxOnProcessorP.base(1),destArrayLocalBoxOnProcessorP.bound(1),
                                destSendBox.base(0),destSendBox.bound(0),
                                destSendBox.base(1),destSendBox.bound(1),notEmpty);
            }
            if( notEmpty )
            {
        // convert destSendBox from dest index-space to src index-space, srcSendBox
                IndexBox srcSendBox;
                boxSrcFromDest(srcSendBox,destSendBox);
      	if( copyOnProcessor && myid==p )
      	{
	  // Do not send a message to the same processor -- just copy the data
        	  if( debug!=0 )
        	  {
          	    fprintf(debugFile,"****copyArray: myid=%i Just copy data on the same processor: *****\n"
                		    "   src=[%i,%i][%i,%i][%i,%i][%i,%i] \n"
                                        "  dest=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
                		    myid,
                		    srcSendBox.base(0),srcSendBox.bound(0),
                		    srcSendBox.base(1),srcSendBox.bound(1),
                		    srcSendBox.base(2),srcSendBox.bound(2),
                		    srcSendBox.base(3),srcSendBox.bound(3),
                                        destSendBox.base(0),destSendBox.bound(0),
                		    destSendBox.base(1),destSendBox.bound(1),
                		    destSendBox.base(2),destSendBox.bound(2),
                		    destSendBox.base(3),destSendBox.bound(3));
        	  }
	  // assign points defined in srcSendBox
        	  FOR_BOX_BOX_WITH_STRIDE(i0,i1,i2,i3,destSendBox, j0,j1,j2,j3,srcSendBox )  
        	  { 
          	    DEST(i0,i1,i2,i3)=SRC(j0,j1,j2,j3); 
        	  }
      	}
      	else 
      	{
        	  srcSendBox.processor=p;
        	  sendBoxes.push_back(srcSendBox);  // We need to send this box of data to processor p
      	}
            }
        }
        ListOfIndexBox::iterator iter; 
        for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
        {
            IndexBox & pSendBox = *iter;
            if( debug!=0 )
            {
      	fprintf(debugFile,"****copyArray:*****\n"
            		">>> myid=%i: Send box =[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i] to processor p=%i\n",
            		myid,
            		pSendBox.base(0),pSendBox.bound(0),S[0].getStride(),
            		pSendBox.base(1),pSendBox.bound(1),S[1].getStride(),
            		pSendBox.base(2),pSendBox.bound(2),S[2].getStride(),
            		pSendBox.base(3),pSendBox.bound(3),S[3].getStride(),
                                pSendBox.processor);
            }
        }
    }
  // ********************************************************************
  // *********** Step 2. Determine the information to receive ***********
  // ********************************************************************
    if( debug!=0 )
    {
        fprintf(debugFile,"copyArray: myid=%i receive data for : destLocalArrayBox=[%i,%i][%i,%i][%i,%i][%i,%i], "
                        "isEmpty=%i\n",
          	    myid,
          	    destLocalArrayBox.base(0),destLocalArrayBox.bound(0),
          	    destLocalArrayBox.base(1),destLocalArrayBox.bound(1),
          	    destLocalArrayBox.base(2),destLocalArrayBox.bound(2),
          	    destLocalArrayBox.base(3),destLocalArrayBox.bound(3),(int)destLocalArrayBox.isEmpty());
        fflush(debugFile);
    }
    ListOfIndexBox receiveBoxes;
  // Here is the local dest sub-array that we need to receive
    IndexBox localDestSubArray;
    notEmpty = IndexBox::intersect(destLocalArrayBox, destSubArray, localDestSubArray );
  // round box to stride-pts: 
    roundBox(localDestSubArray,D);
    notEmpty = !localDestSubArray.isEmpty();
    if( notEmpty )
    {
    // make a list of boxes of where to receive the data from
        if( debug!=0 )
        {
            fprintf(debugFile," Receive points: localDestSubArray=[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
            	      localDestSubArray.base(0),localDestSubArray.bound(0),D[0].getStride(),
            	      localDestSubArray.base(1),localDestSubArray.bound(1),D[1].getStride(),
            	      localDestSubArray.base(2),localDestSubArray.bound(2),D[2].getStride(),
            	      localDestSubArray.base(3),localDestSubArray.bound(3),D[3].getStride());
        }
    // for each processor p, intersect localDestSubArray with srcLocalArrayBoxOnProcessorP
        for( int p=0; p<np; p++ )
        {
            if( copyOnProcessor && myid==p )
      	continue;  // the data has already been transfered between the processor and itself.
            IndexBox srcLocalArrayBoxOnProcessorP;  // defines the bounds of src on processor p
            CopyArray::getLocalArrayBox( p,src,srcLocalArrayBoxOnProcessorP );  // this is without ghost pts 
            if( debug!=0 )
            {
      	fprintf(debugFile,"copyArray: myid=%i srcLocalArrayBox=[%i,%i][%i,%i][%i,%i][%i,%i] on p=%i\n",
            		myid,
            		srcLocalArrayBoxOnProcessorP.base(0),srcLocalArrayBoxOnProcessorP.bound(0),
            		srcLocalArrayBoxOnProcessorP.base(1),srcLocalArrayBoxOnProcessorP.bound(1),
            		srcLocalArrayBoxOnProcessorP.base(2),srcLocalArrayBoxOnProcessorP.bound(2),
            		srcLocalArrayBoxOnProcessorP.base(3),srcLocalArrayBoxOnProcessorP.bound(3),p);
            }
      // convert localDestSubArray from dest index-space to src-index-space
            IndexBox destInSrc;
            boxSrcFromDest(destInSrc,localDestSubArray);
            IndexBox destReceiveBox;
            notEmpty = IndexBox::intersect(destInSrc,srcLocalArrayBoxOnProcessorP, destReceiveBox ); 
      // round box to stride-pts: 
            roundBox(destReceiveBox,S);
            notEmpty = !destReceiveBox.isEmpty();
            if( notEmpty )
            {
      	if( debug!=0 )
      	{
        	  fprintf(debugFile," myid=%i: Expecting to receive box =[%i,%i,%i][%i,%i,%i][%i,%i,%i][%i,%i,%i]"
                                    "from processor p=%i\n",
              		  myid,
              		  destReceiveBox.base(0),destReceiveBox.bound(0),S[0].getStride(),
              		  destReceiveBox.base(1),destReceiveBox.bound(1),S[1].getStride(),
              		  destReceiveBox.base(2),destReceiveBox.bound(2),S[2].getStride(),
              		  destReceiveBox.base(3),destReceiveBox.bound(3),S[3].getStride(),p);
      	}
        // convert destReceiveBox from src index-space to dest index-space
                boxDestFromSrc(destReceiveBox,destReceiveBox);
      	destReceiveBox.processor=p;
                receiveBoxes.push_back(destReceiveBox);
      	if( debug!=0 )
      	{
        	  fprintf(debugFile,"      --> in dest index-space box =[%i,%i][%i,%i][%i,%i][%i,%i] \n",
              		  destReceiveBox.base(0),destReceiveBox.bound(0),D[0].getStride(),
              		  destReceiveBox.base(1),destReceiveBox.bound(1),D[1].getStride(),
              		  destReceiveBox.base(2),destReceiveBox.bound(2),D[2].getStride(),
              		  destReceiveBox.base(3),destReceiveBox.bound(3),D[3].getStride());
      	}
            }
        }
    }
// Return the number points in a box accounting for the stride:
#define getStridedBoxSize(box,S)( ((box.bound(0)-box.base(0))/S[0].getStride()+1)* ((box.bound(1)-box.base(1))/S[1].getStride()+1)* ((box.bound(2)-box.base(2))/S[2].getStride()+1)* ((box.bound(3)-box.base(3))/S[3].getStride()+1) )
  // ************** MPI calls *****************
    if( debugFile!=NULL ) fflush(debugFile);
    const int numReceive=receiveBoxes.size();
    double **rBuff=NULL;   // buffers for receiving data
    MPI_Request *receiveRequest=NULL;
    MPI_Status *receiveStatus=NULL;
    int *receiveBoxIndex = new int [np];  // maps processor number to index in receiveBoxes
    for( int p=0; p<np; p++ ) receiveBoxIndex[p]=-1;
    if( numReceive>0 )
    {
    // ---- post receives first ----
        receiveRequest= new MPI_Request[numReceive]; // remember to delete these
        receiveStatus= new MPI_Status[numReceive]; 
        rBuff = new double* [numReceive];
    // int sendingProc = new int [numReceive];
        for(int m=0; m<numReceive; m++ )
        {
            IndexBox & pReceiveBox = receiveBoxes[m];
      // sendingProc[m]=pReceiveBox.processor;  // this processor will be sending the data
            int bufSize=getStridedBoxSize(pReceiveBox,D);  // we receive the strided data
            rBuff[m]= new double [bufSize];
            assert( pReceiveBox.processor>=0 && pReceiveBox.processor<np );
            receiveBoxIndex[pReceiveBox.processor]=m;  // maps processor number to index in receiveBoxes
            if( debug!=0 )
            {
      	fprintf(debugFile,">>> myid=%i: post a receive for buffer of size %i from p=%i (m=%i,numReceive=%i) \n",
                                myid,bufSize,pReceiveBox.processor,m,numReceive);
            }
            MPI_Irecv(rBuff[m],bufSize,MPI_DOUBLE,pReceiveBox.processor,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
        }
    }
  // ----- Now send the data -----
    const int numSend=sendBoxes.size();
    MPI_Request *sendRequest=NULL;
    double **sBuff=NULL;
    if( numSend>0 )
    {
        sendRequest= new MPI_Request[numSend]; // remember to delete these
        sBuff = new double* [numSend];
        for(int m=0; m<numSend; m++ )
        {
      // NOTE: we only send the strided data 
            IndexBox & pSendBox = sendBoxes[m]; 
            int bufSize=getStridedBoxSize(pSendBox,S);
            sBuff[m]= new double [bufSize];
            double *buff=sBuff[m];
            int i=0;
            FOR_BOX_WITH_STRIDE(i0,i1,i2,i3,S[0].getStride(),S[1].getStride(),S[2].getStride(),S[3].getStride(),pSendBox)
            {
      	buff[i]=SRC(i0,i1,i2,i3); 
      	i++;
            }
            if( debug!=0 )
            {
      	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
            }
            MPI_Isend(sBuff[m],bufSize,MPI_DOUBLE,pSendBox.processor,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
        }
    }
    if( numReceive>0 )
    {
        MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to receive all messages
        for(int m=0; m<numReceive; m++  )
        {
            int bufSize=receiveStatus[m].MPI_TAG;
            int p = receiveStatus[m].MPI_SOURCE;
            assert( p>=0 && p<np );
            if( debug!=0 )
            {
      	fprintf(debugFile,">>> myid=%i: received buffer of size %i from p=%i (m=%i,numReceive=%i)\n myid=%i: buff=",
                        myid,bufSize,p,m,numReceive,myid);
      	for( int j=0; j<bufSize; j++ ) fprintf(debugFile,"%3.1f ",rBuff[m][j]);
      	fprintf(debugFile,"\n");
            }
      // fill in the entries of vLocal
            int n = receiveBoxIndex[p];
            assert( n>=0 && n<numReceive );
      // Question: is n==m always???
            IndexBox & rBox = receiveBoxes[n];
            assert( rBox.processor==p );
      // assign dest(rBox) = rBuff[m][0...]
            const double *buff = rBuff[m];
            int i=0;
            FOR_BOX_WITH_STRIDE(i0,i1,i2,i3,D[0].getStride(),D[1].getStride(),D[2].getStride(),D[3].getStride(),rBox)
            {
      	DEST(i0,i1,i2,i3)=buff[i];   
      	i++;
            }
        }
    }
//   if( debug & 2 )
//   {
//     ListOfIndexBox::iterator iter; 
//     for(int m=0; iter = receiveBoxes.begin(); m++, iter!=receiveBoxes.end(); iter++ )
//     {
//       IndexBox & pReceiveBox = *iter;
//       int bufSize=receiveStatus[m].MPI_TAG;
//       fprintf(debugFile,"<- processor %i: received msg from processor %i, tag=%i p=%i values=",myID,
// 	      receiveStatus[m].MPI_SOURCE,receiveStatus[m].MPI_TAG,p);
//       for( j=0; j<nivd; j++ ) fprintf(debugFile,"%8.2e ",dbuff[p][j]);
//       fprintf(debugFile,"\n");
//       }
//     }
//   }
  // wait to send messages before deleting buffers
    if( numSend>0 )
    {
        if( debug!=0 )
        {
            fprintf(debugFile,"+++ myid=%i: wait for all messges to be sent, numSend=%i\n",myid,numSend); 
            fflush(debugFile);
        }
        MPI_Status *sendStatus = new MPI_Status[numSend]; 
        MPI_Waitall( numSend, sendRequest, sendStatus );   
        delete [] sendStatus;
    }
    if( debug!=0 )
    {
        fprintf(debugFile,"+++ myid=%i: cleanup buffers...\n",myid); 
        fflush(debugFile);
    }
    for (int i=0; i<numReceive; i++ )
    {
        delete [] rBuff[i];
    }
    delete [] rBuff;
    delete [] receiveStatus;
    delete [] receiveRequest;
    delete [] receiveBoxIndex;
    for (int i=0; i<numSend; i++ )
    {
        delete [] sBuff[i];
    }
    delete [] sBuff;
    delete [] sendRequest;
    if( debugFile!=NULL )
    {
        fprintf(debugFile,"**** myid=%i finished in copyArray ****\n\n",myid);
        fflush(debugFile);
    }
    return 0;
#else
  // **** Here is the serial version  ****
    Index *D=D0, *S=S0;
    switch (nd)
    {
    case 1:
        dest(D[0]) = src(S[0]);
        break;
    case 2:
        dest(D[0],D[1]) = src(S[0],S[1]);
        break;
    case 3:
        dest(D[0],D[1],D[2]) = src(S[0],S[1],S[2]);
        break;
    case 4:
        dest(D[0],D[1],D[2],D[3]) = src(S[0],S[1],S[2],S[3]);
        break;
    case 5:
        dest(D[0],D[1],D[2],D[3],D[4]) = src(S[0],S[1],S[2],S[3],S[4]);
        break;
    case 6:
        dest(D[0],D[1],D[2],D[3],D[4],D[5]) = src(S[0],S[1],S[2],S[3],S[4],S[5]);
        break;
//    case 7:
//      dest(D[0],D[1],D[2],D[3],D[4],D[5],D[6]) = src(S[0],S[1],S[2],S[3],S[4],S[5],S[6]);
//      break;
//    case 8:
//      dest(D[0],D[1],D[2],D[3],D[4],D[5],D[6],D[7]) = src(S[0],S[1],S[2],S[3],S[4],S[5],S[6],S[7]);
//      break;
    default:
        Overture::abort("ERROR: nd to large");
        break;
    }
#endif
    return 0;
}



// ****************************************************************
// *************** defineSerialCopyMacro **************************
// ****************************************************************

// defineSerialCopyMacro(int,intSerialArray,MPI_INT);
int CopyArray::
copyArray( intSerialArray & dest, int destProcessor, intSerialArray & src, int srcProcessor )
// ===================================================================================
//   Copy the serial array "src" from processor "srcProcessor" to 
//        the serial array "dest" on processor destProcessor
// 
// /dest (input/output) : destination array
// /destProcessor (input) : destination processor
// /src (input) : source array
// /srcProcessor (input) : source processor
// /author WDH 090808
// ===================================================================================
{
#ifdef USE_PPP
    if( destProcessor==srcProcessor )
    {
    // special case -- no need for communication if src and dest are on the same processors
        if( &dest != &src ) // check that src and dest are not already the same array
        {
            dest.redim(0);
            dest=src;
        }
    }
    else
    {
    // general case
    // Send the dimensions of the src array from srcProcessor to destProcessor
        const int myid = Communication_Manager::My_Process_Number;
        const int debug=0; // set to 1 for debug output
    // The dimensions of the array are sent/saved in the dims(side,axis) array
        const int MAX_DIMS=2*MAX_ARRAY_DIMENSION;
        int pdims[MAX_DIMS];
        #define dims(s,a) pdims[(s)+2*(a)]
    // ---- post receive for the dimensions of the array to be passed ----
        MPI_Request receiveRequest;
        int sendTag= 123987;
        if( myid==destProcessor )
            MPI_Irecv( pdims,MAX_DIMS,MPI_INT,srcProcessor,sendTag,MPI_COMM_WORLD,&receiveRequest );
    // Fill in the dims array 
        int count=0;  // holds total number of array elements to send/receive
        if( myid==srcProcessor )
        {
            count=1;
            for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            {
                dims(0,a)=src.getBase(a);
                dims(1,a)=src.getBound(a);
      	count *= dims(1,a)-dims(0,a)+1;
            }
            assert( count>=0 );
            if( debug>0 ) printf("copySerial:Array dims to send, myid=%i : srcProcessor: count=%i\n",myid,count);
        }
        MPI_Request sendRequest;
        if( myid==srcProcessor )
        {
            MPI_Isend(pdims,MAX_DIMS,MPI_INT,destProcessor,sendTag,MPI_COMM_WORLD,&sendRequest );
        }
        const int numReceive=1;  // wait for one message
        MPI_Status receiveStatus;
        if( myid==destProcessor )
        {
      // --- receive the array dimensions and then allocate the dest array ---
            MPI_Waitall( numReceive, &receiveRequest, &receiveStatus );  // wait to receive all messages
            assert( receiveStatus.MPI_TAG == sendTag );
            assert( receiveStatus.MPI_SOURCE == srcProcessor );
            Range D[MAX_ARRAY_DIMENSION];
            count=1;
            for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            {
      	D[a]=Range(dims(0,a),dims(1,a));
      	count *= dims(1,a)-dims(0,a)+1;
            }
            assert( count>=0 );
            assert( MAX_ARRAY_DIMENSION==6 );
            if( count>0 )
                dest.redim(D[0],D[1],D[2],D[3],D[4],D[5]);
            else
                dest.redim(0);
            if( debug>0 ) printf("copySerial: Allocate dest array: myid=%i : destProcessor: count=%i\n",myid,count);
        }
        if( count==0 )
        { // there is no data to send
            return 0;
        }
    // ---- post receive first for the array elements ----
        sendTag= 456790;
        if( myid==destProcessor )
            MPI_Irecv( dest.getDataPointer(),count,MPI_INT,srcProcessor,sendTag,MPI_COMM_WORLD,&receiveRequest );
    // --- send the array data ---
        if( myid==srcProcessor )
        {
            MPI_Isend(src.getDataPointer(),count,MPI_INT,destProcessor,sendTag,MPI_COMM_WORLD,&sendRequest );
        }
        if( myid==destProcessor )
        {
      // wait to receive the array data
            MPI_Waitall( numReceive, &receiveRequest, &receiveStatus );
            assert( receiveStatus.MPI_TAG == sendTag );
            assert( receiveStatus.MPI_SOURCE == srcProcessor );
        }
    }
#else
  // serial -- only copy if these are not the same array already
    if( &dest != &src )
    {
        dest.redim(0);
        dest=src;
    }
#endif
    return 0;
}

// defineSerialCopyMacro(float,floatSerialArray,MPI_FLOAT);
int CopyArray::
copyArray( floatSerialArray & dest, int destProcessor, floatSerialArray & src, int srcProcessor )
// ===================================================================================
//   Copy the serial array "src" from processor "srcProcessor" to 
//        the serial array "dest" on processor destProcessor
// 
// /dest (input/output) : destination array
// /destProcessor (input) : destination processor
// /src (input) : source array
// /srcProcessor (input) : source processor
// /author WDH 090808
// ===================================================================================
{
#ifdef USE_PPP
    if( destProcessor==srcProcessor )
    {
    // special case -- no need for communication if src and dest are on the same processors
        if( &dest != &src ) // check that src and dest are not already the same array
        {
            dest.redim(0);
            dest=src;
        }
    }
    else
    {
    // general case
    // Send the dimensions of the src array from srcProcessor to destProcessor
        const int myid = Communication_Manager::My_Process_Number;
        const int debug=0; // set to 1 for debug output
    // The dimensions of the array are sent/saved in the dims(side,axis) array
        const int MAX_DIMS=2*MAX_ARRAY_DIMENSION;
        int pdims[MAX_DIMS];
        #define dims(s,a) pdims[(s)+2*(a)]
    // ---- post receive for the dimensions of the array to be passed ----
        MPI_Request receiveRequest;
        int sendTag= 123987;
        if( myid==destProcessor )
            MPI_Irecv( pdims,MAX_DIMS,MPI_INT,srcProcessor,sendTag,MPI_COMM_WORLD,&receiveRequest );
    // Fill in the dims array 
        int count=0;  // holds total number of array elements to send/receive
        if( myid==srcProcessor )
        {
            count=1;
            for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            {
                dims(0,a)=src.getBase(a);
                dims(1,a)=src.getBound(a);
      	count *= dims(1,a)-dims(0,a)+1;
            }
            assert( count>=0 );
            if( debug>0 ) printf("copySerial:Array dims to send, myid=%i : srcProcessor: count=%i\n",myid,count);
        }
        MPI_Request sendRequest;
        if( myid==srcProcessor )
        {
            MPI_Isend(pdims,MAX_DIMS,MPI_INT,destProcessor,sendTag,MPI_COMM_WORLD,&sendRequest );
        }
        const int numReceive=1;  // wait for one message
        MPI_Status receiveStatus;
        if( myid==destProcessor )
        {
      // --- receive the array dimensions and then allocate the dest array ---
            MPI_Waitall( numReceive, &receiveRequest, &receiveStatus );  // wait to receive all messages
            assert( receiveStatus.MPI_TAG == sendTag );
            assert( receiveStatus.MPI_SOURCE == srcProcessor );
            Range D[MAX_ARRAY_DIMENSION];
            count=1;
            for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            {
      	D[a]=Range(dims(0,a),dims(1,a));
      	count *= dims(1,a)-dims(0,a)+1;
            }
            assert( count>=0 );
            assert( MAX_ARRAY_DIMENSION==6 );
            if( count>0 )
                dest.redim(D[0],D[1],D[2],D[3],D[4],D[5]);
            else
                dest.redim(0);
            if( debug>0 ) printf("copySerial: Allocate dest array: myid=%i : destProcessor: count=%i\n",myid,count);
        }
        if( count==0 )
        { // there is no data to send
            return 0;
        }
    // ---- post receive first for the array elements ----
        sendTag= 456790;
        if( myid==destProcessor )
            MPI_Irecv( dest.getDataPointer(),count,MPI_FLOAT,srcProcessor,sendTag,MPI_COMM_WORLD,&receiveRequest );
    // --- send the array data ---
        if( myid==srcProcessor )
        {
            MPI_Isend(src.getDataPointer(),count,MPI_FLOAT,destProcessor,sendTag,MPI_COMM_WORLD,&sendRequest );
        }
        if( myid==destProcessor )
        {
      // wait to receive the array data
            MPI_Waitall( numReceive, &receiveRequest, &receiveStatus );
            assert( receiveStatus.MPI_TAG == sendTag );
            assert( receiveStatus.MPI_SOURCE == srcProcessor );
        }
    }
#else
  // serial -- only copy if these are not the same array already
    if( &dest != &src )
    {
        dest.redim(0);
        dest=src;
    }
#endif
    return 0;
}

// defineSerialCopyMacro(double,doubleSerialArray,MPI_DOUBLE);
int CopyArray::
copyArray( doubleSerialArray & dest, int destProcessor, doubleSerialArray & src, int srcProcessor )
// ===================================================================================
//   Copy the serial array "src" from processor "srcProcessor" to 
//        the serial array "dest" on processor destProcessor
// 
// /dest (input/output) : destination array
// /destProcessor (input) : destination processor
// /src (input) : source array
// /srcProcessor (input) : source processor
// /author WDH 090808
// ===================================================================================
{
#ifdef USE_PPP
    if( destProcessor==srcProcessor )
    {
    // special case -- no need for communication if src and dest are on the same processors
        if( &dest != &src ) // check that src and dest are not already the same array
        {
            dest.redim(0);
            dest=src;
        }
    }
    else
    {
    // general case
    // Send the dimensions of the src array from srcProcessor to destProcessor
        const int myid = Communication_Manager::My_Process_Number;
        const int debug=0; // set to 1 for debug output
    // The dimensions of the array are sent/saved in the dims(side,axis) array
        const int MAX_DIMS=2*MAX_ARRAY_DIMENSION;
        int pdims[MAX_DIMS];
        #define dims(s,a) pdims[(s)+2*(a)]
    // ---- post receive for the dimensions of the array to be passed ----
        MPI_Request receiveRequest;
        int sendTag= 123987;
        if( myid==destProcessor )
            MPI_Irecv( pdims,MAX_DIMS,MPI_INT,srcProcessor,sendTag,MPI_COMM_WORLD,&receiveRequest );
    // Fill in the dims array 
        int count=0;  // holds total number of array elements to send/receive
        if( myid==srcProcessor )
        {
            count=1;
            for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            {
                dims(0,a)=src.getBase(a);
                dims(1,a)=src.getBound(a);
      	count *= dims(1,a)-dims(0,a)+1;
            }
            assert( count>=0 );
            if( debug>0 ) printf("copySerial:Array dims to send, myid=%i : srcProcessor: count=%i\n",myid,count);
        }
        MPI_Request sendRequest;
        if( myid==srcProcessor )
        {
            MPI_Isend(pdims,MAX_DIMS,MPI_INT,destProcessor,sendTag,MPI_COMM_WORLD,&sendRequest );
        }
        const int numReceive=1;  // wait for one message
        MPI_Status receiveStatus;
        if( myid==destProcessor )
        {
      // --- receive the array dimensions and then allocate the dest array ---
            MPI_Waitall( numReceive, &receiveRequest, &receiveStatus );  // wait to receive all messages
            assert( receiveStatus.MPI_TAG == sendTag );
            assert( receiveStatus.MPI_SOURCE == srcProcessor );
            Range D[MAX_ARRAY_DIMENSION];
            count=1;
            for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            {
      	D[a]=Range(dims(0,a),dims(1,a));
      	count *= dims(1,a)-dims(0,a)+1;
            }
            assert( count>=0 );
            assert( MAX_ARRAY_DIMENSION==6 );
            if( count>0 )
                dest.redim(D[0],D[1],D[2],D[3],D[4],D[5]);
            else
                dest.redim(0);
            if( debug>0 ) printf("copySerial: Allocate dest array: myid=%i : destProcessor: count=%i\n",myid,count);
        }
        if( count==0 )
        { // there is no data to send
            return 0;
        }
    // ---- post receive first for the array elements ----
        sendTag= 456790;
        if( myid==destProcessor )
            MPI_Irecv( dest.getDataPointer(),count,MPI_DOUBLE,srcProcessor,sendTag,MPI_COMM_WORLD,&receiveRequest );
    // --- send the array data ---
        if( myid==srcProcessor )
        {
            MPI_Isend(src.getDataPointer(),count,MPI_DOUBLE,destProcessor,sendTag,MPI_COMM_WORLD,&sendRequest );
        }
        if( myid==destProcessor )
        {
      // wait to receive the array data
            MPI_Waitall( numReceive, &receiveRequest, &receiveStatus );
            assert( receiveStatus.MPI_TAG == sendTag );
            assert( receiveStatus.MPI_SOURCE == srcProcessor );
        }
    }
#else
  // serial -- only copy if these are not the same array already
    if( &dest != &src )
    {
        dest.redim(0);
        dest=src;
    }
#endif
    return 0;
}




// ************************************************************************************
//  Macro to define the serial to distributed copy 
// ************************************************************************************

// define the instances of the macro: 

// serialToDistributedCopyMacro(int,intDistributedArray,intSerialArray,MPI_INT);
int CopyArray::
copyArray( const intSerialArray & uLocal,
                      const Index *Jva, 
                      const intSerialArray & uProcessorSet,
                      intDistributedArray & v,
                      const Index *Iva )
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
// /uLocal (input) : source array (NOTE: uLocal should be an empty array and Jva null indexes if
//                    there is no data to sent from a processor).
// /Jva[d] (input) : d=0,1,...,nd-1 where nd=v.numberOfDimensions(), defines the local 
//                    rectangular region to copy from this processor.
//                  The actual data sent will be the intersection of these bounds with Iv.
// /uProcessorSet (input) : a list of source processors (i.e. the processors where uLocal has points)
// /Iva[d] (input) : d=0,1,...,nd-1 where nd=v.numberOfDimensions(), defines the global rectangular 
//                    region to copy. d=array dimension, d=0,1,..,5
// /v (input/output) : destination array. On input this array must  be dimensioned to the correct size. 
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
    Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
    Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];
  // Do this for now: (we don't accept a null-Index on input)
    for( int d=0; d<numDim; d++ )
    {
        Iv[d]=Iva[d];
        Jv[d]=Jva[d];
        assert( Iv[d].getLength()>0 );
        if( uLocalIsEmpty && Jv[d].getLength()>0 )
        {
            printf("copyArray:ERROR: copy serial to distributed: uLocal is empty but Jv[%i]=[%i,%i] is not empty!\n",
           	     d,Jv[d].getBase(),Jv[d].getBound());
            OV_ABORT("error");
        }
        if(  !uLocalIsEmpty && Jv[d].getLength()<=0  )
        {
            printf("copyArray:ERROR: copy serial to distributed: uLocal is NOT empty but Jv[%i].getLength()=%i is empty!\n",
           	     d,Jv[d].getLength());
            OV_ABORT("error");
        }
    }
  // Set the Index's for dimensions larger than the array number of dimensions: 
    for( int d=numDim; d<maxDim; d++ )
    {
        Iv[d]=v.dimension(d);
        if( !uLocalIsEmpty )
            Jv[d]=uLocal.dimension(d);
    }
    bool copyOnProcessor=true;  // if true do not send messages to the same processor
    int debug=0; // 3; // 0; // **************
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
    if( debug !=0 )
    {
        if( !uLocalIsEmpty )
        {
      // double check that myid is in uProcessorSet
            if( min(abs(uProcessorSet-myid))!=0 )
            {
      	printf("CopyArray: ERROR: uLocal is NOT empty, but myid=%i is NOT in uProcessorSet.\n",
             	       myid);
      	OV_ABORT("ERROR");
            }
        }
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
  // post receives for the header info (size, base, bound of data to be received)
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
      	fprintf(debugFile,"send header: p=%i, pp=%i Send header to same proc.\n",p,pp);
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
    int **rbuff = new int* [nprh];
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
            rbuff[npr] = new int [ num ];
            int tag=tag2+myid;
            MPI_Irecv(rbuff[npr], num, MPI_INT ,pp,tag,MY_COMM,&receiveRequest[npr] );
            npr++;
        }
    }
    const int *uLocalp = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uLocalDim0=uLocal.getRawDataSize(0);
    const int uLocalDim1=uLocal.getRawDataSize(1);
    const int uLocalDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) uLocalp[i0+uLocalDim0*(i1+uLocalDim1*(i2+uLocalDim2*(i3)))]
    int i0,i1,i2,i3;
  // allocate, fill and send buffers 
    int **sbuff = new int* [nps];
    int p=0;
    for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
    {
        IndexBox & pSendBox = *iter;
        const int pp = pSendBox.processor;
        if( pp!=myid && !pSendBox.isEmpty()  )
        {
      // -- send this non-empty box of data --
            const int buffSize = pSendBox.size();
            sbuff[p] = new int [ max(1,buffSize) ];
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
            MPI_Isend(sbuff[p],buffSize,MPI_INT,pp,sendTag,MY_COMM,&sendRequestd[p] );
            p++;
        }
    }
    assert( p==nps );
    intSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
    int *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
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
    MPI_Status *receiveStatusd  = new MPI_Status [npr];
    MPI_Waitall( npr, receiveRequest, receiveStatusd );    // wait to receive all messages
    int mpr=0;
  // *wdh* go back to original for( int p=0; p<npr; p++ )
    for( int p=0; p<nprh; p++ )
    {
        int num = rhbuff[p][0];
        if( num>0 )
        {
            const int pp=receiveStatusd[mpr].MPI_SOURCE;
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
    delete [] receiveStatusd;
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
    v(Iva[0],Iva[1],Iva[2],Iva[3])=uLocal(Jva[0],Jva[1],Jva[2],Jva[3]);
    return 0;
#endif
}

// serialToDistributedCopyMacro(float,floatDistributedArray,floatSerialArray,MPI_FLOAT);
int CopyArray::
copyArray( const floatSerialArray & uLocal,
                      const Index *Jva, 
                      const intSerialArray & uProcessorSet,
                      floatDistributedArray & v,
                      const Index *Iva )
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
// /uLocal (input) : source array (NOTE: uLocal should be an empty array and Jva null indexes if
//                    there is no data to sent from a processor).
// /Jva[d] (input) : d=0,1,...,nd-1 where nd=v.numberOfDimensions(), defines the local 
//                    rectangular region to copy from this processor.
//                  The actual data sent will be the intersection of these bounds with Iv.
// /uProcessorSet (input) : a list of source processors (i.e. the processors where uLocal has points)
// /Iva[d] (input) : d=0,1,...,nd-1 where nd=v.numberOfDimensions(), defines the global rectangular 
//                    region to copy. d=array dimension, d=0,1,..,5
// /v (input/output) : destination array. On input this array must  be dimensioned to the correct size. 
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
    Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
    Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];
  // Do this for now: (we don't accept a null-Index on input)
    for( int d=0; d<numDim; d++ )
    {
        Iv[d]=Iva[d];
        Jv[d]=Jva[d];
        assert( Iv[d].getLength()>0 );
        if( uLocalIsEmpty && Jv[d].getLength()>0 )
        {
            printf("copyArray:ERROR: copy serial to distributed: uLocal is empty but Jv[%i]=[%i,%i] is not empty!\n",
           	     d,Jv[d].getBase(),Jv[d].getBound());
            OV_ABORT("error");
        }
        if(  !uLocalIsEmpty && Jv[d].getLength()<=0  )
        {
            printf("copyArray:ERROR: copy serial to distributed: uLocal is NOT empty but Jv[%i].getLength()=%i is empty!\n",
           	     d,Jv[d].getLength());
            OV_ABORT("error");
        }
    }
  // Set the Index's for dimensions larger than the array number of dimensions: 
    for( int d=numDim; d<maxDim; d++ )
    {
        Iv[d]=v.dimension(d);
        if( !uLocalIsEmpty )
            Jv[d]=uLocal.dimension(d);
    }
    bool copyOnProcessor=true;  // if true do not send messages to the same processor
    int debug=0; // 3; // 0; // **************
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
    if( debug !=0 )
    {
        if( !uLocalIsEmpty )
        {
      // double check that myid is in uProcessorSet
            if( min(abs(uProcessorSet-myid))!=0 )
            {
      	printf("CopyArray: ERROR: uLocal is NOT empty, but myid=%i is NOT in uProcessorSet.\n",
             	       myid);
      	OV_ABORT("ERROR");
            }
        }
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
  // post receives for the header info (size, base, bound of data to be received)
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
      	fprintf(debugFile,"send header: p=%i, pp=%i Send header to same proc.\n",p,pp);
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
    float **rbuff = new float* [nprh];
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
            rbuff[npr] = new float [ num ];
            int tag=tag2+myid;
            MPI_Irecv(rbuff[npr], num, MPI_FLOAT ,pp,tag,MY_COMM,&receiveRequest[npr] );
            npr++;
        }
    }
    const float *uLocalp = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uLocalDim0=uLocal.getRawDataSize(0);
    const int uLocalDim1=uLocal.getRawDataSize(1);
    const int uLocalDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) uLocalp[i0+uLocalDim0*(i1+uLocalDim1*(i2+uLocalDim2*(i3)))]
    int i0,i1,i2,i3;
  // allocate, fill and send buffers 
    float **sbuff = new float* [nps];
    int p=0;
    for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
    {
        IndexBox & pSendBox = *iter;
        const int pp = pSendBox.processor;
        if( pp!=myid && !pSendBox.isEmpty()  )
        {
      // -- send this non-empty box of data --
            const int buffSize = pSendBox.size();
            sbuff[p] = new float [ max(1,buffSize) ];
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
            MPI_Isend(sbuff[p],buffSize,MPI_FLOAT,pp,sendTag,MY_COMM,&sendRequestd[p] );
            p++;
        }
    }
    assert( p==nps );
    floatSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
    float *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
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
    MPI_Status *receiveStatusd  = new MPI_Status [npr];
    MPI_Waitall( npr, receiveRequest, receiveStatusd );    // wait to receive all messages
    int mpr=0;
  // *wdh* go back to original for( int p=0; p<npr; p++ )
    for( int p=0; p<nprh; p++ )
    {
        int num = rhbuff[p][0];
        if( num>0 )
        {
            const int pp=receiveStatusd[mpr].MPI_SOURCE;
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
    delete [] receiveStatusd;
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
    v(Iva[0],Iva[1],Iva[2],Iva[3])=uLocal(Jva[0],Jva[1],Jva[2],Jva[3]);
    return 0;
#endif
}

// serialToDistributedCopyMacro(double,doubleDistributedArray,doubleSerialArray,MPI_DOUBLE);
int CopyArray::
copyArray( const doubleSerialArray & uLocal,
                      const Index *Jva, 
                      const intSerialArray & uProcessorSet,
                      doubleDistributedArray & v,
                      const Index *Iva )
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
// /uLocal (input) : source array (NOTE: uLocal should be an empty array and Jva null indexes if
//                    there is no data to sent from a processor).
// /Jva[d] (input) : d=0,1,...,nd-1 where nd=v.numberOfDimensions(), defines the local 
//                    rectangular region to copy from this processor.
//                  The actual data sent will be the intersection of these bounds with Iv.
// /uProcessorSet (input) : a list of source processors (i.e. the processors where uLocal has points)
// /Iva[d] (input) : d=0,1,...,nd-1 where nd=v.numberOfDimensions(), defines the global rectangular 
//                    region to copy. d=array dimension, d=0,1,..,5
// /v (input/output) : destination array. On input this array must  be dimensioned to the correct size. 
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
    Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
    Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];
  // Do this for now: (we don't accept a null-Index on input)
    for( int d=0; d<numDim; d++ )
    {
        Iv[d]=Iva[d];
        Jv[d]=Jva[d];
        assert( Iv[d].getLength()>0 );
        if( uLocalIsEmpty && Jv[d].getLength()>0 )
        {
            printf("copyArray:ERROR: copy serial to distributed: uLocal is empty but Jv[%i]=[%i,%i] is not empty!\n",
           	     d,Jv[d].getBase(),Jv[d].getBound());
            OV_ABORT("error");
        }
        if(  !uLocalIsEmpty && Jv[d].getLength()<=0  )
        {
            printf("copyArray:ERROR: copy serial to distributed: uLocal is NOT empty but Jv[%i].getLength()=%i is empty!\n",
           	     d,Jv[d].getLength());
            OV_ABORT("error");
        }
    }
  // Set the Index's for dimensions larger than the array number of dimensions: 
    for( int d=numDim; d<maxDim; d++ )
    {
        Iv[d]=v.dimension(d);
        if( !uLocalIsEmpty )
            Jv[d]=uLocal.dimension(d);
    }
    bool copyOnProcessor=true;  // if true do not send messages to the same processor
    int debug=0; // 3; // 0; // **************
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
    if( debug !=0 )
    {
        if( !uLocalIsEmpty )
        {
      // double check that myid is in uProcessorSet
            if( min(abs(uProcessorSet-myid))!=0 )
            {
      	printf("CopyArray: ERROR: uLocal is NOT empty, but myid=%i is NOT in uProcessorSet.\n",
             	       myid);
      	OV_ABORT("ERROR");
            }
        }
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
  // post receives for the header info (size, base, bound of data to be received)
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
      	fprintf(debugFile,"send header: p=%i, pp=%i Send header to same proc.\n",p,pp);
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
    double **rbuff = new double* [nprh];
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
            rbuff[npr] = new double [ num ];
            int tag=tag2+myid;
            MPI_Irecv(rbuff[npr], num, MPI_DOUBLE ,pp,tag,MY_COMM,&receiveRequest[npr] );
            npr++;
        }
    }
    const double *uLocalp = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uLocalDim0=uLocal.getRawDataSize(0);
    const int uLocalDim1=uLocal.getRawDataSize(1);
    const int uLocalDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) uLocalp[i0+uLocalDim0*(i1+uLocalDim1*(i2+uLocalDim2*(i3)))]
    int i0,i1,i2,i3;
  // allocate, fill and send buffers 
    double **sbuff = new double* [nps];
    int p=0;
    for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
    {
        IndexBox & pSendBox = *iter;
        const int pp = pSendBox.processor;
        if( pp!=myid && !pSendBox.isEmpty()  )
        {
      // -- send this non-empty box of data --
            const int buffSize = pSendBox.size();
            sbuff[p] = new double [ max(1,buffSize) ];
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
            MPI_Isend(sbuff[p],buffSize,MPI_DOUBLE,pp,sendTag,MY_COMM,&sendRequestd[p] );
            p++;
        }
    }
    assert( p==nps );
    doubleSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
    double *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
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
    MPI_Status *receiveStatusd  = new MPI_Status [npr];
    MPI_Waitall( npr, receiveRequest, receiveStatusd );    // wait to receive all messages
    int mpr=0;
  // *wdh* go back to original for( int p=0; p<npr; p++ )
    for( int p=0; p<nprh; p++ )
    {
        int num = rhbuff[p][0];
        if( num>0 )
        {
            const int pp=receiveStatusd[mpr].MPI_SOURCE;
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
    delete [] receiveStatusd;
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
    v(Iva[0],Iva[1],Iva[2],Iva[3])=uLocal(Jva[0],Jva[1],Jva[2],Jva[3]);
    return 0;
#endif
}
