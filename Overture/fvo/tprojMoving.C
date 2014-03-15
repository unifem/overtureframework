inline int coeff(int i,int j)           {return (i+1) + 3*(j+1);}

//========================================
// tprojTZ.C
//
// Author:		D.L.Brown
// Date Created:	981123
// Date last modified:  
//
// Purpose:
//	Test TwilightZone routines in Projection Class
//
// Interface: (inputs)
//
// Interface: (output)
//
//========================================

#include "Overture.h"
#include "GenericGraphicsInterface.h"
#include "OGgetIndex.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "Oges.h"
#include "davidsReal.h"
#include "axisDefs.h"
#include "loops.h"
#include "MappedGridFiniteVolumeOperators.h"
#include "CompositeGridFiniteVolumeOperators.h"
#include "billsMergeMacro.h"
#include "Projection.h"
#include "makeDebugPlots.h"
#include "TwilightZoneWizard.h"
#include "Ogen.h"
//MovingGrids related stuff
#include "MatrixTransform.h"
#include "MatrixTransformGridMotion.h"
#include "MatrixTransformGridMotionParameters.h"
#include "DynamicMovingGrids.h"
#include "MatrixTransformMotionFunction.h"
#include "testUtils.h"


#undef BOUNDS_CHECK
#define BOUNDS_CHECK	//A++ bounds check on

//... include some useful utilities (norms, etc.)
#include "testUtils.h"

