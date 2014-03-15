#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "interpPoints.h"
#include "SparseRep.h"
#include "ExposedPoints.h"
#include "Ogen.h"
#include "App.h"
#include "gridFunctionNorms.h"
#include "ParallelUtility.h"
#include "ArraySimple.h"
#include "Oges.h"
#include "OgesParameters.h"
#include "PlotIt.h"

extern int 
residual(const RealGridCollectionFunction & coeff,
         const RealGridCollectionFunction & u,
         const RealGridCollectionFunction & f,
         RealGridCollectionFunction & r );

namespace {
  
  int iuOld=0, iuNew=1;

  void diagonalScaling(Oges &solver, 
		       RealCompositeGridFunction &coeff,
		       RealCompositeGridFunction &rhs_cg)
  {
    CompositeGrid &cg = *coeff.getCompositeGrid();
    const int stencilLength = coeff[0].sparse->stencilSize;
    const int numberOfComponents = coeff[0].sparse->numberOfComponents;
    const int stencilDim = stencilLength*numberOfComponents;

    const int numberOfGrids = cg.numberOfGrids();
    
    for ( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	realMappedGridFunction &rhs = rhs_cg[grid];
	realMappedGridFunction &cf   = coeff[grid];
	real *rhsp = rhs.Array_Descriptor.Array_View_Pointer3;
	int rhsd[] = { rhs.getRawDataSize(0), rhs.getRawDataSize(1), rhs.getRawDataSize(2) };
#define RHS(i1,i2,i3,e) rhsp[i1 + rhsd[0]*(i2 + rhsd[1]*( i3 + rhsd[2]*e ) ) ]
	real *cfp = coeff[grid].Array_Descriptor.Array_View_Pointer3;
	int cfd[] = { cf.getRawDataSize(0), cf.getRawDataSize(1), cf.getRawDataSize(2) };
#define CF(i1,i2,i3,i4) cfp[i1 + cfd[0]*(i2 + cfd[1]*( i3 + cfd[2]*i4 ) ) ]
	    
	// we need to do a full matrix-vector multiply (not just the disc. points) because we need to get
	//    the boundary conditions evaluated as well.  Otherwise we could just loop over the interior points
	//    and use indexing that is more simple.
	// Most of the code to do this comes directly from residual.C (thanks Bill!!)
	// // //  BEGIN (mostly) COPIED CODE
	const IntegerDistributedArray & equationNumber = coeff[grid].sparse->equationNumber;
	const IntegerDistributedArray & classify = coeff[grid].sparse->classify;
	
	const int ndra=cg[grid].dimension(Start,axis1), ndrb=cg[grid].dimension(End,axis1);
	const int ndsa=cg[grid].dimension(Start,axis2), ndsb=cg[grid].dimension(End,axis2);
	const int ndta=cg[grid].dimension(Start,axis3), ndtb=cg[grid].dimension(End,axis3);
	
	const realArray & coeffG = coeff[grid];
	
	// // XXX THE FOLLOWING SECTION NEEDS TO CHANGE IF scalarSystemForImplicitTimeStepping==TRUE
	
	// ------- general case ------
	int i1,i2,i3,n;
	for( i3=ndta; i3<=ndtb; i3++ )
	  {
	    for( i2=ndsa; i2<=ndsb; i2++ )
	      {
		for( i1=ndra; i1<=ndrb; i1++ )
		  {
		    // printf(" i1,i2=%i,%i \n",i1,i2);
		    for( n=0; n<numberOfComponents; n++)
		      {
			if( classify(i1,i2,i3,n)!=SparseRepForMGF::unused )
			  {
			    real scale=0;
			    for( int s=0; s<stencilDim; s++)
			      scale=max(scale,fabs(CF(s+stencilDim*n,i1,i2,i3)));
			    if ( scale>10*REAL_MIN )
			      {
				scale = 1./scale;
				
				RHS(i1,i2,i3,n) *= scale;
				for( int s=0; s<stencilDim; s++)
				  CF(s+stencilDim*n,i1,i2,i3) *= scale;
			      }
			  } // is used point
		      } // end number of components
		  } // end i1
	      } // end i2 
	  } // end i3
	
      } // end for grid 
  }

