#include "Cgins.h"
#include "Parameters.h"
#include "MappedGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "Oges.h"
#include "SparseRep.h"
#include "App.h"
#include "GridMaterialProperties.h"

#define insimp EXTERN_C_NAME(insimp)
extern "C"
{
void insimp(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
            const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
            const int& mask,const real&xy,const real&rsxy, real&radisuInverse, const real&u, 
            const int&ndc, real&coeff, real&fe, real&fi, const real &ul, const real&gv,
            const real & gvl, const real&dw,
            const int & ndMatProp, const int& matIndex, const real& matValpc, const real& matVal,
            const int&bc, const int&boundaryCondition, 
            const int&ndbcd1a,const int&ndbcd1b,const int&ndbcd2a,const int&ndbcd2b,
            const int&ndbcd3a,const int&ndbcd3b,const int&ndbcd4a,const int&ndbcd4b,const real&bcData,
            const int&nde, int&equationNumber, int&classify,
            const int&nr1a,const int&nr1b,const int&nr2a,const int&nr2b,const int&nr3a,const int&nr3b,
            const int&ipar, const real&rpar, const DataBase *pdb, int&ierr );
}


// in common/src/getBounds.C : (should use new version in ParallelGridUtility.h)
// void
// getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                      IntegerArray & gidLocal, 
//                                      IntegerArray & dimensionLocal, 
//                                      IntegerArray & bcLocal );


#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
    int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
    int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
    for(i3=I3Base; i3<=I3Bound; i3++) \
    for(i2=I2Base; i2<=I2Bound; i2++) \
    for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
    I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
    I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
    for(i3=I3Base; i3<=I3Bound; i3++) \
    for(i2=I2Base; i2<=I2Bound; i2++) \
    for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_M(m,M)\
    int mBase=M.getBase(), mBound=M.getBound(); \
    for(m=mBase; m<=mBound; m++)

#define U(c)     u(I1,I2,I3,c)   
#define UU(c)   uu(I1,I2,I3,c)
#define UX(c)   ux(I1,I2,I3,c)
#define UY(c)   uy(I1,I2,I3,c)
#define UZ(c)   uz(I1,I2,I3,c)


void Cgins::
buildImplicitSolvers(CompositeGrid & cg)
// ==========================================================================================
// /Description:
//     Determine the number and type of implicit solvers needed. Depending on the boundary
//
//  1) If the equations are decoupled and the boundary conditions for all components are 
//     the same then we can form one scalar implicit system.
//  2) If the equations are decoupled and the boundary conditions are not the same but 
//     decoupled then we can solve separate scalar implicit systems.
//  3) If the boundary conditions or equations are coupled then we solve a implicit system.  
//
// ==========================================================================================
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return;

  real cpu0=getCPU();
  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");

  int numberOfImplicitSolversNeeded=1;
  // numberOfImplicitVelocitySolvers : counts the number of separate Oges solvers needs to solve for (u,v,w)
  int & numberOfImplicitVelocitySolvers = parameters.dbase.get<int>("numberOfImplicitVelocitySolvers");
  int & implicitSolverForTemperature = parameters.dbase.get<int>("implicitSolverForTemperature");
  bool & useFullSystemForImplicitTimeStepping = parameters.dbase.get<bool >("useFullSystemForImplicitTimeStepping");
  
  int grid, n;
  int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");
  
  // *******************************************
  // **** Initialize Implicit Time Stepping ****
  // *******************************************

  if( debug() & 1 )
    printF(" *** Cgins: buildImplicitSolvers: Determine the type of implicit solver(s) to build ***** \n");


  // --- first check to see if the BC's for (u,v,w) are all of the same form ---
  // --- in which case we only need to form a scalar implicit system         ---

  if( parameters.dbase.get<InsParameters::ImplicitVariation>("implicitVariation")!=InsParameters::implicitViscous )
//      pdeModel==InsParameters::twoPhaseFlowModel )
  {
    useFullSystemForImplicitTimeStepping=true;
  }


  if( !useFullSystemForImplicitTimeStepping )
  {
    scalarSystemForImplicitTimeStepping=true;
    if( parameters.isAxisymmetric() )
    {
      // for axisymmetric, the u and v interior equations are different so we either need 2 scalar solvers
      // or we need to solve a system
      numberOfImplicitSolversNeeded=cg.numberOfDimensions();
      assert( cg.numberOfDimensions()==2 );
    }
    for( grid=0; grid<cg.numberOfComponentGrids() && scalarSystemForImplicitTimeStepping; grid++ )
    {
      if( parameters.getGridIsImplicit(grid) )
      {
	for( int axis=0; axis<cg.numberOfDimensions() && scalarSystemForImplicitTimeStepping ; axis++ )
	{
	  for( int side=Start; side<=End; side++ )
	  {
	    int bc=cg[grid].boundaryCondition(side,axis);
	    if( bc>0 && 
		bc!=Parameters::noSlipWall && 
		bc!=InsParameters::inflowWithVelocityGiven &&
		bc!=InsParameters::outflow &&
		bc!=Parameters::dirichletBoundaryCondition &&
                bc!=Parameters::freeSurfaceBoundaryCondition )  // **for now **
	    {
	      if( bc==Parameters::slipWall ||
                  bc==InsParameters::inflowWithPressureAndTangentialVelocityGiven ) // *wdh* 090725
	      {
		if( cg[grid].isRectangular() && 
                    !parameters.gridIsMoving(grid) ) // added check or moving grid *wdh* 2015/11/28
		{
		  // BC's are not the same but they are decoupled -- we can use multiple scalar solvers.
		  numberOfImplicitSolversNeeded=cg.numberOfDimensions();
		}
		else
		{
		  numberOfImplicitSolversNeeded=1;
		  scalarSystemForImplicitTimeStepping=false;
                  useFullSystemForImplicitTimeStepping=true;  // *wdh* 110318
		  break;
		}
	      }
	      else if(  bc==Parameters::axisymmetric ) // *wdh* 080817 
	      {
                numberOfImplicitSolversNeeded=cg.numberOfDimensions();
                assert( cg.numberOfDimensions()==2 );
	      }
	      else
	      {
		if( bc!=Parameters::axisymmetric && bc!=Parameters::slipWall )
		{
		  printF("Cgins:implicitSolve:implicit time stepping: unknown boundary condition bc(%i,%i)=%i "
                         "grid=%s\n",side,axis,bc,(const char*)cg[grid].getName());
		  Overture::abort("error");
		}
		numberOfImplicitSolversNeeded=1;
		scalarSystemForImplicitTimeStepping=false;
                useFullSystemForImplicitTimeStepping=true;  // *wdh* 110318
		break;
	      }
	    }
	  }
	}
      }
    }
  }
  else
  {
    printF("++++ Cgins:buildImplicitSolvers: forcing a system to be solved +++\n");
    scalarSystemForImplicitTimeStepping=false;
    numberOfImplicitSolversNeeded=1;
  }
  
  
  // *wdh* 080817 -- we can now handle multiple scalar implicit solvers and artificial diffusion or axisymmetric
//  if( scalarSystemForImplicitTimeStepping && numberOfImplicitSolversNeeded>1 &&
//      ( ( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") &&
//	  parameters.dbase.get<int>("useNewImplicitMethod")==0 )
////        || 
// 	( parameters.isAxisymmetric() &&
// 	  parameters.dbase.get<int>("useNewImplicitMethod")==0 )) 
//	))
  // *wdh* 090726 -- new implicit method insImp is not implemented for AD with scalar systems
  if( false &&   // *wdh* 100406 -- insImpINS now seems to support AD
      parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
  {
    if( scalarSystemForImplicitTimeStepping )
    {
      printF("Cgins:buildImplicitSolvers: INFO: I could use a scalar system with %i number of solvers but\n"
	     "  this case is not implemented for 2nd-order artificial diffusion.\n",
	     numberOfImplicitSolversNeeded);
    }
    scalarSystemForImplicitTimeStepping=false;
    numberOfImplicitSolversNeeded=1;
  }

  numberOfImplicitVelocitySolvers=numberOfImplicitSolversNeeded;  

  // ************* what is this next here for ??
  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel )
    numberOfImplicitSolversNeeded++;
  
  
  if( parameters.dbase.get<InsParameters::PDEModel >("pdeModel")==InsParameters::BoussinesqModel &&
      !useFullSystemForImplicitTimeStepping )
  { 
    // add an implicit solver for the Temperature (unless we solve the full coupled system)
    if( debug() & 1 )
      printF(" *** Cgins:buildImplicitSolvers: build an implicit solver for the Temperature\n");

    implicitSolverForTemperature=numberOfImplicitSolversNeeded;
    numberOfImplicitSolversNeeded++;
  }
  

  if( scalarSystemForImplicitTimeStepping )
  {
    if( numberOfImplicitVelocitySolvers==1 )
      printF("Cgins:buildImplicitSolvers: implicit time stepping for velocity is for a scalar system "
             "since BC's are consistent.\n");
    else
      printF("Cgins:buildImplicitSolvers: implicit time stepping for velocity uses %i scalar systems "
             "since BC's decouple.\n",numberOfImplicitVelocitySolvers);
  }
  else
  {
    if( !useFullSystemForImplicitTimeStepping )

      printF("Cgins:buildImplicitSolvers: implicit time stepping for velocity is for a system since BC's are NOT consistent\n");
    else
      printF("Cgins:buildImplicitSolvers: using a full system for the velocity for implicit time stepping\n");
  }
  
  if( numberOfImplicitSolversNeeded!=numberOfImplicitSolvers )
  {
    delete [] implicitSolver;
    implicitSolver=NULL;
    numberOfImplicitSolvers=numberOfImplicitSolversNeeded;
    if( numberOfImplicitSolvers>0 )
      implicitSolver= new Oges [numberOfImplicitSolvers];

    // For multigrid we wish to share the multigrid hierarchy so we create an object here and give it to Oges
    if( !parameters.dbase.has_key("multigridCompositeGrid") ) parameters.dbase.put<MultigridCompositeGrid>("multigridCompositeGrid");
    MultigridCompositeGrid & mgcg = parameters.dbase.get<MultigridCompositeGrid>("multigridCompositeGrid");
    for( int imp=0; imp<numberOfImplicitSolvers; imp++ )
    {
      implicitSolver[imp].set(mgcg);
    }
    
  }

  printF(" *** Cgins:buildImplicitSolvers : numberOfImplicitSolvers=%i ***** \n",numberOfImplicitSolvers);

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{implicitSolve}} 
void Cgins::
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
// /cgf1 (input) : (new) holds the RHS 
// /cgf0 (input) : (current) holds the current state of the solution (used for linearization)
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return;

  int & globalStepNumber = parameters.dbase.get<int >("globalStepNumber");
  
  bool recomputeMatrix = parameters.dbase.get<int >("initializeImplicitTimeStepping")==1 ||
                         (globalStepNumber>0 &&  
                         (globalStepNumber % parameters.dbase.get<int >("refactorFrequency")) ==0 );

  if( !recomputeMatrix ) return;
  
  parameters.dbase.get<int >("initializeImplicitTimeStepping")=1;
  if( debug() & 2 )
  {
    printF(">>>Cgins::formMatrixForImplicitSolve: recompute matrix: globalStepNumber=%i refactorFrequency=%i recomputeMatrix=%i\n",
	   globalStepNumber,parameters.dbase.get<int >("refactorFrequency"),recomputeMatrix);
  }

  real cpu0=getCPU();
  int grid, n;
  int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");
  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");

  const bool & useSecondOrderArtificialDiffusion = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion");
  
  // numberOfImplicitVelocitySolvers : counts the number of separate Oges solvers needs to solve for (u,v,w)
  int & numberOfImplicitVelocitySolvers = parameters.dbase.get<int>("numberOfImplicitVelocitySolvers");
  int & implicitSolverForTemperature = parameters.dbase.get<int>("implicitSolverForTemperature");
  
  assert( implicitSolver!=NULL );

  // *******************************************
  // **** Initialize Implicit Time Stepping ****
  // *******************************************
  if( debug() & 4 )
    printF(" ***Cgins::formMatrix... initialize implicit time stepping for viscous terms, "
	   "t=%9.3e dt0=%8.2e ***** \n",cgf1.t,dt0);

  CompositeGrid & cg = cgf1.cg;  // for moving grids this is the grid at the new time 


  CompositeGridOperators & op =  *cgf1.u.getOperators();

  if( parameters.saveLinearizedSolution() )
  {
    // save a copy of cgf0.u -- this is the solution we linearize about 
    printF(" $$$$ save the linearized solution $$$$ (and refactor the matrix at t=%9.3e, dt0=%e)\n",cgf1.t,dt0);
    if( puLinearized==NULL )
    {
      Range all;
      puLinearized= new realCompositeGridFunction(cg,all,all,all,cgf0.u[0].dimension(3));
    }
    *puLinearized=cgf0.u;

    // For moving grid problems we also save the grid velocity 
    if( parameters.isMovingGridProblem() )
    {
      if( pGridVelocityLinearized==NULL )
      {
	pGridVelocityLinearized = new realMappedGridFunction [cg.numberOfComponentGrids()];
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
          if( parameters.gridIsMoving(grid) )
	  {
            realMappedGridFunction & gv = cgf0.getGridVelocity(grid);
            pGridVelocityLinearized[grid].updateToMatchGridFunction(gv);
	  }
	}
      }
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        pGridVelocityLinearized[grid]=cgf0.getGridVelocity(grid);
      }
      
    }
    
  }


  //if( scalarSystemForImplicitTimeStepping &&  
  //    ( !useSecondOrderArtificialDiffusion || parameters.dbase.get<int>("useNewImplicitMethod")==1) )

  // *new* 080816 -- all methods use this first section: 
  if( true )
  {

    for( int imp=0; imp<numberOfImplicitSolvers; imp++ ) 
    {
      // *****************************************
      // **** Initialize the implicit solvers ****
      // *****************************************

      bool outOfDate = false;  // We indicate when the grid changes in updateForMovingGrids
      implicitSolver[imp].setSolverName(sPrintF("CginsImplicitSolver%i",imp));
      implicitSolver[imp].setGrid( cg,outOfDate ); 

      bool useTurbulenceModel= (parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel &&
                                parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::LargeEddySimulation);
      
      assert( !scalarSystemForImplicitTimeStepping || !useTurbulenceModel );
	

      bool usePredefinedEquations = scalarSystemForImplicitTimeStepping && !useSecondOrderArtificialDiffusion;

      // For axisymmetric: the coefficients for u and v are not the same: we cannot use the predefined eqns.
      if( parameters.isAxisymmetric() )
         usePredefinedEquations=false; 

      // We cannot use predefined For Boussinesq and variable material parameters:
      if( pdeModel==InsParameters::BoussinesqModel && imp==implicitSolverForTemperature &&
          parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
      {
	usePredefinedEquations=false; 
      }
      
      if( false )
	printF("Cgins::formMatrix... Implicit time stepping: numberOfImplicitSolvers=%i, imp=%i usePredefinedEquations=%i\n",
                numberOfImplicitSolvers,imp,(int)usePredefinedEquations);


      // Evaluate the boundary conditions for Oges
      IntegerArray boundaryConditions;
      RealArray boundaryConditionData;
      setOgesBoundaryConditions( cgf1, boundaryConditions, boundaryConditionData,imp );

      if( usePredefinedEquations )
      {
	if( debug() & 4 ) 
	  printF("Cgins::formMatrix... Implicit time stepping: use predefined equations for imp=%i, t=%9.3e\n",
            imp,cgf1.t);

	// **** Constant viscosity case *****

	RealArray equationCoefficients(2,cg.numberOfComponentGrids());

	OgesParameters::EquationEnum equation = OgesParameters::heatEquationOperator;
	if( parameters.isAxisymmetric() )
	  implicitSolver[imp].set(OgesParameters::THEisAxisymmetric,true);

	Range G=cg.numberOfComponentGrids();


	real diffusionCoefficent=parameters.dbase.get<real >("nu");
	if( pdeModel==InsParameters::BoussinesqModel && imp==implicitSolverForTemperature )
	{
	  // --- Temperature equation ---

	  diffusionCoefficent=parameters.dbase.get<real >("kThermal");

	  if( debug() & 2 )
	    printF(" ***Cgins::formMatrix: form matrix for Temperature equation kThermal=%f\n",diffusionCoefficent);
	}
	    
	real nuDt = parameters.dbase.get<real >("implicitFactor")*diffusionCoefficent*dt0;

	equationCoefficients(0,G)= 1.;  // for heat equation solve I - nuDt* Delta

	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ ) // *wdh* 040910 
	{
	  if( parameters.getGridIsImplicit(grid) )
	    equationCoefficients(1,grid)=-nuDt;
	  else
	    equationCoefficients(1,grid)=0.;
	}
	    
	if( debug() & 1 && imp==implicitSolverForTemperature )
	  printF(">>>> Cgins: implicit: set predefined equations for TEMPERATURE \n");
	
	implicitSolver[imp].setEquationAndBoundaryConditions(equation,op,boundaryConditions, boundaryConditionData,
							     equationCoefficients );
	    
        implicitSolver[imp].set(OgesParameters::THEkeepCoefficientGridFunction,false); 
      }
      else
      {
        // **********************************************
	// *** We do not use the predefined equations ***
        // **********************************************

	Range & Rt = parameters.dbase.get<Range >("Rt");       // time dependent components
	Range & Rtimp = parameters.dbase.get<Range >("Rtimp"); // time dependent components that may be treated implicitly

	int numberOfTimeDependentComponents = Rt.getLength();
	int numberOfImplicitTimeDependentComponents = Rtimp.getLength();
	const int numberOfComponentsForCoefficients = scalarSystemForImplicitTimeStepping ? 1 : 
	  numberOfImplicitTimeDependentComponents;

	if( debug() & 4 )
	{
	  printF(" @@@@@@@@@@ formImplicit: imp=%i numberOfComponentsForCoefficients=%i,"
		 " useSecondOrderArtificialDiffusion=%i @@@@@@@@@@@\n",
		 imp,numberOfComponentsForCoefficients,(int)useSecondOrderArtificialDiffusion);
	}
	
	// make a grid function to hold the coefficients
	Range all;
	// *wdh* 060929 int stencilWidth = 2*parameters.dbase.get<int >("orderOfAccuracy") + 1;
	int stencilWidth = parameters.dbase.get<int >("orderOfAccuracy") + 1;
	const int numberOfGhostLines=parameters.numberOfGhostPointsNeededForImplicitMatrix();
	if( numberOfGhostLines==2 && parameters.dbase.get<int >("orderOfAccuracy")==2 )
	  stencilWidth=5;

	int stencilSize=int( pow(stencilWidth,cg.numberOfDimensions())+1 );   // add 1 for interpolation equations

	int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);
    
        if( implicitCoeff==NULL )
	{
	  implicitCoeff = new realCompositeGridFunction [numberOfImplicitSolvers];
	}
	
        // use this coeff matrix: 
	realCompositeGridFunction & coeff = implicitCoeff[imp];

	coeff.updateToMatchGrid(cg,stencilDimension,all,all,all); 
	coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines,numberOfComponentsForCoefficients);
    
	// We need to change the stencil size from that used by the pressure solve.
	op.setStencilSize(stencilSize);
	op.setNumberOfComponentsForCoefficients(numberOfComponentsForCoefficients);
	coeff.setOperators(op);
	coeff = 0.0;
	// Form the implicit system on each grid.
	int grid;
	bool isSingular = isImplicitMatrixSingular( cgf0.u );
	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid &mg = cg[grid];

	  realMappedGridFunction & gridVelocity = parameters.gridIsMoving(grid) ? cgf0.getGridVelocity(grid) : cgf0.u[grid];
	  formImplicitTimeSteppingMatrix(coeff[grid],dt0,scalarSystemForImplicitTimeStepping,
					 cgf0.u[grid],gridVelocity,grid,imp );

	}

	coeff.finishBoundaryConditions(); //kkc 060406
	// coeff.display("Here is coeff after finishBoundaryConditions");

	if ( isSingular )  //kkc 060728
	{ 
	  addConstraintEquation(parameters,implicitSolver[imp],coeff,cgf0.u,cgf1.u,numberOfComponentsForCoefficients);
	}
	else
	{
           // supply coefficients
	  if( true )
	  {
            if( false )
	      ::display(boundaryConditions,"boundaryConditions BEFORE implicitSolver[imp].setCoefficientsAndBoundaryConditions");
	    
	    implicitSolver[imp].setCoefficientsAndBoundaryConditions( coeff, boundaryConditions, boundaryConditionData );  
	  }
	  else
	  { // old way:
	    implicitSolver[imp].setCoefficientArray( coeff, boundaryConditions, boundaryConditionData );  
	    implicitSolver[imp].updateToMatchGrid( cg ); // kkc 060731 does this need to be called even if we have rebuilt the matrix? 
	  }
	  
	}

	}
      

      } // end for( imp
    }
    else
    {
      // ************* OLD ********************

      Overture::abort("ERROR: old way");


// *     // ****  do not use predefined equations ****
// * 
// *     Range & Rt = parameters.dbase.get<Range >("Rt");       // time dependent components
// *     Range & Rtimp = parameters.dbase.get<Range >("Rtimp"); // time dependent components that may be treated implicitly
// * 
// *     int numberOfTimeDependentComponents = Rt.getLength();
// *     int numberOfImplicitTimeDependentComponents = Rtimp.getLength();
// *     const int numberOfComponentsForCoefficients = scalarSystemForImplicitTimeStepping ? 1 : 
// *                                                   numberOfImplicitTimeDependentComponents;
// *     // printF(" @@@@@@@@@@ numberOfTimeDependentComponents=%i  @@@@@@@@@@@\n",numberOfTimeDependentComponents);
// *     
// *     // make a grid function to hold the coefficients
// *     Range all;
// *     // *wdh* 060929 int stencilWidth = 2*parameters.dbase.get<int >("orderOfAccuracy") + 1;
// *     int stencilWidth = parameters.dbase.get<int >("orderOfAccuracy") + 1;
// *     const int numberOfGhostLines=parameters.numberOfGhostPointsNeededForImplicitMatrix();
// *     if( numberOfGhostLines==2 && parameters.dbase.get<int >("orderOfAccuracy")==2 )
// *       stencilWidth=5;
// * 
// *     int stencilSize=int( pow(stencilWidth,cg.numberOfDimensions())+1 );   // add 1 for interpolation equations
// * 
// *     int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);
// *     
// * 
// *     coeff.updateToMatchGrid(cg,stencilDimension,all,all,all); 
// *     coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines,numberOfComponentsForCoefficients);
// *     
// *     // We need to change the stencil size from that used by the pressure solve.
// *     op.setStencilSize(stencilSize);
// *     op.setNumberOfComponentsForCoefficients(numberOfComponentsForCoefficients);
// *     coeff.setOperators(op);
// *     coeff = 0.0;
// *     // Form the implicit system on each grid.
// *     int grid;
// *     bool isSingular = isImplicitMatrixSingular( cgf0.u );
// *     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *     {
// *       MappedGrid &mg = cg[grid];
// * 
// *       realMappedGridFunction & gridVelocity = parameters.gridIsMoving(grid) ? cgf0.getGridVelocity(grid) : cgf0.u[grid];
// *       formImplicitTimeSteppingMatrix(coeff[grid],dt0,scalarSystemForImplicitTimeStepping,
// * 				     cgf0.u[grid],gridVelocity,grid,0 );
// * 
// *     }
// * 
// *     //	  BoundaryConditionParameters bcp;
// *     //	  bcp.setCornerBoundaryCondition(BoundaryConditionParameters::symmetryCorner);
// *     //	  coeff.finishBoundaryConditions(bcp); //kkc 060406
// *     coeff.finishBoundaryConditions(); //kkc 060406
// *     // coeff.display("Here is coeff after finishBoundaryConditions");
// * 
// *     // implicitSolver[0].setNumberOfComponents(numberOfTimeDependentComponents);
// * 
// *     if ( isSingular )  //kkc 060728
// *     { 
// *       addConstraintEquation(parameters,implicitSolver[0],coeff,cgf0.u,cgf1.u,numberOfComponentsForCoefficients);
// *     }
// *     else
// *     {
// *       implicitSolver[0].setCoefficientArray( coeff );   // supply coefficients
// *       implicitSolver[0].updateToMatchGrid( cg ); // kkc 060731 does this need to be called even if we have rebuilt the matrix?
// *     }
// * 

    }
    
  parameters.dbase.get<int >("initializeImplicitTimeStepping")=false;

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;
  
}