int 
main (int args, char **argv)
{
  Index::setBoundsCheck(on);

  // ======================================== 
  // Declarations
  // ========================================
  
  Index J1, J2, J3;
  int grid;

  // ========================================
  // Print Banner
  // ========================================
  
  printf ("\n========================================\n");
  printf ("  TPROJMOVING: test Projection \n");
  printf ("    Class MovingGrid features  \n");
  printf ("========================================\n\n");

	// ========================================
	// Synchronize C++ and C I/O Subsystems
	// ========================================

  ios::sync_with_stdio();

	// ========================================
	// These default values need to be defined
	// ========================================

  static const Index nullIndex;
  static const Range all;
  int forAll = (int) MappedGridFiniteVolumeOperators::forAll;

  MappedGridFiniteVolumeOperators::boundaryConditionTypes dirichlet = MappedGridFiniteVolumeOperators::dirichlet;
  real pi = 3.1415927;

	// ========================================
	// A++ array bounds checking on
	// ========================================

  Index::setBoundsCheck(on);

        // =======================================
        // Some control variables; input parameters here
        // =======================================

  Projection::ExactVelocityType exactVelocityType = Projection::shearLayers;
  while (!(exactVelocityType == Projection::noExactVelocity || exactVelocityType == Projection::zeroExactVelocity))
  {
    cout << endl << "<<<Choose either noExactVelocity or zeroExactVelocity for TZ tests>>> " << endl;
    exactVelocityType = chooseExactVelocityType();
  }
  
   
  MappedGridFiniteVolumeOperators::debug = setBoolParameter ("Turn on DEBUG in MappedGridFiniteVolumeOperators class? [n]: ");
  real epsilon = setFloatParameter ("Enter velocity divergence perturbation size for projection: ");

	// ========================================
	// Find grids to use
	// ========================================


  CompositeGrid cg;
  int numberOfGrids, numberOfDimensions;
  requestGrid (cg, numberOfGrids, numberOfDimensions);
  cg.update();

  Index velocityComponents (0,numberOfDimensions);


  // ========================================
  // Make interpolant
  // ========================================

  Interpolant interpolant (cg);
  
  // ========================================
  // GenericGraphicsInterface Object
  // ========================================

  bool openWindow = FALSE;
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("Your slogan here", openWindow);
  GraphicsParameters psp;

  bool PLOT_ON = setBoolParameter("Plotting on? ");
  if (PLOT_ON) ps.createWindow("tprojMoving: Projection class test code");  
  if (PLOT_ON) ps.plot (cg);         //plot the composite grid	  

  // ========================================
  // Moving Grid Stuff
  // ========================================

  int  numberOfLevels=2;

  cout << "Enter moving grid information: " << endl;
  float timestep    = setFloatParameter ("Enter timestep: ");
  int numberOfSteps = setIntParameter ("Enter numberOfSteps: ");
  
  Ogen gridGenerator (ps);

  MatrixTransformGridMotionParameters mtParams (numberOfDimensions, numberOfGrids);
  mtParams.setupWizard ();
  MatrixTransformGridMotion movingMappings (cg, numberOfLevels, &mtParams);

  DynamicMovingGrids movingGrids;
  movingGrids.initialize (cg, &movingMappings, &gridGenerator);
  CompositeGrid* currentGrid;

  // ========================================
  // Create CompositeGridFunctions
  // ========================================
  
  realCompositeGridFunction velocity              (cg, GridFunctionParameters::defaultCentering, numberOfDimensions);
  realCompositeGridFunction projectedVelocity     (cg, GridFunctionParameters::defaultCentering, numberOfDimensions);
  realCompositeGridFunction trueProjectedVelocity (cg, GridFunctionParameters::defaultCentering, numberOfDimensions);
  realCompositeGridFunction projectedVelocityError(cg, GridFunctionParameters::defaultCentering, numberOfDimensions);
  realCompositeGridFunction divergence            (cg, GridFunctionParameters::cellCentered);
  
  CompositeGridFiniteVolumeOperators ops (cg);

  velocity.setOperators (ops);
  projectedVelocity.setOperators (ops);

  // ========================================
  // Initialize TZ function
  // ========================================

  OGFunction* twilightZoneFunction = setTwilightZoneFlowFunction (numberOfDimensions);
  
  // ==========================================
  // Setup projection class
  // ==========================================
  
  Display projDisplay;
  Display::cellCenteredDisplayOption = LogicalTrue;
  
  projDisplay.interactivelySetInteractiveDisplay("projDisplay initialization");
  
  Projection::projectionDebug = LogicalTrue; 
  Oges::debug = 0;

  Projection projection (cg, &ops, &movingGrids);
  projection.ellipticSolverParameterWizard();
  projection.boundaryConditionWizard ();
  if (twilightZoneFunction) projection.setTwilightZoneFlow ();
  projection.setTwilightZoneFlowFunction (twilightZoneFunction);

  bool PLOT_ON_THIS;
  if (PLOT_ON) PLOT_ON_THIS = setBoolParameter ("Plot velocity function and Projection class debug info? ");


  real time = 0., tU = 0., tP = 0.;
  int oldLevel = 0, newLevel = 1;

  //
  // ... timestep iteration.
  //     
  for (int it=1; it<=numberOfSteps; it++)
  {
    cout << "************" << endl;
    cout << "** it = " << it << endl;
    cout << "************" << endl;

    time += timestep;

    //...move the grids
    cout << " ... moving the grids" << endl;

    movingGrids.updateMovedGrids (time, timestep);

    currentGrid = movingGrids.movedGrid (newLevel);
    if (PLOT_ON)
    {
      psp.set (GI_TOP_LABEL, "moved grid");
      ps.plot (*currentGrid, psp);
    }
    
    //...since grid has changed, update everything in sight
    cout << " ... updating objects and gridfunctions to match new grid " << endl;

    ops.updateToMatchGrid                   (*currentGrid);
    interpolant.updateToMatchGrid           (*currentGrid);
    projection.updateToMatchGrid            (*currentGrid);

    velocity.updateToMatchGrid              (*currentGrid);
    projectedVelocity.updateToMatchGrid     (*currentGrid);
    trueProjectedVelocity.updateToMatchGrid (*currentGrid);
    projectedVelocityError.updateToMatchGrid(*currentGrid);
    divergence.updateToMatchGrid            (*currentGrid);

    velocity = 0.;
    projectedVelocity = 0.;
        
    //...simulate updating the velocity
    cout << " ... update the velocity " << endl;
    projection.setPerturbedVelocity (exactVelocityType, epsilon);
    velocity = projection.getPerturbedVelocity ();
    trueProjectedVelocity = projection.getExactVelocity ();

    //...some output
    if (PLOT_ON_THIS)
    {
      makeDebugContourPlots    (velocity, "initial u", velocityComponents, &ps, &psp);
      makeDebugStreamLinePlots (velocity, "initial velocity", &ps, &psp);
    } 

    projDisplay.display (velocity, "Here is the velocity");
    projDisplay.display (trueProjectedVelocity, "Here is the true projected velocity");
    divergence = velocity.div();

    if (PLOT_ON_THIS)
    {
      makeDebugContourPlots    (divergence, "divergence of initial velocity", 0, &ps, &psp);
    } 
  

    //...project the new velocity and apply boundary conditions
    projection.setTimestep (timestep);
    tU = time; 
    tP = time - timestep;

    cout << " ...project the velocity" << endl;

    projectedVelocity = projection.project (velocity, &ps, &psp, PLOT_ON_THIS, oldLevel, tU, tP);

    applyExactBoundaryConditions (projectedVelocity, cg, projection);

    projDisplay.display (projectedVelocity, "Here is the Projected velocity after BCs");
    projDisplay.display (trueProjectedVelocity, "Here isthe true Projected velocity");
    projectedVelocityError = projectedVelocity - trueProjectedVelocity;
    projDisplay.display (projectedVelocityError, "Here is the projected velocity error");

    cout << "Maximum projected velocity error" << endl;
    printMaxNormOfVelocity (projectedVelocityError, cg);

    if (PLOT_ON)
    {
      makeDebugStreamLinePlots (projectedVelocity, "projected velocity", &ps, &psp);
      makeDebugContourPlots    (projectedVelocity, "projected velocity", velocityComponents, &ps, &psp);

      makeDebugStreamLinePlots (projectedVelocityError, "projected velocity error", &ps, &psp);
      makeDebugContourPlots    (projectedVelocityError, "projected velocity error", velocityComponents, &ps, &psp);

      divergence = projectedVelocity.div();
      makeDebugContourPlots    (divergence, "divergence of projected velocity", 0, &ps, &psp);
    } 


  } //end for(it...)
  
  if (twilightZoneFunction) delete twilightZoneFunction;
  
}
