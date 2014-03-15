#include "MappedGridFiniteVolumeOperators.h"
// =================================================================================
//\begin{>gradient.tex}{\subsection{grad}}   
REALMappedGridFunction MappedGridFiniteVolumeOperators::
grad ( 
      const REALMappedGridFunction &phi,
      const GridFunctionParameters & gfParams,
      const Index & I1, // = nullIndex
      const Index & I2, // = nullIndex
      const Index & I3, // = nullIndex
      const Index & C,  // = nullIndex
      const Index & I5, // = nullIndex
      const Index & I6,	// = nullIndex
      const Index & I7,	// = nullIndex
      const Index & I8 )// = nullIndex
//
// /Purpose:
//   Compute gradient of a (scalar or vector) realMappedGridFunction,
//   returning a realMappedGridFunction of the same size plus additional
//   dimensions for the vector components
//
//   /phi:              realMappedGridFunction with any number of components; 
//                      however, only one component index is allowed
//   /gfParams:         gfParams.outputType specifies the output centering of the gradient
//                      gfParams.inputType must either not be set (==defaultCentering)
//                      or may be set to phi.getGridFunctionType().
//                      
//   /C:                if this is specified, then the gradient is only
//                      computed for components specified by C; otherwise
//                      the gradient of all components is computed.
//\end{gradient.tex}
//==============================================================================
{
  GridFunctionParameters::GridFunctionType outputType = gfParams.outputType;
  GridFunctionParameters::GridFunctionType inputType  = phi.getGridFunctionType();

  GridFunctionParameters gfParams0;

  Index J1,J2,J3;
//  getDefaultIndex (inputType, outputType, J1, J2, J3, I1, I2, I3);

  if (gfParams.inputType != inputType && 
      gfParams.inputType != GridFunctionParameters::defaultCentering)
  {
    cout << "MappedGridFiniteVolumeOperators::grad: ERROR: attempt to specify ";
    cout << "gfParams.inputType " << endl;
    throw "grad: ERROR";
  }
  // ... For convenience, define local copies of GridFunctionType's

//  const GridFunctionParameters::GridFunctionType general           = GridFunctionParameters::general; 
//  const GridFunctionParameters::GridFunctionType vertexCentered    = GridFunctionParameters::vertexCentered; 
  const GridFunctionParameters::GridFunctionType cellCentered      = GridFunctionParameters::cellCentered; 
  const GridFunctionParameters::GridFunctionType faceCenteredAll   = GridFunctionParameters::faceCenteredAll; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis1 = GridFunctionParameters::faceCenteredAxis1; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis2 = GridFunctionParameters::faceCenteredAxis2; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis3 = GridFunctionParameters::faceCenteredAxis3; 
  
  realMappedGridFunction returnedValue;
  realMappedGridFunction xDerivs, yDerivs, zDerivs;
  realMappedGridFunction xDerivsFace1, xDerivsFace2, xDerivsFace3;
  realMappedGridFunction yDerivsFace1, yDerivsFace2, yDerivsFace3;
  realMappedGridFunction zDerivsFace1, zDerivsFace2, zDerivsFace3;
    
  int numberOfInputComponents = phi.getComponentDimension(0);
  int nD = numberOfDimensions;

  Index nDC = Index(0,numberOfDimensions);
  Index inputComponents  = Range(phi.getComponentBase(0), phi.getComponentBound(0));
  Index outputComponents = C.length()==0 ? inputComponents : C;

  //... dimension returnedValue according to outputType; then call x,y,z

  switch (outputType)
  {
  case cellCentered:
  case faceCenteredAxis1:
  case faceCenteredAxis2:
  case faceCenteredAxis3:

    getDefaultIndex (inputType, outputType, J1, J2, J3, I1, I2, I3);    

    if (numberOfInputComponents == 1)
    {
      returnedValue.updateToMatchGrid (mappedGrid, outputType, nDC);
      xDerivs.link (returnedValue, Range(xComponent,xComponent));
      xDerivs(J1,J2,J3) = x(phi,gfParams,J1,J2,J3)(J1,J2,J3);

      if (numberOfDimensions>1) {
	yDerivs.link (returnedValue, Range(yComponent,yComponent));
	yDerivs(J1,J2,J3) = y(phi,gfParams,J1,J2,J3)(J1,J2,J3);
      }
	
      if (numberOfDimensions>2) {
	zDerivs.link (returnedValue, Range(zComponent,zComponent));
	zDerivs(J1,J2,J3) = z(phi,gfParams,J1,J2,J3)(J1,J2,J3);
      }
	
    }
    else
    {
      returnedValue.updateToMatchGrid (mappedGrid, outputType, inputComponents, nDC);
      xDerivs.link (returnedValue, inputComponents, Range(xComponent,xComponent));
      xDerivs(J1,J2,J3,outputComponents) = x(phi,gfParams,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
      
      if (numberOfDimensions>1) {
	yDerivs.link (returnedValue, inputComponents, Range(yComponent,yComponent));
	yDerivs(J1,J2,J3,outputComponents) = y(phi,gfParams,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
      }
      
      if (numberOfDimensions>2) {
	zDerivs.link (returnedValue, inputComponents, Range(zComponent,zComponent));
	zDerivs(J1,J2,J3,outputComponents) = z(phi,gfParams,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
      }
    }
    
    break;
    
  case faceCenteredAll:
      //... this case will require looping over each component individually

    if (numberOfInputComponents == 1)
    {

      returnedValue.updateToMatchGrid (mappedGrid, faceCenteredAll, nDC);

      xDerivsFace1.link (returnedValue, Range(xComponent,xComponent), Range(rAxis,rAxis));

      gfParams0.outputType = faceCenteredAxis1;
      getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
      xDerivsFace1(J1,J2,J3) = x(phi,gfParams0,J1,J2,J3)(J1,J2,J3);


      if (nD>1) {
	xDerivsFace2.link (returnedValue, Range(xComponent,xComponent), Range(sAxis,sAxis));
	yDerivsFace1.link (returnedValue, Range(yComponent,yComponent), Range(rAxis,rAxis));
	yDerivsFace2.link (returnedValue, Range(yComponent,yComponent), Range(sAxis,sAxis));

	gfParams0.outputType = faceCenteredAxis1;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	yDerivsFace1(J1,J2,J3) = y(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
	gfParams0.outputType = faceCenteredAxis2;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	xDerivsFace2(J1,J2,J3) = x(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
	yDerivsFace2(J1,J2,J3) = y(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
      }
      if (nD>2) 
      {
	xDerivsFace3.link (returnedValue, Range(xComponent,xComponent), Range(tAxis,tAxis));
	yDerivsFace3.link (returnedValue, Range(yComponent,yComponent), Range(tAxis,tAxis));
	zDerivsFace1.link (returnedValue, Range(zComponent,zComponent), Range(rAxis,rAxis));
	zDerivsFace2.link (returnedValue, Range(zComponent,zComponent), Range(sAxis,sAxis));
	zDerivsFace3.link (returnedValue, Range(zComponent,zComponent), Range(tAxis,tAxis));

	gfParams0.outputType = faceCenteredAxis1;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	zDerivsFace1(J1,J2,J3) = z(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
	gfParams0.outputType = faceCenteredAxis2;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	zDerivsFace2(J1,J2,J3) = z(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
	gfParams0.outputType = faceCenteredAxis3;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	xDerivsFace3(J1,J2,J3) = x(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
	yDerivsFace3(J1,J2,J3) = y(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
	zDerivsFace3(J1,J2,J3) = z(phi,gfParams0,J1,J2,J3)(J1,J2,J3);
      }


    }
    else // ...multiple input componenents case
    {
      returnedValue.updateToMatchGrid (mappedGrid, outputType, inputComponents, nDC);
      xDerivsFace1.link (returnedValue, inputComponents, Range(xComponent,xComponent), Range(rAxis,rAxis));

      gfParams0.outputType = faceCenteredAxis1;
      getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
      xDerivsFace1(J1,J2,J3,outputComponents)  = x(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);

      if (nD>1) {
	xDerivsFace2.link (returnedValue, inputComponents, Range(xComponent,xComponent), Range(sAxis,sAxis));
	yDerivsFace1.link (returnedValue, inputComponents, Range(yComponent,yComponent), Range(rAxis,rAxis));
	yDerivsFace2.link (returnedValue, inputComponents, Range(yComponent,yComponent), Range(sAxis,sAxis));

	gfParams0.outputType = faceCenteredAxis1;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	yDerivsFace1(J1,J2,J3,outputComponents) = y(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
	gfParams0.outputType = faceCenteredAxis2;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	xDerivsFace2(J1,J2,J3,outputComponents) = x(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
	yDerivsFace2(J1,J2,J3,outputComponents) = y(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
      }
      if (nD>2) 
      {
	xDerivsFace3.link (returnedValue, inputComponents, Range(xComponent,xComponent), Range(tAxis,tAxis));
	yDerivsFace3.link (returnedValue, inputComponents, Range(yComponent,yComponent), Range(tAxis,tAxis));
	zDerivsFace1.link (returnedValue, inputComponents, Range(zComponent,zComponent), Range(rAxis,rAxis));
	zDerivsFace2.link (returnedValue, inputComponents, Range(zComponent,zComponent), Range(sAxis,sAxis));
	zDerivsFace3.link (returnedValue, inputComponents, Range(zComponent,zComponent), Range(tAxis,tAxis));

	gfParams0.outputType = faceCenteredAxis1;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	zDerivsFace1(J1,J2,J3,outputComponents) = z(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
	gfParams0.outputType = faceCenteredAxis2;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	zDerivsFace2(J1,J2,J3,outputComponents) = z(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
	gfParams0.outputType = faceCenteredAxis3;
	getDefaultIndex (inputType, gfParams0.outputType, J1, J2, J3, I1, I2, I3);
	xDerivsFace3(J1,J2,J3,outputComponents) = x(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
	yDerivsFace3(J1,J2,J3,outputComponents) = y(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
	zDerivsFace3(J1,J2,J3,outputComponents) = z(phi,gfParams0,J1,J2,J3,outputComponents)(J1,J2,J3,outputComponents);
      }
    }
    break;
  }
  
  return (returnedValue);
}
 
// =================================================================================
//\begin{>>gradient.tex}{\subsection{grad}}   
REALMappedGridFunction MappedGridFiniteVolumeOperators::
grad ( 
      const REALMappedGridFunction &phi,
      const Index & C0,  // = nullIndex
      const Index & C1,  // = nullIndex
      const Index & C2,  // = nullIndex
      const Index & C3,  // = nullIndex
      const Index & C4,  // = nullIndex
      const Index & I1,		// = nullIndex
      const Index & I2,		// = nullIndex
      const Index & I3 )		// = nullIndex


// /Purpose:
//   Take an input cell-centered or face-centered array phi and compute a cell-centered
//   gradient from one of its components, returning a REALMappedGridFunction of appropriate
//   dimensions. 
//
// /phi:		        REALMappedGridFunction with any number of components,
// /c0,c1,c2,c3,c4:	component specification for input array;
//			if phi is faceCentered of type "all", then the component
//			corresponding to the faceCentering is ignored
// /I1,I2,I3:	Index ranges that will contain gradient approximation on return.
//                      If these are defaulted, then gradient approximation is returned
//                      for all possible cells
//
// /grad(all,all,all,Components,Range(0,numberOfDimensions-1)): returned gradient approximation
//
// /Author:		D.L.Brown
// /Date Documentation Last Modified:	951019
//
//\end{gradient.tex} 
// ========================================
{

  if (debug) gradDisplay.interactivelySetInteractiveDisplay ("gradDisplay initialization");

	// ========================================
	// Make sure the indices are in standard order
	// ========================================

  standardOrderingErrorMessage (phi, "grad");
	
	// ========================================
	// make sure phi is either cellCentered or 
	// appropriately faceCentered 
	// ========================================

  if (!phi.getIsCellCentered()){
    if (phi.getFaceCentering() != GridFunctionParameters::all){
      throw "gradient: ERROR: inappropriate input array\n";

    }
  }

  //...960217: This code can only deal with scalar components, so check those, and define c0,...,c4:

  if (C0.length()>1)
  {
    cout << "MappedGridFiniteVolumeOperators::grad: nonscalar components not supported" << endl;
    throw " ";
  }
  if (C1.length()>1)
  {
    cout << "MappedGridFiniteVolumeOperators::grad: nonscalar components not supported" << endl;
    throw " ";
  }  
  if (C2.length()>1)
  {
    cout << "MappedGridFiniteVolumeOperators::grad: nonscalar components not supported" << endl;
    throw " ";
  }  
  if (C3.length()>1)
  {
    cout << "MappedGridFiniteVolumeOperators::grad: nonscalar components not supported" << endl;
    throw " ";
  }
  if (C4.length()>1)
  {
    cout << "MappedGridFiniteVolumeOperators::grad: nonscalar components not supported" << endl;
    throw " ";
  }	

  int c0 = C0.length()==0 ? 0 : C0.getBase();
  int c1 = C1.length()==0 ? 0 : C1.getBase();
  int c2 = C2.length()==0 ? 0 : C2.getBase();
  int c3 = C3.length()==0 ? 0 : C3.getBase();
  int c4 = C4.length()==0 ? 0 : C4.getBase(); 
  
  // ========================================
  // make sure faceCentering array is there, otherwise create it
  // ========================================

  if (!centerNormalDefined) createCenterNormal (*(phi.mappedGrid));

  // ========================================
  // allocate space for returnedValue, temporary arrays
  // ========================================

  Range R[4];
  Index I[3];
  R[0] = all; R[1] = all; R[2] = all; 
  int c[5] = {0,0,0,0,0};

  R[3] = Range(0,numberOfDimensions-1);
    REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGridFunction (phi, R[0], R[1], R[2], R[3]);
  returnedValue = 0.;

  GridFunctionParameters::GridFunctionTypeWithComponents rvType = returnedValue.getGridFunctionTypeWithComponents(); 
//  cout << "grad: initial gfType for returned value is " << rvType << endl;
  

  R[3] = Range(0,numberOfDimensions-1);
  REALMappedGridFunction dPhi;
  dPhi.updateToMatchGridFunction(phi, R[0], R[1], R[2], R[3]);
  dPhi = 0.;

	// ========================================
	// If input is cell-centered, determined indices and call gradFromCellCenteredInput
	// ========================================

  if (phi.getIsCellCentered())

  {

    int extra[3] = {-1,-1,-1};
    getDefaultIndex (returnedValue, c[0],c[1],c[2],c[3],c[4], I[0], I[1], I[2], 
			extra[0], extra[1], extra[2], I1, I2, I3);

	// ========================================
	// Compute dPhi from cellCentered phi using dZero
	// ========================================

    int  rAxes, xAxes;
    ForAxes (rAxes)
    {  
      dPhi(I[0], I[1], I[2], rAxes) = dZero (phi, rAxes, c0, c1, c2, c3, c4, I[0], I[1], I[2])(I[0], I[1], I[2]);
    }

    if (debug) gradDisplay.display (dPhi, "gradient: dPhi");

	// ========================================
	// now, phi_x = r_x*phi_r + s_x*phi_s +t_x*phi_t, etc.
	// ========================================

    if (useCMPGRDGeometryArrays)
    {
      if (debug) cout << "XXXXX gradient: using CMPGRD geometry arrays" << endl;
      ForAxes(xAxes){
	ForAxes(rAxes){
	  returnedValue(I[0], I[1], I[2], xAxes) +=
	    centerNormalCG(I[0], I[1], I[2], xAxes, rAxes) * dPhi(I[0], I[1], I[2], rAxes);
	}

	  // ========================================
	  // divide by volume if not scaled
	  // ========================================

	if (!isVolumeScaled) returnedValue(I[0],I[1],I[2],xAxes) /= cellVolume(I[0],I[1],I[2]);
      }
      rvType = returnedValue.getGridFunctionTypeWithComponents();
      

    }else{

      ForAxes(xAxes){
	ForAxes(rAxes){
	  returnedValue(I[0], I[1], I[2], xAxes) +=
	    centerNormal(I[0], I[1], I[2], rAxes, xAxes) * dPhi(I[0], I[1], I[2], rAxes);
	}

	  // ========================================
	  // divide by volume if not scaled
	  // ========================================

	if (!isVolumeScaled) returnedValue(I[0],I[1],I[2],xAxes) /= cellVolume(I[0],I[1],I[2]);
      }
      rvType = returnedValue.getGridFunctionTypeWithComponents();
    }
    

    

    if (debug) gradDisplay.display (returnedValue, "gradient: returnedValue");

//    cout << " grad: returnedValue gridFunctionType before return is " << rvType << endl;
    
    returnedValue.periodicUpdate();
    return (returnedValue);


  } else {

	// ========================================
    	// PHI is not cellCentered  so it must be faceCentered
    	// of type all.
	// ========================================

    int extra[3] = {0,0,0};
    getDefaultIndex (returnedValue, c[0],c[1],c[2],c[3],c[4], I[0], I[1], I[2], 
			extra[0], extra[1], extra[2], I1, I2, I3);

    int pofc = phi.positionOfFaceCentering();
		
	// ========================================
	// Compute dPhi from faceCentered phi using difference
	// ========================================

    int  rAxes, xAxes;
    ForAxes (rAxes)
    {  
      c[0]=c0; c[1]=c1; c[2]=c2; c[3]=c3; c[4]=c4; c[pofc] = rAxes;
      dPhi(I[0], I[1], I[2], rAxes) = 
	    difference (phi, rAxes, c[0],c[1],c[2],c[3],c[4], I[0], I[1], I[2])(I[0], I[1], I[2]);
    }

	// ========================================
	// now, phi_x = r_x*phi_r + s_x*phi_s +t_x*phi_t, etc.
	// ========================================

    if (useCMPGRDGeometryArrays){
      ForAxes(xAxes){
	ForAxes(rAxes){
	  returnedValue(I[0], I[1], I[2], xAxes) +=
	    centerNormalCG(I[0], I[1], I[2], xAxes, rAxes) * dPhi(I[0], I[1], I[2], rAxes);
	}
      

	  // ========================================
	  // divide by volume if not scaled
	  // ========================================

	if (!isVolumeScaled) returnedValue(I[0], I[1], I[2], xAxes) /= cellVolume(I[0], I[1], I[2]);
      }
      rvType = returnedValue.getGridFunctionTypeWithComponents();
    }else{

      ForAxes(xAxes){
	ForAxes(rAxes){
	  returnedValue(I[0], I[1], I[2], xAxes) +=
	    centerNormal(I[0], I[1], I[2], rAxes, xAxes) * dPhi(I[0], I[1], I[2], rAxes);
	}
      

	  // ========================================
	  // divide by volume if not scaled
	  // ========================================

	if (!isVolumeScaled) returnedValue(I[0], I[1], I[2], xAxes) /= cellVolume(I[0], I[1], I[2]);
      }
      rvType = returnedValue.getGridFunctionTypeWithComponents();
    }

/* ***970304: when no compositeGrid, no mask array; this should be left up to the user
    where (mappedGrid.mask()(I[0],I[1],I[2]) <= 0) returnedValue(I[0],I[1],I[2]) = 0.;
    */

    returnedValue.periodicUpdate();
    rvType = returnedValue.getGridFunctionTypeWithComponents();
//    cout << " grad: returnedValue gridFunctionType before return is " << rvType << endl;
    return (returnedValue);

  } 
  // throw "MappedGridFiniteVolumeOperators:: grad: it should be impossible to reach this statement ";

}

// v7
// =================================================================================
//\begin{>>gradient.tex}{\subsection{FCgrad}}   
REALMappedGridFunction MappedGridFiniteVolumeOperators::
FCgrad (const REALMappedGridFunction &phi,
	const int c0,
	const int c1,
	const int c2,
	const int c3,
	const int c4,
	const Index & I1,
        const Index & I2,
	const Index & I3,
	const Index & I4,
	const Index & I5,
	const Index & I6, 
	const Index & I7, 
	const Index & I8   )
//
// /Purpose:
//  Compute Face-Centered Gradient from Cell-Centered input
//  This routine is provided for backward compatibility only.
//  use grad (const realMappedGridFunction \&, const GridFunctionParameters \&, ...)
//  instead.
//\end{gradient.tex}
//
// /phi:             REALMappedGridFunction with any number of components
// /c0,c1,c2,c3,c4:  which component we are taking the gradient of
// /I1,I2,I3:        Index ranges that will contain gradient approximation on return
//                      (actually, it doesn't make sense to specify these; best at
//                       the moment to allow the defaults of nullIndex)
//  /Comments:
//  Someday, an option to compute a faceCentered gradient from a 
//  faceCentered input array will be included
//
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	951019
//\end{gradient.tex}
  // =================================================================================

/*  v6 only
REALMappedGridFunction MappedGridFiniteVolumeOperators::
FCgrad (const REALMappedGridFunction &phi,
	const int c0,
	const int c1,
	const int c2,
	const int c3,
	const int c4,
	const Index & I1,
        const Index & I2,
	const Index & I3)
*/
{
/*
  cout << "REALMappedGridFunction MappedGridFiniteVolumeOperators:: FCgrad: not yet implemented " << endl;
  int localNumberOfComponents = numberOfDimensions*Components.length(); //this is probably wrong
  REALMappedGridFunction gradient;
  gradient.updateToMatchGridFunction (phi, all, all, all, localNumberOfComponents);
  gradient.setIsCellCentered (TRUE);
  gradient = 0.;
*/

  if (debug) FCgradDisplay.interactivelySetInteractiveDisplay ("FCgradDisplay initialization");
  
  standardOrderingErrorMessage (phi, "FCgrad");
  
  if (!phi.getIsCellCentered())
  {
    throw "FCgrad can't handle anything but cellCentered input array";
  }
  
      // ===========================================
      // allocate space for returnedValue
      // ===========================================

  REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGrid (mappedGrid, all, all, all, numberOfDimensions, faceRange);
  returnedValue.setFaceCentering (GridFunctionParameters::all);
  returnedValue = 0.;

  REALMappedGridFunction dPhi;
  dPhi.updateToMatchGrid (mappedGrid, all, all, all, faceRange);
  dPhi.setFaceCentering (GridFunctionParameters::all);
  dPhi = 0.;
  
  Index I[3];
  int extra[3] = { -1,-1,-1};
  REAL QUARTER = 0.25;
  
//  getDefaultIndex (returnedValue, c0, c1, c2, c3, c4, I[0], I[1], I[2], extra[0], extra[1], extra[2], I1, I2, I3);  


    
  
      // ==========================================
      // compute derivatives wrt r,s,t
      // ==========================================

  int face;
  ForAxes (face)
  {
    for (int i=0; i<3; i++) extra[i] = -inc(face,i);
    getDefaultIndex (dPhi, face, 0, 0, 0, 0, I[0], I[1], I[2], extra[0], extra[1], extra[2], I1, I2, I3);
    dPhi(I[0], I[1], I[2], face) = difference (phi, face, c0, c1, c2, c3, c4, I[0], I[1], I[2])(I[0], I[1], I[2]);
  }

  if (debug) FCgradDisplay.display (dPhi, "FCgrad: dPhi");
    
      // ==========================================
      // compute gradient components
      // ==========================================

  int i;
  int xAxes;
  const  int twoDimensions = 2, threeDimensions = 3;
  
  switch (numberOfDimensions)

  {
  case twoDimensions:
    
    face = rAxis;
    {for (i=0; i<3; i++) extra[i] = -inc(face,i); }
    getDefaultIndex (returnedValue, 0, face, 0, 0, 0, I[0], I[1], I[2], extra[0], extra[1], extra[2], I1, I2, I3);

    returnedValue(I[0],I[1],I[2],xAxis,rAxis) =  // g_x along r-direction = r_x*g_r + s_x*g_s

      rX(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],rAxis)
      + QUARTER*(
        sX(I[0]  ,I[1]  ,I[2])*dPhi(I[0]  ,I[1]  ,I[2],sAxis)
      + sX(I[0]-1,I[1]  ,I[2])*dPhi(I[0]-1,I[1]  ,I[2],sAxis)
      + sX(I[0]  ,I[1]+1,I[2])*dPhi(I[0]  ,I[1]+1,I[2],sAxis)
      + sX(I[0]-1,I[1]+1,I[2])*dPhi(I[0]-1,I[1]+1,I[2],sAxis));
    

    returnedValue(I[0],I[1],I[2],yAxis,rAxis) =  // g_y along r-direction = r_y*g_r + s_y*g_s
      
      rY(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],rAxis)
      + QUARTER*
       (sY(I[0]  ,I[1]  ,I[2])*dPhi(I[0]  ,I[1]  ,I[2],sAxis)
      + sY(I[0]-1,I[1]  ,I[2])*dPhi(I[0]-1,I[1]  ,I[2],sAxis)
      + sY(I[0]  ,I[1]+1,I[2])*dPhi(I[0]  ,I[1]+1,I[2],sAxis)
      + sY(I[0]-1,I[1]+1,I[2])*dPhi(I[0]-1,I[1]+1,I[2],sAxis));
    if (!isVolumeScaled)
    {
      int xAxes;
      ForAxes (xAxes)
      {
        returnedValue(I[0],I[1],I[2],xAxes,rAxis) /= 
         	0.5*(cellVolume(I[0],I[1],I[2]) + cellVolume(I[0]-1,I[1],I[2]));
      }
    }

    face = sAxis;
   {for (int i=0; i<3; i++) extra[i] = -inc(face,i); }
    
    getDefaultIndex (returnedValue, 0, face, 0, 0, 0, I[0], I[1], I[2], extra[0], extra[1], extra[2], I1, I2, I3);
    

    returnedValue(I[0],I[1],I[2],xAxis,sAxis) = // g_x along s-direction = r_x*g_r + s_x*g_s

        QUARTER*(
        rX(I[0]  ,I[1]  ,I[2])*dPhi(I[0]  ,I[1]  ,I[2], rAxis)
      + rX(I[0]  ,I[1]-1,I[2])*dPhi(I[0]  ,I[1]-1,I[2], rAxis)
      + rX(I[0]+1,I[1]  ,I[2])*dPhi(I[0]+1,I[1]  ,I[2], rAxis)
      + rX(I[0]+1,I[1]-1,I[2])*dPhi(I[0]+1,I[1]-1,I[2], rAxis))
    +
      sX(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],sAxis);
    
    returnedValue(I[0],I[1],I[2],yAxis,sAxis) = // g_y along s-direction = r_y*g_r + s_y*g_

        QUARTER*(
        rY(I[0]  ,I[1]  ,I[2])*dPhi(I[0]  ,I[1]  ,I[2], rAxis)
      + rY(I[0]  ,I[1]-1,I[2])*dPhi(I[0]  ,I[1]-1,I[2], rAxis)
      + rY(I[0]+1,I[1]  ,I[2])*dPhi(I[0]+1,I[1]  ,I[2], rAxis)
      + rY(I[0]+1,I[1]-1,I[2])*dPhi(I[0]+1,I[1]-1,I[2], rAxis))
    +
      sY(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],sAxis);   

    if (!isVolumeScaled)
    {
      int xAxes;
      ForAxes (xAxes)
      {
        returnedValue(I[0],I[1],I[2],xAxes,sAxis) /=
	  0.5*(cellVolume(I[0],I[1],I[2]) + cellVolume(I[0],I[1]-1,I[2]));
      }
    }
    
    break;
    
  case threeDimensions:

    face = rAxis;
    {for (i=0; i<3; i++) extra[i] = -inc(face,i); }
    getDefaultIndex (returnedValue, 0, face, 0, 0, 0, I[0], I[1], I[2], extra[0], extra[1], extra[2], I1, I2, I3);

    //...r-face gradients

    returnedValue(I[0],I[1],I[2],xAxis,rAxis) =  // g_x along r-direction = r_x*g_r + s_x*g_s + t_x*g_t

      rX(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],rAxis)
	+ QUARTER*
	  (  sX(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  ,sAxis)
	   + sX(I[0]-1,I[1]  ,I[2]  )*dPhi(I[0]-1,I[1]  ,I[2]  ,sAxis)
	   + sX(I[0]  ,I[1]+1,I[2]  )*dPhi(I[0]  ,I[1]+1,I[2]  ,sAxis)
	   + sX(I[0]-1,I[1]+1,I[2]  )*dPhi(I[0]-1,I[1]+1,I[2]  ,sAxis)

	   + tX(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  ,tAxis)
	   + tX(I[0]-1,I[1]  ,I[2]  )*dPhi(I[0]-1,I[1]  ,I[2]  ,tAxis)
	   + tX(I[0]  ,I[1]  ,I[2]+1)*dPhi(I[0]  ,I[1]  ,I[2]+1,tAxis)
	   + tX(I[0]-1,I[1]  ,I[2]+1)*dPhi(I[0]-1,I[1]  ,I[2]+1,tAxis)
	   );

    returnedValue(I[0],I[1],I[2],yAxis,rAxis) =  // g_y along r-direction = r_y*g_r + s_y*g_s + t_y*g_t
      
      rY(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],rAxis)
      + QUARTER*
	(  sY(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  ,sAxis)
	 + sY(I[0]-1,I[1]  ,I[2]  )*dPhi(I[0]-1,I[1]  ,I[2]  ,sAxis)
	 + sY(I[0]  ,I[1]+1,I[2]  )*dPhi(I[0]  ,I[1]+1,I[2]  ,sAxis)
	 + sY(I[0]-1,I[1]+1,I[2]  )*dPhi(I[0]-1,I[1]+1,I[2]  ,sAxis)

	 + tY(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  ,tAxis)
	 + tY(I[0]-1,I[1]  ,I[2]  )*dPhi(I[0]-1,I[1]  ,I[2]  ,tAxis)
	 + tY(I[0]  ,I[1]  ,I[2]+1)*dPhi(I[0]  ,I[1]  ,I[2]+1,tAxis)
	 + tY(I[0]-1,I[1]  ,I[2]+1)*dPhi(I[0]-1,I[1]  ,I[2]+1,tAxis)
	 );

    returnedValue(I[0],I[1],I[2],yAxis,rAxis) =  // g_z along r-direction = r_z*g_r + s_z*g_s + t_z*g_t
      
      rZ(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],rAxis)
      + QUARTER*
	(  sZ(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  ,sAxis)
	 + sZ(I[0]-1,I[1]  ,I[2]  )*dPhi(I[0]-1,I[1]  ,I[2]  ,sAxis)
	 + sZ(I[0]  ,I[1]+1,I[2]  )*dPhi(I[0]  ,I[1]+1,I[2]  ,sAxis)
	 + sZ(I[0]-1,I[1]+1,I[2]  )*dPhi(I[0]-1,I[1]+1,I[2]  ,sAxis)

	 + tZ(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  ,tAxis)
	 + tZ(I[0]-1,I[1]  ,I[2]  )*dPhi(I[0]-1,I[1]  ,I[2]  ,tAxis)
	 + tZ(I[0]  ,I[1]  ,I[2]+1)*dPhi(I[0]  ,I[1]  ,I[2]+1,tAxis)
	 + tZ(I[0]-1,I[1]  ,I[2]+1)*dPhi(I[0]-1,I[1]  ,I[2]+1,tAxis)
	 );

    if (!isVolumeScaled)
    {
      ForAxes (xAxes)
      {
        returnedValue(I[0],I[1],I[2],xAxes,rAxis) /= 
         	0.5*(cellVolume(I[0],I[1],I[2]) + cellVolume(I[0]-1,I[1],I[2]));
      }
    }

    // ... s-face gradients

    face = sAxis;
   {for (int i=0; i<3; i++) extra[i] = -inc(face,i); }
    
    getDefaultIndex (returnedValue, 0, face, 0, 0, 0, I[0], I[1], I[2], extra[0], extra[1], extra[2], I1, I2, I3);
    

    returnedValue(I[0],I[1],I[2],xAxis,sAxis) = // g_x along s-direction = r_x*g_r + s_x*g_s + t_x*g_t

      sX(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],sAxis)

	+ QUARTER*
	  (  rX(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , rAxis)
	   + rX(I[0]  ,I[1]-1,I[2]  )*dPhi(I[0]  ,I[1]-1,I[2]  , rAxis)
	   + rX(I[0]+1,I[1]  ,I[2]  )*dPhi(I[0]+1,I[1]  ,I[2]  , rAxis)
	   + rX(I[0]+1,I[1]-1,I[2]  )*dPhi(I[0]+1,I[1]-1,I[2]  , rAxis)

    	   + tX(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , tAxis)
	   + tX(I[0]  ,I[1]-1,I[2]  )*dPhi(I[0]  ,I[1]-1,I[2]  , tAxis)
	   + tX(I[0]  ,I[1]  ,I[2]+1)*dPhi(I[0]  ,I[1]  ,I[2]+1, tAxis)
	   + tX(I[0]  ,I[1]-1,I[2]+1)*dPhi(I[0]  ,I[1]-1,I[2]+1, tAxis));
    	 

    
    returnedValue(I[0],I[1],I[2],yAxis,sAxis) = // g_y along s-direction = r_y*g_r + s_y*g_s + t_y*g_t

      sY(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],sAxis)

	+ QUARTER*
	  (  rY(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , rAxis)
	   + rY(I[0]  ,I[1]-1,I[2]  )*dPhi(I[0]  ,I[1]-1,I[2]  , rAxis)
	   + rY(I[0]+1,I[1]  ,I[2]  )*dPhi(I[0]+1,I[1]  ,I[2]  , rAxis)
	   + rY(I[0]+1,I[1]-1,I[2]  )*dPhi(I[0]+1,I[1]-1,I[2]  , rAxis)

    	   + tY(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , tAxis)
	   + tY(I[0]  ,I[1]-1,I[2]  )*dPhi(I[0]  ,I[1]-1,I[2]  , tAxis)
	   + tY(I[0]  ,I[1]  ,I[2]+1)*dPhi(I[0]  ,I[1]  ,I[2]+1, tAxis)
	   + tY(I[0]  ,I[1]-1,I[2]+1)*dPhi(I[0]  ,I[1]-1,I[2]+1, tAxis));

    returnedValue(I[0],I[1],I[2],zAxis,sAxis) = // g_z along s-direction = r_z*g_r + s_z*g_s + t_z*g_t

      sZ(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],sAxis)

	+ QUARTER*
	  (  rZ(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , rAxis)
	   + rZ(I[0]  ,I[1]-1,I[2]  )*dPhi(I[0]  ,I[1]-1,I[2]  , rAxis)
	   + rZ(I[0]+1,I[1]  ,I[2]  )*dPhi(I[0]+1,I[1]  ,I[2]  , rAxis)
	   + rZ(I[0]+1,I[1]-1,I[2]  )*dPhi(I[0]+1,I[1]-1,I[2]  , rAxis)

    	   + tZ(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , tAxis)
	   + tZ(I[0]  ,I[1]-1,I[2]  )*dPhi(I[0]  ,I[1]-1,I[2]  , tAxis)
	   + tZ(I[0]  ,I[1]  ,I[2]+1)*dPhi(I[0]  ,I[1]  ,I[2]+1, tAxis)
	   + tZ(I[0]  ,I[1]-1,I[2]+1)*dPhi(I[0]  ,I[1]-1,I[2]+1, tAxis)

	   );

    // ... t-face gradients

    face = tAxis;
   {for (int i=0; i<3; i++) extra[i] = -inc(face,i); }
    
    getDefaultIndex (returnedValue, 0, face, 0, 0, 0, I[0], I[1], I[2], extra[0], extra[1], extra[2], I1, I2, I3);
    

    returnedValue(I[0],I[1],I[2],xAxis,tAxis) = // g_x along t-direction = r_x*g_r + s_x*g_s + t_x*g_t

      tX(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],tAxis)

	+ QUARTER*
	  (  rX(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , rAxis)
	   + rX(I[0]  ,I[1]  ,I[2]-1)*dPhi(I[0]  ,I[1]  ,I[2]-1, rAxis)
	   + rX(I[0]+1,I[1]  ,I[2]  )*dPhi(I[0]+1,I[1]  ,I[2]  , rAxis)
	   + rX(I[0]+1,I[1]  ,I[2]-1)*dPhi(I[0]+1,I[1]  ,I[2]-1, rAxis)

	   + sX(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , sAxis)
	   + sX(I[0]  ,I[1]  ,I[2]-1)*dPhi(I[0]  ,I[1]  ,I[2]-1, sAxis)
	   + sX(I[0]  ,I[1]+1,I[2]  )*dPhi(I[0]  ,I[1]+1,I[2]  , sAxis)
	   + sX(I[0]  ,I[1]+1,I[2]-1)*dPhi(I[0]  ,I[1]+1,I[2]-1, sAxis)

	   );
    
    returnedValue(I[0],I[1],I[2],yAxis,tAxis) = // g_y along t-direction = r_y*g_r + s_y*g_s + t_y*g_t
      tY(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],tAxis)

	+ QUARTER*
	  (  rY(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , rAxis)
	   + rY(I[0]  ,I[1]  ,I[2]-1)*dPhi(I[0]  ,I[1]  ,I[2]-1, rAxis)
	   + rY(I[0]+1,I[1]  ,I[2]  )*dPhi(I[0]+1,I[1]  ,I[2]  , rAxis)
	   + rY(I[0]+1,I[1]  ,I[2]-1)*dPhi(I[0]+1,I[1]  ,I[2]-1, rAxis)

	   + sY(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , sAxis)
	   + sY(I[0]  ,I[1]  ,I[2]-1)*dPhi(I[0]  ,I[1]  ,I[2]-1, sAxis)
	   + sY(I[0]  ,I[1]+1,I[2]  )*dPhi(I[0]  ,I[1]+1,I[2]  , sAxis)
	   + sY(I[0]  ,I[1]+1,I[2]-1)*dPhi(I[0]  ,I[1]+1,I[2]-1, sAxis)

	   );

    returnedValue(I[0],I[1],I[2],zAxis,tAxis) = // g_z along t-direction = r_z*g_r + s_z*g_s + t_z*g_t

      tZ(I[0],I[1],I[2])*dPhi(I[0],I[1],I[2],tAxis)

	+ QUARTER*
	  (  rZ(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , rAxis)
	   + rZ(I[0]  ,I[1]  ,I[2]-1)*dPhi(I[0]  ,I[1]  ,I[2]-1, rAxis)
	   + rZ(I[0]+1,I[1]  ,I[2]  )*dPhi(I[0]+1,I[1]  ,I[2]  , rAxis)
	   + rZ(I[0]+1,I[1]  ,I[2]-1)*dPhi(I[0]+1,I[1]  ,I[2]-1, rAxis)

	   + sZ(I[0]  ,I[1]  ,I[2]  )*dPhi(I[0]  ,I[1]  ,I[2]  , sAxis)
	   + sZ(I[0]  ,I[1]  ,I[2]-1)*dPhi(I[0]  ,I[1]  ,I[2]-1, sAxis)
	   + sZ(I[0]  ,I[1]+1,I[2]  )*dPhi(I[0]  ,I[1]+1,I[2]  , sAxis)
	   + sZ(I[0]  ,I[1]+1,I[2]-1)*dPhi(I[0]  ,I[1]+1,I[2]-1, sAxis)

	   );

    if (!isVolumeScaled)
    {
      int xAxes;
      ForAxes (xAxes)
      {
        returnedValue(I[0],I[1],I[2],xAxes,tAxis) /=
	  0.5*(cellVolume(I[0],I[1],I[2]) + cellVolume(I[0],I[1]-1,I[2]));
      }

    }
    
    
    break;
    
  default:
    cout << "FCgradient not implemented for numberOfDimensions = " << numberOfDimensions << endl;
    throw " ";


  };
  
    
  
  if (debug) FCgradDisplay.display (returnedValue, "FCgrad: returnedValue");

//  not sure we need to (or can) do this for a faceCentered variable

//  where (mappedGrid.mask()(I[0],I[1], I[2]) <= 0) 
//  {
//     returnedValue(I[0],I[1],I[2]) = 0.;
//  }

  returnedValue.periodicUpdate();
  return (returnedValue);
  
  
}



