#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "InterpolationData.h"


  
InterpolationData::
InterpolationData()
{
  numberOfInterpolationPoints=0;
}

InterpolationData::
~InterpolationData()
{
}



// =======================================================================================================
/// \brief Get a list of interp pts that are on this processor 
/// \Notes:
///    This routine will fill in in the interpData class with the interpolation 
///  data for all interpolation points that live on the current processor (i.e. all
/// points where cg.interpolationPoint(i,.) is located on the part of the grid function (e.g. the mask)
/// that exists on this processor.
/// \Author wdh, 091202
// =======================================================================================================
int ParallelGridUtility::
getLocalInterpolationData( CompositeGrid & cg, InterpolationData *&interpData )
{

  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  if( numberOfComponentGrids<=1 ) return 0;
  
#ifndef USE_PPP

  OV_ABORT("ERROR: getLocalInterpolationData called in serial");
  return 0;

#else

  const bool sendZeroLengthMessages=false;  // if false we optimize away zero length messages

  if( interpData==NULL )
    interpData = new InterpolationData [numberOfComponentGrids];

  const int numberOfDimensions=cg.numberOfDimensions();
  const int numberOfBaseGrids = cg.numberOfBaseGrids();

  if( false &&   // --- the local interp data on cg may not be the arrays we want if they were created by Ogmg ---
      cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAll )
  {
    // The interpolation data is already local -- just reference existing arrays
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
      InterpolationData & ipd = interpData[grid];

      ipd.interpolationPoint.reference( cg->interpolationPointLocal[grid]);
      ipd.interpoleeLocation.reference( cg->interpoleeLocationLocal[grid]);
      ipd.interpoleeGrid.reference( cg->interpoleeGridLocal[grid]);
      ipd.variableInterpolationWidth.reference( cg->variableInterpolationWidthLocal[grid]);
      ipd.interpolationCoordinates.reference(cg->interpolationCoordinatesLocal[grid]);
    }

    return 0;
  }
  


  MPI_Comm MY_COMM = Overture::OV_COMM;  // use this communicator by default; 

  const int myid = Communication_Manager::My_Process_Number;
  const int np=Communication_Manager::Number_Of_Processors;

  const int debug=0; // 3; // *******
  #define GLID_DEBUG
  // #undef GLID_DEBUG

  FILE *debugFile=NULL;
  
  if( debug >0 && debugFile==NULL )
  {
    aString fileName=sPrintF("glidNP%ip%i.debug",np,myid);
    debugFile=fopen((const char*)fileName,"w"); // open a different file on each proc.
    printF("getLocalInterpolationData:: output written to debug file %s\n",(const char*)fileName);
  }
  
  if( debug >0 )
  {  
    fprintf(debugFile,"**** getLocalInterpolationData: numberOfComponentGrids=%i, debug=%i\n",
            numberOfComponentGrids,debug);

  }


  int *numToSendp = new int [np*numberOfComponentGrids];
  int *numToReceivep= new int [np*numberOfComponentGrids];
  #define numToSend(g,p) numToSendp[(g)+numberOfComponentGrids*(p)]
  #define numToReceive(g,p) numToReceivep[(g)+numberOfComponentGrids*(p)]


  // ----------------------------------------------------------------------
  // --- count how many interp. pts. on each grid are on each processor ---
  // ----------------------------------------------------------------------

  int iv[3]={0,0,0}; // 
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    for( int p=0; p<np; p++ )
      numToSend(grid,p)=0;

    int ni=cg.numberOfInterpolationPoints(grid);
    if( ni==0 ) continue;

    const intArray & mask = cg[grid].mask();

    intSerialArray ip; 
    if( ( grid<cg.numberOfBaseGrids() && 
	  cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
	cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )
    {
      // use the interpolation data in the parallel arrays
      // *NO: getLocalArrayWithGhostBoundaries(cg.interpolationPoint[grid],ip);
      ip.reference( cg.interpolationPoint[grid].getLocalArray());
    }
    else
    {
      // NOTE: the "local" arrays may not be local to this processor (e.g. if created by Ogmg)

      // use the interpolation data in the serial arrays
      ip.reference( cg->interpolationPointLocal[grid]);
    }

    ni = ip.getLength(0);      // *** is this correct or do we need to use the ise array??
    for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
	iv[axis]=ip(i,axis);
      int p= mask.Array_Descriptor.findProcNum( iv );  // interp. pt. lives on this processor

      numToSend(grid,p)++;
    }
  } // end for grid 
  
  // send/receive counts on interp pts
  int tag0=61037;  // try to make a unique tag
  MPI_Status status;
  for( int p=0; p<np; p++ )
  {
    int tags=tag0+p, tagr=tag0+myid;
    MPI_Sendrecv(&numToSend(0,p),    numberOfComponentGrids, MPI_INT, p, tags, 
                 &numToReceive(0,p), numberOfComponentGrids, MPI_INT, p, tagr, MY_COMM, &status ); 
  }

  const int numberOfIntsToReceivePerPoint = 2*numberOfDimensions+2; // ip(nd), il(nd), ig, viw
  const int numberOfRealsToReceivePerPoint = numberOfDimensions;    // ci(nd)
  

  if( debug & 2 )
  {
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
      fprintf(debugFile," grid=%i receive: (num,p) = ",grid);
      for( int p=0; p<np; p++ ) 
	fprintf(debugFile," (%i,%i) ",numToReceive(0,p),p);
      fprintf(debugFile,"\n");
      fprintf(debugFile," grid=%i send   : (num,p) = ",grid);
      for( int p=0; p<np; p++ ) 
	fprintf(debugFile," (%i,%i) ",numToSend(0,p),p);
      fprintf(debugFile,"\n");
    }
  }
  

  // totalNumToReceive(p) : number of pts we receive from p (total over all grids)
  // totalNumToSend(p)    : number of pts we send to p (total over all grids)
  int *pTotalNumToReceive = new int [np];
