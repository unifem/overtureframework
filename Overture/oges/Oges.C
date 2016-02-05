#include "Oges.h"
#include "SparseRep.h"
#include "OgesParameters.h"
#include "EquationSolver.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"
#include "MultigridCompositeGrid.h"
#include "OgesExtraEquations.h"
#include "BoundaryData.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
#define R1MACH EXTERN_C_NAME(r1mach)
#define D1MACH EXTERN_C_NAME(d1mach)
#define CGESER EXTERN_C_NAME(cgeser)
#define OGES_MAT_VECT EXTERN_C_NAME(ogesmatvect)
#define OGES_RESIDUAL EXTERN_C_NAME(ogesresidual)

extern "C" 
{ 
  float R1MACH( const int & i );
  double D1MACH( const int & i );

  void OGES_MAT_VECT( const int & n, const real & x, real & y, 
                    const int & nelt, const int & ia, const int & ja, const real & a );

  void OGES_RESIDUAL( const int & n, const real & x, const real & b, real & r, 
                    const int & nelt, const int & ia, const int & ja, const real & a );

}

int Oges::petscIsAvailable=false;        // set to true if PETSc is available
Oges::newPETScFunction Oges::createPETSc=NULL;  // pointer to a function that can "new" a PETSc instance

#ifdef USE_PPP
  MPI_Comm Oges::OGES_COMM_WORLD=MPI_COMM_WORLD;
#else
  MPI_Comm Oges::OGES_COMM_WORLD=0;
#endif

//\begin{>OgesInclude.tex}{\subsection{default constructor}} 
Oges::
Oges()
//==================================================================================
// /Description:
//   Default constructor.
//\end{OgesInclude.tex} 
//==================================================================================
{
  setup();
}

Oges::
Oges( CompositeGrid & cg0 )
{
  cg.reference( cg0 );
  setup();
  privateUpdateToMatchGrid();
}
Oges::
Oges( MappedGrid & mg )
{
  cg.setNumberOfDimensionsAndGrids(mg.numberOfDimensions(), 1);
  cg->numberOfComponentGrids=1;   // fudge for now
  cg[0].reference(mg);
  cg.updateReferences(); // wdh 970220

//  cg = CompositeGrid(mg.numberOfDimensions(),1);  // make a composite grid with 1 component grid
//  cg[0]=mg;

  // *wdh* 990821 cg.update();
  setup();
  privateUpdateToMatchGrid();
}

Oges::
Oges( const Oges & )
//====================================================================
//  Copy constructor: fix this
//====================================================================
{
  setup();
  // privateUpdateToMatchGrid();
}

//===============================================================
// Setup:
//==============================================================
void Oges::
setup()
{
  gridName="unknown";  // name of the composite grid (used to name gridCheckFile)
  solverName="unknown";

  // set solvingSparseSubset to true if we are solving equations on a sparse subset of the grid (e.g. interface)
  solvingSparseSubset=false;
  
  shouldBeInitialized=true;
  // specify some initial values
  numberOfGrids=0;

  equationToSolve=OgesParameters::userDefined;

  for( int i=0; i<maximumNumberOfEquationSolvers; i++ )
    equationSolver[i]=NULL;

  // set default values
  // some of these are changed again later

  //      Default value for job
  //        1=create work spaces
  //        2=generate matrix and factor
  //        4=re-order (if available)
  solverJob=7;

  refactor=false;    // for refactoring and/or
  reorder=false;     // ... reordering
  recomputePreconditioner = true;

  sparseStorageFormat=uncompressed;
  initialized=false;   // initialize on the first call

  preconditionBoundary=false;
  preconditionRightHandSide=true;  // only used when preconditionBoundary==true
  
  numberOfComponents=1;
  numberOfIterations=1;
  stencilSize=0;
  
  numberOfExtraEquations=0;
  coefficientsOfDenseExtraEquations=NULL;  

  classify=NULL;

  useAllGrids=true;   // by default we solve on all grids.
  argc=1;
  argv=new char *[argc];
  argv[0]= new char[5];
  strcpy(argv[0],"oges");

  //  Here is where the user has defined extra equations of over-ridden existing equations:
  if( !dbase.has_key("extraEquations") )
    dbase.put<OgesExtraEquations>("extraEquations");

}

Oges::
~Oges()
{
  if( debug & 4 ) printF ("Inside of ~Oges() \n");
  for( int i=0; i<maximumNumberOfEquationSolvers; i++ )
  {
    delete equationSolver[i];
    equationSolver[i] = 0;
  }

  delete [] classify;
  classify = 0;
  for( int i=0; i<argc; i++ )
    delete [] argv[i];
  delete [] argv;
}

Oges & Oges::
operator=( const Oges & x )
{
  cout << "Oges::operator= ERROR: this operator is not implemented. Use updateToMatchGrid instead\n";

  equationToSolve=x.equationToSolve;
  constantCoefficients=x.constantCoefficients;    
  boundaryConditions=x.boundaryConditions; 
  bcData=x.bcData; 

  return *this;
}



void Oges::
reference(const Oges & )
{
}

int Oges::
setCommandLineArguments( int argc_, char **argv_ )
{
  int i;
  for( i=0; i<argc; i++ )
    delete [] argv[i];
  delete [] argv;
  
  argc=argc_;
  argv=new char *[argc];
  for( i=0; i<argc; i++ )
  {
    argv[i]= new char[strlen(argv_[i])+1];
    strcpy(argv[i],argv_[i]);
  }
  return 0;
}



// ======================================================================================
///  /brief Supply boundary data (these specify coefficients in boundary conditions)
// ======================================================================================
int Oges::
setBoundaryData( std::vector<BoundaryData> & boundaryData )
{
  // *new* 2014/06/30 -- this has as yet to be used ---

  if( !dbase.has_key("pBoundaryDataArray") )
  {
    dbase.put<std::vector<BoundaryData>* >("pBoundaryDataArray")=NULL;
  }
  
  // Keep a pointer to the array of BoundaryData
  dbase.get<std::vector<BoundaryData>* >("pBoundaryDataArray") = &boundaryData;


  return 0;
}


//\begin{>>OgesInclude.tex}{\subsection{setGridsToUse}} 
int Oges::
setGridsToUse( const IntegerArray & gridsToUse )
//==================================================================================
// /Description:
//   Only solve the equations on some grids, these are called the active grids. 
// If an active grid interpolates from an in-active grid, the corresponding 
// interpolation equation will be replaced by a Dirichlet condition (i.e. the identity equation)
// and the solution at that point will left unchanged. (Note that RHS will be altered at
// this interpolation point and set equal to the solution value at that point.)
//
// /gridsToUse (input) : a list of grids to use when solving. 
//             If this array is empty (i.e. a NULL array) then ALL grids will be used.
//              
//
//\end{OgesInclude.tex} 
//==================================================================================
{
  if( gridsToUse.getLength(0)==0 )
  {
    useAllGrids=true;
    useThisGrid.redim(0);
    return 0;
  }

  useThisGrid.redim(cg.numberOfComponentGrids());
  useThisGrid=false;
  for( int g=gridsToUse.getBase(0); g<=gridsToUse.getBound(0); g++ )
  {
    int grid = gridsToUse(g);
    if( grid>=0 && grid<cg.numberOfComponentGrids() )
     useThisGrid(grid)=true;
    else
    {
      printf("Oges::setGridsToUse:ERROR: gridsToUse(%i)=%i is not a valid grid, numberOfComponentGrids=%i\n",
	     g,grid,cg.numberOfComponentGrids());
    }
  }
  useAllGrids=min(useThisGrid)==1;

  return 0;
}

// ======================================================================================================
/// /brief Set the name for the composite grid. 
///  /param name (input) : name for the composite grid (used for labels for e.g.)
// ======================================================================================================
void Oges::
setGridName( const aString & name )
{
  gridName=name;
}

// ======================================================================================================
/// /brief Set the name of this instance of Ogmg (for info in debug files etc.)
///  /param name (input) : name for this instnace of Ogmg.
// ======================================================================================================
void Oges::
setSolverName( const aString & name )
{
  solverName=name;
}

//\begin{>>OgesInclude.tex}{\subsection{activeGrid}}
bool Oges::
activeGrid( int grid ) const
//==================================================================================
// /Description:
//   Return true if this grid is used.
//
// /grid (input) : grid to check
// /Return value (output): true if this grid is active (used)
//
//\end{OgesInclude.tex} 
//==================================================================================
{
  return useAllGrids || useThisGrid(grid);
}

//\begin{>>OgesInclude.tex}{\subsection{getUseThisGrid}}
const IntegerArray & Oges::
getUseThisGrid() const 
//==================================================================================
// /Description:
//   Return the array that indicates which grids are active, useThisGrid(grid)=true if the grid is active
//
// /Return value (output): a reference to useThisGrid.
//
//\end{OgesInclude.tex} 
//==================================================================================
{
  return useThisGrid;
}


