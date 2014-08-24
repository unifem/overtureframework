// ==============================================================================================
//  This class is used to assign unused points next to interpolation points so that wider
//  stencils can be applied.
//
// ==============================================================================================


#include "AssignInterpNeighbours.h"
#include "display.h"
#include "ParallelUtility.h"
#include "MappedGridOperators.h"

#define extrapInterpNeighboursOpt EXTERN_C_NAME(extrapinterpneighboursopt)
// new version: 
#define findInterpNeighbours EXTERN_C_NAME(findinterpneighbours)
extern "C"
{
   void findInterpNeighbours( const int &nd, 
      const int &ndm1a,const int &ndm1b,const int &ndm2a,const int &ndm2b,const int &ndm3a,const int &ndm3b,
      const int &ndi,const int &ndin, const int&indexRange, const int &dimension, const int & pbc,
              const int &ni, int &nin, 
              const int &mask, int &m, const int &ip,int &id, int &ia, 
              int &vew, int & status, int &ipar, int & ierr );

  void extrapInterpNeighboursOpt(const int&nd, 
    const int&ndu1a,const int&ndu1b,const int&ndu2a,const int&ndu2b,const int&ndu3a,const int&ndu3b,
    const int&ndu4a,const int&ndu4b,
    const int&nda1a,const int&nda1b,const int&ndd1a,const int&ndd1b,
    const int&ia,const int&id, const int &vew, real & u,const int&ca,const int&cb, const int& ipar, const real&rpar );
}

int AssignInterpNeighbours::debug=0;
static int numberOfReduceOrderOfExtrapMessages=0;

FILE* AssignInterpNeighbours::debugFile=NULL;

// ===========================================================================
/// \brief Constructor for the class that assigns interpolation neighbours.
// ===========================================================================
AssignInterpNeighbours::
AssignInterpNeighbours()
{
  setup();
}
// ===========================================================================
/// \brief Setup routine (protected)
// ===========================================================================
int AssignInterpNeighbours::
setup()
{
  // Initialize all variables.

  AIN_COMM = Overture::OV_COMM;  // use this communicator by default

  isInitialized=false;

  errorStatus=noErrors; // is this really needed?
  
  numberOfInterpolationNeighbours=0;

  // For dw=3 : we may have a single D=discretization point between to I=interp. pts so the
  // the max. width for E=extrapolation is 4: 
  //     E I D I 
  // We could allow for variable width
//  maximumWidthToExtrapolationInterpolationNeighbours=4;  // This should be at most dw+1
  maximumWidthToExtrapolationInterpolationNeighbours=
       MappedGridOperators::defaultMaximumWidthToExtrapolationInterpolationNeighbours;  // This should be at most dw+1
  extrapolateInterpolationNeighbourPoints=NULL;
  extrapolateInterpolationNeighboursDirection=NULL;
  extrapolateInterpolationNeighboursVariableWidth=NULL;  

  interpolationPoint=NULL; // this points to the cg.interpolationPoint[grid]

  // npr = number of processors that we will receive data from
  // nps = number of processors that we will send data to 
  npr=0; nps=0;
  ppr=NULL; pps=NULL;

  // list of points to recieve/send from other processors
  nar=NULL; iar=NULL; 
  nas=NULL; ias=NULL;  

  return 0;
}


// ===========================================================================
/// \brief Copy constructor.
// ===========================================================================
AssignInterpNeighbours::
AssignInterpNeighbours( const AssignInterpNeighbours & x )
{
  setup();
  *this=x;
}


// ===========================================================================
/// \brief Destructor for the class that assigns interpolation neighbours.
// ===========================================================================
AssignInterpNeighbours::
~AssignInterpNeighbours()
{
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  delete extrapolateInterpolationNeighbourPoints;
  delete extrapolateInterpolationNeighboursDirection;
  delete extrapolateInterpolationNeighboursVariableWidth;

  if( nar!=NULL )
  {
    delete [] nar;
    for( int p=0; p<npr; p++ )
      delete iar[p];
    delete [] iar;
  }
  if( nas!=NULL )
  {
    delete [] nas;
    for( int p=0; p<nps; p++ )
      delete ias[p];
    delete [] ias;
  }
  delete [] ppr;
  delete [] pps;
}

// ===========================================================================
/// \brief provide the interpolation point array, cg.interpolationPoint[grid],
//   (used in serial only)
// ===========================================================================
void AssignInterpNeighbours::
setInterpolationPoint( intArray & interpolationPoint_ )
{
  interpolationPoint=&interpolationPoint_;
}

// =====================================================================================
/// \brief  Call this routine when the grid has changed and we need to re-initialize.
// =====================================================================================
int AssignInterpNeighbours::
gridHasChanged()
{
  isInitialized=false;
  // we could delete existing arrays here -- otherwise they are destroyed at re-initialization

  return 0;
}

// =====================================================================================
/// \brief operator =, deep copy.
// =====================================================================================
AssignInterpNeighbours & AssignInterpNeighbours::
operator= ( const AssignInterpNeighbours & x )
{
  // Here we just invalidate the current object so that it will be re-initialized.
  
  // Should we do better here and copy all the data??

  isInitialized=false;

  return *this;
}


