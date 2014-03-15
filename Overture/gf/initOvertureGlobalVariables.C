#include "GenericDataBase.h"
#include "aString.H"
#include "wdhdefs.h"
// This file is placed in the static library so that the following global variables
// are initialized properly when using a dyanmic library (libOverture.so)


const real Pi = 4.*atan(1.);
const real twoPi = 8.*atan(1.);

// RealArray nullRealArray;  
// IntegerArray  nullIntArray;
// RealDistributedArray nullRealDistributedArray;  
// IntegerDistributedArray  nullIntegerDistributedArray;
// IntegerDistributedArray  nullIntegerDistributedArray;

// MappingParameters nullMappingParameters(TRUE); 
// Mapping::LinkedList Mapping::staticMapList;  // list of Mappings for makeMapping

const aString nullString="";                    // null string for default arguments
const aString blankString=" ";                   // blank string for default arguments
const Index nullIndex;
const Range nullRange;
// for specifying the special "axis" Range for a face :
const Range faceRange=Range(333333,444444,111111); 


// floatMappedGridFunction  nullFloatMappedGridFunction;
// doubleMappedGridFunction nullDoubleMappedGridFunction;
// intMappedGridFunction    nullIntMappedGridFunction;
// realMappedGridFunction   nullRealMappedGridFunction;

// floatGridCollectionFunction  nullFloatGridCollectionFunction;
// doubleGridCollectionFunction nullDoubleGridCollectionFunction;
// intGridCollectionFunction    nullIntGridCollectionFunction;

// //floatMultigridCompositeGridFunction  nullFloatMultigridCompositeGridFunction;
// //doubleMultigridCompositeGridFunction nullDoubleMultigridCompositeGridFunction;
// //intMultigridCompositeGridFunction    nullIntMultigridCompositeGridFunction;

// BoundaryConditionParameters defaultBoundaryConditionParameters;

// GraphicsParameters defaultGraphicsParameters(TRUE);  // used for default arguments

void 
initOvertureGlobalVariables()
{
}
