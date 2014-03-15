#include "Oges.h"

#define p(x) id(ptr+(x))

// #define CGESL0  cgesl0_
#define CGESL1C  cgesl1c_
#define CGESL2C  cgesl2c_
#define CGESL3C  cgesl3c_
#define CGESL4C  cgesl4c_
#define CGESRD  cgesrd_
#define DRMACH  drmach_

extern "C"
{
  
//  void CGESL0( int & id, real & rd, int & ptr, int & neq0, int & ng0, int & pu0, int & pf0,
//          int & isf0, int & kv0, int & i1v0, real & sol0, real & rhs0, real & vn0,
//          int & nv0, int & ise, real & sing, int & debug0, int & iter, real & resmx,
//          int & ip, real & rp, int & ierr );

  void CGESL1C( int & id, real & rd, int & job, int & neq, int & nqs, int & ndia, int & ia,
		int & ndja, int & ja, int & nda, real & a,int & perm, int & iperm, real & sol,
		real & rhs, int & nsp, real & isp, real & rsp, int & esp, real & fratio, int & nze,
		int & debug, real & fratio0, int & ierr );
  
  void CGESL2C( int & id, real & rd, int & job, int & neq, int & nze, int & nqs, int & ndia, 
		int & ia, int & ndja, int & ja, int & nda, real & a, real & sol, real & rhs, 
		int & ikeep, int & iwh, real & wh, real & fratio, real & fratio2, real & uh, 
		int & ias, int & jas, real & as, int & itimp, int & debug, real & fratio0, real & fratio20, 
		int & ierr );

  void CGESL3C( int & id, real & rd, int & job, int & neq, int & nze, int & ndia, 
		int & ia, int & ndja, int & ja, int & nda, real & a, real & sol, real & rhs, 
		int & ndiwk, int & iwk, int & ndwk, real & wk, int & nit, real & tol, 
		int & icg, int & ipc, int & debug, int & iter, real & resmx, int & nsave, int & ierr );
  

  void CGESL4C( int & id, real & rd, int & job, int & neq, int & nze, int & ndia, 
		int & ia, int & ndja, int & ja, int & nda, real & a, real & sol, real & rhs, 
		int & nit, real & tol, real & omega, int & debug, int & iter, real & resmx, int & ierr );
  

  void CGESRD( int & solver, int & neq, int & ia, int & ja,real & a,int & perm,
	       int & iperm, real & u, real & f, real & r, real & resmx, int & idebug, int & ierr );
  
  real DRMACH( const int & );

}


