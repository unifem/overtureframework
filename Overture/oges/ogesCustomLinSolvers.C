
// 
//   Addition to OGES, these are for adding more linear solvers.
//
//   $Id: ogesCustomLinSolvers.C,v 1.5 2002/08/01 18:31:18 henshaw Exp $
//


//  for the stuff from `ogesl.C'
#include "Oges.h"

#define p(x) id(ptr+(x))

#include "SparseRep.h"
#include <string.h>

#ifdef GETLENGTH
#define GET_LENGTH dimension
#else
#define GET_LENGTH getLength
#endif

extern realMappedGridFunction Overture::nullRealMappedGridFunction();

#define COEFF(i,n,I1,I2,I3) \
  coeffG(i+stencilDim*(n),I1,I2,I3)

#define EQUATIONNUMBER(i,n,I1,I2,I3) \
  equationNumberX(i+stencilDim*(n),I1,I2,I3)

#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

//\begin{>>OgesInclude.tex}{\subsection{formMatrix}} 
int Oges::
formMatrix( )
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
  if( transpose )
    solverJob = solverJob | 32;   // set solverJob to solve for transpose
  //.........initial call to coefficient routine, determine the maximum
  //         number of entries in any discrete equation
  //         nde=maximum number of non-zero entries in the matrix
  // initialization :
  kint.redim(Range(1,numberOfGrids+1));  // for ogde
  kint=0;
  
  if( maximumNumberOfCoefficients <= 0 )  
    maximumNumberOfCoefficients=int( numberOfComponents*pow(orderOfAccuracy+1,numberOfDimensions)+1+1+1+1 );
  // numberOfNonzerosBound: the estimate for the number of non-zero entries
  //                        in the matrix
  // ...Use zeroRatio if it has been specified
  if( zeroRatio <= 0. )
    {
      zeroRatio=real(maximumNumberOfCoefficients);
    }
  
  numberOfNonzerosBound=int( numberOfEquations*zeroRatio+.5 );
  
  if( debug & 2 )
    {
      cout << "oges: numberOfEquations     =" << numberOfEquations << endl;
      cout << "oges: numberOfNonzerosBound =" << numberOfNonzerosBound << endl;
      cout << "oges: matrixCutoff          =" << matrixCutoff << endl;
      cout << "oges: realToIntegerRatio    =" << realToIntegerRatio << endl;
    }
    
  //   determine order of discretization
  
  Index Axes(axis1,numberOfDimensions);
  int maximumOrderOfAccuracy=4;
  int grid;
  for( grid=0; grid<numberOfGrids; grid++ )
    {
      maximumOrderOfAccuracy = min( maximumOrderOfAccuracy,
                                    min(cg[grid].discretizationWidth()(Axes))-1 );
    }
  if( orderOfAccuracy  > maximumOrderOfAccuracy )
    {
      cout << "oges: Requested order of accuracy is too large for this grid" << endl;
      cout << "oges: reducing accuracy to =" << maximumOrderOfAccuracy << endl;
      orderOfAccuracy=maximumOrderOfAccuracy;
    }
  if( orderOfAccuracy !=2 && orderOfAccuracy !=4 )
    {
      cerr << "oges: invalid value for orderOfAccuracy = " << orderOfAccuracy << endl;
      exit(1);
    }
  
  maximumInterpolationWidth = max(interpolationWidth);
  
  //...........allocate space for the coefficients of the equation
  //           ce,ie,ne
  if( solverJob & 1 )
    {
      //cout << "------OGES::formMatrix --- Allocating space--\n";
      eqnNo.redim(Range(1,maximumNumberOfCoefficients+1),Range(1,numberOfComponents+1)); 
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
  else
    {
      if( eqnNo.GET_LENGTH(axis1)==0 )
        {
          cerr << "Oges: Error eqnNo not dimensioned?!! " << endl;
          exit(1);
        }
    }
  
  if( Oges::debug & 4 ) 
    {
      cout << "equationType                        =" << equationType << endl;
      cout << "machineEpsilon                      =" << machineEpsilon << endl;
      cout << "matrixCutoff                        =" << matrixCutoff << endl;
      cout << "numberOfComponents                  =" << numberOfComponents << endl;
      cout << "numberOfExtraEquations              =" << numberOfExtraEquations << endl;
      cout << "numberOfGhostLines                  =" << numberOfGhostLines << endl;
      cout << "orderOfAccuracy                     =" << orderOfAccuracy << endl;
      cout << "preconditionBoundary                =" << preconditionBoundary << endl;
      cout << "realToIntegerRatio                  =" << realToIntegerRatio << endl;
      cout << "solverJob                           =" << solverJob << endl;
      cout << "solverType                          =" << solverType << endl;
      cout << "sparseFormat                        =" << sparseFormat << endl;
      cout << "storageFormat                       =" << storageFormat << endl;
      cout << "tranpose                            =" << transpose << endl;
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

//    if( preconditionBoundary )
//      {
//        // ===re-order equations at the boundary to prevent a null pivot
      
      
//        if( Oges::debug & 4 )
//          cout << " oges: setup Boundary preconditionning..." << endl;
      
//        setupBoundaryPC( bpcNumberOfEquations, bpcNumberOfNonzeros, errorNumber );
//      }
  
  if( Oges::debug & 2 )
    {
      cout << "oges: numberOfEquations  = " << numberOfEquations << endl;
      cout << "oges: numberOfNonzeros   == " << numberOfNonzeros << endl;
      cout << "oges: Work space created with zeroRatio =" << zeroRatio << endl;
    }
  //  ...return actual value obtained for zratio
  actualZeroRatio=numberOfNonzeros/real(numberOfEquations);
  if( Oges::debug & 2 )
    cout << "CGES: Actual value for zeroRatio =" << actualZeroRatio << endl; 
  
  //........allocate more work spaces for the appropriate solver
  //allocate2();
  //if( Oges::debug & 4 ) cout << "After allocate2" << endl;
    
  //... and the rest is in 'formRhsAndSolutionVectors'
  return errorNumber;
}


void Oges::
formRhsAndSolutionVectors( realCompositeGridFunction & u, 
                           realCompositeGridFunction & f, int & ierr )
{

  // ..used to be in 'formMatrix'

  int grid;
  Index I1,I2,I3;
  //  Zero out interpolation, extrapolation and periodic points by default
  int isACoefficientMatrix = coeff[0].getIsACoefficientMatrix();
  if( fixupRightHandSide )
  {
    for( grid=0; grid<numberOfGrids; grid++ )
    {
      IntegerDistributedArray & classifyX       = isACoefficientMatrix ? coeff[grid].sparse->classify
                                                        : classify[grid];
      getIndex(cg[grid].dimension(),I1,I2,I3);
      for( int n=0; n<numberOfComponents; n++ )
      {
        if( solverType==bcg )
        { // for iterative solvers also zero out the solution???
          where( classifyX(I1,I2,I3,n) <= 0 )
          {
            // u[grid](I1,I2,I3,n)=0.;   // ***** do this ??
            f[grid](I1,I2,I3,n)=0.;  
          }
        }
        else
        {
          where( classifyX(I1,I2,I3,n) <= 0 )
            f[grid](I1,I2,I3,n)=0.;  
        }
      }
    }
  }  


//   ***** fix this ****
  if( storageFormat==1 )
  {
    // ...compact storage sol=u, rhs=f:
    cerr << "oges: error storageFormat=1 not implemented! " << endl;
    exit(1);
  }

  // ul : is adopted from u but combines all the dimensions
  // fl : is adopted from f but combines all the dimensions
  if( ul.getLength() < numberOfGrids )
    for( grid=ul.getLength(); grid<numberOfGrids; grid++ )
    {
      RealArray temp;
      ul.addElement(temp); 
    }
  
  if( fl.getLength() < numberOfGrids )
    for( grid=fl.getLength(); grid<numberOfGrids; grid++ )
    {
      RealArray temp;
      fl.addElement(temp);
    }
  
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    ul[grid].redim(0);
    ul[grid].adopt(&(u[grid](u[grid].getBase(0),u[grid].getBase(1),u[grid].getBase(2),u[grid].getBase(3))),
                             u[grid].GET_LENGTH(axis1)
                            *u[grid].GET_LENGTH(axis2)
                            *u[grid].GET_LENGTH(axis3)
                            *u[grid].GET_LENGTH(axis3+1));
    fl[grid].redim(0);
    fl[grid].adopt(&(f[grid](f[grid].getBase(0),f[grid].getBase(1),f[grid].getBase(2),f[grid].getBase(3))),
                             f[grid].GET_LENGTH(axis1)
                            *f[grid].GET_LENGTH(axis2)
                            *f[grid].GET_LENGTH(axis3)
                            *f[grid].GET_LENGTH(axis3+1));

  }
  
  // real compatibilityValue;
  //if( compatibilityConstraint && solverType==bcg )
  //{
  // // get the rhs for the compatibility equation (do here since u and f may be the same):
  //  int ne,i1e,i2e,i3e,gride;
  //  equationToIndex(extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
  //  compatibilityValue=f[gride](i1e,i2e,i3e);
  // }

  //.. old 'formRhs'etc continues here

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

  if( storageFormat==0 )
  {
    //  for( int m=1; m<=numberOfEquations; m++ )   // *********************** this is SLOW ****
    //  rhs(m)=fl[kv(m)-1](i1v(m));
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

  RealArray rhsCopy=rhs; // Setting the sol. vec thrashes `rhs'

//    // DEBUG CODE--DUMP THE RHS to a file
//    ofstream rhsFile("rhsDump.dat");
//    //  rhs.display("Here's rhs.  (2)");

//    rhsFile.precision(15);
//    for( int iLooper=1; iLooper<=numberOfEquations; iLooper++ ) {

//      rhsFile.setf(ios::scientific,ios::floatfield);
//      rhsFile.setf(ios::internal,ios::adjustfield);
//      rhsFile << rhs(iLooper) << endl;

//    }
//    rhsFile.close();

  { // Sets the solution vector
    int shift=0, size;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        size=ul[grid].elementCount();
        Range Rg(0,size-1);
        sol(Rg+shift+sol.getBase(0))=ul[grid](Rg);
        shift+=size;
      }
  }

  rhs=rhsCopy;
  //  ---call the appropriate solver
  //callSparseSolver( ierr );


}


