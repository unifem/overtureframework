#include "MappedGridFiniteVolumeOperators.h"
// =================================================================================
//\begin{>divergence.tex}{\subsection{div: divergence of a cellCentered or FaceCentered velocity}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
div (
     const REALMappedGridFunction &u,
     const Index & I1,		// = nullIndex
     const Index & I2,		// = nullIndex
     const Index & I3,          // = nullIndex
     const Index & I4,          // = nullIndex
     const Index & I5,          // = nullIndex
     const Index & I6,          // = nullIndex
     const Index & I7,          // = nullIndex
     const Index & I8)		// = nullIndex
// 
// /Purpose:
// compute cellCentered divergence of input array u
// this can take either a cellcenterd input velocity 		u(cg,cellCentered)
// or face-centered velocities available on all faces 		u(cg,faceCenteredAll)
//
//	/u (input):		cellCentered or appropriately FaceCentered input velocity
//	/I1,I2,I3:	Index'es for which values are to be returned.
//				If any I* = nullIndex, then values will be returned
//				for all possible index values, depending on the 
//				dimensions of u
//
//	/div (output):		div(I1,I2,I3) is returned
//
// /Author:		D.L.Brown
// /Date Documentation Last Modified:	951027
//
//\end{divergence.tex} 
// =================================================================================
{
  bool PLOT_ON0 = FALSE;
  return (div (u, NULL, NULL, PLOT_ON0, I1, I2, I3, I4, I5, I6, I7, I8));
}


// =================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::
div (		const REALMappedGridFunction &u,
                GenericGraphicsInterface * ps,
                GraphicsParameters * psp,
                bool & PLOT_ON,
		const Index & I1,		// = nullIndex
		const Index & I2,		// = nullIndex
		const Index & I3,               // = nullIndex
		const Index & I4,               // = nullIndex
		const Index & I5,               // = nullIndex
		const Index & I6,               // = nullIndex
		const Index & I7,               // = nullIndex
		const Index & I8)		// = nullIndex