// ===========================================================================================================
/// \brief !Define a predefined equation ** this is new ****
///  \param equation : defines the equation. If you are supplying a coefficient matrix as a grid function
///    then equation==userDefined. If you want to solve a predefined equation choose equation==laplaceEquation
///    or ...
///  \param boundaryConditions(side,axis,grid) : defines boundary conditions as
///         dirichlet, neumann, mixed, dirichletAndEvenSymmetry, dirichletAndOddSymmetry, extrapolate (defined by the BoundaryConditionEnum).
///  \param bcData(0:*,side,axis,grid) : for a mixed boundary condition a(0)=bcData(0,side,axis,grid).
///        For extrapolation, orderOfExtrapolation=bcData(0,side,axis,grid) where 0 means use the default
///        of orderOfExtrapolation=orderOfAccuracy+1
///  \param varCoeff (input) : optional input for variable diffusion s(x) in variableHeatEquationOperator,
///               divScalarGradHeatEquationOperator or advectionDiffusionEquationOperator
///  \param advectionCoeff (input) : optional input for variable coefficient a(x) in advectionDiffusionEquationOperator
///           advectionCoeff(I1,I2,I3,n)
///
// ===========================================================================================================
int Oges::
setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, 
                                  CompositeGridOperators & op,
				  const IntegerArray & boundaryConditions_,
				  const RealArray & bcData_, 
                                  RealArray & constantCoeff,
				  realCompositeGridFunction *varCoeff /* =NULL */,
				  realCompositeGridFunction *advectionCoeff /* =NULL */ )
{
  equationToSolve=equation;
  boundaryConditions.redim(0);
  boundaryConditions=boundaryConditions_;
  bcData.redim(0);
  bcData=bcData_;
//   printf("**** Oges::setEquationAndBoundaryConditions *****\n");
//   boundaryConditions.display("boundaryConditions");
//   bcData.display("bcData");
  
  const int orderOfAccuracy = op.getOrderOfAccuracy();
  const int width = orderOfAccuracy+1;  // 3 or 5
  stencilSize=int( pow(width,cg.numberOfDimensions())+1 );


  constantCoefficients=constantCoeff;
  if( equationSolver[parameters.solver]==NULL )
    buildEquationSolvers(parameters.solver);  

  if( parameters.solver==OgesParameters::multigrid )
  {
    // The multigrid solver knows how to handle this
    equationSolver[parameters.solver]->setEquationAndBoundaryConditions(equation,op,boundaryConditions_,bcData,
                          constantCoeff,varCoeff);
  }
  else
  {
    // build the coeff matrix for the predefined equations here.

    const int numberOfGhostLines=(width-1)/2;
    assert( numberOfGhostLines==1 || numberOfGhostLines==2 );

    Range all;
    coeff.updateToMatchGrid(cg,stencilSize,all,all,all);
    coeff=0.;  // this is needed ** fix *** only zero out ghost
    
    coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  

    op.setStencilSize(stencilSize);
    
    coeff.setOperators(op);

    Index I1,I2,I3;
    Index Ib1,Ib2,Ib3;
    Range M=stencilSize;
    BoundaryConditionParameters bcParams;
    bcParams.a.redim(2);
    BoundaryConditionParameters extrapParams;
    if( parameters.orderOfExtrapolation==-1 )
      extrapParams.orderOfExtrapolation=orderOfAccuracy+1;  // default order of extrapolation
    else
      extrapParams.orderOfExtrapolation=parameters.orderOfExtrapolation;

    bool isSingular=true;  // keep track if the equations are singular (all neumann BC's)

    int side,axis;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      if( !c.isRectangular() )
      {
	c.update(MappedGrid::THEvertex );
      }

      int md; // diagonal term
      if( c.numberOfDimensions()==2 )
	md=(width*width)/2; // 4 or 12 ;
      else if( c.numberOfDimensions()==3 )
	md=(width*width*width)/2; // 13 or 62;
      else
	md=width/2; // 1;

      if( parameters.isAxisymmetric )
      {
        assert( equation==OgesParameters::laplaceEquation ||
                equation==OgesParameters::heatEquationOperator );  // fix this 
	if( debug & 2 )
	  printF("***Oges: predefined axisymmetric equation ****\n");

      }


      // First form the Laplace operator : and add any axisymmetric corrections
      if( equation==OgesParameters::laplaceEquation ||
          equation==OgesParameters::heatEquationOperator )
      {
         op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid]);

         if( parameters.isAxisymmetric )
	 {
	   // add on corrections for a cylindrically symmetric problem
	   // Delta p = p.xx + p.yy + (1/y) p.y
	   // note that p.y=0 on y=0 and p.y/y = p.yy at r=0

           const bool isRectangular = c.isRectangular();
	   
           #ifdef USE_PPP
  	     intSerialArray maskLocal;   getLocalArrayWithGhostBoundaries(c.mask(),maskLocal);
	     realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff[grid],coeffLocal);
	     realSerialArray xLocal; if( !isRectangular ) getLocalArrayWithGhostBoundaries(c.vertex(),xLocal);
	   #else
             const intSerialArray & maskLocal = c.mask();
             const realSerialArray & coeffLocal = coeff[grid];
             const realSerialArray & xLocal = c.vertex();
           #endif
	   
	   getIndex(c.dimension(),I1,I2,I3);
           bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,I1,I2,I3);
	   if( ok )
	   {
	     realSerialArray radiusInverse(I1,I2,I3);  
	     if( c.isRectangular() )
	     {
	       real dx[3],xab[2][3];
               #define YY(i2) (xab[0][1]+dx[1]*(i2-i2a))
	       c.getRectangularGridParameters( dx, xab );
	       const int i2a=c.gridIndexRange(0,1);
	       for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	       {
		 radiusInverse(I1,i2,I3)=1./max(REAL_MIN,YY(i2));
	       }
               #undef YY
	     }
	     else
	     {
	       radiusInverse=1./max(REAL_MIN,xLocal(I1,I2,I3,axis2));  
	     }
	     ForBoundary(side,axis)
	     {
	       if( boundaryConditions(side,axis,grid)==OgesParameters::axisymmetric )
	       {
		 if( debug & 2 ) 
		   printF("***Oges: predefined axisymmetric boundary grid,side,axis=%3i %2i %2i \n",grid,side,axis);
	    
		 getBoundaryIndex( c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); 
		 bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,Ib1,Ib2,Ib3);
		 if( ok )
		 {
		   realSerialArray yyCoeff(M,Ib1,Ib2,Ib3);
		   yyCoeff=0.;
		   op[grid].assignCoefficients(MappedGridOperators::yyDerivative,yyCoeff,Ib1,Ib2,Ib3,0,0);
		   // coeff[grid](M,Ib1,Ib2,Ib3)+=op[grid].yyCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3); // add p.yy on axis
		   coeffLocal(M,Ib1,Ib2,Ib3)+=yyCoeff;   // add p.yy on axis
		   radiusInverse(Ib1,Ib2,Ib3)=0.;  // this will remove p.y/y term below from boundary
		 }
	       }
	     }
	     // add p.y/y term

	     // ::display(coeff[grid],"Oges: coeff before adding Dy/y","%6.2f ");

	     realSerialArray yCoeff(M,I1,I2,I3);
	     yCoeff=0.;
	     op[grid].assignCoefficients(MappedGridOperators::yDerivative,yCoeff,I1,I2,I3,0,0);
	     radiusInverse.reshape(1,I1,I2,I3);
	     // *wdh* 040228 for( int m=M.getBase(); m<=M.getBound()-1; m++ )
	     for( int m=M.getBase(); m<=M.getBound(); m++ )
	       yCoeff(m,I1,I2,I3)*=radiusInverse(0,I1,I2,I3);

	     coeffLocal(M,I1,I2,I3)+=yCoeff;
	
	     // ::display(yCoeff,"Oges: Dy/y before BC","%6.2f ");
	     // ::display(coeff[grid],"Oges: coeff before BC","%6.2f ");

	   }
	 } // end if isAxisymmetric

      }
      

      if( equation==OgesParameters::laplaceEquation )
      {
	// done above: op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid]); 
      }
      else if( equation==OgesParameters::heatEquationOperator )
      {
        // Solve constCoeff(0,grid)*I +constCoeff(1,grid)*Laplacian 

        // done above: op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid]);
        coeff[grid]*=constantCoeff(1,grid);
	coeff[grid](md,all,all,all)+=constantCoeff(0,grid);

	isSingular=isSingular && constantCoeff(0,grid)==0.;
	
      }
      else if( equation==OgesParameters::divScalarGradOperator )
      {
        assert( varCoeff!=NULL );
        realMappedGridFunction & variableCoeff = (*varCoeff)[grid];
        op[grid].coefficients(MappedGridOperators::divergenceScalarGradient,coeff[grid],variableCoeff);

      }
      else if( equation==OgesParameters::variableHeatEquationOperator )
      {
        // I + s(x)*Delta
        assert( varCoeff!=NULL );
        realMappedGridFunction & variableCoeff = (*varCoeff)[grid];
        const realArray & var = variableCoeff;
	realArray & coeffa = coeff[grid];
	
        op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid],variableCoeff ); 

        multiply(coeff[grid],variableCoeff);
	
