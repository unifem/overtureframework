// include "OvertureInit.h"
#include "OvertureDefine.h"
#ifdef OV_BUILD_MAPPING_LIBRARY
#include "Mapping.h"
int initializeMappingList();
int destructMappingList();
#else
#include "Overture.h"
#endif

#include "GenericGraphicsInterface.h"
#include "GraphicsParameters.h"
#include "ReparameterizationTransform.h"

#ifndef OV_BUILD_MAPPING_LIBRARY
#include "ParallelUtility.h"
#include "BoundaryConditionParameters.h"
#endif

// kkc prototype for schewchuck's predicates initialization
extern "C" void exactinit();

static bool getGraphicsInterfaceInstantiated=false;
static GenericGraphicsInterface * internalGI=NULL;

FILE* Overture::debugFile=NULL;

Overture::AbortEnum Overture::abortOption=Overture::abortOnAbort;

static int counter=0;
// Mapping::LinkedList *pStaticMappingList = 0;  // list of Mappings for makeMapping

floatSerialArray *Overture::pNullFloatArray = 0;  
doubleSerialArray *Overture::pNullDoubleArray = 0;  
RealArray *Overture::pNullRealArray = 0;  
intSerialArray  *Overture::pNullIntArray = 0;

floatDistributedArray *Overture::pNullFloatDistributedArray = 0;  
doubleDistributedArray *Overture::pNullDoubleDistributedArray = 0;  
RealDistributedArray *Overture::pNullRealDistributedArray = 0;  
IntegerDistributedArray  *Overture::pNullIntegerDistributedArray = 0;


MappingParameters *Overture::pNullMappingParameters = 0;
MappingLinkedList *Overture::pStaticMappingList = 0; 


int Mapping::minimumNumberOfDistributedGhostLines=1;

// Maybe this should be in the Rapsodi library, since only pointers are set to zero
#ifndef OV_BUILD_MAPPING_LIBRARY
int MappedGrid::minimumNumberOfDistributedGhostLines=0;

floatMappedGridFunction  *Overture::pNullFloatMappedGridFunction = 0;
doubleMappedGridFunction *Overture::pNullDoubleMappedGridFunction = 0;
intMappedGridFunction    *Overture::pNullIntMappedGridFunction = 0;
realMappedGridFunction   *Overture::pNullRealMappedGridFunction = 0;

floatGridCollectionFunction  *Overture::pNullFloatGridCollectionFunction = 0;
doubleGridCollectionFunction *Overture::pNullDoubleGridCollectionFunction = 0;
intGridCollectionFunction    *Overture::pNullIntGridCollectionFunction = 0;
realGridCollectionFunction   *Overture::pNullRealGridCollectionFunction = 0;

// this can NOT be in Rapsodi, since this class is not known to Rapsodi (forward declaration enough?)
BoundaryConditionParameters *Overture::pDefaultBoundaryConditionParameters = 0;
#endif

GraphicsParameters *Overture::pDefaultGraphicsParameters = 0;
GraphicsParameters *Overture::pCurrentGraphicsParameters = 0;

GenericGraphicsInterface* Overture::pPlotStuff=NULL;  // this will be assigned by the GenericGraphicsInterface constructor.
ListOfMappingRC* Overture::pMappingList=NULL;

int Overture::referenceCountForPETSc=0;
bool Overture::PETScIsInitialized=false;

typedef void (*shutDownPETScFunctionPointer)();

shutDownPETScFunctionPointer Overture::shutDownPETSc=NULL;


#ifdef USE_PPP
  MPI_Comm Overture::OV_COMM = 0;
#else
  int Overture::OV_COMM=0;
#endif

GenericDataBase::ParallelIOModeEnum GenericDataBase::parallelReadMode=GenericDataBase::multipleFileIO; 
GenericDataBase::ParallelIOModeEnum GenericDataBase::parallelWriteMode=GenericDataBase::multipleFileIO;

