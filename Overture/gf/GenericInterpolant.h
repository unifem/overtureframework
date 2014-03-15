#ifndef GENERIC_INTERPOLANT_H
#define GENERIC_INTERPOLANT_H "GenericInterpolant.h"

//============================================================================
//  GenericInterpolant Class
//============================================================================

class GenericInterpolant
{
 public:
  GenericInterpolant(){};
  ~GenericInterpolant(){};
  virtual void interpolate( realCompositeGridFunction & u )=0
  
};



#endif
