/* $Id: formMatrix.C,v 1.13 2009/09/04 20:38:15 chand Exp $ */
#include "Oges.h"
#include "SparseRep.h"
#include "EquationSolver.h"

// Declare and define base and bounds, perform loop
#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// Perform loop
#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )


//\begin{>>OgesInclude.tex}{\subsection{formMatrix}} 
int Oges::
formMatrix(int & numberOfEquations_, int & numberOfNonzeros_, 
           SparseStorageFormatEnum sparseStorageFormat_,
           bool allocateSpace /* =TRUE */,
           bool factorMatrixInPlace /* =FALSE */ )
//=====================================================================================
// /Purpose: 
//  Form the matrix for the given coefficients. Then extract the
//           matrix, and try other linear solvers. Does not do a solve.
//
// /numberOfEquations\_, numberOfNonzeros\_ (output) :
// /sparseStorageFormat (input) : uncompressed or compressedRow
// /allocateSpace (input) : if true, allocate space for the matrix. Otherwise use existing space
// /factorMatrixInPlace (input) : if true, allocate space in the matrix to hold the factorization
//    (or preconditioner). If false, only allocate space for the matrix elements.
//
// /Errors:  Some...
// /Return Values: An error number --> out of date.
//\end{OgesInclude.tex}
//=====================================================================================
{
  real cpu0 = getCPU();
  
  int errorNumber = 0;
  
  sparseStorageFormat=sparseStorageFormat_;
  int grid;
  // we keep a local reference to the classify array since we may destroy the coeff to save space.
  delete [] classify; //*wdh* there may be a different number of grids.
  classify = new intArray [numberOfGrids];
  for( grid=0; grid<numberOfGrids; grid++) 
  {
    if( useAllGrids || useThisGrid(grid) )
    {
      classify[grid].reference(coeff[grid].sparse->classify);
    }
  }
  
  // --- count the number of equations
  numberOfEquations = 0;
  for (grid=0;grid<numberOfGrids;grid++) 
  {
    if( useAllGrids || useThisGrid(grid) )
      numberOfEquations += arraySize(grid,axis1)*arraySize(grid,axis2)*arraySize(grid,axis3);
  }
  numberOfEquations *= numberOfComponents;


  // const int stencilSize = coeff[grid0].sparse->stencilSize;
  assert( stencilSize>0 );
  int maximumNumberOfCoefficients = int(numberOfComponents*stencilSize+1+1+1+1);

  if( debug & 4 )
    printf("Oges::formMatrix: numberOfEquations=%i, stencilSize=%i\n",numberOfEquations,stencilSize);

  // numberOfNonzerosBound: the estimate for the number of non-zero entries
  //                        in the matrix
  // ...Use zeroRatio if it has been specified
  if (parameters.zeroRatio<=0.) 
  {
    parameters.zeroRatio = real(maximumNumberOfCoefficients);
  }
  
  numberOfNonzerosBound = int(numberOfEquations*parameters.zeroRatio+.5);

  //...........allocate space for the coefficients of the equation
  if( Oges::debug & 2 ) 
    printF("------OGES::formMatrix --- Allocating space--\n");

  // eqnNo.redim(Range(1,maximumNumberOfCoefficients+1),Range(1,numberOfComponents+1)); 

  //...........allocate work spaces for the appropriate solver

  // uncompressed, compressed, factorMatrixInPlace, allocateSpace

  if (allocateSpace) 
  {
    if (!factorMatrixInPlace)
    { // only leave space for the matrix and not the factorization
      ndja = numberOfNonzerosBound;
    }
    else 
    {                   // leave space to fill in the factorization
      ndja = int(numberOfNonzerosBound*parameters.fillinRatio+.5);
    }
    
    ndia   = (sparseStorageFormat==compressedRow && !parameters.solveForTranspose ) ? numberOfEquations+1 : ndja+1;
    nda    = ndja;

    const int base=0;
    if (sparseStorageFormat==other) 
    {
      equationSolver[parameters.solver]->allocateMatrix(ndia,ndja,nda,numberOfEquations);
    }
    else 
    {
      if (ia.getLength(0)!=ndia) 
      {
        ia.redim(Range(base,ndia+base));
      }
      if (ja.getLength(0)!=ndja) 
      {
        ja.redim(Range(base,ndja+base));
      }
      if (a.getLength(0)!=nda) 
      {
        a.redim(Range(base,nda+base));
      }
    }

    int nsol = numberOfEquations;  // *** why here ??
    if (sol.getLength(0)!=nsol) 
    {
      sol.redim(Range(base,nsol+base));
    }
    if (canSolveInPlace()) 
    {
      rhs.reference(sol);  // bug ?
      // printf(" ***** reference rhs to sol ***** ");
      // rhs.adopt(&sol(base),Range(base,numberOfEquations+base));   
    }
    else 
    {
      // printf(" ***** DO NOT reference rhs to sol ***** ");
      if (rhs.getLength(0)!=nsol) 
      {
        rhs.redim(Range(base,nsol+base));
      }
    }
  }
  
  //  ...parameter for throwing away small matrix elements:
  if (parameters.matrixCutoff<=0.) 
  {
    parameters.matrixCutoff = REAL_EPSILON;
  }

  bool gridHasChanged=!initialized || shouldBeInitialized;  // *** fix this **********************************
  if ((parameters.compatibilityConstraint && numberOfExtraEquations==0) || gridHasChanged) 
  {
    // this will build the null vector and assign extra equations to an unused point on the grid
    initialize();
  }

  //..........Generate and load the matrix
  
  if (Oges::debug & 2) 
  {
    cout << "Oges::solve: before generate coefficients ..." << endl;
    printf("numberOfEquations =%i, numberOfComponents=%i, sparseStorageFormat=%s, allocateSpace=%i \n"
           "numberOfNonzerosBound = %i, parameters.fillinRatio=%8.2e,  \n"
           "parameters.compatibilityConstraint=%i \n"
           "size(ia)=%i, size(ja)=%i size(a)=%i \n",
           numberOfEquations,numberOfComponents,
           (sparseStorageFormat==compressedRow ? "compressedRow" : "uncompressed"),
           allocateSpace,numberOfNonzerosBound, parameters.fillinRatio, parameters.compatibilityConstraint,
           ndia,ndja,nda);
  }
  
  generateMatrix(errorNumber);
  
  if (Oges::debug & 4) 
  { 
    cout << "After generateMatrix " << endl;
  }
  if (errorNumber!=0) 
  {
    cerr << "Oges::Error return from generateMatrix, errorNumber=" << errorNumber << endl;
    cerr << "Oges:ERROR: getErrorMessage(errorNumber)" << endl;
    exit(1);
  }
  if (Oges::debug & 2) 
  {
    cout << "oges: numberOfEquations  = " << numberOfEquations << endl;
    cout << "oges: numberOfNonzeros   == " << numberOfNonzeros << endl;
    cout << "oges: Work space created with parameters.zeroRatio =" << parameters.zeroRatio << endl;
  }
  //  ...return actual value obtained for zratio
  actualZeroRatio = numberOfNonzeros/real(numberOfEquations);
  if (Oges::debug & 2) 
  {
    cout << "CGES: Actual value for zeroRatio =" << actualZeroRatio << endl; 
  }
  if (!parameters.keepCoefficientGridFunction) 
  {
    coeff.destroy();
  }

  numberOfEquations_ = numberOfEquations;
  numberOfNonzeros_  = numberOfNonzeros;

  return errorNumber;
}

