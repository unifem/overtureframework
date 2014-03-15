#ifndef ASSIGN_INTERP_NEIGHBOURS
#define ASSIGN_INTERP_NEIGHBOURS "AssignInterpNeighbours"

// ==========================================================================
//  This class is used to assign the unused points next to interpolation
// points so that a wide (e.g. 5-pt) stencil can be used with fewer layers
// of interpolation points. This assignment is currently done with extrapolation
// although in the future we could consider interpolating these points instead.
// ==========================================================================

#include "Overture.h"


class AssignInterpNeighbours
{
public:

AssignInterpNeighbours();

// copy constructor
AssignInterpNeighbours( const AssignInterpNeighbours & x );

~AssignInterpNeighbours();

// Assign values to the unused points next to interpolation points
int 
assign( realMappedGridFunction & uA, Range & C, const BoundaryConditionParameters & bcParams );

// Call this routine when the grid has changed and we need to re-initialize
int gridHasChanged();

AssignInterpNeighbours & operator= ( const AssignInterpNeighbours & x );

// Provide the interpolation point array (used in serial only)
void setInterpolationPoint( intArray & interpolationPoint );

// return size of this object  
real sizeOf(FILE *file = NULL ) const;


static int debug;

protected:

  // This next enum is used for setting an error status
  enum ErrorStatusEnum
  {
    noErrors=0,
    errorInFindInterpolationNeighbours
  } errorStatus;

// setup routine
int 
setup();

// routine for setting up arrays for assigning the neighbours to interpolation points
int 
findInterpolationNeighbours( MappedGrid & mg );


int isInitialized;

int numberOfInterpolationNeighbours;

int maximumWidthToExtrapolationInterpolationNeighbours; 
IntegerArray *extrapolateInterpolationNeighbourPoints;
IntegerArray *extrapolateInterpolationNeighboursDirection;
IntegerArray *extrapolateInterpolationNeighboursVariableWidth;  

intArray *interpolationPoint;

static FILE *debugFile;  // make one debug file for all instances (we use the same name)

// for communicating values:

int npr, nps;     // number of proc. that we receieve or send data to
int *ppr,*pps;    // list of processors for rec. and sending to
int *nar, **iar;  // list of points to recieve from other processors
int *nas, **ias;  // list of points to send to other processor


  #ifdef USE_PPP
    MPI_Comm AIN_COMM;  // Communicator for the parallel interpolator
  #else
    int AIN_COMM;
  #endif

};


#endif
