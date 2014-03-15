#include "Cgcns.h"
#include "CnsParameters.h"
#include "MappedGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "ArraySimple.h"
#include "SparseRep.h"
#include "Integrate.h"
#include "Oges.h"
#include "App.h"

#define ICNSCF EXTERN_C_NAME(icnscf)
#define CNSNOSLIPWALLBCCOEFF EXTERN_C_NAME(cnsnoslipwallbccoeff)
#define INOUTFLOWCOEFF EXTERN_C_NAME(inoutflowcoeff)
#define ICNSWALLBCCOEFF EXTERN_C_NAME(icnswallbc)
extern "C" {
  void ICNSCF(const int *igdim, const int *igint, 
	      const real *vertex, 
	      const real *rx,
	      const real * det,
	      const int *mask,
	      const int *iparam,
	      const real *param, 
	      const real *uL, 
	      real *coeff); // output: the coefficients

  void CNSNOSLIPWALLBCCOEFF(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
			    const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
			    const real*coeff, const real *rhs,
			    const real*ul, const real*x, const real *aj, const real*rsxy,
			    const int*ipar, const real*rpar, const int*indexRange, const int*bc, const real*bd, const int*bt, int&nbv, const int&cfrhs);
  void INOUTFLOWCOEFF(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
		      const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
		      const real*coeff, const real *rhs,
		      const real*ul, const real*x, const real *aj, const real*rsxy,
		      const int*ipar, const real*rpar, const int*indexRange, const int*bc, const real*bd, const int*bt, int&nbd, const int&cfrhs);


  void ICNSWALLBCCOEFF(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
		      const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
		      const real*coeff, const real *rhs,
		      const real*ul, const real*x, const real *aj, const real*rsxy,
		      const int*ipar, const real*rpar, const int*indexRange, const int*bc, const real*ubvd, const real*bd, const int*bt, 
		       int&nbd, int &nbv, const int&cfrhs);
  bool oldbc =false;

}

// getLocalBoundsAndBoundaryConditions lives in cnsBC.C
// extern void
// getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                      IntegerArray & gidLocal, 
//                                      IntegerArray & dimensionLocal, 
//                                      IntegerArray & bcLocal );


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{implicitSolve}} 
// ===================================================================================================================
/// \brief Form the matrix for implicit time stepping.
/// \param dt0 (input) : time step used to build the implicit matrix.
/// \param cgf1 (input) : holds the RHS 
/// \param cgf0 (input) : holds the current state of the solution (used for linearization)
///
/// \note This function was once part of implicitSolve.  It was
/// broken out to allow the construction of the matrix independently of
/// the actual solve.  Basically all the work is done to initialize the
/// implicit time stepping.  The implicit method can be optionally used
/// on only some grids. To implement this approach we simply create a
/// sparse matrix that is just the identity matrix on grids that are
/// advanced explicitly but equal to the standard implicit matrix on
/// grids that are advance implicitly: 
///  \verbatim 
///  I - \nu \alpha \dt \Delta on implicit grids 
///  I on explicit grids 
///  \endverbatim 
/// If the form of the boundary conditions for the different components of
/// u are the same then we can build a single scalar matrix that
/// can be used to advance each component, one after the other. If the
/// boundary conditions are not of the same form then we build a matrix
/// for a system of equations for the velocity components (u,v,w).
///
/// Note that originally cgf1 from implicitSolve was used to get the time,
/// grid, and operators.  We are now using whatever is passed in as "u" to
/// this function.  The operators should be the same (?) and the time is
/// used in the debug output.  What about the grid though? It can change 
/// due to AMR (used with implicit?) as well as from the grid velocity.
///
// ===================================================================================================================
void Cgcns::
formMatrixForImplicitSolve(const real & dt0,
			   GridFunction & cgf1,
			   GridFunction & cgf0 )
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
    if( debug() & 4 )
       printF(" ***Cgcns:: initialize implicit time stepping for viscous terms, t=%9.3e dt0=%8.2e ***** \n",cgf1.t,dt0);

    CompositeGrid & cg = cgf1.cg;


    CompositeGridOperators & op =  *cgf1.u.getOperators();

    // *** OLD way : do not use predefined equations

    IntegerArray components;
    parameters.getComponents(components);
    int nc = scalarSystemForImplicitTimeStepping ? 1 : parameters.dbase.get<int>("numberOfComponents");//components.getLength(0);
    const int numberOfComponents = nc;
    
      // make a grid function to hold the coefficients
    Range all;
    // *wdh* 060929 int stencilWidth = 2*parameters.dbase.get<int >("orderOfAccuracy") + 1;
    int stencilWidth = parameters.dbase.get<int >("orderOfAccuracy") + 1;
    const int numberOfGhostLines=parameters.numberOfGhostPointsNeeded();
    if( numberOfGhostLines==2 && parameters.dbase.get<int >("orderOfAccuracy")==2 )
      stencilWidth=5;
    
    int stencilSize=int( pow(stencilWidth,cg.numberOfDimensions())+1 );   // add 1 for interpolation equations
    
    int stencilDimension=stencilSize*SQR(numberOfComponents);
    coeff.updateToMatchGrid(cg,stencilDimension,all,all,all); 
    coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponents);
    
    // We need to change the stencil size from that used by the pressure solve.
    op.setStencilSize(stencilSize);
    op.setNumberOfComponentsForCoefficients(numberOfComponents);
    coeff.setOperators(op);
    coeff = 0.0;
    // Form the implicit system on each grid.
    int grid;
    bool isSingular = isImplicitMatrixSingular( cgf0.u );
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid &mg = cg[grid];
	
	// new: 060909
	formImplicitTimeSteppingMatrix(coeff[grid],dt0,scalarSystemForImplicitTimeStepping,
				       cgf0.u[grid],grid );
	
      }
    
    //	  BoundaryConditionParameters bcp;
    //	  bcp.setCornerBoundaryCondition(BoundaryConditionParameters::symmetryCorner);
    //	  coeff.finishBoundaryConditions(bcp); //kkc 060406
    coeff.finishBoundaryConditions(); //kkc 060406
    // coeff.display("Here is coeff after finishBoundaryConditions");
    
    // implicitSolver[0].setNumberOfComponents(numberOfComponents);
    
    if ( isSingular )  //kkc 060728
      { 
	addConstraintEquation(parameters,implicitSolver[0],coeff,cgf0.u,cgf1.u,numberOfComponents);
      }
    else
      {
	implicitSolver[0].setCoefficientArray( coeff );   // supply coefficients
	implicitSolver[0].updateToMatchGrid( cg ); // kkc 060731 does this need to be called even if we have rebuilt the matrix?
      }
    
    parameters.dbase.get<int >("initializeImplicitTimeStepping")=FALSE;
  } // end initialize implicit time stepping

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;
  
}