void Oges::
solveEquations( int & ierr )
{
  //=====================================================================
  // 
  //     precondition rhs if necessary
  //     call the appropriate solver routine
  //     perfrom iterative improvement if necessary
  //     copy solution into user arrays if necessary  
  //
  //=====================================================================

  real cpu0 = getCPU();
  ierr=0;

  maximumResidual=0.;
  int n;
  
  if( Oges::debug & 4 )
  {
    cout << "solveEquations: solverJob =" << solverJob 
         << ", numberOfEquations=" << numberOfEquations << endl;
  }
  
  numberOfIterations=0;

  //    ...Copy the right-hand side from the input arrays into a workspace.
  //       (unless the storage format is compressed already)

  if( preconditionBoundary && preconditionRightHandSide  )
  {
    assert( numberOfComponents==1 );
    
    if( Oges::debug & 4 )
    {
      cout << "solveEquations: precondition rhs..., storageFormat=" << storageFormat << endl;
    }
    
    
    // ---matrix has been preconditionned, change rhs to reflect this
    if( storageFormat==0 )    
    {
      // for( int m=1; m<=numberOfEquations; m++ )
      //  rhs(m)=fl[kv(m)-1](i1v(m));
      int shift=0, size;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	size=fl[grid].elementCount();
	Range Rg(0,size-1);
	rhs(Rg+shift+rhs.getBase(0))=fl[grid](Rg);
	shift+=size;
      }
      
      if( Oges::debug & 4 ) 
        cout << " solveEquations: apply preconditioner.. (bpcNumberOfEquations="
             << bpcNumberOfEquations << ")" << endl;
      
      //  ---precondition the rhs
      for( n=1; n<=bpcNumberOfEquations; n++ )
      {
        if( Oges::debug & 64 )
	{
	  printf( " n=%6i, iep(n)=%6i, (j,a)=", n,iep(n));
	  for( int j=iap(n); j<=iap(n+1)-1; j++ )
            printf(" (%6i,%7.2e) ", jap(j),ap(j));
	  printf("\n");
	}
        real temp=0.;
	for( int j=iap(n); j<=iap(n+1)-1; j++ )
          temp=temp+ap(j)*fl[kv(jap(j))-1](i1v(jap(j)));
        rhs(iep(n))=temp;
      }
    }
    else
    {
      // ...compressed storage mode, first store result in the temporary array rhsp
      real temp;
      for( n=1; n<=bpcNumberOfEquations; n++ )
      {
        real temp=0.;
        for( int j=iap(n); j<=iap(n+1)-1; j++ )
          temp=temp+ap(j)*rhs(jap(j));
        rhsp(n)=temp;
      }
      for( n=1; n<=bpcNumberOfEquations; n++ )
      {
	temp=rhs(iep(n));
	rhs(iep(n))=rhsp(n);   //    ! change rhs
        rhsp(n)=temp;            //    ! save the original value
      }
    }
  }
  else if( storageFormat==0 )
  {
    //  for( int m=1; m<=numberOfEquations; m++ )   // *********************** this is SLOW ****
    //	rhs(m)=fl[kv(m)-1](i1v(m));
    //  cout << "solveEquations: ***** copy f to rhs using FAST version ***** " << endl;
    
    int shift=rhs.getBase(0), size;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      size=fl[grid].elementCount();
      if( numberOfComponents==1 )
      {
        Range Rg(0,size-1);
	rhs(Rg+shift)=fl[grid](Rg);
      }
      else
      {
        // multiple components are interleaved first -- not the same as in the grid function where
        // they are last 
        assert( size % numberOfComponents == 0 );
        int numPerComponent = size/numberOfComponents; // number of equations for each component
        Range R0(0,numPerComponent-1);
        Range R(0,size-numberOfComponents,numberOfComponents);          // **note** stride
        for( int n=0; n<numberOfComponents; n++ )
          rhs(R+n+shift) = fl[grid](R0+n*numPerComponent);
      }
      shift+=size;
    }
  }
  if( Oges::debug & 32 )
  {
    rhs.display("Here is the rhs for the matrix");
  }
  

  if( parameters.iterativeImprovement==1 )
  {
    //  ---Iterative improvement : save the rhs for some solvers
    for( int m=1; m<=numberOfEquations; m++ )
      rhsii(m)=rhs(m);
  }

  //  ---call the appropriate solver

  callSparseSolver( ierr );
  
  if( solverJob & 1 )
    solverJob-=1;
  if( solverJob & 2 )
    solverJob-=2;
  if( solverJob & 4 )
    solverJob-=4;
  

  if( parameters.iterativeImprovement==1 )
  {
    //  ---iterative improvement
    real vmax0=0.;
    int nitii=3;
    for( int itii=1; itii<=nitii; itii++ )
    {
      //    --compute the residual in double precision
      //        resii <- rhsii - A sol
      CGESRD( solverJob,numberOfEquations,Oges::ias(1),Oges::jas(1),
              Oges::as(1),Oges::perm(1),Oges::iperm(1),sol(1),rhsii(1),
              resii(1),maximumResidual,Oges::debug,ierr );
      
      //        --solve the residual equation
      //                 A vii = resii
      callSparseSolver( ierr );

      //        ---correct sol <- sol + v
      real vmax=0.;
      for( int m=1; m<=numberOfEquations; m++ )
      {
        vmax=max(vmax,fabs(vii(m)));
	sol(m)=sol(m)+vii(m);
      }
      cout << "CGESL0: itii=" << itii << ", max corr =" << vmax
	<< ", maximumResidual = " << maximumResidual << endl;
      //        ---check for no further convergence
      if( itii > 1 && vmax > 0.1*vmax0 ) break;
      vmax0=vmax;
    }
  }

  //  ...Copy solution from workspace array into solution arrays.
  if( storageFormat==0 )
  {
    //   for( int m=1; m<=numberOfEquations; m++ )
    //	ul[kv(m)-1](i1v(m))=sol(m);
    //  cout << "solveEquations: ***** copy sol to u using FAST version ***** " << endl;
/* ---
    int shift=0, size;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      size=ul[grid].elementCount();
      Range Rg(0,size-1);
      ul[grid](Rg)=sol(Rg+shift+sol.getBase(0));
      shift+=size;
    }
--- */
    if( debug & 32 )
      sol.display("Here is the solution array sol");
    
    int shift=sol.getBase(0), size;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      size=ul[grid].elementCount();
      if( numberOfComponents==1 )
      {
        Range Rg(0,size-1);
	ul[grid](Rg)=sol(Rg+shift);
      }
      else
      {
        // multiple components are interleaved first -- not the same as in the grid function where
        // they are last 
        assert( size % numberOfComponents == 0 );
        int numPerComponent = size/numberOfComponents; // number of equations for each component
        Range R0(0,numPerComponent-1);
        Range R(0,size-numberOfComponents,numberOfComponents);          // **note** stride
        for( int n=0; n<numberOfComponents; n++ )
          ul[grid](R0+n*numPerComponent) = sol(R+n+shift);
      }
      shift+=size;
    }

  }
  
  if( preconditionBoundary && (storageFormat==1) && preconditionRightHandSide )
  {
    //    ---matrix has been preconditionned, restore the rhs if storageFormat=1
    //       ...compressed storage mode:
    for( n=1; n<=bpcNumberOfEquations; n++ )
      rhs(iep(n))=rhsp(n);
  }
  if( debug & 2 )
    printf("OGES::time for ogesl = %e \n",getCPU()-cpu0);
}

