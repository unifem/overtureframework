#ifndef _Dsk
#define _Dsk

#include "OvertureDefine.h"

//kkc 040415 #include <iostream.h>
//kkc 040415 #include <string.h>
#include OV_STD_INCLUDE(iostream)
#include OV_STD_INCLUDE(string)

#include "aString.H"
#include "Types.h"
#include "DskF.h"
#include "GenericDataBase.h"

// The following definitions will eventually go away.  Avoid using them.
#define findInt      findInteger       // Do not use findInt;
#define findFloat    findReal          // Do not use findFloat;
#define findDouble   findDoubleReal    // Do not use findDouble;
#define locateInt    locateInteger     // Do not use locateInt;
#define locateFloat  locateReal        // Do not use locateFloat;
#define locateDouble locateDoubleReal  // Do not use locateDouble;
#define createInt    createInteger     // Do not use createInt;
#define createFloat  createReal        // Do not use createFloat;
#define createDouble createDoubleReal  // Do not use createDouble;
#define linkInt      linkInteger       // Do not use linkInt;
#define linkFloat    linkReal          // Do not use linkFloat;
#define linkDouble   linkDoubleReal    // Do not use linkDouble;
#define getInt       getInteger        // Do not use getInt;
#define getFloat     getReal           // Do not use getFloat;
#define getDouble    getDoubleReal     // Do not use getDouble;
#define putInt       putInteger        // Do not use putInt;
#define putFloat     putReal           // Do not use putFloat;
#define putDouble    putDoubleReal     // Do not use putDouble;

class Dir: GenericDataBase {
  private:
//  Minimum size for a Dir.
    enum { MinimumSize = 96 };

  public:
//
//  Constants:
//
    enum Constructor { CREATE, FIND, DEFAULT, NONE };

//
//  Constructors:
//
    Dir(const Integer size = MinimumSize);
    Dir(const Dir& x);
    Dir(const aString& filename, const aString& flags);
    Dir(const aString& filename);

//
//  Destructor:
//
    virtual ~Dir();

//
//  Assignment and arithmetic:
//
    virtual  GenericDataBase&
             operator=(const GenericDataBase& x);
    Dir&     operator=(const Dir& x);
    Dir&     operator++();                 // Prefix ++ operator
    Dir&     operator--();                 // Prefix -- operator
    Dir      operator++(int);              // Suffix ++ operator
    Dir      operator--(int);              // Suffix -- operator
    Dir&     operator+=(const Integer i);
    Dir&     operator-=(const Integer i);
    Dir      operator+(const Integer i)   const;
    Dir      operator-(const Integer i)   const;
    Dir      operator[](const Integer i)  const;
    Dir      operator()(const Integer i)  const;
    Integer  operator==(const Dir& x) const;
    Integer  operator!=(const Dir& x) const;

    Integer exists(const aString& name) const; // Check whether an object exists.
    virtual Integer isNull()           const; // Test for a null Dir.
    Integer dim(const aString& name)    const; // The number of array elements.
    char type(const aString& name)      const; // The object type.

//
//  Attach a database file.
//
//  Arguments:  const aString& name
//              const aString& flags Optional; by default, flags = " "
//
    virtual Integer mount(const aString& name, const aString& flags);
            Integer mount(const aString& name);

//
//  Detach a database file from a directory.  This is the inverse of mount().
//
    virtual Integer unmount();

//
//  Copy an object.  See the documentation for dskcpy.
//
//  Arguments:  const aString& toName
//              const Dir&    fromDir
//              const aString& fromName
//              const aString& flags Optional; by default, flags = " "
//
    Integer copy(const aString& toName, const Dir& fromDir,
      const aString& fromName, const aString& flags) const;
    Integer copy(const aString& toName, const Dir& fromDir,
      const aString& fromName) const;

//
//  Release an object.  This is an inverse of create(), link(), find() and
//  locate().  If the object sits in a database file then on the outermost
//  nested release, the object is flushed to the database file and its memory
//  is deallocated.
//
//  Arguments:  const aString& name
//              const aString& flags Optional; by default, flags = " "
//
    Integer release(const aString& name, const aString& flags) const;
    Integer release(const aString& name) const;

//
//  Destroy an object.  This is an inverse of create() and link().
//
//  Arguments:  const aString& name
//              const aString& flags Optional; by default, flags = " "
//
    Integer destroy(const aString& name, const aString& flags) const;
    Integer destroy(const aString& name) const;

//
//  Display an object on standard output.
//
//  Arguments:  const aString& name  Optional; by default, name = "."
//              const aString& flags Optional; by default, flags = " "
//
    Integer display(const aString& name, const aString& flags) const;
    Integer display(const aString& name) const;
    Integer display() const;

//
//  Check the consistency of the DSK data structure.
//  Optionally, display a memory map and/or statistics.
//
//  Arguments:  const aString& flags Optional; by default, flags = " "
//
    Integer check(const aString& flags) const;
    Integer check() const;
//
//  Flush all data that reside in writeable database files.
//
    void flush() const;

//
//  Declare member functions findType(name) for each type.
//
//  Arguments:  const aString& name
//
    Complex&       findComplex       (const aString& name) const;
    Integer&       findInteger       (const aString& name) const;
    Logical&       findLogical       (const aString& name) const;
    Pointer&       findPointer       (const aString& name) const;
    DoubleReal&    findDoubleReal    (const aString& name) const;
    Real&          findReal          (const aString& name) const;
    DoubleComplex& findDoubleComplex (const aString& name) const;
    char&          findChar          (const aString& name) const;
    Dir            findDir           (const aString& name) const;

//
//  Declare member functions findType(array,name) for each A++ array type.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
    Integer&    findInteger    (IntegerArray&    array,
                                const aString& name) const;
    Logical&    findLogical    (LogicalArray&    array,
                                const aString& name) const;
    Pointer&    findPointer    (PointerArray&    array,
                                const aString& name) const;
    Real&       findReal       (RealArray&       array,
                                const aString& name) const;
#ifndef DOUBLE
    DoubleReal& findDoubleReal (DoubleRealArray& array,
                                const aString& name) const;
#endif // DOUBLE