//          getIndex(mg.dimension(),I1,I2,I3);
//  	const int stencilSize=coeffa.getLength(0)-1;
//  	for( int m=0; m<stencilSize; m++ )
//  	  coeffa(m,I1,I2,I3)*=var(I1,I2,I3);
	
        coeffa(md,all,all,all)+=1.;  // add the Identity.

	isSingular=false;
	
      }
      else if( equation==OgesParameters::divScalarGradHeatEquationOperator )
      {
        // I + div( s(x) grad )
        assert( varCoeff!=NULL );
        realMappedGridFunction & variableCoeff = (*varCoeff)[grid];
        op[grid].coefficients(MappedGridOperators::divergenceScalarGradient,coeff[grid],variableCoeff);

	realArray & coeffa = coeff[grid];
        coeffa(md,all,all,all)+=1.; // add the Identity.

	isSingular=false;
      }
      else if( equation==OgesParameters::advectionDiffusionEquationOperator )
      {
        // I + a(x).grad + div( s(x) grad )

        assert( varCoeff!=NULL );
        realMappedGridFunction & variableCoeff = (*varCoeff)[grid];
        op[grid].coefficients(MappedGridOperators::divergenceScalarGradient,coeff[grid],variableCoeff);

        assert( advectionCoeff!=NULL );
        realMappedGridFunction & advCoeff = (*advectionCoeff)[grid];

        OV_GET_SERIAL_ARRAY(real,coeff[grid],coeffLocal);
        OV_GET_SERIAL_ARRAY(real,advCoeff,advCoeffLocal);
        OV_GET_SERIAL_ARRAY(int,c.mask(),maskLocal);

        coeffLocal(md,all,all,all)+=1.; // add the Identity.

	getIndex(c.dimension(),I1,I2,I3);
	bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,I1,I2,I3);
	if( ok )
	{
	  realSerialArray xCoeff(M,I1,I2,I3);
          advCoeffLocal.reshape(1,advCoeffLocal.dimension(0),advCoeffLocal.dimension(1),advCoeffLocal.dimension(2),advCoeffLocal.dimension(3));
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    xCoeff=0.;
            MappedGridOperators::derivativeTypes derivType = MappedGridOperators::derivativeTypes(MappedGridOperators::xDerivative+dir);
	    op[grid].assignCoefficients(derivType,xCoeff,I1,I2,I3,0,0);  // coefficients for x, y or z derivative operator

	    for( int m=M.getBase(); m<=M.getBound(); m++ )
	      xCoeff(m,I1,I2,I3)*=advCoeffLocal(0,I1,I2,I3,dir);

	    coeffLocal(M,I1,I2,I3)+=xCoeff;
	  }
	  advCoeffLocal.reshape(advCoeffLocal.dimension(1),advCoeffLocal.dimension(2),advCoeffLocal.dimension(3),advCoeffLocal.dimension(4));
	}
	
	isSingular=false;
      }
      else
      {
        printf("Oges::setEquationAndBoundaryConditions::ERROR: unknown equation=%i \n",(int)equation);
        OV_ABORT("--OGES-- ERROR");
      }
      
      

//       if( orderOfAccuracy==4 )
//       {  
//         // **** do this for now **** fix for Neumann.
//         // extrap 2nd ghost line 
// 	extrapParams.ghostLineToAssign=2;
// 	coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries,extrapParams); 
// 	extrapParams.ghostLineToAssign=1;
	
//       }

      ForBoundary(side,axis)
      {
	  
	if( boundaryConditions(side,axis,grid)==OgesParameters::dirichlet ||
            boundaryConditions(side,axis,grid)==OgesParameters::dirichletAndEvenSymmetry ||
            boundaryConditions(side,axis,grid)==OgesParameters::dirichletAndOddSymmetry )
	{
          isSingular=false;
	  coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,BCTypes::boundary1+side+2*axis);
          if(  boundaryConditions(side,axis,grid)==OgesParameters::dirichlet )
	  {
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,extrapParams);
	  }
	  else if( boundaryConditions(side,axis,grid)==OgesParameters::dirichletAndEvenSymmetry )
	  {
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,BCTypes::boundary1+side+2*axis);
	  }
	  else
	  {
            coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::oddSymmetry,BCTypes::boundary1+side+2*axis);
	  }
	}
	else if( boundaryConditions(side,axis,grid)==OgesParameters::extrapolate )
	{
          // *wdh* new 100814 : look for the order of extrapolation to be optionally specified.
          int orderOfExtrapolation=int( bcData(0,side,axis,grid) +.5 );
          if( orderOfExtrapolation<=0 || orderOfExtrapolation>100 )
	  {
            orderOfExtrapolation=orderOfAccuracy+1;
	  }
	  extrapParams.orderOfExtrapolation=orderOfExtrapolation;
	  // printF("Oges:predefined: extrap BC: orderOfExtrapolation=%i\n",orderOfExtrapolation);

	  coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,extrapParams); 
	}
	else if( boundaryConditions(side,axis,grid)==OgesParameters::neumann || 
		 boundaryConditions(side,axis,grid)==OgesParameters::axisymmetric )
	{
	  // printF("XXXXX OGES: Set a Neumann BC (side,axis,grid)=(%i,%i,%i)\n",side,axis,grid);
	  coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::boundary1+side+2*axis);
	}
	else if( boundaryConditions(side,axis,grid)==OgesParameters::mixed )
	{
	  bcParams.a(0)=bcData(0,side,axis,grid);  // coeff of u
	  bcParams.a(1)=bcData(1,side,axis,grid);  // coeff of du/dn
          // printf("*** Oges: mixed BC: %f*u+%f*u.n : for grid=%i side=%i axis=%i\n",bcParams.a(0),bcParams.a(1),
          //    grid,side,axis );
	  
          if( bcParams.a(0)==0. && bcParams.a(1)==0. )
	  {
	    printf("Oges::setEquationAndBoundaryConditions::ERROR: mixed BC a0*u+a1*u.n has a0 and a1 equal to zero\n"
		   "for grid=%i side=%i axis=%i. Setting a0=1 and a1=1\n", grid,side,axis );
            bcParams.a(0)=1.;
	    bcParams.a(1)=1.;
	  }
          if( bcParams.a(0)==0. && bcParams.a(1)==1. )
	  {
            // printf("**** Oges::setEquationAndBoundaryConditions:: set neumann instead of mixed. a(0)=%f\n",bcParams.a(0));
    	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::boundary1+side+2*axis);
	  }
          else if( bcParams.a(0)==1. && bcParams.a(1)==0. )
	  {
            // printf("**** Oges::setEquationAndBoundaryConditions:: set dirichlet instead of mixed. a(1)=%f\n",bcParams.a(0));
	    isSingular=false;
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,BCTypes::boundary1+side+2*axis);
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,
                                                           extrapParams);
            
	  }
	  else if( bcParams.a(1)!=0. )
	  {
	    // *wdh* 080619 isSingular=bcParams.a(0)==0.;
	    isSingular=isSingular && bcParams.a(0)==0.;
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,BCTypes::boundary1+side+2*axis,bcParams);
	  }
          else
	  {
	    printf("Oges::setEquationAndBoundaryConditions::ERROR: mixed BC a0*u+a1*u.n has a0=%e and a1=%e "
		   "for grid=%i side=%i axis=%i is really a Dirichlet like BC. \n" 
		   " I do not know how to handle a Dirichlet BC with coefficient a0 not equal to 1. \n",
                   bcParams.a(0),bcParams.a(1),grid,side,axis );
	    Overture::abort("Oges:ERROR: option not implemented");
	  }
	  
	}
	else if( boundaryConditions(side,axis,grid)>0 )
	{
	  printf("Oges::setEquationAndBoundaryConditions::ERROR: unknown bc=%i for grid=%i side=%i axis=%i\n",
		 boundaryConditions(side,axis,grid),grid,side,axis);
	  Overture::abort("error");
	}

        // *** assign the 2nd ghost line **** *wdh* 051017
	if( orderOfAccuracy==4 )
	{
          extrapParams.ghostLineToAssign=2;
          if( boundaryConditions(side,axis,grid)==OgesParameters::dirichletAndEvenSymmetry )
	  {
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,BCTypes::boundary1+side+2*axis,
							   extrapParams);
	  }
	  else if( boundaryConditions(side,axis,grid)==OgesParameters::dirichletAndOddSymmetry )
	  {
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::oddSymmetry,BCTypes::boundary1+side+2*axis,
							   extrapParams);
	  }
	  else
	  {
	    coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,
							   extrapParams);
	  }
	  extrapParams.ghostLineToAssign=1;

	}
	
      }  // end ForBoundary
      
    } // end for grid

    // This next call will update parallel ghost *new* 100415
    coeff.finishBoundaryConditions();


    if( isSingular )
    {
      if( Oges::debug & 2 ) printf("+++++++++++Oges::setEquationAndBoundaryConditions:: equation is singular\n");
      set(OgesParameters::THEcompatibilityConstraint,true);
    }
    else
    {
      if( Oges::debug & 2 ) printf("+++++++++++Oges::setEquationAndBoundaryConditions:: equation is NOT singular\n");
      set(OgesParameters::THEcompatibilityConstraint,false);
    }
    
    updateToMatchGrid(cg); // this will initialize and setup extra equations for singular problems.
  }

  
  return 0;
}


//===============================================================================
/// \brief return the number of iterations used in the last solve.
//===============================================================================
int Oges::
getNumberOfIterations()
{
  if( equationSolver[parameters.solver]!=NULL )
  {
    numberOfIterations = equationSolver[parameters.solver]->getNumberOfIterations();
  }

  return numberOfIterations;
}

