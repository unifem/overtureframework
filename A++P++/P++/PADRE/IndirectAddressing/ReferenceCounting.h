#ifndef _ReferenceCounting
#define _ReferenceCounting

//
// Who to blame:  Geoff Chesshire
//

#include <iostream.h>
#include <stdlib.h>

#if defined(STLPORT) || defined(__DECCXX) || defined(__KCC) ||  (__SUNPRO_CC>=0x500)
#define USE_STD_STRING 1
#else
#define USE_STD_STRING 0
#include "bool.h"
#endif

#if USE_STD_STRING
#include <string>
#else
#include <string.h>
#endif


#if defined(__STL_USE_NAMESPACES)||defined(__KCC)|| (__SUNPRO_CC>=0x500)
using namespace std;
#endif


#if USE_STD_STRING
typedef basic_string<char,char_traits<char>,allocator<char> > aString;
#else
typedef char* aString;
#endif

/* #include "OvertureTypes.h" */
enum CopyType { DEEP, SHALLOW, NOCOPY };

class ReferenceCounting {
  public:
    inline ReferenceCounting() {
        className = "ReferenceCounting";
        globalID = incrementGlobalIDCounter();
        referenceCount = uncountedReferences = 0;
    }
    inline ReferenceCounting
      (const ReferenceCounting& x, const CopyType ct = DEEP) {
        className = "ReferenceCounting";
        globalID = incrementGlobalIDCounter();
        referenceCount = uncountedReferences = 0;
        if (&x || &ct);
    }
    inline virtual ~ReferenceCounting() {
        if (referenceCount) {
            cerr << "ReferenceCounting::~ReferenceCounting():  "
                 << "referenceCount != 0 for globalID = "
                 << getGlobalID() << "." << endl;
            exit(1);
        } // end if
    }
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { if (&x); return *this; }
    inline virtual void reference(const ReferenceCounting& x) { if (&x); }
    inline virtual void breakReference() { }
    inline virtual ReferenceCounting* virtualConstructor
      (const CopyType ct = DEEP) const
      { return new ReferenceCounting(*this, ct); }
    inline long incrementReferenceCount()
      { return ++referenceCount + uncountedReferences; }
    inline long decrementReferenceCount()
      { return --referenceCount + uncountedReferences; }
    inline long getReferenceCount() const
      { return referenceCount + uncountedReferences; }
    inline bool uncountedReferencesMayExist() {
        if (!referenceCount) uncountedReferences = 1;
        return uncountedReferences;
    }
    inline virtual aString getClassName() const { return className; }
    
    long getGlobalID() const { return globalID; }
    virtual void consistencyCheck() const;
  private:
    long referenceCount;
    bool uncountedReferences;
    long globalID;
    aString className;
    static long globalIDCounter;
    static long incrementGlobalIDCounter();
};
ostream& operator<<(ostream& s, const ReferenceCounting& x);

#endif // _ReferenceCounting