void Oges::
callSparseSolver( int & ierr )
{
  //=====================================================================
  //     Call the appropriate solver routine
  //
  //
  //=====================================================================

  ierr=0;
  
  maximumResidual=0.;
  numberOfIterations=0;
  
  int id0;  // dummy disk arrays
  real rd0;
  const int numberOfTries=3;
  int tryToFactor;
  
  switch (parameters.solver)
  {
  case OgesParameters::yale:
    if( Oges::debug & 4 )
      cout << "callSparseSolver: calling CGESL1..." << endl;

    for( tryToFactor=0; tryToFactor<numberOfTries; tryToFactor++ )
    {
      CGESL1C( id0,rd0,solverJob,numberOfEquations,numberOfNonzerosBound,
        ndia,ia(1),ndja,
        ja(1),nda,a(1),
        perm(1), iperm(1),          //  id(p(perm)),id(p(iperm)),
        sol(1),rhs(1),
        nsp,rsp(1),rsp(1),yaleExcessWorkSpace,parameters.fillinRatio,
        numberOfNonzeros,Oges::debug,actualFillinRatio,ierr );
      if( ierr<0 )
      { // insufficient storage ... try again
        if( tryToFactor<numberOfTries-1 )
	{
  	  if( Oges::debug & 1 )
  	    cout << "callSparseSolver: yale solver: not enough storage to factor, nsp= " << nsp << endl
	         << "...resizing arrays to try again..." << endl;
          nsp=int( nsp*1.5 );
          rsp.resize(Range(1,nsp+1)); 
        }
	else
	{
          cerr << "Error return from CGESL1C, ierr=" << ierr << endl;
          exit(1);
	}
      }
      else
        break;
    }
    if( ierr != 0 )
    {
      cerr << "Error return from CGESL1C, ierr=" << ierr << endl;
      exit(1);
    }
    // return excess storage, provided there is a significant amount and provided that
    // we have allocated space on this call ( solverJob & 1 )
    // For moving grid problems we may want to keep a little extra around anyway
    if( solverJob & 1 && yaleExcessWorkSpace > numberOfNonzeros*.1 )
    {
      if( Oges::debug & 8 )
      {
        cout << "callSparseSolver: yaleExcessWorkSpace = " << yaleExcessWorkSpace << endl;
        cout << "...resizing arrays to restore excess space..." << endl;
      }
      nsp-=(yaleExcessWorkSpace);
      rsp.resize(Range(1,nsp+1)); 
      yaleExcessWorkSpace=0;
    }
    break;

  case OgesParameters::harwell:

    if( Oges::debug & 4 )
      cout << "callSparseSolver: calling CGESL2C..." << endl;
    
    CGESL2C( id0,rd0,solverJob,numberOfEquations,numberOfNonzeros,numberOfNonzerosBound,
        ndia,ia(1),ndja,ja(1),nda,a(1),sol(1),rhs(1),ikeep(1),iwh(1),wh(1),
        parameters.fillinRatio,parameters.fillinRatio2,parameters.tolerance,ias(1),jas(1),as(1),
        parameters.iterativeImprovement,
        Oges::debug,actualFillinRatio,actualFillinRatio2,ierr );

    if( ierr != 0 )
    {
      cerr << "Error return from CGESL2, ierr=" << ierr << endl;
      exit(1);
    }
    break;

  case OgesParameters::SLAP:
  {
   
    if( storageFormat==0 )
    {
      // for( int m=1; m<=numberOfEquations; m++ )
      //   sol(m)=ul[kv(m)-1](i1v(m));
      int shift=0, size;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	size=ul[grid].elementCount();
	Range Rg(0,size-1);
	sol(Rg+shift+sol.getBase(0))=ul[grid](Rg);
	shift+=size;
      }
    }
    //   ...convergence tolerance is proportional to the number
    //      of grid points in one direction (?)
    //      and must be at least 500*drmach(3)
    real conjugateGradientTolerance=parameters.tolerance;
    if( conjugateGradientTolerance <= 0. )
      conjugateGradientTolerance=max( real(numberOfEquations),500.)*DRMACH(3);
    
    int conjugateGradientNumberOfIterations=parameters.maximumNumberOfIterations;
    if( conjugateGradientNumberOfIterations <= 0 )
      conjugateGradientNumberOfIterations=int( 6*pow(real(numberOfEquations),.5) );

    // on input numberOfIterations = minimum number of iterations
    numberOfIterations=parameters.minimumNumberOfIterations;

    int conjugateGradientType=(parameters.solverMethod==OgesParameters::biConjugateGradient ? 0 :
			       parameters.solverMethod==OgesParameters::biConjugateGradientSquared ? 1 :
                               2);
    int conjugateGradientPreconditioner=(parameters.preconditioner==OgesParameters::noPreconditioner ? 0 :
                                         parameters.preconditioner==OgesParameters::diagonalPreconditioner ? 1 :
                                         parameters.preconditioner==OgesParameters::incompleteLUPreconditioner ? 2 :
                                         3);

    CGESL3C( id0,rd0,solverJob,numberOfEquations,numberOfNonzeros,
        ndia,ia(1),ndja,
        ja(1),nda,a(1),sol(1),rhs(1),
        ndiwk,iwk(1),ndwk,wk(1),
        conjugateGradientNumberOfIterations,
        conjugateGradientTolerance,conjugateGradientType,
        conjugateGradientPreconditioner,Oges::debug,
        numberOfIterations,maximumResidual,parameters.gmresRestartLength,ierr );

    if( ierr < 0 )
    {
      cout << "Warning from CGESL3, ierr=" << ierr << endl;
      ierr=0;
    }
    else if( ierr > 0 )
    {
      cerr << "Error return from CGESL3, ierr=" << ierr << endl;
      exit(1);
    }
    break;
    
  }
 
  case OgesParameters::sor:
    
    //      Initial guess:
    if( storageFormat==0 )
    {
      //for( int m=1; m<=numberOfEquations; m++ )
      //  sol(m)=ul[kv(m)-1](i1v(m));
      int shift=0, size;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	size=ul[grid].elementCount();
	Range Rg(0,size-1);
	sol(Rg+shift+sol.getBase(0))=ul[grid](Rg);
	shift+=size;
      }
    }
    //   ...convergence tolerance is proportional to the number
    //      of grid points in one direction (?)
    //      and must be at least 500*drmach(3)
    int sorNumberOfIterations=parameters.maximumNumberOfIterations;
    if( sorNumberOfIterations <=0 )
      sorNumberOfIterations=numberOfEquations;

    real sorTolerance=parameters.tolerance;
    if( sorTolerance <= 0.0 )
      sorTolerance=numberOfEquations*1.e-8;

    real sorOmega=parameters.sorOmega;
    if( sorOmega <= 0. )
      sorOmega=1.2;
    
    CGESL4C( id0,rd0,solverJob,numberOfEquations,numberOfNonzeros,
        ndia,ia(1),ndja,ja(1),nda,
        a(1),sol(1),rhs(1),
        sorNumberOfIterations,sorTolerance,sorOmega,
	   Oges::debug,numberOfIterations,maximumResidual,ierr );

    if( ierr != 0 )
    {
      cerr << "Error return from CGESL3, ierr=" << ierr << endl;
      exit(1);
    }
    break;
  }
}


