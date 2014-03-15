#ifndef CHECKER_H
#define CHECKER_H

#include "aString.H"
#include "OvertureTypes.h"     // define real to be float or double
#include "wdhdefs.h"           // some useful defines and constants
#include "mathutil.h"          // define max, min,  etc

// This class is used for writing a check file for regression testing

class Checker
{
  
 public:
  Checker(const aString & checkFileName );
  ~Checker();
  
  // set the cutOff for errors (errors below this value are set to zero)
  void setCutOff( real cutOff );
  // set label numbered "index"
  void setLabel(const aString & label, const int index);
  
  void printMessage(const aString & label, real error, real time, real timeInit=-1. );

 protected:

  real errorCutOff;
  FILE *checkFile;
  aString checkFileLabel[4];
  aString fileName;
};

#endif