  int 
  residualWithConstraint(Oges &solver,
			 const RealGridCollectionFunction & coeff,
			 const RealGridCollectionFunction & u,
			 const RealGridCollectionFunction & f,
			 RealGridCollectionFunction & r )
  {
    
    // compute the residual just using the coefficient grid function
    ::residual(coeff, u, f, r);
    //                return 0;
    r.finishBoundaryConditions();

    int hasConstraint = false;
    solver.get(OgesParameters::THEcompatibilityConstraint,hasConstraint); // OgesParameters does not have get/set for bools...
    if ( !hasConstraint ) return 0;

    // a constraint equation has been added to the system, add it to the residual
    const realCompositeGridFunction &constraintCoeff = solver.rightNullVector;
    const GridCollection & cg = *coeff.getGridCollection();
    for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
      {
	const MappedGrid &mg = cg[grid];
	Index I1,I2,I3,C;
	getIndex(mg.gridIndexRange(),I1,I2,I3);
	const realMappedGridFunction &cc_mg = constraintCoeff[grid];
	const realMappedGridFunction &u_mg = u[grid];
	realMappedGridFunction &r_mg = r[grid];

	for ( int e=0; e<solver.numberOfExtraEquations; e++ ) // !! assumes e corresponds to  component in coeff, ne should probably reflect this ??
	  {
	    int ne,i1e,i2e,i3e,gride;
	    solver.equationToIndex( solver.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
	    real ue = u[gride](i1e,i2e,i3e,ne);
	    r_mg(I1,I2,I3,e) -= cc_mg(I1,I2,I3,e)*ue; // - sign because ::residual computes r=f-coeff*u
	  }
      }

  }


}

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{advanceNewton}} 
void DomainSolver::
advanceNewton( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  )
// ======================================================================================
//  /Description:
//     This method executes an inexact-Newton step.   The basic scheme is given by
//     \begin{eqnarray*}
//     L(u^n) u^* = R(u^n)\\
//     u^{n+1} = (1-\xi)u^n + \xi u^*
//     \end{eqnarray*}
//     where $L$ and $R$ are the linearization of the operator (matrix and right hand side respectively)
//     and $\xi$ is the step length determined by a simple line search.
//     To handle the nonlinearity, we assume that $F^{n+1}$ has been linearized using
//     $u^{n+1}=u^n+\delta u$ such that 
//     \begin{eqnarray*}
//      F^{n+1} = Lu^{n+1} + Ru^n + {\cal O}((\delta u)^2)$.
//     \end{eqnarray*}
//     Dropping the second order terms and substituting into the time stepping method above
//     produces leaves only $L$ and $R$ for use in the iteration.