//\begin{>OvertureInclude.tex}{\subsection{Overture global variables.}} 
//\no function header:
// 
//  The Overture class contains global variables that can be used as default arguments
//  to functions. For example, {\tt Overture::nullRealArray() }, can be used as a default
//  argument for a {\tt RealArray}. These instances of classes are accessed as function calls
//  as opposed to building static global variables. Initially the later approach was taken
//  but his caused difficulties since the loader would build the classes in some unknown order.
//
// /floatSerialArray \& nullFloatArray():
// /doubleSerialArray \& nullDoubleArray():
// /RealArray \& nullRealArray():
// /intSerialArray \& nullIntArray():
//	 
// /floatDistributedArray \& nullFloatDistributedArray():
// /doubleDistributedArray \& nullDoubleDistributedArray():
// /RealDistributedArray \& nullRealDistributedArray():
// /IntegerDistributedArray \& nullIntegerDistributedArray():
//	 
// /MappingParameters \& nullMappingParameters():
//	 
// /floatMappedGridFunction \& nullFloatMappedGridFunction():
// /doubleMappedGridFunction \& nullDoubleMappedGridFunction():
// /realMappedGridFunction \& nullRealMappedGridFunction():
// /intMappedGridFunction \& nullIntMappedGridFunction():
//	 
// /floatGridCollectionFunction \& nullFloatGridCollectionFunction():
// /realGridCollectionFunction \& nullRealGridCollectionFunction():
// /doubleGridCollectionFunction \& nullDoubleGridCollectionFunction():
// /intGridCollectionFunction \& nullIntGridCollectionFunction():
//	 
// /BoundaryConditionParameters \& defaultBoundaryConditionParameters():
// /GraphicsParameters \& defaultGraphicsParameters():
//
//\end{testInclude.tex} 


//\begin{>>OvertureInclude.tex}{\subsection{start}} 
int Overture::
start(int & argc, char **&argv)
// =================================================================================================
// /Description:
//    Overture initialization function. Call this routine before calling any Overture functions.
//
// /NOTES:
//    In parallel the value of argc and the values in argv will be sent to all processors.
//   Thus argc and argv will be changed on all processors except processor 0. 
// 
// In parallel call 
//     ParallelUtility::broadCastArgsCleanup(argc,argv);
// to delete the argv arrays created by the parallel broad-cast
//
//\end{OvertureInclude.tex} 
// =================================================================================================
{ 

  // printf(" Overture::start: counter=%i \n",counter);
  if( 0==counter++ )
  {
    // Call the diagnostics mechanism to display memory usage
    // Diagnostic_Manager::report();

    #ifdef USE_PPP
      int numberOfProcessors = 0;
      Optimization_Manager::Initialize_Virtual_Machine ("", numberOfProcessors, argc, argv);
      // assign the Overture MPI communicator:
      OV_COMM = MPI_COMM_WORLD;  // do this for now -- P++ needs to have its own communicator
    #else
      OV_COMM=0;
    #endif

    ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
    Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
//    Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON ); // release memory when done

    // In parallel broadcast the command line arguments from processor 0 to all processors
    #ifndef OV_BUILD_MAPPING_LIBRARY
      ParallelUtility::broadCastArgs(argc,argv);
    #endif    
    // printf(" construct StaticVariables \n");

    // initialize Schewchuk's robust predicates 
    exactinit();

    pNullFloatArray = new floatSerialArray; 
    pNullDoubleArray = new doubleSerialArray; 
    pNullIntArray = new intSerialArray;

    pNullFloatDistributedArray = new floatDistributedArray;
    pNullDoubleDistributedArray = new doubleDistributedArray;
    pNullIntegerDistributedArray = new IntegerDistributedArray;

    pNullMappingParameters= new MappingParameters(true);
    pStaticMappingList = new MappingLinkedList;  // list of Mappings for makeMapping


#ifdef OV_USE_DOUBLE
    pNullRealArray = pNullDoubleArray;
    pNullRealDistributedArray= pNullDoubleDistributedArray;
#else
    pNullRealArray = pNullFloatArray;
    pNullRealDistributedArray= pNullFloatDistributedArray;
#endif

// this can NOT be in the Rapsodi library
#ifndef OV_BUILD_MAPPING_LIBRARY
    assert( pNullFloatMappedGridFunction==0 );
    pNullFloatMappedGridFunction = ::new floatMappedGridFunction;
    pNullDoubleMappedGridFunction= ::new doubleMappedGridFunction;
    pNullIntMappedGridFunction= ::new intMappedGridFunction;

    pNullFloatGridCollectionFunction= new floatGridCollectionFunction;
    pNullDoubleGridCollectionFunction=new doubleGridCollectionFunction;
    pNullIntGridCollectionFunction= new intGridCollectionFunction;

#ifdef OV_USE_DOUBLE
    pNullRealMappedGridFunction= pNullDoubleMappedGridFunction;
    pNullRealGridCollectionFunction= pNullDoubleGridCollectionFunction;
#else
    pNullRealMappedGridFunction= pNullFloatMappedGridFunction;
    pNullRealGridCollectionFunction= pNullFloatGridCollectionFunction;
#endif


    pDefaultBoundaryConditionParameters = new  BoundaryConditionParameters;
#endif

    pDefaultGraphicsParameters= new GraphicsParameters(true);  // used for default arguments

    initializeMappingList();

// need to keep track of if the Graphics Interface was instantiated from getGraphicsInterface
// to avoid deleting it twice
    getGraphicsInterfaceInstantiated = false;
    internalGI = NULL;
  }

  // make sure P++ initiatlization info is printed (includes processor id's)
  fflush(0);
  Communication_Manager::Sync();

  return 0;
}

