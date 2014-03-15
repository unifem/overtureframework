#include "intGridCollectionFunction.h"
#include "GridCollection.h"
#include "Interpolant.h"
#include "GenericGridCollectionOperators.h"

#undef MGCG                 // defined for the MultigridCompositeGrid case
#undef MGCG_OR_CG           // defined for the MultigridCompositeGrid or CompositeGrid Case

// Note that abs is converted to abs in the int case


//==========================================================================
//  This macro defines one of the operators + * 
//  for   intGridCollectionFunction XX intGridCollectionFunction
//==========================================================================
#define NON_COMMUTATIVE_OPERATOR(XX)                                \
intGridCollectionFunction & intGridCollectionFunction::                   \
operator XX ( const intGridCollectionFunction & cgf ) const            \
{                                                                   \
  intGridCollectionFunction *cgfn;                                     \
  if( temporary )                                                   \
  {   \
    cgfn=(intGridCollectionFunction*)this;                             \
    /* for( int grid=0; grid< numberOfComponentGrids; grid++ )  */       \
      /* (*cgfn)[grid] XX =cgf[grid];                           */       \
    /* delete the rhs if it is a temporary */                       \
    if( cgf.temporary )                                             \
    {                                                               \
      intGridCollectionFunction *temp = (intGridCollectionFunction*) &cgf; \
      delete temp;                                                  \
    }                                                               \
  }                                                                 \
  else if( cgf.temporary )                                          \
  {                                                                 \
    cgfn=(intGridCollectionFunction*)&cgf;                             \
    for( int grid=0; grid< numberOfComponentGrids; grid++ )         \
      (*cgfn)[grid] =(*this)[grid] XX (*cgfn)[grid];                \
  }                                                                 \
  /* neither operand is a temporary -- make a temporary to use */   \
  else                                                              \
  {                                                                 \
    cgfn= new intGridCollectionFunction();                             \
    (*cgfn).temporary=TRUE;                                         \
    (*cgfn).updateToMatchGridFunction(cgf);                         \
    Index All;                                                      \
    for( int grid=0; grid< numberOfComponentGrids; grid++ )         \
      (*cgfn)[grid](All,All,All,All) = (*this)[grid](All,All,All,All) XX cgf[grid](All,All,All,All);                   \
  }                                                                 \
  return *cgfn;                                                     \
}

NON_COMMUTATIVE_OPERATOR(/)
NON_COMMUTATIVE_OPERATOR(-)

#undef NON_COMMUTATIVE_OPERATOR




//==========================================================================
//  This macro defines one of the operators += -= *= or /=
//  for   intGridCollectionFunction XX intGridCollectionFunction
//==========================================================================
#define OPERATOR(XX)                                              \
intGridCollectionFunction & intGridCollectionFunction::                 \
operator XX( const intGridCollectionFunction & cgf )                 \
{                                                                 \
  for( int grid=0; grid< numberOfComponentGrids; grid++ )         \
    (*this)[grid] XX cgf[grid];                                   \
  if( cgf.temporary )  /* delete the rhs if it is a temporary */  \
  { /* This is a fudge -- cast away the const  */                   \
    intGridCollectionFunction *temp = (intGridCollectionFunction*) &cgf;  \
    delete temp;                                                  \
  }                                                               \
  return *this;                                                   \
}

OPERATOR(+=)
OPERATOR(-=)
OPERATOR(*=)
OPERATOR(/=)

#undef OPERATOR


//==========================================================================
//  This macro defines one of the operators + - * or / times a int
//==========================================================================
#define OPERATOR(XX)                                                \
intGridCollectionFunction & intGridCollectionFunction::                   \
operator XX( const int value ) const                             \
{                                                                   \
  intGridCollectionFunction *cgfn;                                     \
  if( temporary )                                                   \
  {                                                                 \
    cgfn=(intGridCollectionFunction *)this;                            \
    for( int grid=0; grid< numberOfComponentGrids; grid++ )         \
      (*cgfn)[grid] XX=value;                                       \
  }                                                                 \
  else                                                              \
  {                                                                 \
    cgfn= new intGridCollectionFunction();                             \
    (*cgfn).temporary=TRUE;                                         \
    (*cgfn).updateToMatchGridFunction(*this);                       \
    Index All;                                                      \
    for( int grid=0; grid< numberOfComponentGrids; grid++ )         \
      (*cgfn)[grid](All,All,All,All) = (*this)[grid](All,All,All,All) XX value;                       \
  }                                                                 \
  return *cgfn;                                                     \
}

