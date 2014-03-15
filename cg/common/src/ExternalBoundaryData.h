#ifndef EXTERNAL_BOUNDARY_DATA_H
#define EXTERNAL_BOUNDARY_DATA_H

#include "Overture.h"

// This class is used to access time depenedent boundary condition data that has
// been saved in a file.
class ExternalBoundaryData 
{
public:

enum ExternalFileTypeEnum
{
  probeBoundingBox   // file was saved with cg 
};
  


ExternalBoundaryData();
~ExternalBoundaryData();

int 
getBoundaryData( real t, CompositeGrid & cg, const int side, const int axis, const int grid, RealArray & bd );

int 
update( GenericGraphicsInterface & gi );


protected:

int orderOfTimeInterpolation, orderOfSpaceInterpolation;

ExternalFileTypeEnum externalFileType;
GenericDataBase *pdb;
int numberOfTimes;
RealArray times;
int current;

};

  
#endif
