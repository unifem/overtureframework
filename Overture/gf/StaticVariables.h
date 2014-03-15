#ifndef STATIC_VARIABLES_H
#define STATIC_VARIABLES_H 

#include "A++.h"

// --------- StaticVariables.h ---------------
class StaticVariables
{
  static floatArray *nullFArray;
 public:
  static floatArray nullFloatArray(){return * nullFArray; }  
  StaticVariables(){nullFArray=new floatArray; (*nullFArray).redim(3); *nullFArray=5.;}
  ~StaticVariables(){}
};


class initStaticVariables
{
  StaticVariables *s;
 public:
  initStaticVariables();
  ~initStaticVariables();
};

#endif
