#include "Projection.h"

// ===============================================================================
void Projection::
applyUnprojectedVelocityBoundaryConditions( REALCompositeGridFunction & uStar)
  //
  //  /Purpose: extrapolate uStar to the ghost cells on all sides; this is needed in order to 
  //            compute div(u) for the projection. approximate projection case only.
  // ==============================================================================
{

  ProjectionType projType = getProjectionType (uStar);
  real ZERO = 0.0;

  Index Components = Index (0,numberOfDimensions);
  BoundaryConditionParameters bcParams;

  switch (projType)
  {
  case approximateProjection:

    bcParams.orderOfExtrapolation = 3;
  
    uStar.applyBoundaryCondition (Components, BCTypes::extrapolate, BCTypes::allBoundaries, ZERO, ZERO, bcParams);

  //... take care of corners and periodic edges
    uStar.finishBoundaryConditions ();
    break;
    
  default:
    cout << "WARNING: uStar bcs not implemented except for approximateProjection case" << endl;
    break;
  };
    
}

/*
// ****** 981111 DLB: This looks like its dead code (?) *******

// ========================================================================
void Projection::
applyVelocityBoundaryConditions (REALCompositeGridFunction &uStar)
  //
  // private function: this is used to apply boundary conditions to the 
  // projected velocity at the end of the projection. This is a no-op if
  // the projectionType is not approximateProjection, since BCs are unnecessary
  // for the MAC projections.
  // ========================================================================
{
  VelocityBoundaryConditionType vbcType = projectedVelocityBC;
  applyVelocityBCs (uStar, vbcType);
}


// ==============================================================================
void Projection::
applyVelocityBCs (REALCompositeGridFunction &velocity,  VelocityBoundaryConditionType vbcType)
//
// /Purpose:
//   Do the work involved in applying boundary conditions to a velocity vector.
//   The flag {\bf vbcType} is used to determine which boundary condition to apply
//
// ============================================================================== 
{

  if (getProjectionType(velocity) != approximateProjection)
  {
    return;
  }

  if (twilightZoneFlow)
  {
    cout << "Warning: twilightZone option not implemented for velocity BCs" << endl;
  }

// ========================================
// declarations:
// If exact velocity used, allocate space in boundaryConditionRightHandSide array
// ========================================
  
  REAL ZERO = 0.0, HALF = 0.5;
  int grid, side, axis;
  Index I1ghost, I2ghost, I3ghost, I1boundary, I2boundary, I3boundary;
  int bcIndex = 0, bcComponent = 0, velComponent = 0;
  REAL value;

  REALCompositeGridFunction boundaryConditionRightHandSide;
  if (useExactVelocity) 
  {
    boundaryConditionRightHandSide.updateToMatchGrid (compositeGrid,  cellCentered, numberOfDimensions);
    boundaryConditionRightHandSide = ZERO;
  }

// ========================================
//    If an exact velocity function has been set:
//      Make a reference to either the exact velocity or perturbed velocity
//      depending on which VelocityBoundaryConditionType has been set
// ========================================
      REALCompositeGridFunction forcingVelocity;
      
      if (useExactVelocity)
      {
	switch (vbcType)
	{
	case unprojectedVelocityBC:
	  forcingVelocity.reference (perturbedVelocity);
	  break;
	  
	case projectedVelocityBC:
	  forcingVelocity.reference (exactVelocity);
	  break;
	  
	default:
	  cout << "Projection::applyVelocityBCs: useExactVelocity, but invalid VelocityBoundaryConditionType = " << vbcType << endl;
	  assert (FALSE);
	};
      } 

// ========================================
// First set the BC types:
//
// ========================================
  
// Loop over all grids:
  ForAllGrids (grid)
  {
    MappedGrid & mg = compositeGrid[grid];

// ========================================
//  Loop over all boundaries:
// ========================================

    ForBoundary (side,axis)
    {
      int cmpgrdBC = mg.boundaryCondition()(side,axis);
      int numberOfBoundaryConditions;

// ========================================
//    If an exact velocity function has been set:
//      get Index'es for the boundary and first ghost row along the boundary to use later
// ========================================

      if (useExactVelocity)
      {
	getGhostIndex    (mg.indexRange(), side, axis, I1ghost, I2ghost, I3ghost, +1);
	getBoundaryIndex (mg.indexRange(), side, axis, I1boundary, I2boundary, I3boundary);
      }

      if (cmpgrdBC > 0)
      {
// ========================================
//    Depending on boundary condition type:
//      Set the number of boundary conditions.
//      Set each boundary condition
//      Set the forcing for each boundary condition:
//        if an exact velocity has been set:
//          use the exactVelocity function to get the forcing values for projected velocities
//          use the perturbedVelocity function to get the forcing values for unprojected velocities
//        else
//          magically get the forcing values from somewhere else (?)
//        endif
// ========================================

        switch (listOfVelocityBCs[cmpgrdBC])
	{
	  case cheapNeumann:
            numberOfBoundaryConditions = numberOfDimensions;
            operatorsForVelocity.setNumberOfBoundaryConditions (numberOfBoundaryConditions);
	    for (bcIndex=0; bcIndex<numberOfDimensions; bcIndex++)
	    {
	      bcComponent = bcIndex;
	      velComponent = bcIndex;
	      ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryCondition (bcIndex, side, axis, MappedGridFiniteVolumeOperators::neumann, bcComponent);
	      if (useExactVelocity)
	      {
		boundaryConditionRightHandSide[grid](I1ghost,I2ghost,I3ghost,bcComponent) = 
		  forcingVelocity[grid](I1boundary,I2boundary,I3boundary,velComponent) - forcingVelocity[grid](I1ghost,I2ghost,I3ghost,velComponent);
	      }
	      else
	      {
               ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryConditionValue (ZERO, bcComponent, bcIndex, side, axis);
	      }
	    }
	  
	    break;
	  
	  case normalDerivativeGiven:  
	    // ========================================
	    // later on we could allow a value to be specified here...
	    // ========================================
            numberOfBoundaryConditions = numberOfDimensions;
            operatorsForVelocity.setNumberOfBoundaryConditions (numberOfBoundaryConditions);
	    for (bcIndex=0; bcIndex<numberOfDimensions; bcIndex++)
	    {
	      bcComponent = bcIndex;
	      velComponent = bcIndex;
  	      ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryCondition (bcIndex, side, axis, MappedGridFiniteVolumeOperators::neumann, bcComponent);
	      if (useExactVelocity)
	      {
		boundaryConditionRightHandSide[grid](I1ghost,I2ghost,I3ghost,bcComponent) = 
		  forcingVelocity[grid](I1boundary,I2boundary,I3boundary,velComponent) - forcingVelocity[grid](I1ghost,I2ghost,I3ghost,velComponent);
	      }
	      else
	      {
  	        ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryConditionValue (ZERO, bcComponent, bcIndex, side, axis); 
	      }
	    }
	    
	    break;

	  case valueGiven:  
            numberOfBoundaryConditions = numberOfDimensions;
            operatorsForVelocity.setNumberOfBoundaryConditions (numberOfBoundaryConditions);
	    for (bcIndex=0; bcIndex<numberOfDimensions; bcIndex++)
	    {
	      bcComponent = bcIndex;
	      velComponent = bcIndex;
	      
 	      ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryCondition (bcIndex, side, axis, MappedGridFiniteVolumeOperators::dirichlet, bcComponent);
	      if (useExactVelocity)
	      {
		boundaryConditionRightHandSide[grid](I1ghost,I2ghost,I3ghost,bcComponent) =
		  HALF*(forcingVelocity[grid](I1boundary,I2boundary,I3boundary,velComponent) 
			+ forcingVelocity[grid](I1ghost,I2ghost,I3ghost,velComponent));
	      }
	      else if (velocityBoundaryConditionValueGiven (grid, side, axis, bcComponent))
	      {
		value = velocityBoundaryConditionValue (grid, side, axis, bcComponent);
		((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryConditionValue (value, bcComponent, bcComponent, side, axis, TRUE);
	      }
	      else
	      {
	        ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryConditionValue (ZERO, bcComponent, bcIndex, side, axis); 
	      }
	    }
	    
	    break;
	    
	  case normalValueGiven: //  later on we could allow a value to be specified here..
            //
            // first extrapolate all variables; then apply normal value BC
	    //
            numberOfBoundaryConditions = numberOfDimensions+1;
            operatorsForVelocity.setNumberOfBoundaryConditions (numberOfBoundaryConditions);

	    for (bcIndex=0; bcIndex<numberOfDimensions; bcIndex++)
	    {
	      bcComponent = bcIndex;
  	      ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryCondition (bcIndex, side, axis, MappedGridFiniteVolumeOperators::extrapolate, bcComponent);
	    }
            bcIndex = numberOfDimensions;
	    ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryCondition (bcIndex, side, axis, MappedGridFiniteVolumeOperators::normalComponent);
	    if (useExactVelocity)
	    {
	      cout << "***WARNING***" << endl;
	      cout << "Projection::applyVelocityBoundaryConditions: forcing not implemented for case normalValueGiven" << endl;
	      cout << "***WARNING***" << endl;
	    }
	    else
	    {
  	      ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryConditionValue (ZERO, bcComponent, bcIndex, side, axis);
	    }
	    
	    break;

          case extrapolateAllComponents:
            numberOfBoundaryConditions = numberOfDimensions;
            operatorsForVelocity.setNumberOfBoundaryConditions (numberOfBoundaryConditions);

	    for (bcIndex=0; bcIndex<numberOfDimensions; bcIndex++)
	    {
	      bcComponent = bcIndex;
  	      ((MappedGridFiniteVolumeOperators&)(operatorsForVelocity[grid])).setBoundaryCondition (bcIndex, side, axis, MappedGridFiniteVolumeOperators::extrapolate, bcComponent);
	    }

	  }; // End depending on bc type
      } //  End if cmpgrdBC > 0
    } //  End loop over boundaries
  } // End loop over grids
  
// ========================================
// The boundary conditions are actually applied here
// ========================================    

  if (useExactVelocity) operatorsForVelocity.setBoundaryConditionRightHandSide (boundaryConditionRightHandSide);
  
  operatorsForVelocity.applyBoundaryConditions (velocity);
  
//  operatorsForVelocity.finishBoundaryConditions (velocity);

  if (projectionDebug) projectionDisplay.display (velocity, "Projection::applyVelocityBCs: velocity at end of routine");

}

*/
