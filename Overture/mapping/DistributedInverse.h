#ifndef DISTRIBUTED_INVERSE_H

#include "Mapping.h"
#include "BoundingBox.h"  // define BoundingBox and BoundingBoxStack

// class ApproximateGlobalInverse;
// class ExactLocalInverse;

// ==================================================================================
/// \brief Define an inverse for mappings that are distributed in parallel.
// ==================================================================================
class DistributedInverse
{
public:

DistributedInverse(Mapping& map);
~DistributedInverse();

int computeBoundingBoxes();

int get( const GenericDataBase & dir, const aString & name);    // get from a database file

const RealArray & getBoundingBox() const;
const BoundingBox& getBoundingBoxTree(int side, int axis);

int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

// return size of this object  
real sizeOf(FILE *file = NULL ) const;


protected:

Mapping & map;

// realArray & grid;  // reference to Mapping::grid
// ApproximateGlobalInverse *agi;
// ExactLocalInverse *eli;

bool boundingBoxesComputed;
RealArray boundingBox;
BoundingBox boundingBoxTree[2][3];


};


#endif
