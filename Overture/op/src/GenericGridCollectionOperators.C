#include "GenericGridCollectionOperators.h"
#include "GridFunctionParameters.h"
#include "interpPoints.h"

#undef COMPOSITE_GRID_OPERATORS
// The next line is uncommented in GenericCompositeGridOperators.h
// define COMPOSITE_GRID_OPERATORS

void GenericGridCollectionOperators::
setup()
{
 setStencilSize( int(pow(3,gridCollection.numberOfDimensions())+1) );
 twilightZoneFlow=false;
 twilightZoneFlowFunction =NULL;
 
}


//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{Constructors}}  
GenericGridCollectionOperators::
GenericGridCollectionOperators()
//===========================================================================================
//\end{GenericGridCollectionOperatorsInclude.tex}
//===========================================================================================
{
  setup();
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{}
GenericGridCollectionOperators::
GenericGridCollectionOperators( GridCollection & g0 )
//=======================================================================================
// /Description:
//   Construct a GenericGridCollectionOperators
// /gridCollection0 (input): Associate this grid with the operators.
// /Author: WDH
//\end{GenericGridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  updateToMatchGrid( g0 );
  setup();
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{}
GenericGridCollectionOperators::
GenericGridCollectionOperators( GenericMappedGridOperators & op )
//=======================================================================================
// /Description:
//   Construct a GenericGridCollectionOperators using a MappedGridOperators
// /op (input): Associate this grid with these operators.
// /Author: WDH
//\end{GenericGridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  setup();
  mappedGridOperatorsPointer=&op;
}

//===========================================================================================
// This constructor takes a grid collection and a MappedGridOperators as input
//===========================================================================================
GenericGridCollectionOperators::
GenericGridCollectionOperators( GridCollection & g0, GenericMappedGridOperators & op)
{
  mappedGridOperatorsPointer=&op;
  updateToMatchGrid( g0 );
  setup();
}


//===========================================================================================
//  Copy constructor 
//   deep copy
//===========================================================================================
GenericGridCollectionOperators::
GenericGridCollectionOperators( const GenericGridCollectionOperators & gco ) 
{
  *this=gco;   // this uses the = operator which is a deep copy
}

GenericGridCollectionOperators::
~GenericGridCollectionOperators()
{
//  if( mappedGridOperatorWasNewed )
//    delete mappedGridOperatorsPointer;  // ** only delete if it was newed ***

  mappedGridOperators.deepClean();  // this deletes all the elements in the list
}

//=======================================================================================
//   virtualConstructor
//
//  This routine should create a new object of this class and return as a pointer
//  to the base classGenericGridCollectionOperators.
//
//  Notes:
//   o This routine is needed if this class has been derived from the base class GenericGridCollectionOperators
//   o This routine is used by the classe MultigridCompositeGridOperators
//     in order to construct lists of this class. These classes only know about the base class
//     and so they are unable to create a "new" version of this class
//=======================================================================================
GenericGridCollectionOperators* GenericGridCollectionOperators::
virtualConstructor()
{
  return new GenericGridCollectionOperators();
}



//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{get}}
int GenericGridCollectionOperators::
get( const GenericDataBase & dir, const aString & name)
//-------------------------------------------------------------------
// /Description:
//   Get from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{GenericGridCollectionOperatorsInclude.tex}{}
//-------------------------------------------------------------------
{
  cout << "GenericGridCollectionOperators::get - not implemented yet!\n"; 
  return 1;
  
}




//===========================================================================================
// operator = is a deep copy
//===========================================================================================
GenericGridCollectionOperators & GenericGridCollectionOperators::
operator= ( const GenericGridCollectionOperators & gco )
{
  mappedGridOperators       =gco.mappedGridOperators; // this is shallow, fix! ********
  mappedGridOperatorsPointer=gco.mappedGridOperatorsPointer;
  twilightZoneFlow          =gco.twilightZoneFlow;
  twilightZoneFlowFunction  =gco.twilightZoneFlowFunction;
  
  return *this;
}

//================================================================================
// return the MappedGridOperators object for  MappedGrid "grid"
//================================================================================
GenericMappedGridOperators & GenericGridCollectionOperators::
operator[]( const int grid ) const
{
  if( grid<0 || grid>=mappedGridOperators.getLength() )
  {
    cout << "GenericGridCollectionOperators:ERROR in operator[]  argument grid is invalid" << endl;
    cout << "grid = " << grid << endl;
    cout << "mappedGridOperators.getLength() = " << mappedGridOperators.getLength() << endl;
    cout << "Perhaps you forgot to associate a GridCollection with the GenericGridCollectionOperators object\n";
    cout << "Do this in the constructor of GenericGridCollectionOperators or use updateToMatchGrid\n";
    Overture::abort("This is a fatal error");
  }    
  return mappedGridOperators[grid];
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{put}}
int GenericGridCollectionOperators::
put( GenericDataBase & dir, const aString & name) const
//==================================================================================
// /Description:
//   output onto a database file
// /dir (input): put onto this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{GenericGridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  cout << " GenericGridCollectionOperators::put - not implemented yet!\n"; 
  return 0;
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{setOrderOfAccuracy}}
void GenericGridCollectionOperators::
setOrderOfAccuracy( const int & orderOfAccuracy0 )
//==================================================================================
// /Description:
//   set the order of accuracy
// /orderOfAccuracy0 (input): valid values are 2 or 4.
//\end{GenericGridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].setOrderOfAccuracy( orderOfAccuracy0 );
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{getOrderOfAccuracy}}
int GenericGridCollectionOperators::
getOrderOfAccuracy() const 
//==================================================================================
// /Description:
//   get the order of accuracy
//\end{GenericGridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  if( gridCollection.numberOfGrids()>0 )
    return mappedGridOperators[0].orderOfAccuracy;  // we assume all grids are the same!
  else
    return 2;  // default value
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{setStencilSize}}
void GenericGridCollectionOperators::
setNumberOfComponentsForCoefficients(const int number)
//==================================================================================
// /Description:
//   Indicate the number of components (system size) for functions returning coefficients
//\end{GenericGridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].setNumberOfComponentsForCoefficients(number);
}