//\begin{>>OgesInclude.tex}{\subsection{solve}} 
void Oges::
formRhsAndSolutionVectors( realMappedGridFunction & u, 
                           realMappedGridFunction & f, int & ierr )
//=====================================================================================
// /Purpose: Form the matrix for the given coefficients. Then extract the
//           matrix, and try other linear solvers
//\end{OgesInclude.tex}
//=====================================================================================
{
  Range R[4] = { nullRange,nullRange,nullRange,nullRange };
  R[u.positionOfComponent(0)]= Range(u.getComponentBase(0),u.getComponentBound(0));

  realCompositeGridFunction u0(cg,R[0],R[1],R[2],R[3]);
  u0[0].reference(u);
  realCompositeGridFunction f0(cg,R[0],R[1],R[2],R[3]);
  f0[0].reference(f);

  formRhsAndSolutionVectors(u0,f0,ierr); // call the CompositeGrid version
}    


void Oges::
storeSolIntoGridFunction( int & ierr )
{


  real cpu0 = getCPU();
  ierr=0;
  maximumResidual=0.;
  int n;
  
  //  ---call the appropriate solver
  
  //callSparseSolver( ierr );
  
  if( solverJob & 1 )
    solverJob-=1;
  if( solverJob & 2 )
    solverJob-=2;
  if( solverJob & 4 )
    solverJob-=4;
  

  //  ...Copy solution from workspace array into solution arrays.
  if( storageFormat==0 )
  {

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
  
}