int Oges::
getCompatibilityConstraint() const
{
  return parameters.compatibilityConstraint;
}

//\begin{>>OgesInclude.tex}{\subsection{getMaximumResidual}} 
real Oges::
getMaximumResidual() const
//==================================================================================
// /Description:
//   Return the maximum resdiual from the last solve.
//\end{OgesInclude.tex} 
//==================================================================================
{
  if( equationSolver[parameters.solver]!=NULL )
    return equationSolver[parameters.solver]->getMaximumResidual();
  else
    return 0.;
}



// This next macro can be used to get or put stuff to the dataBase.
#define GET_PUT_LIST(getPut)  \
  uLinearized.getPut(subDir,"uLinearized" );  \
  rightNullVector.getPut(subDir,"rightNullVector" );  \
  int temp;\
  subDir.getPut(preconditionBoundary,"preconditionBoundary" );  \
  subDir.getPut(preconditionRightHandSide,"preconditionRightHandSide" );  \
  temp=coefficientType; subDir.getPut(temp,"coefficientType" ); \
       coefficientTypes & ct=(coefficientTypes&)coefficientType; /* <-cast away const */ ct=(coefficientTypes)temp; \
  coeff.getPut(subDir,"coeff" );  \
/*   subDir.getPut(compatibilityConstraint,"compatibilityConstraint" ); */ \
  subDir.getPut(numberOfIterations,"numberOfIterations" );  \
  /* no: ul,fl */ \
  subDir.getPut(numberOfExtraEquations,"numberOfExtraEquations" );  \
  subDir.getPut(extraEquationNumber,"extraEquationNumber" );  \
  subDir.getPut(machineEpsilon,"machineEpsilon" );  \
  subDir.getPut(initialized,"initialized" );  \
  subDir.getPut(numberOfGrids,"numberOfGrids" );  \
  subDir.getPut(numberOfDimensions,"numberOfDimensions" );  \
  subDir.getPut(numberOfComponents,"numberOfComponents" );  \
  subDir.getPut(refactor,"refactor" );  \
  subDir.getPut(reorder,"reorder" );  \
  subDir.getPut(evaluateJacobian,"evaluateJacobian" );  \
  subDir.getPut(numberOfEquations,"numberOfEquations" );  \
  subDir.getPut(numberOfNonzerosBound,"numberOfNonzerosBound" );  \
  subDir.getPut(numberOfNonzeros,"numberOfNonzeros" );  \
  subDir.getPut(sparseFormat,"sparseFormat" );  \
  subDir.getPut(storageFormat,"storageFormat" );  \
  subDir.getPut(peqn,"peqn" );  \
  subDir.getPut(stencilLength,"stencilLength" );  \
  subDir.getPut(i1v,"i1v" );  \
  subDir.getPut(kv,"kv" );  \
  /*  no: sol,rhs */ \
  subDir.getPut(ndia,"ndia" );  \
  subDir.getPut(ndja,"ndja" );  \
  subDir.getPut(nda,"nda" );  \
  subDir.getPut(ia,"ia" );  \
  subDir.getPut(ja,"ja" );  \
  subDir.getPut(a,"a" );  \
  subDir.getPut(ias,"ias" );  \
  subDir.getPut(jas,"jas" );  \
  subDir.getPut(rhsii,"rhsii" );  \
  subDir.getPut(as,"as" );  \
  subDir.getPut(vii,"vii" );  \
  subDir.getPut(resii,"resii" );  \
  subDir.getPut(nsp,"nsp" );  \
  subDir.getPut(rsp,"rsp" );  \
  subDir.getPut(yaleExcessWorkSpace,"yaleExcessWorkSpace" );  \
  subDir.getPut(ndiwk,"ndiwk" );  \
  subDir.getPut(ndwk,"ndwk" );  \
  subDir.getPut(iwk,"iwk" );  \
  subDir.getPut(wk,"wk" );  \
  subDir.getPut(bpcNumberOfEquations,"bpcNumberOfEquations" );  \
  subDir.getPut(bpcNumberOfNonzeros,"bpcNumberOfNonzeros" );  \
  subDir.getPut(bpciep,"bpciep" );  \
  subDir.getPut(bpciap,"bpciap" );  \
  subDir.getPut(bpcjap,"bpcjap" );  \
  subDir.getPut(bpcap,"bpcap;" );  \
  subDir.getPut(bpcrhsp,"bpcrhsp" );  \
  subDir.getPut(iep,"iep" );  \
  subDir.getPut(iap,"iap" );  \
  subDir.getPut(jap,"jap" );  \
  subDir.getPut(ap,"ap" );  \
  subDir.getPut(rhsp,"rhsp" );  \
  subDir.getPut(iabpc,"iabpc" );  \
  subDir.getPut(iwkbpc,"iwkbpc" );  \
  subDir.getPut(iab,"iab" );  \
  subDir.getPut(jab,"jab" );  \
  subDir.getPut(ia1,"ia1" );  \
  subDir.getPut(ja1,"ja1" );  \
  subDir.getPut(iep0,"iep0" );  \
  subDir.getPut(iap0,"iap0" );  \
  subDir.getPut(jap0,"jap0" );  \
  subDir.getPut(iepi,"iepi" );  \
  subDir.getPut(rwkbpc,"rwkbpc" );  \
  subDir.getPut(ab,"ab" );  \
  subDir.getPut(a1,"a1" );  \
  subDir.getPut(ap0,"ap0" ); \
  temp=equationToSolve; subDir.getPut(temp,"equationToSolve"); \
       EquationEnum & et = (EquationEnum) equationToSolve; /* <-cast away const */ et=(EquationEnum)temp;  \
  subDir.getPut(constantCoefficients,"constantCoefficients");  \
  subDir.getPut(boundaryConditions,"boundaryConditions"); \
  subDir.getPut(bcData,"bcData"); 




//\begin{>>OgesInclude.tex}{\subsection{get}} 
int Oges::
get( const GenericDataBase & dir, const aString & name)
//==================================================================================
// /Description:
//   Get a copy of Oges from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of Oges on the database.
//\end{OgesInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Oges");

  subDir.setMode(GenericDataBase::streamInputMode);

  // AAGET_PUT_LIST(get);

  return 0;
}

//\begin{>>OgesInclude.tex}{\subsection{put}} 
int Oges::
put( GenericDataBase & dir, const aString & name) const   
//==================================================================================
// /Description:
//   Output an image of Oges to a data base. 
// /dir (input): put onto this directory of the database.
// /name (input): the name of Oges on the database.
//\end{OgesInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Oges");                      // create a sub-directory 

  subDir.setMode(GenericDataBase::streamOutputMode);

  // AAGET_PUT_LIST(put);
  
  return 0;
}

//==================================================================================
/// \brief Return the number of extra equations. 
//==================================================================================
int Oges::
getNumberOfExtraEquations() const 
{
  return numberOfExtraEquations;
}

//==================================================================================
/// \brief set the number of extra equations. 
//==================================================================================
int Oges::
setNumberOfExtraEquations( int numExtra )
{
  numberOfExtraEquations=numExtra;
  return 0;
}

//==================================================================================
/// \brief return a list of the extra equation numbers 
//==================================================================================
const IntegerArray & Oges::
getExtraEquationNumbers() const
{
  return extraEquationNumber;
}


//===========================================================================================
/// \brief Set extra equations and over-ride other equations (using compressed row format)
///
/// \param neq (input): number of equatins defined.
/// \param eqn(m), m=0,1,...,neq-1 : equation number (i.e. "i" in the matrix a(i,j)
/// \param ia,ja,a : define "j" and a values
///
/// Equations are defined by: 
///   for m=0,1,...,neq-1
///      i=eqn(m)
///      for k=ia(m),...ia(m+1)-1
///        j=ja(k)
///        a_ij=a(k) 
///
//===========================================================================================
int Oges::
setEquations( int neq, IntegerArray & eqn, IntegerArray & ia, IntegerArray & ja, RealArray & a )
{
  if( neq>0 )
    parameters.userSuppliedEquations=true; 
  else
    parameters.userSuppliedEquations=false; 
  
  OgesExtraEquations & extraEquations=dbase.get<OgesExtraEquations>("extraEquations");

  extraEquations.neq=neq;
  extraEquations.eqn.reference(eqn);
  extraEquations.ia.reference(ia);
  extraEquations.ja.reference(ja);
  extraEquations.a.reference(a);

  return 0;
}


int Oges::
setExtraEquationValues( realCompositeGridFunction & f, real *value )
//==================================================================================
// /Description:
//   Assign values to the right-hand-side for the extra equations
//
// /f (input/output) : fill in rhs values here
// /value[i] (input) : values for each extra equation, i=0,1,2,...,
// 
// /Return values: 0=success
// /Author: wdh
//\end{OgesInclude.tex} 
//==================================================================================
{
  assert( equationSolver[parameters.solver]!=NULL );
  return equationSolver[parameters.solver]->setExtraEquationValues(f,value);
}

