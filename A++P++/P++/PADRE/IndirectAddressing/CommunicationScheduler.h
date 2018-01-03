#ifndef __COMMUNICATION_SCHEDULER__
#define __COMMUNICATION_SCHEDULER__

#ifdef HAVE_CONFIG_H
#include <INDIRECT_ADDRESSING_config.h>
#endif

#include "A++.h"
#include "CommunicationSchedule.h"

#if defined(STLPORT) || defined(__DECCXX) || defined(__KCC) || (__SUNPRO_CC>=0x500)
#include <list>
#if !defined (STLPORT) || defined(__STL_USE_NAMESPACES)|| (__SUNPRO_CC>=0x500)
using namespace std;
#endif

#else // use P++ STL
#include "list.h"
#define USING_PPP_STL 
#endif

#if defined(USING_PPP_STL) || (__SUNPRO_CC<0x500)

typedef map<int,list< vector<int> >,less<int> > MapOfListOfVectorInt;
typedef map<int,list<int>,less<int> > MapOfListOfInt;

#else

typedef map<int,list< vector<int> > > MapOfListOfVectorInt;
typedef map<int,list<int> > MapOfListOfInt;

#endif

template<class dataT,class arrayT>
class CommunicationScheduler
{
#if(__SUNPRO_CC>=0x500)
  typedef map<int , dataT* , less<int>, allocator<dataT*> > MapOfDataT;
#else
  typedef map<int , dataT* , less<int> > MapOfDataT;
#endif


  public:
  
  CommunicationScheduler();
  ~CommunicationScheduler();
  CommunicationScheduler( const CommunicationScheduler &inScheduler);

  void
    computeScheduleForTequalsBofJ(arrayT &inT, const arrayT &inB);

  void
    computeScheduleForAofIequalsT(arrayT &inA, const arrayT &inT);

  CommunicationSchedule<dataT>
    scatterIndicesToArray( const arrayT &inA , arrayT &inViewA );
  
  void
    executeScheduleForTequalsBofJ(const intArray *inJ);
  
  void
    executeScheduleForAofIequalsT(const intArray *inI);

    
  private:
  bool areIndexArraysMatching( int inNumIndices, const intArray *inI );
  void alignAndDeViewIndexArrays( int inNumIndices, const intArray *inI,
                                  intArray *outI, Partitioning_Type *inPartition);

  int getNumberOfDimensionsOfIndirectAddressedView( const arrayT &inViewA );
  
  arrayT *mRhs;
  arrayT *mLhs;
  intArray *mIndex;
  CommunicationSchedule<dataT> mCommunicationSchedule;
  
  MapOfDataT mReceiveMessageBuffer,mSendMessageBuffer;
  MapOfListOfVectorInt mLightWeightReceiveMap;
  
};

#include "CommunicationScheduler.C"


#endif