//\begin{>>CginsInclude.tex}{\subsection{implicitSolve}} 
void Cgins::
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
// /cgf1 (input/output) : (new) On input holds the right-hand-side for the implicit equations; on output
//    holds the solution. For moving grid problems this function is on the new grid. 
// /cgf0 (input) : (current) current best approximation to the solution. Used as initial guess for iterative
//   solvers and used for linearization. For moving grid problems this function is on the old grid.
//\end{CginsInclude.tex}  
// ==========================================================================================
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return;

  real cpu0=getCPU();

  checkArrayIDs("Cgins::implicitSolve: START"); 

  int grid, n;

  const int & uc = parameters.dbase.get<int >("uc");
  const int & tc = parameters.dbase.get<int >("tc");
  const int numberOfDimensions = cgf1.cg.numberOfDimensions();

  FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
  
  int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");
  // numberOfImplicitVelocitySolvers : counts the number of separate Oges solvers needs to solve for (u,v,w)
  int & numberOfImplicitVelocitySolvers = parameters.dbase.get<int>("numberOfImplicitVelocitySolvers");
  int & implicitSolverForTemperature = parameters.dbase.get<int>("implicitSolverForTemperature");
  bool & useFullSystemForImplicitTimeStepping = parameters.dbase.get<bool >("useFullSystemForImplicitTimeStepping");

  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");
  

  assert( implicitSolver!=NULL );
  
  // *wdh* 071011 -- The user should now explicitly call formMatrixForImplicitSolve ----

//   bool recomputeMatrix = (parameters.dbase.get<int >("globalStepNumber") % parameters.dbase.get<int >("refactorFrequency")) ==0;
//   if( recomputeMatrix )
//   {
//     parameters.dbase.get<int >("initializeImplicitTimeStepping")=1;
//     if( debug() & 2 )
//       printF(">>>Cgins::implicitSolve: recompute matrix: parameters.dbase.get<int >("globalStepNumber")=%i refactorFrequency=%i recomputeMatrix=%i\n",
// 	     parameters.dbase.get<int >("globalStepNumber"),parameters.dbase.get<int >("refactorFrequency"),recomputeMatrix);
//   }
  
//   if( parameters.dbase.get<int >("initializeImplicitTimeStepping") )
//   {
//     // *******************************************
//     // **** Initialize Implicit Time Stepping ****
//     // *******************************************

//     // kkc 060302 code moved to formMatrixForImplictSolve
//     formMatrixForImplicitSolve(dt0, cgf1,cgf0);
//     cpu0 = getCPU(); // need to reset this to avoid double counting 
//   } // end initialize implicit time stepping

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
  for( int imp=0; imp<numberOfImplicitSolvers; imp++ )
  {
    if( implicitSolver[imp].isSolverIterative() )
    {
      isIterativeSolver=true;
      break;
      
    }
  }
  
  Range & Rt = parameters.dbase.get<Range >("Rt"); // time dependent components
  Range & Rtimp = parameters.dbase.get<Range >("Rtimp");  // time dependent components that may be treated implicitly

  // Range Ru(uc,uc+numberOfDimensions-1);
    
  Range Rv = Rtimp;
  if( !useFullSystemForImplicitTimeStepping && pdeModel==InsParameters::BoussinesqModel )
  { // do not include T in the range Rv if we solve with separate scalar systems
    assert( Rv.getBound()==tc );
    Rv = Range(Rv.getBase(),Rv.getBound()-1);
  }
  

  if( scalarSystemForImplicitTimeStepping )
  {
    // solve one velocity component at a time

    v.updateToMatchGrid(cgf1.cg);

    if( isIterativeSolver )
      w.updateToMatchGrid(cgf1.cg);
    
    
    for( n=Rv.getBase(); n<=Rv.getBound(); n++ )  // velocity component 
    {
      const int imp =  numberOfImplicitVelocitySolvers==1 ? 0 : n-Rv.getBase();

      assert( imp>=0 && imp<numberOfImplicitVelocitySolvers );

      for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
      {
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

      
      if( debug() & 4 )
	printP("solve implicit time step for component n=%i (%s), imp=%i..\n",n,(const char*)cgf0.u.getName(n),imp);

      if( implicitSolver[imp].isSolverIterative() )
      {
	// for iterative solvers we need a separate RHS! since the rhs will have points zeroed out.

	if( debug() & 4 ) 
	{
	  cgf1.u.display(sPrintF("cgf1.u before implicit solve (n=%i)",n),debugFile,"%6.3f ");
	  w.display(sPrintF("RHS w=cgf1.u(n) before implicit solve (n=%i)",n),debugFile,"%6.3f ");
	}
	
	implicitSolver[imp].solve( v,w );  

	numberOfIterations+=implicitSolver[imp].getNumberOfIterations();

	if( debug() & 4 ) 
	{
	  v.display(sPrintF("Solution after implicit solve (n=%i)",n),debugFile,"%6.3f ");
	}
	

	if( debug() & 2 )
	  printP(" ** implicit time stepping: component %i iterations= %i (t=%e, dt=%8.1e, step=%i, "
		 "max residual=%8.2e)\n",n,implicitSolver[imp].getNumberOfIterations(),
		 cgf1.t,dt,parameters.dbase.get<int >("globalStepNumber"),implicitSolver[imp].getMaximumResidual());

	if( FALSE && implicitSolver[imp].getNumberOfIterations() > 10 )
	{
	  fprintf(debugFile,"***WARNING*** % iterations required to implicit solve of component n=%i\n",
		  implicitSolver[imp].getNumberOfIterations(),n);
	  v.display("\n ****v (left hand side for implicit solve)",debugFile);
	  w.display("\n ****w (right hand side for implicit solve)",debugFile);
	  char buff[80];
	  for( grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
	    display(coeff[grid].sparse->classify,sPrintF(buff,"\n ****classify on grid=%i",grid),debugFile);

	}
	
      }
      else
      {
	if( debug() & 4 ) 
	{
	  v.display(sPrintF("RHS before implicit solve, component=%i",n),debugFile,"%6.3f ");
	  // fprintf(debugFile," ***Errors before solve for component=%i\n",n);
	  // determineErrors( cgf1 );
	}
	
	implicitSolver[imp].solve( v,v );  

	if( debug() & 4 )
	{
	  v.display(sPrintF("Solution after implicit solve, component=%i",n),debugFile,"%6.3f ");
	  // fprintf(debugFile," ***Errors after solve for component=%i\n",n);
	  // determineErrors( cgf1 );
	}
      }
      for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
      {
	assign(cgf1.u[grid],all,all,all,n, v[grid],all,all,all,0);
      }

    }
    numberOfIterations/=max(1,Rv.getLength());
  }
  else
  {
    // Solve for all implicit components at once.
    v.updateToMatchGrid(cgf1.cg,all,all,all,Rv);

    if( implicitSolver[0].isSolverIterative() )
    { // in this case we need an initial guess and a rhs
      w.updateToMatchGrid(cgf1.cg,all,all,all,Rv);
      for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
      {
	assign(w[grid],all,all,all,Rv, cgf1.u[grid],all,all,all,Rv); // rhs
	assign(v[grid],all,all,all,Rv, cgf0.u[grid],all,all,all,Rv); // initial guess
      }
    }
    else
    {
      for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
      {
	assign(v[grid],all,all,all,Rv, cgf1.u[grid],all,all,all,Rv); // rhs
      }
    }
      

    if( implicitSolver[0].isSolverIterative() )
    {
      // for iterative solvers we need a separate RHS!
      if( false ) v=0.;
      
      implicitSolver[0].solve( v,w );  

      if( false )
      {
	printF("Cgins::implicitSolve: its = %i ** \n",implicitSolver[0].getNumberOfIterations());
	implicitSolver[0].solve( v,w );  
	printF("Cgins::implicitSolve: its = %i (AGAIN) ** \n",implicitSolver[0].getNumberOfIterations());
      }
      
      numberOfIterations+=implicitSolver[0].getNumberOfIterations();
    }
    else
    {
      implicitSolver[0].solve( v,v );
    }
    
    for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
    {
      // note: "explicit variables" have already been filled in to cgf1.u I think 
      assign(cgf1.u[grid],all,all,all,Rv, v[grid],all,all,all,Rv);
    }

    if( implicitSolver[0].isSolverIterative() && ( debug() & 1 || debug() & 2 ) )
    {
      if( false && debug() & 1 ) 
	printF("Cgins::implicitSolve: number of iterations to solve implicit time step matrix = %i ** \n",
	       numberOfIterations);

      if( true || debug() & 2 )
      {
	real absoluteTolerance,relativeTolerance;
	implicitSolver[0].get(OgesParameters::THEabsoluteTolerance,absoluteTolerance);
	implicitSolver[0].get(OgesParameters::THErelativeTolerance,relativeTolerance);

	// compute the max-residual if needed
	real maxResidual = implicitSolver[0].getMaximumResidual();
    
	printF(" ** Cgins::implicitSolve its = %i (t=%9.3e, dt=%8.1e, step=%i, max res=%8.2e "
	       "rel-tol=%7.1e, abs-tol=%7.1e)\n",
	       implicitSolver[0].getNumberOfIterations(),cgf1.t,dt,parameters.dbase.get<int >("globalStepNumber"),maxResidual,
	       relativeTolerance,absoluteTolerance);
      }
    }
  }
  
  if( !useFullSystemForImplicitTimeStepping && pdeModel==InsParameters::BoussinesqModel )
  {
    // Solve the Temperature equation

    if( debug() & 4 )
      printF(" ***Cgins::implicitSolve:  Solve the Temperature equation\n");

    int imp = implicitSolverForTemperature;

    v.updateToMatchGrid(cgf1.cg,all,all,all);
    
    const int n = tc;
    
    for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
    {
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

      
    if( debug() & 4 )
      printF("Cgins::implicitSolve: solve implicit time step for the Temperature...\n");


    if( implicitSolver[imp].isSolverIterative() )
    {
      if( debug() & 4 ) 
      {
	v.display(sPrintF("solution before implicit solve for T"),debugFile,"%6.3f ");
	w.display(sPrintF("RHS before implicit solve for T"),debugFile,"%6.3f ");
      }
      
      implicitSolver[imp].solve( v,w );  

      if( debug() & 2 ) printF(" ..number of iterations to solve the implicit T eqn = %i\n",
			       implicitSolver[imp].getNumberOfIterations());
    }
    else
    {
      if( debug() & 4 ) 
	v.display(sPrintF("RHS before implicit solve for T"),debugFile,"%6.3f ");

      implicitSolver[imp].solve( v,v );  
    }

    if( debug() & 4 )
      v.display(sPrintF("Solution after implicit solve for T"),debugFile,"%6.3f ");

    for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
    {
      assign(cgf1.u[grid],all,all,all,n, v[grid],all,all,all,0);
    }
  }
  
  parameters.dbase.get<int >("numberOfIterationsForImplicitTimeStepping")+=numberOfIterations;
  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;

  checkArrayIDs("Cgins::implicitSolve: END"); 

}


// ===================================================================================================================
/// \brief Compute the residual for "steady state" solvers
/// \param t (input): current time 
/// \param dt (input): current global time step -- is this used ?
/// \param cgf (input): holds solution to compute the residual from
/// \param residual (output): residual
///
// ===================================================================================================================
int Cgins::
getResidual( real t, real dt, GridFunction & cgf, realCompositeGridFunction & residual)
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return 0;

  CompositeGrid & cg = cgf.cg;

  // We should have a RHS that holds the forcing, in addition to the residual

  assign(residual,0.);
  
  // When computing the residual we should not adjust the forcing for implicit time stepping
  const int adjustForcingForImplicitSave = parameters.dbase.get<int>("adjustForcingForImplicit");
  parameters.dbase.get<int>("adjustForcingForImplicit")=false;
  int iparam[10];
  real rparam[10];
  rparam[0]=cgf.t; // gf[mk].t;
  rparam[1]=cgf.t; // This should be : tForce 
  rparam[2]=cgf.t; // tImplicit
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    realMappedGridFunction & u0 = cgf.u[grid];
    realMappedGridFunction & resid = residual[grid];

    realMappedGridFunction f;               // fix this -- use a temporary from somewhere
    f.updateToMatchGridFunction(resid);
    assign(f,0.);

    iparam[0]=grid;
    iparam[1]=cg.refinementLevelNumber(grid);

    real time0=getCPU();
    // assign the forcing: 
    addForcing(f,u0,iparam,rparam); // this does not use the mask
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForForcing"))+=getCPU()-time0;

    realMappedGridFunction & gridVelocity = parameters.gridIsMoving(grid) ? cgf.getGridVelocity(grid) : u0;

    realMappedGridFunction & coeffg = coeff.numberOfComponentGrids()==cg.numberOfComponentGrids() ? coeff[grid] : u0;

    // resid = f - L(u0)
    insImplicitMatrix(InsParameters::evalResidual,coeffg,dt,u0,resid,f,gridVelocity,grid);


    // -- assign the BC's to the forcing 
    // assert( parameters.getGridIsImplicit(grid) ) ; // do this for now 
    
    applyBoundaryConditionsForImplicitTimeStepping(f,
						   cgf.u[grid],  // -- fix this -- should be uL
						   cgf.getGridVelocity(grid),
						   cgf.t,
						   parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping"),
						   grid );

    insImplicitMatrix(InsParameters::evalResidualForBoundaryConditions,coeffg,dt,u0,resid,f,gridVelocity,grid);

  }
  parameters.dbase.get<int>("adjustForcingForImplicit")=adjustForcingForImplicitSave;  // reset


  // Assign the "forcing" from the BC's 
  // applyBoundaryConditionsForImplicitTimeStepping( cgf );



  return 0;
}


