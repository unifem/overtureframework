#include <Overture.h>
#include <CompositeGridFiniteVolumeOperators.h>
#include <Projection.h>

void 
printMaxNormOfScalar (REALCompositeGridFunction & scalar, CompositeGrid & cg)
{  
  Index J1,J2,J3;
  int grid;

  int numberOfComponentGrids = cg.numberOfComponentGrids();
  ForAllGrids (grid)
  {   
    MappedGrid & mg = cg[grid];

    int extra0 = mg.numberOfGhostPoints()(0,0);  // assume the same number on each side
    int extra1 = mg.numberOfGhostPoints()(0,1);  // assume the same number on each side
    int extra2 = mg.numberOfGhostPoints()(0,2);  // assume the same number on each side

    getIndex (mg.indexRange(), J1,J2,J3,extra0,extra1,extra2);
    REAL maxValue;
    where (cg[grid].mask()(J1,J2,J3) > 0)
    {
       maxValue = max(abs(scalar[grid](J1,J2,J3)));
    }
    cout << "Maximum value on grid " << grid << " = \t\t" << maxValue << endl;
  }
}


REAL 
getMaxNormOfScalarOnAllGrids (REALCompositeGridFunction & scalar, CompositeGrid & cg)
{  
//
// /Purpose:
//   return maximum value of a scalar over all discretization points of all grids
//   in a CompositeGrid
//
  Index J1,J2,J3;
  int grid;
  REAL returnValue = 0.0;

  int numberOfComponentGrids = cg.numberOfComponentGrids();
  ForAllGrids (grid)
  {   
    MappedGrid & mg = cg[grid];
    int extra0 = mg.numberOfGhostPoints()(0,0);  // assume the same number on each side
    int extra1 = mg.numberOfGhostPoints()(0,1);  // assume the same number on each side
    int extra2 = mg.numberOfGhostPoints()(0,2);  // assume the same number on each side

    getIndex (mg.indexRange(), J1,J2,J3,extra0, extra1, extra2);
    REAL maxValue;
    where (cg[grid].mask()(J1,J2,J3) > 0)
    {
       maxValue = max(abs(scalar[grid](J1,J2,J3)));
    }
    returnValue = max(maxValue,returnValue);
  }
  return (returnValue);
}

REAL 
getMaxNormOfScalarOnGrid (REALMappedGridFunction & scalar, MappedGrid & mg)
{
//
// /Purpose:
//   return maximum value of a scalar on discretizations points of a single
//   MappedGrid
//
  Index J1,J2,J3;
  getIndex (mg.indexRange(), J1,J2,J3);
  REAL maxValue;

  Display testUtilsDisplay;
  REALMappedGridFunction argument (mg);
  argument = abs(scalar);
//  testUtilsDisplay.display (argument, "getMaxNorm...this is the argument of the max");
//  testUtilsDisplay.display (mg.mask(), "getMaxNorm...mask() array");

  REAL temp = max(argument);
//  cout << "getMaxNorm... max value is " << temp << endl;

  where (mg.mask()(J1,J2,J3) > 0)
  {
     maxValue = max(abs(scalar(J1,J2,J3)));
  }
  return (maxValue);
}

  

void 
printMaxNormOfProjectedDivergence (REALCompositeGridFunction & divergence, CompositeGrid & cg)
//
// /Purpose:
//   Print maximum value, but leave out the first row of boundary points, since
//   divergence will not be good there
//
{
  Index J1,J2,J3;
  int grid;
  int numberOfComponentGrids = cg.numberOfComponentGrids();
  int nGhost = -1;
  
  ForAllGrids (grid)
  {
    MappedGrid & mg = cg[grid];
    getIndex (mg.indexRange(), J1, J2, J3, nGhost);
    REAL maxValue;
    where (cg[grid].mask()(J1,J2,J3) > 0)
    {
       maxValue = max(abs(divergence[grid](J1,J2,J3)));
    }
    cout << "Maximum value on interior of grid " << grid << " = \t\t" << maxValue << endl;
  }    
}


void 
printMaxNormOfVelocity (REALCompositeGridFunction & velocity, CompositeGrid & cg)
// 
// /Purpose: 
//   print maximum of each velocity component
//
{
  Index J1,J2,J3;
  int grid, component;
  int numberOfComponentGrids = cg.numberOfComponentGrids();
  int numberOfDimensions = cg.numberOfDimensions();
  
  ForAllGrids (grid)
  {
    MappedGrid & mg = cg[grid];
    int extra0 = mg.numberOfGhostPoints()(0,0); // ...assume the same number on every side
    int extra1 = mg.numberOfGhostPoints()(0,1); // ...assume the same number on every side
    int extra2 = mg.numberOfGhostPoints()(0,2); // ...assume the same number on every side

    getIndex (mg.indexRange(), J1,J2,J3, extra0, extra1, extra2);
    REAL maxValue[3];
    for (component = 0; component < numberOfDimensions; component++)
    {
     where (cg[grid].mask()(J1,J2,J3) > 0) // ...projected velocities are not necessarily interpolated 951108
      {
        maxValue[component] = max(abs(velocity[grid](J1,J2,J3,component)));
      }
    }
    cout << "Maximum values on grid " << grid << " = \t\t";
    for (component = 0; component < numberOfDimensions; component++)
      cout << maxValue[component] << "  ";
    cout << endl;
  }
}

