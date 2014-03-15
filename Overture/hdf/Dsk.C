#ifndef INLINE

#include "Dsk.h"

#define inline // Make "inline" disappear.  This must come after all includes.

#endif // INLINE

inline Dir::Dir(const Integer size) {
    className = "Dir";
    _disk = new Integer[size];
    Integer ierr; dskini_(_disk, size, _dir, ierr); if (ierr) message(ierr);
    // _disk[refcnt] = 1;  This is set by dskini_().
}

inline Dir::Dir(const Dir& x): GenericDataBase(x)
  { className = "Dir"; _disk = x._disk; _dir = x._dir; _disk[refcnt]++; }

inline Dir::Dir(const aString& filename, const aString& flags) {
    className = "Dir";
    _disk = new Integer[MinimumSize];
    Integer ierr;
    dskini_(_disk, MinimumSize, _dir, ierr); if (ierr) message(ierr);
    // _disk[refcnt] = 1;  This is set by dskini_().
    mount(filename, flags);
}

inline Dir::Dir(const aString& filename) {
    className = "Dir";
    _disk = new Integer[MinimumSize];
    Integer ierr;
    dskini_(_disk, MinimumSize, _dir, ierr); if (ierr) message(ierr);
    // _disk[refcnt] = 1;  This is set by dskini_().
    mount(filename);
}

inline Dir::~Dir() {
    if (--_disk[refcnt] == 0)
      { Integer ierr; dskfin_(_disk, ierr); delete [] _disk; }
    className = "";
}

inline GenericDataBase& Dir::operator=(const GenericDataBase& x) {
    if (className != x.className) {
        cout << "Dir::operator=(const GenericDataBase&):  className mismatch."
             << endl;
        exit(1);
    } // end if
    return operator=((const Dir&)x);
}
inline Dir& Dir::operator=(const Dir& x) {
    if (_disk!=x._disk) {
        if (--_disk[refcnt]==0) { destroy(".", " R"); delete [] _disk; }
        _disk = x._disk; _disk[refcnt]++;
    }
    _dir = x._dir; return *this;
}
inline Dir& Dir::operator++() { _dir++; return *this; } // Prefix ++ operator
inline Dir& Dir::operator--() { _dir--; return *this; } // Prefix -- operator
inline Dir  Dir::operator++(int) { _dir++; return Dir(_disk, _dir-1); }// Suffix
inline Dir  Dir::operator--(int) { _dir--; return Dir(_disk, _dir+1); }// Suffix
inline Dir& Dir::operator+=(const Integer i) { _dir += i; return *this; }
inline Dir& Dir::operator-=(const Integer i) { _dir -= i; return *this; }
inline Dir  Dir::operator+(const Integer i) const {return Dir(_disk, _dir + i);}
inline Dir  Dir::operator-(const Integer i) const {return Dir(_disk, _dir - i);}
inline Dir  Dir::operator[](const Integer i) const
  {return Dir(_disk, _dir + i); }
inline Dir  Dir::operator()(const Integer i) const
  {return Dir(_disk, _dir + (i-1)); }
inline Integer Dir::operator==(const Dir& x) const
  { return _disk==x._disk && _dir==x._dir; }
inline Integer Dir::operator!=(const Dir& x) const
  { return _disk!=x._disk || _dir!=x._dir; }

//
//  Check whether an object exists.
//
inline Integer Dir::exists(const aString& name) const
  { return dsklds_(_disk, _dir, name, strlen(name))!=0; }

//
//  Test for a null Dir.
//
inline Integer Dir::isNull() const { return _dir==0; }

//
//  Return the number of array elements.
//
//  Arguments:  const aString& name
//
inline Integer Dir::dim(const aString& name) const
  { return dskdim_(_disk, _dir, name, strlen(name)); }

//
//  Return the object type.
//
//  Arguments:  const aString& name
//
inline char Dir::type(const aString& name) const {
    char temp; const Integer block=0, total=0, len_temp=1; 
    Integer desc, ndims, owner, dims, range, ovrlap, period, blocks, ierr;
    desc = dskfds_(_disk, _dir, name, strlen(name));
    dskdnf_(_disk, desc, block, total, &temp, ndims, owner, dims,
      range, ovrlap, period, blocks, ierr, len_temp);
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    return temp;
}

//
//  Attach a database file.
//
//  Arguments:  const aString& name
//              const aString& flags Optional; by default, flags = " "
//
inline Integer Dir::mount(const aString& name, const aString& flags) {
    Integer unit=999999, ierr;
    dskmnt_(_disk, _dir, name, unit, flags, ierr, strlen(name),
      strlen(flags));
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    return ierr;
}
inline Integer Dir::mount(const aString& name) { return mount(name, " "); }

//
//  Detach a database file from a directory.  This is the inverse of mount().
//
inline Integer Dir::unmount() {
    Integer ierr;
    dskumt_(_disk, _dir, ierr);
    if (ierr) message(ierr);
    return ierr;
}

//
//  Copy an object.  See the documentation for dskcpy.
//
//  Arguments:  const aString& toName
//              const Dir&    fromDir
//              const aString& fromName
//              const aString& flags Optional; by default, flags = " "
//
inline Integer Dir::copy(const aString& toName, const Dir& fromDir,
  const aString& fromName, const aString& flags) const {
    Integer ierr;
    dskcpy_(fromDir._disk, fromDir._dir, fromName, _disk, _dir, toName, flags,
      ierr, strlen(fromName), strlen(toName), strlen(flags));
    if (ierr) cout << "toName = " << toName
                << ",  fromName = " << fromName << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    return ierr;
}
inline Integer Dir::copy(const aString& toName, const Dir& fromDir,
  const aString& fromName) const { return copy(toName, fromDir, fromName, " "); }

//
//  Release an object.  This is an inverse of create(), link(), find() and
//  locate().  If the object sits in a database file then on the outermost
//  nested release, the object is flushed to the database file and its memory
//  is deallocated.
//
//  Arguments:  const aString& name
//              const aString& flags Optional; by default, flags = " "
//
inline Integer Dir::release(const aString& name, const aString& flags) const {
    Integer ierr;
    dskrel_(_disk, _dir, name, flags, ierr, strlen(name), strlen(flags));
    if (ierr==100*relerr+erwrro) ierr = 0; // Ignore writes to read-only files.
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    return ierr;
}
inline Integer Dir::release(const aString& name) const
  { return release(name, " "); }