//\begin{>>OgesInclude.tex}{\subsection{initialize}} 
int Oges::
initialize( )
//=====================================================================================
// /Purpose: 
//  Perform initializations that depend on the grid and the parameters.
//  The equation number for the compatibility constraint is defined.
//\end{OgesInclude.tex}
//=====================================================================================
{
  int errorNumber = 0;
  
  if( equationSolver[parameters.solver]==NULL )  // *wdh* 090707
    buildEquationSolvers(parameters.solver); 

  // Initialize: gridEquationBase(grid) = first eqn number on a grid (minus one)
  if (numberOfGrids>0) 
  {
    gridEquationBase.redim(numberOfGrids+1); 
    gridEquationBase(0) = 0;
    for (int grid=1;grid<=numberOfGrids;grid++) 
    {
      gridEquationBase(grid) = gridEquationBase(grid-1)+
	numberOfComponents*arraySize(grid-1,axis1)*arraySize(grid-1,axis2)*arraySize(grid-1,axis3);
    }
  }
  
  if( TRUE ||  // *wdh* March 14, 2017 -- do this in parallel too --
      parameters.solver!=OgesParameters::PETScNew )
  { // do not build a nullVector for the new PETSc interface -- at least for now

    if( parameters.compatibilityConstraint ||
        parameters.userSuppliedEquations   ||   // *wdh* 2015/10/11
	numberOfExtraEquations>0 )              // *wdh* 2015/10/11
    {
      int userSuppliedConstraint = 0; // kkc 090903, check for a user defined constraints
      get(OgesParameters::THEuserSuppliedCompatibilityConstraint,userSuppliedConstraint);

      // kkc 090903 the following if statement checks to see if 
      //   a) the number of extra equations has been defined (if not, assume one extra equation), and
      //   b) if a user defined constraint has been set, if not one extra equation will be built
      if( numberOfExtraEquations>0 && ( userSuppliedConstraint || parameters.userSuppliedEquations ) )
      {
      }
      else
      {
         numberOfExtraEquations = 1;
      }
      
      if( Oges::debug & 2 ) 
	printF("--OGES--initialize: compatibilityConstraint=%i userSuppliedEquations=%i userSuppliedConstraint=%i numberOfExtraEquations=%i\n",
	       (int)parameters.compatibilityConstraint,(int)parameters.userSuppliedEquations,userSuppliedConstraint,numberOfExtraEquations );
      

      findExtraEquations();

      if( parameters.solver!=OgesParameters::PETScNew )
      { // do not build a nullVector for the new PETSc interface
	makeRightNullVector();

	if ( !coefficientsOfDenseExtraEquations ) // kkc 080725, the coefficients might be set differently from the rightNullVector
	  coefficientsOfDenseExtraEquations = &rightNullVector;
      }
      
    }
    else if(numberOfExtraEquations>0) 
    {
      // compatibility equation must have been turned off
      numberOfExtraEquations=0;
      rightNullVector.destroy();
      coefficientsOfDenseExtraEquations =NULL;
    }
  }
  // **** OLD: **********
  else if( parameters.solver==OgesParameters::PETScNew )
  {
    if( true )
    {
      // *wdh* 2017/02/24
      if( Oges::debug & 2 )
	printF("\n _____ formMatrix: PETScNew: parameters.compatibilityConstraint=%i, numberOfExtraEquations=%i\n",
	       (int)parameters.compatibilityConstraint,numberOfExtraEquations);

      // ** In parallel the PETScSolver will generate the null vector directly ***
      // ** FIX ME if there are 
      if( parameters.userSuppliedEquations )
      {
	OV_ABORT("Oges::finish me for parallel Bill!");
      }
      

      if( parameters.compatibilityConstraint ||
	  parameters.userSuppliedEquations   ||   // *wdh* 2015/10/11
	  numberOfExtraEquations>0 )              // *wdh* 2015/10/11
      {
	int userSuppliedConstraint = 0; // kkc 090903, check for a user defined constraints
	get(OgesParameters::THEuserSuppliedCompatibilityConstraint,userSuppliedConstraint);

	// kkc 090903 the following if statement checks to see if 
	//   a) the number of extra equations has been defined (if not, assume one extra equation), and
	//   b) if a user defined constraint has been set, if not one extra equation will be built
	if( numberOfExtraEquations>0 && ( userSuppliedConstraint || parameters.userSuppliedEquations ) )
	{
	}
	else
	{
	  numberOfExtraEquations = 1;
	}
	// PETScSolver also finds the location of the extra equation

      }
    }
    
  }

  initialized         = false;
  shouldBeInitialized = false;
  return(errorNumber);
}