OPERATOR(+)
OPERATOR(-)
OPERATOR(*)
OPERATOR(/)
  
#undef OPERATOR

//==========================================================================
//  This macro defines one of the operators = +=  -= *= or /=  
//  for   intGridCollectionFunction XX int
//==========================================================================
#define OPERATOR(XX)                                              \
intGridCollectionFunction & intGridCollectionFunction::                 \
operator XX( const int value )                                 \
{                                                                 \
  for( int grid=0; grid< numberOfComponentGrids; grid++ )         \
    (*this)[grid] XX value;                                       \
  return *this;                                                   \
}

OPERATOR(=)
OPERATOR(+=)
OPERATOR(-=)
OPERATOR(*=)
OPERATOR(/=)

#undef OPERATOR


// ---------------------friend functions------------------------

//==========================================================================
//  This macro defines one of the operators + - * / 
//  for   int XX intGridCollectionFunction
//==========================================================================
#define OPERATOR(XX)                                              \
intGridCollectionFunction &                                          \
operator XX( const int u, const intGridCollectionFunction & U)    \
{                                                                 \
  return U XX u;                                                  \
}

OPERATOR(+)
OPERATOR(-)
OPERATOR(*)
OPERATOR(/)

#undef OPERATOR





//=====================================================================
// Allocate space for the name array
//====================================================================
void intGridCollectionFunction::
dimensionName()
{
  Range *R = rcData->R;  // make a reference to the array in rcData
  // first count the total number of components
  int component,numberOfComponents=1;
  for( int i=0; i<maximumNumberOfIndicies; i++ )
  {
    component=positionOfComponent(i);
    if( component < maximumNumberOfIndicies )
      numberOfComponents*=R[component].length();
  }
  if( rcData->numberOfNames < numberOfComponents+1 )
  {
    aString *newName = ::new aString[numberOfComponents+1];
    for( int i=0; i<rcData->numberOfNames; i++ )
      newName[i]=rcData->name[i];
    for( i=rcData->numberOfNames; i<numberOfComponents+1; i++ )
      newName[i]=" ";
    delete [] rcData->name;
    rcData->name=newName;
    rcData->numberOfNames=numberOfComponents+1;
  }
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setName}}
void intGridCollectionFunction::
setName(const aString & name, 
        const int & component0,  /* =defaultValue */
        const int & component1,  /* =defaultValue */
        const int & component2,  /* =defaultValue */
        const int & component3,  /* =defaultValue */
        const int & component4   /* =defaultValue */  )
//==================================================================================
// /Description:
//   Set the name of the grid function or a component as in
//   \begin{verbatim} 
//     u.setName("nameOfGridFunction");  
//     u.setName("nameOfComponent0",0);
//     u.setName("nameOfComponent1",1);
//   \end{verbatim} 
// /name: the name of the grid function or component.
// /component0, component1,... (input): give the name for this component. 
//    if all of component0,component1,component2 ==defaultValue then the name
//    of the grid function is set. Otherwise the default value becomes
//    the base value for that component.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;  // make a reference to the array in rcData
  dimensionName();
  int c[maximumNumberOfComponents] = { component0, component1, component2, component3, component4 };
  
  if( c[0]==defaultValue && c[1]==defaultValue && c[2]==defaultValue &&
      c[3]==defaultValue && c[4]==defaultValue )
    rcData->name[0]=name;   // assign the name for the grid function
  else
  {
    for( int i=0; i<maximumNumberOfComponents; i++ )
    {
      if( c[i]==defaultValue )
        c[i]=R[positionOfComponent(i)].getBase();
      else if(c[i] < R[positionOfComponent(i)].getBase() ||
 	      c[i] > R[positionOfComponent(i)].getBound() )
      {
	printf("intGridCollectionFunction::setName:ERROR component%i=%i is invalid ! \n",i,c[i]);
	printf(" It should be in the range (%i,%i) \n",R[positionOfComponent(i)].getBase(),
	       R[positionOfComponent(i)].getBound());
	return;
      }
    }
    rcData->GFNAME(c[0],c[1],c[2],c[3],c[4])=name;
  }
}


