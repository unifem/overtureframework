// ===================================================================================================
// This file contains class that are used with interfaces between domains for multi-domain problems.
// ===================================================================================================

#ifndef INTERFACE_H
#define INTERFACE_H

#include "Overture.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <vector>
#include <list>
#else
#include <vector.h>
#include <list.h>
#endif

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

// forward declaration
class Parameters;

// ------------------- forward declaration:
int
getInterfaceData( real tSource, int grid, int side, int axis, 
		  int interfaceDataOptions,
		  RealArray & data,
                  Parameters & parameters,
                  bool saveTimeHistory = false );

// ------------------- forward declaration:
Parameters & 
getInterfaceParameters( int grid, int side, int axis, Parameters & parameters);

// ==============================================================================
/// \brief This class holds past interface values
// ==============================================================================
class InterfaceData
{
public:
InterfaceData();
~InterfaceData();

  real t;         // time 
  RealArray u,f;  // for now save both the solution and RHS values on the interface
};


typedef std::vector<InterfaceData> InterfaceDataList ;

// ==============================================================================
/// \brief This class holds a circular list of past interface values 
// ==============================================================================
class InterfaceDataHistory
{
public:
InterfaceDataHistory();
~InterfaceDataHistory();

  int current;  // points to the latest entry in the interfaceDataList
  InterfaceDataList interfaceDataList;  // circular list
};



class GridFaceDescriptor
// ==========================================================================================
//  This class holds information about a particular grid face that belongs to an interface
// ==========================================================================================
{
 public:

GridFaceDescriptor(int domain_, int grid_, int side_, int axis_ );

~GridFaceDescriptor();

int domain,grid,side,axis;
int interfaceBC;
real a[3];       // holds the coefficients of the BC such as a[0]*u + a[1]*u.n = 
RealArray *u;

InterfaceDataHistory interfaceDataHistory;  // holds a history of interface values at past times
InterfaceDataHistory interfaceDataIterates;  // interface values at past iterates for the current time.

// the dbase can hold additional information for the interface
mutable DataBase dbase; 

};
  
typedef std::vector<GridFaceDescriptor> GridList;

class InterfaceTransfer;  // forward declaration

class InterfaceDescriptor
// ===========================================================================================
//  This class holds information about an interface such as the 
//  the lists of grid faces that are adjacent to a particular interface.
//
// /gridList1: list of GridFaceDescriptor's for grid faces adjacent to side 1 of this interface
// /gridList2: list of GridFaceDescriptor's for grid faces adjacent to side 2 of this interface
// ===========================================================================================
{
public:

InterfaceDescriptor();
~InterfaceDescriptor();

int domain1, domain2;                    // domain identifiers for side1 and side2 of the interface
GridList gridListSide1, gridListSide2;   // lists of GridFaceDescriptor for the two sides
InterfaceTransfer *interfaceTransfer;    // This object knows how to transfer info across an interface

real interfaceTolerance;                 // tolerance for satisfying the interface equations
real interfaceOmega;                     // relaxation parameter for solving the interface equations
real estimatedConvergenceRate;           // estimate convergence rate for solving the interface equations

int maximumNumberOfIntefaceIterations;   // max iterations allowed when solving the interface equations

int numberOfInterfaceSolves;             // keeps track of how many iterface solves (for statistics)

int totalNumberOfInterfaceIterations;    // keeps track of how many iterations were used (for statistics)
};
  


typedef std::vector<InterfaceDescriptor> InterfaceList;


// // ==========================================================================================
// /// /brief This class holds information about interface boundary conditions on a single face.
// // ==========================================================================================
// class InterfaceBoundaryConditionInfo
// {
//  public:

// InterfaceBoundaryConditionInfo(int domain, int grid, int side, int axis ) 
//   : face(domain,grid,side,axis)
//     { 
//       component=-1;
//       interfaceBC=-1;
//       u=NULL;
//       a[0]=1.; a[1]=0; a[2]=0.;
//     };

//   GridFaceDescriptor face;
//   int component;
//   int interfaceBC;
//   real a[3];       // hold coefficients of BC such as a[0]*u + a[1]*u.n = 
//   RealArray *u;
// };



#endif
