// This file automatically generated from ParallelUtility.bC with bpp.
#include "ParallelUtility.h"

#ifndef OV_USE_DOUBLE
#define MPI_Real MPI_FLOAT
#else
#define MPI_Real MPI_DOUBLE
#endif

// **********************************************************
// *************** defineCopyMacro **************************
// **********************************************************

// defineCopyMacro(int,intArray,iDataMove);
int ParallelUtility::
copy( intArray & dest, Index *D, 
            const intArray & src, Index *S, int nd )
// ===================================================================================
//   Make the copy:
//            des(D[0],D[1], ... D[nd-1]) = src( S[0],...,S[nd-1] ) 
// /dest (input/output) : destination array
// /D[] (input) : destination Index values (un-specified dimensions of dest will default to "all")
// /src (input) : source array
// /S[] (input) : source Index values (un-specified dimensions of src will default to "all")
// /nd (input) : number of Index's supplied
// ===================================================================================
{
#ifdef USE_PPP
    CopyArray::copyArray(dest,D,src,S,nd);  // use Bill's version
#endif
#if 0 
    const int destNumberOfDimensions=dest.numberOfDimensions();
    const int srcNumberOfDimensions=src.numberOfDimensions();
    const int numDims=max(destNumberOfDimensions,srcNumberOfDimensions);
    int *srcDims = new int [numDims]; //
    int *srcLos = new int [numDims]; //
    int *srcHis = new int [numDims]; //
    int *srcStrides = new int [numDims]; //
    int *destDims = new int [numDims]; //
    int *destLos = new int [numDims]; //
    int *destHis = new int [numDims]; //
    int *destStrides = new int [numDims]; //
    for( int axis=0; axis<numDims; axis++ )
    {
        if( axis<srcNumberOfDimensions )
            srcDims[axis]=axis;    
        else
            srcDims[axis]=-1;  // set non-existing dimensions to -1 (Los,His,.. not used in this case)
        if( axis<nd && S[axis].getLength()>0 )
        {
            srcLos[axis]=S[axis].getBase() -src.getBase(axis); // base 0 for PARTI
            srcHis[axis]=S[axis].getBound()-src.getBase(axis);
            srcStrides[axis]=S[axis].getStride();
            if( S[axis].getBase()<src.getBase(axis) || S[axis].getBound()>src.getBound(axis) )
            {
      	printf(" ParallelUtility::copy:ERROR Source index values are out of bounds!\n"
                              "   axis=%i S=[%i,%i] but src [base,bound]=[%i,%i]\n",
             	       axis,S[axis].getBase(),S[axis].getBound(),src.getBase(axis),src.getBound(axis));
      	Overture::abort("error");
            }
        }
        else
        { // use full range of values for unspecified dimensions
            srcLos[axis]=0; 
            srcHis[axis]=src.getBound(axis)-src.getBase(axis);
            srcStrides[axis]=src.getStride(axis);
        }
        if( axis<destNumberOfDimensions )
            destDims[axis]=axis;
        else
            destDims[axis]=-1;
        if( axis<nd && D[axis].getLength()>0 )
        {
            destLos[axis]=D[axis].getBase() -dest.getBase(axis);
            destHis[axis]=D[axis].getBound()-dest.getBase(axis);
            destStrides[axis]=D[axis].getStride();
            if( D[axis].getBase()<dest.getBase(axis) || D[axis].getBound()>dest.getBound(axis) )
            {
      	printf(" ParallelUtility::copy:ERROR Destination index values are out of bounds!\n"
                              "   axis=%i D=[%i,%i] but dest [base,bound]=[%i,%i]\n",
             	       axis,D[axis].getBase(),D[axis].getBound(),dest.getBase(axis),dest.getBound(axis));
      	Overture::abort("error");
            }
        }
        else
        { // use full range of values for unspecified dimensions
            destLos[axis]=0; 
            destHis[axis]=dest.getBound(axis)-dest.getBase(axis);
            destStrides[axis]=dest.getStride(axis);
        }
        assert( srcStrides[axis]>0 && destStrides[axis]>0 );
        if( (srcHis[axis]-srcLos[axis])/srcStrides[axis] != (destHis[axis]-destLos[axis])/destStrides[axis] )
        {
            printf(" ParallelUtility::copy:ERROR non-conformable operation!\n"
           	     "   axis=%i src=[%i,%i,%i] with count=%i, but dest=[%i,%i,%i] with count=%i\n",
           	     axis,
                          srcLos[axis],srcHis[axis],srcStrides[axis],(srcHis[axis]-srcLos[axis])/srcStrides[axis]+1,
                          destLos[axis],destHis[axis],destStrides[axis],(destHis[axis]-destLos[axis])/destStrides[axis]+1);
            Overture::abort("error");
        }
    }
#ifndef USE_PADRE
    DARRAY *srcDArray = src.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
    DARRAY *destDArray = dest.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
#else
  // Padre version:
    DARRAY *srcDArray = src.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
                                              pPARTI_Representation->BlockPartiArrayDescriptor; 
    DARRAY *destDArray = dest.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
                                              pPARTI_Representation->BlockPartiArrayDescriptor; 
#endif
  // build the communication schedule
    SCHED *sched = subArraySched(srcDArray,destDArray,numDims,
                                                              srcDims,srcLos,srcHis,srcStrides,
                                                              destDims,destLos,destHis,destStrides );
// These seem to be all the same: what about views??
//    printf(" src: getDataPointer                                     =%i \n"
//           "      getLocalArrayWithGhostBoundaries().getDataPointer()=%i \n"
//           "      Array_Descriptor.SerialArray.getDataPointer()      =%i \n",
//  	 src.getDataPointer(),psrc,src.Array_Descriptor.SerialArray->getDataPointer());
//    int* psrc = src.getLocalArrayWithGhostBoundaries().getDataPointer();
//    int* pdest= dest.getLocalArrayWithGhostBoundaries().getDataPointer();
    int* psrc = src.getDataPointer();
    int* pdest= dest.getDataPointer();
  //   int dummy=0;
  //   if( psrc==0 ) psrc=&dummy;  // *wdh* 070516 -- try this for testing
  //   if( pdest==0 ) pdest=&dummy;
    iDataMove(psrc,sched,pdest);
  // MPI_Barrier(Overture::OV_COMM); // ***************** add this for testing bug with strided data **************
  // clean up 
    delete [] srcDims;
    delete [] srcLos;
    delete [] srcHis;
    delete [] srcStrides;
    delete [] destDims;
    delete [] destLos;
    delete [] destHis;
    delete [] destStrides;
    if( sched!=NULL )   // *wdh* 060503 -- a sched is NULL is there is nothing to do on this processor ?
        delete_SCHED( sched );
#endif
#ifndef USE_PPP
  // **** Here is the serial version  ****
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

// defineCopyMacro(float,floatArray,fDataMove);
int ParallelUtility::
copy( floatArray & dest, Index *D, 
            const floatArray & src, Index *S, int nd )
// ===================================================================================
//   Make the copy:
//            des(D[0],D[1], ... D[nd-1]) = src( S[0],...,S[nd-1] ) 
// /dest (input/output) : destination array
// /D[] (input) : destination Index values (un-specified dimensions of dest will default to "all")
// /src (input) : source array
// /S[] (input) : source Index values (un-specified dimensions of src will default to "all")
// /nd (input) : number of Index's supplied
// ===================================================================================
{
#ifdef USE_PPP
    CopyArray::copyArray(dest,D,src,S,nd);  // use Bill's version
#endif
#if 0 
    const int destNumberOfDimensions=dest.numberOfDimensions();
    const int srcNumberOfDimensions=src.numberOfDimensions();
    const int numDims=max(destNumberOfDimensions,srcNumberOfDimensions);
    int *srcDims = new int [numDims]; //
    int *srcLos = new int [numDims]; //
    int *srcHis = new int [numDims]; //
    int *srcStrides = new int [numDims]; //
    int *destDims = new int [numDims]; //
    int *destLos = new int [numDims]; //
    int *destHis = new int [numDims]; //
    int *destStrides = new int [numDims]; //
    for( int axis=0; axis<numDims; axis++ )
    {
        if( axis<srcNumberOfDimensions )
            srcDims[axis]=axis;    
        else
            srcDims[axis]=-1;  // set non-existing dimensions to -1 (Los,His,.. not used in this case)
        if( axis<nd && S[axis].getLength()>0 )
        {
            srcLos[axis]=S[axis].getBase() -src.getBase(axis); // base 0 for PARTI
            srcHis[axis]=S[axis].getBound()-src.getBase(axis);
            srcStrides[axis]=S[axis].getStride();
            if( S[axis].getBase()<src.getBase(axis) || S[axis].getBound()>src.getBound(axis) )
            {
      	printf(" ParallelUtility::copy:ERROR Source index values are out of bounds!\n"
                              "   axis=%i S=[%i,%i] but src [base,bound]=[%i,%i]\n",
             	       axis,S[axis].getBase(),S[axis].getBound(),src.getBase(axis),src.getBound(axis));
      	Overture::abort("error");
            }
        }
        else
        { // use full range of values for unspecified dimensions
            srcLos[axis]=0; 
            srcHis[axis]=src.getBound(axis)-src.getBase(axis);
            srcStrides[axis]=src.getStride(axis);
        }
        if( axis<destNumberOfDimensions )
            destDims[axis]=axis;
        else
            destDims[axis]=-1;
        if( axis<nd && D[axis].getLength()>0 )
        {
            destLos[axis]=D[axis].getBase() -dest.getBase(axis);
            destHis[axis]=D[axis].getBound()-dest.getBase(axis);
            destStrides[axis]=D[axis].getStride();
            if( D[axis].getBase()<dest.getBase(axis) || D[axis].getBound()>dest.getBound(axis) )
            {
      	printf(" ParallelUtility::copy:ERROR Destination index values are out of bounds!\n"
                              "   axis=%i D=[%i,%i] but dest [base,bound]=[%i,%i]\n",
             	       axis,D[axis].getBase(),D[axis].getBound(),dest.getBase(axis),dest.getBound(axis));
      	Overture::abort("error");
            }
        }
        else
        { // use full range of values for unspecified dimensions
            destLos[axis]=0; 
            destHis[axis]=dest.getBound(axis)-dest.getBase(axis);
            destStrides[axis]=dest.getStride(axis);
        }
        assert( srcStrides[axis]>0 && destStrides[axis]>0 );
        if( (srcHis[axis]-srcLos[axis])/srcStrides[axis] != (destHis[axis]-destLos[axis])/destStrides[axis] )
        {
            printf(" ParallelUtility::copy:ERROR non-conformable operation!\n"
           	     "   axis=%i src=[%i,%i,%i] with count=%i, but dest=[%i,%i,%i] with count=%i\n",
           	     axis,
                          srcLos[axis],srcHis[axis],srcStrides[axis],(srcHis[axis]-srcLos[axis])/srcStrides[axis]+1,
                          destLos[axis],destHis[axis],destStrides[axis],(destHis[axis]-destLos[axis])/destStrides[axis]+1);
            Overture::abort("error");
        }
    }
#ifndef USE_PADRE
    DARRAY *srcDArray = src.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
    DARRAY *destDArray = dest.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
#else
  // Padre version:
    DARRAY *srcDArray = src.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
                                              pPARTI_Representation->BlockPartiArrayDescriptor; 
    DARRAY *destDArray = dest.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
                                              pPARTI_Representation->BlockPartiArrayDescriptor; 
#endif
  // build the communication schedule
    SCHED *sched = subArraySched(srcDArray,destDArray,numDims,
                                                              srcDims,srcLos,srcHis,srcStrides,
                                                              destDims,destLos,destHis,destStrides );
// These seem to be all the same: what about views??
//    printf(" src: getDataPointer                                     =%i \n"
//           "      getLocalArrayWithGhostBoundaries().getDataPointer()=%i \n"
//           "      Array_Descriptor.SerialArray.getDataPointer()      =%i \n",
//  	 src.getDataPointer(),psrc,src.Array_Descriptor.SerialArray->getDataPointer());
//    float* psrc = src.getLocalArrayWithGhostBoundaries().getDataPointer();
//    float* pdest= dest.getLocalArrayWithGhostBoundaries().getDataPointer();
    float* psrc = src.getDataPointer();
    float* pdest= dest.getDataPointer();
  //   float dummy=0;
  //   if( psrc==0 ) psrc=&dummy;  // *wdh* 070516 -- try this for testing
  //   if( pdest==0 ) pdest=&dummy;
    fDataMove(psrc,sched,pdest);
  // MPI_Barrier(Overture::OV_COMM); // ***************** add this for testing bug with strided data **************
  // clean up 
    delete [] srcDims;
    delete [] srcLos;
    delete [] srcHis;
    delete [] srcStrides;
    delete [] destDims;
    delete [] destLos;
    delete [] destHis;
    delete [] destStrides;
    if( sched!=NULL )   // *wdh* 060503 -- a sched is NULL is there is nothing to do on this processor ?
        delete_SCHED( sched );
#endif
#ifndef USE_PPP
  // **** Here is the serial version  ****
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

// defineCopyMacro(double,doubleArray,dDataMove);
int ParallelUtility::
copy( doubleArray & dest, Index *D, 
            const doubleArray & src, Index *S, int nd )
// ===================================================================================
//   Make the copy:
//            des(D[0],D[1], ... D[nd-1]) = src( S[0],...,S[nd-1] ) 
// /dest (input/output) : destination array
// /D[] (input) : destination Index values (un-specified dimensions of dest will default to "all")
// /src (input) : source array
// /S[] (input) : source Index values (un-specified dimensions of src will default to "all")
// /nd (input) : number of Index's supplied
// ===================================================================================
{
#ifdef USE_PPP
    CopyArray::copyArray(dest,D,src,S,nd);  // use Bill's version
#endif
#if 0 
    const int destNumberOfDimensions=dest.numberOfDimensions();
    const int srcNumberOfDimensions=src.numberOfDimensions();
    const int numDims=max(destNumberOfDimensions,srcNumberOfDimensions);
    int *srcDims = new int [numDims]; //
    int *srcLos = new int [numDims]; //
    int *srcHis = new int [numDims]; //
    int *srcStrides = new int [numDims]; //
    int *destDims = new int [numDims]; //
    int *destLos = new int [numDims]; //
    int *destHis = new int [numDims]; //
    int *destStrides = new int [numDims]; //
    for( int axis=0; axis<numDims; axis++ )
    {
        if( axis<srcNumberOfDimensions )
            srcDims[axis]=axis;    
        else
            srcDims[axis]=-1;  // set non-existing dimensions to -1 (Los,His,.. not used in this case)
        if( axis<nd && S[axis].getLength()>0 )
        {
            srcLos[axis]=S[axis].getBase() -src.getBase(axis); // base 0 for PARTI
            srcHis[axis]=S[axis].getBound()-src.getBase(axis);
            srcStrides[axis]=S[axis].getStride();
            if( S[axis].getBase()<src.getBase(axis) || S[axis].getBound()>src.getBound(axis) )
            {
      	printf(" ParallelUtility::copy:ERROR Source index values are out of bounds!\n"
                              "   axis=%i S=[%i,%i] but src [base,bound]=[%i,%i]\n",
             	       axis,S[axis].getBase(),S[axis].getBound(),src.getBase(axis),src.getBound(axis));
      	Overture::abort("error");
            }
        }
        else
        { // use full range of values for unspecified dimensions
            srcLos[axis]=0; 
            srcHis[axis]=src.getBound(axis)-src.getBase(axis);
            srcStrides[axis]=src.getStride(axis);
        }
        if( axis<destNumberOfDimensions )
            destDims[axis]=axis;
        else
            destDims[axis]=-1;
        if( axis<nd && D[axis].getLength()>0 )
        {
            destLos[axis]=D[axis].getBase() -dest.getBase(axis);
            destHis[axis]=D[axis].getBound()-dest.getBase(axis);
            destStrides[axis]=D[axis].getStride();
            if( D[axis].getBase()<dest.getBase(axis) || D[axis].getBound()>dest.getBound(axis) )
            {
      	printf(" ParallelUtility::copy:ERROR Destination index values are out of bounds!\n"
                              "   axis=%i D=[%i,%i] but dest [base,bound]=[%i,%i]\n",
             	       axis,D[axis].getBase(),D[axis].getBound(),dest.getBase(axis),dest.getBound(axis));
      	Overture::abort("error");
            }
        }
        else
        { // use full range of values for unspecified dimensions
            destLos[axis]=0; 
            destHis[axis]=dest.getBound(axis)-dest.getBase(axis);
            destStrides[axis]=dest.getStride(axis);
        }
        assert( srcStrides[axis]>0 && destStrides[axis]>0 );
        if( (srcHis[axis]-srcLos[axis])/srcStrides[axis] != (destHis[axis]-destLos[axis])/destStrides[axis] )
        {
            printf(" ParallelUtility::copy:ERROR non-conformable operation!\n"
           	     "   axis=%i src=[%i,%i,%i] with count=%i, but dest=[%i,%i,%i] with count=%i\n",
           	     axis,
                          srcLos[axis],srcHis[axis],srcStrides[axis],(srcHis[axis]-srcLos[axis])/srcStrides[axis]+1,
                          destLos[axis],destHis[axis],destStrides[axis],(destHis[axis]-destLos[axis])/destStrides[axis]+1);
            Overture::abort("error");
        }
    }
#ifndef USE_PADRE
    DARRAY *srcDArray = src.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
    DARRAY *destDArray = dest.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
#else
  // Padre version:
    DARRAY *srcDArray = src.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
                                              pPARTI_Representation->BlockPartiArrayDescriptor; 
    DARRAY *destDArray = dest.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
                                              pPARTI_Representation->BlockPartiArrayDescriptor; 
#endif
  // build the communication schedule
    SCHED *sched = subArraySched(srcDArray,destDArray,numDims,
                                                              srcDims,srcLos,srcHis,srcStrides,
                                                              destDims,destLos,destHis,destStrides );
// These seem to be all the same: what about views??
//    printf(" src: getDataPointer                                     =%i \n"
//           "      getLocalArrayWithGhostBoundaries().getDataPointer()=%i \n"
//           "      Array_Descriptor.SerialArray.getDataPointer()      =%i \n",
//  	 src.getDataPointer(),psrc,src.Array_Descriptor.SerialArray->getDataPointer());
//    double* psrc = src.getLocalArrayWithGhostBoundaries().getDataPointer();
//    double* pdest= dest.getLocalArrayWithGhostBoundaries().getDataPointer();
    double* psrc = src.getDataPointer();
    double* pdest= dest.getDataPointer();
  //   double dummy=0;
  //   if( psrc==0 ) psrc=&dummy;  // *wdh* 070516 -- try this for testing
  //   if( pdest==0 ) pdest=&dummy;
    dDataMove(psrc,sched,pdest);
  // MPI_Barrier(Overture::OV_COMM); // ***************** add this for testing bug with strided data **************
  // clean up 
    delete [] srcDims;
    delete [] srcLos;
    delete [] srcHis;
    delete [] srcStrides;
    delete [] destDims;
    delete [] destLos;
    delete [] destHis;
    delete [] destStrides;
    if( sched!=NULL )   // *wdh* 060503 -- a sched is NULL is there is nothing to do on this processor ?
        delete_SCHED( sched );
#endif
#ifndef USE_PPP
  // **** Here is the serial version  ****
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



// ======================================================================================
//  Return the max value of a scalar over all processors in a communicator
//  /processor: return the result to this processor (-1 equals all processors)
// ======================================================================================
real ParallelUtility::
getMaxValue(real value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    real maxValue=value;
    #ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, comm);
    else
        MPI_Reduce        (&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, processor, comm);
    #endif
    return maxValue;
}

int ParallelUtility::
getMaxValue(int value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    int maxValue=value;
    #ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &maxValue, 1, MPI_INT, MPI_MAX, comm);
    else
        MPI_Reduce        (&value, &maxValue, 1, MPI_INT, MPI_MAX, processor, comm);
    #endif
    return maxValue;
}

doubleLengthInt ParallelUtility::
getMaxValue(doubleLengthInt value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    doubleLengthInt maxValue=value;
    #ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &maxValue, 1, MPI_LONG_LONG_INT, MPI_MAX, comm);
    else
        MPI_Reduce        (&value, &maxValue, 1, MPI_LONG_LONG_INT, MPI_MAX, processor, comm);
    #endif
    return maxValue;
}

real ParallelUtility::
getMinValue(real value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */ )
{
    real minValue=value;
    #ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &minValue, 1, MPI_DOUBLE, MPI_MIN, comm);
    else
        MPI_Reduce        (&value, &minValue, 1, MPI_DOUBLE, MPI_MIN, processor, comm);
    #endif
    return minValue;
}

int ParallelUtility::
getMinValue(int value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    int minValue=value;
    #ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &minValue, 1, MPI_INT, MPI_MIN, comm);
    else
        MPI_Reduce        (&value, &minValue, 1, MPI_INT, MPI_MIN, processor, comm);
    #endif
    return minValue;
}

doubleLengthInt ParallelUtility::
getMinValue(doubleLengthInt value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    doubleLengthInt minValue=value;
    #ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &minValue, 1, MPI_LONG_LONG_INT, MPI_MIN, comm);
    else
        MPI_Reduce        (&value, &minValue, 1, MPI_LONG_LONG_INT, MPI_MIN, processor, comm);
    #endif
    return minValue;
}

// ======================================================================================
//  Return the sum of a scalar over all processors in a communicator
//  /processor: return the result to this processor (-1 equals all processors)
// ======================================================================================
real ParallelUtility::
getSum(real value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    real sum=0.;
#ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &sum, 1, MPI_DOUBLE, MPI_SUM, comm);
    else
        MPI_Reduce   (&value, &sum, 1, MPI_DOUBLE, MPI_SUM, processor, comm);
#else
    sum=value;
#endif
    return sum; 
}

int ParallelUtility::
getSum(int value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    int sum=0;
#ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &sum, 1, MPI_INT, MPI_SUM, comm);
    else
        MPI_Reduce   (&value, &sum, 1, MPI_INT, MPI_SUM, processor, comm);
#else
    sum=value;
#endif
    return sum; 
}

doubleLengthInt ParallelUtility::
getSum(doubleLengthInt value, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    doubleLengthInt sum=0;
#ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(&value, &sum, 1, MPI_LONG_LONG_INT, MPI_SUM, comm);
    else
        MPI_Reduce   (&value, &sum, 1, MPI_LONG_LONG_INT, MPI_SUM, processor, comm);
#else
    sum=value;
#endif
    return sum; 
}

// ======================================================================================
//  Return the max value of all components of a vector over all processors in a communicator
//  /processor: return the result to this processor (-1 equals all processors)
// ======================================================================================
void ParallelUtility::
getMaxValues(real *value, real *maxValue, int n, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    #ifdef USE_PPP 
    real *localMaxValue=maxValue;
    if( value==maxValue )
        localMaxValue = new real [n];  // allocate a new array if the user has used the same array for value and maxValue
    if( processor==-1 )
        MPI_Allreduce(value, localMaxValue, n, MPI_DOUBLE, MPI_MAX, comm);
    else
        MPI_Reduce   (value, localMaxValue, n, MPI_DOUBLE, MPI_MAX, processor, comm);
    if( value==maxValue )
    {
      for( int i=0; i<n; i++ ) maxValue[i]=localMaxValue[i];
      delete [] localMaxValue;
    }
    
    #else
        for( int i=0; i<n; i++ ) maxValue[i]=value[i];
    #endif
}

void ParallelUtility::
getMaxValues(int *value, int *maxValue, int n, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    #ifdef USE_PPP 
    int *localMaxValue=maxValue;
    if( value==maxValue )
        localMaxValue = new int [n];  // allocate a new array if the user has used the same array for value and maxValue
    if( processor==-1 )
        MPI_Allreduce(value, localMaxValue, n, MPI_INT, MPI_MAX, comm);
    else
        MPI_Reduce   (value, localMaxValue, n, MPI_INT, MPI_MAX, processor, comm);
    if( value==maxValue )
    {
      for( int i=0; i<n; i++ ) maxValue[i]=localMaxValue[i];
      delete [] localMaxValue;
    }
    #else
        for( int i=0; i<n; i++ ) maxValue[i]=value[i];
    #endif
}

// // ======================================================================================
// /// \brief Return the max value of all components of a vector over all processors
// /// \param processor (input): return the result to this processor (-1 equals all processors)
// // ======================================================================================
// void ParallelUtility::
// getMaxValues( const std::vector<real>& a_val,
//               std::vector<real>& a_max_val,
//               int processor /* = -1 */,
//               MPI_Comm a_comm /*= MPI_COMM_WORLD */ )
// {
//    const int n( a_val.size() );
//    a_max_val.resize( n );
//    real* val = const_cast<real*>( &(a_val[0]) );
// #ifdef USE_PPP 
//    real* max_val_lcl( &(a_max_val[0]) );
//    if ( val == max_val_lcl ) {
//       max_val_lcl = new real[n];
//    }
//    if( processor==-1 )
//      MPI_Allreduce( val, max_val_lcl, n, MPI_DOUBLE, MPI_MAX, a_comm );
//    else
//      MPI_Reduce   ( val, max_val_lcl, n, MPI_DOUBLE, MPI_MAX, processor, a_comm );

//    if ( val == max_val_lcl ) {
//       std::copy( max_val_lcl, max_val_lcl + n, a_max_val.begin() );
//       delete [] max_val_lcl;
//    }
// #else
//    std::copy( a_val.begin(), a_val.end(), a_max_val.begin() );
// #endif
// }

// ======================================================================================
/// \brief Return the max value of all components of a vector over all processors
// ======================================================================================
void ParallelUtility::
getMaxValues( const std::vector<real>& a_val,
                            std::vector<real>& a_max_val,
                            MPI_Comm& a_comm )
{
      const int n( a_val.size() );
      a_max_val.resize( n );
      real* val = const_cast<real*>( &(a_val[0]) );
#ifdef USE_PPP 
      real* max_val_lcl( &(a_max_val[0]) );
      if ( val == max_val_lcl ) {
            max_val_lcl = new real[n];
      }
      MPI_Allreduce( val, max_val_lcl, n, MPI_DOUBLE, MPI_MAX, a_comm );
      if ( val == max_val_lcl ) {
            std::copy( max_val_lcl, max_val_lcl + n, a_max_val.begin() );
            delete [] max_val_lcl;
      }
#else
      std::copy( a_val.begin(), a_val.end(), a_max_val.begin() );
#endif
}


void ParallelUtility::
getMinValues(real *value, real *minValue, int n, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */ )
{
    #ifdef USE_PPP 
    real *localMinValue=minValue;
    if( value==minValue )
        localMinValue = new real [n];  // allocate a new array if the user has used the same array for value and minValue
    if( processor==-1 )
        MPI_Allreduce(value, localMinValue, n, MPI_DOUBLE, MPI_MIN, comm);
    else
        MPI_Reduce   (value, localMinValue, n, MPI_DOUBLE, MPI_MIN, processor, comm);
    if( value==minValue )
    {
      for( int i=0; i<n; i++ ) minValue[i]=localMinValue[i];
      delete [] localMinValue;
    }
    #else
        for( int i=0; i<n; i++ ) minValue[i]=value[i];
    #endif
}

void ParallelUtility::
getMinValues(int *value, int *minValue, int n, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
    #ifdef USE_PPP 
    int *localMinValue=minValue;
    if( value==minValue )
        localMinValue = new int [n];  // allocate a new array if the user has used the same array for value and minValue
    if( processor==-1 )
        MPI_Allreduce(value, localMinValue, n, MPI_INT, MPI_MIN, comm);
    else
        MPI_Reduce   (value, localMinValue, n, MPI_INT, MPI_MIN, processor, comm);
    if( value==minValue )
    {
      for( int i=0; i<n; i++ ) minValue[i]=localMinValue[i];
      delete [] localMinValue;
    }
    #else
        for( int i=0; i<n; i++ ) minValue[i]=value[i];
    #endif
}

// ======================================================================================
//  Return the sum of a scalar over all processors in a communicator
//  /processor: return the result to this processor (-1 equals all processors)
// ======================================================================================
void ParallelUtility::
getSums(real *value, real *sum, int n, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */ )
{
#ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(value, sum, n, MPI_DOUBLE, MPI_SUM, comm);
    else
        MPI_Reduce   (value, sum, n, MPI_DOUBLE, MPI_SUM, processor, comm);
#else
    for( int i=0; i<n; i++ ) sum[i]=value[i];
#endif
}

void ParallelUtility::
getSums(int *value, int *sum, int n, int processor /* = -1 */, MPI_Comm comm /*= MPI_COMM_WORLD */)
{
#ifdef USE_PPP 
    if( processor==-1 )
        MPI_Allreduce(value, sum, n, MPI_INT, MPI_SUM, comm);
    else
        MPI_Reduce   (value, sum, n, MPI_INT, MPI_SUM, processor, comm );
#else
    for( int i=0; i<n; i++ ) sum[i]=value[i];
#endif
}



void 
broadCast( int & value, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
{
#ifdef USE_PPP 
    if( Communication_Manager::Number_Of_Processors==0 )
        return;

  // *wdh* new way, 060524
    MPI_Bcast( &value, 1, MPI_INT, fromProcessor, comm); 

#endif  
}
void 
broadCast( float & value, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
{
#ifdef USE_PPP 
    if( Communication_Manager::Number_Of_Processors==0 )
        return;

  // *wdh* new way, 060524
    MPI_Bcast( &value, 1, MPI_FLOAT, fromProcessor, comm); 

#endif  
}
void 
broadCast( double & value, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
{
#ifdef USE_PPP 
    if( Communication_Manager::Number_Of_Processors==0 )
        return;

  // *wdh* new way, 060524
    MPI_Bcast( &value, 1, MPI_DOUBLE, fromProcessor, comm); 

#endif  
}
void 
broadCast( bool & value, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
{
#ifdef USE_PPP 
    if( Communication_Manager::Number_Of_Processors==0 )
        return;

  // *wdh* new way, 060524
    MPI_Bcast( &value, 1, MPI_LOGICAL, fromProcessor, comm); 

#endif  
}



// broadCastArrayMacro(intSerialArray,MPI_INT)
void
broadCast( intSerialArray & buff, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
// Broadcast a serial array to all processors in a communicator
{
#ifdef USE_PPP 
    if( Communication_Manager::Number_Of_Processors==0 )
        return;
  // *wdh* new way, 060524
  // here we assume that all arrays are already dimensioned to the correct length
    const int numValues = buff.elementCount();
    MPI_Bcast( buff.getDataPointer(), numValues, MPI_INT, fromProcessor, comm); 
#endif
}
// broadCastArrayMacro(floatSerialArray,MPI_FLOAT)
void
broadCast( floatSerialArray & buff, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
// Broadcast a serial array to all processors in a communicator
{
#ifdef USE_PPP 
    if( Communication_Manager::Number_Of_Processors==0 )
        return;
  // *wdh* new way, 060524
  // here we assume that all arrays are already dimensioned to the correct length
    const int numValues = buff.elementCount();
    MPI_Bcast( buff.getDataPointer(), numValues, MPI_FLOAT, fromProcessor, comm); 
#endif
}
// broadCastArrayMacro(doubleSerialArray,MPI_DOUBLE)
void
broadCast( doubleSerialArray & buff, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
// Broadcast a serial array to all processors in a communicator
{
#ifdef USE_PPP 
    if( Communication_Manager::Number_Of_Processors==0 )
        return;
  // *wdh* new way, 060524
  // here we assume that all arrays are already dimensioned to the correct length
    const int numValues = buff.elementCount();
    MPI_Bcast( buff.getDataPointer(), numValues, MPI_DOUBLE, fromProcessor, comm); 
#endif
}


void
broadCast( aString & string, const int & fromProcessor, MPI_Comm comm /*= MPI_COMM_WORLD */ )
// Broadcast a aString to all processors in a communicator
{
    if( Communication_Manager::Number_Of_Processors==0 )
        return;

  // broadcast the length to all processors
    int length=string.length();
    broadCast(length,fromProcessor);

  // copy the string to a serial array, broadcast, and copy back

    intSerialArray buff(length+1);                // make same size on all processors
    buff=0;
    if( Communication_Manager::localProcessNumber()==fromProcessor )
    {
        for( int i=0; i<length; i++ )
            buff(i)=string[i];
    }
    broadCast( buff, fromProcessor, comm );
    
    if( Communication_Manager::localProcessNumber()!=fromProcessor )
    {
        char *s = new char [length+1];
        for( int i=0; i<length+1; i++ )
            s[i]=(char)buff(i);

        string=s;
        delete [] s;
    }
}

doubleSerialArray &
getLocalArrayWithGhostBoundaries( const doubleArray & u, doubleSerialArray & uLocal )
// =================================================================================
// /Description:
//    Return the local array with ghost boundaries.
//
// /u (input) : get local array for this distributed array
// /uLocal (output):  local array with ghost boundaries
// /Return value : uLocal
// 
// This version elimimates the problem with too many array ID's being created.
// =================================================================================
{
#ifdef USE_PPP
    if( u.getDataPointer()==NULL ) return uLocal;

    Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Internal_Index_List;
    int i;
    for (i=0;i<MAX_ARRAY_DIMENSION;i++)
    {
        Internal_Index_List[i] = new Range(u.getLocalFullRange(i));
        APP_ASSERT (Internal_Index_List[i] != NULL);
    }

    uLocal.adopt(u.getDataPointer(),Internal_Index_List);
    for (i=0; i < MAX_ARRAY_DIMENSION; i++) 
    {
        delete Internal_Index_List[i];
    }
#else
    uLocal.reference(u);
#endif

    return uLocal;
}

floatSerialArray &
getLocalArrayWithGhostBoundaries( const floatArray & u, floatSerialArray & uLocal )
// =================================================================================
// /Description:
//    Return the local array with ghost boundaries.
//
// /u (input) : get local array for this distributed array
// /uLocal (output):  local array with ghost boundaries
// /Return value : uLocal
// 
// This version elimimates the problem with too many array ID's being created.
// =================================================================================
{
#ifdef USE_PPP
    if( u.getDataPointer()==NULL ) return uLocal;

    Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Internal_Index_List;
    int i;
    for (i=0;i<MAX_ARRAY_DIMENSION;i++)
    {
        Internal_Index_List[i] = new Range(u.getLocalFullRange(i));
        APP_ASSERT (Internal_Index_List[i] != NULL);
    }

    uLocal.adopt(u.getDataPointer(),Internal_Index_List);
    for (i=0; i < MAX_ARRAY_DIMENSION; i++) 
    {
        delete Internal_Index_List[i];
    }
#else
    uLocal.reference(u);
#endif

    return uLocal;
}


intSerialArray &
getLocalArrayWithGhostBoundaries( const intArray & u, intSerialArray & uLocal )
// =================================================================================
// /Description:
//    Return the local array with ghost boundaries.
//
// /u (input) : get local array for this distributed array
// /uLocal (output):  local array with ghost boundaries
// /Return value : uLocal
// 
// This version elimimates the problem with too many array ID's being created.
// =================================================================================
{
#ifdef USE_PPP
    if( u.getDataPointer()==NULL ) return uLocal;

    Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Internal_Index_List;
    int i;
    for (i=0;i<MAX_ARRAY_DIMENSION;i++)
    {
        Internal_Index_List[i] = new Range(u.getLocalFullRange(i));
        APP_ASSERT (Internal_Index_List[i] != NULL);
    }

    uLocal.adopt(u.getDataPointer(),Internal_Index_List);
    for (i=0; i < MAX_ARRAY_DIMENSION; i++) 
    {
        delete Internal_Index_List[i];
    }
#else
    uLocal.reference(u);
#endif

    return uLocal;
}


int ParallelUtility::
broadCastArgs(int & argc, char **&argv, int sourceProcessor /* = 0 */, MPI_Comm comm /*= MPI_COMM_WORLD */ )
// ====================================================================
// /Description:
//   Broadcast the command line args to all processors in a communicator. NOTE that argc 
//   will be changed on ALL processors to only include the first set of
//   non NULL argv[i] from the sourceProcessor.
//
// /argc,argv (input/output) : input on sourceProcessor. Output on all processors.
// 
// /Note: call broadCastArgsCleanup to clean up after use of the array argv.
// ===================================================================
{
    #ifndef USE_PPP
        return 0;
    #endif
    const int myid=max(0,Communication_Manager::My_Process_Number);
    int numArgs=0;  // count the initial number of non-NULL args on myid=0
    if( myid==sourceProcessor )
    {
        for( int i=0; i<argc; i++ )
        {
            if( argv[i]==NULL ) break;
            numArgs++;
        }
    }
    broadCast(numArgs,sourceProcessor,comm);
    aString *argStrings = new aString[numArgs];
    if( myid!=sourceProcessor ) argv = new char* [numArgs];
    for( int i=0; i<numArgs; i++ )
    {
        if( myid==sourceProcessor ) argStrings[i]=argv[i];
        broadCast(argStrings[i],sourceProcessor,comm);
        if( myid!=sourceProcessor )
        {
            argv[i] = new char [argStrings[i].length()+1];
            strcpy(argv[i],argStrings[i].c_str());
        }
        
//     printf("broadCastArgs: myid=%i argv[%i]=%s\n",myid,i,argv[i]);
//     fflush(0);
//     Communication_Manager::Sync();
    }
    argc=numArgs;
    delete [] argStrings;

    return 0;
}

int ParallelUtility::
broadCastArgsCleanup(int & argc, char **&argv, int sourceProcessor /* = 0 */ )
// ====================================================================
// /Description:
//     Clean up routine for broadCastArgs. 
// ====================================================================
{
    #ifndef USE_PPP
        return 0;
    #endif
    const int myid=max(0,Communication_Manager::My_Process_Number);
    if( myid!=sourceProcessor )
    {
        for( int i=0; i<argc; i++ )
        {
            delete [] argv[i];
        }
        argc=0;
        delete [] argv; argv=NULL;
    }
    
}


int 
getLineFromFile( FILE *file, char s[], int lim);

int ParallelUtility::
getArgsFromFile(const aString & fileName, int & argc, char **&argv )
// ====================================================================
// /Description:
//   Get command line arguments from a file  (for use in parallel codes for example)
//
// /argc,argv (output) : 
// 
// /Note: call deleteArgsFromFile to clean up after use of the array argv.
// ===================================================================
{
    const int myid=Communication_Manager::My_Process_Number;

    FILE *file = fopen((const char*)fileName,"r" );
    if( file==NULL )
    {
        printf("getArgsFromFile:ERROR: opening file with name=[%s]\n",(const char*)fileName);
        return 1;
    }

    const int maxArgs=100;
    argv = new  char* [maxArgs];  // could delete
    argv[0] = new char[8];
    strncpy(argv[0],"unknown",8);
    
        
    aString line;
    const int maxBuff=300;
    char buff[maxBuff];
    argc=1;
    while( argc<maxArgs && getLineFromFile(file,buff,maxBuff) )
    {
        line=buff;
        if( myid <=0 ) printf(" line=[%s]\n",(const char*)line);
        if( line[0]=='*' ) continue;  // skip comments
            
        int i=0;
        int length=line.length();
        while( i<length  )
        {
            while(  i<length && line[i]==' ' ) i++;
            int istart=i;  
            i++;
            while(  i<length && line[i]!=' ' ) i++;
            int iend=i-1;
            
            if( iend>istart )
            {
	// printf(" line(istart,iend)=[%s]\n",(const char*)line(istart,iend));

      	argv[argc] = new char [iend-istart+2];
      	strncpy(argv[argc],(const char*)(line(istart,iend)),iend-istart+1);
      	argv[argc][iend-istart+1]=0;
	// printf(" argv[%i] = %s\n",argc,argv[argc]);
      	argc++;
            }
        }
    }
    if( argc==maxArgs ) printf(" ****getArgsFromFile:ERROR: too many args in the input file -- skipping the rest ***\n");
        
    fclose(file);
    return 0;
}



int ParallelUtility::
deleteArgsFromFile(int & argc, char **&argv )
// ====================================================================
// /Description:
//   This function will delete the arrays created by getArgsFromFile
// 
// ===================================================================
{
    for( int i=0; i<argc; i++ )
    {
        delete [] argv[i];
    }
    delete [] argv;

    return 0;
}

bool ParallelUtility::
getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
                                        Index & I1, Index & I2, Index & I3, 
                                        int option /* = 0 */ )
// ======================================================================================
// /Description: Get the local bounds
//
//  /u,uLocal (input): the arrays 
//  /I1,I2,I3 (input/output) : on input the bounds to be adjusted. On output these are
//      the adjusted bounds assuming that the local bounds define a non-empty region.
//  /option (input): 0: do not include parallel ghost boundaries. 
//                   1: include parallel ghost boundaries.
// /Return value: true if the local bounds define a non-empty region, false otherwise.
// ======================================================================================
{
    int n1a,n1b,n2a,n2b,n3a,n3b;
    return getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b,option);
}

bool ParallelUtility::
getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
                                        Index & I1, Index & I2, Index & I3, Index & I4,
                                        int option /* = 0 */ )
// ======================================================================================
// /Description: Get the local bounds
//
//  /u,uLocal (input): the arrays 
//  /I1,I2,I3,I4 (input/output) : on input the bounds to be adjusted. On output these are
//      the adjusted bounds assuming that the local bounds define a non-empty region.
//  /option (input): 0: do not include parallel ghost boundaries. 
//                   1: include parallel ghost boundaries.
// /Return value: true if the local bounds define a non-empty region, false otherwise.
// ======================================================================================
{
    int n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b;
    return getLocalArrayBounds(u,uLocal,I1,I2,I3,I4,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,option);
}

bool ParallelUtility::
getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
                                        Index & I1, Index & I2, Index & I3, 
                                        int & n1a, int & n1b, int & n2a, int & n2b, int & n3a, int & n3b,
                                        int option /* = 0 */ )
// ======================================================================================
// /Description: Get the local bounds
//
//  /u,uLocal (input): the arrays 
//  /I1,I2,I3 (input/output) : on input the bounds to be adjusted. On output these are
//      the adjusted bounds assuming that the local bounds define a non-empty region.
//  /n1a,n1b,n2a,n2b,n3a,n3b (output) : local bounds.
//  /option (input): 0: do not include parallel ghost boundaries. 
//                   1: include parallel ghost boundaries.
// /Return value: true if the local bounds define a non-empty region, false otherwise.
// ======================================================================================
{
    #ifndef USE_PPP
        n1a=I1.getBase(); n1b=I1.getBound();
        n2a=I2.getBase(); n2b=I2.getBound();
        n3a=I3.getBase(); n3b=I3.getBound();
    #else
    if( option==0 )
    {
    // do not include parallel ghost boundaries
        n1a = max(I1.getBase() , uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
        n1b = min(I1.getBound(),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));
        n2a = max(I2.getBase() , uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
        n2b = min(I2.getBound(),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));
        n3a = max(I3.getBase() , uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
        n3b = min(I3.getBound(),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));
    }
    else
    {
    // include parallel ghost boundaries
        n1a = max(I1.getBase() , uLocal.getBase(0));
        n1b = min(I1.getBound(),uLocal.getBound(0));
        n2a = max(I2.getBase() , uLocal.getBase(1));
        n2b = min(I2.getBound(),uLocal.getBound(1));
        n3a = max(I3.getBase() , uLocal.getBase(2));
        n3b = min(I3.getBound(),uLocal.getBound(2));
    }
    #endif
    if( n1a>n1b || n2a>n2b || n3a>n3b )
        return false;

    I1=Range(n1a,n1b);
    I2=Range(n2a,n2b);
    I3=Range(n3a,n3b);

    return true;
}

bool ParallelUtility::
getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
                                        Index & I1, Index & I2, Index & I3, Index & I4,
                                        int & n1a, int & n1b, int & n2a, int & n2b, int & n3a, int & n3b, int & n4a, int & n4b,
                                        int option /* = 0 */ )
// ======================================================================================
// /Description: Get the local bounds
//
//  /u,uLocal (input): the arrays 
//  /I1,I2,I3,I4 (input/output) : on input the bounds to be adjusted. On output these are
//      the adjusted bounds assuming that the local bounds define a non-empty region.
//  /n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b (output) : local bounds.
//  /option (input): 0: do not include parallel ghost boundaries. 
//                   1: include parallel ghost boundaries.
// /Return value: true if the local bounds define a non-empty region, false otherwise.
// ======================================================================================
{
    #ifndef USE_PPP
        n1a=I1.getBase(); n1b=I1.getBound();
        n2a=I2.getBase(); n2b=I2.getBound();
        n3a=I3.getBase(); n3b=I3.getBound();
        n4a=I4.getBase(); n4b=I4.getBound();
    #else
    if( option==0 )
    {
    // do not include parallel ghost boundaries
        n1a = max(I1.getBase() , uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
        n1b = min(I1.getBound(),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));
        n2a = max(I2.getBase() , uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
        n2b = min(I2.getBound(),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));
        n3a = max(I3.getBase() , uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
        n3b = min(I3.getBound(),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));
        n4a = max(I4.getBase() , uLocal.getBase(3)+u.getGhostBoundaryWidth(3));
        n4b = min(I4.getBound(),uLocal.getBound(3)-u.getGhostBoundaryWidth(3));
    }
    else
    {
    // include parallel ghost boundaries
        n1a = max(I1.getBase() , uLocal.getBase(0));
        n1b = min(I1.getBound(),uLocal.getBound(0));
        n2a = max(I2.getBase() , uLocal.getBase(1));
        n2b = min(I2.getBound(),uLocal.getBound(1));
        n3a = max(I3.getBase() , uLocal.getBase(2));
        n3b = min(I3.getBound(),uLocal.getBound(2));
        n4a = max(I4.getBase() , uLocal.getBase(3));
        n4b = min(I4.getBound(),uLocal.getBound(3));
    }
    #endif
    if( n1a>n1b || n2a>n2b || n3a>n3b || n4a>n4b )
        return false;

    I1=Range(n1a,n1b);
    I2=Range(n2a,n2b);
    I3=Range(n3a,n3b);
    I4=Range(n4a,n4b);

    return true;
}

bool ParallelUtility::
getLocalArrayBounds(const intArray & u, const intSerialArray & uLocal,
                                        Index & I1, Index & I2, Index & I3, 
                                        int option /* = 0 */ )
// ======================================================================================
// /Description: Get the local bounds
//
//  /u,uLocal (input): the arrays 
//  /I1,I2,I3 (input/output) : on input the bounds to be adjusted. On output these are
//      the adjusted bounds assuming that the local bounds define a non-empty region.
//  /option (input): 0: do not include parallel ghost boundaries. 
//                   1: include parallel ghost boundaries.
// /Return value: true if the local bounds define a non-empty region, false otherwise.
// ======================================================================================
{
    int n1a,n1b,n2a,n2b,n3a,n3b;
    return getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b,option);
}



bool ParallelUtility::
getLocalArrayBounds(const intArray & u, const intSerialArray & uLocal,
                                        Index & I1, Index & I2, Index & I3, 
                                        int & n1a, int & n1b, int & n2a, int & n2b, int & n3a, int & n3b,
                                        int option /* = 0 */ )
// ======================================================================================
// /Description: Get the local bounds
//
//  /u,uLocal (input): the arrays 
//  /I1,I2,I3 (input/output) : on input the bounds to be adjusted. On output these are
//      the adjusted bounds assuming that the local bounds define a non-empty region.
//  /n1a,n1b,n2a,n2b,n3a,n3b (output) : local bounds.
//  /option (input): 0: do not include parallel ghost boundaries. 
//                   1: include parallel ghost boundaries.
// /Return value: true if the local bounds define a non-empty region, false otherwise.
// ======================================================================================
{
    #ifndef USE_PPP
        n1a=I1.getBase(); n1b=I1.getBound();
        n2a=I2.getBase(); n2b=I2.getBound();
        n3a=I3.getBase(); n3b=I3.getBound();
    #else
    if( option==0 )
    {
    // do not include parallel ghost boundaries
        n1a = max(I1.getBase() , uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
        n1b = min(I1.getBound(),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));
        n2a = max(I2.getBase() , uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
        n2b = min(I2.getBound(),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));
        n3a = max(I3.getBase() , uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
        n3b = min(I3.getBound(),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));
    }
    else
    {
    // include parallel ghost boundaries
        n1a = max(I1.getBase() , uLocal.getBase(0));
        n1b = min(I1.getBound(),uLocal.getBound(0));
        n2a = max(I2.getBase() , uLocal.getBase(1));
        n2b = min(I2.getBound(),uLocal.getBound(1));
        n3a = max(I3.getBase() , uLocal.getBase(2));
        n3b = min(I3.getBound(),uLocal.getBound(2));
    }
    #endif
    if( n1a>n1b || n2a>n2b || n3a>n3b )
        return false;

    I1=Range(n1a,n1b);
    I2=Range(n2a,n2b);
    I3=Range(n3a,n3b);

    return true;
}



// REDISTRIBUTE_ARRAY(intArray)
int ParallelUtility::
redistribute(const intArray & u, intArray & v, const Range & P)
// /Description:
//    Build v, a copy of the array u that lives on the processors defined by the Range P
//  NOTE: P must be the same for all processors.
{
  // build a Partition that lives on this processor
    Partitioning_Type partition; 
    partition.SpecifyProcessorRange(P); 
    if( u.getInternalPartitionPointer()!=NULL )
    {
        Partitioning_Type uPartition=u.getPartition();
        for( int axis=0; axis<MAX_ARRAY_DIMENSION; axis++ )
        {
            int ghost=uPartition.getGhostBoundaryWidth(axis);
            if( ghost>0 )
      	partition.partitionAlongAxis(axis, true , ghost);
            else
      	partition.partitionAlongAxis(axis, false, 0);
        }
    }
    v.partition(partition);   
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2));
    v = u; // copy mask to this processor
    return 0;
}
// REDISTRIBUTE_ARRAY(floatArray)
int ParallelUtility::
redistribute(const floatArray & u, floatArray & v, const Range & P)
// /Description:
//    Build v, a copy of the array u that lives on the processors defined by the Range P
//  NOTE: P must be the same for all processors.
{
  // build a Partition that lives on this processor
    Partitioning_Type partition; 
    partition.SpecifyProcessorRange(P); 
    if( u.getInternalPartitionPointer()!=NULL )
    {
        Partitioning_Type uPartition=u.getPartition();
        for( int axis=0; axis<MAX_ARRAY_DIMENSION; axis++ )
        {
            int ghost=uPartition.getGhostBoundaryWidth(axis);
            if( ghost>0 )
      	partition.partitionAlongAxis(axis, true , ghost);
            else
      	partition.partitionAlongAxis(axis, false, 0);
        }
    }
    v.partition(partition);   
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2));
    v = u; // copy mask to this processor
    return 0;
}
// REDISTRIBUTE_ARRAY(doubleArray)
int ParallelUtility::
redistribute(const doubleArray & u, doubleArray & v, const Range & P)
// /Description:
//    Build v, a copy of the array u that lives on the processors defined by the Range P
//  NOTE: P must be the same for all processors.
{
  // build a Partition that lives on this processor
    Partitioning_Type partition; 
    partition.SpecifyProcessorRange(P); 
    if( u.getInternalPartitionPointer()!=NULL )
    {
        Partitioning_Type uPartition=u.getPartition();
        for( int axis=0; axis<MAX_ARRAY_DIMENSION; axis++ )
        {
            int ghost=uPartition.getGhostBoundaryWidth(axis);
            if( ghost>0 )
      	partition.partitionAlongAxis(axis, true , ghost);
            else
      	partition.partitionAlongAxis(axis, false, 0);
        }
    }
    v.partition(partition);   
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2));
    v = u; // copy mask to this processor
    return 0;
}

