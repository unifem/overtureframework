//===============================================================================
//  Coefficient Matrix Example 
//    Solve the steady-state linearized incompressible Navier-Stokes equations
//==============================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "OGPolyFunction.h"

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
                   normalComponent       = BCTypes::normalComponent,
                   extrapolate           = BCTypes::extrapolate,
                   extrapolateNormalComponent = BCTypes::extrapolateNormalComponent,
                   extrapolateTangentialComponent0 = BCTypes::extrapolateTangentialComponent0,
                   normalDerivativeOfTangentialComponent0 = BCTypes::normalDerivativeOfTangentialComponent0,
                   allBoundaries         = BCTypes::allBoundaries; 

  //  AnnulusMapping map; // replace this line with the one below to use an annulus
  SquareMapping map;
  map.setGridDimensions(axis1,5);
  map.setGridDimensions(axis2,5);
    
  MappedGrid mg(map);
  mg.update();

  // label boundary conditions
  const int inflow=1, noSlipWall=2, slipWall=3, outflow=4;
  mg.boundaryCondition()(Start,axis1)=inflow;
  mg.boundaryCondition()(End  ,axis1)=outflow;
  mg.boundaryCondition()(Start,axis2)=noSlipWall;
  mg.boundaryCondition()(End  ,axis2)=slipWall;
    
  // create a twilight-zone function for checking errors
  int degreeOfSpacePolynomial = 2;
  int degreeOfTimePolynomial = 0;
  int numberOfComponents = 3;
  OGPolyFunction exact(degreeOfSpacePolynomial,mg.numberOfDimensions(),numberOfComponents,
		      degreeOfTimePolynomial);

  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=int(pow(3,mg.numberOfDimensions())+.5);
  int stencilDimension=stencilSize*SQR(numberOfComponents);
  realMappedGridFunction coeff(mg,stencilDimension,all,all,all); 
  // make this grid function a coefficient matrix:
  int numberOfGhostLines=1;  // we will solve for values including the first ghostline
  coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponents);
  coeff=0.;
    
  MappedGridOperators op(mg);                            // create some operators
  op.setStencilSize(stencilSize);
  op.setNumberOfComponentsForCoefficients(numberOfComponents);
  coeff.setOperators(op);

  // Form a system of equations for (u,v,p)
  //     nu( \Delta u ) - u0(x) u.x - v0(x) u.y - p.x =  f_0
  //     nu( \Delta v ) - u0(x) v.x - v0(x) v.y - p.y =  f_1
  //         \Delta p - damping (u.x +v.y)            =  f_2
  const real nu=.1, damping=10.;
  const int eq1=0, eq2=1, eq3=2;   // equation numbers
  const int uc=0, vc=1, pc=2;      // component numbers

  RealDistributedArray u0,v0;                 // we have to make copies since we cannot pass a view to multiply(.,coeff)
  u0.redim(mg.vertex()(all,all,all,axis1));
  v0.redim(mg.vertex()(all,all,all,axis1));
  u0=mg.vertex()(all,all,all,axis1) + mg.vertex()(all,all,all,axis2);  // u0 = x+y 
  v0=mg.vertex()(all,all,all,axis1) - mg.vertex()(all,all,all,axis2);  // v0 = x-y 

  coeff=(nu*op.laplacianCoefficients(all,all,all,eq1,uc)            // equation 1
	 - multiply(u0,op.xCoefficients(all,all,all,eq1,uc)) 
	 - multiply(v0,op.yCoefficients(all,all,all,eq1,uc)) 
            - op.xCoefficients(all,all,all,eq1,pc)
        )
    + (nu*op.laplacianCoefficients(all,all,all,eq2,vc)             // equation 2
	 - multiply(u0,op.xCoefficients(all,all,all,eq2,vc)) 
	 - multiply(v0,op.yCoefficients(all,all,all,eq2,vc)) 
            - op.yCoefficients(all,all,all,eq2,pc)
      )
    + (op.laplacianCoefficients(all,all,all,eq3,pc)                // equation 3
           -damping*( op.xCoefficients(all,all,all,eq3,uc)
                     +op.yCoefficients(all,all,all,eq3,vc))
      );
  
  coeff.applyBoundaryConditionCoefficients(eq1,uc,dirichlet,  inflow);  
  coeff.applyBoundaryConditionCoefficients(eq1,uc,extrapolate,inflow);  
  coeff.applyBoundaryConditionCoefficients(eq2,vc,dirichlet,  inflow);
  coeff.applyBoundaryConditionCoefficients(eq2,vc,extrapolate,inflow);
  coeff.applyBoundaryConditionCoefficients(eq3,pc,dirichlet,  inflow);
  coeff.applyBoundaryConditionCoefficients(eq3,pc,extrapolate,inflow);

  coeff.applyBoundaryConditionCoefficients(eq1,uc,dirichlet,  noSlipWall);  
  coeff.applyBoundaryConditionCoefficients(eq1,uc,extrapolate,noSlipWall);  
  coeff.applyBoundaryConditionCoefficients(eq2,vc,dirichlet,  noSlipWall  );
  coeff.applyBoundaryConditionCoefficients(eq2,vc,extrapolate,noSlipWall  );
  coeff.applyBoundaryConditionCoefficients(eq3,pc,neumann,    noSlipWall );

  coeff.applyBoundaryConditionCoefficients(eq1,uc,neumann,    outflow);
  coeff.applyBoundaryConditionCoefficients(eq2,vc,neumann,    outflow);
  coeff.applyBoundaryConditionCoefficients(eq3,pc,dirichlet,  outflow);
  coeff.applyBoundaryConditionCoefficients(eq3,pc,extrapolate,outflow);

  Range V(uc,vc);
  // slip wall: n.u=0, (t.u).n=0, extrapolate n.u to the ghostline.
  // NOTE on slip wall BC's:
  // The sparse solver may run into a null pivot depending on the order of the following
  // equations (if the sparse solver does not pivot). The order below works for a 
  // horizontal slip wall. A vertical slip wall would require an interchange of eq1<->eq2
  coeff.applyBoundaryConditionCoefficients(eq1,V,normalDerivativeOfTangentialComponent0, slipWall  );
  coeff.applyBoundaryConditionCoefficients(eq2,V,normalComponent,                 slipWall  );
  coeff.applyBoundaryConditionCoefficients(eq2,V,extrapolateNormalComponent,      slipWall  );
  coeff.applyBoundaryConditionCoefficients(eq3,pc,neumann,                        slipWall );

  //coeff.applyBoundaryConditionCoefficients(eq1,uc,extrapolate,                    slipWall);  
  //coeff.applyBoundaryConditionCoefficients(eq2,V,normalComponent,                 slipWall  );
  //coeff.applyBoundaryConditionCoefficients(eq2,vc,extrapolate,                    slipWall  );

  coeff.finishBoundaryConditions();

  realMappedGridFunction u(mg,all,all,all,numberOfComponents),
                         f(mg,all,all,all,numberOfComponents);

  Oges solver( mg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients to solver
//solver.setSolverType(Oges::harwell);  

  // assign the right-hand-side
  f=0.;
  Index I1,I2,I3;
  getIndex(mg.indexRange(),I1,I2,I3);  
  f(I1,I2,I3,uc)=nu*(exact.xx(mg,I1,I2,I3,uc)+exact.yy(mg,I1,I2,I3,uc))
                -u0(I1,I2,I3)*exact.x(mg,I1,I2,I3,uc)-v0(I1,I2,I3)*exact.y(mg,I1,I2,I3,uc)
                -exact.x(mg,I1,I2,I3,pc);
  f(I1,I2,I3,vc)=nu*(exact.xx(mg,I1,I2,I3,vc)+exact.yy(mg,I1,I2,I3,vc))
                -u0(I1,I2,I3)*exact.x(mg,I1,I2,I3,vc)-v0(I1,I2,I3)*exact.y(mg,I1,I2,I3,vc)
                -exact.y(mg,I1,I2,I3,pc);
  f(I1,I2,I3,pc)=exact.xx(mg,I1,I2,I3,pc)+exact.yy(mg,I1,I2,I3,pc)
                 -damping*( exact.x(mg,I1,I2,I3,uc)+exact.y(mg,I1,I2,I3,vc) );

  int side,axis;
  Index Ib1,Ib2,Ib3;
  Index Ig1,Ig2,Ig3;
  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition()(side,axis) > 0  )
    {
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
      const realArray & normal  = mg.vertexBoundaryNormal(side,axis);
      const realArray & tangent = mg.centerBoundaryTangent(side,axis);
      switch ( mg.boundaryCondition()(side,axis) )
      {
      case inflow:
        f(Ib1,Ib2,Ib3,uc)=exact(mg,Ib1,Ib2,Ib3,uc);
        f(Ib1,Ib2,Ib3,vc)=exact(mg,Ib1,Ib2,Ib3,vc);
        f(Ib1,Ib2,Ib3,pc)=exact(mg,Ib1,Ib2,Ib3,pc);
        break;
      case outflow:
	f(Ig1,Ig2,Ig3,uc)=normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,uc)
	                 +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,uc);
	f(Ig1,Ig2,Ig3,vc)=normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,vc)
	                 +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,vc);
        f(Ib1,Ib2,Ib3,pc)=exact(mg,Ib1,Ib2,Ib3,pc);
	break;
      case noSlipWall:
        f(Ib1,Ib2,Ib3,uc)=exact(mg,Ib1,Ib2,Ib3,uc);
        f(Ib1,Ib2,Ib3,vc)=exact(mg,Ib1,Ib2,Ib3,vc);
	f(Ig1,Ig2,Ig3,pc)=normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,pc)
	                 +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,pc);
        break;
      case slipWall:
        // normal derivative of the tangential component:
	f(Ig1,Ig2,Ig3,uc)=
	  tangent(Ib1,Ib2,Ib3,0)*( normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,uc)
				  +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,uc))
	 +tangent(Ib1,Ib2,Ib3,1)*( normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,vc)
				  +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,vc));
        // normal component:
        f(Ib1,Ib2,Ib3,vc)=normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,uc)
	                 +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,vc);
	f(Ig1,Ig2,Ig3,pc)=normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,pc)
	                 +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,pc);
        break;
      }
    }
  }
  
  solver.solve( u,f );   // solve the equations
  u.display("Here is the solution u");
    
  getIndex(mg.gridIndexRange(),I1,I2,I3,1);
  for( int n=0; n<numberOfComponents; n++ )
  {
    real error=0.;
    abs(u(I1,I2,I3,n)-exact(mg,I1,I2,I3,n)).display("Error including ghost points");
    
    error=max(error,max( abs(u(I1,I2,I3,n)-exact(mg,I1,I2,I3,n))));
    printf("Maximum error for component %i is = %e\n",n,error);  
  }
  Overture::finish();          
  return(0);
}
