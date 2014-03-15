#include "Projection.h"
#include <SparseRep.h>  // *** need this to print classify array out.
#include "makeDebugPlots.h"
//=================================================================================
//\begin{>>projectionDoc.tex}{\subsection{project}}   
void Projection::
project (realCompositeGridFunction & uStar,
	 realCompositeGridFunction & phi0,
	 const int & level, // = 0
	 const real & velocityTime, // = 0
	 const real & pressureTime) // = 0.
//
// /Purpose:
//  Compute regular or "MAC"  projection potential phi0. A call to this function must
//     be preceeded by a call to  {\bf Projection::updateToMatchGrid}, and in the case
//     of a variable density projection, a call to {setDensity}. NOTE: uStar is not
//     const because the ghost cell values will be changed before projecting.
//   
//  Choice of projection depends on the centering of the input array uStar. If 
//  uStar is
//  \begin{itemize}
// 
//  \item {\bf cellCentered}: then a regular projection is used,
//  \item {\bf faceCenteredAllWith0Components}: it is assumed to hold normal velocities on faces,
//  \item {\bf faceCenteredAllWith1Component}: it is assumed to hold faceCentered velocities.
//
//  \end{itemize} 
//
//   
// /uStar (compositeGrid, cellCentered):                         (regular projection)
// /uStar (compositeGrid, faceCenteredAll, numberOfDimensions):     (MAC projection of faceCentered velocities)
// /uStar (compositeGrid, faceCenteredAll):                        (MAC projection of normal velocities)
//                                the input velocity field
// /level: optional argument; in the moving grid case this specifies which (grid) level
//         to evaluate the moving grid boundary velocities at for the elliptic problem boundary conditions
// /velocityTime: optional argument: only needed for the twilightZone case; this is the time at
//                which the (output) projected velocity is centered
// /pressureTime: optional argument: only needed for the twilightZone case; this is the time at
//                which the (output) pressure is centered. Note that is typically not the same time level
//                as the projected velocity.
//
// /phi0 (compositeGrid, cellCentered):    (returned) projection potential
//
// /Author:				D.L.Brown
//
//\end{projectionDoc.tex} 
//================================================================================

{
  bool PLOT_ON0 = FALSE;
  project (uStar, phi0, NULL, NULL, PLOT_ON0, level);
}
  
//=================================================================================
void Projection::
project (
	 realCompositeGridFunction & uStar,
	 realCompositeGridFunction & phi0,
	 GenericGraphicsInterface * ps,
	 GraphicsParameters * psp,
	 bool & PLOT_ON,
	 const int & level,
	 const real & velocityTime,
	 const real & pressureTime
	 )
//=================================================================================
{
  setPhi (uStar, ps, psp, PLOT_ON, level, velocityTime, pressureTime);

  if (&phi0 != &phi) phi0 = phi;


//  cout << "phi.getGridFunctionTypeWithComponents = " << phi.getGridFunctionTypeWithComponents() << " before function return" << endl;
//  cout << "phi0.getGridFunctionTypeWithComponents = " << phi0.getGridFunctionTypeWithComponents() << " before function return" << endl;
  return;
}
  
//==============================================================================
void Projection::
setPhi (
	realCompositeGridFunction & uStar,
	GenericGraphicsInterface * ps,
	GraphicsParameters * psp,
	bool & PLOT_ON,
	const int & level,
	const real & velocityTime,
	const real & pressureTime
	)