void 
printMaxNormOfDifference (const REALMappedGridFunction & u, const REALMappedGridFunction & v)
{
  MappedGrid * mg = u.mappedGrid;
  
  Index J1,J2,J3;
  int component;
  int numberOfComponents = u.getComponentDimension(0);
  REAL maxValue;
  
  int extra0 = mg->numberOfGhostPoints()(0,0); // ...assume the same number on every side
  int extra1 = mg->numberOfGhostPoints()(0,1); // ...assume the same number on every side
  int extra2 = mg->numberOfGhostPoints()(0,2); // ...assume the same number on every side

  getIndex (mg->indexRange(), J1,J2,J3, extra0, extra1, extra2);
  
  cout << "Maximum differences: ";
  
  for (component=0; component<numberOfComponents; component++)
  {
    maxValue = 0.;
    
    where (mg->mask()(J1,J2,J3) > 0)
    {
      maxValue = max(abs(u(J1,J2,J3,component) - v(J1,J2,J3,component)));
      cout << maxValue << " ";
    }
  }
  cout << endl;
}

void 
printMaxNormOfDifference (const REALGridCollectionFunction & u, const REALGridCollectionFunction & v)
//
// /Purpose:
//   print the maximum norms of the differences of two GridCollectionFunction's
//
{
  
  GridCollection * gc = u.gridCollection;
  int numberOfComponentGrids = gc->numberOfGrids();

  int grid,component;
  int numberOfComponents = u.getComponentDimension(0);
  cout << " number of components is " << numberOfComponents << endl;


  ForAllGrids (grid)
  {

    MappedGrid & mg = (*gc)[grid];

    Index J1,J2,J3;
    int extra0 = mg.numberOfGhostPoints()(0,0); // ...assume the same number on every side
    int extra1 = mg.numberOfGhostPoints()(0,1); // ...assume the same number on every side
    int extra2 = mg.numberOfGhostPoints()(0,2); // ...assume the same number on every side

    getIndex (mg.indexRange(), J1,J2,J3, extra0, extra1, extra2);

    REAL maxValue;
  
    cout << "Maximum differences on grid " << grid << " :";
  
    for (component=0; component<numberOfComponents; component++)
    {
      maxValue = 0.;
    
      where (mg.mask()(J1,J2,J3) > 0)
      {
        maxValue = max(abs(u[grid](J1,J2,J3,component) - v[grid](J1,J2,J3,component)));
        cout << maxValue << " ";
      }
    }
    cout << endl;
  }
  
}

void 
printMaxNormOfGridFunction (REALMappedGridFunction & mgf, MappedGrid & mg)
// 
// /Purpose: 
//   print maximum of each velocity component
//
{
  Index J1,J2,J3;
  int numberOfDimensions = mg.numberOfDimensions();
  int numberOfComponents = mgf.getComponentDimension(0); //...only works for one component index
  const int maxNumberOfComponents = 20;                        // *** this is an arbitrary limit
  aString name = mgf.getName();
  
    int extra0 = mg.numberOfGhostPoints()(0,0); // ...assume the same number on every side
    int extra1 = mg.numberOfGhostPoints()(0,1);
    int extra2 = mg.numberOfGhostPoints()(0,2);
    
    getIndex (mg.indexRange(), J1,J2,J3, extra0, extra1, extra2);
    REAL maxValue[maxNumberOfComponents];
    int component;
    
    for (component = 0; component < numberOfComponents; component++)
    {
     where (mg.mask()(J1,J2,J3) > 0)
      {
        maxValue[component] = max(abs(mgf(J1,J2,J3,component)));
      }
    }

    cout << "Maximum values of " << name << ":" << endl;
    
    for (component = 0; component < numberOfDimensions; component++)
       cout << maxValue[component] << "  ";
    cout << endl;
}
  
void 
applyExactBoundaryConditions (realCompositeGridFunction & projectedVelocity, 
			      CompositeGrid & cg,
			      Projection & projection)
