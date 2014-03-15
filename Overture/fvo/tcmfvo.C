//===============================================================================
//  Coefficient Matrix Example
//    Solve Poisson's equation on a MappedGrid
//      o first solve with Dirichlet BC's
//      o secondly solve with Dirichlet on some sides and Neumann on others
//==============================================================================
#include "Overture.h"  
#include "MappedGridFiniteVolumeOperators.h"
#include "Oges.h"
#include "Square.h"
#include "OGPolyFunction.h"
#include "display.h"
#include "testUtils.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  Display display;
  int degreeOfSpacePolynomial = 2;

  int n, nghost0, nghost1;
  cout << "Enter Oges::debug, n (number of grid lines), nghost0, nghost1 (numberOfGhost lines), degreeOfSpacePolynomial\n";
  cin >> Oges::debug >> n >> nghost0 >> nghost1 >> degreeOfSpacePolynomial;

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  GridFunctionParameters::GridFunctionType 
    cellCentered     = GridFunctionParameters::cellCentered ,
    defaultCentering = GridFunctionParameters::defaultCentering ;

  SquareMapping map;
  int numberOfGridLines=n;
  map.setGridDimensions(axis1,numberOfGridLines);
  map.setGridDimensions(axis2,numberOfGridLines);
    
  MappedGrid mg(map);
  mg.changeToAllCellCentered();
  
  for( int side=Start; side<=End; side++ )
  {
  mg.numberOfGhostPoints()(side,axis1) = nghost0;
  mg.numberOfGhostPoints()(side,axis2) = nghost1;
  
//  mg.dimension()(side,axis1)+=2*side-1;  // why are we screwing around with the dimension array here?
  }
  mg.changeToAllCellCentered();

  mg.update();
  
  mg.update(
    MappedGrid::THEcenter
    | MappedGrid::THEfaceNormal
    | MappedGrid::THEcellVolume
    | MappedGrid::THEcenterNormal
    | MappedGrid::THEcenterArea
    | MappedGrid::THEfaceArea
    | MappedGrid::THEmask
    | MappedGrid::THEcenterBoundaryNormal
    ,
    MappedGrid::COMPUTEgeometryAsNeeded
    | MappedGrid::USEdifferenceApproximation
    );
  mg.discretizationWidth().display("Here is dw");
  // label boundary conditions
  const int inflow=1, outflow=2, wall=3;
  mg.boundaryCondition()(Start,axis1)=inflow;
  mg.boundaryCondition()(End  ,axis1)=outflow;
  mg.boundaryCondition()(Start,axis2)=wall;
  mg.boundaryCondition()(End  ,axis2)=wall;

  // create a twilight-zone function for checking errors
  int degreeOfTimePolynomial = 1;
  int numberOfComponents = mg.numberOfDimensions();
  OGPolyFunction exact(degreeOfSpacePolynomial,
		       mg.numberOfDimensions(),
		       numberOfComponents,
		       degreeOfTimePolynomial);


  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=pow(3,mg.numberOfDimensions());

  realMappedGridFunction coeff(mg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
  coeff = (real)0.;
    
  // create grid functions: 
  realMappedGridFunction 
    u(mg,cellCentered), 
    f(mg,cellCentered);

  u = (real) 0.0;
  f = (real) 0.0;

  //... create operators and associate them with coeff
  MappedGridFiniteVolumeOperators op(mg);             
  op.setStencilSize(stencilSize);
  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  if( Oges::debug & 64 )
    display.display(coeff, "Here is coeff after assigning interior equations");

  // assign the rhs for interior: u.xx+u.yy=f
  Index I1,I2,I3;
  int component0 = 0, eqn0 = 0;
  getIndex(mg.indexRange(),I1,I2,I3);  

  f(I1,I2,I3)=exact.xx(mg,I1,I2,I3,component0)+exact.yy(mg,I1,I2,I3,component0);

  if (Oges::debug & 64)
    display.display (f, "Here is f before BCs");
  
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(eqn0, component0, dirichlet, allBoundaries);

  if( Oges::debug & 64 )
    display.display(coeff, "Here is coeff=laplacianCoefficients after BCs");
  
  coeff.finishBoundaryConditions();

  if( Oges::debug & 64 )
    display.display(coeff, "Here is coeff=laplacianCoefficients after finishBCs");

  //...assign rhs for boundary conditions

  int axis;
  Index Ib1,Ib2,Ib3,If1,If2,If3,Ig1,Ig2,Ig3;
  int component = 0;
  real zero     = (real)0.0;

  GridFunctionParameters::GridFunctionType faceCenteredAxis[3];
  faceCenteredAxis[0] = GridFunctionParameters::faceCenteredAxis1;
  faceCenteredAxis[1] = GridFunctionParameters::faceCenteredAxis2;  
  faceCenteredAxis[2] = GridFunctionParameters::faceCenteredAxis3;

  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition()(side,axis) > 0 )
    {
      getGhostIndex(mg.indexRange(),side,axis,Ig1,Ig2,Ig3);
      getGhostIndex(mg.indexRange(),side,axis,If1,If2,If3,side); //Index'es for faces
      f(Ig1,Ig2,Ig3)=exact(mg,If1,If2,If3,component,zero,faceCenteredAxis[axis]);
    }
  }

  if (Oges::debug & 64)
    display.display (f, "Here is f after BCs");

  
  Oges solver( mg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients

  
  
  solver.solve( u,f );   // solve the equations

  if( Oges::debug & 64 )
    display.display(u,"Here is the solution u");


  getIndex(mg.gridIndexRange(),I1,I2,I3,1);
  if( Oges:: debug & 64 )
    display.display(exact(mg,I1,I2,I3,0),"Here is the exact solution");

  realMappedGridFunction exactgf(mg,defaultCentering);
  exactgf(I1,I2,I3) = exact(mg,I1,I2,I3,0);
  
//  real error=0.;
//  error=max(error,max(abs(u(I1,I2,I3)-exact(mg,I1,I2,I3,0))));    
//  printf("Maximum error with dirichlet bc's= %e\n",error);  
// try this instead:

  cout << "Maximum error with dirichlet BC's" << endl;
  printMaxNormOfDifference (u, exactgf);
  
  // ---------------------------------------------------------------
  // ----- Neumann BC's ----
  // ---------------------------------------------------------------

  cout << "TESTING SINGULAR PROBLEM..." << endl;
  
  mg.boundaryCondition()(Start,axis1)=wall;
  mg.boundaryCondition()(End  ,axis1)=wall;
  mg.boundaryCondition()(Start,axis2)=wall;
  mg.boundaryCondition()(End  ,axis2)=wall;

  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator

  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  inflow);
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  outflow);
  coeff.applyBoundaryConditionCoefficients(0,0,neumann,    wall);

  coeff.finishBoundaryConditions();

  f(I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);

  if (Oges::debug & 64)
    display.display (f, "Here is f before BCs");

  bool singularProblem=TRUE;
  
  ForBoundary(side,axis)
  {

    if( mg.boundaryCondition()(side,axis) ==wall )
    { // for Neumann BC's -- fill in f on first ghostline

      getGhostIndex    (mg.indexRange(), side, axis, Ig1, Ig2, Ig3);
      getGhostIndex    (mg.indexRange(), side, axis, If1, If2, If3, side); //boundary faces

      RealArray & normal = mg.centerBoundaryNormal(side,axis);
      if( mg.numberOfDimensions()==2 )
	f(Ig1,Ig2,Ig3)=
            normal(If1,If2,If3,0)*exact.x(mg,If1,If2,If3,component, zero,faceCenteredAxis[axis])
	   +normal(If1,If2,If3,1)*exact.y(mg,If1,If2,If3,component, zero,faceCenteredAxis[axis]);
      else
	f(Ig1,Ig2,Ig3)=
            normal(If1,If2,If3,0)*exact.x(mg,If1,If2,If3,component, zero, faceCenteredAxis[axis])
           +normal(If1,If2,If3,1)*exact.y(mg,If1,If2,If3,component, zero, faceCenteredAxis[axis])
           +normal(If1,If2,If3,2)*exact.z(mg,If1,If2,If3,component, zero, faceCenteredAxis[axis]);
    }
    else if( mg.boundaryCondition()(side,axis) ==inflow ||  mg.boundaryCondition()(side,axis) ==outflow )
    {
      singularProblem=FALSE;
      getGhostIndex(mg.indexRange(),side,axis,Ig1,Ig2,Ig3);
      f(Ig1,Ig2,Ig3) = exact(mg, If1, If2, If3, 0, faceCenteredAxis[axis]);
    }
  }

  if (Oges::debug & 64)
    display.display (f, "Here is f after BCs");
  
  mg.gridSpacing().display("Here is the grid spacing");

  // if the problem is singular Oges will add an extra constraint equation to make the system nonsingular
  if( singularProblem )
    solver.setCompatibilityConstraint(TRUE);
  // Tell the solver to refactor the matrix since the coefficients have changed
  solver.setRefactor(TRUE);
  // we need to reorder too because the matrix changes a lot for the singular case
  solver.setReorder(TRUE);

  if( singularProblem )
  {
    // we need to first initialize the solver before we can fill in the rhs for the compatbility equation
    solver.initialize();
    int ne, i1e, i2e, i3e, gride;
    solver.equationToIndex( solver.extraEquationNumber(0), ne, i1e, i2e, i3e, gride);
    getIndex(mg.dimension(), I1, I2, I3);
    f(i1e, i2e, i3e)=sum(solver.rightNullVector[0](I1, I2, I3)*exact(mg, I1, I2, I3, 0,0.));
  }

  solver.solve( u,f );   // solve the equations

  if( Oges::debug & 64 )
    display.display(u,"Here is the solution u");

  getIndex(mg.indexRange(),I1,I2,I3,1);
  if( Oges:: debug & 64 )
    display.display(exact(mg,I1,I2,I3,0),"Here is the exact solution");

  Index Ia1,Ia2,Ia3;
  getIndex(mg.indexRange(),Ia1,Ia2,Ia3,1);  // include ghost points
  // mg.indexRange().display("Here is mg.indexRange()");
  // Ia1.display("Here is Ia1");

//  error=max(error,max(abs(u(Ia1,Ia2,Ia3)-exact(mg,Ia1,Ia2,Ia3,0))));    
  // abs(u(Ia1,Ia2,Ia3)-exact(mg,Ia1,Ia2,Ia3,0)).display("abs(error)");
//  printf("Maximum error with neumann bc's= %e\n",error);  
  printf("Maximum error with neumann bc's: ");  

  exactgf(Ia1,Ia2,Ia3) = exact(mg,Ia1,Ia2,Ia3,0);
  printMaxNormOfDifference (u, exactgf);
  


  return(0);

}

