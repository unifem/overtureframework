#ifndef MAPPINGWS_H
#define MAPPINGWS_H "MappingWS.h"


#include <A++.h>

#include "OvertureTypes.h"     // define real to be float or double

#include "wdhdefs.h"           // some useful defines and constants

//===========================================================================
/// \brief This object holds workspace data for evaluating and inverting Mappings.
//===========================================================================
class MappingWorkSpace
{
 public:
  RealArray x0;      // new copy needed so we can shift x in the periodic case
  RealArray r0;      // or if the inverse is "blocked"
  RealArray rx0; 
  IntegerArray index0;   // index pointer from r0 back to r
  bool index0IsSequential;
  Index I0;

  MappingWorkSpace( )
  {
    index0IsSequential=TRUE;
  }
  virtual ~MappingWorkSpace() //kkc 040415 added virtual to get rid of warnings
  {
  }

  // return size of this object  
  virtual real sizeOf(FILE *file = NULL ) const;

};

#endif   // MAPPINGWS_H
