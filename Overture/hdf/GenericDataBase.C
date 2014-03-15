#include "GenericDataBase.h"

static GenericDataBase nullGenericDataBase;

int GenericDataBase::maximumNumberOfLocalFilesForWriting=128;  // move to Overture init

static int
gdbError(const aString & func )
{
  cout << "GenericDataBase:ERROR: function `" << (const char *) func << "' in base class called!\n";
  if( &func )
    throw "GenericDataBase:ERROR";
  return 1;
}

//\begin{>GenericDataBaseInclude.tex}{\subsection{Constructors}} 
GenericDataBase:: 
GenericDataBase()
//=====================================================================================
// /Description:
//   Default constructor;
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  className="GenericDataBase";
  OvertureVersion=OVERTURE_VERSION; // this is changed if an old file is read in 
  
  mode=normalMode;
  issueWarnings=1;
  referenceCountingList=NULL;
 
  const int np = Communication_Manager::numberOfProcessors();
  int numFiles = min(maximumNumberOfLocalFilesForWriting,np);
  #ifdef USE_PPP
    numberOfLocalFilesForWriting=numFiles;
    numberOfLocalFilesForReading=numFiles;
  #else
    // special case: In serial we do not build extra files -- just write distributed data to the same file.
    numberOfLocalFilesForWriting=0;  
    numberOfLocalFilesForReading=0;
  #endif
  numberOfProcessorsUsedToWriteFile=np;
    
}

//\begin{>>GenericDataBaseInclude.tex}{}
GenericDataBase:: 
GenericDataBase(const GenericDataBase & gdb)
//=====================================================================================
// /Description:
//   Copy constructor.
//   Make a copy of the directory. This does not copy the data-base file.
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  *this=gdb;
}

GenericDataBase:: 
~GenericDataBase()
{
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{virtualConstructor}} 
GenericDataBase* GenericDataBase::
virtualConstructor() const
//=====================================================================================
// /Description:
//   This function will create a data-base (of a derived class) using "new" and 
//   return a pointer to it.
//   
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  gdbError("virtualConstructor");
  return new GenericDataBase;
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{operator =}} 
GenericDataBase & 
GenericDataBase::operator=(const GenericDataBase & gdb )
//=====================================================================================
// /Description:
//   Make a copy of the directory. This does not copy the data-base file.
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  className=gdb.className;
  issueWarnings=gdb.issueWarnings;
  referenceCountingList=gdb.referenceCountingList;
  OvertureVersion = gdb.OvertureVersion;
  
  return *this;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{mount(fileName,flags)}} 
int GenericDataBase::
mount(const aString & fileName, const aString & flags)
//=====================================================================================
// /Description:
//   Mount a data-base file.
// /fileName (input): Name of the file to open.
// /flags (input): flags to indicate how to access the file, "I" = initialize
//   a new file, "W" = open an existing file for reading and writing,
//   "R" = open an existing file read-only.
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("mount");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{unmount}} 
int GenericDataBase::
unmount()
//=====================================================================================
// /Description:
//   Close the data-base file;
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("unmount");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{flush()}} 
int GenericDataBase::
flush()
//=====================================================================================
// /Description:
//   Flush the data to the file. 
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("flush");
}

  
//\begin{>>GenericDataBaseInclude.tex}{\subsection{isNull()}} 
int GenericDataBase::
isNull() const
//=====================================================================================
// /Description:
//   return TRUE if this object is NOT attached to any file, return FALSE if it is attached to a file.
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("isNull");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{turnOnWarnings}} 
int GenericDataBase::
turnOnWarnings()
//=====================================================================================
// /Description:
//   Turn on warnings. For example the get functions will complain if the object they
// are looking for is not found.
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  issueWarnings=1;
  return 0;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{turnOffWarnings}} 
int GenericDataBase::
turnOffWarnings()
//=====================================================================================
// /Description:
//   Turn off warnings.
// /Author: WDH
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  issueWarnings=0;
  return 0;
}



