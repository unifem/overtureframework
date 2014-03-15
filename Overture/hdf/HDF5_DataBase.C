// This file automatically generated from HDF5_DataBase.bC with bpp.
#include "HDF5_DataBase.h"
#include "DataBaseBuffer.h"
#include <hdf5.h>
#include "ParallelUtility.h"

// These routines where based on the version for HDF4 written by WDH.
// First HDF5 version by Nathan Crane
// 2006: changes by Kyle Chand to work in parallel
// 2006: more fixes by WDH, plus renamed variables to remove "_" ala Overture convention.
// 2007: WDH: Added new multipleFileIO option since HDF5 parallel IO doesn't seem to work.

#ifndef USE_PPP
#define MPI_Wtime getCPU
// *** add this for now for testing
#include "wdhdefs.h"
#endif

double timeForParallelGetArray=0.;
double timeForSerialGetArray=0.;
double timeForScalarGet=0.;

int HDF_DataBase::
getFileGroup(int p, AcessModeEnum accessMode ) const
// ==========================================================================================
// 
// /Description:
//    Return the fileGroup that processor p belongs to.
// 
// /p (input) : processor number
// /accessMode (input) : return the fileGroup for reading or writing.
//
// /Return value: the file-group. 
// 
// ==========================================================================================
{
    const int numberOfLocalFiles = accessMode==write ? numberOfLocalFilesForWriting : numberOfLocalFilesForReading;
  // const int np = Communication_Manager::numberOfProcessors();
    assert( numberOfProcessorsUsedToWriteFile>0 );
    int fileGroup = int( p*numberOfLocalFiles/numberOfProcessorsUsedToWriteFile ); 
    assert( fileGroup>=0 && fileGroup<numberOfLocalFiles );
    return fileGroup;
}


aString HDF_DataBase::
getSerialFileName( const int p, AcessModeEnum accessMode ) const 
// ===========================================================================
//  /Description: 
//      Construct the name for the serialDataBase file. 
//   
// /fileName (input) : original file name
// /p (input) : processor number
// /accessMode (input) : return the file name for reading or writing.
// 
// /Return value: the name for the serial data base file.
// ===========================================================================
{
    aString serialFileName, suffix;
            
    int fileGroup = getFileGroup(p, accessMode );
    
    sPrintF(suffix,".g%i",fileGroup);  // file suffix (g=group, -- for now group=processor-number)
    serialFileName= fileName + suffix;
    
    return serialFileName;
}


HDF_DataBase* HDF_DataBase::
openLocalFile(int p, AcessModeEnum accessMode ) const 
// ==========================================================================================
// 
// /Description:
//     Open the local file that processor "p" uses for writing.
// 
// /p (input) : processor number
// /accessMode (input) : open file for reading or writing.
// 
// ==========================================================================================
{
    const bool openForWriting = accessMode==write;
    const int numberOfLocalFiles = openForWriting ? numberOfLocalFilesForWriting : numberOfLocalFilesForReading;
    const int myid = max(0,Communication_Manager::My_Process_Number);

    if( numberOfLocalFiles<=0 )
    { // special case: when the file was written in serial, we do not build extra local files.
        return (HDF_DataBase*)this;
    }
    
    if( serialDataBase==NULL )
    {
        serialDataBase = new HDF_DataBase* [numberOfLocalFiles];
        for( int f=0; f<numberOfLocalFiles; f++ )
            serialDataBase[f]=NULL;
    }
    
  // fid = processor p belongs to this file-group.
    int fileGroup = getFileGroup(p,accessMode);

  // procPrevious/procNext is the previous/next processor in the file-group of processor p.
  // procPrevious/procNext = -1 means there is no previous/next processor in this group. 
    int procPrevious = p-1;
    if( procPrevious>=0 && getFileGroup(procPrevious,accessMode)!=fileGroup )
        procPrevious=-1;

    aString serialFileName = getSerialFileName(p,accessMode);

    if( openForWriting && procPrevious>=0 )
    {
        #ifdef USE_PPP
    // we need to wait for processor procPrevious to finish before we can open the file for writing
        const MPI_Comm & OV_COMM = Overture::OV_COMM;
        MPI_Status status;
        int info;
        int tag= 183265 + procPrevious;
        int err = MPI_Recv(&info, 1, MPI_INT, procPrevious, tag, OV_COMM, &status );
        if( err == MPI_SUCCESS )
        {
            if( debug & 1 ) printf("openLocalFile: myid=%i received go-ahead from procPrevious=%i\n", myid,procPrevious);
        }
        else
        {
            printf("HDF_DataBase::openLocalFile: ERROR return receiving go-ahead from processor %i\n",procPrevious);
            Overture::abort("error");
        }
        #endif
    }
            

    if( serialDataBase[fileGroup]==NULL )
    {
        serialDataBase[fileGroup] = new HDF_DataBase();
        serialDataBase[fileGroup]->processorForWriting=p;  // set the processor for writing 


    // In this mode we also create a separate file that this processor can write to.
    // This file will hold the local array portions of distributed arrays.
        int rt=0;
        if( openForWriting && procPrevious==-1 )
        {
            rt= serialDataBase[fileGroup]->mount(serialFileName, "IS" );
          if( debug & 1 ) printf("openLocalFile: myid=%i open a new file: %s (I)\n", myid,(const char*)serialFileName);

        }
        else if( openForWriting )
        {
            rt= serialDataBase[fileGroup]->mount(serialFileName, "WS" );
          if( debug & 1 ) printf("openLocalFile: myid=%i open an old file (W) : %s\n", myid,(const char*)serialFileName);

        }
        else
        {
            rt= serialDataBase[fileGroup]->mount(serialFileName, "RS" );
          if( debug & 1 ) printf("openLocalFile: myid=%i open an old file (R) : %s\n", myid,(const char*)serialFileName);
        }
        if( rt!=0 )
        {
            printf("HDF_DataBase::openLocalFileForWriting::ERROR mounting %s, return code=%i\n",
                          (const char*)serialFileName,rt);
            Overture::abort("error");
        }

    }
    else if( serialDataBase[fileGroup]->fileID == -1 )  // check if file is already mounted ...
    {
    // mount an old file for reading or writing 
        if( debug & 1 ) printf("openLocalFile: myid=%i open: %s\n", myid,(const char*)serialFileName);

        int rt=0;
        if( openForWriting )
            rt= serialDataBase[fileGroup]->mount(serialFileName, "WS" );
        else
            rt= serialDataBase[fileGroup]->mount(serialFileName, "RS" );
            
        if( rt!=0 )
        {
            printf("HDF_DataBase::openLocalFileForWriting::ERROR mounting the old file %s, return code=%i\n",
                          (const char*)serialFileName,rt);
            Overture::abort("error");
        }

    }
    
    
    return serialDataBase[fileGroup];
}

int HDF_DataBase::
closeLocalFile(int p, AcessModeEnum accessMode ) const 
// ==========================================================================================
// 
// /Description:
//     Close the local file that processor "p" uses for reading or writing.
// 
// /p (input) : processor number
// /accessMode (input) : access mode is read or write
// 
// ==========================================================================================
{
    const int myid = max(0,Communication_Manager::My_Process_Number);
    const int np = Communication_Manager::numberOfProcessors();

    const bool closeForWriting = accessMode==write;
    const int numberOfLocalFiles = closeForWriting ? numberOfLocalFilesForWriting : numberOfLocalFilesForReading;

    if( numberOfLocalFiles<=0 )
    { // special case: when the file was read/written in serial, we do not build extra local files.
        return 0;
    }

  // fileGroup = processor p belongs to this file-group.
    int fileGroup = getFileGroup(p,accessMode);


    aString serialFileName = getSerialFileName(p,accessMode);  // just used for info messages

    assert( serialDataBase[fileGroup]!=NULL );
    
    int rt= 0;
    rt = serialDataBase[fileGroup]->unmount();
    
    if( rt!=0 )
        printf("HDF_DataBase::closeLocalFile::ERROR unmounting %s, return code=%i\n",(const char*)serialFileName,rt);

    if( debug & 1 ) printf("closeLocalFile: myid=%i, close file %s\n",myid,(const char*)serialFileName);


    if( closeForWriting )
    {
    // procPrevious/procNext is the previous/next processor in the file-group of processor p.
    // procPrevious/procNext = -1 means there is no previous/next processor in this group. 
        int procNext = p+1;
        if( procNext>=np || getFileGroup(procNext,accessMode)!=fileGroup )
            procNext=-1;
        if( procNext>=0 )
        {

            if( debug & 1 ) printf("closeLocalFile: myid=%i, close file %s. Send go ahead to procNext=%i.\n",
                       			     myid,(const char*)serialFileName,procNext);

      // Send a message to procNext to indicate that the file-group is available for writing 
            #ifdef USE_PPP
              const MPI_Comm & OV_COMM = Overture::OV_COMM;
              int tag= 183265 + myid;
              int & info = (int&)fileID;
              MPI_Ssend(&info, 1, MPI_INT, procNext, tag, OV_COMM);
            #endif
        }
        
    }

    return rt;

}

aString HDF_DataBase::
getSerialArrayName( const aString & fullGroupPath, const aString & name, const int p ) const
// ===========================================================================
//  /Description: 
//      Construct the name for a local serial array of a distributed array.
//   
//  /fullGroupPath (input) : current path.
//  /name (input) : name of the distributed array
// 
//  /Return value: the name for the serial array
// ===========================================================================
{
    aString serialArrayName, suffix;
    sPrintF(suffix,".p%i",p);  // name suffix 
            
//  serialArrayName = fullGroupPath + name + suffix;
//  serialArrayName = fullGroupPath + "/" + name + suffix;
    if( numberOfLocalFilesForReading<=0 )
    {
    // special case: when the file was written on 1 processor, the serial arrays go in the main data-base
        serialArrayName = name + suffix;
    }
    else
    {
    // When writing to the local data-base, build a unique name based on the full-group-path
    // We change '/' in the name to '-' since we don't actually build sub-directories
        aString prefix;
        if( fullGroupPath=="/root" )
            prefix=fullGroupPath(5,fullGroupPath.length()-1); // strip off '/root' from fullGroupPath
        else
            prefix=fullGroupPath(6,fullGroupPath.length()-1); // strip off '/root/' from fullGroupPath

        serialArrayName = prefix + "/" + name + suffix; 
        int len=serialArrayName.length();
        for( int i=0; i<len; i++ )
        {
            if( serialArrayName[i]=='/' ) serialArrayName[i]='\\';
        }
    }
    
    return serialArrayName;
}


namespace {






  // This next struct is used by the locate function:
    struct hdfGroupInfo 
    {   
        aString name;
        aString className;
        int actualNumber;
        int maxNumber;
        aString *nameOut;
        aString parentFullPath;
        int fileID;
    };

  //
  //		Set the class name of a HDF object
  //
    herr_t setClassName(hid_t locID, aString className) 
    {
    // /Author: Nathan Crane
        hsize_t dims[1]={className.length()+1}, rank=1;
        hid_t dataspaceID = H5Screate_simple(rank,dims,NULL);
        hid_t classNameAttribID = H5Acreate(locID,"className",H5T_NATIVE_UCHAR,dataspaceID,H5P_DEFAULT);
        H5Sclose(dataspaceID);
        int istat = H5Awrite(classNameAttribID,H5T_NATIVE_UCHAR,(char*)((const char*)className));
        if( istat<0 ) {
            printf("HDF_DataBase::create: ERROR return from H5Awrite\n");
            return 1;
        }
        H5Aclose(classNameAttribID);
        return 0;
    }
  //
  //		Get the class name of a HDF object
  //
    herr_t getClassName(hid_t locID, aString & className) 
    {
    // /Author: Nathan Crane
        hid_t classNameAttribID = H5Aopen_name(locID,"className");
        hid_t dataspaceID = H5Aget_space(classNameAttribID);
        hsize_t dims[1];
        H5Sget_simple_extent_dims(dataspaceID,dims,NULL);
        char *temp = new char[dims[0]];    
        H5Aread(classNameAttribID,H5T_NATIVE_UCHAR,(void*)temp);
        H5Sclose(dataspaceID);
        H5Aclose(classNameAttribID);
        className=temp;
        delete [] temp;
        return 0;
    }

//
//  Operator function for group iteration, simply return the object id and name
//  (Used by the locat function) 
//
    herr_t extractNamedDir(hid_t locID, const char *name, void * obj_data) 
    {
        hdfGroupInfo *hdfObjData = (hdfGroupInfo *) obj_data; 
        aString lookForName = hdfObjData->name;
        aString lookForClass = hdfObjData->className;
        H5G_stat_t statbuf;

        H5Gget_objinfo(locID,name,FALSE,&statbuf);

        if(statbuf.type==H5G_GROUP) 
        {
//
//		Extract the class name attribute for the group
//
            aString fullGroupName = hdfObjData->parentFullPath + "/" +name;
            hid_t sub_dirID = H5Gopen(hdfObjData->fileID,fullGroupName);
            aString className;
            getClassName(sub_dirID,className);
            H5Gclose(sub_dirID);
//
//		Check if name and class name both match the lookup names
//
            if((aString)className == lookForClass && ((aString)name == lookForName || lookForName=="directory")) 
            {
      	return sub_dirID;    
            }
        }    
        return 0;
    }

}

int HDF_DataBase::debug = 0;  

int HDF_DataBase::
getID() const 
{
    return 0;
}

int HDF_DataBase::
build(GenericDataBase& db0,int id)
{
    return 0;
}

//\begin{>HDF_DataBaseInclude.tex}{\subsection{turnOnWarnings}} 
int HDF_DataBase::
turnOnWarnings()
//=====================================================================================
// /Description:
//   Turn default HDF5 warning messages on;
// /Author: WDH
//
//\end{HDF_DataBaseInclude.tex} 
//=====================================================================================
{
  // *wdh* 081109 -- HDF5 warnings are basically usesless for users -- do not turn on ---
//  H5E_auto_t *func = new H5E_auto_t;   // *wdh* this should be deleted when finished
//  *func=(H5E_auto_t)H5Eprint;
//  H5Eset_auto(*func,stderr);
    issueWarnings=1;
    return 0;
}

//\begin{>HDF_DataBaseInclude.tex}{\subsection{turnOffWarnings}} 
int HDF_DataBase::
turnOffWarnings()
//=====================================================================================
// /Description:
//   Turn default HDF5 warning messages off;
// /Author: WDH
//
//\end{HDF_DataBaseInclude.tex} 
//=====================================================================================
{
    H5Eset_auto(NULL,stderr);
    issueWarnings=0;
    return 0;
}



//\begin{>HDF_DataBaseInclude.tex}{\subsection{Constructors}} 
HDF_DataBase::
HDF_DataBase()
//=====================================================================================
// /Description:
//   Default constructor;
// /Author: WDH and Nathan Crane
//
// /Notes:
//   fullGroupPath is a name of the directory such as  "/root/myDirectory".
// The root directory is named "/root".
//\end{HDF_DataBaseInclude.tex} 
//=====================================================================================
{  
    className="HDF_DataBase";
    accessMode=none;
    fileID=-1;

    fileName="";
    fullGroupPath="";

    mode=normalMode; // *wdh* do not change this
    dataBaseBuffer=NULL;
    bufferWasCreatedInThisDirectory=false;

    serialDataBase=NULL;
    
    processorForWriting=0;  // only one processor writes scalars to the file.


}

//\begin{>>HDF_DataBaseInclude.tex}{}
HDF_DataBase::
HDF_DataBase(const HDF_DataBase & db )
//=====================================================================================
// /Description:
//   Copy constructor (shallow copy).
//   Make a copy of the directory. This does not copy the data-base file.
// /Author: WDH
//
//\end{HDF_DataBaseInclude.tex} 
//=====================================================================================
{

    *this=db;
}

//\begin{>>HDF_DataBaseInclude.tex}{}
HDF_DataBase::
HDF_DataBase(const GenericDataBase & db )
//=====================================================================================
// /Description:
//   Copy constructor, this works if db is really a member of this derived class.
// /Author: WDH
//
//\end{HDF_DataBaseInclude.tex} 
//=====================================================================================
{
  // cast to HDF_DataBase -- first check class name
    if( db.className=="HDF_DataBase" )
    {
        *this=(const HDF_DataBase &) db;
    }
    else
    {
        cout << "HDF_DataBase:ERROR: copy constructor - input type is not HDF_DataBase\n";
        throw "HDF_DataBase:ERROR: copy constructor - input type is not HDF_DataBase";
    }
}

GenericDataBase* HDF_DataBase::
virtualConstructor() const
{
    return new HDF_DataBase;
}



HDF_DataBase & HDF_DataBase::
operator=(const HDF_DataBase & db )
//=====================================================================================
// /Description:
//   Shallow copy: make a copy of the directory. This does not copy the data-base file.
// /Author: WDH
//
//=====================================================================================
{ 
    if( debug & 4 )
        cout << "HDF_DataBase: operator = called " << endl;

    this->GenericDataBase::operator=(db);            // call = base class

    fileID=db.fileID;

    fileName = db.fileName;
    fullGroupPath = db.fullGroupPath;
    accessMode=db.accessMode;

    mode=db.mode;
    dataBaseBuffer=db.dataBaseBuffer; // copy pointer only
    bufferWasCreatedInThisDirectory=false;

    serialDataBase=db.serialDataBase;

    numberOfLocalFilesForWriting=db.numberOfLocalFilesForWriting; 
    numberOfLocalFilesForReading=db.numberOfLocalFilesForReading;
    numberOfProcessorsUsedToWriteFile=db.numberOfProcessorsUsedToWriteFile;
    
    return *this;
}

GenericDataBase & HDF_DataBase::
operator=(const GenericDataBase & db )
{
  // cast to HDF_DataBase if appropriate
    if( db.className=="HDF_DataBase" )
    {
        *this = (const HDF_DataBase &) db;
    }
    else
    {
        cout << "HDF_DataBase:ERROR: operator= - input type is not HDF_DataBase\n";
        throw "HDF_DataBase:ERROR: operator= - input type is not HDF_DataBase";
    }
    return *this;
}

HDF_DataBase::
~HDF_DataBase()
{
    if( dataBaseBuffer!=NULL && bufferWasCreatedInThisDirectory )
    {
        mode=normalMode;
        closeStream();  // close stream buffers
    }

    if( debug & 4 )
        cout << "HDF_DataBase: destructor called\n";
}


void HDF_DataBase::
destroy()
{
  // close the vgroup, delete from the list and then delete
    close();
}


  

int HDF_DataBase:: 
close()
// =================================================================
//    Close the directory, flush data. This does nothing for hdf5. 
// =================================================================
{
    return 0;
}
  
void HDF_DataBase::
reference( const HDF_DataBase & db )
{
    *this=db;
}

