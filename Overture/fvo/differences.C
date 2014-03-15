#include "MappedGridFiniteVolumeOperators.h"

// =================================================================================
//\begin{>differences.tex}{\subsection{difference}}  
REALMappedGridFunction MappedGridFiniteVolumeOperators::
difference (const REALMappedGridFunction &u,
	    const int axis,
	    const int c0,		// = 0
	    const int c1,		// = 0
	    const int c2,		// = 0
	    const int c3,		// = 0
	    const int c4,		// = 0
	    const Index & I1,	// = nullIndex
	    const Index & I2, 	// = nullIndex
	    const Index & I3)	// = nullIndex

//
// /Purpose:
// This routine takes a component of a REALMappedGridFunction, and differences  
// it in the "axis" direction (with an undivided difference). 
// The returned REALMappedGridFunction
// has all the same characteristics as the input function, except that
// the centering changes from "cell" to "vertex" or vice versa along
// the "axis" direction. This is always a "backward" difference for
// cellCentered input data, and a "forward" difference for vertexCentered
// input data.
//
// /u (input):                 the input grid function
// /axis (input):              difference in this direction
// /c0,c1,c2,c3,c4 (input):    difference this component
// /I1,I2,I3 (input):          (optional) return values for these Index'es.
// /difference (output):       the undivided difference centered as described above.
//
// /3D: yes
// /Author:		D.L.Brown
// /Date Documentation Last Modified:	951031
//\end{differences.tex} 
// ========================================
{
  
  standardOrderingErrorMessage (u, "difference");
  if (debug) differenceDisplay.interactivelySetInteractiveDisplay ("differenceDisplay initialization");

  // initialize storage for returnedValue; 

  REALMappedGridFunction uComponent;
  uComponent.link (u, Range(c0,c0), Range(c1,c1), Range(c2,c2), Range(c3,c3), Range(c4,c4));
    REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGridFunction (uComponent);
  returnedValue = 0.;

  if (debug) differenceDisplay.display (returnedValue, "difference: initialized array");

  Index R1,R2,R3;
  int extra[3] = {0,0,0};

	// ========================================
	// set the centering of returnedValue based on axis of differencing
	// ========================================

    if(u.getIsCellCentered ( axis, c0,c1,c2,c3,c4))
    {
      returnedValue.setIsCellCentered (FALSE, axis);
      extra[axis] = -1;

    } else {
      returnedValue.setIsCellCentered (TRUE, axis);
      extra[axis] = 0;
    }
	// ========================================
	// get the Index'es in which values are to be returned
	// ========================================

    int c[5] = {0,0,0,0,0};
    getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

	// ========================================
	// compute the differences
	// ========================================

    if(u.getIsCellCentered ( axis, c0, c1, c2, c3, c4))
    {
      returnedValue(R1,R2,R3) = 
	u(R1,R2,R3,c0,c1,c2,c3,c4) - u(R1-inc(axis,rAxis),R2-inc(axis,sAxis),R3-inc(axis,tAxis),c0,c1,c2,c3,c4);
    } else {
      returnedValue(R1,R2,R3) = 
 	u(R1+inc(axis,rAxis),R2+inc(axis,sAxis),R3+inc(axis,tAxis),c0,c1,c2,c3,c4) - u(R1,R2,R3,c0,c1,c2,c3,c4);
    }

  returnedValue.periodicUpdate();

  if (debug) differenceDisplay.display( returnedValue, "difference: returnedValue");
  return (returnedValue);
}
// =================================================================================
//\begin{>>differences.tex}{\subsection{dZero}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
dZero ( const REALMappedGridFunction &u,
	const int axis,
	const int c0,		// = 0
	const int c1,		// = 0
	const int c2,		// = 0
	const int c3,		// = 0
	const int c4,		// = 0
	const Index & I1,	// = nullIndex
	const Index & I2, 	// = nullIndex
	const Index & I3)	// = nullIndex

//
// /Purpose:
// This routine takes a REALMappedGridFunction, and differences a particular 
// component in the "axis" direction with an undivided central difference. 
// The returned REALMappedGridFunction
// has all the same characteristics as the specified component of the input function.
// Results are returned either according to the input values of I1,I2,I3,
// or if these Index'es = nullIndex, values will be returned at all cells,
// edges or vertices that are dimensioned minus one row of ghost cells.
//
// /u (input): the input GridFunction
// /axis: difference in this direction
// /I1, I2, I3 (input): (optional) Index'es describing where output is desired
// /c0,c1,c2,c3,c4 (input): which component to difference
//
// /dZero (output): the undivided central differences as described above.
//
// /3D: yes
// /Author:		D.L.Brown
// /Date Documentation Last Modified: 951031
//\end{differences.tex} 
// ========================================
{

  standardOrderingErrorMessage (u, "dZero");
  if (debug) dZeroDisplay.interactivelySetInteractiveDisplay ("dZeroDisplay initialization");

  const REAL HALF = 0.5;

	// ========================================
  	// initialize storage for returnedValue; 
	// ========================================

  REALMappedGridFunction uComponent;
  uComponent.link (u, Range(c0,c0), Range(c1,c1), Range(c2,c2), Range(c3,c3), Range(c4,c4));
  REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGridFunction (uComponent);
  returnedValue = 0.;

  if (debug) dZeroDisplay.display (returnedValue, "dZero: initialized array");

  Index R1,R2,R3;
  int extra[3];


	// ========================================
	// since this is a central difference, the cell-centering
	// of the returnedValue is the same as the input value, but
	// we have to reduce the Range in the axis direction
	// ========================================

    int i;
    ForAllAxes(i) extra[i] = i == axis ? -1 : 0;
    int c[5] = {0,0,0,0,0};
    getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

	// ========================================
	// Now compute central undivided differences
	// ========================================

    if(u.getIsCellCentered ( axis, c0, c1, c2, c3, c4))
    {
      returnedValue(R1,R2,R3) = HALF*(
	  u(R1+inc(axis,rAxis),R2+inc(axis,sAxis),R3+inc(axis,tAxis),c0,c1,c2,c3,c4) 
	- u(R1-inc(axis,rAxis),R2-inc(axis,sAxis),R3-inc(axis,tAxis),c0,c1,c2,c3,c4));
    } else {

      returnedValue(R1,R2,R3) = HALF*(
	  u(R1+inc(axis,rAxis),R2+inc(axis,sAxis),R3+inc(axis,tAxis),c0,c1,c2,c3,c4) 
	- u(R1-inc(axis,rAxis),R2-inc(axis,sAxis),R3-inc(axis,tAxis),c0,c1,c2,c3,c4));
    }

  returnedValue.periodicUpdate();

  if (debug) dZeroDisplay.display (returnedValue, "dZero: returnedValue");
  return (returnedValue);
}
	