// ===================================================================================================================
/// \brief build the implicit time stepping matrix or eval the RHS.
/// \param option: buildMatrix, evalRightHandSide, evalResidual, evalResidualForBoundaryConditions
///
// ===================================================================================================================
int Cgins::
insImplicitMatrix(InsParameters::InsImplicitMatrixOptionsEnum option,
		  realMappedGridFunction & coeff,
		  const real & dt0, 
		  const realMappedGridFunction & u0,
		  realMappedGridFunction & fe,
		  realMappedGridFunction & fi,
		  const realMappedGridFunction & gridVelocity,
		  const int & grid)
{

  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");

  MappedGrid & mg = *coeff.getMappedGrid();
  const int numberOfDimensions = mg.numberOfDimensions();
  
  int & fillCoefficientsScalarSystem = parameters.dbase.get<int>("fillCoefficientsScalarSystem");
  if( false || (debug() & 4 && fillCoefficientsScalarSystem!=0 && option==InsParameters::buildMatrix) )
  {
    printF("@@@@@ insImplicitMatrix fillCoefficientsScalarSystem=%i @@@@@@\n",fillCoefficientsScalarSystem);
  }
  

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
//   Range e0(0,0), e1(1,1), e2(2,2);  // e0 = first equation, e1=second equation
//   Range c0(0,0), c1(1,1), c2(2,2);  // c0 = first component, c1 = second component
//   Range Rx(0,numberOfDimensions-1);
  int n;

  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

  Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
  const bool computeTemperature = (pdeModel==InsParameters::BoussinesqModel ||
				   pdeModel==InsParameters::viscoPlasticModel);

  int numberOfComponentsForCoefficients = 0;
  int numberOfGhostLines = 0;
  int stencilSize = 0;
  int stencilDim = 0;
  SparseRepForMGF *pSparse = coeff.sparse;
  if( option==InsParameters::buildMatrix )
  {
    assert( coeff.sparse!=NULL );
    SparseRepForMGF & sparse = *coeff.sparse;
    numberOfComponentsForCoefficients = sparse.numberOfComponents;  // size of the system of equations
    numberOfGhostLines = sparse.numberOfGhostLines;
    stencilSize = sparse.stencilSize;
    stencilDim=stencilSize*numberOfComponentsForCoefficients; // number of coefficients per equation
  }
  
  const int width = parameters.dbase.get<int >("orderOfAccuracy")+1; 
  const int halfWidth1 = (width-1)/2;
  const int halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
  const int halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;

  if( debug() & 4 ) 
  {
    printF("\n===== insImplicitMatrix: **new** call insimp =====\n");
    if( option==InsParameters::buildMatrix )
      printF("===== numberOfComponentsForCoefficients = %i, dt=%e dt0=%e =====\n",
	     numberOfComponentsForCoefficients,dt,dt0);
  }
  
#ifdef USE_PPP
  realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff,coeffLocal);
  realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0,u0Local);
  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
#else
  realSerialArray & coeffLocal = coeff;
  const realSerialArray & u0Local = u0;
  intSerialArray & maskLocal = mg.mask();
#endif

  const bool vertexNeeded = parameters.isAxisymmetric();  // we need the vertex array in this case
  if( vertexNeeded )
  {
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
  }
  const bool isRectangular = mg.isRectangular();
  real dx[3]={1.,1.,1.};
  if( isRectangular )
    mg.getDeltaX(dx);
  else
    mg.update(MappedGrid::THEinverseVertexDerivative);


  // fill in coeff at these points:
  getIndex(mg.gridIndexRange(),I1,I2,I3);
  // bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,I1,I2,I3);
  int n1a,n1b,n2a,n2b,n3a,n3b;
  bool ok = ParallelUtility::getLocalArrayBounds(u0,u0Local,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b);
  if( ok )
  {
    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
    // NOTE: bcLocal(side,axis) == -1 for internal boundaries between processors
    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u0,gidLocal,dimLocal,bcLocal ); 


    realCompositeGridFunction *& pDistanceToBoundary =
      parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary");

    // we don't need uL for evaluating the residual: 
    bool linearizedSolutionIsNeeded = (parameters.saveLinearizedSolution() && 
				       option!=InsParameters::evalResidual &&
				       option!=InsParameters::evalResidualForBoundaryConditions);

#ifdef USE_PPP
    RealArray xLocal; if( vertexNeeded ) getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
    realSerialArray rsxyLocal; 
    if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rsxyLocal);
    realSerialArray dwLocal;
    if( pDistanceToBoundary!=NULL ) getLocalArrayWithGhostBoundaries(((*pDistanceToBoundary)[grid]),dwLocal);
    intSerialArray equationNumberLocal; 
    if( option==InsParameters::buildMatrix ) 
      getLocalArrayWithGhostBoundaries(pSparse->equationNumber,equationNumberLocal);
    intSerialArray classifyLocal; 
    if( option==InsParameters::buildMatrix ) 
      getLocalArrayWithGhostBoundaries(pSparse->classify,classifyLocal);

    realSerialArray feLocal; getLocalArrayWithGhostBoundaries(fe,feLocal);  
    realSerialArray fiLocal; getLocalArrayWithGhostBoundaries(fi,fiLocal);  

    realSerialArray uLinearized;
    if( linearizedSolutionIsNeeded )
    {
      assert( puLinearized!=NULL );
      getLocalArrayWithGhostBoundaries((*puLinearized)[grid],uLinearized);
    }
    else
    {
      uLinearized.reference(u0Local);
    }
    
    realSerialArray gridVelocityLocal, gridVelocityLinearizedLocal; 
    if( parameters.gridIsMoving(grid) ) 
    {
      getLocalArrayWithGhostBoundaries(gridVelocity,gridVelocityLocal);
      if( linearizedSolutionIsNeeded )
      {
	assert( pGridVelocityLinearized!=NULL );
	getLocalArrayWithGhostBoundaries(pGridVelocityLinearized[grid],gridVelocityLinearizedLocal);
      }
    }
    
#else
    const RealArray & xLocal = vertexNeeded ? mg.vertex() : u0;
    const realSerialArray & rsxyLocal = isRectangular? u0Local : mg.inverseVertexDerivative();
    const realSerialArray & dwLocal =  pDistanceToBoundary==NULL ? u0Local : ((*pDistanceToBoundary)[grid]);
    intSerialArray & equationNumberLocal = option==InsParameters::buildMatrix ? pSparse->equationNumber : maskLocal;
    intSerialArray & classifyLocal = option==InsParameters::buildMatrix ? pSparse->classify : maskLocal;

    realSerialArray & feLocal = fe;
    realSerialArray & fiLocal = fi;

    realSerialArray uLinearized;
    if( linearizedSolutionIsNeeded )
    {
      assert( puLinearized!=NULL );
      uLinearized.reference((*puLinearized)[grid]);
    }//      const realSerialArray & gridVelocityLocal =(*gridVelocity[grid]); 
    else
    {
      uLinearized.reference(u0Local);
    }
    realSerialArray gridVelocityLocal, gridVelocityLinearizedLocal; 
    if( parameters.gridIsMoving(grid) ) 
    {
      gridVelocityLocal.reference(gridVelocity);
      if( linearizedSolutionIsNeeded )
      {
	assert( pGridVelocityLinearized!=NULL );
	gridVelocityLinearizedLocal.reference(pGridVelocityLinearized[grid]);
      }
    }
#endif

    // For axisymmetric problems define:
    //    radiusInverse(i1,i2,i3) = 1/y  : off the axis of symmetry
    //                            = 0    : on the axis of symmetry
    RealArray radiusInverse;
    if( parameters.isAxisymmetric() )
    {
      Range D1=u0Local.dimension(0), D2=u0Local.dimension(1), D3=u0Local.dimension(2);
      radiusInverse.redim(D1,D2,D3);
      radiusInverse=0.;
      radiusInverse(I1,I2,I3) = 1./max(REAL_MIN,xLocal(I1,I2,I3,axis2));
      Index Ib1,Ib2,Ib3;
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
	  {
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    int includeGhost=1;
	    bool ok = ParallelUtility::getLocalArrayBounds(u0,u0Local,Ib1,Ib2,Ib3,includeGhost);
	    radiusInverse(Ib1,Ib2,Ib3)=0.;
	  }
	}
      }
      if( debug() & 8 )
      {
	display(radiusInverse,sPrintF("insImplicitMatrix: radiusInverse, grid=%i",grid),pDebugFile,"%8.5f ");
      }
    }

    const real *pxy=xLocal.getDataPointer(); 
    real *pfe = feLocal.getDataPointer(); // not used unless we eval the RHS
    real *pfi = fiLocal.getDataPointer(); // not used unless we eval the RHS
    const real *prsxy = rsxyLocal.getDataPointer();
    const real *pdw=dwLocal.getDataPointer();
    real *pgv=gridVelocityLocal.getDataPointer();
    real *pgvLinearized=gridVelocityLinearizedLocal.getDataPointer(); 

    const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");

    bool & useSecondOrderArtificialDiffusion = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion");

    // --- Variable material properies ---
    GridMaterialProperties::MaterialFormatEnum materialFormat = GridMaterialProperties::constantMaterialProperties;
    int ndMatProp=1;  // for piecewise constant materials, this is the leading dimension of the matVal array
    int *matIndexPtr=maskLocal.getDataPointer();   // if not used, point to mask
    real*matValPtr=u0Local.getDataPointer();       // if not used, point to u
    if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
    {
      // Material properties do vary 

      if( debug() & 4 )
	printF("Cgins::insImplicitMatrix: Material properties do vary\n");

      std::vector<GridMaterialProperties> & materialProperties = 
	parameters.dbase.get<std::vector<GridMaterialProperties> >("materialProperties");

      GridMaterialProperties & matProp = materialProperties[grid];
      materialFormat = matProp.getMaterialFormat();
      
      if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
      {
	IntegerArray & matIndex = matProp.getMaterialIndexArray();
        matIndexPtr = matIndex.getDataPointer();
      }
      
      RealArray & matVal = matProp.getMaterialValuesArray();
      matValPtr = matVal.getDataPointer();
      ndMatProp = matVal.getLength(0);  

      // ::display(matVal,"Cgins::getUt: matVal");
    }


    const int ndipar=60, ndrpar=40;
    int ipar[ndipar];
    real rpar[ndrpar];
    ipar[0] = I1.getBase();
    ipar[1] = I1.getBound();
    ipar[2] = I2.getBase();
    ipar[3] = I2.getBound();
    ipar[4] = I3.getBase();
    ipar[5] = I3.getBound();
      
    ipar[6] = parameters.dbase.get<int >("pc");
    ipar[7] = uc;
    ipar[8] = vc;
    ipar[9] = wc;
    ipar[10]= parameters.dbase.get<int >("kc");
    ipar[11]= parameters.dbase.get<int >("sc");
    ipar[12]= parameters.dbase.get<int >("tc");

    ipar[13]= grid;
    ipar[14]= parameters.dbase.get<int >("orderOfAccuracy");
    ipar[15]= parameters.gridIsMoving(grid);

    int fillCoefficients = option==InsParameters::buildMatrix;
    int evalRightHandSide= option==InsParameters::evalRightHandSide;
      
    ipar[16]= parameters.dbase.get<InsParameters::ImplicitVariation>("implicitVariation");
      
    ipar[17]= fillCoefficients;
    ipar[18]= evalRightHandSide;

    ipar[19]= parameters.getGridIsImplicit(grid);
    ipar[20]= parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
    ipar[21]= parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
    ipar[22]= parameters.isAxisymmetric();
    ipar[23]= useSecondOrderArtificialDiffusion;
    ipar[24]= parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");

    ipar[25]= parameters.dbase.get<bool >("advectPassiveScalar");
      
    ipar[26]= isRectangular ? 0 : 1; // gridType;
    ipar[27]= turbulenceModel;
    ipar[28]=(int)pdeModel;
    ipar[29]=numberOfComponentsForCoefficients;
    ipar[30]=stencilSize;

    if( option==InsParameters::buildMatrix ) 
    {
      assert( coeff.sparse!=NULL );
      SparseRepForMGF & sparse = *coeff.sparse;
      ipar[31]=sparse.equationOffset;
      ipar[32]=sparse.equationNumber.getBase(1);
      ipar[33]=sparse.equationNumber.getLength(1);
      ipar[34]=sparse.equationNumber.getBase(2);
      ipar[35]=sparse.equationNumber.getLength(2);
      ipar[36]=sparse.equationNumber.getBase(3);
      ipar[37]=sparse.equationNumber.getLength(3);
    }
    else
    {
      ipar[31]=0;
      ipar[32]=0;
      ipar[33]=0;
      ipar[34]=0;
      ipar[35]=0;
      ipar[36]=0;
      ipar[37]=0;
    }
    
    
    ipar[38]=mg.gridIndexRange(0,0);
    ipar[39]=mg.gridIndexRange(1,0);
    ipar[40]=mg.gridIndexRange(0,1);
    ipar[41]=mg.gridIndexRange(1,1);
    ipar[42]=mg.gridIndexRange(0,2);
    ipar[43]=mg.gridIndexRange(1,2);

    int orderOfExtrapolation=parameters.dbase.get<int >("orderOfAccuracy")+1;
    ipar[44]=orderOfExtrapolation;
    int orderOfExtrapolationForOutflow = parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
    if( orderOfExtrapolationForOutflow==-1 ) orderOfExtrapolationForOutflow=orderOfExtrapolation;
     
    ipar[45]=orderOfExtrapolationForOutflow;

    int evalResidual=option==InsParameters::evalResidual;
    ipar[46]=evalResidual;
    ipar[47]=int(option==InsParameters::evalResidualForBoundaryConditions);
    ipar[48]=parameters.dbase.get<int >("debug");
    ipar[49]=parameters.dbase.get<int >("numberOfComponents");
    ipar[50]=parameters.dbase.get<int >("rc");
    ipar[51]=materialFormat;
    
    rpar[0] = mg.gridSpacing(0);
    rpar[1] = mg.gridSpacing(1);
    rpar[2] = mg.gridSpacing(2);
    rpar[3] = dx[0];
    rpar[4] = dx[1];
    rpar[5] = dx[2];
    rpar[6] = dt;

    // *wdh* 090716 -- bug found : insimp does not fill in just the identity for grids that
    // are explicit !  
    real implicitFactor = parameters.dbase.get<real >("implicitFactor");
    if( !parameters.getGridIsImplicit(grid) )
    {
      implicitFactor=0.;   // *wdh* 090716   *** do this for now =========== fix me =============
    }
      
    rpar[7] = implicitFactor;

    rpar[8] = parameters.dbase.get<real >("nu");

    // the SA model always has AD in the equations so turn it off here by setting the coeff's to zero:
    rpar[9] = useSecondOrderArtificialDiffusion ? parameters.dbase.get<real >("ad21") : 0.;
    rpar[10]= useSecondOrderArtificialDiffusion ? parameters.dbase.get<real >("ad22") : 0.;
    rpar[11]= parameters.dbase.get<bool>("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad41") : 0.;
    rpar[12]= parameters.dbase.get<bool>("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad42") : 0.;
    rpar[13]=parameters.dbase.get<real >("nuPassiveScalar");
    const real adcPassiveScalar=1.; // coeff or linear artificial diffusion for the passive scalar ** add to params
    rpar[14]=adcPassiveScalar;
           			
    rpar[15]= parameters.dbase.get<real >("ad21n");
    rpar[16]= parameters.dbase.get<real >("ad22n");
    rpar[17]= parameters.dbase.get<real >("ad41n");
    rpar[18]= parameters.dbase.get<real >("ad42n");
    const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
    rpar[19]= yEps;  // for axisymmetric

    ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
    rpar[20]=gravity[0];
    rpar[21]=gravity[1];
    rpar[22]=gravity[2];

    real thermalExpansivity=1.;   
    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
    rpar[23]=thermalExpansivity;
    real adcBoussinesq=0.; // coefficient of artificial diffusion for Boussinesq T equation 
    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("adcBoussinesq",adcBoussinesq);
    rpar[24]=adcBoussinesq;
    rpar[25]= parameters.dbase.get<real >("kThermal");

    if( option==InsParameters::buildMatrix )
      coeffLocal=0.;  // is this needed ?

    DataBase *pdb = &parameters.dbase;

    int ierr=0;
    insimp(numberOfDimensions,
	   u0Local.getBase(0),u0Local.getBound(0),
	   u0Local.getBase(1),u0Local.getBound(1),
	   u0Local.getBase(2),u0Local.getBound(2),
	   u0Local.getBase(3),u0Local.getBound(3),
	   *maskLocal.getDataPointer(),
	   *pxy, *prsxy, *radiusInverse.getDataPointer(), *u0Local.getDataPointer(),
	   coeffLocal.getLength(0),*coeffLocal.getDataPointer(),
	   *pfe, *pfi, *uLinearized.getDataPointer(), *pgv, *pgvLinearized, *pdw, 
           ndMatProp,*matIndexPtr,*matValPtr,*matValPtr,
           bcLocal(0,0), mg.boundaryCondition(0,0),
	   bcData.getBase(0),bcData.getBound(0),
	   bcData.getBase(1),bcData.getBound(1),
	   bcData.getBase(2),bcData.getBound(2),
	   bcData.getBase(3),bcData.getBound(3),*bcData.getDataPointer(),
	   equationNumberLocal.getLength(0),*equationNumberLocal.getDataPointer(),
	   *classifyLocal.getDataPointer(),
	   n1a,n1b,n2a,n2b,n3a,n3b, 
	   ipar[0], rpar[0], pdb, ierr );
      


    if( option==InsParameters::buildMatrix )
    {
      coeff.updateGhostBoundaries();  // *wdh* 100413 : This is needed for Ogmg
    }
    

    if( option==InsParameters::buildMatrix && debug() & 8 )
    {
      //::displayCoeff(coeff,sPrintF("coeff after insimp for grid=%i",grid),pDebugFile,"%3.1f ");
      // ::display(coeffLocal,sPrintF("coeffLocal after insimp for grid=%i",grid),pDebugFile,"%3.1f ");
      //::display(classifyLocal,sPrintF("classify after insimp for grid=%i",grid),pDebugFile,"%5i");
    }
    if( option==InsParameters::buildMatrix && debug() & 16 )
    {
      ::display(equationNumberLocal,sPrintF("equationNumber after insimp for grid=%i",grid),pDebugFile,"%5i");
    }
      
  } // end if ok
  
  return 0;
}







