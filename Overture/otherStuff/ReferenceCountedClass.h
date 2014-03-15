/*  -*-Mode: c++; -*-  */
#ifndef REFERENCE_COUNTED_CLASS
#define REFERENCE_COUNTED_CLASS "ReferenceCountedClass.h"

//===================================================================
//  ReferenceCountedClass
//
//  This class demonstrates a simple example of a reference counted class
//
//==================================================================
#include "ReferenceTypes.h"          // define intR floatR doubleR
typedef floatR realR;
typedef float  real;

class ReferenceCountedClass : public ReferenceCounting    // derive the class from ReferenceCounting
{
 public:
  intR i;                           // this is a reference counted int
  realR x;                          // this is a reference counted real
  intArray array;                   // A++ arrays are reference counted

  ReferenceCountedClass( );                                                // default constructor
  ~ReferenceCountedClass();                                                // destructor
  ReferenceCountedClass(const ReferenceCountedClass & rcc,                 // copy constructor
                        const CopyType copyType = DEEP );
  ReferenceCountedClass& operator=( const ReferenceCountedClass & rcc );   // assignment operator
  void reference( const ReferenceCountedClass & rcc );                     // reference this object to another
  void breakReference();                                                   // break a reference

 private:
  void initialize();                                                       // used by constructors
  // These are used by list's of ReferenceCounting objects
  virtual void reference( const ReferenceCounting & rcc )
  { ReferenceCountedClass::reference( (ReferenceCountedClass&) rcc ); }
  virtual ReferenceCounting & operator=( const ReferenceCounting & rcc )
  { return ReferenceCountedClass::operator=( (ReferenceCountedClass&) rcc ); }
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP )
  { return ::new ReferenceCountedClass(*this,ct); }  

 protected:
    class RCData : public ReferenceCounting   // this class hold the reference counted data
   {
    public:
     int i;                                   // here is where i is really kept
     real x;                                  // here is where x is really kept
     RCData(); 
     ~RCData();
     RCData& operator=(const RCData & rcc );
    private:
      // These are used by list's of ReferenceCounting objects
      virtual void reference( const ReferenceCounting & rcc )
      { RCData::reference( (RCData&) rcc ); }
      virtual ReferenceCounting & operator=( const ReferenceCounting & rcc )
      { return RCData::operator=( (RCData&) rcc ); }
      virtual ReferenceCounting* virtualConstructor( const CopyType )
      { return ::new RCData(); }  
   };
 protected:
  RCData *rcData;
};  
#endif 
