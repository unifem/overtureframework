// This file defines common functions for gridCollectionFunction and compositeGridFunction's
// define COLLECTION_FUNCTION to be typeGridCollectionFunction or typeCompositeGridFunction
// define INT_COLLECTION_FUNCTION to be intGridCollectionFunction or intCompositeGridFunction
// define COLLECTION to be GridCollection or CompositeGrid
// define QUOTES_COLLECTION_FUNCTION to be "typeGridCollectionFunction" or "typeCompositeGridFunction"
// one of INT_COLLECTION_FUNCTION FLOAT_COLLECTION_FUNCTION or DOUBLE_COLLECTION_FUNCTION should
// be defined
// define the keyword INTEGRAL_TYPE to be one of int, float double

#ifdef MGCG
#define NG numberOfMultigridLevels()
#define PAAAAP
#else
#define NG numberOfGrids()
#define PAAAAP (All,All,All,All)
#endif

// put quotes around the argument
#define QUOTES(a) #a

INTEGRAL_TYPE 
max( const COLLECTION_FUNCTION & cgf )
{
  INTEGRAL_TYPE maximum=0;
  for( int grid=0; grid< cgf.NG; grid++ )
    maximum=max(maximum,max(cgf[grid]));
  if( cgf.temporary )  // delete the rhs if it is a temporary 
  { // This is a fudge -- cast away the const  
    COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf; 
    delete temp;                                                
  }                                                             
  return maximum;
}

INTEGRAL_TYPE 
min( const COLLECTION_FUNCTION & cgf ) 
{
  INTEGRAL_TYPE minimum=0;
  if( cgf.NG > 0 ) 
  {
    minimum=min(cgf[0]);
    for( int grid=1; grid< cgf.NG; grid++ )
      minimum=min(minimum,min(cgf[grid]));
  }
  if( cgf.temporary )  // delete the rhs if it is a temporary 
  { // This is a fudge -- cast away the const  
    COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf; 
    delete temp;                                                
  }                                                             
  return minimum;
}




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//   Define operators + - * / += -= *= /=
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//==========================================================================
//  This macro defines one of the operators + - * or /
//  for   COLLECTION_FUNCTION XX COLLECTION_FUNCTION
//==========================================================================
#define OPERATOR(XX)                                              \
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                 \
operator XX( const COLLECTION_FUNCTION & cgf ) const           \
{                                                                 \
  COLLECTION_FUNCTION *cgfn= new COLLECTION_FUNCTION();     \
  (*cgfn)=*this;                                                  \
  for( int grid=0; grid< NG; grid++ )         \
    (*cgfn)[grid] XX=cgf[grid];                                   \
  return *cgfn;                                                   \
}

/* OPERATOR(+) */
/* OPERATOR(-) */
/* OPERATOR(*) */
/* OPERATOR(/) */
  
#undef OPERATOR

//==========================================================================
//  This macro defines one of the operators + * 
//  for   COLLECTION_FUNCTION XX COLLECTION_FUNCTION
//==========================================================================
#define COMMUTATIVE_OPERATOR(XX)                                    \
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                   \
operator XX( const COLLECTION_FUNCTION & cgf ) const             \
{                                                                   \
  COLLECTION_FUNCTION *cgfn;                                     \
  if( temporary )                                                   \
  {                                                                 \
    cgfn=(COLLECTION_FUNCTION*)this;                             \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid] XX=cgf[grid];                                   \
    if( cgf.temporary )  /* delete the rhs if it is a temporary  */ \
    { /* This is a fudge -- cast away the const */                  \
      COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf;\
      delete temp;                                                  \
    }                                                               \
  }                                                                 \
  else if( cgf.temporary )                                          \
  { /* this works for coumutative operations, t=a+t -> t+=a  */     \
    cgfn=(COLLECTION_FUNCTION*)&cgf;                             \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid] XX=(*this)[grid];                               \
  }                                                                 \
  else                                                              \
  { /* neither operand is a temporary -- make a temporary to use */ \
    cgfn= new COLLECTION_FUNCTION();                             \
    (*cgfn).temporary=TRUE;                                         \
    (*cgfn).updateToMatchGridFunction(cgf);                         \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid] = (*this)[grid] XX cgf[grid];                   \
  }                                                                 \
  return *cgfn;                                                     \
}


