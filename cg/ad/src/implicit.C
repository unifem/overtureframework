// =============================================================================================
//      Functions for implicit time stepping 
// =============================================================================================


#include "Cgad.h"
#include "AdParameters.h"
#include "Ogshow.h"
#include "Oges.h"
#include "SparseRep.h"
#include "App.h"

void Cgad::
buildImplicitSolvers(CompositeGrid & cg)
// ==========================================================================================
// /Description:
//     Determine the number and type of implicit solvers needed. 
//
//  1) If the equations are decoupled and the boundary conditions for all components are 
//     the same then we can form one scalar implicit system.
//  2) If the equations are decoupled and the boundary conditions are not the same but 
//     decoupled then we can solve separate scalar implicit systems.
//
// ==========================================================================================
{
  real cpu0=getCPU();
  
  int grid, n;
  int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");

  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-
                                 parameters.dbase.get<int >("numberOfExtraVariables");
  int numberOfImplicitSolversNeeded=numberOfComponents;
  scalarSystemForImplicitTimeStepping=true;  // what should this be ??
  

  printF(" *** Cgad: buildImplicitSolvers : numberOfImplicitSolvers=%i ***** \n",numberOfImplicitSolvers);

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{implicitSolve}} 
void Cgad::
formMatrixForImplicitSolve(const real & dt0,
			   GridFunction & cgf1,
			   GridFunction & cgf0 )