// ---- from Petri: ogesCustomLinSolvers.C



// 
//   Addition to OGES, these are for adding more linear solvers.
//
//   $Id: ogesl.C,v 1.4 2002/08/01 18:31:18 henshaw Exp $
//


//  for the stuff from `ogesl.C'
#include "SparseRep.h"
#include <string.h>


//\begin{>>OgesInclude.tex}{\subsection{formMatrix}} 
int Oges::
formMatrix(int & numberOfEquations_, int & numberOfNonzeros_ )
//=====================================================================================
// /Purpose: 
//  Form the matrix for the given coefficients. Then extract the
//           matrix, and try other linear solvers. Does not do a solve.
// /Errors:  Some...
// /Return Values: An error number --> out of date.
//\end{OgesInclude.tex}
//=====================================================================================
{
  
  real cpu0=getCPU();
  
  int errorNumber=0;
  
  if( !initialized )
  {
    errorNumber=initialize() ;
  }
  else
  {
    if( shouldBeInitialized )
    {
      printf("Oges::solve: ERROR: Me thinks that you have changed some parameters that will affect the matrix\n"
             " such as the numberOfComponents or the order of accuracy, but the matrix is not being \n"
             "initialized when I think that it should be. \n"
             "Maybe you called Oges::updateToMatchGrid before setting these parameters?\n"
             "For example you should call Oges::setCoefficientArray(...) before Oges::updateToMatchGrid\n");
      Overture::abort("error");
    }
  }
    
  solverJob = solverJob | 2;     // set for 'generateMatrix'

  //.........Generate the matrix
  if( parameters.solveForTranspose )
    solverJob = solverJob | 32;   // set solverJob to solve for transpose
  //.........initial call to coefficient routine, determine the maximum
  //         number of entries in any discrete equation
  //         nde=maximum number of non-zero entries in the matrix
  
  const int stencilSize = coeff[0].sparse->stencilSize;
  int maximumNumberOfCoefficients=int( numberOfComponents*stencilSize+1+1+1+1 );
  // numberOfNonzerosBound: the estimate for the number of non-zero entries
  //                        in the matrix
  // ...Use zeroRatio if it has been specified
  if( parameters.zeroRatio <= 0. )
  {
    parameters.zeroRatio=real(maximumNumberOfCoefficients);
  }
  
  numberOfNonzerosBound=int( numberOfEquations*parameters.zeroRatio+.5 );
  
  if( debug & 2 )
  {
    cout << "oges: numberOfEquations     =" << numberOfEquations << endl;
    cout << "oges: numberOfNonzerosBound =" << numberOfNonzerosBound << endl;
    cout << "oges: realToIntegerRatio    =" << realToIntegerRatio << endl;
  }
    
  
//  maximumInterpolationWidth = max(interpolationWidth);
  
  //...........allocate space for the coefficients of the equation
  //           ce,ie,ne
  if( solverJob & 1 )
  {
    //cout << "------OGES::formMatrix --- Allocating space--\n";
    // eqnNo.redim(Range(1,maximumNumberOfCoefficients+1),Range(1,numberOfComponents+1)); 
    //...........allocate work spaces for the appropriate solver
    //allocate1();
    int nsol=numberOfEquations;
    sol.redim(Range(1,nsol+1));
    rhs.redim(Range(1,nsol+1));
    ndia=numberOfEquations+1;
    ia.redim( Range(1,ndia+1) );
    ndja=numberOfNonzerosBound;
    ja.redim(Range(1,ndja+1));
    nda=numberOfNonzerosBound;
    a.redim(Range(1,nda+1));
      
  }
//   else
//   {
//     if( eqnNo.getLength(axis1)==0 )
//     {
//       cerr << "Oges: Error eqnNo not dimensioned?!! " << endl;
//       exit(1);
//     }
//   }
  
  if( Oges::debug & 4 ) 
  {
    parameters.display();
    cout << "machineEpsilon                      =" << machineEpsilon << endl;
    cout << "numberOfComponents                  =" << numberOfComponents << endl;
    cout << "preconditionBoundary                =" << preconditionBoundary << endl;
    cout << "realToIntegerRatio                  =" << realToIntegerRatio << endl;
    cout << "solverJob                           =" << solverJob << endl;
    cout << "sparseFormat                        =" << sparseFormat << endl;
    cout << "storageFormat                       =" << storageFormat << endl;
  }
  
  //..........Generate and load the matrix
  
  if( errorNumber!=0 )
  {
    cerr << "Error before Oges::formMatrix: errorNumber=" << errorNumber << endl;
    cerr << "oges:ERROR: getErrorMessage(errorNumber)" << endl;
    exit(1);
  }
  if( Oges::debug & 2 )
  {
    cout << "Oges::solve: before generate coefficients ..." << endl;
    cout << "numberOfEquations = " << numberOfEquations << endl;
    cout << "oges: ndia= " << ndia << ", Oges::ndia=" << Oges::ndia << endl;
    cout << "numberOfGrids = " << numberOfGrids << endl;
    cout << "numberOfComponents = " << numberOfComponents << endl;
  }
  
  generateMatrix( errorNumber );
  
  if( Oges::debug & 4 ) 
    cout << "After generateMatrix " << endl;
  if( errorNumber!=0 ) {
    cerr << "Oges::Error return from generateMatrix, errorNumber=" << errorNumber << endl;
    cerr << "Oges:ERROR: getErrorMessage(errorNumber)" << endl;
    exit(1);
  }

  if( Oges::debug & 2 )
  {
    cout << "oges: numberOfEquations  = " << numberOfEquations << endl;
    cout << "oges: numberOfNonzeros   == " << numberOfNonzeros << endl;
    cout << "oges: Work space created with parameters.zeroRatio =" << parameters.zeroRatio << endl;
  }
  //  ...return actual value obtained for zratio
  actualZeroRatio=numberOfNonzeros/real(numberOfEquations);
  if( Oges::debug & 2 )
    cout << "CGES: Actual value for zeroRatio =" << actualZeroRatio << endl; 
  
  //........allocate more work spaces for the appropriate solver
  //allocate2();
  //if( Oges::debug & 4 ) cout << "After allocate2" << endl;
    
  //... and the rest is in 'formRhsAndSolutionVectors'

  numberOfEquations_=numberOfEquations;
  numberOfNonzeros_=numberOfNonzeros;

  return errorNumber;
}