//
//  Destroy an object.  This is an inverse of create() and link().
//
//  Arguments:  const aString& name
//              const aString& flags Optional; by default, flags = " "
//
inline Integer Dir::destroy(const aString& name, const aString& flags) const {
    Integer ierr;
    dskdel_(_disk, _dir, name, flags, ierr,
      strlen(name), strlen(flags));
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    return ierr;
}
inline Integer Dir::destroy(const aString& name) const
  { return destroy(name, " "); }

//
//  Display an object on standard output.
//
//  Arguments:  const aString& name  Optional; by default, name = "."
//              const aString& flags Optional; by default, flags = " "
//
inline Integer Dir::display(const aString& name, const aString& flags) const {
    const Integer fortranLogicalUnit=6;
    Integer ierr;
    dskout_(_disk, _dir, name, flags, fortranLogicalUnit, ierr,
      strlen(name), strlen(flags));
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    return ierr;
}
inline Integer Dir::display(const aString& name) const
  { return display(name, " "); }
inline Integer Dir::display() const { return display("."); }

//
//  Check the consistency of the DSK data structure.
//  Optionally, display a memory map and/or statistics.
//
//  Arguments:  const aString& flags Optional; by default, flags = " "
//
inline Integer Dir::check(const aString& flags) const {
    const Integer fortranLogicalUnit=6;
    Integer ierr;
    dskdf_(_disk, flags, fortranLogicalUnit, ierr, strlen(flags));
    if (ierr) message(ierr);
    return ierr;
}
inline Integer Dir::check() const { return check(" "); }

//
//  Flush all data that reside in writeable database files.
//
void Dir::flush() const { dskffl_(_disk); }

//
//  This macro is used in several of the member functions below.
//
#define DSK_ADOPT(array,loc,name) if (loc!=NULL) {                             \
    char type; const Integer block=0, total=0, len_type=1;                     \
    Integer desc, ndims, owner, dims, range, ovrlap, period, blocks, ierr;     \
    desc = dskfds_(_disk, _dir, name, strlen(name));                           \
    dskdnf_(_disk, desc, block, total, &type, ndims, owner, dims,              \
      range, ovrlap, period, blocks, ierr, len_type);                          \
    if (ierr) message(ierr); else {                                            \
        Range dimens[4]; for (Integer j=0; j<3; j++)                           \
          dimens[j] = Range(_disk[dims+2*j-1], _disk[dims+2*j]);               \
        Integer n=1; for (j=3; j<ndims; j++)                                   \
          n *= _disk[dims+2*j] - _disk[dims+2*j-1] + 1;                        \
        dimens[3] = Range(_disk[dims+3], _disk[dims+3] + n - 1);               \
        for (j=ndims; j<4; j++) dimens[j] = Range(0, 0);                       \
        array.adopt(loc, dimens[0], dimens[1], dimens[2], dimens[3]);          \
    }                                                                          \
}

//
//  Declare member functions findType(name) for each type.
//
//  Arguments:  const aString& name
//
#define DSK_MACRO(findType, Type, Tflag)                                       \
inline Type& Dir::findType(const aString& name) const {                         \
    Integer iloc = find(name);                                                 \
    Type *loc = iloc ? (Type*)_disk + DSK_LOC(iloc) - 1 : NULL;                \
    if (loc!=NULL && type(name)!=Tflag) {                                      \
        cout << "Error from find(" << name << "):  type mismatch" << endl;     \
        exit(1);                                                               \
    }                                                                          \
    return *loc;                                                               \
}
#define DSK_LOC(loc) loc
DSK_MACRO(findComplex,       Complex,       'c')
DSK_MACRO(findInteger,       Integer,       'i')
DSK_MACRO(findLogical,       Logical,       'l')
DSK_MACRO(findPointer,       Pointer,       'p')
DSK_MACRO(findDoubleReal,    DoubleReal,    'q')
DSK_MACRO(findReal,          Real,          'r')
DSK_MACRO(findDoubleComplex, DoubleComplex, 'z')
#undef DSK_LOC
#define DSK_LOC(loc) (((loc) - 1) * (sizeof(Integer) / sizeof(char)) + 1)
DSK_MACRO(findChar,          char,          's')
#undef DSK_LOC
#undef DSK_MACRO
inline Dir Dir::findDir(const aString& name) const {
    Dir result(_disk, find(name));
    if ((!result.isNull()) && type(name)!='d') {
        cout << "Error from findDir(" << name << "):  type mismatch" << endl;
        exit(1);
    } // end if
    return result;
}

//
//  Declare member functions findType(array,name) for each A++ array type.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
#define DSK_MACRO(findType, Type, TypeArray, Tflag)                            \
inline Type& Dir::findType(TypeArray& array, const aString& name) const {       \
    Type *loc = &findType(name);                                               \
    if (loc!=NULL && type(name)!=Tflag) {                                      \
        cout << "Error from find(" << name << "):  type mismatch" << endl;     \
        exit(1);                                                               \
    }                                                                          \
    DSK_ADOPT(array, loc, name);                                               \
    return *loc;                                                               \
}
DSK_MACRO(findInteger,    Integer,     IntegerArray,     'i')
DSK_MACRO(findLogical,    Logical,     LogicalArray,     'l')
DSK_MACRO(findPointer,    Pointer,     PointerArray,     'p')
#ifndef DOUBLE
DSK_MACRO(findDoubleReal, DoubleReal,  DoubleRealArray,  'q')
#endif // DOUBLE
DSK_MACRO(findReal,       Real,        RealArray,        'r')
#undef DSK_MACRO