//\begin{>>HDF_DataBaseInclude.tex}{\subsection{mount}} 
int HDF_DataBase:: 
mount(const aString & dataBaseFileName, const aString & flags )
//=============================================================================
// /Description:
//   Mount a data-base file.
// 
// /dataBaseFileName (input): the name of the file.
// /flags (input): flags to indicate how to access the file, "I" = initialize
//   a new file, "W" = open an existing file for reading and writing,
//   "R" = open an existing file read-only.
//   In parallel, flags="IS" and flags="RS" mean mount a "serial file" instead of a parallel one.
//   These last two flags are only used internally. 
//
// /Return values: 0=success, -1=unable to open a file that was supposed to exist,
//     1=error in file format for an existing file, 2=unknown value for flags 
// 
//\end{HDF_DataBaseInclude.tex} 
//=============================================================================
{
  // debug=3;

    const int myid = max(0,Communication_Manager::My_Process_Number);
    const int np = Communication_Manager::numberOfProcessors();

    if( fileID > 0 )
    {
        printf("HDF_DataBase:mount: ERROR: cannot mount a file on a dataBase that already has a file mounted!\n");
        printf("                    myid=%i, fileName=%s, fileID=%i\n",myid,(const char*)fileName,fileID);
        Overture::abort("ERROR");
    }

    fileName = dataBaseFileName;
    
    
    /* Open an HDF file with full access. */
    if( flags==" I" || flags[0]=='I' || flags[0]=='i'  || 
            flags==" W" || flags[0]=='W' || flags[0]=='w' ) 
    {
        accessMode=write;
            
        hid_t plistID = H5P_DEFAULT;  // property list for the file
#ifdef USE_PPP
        if( flags != "IS" && flags!="WS" )
        {
            plistID = H5Pcreate(H5P_FILE_ACCESS);
            H5Pset_fapl_mpio(plistID,MPI_COMM_WORLD,MPI_INFO_NULL);
            if (plistID == -1) 
      	return -1; // property list could not be created
        }
#endif
            
    // the file should be opened with read/write access:
        if( flags==" I" || flags[0]=='I' || flags[0]=='i' )
        {
            fileID = H5Fcreate((const char *)fileName, H5F_ACC_TRUNC, H5P_DEFAULT, plistID);
        
#ifdef USE_PPP
            H5Pclose(plistID);
#endif
            
            if (fileID <= -1) 
      	return -1; // file couldn't be opened

      //	--- Create a root directory ---
            hid_t groupID = H5Gcreate(fileID,"root",0);
            setClassName(groupID,"directory");
            H5Gclose(groupID);
            fullGroupPath="/root";

      // Save the Overture version number 
            OvertureVersion = OVERTURE_VERSION;
            printF("File created with %s\n",(const char*)OvertureVersion);
            put( OvertureVersion,"OvertureVersion" );

      // Save the parallelWriteMode : if we write with multipleFileIO then we must read with multipleFileIO
            put((int)parallelWriteMode,"parallelWriteMode"); 

      // save the number of local files that will be written with local array data:
      // -- we save numberOfLocalFilesForWriting, but name it numberOfLocalFilesForReading
            put(numberOfLocalFilesForWriting,"numberOfLocalFilesForReading"); 
            put(numberOfProcessorsUsedToWriteFile,"numberOfProcessorsUsedToWriteFile");
            
            numberOfLocalFilesForReading=numberOfLocalFilesForWriting;  // do this for now 
        
      // --- optionally create the serial data base file ---
            if( flags != "IS" && parallelWriteMode==multipleFileIO && numberOfLocalFilesForReading>0 )
            {
        // We need to allocate the serialDataBase array now so the pointer will be passed to the DataBase's
        // for "sub-directories"
      	if( serialDataBase==NULL )
      	{
        	  serialDataBase = new HDF_DataBase* [numberOfLocalFilesForReading];
        	  for( int f=0; f<numberOfLocalFilesForReading; f++ )
          	    serialDataBase[f]=NULL;
      	}
            }
        }
        else
        {
      // -- Open an existing file  ---

      // fileID = H5Fcreate((const char *)fileName, H5F_ACC_RDWR, H5P_DEFAULT, plistID);
      // fileID = H5Fopen((const char *)fileName, H5F_ACC_RDWR, H5P_DEFAULT );
            fileID = H5Fopen((const char *)fileName, H5F_ACC_RDWR, plistID );
            fullGroupPath="/root";
            
            if (fileID <= -1) 
      	return -1; // file couldn't be opened
        }
        

    }
    else if( flags==" R" || flags[0]=='R' || flags[0]=='r' )
    {
        accessMode=read;
//
//		Check that the file is a HDF file
//
        if( !H5Fis_hdf5(fileName) ) 
        {
            return -1;
        }

        hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
        if( flags!="RS" )
        {
            plistID = H5Pcreate(H5P_FILE_ACCESS);
            H5Pset_fapl_mpio(plistID,MPI_COMM_WORLD,MPI_INFO_NULL);
            if (plistID == -1) 
      	return -1; // property list could not be created
        }
#endif

        fileID = H5Fopen((const char *)fileName, H5F_ACC_RDONLY, plistID);

#ifdef USE_PPP
        if( flags!="RS" )
            H5Pclose(plistID);
#endif

        if (fileID == -1) 
        {
            if( debug & 1 ) printf("HDF5:mount:ERROR: myid=%i, unable to open file %s read-only\n",
                                                          myid,(const char*)fileName);
            return -1; // file couldn't be opened
        }
        
    // attach the root -- here we assume that it is the first vgroup in file *****
        fullGroupPath="/root";

    // Get the Overture version number 
        OvertureVersion="Overture.unknown";
        turnOffWarnings();  issueWarnings=1;  // turn off warnings from hdf but keep my warnings
        get( OvertureVersion,"OvertureVersion" );
        turnOnWarnings();
        if( debug & 1 )
            printF("HDF_DB:mount: file %s was created with %s (current version is %s)\n",
                            (const char*)fileName,(const char*)OvertureVersion,OVERTURE_VERSION);

    // Get the write mode, if we wrote with multipleFileIO then we must read with multipleFileIO
        int fileParallelWriteMode=-1;
        get(fileParallelWriteMode,"parallelWriteMode"); 
        if( fileParallelWriteMode==multipleFileIO && parallelReadMode!=multipleFileIO )
        {
            parallelReadMode=(ParallelIOModeEnum)fileParallelWriteMode;
            printF("HDF_DataBase:mount: This file was written with multipleFileIO. I will read it back with multipleFileIO\n");
        }
        else if( fileParallelWriteMode!=multipleFileIO && parallelReadMode==multipleFileIO )
        {
            parallelReadMode=(ParallelIOModeEnum)fileParallelWriteMode; 
            printF("HDF_DataBase:mount: This file cannot be read with multipleFileIO since it was not written with multipleFileIO.\n"
                          "                    I will read it back with %s\n",
                (parallelReadMode==independentIO ? "independentIO" : parallelReadMode==collectiveIO ? "independentIO" : "unknown"));
        }
        
        
    // get the number of local files that were written with local array data:
        get(numberOfLocalFilesForReading,"numberOfLocalFilesForReading");
        get(numberOfProcessorsUsedToWriteFile,"numberOfProcessorsUsedToWriteFile");

    // --- optionally create the serial data base file ---
        if( flags != "RS" && parallelReadMode==multipleFileIO && numberOfLocalFilesForReading>0 )
        {
      // In this mode we also read the separate files that were written by different processors
      // This file will hold the local array portions of distributed arrays.

      // We need to allocate the serialDataBase array now so the pointer will be passed to the DataBase's
      // for "sub-directories"
            if( serialDataBase==NULL )
            {
      	serialDataBase = new HDF_DataBase* [numberOfLocalFilesForReading];
      	for( int f=0; f<numberOfLocalFilesForReading; f++ )
        	  serialDataBase[f]=NULL;
            }
        }
    }
    else
    {
        cout << "HDF_DataBase:mount: unknown flags = " << (const char *)flags << endl;
        return 2;
    }

    return 0;
}

int HDF_DataBase:: 
isNull() const
//======================================================================================
// /Description: Return TRUE if the dataBase is not pointing to a valid directory,
//    FALSE otherwise. 
//======================================================================================
{
    if(fileID <= 0 || (const char*)fullGroupPath==NULL) return 1;
    else return 0;
}

int HDF_DataBase:: 
unmount() 
//=============================================================================
// /Description:
//   Flush all the data to the file and close it.
//   Return the number of databases still connected to this file
//=============================================================================
{
    assert( fileID > 0);

    if( dataBaseBuffer!=NULL )
        closeStream();    // flush any stream buffers
    

    int returnValue=H5Fclose(fileID);
    if( returnValue<0 ) 
    {
        printf("****HDF_DataBase:unmount: could not unmount the file, err return=%i! fileID=%i\n",
         	   returnValue,fileID);
    // *wdh* 2012/05/17 return -1;
        OV_ABORT("HDF_DataBase::unmount:ERROR");
    }

    fileID = -1;

    if( serialDataBase!=NULL )
    {
        const int myid = max(0,Communication_Manager::My_Process_Number);
    // if( debug & 2 ) printf(" Unmount serial data base(s), myid=%i\n",myid);
        
        for( int fid=0; fid<numberOfLocalFilesForReading; fid++ )
        {
            if( serialDataBase[fid]!=NULL )
            {
      	if( serialDataBase[fid]->fileID!=-1 )
      	{
        	  if( debug & 2 ) printf(" Unmount serial data base %i\n",fid);

        	  int rt = serialDataBase[fid]->unmount();
        	  if( rt!=0 ) printf("HDF_DataBase::unmount:ERROR: closing the serialDataBase[%i]!\n",fid);
      	}
                delete serialDataBase[fid]; 
                serialDataBase[fid]=NULL;
            }
        }

        if( debug & 2 ) printf("**HDF5: delete serialDataBase ***\n");

        delete [] serialDataBase;
        serialDataBase=NULL;
    }
    

    return 0;
}

int HDF_DataBase:: 
flush() 
//=============================================================================
// /Description:
//   Flush all the data to the file
//=============================================================================
{
    assert( fileID > 0 && ((const char*)fullGroupPath != NULL));

    if( dataBaseBuffer!=NULL )
        closeStream();    // flush any stream buffers

  // THIS WILL FLUSH ALL THE DATA IN ALL DIRS OF THE FILE
    H5Fflush(fileID,H5F_SCOPE_GLOBAL);

    return 0;
}

int HDF_DataBase:: 
create(GenericDataBase & db0, const aString & name, const aString & dirClassName ) 
//=============================================================================
// /Description:
//     Create a sub-directory.
// 
// /name (input): name of the sub-directory
//   If name="." then the current directory will be returned.
// /dirClassName (input): name of the class for the directory, default="directory"
// /return value: is 0 is the directory was successfully created, 1 otherwise
//=============================================================================
{  
    assert( fileID > 0);
    if( accessMode!=write )
    {
        cout << "HDF_DataBase:ERROR: cannot createDir on a read-only dataBase \n";
        throw  "HDF_DataBase:ERROR: cannot createDir on a read-only dataBase ";
    }    

  // cast to this derived type
    if( db0.className!="HDF_DataBase" )
    {
        cout << "HDF_DataBase:ERROR: create - input type is not HDF_DataBase\n";
        throw "HDF_DataBase:ERROR: create - input type is not HDF_DataBase";
    }

    HDF_DataBase & db = (HDF_DataBase &) db0;

    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    // flush any stream buffers

  // copy all the current directory's info (fileID, etc) into the new dir handle
    db=*this;  // initialize
    if( name=="." || mode==streamOutputMode )
        return 0;

  // now append the new name to the full group path of the new dir.
    db.fullGroupPath = fullGroupPath + "/" + name;

  // actually create the hdf group that will represent the new dir.
    hid_t groupID = H5Gcreate(fileID,(const char*)db.fullGroupPath,0);

    if(groupID <=0) 
    {
        cout << "HDF_DataBase:create: fatal error in creating a new directory! \n";
        throw "HDF_DataBase:create: fatal error in creating a new directory! \n";
    } 
//
//		Set the directory class name with a attribute
//
    setClassName(groupID,dirClassName);
    H5Gclose(groupID);
    return 0;   
}


int HDF_DataBase:: 
link(GenericDataBase & dbLink, const HDF_DataBase & dbLinkDest, const aString & name) 
//=============================================================================
// /Description:
//   Link an existing directory to the current directory
// /dbLink (In/out): new database object to create
// /dbLinkDest (In): database to link sub directory from
// /name (In): name to give link
// /return value: is 0 is the directory was successfully links, 1 otherwise
//=============================================================================
{  
    assert( fileID > 0 && (const char*)fullGroupPath != NULL);
    if( accessMode!=write )
    {
        cout << "HDF_DataBase:ERROR: cannot createDir on a read-only dataBase \n";
        throw  "HDF_DataBase:ERROR: cannot createDir on a read-only dataBase ";
    }    

  // cast to this derived type
    if( dbLink.className!="HDF_DataBase" )
    {
        cout << "HDF_DataBase:ERROR: create - input type is not HDF_DataBase\n";
        throw "HDF_DataBase:ERROR: create - input type is not HDF_DataBase";
    }
    HDF_DataBase & db = (HDF_DataBase &) dbLink;

    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    // flush any stream buffers
//
//		Link to the old directory, using filenames begining at root
//
    db.fileID = fileID;
    db.fullGroupPath = dbLinkDest.fullGroupPath + "/" + name;
    int returnValue = H5Glink(fileID,H5G_LINK_HARD,fullGroupPath,db.fullGroupPath);

    return returnValue;   
}


int HDF_DataBase:: 
find(GenericDataBase & db, const aString & name, const aString & dirClassName) const
//=============================================================================
// /Description:
//   find a sub-directory with a given name and class-name (optional)
//   If name="." then the current directory will be returned.
//   This function will "crash" if the sub-directory was not found. Use
//   locate if you don't want the function to crash.
// /name (input): name of the sub-directory
// /dirClassName (input): name of the class for the directory, default="directory"
// /return value: is 0 is the directory was found, 1 otherwise
//=============================================================================
{
    int returnValue;
    returnValue = locate(db,name,dirClassName);
    if( returnValue!=0 )
    {
        HDF_DataBase & hdf = (HDF_DataBase &)db;
        
        cout << "FindDir:ERROR: unable to find directory " << (const char *) name << endl;
        cout << "fullGroupPath=[" << hdf.fullGroupPath << "]\n";
        
        throw "FindDir:ERROR: unable to find directory ";
    }
    return returnValue;
}



int HDF_DataBase:: 
locate(GenericDataBase & db0, const aString & name, const aString & dirClassName) const
//=============================================================================
// /Description:
//   locate a sub-directory with a given name and class-name (optional)
//   If name="." then the current directory will be returned.
// /name (input): name of the sub-directory
// /dirClassName (input): name of the class for the directory, default="directory"
// /return value: is 0 is the directory was found, 1 otherwise
//   See also the find function.
//=============================================================================
{
    assert( fileID > 0 && (const char*)fullGroupPath!=NULL );

    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    // flush any stream buffers

  // cast to this derived type
    if( db0.className!="HDF_DataBase" )
    {
        cout << "HDF_DataBase:ERROR: create - input type is not HDF_DataBase\n";
        throw "HDF_DataBase:ERROR: create - input type is not HDF_DataBase";
    }
    HDF_DataBase & db = (HDF_DataBase &) db0;
    db=*this; // initialize
    if( name=="." || mode==streamInputMode )
        return 0;

  //   Iterate through the groups searching for the correct directory and class name

    int cur_object=0;
    hdfGroupInfo lookForInfo;
    lookForInfo.name = name;
    lookForInfo.className = dirClassName;
    lookForInfo.parentFullPath = fullGroupPath;
    lookForInfo.fileID = fileID;
    int returnValue;
    char *className;

    hid_t groupID=H5Gopen(fileID,fullGroupPath);
    returnValue=H5Giterate(groupID,".",&cur_object,extractNamedDir,(void *)&lookForInfo);
    H5Gclose(groupID);

    if(returnValue>0) 
    {
        db.fullGroupPath = fullGroupPath + "/" + name;
        return 0;
    } 
    else 
    { 
        if( debug & 1 )
        {
            cout << "FindDir:ERROR: unable to find directory " << (const char *) name << endl;
            cout << "fullGroupPath=[" << db.fullGroupPath << "]\n";
            cout << "dirClassName=[" << dirClassName << "]\n";
            
        }
        return 1;
    }
}


namespace 
{
herr_t 
extractNamedClasses(hid_t locID, const char *name, void * obj_data) 
//=====================================================================================
//   Operator function for group iteration, return multiple classes
//   Used by the "find" function
// /Author: Nathan Crane
//=====================================================================================
{
    hdfGroupInfo *hdfObjData = (hdfGroupInfo *) obj_data; 
        
    aString lookForClass = hdfObjData->className;
        
    H5G_stat_t statbuf;
    H5Gget_objinfo(locID,name,FALSE,&statbuf);
  //
  //		Extract the class name attribute for the group
  //
    aString fullGroupName = hdfObjData->parentFullPath + "/" +name;
        
    aString className;
    if(statbuf.type==H5G_GROUP) 
    {
        hid_t objID = H5Gopen(hdfObjData->fileID,fullGroupName);
        getClassName(objID,className);
        H5Gclose(objID);
    } 
    else if(statbuf.type==H5G_DATASET)
    {
        hid_t objID = H5Dopen(hdfObjData->fileID,fullGroupName);
        getClassName(objID,className);
        H5Dclose(objID);
    }

  // Check if name and class name both match the lookup names
    if(className == lookForClass) 
    {
        if(hdfObjData->actualNumber < hdfObjData->maxNumber) 
        {
            hdfObjData->nameOut[hdfObjData->actualNumber]=name;
        }
        hdfObjData->actualNumber++;
    }
    return 0;
}

}

int HDF_DataBase::
find(aString *nameOut, const aString & dirClassName, const int & maxNumber, int & actualNumber) const
// ====================================================================================================
// /Description:
//   Find all objects with the given class name in this directory.
// 
// See comments in GenericDataBase.C
// ====================================================================================================
{
    assert( fileID > 0 && (const char*)fullGroupPath!=NULL );

    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    // flush any stream buffers

    int cur_object=0;
    hdfGroupInfo lookForInfo;
    lookForInfo.className = dirClassName;
    lookForInfo.actualNumber=actualNumber;
    lookForInfo.maxNumber=maxNumber;
    lookForInfo.nameOut=nameOut;
    lookForInfo.actualNumber=0;
    lookForInfo.parentFullPath = fullGroupPath;
    lookForInfo.fileID = fileID;
    hid_t groupID = H5Gopen(fileID,fullGroupPath);

    H5Giterate(groupID,".",&cur_object,extractNamedClasses,(void *)&lookForInfo);

    H5Gclose(groupID);
    actualNumber = lookForInfo.actualNumber;

    if(actualNumber < maxNumber) return actualNumber;
    else return maxNumber;
}

namespace 
{
herr_t 
extractNamedDatabases(hid_t locID, const char *name, void * obj_data) 
// ====================================================================================
//  Operator function for group iteration, return multiple directorys (databases)
//   Used by the "find" function.
// /Author: Nathan Crane
// =====================================================================================
{
        
    hdfGroupInfo *hdfObjData = (hdfGroupInfo *) obj_data; 
        
    aString lookForClass = hdfObjData->className;
        
    H5G_stat_t statbuf;
    H5Gget_objinfo(locID,name,FALSE,&statbuf);
    if(statbuf.type!=H5G_GROUP) return 0;
  //
  //		Extract the class name attribute for the group
  //
    aString fullGroupName = hdfObjData->parentFullPath + "/" +name;
    hid_t sub_dirID = H5Gopen(hdfObjData->fileID,fullGroupName);
    aString className;
    getClassName(sub_dirID,className);
    H5Gclose(sub_dirID);
  //
  //		Check if name and class name both match the lookup names
  //
    if(className == lookForClass) 
    {
        if(hdfObjData->actualNumber < hdfObjData->maxNumber) 
        {
            hdfObjData->nameOut[hdfObjData->actualNumber]=name;
        }
        hdfObjData->actualNumber++;
    }
    return 0;
}
}

int HDF_DataBase::
find(GenericDataBase *dbOut, aString *nameOut, const aString & dirClassName, 
                                            const int & maxNumber, int & actualNumber) const
// =====================================================================================
// /Description:
//   Find all sub-directories with a given dirClassName
//
//  /dbOut (input/output): return directories found in this array. You must allocate
//     at least maxNumber directories in dbOut, for example with if maxNumber=10 you
//     could say
//     \begin{verbatim}
//         HDF_DataBase dbOut[10];
//     \end{verbatim}
//  /nameOut : array of Strings to hold the names of the directories. You must allocate at
//     least maxNumber Strings in this array.
//  /maxNumber (input): this is the maximum number of directories that 
//         can be stored in dbOut[]. 
//  /actualNumber (output): This is the actual number of directories
//         that exist.
//  /return value:  The number of directories that were saved in the db array.
//
// /Description:
//   To first determine the number of sub-directories with the given dirClassName that exist 
//    make a call with maxNumber=0. Then allocate db[actualNumber] and call again.
//    
// =====================================================================================
{
    assert( fileID > 0 && (const char*)fullGroupPath!=NULL );
    if( dbOut->className!="HDF_DataBase" )
    {
        cout << "HDF_DataBase:findDir:ERROR the arg GenericDataBase *db does not point to `HDF_DataBase`"<<endl;
        throw "HDF_DataBase:findDir:ERROR";
    }
    HDF_DataBase *db = (HDF_DataBase *)dbOut;
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    // flush any stream buffers

    int cur_object=0;
    hdfGroupInfo lookForInfo;
    lookForInfo.className = dirClassName;
    lookForInfo.actualNumber=actualNumber;
    lookForInfo.maxNumber=maxNumber;
    lookForInfo.nameOut=nameOut;
    lookForInfo.actualNumber=0;
    lookForInfo.parentFullPath = fullGroupPath;
    lookForInfo.fileID = fileID;

    hid_t groupID = H5Gopen(fileID,fullGroupPath);

    H5Giterate(groupID,".",&cur_object,extractNamedDatabases,(void *)&lookForInfo);

    H5Gclose(groupID);
    actualNumber = lookForInfo.actualNumber;
//
//		Set up the correct group id numbers in the returned databases
//
    int num_found;
    if(maxNumber < actualNumber) num_found=maxNumber;
    else num_found = actualNumber;
  
    for(int idatabase=0; idatabase<num_found; idatabase++) 
    {
        db[idatabase] = *this;
        db[idatabase].fullGroupPath=fullGroupPath +"/" + nameOut[idatabase];
    }
    return num_found;
}

void HDF_DataBase::
closeStream() const
// this is not really a const function
{
    HDF_DataBase & db = (HDF_DataBase & )(*this);  // cast away const

    if( dataBaseBuffer==NULL ||  !dataBaseBuffer->isOpen() || !bufferWasCreatedInThisDirectory )
        return;

    if( mode!=normalMode )
    {
        cout << "HDF_DataBase::closeStream: error: you should be in normalMode to close the stream buffers\n";
        throw "error";
    }
    if( HDF_DataBase::debug & 1 )
        printf("HDF_DataBase::closeStream: close the stream buffers: db.dataBaseBuffer=%d \n",db.dataBaseBuffer);
  // close and de-allocate buffers
    db.mode=bufferMode;
    db.dataBaseBuffer->closeBuffer(db); 
    delete db.dataBaseBuffer;
    db.dataBaseBuffer=NULL;
    db.mode=normalMode;

}

void HDF_DataBase::
setMode(const InputOutputMode & mode_ /* =normalMode */)
//=====================================================================================
//    
// /Description:
//   Set the input-output mode for the data base.
// /mode\_ (input) : input-output mode, {\tt normalMode}, {\tt streamInputMode}, 
//  {\tt streamOutput}, or {\tt noStreamMode}. In {\tt normalMode} the data is saved in the standard
//   hierarchical manner. In {\tt streamInputMode}/{\tt streamOutputMode} mode the
//   data is input/output continuguously from/into a buffer. The name of the object is ignored and
//   the act of creating new directories is ignored. In stream mode the data must be read back 
//   in in exactly the order it was written. In {\tt noStreamMode}
//   any requests to change to  {\tt streamInputMode} or {\tt streamOutputMode} will be ignored. This can
//   be used to suggest that no streaming should be done. To overide this mode you must first set the
//   mode to {\tt normalMode} and then you can change the mode to a streaming mode.
//=====================================================================================
{

    if( mode==mode_ || (mode==noStreamMode && mode_!=normalMode) )
        return;

    if( mode_==bufferMode )
    {
        mode=mode_;
        return;
    }

    if( dataBaseBuffer==NULL )
    {
        dataBaseBuffer= new DataBaseBuffer;
        bufferWasCreatedInThisDirectory=true;
    }

    InputOutputMode oldMode =mode;
    if( mode_!=oldMode )
    {
        if( mode_==streamOutputMode )
        {
      // open new buffers
            mode=bufferMode;
            dataBaseBuffer->openBuffer(*this,mode_);
        }
        else if( mode_==streamInputMode )
        {
      // get existing buffers
            mode=bufferMode;
            dataBaseBuffer->openBuffer(*this,mode_); // we should be in normalMode for this get
        }
        else if( mode_==normalMode )
        {
            if( !bufferWasCreatedInThisDirectory && (oldMode==streamOutputMode || oldMode==streamInputMode) )
            {
      	cout << "HDF_DataBase::setMode:ERROR: attempt to set mode back to normalMode from streamOutputMode or \n"
                          << " streamInputMode BUT this directory did not originally set the mode! \n";
      	cout << "  ...this could cause fatal errors, continuing anyway... \n";
            }
      // close and de-allocate buffers
            mode=normalMode;   // change the mode so we can put arrays in normal way

            closeStream();
        }
        mode=mode_;
    }
    
}

//=====================================================================================
// /Description: Save an A++ array in the data-base. The array is saved as an HDF
// Scientific Data Set.
// /x (input): array to save
// /name (input): save the array with this name.
//=====================================================================================
// Define a macro to save a float/int/double A++ Array
//  type=floatArray/intArray/doubleArray HDFType=corresponding HDF type

