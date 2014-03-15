#include "Projection.h"

// ===============================================================================
int Projection::
getMaximumCMPGRDBC ()
//
// figure out what the maximum integer used to label CMPGRD boundary
// conditions is and return it
//
// ===============================================================================
{
  int returnedValue = 0;
  int grid;
  ForAllGrids (grid)
  {
    int side, axis;
    MappedGrid & mg = compositeGrid[grid];
    ForBoundary(side,axis)
    {
      if (mg.boundaryCondition()(side,axis) > returnedValue) returnedValue = mg.boundaryCondition()(side,axis);
    }
  } 
  cout << "getMaximumCMPGRDBC: " << returnedValue << " is the maximum CMPGRD boundary condition code for this grid" << endl;
  return (returnedValue);
}

void Projection::
associateCMPGRDBoundaryCondition (const int bcCMPGRD,
 			          const BoundaryConditionType pressureBC)
{
  cout << "Projection::associateCMPGRDBoundaryCondition is obosolete; please use associateOvertureBoundaryCondition" << endl;
  associateOvertureBoundaryCondition (bcCMPGRD, pressureBC);
}

	
// ===============================================================================
//\begin{>> projectionDoc.tex}{\subsection{associateOvertureBoundaryCondition}} 
void Projection::
associateOvertureBoundaryCondition (const int bcOverture,
				    const BoundaryConditionType ellipticBC)
//
// /Purpose:
//    associate Projection BoundaryConditionType's with a Overture boundaryCondition value.
//    Overture labels the sides of grids with integer codes. These codes need
//    to be associated with boundary conditions for the projection. This routine
//    associates boundary conditions for the elliptic solution $\phi$ 
//    with a Overture boundary condition code. This routine can be used
//    to hardwire boundary condition associations into a solver if you don't 
//    want to use the interactive routine {\bf interactivelyAssociateBoundaryConditions}. 
//
// /bcOverture: this Overture boundaryCondition value
// /ellipticBC: boundary condition type for the elliptic solve 
//              The choices are 
//    \begin{itemize}
//      \item{homogeneousNeumann} Set $\partial\phi/\partial n = 0$, really!
//      \item{valueGiven} This is supposed to set a value for phi, but the only choice at 
//                        the moment is zero.
//      \item{fixedWallNeumann} Set $\partial\phi  \partial n = n \cdot u^*$.  $u^*$ is
//                        computed using third order extrapolation from the interior.
//                        This is an appropriate boundary condition if $u = 0$ on the 
//                        boundary, by consistency with the definition of $u^*$.
//      \item{movingWallNeumann} Set $\partial\phi  \partial n = n \cdot (u^* - u_{grid})$.  $u^*$ is
//                        computed using third order extrapolation from the interior. $u_{grid}$
//                        is the velocity of the moving boundary, which is extracted from
//                        the DynamicMovingGrids object. It is determined at the boundary
//                        by third-order extrapolation as well.
//      \item{extrapolateAllComponents}  This sets a homogeneous extrapolation BC for $\phi$. (I'm not
//                        sure why you would want to do this, but its in here.)
//    \end{itemize}
//
//
// /Author: D. L. Brown
// /Changes: (960130): no longer sets velocity BCs
//\end{projectionDoc.tex}
// ===============================================================================
{ 
  if (firstCallToUTMG)
    {
      cout << "Projection::associateOvertureBoundaryCondition: listOfBCs has not yet been initialized and so cannot be set " << endl;
      cout << " call Projection::updateToMatchGridOnly first" << endl;
      throw "associateOvertureBoundaryCondition";
    }
  
  if (bcOverture > 0 && bcOverture <= maximumNumberOfOvertureBCs)
  {
    listOfBCs[bcOverture]         = ellipticBC;
  }
  else if (bcOverture <= 0)
  {
    cout << "Projection::associateOvertureBoundaryCondition: input value of bcOverture must be positive" << endl;
    throw "associateOvertureBoundaryCondition";
  }
  else
  {
    cout << "Projection::associateOvertureBoundaryCondition: input value of bcOverture = " << bcOverture << " must be less than maximumNumberOfOvertureBCs = " <<
      maximumNumberOfOvertureBCs << endl;
    throw "associateOvertureBoundaryCondition";
  }
    
}
void Projection::
interactivelyAssociateBoundaryConditions ()
//
// /Purpose:
//   Retained for backward compatibility.  This has been replaced by the function
//   Projection::boundaryConditionWizard()
//
{
  cout << "Projection::interactivelyAssociateBoundaryConditions: WARNING: obsolete function " << endl;
  cout << "   Use Projection::boundaryConditionWizard instead" << endl;
  
  boundaryConditionWizard();
}