//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{setStencilSize}}
void GenericGridCollectionOperators::
setStencilSize(const int stencilSize0)
//==================================================================================
// /Description:
//   Indicate the stencil size for functions returning coefficients
//\end{GenericGridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  stencilSize=stencilSize0;
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].setStencilSize( stencilSize0 );
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsection{sizeOf}} 
real GenericGridCollectionOperators::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
// /Description:
//   Return size of this object  
//\end{GenericGridCollectionOperatorsInclude.tex}  
// =======================================================================================
{
  real size=sizeof(*this);
  // size+=result.sizeOf();
  return size;
}


//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{useConservativeApproximations}}
void GenericGridCollectionOperators::
useConservativeApproximations(bool trueOrFalse /* = TRUE  */)
//==================================================================================
// /Description: 
//    Indicate whether to use the {\sl conservative} approximations to the operators
//  {\tt div}, {\tt laplacian}, {\tt divScalarGrad} and {\tt scalarGrad} and correspoding boundary 
// conditions 
//  /trueOrFalse (input): TRUE means use conservative approximations.
//\end{GenericGridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].useConservativeApproximations(trueOrFalse);
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{setAveragingType}}
void GenericGridCollectionOperators::
setAveragingType(const GenericMappedGridOperators::AveragingType & type )
//==================================================================================
// /Description: 
//    Set the averaging type for certain operators such as {\tt divScalarGrad}. The default
// is {\tt arithmeticAverage}. The {\tt harmonicAverage} is often used for problems
// with discontinuos coefficients. Recall that 
// \begin{align*}
//    \mbox{arithmetic average} ~= {a+b \over 2} \\
//    \mbox{harmonic  average} ~= {2 \over {1\over a} + {1\over b}} ~= {2 a b \over a+ b }
// \end{align*}
// /type (input) : one of {\tt arithmeticAverage} or {\tt harmonicAverage}.
//\end{GenericGridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].setAveragingType(type);
}


//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{updateToMatchGrid}}
void GenericGridCollectionOperators::
updateToMatchGrid( GridCollection & gc )              // here is a (new) GridCollection to use
//========================================================================
// /Description:
//   Use this update function to associate a new GridCollection with this object or to
//   update the object when the GridCollection changes.
// /gc (input): use this grid.
// /Notes:
//  perform computations here that only depend on the grid
//\end{GenericGridCollectionOperatorsInclude.tex}  
//========================================================================
{
  gridCollection.reference(gc);                       // keep a reference to this GridCollection

  // Update the list of MappedGridOperators
  //   o add or remove entries from the list, as required
  //   o update existing ones
  int oldLength=mappedGridOperators.getLength();
  if( oldLength > gridCollection.numberOfGrids() )
  {
    for( int grid=oldLength-1; grid>=gridCollection.numberOfGrids(); grid-- )
    {
      GenericMappedGridOperators *pointer = &mappedGridOperators[grid];
      mappedGridOperators.deleteElement( grid );      // remove excess from the list
      delete pointer;   // this will actually delete the object
    }
  }
  else
  { // create new entries by calling the virtualConstructor for the MappedGridOperators class
    for( int grid=oldLength; grid<gridCollection.numberOfGrids(); grid++ )
    {
      // The cast is needed for the multigrid case, it is a safe cast
      GenericMappedGridOperators *mgop=mappedGridOperatorsPointer->virtualConstructor();
      mappedGridOperators.addElement( *mgop,grid );     
    }
  }
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
  {
    mappedGridOperators[grid].updateToMatchGrid( gridCollection[grid] );   // update 
    
#ifdef COMPOSITE_GRID_OPERATORS
    if( gridCollection.interpolationPoint.getLength()>grid )
      mappedGridOperators[grid].setInterpolationPoint(gridCollection.interpolationPoint[grid]);
#endif
  }
  
  setTwilightZoneFlow(twilightZoneFlow);
  if( twilightZoneFlowFunction!=NULL )
    setTwilightZoneFlowFunction(*twilightZoneFlowFunction);
    
}

//===========================================================================================
// This update operator allows one to specify which Differential operators to use
//  Notes:
//   o by default the finite difference operator class MappedGridOperators is used
//     to define derivatives and BCs
//   o If you want to use the finite volume difference operators you should call
//     this update function with an object of that class
//===========================================================================================
void GenericGridCollectionOperators::
updateToMatchOperators(GenericMappedGridOperators & op)
{
  mappedGridOperatorsPointer=&op;
}



//---------------------------------------------------------------------------------------
//   These routines return particular derivatives
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// Here is a macro to define a derivative "x"
//
//  Replace "x" by any of "y", "z", "xx", ..., to define the different derivatives
//--------------------------------------------------------------------------------------

// These next macro definitions are use to handle non-standard arguments to the Derivative macro
#undef  ARGS1
#undef  ARGS2
#define ARGS1 const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4, \
              const Index & C5