// declare instances of the serial A++ array put macro
// putSerialArrayMacro(floatSerialArray,H5T_NATIVE_FLOAT);
int 
HDF_DataBase::
put ( const floatSerialArray & x, const aString &name )
{
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put: cannot put an array to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    if ( mode==streamOutputMode ) 
    {
        if( false )
        {
    // This next call will save the array in non-buffered mode!
            dataBaseBuffer->putToBuffer(x);
        }
        else
        {
            /* save in the stream buffer */ 
            int dims[MAX_ARRAY_DIMENSION][2]; 
            int size=1; 
            for( int d=0; d<MAX_ARRAY_DIMENSION; d++) 
            { 
      	dims[d][0]=x.getBase(d); 
      	dims[d][1]=x.getBound(d);  
      	size*=(dims[d][1]-dims[d][0]+1);  
            } 
            dataBaseBuffer->putToBuffer( MAX_ARRAY_DIMENSION*2, dims[0] ); 
            dataBaseBuffer->putToBuffer( size,x.getDataPointer() ); 
        }
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hsize_t rank = MAX_ARRAY_DIMENSION; // using an A++/P++ macro here...
    hsize_t xDmins[MAX_ARRAY_DIMENSION];
    long int xBase[MAX_ARRAY_DIMENSION];
    hsize_t total_size=1;
    for ( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { // note we transpose the array here to get the ordering correct in the file
        xDmins[rank-a-1] = x.getLength(a);
        xBase[rank-a-1] = x.getBase(a);
        total_size *= xDmins[rank-a-1];
    }
    if( debug & 2 ) 
    {
        const int myid = max(0,Communication_Manager::My_Process_Number);
        printf(" put(floatSerialArray): put the array [%s/%s] on myid=%i\n",(const char*)fullGroupPath,(const char*)name,myid);
    }
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if( groupID<0 )
    {
        printf("HDF_DataBase:ERROR:put(floatSerialArray): Error opening group %s\n",(const char*)fullGroupPath);
        Overture::abort("error");  // ******************************
    }
    if ( !total_size )
    { // the write a scalar and detect it later
        hid_t dataspace = H5Screate(H5S_SCALAR);
        hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
        hid_t memspace =  H5Screate(H5S_SCALAR);
    // attach the class name
        setClassName( datasetID, "floatSerialArray");//!!! NOTE : floatSerialArray will be replaced by the macro argument!
        hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
        plistID = H5Pcreate(H5P_DATASET_XFER);
        if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put: could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
        const int myid = max(0,Communication_Manager::My_Process_Number);
        if ( myid==processorForWriting ) 
        {
#endif
      // printf(" put NULL serialArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
            int dummy=0;
            if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, &dummy)<0 )
            {
      	cout<< "HDF_DataBase:ERROR:put: could not write serial array with entry name = "<<name<<endl;
      	return 1;
            }
#ifdef USE_PPP
        }
        H5Pclose(plistID);
#endif
        H5Sclose(memspace);
        H5Dclose(datasetID);
        H5Sclose(dataspace);
        H5Gclose(groupID);
        return 0;
    }
  // create the dataspace telling hdf5 what the file image of the array will be
    hid_t dataspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_FLOAT, dataspace, H5P_DEFAULT);
    if( datasetID<0 )
    {
        printf("HDF_DataBase:ERROR:put(floatSerialArray): Error creating %s/%s\n",(const char*)fullGroupPath,(const char*)name);
        Overture::abort("error");  // ******************************
    }
  // attach the bounds as an attribute of the dataset
    hsize_t ab_rank=1;
    hsize_t abDims[] = { MAX_ARRAY_DIMENSION };
    hid_t attribspace = H5Screate_simple(ab_rank, abDims, NULL);
  // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(ab_rank, abDims, NULL);
    hid_t attribID = H5Acreate(datasetID, "arrayBase", H5T_NATIVE_LONG, attribspace, H5P_DEFAULT);
    if ( H5Awrite(attribID, H5T_NATIVE_LONG, xBase)<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not write array bases for entry name = "<<name<<endl;
        return 1;
    }
    H5Aclose(attribID);
  // *wdh* 10073 : not used ? H5Sclose(attribmem);
    H5Sclose(attribspace);
  // attach the class name
    setClassName( datasetID, "floatSerialArray");//!!! NOTE : floatSerialArray will be replaced by the macro argument!
  // I wonder if HDF5 is smart enough now to handle zero sized dataspaces?
  // now create the dataspace describing the memory image of the array
    hid_t memspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid!=processorForWriting ) 
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
#endif
  // printf(" put serialArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
    if ( H5Dwrite(datasetID, H5T_NATIVE_FLOAT, memspace, dataspace, plistID, x.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not write serial array with entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    return 0;
}
// putSerialArrayMacro(doubleSerialArray,H5T_NATIVE_DOUBLE);
int 
HDF_DataBase::
put ( const doubleSerialArray & x, const aString &name )
{
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put: cannot put an array to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    if ( mode==streamOutputMode ) 
    {
        if( false )
        {
    // This next call will save the array in non-buffered mode!
            dataBaseBuffer->putToBuffer(x);
        }
        else
        {
            /* save in the stream buffer */ 
            int dims[MAX_ARRAY_DIMENSION][2]; 
            int size=1; 
            for( int d=0; d<MAX_ARRAY_DIMENSION; d++) 
            { 
      	dims[d][0]=x.getBase(d); 
      	dims[d][1]=x.getBound(d);  
      	size*=(dims[d][1]-dims[d][0]+1);  
            } 
            dataBaseBuffer->putToBuffer( MAX_ARRAY_DIMENSION*2, dims[0] ); 
            dataBaseBuffer->putToBuffer( size,x.getDataPointer() ); 
        }
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hsize_t rank = MAX_ARRAY_DIMENSION; // using an A++/P++ macro here...
    hsize_t xDmins[MAX_ARRAY_DIMENSION];
    long int xBase[MAX_ARRAY_DIMENSION];
    hsize_t total_size=1;
    for ( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { // note we transpose the array here to get the ordering correct in the file
        xDmins[rank-a-1] = x.getLength(a);
        xBase[rank-a-1] = x.getBase(a);
        total_size *= xDmins[rank-a-1];
    }
    if( debug & 2 ) 
    {
        const int myid = max(0,Communication_Manager::My_Process_Number);
        printf(" put(doubleSerialArray): put the array [%s/%s] on myid=%i\n",(const char*)fullGroupPath,(const char*)name,myid);
    }
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if( groupID<0 )
    {
        printf("HDF_DataBase:ERROR:put(doubleSerialArray): Error opening group %s\n",(const char*)fullGroupPath);
        Overture::abort("error");  // ******************************
    }
    if ( !total_size )
    { // the write a scalar and detect it later
        hid_t dataspace = H5Screate(H5S_SCALAR);
        hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
        hid_t memspace =  H5Screate(H5S_SCALAR);
    // attach the class name
        setClassName( datasetID, "doubleSerialArray");//!!! NOTE : doubleSerialArray will be replaced by the macro argument!
        hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
        plistID = H5Pcreate(H5P_DATASET_XFER);
        if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put: could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
        const int myid = max(0,Communication_Manager::My_Process_Number);
        if ( myid==processorForWriting ) 
        {
#endif
      // printf(" put NULL serialArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
            int dummy=0;
            if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, &dummy)<0 )
            {
      	cout<< "HDF_DataBase:ERROR:put: could not write serial array with entry name = "<<name<<endl;
      	return 1;
            }
#ifdef USE_PPP
        }
        H5Pclose(plistID);
#endif
        H5Sclose(memspace);
        H5Dclose(datasetID);
        H5Sclose(dataspace);
        H5Gclose(groupID);
        return 0;
    }
  // create the dataspace telling hdf5 what the file image of the array will be
    hid_t dataspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT);
    if( datasetID<0 )
    {
        printf("HDF_DataBase:ERROR:put(doubleSerialArray): Error creating %s/%s\n",(const char*)fullGroupPath,(const char*)name);
        Overture::abort("error");  // ******************************
    }
  // attach the bounds as an attribute of the dataset
    hsize_t ab_rank=1;
    hsize_t abDims[] = { MAX_ARRAY_DIMENSION };
    hid_t attribspace = H5Screate_simple(ab_rank, abDims, NULL);
  // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(ab_rank, abDims, NULL);
    hid_t attribID = H5Acreate(datasetID, "arrayBase", H5T_NATIVE_LONG, attribspace, H5P_DEFAULT);
    if ( H5Awrite(attribID, H5T_NATIVE_LONG, xBase)<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not write array bases for entry name = "<<name<<endl;
        return 1;
    }
    H5Aclose(attribID);
  // *wdh* 10073 : not used ? H5Sclose(attribmem);
    H5Sclose(attribspace);
  // attach the class name
    setClassName( datasetID, "doubleSerialArray");//!!! NOTE : doubleSerialArray will be replaced by the macro argument!
  // I wonder if HDF5 is smart enough now to handle zero sized dataspaces?
  // now create the dataspace describing the memory image of the array
    hid_t memspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid!=processorForWriting ) 
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
#endif
  // printf(" put serialArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
    if ( H5Dwrite(datasetID, H5T_NATIVE_DOUBLE, memspace, dataspace, plistID, x.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not write serial array with entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    return 0;
}
// putSerialArrayMacro(intSerialArray,H5T_NATIVE_INT);
int 
HDF_DataBase::
put ( const intSerialArray & x, const aString &name )
{
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put: cannot put an array to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    if ( mode==streamOutputMode ) 
    {
        if( false )
        {
    // This next call will save the array in non-buffered mode!
            dataBaseBuffer->putToBuffer(x);
        }
        else
        {
            /* save in the stream buffer */ 
            int dims[MAX_ARRAY_DIMENSION][2]; 
            int size=1; 
            for( int d=0; d<MAX_ARRAY_DIMENSION; d++) 
            { 
      	dims[d][0]=x.getBase(d); 
      	dims[d][1]=x.getBound(d);  
      	size*=(dims[d][1]-dims[d][0]+1);  
            } 
            dataBaseBuffer->putToBuffer( MAX_ARRAY_DIMENSION*2, dims[0] ); 
            dataBaseBuffer->putToBuffer( size,x.getDataPointer() ); 
        }
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hsize_t rank = MAX_ARRAY_DIMENSION; // using an A++/P++ macro here...
    hsize_t xDmins[MAX_ARRAY_DIMENSION];
    long int xBase[MAX_ARRAY_DIMENSION];
    hsize_t total_size=1;
    for ( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { // note we transpose the array here to get the ordering correct in the file
        xDmins[rank-a-1] = x.getLength(a);
        xBase[rank-a-1] = x.getBase(a);
        total_size *= xDmins[rank-a-1];
    }
    if( debug & 2 ) 
    {
        const int myid = max(0,Communication_Manager::My_Process_Number);
        printf(" put(intSerialArray): put the array [%s/%s] on myid=%i\n",(const char*)fullGroupPath,(const char*)name,myid);
    }
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if( groupID<0 )
    {
        printf("HDF_DataBase:ERROR:put(intSerialArray): Error opening group %s\n",(const char*)fullGroupPath);
        Overture::abort("error");  // ******************************
    }
    if ( !total_size )
    { // the write a scalar and detect it later
        hid_t dataspace = H5Screate(H5S_SCALAR);
        hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
        hid_t memspace =  H5Screate(H5S_SCALAR);
    // attach the class name
        setClassName( datasetID, "intSerialArray");//!!! NOTE : intSerialArray will be replaced by the macro argument!
        hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
        plistID = H5Pcreate(H5P_DATASET_XFER);
        if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put: could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
        const int myid = max(0,Communication_Manager::My_Process_Number);
        if ( myid==processorForWriting ) 
        {
#endif
      // printf(" put NULL serialArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
            int dummy=0;
            if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, &dummy)<0 )
            {
      	cout<< "HDF_DataBase:ERROR:put: could not write serial array with entry name = "<<name<<endl;
      	return 1;
            }
#ifdef USE_PPP
        }
        H5Pclose(plistID);
#endif
        H5Sclose(memspace);
        H5Dclose(datasetID);
        H5Sclose(dataspace);
        H5Gclose(groupID);
        return 0;
    }
  // create the dataspace telling hdf5 what the file image of the array will be
    hid_t dataspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
    if( datasetID<0 )
    {
        printf("HDF_DataBase:ERROR:put(intSerialArray): Error creating %s/%s\n",(const char*)fullGroupPath,(const char*)name);
        Overture::abort("error");  // ******************************
    }
  // attach the bounds as an attribute of the dataset
    hsize_t ab_rank=1;
    hsize_t abDims[] = { MAX_ARRAY_DIMENSION };
    hid_t attribspace = H5Screate_simple(ab_rank, abDims, NULL);
  // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(ab_rank, abDims, NULL);
    hid_t attribID = H5Acreate(datasetID, "arrayBase", H5T_NATIVE_LONG, attribspace, H5P_DEFAULT);
    if ( H5Awrite(attribID, H5T_NATIVE_LONG, xBase)<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not write array bases for entry name = "<<name<<endl;
        return 1;
    }
    H5Aclose(attribID);
  // *wdh* 10073 : not used ? H5Sclose(attribmem);
    H5Sclose(attribspace);
  // attach the class name
    setClassName( datasetID, "intSerialArray");//!!! NOTE : intSerialArray will be replaced by the macro argument!
  // I wonder if HDF5 is smart enough now to handle zero sized dataspaces?
  // now create the dataspace describing the memory image of the array
    hid_t memspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid!=processorForWriting ) 
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
#endif
  // printf(" put serialArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
    if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, x.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:put: could not write serial array with entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    return 0;
}



namespace
{

// ********** assume that the distrubted dimensions are amongst the first 4: ***********
#define MAX_DISTRIBUTED_DIMENSIONS 4

struct ArrayDistributionInfo
{
int baseProc, nProcs;
int base[MAX_ARRAY_DIMENSION];
int dimProc[MAX_ARRAY_DIMENSION];
int dimVecL_L[MAX_ARRAY_DIMENSION];
int dimVecL[MAX_ARRAY_DIMENSION];
int dimVecL_R[MAX_ARRAY_DIMENSION];
};


// getArrayDistributionInfoMacro(intArray)
int 
getArrayDistributionInfo( const intArray & u, ArrayDistributionInfo & adi )
// =====================================================================================
// /Description:
//      Fill in the ArrayDistributionInfo object with the distribution information
//  for the array u.
//
// /u (input) : get distribution info for this array
// /adi (ouptut) : holds distribution info.
// =====================================================================================
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
    adi.baseProc = uDecomp->baseProc;
    adi.nProcs   = uDecomp->nProcs;
    for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    {
        adi.base[d] = u.getBase(d);
    }
    const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    for( int d=0; d<numDim; d++ )
    {
        adi.dimProc[d] = max(1,uDecomp->dimProc[d]);  // this dimension is split across this many processors
    // Here is the distribution along each dimension following the block parti distribution
    // 
    //      +------+------+-------+---- ...    --+------+---------+
    //        left  center  center                center   right
        adi.dimVecL_L[d] = uDArray->dimVecL_L[d];  // left
        adi.dimVecL[d]   = uDArray->dimVecL[d];    // center
        adi.dimVecL_R[d] = uDArray->dimVecL_R[d];  // right
    }
    for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
    {
        adi.dimProc[d] = 1;
        adi.dimVecL_L[d] = 1;  // left
        adi.dimVecL[d]   = 1;    // center
        adi.dimVecL_R[d] = 1;  // right
    }
#else
  // in serial we make up a serial distribution:
    adi.baseProc = 0;
    adi.nProcs   = 1;
    for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    {
        adi.base[d] = u.getBase(d);
    }
    const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    for( int d=0; d<numDim; d++ )
    {
        adi.dimProc[d] = 1;                 // this dimension is split across this many processors
    // Here is the distribution along each dimension following the block parti distribution
    // 
    //      +------+------+-------+---- ...    --+------+---------+
    //        left  center  center                center   right
        adi.dimVecL_L[d] = u.getLength(d);  // left
        adi.dimVecL[d]   = u.getLength(d);    // center
        adi.dimVecL_R[d] = u.getLength(d);  // right
    }
#endif
    return 0;
}
// getArrayDistributionInfoMacro(floatArray)
int 
getArrayDistributionInfo( const floatArray & u, ArrayDistributionInfo & adi )
// =====================================================================================
// /Description:
//      Fill in the ArrayDistributionInfo object with the distribution information
//  for the array u.
//
// /u (input) : get distribution info for this array
// /adi (ouptut) : holds distribution info.
// =====================================================================================
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
    adi.baseProc = uDecomp->baseProc;
    adi.nProcs   = uDecomp->nProcs;
    for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    {
        adi.base[d] = u.getBase(d);
    }
    const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    for( int d=0; d<numDim; d++ )
    {
        adi.dimProc[d] = max(1,uDecomp->dimProc[d]);  // this dimension is split across this many processors
    // Here is the distribution along each dimension following the block parti distribution
    // 
    //      +------+------+-------+---- ...    --+------+---------+
    //        left  center  center                center   right
        adi.dimVecL_L[d] = uDArray->dimVecL_L[d];  // left
        adi.dimVecL[d]   = uDArray->dimVecL[d];    // center
        adi.dimVecL_R[d] = uDArray->dimVecL_R[d];  // right
    }
    for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
    {
        adi.dimProc[d] = 1;
        adi.dimVecL_L[d] = 1;  // left
        adi.dimVecL[d]   = 1;    // center
        adi.dimVecL_R[d] = 1;  // right
    }
#else
  // in serial we make up a serial distribution:
    adi.baseProc = 0;
    adi.nProcs   = 1;
    for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    {
        adi.base[d] = u.getBase(d);
    }
    const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    for( int d=0; d<numDim; d++ )
    {
        adi.dimProc[d] = 1;                 // this dimension is split across this many processors
    // Here is the distribution along each dimension following the block parti distribution
    // 
    //      +------+------+-------+---- ...    --+------+---------+
    //        left  center  center                center   right
        adi.dimVecL_L[d] = u.getLength(d);  // left
        adi.dimVecL[d]   = u.getLength(d);    // center
        adi.dimVecL_R[d] = u.getLength(d);  // right
    }
#endif
    return 0;
}
// getArrayDistributionInfoMacro(doubleArray)
int 
getArrayDistributionInfo( const doubleArray & u, ArrayDistributionInfo & adi )
// =====================================================================================
// /Description:
//      Fill in the ArrayDistributionInfo object with the distribution information
//  for the array u.
//
// /u (input) : get distribution info for this array
// /adi (ouptut) : holds distribution info.
// =====================================================================================
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
    adi.baseProc = uDecomp->baseProc;
    adi.nProcs   = uDecomp->nProcs;
    for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    {
        adi.base[d] = u.getBase(d);
    }
    const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    for( int d=0; d<numDim; d++ )
    {
        adi.dimProc[d] = max(1,uDecomp->dimProc[d]);  // this dimension is split across this many processors
    // Here is the distribution along each dimension following the block parti distribution
    // 
    //      +------+------+-------+---- ...    --+------+---------+
    //        left  center  center                center   right
        adi.dimVecL_L[d] = uDArray->dimVecL_L[d];  // left
        adi.dimVecL[d]   = uDArray->dimVecL[d];    // center
        adi.dimVecL_R[d] = uDArray->dimVecL_R[d];  // right
    }
    for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
    {
        adi.dimProc[d] = 1;
        adi.dimVecL_L[d] = 1;  // left
        adi.dimVecL[d]   = 1;    // center
        adi.dimVecL_R[d] = 1;  // right
    }
#else
  // in serial we make up a serial distribution:
    adi.baseProc = 0;
    adi.nProcs   = 1;
    for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    {
        adi.base[d] = u.getBase(d);
    }
    const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    for( int d=0; d<numDim; d++ )
    {
        adi.dimProc[d] = 1;                 // this dimension is split across this many processors
    // Here is the distribution along each dimension following the block parti distribution
    // 
    //      +------+------+-------+---- ...    --+------+---------+
    //        left  center  center                center   right
        adi.dimVecL_L[d] = u.getLength(d);  // left
        adi.dimVecL[d]   = u.getLength(d);    // center
        adi.dimVecL_R[d] = u.getLength(d);  // right
    }
#endif
    return 0;
}
    


bool 
getLocalArrayBox( int p, ArrayDistributionInfo & adi, IndexBox & box )
// ================================================================================================
// /Description:
//   Return a box for the local array of u on processor p (with ghost points)
// 
// /p (input) : build a box for this processor.
// /adi (input) : parallel distribution info (as computed, for e.g. by getArrayDistributionInfo).
// /box (output) : a box representing the array bounds (no ghost points) 
//                 for the local array on processor p.
// =================================================================================================
{
    int p0= p - adi.baseProc;  // processor number offset from base (starting) processor for this distribution

    if( p0<0 || p0>=adi.nProcs )
    { // There are no elements in the local array for this processor
        box.setBounds(0,-1, 0,-1, 0,-1, 0,-1); // return an empty box
        return true;
    }
    
  //  Compute the "processor vector" (where we are located in the "grid" of distributed local arrays)
  //       pv[0], pv[1], ..., pv[numDim-1]
  // Such that 
  //    p = baseProc + pv[numDim-1] + dimProc[numDim-1]*( pv[numDim-2] + dimProc[numDim-2]*( pv[numDim-3] + ... )

    int pv[MAX_DISTRIBUTED_DIMENSIONS];
    const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
    for( int d=numDim-1; d>=0; d-- )
    {
        const int dimProc = adi.dimProc[d];  // number of processors allocated to this dimension
        pv[d] = p0 - (p0/dimProc)*dimProc;
        p0= (p0-pv[d])/dimProc;
    }
    for( int d=numDim; d<MAX_DISTRIBUTED_DIMENSIONS; d++ )
        pv[d]=0;


  // Now compute the bounds on the local array u on processor p
    int ia[MAX_DISTRIBUTED_DIMENSIONS], ib[MAX_DISTRIBUTED_DIMENSIONS];
    for( int d=0; d<numDim; d++ )
    {
        int dimProc = adi.dimProc[d];  // this dimension is split across this many processors
    // Fill in the distribution along each dimension following the block parti distribution
    // 
    //      +------+------+-------+---- ...    --+------+---------+
    //        left  center  center                center   right
        const int left = max(1,adi.dimVecL_L[d]), center=max(1,adi.dimVecL[d]), right=max(1,adi.dimVecL_R[d]);

        ia[d] = adi.base[d];
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
        ia[d]=ib[d]=0;
    }

    box.setBounds(ia[0],ib[0], ia[1],ib[1], ia[2],ib[2], ia[3],ib[3]);

    return true;
}

      
}

//=====================================================================================
// /Description: Save an P++ array in the data-base. 
// 
// /x (input): array to save
// /name (input): save the array with this name.
//=====================================================================================
// Define a macro to save a float/int/double A++ Array
//  type=floatArray/intArray/doubleArray HDFType=corresponding HDF type

