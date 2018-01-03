#ifndef __COMMUNICATION_SCHEDULE_RECORD__
#define __COMMUNICATION_SCHEDULE_RECORD__

#ifdef HAVE_CONFIG_H
#include <INDIRECT_ADDRESSING_config.h>
#endif

#include "A++.h"

#ifndef USE_PADRE
#include "ReferenceCounting.h"  // defines intR, floatR, doubleR (references to int, float, double)

class CommunicationScheduleRecord : public ReferenceCounting  //derive from ReferenceCounting
{ 
  public:
  CommunicationScheduleRecord();
  
  CommunicationScheduleRecord(const SerialArray_Domain_Type &inTemplateDomain,
                              intSerialArray* inArray0, int inArrayDim);
  
  CommunicationScheduleRecord( const CommunicationScheduleRecord &inRecord,
                               const CopyType copyType = DEEP );
  
  ~CommunicationScheduleRecord();

  CommunicationScheduleRecord & operator=(const CommunicationScheduleRecord &rlhs);

  void reference( const CommunicationScheduleRecord &inRecord);

  void breakReference();
  
/*   void initializeRecord( const list<int> &inListOfIndices ); */

  bool getIsInitialized()const;

  const SerialArray_Domain_Type& getIndexDomain()const;
  
  private:
  void initialize(); // used by constructors

  void initialize(const SerialArray_Domain_Type &inTemplateDomain,
                  intSerialArray *inArray0, int inArrayDim); // used by constructors

  // the next functions are needed if this object is put onto Lists
  // of ReferenceCounted items
  virtual void reference( const ReferenceCounting &inRcc)
    { CommunicationScheduleRecord::reference( (CommunicationScheduleRecord&)inRcc); }
  
  virtual ReferenceCounting& operator=( const ReferenceCounting &inRcc)
  { return CommunicationScheduleRecord::operator=( (CommunicationScheduleRecord&)inRcc); }
  
  virtual ReferenceCounting* virtualConstructor( const CopyType inCt = DEEP)
    { return ::new CommunicationScheduleRecord(*this, inCt); }

  protected:
  class CSRData : public ReferenceCounting
    {
      public:
      SerialArray_Domain_Type *mIndexDomain;
      bool mIsInitialized;

      CSRData();
      ~CSRData();
      CSRData& operator=(const CSRData &rlhs);

      private:
      virtual void reference( const ReferenceCounting &inRcc )
        { CSRData::reference( (CSRData&) inRcc ); }

      virtual ReferenceCounting& operator=( const ReferenceCounting &inRcc )
      { return CSRData::operator=( (CSRData&) inRcc );}

      virtual ReferenceCounting* virtualConstructor( const CopyType )
        { return ::new CSRData(); }
    };

  protected:
  CSRData *mCSRData;
};

#endif

#endif
