#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"
#include "display.h"
#include "GenericGraphicsInterface.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

int 
checkGrid( CompositeGrid & cg, GenericGraphicsInterface *ps =0, int debug=0 )
// ===========================================================================
// /Description:
//   Check the validity of a grid by solving an elliptic problem on it.
//
// ===========================================================================
{
  int solverType=OgesParameters::yale; 

  if( debug > 3 )
    SparseRepForMGF::debug=3;  

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  real worstError=0.;
  cg.update();

  if( debug >1 )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      displayMask(cg[grid].mask(),"mask");
  }

  const int inflow=1, outflow=2; // , wall=3;
  
    // create a twilight-zone function for checking the errors
  OGFunction *exactPointer;
  if( min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
  {
    // this grid is probably periodic in space, use a trig function
    printf("TwilightZone: trigonometric polynomial\n");
    exactPointer = new OGTrigFunction(2.,2.);  // 2*Pi periodic
  }
  else
  {
    printf("TwilightZone: algebraic polynomial\n");
    int degreeOfSpacePolynomial = 2;
    int degreeOfTimePolynomial = 1;
    int numberOfComponents = cg.numberOfDimensions();
    exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
				      degreeOfTimePolynomial);
    
  }
  OGFunction & exact = *exactPointer;

  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=int(pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
  coeff=0.;
    
  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg);
  f=0.; // for iterative solvers

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);

    //   op.setTwilightZoneFlow(TRUE);
    // op.setTwilightZoneFlowFunction(exact);

  f.setOperators(op); // for apply the BC
  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);

  coeff.finishBoundaryConditions();
  // coeff.display("Here is coeff after finishBoundaryConditions");


  Oges solver( cg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients
  solver.set(OgesParameters::THEsolverType,solverType); 
  if( solver.isSolverIterative() ) 
  {
    solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
    solver.set(OgesParameters::THErelativeTolerance,max(1.e-8,REAL_EPSILON*10.));
  }    

  // assign the rhs: Laplacian(u)=f, u=exact on the boundary
  Index I1,I2,I3, Ia1,Ia2,Ia3;
  int side,axis;
  Index Ib1,Ib2,Ib3;
  int grid;

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.indexRange(),I1,I2,I3);  

    if( cg.numberOfDimensions()==1 )
      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
    else if( cg.numberOfDimensions()==2 )
      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
    else
      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);

    ForBoundary(side,axis)
    {
      if( mg.boundaryCondition()(side,axis) > 0 )
      {
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	// f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
        #ifdef USE_PPP
	Overture::abort("Ogmg::checkGrid:ERROR: finish me Bill!");
        #else
	f[grid].applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::boundary(side,axis),exact(mg,Ib1,Ib2,Ib3,0));
        #endif
      }
    }
  }
  // f.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);   
  // f.display("Here is f");
  
  u=0.;  // initial guess for iterative solvers
  real time0=getCPU();
  solver.solve( u,f );   // solve the equations
  real time=getCPU()-time0;
  cout << "time for 1st solve of the Dirichlet problem = " << time << endl;
  
  // solve again
  u=0.;
  time0=getCPU();
  solver.solve( u,f );   // solve the equations
  time=getCPU()-time0;
  cout << "time for 2nd solve of the Dirichlet problem = " << time << endl;

  // u.display("Here is the solution to Laplacian(u)=f");
  real error=0.;
  RealCompositeGridFunction err(cg);
  err=0.;

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
    where( cg[grid].mask()(I1,I2,I3)!=0 )
    {
      err[grid](I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))/max(abs(exact(cg[grid],I1,I2,I3,0)));
      error=max(error, max(err[grid](I1,I2,I3)) );
    }
    if( debug & 8 )
    {
      display(err[grid],"abs(error on indexRange +1)");
      // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
    }
  }
  printf("Maximum (relative) error with dirichlet bc's= %e\n",error);  
  worstError=max(worstError,error);

  GraphicsParameters psp;
  if( ps!=0 )
  {
    aString answer,menu[]= {"solution","error","grid","continue",""}; //
    ps->erase();
    for( ;; )
    {
      ps->getMenuItem(menu,answer,"choose from menu");
      if( answer=="continue" )
        break;
      else if( answer=="solution" )
      {
	psp.set(GI_TOP_LABEL,"checkGrid solution with dirichlet"); 
	PlotIt::contour(*ps,u,psp);
      }
      else if( answer=="error" )
      {
	psp.set(GI_TOP_LABEL,"checkGrid relative error with dirichlet"); 
	PlotIt::contour(*ps,err,psp);
      }
      else if( answer=="grid" )
      {
	psp.set(GI_TOP_LABEL,"checkGrid grid"); 
	PlotIt::plot(*ps,cg);
      }
      else
      {
	printf("Unknown response: [%s]\n",(const char*)answer);
	ps->stopReadingCommandFile();
      }
    }
  }

  

  
  // ----- Neumann BC's ----

  coeff=0.;
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,neumann,allBoundaries);
  coeff.finishBoundaryConditions();

  Index Ig1,Ig2,Ig3;
  bool singularProblem=TRUE;  

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.indexRange(),I1,I2,I3);  
    if( mg.numberOfDimensions()==1 )
      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
    else if(  mg.numberOfDimensions()==2 )
      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
    else 
      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);
    ForBoundary(side,axis)
    {
      if( mg.boundaryCondition()(side,axis) > 0  )
      { // for Neumann BC's -- fill in f on first ghostline
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	const realArray & normal = mg.vertexBoundaryNormal(side,axis);
	if( mg.numberOfDimensions()==1 )
	  f[grid](Ig1,Ig2,Ig3)=(2*side-1)*exact.x(mg,Ib1,Ib2,Ib3,0);  
	else if( mg.numberOfDimensions()==2 )
	  f[grid](Ig1,Ig2,Ig3)=
	    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
	    +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0);
	else
	  f[grid](Ig1,Ig2,Ig3)=
	    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
	    +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
	    +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0);
      }
      else if( mg.boundaryCondition()(side,axis) ==inflow ||  mg.boundaryCondition()(side,axis) ==outflow )
      {
	singularProblem=FALSE;
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
      }
    }
  }
  // if the problem is singular Oges will add an extra constraint equation to make the system nonsingular
  if( singularProblem )
    solver.set(OgesParameters::THEcompatibilityConstraint,TRUE);
  // Tell the solver to refactor the matrix since the coefficients have changed
  solver.setRefactor(TRUE);
  // we need to reorder too because the matrix changes a lot for the singular case
  solver.setReorder(TRUE);

  if( singularProblem )
  {
    // we need to first initialize the solver before we can fill in the rhs for the compatibility equation
    solver.initialize();
    int ne,i1e,i2e,i3e,gride;
    solver.equationToIndex( solver.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
    f[gride](i1e,i2e,i3e)=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      f[gride](i1e,i2e,i3e)+=sum(solver.rightNullVector[grid](I1,I2,I3)*exact(cg[grid],I1,I2,I3,0,0.));
    }
  }

  u=0.;  // initial guess for iterative solvers
  time0=getCPU();
  solver.solve( u,f );   // solve the equations
  time=getCPU()-time0;
  cout << "time for 1st solve of the Neumann problem = " << time << endl;

  // turn off refactor for the 2nd solve
  solver.setRefactor(FALSE);
  solver.setReorder(FALSE);
  u=0.;  // initial guess for iterative solvers
  time0=getCPU();
  solver.solve( u,f );   // solve the equations
  time=getCPU()-time0;
  cout << "time for 2nd solve of the Neumann problem = " << time << endl;

  error=0.;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3);  
    where( cg[grid].mask()(I1,I2,I3)!=0 )
      error=max(error,  max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)))/
		max(abs(exact(cg[grid],I1,I2,I3,0))) );
    if( debug & 32 ) 
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);  
      u[grid].display("Computed solution");
      exact(cg[grid],I1,I2,I3,0).display("exact solution");
      abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
    }
  }
  printf("Maximum relative error with neumann bc's= %e\n",error);  
  worstError=max(worstError,error);
    


  return(0);
}


