//===============================================================================
//  Coefficient Matrix Example 
//    Solve a System of Equations on a CompositeGrid
//
//Usage: tcm4 [<gridName>] [-solver=<yale|harwell|slap|petsc>] [-debug=<value>] [-noTiming] [-trig] 
//	      [-dirichlet] [-neumann] [-freq=<value>][-outputMatrix]
//==============================================================================
#include "Overture.h"  
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "Checker.h"
#include "ParallelUtility.h"
#include "SparseRep.h"
#include "PlotIt.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

namespace {

  bool measureCPU=TRUE;
  
  enum TWType {
    TWPoly,
    TWTrig
  };
  

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

  
  enum ProblemFlags {
    dirichletFlag = 0x1,
    neumannFlag   = dirichletFlag<<1,
    neumannDirichletFlag = neumannFlag<<1
  };

  int numberOfComponents = 2;
  bool outputMatrix = false;
  const real a1=1., a2=2., a3=3., a4=4.;
  int includeGhost = 1;
  //  const real a1=1., a2=0., a3=1., a4=0.;

  void buildMatrix(ProblemFlags problem, realCompositeGridFunction &coeff)
  {
    coeff = 0.;

    // Solve a system of equations for (u_0,u_1) = (u,v)
    //     a1(  u_xx + u_yy ) + a2*v_x = f_0
    //     a3(  v_xx + v_yy ) + a4*u_y = f_1

    CompositeGridOperators &op = *coeff.getOperators();

    Range e0(0,0), e1(1,1);  // e0 = first equation, e1=second equation
    Range c0(0,0), c1(1,1);  // c0 = first component, c1 = second component
    coeff=a1*op.laplacianCoefficients(e0,c0)+a2*op.xCoefficients(e0,c1)
      +a3*op.laplacianCoefficients(e1,c1)+a4*op.yCoefficients(e1,c0);
    
    if ( problem==dirichletFlag )
      {
	coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);  
	coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);  
	