//
// /Purpose:
//   Private class function that sets up the elliptic problem, and solves for
//   phi.
// /981105: Major cleanup of code
//
//==============================================================================
{





  if (twilightZoneFlow) assert (twilightZoneFlowFunction != NULL);

  cout << "Projection::setPhi called with velocityTime = " 
       << velocityTime 
       << ", and pressureTime = " 
       << pressureTime 
       << endl;

  realCompositeGridFunction ellipticRightHandSide (compositeGrid, defaultCentering);
  ellipticRightHandSide = 0.;

  Index I1m, I2m, I3m;
  Index I1, I2, I3;
  Index J1, J2, J3;
  

// =================================================================
//Interrogate uStar:      if (uStar.iscellCentered) isCellCentered = TRUE
//		 	if (uStar.getFaceCentering = all && uStar.getComponentDimension(1) == 3) isFaceCentered = TRUE;
//			if (uSTar.getFaceCentering = all && uStar.getComponentDimension(1) == 1) isNormalVelocity = TRUE;
//			if not one of these, return with an error
// =================================================================

  ProjectionType projectionType = getProjectionType (uStar);

  if (projectionType == macProjection) cout << "Projection::project: MAC projection..." << endl;
  if (projectionType == macProjectionOfNormalVelocity) cout << "Projection::project: MAC projection of normal velocities " << endl;
  if (projectionType == approximateProjection) cout << "Projection::project: Regular projection ..." << endl;
  if (projectionType == noProjection)
    throw "Projection::project: input uStar not in a recognized form ";

  if (reinitializeCoefficients) 
  {
    formLaplacianCoefficients();
    reinitializeCoefficients = LogicalFalse;
  }
    
  // =============================
  // interior of the RHS is divergence of input velocity
  // =============================


  formEllipticRightHandSide (ellipticRightHandSide, uStar, level, velocityTime, pressureTime,
			     ps, psp, PLOT_ON);

  // ========================================
  // Solve elliptic equation to get phi
  // ========================================  

  ellipticSolver.solve (phi, ellipticRightHandSide);
 
 if (twilightZoneFlow) checkEllipticConstraint (phi);

  //...setRefactor to FALSE so elliptic solver won't factor matrix again by default
  ellipticSolver.setRefactor (FALSE);

//  phi.periodicUpdate();
  
  if (projectionDebug) projectionDisplay.display (phi, "Projection::project: here is the elliptic solution");
  if (PLOT_ON) makeDebugContourPlots (phi, "elliptic solution, phi", 0, ps, psp);
}


//================================================================================
void Projection::
checkEllipticConstraint (const realCompositeGridFunction & phi_)
{
  //...check to make sure that the solution satisfies the constraint in the TZ case
  Index I1,I2,I3;
  
  real checkValue = (real)0.0;
  
  if (ellipticCompatibilityConstraintSet)
  {
    int grid;
    ForAllGrids (grid)
    {
      getIndex (compositeGrid[grid].dimension(),I1,I2,I3);
      checkValue += sum (ellipticSolver.rightNullVector[grid](I1,I2,I3) * phi_[grid](I1,I2,I3));
    }
    cout << "Solution average value is " << checkValue << endl;
  }
}
  

// ========================================================================================
//\begin{>>projectionDoc.tex}{\subsection{project}}  
realCompositeGridFunction Projection::
project (realCompositeGridFunction & uStar,
	 const int & level, // = 0
	 const real & velocityTime, // = undefinedRealValue
	 const real & pressureTime) // = undefinedRealValue
//
// /Purpose:
//
//  Project uStar to an approximately divergence free field using a regular
//  or "MAC" projection. A call to this function must be preceeded by a
//  call to {\bf Projection::updateToMatchGrid}, and in the case of a variable
//  density projection, a call to {\bf setDensity}. 
//
//  Choice of projection depends on the centering of the input array uStar
//  If uStar is cellCentered, then a regular projection is used
//  If uStar is faceCenteredAllWith0Components, it is assumed to hold normal velocities on faces
//  If uStar is faceCenteredAllWith1Component, it is assumed to hold faceCentered velocities
//
//   
// /uStar (compositeGrid, cellCentered):                         (regular projection)
// /uStar (compositeGrid, faceCenteredAll, numberOfDimensions):     (MAC projection of faceCentered velocities)
// /uStar (compositeGrid, faceCenteredAll):                        (MAC projection of normal velocities)
//                                the input velocity field
// /level: optional argument; in the moving grid case this specifies which (grid) level
//         to evaluate the moving grid boundary velocities at for the elliptic problem boundary conditions
// /velocityTime: optional argument: only needed for the twilightZone case; this is the time at
//                which the (output) projected velocity is centered
// /pressureTime: optional argument: only needed for the twilightZone case; this is the time at
//                which the (output) pressure is centered. Note that is typically not the same time level
//                as the projected velocity.

// /project: returned value is the projected velocity, dimensioned in the same way as the input velocity
//
// /Author:				D.L.Brown
//\end{projectionDoc.tex}  
//===============================================================================
{
  bool PLOT_ON0 = FALSE;
  return (project (uStar, NULL, NULL, PLOT_ON0, level, velocityTime, pressureTime));
}
  