// COMMUTATIVE_OPERATOR(+)
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                   
operator +( const COLLECTION_FUNCTION & cgf ) const             
{                                                                   
  COLLECTION_FUNCTION *cgfn;                                     
  if( temporary )                                                   
  {                                                                 
    cgfn=(COLLECTION_FUNCTION*)this;                             
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid] +=cgf[grid];                                   
    if( cgf.temporary )  /* delete the rhs if it is a temporary  */ 
    { /* This is a fudge -- cast away the const */                  
      COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf;
      delete temp;                                                  
    }                                                               
  }                                                                 
  else if( cgf.temporary )                                          
  { /* this works for coumutative operations, t=a+t -> t+=a  */     
    cgfn=(COLLECTION_FUNCTION*)&cgf;                             
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid] +=(*this)[grid];                               
  }                                                                 
  else                                                              
  { /* neither operand is a temporary -- make a temporary to use */ 
    cgfn= ::new COLLECTION_FUNCTION();                             
    (*cgfn).temporary=TRUE;                                         
    (*cgfn).updateToMatchGridFunction(cgf);                         
    Index All;
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid] PAAAAP = (*this)[grid]PAAAAP + cgf[grid]PAAAAP;         
  }                                                                 
  return *cgfn;                                                     
}


// COMMUTATIVE_OPERATOR(*)
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                   
operator *( const COLLECTION_FUNCTION & cgf ) const             
{                                                                   
  COLLECTION_FUNCTION *cgfn;                                     
  if( temporary )                                                   
  {                                                                 
    cgfn=(COLLECTION_FUNCTION*)this;                             
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid] *=cgf[grid];                                   
    if( cgf.temporary )  /* delete the rhs if it is a temporary  */ 
    { /* This is a fudge -- cast away the const */                  
      COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf;
      delete temp;                                                  
    }                                                               
  }                                                                 
  else if( cgf.temporary )                                          
  { /* this works for coumutative operations, t=a+t -> t+=a  */     
    cgfn=(COLLECTION_FUNCTION*)&cgf;                             
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid] *=(*this)[grid];                               
  }                                                                 
  else                                                              
  { /* neither operand is a temporary -- make a temporary to use */ 
    cgfn= ::new COLLECTION_FUNCTION();                             
    (*cgfn).temporary=TRUE;                                         
    (*cgfn).updateToMatchGridFunction(cgf);                         
    Index All;
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid]PAAAAP = (*this)[grid]PAAAAP * cgf[grid]PAAAAP;          
  }                                                                 
  return *cgfn;                                                     
}

#undef COMMUTATIVE_OPERATOR

//==========================================================================
//  This macro defines one of the operators + * 
//  for   COLLECTION_FUNCTION XX COLLECTION_FUNCTION
// Note: for the gnu compiler we add the argument XXE which should be XX=
//==========================================================================
#define NON_COMMUTATIVE_OPERATOR(XX,XXE)                                \
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                   \
operator XX ( const COLLECTION_FUNCTION & cgf ) const            \
{                                                                   \
  COLLECTION_FUNCTION *cgfn;                                     \
  if( temporary )                                                   \
  {   \
    cgfn=(COLLECTION_FUNCTION*)this;                             \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid] XXE cgf[grid];                                  \
    /* delete the rhs if it is a temporary */                       \
    if( cgf.temporary )                                             \
    {                                                               \
      COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf; \
      delete temp;                                                  \
    }                                                               \
  }                                                                 \
  else if( cgf.temporary )                                          \
  {                                                                 \
    cgfn=(COLLECTION_FUNCTION*)&cgf;                             \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid] =(*this)[grid] XX (*cgfn)[grid];                \
  }                                                                 \
  /* neither operand is a temporary -- make a temporary to use */   \
  else                                                              \
  {                                                                 \
    cgfn= new COLLECTION_FUNCTION();                             \
    (*cgfn).temporary=TRUE;                                         \
    (*cgfn).updateToMatchGridFunction(cgf);                         \
    Index All;                                                      \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid]PAAAAP = (*this)[grid]PAAAAP XX cgf[grid]PAAAAP;                   \
  }                                                                 \
  return *cgfn;                                                     \
}

