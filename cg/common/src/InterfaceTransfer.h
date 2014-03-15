#ifndef INTERFACE_TRANSFER_H
#define INTERFACE_TRANSFER_H

// -------------------------------------------------------------------------------------
// Class InterfaceTransfer : used to transfer information across an interface between
// two composite grids.
// -------------------------------------------------------------------------------------

#include "Overture.h"

// forward declarations: 
class InterfaceDescriptor;
class DomainSolver;
class Parameters;
class InterpolatePointsOnAGrid;


class InterfaceTransfer
{
public:


InterfaceTransfer();
~InterfaceTransfer();

// Initialize the interface transfer at an interface 
// (defines 2 transfer functions, from one side to the other and vice versa)
int 
initialize( InterfaceDescriptor & interfaceDescriptor,
            std::vector<DomainSolver*> domainSolver,
            std::vector<int> & gfIndex,
            Parameters & parameters  );


// Set the interface transfer interpolation width
int
setInterpolationWidth( int width, int interfaceSide=-1 );

// Set the default interface transfer interpolation widths. These values will apply when
// new InterfaceTransfer objects are built.
static int 
setDefaultInterpolationWidth( int width, int interfaceSide=-1 );

// Transfer the data from one side to the other
int 
transferData( int domainSource, int domainTarget, 
              RealArray **sourceDataArray, Range & Cs,
              RealArray **targetDataArray, Range & Ct,
	      InterfaceDescriptor & interfaceDescriptor,
	      std::vector<DomainSolver*> domainSolver,
	      std::vector<int> & gfIndex,
	      Parameters & parameters );


protected:

int initialized; 
InterpolatePointsOnAGrid *interpolatePointsOnAGrid;    // for interpolating points on the interface
IntegerArray *indirectionArray;                        // holds indicies (i1,i2,i3,grid) of points on a interface
int interpolationWidth[2];                             // interpolation width (one for each transfer direction)
static int defaultInterpolationWidth[2];               // default interpolation width (one for each transfer direction)

int 
internalInterpolate( RealArray **sourceDataArray, Range & Cs,
                     RealArray **targetDataArray, Range & Ct, 
                     CompositeGrid & cg, int interfaceSide  );


};


#endif