#define ARGS2 ,nullIndex,nullIndex,nullIndex,C1

// here is the macro for a gridCollection and CompositeGrid
#define DERIVATIVE(x)                                                                       \
realGridCollectionFunction GenericGridCollectionOperators::                                 \
x( const realGridCollectionFunction & u, ARGS1 )                                            \
{                                                                                           \
 /* Determine the dimensions of the return type from the dimensions of the derivative of the mappedGridFunction.*/ \
  realMappedGridFunction r;  \
  int grid=0;  \
  r=mappedGridOperators[grid].x(u[grid] ARGS2);  \
  Range R[8];  \
  for( int i=0; i<5; i++ )  \
    R[r.positionOfComponent(i)]=Range(r.getComponentBase(i),r.getComponentBound(i));  \
  realGridCollectionFunction result((GridCollection&)(*u.gridCollection),R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);  \
  result[0]=r;  \
  \
  for( grid=1; grid<gridCollection.numberOfGrids(); grid++ )                                \
    result[grid]=mappedGridOperators[grid].x(u[grid] ARGS2);                                \
  result.updateToMatchComponentGrids();                                                     \
  return result;                                                                            \
} \
realGridCollectionFunction GenericGridCollectionOperators::                                 \
x( const realGridCollectionFunction & u, const GridFunctionParameters & gfType, ARGS1 )     \
{                                                                                           \
  realMappedGridFunction r;  \
  int grid=0;  \
  r=mappedGridOperators[grid].x(u[grid],gfType ARGS2);  \
  Range R[8];  \
  for( int i=0; i<5; i++ )  \
    R[r.positionOfComponent(i)]=Range(r.getComponentBase(i),r.getComponentBound(i));  \
  realGridCollectionFunction result((GridCollection&)(*u.gridCollection),R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);  \
  result[0]=r;  \
  for( grid=1; grid<gridCollection.numberOfGrids(); grid++ )                                \
    result[grid]=mappedGridOperators[grid].x(u[grid],gfType ARGS2);      \
  \
  result.updateToMatchComponentGrids();                                                     \
  return result;                                                                              \
}
// Now define all the instances of this function:
DERIVATIVE(x)
DERIVATIVE(y)
DERIVATIVE(z)
DERIVATIVE(xx)
DERIVATIVE(xy)
DERIVATIVE(xz)
DERIVATIVE(yy)
DERIVATIVE(yz)
DERIVATIVE(zz)

DERIVATIVE(r1)
DERIVATIVE(r2)
DERIVATIVE(r3)
DERIVATIVE(r1r1)
DERIVATIVE(r1r2)
DERIVATIVE(r1r3)
DERIVATIVE(r2r2)
DERIVATIVE(r2r3)
DERIVATIVE(r3r3)

DERIVATIVE(laplacian)

DERIVATIVE(cellsToFaces)
DERIVATIVE(convectiveDerivative)
DERIVATIVE(contravariantVelocity)
DERIVATIVE(divNormal)
DERIVATIVE(normalVelocity)
DERIVATIVE(identity)


DERIVATIVE(grad)
DERIVATIVE(vorticity)
DERIVATIVE(div)

// some arguments are different in the following operators
#undef  ARGS1
#undef  ARGS2
#define ARGS1 const realGridCollectionFunction &w, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3

#define ARGS2 ,w[grid],C1,C2,C3

DERIVATIVE(convectiveDerivative)


#undef  ARGS1
#undef  ARGS2
#define ARGS1 const realGridCollectionFunction &w, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4

#define ARGS2 ,w[grid]
DERIVATIVE(scalarGrad)
DERIVATIVE(divScalarGrad)
DERIVATIVE(divInverseScalarGrad)
DERIVATIVE(divVectorScalar)


#undef  ARGS1
#undef  ARGS2
#define ARGS1 const realGridCollectionFunction &w, \
              const int & direction1, \
              const int & direction2, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4

#define ARGS2 ,w[grid],direction1,direction2
DERIVATIVE(derivativeScalarDerivative)


#undef  ARGS1
#define ARGS1 const int axis1, \
                    const int axis2, \
		    const int c0,  \
		    const int c1,  \
		    const int c2,  \
		    const int c3,  \
		    const int c4

#undef  ARGS1
#define ARGS1 const int c0,  \
	      const int c1,  \
	      const int c2,  \
	      const int c3,  \
	      const int c4

#undef  ARGS2
#define ARGS2 ,c0,c1,c2,c3,c4
DERIVATIVE(FCgrad)


#undef DERIVATIVE


//---------------------------------------------------------------------------------------
// Here is a macro to define a function that returns the coefficients of the derivative "X"
//
//  Replace "X" by any of "Y", "Z", "XX", ..., to define the different derivatives
//--------------------------------------------------------------------------------------
#undef  ARGS1
#define ARGS1 const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4

#undef  ARGS2
#define ARGS2 

