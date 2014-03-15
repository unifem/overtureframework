//===============================================================================
//  Coefficient Matrix Example 
//    Solve a System of Equations on a CompositeGrid
//
// Usage: `tcm4 [<gridName>] [-solver=[yale][harwell][slap][petsc]] [-debug=<value>] -noTiming' 
//==============================================================================
#include "Overture.h"  
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "OGPolyFunction.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
bool measureCPU=TRUE;
real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU();
  else
    return 0;
}
int 
main(int argc, char **argv)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
    
  int solverType=OgesParameters::yale; 
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
        measureCPU=FALSE;
      else if( arg(0,6)=="-debug=" )
      {
        sScanF(arg(7,arg.length()-1),"%i",&Oges::debug);
	printf("Setting Oges::debug=%i\n",Oges::debug);
      }
      else if( arg(0,7)=="-solver=" )
      {
        aString solver=arg(8,arg.length()-1);
        if( solver=="yale" )
          solverType=OgesParameters::yale;
	else if( solver=="harwell" )
          solverType=OgesParameters::harwell;
	else if( solver=="slap" )
          solverType=OgesParameters::SLAP;
        else if( solver=="petsc" )
          solverType=OgesParameters::PETSc;
	else
	{
	  printf("Unknown solver=%s \n",(const char*)solver);
	  Overture::abort("error");
	}
	
	printf("Setting solverType=%i\n",solverType);
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[1];
      }
    }
  }
  else
    cout << "Usage: `tcm4 [<gridName>] [-solver=[yale][harwell][slap][petsc]] [-debug=<value>] -noTiming' \n";

  // make some shorter names for readability
  BCTypes::BCNames 
                   dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   normalComponent       = BCTypes::normalComponent,
                   aDotU                 = BCTypes::aDotU,
                   generalizedDivergence = BCTypes::generalizedDivergence,
                   generalMixedDerivative= BCTypes::generalMixedDerivative,
                   aDotGradU             = BCTypes::aDotGradU,
                   vectorSymmetry        = BCTypes::vectorSymmetry,
                   allBoundaries         = BCTypes::allBoundaries; 

  real worstError=0.;
  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];

    cout << "\n *****************************************************************\n";
    cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
    cout << " *****************************************************************\n\n";

    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update();

    const int inflow=1, outflow=2, wall=3;
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( cg[grid].boundaryCondition()(Start,axis1) > 0 )
	cg[grid].boundaryCondition()(Start,axis1)=inflow;
      if( cg[grid].boundaryCondition()(End  ,axis1) > 0 )
	cg[grid].boundaryCondition()(End  ,axis1)=inflow;
      if( cg[grid].boundaryCondition()(Start,axis2) > 0 )
	cg[grid].boundaryCondition()(Start,axis2)=wall;
      if( cg[grid].boundaryCondition()(End  ,axis2) > 0 )
	cg[grid].boundaryCondition()(End  ,axis2)=wall;
    }    

    // create a twilight-zone function
    int degreeOfSpacePolynomial = 2;
    int degreeOfTimePolynomial = 1;
    int numberOfComponents = 2;
    OGPolyFunction exact(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
			 degreeOfTimePolynomial);

    Range all;
    // make a grid function to hold the coefficients
    int stencilSize=int( pow(3,cg.numberOfDimensions())+1 );  // add 1 for interpolation equations
    int stencilDimension=stencilSize*SQR(numberOfComponents);
    realCompositeGridFunction coeff(cg,stencilDimension,all,all,all); 
    // make this grid function a coefficient matrix:
    int numberOfGhostLines=1;
    coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponents);
    coeff=0.;
    
  // create grid functions: 
    realCompositeGridFunction u(cg,all,all,all,numberOfComponents),
      f(cg,all,all,all,numberOfComponents);

    CompositeGridOperators op(cg);                            // create some differential operators 
    op.setNumberOfComponentsForCoefficients(numberOfComponents);
    u.setOperators(op);                              // associate differential operators with u
    coeff.setOperators(op);
  
  // Solve a system of equations for (u_0,u_1) = (u,v)
  //     a1(  u_xx + u_yy ) + a2*v_x = f_0
  //     a3(  v_xx + v_yy ) + a4*u_y = f_1

    const real a1=1., a2=2., a3=3., a4=4.;
