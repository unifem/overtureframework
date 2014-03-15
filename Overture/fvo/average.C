#include "MappedGridFiniteVolumeOperators.h"
// =================================================================================
//\begin{>average.tex}{\subsection{average}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
average (const REALMappedGridFunction &u,
	const int axis,		// = allAxes
	const int c0,		// = 0
	const int c1,		// = 0
	const int c2,		// = 0
	const int c3,		// = 0
	const int c4,		// = 0
	const Index & I1,	// = nullIndex
	const Index & I2, 	// = nullIndex
	const Index & I3)	// = nullIndex


//
//
// /Purpose:
// This routine takes a component of a REALMappedGridFunction, and averages  
// it in the "axis" direction. The returned REALMappedGridFunction
// has all the same characteristics as the input function, except that
// the centering changes from "cell" to "vertex" or vice versa along
// the "axis" direction.
//
// /u (input):		REALMappedGridFunction to average
// /c0,c1,... (input): 	component of u to average
// /I1,I2,I3 (input):	Index'es at which average is to be returned;
//			if == nullIndex, return at max possible places
//			as determined by the grid
//
// /average (output):	 average in a REALMappedGridFunction
//
// /3D: yes
// /Author:		D.L.Brown
// /Date Documentation Last Modified:	951030
//\end{average.tex} 
// ========================================
{
  
  const REAL HALF = 0.5;

  // initialize storage for returnedValue; 

	// ========================================
	// make sure the input array is in a form we can deal with
	// ========================================

  standardOrderingErrorMessage (u, "average");

  if (axis == allAxes) {
    throw "Averaging over all axes not yet implemented: fatal error \n";
  }

  if (debug) averageDisplay.interactivelySetInteractiveDisplay ("averageDisplay initialization");

  REALMappedGridFunction uComponent;
  uComponent.link(u, Range(c0,c0), Range(c1,c1), Range(c2,c2), Range(c3,c3), Range(c4,c4));
    REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGridFunction (uComponent);
  returnedValue = 0.;

  if (debug) averageDisplay.display (returnedValue, "average: initialized returnedValue array");

  // figure out for what Indexes values are to be returned: either use the input 
  // Indexes I1,I2,I3, or use the dimension array for the grid
  // then compute the averages; cell values are averaged backwards, vertex values averaged forwards for
  // nice indices

  Index R1,R2,R3;
  int extra[3] = {0,0,0};


	//========================================
	// Set the cell-centering of the returnedValue according to the
	// direction of averaging 
	// ========================================

    if(u.getIsCellCentered ( axis, c0, c1, c2, c3, c4))
    {
      returnedValue.setIsCellCentered (FALSE, axis);
      extra[axis] = -1;

    } else {

      returnedValue.setIsCellCentered (TRUE, axis);
      extra[axis] = 0;
    }
	// ========================================
	// Get the indexes where values are to be returned
	// ========================================

    int c[5] = {0,0,0,0,0};
    getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

	// ========================================
	// computed the averages
	// ========================================

    int i1,i2,i3;
    i1 = inc(axis,rAxis);
    i2 = inc(axis,sAxis);
    i3 = inc(axis,tAxis);
    if(returnedValue.getIsCellCentered ( axis))
    {
      returnedValue(R1,R2,R3,0) = 
	HALF*(u(R1,R2,R3,c0,c1,c2,c3,c4) + u(R1+i1,R2+i2,R3+i3,c0,c1,c2,c3,c4));
    } else {
      returnedValue(R1,R2,R3,0) = 
	HALF*(u(R1,R2,R3,c0,c1,c2,c3,c4) + u(R1-i1,R2-i2,R3-i3,c0,c1,c2,c3,c4));
    }

	// ========================================
	// Periodic update
	// ========================================

  if (debug) averageDisplay.display (returnedValue, "average: returnedValue from average routine before periodicUpdate:");

  returnedValue.periodicUpdate();

  if (debug)
  {
    averageDisplay.display (u,		   "average: input to average routine:");
    averageDisplay.display (returnedValue, "average: returnedValue from average routine:");
  }

  return (returnedValue);

}