// here is the macro for a gridCollection and CompositeGrid
#define DERIVATIVE_COEFFICIENTS(X)                                                           \
realGridCollectionFunction GenericGridCollectionOperators::                                        \
X(ARGS1)                                                                                    \
{                                                                                           \
  Range all;                                                                                \
  int stencilDim = stencilSize*SQR(mappedGridOperators[0].numberOfComponentsForCoefficients);    \
  realGridCollectionFunction result(gridCollection,stencilDim,all,all,all);                         \
  Index M(0,stencilDim-1);                                                                 \
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )                     \
  {                                                                                         \
    result[grid]=0.; \
    getIndex(gridCollection[grid].dimension(),I1,I2,I3,-mappedGridOperators[grid].orderOfAccuracy/2);  \
    result[grid](M,I1,I2,I3)=mappedGridOperators[grid].X(ARGS2 I1,I2,I3,C1,C2,C3,C4)(M,I1,I2,I3); \
  }                                                                                         \
  result.setOperators(*this);                                                               \
  return result;                                                                            \
} \
realGridCollectionFunction GenericGridCollectionOperators::                                        \
X(const GridFunctionParameters & gfType, ARGS1)                           \
{                                                                                           \
  Range all;                                                                                \
  int stencilDim = stencilSize*SQR(mappedGridOperators[0].numberOfComponentsForCoefficients);    \
  realGridCollectionFunction result(gridCollection,stencilDim,all,all,all);                         \
  Index M(0,stencilDim-1);                                                                 \
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )                     \
  {                                                                                         \
    result[grid]=0.; \
    getIndex(gridCollection[grid].dimension(),I1,I2,I3,-mappedGridOperators[grid].orderOfAccuracy/2);  \
    result[grid](M,I1,I2,I3)=mappedGridOperators[grid].X(gfType, ARGS2 I1,I2,I3,C1,C2,C3,C4)(M,I1,I2,I3); \
  }                                                                                         \
  result.setOperators(*this);                                                               \
  return result;                                                                            \
}

// Now define all the instances of this function:
DERIVATIVE_COEFFICIENTS(xCoefficients)
DERIVATIVE_COEFFICIENTS(yCoefficients)
DERIVATIVE_COEFFICIENTS(zCoefficients)
DERIVATIVE_COEFFICIENTS(xxCoefficients)
DERIVATIVE_COEFFICIENTS(xyCoefficients)
DERIVATIVE_COEFFICIENTS(xzCoefficients)
DERIVATIVE_COEFFICIENTS(yyCoefficients)
DERIVATIVE_COEFFICIENTS(yzCoefficients)
DERIVATIVE_COEFFICIENTS(zzCoefficients)

DERIVATIVE_COEFFICIENTS(laplacianCoefficients)
DERIVATIVE_COEFFICIENTS(divCoefficients)
DERIVATIVE_COEFFICIENTS(identityCoefficients)

DERIVATIVE_COEFFICIENTS(r1Coefficients)
DERIVATIVE_COEFFICIENTS(r2Coefficients)
DERIVATIVE_COEFFICIENTS(r3Coefficients)
DERIVATIVE_COEFFICIENTS(r1r1Coefficients)
DERIVATIVE_COEFFICIENTS(r1r2Coefficients)
DERIVATIVE_COEFFICIENTS(r1r3Coefficients)
DERIVATIVE_COEFFICIENTS(r2r2Coefficients)
DERIVATIVE_COEFFICIENTS(r2r3Coefficients)
DERIVATIVE_COEFFICIENTS(r3r3Coefficients)

DERIVATIVE_COEFFICIENTS(gradCoefficients)


#undef  ARGS1
#define ARGS1 const realGridCollectionFunction &s, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4
#undef  ARGS2
// here is the macro for a gridCollection and CompositeGrid
#define ARGS2 s[grid],

DERIVATIVE_COEFFICIENTS(scalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(divScalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(divInverseScalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(divVectorScalarCoefficients)


#undef  ARGS1
#define ARGS1 const realGridCollectionFunction &s, \
              const int & direction1, \
              const int & direction2, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4
#undef  ARGS2
// here is the macro for a gridCollection and CompositeGrid
#define ARGS2 s[grid],direction1,direction2,

DERIVATIVE_COEFFICIENTS(derivativeScalarDerivativeCoefficients)


#undef DERIVATIVE_COEFFICIENTS
#undef ARGS1
#undef ARGS2



// --------------------Boundary Condition Routines --------------------------------

// boundary conditions are not put in thr multigrid case (should we?)
//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{setTwilightZoneFlow}}  
void GenericGridCollectionOperators::
setTwilightZoneFlow( const int & twilightZoneFlow_ )
//=======================================================================================
// /Description:
//   Indicate whether or not twilightzone flow forcing should be added to the BC's
// /twilightZoneFlow\_ (input): if 1 then add the twilight-zone forcing to all boundary
//    conditions except for extrapolation. If 2 then also add to extrapolation. 
//\end{GenericGridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  twilightZoneFlow=twilightZoneFlow_;
  for( int grid=0; grid<gridCollection.numberOfComponentGrids(); grid++ )  
    mappedGridOperators[grid].setTwilightZoneFlow(twilightZoneFlow);
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{setTwilightZoneFlowFunction}}  
void GenericGridCollectionOperators:: 
setTwilightZoneFlowFunction( OGFunction & twilightZoneFlowFunction_ )
//=======================================================================================
// /Description:
//   Indicate which twilightzone flow function should be used
// /TwilightZoneFlowFunction0 (input): use this twilightzone function.
//\end{GenericGridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  twilightZoneFlowFunction=&twilightZoneFlowFunction_;
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].setTwilightZoneFlowFunction(*twilightZoneFlowFunction);
}


//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{applyBoundaryConditions}}  
void GenericGridCollectionOperators:: 
applyBoundaryConditions(realGridCollectionFunction & u, 
			const real & time, /* =0. */
			const int & grid0  /* =forAll */ )
