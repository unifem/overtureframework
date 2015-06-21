#ifndef EYE_CURVES_H
#define EYE_CURVES_H "EyeCurves.h"
//==============================================================================================
// EyeCurves : this class can be used to evaluate the boundary of the eyelid over time.
//
//=============================================================================================
#include "Overture.h"  

//===============================================================================================
/// \brief This class can be used to evaluate the boundary of the eyelid over time.
/// \Authors original code by Gregory Barron (RIT) (2015).
///          Turned into a clas by WDH, (2015).
// ==============================================================================================
class EyeCurves
{

public:

EyeCurves();
~EyeCurves();

int getEyeCurve( RealArray & curve , real t, int numPoints );

void saveEyeCurve( real t, int numPoints, aString & fileName );

protected:
  real coords(real *xSmoothsPtr, real space);
  void initializeCoefficients(real *xSmoothsPtr, real time, real *Xstorage );
  void readFile();

private:

  int numSpaceModes, numSequences; /* Numbers for numSpaceModes and numTimeModes */
  int numTimeModes, numTimes;      /* reflect what was used in MATLAB            */
  int totalStartSize;

  real *xSmooths;
  real *ySmooths;

  real xScale;  // scale coordinates in space by this amount
};
#endif