Integer Dir::find(GenericDataBase& db, const aString& name) const {
    if (className != db.className) {
        cout << "Dir::find(GenericDataBase&, const aString&):  className mismatch."
             << endl;
        exit(1);
    } // end if
    Dir& dir = (Dir&)db; dir = findDir(name);
    return dir.isNull();
}
Integer Dir::find(GenericDataBase& db, const aString& name,
  const aString& dirClassName) const {
    Dir& dir = (Dir&)db; dir = findDir(name);
    aString dbClassName; dir.get(dbClassName, "className");
    if (dbClassName != dirClassName) {
        cout << "Dir::find(GenericDataBase&, const aString&, const aString&):  dirClassName mismatch."
             << endl;
        exit(1);
    } // end if
    return dir.isNull();
}
Integer Dir::find(aString *name, const aString& dirClassName, 
  const Integer& maxNumber, Integer& actualNumber) const {
    actualNumber = 0;
    return 0;
}
Integer Dir::find(GenericDataBase* db, aString *name, const aString& dirClassName, 
  const Integer& maxNumber, Integer& actualNumber) const {
    actualNumber = 0;
    return 0;
}

//
//  Declare member functions find(array,name) for some A++ array types.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
#define DSK_MACRO(findType, Type, TypeArray, Tflag)                            \
inline Type& Dir::find(TypeArray& array, const aString& name) const {           \
    Type *loc = &findType(name);                                               \
    if (loc!=NULL && type(name)!=Tflag) {                                      \
        cout << "Error from find(" << name << "):  type mismatch" << endl;     \
        exit(1);                                                               \
    }                                                                          \
    DSK_ADOPT(array, loc, name);                                               \
    return *loc;                                                               \
}
DSK_MACRO(findInteger,    Integer,    IntegerArray,    'i')
#ifndef DOUBLE
DSK_MACRO(findDoubleReal, DoubleReal, DoubleRealArray, 'q')
#endif // DOUBLE
DSK_MACRO(findReal,       Real,       RealArray,       'r')
#undef DSK_MACRO

//
//  Declare member functions locateType(name) for each type.
//
//  Arguments:  const aString& name
//
#define DSK_MACRO(locateType, Type, Tflag)                                     \
inline Type& Dir::locateType(const aString& name) const {                       \
    Integer iloc = locate(name);                                               \
    Type *loc = iloc ? (Type*)_disk + DSK_LOC(iloc) - 1 : NULL;                \
    if (loc!=NULL && type(name)!=Tflag) {                                      \
        cout << "Error from locate(" << name << "):  type mismatch" << endl;   \
        exit(1);                                                               \
    }                                                                          \
    return *loc;                                                               \
}
#define DSK_LOC(loc) loc
DSK_MACRO(locateComplex,       Complex,       'c')
DSK_MACRO(locateInteger,       Integer,       'i')
DSK_MACRO(locateLogical,       Logical,       'l')
DSK_MACRO(locatePointer,       Pointer,       'p')
DSK_MACRO(locateDoubleReal,    DoubleReal,    'q')
DSK_MACRO(locateReal,          Real,          'r')
DSK_MACRO(locateDoubleComplex, DoubleComplex, 'z')
#undef DSK_LOC
#define DSK_LOC(loc) (((loc) - 1) * (sizeof(Integer) / sizeof(char)) + 1)
DSK_MACRO(locateChar,          char,          's')
#undef DSK_LOC
#undef DSK_MACRO
inline Dir Dir::locateDir(const aString& name) const {
    Dir result(_disk, locate(name));
    if ((!result.isNull()) && type(name)!='d') {
        cout << "Error from locateDir(" << name << "):  type mismatch" << endl;
        exit(1);
    } // end if
    return result;
}

//
//  Declare member functions locateType(array,name) for each A++ array type.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
#define DSK_MACRO(locateType, Type, TypeArray, Tflag)                          \
inline Type& Dir::locateType(TypeArray& array, const aString& name) const {     \
    Type *loc = &locateType(name);                                             \
    if (loc!=NULL && type(name)!=Tflag) {                                      \
        cout << "Error from locate(" << name << "):  type mismatch" << endl;   \
        exit(1);                                                               \
    }                                                                          \
    DSK_ADOPT(array, loc, name);                                               \
    return *loc;                                                               \
}
DSK_MACRO(locateInteger,    Integer,    IntegerArray,    'i')
DSK_MACRO(locateLogical,    Logical,    LogicalArray,    'l')
DSK_MACRO(locatePointer,    Pointer,    PointerArray,    'p')
#ifndef DOUBLE
DSK_MACRO(locateDoubleReal, DoubleReal, DoubleRealArray, 'q')
#endif // DOUBLE
DSK_MACRO(locateReal,       Real,       RealArray,       'r')
#undef DSK_MACRO

//
//  Declare member functions locate(array,name) for some A++ array types.
//
//  Arguments:  TypeArray&    array On return, uses allocated memory
//              const aString& name
//
#define DSK_MACRO(locateType, Type, TypeArray, Tflag)                          \
inline Type& Dir::locate(TypeArray& array, const aString& name) const {         \
    Type *loc = &locateType(name);                                             \
    if (loc!=NULL && type(name)!=Tflag) {                                      \
        cout << "Error from locate(" << name << "):  type mismatch" << endl;   \
        exit(1);                                                               \
    }                                                                          \
    DSK_ADOPT(array, loc, name);                                               \
    return *loc;                                                               \
}
    DSK_MACRO(locateInteger,    Integer,    IntegerArray,    'i')
#ifndef DOUBLE
    DSK_MACRO(locateDoubleReal, DoubleReal, DoubleRealArray, 'q')
#endif // DOUBLE
    DSK_MACRO(locateReal,       Real,       RealArray,       'r')
#undef DSK_MACRO

//
//  Declare member functions createType(name,flags,ip) for each type.
//
//  Arguments:  const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//
//  Returned value:
//              Type&                Reference to first array element
//          or  Dir                 Directory, returned by value
//
#define DSK_MACRO(createType, Type, Tflag)                                     \
inline Type& Dir::createType(const aString& name, const aString& flags,          \
  const Integer& ip) const {                                                   \
    Type *loc = (Type*)_disk + DSK_LOC(create(name, Tflag, flags, ip)) - 1;    \
    return *loc;                                                               \
}                                                                              \
inline Type& Dir::createType(const aString& name, const aString& flags,          \
  const Integer* ip) const                                                     \
  { return createType(name, flags, *ip); }                                     \