#define totalNumToReceive(p)  pTotalNumToReceive[p]
  int *pTotalNumToSend = new int [np];
#define totalNumToSend(p)  pTotalNumToSend[p]
  for( int p=0; p<np; p++ )
  {
    totalNumToReceive(p)=0;
    totalNumToSend(p)=0;
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
      totalNumToReceive(p)+=numToReceive(grid,p);
      totalNumToSend(p)   +=numToSend(grid,p);
    }
  }

  // ------------------------------------------------------------------------------------------------
  // npr = number of processors that we will receive data from 
  // nps = number of processors that we will send data to 
  // ------------------------------------------------------------------------------------------------
  int npr=0;  
  int nps=0;  
  for( int p=0; p<np; p++ )
  {
    // NOTE: npr depends on numToSend : this is how many we rec. in assign
    if( totalNumToReceive(p)>0 || sendZeroLengthMessages ){ npr++; }
    if( totalNumToSend(p)>0    || sendZeroLengthMessages ){ nps++; }
  }
  
  // Define the processor maps: 
  //   mapr(p) p=0,..,npr  : actual processor numbers we receive from (in assign stage)
  //   maps(p) p=0,..,nps  : actual processor numbers we send to      (in assign stage)
  int *pMapr = new int [npr];    
  int *pMaps = new int [nps];
  #define mapr(p) pMapr[p]
  #define maps(p) pMaps[p]
  // if mapr(p)=pp then imapr(pp) = p  i.e. imapr is the inverse of mapr 
  int *piMapr = new int [np];    
  #define imapr(p) piMapr[p]
  // imaps is the inverse of maps 
  int *piMaps = new int [np];    
  #define imaps(p) piMaps[p]

  int kr=0, ks=0;
  for( int p=0; p<np; p++ )
  {
    imapr(p)=-1; // -1 means there there is no inverse
    imaps(p)=-1; // -1 means there there is no inverse
    if( totalNumToReceive(p)>0 || sendZeroLengthMessages ){ mapr(kr)=p; imapr(p)=kr; kr++; }
    if( totalNumToSend(p)>0    || sendZeroLengthMessages ){ maps(ks)=p; imaps(p)=ks; ks++; } 
  }
  assert( kr==npr && ks==nps );

  if( debug & 2 )
  {
    fprintf(debugFile," *** npr=%i, nps=%i\n",npr,nps);
    fprintf(debugFile," mapr=");
    for( int p=0; p<npr; p++ ) fprintf(debugFile,"%i, ",mapr(p));
    fprintf(debugFile,"\n");
    fprintf(debugFile," imapr=");
    for( int p=0; p<npr; p++ ) fprintf(debugFile,"%i, ",imapr(p));
    fprintf(debugFile,"\n");
    fprintf(debugFile," maps=");
    for( int p=0; p<nps; p++ ) fprintf(debugFile,"%i, ",maps(p));
    fprintf(debugFile,"\n");

    fflush(debugFile);
  }

  //  numInterpPerGrid(grid) = total interp pts per grid on this proc. 
  int *pNumInterpPerGrid = new int [numberOfComponentGrids];
