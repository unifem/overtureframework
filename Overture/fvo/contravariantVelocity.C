#include "MappedGridFiniteVolumeOperators.h"
// =================================================================================
//\begin{>contravariantVelocity.tex}{\subsection{normalVelocity}}   
REALMappedGridFunction MappedGridFiniteVolumeOperators::
normalVelocity (const REALMappedGridFunction &u,                        
		const Index & I1,		// = nullIndex                           
		const Index & I2,		// = nullIndex
		const Index & I3,               // = nullIndex
		const Index & I4,               // = nullIndex
		const Index & I5,               // = nullIndex
		const Index & I6,               // = nullIndex
		const Index & I7,               // = nullIndex
		const Index & I8)  		// = nullIndex
//
// /Purpose:
//	Compute face-area-weighted normal velocity from either cell-centered or
//	face-centered input data
//
//   /u (input):	 velocity either cell or face centered
//   /I1,I2,I3 (input):  Index'es where values are to be returned
//   /I4,...,I8: ignored
//
//   /normalVelocity (output):   velocities with GridFunctionType
//                               faceCenteredAll. There are currently no 
//                               other centerings available.
//
//
// /Author:		D.L.Brown
// /Date Documentation Last Modified:	951030
//\end{contravariantVelocity.tex}  
//========================================
{
  return (contravariantVelocity (u, I1, I2, I3, I4, I5, I6, I7, I8));
}

// =================================================================================
//\begin{>>contravariantVelocity.tex}{\subsection{contravariantVelocity}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
contravariantVelocity (	const REALMappedGridFunction &u,                        
			const Index & I1,		// = nullIndex                           
			const Index & I2,		// = nullIndex
			const Index & I3,               // = nullIndex
		       const Index & I4,               // = nullIndex
		       const Index & I5,               // = nullIndex
		       const Index & I6,               // = nullIndex
		       const Index & I7,               // = nullIndex
		       const Index & I8)  		// = nullIndex
//
// /Purpose:
//   This routine is an alias for {\bf normalVelocity} retained 
//   for backward compatibility of the class. (see {\bf normalVelocity}).  
//
//\end{contravariantVelocity.tex} 
//========================================
{

  standardOrderingErrorMessage(u, "contravariantVelocity");
  if (debug) contravariantVelocityDisplay.interactivelySetInteractiveDisplay ("contravariantVelocityDisplay initialization");

	// ========================================
	// We expect the input to be of the form u(Dim,Dim,Dim,Range(0,numberOfDimensions-1)) or
	//				         u(Dim,Dim,Dim,Range(0,numberOfDimensions-1),faceRange)
	// so check this.
	// ========================================

  if (debug) contravariantVelocityDisplay.display (u, "contravariantVelocity: input u");

  if (debug) cout << "contravariantVelocity: useCMPGRDGeometryArrays = " << useCMPGRDGeometryArrays << endl;

 GridFunctionParameters::GridFunctionTypeWithComponents gfType = u.getGridFunctionTypeWithComponents(); 

  if (gfType != GridFunctionParameters::cellCenteredWith1Component &&
      gfType != GridFunctionParameters::faceCenteredAllWith1Component)
  {
    cout << "contravariantVelocity: gridfunction type is " << gfType << endl;
    throw "contravariantVelocity: invalid input gridfunction type";
  }
  

/*

//  if (u.getIsCellCentered()){  
   if (getIsCellCentered(u)){
  
    // all components cellCentered

    for (int i=1; i<5; i++){
      if (u.getComponentDimension(i) > 1){
	printf ("contravariantVelocity: ERROR:  cellCentered but too many indices\n");
	exit (-1);
      }

    } // number of indices OK, now check number of components

    if (u.getComponentDimension(0) != numberOfDimensions){
      printf ("contravariantVelocity: ERROR: wrong number of components\n");
      exit (-1);

    } // number of components OK

  } else { // not cellCentered, so it better be faceCentered of type all

    if (u.getFaceCentering() != REALMappedGridFunction::all){
      printf ("faceCentering: ERROR: input array not properly faceCentered\n");
      exit (-1);

    } // properly faceCentered

    }
*/    


	// ========================================
  	// Initialize the contravariant velocities 
	// ========================================

  REALMappedGridFunction uBar(mappedGrid, GridFunctionParameters::faceCenteredAll);

  uBar = 0.;

//    if (u.getIsCellCentered()){
//   if (getIsCellCentered(u)){

    if (gfType == GridFunctionParameters::cellCenteredWith1Component){ 

    Index R1,R2,R3;

	// ========================================
  	// loop over all faces and compute the corresponding contravariant velocity
	// ========================================

    int face;
    ForAxes (face)
    {
	// ========================================
	// It's cell centered and there are enough components, so determine R1,R2,R3,
	// which are the ranges in which values are to be returned. Either given or
	// one less than the dimension of the input array in the face direction.
	// ========================================

      int c0=face,c1=0,c2=0,c3=0,c4=0;
      int extra1 = -inc(face,rAxis); int extra2 = -inc(face,sAxis); int extra3 = - inc(face,tAxis);
      getDefaultIndex (uBar, c0, c1, c2, c3, c4,  R1, R2, R3, extra1, extra2, extra3, I1, I2, I3);

	// ========================================
    	// uBar = rx*u + ry*v + rz*w
	// ========================================

      int component;
      ForAxes (component){
	c0 = component;
	REALMappedGridFunction uAverage = average(u,face,c0,c1,c2,c3,c4);
	if (useCMPGRDGeometryArrays) {
	  uBar(R1,R2,R3,face) += faceNormalCG(R1,R2,R3,component,face) * uAverage(R1,R2,R3);
	} else {
	  uBar(R1,R2,R3,face) += faceNormal(R1,R2,R3,face,component) * uAverage(R1,R2,R3);
	}
	if (debug) 
	{
	  contravariantVelocityDisplay.display (uAverage, "contravariantVelocity: this is uAverage: ");
	}
      }
      if (debug)
      {
	contravariantVelocityDisplay.display (u       , "contravariantVelocity: this is u (input): ");
	contravariantVelocityDisplay.display (uBar    , "contravariantVelocity: this is uBar:");
      }
    }

    uBar.periodicUpdate();

    GridFunctionParameters::GridFunctionTypeWithComponents returnedType = uBar.getGridFunctionTypeWithComponents();
 
    return (uBar);
  }
  

  if (gfType == GridFunctionParameters::faceCenteredAllWith1Component)
  {

    Index R1,R2,R3;

	// ========================================
  	// Compute the contravariant velocities at appropriate cell edges
	// ========================================

    int face;
    ForAxes (face)
    {
      int extra[3] = {0,0,0};
      int c0=face, c1=0, c2=0, c3=0, c4=0;
      getDefaultIndex (uBar, c0, c1, c2, c3, c4, R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

	// ========================================
    	// uBar = rx*u + ry*v + rz*w
	// ========================================

      int direction;
      ForAxes (direction)
	if (useCMPGRDGeometryArrays){
	  uBar(R1,R2,R3,face) += faceNormalCG(R1,R2,R3,direction,face) * u(R1,R2,R3,direction,face);
	} else {
	  uBar(R1,R2,R3,face) += faceNormal(R1,R2,R3,face,direction) * u(R1,R2,R3,direction,face);
	}
    }

    uBar.periodicUpdate();

    GridFunctionParameters::GridFunctionTypeWithComponents returnedType = uBar.getGridFunctionTypeWithComponents();

    return (uBar);
  }

  return uBar;

}

