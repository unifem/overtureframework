//===============================================================================
//  Coefficient Matrix Example
//    Using Oges to solve Poisson's equation on a CompositeGrid
//
// Usage: `toges [<file.cmd>] [noplot] [-grid=grid] [-solver=[yale][harwell][petsc][slap]] [-debug=<value>] -noTiming'
//==============================================================================
#include "Oges.h"

#include "CompositeGridOperators.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"
#include "PlotStuff.h"


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
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
  aString commandFileName="";
  bool plotOption=TRUE;
  
  // This macro will initialize the PETSc solver if OVERTURE_USE_PETSC is defined.
  INIT_PETSC_SOLVER();
  
  int solverType=OgesParameters::yale; 
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
        measureCPU=FALSE;
      else if( arg=="noplot" )
      {
        plotOption=FALSE;
      }
      else if( arg(0,6)=="-debug=" )
      {
        sScanF(arg(7,arg.length()-1),"%i",&Oges::debug);
	printf("Setting Oges::debug=%i\n",Oges::debug);
      }
      else if( arg(0,5)=="-grid=" )
      {
	numberOfGridsToTest=1;
	gridName[0]=arg;
        int len=gridName[0].length();
        gridName[0]=gridName[0](6,len);
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
	commandFileName=arg;
      }
    }
  }
  else
    cout << "Usage: `toges [<file.cmd>] [noplot] [-grid=grid] [-solver=yale/harwell/petsc/slap] [-debug=<value>] -noTiming' \n";


  PlotStuff ps(plotOption,"Oges test");
  PlotStuffParameters psp;
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  
  aString nameOfOGFile=gridName[0];

  if( commandFileName!="" )
  {
    cout << "read command file =" << commandFileName << endl;
    ps.readCommandFile(commandFileName);
    
    ps.inputString(nameOfOGFile,"Enter the name of the grid");
    printf(" Using grid=%s\n",(const char*)nameOfOGFile);
  }

  if( Oges::debug > 3 )
    SparseRepForMGF::debug=3;

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  real worstError=0.;

  cout << "\n *****************************************************************\n";
  cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
  cout << " *****************************************************************\n\n";


  aString answer;
  aString menu[]=
  {
    "change options",
    "solve",
    "solution",
    "error",
    "plot grid",
    ">choose grid",
      "square5",
      "square10",
      "square20",
      "cic",
      "threeHoles",
      "box5",
      "sib",
    "<choose active grids",
    "debug",
    "exit",
    ""
  };
  

  bool notDone=TRUE;
  while( notDone )
  {
    
    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update();

    Oges solver( cg );            // create a solver
    solver.set(OgesParameters::THEsolverType,solverType); 

    Range all;
    // create grid functions: 
    realCompositeGridFunction u(cg),f(cg),w(cg,all,all,all,2),err(cg,all,all,all,2);
    u=0.;

    w=0.;
    w.setName("solution (Dirichlet)",0);
    w.setName("solution (Neumann)",1);

    err=0.;
    err.setName("error (Dirichlet)",0);
    err.setName("error (Neumann)",1);

    bool useAllGrids=true;
    
    ps.erase();
    PlotIt::plot(ps,cg,psp);

    bool useSameGrid=TRUE;
    while( notDone && useSameGrid )
    {
      ps.getMenuItem(menu,answer,"choose an option");

      if( answer=="exit" )
      {
	notDone=FALSE;
      }
      else if( answer=="change options" )
      {
	solver.update(ps,cg);              //    change solver parameters
        solver.get(OgesParameters::THEsolverType,solverType); // set local variable
        if( FALSE && solverType==OgesParameters::multigrid )
	{
          printf("*** toges: update the multigridLevel\n");
          cg.update(CompositeGrid::THEmultigridLevel);
          // solver.updateToMatchGrid(cg);
          // u.updateToMatchGrid(cg);
          // f.updateToMatchGrid(cg);
	}
      }
      else if( answer=="debug" )
      {
	ps.inputString(answer,"Enter debug");
	sScanF(answer,"%i",&Oges::debug);
	printf("Setting Oges::debug=%i\n",Oges::debug);
      }
      else if( answer=="choose active grids" )
      {
        // We can selctively solve on only some grids (the solution is assumed known on other grids)
	useAllGrids=false;
	
        IntegerArray activeGrids;
        int num=ps.getValues("Enter active grids (`done' to finish)",activeGrids,0,cg.numberOfComponentGrids()-1);
        activeGrids.display("activeGrids");
        solver.setGridsToUse(activeGrids);
      }
      else if( answer=="solution" )
      {
        ps.erase();
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	PlotIt::contour(ps,w,psp);
      }
      else if( answer=="error" )
      {
        ps.erase();
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	PlotIt::contour(ps,err,psp);
      }
      else if( answer=="plot grid" )
      {
        ps.erase();
	PlotIt::plot(ps,cg,psp);
      }
      else if( answer!="solve" )
      {
	nameOfOGFile=answer;
	useSameGrid=FALSE;
      }
      else if( answer=="square5" )
      {
	nameOfOGFile="square5.hdf";
	useSameGrid=FALSE;
      }
      else if( answer=="square10" )
      {
	nameOfOGFile="square10.hdf";
	useSameGrid=FALSE;
      }
      else if( answer=="square20" )
      {
	nameOfOGFile="square20.hdf";
	useSameGrid=FALSE;
      }
      else if( answer=="cic" )
      {
	nameOfOGFile="cic.hdf";
	useSameGrid=FALSE;
      }
      else if( answer=="box5" )
      {
	nameOfOGFile="box5.hdf";
	useSameGrid=FALSE;
      }
      else if( answer=="sib" )
      {
	nameOfOGFile="sib.hdf";
	useSameGrid=FALSE;
      }
      else if( answer=="solve" )
      {
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

	// make a grid function to hold the coefficients
	int stencilSize=int(pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
	realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
	coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
	coeff=0.;
    
	f=0.; // for iterative solvers

	CompositeGridOperators op(cg);                            // create some differential operators
	op.setStencilSize(stencilSize);

	coeff.setOperators(op);
	
	Index I1,I2,I3, Ia1,Ia2,Ia3;
	int side,axis;
	Index Ib1,Ib2,Ib3;
	int grid;
        real time0,time, error;

	bool solveDirichlet=TRUE;
	if( solveDirichlet )
	{
	  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
	  // fill in the coefficients for the boundary conditions
	  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
	  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);

	  coeff.finishBoundaryConditions();
	  // coeff.display("Here is coeff after finishBoundaryConditions");

          if( !useAllGrids )
	  {
            // we shouldn't need the coefficients on in-active grids
	    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	    {
	      if( !solver.activeGrid(grid) )
		coeff[grid].destroy();
	    }
	  }
	  

	  solver.setCoefficientArray( coeff );   // supply coefficients
//       if( solverType>2 )
//       {
// 	solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
// 	solver.set(OgesParameters::THEtolerance,max(1.e-8,REAL_EPSILON*10.));
//       }    

	  // assign the rhs: Laplacian(u)=f, u=exact on the boundary
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg[grid];

	    getIndex(mg.dimension(),I1,I2,I3);  
            if( !solver.activeGrid(grid) )
              u[grid](I1,I2,I3)=exact(mg,I1,I2,I3,0);  // assign in-active grids
            else
	    {
              // u[grid](I1,I2,I3)=exact(mg,I1,I2,I3,0);  // temporarily set to exact solution
              realArray uExact;
	      uExact=exact(mg,I1,I2,I3,0);
              realArray & uu = u[grid];
	      uu=0.;
              const int ni = cg.numberOfInterpolationPoints(grid);
              const intArray & ip = cg.interpolationPoint[grid];
              const intArray & ig = cg.interpoleeGrid[grid];
              const IntegerArray & useThisGrid = solver.getUseThisGrid();
	      
	      int i1,i2,i3=0;
	      if( cg.numberOfDimensions()==2 )
	      {
		for( int i=0; i<ni; i++ )
		{
                  // printf(" i=%i ig=%i useThisGrid(ig(i))=%i \n",i,ig(i),useThisGrid(ig(i)));
		  
		  if( !useAllGrids && !useThisGrid(ig(i)) ) // the interpolee grid is in-active
		  {
		    i1=ip(i,0);
		    i2=ip(i,1);
		    uu(i1,i2,i3)=uExact(i1,i2,i3);
		  }
		}
	      }
	      else if( cg.numberOfDimensions()==3 )
	      {
		for( int i=0; i<ni; i++ )
		{
		  if( !useAllGrids && !useThisGrid(ig(i)) ) // the interpolee grid is in-active
		  {
		    i1=ip(i,0);
		    i2=ip(i,1);
		    i3=ip(i,2);
		    uu(i1,i2,i3)=uExact(i1,i2,i3);
		  }
		}
	      }

	    }
	    

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
    

          if( useAllGrids )
  	    u=0.;  // initial guess for iterative solvers
	  time0=CPU();
	  solver.solve( u,f );   // solve the equations
	  time=CPU()-time0;
	  printf("time for 1st solve of the Dirichlet problem = %8.2e, (iterations=%i)\n",time,
		 solver.getNumberOfIterations());
  
	  // solve again
          if( useAllGrids )
  	    u=0.;
	  time0=CPU();
	  solver.solve( u,f );   // solve the equations
	  time=CPU()-time0;
	  printf("time for 2nd solve of the Dirichlet problem = %8.2e, (iterations=%i)\n\n",time,
		 solver.getNumberOfIterations());

	  // u.display("Here is the solution to Laplacian(u)=f");
	  error=0.;
          err=0;
          w=0.;
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
            realArray & errg=err[grid];
            w[grid](all,all,all,0)=u[grid];
	    
	    where( cg[grid].mask()(I1,I2,I3)!=0 )
	    {
	      errg(I1,I2,I3,0)=fabs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	    }
            real maxErr=max(fabs(errg(I1,I2,I3,0))/max(fabs(exact(cg[grid],I1,I2,I3,0))));
            error=max(error,maxErr);
            printf(" ***Dirichlet: grid=%i : max rel err = %8.2e\n",grid,maxErr);
	    
	    if( Oges::debug & 8 )
	    {
	      errg(I1,I2,I3,0).display("Dirichlet: abs(error on indexRange +1)");
	      // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
	    }
	  }
	  printf(" ***Dirichlet: maximum rel error on all grids = %e ***\n\n",error);  
	  worstError=max(worstError,error);
	}
	bool solveNeumann=useAllGrids;
	if( solveNeumann )
	{
  
	  // ----- Neumann BC's ----

	  coeff=0.;
	  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
	  // fill in the coefficients for the boundary conditions
	  coeff.applyBoundaryConditionCoefficients(0,0,neumann,allBoundaries);
	  coeff.finishBoundaryConditions();
	  solver.setCoefficientArray( coeff );   // supply coefficients

          cg.update(MappedGrid::THEvertexBoundaryNormal);
	  
	  Index Ig1,Ig2,Ig3;
	  bool singularProblem=TRUE;  
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg[grid];

	    getIndex(mg.dimension(),I1,I2,I3);  
            if( !solver.activeGrid(grid) )
              u[grid](I1,I2,I3)=exact(mg,I1,I2,I3,0);  // assign in-active grids

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
		realArray & normal = mg.vertexBoundaryNormal(side,axis);
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
	  time0=CPU();
	  solver.solve( u,f );   // solve the equations
	  time=CPU()-time0;
	  printf("time for 1st solve of the Neumann problem = %8.2e, (iterations=%i)\n",time,
		 solver.getNumberOfIterations());

	  // turn off refactor for the 2nd solve
	  solver.setRefactor(FALSE);
	  solver.setReorder(FALSE);
	  u=0.;  // initial guess for iterative solvers
	  time0=CPU();
	  solver.solve( u,f );   // solve the equations
	  time=CPU()-time0;
	  printf("time for 2nd solve of the Neumann problem = %8.2e, (iterations=%i)\n\n",time,
		 solver.getNumberOfIterations());

	  error=0.;
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    getIndex(cg[grid].indexRange(),I1,I2,I3);  
            realArray & errg=err[grid];
            w[grid](all,all,all,1)=u[grid];

	    where( cg[grid].mask()(I1,I2,I3)!=0 )
	    {
	      errg(I1,I2,I3,1)=fabs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	    }
	    real maxErr=max(fabs(errg(I1,I2,I3,0))/max(fabs(exact(cg[grid],I1,I2,I3,0))));
            error=max(error,maxErr);
            printf(" ***Neumann: grid=%i : max rel err = %8.2e\n",grid,maxErr);

	    
	    if( Oges::debug & 8 )
	    {
	      errg(I1,I2,I3,1).display("Neumann: abs(error on indexRange +1)");
	      // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
	    }

//  	    where( cg[grid].mask()(I1,I2,I3)!=0 )
//  	      error=max(error,  max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)))/
//  			max(abs(exact(cg[grid],I1,I2,I3,0))) );
//  	    if( Oges::debug & 32 ) 
//  	    {
//  	      getIndex(cg[grid].dimension(),I1,I2,I3);  
//  	      u.display("Computed solution");
//  	      exact(cg[grid],I1,I2,I3,0).display("exact solution");
//  	      abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
//  	    }
	  }
	  printf("***Neumann: Maximum rel error on all grids= %e\n\n",error);  
	  worstError=max(worstError,error);

	  solver.set(OgesParameters::THEcompatibilityConstraint,FALSE); // reset for next solve
	}
	
      }
    }
  }
  

  Overture::finish();          
  return(0);
}