#define numInterpPerGrid(grid)  pNumInterpPerGrid[grid]
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    numInterpPerGrid(grid)=0;
    for( int p=0; p<np; p++ )
     numInterpPerGrid(grid)+=numToReceive(grid,p);
  }
  
  // Allocate send-buffers and receive-buffers
  int **psbuffi  = new int*  [nps];   // int send buffer
  real **psbuffr = new real* [nps];   // real send buffer
  int **prbuffi  = new int*  [npr];   // int rec. buffer
  real **prbuffr = new real* [npr];   // real rec. buffer
  for( int p=0; p<npr; p++ )
  {
    const int pp = mapr(p); // actual proc. number 
    const int numr =  totalNumToReceive(pp);
    prbuffi[p] = new int  [ max(1,numr*numberOfIntsToReceivePerPoint) ];
    prbuffr[p] = new real [ max(1,numr*numberOfRealsToReceivePerPoint) ];
  }
  for( int p=0; p<nps; p++ )
  {
    const int pp = maps(p); // pp is the actual processor id in [0,np-1]
    const int nums =  totalNumToSend(pp);
    psbuffi[p] = new int  [ nums*numberOfIntsToReceivePerPoint ];
    psbuffr[p] = new real [ nums*numberOfRealsToReceivePerPoint ];
  }
  
#define sbuffi(i,p) psbuffi[p][i]
#define sbuffr(i,p) psbuffr[p][i]

