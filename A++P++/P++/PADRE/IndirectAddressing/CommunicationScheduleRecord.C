
#include "CommunicationScheduleRecord.h"

#ifndef USE_PADRE

//\begin{>CommunicationScheduleRecord.tex}{\subsection{Public Member Functions}}
//\no function header:
//\end{CommunicationScheduleRecord.tex}


//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{Constructor}}
CommunicationScheduleRecord::
CommunicationScheduleRecord()
//=================================================================
// /Description:  Default constructor sets the initialization state
//    to false.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  28 June 2000
//\end{CommunicationScheduleRecord.tex}
//
//--ChangeLog------------------------------------------------------
// 10 July 2000 BJM:  Removed initialization of mIsInitialized as 
//    part of making this class ReferenceCounted.
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord default constructor.\n");
#endif
  
  //	
  // call the initialization function for the data
  //
  initialize();
  
  //
  // set the list data
  //
  mCSRData->mIsInitialized = false;
  
  incrementReferenceCount();

#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("     CommunicationScheduleRecord.referenceCount = %d\n",getReferenceCount());
  parallelPrintf("leaving CommunicationScheduleRecord default constructor.\n");
#endif
}



//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{Constructor}}
CommunicationScheduleRecord::
CommunicationScheduleRecord(const SerialArray_Domain_Type &inTemplateDomain,
                            intSerialArray* inArray0, int inArrayDim)
//=================================================================
// /Description:  Thisconstructor sets the initialization state
//    to true and fills the index data.
//
// /inTemplateDomain (input):  The Domain used as a pattern for
//    the internal domain used to hold the indices.
// /inArray0 (input):  The indices we will store in the newly 
//    created SerialArray_Domain_Type.
//
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//
//--ChangeLog------------------------------------------------------
// 10 July 2000 BJM:  Initial revision.
// 13 July 2000 BJM:  modified record to hold a list of int's
//    instead of a list of int's.  The first of the pair will be the
//    reference index, and the second one the value.
// 13 July 2000 BJM:  Realized pair not needed, went back to old way.
// 17 July 2000 bjm:  Back to list of pair<int,int>'s to support
//     different distributions of J,T, and B.
// 19 July 2000 bjm:  AGAIN remove the pair<int,int> from the record
//     since we now assume that the values in T are the same as the
//     values in J.
// 24 July 2000 bjm:  Replacing list<int> as representation of indices
//     with Array_Domain_Type.
// 25 July 2000 bjm:  Changed constructor parameters to supply the
//     Domain used as a pattern to build our index-holding domain and
//     added a parameter to hold the indices themselves.
// 31 July 2000 bjm:  Replaced Array_Domain_Type representation of
//     indices with SerialArray_Domain_Type.
// 8 Nov. 2000 bjm: Replaced intSerialArray with intSerialArray *
//     to allow passing in of multiple dimensions via array.  Also
//     added inArrayDim to say how many dims of array come in.
// 11 Nov. 2000 bjm:  Switched intSerialArray ** 
//     to vector<intSerialArray>.
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord list constructor.\n");
#endif

  //	
  // call the initialization function for the data
  //
  initialize(inTemplateDomain, inArray0, inArrayDim);
  
  //
  // set the initialized flag
  //
  mCSRData->mIsInitialized = true;

  incrementReferenceCount();
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("leaving CommunicationScheduleRecord list constructor.\n");
#endif

}


//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{Copy Constructor}}
CommunicationScheduleRecord::
CommunicationScheduleRecord( const CommunicationScheduleRecord &inRecord, 
                             const CopyType inCopyType )
//=================================================================
// /Description:  Copy constructor.  Does nothing.
//
// /inRecord (input):  The object to copy.
// /inCopyType (input):  Shallow or deep copy.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  28 June 2000
//\end{CommunicationScheduleRecord.tex}
//
//--ChangeLog------------------------------------------------------
// 10 July 2000 BJM:  added copy constructor code as 
//    part of making this class ReferenceCounted.
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord copy constructor.\n");
#endif

  if( inCopyType == DEEP )
  {
    initialize();
    (*this) = inRecord;
  }
  else
  {
    mCSRData = inRecord.mCSRData;  // copy the pointer to the data.
    mCSRData->incrementReferenceCount();  // increment the reference count for the data
    reference( inRecord );  // reference this object to inRecord.
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
    parallelPrintf("     mCSRData.referenceCount = %d\n",mCSRData->getReferenceCount());
#endif
  }
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("leaving CommunicationScheduleRecord copy constructor.\n");
#endif
  
}


