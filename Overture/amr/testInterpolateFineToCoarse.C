#include "Interpolate.h"
#include "InterpolateHelpfulFunctions.h"
#include "PlotStuff.h"
#include "testUtils.h"

int 
main( int argc, char **argv )
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  cout << "============================================================" << endl;
  cout << endl;
  cout << "  Interpolate::interpolateFineToCoarse test routine              " << endl;
  cout << endl;
  cout << "============================================================" << endl;

  real executionTime;
  bool debug = LogicalFalse;
  
  bool plotting = LogicalFalse;
  cout << "Plotting? ";
  aString answer;
  cin >> answer;
  if (answer(0,0) == "y") plotting = LogicalTrue;

//...set up graphics
  bool plotStuffInitialize = LogicalFalse;
  PlotStuff ps(plotStuffInitialize, "testInterpolate");       // create a PlotStuff object
  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

//...set up the interpolation parameters

  int nd = 2;
  InterpolateParameters interpParams;
  interpParams.setNumberOfDimensions (nd);
  interpParams.display ();

  interpParams.interactivelySetParameters ();
  interpParams.display ();

//...set up the test function for interpolation

  int numberOfDimensions = interpParams.numberOfDimensions();
  int numberOfGhostPoints = interpParams.interpolateOrder()/2;
  int interpOrder = interpParams.interpolateOrder();
  
  int itype;
  
  cout << "Twilightzone periodic(0) or polynomial(1)? " ;
  cin >> itype;
  
  TwilightZoneFlowFunctionType type = itype == 0 ? TrigFunction : PolyFunction;
  OGFunction* exact = setTwilightZoneFlowFunction (type, 1, numberOfDimensions);

  bool timing = LogicalTrue;
  Interpolate interpolate;
  interpolate.initialize (interpParams, timing);

//...Make a grid and some refinements to test the Interpolate class functions

  real xmin = 0., ymin = 0., xmax = 1., ymax = 1.;
  int nf[3], &nxf=nf[0], &nyf=nf[1], &nzf=nf[2];
  int n[3],  &nx =n[0],  &ny= n[1],  &nz= n[2];

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

  int r[3], i, j;
  for (i=0; i<3; i++) r[i] = interpParams.amrRefinementRatio(i);  

  int size = int(pow(2,powerOf2Min));
  cout << "Coarsest coarse grid will be dimension " << size << endl;
  cout << "Coarsest fine grid will be dimension " << r[0]*size <<","<<r[1]*size << endl;
  
  cout << "Enter target Ranges on coarsest coarse grid I1.getBase, I1.getBound ";
  cin >> base >> bound;
  I1 = Range (base, bound, 1);


  cout << "Enter target Ranges on coarsest coarse grid I2.getBase, I2.getBound ";
  cin >> base >> bound;
  I2 = Range (base, bound, 1);

  MappedGrid* cg;
  MappedGrid* fg;

  realMappedGridFunction uFine;
  uFine.setName("exact values");
  realMappedGridFunction uCoarse;
  uCoarse.setName("interpolated values");
  
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
    uCoarse = (real) 0.;
    

//...make the refined grid function
    for (i=0; i<3; i++)
    {
      nf[i] = r[i]*n[i];
    }

    cout << "Fine grid is size nx,ny = " << nxf <<","<< nyf << endl;

    fineMapping.setGridDimensions (axis1, nxf+1);
    fineMapping.setGridDimensions (axis2, nyf+1);

    fg = new MappedGrid (fineMapping);
    MappedGrid& fineGrid   = *fg;
    for (i=0; i<2; i++) 
      for (j=0; j<3; j++) 
	fineGrid.setNumberOfGhostPoints (i,j,numberOfGhostPoints);
    fineGrid.update();

    uFine.updateToMatchGrid (fineGrid);

    setValues (uFine, C, exact);

    cout << "size of uFine is " << endl;
    cout << uFine.getBase(0) <<","<< uFine.getBound(0) << "," << uFine.getStride(0) << endl;
    cout << uFine.getBase(1) <<","<< uFine.getBound(1) << "," << uFine.getStride(1) << endl;

    if (plotting)
    {
      ps.createWindow ("Interpolate test routine results");
      psp.set (GI_TOP_LABEL, sPrintF(buffer, "Fine function, gridsize %5d %5d",nxf,nyf));
      PlotIt::contour (ps,uFine, psp);
    }
    
//...define a region to interpolate to

    cout << "Interpolate to region:" << endl;
    for (i=0; i<3; i++) 
      cout << Iv[i].getBase() << " " << Iv[i].getBound() << endl;

    interpolate.interpolateFineToCoarse (uCoarse, Iv, uFine);

    if (plotting)
    { 
      psp.set (GI_TOP_LABEL, 
	       sPrintF(buffer, 
		       "Interpolated function, gridsize %5d %5d interp order %2d",nx,ny,interpOrder));
      PlotIt::contour (ps,uCoarse, psp);
    }
    
    real time = 0.;

    maxNorm = printMaxNormOfDifference (uCoarse, exact, I1, I2, I3, C, time);
    cout << "Ratio of max norms is " << (maxNorm != 0. ? prevMaxNorm/maxNorm : 0) << endl;
    prevMaxNorm = maxNorm;

    I1 = Range (max(uCoarse.getBase(axis1),I1.getBase()*2), 
		min(uCoarse.getBound(axis1),I1.getBound()*2), 1);
    I2 = Range (max(uCoarse.getBase(axis2),I2.getBase()*2), 
		min(uCoarse.getBound(axis2),I2.getBound()*2), 1);

    delete cg;
    delete fg;

  }

  return 0;
}