// ===================================================================================================================
/// \brief Solve the implicit time-stepping equations.
/// \param dt0 (input) : time step used to build the implicit matrix.
/// \param cgf1 (input) : holds the RHS 
/// \param cgf0 (input) : holds the current state of the solution (used for linearization)
///
// ==================================================================================================================
void Cgcns::
implicitSolve(const real & dt0,
	      GridFunction & cgf1,
              GridFunction & cgf0)
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
	  printF(" ** implicit time stepping: component %i iterations= %i (t=%e, dt=%8.1e, step=%i, "
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



int Cgcns::
formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
			       const real & dt0, 
			       int scalarSystem, 
			       realMappedGridFunction & uL,
			       const int & grid )
{
  // kkc 060304
  // This function manages the construction of the matrix used to solve
  // the linearized compressible Navier-Stokes equation.  The linearization
  // is performed about uL.  
  //

  real t0=getCPU();
  MappedGridOperators &op = *coeff.getOperators();
  Range all;
  coeff=0.;

  // We need to compute I+dt\theta L where L is the u^{n+1} part of the linearized operator

  // // the next bunch of initialization stuff is taken from getUtCNS
  MappedGrid & mg = *(coeff.getMappedGrid());
  Index I1,I2,I3;
  getIndex(mg.extendedIndexRange(),I1,I2,I3);

  const real theta = parameters.dbase.get<real >("implicitFactor");
  const int numberOfDimensions=mg.numberOfDimensions();
  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");
  const int rc = parameters.dbase.get<int >("rc");
  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");
  const int tc = parameters.dbase.get<int >("tc");
  const bool gridIsMoving = parameters.gridIsMoving(grid);
  const int stencilSize=coeff.sparse->stencilSize;
  const int width = 2*parameters.dbase.get<int >("orderOfAccuracy")+1;
  //  const int width = parameters.dbase.get<int >("orderOfAccuracy")+1;
  const int halfWidth = (width-1)/2;

  ArraySimpleFixed<int,20,1,1,1> iparam;
  iparam[0] = numberOfDimensions;
  iparam[1] = numberOfComponents;
  iparam[2] = rc;
  iparam[3] = uc;
  iparam[4] = vc;
  iparam[5] = wc;
  iparam[6] = tc;
  iparam[7] = gridIsMoving;
  iparam[8] = parameters.isAxisymmetric();
  iparam[9] = parameters.dbase.get<bool >("axisymmetricWithSwirl");
  iparam[10] = stencilSize;
  iparam[11]= width;
  iparam[12]= halfWidth;
  iparam[15]= parameters.dbase.get<int >("debug");
  iparam[18]= parameters.dbase.get<int >("radialAxis"); 
  iparam[19]= grid;

  const real mu = parameters.dbase.get<real >("mu");
  const real gamma = parameters.dbase.get<real >("gamma");
  const real kThermal = parameters.dbase.get<real >("kThermal");
  const real Rg = parameters.dbase.get<real >("Rg");
  const real reynoldsNumber = parameters.dbase.get<real >("reynoldsNumber");
  const real prandtlNumber = parameters.dbase.get<real >("prandtlNumber");
  const real machNumber = parameters.dbase.get<real >("machNumber");
  const ArraySimpleFixed<real,3,1,1,1> &gravity=parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  ArraySimpleFixed<real,20,1,1,1> rparam;
  rparam[0]=reynoldsNumber;
  rparam[1]=prandtlNumber;
  rparam[2]=machNumber;
  rparam[3]=gamma;
  rparam[4]=parameters.dbase.get<real >("implicitFactor");
  rparam[5]=mg.gridSpacing(0);  
  rparam[6]=mg.gridSpacing(1);
  rparam[7]=mg.gridSpacing(2);
  rparam[8]=0; // not used for anything

  rparam[9]=parameters.dbase.get<real >("dt");
  rparam[12]= parameters.dbase.get<real >("av2");
  rparam[13]= parameters.dbase.get<real >("av4");
  rparam[14]= gravity[0];
  rparam[15]= gravity[1];
  rparam[16]= gravity[2];
  rparam[17] = parameters.dbase.get<real>("strickwerdaCoeff");

  const realArray & u = uL;
  //  const realArray & gridVelocity = gridVelocity_;

  mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian | MappedGrid::THEmask);

  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    //    realSerialArray utLocal;  getLocalArrayWithGhostBoundaries(ut,utLocal);
    //    const realSerialArray & gridVelocityLocal= gridVelocity.getLocalArray();

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray & maskLocal= (*pmg2).mask().getLocalArray();
    const intSerialArray & maskLocal= mg.mask().getLocalArray();

    //    utLocal=0.; // ***** do this ****

  #else
    const realSerialArray & uLocal  = u;
    //    const realSerialArray & utLocal0 = ut; 
    //    realSerialArray & utLocal = (realSerialArray &)utLocal0;
    //    const realSerialArray & gridVelocityLocal = gridVelocity;

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray  & maskLocal = (*pmg2).mask(); 
    const intSerialArray  & maskLocal = mg.mask(); 

  #endif
  const int *pmask = maskLocal.getDataPointer();

  const int nGhost=2;
  const IntegerArray & gid = mg.gridIndexRange();
  const IntegerArray & indexRange = mg.indexRange();
  
  IntegerArray d(2,3),nr(2,3); 
  d = nr = 0;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    d(0,axis)=uLocal.getBase(axis);
    d(1,axis)=uLocal.getBound(axis);
    
    nr(0,axis)=max(d(0,axis)+nGhost,gid(0,axis));
    nr(1,axis)=min(d(1,axis)-nGhost,gid(1,axis));
    
  }

#ifdef USE_PPP
  const realSerialArray & vertex = mg.vertex().getLocalArray();
  const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
  const realSerialArray & det = mg.centerJacobian().getLocalArray();