    virtual Integer find(GenericDataBase& db,
                         const aString& name) const;
    virtual Integer find(GenericDataBase& db,
                         const aString& name,
                         const aString& dirClassName) const;
    virtual Integer find(aString *name,
                         const aString& dirClassName,
                         const Integer& maxNumber,
                         Integer& actualNumber) const;
    virtual Integer find(GenericDataBase *db,
                         aString *name,
                         const aString& dirClassName,
                         const Integer& maxNumber,
                         Integer& actualNumber) const;
//
//  Declare member functions find(array,name) for some A++ array types.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
    Integer&    find(IntegerArray&    array, const aString& name) const;
    Real&       find(RealArray&       array, const aString& name) const;
#ifndef DOUBLE
    DoubleReal& find(DoubleRealArray& array, const aString& name) const;
#endif // DOUBLE

//
//  Declare member functions locateType(name) for each type.
//
//  Arguments:  const aString& name
//
    Complex&       locateComplex       (const aString& name) const;
    Integer&       locateInteger       (const aString& name) const;
    Logical&       locateLogical       (const aString& name) const;
    Pointer&       locatePointer       (const aString& name) const;
    DoubleReal&    locateDoubleReal    (const aString& name) const;
    Real&          locateReal          (const aString& name) const;
    DoubleComplex& locateDoubleComplex (const aString& name) const;
    char&          locateChar          (const aString& name) const;
    Dir            locateDir           (const aString& name) const;

//
//  Declare member functions locateType(array,name) for each A++ array type.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
    Integer&    locateInteger    (IntegerArray&    array,
                                  const aString& name) const;
    Logical&    locateLogical    (LogicalArray&    array,
                                  const aString& name) const;
    Pointer&    locatePointer    (PointerArray&    array,
                                  const aString& name) const;
    Real&       locateReal       (RealArray&       array,
                                  const aString& name) const;
#ifndef DOUBLE
    DoubleReal& locateDoubleReal (DoubleRealArray& array,
                                  const aString& name) const;
#endif // DOUBLE