//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{Destructor}}
CommunicationScheduleRecord::
~CommunicationScheduleRecord()
//=================================================================
// /Description:  Destructor.  Does nothing.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  28 June 2000
//\end{CommunicationScheduleRecord.tex}
//
//--ChangeLog------------------------------------------------------
// 10 July 2000 BJM:  added destructor code as 
//    part of making this class ReferenceCounted.
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord destructor.\n");
#endif
  //
  // if there are no references to the data,
  //
  if( mCSRData->decrementReferenceCount() == 0 )
  {
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
    parallelPrintf("     mCSRData.referenceCount = %d\n",mCSRData->getReferenceCount());
#endif
    //
    // delete the data (this may be insufficient leading to a memory leak).
    //
    mCSRData->mIndexDomain->decrementReferenceCount();
    if (mCSRData->mIndexDomain->getReferenceCount() < intSerialArray::getReferenceCountBase())
    {
      delete mCSRData->mIndexDomain;
    }
    delete mCSRData;
  }
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("leaving CommunicationScheduleRecord destructor.\n");
#endif
}


// //\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{initializeRecord}}
// void
// CommunicationScheduleRecord::
// initializeRecord( const list<int> &inListOfIndices )
// //=================================================================
// // /Description:  \textbf{initializeRecord} is the routine that is
// //   called to build a valid CommunicationScheduleRecord.
// //
// // /inListOfIndices (input):  The list holding the indices.
// //
// // /Return Value:  None.
// //
// // /Author:  BJM
// // /Date:  28 June 2000
// //\end{CommunicationScheduleRecord.tex}
// //=================================================================
// {
//   mIndexList = inListOfIndices;

//   mIsInitialized = true;
  
// }



//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{getIsInitialized}}
bool
CommunicationScheduleRecord::
getIsInitialized() const
//=================================================================
// /Description:  \textbf{getIsInitialized} returns whether the 
//    record has been initialized yet.
//
// /Return Value:  Bool indicating initialization state.
//
// /Author:  BJM
// /Date:  28 June 2000
//\end{CommunicationScheduleRecord.tex}
//=================================================================
{
  return mCSRData->mIsInitialized;
}

//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{getIndexList}}
const SerialArray_Domain_Type& 
CommunicationScheduleRecord::
getIndexDomain()const
//=================================================================
// /Description:  \textbf{getIndexList} returns a const reference
//   to the list of integers defining the indices for this record.
//
// /Return Value:  The list of indices for this record.
//
// /Author:  BJM
// /Date:  3 July 2000
//\end{CommunicationScheduleRecord.tex}
//
//--ChangeLog-------------------------------------------------------
// 13 July 2000 BJM:  Modified to return a list of int's.
// 17 July 2000 bjm:  Back to list of pair<int,int>'s to support
//     different distributions of J,T, and B.
//
// 19 July 2000 bjm:  AGAIN remove the pair<int,int> from the record
//     since we now assume that the values in T are the same as the
//     values in J.  
// 24 July 2000 bjm:  Replacing list<int> as representation of indices
//     with Array_Domain_Type.
// 25 July 2000 bjm:  We store a pointer to the index domain so we
//     have to return *(indexDomain).
//
// 31 July 2000 bjm:  Replaced Array_Domain_Type representation of
//     indices with SerialArray_Domain_Type.
//=================================================================
{
  return *(mCSRData->mIndexDomain);
}



//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{reference}}
void
CommunicationScheduleRecord::
reference( const CommunicationScheduleRecord &inRecord)
//=================================================================
// /Description:  \textbf{reference} is the function that references
//   this object to the input one.
//
// /inRecord (input):  The object we wish this to reference.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord reference.\n");
#endif
  if( this == &inRecord )
  {
    return;
  }

  if( mCSRData->decrementReferenceCount() == 0 )
  {
    ::delete mCSRData;
  }
  
  mCSRData = inRecord.mCSRData;
  mCSRData->incrementReferenceCount();
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("leaving CommunicationScheduleRecord reference.\n");
#endif
}