#else
  const realSerialArray & vertex = mg.vertex();
  const realSerialArray & rx = mg.inverseCenterDerivative();
  const realSerialArray & det = mg.centerJacobian();
#endif

  // // // end local variable initialization

  // // now actually build the coefficient matrix
  ICNSCF(d.getDataPointer(),nr.getDataPointer(),vertex.getDataPointer(), rx.getDataPointer(), 
	 det.getDataPointer(), pmask,// grid info
	 iparam.ptr(),rparam.ptr(), // solver paramters
	 uL.getDataPointer(), // state to linearize about
	 coeff.getDataPointer()); // output: the coefficients

  op.setOrderOfAccuracy(4); // do this to get the identity coeffs in the right place


  if ( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::steadyStateNewton)
    {
      if ( parameters.dbase.get<real >("implicitFactor")<10*REAL_EPSILON )
	coeff *= dt0;
      else
	coeff *= parameters.dbase.get<real >("implicitFactor")*dt0;
      
      for( int n=0; n<uL.getLength(3); n++ ) 
	coeff += op.identityCoefficients(all,all,all,n,n);
    }
 
  // BOUNDARY CONDITIONS

  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
  BoundaryConditionParameters bcParams;
  const IntegerArray & bc = mg.boundaryCondition();

  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( uL,gidLocal,dimLocal,bcLocal ); 
  //  getTimeDependentBoundaryConditions( t,grid ); 

  BCTypes::BCNames 
    dirichlet             = BCTypes::dirichlet,
    neumann               = BCTypes::neumann,
    mixed                 = BCTypes::mixed,
    vectorSymmetry        = BCTypes::vectorSymmetry,
    extrapolate           = BCTypes::extrapolate,
    normalComponent       = BCTypes::normalComponent,
    evenSymmetry          = BCTypes::evenSymmetry;
  //  typedef Parameters::BoundaryCondition BoundaryCondition;
  typedef int BoundaryCondition;
  const BoundaryCondition & noSlipWall                = Parameters::noSlipWall;
  const BoundaryCondition & slipWall                  = Parameters::slipWall;
  const BoundaryCondition & superSonicOutflow         = CnsParameters::superSonicOutflow;
  const BoundaryCondition & superSonicInflow          = CnsParameters::superSonicInflow;
  const BoundaryCondition & subSonicOutflow           = CnsParameters::subSonicOutflow;
  const BoundaryCondition & subSonicInflow            = CnsParameters::subSonicInflow;
  const BoundaryCondition & symmetry                  = Parameters::symmetry;
  const BoundaryCondition & inflowWithVelocityGiven   = CnsParameters::inflowWithVelocityGiven;
  const BoundaryCondition & outflow                   = CnsParameters::outflow;
  const BoundaryCondition & dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
  const BoundaryCondition & neumannBoundaryCondition  = Parameters::neumannBoundaryCondition;
  const BoundaryCondition & axisymmetric              = Parameters::axisymmetric;
  const BoundaryCondition & farField                  = CnsParameters::farField;

  //  bcParams.ghostLineToAssign=0;
  // dirichlet conditions
  {
      for( int line=0; line<=2; line++ )
      {
	bcParams.lineToAssign=line;
	//	bcParams.ghostLineToAssign = line;
	for( int n=0; n<uL.getLength(3); n++ )
	  coeff.applyBoundaryConditionCoefficients(n,n,dirichlet,dirichletBoundaryCondition,bcParams);
      }
    bcParams.lineToAssign=0; // reset
    bcParams.extraInTangentialDirections=0;
  }

  // // fill in equations for no-slip wall density and velocity boundary conditions
  int cfrhs = 0; // fill in coeff 

  RealArray &ubv = parameters.dbase.get<RealArray >("userBoundaryConditionParameters");
  IntegerArray ubt;
  ubt = parameters.dbase.get<IntegerArray >("bcInfo") - Parameters::numberOfPredefinedBoundaryConditionTypes;
  int nbv = ubv.getLength(0);
  RealArray &ubd = parameters.dbase.get<RealArray >("bcData");
//  ubd.display("UBD");
  //  ubv.display("UBV");
  //  ubt.display("UBT");
  int nbd = ubd.getLength(0);
  //  assert(nbv==nbd);
  if ( !oldbc ) 
    {
      ICNSWALLBCCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
			uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
			coeff.getDataPointer(), uL.getDataPointer(),// uL is just a dummy here
			uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
			iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), 
			ubv.getDataPointer(), 
			ubd.getDataPointer(), 
			ubt.getDataPointer(),
			nbd,nbv,cfrhs);
    }
  else
    {
      INOUTFLOWCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
		       uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
		       coeff.getDataPointer(), uL.getDataPointer(),// uL is just a dummy here
		       uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
		       iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), ubd.getDataPointer(), 
		       ubt.getDataPointer(),
		       nbd,cfrhs);

      CNSNOSLIPWALLBCCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
			     uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
			     coeff.getDataPointer(), uL.getDataPointer(),// uL is just a dummy here
			     uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
			     iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), ubv.getDataPointer(), 
			     ubt.getDataPointer(),
			     nbv,cfrhs);
    }


  // set the classify array in the sparse rep to "used" for ghost points that are set by cnsnoslipwallbccoeff or inoutflowcoeff
  for ( int a=0; a<numberOfDimensions; a++ )
    for ( int s=0; s<2; s++ )
      {
	if ( mg.boundaryCondition()(s,a)==CnsParameters::noSlipWall || mg.boundaryCondition()(s,a)==slipWall ||
	     mg.boundaryCondition()(s,a)==CnsParameters::subSonicInflow || mg.boundaryCondition()(s,a)==CnsParameters::subSonicOutflow )
	  {
	    Index I1g,I2g,I3g;
	    getGhostIndex(uL,s,a,I1g,I2g,I3g);
	    coeff.sparse->setClassify(SparseRepForMGF::ghost1,I1g,I2g,I3g,Range(rc,tc));
	    //	    getGhostIndex(uL,s,a,I1g,I2g,I3g,0);
	    //	    coeff.sparse->setClassify(SparseRepForMGF::extrapolation,I1g.getBase(),I2g.getBase(),I3g.getBase(),Range(rc,rc));
	    //	    coeff.sparse->setClassify(SparseRepForMGF::extrapolation,I1g.getBound(),I2g.getBase(),I3g.getBase(),Range(rc,rc));
	    //	    getGhostIndex(uL,s,a,I1g,I2g,I3g,2);
	    //	    coeff.sparse->setClassify(SparseRepForMGF::ghost2,I1g,I2g,I3g,Range(uc,vc));
	  }
      }

  //  op.setOrderOfAccuracy(2);
  bcParams.orderOfExtrapolation=2;
  bcParams.lineToAssign=1;
  //  if (parameters.dbase.get<bool >("axisymmetricWithSwirl"))
  //    coeff.applyBoundaryConditionCoefficients(wc,wc,dirichlet,noSlipWall,bcParams);

  // 
  // symmetry boundary conditions
  //
  bcParams.lineToAssign=0;
  coeff.applyBoundaryConditionCoefficients(tc,tc,neumann,symmetry,bcParams);

  Range V(uc,uc+numberOfDimensions-1);

  Range C(0,parameters.dbase.get<int >("numberOfComponents")-1);  // ***** is this correct ******
  coeff.applyBoundaryConditionCoefficients(V,V,vectorSymmetry,symmetry);
  //  coeff.applyBoundaryConditionCoefficients(V,V,vectorSymmetry,slipWall);
  coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,symmetry);

  coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,symmetry);
    
  if (parameters.dbase.get<bool >("axisymmetricWithSwirl"))
    coeff.applyBoundaryConditionCoefficients(wc,wc,neumann,symmetry);