int Oges::
formRhsAndSolutionVectors( realCompositeGridFunction & u, 
                           realCompositeGridFunction & f )
// ================================================================================
// /Description:
//   Form the sparse vectors sol and rhs from the grid functions u and f.
// ================================================================================
{
  Index I1,I2,I3;
  int grid;


  //  Zero out interpolation, extrapolation and periodic points (default)
  if (parameters.fixupRightHandSide) 
  {
    assert(classify!=NULL);
    for (grid=0;grid<numberOfGrids;grid++) 
    {
      if( !useAllGrids && !useThisGrid(grid) ) 
        continue; // skip this in-active grid.

      // IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;
      IntegerDistributedArray & classifyX = classify[grid];

      const int *classifyXp = classifyX.Array_Descriptor.Array_View_Pointer3;
      const int classifyXDim0=classifyX.getRawDataSize(0);
      const int classifyXDim1=classifyX.getRawDataSize(1);
      const int classifyXDim2=classifyX.getRawDataSize(2);
#define CLASSIFYX(i0,i1,i2,i3) classifyXp[i0+classifyXDim0*(i1+classifyXDim1*(i2+classifyXDim2*(i3)))]

      realArray & ff = f[grid];
      real *ffp = ff.Array_Descriptor.Array_View_Pointer3;
      const int ffDim0=ff.getRawDataSize(0);
      const int ffDim1=ff.getRawDataSize(1);
      const int ffDim2=ff.getRawDataSize(2);
      const int f3Base=f[grid].getBase(3);
#define FF(i0,i1,i2,i3) ffp[i0+ffDim0*(i1+ffDim1*(i2+ffDim2*(i3)))]


      getIndex(cg[grid].dimension(),I1,I2,I3);
      bool useOpt=true;
      if( useOpt ) // *wdh* 030820
      {
	int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();
	int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();
	int i1,i2,i3;
	for( int n=0; n<numberOfComponents; n++ ) 
	{
	  FOR_3(i1,i2,i3,I1,I2,I3)
	  {
	    if( CLASSIFYX(i1,i2,i3,n)<=0 ) 
	    {
	      FF(i1,i2,i3,n+f3Base)=0.;  
	    }
	  }
	}
      }
      else
      {
	for (int n=0;n<numberOfComponents;n++) 
	{
	  if (parameters.solver==OgesParameters::SLAP) 
	  {
	    // for iterative solvers also zero out the solution???
	    where (classifyX(I1,I2,I3,n)<=0) 
	    {
	      // u[grid](I1,I2,I3,n)=0.;   // ***** do this ??
	      f[grid](I1,I2,I3,n)=0.;  
	    }
	  }
	  else 
	  {
	    where (classifyX(I1,I2,I3,n)<=0) 
	    {
	      f[grid](I1,I2,I3,n)=0.;
	    }
	  }
	}
      }
      
    }
  }  

  // ul : is adopted from u but combines all the dimensions
  // fl : is adopted from f but combines all the dimensions
  if (ul.getLength()<numberOfGrids) 
  {
    for (grid=ul.getLength();grid<numberOfGrids;grid++) 
    {
      RealArray temp;
      ul.addElement(temp); 
    }
  }
  if (fl.getLength()<numberOfGrids)
  {
    for (grid=fl.getLength();grid<numberOfGrids;grid++) 
    {
      RealArray temp;
      fl.addElement(temp);
    }
  }
  
  for (grid=0;grid<numberOfGrids;grid++) 
  {
    ul[grid].redim(0);
    ul[grid].adopt(&(u[grid](u[grid].getBase(0),u[grid].getBase(1),u[grid].getBase(2),u[grid].getBase(3))),
                             u[grid].getLength(axis1)
                            *u[grid].getLength(axis2)
                            *u[grid].getLength(axis3)
                            *u[grid].getLength(axis3+1));
    fl[grid].redim(0);
    fl[grid].adopt(&(f[grid](f[grid].getBase(0),f[grid].getBase(1),f[grid].getBase(2),f[grid].getBase(3))),
                             f[grid].getLength(axis1)
                            *f[grid].getLength(axis2)
                            *f[grid].getLength(axis3)
                            *f[grid].getLength(axis3+1));
  }
  
  if( !useAllGrids && cg.numberOfComponentGrids()>1 )
  {
    // We are only solving for some grids. For interpolation points on active grids that
    // interpolate from in-active grids we need to set the RHS (f) to equal u.

    int *useThisGridp = useThisGrid.getDataPointer();

#define useThis(grid) useThisGridp[grid]
#define IP(i,axis) ipp[(i)+ni*(axis)]
#define IG(i) igp[(i)]

#define UU(i0,i1,i2,i3) uup[i0+uuDim0*(i1+uuDim1*(i2+uuDim2*(i3)))]

#define CC(i,n) ((i)+(n)*numPerComponent)
#define CS(i,n) ((n)+numberOfComponents*(i)+shift)

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ ) 
    {
      if( useThisGrid(grid) && cg.numberOfInterpolationPoints(grid)>0 )
      {
        const int ni = cg.numberOfInterpolationPoints(grid);
	realArray & ff = f[grid];
	real *ffp = ff.Array_Descriptor.Array_View_Pointer3;
	const int ffDim0=ff.getRawDataSize(0);
	const int ffDim1=ff.getRawDataSize(1);
	const int ffDim2=ff.getRawDataSize(2);
        const int f3Base=f[grid].getBase(3);

	realArray & uu = u[grid];
	const real *uup = uu.Array_Descriptor.Array_View_Pointer3;
	const int uuDim0=uu.getRawDataSize(0);
	const int uuDim1=uu.getRawDataSize(1);
	const int uuDim2=uu.getRawDataSize(2);
        const int uBase3=u[grid].getBase(3);

	const int *ipp = cg.interpolationPoint[grid].getDataPointer();
	const int *igp = cg.interpoleeGrid[grid].getDataPointer();

        int i1,i2,i3=0;
	if( cg.numberOfDimensions()==2 )
	{
	  for( int i=0; i<ni; i++ )
	  {
	    if( !useThis(IG(i)) ) // the interpolee grid is in-active
	    {
	      i1=IP(i,0);
	      i2=IP(i,1);

              // printf("activeGrids: grid=%i: set RHS(%i,%i,%i) to -u=%8.2e (interpoleee=%i)\n",
              //      grid,i1,i2,i3,-UU(i1,i2,i3,0),IG(i));
	      
	      for( int n=0; n<numberOfComponents; n++ )
		FF(i1,i2,i3,n+f3Base)=-UU(i1,i2,i3,n+uBase3);   // use -UU since the coeff. in the matrix is -1
	    }
	  }
	}
	else if( cg.numberOfDimensions()==3 )
	{
	  for( int i=0; i<ni; i++ )
	  {
	    if( !useThis(IG(i)) ) // the interpolee grid is in-active
	    {
	      i1=IP(i,0);
	      i2=IP(i,1);
	      i3=IP(i,2);
	      for( int n=0; n<numberOfComponents; n++ )
		FF(i1,i2,i3,n+f3Base)=-UU(i1,i2,i3,n+uBase3);
	    }
	  }
	}
	else
	{
	  Overture::abort("error, nd=1");
	}
      }
    }
    
  }

  int shift=rhs.getBase(0),size;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ ) 
  {
    if( useAllGrids || useThisGrid(grid) )
    {
      size = fl[grid].elementCount();
      if (numberOfComponents==1) 
      {
	Range Rg(0,size-1);
	rhs(Rg+shift) = fl[grid](Rg);
      }
      else 
      {
	// multiple components are interleaved first -- not the same as in the grid function where
	// they are last 
	assert(size%numberOfComponents==0);
	int numPerComponent = size/numberOfComponents; // number of equations for each component
	Range R0(0,numPerComponent-1);
	Range R(0,size-numberOfComponents,numberOfComponents);          // **note** stride
	for( int n=0; n<numberOfComponents; n++ ) 
	{
	  rhs(R+n+shift) = fl[grid](R0+n*numPerComponent);
	}
      }
      shift += size;
    }
    
  }

  // Sets the solution vector for iterative solvers
  if( isSolverIterative() ) 
  {
    shift=0;
    for (grid=0;grid<cg.numberOfComponentGrids();grid++) 
    {
      if( useAllGrids || useThisGrid(grid) )
      {
        RealArray & ug = ul[grid];

	size = ul[grid].elementCount();
	// Range Rg(0,size-1);

        real *psol= sol.Array_Descriptor.Array_View_Pointer0;
        #define SOL(i0) psol[i0]
        real *pug= ug.Array_Descriptor.Array_View_Pointer0;
        #define UG(i0) pug[i0]

	if( numberOfComponents==1 ) 
	{
	  // sol(Rg+shift+sol.getBase(0)) = ug(Rg);
          const int ia=shift+sol.getBase(0);
          for( int i=0; i<size; i++ )
	  {
            SOL(i+ia) = UG(i);
	  }
	  
	}
	else
	{
  	  assert(size%numberOfComponents==0);
	  int numPerComponent = size/numberOfComponents; // number of equations for each component

	  for( int n=0; n<numberOfComponents; n++ ) 
	  {
	    for( int i=0; i<numPerComponent; i++ )
	    {
	      SOL(CS(i,n)) = UG(CC(i,n));
	    }
	  }
	}
	shift += size;
      }
      #undef SOL
      #undef UG
      
    }
  }

  

  if (Oges::debug & 32) 
  {
    sol.display("Here is the sol for the matrix");
    rhs.display("Here is the rhs for the matrix");
  }
  return(0);
}