// declare instances of the parallel P++ array put macro
// putParallelArrayMacro(floatArray,floatSerialArray,H5T_NATIVE_FLOAT);
int 
HDF_DataBase::
putDistributed ( const floatArray & x, const aString &name )
{
// #ifndef USE_PPP
//   return put(x,name);
// #else  
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put(floatArray): cannot put an array to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    const int myid = max(0,Communication_Manager::My_Process_Number);
    const int np = Communication_Manager::numberOfProcessors();
  // printf(" putParallelArray: myid=%i name=%s mode=%i\n",myid,(const char*)name,mode);
    if ( mode==streamOutputMode ) 
    {
    // This next call will save the array in non-buffered mode!
        dataBaseBuffer->putDistributedToBuffer(x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  // !!! should this only be done by process 0?
  // get info on the full parallel array
    hsize_t rank = MAX_ARRAY_DIMENSION; // using an A++/P++ macro here...
    hsize_t xDmins[MAX_ARRAY_DIMENSION];
    long int xBase[MAX_ARRAY_DIMENSION];
    hsize_t total_size=1;
    for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { 
    // note we transpose the array here to get the ordering correct in the file
    // (hdf5 saves the array dimensions in the reverse order)
        xDmins[rank-a-1] = x.getLength(a);
        xBase[rank-a-1] = x.getBase(a);
        total_size *= xDmins[rank-a-1];
    }
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( !total_size )
    { 
    // **** The parallel array has ZERO entries -- just write a scalar and detect it later ****
        hid_t dataspace = H5Screate(H5S_SCALAR);
        hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
        hid_t memspace =  H5Screate(H5S_SCALAR);
    // attach the class name
        setClassName( datasetID, "floatArray");//!!! NOTE : floatArray will be replaced by the macro argument!
        hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
        plistID = H5Pcreate(H5P_DATASET_XFER);
        if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(floatArray): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif      
        if ( myid==processorForWriting ) 
        {
      // printf(" put NULL parallelArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
            int dummy=0;
            if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, &dummy)<0 )
            {
      	cout<< "HDF_DataBase:ERROR:put(floatArray): could not write NULL array with entry name = "<<name<<endl;
      	return 1;
            }
        }
        H5Pclose(plistID);
        H5Sclose(memspace);
        H5Dclose(datasetID);
        H5Sclose(dataspace);
        H5Gclose(groupID);
        return 0;
    }
  // get info on the local portion of the parallel array 
  // floatSerialArray xl;   xl.reference(x.getLocalArrayWithGhostBoundaries());
  // *wdh* 061214 const floatSerialArray & xl = x.getLocalArrayWithGhostBoundaries();
    floatSerialArray xl; getLocalArrayWithGhostBoundaries(x,xl);
  // NOTE: sizeof(hsize_t)=8 sizeof(H5T_NATIVE_LONG)=4 on 32bit and 64 bit
    hsize_t xlDims[MAX_ARRAY_DIMENSION];
    long int xlBase[MAX_ARRAY_DIMENSION];
    hsize_t localSize = 1;
    for ( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { // note we transpose the array here to get the ordering correct in the file
        xlDims[rank-a-1] = xl.getLength(a);
        xlBase[rank-a-1] = xl.getBase(a);
        localSize *= xlDims[rank-a-1];
    }
  // -- create the dataspace telling hdf5 what the file image of the array will be
  // hid_t dataspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t dataspace;
    if( parallelWriteMode==multipleFileIO ) // *wdh* 100329
    {
    // In this case we do not write the full distributed array in one object 
    // so just allocate a single element in the array (since it takes space in the file)
        hsize_t xDims1[MAX_ARRAY_DIMENSION];
        for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            xDims1[a]=1;
        dataspace = H5Screate_simple(rank, xDims1, NULL);  
    }
    else
    { // here we create a full size distributed array in the file: 
        dataspace = H5Screate_simple(rank, xDmins, NULL);
    }
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_FLOAT, dataspace, H5P_DEFAULT);
    if( parallelWriteMode==multipleFileIO )
    {
   // attach the array dimensions, xDmins, as an attribute of the dataset (needed for multipleFileIO)
      hsize_t ab_rank=1;
      hsize_t abDims[] = { MAX_ARRAY_DIMENSION };
      hid_t attribspace = H5Screate_simple(ab_rank, abDims, NULL);
   // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(ab_rank, abDims, NULL);
   // *wdh* 100730 hid_t attribID = H5Acreate(datasetID, "arrayDims", H5T_NATIVE_LONG, attribspace, H5P_DEFAULT);
   // NOTE: xDmins is of size hsize_t so we must save it as this size: 
      hid_t attribID = H5Acreate(datasetID, "arrayDims", H5T_NATIVE_HSIZE, attribspace, H5P_DEFAULT);
      if( H5Awrite(attribID, H5T_NATIVE_HSIZE, xDmins)<0 )
      {
          cout << "HDF_DataBase:ERROR:putDistributed(floatArray): could not write array dims for entry name = " << name << endl;
          return 1;
      }
      H5Aclose(attribID);
   // *wdh* 10073 : not used ? H5Sclose(attribmem);
      H5Sclose(attribspace);
    }
    if( true )
    {
    // ******* We save distribution info for this array *******
        ArrayDistributionInfo adi;
        getArrayDistributionInfo( x, adi );
        if( debug & 4  )
        {
            printf(" put(floatArray): sizeof(ArrayDistributionInfo) = %i \n",sizeof(ArrayDistributionInfo));
            const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
            printf(" put: baseProc=%i, nProcs=%i\n",adi.baseProc,adi.nProcs);
            for( int d=0; d<numDim; d++ )
            {
      	printf(" put: d=%i base=%i dimProc=%i left=%i center=%i right=%i\n",
             	       d,adi.base[d],adi.dimProc[d],adi.dimVecL_L[d],adi.dimVecL[d],adi.dimVecL_R[d]);
            }
        }
        hsize_t rank=1;
        hsize_t dims[] = { MAX_ARRAY_DIMENSION*5+2 };
        hid_t attribspace = H5Screate_simple(rank, dims, NULL);
    // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(rank, dims, NULL);
        hid_t attribID = H5Acreate(datasetID, "distribInfo", H5T_NATIVE_INT, attribspace, H5P_DEFAULT);
        if( H5Awrite(attribID, H5T_NATIVE_INT, &adi )<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(floatArray): could not write distribInfo for entry name = "<<name<<endl;
            return 1;
        }
        H5Aclose(attribID);
    // *wdh* 10073 : not used ? H5Sclose(attribmem);
        H5Sclose(attribspace);
    }
  // attach the class name
    setClassName( datasetID, "floatArray");//!!! NOTE : floatArray will be replaced by the macro argument!
    if( parallelWriteMode==multipleFileIO )
    {
        H5Dclose(datasetID);
        H5Gclose(groupID);
        aString serialArrayName = getSerialArrayName( fullGroupPath,name,myid );
        if( debug & 2 ) printf(" putDistributed: put the local array [%s] (%s) on myid=%i\n",
                      (const char*)serialArrayName,(const char*)fullGroupPath,myid);
        HDF_DataBase *sdb = openLocalFile(myid,write);
        assert( sdb != NULL );
        int rt=0;
        rt = sdb->put( xl, serialArrayName );
        if( rt!=0 )
            printf("HDF_DataBase:ERROR:put(floatArray): error getting local array [%s] on myid=%i:",(const char*)serialArrayName,myid);
        closeLocalFile(myid,write);
        if( false )
        {
            aString buff;
            ::display(xl,sPrintF(buff,"PUT local array [%s] on myid=%i",(const char*)serialArrayName,myid));
        }
    // We cannot start a new parallel-put until the last processor in the fileGroup has finished
    // writing to the file. Do this for now: 
        #ifdef USE_PPP
        if( numberOfLocalFilesForWriting!=np )
        {
                MPI_Barrier(Overture::OV_COMM); 
        }
        #endif
        return rt;
    }
    else
    {
    // printf("*********** WARNING parallelWriteMode=%i != multipleFileIO *********** \n",
    //       (int)parallelWriteMode  );
    }
  // now create the dataspace describing the memory image of the array
    if ( !localSize ) rank=0;
    hid_t memspace = H5Screate_simple(rank, xlDims, NULL);
    if ( !localSize ) rank=MAX_ARRAY_DIMENSION;
  // select the hyperslab of the memory space we are going to write
// old version: #define HSIZE_T hssize_t
#define HSIZE_T hsize_t
    HSIZE_T xlOffset[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<rank; a++ )
    {
        if ( xDmins[rank-a-1]>1 && xlDims[rank-a-1]>1 ) // *wdh* 061028
        {
            xlOffset[rank-a-1] = x.getInternalGhostCellWidth(a);
            xlDims[rank-a-1] -= 2*x.getInternalGhostCellWidth(a);
        }
        else
        {
            xlOffset[rank-a-1] = 0;
        }
    }
    if( localSize )
        H5Sselect_hyperslab(memspace, H5S_SELECT_SET, xlOffset, NULL, xlDims, NULL);
  // select the hyperslab of the file dataspace we are going to write to
    HSIZE_T xOffset[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<rank; a++ )
    {
        xOffset[rank-a-1] = x.getLocalBase(a)-x.getBase(a);
        if ( xDmins[rank-a-1]>1 && (x.getLocalBase(a)==xl.getBase(a)) ) 
            xOffset[rank-a-1] += x.getInternalGhostCellWidth(a);
    }
    if ( total_size )
        H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, xOffset, NULL, xlDims, NULL);
    hid_t plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:put(floatArray): could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    if( parallelWriteMode==GenericDataBase::independentIO )
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
    else
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
#endif
    if ( !total_size || !localSize ) 
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
  // printf(" putParallelArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
    if( H5Dwrite(datasetID, H5T_NATIVE_FLOAT, memspace, dataspace, plistID, xl.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:put(floatArray): could not write parallel array with entry name = "<<name<<endl;
        return 1;
    }
    H5Pclose(plistID);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    return 0;
// #endif
}
// putParallelArrayMacro(doubleArray,doubleSerialArray,H5T_NATIVE_DOUBLE);
int 
HDF_DataBase::
putDistributed ( const doubleArray & x, const aString &name )
{
// #ifndef USE_PPP
//   return put(x,name);
// #else  
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put(doubleArray): cannot put an array to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    const int myid = max(0,Communication_Manager::My_Process_Number);
    const int np = Communication_Manager::numberOfProcessors();
  // printf(" putParallelArray: myid=%i name=%s mode=%i\n",myid,(const char*)name,mode);
    if ( mode==streamOutputMode ) 
    {
    // This next call will save the array in non-buffered mode!
        dataBaseBuffer->putDistributedToBuffer(x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  // !!! should this only be done by process 0?
  // get info on the full parallel array
    hsize_t rank = MAX_ARRAY_DIMENSION; // using an A++/P++ macro here...
    hsize_t xDmins[MAX_ARRAY_DIMENSION];
    long int xBase[MAX_ARRAY_DIMENSION];
    hsize_t total_size=1;
    for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { 
    // note we transpose the array here to get the ordering correct in the file
    // (hdf5 saves the array dimensions in the reverse order)
        xDmins[rank-a-1] = x.getLength(a);
        xBase[rank-a-1] = x.getBase(a);
        total_size *= xDmins[rank-a-1];
    }
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( !total_size )
    { 
    // **** The parallel array has ZERO entries -- just write a scalar and detect it later ****
        hid_t dataspace = H5Screate(H5S_SCALAR);
        hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
        hid_t memspace =  H5Screate(H5S_SCALAR);
    // attach the class name
        setClassName( datasetID, "doubleArray");//!!! NOTE : doubleArray will be replaced by the macro argument!
        hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
        plistID = H5Pcreate(H5P_DATASET_XFER);
        if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(doubleArray): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif      
        if ( myid==processorForWriting ) 
        {
      // printf(" put NULL parallelArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
            int dummy=0;
            if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, &dummy)<0 )
            {
      	cout<< "HDF_DataBase:ERROR:put(doubleArray): could not write NULL array with entry name = "<<name<<endl;
      	return 1;
            }
        }
        H5Pclose(plistID);
        H5Sclose(memspace);
        H5Dclose(datasetID);
        H5Sclose(dataspace);
        H5Gclose(groupID);
        return 0;
    }
  // get info on the local portion of the parallel array 
  // doubleSerialArray xl;   xl.reference(x.getLocalArrayWithGhostBoundaries());
  // *wdh* 061214 const doubleSerialArray & xl = x.getLocalArrayWithGhostBoundaries();
    doubleSerialArray xl; getLocalArrayWithGhostBoundaries(x,xl);
  // NOTE: sizeof(hsize_t)=8 sizeof(H5T_NATIVE_LONG)=4 on 32bit and 64 bit
    hsize_t xlDims[MAX_ARRAY_DIMENSION];
    long int xlBase[MAX_ARRAY_DIMENSION];
    hsize_t localSize = 1;
    for ( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { // note we transpose the array here to get the ordering correct in the file
        xlDims[rank-a-1] = xl.getLength(a);
        xlBase[rank-a-1] = xl.getBase(a);
        localSize *= xlDims[rank-a-1];
    }
  // -- create the dataspace telling hdf5 what the file image of the array will be
  // hid_t dataspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t dataspace;
    if( parallelWriteMode==multipleFileIO ) // *wdh* 100329
    {
    // In this case we do not write the full distributed array in one object 
    // so just allocate a single element in the array (since it takes space in the file)
        hsize_t xDims1[MAX_ARRAY_DIMENSION];
        for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            xDims1[a]=1;
        dataspace = H5Screate_simple(rank, xDims1, NULL);  
    }
    else
    { // here we create a full size distributed array in the file: 
        dataspace = H5Screate_simple(rank, xDmins, NULL);
    }
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT);
    if( parallelWriteMode==multipleFileIO )
    {
   // attach the array dimensions, xDmins, as an attribute of the dataset (needed for multipleFileIO)
      hsize_t ab_rank=1;
      hsize_t abDims[] = { MAX_ARRAY_DIMENSION };
      hid_t attribspace = H5Screate_simple(ab_rank, abDims, NULL);
   // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(ab_rank, abDims, NULL);
   // *wdh* 100730 hid_t attribID = H5Acreate(datasetID, "arrayDims", H5T_NATIVE_LONG, attribspace, H5P_DEFAULT);
   // NOTE: xDmins is of size hsize_t so we must save it as this size: 
      hid_t attribID = H5Acreate(datasetID, "arrayDims", H5T_NATIVE_HSIZE, attribspace, H5P_DEFAULT);
      if( H5Awrite(attribID, H5T_NATIVE_HSIZE, xDmins)<0 )
      {
          cout << "HDF_DataBase:ERROR:putDistributed(doubleArray): could not write array dims for entry name = " << name << endl;
          return 1;
      }
      H5Aclose(attribID);
   // *wdh* 10073 : not used ? H5Sclose(attribmem);
      H5Sclose(attribspace);
    }
    if( true )
    {
    // ******* We save distribution info for this array *******
        ArrayDistributionInfo adi;
        getArrayDistributionInfo( x, adi );
        if( debug & 4  )
        {
            printf(" put(doubleArray): sizeof(ArrayDistributionInfo) = %i \n",sizeof(ArrayDistributionInfo));
            const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
            printf(" put: baseProc=%i, nProcs=%i\n",adi.baseProc,adi.nProcs);
            for( int d=0; d<numDim; d++ )
            {
      	printf(" put: d=%i base=%i dimProc=%i left=%i center=%i right=%i\n",
             	       d,adi.base[d],adi.dimProc[d],adi.dimVecL_L[d],adi.dimVecL[d],adi.dimVecL_R[d]);
            }
        }
        hsize_t rank=1;
        hsize_t dims[] = { MAX_ARRAY_DIMENSION*5+2 };
        hid_t attribspace = H5Screate_simple(rank, dims, NULL);
    // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(rank, dims, NULL);
        hid_t attribID = H5Acreate(datasetID, "distribInfo", H5T_NATIVE_INT, attribspace, H5P_DEFAULT);
        if( H5Awrite(attribID, H5T_NATIVE_INT, &adi )<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(doubleArray): could not write distribInfo for entry name = "<<name<<endl;
            return 1;
        }
        H5Aclose(attribID);
    // *wdh* 10073 : not used ? H5Sclose(attribmem);
        H5Sclose(attribspace);
    }
  // attach the class name
    setClassName( datasetID, "doubleArray");//!!! NOTE : doubleArray will be replaced by the macro argument!
    if( parallelWriteMode==multipleFileIO )
    {
        H5Dclose(datasetID);
        H5Gclose(groupID);
        aString serialArrayName = getSerialArrayName( fullGroupPath,name,myid );
        if( debug & 2 ) printf(" putDistributed: put the local array [%s] (%s) on myid=%i\n",
                      (const char*)serialArrayName,(const char*)fullGroupPath,myid);
        HDF_DataBase *sdb = openLocalFile(myid,write);
        assert( sdb != NULL );
        int rt=0;
        rt = sdb->put( xl, serialArrayName );
        if( rt!=0 )
            printf("HDF_DataBase:ERROR:put(doubleArray): error getting local array [%s] on myid=%i:",(const char*)serialArrayName,myid);
        closeLocalFile(myid,write);
        if( false )
        {
            aString buff;
            ::display(xl,sPrintF(buff,"PUT local array [%s] on myid=%i",(const char*)serialArrayName,myid));
        }
    // We cannot start a new parallel-put until the last processor in the fileGroup has finished
    // writing to the file. Do this for now: 
        #ifdef USE_PPP
        if( numberOfLocalFilesForWriting!=np )
        {
                MPI_Barrier(Overture::OV_COMM); 
        }
        #endif
        return rt;
    }
    else
    {
    // printf("*********** WARNING parallelWriteMode=%i != multipleFileIO *********** \n",
    //       (int)parallelWriteMode  );
    }
  // now create the dataspace describing the memory image of the array
    if ( !localSize ) rank=0;
    hid_t memspace = H5Screate_simple(rank, xlDims, NULL);
    if ( !localSize ) rank=MAX_ARRAY_DIMENSION;
  // select the hyperslab of the memory space we are going to write
// old version: #define HSIZE_T hssize_t
#define HSIZE_T hsize_t
    HSIZE_T xlOffset[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<rank; a++ )
    {
        if ( xDmins[rank-a-1]>1 && xlDims[rank-a-1]>1 ) // *wdh* 061028
        {
            xlOffset[rank-a-1] = x.getInternalGhostCellWidth(a);
            xlDims[rank-a-1] -= 2*x.getInternalGhostCellWidth(a);
        }
        else
        {
            xlOffset[rank-a-1] = 0;
        }
    }
    if( localSize )
        H5Sselect_hyperslab(memspace, H5S_SELECT_SET, xlOffset, NULL, xlDims, NULL);
  // select the hyperslab of the file dataspace we are going to write to
    HSIZE_T xOffset[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<rank; a++ )
    {
        xOffset[rank-a-1] = x.getLocalBase(a)-x.getBase(a);
        if ( xDmins[rank-a-1]>1 && (x.getLocalBase(a)==xl.getBase(a)) ) 
            xOffset[rank-a-1] += x.getInternalGhostCellWidth(a);
    }
    if ( total_size )
        H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, xOffset, NULL, xlDims, NULL);
    hid_t plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:put(doubleArray): could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    if( parallelWriteMode==GenericDataBase::independentIO )
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
    else
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
#endif
    if ( !total_size || !localSize ) 
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
  // printf(" putParallelArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
    if( H5Dwrite(datasetID, H5T_NATIVE_DOUBLE, memspace, dataspace, plistID, xl.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:put(doubleArray): could not write parallel array with entry name = "<<name<<endl;
        return 1;
    }
    H5Pclose(plistID);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    return 0;
// #endif
}
// putParallelArrayMacro(intArray,intSerialArray,H5T_NATIVE_INT);
int 
HDF_DataBase::
putDistributed ( const intArray & x, const aString &name )
{
// #ifndef USE_PPP
//   return put(x,name);
// #else  
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put(intArray): cannot put an array to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    const int myid = max(0,Communication_Manager::My_Process_Number);
    const int np = Communication_Manager::numberOfProcessors();
  // printf(" putParallelArray: myid=%i name=%s mode=%i\n",myid,(const char*)name,mode);
    if ( mode==streamOutputMode ) 
    {
    // This next call will save the array in non-buffered mode!
        dataBaseBuffer->putDistributedToBuffer(x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  // !!! should this only be done by process 0?
  // get info on the full parallel array
    hsize_t rank = MAX_ARRAY_DIMENSION; // using an A++/P++ macro here...
    hsize_t xDmins[MAX_ARRAY_DIMENSION];
    long int xBase[MAX_ARRAY_DIMENSION];
    hsize_t total_size=1;
    for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { 
    // note we transpose the array here to get the ordering correct in the file
    // (hdf5 saves the array dimensions in the reverse order)
        xDmins[rank-a-1] = x.getLength(a);
        xBase[rank-a-1] = x.getBase(a);
        total_size *= xDmins[rank-a-1];
    }
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( !total_size )
    { 
    // **** The parallel array has ZERO entries -- just write a scalar and detect it later ****
        hid_t dataspace = H5Screate(H5S_SCALAR);
        hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
        hid_t memspace =  H5Screate(H5S_SCALAR);
    // attach the class name
        setClassName( datasetID, "intArray");//!!! NOTE : intArray will be replaced by the macro argument!
        hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
        plistID = H5Pcreate(H5P_DATASET_XFER);
        if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(intArray): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif      
        if ( myid==processorForWriting ) 
        {
      // printf(" put NULL parallelArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
            int dummy=0;
            if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, &dummy)<0 )
            {
      	cout<< "HDF_DataBase:ERROR:put(intArray): could not write NULL array with entry name = "<<name<<endl;
      	return 1;
            }
        }
        H5Pclose(plistID);
        H5Sclose(memspace);
        H5Dclose(datasetID);
        H5Sclose(dataspace);
        H5Gclose(groupID);
        return 0;
    }
  // get info on the local portion of the parallel array 
  // intSerialArray xl;   xl.reference(x.getLocalArrayWithGhostBoundaries());
  // *wdh* 061214 const intSerialArray & xl = x.getLocalArrayWithGhostBoundaries();
    intSerialArray xl; getLocalArrayWithGhostBoundaries(x,xl);
  // NOTE: sizeof(hsize_t)=8 sizeof(H5T_NATIVE_LONG)=4 on 32bit and 64 bit
    hsize_t xlDims[MAX_ARRAY_DIMENSION];
    long int xlBase[MAX_ARRAY_DIMENSION];
    hsize_t localSize = 1;
    for ( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    { // note we transpose the array here to get the ordering correct in the file
        xlDims[rank-a-1] = xl.getLength(a);
        xlBase[rank-a-1] = xl.getBase(a);
        localSize *= xlDims[rank-a-1];
    }
  // -- create the dataspace telling hdf5 what the file image of the array will be
  // hid_t dataspace = H5Screate_simple(rank, xDmins, NULL);
    hid_t dataspace;
    if( parallelWriteMode==multipleFileIO ) // *wdh* 100329
    {
    // In this case we do not write the full distributed array in one object 
    // so just allocate a single element in the array (since it takes space in the file)
        hsize_t xDims1[MAX_ARRAY_DIMENSION];
        for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
            xDims1[a]=1;
        dataspace = H5Screate_simple(rank, xDims1, NULL);  
    }
    else
    { // here we create a full size distributed array in the file: 
        dataspace = H5Screate_simple(rank, xDmins, NULL);
    }
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
    if( parallelWriteMode==multipleFileIO )
    {
   // attach the array dimensions, xDmins, as an attribute of the dataset (needed for multipleFileIO)
      hsize_t ab_rank=1;
      hsize_t abDims[] = { MAX_ARRAY_DIMENSION };
      hid_t attribspace = H5Screate_simple(ab_rank, abDims, NULL);
   // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(ab_rank, abDims, NULL);
   // *wdh* 100730 hid_t attribID = H5Acreate(datasetID, "arrayDims", H5T_NATIVE_LONG, attribspace, H5P_DEFAULT);
   // NOTE: xDmins is of size hsize_t so we must save it as this size: 
      hid_t attribID = H5Acreate(datasetID, "arrayDims", H5T_NATIVE_HSIZE, attribspace, H5P_DEFAULT);
      if( H5Awrite(attribID, H5T_NATIVE_HSIZE, xDmins)<0 )
      {
          cout << "HDF_DataBase:ERROR:putDistributed(intArray): could not write array dims for entry name = " << name << endl;
          return 1;
      }
      H5Aclose(attribID);
   // *wdh* 10073 : not used ? H5Sclose(attribmem);
      H5Sclose(attribspace);
    }
    if( true )
    {
    // ******* We save distribution info for this array *******
        ArrayDistributionInfo adi;
        getArrayDistributionInfo( x, adi );
        if( debug & 4  )
        {
            printf(" put(intArray): sizeof(ArrayDistributionInfo) = %i \n",sizeof(ArrayDistributionInfo));
            const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
            printf(" put: baseProc=%i, nProcs=%i\n",adi.baseProc,adi.nProcs);
            for( int d=0; d<numDim; d++ )
            {
      	printf(" put: d=%i base=%i dimProc=%i left=%i center=%i right=%i\n",
             	       d,adi.base[d],adi.dimProc[d],adi.dimVecL_L[d],adi.dimVecL[d],adi.dimVecL_R[d]);
            }
        }
        hsize_t rank=1;
        hsize_t dims[] = { MAX_ARRAY_DIMENSION*5+2 };
        hid_t attribspace = H5Screate_simple(rank, dims, NULL);
    // *wdh* 10073 : not used ? hid_t attribmem = H5Screate_simple(rank, dims, NULL);
        hid_t attribID = H5Acreate(datasetID, "distribInfo", H5T_NATIVE_INT, attribspace, H5P_DEFAULT);
        if( H5Awrite(attribID, H5T_NATIVE_INT, &adi )<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(intArray): could not write distribInfo for entry name = "<<name<<endl;
            return 1;
        }
        H5Aclose(attribID);
    // *wdh* 10073 : not used ? H5Sclose(attribmem);
        H5Sclose(attribspace);
    }
  // attach the class name
    setClassName( datasetID, "intArray");//!!! NOTE : intArray will be replaced by the macro argument!
    if( parallelWriteMode==multipleFileIO )
    {
        H5Dclose(datasetID);
        H5Gclose(groupID);
        aString serialArrayName = getSerialArrayName( fullGroupPath,name,myid );
        if( debug & 2 ) printf(" putDistributed: put the local array [%s] (%s) on myid=%i\n",
                      (const char*)serialArrayName,(const char*)fullGroupPath,myid);
        HDF_DataBase *sdb = openLocalFile(myid,write);
        assert( sdb != NULL );
        int rt=0;
        rt = sdb->put( xl, serialArrayName );
        if( rt!=0 )
            printf("HDF_DataBase:ERROR:put(intArray): error getting local array [%s] on myid=%i:",(const char*)serialArrayName,myid);
        closeLocalFile(myid,write);
        if( false )
        {
            aString buff;
            ::display(xl,sPrintF(buff,"PUT local array [%s] on myid=%i",(const char*)serialArrayName,myid));
        }
    // We cannot start a new parallel-put until the last processor in the fileGroup has finished
    // writing to the file. Do this for now: 
        #ifdef USE_PPP
        if( numberOfLocalFilesForWriting!=np )
        {
                MPI_Barrier(Overture::OV_COMM); 
        }
        #endif
        return rt;
    }
    else
    {
    // printf("*********** WARNING parallelWriteMode=%i != multipleFileIO *********** \n",
    //       (int)parallelWriteMode  );
    }
  // now create the dataspace describing the memory image of the array
    if ( !localSize ) rank=0;
    hid_t memspace = H5Screate_simple(rank, xlDims, NULL);
    if ( !localSize ) rank=MAX_ARRAY_DIMENSION;
  // select the hyperslab of the memory space we are going to write
// old version: #define HSIZE_T hssize_t
#define HSIZE_T hsize_t
    HSIZE_T xlOffset[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<rank; a++ )
    {
        if ( xDmins[rank-a-1]>1 && xlDims[rank-a-1]>1 ) // *wdh* 061028
        {
            xlOffset[rank-a-1] = x.getInternalGhostCellWidth(a);
            xlDims[rank-a-1] -= 2*x.getInternalGhostCellWidth(a);
        }
        else
        {
            xlOffset[rank-a-1] = 0;
        }
    }
    if( localSize )
        H5Sselect_hyperslab(memspace, H5S_SELECT_SET, xlOffset, NULL, xlDims, NULL);
  // select the hyperslab of the file dataspace we are going to write to
    HSIZE_T xOffset[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<rank; a++ )
    {
        xOffset[rank-a-1] = x.getLocalBase(a)-x.getBase(a);
        if ( xDmins[rank-a-1]>1 && (x.getLocalBase(a)==xl.getBase(a)) ) 
            xOffset[rank-a-1] += x.getInternalGhostCellWidth(a);
    }
    if ( total_size )
        H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, xOffset, NULL, xlDims, NULL);
    hid_t plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:put(intArray): could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    if( parallelWriteMode==GenericDataBase::independentIO )
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
    else
        H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
#endif
    if ( !total_size || !localSize ) 
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
  // printf(" putParallelArray:H5Dwrite myid=%i name=%s\n",myid,(const char*)name);
    if( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, xl.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:put(intArray): could not write parallel array with entry name = "<<name<<endl;
        return 1;
    }
    H5Pclose(plistID);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    return 0;
// #endif
}
      	