// ================================================================================
realCompositeGridFunction Projection::
project (
		realCompositeGridFunction & uStar,
		GenericGraphicsInterface * ps,
		GraphicsParameters * psp,
		bool & PLOT_ON,
		const int & level,
		const real & velocityTime,
		const real & pressureTime
		)

{
  realCompositeGridFunction returnedValue;
  real HALF = (real) 0.5;
  real ZERO = (real) 0.0;
  


  // =================================================================
  //Interrogate uStar:  if (uStar.iscellCentered) isCellCentered = TRUE
  //		 	if (uStar.getFaceCentering = all && uStar.getComponentDimension(1) == 3) isFaceCentered = TRUE;
  //			if (uSTar.getFaceCentering = all && uStar.getComponentDimension(1) == 1) isNormalVelocity = TRUE;
  //			if not one of these, return with an error
  // =================================================================

  ProjectionType projectionType = getProjectionType (uStar);

  returnedValue.updateToMatchGridFunction (uStar);


  // ========================================
  // Solve elliptic equation for the projection potential
  // ========================================

  setPhi (uStar, ps, psp, PLOT_ON, level, velocityTime, pressureTime);

  // =============================
  // Compute velocity correction = grad(phi)
  // =============================

  realCompositeGridFunction velocityCorrection;
  velocityCorrection = ZERO;

  GridFunctionParameters::GridFunctionTypeWithComponents uStarType = uStar.getGridFunctionTypeWithComponents();
  GridFunctionParameters::GridFunctionTypeWithComponents vcType    = velocityCorrection.getGridFunctionTypeWithComponents();
  GridFunctionParameters::GridFunctionTypeWithComponents rvType    = returnedValue.getGridFunctionTypeWithComponents();
  GridFunctionParameters::GridFunctionTypeWithComponents phiType   = phi.getGridFunctionTypeWithComponents(); 
  
  int component;
  
  switch (projectionType) 
  {
  case macProjection:
  
    velocityCorrection = phi.FCgrad();

      if (isVariableDensityProjection)      // velocityCorrection /= density;
      {
        int grid; Index J1, J2, J3; int extra = -1;
        ForAllGrids (grid)
        {
	  for (component=0; component<numberOfDimensions; component++)
	  {

            int face;
	    ForAxes (face)
	    {
  	      getIndex (compositeGrid[grid].dimension(), J1, J2, J3, extra);
	      where (compositeGrid[grid].mask()(J1,J2,J3) != 0)
	      {
	        velocityCorrection[grid](J1,J2,J3,component,face) *= 
	          HALF*(1./density[grid](J1,J2,J3) + 1./density[grid](J1-inc(0,face),J2-inc(1,face),J3-inc(2,face)));
	      }
            }
	  }
        }    
        if (projectionDebug) projectionDisplay.display (velocityCorrection, "here is velocityCorrection = grad(phi)");
      }
      break;
      
    case approximateProjection:

      phiType = phi.getGridFunctionTypeWithComponents ();
//      cout << "phi.getGridFunctionTypeWithComponents () = " << phi.getGridFunctionTypeWithComponents() << " before grad comp." << endl;
      velocityCorrection = phi.grad();
      
      vcType    = velocityCorrection.getGridFunctionTypeWithComponents();
      if (isVariableDensityProjection)  // Velocitycorrection /= density;
      {
        int grid; Index J1, J2, J3;
        ForAllGrids (grid)
        {
	  for (component=0; component<numberOfDimensions; component++)
	  {
	    getIndex (compositeGrid[grid].dimension(), J1, J2, J3);
	    where (compositeGrid[grid].mask()(J1,J2,J3) != 0)
	    {
	      velocityCorrection[grid](J1,J2,J3,component) /= density[grid](J1,J2,J3);
	    }
	  }
	}
      }   
      vcType    = velocityCorrection.getGridFunctionTypeWithComponents();
      
      break;
      
    case macProjectionOfNormalVelocity:
      
      realCompositeGridFunction temp;
// ***OP
//      temp = operators.FCgrad (phi);
      temp = phi.FCgrad();

//      velocityCorrection = operators.normalVelocity (temp);
      velocityCorrection = temp.normalVelocity();

      if (isVariableDensityProjection)      // velocityCorrection /= density;
      {
        int grid; Index J1, J2, J3; int extra = -1;
        ForAllGrids (grid)
        { 
          realMappedGridFunction averageInverseDensity(compositeGrid[grid],all,all,all);
	  for (component=0; component<numberOfDimensions; component++)
	  {
	    getIndex (compositeGrid[grid].dimension(), J1, J2, J3, extra);
	    where (compositeGrid[grid].mask()(J1,J2,J3) != 0)
	    {
              averageInverseDensity(J1,J2,J3) = 
	      HALF*(1./density[grid](J1,J2,J3) + 1./density[grid](J1-inc(0,component),J2-inc(1,component),J3-inc(2,component)));
	      velocityCorrection[grid](J1,J2,J3,component) *= averageInverseDensity(J1,J2,J3);
	    }
	  }
	}
      }
      break;
    };
  
    
  if (PLOT_ON) makeDebugContourPlots (velocityCorrection, "grad phi; component 0", 0, ps, psp);
  if (PLOT_ON) makeDebugContourPlots (velocityCorrection, "grad phi; component 1", 1, ps, psp);
  
		    
    if (projectionDebug) projectionDisplay.display (velocityCorrection, "here is velocityCorrection = grad(phi)");
   
  // =============================
  // Correct the velocity
  // =============================

  if (projectionDebug) projectionDisplay.display (uStar, "uStar before correction");
  if (projectionDebug) projectionDisplay.display (velocityCorrection, "velocityCorrection before correction");


   uStarType = uStar.getGridFunctionTypeWithComponents();
   vcType    = velocityCorrection.getGridFunctionTypeWithComponents();
   rvType    = returnedValue.getGridFunctionTypeWithComponents();
  
  returnedValue = uStar - velocityCorrection;

  rvType     = returnedValue.getGridFunctionTypeWithComponents();
  
  returnedValue.periodicUpdate ();

  if (projectionDebug) projectionDisplay.display (returnedValue, "returnedValue before applying BCs");

//...960130: returnedValue will not have any BCs applied to it
//  applyVelocityBoundaryConditions (returnedValue);

  if (PLOT_ON) makeDebugContourPlots (returnedValue, "uStar[0] after correction and before interp", 0, ps, psp);

  //... Its not reasonable to interpolate faceCentered or normal velocities; just interpolate if approximateProjection 951108

  if (projectionType == approximateProjection) returnedValue.interpolate();

  if (PLOT_ON) makeDebugContourPlots (returnedValue, "project: projected velocity", 0, ps, psp);
  
  return (returnedValue);
}


