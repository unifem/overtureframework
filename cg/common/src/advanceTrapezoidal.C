#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "interpPoints.h"
#include "SparseRep.h"
#include "ExposedPoints.h"
#include "Ogen.h"
#include "App.h"
#include "ParallelUtility.h"
#include "ArraySimple.h"
#include "Oges.h"
#include "OgesParameters.h"

namespace {
  
  int iuOld=0, iuNew=1;

}

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{advanceImplicitMultiStep}} 
void DomainSolver::
advanceTrapezoidal( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  )
// ======================================================================================
//  /Description:
//     This method executes a time step using a generalized trapezoidal method that can handle explicit Euler,
//     backward Euler, Crank-Nicolson and everything in between.  The basic scheme is given by
//     \begin{eqnarray*}
//     u^{n+1}-u^{n} = \Delta t\left( \theta F^{n+1} + (1-\theta)F^{n} \right)
//     \end{eqnarray*}
//     where $F^n$ is a (possibly nonlinear) function of $u^n$ and $\theta$ is a parameter that controls
//     the ``implicitness'' of the method.  Setting $\theta=\frac{1}{2}$ results in a
//     Crank-Nicolson type scheme that is second order accurate in time.
//
//     To handle the nonlinearity, we assume that $F^{n+1}$ has been linearized using
//     $u^{n+1}=u^n+\delta u$ such that 
//     \begin{eqnarray*}
//      F^{n+1} = Lu^{n+1} + Ru^n + {\cal O}((\delta u)^2)$.
//     \end{eqnarray*}
//     Dropping the second order terms and substituting into the time stepping method above
//     produces 
//     \begin{eqnarray*}
//     u^{n+1}-u^{n} = \Delta t\left( \theta ( Lu^{n+1}+Ru^n ) + Ru^n+(1-\theta)Lu^n\right)
//     \end{eqnarray*}
//     So, in the end, when all is said in done, at the end of the day, in the fullness of time,
//     eventually we solve
//     \begin{eqnarray*}
//     \left(I-\Delta t \theta L\right)u^{n+1} = \left(I+\Delta tR+\Delta t(1-\theta)L\right) u^n.
//     \end{eqnarray*}
//     This method mainly relies on three functions in the {\tt DomainSolver} interface:
//     {\tt formMatrixForImplicitSolve}, {\tt getUt}, and {\tt implicitSolve}.  The first function must 
//     build $I-\theta\Delta tL$, stored in {\tt GridFunction::coeff}.  $R$ is built by {\tt getUt} and the
//     linear system is solved (if neccessary) by {\tt implicitSolve}.
//
//
//     kkc 060301
//
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
  // Notes:
  // - the right hand side will be placed in uNew and overwritten when the system is solved
  // - this code currently assumes that all the components will be solved for in one system

  if ( init )
    {
      init=false;
    }

  assert(parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::trapezoidal   ||
	 parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::backwardEuler ||
	 parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::crankNicolson  );
  
  assert(!parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping")); // see below for a hint about what to change to make this work

  int numberOfComponentsForCoefficients = gf[current].u.getNumberOfComponents();
  ArraySimpleFixed<real,20,1,1,1> rparam;
  ArraySimpleFixed<int,20,1,1,1> iparam;
  int nDim = gf[current].cg.numberOfDimensions();

  for( int subStep=0; subStep<numberOfSubSteps; subStep++ )
  {
    //    cout<<"subStep : "<<subStep<<", iuOld : "<<iuOld<<", iuNew "<<iuNew<<endl;

    int globalStepNumber = parameters.dbase.get<int >("globalStepNumber")++;
    GridFunction &uOld = gf[iuOld];
    GridFunction &uNew = gf[iuNew];

    bool recomputeMatrix = (initialStep+subStep)%parameters.dbase.get<int >("refactorFrequency")==0;

    if ( recomputeMatrix )
      {
	//	cout<<"recomputeing matrix, step "<<initialStep+subStep<<endl;
	parameters.dbase.get<int >("initializeImplicitTimeStepping") = true;
	formMatrixForImplicitSolve(dt0,uNew,uOld); // this call will build the DomainSolver::coeff which is I+\Delta t L
	parameters.dbase.get<int >("initializeImplicitTimeStepping") = false;
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
	uNew.u[grid] = 0.0;
// 	mappedGridSolver[grid]->getUt(uOld.u[grid],uOld.getGridVelocity(grid),
// 				      uNew.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],&uNew.cg[grid]);
	getUt(uOld.u[grid],uOld.getGridVelocity(grid),
	      uNew.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],&uNew.cg[grid]);

	if( debug() & 4 ) 
	  gf[iuNew].u.display("advanceTrapezoidal : RHS after getUt ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");

	Index I1,I2,I3,all;
	getIndex(uNew.cg[grid].indexRange(),I1,I2,I3);
	uNew.u[grid](I1,I2,I3,all) *= dt0;
	uNew.u[grid](I1,I2,I3,all) += dt0*fn[0][grid](I1,I2,I3,all);
	uNew.u[grid](I1,I2,I3,all) += uOld.u[grid](I1,I2,I3,all);

	if( debug() & 4 ) 
	  gf[iuNew].u.display("advanceTrapezoidal : RHS after adding u^n ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
      }

    gf[iuNew].t = t0 + dt0;

    if ( (1.-parameters.dbase.get<real >("implicitFactor"))>(100.*REAL_EPSILON) )
      { // only do this part if we are NOT using backward Euler 
	applyBoundaryConditionsForImplicitTimeStepping( gf[iuNew] );

	for ( int grid=0; grid<uOld.cg.numberOfComponentGrids(); grid++ )
	  {
	    // finish the right hand side by adding dt0*(1-th)* L uOld
	    bool fixCoeff = false;
	    real th = (1.-parameters.dbase.get<real >("implicitFactor"));
	    if ( parameters.dbase.get<real >("implicitFactor")<10.*REAL_EPSILON ) 
	      {
		th=1.; // coeff will just be the identity 
		fixCoeff=true&&recomputeMatrix;
	      }
	    else
	      th/=parameters.dbase.get<real >("implicitFactor");
 	    realMappedGridFunction &rhs = uNew.u[grid];
 	    realMappedGridFunction &cf   = coeff[grid];
 	    real *rhsp = uNew.u[grid].Array_Descriptor.Array_View_Pointer3;
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
				// compute a cutoff scale for coefficients in the matrix so we can skip zero entries
				// This code is taken from Oges::generateMatrix so that we only get the matrix entries
				//   ogen will think is there.  Because we are also subtracting the identity, this
				//   check prevents us from hitting the same diagonal entry multiple times (which would perform the
				//   subtraction more than once ). I mention this because this check is not needed for a simple
				//   matrix-vector multiply (as found in residual.C) since a zero coefficient contributes 
				//   nothing to the result.
				real scale=0;
				for( int s=0; s<stencilDim; s++)
				  scale=max(scale,fabs(CF(s+stencilDim*n,i1,i2,i3)));
				scale*=2.*matrixCutoff;
				// scale better not be equal to zero! this is checked in Oges::generateMatrix

				for( int i=0; i<stencilDim; i++)
				  {
				    real cfn = CF(i+stencilDim*n,i1,i2,i3);
				    
				    if ( fabs(cfn)>scale )
				      {
					int eqn=equationNumber(i+stencilDim*(n),i1,i2,i3);
				                                                                                                                                                  
					int gj=numberOfGrids-1;
					for( int grid1=1; grid1<numberOfGrids; grid1++ )
					  {
					    if( eqn <= gridEquationBase(grid1) )
					      {
						gj=grid1-1;
						break;
					      }
					  }
					int nj, j1, j2,j3;
					
					//				    coeff[gj].sparse->equationToIndex( eqn, nj, j1, j2,j3 );
					eqn-=(gridEquationBase(gj)+1);
					nj= (eqn % numberOfComponents);
					eqn/=numberOfComponents;
					j1=(eqn % arraySize(gj,axis1))+arrayDims(gj,axis1);
					eqn/=arraySize(gj,axis1);
					j2=(eqn % arraySize(gj,axis2))+arrayDims(gj,axis2);
					eqn/=arraySize(gj,axis2);
					j3=(eqn % arraySize(gj,axis3))+arrayDims(gj,axis3);

					// note that coeff=I-(\theta dt L) so we need to add on (1-theta)*( I-coeff )/theta (unless theta=0)
					bool isdiagonal = grid==gj && j1==i1 && j2==i2 && j3==i3 && nj==n;
					
					if ( !isdiagonal )
					  RHS(i1,i2,i3,n) -= th*cfn*uOld.u[gj](j1,j2,j3,nj);
					else
					  RHS(i1,i2,i3,n) -= th*(cfn-1.)*uOld.u[gj](j1,j2,j3,nj);

					if ( fixCoeff ) CF(i+stencilDim*n,i1,i2,i3) = !isdiagonal ? 0. : 1;

				      } // if nonzero entry
				  } // end stencil dim
			      } // is used point
			  } // end number of components
		      } // end i1
		  } // end i2 
	      } // end i3
	    
	    // // // END OF (mostly) COPIED CODE
	
	  } // end for grid 
      } // if not backward Euler
    