    virtual Integer locate(GenericDataBase& db,
                           const aString& name) const;
    virtual Integer locate(GenericDataBase& db,
                           const aString& name,
                           const aString& dirClassName) const;
//
//  Declare member functions locate(array,name) for some A++ array types.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
    Integer&    locate(IntegerArray&    array, const aString& name) const;
    Real&       locate(RealArray&       array, const aString& name) const;
#ifndef DOUBLE
    DoubleReal& locate(DoubleRealArray& array, const aString& name) const;
#endif // DOUBLE

//
//  Declare member functions createType(name,flags,ip) for each type.
//
//  Arguments:  const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//
//  Returned value:
//              Type&               Reference to first array element
//          or  Dir                 Directory, returned by value
//
    Complex&       createComplex       (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    Complex&       createComplex       (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    Complex&       createComplex       (const aString& name,
                                        const aString& flags) const;
    Complex&       createComplex       (const aString& name,
                                        const Integer& ip) const;
    Complex&       createComplex       (const aString& name,
                                        const Integer* ip) const;
    Complex&       createComplex       (const aString& name) const;
    Integer&       createInteger       (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    Integer&       createInteger       (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    Integer&       createInteger       (const aString& name,
                                        const aString& flags) const;
    Integer&       createInteger       (const aString& name,
                                        const Integer& ip) const;
    Integer&       createInteger       (const aString& name,
                                        const Integer* ip) const;
    Integer&       createInteger       (const aString& name) const;
    Logical&       createLogical       (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    Logical&       createLogical       (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    Logical&       createLogical       (const aString& name,
                                        const aString& flags) const;
    Logical&       createLogical       (const aString& name,
                                        const Integer& ip) const;
    Logical&       createLogical       (const aString& name,
                                        const Integer* ip) const;
    Logical&       createLogical       (const aString& name) const;
    Pointer&       createPointer       (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    Pointer&       createPointer       (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    Pointer&       createPointer       (const aString& name,
                                        const aString& flags) const;
    Pointer&       createPointer       (const aString& name,
                                        const Integer& ip) const;
    Pointer&       createPointer       (const aString& name,
                                        const Integer* ip) const;
    Pointer&       createPointer       (const aString& name) const;
    DoubleReal&    createDoubleReal    (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    DoubleReal&    createDoubleReal    (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    DoubleReal&    createDoubleReal    (const aString& name,
                                        const aString& flags) const;
    DoubleReal&    createDoubleReal    (const aString& name,
                                        const Integer& ip) const;
    DoubleReal&    createDoubleReal    (const aString& name,
                                        const Integer* ip) const;
    DoubleReal&    createDoubleReal    (const aString& name) const;
    Real&          createReal          (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    Real&          createReal          (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    Real&          createReal          (const aString& name,
                                        const aString& flags) const;
    Real&          createReal          (const aString& name,
                                        const Integer& ip) const;
    Real&          createReal          (const aString& name,
                                        const Integer* ip) const;
    Real&          createReal          (const aString& name) const;
    DoubleComplex& createDoubleComplex (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    DoubleComplex& createDoubleComplex (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    DoubleComplex& createDoubleComplex (const aString& name,
                                        const aString& flags) const;
    DoubleComplex& createDoubleComplex (const aString& name,
                                        const Integer& ip) const;
    DoubleComplex& createDoubleComplex (const aString& name,
                                        const Integer* ip) const;
    DoubleComplex& createDoubleComplex (const aString& name) const;
    Dir            createDir           (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    Dir            createDir           (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    Dir            createDir           (const aString& name,
                                        const aString& flags) const;
    Dir            createDir           (const aString& name,
                                        const Integer& ip) const;
    Dir            createDir           (const aString& name,
                                        const Integer* ip) const;
    Dir            createDir           (const aString& name) const;
    char&          createChar          (const aString& name,
                                        const aString& flags,
                                        const Integer& ip) const;
    char&          createChar          (const aString& name,
                                        const aString& flags,
                                        const Integer* ip) const;
    char&          createChar          (const aString& name,
                                        const aString& flags) const;
    char&          createChar          (const aString& name,
                                        const Integer& ip) const;
    char&          createChar          (const aString& name,
                                        const Integer* ip) const;
    char&          createChar          (const aString& name) const;

//
//  Declare member functions createType(array,name,flags,ip)
//  for each A++ array type.
//
//  Arguments:  TypeArray&     array On return, uses allocated memory
//              const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//
//  Returned value:
//              Type&               Reference to first array element
//
    Integer& createInteger        (IntegerArray& array, const aString& name,
                                   const aString& flags, const Integer& ip)
                                   const;
    Integer& createInteger        (IntegerArray& array, const aString& name,
                                   const aString& flags, const Integer* ip)
                                   const;
    Integer& createInteger        (IntegerArray& array, const aString& name,
                                   const aString& flags) const;
    Integer& createInteger        (IntegerArray& array, const aString& name,
                                   const Integer& ip) const;
    Integer& createInteger        (IntegerArray& array, const aString& name,
                                   const Integer* ip) const;
    Integer& createInteger        (IntegerArray& array, const aString& name)
                                   const;
    Logical& createLogical        (LogicalArray& array, const aString& name,
                                   const aString& flags, const Integer& ip)
                                   const;
    Logical& createLogical        (LogicalArray& array, const aString& name,
                                   const aString& flags, const Integer* ip)
                                   const;
    Logical& createLogical        (LogicalArray& array, const aString& name,
                                   const aString& flags) const;
    Logical& createLogical        (LogicalArray& array, const aString& name,
                                   const Integer& ip) const;
    Logical& createLogical        (LogicalArray& array, const aString& name,
                                   const Integer* ip) const;
    Logical& createLogical        (LogicalArray& array, const aString& name)
                                   const;
    Pointer& createPointer        (PointerArray& array, const aString& name,
                                   const aString& flags, const Integer& ip)
                                   const;
    Pointer& createPointer        (PointerArray& array, const aString& name,
                                   const aString& flags, const Integer* ip)
                                   const;
    Pointer& createPointer        (PointerArray& array, const aString& name,
                                   const aString& flags) const;
    Pointer& createPointer        (PointerArray& array, const aString& name,
                                   const Integer& ip) const;
    Pointer& createPointer        (PointerArray& array, const aString& name,
                                   const Integer* ip) const;
    Pointer& createPointer        (PointerArray& array, const aString& name)
                                   const;
    Real&    createReal           (RealArray&    array, const aString& name,
                                   const aString& flags, const Integer& ip)
                                   const;
    Real&    createReal           (RealArray&    array, const aString& name,
                                   const aString& flags, const Integer* ip)
                                   const;
    Real&    createReal           (RealArray&    array, const aString& name,
                                   const aString& flags) const;
    Real&    createReal           (RealArray&    array, const aString& name,
                                   const Integer& ip) const;
    Real&    createReal           (RealArray&    array, const aString& name,
                                   const Integer* ip) const;
    Real&    createReal           (RealArray&    array, const aString& name)
                                   const;
#ifndef DOUBLE
    DoubleReal&  createDoubleReal (DoubleRealArray&  array, const aString& name,
                                   const aString& flags, const Integer& ip)
                                   const;
    DoubleReal&  createDoubleReal (DoubleRealArray&  array, const aString& name,
                                   const aString& flags, const Integer* ip)
                                   const;
    DoubleReal&  createDoubleReal (DoubleRealArray&  array, const aString& name,
                                   const aString& flags) const;
    DoubleReal&  createDoubleReal (DoubleRealArray&  array, const aString& name,
                                   const Integer& ip) const;
    DoubleReal&  createDoubleReal (DoubleRealArray&  array, const aString& name,
                                   const Integer* ip) const;
    DoubleReal&  createDoubleReal (DoubleRealArray&  array, const aString& name)
                                   const;
#endif // DOUBLE

    virtual Integer create(GenericDataBase& db, const aString& name);
    virtual Integer create(GenericDataBase& db, const aString& name,
                           const aString& dirClassName);
//
//  Declare member functions create(array,name,flags,ip)
//  for some A++ array types.
//
//  Arguments:  TypeArray&     array On return, uses allocated memory
//              const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//
//  Returned value:
//              Type&               Reference to first array element
//
    Integer&     create (IntegerArray& array, const aString& name,
                         const aString& flags, const Integer& ip) const;
    Integer&     create (IntegerArray& array, const aString& name,
                         const aString& flags, const Integer* ip) const;
    Integer&     create (IntegerArray& array, const aString& name,
                         const aString& flags) const;
    Integer&     create (IntegerArray& array, const aString& name,
                         const Integer& ip) const;
    Integer&     create (IntegerArray& array, const aString& name,
                         const Integer* ip) const;
    Integer&     create (IntegerArray& array, const aString& name) const;
    Real&        create (RealArray&    array, const aString& name,
                         const aString& flags, const Integer& ip) const;
    Real&        create (RealArray&    array, const aString& name,
                         const aString& flags, const Integer* ip) const;
    Real&        create (RealArray&    array, const aString& name,
                         const aString& flags) const;
    Real&        create (RealArray&    array, const aString& name,
                         const Integer& ip) const;
    Real&        create (RealArray&    array, const aString& name,
                         const Integer* ip) const;
    Real&        create (RealArray&    array, const aString& name) const;
#ifndef DOUBLE
    DoubleReal&  create (DoubleRealArray& array, const aString& name,
                         const aString& flags, const Integer& ip) const;
    DoubleReal&  create (DoubleRealArray& array, const aString& name,
                         const aString& flags, const Integer* ip) const;
    DoubleReal&  create (DoubleRealArray& array, const aString& name,
                         const aString& flags) const;
    DoubleReal&  create (DoubleRealArray& array, const aString& name,
                         const Integer& ip) const;
    DoubleReal&  create (DoubleRealArray& array, const aString& name,
                         const Integer* ip) const;
    DoubleReal&  create (DoubleRealArray& array, const aString& name) const;
#endif // DOUBLE

//
//  Declare member functions linkType(name,flags,ip,loc) for each type.
//
//  Arguments:  const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//              Type& loc
//          or  Type* loc
//
//  Returned value:
//              Type&               Reference to first array element
//          or  Dir                 Directory, returned by value
//
    Complex& linkComplex             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Complex*       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Complex&       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Complex*       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Complex&       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const aString& flags,
                                      Complex*       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const aString& flags,
                                      Complex&       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const Integer& ip,
                                      Complex*       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const Integer& ip,
                                      Complex&       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const Integer* ip,
                                      Complex*       loc) const;
    Complex& linkComplex             (const aString& name,
                                      const Integer* ip,
                                      Complex&       loc) const;
    Complex& linkComplex             (const aString& name,
                                      Complex*       loc) const;
    Complex& linkComplex             (const aString& name,
                                      Complex&       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Integer*       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Integer&       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Integer*       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Integer&       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const aString& flags,
                                      Integer*       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const aString& flags,
                                      Integer&       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const Integer& ip,
                                      Integer*       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const Integer& ip,
                                      Integer&       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const Integer* ip,
                                      Integer*       loc) const;
    Integer& linkInteger             (const aString& name,
                                      const Integer* ip,
                                      Integer&       loc) const;
    Integer& linkInteger             (const aString& name,
                                      Integer*       loc) const;
    Integer& linkInteger             (const aString& name,
                                      Integer&       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Logical*       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Logical&       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Logical*       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Logical&       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const aString& flags,
                                      Logical*       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const aString& flags,
                                      Logical&       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const Integer& ip,
                                      Logical*       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const Integer& ip,
                                      Logical&       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const Integer* ip,
                                      Logical*       loc) const;
    Logical& linkLogical             (const aString& name,
                                      const Integer* ip,
                                      Logical&       loc) const;
    Logical& linkLogical             (const aString& name,
                                      Logical*       loc) const;
    Logical& linkLogical             (const aString& name,
                                      Logical&       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Pointer*       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Pointer&       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Pointer*       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Pointer&       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const aString& flags,
                                      Pointer*       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const aString& flags,
                                      Pointer&       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const Integer& ip,
                                      Pointer*       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const Integer& ip,
                                      Pointer&       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const Integer* ip,
                                      Pointer*       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      const Integer* ip,
                                      Pointer&       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      Pointer*       loc) const;
    Pointer& linkPointer             (const aString& name,
                                      Pointer&       loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      DoubleReal*    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      DoubleReal&    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      DoubleReal*    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      DoubleReal&    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const aString& flags,
                                      DoubleReal*    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const aString& flags,
                                      DoubleReal&    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const Integer& ip,
                                      DoubleReal*    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const Integer& ip,
                                      DoubleReal&    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const Integer* ip,
                                      DoubleReal*    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      const Integer* ip,
                                      DoubleReal&    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      DoubleReal*    loc) const;
    DoubleReal&    linkDoubleReal    (const aString& name,
                                      DoubleReal&    loc) const;
    Real&          linkReal          (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Real*          loc) const;
    Real&          linkReal          (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      Real&          loc) const;
    Real&          linkReal          (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Real*          loc) const;
    Real&          linkReal          (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      Real&          loc) const;
    Real&          linkReal          (const aString& name,
                                      const aString& flags,
                                      Real*          loc) const;
    Real&          linkReal          (const aString& name,
                                      const aString& flags,
                                      Real&          loc) const;
    Real&          linkReal          (const aString& name,
                                      const Integer& ip,
                                      Real*          loc) const;
    Real&          linkReal          (const aString& name,
                                      const Integer& ip,
                                      Real&          loc) const;
    Real&          linkReal          (const aString& name,
                                      const Integer* ip,
                                      Real*          loc) const;
    Real&          linkReal          (const aString& name,
                                      const Integer* ip,
                                      Real&          loc) const;
    Real&          linkReal          (const aString& name,
                                      Real*          loc) const;
    Real&          linkReal          (const aString& name,
                                      Real&          loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      DoubleComplex* loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      DoubleComplex& loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      DoubleComplex* loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      DoubleComplex& loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const aString& flags,
                                      DoubleComplex* loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const aString& flags,
                                      DoubleComplex& loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const Integer& ip,
                                      DoubleComplex* loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const Integer& ip,
                                      DoubleComplex& loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const Integer* ip,
                                      DoubleComplex* loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      const Integer* ip,
                                      DoubleComplex& loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      DoubleComplex* loc) const;
    DoubleComplex& linkDoubleComplex (const aString& name,
                                      DoubleComplex& loc) const;
    char&          linkChar          (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      char*          loc) const;
    char&          linkChar          (const aString& name,
                                      const aString& flags, const Integer& ip,
                                      char&          loc) const;
    char&          linkChar          (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      char*          loc) const;
    char&          linkChar          (const aString& name,
                                      const aString& flags, const Integer* ip,
                                      char&          loc) const;
    char&          linkChar          (const aString& name,
                                      const aString& flags,
                                      char*          loc) const;
    char&          linkChar          (const aString& name,
                                      const aString& flags,
                                      char&          loc) const;
    char&          linkChar          (const aString& name,
                                      const Integer& ip,
                                      char*          loc) const;
    char&          linkChar          (const aString& name,
                                      const Integer& ip,
                                      char&          loc) const;
    char&          linkChar          (const aString& name,
                                      const Integer* ip,
                                      char*          loc) const;
    char&          linkChar          (const aString& name,
                                      const Integer* ip,
                                      char&          loc) const;
    char&          linkChar          (const aString& name,
                                      char*          loc) const;
    char&          linkChar          (const aString& name,
                                      char&          loc) const;

//
//  Declare member functions linkType(TypeArray,name,flags,ip,loc)
//  for each A++ array type.
//
//  Arguments:  TypeArray&     array On return, uses allocated memory
//              const aString&  name
//              const aString&  flags Optional; by default, flags = " D* (*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//              Type& loc
//          or  Type* loc
//
//  Returned value:
//              Type&               Reference to first array element
//
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Integer*      loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Integer&      loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Integer*      loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Integer&      loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const aString& flags,
                                Integer*       loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const aString& flags,
                                Integer&       loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const Integer& ip,
                                Integer*       loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const Integer& ip,
                                Integer&       loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const Integer* ip,
                                Integer*       loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                const Integer* ip,
                                Integer&       loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                Integer*      loc) const;
    Integer&    linkInteger    (IntegerArray&     array, const aString& name,
                                Integer&      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Logical*      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Logical&      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Logical*      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Logical&      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const aString& flags,
                                Logical*      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const aString& flags,
                                Logical&      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const Integer& ip,
                                Logical*      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const Integer& ip,
                                Logical&      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const Integer* ip,
                                Logical*      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                const Integer* ip,
                                Logical&      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                Logical*      loc) const;
    Logical&    linkLogical    (LogicalArray&     array, const aString& name,
                                Logical&      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Pointer*      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Pointer&      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Pointer*      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Pointer&      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const aString& flags,
                                Pointer*      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const aString& flags,
                                Pointer&      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const Integer& ip,
                                Pointer*      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const Integer& ip,
                                Pointer&      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                const Integer* ip,
                                Pointer*      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                 const Integer* ip,
                                 Pointer&      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                 Pointer*      loc) const;
    Pointer&    linkPointer    (PointerArray&     array, const aString& name,
                                Pointer&      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Real*      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const aString& flags, const Integer& ip,
                                Real&      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Real*      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const aString& flags, const Integer* ip,
                                Real&      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const aString& flags,
                                Real*      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const aString& flags,
                                Real&      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const Integer& ip,
                                Real*      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const Integer& ip,
                                Real&      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const Integer* ip,
                                Real*      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                const Integer* ip,
                                Real&      loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                Real*       loc) const;
    Real&       linkReal       (RealArray&        array, const aString& name,
                                Real&       loc) const;
#ifndef DOUBLE
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const aString& flags, const Integer& ip,
                                DoubleReal* loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const aString& flags, const Integer& ip,
                                DoubleReal& loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const aString& flags, const Integer* ip,
                                DoubleReal* loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const aString& flags, const Integer* ip,
                                DoubleReal& loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const aString& flags,
                                DoubleReal* loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const aString& flags,
                                DoubleReal& loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const Integer& ip,
                                DoubleReal* loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const Integer& ip,
                                DoubleReal& loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const Integer* ip,
                                DoubleReal* loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                const Integer* ip,
                                DoubleReal& loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                DoubleReal*  loc) const;
    DoubleReal& linkDoubleReal (DoubleRealArray&  array, const aString& name,
                                DoubleReal&  loc) const;
#endif // DOUBLE

//
//  Declare member functions link(TypeArray,name,flags,ip,loc)
//  for some A++ array types.
//
//  Arguments:  TypeArray&     array On return, uses allocated memory
//              const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//              Type& loc
//              Type*     loc
//
//  Returned value:
//              Type&               Reference to first array element
//
    Integer&    link (IntegerArray&     array, const aString& name,
                      const aString& flags, const Integer& ip,
                      Integer*     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const aString& flags, const Integer& ip,
                      Integer&     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const aString& flags, const Integer* ip,
                      Integer*     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const aString& flags, const Integer* ip,
                      Integer&     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const aString& flags,
                      Integer*     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const aString& flags,
                      Integer&     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const Integer& ip,
                      Integer*     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const Integer& ip,
                      Integer&     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const Integer* ip,
                      Integer*     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      const Integer* ip,
                      Integer&     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      Integer*     loc) const;
    Integer&    link (IntegerArray&     array, const aString& name,
                      Integer&     loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const aString& flags, const Integer& ip,
                      Real*        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const aString& flags, const Integer& ip,
                      Real&        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const aString& flags, const Integer* ip,
                      Real*        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const aString& flags, const Integer* ip,
                      Real&        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const aString& flags,
                      Real*        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const aString& flags,
                      Real&        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const Integer& ip,
                      Real*        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const Integer& ip,
                      Real&        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const Integer* ip,
                      Real*        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      const Integer* ip,
                      Real&        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      Real*        loc) const;
    Real&       link (RealArray&        array, const aString& name,
                      Real&        loc) const;
#ifndef DOUBLE
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const aString& flags, const Integer& ip,
                      DoubleReal*  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const aString& flags, const Integer& ip,
                      DoubleReal&  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const aString& flags, const Integer* ip,
                      DoubleReal*  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const aString& flags, const Integer* ip,
                      DoubleReal&  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const aString& flags,
                      DoubleReal*  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const aString& flags,
                      DoubleReal&  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const Integer& ip,
                      DoubleReal*  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const Integer& ip,
                      DoubleReal&  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const Integer* ip,
                      DoubleReal*  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      const Integer* ip,
                      DoubleReal&  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      DoubleReal*  loc) const;
    DoubleReal& link (DoubleRealArray&  array, const aString& name,
                      DoubleReal&  loc) const;
#endif // DOUBLE

//
//  Declare member functions getType(x,name,flags,ip) for each type.
//
//  Arguments:  const Type*   x
//          or  const Type&   x
//              const aString& name
//
//  Returned value:
//              Integer              Error code
//
    Integer getComplex       (const Complex*       x, const aString& name) const;
    Integer getComplex       (const Complex&       x, const aString& name) const;
    Integer getInteger       (const Integer*       x, const aString& name) const;
    Integer getInteger       (const Integer&       x, const aString& name) const;
    Integer getLogical       (const Logical*       x, const aString& name) const;
    Integer getLogical       (const Logical&       x, const aString& name) const;
    Integer getPointer       (const Pointer*       x, const aString& name) const;
    Integer getPointer       (const Pointer&       x, const aString& name) const;
    Integer getDoubleReal    (const DoubleReal*    x, const aString& name) const;
    Integer getDoubleReal    (const DoubleReal&    x, const aString& name) const;
    Integer getReal          (const Real*          x, const aString& name) const;
    Integer getReal          (const Real&          x, const aString& name) const;
    Integer getDoubleComplex (const DoubleComplex* x, const aString& name) const;
    Integer getDoubleComplex (const DoubleComplex& x, const aString& name) const;
    Integer getChar          (const char*          x, const aString& name) const;
    Integer getChar          (const char&          x, const aString& name) const;

//
//  Declare member functions get(x,name) for some types.
//
//  Arguments:  Type&         x
//              const aString& name
//
    virtual Integer get (Complex&       x, const aString& name) const;
    virtual Integer get (Integer&       x, const aString& name) const;
    virtual Integer get (DoubleReal&    x, const aString& name) const;
    virtual Integer get (Real&          x, const aString& name) const;
    virtual Integer get (DoubleComplex& x, const aString& name) const;
    virtual Integer get (aString&        x, const aString& name) const;
    virtual Integer get (aString         x[], const aString& name,
                                        const Integer numberOfStrings) const;

//  This function will disappear soon:
    Integer get (char*          x, const aString& name) const;

//
//  Declare member functions get(array,name) for some A++ array types.
//
//  Arguments:  TypeArray&    array
//              const aString& name
//
    virtual Integer get (IntegerArray&    array, const aString& name) const;
    virtual Integer get (RealArray&       array, const aString& name) const;
#ifndef DOUBLE
    virtual Integer get (DoubleRealArray& array, const aString& name) const;
#endif // DOUBLE

//
//  Declare member functions putType(x,name,flags,ip) for each type.
//
//  Arguments:  const Type*    x
//          or  const Type&    x
//              const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//
//  Returned value:
//              Integer              Error code
//
    Integer putComplex       (const Complex*       x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putComplex       (const Complex&       x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putComplex       (const Complex*       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putComplex       (const Complex&       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putComplex       (const Complex*       x, const aString& name,
                              const aString& flags) const;
    Integer putComplex       (const Complex&       x, const aString& name,
                              const aString& flags) const;
    Integer putComplex       (const Complex*       x, const aString& name,
                              const Integer& ip) const;
    Integer putComplex       (const Complex&       x, const aString& name,
                              const Integer& ip) const;
    Integer putComplex       (const Complex*       x, const aString& name,
                              const Integer* ip) const;
    Integer putComplex       (const Complex&       x, const aString& name,
                              const Integer* ip) const;
    Integer putComplex       (const Complex*       x, const aString& name) const;
    Integer putComplex       (const Complex&       x, const aString& name) const;
    Integer putInteger       (const Integer*           x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putInteger       (const Integer&       x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putInteger       (const Integer*       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putInteger       (const Integer&       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putInteger       (const Integer*       x, const aString& name,
                              const aString& flags) const;
    Integer putInteger       (const Integer&       x, const aString& name,
                              const aString& flags) const;
    Integer putInteger       (const Integer*       x, const aString& name,
                              const Integer& ip) const;
    Integer putInteger       (const Integer&       x, const aString& name,
                              const Integer& ip) const;
    Integer putInteger       (const Integer*       x, const aString& name,
                              const Integer* ip) const;
    Integer putInteger       (const Integer&       x, const aString& name,
                              const Integer* ip) const;
    Integer putInteger       (const Integer*       x, const aString& name) const;
    Integer putInteger       (const Integer&       x, const aString& name) const;
    Integer putLogical       (const Logical*       x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putLogical       (const Logical&       x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putLogical       (const Logical*       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putLogical       (const Logical&       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putLogical       (const Logical*       x, const aString& name,
                              const aString& flags) const;
    Integer putLogical       (const Logical&       x, const aString& name,
                              const aString& flags) const;
    Integer putLogical       (const Logical*       x, const aString& name,
                              const Integer& ip) const;
    Integer putLogical       (const Logical&       x, const aString& name,
                              const Integer& ip) const;
    Integer putLogical       (const Logical*       x, const aString& name,
                              const Integer* ip) const;
    Integer putLogical       (const Logical&       x, const aString& name,
                              const Integer* ip) const;
    Integer putLogical       (const Logical*       x, const aString& name) const;
    Integer putLogical       (const Logical&       x, const aString& name) const;
    Integer putPointer       (const Pointer*       x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putPointer       (const Pointer&       x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putPointer       (const Pointer*       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putPointer       (const Pointer&       x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putPointer       (const Pointer*       x, const aString& name,
                              const aString& flags) const;
    Integer putPointer       (const Pointer&       x, const aString& name,
                              const aString& flags) const;
    Integer putPointer       (const Pointer*       x, const aString& name,
                              const Integer& ip) const;
    Integer putPointer       (const Pointer&       x, const aString& name,
                              const Integer& ip) const;
    Integer putPointer       (const Pointer*       x, const aString& name,
                              const Integer* ip) const;
    Integer putPointer       (const Pointer&       x, const aString& name,
                              const Integer* ip) const;
    Integer putPointer       (const Pointer*       x, const aString& name) const;
    Integer putPointer       (const Pointer&       x, const aString& name) const;
    Integer putDoubleReal    (const DoubleReal*    x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putDoubleReal    (const DoubleReal&    x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putDoubleReal    (const DoubleReal*    x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putDoubleReal    (const DoubleReal&    x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putDoubleReal    (const DoubleReal*    x, const aString& name,
                              const aString& flags) const;
    Integer putDoubleReal    (const DoubleReal&    x, const aString& name,
                              const aString& flags) const;
    Integer putDoubleReal    (const DoubleReal*    x, const aString& name,
                              const Integer& ip) const;
    Integer putDoubleReal    (const DoubleReal&    x, const aString& name,
                              const Integer& ip) const;
    Integer putDoubleReal    (const DoubleReal*    x, const aString& name,
                              const Integer* ip) const;
    Integer putDoubleReal    (const DoubleReal&    x, const aString& name,
                              const Integer* ip) const;
    Integer putDoubleReal    (const DoubleReal*    x, const aString& name) const;
    Integer putDoubleReal    (const DoubleReal&    x, const aString& name) const;
    Integer putReal          (const Real*          x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putReal          (const Real&          x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putReal          (const Real*          x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putReal          (const Real&          x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putReal          (const Real*          x, const aString& name,
                              const aString& flags) const;
    Integer putReal          (const Real&          x, const aString& name,
                              const aString& flags) const;
    Integer putReal          (const Real*          x, const aString& name,
                              const Int& ip) const;
    Integer putReal          (const Real&          x, const aString& name,
                              const Integer& ip) const;
    Integer putReal          (const Real*          x, const aString& name,
                              const Integer* ip) const;
    Integer putReal          (const Real&          x, const aString& name,
                              const Integer* ip) const;
    Integer putReal          (const Real*          x, const aString& name) const;
    Integer putReal          (const Real&          x, const aString& name) const;
    Integer putDoubleComplex (const DoubleComplex* x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putDoubleComplex (const DoubleComplex& x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putDoubleComplex (const DoubleComplex* x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putDoubleComplex (const DoubleComplex& x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putDoubleComplex (const DoubleComplex* x, const aString& name,
                              const aString& flags) const;
    Integer putDoubleComplex (const DoubleComplex& x, const aString& name,
                              const aString& flags) const;
    Integer putDoubleComplex (const DoubleComplex* x, const aString& name,
                              const Integer& ip) const;
    Integer putDoubleComplex (const DoubleComplex& x, const aString& name,
                              const Integer& ip) const;
    Integer putDoubleComplex (const DoubleComplex* x, const aString& name,
                              const Integer* ip) const;
    Integer putDoubleComplex (const DoubleComplex& x, const aString& name,
                              const Integer* ip) const;
    Integer putDoubleComplex (const DoubleComplex* x, const aString& name) const;
    Integer putDoubleComplex (const DoubleComplex& x, const aString& name) const;
    Integer putChar          (const char*          x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putChar          (const char&          x, const aString& name,
                              const aString& flags, const Integer& ip) const;
    Integer putChar          (const char*          x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putChar          (const char&          x, const aString& name,
                              const aString& flags, const Integer* ip) const;
    Integer putChar          (const char*          x, const aString& name,
                              const aString& flags) const;
    Integer putChar          (const char&          x, const aString& name,
                              const aString& flags) const;
    Integer putChar          (const char*          x, const aString& name,
                              const Integer& ip) const;
    Integer putChar          (const char&          x, const aString& name,
                              const Integer& ip) const;
    Integer putChar          (const char*          x, const aString& name,
                              const Integer* ip) const;
    Integer putChar          (const char&          x, const aString& name,
                              const Integer* ip) const;
    Integer putChar          (const char*          x, const aString& name) const;
    Integer putChar          (const char&          x, const aString& name) const;

//
//  Declare member functions put(x,name) for some types.
//
//  Arguments:  const Type&   x  (for types other than char)
//           or const char*   x
//              const aString& name
//
    virtual Integer put (const Complex&       x, const aString& name);
    virtual Integer put (const Integer&       x, const aString& name);
    virtual Integer put (const DoubleReal&    x, const aString& name);
    virtual Integer put (const Real&          x, const aString& name);
    virtual Integer put (const DoubleComplex& x, const aString& name);
    virtual Integer put (const aString&        x, const aString& name);
    virtual Integer put (const aString         x[], const aString& name,
                                              const Integer numberOfStrings);

//
//  Declare member functions put(array,name) for some A++ array types.
//
//  Arguments:  const TypeArray& array
//              const aString&    name
//
    virtual Integer put (const IntegerArray&    array, const aString& name);
    virtual Integer put (const RealArray&       array, const aString& name);
#ifndef DOUBLE
    virtual Integer put (const DoubleRealArray& array, const aString& name);
#endif // DOUBLE

//
//  This constructor should be private, but it is needed for
//  now in order to create a Dir from a Fortran DSK directory:
//
    Dir(Integer* disk, const Integer& dir);

//
//  These conversions are needed for passing disk_ and dir_ to Fortran:
//
    inline operator Complex*()       const { return       (Complex*)_disk; }
    inline operator Integer*()       const { return       (Integer*)_disk; }
//  inline operator Logical*()       const { return       (Logical*)_disk; }
//  inline operator Pointer*()       const { return       (Pointer*)_disk; }
    inline operator DoubleReal*()    const { return    (DoubleReal*)_disk; }
    inline operator Real*()          const { return          (Real*)_disk; }
    inline operator char*()          const { return          (char*)_disk; }
    inline operator DoubleComplex*() const { return (DoubleComplex*)_disk; }
    inline operator const Integer&() const { return _dir; }

  private:
//  Root block structure:
    enum {     dsksiz = 0, autodp = dsksiz + 1, freept = autodp + 1,
      freep2 = freept + 1, goodpt = freep2 + 1, goodp2 = goodpt + 1,
      spacep = goodp2 + 1, space2 = spacep + 1, lckque = space2 + 1,
      lckqu2 = lckque + 1, retlev = lckqu2 + 1, logcod = retlev + 1,
      actcod = logcod + 1, mountt = actcod + 1, iomode = mountt + 1,
      distpt = iomode + 1, mapptr = distpt + 1, symptr = mapptr + 1,
      snccnt = symptr + 1, sncrc1 = snccnt + 1, sncrc2 = sncrc1 + 1,
      sncsd1 = sncrc2 + 1, sncsd2 = sncsd1 + 1, contx0 = sncsd2 + 1,
      contxt = contx0 + 1, conmod = contxt + 1, consch = conmod + 1,
      bigblk = consch + 1, refcnt = bigblk + 1, rootx2 = refcnt + 1,
      rootx3 = rootx2 + 1, rootx4 = rootx3 + 1};

//  Routine name codes.
    enum {     cpberr = 1, cpserr = cpberr + 1, cpyerr = cpserr + 1,
      dcberr = cpyerr + 1, dcserr = dcberr + 1, deferr = dcserr + 1,
      delerr = deferr + 1, dferr  = delerr + 1, dfderr = dferr  + 1,
      dimerr = dfderr + 1, dlkerr = dimerr + 1, dnferr = dlkerr + 1,
      drlerr = dnferr + 1, dswerr = drlerr + 1, fdserr = dswerr + 1,
      fflerr = fdserr + 1, fnderr = fflerr + 1, inierr = fnderr + 1,
      ldserr = inierr + 1, locerr = ldserr + 1, lnkerr = locerr + 1,
      mnterr = lnkerr + 1, nfoerr = mnterr + 1, outerr = nfoerr + 1,
      relerr = outerr + 1, sncerr = relerr + 1, swperr = sncerr + 1,
      umterr = swperr + 1, apderr = umterr + 1, direrr = apderr + 1};

//  Error message codes.
    enum {     erszlt = 1, erfplt = erszlt + 1, erfpgt = erfplt + 1,
      ergplt = erfpgt + 1, ergpgt = ergplt + 1, erdrlt = ergpgt + 1,
      erdrgt = erdrlt + 1, erlnlt = erdrgt + 1, erdacn = erlnlt + 1,
      erdmle = erdacn + 1, ercnal = erdmle + 1, erlcng = ercnal + 1,
      erlcnb = erlcng + 1, erlcnd = erlcnb + 1, erloid = erlcnd + 1,
      erbdnf = erloid + 1, ercndd = erbdnf + 1, ernnfd = ercndd + 1,
      erdine = ernnfd + 1, ersits = erdine + 1, ercnld = ersits + 1,
      ersbsw = ercnld + 1, erntsd = ersbsw + 1, erfioo = erntsd + 1,
      eruniv = erfioo + 1, erunam = eruniv + 1, erdnmp = erunam + 1,
      erlcnz = erdnmp + 1, erptng = erlcnz + 1, erbsng = erptng + 1,
      erbtng = erbsng + 1, erlwng = erbtng + 1, erdsng = erlwng + 1,
      eroinl = erdsng + 1, erabnl = eroinl + 1, ericfl = erabnl + 1,
      erbiib = ericfl + 1, erwrro = erbiib + 1, errlng = erwrro + 1,
      ernbng = errlng + 1, erbbdf = ernbng + 1, erioxt = erbbdf + 1,
      ercndm = erioxt + 1, eramod = ercndm + 1, ernblz = eramod + 1,
                           ernble = ernblz + 1, erdswl = ernble + 1,
      erdmp1 = erdswl + 1, erdmpo = erdmp1 + 1, erdm12 = erdmpo + 1,
      erdm1n = erdm12 + 1, erovdr = erdm1n + 1, erov0r = erovdr + 1,
      erirdm = erov0r + 1, erir12 = erirdm + 1, eraddi = erir12 + 1,
      eradnb = eraddi + 1, eradna = eradnb + 1, eradnc = eradna + 1,
      eradwl = eradnc + 1, eradnv = eradwl + 1, eradnd = eradnv + 1,
      eradnm = eradnd + 1, ermpnf = eradnm + 1, ermpwk = ermpnf + 1,
      ersons = ermpwk + 1, ercpns = ersons + 1, ercpst = ercpns + 1,
      ermxdm = ercpst + 1, erbnor = ermxdm + 1, erlndm = erbnor + 1,
      erlbic = erlndm + 1, erldic = erlbic + 1, erlric = erldic + 1,
      erloic = erlric + 1, erlpic = erloic + 1, erstrd = erlpic + 1,
      erstrp = erstrd + 1, erstrb = erstrp + 1, erstrr = erstrb + 1,
      erstro = erstrr + 1, erirod = erstro + 1, erirgd = erirod + 1,
      erixad = erirgd + 1, ershas = erixad + 1, ergstf = ershas + 1,
      ertyfl = ergstf + 1, erflps = ertyfl + 1, ernits = erflps + 1};

    Integer* _disk;
    Integer  _dir;

    char* truncate(char *string, Integer len) const;
    Integer message(const Integer& ierr) const;
    Integer find(const char* name) const;
    Integer locate(const char* name) const;
    Integer create(const char* name, const char type,
      const char* flags, const Integer& ip) const;
    Integer link(const char* name, const char type, const char* flags,
      const Integer& ip, const Integer& loc) const;
};

#ifdef INLINE
//
// Include inline the member functions of class Dir.
//
#include "Dsk.C"
#endif // INLINE

#endif // _Dsk
