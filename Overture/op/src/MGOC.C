#include "MappedGridOperators.h"
#include "ParallelUtility.h"

//=============================================================================================
// This macro defines a function to return the coefficients of derivative "X"
// Notes:
//   X : function name
//   derivativeType : derivative type
//=============================================================================================
#define DERIVATIVE_COEFFICIENTS(X,derivativeType)                                       \
realMappedGridFunction MappedGridOperators::                                            \
X(const Index & I1_, const Index & I2_, const Index & I3_, const Index & I4_,               \
  const Index & I5_, const Index & I6_, const Index & I7_, const Index & I8_ )              \
{                                                                                       \
  return xiCoefficients(derivativeType,I1_,I2_,I3_,I4_,I5_);                                 \
}                 \
realMappedGridFunction MappedGridOperators::                                            \
X(const GridFunctionParameters & gfType,   \
  const Index & I1_, const Index & I2_, const Index & I3_, const Index & I4_,               \
  const Index & I5_, const Index & I6_, const Index & I7_, const Index & I8_ )              \
{                                                                                       \
  return xiCoefficients(derivativeType,I1_,I2_,I3_,I4_,I5_);                                 \
}                 

// Now define instances of this macro for each of the derivatives we know how to compute

DERIVATIVE_COEFFICIENTS(xCoefficients,xDerivative)
DERIVATIVE_COEFFICIENTS(yCoefficients,yDerivative)
DERIVATIVE_COEFFICIENTS(zCoefficients,zDerivative)
DERIVATIVE_COEFFICIENTS(xxCoefficients,xxDerivative)
DERIVATIVE_COEFFICIENTS(xyCoefficients,xyDerivative)
DERIVATIVE_COEFFICIENTS(xzCoefficients,xzDerivative)
DERIVATIVE_COEFFICIENTS(yyCoefficients,yyDerivative)
DERIVATIVE_COEFFICIENTS(yzCoefficients,yzDerivative)
DERIVATIVE_COEFFICIENTS(zzCoefficients,zzDerivative)

DERIVATIVE_COEFFICIENTS(laplacianCoefficients,laplacianOperator)
DERIVATIVE_COEFFICIENTS(divCoefficients,divergence)
DERIVATIVE_COEFFICIENTS(identityCoefficients,identityOperator)

DERIVATIVE_COEFFICIENTS(r1Coefficients,r1Derivative)
DERIVATIVE_COEFFICIENTS(r2Coefficients,r2Derivative)
DERIVATIVE_COEFFICIENTS(r3Coefficients,r3Derivative)
DERIVATIVE_COEFFICIENTS(r1r1Coefficients,r1r1Derivative)
DERIVATIVE_COEFFICIENTS(r1r2Coefficients,r1r2Derivative)
DERIVATIVE_COEFFICIENTS(r1r3Coefficients,r1r3Derivative)
DERIVATIVE_COEFFICIENTS(r2r2Coefficients,r2r2Derivative)
DERIVATIVE_COEFFICIENTS(r2r3Coefficients,r2r3Derivative)
DERIVATIVE_COEFFICIENTS(r3r3Coefficients,r3r3Derivative)

// DERIVATIVE_COEFFICIENTS(gradCoefficients,gradient)
#undef DERIVATIVE_COEFFICIENTS

realMappedGridFunction MappedGridOperators::                                           
gradCoefficients(const Index & I1_, const Index & I2_, const Index & I3_, const Index & I4_, 
  const Index & I5_, const Index & I6_, const Index & I7_, const Index & I8_ )             
{                                                                                      
  assert( numberOfComponentsForCoefficients==1 );
  Range all;
  int stencilDimension=stencilSize*numberOfDimensions;
  realMappedGridFunction derivative(mappedGrid,stencilDimension,all,all,all);
  derivative=0.; 
  
  int e0=I4_.getBase();
  int c0=I5_.getBase();

  Index M=Index(0,stencilSize);
  derivative(M+stencilSize*c0,I1_,I2_,I3_)=xCoefficients(I1_,I2_,I3_)(M,I1_,I2_,I3_);
  if( numberOfDimensions>1 )
  {
    c0++;
    derivative(M+stencilSize*c0,I1_,I2_,I3_)=yCoefficients(I1_,I2_,I3_)(M,I1_,I2_,I3_); 
  }
  if( numberOfDimensions>2 )
  {
    c0++;
    derivative(M+stencilSize*c0,I1_,I2_,I3_)=zCoefficients(I1_,I2_,I3_)(M,I1_,I2_,I3_); 
  }
  return derivative;
} 


realMappedGridFunction MappedGridOperators::                                 
gradCoefficients(const GridFunctionParameters & gfType,  
  const Index & I1_, const Index & I2_, const Index & I3_, const Index & I4_,  
  const Index & I5_, const Index & I6_, const Index & I7_, const Index & I8_ ) 
{                                                                              
  return gradCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);                 
}                 