// ========================================
//
{

  if (debug) divDisplay.interactivelySetInteractiveDisplay ("divDisplay initialization");
  
  REALMappedGridFunction divergence;
  int localNumberOfComponents = 1;
  divergence.updateToMatchGrid (mappedGrid, all, all, all, localNumberOfComponents);

  int axis;

  divergence.setIsCellCentered (TRUE);

  divergence = 0.;

  Index R1,R2,R3;

	// ========================================
	// determine whether the input gridFunction is cell-centered of
	// dimension nd, or face-centered of dimension nd*nd
	// and then determine the appropriate Ranges in which values
	// will be returned.
	// ========================================

   GridFunctionParameters::GridFunctionTypeWithComponents uType = u.getGridFunctionTypeWithComponents(); 

   if (u.getIsCellCentered()){

	// ========================================
	// if u is CellCentered, check to see that dimension = numberOfDimensions
	// ========================================

    int inputNumberOfComponents = u.getComponentDimension(0);
    if (inputNumberOfComponents != numberOfDimensions){

      cout << "MappedGridFiniteVolumeOperators::div: input array is not an appropriate vector" << endl;
      cout << "Input array is CellCentered, but " << endl;
      cout << "Actual numberOfComponents   = " << inputNumberOfComponents << endl;
      cout << "Required numberOfComponents = " << numberOfDimensions << endl << endl;
      throw " ";
      
    }
	// ========================================
	// It's cell centered and there are enough components, so determine R1,R2,R3,
	// which are the ranges in which values are to be returned. Either given or
	// one less than the dimension of the input array
	// ========================================

    int extra[3] = {-1,-1,-1};

    int c[5] = {0,0,0,0,0};
    getDefaultIndex (divergence, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

    if (debug) {
      cout << "div will be computed in ranges" << endl;
      cout << "Range(" << R1.getBase() << "," << R1.getBound() << ")" << endl;
      cout << "Range(" << R2.getBase() << "," << R2.getBound() << ")" << endl;
      cout << "Range(" << R3.getBase() << "," << R3.getBound() << ")" << endl << endl;
    }


  } else {

	// ========================================
	// u is not cellCentered, so first determine whether it is appropriately dimensioned
	// ========================================
    
    int dim1 = u.getComponentDimension(0);
    int dim2 = u.getComponentDimension(1);

    if (dim1 != numberOfDimensions || dim2 != numberOfDimensions){

      cout << "MappedGridFiniteVolumeOperators::div: input array is not an appropriate vector" << endl;
      cout << "Input array not cellCentered, so since the only other option is faceCentered, " << endl;
      cout << "the number of components must be appropriate. " << endl;
      throw " ";
    }

    if (u.getFaceCentering() != GridFunctionParameters::all)
    {
      cout << "MappedGridFiniteVolumeOperators::div: input array is not properly faceCentered " << endl;
      cout << "Can't compute the divergence " << endl;
      throw " ";
    }
	// ========================================
	// passed all tests, so compute div
	// ========================================
    int extra[3] = {0,0,0};

    int c[5] = {0,0,0,0,0};
    getDefaultIndex (divergence, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);

    if (debug) {
      cout << "div will be computed in ranges:" << endl;
      cout << "Range(" << R1.getBase() << "," << "R1.getBound()" << ")" << endl;
      cout << "Range(" << R2.getBase() << "," << "R2.getBound()" << ")" << endl;
    }

  }

	// ========================================
	// Arithmetic is done here:
  	// loop over all axes, difference and sum to get scaled divergence
	// ========================================

  REALMappedGridFunction uBar;
//  uBar.updateToMatchGridFunction (u, GridFunctionParameters::faceCenteredAll);
  uBar.updateToMatchGrid (mappedGrid, GridFunctionParameters::faceCenteredAll);

  uBar = contravariantVelocity (u);
  if (debug) divDisplay.display (u,    "MappedGridFiniteVolumeOperators::div: input velocity");
  if (debug) divDisplay.display (uBar, "MappedGridFiniteVolumeOperators::div: contravariant velocity");

  if (PLOT_ON && psp && ps )
  {
    psp->set (GI_TOP_LABEL, "velocity");
    psp->set (GI_COMPONENT_FOR_CONTOURS, 0);
    PlotIt::contour (*ps, u, *psp);
    psp->set (GI_COMPONENT_FOR_CONTOURS, 1);
    PlotIt::contour (*ps, u, *psp);
  }

  int c[5] = {0,0,0,0,0};
  ForAxes (axis) {
    c[0] = axis;
    divergence(R1,R2,R3) += difference( uBar, axis, c[0], c[1], c[2], c[3], c[4])( R1, R2, R3);
    if (debug) divDisplay.display (divergence, "MappedGridFiniteVolumeOperators::div: intermediate value of div");
  }

  if (PLOT_ON && psp && ps)
  {
    psp->set (GI_TOP_LABEL, "intermediate value of div");
    PlotIt::contour (*ps, divergence, *psp);
  }

/* ***970304: when no compositeGrid, no mask array; this should be left up to the user
  where (mappedGrid.mask()(R1,R2,R3) <= 0)
  {
     divergence(R1,R2,R3) = 0.;
  }
  */

  if (PLOT_ON && psp && ps)
  {
    psp->set (GI_TOP_LABEL, "2nd intermed value of div");
    PlotIt::contour (*ps, divergence, *psp);
  }
  
  if (debug) divDisplay.display (divergence, "MappedGridFiniteVolumeOperators::div: divergence before volume scaling");

	// ========================================
  	// optionally divide by volume of cell
	// ========================================

  if (!isVolumeScaled) divergence(R1,R2,R3) /= cellVolume(R1,R2,R3);

  divergence.periodicUpdate ();

  if (PLOT_ON && psp && ps)
  {
    psp->set (GI_TOP_LABEL, "divergence after periodicUpdate");
    PlotIt::contour (*ps, divergence, *psp);
  }
  
  return (divergence);
}

// =================================================================================
//\begin{>>divergence.tex}{\subsection{divNormal: divergence of a normal velocity}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
divNormal (	const REALMappedGridFunction &uBar,
		const Index & I1,		// = nullIndex
		const Index & I2,		// = nullIndex
		const Index & I3,               // = nullIndex
		const Index & I4,               // = nullIndex
		const Index & I5,               // = nullIndex
		const Index & I6,               // = nullIndex
		const Index & I7,               // = nullIndex
		const Index & I8)		// = nullIndex
//
// /Purpose:
// compute cellCentered divergence of input normal velocities uBar
//
//	/uBar(mg,faceCenteredAll) (input):		faceCentered Normal velocities
//	/I1,I2,I3:	Index'es for which values are to be returned.
//			If any I* = nullIndex, then values will be returned
//			for all possible index values, depending on the 
//			dimensions of u
//
//	/div (output):		div(I1,I2,I3) is returned
//
// /Author:		D.L.Brown
//\end{divergence.tex} 
// =================================================================================
{
  bool PLOT_ON0 = FALSE;
  return (divNormal (uBar, NULL, NULL, PLOT_ON0, I1, I2, I3, I4, I5, I6, I7, I8));
}


// =================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::
divNormal (	const REALMappedGridFunction &uBar,
                GenericGraphicsInterface * ps,
                GraphicsParameters * psp,
                bool & PLOT_ON,
		const Index & I1,		// = nullIndex
		const Index & I2,		// = nullIndex
		const Index & I3,               // = nullIndex
		const Index & I4,               // = nullIndex
		const Index & I5,               // = nullIndex
		const Index & I6,               // = nullIndex
		const Index & I7,               // = nullIndex
		const Index & I8)		// = nullIndex

// =================================================================================
{
//
  if (debug) divDisplay.interactivelySetInteractiveDisplay ("divDisplay initialization");
  
  REALMappedGridFunction divergence;
  int localNumberOfComponents = 1;
  divergence.updateToMatchGrid (mappedGrid, all, all, all, localNumberOfComponents);

  int axis;

  divergence.setIsCellCentered (TRUE);

  divergence = 0.;

  Index R1,R2,R3;

  if (uBar.getFaceCentering() != GridFunctionParameters::all)
  {

    cout << "MappedGridFiniteVolumeOperators::div: input array is not properly faceCentered " << endl;
    cout << "Can't compute the divergence " << endl;
    throw " ";
    
  }
  

  int extra[3] = {0,0,0};

  int c[5] = {0,0,0,0,0};
  getDefaultIndex (divergence, c[0], c[1], c[2], c[3], c[4], R1, R2, R3, extra[0], extra[1], extra[2], I1, I2, I3);
  if (debug) {
      cout << "div will be computed in ranges:" << endl;
      cout << "Range(" << R1.getBase() << "," << "R1.getBound()" << ")" << endl;
      cout << "Range(" << R2.getBase() << "," << "R2.getBound()" << ")" << endl;
  }


	// ========================================
	// Arithmetic is done here:
  	// loop over all axes, difference and sum to get scaled divergence
	// ========================================

 // c = {0,0,0,0,0};
  
  ForAxes (axis) {
    c[0] = axis;
    divergence(R1,R2,R3) += difference( uBar, axis, c[0], c[1], c[2], c[3], c[4])( R1, R2, R3);
    if (debug) divDisplay.display (divergence, "MappedGridFiniteVolumeOperators::div: intermediate value of div");
  }

  if (PLOT_ON && psp && ps)
  {
    psp->set (GI_TOP_LABEL, "div before mask, scaling, periodicUpdate");
    PlotIt::contour (*ps, divergence, *psp);
  }

  where (mappedGrid.mask()(R1,R2,R3) <= 0)
  {
     divergence(R1,R2,R3) = 0.;
  }

  if (PLOT_ON && psp && ps)
  {
    psp->set (GI_TOP_LABEL, "div after mask applied");
    PlotIt::contour (*ps, divergence, *psp);
  }
  
  if (debug) divDisplay.display (divergence, "MappedGridFiniteVolumeOperators::div: divergence before volume scaling");

	// ========================================
  	// optionally divide by volume of cell
	// ========================================

  if (!isVolumeScaled) divergence(R1,R2,R3) /= cellVolume(R1,R2,R3);

  divergence.periodicUpdate ();

  if (PLOT_ON && psp && ps)
  {
    psp->set (GI_TOP_LABEL, "div after scaling and periodicUpdate");
    PlotIt::contour (*ps, divergence, *psp);
  }
  
  return (divergence);
}