//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{breakReference}}
void
CommunicationScheduleRecord::
breakReference()
//=================================================================
// /Description:  \textbf{breakReference} is the function that breaks
//   the reference from this object and copies the data to this.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord breakReference.\n");
#endif
  // if there is only one reference, no need to make a copy
  if( mCSRData->getReferenceCount() != 1 )
  {
    CommunicationScheduleRecord theRecord = *this; //deep copy
    reference( theRecord );  // make a reference to this new copy
  }
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("leaving CommunicationScheduleRecord breakReference.\n");
#endif
}



//\begin{>>CommunicationScheduleRecord.tex}{\subsection{Private Member Functions}}
//\no function header:
//\end{CommunicationScheduleRecord.tex}

//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{initialize}}
void
CommunicationScheduleRecord::
initialize()
//=================================================================
// /Description:  \textbf{initialize} is the function that assigns
//   initial values to the data variables.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord initialize.\n");
#endif

  mCSRData = new CommunicationScheduleRecord::CSRData;
  
  mCSRData->incrementReferenceCount();
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("     mCSRData.referenceCount = %d\n",mCSRData->getReferenceCount());
  parallelPrintf("leaving CommunicationScheduleRecord initialize.\n");
#endif
  
}

//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{initialize}}
void
CommunicationScheduleRecord::
initialize(const SerialArray_Domain_Type &inTemplateDomain,
           intSerialArray* inArray0, int inArrayDim)
//=================================================================
// /Description:  \textbf{initialize} is the function that assigns
//   initial values to the data variables.  In this case, it builds
//   a new Array_Domain_Type to store in the data, and fills
//   in the indices with those supplied by the input array.
//
// /inTemplateDomain (input):  The Domain used as a pattern for
//    the internal domain used to hold the indices.
// /inArray0 (input):  The indices we will store in the newly 
//    created Array_Domain_Type.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  25 July 2000
//\end{CommunicationScheduleRecord.tex}
//
//--ChangeLog------------------------------------------------------
//
// 31 July 2000 bjm:  Replaced Array_Domain_Type representation of
//     indices with SerialArray_Domain_Type.
// 8 Nov. 2000 bjm: Replaced intSerialArray with intSerialArray *
//     to allow passing in of multiple dimensions via array.  Also
//     added inArrayDim to say how many dims of array come in.
// 11 Nov. 2000 bjm:  Switched intSerialArray ** 
//     to <intSerialArray>.
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord initialize- list version.\n");
#endif

  int myProcessorNumber = Communication_Manager::My_Process_Number;
  
  mCSRData = new CommunicationScheduleRecord::CSRData;
  
  mCSRData->incrementReferenceCount();
  
  int i,dim;
  
  //
  // declare the array holding the indirect indexing pointers
  // and fill them up
  //
  Indirect_Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type theIndexPointer;
  for(i=0;i<MAX_ARRAY_DIMENSION; i++)
  {
    theIndexPointer[i] = new Internal_Indirect_Addressing_Index;
    APP_ASSERT(theIndexPointer[i] != NULL);
  }

#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord initialize(list):  Declared and allocated Internal_Indirect_Addressing_Index's\n");
#endif
  //
  //create one array for the each input index-holding intArray
  // and set it into theIndexPointer array
  //
//   APP_ASSERT( inArray0.numberOfDimensions() == 1 );
  

