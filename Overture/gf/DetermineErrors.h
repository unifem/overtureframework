#ifndef DETERMINE_ERRORS_H
#define DETERMINE_ERRORS_H

// This class can be used to 
//      o compute errors in solutions, 
//      o to print errors and solutions in a nicely formatted form


#include "Overture.h"

class DetermineErrors
{
 public:

  enum ErrorOption
  {
    includeBoundaries=1,
    includeGhostPoints=2
  };
  
  DetermineErrors(OGFunction *exactSolution=NULL);
  ~DetermineErrors();
  
  real maximumError(realMappedGridFunction & u, 
		    realMappedGridFunction & error,
		    const int & option= includeBoundaries,
                    FILE *outputFile = NULL  );

  int setExactSolution(OGFunction *exactSolution);

 protected:
  OGFunction *exactSolution;
  realCompositeGridFunction cgfError;
  realMappedGridFunction mgfError;
  

};

#endif