inline Type& Dir::createType(const aString& name, const aString& flags) const    \
  { return createType(name, flags, 0); }                                       \
inline Type& Dir::createType(const aString& name, const Integer& ip) const      \
  { return createType(name, " D*(*)", ip); }                                   \
inline Type& Dir::createType(const aString& name, const Integer* ip) const      \
  { return createType(name, " D*(*)", *ip); }                                  \
inline Type& Dir::createType(const aString& name) const                         \
  { return createType(name, " "); }
#define DSK_LOC(loc) loc
DSK_MACRO(createComplex,       Complex,       'c')
DSK_MACRO(createInteger,       Integer,       'i')
DSK_MACRO(createLogical,       Logical,       'l')
DSK_MACRO(createPointer,       Pointer,       'p')
DSK_MACRO(createDoubleReal,    DoubleReal,    'q')
DSK_MACRO(createReal,          Real,          'r')
DSK_MACRO(createDoubleComplex, DoubleComplex, 'z')
#undef DSK_LOC
#define DSK_LOC(loc) (((loc) - 1) * (sizeof(Integer) / sizeof(char)) + 1)
DSK_MACRO(createChar,          char,          's')
#undef DSK_LOC
#undef DSK_MACRO
inline Dir Dir::createDir(const aString& name, const aString& flags,
  const Integer& ip) const
  { Dir result(_disk, create(name, 'd', flags, ip)); return result; }
inline Dir Dir::createDir(const aString& name, const aString& flags,
  const Integer* ip) const
  { Dir result(_disk, create(name, 'd', flags, *ip)); return result; }
inline Dir Dir::createDir(const aString& name, const aString& flags) const
  { return createDir(name, flags, 0); }
inline Dir Dir::createDir(const aString& name, const Integer& ip) const
  { return createDir(name, " D*(*)", ip); }
inline Dir Dir::createDir(const aString& name, const Integer* ip) const
  { return createDir(name, " D*(*)", *ip); }
Dir Dir::createDir(const aString& name) const { return createDir(name, " "); }

Integer Dir::locate(GenericDataBase& db, const aString& name) const {
    if (className != db.className) {
        cout << "Dir::locate(GenericDataBase&, const aString&):  className mismatch."
             << endl;
        exit(1);
    } // end if
    Dir& dir = (Dir&)db; dir = locateDir(name);
    return dir.isNull();
}
Integer Dir::locate(GenericDataBase& db, const aString& name,
  const aString& dirClassName) const {
    Dir& dir = (Dir&)db; dir = locateDir(name);
    if (!dir.isNull()) {
        aString dbClassName; dir.get(dbClassName, "className");
        if (dbClassName != dirClassName) {
            cout << "Dir::locate(GenericDataBase&, const aString&, const aString&):  dirClassName mismatch."
                 << endl;
            exit(1);
        } // end if
    } // end if
    return dir.isNull();
}

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
//              Type&                Reference to first array element
//
#define DSK_MACRO(createType, Type, TypeArray)                                 \
inline Type& Dir::createType(TypeArray& array, const aString& name,             \
  const aString& flags, const Integer& ip) const {                              \
    Type *loc = &createType(name, flags, ip);                                  \
    DSK_ADOPT(array, loc, name);                                               \
    return *loc;                                                               \
}                                                                              \
inline Type& Dir::createType(TypeArray& array, const aString& name,             \
  const aString& flags, const Integer* ip) const                                \
  { return createType(array, name, flags, *ip); }                              \
inline Type& Dir::createType(TypeArray& array, const aString& name,             \
  const aString& flags) const { return createType(array, name, flags, 0); }     \
inline Type& Dir::createType(TypeArray& array, const aString& name,             \
  const Integer& ip) const                                                     \
  { return createType(array, name, " D*(*)", ip); }                            \
inline Type& Dir::createType(TypeArray& array, const aString& name,             \
  const Integer* ip) const                                                     \
  { return createType(array, name, " D*(*)", *ip); }                           \
inline Type& Dir::createType(TypeArray& array, const aString& name) const       \
  { return createType(array, name, " "); }
DSK_MACRO(createInteger,    Integer,    IntegerArray   )
DSK_MACRO(createLogical,    Logical,    LogicalArray   )
DSK_MACRO(createPointer,    Pointer,    PointerArray   )
#ifndef DOUBLE
DSK_MACRO(createDoubleReal, DoubleReal, DoubleRealArray)
#endif // DOUBLE
DSK_MACRO(createReal,       Real,       RealArray      )
#undef DSK_MACRO

Integer Dir::create(GenericDataBase& db, const aString& name) {
    if (className != db.className) {
        cout << "Dir::create(GenericDataBase&, const aString&):  className mismatch."
             << endl;
        exit(1);
    } // end if
    Dir& dir = (Dir&)db; dir = createDir(name);
    return dir.isNull();
}
Integer Dir::create(GenericDataBase& db, const aString& name,
  const aString& dirClassName) {
    create(db, name);
    Dir& dir = (Dir&)db; dir.put(dirClassName, "className");
    return dir.isNull();
}

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
//              Type&                Reference to first array element
//
#define DSK_MACRO(createType, Type, TypeArray)                                 \
inline Type& Dir::create(TypeArray& array, const aString& name,                 \
  const aString& flags, const Integer& ip) const {                              \
    Type *loc = &createType(name, flags, ip);                                  \
    DSK_ADOPT(array, loc, name);                                               \
    return *loc;                                                               \
}                                                                              \
inline Type& Dir::create(TypeArray& array, const aString& name,                 \
  const aString& flags, const Integer* ip) const                                \
  { return create(array, name, flags, *ip); }                                  \
inline Type& Dir::create(TypeArray& array, const aString& name,                 \
  const aString& flags) const { return create(array, name, flags, 0); }         \
inline Type& Dir::create(TypeArray& array, const aString& name,                 \
  const Integer& ip) const { return create(array, name, " D*(*)", ip); }       \
inline Type& Dir::create(TypeArray& array, const aString& name,                 \
  const Integer* ip) const { return create(array, name, " D*(*)", *ip); }      \
