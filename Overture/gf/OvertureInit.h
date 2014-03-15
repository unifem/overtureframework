#ifndef OVERTURE_INIT_H
#define OVERTURE_INIT_H

class floatMappedGridFunction;
class doubleMappedGridFunction;
class intMappedGridFunction;
// class realMappedGridFunction;
#ifdef OV_USE_DOUBLE
#define realMappedGridFunction doubleMappedGridFunction
#define realGridCollectionFunction doubleGridCollectionFunction
#else
#define realMappedGridFunction floatMappedGridFunction
#define realGridCollectionFunction floatGridCollectionFunction
#endif

class floatGridCollectionFunction;
class doubleGridCollectionFunction;
class intGridCollectionFunction;

class MappingParameters;
class BoundaryConditionParameters;
class GraphicsParameters;
class GenericGraphicsInterface;
class ListOfMappingRC;
class Mapping;
class MappingLinkedList;

class Overture
{
 public:
 static int start(int & argc, char **&argv);
 static int finish();

  enum AbortEnum
  {
    abortOnAbort,
    throwErrorOnAbort
  };
  static AbortEnum getAbortOption();
  static void setAbortOption( AbortEnum action );
  static void abort();
  static void abort(const aString & message);

  // memory usage functions:
  // return memory use in Mb:
  static real getCurrentMemoryUsage();                       
  // Turn on the checking of the memory usage (needed for checkMemoryUsage)
  static void turnOnMemoryChecking(bool trueOrFalse=true, real memoryUsageScaleFactor=1.1 ); 
  // check current memory usage and print a message if it has increased by a certain fraction:
  static real checkMemoryUsage(const aString & label, FILE *file=stdout);
  static real getMaximumMemoryUsage();
  // print current memory usage:
  static real printMemoryUsage(const aString & label, FILE *file=stdout);

  static int incrementReferenceCountForPETSc();
  static int decrementReferenceCountForPETSc();

  static floatSerialArray & nullFloatArray();
  static doubleSerialArray & nullDoubleArray();
  static RealArray & nullRealArray();
  static intSerialArray & nullIntArray();
	 
  static floatDistributedArray & nullFloatDistributedArray();
  static doubleDistributedArray & nullDoubleDistributedArray();
  static RealDistributedArray & nullRealDistributedArray();
  static IntegerDistributedArray & nullIntegerDistributedArray();
	 
  static MappingParameters & nullMappingParameters();
  static MappingLinkedList & staticMappingList();
	 
  static floatMappedGridFunction & nullFloatMappedGridFunction();
  static doubleMappedGridFunction & nullDoubleMappedGridFunction();
  static realMappedGridFunction & nullRealMappedGridFunction();
  static intMappedGridFunction & nullIntMappedGridFunction();
	 
  static floatGridCollectionFunction & nullFloatGridCollectionFunction();
  static realGridCollectionFunction & nullRealGridCollectionFunction();
  static doubleGridCollectionFunction & nullDoubleGridCollectionFunction();
  static intGridCollectionFunction & nullIntGridCollectionFunction();
	 
  static BoundaryConditionParameters & defaultBoundaryConditionParameters();
  static GraphicsParameters & defaultGraphicsParameters();
  static void setDefaultGraphicsParameters( GraphicsParameters *gp =NULL );

  static GenericGraphicsInterface* 
  getGraphicsInterface(const aString & windowTitle="Your Slogan Here", const bool initialize=true,
		       int argc=0, char *argv[]=NULL);
  static void 
  setGraphicsInterface( GenericGraphicsInterface *ps);

  static ListOfMappingRC* getMappingList();
  static void setMappingList(ListOfMappingRC *list);

  static FILE* debugFile;
  static void openDebugFile();
  static void (*shutDownPETSc)();     // Here is the function that will shut down PETSc. (NULL if we are not using PETSc)

  #ifdef USE_PPP
    static MPI_Comm OV_COMM;  // Overture COMM_WORLD for MPI
  #else
    static int OV_COMM;
  #endif

 protected:
  static GenericGraphicsInterface *pPlotStuff;
  static ListOfMappingRC *pMappingList;
  static AbortEnum abortOption;

 private:

  static floatSerialArray *pNullFloatArray;  
  static doubleSerialArray *pNullDoubleArray;  
  static RealArray *pNullRealArray;  
  static intSerialArray  *pNullIntArray;

  static floatDistributedArray *pNullFloatDistributedArray;  
  static doubleDistributedArray *pNullDoubleDistributedArray;  
  static RealDistributedArray *pNullRealDistributedArray;  
  static IntegerDistributedArray  *pNullIntegerDistributedArray;


  static MappingParameters *pNullMappingParameters;
  static MappingLinkedList *pStaticMappingList;

  static floatMappedGridFunction  *pNullFloatMappedGridFunction;
  static doubleMappedGridFunction *pNullDoubleMappedGridFunction;
  static intMappedGridFunction    *pNullIntMappedGridFunction;
  static realMappedGridFunction   *pNullRealMappedGridFunction;

  static floatGridCollectionFunction  *pNullFloatGridCollectionFunction;
  static doubleGridCollectionFunction *pNullDoubleGridCollectionFunction;
  static intGridCollectionFunction    *pNullIntGridCollectionFunction;
  static realGridCollectionFunction   *pNullRealGridCollectionFunction;

  static BoundaryConditionParameters *pDefaultBoundaryConditionParameters;

  static GraphicsParameters *pDefaultGraphicsParameters;  // used for default arguments
  static GraphicsParameters *pCurrentGraphicsParameters;

  static real maximumMemoryUse, oldMaximumMemoryInUse;  // keeps track of maximum memory used
  static bool checkMemoryUsageIsOn;
  static real memoryUsageScaleFactor; // =1.1 : print message when memory has increased by this amount from previous message

  // We must only shutdown PETSc once so we keep track of it's use here
  static int referenceCountForPETSc;  // keeps track of objects that reference PETSc
  static bool PETScIsInitialized;     // true if PETSc has been initialized
};

#undef realMappedGridFunction
#undef realGridCollectionFunction
#endif