//=======================================================================================
// /Description:
//  Apply the boundary conditions to a grid function.
//  This routine implements every boundary condition known to man (ha!)
//
// /u (input/output): apply boundary conditions to this grid function.
// /t (input): apply boundary conditions at this time.
//
// /Limitations:
//  only second order accurate, only homogenous Neumann etc.
//\end{GenericGridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  if( grid0==forAll )
  {
    for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    {
      mappedGridOperators[grid].applyBoundaryConditions(u[grid],time);
      if( mappedGridOperators[grid].errorStatus==GenericMappedGridOperators::errorInFindInterpolationNeighbours )
      {
        printf("GenericGridCollectionOperators::applyBoundaryConditions:ERROR\n"
               "  Error in findInterpolationNeighbours for grid=%i\n"
                 ,grid);

	if( u.getGridCollection()->getClassName()=="CompositeGrid" )
	{ // This must be a CompositeGrid if it has interpolation points
          aString gridFileName="errorGrid.hdf";
	  CompositeGrid & cg = (CompositeGrid &)(*u.getGridCollection());
	  cg.saveGridToAFile(gridFileName,"errorGrid");
	}
	Overture::abort("error");
      }
    }
    
  }
  else
  {
    mappedGridOperators[grid0].applyBoundaryConditions(u[grid0],time);
  }
}

//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{ApplyBoundaryConditions}}  
void GenericGridCollectionOperators:: 
assignBoundaryConditionCoefficients(realGridCollectionFunction & coeff, 
				    const real & time, /* =0. */ 
				    const int & grid0  /* =forAll */ )
//=======================================================================================
// /Description:
//  Fill in the coefficients of the boundary conditions.
//
// /coeff (input/output): grid function to hold the coefficients of the BC.
// /time (input): apply boundary conditions at this time.
// /Limitations:
//  too many to write down.
//\end{GenericGridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  if( grid0==forAll )
  {
    for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
      mappedGridOperators[grid].assignBoundaryConditionCoefficients(coeff[grid],time);
  }
  else
  {
    mappedGridOperators[grid0].assignBoundaryConditionCoefficients(coeff[grid0],time);
  }
}

//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{applyBoundaryCondition}}  
void GenericGridCollectionOperators::
applyBoundaryCondition(realGridCollectionFunction & u, 
                              const Index & Components,
                              const BCTypes::BCNames & bcType  /* = BCTypes::dirichlet */,
                              const int & bc,                   /* = BCTypes::allBoundaries */
                              const real & forcing,             /* =0. */
                              const real & time,                /* =0. */
                              const BoundaryConditionParameters & 
                                 bcParameters /* = Overture::defaultBoundaryConditionParameters() */ )
//=======================================================================================
//\end{GridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].applyBoundaryCondition(u[grid],Components,bcType,
           bc,forcing,time,bcParameters,grid );
}

//\begin{>>GridCollectionOperatorsInclude.tex}{}
void GenericGridCollectionOperators::
applyBoundaryCondition(realGridCollectionFunction & u, 
                              const Index & Components,
                              const BCTypes::BCNames & bcType,
                              const int & bc,
                              const RealArray & forcing,
                              const real & time,                /* =0. */
                              const BoundaryConditionParameters & 
                                    bcParameters /* = Overture::defaultBoundaryConditionParameters() */)
//=======================================================================================
//\end{GridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].applyBoundaryCondition(u[grid],Components,bcType,
           bc,forcing,time,bcParameters,grid );
}

//\begin{>>GridCollectionOperatorsInclude.tex}{}
void GenericGridCollectionOperators::
applyBoundaryCondition(realGridCollectionFunction & u, 
                              const Index & Components,
                              const BCTypes::BCNames & bcType,
                              const int & bc,
                              const realGridCollectionFunction & forcing,
                              const real & time,                /* =0. */
                              const BoundaryConditionParameters & bcParameters 
                                    /* = Overture::defaultBoundaryConditionParameters() */)
//=======================================================================================
// /Description:
//  Apply a boundary condition to a grid function.
//  This routine implements every boundary condition known to man (ha!)
//
// /u (input/output): apply boundary conditions to this grid function.
// /Components (input): apply to these components
// /bcType  (input): the name of the boundary condition to apply (dirichlet, neumann,...)
// /bc (input): apply the boundary condition on all sides of the grid where the
//     boundaryCondition array (in the MappedGrid) is equal to this value. By default
//     {\tt bc=BCTypes allBoundaries} apply to all boundaries (with a positive value for boundaryCondition).
//     To apply a boundary condition to a specified side use
//         \begin{itemize}
//           \item {\tt bc=BCTypes::boundary1} for $(side,axis)=(0,0)$
//           \item {\tt bc=BCTypes::boundary2} for $(side,axis)=(1,0)$
//           \item {\tt bc=BCTypes::boundary3} for $(side,axis)=(0,1)$
//           \item {\tt bc=BCTypes::boundary4} for $(side,axis)=(1,1)$
//           \item {\tt bc=BCTypes::boundary5} for $(side,axis)=(0,2)$
//           \item {\tt bc=BCTypes::boundary6} for $(side,axis)=(1,2)$
//         \end{itemize}
//     or use {\tt bc=BCTypes::boundary1+side+3*axis} for given values of $(side,axis)$ (this 
//    could be used in a loop, for example).
// /forcing (input): This value is used as a forcing for the boundary condition, if needed. 
// /time (input): apply boundary conditions at this time (used by twilightZoneFlow)
// /bcParameters (input): optional parameters are passed using this object.
//   See the examples for how to pass parameters with this argument.
// /Limitations:
//  only second order accurate.
//
//\end{GridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].applyBoundaryCondition(u[grid],Components,bcType,
           bc,forcing[grid],time,bcParameters,grid );
}


