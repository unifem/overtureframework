#include "Overture.h"
#include "PlotStuff.h"
#include "CompositeGridFiniteVolumeOperators.h"
#include "Cgsh.h"

main()
{
  
  //...the usual stuff
  Index::setBoundsCheck (on);
  ios::sync_with_stdio ();

//============================================================
// Declare and read in a CompositeGrid from 
// and HDF database file
//============================================================

  CompositeGrid cg;
  aString gridName;
  gridName = "./periodicCircleInBox.hdf";
  
  char buff[80];
  
  cout << "Enter grid name: ";   // direct this message to output
  cin >> gridName;               // direct the input to this string
  cout << "opening file " << gridName << endl;

  getFromADataBase (cg, gridName);
  
// ... update the grid data 
  cg.update ();
  int numberOfGrids = cg.numberOfGrids();
  int numberOfDimensions = cg.numberOfDimensions();
  
  cout << "numberOfGrids = " << numberOfGrids << endl;

// ... make a grid function, operators

  realCompositeGridFunction u (cg, GridFunctionParameters::defaultCentering, numberOfDimensions);
  CompositeGridFiniteVolumeOperators ops (cg);
  u.setOperators(ops);
  
  u = 1;
  
// ... apply boundary conditions

  Index Components;
  Components =  Range (0,numberOfDimensions-1);
  int wall = 1;
  real time = 0.;
  real ZERO = 0.;
  
  ops.applyBoundaryCondition (u, Components, BCTypes::dirichlet, wall, ZERO, time);
  
}