inline Type& Dir::create(TypeArray& array, const aString& name) const           \
  { return create(array, name, " "); }
DSK_MACRO(createInteger,    Integer,    IntegerArray   )
#ifndef DOUBLE
DSK_MACRO(createDoubleReal, DoubleReal, DoubleRealArray)
#endif // DOUBLE
DSK_MACRO(createReal,       Real,       RealArray      )
#undef DSK_MACRO

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
//              Type&                Reference to first array element
//          or  Dir                  Directory, returned by value
//
#define DSK_MACRO(linkType, Type, Tflag)                                       \
inline Type& Dir::linkType(const aString& name, const aString& flags,            \
const Integer& ip, Type* loc) const {                                          \
    Type* l = (link(name, Tflag, flags, ip, DSK_LOC(loc-(Type*)_disk+1))) ?    \
      loc : NULL;                                                              \
    return *l;                                                                 \
}                                                                              \
inline Type& Dir::linkType(const aString& name, const aString& flags,            \
  const Integer& ip, Type& loc) const                                          \
  { return linkType(name, flags, ip, &loc); }                                  \
inline Type& Dir::linkType(const aString& name, const aString& flags,            \
  const Integer* ip, Type* loc) const                                          \
  { return linkType(name, flags, *ip, loc); }                                  \
inline Type& Dir::linkType(const aString& name, const aString& flags,            \
  const Integer* ip, Type& loc) const                                          \
  { return linkType(name, flags, *ip, &loc); }                                 \
inline Type& Dir::linkType(const aString& name, const aString& flags,            \
  Type* loc) const { return linkType(name, flags, 0, loc); }                   \
inline Type& Dir::linkType(const aString& name, const aString& flags,            \
  Type& loc) const { return linkType(name, flags, 0, &loc); }                  \
inline Type& Dir::linkType(const aString& name, const Integer& ip, Type* loc)   \
  const { return linkType(name, " D*(*)", ip, loc); }                          \
inline Type& Dir::linkType(const aString& name, const Integer& ip, Type& loc)   \
  const { return linkType(name, " D*(*)", ip, &loc); }                         \
inline Type& Dir::linkType(const aString& name, const Integer* ip, Type* loc)   \
  const { return linkType(name, " D*(*)", *ip, loc); }                         \
inline Type& Dir::linkType(const aString& name, const Integer* ip, Type& loc)   \
  const { return linkType(name, " D*(*)", *ip, &loc); }                        \
inline Type& Dir::linkType(const aString& name, Type* loc) const                \
  { return linkType(name, " ", loc); }                                         \
inline Type& Dir::linkType(const aString& name, Type& loc) const                \
  { return linkType(name, " ", &loc); }
#define DSK_LOC(loc) loc
DSK_MACRO(linkComplex,       Complex,       'c')
DSK_MACRO(linkInteger,       Integer,       'i')
DSK_MACRO(linkLogical,       Logical,       'l')
DSK_MACRO(linkPointer,       Pointer,       'p')
DSK_MACRO(linkDoubleReal,    DoubleReal,    'q')
DSK_MACRO(linkReal,          Real,          'r')
DSK_MACRO(linkDoubleComplex, DoubleComplex, 'z')
#undef DSK_LOC
#define DSK_LOC(loc) (((loc) - 1) * (sizeof(Integer) / sizeof(char)) + 1)
DSK_MACRO(linkChar,          char,          's')
#undef DSK_LOC
#undef DSK_MACRO

//
//  Declare member functions linkType(TypeArray,name,flags,ip,loc)
//  for each A++ array type.
//
//  Arguments:  TypeArray&     array On return, uses allocated memory
//              const aString&  name
//              const aString&  flags Optional; by default, flags = " D*(*)"
//              const Integer& ip    Optional; by default, ip = 0
//          or  const Integer* ip
//              Type& loc
//          or  Type* loc
//
//  Returned value:
//              Type&                Reference to first array element
//
#define DSK_MACRO(linkType, Type, TypeArray)                                   \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const aString& flags, const Integer& ip, Type* loc) const {                   \
    Type* l = &linkType(name, flags, ip, loc);                                 \
    DSK_ADOPT(array, l, name);                                                 \
    return *l;                                                                 \
}                                                                              \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const aString& flags, const Integer& ip, Type& loc) const                     \
  { return linkType(array, name, flags, ip, &loc); }                           \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const aString& flags, const Integer* ip, Type* loc) const                     \
  { return linkType(array, name, flags, *ip, loc); }                           \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const aString& flags, const Integer* ip, Type& loc) const                     \
  { return linkType(array, name, flags, *ip, &loc); }                          \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const aString& flags, Type* loc) const                                        \
  { return linkType(array, name, flags, 0, loc); }                             \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const aString& flags, Type& loc) const                                        \
  { return linkType(array, name, flags, 0, &loc); }                            \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const Integer& ip, Type* loc) const                                          \
  { return linkType(array, name, " D*(*)", ip, loc); }                         \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const Integer& ip, Type& loc) const                                          \
  { return linkType(array, name, " D*(*)", ip, &loc); }                        \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const Integer* ip, Type* loc) const                                          \
  { return linkType(array, name, " D*(*)", *ip, loc); }                        \
inline Type& Dir::linkType(TypeArray& array, const aString& name,               \
  const Integer* ip, Type& loc) const                                          \
  { return linkType(array, name, " D*(*)", *ip, &loc); }                       \
inline Type& Dir::linkType(TypeArray& array, const aString& name, Type* loc)    \
  const { return linkType(array, name, " ", loc); }                            \
inline Type& Dir::linkType(TypeArray& array, const aString& name, Type& loc)    \
  const { return linkType(array, name, " ", &loc); }
