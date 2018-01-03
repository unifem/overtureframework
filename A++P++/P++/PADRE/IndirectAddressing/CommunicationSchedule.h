#ifndef __COMMUNICATION_SCHEDULE__
#define __COMMUNICATION_SCHEDULE__

#ifdef HAVE_CONFIG_H
#include <INDIRECT_ADDRESSING_config.h>
#endif

#include "A++.h"

#if defined(STLPORT) || defined(__DECCXX) || defined(KAI) || (__SUNPRO_CC>=0x500)
#include <list>
#include <map>
#include <vector>
#if !defined (STLPORT) || defined(__STL_USE_NAMESPACES) || (__SUNPRO_CC>=0x500)
using namespace std;
#endif

#else
#include "map.h"
#include "list.h"
#include "vector.h"

#endif
#include "CommunicationScheduleRecord.h"

typedef list<vector<int> > ListOfVectorInts;
typedef map<int, int, less<int> > ProcessorIndexedIntMap;
typedef map<int, CommunicationScheduleRecord*, less<int> > ProcessorIndexedScheduleDataMap;

template<class T>
class CommunicationSchedule
{
  public:
  
  CommunicationSchedule();
  ~CommunicationSchedule();
  CommunicationSchedule(const CommunicationSchedule<T> &inCommSched);
  
/*   void addSendRecord( const CommunicationScheduleRecord &inRecord); */
/*   void addReceiveRecord( const CommunicationScheduleRecord &inRecord); */

  void createNewReceiveRecord( int inProc, const SerialArray_Domain_Type &inIndexDomain,
                               const ListOfVectorInts &inIndexList);
  void createNewSendRecord( int inProc, const SerialArray_Domain_Type &inIndexDomain,
                            const ListOfVectorInts &inIndexList);

  //
  // turn off non-const versions to avoid any ambiguous behavior
  //
/*   CommunicationScheduleRecord& getReceiveRecord( int inProc ); */
/*   CommunicationScheduleRecord& getSendRecord( int inProc ); */

  const CommunicationScheduleRecord& getReceiveRecord( int inProc ) const;
  const CommunicationScheduleRecord& getSendRecord( int inProc ) const;

  const ProcessorIndexedScheduleDataMap& getReceiveData() const;
  const ProcessorIndexedScheduleDataMap& getSendData() const;

/*   const ProcessorIndexedIntMap& getReceiveSizeMap() const; */
/*   const ProcessorIndexedIntMap& getSendSizeMap() const; */
  
  private:
  
/*   ProcessorIndexedIntMap mReceiveMessageSize, mSendMessageSize; */
  ProcessorIndexedScheduleDataMap mReceiveData, mSendData;
  int mType; 
  int mHash;
  int mReferenceCount;
  int mIDNumber;
  
};

template<class T>
ostream& operator<<(ostream &s, const CommunicationSchedule<T> &c);

#include "CommunicationSchedule.C"

#endif