//\begin{>>MappedGridSolverInclude.tex}{\subsection{formImplicitTimeSteppingMatrixINS}} 
int Cgins::
formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
			       const real & dt0, 
			       int scalarSystem,
			       realMappedGridFunction & u0,
			       const realMappedGridFunction & gridVelocity,
			       const int & grid,
			       const int & imp )
// ==========================================================================================
// /Description:
//    Form the implicit time steping matrix for the INS equations on a single grid.
//
// If the form of the boundary conditions for the different components of $\uv$ are the same
// then we can build a single scalar matrix that can be used to advance each component, one after
// the other. If the boundary conditions are not of the same form then we build a matrix for
// a system of equations for the velocity components $(u,v,w)$.
//
// /coeff (input/output) : fill-in this coefficient matrix.
// /dt0 (input) : time step used to build the implicit matrix.
// /scalarSystem (input) : If true then the same matrix is used to solve for all components (e.g. all velocity
//   components could have the same form of the matrix).
// /u0 (input) : current best approximation to the solution. Used for linearization.
// 
//\end{MappedGridSolverInclude.tex}  
// ==========================================================================================
{
  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

// *    const int & uc = parameters.dbase.get<int >("uc");
// *    const int & vc = parameters.dbase.get<int >("vc");
// *    const int & wc = parameters.dbase.get<int >("wc");

  MappedGrid & mg = *coeff.getMappedGrid();
  const int numberOfDimensions = mg.numberOfDimensions();
  // const int numberOfComponents = scalarSystem ? 1 : numberOfDimensions;
  
// *    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
// *    Range e0(0,0), e1(1,1), e2(2,2);  // e0 = first equation, e1=second equation
// *    Range c0(0,0), c1(1,1), c2(2,2);  // c0 = first component, c1 = second component
// *    Range Rx(0,numberOfDimensions-1);
// *    int n;

// *    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

// *    Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
// *    const bool computeTemperature = (pdeModel==InsParameters::BoussinesqModel ||
// *                                     pdeModel==InsParameters::viscoPlasticModel);

// *    assert( coeff.sparse!=NULL );
// *    SparseRepForMGF & sparse = *coeff.sparse;
// *    const int numberOfComponentsForCoefficients = sparse.numberOfComponents;  // size of the system of equations
// *    const int numberOfGhostLines = sparse.numberOfGhostLines;
// *    const int stencilSize = sparse.stencilSize;
// *    const int stencilDim=stencilSize*numberOfComponentsForCoefficients; // number of coefficients per equation

// *    const int width = parameters.dbase.get<int >("orderOfAccuracy")+1; 
// *    const int halfWidth1 = (width-1)/2;
// *    const int halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
// *    const int halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;

  bool useOpt= !scalarSystem;
  if( scalarSystem )
  {
    useOpt=true;  // new way 
    
    // We are solving a scalar implicit system for a component of (u,v,w) or for T 
    int & fillCoefficientsScalarSystem = parameters.dbase.get<int>("fillCoefficientsScalarSystem");
    
    const int fillCoeffU=1,fillCoeffV=2,fillCoeffW=3,fillCoeffT=4;
    const int & implicitSolverForTemperature = parameters.dbase.get<int>("implicitSolverForTemperature");
    if( imp<numberOfDimensions )
      fillCoefficientsScalarSystem = fillCoeffU+imp;
    if( pdeModel==InsParameters::BoussinesqModel && imp==implicitSolverForTemperature )
      fillCoefficientsScalarSystem = fillCoeffT;
  }
  

  if( true || useOpt )  // *wdh* 080817 -- all methods use insImplicitMatrix now
  {
    // build the implicit matrix 
    insImplicitMatrix(InsParameters::buildMatrix,coeff,dt0,u0, u0,u0,gridVelocity,grid);

  }
  else
  {
    // old way 
    Overture::abort("error: old way");

// *     // Form the (scalar) matrix I - implicitFactor*nu*dt* Laplacian
// *     
// *     real nuDt = parameters.dbase.get<real >("implicitFactor")*parameters.dbase.get<real >("nu")*dt0;
// *     const real ad21 = parameters.dbase.get<real >("ad21");
// *     const real ad22 = parameters.dbase.get<real >("ad22");
// *     const RealArray & gridSpacing = mg.gridSpacing();
// *     MappedGridOperators & op = *u0.getOperators();
// *     
// *     getIndex(mg.gridIndexRange(),I1,I2,I3);
// * 
// *     realArray ad; 
// *     if( parameters.getGridIsImplicit(grid) )
// *     {
// *       if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
// *       {
// * 	// here we compute ad : the coefficient of the aritificial diffusion.
// * 	    
// * 	// op.getDerivatives(u0,I1,I2,I3,parameters.dbase.get<Range >("Ru")); // compute u.x for artificial diffussion
// * 	realArray ux(I1,I2,I3,parameters.dbase.get<Range >("Ru")), uy(I1,I2,I3,parameters.dbase.get<Range >("Ru"));
// * 	op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,parameters.dbase.get<Range >("Ru"));
// * 	op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,parameters.dbase.get<Range >("Ru"));
// * 
// * 	real cd22=ad22/SQR(numberOfDimensions);
// * 	const real implicitFactor=1.5*dt0;      // implicit diffusion is bigger than explicit for safety ******
// * 	
// * 	ad.redim(u0.dimension(0),u0.dimension(1),u0.dimension(2));
// * 	ad=0.;
// * 	if( numberOfDimensions==2 )
// * 	{
// * 	  ad(I1,I2,I3)=(-implicitFactor*ad21)+(-implicitFactor*cd22)*(abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)));
// * 	}
// * 	else if( numberOfDimensions==3 ) 
// * 	{
// * 	  realArray uz(I1,I2,I3,parameters.dbase.get<Range >("Ru"));
// * 	  op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,parameters.dbase.get<Range >("Ru"));
// * 	
// * 	  ad(I1,I2,I3)=(-implicitFactor*ad21)+
// * 	    (-implicitFactor*cd22)*(abs(UX(uc))+abs(UX(vc))+abs(UX(wc))+abs(UY(uc))+abs(UY(vc))+
// * 				    abs(UY(wc))+abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc)));
// * 	}
// * 	// printf(" ------->> max(fabs(ad))=%e \n",max(fabs(ad(I1,I2,I3)/dt0)));
// *       }
// *     }
// *     
// * #define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
// * #define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilSize*(n))
// * 
// * // Use this for indexing into coefficient matrices representing systems of equations
// * #define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
// * #define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))
// * 
// * // M123 with a fixed offset
// * #define MCE(m1,m2,m3) (M123(m1,m2,m3)+CE(c0,e0))
// *       
// * 
// * #ifndef USE_PPP
// *     bool useOpt=false;
// * #else
// *     bool useOpt=!scalarSystem;
// * #endif
// *     if( useOpt )
// *     {
// *       if( scalarSystem )
// *       {
// * 	Overture::abort("ERROR: finish this");
// *       }
// *       else
// *       {
// *       
// * 	coeff=0.;
// * 
// * 	if( parameters.getGridIsImplicit(grid) )
// * 	{
// * 	  if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
// * 	  {
// * 	    Overture::abort("ERROR: finish this");
// * 	  }
// *       
// * 	  for( int m=0; m<numberOfComponentsForCoefficients; m++ )
// * 	  {
// * 	    Range e(m,m), c(m,m);
// * 	    op.coefficients(MappedGridOperators::laplacianOperator,coeff,I1,I2,I3,e,c);
// * 	  }
// * 	  coeff*=-nuDt; // form  (-nu*dt)*Delta
// * 	  for( int m=0; m<numberOfComponentsForCoefficients; m++ )
// * 	  {
// * 	    int md=M123CE(0,0,0,m,m); // diagonal term 
// * 	    coeff(md,I1,I2,I3)+=1.; 
// * 	  }
// * 	}
// * 	else
// * 	{
// * 	  for( int m=0; m<numberOfComponentsForCoefficients; m++ )
// * 	  {
// * 	    Range e(m,m), c(m,m);
// * 	    op.coefficients(MappedGridOperators::identityOperator,coeff,I1,I2,I3,e,c);
// * 	  }
// *      
// * 	}
// *       
// *       }
// * 
// * 
// *     }
// *     else
// *     {
// *       // **old way**
// *       if( scalarSystem )
// *       {
// * 	if( parameters.getGridIsImplicit(grid) )
// * 	{
// * 	  if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
// * 	  {
// * 	    if( debug() & 4 ) printF("***Add artificial diffusion to the implicit matrix\n");
// * 	    // add artificial viscosity to matrix   ***** need an undivided difference laplacian operator *****
// * 	    if( numberOfDimensions==2 )
// * 	      coeff=SQR(gridSpacing(axis1))*op.r1r1Coefficients()+SQR(gridSpacing(axis2))*op.r2r2Coefficients();
// * 	    else
// * 	      coeff=(SQR(gridSpacing(axis1))*op.r1r1Coefficients()+
// * 		     SQR(gridSpacing(axis2))*op.r2r2Coefficients()+
// * 		     SQR(gridSpacing(axis3))*op.r3r3Coefficients());
// * 	  
// * 	    coeff=multiply(ad,coeff);
// * 	    coeff+=op.identityCoefficients()-nuDt*op.laplacianCoefficients(); 
// * 	  }
// * 	  else
// * 	    coeff=op.identityCoefficients()-nuDt*op.laplacianCoefficients(); 
// * 
// * 	}
// * 	else
// * 	  coeff=op.identityCoefficients();
// *       }
// *       else
// *       {
// * 	if( parameters.getGridIsImplicit(grid) )
// * 	{
// * 	  if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
// * 	  {
// * 	    if( parameters.dbase.get<int >("myid")==0 )
// * 	      printf("***Add artificial diffusion to the implicit matrix\n");
// * 	    // add artificial viscosity to matrix   ***** need an undivided difference laplacian operator *****
// * 	    if( numberOfDimensions==2 )
// * 	      coeff=(SQR(gridSpacing(axis1))*(op.r1r1Coefficients(I1,I2,I3,e0,c0)+op.r1r1Coefficients(I1,I2,I3,e1,c1))+
// * 		     SQR(gridSpacing(axis2))*(op.r2r2Coefficients(I1,I2,I3,e0,c0)+op.r2r2Coefficients(I1,I2,I3,e1,c1)));
// * 	    else
// * 	      coeff=(SQR(gridSpacing(axis1))*(op.r1r1Coefficients(I1,I2,I3,e0,c0)+op.r1r1Coefficients(I1,I2,I3,e1,c1))+
// * 		     SQR(gridSpacing(axis2))*(op.r2r2Coefficients(I1,I2,I3,e0,c0)+op.r2r2Coefficients(I1,I2,I3,e1,c1))+
// * 		     SQR(gridSpacing(axis3))*(op.r3r3Coefficients(I1,I2,I3,e0,c0)+op.r3r3Coefficients(I1,I2,I3,e1,c1)));
// * 	  
// * 	    coeff=multiply(ad,coeff);
// * 
// * 	    coeff+=(op.identityCoefficients(I1,I2,I3,e0,c0)-nuDt*op.laplacianCoefficients(I1,I2,I3,e0,c0)+
// * 		    op.identityCoefficients(I1,I2,I3,e1,c1)-nuDt*op.laplacianCoefficients(I1,I2,I3,e1,c1));
// * 	  }
// * 	  else
// * 	  {
// * 	    coeff=(op.identityCoefficients(I1,I2,I3,e0,c0)-nuDt*op.laplacianCoefficients(I1,I2,I3,e0,c0)+
// * 		   op.identityCoefficients(I1,I2,I3,e1,c1)-nuDt*op.laplacianCoefficients(I1,I2,I3,e1,c1));
// * 	  }
// * 	  if( numberOfDimensions==3 )
// * 	    coeff+=op.identityCoefficients(I1,I2,I3,e2,c2)-nuDt*op.laplacianCoefficients(I1,I2,I3,e2,c2);
// * 
// * 
// * 	  if( false )
// * 	  {
// * 	    Index Ib1,Ib2,Ib3;
// * 	    getBoundaryIndex(mg.gridIndexRange(),0,0,Ib1,Ib2,Ib3);
// * 	  
// * 	    printf("********* nuDt=%e\n",nuDt);
// * 	    int ib1=Ib1.getBase(), ib2=Ib2.getBase(), ib3=Ib3.getBase();
// * 	    int stencilDim=pow(3,numberOfDimensions)*numberOfDimensions;
// * 	    Index ME(0,stencilDim);
// * 	    ::display(coeff(ME     ,ib1,ib2,ib3),"After I-nu*Delta: coeff(ME,ib1,ib2,ib3)");
// * 	    ::display(coeff(ME+stencilDim,ib1,ib2,ib3),"After I-nu*Delta: coeff(ME+ce01,ib1,ib2,ib3)");
// * 	    ::display(coeff(ME+stencilDim*2,ib1,ib2,ib3),"After I-nu*Delta: coeff(ME+ce02,ib1,ib2,ib3)");
// * 	  }
// * 	
// * 
// * 	  if( numberOfDimensions==2 && parameters.isAxisymmetric() )
// * 	  {
// * 	    // add on corrections for a axisymmetric problem
// * 	    //  nu*( u.xx + u.yy + (1/y) u.y )
// * 	    //  nu*( v.xx + v.yy + (1/y) v.y - v/r^2 )
// * 	    realArray radiusInverse;
// * 	    radiusInverse=1./max(REAL_MIN,mg.vertex()(nullRange,nullRange,nullRange,axis2));
// * 
// * 	    int side,axis;
// * 	    ForBoundary(side,axis)
// * 	    {
// * 	      if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
// * 	      {
// * 		Index Ib1,Ib2,Ib3;
// * 		getBoundaryIndex( mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); 
// * 		coeff-=nuDt*(op.yyCoefficients(Ib1,Ib2,Ib3,e0,c0)+.5*op.yyCoefficients(Ib1,Ib2,Ib3,e1,c1));
// * 		radiusInverse(Ib1,Ib2,Ib3)=0.;  // this will remove terms on the axis below
// * 	      }
// * 	    }
// * 	    // add axisymmetric terms
// * 	    realMappedGridFunction aCoeff; // *** fix this ****
// * 	    aCoeff.updateToMatchGridFunction(coeff);
// * 	    aCoeff=op.yCoefficients(I1,I2,I3,e0,c0)+op.yCoefficients(I1,I2,I3,e1,c1)-
// * 	      multiply(radiusInverse,op.identityCoefficients(I1,I2,I3,e1,c1));
// * 	    coeff-=nuDt*multiply( radiusInverse,aCoeff );
// * 	  }
// * 	}
// * 	else
// * 	{
// * 	  coeff=op.identityCoefficients(I1,I2,I3,e0,c0)+op.identityCoefficients(I1,I2,I3,e1,c1);
// * 	  if( numberOfDimensions==3 )
// * 	    coeff+=op.identityCoefficients(I1,I2,I3,e2,c2);
// * 	}
// *       }
// *     }
    
  } // end old way (not useOpt) 
  

// *   if( !useOpt )
// *   {
// * 
// *     // fill in the coefficients for the boundary conditions
// *     Parameters::BoundaryCondition noSlipWall                = Parameters::noSlipWall;
// *     InsParameters::BoundaryConditions inflowWithVelocityGiven   = InsParameters::inflowWithVelocityGiven;
// *     Parameters::BoundaryCondition slipWall                  = Parameters::slipWall;
// *     Parameters::BoundaryCondition dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
// *     
// *     if( parameters.getGridIsImplicit(grid) )
// *     {
// *       if( scalarSystem )
// *       {
// * 	coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::dirichlet,noSlipWall);
// * 	coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::dirichlet,inflowWithVelocityGiven);
// * 	coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::dirichlet,dirichletBoundaryCondition);
// * 	coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::extrapolate,BCTypes::allBoundaries);
// *       }
// *       else
// *       {
// * 	for( n=0; n<numberOfDimensions; n++ )
// * 	{
// * 	  coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,noSlipWall);
// * 	  coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,inflowWithVelocityGiven);
// * 	  coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,dirichletBoundaryCondition);
// * 	  coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::extrapolate,BCTypes::allBoundaries);
// * 	}
// * 	if( numberOfDimensions==2 && parameters.isAxisymmetric() )
// * 	{
// * 	  // u.y = v = v.yy = 0
// * 	  coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::neumann,Parameters::axisymmetric);
// * 	  coeff.applyBoundaryConditionCoefficients(e1,c1,BCTypes::dirichlet,Parameters::axisymmetric);
// * 
// * 	  BoundaryConditionParameters extrapParams;
// * 	  extrapParams.lineToAssign=1;
// * 	  extrapParams.orderOfExtrapolation=2;
// * 	  coeff.applyBoundaryConditionCoefficients(e1,c1,BCTypes::extrapolate,Parameters::axisymmetric);
// * 	}
// *       
// * 
// * 	// ** new slip wall BC's ***
// * 	if( debug() & 4 ) printF("@@@ Cgins::formImplicitTimeSteppingMatrix: use new slip wall BC @@@\n");
// * 	
// * 	// boundary values use:
// * 	//    n.u = f
// * 	//    tau.(Lu) = g   (tangential component of the equations on the boundary
// * 	// To avoid a zero pivot we combine the above equations as
// * 	//     (n.u) n + ( tau.(Lu) ) tau = n f + tau g 
// * 	//
// * #ifdef USE_PPP
// * 	realSerialArray c; getLocalArrayWithGhostBoundaries(coeff,c);
// * #else
// * 	realSerialArray & c=coeff;
// * #endif
// * 
// * 	// this code was taken from maxwell/mxeigbc.C
// * //       assert( coeff.sparse!=NULL );
// * //       SparseRepForMGF & sparse = *coeff.sparse;
// * 
// * //       const int numberOfComponents = sparse.numberOfComponents;  // size of the system of equations
// * //       const int numberOfGhostLines = sparse.numberOfGhostLines;
// * //       const int stencilSize = sparse.stencilSize;
// * 
// * //       const int stencilDim=stencilSize*numberOfComponents; // number of coefficients per equation
// * //       const int stencilLength0=stencilSize;  // why is this here?
// *     
// * 	Range E=numberOfComponents, C=numberOfComponents;
// * 
// * 	// for readability, give names to equation numbers and component numbers
// * 	const int e0=0, e1=1, e2=numberOfDimensions>2 ? 2 : 1;
// * 	const int c0=0, c1=1, c2=numberOfDimensions>2 ? 2 : 1;
// * 
// * 	Index M(0,stencilSize);  // number of coefficients per component of each equation 
// * 	// ME : number of coefficients per equation ( = M*numberOfComponents)
// * 	// Index ME(e0*stencilDim,e2*stencilDim); 
// * 	Index ME(0,stencilDim);
// * 
// * 	const int ce00 = CE(c0,e0);
// * 	const int ce10 = CE(c1,e0);
// * 	const int ce20 = CE(c2,e0);
// * 
// * 	const int ce01 = CE(c0,e1);
// * 	const int ce11 = CE(c1,e1);
// * 	const int ce21 = CE(c2,e1);
// * 
// * 	const int ce02 = CE(c0,e2);
// * 	const int ce12 = CE(c1,e2);
// * 	const int ce22 = CE(c2,e2);
// * 
// * 	const int md0=M123CE(0,0,0,c0,e0);  // diagonal entry on eqn 0, component 0 
// * 	const int md1=M123CE(0,0,0,c1,e0);  // diagonal entry on eqn 0, component 1
// * 	const int md2=M123CE(0,0,0,c2,e0);  // diagonal entry on eqn 0, component 2
// * 
// * 	int side,axis;
// * 	int i1,i2,i3;
// * 	Index Ib1,Ib2,Ib3;
// * 	mg.update(MappedGrid::THEvertexBoundaryNormal | MappedGrid::THEcenterBoundaryTangent);
// * 	intArray & mask = mg.mask();
// * #ifdef USE_PPP
// * 	intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
// * #else
// * 	const intSerialArray & maskLocal=mask;
// * #endif
// * 	ForBoundary(side,axis)
// * 	{
// * 	  if( mg.boundaryCondition(side,axis)==Parameters::slipWall )
// * 	  {
// * #ifdef USE_PPP
// * 	    const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
// * 	    const RealArray & tangent = mg.centerBoundaryTangentArray(side,axis);
// * #else
// * 	    const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
// * 	    const RealArray & tangent = mg.centerBoundaryTangent(side,axis);
// * #endif
// * 	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
// * 	    // getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3);
// * 
// * 	    const int includeGhost=1;
// * 	    bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ib1,Ib2,Ib3,includeGhost);
// * 	    if( !ok ) continue;
// * 	    
// * 	    RealArray cn(ME,Ib1,Ib2,Ib3), ct1(ME,Ib1,Ib2,Ib3);
// * 	    cn=0.; ct1=0.; 
// * 	    int m,m1,m2,m3;
// * 
// * 	    // Here is the layout of a row of c, c(M123CE(m1,m2,m3,c,e),i1,i2,i3) (for fixed (i1,i2,i3))
// * 	    //       <-------------e0-------------> <-------------e1-------------> <-------------e2------------->
// * 	    //  c : [(...c0...)(...c1...)(...c2...),(...c0...)(...c1...)(...c2...),(...c0...)(...c1...)(...c2...))
// * 	    //        <-- M ->
// * 	    //        <----------- ME ----------->
// * 
// * 	    if( mg.numberOfDimensions()==2 )
// * 	    {
// * 	      FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
// * 	      {
// * 		if( maskLocal(i1,i2,i3)>0 )
// * 		{
// * 		  cn(md0,i1,i2,i3)=normal(i1,i2,i3,0);   // form the equation for n.u 
// * 		  cn(md1,i1,i2,i3)=normal(i1,i2,i3,1);
// * 		  FOR_M(m,M)
// * 		  { // form the eqn for tau.L 
// * 		    ct1(m     ,i1,i2,i3)=(tangent(i1,i2,i3,0)*c(m+ce00,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,1)*c(m+ce01,i1,i2,i3));
// * 		    ct1(m+ce10,i1,i2,i3)=(tangent(i1,i2,i3,0)*c(m+ce10,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,1)*c(m+ce11,i1,i2,i3));
// * 		  }
// * 		}
// * 	      }
// * 	      FOR_3(i1,i2,i3,Ib1,Ib2,Ib3)
// * 	      {
// * 		if( maskLocal(i1,i2,i3)>0 )
// * 		{
// * 		  FOR_M(m,ME)
// * 		  { // here we add two full rows of the matrix to form a new row
// * 		    c(m     ,i1,i2,i3)=normal(i1,i2,i3,0)*cn(m,i1,i2,i3) + tangent(i1,i2,i3,0)*ct1(m,i1,i2,i3);
// * 		    c(m+ce01,i1,i2,i3)=normal(i1,i2,i3,1)*cn(m,i1,i2,i3) + tangent(i1,i2,i3,1)*ct1(m,i1,i2,i3);
// * 		  }
// * 		}
// * 	      }
// * 	    }
// * 	    else
// * 	    { // *** 3D ***
// * 
// * 	      // ::display(tangent,"tangent");
// * 	      
// * 	      int ib1=(Ib1.getBase()+Ib1.getBound())/2, ib2=(Ib2.getBase()+Ib2.getBound())/2, 
// * 		ib3=(Ib3.getBase()+Ib3.getBound())/2;
// * 	      
// * 	      // ::display(c(ME     ,ib1,ib2,ib3),"START: c(ME,ib1,ib2,ib3)");
// * 	      // ::display(c(ME+ce01,ib1,ib2,ib3),"START: c(ME+ce01,ib1,ib2,ib3)");
// * 	      // ::display(c(ME+ce02,ib1,ib2,ib3),"START: c(ME+ce02,ib1,ib2,ib3)");
// * 
// * 	      RealArray ct2(ME,Ib1,Ib2,Ib3);
// * 	      ct2=0.;
// * 	      FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
// * 	      {
// * 		if( maskLocal(i1,i2,i3)>0 )
// * 		{
// * 		  cn(md0,i1,i2,i3)=normal(i1,i2,i3,0);   // form the equation for n.u 
// * 		  cn(md1,i1,i2,i3)=normal(i1,i2,i3,1);
// * 		  cn(md2,i1,i2,i3)=normal(i1,i2,i3,2);
// * 		  FOR_M(m,M)
// * 		  { // form the eqn's for tau1.L and tau2.L
// * 		    ct1(m     ,i1,i2,i3)=(tangent(i1,i2,i3,0)*c(m+ce00,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,1)*c(m+ce01,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,2)*c(m+ce02,i1,i2,i3));
// * 		    ct1(m+ce10,i1,i2,i3)=(tangent(i1,i2,i3,0)*c(m+ce10,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,1)*c(m+ce11,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,2)*c(m+ce12,i1,i2,i3));
// * 		    ct1(m+ce20,i1,i2,i3)=(tangent(i1,i2,i3,0)*c(m+ce20,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,1)*c(m+ce21,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,2)*c(m+ce22,i1,i2,i3));
// * 
// * 		    ct2(m     ,i1,i2,i3)=(tangent(i1,i2,i3,3)*c(m+ce00,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,4)*c(m+ce01,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,5)*c(m+ce02,i1,i2,i3));
// * 		    ct2(m+ce10,i1,i2,i3)=(tangent(i1,i2,i3,3)*c(m+ce10,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,4)*c(m+ce11,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,5)*c(m+ce12,i1,i2,i3));
// * 		    ct2(m+ce20,i1,i2,i3)=(tangent(i1,i2,i3,3)*c(m+ce20,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,4)*c(m+ce21,i1,i2,i3)+
// * 					  tangent(i1,i2,i3,5)*c(m+ce22,i1,i2,i3));
// * 		  }
// * 		}
// * 	      }
// * 	      // ::display(cn(ME,ib1,ib2,ib3),"cn");
// * 	      // ::display(ct1(ME,ib1,ib2,ib3),"ct1");
// * 	      // ::display(ct2(ME,ib1,ib2,ib3),"ct2");
// * 	      
// * 	      FOR_3(i1,i2,i3,Ib1,Ib2,Ib3)
// * 	      {
// * 		if( maskLocal(i1,i2,i3)>0 )
// * 		{
// * 		  FOR_M(m,ME)
// * 		  { // here we add two full rows of the matrix to form a new row
// * 		    c(m     ,i1,i2,i3)=( normal(i1,i2,i3,0) *cn(m,i1,i2,i3) + 
// * 					 tangent(i1,i2,i3,0)*ct1(m,i1,i2,i3) +
// * 					 tangent(i1,i2,i3,3)*ct2(m,i1,i2,i3));
// * 		    c(m+ce01,i1,i2,i3)=( normal(i1,i2,i3,1) *cn(m,i1,i2,i3) + 
// * 					 tangent(i1,i2,i3,1)*ct1(m,i1,i2,i3) +
// * 					 tangent(i1,i2,i3,4)*ct2(m,i1,i2,i3) );
// * 		    c(m+ce02,i1,i2,i3)=( normal(i1,i2,i3,2) *cn(m,i1,i2,i3) + 
// * 					 tangent(i1,i2,i3,2)*ct1(m,i1,i2,i3) +
// * 					 tangent(i1,i2,i3,5)*ct2(m,i1,i2,i3) );
// * 		  }
// * 
// * //                   if( false )
// * // 		  {
// * // 		    real norm=max( max(fabs(c(ME,i1,i2,i3))), 
// * // 				   max(fabs(c(ME+ce01,i1,i2,i3))), 
// * // 				   max(fabs(c(ME+ce02,i1,i2,i3))) );
// * 
// * // 		    if( norm<1.e-5 )
// * // 		    {
// * // 		      printf("********* ERROR: there is a null equation ***********\n");
// * // 		      ::display(c(ME     ,i1,i2,i3),"c(ME,i1,i2,i3)");
// * // 		      ::display(c(ME+ce01,i1,i2,i3),"c(ME+ce01,i1,i2,i3)");
// * // 		      ::display(c(ME+ce02,i1,i2,i3),"c(ME+ce02,i1,i2,i3)");
// * // 		      Overture::abort("done");
// * 		      
// * // 		    }
// * //		  }
// * 
// * 		}
// * 	      }
// * 	      
// * 	      
// * 	      // ::display(c(ME     ,ib1,ib2,ib3),"c(ME,ib1,ib2,ib3)");
// * 	      // ::display(c(ME+ce01,ib1,ib2,ib3),"c(ME+ce01,ib1,ib2,ib3)");
// * 	      // ::display(c(ME+ce02,ib1,ib2,ib3),"c(ME+ce02,ib1,ib2,ib3)");
// * 	      // Overture::abort("done");
// * 	      
// * 
// * 	    }
// * 	    
// * 	  }
// * 	}
// * 	
// * 	// ghost values are determined by vector symmetry:
// * 	coeff.applyBoundaryConditionCoefficients(Rx,Rx,BCTypes::vectorSymmetry,Parameters::slipWall);
// * 	// coeff.applyBoundaryConditionCoefficients(Rx,Rx,BCTypes::extrapolate,Parameters::slipWall);
// * 
// * 	if( true || debug() & 8 )
// * 	{
// * 	  ::display(c,sPrintF("coeff after old BC's for grid=%i",grid),pDebugFile,"%3.1f ");
// * 	}
// * 	// coeff.updateGhostBoundaries();
// * 	
// *       }
// *     }
// *     else
// *     {
// *       if( scalarSystem )
// * 	coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,BCTypes::allBoundaries);
// *       else
// *       {
// * 	for( n=0; n<numberOfDimensions; n++ )
// * 	{
// * 	  coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,  BCTypes::allBoundaries);
// * 	  coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::extrapolate,BCTypes::allBoundaries);
// * 	}
// *       }
// *     }
// *   } // end if !useOpt
// *   
// *   // *** don't do here?  coeff.finishBoundaryConditions();
// * 


  return 0;
}
// * 
// * #undef M123
// * #undef M123N
// * #undef CE
// * #undef M123CE
// * #undef MCE