int Oges::
storeSolutionIntoGridFunction( )
{
  //  ...Copy solution from workspace array into solution arrays.
  int shift=sol.getBase(0),size;
  for (int grid=0;grid<cg.numberOfComponentGrids();grid++) 
  {
    RealArray & ug = ul[grid];
    if( useAllGrids || useThisGrid(grid) )
    {

      size = ug.elementCount();
      real *psol= sol.Array_Descriptor.Array_View_Pointer0;
      #define SOL(i0) psol[i0]
      real *pug= ug.Array_Descriptor.Array_View_Pointer0;
      #define UG(i0) pug[i0]

      if (numberOfComponents==1) 
      {
	// Range Rg(0,size-1);
	// ug(Rg) = sol(Rg+shift);
        const int ia=shift+sol.getBase(0);
        for( int i=0; i<size; i++ )
  	  UG(i) = SOL(i+ia);
      }
      else 
      {
	// multiple components are interleaved first -- not the same as in the grid function where
	// they are last 
	assert(size%numberOfComponents==0);
	int numPerComponent = size/numberOfComponents; // number of equations for each component
	Range R0(0,numPerComponent-1);
	Range R(0,size-numberOfComponents,numberOfComponents);          // **note** stride
	if( false )
	{
	  for(int n=0;n<numberOfComponents;n++) 
	  {
	    ug(R0+n*numPerComponent) = sol(R+n+shift);
	  }
	}
	else
	{
	  for( int n=0; n<numberOfComponents; n++ ) 
	  {
	    for( int i=0; i<numPerComponent; i++ )
	    {
              // printF("oges: store soln: (i,n)=(%i,%i) ug=%5.2f sol=%5.2f dif=%8.2e\n",i,n,ug(CC(i,n)),sol(CS(i,n)),
              //        fabs(ug(CC(i,n))-sol(CS(i,n))));
	      UG(CC(i,n)) = SOL(CS(i,n));
	    }
	  }
	}
	
      }
      #undef SOL
      #undef UG

      shift += size;
    }
  }
  return(0);
}



#undef CC
#undef CS



