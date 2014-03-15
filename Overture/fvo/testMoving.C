#include "Overture.h"
#include "GenericGraphicsInterface.h"
#include "Ogen.h"
#include "MatrixTransform.h"
#include "MatrixTransformGridMotion.h"
#include "MatrixTransformGridMotionParameters.h"
#include "DynamicMovingGrids.h"
#include "MatrixTransformMotionFunction.h"
#include "testUtils.h"
    
void 
makeMoviePlots (GenericGraphicsInterface& ps,
		GraphicsParameters &psp,
		CompositeGrid** cgMoving,
		const int & numberOfLevels,
		const int & it,
		const bool & noplot)
{
  char buff[80];
  
  for (int level=0; level<numberOfLevels; level++)
  {
    if (!noplot) ps.erase();
    psp.set(GI_TOP_LABEL, sPrintF(buff, "Grid at timestep %i level %i", it, level));
    if (!noplot) PlotIt::plot(ps,*(cgMoving[level]),psp);
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,LogicalTrue);
    if (!noplot) ps.redraw(LogicalTrue);
  }
}


main(int argc, char* argv[])
{
  //
  // This is a test routine that uses the MatrixTransformGridMotion class to
  // handle Mappings and their motion and uses the DynamicMovingGrids 
  // class to move grids
  //

  //...the usual stuff
  Index::setBoundsCheck (on);
  ios::sync_with_stdio ();

  bool noplot = LogicalFalse;
  if (argc>1){
    cout << "argv[1] = " << argv[1] << endl;
    aString av1 = argv[1];
    if (av1=="noplot") noplot = LogicalTrue;
    cout << "noplot = " << noplot << endl;
  }

//============================================================
// Declare and read in a CompositeGrid from 
// and HDF database file
//============================================================

  CompositeGrid cg;
  int numberOfGrids, numberOfDimensions;
  requestGrid (cg, numberOfGrids, numberOfDimensions);

  bool initializePS = LogicalFalse;
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("Test DynamicMovingGrid Classes", initializePS);
  GraphicsParameters psp;

//============================================================
// A Ogen grid generator object is created here; It will be
// passed to the DynamicMovingGrids object and used to 
// construct the CompositeGrid each time that the component
// grids move
//============================================================

  Ogen gridGenerator (ps);

//============================================================
// The MatrixTransformGridMotionParameters class is used to 
// store the parameters for the MatrixTransformGridMotion class.
// The latter class describes the grid motion using
// the MatrixTransform mapping class.  The component grids
// can either be moved using simple rotations and translations
// or by using a prescribed function, provided by the 
// MatrixTransformMotionFunction class.  Note that the base
// class (GenericMotionFunction) does not require such a parameters
// class; this is (for the moment) specific to the use of the
// MatrixTransform version of the GridMotion class
//============================================================

  int numberOfLevels = 4;
  int numberOfSteps = 20;
  real timestep = .01;

  cout << "Enter numberOfSteps,numberOfLevels,timestep: ";
  cin >> numberOfSteps >> numberOfLevels >> timestep;
  cout << "numberOfSteps = " << numberOfSteps 
       << "numberOfLevels = " << numberOfLevels
       << ", timestep = " << timestep << endl;
  
  MatrixTransformGridMotionParameters mtParams(numberOfDimensions, numberOfGrids);
  mtParams.setupWizard();

//============================================================
// The MatrixTransformGridMotion class describes how the 
// mappings move. This class is derived from the GenericGridMotion
// abstract base class. The parameters
// and grid motion functions are made known to it through the
// MatrixTransformGridMotionParameters object (mtParams)
//============================================================

  MatrixTransformGridMotion movingMappings(cg, numberOfLevels, &mtParams);

  if (!noplot) ps.createWindow();

//============================================================
// The DynamicMovingGrids class is the high-level interface
// to the moving grids.  It provides a uniform interface to
// solver, independent of the details of how the grids are moved.
// It uses a (derived) GenericGridMotion object to tell it
// how to actually move the grids.  
//============================================================

  DynamicMovingGrids movingGrids;
  movingGrids.initialize (cg, &movingMappings, &gridGenerator);

  psp.set (GI_TOP_LABEL, "initial grid");

  if (!noplot) ps.plot (cg, psp);

//============================================================
// The DynamicMovingGrids class returns a list of pointers
// to the CompositeGrid's at all the levels (fractional timesteps)
// in each timestep.  Declare that array here.
//============================================================

  CompositeGrid** cgMoving;

    //...move the grid a bunch of times

  realCompositeGridFunction gridVelocity;
  
  real time = -timestep;
  for (int it=1; it<=numberOfSteps; it++)
  {

    cout << "************" << endl;
    cout << "** it = " << it << endl;
    cout << "************" << endl;

    time += timestep;

    //============================================================
    // Here is how we get the CompositeGrid array with the "moved"
    // grids.
    //============================================================

    movingGrids.updateMovedGrids (time, timestep);
    cgMoving = movingGrids.movedGrids ();

    //...first time through, plot the velocity function
    if (it == 1)
    {
      int level = 0;

      //============================================================
      // Here is how we get the gridVelocity function for the 
      // CompositeGrid at a given level and time. This 
      // calls a function in the (derived) GenericGridMotion class
      //============================================================ 

      movingGrids.getGridVelocity (gridVelocity, level, time);

      psp.set(GI_TOP_LABEL, "grid velocity contours");
      if (!noplot) PlotIt::contour (ps,gridVelocity);
    }
      
    //...now make movie plots

      makeMoviePlots (ps, psp, cgMoving, numberOfLevels, it, noplot);
  }
  
  //...plot the last grid again don't erase
  if (!noplot) ps.erase();
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,LogicalFalse);
  if (!noplot) PlotIt::plot(ps,*(cgMoving[numberOfLevels-1]),psp);


  
}


  