//---------------------------------------------------------------------------------------
//   Return a derivative
//
//  E : equation number(s) in a system of equations
//  C : component number(s) in a system of equations
//---------------------------------------------------------------------------------------
realMappedGridFunction MappedGridOperators::
xiCoefficients(const derivativeTypes & derivativeType0, 
	       const Index & I1_, 
	       const Index & I2_, 
	       const Index & I3_,
	       const Index & E,
	       const Index & C )
{
  // real time = getCPU();
  
  MappedGrid & c = mappedGrid;
  numberOfDimensions=c.numberOfDimensions();

  // Determine ranges over which to compute the derivatives
  //   by default do as many points as possible, given the width of the stencil
  Range R1,R2,R3,R4,R5; 
  int w0 = orderOfAccuracy/2;
  int w1 = numberOfDimensions>1 ? w0 : 0;
  int w2 = numberOfDimensions>2 ? w0 : 0;
  R1= I1_.length()==0 ? Range(c.dimension(Start,0)+w0,c.dimension(End,0)-w0) : Range(I1_.getBase(),I1_.getBound());
  R2= I2_.length()==0 ? Range(c.dimension(Start,1)+w1,c.dimension(End,1)-w1) : Range(I2_.getBase(),I2_.getBound());
  R3= I3_.length()==0 ? Range(c.dimension(Start,2)+w2,c.dimension(End,2)-w2) : Range(I3_.getBase(),I3_.getBound());
  R4=   E.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  E.getBase(),  E.getBound());
  R5=   C.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  C.getBase(),  C.getBound());

  Range all;
  int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);
  
  realMappedGridFunction uX(c,stencilDimension,all,all,all);
  uX=0.;

  // don't do this anymore --- uX just held unused memory.
//   int update=uX.updateToMatchGrid(c,stencilDimension,all,all,all);
//   if( update!=0 )  // if the grid sizes have changed we should initialize the grid function to zero
//     uX=0.;         // This gives default values to points not defined below
//   else if( R4.length()!=numberOfComponentsForCoefficients || R5.length()!=numberOfComponentsForCoefficients )
//   { // zero out unused components!   ***** could do better here *******
//     uX=0.;      
//   }

  // ----------------------------------------------------------------
  // -------------Evaluate the derivative Coefficients---------------
  // ----------------------------------------------------------------
  if( derivativeType0 >= r1Derivative && derivativeType0 <= r3r3Derivative )
  {
    coefficients(derivativeType0,uX,R1,R2,R3,R4,R5);
  }
  else
  {
    assert(derivCoefficientsFunction[derivativeType0]!=NULL);
    (*derivCoefficientsFunction[derivativeType0])(uX,R1,R2,R3,R4,R5,*this);
  }
  
  // time=getCPU()-time;
  // printf("xiCoefficients: time = %e \n",time);

  return uX;
    
}

void 
divScalarGradFDerivCoefficients(RealDistributedArray & derivative,
				const realMappedGridFunction & s,
				const Index & I1,
				const Index & I2,
				const Index & I3,
				const Index & E,
				const Index & C,
				MappedGridOperators & mgop );

void 
derivativeScalarDerivativeFDerivCoefficients(RealDistributedArray & derivative,
					     const realMappedGridFunction & s,
					     const int & direction1,
					     const int & direction2,
					     const Index & I1,
					     const Index & I2,
					     const Index & I3,
					     const Index & E,
					     const Index & C,
					     MappedGridOperators & mgop );

void 
scalarGradFDerivCoefficients(RealDistributedArray & derivative,
				const realMappedGridFunction & s,
				const Index & I1,
				const Index & I2,
				const Index & I3,
				const Index & E,
				const Index & C,
				MappedGridOperators & mgop );

void 
divVectorScalarFDerivCoefficients(RealDistributedArray & derivative,
				  const realMappedGridFunction & s,
				  const Index & I1,
				  const Index & I2,
				  const Index & I3,
				  const Index & E,
				  const Index & C,
				  MappedGridOperators & mgop );



//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{divScalarGradCoefficients}}
realMappedGridFunction MappedGridOperators::
divScalarGradCoefficients(const realMappedGridFunction & scalar,
			  const Index & I1_ /* = nullIndex */,
			  const Index & I2_ /* = nullIndex */,
			  const Index & I3_ /* = nullIndex */,
			  const Index & I4_ /* = nullIndex */,
			  const Index & I5_ /* = nullIndex */,
			  const Index & I6_ /* = nullIndex */,
			  const Index & I7_ /* = nullIndex */,
			  const Index & I8_ /* = nullIndex */ )
//=======================================================================================
// /Description:
//   Form the coefficient matrix for the operator $\grad\cdot(\rm{scalar}\grad )$.
// /scalar (input) : coefficient that appears in the operator.
// /Author: WDH
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{

  if( !conservative || orderOfAccuracy==4 )
  {
    realMappedGridFunction result;
    result=multiply(scalar,laplacianCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    if( numberOfDimensions==1 )
      result+=multiply(x(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),xCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    else if( numberOfDimensions==2 )
      result+=multiply(x(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),xCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_))
	+multiply(y(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),yCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    else
      result+=multiply(x(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),xCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_))
	+multiply(y(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),yCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_))
	+multiply(z(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),zCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    return result;
  }
  else
  {
    // conservative form
    MappedGrid & c = mappedGrid;
    numberOfDimensions=c.numberOfDimensions();

    // Determine ranges over which to compute the derivatives
    //   by default do as many points as possible, given the width of the stencil
    Index E=I4_, C=I5_;

    Range R1,R2,R3,R4,R5; 
    int w0 = orderOfAccuracy/2;
    int w1 = numberOfDimensions>1 ? w0 : 0;
    int w2 = numberOfDimensions>2 ? w0 : 0;
    R1= I1_.length()==0 ? Range(c.dimension(Start,0)+w0,c.dimension(End,0)-w0) : Range(I1_.getBase(),I1_.getBound());
    R2= I2_.length()==0 ? Range(c.dimension(Start,1)+w1,c.dimension(End,1)-w1) : Range(I2_.getBase(),I2_.getBound());
    R3= I3_.length()==0 ? Range(c.dimension(Start,2)+w2,c.dimension(End,2)-w2) : Range(I3_.getBase(),I3_.getBound());
    R4=   E.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  E.getBase(),  E.getBound());
    R5=   C.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  C.getBase(),  C.getBound());

    Range all;
    int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);

    realMappedGridFunction uX(c,stencilDimension,all,all,all);
    uX=0.;
    