// =================================================================================
//\begin{>>average.tex}{\subsection{faceAverage}}  
REALMappedGridFunction MappedGridFiniteVolumeOperators::
faceAverage (
	const REALMappedGridFunction &u,
	const int axis1,
	const int axis2,
	const int c0,		// = 0
	const int c1,		// = 0
	const int c2,		// = 0
	const int c3,		// = 0
	const int c4,		// = 0
	const Index & I1,	// = nullIndex
	const Index & I2, 	// = nullIndex
	const Index & I3)	// = nullIndex
//
// Purpose:
//   This routine is no longer supported (951109)
//\end{average.tex}
// 
// Purpose:
// This routine takes a REALMappedGridFunction, and averages each 
// component in both the axis1 and axis2 directions. The returned REALMappedGridFunction
// has all the same characteristics as the input function, except that
// the centering changes from "cell" to "vertex" or vice versa along
// the axis1 and axis2 directions. So in 2D this can be used e.g. to get cell-centered
// data from vertex-centered data, and in 3D this can be used to get face-centered
// date from vertex-centered data.
//
//   LIMITATION: 950425: for the  moment, this just works for taking vertex-
//	centered data and producing face-centered averages in 3D. Need to write
//      the rest of the routine sometime
//
//   NEW VERSION 950602: changed the way component is specified; returns a scalar gridFunction
//   951030: This function will no longer be supported.
//
// Interface: (inputs)
//
// Interface: (output)
//
// Status and Warnings:
//  There are no known bugs, nor will there ever be.
//  It is assumed that the Coordinate dimensions come first
// 
// ========================================
{
  
  const REAL QUARTER = 0.25;

  int i;
  for (i=0; i<3; i++){
    if (u.positionOfComponent(i) != i){
      printf ("faceAverage: ERROR, only works if the Coordinate dimensions come first \n");
      exit (-1);
    }
  }

  // initialize storage for returnedValue; 

  REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGridFunction (u, all, all, all);
  returnedValue = 0.;

  // figure out for what Indexes values are to be returned: either use the input 
  // Indexes I1,I2,I3, or use the dimension array for the grid
  // then compute the averages; cell values are averaged backwards, vertex values averaged forwards for
  // nice indices

  Index R1,R2,R3;
  int extra[3] = {0,0,0};


	//========================================
	// Set the cell-centering of the returnedValue according to the
	// direction of averaging 
	// ========================================

    int axis[2]; axis[0] = axis1; axis[1] = axis2;

    for (i=0; i<2; i++)
    {
      if(u.getIsCellCentered ( axis[i], c0, c1, c2, c3, c4))
      {
  //      returnedValue.setIsCellCentered (FALSE, axis[i], component);
  //      extra[i] = -1;
	  cout << "average: not implemented for cell-centered data yet\n" << endl;
	  exit (-1);

      } else {

	returnedValue.setIsCellCentered (TRUE, axis[i]);
      }
    }
	// ========================================
	// Get the indexes where values are to be returned
	// ========================================

    getDefaultIndex (returnedValue, 0, 0, 0, 0, 0, R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

	// ========================================
	// computed the averages
	// ========================================


      returnedValue(R1,R2,R3) = 
	QUARTER*(u(R1                 ,R2,                 R3,                 c0,c1,c2,c3,c4) 
	       + u(R1+inc(axis1,rAxis),R2+inc(axis1,sAxis),R3+inc(axis1,tAxis),c0,c1,c2,c3,c4)
	       + u(R1+inc(axis2,rAxis),R2+inc(axis2,sAxis),R3+inc(axis2,tAxis),c0,c1,c2,c3,c4)
	       + u(R1+inc(axis1,rAxis)+inc(axis2,rAxis),
		   R2+inc(axis1,sAxis)+inc(axis2,sAxis),
		   R3+inc(axis1,tAxis)+inc(axis2,tAxis), c0,c1,c2,c3,c4));

  returnedValue.periodicUpdate();

  return (returnedValue);

}