// ** add this to ParallelUtility **

// REDISTRIBUTE_ARRAY(intArray,intSerialArray)
int ParallelUtility::
redistribute( const intArray & u, intSerialArray & v )
// /Description:
//    Build v, a copy of the array u that lives on the local processor
{
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    Index Iv[4]={u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3)};
  // every processor gets a copy of the entire array:
    IndexBox *vBox = new IndexBox [np];
    for( int p=0; p<np; p++ )
    {
        vBox[p].setBounds(u.getBase(0),u.getBound(0),
                                            u.getBase(1),u.getBound(1),
                                            u.getBase(2),u.getBound(2),
                                            u.getBase(3),u.getBound(3) );
    }
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
    CopyArray::copyArray( u,Iv,vBox,v ); 
    delete [] vBox;
    return 0;
}
// REDISTRIBUTE_ARRAY(floatArray,floatSerialArray)
int ParallelUtility::
redistribute( const floatArray & u, floatSerialArray & v )
// /Description:
//    Build v, a copy of the array u that lives on the local processor
{
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    Index Iv[4]={u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3)};
  // every processor gets a copy of the entire array:
    IndexBox *vBox = new IndexBox [np];
    for( int p=0; p<np; p++ )
    {
        vBox[p].setBounds(u.getBase(0),u.getBound(0),
                                            u.getBase(1),u.getBound(1),
                                            u.getBase(2),u.getBound(2),
                                            u.getBase(3),u.getBound(3) );
    }
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
    CopyArray::copyArray( u,Iv,vBox,v ); 
    delete [] vBox;
    return 0;
}
// REDISTRIBUTE_ARRAY(doubleArray,doubleSerialArray)
int ParallelUtility::
redistribute( const doubleArray & u, doubleSerialArray & v )
// /Description:
//    Build v, a copy of the array u that lives on the local processor
{
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    Index Iv[4]={u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3)};
  // every processor gets a copy of the entire array:
    IndexBox *vBox = new IndexBox [np];
    for( int p=0; p<np; p++ )
    {
        vBox[p].setBounds(u.getBase(0),u.getBound(0),
                                            u.getBase(1),u.getBound(1),
                                            u.getBase(2),u.getBound(2),
                                            u.getBase(3),u.getBound(3) );
    }
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
    CopyArray::copyArray( u,Iv,vBox,v ); 
    delete [] vBox;
    return 0;
}