#ifdef USE_PPP
// void GenericGridCollectionOperators::
// applyBoundaryCondition(realGridCollectionFunction & u, 
//                               const Index & Components,
//                               const BCTypes::BCNames & bcType,
//                               const int & bc,
//                               const RealDistributedArray & forcing,
//                               const real & time,                /* =0. */
//                               const BoundaryConditionParameters & 
//                                     bcParameters /* = Overture::defaultBoundaryConditionParameters() */)
// //=======================================================================================
// //=======================================================================================
// {
//   for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
//     mappedGridOperators[grid].applyBoundaryCondition(u[grid],Components,bcType,
//            bc,forcing,time,bcParameters,grid );
// }
#endif



//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{applyBoundaryConditionCoefficients}}  
void GenericGridCollectionOperators::
applyBoundaryConditionCoefficients(realGridCollectionFunction & coeff, 
				   const Index & Equations,
				   const Index & Components,
				   const BCTypes::BCNames & 
				   bcType,     /* = BCTypes::dirichlet */
				   const int & bc,   /* = BCTypes::allBoundaries */
				   const BoundaryConditionParameters & 
				   bcParameters   /* = Overture::defaultBoundaryConditionParameters() */)
//=======================================================================================
// /Description:
//  Fill in the coefficients of the boundary conditions.
//
// /coeff (input/output): grid function to hold the coefficients of the BC.
// /t (input): apply boundary conditions at this time.
// /Limitations:
//  too many to write down.
//\end{GridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    mappedGridOperators[grid].applyBoundaryConditionCoefficients(coeff[grid],Equations,Components,bcType,
           bc,bcParameters,grid );
}

#ifdef COMPOSITE_GRID_OPERATORS
#include "SparseRep.h"
#endif
//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{finishBoundaryConditions}}  
void GenericGridCollectionOperators::
finishBoundaryConditions(realGridCollectionFunction & u,
                         const BoundaryConditionParameters & bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
                         const Range & C0 /* =nullRange */,
                         const IntegerArray & gridsToUpdate /* = Overture::nullIntArray */ )
//=======================================================================================
// /Description: Call this routine when all boundary conditions have been applied.
//  This function will fix up the solution values in corners and update periodic
//  edges.
// /u (input/output): Grid function to which boundary conditions were applied.
// /bcParameters (input): Supply parameters such as bcParameters.orderOfExtrapolation which indicates
//   the order of extrapolation to use.
// /C0 (input) : apply to these components.
// /gridsToUpdate (input) : optionally supply a list of grids to update. Bu default all grids are updated.
//\end{GridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  const bool updateSomeGrids = gridsToUpdate.getLength(0)>0;
  const int gridStart=updateSomeGrids ? gridsToUpdate.getBase(0)  : 0;
  const int gridEnd  =updateSomeGrids ? gridsToUpdate.getBound(0) : gridCollection.numberOfGrids()-1;

  for( int g=gridStart; g<=gridEnd; g++ )
  {
    int grid = updateSomeGrids ? gridsToUpdate(g) : g;
    mappedGridOperators[grid].finishBoundaryConditions(u[grid],bcParameters,C0);
  }
  
  if( u.getIsACoefficientMatrix() )
  {
#ifndef USE_PPP
    // for now we do NOT fill in the interpolation coeff's for P++
   #ifdef COMPOSITE_GRID_OPERATORS
    getInterpolationCoefficients( (realCompositeGridFunction &)u,gridsToUpdate );
   #endif
    interpolateRefinements( u,0,bcParameters );  // still need to add gridsToUpdate here
#endif
  }
}



#ifdef COMPOSITE_GRID_OPERATORS

#include <float.h>

#define Q11(x) (1.-(x))
#define Q21(x) (x)

#define Q12(x) .5*((x)-1.)*((x)-2.)
#define Q22(x) (x)*(2.-(x))
#define Q32(x) .5*(x)*((x)-1.)

#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123(m1,m2,m3) (m1)+width0[axis1]*(m2+width0[axis2]*(m3))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))



void GenericCompositeGridOperators::
getInterpolationCoefficients(realCompositeGridFunction & coeff0,
                             const IntegerArray & gridsToUpdate  /* = Overture::nullIntArray() */ )