// =====================================================================================
/// \brief Return size of this object.
// =====================================================================================
real AssignInterpNeighbours::
sizeOf(FILE *file /* = NULL */ ) const
{
  real size=sizeof(*this);
  if( extrapolateInterpolationNeighbourPoints!=NULL )
  {
    size+=(extrapolateInterpolationNeighbourPoints->elementCount()+
           extrapolateInterpolationNeighboursDirection->elementCount())*sizeof(int);
  }
  if( extrapolateInterpolationNeighboursVariableWidth!=NULL )
  {
    size+=(extrapolateInterpolationNeighboursVariableWidth->elementCount())*sizeof(int);
  }
  
  const int numberOfDimensions=3; // upper bound
  if( npr>0 )
  {
    size += npr*sizeof(int);   // nar
    size += 2*npr*sizeof(int*);  // *iar + *ppr
    for( int p=0; p<npr; p++ )
      size += nar[p]*numberOfDimensions*sizeof(int);
  }
  if( nps>0 )
  {
    size += nps*sizeof(int);   // nas
    size += 2*nps*sizeof(int*);  // *ias + *pps
    for( int p=0; p<nps; p++ )
      size += nas[p]*numberOfDimensions*sizeof(int);
  }
  

  return size;
}


// ===============================================================================================
/// \brief Find the unused points that need to be assigned.
/// \details
///   Initialization routine: find the unused points that lie next to interpolation points.
///  We also assign corners too (for AMR interpolation). In parallel , setup the comminication
/// schedule. 
// ================================================================================================
int AssignInterpNeighbours::
findInterpolationNeighbours( MappedGrid & mg )
{
  const int numberOfDimensions = mg.numberOfDimensions();
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=max(1,Communication_Manager::Number_Of_Processors);


  // this routine should only be called if we need to initialize
  assert( !isInitialized );
  isInitialized=true;

  numberOfInterpolationNeighbours=0;

  #undef AIN_DEBUG

  // debug=7;  // *********************
  // #define AIN_DEBUG

  if( debug >0 && debugFile==NULL )
  {
    aString fileName=sPrintF("finNP%ip%i.debug",np,myid);
    debugFile=fopen((const char*)fileName,"w"); // open a different file on each proc.
    printF("AssignInterpNeighbours: output written to debug file %s\n",(const char*)fileName);
  }
  if( debug>0 )
  {
    fprintf(debugFile,"**** findInterpolationNeighbours grid=%s ******\n",(const char*)mg.getName());
    ::display(mg.gridIndexRange(),"gridIndexRange",debugFile,"%i ");
    ::display(mg.extendedIndexRange(),"extendedIndexRange",debugFile,"%i ");
    ::display(mg.boundaryCondition(),"boundaryCondition",debugFile,"%i ");
  }

  int n1a,n1b,n2a,n2b,n3a,n3b;
  if( interpolationPoint!=NULL )
  {
    // printf("**** findInterpolationNeighbours: use opt version -- interpolationPoint found! ****\n");

    const int dw = max(mg.discretizationWidth());
    // printf("AssignInterpNeighbours::findInterpolationNeighbours: dw=%i\n",dw);
    
    if( maximumWidthToExtrapolationInterpolationNeighbours > dw+1 )
    {
      printf("AssignInterpNeighbours::findInterpolationNeighbours: WARNING: "
             " maximumWidthToExtrapolationInterpolationNeighbours = %i > discretizationWidth+1 = %i\n"
             " It may not be possible to extrapolate to this order.\n",
             maximumWidthToExtrapolationInterpolationNeighbours,dw+1);
    }
    

    IntegerArray pbc(2,3);
    pbc=0;  // set to one if pbc(side,axis) is an internal parallel boundary

     #ifdef USE_PPP

       // In the parallel case we find the interpolation points on this processor using the local mask

       const intArray & maskd = mg.mask();
       intSerialArray mask; getLocalArrayWithGhostBoundaries(maskd,mask); 

       IntegerArray ip;
       Index I1,I2,I3;
       getIndex(mg.extendedIndexRange(),I1,I2,I3);  // includes ghost lines on interpolation boundaries

       // The local mask array includes ghost boundaries on ALL sides
       n1a=max(I1.getBase() ,mask.getBase(0) +maskd.getGhostBoundaryWidth(0));
       n1b=min(I1.getBound(),mask.getBound(0)-maskd.getGhostBoundaryWidth(0));

       n2a=max(I2.getBase() ,mask.getBase(1) +maskd.getGhostBoundaryWidth(1));
       n2b=min(I2.getBound(),mask.getBound(1)-maskd.getGhostBoundaryWidth(1));

       n3a=max(I3.getBase() ,mask.getBase(2) +maskd.getGhostBoundaryWidth(2));
       n3b=min(I3.getBound(),mask.getBound(2)-maskd.getGhostBoundaryWidth(2));
       
       // pbc(side,axis)=1 if this face  is an internal parallel boundary
       // printf(" Bounds:  mask=[%i,%i][%i,%i] maskd=[%i,%i][%i,%i]\n",
       //	      mask.getBase(0), mask.getBound(0),
       //	      mask.getBase(1), mask.getBound(1),
       //	      maskd.getBase(0), maskd.getBound(0),
       //	      maskd.getBase(1), maskd.getBound(1));

       // NOTE: mask.getBase(0) = maskd.getBase(0) - maskd.getGhostBoundaryWidth(0) on the "left edge"
       for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
       {
	 if( mask.getBase(axis)>maskd.getBase(axis) )
           pbc(0,axis)=1;
	 if( mask.getBound(axis)<maskd.getBound(axis) )
           pbc(1,axis)=1;
       }
       // pbc=0;
       

       IntegerArray extendedIndexRange(2,3);  // local version for this processor
       IntegerArray dimension(2,3);           // local version for this processor
       // IntegerArray dimension(2,3);           // local version for this processor -- do NOT include parallel ghost
       if( n1a<=n1b && n2a<=n2b && n3a<=n3b )
       {
//          for( int dir=0; dir<3; dir++ )
// 	 {
// 	   dimension(0,dir)=max(mg.dimension(0,dir),mask.getBase(dir) +maskd.getGhostBoundaryWidth(dir));
// 	   dimension(1,dir)=min(mg.dimension(1,dir),mask.getBound(dir)-maskd.getGhostBoundaryWidth(dir));
// 	 }
	 

         extendedIndexRange(0,0)=n1a; extendedIndexRange(1,0)=n1b;
         extendedIndexRange(0,1)=n2a; extendedIndexRange(1,1)=n2b;
         extendedIndexRange(0,2)=n3a; extendedIndexRange(1,2)=n3b;
	 
         // determine interpolation points on this processor -- 
         // *wdh* 060523: include interp. pts on the first parallel ghost line if 
         //               there are more than 1 parallel ghost line.
         if( maskd.getGhostBoundaryWidth(0)>1 )
  	   I1 = Range(n1a-1,n1b+1);  
         else
  	   I1 = Range(n1a,n1b);
	 if( maskd.getGhostBoundaryWidth(1)>1 )
           I2 = Range(n2a-1,n2b+1);
         else
           I2 = Range(n2a,n2b);
         if( maskd.getGhostBoundaryWidth(2)>1 )
  	   I3 = Range(n3a-1,n3b+1);
         else
	   I3 = Range(n3a,n3b);

         // optimize this: 
	 ip = (mask(I1,I2,I3)<0).indexMap(); // interpolation points on this processor;

       }
  
       const int ni=ip.getLength(0);

       for( int axis=0; axis<3; axis++ )
       {
	 dimension(0,axis)=mask.getBase(axis) +maskd.getGhostBoundaryWidth(axis);
	 dimension(1,axis)=mask.getBound(axis)-maskd.getGhostBoundaryWidth(axis);
       }
       
  
     #else

       const IntegerArray & extendedIndexRange = mg.extendedIndexRange();
       const IntegerArray & dimension = mg.dimension();

       const intArray & mask = mg.mask();
       intArray & ip = *interpolationPoint;
       const int ni=ip.getLength(0);

     #endif

     if( extrapolateInterpolationNeighbourPoints==NULL )
     {
       extrapolateInterpolationNeighbourPoints=new IntegerArray;
       extrapolateInterpolationNeighboursDirection=new IntegerArray;
     }
  
     IntegerArray & ia = *extrapolateInterpolationNeighbourPoints;
     IntegerArray & id = *extrapolateInterpolationNeighboursDirection;
     IntegerArray status;
       
     int ndin=0;  // dimension for interp arrays 
     if( ni>0 )
     {
	 
       // estimate the max number of interpolation point neighbours
       ndin = ni*numberOfDimensions*numberOfDimensions+100;
       if( numberOfDimensions==1 )
	 ndin=ni*2+100;
       else if( numberOfDimensions==2 )
	 ndin=ni*2+100;
       else
	 ndin=ni*3+1000;
     

       ia.redim(ndin,numberOfDimensions);
       id.redim(ndin,numberOfDimensions);

       // The mask array m is used to mark all interp neighbours that are needed to avoid
       // duplicated counting.
       IntegerArray m(mask.dimension(0),mask.dimension(1),mask.dimension(2));

       // In parallel status(i)=0 will indicate whether a point can be extrapolated on this processor.
       status.redim(ndin);
       status=0;

       int *pia = getDataPointer(ia);
     
       int ipar[5];
       ipar[0]=maximumWidthToExtrapolationInterpolationNeighbours;
       bool useVariableExtrapolation=false;
       ipar[1]=(int)useVariableExtrapolation;
       int numberOfGhostPointsAvailable=min(mg.numberOfGhostPoints()(Range(0,1),Range(0,numberOfDimensions-1)));
       ipar[2]=numberOfGhostPointsAvailable; 
#ifdef USE_PPP
       ipar[2]=min(ipar[2],maskd.getGhostBoundaryWidth(0)); // assume all ghost boundary widths are the same
#endif
       int *pvew = extrapolateInterpolationNeighboursVariableWidth!=NULL ? 
	 getDataPointer(*extrapolateInterpolationNeighboursVariableWidth) : pia;
     
       ipar[3]=myid;

       int ierr=0;
       findInterpNeighbours( numberOfDimensions,
			     mask.getBase(0),mask.getBound(0),
			     mask.getBase(1),mask.getBound(1),
			     mask.getBase(2),mask.getBound(2),
			     ni,ndin,extendedIndexRange(0,0),dimension(0,0),pbc(0,0),
			     ni,numberOfInterpolationNeighbours,
			     *getDataPointer(mask),
			     *getDataPointer(m),*getDataPointer(ip), *getDataPointer(id), 
			     *pia, *pvew, status(0), ipar[0], ierr );

       if( ierr!=0 )
       {
	 printF("AssignInterpNeighbours::findInterpolationNeighbours:ERROR return from findInterpNeighbours!\n");
	 OV_ABORT("error");
       }


//      if( ipar[0]<maximumWidthToExtrapolationInterpolationNeighbours )
//      {
//        maximumWidthToExtrapolationInterpolationNeighbours=ipar[0];
//        printf("GenericMappedGridOperators::findInterpolationNeighbours:max extrapolation width reduced to %i\n",
// 	      maximumWidthToExtrapolationInterpolationNeighbours);
//      }
       
     } // end if ni>0 
     
     assert( numberOfInterpolationNeighbours <= ndin );

#ifdef USE_PPP
     
     // *********************************************
     // ************ Parallel ***********************
     // *********************************************
     if( debug & 2 )
     {
       fprintf(debugFile," Bounds:  mask=[%i,%i][%i,%i] maskd=[%i,%i][%i,%i]\n",
	       mask.getBase(0), mask.getBound(0),
	       mask.getBase(1), mask.getBound(1),
	       maskd.getBase(0), maskd.getBound(0),
	       maskd.getBase(1), maskd.getBound(1));
       if( debug & 4 )
	 displayMask(mask,"mask",debugFile);

     }

     int *iap = ia.Array_Descriptor.Array_View_Pointer1;
     const int iaDim0=ia.getRawDataSize(0);
     #define IA(i0,i1) iap[i0+iaDim0*(i1)]

     int *idp = id.Array_Descriptor.Array_View_Pointer1;
     const int idDim0=id.getRawDataSize(0);
     #define ID(i0,i1) idp[i0+idDim0*(i1)]

     const int numberOfDimensions=mg.numberOfDimensions();
     const int shift=2; // what should this be?
     int index[3]={0,0,0}; //

     int *pNumToReceive = new int [np];            
     #define numToReceive(p) pNumToReceive[p]
     int *pNumToSend = new int [np];               
     #define numToSend(p) pNumToSend[p]
     for( int p=0; p<np; p++ )
     {
       numToReceive(p)=0;
       numToSend(p)=0;
     }
     for( int i=0; i<numberOfInterpolationNeighbours; i++ )
     {
       //   status(i)= 0 : means we can evaluate the pt on this processor: 
       //            = 2 : this point cannot extrap on this processor
       //            =-1 : this point is not needed

       if( status(i)==0 )
       {
         #ifdef AIN_DEBUG
	 if( debug & 4 )
	   fprintf(debugFile," myid=%i: pt i=%i, ia=(%i,%i) id=(%i,%i) is assigned from this proc.\n",
		   myid,i,IA(i,0),IA(i,1),ID(i,0),ID(i,1));
         #endif
       }
       else if( status(i)==2 )
       {
	 // check if this point is needed from another processor
         for( int axis=0; axis<numberOfDimensions; axis++ )
	 { // index of a point on the extrapolation stencil that should be on the next processor
           index[axis]=IA(i,axis)+shift*ID(i,axis);
	 }
	 int sp= maskd.Array_Descriptor.findProcNum( index );

         status(i)=10+sp;  // save the proc num
         #ifdef AIN_DEBUG
         if( debug & 2 )
	   fprintf(debugFile," myid=%i: pt i=%i, ia=(%i,%i) id=(%i,%i) is obtained from proc. sp=%i.\n",
                 myid,i,IA(i,0),IA(i,1),ID(i,0),ID(i,1),sp);
         #endif
	 
	 // OV_ABORT("error");
         numToSend(sp)++;
	 

       }
       else if( status(i)==-1 )
       {
	 // this point not needed 
       }
       else
       {
         // this case should not happen 
         OV_ABORT("FIN: error");
       }

     }
     
     if( debug & 1 )
     {
       fprintf(debugFile," Send this many pts: numToSend=");
       for( int p=0; p<np; p++ )
	 fprintf(debugFile," %i,",numToSend(p));
       fprintf(debugFile,"\n");
       fflush(debugFile);
     }
     const int numToSendPerProc=2*numberOfDimensions; // we send ia(nd),id(nd)
     for( int p=0; p<np; p++ )
     {
       numToSend(p) = numToSend(p)*numToSendPerProc;  // here is how much data we will send
     }
	  
     int tag0=501346;  // try to make a unique tag
     MPI_Status mpiStatus;
     for( int p=0; p<np; p++ )
     {
       int tags=tag0+p, tagr=tag0+myid;
       MPI_Sendrecv(&numToSend(p),    1, MPI_INT, p, tags, 
		    &numToReceive(p), 1, MPI_INT, p, tagr, AIN_COMM, &mpiStatus ); 
     }
     if( debug & 1 )
     {
       fprintf(debugFile," receive this many pts: numToReceive=");
       for( int p=0; p<np; p++ )
	 fprintf(debugFile," %i,",numToReceive(p)/numToSendPerProc);
       fprintf(debugFile,"\n");
       fflush(debugFile);
     }

     // Delete Old arrays using old values for npr and nps
     if( iar !=NULL )
     { 
       // assert( npr>0 );
       assert( nar!=NULL );
       for( int p=0; p<npr; p++ )
	 delete iar[p];
       delete [] iar; iar=NULL;
       delete [] nar; nar=NULL;
     }
     if( ias !=NULL )
     { 
       // assert( nps>0 );
       assert( nas!=NULL );
       for( int p=0; p<nps; p++ )
	 delete ias[p];
       delete [] ias; ias=NULL;
       delete [] nas; nas=NULL;
     }

     // npr = number of processors that we will receive data from IN THE ASSIGN stage which equals
     //          the num. of proc. that we send to in the SETUP stage!
     // nps = number of processors that we will send data to      IN THE ASSIGN stage  which equals
     //          the num. of proc. that we rec. from in the SETUP stage!
     npr=0;  
     nps=0;  
     for( int p=0; p<np; p++ )
     {
       // NOTE: npr depends on numToSend : this is how many we rec. in assign
       if( numToSend(p)>0    ){ npr++; } // NOTE: npr
       if( numToReceive(p)>0 ){ nps++; } // NOTE: nps 
     }
     if( debug & 2 )
     {
       fprintf(debugFile," *** npr=%i, nps=%i\n",npr,nps);
       fflush(debugFile);
     }
     
     if( false && npr==0 && nps==0 ) // is this ok to do?
     {
       // no communication required
       delete [] pNumToReceive;
       delete [] pNumToSend;
       return 0;
     }


     // p=pr(pp) : pp=0,..,npr-1 : processors to receive data from 
     // p=ps(pp) : pp=0,..,nps-1 : processors to send data to 
     ppr = new int [npr]; 
#define pr(p) ppr[p]
     pps = new int [nps]; 
#define ps(p) pps[p]
     // ppr(pp) = p if pr(p)=pp (i.e. ppr is the "inverse" of pr)
     int *pppr = new int [np];
     #define ppr(p) pppr[p]

     int kr=0, ks=0;
     for( int p=0; p<np; p++ )
     {
       ppr(p)=-1;
       // --- NOTE: reassign numToReceive and numToSend to valid proc. ---
       if( numToSend(p)>0    ){ pr(kr)=p; numToSend(kr)=numToSend(p); ppr(p)=kr; kr++; }
       if( numToReceive(p)>0 ){ ps(ks)=p; numToReceive(ks)=numToReceive(p);      ks++; }
     }
  
     assert( kr==npr && ks==nps );

     // --- allocate send and receive buffers ---
     int **psbuff = new int* [npr];  // note: npr
     #define sendBuff(i,p) psbuff[p][i]
     for( int p=0; p<npr; p++ )
       psbuff[p] = new int[ numToSend(p) ]; // can we allocate zero items ?

     int **prbuff = new int* [nps];
     #define receiveBuff(i,p) prbuff[p][i]
     for( int p=0; p<nps; p++ )
       prbuff[p] = new int[ max(1,numToReceive(p)) ]; 
     
     MPI_Request *sendRequest    = new MPI_Request[npr];   
     MPI_Status *sendStatus      = new MPI_Status [npr];
     MPI_Request *receiveRequest = new MPI_Request[nps];
     MPI_Status *receiveStatus   = new MPI_Status [nps];


     // --- post receives first ---
     const int tag1=161823; // make a unique tag
     for( int p=0; p<nps; p++ )
     {
       const int pp=ps(p); // pp is in [0,np-1] 
       int tag=tag1+myid;
       assert( numToReceive(p)>0 );
       if( debug & 2 )
	 fprintf(debugFile," *** post receive for %i values from p=%i\n",numToReceive(p),pp);
       MPI_Irecv(prbuff[p],numToReceive(p),MPI_INT ,pp,tag,AIN_COMM,&receiveRequest[p] ); // rec. from prc. pp
     }
  
     // --- fill in the send buffers ---
     // nar[p] = number of pts that will be received from proc. p in the ASSIGN stage
     // IAR(i,axis,p) = (i1,i2,i3) = index of the pt that will be received from proc. p
     #define IAR(i,axis,p) iar[p][(i)+nar[p]*(axis)]

     assert( nar==NULL && iar==NULL );
     nar = new int [npr];
     iar = new int* [npr]; for( int p=0; p<npr; p++ ){ iar[p]=NULL; } // 

     assert( nar!=NULL );
     for( int p=0; p<npr; p++ )
     {
       nar[p]=numToSend(p)/numToSendPerProc;  // this is how many pts we will rec. in the assign stage
       assert( iar[p]==NULL );
       if( nar[p]>0 )
	 iar[p] = new int [nar[p]*numberOfDimensions];
       else
	 iar[p]=NULL;
     }
     
     // num(p) : holds count of number of values that will be sent (setup) or rec'd (assign) from proc. p
     int *pnum = new int[npr];               
     #define num(p) pnum[p]
     for( int p=0; p<npr; p++ )
       num(p)=0;
     for( int i=0; i<numberOfInterpolationNeighbours; i++ )
     {
       if( status(i)>=10 )
       {
	 // proc. pp will eval this pt: 
         const int pp = status(i)-10;  // pp in in the range [0,np-1]
         const int p = ppr(pp);        // p should be in the range [0,npr-1]
         assert( p>=0 && p<npr );
	 
	 int & k = num(p);  // *note reference*
	 const int jr = k/numToSendPerProc;
	 for( int axis=0; axis<numberOfDimensions; axis++ )
 	 {
	   sendBuff(k,p)=IA(i,axis); k++;
           IAR(jr,axis,p)=IA(i,axis);         // save this value 
 	 }
	 for( int axis=0; axis<numberOfDimensions; axis++ )
 	 {
	   sendBuff(k,p)=ID(i,axis); k++;
 	 }
       }
     }

     for( int p=0; p<npr; p++ )
     {
       // consistency check: 
       assert( num(p)==numToSend(p) );
     }
     

     // Send data
     // ***** send all info ****
     for( int p=0; p<npr;  p++ )
     {
       const int pp=pr(p); // pp is in [0,np-1] 
       int tag=tag1+pp;
       assert( numToSend(p)>0 );
       if( debug & 2 )
	 fprintf(debugFile," *** send %i values to p=%i\n",numToSend(p),pp);

       MPI_Isend(psbuff[p],numToSend(p),MPI_INT ,pp,tag,AIN_COMM,&sendRequest[p] ); // send to proc. pp 
     }

     
     // -- fill in the ia,id arrays with valid pts, compress out unused pts  ---
     int na=0;  // counts actual num to assign
     for( int i=0; i<numberOfInterpolationNeighbours; i++ )
     {
       //   status(i)= 0 : means we can evaluate the pt on this processor: 
       //            = 2 : this point cannot extrap on this processor
       //            =-1 : this point is not needed

       if( status(i)==0 )
       {
	 if( i!=na )
	 {
	   for( int axis=0; axis<numberOfDimensions; axis++ )
	   {
             IA(na,axis)=IA(i,axis);
	     ID(na,axis)=ID(i,axis);
	   }
	 }
	 na++;
       }
     }
	     
     if( nps>0 )
       MPI_Waitall( nps, receiveRequest, receiveStatus );  // wait to receive all messages
 
     // nas[p] = number of pts that will be sent to proc. p
     // IAS(i,axis,p) = (i1,i2,i3) = index of the pt that will be sent to proc. p
     #define IAS(i,axis,p) ias[p][(i)+nas[p]*(axis)]

     assert( nas==NULL && ias==NULL );
     nas = new int [nps];
     ias = new int* [nps]; for( int p=0; p<nps; p++ ) ias[p]=NULL;
     assert( nas!=NULL );

     // Receive data
     int iav[3]={0,0,0}, idv[3]={0,0,0};
     for( int p=0; p<nps; p++ )
     {
       const int pp=ps(p); // pp is in [0,np-1] 

       int num=-1;
       MPI_Get_count( &receiveStatus[p], MPI_INT, &num );
       assert( num==numToReceive(p) );
       
       int k=0;
       const int numPts = numToReceive(p)/numToSendPerProc;
       assert( na+numPts <= ndin );
       assert( ias[p]==NULL );
       if( numPts>0 )
         ias[p] = new int [numPts*numberOfDimensions];
       else
	 ias[p]=NULL;
       nas[p]=numPts;
       int jp = 0;
       for( int j=0; j<numPts; j++ )
       {
         for( int axis=0; axis<numberOfDimensions; axis++ )
	 {
	   iav[axis] = receiveBuff(k,p); k++;
           IA(na,axis) =iav[axis];  // save this pt in the main list so it will be assigned
           IAS(jp,axis,p)=iav[axis];  // save this index so we know which pts go to proc. p
	 }
         for( int axis=0; axis<numberOfDimensions; axis++ )
	 {
	   idv[axis] = receiveBuff(k,p); k++;
           ID(na,axis) = idv[axis];
	 }
         #ifdef AIN_DEBUG
         if( debug & 2 )
	   fprintf(debugFile," myid=%i : evaluate point ia=(%i,%i) id=(%i,%i) for proc p=%i\n",
                   myid,iav[0],iav[1],idv[0],idv[1],pp);
         #endif
         #ifdef AIN_DEBUG
          // --- check that the point ia,id defines a valid extrapolation stencil on this proc. ----
          int w=maximumWidthToExtrapolationInterpolationNeighbours-1;
  	  int i1=iav[0], i2=iav[1], i3=iav[2];                 // first pt in the stencil 
          int j1=i1+idv[0]*w, j2=i2+idv[1]*w, j3=i3+idv[2]*w;  // final pt in the stencil
	  if( i1<mask.getBase(0) || i1>mask.getBound(0) ||
              i2<mask.getBase(1) || i2>mask.getBound(1) ||
              i3<mask.getBase(2) || i3>mask.getBound(2) ||
              j1<mask.getBase(0) || j1>mask.getBound(0) ||
              j2<mask.getBase(1) || j2>mask.getBound(1) ||
              j3<mask.getBase(2) || j3>mask.getBound(2) )
	  {
	    printf("AIN:ERROR: stencil pt i=(%i,%i,%i) to j=(%i,%i,%i) (from p=%i) cannot be evaluated"
                   " on myid=%i : stencil outside index bounds\n",i1,i2,i3,j1,j2,j3,pp,myid);
	    
	    OV_ABORT("ERROR");
	  }
          if( mask(i1,i2,i3)!=0 )
	  {
	    printf("AIN:ERROR: stencil pt i=(%i,%i,%i) j=(%i,%i,%i) (from p=%i) cannot be evaluated"
                   " on myid=%i : mask(i)!=0 \n",i1,i2,i3,j1,j2,j3,pp,myid);
	    displayMask(mask,"mask",debugFile);
	    OV_ABORT("ERROR");
	  }
	  
          // all pts in the stencil except the first should have mask!=0 
          for( int w=1; w<maximumWidthToExtrapolationInterpolationNeighbours; w++ )
	  {
            j1=i1+idv[0]*w, j2=i2+idv[1]*w, j3=i3+idv[2]*w;
            if( mask(j1,j2,j3)==0 )
	    {
	      printf("AIN:ERROR: stencil pt i=(%i,%i,%i), j=(%i,%i,%i) (from p=%i) cannot be evaluated"
		     " on myid=%i : mask(j)==0 \n",i1,i2,i3,j1,j2,j3,pp,myid);
	      fprintf(debugFile,"AIN:ERROR: stencil pt i=(%i,%i,%i), j=(%i,%i,%i) (from p=%i) cannot be evaluated"
		     " on myid=%i : mask(j)==0 \n",i1,i2,i3,j1,j2,j3,pp,myid);
	      displayMask(mask,"mask",debugFile);
	      fflush(debugFile);
	      fclose(debugFile);
	      OV_ABORT("ERROR");
	    }
	  }
	  
         #endif

	 na++;
         jp++;
       }
     }
     numberOfInterpolationNeighbours=na; // Here is the new number 
     assert( numberOfInterpolationNeighbours<= ndin );
     
     if( debug & 2 )
     {
       fflush(debugFile);
     }
     

     // NOTE: when we interpolate we can fill in the extra values directly into uLocal,
     //       then as a separate step send the values to the approp. proc.


     // we must wait for the send's to complete before deleting the buffers
     if( npr>0 )
       MPI_Waitall( npr, sendRequest, sendStatus ); 

     delete [] pNumToReceive;
     delete [] pNumToSend;

     for( int p=0; p<npr; p++ )
       delete [] psbuff[p];
     for( int p=0; p<nps; p++ )
       delete [] prbuff[p];

     delete [] psbuff;
     delete [] prbuff;
     delete [] pnum;
     delete [] pppr;
     
     delete [] sendRequest;
     delete [] sendStatus;
     delete [] receiveRequest;
     delete [] receiveStatus;


#endif /* end P++ */ 

     if( numberOfInterpolationNeighbours>0 && numberOfInterpolationNeighbours!=ndin )
     {
       ia.resize(numberOfInterpolationNeighbours,numberOfDimensions);
       id.resize(numberOfInterpolationNeighbours,numberOfDimensions);
       // ia.display("ia *new*");
       // id.display("id *new* ");
     }
     else
     {
       ia.redim(0);
       id.redim(0);
     }
     
     if( false )
     {
       printf(" findInterpolationNeighbours:opt: ni=%i, numberOfInterpolationNeighbours=%i\n",ni,
	      numberOfInterpolationNeighbours);
       
       display(ip,"ip - interpolation points","%3i ");
       display(ia,"ia - interpolation neighb","%3i ");
       display(id,"id - interpolation direct","%3i ");
       
     }
     

  }

  return 0;
  
}   