//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+nc*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+nc*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+nc*(2),side,axis,grid)



//\begin{>>MappedGridSolverInclude.tex}{\subsection{applyBoundaryConditionsForImplicitTimeSteppingINS}} 
int Cgins::
applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & u, 
                                               realMappedGridFunction &uL,
					       realMappedGridFunction & gridVelocity,
					       real t,
					       int scalarSystem,
					       int grid )
// ======================================================================================
//  /Description:
//      Apply boundary conditions to the rhs side grid function used in the implicit solve.
// /u (input/output) : apply boundary conditions to this grid function.
// /gridVelocity (input) : for BC's on moving grids.
// /t (input) : time
// /scalarSystem (input) : 
// /grid (input) : component grid number.
//
//\end{MappedGridSolverInclude.tex}  
// ==========================================================================================
{
  // const real & t = cgf.t;
  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");
  Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");

  const bool assignTemperature = pdeModel==InsParameters::BoussinesqModel ||
                                 pdeModel==InsParameters::viscoPlasticModel;

  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");
  const int & nc = parameters.dbase.get<int >("numberOfComponents");
  const int & kc = parameters.dbase.get<int >("kc");

  const int ec = kc+1;  // epsilon for kEpsilon

  const Range & Ru = parameters.dbase.get<Range >("Ru"); // velocity components 
  Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
  Range V(uc,uc+parameters.dbase.get<int >("numberOfDimensions")-1); // velocity components
  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
  
  const int orderOfAccuracy = parameters.dbase.get<int >("orderOfAccuracy"); 
  

  Range K;
  if( turbulenceModel==Parameters::kEpsilon ||
      turbulenceModel==Parameters::kOmega )
  {
    K=Range(kc,ec);  // k and epsilon or k and omega
  }
  if( pdeModel==InsParameters::twoPhaseFlowModel )
  {
    K=Range(tc,tc);  // psi 
  }
  

  FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");

  typedef int BoundaryCondition;
  
  const BoundaryCondition & noSlipWall = Parameters::noSlipWall;
  const BoundaryCondition & slipWall   = Parameters::slipWall;
  const BoundaryCondition & inflowWithVelocityGiven = InsParameters::inflowWithVelocityGiven;
  const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
               = InsParameters::inflowWithPressureAndTangentialVelocityGiven;
  const BoundaryCondition & outflow = InsParameters::outflow;
  const BoundaryCondition & symmetry = Parameters::symmetry;
  const BoundaryCondition & dirichletBoundaryCondition = Parameters::dirichletBoundaryCondition;
  const Parameters::BoundaryCondition & interfaceBoundaryCondition= Parameters::interfaceBoundaryCondition;
  
  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
  //   extrapolate           = BCTypes::extrapolate,
  //   aDotU                 = BCTypes::aDotU,
  //   generalizedDivergence = BCTypes::generalizedDivergence,
  //   tangentialComponent   = BCTypes::tangentialComponent,
  //   vectorSymmetry        = BCTypes::vectorSymmetry,
  //   allBoundaries         = BCTypes::allBoundaries,
    normalComponent       = BCTypes::normalComponent;

  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");

  const int includeGhost=1;
  
  // *** assign boundary conditions for the implicit method ***** 

  const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
  const bool interfaceBoundaryConditionsAreSpecified=parameters.dbase.has_key("interfaceCondition");
  IntegerArray & interfaceCondition = (interfaceBoundaryConditionsAreSpecified ? 
				       parameters.dbase.get<IntegerArray>("interfaceCondition") :
				       Overture::nullIntArray() );

  if( parameters.getGridIsImplicit(grid) )
  {
    // ** NOTE that we are assigning the RHS for the implicit solve ***

    BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);

    // *wdh* 090725 -- add this check --
    MappedGrid & mg = *u.getMappedGrid();
    const int numberOfDimensions = mg.numberOfDimensions();
    
    bool assignSlipWall=false;
    bool assignNoSlipWall=false;
    bool assignInflowWithVelocityGiven=false;
    bool assignOutflow=false;
    bool assignTractionFree=false;
    bool assignAxisymmetric=false;
    bool assignSymmetry=false;
    bool assignDirichletBoundaryCondition=false;
    bool assignInflowWithPressureAndTangentialVelocityGiven=false;
    bool assignFreeSurfaceBoundaryCondition=false;
  
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	const int bc=mg.boundaryCondition(side,axis);
	switch (bc)
	{
	case 0 : break;
	case -1: break;
	case Parameters::slipWall:                   assignSlipWall=true; break;
	case Parameters::noSlipWall :                assignNoSlipWall=true; break;
	case InsParameters::inflowWithVelocityGiven: assignInflowWithVelocityGiven=true; break;
	case InsParameters::outflow:                 assignOutflow=true; break;
	case InsParameters::freeSurfaceBoundaryCondition: assignFreeSurfaceBoundaryCondition=true; break;
//	case InsParameters::tractionFree:            assignTractionFree=true; break;
	case Parameters::axisymmetric:               assignAxisymmetric=true; break;
//	case Parameters::symmetry :                  assignSymmetry=true; break;
	case Parameters::dirichletBoundaryCondition: assignDirichletBoundaryCondition=true; break;
	case InsParameters::inflowWithPressureAndTangentialVelocityGiven :
	  assignInflowWithPressureAndTangentialVelocityGiven=true; break;
	default: 
	  printF("applyBoundaryConditionsForImplicitTimeStepping:ERROR: unknown boundary condition =%i "
                 "on grid %i, side=%i, axis=%i\n",bc,grid,side,axis);
	  OV_ABORT("error");
	}
      }
    }

    // ===============================================
    // === assign dirichlet BC's for the velocity ====
    // ===============================================

    if( parameters.gridIsMoving(grid) )
    {
      u.applyBoundaryCondition(V,dirichlet,noSlipWall,gridVelocity,t);
      u.applyBoundaryCondition(V,dirichlet,dirichletBoundaryCondition,gridVelocity,t);
    }
    else
    {
      // u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,t,Overture::defaultBoundaryConditionParameters(),grid);
      // Allow for variable inflow on a wall by passing pBoundaryData:  *wdh* 2011/08/29
      u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,pBoundaryData,t,
                               Overture::defaultBoundaryConditionParameters(),grid);

      u.applyBoundaryCondition(V,dirichlet,dirichletBoundaryCondition,bcData,t,
				Overture::defaultBoundaryConditionParameters(),grid);
    }

    if( turbulenceModel==Parameters::kEpsilon ||
	turbulenceModel==Parameters::kOmega ||
        pdeModel==InsParameters::twoPhaseFlowModel )
    {
      u.applyBoundaryCondition(K,dirichlet,dirichletBoundaryCondition,bcData,t,Overture::defaultBoundaryConditionParameters(),grid);
      u.applyBoundaryCondition(K,dirichlet,inflowWithVelocityGiven,bcData,t,Overture::defaultBoundaryConditionParameters(),grid);

      // do this for now: 
      u.applyBoundaryCondition(K,dirichlet,noSlipWall,bcData,t,Overture::defaultBoundaryConditionParameters(),grid);
    }
    