#if 0
  // 
  // inflow boundary conditions, dirichlet on u,v; dirchlet on either rho or T and neumann on either rho or T
  //
  Range VW(uc, uc+numberOfDimensions-1);
  for ( int c=VW.getBase(); c<=VW.getBound(); c++ )
    {
      coeff.applyBoundaryConditionCoefficients(c,c,dirichlet,subSonicInflow);
      coeff.applyBoundaryConditionCoefficients(c,c,neumann,subSonicInflow);
    }

  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      coeff.applyBoundaryConditionCoefficients(wc,wc,dirichlet,subSonicInflow);
      coeff.applyBoundaryConditionCoefficients(wc,wc,extrapolate,subSonicInflow);
    }
      


  int nb=BCTypes::boundary1;
  bcParams.lineToAssign=0;
  bool hasOutflow = false;
  for ( int a=0; a<numberOfDimensions; a++ )
    for ( int s=0; s<2; s++ )
      {
	if ( mg.boundaryCondition()(s,a)==CnsParameters::subSonicInflow )
	  {
	    if ( parameters.dbase.get<RealArray>("bcData")(rc,s,a,grid)>0. )
	      {
		coeff.applyBoundaryConditionCoefficients(rc,rc,dirichlet,nb);
		coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,nb);
		coeff.applyBoundaryConditionCoefficients(tc,tc,neumann,nb);
	      }
	    else
	      {
		coeff.applyBoundaryConditionCoefficients(tc,tc,dirichlet,nb);
		coeff.applyBoundaryConditionCoefficients(tc,tc,neumann,nb);
		coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,nb);
	      }
	  }
	hasOutflow = hasOutflow || mg.boundaryCondition()(s,a)==CnsParameters::subSonicOutflow;
	nb++;
      }
  bcParams.lineToAssign=0;
  //
  // outflow conditions, mixed on T(input parsing only understands that and pressure), extrapolate on everything else??
  //
  bcParams.orderOfExtrapolation=1;
  for ( int c=VW.getBase(); c<=VW.getBound(); c++ )
    coeff.applyBoundaryConditionCoefficients(c,c,extrapolate,subSonicOutflow);
  nb=BCTypes::boundary1;
  // fill in the coefficients for the mixed derivative BC:
  bcParams.a.redim(3,2,bcData.getLength(2),1/*grid+1*/); bcParams.a=0.;

  for( int i=0; hasOutflow && i<=1; i++ )
    bcParams.a(i,all,all,0/*grid*/)=bcData(tc+parameters.dbase.get<int >("numberOfComponents")*(1+i),all,all,grid);

  coeff.applyBoundaryConditionCoefficients(tc,tc,mixed,subSonicOutflow,bcParams);
  coeff.applyBoundaryConditionCoefficients(rc,rc,neumann,subSonicOutflow);
#endif

//   for ( int a=0; a<numberOfDimensions; a++ )
//     for ( int s=0; s<2; s++ )
//       {
// 	if ( mg.boundaryCondition()(s,a)==Parameters::subSonicOutflow )
// 	  {
// 	    // !!! it would be better to have a pressure condition, but that would require linearlization
// 	    //     and a new fortran function... work for later...
// 	  }
// 	nb++;
//       }
  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      //      coeff.applyBoundaryConditionCoefficients(wc,wc,dirichlet,subSonicOutflow);
      //      coeff.applyBoundaryConditionCoefficients(wc,wc,extrapolate,subSonicOutflow);
      //      coeff.applyBoundaryConditionCoefficients(wc,wc,extrapolate,subSonicOutflow);
      
    }

  //
  // extrapolate second ghost line
  //
  bcParams.ghostLineToAssign=2;
  //  bcParams.lineToAssign=2;
  bcParams.orderOfExtrapolation=3; 
  for( int n=0; n<uL.getLength(3); n++ )
    coeff.applyBoundaryConditionCoefficients(n,n,extrapolate,BCTypes::allBoundaries,bcParams);

  //  bcParams.orderOfExtrapolation=3; 
  //for( int n=uc; n<=vc; n++ )
  //  coeff.applyBoundaryConditionCoefficients(n,n,extrapolate,BCTypes::allBoundaries,bcParams);
    //  bcParams.orderOfExtrapolation=3; 
  //  bcParams.ghostLineToAssign=1;
  //  bcParams.orderOfExtrapolation=2; 
  //  coeff.applyBoundaryConditionCoefficients(rc,rc,extrapolate,BCTypes::allBoundaries,bcParams);


        bcParams.ghostLineToAssign=1;
        bcParams.orderOfExtrapolation=3; 
	//	for( int n=0; n<uL.getLength(3); n++ )
	//		coeff.applyBoundaryConditionCoefficients(rc,rc,extrapolate,BCTypes::allBoundaries,bcParams);