DSK_MACRO(linkInteger,    Integer,    IntegerArray   )
DSK_MACRO(linkLogical,    Logical,    LogicalArray   )
DSK_MACRO(linkPointer,    Pointer,    PointerArray   )
#ifndef DOUBLE
DSK_MACRO(linkDoubleReal, DoubleReal, DoubleRealArray)
#endif // DOUBLE
DSK_MACRO(linkReal,       Real,       RealArray      )
#undef DSK_MACRO

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
//              Type* loc
//
//  Returned value:
//              Type&                Reference to first array element
//
#define DSK_MACRO(linkType, Type, TypeArray)                                   \
inline Type& Dir::link(TypeArray& array, const aString& name,                   \
  const aString& flags, const Integer& ip, Type* loc) const {                   \
    Type* l = &linkType(name, flags, ip, loc);                                 \
    DSK_ADOPT(array, l, name);                                                 \
    return *l;                                                                 \
}                                                                              \
inline Type& Dir::link(TypeArray& array, const aString& name,                   \
  const aString& flags, const Integer& ip, Type& loc) const                     \
  { return link(array, name, flags, ip, &loc); }                               \
inline Type& Dir::link(TypeArray& array, const aString& name,                   \
  const aString& flags, const Integer* ip, Type* loc) const                     \
  { return link(array, name, flags, *ip, loc); }                               \
inline Type& Dir::link(TypeArray& array, const aString& name,                   \
  const aString& flags, const Integer* ip, Type& loc) const                     \
  { return link(array, name, flags, *ip, &loc); }                              \
inline Type& Dir::link(TypeArray& array, const aString& name,                   \
  const aString& flags, Type* loc) const                                        \
  { return link(array, name, flags, 0, loc); }                                 \
inline Type& Dir::link(TypeArray& array, const aString& name,                   \
  const aString& flags, Type& loc) const                                        \
  { return link(array, name, flags, 0, &loc); }                                \
inline Type& Dir::link(TypeArray& array, const aString& name, const Integer& ip,\
  Type* loc) const { return link(array, name, " D*(*)", ip, loc); }            \
inline Type& Dir::link(TypeArray& array, const aString& name, const Integer& ip,\
  Type& loc) const { return link(array, name, " D*(*)", ip, &loc); }           \
inline Type& Dir::link(TypeArray& array, const aString& name, const Integer* ip,\
  Type* loc) const { return link(array, name, " D*(*)", *ip, loc); }           \
inline Type& Dir::link(TypeArray& array, const aString& name, const Integer* ip,\
  Type& loc) const                                                             \
  { return link(array, name, " D*(*)", *ip, &loc); }                           \
inline Type& Dir::link(TypeArray& array, const aString& name, Type* loc)        \
  const { return link(array, name, " ", loc); }                                \
inline Type& Dir::link(TypeArray& array, const aString& name, Type& loc)        \
  const { return link(array, name, " ", &loc); }
DSK_MACRO(linkInteger,    Integer,    IntegerArray   )
#ifndef DOUBLE
DSK_MACRO(linkDoubleReal, DoubleReal, DoubleRealArray)
#endif // DOUBLE
DSK_MACRO(linkReal,       Real,       RealArray      )
#undef DSK_MACRO

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
#define DSK_MACRO(getType, Type, Tflag)                                        \
inline Integer Dir::getType(const Type* x, const aString& name) const {         \
    char type; const Integer block=0, total=0, len_type=1, readit=0;           \
    Integer desc, ndims, owner, dims, range, ovrlap, period, blocks, ierr;     \
    desc = dskfds_(_disk, _dir, name, strlen(name));                           \
    dskdnf_(_disk, desc, block, total, &type, ndims, owner, dims,              \
      range, ovrlap, period, blocks, ierr, len_type);                          \
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;        \
    if (ierr) message(ierr);                                                   \
    if (desc!=0 && type!=Tflag) {                                              \
        cout << "Error from get<type>(" << name << "):  type mismatch" << endl;\
        exit(1);                                                               \
    }                                                                          \
    dskdcb_(_disk, desc, (void*)x, _disk[dims-1], _disk[dims-1], readit, ierr);\
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;        \
    if (ierr) message(ierr);                                                   \
    return ierr;                                                               \
}                                                                              \
inline Integer Dir::getType(const Type& x, const aString& name) const           \
  { return getType(&x, name); }
DSK_MACRO(getComplex,       Complex,      'c')
DSK_MACRO(getInteger,       Integer,      'i')
DSK_MACRO(getLogical,       Logical,      'l')
DSK_MACRO(getPointer,       Pointer,      'p')
DSK_MACRO(getDoubleReal,    DoubleReal,   'q')
DSK_MACRO(getReal,          Real,         'r')
DSK_MACRO(getDoubleComplex, DoubleComplex,'z')
#undef DSK_MACRO
inline Integer Dir::getChar(const char* x, const aString& name) const {
    char type; const Integer block=0, total=0, len_type=1, readit=0;
    Integer desc, ndims, owner, dims, range, ovrlap, period, blocks, ierr;
    desc = dskfds_(_disk, _dir, name, strlen(name));
    dskdnf_(_disk, desc, block, total, &type, ndims, owner, dims,
      range, ovrlap, period, blocks, ierr, len_type);
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    if (desc!=0 && type!='s') {
        cout << "Error from getChar(" << name << "):  type mismatch" << endl;
        exit(1);
    } // end if
    Integer *dims0 = new Integer [2*ndims];
    for (Integer i=0; i<ndims; i++) dims0[i] = _disk[dims+i-1];
    dims0[1] = 4 * _disk[dims];
    dskdcs_(_disk, desc, (char*)x, *dims0, *dims0, readit, ierr,
      dims0[1]); ((char*)x)[dims0[1]] = 0;
    delete [] dims0;
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    return ierr;
}
inline Integer Dir::getChar(const char& x, const aString& name) const
  { return getChar(&x, name); }

//
//  Declare member functions get(x,name) for some types.
//
//  Arguments:  Type&         x
//              const aString& name
//
#define DSK_MACRO(findType, Type)                                              \
inline Integer Dir::get(Type& x, const aString& name) const                     \
  { Type& temp = findType(name); x = temp; release(name); return 0; }
