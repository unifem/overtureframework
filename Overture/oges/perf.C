//===============================================================================
// 
// Compare the performance of various sparse solver packages and algorithms.
//
// 
//==============================================================================
#include "Oges.h"

#include "CompositeGridOperators.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"
#include "PlotStuff.h"
#include "display.h"


int 
residual(const RealGridCollectionFunction & coeff,
         const RealGridCollectionFunction & u,
         const RealGridCollectionFunction & f,
         RealGridCollectionFunction & r );

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
	else if( solver=="petsc" || solver=="PETSc" )
          solverType=OgesParameters::PETSc;
	else if( solver=="slap" || solver=="SLAP" )
          solverType=OgesParameters::SLAP;
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
    cout << "Usage: `toges [<gridName>] [-solver=[yale][harwell][gmres]] [-debug=<value>] -noTiming' \n";



  if( Oges::debug > 3 )
    SparseRepForMGF::debug=3;

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  real worstError=0.;
  aString nameOfOGFile=gridName[0];

  cout << "\n *****************************************************************\n";
  cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
  cout << " *****************************************************************\n\n";


  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();
  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg),r(cg);
  u=0.;

  const int inflow=1, outflow=2, wall=3;
  
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

  int numberOfOptions= solverType==OgesParameters::SLAP ? 2 : solverType==OgesParameters::PETSc ? 8 : 1;
  
  for( int it=0; it<numberOfOptions; it++ )
  {
    
    Oges solver( cg );            // create a solver
    solver.set(OgesParameters::THEsolverType,solverType); 
    // solver.setCommandLineArguments( argc,argv );

//     noPreconditioner,
//     jacobiPreconditioner,
//     sorPreconditioner,
//     luPreconditioner,
//     shellPreconditioner,
//     blockJacobiPreconditioner,
//     multigridPreconditioner,
//     eisenstatPreconditioner,
//     incompleteCholeskyPreconditioner,
//     incompleteLUPreconditioner,
//     additiveSchwarzPreconditioner,
//     slesPreconditioner,
//     compositePreconditioner,
//     redundantPreconditioner,
//     diagonalPreconditioner,
//     ssorPreconditioner
    if( solverType==OgesParameters::SLAP )
    {
      if( it==0 )
      {
        solver.set(OgesParameters::THEpreconditioner,OgesParameters::noPreconditioner); 
      }
      else if( it==1 )
      {
        solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner); 
      }
    }
    else if( solverType==OgesParameters::PETSc )
    {
//       if( it==0 )
//       {
//         solver.set(OgesParameters::THEpreconditioner,OgesParameters::noPreconditioner); 
//       }
      if( it==0 )
      {
//         solver.set(OgesParameters::THEpreconditioner,OgesParameters::noPreconditioner); 
        solver.set(OgesParameters::THEpreconditioner,OgesParameters::jacobiPreconditioner); 
      }
      else if( it==1 )
      {
        solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner); 
	solver.set(OgesParameters::THEnumberOfIncompleteLULevels,0);
      }
      else if( it==2 )
      {
        solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner); 
	solver.set(OgesParameters::THEnumberOfIncompleteLULevels,1);
      }
      else if( it==3 )
      {
        solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner); 
	solver.set(OgesParameters::THEnumberOfIncompleteLULevels,2);
      }
      else if( it==4 )
      {
        solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner); 
	solver.set(OgesParameters::THEnumberOfIncompleteLULevels,3);
      }
      else if( it==5 )
      {
        solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
	solver.set(OgesParameters::THEnumberOfIncompleteLULevels,0);
      }
      
      else if( it==6 )
      {
        solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
	solver.set(OgesParameters::THEnumberOfIncompleteLULevels,1);
      }
      
      else if( it==7 )
      {
        solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
	solver.set(OgesParameters::THEnumberOfIncompleteLULevels,2);
      }
      
    }
    
    
	// make a grid function to hold the coefficients
    Range all;
    int stencilSize=int(pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
    realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
    coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    coeff=0.;
    
    f=0.; // for iterative solvers

    CompositeGridOperators op(cg);                            // create some differential operators
    op.setStencilSize(stencilSize);

    coeff.setOperators(op);
  
    coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
    coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);

    coeff.finishBoundaryConditions();
    // coeff.display("Here is coeff after finishBoundaryConditions");


    solver.setCoefficientArray( coeff );   // supply coefficients
//       if( solverType>2 )
//       {
// 	solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
// 	solver.set(OgesParameters::THEtolerance,max(1.e-8,REAL_EPSILON*10.));
//       }    

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
	  f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
	}
      }
    }
  
    cout << "*** solver name = " << solver.parameters.getSolverName() << endl;
    

    u=0.;  // initial guess for iterative solvers
    real time0=CPU();
    solver.solve( u,f );   // solve the equations
    real time=CPU()-time0;
    printf("time for 1st solve of the Dirichlet problem = %8.2e, (iterations=%i)\n",time,
	   solver.getNumberOfIterations());
  
    // solve again
    u=0.;
    time0=CPU();
    solver.solve( u,f );   // solve the equations
    time=CPU()-time0;
    printf("time for 2nd solve of the Dirichlet problem = %8.2e, (iterations=%i)\n",time,
	   solver.getNumberOfIterations());

    // u.display("Here is the solution to Laplacian(u)=f");
    real error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
      where( cg[grid].mask()(I1,I2,I3)!=0 )
	error=max(error, max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)))/
		  max(abs(exact(cg[grid],I1,I2,I3,0))) );
      if( Oges::debug & 8 )
      {
	RealArray err(I1,I2,I3);
	err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))/max(abs(exact(cg[grid],I1,I2,I3,0)));
	where( cg[grid].mask()(I1,I2,I3)==0 )
	  err(I1,I2,I3)=0.;
	err.display("abs(error on indexRange +1)");
	// abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
      }
    }
    residual(coeff,u,f,r);

    // r.display("residual");
    

    printf("Maximum relative error with dirichlet bc's= %8.2e, max residual=%8.2e max|u|=%8.2e size=%e\n",error,
           max(fabs(r)),max(fabs(u)),solver.sizeOf());  
    worstError=max(worstError,error);

    if( Oges::debug & 1 )
      solver.sizeOf(stdout);
    

    
  }

  return(0);
}