//        //    coeff.applyBoundaryConditionCoefficients(rc,rc,extrapolate,BCTypes::allBoundaries,bcParams);
//        coeff.applyBoundaryConditionCoefficients(uc,uc,extrapolate,noSlipWall,bcParams);
//        coeff.applyBoundaryConditionCoefficients(vc,vc,extrapolate,noSlipWall,bcParams);
//        bcParams.ghostLineToAssign=2;
//        coeff.applyBoundaryConditionCoefficients(uc,uc,extrapolate,noSlipWall,bcParams);
//        coeff.applyBoundaryConditionCoefficients(vc,vc,extrapolate,noSlipWall,bcParams);


  op.setOrderOfAccuracy(2);
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+= getCPU()-t0;
  return 0;
}

// ===================================================================================================================
/// \brief Fill in the boundary conditions for the right-hand-side of the implicit time-stepping equations.
/// \param rhs (input/output) : assign right-hand-side values here
/// \param uL (input) : holds the linearized solution.
/// \param gridVelocity (input) : grid velocity
/// \param t (input) : time
/// \param scalarSystem (input) :
/// \param grid (input) : grid number.
///
// ==================================================================================================================
int Cgcns::
applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & rhs, 
					       realMappedGridFunction & uL,
					       realMappedGridFunction & gridVelocity,
					       real t,
					       int scalarSystem,
					       int grid )
{
  MappedGrid & mg = *(rhs.getMappedGrid());
  Index I1,I2,I3;
  getIndex(mg.extendedIndexRange(),I1,I2,I3);

  const real theta = parameters.dbase.get<real >("implicitFactor");
  const int numberOfDimensions=mg.numberOfDimensions();
  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");
  const int rc = parameters.dbase.get<int >("rc");
  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");
  const int tc = parameters.dbase.get<int >("tc");
  const bool gridIsMoving = parameters.gridIsMoving(grid);

  ArraySimpleFixed<int,20,1,1,1> iparam;
  iparam[0] = numberOfDimensions;
  iparam[1] = numberOfComponents;
  iparam[2] = rc;
  iparam[3] = uc;
  iparam[4] = vc;
  iparam[5] = max(wc,vc);
  iparam[6] = tc;
  iparam[7] = gridIsMoving;
  iparam[8] = parameters.isAxisymmetric();
  iparam[9] = parameters.dbase.get<bool >("axisymmetricWithSwirl");
  iparam[15]= parameters.dbase.get<int >("debug");
  iparam[18]= parameters.dbase.get<int >("radialAxis"); 
  iparam[19]= grid;

  const real mu = parameters.dbase.get<real >("mu");
  const real gamma = parameters.dbase.get<real >("gamma");
  const real kThermal = parameters.dbase.get<real >("kThermal");
  const real Rg = parameters.dbase.get<real >("Rg");
  const real reynoldsNumber = parameters.dbase.get<real >("reynoldsNumber");
  const real prandtlNumber = parameters.dbase.get<real >("prandtlNumber");
  const real machNumber = parameters.dbase.get<real >("machNumber");
  const ArraySimpleFixed<real,3,1,1,1> &gravity=parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  ArraySimpleFixed<real,20,1,1,1> rparam;
  rparam[0]=reynoldsNumber;
  rparam[1]=prandtlNumber;
  rparam[2]=machNumber;
  rparam[3]=gamma;
  rparam[4]=parameters.dbase.get<real >("implicitFactor");
  rparam[5]=mg.gridSpacing(0);  
  rparam[6]=mg.gridSpacing(1);
  rparam[7]=mg.gridSpacing(2);
  rparam[8]=0; // not used for anything
  rparam[9]=parameters.dbase.get<real >("dt");

  rparam[14]= gravity[0];
  rparam[15]= gravity[1];
  rparam[16]= gravity[2];
  rparam[17] = parameters.dbase.get<real>("strickwerdaCoeff");

  const realArray & u = uL;
  //  const realArray & gridVelocity = gridVelocity_;

  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    //    realSerialArray utLocal;  getLocalArrayWithGhostBoundaries(ut,utLocal);
    //    const realSerialArray & gridVelocityLocal= gridVelocity.getLocalArray();

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray & maskLocal= (*pmg2).mask().getLocalArray();
    const intSerialArray & maskLocal= mg.mask().getLocalArray();

    //    utLocal=0.; // ***** do this ****

  #else
    const realSerialArray & uLocal  = u;
    //    const realSerialArray & utLocal0 = ut; 
    //    realSerialArray & utLocal = (realSerialArray &)utLocal0;
    //    const realSerialArray & gridVelocityLocal = gridVelocity;

    // For moving grids make sure we use the mask from the new grid locations:
    //    const intSerialArray  & maskLocal = (*pmg2).mask(); 
    const intSerialArray  & maskLocal = mg.mask(); 

  #endif
  const int *pmask = maskLocal.getDataPointer();

  const int nGhost=2;
  const IntegerArray & gid = mg.gridIndexRange();
  const IntegerArray & indexRange = mg.indexRange();
  
  IntegerArray d(2,3),nr(2,3); 
  d = nr = 0;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    d(0,axis)=uLocal.getBase(axis);
    d(1,axis)=uLocal.getBound(axis);
    
    nr(0,axis)=max(d(0,axis)+nGhost,gid(0,axis));
    nr(1,axis)=min(d(1,axis)-nGhost,gid(1,axis));
    
  }

  mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
#ifdef USE_PPP
  const realSerialArray & vertex = mg.vertex().getLocalArray();
  const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
  const realSerialArray & det = mg.centerJacobian().getLocalArray();
#else
  const realSerialArray & vertex = mg.vertex();
  const realSerialArray & rx = mg.inverseCenterDerivative();
  const realSerialArray & det = mg.centerJacobian();
