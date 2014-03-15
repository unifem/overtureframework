// **** test out bug with multiple GMRES solvers ****

//===============================================================================
//  Coefficient Matrix Example
//    Solve Poisson's equation on a MappedGrid
//      o first solve with Dirichlet BC's
//      o secondly solve with Dirichlet on some sides and Neumann on others
//==============================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "Square.h"
#include "OGPolyFunction.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int n;
  cout << "Enter Oges::debug, n (number of grid lines)\n";
  cin >> Oges::debug >> n;

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  SquareMapping map;
  int numberOfGridLines=n;
  map.setGridDimensions(axis1,numberOfGridLines);
  map.setGridDimensions(axis2,numberOfGridLines);
    
  MappedGrid mg(map);
  for( int side=Start; side<=End; side++ )
  {
    mg.numberOfGhostPoints()(side,axis1)=2;
    mg.dimension()(side,axis1)+=2*side-1;
  }
  mg.update();
  mg.discretizationWidth().display("Here is dw");
  // label boundary conditions
  const int inflow=1, outflow=2, wall=3;
  mg.boundaryCondition()(Start,axis1)=inflow;
  mg.boundaryCondition()(End  ,axis1)=outflow;
  mg.boundaryCondition()(Start,axis2)=wall;
  mg.boundaryCondition()(End  ,axis2)=wall;
    
  // create a twilight-zone function for checking errors
  int degreeOfSpacePolynomial = 2;
  int degreeOfTimePolynomial = 1;
  int numberOfComponents = mg.numberOfDimensions();
  OGPolyFunction exact(degreeOfSpacePolynomial,mg.numberOfDimensions(),numberOfComponents,
		      degreeOfTimePolynomial);


  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=pow(3,mg.numberOfDimensions());
  realMappedGridFunction coeff(mg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    
  // create grid functions: 
  realMappedGridFunction u(mg),f(mg),u2(mg),f2(mg);

  MappedGridOperators op(mg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  if( Oges::debug & 64 )
    coeff.display("Here is coeff=laplacianCoefficients");
  
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,allBoundaries);
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
  coeff.finishBoundaryConditions();
  
  Oges solver( mg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients
  solver.setSolverType(Oges::bcg);

  // assign the rhs: u.xx+u.yy=f, u=exact on the boundary
  Index I1,I2,I3, Ia1,Ia2,Ia3;
  getIndex(mg.indexRange(),I1,I2,I3);  

  f(I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
  int axis;
  Index Ib1,Ib2,Ib3;
  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition()(side,axis) > 0 )
    {
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      f(Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
    }
  }
  u=0.;
  solver.solve( u,f );   // solve the equations

  // u.display("Here is the solution to u.xx+u.yy=f");
  real error=0.;
  error=max(error,max(abs(u(I1,I2,I3)-exact(mg,I1,I2,I3,0))));    
  printf("Maximum error with dirichlet bc's= %e\n",error);  

  
  // -----------------------
  // ----- Neumann BC's ----
  // -----------------------

  mg.boundaryCondition()(Start,axis1)=wall;
  mg.boundaryCondition()(End  ,axis1)=wall;
  mg.boundaryCondition()(Start,axis2)=wall;
  mg.boundaryCondition()(End  ,axis2)=wall;

  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  inflow);

  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,inflow);

  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  outflow);
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,outflow);

  coeff.applyBoundaryConditionCoefficients(0,0,neumann,    wall);
  coeff.finishBoundaryConditions();

  f2(I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);

  Index Ig1,Ig2,Ig3;
  bool singularProblem=TRUE;
  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition()(side,axis) ==wall )
    { // for Neumann BC's -- fill in f on first ghostline
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
      RealArray & normal = mg.vertexBoundaryNormal(side,axis);
      if( mg.numberOfDimensions()==2 )
	f2(Ig1,Ig2,Ig3)=
            normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
	   +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0);
      else
	f2(Ig1,Ig2,Ig3)=
            normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
           +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
           +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0);
    }
    else if( mg.boundaryCondition()(side,axis) ==inflow ||  mg.boundaryCondition()(side,axis) ==outflow )
    {
      singularProblem=FALSE;
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      f2(Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
    }
  }

  Oges solver2( mg );                     // create a solver
  solver2.setSolverType(Oges::bcg);
  solver2.setCoefficientArray( coeff );   // supply coefficients
  // if the problem is singular Oges will add an extra constraint equation to make the system nonsingular
  if( singularProblem )
    solver2.setCompatibilityConstraint(TRUE);


  if( singularProblem )
  {
    // we need to first initialize the solver2 before we can fill in the rhs for the compatbility equation
    solver2.initialize();
    int ne,i1e,i2e,i3e,gride;
    solver2.equationToIndex( solver2.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
    getIndex(mg.dimension(),I1,I2,I3);
    f2(i1e,i2e,i3e)=sum(solver2.rightNullVector[0](I1,I2,I3)*exact(mg,I1,I2,I3,0,0.));
  }

  u2=0.;
  solver2.solve( u2,f2 );   // solve the equations
  getIndex(mg.indexRange(),Ia1,Ia2,Ia3,1);  // include ghost points
  // mg.indexRange().display("Here is mg.indexRange()");
  // Ia1.display("Here is Ia1");

  error=max(error,max(abs(u(Ia1,Ia2,Ia3)-exact(mg,Ia1,Ia2,Ia3,0))));    
  // abs(u(Ia1,Ia2,Ia3)-exact(mg,Ia1,Ia2,Ia3,0)).display("abs(error)");
  printf("Maximum error with neumann bc's= %e\n",error);  

  for( int it=0; it<3; it++ )
  {
    u=f;
    solver.solve(u,u);
    u2=f2;
    solver2.solve( u2,u2 );   // solve the equations
  }
  
  return(0);

}