//=====================================================================================  
// /Description: get an A++ array from the data-base.   
// /x (output): the array to get. This array will be redim'd to have the  
//   correct dimensions (base and bound).  
// /name (input): the name of the array in the data-base  
// /Iv[6] (input): optionally specify the Index's of a sub-array to get.
//=====================================================================================  

// getSerialArrayMacro(floatSerialArray,H5T_NATIVE_FLOAT);
int 
HDF_DataBase::
get( floatSerialArray &x, const aString &name, Index *Iv /* =NULL */ ) const
{
    double time0=MPI_Wtime();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//   if ( accessMode!=read )
//     {
//       cout<< "HDF_DataBase:ERROR:put: cannot get an array from a write-only file, entry name = "<<name<<endl;
//       return 1;
//     }
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
  // all processes read this data
#endif
    if ( mode==streamInputMode ) 
    {
        if( false )
        {
            dataBaseBuffer->getFromBuffer(x);
        }
        else
        {
            /* get from the stream buffer */ 
            int dims[MAX_ARRAY_DIMENSION][2];  
            dataBaseBuffer->getFromBuffer( MAX_ARRAY_DIMENSION*2, dims[0] );  
            int size=1;  
            for( int d=0; d<MAX_ARRAY_DIMENSION; d++)  
      	size*=(dims[d][1]-dims[d][0]+1);  
            x.redim(Range(dims[0][0],dims[0][1]),  
            	      Range(dims[1][0],dims[1][1]),  
            	      Range(dims[2][0],dims[2][1]),  
            	      Range(dims[3][0],dims[3][1]));  
            dataBaseBuffer->getFromBuffer( size,x.getDataPointer() );  
        }
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(floatSerialArray): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
            printf("HDF_DataBase:ERROR:get(floatSerialArray): cannot get entry with name = %s (directory=%s)\n",
                      (const char*)name,(const char*)fullGroupPath);
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get(floatSerialArray): non-simple dataspace entry with name = "<<name<<endl;
        return 1;
    }
    if ( H5Sget_simple_extent_type(dataspace)==H5S_SCALAR ) 
    { // this was a null array
        x.redim(0);
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    } 
  // obtain the bounds and dimensions of the array from the dataset and dataspace
    hsize_t ndims =   H5Sget_simple_extent_ndims(dataspace);
    if ( ndims>MAX_ARRAY_DIMENSION )
    {
        cout<< "HDF_DataBase:ERROR:get(floatSerialArray): array in database has too many dimensions,  entry name = "<<name<<endl;
        return 1;
    }
    hsize_t dims[MAX_ARRAY_DIMENSION];
    H5Sget_simple_extent_dims(dataspace,dims,NULL);
    long int arrayBase[MAX_ARRAY_DIMENSION];
    hid_t attribID = H5Aopen_name(datasetID, "arrayBase");
    if ( attribID<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not find arrayBase attribute for entry with name = "<<name<<endl;
        return 1;
    }
    hid_t attribspace = H5Aget_space(attribID);
    H5Aread(attribID, H5T_NATIVE_LONG, arrayBase);
    H5Sclose(attribspace);
    H5Aclose(attribID);
    if ( ndims==0 )
    {
    // this was a null array, our work here is done
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    }
    bool subArrayRequested=false;
    if( Iv !=NULL )
    {
    // user has requested a sub-array to be read in.
        subArrayRequested=true;
        hsize_t offset[MAX_ARRAY_DIMENSION];
        for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
        {
            if( Iv[a].getBase() <arrayBase[ndims-1-a] ||
                    Iv[a].getBound()>arrayBase[ndims-1-a]+dims[ndims-1-a]-1 )
            {
      	printf("HDF5_DataBase:get(floatSerialArray):ERROR: invalid sub-array requested, name=%s\n",
                            (const char*)name);
                printf(" array dimension %i, array-[base,bound]=[%i,%i], requested [%i,%i]\n",
             	       a,arrayBase[ndims-1-a],arrayBase[ndims-1-a]+dims[ndims-1-a]-1,Iv[a].getBase(),
                              Iv[a].getBound());
                printf("... will choose largest available sub-array.\n");
            }
            int base = max((int)arrayBase[ndims-1-a],Iv[a].getBase());
       // set the offset for the sub-array being read in :
            offset[ndims-1-a]= base - arrayBase[ndims-1-a];
      // new base and dimensions:
            arrayBase[ndims-1-a] = base;
            dims[ndims-1-a] = min(dims[ndims-1-a], Iv[a].getBound()-Iv[a].getBase()+1 );
        }
    // Define hyperslab in the dataset. 
        herr_t status = H5Sselect_hyperslab (dataspace, H5S_SELECT_SET, offset, NULL, dims, NULL);
    }
  // only redim the array if it is not already the same size.
    bool sameDimensions=true;
    for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    {
        if( x.getBase(a)!=arrayBase[ndims-1-a] || x.getLength(a)!=dims[ndims-1-a] )
        {
            sameDimensions=false;
            break;
        }
    }
    if( !sameDimensions )
    {
        x.redim( Range(arrayBase[ndims-1], arrayBase[ndims-1]+dims[ndims-1]-1),
           	     Range(arrayBase[ndims-2], arrayBase[ndims-2]+dims[ndims-2]-1),
           	     Range(arrayBase[ndims-3], arrayBase[ndims-3]+dims[ndims-3]-1),
           	     Range(arrayBase[ndims-4], arrayBase[ndims-4]+dims[ndims-4]-1)
#if MAX_ARRAY_DIMENSION>4
           	     ,Range(arrayBase[ndims-5], arrayBase[ndims-5]+dims[ndims-5]-1)
#endif
#if MAX_ARRAY_DIMENSION>5
           	     ,Range(arrayBase[ndims-6], arrayBase[ndims-6]+dims[ndims-6]-1)
#endif
#if MAX_ARRAY_DIMENSION>6
           	     ,Range(arrayBase[ndims-7], arrayBase[ndims-7]+dims[ndims-7]-1)
#endif
            );
    }
  // describe the array in memory
    hid_t memspace = H5Screate_simple(ndims, dims, NULL);
    int plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
    if ( H5Dread(datasetID, H5T_NATIVE_FLOAT, memspace, dataspace, plistID, x.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not read serial array with entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    timeForSerialGetArray+=MPI_Wtime()-time0;
    return 0;
}
// getSerialArrayMacro(doubleSerialArray,H5T_NATIVE_DOUBLE);
int 
HDF_DataBase::
get( doubleSerialArray &x, const aString &name, Index *Iv /* =NULL */ ) const
{
    double time0=MPI_Wtime();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//   if ( accessMode!=read )
//     {
//       cout<< "HDF_DataBase:ERROR:put: cannot get an array from a write-only file, entry name = "<<name<<endl;
//       return 1;
//     }
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
  // all processes read this data
#endif
    if ( mode==streamInputMode ) 
    {
        if( false )
        {
            dataBaseBuffer->getFromBuffer(x);
        }
        else
        {
            /* get from the stream buffer */ 
            int dims[MAX_ARRAY_DIMENSION][2];  
            dataBaseBuffer->getFromBuffer( MAX_ARRAY_DIMENSION*2, dims[0] );  
            int size=1;  
            for( int d=0; d<MAX_ARRAY_DIMENSION; d++)  
      	size*=(dims[d][1]-dims[d][0]+1);  
            x.redim(Range(dims[0][0],dims[0][1]),  
            	      Range(dims[1][0],dims[1][1]),  
            	      Range(dims[2][0],dims[2][1]),  
            	      Range(dims[3][0],dims[3][1]));  
            dataBaseBuffer->getFromBuffer( size,x.getDataPointer() );  
        }
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(doubleSerialArray): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
            printf("HDF_DataBase:ERROR:get(doubleSerialArray): cannot get entry with name = %s (directory=%s)\n",
                      (const char*)name,(const char*)fullGroupPath);
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get(doubleSerialArray): non-simple dataspace entry with name = "<<name<<endl;
        return 1;
    }
    if ( H5Sget_simple_extent_type(dataspace)==H5S_SCALAR ) 
    { // this was a null array
        x.redim(0);
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    } 
  // obtain the bounds and dimensions of the array from the dataset and dataspace
    hsize_t ndims =   H5Sget_simple_extent_ndims(dataspace);
    if ( ndims>MAX_ARRAY_DIMENSION )
    {
        cout<< "HDF_DataBase:ERROR:get(doubleSerialArray): array in database has too many dimensions,  entry name = "<<name<<endl;
        return 1;
    }
    hsize_t dims[MAX_ARRAY_DIMENSION];
    H5Sget_simple_extent_dims(dataspace,dims,NULL);
    long int arrayBase[MAX_ARRAY_DIMENSION];
    hid_t attribID = H5Aopen_name(datasetID, "arrayBase");
    if ( attribID<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not find arrayBase attribute for entry with name = "<<name<<endl;
        return 1;
    }
    hid_t attribspace = H5Aget_space(attribID);
    H5Aread(attribID, H5T_NATIVE_LONG, arrayBase);
    H5Sclose(attribspace);
    H5Aclose(attribID);
    if ( ndims==0 )
    {
    // this was a null array, our work here is done
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    }
    bool subArrayRequested=false;
    if( Iv !=NULL )
    {
    // user has requested a sub-array to be read in.
        subArrayRequested=true;
        hsize_t offset[MAX_ARRAY_DIMENSION];
        for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
        {
            if( Iv[a].getBase() <arrayBase[ndims-1-a] ||
                    Iv[a].getBound()>arrayBase[ndims-1-a]+dims[ndims-1-a]-1 )
            {
      	printf("HDF5_DataBase:get(doubleSerialArray):ERROR: invalid sub-array requested, name=%s\n",
                            (const char*)name);
                printf(" array dimension %i, array-[base,bound]=[%i,%i], requested [%i,%i]\n",
             	       a,arrayBase[ndims-1-a],arrayBase[ndims-1-a]+dims[ndims-1-a]-1,Iv[a].getBase(),
                              Iv[a].getBound());
                printf("... will choose largest available sub-array.\n");
            }
            int base = max((int)arrayBase[ndims-1-a],Iv[a].getBase());
       // set the offset for the sub-array being read in :
            offset[ndims-1-a]= base - arrayBase[ndims-1-a];
      // new base and dimensions:
            arrayBase[ndims-1-a] = base;
            dims[ndims-1-a] = min(dims[ndims-1-a], Iv[a].getBound()-Iv[a].getBase()+1 );
        }
    // Define hyperslab in the dataset. 
        herr_t status = H5Sselect_hyperslab (dataspace, H5S_SELECT_SET, offset, NULL, dims, NULL);
    }
  // only redim the array if it is not already the same size.
    bool sameDimensions=true;
    for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    {
        if( x.getBase(a)!=arrayBase[ndims-1-a] || x.getLength(a)!=dims[ndims-1-a] )
        {
            sameDimensions=false;
            break;
        }
    }
    if( !sameDimensions )
    {
        x.redim( Range(arrayBase[ndims-1], arrayBase[ndims-1]+dims[ndims-1]-1),
           	     Range(arrayBase[ndims-2], arrayBase[ndims-2]+dims[ndims-2]-1),
           	     Range(arrayBase[ndims-3], arrayBase[ndims-3]+dims[ndims-3]-1),
           	     Range(arrayBase[ndims-4], arrayBase[ndims-4]+dims[ndims-4]-1)
#if MAX_ARRAY_DIMENSION>4
           	     ,Range(arrayBase[ndims-5], arrayBase[ndims-5]+dims[ndims-5]-1)
#endif
#if MAX_ARRAY_DIMENSION>5
           	     ,Range(arrayBase[ndims-6], arrayBase[ndims-6]+dims[ndims-6]-1)
#endif
#if MAX_ARRAY_DIMENSION>6
           	     ,Range(arrayBase[ndims-7], arrayBase[ndims-7]+dims[ndims-7]-1)
#endif
            );
    }
  // describe the array in memory
    hid_t memspace = H5Screate_simple(ndims, dims, NULL);
    int plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
    if ( H5Dread(datasetID, H5T_NATIVE_DOUBLE, memspace, dataspace, plistID, x.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not read serial array with entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    timeForSerialGetArray+=MPI_Wtime()-time0;
    return 0;
}
// getSerialArrayMacro(intSerialArray,H5T_NATIVE_INT);
int 
HDF_DataBase::
get( intSerialArray &x, const aString &name, Index *Iv /* =NULL */ ) const
{
    double time0=MPI_Wtime();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//   if ( accessMode!=read )
//     {
//       cout<< "HDF_DataBase:ERROR:put: cannot get an array from a write-only file, entry name = "<<name<<endl;
//       return 1;
//     }
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
  // all processes read this data
#endif
    if ( mode==streamInputMode ) 
    {
        if( false )
        {
            dataBaseBuffer->getFromBuffer(x);
        }
        else
        {
            /* get from the stream buffer */ 
            int dims[MAX_ARRAY_DIMENSION][2];  
            dataBaseBuffer->getFromBuffer( MAX_ARRAY_DIMENSION*2, dims[0] );  
            int size=1;  
            for( int d=0; d<MAX_ARRAY_DIMENSION; d++)  
      	size*=(dims[d][1]-dims[d][0]+1);  
            x.redim(Range(dims[0][0],dims[0][1]),  
            	      Range(dims[1][0],dims[1][1]),  
            	      Range(dims[2][0],dims[2][1]),  
            	      Range(dims[3][0],dims[3][1]));  
            dataBaseBuffer->getFromBuffer( size,x.getDataPointer() );  
        }
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(intSerialArray): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
            printf("HDF_DataBase:ERROR:get(intSerialArray): cannot get entry with name = %s (directory=%s)\n",
                      (const char*)name,(const char*)fullGroupPath);
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get(intSerialArray): non-simple dataspace entry with name = "<<name<<endl;
        return 1;
    }
    if ( H5Sget_simple_extent_type(dataspace)==H5S_SCALAR ) 
    { // this was a null array
        x.redim(0);
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    } 
  // obtain the bounds and dimensions of the array from the dataset and dataspace
    hsize_t ndims =   H5Sget_simple_extent_ndims(dataspace);
    if ( ndims>MAX_ARRAY_DIMENSION )
    {
        cout<< "HDF_DataBase:ERROR:get(intSerialArray): array in database has too many dimensions,  entry name = "<<name<<endl;
        return 1;
    }
    hsize_t dims[MAX_ARRAY_DIMENSION];
    H5Sget_simple_extent_dims(dataspace,dims,NULL);
    long int arrayBase[MAX_ARRAY_DIMENSION];
    hid_t attribID = H5Aopen_name(datasetID, "arrayBase");
    if ( attribID<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not find arrayBase attribute for entry with name = "<<name<<endl;
        return 1;
    }
    hid_t attribspace = H5Aget_space(attribID);
    H5Aread(attribID, H5T_NATIVE_LONG, arrayBase);
    H5Sclose(attribspace);
    H5Aclose(attribID);
    if ( ndims==0 )
    {
    // this was a null array, our work here is done
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    }
    bool subArrayRequested=false;
    if( Iv !=NULL )
    {
    // user has requested a sub-array to be read in.
        subArrayRequested=true;
        hsize_t offset[MAX_ARRAY_DIMENSION];
        for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
        {
            if( Iv[a].getBase() <arrayBase[ndims-1-a] ||
                    Iv[a].getBound()>arrayBase[ndims-1-a]+dims[ndims-1-a]-1 )
            {
      	printf("HDF5_DataBase:get(intSerialArray):ERROR: invalid sub-array requested, name=%s\n",
                            (const char*)name);
                printf(" array dimension %i, array-[base,bound]=[%i,%i], requested [%i,%i]\n",
             	       a,arrayBase[ndims-1-a],arrayBase[ndims-1-a]+dims[ndims-1-a]-1,Iv[a].getBase(),
                              Iv[a].getBound());
                printf("... will choose largest available sub-array.\n");
            }
            int base = max((int)arrayBase[ndims-1-a],Iv[a].getBase());
       // set the offset for the sub-array being read in :
            offset[ndims-1-a]= base - arrayBase[ndims-1-a];
      // new base and dimensions:
            arrayBase[ndims-1-a] = base;
            dims[ndims-1-a] = min(dims[ndims-1-a], Iv[a].getBound()-Iv[a].getBase()+1 );
        }
    // Define hyperslab in the dataset. 
        herr_t status = H5Sselect_hyperslab (dataspace, H5S_SELECT_SET, offset, NULL, dims, NULL);
    }
  // only redim the array if it is not already the same size.
    bool sameDimensions=true;
    for( int a=0; a<MAX_ARRAY_DIMENSION; a++ )
    {
        if( x.getBase(a)!=arrayBase[ndims-1-a] || x.getLength(a)!=dims[ndims-1-a] )
        {
            sameDimensions=false;
            break;
        }
    }
    if( !sameDimensions )
    {
        x.redim( Range(arrayBase[ndims-1], arrayBase[ndims-1]+dims[ndims-1]-1),
           	     Range(arrayBase[ndims-2], arrayBase[ndims-2]+dims[ndims-2]-1),
           	     Range(arrayBase[ndims-3], arrayBase[ndims-3]+dims[ndims-3]-1),
           	     Range(arrayBase[ndims-4], arrayBase[ndims-4]+dims[ndims-4]-1)
#if MAX_ARRAY_DIMENSION>4
           	     ,Range(arrayBase[ndims-5], arrayBase[ndims-5]+dims[ndims-5]-1)
#endif
#if MAX_ARRAY_DIMENSION>5
           	     ,Range(arrayBase[ndims-6], arrayBase[ndims-6]+dims[ndims-6]-1)
#endif
#if MAX_ARRAY_DIMENSION>6
           	     ,Range(arrayBase[ndims-7], arrayBase[ndims-7]+dims[ndims-7]-1)
#endif
            );
    }
  // describe the array in memory
    hid_t memspace = H5Screate_simple(ndims, dims, NULL);
    int plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
    if ( H5Dread(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, x.getDataPointer())<0 )
    {
        cout<< "HDF_DataBase:ERROR:get: could not read serial array with entry name = "<<name<<endl;
        return 1;
    }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    timeForSerialGetArray+=MPI_Wtime()-time0;
    return 0;
}


//=====================================================================================  
// /Description: get an A++ array from the data-base.   
// /x (output): the array to get. This array will be redim'd to have the  
//   correct dimensions (base and bound).  
// /name (input): the name of the array in the data-base  
//=====================================================================================  
// Define a macro to retrive a float/int/double P++ Array
// type=floatArray/intArray/doubleArray HDFType=corresponding HDF type

// declare instances for the parallel get macro
// getParallelArrayMacro(floatArray,floatSerialArray,H5T_NATIVE_FLOAT);
int 
HDF_DataBase::
getDistributed( floatArray &x, const aString &name ) const
{
// #ifndef USE_PPP
//   return get(x,name);
// #else  
    double time0=getCPU();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//  if ( accessMode!=read )
//  {
//    cout<< "HDF_DataBase:ERROR:get: cannot get a parallel array from a write-only file, entry name = "<<name<<endl;
//    return 1;
//  }
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( mode==streamInputMode ) 
    {
        dataBaseBuffer->getDistributedFromBuffer(x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:getDistributed(floatArray): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
        {
            cout<< "HDF_DataBase:ERROR:getDistributed(floatArray): cannot get entry with name = "<<name<<endl;
            Overture::abort("error");
        }
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:getDistributed(floatArray): non-simple dataspace entry with name = "<<name<<endl;
        return 1;
    }
    if ( H5Sget_simple_extent_type(dataspace)==H5S_SCALAR ) 
    { // this was a null array
        x.redim(0);
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    } 
  // obtain the bounds and dimensions of the array from the dataset and dataspace
    hsize_t ndims =   H5Sget_simple_extent_ndims(dataspace);
    if ( ndims>MAX_ARRAY_DIMENSION )
    {
        cout<< "HDF_DataBase:ERROR:get(floatArray): array in database has too many dimensions,  entry name = "<<name<<endl;
        return 1;
    }
    hsize_t dims[MAX_ARRAY_DIMENSION];
    H5Sget_simple_extent_dims(dataspace,dims,NULL);  // get array dimensions (these will be set to 1 if parallelReadMode==multipleFileIO)
  // cout << " dims =" << dims[0] << dims[1] << dims[2] << dims[3] << dims[4] << dims[5] << endl;
  //  printF("HDF5: getDistributed: sizeof(hsize_t)=%i sizeof(H5T_NATIVE_LONG)=%i dims=[%lli,%lli,%lli,%lli,%lli,%lli]]\n",
  // 	 sizeof(hsize_t),sizeof(H5T_NATIVE_LONG),dims[0],dims[1],dims[2],dims[3],dims[4],dims[5]);
//   cout<<"dims, procID "<<myid<<" : ";
//   for ( int a=0; a<ndims; a++ )
//     {
//       cout<<dims[a]<<"  ";
//     }
//   cout<<endl;
//   MPI_Barrier(MPI_COMM_WORLD);
  // ******* get the full array dimensions for this array *******
    if( parallelReadMode==multipleFileIO )
    {
        hid_t attribID = H5Aopen_name(datasetID, "arrayDims");
        if ( attribID<=0 )
        {
            cout<< "HDF_DataBase:ERROR:getDistributed(floatArray): could not find arrayDims attribute for entry with name = "<<name<<endl;
            return 1;
        }
        hid_t attribspace = H5Aget_space(attribID);
    // *wdh* 100730 H5Aread(attribID, H5T_NATIVE_LONG, dims);
        H5Aread(attribID, H5T_NATIVE_HSIZE, dims);
        H5Sclose(attribspace);
        H5Aclose(attribID);
    // for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    //   printf(" HDF_DataBase: array dims[%i]=%li\n",d,dims[d]);
    }
  // --- get the parallel distribution info ---
    ArrayDistributionInfo adi;
    long int arrayBase[MAX_ARRAY_DIMENSION];
    if( true )
    {
        hid_t attribID = H5Aopen_name(datasetID, "distribInfo");
        if ( attribID<=0 )
        {
            cout<< "HDF_DataBase:ERROR:get(floatArray): could not find distribInfo for entry with name = "<<name<<endl;
            return 1;
        }
        hid_t attribspace = H5Aget_space(attribID);
        H5Aread(attribID, H5T_NATIVE_INT, &adi);
        H5Sclose(attribspace);
        H5Aclose(attribID);
        for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
                arrayBase[d]=0;
        for( int d=0; d<ndims; d++ )
        {
              arrayBase[ndims-1-d]= (long)adi.base[d];
        }
        if( debug & 2  )
        {
            const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
            printf(" get: baseProc=%i, nProcs=%i\n",adi.baseProc,adi.nProcs);
            for( int d=0; d<numDim; d++ )
            {
      	printf(" get: d=%i base=%i dimProc=%i left=%i center=%i right=%i\n",
             	       d,adi.base[d],adi.dimProc[d],adi.dimVecL_L[d],adi.dimVecL[d],adi.dimVecL_R[d]);
            }
        }
    }
    if ( ndims==0 )
    {
    // this was a null array, our work here is done
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    }
    x.redim( Range(arrayBase[ndims-1], arrayBase[ndims-1]+dims[ndims-1]-1),
         	   Range(arrayBase[ndims-2], arrayBase[ndims-2]+dims[ndims-2]-1),
         	   Range(arrayBase[ndims-3], arrayBase[ndims-3]+dims[ndims-3]-1),
         	   Range(arrayBase[ndims-4], arrayBase[ndims-4]+dims[ndims-4]-1)
#if MAX_ARRAY_DIMENSION>4
        	  ,Range(arrayBase[ndims-5], arrayBase[ndims-5]+dims[ndims-5]-1)
#endif
#if MAX_ARRAY_DIMENSION>5
        	  ,Range(arrayBase[ndims-6], arrayBase[ndims-6]+dims[ndims-6]-1)
#endif
#if MAX_ARRAY_DIMENSION>6
        	  ,Range(arrayBase[ndims-7], arrayBase[ndims-7]+dims[ndims-7]-1)
#endif
         	   );
    floatSerialArray xl;
  // *wdh* 061214 xl.reference(x.getLocalArrayWithGhostBoundaries());
    getLocalArrayWithGhostBoundaries(x,xl);
    if( parallelReadMode==multipleFileIO )
    {
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
    // --- new way ---
    // --- Read in the local array data from the serial data base files ---
    //     We need to determine which files to read to get all the data
    //  Given : local-array [base,bound] 
    //          file-local-array : [base,bound] 
    // NOTE: If we intersect boxes that do not include ghost points, but then copy ghost points,
    //       we can avoid reading extra files. 
        int rt=0;
        if( debug & 2 )
            printf("*** getDistributed(floatArray) myid=%i numberOfLocalFilesForReading=%i\n",myid,numberOfLocalFilesForReading);
        bool readParallel= numberOfLocalFilesForReading>0;
        #ifdef USE_PPP
        readParallel=true;  // in parallel we always must read in parallel (since we may have ghost points)
        #endif
        if( readParallel )
        {
      // -- file was written in parallel ---
            IndexBox xBox; // box of points we need, (NO ghost)
            CopyArray::getLocalArrayBox( myid, x, xBox );
            if( debug & 2 )
      	printf("*** myid=%i %s local-array box=[%i,%i][%i,%i]\n",myid,(const char*)name,
                                              xBox.base(0),xBox.bound(0),xBox.base(1),xBox.bound(1));
            for( int p=adi.baseProc; p<adi.baseProc+adi.nProcs; p++ )
            {
      	IndexBox pBox, iBox;
      	getLocalArrayBox( p, adi, pBox );
      	if( debug & 2 )
        	  printf(" myid=%i : array in file for p=%i has box=[%i,%i][%i,%i][%i,%i][%i,%i]\n",myid,p,
             		 pBox.base(0),pBox.bound(0),pBox.base(1),pBox.bound(1),pBox.base(2),pBox.bound(2),pBox.base(3),pBox.bound(3));
      	if( IndexBox::intersect(xBox, pBox, iBox) )
      	{
        	  if( debug & 2 )
          	    printf(" myid=%i local-array data must be read from processor p=%i, iBox=[%i,%i][%i,%i]\n",
               		   myid,p,iBox.base(0),iBox.bound(0),iBox.base(1),iBox.bound(1));
                    HDF_DataBase *sdb = openLocalFile(p,read);
        	  assert( sdb != NULL );
        	  aString serialArrayName = getSerialArrayName( fullGroupPath,name,p );
	  // NOTE: What if the ghost boundary width is not the same!
	  //  -- this could occur if an array was saved with 1-parallel-ghost but read back
	  //    in with 2 parallel-ghost --> we could detect this and force and updateGhostBoundaries
	  // NOTE: ** we only read the part of the array we need, to avoid reading in a large array ***
        	  Index Iv[MAX_ARRAY_DIMENSION];
        	  int nDim=min(x.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
        	  for( int d=0; d<nDim; d++ )
        	  { // read parallel-ghost (but no need to read pts outside the array dimensions)
          	    int base = max(x.getBase(d),iBox.base(d)-x.getGhostBoundaryWidth(d));
          	    int bound= min(x.getBound(d),iBox.bound(d)+x.getGhostBoundaryWidth(d));
          	    Iv[d] = Range(base,bound);
        	  }
        	  for( int d=nDim; d< MAX_ARRAY_DIMENSION; d++ )
        	  {
          	    Iv[d] = x.dimension(d);
        	  }
        	  if( debug & 2 )
          	    printf(" getDistributed: myid=%i, get the local array [%s] Iv=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
               		   myid,(const char*)serialArrayName,Iv[0].getBase(),Iv[0].getBound(),
               		   Iv[1].getBase(),Iv[1].getBound(),Iv[2].getBase(),Iv[2].getBound(),Iv[3].getBase(),Iv[3].getBound());
        	  assert( MAX_ARRAY_DIMENSION==6 ); // assumed in the get routine below:
        	  floatSerialArray xf;
        	  rt = sdb->get( xf, serialArrayName,Iv );
	  // NOTE: copy available ghost points too: 
        	  xl(Iv[0],Iv[1],Iv[2],Iv[3])= xf(Iv[0],Iv[1],Iv[2],Iv[3]);  // note: extra arguments default to "all"
                    if( debug & 4 ) 
        	  {
          	    aString buff;
          	    ::display(xl,sPrintF(buff,"local array [%s] on myid=%i",(const char*)serialArrayName,myid));
	    // ::display(xf,sPrintF(buff,"array from the file on myid=%i",myid));
	    // ::display(x,sPrintF(buff,"x  on myid=%i",myid));
        	  }
                    closeLocalFile(p,read);
      	}
            }
        }
        else
        {
       // --- file was written in serial : reading is simpler  ---
            aString serialArrayName = getSerialArrayName( fullGroupPath,name,myid );
            if( debug & 2 ) printf(" getDistributed: get the local array [%s] on myid=%i\n",(const char*)serialArrayName,myid);
            if( debug & 1 ) xl=-99999;  // do this for testing
            rt = get( xl, serialArrayName );
            if( rt!=0 )
      	printf("HDF_DataBase:ERROR:get(floatArray): error getting local array [%s] on myid=%i:",(const char*)serialArrayName,myid);
            if( debug & 4 )
            {
      	aString buff;
      	::display(xl,sPrintF(buff,"local array [%s] on myid=%i",(const char*)serialArrayName,myid));
            }
        }
        return rt;
    }
    xl=-99999;
  // compute and select the hyperslab we are going to read from the database
  //  note that this will include interior ghost boundaries
    long int xlBase[MAX_ARRAY_DIMENSION];
    long int xlBound[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<ndims; a++ )
    {
        xlBase[ndims-a-1] = x.getLocalBase(a);
        xlBound[ndims-a-1]= x.getLocalBound(a);
    }
    HSIZE_T xOffset[MAX_ARRAY_DIMENSION];
    hsize_t xLength[MAX_ARRAY_DIMENSION];
    hsize_t localSize = 1;
    for ( int a=0; a<ndims; a++ )
    {
        xOffset[a] = xlBase[a]-arrayBase[a];
        xLength[a] = xlBound[a]-xlBase[a]+1;
        localSize *= xLength[a];
    }
  // compute and select the destination (memory) hyperslab
    HSIZE_T xlOffset[MAX_ARRAY_DIMENSION];
    hsize_t xlLength[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<ndims; a++ )
    {
        xlOffset[ndims-a-1] = 0;
        xlLength[ndims-a-1] = xl.getBound(a)-xl.getBase(a)+1;
    }
  // describe the array in memory
    hsize_t ndo = ndims;
    if ( !localSize ) ndims=0;
    hid_t memspace = H5Screate_simple(ndims, xlLength, NULL);
    if ( !localSize ) ndims=ndo;
    for ( int a=0; a<ndims; a++ )
        if (xl.getBase(a)<xlBase[ndims-a-1] ) xlOffset[ndims-a-1] += x.getInternalGhostCellWidth(a);
    if ( localSize )
        if ( H5Sselect_hyperslab(memspace, H5S_SELECT_SET, xlOffset, NULL, xLength, NULL)<0 )
        {
            cout<< "HDF_DataBase:ERROR:get(floatArray): could not select memory hyperslab for entry with name = "<<name<<endl;
            return 1;
        }
    if ( H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, xOffset, NULL, xLength, NULL)<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(floatArray): could not select file hyperslab for entry with name = "<<name<<endl;
        return 1;
    }
    if ( !localSize )
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
    int plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(floatArray): could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
  // !!! why doesn't this work on mcr?  herr_t ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
  // !!!  why should we use COLLECTIVE when reading?
  // *wdh* herr_t ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
    herr_t ok;
#ifdef USE_PPP
    if( parallelReadMode==GenericDataBase::independentIO )
        ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
    else
        ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
    if ( ok<0 ) 
    {
        cout<<"HDF_DataBase:Error:get(floatArray): "<<myid<<" : could not set hdf5 plist!"<<endl;
        return 1;
    }
#endif
    int nr = H5Dread(datasetID, H5T_NATIVE_FLOAT, memspace, dataspace, plistID, xl.getDataPointer());
    if( nr<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(floatArray): could not read parallel array with entry name = "<<name<<endl;
        return 1;
    }
// *wdh* 061026  Communication_Manager::Sync(); // put this here in case it matters when using INDEPENDENT reads
    H5Pclose(plistID);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    timeForParallelGetArray+=getCPU()-time0;
    return 0;
// #endif
}
// getParallelArrayMacro(doubleArray,doubleSerialArray,H5T_NATIVE_DOUBLE);
int 
HDF_DataBase::
getDistributed( doubleArray &x, const aString &name ) const
{
// #ifndef USE_PPP
//   return get(x,name);
// #else  
    double time0=getCPU();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//  if ( accessMode!=read )
//  {
//    cout<< "HDF_DataBase:ERROR:get: cannot get a parallel array from a write-only file, entry name = "<<name<<endl;
//    return 1;
//  }
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( mode==streamInputMode ) 
    {
        dataBaseBuffer->getDistributedFromBuffer(x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:getDistributed(doubleArray): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
        {
            cout<< "HDF_DataBase:ERROR:getDistributed(doubleArray): cannot get entry with name = "<<name<<endl;
            Overture::abort("error");
        }
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:getDistributed(doubleArray): non-simple dataspace entry with name = "<<name<<endl;
        return 1;
    }
    if ( H5Sget_simple_extent_type(dataspace)==H5S_SCALAR ) 
    { // this was a null array
        x.redim(0);
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    } 
  // obtain the bounds and dimensions of the array from the dataset and dataspace
    hsize_t ndims =   H5Sget_simple_extent_ndims(dataspace);
    if ( ndims>MAX_ARRAY_DIMENSION )
    {
        cout<< "HDF_DataBase:ERROR:get(doubleArray): array in database has too many dimensions,  entry name = "<<name<<endl;
        return 1;
    }
    hsize_t dims[MAX_ARRAY_DIMENSION];
    H5Sget_simple_extent_dims(dataspace,dims,NULL);  // get array dimensions (these will be set to 1 if parallelReadMode==multipleFileIO)
  // cout << " dims =" << dims[0] << dims[1] << dims[2] << dims[3] << dims[4] << dims[5] << endl;
  //  printF("HDF5: getDistributed: sizeof(hsize_t)=%i sizeof(H5T_NATIVE_LONG)=%i dims=[%lli,%lli,%lli,%lli,%lli,%lli]]\n",
  // 	 sizeof(hsize_t),sizeof(H5T_NATIVE_LONG),dims[0],dims[1],dims[2],dims[3],dims[4],dims[5]);
//   cout<<"dims, procID "<<myid<<" : ";
//   for ( int a=0; a<ndims; a++ )
//     {
//       cout<<dims[a]<<"  ";
//     }
//   cout<<endl;
//   MPI_Barrier(MPI_COMM_WORLD);
  // ******* get the full array dimensions for this array *******
    if( parallelReadMode==multipleFileIO )
    {
        hid_t attribID = H5Aopen_name(datasetID, "arrayDims");
        if ( attribID<=0 )
        {
            cout<< "HDF_DataBase:ERROR:getDistributed(doubleArray): could not find arrayDims attribute for entry with name = "<<name<<endl;
            return 1;
        }
        hid_t attribspace = H5Aget_space(attribID);
    // *wdh* 100730 H5Aread(attribID, H5T_NATIVE_LONG, dims);
        H5Aread(attribID, H5T_NATIVE_HSIZE, dims);
        H5Sclose(attribspace);
        H5Aclose(attribID);
    // for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    //   printf(" HDF_DataBase: array dims[%i]=%li\n",d,dims[d]);
    }
  // --- get the parallel distribution info ---
    ArrayDistributionInfo adi;
    long int arrayBase[MAX_ARRAY_DIMENSION];
    if( true )
    {
        hid_t attribID = H5Aopen_name(datasetID, "distribInfo");
        if ( attribID<=0 )
        {
            cout<< "HDF_DataBase:ERROR:get(doubleArray): could not find distribInfo for entry with name = "<<name<<endl;
            return 1;
        }
        hid_t attribspace = H5Aget_space(attribID);
        H5Aread(attribID, H5T_NATIVE_INT, &adi);
        H5Sclose(attribspace);
        H5Aclose(attribID);
        for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
                arrayBase[d]=0;
        for( int d=0; d<ndims; d++ )
        {
              arrayBase[ndims-1-d]= (long)adi.base[d];
        }
        if( debug & 2  )
        {
            const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
            printf(" get: baseProc=%i, nProcs=%i\n",adi.baseProc,adi.nProcs);
            for( int d=0; d<numDim; d++ )
            {
      	printf(" get: d=%i base=%i dimProc=%i left=%i center=%i right=%i\n",
             	       d,adi.base[d],adi.dimProc[d],adi.dimVecL_L[d],adi.dimVecL[d],adi.dimVecL_R[d]);
            }
        }
    }
    if ( ndims==0 )
    {
    // this was a null array, our work here is done
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    }
    x.redim( Range(arrayBase[ndims-1], arrayBase[ndims-1]+dims[ndims-1]-1),
         	   Range(arrayBase[ndims-2], arrayBase[ndims-2]+dims[ndims-2]-1),
         	   Range(arrayBase[ndims-3], arrayBase[ndims-3]+dims[ndims-3]-1),
         	   Range(arrayBase[ndims-4], arrayBase[ndims-4]+dims[ndims-4]-1)
#if MAX_ARRAY_DIMENSION>4
        	  ,Range(arrayBase[ndims-5], arrayBase[ndims-5]+dims[ndims-5]-1)
#endif
#if MAX_ARRAY_DIMENSION>5
        	  ,Range(arrayBase[ndims-6], arrayBase[ndims-6]+dims[ndims-6]-1)
#endif
#if MAX_ARRAY_DIMENSION>6
        	  ,Range(arrayBase[ndims-7], arrayBase[ndims-7]+dims[ndims-7]-1)
#endif
         	   );
    doubleSerialArray xl;
  // *wdh* 061214 xl.reference(x.getLocalArrayWithGhostBoundaries());
    getLocalArrayWithGhostBoundaries(x,xl);
    if( parallelReadMode==multipleFileIO )
    {
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
    // --- new way ---
    // --- Read in the local array data from the serial data base files ---
    //     We need to determine which files to read to get all the data
    //  Given : local-array [base,bound] 
    //          file-local-array : [base,bound] 
    // NOTE: If we intersect boxes that do not include ghost points, but then copy ghost points,
    //       we can avoid reading extra files. 
        int rt=0;
        if( debug & 2 )
            printf("*** getDistributed(doubleArray) myid=%i numberOfLocalFilesForReading=%i\n",myid,numberOfLocalFilesForReading);
        bool readParallel= numberOfLocalFilesForReading>0;
        #ifdef USE_PPP
        readParallel=true;  // in parallel we always must read in parallel (since we may have ghost points)
        #endif
        if( readParallel )
        {
      // -- file was written in parallel ---
            IndexBox xBox; // box of points we need, (NO ghost)
            CopyArray::getLocalArrayBox( myid, x, xBox );
            if( debug & 2 )
      	printf("*** myid=%i %s local-array box=[%i,%i][%i,%i]\n",myid,(const char*)name,
                                              xBox.base(0),xBox.bound(0),xBox.base(1),xBox.bound(1));
            for( int p=adi.baseProc; p<adi.baseProc+adi.nProcs; p++ )
            {
      	IndexBox pBox, iBox;
      	getLocalArrayBox( p, adi, pBox );
      	if( debug & 2 )
        	  printf(" myid=%i : array in file for p=%i has box=[%i,%i][%i,%i][%i,%i][%i,%i]\n",myid,p,
             		 pBox.base(0),pBox.bound(0),pBox.base(1),pBox.bound(1),pBox.base(2),pBox.bound(2),pBox.base(3),pBox.bound(3));
      	if( IndexBox::intersect(xBox, pBox, iBox) )
      	{
        	  if( debug & 2 )
          	    printf(" myid=%i local-array data must be read from processor p=%i, iBox=[%i,%i][%i,%i]\n",
               		   myid,p,iBox.base(0),iBox.bound(0),iBox.base(1),iBox.bound(1));
                    HDF_DataBase *sdb = openLocalFile(p,read);
        	  assert( sdb != NULL );
        	  aString serialArrayName = getSerialArrayName( fullGroupPath,name,p );
	  // NOTE: What if the ghost boundary width is not the same!
	  //  -- this could occur if an array was saved with 1-parallel-ghost but read back
	  //    in with 2 parallel-ghost --> we could detect this and force and updateGhostBoundaries
	  // NOTE: ** we only read the part of the array we need, to avoid reading in a large array ***
        	  Index Iv[MAX_ARRAY_DIMENSION];
        	  int nDim=min(x.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
        	  for( int d=0; d<nDim; d++ )
        	  { // read parallel-ghost (but no need to read pts outside the array dimensions)
          	    int base = max(x.getBase(d),iBox.base(d)-x.getGhostBoundaryWidth(d));
          	    int bound= min(x.getBound(d),iBox.bound(d)+x.getGhostBoundaryWidth(d));
          	    Iv[d] = Range(base,bound);
        	  }
        	  for( int d=nDim; d< MAX_ARRAY_DIMENSION; d++ )
        	  {
          	    Iv[d] = x.dimension(d);
        	  }
        	  if( debug & 2 )
          	    printf(" getDistributed: myid=%i, get the local array [%s] Iv=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
               		   myid,(const char*)serialArrayName,Iv[0].getBase(),Iv[0].getBound(),
               		   Iv[1].getBase(),Iv[1].getBound(),Iv[2].getBase(),Iv[2].getBound(),Iv[3].getBase(),Iv[3].getBound());
        	  assert( MAX_ARRAY_DIMENSION==6 ); // assumed in the get routine below:
        	  doubleSerialArray xf;
        	  rt = sdb->get( xf, serialArrayName,Iv );
	  // NOTE: copy available ghost points too: 
        	  xl(Iv[0],Iv[1],Iv[2],Iv[3])= xf(Iv[0],Iv[1],Iv[2],Iv[3]);  // note: extra arguments default to "all"
                    if( debug & 4 ) 
        	  {
          	    aString buff;
          	    ::display(xl,sPrintF(buff,"local array [%s] on myid=%i",(const char*)serialArrayName,myid));
	    // ::display(xf,sPrintF(buff,"array from the file on myid=%i",myid));
	    // ::display(x,sPrintF(buff,"x  on myid=%i",myid));
        	  }
                    closeLocalFile(p,read);
      	}
            }
        }
        else
        {
       // --- file was written in serial : reading is simpler  ---
            aString serialArrayName = getSerialArrayName( fullGroupPath,name,myid );
            if( debug & 2 ) printf(" getDistributed: get the local array [%s] on myid=%i\n",(const char*)serialArrayName,myid);
            if( debug & 1 ) xl=-99999;  // do this for testing
            rt = get( xl, serialArrayName );
            if( rt!=0 )
      	printf("HDF_DataBase:ERROR:get(doubleArray): error getting local array [%s] on myid=%i:",(const char*)serialArrayName,myid);
            if( debug & 4 )
            {
      	aString buff;
      	::display(xl,sPrintF(buff,"local array [%s] on myid=%i",(const char*)serialArrayName,myid));
            }
        }
        return rt;
    }
    xl=-99999;
  // compute and select the hyperslab we are going to read from the database
  //  note that this will include interior ghost boundaries
    long int xlBase[MAX_ARRAY_DIMENSION];
    long int xlBound[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<ndims; a++ )
    {
        xlBase[ndims-a-1] = x.getLocalBase(a);
        xlBound[ndims-a-1]= x.getLocalBound(a);
    }
    HSIZE_T xOffset[MAX_ARRAY_DIMENSION];
    hsize_t xLength[MAX_ARRAY_DIMENSION];
    hsize_t localSize = 1;
    for ( int a=0; a<ndims; a++ )
    {
        xOffset[a] = xlBase[a]-arrayBase[a];
        xLength[a] = xlBound[a]-xlBase[a]+1;
        localSize *= xLength[a];
    }
  // compute and select the destination (memory) hyperslab
    HSIZE_T xlOffset[MAX_ARRAY_DIMENSION];
    hsize_t xlLength[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<ndims; a++ )
    {
        xlOffset[ndims-a-1] = 0;
        xlLength[ndims-a-1] = xl.getBound(a)-xl.getBase(a)+1;
    }
  // describe the array in memory
    hsize_t ndo = ndims;
    if ( !localSize ) ndims=0;
    hid_t memspace = H5Screate_simple(ndims, xlLength, NULL);
    if ( !localSize ) ndims=ndo;
    for ( int a=0; a<ndims; a++ )
        if (xl.getBase(a)<xlBase[ndims-a-1] ) xlOffset[ndims-a-1] += x.getInternalGhostCellWidth(a);
    if ( localSize )
        if ( H5Sselect_hyperslab(memspace, H5S_SELECT_SET, xlOffset, NULL, xLength, NULL)<0 )
        {
            cout<< "HDF_DataBase:ERROR:get(doubleArray): could not select memory hyperslab for entry with name = "<<name<<endl;
            return 1;
        }
    if ( H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, xOffset, NULL, xLength, NULL)<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(doubleArray): could not select file hyperslab for entry with name = "<<name<<endl;
        return 1;
    }
    if ( !localSize )
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
    int plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(doubleArray): could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
  // !!! why doesn't this work on mcr?  herr_t ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
  // !!!  why should we use COLLECTIVE when reading?
  // *wdh* herr_t ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
    herr_t ok;
#ifdef USE_PPP
    if( parallelReadMode==GenericDataBase::independentIO )
        ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
    else
        ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
    if ( ok<0 ) 
    {
        cout<<"HDF_DataBase:Error:get(doubleArray): "<<myid<<" : could not set hdf5 plist!"<<endl;
        return 1;
    }
#endif
    int nr = H5Dread(datasetID, H5T_NATIVE_DOUBLE, memspace, dataspace, plistID, xl.getDataPointer());
    if( nr<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(doubleArray): could not read parallel array with entry name = "<<name<<endl;
        return 1;
    }
// *wdh* 061026  Communication_Manager::Sync(); // put this here in case it matters when using INDEPENDENT reads
    H5Pclose(plistID);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    timeForParallelGetArray+=getCPU()-time0;
    return 0;
// #endif
}
// getParallelArrayMacro(intArray,intSerialArray,H5T_NATIVE_INT);
int 
HDF_DataBase::
getDistributed( intArray &x, const aString &name ) const
{
// #ifndef USE_PPP
//   return get(x,name);
// #else  
    double time0=getCPU();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//  if ( accessMode!=read )
//  {
//    cout<< "HDF_DataBase:ERROR:get: cannot get a parallel array from a write-only file, entry name = "<<name<<endl;
//    return 1;
//  }
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( mode==streamInputMode ) 
    {
        dataBaseBuffer->getDistributedFromBuffer(x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:getDistributed(intArray): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
        {
            cout<< "HDF_DataBase:ERROR:getDistributed(intArray): cannot get entry with name = "<<name<<endl;
            Overture::abort("error");
        }
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:getDistributed(intArray): non-simple dataspace entry with name = "<<name<<endl;
        return 1;
    }
    if ( H5Sget_simple_extent_type(dataspace)==H5S_SCALAR ) 
    { // this was a null array
        x.redim(0);
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    } 
  // obtain the bounds and dimensions of the array from the dataset and dataspace
    hsize_t ndims =   H5Sget_simple_extent_ndims(dataspace);
    if ( ndims>MAX_ARRAY_DIMENSION )
    {
        cout<< "HDF_DataBase:ERROR:get(intArray): array in database has too many dimensions,  entry name = "<<name<<endl;
        return 1;
    }
    hsize_t dims[MAX_ARRAY_DIMENSION];
    H5Sget_simple_extent_dims(dataspace,dims,NULL);  // get array dimensions (these will be set to 1 if parallelReadMode==multipleFileIO)
  // cout << " dims =" << dims[0] << dims[1] << dims[2] << dims[3] << dims[4] << dims[5] << endl;
  //  printF("HDF5: getDistributed: sizeof(hsize_t)=%i sizeof(H5T_NATIVE_LONG)=%i dims=[%lli,%lli,%lli,%lli,%lli,%lli]]\n",
  // 	 sizeof(hsize_t),sizeof(H5T_NATIVE_LONG),dims[0],dims[1],dims[2],dims[3],dims[4],dims[5]);
//   cout<<"dims, procID "<<myid<<" : ";
//   for ( int a=0; a<ndims; a++ )
//     {
//       cout<<dims[a]<<"  ";
//     }
//   cout<<endl;
//   MPI_Barrier(MPI_COMM_WORLD);
  // ******* get the full array dimensions for this array *******
    if( parallelReadMode==multipleFileIO )
    {
        hid_t attribID = H5Aopen_name(datasetID, "arrayDims");
        if ( attribID<=0 )
        {
            cout<< "HDF_DataBase:ERROR:getDistributed(intArray): could not find arrayDims attribute for entry with name = "<<name<<endl;
            return 1;
        }
        hid_t attribspace = H5Aget_space(attribID);
    // *wdh* 100730 H5Aread(attribID, H5T_NATIVE_LONG, dims);
        H5Aread(attribID, H5T_NATIVE_HSIZE, dims);
        H5Sclose(attribspace);
        H5Aclose(attribID);
    // for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
    //   printf(" HDF_DataBase: array dims[%i]=%li\n",d,dims[d]);
    }
  // --- get the parallel distribution info ---
    ArrayDistributionInfo adi;
    long int arrayBase[MAX_ARRAY_DIMENSION];
    if( true )
    {
        hid_t attribID = H5Aopen_name(datasetID, "distribInfo");
        if ( attribID<=0 )
        {
            cout<< "HDF_DataBase:ERROR:get(intArray): could not find distribInfo for entry with name = "<<name<<endl;
            return 1;
        }
        hid_t attribspace = H5Aget_space(attribID);
        H5Aread(attribID, H5T_NATIVE_INT, &adi);
        H5Sclose(attribspace);
        H5Aclose(attribID);
        for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
                arrayBase[d]=0;
        for( int d=0; d<ndims; d++ )
        {
              arrayBase[ndims-1-d]= (long)adi.base[d];
        }
        if( debug & 2  )
        {
            const int numDim=MAX_DISTRIBUTED_DIMENSIONS; // min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
            printf(" get: baseProc=%i, nProcs=%i\n",adi.baseProc,adi.nProcs);
            for( int d=0; d<numDim; d++ )
            {
      	printf(" get: d=%i base=%i dimProc=%i left=%i center=%i right=%i\n",
             	       d,adi.base[d],adi.dimProc[d],adi.dimVecL_L[d],adi.dimVecL[d],adi.dimVecL_R[d]);
            }
        }
    }
    if ( ndims==0 )
    {
    // this was a null array, our work here is done
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
        return 0;
    }
    x.redim( Range(arrayBase[ndims-1], arrayBase[ndims-1]+dims[ndims-1]-1),
         	   Range(arrayBase[ndims-2], arrayBase[ndims-2]+dims[ndims-2]-1),
         	   Range(arrayBase[ndims-3], arrayBase[ndims-3]+dims[ndims-3]-1),
         	   Range(arrayBase[ndims-4], arrayBase[ndims-4]+dims[ndims-4]-1)
#if MAX_ARRAY_DIMENSION>4
        	  ,Range(arrayBase[ndims-5], arrayBase[ndims-5]+dims[ndims-5]-1)
#endif
#if MAX_ARRAY_DIMENSION>5
        	  ,Range(arrayBase[ndims-6], arrayBase[ndims-6]+dims[ndims-6]-1)
#endif
#if MAX_ARRAY_DIMENSION>6
        	  ,Range(arrayBase[ndims-7], arrayBase[ndims-7]+dims[ndims-7]-1)
#endif
         	   );
    intSerialArray xl;
  // *wdh* 061214 xl.reference(x.getLocalArrayWithGhostBoundaries());
    getLocalArrayWithGhostBoundaries(x,xl);
    if( parallelReadMode==multipleFileIO )
    {
        H5Sclose(dataspace);
        H5Dclose(datasetID);
        H5Gclose(groupID);
    // --- new way ---
    // --- Read in the local array data from the serial data base files ---
    //     We need to determine which files to read to get all the data
    //  Given : local-array [base,bound] 
    //          file-local-array : [base,bound] 
    // NOTE: If we intersect boxes that do not include ghost points, but then copy ghost points,
    //       we can avoid reading extra files. 
        int rt=0;
        if( debug & 2 )
            printf("*** getDistributed(intArray) myid=%i numberOfLocalFilesForReading=%i\n",myid,numberOfLocalFilesForReading);
        bool readParallel= numberOfLocalFilesForReading>0;
        #ifdef USE_PPP
        readParallel=true;  // in parallel we always must read in parallel (since we may have ghost points)
        #endif
        if( readParallel )
        {
      // -- file was written in parallel ---
            IndexBox xBox; // box of points we need, (NO ghost)
            CopyArray::getLocalArrayBox( myid, x, xBox );
            if( debug & 2 )
      	printf("*** myid=%i %s local-array box=[%i,%i][%i,%i]\n",myid,(const char*)name,
                                              xBox.base(0),xBox.bound(0),xBox.base(1),xBox.bound(1));
            for( int p=adi.baseProc; p<adi.baseProc+adi.nProcs; p++ )
            {
      	IndexBox pBox, iBox;
      	getLocalArrayBox( p, adi, pBox );
      	if( debug & 2 )
        	  printf(" myid=%i : array in file for p=%i has box=[%i,%i][%i,%i][%i,%i][%i,%i]\n",myid,p,
             		 pBox.base(0),pBox.bound(0),pBox.base(1),pBox.bound(1),pBox.base(2),pBox.bound(2),pBox.base(3),pBox.bound(3));
      	if( IndexBox::intersect(xBox, pBox, iBox) )
      	{
        	  if( debug & 2 )
          	    printf(" myid=%i local-array data must be read from processor p=%i, iBox=[%i,%i][%i,%i]\n",
               		   myid,p,iBox.base(0),iBox.bound(0),iBox.base(1),iBox.bound(1));
                    HDF_DataBase *sdb = openLocalFile(p,read);
        	  assert( sdb != NULL );
        	  aString serialArrayName = getSerialArrayName( fullGroupPath,name,p );
	  // NOTE: What if the ghost boundary width is not the same!
	  //  -- this could occur if an array was saved with 1-parallel-ghost but read back
	  //    in with 2 parallel-ghost --> we could detect this and force and updateGhostBoundaries
	  // NOTE: ** we only read the part of the array we need, to avoid reading in a large array ***
        	  Index Iv[MAX_ARRAY_DIMENSION];
        	  int nDim=min(x.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
        	  for( int d=0; d<nDim; d++ )
        	  { // read parallel-ghost (but no need to read pts outside the array dimensions)
          	    int base = max(x.getBase(d),iBox.base(d)-x.getGhostBoundaryWidth(d));
          	    int bound= min(x.getBound(d),iBox.bound(d)+x.getGhostBoundaryWidth(d));
          	    Iv[d] = Range(base,bound);
        	  }
        	  for( int d=nDim; d< MAX_ARRAY_DIMENSION; d++ )
        	  {
          	    Iv[d] = x.dimension(d);
        	  }
        	  if( debug & 2 )
          	    printf(" getDistributed: myid=%i, get the local array [%s] Iv=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
               		   myid,(const char*)serialArrayName,Iv[0].getBase(),Iv[0].getBound(),
               		   Iv[1].getBase(),Iv[1].getBound(),Iv[2].getBase(),Iv[2].getBound(),Iv[3].getBase(),Iv[3].getBound());
        	  assert( MAX_ARRAY_DIMENSION==6 ); // assumed in the get routine below:
        	  intSerialArray xf;
        	  rt = sdb->get( xf, serialArrayName,Iv );
	  // NOTE: copy available ghost points too: 
        	  xl(Iv[0],Iv[1],Iv[2],Iv[3])= xf(Iv[0],Iv[1],Iv[2],Iv[3]);  // note: extra arguments default to "all"
                    if( debug & 4 ) 
        	  {
          	    aString buff;
          	    ::display(xl,sPrintF(buff,"local array [%s] on myid=%i",(const char*)serialArrayName,myid));
	    // ::display(xf,sPrintF(buff,"array from the file on myid=%i",myid));
	    // ::display(x,sPrintF(buff,"x  on myid=%i",myid));
        	  }
                    closeLocalFile(p,read);
      	}
            }
        }
        else
        {
       // --- file was written in serial : reading is simpler  ---
            aString serialArrayName = getSerialArrayName( fullGroupPath,name,myid );
            if( debug & 2 ) printf(" getDistributed: get the local array [%s] on myid=%i\n",(const char*)serialArrayName,myid);
            if( debug & 1 ) xl=-99999;  // do this for testing
            rt = get( xl, serialArrayName );
            if( rt!=0 )
      	printf("HDF_DataBase:ERROR:get(intArray): error getting local array [%s] on myid=%i:",(const char*)serialArrayName,myid);
            if( debug & 4 )
            {
      	aString buff;
      	::display(xl,sPrintF(buff,"local array [%s] on myid=%i",(const char*)serialArrayName,myid));
            }
        }
        return rt;
    }
    xl=-99999;
  // compute and select the hyperslab we are going to read from the database
  //  note that this will include interior ghost boundaries
    long int xlBase[MAX_ARRAY_DIMENSION];
    long int xlBound[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<ndims; a++ )
    {
        xlBase[ndims-a-1] = x.getLocalBase(a);
        xlBound[ndims-a-1]= x.getLocalBound(a);
    }
    HSIZE_T xOffset[MAX_ARRAY_DIMENSION];
    hsize_t xLength[MAX_ARRAY_DIMENSION];
    hsize_t localSize = 1;
    for ( int a=0; a<ndims; a++ )
    {
        xOffset[a] = xlBase[a]-arrayBase[a];
        xLength[a] = xlBound[a]-xlBase[a]+1;
        localSize *= xLength[a];
    }
  // compute and select the destination (memory) hyperslab
    HSIZE_T xlOffset[MAX_ARRAY_DIMENSION];
    hsize_t xlLength[MAX_ARRAY_DIMENSION];
    for ( int a=0; a<ndims; a++ )
    {
        xlOffset[ndims-a-1] = 0;
        xlLength[ndims-a-1] = xl.getBound(a)-xl.getBase(a)+1;
    }
  // describe the array in memory
    hsize_t ndo = ndims;
    if ( !localSize ) ndims=0;
    hid_t memspace = H5Screate_simple(ndims, xlLength, NULL);
    if ( !localSize ) ndims=ndo;
    for ( int a=0; a<ndims; a++ )
        if (xl.getBase(a)<xlBase[ndims-a-1] ) xlOffset[ndims-a-1] += x.getInternalGhostCellWidth(a);
    if ( localSize )
        if ( H5Sselect_hyperslab(memspace, H5S_SELECT_SET, xlOffset, NULL, xLength, NULL)<0 )
        {
            cout<< "HDF_DataBase:ERROR:get(intArray): could not select memory hyperslab for entry with name = "<<name<<endl;
            return 1;
        }
    if ( H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, xOffset, NULL, xLength, NULL)<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(intArray): could not select file hyperslab for entry with name = "<<name<<endl;
        return 1;
    }
    if ( !localSize )
    {
        H5Sselect_none(memspace);
        H5Sselect_none(dataspace);
    }
    int plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(intArray): could not create xfer property list for entry name = "<<name<<endl;
        return 1;
    }
  // !!! why doesn't this work on mcr?  herr_t ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
  // !!!  why should we use COLLECTIVE when reading?
  // *wdh* herr_t ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
    herr_t ok;
#ifdef USE_PPP
    if( parallelReadMode==GenericDataBase::independentIO )
        ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
    else
        ok = H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE); 
    if ( ok<0 ) 
    {
        cout<<"HDF_DataBase:Error:get(intArray): "<<myid<<" : could not set hdf5 plist!"<<endl;
        return 1;
    }
#endif
    int nr = H5Dread(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, xl.getDataPointer());
    if( nr<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(intArray): could not read parallel array with entry name = "<<name<<endl;
        return 1;
    }
// *wdh* 061026  Communication_Manager::Sync(); // put this here in case it matters when using INDEPENDENT reads
    H5Pclose(plistID);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(datasetID);
    H5Gclose(groupID);
    timeForParallelGetArray+=getCPU()-time0;
    return 0;
// #endif
}


// Define a macro to save either float/int/double
//  type=float/int/double HDFType=corresponding HDF type
//=====================================================================================  
// /Description: Save a type in the data-base. The type is saved as an HDF vdata.  
// /x (input): value to save.  
// /name (input): save the value with this name.  
// /Notes:
//    Save a float/int/double in a vdata. 
//=====================================================================================  

// putScalar(float, H5T_NATIVE_FLOAT);
int 
HDF_DataBase::
put( const float &x, const aString &name )
{
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put: cannot put a scalar to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    if ( mode==streamOutputMode ) 
    {
        dataBaseBuffer->putToBuffer(1,&x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
  // create the dataspace telling hdf5 what the file image of the array will be
    hid_t dataspace = H5Screate(H5S_SCALAR);
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_FLOAT, dataspace, H5P_DEFAULT);
  // attach the class name
    setClassName( datasetID, "float");//!!! NOTE : float will be replaced by the macro argument!
  // now create the dataspace describing the memory image of the array
    hid_t memspace = H5Screate(H5S_SCALAR);
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(float): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid==processorForWriting ) 
        {
#endif
    if ( H5Dwrite(datasetID, H5T_NATIVE_FLOAT, memspace, dataspace, plistID, &x)<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(float): could not write scalar with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
        }
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    return 0;
}
// putScalar(double, H5T_NATIVE_DOUBLE);
int 
HDF_DataBase::
put( const double &x, const aString &name )
{
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put: cannot put a scalar to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    if ( mode==streamOutputMode ) 
    {
        dataBaseBuffer->putToBuffer(1,&x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
  // create the dataspace telling hdf5 what the file image of the array will be
    hid_t dataspace = H5Screate(H5S_SCALAR);
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT);
  // attach the class name
    setClassName( datasetID, "double");//!!! NOTE : double will be replaced by the macro argument!
  // now create the dataspace describing the memory image of the array
    hid_t memspace = H5Screate(H5S_SCALAR);
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(double): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid==processorForWriting ) 
        {
#endif
    if ( H5Dwrite(datasetID, H5T_NATIVE_DOUBLE, memspace, dataspace, plistID, &x)<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(double): could not write scalar with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
        }
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    return 0;
}
// putScalar(int, H5T_NATIVE_INT);
int 
HDF_DataBase::
put( const int &x, const aString &name )
{
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put: cannot put a scalar to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    if ( mode==streamOutputMode ) 
    {
        dataBaseBuffer->putToBuffer(1,&x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
  // create the dataspace telling hdf5 what the file image of the array will be
    hid_t dataspace = H5Screate(H5S_SCALAR);
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT);
  // attach the class name
    setClassName( datasetID, "int");//!!! NOTE : int will be replaced by the macro argument!
  // now create the dataspace describing the memory image of the array
    hid_t memspace = H5Screate(H5S_SCALAR);
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(int): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid==processorForWriting ) 
        {
#endif
    if ( H5Dwrite(datasetID, H5T_NATIVE_INT, memspace, dataspace, plistID, &x)<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(int): could not write scalar with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
        }
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    return 0;
}
#ifdef OV_BOOL_DEFINED
// putScalar(bool, H5T_NATIVE_HBOOL);
int 
HDF_DataBase::
put( const bool &x, const aString &name )
{
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
    if ( accessMode!=write )
    {
        cout<< "HDF_DataBase:ERROR:put: cannot put a scalar to a read-only file, entry name = "<<name<<endl;
        return 1;
    }
    if ( mode==streamOutputMode ) 
    {
        dataBaseBuffer->putToBuffer(1,&x);
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
  // open the group corresponding to this directory
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
  // create the dataspace telling hdf5 what the file image of the array will be
    hid_t dataspace = H5Screate(H5S_SCALAR);
    hid_t datasetID = H5Dcreate(groupID, name, H5T_NATIVE_HBOOL, dataspace, H5P_DEFAULT);
  // attach the class name
    setClassName( datasetID, "bool");//!!! NOTE : bool will be replaced by the macro argument!
  // now create the dataspace describing the memory image of the array
    hid_t memspace = H5Screate(H5S_SCALAR);
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(bool): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid==processorForWriting ) 
        {
#endif
    if ( H5Dwrite(datasetID, H5T_NATIVE_HBOOL, memspace, dataspace, plistID, &x)<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(bool): could not write scalar with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
        }
    H5Pclose(plistID);
#endif
    H5Sclose(memspace);
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    return 0;
}
#endif


// getScalar(float, H5T_NATIVE_FLOAT);
int 
HDF_DataBase::
get( float &x, const aString &name ) const
{
    double time0=MPI_Wtime();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//   if ( accessMode!=read )
//   {
//     cout<< "HDF_DataBase:ERROR:get: cannot get a scalar from a write-only file, entry name = "<<name<<endl;
//     return 1;
//   }
    const int myid = max(0,Communication_Manager::My_Process_Number);
  // all processes will read this data
    if ( mode==streamInputMode ) 
        {
            dataBaseBuffer->getFromBuffer(1,&x);
            return 0;
        }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(float): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
            cout<< "HDF_DataBase:ERROR:get(float): cannot get entry with name = "<<name<<endl;
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get(float): non-simple dataspace entry with name = "<<name<<endl;
        H5Dclose(datasetID);
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(float): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
    if ( H5Dread(datasetID,H5T_NATIVE_FLOAT,H5S_ALL,H5S_ALL,H5P_DEFAULT,&x)<0 ) // there better not be more than one piece of data in there!
        {
            cout<< "HDF_DataBase:ERROR:get: could not read scalar data with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    timeForScalarGet+=MPI_Wtime()-time0;
    return 0;
}
// getScalar(double, H5T_NATIVE_DOUBLE);
int 
HDF_DataBase::
get( double &x, const aString &name ) const
{
    double time0=MPI_Wtime();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//   if ( accessMode!=read )
//   {
//     cout<< "HDF_DataBase:ERROR:get: cannot get a scalar from a write-only file, entry name = "<<name<<endl;
//     return 1;
//   }
    const int myid = max(0,Communication_Manager::My_Process_Number);
  // all processes will read this data
    if ( mode==streamInputMode ) 
        {
            dataBaseBuffer->getFromBuffer(1,&x);
            return 0;
        }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(double): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
            cout<< "HDF_DataBase:ERROR:get(double): cannot get entry with name = "<<name<<endl;
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get(double): non-simple dataspace entry with name = "<<name<<endl;
        H5Dclose(datasetID);
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(double): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
    if ( H5Dread(datasetID,H5T_NATIVE_DOUBLE,H5S_ALL,H5S_ALL,H5P_DEFAULT,&x)<0 ) // there better not be more than one piece of data in there!
        {
            cout<< "HDF_DataBase:ERROR:get: could not read scalar data with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    timeForScalarGet+=MPI_Wtime()-time0;
    return 0;
}
// getScalar(int, H5T_NATIVE_INT);
int 
HDF_DataBase::
get( int &x, const aString &name ) const
{
    double time0=MPI_Wtime();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//   if ( accessMode!=read )
//   {
//     cout<< "HDF_DataBase:ERROR:get: cannot get a scalar from a write-only file, entry name = "<<name<<endl;
//     return 1;
//   }
    const int myid = max(0,Communication_Manager::My_Process_Number);
  // all processes will read this data
    if ( mode==streamInputMode ) 
        {
            dataBaseBuffer->getFromBuffer(1,&x);
            return 0;
        }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(int): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
            cout<< "HDF_DataBase:ERROR:get(int): cannot get entry with name = "<<name<<endl;
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get(int): non-simple dataspace entry with name = "<<name<<endl;
        H5Dclose(datasetID);
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(int): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
    if ( H5Dread(datasetID,H5T_NATIVE_INT,H5S_ALL,H5S_ALL,H5P_DEFAULT,&x)<0 ) // there better not be more than one piece of data in there!
        {
            cout<< "HDF_DataBase:ERROR:get: could not read scalar data with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    timeForScalarGet+=MPI_Wtime()-time0;
    return 0;
}
#ifdef OV_BOOL_DEFINED
// getScalar(bool, H5T_NATIVE_HBOOL);
int 
HDF_DataBase::
get( bool &x, const aString &name ) const
{
    double time0=MPI_Wtime();
    assert( fileID>0 && fullGroupPath.c_str()!=NULL );
  // We can always read from a file *wdh* 060710
//   if ( accessMode!=read )
//   {
//     cout<< "HDF_DataBase:ERROR:get: cannot get a scalar from a write-only file, entry name = "<<name<<endl;
//     return 1;
//   }
    const int myid = max(0,Communication_Manager::My_Process_Number);
  // all processes will read this data
    if ( mode==streamInputMode ) 
        {
            dataBaseBuffer->getFromBuffer(1,&x);
            return 0;
        }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream(); // flush any stream buffers  
    hid_t groupID = H5Gopen(fileID, fullGroupPath);
    if ( groupID<0 )
    {
        cout<< "HDF_DataBase:ERROR:get(bool): cannot get from directory = "<<fullGroupPath<<endl;
        return 1;
    }
  // get the description for the array in the file
    hid_t datasetID = H5Dopen(groupID, name);
    if ( datasetID<0 )
    {
        if( issueWarnings )
            cout<< "HDF_DataBase:ERROR:get(bool): cannot get entry with name = "<<name<<endl;
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t dataspace = H5Dget_space(datasetID);
    if ( H5Sis_simple(dataspace)<=0 )
    {
        cout<< "HDF_DataBase:ERROR:get(bool): non-simple dataspace entry with name = "<<name<<endl;
        H5Dclose(datasetID);
        H5Gclose(groupID);  // *wdh* 060710
        return 1;
    }
    hid_t plistID = H5P_DEFAULT;
#ifdef USE_PPP
  // this should be the default but set it just in case the default changes
    plistID = H5Pcreate(H5P_DATASET_XFER);
    if ( plistID<0 )
        {
            cout<< "HDF_DataBase:ERROR:put(bool): could not create xfer property list for entry name = "<<name<<endl;
            return 1;
        }
    H5Pset_dxpl_mpio(plistID, H5FD_MPIO_INDEPENDENT); 
#endif
    if ( H5Dread(datasetID,H5T_NATIVE_HBOOL,H5S_ALL,H5S_ALL,H5P_DEFAULT,&x)<0 ) // there better not be more than one piece of data in there!
        {
            cout<< "HDF_DataBase:ERROR:get: could not read scalar data with entry name = "<<name<<endl;
            return 1;
        }
#ifdef USE_PPP
    H5Pclose(plistID);
#endif
    H5Dclose(datasetID);
    H5Sclose(dataspace);
    H5Gclose(groupID);
    timeForScalarGet+=MPI_Wtime()-time0;
    return 0;
}
#endif


// put a single aString:
int HDF_DataBase::   
put( const aString & x, const aString & name ) 
{  
    assert( fileID > 0 && (const char*)fullGroupPath!=NULL );   
    if( accessMode!=write ) 
    { 
        cout << "HDF_DataBase:ERROR:put: cannot put a float/double/int to a read-only file, name ="  
                  << (const char*) name << endl; 
        return 1; 
    } 

    if( mode==streamOutputMode )
    {
        int num=x.length()+1;
        dataBaseBuffer->putToBuffer( 1,&num );   // save number of chars
        dataBaseBuffer->putToBuffer( num,(const char *)x ); 
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    // flush any stream buffers 
    hsize_t dims[1]={x.length()+1}, rank=1;
    hid_t dataspaceID = H5Screate_simple(rank,dims,NULL);
    hid_t groupID = H5Gopen(fileID,fullGroupPath);
    hid_t sdsID = H5Dcreate(groupID,(char*)((const char *)name),H5T_NATIVE_UCHAR,dataspaceID,H5P_DEFAULT);
    if( sdsID<0 ) 
    {
        cout<<"ERROR putting aString to hdf5 file, string was : "<<x<<endl;
        H5Gclose(groupID);
        H5Sclose(dataspaceID);
        return 1;
    }

    hid_t memspace=H5Screate_simple(rank,dims,NULL);
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid!=processorForWriting ) 
        {
            H5Sselect_none(memspace);
            H5Sselect_none(dataspaceID);
        }
#endif

    int istat = H5Dwrite(sdsID,H5T_NATIVE_UCHAR,memspace,dataspaceID,H5P_DEFAULT,(unsigned char*)((const char*)x));
    if( istat<0 ) 
    {
        cout<<"ERROR putting aString to hdf5 file, string was : "<<x<<endl;
        H5Gclose(groupID);
        H5Sclose(dataspaceID);
        return 1;
    }

    setClassName(sdsID,"string"); H5Gclose(groupID);
    H5Sclose(dataspaceID);
    H5Dclose(sdsID);

    return 0;   
}

// put a "c" array of floats, int's or doubles

// putCArrayMacro(int, H5T_NATIVE_INT);
int
HDF_DataBase::
put( const int x[], const aString &name, const int number)
{
    intSerialArray xa(number);
    for ( int i=0; i<number; i++ )
        xa(i) = x[i];
    put(xa,name);
    return 0;
}
// putCArrayMacro(float, H5T_NATIVE_FLOAT);
int
HDF_DataBase::
put( const float x[], const aString &name, const int number)
{
    floatSerialArray xa(number);
    for ( int i=0; i<number; i++ )
        xa(i) = x[i];
    put(xa,name);
    return 0;
}
// putCArrayMacro(double, H5T_NATIVE_DOUBLE);
int
HDF_DataBase::
put( const double x[], const aString &name, const int number)
{
    doubleSerialArray xa(number);
    for ( int i=0; i<number; i++ )
        xa(i) = x[i];
    put(xa,name);
    return 0;
}

//=================================================================================
// /Description:
//   Put an array of Strings.
// /x[] (input): array of Strings to save.
// /name (input): save the array under this name.
// /numberOfStrings (input): Save this many elements from the array.
// 
// /Notes:
//    The array of strings are concatenated together and saved in a vdata.
//=================================================================================
int HDF_DataBase::   
put( const aString x[], const aString & name, const int numberOfStrings ) 
{  
    assert( fileID > 0 && (const char*)fullGroupPath!=NULL);   
    if( accessMode!=write ) 
    { 
        cout << "HDF_DataBase:ERROR:put: cannot put a float/double/int to a read-only file, name ="  
                  << (const char*) name << endl; 
        return 1; 
    } 

    if( mode==streamOutputMode )
    {
        dataBaseBuffer->putToBuffer( 1,&numberOfStrings );   // save number of chars
        int i;
        for( i=0; i<numberOfStrings; i++ )
            put( x[i],"dummy");
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    // flush any stream buffers 
//
//		Determine total string length and the length of each string
//
    int istring;
    int totalStringLength=0;
    for(istring=0; istring<numberOfStrings; istring++) {
        totalStringLength+=x[istring].length()+1;
    }
//
//		Dump all strings to output
//
    hsize_t dims[1]={totalStringLength}, rank=1;
    hid_t dataspaceID = H5Screate_simple(rank,dims,NULL);
    hid_t groupID = H5Gopen(fileID,fullGroupPath);
    hid_t sdsID = H5Dcreate(groupID,(char*)((const char *)name),H5T_NATIVE_UCHAR,dataspaceID,H5P_DEFAULT);
    if( sdsID<0 ) 
    {
        printf("HDF_DataBase::put(aString[]): ERROR return from H5Dcreate, name=%s\n",(const char*)name);
        H5Gclose(groupID);
        H5Sclose(dataspaceID);
        return 1;
    }
//
//		Connecticate strings, and seperators
//
    char *temp = new char[totalStringLength];;
    totalStringLength=0;
    for(istring=0; istring < numberOfStrings; istring++){
        for(int ichar=0; ichar < x[istring].length(); ichar++) {
            temp[totalStringLength]=x[istring][ichar];
            totalStringLength++;
        }
        temp[totalStringLength]='\0';
        totalStringLength++;
    }

    hid_t memspace=H5Screate_simple(rank,dims,NULL);
#ifdef USE_PPP
    const int myid = max(0,Communication_Manager::My_Process_Number);
    if ( myid!=processorForWriting ) 
        {
            H5Sselect_none(memspace);
            H5Sselect_none(dataspaceID);
        }
#endif

    int istat = H5Dwrite(sdsID,H5T_NATIVE_UCHAR,memspace,dataspaceID,H5P_DEFAULT,(unsigned char*)((const char*)temp));

    delete [] temp;
    if( istat<0 ) 
    {
        printf("HDF_DataBase::put(aString[]): ERROR return from H5Dwrite, name=%s\n",(const char*)name);
        H5Gclose(groupID);
        H5Sclose(dataspaceID);

        return 1;
    }
    setClassName(sdsID,"stringArray");
    H5Gclose(groupID);
    H5Sclose(dataspaceID);

    H5Dclose(sdsID);
    return 0;
}


int HDF_DataBase::   
get( aString & x, const aString & name ) const 
{   
    assert( fileID > 0 && (const char*)fullGroupPath!=NULL );   
    if( mode==streamInputMode )
    {
        int num;
        dataBaseBuffer->getFromBuffer( 1,&num );   // get number of chars
        char *temp = new char [num];
        dataBaseBuffer->getFromBuffer( num,temp ); 
        x=temp;
        delete [] temp;
        return 0;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    /* flush any stream buffers */ 
    hsize_t rank=1, dims[1];
//
//		Open the named data set
//
    hid_t groupID = H5Gopen(fileID,fullGroupPath);
    hid_t sdsID = H5Dopen(groupID,(char*)((const char*)name)); 
    H5Gclose(groupID); 
    if(sdsID <= 0) 
    {
        if( issueWarnings )
            cout<< "HDF_DataBase:ERROR:get(aString): cannot get entry with name = "<<name<<endl;
        return 1;
    }
    hid_t dataspaceID = H5Dget_space(sdsID);
    H5Sget_simple_extent_dims(dataspaceID,dims,NULL);
    char *temp = new char[dims[0]];
//
//		Read the data set into the new array
//
    H5Dread(sdsID,H5T_NATIVE_UCHAR,H5S_ALL,H5S_ALL,H5P_DEFAULT,(unsigned char*)((const char *)temp));  
    x=temp;
    delete [] temp;
//
//		Finish clean up
//
    H5Sclose(dataspaceID);
    H5Dclose(sdsID);

    return 0;
}


// put a "c" array of floats, int's or doubles

// getCArrayMacro(int, H5T_NATIVE_INT);
int
HDF_DataBase::
get( int x[], const aString &name, const int number) const
{
    intSerialArray xa(number);
    int ierr = get(xa,name);
    for ( int i=0; i<number; i++ )
        x[i] = xa(i);
    return ierr;
}
// getCArrayMacro(float, H5T_NATIVE_FLOAT);
int
HDF_DataBase::
get( float x[], const aString &name, const int number) const
{
    floatSerialArray xa(number);
    int ierr = get(xa,name);
    for ( int i=0; i<number; i++ )
        x[i] = xa(i);
    return ierr;
}
// getCArrayMacro(double, H5T_NATIVE_DOUBLE);
int
HDF_DataBase::
get( double x[], const aString &name, const int number) const
{
    doubleSerialArray xa(number);
    int ierr = get(xa,name);
    for ( int i=0; i<number; i++ )
        x[i] = xa(i);
    return ierr;
}


// get an array of Strings
// get at most numberOfStrings elements in the array.
// return the number of strings actually saved in the array
int HDF_DataBase::   
get( aString x[], const aString & name, const int numberOfStrings ) const   
{   
    assert( fileID > 0 && (const char*)fullGroupPath!=NULL );   
    int ichar;
    if( mode==streamInputMode )
    {
        int numberSaved=0;
        dataBaseBuffer->getFromBuffer( 1,&numberSaved );   // get number of strings in array
        if( numberSaved>numberOfStrings )
            numberSaved=numberOfStrings;
        for( int i=0; i<numberSaved; i++ )
            get( x[i],"dummy");
        return numberSaved;
    }
    if( dataBaseBuffer!=NULL && (mode==normalMode || mode==noStreamMode) )
        closeStream();    /* flush any stream buffers */ 
    hsize_t rank=1, dims[1];
//
//		Open the named data set
//
    hid_t groupID = H5Gopen(fileID,fullGroupPath);
    hid_t sdsID = H5Dopen(groupID,(char*)((const char*)name));  
    H5Gclose(groupID);
    if(sdsID <= 0) 
    {
        if( issueWarnings )
            cout<< "HDF_DataBase:ERROR:get(aString[]): cannot get entry with name = "<<name<<endl;
        return 0;  // zero header comments returned
    }
    hid_t dataspaceID = H5Dget_space(sdsID);
    H5Sget_simple_extent_dims(dataspaceID,dims,NULL);
    H5Sclose(dataspaceID);
    char *temp = new char[dims[0]];
//
//		Read the data set into the new array
//
    H5Dread(sdsID,H5T_NATIVE_UCHAR,H5S_ALL,H5S_ALL,H5P_DEFAULT,(unsigned char*)((const char *)temp));  
    H5Dclose(sdsID);
//
//		Count the total number of strings
// 
    int totalNumberOfStrings=0;
    for(ichar=0; ichar<dims[0]; ichar++) 
    {
        if(temp[ichar]=='\0') 
        {
            totalNumberOfStrings++;
        }
    }
    int numberSaved;
    if(totalNumberOfStrings < numberOfStrings) numberSaved=totalNumberOfStrings;
    else numberSaved=numberOfStrings;
//
//		Read in the specified number of strings
//
    int string_start=0;
    int string_end;
    int string_len;
    int istring=0;
    for(ichar=0; ichar<dims[0]; ichar++) 
    {
//
//		Read in strings seperated by \0 character
//
        if(temp[ichar]=='\0') 
        {
            string_end=ichar-1;
            string_len=(string_end-string_start)+1;
            if(istring<numberSaved) 
            {
                x[istring] = &temp[string_start];
            }
            istring++;
            string_start=ichar+1;
        }    
    }
//
//		Free all temporary vars
//
    delete [] temp;
    return numberSaved;   
}

void  HDF_DataBase::
printStatistics() const 
// output statistics on the file such as number of vgroups, vdatas
{

//  int numberOfVgroups = Hnumber(fileID,DFTAG_VG);
//  int numberOfVDatas  = Hnumber(fileID,DFTAG_VS);
//  int numberOfSDs     = Hnumber(fileID,DFTAG_SD);
//  int numberOfSDGs    = Hnumber(fileID,DFTAG_SDG);
//  int numberOfObjects  = Hnumber(fileID,DFTAG_WILDCARD);
    
//  printf("HDF_DataBase:: There are %i vgroups, %i vdatas %i sds %i sdgs and %i objects in the file \n",
//       numberOfVgroups,numberOfVDatas,numberOfSDs,numberOfSDGs,numberOfObjects);
}

