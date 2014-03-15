/*  -*-Mode: c++; -*-  */
#ifndef ARRAY_GET_INDEX_H
#define ARRAY_GET_INDEX_H "arrayGetIndex.h"

#include "GenericDataBase.h"
#include "A++.h"
#include "wdhdefs.h"

//=================================================================================================
//  Define some useful functions for getting A++ indicies for Overture
//
//   Note: An "index Array" is an IntegerArray of dimensions (0:1,0:2) such as indexRange, gridIndexRange,
//         or dimension.
//
//   getIndex : determine the Indices corresponding to the index space defined by an index array 
//
//   getBoundaryIndex : determine the Indices corresponding to one of the boundaries of the
//              index space defined by an index arry
//
//   getGhostIndex: This returns the indicies corresponding to a ghost line
//
//=================================================================================================

// ----------------------------getIndex---------------------------------------------------

const int arrayGetIndexDefaultValue=-999999;

void 
getIndex(const IntegerArray & indexArray,   // this index array and determines I1,I2,I3
	 Index & I1,                    // output: Index for axis1
	 Index & I2,                    // output: Index for axis2
	 Index & I3,                    // output: Index for axis3
	 int extra1=0,          // increase Index's by this amount along axis1
	 int extra2=arrayGetIndexDefaultValue,         // by default extra2=extra1
	 int extra3=arrayGetIndexDefaultValue          // by default extra3=extra1
        );

// ----------------------------getBoundaryIndex---------------------------------------------------
//  These functions return Index's for a Boundary. The boundary is defined by the parameters
//  side=0,1 and axis=0,1,2.
//
//-----------------------------------------------------------------------------------------------
void 
getBoundaryIndex(const IntegerArray & indexArray,       // get Index's for boundary of an indexArray
		 int side, 
		 int axis, 
		 Index & Ib1, 
		 Index & Ib2, 
		 Index & Ib3, 
		 int extra1=0,
		 int extra2=arrayGetIndexDefaultValue,
		 int extra3=arrayGetIndexDefaultValue
		 );

//-----------------------------------------------------------------------------------------------
// These functions return the Index's for a Ghost-line. These are similar to getBoundaryIndex
// and in fact will give the same answer as getBoundaryIndex for the choice ghostLine=0.
// 
//  Input:
//    side,axis : get Index's for this side and axis
//    ghostLine : get Index's for this ghost-line, 
//                   ghostLine=1 : first ghost-line
//                   ghostLine=2 : second ghost-line
//                   ghostLine=0 : boundary
//                   ghostLine=-1: first line inside
//----------------------------------------------------------------------------------------------
void 
getGhostIndex(const IntegerArray & indexArray,          // get Index's for ghost line
	      int side, 
	      int axis,
	      Index & Ib1, 
	      Index & Ib2, 
	      Index & Ib3, 
	      int ghostLine=1, 
	      int extra1=0,
	      int extra2=arrayGetIndexDefaultValue,
	      int extra3=arrayGetIndexDefaultValue
	      );

#endif
