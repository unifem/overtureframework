#include "testUtils.h"

#undef ForAllGrids
#define ForAllGrids( grid) for (grid=0; grid<numberOfComponentGrids; grid++)

void 
printMaxNormOfScalar (realCompositeGridFunction & scalar, CompositeGrid & cg)
{  
  Index J1,J2,J3;
  int grid;

  int numberOfComponentGrids = cg.numberOfComponentGrids();
  ForAllGrids (grid)
  {   
    MappedGrid & mg = cg[grid];
    int extra = mg.numberOfGhostPoints(0,0);  // assume the same number on each side
    getIndex (mg.indexRange(), J1,J2,J3,extra);
    real maxValue;
    where (cg[grid].mask()(J1,J2,J3) > 0)
    {
       maxValue = max(abs(scalar[grid](J1,J2,J3)));
    }
    cout << "Maximum value on grid " << grid << " = \t\t" << maxValue << endl;
  }
}


real 
getMaxNormOfScalarOnAllGrids (realCompositeGridFunction & scalar, CompositeGrid & cg)
{  
//
// /Purpose:
//   return maximum value of a scalar over all discretization points of all grids
//   in a CompositeGrid
//
  Index J1,J2,J3;
  int grid;
  real returnValue = 0.0;

  int numberOfComponentGrids = cg.numberOfComponentGrids();
  ForAllGrids (grid)
  {   
    MappedGrid & mg = cg[grid];
    int extra = mg.numberOfGhostPoints(0,0);  // assume the same number on each side
    getIndex (mg.indexRange(), J1,J2,J3,extra);
    real maxValue;
    where (cg[grid].mask()(J1,J2,J3) > 0)
    {
       maxValue = max(abs(scalar[grid](J1,J2,J3)));
    }
    returnValue = max(maxValue,returnValue);
  }
  return (returnValue);
}

real 
getMaxNormOfScalarOnGrid (realMappedGridFunction & scalar, MappedGrid & mg)
{
//
// /Purpose:
//   return maximum value of a scalar on discretizations points of a single
//   MappedGrid
//
  Index J1,J2,J3;
  getIndex (mg.indexRange(), J1,J2,J3);
  real maxValue;

//  Display testUtilsDisplay;
  realMappedGridFunction argument (mg);
  argument = abs(scalar);

  real temp = max(argument);

  where (mg.mask()(J1,J2,J3) > 0)
  {
     maxValue = max(abs(scalar(J1,J2,J3)));
  }
  return (maxValue);
}

  

void 
printMaxNormOfProjectedDivergence (realCompositeGridFunction & divergence, CompositeGrid & cg)
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
    real maxValue;
    where (cg[grid].mask()(J1,J2,J3) > 0)
    {
       maxValue = max(abs(divergence[grid](J1,J2,J3)));
    }
    cout << "Maximum value on interior of grid " << grid << " = \t\t" << maxValue << endl;
  }    
}


