#include "Mapping.h"
#include "MappingP.h"

//===========================================================================
// Here is a class to use to pass parameters to map and inverseMap
//===========================================================================

MappingParameters:: 
MappingParameters( const int isNull0 )
{
  isNull=isNull0;
  periodicityOfSpace=0;
  periodicityVector.redim(3,3);
  periodicityVector=0.;
  computeGlobalInverse=TRUE;            // compute "full" inverse by default
  coordinateType=Mapping::cartesian;    // evaluate mapping in cartesian coordinates by default
  approximateGlobalInverse=NULL;
  exactLocalInverse=NULL;
}

MappingParameters:: 
~MappingParameters()
{
}

// Copy constructor is deep by default
MappingParameters:: 
MappingParameters( const MappingParameters & params, const CopyType copyType )
{
  if( copyType==DEEP )
  {
    *this=params;
  }
  else
  {
    cout << "MappingParameters:: sorry no shallow copy constructor, doing a deep! \n";
    *this=params;
  }
}


MappingParameters & MappingParameters::
operator =( const MappingParameters & X )
{
  isNull                   =X.isNull;
  periodicityOfSpace       =X.periodicityOfSpace;
  periodicityVector.redim(3,3);
  periodicityVector        =X.periodicityVector;
  computeGlobalInverse     =X.computeGlobalInverse;
  coordinateType           =X.coordinateType;
  approximateGlobalInverse =X.approximateGlobalInverse;
  exactLocalInverse        =X.exactLocalInverse;
  return *this;
}

  