//------------------------------------------------------------------------------
//   Derivatives:
//------------------------------------------------------------------------------

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setOperators}}
void intGridCollectionFunction::
setOperators(GenericGridCollectionOperators & operators0 )
//==================================================================================
// /Description:
//   Supply a derivative object to use for computing derivatives
//   on all component grids. This operator is used for the member functions
//   .x .y .z .xx .xy etc.
// /operators0: use these operators.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  operators=&operators0;
  for( int grid=0; grid< numberOfComponentGrids; grid++ )
  {
    mappedGridFunctionList[grid].setOperators((*operators)[grid]);  // set operators on component grids 
  }
}


/* ----
intGridCollectionFunction & intGridCollectionFunction::
operator ()( const Index & N )
{
  intGridCollectionFunction result;  // ***
  result=*this;                                 // ****** fix this ******
  GridCollection & cg = *(this->gridCollection);
  
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    #ifndef MGCG_OR_CG
      getIndex(cg[grid].indexRange,I1,I2,I3);
      result[grid](I1,I2,I3,N)=mappedGridFunctionList[grid].x(I1,I2,I3,N);
    #else
      result[grid](I1,I2,I3,N)=mappedGridFunctionList[grid].x(N);
    #endif
  }
  return result;
}
----------*/


//====================================================================================
// This function is used to print an error message for the derivative routines
//====================================================================================
void intGridCollectionFunction::
derivativeError() const
{
  cout << "intMappedGridFunction: ERROR: do not know how to differentiate this grid function\n"
       << "Either you are trying to differentiate an intMappedGridFunction or \n"
       << "you are trying to differentiate a int/float when real=float/int \n";
}
//====================================================================================
// supply an erro message for the BC routines
//====================================================================================
void intGridCollectionFunction::
boundaryConditionError() const
{
  cout << "intMappedGridFunction: ERROR: do not know how to apply boundary conditions to this grid function\n";
}

//==========================================================================================================
// MACRO DERIVATIVE
//  Define a macro to return the derivative "x"
// Notes:
//  o since we can only take a derivative of a realGridCollectionGridFunction we need to have a different
//    version of this macro depending on whether we compile the code with OV_USE_DOUBLE
//==========================================================================================================
#undef  ARGS1
#undef  ARGS2
#define ARGS1 const Index & C0, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4
#define ARGS2 ,C0,C1,C2,C3,C4


#ifdef OV_USE_DOUBLE
#define DERIVATIVE(x)                                                                                          \
intGridCollectionFunction intGridCollectionFunction::                                                                \
x(ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
      derivativeError(); return *this;                                                                   \
       \
       \
  }                                                                                                            \
  else                                                                                                         \
  {                                                                                                            \
    cout << "intGridCollectionFunction:ERROR:trying to take a derivative without defining a derivative routine\n";\
    return *this;                                                                                              \
  }                                                                                                            \
}
#else
#define DERIVATIVE(x)                                                                                          \
intGridCollectionFunction intGridCollectionFunction::                                                                \
x(ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
      derivativeError(); return *this;                                                                   \
       \
       \
  }                                                                                                            \
  else                                                                                                         \
  {                                                                                                            \
    cout << "intGridCollectionFunction:ERROR:trying to take a derivative without defining a derivative routine\n";\
    return *this;                                                                                              \
  }                                                                                                            \
}
#endif

// Now define all the instances of this macro

DERIVATIVE(x)
DERIVATIVE(y)
DERIVATIVE(z)
DERIVATIVE(xx)
DERIVATIVE(xy)
DERIVATIVE(xz)
DERIVATIVE(yy)
DERIVATIVE(yz)
DERIVATIVE(zz)

DERIVATIVE(laplacian)
DERIVATIVE(grad)
DERIVATIVE(div)

DERIVATIVE(r1)
DERIVATIVE(r2)
DERIVATIVE(r3)
DERIVATIVE(r1r1)
DERIVATIVE(r1r2)
DERIVATIVE(r1r3)
DERIVATIVE(r2r2)
DERIVATIVE(r2r3)
DERIVATIVE(r3r3)

DERIVATIVE(cellsToFaces)
DERIVATIVE(convectiveDerivative)
DERIVATIVE(contravariantVelocity)
DERIVATIVE(divNormal)
DERIVATIVE(normalVelocity)
DERIVATIVE(identity)
DERIVATIVE(vorticity)


#undef  ARGS1
#undef  ARGS2
#define ARGS1 const intGridCollectionFunction &w, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3