//\begin{>>OgesInclude.tex}{\subsection{setExtraEquationValues}} 
int Oges::
getExtraEquationValues( const realCompositeGridFunction & u, real *value )
//==================================================================================
// /Description:
//   Return solution values from the extra equations
//
// /u(input) : grid function holding the solution.
// /value[i] (output) : values for each extra equation, i=0,1,2,...,
// 
// /Return values: 0=success
// /Author: wdh
//\end{OgesInclude.tex} 
//==================================================================================
{
  assert( equationSolver[parameters.solver]!=NULL );
  return equationSolver[parameters.solver]->getExtraEquationValues(u,value);
}


//\begin{>>OgesInclude.tex}{\subsection{evaluateExtraEquation}} 
int Oges::
evaluateExtraEquation( const realCompositeGridFunction & u, real & value, int extraEquation /* =0 */ )
//==================================================================================
// /Description:
//    Evaluate the dot product of the coefficients of an extra equation times u 
//
// /u (input) : grid function to dot with the extra equation
// /value (output) : the dot product
// /extraEquation (input) : the number of the extra equation (0,1,...,numberOfExtraEquations-1)
// 
// /Return values: 0=success
// /Author: wdh
//\end{OgesInclude.tex} 
//==================================================================================
{
  assert( equationSolver[parameters.solver]!=NULL );
  return equationSolver[parameters.solver]->evaluateExtraEquation(u,value,extraEquation);
}


//\begin{>>OgesInclude.tex}{\subsection{evaluateExtraEquation}} 
int Oges::
evaluateExtraEquation( const realCompositeGridFunction & u, real & value, real & sumOfExtraEquationCoefficients,
                       int extraEquation /* =0 */ )
//==================================================================================
// /Description:
//    Evaluate the dot product of the coefficients of an extra equation times u 
//  Also return the sum of the coefficients of the extra equation (i.e. the dot product with the "1" vector)
//
// /u (input) : grid function to dot with the extra equation
// /value (output) : the dot product
// /sumOfExtraEquationCoefficients (output) : sum of the coefficients of the extra equation
// /extraEquation (input) : the number of the extra equation (0,1,...,numberOfExtraEquations-1)
// 
// /Return values: 0=success
// /Author: wdh
//\end{OgesInclude.tex} 
//==================================================================================
{
  assert( equationSolver[parameters.solver]!=NULL );
  return equationSolver[parameters.solver]->evaluateExtraEquation(u,value,sumOfExtraEquationCoefficients,
                                                                  extraEquation);
}




//\begin{>>OgesInclude.tex}{\subsection{writeMatrixToFile}} 
int Oges::
writeMatrixToFile( aString filename )
//==================================================================================
  // /Description:
  //   Write the current solver matrix (using indicies with base 1) to the file <fileName>.
  //   The file consists of triplets $i$, $j$, $A(i,j)$ (without commas) 
  //   for each non-zero element of the matrix. 
  //   (Here $i$=row, $j$=column, and 
  //    $A(i,j)=A_{ij}$ element of the matrix.) 
  // /Author: pf, wdh
//\end{OgesInclude.tex} 
//==================================================================================
{
  if( sparseStorageFormat==other )
  {
    printF("Oges::writeMatrixToFile:ERROR: Unable to write a matrix with sparseStorageFormat==other\n");
    return 0;
  }
  if( !initialized )
  {
    printF("Oges::writeMatrixToFile:ERROR: Unable to write a matrix before the matrix has been factored.\n");
    return 0;
  }
  

  //..OPEN FILE
  FILE *fpMatrix;
  if( !(fpMatrix = fopen( filename, "w"))) 
  {
    cout << "Oges::writeMatrixToFile ERROR::Couldn't open file `"<< filename
         << "' for writing.\n";
    return 0;
  }
  int neq=numberOfEquations;
//  int nnz=numberOfNonzeros;
  //oges.formMatrix(neq,nnz,Oges::compressedRow,allocateSpace,factorMatrixInPlace);

  real          *aval;        // local POINTERS to the matrix in Oges
  int           *ia_,*ja_;

  aval= a.getDataPointer();
  ia_=  ia.getDataPointer();
  ja_=  ja.getDataPointer();
  int irowm1, j,jcolm1;


  //..SAVE MATRIX
  //....The matrix should be in Compressed Sparse Row format
  //....Note that C-arrays start at 0, while matrices are
  //.....commonly numbered from 1
  //
  if( sparseStorageFormat==compressedRow )
  {
    for( irowm1=0; irowm1<neq; irowm1++ ) 
    {
      // rownorm=0.0;
      for( j=ia_[irowm1]; j<=ia_[irowm1+1]-1; j++ ) 
      {
	real v=aval[ j-1 ];
	jcolm1=ja_[ j-1 ]-1;

	int i=irowm1+1;
	int j=jcolm1+1;
	//printf(" %i    %i   %24.16f\n", i,j,v);
	fprintf(fpMatrix," %i    %i   %24.16f\n", i,j,v);
      } 
    }
  }
  else if( sparseStorageFormat==uncompressed )
  {
    // ** this doesn't seemt to work for Harwell since the initial matrix is not saved ***
    for( int k=0; k<numberOfNonzeros; k++) 
    {
      fprintf(fpMatrix," %i    %i   %24.16f\n", ia_[k],ja_[k],aval[k]);
    }
  }
  
  
  fclose(fpMatrix);
  return 1;
}

//\begin{>>OgesInclude.tex}{\subsection{outputSparseMatrix}} 
int Oges::
outputSparseMatrix( const aString & fileName )
//==================================================================================
// /Description:
//   Output the matrix in compressed row format OR uncompressed format (with indices starting at 0). 
// See the format below
// /fileName (input) : save the results to this file
//\end{OgesInclude.tex} 
//==================================================================================
{
  if( sparseStorageFormat==other )
  {
    printF("Oges::outputSparseMatrix:ERROR: Unable to write a matrix with sparseStorageFormat==other\n");
    return 0;
  }
  if( !initialized )
  {
    printF("Oges::outputSparseMatrix:ERROR: Unable to write a matrix before the matrix has been factored.\n");
    return 0;
  }

  FILE *file;
  if( !(file = fopen( fileName, "w"))) 
  {
    cout << "Oges::outputSparseMatrix: ERROR::Couldn't open file `"<< fileName
         << "' for writing.\n";
    return 0;
  }
  int neq=numberOfEquations;
  int nnz=numberOfNonzeros;

  real *a_ = a.getDataPointer();
  int *ia_ =  ia.getDataPointer();
  int *ja_=  ja.getDataPointer();

  // .................. output the matrix ................
  fprintf(file,"%i %i ",numberOfEquations,numberOfNonzeros);
  if( sparseStorageFormat==compressedRow )
  {
    for( int i=0; i<numberOfEquations+1; i++ )
      fprintf(file,"%i ",ia_[i]-1);
  }
  else if( sparseStorageFormat==uncompressed )
  {
    for( int i=0; i<numberOfNonzeros; i++ )
      fprintf(file,"%i ",ia_[i]-1);
  }
  
  for( int i=0; i<numberOfNonzeros; i++ )
    fprintf(file,"%i ",ja_[i]-1);
  
  for( int i=0; i<numberOfNonzeros; i++ )
    fprintf(file,"%20.16e ",a_[i]);
  
  fclose(file);

  return 1;
  
}