// ===============================================================================
//\begin{>> projectionDoc.tex}{\subsection{boundaryConditionWizard}}
void Projection::
boundaryConditionWizard ()
//
// /Purpose:
//   provides an interactive interface to the {\tt associateOvertureBoundaryCondition} 
//   function: loops over all sides on the grids; when it finds a new non-trivial Overture
//   boundaryCondition code, it asks which Projection boundary condition to apply. 
//   This function should be called before any calls to the {\tt project}
//   functions.  
//
//\end{projectionDoc.tex}
// =============================================================================== 
{
  int grid, side, axis;
  REAL boundaryVelocity[3];

  cout << "============================================================" << endl;
  cout << "  Interactively associate Projection boundary conditions    " << endl;
  cout << "          with Overture boundaryCondition codes               " << endl;
  cout << " "                                                            << endl;
  cout << "       Use the following Projection BC codes:  "              << endl;
  cout << " "                                                            << endl;
  cout << "    valueGiven                 " << valueGiven                << endl;
  cout << "    normalGiven                " << normalValueGiven          << endl;
  cout << "    fixedWallNeumann           " << fixedWallNeumann          << endl;
  cout << "    movingWallNeumann          " << movingWallNeumann         << endl;
  cout << " "                                                            << endl;
  cout << "============================================================" << endl;  
  cout << " " << endl;
  cout << " " << endl;

  int i;
  if (FALSE)
  {
    cout << "listOfBCs: " << endl;
    for (i=0; i<= maximumNumberOfOvertureBCs; i++) cout << listOfBCs[i] << " ";
    cout << endl << "listOfVelocityBCs:" << endl;
    for (i=0; i<= maximumNumberOfOvertureBCs; i++) cout << listOfVelocityBCs[i] << " ";
    cout << endl;
  }
  
// ...loop over all grids  
  ForAllGrids (grid)
  {
    MappedGrid & mg = compositeGrid[grid];

// ... loop over all boundaries
    ForBoundary (side,axis)
    {
      int bcCode = mg.boundaryCondition()(side,axis);

// ... if this is a non-trivial boundary then...
      if (bcCode > 0) 
      {
	
        int inputValue;      

//    ... if no Projection BC has been set, prompt user for boundary conditions ...
        if (listOfBCs[bcCode] == noBoundaryConditionDefined)
        {
          cout << "  For Overture boundaryCondition code = " << bcCode << endl;


//        ... get the boundary condition for elliptic solve
          inputValue = -1;
	  while (inputValue < 0 || inputValue >= numberOfBoundaryConditionTypes)
	  {

	    cout << "    === Enter Projection phi BC code: ";
	    cin >> inputValue;
            if (inputValue >= 0 && inputValue < numberOfBoundaryConditionTypes)
	    {
              listOfBCs[bcCode] = (BoundaryConditionType) inputValue;
	    }
	    else
	    {
	      cout << "    **** invalid BoundaryConditionType entered " << endl;
	    }
	  }

	  if (FALSE)
	  {
	    
	    //        ... get the boundary condition for the velocity
	    // ... removed 960130
	    inputValue = -1;
	    while (inputValue < 0 || inputValue >= numberOfBoundaryConditionTypes)
	    {
	      cout << "    === Enter Projection velocity BC code: ";
	      cin >> inputValue;

	      if (inputValue >= 0 && inputValue < numberOfBoundaryConditionTypes)
	      {
		listOfVelocityBCs[bcCode] = (BoundaryConditionType) inputValue;
	      }
	      else
	      {
		cout << "    **** invalid BoundaryConditionType entered " << endl;
	      }
	    }
	  }
	  
	  
	}
	
      }
    }
  }

// ====================
// Now prompt for velocities in the case of valueGiven BCs 
// (960130: remove this)
// ====================

  if (FALSE) 
  {
    // ...loop over all grids  
    ForAllGrids (grid)
    {
      MappedGrid & mg = compositeGrid[grid];

      // ... loop over all boundaries
      ForBoundary (side,axis)
      {
	int bcCode = mg.boundaryCondition()(side,axis);

	// ... if this is a non-trivial boundary then...
	if (bcCode > 0) 
	{
	  //    ... if this is a valueGiven boundary, then prompt for values
	  if (listOfVelocityBCs[bcCode] == valueGiven)
	  {
	    switch (numberOfDimensions)
	    {
	    case 2:
	      cout << "Enter u,v on side (" << side << "," << axis << ") of grid " << grid << ":";
	      cin >> boundaryVelocity[0] >> boundaryVelocity[1];

	      setVelocityBoundaryConditionValue (boundaryVelocity[0],0,side,axis,grid);
	      setVelocityBoundaryConditionValue (boundaryVelocity[1],1,side,axis,grid);
	      break;
	      
	    case 3:
	      cout << "Enter u,v,w on side (" << side << "," << axis << ") of grid " << grid << ":";
	      cin >> boundaryVelocity[0] >> boundaryVelocity[1] >> boundaryVelocity[2];
	      
	      setVelocityBoundaryConditionValue (boundaryVelocity[0],0,side,axis,grid);
	      setVelocityBoundaryConditionValue (boundaryVelocity[1],1,side,axis,grid);
	      setVelocityBoundaryConditionValue (boundaryVelocity[2],2,side,axis,grid);
	      break;

	    default:
	      break;
	    };
	  }
	}
      }
    }
  }
  
  
  
  if (FALSE) 
  {
    cout << "listOfBCs: " << endl;
    for (i=0; i<= maximumNumberOfOvertureBCs; i++) cout << listOfBCs[i] << " ";
    cout << endl << "listOfVelocityBCs:" << endl;
    for (i=0; i<= maximumNumberOfOvertureBCs; i++) cout << listOfVelocityBCs[i] << " ";
    cout << endl;
  }
  
}
//==============================================================================
//\begin{>>ProjectionDoc.tex}{\subsection{setVelocityBoundaryConditionValue}}
void Projection::
setVelocityBoundaryConditionValue(
				  REAL value,        // = 0.
				  int component,     // = forAll
				  int side,          // = forAll
				  int axis,          // = forAll
				  int grid,          // = forAll
				  bool trueOrFalse)  // = TRUE
//
// /Purpose:
//   This routine sets boundary values for the velocity on a particular
//   side of a particular component grid.
//
//   /value: The value to be set
//   /component: which component of the velocity is to be set to this value
//   /side: which side (0,1) of the grid this applies to
//   /axis: which axis (0,1,2) of the grid this applies to
//   /grid: which component grid this applies to
//
//\end{ProjectionDoc.tex}
//==============================================================================
{
  Range Side, Axis, Grid, Component;
  Side = side==forAll ? Range(Start,End) : Range(side,side);
  Axis = axis==forAll ? Range(0,numberOfDimensions-1) : Range(axis,axis);
  Grid = grid==forAll ? Range(0,numberOfComponentGrids-1) : Range(grid,grid);
  Component = component==forAll ? Range (0,numberOfDimensions-1) : Range(component,component); 
    

  velocityBoundaryConditionValueGiven (Grid, Side, Axis, Component) = trueOrFalse;
  if (trueOrFalse)
    velocityBoundaryConditionValue(Grid, Side, Axis, Component) = value;  
}