#define ARGS2 ,w,C1,C2,C3

DERIVATIVE(convectiveDerivative)


#undef  ARGS1
#undef  ARGS2
#define ARGS1 const intGridCollectionFunction &w, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4

#define ARGS2 ,w
DERIVATIVE(divScalarGrad)
DERIVATIVE(divInverseScalarGrad)

#undef  ARGS1
#define ARGS1 const int axis1, \
                    const int axis2, \
		    const int c0,  \
		    const int c1,  \
		    const int c2,  \
		    const int c3,  \
		    const int c4

#undef  ARGS2
#define ARGS2 ,axis1,axis2,c0,c1,c2,c3,c4
DERIVATIVE(faceAverage)

#undef  ARGS1
#define ARGS1 const int axis,  \
		    const int c0,  \
		    const int c1,  \
		    const int c2,  \
		    const int c3,  \
		    const int c4

#undef  ARGS2
#define ARGS2 ,axis,c0,c1,c2,c3,c4
DERIVATIVE(average)
DERIVATIVE(difference)
DERIVATIVE(dZero)


#undef  ARGS1
#define ARGS1 const int c0,  \
	      const int c1,  \
	      const int c2,  \
	      const int c3,  \
	      const int c4

#undef  ARGS2
#define ARGS2 ,c0,c1,c2,c3,c4
DERIVATIVE(CCgrad)
DERIVATIVE(FCgrad)


#undef DERIVATIVE



//==========================================================================================================
//  Define a macro to return the coefficients of the derivative "X"
// Notes:
//  o since we can only take a derivative of a realGridCollectionGridFunction we need to have a different
//    version of this macro depending on whether we compile the code with OV_USE_DOUBLE
//==========================================================================================================
#undef  ARGS1
#define ARGS1 const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4

#undef  ARGS2
#define ARGS2  C1,C2,C3,C4


#ifdef OV_USE_DOUBLE
#define DERIVATIVE_COEFFICIENTS(X)                                                                             \
intGridCollectionFunction intGridCollectionFunction::                                                                \
X(ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
      derivativeError(); return *this;                                                                   \
       \
       \
  }                                                                                                            \
  else                                                                                                         \
  {                                                                                                            \
    cout << "intGridCollectionFunction:ERROR:trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                              \
  }                                                                                                            \
}
#else
#define DERIVATIVE_COEFFICIENTS(X)                                                                             \
intGridCollectionFunction intGridCollectionFunction::                                                                \
X(ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
      derivativeError(); return *this;                                                                   \
       \
       \
  }                                                                                                            \
  else                                                                                                         \
  {                                                                                                            \
    cout << "intGridCollectionFunction:ERROR:trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                              \
  }                                                                                                            \
}
#endif

// Now define all the instances of this macro

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
DERIVATIVE_COEFFICIENTS(identityCoefficients)
DERIVATIVE_COEFFICIENTS(divCoefficients)

DERIVATIVE_COEFFICIENTS(r1Coefficients)
DERIVATIVE_COEFFICIENTS(r2Coefficients)
DERIVATIVE_COEFFICIENTS(r3Coefficients)
DERIVATIVE_COEFFICIENTS(r1r1Coefficients)
DERIVATIVE_COEFFICIENTS(r1r2Coefficients)
DERIVATIVE_COEFFICIENTS(r1r3Coefficients)
DERIVATIVE_COEFFICIENTS(r2r2Coefficients)
DERIVATIVE_COEFFICIENTS(r2r3Coefficients)
DERIVATIVE_COEFFICIENTS(r3r3Coefficients)

DERIVATIVE_COEFFICIENTS(gradCoefficients);


#undef  ARGS1
#define ARGS1 const intGridCollectionFunction &s, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4
#undef  ARGS2
#define ARGS2 s

DERIVATIVE_COEFFICIENTS(divScalarGradCoefficients);
DERIVATIVE_COEFFICIENTS(divInverseScalarGradCoefficients);


#undef DERIVATIVE_COEFFICIENTS
#undef ARGS1
#undef ARGS2


#undef DERIVATIVE_COEFFICIENTS


/* ----- re-think this for grid collections: 
void intGridCollectionFunction::
getDerivatives( const Index & N )
{
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
    mappedGridFunctionList[grid].getDerivatives( I1,I2,I3,N );
}
------ */