//\begin{>>OvertureInclude.tex}{\subsection{finish}} 
int Overture::
finish()
// =================================================================================================
// /Description:
//    Overture cleanup function. Call this routine when you are done using Overture.
//\end{OvertureInclude.tex} 
// =================================================================================================
{ 
  if( Communication_Manager::My_Process_Number==0 ) printf("Overture::finish called.\n");
  
  // printf(" Overture::end, counter=%i \n",counter);
  if( 0==--counter )
  {

    // If the shutDownPETSc function pointer is non NULL then we call that routine
    // to shut down PETSc (in buildEquationSolvers.C)
    if( PETScIsInitialized  )
    {
      if( referenceCountForPETSc!=0 )
      {
	printF("Overture::finish:ERROR: the reference count for PETSc =%i is not zero.\n"
               " You should destroy any objects that use PETSc before calling Overture::finish()\n",referenceCountForPETSc);
      }
      if( shutDownPETSc!=NULL )
      {
	printF("Overture::finish: shut down PETSc...\n");
        (*shutDownPETSc)();
      }
      else
      {
        printF("Overture::finish:ERROR: PETSc is initialized but there is no function set to shutdown PETSc\n");
      }
      
    }
    
    // printf(" delete StaticVariables \n");
    destructMappingList();

    delete pNullFloatArray;                  
    delete pNullDoubleArray;                  
    delete pNullIntArray;                   

    delete pNullFloatDistributedArray;
    delete pNullDoubleDistributedArray;
    delete pNullIntegerDistributedArray;

    delete pNullMappingParameters;
    delete pStaticMappingList;

    for( int i=0; i<ReparameterizationTransform::maximumNumberOfRecursionLevels; i++ )
      delete ReparameterizationTransform::localParams[i];

// this can NOT be in the Rapsodi library
#ifndef OV_BUILD_MAPPING_LIBRARY
    ::delete pNullFloatMappedGridFunction;
    ::delete pNullDoubleMappedGridFunction;
    ::delete pNullIntMappedGridFunction;

    delete pNullFloatGridCollectionFunction;
    delete pNullDoubleGridCollectionFunction;
    delete pNullIntGridCollectionFunction;

    delete pDefaultBoundaryConditionParameters;
#endif

    delete pDefaultGraphicsParameters;

// delete the graphics interface
    if (getGraphicsInterfaceInstantiated && internalGI)
    {
      delete internalGI;
      // printf("INFO: The GraphicsInterface was destroyed by Overture::finish\n");
      internalGI = NULL;
      pPlotStuff = NULL;
    }
    
    if( debugFile!=NULL )
    {
      fclose(debugFile);
      debugFile=NULL;
    }

    // Call the diagnostics mechanism to display memory usage
    // Diagnostic_Manager::report();
#ifdef USE_PPP
    Optimization_Manager::Exit_Virtual_Machine();
#endif

    // Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON ); // release memory when done

  }
  return 0;
}

//\begin{>>OvertureInclude.tex}{\subsection{getAbortOption}} 
Overture::AbortEnum Overture::
getAbortOption()
// =================================================================================================
// /Description:
//   Return the current abort option.
// \begin{verbatim}
//    enum AbortEnum
//    {
//      abortOnAbort,
//      throwErrorOnAbort
//    };
// \end{verbatim}
//\end{OvertureInclude.tex} 
// =================================================================================================
{ 
  return abortOption;
} 

//\begin{>>OvertureInclude.tex}{\subsection{setAbortOption}} 
void Overture::
setAbortOption( AbortEnum action )
// =================================================================================================
// /Description:
//    Specify the action to take when the Overture::abort() function is called.
//\end{OvertureInclude.tex} 
// =================================================================================================
{ 
  abortOption=action; 
} 

