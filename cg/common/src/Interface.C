#include "Interface.h"
#include "InterfaceTransfer.h"

InterfaceData::
InterfaceData()
{
  t=-1.;
}

InterfaceData::
~InterfaceData()
{
}



InterfaceDataHistory::
InterfaceDataHistory()
{
  current=-1;
}

InterfaceDataHistory::
~InterfaceDataHistory()
{
}

// ==========================================================================================
/// \brief This class holds information about a particular grid face that belongs to an interface
/// \details This object contains information that describes one face of an interface.
///      A given interface will have at least two GridFaceDescriptor's - one for each side of
///      the interface. If there are multiple overlapping grids that form one side of the
///     interface then there will be multiple GridFaceDescriptor's for even one side. 
// ==========================================================================================
GridFaceDescriptor::
GridFaceDescriptor(int domain_, int grid_, int side_, int axis_ )
{ 
  domain=domain_; grid=grid_; side=side_; axis=axis_;

  interfaceBC=-1;
  u=NULL;
  a[0]=1.; a[1]=0; a[2]=0.;
}

GridFaceDescriptor::
~GridFaceDescriptor()
{
}

// ==========================================================================================
/// \brief This class holds information about and interface such as the 
///  the lists of grid faces that are adjacent to a particular interface.
/// \details This class holds the information that defines a single interface. 
///  gridList1: list of GridFaceDescriptor's for grid faces adjacent to side 1 of this interface
///  gridList2: list of GridFaceDescriptor's for grid faces adjacent to side 2 of this interface
// ==========================================================================================

InterfaceDescriptor::
InterfaceDescriptor()
{
  domain1=-1;                               // identifier, 0,1,... for the domain associated with side1
  domain2=-1;                               // identifier, 0,1,... for the domain associated with side2
  interfaceTransfer=NULL;                   // This object knows how to transfer info across an interface
  interfaceTolerance=1.e-3;                 // tolerance for satisfying the interface equations
  interfaceOmega=1.;                        // relaxation parameter for solving the interface equations
  maximumNumberOfIntefaceIterations=100;    // max iterations allowed when solving the interface equations
  estimatedConvergenceRate=0.;              // estimate convergence rate for solving the interface equations
  numberOfInterfaceSolves=0;                // keeps track of how many iterface solves (for statistics)
  totalNumberOfInterfaceIterations=0;       // keeps track of how many iterations were used (for statistics)
}

InterfaceDescriptor::
~InterfaceDescriptor()
{
  delete interfaceTransfer;
}