#define rbuffi(i,p) prbuffi[p][i]
#define rbuffr(i,p) prbuffr[p][i]

  MPI_Request *sendRequest    = new MPI_Request[nps];   
  MPI_Status *sendStatus      = new MPI_Status [nps];
  MPI_Request *receiveRequest = new MPI_Request[npr];
  MPI_Status *receiveStatus   = new MPI_Status [npr];

  // for real's 
  MPI_Request *sendRequestr    = new MPI_Request[nps];   
  MPI_Status *sendStatusr      = new MPI_Status [nps];
  MPI_Request *receiveRequestr = new MPI_Request[npr];
  MPI_Status *receiveStatusr   = new MPI_Status [npr];


  // --- post receives ---
  const int tag1=72934; // make a unique tag
  const int tag2=31044; // make a unique tag
  for( int p=0; p<npr; p++ )
  {
    const int pp = mapr(p);  // pp is the actual processor id in [0,np-1]
    int tag=tag1+myid;
    int tnumi = totalNumToReceive(pp)*numberOfIntsToReceivePerPoint;
    MPI_Irecv(prbuffi[p],tnumi,MPI_INT ,pp,tag,MY_COMM,&receiveRequest[p] );
    tag=tag2+myid;
    int tnumr = totalNumToReceive(pp)*numberOfRealsToReceivePerPoint;
    MPI_Irecv(prbuffr[p],tnumr,MPI_Real,pp,tag,MY_COMM,&receiveRequestr[p] );
  }

   
  int *numi = new int [max(npr,nps)];
  int *numr = new int [max(npr,nps)];
  
  // set counts to zero
  for( int p=0; p<nps; p++ )
  {
    numi[p]=0;
    numr[p]=0;
  }

  // Fill buffers will data to send to each processor
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    int ni=cg.numberOfInterpolationPoints(grid);
    if( ni==0 ) continue;

    const intArray & mask = cg[grid].mask();

    intSerialArray ip,il,ig,viw;
    realSerialArray ci; 

    if( ( grid<cg.numberOfBaseGrids() && 
	  cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
	cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )
    {

      // use the interpolation data in the parallel arrays
//       getLocalArrayWithGhostBoundaries(cg.interpolationPoint[grid],ip);
//       getLocalArrayWithGhostBoundaries(cg.interpoleeLocation[grid],il);
//       getLocalArrayWithGhostBoundaries(cg.interpoleeGrid[grid],ig);
//       getLocalArrayWithGhostBoundaries(cg.variableInterpolationWidth[grid],viw);
//       getLocalArrayWithGhostBoundaries(cg.interpolationCoordinates[grid],ci);
      ip.reference( cg.interpolationPoint[grid].getLocalArray());
      il.reference( cg.interpoleeLocation[grid].getLocalArray());
      ig.reference( cg.interpoleeGrid[grid].getLocalArray());
      viw.reference( cg.variableInterpolationWidth[grid].getLocalArray());
      ci.reference(cg.interpolationCoordinates[grid].getLocalArray());
    }
    else
    {
      // use the interpolation data in the serial arrays (for now these are refinement grids)
      ip.reference( cg->interpolationPointLocal[grid]);
      il.reference( cg->interpoleeLocationLocal[grid]);
      ig.reference( cg->interpoleeGridLocal[grid]);
      viw.reference( cg->variableInterpolationWidthLocal[grid]);
      ci.reference(cg->interpolationCoordinatesLocal[grid]);
    }

    int niLocal = 0; // cg->numberOfInterpolationPointsLocal(grid);
    ni = ip.getLength(0);
    // assert( ni==ip.getLength(0) );
    if( debug )
    {
      fprintf(debugFile," grid=%i cg.numberOfInterpolationPoints(grid)=%i ip.getLength(0)=%i niLocal=%i\n",
              grid,cg.numberOfInterpolationPoints(grid),ip.getLength(0),niLocal);
      if( debug & 4 )
      {
	for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
	{
	  fprintf(debugFile," myid=%i : i=%5i ip=(%4i,%4i,%4i) il=(%4i,%4i,%4i) ig=%4i viw=%2i ci=(%5.2f,%5.2f,%5.2f)\n",
		  myid,i,
		  ip(i,0),ip(i,1),(numberOfDimensions==2 ? 0 : ip(i,2)),
		  il(i,0),il(i,1),(numberOfDimensions==2 ? 0 : il(i,2)),ig(i),viw(i),
		  ci(i,0),ci(i,1),(numberOfDimensions==2 ? 0 : ci(i,2))
	    );
	}
      }
    }
    
    for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
	iv[axis]=ip(i,axis);
      const int pp= mask.Array_Descriptor.findProcNum( iv );  // interp. pt. lives on this processor

      const int p = imaps(pp);
      assert( p>=0 && p<nps );
      
      int & ki = numi[p];
      int & kr = numr[p];

      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	sbuffi(ki,p) = ip(i,axis);  ki++;
        // if( debug ) fprintf(debugFile," i=%i axis=%i ip=%i\n",i,axis,ip(i,axis));
      } 
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	sbuffi(ki,p) = il(i,axis);  ki++;
        // if( debug ) fprintf(debugFile," i=%i axis=%i il=%i\n",i,axis,il(i,axis));
      } 
      sbuffi(ki,p) = ig(i);   ki++;
      sbuffi(ki,p) = viw(i);  ki++;
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	sbuffr(kr,p) = ci(i,axis);  kr++;
      } 
    }
  }

  // Send the data 
  // --- send info ---
  for( int p=0; p<nps; p++ )
  {
    const int pp = maps(p);  // pp is the actual processor id in [0,np-1]
    if( debug ) 
    {
      assert( numi[p]==totalNumToSend(pp)*numberOfIntsToReceivePerPoint );
      assert( numr[p]==totalNumToSend(pp)*numberOfRealsToReceivePerPoint );
      

      fprintf(debugFile,"myid=%i: send (%i,%i) (ints,reals) to p=%i : values=",myid,
	      numi[p],numr[p],pp);
      if( debug & 4 )
        for( int j=0; j<numi[p]; j++ ) fprintf(debugFile,"%i ",sbuffi(j,p)); 
      else
        fprintf(debugFile,"not printed");
      fprintf(debugFile,"\n");
      fflush(debugFile);
    }
    // int tag=numi[p];
    int tag=tag1+pp;
    MPI_Isend(psbuffi[p],numi[p],MPI_INT ,pp,tag,MY_COMM,&sendRequest[p] );
    tag=tag2+pp;
    MPI_Isend(psbuffr[p],numr[p],MPI_Real,pp,tag,MY_COMM,&sendRequestr[p] );
  }

  MPI_Waitall( npr, receiveRequest, receiveStatus );    // wait to receive all messages
  MPI_Waitall( npr, receiveRequestr, receiveStatusr );  // wait to receive all messages


  if( debug )
  {
    for( int p=0; p<npr; p++ )
    {
      const int pp = mapr(p);  

      int numir=totalNumToReceive(pp)*numberOfIntsToReceivePerPoint;
      int numi=0;
      MPI_Get_count( &receiveStatus[p], MPI_INT, &numi );
      assert( numir==numi );

      fprintf(debugFile,"received msg from p=%i, tag=%i p=%i i-values=",
	      receiveStatus[p].MPI_SOURCE,receiveStatus[p].MPI_TAG,pp);
      if( debug & 4  )
	for( int j=0; j<numi; j++ ) fprintf(debugFile,"%i ",rbuffi(j,p));
      else
        fprintf(debugFile,"not printed");
      
      fprintf(debugFile,"\n");
    }
  }

  // --------------------------------
  // --- Unpack the received data ---
  // --------------------------------

  // set counts to zero
  for( int p=0; p<npr; p++ )
  {
    numi[p]=0;
    numr[p]=0;
  }
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    InterpolationData & ipd = interpData[grid];

    intSerialArray & ip = ipd.interpolationPoint;
    intSerialArray & il = ipd.interpoleeLocation;
    intSerialArray & ig = ipd.interpoleeGrid;
    intSerialArray & viw= ipd.variableInterpolationWidth;
    realSerialArray & ci= ipd.interpolationCoordinates;

    if( debug & 2 )
    {
      fprintf(debugFile," grid=%i numInterpPerGrid(grid)=%i\n",grid,numInterpPerGrid(grid));
      fflush(debugFile);
    }


    if( numInterpPerGrid(grid)>0 )
    {
      ip.redim(numInterpPerGrid(grid),numberOfDimensions);
      il.redim(numInterpPerGrid(grid),numberOfDimensions);
      ig.redim(numInterpPerGrid(grid));
      viw.redim(numInterpPerGrid(grid));
      ci.redim(numInterpPerGrid(grid),numberOfDimensions);
    }
    else
    {
      ip.redim(0);
      il.redim(0);
      ig.redim(0);
      viw.redim(0);
      ci.redim(0);
    }
    
    
    int ni=0; // counts interp pts on this grid 
    for( int p=0; p<npr; p++ )
    {
      const int pp = mapr(p);  // pp is the actual processor id in [0,np-1]
      int & ki = numi[p]; // note reference 
      int & kr = numr[p]; // note reference 

      for( int k=0; k<numToReceive(grid,pp); k++ )
      {
        assert( ni<numInterpPerGrid(grid) );

	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  ip(ni,axis) = rbuffi(ki,p);  ki++;
	} 
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  il(ni,axis) = rbuffi(ki,p);  ki++;
	} 
	ig(ni) = rbuffi(ki,p);  ki++;
	viw(ni)= rbuffi(ki,p);  ki++;
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  ci(ni,axis)= rbuffr(kr,p);  kr++;
	} 
        #ifdef GLID_DEBUG
	if( debug & 4 )
	{
          fprintf(debugFile," received pt ni=%i from pp=%i (p=%i) : "
                  "ip=(%4i,%4i,%4i) il=(%4i,%4i,%4i) ig=%4i viw=%2i ci=(%5.2f,%5.2f,%5.2f)\n",
		  ni,pp,p,               
                  ip(ni,0),ip(ni,1),(numberOfDimensions==2 ? 0 : ip(ni,2)),
                  il(ni,0),il(ni,1),(numberOfDimensions==2 ? 0 : il(ni,2)),ig(ni),viw(ni),
		  ci(ni,0),ci(ni,1),(numberOfDimensions==2 ? 0 : ci(ni,2)));
	}
	#endif 

	ni++;
      }
    } // end for p 
    assert( ni == numInterpPerGrid(grid) );
    ipd.numberOfInterpolationPoints=ni;

  } // end for grid 


  // clean up
  // we must wait for the send's to complete before deleting the buffers
  MPI_Waitall( nps, sendRequest, sendStatus );  // wait to receive all messages
  MPI_Waitall( nps, sendRequestr, sendStatusr );  // wait to receive all messages

  delete [] numToSendp;
  delete [] numToReceivep;
  
  delete [] pTotalNumToReceive;
  delete [] pTotalNumToSend;
  
  delete [] pMapr;
  delete [] pMaps;
  delete [] piMapr;
  delete [] piMaps;

  for( int p=0; p<npr; p++ )
  {
    delete [] prbuffi[p];
    delete [] prbuffr[p];
  }
  for( int p=0; p<nps; p++ )
  {
    delete [] psbuffi[p];
    delete [] psbuffr[p];
  }
  delete [] psbuffi;
  delete [] psbuffr;
  delete [] prbuffi;
  delete [] prbuffr;
  
  delete [] pNumInterpPerGrid;
  
  delete [] numi;
  delete [] numr;
  

  delete [] sendRequest;
  delete [] sendStatus;
  delete [] receiveRequest;
  delete [] receiveStatus;

  delete [] sendRequestr;
  delete [] sendStatusr;
  delete [] receiveRequestr;
  delete [] receiveStatusr;

  if( debugFile!=NULL )
    fclose(debugFile);

  return 0;

#endif

}