DSK_MACRO(findComplex,       Complex      )
DSK_MACRO(findInteger,       Integer      )
DSK_MACRO(findDoubleReal,    DoubleReal   )
DSK_MACRO(findReal,          Real         )
DSK_MACRO(findDoubleComplex, DoubleComplex)
#undef DSK_MACRO
inline Integer Dir::get(aString& x, const aString& name) const {
    char type; const Integer block=0, total=1, len_type=1;
    Integer desc, ndims, owner, dims, range, ovrlap, period, blocks, ierr;
    desc = dskfds_(_disk, _dir, name, strlen(name));
    dskdnf_(_disk, desc, block, total, &type, ndims, owner, dims,
      range, ovrlap, period, blocks, ierr, len_type);
    if (ierr) {
        cout << "name = " << name << ", ierr = " << ierr << endl;
        message(ierr); x = ""; return ierr;
    } else if (ndims != 1) {
        cout << "get(aString&, const aString&):  Wrong number of dimensions."
             << endl;
        x = ""; return 1;
    }
    char *buffer = new char[dims*4+1]; buffer[dims*4] = '\0';
    getChar(buffer, name); x = buffer; delete [] buffer; return 0;
}
inline int Dir::get(aString x[], const aString& name, const int numberOfStrings)
  const { return 0; }
inline Integer Dir::get(char* x, const aString& name) const {
    cout << "get(char*, const aString&):  This routine will disappear soon."
         << endl
         << "You should use get(aString&, const aString&) instead." << endl;
    getChar(x, name); return 0;
}

//
//  Declare member functions get(array,name) for some A++ array types.
//
//  Arguments:  TypeArray&    array
//              const aString& name
//
#define DSK_MACRO(findType, TypeArray)                                         \
inline Integer Dir::get(TypeArray& array, const aString& name) const {          \
    TypeArray temp; findType(temp, name); array = temp;                        \
    for (Integer j=0; j<4; j++) array.setBase(temp.getBase(j), j);             \
    release(name); return 0;                                                   \
}
DSK_MACRO(findInteger,    IntegerArray   )
#ifndef DOUBLE
DSK_MACRO(findDoubleReal, DoubleRealArray)
#endif // DOUBLE
DSK_MACRO(findReal,       RealArray      )
#undef DSK_MACRO

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
//              Int                  Error code
//
#define DSK_MACRO(putType, createType, Type)                                   \
inline Integer Dir::putType(const Type* x, const aString& name,                 \
  const aString& flags, const Integer& ip) const {                              \
    if (exists(name)) destroy(name);                                           \
    createType(name, flags, ip);                                               \
    char type; const Integer block=0, total=0, len_type=1, writit=1;           \
    Integer desc, ndims, owner, dims, range, ovrlap, period, blocks, ierr;     \
    desc = dskfds_(_disk, _dir, name, strlen(name));                           \
    dskdnf_(_disk, desc, block, total, &type, ndims, owner, dims,              \
      range, ovrlap, period, blocks, ierr, len_type);                          \
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;        \
    if (ierr) message(ierr);                                                   \
    dskdcb_(_disk, desc, (void*)x, _disk[dims-1], _disk[dims-1], writit, ierr);\
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;        \
    if (ierr) message(ierr);                                                   \
    release(name, " W"); return ierr;                                          \
}                                                                              \
inline Integer Dir::putType(const Type& x, const aString& name,                 \
  const aString& flags, const Integer& ip) const                                \
  { return putType(&x, name, flags, ip); }                                     \
inline Integer Dir::putType(const Type* x, const aString& name,                 \
  const aString& flags, const Integer* ip) const                                \
  { return putType(x, name, flags, *ip); }                                     \
inline Integer Dir::putType(const Type& x, const aString& name,                 \
  const aString& flags, const Integer* ip) const                                \
  { return putType(&x, name, flags, *ip); }                                    \
inline Integer Dir::putType(const Type* x, const aString& name,                 \
  const aString& flags) const { return putType(x, name, flags, 0); }            \
inline Integer Dir::putType(const Type& x, const aString& name,                 \
  const aString& flags) const { return putType(&x, name, flags, 0); }           \
inline Integer Dir::putType(const Type* x, const aString& name,                 \
  const Integer& ip) const { return putType(x, name, " D*(*)", ip); }          \
inline Integer Dir::putType(const Type& x, const aString& name,                 \
  const Integer& ip) const { return putType(&x, name, " D*(*)", ip); }         \
inline Integer Dir::putType(const Type* x, const aString& name,                 \
  const Integer* ip) const { return putType(x, name, " D*(*)", *ip); }         \
inline Integer Dir::putType(const Type& x, const aString& name,                 \
  const Integer* ip) const { return putType(&x, name, " D*(*)", *ip); }        \
inline Integer Dir::putType(const Type* x, const aString& name) const           \
  { return putType(x, name, " ", 0); }                                         \
inline Integer Dir::putType(const Type& x, const aString& name) const           \
  { return putType(&x, name, " ", 0); }
DSK_MACRO(putComplex,       createComplex,       Complex      )
DSK_MACRO(putInteger,       createInteger,       Integer      )
DSK_MACRO(putLogical,       createLogical,       Logical      )
DSK_MACRO(putPointer,       createPointer,       Pointer      )
DSK_MACRO(putDoubleReal,    createDoubleReal,    DoubleReal   )
DSK_MACRO(putReal,          createReal,          Real         )
DSK_MACRO(putDoubleComplex, createDoubleComplex, DoubleComplex)
#undef DSK_MACRO
inline Integer Dir::putChar(const char* x, const aString& name,
  const aString& flags, const Integer& ip) const {
    if (exists(name)) destroy(name);
    createChar(name, flags, ip);
    char type; const Integer block=0, total=0, len_type=1, writit=1;
    Integer desc, ndims, owner, dims, range, ovrlap, period, blocks, ierr;
    desc = dskfds_(_disk, _dir, name, strlen(name));
    dskdnf_(_disk, desc, block, total, &type, ndims, owner, dims,
      range, ovrlap, period, blocks, ierr, len_type);
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    Integer *dims0 = new Integer [2*ndims];
    for (Integer i=0; i<ndims; i++) dims0[i] = _disk[dims+i-1];
    dims0[1] = 4 * _disk[dims];
    dskdcs_(_disk, desc, (char*)x, *dims0, *dims0, writit, ierr, dims0[1]);
    delete [] dims0;
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    release(name, " W"); return ierr;
}
inline Integer Dir::putChar(const char& x, const aString& name,
  const aString& flags, const Integer& ip) const
  { return putChar(&x, name, flags, ip); }
