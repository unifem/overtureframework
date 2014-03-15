#include "MappedGridFiniteVolumeOperators.h"

//===============================================================================
//\begin{>vorticity.tex}{\subsection{vorticity}}  
REALMappedGridFunction MappedGridFiniteVolumeOperators::
vorticity (const REALMappedGridFunction &u,
	   const Index & I1,
	   const Index & I2,
	   const Index & I3,               // = nullIndex
		const Index & I4,               // = nullIndex
		const Index & I5,               // = nullIndex
		const Index & I6,               // = nullIndex
		const Index & I7,               // = nullIndex
		const Index & I8)
//
// /Purpose:
//   approximate the vorticity with a centered formula in 2D
//
//   /u (input):     velocity function
//   /I1, I2, I3 (input):             if = nullIndex, compute values at all possible cells
//                                otherwise, compute only in cells specified by these
//                                Index'es
//
//   /vorticity (output):         vorticity returned for cells specified by I1,I2,I3
//                                or for all possible cells.
//   /3D: no
//
// /Author:				D.L.Brown
//\end{vorticity.tex} 
//========================================
{
  if (debug) vorticityDisplay.interactivelySetInteractiveDisplay ("vorticityDisplay initialization");
  REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGrid (mappedGrid, all, all, all);
  returnedValue = 0.;

  const REAL HALF = 0.5;
  Index R1,R2,R3;
  
  if (! u.getIsCellCentered())
  {
    throw "MappedGridFiniteVolumeOperators::vorticity: not implemented for non-cellCentered input";
  }
  
  int extra[3] = {-1,-1,-1};
  int c[5]     = {0,0,0,0,0};
  getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

  
      // compute vorticity at cell centers

  if (numberOfDimensions == 2)
  {
    returnedValue(R1,R2,R3) = HALF/mappedGrid.cellVolume()(R1,R2,R3)*
      (rYCenter(R1,R2,R3) * (u(R1+1,R2  ,R3,uComponent) - u(R1-1,R2  ,R3,uComponent))
      -rXCenter(R1,R2,R3) * (u(R1+1,R2  ,R3,vComponent) - u(R1-1,R2  ,R3,vComponent))
      +sYCenter(R1,R2,R3) * (u(R1  ,R2+1,R3,uComponent) - u(R1  ,R2-1,R3,uComponent))
      -sXCenter(R1,R2,R3) * (u(R1  ,R2+1,R3,vComponent) - u(R1  ,R2-1,R3,vComponent)));
  }
  else
  {
    cout << "WARNING: MappedGridFiniteVolumeOperators::vorticity not implemented for numberOfDimensions = 3" << endl;
  }

/* ***970304: when no compositeGrid, no mask array; leave this up to the user
  where (mappedGrid.mask()(R1,R2,R3) <= 0)
  {
     returnedValue(R1,R2,R3) = 0.;
  }  
  */

  returnedValue.periodicUpdate ();
  
  return (returnedValue);
}