//     u.applyBoundaryCondition(V,dirichlet,inflowWithVelocityGiven,
// 			      bcData,t,Overture::defaultBoundaryConditionParameters(),grid);

    u.applyBoundaryCondition(V,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,
			     Overture::defaultBoundaryConditionParameters(),grid);


    if( parameters.isAxisymmetric() )
      u.applyBoundaryCondition(vc,dirichlet,Parameters::axisymmetric,0,t);



    // =================================
    // === Assign non-Dirichlet BC's ===
    // =================================

    // slip wall: 
    //                   n.u=
    //              (t.u).n = 0

    const bool isRectangular=mg.isRectangular();
    if( true || !isRectangular )  // we need this now
      mg.update(MappedGrid::THEcenterBoundaryTangent | MappedGrid::THEvertexBoundaryNormal); 
    if( twilightZoneFlow )
      mg.update( MappedGrid::THEcenter );

    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    #else
      realSerialArray & uLocal = u; 
    #endif

    const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
      
    realArray & x= mg.center();
    #ifdef USE_PPP
      realSerialArray xLocal; 
      if( !rectangular || twilightZoneFlow ) 
        getLocalArrayWithGhostBoundaries(x,xLocal);
    #else
      const realSerialArray & xLocal = x;
    #endif
    
    int side,axis;
    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3,Ip1,Ip2,Ip3;
    ForBoundary( side,axis )
    {
      if( false &&   // set this to true for debugging
	  mg.boundaryCondition(side,axis)==noSlipWall && parameters.gridIsMoving(grid) )
      {
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	const intArray & mask = mg.mask();
	real uMin=0., uMax=0., vMin=0., vMax=0., guMin=0., guMax=0., gvMin=0., gvMax=0., err=0.;
	assert( gridVelocity.getBase(3)==0 && gridVelocity.getBound(3)==mg.numberOfDimensions()-1 );
	where( mask(Ib1,Ib2,Ib3)>0 )
	{
	  uMin = min(u(Ib1,Ib2,Ib3,uc));
	  uMax = max(u(Ib1,Ib2,Ib3,uc));
	  vMin = min(u(Ib1,Ib2,Ib3,vc));
	  vMax = max(u(Ib1,Ib2,Ib3,vc));
	  guMin=min(gridVelocity(Ib1,Ib2,Ib3,0));
	  guMax=max(gridVelocity(Ib1,Ib2,Ib3,0));
	  gvMin=min(gridVelocity(Ib1,Ib2,Ib3,1));
	  gvMax=max(gridVelocity(Ib1,Ib2,Ib3,1));
	  err = max(fabs(u(Ib1,Ib2,Ib3,vc)-gridVelocity(Ib1,Ib2,Ib3,1)));
	}
	printF("implicitBC: t=%9.3e: (grid,side,axis)=(%i,%i,%i) (uMin,uMax)=(%9.2e,%9.2e) (vMin,vMax)=(%9.2e,%9.2e)"
	       "\n   (guMin,guMax)=(%9.2e,%9.2e) (gvMin,gvMax)=(%9.2e,%9.2e) err=%8.2e <<<< \n",
	       t,grid,side,axis,uMin,uMax,vMin,vMax,guMin,guMax,gvMin,gvMax,err);
      }
      else if( mg.boundaryCondition(side,axis)==Parameters::freeSurfaceBoundaryCondition )
      {
        // ** for now we use a Neumann BC ** FIX ME
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
	if( !ok ) continue;
	if( !twilightZoneFlow )
	{
	  uLocal(Ig1,Ig2,Ig3,V)=0.;   // for extrapolation (or Neumann)
	}
	else
	{
	  // freeSurfaceBoundaryCondition: Neumann BC 
	  // printF(" implicit:RHS BC: neumman BC for freeSurfaceBoundaryCondition\n");
	    
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	  if( !ok ) continue;

	  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	  // should be extrapolation at outflow ? -- for now is Neumann
            
	  if(  isRectangular )
	  {
	    realSerialArray uex(Ib1,Ib2,Ib3,V);
	    int rectangular=0;
	    int nxd[3]={0,0,0}; //
	    nxd[axis]=1;  // x,y, or z derivative
	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,nxd[0],nxd[1],nxd[2],Ib1,Ib2,Ib3,V,t);

	    uLocal(Ig1,Ig2,Ig3,V)=uex(Ib1,Ib2,Ib3,V)*(2*side-1);
	  }
	  else
	  {
#ifdef USE_PPP
	    const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
#else
	    const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
#endif
           
	    realSerialArray uex(Ib1,Ib2,Ib3,V),uey(Ib1,Ib2,Ib3,V);
	    int rectangular=0;
	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
	    e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);

	    if( mg.numberOfDimensions()==2 )
	    {
	      for( int n=V.getBase(); n<=V.getBound(); n++ )
	      {
		uLocal(Ig1,Ig2,Ig3,n)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,n)+
				       normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,n));
	      }
	    }
	    else
	    {
	      realSerialArray uez(Ib1,Ib2,Ib3,V);
	      e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,V,t);
	      for( int n=V.getBase(); n<=V.getBound(); n++ )
	      {
		uLocal(Ig1,Ig2,Ig3,n)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,n)+
				       normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,n)+
				       normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,n));
	      }
	    }
	  }
	}
      }

      else if( mg.boundaryCondition(side,axis)==outflow )
      {
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
	if( !ok ) continue;

	if( turbulenceModel==Parameters::kEpsilon ||
	    turbulenceModel==Parameters::kOmega )
	{
	  uLocal(Ig1,Ig2,Ig3,K)=0.;   // for extrapolation (or Neumann)
	}


	if( !twilightZoneFlow )
	{
	  uLocal(Ig1,Ig2,Ig3,V)=0.;   // for extrapolation (or Neumann)
	}
	else
	{
	  if( parameters.dbase.get<int>("outflowOption")==0 && 
              parameters.dbase.get<int >("checkForInflowAtOutFlow")!=2  )
	  {
            // extrapolation at outflow
            // printF(" implicit:RHS BC: extrapolation BC for outflow\n");
            uLocal(Ig1,Ig2,Ig3,V)=0.;
	  }
	  else 
	  {
	    // Neumann BC 
            // printF(" implicit:RHS BC: neumman BC for outflow\n");
	    
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	    if( !ok ) continue;

	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    // should be extrapolation at outflow ? -- for now is Neumann
            
	    if(  isRectangular )
	    {
	      realSerialArray uex(Ib1,Ib2,Ib3,V);
	      int rectangular=0;
	      int nxd[3]={0,0,0}; //
	      nxd[axis]=1;  // x,y, or z derivative
	      e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,nxd[0],nxd[1],nxd[2],Ib1,Ib2,Ib3,V,t);

	      uLocal(Ig1,Ig2,Ig3,V)=uex(Ib1,Ib2,Ib3,V)*(2*side-1);
	    }
	    else
	    {
#ifdef USE_PPP
	      const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
#else
	      const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
#endif
           
	      realSerialArray uex(Ib1,Ib2,Ib3,V),uey(Ib1,Ib2,Ib3,V);
	      int rectangular=0;
	      e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
	      e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);

	      if( mg.numberOfDimensions()==2 )
	      {
		for( int n=V.getBase(); n<=V.getBound(); n++ )
		{
		  uLocal(Ig1,Ig2,Ig3,n)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,n)+
					 normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,n));
		}
	      }
	      else
	      {
		realSerialArray uez(Ib1,Ib2,Ib3,V);
		e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,V,t);
		for( int n=V.getBase(); n<=V.getBound(); n++ )
		{
		  uLocal(Ig1,Ig2,Ig3,n)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,n)+
					 normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,n)+
					 normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,n));
		}


	      }

	    }
	  
	  }
	}
      }
      else if( mg.boundaryCondition(side,axis)==slipWall )
      {
        // ----------------------
	// ------ slipWall ------
        // ----------------------
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
	if( !ok ) continue;
	    

	if( turbulenceModel==Parameters::kEpsilon ||
	    turbulenceModel==Parameters::kOmega )
	{
	  uLocal(Ig1,Ig2,Ig3,K)=0.;   // for Neumann
	}

	bool useNewSlipWall=true;
	if( !scalarSystem && useNewSlipWall )
	{
#ifdef USE_PPP
//	  RealArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
	  const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
	  const RealArray & tangent = mg.centerBoundaryTangentArray(side,axis);
#else
	  const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
	  const RealArray & tangent = mg.centerBoundaryTangent(side,axis);
#endif

// 	     printf(" slipWallBC: tangent dims=[%i,%i][%i,%i][%i,%i][%i,%i][%i,%i]\n",
// 		    tangent.getBase(0),tangent.getBound(0),
// 		    tangent.getBase(1),tangent.getBound(1),
// 		    tangent.getBase(2),tangent.getBound(2),
// 		    tangent.getBase(3),tangent.getBound(3),
// 		    tangent.getBase(4),tangent.getBound(4));

	  RealArray nDotU(Ib1,Ib2,Ib3);
	    
	  if( !twilightZoneFlow )
	  {
	    // project u(Ib1,Ib2,Ib3,V) --> n.u=0
	    if( mg.numberOfDimensions()==2 )
	    {
	      nDotU=(uLocal(Ib1,Ib2,Ib3,uc)*normal(Ib1,Ib2,Ib3,0)+
		     uLocal(Ib1,Ib2,Ib3,vc)*normal(Ib1,Ib2,Ib3,1));
	    }
	    else
	    {
	      nDotU=(uLocal(Ib1,Ib2,Ib3,uc)*normal(Ib1,Ib2,Ib3,0)+
		     uLocal(Ib1,Ib2,Ib3,vc)*normal(Ib1,Ib2,Ib3,1)+
		     uLocal(Ib1,Ib2,Ib3,wc)*normal(Ib1,Ib2,Ib3,2));
	    }
	    // now project the boundary values
	    uLocal(Ib1,Ib2,Ib3,uc) -= nDotU*normal(Ib1,Ib2,Ib3,0);
	    uLocal(Ib1,Ib2,Ib3,vc) -= nDotU*normal(Ib1,Ib2,Ib3,1);
	    if( mg.numberOfDimensions()==3 )
	      uLocal(Ib1,Ib2,Ib3,wc) -= nDotU*normal(Ib1,Ib2,Ib3,1);

	    uLocal(Ig1,Ig2,Ig3,V)=0.;   // vector symmetry RHS
	  }
	  else
	  {
	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    RealArray ue(Ib1,Ib2,Ib3,V);
	    int rectangular=0;
	    e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,V,t);

	    if( mg.numberOfDimensions()==2 )
	    {
	      nDotU=((uLocal(Ib1,Ib2,Ib3,uc)-ue(Ib1,Ib2,Ib3,uc))*normal(Ib1,Ib2,Ib3,0)+
		     (uLocal(Ib1,Ib2,Ib3,vc)-ue(Ib1,Ib2,Ib3,vc))*normal(Ib1,Ib2,Ib3,1));
	    }
	    else
	    {
	      nDotU=((uLocal(Ib1,Ib2,Ib3,uc)-ue(Ib1,Ib2,Ib3,uc))*normal(Ib1,Ib2,Ib3,0)+
		     (uLocal(Ib1,Ib2,Ib3,vc)-ue(Ib1,Ib2,Ib3,vc))*normal(Ib1,Ib2,Ib3,1)+
		     (uLocal(Ib1,Ib2,Ib3,wc)-ue(Ib1,Ib2,Ib3,wc))*normal(Ib1,Ib2,Ib3,2));
	    }
	    // now project the boundary values
	    uLocal(Ib1,Ib2,Ib3,uc) -= nDotU*normal(Ib1,Ib2,Ib3,0);
	    uLocal(Ib1,Ib2,Ib3,vc) -= nDotU*normal(Ib1,Ib2,Ib3,1);
	    if( mg.numberOfDimensions()==3 )
	      uLocal(Ib1,Ib2,Ib3,wc) -= nDotU*normal(Ib1,Ib2,Ib3,1);
	      

	    // vector symmetry RHS:

	    getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line in
	    ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ip1,Ip2,Ip3,includeGhost);
	    if( !ok ) continue;

	    RealArray up(Ip1,Ip2,Ip3,V),ug(Ig1,Ig2,Ig3,V);
	      
	    // exact soln on the first ghost line:
	    e.gd( ug,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ig1,Ig2,Ig3,V,t); 
	    // exact soln on the first line in:
	    e.gd( up,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ip1,Ip2,Ip3,V,t); 


	    RealArray tau1DotU(Ib1,Ib2,Ib3);
	    if( mg.numberOfDimensions()==2 )
	    {
	      nDotU=(normal(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,uc)+
		     normal(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,vc));
	      tau1DotU=(tangent(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,uc)+
			tangent(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,vc));
	      for( int d=0; d<mg.numberOfDimensions(); d++ )
	      {
		uLocal(Ig1,Ig2,Ig3,uc+d)=(ug(Ig1,Ig2,Ig3,uc+d)+ 
					  nDotU*normal(Ib1,Ib2,Ib3,d) -
					  tau1DotU*tangent(Ib1,Ib2,Ib3,d));
	      }
	    }
	    else if( mg.numberOfDimensions()==3 )
	    {
	      RealArray tau2DotU(Ib1,Ib2,Ib3);
	      nDotU=(normal(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,uc)+
		     normal(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,vc)+
		     normal(Ib1,Ib2,Ib3,2)*up(Ip1,Ip2,Ip3,wc));
		  
	      tau1DotU=(tangent(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,uc)+
			tangent(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,vc)+
			tangent(Ib1,Ib2,Ib3,2)*up(Ip1,Ip2,Ip3,wc));
		  
	      tau2DotU=(tangent(Ib1,Ib2,Ib3,3)*up(Ip1,Ip2,Ip3,uc)+
			tangent(Ib1,Ib2,Ib3,4)*up(Ip1,Ip2,Ip3,vc)+
			tangent(Ib1,Ib2,Ib3,5)*up(Ip1,Ip2,Ip3,wc));

	      for( int d=0; d<mg.numberOfDimensions(); d++ )
	      {
		uLocal(Ig1,Ig2,Ig3,uc+d)=(ug(Ig1,Ig2,Ig3,uc+d)+
					  nDotU*normal(Ib1,Ib2,Ib3,d) -
					  tau1DotU*tangent(Ib1,Ib2,Ib3,d) - 
					  tau2DotU*tangent(Ib1,Ib2,Ib3,3+d));
	      }
	    }

	  }
	    
	}
	else
	{
	  // old way for slip wall
	  const int unc=uc+axis;  // normal component
	  if( !twilightZoneFlow )
	  {
	    uLocal(Ig1,Ig2,Ig3,V)=0.;   // (t.u).n = 0
	    uLocal(Ib1,Ib2,Ib3,unc)=0.;   // n.u=   *********************** should be grid velocity *****
	  }
	  else
	  {
	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    if( isRectangular )
	    {
	      uLocal(Ig1,Ig2,Ig3,unc)=0.; // extrap normal component  // *** should use u.x+v.y=0
	      if( scalarSystem )
	      {
		// u(Ib1,Ib2,Ib3,unc)=e(mg,Ib1,Ib2,Ib3,unc,t);  // just dirichlet, not \nv\cdot\uv
		e.gd(uLocal,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,unc,t);  
	      }
	      else 
	      {
		// u(Ib1,Ib2,Ib3,unc)=e(mg,Ib1,Ib2,Ib3,unc,t)*(2*side-1);
		e.gd(uLocal,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,unc,t); 
		uLocal(Ib1,Ib2,Ib3,unc)*=(2*side-1);
	      }
	      
	      int nxd[3]={0,0,0}; //
	      nxd[axis]=1;  // x,y, or z derivative

	      if( true )
	      {
		// *wdh* 100329 -- new way for parallel
		realSerialArray ue(Ib1,Ib2,Ib3,V);
		e.gd(ue,xLocal,mg.numberOfDimensions(),rectangular,0,nxd[0],nxd[1],nxd[2],Ib1,Ib2,Ib3,V,t);
		uLocal(Ig1,Ig2,Ig3,V) = ue(Ib1,Ib2,Ib3,V)*(2*side-1);
	      }
	      else
	      {
		// old way
		for( int m=0; m<mg.numberOfDimensions()-1; m++ )
		{
		  int c= uc + ((axis+1+m) % mg.numberOfDimensions());
		  // ::display( u(Ig1,Ig2,Ig3,c), "ghost value before implicit BC", "%5.2f ");
		  e.gd(u,0,nxd[0],nxd[1],nxd[2],Ib1,Ib2,Ib3,c, Ig1,Ig2,Ig3,c,t);  
		  u(Ig1,Ig2,Ig3,c)*=(2*side-1);
		  // ::display( u(Ig1,Ig2,Ig3,c), "ghost value after implicit BC", "%5.2f ");
		}
	      }
	      
	    }
	    else
	    {
	      // **** not rectangular ****
	      
	      const int ut1c=uc + ((axis+1)%mg.numberOfDimensions());
	      const int ut2c=uc + ((axis+2)%mg.numberOfDimensions());
	  
#ifdef USE_PPP
	      const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
	      const realSerialArray & tangent = mg.centerBoundaryTangentArray(side,axis);
#else
	      const realArray & normal  = mg.vertexBoundaryNormal(side,axis);
	      const realArray & tangent = mg.centerBoundaryTangent(side,axis);
#endif


	      if( mg.numberOfDimensions()==2 )
	      {
		uLocal(Ig1,Ig2,Ig3,unc)=0.; // extrap normal component

		realSerialArray ue(Ib1,Ib2,Ib3,V), &uex=ue, uey(Ib1,Ib2,Ib3,V); 
		e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,V,t);
                
		// uLocal(Ib1,Ib2,Ib3,unc)= (normal(Ib1,Ib2,Ib3,0)*e(mg,Ib1,Ib2,Ib3,uc,t)+
		//		             normal(Ib1,Ib2,Ib3,1)*e(mg,Ib1,Ib2,Ib3,vc,t));
		uLocal(Ib1,Ib2,Ib3,unc)= (normal(Ib1,Ib2,Ib3,0)*ue(Ib1,Ib2,Ib3,uc)+
					  normal(Ib1,Ib2,Ib3,1)*ue(Ib1,Ib2,Ib3,vc));

// 		uLocal(Ig1,Ig2,Ig3,ut1c)=
// 		  tangent(Ib1,Ib2,Ib3,0)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,uc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,uc,t)
// 		    )
// 		  +tangent(Ib1,Ib2,Ib3,1)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,vc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,vc,t)
// 		    );

		e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
		e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);
		uLocal(Ig1,Ig2,Ig3,ut1c)=(tangent(Ib1,Ib2,Ib3,0)*(
					    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
					    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc)
					    )
					  +tangent(Ib1,Ib2,Ib3,1)*(
					    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
					    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc)
					    ));

	      }
	      else
	      {
		uLocal(Ig1,Ig2,Ig3,unc)=0.; // extrap normal component

		realSerialArray ue(Ib1,Ib2,Ib3,V), &uex=ue, uey(Ib1,Ib2,Ib3,V), uez(Ib1,Ib2,Ib3,V); 
		e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,V,t);

		uLocal(Ib1,Ib2,Ib3,unc)= (normal(Ib1,Ib2,Ib3,0)*ue(Ib1,Ib2,Ib3,uc)+
					  normal(Ib1,Ib2,Ib3,1)*ue(Ib1,Ib2,Ib3,vc)+
					  normal(Ib1,Ib2,Ib3,2)*ue(Ib1,Ib2,Ib3,wc));

		e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
		e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);
		e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,V,t);

		uLocal(Ig1,Ig2,Ig3,ut1c)=
		  tangent(Ib1,Ib2,Ib3,0)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,uc)
		    )
		  +tangent(Ib1,Ib2,Ib3,1)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,vc)
		    )
		  +tangent(Ib1,Ib2,Ib3,2)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,wc)
		    );
		uLocal(Ig1,Ig2,Ig3,ut2c)=
		  tangent(Ib1,Ib2,Ib3,3)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,uc)
		    )
		  +tangent(Ib1,Ib2,Ib3,4)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,vc)
		    )
		  +tangent(Ib1,Ib2,Ib3,5)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,wc)
		    );

	      }
	    }
	  }
	}
      }
      else if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
      {
	// u.n =  v.n=
	assert( mg.numberOfDimensions()==2 );
	  
#ifdef USE_PPP
	assert( mg.rcData->pVertexBoundaryNormal[axis][side]!=NULL );
	const realSerialArray & normal = *mg.rcData->pVertexBoundaryNormal[axis][side];
#else
	const realArray & normal  = mg.vertexBoundaryNormal(side,axis);
#endif


	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

	bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;
	ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);

	if( !twilightZoneFlow )
	{
	  uLocal(Ig1,Ig2,Ig3,uc)=0.; 
	  uLocal(Ig1,Ig2,Ig3,vc)=0.; 
	}
	else
	{
	  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	  realSerialArray uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 

	  e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,uc,t);
	  e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,uc,t);


	  uLocal(Ig1,Ig2,Ig3,uc)=(normal(Ib1,Ib2,Ib3,0)*uex+
				  normal(Ib1,Ib2,Ib3,1)*uey);
	  uLocal(Ig1,Ig2,Ig3,vc)=0.; // v.yy=0
	}
      }
      else if( mg.boundaryCondition(side,axis)==InsParameters::inflowWithPressureAndTangentialVelocityGiven )
      {
        // ----------------------------------------------------------------
	// ------ inflow with pressure and tangential velocity given ------
        // ----------------------------------------------------------------

	// printF("insIMPBC: assign RHS for inflow with pressure and tangential velocity given, t=%8.2e\n",t);
	
        // Tangential components of the velocity are zero:
        //    tau.u = 0 
        // Neumann BC on all components (we could also extrapolate)
        //    u.n = 0 

        #ifdef USE_PPP
	  assert( mg.rcData->pVertexBoundaryNormal[axis][side]!=NULL );
	  const realSerialArray & normal = *mg.rcData->pVertexBoundaryNormal[axis][side];
        #else
	  const realArray & normal  = mg.vertexBoundaryNormal(side,axis);
        #endif

	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;
	ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);


	if( isRectangular )
	{
          // normal and tangential components: 
          const int unc  = uc+axis;
	  const int ut1c = uc + ( (axis+1) % numberOfDimensions );
	  const int ut2c = uc + ( (axis+2) % numberOfDimensions );
	  
	  if( !twilightZoneFlow )
	  {
	    uLocal(Ib1,Ib2,Ib3,ut1c)=0.;   // RHS for tau.u = 0 
            if( numberOfDimensions>2 )
             uLocal(Ib1,Ib2,Ib3,ut2c)=0.;

	    uLocal(Ig1,Ig2,Ig3,V)=0.;   // RHS for u.n = 0 
	  }
	  else
	  {
	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    realSerialArray ue(Ib1,Ib2,Ib3,V), uex(Ib1,Ib2,Ib3,V), uey(Ib1,Ib2,Ib3,V); 

            e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,V,t);
            uLocal(Ib1,Ib2,Ib3,ut1c)=ue(Ib1,Ib2,Ib3,ut1c);   // RHS for tau.u = 0 
            if( numberOfDimensions>2 )
             uLocal(Ib1,Ib2,Ib3,ut2c)=ue(Ib1,Ib2,Ib3,ut2c);


	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
	    e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);

            // we don't need the normals here *** fix me ***
	    if( numberOfDimensions==2 )
	    {
	      uLocal(Ig1,Ig2,Ig3,uc)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc));
	      uLocal(Ig1,Ig2,Ig3,vc)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc));
	    }
	    else
	    {
              realSerialArray & uez = ue;
	      e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,V,t);
	      for( int dir=0; dir<numberOfDimensions; dir++ )
	      {
		int c = uc+dir;
		uLocal(Ig1,Ig2,Ig3,c)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,c)+
				       normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,c)+
				       normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,c));
	      }
	      
	    }
	    

	  }
	  
	}
	else  // not rectangular 
	{
	  if( !twilightZoneFlow )
	  {
            // We assume the the values on the boundary are the correct RHS for the interior PDE
            // Here we set the tangential components to zero : 

            // rhs = (n.u)n + (tau.u) tau 
            //     = (n.u)n
	    realSerialArray nDotU(Ib1,Ib2,Ib3);
	    
            if( numberOfDimensions==2 )
              nDotU = (normal(Ib1,Ib2,Ib3,0)*uLocal(Ib1,Ib2,Ib3,uc) + 
		       normal(Ib1,Ib2,Ib3,1)*uLocal(Ib1,Ib2,Ib3,vc));
            else
              nDotU = (normal(Ib1,Ib2,Ib3,0)*uLocal(Ib1,Ib2,Ib3,uc) + 
		       normal(Ib1,Ib2,Ib3,1)*uLocal(Ib1,Ib2,Ib3,vc)+ 
		       normal(Ib1,Ib2,Ib3,2)*uLocal(Ib1,Ib2,Ib3,wc));
	    for( int dir=0; dir<numberOfDimensions; dir++ )
	    {
	      int c = uc+dir;
	      uLocal(Ib1,Ib2,Ib3,c) = nDotU*normal(Ib1,Ib2,Ib3,dir);
	    }

	    uLocal(Ig1,Ig2,Ig3,V)=0.;   // RHS for u.n = 0 
	  }
	  else
	  {
            // Here we need to set the tangental component of u : 
            //   tau.u = tau.ue 
            // u <- ue + ( n.u - n.ue) n 

	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    realSerialArray ue(Ib1,Ib2,Ib3,V);

            e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,V,t);

	    realSerialArray nDotU(Ib1,Ib2,Ib3);
	    
            if( numberOfDimensions==2 )
              nDotU = (normal(Ib1,Ib2,Ib3,0)*(uLocal(Ib1,Ib2,Ib3,uc)-ue(Ib1,Ib2,Ib3,uc)) + 
		       normal(Ib1,Ib2,Ib3,1)*(uLocal(Ib1,Ib2,Ib3,vc)-ue(Ib1,Ib2,Ib3,vc)));
            else			      								       
              nDotU = (normal(Ib1,Ib2,Ib3,0)*(uLocal(Ib1,Ib2,Ib3,uc)-ue(Ib1,Ib2,Ib3,uc)) + 
		       normal(Ib1,Ib2,Ib3,1)*(uLocal(Ib1,Ib2,Ib3,vc)-ue(Ib1,Ib2,Ib3,vc))+ 
		       normal(Ib1,Ib2,Ib3,2)*(uLocal(Ib1,Ib2,Ib3,wc)-ue(Ib1,Ib2,Ib3,wc)));

	    for( int dir=0; dir<numberOfDimensions; dir++ )
	    {
	      int c = uc+dir;
	      uLocal(Ib1,Ib2,Ib3,c) = ue(Ib1,Ib2,Ib3,c) + nDotU*normal(Ib1,Ib2,Ib3,dir);
	    }

	    // Neumann BC: 
            realSerialArray uex(Ib1,Ib2,Ib3,V), uey(Ib1,Ib2,Ib3,V);
	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
	    e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);

	    if( numberOfDimensions==2 )
	    {
	      uLocal(Ig1,Ig2,Ig3,uc)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc));
	      uLocal(Ig1,Ig2,Ig3,vc)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc));
	    }
	    else
	    {
              realSerialArray & uez = ue;
	      e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,V,t);
	      for( int dir=0; dir<numberOfDimensions; dir++ )
	      {
		int c = uc+dir;
		uLocal(Ig1,Ig2,Ig3,c)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,c)+
				       normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,c)+
				       normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,c));
	      }
	      
	    }
	    

	  }

	}

      }
      else if( mg.boundaryCondition(side,axis)>0 &&
               mg.boundaryCondition(side,axis)!=dirichletBoundaryCondition &&
               mg.boundaryCondition(side,axis)!=noSlipWall && 
               mg.boundaryCondition(side,axis)!=inflowWithVelocityGiven )
      {
	printF(" ***Cgins::implicitBC:ERROR: bc=%i not implemented yet\n",mg.boundaryCondition(side,axis));
	OV_ABORT("finish me");
      }
      


      if( assignTemperature )
      {
        // ==========================================
        // === Assign the RHS for the Temperature ===
        // ==========================================

	if( debug() & 4 )
	  printF(" ***Cgins::implicitBC: Assign RHS for the Temperature equation.\n");


	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

	bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);

	// if( debug() & 4 )
	//   printF(" ***Cgins::implicitBC: (side,axis,grid)=(%i,%i,%i) ok=%i\n",side,axis,grid,ok);

	if( !ok ) continue;
	ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
	if( !ok ) continue;
	      

        int bc0 = mg.boundaryCondition(side,axis);

	if( interfaceType(side,axis,grid) != Parameters::noInterface ) 
	{
          // This face is a domain interface 

          // We only support interfaces on no-slip walls:
          assert( bc0 == noSlipWall );

          // When solving the interface equations by iteration, the BC on this face may be 
          // dirichlet or mixed: 
	  bc0 = interfaceCondition(side,axis,grid);
	  assert( bc0==Parameters::dirichletInterface || bc0==Parameters::neumannInterface );
	  // printP("** Cgins:implicitBC: setting an interface bc(%i,%i,%i)=%i\n",side,axis,grid,bc0);
	}

        bool assignDirichlet=false, assignNeumann=false, assignExtrapolate=false;
	if( bc0==slipWall || 
            bc0==noSlipWall || 
            bc0==inflowWithVelocityGiven || 
            bc0==dirichletBoundaryCondition ||
            bc0==Parameters::dirichletInterface ||
            bc0==Parameters::neumannInterface ||
            bc0==inflowWithPressureAndTangentialVelocityGiven ||
            bc0==Parameters::freeSurfaceBoundaryCondition )
	{
	  assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
	  
	  if( debug() & 4 )
	    printF("++++Cgins::implicitBC: Mixed BC for T: (grid,side,axis)=(%i,%i,%i), %3.2f*T+%3.2f*T.n=%3.2f \n",
		   grid,side,axis, 
		   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid));

	  assignDirichlet = mixedNormalCoeff(tc,side,axis,grid)==0.;  // coeff of T.n 
	  assignNeumann=!assignDirichlet;
	}
	else if( bc0==symmetry || bc0==Parameters::axisymmetric || bc0==InsParameters::tractionFree )
	{
          assignNeumann=true;
          if( !(mixedCoeff(tc,side,axis,grid)==0. || mixedNormalCoeff(tc,side,axis,grid)==1.) )
	  {
	    printF("ERROR: mixedCoeff(tc,side,axis,grid)=%8.2e mixedNormalCoeff(tc,side,axis,grid)=%8.2e for bc0=%i\n",
		   mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid),bc0);
	    Overture::abort("error");
	  }
	}
	else if( bc0==outflow  || bc0==InsParameters::convectiveOutflow )
	{
	  assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
          assignNeumann=true;  // ******* do this for now --> change this to extrapolate
          // assignExtrapolate=true;
	}
	else if( bc0 > 0 )
	{
	  printF("Cgins::applyBCforImplicitTimeStepping:ERROR unknown BC value for T! \n"
		 "cg0[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i\n",grid,side,axis,
		 mg.boundaryCondition(side,axis));
	  Overture::abort("Cgins::applyBCforImplicitTimeStepping:ERROR unknown BC value for T");
	}

	// if( debug() & 4 )
	//   printF(" ***Cgins::implicitBC: (side,axis,grid)=(%i,%i,%i) assignDirichlet=%i, assignNeumann=%i\n",
        //            side,axis,grid,assignDirichlet,assignNeumann);
	

        if( bc0==Parameters::dirichletInterface )
	{
	  assert( assignDirichlet );
	  
	  if( debug() & 4 )
	    printP("** Cgins:applyImpBC:T: setting RHS for a dirichlet interface bc(%i,%i,%i)=%i\n",side,axis,grid,bc0);
          RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	  uLocal(Ib1,Ib2,Ib3,tc)=bd(Ib1,Ib2,Ib3,tc);

	  if( debug() & 4 ) 
            ::display(bd(Ib1,Ib2,Ib3,tc),"Cgins:applyImpBC:T: RHS for Dirichlet interface BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%5.2f ");
	}
        else if( bc0==Parameters::neumannInterface )
	{
          assert( assignNeumann );
	  
	  if( debug() & 4 )
	    printP("** Cgins:applyImpBC: setting RHS for a neumann interface bc(%i,%i,%i)=%i\n",side,axis,grid,bc0);
	  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	  if( debug() & 4 ) 
            ::display(bd(Ib1,Ib2,Ib3,tc),"Cgins:applyImpBC:T: RHS for Neumann interface BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%6.3f ");

          uLocal(Ig1,Ig2,Ig3,tc)=bd(Ib1,Ib2,Ib3,tc);

	  if( false && twilightZoneFlow ) //  TEMP *******************************************************
	  {
            #ifdef USE_PPP
  	      const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
            #else
	      const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
            #endif

	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
	    e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
	    e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);

            real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
	    if( mg.numberOfDimensions()==2 )
	    {
	      uLocal(Ig1,Ig2,Ig3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
				         normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
	    }
	    else
	    {
              realSerialArray uez(Ib1,Ib2,Ib3);
              e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
	      uLocal(Ig1,Ig2,Ig3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
				         normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
				         normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
	    }
	    if( debug() & 4 ) 
	      ::display(uLocal(Ig1,Ig2,Ig3,tc),"Cgins:applyImpBC:T: TRUE RHS for Neumann interface BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%6.3f ");

	  } // *******************************************
	  

	  if( true )
	  {
	    // new way  -- this code is duplicated from ad/src/applyBoundaryConditions.C  -- we should share this ---

            // If the adjacent face is a dirichlet BC then we change the right-hand-side on the extended
            // boundary point: 
            // 
            //   bc=dirichlet
            //       ------c--o   <-  adjust RHS to neuman interface here 
            //             |
            //       ------+--+   <- interface side 
            //             |
            // Since the interior equation is not applied at the corner pt "c" we can not just impose
            // [ k T.n ]=0 since we need another equation to determine the 2 ghost point values (there is
            // another ghost pt value on the other side of the interface ). Therefore we set 
            //  k*T.n = given at point "c" 

            #ifdef USE_PPP
	      const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
            #else
	      const realArray & normal  = mg.vertexBoundaryNormal(side,axis);
            #endif

            Range N(tc,tc);
	    Index Jbv[3], &Jb1=Jbv[0], &Jb2=Jbv[1], &Jb3=Jbv[2];
	    Index Jgv[3], &Jg1=Jgv[0], &Jg2=Jgv[1], &Jg3=Jgv[2];

            // loop over adjacent sides
	    for( int dir=1; dir<mg.numberOfDimensions(); dir++ ) for( int side2=0; side2<=1; side2++ )
	    {
	      int dir2 = (axis+dir) % mg.numberOfDimensions();
              if( mg.boundaryCondition(side2,dir2)==dirichletBoundaryCondition )
	      {
		Jb1=Ib1, Jb2=Ib2, Jb3=Ib3;
		Jg1=Ig1, Jg2=Ig2, Jg3=Ig3;
                // check for parallel: 
                if( Jbv[dir2].getBase()  > mg.gridIndexRange(side2,dir2) ||
                    Jbv[dir2].getBound() < mg.gridIndexRange(side2,dir2) )
		{  
		  ok=false;
		  continue;
		}
                Jbv[dir2]=mg.gridIndexRange(side2,dir2);
		Jgv[dir2]=mg.gridIndexRange(side2,dir2);

		if( debug() & 8 )
		{
		  printP("Cgins:impBC: set RHS to exact where interface meets adj-dirichlet "
			 " (side,axis)=(%i,%i) (side2,dir2)=(%i,%i) Jv=[%i,%i][%i,%i][%i,%i]\n",
			 side,axis,side2,dir2,Jb1.getBase(),Jb1.getBound(),Jb2.getBase(),Jb2.getBound(),
			 Jb3.getBase(),Jb3.getBound() );
		}
		
		if( twilightZoneFlow )
		{
		  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
		  RealArray ue(Jb1,Jb2,Jb3,N), uex(Jb1,Jb2,Jb3,N), uey(Jb1,Jb2,Jb3,N);
		  int rectangular=0;
		  e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Jb1,Jb2,Jb3,N,t);
		  e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Jb1,Jb2,Jb3,N,t);
		  e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Jb1,Jb2,Jb3,N,t);

		  if( mg.numberOfDimensions()==2 )
		  {
		    for( int n=N.getBase(); n<=N.getBound(); n++ )
		    {
		      real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
		      ue(Jb1,Jb2,Jb3,n)=a1*(uex(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,0)+
					    uey(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,1))+ a0*ue(Jb1,Jb2,Jb3,n);
		    }
                    uLocal(Jg1,Jg2,Jg3,N)=ue(Jb1,Jb2,Jb3,N);
		  }
                  else 
		  {
                    RealArray uez(Jb1,Jb2,Jb3,N);
		    e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Jb1,Jb2,Jb3,N,t);
		    for( int n=N.getBase(); n<=N.getBound(); n++ )
		    {
		      real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
		      ue(Jb1,Jb2,Jb3,n)=a1*(uex(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,0)+
					    uey(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,1)+
					    uez(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,2))+ a0*ue(Jb1,Jb2,Jb3,n);
		    }
                    uLocal(Jg1,Jg2,Jg3,N)=ue(Jb1,Jb2,Jb3,N);
		  }
		  
		}
		else
		{
                  //MappedGridOperators & op = *(u.getOperators());
		  //op.derivative(MappedGridOperators::xDerivative,uLocal,ux  ,Jb1,Jb2,Jb3,N);
		  //op.derivative(MappedGridOperators::yDerivative,uLocal,uy  ,Jb1,Jb2,Jb3,N);
		  //op.derivative(MappedGridOperators::zDerivative,uLocal,uz  ,Jb1,Jb2,Jb3,N);

                  // Here we assume the Dirichlet BC is a constant value so that the normal derivative  *fix me*
                  // of the solution along the Dirichlet BC is zero: 
                  // *NOTE* if we fix this for the case of a variable Dirichlet BC then we must also adjust the 
                  //   interface getRHS to eval   k*u.n - k*ue.n so that the residuals in the interface equations 
                  //   will go to zero. 
		  for( int n=N.getBase(); n<=N.getBound(); n++ )
		  {
		    real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
                    // note: uLocal(Jb1,Jb2,Jb3,n) has been set above by the Dirichlet BC 
		    uLocal(Jg1,Jg2,Jg3,n)=a0*uLocal(Jb1,Jb2,Jb3,n);
		  }
		    
		}
	      }
	    }


	  }

	  
	}  // end of bc0==neumannInterface
	else if( assignDirichlet )
	{
	  if( !twilightZoneFlow )
	  {
            // *wdh* 110803 - fixed to use boundaryData if supplied
            if( pBoundaryData[side][axis]==NULL )
	    {
              // printF(" $$$$$$$$$$$$ applyBC DIRICHLET for implicit TEMPERATURE system : mixedRHS $$$$$$$$$$\n");
	      uLocal(Ib1,Ib2,Ib3,tc)=mixedRHS(tc,side,axis,grid);
	    }
	    else
	    {  
              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
  	      // printF(" $$$$$$$$$$$$ applyBC DIRICHLET for implicit TEMPERATURE system : boundaryData $$$$$$$$$$\n");
              uLocal(Ib1,Ib2,Ib3,tc)=bd(Ib1,Ib2,Ib3,tc);
	    }
	  
	  }
	  else
	  {
	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    realSerialArray ue(Ib1,Ib2,Ib3);
	    e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
	    uLocal(Ib1,Ib2,Ib3,tc)=ue;
	  }
	  
	}
	else if( assignNeumann )
	{
	  if( !twilightZoneFlow )
	  {
            // *wdh* 110803 - fixed to use boundaryData if supplied
   	    if( pBoundaryData[side][axis]==NULL )
	    {
  	      // printF(" $$$$$$$$$$$$ applyBC for implicit TEMPERATURE system : mixedRHS $$$$$$$$$$\n");
	      uLocal(Ig1,Ig2,Ig3,tc)=mixedRHS(tc,side,axis,grid);  // set ghost value to RHS for neumann BC
	    }
	    else
	    {  
              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
  	      // printF(" $$$$$$$$$$$$ applyBC for implicit TEMPERATURE system : boundaryData $$$$$$$$$$\n");
              uLocal(Ig1,Ig2,Ig3,tc)=bd(Ib1,Ib2,Ib3,tc);
	    }
	    
	  }
	  else
	  {
	    if( debug() & 4 ) printF("Cgins::applyBCForImplicit: apply neumann BC for T, (side,axis,grid)=(%i,%i,%i)\n",
				     side,axis,grid);
	    
            #ifdef USE_PPP
  	      const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
            #else
	      const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
            #endif

	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
	    e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
	    e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);

            real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
	    if( mg.numberOfDimensions()==2 )
	    {
	      uLocal(Ig1,Ig2,Ig3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
				         normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
	    }
	    else
	    {
              realSerialArray uez(Ib1,Ib2,Ib3);
              e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
	      uLocal(Ig1,Ig2,Ig3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
				         normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
				         normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
	    }
	  }
	}
	else if( bc0>0 )
	{
	  assert( assignExtrapolate==true );
	  uLocal(Ig1,Ig2,Ig3,tc)=0.;
	}

      }

    } // end for boundary
    
    // *wdh* 3243 if( assignTemperature  )
    if( assignTemperature || orderOfAccuracy>2 ) // *wdh* ABC
    {
      // ************ try this ********* 080909
      u.updateGhostBoundaries();
    }
    
  } // end if grid is implicit 
  

  return 0;
}