void 
printMaxNormOfVelocity (realCompositeGridFunction & velocity, CompositeGrid & cg)
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
    int extra = mg.numberOfGhostPoints(0,0); // ...assume the same number on every side
    getIndex (mg.indexRange(), J1,J2,J3, extra);
    real maxValue[3];
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
printMaxNormOfDifference (const realGridCollectionFunction & u, const realGridCollectionFunction & v)
//
// /Purpose:
//   print the maximum norms of the differences of two GridCollectionFunction's
//
{
  
  GridCollection * gc = u.gridCollection;
//  int numberOfComponentGrids = gc->numberOfComponentGrids;
  int numberOfComponentGrids = gc->numberOfGrids();

  int grid,component;
  int numberOfComponents = u.getComponentDimension(0);
  cout << " number of components is " << numberOfComponents << endl;
  
  ForAllGrids (grid)
  {


    MappedGrid & mg = (*gc)[grid];
  
    Index J1,J2,J3;
    real maxValue;
  
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
printMaxNormOfGridFunction (const realMappedGridFunction & mgf, const int& extra)
// 
// /Purpose: 
//   print maximum of each  component of a gridFunction
//
{
  Index J1,J2,J3;
  MappedGrid * mg = mgf.getMappedGrid();
  
  int numberOfComponents = mgf.getComponentDimension(0); //...only works for one component index
  const int maxNumberOfComponents = 20;                        // *** this is an arbitrary limit
  aString name = mgf.getName();
  
//  int extra = mg->numberOfGhostPoints(0,0); // ...assume the same number on every side
    getIndex (mg->indexRange(), J1,J2,J3, extra);
    real maxValue[maxNumberOfComponents];
    int component;
    for (component = 0; component < numberOfComponents; component++)
    {
     where (mg->mask()(J1,J2,J3) > 0)
      {
        maxValue[component] = max(abs(mgf(J1,J2,J3,component)));
      }
    }

    cout << "Maximum values of " << name << ":" << endl;
    
    for (component = 0; component < numberOfComponents; component++)
       cout << maxValue[component] << "  ";
    cout << endl;
}

void 
printMaxNormOfDifference (const realMappedGridFunction & mgf, 
			  const realMappedGridFunction & v, 
			  const int& extra)
// 
// /Purpose: 
//   print maximum of difference of each component of two gridFunctions
//
{
  Index J1,J2,J3;
  MappedGrid * mg = mgf.getMappedGrid();
  
  int numberOfComponents = mgf.getComponentDimension(0); //...only works for one component index
  int noCv = v.getComponentDimension(0);
  if (numberOfComponents > noCv)
  {
    cout << "printMaxNormOfDifference: ERROR: the two gridFunctions don't have a compatible number of components" << endl;
    Overture::abort(" ");
  }
    
  const int maxNumberOfComponents = 20;                        // *** this is an arbitrary limit
  aString name  = mgf.getName();
  aString namev = v.getName();
    
//  int extra = mg->numberOfGhostPoints(0,0); // ...assume the same number on every side
    getIndex (mg->indexRange(), J1,J2,J3, extra);
    real maxValue[maxNumberOfComponents];
    int component;
    for (component = 0; component < numberOfComponents; component++)
    {
     where (mg->mask()(J1,J2,J3) > 0)
      {
        maxValue[component] = max(abs(mgf(J1,J2,J3,component) - v(J1,J2,J3,component)));
      }
    }

    cout << "Maximum differences of " << name << " and " << namev << endl;
    
    for (component = 0; component < numberOfComponents; component++)
       cout << maxValue[component] << "  ";
    cout << endl;
}
  

void 
printMaxNormOfDifference (const realGridCollectionFunction & gcf,
			  OGFunction* exactSolutionFunction,
			  const Index& Components,
			  const real time,
			  const bool includeGhosts,
			  const int& extra)
{
  GridCollection& gc = *(gcf.getGridCollection());
  int numberOfGrids = gc.numberOfGrids();
  aString name  = gcf.getName();
  bool calledFromAbove = LogicalTrue;
  int grid;

  cout << "Maximum differences of " << name << " and exact solution at time " << time <<  endl;
  for (grid=0; grid<numberOfGrids; grid++)
  {
    realMappedGridFunction& mgf = gcf[grid];
    cout << "grid " << grid << ": ";
    printMaxNormOfDifference (mgf, exactSolutionFunction, Components, time, includeGhosts, extra, calledFromAbove);
    cout << endl;
  }
  
}

void 
printMaxNormOfDifference (const realMappedGridFunction & mgf,  
			  OGFunction * exactSolutionFunction, 
			  const Index& Components,
			  const real time, 
			  const bool includeGhosts,
			  const int& extra,
			  const bool& calledFromAbove)
{
  Index J1,J2,J3;
  MappedGrid & mg = *(mgf.getMappedGrid());
  GridFunctionParameters::GridFunctionType gfType = mgf.getGridFunctionType();

//  int numberOfComponents = mgf.getComponentDimension(0); //...only works for one component index
  int cmin = mgf.getComponentBase(0);
  int cmax = mgf.getComponentBound(0);
  int c0,c1;

  if (Components != nullIndex)
    //...in this case, check input C and use it if OK
  {
    c0 = Components.getBase();
    c1 = Components.getBound();
    assert (c0 >= cmin && c1 <= cmax);
    cmin = c0;
    cmax = c1;
  }
    
  if (exactSolutionFunction == NULL)
  {
    cout << "PrintMaxNormOfDifference: OGFunction not set, so I can't compute a difference" << endl;
    return;
  }
  
  OGFunction & e = *exactSolutionFunction;

  //...make storage gridFunctions for a single component of exact solution
  realMappedGridFunction exact;
  exact.updateToMatchGrid (mg, gfType);
  
    
  const int maxNumberOfComponents = 20;                        // *** this is an arbitrary limit
  aString name  = mgf.getName();
    
//  int extra = mg->numberOfGhostPoints(0,0); // ...assume the same number on every side
//  getIndex (mg->indexRange, J1,J2,J3, extra);
    getIndex (mg->dimension, J1,J2,J3, extra);
    real maxValue[maxNumberOfComponents];

    int component;
    for (component = cmin; component <= cmax; component++)
    {
      exact(J1,J2,J3) = e(mg, J1, J2, J3, component, time);

      if (includeGhosts)
      {
	where (mg.mask()(J1,J2,J3) != 0)
	  maxValue[component] = max(abs(mgf(J1,J2,J3,component) - exact(J1,J2,J3)));
      }
      else
      {
	where (mg.mask()(J1,J2,J3) > 0)
	  maxValue[component] = max(abs(mgf(J1,J2,J3,component) - exact(J1,J2,J3)));
	
      }
      
    }

    if (!calledFromAbove)
      cout << "Maximum differences of " << name << " and exact solution at time " << time << endl;
    
    for (component = cmin; component <= cmax; component++) 
      cout << maxValue[component] << "  ";

    if (!calledFromAbove) cout << endl;
}

real
printMaxNormOfDifference (const realMappedGridFunction & mgf,  
			  OGFunction * exactSolutionFunction, 
			  const Index& I1,
			  const Index& I2,
			  const Index& I3,
			  const Index& Components,
			  const real time, 
			  const bool includeGhosts,
			  const int& extra,
			  const bool& calledFromAbove)
//
// /Purpose: print out the max norm of difference between input function mgf
//           and twilight zone function exactSolutionFunction.
//           This version only looks at region defined by (I1,I2,I3,C)
//           also returns max norm of difference of first component.
//
{
  
  MappedGrid & mg = *(mgf.getMappedGrid());
  Index J1, J2, J3;
  getIndex (mg->dimension, J1,J2,J3, extra);
  J1 = I1==nullIndex ? J1 : I1;
  J2 = I2==nullIndex ? J2 : I2;
  J3 = I3==nullIndex ? J3 : I3;
  
  GridFunctionParameters::GridFunctionType gfType = mgf.getGridFunctionType();

//  int numberOfComponents = mgf.getComponentDimension(0); //...only works for one component index
  int cmin = mgf.getComponentBase(0);
  int cmax = mgf.getComponentBound(0);
  int c0,c1;

  if (Components != nullIndex)
    //...in this case, check input C and use it if OK
  {
    c0 = Components.getBase();
    c1 = Components.getBound();
    assert (c0 >= cmin && c1 <= cmax);
    cmin = c0;
    cmax = c1;
  }
    
  if (exactSolutionFunction == NULL)
  {
    cout << "PrintMaxNormOfDifference: OGFunction not set, so I can't compute a difference" << endl;
    return -1.;
  }
  
  OGFunction & e = *exactSolutionFunction;

  //...make storage gridFunctions for a single component of exact solution
  realMappedGridFunction exact;
  exact.updateToMatchGrid (mg, gfType);
  exact = 0.;

  const int maxNumberOfComponents = 20;                        // *** this is an arbitrary limit
  aString name  = mgf.getName();
    
//  int extra = mg->numberOfGhostPoints(0,0); // ...assume the same number on every side
//  getIndex (mg->indexRange, J1,J2,J3, extra);
  

  real maxValue[maxNumberOfComponents];

  int component;
    for (component = cmin; component <= cmax; component++)
    {
      exact(J1,J2,J3) = e(mg, J1, J2, J3, component, time);

      if (includeGhosts)
      {
	where (mg.mask()(J1,J2,J3) != 0)
	  maxValue[component] = max(abs(mgf(J1,J2,J3,component) - exact(J1,J2,J3)));
      }
      else
      {
	where (mg.mask()(J1,J2,J3) > 0)
	  maxValue[component] = max(abs(mgf(J1,J2,J3,component) - exact(J1,J2,J3)));
	
      }
      
    }

    exact(J1,J2,J3) -= mgf(J1,J2,J3,0);

    if (!calledFromAbove)
      cout << "Maximum differences of " << name << " and exact solution at time " << time << endl;
    
    for (component = cmin; component <= cmax; component++) 
      cout << maxValue[component] << "  ";

    if (!calledFromAbove) cout << endl;
    return maxValue[0];
}