// ==========================================================================================
// /Description: This function was once part of implicitSolve.  It was
// broken out to allow the construction of the matrix independently of
// the actual solve.  Basically all the work is done to initialize the
// implicit time stepping.  The implicit method can be optionally used
// on only some grids. To implement this approach we simply create a
// sparse matrix that is just the identity matrix on grids that are
// advanced explicitly but equal to the standard implicit matrix on
// grids that are advance implicitly: 
//  \begin{verbatim} 
//  I - \nu \alpha \dt \Delta on implicit grids 
//  I on explicit grids 
//  \end{verbatim} 
// If the form of the boundary conditions for the different components of
// $\uv$ are the same then we can build a single scalar matrix that
// can be used to advance each component, one after the other. If the
// boundary conditions are not of the same form then we build a matrix
// for a system of equations for the velocity components $(u,v,w)$.
//
// Note that originally cgf1 from implicitSolve was used to get the time,
// grid, and operators.  We are now using whatever is passed in as "u" to
// this function.  The operators should be the same (?) and the time is
// used in the debug output.  What about the grid though? It can change 
// due to AMR (used with implicit?) as well as from the grid velocity.
// /dt0 (input) : time step used to build the implicit matrix.
// /cgf1 (input) : holds the RHS 
// /cgf0 (input) : holds the current state of the solution (used for linearization)
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
  real cpu0=getCPU();
  int grid, n;
  int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");
  
  assert( implicitSolver!=NULL );

  if( parameters.dbase.get<int >("initializeImplicitTimeStepping") )
  {
    // *******************************************
    // **** Initialize Implicit Time Stepping ****
    // *******************************************
    FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
    
    if( debug() & 4 )
      printF(" *** Cgad:formMatrixForImplicitSolve: form the implicit time stepping matrix, t=%9.3e dt0=%8.2e *** \n",
              cgf1.t,dt0);

    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-
      parameters.dbase.get<int >("numberOfExtraVariables");

    CompositeGrid & cg = cgf1.cg;
    CompositeGridOperators & op =  *cgf1.u.getOperators();

    std::vector<real> & kappa = parameters.dbase.get<std::vector<real> >("kappa");

    const bool variableDiffusivity = parameters.dbase.get<bool >("variableDiffusivity");
    const bool variableAdvection = parameters.dbase.get<bool >("variableAdvection");

    // ***** Use predefined equations *****

    // boundaryConditions(side,axis,grid)=OgesParameters::dirichlet; 
    // ****** this is duplicated from insp.C ****** fix this

    for( int imp=0; imp<numberOfImplicitSolvers; imp++ ) 
    {
      // *****************************************
      // **** Initialize the implicit solvers ****
      // *****************************************

      IntegerArray boundaryConditions;
      RealArray boundaryConditionData;
      setOgesBoundaryConditions( cgf1, boundaryConditions, boundaryConditionData,imp );

      implicitSolver[imp].setGrid( cg ); 

      RealArray equationCoefficients(2,cg.numberOfComponentGrids());


      if( parameters.isAxisymmetric() )
	implicitSolver[imp].set(OgesParameters::THEisAxisymmetric,true);

      Range G=cg.numberOfComponentGrids();

      // ListOfShowFileParameters & pdeParameters=parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

      if( debug() & 4 )
      {
	fPrintF(debugFile,">>>> setEquationAndBoundaryConditions for implicit solver %i \n",imp);
	::display(boundaryConditions,"boundaryConditions for Oges",debugFile);
      }

      if( variableDiffusivity )
      {
        // --- variable kappa  ---
	printF("--Cgad::formMatrixForImplicitSolve: form matrix for variable diffusivity at t=%9.3e\n",cgf1.t);
	
	realCompositeGridFunction*& pKappaVar= parameters.dbase.get<realCompositeGridFunction*>("kappaVar");
	if( variableDiffusivity && pKappaVar==NULL )
	{
	  OV_ABORT(" Cgad::formMatrixForImplicitSolve:ERROR:kappaVar not created! ");
	}
        realCompositeGridFunction & kappaVar = *pKappaVar;
	
        // *fix me: 
	realCompositeGridFunction & varCoeff = poissonCoefficients;  // work space 
        Range all;
        varCoeff.updateToMatchGrid(cg,all,all,all,numberOfComponents);
	

        // Oges will form
        //       I + div( scalar grad ) 
        // We set:
        //    scalar = (-implicitFactor*dt)*kappaVar : 
	const real implicitFactor = parameters.dbase.get<real >("implicitFactor");
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ ) // *wdh* 040910 
	{
   	  OV_GET_SERIAL_ARRAY_CONST(real,kappaVar[grid],kappaVarLocal);
   	  OV_GET_SERIAL_ARRAY(real,varCoeff[grid],varCoeffLocal);

	  varCoeffLocal = (-implicitFactor*dt0)*kappaVarLocal;
	}
	
        OgesParameters::EquationEnum equation = OgesParameters::divScalarGradHeatEquationOperator;
	implicitSolver[imp].setEquationAndBoundaryConditions(equation,op,boundaryConditions, boundaryConditionData,
							     equationCoefficients,&varCoeff );
      }
      else
      {
        // -- constant kappa ---
 
        OgesParameters::EquationEnum equation = OgesParameters::heatEquationOperator;

	real kappaDt = parameters.dbase.get<real >("implicitFactor")*kappa[imp]*dt0;
	equationCoefficients(0,G)= 1.;  // for heat equation solve I - alpha*nu*dt* Delta
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ ) // *wdh* 040910 
	{
	  if( parameters.getGridIsImplicit(grid) )
	    equationCoefficients(1,grid)=-kappaDt;
	  else
	    equationCoefficients(1,grid)=0.;
	}

	implicitSolver[imp].setEquationAndBoundaryConditions(equation,op,boundaryConditions, boundaryConditionData,
							     equationCoefficients );
      }
      
      implicitSolver[imp].set(OgesParameters::THEkeepCoefficientGridFunction,false); 

    } // end for( imp
    
    parameters.dbase.get<int >("initializeImplicitTimeStepping")=false;
  } // end initialize implicit time stepping

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;
  
}


//\begin{>>CginsInclude.tex}{\subsection{implicitSolve}} 
void Cgad::
implicitSolve(const real & dt0,
	      GridFunction & cgf1,
              GridFunction & cgf0)