// ******************************************************************
//  define a copy from a P++ array to a generally distributed array
// *******************************************************************

int CopyArray::debug=0;

#define FOR_BOX(i0,i1,i2,i3,box)const int i0b=box.bound(0),i1b=box.bound(1),i2b=box.bound(2),i3b=box.bound(3);for( int i3=box.base(3); i3<=i3b; i3++ )for( int i2=box.base(2); i2<=i2b; i2++ )for( int i1=box.base(1); i1<=i1b; i1++ )for( int i0=box.base(0); i0<=i0b; i0++ )

IndexBox::IndexBox()
{
    for( int d=0; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
    {
        ab[d][0]=0;
        ab[d][1]=-1;
    }
    processor=-1;
}

IndexBox::IndexBox(int i1a, int i1b, int i2a, int i2b, int i3a, int i3b, int i4a, int i4b)
{
    setBounds(i1a, i1b, i2a, i2b, i3a, i3b, i4a, i4b);
}

IndexBox::
~IndexBox()
{
}

void IndexBox::
setBounds(int i1a, int i1b, int i2a, int i2b, int i3a, int i3b, int i4a, int i4b)
// Assign the corners of the box
{
    ab[0][0]=i1a; 
    ab[0][1]=i1b;

    ab[1][0]=i2a; 
    ab[1][1]=i2b;

    ab[2][0]=i3a; 
    ab[2][1]=i3b;

    ab[3][0]=i4a; 
    ab[3][1]=i4b;

}

bool IndexBox::isEmpty() const
// Return true if the box is empty
{
    bool empty=false;
    for( int d=0; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
    {
        if( ab[d][0]>ab[d][1] )
        {
      // printf(" isEmpty: d=%i a=%i b=%i --> empty!\n",d,ab[0][d],ab[1][d]);
            
            empty=true;
            break;
        }
    }
    return empty;
}

int IndexBox::
size() const
// Return the total number of elements in the box 
{
    int num=1;
    for( int d=0; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
    {
        num*=ab[d][1]-ab[d][0]+1;
    }
    return num;
}


bool IndexBox::
intersect(const IndexBox & a, const IndexBox & b, IndexBox & c)
//   c = a intersect b 
// 
// Return true if the resultant box is non-empty. 
// 
{
    int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
    bool notEmpty=true;
    for( int d=0; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
    {
        ia[d]=max(a.base(d),b.base(d)); 
        ib[d]=min(a.bound(d),b.bound(d));
        if( ia[d]>ib[d] )
        {
            notEmpty=false;
            return notEmpty;
        }
    }
    
    c.setBounds(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);
    return notEmpty;
}

static FILE *debugFile=NULL;


// *****************************************************************************
// Macro to define copyArray functions for realArray, intArray
// *****************************************************************************
// ********************************************************************************


 // ******** floatArray Versions *******

 // defineCopyArrayFunctions(floatDistributedArray,floatSerialArray,float,MPI_Real)
  void CopyArray::
  getLocalArrayInterval(const floatDistributedArray & u, int p, int *pv)
 //  Return the "processor vector"
 //       pv[0], pv[1], ..., pv[numDim-1]
 // Such that 
 //    p = baseProc + pv[numDim-1] + dimProc[numDim-1]*( pv[numDim-2] + dimProc[numDim-2]*( pv[numDim-3] + ... )
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
      p-= uDecomp->baseProc; 
 // ** note order ***  for( int d=0; d<numDim; d++ )
      for( int d=numDim-1; d>=0; d-- )
      {
          const int dimProc = uDecomp->dimProc[d];  // number of processors allocated to this dimension
          pv[d] = p - (p/dimProc)*dimProc;
          p= (p-pv[d])/dimProc;
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
          pv[d]=0;
  #endif
  }
  bool CopyArray::
  getLocalArrayBox( int p, const floatDistributedArray & u, IndexBox & uBox )
 // ===================================================================================
 // /Description:
 //   Return a box for the local array of u on processor p (no ghost points)
 // 
 // /p (input) : build a box for this processor.
 // /u (input) : parallel array
 // /box (output) : a box representing the array bounds (no ghost points) 
 //                 for the local array on processor p.
 // ===================================================================================
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
   // *wdh* 060824 -- fixed ---
      int p0=p-uDecomp->baseProc;
      if( p0<0 || p0>=uDecomp->nProcs )
      {
          uBox.setBounds(0,-1, 0,-1, 0,-1, 0,-1); // return an empty box
          return true;
      }
      int pv[MAX_DISTRIBUTED_DIMENSIONS];
      CopyArray::getLocalArrayInterval(u,p,pv);  // find where this processor lives in the P++ distribution
   // Now compute the bounds on the local array u on processor p
      int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
      const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
      for( int d=0; d<numDim; d++ )
      {
          int dimProc = uDecomp->dimProc[d];  // this dimension is split across this many processors
     // Fill in the distribution along each dimension following the block parti distribution
     // 
     //      +------+------+-------+---- ...    --+------+---------+
     //        left  center  center                center   right
          const int left = uDArray->dimVecL_L[d], center=uDArray->dimVecL[d], right=uDArray->dimVecL_R[d];
          ia[d] = u.getBase(d);
          if( pv[d]>0 ) ia[d]+=left;
          if( pv[d]>1 ) ia[d]+=center*(pv[d]-1);
          ib[d]=ia[d]; 
          if( pv[d]==0 )
              ib[d]+=left-1;  // add length of the left most interval
          else if( pv[d]<dimProc-1 )  
              ib[d]+=center-1;  // add length of a centre interval
          else
              ib[d]+=right-1;   // add length of the right interval
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
      {
     // ia[d]=ib[d]=0;
     // *wdh* 070924  u(-1:3,4:7,3;3) <- this has u.numberOfDimensions()==2, not 3 ! 
          ia[d]=ib[d]=u.getBase(d); 
      }
      uBox.setBounds(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);
  #else
   // Serial case:
      uBox.setBounds(u.getBase(0),u.getBound(0), u.getBase(1),u.getBound(1),
                                    u.getBase(2),u.getBound(2), u.getBase(3),u.getBound(3) );
  #endif
      return true;
  }
  bool CopyArray::
  getLocalArrayBoxWithGhost( int p, const floatDistributedArray & u, IndexBox & uBox )
 // ===================================================================================
 // Return a box for the local array of u on processor p (with ghost points)
 // ===================================================================================
  {
      bool returnValue=getLocalArrayBox( p,u,uBox );
  #ifdef USE_PPP
      if( uBox.base(0)<=uBox.bound(0) )
      { // add on parallel ghost points if the box is not empty
          uBox.setBounds(uBox.base(0)-u.getGhostBoundaryWidth(0), uBox.bound(0)+u.getGhostBoundaryWidth(0), 
                 		   uBox.base(1)-u.getGhostBoundaryWidth(1), uBox.bound(1)+u.getGhostBoundaryWidth(1), 
                 		   uBox.base(2)-u.getGhostBoundaryWidth(2), uBox.bound(2)+u.getGhostBoundaryWidth(2), 
                 		   uBox.base(3)-u.getGhostBoundaryWidth(3), uBox.bound(3)+u.getGhostBoundaryWidth(3));
      }
  #endif
      return returnValue;
  }
  int CopyArray::
  copyArray( const floatDistributedArray & u,
                        Index *Iv, 
                        IndexBox *vBox, // bounds of v on each processor, vBox[p] p=0,1,..,numProc-1
                        floatSerialArray & vLocal )
 // =======================================================================================
 // /Description:
 //    Perform a copy from a distributed array u, to a set of serial arrays v which loosely
 //    speaking looks like:
 //
 //           v(Iv[0],Iv[1],...) = u(Iv[0],Iv[1],...)
 // 
 //    where v is some distributed array associated with each vLocal. In fact the vLocal arrays
 //    can be more general and are defined by a given size on each processor, vBox[p]. 
 //
 // /u (input) : source array
 // /Iv[d] (input) : defines the global rectangular region to copy. d=array dimension, d=0,1,..,5
 //        The region to copy on each processor is the intersection of the region defined by { Iv[d] }
 //        with the bounds of the destination array v. 
 // /vBox[p] (input) : defines the bounds on v on all processors, p=0,1,..,np-1. (These bounds define a
 //     more general distribution than currently supported by P++ -- in fact these bounds do NOT need to
 //     exactly partition the global index space; they may overlap for e.g. -- but all bounds must be
 //     inside the rectangle { Iv[d] } ). 
 // /vLocal (input/output) : destination array (serial) (one for each processor). On input this array must
 //       be dimensioned to the correct size. 
 // 
 //  This type of copy is not currently supported by block PARTI. 
 // 
 // =======================================================================================
  {
  #ifdef USE_PPP
      const int myid = Communication_Manager::My_Process_Number;
      const int np=max(1,Communication_Manager::Number_Of_Processors);
      const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
      assert( numDim<=4 );
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
   // Step 1. Determine the information to send
   // Here is the global sub-array that we want to copy:
      IndexBox subArray(Iv[0].getBase(),Iv[0].getBound(),
                                          Iv[1].getBase(),Iv[1].getBound(),
                                          Iv[2].getBase(),Iv[2].getBound(),
                                          Iv[3].getBase(),Iv[3].getBound());
 //  const floatSerialArray & uLocal = u.getLocalArrayWithGhostBoundaries();
      floatSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
   // Here is the box for the local subArray
      int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
      for( int d=0; d<numDim; d++ )
      {
          ia[d] = uLocal.getBase(d) +u.getGhostBoundaryWidth(d);  // exclude ghost  
          ib[d] = uLocal.getBound(d)-u.getGhostBoundaryWidth(d);
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
      {
     // ia[d]=ib[d]=0;
          ia[d]=ib[d]=u.getBase(d); // *wdh* 070924  u(-1:3,4:7,3;3) <- this has u.numberOfDimensions()==2, not 3 ! 
      }
      IndexBox uLocalBox(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);
      if( debug!=0 )
      {
          fprintf(debugFile,"copyArray: myid=%i uLocalBox==[%i,%i][%i,%i][%i,%i][%i,%i]\n",
            	    myid,
            	    uLocalBox.base(0),uLocalBox.bound(0),
            	    uLocalBox.base(1),uLocalBox.bound(1),
            	    uLocalBox.base(2),uLocalBox.bound(2),
            	    uLocalBox.base(3),uLocalBox.bound(3)); 
          fflush(debugFile);
      }
      const float *up = uLocal.Array_Descriptor.Array_View_Pointer3;
      const int uDim0=uLocal.getRawDataSize(0);
      const int uDim1=uLocal.getRawDataSize(1);
      const int uDim2=uLocal.getRawDataSize(2);
  #undef U
  #define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]
      float *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
      const int vDim0=vLocal.getRawDataSize(0);
      const int vDim1=vLocal.getRawDataSize(1);
      const int vDim2=vLocal.getRawDataSize(2);
  #undef V
  #define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]
      IndexBox localSubArray;
      bool notEmpty = IndexBox::intersect(uLocalBox,subArray, localSubArray );
   // make a list of boxes of where to send the data
      ListOfIndexBox sendBoxes;
      if( notEmpty )
      {
     // We need to send info from uLocal
     // for each processor p, intersect localSubArray with vBox
          for( int p=0; p<np; p++ )
          {
              IndexBox pSendBox;
              notEmpty = IndexBox::intersect(localSubArray,vBox[p], pSendBox); 
              if( copyOnProcessor && notEmpty && myid==p )
              {
         // Do not send a message to the same processor -- just copy the data
        	if( debug!=0 )
        	{
          	  fprintf(debugFile,"****copyArray:*****\n"
                		  ">>> myid=%i: Just copy data: [%i,%i][%i,%i][%i,%i][%i,%i] on the same processor p=%i\n",
                		  myid,
                		  pSendBox.base(0),pSendBox.bound(0),
                		  pSendBox.base(1),pSendBox.bound(1),
                		  pSendBox.base(2),pSendBox.bound(2),
                		  pSendBox.base(3),pSendBox.bound(3),myid);
        	}
         // assign points defined in pSendBox:
                  FOR_BOX(i0,i1,i2,i3,pSendBox)
        	{ 
          	  V(i0,i1,i2,i3)=U(i0,i1,i2,i3);  
        	}
              }
              else if( notEmpty )
              {
        	pSendBox.processor=p;
                  sendBoxes.push_back(pSendBox);  // We need to send this box of data to processor p
              }
          }
          ListOfIndexBox::iterator iter; 
          for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
          {
              IndexBox & pSendBox = *iter;
              if( debug!=0 )
              {
        	fprintf(debugFile,"****copyArray:*****\n"
              		">>> myid=%i: Send box =[%i,%i][%i,%i][%i,%i][%i,%i] to processor p=%i\n",
              		myid,
              		pSendBox.base(0),pSendBox.bound(0),
              		pSendBox.base(1),pSendBox.bound(1),
              		pSendBox.base(2),pSendBox.bound(2),
              		pSendBox.base(3),pSendBox.bound(3),pSendBox.processor);
              }
          }
     // Send messages.....
     // if( p==myid ) // do not send data
      }
   // *********** Step 2. Determine the information to receive ***********
      if( debug!=0 )
      {
          fprintf(debugFile,"copyArray: myid=%i vLocalBox==[%i,%i][%i,%i][%i,%i][%i,%i], isEmpty=%i\n",
            	    myid,
            	    vBox[myid].base(0),vBox[myid].bound(0),
            	    vBox[myid].base(1),vBox[myid].bound(1),
            	    vBox[myid].base(2),vBox[myid].bound(2),
            	    vBox[myid].base(3),vBox[myid].bound(3),(int)vBox[myid].isEmpty());
          fflush(debugFile);
      }
      ListOfIndexBox receiveBoxes;
      IndexBox vLocalBox;
      notEmpty = IndexBox::intersect(vBox[myid],subArray, vLocalBox );
      if( notEmpty )
      {
     // make a list of boxes of where to receive the data from
     // for each processor p, intersect vBox with Iv[] with the local u array on processor p
          for( int p=0; p<np; p++ )
          {
              if( copyOnProcessor && myid==p )
        	continue;  // the data has already been transfered between the processor and itself.
              IndexBox puBox;  // defines the bounds of u on processor p
              CopyArray::getLocalArrayBox( p,u,puBox );  // this is without ghost pts -- is this right???
              if( debug!=0 )
              {
        	fprintf(debugFile,"copyArray: myid=%i puBox=[%i,%i][%i,%i][%i,%i][%i,%i] on p=%i\n",
              		myid,
              		puBox.base(0),puBox.bound(0),
              		puBox.base(1),puBox.bound(1),
              		puBox.base(2),puBox.bound(2),
              		puBox.base(3),puBox.bound(3),p);
              }
              IndexBox pReceiveBox;
              notEmpty = IndexBox::intersect(vLocalBox,puBox, pReceiveBox); 
              if( notEmpty )
              {
        	pReceiveBox.processor=p;
                  receiveBoxes.push_back(pReceiveBox);
              }
          }
          ListOfIndexBox::iterator iter; 
          for(iter = receiveBoxes.begin(); iter!=receiveBoxes.end(); iter++ )
          {
              IndexBox & pReceiveBox = *iter;
              if( debug!=0 )
              {
                  fprintf(debugFile,"****copyArray:*****\n"
              		"<<< myid=%i: Expecting to receive box =[%i,%i][%i,%i][%i,%i][%i,%i] from processor p=%i\n",
              		myid,
              		pReceiveBox.base(0),pReceiveBox.bound(0),
              		pReceiveBox.base(1),pReceiveBox.bound(1),
              		pReceiveBox.base(2),pReceiveBox.bound(2),
              		pReceiveBox.base(3),pReceiveBox.bound(3),pReceiveBox.processor);
              }
          }
     // post receives...
      }
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
     // post receives first
          receiveRequest= new MPI_Request[numReceive]; // remember to delete these
          receiveStatus= new MPI_Status[numReceive]; 
          rBuff = new float* [numReceive];
     // int sendingProc = new int [numReceive];
          for(int m=0; m<numReceive; m++ )
          {
              IndexBox & pReceiveBox = receiveBoxes[m];
       // sendingProc[m]=pReceiveBox.processor;  // this processor will be sending the data
              int bufSize=pReceiveBox.size();
              rBuff[m]= new float [bufSize];
              assert( pReceiveBox.processor>=0 && pReceiveBox.processor<np );
              receiveBoxIndex[pReceiveBox.processor]=m;  // maps processor number to index in receiveBoxes
              if( debug!=0 )
              {
        	fprintf(debugFile,">>> myid=%i: post a receive for buffer of size %i from p=%i (m=%i,numReceive=%i) \n",
                                  myid,bufSize,pReceiveBox.processor,m,numReceive);
              }
              MPI_Irecv(rBuff[m],bufSize,MPI_Real,pReceiveBox.processor,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
          }
      }
   // Now send the data
      const int numSend=sendBoxes.size();
      MPI_Request *sendRequest=NULL;
      float **sBuff=NULL;
      if( numSend>0 )
      {
     // send data
          sendRequest= new MPI_Request[numSend]; // remember to delete these
          sBuff = new float* [numSend];
          for(int m=0; m<numSend; m++ )
          {
              IndexBox & pSendBox = sendBoxes[m]; 
              int bufSize=pSendBox.size();
              sBuff[m]= new float [bufSize];
 //       for( int i=0; i<bufSize; i++ )
 //       {
 // 	sBuff[m][i]=i;
 //       }
              float *buff=sBuff[m];
              int i=0;
              FOR_BOX(i0,i1,i2,i3,pSendBox)
              {
        	buff[i]=uLocal(i0,i1,i2,i3);  
        	i++;
              }
              if( debug!=0 )
              {
        	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
              }
              MPI_Isend(sBuff[m],bufSize,MPI_Real,pSendBox.processor,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
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
       // assign vLocal(rBox) = rBuff[m][0...]
              const float *buff = rBuff[m];
              int i=0;
              FOR_BOX(i0,i1,i2,i3,rBox)
              {
        	vLocal(i0,i1,i2,i3)=buff[i]; 
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
          fprintf(debugFile,"**** myid=%i finished in copyArray ****\n",myid);
          fflush(debugFile);
      }
      return 0;
  #else
   // serial case
      vLocal(Iv[0],Iv[1],Iv[2],Iv[3])=u(Iv[0],Iv[1],Iv[2],Iv[3]);
      return 0;
  #endif
  }

 // ******** doubleArray Versions *******

 // defineCopyArrayFunctions(doubleDistributedArray,doubleSerialArray,double,MPI_Real)
  void CopyArray::
  getLocalArrayInterval(const doubleDistributedArray & u, int p, int *pv)
 //  Return the "processor vector"
 //       pv[0], pv[1], ..., pv[numDim-1]
 // Such that 
 //    p = baseProc + pv[numDim-1] + dimProc[numDim-1]*( pv[numDim-2] + dimProc[numDim-2]*( pv[numDim-3] + ... )
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
      p-= uDecomp->baseProc; 
 // ** note order ***  for( int d=0; d<numDim; d++ )
      for( int d=numDim-1; d>=0; d-- )
      {
          const int dimProc = uDecomp->dimProc[d];  // number of processors allocated to this dimension
          pv[d] = p - (p/dimProc)*dimProc;
          p= (p-pv[d])/dimProc;
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
          pv[d]=0;
  #endif
  }
  bool CopyArray::
  getLocalArrayBox( int p, const doubleDistributedArray & u, IndexBox & uBox )
 // ===================================================================================
 // /Description:
 //   Return a box for the local array of u on processor p (no ghost points)
 // 
 // /p (input) : build a box for this processor.
 // /u (input) : parallel array
 // /box (output) : a box representing the array bounds (no ghost points) 
 //                 for the local array on processor p.
 // ===================================================================================
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
   // *wdh* 060824 -- fixed ---
      int p0=p-uDecomp->baseProc;
      if( p0<0 || p0>=uDecomp->nProcs )
      {
          uBox.setBounds(0,-1, 0,-1, 0,-1, 0,-1); // return an empty box
          return true;
      }
      int pv[MAX_DISTRIBUTED_DIMENSIONS];
      CopyArray::getLocalArrayInterval(u,p,pv);  // find where this processor lives in the P++ distribution
   // Now compute the bounds on the local array u on processor p
      int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
      const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
      for( int d=0; d<numDim; d++ )
      {
          int dimProc = uDecomp->dimProc[d];  // this dimension is split across this many processors
     // Fill in the distribution along each dimension following the block parti distribution
     // 
     //      +------+------+-------+---- ...    --+------+---------+
     //        left  center  center                center   right
          const int left = uDArray->dimVecL_L[d], center=uDArray->dimVecL[d], right=uDArray->dimVecL_R[d];
          ia[d] = u.getBase(d);
          if( pv[d]>0 ) ia[d]+=left;
          if( pv[d]>1 ) ia[d]+=center*(pv[d]-1);
          ib[d]=ia[d]; 
          if( pv[d]==0 )
              ib[d]+=left-1;  // add length of the left most interval
          else if( pv[d]<dimProc-1 )  
              ib[d]+=center-1;  // add length of a centre interval
          else
              ib[d]+=right-1;   // add length of the right interval
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
      {
     // ia[d]=ib[d]=0;
     // *wdh* 070924  u(-1:3,4:7,3;3) <- this has u.numberOfDimensions()==2, not 3 ! 
          ia[d]=ib[d]=u.getBase(d); 
      }
      uBox.setBounds(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);
  #else
   // Serial case:
      uBox.setBounds(u.getBase(0),u.getBound(0), u.getBase(1),u.getBound(1),
                                    u.getBase(2),u.getBound(2), u.getBase(3),u.getBound(3) );
  #endif
      return true;
  }
  bool CopyArray::
  getLocalArrayBoxWithGhost( int p, const doubleDistributedArray & u, IndexBox & uBox )
 // ===================================================================================
 // Return a box for the local array of u on processor p (with ghost points)
 // ===================================================================================
  {
      bool returnValue=getLocalArrayBox( p,u,uBox );
  #ifdef USE_PPP
      if( uBox.base(0)<=uBox.bound(0) )
      { // add on parallel ghost points if the box is not empty
          uBox.setBounds(uBox.base(0)-u.getGhostBoundaryWidth(0), uBox.bound(0)+u.getGhostBoundaryWidth(0), 
                 		   uBox.base(1)-u.getGhostBoundaryWidth(1), uBox.bound(1)+u.getGhostBoundaryWidth(1), 
                 		   uBox.base(2)-u.getGhostBoundaryWidth(2), uBox.bound(2)+u.getGhostBoundaryWidth(2), 
                 		   uBox.base(3)-u.getGhostBoundaryWidth(3), uBox.bound(3)+u.getGhostBoundaryWidth(3));
      }
  #endif
      return returnValue;
  }
  int CopyArray::
  copyArray( const doubleDistributedArray & u,
                        Index *Iv, 
                        IndexBox *vBox, // bounds of v on each processor, vBox[p] p=0,1,..,numProc-1
                        doubleSerialArray & vLocal )
 // =======================================================================================
 // /Description:
 //    Perform a copy from a distributed array u, to a set of serial arrays v which loosely
 //    speaking looks like:
 //
 //           v(Iv[0],Iv[1],...) = u(Iv[0],Iv[1],...)
 // 
 //    where v is some distributed array associated with each vLocal. In fact the vLocal arrays
 //    can be more general and are defined by a given size on each processor, vBox[p]. 
 //
 // /u (input) : source array
 // /Iv[d] (input) : defines the global rectangular region to copy. d=array dimension, d=0,1,..,5
 //        The region to copy on each processor is the intersection of the region defined by { Iv[d] }
 //        with the bounds of the destination array v. 
 // /vBox[p] (input) : defines the bounds on v on all processors, p=0,1,..,np-1. (These bounds define a
 //     more general distribution than currently supported by P++ -- in fact these bounds do NOT need to
 //     exactly partition the global index space; they may overlap for e.g. -- but all bounds must be
 //     inside the rectangle { Iv[d] } ). 
 // /vLocal (input/output) : destination array (serial) (one for each processor). On input this array must
 //       be dimensioned to the correct size. 
 // 
 //  This type of copy is not currently supported by block PARTI. 
 // 
 // =======================================================================================
  {
  #ifdef USE_PPP
      const int myid = Communication_Manager::My_Process_Number;
      const int np=max(1,Communication_Manager::Number_Of_Processors);
      const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
      assert( numDim<=4 );
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
   // Step 1. Determine the information to send
   // Here is the global sub-array that we want to copy:
      IndexBox subArray(Iv[0].getBase(),Iv[0].getBound(),
                                          Iv[1].getBase(),Iv[1].getBound(),
                                          Iv[2].getBase(),Iv[2].getBound(),
                                          Iv[3].getBase(),Iv[3].getBound());
 //  const doubleSerialArray & uLocal = u.getLocalArrayWithGhostBoundaries();
      doubleSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
   // Here is the box for the local subArray
      int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
      for( int d=0; d<numDim; d++ )
      {
          ia[d] = uLocal.getBase(d) +u.getGhostBoundaryWidth(d);  // exclude ghost  
          ib[d] = uLocal.getBound(d)-u.getGhostBoundaryWidth(d);
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
      {
     // ia[d]=ib[d]=0;
          ia[d]=ib[d]=u.getBase(d); // *wdh* 070924  u(-1:3,4:7,3;3) <- this has u.numberOfDimensions()==2, not 3 ! 
      }
      IndexBox uLocalBox(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);
      if( debug!=0 )
      {
          fprintf(debugFile,"copyArray: myid=%i uLocalBox==[%i,%i][%i,%i][%i,%i][%i,%i]\n",
            	    myid,
            	    uLocalBox.base(0),uLocalBox.bound(0),
            	    uLocalBox.base(1),uLocalBox.bound(1),
            	    uLocalBox.base(2),uLocalBox.bound(2),
            	    uLocalBox.base(3),uLocalBox.bound(3)); 
          fflush(debugFile);
      }
      const double *up = uLocal.Array_Descriptor.Array_View_Pointer3;
      const int uDim0=uLocal.getRawDataSize(0);
      const int uDim1=uLocal.getRawDataSize(1);
      const int uDim2=uLocal.getRawDataSize(2);
  #undef U
  #define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]
      double *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
      const int vDim0=vLocal.getRawDataSize(0);
      const int vDim1=vLocal.getRawDataSize(1);
      const int vDim2=vLocal.getRawDataSize(2);
  #undef V
  #define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]
      IndexBox localSubArray;
      bool notEmpty = IndexBox::intersect(uLocalBox,subArray, localSubArray );
   // make a list of boxes of where to send the data
      ListOfIndexBox sendBoxes;
      if( notEmpty )
      {
     // We need to send info from uLocal
     // for each processor p, intersect localSubArray with vBox
          for( int p=0; p<np; p++ )
          {
              IndexBox pSendBox;
              notEmpty = IndexBox::intersect(localSubArray,vBox[p], pSendBox); 
              if( copyOnProcessor && notEmpty && myid==p )
              {
         // Do not send a message to the same processor -- just copy the data
        	if( debug!=0 )
        	{
          	  fprintf(debugFile,"****copyArray:*****\n"
                		  ">>> myid=%i: Just copy data: [%i,%i][%i,%i][%i,%i][%i,%i] on the same processor p=%i\n",
                		  myid,
                		  pSendBox.base(0),pSendBox.bound(0),
                		  pSendBox.base(1),pSendBox.bound(1),
                		  pSendBox.base(2),pSendBox.bound(2),
                		  pSendBox.base(3),pSendBox.bound(3),myid);
        	}
         // assign points defined in pSendBox:
                  FOR_BOX(i0,i1,i2,i3,pSendBox)
        	{ 
          	  V(i0,i1,i2,i3)=U(i0,i1,i2,i3);  
        	}
              }
              else if( notEmpty )
              {
        	pSendBox.processor=p;
                  sendBoxes.push_back(pSendBox);  // We need to send this box of data to processor p
              }
          }
          ListOfIndexBox::iterator iter; 
          for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
          {
              IndexBox & pSendBox = *iter;
              if( debug!=0 )
              {
        	fprintf(debugFile,"****copyArray:*****\n"
              		">>> myid=%i: Send box =[%i,%i][%i,%i][%i,%i][%i,%i] to processor p=%i\n",
              		myid,
              		pSendBox.base(0),pSendBox.bound(0),
              		pSendBox.base(1),pSendBox.bound(1),
              		pSendBox.base(2),pSendBox.bound(2),
              		pSendBox.base(3),pSendBox.bound(3),pSendBox.processor);
              }
          }
     // Send messages.....
     // if( p==myid ) // do not send data
      }
   // *********** Step 2. Determine the information to receive ***********
      if( debug!=0 )
      {
          fprintf(debugFile,"copyArray: myid=%i vLocalBox==[%i,%i][%i,%i][%i,%i][%i,%i], isEmpty=%i\n",
            	    myid,
            	    vBox[myid].base(0),vBox[myid].bound(0),
            	    vBox[myid].base(1),vBox[myid].bound(1),
            	    vBox[myid].base(2),vBox[myid].bound(2),
            	    vBox[myid].base(3),vBox[myid].bound(3),(int)vBox[myid].isEmpty());
          fflush(debugFile);
      }
      ListOfIndexBox receiveBoxes;
      IndexBox vLocalBox;
      notEmpty = IndexBox::intersect(vBox[myid],subArray, vLocalBox );
      if( notEmpty )
      {
     // make a list of boxes of where to receive the data from
     // for each processor p, intersect vBox with Iv[] with the local u array on processor p
          for( int p=0; p<np; p++ )
          {
              if( copyOnProcessor && myid==p )
        	continue;  // the data has already been transfered between the processor and itself.
              IndexBox puBox;  // defines the bounds of u on processor p
              CopyArray::getLocalArrayBox( p,u,puBox );  // this is without ghost pts -- is this right???
              if( debug!=0 )
              {
        	fprintf(debugFile,"copyArray: myid=%i puBox=[%i,%i][%i,%i][%i,%i][%i,%i] on p=%i\n",
              		myid,
              		puBox.base(0),puBox.bound(0),
              		puBox.base(1),puBox.bound(1),
              		puBox.base(2),puBox.bound(2),
              		puBox.base(3),puBox.bound(3),p);
              }
              IndexBox pReceiveBox;
              notEmpty = IndexBox::intersect(vLocalBox,puBox, pReceiveBox); 
              if( notEmpty )
              {
        	pReceiveBox.processor=p;
                  receiveBoxes.push_back(pReceiveBox);
              }
          }
          ListOfIndexBox::iterator iter; 
          for(iter = receiveBoxes.begin(); iter!=receiveBoxes.end(); iter++ )
          {
              IndexBox & pReceiveBox = *iter;
              if( debug!=0 )
              {
                  fprintf(debugFile,"****copyArray:*****\n"
              		"<<< myid=%i: Expecting to receive box =[%i,%i][%i,%i][%i,%i][%i,%i] from processor p=%i\n",
              		myid,
              		pReceiveBox.base(0),pReceiveBox.bound(0),
              		pReceiveBox.base(1),pReceiveBox.bound(1),
              		pReceiveBox.base(2),pReceiveBox.bound(2),
              		pReceiveBox.base(3),pReceiveBox.bound(3),pReceiveBox.processor);
              }
          }
     // post receives...
      }
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
     // post receives first
          receiveRequest= new MPI_Request[numReceive]; // remember to delete these
          receiveStatus= new MPI_Status[numReceive]; 
          rBuff = new double* [numReceive];
     // int sendingProc = new int [numReceive];
          for(int m=0; m<numReceive; m++ )
          {
              IndexBox & pReceiveBox = receiveBoxes[m];
       // sendingProc[m]=pReceiveBox.processor;  // this processor will be sending the data
              int bufSize=pReceiveBox.size();
              rBuff[m]= new double [bufSize];
              assert( pReceiveBox.processor>=0 && pReceiveBox.processor<np );
              receiveBoxIndex[pReceiveBox.processor]=m;  // maps processor number to index in receiveBoxes
              if( debug!=0 )
              {
        	fprintf(debugFile,">>> myid=%i: post a receive for buffer of size %i from p=%i (m=%i,numReceive=%i) \n",
                                  myid,bufSize,pReceiveBox.processor,m,numReceive);
              }
              MPI_Irecv(rBuff[m],bufSize,MPI_Real,pReceiveBox.processor,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
          }
      }
   // Now send the data
      const int numSend=sendBoxes.size();
      MPI_Request *sendRequest=NULL;
      double **sBuff=NULL;
      if( numSend>0 )
      {
     // send data
          sendRequest= new MPI_Request[numSend]; // remember to delete these
          sBuff = new double* [numSend];
          for(int m=0; m<numSend; m++ )
          {
              IndexBox & pSendBox = sendBoxes[m]; 
              int bufSize=pSendBox.size();
              sBuff[m]= new double [bufSize];
 //       for( int i=0; i<bufSize; i++ )
 //       {
 // 	sBuff[m][i]=i;
 //       }
              double *buff=sBuff[m];
              int i=0;
              FOR_BOX(i0,i1,i2,i3,pSendBox)
              {
        	buff[i]=uLocal(i0,i1,i2,i3);  
        	i++;
              }
              if( debug!=0 )
              {
        	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
              }
              MPI_Isend(sBuff[m],bufSize,MPI_Real,pSendBox.processor,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
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
       // assign vLocal(rBox) = rBuff[m][0...]
              const double *buff = rBuff[m];
              int i=0;
              FOR_BOX(i0,i1,i2,i3,rBox)
              {
        	vLocal(i0,i1,i2,i3)=buff[i]; 
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
          fprintf(debugFile,"**** myid=%i finished in copyArray ****\n",myid);
          fflush(debugFile);
      }
      return 0;
  #else
   // serial case
      vLocal(Iv[0],Iv[1],Iv[2],Iv[3])=u(Iv[0],Iv[1],Iv[2],Iv[3]);
      return 0;
  #endif
  }


 // ******** intArray Versions *******

 // defineCopyArrayFunctions(intDistributedArray,intSerialArray,int,MPI_INT)
  void CopyArray::
  getLocalArrayInterval(const intDistributedArray & u, int p, int *pv)
 //  Return the "processor vector"
 //       pv[0], pv[1], ..., pv[numDim-1]
 // Such that 
 //    p = baseProc + pv[numDim-1] + dimProc[numDim-1]*( pv[numDim-2] + dimProc[numDim-2]*( pv[numDim-3] + ... )
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
      p-= uDecomp->baseProc; 
 // ** note order ***  for( int d=0; d<numDim; d++ )
      for( int d=numDim-1; d>=0; d-- )
      {
          const int dimProc = uDecomp->dimProc[d];  // number of processors allocated to this dimension
          pv[d] = p - (p/dimProc)*dimProc;
          p= (p-pv[d])/dimProc;
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
          pv[d]=0;
  #endif
  }
  bool CopyArray::
  getLocalArrayBox( int p, const intDistributedArray & u, IndexBox & uBox )
 // ===================================================================================
 // /Description:
 //   Return a box for the local array of u on processor p (no ghost points)
 // 
 // /p (input) : build a box for this processor.
 // /u (input) : parallel array
 // /box (output) : a box representing the array bounds (no ghost points) 
 //                 for the local array on processor p.
 // ===================================================================================
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
   // *wdh* 060824 -- fixed ---
      int p0=p-uDecomp->baseProc;
      if( p0<0 || p0>=uDecomp->nProcs )
      {
          uBox.setBounds(0,-1, 0,-1, 0,-1, 0,-1); // return an empty box
          return true;
      }
      int pv[MAX_DISTRIBUTED_DIMENSIONS];
      CopyArray::getLocalArrayInterval(u,p,pv);  // find where this processor lives in the P++ distribution
   // Now compute the bounds on the local array u on processor p
      int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
      const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
      for( int d=0; d<numDim; d++ )
      {
          int dimProc = uDecomp->dimProc[d];  // this dimension is split across this many processors
     // Fill in the distribution along each dimension following the block parti distribution
     // 
     //      +------+------+-------+---- ...    --+------+---------+
     //        left  center  center                center   right
          const int left = uDArray->dimVecL_L[d], center=uDArray->dimVecL[d], right=uDArray->dimVecL_R[d];
          ia[d] = u.getBase(d);
          if( pv[d]>0 ) ia[d]+=left;
          if( pv[d]>1 ) ia[d]+=center*(pv[d]-1);
          ib[d]=ia[d]; 
          if( pv[d]==0 )
              ib[d]+=left-1;  // add length of the left most interval
          else if( pv[d]<dimProc-1 )  
              ib[d]+=center-1;  // add length of a centre interval
          else
              ib[d]+=right-1;   // add length of the right interval
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
      {
     // ia[d]=ib[d]=0;
     // *wdh* 070924  u(-1:3,4:7,3;3) <- this has u.numberOfDimensions()==2, not 3 ! 
          ia[d]=ib[d]=u.getBase(d); 
      }
      uBox.setBounds(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);
  #else
   // Serial case:
      uBox.setBounds(u.getBase(0),u.getBound(0), u.getBase(1),u.getBound(1),
                                    u.getBase(2),u.getBound(2), u.getBase(3),u.getBound(3) );
  #endif
      return true;
  }
  bool CopyArray::
  getLocalArrayBoxWithGhost( int p, const intDistributedArray & u, IndexBox & uBox )
 // ===================================================================================
 // Return a box for the local array of u on processor p (with ghost points)
 // ===================================================================================
  {
      bool returnValue=getLocalArrayBox( p,u,uBox );
  #ifdef USE_PPP
      if( uBox.base(0)<=uBox.bound(0) )
      { // add on parallel ghost points if the box is not empty
          uBox.setBounds(uBox.base(0)-u.getGhostBoundaryWidth(0), uBox.bound(0)+u.getGhostBoundaryWidth(0), 
                 		   uBox.base(1)-u.getGhostBoundaryWidth(1), uBox.bound(1)+u.getGhostBoundaryWidth(1), 
                 		   uBox.base(2)-u.getGhostBoundaryWidth(2), uBox.bound(2)+u.getGhostBoundaryWidth(2), 
                 		   uBox.base(3)-u.getGhostBoundaryWidth(3), uBox.bound(3)+u.getGhostBoundaryWidth(3));
      }
  #endif
      return returnValue;
  }
  int CopyArray::
  copyArray( const intDistributedArray & u,
                        Index *Iv, 
                        IndexBox *vBox, // bounds of v on each processor, vBox[p] p=0,1,..,numProc-1
                        intSerialArray & vLocal )
 // =======================================================================================
 // /Description:
 //    Perform a copy from a distributed array u, to a set of serial arrays v which loosely
 //    speaking looks like:
 //
 //           v(Iv[0],Iv[1],...) = u(Iv[0],Iv[1],...)
 // 
 //    where v is some distributed array associated with each vLocal. In fact the vLocal arrays
 //    can be more general and are defined by a given size on each processor, vBox[p]. 
 //
 // /u (input) : source array
 // /Iv[d] (input) : defines the global rectangular region to copy. d=array dimension, d=0,1,..,5
 //        The region to copy on each processor is the intersection of the region defined by { Iv[d] }
 //        with the bounds of the destination array v. 
 // /vBox[p] (input) : defines the bounds on v on all processors, p=0,1,..,np-1. (These bounds define a
 //     more general distribution than currently supported by P++ -- in fact these bounds do NOT need to
 //     exactly partition the global index space; they may overlap for e.g. -- but all bounds must be
 //     inside the rectangle { Iv[d] } ). 
 // /vLocal (input/output) : destination array (serial) (one for each processor). On input this array must
 //       be dimensioned to the correct size. 
 // 
 //  This type of copy is not currently supported by block PARTI. 
 // 
 // =======================================================================================
  {
  #ifdef USE_PPP
      const int myid = Communication_Manager::My_Process_Number;
      const int np=max(1,Communication_Manager::Number_Of_Processors);
      const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
      assert( numDim<=4 );
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
   // Step 1. Determine the information to send
   // Here is the global sub-array that we want to copy:
      IndexBox subArray(Iv[0].getBase(),Iv[0].getBound(),
                                          Iv[1].getBase(),Iv[1].getBound(),
                                          Iv[2].getBase(),Iv[2].getBound(),
                                          Iv[3].getBase(),Iv[3].getBound());
 //  const intSerialArray & uLocal = u.getLocalArrayWithGhostBoundaries();
      intSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
   // Here is the box for the local subArray
      int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
      for( int d=0; d<numDim; d++ )
      {
          ia[d] = uLocal.getBase(d) +u.getGhostBoundaryWidth(d);  // exclude ghost  
          ib[d] = uLocal.getBound(d)-u.getGhostBoundaryWidth(d);
      }
      for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
      {
     // ia[d]=ib[d]=0;
          ia[d]=ib[d]=u.getBase(d); // *wdh* 070924  u(-1:3,4:7,3;3) <- this has u.numberOfDimensions()==2, not 3 ! 
      }
      IndexBox uLocalBox(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);
      if( debug!=0 )
      {
          fprintf(debugFile,"copyArray: myid=%i uLocalBox==[%i,%i][%i,%i][%i,%i][%i,%i]\n",
            	    myid,
            	    uLocalBox.base(0),uLocalBox.bound(0),
            	    uLocalBox.base(1),uLocalBox.bound(1),
            	    uLocalBox.base(2),uLocalBox.bound(2),
            	    uLocalBox.base(3),uLocalBox.bound(3)); 
          fflush(debugFile);
      }
      const int *up = uLocal.Array_Descriptor.Array_View_Pointer3;
      const int uDim0=uLocal.getRawDataSize(0);
      const int uDim1=uLocal.getRawDataSize(1);
      const int uDim2=uLocal.getRawDataSize(2);
  #undef U
  #define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]
      int *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
      const int vDim0=vLocal.getRawDataSize(0);
      const int vDim1=vLocal.getRawDataSize(1);
      const int vDim2=vLocal.getRawDataSize(2);
  #undef V
  #define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]
      IndexBox localSubArray;
      bool notEmpty = IndexBox::intersect(uLocalBox,subArray, localSubArray );
   // make a list of boxes of where to send the data
      ListOfIndexBox sendBoxes;
      if( notEmpty )
      {
     // We need to send info from uLocal
     // for each processor p, intersect localSubArray with vBox
          for( int p=0; p<np; p++ )
          {
              IndexBox pSendBox;
              notEmpty = IndexBox::intersect(localSubArray,vBox[p], pSendBox); 
              if( copyOnProcessor && notEmpty && myid==p )
              {
         // Do not send a message to the same processor -- just copy the data
        	if( debug!=0 )
        	{
          	  fprintf(debugFile,"****copyArray:*****\n"
                		  ">>> myid=%i: Just copy data: [%i,%i][%i,%i][%i,%i][%i,%i] on the same processor p=%i\n",
                		  myid,
                		  pSendBox.base(0),pSendBox.bound(0),
                		  pSendBox.base(1),pSendBox.bound(1),
                		  pSendBox.base(2),pSendBox.bound(2),
                		  pSendBox.base(3),pSendBox.bound(3),myid);
        	}
         // assign points defined in pSendBox:
                  FOR_BOX(i0,i1,i2,i3,pSendBox)
        	{ 
          	  V(i0,i1,i2,i3)=U(i0,i1,i2,i3);  
        	}
              }
              else if( notEmpty )
              {
        	pSendBox.processor=p;
                  sendBoxes.push_back(pSendBox);  // We need to send this box of data to processor p
              }
          }
          ListOfIndexBox::iterator iter; 
          for(iter = sendBoxes.begin(); iter!=sendBoxes.end(); iter++ )
          {
              IndexBox & pSendBox = *iter;
              if( debug!=0 )
              {
        	fprintf(debugFile,"****copyArray:*****\n"
              		">>> myid=%i: Send box =[%i,%i][%i,%i][%i,%i][%i,%i] to processor p=%i\n",
              		myid,
              		pSendBox.base(0),pSendBox.bound(0),
              		pSendBox.base(1),pSendBox.bound(1),
              		pSendBox.base(2),pSendBox.bound(2),
              		pSendBox.base(3),pSendBox.bound(3),pSendBox.processor);
              }
          }
     // Send messages.....
     // if( p==myid ) // do not send data
      }
   // *********** Step 2. Determine the information to receive ***********
      if( debug!=0 )
      {
          fprintf(debugFile,"copyArray: myid=%i vLocalBox==[%i,%i][%i,%i][%i,%i][%i,%i], isEmpty=%i\n",
            	    myid,
            	    vBox[myid].base(0),vBox[myid].bound(0),
            	    vBox[myid].base(1),vBox[myid].bound(1),
            	    vBox[myid].base(2),vBox[myid].bound(2),
            	    vBox[myid].base(3),vBox[myid].bound(3),(int)vBox[myid].isEmpty());
          fflush(debugFile);
      }
      ListOfIndexBox receiveBoxes;
      IndexBox vLocalBox;
      notEmpty = IndexBox::intersect(vBox[myid],subArray, vLocalBox );
      if( notEmpty )
      {
     // make a list of boxes of where to receive the data from
     // for each processor p, intersect vBox with Iv[] with the local u array on processor p
          for( int p=0; p<np; p++ )
          {
              if( copyOnProcessor && myid==p )
        	continue;  // the data has already been transfered between the processor and itself.
              IndexBox puBox;  // defines the bounds of u on processor p
              CopyArray::getLocalArrayBox( p,u,puBox );  // this is without ghost pts -- is this right???
              if( debug!=0 )
              {
        	fprintf(debugFile,"copyArray: myid=%i puBox=[%i,%i][%i,%i][%i,%i][%i,%i] on p=%i\n",
              		myid,
              		puBox.base(0),puBox.bound(0),
              		puBox.base(1),puBox.bound(1),
              		puBox.base(2),puBox.bound(2),
              		puBox.base(3),puBox.bound(3),p);
              }
              IndexBox pReceiveBox;
              notEmpty = IndexBox::intersect(vLocalBox,puBox, pReceiveBox); 
              if( notEmpty )
              {
        	pReceiveBox.processor=p;
                  receiveBoxes.push_back(pReceiveBox);
              }
          }
          ListOfIndexBox::iterator iter; 
          for(iter = receiveBoxes.begin(); iter!=receiveBoxes.end(); iter++ )
          {
              IndexBox & pReceiveBox = *iter;
              if( debug!=0 )
              {
                  fprintf(debugFile,"****copyArray:*****\n"
              		"<<< myid=%i: Expecting to receive box =[%i,%i][%i,%i][%i,%i][%i,%i] from processor p=%i\n",
              		myid,
              		pReceiveBox.base(0),pReceiveBox.bound(0),
              		pReceiveBox.base(1),pReceiveBox.bound(1),
              		pReceiveBox.base(2),pReceiveBox.bound(2),
              		pReceiveBox.base(3),pReceiveBox.bound(3),pReceiveBox.processor);
              }
          }
     // post receives...
      }
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
     // post receives first
          receiveRequest= new MPI_Request[numReceive]; // remember to delete these
          receiveStatus= new MPI_Status[numReceive]; 
          rBuff = new int* [numReceive];
     // int sendingProc = new int [numReceive];
          for(int m=0; m<numReceive; m++ )
          {
              IndexBox & pReceiveBox = receiveBoxes[m];
       // sendingProc[m]=pReceiveBox.processor;  // this processor will be sending the data
              int bufSize=pReceiveBox.size();
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
   // Now send the data
      const int numSend=sendBoxes.size();
      MPI_Request *sendRequest=NULL;
      int **sBuff=NULL;
      if( numSend>0 )
      {
     // send data
          sendRequest= new MPI_Request[numSend]; // remember to delete these
          sBuff = new int* [numSend];
          for(int m=0; m<numSend; m++ )
          {
              IndexBox & pSendBox = sendBoxes[m]; 
              int bufSize=pSendBox.size();
              sBuff[m]= new int [bufSize];
 //       for( int i=0; i<bufSize; i++ )
 //       {
 // 	sBuff[m][i]=i;
 //       }
              int *buff=sBuff[m];
              int i=0;
              FOR_BOX(i0,i1,i2,i3,pSendBox)
              {
        	buff[i]=uLocal(i0,i1,i2,i3);  
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
       // assign vLocal(rBox) = rBuff[m][0...]
              const int *buff = rBuff[m];
              int i=0;
              FOR_BOX(i0,i1,i2,i3,rBox)
              {
        	vLocal(i0,i1,i2,i3)=buff[i]; 
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
          fprintf(debugFile,"**** myid=%i finished in copyArray ****\n",myid);
          fflush(debugFile);
      }
      return 0;
  #else
   // serial case
      vLocal(Iv[0],Iv[1],Iv[2],Iv[3])=u(Iv[0],Iv[1],Iv[2],Iv[3]);
      return 0;
  #endif
  }




static inline int floordiv (int numer, int denom)
{
    if (numer>0)
        return (numer/denom);
    else
        return (numer-denom+1)/denom;
}

// ---------------------------------------------
// Macro 
// ---------------------------------------------

 // copyCoarseToFineMacro(floatArray,floatSerialArray)
  void CopyArray::
  copyCoarseToFine( const floatArray & uc, const floatArray & uf, Index *Iv, floatSerialArray & uc2, int *ratio, int *ghost)
 // or copySubArray
 // ===============================================================================================
 // 
 // Copy coarse grid values uc, to local arrays uc2 that are distributed in the same way as a fine
 // grid uf. 
 //
 // /Iv[d] : d=0,1,2,3 - defines the box of values to copy (in the fine grid index space)
 // /ratio[d] : d=0,1,2 - refinement ratios
 // /ghost[d] : d=0,1,2 - extra ghost values to include on each local array
 //
 // /Motivation:
 //    Suppose we have a coarse grid patch with solution uc and a fine grid patch, uf, that sits in the
 // coarse grid patch. If we want to know the coarse grid values that lie underneath the fine grid
 // patch on a local processor then we will need to communicate the underlying coarse grid values
 // to the fine grid processor. This function is used to make this copy of coarse grid values to
 // the fine grid processor. 
 // ==============================================================================================
  {
      const int myid=max(Communication_Manager::My_Process_Number,0);
      const int np=max(1,Communication_Manager::Number_Of_Processors);
      FILE *pDebugFile = Overture::debugFile;  // debug file that is different on each processor
   // global bounds on the fine grid covered by the coarse grid patch
      int nf1a = Iv[0].getBase();
      int nf1b = Iv[0].getBound();
      int nf2a = Iv[1].getBase();
      int nf2b = Iv[1].getBound();
      int nf3a = Iv[2].getBase();
      int nf3b = Iv[2].getBound();
      Range R4=Iv[3];
   // global coarse grid bounds needed:
      int mc1a = floordiv(nf1a,ratio[0])-ghost[0], mc1b = floordiv(nf1b+(ratio[0]-1),ratio[0])+ghost[0];   
      int mc2a = floordiv(nf2a,ratio[1])-ghost[1], mc2b = floordiv(nf2b+(ratio[1]-1),ratio[1])+ghost[1];
      int mc3a = floordiv(nf3a,ratio[2])-ghost[2], mc3b = floordiv(nf3b+(ratio[2]-1),ratio[2])+ghost[2];
      Index Ivc[4];   // global coarse grid bounds
      Ivc[0]=Range(mc1a,mc1b);
      Ivc[1]=Range(mc2a,mc2b);
      Ivc[2]=Range(mc3a,mc3b);
      Ivc[3]=Iv[3]; 
   // For the parallel copy we need to know the sizes of the local "coarse grid" arrays on all processors
      IndexBox *uc2BoxArray = new IndexBox[np]; 
      for( int p=0; p<np; p++ )
      {
          IndexBox fBox;  // box for coarse and fine grid bounds on processor p 
          CopyArray::getLocalArrayBoxWithGhost( p, uf, fBox );
     // Here are the fine grid points on processor p: 
          int n1a = max(Iv[0].getBase(),  fBox.base(0) +uf.getGhostBoundaryWidth(0));
          int n1b = min(Iv[0].getBound(), fBox.bound(0)-uf.getGhostBoundaryWidth(0));
          int n2a = max(Iv[1].getBase(), fBox.base(1) +uf.getGhostBoundaryWidth(1));
          int n2b = min(Iv[1].getBound(),fBox.bound(1)-uf.getGhostBoundaryWidth(1));
          int n3a = max(Iv[2].getBase(), fBox.base(2) +uf.getGhostBoundaryWidth(2));
          int n3b = min(Iv[2].getBound(),fBox.bound(2)-uf.getGhostBoundaryWidth(2));
     // Here are the coarse grid points on processor p that we need: 
          int m1a,m1b,m2a,m2b,m3a,m3b;
          if( n1a<=n1b && n2a<=n2b && n3a<=n3b )
          {
              m1a = floordiv(n1a,ratio[0])-ghost[0], m1b = floordiv(n1b+(ratio[0]-1),ratio[0])+ghost[0];   
              m2a = floordiv(n2a,ratio[1])-ghost[1], m2b = floordiv(n2b+(ratio[1]-1),ratio[1])+ghost[1];
              m3a = floordiv(n3a,ratio[2])-ghost[2], m3b = floordiv(n3b+(ratio[2]-1),ratio[2])+ghost[2];
          }
          else
          {
              m1a=m2a=m3a=0;  m1b=m2b=m3b=-1;
          }
          if( pDebugFile!=NULL )
          {
              printf("copyCoarseToFine: Coarse bounds as computed on myid=%i\n"
             	     "               m1a,m1b,...  =[%i,%i][%i,%i][%i,%i]\n"
             	     "               n1a,n1b,...  =[%i,%i][%i,%i][%i,%i]\n"
             	     "               fBox         =[%i,%i][%i,%i][%i,%i]\n"
             	     ,myid,
             	     m1a,m1b,m2a,m2b,m3a,m3b,
             	     n1a,n1b,n2a,n2b,n3a,n3b,
             	     fBox.base(0),fBox.bound(0),fBox.base(1),fBox.bound(1),fBox.base(2),fBox.bound(2));
          }
          uc2BoxArray[p].setBounds(m1a,m1b, m2a,m2b, m3a,m3b, Iv[3].getBase(), Iv[3].getBound() );
      } // end for p
   // uc2: space to hold the coarse grid points
   //      We will copy these from other processors as needed
   // ***NOTE: we could avoid this copy if np==1, just use ucLocal ***
      IndexBox & cBox = uc2BoxArray[myid];
      if( cBox.base(0)<=cBox.bound(0) && cBox.base(1)<=cBox.bound(1) && cBox.base(2)<=cBox.bound(2) )
      {
          Range R1(cBox.base(0),cBox.bound(0)),R2(cBox.base(1),cBox.bound(1)),R3(cBox.base(2),cBox.bound(2));
          uc2.redim(R1,R2,R3,R4);  
      }
      if( pDebugFile!=NULL )
      {
          fprintf(pDebugFile,
                                              "*** copyCoarseToFine:                       Iv=[%i,%i][%i,%i][%i,%i]\n"
                                              "                 fine grid bounds :            [%i,%i][%i,%i][%i,%i]\n"
                                              "               coarse grid bounds :            [%i,%i][%i,%i][%i,%i]\n"
                                              "         uc2: local coarse grid bounds:        [%i,%i][%i,%i][%i,%i]\n"
                                              " ratio=[%i,%i,%i], ghost=%i,%i,%i\n"
                          ,Iv[0].getBase(),Iv[0].getBound(),Iv[1].getBase(),Iv[1].getBound(),
                            Iv[2].getBase(),Iv[2].getBound(),
                          uf.getBase(0),uf.getBound(0),uf.getBase(1),uf.getBound(1),uf.getBase(2),uf.getBound(2),
                          uc.getBase(0),uc.getBound(0),uc.getBase(1),uc.getBound(1),uc.getBase(2),uc.getBound(2),
                          uc2.getBase(0),uc2.getBound(0),uc2.getBase(1),uc2.getBound(1),uc2.getBase(2),uc2.getBound(2),
                          ratio[0],ratio[1],ratio[2],ghost[0],ghost[1],ghost[2]);
          fflush( pDebugFile );
      }
   // CopyArray::debug=1;
   // Now fill in the local uc2 arrays from the distributed uc array:
      CopyArray::copyArray(uc,Ivc,uc2BoxArray,uc2); 
      delete [] uc2BoxArray;
 //   if( debugFile!=NULL )
 //   {
 //     ::display(uc ,"copyCoarseToFine: uc : the coarse grid values      ",debugFile," %3.1f ");
 //     ::display(uf ,"copyCoarseToFine: uf : BEFORE interpFineFromCoarse ",debugFile," %3.1f ");
 //   }
      if( pDebugFile!=NULL )
      { // NOTE: do not display a distributed array to the pDebugFile since this points to different
     // files on different processors
          ::display(uc2,"copyCoarseToFine: uc2: the coarse grid copied the the local processor",pDebugFile," %3.1f ");
          fflush( pDebugFile );
      }
  }
 // copyCoarseToFineMacro(doubleArray,doubleSerialArray)
  void CopyArray::
  copyCoarseToFine( const doubleArray & uc, const doubleArray & uf, Index *Iv, doubleSerialArray & uc2, int *ratio, int *ghost)
 // or copySubArray
 // ===============================================================================================
 // 
 // Copy coarse grid values uc, to local arrays uc2 that are distributed in the same way as a fine
 // grid uf. 
 //
 // /Iv[d] : d=0,1,2,3 - defines the box of values to copy (in the fine grid index space)
 // /ratio[d] : d=0,1,2 - refinement ratios
 // /ghost[d] : d=0,1,2 - extra ghost values to include on each local array
 //
 // /Motivation:
 //    Suppose we have a coarse grid patch with solution uc and a fine grid patch, uf, that sits in the
 // coarse grid patch. If we want to know the coarse grid values that lie underneath the fine grid
 // patch on a local processor then we will need to communicate the underlying coarse grid values
 // to the fine grid processor. This function is used to make this copy of coarse grid values to
 // the fine grid processor. 
 // ==============================================================================================
  {
      const int myid=max(Communication_Manager::My_Process_Number,0);
      const int np=max(1,Communication_Manager::Number_Of_Processors);
      FILE *pDebugFile = Overture::debugFile;  // debug file that is different on each processor
   // global bounds on the fine grid covered by the coarse grid patch
      int nf1a = Iv[0].getBase();
      int nf1b = Iv[0].getBound();
      int nf2a = Iv[1].getBase();
      int nf2b = Iv[1].getBound();
      int nf3a = Iv[2].getBase();
      int nf3b = Iv[2].getBound();
      Range R4=Iv[3];
   // global coarse grid bounds needed:
      int mc1a = floordiv(nf1a,ratio[0])-ghost[0], mc1b = floordiv(nf1b+(ratio[0]-1),ratio[0])+ghost[0];   
      int mc2a = floordiv(nf2a,ratio[1])-ghost[1], mc2b = floordiv(nf2b+(ratio[1]-1),ratio[1])+ghost[1];
      int mc3a = floordiv(nf3a,ratio[2])-ghost[2], mc3b = floordiv(nf3b+(ratio[2]-1),ratio[2])+ghost[2];
      Index Ivc[4];   // global coarse grid bounds
      Ivc[0]=Range(mc1a,mc1b);
      Ivc[1]=Range(mc2a,mc2b);
      Ivc[2]=Range(mc3a,mc3b);
      Ivc[3]=Iv[3]; 
   // For the parallel copy we need to know the sizes of the local "coarse grid" arrays on all processors
      IndexBox *uc2BoxArray = new IndexBox[np]; 
      for( int p=0; p<np; p++ )
      {
          IndexBox fBox;  // box for coarse and fine grid bounds on processor p 
          CopyArray::getLocalArrayBoxWithGhost( p, uf, fBox );
     // Here are the fine grid points on processor p: 
          int n1a = max(Iv[0].getBase(),  fBox.base(0) +uf.getGhostBoundaryWidth(0));
          int n1b = min(Iv[0].getBound(), fBox.bound(0)-uf.getGhostBoundaryWidth(0));
          int n2a = max(Iv[1].getBase(), fBox.base(1) +uf.getGhostBoundaryWidth(1));
          int n2b = min(Iv[1].getBound(),fBox.bound(1)-uf.getGhostBoundaryWidth(1));
          int n3a = max(Iv[2].getBase(), fBox.base(2) +uf.getGhostBoundaryWidth(2));
          int n3b = min(Iv[2].getBound(),fBox.bound(2)-uf.getGhostBoundaryWidth(2));
     // Here are the coarse grid points on processor p that we need: 
          int m1a,m1b,m2a,m2b,m3a,m3b;
          if( n1a<=n1b && n2a<=n2b && n3a<=n3b )
          {
              m1a = floordiv(n1a,ratio[0])-ghost[0], m1b = floordiv(n1b+(ratio[0]-1),ratio[0])+ghost[0];   
              m2a = floordiv(n2a,ratio[1])-ghost[1], m2b = floordiv(n2b+(ratio[1]-1),ratio[1])+ghost[1];
              m3a = floordiv(n3a,ratio[2])-ghost[2], m3b = floordiv(n3b+(ratio[2]-1),ratio[2])+ghost[2];
          }
          else
          {
              m1a=m2a=m3a=0;  m1b=m2b=m3b=-1;
          }
          if( pDebugFile!=NULL )
          {
              printf("copyCoarseToFine: Coarse bounds as computed on myid=%i\n"
             	     "               m1a,m1b,...  =[%i,%i][%i,%i][%i,%i]\n"
             	     "               n1a,n1b,...  =[%i,%i][%i,%i][%i,%i]\n"
             	     "               fBox         =[%i,%i][%i,%i][%i,%i]\n"
             	     ,myid,
             	     m1a,m1b,m2a,m2b,m3a,m3b,
             	     n1a,n1b,n2a,n2b,n3a,n3b,
             	     fBox.base(0),fBox.bound(0),fBox.base(1),fBox.bound(1),fBox.base(2),fBox.bound(2));
          }
          uc2BoxArray[p].setBounds(m1a,m1b, m2a,m2b, m3a,m3b, Iv[3].getBase(), Iv[3].getBound() );
      } // end for p
   // uc2: space to hold the coarse grid points
   //      We will copy these from other processors as needed
   // ***NOTE: we could avoid this copy if np==1, just use ucLocal ***
      IndexBox & cBox = uc2BoxArray[myid];
      if( cBox.base(0)<=cBox.bound(0) && cBox.base(1)<=cBox.bound(1) && cBox.base(2)<=cBox.bound(2) )
      {
          Range R1(cBox.base(0),cBox.bound(0)),R2(cBox.base(1),cBox.bound(1)),R3(cBox.base(2),cBox.bound(2));
          uc2.redim(R1,R2,R3,R4);  
      }
      if( pDebugFile!=NULL )
      {
          fprintf(pDebugFile,
                                              "*** copyCoarseToFine:                       Iv=[%i,%i][%i,%i][%i,%i]\n"
                                              "                 fine grid bounds :            [%i,%i][%i,%i][%i,%i]\n"
                                              "               coarse grid bounds :            [%i,%i][%i,%i][%i,%i]\n"
                                              "         uc2: local coarse grid bounds:        [%i,%i][%i,%i][%i,%i]\n"
                                              " ratio=[%i,%i,%i], ghost=%i,%i,%i\n"
                          ,Iv[0].getBase(),Iv[0].getBound(),Iv[1].getBase(),Iv[1].getBound(),
                            Iv[2].getBase(),Iv[2].getBound(),
                          uf.getBase(0),uf.getBound(0),uf.getBase(1),uf.getBound(1),uf.getBase(2),uf.getBound(2),
                          uc.getBase(0),uc.getBound(0),uc.getBase(1),uc.getBound(1),uc.getBase(2),uc.getBound(2),
                          uc2.getBase(0),uc2.getBound(0),uc2.getBase(1),uc2.getBound(1),uc2.getBase(2),uc2.getBound(2),
                          ratio[0],ratio[1],ratio[2],ghost[0],ghost[1],ghost[2]);
          fflush( pDebugFile );
      }
   // CopyArray::debug=1;
   // Now fill in the local uc2 arrays from the distributed uc array:
      CopyArray::copyArray(uc,Ivc,uc2BoxArray,uc2); 
      delete [] uc2BoxArray;
 //   if( debugFile!=NULL )
 //   {
 //     ::display(uc ,"copyCoarseToFine: uc : the coarse grid values      ",debugFile," %3.1f ");
 //     ::display(uf ,"copyCoarseToFine: uf : BEFORE interpFineFromCoarse ",debugFile," %3.1f ");
 //   }
      if( pDebugFile!=NULL )
      { // NOTE: do not display a distributed array to the pDebugFile since this points to different
     // files on different processors
          ::display(uc2,"copyCoarseToFine: uc2: the coarse grid copied the the local processor",pDebugFile," %3.1f ");
          fflush( pDebugFile );
      }
  }
 // copyCoarseToFineMacro(intArray,intSerialArray)
  void CopyArray::
  copyCoarseToFine( const intArray & uc, const intArray & uf, Index *Iv, intSerialArray & uc2, int *ratio, int *ghost)
 // or copySubArray
 // ===============================================================================================
 // 
 // Copy coarse grid values uc, to local arrays uc2 that are distributed in the same way as a fine
 // grid uf. 
 //
 // /Iv[d] : d=0,1,2,3 - defines the box of values to copy (in the fine grid index space)
 // /ratio[d] : d=0,1,2 - refinement ratios
 // /ghost[d] : d=0,1,2 - extra ghost values to include on each local array
 //
 // /Motivation:
 //    Suppose we have a coarse grid patch with solution uc and a fine grid patch, uf, that sits in the
 // coarse grid patch. If we want to know the coarse grid values that lie underneath the fine grid
 // patch on a local processor then we will need to communicate the underlying coarse grid values
 // to the fine grid processor. This function is used to make this copy of coarse grid values to
 // the fine grid processor. 
 // ==============================================================================================
  {
      const int myid=max(Communication_Manager::My_Process_Number,0);
      const int np=max(1,Communication_Manager::Number_Of_Processors);
      FILE *pDebugFile = Overture::debugFile;  // debug file that is different on each processor
   // global bounds on the fine grid covered by the coarse grid patch
      int nf1a = Iv[0].getBase();
      int nf1b = Iv[0].getBound();
      int nf2a = Iv[1].getBase();
      int nf2b = Iv[1].getBound();
      int nf3a = Iv[2].getBase();
      int nf3b = Iv[2].getBound();
      Range R4=Iv[3];
   // global coarse grid bounds needed:
      int mc1a = floordiv(nf1a,ratio[0])-ghost[0], mc1b = floordiv(nf1b+(ratio[0]-1),ratio[0])+ghost[0];   
      int mc2a = floordiv(nf2a,ratio[1])-ghost[1], mc2b = floordiv(nf2b+(ratio[1]-1),ratio[1])+ghost[1];
      int mc3a = floordiv(nf3a,ratio[2])-ghost[2], mc3b = floordiv(nf3b+(ratio[2]-1),ratio[2])+ghost[2];
      Index Ivc[4];   // global coarse grid bounds
      Ivc[0]=Range(mc1a,mc1b);
      Ivc[1]=Range(mc2a,mc2b);
      Ivc[2]=Range(mc3a,mc3b);
      Ivc[3]=Iv[3]; 
   // For the parallel copy we need to know the sizes of the local "coarse grid" arrays on all processors
      IndexBox *uc2BoxArray = new IndexBox[np]; 
      for( int p=0; p<np; p++ )
      {
          IndexBox fBox;  // box for coarse and fine grid bounds on processor p 
          CopyArray::getLocalArrayBoxWithGhost( p, uf, fBox );
     // Here are the fine grid points on processor p: 
          int n1a = max(Iv[0].getBase(),  fBox.base(0) +uf.getGhostBoundaryWidth(0));
          int n1b = min(Iv[0].getBound(), fBox.bound(0)-uf.getGhostBoundaryWidth(0));
          int n2a = max(Iv[1].getBase(), fBox.base(1) +uf.getGhostBoundaryWidth(1));
          int n2b = min(Iv[1].getBound(),fBox.bound(1)-uf.getGhostBoundaryWidth(1));
          int n3a = max(Iv[2].getBase(), fBox.base(2) +uf.getGhostBoundaryWidth(2));
          int n3b = min(Iv[2].getBound(),fBox.bound(2)-uf.getGhostBoundaryWidth(2));
     // Here are the coarse grid points on processor p that we need: 
          int m1a,m1b,m2a,m2b,m3a,m3b;
          if( n1a<=n1b && n2a<=n2b && n3a<=n3b )
          {
              m1a = floordiv(n1a,ratio[0])-ghost[0], m1b = floordiv(n1b+(ratio[0]-1),ratio[0])+ghost[0];   
              m2a = floordiv(n2a,ratio[1])-ghost[1], m2b = floordiv(n2b+(ratio[1]-1),ratio[1])+ghost[1];
              m3a = floordiv(n3a,ratio[2])-ghost[2], m3b = floordiv(n3b+(ratio[2]-1),ratio[2])+ghost[2];
          }
          else
          {
              m1a=m2a=m3a=0;  m1b=m2b=m3b=-1;
          }
          if( pDebugFile!=NULL )
          {
              printf("copyCoarseToFine: Coarse bounds as computed on myid=%i\n"
             	     "               m1a,m1b,...  =[%i,%i][%i,%i][%i,%i]\n"
             	     "               n1a,n1b,...  =[%i,%i][%i,%i][%i,%i]\n"
             	     "               fBox         =[%i,%i][%i,%i][%i,%i]\n"
             	     ,myid,
             	     m1a,m1b,m2a,m2b,m3a,m3b,
             	     n1a,n1b,n2a,n2b,n3a,n3b,
             	     fBox.base(0),fBox.bound(0),fBox.base(1),fBox.bound(1),fBox.base(2),fBox.bound(2));
          }
          uc2BoxArray[p].setBounds(m1a,m1b, m2a,m2b, m3a,m3b, Iv[3].getBase(), Iv[3].getBound() );
      } // end for p
   // uc2: space to hold the coarse grid points
   //      We will copy these from other processors as needed
   // ***NOTE: we could avoid this copy if np==1, just use ucLocal ***
      IndexBox & cBox = uc2BoxArray[myid];
      if( cBox.base(0)<=cBox.bound(0) && cBox.base(1)<=cBox.bound(1) && cBox.base(2)<=cBox.bound(2) )
      {
          Range R1(cBox.base(0),cBox.bound(0)),R2(cBox.base(1),cBox.bound(1)),R3(cBox.base(2),cBox.bound(2));
          uc2.redim(R1,R2,R3,R4);  
      }
      if( pDebugFile!=NULL )
      {
          fprintf(pDebugFile,
                                              "*** copyCoarseToFine:                       Iv=[%i,%i][%i,%i][%i,%i]\n"
                                              "                 fine grid bounds :            [%i,%i][%i,%i][%i,%i]\n"
                                              "               coarse grid bounds :            [%i,%i][%i,%i][%i,%i]\n"
                                              "         uc2: local coarse grid bounds:        [%i,%i][%i,%i][%i,%i]\n"
                                              " ratio=[%i,%i,%i], ghost=%i,%i,%i\n"
                          ,Iv[0].getBase(),Iv[0].getBound(),Iv[1].getBase(),Iv[1].getBound(),
                            Iv[2].getBase(),Iv[2].getBound(),
                          uf.getBase(0),uf.getBound(0),uf.getBase(1),uf.getBound(1),uf.getBase(2),uf.getBound(2),
                          uc.getBase(0),uc.getBound(0),uc.getBase(1),uc.getBound(1),uc.getBase(2),uc.getBound(2),
                          uc2.getBase(0),uc2.getBound(0),uc2.getBase(1),uc2.getBound(1),uc2.getBase(2),uc2.getBound(2),
                          ratio[0],ratio[1],ratio[2],ghost[0],ghost[1],ghost[2]);
          fflush( pDebugFile );
      }
   // CopyArray::debug=1;
   // Now fill in the local uc2 arrays from the distributed uc array:
      CopyArray::copyArray(uc,Ivc,uc2BoxArray,uc2); 
      delete [] uc2BoxArray;
 //   if( debugFile!=NULL )
 //   {
 //     ::display(uc ,"copyCoarseToFine: uc : the coarse grid values      ",debugFile," %3.1f ");
 //     ::display(uf ,"copyCoarseToFine: uf : BEFORE interpFineFromCoarse ",debugFile," %3.1f ");
 //   }
      if( pDebugFile!=NULL )
      { // NOTE: do not display a distributed array to the pDebugFile since this points to different
     // files on different processors
          ::display(uc2,"copyCoarseToFine: uc2: the coarse grid copied the the local processor",pDebugFile," %3.1f ");
          fflush( pDebugFile );
      }
  }




#define FOR_4(i0,i1,i2,i3,Iv)const int i0b=Iv[0].getBound(),i1b=Iv[1].getBound(),i2b=Iv[2].getBound(),i3b=Iv[3].getBound();for( int i3=Iv[0].getBase(); i3<=i3b; i3++ )for( int i2=Iv[1].getBase(); i2<=i2b; i2++ )for( int i1=Iv[2].getBase(); i1<=i1b; i1++ )for( int i0=Iv[3].getBase(); i0<=i0b; i0++ )




// getAggregateArrayMacro(floatSerialArray,float,MPI_FLOAT);
void CopyArray::
getAggregateArray( floatSerialArray & u, Index *Iv, floatSerialArray & u0, int p0)
// ====================================================================================
//   Combine the values from local arrays to form an aggregate array on processor p0
// 
//  Aggregate over dimension 0:
//        u0(i,Iv[1]) = [u_0(j,Iv[1]), u_1(j,Iv[1]), u_2(j,Iv[1]), ... ]   
// 
//  /u (input) : source arrays
//  /Iv[0..1] : data to copy is u(Iv[0],Iv[1])
//  /u0 (output) : destination array on processor p0
//  /p0 (input) : rank of the destination processor.
// ===========================================================================
{
#ifdef USE_PPP
    const int myid = Communication_Manager::My_Process_Number;
    const int np=max(1,Communication_Manager::Number_Of_Processors);
//const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    int n0 = Iv[0].length(); // number of entries (dimension 0)
    int n0Total=ParallelUtility::getSum(n0);  // total number of entries (dimension 0)
    int n1=Iv[1].length();  // this value is assumed to be the same on all processors
    int numValues=n0*n1;
    int maxNumPerProc=ParallelUtility::getMaxValue(numValues); 
//   int numValues = Iv[0].length()*Iv[1].length()*Iv[2].length()*Iv[3].length();
//   int maxNumPerProc=getMaxValue(numValues); 
//   int totalNum=getSum(numValues); 
  // for now expect a message from every processor:  (could optimize this)
    const int numReceive=myid==p0 ? np : 0;   // number of messages to receive on this proc.
    float **rBuff=NULL;   // buffers for receiving data
    MPI_Request *receiveRequest=NULL;
    MPI_Status *receiveStatus=NULL;
    if( numReceive>0 )
    {
    // post receives first
        receiveRequest= new MPI_Request[numReceive]; // remember to delete these
        receiveStatus= new MPI_Status[numReceive]; 
        rBuff = new float* [numReceive];
        for(int m=0; m<numReceive; m++ )
        {
            int bufSize=maxNumPerProc; // expect at most this many values
            rBuff[m]= new float [bufSize];
            MPI_Irecv(rBuff[m],bufSize,MPI_FLOAT,m,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
        }
    }
  // Now send the data
    const int numSend=1; // for now every processor sends a message
    MPI_Request *sendRequest=NULL;
    float **sBuff=NULL;
    if( numSend>0 )
    {
    // send data
        sendRequest= new MPI_Request[numSend]; 
        sBuff = new float* [numSend];
        for(int m=0; m<numSend; m++ )
        {
            int bufSize=numValues; 
            sBuff[m]= new float [bufSize];
            float *buff=sBuff[m];
            int i=0; 
            const int i1a=Iv[1].getBase(), i1b=Iv[1].getBound();
            const int i0a=Iv[0].getBase(), i0b=Iv[0].getBound();
            for( int i1=i1a; i1<=i1b; i1++)
            for( int i0=i0a; i0<=i0b; i0++)
            {
      	buff[i]=u(i0,i1);  
      	i++;
            }
//       if( debug!=0 )
//       {
// 	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
//       }
            MPI_Isend(sBuff[m],bufSize,MPI_FLOAT,p0,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
        }
    }
    if( numReceive>0 )
    {
    // *wdh* 060523 -- only redimension if the existing dimensions are not correct
        if( u0.dimension(0)!=Range(n0Total) || u0.dimension(1)!=Iv[1] )
            u0.redim(n0Total,Iv[1]); // put aggregate values here 
        MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to receive all messages
        int i=0;  // counts total number of values recieved (dimension 0)
        for(int m=0; m<numReceive; m++  )
        {
            int bufSize=receiveStatus[m].MPI_TAG;
            int p = receiveStatus[m].MPI_SOURCE;
            assert( p>=0 && p<np );
//       if( debug!=0 )
//       {
// 	fprintf(debugFile,">>> myid=%i: received buffer of size %i from p=%i (m=%i,numReceive=%i)\n myid=%i: buff=",
//             myid,bufSize,p,m,numReceive,myid);
// 	for( int j=0; j<bufSize; j++ ) fprintf(debugFile,"%3.1f ",rBuff[m][j]);
// 	fprintf(debugFile,"\n");
//       }
      // assign vLocal(rBox) = rBuff[m][0...]
            const float *buff = rBuff[m];
            assert( bufSize % n1 == 0);
            int nd0 = bufSize/n1;
            const int i1a=Iv[1].getBase(), i1b=Iv[1].getBound();
            const int i0a=0, i0b=nd0-1;
            for( int i0=i0a; i0<=i0b; i0++)
            {
                for( int i1=i1a; i1<=i1b; i1++)
      	{
        	  u0(i,i1)=buff[i0+nd0*(i1)]; 
      	}
      	i++;
            }
        }
        assert( i==n0Total );
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
//     if( debug!=0 )
//     {
//       fprintf(debugFile,"+++ myid=%i: wait for all messges to be sent, numSend=%i\n",myid,numSend); 
//       fflush(debugFile);
//     }
        MPI_Status *sendStatus = new MPI_Status[numSend]; 
        MPI_Waitall( numSend, sendRequest, sendStatus );   
        delete [] sendStatus;
    }
//   if( debug!=0 )
//   {
//     fprintf(debugFile,"+++ myid=%i: cleanup buffers...\n",myid); 
//     fflush(debugFile);
//   }
    for (int i=0; i<numReceive; i++ )
    {
        delete [] rBuff[i];
    }
    delete [] rBuff;
    delete [] receiveStatus;
    delete [] receiveRequest;
    for (int i=0; i<numSend; i++ )
    {
        delete [] sBuff[i];
    }
    delete [] sBuff;
    delete [] sendRequest;
#else
  //   Overture::abort("CopyArray::getAggregateArray:ERROR:this function should only be called in parallel");
    u0.redim(Iv[0],Iv[1]);
    u0=u(Iv[0],Iv[1]);
#endif
}

// getAggregateArrayMacro(doubleSerialArray,double,MPI_DOUBLE);
void CopyArray::
getAggregateArray( doubleSerialArray & u, Index *Iv, doubleSerialArray & u0, int p0)
// ====================================================================================
//   Combine the values from local arrays to form an aggregate array on processor p0
// 
//  Aggregate over dimension 0:
//        u0(i,Iv[1]) = [u_0(j,Iv[1]), u_1(j,Iv[1]), u_2(j,Iv[1]), ... ]   
// 
//  /u (input) : source arrays
//  /Iv[0..1] : data to copy is u(Iv[0],Iv[1])
//  /u0 (output) : destination array on processor p0
//  /p0 (input) : rank of the destination processor.
// ===========================================================================
{
#ifdef USE_PPP
    const int myid = Communication_Manager::My_Process_Number;
    const int np=max(1,Communication_Manager::Number_Of_Processors);
//const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    int n0 = Iv[0].length(); // number of entries (dimension 0)
    int n0Total=ParallelUtility::getSum(n0);  // total number of entries (dimension 0)
    int n1=Iv[1].length();  // this value is assumed to be the same on all processors
    int numValues=n0*n1;
    int maxNumPerProc=ParallelUtility::getMaxValue(numValues); 
//   int numValues = Iv[0].length()*Iv[1].length()*Iv[2].length()*Iv[3].length();
//   int maxNumPerProc=getMaxValue(numValues); 
//   int totalNum=getSum(numValues); 
  // for now expect a message from every processor:  (could optimize this)
    const int numReceive=myid==p0 ? np : 0;   // number of messages to receive on this proc.
    double **rBuff=NULL;   // buffers for receiving data
    MPI_Request *receiveRequest=NULL;
    MPI_Status *receiveStatus=NULL;
    if( numReceive>0 )
    {
    // post receives first
        receiveRequest= new MPI_Request[numReceive]; // remember to delete these
        receiveStatus= new MPI_Status[numReceive]; 
        rBuff = new double* [numReceive];
        for(int m=0; m<numReceive; m++ )
        {
            int bufSize=maxNumPerProc; // expect at most this many values
            rBuff[m]= new double [bufSize];
            MPI_Irecv(rBuff[m],bufSize,MPI_DOUBLE,m,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
        }
    }
  // Now send the data
    const int numSend=1; // for now every processor sends a message
    MPI_Request *sendRequest=NULL;
    double **sBuff=NULL;
    if( numSend>0 )
    {
    // send data
        sendRequest= new MPI_Request[numSend]; 
        sBuff = new double* [numSend];
        for(int m=0; m<numSend; m++ )
        {
            int bufSize=numValues; 
            sBuff[m]= new double [bufSize];
            double *buff=sBuff[m];
            int i=0; 
            const int i1a=Iv[1].getBase(), i1b=Iv[1].getBound();
            const int i0a=Iv[0].getBase(), i0b=Iv[0].getBound();
            for( int i1=i1a; i1<=i1b; i1++)
            for( int i0=i0a; i0<=i0b; i0++)
            {
      	buff[i]=u(i0,i1);  
      	i++;
            }
//       if( debug!=0 )
//       {
// 	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
//       }
            MPI_Isend(sBuff[m],bufSize,MPI_DOUBLE,p0,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
        }
    }
    if( numReceive>0 )
    {
    // *wdh* 060523 -- only redimension if the existing dimensions are not correct
        if( u0.dimension(0)!=Range(n0Total) || u0.dimension(1)!=Iv[1] )
            u0.redim(n0Total,Iv[1]); // put aggregate values here 
        MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to receive all messages
        int i=0;  // counts total number of values recieved (dimension 0)
        for(int m=0; m<numReceive; m++  )
        {
            int bufSize=receiveStatus[m].MPI_TAG;
            int p = receiveStatus[m].MPI_SOURCE;
            assert( p>=0 && p<np );
//       if( debug!=0 )
//       {
// 	fprintf(debugFile,">>> myid=%i: received buffer of size %i from p=%i (m=%i,numReceive=%i)\n myid=%i: buff=",
//             myid,bufSize,p,m,numReceive,myid);
// 	for( int j=0; j<bufSize; j++ ) fprintf(debugFile,"%3.1f ",rBuff[m][j]);
// 	fprintf(debugFile,"\n");
//       }
      // assign vLocal(rBox) = rBuff[m][0...]
            const double *buff = rBuff[m];
            assert( bufSize % n1 == 0);
            int nd0 = bufSize/n1;
            const int i1a=Iv[1].getBase(), i1b=Iv[1].getBound();
            const int i0a=0, i0b=nd0-1;
            for( int i0=i0a; i0<=i0b; i0++)
            {
                for( int i1=i1a; i1<=i1b; i1++)
      	{
        	  u0(i,i1)=buff[i0+nd0*(i1)]; 
      	}
      	i++;
            }
        }
        assert( i==n0Total );
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
//     if( debug!=0 )
//     {
//       fprintf(debugFile,"+++ myid=%i: wait for all messges to be sent, numSend=%i\n",myid,numSend); 
//       fflush(debugFile);
//     }
        MPI_Status *sendStatus = new MPI_Status[numSend]; 
        MPI_Waitall( numSend, sendRequest, sendStatus );   
        delete [] sendStatus;
    }
//   if( debug!=0 )
//   {
//     fprintf(debugFile,"+++ myid=%i: cleanup buffers...\n",myid); 
//     fflush(debugFile);
//   }
    for (int i=0; i<numReceive; i++ )
    {
        delete [] rBuff[i];
    }
    delete [] rBuff;
    delete [] receiveStatus;
    delete [] receiveRequest;
    for (int i=0; i<numSend; i++ )
    {
        delete [] sBuff[i];
    }
    delete [] sBuff;
    delete [] sendRequest;
#else
  //   Overture::abort("CopyArray::getAggregateArray:ERROR:this function should only be called in parallel");
    u0.redim(Iv[0],Iv[1]);
    u0=u(Iv[0],Iv[1]);
#endif
}

// getAggregateArrayMacro(intSerialArray,int,MPI_INT);
void CopyArray::
getAggregateArray( intSerialArray & u, Index *Iv, intSerialArray & u0, int p0)
// ====================================================================================
//   Combine the values from local arrays to form an aggregate array on processor p0
// 
//  Aggregate over dimension 0:
//        u0(i,Iv[1]) = [u_0(j,Iv[1]), u_1(j,Iv[1]), u_2(j,Iv[1]), ... ]   
// 
//  /u (input) : source arrays
//  /Iv[0..1] : data to copy is u(Iv[0],Iv[1])
//  /u0 (output) : destination array on processor p0
//  /p0 (input) : rank of the destination processor.
// ===========================================================================
{
#ifdef USE_PPP
    const int myid = Communication_Manager::My_Process_Number;
    const int np=max(1,Communication_Manager::Number_Of_Processors);
//const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    int n0 = Iv[0].length(); // number of entries (dimension 0)
    int n0Total=ParallelUtility::getSum(n0);  // total number of entries (dimension 0)
    int n1=Iv[1].length();  // this value is assumed to be the same on all processors
    int numValues=n0*n1;
    int maxNumPerProc=ParallelUtility::getMaxValue(numValues); 
//   int numValues = Iv[0].length()*Iv[1].length()*Iv[2].length()*Iv[3].length();
//   int maxNumPerProc=getMaxValue(numValues); 
//   int totalNum=getSum(numValues); 
  // for now expect a message from every processor:  (could optimize this)
    const int numReceive=myid==p0 ? np : 0;   // number of messages to receive on this proc.
    int **rBuff=NULL;   // buffers for receiving data
    MPI_Request *receiveRequest=NULL;
    MPI_Status *receiveStatus=NULL;
    if( numReceive>0 )
    {
    // post receives first
        receiveRequest= new MPI_Request[numReceive]; // remember to delete these
        receiveStatus= new MPI_Status[numReceive]; 
        rBuff = new int* [numReceive];
        for(int m=0; m<numReceive; m++ )
        {
            int bufSize=maxNumPerProc; // expect at most this many values
            rBuff[m]= new int [bufSize];
            MPI_Irecv(rBuff[m],bufSize,MPI_INT,m,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[m] );
        }
    }
  // Now send the data
    const int numSend=1; // for now every processor sends a message
    MPI_Request *sendRequest=NULL;
    int **sBuff=NULL;
    if( numSend>0 )
    {
    // send data
        sendRequest= new MPI_Request[numSend]; 
        sBuff = new int* [numSend];
        for(int m=0; m<numSend; m++ )
        {
            int bufSize=numValues; 
            sBuff[m]= new int [bufSize];
            int *buff=sBuff[m];
            int i=0; 
            const int i1a=Iv[1].getBase(), i1b=Iv[1].getBound();
            const int i0a=Iv[0].getBase(), i0b=Iv[0].getBound();
            for( int i1=i1a; i1<=i1b; i1++)
            for( int i0=i0a; i0<=i0b; i0++)
            {
      	buff[i]=u(i0,i1);  
      	i++;
            }
//       if( debug!=0 )
//       {
// 	fprintf(debugFile,"<<< myid=%i: send buffer of size %i to p=%i\n",myid,bufSize,pSendBox.processor);
//       }
            MPI_Isend(sBuff[m],bufSize,MPI_INT,p0,bufSize,MPI_COMM_WORLD,&sendRequest[m] );
        }
    }
    if( numReceive>0 )
    {
    // *wdh* 060523 -- only redimension if the existing dimensions are not correct
        if( u0.dimension(0)!=Range(n0Total) || u0.dimension(1)!=Iv[1] )
            u0.redim(n0Total,Iv[1]); // put aggregate values here 
        MPI_Waitall( numReceive, receiveRequest, receiveStatus );  // wait to receive all messages
        int i=0;  // counts total number of values recieved (dimension 0)
        for(int m=0; m<numReceive; m++  )
        {
            int bufSize=receiveStatus[m].MPI_TAG;
            int p = receiveStatus[m].MPI_SOURCE;
            assert( p>=0 && p<np );
//       if( debug!=0 )
//       {
// 	fprintf(debugFile,">>> myid=%i: received buffer of size %i from p=%i (m=%i,numReceive=%i)\n myid=%i: buff=",
//             myid,bufSize,p,m,numReceive,myid);
// 	for( int j=0; j<bufSize; j++ ) fprintf(debugFile,"%3.1f ",rBuff[m][j]);
// 	fprintf(debugFile,"\n");
//       }
      // assign vLocal(rBox) = rBuff[m][0...]
            const int *buff = rBuff[m];
            assert( bufSize % n1 == 0);
            int nd0 = bufSize/n1;
            const int i1a=Iv[1].getBase(), i1b=Iv[1].getBound();
            const int i0a=0, i0b=nd0-1;
            for( int i0=i0a; i0<=i0b; i0++)
            {
                for( int i1=i1a; i1<=i1b; i1++)
      	{
        	  u0(i,i1)=buff[i0+nd0*(i1)]; 
      	}
      	i++;
            }
        }
        assert( i==n0Total );
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
//     if( debug!=0 )
//     {
//       fprintf(debugFile,"+++ myid=%i: wait for all messges to be sent, numSend=%i\n",myid,numSend); 
//       fflush(debugFile);
//     }
        MPI_Status *sendStatus = new MPI_Status[numSend]; 
        MPI_Waitall( numSend, sendRequest, sendStatus );   
        delete [] sendStatus;
    }
//   if( debug!=0 )
//   {
//     fprintf(debugFile,"+++ myid=%i: cleanup buffers...\n",myid); 
//     fflush(debugFile);
//   }
    for (int i=0; i<numReceive; i++ )
    {
        delete [] rBuff[i];
    }
    delete [] rBuff;
    delete [] receiveStatus;
    delete [] receiveRequest;
    for (int i=0; i<numSend; i++ )
    {
        delete [] sBuff[i];
    }
    delete [] sBuff;
    delete [] sendRequest;
#else
  //   Overture::abort("CopyArray::getAggregateArray:ERROR:this function should only be called in parallel");
    u0.redim(Iv[0],Iv[1]);
    u0=u(Iv[0],Iv[1]);
#endif
}




/// \brief Synchronize (set an MPI barrier)
void ParallelUtility::
Sync( MPI_Comm a_comm )
{
#ifdef USE_PPP
      int return_code = MPI_Barrier( a_comm );
      assert( return_code == MPI_SUCCESS );
#endif
}
