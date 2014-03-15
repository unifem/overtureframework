#include "MappedGridFiniteVolumeOperators.h"
// ==============================================================================
//\begin{>convectiveDerivative.tex}{\subsection{convectiveDerivative $(u \cdot \nabla) u$}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
convectiveDerivative (const REALMappedGridFunction &u,
		      const Index & I1,  //=nullIndex
		      const Index & I2,  //=nullIndex
		      const Index & I3,  //=nullIndex
		      const Index & I4,  //=nullIndex
		      const Index & I5,  //=nullIndex
		      const Index & I6,  //=nullIndex
		      const Index & I7,  //=nullIndex
		      const Index & I8)  //=nullIndex

//
// /Purpose:
//   approximate the convective velocity derivative with a centered formula
//
//   /u(mg,cellCentered,numberOfDimensions) (input):    velocity function
//   /I1, I2, I3 (input/optional):
//             if = nullIndex, compute values at all possible cells
//             otherwise, compute only in cells specified by these
//             Index'es
//   /I4 through I8:
//             these Index'es are ignored
//
//   /convectiveDerivative: an approximation of the convective velocity derivative
//             for all possible cells.
// 
// /3D: yes
// /Author:				D.L.Brown
//\end{convectiveDerivative.tex} 
//========================================
{
  if (debug) cdDisplay.interactivelySetInteractiveDisplay ("cdDisplay initialization");
  REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGrid (mappedGrid, all, all, all, numberOfDimensions);
  returnedValue = 0.;

  const REAL HALF = 0.5;  
  Index R1,R2,R3;
  
  if (! u.getIsCellCentered())
  {
    throw "MappedGridFiniteVolumeOperators::convectiveDerivative: not implemented for non-cellCentered input";
  }
  
  int extra[3] = {-1,-1,-1};
  int c[5]     = {0,0,0,0,0};
  getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

  REALMappedGridFunction uBar;
  uBar.updateToMatchGridFunction (u);
  uBar = 0.;
  
      // compute contravariant velocity at cell centers

  int face,direction;
  ForAxes (face) // r,s,t
  {
    ForAxes (direction)    // x,y,z
    {
      uBar(R1,R2,R3,face) += mappedGrid.centerNormal()(R1,R2,R3,direction,face)*u(R1,R2,R3,direction);
    }
  }
  
      // multiply uBar by Dzero(u) and sum to get components of convectiveDerivative

  ForAxes (direction) // x,y,z
  {
    ForAxes (face) // r,s,t
    {
      returnedValue(R1,R2,R3,direction) += 
	HALF*uBar(R1,R2,R3,face)*(u(R1+inc(face,rAxis),R2+inc(face,sAxis),R3+inc(face,tAxis),direction) -
				  u(R1-inc(face,rAxis),R2-inc(face,sAxis),R3-inc(face,tAxis),direction));
    }
  }

  if (!isVolumeScaled) 
  {
    ForAxes (direction)
    {
      returnedValue(R1,R2,R3,direction) /= mappedGrid.cellVolume()(R1,R2,R3);
    }
  }
  
  returnedValue.periodicUpdate ();
  
  return (returnedValue);
}

// ==============================================================================
//\begin{>>convectiveDerivative.tex}{\subsection{convectiveDerivative $(u \cdot \nabla) w$}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
convectiveDerivative (const REALMappedGridFunction &w,
		      const REALMappedGridFunction &u,
		      const Index & I1,  // =nullIndex
		      const Index & I2,  // =nullIndex
		      const Index & I3               // = nullIndex
		      )


//
// /Purpose:
//   approximate the convective  derivative of passive variable(s) with a centered formula
//
//   /u(mg,cellCentered,numberOfDimensions) (input):    convective velocity 
//   /w(mg,cellCentered,numberOfComponents) (input):    passive variables to convect
//   /I1, I2, I3 (input/optional):
//             if = nullIndex, compute values at all possible cells
//             otherwise, compute only in cells specified by these
//             Index'es
//
//   /convectiveDerivative: an approximation of the convective velocity derivative
//             for all possible cells.
// 
// /3D: yes
// /Author:				D.L.Brown
// /961211: Major syntax change: reversed order of u and w to be consistent with 
//          the way MappedGridFunction class is implemented.
//\end{convectiveDerivative.tex} 
//========================================
{
  if (debug) cdDisplay.interactivelySetInteractiveDisplay ("cdDisplay initialization");

  int numberOfConvectedComponents = w.getComponentDimension(0);
  
  REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGrid (mappedGrid, all, all, all, numberOfConvectedComponents);
  returnedValue = 0.;

  const REAL HALF = 0.5;  
  Index R1,R2,R3;
  
  if ((! u.getIsCellCentered()) || (!w.getIsCellCentered()))
  {
    throw "MappedGridFiniteVolumeOperators::convectiveDerivative: not implemented for non-cellCentered input";
  }
  
  int extra[3] = {-1,-1,-1};
  int c[5]     = {0,0,0,0,0};
  getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

  REALMappedGridFunction uBar;
  uBar.updateToMatchGridFunction (u);
  uBar = 0.;
  
      // compute contravariant velocity at cell centers

  int face,direction;
  ForAxes (face) // r,s,t
  {
    ForAxes (direction)    // x,y,z
    {
      uBar(R1,R2,R3,face) += mappedGrid.centerNormal()(R1,R2,R3,direction,face)*u(R1,R2,R3,direction);
    }
  }

  if (debug) cdDisplay.display (uBar, "convectiveDerivative: contravariant velocity");
  
      // multiply uBar by Dzero(w) and sum to get components of convectiveDerivative

  int component;
  for (component=0; component < numberOfConvectedComponents; component++)
  {
    ForAxes (face) // r,s,t
    {
      returnedValue(R1,R2,R3,component) += 
	HALF*uBar(R1,R2,R3,face)*(w(R1+inc(face,rAxis),R2+inc(face,sAxis),R3+inc(face,tAxis),component) -
				  w(R1-inc(face,rAxis),R2-inc(face,sAxis),R3-inc(face,tAxis),component));
    }
  }

  if (debug) cdDisplay.display (returnedValue, "convectiveDerivative: returnedValue before volume scaling");

  if (!isVolumeScaled) 
  {
    for (component=0; component < numberOfConvectedComponents; component++)
    {
      returnedValue(R1,R2,R3,component) /= mappedGrid.cellVolume()(R1,R2,R3);
    }
  }
  
  returnedValue.periodicUpdate ();
 
  return (returnedValue);
}