//\begin{>>OgesInclude.tex}{\subsection{writeMatrixGridInformationToFile}} 
int Oges::
writeMatrixGridInformationToFile( aString filename )
//==================================================================================
// /Description:
//   Write the grid information about the current solver matrix 
//   to the file <fileName>.
//   
// \begin{verbatim}
//   For each equation in the matrix, a line
//   is saved in the file with the following format:
//
//   ieq   grid    simpleClassify    fullClassify
//
// where:
//
// ieq=               equation number in the linear system
// grid=              grid number for this point
//
// (In the classify flags,  any non-negative value indicates a used 
//  point. Negative values are equations with zero for the rhs )
//
// simpleClassify=  
//                    -1=connecting grids (=interpolation, 
//                             extrapolation, or periodic bdry)
//                     0=hole point (unused)
//                     1=interior (=discretization) point
//                     2=boundary point 
//                        (=boundary, ghostline, periodic)
//
// fullClassify=      
//                     interior=       1,
//                     boundary=       2,
//                     ghost1=         3,
//                     ghost2=         4,
//                     ghost3=         5,
//                     ghost4=         6,
//                     interpolation= -1,
//                     periodic=-      2,
//                     extrapolation= -3,
//                     unused=         0
// \end{verbatim}
//
// /Author: pf
//\end{OgesInclude.tex} 
//============================================================================
{
  //..OPEN FILE
  FILE *fpMatrix;
  if( !(fpMatrix = fopen( filename, "w"))) 
  {
    cout << "Oges::writeMatrixGridInformationToFile ERROR::Couldn't open file `"<<filename
         << "' for writing.\n";
    return 0;
  }
  int neq=numberOfEquations;
//  int nnz=numberOfNonzeros;
  //oges.formMatrix(neq,nnz,Oges::compressedRow,allocateSpace,factorMatrixInPlace);

//  real          *aval;        // local POINTERS to the matrix in Oges
//  int           *ia_,*ja_;

//  aval= a.getDataPointer();
//  ia_=  ia.getDataPointer();
//  ja_=  ja.getDataPointer();

  for( int ieq=1; ieq<=neq; ieq++ )
  {
    int nn, i1,i2,i3,gridNumber;
    equationToIndex( ieq, nn, i1,  i2, i3, gridNumber );
    int ic=coeff[gridNumber].sparse->classify(i1,i2,i3,nn);
    //enum SparseRepForMGF::classifyTypes  classifyFlag
    //  =coeff[gridNumber].sparse->classify(i1,i2,i3,nn);
    SparseRepForMGF::classifyTypes  classifyFlag
      = (SparseRepForMGF::classifyTypes)coeff[gridNumber].sparse->classify(i1,i2,i3,nn);

    fprintf(fpMatrix," %8i  %8i   ",ieq,gridNumber);
    //,ic,i1,i2,i3,ieq);

    //..SimpleClassify: using a switch to ensure the enum can be changed
    int iSimpleClassify=-99; //=unknown
    switch( classifyFlag )  {
      case SparseRepForMGF::extrapolation: 
      case SparseRepForMGF::interpolation: 
      case SparseRepForMGF::periodic:
	iSimpleClassify=-1;
	break;
      case SparseRepForMGF::interior: 
	iSimpleClassify=1;
	break;
      case SparseRepForMGF::boundary: 
      case SparseRepForMGF::ghost1: 
      case SparseRepForMGF::ghost2: 
      case SparseRepForMGF::ghost3: 
      case SparseRepForMGF::ghost4:
	iSimpleClassify=2;
	break;
      case SparseRepForMGF::unused:
	iSimpleClassify=0;
	break;
    }

    //..SimpleClassify: using a switch to ensure the enum can be changed
    int iFullClassify=-99; //=unknown
    switch( classifyFlag ) {
      case SparseRepForMGF::extrapolation:
	iFullClassify=-3;
	break;
      case SparseRepForMGF::periodic:
	iFullClassify=-2;
	break;
      case SparseRepForMGF::interpolation:
	iFullClassify=-1;
	break;
      case SparseRepForMGF::interior: 
	iFullClassify=1;
	break;
      case SparseRepForMGF::boundary: 
	iFullClassify=2;
	break;
      case SparseRepForMGF::ghost1: 
	iFullClassify=3;
	break;
      case SparseRepForMGF::ghost2: 
	iFullClassify=4;
	break;
      case SparseRepForMGF::ghost3: 
	iFullClassify=5;
	break;
      case SparseRepForMGF::ghost4: 
	iFullClassify=6;
	break;
      case SparseRepForMGF::unused:
	iFullClassify=0;
	break;
    }

    fprintf(fpMatrix," %8i  %8i \n ", iSimpleClassify, iFullClassify);

  }
   fclose(fpMatrix);
   return 1;
}


//\begin{>>OgesInclude.tex}{\subsection{writePetscMatrixToFile}} 
int Oges::
writePetscMatrixToFile( aString filename,
			realCompositeGridFunction & u,
			realCompositeGridFunction & f)
//==================================================================================
// /Description:
//    Only available when linked with PETSc (-DOVERTURE\_USE\_PETSC)
//
//     Write the current solver matrix to the file <fileName>.
//     Uses the PETSc binary format. Supply u,f as to 'solver',
//     the RHS corresponding to f is also saved in the matrix file.
//
// /Author: pf
//\end{OgesInclude.tex} 
//==================================================================================
{
  OgesParameters::SolverEnum petsc = OgesParameters::PETSc;
  if( equationSolver[petsc]==NULL )  buildEquationSolvers(petsc);
  
  if( equationSolver[ petsc ]!=NULL )
  {

//    int neq, nnz; // number of equations & nonzeros, returned from Oges
    int rescaleFlag;
    get(OgesParameters::THErescaleRowNorms,rescaleFlag);
    set( OgesParameters::THErescaleRowNorms, true );

    equationSolver[petsc]->saveBinaryMatrix( filename, u, f );

    set( OgesParameters::THErescaleRowNorms, rescaleFlag );

    return 0;
  }
  else // if( parameters.solver!=SLAP )
  {
    printf("Oges::writePetscMatrixToFile:ERROR: PETSc is not available \n");
    Overture::abort("error");
    // return -1;
  }
  return 0;
}


//\begin{>>OgesInclude.tex}{\subsection{canSolveInPlace}} 
bool Oges::
canSolveInPlace() const
//=====================================================================================
// /Description:
//   Return true if the rhs and sol vectors can be the same.
//\end{OgesInclude.tex}
//=====================================================================================
{
  return ! ( isSolverIterative() || preconditionBoundary || 
             parameters.solveForTranspose );  // *wdh* 011214: added for Yale and transpose
}




//=====================================================================================
/// \brief Supply matrix coefficients and boundary conditions (new way).
///
/// \details This function should be used in the following fashion:
///       - call oges.setGrid(cg) to define a new grid. When using multigrid this will build the multigrid levels.
///       - call this function to define the coefficients and boundary conditions. Do NOT call oges.updateToMatchGrid(cg)
///       - call oges.solve(u,f) to solve the equations.
///   If the equations change but not the grid, call this function again with the new coefficients. If the grid has changed
///   then call both oges.setGrid() and this function again. 
/// 
/// \param coeff0 (input): Here are the coefficients. Oges will keep a reference to this
///  grid function. 
/// \param boundaryConditions(0:1,0:2,numberOfComponentGrids) (input) : optionally supply boundary conditions.
///  These are currently needed by the multigrid solver.
/// \param bcData : data for the boundary conditions.
//=====================================================================================
int Oges::
setCoefficientsAndBoundaryConditions( realCompositeGridFunction & coeff0,
				      const IntegerArray & boundaryConditions,
				      const RealArray & bcData )
{
  if( equationSolver[parameters.solver]==NULL )
    buildEquationSolvers(parameters.solver);  

  if( parameters.solver==OgesParameters::multigrid )
  {
    equationSolver[parameters.solver]->setCoefficientsAndBoundaryConditions( coeff0,boundaryConditions,bcData );
  }
  else
  {
    setCoefficientArray( coeff0,boundaryConditions,bcData);
    initialize(); 
    solverJob|=2;   // refactor
  }
  
  return 0;
}



//\begin{>>OgesInclude.tex}{\subsection{setCoefficientArray}}
int Oges::
setCoefficientArray( realCompositeGridFunction & coeff0,
		     const IntegerArray & boundaryConditions /* =Overture::nullIntArray() */,
		     const RealArray & bcData /* =Overture::nullRealArray() */ )
//=====================================================================================
// /Purpose: Supply a coefficient grid function to be used to discretize the equations.
// /coeff0 (input): Here are the coefficients. Oges will keep a reference to this
//  grid function. 
// /boundaryConditions(0:1,0:2,numberOfComponentGrids) (input) : optionally supply boundary conditions.
//  These are needed by the multigrid solver.
// /bcData : data for the boundary conditions.
//\end{OgesInclude.tex}
//=====================================================================================
{
  if( equationSolver[parameters.solver]==NULL )
    buildEquationSolvers(parameters.solver);  

  if( parameters.solver==OgesParameters::multigrid )
  {
    // The multigrid solver knows how to handle this
    equationSolver[parameters.solver]->setCoefficientArray( coeff0,boundaryConditions,bcData );
  }
  else
  {
    coeff.reference(coeff0);

    // printf("Oges::setCoefficientArray: coeff0.getIsACoefficientMatrix()=%i\n",coeff0.getIsACoefficientMatrix());
    // printf("Oges::setCoefficientArray: coeff.getIsACoefficientMatrix() =%i\n",coeff.getIsACoefficientMatrix());
  
    if( true || coeff.getIsACoefficientMatrix() )
    {
      int grid0=0;
      if( !useAllGrids )
      {
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  if( useThisGrid(grid) )
	  {
	    grid0=grid;
	    break;
	  }
	}
      }
      if( coeff[grid0].sparse!=NULL )
      {
	numberOfComponents=coeff[grid0].sparse->numberOfComponents;
	stencilSize=coeff[grid0].sparse->stencilSize;
	// printf("Oges::setCoefficientArray: stencilSize=%i\n",stencilSize);
      }
      else
      {
	printf("Oges::setCoefficientArray:ERROR: there is no sparseRep available with the coefficient matrix\n"
	       "                                 Maybe it is not a coefficient matrix?\n");
	Overture::abort("Oges::setCoefficientArray:ERROR");
      }
    }
    shouldBeInitialized=true;
  }
  
  return 0;
}

//\begin{>>OgesInclude.tex}{\subsection{setCoefficientArray}}
int Oges::
setCoefficientArray( realMappedGridFunction & coeff0,
		     const IntegerArray & boundaryConditions /* =Overture::nullIntArray() */,
		     const RealArray & bcData /* =Overture::nullRealArray() */ )
//=====================================================================================
// /Purpose: Supply a coefficient grid function (single grid only)
//  to be used to discretize the equations.
// /coeff0 (input): Here are the coefficients. Oges will keep a reference to this
//  grid function.
// /boundaryConditions(side,axis,grid) : optionally supply boundary conditions. These are needed
//   by the multigrid solver.
// /bcData : data for the boundary conditions.
//\end{OgesInclude.tex}
//=====================================================================================
{
  coeff.updateToMatchGrid(cg,Range(coeff0.getComponentBase(0),coeff0.getComponentBound(0)),
			  nullRange,nullRange,nullRange);
  coeff[0].reference(coeff0);
  if( coeff0.getIsACoefficientMatrix() )
  {
    numberOfComponents=coeff0.sparse->numberOfComponents;
    stencilSize=coeff0.sparse->stencilSize;
  }
  shouldBeInitialized=true;
  return 0;
}