//  const real a1=1., a2=0., a3=1., a4=0.;

    Range e0(0,0), e1(1,1);  // e0 = first equation, e1=second equation
    Range c0(0,0), c1(1,1);  // c0 = first component, c1 = second component
    coeff=a1*op.laplacianCoefficients(e0,c0)+a2*op.xCoefficients(e0,c1)
      +a3*op.laplacianCoefficients(e1,c1)+a4*op.yCoefficients(e1,c0);

    coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);  
    coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);  

    coeff.applyBoundaryConditionCoefficients(1,1,dirichlet,  allBoundaries);
    coeff.applyBoundaryConditionCoefficients(1,1,extrapolate,allBoundaries);
/* --
   coeff.applyBoundaryConditionCoefficients(1,1,dirichlet,  inflow);
   coeff.applyBoundaryConditionCoefficients(1,1,extrapolate,inflow);
   coeff.applyBoundaryConditionCoefficients(1,1,neumann,     wall);
 -- */

    coeff.finishBoundaryConditions();
    if( Oges::debug & 16 ) 
      coeff.display("Here is coeff after finishBoundaryConditions");

    Oges solver( cg );                     // create a solver
    solver.setCoefficientArray( coeff );   // supply coefficients
    solver.set(OgesParameters::THEsolverType,solverType); 
    if( solverType==OgesParameters::SLAP ||  solverType==OgesParameters::PETSc )
    {
      solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
      solver.set(OgesParameters::THEtolerance,max(1.e-8,REAL_EPSILON*10.));
    }    

    // assign the rhs:  u=exact on the boundary
    Index I1,I2,I3, Ia1,Ia2,Ia3;
    int side,axis;
    Index Ib1,Ib2,Ib3;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.indexRange(),I1,I2,I3);  

      f[grid](I1,I2,I3,0)=a1*(exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0))+a2*exact.x(mg,I1,I2,I3,1);
      f[grid](I1,I2,I3,1)=a3*(exact.xx(mg,I1,I2,I3,1)+exact.yy(mg,I1,I2,I3,1))+a4*exact.y(mg,I1,I2,I3,0);
      if( cg.numberOfDimensions()==3 )
      {
	f[grid](I1,I2,I3,0)+=a1*exact.zz(mg,I1,I2,I3,0);
	f[grid](I1,I2,I3,1)+=a3*exact.zz(mg,I1,I2,I3,1);
      }

      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  f[grid](Ib1,Ib2,Ib3,0)=exact(mg,Ib1,Ib2,Ib3,0);
	  f[grid](Ib1,Ib2,Ib3,1)=exact(mg,Ib1,Ib2,Ib3,1);
	}
      }
    }
  
    u=0.;  // for interative solvers.
    solver.solve( u,f );   // solve the equations

  // u.display("Here is the solution to u.xx+u.yy=f");
    for( int n=0; n<numberOfComponents; n++ )
    {
      real error=0.;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].indexRange(),I1,I2,I3);  
        RealArray err = (u[grid](I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n))/max(abs(exact(cg[grid],I1,I2,I3,n)));
	where( cg[grid].mask()(I1,I2,I3)!=0 )
	{
	   error=max(error,max(abs(err)));
	}
	if( Oges::debug & 4 ) 
	  abs(u[grid](I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n)).display("abs(error)");
      }
      printf("Maximum relative error in component %i with dirichlet bc's= %e\n",n,error);  
      worstError=max(worstError,error);
    }
    
  } // end loop over grids
  printf("\n\n ************************************************************************************************\n");
  if( worstError > .025 )
    printf(" ************** Warning, there is a large error somewhere, worst error =%e ******************\n",
	   worstError);
  else
    printf(" ************** Test apparently successful, worst error =%e ******************\n",worstError);
  printf(" **************************************************************************************************\n\n");

  return(0);
}