//\begin{>>GenericDataBaseInclude.tex}{\subsection{create(dataBase,name,class)}} 
int GenericDataBase::
create(GenericDataBase & db, const aString & name, const aString & dirClassName )
//=====================================================================================
// /Description:
//   Create a sub-directory with a given name and class name.
// /db (output): This new object will be the sub-directory
// /name (input): name of the sub-directory
// /dirClassName (input): name of the class for the directory, default="directory"
// /return value: is 0 is the directory was successfully created, 1 otherwise
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("create(db)");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{find(dataBase,name,class) }} 
int GenericDataBase::
find(GenericDataBase & db, const aString & name, const aString & dirClassName ) const
//=====================================================================================
// /Description:
//   Find a sub-directory with a given name and class-name (optional)
//   If name="." then the current directory will be returned.
//   This function will "crash" if the sub-directory was not found. Use
//   locate if you don't want the function to crash.
// /db (output): This object will be the sub-directory on return
// /name (input): name of the sub-directory
// /dirClassName (input): name of the class for the directory, default="directory"
// /return value: is 0 is the directory was found, 1 otherwise
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("find(db)");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{locate(dataBase,name,class)}} 
int GenericDataBase::
locate(GenericDataBase & db, const aString & name, const aString & dirClassName ) const
//=====================================================================================
// /Description:
//   Find a sub-directory with a given name and class-name (optional)
//   If name="." then the current directory will be returned.
//   See also the find member function.
// /db (output): This object will be the sub-directory on return
// /name (input): name of the sub-directory
// /dirClassName (input): name of the class for the directory, default="directory"
// /return value: is 0 is the directory was found, 1 otherwise
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("locate(db)");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{find(name[~],class,maxNumber,actualNumber) }} 
int GenericDataBase::
find(aString *name, const aString & dirClassName, const int & maxNumber, int & actualNumber) const
//=====================================================================================
// /Description:
//   Find the names of all objects in the current directory with a given class-name
//
//  /name (input/output): array of Strings to hold the names of the directories. You must allocate at
//     least maxNumber Strings in this array.
//  /dirClassName (input): find all objects with this class name. This can be a user defined class name
//    such as "grid" as well as "int", "float", "double", "string", "intArray", "floatArray" and "doubleArray".
//  /maxNumber (input): this is the maximum number of Strings that 
//         can be stored in name[]. 
//  /actualNumber (output): This is the actual number of objects that exist.
//  /return value:  The number of Strings that were saved in the name array.
//
// /Description:
//   To first determine the number of objects with the given class-name that exist 
//    make a call with maxNumber=0. Then allocate aString name[actualNumber] and call again.
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("find(aString *name)");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{find(dataBase db[~],class,maxNumber,actualNumber) }} 
int GenericDataBase::
find(GenericDataBase *db, aString *name, const aString & dirClassName, const int & maxNumber,
     int & actualNumber) const
//=====================================================================================
// /Description:
//   Find all sub-directories with a given class-name
//
//  /db (input/output): return directories found in this array. You must allocate
//     at least maxNumber directories in db, for example with if maxNumber=10 you
//     could say
//     \begin{verbatim}
//         ADataBase db[10];
//     \end{verbatim}
//  /name : array of Strings to hold the names of the directories. You must allocate at
//     least maxNumber Strings in this array.
//  /maxNumber (input): this is the maximum number of directories that 
//         can be stored in db[]. 
//  /actualNumber (output): This is the actual number of directories
//         that exist.
//  /return value:  The number of directories that were saved in the db array.
//
// /Description:
//   To first determine the number of sub-directories with the given class-name that exist 
//    make a call with maxNumber=0. Then allocate db[actualNumber] and name[actualNumber] and call again.
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("find(*db)");
}

/* ----
int GenericDataBase::
destroy(const aString & name, const aString & flags) 
{
  return gdbError("destroy");
}
--- */


//\begin{>>GenericDataBaseInclude.tex}{\subsection{put([float][double][int][aString],name) }} 
int GenericDataBase::
put( const float & x, const aString & name ) 
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(float)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
put( const double & x, const aString & name ) 
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(double)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
put( const int & x, const aString & name )
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(int)");
}

#ifdef OV_BOOL_DEFINED
//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
put( const bool & x, const aString & name )
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(bool)");
}
#endif