//\begin{>>OvertureInclude.tex}{\subsection{abort}} 
void Overture::
abort()
// =================================================================================================
// /Description:
//    Abort the program or throw an error depending on the value of getAbortOption
//\end{OvertureInclude.tex} 
// =================================================================================================
{ 
  abort("error");
} 

//\begin{>>OvertureInclude.tex}{\subsection{abort}} 
void Overture::
abort(const aString & message)
// =================================================================================================
// /Description:
//    Abort the program or throw an error depending on the value of getAbortOption
// /message (input) : print this message.
//\end{OvertureInclude.tex} 
// =================================================================================================
{ 
  cout << message << endl;
  if( abortOption==Overture::abortOnAbort )
  {
    printf("Overture::abort: I am now going to purposely abort so that you can get a traceback from a debugger\n");
    ::abort();
  }
  else
  {
    throw "error";
  }
} 


floatSerialArray & Overture::nullFloatArray()
{ 
  if( pNullFloatArray==NULL )
    pNullFloatArray= new floatSerialArray;
  return *pNullFloatArray; 
}
doubleSerialArray & Overture::nullDoubleArray()
{ 
  if( pNullDoubleArray==NULL )
    pNullDoubleArray = new doubleSerialArray;
  return *pNullDoubleArray; 
}  
RealArray & Overture::nullRealArray()
{
  if( pNullRealArray==NULL )
    pNullRealArray = new RealArray;
  return *pNullRealArray; 
}  
intSerialArray & Overture::nullIntArray()
{
  if( pNullIntArray==NULL )
    pNullIntArray = new intSerialArray;
  return *pNullIntArray; 
}  
floatDistributedArray & Overture::nullFloatDistributedArray()
{
  if( pNullFloatDistributedArray==NULL )
    pNullFloatDistributedArray = new floatDistributedArray;
  return *pNullFloatDistributedArray; 
}  
doubleDistributedArray & Overture::nullDoubleDistributedArray()
{
  if( pNullDoubleDistributedArray==NULL )
    pNullDoubleDistributedArray = new doubleDistributedArray;
  return *pNullDoubleDistributedArray; 
}  
RealDistributedArray & Overture::nullRealDistributedArray()
{
  if( pNullRealDistributedArray==NULL )
    pNullRealDistributedArray = new RealDistributedArray;
  return *pNullRealDistributedArray; 
}  
IntegerDistributedArray & Overture::nullIntegerDistributedArray()
{
  if( pNullIntegerDistributedArray==NULL )
    pNullIntegerDistributedArray = new IntegerDistributedArray;
  return *pNullIntegerDistributedArray; 
}  

MappingParameters & Overture::nullMappingParameters()
{
  if( pNullMappingParameters==NULL )
    pNullMappingParameters = new MappingParameters(true);
  return *pNullMappingParameters;
}

MappingLinkedList & Overture::staticMappingList()
{
  if( pStaticMappingList==NULL )
    pStaticMappingList = new MappingLinkedList;
  return *pStaticMappingList;
}

// this can NOT be in the Rapsodi library
#ifndef OV_BUILD_MAPPING_LIBRARY

floatMappedGridFunction & Overture::nullFloatMappedGridFunction()
{
  if( pNullFloatMappedGridFunction==NULL )
    pNullFloatMappedGridFunction = new floatMappedGridFunction;
  return *pNullFloatMappedGridFunction;
}
doubleMappedGridFunction & Overture::nullDoubleMappedGridFunction()
{
  if( pNullDoubleMappedGridFunction==NULL )
    pNullDoubleMappedGridFunction = new doubleMappedGridFunction;
  return *pNullDoubleMappedGridFunction;
}
realMappedGridFunction & Overture::nullRealMappedGridFunction()
{
  if( pNullRealMappedGridFunction==NULL )
    pNullRealMappedGridFunction = new realMappedGridFunction;
  return *pNullRealMappedGridFunction;
}
intMappedGridFunction & Overture::nullIntMappedGridFunction()
{
  if( pNullIntMappedGridFunction==NULL )
    pNullIntMappedGridFunction = new intMappedGridFunction;
  return *pNullIntMappedGridFunction;
}

