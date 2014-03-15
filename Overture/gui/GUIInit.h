#ifndef GUI_INIT_H
#define GUI_INIT_H

#include <string>
#include "kk_Array.hh"

#define aString std::string


#ifndef NO_APP

#include "OvertureTypes.h"
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
class ListOfMappingRC;
#endif

#include "GUITypes.h"

class GraphicsParameters;
class GenericGraphicsInterface;

class Overture
{
 public:
 static int start(int argc, char *argv[]);
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

#ifndef NO_APP
  static floatSerialArray & nullFloatArray();
  static doubleSerialArray & nullDoubleArray();
  static intSerialArray & nullIntArray();
#else
  static KK::Array<float> & nullFloatArray();
  static KK::Array<double> & nullDoubleArray();
  static KK::Array<int> & nullIntArray();
#endif
  static RealArray & nullRealArray();
	 
#ifndef NO_APP
  static floatDistributedArray & nullFloatDistributedArray();
  static doubleDistributedArray & nullDoubleDistributedArray();
  static RealDistributedArray & nullRealDistributedArray();
  static IntegerDistributedArray & nullIntegerDistributedArray();
	 
  static MappingParameters & nullMappingParameters();
	 
  static floatMappedGridFunction & nullFloatMappedGridFunction();
  static doubleMappedGridFunction & nullDoubleMappedGridFunction();
  static realMappedGridFunction & nullRealMappedGridFunction();
  static intMappedGridFunction & nullIntMappedGridFunction();
	 
  static floatGridCollectionFunction & nullFloatGridCollectionFunction();
  static realGridCollectionFunction & nullRealGridCollectionFunction();
  static doubleGridCollectionFunction & nullDoubleGridCollectionFunction();
  static intGridCollectionFunction & nullIntGridCollectionFunction();
	 
  static BoundaryConditionParameters & defaultBoundaryConditionParameters();
#endif

  static GraphicsParameters & defaultGraphicsParameters();
  static void setDefaultGraphicsParameters( GraphicsParameters *gp =NULL );

  static GenericGraphicsInterface* 
  getGraphicsInterface(const aString & windowTitle="Your Slogan Here", const bool initialize=true,
		       int argc=0, char *argv[]=NULL);
  static void 
  setGraphicsInterface( GenericGraphicsInterface *ps);

#ifndef NO_APP
  static ListOfMappingRC* getMappingList();
  static void setMappingList(ListOfMappingRC *list);
#endif

 protected:
  static GenericGraphicsInterface *pPlotStuff;

#ifndef NO_APP
  static ListOfMappingRC *pMappingList;
#endif

  static AbortEnum abortOption;

 private:

#ifndef NO_APP
  static floatSerialArray *pNullFloatArray;  
  static doubleSerialArray *pNullDoubleArray;  
  static intSerialArray  *pNullIntArray;
#else
  static KK::Array<float> *pNullFloatArray;  
  static KK::Array<double> *pNullDoubleArray;  
  static KK::Array<int>  *pNullIntArray;
#endif
  static RealArray *pNullRealArray;  

#ifndef NO_APP
  static floatDistributedArray *pNullFloatDistributedArray;  
  static doubleDistributedArray *pNullDoubleDistributedArray;  
  static RealDistributedArray *pNullRealDistributedArray;  
  static IntegerDistributedArray  *pNullIntegerDistributedArray;

#endif

#ifndef NO_APP
  static MappingParameters *pNullMappingParameters;
#endif

#ifndef NO_APP
  static floatMappedGridFunction  *pNullFloatMappedGridFunction;
  static doubleMappedGridFunction *pNullDoubleMappedGridFunction;
  static intMappedGridFunction    *pNullIntMappedGridFunction;
  static realMappedGridFunction   *pNullRealMappedGridFunction;

  static floatGridCollectionFunction  *pNullFloatGridCollectionFunction;
  static doubleGridCollectionFunction *pNullDoubleGridCollectionFunction;
  static intGridCollectionFunction    *pNullIntGridCollectionFunction;
  static realGridCollectionFunction   *pNullRealGridCollectionFunction;

  static BoundaryConditionParameters *pDefaultBoundaryConditionParameters;
#endif

  static GraphicsParameters *pDefaultGraphicsParameters;  // used for default arguments
  static GraphicsParameters *pCurrentGraphicsParameters;

};

#ifndef NO_APP
#undef realMappedGridFunction
#undef realGridCollectionFunction
#endif


#endif