//==========================================================================
//  Compute Interpolation coefficients and add to the coefficient matrix
//
// Input 
//  coeff
// NOTE: The interpolation equations are the same for all components
//===================================================================
{

  // cout << "CompositeGridOperators: getInterpolationCoefficients...\n";

  CompositeGrid & cg = *coeff0.getCompositeGrid();
//  if( cg.numberOfGrids() ==0 ||  // *wdh* 000508
//      (cg.numberOfGrids()==1 && cg.numberOfInterpolationPoints(0)<= 0) )
  if( cg.numberOfBaseGrids() ==0 ||
      (cg.numberOfBaseGrids()==1 && cg.numberOfInterpolationPoints(0)<= 0) )
    return;
  
  if( cg.numberOfMultigridLevels()>1 )
  {
    // recursively call this routine for each multigrid level.
    for( int l=0; l<cg.numberOfMultigridLevels(); l++ )
    {
      // assert( coeff0.multigridLevel[l].getIsACoefficientMatrix() );
      getInterpolationCoefficients( coeff0.multigridLevel[l],gridsToUpdate );
    }
    return;
  }
  const int numberOfDimensions = cg.numberOfDimensions();
  
  int iv[3];
  int & i1=iv[axis1];
  int & i2=iv[axis2];
  int & i3=iv[axis3];
  int iiv[3];  // holds lower corner of interpolation stencil
  
  Range all;
  int width0[3]={1,1,1};
  RealArray q;
  int m1,m2,m3;
  
  // compute the allowable tolerance for interpolating near the boundary of a grid:
  int axis,grid;
  const int mgLevel=0;
    
  const real epsilon=REAL_EPSILON*20.;
  RealArray epsilonForInterpolation(numberOfDimensions,cg.numberOfGrids());

  for( grid=0; grid<cg.numberOfGrids(); grid++ )
    for( axis=0; axis<numberOfDimensions; axis++ )
      epsilonForInterpolation(axis,grid)=max(cg.epsilon()*2.1/cg[grid].gridSpacing(axis),
					     max(cg[grid].sharedBoundaryTolerance()(all,axis)));
    
  const IntegerArray & interpolationWidth = cg.interpolationWidth(all,all,all,mgLevel);

  const bool updateSomeGrids = gridsToUpdate.getLength(0)>0;
  const int gridStart=updateSomeGrids ? gridsToUpdate.getBase(0)  : 0;
  const int gridEnd  =updateSomeGrids ? gridsToUpdate.getBound(0) : cg.numberOfGrids()-1;

  IntegerArray useThisGrid;
  if( updateSomeGrids )
  {
    useThisGrid.redim(cg.numberOfGrids());
    useThisGrid=0;
    useThisGrid(gridsToUpdate)=1;
  }

  // for( grid=0; grid<cg.numberOfGrids(); grid++ )
  for( int g=gridStart; g<=gridEnd; g++ )
  {
    grid = updateSomeGrids ? gridsToUpdate(g) : g;

    MappedGrid & mg = cg[grid];
    realMappedGridFunction & coeff = coeff0[grid];
    int stencilSize0 = coeff.sparse->stencilSize;
    int numberOfComponentsForCoefficients = coeff.sparse->numberOfComponents;
    const intArray & variableInterpolationWidth = cg.variableInterpolationWidth[grid];
    const intArray & mask = mg.mask();
    const intArray & interpolationPoint = cg.interpolationPoint[grid];
    
    // fill in default values
    for( axis=numberOfDimensions; axis < 3; axis++ )
    {
      iv[axis]=iiv[axis]=mg.dimension()(Start,axis);  
    }
    for( int m=0; m<cg.numberOfInterpolationPoints(grid); m++ )
    {
      // here is the interpolee grid:
      int gridInterpolee = cg.interpoleeGrid[grid](m);
      MappedGrid & mgInterpolee = cg[gridInterpolee];
      realMappedGridFunction & coeffInterpolee = coeff0[gridInterpolee];
  

      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	iv[axis]=interpolationPoint(m,axis);
        width0[axis]=variableInterpolationWidth(m);
      }
      
      if( mask(i1,i2,i3) >= 0 )
      {
	cout << " getInterpolationCoefficients: ERROR mask >=0 " << endl;
	printf("grid=%i, (i1,i2,i3)=(%i,%i,%i), mask=%i, gridInterpolee=%i, point number=%i\n",
	       grid,i1,i2,i3,mask(i1,i2,i3),gridInterpolee,m);
	// mg.mask().display("Here is the mask");
	Overture::abort(" getInterpolationCoefficients: ERROR mask >=0 ");
      }

      // no need to check this every time  ****************************************************
      // *wdh* width0(Range(0,2))=interpolationWidth(Range(0,2),grid,gridInterpolee);  // use this width

      if( stencilSize0 < width0[axis1]*width0[axis2]*width0[axis3]+1 )
      {
	cout << "CompositeGridOperators::getInterpolationCoefficients:ERROR: "
	  " not enough room in matrix to hold interpolation coefficients\n";
	printf(" interpolation widths = %i, %i, %i, matrix stencil size = %i should be >= %i \n",
	       width0[axis1],width0[axis2],width0[axis3], stencilSize0,
	       width0[axis1]*width0[axis2]*width0[axis3]+1);
	cout << "You need to declare your coefficient matrix with a larger stencil\n";
	Overture::abort("getInterpolationCoefficients:ERROR:");
      }

      // q holds the interpolation weigths
      q.redim(3,max(width0[0],width0[1],width0[2]));
      q=1.;

      const int iw = variableInterpolationWidth(m);
      const real delta = 0.5*(max(2,iw)-1)+epsilon;
      
      //.........First form 1D interpolation coefficients
      for( axis=axis1; axis<numberOfDimensions; axis++ ) 
      {
	iiv[axis]=cg.interpoleeLocation[grid](m,axis);
	real rsb=cg.interpolationCoordinates[grid](m,axis)/mgInterpolee.gridSpacing()(axis)
	  +mgInterpolee.indexRange()(Start,axis);
	real px= mgInterpolee.isCellCentered()(axis)  ? rsb-iiv[axis]-.5 : rsb-iiv[axis];

	// *wdh* const int iw = interpolationWidth(axis,grid,gridInterpolee);
	
        // "interpolation" means that 0 < px < iw-1 (otherwise we are extrapolating)
	if( fabs( px - 0.5*(iw-1)) >  delta )
	{
	  // Interpolation weight is too large, this could be an error
	  //..............okay if extrapolating near a boundary
	  real pxmax=mgInterpolee.isCellCentered()(axis)  ? .5 : 0.; 
	  // real eps=REAL_EPSILON*max(500.,100./mgInterpolee.gridSpacing()(axis));
	  const real eps=epsilonForInterpolation(axis,gridInterpolee);
	  if( ! (
	    ( ( cg.interpoleeLocation[grid](m,axis) < mgInterpolee.indexRange(Start,axis) +iw ) ||
	      ( cg.interpoleeLocation[grid](m,axis) > mgInterpolee.indexRange(End,axis)    -iw)  ) &&
	    (px >= -eps-pxmax && px <= iw-1.+eps+pxmax) ) )
	  {
	    printf("getInterpolationCoefficients:ERROR: Invalid interpolation found! \n");
	    
            printf("  | px(axis=%i)=%e - .5*((iw=%i)-1) |==%e > .5*(iw-1) and not near the boundary\n",
		   axis,px,iw, fabs( px - 0.5*(iw-1)));
	    printf("  px(axis=%i)=%e == (rI=%e)/(dr=%e)-(interpoleeLocation=%i) \n",
		   axis,px,cg.interpolationCoordinates[grid](m,axis),mgInterpolee.gridSpacing()(axis),
		   iiv[axis]);
	    
	    printf( "  interpolation weight px = %9.2e (eps=%9.2e,epsilon=%9.2e)  m=%i, axis =%i, width=%i \n"
		    "  interpolating point     (i1,i2,i3,grid) =(%i,%i,%i,%i:[%s]) \n"
		    "  interpoleeLocation:     (i1,i2,i3,gridInterpolee)=(%i,%i,%i,%i:[%s]) \n"
		    "  interpolationCoordinate=(%e,%e,%e)  mgInterpolee.indexRange()(.,axis)=(%i,%i) \n",
		    px,eps,epsilon,m,axis,iw,iv[axis1],iv[axis2],iv[axis3],grid,
		    (const char*)cg[grid].mapping().getName(Mapping::mappingName),
		    cg.interpoleeLocation[grid](m,axis1),
		    cg.interpoleeLocation[grid](m,axis2),
		    (numberOfDimensions>2 ? cg.interpoleeLocation[grid](m,axis3) : 0),
		    cg.interpoleeGrid[grid](m),
		    (const char*)cg[cg.interpoleeGrid[grid](m)].mapping().getName(Mapping::mappingName),
		    cg.interpolationCoordinates[grid](m,axis1),
		    cg.interpolationCoordinates[grid](m,axis2),
		    (numberOfDimensions>2 ? cg.interpolationCoordinates[grid](m,axis3) : 0),
		    mg.indexRange()(Start,axis),
		    mgInterpolee.indexRange()(End,axis));
	  }
	}
	
	if( width0[axis] < iw )
	{
	  //......interpolation width less than maximum allowed
	  if( px > width0[axis]/2. )
	  {
	    int ipx=min(int(px-(width0[axis]-2)/2.),iw-width0[axis]);
	    px-=ipx;
	    iiv[axis]+=ipx;
	  }
	}

	switch (width0[axis])
	{
	case 3:
	  //........quadratic interpolation
	  q(axis,0)=Q12(px);
	  q(axis,1)=Q22(px);
	  q(axis,2)=Q32(px);
	  break;
	case 2:
	  //.......linear interpolation
	  q(axis,0)=Q11(px);
	  q(axis,1)=Q21(px);
	  break;
	default:
	  // .....order >3 - compute lagrange interpolation
	  for(m1=0; m1<width0[axis]; m1++ ) 
	  {
	    real qq=1.;
	    for( m2=0; m2<width0[axis]; m2++ )
	    {
	      if( m1 != m2  )
		qq*=(px-m2)/(m1-m2);
	    }
	    q(axis,m1)=qq;
	  }
	}
      }

      // zero out coefficients ****** could do better here *****
      coeff(all,i1,i2,i3)=0.;  

      //.......Now form the interpolation coefficients
	  
      if( !updateSomeGrids || useThisGrid(gridInterpolee) ) // don't fill in coefficients for unused grids
      {
	for( m3=0; m3< width0[axis3]; m3++ ) 
	  for( m2=0; m2< width0[axis2]; m2++ ) 
	    for( m1=0; m1< width0[axis1]; m1++ ) 
	    {
	      int n=0;
	      coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=q(axis1,m1)*q(axis2,m2)*q(axis3,m3);
	      coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
			 	coeffInterpolee.sparse->indexToEquation(n,iiv[axis1]+m1,iiv[axis2]+m2,iiv[axis3]+m3) );  
	      // just copy values for other components
	      for( n=1; n<numberOfComponentsForCoefficients; n++ )
	      {
	
		coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=coeff(M123CE(m1,m2,m3,0,0),i1,i2,i3);
		coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
			    coeffInterpolee.sparse->indexToEquation(n,iiv[axis1]+m1,iiv[axis2]+m2,iiv[axis3]+m3) );  
	      }
	    }
      }
      // Now add coefficient of the point being interpolated
      m1=width0[axis1]-1;
      m2=width0[axis2]-1;
      m3=width0[axis3]-1;
      for( int n=0; n<numberOfComponentsForCoefficients; n++ )
      {
	coeff(M123CE(m1+1,m2,m3,n,n),i1,i2,i3)=-1.;
	coeff.sparse->setCoefficientIndex(M123CE(m1+1,m2,m3,n,n), n,i1,i2,i3, n,i1,i2,i3);
      }

    }
  }
}

#undef Q11
#undef Q21

#undef Q12
#undef Q22
#undef Q32

#undef CE
#undef M123
#undef M123CE

#endif