NON_COMMUTATIVE_OPERATOR(/,/=)
NON_COMMUTATIVE_OPERATOR(-,-=)

#undef NON_COMMUTATIVE_OPERATOR




//==========================================================================
//  This macro defines one of the operators += -= *= or /=
//  for   COLLECTION_FUNCTION XX COLLECTION_FUNCTION
//==========================================================================
#define OPERATOR(XX)                                              \
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                 \
operator XX( const COLLECTION_FUNCTION & cgf )                 \
{                                                                 \
  for( int grid=0; grid< NG; grid++ )         \
    (*this)[grid] XX cgf[grid];                                   \
  if( cgf.temporary )  /* delete the rhs if it is a temporary */  \
  { /* This is a fudge -- cast away the const  */                   \
    COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf;  \
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
//  This macro defines one of the operators + - * or / times a INTEGRAL_TYPE
// Note: for the gnu compiler we add the argument XXE which should be XX=
//==========================================================================
#define OPERATOR(XX,XXE)                                                \
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                   \
operator XX( const INTEGRAL_TYPE value ) const                             \
{                                                                   \
  COLLECTION_FUNCTION *cgfn;                                     \
  if( temporary )                                                   \
  {                                                                 \
    cgfn=(COLLECTION_FUNCTION *)this;                            \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid] XXE value;                                       \
  }                                                                 \
  else                                                              \
  {                                                                 \
    cgfn= new COLLECTION_FUNCTION();                             \
    (*cgfn).temporary=TRUE;                                         \
    (*cgfn).updateToMatchGridFunction(*this);                       \
    Index All;                                                      \
    for( int grid=0; grid< NG; grid++ )         \
      (*cgfn)[grid]PAAAAP = (*this)[grid]PAAAAP XX value;                       \
  }                                                                 \
  return *cgfn;                                                     \
}

OPERATOR(+,+=)
OPERATOR(-,-=)
OPERATOR(*,*=)
OPERATOR(/,/=)
  
#undef OPERATOR

// ========================================================================
//   Unary operators
// ========================================================================
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                   
operator -() const             
{                                                                   
  COLLECTION_FUNCTION *cgfn;                                     
  if( temporary )                                                   
  {                                                                 
    cgfn=(COLLECTION_FUNCTION*)this;                             
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid] =-(*cgfn)[grid];                                   
  }                                                                 
  else                                                              
  { /* "this" is not a temporary -- make a temporary to use */ 
    cgfn= ::new COLLECTION_FUNCTION();                             
    (*cgfn).temporary=TRUE;                                         
    (*cgfn).updateToMatchGridFunction(*this);                         
    for( int grid=0; grid< NG; grid++ )         
      (*cgfn)[grid] = -(*this)[grid];
  }                                                                 
  return *cgfn;                                                     
}
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                   
operator +() const             
{                                                                   
  COLLECTION_FUNCTION *cgfn;                                     
  if( temporary )                                                   
  {                                                                 
    cgfn=(COLLECTION_FUNCTION*)this;                             
  }                                                                 
  else                                                              
  {  /* "this" is not a temporary -- make a temporary to use */ 
    cgfn= ::new COLLECTION_FUNCTION();                             
    (*cgfn).temporary=TRUE;                                         
    (*cgfn)=(*this);
  }                                                                 
  return *cgfn;                                                     
}



//==========================================================================
//  This macro defines one of the operators = +=  -= *= or /=  
//  for   COLLECTION_FUNCTION XX INTEGRAL_TYPE
//==========================================================================
#define OPERATOR(XX)                                              \
COLLECTION_FUNCTION & COLLECTION_FUNCTION::                 \
operator XX( const INTEGRAL_TYPE value )                                 \
{                                                                 \
  for( int grid=0; grid< NG; grid++ )         \
    (*this)[grid] XX value;                                       \
  return *this;                                                   \
}