#endif

  // // // end local variable initialization
  // BOUNDARY CONDITIONS
  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
  BoundaryConditionParameters bcParams;
  const IntegerArray & bc = mg.boundaryCondition();
  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( uL,gidLocal,dimLocal,bcLocal ); 
  // determine time dependent conditions:
  getTimeDependentBoundaryConditions( mg,t,grid ); 

  BCTypes::BCNames 
    dirichlet             = BCTypes::dirichlet,
    neumann               = BCTypes::neumann,
    mixed                 = BCTypes::mixed,
    vectorSymmetry        = BCTypes::vectorSymmetry,
    extrapolate           = BCTypes::extrapolate,
    normalComponent       = BCTypes::normalComponent,
    evenSymmetry          = BCTypes::evenSymmetry;
  //  typedef Parameters::BoundaryCondition BoundaryCondition;
  typedef int BoundaryCondition;
  const BoundaryCondition & noSlipWall                = Parameters::noSlipWall;
  const BoundaryCondition & slipWall                  = Parameters::slipWall;
  const BoundaryCondition & superSonicOutflow         = CnsParameters::superSonicOutflow;
  const BoundaryCondition & superSonicInflow          = CnsParameters::superSonicInflow;
  const BoundaryCondition & subSonicOutflow           = CnsParameters::subSonicOutflow;
  const BoundaryCondition & subSonicInflow            = CnsParameters::subSonicInflow;
  const BoundaryCondition & symmetry                  = Parameters::symmetry;
  const BoundaryCondition & inflowWithVelocityGiven   = CnsParameters::inflowWithVelocityGiven;
  const BoundaryCondition & outflow                   = CnsParameters::outflow;
  const BoundaryCondition & dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
  const BoundaryCondition & neumannBoundaryCondition  = Parameters::neumannBoundaryCondition;
  const BoundaryCondition & axisymmetric              = Parameters::axisymmetric;
  const BoundaryCondition & farField                  = CnsParameters::farField;

  int cfrhs = 1; // fill in rhs
  //  bcParams.ghostLineToAssign = 1;
  //  bcParams.lineToAssign = 1;
  //  rhs.applyBoundaryCondition(tc,dirichlet,noSlipWall,bcData,pBoundaryData,t,Overture::defaultBoundaryConditionParameters(),grid);

  RealArray &ubv = parameters.dbase.get<RealArray >("userBoundaryConditionParameters");
  IntegerArray ubt;
  ubt = parameters.dbase.get<IntegerArray >("bcInfo") - Parameters::numberOfPredefinedBoundaryConditionTypes;
  int nbv = ubv.getLength(0);
  RealArray &ubd = parameters.dbase.get<RealArray >("bcData");
  int nbd = ubd.getLength(0);

  //  ubd.display("bcData");
  if ( oldbc ) 
    {
      INOUTFLOWCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
		       uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
		       uL.getDataPointer(), rhs.getDataPointer(),
		       uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
		       iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), ubd.getDataPointer(), 
		       ubt.getDataPointer(),
		       nbd,cfrhs);
      
      CNSNOSLIPWALLBCCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
			     uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
			     /* uL is just a dummy here*/ uL.getDataPointer(), rhs.getDataPointer(),
			     uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
			     iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), ubv.getDataPointer(), ubt.getDataPointer(),
			     nbv,
			     cfrhs);
    }
  else
    {
      ICNSWALLBCCOEFF(  numberOfDimensions, uL.getBase(0),uL.getBound(0),uL.getBase(1),uL.getBound(1),
			uL.getBase(2),uL.getBound(2),uL.getBase(3),uL.getBound(3),
			uL.getDataPointer(), rhs.getDataPointer(),
			uL.getDataPointer(), vertex.getDataPointer(), det.getDataPointer(), rx.getDataPointer(),
			iparam.ptr(), rparam.ptr(), gidLocal.getDataPointer(),bcLocal.getDataPointer(), 
			ubv.getDataPointer(), 
			ubd.getDataPointer(), 
			ubt.getDataPointer(),
			nbd,nbv,cfrhs);
    }


  //  Range Vc(uc,max(wc,vc));
  Range C(uc,vc);
  Range V(rc,tc);
  Range RR(rc,rc);
  Range all;
  //  bcParams.ghostLineToAssign = 1;
  bcParams.lineToAssign = 1;
  rhs.applyBoundaryCondition(V,dirichlet,symmetry,0,t,bcParams);

  // 
  // inflow boundary conditions, dirichlet on u,v; dirchlet on either rho or T and mixed on either rho or T
  //

  BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);

#if 0
  Range VW(uc, uc+numberOfDimensions-1);
    rhs.applyBoundaryCondition(VW,dirichlet,subSonicInflow, bcData, pBoundaryData,t,
  			     Overture::defaultBoundaryConditionParameters(),grid);
  bcParams.lineToAssign = 0;
  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      rhs.applyBoundaryCondition(wc,dirichlet,subSonicInflow,bcData, pBoundaryData,t,
				 Overture::defaultBoundaryConditionParameters(),grid);//0,t,bcParams);
      bcParams.lineToAssign = 1;
      rhs.applyBoundaryCondition(wc,dirichlet,subSonicInflow,0,t,bcParams);
    }

  
  int nb=BCTypes::boundary1;
  for ( int a=0; a<numberOfDimensions; a++ )
    for ( int s=0; s<2; s++ )
      {
	if ( mg.boundaryCondition()(s,a)==CnsParameters::subSonicInflow )
	  {
	    if ( parameters.dbase.get<RealArray>("bcData")(rc,s,a,grid)>0. )
	      {
		bcParams.lineToAssign=0;
		rhs.applyBoundaryCondition(rc,dirichlet, subSonicInflow, bcData, pBoundaryData,t,
					   Overture::defaultBoundaryConditionParameters(),grid);
		
		bcParams.lineToAssign=1;
		rhs.applyBoundaryCondition(tc,dirichlet,subSonicInflow,0,t,bcParams);
		rhs.applyBoundaryCondition(rc,dirichlet,subSonicInflow,0,t,bcParams);

		
		//		coeff.applyBoundaryConditionCoefficients(tc,tc,mixed,nb,bcParams);
	      }
	    else
	      {
		bcParams.lineToAssign=0;
		rhs.applyBoundaryCondition(tc,dirichlet, subSonicInflow, bcData, pBoundaryData,t,
					   Overture::defaultBoundaryConditionParameters(),grid);
		bcParams.lineToAssign=1;
		rhs.applyBoundaryCondition(rc,dirichlet,subSonicInflow,0,t,bcParams);
		rhs.applyBoundaryCondition(tc,dirichlet,subSonicInflow,0,t,bcParams);
	      }
	  }
	nb++;
      }
  bcParams.lineToAssign = 0;

  //
  // outflow conditions, mixed on either rho or T, neumann on everything else??
  //
  bcParams.lineToAssign=1;
  rhs.applyBoundaryCondition(VW,dirichlet,subSonicOutflow,0.,t,bcParams);
  nb=BCTypes::boundary1;
  for ( int a=0; a<numberOfDimensions; a++ )
    for ( int s=0; s<2; s++ )
      {
	if ( mg.boundaryCondition()(s,a)==CnsParameters::subSonicOutflow )
	  {
	    rhs.applyBoundaryCondition(tc,dirichlet,nb,bcData(tc,s,a,grid),t,bcParams);
	    rhs.applyBoundaryCondition(rc,dirichlet,nb,0.,t,bcParams);
	  }
	nb++;
      }
  bcParams.lineToAssign = 0;
  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
    {
      //      rhs.applyBoundaryCondition(wc,dirichlet,subSonicOutflow,0.,t);
      //      rhs.applyBoundaryCondition(wc,dirichlet,subSonicOutflow,bcData, pBoundaryData,t,
      //				 Overture::defaultBoundaryConditionParameters(),grid);//0,t,bcParams);
      bcParams.lineToAssign = 1;
      rhs.applyBoundaryCondition(wc,dirichlet,subSonicOutflow,0.,t,bcParams);
    }