//
// ...Apply Dirichlet BCs on all non-periodic boundaries using the exact 
//    velocity function from the Projection class
//
{
  real HALF = (real) 0.5;

  realCompositeGridFunction exactVelocity = projection.getExactVelocity();
  int numberOfDimensions = cg.numberOfDimensions();
  int numberOfComponentGrids = cg.numberOfComponentGrids();
  
  Index velocityComponents (0,numberOfDimensions);

  //...FVO BCs want to set the value on the face, so modify forcing by averaging

  int grid, side, axis;
  ForAllGrids (grid) ForBoundary (side,axis)
  {
    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
    MappedGrid &mg = cg[grid];
    getBoundaryIndex (mg.indexRange(), side, axis, Ib1, Ib2, Ib3);
    getGhostIndex    (mg.indexRange(), side, axis, Ig1, Ig2, Ig3);

    exactVelocity[grid](Ig1,Ig2,Ig3,velocityComponents) = HALF*(
      exactVelocity[grid](Ib1,Ib2,Ib3,velocityComponents) +
      exactVelocity[grid](Ig1,Ig2,Ig3,velocityComponents));
  }

  Display display;
  display.display (exactVelocity, "exactVelocity before using it for bcs");
  
  projectedVelocity.applyBoundaryCondition (velocityComponents,
					    BCTypes::dirichlet,
					    BCTypes::allBoundaries,
					    exactVelocity);
  projectedVelocity.finishBoundaryConditions();
}
  
Projection::ExactVelocityType 
chooseExactVelocityType ()
{
    
  int choice = -1;
  Projection::ExactVelocityType exactVelocityType = Projection::noExactVelocity;

  while (choice <0 || choice >= Projection::numberOfExactVelocityTypes)
  {
    cout << endl;
    
    cout << "Choose velocity function:" << endl;
    cout << "  " << (int)Projection::noExactVelocity << " no exact velocity "<< endl;
    cout << "  " << (int)Projection::zeroExactVelocity << " zero " << endl;
    cout << "  " << (int)Projection::periodicVelocity << " periodicVelocity "<< endl;
    cout << "  " << (int)Projection::polynomialVelocity << " polynomialVelocity" << endl;
    cout << "  " << (int)Projection::shearLayers << " shearLayers: " << endl;
    cout << "   Choice? " ;
    cin >> choice;
  }
    
  exactVelocityType = (Projection::ExactVelocityType) choice;
  return exactVelocityType;
}


void
setProjectionEllipticSolverParameters (Projection& projection)
{
  
  int solverType;
  
  cout << "***Enter elliptic solver type (1=yale, 2=harwell, 3=bcg, 4=sor): ";
  cin >> solverType;
  projection.setEllipticSolverParameter (Projection::solverType, (Oges::solvers) solverType);
  
  if (solverType == 3)
  {
    int preconditionerType;
    cout << "***Enter preconditioner (0=none, 1=diagonal, 2=incompleteLU, 3=SSOR): ";
    cin >> preconditionerType;
    projection.setEllipticSolverParameter (Projection::conjugateGradientPreconditioner, 
					   (Oges::conjugateGradientPreconditioners) preconditionerType);
  }

  if (solverType == 3)
  {
    int conjugateGradientType;
    cout << "***Enter conjugate gradient type (0=biCG, 1=biCGSquared, 2=GMRes, 3=CGStab): ";
    cin >> conjugateGradientType;
    projection.setEllipticSolverParameter (Projection::conjugateGradientType, (Oges::conjugateGradientTypes) conjugateGradientType);
  }
  
  
  if (solverType > 2)
  {
    int numberOfIterations;
    cout << "***Enter number of iterations: ";
    cin >> numberOfIterations;
    if (solverType == 3)
      projection.setEllipticSolverParameter (Projection::conjugateGradientNumberOfIterations, numberOfIterations);
    if (solverType == 4)
      projection.setEllipticSolverParameter (Projection::sorNumberOfIterations, numberOfIterations);
  }
}

bool 
setBoolParameter (const aString & label)
{
  
  aString yes = "n";
  bool result = LogicalFalse;
  cout << label ;
  cout.flush();
  
  cin >> yes;
  if (yes(0,0) == "y") result = LogicalTrue;
  return result;
}

float
setFloatParameter (const aString & label)
{
  
  float answer;
  cout << label ;
  cout.flush();
  
  cin >> answer;
  return answer;
}

int 
setIntParameter (const aString & label)
{
  
  int answer;
  cout << label ;
  cout.flush();
  
  cin >> answer;
  return answer;
}
      
      
bool 
requestGrid (CompositeGrid & cg, int& numberOfGrids, int& numberOfDimensions)
//
// /Purpose: interactively request and read in an HDF file grid;
//           Return the grid, and numberOfGrids, numberOfDimensions;
//
{
  aString gridName;
//  char buff(80);
  
  cout << "Enter grid name: " ;
  cin >> gridName;
  cout << "opening file " << gridName << endl;
  bool success = getFromADataBase (cg, gridName);

  numberOfGrids = cg.numberOfGrids();
  numberOfDimensions = cg.numberOfDimensions();

  cout << "This is a " 
       << numberOfDimensions 
       << " dimensional grid with " 
       << numberOfGrids 
       << " component grids " 
       << endl;
  
  return success;
}