// ==========================================================================================
// /Description:
//    The implicit method can be optionally used on only some grids. To implement this
//   approach we simply create a sparse matrix that is just the identity matrix on grids that
// are advanced explicitly but equal to the standard implicit matrix on grids that are advance
// implicitly:
// \begin{verbatim}
//         I - \nu \alpha \dt \Delta      on implicit grids
//         I                              on explicit grids
// \end{verbatim}
// If the form of the boundary conditions for the different components of $\uv$ are the same
// then we can build a single scalar matrix that can be used to advance each component, one after
// the other. If the boundary conditions are not of the same form then we build a matrix for
// a system of equations for the velocity components $(u,v,w)$.
//
// /dt0 (input) : time step used to build the implicit matrix.
// /cgf1 (input/output) : On input holds the right-hand-side for the implicit equations; on output
//    holds the solution.
// /cgf0 (input) : current best approximation to the solution. Used as initial guess for iterative
//   solvers and used for linearization.
//\end{CginsInclude.tex}  
// ==========================================================================================
{
  real cpu0=getCPU();
  int grid, n;
  int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");
  
  assert( implicitSolver!=NULL );
  
  if( parameters.dbase.get<int >("initializeImplicitTimeStepping") )
  {
    // *******************************************
    // **** Initialize Implicit Time Stepping ****
    // *******************************************

    // kkc 060302 code moved to formMatrixForImplictSolve
    formMatrixForImplicitSolve(dt0, cgf1,cgf0);
    cpu0 = getCPU(); // need to reset this to avoid double counting 
  } // end initialize implicit time stepping

  // we need temporary space
  if( pvIMS==NULL )
    pvIMS= new realCompositeGridFunction;
  if( pwIMS==NULL )
    pwIMS = new realCompositeGridFunction;

  realCompositeGridFunction & v = *pvIMS;
  realCompositeGridFunction & w = *pwIMS; 
  Range all;
  

  int numberOfIterations=0;
  bool isIterativeSolver=false;  // set to true if there is at least one iterative solver
  for( n=parameters.dbase.get<Range >("Rt").getBase(); n<=parameters.dbase.get<Range >("Rt").getBound(); n++ )
  {
    const int imp =  numberOfImplicitSolvers==1 ? 0 : n-parameters.dbase.get<Range >("Rt").getBase();
    if( implicitSolver[imp].isSolverIterative() )
    {
      isIterativeSolver=true;
      break;
      
    }
  }
  
  #ifdef USE_PPP
    bool useLink=false;
  #else
    bool useLink=!isIterativeSolver; // true;  // "true" version is broken for iterative solvers
  #endif

  Range & Rt = parameters.dbase.get<Range >("Rt");
    
  if( scalarSystemForImplicitTimeStepping )
  {
    // solve one component at a time

    if( !useLink )
    {
      v.updateToMatchGrid(cgf1.cg);
    }
    if( isIterativeSolver )
    {
      w.updateToMatchGrid(cgf1.cg);
    }
    
    
    for( n=Rt.getBase(); n<=Rt.getBound(); n++ )
    {
      const int imp =  numberOfImplicitSolvers==1 ? 0 : n-Rt.getBase();
      assert( imp>=0 && imp<numberOfImplicitSolvers );

      if( useLink )
        v.link(cgf1.u,Range(n,n));   // link to a component
      else
      {
	for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
	{
	  // *wdh* 050418 v[grid]=cgf1.u[grid](all,all,all,n);
	  if( implicitSolver[imp].isSolverIterative() )
	  {
	    assign(w[grid],all,all,all,0, cgf1.u[grid],all,all,all,n); // This will be the RHS
	    assign(v[grid],all,all,all,0, cgf0.u[grid],all,all,all,n); // ***** initial guess ****
	  }
	  else
	  {
	    assign(v[grid],all,all,all,0, cgf1.u[grid],all,all,all,n);
	  }
	}
      }

      
      if( debug() & 4 )
	printF("solve implicit time step for component n=%i., imp=%i..\n",n,imp);

      if( implicitSolver[imp].isSolverIterative() )
      {
        // for iterative solvers we need a separate RHS! since the rhs will have points zeroed out.

        // if( !useLink ) w=v;  // rhs
// 	for( grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
//           v[grid]=cgf0.u[grid](nullRange,nullRange,nullRange,n);   // ***** initial guess ****
	

        if( debug() & 4 ) 
	{
	  cgf1.u.display(sPrintF("cgf1.u before implicit solve (n=%i)",n),parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
	  w.display(sPrintF("RHS w=cgf1.u(n) before implicit solve (n=%i)",n),parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
	}
	
	implicitSolver[imp].solve( v,w );  

        numberOfIterations+=implicitSolver[imp].getNumberOfIterations();

        if( debug() & 4 ) 
	{
	  v.display(sPrintF("Solution after implicit solve (n=%i)",n),parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
	}
	

        if( debug() & 2 )
	  printP(" ** implicit time stepping: component %i iterations= %i (t=%e, dt=%8.1e, step=%i, "
                 "max residual=%8.2e)\n",n,implicitSolver[imp].getNumberOfIterations(),
                  cgf1.t,dt,parameters.dbase.get<int >("globalStepNumber"),implicitSolver[imp].getMaximumResidual());

        if( FALSE && implicitSolver[imp].getNumberOfIterations() > 10 )
	{
          fprintf(parameters.dbase.get<FILE* >("debugFile"),"***WARNING*** % iterations required to implicit solve of component n=%i\n",
		  implicitSolver[imp].getNumberOfIterations(),n);
	  v.display("\n ****v (left hand side for implicit solve)",parameters.dbase.get<FILE* >("debugFile"));
	  w.display("\n ****w (right hand side for implicit solve)",parameters.dbase.get<FILE* >("debugFile"));
          char buff[80];
	  for( grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
            display(coeff[grid].sparse->classify,sPrintF(buff,"\n ****classify on grid=%i",grid),parameters.dbase.get<FILE* >("debugFile"));

	}
	
      }
      else
      {
        if( debug() & 4 ) 
	{
          v.display(sPrintF("RHS before implicit solve, component=%i",n),parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
	  // fprintf(parameters.dbase.get<FILE* >("debugFile")," ***Errors before solve for component=%i\n",n);
	  // determineErrors( cgf1 );
	}
	
	implicitSolver[imp].solve( v,v );  

        if( debug() & 4 )
	{
          v.display(sPrintF("Solution after implicit solve, component=%i",n),parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
	  // fprintf(parameters.dbase.get<FILE* >("debugFile")," ***Errors after solve for component=%i\n",n);
	  // determineErrors( cgf1 );
	}
      }
      if( !useLink )
      {
	for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
	{
	  // cgf1.u[grid](all,all,all,n)=v[grid];
          assign(cgf1.u[grid],all,all,all,n, v[grid],all,all,all,0);
	}
	
      }

    }
    numberOfIterations/=max(1,Rt.getLength());
  }
  else
  {
    // Solve for all components at once.
    if( !useLink )
    {
      v.updateToMatchGrid(cgf1.cg,all,all,all,Rt);

      if(implicitSolver[0].isSolverIterative() )
      { // in this case we need an initial guess and a rhs
        w.updateToMatchGrid(cgf1.cg,all,all,all,Rt);
	for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
	{
	  assign(w[grid],all,all,all,Rt, cgf1.u[grid],all,all,all,Rt); // rhs
	  assign(v[grid],all,all,all,Rt, cgf0.u[grid],all,all,all,Rt); // initial guess
	}
      }
      else
      {
	for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
	{
	  assign(v[grid],all,all,all,Rt, cgf1.u[grid],all,all,all,Rt); // rhs
	}
      }
      
    }

    if( implicitSolver[0].isSolverIterative() )
    {
      // for iterative solvers we need a separate RHS!
      if( useLink )  // **** wrong **** 
      {
        v.link(cgf0.u,Rt); // initial guess.
        w.link(cgf1.u,Rt); // rhs 
      }

      implicitSolver[0].solve( v,w );  
      numberOfIterations+=implicitSolver[0].getNumberOfIterations();

      if( useLink )
      {
        w=v;      // copy solution to cgf1.u
      }
      
    }
    else
    {
      if( useLink)
        v.link(cgf1.u,Rt);

      implicitSolver[0].solve( v,v );
    }
    
    if( !useLink )
    {
      // cgf1.u = v;
      for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
      {
	assign(cgf1.u[grid],all,all,all,Rt, v[grid],all,all,all,Rt);
      }
    }
    

    if( implicitSolver[0].isSolverIterative() && debug() & 1 )
      printF(" ** number of iterations to solve implicit time step matrix = %i \n",numberOfIterations);

  }
  
  parameters.dbase.get<int >("numberOfIterationsForImplicitTimeStepping")+=numberOfIterations;
  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;
}



#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


int Cgad::
setOgesBoundaryConditions( GridFunction &cgf, IntegerArray & boundaryConditions, RealArray &boundaryConditionData,
                           const int imp )
// ===================================================================================================================
// /Description:
//   Assign the boundaryCondition data for passing to Oges (predfined equations) when it builds the implicit system.
//
// This function is called by DomainSolver::formMatrixForImplicitSolve
// 
//  /cgf (input) : A grid function holding the current grid.
//  /boundaryConditions (output) : boundary conditions for Oges
//  /boundaryConditionData (output) : boundary condition data for Oges 
//  /imp (input) : the number of the implicit system being solved
// ====================================================================================================================
{
  CompositeGrid & cg = cgf.cg;

  // allocate arrays:
  boundaryConditions.redim(2,3,cg.numberOfComponentGrids());  // for Oges
  boundaryConditions=0;
  boundaryConditionData.redim(2,2,3,cg.numberOfComponentGrids());               // for Oges
  boundaryConditionData=0.;

  const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

  const bool interfaceBoundaryConditionsAreSpecified=parameters.dbase.has_key("interfaceCondition");
  IntegerArray & interfaceCondition = (interfaceBoundaryConditionsAreSpecified ? 
				       parameters.dbase.get<IntegerArray>("interfaceCondition") :
				       Overture::nullIntArray() );
  
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

  RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int tc = parameters.dbase.get<int >("tc");   
  assert( tc==0 ); 

  int grid,side,axis;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const bool isRectanglar=mg.isRectangular();

    mg.update(MappedGrid::THEvertexBoundaryNormal );
	  
    ForBoundary( side,axis )
    {
      int bc = mg.boundaryCondition(side,axis);
      // if( bc == Parameters::interfaceBoundaryCondition && interfaceBoundaryConditionsAreSpecified )
      if( interfaceType(side,axis,grid) != Parameters::noInterface ) 
      {
        // This is an interface
	if( bc!=AdParameters::mixedBoundaryCondition )
	{
	  printP("Cgad:setOgesBC:ERROR:the interface on (side,axis,grid)=(%i,%i,%i)\n"
		 " should have a mixed boundary condition associated with it, but bc=%i\n",
		 side,axis,grid,bc);
	  Overture::abort("error");
	}

        // *** here is the BC we actually use: *** --> we could instead just look at the 
        //   mixedNormalCoeff and mixedCoeff and not use interfaceCondition ?
	bc = interfaceCondition(side,axis,grid);
        assert( bc==Parameters::dirichletInterface || bc==Parameters::neumannInterface );
	printP("** Cgad:setOgesBC: setting an interface bc(%i,%i,%i)=%i, (%s)\n",side,axis,grid,bc,
                  (bc==Parameters::dirichletInterface ? "dirichlet" : "neumann"));
      }
      
      switch( bc )
      {
      case Parameters::dirichletBoundaryCondition:
      case Parameters::interfaceBoundaryCondition:  // for now treat an interface with dirichlet
      case Parameters::dirichletInterface:
	boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  
	break;
      case Parameters::axisymmetric:
	boundaryConditions(side,axis,grid)=OgesParameters::axisymmetric;  // *wdh* 080718
	break;
      case Parameters::neumannBoundaryCondition:
      case AdParameters::mixedBoundaryCondition:
      case Parameters::neumannInterface:
	boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
	
        if( bc!=Parameters::neumannBoundaryCondition )
	{ // assign the mixed-BC coefficients if appropriate
	  real a0 = mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
          if( a0!=0. || a1!=1. )
	  {
	    boundaryConditions(side,axis,grid)=OgesParameters::mixed; 
	    boundaryConditionData(0,side,axis,grid)=a0;
	    boundaryConditionData(1,side,axis,grid)=a1;
	    if( debug() & 2 )
	      printP("*****Cgad:setOgesBC: Set a mixed BC : %f*u + %f*u.n \n",a0,a1);

	  }
          else
	  {
	    if( debug() & 2 )
              printP("*****Cgad:setOgesBC: Set a neumann BC : %f*u + %f*u.n \n",a0,a1);
	  }
	  
	}
	break;
      default:
      {
	if( mg.boundaryCondition(side,axis)>0 )
	{
	  printf(" Cgad::setOgesBoundaryConditions:ERROR: unknown boundary condition, mg.boundaryCondition(%i,%i)=%i\n",
                 side,axis,mg.boundaryCondition(side,axis));
	  Overture::abort("error");
	}
      }
      }
    }
  }
  return 0;
}