//\begin{>>OgesInclude.tex}{\subsection{setEvaluateJacobian}}
void Oges::
setEvaluateJacobian( const int evaluateJacobian0 )
//=====================================================================================
// /Purpose: ?
//\end{OgesInclude.tex}
//=====================================================================================
{ 
  evaluateJacobian=evaluateJacobian0; 
}



//\begin{>>OgesInclude.tex}{\subsection{setGrid}}
void Oges::
setGrid( CompositeGrid & cg0, bool outOfDate /* =true */  )
//=====================================================================================
// /Purpose:
// Supply a CompositeGrid to Oges. Use this routine, for example,
// if an Oges object was created with the default constructor.
// Call this routine before calling initialize.
// /cg0 (input): Oges will keep a reference to this grid.
// /outOfDate : set to true if the grid is out of date. This is normally true except in the case
//    of using the multigrid solver in which case the multigrid hierachy only needs to be built once
//    so multiple instances of Oges need only mark the grid as out of date once. The multigrid hierarchy
//   may also be marked out of date if you mark the MultigridCompositeGrid that was optionally supplied to Oges.
//  You you do this then you can call setGrid with outOfDate=false in all cases.
//\end{OgesInclude.tex}
//=====================================================================================
{
  cg.reference(cg0);
  privateUpdateToMatchGrid();
  shouldBeInitialized=true;

  if( equationSolver[parameters.solver]==NULL )
    buildEquationSolvers(parameters.solver);  

  if( parameters.solver==OgesParameters::multigrid )
  {
    if( outOfDate )
    {
      printF(" Oges::setGrid: Setting  mgcg.setGridIsUpToDate(false);\n");
      mgcg.setGridIsUpToDate(false);  // mark the multgrid hierachy as out of date.
    }
    
    // The multigrid solver knows how to handle this
    equationSolver[parameters.solver]->setGrid(cg);
  }

}

//\begin{>>OgesInclude.tex}{\subsection{setGrid}}
void Oges::
setGrid( MappedGrid & mg, bool outOfDate /* =true */ )
//=====================================================================================
// /Purpose:
// Supply a MappedGrid to Oges. Use this routine, for example,
// if an Oges object was created with the default constructor.
// Call this routine before calling initialize.
// /mg (input): Oges will keep a reference to this grid.
//\end{OgesInclude.tex}
//=====================================================================================
{
  cg = CompositeGrid(mg.numberOfDimensions(),1);  // make a composite grid with 1 component grid
// ****  cg[0].reference(mg);
  cg[0]=mg;
  // *wdh* 990821 cg.update();
  privateUpdateToMatchGrid();
  shouldBeInitialized=true;

  if( equationSolver[parameters.solver]==NULL )
    buildEquationSolvers(parameters.solver);  

  if( parameters.solver==OgesParameters::multigrid )
  {
    if( outOfDate )
      mgcg.setGridIsUpToDate(false);  // mark the multgrid hierachy as out of date.
    
    // The multigrid solver knows how to handle this
    equationSolver[parameters.solver]->setGrid(cg);
  }
}

// =======================================================================================
/// \brief  Supply a MultigridCompositeGrid (for use with the multigrid solver Ogmg).
/// \param mgcg (input) : use this object to hold the multigrid hierarchy. 
/// \details The MultigridCompositeGrid object can be used to share a multigrid hierarchy 
///  amongst different applications and means that the coarse grid levels need only be
/// generated once. 
// =======================================================================================
void Oges::
set( MultigridCompositeGrid & mgcg_ )
{
  mgcg.reference(mgcg_);
}


//\begin{>>OgesInclude.tex}{\subsection{set(OptionEnum,int)}}
int Oges::
set( OgesParameters::OptionEnum option, int value /* = 0 */ )
//=====================================================================================
// /Description:
//   Set an option from the {\tt OgesParameters::OptionEnum} enumerator.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
// /option (input) : choose an option
// /value (input) : value to assign (for options requiring a value).
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.set(option,value);
}

//\begin{>>OgesInclude.tex}{\subsection{set(OptionEnum,float)}}
int Oges::
set( OgesParameters::OptionEnum option, float value )
//=====================================================================================
// /Description:
//   Set an option from the {\tt OgesParameters::OptionEnum} enumerator.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
// /option (input) : choose an option
// /value (input) : value to assign (for options requiring a value).
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.set(option,value);
}

//\begin{>>OgesInclude.tex}{\subsection{set(OptionEnum,double)}}
int Oges::
set( OgesParameters::OptionEnum option, double value )
//=====================================================================================
// /Description:
//   Set an option from the {\tt OgesParameters::OptionEnum} enumerator.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
// /option (input) : choose an option
// /value (input) : value to assign (for options requiring a value).
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.set(option,value);
}

//\begin{>>OgesInclude.tex}{\subsection{set(SolverEnum)}}
int Oges::
set( OgesParameters::SolverEnum option )
//=====================================================================================
// /Description:
//   Select a solver from the {\tt OgesParameters::SolverEnum} enumerator.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
// /option (input) : option selected.
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.set(option);
}

//\begin{>>OgesInclude.tex}{\subsection{set(SolverMethodEnum)}}
int Oges::
set( OgesParameters::SolverMethodEnum option )
//=====================================================================================
// /Description:
//   Select a solver method from the {\tt OgesParameters::SolverMethodEnum} enumerator.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
// /option (input) : option selected.
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.set(option);
}


//\begin{>>OgesInclude.tex}{\subsection{set(PreconditionerEnum)}}
int Oges::
set( OgesParameters::PreconditionerEnum option )
//=====================================================================================
// /Description:
//   Select a preconditioner from the {\tt OgesParameters::PreconditionerEnum} enumerator.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
// /option (input) : option selected.
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.set(option);
}

//\begin{>>OgesInclude.tex}{\subsection{set(MatrixOrderingEnum)}}
int Oges::
set( OgesParameters::MatrixOrderingEnum option )
//=====================================================================================
// /Description:
//   Select a matrix ordering from the {\tt OgesParameters::MatrixOrderingEnum} enumerator.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
// /option (input) : option selected.
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.set(option);
}

	   
	   		      
//\begin{>>OgesInclude.tex}{\subsection{get(OptionEnum,int\&)}}
int Oges::
get( OgesParameters::OptionEnum option, int & value ) const
//=====================================================================================
// /Description:
//  Return the current value of an option (this version appropriate for options that have
//  a value of type `int'.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.get(option,value);
}

//\begin{>>OgesInclude.tex}{\subsection{get(OptionEnum,real\&)}}
int Oges::
get( OgesParameters::OptionEnum option, real & value ) const
//=====================================================================================
// /Description:
//  Return the current value of an option (this version appropriate for options that have
//  a value of type `real'.
//  See section (\ref{sec:OgesParameters}) for a full description of the options available.
//\end{OgesInclude.tex}
//=====================================================================================
{
  return parameters.get(option,value);
}


//\begin{>>OgesInclude.tex}{\subsection{setOgesParameters}}
int Oges::
setOgesParameters( const OgesParameters & par )
//=====================================================================================
// /Description:
//    Assign the values from an OgesParameters object to an Oges object.
//\end{OgesInclude.tex}
//=====================================================================================
{
  parameters=par;
  
  return 0;
}

// ===============================================================================
/// \brief Set the MPI communicator used by Oges solvers (e..g. PETSc)
// ===============================================================================
int Oges::
setCommunicator( MPI_Comm & comm )
{
  return parameters.setCommunicator(comm);
}

// ===============================================================================
/// \brief Get the MPI communicator used by Oges solvers (e..g. PETSc)
// ===============================================================================
MPI_Comm& Oges::
getCommunicator()
{
  return parameters.getCommunicator();
}