	coeff.applyBoundaryConditionCoefficients(1,1,dirichlet,  allBoundaries);
	coeff.applyBoundaryConditionCoefficients(1,1,extrapolate,allBoundaries);
      }
    else if ( problem==neumannFlag )
      {
	coeff.applyBoundaryConditionCoefficients(0,0,neumann,  allBoundaries);  
	coeff.applyBoundaryConditionCoefficients(1,1,neumann,  allBoundaries);
      }
    else if ( problem==neumannDirichletFlag )
      {
	coeff.applyBoundaryConditionCoefficients(0,0,neumann,  allBoundaries);  

	coeff.applyBoundaryConditionCoefficients(1,1,dirichlet,  allBoundaries);
	coeff.applyBoundaryConditionCoefficients(1,1,extrapolate,allBoundaries);
      }

    coeff.finishBoundaryConditions();
    if( Oges::debug & 16 ) 
      coeff.display("Here is coeff after finishBoundaryConditions");

  }

  void buildForcing(ProblemFlags problem, realCompositeGridFunction &f, OGFunction &exact)
  {
    f=0.;
    // assign the rhs:  u=exact on the boundary
    CompositeGrid &cg = *f.getCompositeGrid();
    int numberOfDimensions = cg.numberOfDimensions();
    Index I1,I2,I3, Ia1,Ia2,Ia3;
    int side,axis;
    Index Ib1,Ib2,Ib3;
    Index Ig1,Ig2,Ig3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	getIndex(mg.indexRange(),I1,I2,I3);  
	realArray & x= mg.center();
#ifdef USE_PPP
	realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
#else
	const realSerialArray & xLocal = x;
#endif
	f[grid](I1,I2,I3,0)=a1*(exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0))+a2*exact.x(mg,I1,I2,I3,1);
	f[grid](I1,I2,I3,1)=a3*(exact.xx(mg,I1,I2,I3,1)+exact.yy(mg,I1,I2,I3,1))+a4*exact.y(mg,I1,I2,I3,0);
	if( cg.numberOfDimensions()==3 )
	  {
	    f[grid](I1,I2,I3,0)+=a1*exact.zz(mg,I1,I2,I3,0);
	    f[grid](I1,I2,I3,1)+=a3*exact.zz(mg,I1,I2,I3,1);
	  }
	
	if ( problem==dirichletFlag )
	  {
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
	else if ( problem==neumannFlag )
	  {
	    ForBoundary(side,axis)
	      {
#ifdef USE_PPP
		const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
#else
		const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
#endif

		getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		//		realSerialArray fLocal; 
		//		bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ig1,Ig2,Ig3,includeGhost);
		//		if( !ok ) continue; // there are no points on this processor.
		const int rectangularForTZ=0;
		realSerialArray uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3);
		if( mg.boundaryCondition()(side,axis) > 0 )
		  {
		    for ( int n=0; n<numberOfComponents; n++ )
		      {
			exact.gd( uex,xLocal,numberOfDimensions,rectangularForTZ,0,1,0,0,Ib1,Ib2,Ib3,n,0.);
			exact.gd( uey,xLocal,numberOfDimensions,rectangularForTZ,0,0,1,0,Ib1,Ib2,Ib3,n,0.);
			f[grid](Ig1,Ig2,Ig3,n) = normal(Ib1,Ib2,Ib3,0)*uex + normal(Ib1,Ib2,Ib3,1)*uey;
			if( numberOfDimensions==3 )
			  {
			    exact.gd( uex,xLocal,numberOfDimensions,rectangularForTZ,0,0,0,1,Ib1,Ib2,Ib3,n,0.);  // uex = T.z
			    f[grid](Ig1,Ig2,Ig3,n) +=normal(Ib1,Ib2,Ib3,2)*uex;
			  }
		      } // for each component
		  } // if a real boundary
	      } // for boundary
	  } // if neumann
	else if ( problem==neumannDirichletFlag )
	  {
	    ForBoundary(side,axis)
	      {
#ifdef USE_PPP
		const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
#else
		const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
#endif
		if( mg.boundaryCondition()(side,axis) > 0 )
		  {
		    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

		    // neumann condition on the first variable
		    const int rectangularForTZ=0;
		    realSerialArray uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3);
		    exact.gd( uex,xLocal,numberOfDimensions,rectangularForTZ,0,1,0,0,Ib1,Ib2,Ib3,0,0.);
		    exact.gd( uey,xLocal,numberOfDimensions,rectangularForTZ,0,0,1,0,Ib1,Ib2,Ib3,0,0.);
		    f[grid](Ig1,Ig2,Ig3,0) = normal(Ib1,Ib2,Ib3,0)*uex + normal(Ib1,Ib2,Ib3,1)*uey;
		    if( numberOfDimensions==3 )
		      {
			exact.gd( uex,xLocal,numberOfDimensions,rectangularForTZ,0,0,0,1,Ib1,Ib2,Ib3,0,0.);  // uex = T.z
			f[grid](Ig1,Ig2,Ig3,0) +=normal(Ib1,Ib2,Ib3,2)*uex;
		      }

		    // dirichlet condition on the second variable
		    f[grid](Ib1,Ib2,Ib3,1)=exact(mg,Ib1,Ib2,Ib3,1);
		  }
	      }
	  } // if neumann+dirichlet

      } // makeForcing
    
  } // anonymous namespace

  void solveSystem(int solverType, int numberOfConstraints, OGFunction &exact, 
		   realCompositeGridFunction &coeff, realCompositeGridFunction &f, realCompositeGridFunction &u, const real tol )
  {
    CompositeGrid &cg = *coeff.getCompositeGrid();
    Oges solver( cg );                     // create a solver
    int numberOfDimensions = cg.numberOfDimensions();
    int numberOfGrids = cg.numberOfComponentGrids();
    solver.setCoefficientArray( coeff );   // supply coefficients
    solver.set(OgesParameters::THEsolverType,solverType); 
    if( solverType==OgesParameters::SLAP ||  solverType==OgesParameters::PETSc )
      {
	solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
	solver.set(OgesParameters::THEtolerance,max(tol,REAL_EPSILON*10.));
      } 

    if ( numberOfConstraints==1 )
      {
	// this should cause the automatic creation of the compatibility constraint on the first equation...
	solver.set(OgesParameters::THEcompatibilityConstraint,true);
	solver.initialize(); // this will form the right null vector
	realCompositeGridFunction ue(cg);
	exact.assignGridFunction(ue,0.);
	real value=0.;
	solver.evaluateExtraEquation(ue,value);

	solver.setExtraEquationValues(f,&value );
      }
    else if ( numberOfConstraints==2 )
      {
	// kkc 090903
	// Because we have a system of two equations we cannot use the built-in compatibility constraint (which assumes one constraint).
	// The following code sets up a constraint for each equation (total of two extra equations) and adds them as a user supplied constraint.
	solver.initialize();
	Range all;
	real nullVectorScaling = 0;
	solver.get(OgesParameters::THEnullVectorScaling, nullVectorScaling);
	realCompositeGridFunction &constraint = solver.rightNullVector;
	constraint.updateToMatchGrid(cg,all,all,all,numberOfComponents);
	constraint=0.;
	Index I1,I2,I3;
	for ( int n=0; n<numberOfComponents; n++ )
	  {
	    for( int grid=numberOfGrids-1; grid>=0; grid-- ) // why is this loop backwards? I took it directly from makeRightNullVector...
	      {
		MappedGrid & c = cg[grid];
		IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;
		
		getIndex(c.dimension(),I1,I2,I3);
		real scale = (n+1)*nullVectorScaling; // multiply by n+1 so that each equation gets a different scaling and we can see it in the matix
		// do not include the ghost line so that we retain u.n=0 as a BC!
		where( classifyX(I1,I2,I3,n)==SparseRepForMGF::interior || classifyX(I1,I2,I3,n)==SparseRepForMGF::boundary )
		  constraint[grid](I1,I2,I3,n)=scale;
	      }
	  }
	solver.numberOfExtraEquations = 2; // this should probably be in OgesParameters...
	solver.set(OgesParameters::THEcompatibilityConstraint,true);
	solver.set(OgesParameters::THEuserSuppliedCompatibilityConstraint,true);
	solver.updateToMatchGrid( cg ); // why do we need this? it will call initialize...

	realCompositeGridFunction ue(cg,all,all,all,numberOfComponents);
	exact.assignGridFunction(ue,0.);
	ArraySimple<real> value(numberOfComponents);
	value = 0.;
	for ( int n=0; n<numberOfComponents; n++ )
	  {
	    solver.evaluateExtraEquation(ue,value[n],n);
	  }
	solver.setExtraEquationValues(f,value.ptr());
	if( Oges::debug & 16 ) 
	  {
	    constraint.display("here are the constraint coefficients");
	    cout<<"here are the extra equation values"<<value<<endl;
	  }
      }

    if( outputMatrix )
      solver.set(OgesParameters::THEkeepSparseMatrix,true);
    
    u=0.;  // for interative solvers.
    real time0=getCPU();
    solver.solve( u,f );   // solve the equations
    
    printf("residual=%8.2e, time for solve = %8.2e (iterations=%i)\n",
	   solver.getMaximumResidual(),getCPU()-time0,solver.getNumberOfIterations());
    
    if( false ) // kkc 090903, changed from true to false, why was solve being called twice??
      {
	solver.solve( u,f );   // solve the equations
	
	printf("residual=%8.2e, time for 2nd solve = %8.2e (iterations=%i)\n",
	       solver.getMaximumResidual(),getCPU()-time0,solver.getNumberOfIterations());
	
      }
  }

  real computeMaxError(int n, realCompositeGridFunction &u, OGFunction &exact)
  {
    CompositeGrid &cg = *u.getCompositeGrid();
    Index I1,I2,I3;
    //    for( int n=0; n<numberOfComponents; n++ )
    //      {
    real error=0.;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].indexRange(),I1,I2,I3);  
	realArray err = (u[grid](I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n))/max(abs(exact(cg[grid],I1,I2,I3,n)));
	where( cg[grid].mask()(I1,I2,I3)!=0 )
	  {
	    error=max(error,max(abs(err)));
	  }
	if( Oges::debug & 4 ) 
	  {
	    abs(u[grid](I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n)).display("abs(error)");
	    u.display("u");

	  }
      }
    //	PlotIt::contour(*Overture::getGraphicsInterface(),u);
    //	printf("Maximum relative error in component %i with dirichlet bc's= %e\n",n,error);  
    //	checker.printMessage(msg,error,time);
    return error;
  }
  //  }
}

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
  Overture::start(argc,argv);  // initialize Overture

  printF("Usage: tcm4 [<gridName>] [-solver=<yale|harwell|slap|petsc>] [-debug=<value>] [-noTiming] [-trig] \n"
	 "            [-dirichlet] [-neumann] [-neumann+dirichlet] [-freq=<value>][-outputMatrix] [-check]\n");

  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
  const int maxNumberOfSolversToTest = 4;
  int numberOfSolversToTest = maxNumberOfSolversToTest;
  aString solverName[maxNumberOfSolversToTest] = {"yale","harwell","slap","petsc"};
  int solverType[maxNumberOfSolversToTest] = {OgesParameters::yale,
					      OgesParameters::harwell,
					      OgesParameters::SLAP,
					      OgesParameters::PETSc};
  
  real tol=1.e-8;
  BCTypes::BCNames bcTest = dirichlet;
  int problemsToSolve = dirichletFlag|neumannFlag|neumannDirichletFlag;
  TWType twType = TWPoly;
  real fx=2., fy=2., fz=2.; // frequencies for trig TZ
  int degreeOfSpacePolynomial = 2;
  int degreeOfTimePolynomial = 1;
  int len=0;
  if( argc >= 1 )
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
      else if (arg=="outputMatrix" )
      {
	outputMatrix=true;
      }
      else if ( arg=="-dirichlet" )
      {
	problemsToSolve = dirichletFlag;
      }
      else if ( arg=="-neumann" )
      {
	problemsToSolve = neumannFlag;
      }
      else if ( arg=="-neumann+dirichlet" )
      {
	problemsToSolve = neumannDirichletFlag;
      }
      else if ( arg=="-trig" )
      {
	twType = TWTrig;
      }
      else if( arg(0,7)=="-solver=" )
      {
        aString solver=arg(8,arg.length()-1);
        if( solver=="yale" )
          solverType[0]=OgesParameters::yale;
	else if( solver=="harwell" )
          solverType[0]=OgesParameters::harwell;
	else if( solver=="slap" )
          solverType[0]=OgesParameters::SLAP;
        else if( solver=="petsc" )
          solverType[0]=OgesParameters::PETSc;
	else
	{
	  printf("Unknown solver=%s \n",(const char*)solver);
	  throw "error";
	}
	
	numberOfSolversToTest = 1;
	solverName[0] = solver;

	//	printf("Setting solverType=%i\n",solver);
      }
      else if ( arg=="-check" )
      {
	//numberOfSolversToTest=2;
	//solverName[0] = "yale"; solverType[0] = OgesParameters::yale;
	//solverName[1] = "slap"; solverType[1] = OgesParameters::SLAP;
        // *wdh* 100608 -- only check slap to make the test faster
	numberOfSolversToTest=1;
	solverName[0] = "slap"; solverType[0] = OgesParameters::SLAP;
        tol=1.e-3;
      }
      else if( (len=arg.matches("-freq=")) )
      {
	sScanF(arg(len,arg.length()-1),"%e",&fx);
	fy=fx; fz=fx;
	printF("Setting fx=fy=fz=%e\n",fx);
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=arg;
      }
    }
  }
  //kkc 090902  else
  //kkc 090902    cout << "Usage: `tcm4 [<gridName>] [-solver=[yale][harwell][slap][petsc]] [-debug=<value>] -noTiming' \n";

  aString checkFileName;
  if( REAL_EPSILON == DBL_EPSILON )
    checkFileName="tcm4.dp.check.new";  // double precision
  else  
    checkFileName="tcm4.sp.check.new";
  Checker checker(checkFileName);  // for saving a check file.
  checker.setCutOff(0.); // for initial testing of the modified code...

  real worstError=0.;

  for ( int ls=0; ls<numberOfSolversToTest; ls++ )
    {
      checker.setLabel(solverName[ls],0);
      for( int it=0; it<numberOfGridsToTest; it++ )
	{
	  aString nameOfOGFile=gridName[it];
	  checker.setLabel(nameOfOGFile,1);
	  
	  cout << "\n *****************************************************************\n";
	  cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
	  cout << " *****************************************************************\n\n";
	  
	  CompositeGrid cg;
	  getFromADataBase(cg,nameOfOGFile);
	  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal);
	  
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
	  OGFunction *exactP = (twType==TWPoly) ? (OGFunction *)new OGPolyFunction(degreeOfSpacePolynomial,
										   cg.numberOfDimensions(),
										   numberOfComponents,	 
										   degreeOfTimePolynomial) :
                                                  (OGFunction *)new OGTrigFunction(fx,fy,fz);
	  OGFunction &exact = *exactP;

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
	  //	  PlotIt::contour(*Overture::getGraphicsInterface(),u);

	  CompositeGridOperators op(cg);                            // create some differential operators 
	  op.setNumberOfComponentsForCoefficients(numberOfComponents);
	  u.setOperators(op);                              // associate differential operators with u
	  coeff.setOperators(op);
	  aString msg;
	  real time0 = getCPU();
	  if ( problemsToSolve & dirichletFlag )
	    {
	      buildMatrix(dirichletFlag, coeff);
	      buildForcing(dirichletFlag, f, exact);
	      solveSystem(solverType[ls],0,exact,coeff,f,u,tol);
	      real time = getCPU()-time0;
	      
	      for ( int n=0; n<numberOfComponents; n++ )
		{
		  real error = computeMaxError(n,u,exact);
		  sPrintF(msg,"dirichlet: error (n=%i)",n);
		  checker.printMessage(msg,error,time);
		  worstError=max(worstError,error);
		}
	    }

	  if ( problemsToSolve & neumannDirichletFlag )
	    { // we put this problem here so that we don't confuse the sparse rep classify with two extra equations from the all neumann case
	      buildMatrix(neumannDirichletFlag, coeff);
	      buildForcing(neumannDirichletFlag, f, exact);
	      solveSystem(solverType[ls],1,exact,coeff,f,u,tol);
	      real time = getCPU()-time0;
	      
	      for ( int n=0; n<numberOfComponents; n++ )
		{
		  real error = computeMaxError(n,u,exact);
		  sPrintF(msg,"neumann-dirichlet: error (n=%i)",n);
		  checker.printMessage(msg,error,time);
		  worstError=max(worstError,error);
		}
	    }
	  
	  if ( problemsToSolve & neumannFlag )
	    {
	      buildMatrix(neumannFlag, coeff);
	      buildForcing(neumannFlag, f, exact);
	      solveSystem(solverType[ls],2,exact,coeff,f,u,tol);
	      real time = getCPU()-time0;
	      
	      for ( int n=0; n<numberOfComponents; n++ )
		{
		  real error = computeMaxError(n,u,exact);
		  sPrintF(msg,"neumann: error (n=%i)",n);
		  checker.printMessage(msg,error,time);
		  worstError=max(worstError,error);
		}
	    }


	  // u.display("Here is the solution to u.xx+u.yy=f");
	  delete exactP; exactP=0;

	} // end loop over grids
    } // end loop over solvers to test

  printf("\n\n ************************************************************************************************\n");
  if( worstError > .025 )
    printf(" ************** Warning, there is a large error somewhere, worst error =%e ******************\n",
	   worstError);
  else
    printf(" ************** Test apparently successful, worst error =%e ******************\n",worstError);
  printf(" **************************************************************************************************\n\n");

  Overture::finish();          
  return(0);
}