#undef DAI2
#undef DAI3

// ===================================================================================================================
/// \brief Assign the boundaryCondition data for passing to Oges (predfined equations) 
/// when it builds the implicit system.
///
/// \detail This function is called by DomainSolver::formMatrixForImplicitSolve
/// 
/// \param cgf (input) : A grid function holding the current grid.
/// \param boundaryConditions (output) : boundary conditions for Oges
/// \param boundaryConditionData (output) : boundary condition data for Oges 
/// \param imp (input) : the number of the implicit system being solved
/// 
// ====================================================================================================================
int Cgins::
setOgesBoundaryConditions( GridFunction &cgf, IntegerArray & boundaryConditions, RealArray &boundaryConditionData,
                           const int imp )
{
  CompositeGrid & cg = cgf.cg;

  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");
  int & implicitSolverForTemperature = parameters.dbase.get<int>("implicitSolverForTemperature");
  int & numberOfImplicitVelocitySolvers = parameters.dbase.get<int>("numberOfImplicitVelocitySolvers");
  
  const bool solveForTemperture = pdeModel==InsParameters::BoussinesqModel && imp==implicitSolverForTemperature; 

  const bool & useFullSystemForImplicitTimeStepping = parameters.dbase.get<bool >("useFullSystemForImplicitTimeStepping");
  const int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");

  if( debug() & 1 && solveForTemperture )
    printF("Cgins::setOgesBoundaryConditions: Solve for T imp=%i, numberOfImplicitSolvers=%i\n",imp,numberOfImplicitSolvers);

  const int & tc = parameters.dbase.get<int >("tc");
  const int & nc = parameters.dbase.get<int >("numberOfComponents");
  const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");

  boundaryConditions.redim(2,3,cg.numberOfComponentGrids());  // for Oges
  boundaryConditions=0;
  boundaryConditionData.redim(2,2,3,cg.numberOfComponentGrids());               // for Oges
  boundaryConditionData=0.;

  const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
  const bool interfaceBoundaryConditionsAreSpecified=parameters.dbase.has_key("interfaceCondition");
  IntegerArray & interfaceCondition = (interfaceBoundaryConditionsAreSpecified ? 
				       parameters.dbase.get<IntegerArray>("interfaceCondition") :
				       Overture::nullIntArray() );
  int grid,side,axis;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const bool isRectanglar=mg.isRectangular();
	  
    if( !solveForTemperture )
    {
      // ========================================
      // ==== Assign BC's for the velocities ====
      // ========================================
      ForBoundary( side,axis )
      {
	boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  // default
      
	switch( mg.boundaryCondition(side,axis) )
	{
	case Parameters::slipWall:
	  if( !(isRectanglar && numberOfImplicitSolvers>0) && !useFullSystemForImplicitTimeStepping )
	  {
	    printF("Cgins::setOgesBoundaryConditions:ERROR:A slip wall BC cannot be treated with scalar\n"
                   " implicit system on a grid that is not Cartesian since the boundary conditions \n"
                   " do not decouple. This case should not happen!\n");
            printF(" numberOfImplicitSolvers=%i\n"
                   " numberOfImplicitVelocitySolvers=%i \n"
                   " useFullSystemForImplicitTimeStepping=%i\n"
                   " scalarSystemForImplicitTimeStepping=%i\n",
                   numberOfImplicitSolvers,numberOfImplicitVelocitySolvers,
                   useFullSystemForImplicitTimeStepping,(int)scalarSystemForImplicitTimeStepping);
	    
	    OV_ABORT("error");
	  }
	  
	  if( axis==imp )
	  { // The normal component to a horizontal or vertical wall has a dirichlet BC
	    boundaryConditions(side,axis,grid)=OgesParameters::dirichlet; 
	  }
	  else
	  {
	    boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
	  }
	  break;
	case Parameters::noSlipWall:
	case InsParameters::inflowWithVelocityGiven:
	case Parameters::dirichletBoundaryCondition:
	  // these are all dirichlet BC's for \uv
	  break;
	case Parameters::axisymmetric:
	  // boundaryConditions(side,axis,grid)=OgesParameters::axisymmetric;  // *wdh* 080718

          assert( numberOfImplicitVelocitySolvers==2 || useFullSystemForImplicitTimeStepping );  // *wdh* 080817
	  if( imp==0 )
	  { // u has a Neumann BC
	    boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
	  }
	  else
	  { // v has a dirichlet
            boundaryConditions(side,axis,grid)=OgesParameters::dirichlet; 
	  }
	  
	  break;
	case Parameters::symmetry:
	  Overture::abort("implicit method not implemented for this BC");
	  break;
	case InsParameters::inflowWithPressureAndTangentialVelocityGiven:
	  assert( isRectanglar && numberOfImplicitSolvers>0 );
	  if( axis!=imp )
	  {// The tangential component to a horizontal or vertical wall has a dirichlet BC
	    boundaryConditions(side,axis,grid)=OgesParameters::dirichlet; 
	  }
	  else
	  {
	    boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
	  }
	  break;
	case InsParameters::outflow:
	case InsParameters::tractionFree:
	case InsParameters::convectiveOutflow:
          if( parameters.dbase.get<int>("outflowOption")==1 ||
              parameters.dbase.get<int >("checkForInflowAtOutFlow")==2 )
	  {
            // printF("implicitMatrix: use Neumann BC at outflow\n");
  	    boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
	  }
          else
	  {
            // printF("implicitMatrix: use extrapolate BC at outflow\n");
            // Specify the order of extrapolation at outflow: *wdh* 100814
            boundaryConditionData(0,side,axis,grid)=parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
	    
            boundaryConditions(side,axis,grid)=OgesParameters::extrapolate; 
	  }
	  break;

        case Parameters::freeSurfaceBoundaryCondition:
	  // do this for now: *FIX ME*
          boundaryConditions(side,axis,grid)=OgesParameters::neumann;
	  break;

	default:
	  boundaryConditions(side,axis,grid)=mg.boundaryCondition(side,axis);
	  if( mg.boundaryCondition(side,axis) > 0 )
	  {
	    printF("Cgins::setOgesBoundaryConditions:ERROR unknown BC value for velocities! \n"
	           "cg0[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i\n",grid,side,axis,
		   mg.boundaryCondition(side,axis));
	    Overture::abort("Cgins::setOgesBoundaryConditions:ERROR unknown BC value for velocities");
	  }
	}
      }
    }
    else
    {

      // =========================================
      // ==== Assign BC's for the Temperature ====
      // =========================================

	if( debug() & 4 )
	  printF(" ***Cgins::setOgesBC: Set Oges BC's for the Temperature equation.\n");

      ForBoundary( side,axis )
      {
	int bc = mg.boundaryCondition(side,axis);
	if( interfaceType(side,axis,grid)!=Parameters::noInterface )
	{
          // This face is a domain interface 

          // We only support interfaces on no-slip walls:
          assert( bc == InsParameters::noSlipWall );

          // When solving the interface equations by iteration, the BC on this face may be 
          // dirichlet or mixed: 
	  bc = interfaceCondition(side,axis,grid);
	  assert( bc==Parameters::dirichletInterface || bc==Parameters::neumannInterface );
	  printP("** Cgins:setOgesBC: setting an interface bc(%i,%i,%i)=%i\n",side,axis,grid,bc);
	}

	boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  // default
      
	switch( bc )
	{
	case Parameters::slipWall:
	case Parameters::noSlipWall:
	case InsParameters::inflowWithVelocityGiven:
	case Parameters::dirichletBoundaryCondition:
	case InsParameters::inflowWithPressureAndTangentialVelocityGiven:
	{
          if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
	  {
	    boundaryConditions(side,axis,grid)=OgesParameters::dirichlet; 
	  }
	  else
	  {
            if( mixedCoeff(tc,side,axis,grid)==0. )
	    {
	      // Neumann BC
	      boundaryConditions(side,axis,grid)=OgesParameters::neumann;  

	      if( debug() & 4 ) printP("Cgins::setOgesBC: imp=%i set neumann BC for T, (side,axis,grid)=(%i,%i,%i)\n",
				       imp, side,axis,grid);
	    }
	    else
	    {
	      // Mixed BC *wdh* 110202
	      boundaryConditions(side,axis,grid)=OgesParameters::mixed;  
	      real a0 = mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
	      
	      boundaryConditionData(0,side,axis,grid)=a0;
	      boundaryConditionData(1,side,axis,grid)=a1;
	      if( debug() & 2 )
		printP("*****Cgins:setOgesBC: imp=%i, set mixed BC for T : %f*T + %f*T.n, "
		       "(side,axis,grid)=(%i,%i,%i) \n", imp,a0,a1,side,axis,grid);

	    }

	  }
	  break;
	}
        case Parameters::dirichletInterface:
          printP("*****Cgins:setOgesBC: Dirichlet interface BC is set\n");
	  boundaryConditions(side,axis,grid)=OgesParameters::dirichlet; 
          break;
        case Parameters::neumannInterface:
	{
	  boundaryConditions(side,axis,grid)=OgesParameters::neumann;
	  real a0 = mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
          if( a0!=0. || a1!=1. )
	  {
	    boundaryConditions(side,axis,grid)=OgesParameters::mixed; 
	    boundaryConditionData(0,side,axis,grid)=a0;
	    boundaryConditionData(1,side,axis,grid)=a1;
	    if( debug() & 2 )
	      printP("*****Cgins:setOgesBC: Set a mixed interface BC: %f*T + %f*T.n \n",a0,a1);

	  }
          else
	  {
            if( debug() & 2 )
              printP("*****Cgins:setOgesBC: Neumann interface BC is : %f*T + %f*T.n \n",a0,a1);
	  }
	  
          break;
	}
	case Parameters::symmetry:
	case Parameters::axisymmetric:
	case InsParameters::tractionFree:
	  boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
          break;
	case InsParameters::outflow:
	case InsParameters::convectiveOutflow:
	  boundaryConditions(side,axis,grid)=OgesParameters::neumann; // leave this for now, could be extrapolate
	  break;
	default:
	  boundaryConditions(side,axis,grid)=mg.boundaryCondition(side,axis);
	  if( mg.boundaryCondition(side,axis) > 0 )
	  {
	    printF("Cgins::setOgesBoundaryConditions:ERROR unknown BC value for T! \n"
	           "cg0[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i\n",grid,side,axis,
		   mg.boundaryCondition(side,axis));
	    Overture::abort("Cgins::setOgesBoundaryConditions:ERROR unknown BC value for T");
	  }
	}
      }

    }
    
  }

  return 0;
}
