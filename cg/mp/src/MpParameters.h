#ifndef MP_PARAMETERS
#define MP_PARAMETERS

// Parameters for the multi-physics solver

#include "Parameters.h"

// Here are the run time and PDE parameters
class MpParameters : public Parameters
{
public:

enum PlotOptionEnum 
{
  plotGrid        =1,
  plotContour     =2,
  plotStreamlines =4,
  plotDisplacement=8,
  plotNothing
};

// There are different multi-domain time stepping algorithms: 
enum MultiDomainAlgorithmEnum
{
  defaultMultiDomainAlgorithm=0,
  stepAllThenMatchMultiDomainAlgorithm
};


MpParameters(const int & numberOfDimensions0=3);
~MpParameters();

int 
displayInterfaceInfo(FILE *file = stdout );

virtual int 
displayPdeParameters(FILE *file = stdout );

virtual int
saveParametersToShowFile();

virtual int
setParameters(const int & numberOfDimensions0=2, 
	      const aString & reactionName =nullString);
virtual int 
setPdeParameters(CompositeGrid & cg, const aString & command = nullString,
                 DialogData *interface =NULL );

// virtual int
// setUserDefinedParameters();

};

#endif