#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord initialize(list):  The indexData array pointer has been allocated, now we fill it with the data from inArray0.\n");
#endif

  for(dim=0;dim<inArrayDim;dim++)
  {
    //
    // create the intSerialArray to hold the index data
    //
    intSerialArray *theIndexData = new intSerialArray(inArray0[dim].getLength(0) );
    APP_ASSERT(theIndexData != NULL);

    //
    // fill the intSeralArray with the input index information
    //
    int theIndex=(inArray0[dim]).getBase(0);
    for(i=theIndexData->getBase(0);i<=theIndexData->getBound(0);i+=theIndexData->getStride(0),
          theIndex+=(inArray0[dim]).getStride(0))
    {
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
      parallelPrintf("CommunicationScheduleRecord initialize(list):  assigning (theIndexData[%d])(%d) = inArray0[%d](%d)=%d.\n",
                     dim,i,dim,theIndex,(inArray0[dim])(theIndex));
#endif
      (*theIndexData)(i) = (inArray0[dim])(theIndex);
    }

    //
    // set the intSerialArray inside the INdirectAddressing object
    //
    theIndexPointer[dim]->setIntSerialArray( theIndexData );

    //
    // delete the intSerialArray if possible
    //
    APP_ASSERT( theIndexData->getReferenceCount() >= theIndexData->getReferenceCountBase() );
    theIndexData->decrementReferenceCount();
    if(  theIndexData->getReferenceCount() < theIndexData->getReferenceCountBase() )
    {
      delete theIndexData;
    }
  }
  
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord initialize(list):  Completed assigning the inArray0 data to theIndexData and setting this intArray for use as indirection data.\n");
#endif
  

  //
  // theIndexPointer is now filled with the indirect indexing information.
  // we now build the SerialArray_Domain_Type object to hold this data.
  //
  mCSRData->mIndexDomain = new SerialArray_Domain_Type(inTemplateDomain , theIndexPointer );
  APP_ASSERT(mCSRData->mIndexDomain != NULL); 
  
  //
  // delete the arrays holding the indirect indexing pointers
  // and set them to NULL
  //
  for(i=0;i<MAX_ARRAY_DIMENSION; i++)
  {
    theIndexPointer[i]->setIntSerialArray(NULL);
    delete theIndexPointer[i];
    theIndexPointer[i] = NULL;
  }

#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("     mCSRData.referenceCount = %d\n",mCSRData->getReferenceCount());
  parallelPrintf("leaving CommunicationScheduleRecord initialize.\n");
#endif
  
}


//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{operator\=}}
CommunicationScheduleRecord &
CommunicationScheduleRecord::
operator=( const CommunicationScheduleRecord &inRhs )
//=================================================================
// /Description:  \textbf{operator\=} is the deep copy equals
//   operator.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord operator=.\n");
#endif
  // deep copy of the referenced counted data
  mCSRData = inRhs.mCSRData;
  
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("leaving CommunicationScheduleRecord operator=.\n");
#endif
  return *this;
}



//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{constructor}}
CommunicationScheduleRecord::CSRData::
CSRData()
//=================================================================
// /Description:  \textbf{constructor} is the default constructor 
//   for the data container class
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord::CSRData default constructor.\n");
  parallelPrintf("     mCSRData.referenceCount = %d\n",this->getReferenceCount());
  parallelPrintf("leaving CommunicationScheduleRecord::CSRData default constructor.\n");
#endif
}

//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{destructor}}
CommunicationScheduleRecord::CSRData::
~CSRData()
//=================================================================
// /Description:  \textbf{destructor} is the default destructor 
//   for the data container class
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord::CSRData destructor.\n");
  parallelPrintf("     mCSRData.referenceCount = %d\n",this->getReferenceCount());
  parallelPrintf("leaving CommunicationScheduleRecord::CSRData destructor.\n");
#endif

}



//\begin{>>CommunicationScheduleRecord.tex}{\subsubsection{operator\=}}
CommunicationScheduleRecord::CSRData&
CommunicationScheduleRecord::CSRData::
operator=(const CommunicationScheduleRecord::CSRData &inRhs)
//=================================================================
// /Description:  \textbf{operator\=} is the deep copy of the 
//   reference counted for the data container class.
//
// /Return Value:  None.
//
// /Author:  BJM
// /Date:  10 July 2000
//\end{CommunicationScheduleRecord.tex}
//
//--ChangeLog-------------------------------------------------------
// 24 July 2000 bjm:  Replacing list<int> as representation of indices
//     with Array_Domain_Type.
//
// 31 July 2000 bjm:  Replaced Array_Domain_Type representation of
//     indices with SerialArray_Domain_Type.
//=================================================================
{
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("CommunicationScheduleRecord::CSRData operator=.\n");
#endif
  mIndexDomain = inRhs.mIndexDomain;
  mIsInitialized = inRhs.mIsInitialized;
#ifdef INDIRECT_ADDRESSING_INTERNALDEBUG
  parallelPrintf("leaving CommunicationScheduleRecord::CSRData operator=.\n");
#endif
  return *this;
}

#endif