floatGridCollectionFunction & Overture::nullFloatGridCollectionFunction()
{
  if( pNullFloatGridCollectionFunction==NULL )
    pNullFloatGridCollectionFunction = new floatGridCollectionFunction;
  return *pNullFloatGridCollectionFunction;
}
realGridCollectionFunction & Overture::nullRealGridCollectionFunction()
{
  if( pNullRealGridCollectionFunction==NULL )
    pNullRealGridCollectionFunction = new realGridCollectionFunction;
  return *pNullRealGridCollectionFunction;
}
doubleGridCollectionFunction & Overture::nullDoubleGridCollectionFunction()
{
  if( pNullDoubleGridCollectionFunction==NULL )
    pNullDoubleGridCollectionFunction = new doubleGridCollectionFunction;
  return *pNullDoubleGridCollectionFunction;
}
intGridCollectionFunction & Overture::nullIntGridCollectionFunction()
{
  if( pNullIntGridCollectionFunction==NULL )
    pNullIntGridCollectionFunction = new intGridCollectionFunction;
  return *pNullIntGridCollectionFunction;
}

BoundaryConditionParameters & Overture::defaultBoundaryConditionParameters()
{
  if( pDefaultBoundaryConditionParameters==NULL )
    pDefaultBoundaryConditionParameters = new BoundaryConditionParameters;
 return *pDefaultBoundaryConditionParameters;
}

#endif


GraphicsParameters & Overture::defaultGraphicsParameters()
{
  
  if( pCurrentGraphicsParameters!=NULL )
  {
    return *pCurrentGraphicsParameters;
  }
  if( pDefaultGraphicsParameters==NULL )
    pDefaultGraphicsParameters = new GraphicsParameters(true);

  pCurrentGraphicsParameters=pDefaultGraphicsParameters;
  
  return *pCurrentGraphicsParameters;
}

#include "GL_GraphicsInterface.h"
//\begin{>>OvertureInclude.tex}{\subsection{getGraphicsInterface}} 
GenericGraphicsInterface* Overture::
getGraphicsInterface(const aString & windowTitle/*="Your Slogan Here"*/, const bool initialize/*=true*/,
		     int argc/* = 0*/, char *argv[] /*=NULL*/)
// ========================================================================================
// /Description:
//   Return a pointer to the one and only graphics interface. If the pointer is null,
//   a new graphics interface will be built.
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  if (!pPlotStuff)
  {
    // Note: If you want to make another kind of Graphics Interface, you
    // would change the next lines...
    if (argc==0)
      internalGI = new GL_GraphicsInterface(initialize, windowTitle);
    else
      internalGI = new GL_GraphicsInterface(argc, argv, windowTitle);
    getGraphicsInterfaceInstantiated = true;
    pPlotStuff = internalGI;
    // printf("INFO: The GraphicsInterface was instantiated by Overture::getGraphicsInterface\n");
  }
  
  return pPlotStuff;
}

//\begin{>>OvertureInclude.tex}{\subsection{setGraphicsInterface}} 
void Overture::
setGraphicsInterface( GenericGraphicsInterface *ps)
// ========================================================================================
// /Description:
//   Set the default graphics interface. This is normally called by the GenericGraphicsInterface constructor,
// there is no need for a typical user to call this function.
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  pPlotStuff=ps;
}


//\begin{>>OvertureInclude.tex}{\subsection{getMappingList}} 
ListOfMappingRC* Overture::
getMappingList()
// ========================================================================================
// /Description:
//   Return a pointer to the default list of mappings. This pointer may be NULL.
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  return pMappingList;
}

//\begin{>>OvertureInclude.tex}{\subsection{setMappingList}} 
void Overture::
setMappingList(ListOfMappingRC *list)
// ========================================================================================
// /Description:
//   Set the default list of mappings.
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  pMappingList=list;
}


//\begin{>>OvertureInclude.tex}{\subsection{setDefaultGraphicsParameters}} 
void Overture::
setDefaultGraphicsParameters( GraphicsParameters *gp /* =NULL */ )
// ========================================================================================
// /Description:
//   Set the default graphics parameters. By default reset the graphics parameters to
// the standard one.
//
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  if( gp!=NULL )
  {
    pCurrentGraphicsParameters=gp;
  }
  else
  {
    pCurrentGraphicsParameters=pDefaultGraphicsParameters;
  }
  
}

//\begin{>>OvertureInclude.tex}{\subsection{openDebugFile}} 
void Overture::
openDebugFile()
// ========================================================================================
// /Description:
//    Open the file Overture::debugFile for writing debugging info to.
//  On a serial machine the file is named "overture.debug"
//  On a parallel machine the file on processor 0 is named "overture.debug"
//  while the file on processor X is named "overtureX.debug"
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  if( Overture::debugFile==NULL )
  {
    int myid=max(0,Communication_Manager::My_Process_Number);
    if( myid==0 )
      debugFile = fopen("overture.debug","w" ); 
    else
      debugFile = fopen(sPrintF("overture%i.debug",myid),"w" ); 

  }
}