//     int update=uX.updateToMatchGrid(c,stencilDimension,all,all,all);
//     if( update!=0 )  // if the grid sizes have changed we should initialize the grid function to zero
//       uX=0.;         // This gives default values to points not defined below
//     else if( R4.length()!=numberOfComponentsForCoefficients || R5.length()!=numberOfComponentsForCoefficients )
//     { // zero out unused components!   ***** could do better here *******
//       uX=0.;      
//     }

    divScalarGradFDerivCoefficients(uX,scalar,R1,R2,R3,R4,R5,*this);

    return uX;
  }
}

realMappedGridFunction MappedGridOperators::
divScalarGradCoefficients(const GridFunctionParameters & gfType,			
			  const realMappedGridFunction & scalar,
			  const Index & I1_ /* = nullIndex */,
			  const Index & I2_ /* = nullIndex */,
			  const Index & I3_ /* = nullIndex */,
			  const Index & I4_ /* = nullIndex */,
			  const Index & I5_ /* = nullIndex */,
			  const Index & I6_ /* = nullIndex */,
			  const Index & I7_ /* = nullIndex */,
			  const Index & I8_ /* = nullIndex */ )
{
  return divScalarGradCoefficients(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
}


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{derivativeScalarDerivativeCoefficients}}
realMappedGridFunction MappedGridOperators::
derivativeScalarDerivativeCoefficients(const realMappedGridFunction & scalar,
			    const int & direction1,
			    const int & direction2,
			  const Index & I1 /* = nullIndex */,
			  const Index & I2 /* = nullIndex */,
			  const Index & I3 /* = nullIndex */,
			  const Index & I4 /* = nullIndex */,
			  const Index & I5 /* = nullIndex */,
			  const Index & I6 /* = nullIndex */,
			  const Index & I7 /* = nullIndex */,
			  const Index & I8 /* = nullIndex */ )