// =============================================================================
/// \brief Assign values to the unused points next to interpolation points
// =============================================================================
int AssignInterpNeighbours::
assign( realMappedGridFunction & uA, Range & C, const BoundaryConditionParameters & bcParameters )
{

  // Assign (extrapolate) the unused points that lie next to interpolation points
  // 
  //             e e e e
  //           e e I I I       e=extrapolate
  //           e I I X X       I= interpolation pt
  //           e I X X X       X= discretaization pt
  //           e I X X X
  real time1=getCPU();

  MappedGrid & mg = *uA.getMappedGrid();

  const int numberOfDimensions = mg.numberOfDimensions();
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  if( !isInitialized )
  {
    findInterpolationNeighbours(mg);

    if( errorStatus==errorInFindInterpolationNeighbours )
    {
      printF("AssignInterpNeighbours::assign:ERROR: error return from findInterpolationNeighbours\n");
      return 1;
    }
      
  }

  const int c0=C.getBase(), c1=C.getBound();
  const int numComponents = c1-c0+1;

  if( debug & 2 )
    fprintf(debugFile,"\n --- START assign points c0=%i c1=%i ----\n",c0,c1);

    
  #ifdef USE_PPP
    realSerialArray uLocal; ::getLocalArrayWithGhostBoundaries(uA,uLocal);
  #else
    const realSerialArray & uLocal = uA;
  #endif

    
  if( numberOfInterpolationNeighbours>0 )
  {
    // --- Assign any points ---

    const IntegerArray & ia = *extrapolateInterpolationNeighbourPoints;
    const IntegerArray & id = *extrapolateInterpolationNeighboursDirection;
    const int *pia=getDataPointer(ia);
    bool useVariableExtrapolationWidth=extrapolateInterpolationNeighboursVariableWidth!=NULL;
    const int *pvew = useVariableExtrapolationWidth ? 
      getDataPointer(*extrapolateInterpolationNeighboursVariableWidth) : pia;
	
    int extrapOrder=bcParameters.orderOfExtrapolation;
    if( extrapOrder > maximumWidthToExtrapolationInterpolationNeighbours-1 )
    {
      extrapOrder=maximumWidthToExtrapolationInterpolationNeighbours-1;
      if( numberOfReduceOrderOfExtrapMessages<=10 )
      {
	numberOfReduceOrderOfExtrapMessages++;
	printF("AssignInterpNeighbours:INFO: reducing order of extrapolation to %i from requested order=%i. "
	       "(since maximumWidthToExtrapolationInterpolationNeighbours=%i)\n",extrapOrder,
               bcParameters.orderOfExtrapolation,maximumWidthToExtrapolationInterpolationNeighbours);
	if( numberOfReduceOrderOfExtrapMessages==10 )
          printF("Too many reducing order of extrapolation info messages. I will not print anymore.\n");
      }
      
    }
    int ipar[]={maximumWidthToExtrapolationInterpolationNeighbours,
		extrapOrder, 
		(int)bcParameters.extrapolationOption,
		(int)useVariableExtrapolationWidth
    };//
    const real uEps=1000.*REAL_MIN; // for limited extrapolation
    real rpar[]={bcParameters.extrapolateWithLimiterParameters[0],
		 bcParameters.extrapolateWithLimiterParameters[1],
		 uEps}; //

    extrapInterpNeighboursOpt(mg.numberOfDimensions(), 
			      uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			      uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			      ia.getBase(0),ia.getBound(0),id.getBase(0),id.getBound(0),
			      *pia,*getDataPointer(id),*pvew, *getDataPointer(uLocal),
			      C.getBase(),C.getBound(),ipar[0],rpar[0] );

    if( debug & 4 )
    {
      int *iap = ia.Array_Descriptor.Array_View_Pointer1;
      const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]

      int i1,i2,i3=0;
      for( int i=0; i<numberOfInterpolationNeighbours; i++ )
      {
	i1=IA(i,0); i2=IA(i,1);
	if( numberOfDimensions==3 ) i3=IA(i,2);
	for( int c=c0; c<=c1; c++ )
	  fprintf(debugFile," assign u(%i,%i,%i,%i)=%9.3e on this proc.\n",i1,i2,i3,c,uLocal(i1,i2,i3,c));
      }
    }
    

  }


  #ifdef USE_PPP

   // *********************************************
   // ************ Parallel ***********************
   // *********************************************

  if( npr==0 && nps ==0 )
  { // No commuication required on this proc. 
    if( debug & 2 )
    {
      fprintf(debugFile," DONE: no communication required on this proc.\n");
    }
    return 0; 
  }
  

  assert( nar!=NULL );
  assert( iar!=NULL );

  // npr = number of processors that we will receive data from
  // nps = number of processors that we will send data to 
  // p=pr(pp) : pp=0,..,npr-1 : processors to receive data from 
  // p=ps(pp) : pp=0,..,nps-1 : processors to send data to 


  // --- post receives ---
  real **prbuff = new real* [npr];  // buffer for rec.
  #define rbuff(i,p) prbuff[p][i]
  int *pnumr = new int [npr];     
  #define numr(p) pnumr[p]
  for( int p=0; p<npr; p++ )
  {
    const int pp=pr(p); // pp is in [0,np-1] 
    numr(p) = nar[p]*numComponents;
    prbuff[p] = new real [max(1,numr(p))];
  }
  
  MPI_Request *sendRequest   = new MPI_Request[nps];  
  MPI_Status *sendStatus     = new MPI_Status [nps];
  MPI_Request *receiveRequest= new MPI_Request[npr];
  MPI_Status *receiveStatus  = new MPI_Status [npr];

  const int tag1=83236; // make a unique tag
  for( int p=0; p<npr; p++ )
  {
    const int pp=pr(p); // pp is in [0,np-1] 
    int tag=tag1+myid;
    MPI_Irecv(prbuff[p],numr(p),MPI_Real ,pp,tag,AIN_COMM,&receiveRequest[p] ); // rec. from proc=pp
  }


  // -- collect the values that will be send to other processors ---
  real **psbuff = new real* [nps];  // buffer for send
  #define sbuff(i,p) psbuff[p][i]

  int i1,i2,i3=0;
  int * pnums = new int [nps];   
  #define nums(p) pnums[p]
  for( int p=0; p<nps; p++ )
  {
    const int pp=ps(p);  // pp is in [0,np-1] 
    nums(p)=nas[p]*numComponents;
    psbuff[p] = new real [nums(p)];
      
    int k = 0;
    for( int i=0; i<nas[p]; i++ )
    {
      i1=IAS(i,0,p); i2=IAS(i,1,p); 
      if( numberOfDimensions==3 ) i3=IAS(i,2,p);
      
      for( int c=c0; c<=c1; c++ )
      {
	sbuff(k,p) = uLocal(i1,i2,i3,c); k++;
	if( debug & 2 )
	  fprintf(debugFile," Send u(%i,%i,%i,%i)=%9.3e to p=%i\n",i1,i2,i3,c,uLocal(i1,i2,i3,c),pp);
      }
    }
    assert( k==nums(p) );
  }
  // Send data
  for( int p=0; p<nps; p++ )
  {
    const int pp=ps(p);  // pp is in [0,np-1] 
    int tag=tag1+pp;
    // if( nums(p)>0 )
    MPI_Isend(psbuff[p],nums(p),MPI_Real ,pp,tag,AIN_COMM,&sendRequest[p] ); // send to proc. pp
    if( debug & 4 )
    {
      fprintf(debugFile," Send %i reals to p=%i : ",nums(p),pp);
      for( int i=0; i<nums(p); i++ )
	fprintf(debugFile,"%5.2f ",psbuff[p][i]);
      fprintf(debugFile,"\n");
    }
    
  }

  if( npr>0 )
    MPI_Waitall( npr, receiveRequest, receiveStatus );  // wait to receive all messages

  // Receive data
  for( int p=0; p<npr; p++ )
  {
    const int pp=pr(p); // pp is in [0,np-1] 
    assert( numr(p)>0 );
    int num=-1;
    MPI_Get_count( &receiveStatus[p], MPI_Real, &num );
    assert( num==numr(p) );
    if( debug & 4 )
    {
      fprintf(debugFile," Rec. %i reals from  p=%i : ",numr(p),pp);
      for( int i=0; i<numr(p); i++ )
	fprintf(debugFile,"%5.2f ",prbuff[p][i]);
      fprintf(debugFile,"\n");
    }
       
    int k = 0;
    for( int i=0; i<nar[p]; i++ )
    {
      i1=IAR(i,0,p); i2=IAR(i,1,p);          
      if( numberOfDimensions==3 ) i3=IAR(i,2,p);
      
      for( int c=c0; c<=c1; c++ )
      {
	uLocal(i1,i2,i3,c) = rbuff(k,p); k++;
        #ifdef AIN_DEBUG
	if( debug & 2 )
	  fprintf(debugFile," set u(%i,%i,%i,%i) = %9.3e (from p=%i)\n",i1,i2,i3,c,uLocal(i1,i2,i3,c),pp);
        #endif
      }
    }
    assert( k==numr(p) );
  }
  
  // we must wait for the send's to complete before deleting the buffers
  if( nps>0 )
    MPI_Waitall( nps, sendRequest, sendStatus ); 

  for( int p=0; p<nps; p++ )
    delete [] psbuff[p];
  for( int p=0; p<npr; p++ )
    delete [] prbuff[p];

  delete [] psbuff;
  delete [] prbuff;

  delete [] pnumr;   
  delete [] pnums;   

  delete [] sendRequest;
  delete [] sendStatus;
  delete [] receiveRequest;
  delete [] receiveStatus;

  if( debug & 2 )
    fprintf(debugFile,"\n --- DONE assign points c0=%i c1=%i ----\n",c0,c1);


 #endif /* PPP */ 



  return 0;
}