inline Integer Dir::putChar(const char* x, const aString& name,
  const aString& flags, const Integer* ip) const
  { return putChar(x, name, flags, *ip); }
inline Integer Dir::putChar(const char& x, const aString& name,
  const aString& flags, const Integer* ip) const
  { return putChar(&x, name, flags, *ip); }
inline Integer Dir::putChar(const char* x, const aString& name,
  const aString& flags) const
  { return putChar(x, name, flags, 0); }
inline Integer Dir::putChar(const char& x, const aString& name,
  const aString& flags) const
  { return putChar(&x, name, flags, 0); }
inline Integer Dir::putChar(const char* x, const aString& name,
  const Integer& ip) const
  { return putChar(x, name, " D*(*)", ip); }
inline Integer Dir::putChar(const char& x, const aString& name,
  const Integer& ip) const
  { return putChar(&x, name, " D*(*)", ip); }
inline Integer Dir::putChar(const char* x, const aString& name,
  const Integer* ip) const
  { return putChar(x, name, " D*(*)", *ip); }
inline Integer Dir::putChar(const char& x, const aString& name,
  const Integer* ip) const
  { return putChar(&x, name, " D*(*)", *ip); }
inline Integer Dir::putChar(const char* x, const aString& name) const
  { return putChar(x, name, " ", 0); }
inline Integer Dir::putChar(const char& x, const aString& name) const
  { return putChar(&x, name, " ", 0); }

//
//  Declare member functions put(x,name) for some types.
//
//  Arguments:  const Type&   x
//              const aString& name
//
#define DSK_MACRO(createType, Type)                                            \
inline Integer Dir::put(const Type& x, const aString& name) {                   \
    if (exists(name)) destroy(name);                                           \
    Type& temp = createType(name); temp = x; release(name, " W"); return 0;    \
}
DSK_MACRO(createComplex,       Complex      )
DSK_MACRO(createInteger,       Integer      )
DSK_MACRO(createDoubleReal,    DoubleReal   )
DSK_MACRO(createReal,          Real         )
DSK_MACRO(createDoubleComplex, DoubleComplex)
#undef DSK_MACRO
inline Integer Dir::put(const aString& x, const aString& name) {
    if (exists(name)) destroy(name);
    const Integer len_y = (x.length() + 3) / 4 * 4;
    char *y = strncpy(new char[len_y+1], x, len_y+1);
//  y is x, padded with nulls.
    createChar(name, " D(*)", len_y);
    Integer desc = dskfds_(_disk, _dir, name, strlen(name));
    const Integer writit=1; Integer dims[2], ierr; dims[0] = 1; dims[1] = len_y;
    dskdcs_(_disk, desc, y, *dims, *dims, writit, ierr, dims[1]);
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) message(ierr);
    delete [] y; release(name, " W"); return ierr;
}
inline int Dir::put(const aString x[], const aString& name,
  const int numberOfStrings) { return 0; }

//
//  Declare member functions put(array,name) for some A++ array types.
//
//  Arguments:  const TypeArray& array
//              const aString&    name
//
#define DSK_MACRO(createType, TypeArray)                                       \
inline Integer Dir::put(const TypeArray& array, const aString& name) {          \
    if (exists(name)) destroy(name);                                           \
    for (Integer ip[4][2], i=0; i<4; i++)                                      \
      { ip[i][0] = array.getBase(i); ip[i][1] = array.getBound(i); }           \
    TypeArray temp; createType(temp, name, " D4(*:*)", (Integer*)ip);          \
    temp = array; release(name, " W"); return 0;                               \
}
DSK_MACRO(createInteger,    IntegerArray   )
#ifndef DOUBLE
DSK_MACRO(createDoubleReal, DoubleRealArray)
#endif // DOUBLE
DSK_MACRO(createReal,       RealArray      )
#undef DSK_MACRO
#undef DSK_ADOPT

//
//  The next constructor should be private, but it is needed for
//  now in order to create a Dir from a Fortran DSK directory:
//
inline Dir::Dir(Integer* disk, const Integer& dir)
  { _disk = disk; _dir = dir, _disk[refcnt]++; }

inline char* Dir::truncate(char *string, Integer len) const {
    while (len && string[len-1] == ' ') len--; string[len] = 0;
    return string;
}
inline Integer Dir::message(const Integer& ierr) const {
    char error[80];
    dskerr_(error, ierr, sizeof(error)-1);
    cout << truncate(error, sizeof(error)-1) << endl;
    return ierr;
}
inline Integer Dir::find(const char* name) const
  { return dskfnd_(_disk, _dir, name, strlen(name)); }
inline Integer Dir::locate(const char* name) const
  { return dskloc_(_disk, _dir, name, strlen(name)); }
inline Integer Dir::create(const char* name, const char type,
  const char* flags, const Integer& ip) const {
    Integer loc, ierr;
    char *t_fl = new char[strlen(flags)+5];
    t_fl[0] = t_fl[3] = ' '; t_fl[1] = 'T'; t_fl[2] = type;
    strcpy(&t_fl[4], flags);
    dskdef_(_disk, _dir, name, t_fl, ip, loc, ierr,
      strlen(name), strlen(t_fl));
    delete [] t_fl;
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) { message(ierr); return 0; }
    return loc;
}
inline Integer Dir::link(const char* name, const char type, const char* flags,
  const Integer& ip, const Integer& loc) const {
    Integer ierr;
    char *t_fl = new char[strlen(flags)+5];
    t_fl[0] = t_fl[3] = ' '; t_fl[1] = 'T'; t_fl[2] = type;
    strcpy(&t_fl[4], flags);
    dsklnk_(_disk, _dir, name, t_fl, ip, loc, ierr,
      strlen(name), strlen(t_fl));
    delete [] t_fl;
    if (ierr) cout << "name = " << name << ", ierr = " << ierr << endl;
    if (ierr) { message(ierr); return 0; }
    return loc;
}