#undef UU
#undef CF
#undef RHS
    

    //    if( debug() & 4 ) 
    //      gf[iuNew].u.display("advanceTrapezoidal : RHS before implicit bc ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
    if ( parameters.dbase.get<real >("implicitFactor")>10*REAL_EPSILON ) // otherwise we have forward Euler and uNew is finished
      applyBoundaryConditionsForImplicitTimeStepping( gf[iuNew] );
    
    // // at this point we should have both coeff and the rhs computed correctly
    
    if( debug() & 4 ) 
      gf[iuNew].u.display("advanceTrapezoidal : RHS before implicit solve ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");

    if ( parameters.dbase.get<real >("implicitFactor")>10*REAL_EPSILON ) // otherwise we have forward Euler and uNew is finished
    {
      if ( implicitSolver[0].isSolverIterative() )
	{
	  //	int precond_freq = implicitSolver[0].getNumberOfIterations() < 10 ? 1000 : parameters.dbase.get<int>("preconditionerFrequency");
	  //	  int precond_freq = parameters.dbase.get<int>("preconditionerFrequency");
	  //	  implicitSolver[0].recomputePreconditioner = implicitSolver[0].getNumberOfIterations() < 10 ? false : (globalStepNumber%precond_freq)==0;
	  int precond_freq = parameters.dbase.get<int>("preconditionerFrequency");
	  int maxIts = 0;
	  implicitSolver[0].get(OgesParameters::THEmaximumNumberOfIterations,maxIts);
	  bool usedMaxIterations = implicitSolver[0].getNumberOfIterations()>=maxIts;
	  implicitSolver[0].recomputePreconditioner = implicitSolver[0].getNumberOfIterations() < 10 ? false : (usedMaxIterations || (globalStepNumber%precond_freq)==0);
	}
      implicitSolve(dt0,gf[iuNew],gf[iuOld]);// uOld is provided as an initial guess for iterative solvers
    }
    
    if( debug() & 4 ) 
      gf[iuNew].u.display("advanceTrapezoidal : solution after implicit solve ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
    
    //    if ( parameters.dbase.get<real >("implicitFactor")<REAL_EPSILON ) 
    //      applyBoundaryConditions(gf[iuNew]); // then we basically have forward euler, which is explicit
    
    if( debug() & 4 ) 
      gf[iuNew].u.display("advanceTrapezoidal : solution after explicit bc ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
    
    solveForTimeIndependentVariables( gf[iuNew] ); 
    
    t0+=dt0;
    
    iuOld = iuNew;
    iuNew = (iuOld+1)%2;

    current = iuOld;
    //    realCompositeGridFunction v;
    //    realCompositeGridFunction & u = getAugmentedSolution(gf[current],v);
    //    int dum =2; real dum2=100;
    //    plot( t0, dum, dum2 );
  } // end for each substep
  
  
  current = iuOld;  // new way *wdh* 060919
  
//   solution.u.reference(gf[iuOld].u);
//   solution.cg.reference(gf[iuOld].cg);
//   solution.referenceGridVelocity(gf[iuOld]);
//   solution.t = t0;
  fn[0] = (gf[iuOld].u-gf[iuNew].u)/dt;
  saveSequenceInfo(t0,evaluate((gf[iuOld].u-gf[iuNew].u)/dt)); 

  output( gf[current],initialStep+numberOfSubSteps-1);

}