//\begin{>>OgesInclude.tex}{\subsection{sizeOf}} 
real Oges:: 
sizeOf( FILE *file /* =NULL */ ) const 
//===================================================================================
// /Description: 
//   Return number of bytes allocated by Oges; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{OgesInclude.tex}
//===================================================================================
{
  real size=0.;

  real sizeOfThis=sizeof(*this);
  size+=sizeOfThis;
  
  real sizeOfCG=sizeof(cg);
  size+=sizeOfCG;
  
  real sizeOfOgesMatrix=ia.elementCount()*sizeof(int)+ja.elementCount()*sizeof(int)+a.elementCount()*sizeof(real);
  size+=sizeOfOgesMatrix;

  real sizeOfCoeff=coeff.sizeOf();
  size+=sizeOfCoeff;
  if( coeff.numberOfGrids()==0 && classify!=NULL )
  {
    for( int grid=0; grid<numberOfGrids; grid++ )
      size+=classify[grid].elementCount()*sizeof(int);
  }

  real sizeOfStuff = sol.elementCount()*sizeof(real)+rhs.elementCount()*sizeof(real)+
                     rightNullVector.sizeOf();
  size+=sizeOfStuff;
  
  real solverSize=0.;
  if( equationSolver[parameters.solver]!=NULL )
    solverSize=equationSolver[parameters.solver]->sizeOf(file);
  size+=solverSize;
  

  size=max(1.,size);

  if( file!=NULL )
  {
    fprintf(file,
	    "                                          Kbytes       %% \n"
	    " sizeof(*this).........................%11.1f  %5.2f\n"
            "   sizeof(cg) (no data)................%11.1f  %5.2f\n"
	    " Oges matrix (ia,ja,a).................%11.1f  %5.2f\n"
            " reference to coeff matrix.............%11.1f  %5.2f\n"
	    " Oges other stuff.(sol,rhs,null).......%11.1f  %5.2f\n"
            " sparse solver.........................%11.1f  %5.2f\n"
            " total counted.........................%11.1f  %5.2f\n"
            " bytes/unknown = %7.2f\n",
            sizeOfThis/1000.,100.*sizeOfThis/size,
            sizeOfCG/1000.,100.*sizeOfCG/size,
            sizeOfOgesMatrix/1000.,100.*sizeOfOgesMatrix/size,
            sizeOfCoeff/1000.,100.*sizeOfCoeff/size,
            sizeOfStuff/1000.,100.*sizeOfStuff/size,
            solverSize/1000.,100.*solverSize/size,
            size/1000.,100.,
            size/max(1,numberOfEquations));
  }
  return size;
}


// ============================================================================
/// \brief Print a description of the solver and options used.
// ============================================================================
int Oges::
printSolverDescription( const aString & label, FILE *file /* = stdout */ ) const
{
  parameters.printSolverDescription(label,file);
  equationSolver[parameters.solver]->printSolverDescription(label,file);

  return 0;
}


//\begin{>>OgesInclude.tex}{\subsection{printStatistics}} 
int Oges::
printStatistics(FILE *file /* = stdout */ ) const
//===================================================================================
// /Description:
//   Output any relevant statistics
//\end{>>OgesInclude.tex}
//===================================================================================
{
  if( equationSolver[parameters.solver]!=NULL )
    equationSolver[parameters.solver]->printStatistics(file);

  return 0;
}



//\begin{>>OgesInclude.tex}{\subsection{updateToMatchGrid}} 
int Oges::
updateToMatchGrid( CompositeGrid & cg0 )
//===================================================================================
// /Purpose: 
//    Give Oges a new matrix to use. Use this routine, for example, when a grid has
//    moved. 
//    This routine will cause the matrix to be refactored the next time solve is called.
//
// /cg0 (input): use this CompositeGrid
//\end{>>OgesInclude.tex}
//===================================================================================
{
  cg.reference(cg0);
  privateUpdateToMatchGrid();
  initialize(); 
  solverJob|=2;   // refactor
  return 0;
}

//\begin{>>OgesInclude.tex}{\subsection{updateToMatchGrid}} 
int Oges::
updateToMatchGrid( MappedGrid & mg )
//===================================================================================
// /Purpose:
//    Use this version when you are solving a problem on a MappedGrid.
// /mg (input): use this MappedGrid
//\end{>>OgesInclude.tex}
//===================================================================================
{
//  cg = CompositeGrid(mg.numberOfDimensions(),1);  // make a composite grid with 1 component grid
  cg.setNumberOfDimensionsAndGrids(mg.numberOfDimensions(), 1);
  cg[0].reference(mg);
  cg.updateReferences(); // wdh 970220

  cg.geometryHasChanged(CompositeGrid::EVERYTHING & ~MappedGrid::EVERYTHING);

  // *wdh* 990821 cg.update();
  privateUpdateToMatchGrid();
  initialize(); 
  solverJob|=2;   // refactor
  return 0;
}



void Oges:: 
privateUpdateToMatchGrid()
{
  numberOfGrids=cg.numberOfComponentGrids();
  numberOfDimensions=cg.numberOfDimensions();

}






void Oges::
equationToIndex( const int eqnNo0, int & n, int & i1, int & i2, int & i3, int & grid )
//=============================================================================
  // Convert an Equation Number to a point on a grid (Inverse of equationNo)
  // input -
  //  eqnNo0 : equation number
  // Output
  //  n : component number ( n=0,1,..,numberOfComponents-1 )
  //  i1,i2,i3 : grid indices
  //  grid : component grid number (grid=0,1,2..,numberOfCompoentGrids-1)   
  //=============================================================================
{
  int eqn=eqnNo0;
  grid=numberOfGrids-1;
  for( int grid1=1; grid1<numberOfGrids; grid1++ )
  {
    if( eqn <= gridEquationBase(grid1) )
    {
      grid=grid1-1;
      break;
    }
  }
  eqn-=(gridEquationBase(grid)+1);
  n= (eqn % numberOfComponents);
  eqn/=numberOfComponents;
  i1=(eqn % arraySize(grid,axis1))+arrayDims(grid,Start,axis1);
  eqn/=arraySize(grid,axis1);
  i2=(eqn % arraySize(grid,axis2))+arrayDims(grid,Start,axis2);
  eqn/=arraySize(grid,axis2);
  i3=(eqn % arraySize(grid,axis3))+arrayDims(grid,Start,axis3);

}


IntegerDistributedArray Oges::
equationNo(const int n, const Index & I1, const Index & I2, const Index & I3, 
			    const int grid )
{
  //=============================================================================
  // Return an IntegerArray of equation numbers for given indices
  //
  // Input -
  //  n : component number ( n=0,1,..,numberOfComponents-1 )
  //  I1,I2,I3 : Index objects defining the grid indices
  //  grid : component grid number (grid=0,1,2..,numberOfCompoentGrids-1)   
  // Output -
  //   Eqn(I1,I2,I3) : equation Numbers
  //=============================================================================

  IntegerDistributedArray Eqn(Range(I1.getBase(),I1.getBound()),
               Range(I2.getBase(),I2.getBound()),
	       Range(I3.getBase(),I3.getBound()));

  for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
  for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
  for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
  {
    Eqn(i1,i2,i3)=equationNo(n,i1,i2,i3,grid);
  }
  return Eqn;
}



int Oges::
update( GenericGraphicsInterface & gi, CompositeGrid & cGrid )
// =====================================================================================
// /Description:
//   Update parameters interactively.
// =====================================================================================
{
  return parameters.update(gi,cGrid);
}


//\begin{>>OgesInclude.tex}{\subsection{getMatrix}} 
int Oges::
getMatrix( IntegerArray & ia_, IntegerArray & ja_, RealArray & a_, 
           SparseStorageFormatEnum format /* =compressedRow */)
// =====================================================================================
// /Description:
//   Return the matrix in a given format.
// /ia\_,ja\_,a\_ (output) : reference to the matrix in sparse form.
// /format (input): sparse format
//\end{>>OgesInclude.tex}
// =====================================================================================
{
  if( !initialized || shouldBeInitialized )
  {
    if( Oges::debug & 1 ) cout << " *** Oges::getMatrix: initialize... ****\n";
    solverJob=7;  
  }
  
  int shouldUpdateMatrix= (solverJob & 2) || refactor==true;

  
  double timeBuild=getCPU();
  if( shouldUpdateMatrix ) 
  {
    if( Oges::debug & 1 ) cout << "...Build Matrix\n";
    //..build compressed-row matrix ia,ja,a

    // const int stencilSize = coeff[0].sparse->stencilSize;
    assert( stencilSize>0 );
    
    bool fillinRatioSpecified = parameters.fillinRatio>0;
    real fillinRatio=fillinRatioSpecified>0 ? parameters.fillinRatio : stencilSize+10;  // +5; *wdh* 971024
    if( !fillinRatioSpecified && parameters.compatibilityConstraint && numberOfDimensions==3 )
      fillinRatio+=5;
    parameters.fillinRatio=fillinRatio;

    bool allocateSpace = true;           // fix these ********************
    bool factorMatrixInPlace = false;

    int newNumberOfEquations, newNumberOfNonzeros;
    formMatrix(newNumberOfEquations,newNumberOfNonzeros,
                    format,allocateSpace,factorMatrixInPlace);

    
    numberOfEquations=newNumberOfEquations;
    numberOfNonzeros=newNumberOfNonzeros;

  }
  
  ia_.reference(ia);
  ja_.reference(ja);
  a_.reference(a);

  return 0;
}

  // compute y=A*x 
int Oges::
matrixVectorMultiply( int n, real *x, real *y )
{
  OGES_MAT_VECT( numberOfEquations,
               *x, *y, 
               numberOfNonzeros, 
               *ia.getDataPointer(),
               *ja.getDataPointer(),
               *a.getDataPointer() );

  return 0;
}

  // compute r=A*x-b
int Oges::
computeResidual( int n, real *x, real *b, real *r )
{
  OGES_RESIDUAL( numberOfEquations,
		*x, *b, *r,
		numberOfNonzeros, 
		*ia.getDataPointer(),
		*ja.getDataPointer(),
		*a.getDataPointer() );
  return 0;
}

// assign the values of the grid function u into the vector x
int Oges::
assignVector( int n, real *x, realCompositeGridFunction & u )
{
  return 0;
}

// store the vector x into the grid function u
int Oges::
storeVector( int n, real *x, realCompositeGridFunction & u )
{
  return 0;
}