// ****************** memory checking *********************************8

bool Overture::checkMemoryUsageIsOn=false;
real Overture::maximumMemoryUse=0.;
real Overture::oldMaximumMemoryInUse=0.;
real Overture::memoryUsageScaleFactor=1.1; // print message when memory has increased by this amount from previous 

// ========================================================================================
/// \brief turn on or off Overture's checking of the memory usage. 
/// \param trueOrFalse (input) : if true then print messages (from checkMemoryUsage) when the memory has increased
///     by a factor of memUsageScaleFactor from the previous maximum usage. 
/// \param memUsageScaleFactor (input) : indicates how often to print messages. 
// ========================================================================================
void Overture::
turnOnMemoryChecking(bool trueOrFalse /* =true */, real memUsageScaleFactor /* =1.1 */)
{
  checkMemoryUsageIsOn=trueOrFalse;
  memoryUsageScaleFactor=memUsageScaleFactor;
}

//\begin{>>OvertureInclude.tex}{\subsection{checkMemoryUsage}} 
real Overture::
checkMemoryUsage(const aString & label, FILE *file /* =stdout */)
// ========================================================================================
// /Description:
//   Check the current memory usage in Mega-bytes and print a message if the memory use
//  has increased by 10 percent. You must first call turnOnMemoryChecking(true) for this
//  function to be turned on.
//
// /label (input) : use this label on message
// /file (input): output messages to this file. If NULL, output no message.
// /Return value: current memory use (Mb).
// 
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  real mem=0.;
  if( checkMemoryUsageIsOn )
  {
    
    mem = getCurrentMemoryUsage();
    if( mem > maximumMemoryUse )
    {
      if( mem>oldMaximumMemoryInUse*memoryUsageScaleFactor && file!=NULL )
      {
        int myid=max(0,Communication_Manager::My_Process_Number);
        fprintf(file,"Overture::checkMemoryUsage: %s, myid=%i: memory usage has increased to %g (Mb)\n",
              (const char*)label,myid,mem);
        oldMaximumMemoryInUse=mem;
      }
      maximumMemoryUse=mem;
    }
  }
  return mem;
}

//\begin{>>OvertureInclude.tex}{\subsection{getMaximumMemoryUsage}} 
real Overture::
getMaximumMemoryUsage()
// ========================================================================================
// /Description:
//   Return the maximum memory use recorded by calls to checkMemoryUsage.
// 
// /Return value: maximum memory use detected (Mb).
// 
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  return maximumMemoryUse;
}


//\begin{>>OvertureInclude.tex}{\subsection{checkMemoryUsage}} 
real Overture::
printMemoryUsage(const aString & label, FILE *file /* =stdout */)
// ========================================================================================
// /Description:
//   Display the current memory usage.
//
// /label (input) : use this label on message
// /file (input): output messages to this file. If NULL, output no message.
// 
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  fflush(file);
  Communication_Manager::Sync();

  real mem = getCurrentMemoryUsage();
  if( mem > maximumMemoryUse )
    maximumMemoryUse=mem;

  int myid=max(0,Communication_Manager::My_Process_Number);
  fprintf(file,"Overture::printMemoryUsage: %s, myid=%i: memory= %g (Mb) [max-recorded=%g (Mb)]\n",
	  (const char*)label,myid,mem,maximumMemoryUse);
  fflush(file);
  Communication_Manager::Sync();
  return mem;
}

//\begin{>>OvertureInclude.tex}{\subsection{incrementReferenceCountForPETSc}}
int Overture::
incrementReferenceCountForPETSc()
// ========================================================================================
// /Description:
//   Increment the reference count for objects that use PETSc.
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  referenceCountForPETSc++;
  PETScIsInitialized=true;
  return 0;
}

//\begin{>>OvertureInclude.tex}{\subsection{decrementReferenceCountForPETSc}}
int Overture::
decrementReferenceCountForPETSc()
// ========================================================================================
// /Description:
//   Decrement the reference count for objects that use PETSc.
//\end{OvertureInclude.tex} 
// ========================================================================================
{
  referenceCountForPETSc--;
  return 0;
}