#endif 

  bcParams.lineToAssign = 2;
  //  bcParams.ghostLineToAssign = 2;
  if ( false ) {
  rhs.applyBoundaryCondition(V,extrapolate,BCTypes::allBoundaries,0,t,bcParams);
  }
  // dirichlet conditions
  {
    //    bcParams.extraInTangentialDirections=2;  // *wdh* 050611 -- assign extended boundary
    Range C(rc,tc);
    for( int line=0; line<=2; line++ )
      {
	bcParams.lineToAssign=line;
	rhs.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,0.,t,bcParams);
      }
    bcParams.lineToAssign=0; // reset
    bcParams.extraInTangentialDirections=0;
  }
#if 0
  for ( int s=0; s<2; s++ )
    for ( int a=0; a<2; a++ )
      {
	Index I1,I2,I3;
	getGhostIndex(mg.indexRange(),s,a,I1,I2,I3);
	Range all;
	cout<<"GRID = "<<grid<<" SIDE = "<<s<<"  AXIS = "<<a<<endl;
	rhs(I1,I2,I3,all).display("rhs");
      }
#endif
  //  bcParams.lineToAssign=0; // reset
  //  rhs.applyBoundaryCondition(rc,dirichlet,BCTypes::allBoundaries,0.,t,bcParams);
         bcParams.lineToAssign = 1;
	 //      rhs.applyBoundaryCondition(rc,dirichlet,BCTypes::allBoundaries,0,t,bcParams);
//       rhs.applyBoundaryCondition(VW,dirichlet,BCTypes::allBoundaries,0,t,bcParams);
//       bcParams.lineToAssign = 2;
//       rhs.applyBoundaryCondition(VW,dirichlet,BCTypes::allBoundaries,0,t,bcParams);
  return 0;
}

// ===================================================================================================================
/// \brief Return true if the implicit time-stepping matrix is singular.
/// \param uL (input) : holds the linearized solution.
// ==================================================================================================================
bool Cgcns::
isImplicitMatrixSingular( realCompositeGridFunction &uL )
{
  CompositeGrid &cg = *uL.getCompositeGrid();
  bool isSingular = ( parameters.dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleNavierStokes &&
		      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton);

  //return false;
      return isSingular;
  for( int grid=0; grid<cg.numberOfComponentGrids() && isSingular; grid++ )
    {
      MappedGrid &mg = cg[grid];
      
      for ( int s=0; s<2; s++ )
	for ( int a=0; a<cg.numberOfDimensions(); a++ )
	  {
	    isSingular = isSingular && ( mg.boundaryCondition(s,a)==Parameters::noSlipWall ||
					 mg.boundaryCondition(s,a)==Parameters::slipWall || 
					 mg.boundaryCondition(s,a)==Parameters::interpolation ||
					 mg.boundaryCondition(s,a)<0 );
	  }
    }
  
  return isSingular;
}

