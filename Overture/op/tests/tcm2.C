//===============================================================================
//  Coefficient Matrix Example 
//    Solve a system of equations on a MappedGrid
//==============================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "OGPolyFunction.h"
#include "display.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  // cout << "Enter Oges::debug\n";   cin >> Oges::debug; 

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  //  AnnulusMapping map;   // switch this with the line below to get an Annulus
  SquareMapping map;
  map.setGridDimensions(axis1,5);
  map.setGridDimensions(axis2,5);
    
  MappedGrid mg(map);
  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal);

  // label boundary conditions
  const int inflow=1, wall=2;
  mg.boundaryCondition()(Start,axis1)=inflow;
  mg.boundaryCondition()(End  ,axis1)=inflow;
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
  int stencilSize=int( pow(3,mg.numberOfDimensions()) );
  int numberOfComponentsForCoefficients=2;
  int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction coeff(mg,stencilDimension,all,all,all); 
  // make this grid function a coefficient matrix:
  int numberOfGhostLines=1;  // we will solve for values including the first ghostline
  coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponentsForCoefficients);
  coeff=0.;
    
  MappedGridOperators op(mg);                            // create some operators
  op.setStencilSize(stencilSize);
  op.setNumberOfComponentsForCoefficients(numberOfComponentsForCoefficients);
  coeff.setOperators(op);

  // Form a system of equations for (u,v)
  //     a1(  u_xx + u_yy ) + a2*v_x - u = f_0
  //     a3(  v_xx + v_yy ) + a4*u_y     = f_1
  //  BC's:   u=given   on all boundaries
  //          v=given   on inflow
  //          v.n=given on walls
  const real a1=1., a2=2., a3=3., a4=4.;
  // const real a1=1., a2=0., a3=1., a4=0.;

  const int eqn0=0;    // labels equation 0
  const int eqn1=1;    // labels equation 1
  const int uc=0, vc=1;  // labels for the u and v components
  coeff=a1*op.laplacianCoefficients(all,all,all,eqn0,uc)+a2*op.xCoefficients(all,all,all,eqn0,vc)
          -op.identityCoefficients(all,all,all,eqn0,uc)
       +a3*op.laplacianCoefficients(all,all,all,eqn1,vc)+a4*op.yCoefficients(all,all,all,eqn1,uc);
  if( Oges:: debug & 4 )
    display(coeff,"Here is coeff after assigning interior equations ","%5.2f ");

  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);  
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);  
  if( Oges:: debug & 4 )
    display(coeff,"Here is coeff after dirichlet/extrapolate BC's for (0) ","%5.2f ");

  coeff.applyBoundaryConditionCoefficients(1,1,dirichlet,  inflow);
  coeff.applyBoundaryConditionCoefficients(1,1,extrapolate,inflow);
  coeff.applyBoundaryConditionCoefficients(1,1,neumann,     wall);

  if( Oges:: debug & 4 )
    display(coeff,"Here is coeff with dirichlet (0) and neumann BC's on wall (1)","%5.2f ");

  coeff.finishBoundaryConditions();

  realMappedGridFunction u(mg,all,all,all,2),f(mg,all,all,all,2);

  Oges solver( mg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients to solver

  // assign the right-hand-side
  Index I1,I2,I3;
  getIndex(mg.indexRange(),I1,I2,I3);  
  f(I1,I2,I3,0)=a1*(exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0))+a2*exact.x(mg,I1,I2,I3,1)-exact(mg,I1,I2,I3,0);
  f(I1,I2,I3,1)=a3*(exact.xx(mg,I1,I2,I3,1)+exact.yy(mg,I1,I2,I3,1))+a4*exact.y(mg,I1,I2,I3,0);

  int side,axis;
  Index Ib1,Ib2,Ib3;
  Index Ig1,Ig2,Ig3;
  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition()(side,axis) > 0  )
    {
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      f(Ib1,Ib2,Ib3,0)=exact(mg,Ib1,Ib2,Ib3,0);
      if( mg.boundaryCondition()(side,axis)==inflow )
      {
        f(Ib1,Ib2,Ib3,1)=exact(mg,Ib1,Ib2,Ib3,1);
      }
      else
      {
	// for Neumann BC's -- fill in f on first ghostline
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	realArray & normal = mg.vertexBoundaryNormal(side,axis);
	if( mg.numberOfDimensions()==2 )
	  f(Ig1,Ig2,Ig3,1)=
	    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,1)
	      +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,1);
	else
	  f(Ig1,Ig2,Ig3,1)=
	    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,1)
	      +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,1)
		+normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,1);
      }
    }
  }

  if( Oges:: debug & 4 )
    display(f,"Here is the rhs");

  solver.solve( u,f );   // solve the equations

  getIndex(mg.gridIndexRange(),I1,I2,I3,1);

  display(u,"Here is the solution u","%5.2f ");

  if( Oges:: debug & 4 )
    display(exact(mg,I1,I2,I3,Range(0,1)),"Here is the exact solution");
    
  for( int n=0; n<numberOfComponentsForCoefficients; n++ )
  {
    
    real error=0.;
    display(evaluate(abs(u(I1,I2,I3,n)-exact(mg,I1,I2,I3,n))),"Error including ghost points","%6.2e ");
    
    error=max(error,max( abs(u(I1,I2,I3,n)-exact(mg,I1,I2,I3,n))));
    printf("Maximum error for component %i is = %e\n",n,error);  
  }


  Overture::finish();          
  return(0);
}