//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
put( const aString & x, const aString & name ) 
//=====================================================================================
// /Description: Save a float, double, int or aString in the data-base with a given name.
//
// /x (input): The object to save.
// /name (input): Save "x" under this name in the data-base.
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(aString)");
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{get([float][double][int][aString],name) }} 
int GenericDataBase::
get( float & x, const aString & name ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(float)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
get( double & x, const aString & name ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(double)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
get( int & x, const aString & name ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(int)");
}

#ifdef OV_BOOL_DEFINED
//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
get( bool & x, const aString & name ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(bool)");
}
#endif

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
get( aString & x, const aString & name ) const
//=====================================================================================
//    
// /Description: Get a float, double, int or aString from the data-base with a given name.
//
// /x (output): The object to get.
// /name (input): The name of "x" in the data-base.
// /Return value : 0 if found, non-zero if not found   
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(aString)");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{get([floatSerialArray][doubleSerialArray][intSerialArray],name) }} 
int GenericDataBase::
get( floatSerialArray & x, const aString & name, Index *Iv /* =NULL */ ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(floatSerialArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
get( doubleSerialArray & x, const aString & name, Index *Iv /* =NULL */ ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{ 
  return gdbError("get(doubleSerialArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
get( intSerialArray & x, const aString & name, Index *Iv /* =NULL */ ) const
//=====================================================================================
//    
// /Description: get an A++ SerialArray from a data-base.
// /x (output): SerialArray to get. x will be "resized" to have the proper dimensions (base/bound)
// /name (input): the name of tha SerialArray to get
// /Return value : 0 if found, non-zero if not found   
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(intSerialArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{getDistributed([floatArray][doubleArray][intArray],name) }} 
int GenericDataBase::
getDistributed( floatArray & x, const aString & name ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("getDistributed(floatArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
getDistributed( doubleArray & x, const aString & name ) const
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{ 
  return gdbError("getDistributed(doubleArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
getDistributed( intArray & x, const aString & name ) const
//=====================================================================================
//    
// /Description: get an A++ array from a data-base.
// /x (output): array to get. x will be "resized" to have the proper dimensions (base/bound)
// /name (input): the name of tha array to get
// /Return value : 0 if found, non-zero if not found   
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("getDistributed(intArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{put([floatSerialArray][doubleSerialArray][intSerialArray],name) }} 
int GenericDataBase::
put( const floatSerialArray & x, const aString & name ) 
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(floatSerialArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
put( const doubleSerialArray & x, const aString & name ) 
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(doubleSerialArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
put( const intSerialArray & x, const aString & name )
//=====================================================================================
//    
// /Description: Save an A++ SerialArray in the data-base.
// /x (input): SerialArray to save
// /name (input): save the SerialArray with this name.
// /Iv[6] (input): optionally specify the Ranges of a sub-array to get.
// 
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(intSerialArray)");
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{putDistributed([floatArray][doubleArray][intArray],name) }} 
int GenericDataBase::
putDistributed( const floatArray & x, const aString & name ) 
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("putDistributed(floatArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
putDistributed( const doubleArray & x, const aString & name ) 
//=====================================================================================
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("putDistributed(doubleArray)");
}

//\begin{>>GenericDataBaseInclude.tex}{}
int GenericDataBase::
putDistributed( const intArray & x, const aString & name )
//=====================================================================================
//    
// /Description: Save an A++ array in the data-base.
// /x (input): array to save
// /name (input): save the array with this name.
//    
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("putDistributed(intArray)");
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{put(int[~],name,number) }} 
int GenericDataBase::
put( const int x[], const aString & name, const int number ) 
//=====================================================================================
//    
// /Description: save an array of int's to a data-base directory.
// /x (input): array to save.
// /name (input): save the array with this name
// /number (input): The number of entries in the array to save.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(int[])");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{put(float[~],name,number) }} 
int GenericDataBase::
put( const float x[], const aString & name, const int number ) 
//=====================================================================================
//    
// /Description: save an array of float's to a data-base directory.
// /x (input): array to save.
// /name (input): save the array with this name
// /numberOfStrings (input): The number of entries in the array to save.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("float(aString[])");
}
//\begin{>>GenericDataBaseInclude.tex}{\subsection{put(double[~],name,number) }} 
int GenericDataBase::
put( const double x[], const aString & name, const int number ) 
//=====================================================================================
//    
// /Description: save an array of double's to a data-base directory.
// /x (input): array  to save.
// /name (input): save the array with this name
// /numberOfStrings (input): The number of entries in the array to save.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(double[])");
}
//\begin{>>GenericDataBaseInclude.tex}{\subsection{put(aString[~],name,number) }} 
int GenericDataBase::
put( const aString x[], const aString & name, const int number ) 
//=====================================================================================
//    
// /Description: save an array of Strings to a data-base directory.
// /x (input): array to save.
// /name (input): save the array with this name
// /numberOfStrings (input): The number of entries in the array to save.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("put(aString[])");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{get(int[~],name,number) }} 
int GenericDataBase::
get( int x[], const aString & name, const int number ) const
//=====================================================================================
//    
// /Description: get an array from a data-base directory.
// /x (output): save the array x.
// /name (input): name of the array.
// /number (input): The maximum number of entries in the array to get.
// /return value: The actual number of entries that were saved in the array x.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(int[])");
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{get(float[~],name,number) }} 
int GenericDataBase::
get( float x[], const aString & name, const int number ) const
//=====================================================================================
//    
// /Description: get an array from a data-base directory.
// /x (output): save the array x.
// /name (input): name of the array.
// /number (input): The maximum number of entries in the array to get.
// /return value: The actual number of entries that were saved in the array x.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(float[])");
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{get(double[~],name,number) }} 
int GenericDataBase::
get( double x[], const aString & name, const int number ) const
//=====================================================================================
//    
// /Description: get an array from a data-base directory.
// /x (output): save the array x.
// /name (input): name of the array.
// /number (input): The maximum number of entries in the array to get.
// /return value: The actual number of entries that were saved in the array x.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(double[])");
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{get(aString[~],name,number) }} 
int GenericDataBase::
get( aString x[], const aString & name, const int number ) const
//=====================================================================================
//    
// /Description: get an array from a data-base directory.
// /x (output): save the array x.
// /name (input): name of the array.
// /number (input): The maximum number of entries in the array to get.
// /return value: The actual number of entries that were saved in the array x.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return gdbError("get(aString[])");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{setMode}} 
void GenericDataBase::
setMode(const InputOutputMode & mode_ /* =standard */)
//=====================================================================================
//    
// /Description:
//   Set the input-output mode for the data base. Note that any sub-directories subsequently
//  created in this data base will inherit this value for mode. Changing the mode from
//  {\tt streamInputMode} back to {\tt normalMode} will cause the
//   buffers to be saved in the data base. The buffers will also be saved when a directory 
//  is deleted provided that this directory was the one in which streaming mode was initially
//   turned on.  Currently only one set of buffers can be saved
//  in any directory which means that within a given directory the streaming mode can only
//  be turned on and off once.
//     
// /mode\_ (input) : input-output mode, {\tt normalMode}, {\tt streamInputMode}, 
//  {\tt streamOutputMode}, or {\tt noStreamMode}. In {\tt normalMode} the data is saved in the standard
//   hierarchical manner. In {\tt streamInputMode}/{\tt streamOutputMode} mode the
//   data is input/output continuguously from/into a buffer. The name of the object is ignored and
//   the act of creating new directories is ignored. In stream mode the data must be read back 
//   in in exactly the order it was written. In {\tt noStreamMode}
//   any requests to change to  {\tt streamInputMode} or {\tt streamOutputMode} will be ignored. This can
//   be used to suggest that no streaming should be done. To overide this mode you must first set the
//   mode to {\tt normalMode} and then you can change the mode to a streaming mode.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  if( mode!=noStreamMode || mode_==normalMode )
    mode=mode_;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{getMode}} 
GenericDataBase::InputOutputMode GenericDataBase::
getMode() const
//=====================================================================================
//    
// /Description:
//   Return the current input-output mode for the data base.
//
// /Return value: the current input-output mode.
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return mode;
}



//\begin{>>GenericDataBaseInclude.tex}{\subsection{printStatistics}} 
void  GenericDataBase::
printStatistics() const 
//=====================================================================================
//    
// /Description: 
//   Output statistics about the data base, such as the number of entries etc.
//
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{

}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{getList}} 
ReferenceCountingList* GenericDataBase::
getList() const
//=====================================================================================
// /Description: 
//   Return a pointer to a list that holds reference counted objects that are in the data base.
// This list can be used to keep track of items that have been saved in the data base. Each item
// in the list has an ID and a pointer to an object. In this way one can avoid saving multiple
// copies of objects since one can determine whether an object has already be saved. This feature
// is used when saving Mapping's to avoid multiple copies of a Mapping being saved.
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return referenceCountingList;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{getID}} 
int GenericDataBase::
getID() const
//=====================================================================================
// /Description: 
//   Get the identifier for this directory
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
 return gdbError("getID()");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{build}} 
int GenericDataBase::
build(GenericDataBase & db, int id)
//=====================================================================================
// /Description: 
//    Build a directory with the given ID, such as that returned by the member function {\tt getID()}.
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
 return gdbError("build()");
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{setParallelReadMode}} 
void GenericDataBase::
setParallelReadMode(ParallelIOModeEnum mode)
//=====================================================================================
// /Description: 
//   Set the read mode for HDF5, mode=independentIO (H5FD\_MPIO\_INDEPENDENT) or
//      mode=collectiveIO (H5FD\_MPIO\_COLLECTIVE) or mode=multipleFileIO.
// 
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  parallelReadMode=mode;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{setParallelWriteMode}} 
void GenericDataBase::
setParallelWriteMode(ParallelIOModeEnum mode)
//=====================================================================================
// /Description: 
//   Set the write mode for HDF5, mode=independentIO (H5FD\_MPIO\_INDEPENDENT) or
//      collectiveIO (H5FD\_MPIO\_COLLECTIVE) or mode=multipleFileIO.
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  parallelWriteMode=mode;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{setParallelReadMode}} 
GenericDataBase::ParallelIOModeEnum GenericDataBase::
getParallelReadMode()
//=====================================================================================
// /Description: 
//   Set the read mode for HDF5, mode=independentIO (H5FD\_MPIO\_INDEPENDENT) or
//      mode=collectiveIO (H5FD\_MPIO\_COLLECTIVE) or mode=multipleFileIO.
// 
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return parallelReadMode;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{setParallelWriteMode}} 
GenericDataBase::ParallelIOModeEnum GenericDataBase::
getParallelWriteMode()
//=====================================================================================
// /Description: 
//   Set the write mode for HDF5, mode=independentIO (H5FD\_MPIO\_INDEPENDENT) or
//      collectiveIO (H5FD\_MPIO\_COLLECTIVE) or mode=multipleFileIO.
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return parallelWriteMode;
}


//\begin{>>GenericDataBaseInclude.tex}{\subsection{getNumberOfLocalFilesForReading}} 
int GenericDataBase::
getNumberOfLocalFilesForReading() const 
//=====================================================================================
// /Description: 
//   Return the number of additional local files that are read for distributed data.
// These are extra files where each processor has saved information. 
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return numberOfLocalFilesForReading;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{getNumberOfLocalFilesForWriting}} 
int GenericDataBase::
getNumberOfLocalFilesForWriting() const 
//=====================================================================================
// /Description: 
//   Return the number of additional local files that are written for distributed data.
// These are extra files where each processor will save information. 
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  return numberOfLocalFilesForWriting;
}

//\begin{>>GenericDataBaseInclude.tex}{\subsection{setMaximumNumberOfFilesForWriting}} 
int GenericDataBase::
setMaximumNumberOfFilesForWriting( int maxNumberOfFiles )
//=====================================================================================
// /Description: 
//    (static function) Set the maximum number of local files that are written for distributed data.
//    These are extra files where each processor will save information. 
// 
//\end{GenericDataBaseInclude.tex} 
//=====================================================================================
{
  maximumNumberOfLocalFilesForWriting=maxNumberOfFiles;
  return 0;
}