OPERATOR(=)
OPERATOR(+=)
OPERATOR(-=)
OPERATOR(*=)
OPERATOR(/=)

#undef OPERATOR


// ========================================================================
//    Comparison operators
// ========================================================================
// Note that the expression 
//   floatGridCollectionFunction b,c,d;
//   intGridCollectionFunction a;
//     a = (b+c) < d ; 
// will cause a leak since the temporary for (b+c) will not be deleted !!

#define OPERATOR(XX) \
INT_COLLECTION_FUNCTION COLLECTION_FUNCTION::                     \
operator XX ( const COLLECTION_FUNCTION & cgf ) const               \
{                                                                     \
  assert(gridCollection!=NULL);  \
  if( cgf.numberOfGrids() != numberOfComponentGrids() ) \
  { \
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR: cannot compare two grid functions with different number of grids \n"; \
    cout << "left operand has " << numberOfGrids() << " grids, right operand has " << cgf.numberOfGrids() << " grids\n"; \
    throw "error"; \
  } \
  \
  /* get dimension bounds so we can make a grid function of the correct shape */  \
  Range *R = rcData->R;  \
  Range Ru[maximumNumberOfIndicies];  \
  \
  int i; \
  for( i=0; i<maximumNumberOfIndicies; i++ )  \
    Ru[i]=nullRange;  \
  int numberOfComponents=getNumberOfComponents();  \
  for( i=0; i<numberOfComponents; i++ )  \
    if( positionOfComponent(i)<maximumNumberOfIndicies )  \
      Ru[positionOfComponent(i)]=R[positionOfComponent(i)];  \
  \
  INT_COLLECTION_FUNCTION cgfn((COLLECTION &)(*gridCollection),Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7]);  \
  \
  for( int grid=0; grid< NG; grid++ )           \
    cgfn[grid]= (*this)[grid] XX cgf[grid];  \
  \
  if( cgf.temporary )  /* delete the rhs if it is a temporary  */   \
  { /* This is a fudge -- cast away the const */                    \
    COLLECTION_FUNCTION *temp = (COLLECTION_FUNCTION*) &cgf;  \
    delete temp;                                                    \
  }                                                                 \
  return cgfn;  \
}

OPERATOR(<)
OPERATOR(<=)
OPERATOR(>)
OPERATOR(>=)
OPERATOR(==)
OPERATOR(!=)

#undef OPERATOR


// ---------------------friend functions------------------------

//==========================================================================
//  This macro defines one of the operators + - * / 
//  for   INTEGRAL_TYPE XX COLLECTION_FUNCTION
//==========================================================================
#define OPERATOR(XX)                                              \
COLLECTION_FUNCTION &                                          \
operator XX( const INTEGRAL_TYPE u, const COLLECTION_FUNCTION & U)    \
{                                                                 \
  return U XX u;                                                  \
}

OPERATOR(+)
OPERATOR(*)
// wrong: OPERATOR(-)
// wrong: OPERATOR(/)

#undef OPERATOR



//==========================================================================
//  This macro defines one of the operators + * 
//  for   INTEGRAL_TYPE XX COLLECTION_FUNCTION
//==========================================================================
#define NON_COMMUTATIVE_OPERATOR(XX)                                \
COLLECTION_FUNCTION &  \
operator XX ( const INTEGRAL_TYPE u, const COLLECTION_FUNCTION & cgf )         \
{                                                                   \
  COLLECTION_FUNCTION *cgfn;                                     \
  if( cgf.temporary )                                          \
  {                                                                 \
    cgfn=(COLLECTION_FUNCTION*)&cgf;                             \
    for( int grid=0; grid< cgf.NG; grid++ )         \
      (*cgfn)[grid] =u  XX (*cgfn)[grid];                \
  }                                                                 \
  /* neither operand is a temporary -- make a temporary to use */   \
  else                                                              \
  {                                                                 \
    cgfn= new COLLECTION_FUNCTION();                             \
    (*cgfn).temporary=TRUE;                                         \
    (*cgfn).updateToMatchGridFunction(cgf);                         \
    Index All;                                                      \
    for( int grid=0; grid< cgf.NG; grid++ )         \
      (*cgfn)[grid]PAAAAP = u XX cgf[grid]PAAAAP;                   \
  }                                                                 \
  return *cgfn;                                                     \
}

NON_COMMUTATIVE_OPERATOR(/)
NON_COMMUTATIVE_OPERATOR(-)

#undef NON_COMMUTATIVE_OPERATOR







//====================================================================================
// This function is used to print an error message for the derivative routines
//====================================================================================
void COLLECTION_FUNCTION::
derivativeError() const
{
  cout << QUOTES_COLLECTION_FUNCTION ": ERROR: do not know how to differentiate this grid function\n"
       << "Either you are trying to differentiate an int" QUOTES_COLLECTION_FUNCTION "or \n"
       << "you are trying to differentiate a double/float when real=float/double \n";
}
//====================================================================================
// supply an erro message for the BC routines
//====================================================================================
void COLLECTION_FUNCTION::
boundaryConditionError() const
{
  cout << QUOTES_COLLECTION_FUNCTION ": ERROR: do not know how to apply boundary conditions to this grid function\n";
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



// --------Define the RETURN STATEMENT that appears in the macro to follow------------------
#undef RETURN_STATEMENT
#undef RETURN_STATEMENT2
#ifdef OV_USE_DOUBLE

  #if defined(DOUBLE_COLLECTION_FUNCTION)
    #define RETURN_STATEMENT(x)  return getOperators()->x(*this ARGS2);
    #define RETURN_STATEMENT2(x) return getOperators()->x(*this,gfType ARGS2);
  #else
    #define RETURN_STATEMENT(x)  derivativeError(); return *this;
    #define RETURN_STATEMENT2(x) derivativeError(); return *this;
#endif

#else

  #if defined(FLOAT_COLLECTION_FUNCTION)
    #define RETURN_STATEMENT(x)  return getOperators()->x(*this ARGS2);
    #define RETURN_STATEMENT2(x) return getOperators()->x(*this,gfType ARGS2);
  #else
    #define RETURN_STATEMENT(x)  derivativeError(); return *this;
    #define RETURN_STATEMENT2(x) derivativeError(); return *this;
  #endif

#endif 
// ---------------------------------------------------------



#define DERIVATIVE(x)                                                                               \
COLLECTION_FUNCTION COLLECTION_FUNCTION::                                                                \
x(ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
    RETURN_STATEMENT(x) \
  }  \
  else                                                                                                         \
  {                                                                                                            \
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to take a derivative without defining a derivative routine\n";\
    return *this;                                                                                              \
  }                                                                                                            \
} \
COLLECTION_FUNCTION COLLECTION_FUNCTION::                                                                \
x(const GridFunctionParameters & gfType, ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
    RETURN_STATEMENT2(x) \
  }  \
  else                                                                                                         \
  {                                                                                                            \
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to take a derivative without defining a derivative routine\n";\
    return *this;                                                                                              \
  }                                                                                                            \
}

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
#define ARGS1 const COLLECTION_FUNCTION &w, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3

#define ARGS2 ,w,C1,C2,C3

DERIVATIVE(convectiveDerivative)


#undef  ARGS1
#undef  ARGS2
#define ARGS1 const COLLECTION_FUNCTION &w, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4

#define ARGS2 ,w
DERIVATIVE(divScalarGrad)
DERIVATIVE(divInverseScalarGrad)
DERIVATIVE(scalarGrad)
DERIVATIVE(divVectorScalar)

#undef  ARGS1
#undef  ARGS2
#define ARGS1 const COLLECTION_FUNCTION &w, \
              const int & direction1, \
              const int & direction2, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4

#define ARGS2 ,w,direction1,direction2
DERIVATIVE(derivativeScalarDerivative)


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


// --------Define the RETURN STATEMENT that appears in the macro to follow------------------
#undef RETURN_STATEMENT
#undef RETURN_STATEMENT2
#ifdef OV_USE_DOUBLE

  #if defined(DOUBLE_COLLECTION_FUNCTION)
    #define RETURN_STATEMENT(x)  return getOperators()->x(ARGS2);
    #define RETURN_STATEMENT2(x) return getOperators()->x(gfType, ARGS2);
  #else 
    #define RETURN_STATEMENT(x)  derivativeError(); return *this;
    #define RETURN_STATEMENT2(x) derivativeError(); return *this;
  #endif

#else

  #if defined(FLOAT_COLLECTION_FUNCTION)
    #define RETURN_STATEMENT(x)  return getOperators()->x(ARGS2);
    #define RETURN_STATEMENT2(x) return getOperators()->x(gfType, ARGS2);
  #else
    #define RETURN_STATEMENT(x)  derivativeError(); return *this;
    #define RETURN_STATEMENT2(x) derivativeError(); return *this;
  #endif

#endif 
// ---------------------------------------------------------

#define DERIVATIVE_COEFFICIENTS(X)                                                                    \
COLLECTION_FUNCTION COLLECTION_FUNCTION::                                                                \
X(ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
    RETURN_STATEMENT(X)                                                                                  \
  }                                                                                                            \
  else                                                                                                         \
  {                                                                                                            \
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                              \
  }                                                                                                            \
}  \
COLLECTION_FUNCTION COLLECTION_FUNCTION::                                                                \
X(const GridFunctionParameters & gfType, ARGS1) const                                \
{                                                                                                              \
  if( operators!=NULL )                                                                                        \
  {                                                                                                            \
    RETURN_STATEMENT2(X)                                                                                  \
  }                                                                                                            \
  else                                                                                                         \
  {                                                                                                            \
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                              \
  }                                                                                                            \
}

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

DERIVATIVE_COEFFICIENTS(gradCoefficients)


#undef  ARGS1
#define ARGS1 const COLLECTION_FUNCTION &s, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4
#undef  ARGS2
#define ARGS2 s

DERIVATIVE_COEFFICIENTS(divScalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(divInverseScalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(scalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(divVectorScalarCoefficients)

#undef  ARGS1
#define ARGS1 const COLLECTION_FUNCTION &s, \
              const int & direction1, \
              const int & direction2, \
              const Index & C1, \
              const Index & C2, \
              const Index & C3, \
              const Index & C4
#undef  ARGS2
#define ARGS2 s,direction1,direction2

DERIVATIVE_COEFFICIENTS(derivativeScalarDerivativeCoefficients)


#undef DERIVATIVE_COEFFICIENTS
#undef RETURN_STATEMENT
#undef RETURN_STATEMENT2
#undef ARGS1
#undef ARGS2




//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{applyBoundaryConditions}}
void COLLECTION_FUNCTION:: 
applyBoundaryConditions(const real & time /* = 0. */ , 
                        const int & grid0  /* =forAll */ )
//==================================================================================
// /Description:
//   Apply the boundary conditions to this grid function. This routine just calls the 
//   function of the same name in the MappedGridOperators.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( getOperators()!=NULL )                         
  {                                                                                                           
    #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
        (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
      getOperators()->applyBoundaryConditions(*this,time,grid0);
    #else
      boundaryConditionError();
    #endif
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to applyBoundaryConditions without defining an BC routine\n";
    Overture::abort("error");
  }                                                                                                           
}
                                 
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{ApplyBoundaryConditions}}
void COLLECTION_FUNCTION:: 
assignBoundaryConditionCoefficients(const real & time /* = 0. */ , 
                                    const int & grid0   /* =forAll */ )
//==================================================================================
// /Description:
//   Fill in the coefficients of the boundary conditions into this grid function. 
//   This routine just calls the function of the same name in the MappedGridOperators.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( getOperators()!=NULL )                       
  {                                                                                                           
    #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
        (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
      getOperators()->assignBoundaryConditionCoefficients(*this,time,grid0);
    #else
      boundaryConditionError();
    #endif
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to assignBoundaryConditionCoefficients without defining an BC routine\n";
  }                                                                                                           
}

  // new BC interface:
void COLLECTION_FUNCTION::
applyBoundaryCondition(const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const real & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters )
{
  if( getOperators()!=NULL )                         
  {                                                                                                           
    #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
        (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
       getOperators()->applyBoundaryCondition( *this, Components,bcType,bc,forcing,time,bcParameters );
    #else
      boundaryConditionError();
    #endif
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to applyBoundaryConditions without defining an BC routine\n";
    Overture::abort("error");
  }                                                                                                           
}


void COLLECTION_FUNCTION::
applyBoundaryCondition(const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const RealArray & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters )
{
  if( getOperators()!=NULL )                         
  {                                                                                                           
    #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
        (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
       getOperators()->applyBoundaryCondition( *this, Components,bcType,bc,forcing,time,bcParameters );
    #else
      boundaryConditionError();
    #endif
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to applyBoundaryCondition without defining operators\n";
    Overture::abort("error");
  }                                                                                                           
}

void COLLECTION_FUNCTION::
applyBoundaryCondition(const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const COLLECTION_FUNCTION & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters )
{
  if( getOperators()!=NULL )                         
  {                                                                                                           
    #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
        (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
       getOperators()->applyBoundaryCondition( *this, Components,bcType,bc,forcing,time,bcParameters );
    #else
      boundaryConditionError();
    #endif
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to applyBoundaryCondition without defining operators\n";
    Overture::abort("error");
  }                                                                                                           
}

#ifdef USE_PPP
// void COLLECTION_FUNCTION::
// applyBoundaryCondition(const Index & Components,
// 		       const BCTypes::BCNames & bcType,
// 		       const int & bc,
// 		       const RealDistributedArray & forcing,
// 		       const real & time,
// 		       const BoundaryConditionParameters & bcParameters )
// {
//   if( getOperators()!=NULL )                         
//   {                                                                                                           
//     #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
//         (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
//        getOperators()->applyBoundaryCondition( *this, Components,bcType,bc,forcing,time,bcParameters );
//     #else
//       boundaryConditionError();
//     #endif
//   }                                                                                                           
//   else                                                                                                        
//   {                                                                                                           
//     cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to applyBoundaryCondition without defining operators\n";
//     Overture::abort("error");
//   }                                                                                                           
// }
#endif


// fix corners and periodic update:
void COLLECTION_FUNCTION::
finishBoundaryConditions(const BoundaryConditionParameters & bcParameters,
                         const Range & C0,
                         const IntegerArray & gridsToUpdate /* = Overture::nullIntArray() */ )
{
  if( getOperators()!=NULL )                         
  {                                                                                                           
    #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
        (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
       getOperators()->finishBoundaryConditions(*this,bcParameters,C0,gridsToUpdate);
    #else
      boundaryConditionError();
    #endif
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to applyBoundaryCondition without defining operators\n";
    Overture::abort("error");
  }                                                                                                           
}

void COLLECTION_FUNCTION::
applyBoundaryConditionCoefficients(const Index & Equation,
                                   const Index & Components,
				   const BCTypes::BCNames & bcType,
				   const int & bc,
				   const BoundaryConditionParameters & bcParameters )
{
  if( getOperators()!=NULL )                         
  {                                                                                                           
    #if ( defined(OV_USE_DOUBLE) & defined(DOUBLE_COLLECTION_FUNCTION) ) | \
        (!defined(OV_USE_DOUBLE) & defined(FLOAT_COLLECTION_FUNCTION) )
       getOperators()->applyBoundaryConditionCoefficients( *this, Equation,Components,bcType,bc,bcParameters );
    #else
      boundaryConditionError();
    #endif
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << QUOTES_COLLECTION_FUNCTION ":ERROR:trying to applyBoundaryConditionCoefficients without defining operators\n";
    Overture::abort("error");
  }                                                                                                           
}

