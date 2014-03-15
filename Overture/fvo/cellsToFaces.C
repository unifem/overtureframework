#include "MappedGridFiniteVolumeOperators.h"
// =================================================================================
//\begin{>cellsToFaces.tex}{\subsection{cellsToFaces}}    
REALMappedGridFunction MappedGridFiniteVolumeOperators::
cellsToFaces (		const REALMappedGridFunction &u,            
			const Index & I1,		// = nullIndex                          
			const Index & I2,               // = nullIndex
			const Index & I3,               // = nullIndex
		const Index & I4,               // = nullIndex
		const Index & I5,               // = nullIndex
		const Index & I6,               // = nullIndex
		const Index & I7,               // = nullIndex
		const Index & I8)  		// = nullIndex
//
// /Purpose:
//	Take a cell-centered multi-component grid function and turn it into
//	a face-centered one by averaging to the faces. The function adds
//      one index to the input MappedGridFunction and puts the faceRange there.
//
// /u (mg,cellCentered,Range0,Range1,...): input function
//
// /cellsToFaces(mg,faceCenteredAll,Range0,Range1,...): output function
// 
// /Author: D. L. Brown
// /Date Documentation Last Modified: 951030
//
//\end{cellsToFaces.tex} 
//========================================
{
  if (debug) cellsToFacesDisplay.interactivelySetInteractiveDisplay ("cellsToFacesDisplay initialization");

  // ========================================
  // Make sure the input array is cellCentered
  // ========================================

  if (!u.getIsCellCentered()){
    printf ("cellsToFaces: ERROR: input gridFunction is not cellCentered\n");
  }

  // ========================================
  // Since the number of indices will increase by one, this limits
  // the number of components that the input array is allowed to have
  // ========================================

//  if (u.getComponentDimension(4)>1){

  int inputNumberOfComponents = u.getNumberOfComponents();
  if (inputNumberOfComponents > 4)
  {
    printf ("cellsToFaces: input function has too many indices to convert to faceCentered");
    exit (-1);
  }

   // ========================================
   // rather than being generic, we assume the input array is dimensioned
   //  u(coord, coord, coord, comp, comp,...); make sure this is true
   // ========================================

   standardOrderingErrorMessage (u, "cellsToFaces");

    // ========================================
    // set up indexing for returnedValue and allocate space
    // ========================================

  REALMappedGridFunction returnedValue;

    Range R[8];
    int i;
    for (i=0; i<3; i++)R[i] = all;

//  for (i=3; i<8; i++) R[i] = Range(u.getComponentBase(i-3),u.getComponentBound(i-3));

    for (i=3; i<8; i++) R[i] = (i-3 >= inputNumberOfComponents) ? nullRange : Range(u.getComponentBase(i-3),u.getComponentBound(i-3));


//  int faceRangeLocation = 3;
// for (i=7; i>2; i--)
//  {
//    if (u.getComponentDimension(i-3) == 1)faceRangeLocation = i;
//  }

  int faceRangeLocation = 3 + inputNumberOfComponents;
  
  R[faceRangeLocation] = faceRange;
  faceRangeLocation -= 3;

  if (debug) cout << "MappedGridFiniteVolumeOperators::cellsToFaces: faceRangeLocation = " << faceRangeLocation << endl;
      
  returnedValue.updateToMatchGrid (mappedGrid, R[0], R[1], R[2], R[3], R[4], R[5], R[6], R[7]); 
  returnedValue.setFaceCentering (GridFunctionParameters::all);
  returnedValue = 0.;

  GridFunctionParameters::GridFunctionTypeWithComponents gfType = returnedValue.getGridFunctionTypeWithComponents(); 

  if (debug) cellsToFacesDisplay.display (returnedValue	,"cellsToFaces: this is initial returnedValue:");


	
    Index R1,R2,R3;
// debug purposes:
  
    Index S[3]; getIndex (mappedGrid.dimension(), S[0], S[1], S[2]);


	// ========================================
  	// loop over all faces and compute the corresponding average
	// ========================================

  int index[5];
  for (i=0; i<4; i++) index[i] = i<faceRangeLocation ? i : i+1;

    int face;
    ForAxes (face)
    {
      int extra1 = -inc(face,rAxis); int extra2 = -inc(face,sAxis); int extra3 = - inc(face,tAxis);
      int c[5] = {0,0,0,0,0}; c[faceRangeLocation] = face;
      getDefaultIndex (returnedValue, c[0],c[1],c[2],c[3],c[4], R1, R2, R3, extra1, extra2, extra3, I1, I2, I3);

      for (c[index[3]]=u.getComponentBase(3); c[index[3]]<=u.getComponentBound(3); c[index[3]]++){
	for (c[index[2]]=u.getComponentBase(2); c[index[2]]<=u.getComponentBound(2); c[index[2]]++){
	  for (c[index[1]]=u.getComponentBase(1); c[index[1]]<=u.getComponentBound(1); c[index[1]]++){
	    for (c[index[0]]=u.getComponentBase(0); c[index[0]]<=u.getComponentBound(0); c[index[0]]++){
              c[faceRangeLocation] = face;
	      int cLast = 0;
	      
	      returnedValue(S[0],S[1],S[2],c[0],c[1],c[2],c[3],c[4]) =   
		average(u,face,c[index[0]],c[index[1]],c[index[2]],c[index[3]],cLast,R1,R2,R3)(S[0],S[1],S[2]);

      }}}}
    }
    if (debug)
    {
      if (debug) cellsToFacesDisplay.display (u       		, "cellsToFaces: this is u (input): ");
      if (debug) cellsToFacesDisplay.display (returnedValue    	, "cellsToFaces: this is uFace:");
    }

    gfType = returnedValue.getGridFunctionTypeWithComponents(); 
    return (returnedValue);
}