//=======================================================================================
// /Description:
//   Form the coefficient matrix for the operator
// \[ 
//    { \partial \over \partial x_{\rm direction1} } ( \rm{scalar} { \partial \over \partial x_{\rm direction2} }u)
// \]
// /scalar (input) : coefficient that appears in the operator.
// /direction1,direction2 (input) : specify the derivatives to use.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{

  if( !conservative || orderOfAccuracy==4 )
  {
    realMappedGridFunction result;

    if( direction1==0 && direction2==0 )
    {
      result =multiply(scalar,xxCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(x(scalar,I1,I2,I3,I4,I5,I6,I7,I8),xCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==0 && direction2==1 )
    {
      result =multiply(scalar,xyCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(x(scalar,I1,I2,I3,I4,I5,I6,I7,I8),yCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==0 && direction2==2 )
    {
      result =multiply(scalar,xzCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(x(scalar,I1,I2,I3,I4,I5,I6,I7,I8),zCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==1 && direction2==0 )
    {
      result =multiply(scalar,xyCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(y(scalar,I1,I2,I3,I4,I5,I6,I7,I8),xCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==1 && direction2==1 )
    {
      result =multiply(scalar,yyCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(y(scalar,I1,I2,I3,I4,I5,I6,I7,I8),yCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==1 && direction2==2 )
    {
      result =multiply(scalar,yzCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(y(scalar,I1,I2,I3,I4,I5,I6,I7,I8),zCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==2 && direction2==0 )
    {
      result =multiply(scalar,xzCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(z(scalar,I1,I2,I3,I4,I5,I6,I7,I8),xCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==2 && direction2==1 )
    {
      result =multiply(scalar,yzCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(z(scalar,I1,I2,I3,I4,I5,I6,I7,I8),yCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    else if( direction1==2 && direction2==2 )
    {
      result =multiply(scalar,zzCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
      result+=multiply(z(scalar,I1,I2,I3,I4,I5,I6,I7,I8),zCoefficients(I1,I2,I3,I4,I5,I6,I7,I8));
    }
    
    return result;
  }
  else
  {
    // conservative form
    MappedGrid & c = mappedGrid;
    numberOfDimensions=c.numberOfDimensions();

    // Determine ranges over which to compute the derivatives
    //   by default do as many points as possible, given the width of the stencil
    Index E=I4, C=I5;

    Range R1,R2,R3,R4,R5; 
    int w0 = orderOfAccuracy/2;
    int w1 = numberOfDimensions>1 ? w0 : 0;
    int w2 = numberOfDimensions>2 ? w0 : 0;
    R1= I1.length()==0 ? Range(c.dimension(Start,0)+w0,c.dimension(End,0)-w0) : Range(I1.getBase(),I1.getBound());
    R2= I2.length()==0 ? Range(c.dimension(Start,1)+w1,c.dimension(End,1)-w1) : Range(I2.getBase(),I2.getBound());
    R3= I3.length()==0 ? Range(c.dimension(Start,2)+w2,c.dimension(End,2)-w2) : Range(I3.getBase(),I3.getBound());
    R4=   E.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  E.getBase(),  E.getBound());
    R5=   C.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  C.getBase(),  C.getBound());

    Range all;
    int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);

    realMappedGridFunction uX(c,stencilDimension,all,all,all);
    uX=0.;

//     int update=uX.updateToMatchGrid(c,stencilDimension,all,all,all);
//     if( update!=0 )  // if the grid sizes have changed we should initialize the grid function to zero
//       uX=0.;         // This gives default values to points not defined below
//     else if( R4.length()!=numberOfComponentsForCoefficients || R5.length()!=numberOfComponentsForCoefficients )
//     { // zero out unused components!   ***** could do better here *******
//       uX=0.;      
//     }

    if( true )
    {
      MappedGridOperators::derivativeTypes derivType;
      if( direction1==0 && direction2==0 )
	derivType= MappedGridOperators::xDerivativeScalarXDerivative;
      else if( direction1==0 && direction2==1 )
	derivType= MappedGridOperators::xDerivativeScalarYDerivative;
      else if( direction1==1 && direction2==1 )
	derivType= MappedGridOperators::yDerivativeScalarYDerivative;
      else if( direction1==1 && direction2==0 )
	derivType= MappedGridOperators::yDerivativeScalarXDerivative;
      else if( direction1==0 && direction2==2 )
	derivType= MappedGridOperators::xDerivativeScalarZDerivative;
      else if( direction1==1 && direction2==2 )
	derivType= MappedGridOperators::yDerivativeScalarZDerivative;
      else if( direction1==2 && direction2==2 )
	derivType= MappedGridOperators::zDerivativeScalarZDerivative;
      else if( direction1==2 && direction2==0 )
	derivType= MappedGridOperators::zDerivativeScalarXDerivative;
      else if( direction1==2 && direction2==1 )
	derivType= MappedGridOperators::zDerivativeScalarYDerivative;

      coefficients(derivType,uX,scalar,R1,R2,R3,R4,R5);
    }
    else
    {
      // these are not correct in 3D curvilinear
      derivativeScalarDerivativeFDerivCoefficients(uX,scalar,direction1,direction2,R1,R2,R3,R4,R5,*this);
    }
    
    return uX;
  }

}

realMappedGridFunction MappedGridOperators::
derivativeScalarDerivativeCoefficients(const GridFunctionParameters & gfType,			
			  const realMappedGridFunction & scalar,
			    const int & direction1,
			    const int & direction2,
			  const Index & I1_ /* = nullIndex */,
			  const Index & I2_ /* = nullIndex */,
			  const Index & I3_ /* = nullIndex */,
			  const Index & I4_ /* = nullIndex */,
			  const Index & I5_ /* = nullIndex */,
			  const Index & I6_ /* = nullIndex */,
			  const Index & I7_ /* = nullIndex */,
			  const Index & I8_ /* = nullIndex */ )
{
  return derivativeScalarDerivativeCoefficients(scalar,direction1,direction2,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
}



//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{scalarGradCoefficients}}
realMappedGridFunction MappedGridOperators::
scalarGradCoefficients(const realMappedGridFunction & scalar,
			  const Index & I1_ /* = nullIndex */,
			  const Index & I2_ /* = nullIndex */,
			  const Index & I3_ /* = nullIndex */,
			  const Index & I4_ /* = nullIndex */,
			  const Index & I5_ /* = nullIndex */,
			  const Index & I6_ /* = nullIndex */,
			  const Index & I7_ /* = nullIndex */,
			  const Index & I8_ /* = nullIndex */ )
//=======================================================================================
// /Description:
//   Form the coefficient matrix for the operator $\rm{scalar}\grad$.
// /scalar (input) : coefficient that appears in the operator.
// /Author: WDH
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  if( !conservative || orderOfAccuracy==4 )
  {
    realMappedGridFunction result;
    result=multiply(scalar,gradCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    return result;
  }
  else
  {
    // conservative form
    MappedGrid & c = mappedGrid;
    numberOfDimensions=c.numberOfDimensions();

    // Determine ranges over which to compute the derivatives
    //   by default do as many points as possible, given the width of the stencil
    Index E=I4_, C=I5_;

    Range R1,R2,R3,R4,R5; 
    int w0 = orderOfAccuracy/2;
    int w1 = numberOfDimensions>1 ? w0 : 0;
    int w2 = numberOfDimensions>2 ? w0 : 0;
    R1= I1_.length()==0 ? Range(c.dimension(Start,0)+w0,c.dimension(End,0)-w0) : Range(I1_.getBase(),I1_.getBound());
    R2= I2_.length()==0 ? Range(c.dimension(Start,1)+w1,c.dimension(End,1)-w1) : Range(I2_.getBase(),I2_.getBound());
    R3= I3_.length()==0 ? Range(c.dimension(Start,2)+w2,c.dimension(End,2)-w2) : Range(I3_.getBase(),I3_.getBound());
    R4=   E.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  E.getBase(),  E.getBound());
    R5=   C.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  C.getBase(),  C.getBound());

    Range all;
    int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients)*numberOfDimensions;  // *note*

    realMappedGridFunction uX(c,stencilDimension,all,all,all);
    uX=0.;

//     int update=uX.updateToMatchGrid(c,stencilDimension,all,all,all);
//     if( update!=0 )  // if the grid sizes have changed we should initialize the grid function to zero
//       uX=0.;         // This gives default values to points not defined below

    scalarGradFDerivCoefficients(uX,scalar,R1,R2,R3,R4,R5,*this);
    return uX;

  }

}

realMappedGridFunction MappedGridOperators::
scalarGradCoefficients(const GridFunctionParameters & gfType,			
			  const realMappedGridFunction & scalar,
			  const Index & I1_ /* = nullIndex */,
			  const Index & I2_ /* = nullIndex */,
			  const Index & I3_ /* = nullIndex */,
			  const Index & I4_ /* = nullIndex */,
			  const Index & I5_ /* = nullIndex */,
			  const Index & I6_ /* = nullIndex */,
			  const Index & I7_ /* = nullIndex */,
			  const Index & I8_ /* = nullIndex */ )
{
  return scalarGradCoefficients(scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
}



// scalar times identityCoefficients
realMappedGridFunction MappedGridOperators::
scalarCoefficients(				
		   const realMappedGridFunction & s,
		   const Index & I1_,
		   const Index & I2_,
		   const Index & I3_,
		   const Index & I4_,
		   const Index & I5_,
		   const Index & I6_,
		   const Index & I7_,
		   const Index & I8_
		   )
{
  cout << "MappedGridOperators::scalarCoefficients not implemented\n";
  if( &s )
    Overture::abort("MappedGridOperators::scalarCoefficients not implemented");

  return s;                                  
}

// scalar array times identityCoefficients -- use this to multiply an array of values
// times a coefficient matrix that represents a system of equations
realMappedGridFunction MappedGridOperators::
scalarCoefficients( const RealDistributedArray & s )
{
  cout << "MappedGridOperators::scalarCoefficients not implemented\n";
  if( &s )
    Overture::abort("MappedGridOperators::scalarCoefficients not implemented");
  return Overture::nullRealMappedGridFunction();                                  
}


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{divVectorScalarCoefficients}}
realMappedGridFunction MappedGridOperators::
divVectorScalarCoefficients(const realMappedGridFunction & s,
			  const Index & I1_ /* = nullIndex */,
			  const Index & I2_ /* = nullIndex */,
			  const Index & I3_ /* = nullIndex */,
			  const Index & I4_ /* = nullIndex */,
			  const Index & I5_ /* = nullIndex */,
			  const Index & I6_ /* = nullIndex */,
			  const Index & I7_ /* = nullIndex */,
			  const Index & I8_ /* = nullIndex */ )
//=======================================================================================
// /Description:
//   Form the coefficient matrix for the operator $\grad\cdot(\Sv)$.
// /s (input) : The coefficient appearing in the derivative expression, number of components
//   equal to the number of space dimensions.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{

  if( !conservative || orderOfAccuracy==4 )
  {
    realMappedGridFunction result;
    result=multiply(x(s,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),xCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    if( numberOfDimensions==2 )
      result+=multiply(y(s,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),yCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    else
      result+=multiply(z(s,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_),zCoefficients(I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    return result;
  }
  else
  {
    // conservative form
    MappedGrid & c = mappedGrid;
    numberOfDimensions=c.numberOfDimensions();

    // Determine ranges over which to compute the derivatives
    //   by default do as many points as possible, given the width of the stencil
    Index E=I4_, C=I5_;

    Range R1,R2,R3,R4,R5; 
    int w0 = orderOfAccuracy/2;
    int w1 = numberOfDimensions>1 ? w0 : 0;
    int w2 = numberOfDimensions>2 ? w0 : 0;
    R1= I1_.length()==0 ? Range(c.dimension(Start,0)+w0,c.dimension(End,0)-w0) : Range(I1_.getBase(),I1_.getBound());
    R2= I2_.length()==0 ? Range(c.dimension(Start,1)+w1,c.dimension(End,1)-w1) : Range(I2_.getBase(),I2_.getBound());
    R3= I3_.length()==0 ? Range(c.dimension(Start,2)+w2,c.dimension(End,2)-w2) : Range(I3_.getBase(),I3_.getBound());
    R4=   E.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  E.getBase(),  E.getBound());
    R5=   C.length()==0 ? Range(0,numberOfComponentsForCoefficients-1)         : Range(  C.getBase(),  C.getBound());

    Range all;
    int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);

    realMappedGridFunction uX(c,stencilDimension,all,all,all);
    uX=0.;
    
//     int update=uX.updateToMatchGrid(c,stencilDimension,all,all,all);
//     if( update!=0 )  // if the grid sizes have changed we should initialize the grid function to zero
//       uX=0.;         // This gives default values to points not defined below
//     else if( R4.length()!=numberOfComponentsForCoefficients || R5.length()!=numberOfComponentsForCoefficients )
//     { // zero out unused components!   ***** could do better here *******
//       uX=0.;      
//     }

    divVectorScalarFDerivCoefficients(uX,s,R1,R2,R3,R4,R5,*this);

    return uX;
  }
}

realMappedGridFunction MappedGridOperators::
divVectorScalarCoefficients(const GridFunctionParameters & gfType,			
			  const realMappedGridFunction & s,
			  const Index & I1_ /* = nullIndex */,
			  const Index & I2_ /* = nullIndex */,
			  const Index & I3_ /* = nullIndex */,
			  const Index & I4_ /* = nullIndex */,
			  const Index & I5_ /* = nullIndex */,
			  const Index & I6_ /* = nullIndex */,
			  const Index & I7_ /* = nullIndex */,
			  const Index & I8_ /* = nullIndex */ )
{
  return divVectorScalarCoefficients(s,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
}


#define  laplacianCoeff laplaciancoeff_
#define  laplacianCoeff4 laplaciancoeff4_
#define  identityCoeff identitycoeff_
#define coeffOperator EXTERN_C_NAME(coeffoperator)
extern "C"
{

  void laplacianCoeff( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                       const int &nd3a, const int &nd3b,
		       const int &n1a, const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b, 
                       const int &ndc, const int &nc,  const int &ns,
                       const int &ea,const int &eb, const int &ca,const int &cb,
                       const real & d22, const real & d12, const real & h22, const real & rsxy,
                       real & coeff, const int & gridType, const int & order );

  void laplacianCoeff4( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                       const int &nd3a, const int &nd3b,
		       const int &n1a, const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b, 
                       const int &ndc, const int &nc,  const int &ns,
                       const int &ea,const int &eb, const int &ca,const int &cb,
                       const real & d24, const real & d14, const real & h42, const real & rsxy,
                       real & coeff, const int & gridType, const int & order );

  void identityCoeff( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                       const int &nd3a, const int &nd3b,
		       const int &n1a, const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b, 
                       const int &ndc, const int &nc, const int &ns, 
                       const int &ea,const int &eb, const int &ca,const int &cb,
                       real & coeff );

  void coeffOperator( const int &nd, 
     const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,
     const int &ndc1a,const int &ndc1b,const int &ndc2a,const int &ndc2b,const int &ndc3a,const int &ndc3b,
     const int &nds1a,const int &nds1b,const int &nds2a,const int &nds2b,const int &nds3a,const int &nds3b,
     const int &n1a,const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b, 
     const int &ndc, const int &nc, const int &ns, const int &ea,const int &eb, const int &ca,const int &cb,
     const real &dx, const real &dr,
     const real &rsxy, const real &jacobian, real &coeff, const real &s, 
     const int &ndw, real &w, 
     const int &derivative, const int &derivType, const int &gridType, const int &order, const int &averagingType, 
		      const int &dir1, const int &dir2, int &ierr  );

}



//! Evaluate the coeffcients for a given derivative and add to "coeff"
/*!
    This routine is a memory and speed efficient way to evaluate coefficient matrices.
 /param derivativeType (input) : evaluate the coefficients for this derivative
 /param coeff (input) : a coefficient array
 /param I1,I2,I3 (input) : optionally specify which points should be assigned.
 */
int MappedGridOperators::
coefficients(const derivativeTypes & derivativeType_,
	     realMappedGridFunction & coeff, 
	     const Index & I1 /* = nullIndex */, 
	     const Index & I2 /* = nullIndex */, 
	     const Index & I3 /* = nullIndex */, 
	     const Index & E /* = nullIndex */,   
	     const Index & C /* = nullIndex */ )
{
  return coefficients(derivativeType_,coeff,coeff,I1,I2,I3,E,C);
}


//! This more general version takes a scalar too.
int MappedGridOperators::
coefficients(const derivativeTypes & derivativeType_,
	     realMappedGridFunction & coeff, 
	     const realMappedGridFunction & scalar, 
	     const Index & I1 /* = nullIndex */, 
	     const Index & I2 /* = nullIndex */, 
	     const Index & I3 /* = nullIndex */, 
	     const Index & E /* = nullIndex */,   
	     const Index & C /* = nullIndex */ )
{
  return assignCoefficients(derivativeType_,coeff,scalar,I1,I2,I3,E,C);
}

#ifdef USE_PPP
//! Fill the coefficients into an array.
int MappedGridOperators::
assignCoefficients(const derivativeTypes & derivativeType_,
	     realArray & coeff0, 
	     const Index & I1 /* = nullIndex */, 
	     const Index & I2 /* = nullIndex */, 
	     const Index & I3 /* = nullIndex */, 
	     const Index & E /* = nullIndex */,   
	     const Index & C /* = nullIndex */ )
{
  realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff0,coeffLocal);
  return assignCoefficientsInternal(derivativeType_,coeffLocal,coeffLocal,I1,I2,I3,E,C);
}

int MappedGridOperators::
assignCoefficients(const derivativeTypes & derivativeType_,
	     realArray & coeff0, 
	     const realArray & scalar0, 
	     const Index & I1 /* = nullIndex */, 
	     const Index & I2 /* = nullIndex */, 
	     const Index & I3 /* = nullIndex */, 
	     const Index & E /* = nullIndex */,   
		   const Index & C /* = nullIndex */ )
{
  realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff0,coeffLocal);
  realSerialArray scalarLocal; getLocalArrayWithGhostBoundaries(scalar0,scalarLocal);
  return assignCoefficientsInternal(derivativeType_,coeffLocal,scalarLocal,I1,I2,I3,E,C);

}

#endif 

//! Fill the coefficients into an array.
int MappedGridOperators::
assignCoefficients(const derivativeTypes & derivativeType_,
	     realSerialArray & coeff0, 
	     const Index & I1 /* = nullIndex */, 
	     const Index & I2 /* = nullIndex */, 
	     const Index & I3 /* = nullIndex */, 
	     const Index & E /* = nullIndex */,   
	     const Index & C /* = nullIndex */ )
{
  return assignCoefficientsInternal(derivativeType_,coeff0,coeff0,I1,I2,I3,E,C);
}

int MappedGridOperators::
assignCoefficients(const derivativeTypes & derivativeType_,
	     realSerialArray & coeff0, 
	     const realSerialArray & scalar0, 
	     const Index & I1 /* = nullIndex */, 
	     const Index & I2 /* = nullIndex */, 
	     const Index & I3 /* = nullIndex */, 
	     const Index & E /* = nullIndex */,   
		   const Index & C /* = nullIndex */ )
{
  return assignCoefficientsInternal(derivativeType_,coeff0,scalar0,I1,I2,I3,E,C);

}


//! Fill the coefficients into an array. This version takes a scalar that is required by
//! some operators.
int MappedGridOperators::
assignCoefficientsInternal(const derivativeTypes & derivativeType_,
	     realSerialArray & coeff, 
	     const realSerialArray & scalar, 
	     const Index & I1 /* = nullIndex */, 
	     const Index & I2 /* = nullIndex */, 
	     const Index & I3 /* = nullIndex */, 
	     const Index & E /* = nullIndex */,   
	     const Index & C /* = nullIndex */ )
{

  #ifdef USE_PPP
    intSerialArray mask; getLocalArrayWithGhostBoundaries(mappedGrid.mask(),mask);
  #else    
    const intSerialArray  & mask   = mappedGrid.mask();
  #endif

  // nd1a,nd1b, ... dimensions for rsxy and jac
  const int nd1a=mask.getBase(0), nd1b=mask.getBound(0);
  const int nd2a=mask.getBase(1), nd2b=mask.getBound(1);
  const int nd3a=mask.getBase(2), nd3b=mask.getBound(2);

  int w0 = orderOfAccuracy/2;
  int w1 = numberOfDimensions>1 ? w0 : 0;
  int w2 = numberOfDimensions>2 ? w0 : 0;

  int n1a = I1.length()==0 ? nd1a+w0 : I1.getBase();
  int n1b = I1.length()==0 ? nd1b-w0 : I1.getBound();
  int n2a = I2.length()==0 ? nd2a+w1 : I2.getBase();
  int n2b = I2.length()==0 ? nd2b-w1 : I2.getBound();
  int n3a = I3.length()==0 ? nd3a+w2 : I3.getBase();
  int n3b = I3.length()==0 ? nd3b-w2 : I3.getBound();

  #ifdef USE_PPP
    n1a=max(nd1a+w0,n1a); n1b=min(nd1b-w0,n1b);
    n2a=max(nd2a+w1,n2a); n2b=min(nd2b-w1,n2b);
    n3a=max(nd3a+w2,n3a); n3b=min(nd3b-w2,n3b);
  #endif


  const int ndc=coeff.getLength(0);
  const int ndc1a = coeff.getBase(1);
  const int ndc1b = coeff.getBound(1);
  const int ndc2a = coeff.getBase(2);
  const int ndc2b = coeff.getBound(2);
  const int ndc3a = coeff.getBase(3);
  const int ndc3b = coeff.getBound(3);

  const int nds1a = scalar.getBase(0);
  const int nds1b = scalar.getBound(0);
  const int nds2a = scalar.getBase(1);
  const int nds2b = scalar.getBound(1);
  const int nds3a = scalar.getBase(2);
  const int nds3b = scalar.getBound(2);


  const int nc=numberOfComponentsForCoefficients;
  const int ea = E.getLength()==0 ? 0 : E.getBase();
  const int eb = E.getLength()==0 ? numberOfComponentsForCoefficients-1 : E.getBound();
  const int ca = C.getLength()==0 ? 0 : C.getBase();
  const int cb = C.getLength()==0 ? numberOfComponentsForCoefficients-1 : C.getBound();

  const int ns = stencilSize;
  
  int gridType = rectangular ? 0 : 1;

  // printf(" **** rectangular = %i \n",rectangular);
  
  if(!rectangular) 
    mappedGrid.update(MappedGrid::THEinverseVertexDerivative);  // *wdh* 2013/08/28

  // if the grid is rectangular then rsxy is not used, just point to coeff
  const realArray & inverseVertexDerivative = rectangular ? Overture::nullRealDistributedArray() : 
                                              mappedGrid.inverseVertexDerivative();
  #ifdef USE_PPP
    realSerialArray rsxyLocal;
    if( !rectangular )
      getLocalArrayWithGhostBoundaries(inverseVertexDerivative,rsxyLocal);
  #else
    const realSerialArray & rsxyLocal = inverseVertexDerivative;
  #endif

  const real *rsxy = getDataPointer(rsxyLocal);
  
  const bool useOpt=true;
  if( useOpt )
  {
    real dr[3];
    for( int axis=0; axis<3; axis++ )
    {
      dr[axis]=mappedGrid.gridSpacing(axis); // .5/d12(axis);
    }

    const real *s = getDataPointer(scalar);
    real *jacobian = (real*)rsxy;
    
    int ndw=1;  // size of work space 

    const int derivType= usingConservativeApproximations() ? 1 : 0;
    const int averageType = getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? 0 : 1;

    int dir1=-1, dir2=-1;
    if( (derivativeType_>=xDerivativeScalarXDerivative && derivativeType_<=zDerivativeScalarZDerivative)  ||
        derivativeType_ == divergenceScalarGradient )
    {
      if( derivativeType_>=xDerivativeScalarXDerivative && derivativeType_<=zDerivativeScalarZDerivative )
      {
	dir1=(derivativeType_-xDerivativeScalarXDerivative)/3;
	dir2=(derivativeType_-xDerivativeScalarXDerivative) % 3;
      }

      if( !rectangular ) // *wdh* 021021
        mappedGrid.update(MappedGrid::THEcenterJacobian);
        
      const realArray & centerJacobian = rectangular ? Overture::nullRealDistributedArray() : 
                                         mappedGrid.centerJacobian();

      #ifdef USE_PPP
        realSerialArray centerJacobianLocal;
        if(!rectangular) getLocalArrayWithGhostBoundaries(centerJacobian,centerJacobianLocal);
      #else
        const realSerialArray & centerJacobianLocal = centerJacobian;
      #endif

      jacobian = getDataPointer(centerJacobianLocal);
  
      const int derivative = derivativeType_ == divergenceScalarGradient ? 1 : 2;

      ndw=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1);  // work space -- no need to use d here
      
      if( rectangular )
      {
	if( derivative==0 || derivative==1 )
	  ndw *= mappedGrid.numberOfDimensions();  // laplacian and divScalarGrad need more space.
        else 
          ndw *=1;   // only 1 work space array needed for derivativeScalarDerivative if rectangular
      }
      else
      {
        ndw *= SQR(mappedGrid.numberOfDimensions()); 
      }
    }

    real *pcoeff = coeff.getDataPointer(); // getDataPointer(coeff)

    int ierr=0;
    if( n1a<=n1b && n2a<=n2b && n3a<=n3b )
    {
      real *w = new real [ndw];
      coeffOperator( mappedGrid.numberOfDimensions(), nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
		   ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b, 
		   nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, 
		   n1a,n1b,n2a,n2b,n3a,n3b, 
		   ndc, nc,ns, ea,eb,ca,cb,
		   dx[0], dr[0],
		   *rsxy, *jacobian, *pcoeff, *s, 
		   ndw,w[0],  
		   derivativeType_, derivType, gridType, orderOfAccuracy, averagingType, 
 		   dir1, dir2, ierr  );
    
      delete [] w;
      if( ierr==2 )
      {
	printf("MappedGridOperators::assignCoefficients:ERROR return from coeffOperator: not enough work space!\n");
	Overture::abort("");
      }
    }
    
    return ierr;
  }
  else
  {
// *     // ******* OLD WAY ********
// * 
// * 
// *     const IntegerArray & d = mappedGrid.dimension();
// * 
// *     if( I1.length()==0 && I2.length()==0 && I3.length()==0 )
// *       coeff0=0.;   // ***** fix this *****
// * 
// *     RealArray d12,d22,d14,d24, h21(3),h22(3),h41(3),h42(3);  // fix this -- only pass dr,dx
// *     if( !rectangular )
// *     {
// *       d12=1./(2.*mappedGrid.gridSpacing());  
// *       d22=1./SQR(mappedGrid.gridSpacing());
// * 
// *       d14=1./(12.*mappedGrid.gridSpacing());
// *       d24=1./(12.*SQR(mappedGrid.gridSpacing()));
// *     }
// *     else
// *     {
// *       for( int axis=0; axis<3; axis++ )
// *       {
// * 	h21(axis)=1./(2.*dx[axis]); 
// * 	h22(axis)=1./SQR(dx[axis]);
// * 
// * 	h41(axis)=1./(12.*dx[axis]);
// * 	h42(axis)=1./(12.*SQR(dx[axis]));
// *       }
// *     }
// * 
// * 
// *     switch (derivativeType_) 
// *     {
// *     case laplacianOperator:
// *       if( orderOfAccuracy==2 )
// *       {
// * 	laplacianCoeff( mappedGrid.numberOfDimensions(), d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
// * 			n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc,ns, ea,eb,ca,cb, d22(0), d12(0), h22(0),
// * 			*rsxy, *getDataPointer(coeff), gridType, orderOfAccuracy );
// *       }
// *       else
// *       {
// * 	laplacianCoeff4( mappedGrid.numberOfDimensions(), d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
// * 			 n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc,ns, ea,eb,ca,cb, d24(0), d14(0), h42(0),
// * 			 *rsxy, *getDataPointer(coeff), gridType, orderOfAccuracy );
// *       }
// *     
// *       break;
// *     case identityOperator:
// *       identityCoeff( mappedGrid.numberOfDimensions(), d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
// * 		     n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc,ns, ea,eb,ca,cb, *getDataPointer(coeff) );
// *       break;
// *     default:
// *       printf("MappedGridOperators::coefficients:ERROR: This operator not implemented yet. "
// * 	     " derivativeType=%i\n",(int)derivativeType_);
// *       return 1;
// *     }
  }
  
  return 0;

}


