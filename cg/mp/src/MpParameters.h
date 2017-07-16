#ifndef MP_PARAMETERS
#define MP_PARAMETERS

// Parameters for the multi-physics solver

#include "Parameters.h"

// loop over the value domains in default order
#define ForDomain(d) for( int d=0; d<domainSolver.size(); d++ )\
                       if( domainSolver[d]!=NULL )

// define a loop over all valid domains (in the order indicated by domainOrder): 
#define ForDomainOrdered(d) for( int dd=0, d=0; dd<domainSolver.size(); dd++ )\
                              if( domainSolver[d=domainOrder[dd]]!=NULL )


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
  stepAllThenMatchMultiDomainAlgorithm,
  multiStageAlgorithm
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

int 
setupMultiStageAlgorithm(CompositeGrid & cg, DialogData & dialog );

// virtual int
// setUserDefinedParameters();

};

#endif