//     This method mainly relies on three functions in the {\tt DomainSolver} interface:
//     {\tt formMatrixForImplicitSolve}, {\tt getUt}, and {\tt implicitSolve}.  The first function must 
//     build $I-\theta\Delta tL$, stored in {\tt GridFunction::coeff}.  $R$ is built by {\tt getUt} and the
//     linear system is solved (if neccessary) by {\tt implicitSolve}.
//
//
//     kkc 060721
//
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
  // Notes:
  // - the right hand side will be placed in uNew and overwritten when the system is solved
  // - this code currently assumes that all the components will be solved for in one system
  // - soveForTimeIndependentVariables is currently not called (but should be...?)

  if ( init )
    {
      init=false;
    }

  assert(parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::trapezoidal   ||
	 parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::backwardEuler ||
	 parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::crankNicolson  );
  
  assert(!parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping")); // see below for a hint about what to change to make this work

  ListOfShowFileParameters &pPar = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");
  real initialResidual = REAL_MAX;
  bool hasInitialResidual = pPar.getParameter("initial_newton_residual", initialResidual);
  real newtonRelativeTolerance = 1e-9;
  real newtonAbsoluteTolerance = 1e-10;
  int maxNumberOfFailedNewtonSteps = 10;
  pPar.getParameter("newton_relative_tol",newtonRelativeTolerance);
  pPar.getParameter("newton_absolute_tol",newtonAbsoluteTolerance);
  pPar.getParameter("maxNumberOfFailedNewtonSteps",maxNumberOfFailedNewtonSteps);
  if ( !parameters.dbase.has_key("numberOfFailedNewtonSteps") )
    {
      parameters.dbase.put<int>("numberOfFailedNewtonSteps");
      parameters.dbase.get<int>("numberOfFailedNewtonSteps") = 0;
    }
  
  int &numberOfFailedSteps = parameters.dbase.get<int>("numberOfFailedNewtonSteps");

  int maxSearchIterations = 10;
  bool hasMaxSearchIterations = pPar.getParameter("newton_max_search_iterations",maxSearchIterations);
  int useL2Norm = 0;
  bool hasUseL2Norm = pPar.getParameter("newton_use_l2",useL2Norm);
  
  
  int numberOfComponentsForCoefficients = gf[current].u.getNumberOfComponents();
  ArraySimpleFixed<real,20,1,1,1> rparam;
  ArraySimpleFixed<int,20,1,1,1> iparam;
  int nDim = gf[current].cg.numberOfDimensions();

  GridFunction residual;
  residual.u.updateToMatchGridFunction(gf[iuOld].u);

  for( int subStep=0; subStep<numberOfSubSteps; subStep++ )
  {
    //       cout<<"subStep : "<<subStep<<", iuOld : "<<iuOld<<", iuNew "<<iuNew<<endl;

    int globalStepNumber = parameters.dbase.get<int >("globalStepNumber")++;
    GridFunction &uOld = gf[iuOld];
    GridFunction &uNew = gf[iuNew];

    for ( int grid=0; grid<uOld.cg.numberOfComponentGrids(); grid++ )
      {
	uNew.u[grid] = 0.0;
      }

    bool recomputeMatrix = (initialStep+subStep)%parameters.dbase.get<int >("refactorFrequency")==0;
    if ( recomputeMatrix ) 
      { // build the matrix here because the constraint value in the rhs is written here too

	//	cout<<"rebuilding matrix, step = "<<initialStep + subStep<<endl;
	parameters.dbase.get<int >("initializeImplicitTimeStepping") = true;
	formMatrixForImplicitSolve(dt0,uNew,uOld); // this call will build the DomainSolver::coeff which is - L
	parameters.dbase.get<int >("initializeImplicitTimeStepping") = false;
      }
    else if ( implicitSolver[0].rightNullVector.numberOfComponentGrids() )
      {
	Oges &solver = implicitSolver[0];
	realCompositeGridFunction &constraintCoeff = solver.rightNullVector;
	CompositeGrid &cg = uOld.cg;
	real cval = 0.0;
	for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
	  {
	    MappedGrid &mg = cg[grid];
	    Index I1,I2,I3;
	    getIndex(mg.gridIndexRange(),I1,I2,I3);
	    realMappedGridFunction &cc_mg = constraintCoeff[grid];
	    cval += sum(uOld.u[grid](I1,I2,I3,parameters.dbase.get<int >("rc"))*cc_mg(I1,I2,I3,parameters.dbase.get<int >("rc")));
	  } 
	int ne,i1e,i2e,i3e,gride;
	solver.equationToIndex( solver.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
	//	cout<<"RHS = "<<cval<<endl;
	uNew.u[gride](i1e,i2e,i3e,ne)=cval;
      }


    OgesParameters & ogesParameters = implicitSolver[0].parameters;
    real matrixCutoff = 0.;
    matrixCutoff = ogesParameters.get(OgesParameters::THEmatrixCutoff,matrixCutoff);

    // Collect up the following info every substep in case the grid changes.
    // actually, we only need this info of we are NOT doing backward Euler...
    // // // the next section of code was taken from residual.C
    const int stencilLength = coeff[0].sparse->stencilSize;
    const int numberOfComponents = coeff[0].sparse->numberOfComponents;
    const int stencilDim = stencilLength*numberOfComponents;

    CompositeGrid &cg = uOld.cg;
    const int numberOfGrids = cg.numberOfGrids();
    
    IntegerArray arraySize(numberOfGrids,3), arrayDims(numberOfGrids,3);
    arraySize=1;
    arrayDims=0;
    for( int grid=0; grid<numberOfGrids; grid++ )
      {
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  {
	    arraySize(grid,axis)=cg[grid].dimension(End,axis)-cg[grid].dimension(Start,axis)+1;
	    arrayDims(grid,axis)=cg[grid].dimension(Start,axis);
	  }
      }
    
    IntegerArray gridEquationBase(numberOfGrids+1);
    gridEquationBase.redim(numberOfGrids+1);
    gridEquationBase(0)=0;
    for( int grid=1; grid<=numberOfGrids; grid++ )
      gridEquationBase(grid)=gridEquationBase(grid-1)+
	numberOfComponents*((cg[grid-1].dimension(End,axis1)-cg[grid-1].dimension(Start,axis1)+1)*
			    (cg[grid-1].dimension(End,axis2)-cg[grid-1].dimension(Start,axis2)+1)*
			    (cg[grid-1].dimension(End,axis3)-cg[grid-1].dimension(Start,axis3)+1));
    
    // // // //


    // some options in getUt return R in two parts, one from explict discretizations (dvdt->uNew.u) 
    //      and one from linearization of a nonlinear implicit operator (dvdtImplicit->fn[0]).  The sum
    //      of these terms forms R.  
    fn[0] = 0.;
    for ( int grid=0; grid<uOld.cg.numberOfComponentGrids(); grid++ )
      {
	rparam[0]=uOld.t;
	rparam[1]=uOld.t+dt0*.5; // tforce
	rparam[2]=uOld.t+dt0; // tImplicit
	iparam[0]=grid;
	iparam[1]=uOld.cg.refinementLevelNumber(grid);
	iparam[2]=numberOfStepsTaken;
	if( debug() & 4 ) 
	  gf[iuNew].u.display("advanceNewton : RHS before getUt ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");

// 	mappedGridSolver[grid]->getUt(uOld.u[grid],uOld.getGridVelocity(grid),
// 				      uNew.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],&uNew.cg[grid]);
	getUt(uOld.u[grid],uOld.getGridVelocity(grid),
	      uNew.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],&uNew.cg[grid]);

	if( debug() & 4 ) 
	  gf[iuNew].u.display("advanceNewton : RHS after getUt ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");

	Index I1,I2,I3,all;
	//XXX	getIndex(uNew.cg[grid].indexRange(),I1,I2,I3);
	getIndex(uNew.cg[grid].dimension(),I1,I2,I3);
	uNew.u[grid](I1,I2,I3,all) += fn[0][grid](I1,I2,I3,all);
	//	uNew.u[grid](I1,I2,I3,all) += uOld.u[grid](I1,I2,I3,all);

	if( debug() & 4 ) 
	  gf[iuNew].u.display("advanceNewton : RHS after adding f^n ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
      }

    gf[iuNew].t = t0 + dt0;

    Range RR(0,0),all;
    applyBoundaryConditionsForImplicitTimeStepping( gf[iuNew] );

    // compute the residual from the previous time step (basically F(u^n))
    //    ::residual(coeff, uOld.u, uNew.u, residual.u);
    ::residualWithConstraint(implicitSolver[0],coeff, uOld.u, uNew.u, residual.u);

    // // at this point we should have both coeff and the rhs computed correctly
    if( debug() & 4 ) 
      gf[iuNew].u.display("advanceNewton : RHS before implicit solve ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");

    //    ::diagonalScaling(implicitSolver[0],coeff,uNew.u);

    if ( implicitSolver[0].isSolverIterative() )
      {
	//	int precond_freq = implicitSolver[0].getNumberOfIterations() < 10 ? 1000 : parameters.dbase.get<int>("preconditionerFrequency");
	int precond_freq = parameters.dbase.get<int>("preconditionerFrequency");
	int maxIts = 0;
	implicitSolver[0].get(OgesParameters::THEmaximumNumberOfIterations,maxIts);
	bool usedMaxIterations = implicitSolver[0].getNumberOfIterations()>=maxIts;
	implicitSolver[0].recomputePreconditioner = implicitSolver[0].getNumberOfIterations() < 10 ? false : (usedMaxIterations || (globalStepNumber%precond_freq)==0);
	
      }

    real ff = parameters.dbase.get<real >("implicitFactor");

    if ( ff>REAL_EPSILON ) {
      implicitSolve(dt0,gf[iuNew],gf[iuOld]);// uOld is provided as an initial guess for iterative solvers
    } // else just use the routine to compute the nonlinear residual

    if( debug() & 4 ) 
      gf[iuNew].u.display("advanceNewton : solution after implicit solve ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
    
    Range N = parameters.dbase.get<Range >("Rt");
    real maximumResidual=0;
    real maximuml2 = 0.;
    RealArray maxRes(N);
    RealArray l2Res(N);
    maxRes=0.;
    l2Res=0;
    int maskOption=1;  // check mask()>0 
    int extra=0;
    real maxL_inf = 0;
    real maxL_2 = 0;
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      {
	maxRes(n)=maxNorm(residual.u,n,maskOption,extra);
	l2Res(n) =l2Norm(residual.u,n,maskOption,extra);
	maxL_inf = max(maxRes(n),maxL_inf);
	maxL_2 = max(l2Res(n),maxL_2);
      }

    real maxL_inf_tmp = 0;
    real maxL_2_tmp = 0;

    real &maxL     = useL2Norm ? maxL_2 : maxL_inf;
    real &maxL_tmp = useL2Norm ? maxL_2_tmp : maxL_inf_tmp;

    if ( !hasInitialResidual )
      {
	pPar.setParameter("initial_newton_residual", maxL);
	initialResidual = maxL;
	saveSequenceInfo(t0,residual.u);

	//	PlotIt::contour(*Overture::getGraphicsInterface(),residual.u);
      }

    real nonLin_tol = max(newtonAbsoluteTolerance, initialResidual*newtonRelativeTolerance);

    bool ok = false;
    GridFunction rhs,fullStep;
    fullStep.u.updateToMatchGridFunction(gf[iuNew].u);
    fullStep.u = gf[iuNew].u;

    RealArray maxRes_tmp(N);
    RealArray l2Res_tmp(N);
    gf[iuNew].u = (1.-ff)*gf[iuOld].u + ff*fullStep.u;//gf[iuNew].u;

    real phi[] = {maxL, REAL_MAX, REAL_MAX};
    real l[] = {ff, 0., 0.};
    int ii=0;
    real absTol,relTol;
    implicitSolver[0].get(OgesParameters::THEabsoluteTolerance,absTol);
    implicitSolver[0].get(OgesParameters::THErelativeTolerance,relTol);
    
    real maxL_rel = maxL/initialResidual;
    //081210 is the linear solver's residual scaled?    real eta_old = absTol/maxL_inf;
    real eta_old = absTol/maxL_rel;

    rhs.u.updateToMatchGridFunction(gf[iuNew].u);
    rhs.cg.reference(gf[iuNew].cg);
    ok = false;
    while ( !ok && /*ff>1e-4 &&*/ ii<maxSearchIterations )
      {
	parameters.dbase.get<int >("initializeImplicitTimeStepping") = true;
	formMatrixForImplicitSolve(dt0,rhs,uNew); // this call will build the DomainSolver::coeff which is - L
	parameters.dbase.get<int >("initializeImplicitTimeStepping") = false;

	fn[0] = 0.;
	for ( int grid=0; grid<uOld.cg.numberOfComponentGrids(); grid++ )
	  {
	    rhs.u[grid]=0.;
	    rparam[0]=uOld.t;
	    rparam[1]=uOld.t+dt0*.5; // tforce
	    rparam[2]=uOld.t+dt0; // tImplicit
	    iparam[0]=grid;
	    iparam[1]=uOld.cg.refinementLevelNumber(grid);
	    iparam[2]=numberOfStepsTaken;

	    getUt(uNew.u[grid],uNew.getGridVelocity(grid),
		  rhs.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],&uNew.cg[grid]);
	    
	    Index I1,I2,I3,all;
	    //XXXX	    getIndex(uNew.cg[grid].indexRange(),I1,I2,I3);
	    getIndex(uNew.cg[grid].dimension(),I1,I2,I3);
	    rhs.u[grid](I1,I2,I3,all) += fn[0][grid](I1,I2,I3,all);
	    //	uNew.u[grid](I1,I2,I3,all) += uOld.u[grid](I1,I2,I3,all);
	  }
	int old_current = current;
	current = iuNew;
	applyBoundaryConditionsForImplicitTimeStepping( rhs );
	current = old_current;

	//	::residual(coeff, uNew.u, rhs.u, residual.u);
	::residualWithConstraint(implicitSolver[0],coeff, uNew.u, rhs.u, residual.u);

	maxL_inf_tmp = 0;
	maxL_2_tmp = 0;
	for( int n=N.getBase(); n<=N.getBound(); n++ )
	  {
	    maxRes_tmp(n)=maxNorm(residual.u,n,maskOption,extra);
	    l2Res_tmp(n) =l2Norm(residual.u,n,maskOption,extra);
	    maxL_inf_tmp = max(maxRes_tmp(n),maxL_inf_tmp);
	    maxL_2_tmp = max(l2Res_tmp(n),maxL_2_tmp);
	  }
	real tfac = 1e-4;
	//	cout<<"ff = "<<ff<<", maxL_inf = "<<maxL_inf<<", maxL_inf_tmp = "<<maxL_inf_tmp<<", maxLtol = "<<(1.-tfac*(1.-eta_old))*maxL_inf<<", tol = "<<nonLin_tol<<endl;
	//	cout<<"      maxL_2 = "<<maxL_2<<", maxL_2_tmp = "<<maxL_2_tmp<<endl;
	//	if ( maxL_inf_tmp>((1.-tfac*(1.-eta_old))*maxL_inf) ) 
	real maxL_inf_tmp_rel = maxL_inf_tmp/initialResidual;
	real maxL_inf_rel = maxL_inf/initialResidual;
	real maxL_tmp_rel = maxL_tmp/initialResidual;
	real maxL_rel = maxL/initialResidual;
	cout<<"step = "<<ff<<", "<< (useL2Norm ? "(l2) : current = " : "(max) : current = ")<<maxL<<", tmp = "<<maxL_tmp<<", step tol = "<<((1.-tfac*(1.-eta_old))*maxL)<<", tol = "<<nonLin_tol<<endl;
	//	if ( maxL_inf_tmp_rel>((1.-tfac*(1.-eta_old))*maxL_inf_rel) ) 
	if ( maxL_tmp_rel>((1.-tfac*(1.-eta_old))*maxL_rel) ) 
	  {
	    real ff_old = ff;
	    if ( false && ii>1 )
	      {
		phi[(ii+1)%2 + 1] = maxL_inf_tmp;
		l[(ii+1)%2 + 1] = ff_old;

		real p2 = -(-l[0] * phi[2] + phi[1] * l[0] + l[2] * phi[0] + phi[2] * l[1] - l[2] * phi[1] - phi[0] * l[1]) / (-l[0] * l[2] + l[1] * l[0] + pow(l[2], 0.2e1) - l[2] * l[1]) / (l[0] - l[1]);
		if ( p2<REAL_MIN ) 
		  {
		    ff/=2.;
		    cout<<"P'' is negative = "<<p2<<endl;
		  }
		else
		  {
		    real fft= .5*( (l[0]*l[0]*(phi[1]-phi[2])+l[1]*l[1]*(phi[2]-phi[0])+l[2]*l[2]*(phi[0]-phi[1]))/
				(l[0]*(phi[1]-phi[2]) + l[1]*(phi[2]-phi[0]) + l[2]*(phi[0]-phi[1])) );

		    cout<<"QUADRATIC ff = "<<fft<<", p'' = "<<p2<<endl;
		    ff = fft<REAL_MIN ? ff/2. : max(1e-4,min(fft,.9));
		  }
	      }
	    else
	      {
		//phi[ii] = maxL_inf_tmp;
		//l[ii] = ff_old;
		ff/=2.;
	      }

	    gf[iuNew].u = (1.-ff)*gf[iuOld].u + ff*fullStep.u;
	  }
	else 
	  ok = true;
	ii++;
      }

    if (!ok) 
      numberOfFailedSteps++;
    else
      numberOfFailedSteps=0;

    real maxL_rel_tmp = maxL_tmp/initialResidual;
    //    real maxL_rel = maxL/initialResidual;
    //    real eta = min(.9,fabs(maxL_inf_tmp-absTol)/maxL_inf);
    //    real eta = min(.9,fabs(maxL_inf_tmp-absTol)/maxL_inf);
    real eta = min(.1,fabs(maxL_rel_tmp-absTol)/maxL_rel);
    //real eta = min(.9,(maxL_inf_tmp*maxL_inf_tmp)/(maxL_inf*maxL_inf));
    //real eta_test = pow(eta_old,(1.+sqrt(5.))/2.);
    cout<<"NEWTON STEP LENGTH = "<<ff<<", eta (prov) = "<<eta<<", old eta = "<<eta_old;
    //    gf[iuNew].u = gf[iuNew].u;
    real eta_test = .9*eta_old*eta_old;
    eta = eta_test<=.1 ? min(eta,.1) : min(.1,max(eta,eta_test));
    cout<<", eta (act) = "<<eta<<endl;
    //    implicitSolver[0].set(OgesParameters::THEabsoluteTolerance,min(absTol,eta*maxL_inf_tmp));
    implicitSolver[0].set(OgesParameters::THEabsoluteTolerance,min(absTol,eta*maxL_rel_tmp));

    if( debug() & 4 ) 
      gf[iuNew].u.display("advanceNewton : solution after damped newton update ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
    
    //applyBoundaryConditions(gf[iuNew]);
    
    if( debug() & 4 ) 
      gf[iuNew].u.display("advanceNewton : solution after explicit bc ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
    
    solveForTimeIndependentVariables( gf[iuNew] ); 
    
    t0+=dt0;

    
    iuOld = iuNew;
    iuNew = (iuOld+1)%2;

    current = iuOld;  // new way *wdh* 060919 // kkc 070202 this is apparently needed here for some reason
    if (maxL_tmp<=nonLin_tol)
      {
	maxL = maxL_tmp;
	real maxL_rel = maxL/initialResidual;
	printF("NEWTON ITERATION CONVERGED TO %g (%g relative)\n",maxL,maxL_rel);
	numberOfSubSteps = subStep;
	parameters.dbase.get<int >("maxIterations") = 0;
	break;
      }
    else if ( numberOfFailedSteps>=maxNumberOfFailedNewtonSteps )
      {
	maxL = maxL_tmp;
	real maxL_rel = maxL/initialResidual;
	printF("NEWTON ITERATION FAILURE, RESIDUAL IS %g (%g relative)\n",maxL,maxL_rel);
	numberOfSubSteps = subStep;
	parameters.dbase.get<int >("maxIterations") = 0;
	break;
      }


  } // end for each substep
  
  current = iuOld;  // new way *wdh* 060919

//   solution.u.reference(gf[iuOld].u);
//   solution.cg.reference(gf[iuOld].cg);
//   solution.referenceGridVelocity(gf[iuOld]);
//   solution.t = t0;

  saveSequenceInfo(t0,residual.u);
  fn[0].reference(residual.u);
  output( gf[current],initialStep+numberOfSubSteps-1);

}


