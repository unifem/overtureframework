#include "Interpolate.h"
#include "InterpolateHelpfulFunctions.h"
#include "PlotStuff.h"
#include "testUtils.h"
#include "TestParameters.h"

main ()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  cout << "========================================" << endl;
  cout << endl;
  cout << "  Interpolate test routine              " << endl;
  cout << endl;
  cout << "========================================" << endl;

  real executionTime;
  
//...set up graphics
  bool plotStuffInitialize = LogicalFalse;
  PlotStuff ps(plotStuffInitialize, "testInterpolate");       // create a PlotStuff object
  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

//  Display display;
//  display.interactivelySetInteractiveDisplay ("turn off display? [o]");

//...set up the interpolation parameters

  int nd = 2;
  int numberOfDimensions = 2;
  
  TestParameters testParams;
  testParams.display ();
  testParams.interactivelySetParameters ();
  testParams.display ();

  bool plotting = testParams.plotting;
  bool debug    = testParams.debug;
  numberOfDimensions = testParams.numberOfDimensions;
  

  InterpolateParameters interpParams (numberOfDimensions, debug);

  interpParams.setAmrRefinementRatio             (testParams.amrRefinementRatio);
  interpParams.setInterpolateType                (testParams.interpolateType);
  interpParams.setInterpolateOrder               (testParams.interpolateOrder);
  interpParams.setGridCentering                  (testParams.gridCentering);
  interpParams.setUseGeneralInterpolationFormula (testParams.useGeneralInterpolationFormula);
  
//InterpolateParameters::InterpolateOffsetDirection iod[3];
  IntegerArray amrRefinementRatio(3);
  amrRefinementRatio = testParams.amrRefinementRatio;

  int i;
//   for (i=0; i<3; i++) 
//     iod[i] = (InterpolateParameters::InterpolateOffsetDirection) 
//       testParams.interpolateOffsetDirection(i);

//...set up the test function for interpolation

  int numberOfGhostPoints = interpParams.interpolateOrder()/2;
  cout << "testInterpolate: setting up a grid with " 
       << numberOfGhostPoints 
       << " ghost points " << endl;
  
  int interpOrder = interpParams.interpolateOrder();
  
  TwilightZoneFlowFunctionType type = testParams.tzType;
  OGFunction* exact = setTwilightZoneFlowFunction (type, 1, numberOfDimensions);

  bool timing = LogicalTrue;
  Interpolate interpolate;
  interpolate.initialize (interpParams, timing);

//...Make a grid and some refinements to test the Interpolate class functions

  real xmin = 0., ymin = 0., xmax = 1., ymax = 1.;
  int n[3], &nx=n[0], &ny=n[1], &nz=n[2];

//...simple square mapping
  SquareMapping coarseMapping (xmin, xmax, ymin, ymax);
  SquareMapping fineMapping   (xmin, xmax, ymin, ymax);


  Index Iv[3], &I1 = Iv[0], &I2 = Iv[1], &I3 = Iv[2];

  int base, bound;

  int powerOf2Min = 4;
  int powerOf2Max = 8;
  cout << "Enter min and max power of 2 for coarse grid: ";
  cin >> powerOf2Min >> powerOf2Max;
  
  int powerOf2    = powerOf2Min;

  int r[3], j;
  for (i=0; i<3; i++) r[i] = interpParams.amrRefinementRatio(i);  

  int size = int(pow(2,powerOf2Min));
  cout << "Coarsest coarse grid will be dimension " << size << endl;
  cout << "Coarsest fine grid will be dimension " 
       << r[0]*size <<","<<r[1]*size << endl;
  
  
  cout << "Enter target Ranges on coarsest fine grid I1.getBase, I1.getBound ";
  cin >> base >> bound;
  I1 = Range (base, bound, 1);


  cout << "Enter target Ranges on coarsest fine grid I2.getBase, I2.getBound ";
  cin >> base >> bound;
  I2 = Range (base, bound, 1);

  MappedGrid* cg;
  MappedGrid* fg;

  realMappedGridFunction uFine;
  uFine.setName("interpolated values");
  realMappedGridFunction uCoarse;

    if (plotting)
    {
      ps.createWindow ("Interpolate test routine results");
    }
  
  real maxNorm, prevMaxNorm = 0.;
  for (powerOf2 = powerOf2Min; powerOf2<=powerOf2Max; powerOf2++)
  {
    
    nx = int(pow(2,powerOf2));
    ny = int(pow(2,powerOf2));
    nz = 1;

    coarseMapping.setGridDimensions (axis1, nx+1);
    coarseMapping.setGridDimensions (axis2, ny+1);

    cg = new MappedGrid (coarseMapping);

    MappedGrid& coarseGrid = *cg;
    for (i=0; i<2; i++) 
      for (j=0; j<3; j++) 
	coarseGrid.setNumberOfGhostPoints (i,j,numberOfGhostPoints);
    coarseGrid.update();
  
    Range C(0,0);
    uCoarse.updateToMatchGrid (coarseGrid);
    setValues (uCoarse, C, exact);

    // display.display (uCoarse, "Coarse array");

    if (plotting)
    {
      psp.set (GI_TOP_LABEL, sPrintF(buffer, "Coarse function, gridsize %5d %5d",nx,ny));
      PlotIt::contour (ps,uCoarse, psp);
    }

    

//...make the refined grid function
    for (i=0; i<3; i++)
    {
      n[i] *= r[i];
    }

    cout << "Fine grid is size nx,ny = " << nx <<","<< ny << endl;

    fineMapping.setGridDimensions (axis1, nx+1);
    fineMapping.setGridDimensions (axis2, ny+1);

    fg = new MappedGrid (fineMapping);
    MappedGrid& fineGrid   = *fg;
    for (i=0; i<2; i++) 
      for (j=0; j<3; j++) 
	fineGrid.setNumberOfGhostPoints (i,j,numberOfGhostPoints);
    fineGrid.update();

    uFine.updateToMatchGrid (fineGrid);

    uFine = 0.;

    cout << "size of uFine is " << endl;
    cout << uFine.getBase(0) <<","<< uFine.getBound(0) << "," << uFine.getStride(0) << endl;
    cout << uFine.getBase(1) <<","<< uFine.getBound(1) << "," << uFine.getStride(1) << endl;

//...define a region to interpolate to

    cout << "Interpolate to region:" << endl;
    for (i=0; i<3; i++) cout << Iv[i].getBase() << " " << Iv[i].getBound() << endl;

    interpolate.interpolateCoarseToFine (uFine, Iv, uCoarse, amrRefinementRatio);

    // display.display (uFine, "fine array after interpolation");

    if (plotting)
    { 
      executionTime = getCPU();
      psp.set (GI_TOP_LABEL, 
	       sPrintF(buffer, 
		       "Interpolated function, gridsize %5d %5d interp order %2d",nx,ny,interpOrder));
      PlotIt::contour (ps,uFine, psp);
      cout << "Time to plot interpolated function = " << getCPU() - executionTime << endl;
    }
    
    real time = 0.;

    maxNorm = printMaxNormOfDifference (uFine, exact, I1, I2, I3, C, time);
    cout << "Ratio of max norms is " << prevMaxNorm/maxNorm << endl;
    prevMaxNorm = maxNorm;

    I1 = Range (max(uFine.getBase(axis1),I1.getBase()*2), 
		min(uFine.getBound(axis1),I1.getBound()*2), 1);
    I2 = Range (max(uFine.getBase(axis2),I2.getBase()*2), 
		min(uFine.getBound(axis2),I2.getBound()*2), 1);

    delete cg;
    delete fg;

  }
  
}
