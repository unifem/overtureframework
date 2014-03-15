#ifndef TestParameters_h
#define TestParameters_h

#include <Overture.h>
#include <NameList.h>
#include "TwilightZone.h"
#include "InterpolateParameters.h"

class TestParameters
{
 public:

  TestParameters (const int numberOfDimensions_ = InterpolateParameters::defaultNumberOfDimensions);
  ~TestParameters();

  int interactivelySetParameters ();
  void display () const;

  bool debug;

  bool plotting;
  IntegerArray amrRefinementRatio;  
  int numberOfDimensions;
  GridFunctionParameters::GridFunctionType gridCentering;
  InterpolateParameters::InterpolateType interpolateType;
  int interpolateOrder;
  bool useGeneralInterpolationFormula;

  aString TZFunctionTypeString[3];
  TwilightZoneFlowFunctionType tzType;

//String InterpolateOffsetDirectionString[2];
//IntegerArray interpolateOffsetDirection;

 public:  

  NameList nl;
};

#endif
  

  