// ===================================================================================================================
/// \brief Add constraint equations to the implicit time-stepping matrix.
/// \param parameters (input) : 
/// \param solver (input) : 
/// \param coeff (input) : 
/// \param ucur (input) : 
/// \param rhs (input) : 
/// \param numberOfComponents (input) : 
// ==================================================================================================================
int Cgcns::
addConstraintEquation( Parameters &parameters, Oges& solver, 
		       realCompositeGridFunction &coeff, 
		       realCompositeGridFunction &ucur, 
		       realCompositeGridFunction &rhs, const int &numberOfComponents) 
{
  
  int isAlreadySet = false;
  solver.get(OgesParameters::THEcompatibilityConstraint,isAlreadySet); // OgesParameters does not have get/set for bools...
  realCompositeGridFunction &constraintCoeff = solver.rightNullVector;
  CompositeGrid &cg = *coeff.getCompositeGrid(true);

  real mass=0;
  real area=0;
  real momentum[]={0,0,0};
  real energy = 0;
  int rc=parameters.dbase.get<int>("rc");
  int uc=parameters.dbase.get<int>("uc");
  int vc=parameters.dbase.get<int>("vc");
  int wc=parameters.dbase.get<int>("wc");
  int tc=parameters.dbase.get<int>("tc");
  real gamma = parameters.dbase.get<real>("gamma");
  real mach = parameters.dbase.get<real>("machNumber");
  real efac = gamma/(mach*mach*(gamma-1.));

  if ( !isAlreadySet )
    {
      cout<<"Setting mass conservation constraint equation"<<endl;
      CompositeGrid &cg = *coeff.getCompositeGrid(true);
      for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
	for ( int a=0; a<cg.numberOfDimensions(); a++ )
	  cg[grid].setDiscretizationWidth(a,3);
	  
      Integrate integrator(cg);
      if ( parameters.isAxisymmetric() )
	{
	  integrator.setRadialAxis(parameters.dbase.get<int >("radialAxis"));
	}

      Range all;
      constraintCoeff.updateToMatchGrid(cg,all,all,all,numberOfComponents);
	  
      constraintCoeff = 0.0;
      for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
	for ( int a=0; a<cg.numberOfDimensions(); a++ )
	  cg[grid].setDiscretizationWidth(a,5);
	  
	  
      RealCompositeGridFunction &weights = integrator.integrationWeights();
	  
      Range Rho(parameters.dbase.get<int >("rc"),parameters.dbase.get<int >("rc"));
      
      for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
	{
	  MappedGrid &mg = cg[grid];
	  Index I1,I2,I3;
	  getIndex(mg.gridIndexRange(),I1,I2,I3);
	  realMappedGridFunction &cc_mg = constraintCoeff[grid];
	  // !!! kkc why are the weights in the ghost points not set to zero ?
	  //         even Integrate's volumeIntegral ignores the boundary values...
	  assign(cc_mg,I1,I2,I3,Rho, weights[grid],I1,I2,I3,0); 

	  mass += sum(ucur[grid](I1,I2,I3,parameters.dbase.get<int >("rc"))*cc_mg(I1,I2,I3,parameters.dbase.get<int >("rc")));
	  area += sum(cc_mg(I1,I2,I3,parameters.dbase.get<int >("rc")));
	  for ( int c=uc; c<=max(vc,wc); c++ )
	    {
	      momentum[c-uc] += sum(ucur[grid](I1,I2,I3,rc)*ucur[grid](I1,I2,I3,c)*cc_mg(I1,I2,I3,rc));
	      energy += .5*sum(ucur[grid](I1,I2,I3,rc)*ucur[grid](I1,I2,I3,c)*ucur[grid](I1,I2,I3,c)*cc_mg(I1,I2,I3,rc));
	    }
	  energy += sum(efac*ucur[grid](I1,I2,I3,rc)*ucur[grid](I1,I2,I3,tc)*cc_mg(I1,I2,I3,rc));
	  //	      ucur[grid](I1,I2,I3,parameters.dbase.get<int >("rc")).display();
	  //	      cc_mg.display("cc_mg");
	}
      solver.setCoefficientArray( coeff );   // supply coefficients
      solver.set(OgesParameters::THEcompatibilityConstraint,true);
      solver.set(OgesParameters::THEuserSuppliedCompatibilityConstraint,true);
      solver.updateToMatchGrid( cg ); // why do we need this? it will call initialize...
      //      solver.initialize();
    }
  else
    {
      solver.setCoefficientArray( coeff );   // supply coefficients
      solver.updateToMatchGrid( cg ); // why do we need this? it will call initialize...
      for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
	{
	  MappedGrid &mg = cg[grid];
	  Index I1,I2,I3;
	  getIndex(mg.gridIndexRange(),I1,I2,I3);
	  realMappedGridFunction &cc_mg = constraintCoeff[grid];
	  mass += sum(ucur[grid](I1,I2,I3,parameters.dbase.get<int >("rc"))*cc_mg(I1,I2,I3,parameters.dbase.get<int >("rc")));
	  for ( int c=uc; c<=max(vc,wc); c++ )
	    {
	      momentum[c-uc] += sum(ucur[grid](I1,I2,I3,rc)*ucur[grid](I1,I2,I3,c)*cc_mg(I1,I2,I3,rc));
	      energy += .5*sum(ucur[grid](I1,I2,I3,rc)*ucur[grid](I1,I2,I3,c)*ucur[grid](I1,I2,I3,c)*cc_mg(I1,I2,I3,rc));
	    }
	  energy += sum(efac*ucur[grid](I1,I2,I3,rc)*ucur[grid](I1,I2,I3,tc)*cc_mg(I1,I2,I3,rc));
	  area += sum(cc_mg(I1,I2,I3,parameters.dbase.get<int >("rc")));
	}
    }

  int ne,i1e,i2e,i3e,gride;
  solver.equationToIndex( solver.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
  //  cout<<i1e<<"  "<<i2e<<" "<<ne<<endl;

  rhs[gride](i1e,i2e,i3e,ne)=mass;

  if ( parameters.isAxisymmetric() )
    {
      mass*=2.*M_PI;
      for ( int a=0; a<3; a++ )
	momentum[a] *= 2.*M_PI;
      energy *= 2.*M_PI;
      area *= 2.*M_PI;
    }

  cout<<"MASS = "<<mass<<endl;
  cout<<"MOM  = "<<momentum[0]<<"  "<<momentum[1]<<"  "<<momentum[2]<<endl;
  cout<<"ENG  = "<<energy<<endl;
  cout<<"Volume = "<<area<<endl;


  return 0;

}

// ===================================================================================================================
/// \brief Allocate the appropriate number of implicit solvers (Oges objects)
/// \param cg (input) : 
// ==================================================================================================================
void Cgcns::
buildImplicitSolvers(CompositeGrid &cg)
{
  real cpu0=getCPU();
  int numberOfImplicitSolversNeeded=1;
  
  int grid, n;
  int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");
  
  // *******************************************
  // **** Initialize Implicit Time Stepping ****
  // *******************************************

  printF(" *** buildImplicitSolvers ***** \n");


  numberOfImplicitSolversNeeded=1;

  if( scalarSystemForImplicitTimeStepping && numberOfImplicitSolversNeeded==2 &&
      ( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") || parameters.isAxisymmetric() ) )
  {
    printF("buildImplicitSolvers: INFO: I could use a scalar system with %i number of solvers but\n"
           "  this case is not implemented for 2nd-order artificial diffusion nor axis-symmetric\n",
                numberOfImplicitSolversNeeded);
    
    scalarSystemForImplicitTimeStepping=false;
    numberOfImplicitSolversNeeded=1;
  }


  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel )
    numberOfImplicitSolversNeeded++;
  
  
  if( scalarSystemForImplicitTimeStepping )
    printF("buildImplicitSolvers: implicit time stepping for velocity is for a scalar system since BC's are consistent\n");
  else
  {
    if( !parameters.dbase.get<bool >("useFullSystemForImplicitTimeStepping") )

      printF("buildImplicitSolvers: implicit time stepping for velocity is for a system since BC's are NOT consistent\n");
    else
      printF("buildImplicitSolvers: using a full system for the velocity for implicit time stepping\n");
  }
  
  if( numberOfImplicitSolversNeeded!=numberOfImplicitSolvers )
  {
    delete [] implicitSolver;
    implicitSolver=NULL;
    numberOfImplicitSolvers=numberOfImplicitSolversNeeded;
    if( numberOfImplicitSolvers>0 )
      implicitSolver= new Oges [numberOfImplicitSolvers];
  }




  printF(" *** buildImplicitSolvers : numberOfImplicitSolvers=%i ***** \n",numberOfImplicitSolvers);

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForImplicitSolve"))+=getCPU()-cpu0;
}