// ================================================================================
void Projection::
formEllipticRightHandSide ( realCompositeGridFunction & ellipticRightHandSide,
			    realCompositeGridFunction & uStar,
			    const int & level,
			    const real & velocityTime,
			    const real & pressureTime,
			    GenericGraphicsInterface * ps,
			    GraphicsParameters * psp,
			    bool & PLOT_ON
  )
//
// /Purpose: private function to form the elliptic RHS for the projection.
//           takes into account details for TwilightZone case and moving grids
//
// /DLB 981105
//================================================================================
{

  real ZERO = 0.0;
  Index I1, I2, I3;
  OGFunction *e = twilightZoneFlowFunction;
  ProjectionType projectionType = getProjectionType (uStar);

  applyUnprojectedVelocityBoundaryConditions (uStar);

// ... if this is a twilightZone case, we need to know the grid for the next level, since
//     the TZ function must be evaluated.

  CompositeGrid nextLevelCompositeGrid;
  CompositeGrid* gridPointer;
  
  if (twilightZoneFlow) // *** its possible that we could have x-dependent BCs in which case need to do this in general
  {
    // ... if movingGridsPointer is not set, presume the grid is not moving and use the same grid
    if (!movingGridsPointerIsSet){
      nextLevelCompositeGrid.reference (compositeGrid);
    }
    
    // ... otherwise, get the grid from the movingGrids object
    else
    {
      int nextLevel = level+1;  // *** is this correct?

      gridPointer = movingGridsPointer->movedGrid (nextLevel);
      nextLevelCompositeGrid.reference (*gridPointer);
    }

    // ... some debug stuff
    realCompositeGridFunction exVelocity (nextLevelCompositeGrid);
    exVelocity = (*e)(nextLevelCompositeGrid, xComponent, velocityTime);
    if (projectionDebug) projectionDisplay.display (exVelocity, "Pr: exact velocity at the new time on new grid");

    realMappedGridFunction xNew;
    xNew.link (nextLevelCompositeGrid[0].center(), Range(xComponent,xComponent));
    if (projectionDebug) projectionDisplay.display (xNew, "Pr: new grid values of x");
    
  }

// ... set RHS interior
  
  switch (projectionType)
  {
  case approximateProjection:
  case macProjection: 

    ellipticRightHandSide = uStar.div();

    // ... for twilightZoneFlow, subtract div(Uexact) from RHS
    if (twilightZoneFlow)
    {
      if (projectionDebug) projectionDisplay.display 
			     (ellipticRightHandSide, 
			      "Projection:project::Here is ellipticRightHandSide before twilightZone forcing and BCs");

      switch (numberOfDimensions)
      {

	case (oneDimension):
	ellipticRightHandSide -= e->x(nextLevelCompositeGrid, xComponent, velocityTime);
	break;
	

	case (twoDimensions):
	ellipticRightHandSide -= 
	  e->x(nextLevelCompositeGrid, xComponent, velocityTime) +
	  e->y(nextLevelCompositeGrid, yComponent, velocityTime);
	break;
	
	case (threeDimensions):
	ellipticRightHandSide -= 
	  e->x(nextLevelCompositeGrid, xComponent, velocityTime) +
	  e->y(nextLevelCompositeGrid, yComponent, velocityTime) +
	  e->z(nextLevelCompositeGrid, zComponent, velocityTime);
	break;
      };

      // ... if twilightZoneFlow and singular problem, RHS of compatibility equation must be set

      if (ellipticCompatibilityConstraintSet)
      {
	if (!timestepIsSet)
	{
	  cout << "Projection::setPhi: please call setTimestep before using TwilightZoneFlow option" << endl;
	  throw "Projection:error";
	}
	if (isIncrementalPressureFormulation)
	{
	  if (previousTime == undefinedRealValue)
	  {
	    cout << "Projection::setPhi: please set previous pressure time level by calling setPreviousPressureTimeLevel" << endl;
	    throw "Projection:error";
	  }
	}
	
	int ne,i1e,i2e,i3e,gride, grid;
	//...assume that the pressure is the last variable in the TZ function
	int pComponent = numberOfDimensions;
	ellipticSolver.equationToIndex (ellipticSolver.extraEquationNumber(0), ne,i1e,i2e,i3e,gride);
	ellipticRightHandSide[gride](i1e,i2e,i3e) = ZERO;
        projectionDisplay.display (ellipticSolver.rightNullVector, "rightNullVector");
	cout << "Projection.project: setting pressure constant for pressure at time " << pressureTime << endl;
	
	ForAllGrids (grid)
	  {
	    projectionDisplay.display (coefficients[grid].sparse->classify, "classify array");
	    
	    getIndex (compositeGrid[grid].dimension(),I1,I2,I3);
	    if (isIncrementalPressureFormulation)
	    {
	      // *** could this be a problem with the moving grid case since the grids are not the same at the two time levels?
	    ellipticRightHandSide[gride](i1e,i2e,i3e) += timestep *
	      sum (ellipticSolver.rightNullVector[grid](I1,I2,I3) * 
		   ((*e)(compositeGrid[grid],I1,I2,I3,pComponent,pressureTime) - (*e)(compositeGrid[grid],I1,I2,I3,pComponent,previousTime)));
	    }
	    else
	    {
	    ellipticRightHandSide[gride](i1e,i2e,i3e) += timestep *
	      sum (ellipticSolver.rightNullVector[grid](I1,I2,I3) * (*e)(compositeGrid[grid],I1,I2,I3,pComponent,pressureTime));
	    }
	    
	  }
	cout << "TwilightZoneFlow: compatibility constant is" << ellipticRightHandSide[gride](i1e,i2e,i3e) << endl;
      }
    }
    
    
    break;
    
  case macProjectionOfNormalVelocity:

    ellipticRightHandSide = uStar.divNormal();

    break;
    
  };

  if (projectionDebug) projectionDisplay.display (ellipticRightHandSide, 
						  "Projection:project::Here is ellipticRightHandSide before  BCs");

  //... set RHS BCs

  applyRightHandSideBoundaryConditions (ellipticRightHandSide, uStar, level, velocityTime, pressureTime);

  if (projectionDebug) projectionDisplay.display (ellipticRightHandSide, "Projection:project::Here is ellipticRightHandSide after BCs");
  if (PLOT_ON) makeDebugContourPlots (ellipticRightHandSide, "ellipticRightHandSide", 0, ps, psp);
  
}
