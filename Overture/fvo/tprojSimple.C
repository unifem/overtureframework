inline int coeff(int i,int j)           {return (i+1) + 3*(j+1);}

//========================================
// tprojSimple.C
//
// Author:		D.L.Brown
// Date Created:	951101
// Date last modified:  981022
//
// Purpose:
//	Test Projection class
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
  real timestep;

  // ========================================
  // Print Banner
  // ========================================
  
  printf ("\n========================================\n");
  printf ("  TPROJSIMPLE: test Projection \n");
  printf ("                 Classe  \n");
  printf ("========================================\n\n");

	// ========================================
	// Check argument line parameters
	// ========================================

  int ierr = 0;
  if (args < 2)
    {
      cerr << "Usage: tproject.x CMPGRD_datafile:" << endl;
      ierr = -1;
      return (ierr);
    }
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

	// ========================================
	// hardwire control of some output routines
	// ========================================

  aString yes = "y";
  
  bool testDebug = FALSE;

  aString nameOfOGFile 		= argv[1];
  cout << "tproject.x: Opening " << nameOfOGFile << " ... " << endl;


        // =======================================
        // Some control variables; input parameters here
        // =======================================

  Projection::ExactVelocityType exactVelocityType = chooseExactVelocityType();
   
  MappedGridFiniteVolumeOperators::debug = setBoolParameter ("Turn on DEBUG in MappedGridFiniteVolumeOperators class? [n]: ");
  real epsilon = setFloatParameter ("Enter velocity divergence perturbation size for projection: ");

	// ========================================
	// Find grids to use
	// ========================================


  CompositeGrid cg;
  getFromADataBase (cg, nameOfOGFile);
  cg.update();

  int numberOfComponentGrids = cg.numberOfGrids();
  int numberOfDimensions     = cg.numberOfDimensions();
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


  
  if (PLOT_ON) ps.createWindow("tprojSimple: Projection class test code");  
	
  if (PLOT_ON) ps.plot (cg);         //plot the composite grid	  


	// ========================================
	// Create CompositeGridFunctions
	// ========================================
  
  realCompositeGridFunction velocity (cg, all, all, all, numberOfDimensions);
  realCompositeGridFunction projectedVelocity (cg, all, all, all, numberOfDimensions);
  realCompositeGridFunction trueProjectedVelocity (cg, all, all, all, numberOfDimensions);
  realCompositeGridFunction projectedVelocityError (cg, all, all, all, numberOfDimensions);
  realCompositeGridFunction divergence (cg, GridFunctionParameters::cellCentered);
  
  CompositeGridFiniteVolumeOperators operatorForU (cg);

  velocity.setOperators (operatorForU);
  projectedVelocity.setOperators (operatorForU);
    

        // ==========================================
        // Test projection class
        // ==========================================
  
  Display projDisplay;
  Display::cellCenteredDisplayOption = LogicalTrue;
  
  projDisplay.interactivelySetInteractiveDisplay("projDisplay initialization");
  
  Projection::projectionDebug = TRUE; 

  Projection projection;
  projection.setOperators (&operatorForU);
  projection.updateToMatchGrid (cg);


  // ========================================
  // Test projection elliptic solver options
  // ========================================

  projection.ellipticSolverParameterWizard ();

  // ========================================
  // Set the boundary conditions for the projection class
  // ========================================

  projection.boundaryConditionWizard ();

  // ========================================
  // Set the perturbed velocity
  // ========================================

  projection.setPerturbedVelocity (exactVelocityType, epsilon);
  velocity = projection.getPerturbedVelocity ();
  trueProjectedVelocity = projection.getExactVelocity ();

  projDisplay.display (velocity, "Here is the velocity");
  projDisplay.display (trueProjectedVelocity, "Here is the true projected velocity");
  divergence = velocity.div();

  bool PLOT_ON_THIS;
  if (PLOT_ON) PLOT_ON_THIS = setBoolParameter ("Plot initial velocity function and Projection class debug info? ");
  
  if (PLOT_ON_THIS)
  {
    makeDebugContourPlots    (velocity, "initial u", velocityComponents, &ps, &psp);
    makeDebugStreamLinePlots (velocity, "initial velocity", &ps, &psp);
    makeDebugContourPlots    (divergence, "divergence of initial velocity", 0, &ps, &psp);
  } 

// ============================================================
// Test projection of cellCentered velocity ("approximateProjecton")
// ============================================================  

  cout << "===========" << endl << "Regular projection of cellCentered velocity ... " << endl<< "===========" << endl;



// Oges::debug = 127; 
  Oges::debug = 0;
  

  projectedVelocity = projection.project (velocity, &ps, &psp, PLOT_ON_THIS);
  projDisplay.display (projectedVelocity, "Here is the Projected velocity before BCs");

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
  
}
