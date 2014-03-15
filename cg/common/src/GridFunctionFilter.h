#include "Overture.h"

class GridFunctionFilter
{
public:

GridFunctionFilter();
~GridFunctionFilter();

enum FilterTypeEnum
{
  explicitFilter,
  implicitFilter
};


// apply the filter to v 
int 
applyFilter( realCompositeGridFunction & v,
	     Range & C,
	     realCompositeGridFunction & w /* work space */ );


// update parameters
int 
update( GenericGraphicsInterface & gi );


static int debug;

// make these public for now 
public:

FilterTypeEnum filterType;

int orderOfFilter;
int filterFrequency;
int numberOfFilterIterations;
int numberOfFilterStages;

real filterCoefficient;

int numberOfFilterApplications;

};


