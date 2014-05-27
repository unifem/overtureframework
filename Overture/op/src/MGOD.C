#include "MappedGridOperators.h"
#include "UnstructuredOperators.h"
#include "ParallelUtility.h"

// extern realMappedGridFunction Overture::nullDoubleMappedGridFunction();
// extern realMappedGridFunction Overture::nullFloatMappedGridFunction();
// ifdef OV_USE_DOUBLE
// define NULLRealMappedGridFunction Overture::nullDoubleMappedGridFunction()
// else
// define NULLRealMappedGridFunction Overture::nullFloatMappedGridFunction()
// endif


//==============================================================================
//   MappedGridOperators: Derivatives
//=============================================================================

//-----------------------------------------------------------------------------
// Define second order difference approximations:
// The include file cgux2a.h defines second order difference approximations
//
// Notes:
//  For efficiency we define UR2, US2 and UT2 to be the arrays that we have precomputed
//  the parametric derivatives in. (Otherwise the derivatives would get
//  recomputed for all derivatives). UR2, US2 and UT2 are used in cgux2a.h
//  to define UX2, UY2, UXX2, ...
//-----------------------------------------------------------------------------
#define U(I1,I2,I3,N)   u(I1,I2,I3,N)
#define UR2(I1,I2,I3,N) ur(I1,I2,I3,N)
#define US2(I1,I2,I3,N) us(I1,I2,I3,N)
#define UT2(I1,I2,I3,N) ut(I1,I2,I3,N)
#include "cgux2a.h"    // define 2nd order difference approximations

//-----------------------------------------------------------------------------
// Define fourth order difference approximations:
// The include file cgux4a.h defines fourth order difference approximations
//
// Notes : see above
//----------------------------------------------------------------------------
#define UR4(I1,I2,I3,N) ur(I1,I2,I3,N)
#define US4(I1,I2,I3,N) us(I1,I2,I3,N)
#define UT4(I1,I2,I3,N) ut(I1,I2,I3,N)
#include "cgux4a.h"    // define 4th order difference approximations

//====================================================================================================
// This macro defines how to determine a derivative called "x"
// Notes:
//  o argument 1 "x" is the name of the function
//  o argument 2 "xDerivative" is the corresponding enumerator
//====================================================================================================
#define DERIVATIVE(x,xDerivative)                                                                           \
realMappedGridFunction MappedGridOperators::                                                                 \
x(const realMappedGridFunction & u, \
  const Index & I1_, const Index & I2_, const Index & I3_, const Index & I4_,               \
  const Index & I5_, const Index & I6_, const Index & I7_, const Index & I8_ )       \
{                                                                                                            \
  return xi(xDerivative,u,I1_,I2_,I3_,I4_,I5_,I6_);                                                                   \
}             \
realMappedGridFunction MappedGridOperators::                                                                 \
x(const realMappedGridFunction & u, \
  const GridFunctionParameters & gfType,   \
  const Index & I1_, const Index & I2_, const Index & I3_, const Index & I4_,               \
  const Index & I5_, const Index & I6_, const Index & I7_, const Index & I8_ )       \
{                                                                                                            \
  return xi(xDerivative,u,I1_,I2_,I3_,I4_,I5_,I6_);                                                                   \
}             

// Now define instances of this macro for each of the derivatives we know how to compute

// define member function x:
DERIVATIVE(x,xDerivative)
// define member function y:
DERIVATIVE(y,yDerivative)
// define member function z:
DERIVATIVE(z,zDerivative)
// define member function xx:
DERIVATIVE(xx,xxDerivative)
// define member function xy:
DERIVATIVE(xy,xyDerivative)
DERIVATIVE(xz,xzDerivative)
DERIVATIVE(yy,yyDerivative)
DERIVATIVE(yz,yzDerivative)
DERIVATIVE(zz,zzDerivative)

DERIVATIVE(r1,r1Derivative)
DERIVATIVE(r2,r2Derivative)
DERIVATIVE(r3,r3Derivative)

DERIVATIVE(r1r1,r1r1Derivative)
DERIVATIVE(r1r2,r1r2Derivative)
DERIVATIVE(r1r3,r1r3Derivative)
DERIVATIVE(r2r2,r2r2Derivative)
DERIVATIVE(r2r3,r2r3Derivative)
DERIVATIVE(r3r3,r3r3Derivative)

DERIVATIVE(div,divergence)
DERIVATIVE(grad,gradient)
DERIVATIVE(laplacian,laplacianOperator)
DERIVATIVE(identity,identityOperator)

DERIVATIVE(vorticity,vorticityOperator)

#undef DERIVATIVE

void 
divScalarGradFDerivative(const realMappedGridFunction & ugf,
			 const realMappedGridFunction & s,
			 RealDistributedArray & derivative,
			 const Index & I1,
			 const Index & I2,
			 const Index & I3,
			 const Index & N,
			 MappedGridOperators & mgop );

void 
scalarGradFDerivative(const realMappedGridFunction & ugf,
		      const realMappedGridFunction & s,
		      RealDistributedArray & derivative,
		      const Index & I1,
		      const Index & I2,
		      const Index & I3,
		      const Index & N,
		      MappedGridOperators & mgop );

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{divScalarGrad}}
realMappedGridFunction MappedGridOperators::                                                                
divScalarGrad( const realMappedGridFunction & u, 
	      const realMappedGridFunction & scalar, 
	      const Index & I1_, 
	      const Index & I2_, 
	      const Index & I3_,
	      const Index & I4_,
	      const Index & I5_,
	      const Index & I6_,
	      const Index & I7_,
	      const Index & I8_)
//=======================================================================================
// /Description:
//   Evaluate the derivative $\grad\cdot(\rm{scalar}\grad u)$.
// /u (input): 
// /scalar (input) : The coefficient appearing in the derivative expression.
// /Author: WDH
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{                                                                                                           
  realMappedGridFunction result;
  // result.updateToMatchGridFunction(u);
  // dimension the result grid function to match the number of components being computed
  Index N;
  const Index & C1=I4_, & C2=I5_;
  if( C2==nullIndex )
    N=C1;
  else
  {
    if( C1.length()==1 && C2.length()==1 )
    {
      N=C1.getBase()+u.getComponentDimension(0)*(C2.getBase()-u.getComponentBase(1));
    }
    else if( C1==nullIndex && C2.length()==1 )
    {
      N=Range(C1.getBase()+u.getComponentDimension(0)*(C2.getBase() -u.getComponentBase(1)),
              C1.getBase()+u.getComponentDimension(0)*(C2.getBound()-u.getComponentBase(1)));
    }
    else 
    {
      printf("MappedGridOperators::ERROR: you are trying to differentiate a matrix gridFunction\n"
             " with Components C1=(%i,%i) and C2=(%i,%i). I can only do the case when all components \n"
             " are contiguous. Try differentiating 1 component at a time or all components.\n",
              C1.getBase(),C1.getBound(),C2.getBase(),C2.getBound());
      Overture::abort("error");
    }
  }
  if( C1==nullIndex && C2==nullIndex )
    result.updateToMatchGridFunction( u );
  else
    result.updateToMatchGridFunction( u,nullRange,nullRange,nullRange,N );



  result=0.; 
  
  if( !usingConservativeApproximations() || orderOfAccuracy==4 )
  {
    if( usingConservativeApproximations() )
      printf("divScalarGrad:WARNING: non-conservative 4th order approximation being used\n");

    result=laplacian(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar;
    if( numberOfDimensions==1 )
      result+=x(scalar,I1_,I2_,I3_)*x(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
    else if( numberOfDimensions==2 )
      result+=(x(scalar,I1_,I2_,I3_)*x(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)+
	       y(scalar,I1_,I2_,I3_)*y(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
    else 
      result+=(x(scalar,I1_,I2_,I3_)*x(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)+
	       y(scalar,I1_,I2_,I3_)*y(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)+
	       z(scalar,I1_,I2_,I3_)*z(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
  }
  else
  {
    // conservative form
    int w[4] = {0,0,0,0};
    int axis;
    for( axis=0; axis<numberOfDimensions; axis++ )
      w[u.positionOfCoordinate(axis)]=max(orderOfAccuracy,0)/2;  
    Range R1,R2,R3,R4;
    R1= I1_.length()==0 ? Range(u.getBase(0)+w[0],u.getBound(0)-w[0]) : Range(I1_.getBase(),I1_.getBound());
    R2= I2_.length()==0 ? Range(u.getBase(1)+w[1],u.getBound(1)-w[1]) : Range(I2_.getBase(),I2_.getBound());
    R3= I3_.length()==0 ? Range(u.getBase(2)+w[2],u.getBound(2)-w[2]) : Range(I3_.getBase(),I3_.getBound());
    R4= I4_.length()==0 ? Range(u.getBase(3)+w[3],u.getBound(3)-w[3]) : Range(I4_.getBase(),I4_.getBound());
    divScalarGradFDerivative(u,scalar,result,R1,R2,R3,R4,*this);
  }
  return result;
}             
realMappedGridFunction MappedGridOperators::                                                                
divScalarGrad( const realMappedGridFunction & u, 
              const GridFunctionParameters & gfType,			
	      const realMappedGridFunction & scalar, 
	      const Index & I1_, 
	      const Index & I2_, 
	      const Index & I3_,
	      const Index & I4_,
	      const Index & I5_,
	      const Index & I6_,
	      const Index & I7_,
	      const Index & I8_)
{                                                                                                           
  return divScalarGrad(u,scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
}             

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{scalarGrad}}
realMappedGridFunction MappedGridOperators::                                                                
scalarGrad( const realMappedGridFunction & u, 
	      const realMappedGridFunction & scalar, 
	      const Index & I1_, 
	      const Index & I2_, 
	      const Index & I3_,
	      const Index & I4_,
	      const Index & I5_,
	      const Index & I6_,
	      const Index & I7_,
	      const Index & I8_)
//=======================================================================================
// /Description:
//   Evaluate the derivative $\rm{scalar}\grad u$.
// /u (input): 
// /scalar (input) : The coefficient appearing in the derivative expression.
// /Author: WDH
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{                                                                                                           
  realMappedGridFunction result;
  result.updateToMatchGrid(mappedGrid,nullRange,nullRange,nullRange,numberOfDimensions);
  result=0.;
  
  if( !usingConservativeApproximations() || orderOfAccuracy==4 )
  {
    result=grad(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
    int n = I4_.length()==0 ? u.getComponentBase(0) : I4_.getBase();
    for( int axis=0; axis<numberOfDimensions; axis++ )
      result(I1_,I2_,I3_,n+axis)*=scalar(I1_,I2_,I3_);
  }
  else
  {
    // conservative form
    int w[4] = {0,0,0,0};
    int axis;
    for( axis=0; axis<numberOfDimensions; axis++ )
      w[u.positionOfCoordinate(axis)]=max(orderOfAccuracy,0)/2;  
    Range R1,R2,R3,R4;
    R1= I1_.length()==0 ? Range(u.getBase(0)+w[0],u.getBound(0)-w[0]) : Range(I1_.getBase(),I1_.getBound());
    R2= I2_.length()==0 ? Range(u.getBase(1)+w[1],u.getBound(1)-w[1]) : Range(I2_.getBase(),I2_.getBound());
    R3= I3_.length()==0 ? Range(u.getBase(2)+w[2],u.getBound(2)-w[2]) : Range(I3_.getBase(),I3_.getBound());
    R4= I4_.length()==0 ? Range(u.getBase(3)+w[3],u.getBound(3)-w[3]) : Range(I4_.getBase(),I4_.getBound());
    scalarGradFDerivative(u,scalar,result,R1,R2,R3,R4,*this);
  }
  return result;
}             
realMappedGridFunction MappedGridOperators::                                                                
scalarGrad( const realMappedGridFunction & u, 
              const GridFunctionParameters & gfType,			
	      const realMappedGridFunction & scalar, 
	      const Index & I1_, 
	      const Index & I2_, 
	      const Index & I3_,
	      const Index & I4_,
	      const Index & I5_,
	      const Index & I6_,
	      const Index & I7_,
	      const Index & I8_)
{                                                                                                           
  return scalarGrad(u,scalar,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
}             

void 
derivativeScalarDerivativeFDerivative(const realMappedGridFunction & ugf,
				      const realMappedGridFunction & s,
				      RealDistributedArray & derivative,
				      const int & direction1,
				      const int & direction2,
				      const Index & I1,
				      const Index & I2,
				      const Index & I3,
				      const Index & N,
				      MappedGridOperators & mgop );


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{derivativeScalarDerivative}}
realMappedGridFunction MappedGridOperators::                                                                
derivativeScalarDerivative( const realMappedGridFunction & u, 
			    const realMappedGridFunction & scalar, 
			    const int & direction1,
			    const int & direction2,
			    const Index & I1_, 
			    const Index & I2_, 
			    const Index & I3_,
			    const Index & I4_,
			    const Index & I5_,
			    const Index & I6_,
			    const Index & I7_,
			    const Index & I8_)
//=======================================================================================
// /Description:
//   Evaluate the derivative 
// \[ 
//    { \partial \over \partial x_{\rm direction1} } ( \rm{scalar} { \partial \over \partial x_{\rm direction2} }u)
// \]
// /u (input): 
// /scalar (input) : The coefficient appearing in the derivative expression.
// /direction1,direction2 (input) : specify the derivatives to use.
// 
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{                                                                                                           
  realMappedGridFunction result;
  // result.updateToMatchGridFunction(u);
  // dimension the result grid function to match the number of components being computed
  Index N;
  const Index & C1=I4_, & C2=I5_;
  if( C2==nullIndex )
    N=C1;
  else
  {
    if( C1.length()==1 && C2.length()==1 )
    {
      N=C1.getBase()+u.getComponentDimension(0)*(C2.getBase()-u.getComponentBase(1));
    }
    else if( C1==nullIndex && C2.length()==1 )
    {
      N=Range(C1.getBase()+u.getComponentDimension(0)*(C2.getBase() -u.getComponentBase(1)),
              C1.getBase()+u.getComponentDimension(0)*(C2.getBound()-u.getComponentBase(1)));
    }
    else 
    {
      printf("MappedGridOperators::ERROR: you are trying to differentiate a matrix gridFunction\n"
             " with Components C1=(%i,%i) and C2=(%i,%i). I can only do the case when all components \n"
             " are contiguous. Try differentiating 1 component at a time or all components.\n",
              C1.getBase(),C1.getBound(),C2.getBase(),C2.getBound());
      Overture::abort("error");
    }
  }
  if( C1==nullIndex && C2==nullIndex )
    result.updateToMatchGridFunction( u );
  else
    result.updateToMatchGridFunction( u,nullRange,nullRange,nullRange,N );

  result=0.;
  
  if( false )
  {
    // use newer optimized function:
    derivativeTypes derivType;
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

    derivative(derivType,u,scalar,result,I1_,I2_,I3_,I4_);
  }
  else
  {
    // ****** old way ***********
    if( direction1<0 || direction1>numberOfDimensions ||
	direction2<0 || direction2>numberOfDimensions )
    {
      printf("derivativeScalarDerivative:ERROR: invalide value for direction1=%i or direction2=%i \n"
	     " should be between 0 and numberOfDimensions-1=%i \n",direction1,direction2,numberOfDimensions-1);
      Overture::abort("error");
     
    }
    if( !usingConservativeApproximations() || orderOfAccuracy==4 )
    {
      if( usingConservativeApproximations() )
	printf("derivativeScalarDerivative:WARNING: non-conservative 4th order approximation being used\n");
    
      switch (direction1)
      {
      case 0:
	if( direction2==0 )
	  result=(xx(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  x(scalar,I1_,I2_,I3_)*x(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	else if( direction2==1 )
	  result=(xy(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  x(scalar,I1_,I2_,I3_)*y(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	else
	  result=(xz(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  x(scalar,I1_,I2_,I3_)*z(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	break;
      case 1:
	if( direction2==0 )
	  result=(xy(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  y(scalar,I1_,I2_,I3_)*x(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	else if( direction2==1 )
	  result=(yy(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  y(scalar,I1_,I2_,I3_)*y(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	else
	  result=(yz(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  y(scalar,I1_,I2_,I3_)*z(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	break;
      case 2:
	if( direction2==0 )
	  result=(xz(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  z(scalar,I1_,I2_,I3_)*x(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	else if( direction2==1 )
	  result=(yz(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  z(scalar,I1_,I2_,I3_)*y(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	else
	  result=(zz(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_)*scalar+
		  z(scalar,I1_,I2_,I3_)*z(u,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_));
	break;
      }
    }
    else
    {
      // conservative form
      int w[4] = {0,0,0,0};
      int axis;
      for( axis=0; axis<numberOfDimensions; axis++ )
	w[u.positionOfCoordinate(axis)]=max(orderOfAccuracy,0)/2;  
      Range R1,R2,R3,R4;
      R1= I1_.length()==0 ? Range(u.getBase(0)+w[0],u.getBound(0)-w[0]) : Range(I1_.getBase(),I1_.getBound());
      R2= I2_.length()==0 ? Range(u.getBase(1)+w[1],u.getBound(1)-w[1]) : Range(I2_.getBase(),I2_.getBound());
      R3= I3_.length()==0 ? Range(u.getBase(2)+w[2],u.getBound(2)-w[2]) : Range(I3_.getBase(),I3_.getBound());
      R4= I4_.length()==0 ? Range(u.getBase(3)+w[3],u.getBound(3)-w[3]) : Range(I4_.getBase(),I4_.getBound());
      derivativeScalarDerivativeFDerivative(u,scalar,result,direction1,direction2,R1,R2,R3,R4,*this);
    }
  }
  
  return result;
}             

realMappedGridFunction MappedGridOperators::                                                                
derivativeScalarDerivative( const realMappedGridFunction & u, 
			    const GridFunctionParameters & gfType,			
			    const realMappedGridFunction & scalar, 
			    const int & direction1,
			    const int & direction2,
			    const Index & I1_, 
			    const Index & I2_, 
			    const Index & I3_,
			    const Index & I4_,
			    const Index & I5_,
			    const Index & I6_,
			    const Index & I7_,
			    const Index & I8_)
{                                                                                                           
  return derivativeScalarDerivative(u,scalar,direction1,direction2,I1_,I2_,I3_,I4_,I5_,I6_,I7_,I8_);
}             


void 
divVectorScalarFDerivative(const realMappedGridFunction & ugf,
				      const realMappedGridFunction & s,
				      RealDistributedArray & derivative,
				      const Index & I1,
				      const Index & I2,
				      const Index & I3,
				      const Index & N,
				      MappedGridOperators & mgop );


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{divVectorScalar}}
realMappedGridFunction MappedGridOperators::                                                                
divVectorScalar( const realMappedGridFunction & u, 
			    const realMappedGridFunction & s, 
			    const Index & I1, 
			    const Index & I2, 
			    const Index & I3,
			    const Index & I4,
			    const Index & I5,
			    const Index & I6,
			    const Index & I7,
			    const Index & I8)
//=======================================================================================
// /Description:
//   Evaluate the divergence of a known vector times the dependent variable $u$:
// \[ 
//    \grad\cdot( \Sv u)
// \]
// /u (input): 
// /s (input) : The coefficient appearing in the derivative expression, number of components
//   equal to the number of space dimensions.
// 
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{                                                                                                           
  realMappedGridFunction result;
  result.updateToMatchGridFunction(u);
  result=0.;
  
  if( !usingConservativeApproximations() || orderOfAccuracy==4 )
  {
    if( usingConservativeApproximations() )
      printf("divVectorScalar:WARNING: non-conservative 4th order approximation being used\n");
    
    realMappedGridFunction su;
    Range all;
    su.updateToMatchGridFunction(mappedGrid,all,all,all,numberOfDimensions);
    for( int axis=0; axis<numberOfDimensions; axis++ )
      su(I1,I2,I3,axis)=s(I1,I2,I3,axis)*u(I1,I2,I3,0);
    
    result=div(su);
  }
  else
  {
    // conservative form
    int w[4] = {0,0,0,0};
    int axis;
    for( axis=0; axis<numberOfDimensions; axis++ )
      w[u.positionOfCoordinate(axis)]=max(orderOfAccuracy,0)/2;  
    Range R1,R2,R3,R4;
    R1= I1.length()==0 ? Range(u.getBase(0)+w[0],u.getBound(0)-w[0]) : Range(I1.getBase(),I1.getBound());
    R2= I2.length()==0 ? Range(u.getBase(1)+w[1],u.getBound(1)-w[1]) : Range(I2.getBase(),I2.getBound());
    R3= I3.length()==0 ? Range(u.getBase(2)+w[2],u.getBound(2)-w[2]) : Range(I3.getBase(),I3.getBound());
    R4= I4.length()==0 ? Range(u.getBase(3)+w[3],u.getBound(3)-w[3]) : Range(I4.getBase(),I4.getBound());
    divVectorScalarFDerivative(u,s,result,R1,R2,R3,R4,*this);
  }
  return result;
}             

realMappedGridFunction MappedGridOperators::                                                                
divVectorScalar( const realMappedGridFunction & u, 
			    const GridFunctionParameters & gfType,			
			    const realMappedGridFunction & s, 
			    const Index & I1, 
			    const Index & I2, 
			    const Index & I3,
			    const Index & I4,
			    const Index & I5,
			    const Index & I6,
			    const Index & I7,
			    const Index & I8)
{                                                                                                           
  return divVectorScalar(u,s,I1,I2,I3,I4,I5,I6,I7,I8);
}             




// -----Here are functions used to evaluate a whole set of derivatives at a time (for efficiency)

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setNumberOfDerivativesToEvaluate}}
void MappedGridOperators::
setNumberOfDerivativesToEvaluate( const int & numberOfDerivatives )
//=======================================================================================
// /Description:
//   Specify how many derivatives are to be evaluated
// /numberOfDerivatives (input): Indicate how many derivatives that you want to
//   evaluate in the call to {\ff getDerivatives}.
// /Author: WDH
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  numberOfDerivativesToEvaluate=numberOfDerivatives;
  derivativeType.resize(numberOfDerivativesToEvaluate);
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setDerivativeType}}
void MappedGridOperators::
setDerivativeType(const int & index, const derivativeTypes & derivativeType0, RealDistributedArray & ux1x2 )
//=======================================================================================
// /Description:
//   Specify which derivative to evaluate and provide an array to save the results in.
// /index (input): Specify this derivative. $0 \le index < {\ff numberOfDerivatives}$   
//   where  {\ff numberOfDerivatives} was specified with {\ff setNumberOfDerivativesToEvaluate}.  
// /derivativeType0 (input): indicates which derivative to evaluate, from the enum
//      {\ff derivativeTypes}.
// /ux1x2 (input): Here is the array that the function {\ff getDerivatives} will save the
//   derivative in. This class keeps a reference to the array {\ff ux1x1}.  
//   This array will be automatically be made large enough to hold the result.
//   
// /Author: WDH
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  if( index < 0 || index >= numberOfDerivativesToEvaluate )
  {
    printf("MappedGridOperators::setDerivativeType:ERROR You are trying to set an invalid"
           " derivative = %i \n",index);
    printf(" numberOfDerivativesToEvaluate = %i\n",numberOfDerivativesToEvaluate);
    Overture::abort("error");
  }
  derivativeArray[index]=&ux1x2;
  derivativeType(index)=derivativeType0;
  
}



//---------------------------------------------------------------------------------------
//   Compute a derivative
//
// Notes:
//   o This routine is the general purpose routine for computing any of the possible derivatives.
//   o Values for I1,I2,I3 default to the interior points (indexRange)
//   o Values for N default to all components
//---------------------------------------------------------------------------------------
realMappedGridFunction MappedGridOperators::
xi(const derivativeTypes & derivativeType0,  
   const realMappedGridFunction & u, 
   const Index & I1_,
   const Index & I2_, 
   const Index & I3_, 
   const Index & C1,
   const Index & C2,
   const Index & C3 )
{
  if( numberOfDimensions==0 )
  {
    cout << "MappedGridOperators::ERROR: you must assign a MappedGrid before taking derivatives! \n";
    return Overture::nullRealMappedGridFunction();
  }

  Index N;
  if( C2==nullIndex )
    N=C1;
  else
  {
    #ifdef USE_PPP
      // Note: this case may acutally work if C2.getBase() == u.getComponentBase(1)
      printf("MappedGridOperators::xi:ERROR: unable to take a derivative of a matrix grid function in parallel\n"
             " The problem is that this requires a reshape at the end which doesn't seem to work\n");
      Overture::abort("error");
    #endif

    if( C1.length()==1 && C2.length()==1 )
    {
      N=C1.getBase()+u.getComponentDimension(0)*(C2.getBase()-u.getComponentBase(1));
    }
    else if( C1==nullIndex && C2.length()==1 )
    {
      N=Range(C1.getBase()+u.getComponentDimension(0)*(C2.getBase() -u.getComponentBase(1)),
              C1.getBase()+u.getComponentDimension(0)*(C2.getBound()-u.getComponentBase(1)));
    }
    else 
    {
      printf("MappedGridOperators::ERROR: you are trying to differentiate a matrix gridFunction\n"
             " with Components C1=(%i,%i) and C2=(%i,%i). I can only do the case when all components \n"
             " are contiguous. Try differentiating 1 component at a time or all components.\n",
              C1.getBase(),C1.getBound(),C2.getBase(),C2.getBound());
      Overture::abort("error");
    }
  }
  // allocate space for ux, initialize to zero if the size changes 
  int update;
  realMappedGridFunction ux;
  if( derivativeType0 == gradient )
  {  // more components are needed for a gradient
    Range all;
    if( u.getNumberOfComponents()==0 || u.getComponentDimension(0)==1 )
      update=ux.updateToMatchGridFunction( u,all,all,all,numberOfDimensions );
    else if( u.getNumberOfComponents()==1 )
      update=ux.updateToMatchGridFunction( u,all,all,all,Range(u.getComponentBase(0),u.getComponentBound(0)),
             numberOfDimensions );
    else
    {
      printf("MappedGridOperators::ERROR: in grad. I can only take the gradient of a scalar or vector grid function\n"
             "                            u.getNumberOfComponents()=%i \n",u.getNumberOfComponents());
      Overture::abort("error");
    }
    if( update!=0 )
      ux=0.;  // initialize
  }
  else if( derivativeType0 == divergence )
  { // only one component needed for div
    Range all;
    update=ux.updateToMatchGridFunction( u,all,all,all,1 );  
    if( update!=0 )
      ux=0.;  // initialize
  }
  else if( derivativeType0 == vorticityOperator )
  { 
    // in 2d there is only one component of the vorticity
    Range all;
    update=ux.updateToMatchGridFunction( u,all,all,all,numberOfDimensions==2 ? 1 : numberOfDimensions);  
    // ux.display("xi: Here is ux for vorticity");
    
    if( update!=0 )
      ux=0.;  // initialize
  }
  else
  {  // make ux look like u (same dimensions, same properties)
    // update=ux.updateToMatchGridFunction( u );   
    // real time=getCPU();
    if( C1==nullIndex && C2==nullIndex )
      update=ux.updateToMatchGridFunction( u );
    else
      update=ux.updateToMatchGridFunction( u,nullRange,nullRange,nullRange,N );

    if( update!=0 )
    {
       ux=0.;  // initialize
    }
    // time=getCPU()-time;
    //  printf("Time to update and initialize=%8.2e\n",time);    
  }
  

  // the derivative function takes a list of derivatives to evaluate, in this case
  // we only compute one derivative:
//  numberOfDerivatives=1;
//  derivativesToEvaluate(0)=derivativeType0;
  IntegerArray derivType(1);
  derivType(0)=derivativeType0;
  RealDistributedArray *derivative[1];
  derivative[0] = &ux;
  const bool checkArrayDimensions=FALSE;  // no need to check array sizes
  if( C3!=nullIndex )
  {
    printf("MappedGridOperators::ERROR: you are trying to differentiate a 3-tensor gridFunction\n"
	   " with Components C1=(%i,%i), C2=(%i,%i), C3=(%i,%i). I cannot do this case\n", 
	   C1.getBase(),C1.getBound(),C2.getBase(),C2.getBound(),C3.getBase(),C3.getBound());
    Overture::abort("error");
  }
  // ***** Note that the derivative routines think that the function they are differentiating
  //       has only one component (since the GridFunctions compress more than 4 indicies into 4 indicies).
  //       Thus for matrix grid functions we must create a component Index N to point to the correct components
  //       to differentiate. This means we cannot do all cases.
  computeDerivatives( 1,derivType,derivative,u,I1_,I2_,I3_,N,checkArrayDimensions );

  if( C2!=nullIndex )
    update=ux.updateToMatchGridFunction( u,nullRange,nullRange,nullRange,C1,C2 );
  // ux.display("xi: Here is ux for vorticity after compute");

  return ux;
}

//---------------------------------------------------------------------------------------
// getDerivatives:
//
//  o This is an efficient way to compute many derivatives because computations can be shared
//  o This routine first computes u.r, u.s, [u.t] for efficiency
//---------------------------------------------------------------------------------------
//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{getDerivatives}}
void MappedGridOperators::
getDerivatives(const realMappedGridFunction & u, 
	       const Index & I1_,      /* =nullIndex */
	       const Index & I2_,      /* =nullIndex */ 
	       const Index & I3_,      /* =nullIndex */ 
	       const Index & N,       /* =nullIndex */
               const Index & Evaluate /* =nullIndex */ )   
//=======================================================================================
// /Description:
//   This is an efficient way to compute derivatives. Compute the derivatives of
//  u that were specified with  {\ff setNumberOfDerivativesToEvaluate} and 
//  {\ff setDerivativeType}.
// /u (input): Compute the derivatives of this grid function.  
// /I1,I2,I3 (input): evaluate the derivatives at these coordinate Index values (by default evaluate
//     at as many points as is possible; for second-order discretization all points but the last
//   ghost line are evaluated, for fourth order all points but the 2 last ghostlines are evaluated).
// /N (input): Evaluate the derivatives of these components of u (by default all components are
//     evaluated). 
// /Evaluate (input): evaluate this subset of the derivatives. The derivatives to be evaluated
//    are numbered from 0,1,2,... For example, suppose you used setDerivativeType to specify:
//     \begin{verbatim}
//         setDerivativeType(0,MappedGridOperators::xDerivative,ux);
//         setDerivativeType(1,MappedGridOperators::yDerivative,uy);
//         setDerivativeType(2,MappedGridOperators::xxDerivative,uxx);
//         setDerivativeType(3,MappedGridOperators::yyDerivative,uyy);
//     \end{verbatim}
//   If you only want to evaluate the second derivatives you can choose {\tt Evaluate=Index(2,2)}
//   to only evaluate derivatives 2 and 3.
// /Notes:
//  This is an efficient way to compute many derivatives because computations can be shared
//  This routine first computes u.r, u.s, [u.t] for efficiency
//
//  **WARNING** on each call to getDerivatives, the arrays used to hold the results will be
//  redimensioned if the new results do not fit into the existing array (not just the size
//  but the (base,bound) values for each dimension).
//  Thus if you call {\tt getDerivatives} in consecutive statements with different 
//  values for N and Evaluate, then the results from the first call may be destroyed
//  if the arrays were not big enough. You can either explicitly dimension the arrays
//  to be large enough or else initially call getDerivatives with the default values for
//  N and Evaluate so the arrays are dimensioned to be full size.  
//
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  if( numberOfDimensions==0 )
  {
    cout << "MappedGridOperators::ERROR: you must assign a MappedGrid before taking derivatives! \n";
  }
  if( Evaluate.length()==0 )
    computeDerivatives( numberOfDerivativesToEvaluate,derivativeType,derivativeArray,u,I1_,I2_,I3_,N );
  else
  {
    // make a separate list of derivatives to evaluate
    int numberToEvaluate=Evaluate.length();
    IntegerArray derivType(numberToEvaluate);
    RealDistributedArray **derivative = new RealDistributedArray* [numberToEvaluate];
    int i,d;
    for( d=Evaluate.getBase(), i=0; d<=Evaluate.getBound(); d++,i++ )
    {
      derivType(i)=derivativeType(d);
      derivative[i]=derivativeArray[d];
    }
    computeDerivatives( numberToEvaluate,derivType,derivative,u,I1_,I2_,I3_,N );
    delete [] derivative;
  }
}


//---------------------------------------------------------------------------------------
//   Compute a list of Derivatives
//
// Input -
//   numberOfDerivatives : number of different derivatives to evaluate
//   derivativesToEvaluate : list of types of derivatives to evaluate
//   derivative : arrays of pointers to RealArray's in which to save the results
//
// Notes:
//   o This routine is the general purpose routine for computing any of the possible derivatives.
//   o Values for I1,I2,I3 default to the interior points (indexRange)
//   o Values for N default to all components
//---------------------------------------------------------------------------------------
void MappedGridOperators::
computeDerivatives( const int & numberOfDerivatives,
		   const IntegerArray & derivativesToEvaluate,  
		   RealDistributedArray *deriv[],
		   const realMappedGridFunction & u, 
		   const Index & I1_,
		   const Index & I2_, 
		   const Index & I3_, 
		   const Index & N,
                   const bool & checkArrayDimensions /* = TRUE */ )
{

  if( numberOfDimensions==0 )
  {
    cout << "MappedGridOperators::ERROR: you must assign a MappedGrid before taking derivatives! \n";
    return;
  }

  MappedGrid & c = mappedGrid;
  if( u.positionOfComponent(0) < u.positionOfCoordinate(c.numberOfDimensions()-1) )
  {
    cout << "MappedGridOperators::ERROR: sorry, can only differentiate grid functions"
            " with components that follow all coordinates";
    return;
  }
  if( !(c.isAllVertexCentered() || c.isAllCellCentered()) )
  {
    cout << "MappedGridOperators::updateToMatchGrid:ERROR: I only know how to take derivatives of a \n"
            " gridFunction that isAllVertexCentered or isAllCellCentered \n";
    Overture::abort("MappedGridOperators::updateToMatchGrid:ERROR");
  }

  // Determine ranges over which to compute the derivatives
  //   by default do as many points as possible, given the width of the stencil
  int w[4] = {0,0,0,0};
  int axis;
  if ( c.getGridType()!=GenericGrid::unstructuredGrid )
    for( axis=0; axis<numberOfDimensions; axis++ )
      w[u.positionOfCoordinate(axis)]=max(orderOfAccuracy,0)/2;  

  Range R1,R2,R3,R4;
  R1= I1_.length()==0 ? Range(u.getBase(0)+w[0],u.getBound(0)-w[0]) : Range(I1_.getBase(),I1_.getBound());
  R2= I2_.length()==0 ? Range(u.getBase(1)+w[1],u.getBound(1)-w[1]) : Range(I2_.getBase(),I2_.getBound());
  R3= I3_.length()==0 ? Range(u.getBase(2)+w[2],u.getBound(2)-w[2]) : Range(I3_.getBase(),I3_.getBound());
  R4=   N.length()==0 ? Range(u.getBase(3)+w[3],u.getBound(3)-w[3]) : Range(  N.getBase(),  N.getBound());

  int i;
  // First make sure that the user supplied arrays are the large enough
  //      derivative is a list of pointers to the user supplied arrays
  if( checkArrayDimensions )
  {
    for( i=0; i<numberOfDerivatives; i++ )
    {
      if( deriv[i]==NULL )
      {
	printf("MappedGridOperators::getDerivatives:No array have been specified for"
	       " derivative number %i\n",i);
	printf("You should call setDerivative to specify this \n");
	exit(1);
      }
      if( derivativesToEvaluate(i)!=divergence && 
	  !(numberOfDimensions==2 && derivativesToEvaluate(i)==vorticityOperator) )
      {
	if(R1.getBase()<(*deriv[i]).getBase(0) || R1.getBound()>(*deriv[i]).getBound(0) ||
	   R2.getBase()<(*deriv[i]).getBase(1) || R2.getBound()>(*deriv[i]).getBound(1) ||
	   R3.getBase()<(*deriv[i]).getBase(2) || R3.getBound()>(*deriv[i]).getBound(2) ||
	   R4.getBase()<(*deriv[i]).getBase(3) || R4.getBound()>(*deriv[i]).getBound(3) )
	{
          if( R1.getLength() > (*deriv[i]).getLength(0) ||
              R2.getLength() > (*deriv[i]).getLength(1) ||
              R3.getLength() > (*deriv[i]).getLength(2) ||
              R4.getLength() > (*deriv[i]).getLength(3) )
	  {
	    // we need too increase the size
     	    (*deriv[i]).partition(u.getPartition()); (*deriv[i]).redim(R1,R2,R3,R4);  
	  }
	  else
	  {
	    // we can just change the base and bound since each dimension is long enough.
     	    (*deriv[i]).setBase(R1.getBase(),0);
     	    (*deriv[i]).setBase(R2.getBase(),1);
     	    (*deriv[i]).setBase(R3.getBase(),2);
     	    (*deriv[i]).setBase(R4.getBase(),3);
	  }
	}
      }
      else
      {
	if(R1.getBase()<(*deriv[i]).getBase(0) || R1.getBound()>(*deriv[i]).getBound(0) ||
	   R2.getBase()<(*deriv[i]).getBase(1) || R2.getBound()>(*deriv[i]).getBound(1) ||
	   R3.getBase()<(*deriv[i]).getBase(2) || R3.getBound()>(*deriv[i]).getBound(2) )
	{
	  // printf(">>>>>>>>>>>>>>>>>>>>>>>>> dimension for divergence <<<<<<<<<<<<<<<<<<<< \n");
          if( R1.getLength() > (*deriv[i]).getLength(0) ||
              R2.getLength() > (*deriv[i]).getLength(1) ||
              R3.getLength() > (*deriv[i]).getLength(2) )
	  {
	    // we need too increase the size
     	    (*deriv[i]).partition(u.getPartition()); (*deriv[i]).redim(R1,R2,R3);  
	  }
	  else
	  {
	    // we can just change the base and bound since each dimension is long enough.
     	    (*deriv[i]).setBase(R1.getBase(),0);
     	    (*deriv[i]).setBase(R2.getBase(),1);
     	    (*deriv[i]).setBase(R3.getBase(),2);
	  }
	}
      }
    }
  }
  bool useOptimized=true;
  
  int *wasNotComputed = new int[numberOfDerivatives];
  bool allComputed=true;
  for( i=0; i<numberOfDerivatives; i++ )
  {
    wasNotComputed[i]=1;
// *wdh* 060214 #ifndef USE_PPP
    if( useOptimized ) // *** for now only call optimized version in serial
    {
      wasNotComputed[i]=derivative(derivativeTypes(derivativesToEvaluate(i)),u,*deriv[i],I1_,I2_,I3_,N);
    }
// #endif
    allComputed = allComputed && !wasNotComputed[i];
  }
  if( allComputed )
  {
    delete [] wasNotComputed;
    return;
  }
  

  if( rectangular )
  {
    // ----------------------------------------------------
    // -------------Evaluate the derivatives---------------
    // ----------------------------------------------------
    if( orderOfAccuracy==spectral )
    {
      for( axis=0; axis<mappedGrid.numberOfDimensions(); axis++ )
      {
        if( mappedGrid.isPeriodic(axis)!=Mapping::functionPeriodic )
	{
	  cout << "MappedGridOperators::ERROR: orderOfAccuracy==spectral but grid is not periodic \n";
	  Overture::abort("error");
	}
      }
      spectralDerivatives(numberOfDerivatives,derivativesToEvaluate,deriv,u,R1,R2,R3,R4);
    }
    else
    {
      for( i=0; i<numberOfDerivatives; i++ )
      {
        if( wasNotComputed[i] )
	{
          // printf("Call old deriv for derivativesToEvaluate(i)=%i\n",derivativesToEvaluate(i));
	  
	  assert(derivativeFunction[derivativesToEvaluate(i)]!=NULL);
	  (*derivativeFunction[derivativesToEvaluate(i)])(u,(*deriv[i]),R1,R2,R3,R4,*this );
	}
	
      }
    }
  }    
  else
  {
    // ------ Grid is not rectangular so use the Mapping method-----------

    // Determine the size needed for temporary arrays:
    Range R1g=Range(R1.getBase()-w[0],R1.getBound()+w[0]);
    Range R2g=Range(R2.getBase()-w[1],R2.getBound()+w[1]);
    Range R3g=Range(R3.getBase()-w[2],R3.getBound()+w[2]);
    Range R4g=Range(R4.getBase()-w[3],R4.getBound()+w[3]);

    // make sure the temporary arrays are large enough
    urp=usp=utp=NULL;
    realDistributedArray ur; ur.partition(mappedGrid.getPartition()); ur.redim(R1g,R2g,R3g,R4g);
    urp=&ur;
    RealDistributedArray us,ut;
    if(  numberOfDimensions>1 )
    {
      us.partition(u.getPartition()); 
      // *wdh* 040309 : to get around P++ bug with ghost: us.redim(R1g,R2,R3g,R4g); 
      us.redim(R1g,R2g,R3g,R4g); 
      usp=&us;
    }
    if( numberOfDimensions>2 )
    {
      ut.partition(mappedGrid.getPartition()); ut.redim(R1g,R2g,R3,R4g); 
      utp=&ut;
    }
    //  ...compute u.r,u.s,u.t and save in arrays, these are used in the
    //  computations of u.x or u.y ...

    if( orderOfAccuracy==2 )
    {
      RealArray d12; d12=1./(2.*mappedGrid.gridSpacing());

      ur(R1 ,R2g,R3g,R4g)=UR2A(R1 ,R2g,R3g,R4g);
      if( numberOfDimensions>1 )
        us(R1g,R2 ,R3g,R4g)=US2A(R1g,R2 ,R3g,R4g);
      if( numberOfDimensions>2 )
	ut(R1g,R2g,R3,R4g)=UT2A(R1g,R2g,R3,R4g);
    }
    else if( orderOfAccuracy==4 )
    {
      RealArray d14; d14=1./(12.*mappedGrid.gridSpacing());

      ur(R1 ,R2g,R3g,R4g)=UR4A(R1 ,R2g,R3g,R4g);
      if( numberOfDimensions>1 )
        us(R1g,R2 ,R3g,R4g)=US4A(R1g,R2 ,R3g,R4g);
      if( numberOfDimensions>2 )
	ut(R1g,R2g,R3,R4g)=UT4A(R1g,R2g,R3,R4g);
    }
    else 
    {
      cout << "MappedGridOperators:ERROR: invalid order of accuracy =";
      if( orderOfAccuracy==spectral )
        cout << "spectral\n";
      else
        cout << orderOfAccuracy << endl;
      Overture::abort("error");
	
    }
    // ----------------------------------------------------
    // -------------Evaluate the derivatives---------------
    // ----------------------------------------------------
    for( i=0; i<numberOfDerivatives; i++ )
    {
      if( wasNotComputed[i] )
      {
	assert(derivativeFunction[derivativesToEvaluate(i)]!=NULL);
	(*derivativeFunction[derivativesToEvaluate(i)])(u,(*deriv[i]),R1,R2,R3,R4,*this );
      }
    }

  }  // endif non rectangular mapping
  
  delete [] wasNotComputed;
  
}

#define xFDeriv  EXTERN_C_NAME(xfderiv)
#define yFDeriv  EXTERN_C_NAME(yfderiv)
#define zFDeriv  EXTERN_C_NAME(zfderiv)
#define xxFDeriv  EXTERN_C_NAME(xxfderiv)
#define xyFDeriv  EXTERN_C_NAME(xyfderiv)
#define xzFDeriv  EXTERN_C_NAME(xzfderiv)
#define yyFDeriv  EXTERN_C_NAME(yyfderiv)
#define yzFDeriv  EXTERN_C_NAME(yzfderiv)
#define zzFDeriv  EXTERN_C_NAME(zzfderiv)
#define laplacianFDeriv  EXTERN_C_NAME(laplacianfderiv)
#define divScalarGradFDeriv  EXTERN_C_NAME(divscalargradfderiv)

// Here is the generic function call to the optimized fortran operators.
#define DERIV_OPT(type) type( mappedGrid.numberOfDimensions(), \
			      mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1),\
                              mask.getBase(2),mask.getBound(2),\
                              u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1), \
			      u.getBase(2),u.getBound(2),u.getBase(3),u.getBound(3), \
			      ux.getBase(0),ux.getBound(0),ux.getBase(1),ux.getBound(1), \
			      ux.getBase(2),ux.getBound(2),ndd4a,ndd4b,\
	n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, h21(0), d22(0), d12(0), h22(0), d14(0), d24(0), h41(0), h42(0), \
		*rsxy, *getDataPointer(u), *getDataPointer(ux), gridType, orderOfAccuracy )
// Here is the prototype
#define DERIV_PROTO(type) \
  void type( const int &nd,  \
   const int & nd1a, const int & nd1b, const int & nd2a, const int & nd2b,\
   const int & nd3a, const int & nd3b, \
   const int & ndu1a, const int & ndu1b, const int & ndu2a, const int & ndu2b,\
   const int & ndu3a, const int & ndu3b, const int & ndu4a, const int & ndu4b,  \
   const int & ndd1a, const int & ndd1b, const int & ndd2a, const int & ndd2b,\
   const int & ndd3a, const int & ndd3b, const int & ndd4a, const int & ndd4b,  \
		const int &n1a, const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b,   \
		const int &ca,const int &cb, \
		const real & h21, const real & d22, const real & d12, const real & h22,   \
		const real & d14, const real & d24, const real & h41, const real & h42,   \
                const real & rsxy,  \
                const real & u,  \
                const real & ux,  \
		const int & gridType, const int & order )

extern "C"
{

  DERIV_PROTO(xFDeriv);
  DERIV_PROTO(yFDeriv);
  DERIV_PROTO(zFDeriv);
  DERIV_PROTO(xxFDeriv);
  DERIV_PROTO(xyFDeriv);
  DERIV_PROTO(xzFDeriv);
  DERIV_PROTO(yyFDeriv);
  DERIV_PROTO(yzFDeriv);
  DERIV_PROTO(zzFDeriv);
  DERIV_PROTO(laplacianFDeriv);

  void divScalarGradFDeriv( const int &nd, 
        const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,         
        const int &ndu1a,const int &ndu1b,const int &ndu2a,const int &ndu2b,const int &ndu3a,const int &ndu3b,
        const int &ndu4a,const int &ndu4b, 
        const int &ndd1a,const int &ndd1b,const int &ndd2a,const int &ndd2b,const int &ndd3a,const int &ndd3b,
        const int &ndd4a,const int &ndd4b, 
        const int &n1a,const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b, 
        const int &ca,const int &cb, 
        const real&dx, const real&dr,
        const real&rsxy, const real&jacobian, const real&u,const real&s, real&deriv, 
        const int &ndw, const real&w,  
	const int & derivative, const int &derivType, const int &gridType, const int &order, 
        const int &averagingType,const int & dir1,const int & dir2 );

}


#define xFiniteDiffDeriv           EXTERN_C_NAME(xfinitediffderiv)        
#define yFiniteDiffDeriv	   EXTERN_C_NAME(yfinitediffderiv)        
#define zFiniteDiffDeriv	   EXTERN_C_NAME(zfinitediffderiv)        
#define xxFiniteDiffDeriv	   EXTERN_C_NAME(xxfinitediffderiv)      
#define xyFiniteDiffDeriv	   EXTERN_C_NAME(xyfinitediffderiv)       
#define xzFiniteDiffDeriv	   EXTERN_C_NAME(xzfinitediffderiv)       
#define yyFiniteDiffDeriv	   EXTERN_C_NAME(yyfinitediffderiv)       
#define yzFiniteDiffDeriv	   EXTERN_C_NAME(yzfinitediffderiv)       
#define zzFiniteDiffDeriv	   EXTERN_C_NAME(zzfinitediffderiv)       
#define laplacianFiniteDiffDeriv   EXTERN_C_NAME(laplacianfinitediffderiv)
#define divFiniteDiffDeriv	   EXTERN_C_NAME(divfinitediffderiv)       
#define gradFiniteDiffDeriv	   EXTERN_C_NAME(gradfinitediffderiv)       
#define vorticityFiniteDiffDeriv   EXTERN_C_NAME(vorticityfinitediffderiv)       


// Here is the generic function call to the optimized fortran operators.
#define DERIVATIVE_OPT(type) type( mappedGrid.numberOfDimensions(), \
			      mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1),\
                              mask.getBase(2),mask.getBound(2),\
                              u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1), \
			      u.getBase(2),u.getBound(2),u.getBase(3),u.getBound(3), \
			      ux.getBase(0),ux.getBound(0),ux.getBase(1),ux.getBound(1), \
			      ux.getBase(2),ux.getBound(2),ndd4a,ndd4b,\
                              ipar[0],rpar[0],\
		              *getDataPointer(u), *getDataPointer(ux), *rsxy, *pmask )

// 6th and 8th order approximations
#define DERIV_PROTOTYPE(type)\
  void \
  type( const int&nd, const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,\
        const int&nd3a,const int&nd3b, const int&ndu1a,\
        const int&ndu1b,const int&ndu2a,const int&ndu2b,const int&ndu3a,const int&ndu3b,\
        const int&ndu4a,const int&ndu4b, const int&ndd1a,const int&ndd1b,const int&ndd2a,\
        const int&ndd2b,const int&ndd3a,const int&ndd3b,const int&ndd4a,const int&ndd4b, \
        const int&ipar, const real&rpar, const real&u,const real&deriv, const real&rsxy, const int&mask )



extern "C"
{
 DERIV_PROTOTYPE(xFiniteDiffDeriv);
 DERIV_PROTOTYPE(yFiniteDiffDeriv);
 DERIV_PROTOTYPE(zFiniteDiffDeriv);
 DERIV_PROTOTYPE(xxFiniteDiffDeriv);
 DERIV_PROTOTYPE(xyFiniteDiffDeriv);
 DERIV_PROTOTYPE(xzFiniteDiffDeriv);
 DERIV_PROTOTYPE(yyFiniteDiffDeriv);
 DERIV_PROTOTYPE(yzFiniteDiffDeriv);
 DERIV_PROTOTYPE(zzFiniteDiffDeriv);
 DERIV_PROTOTYPE(laplacianFiniteDiffDeriv);
 DERIV_PROTOTYPE(divFiniteDiffDeriv);
 DERIV_PROTOTYPE(gradFiniteDiffDeriv);
 DERIV_PROTOTYPE(vorticityFiniteDiffDeriv);

}




//! Evaluate a derivative 
/*!
    This routine is a memory and speed efficient way to evaluate derivatives
 /param derivativeType (input) : evaluate the coefficients for this derivative
 /param u (input) : differentiate this function
 /param ux (output) : result
 /param I1,I2,I3,C (input) : optionally specify which points should be assigned.

 /return values: 0 for success, 1 if unable to evaluate the derivative.
 */
#ifdef USE_PPP
// ------ parallel case -------

int MappedGridOperators::
derivative(const derivativeTypes & derivativeType_,
	   const realArray & u_, 
	   realArray & ux_, 
	   const Index & I1_ /* = nullIndex */, 
	   const Index & I2_ /* = nullIndex */, 
	   const Index & I3_ /* = nullIndex */, 
	   const Index & C /* =nullIndex */)
{
//  const realSerialArray & u      = u_.getLocalArrayWithGhostBoundaries();
//  const realSerialArray & ux     = ux_.getLocalArrayWithGhostBoundaries();
  realSerialArray u;  getLocalArrayWithGhostBoundaries(u_,u);
  realSerialArray ux; getLocalArrayWithGhostBoundaries(ux_,ux);

  return derivativeInternal(derivativeType_,u,u,ux,I1_,I2_,I3_,C );
}

//! Evaluate a derivative 
/*!
    This routine is a memory and speed efficient way to evaluate derivatives
 /param derivativeType (input) : evaluate the coefficients for this derivative
 /param u (input) : differentiate this function
 /param s (input) : scalar used for some derivatives
 /param ux (output) : result
 /param I1,I2,I3,C (input) : optionally specify which points should be assigned.

 /return values: 0 for success, 1 if unable to evaluate the derivative.
 */
int MappedGridOperators::
derivative(const derivativeTypes & derivativeType_,
	   const realArray & u_, 
	   const realArray & scalar_, 
	   realArray & ux_, 
	   const Index & I1_ /* = nullIndex */, 
	   const Index & I2_ /* = nullIndex */, 
	   const Index & I3_ /* = nullIndex */, 
	   const Index & C /* =nullIndex */)
{
//   const realSerialArray & u      = u_.getLocalArrayWithGhostBoundaries();
//   const realSerialArray & scalar = scalar_.getLocalArrayWithGhostBoundaries();
//   const realSerialArray & ux     = ux_.getLocalArrayWithGhostBoundaries();

  realSerialArray u;  getLocalArrayWithGhostBoundaries(u_,u);
  realSerialArray scalar;  getLocalArrayWithGhostBoundaries(scalar_,scalar);
  realSerialArray ux; getLocalArrayWithGhostBoundaries(ux_,ux);

  return derivativeInternal(derivativeType_,u,scalar,ux,I1_,I2_,I3_,C );
}

#endif 

int MappedGridOperators::
derivative(const derivativeTypes & derivativeType_,
	   const realSerialArray & u, 
	   realSerialArray & ux, 
	   const Index & I1_ /* = nullIndex */, 
	   const Index & I2_ /* = nullIndex */, 
	   const Index & I3_ /* = nullIndex */, 
	   const Index & C /* =nullIndex */)
{
  return derivativeInternal(derivativeType_,u,u,ux,I1_,I2_,I3_,C );
}
int MappedGridOperators::
derivative(const derivativeTypes & derivativeType_,
	   const realSerialArray & u, 
	   const realSerialArray & scalar, 
	   realSerialArray & ux, 
	   const Index & I1_ /* = nullIndex */, 
	   const Index & I2_ /* = nullIndex */, 
	   const Index & I3_ /* = nullIndex */, 
	   const Index & C /* =nullIndex */)
{
  return derivativeInternal(derivativeType_,u,scalar,ux,I1_,I2_,I3_,C );
}



//! Evaluate a derivative 
/*!
    This routine is a memory and speed efficient way to evaluate derivatives
 /param derivativeType (input) : evaluate the coefficients for this derivative
 /param u (input) : differentiate this function
 /param s (input) : scalar used for some derivatives
 /param ux (output) : result
 /param I1,I2,I3,C (input) : optionally specify which points should be assigned.

 /return values: 0 for success, 1 if unable to evaluate the derivative.
 */
int MappedGridOperators::
derivativeInternal(const derivativeTypes & derivativeType_,
		   const realSerialArray & u, 
		   const realSerialArray & scalar, 
		   realSerialArray & ux, 
		   const Index & I1_ /* = nullIndex */, 
		   const Index & I2_ /* = nullIndex */, 
		   const Index & I3_ /* = nullIndex */, 
		   const Index & C /* =nullIndex */)
{


  // kkc 030404
  if ( mappedGrid.getGridType()==GenericGrid::unstructuredGrid )
  {
    UnstructuredOperators uop(mappedGrid);

    #ifndef USE_PPP
      return uop.derivative(derivativeType_, u, scalar, ux, I1_, C);
    #else
      Overture::abort("ERROR: fix this Bill!");
    #endif
  }

  #ifndef USE_PPP
    const intArray & mask = mappedGrid.mask();   // not currently used
  #else
    // const intSerialArray & mask = mappedGrid.mask().getLocalArrayWithGhostBoundaries();
    intSerialArray mask; getLocalArrayWithGhostBoundaries(mappedGrid.mask(),mask);
  #endif
  const int *pmask = mask.getDataPointer();

  const IntegerArray & d = mappedGrid.dimension();

  int w0 = orderOfAccuracy/2;
  int w1 = numberOfDimensions>1 ? w0 : 0;
  int w2 = numberOfDimensions>2 ? w0 : 0;

  int n1a = I1_.length()==0 ? d(0,0)+w0 : I1_.getBase();
  int n1b = I1_.length()==0 ? d(1,0)-w0 : I1_.getBound();
  int n2a = I2_.length()==0 ? d(0,1)+w1 : I2_.getBase();
  int n2b = I2_.length()==0 ? d(1,1)-w1 : I2_.getBound();
  int n3a = I3_.length()==0 ? d(0,2)+w2 : I3_.getBase();
  int n3b = I3_.length()==0 ? d(1,2)-w2 : I3_.getBound();

  n1a=max(n1a,u.getBase(0) +w0);
  n1b=min(n1b,u.getBound(0)-w0);
  n2a=max(n2a,u.getBase(1) +w1);
  n2b=min(n2b,u.getBound(1)-w1);
  n3a=max(n3a,u.getBase(2) +w2);
  n3b=min(n3b,u.getBound(2)-w2);
  
//    #ifdef USE_PPP
//    printf("MGOP:derivative uLocal:node=%i bounds=[%i,%i][%i,%i][%i,%i] \n",Communication_Manager::My_Process_Number,
//  	 u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),u.getBase(2),u.getBound(2));
  
//    printf("MGOP:derivative uLocal:node=%i n1a,..=[%i,%i][%i,%i][%i,%i] \n",Communication_Manager::My_Process_Number,
//           n1a,n1b,n2a,n2b,n3a,n3b);
//    #endif

  const int ca = C.getLength()==0 ? u.getBase(3) : C.getBase();
  const int cb = C.getLength()==0 ? u.getBound(3) : C.getBound();

  // da : denotes the base for ux in the fortran routine: ux(I1,I2,I3,da:*)
  //      If we only evaluate component 5 then we pretend ux looks like ux(I1,I2,I3,5:5)

  const int ndd4a = ux.getBase(3)<=ca && ux.getBound(3)>=cb ? ux.getBase(3) : ca;
  const int ndd4b = ndd4a+(cb-ca);
  
  // printf(" u.getBase(3)=%i u.getBound(3)=%i ux.getBase(3)=%i ux.getBound(3)=%i ca=%i cb=%i ndd4a=%i ndd4b=%i\n",
  //  u.getBase(3),u.getBound(3),ux.getBase(3),ux.getBound(3),ca,cb,ndd4a,ndd4b);
  
  int gridType = rectangular ? 0 : 1;

  // if the grid is rectangular then rsxy is not used, just point to ux
  // real *rsxy = rectangular ? getDataPointer(ux) : getDataPointer(mappedGrid.inverseVertexDerivative());
  real *rsxy=NULL;
  if( !rectangular )
  {
    realArray & inverseVertexDerivative = mappedGrid.inverseVertexDerivative();
    #ifndef USE_PPP
      rsxy=getDataPointer(inverseVertexDerivative);
    #else 
      // rsxy=getDataPointer(inverseVertexDerivative.getLocalArrayWithGhostBoundaries());
      rsxy=inverseVertexDerivative.getLocalArray().getDataPointer();
    #endif
  }
  else
  {
    rsxy = getDataPointer(ux); // not used in this case, just give a valid pointer
  }


  int ndw;
  if( derivativeType_ == divergenceScalarGradient || derivativeType_ == divergenceTensorGradient ||
       (derivativeType_ == laplacianOperator && usingConservativeApproximations() && !rectangular ) ||
       (derivativeType_>=xDerivativeScalarXDerivative && derivativeType_<=zDerivativeScalarZDerivative) ||
      derivativeType_ == divergence ) // *new* 051016
  {
    // we can also do conservative Laplacian non-rectangular here 

    if( orderOfAccuracy!=2 && orderOfAccuracy!=4 && orderOfAccuracy!=6 && orderOfAccuracy!=8 )
    {
      printf("***MappedGridOperators::derivative:ERROR: invalid order of accuracy=%i\n",orderOfAccuracy);
      return 1;
    }

    const int derivative = derivativeType_ == laplacianOperator ? 0 :
                           derivativeType_ == divergenceScalarGradient ? 1 :
                           (derivativeType_>=xDerivativeScalarXDerivative && 
                            derivativeType_<=zDerivativeScalarZDerivative) ? 2 : 
                           derivativeType_ == divergenceTensorGradient ? 3 :
                           derivativeType_ == divergence ? 4 :
                             5; 
    assert( derivative!=5 );
    
    int dir1=-1, dir2=-1;
    if( derivative==2  ) // derivativeScalarDerivative
    {
      dir1=(derivativeType_-xDerivativeScalarXDerivative)/3;
      dir2=(derivativeType_-xDerivativeScalarXDerivative) % 3;
    }

    // *wdh* 040204 ndw=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);  // work space -- no need to use d here
    ndw=(mask.getBound(0)-mask.getBase(0)+1)*
        (mask.getBound(1)-mask.getBase(1)+1)*
        (mask.getBound(2)-mask.getBase(2)+1);  
      
    if( rectangular )
    {
      if( derivative==0 || derivative==1 )
        ndw *= mappedGrid.numberOfDimensions();       
      else if( derivative==3 )
        ndw *= SQR(mappedGrid.numberOfDimensions());  // divTensorGrad needs more work space
      else
        ndw *= 1;  // only 1 work space array needed for derivativeScalarDerivative if rectangular
    }
    else
    {
      ndw *= SQR(mappedGrid.numberOfDimensions());
    }
    if( derivativeType_ == divergence )
      ndw=1;  // no work space needed for divergence
    
    real dr[3];
    real *w = new real [ndw];   

    for( int axis=0; axis<3; axis++ )
    {
      dr[axis]=mappedGrid.gridSpacing(axis); 
    }
    const int derivType= usingConservativeApproximations() ? 1 : 0;
    const int averageType = getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? 0 : 1;

    real *jacobian=NULL;
    if( !rectangular && ( derivativeType_ != divergence || usingConservativeApproximations()) )
    {
      mappedGrid.update(MappedGrid::THEcenterJacobian);
      realArray & centerJacobian = mappedGrid.centerJacobian();
      #ifndef USE_PPP
        jacobian=getDataPointer(centerJacobian);
      #else 
        jacobian=centerJacobian.getLocalArray().getDataPointer();
      #endif
    }
    else
    {
      jacobian = getDataPointer(ux); // not used in this case, just give a valid pointer
    }
    
    // real *jacobian = rectangular ? getDataPointer(ux) : pCenterJacobian;

    divScalarGradFDeriv( mappedGrid.numberOfDimensions(), 
			 mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1),
                         mask.getBase(2),mask.getBound(2),
			 u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),
			 u.getBase(2),u.getBound(2),u.getBase(3),u.getBound(3),
			 ux.getBase(0),ux.getBound(0),ux.getBase(1),ux.getBound(1),
			 ux.getBase(2),ux.getBound(2),ux.getBase(3),ux.getBound(3),
			 n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
			 dx[0], dr[0],
			 *rsxy, *jacobian, 
			 *getDataPointer(u), *getDataPointer(scalar), *getDataPointer(ux),
			 ndw,w[0],
			 derivative, derivType, gridType, orderOfAccuracy, averageType, dir1,dir2  );

    delete [] w;
    return 0;

  }

  // *wdh* 090420 -- evaluate the conservative vorticity in non-conservative form
  bool computeNonconservativeDerivative=false;
  if( usingConservativeApproximations() && orderOfAccuracy==2 && !rectangular &&
      derivativeType_== vorticityOperator)
  {
    printF("MappedGridOperators::derivative:WARNING: conservative approx. not available for derivativeType=%i, "
           " using non-conservative approx. instead\n",derivativeType_);
    computeNonconservativeDerivative=true;
  }
  
  
  
  if( !usingConservativeApproximations() || orderOfAccuracy==4 || rectangular || computeNonconservativeDerivative )
  {
    int ipar[]={n1a,n1b,n2a,n2b,n3a,n3b,ca,cb,gridType,orderOfAccuracy }; //
    real rpar[]={mappedGrid.gridSpacing(0),
		 mappedGrid.gridSpacing(1),
		 mappedGrid.gridSpacing(2),
		 dx[0],dx[1],dx[2] };  //

    if( orderOfAccuracy==2 || orderOfAccuracy==4 )
    {
      RealArray d12,d22,d14,d24, h21(3),h22(3),h41(3),h42(3);  // fix this -- only pass dr,dx
      if( !rectangular )
      {
	d12=1./(2.*mappedGrid.gridSpacing());  
	d22=1./SQR(mappedGrid.gridSpacing());

	d14=1./(12.*mappedGrid.gridSpacing());
	d24=1./(12.*SQR(mappedGrid.gridSpacing()));
      }
      else
      {
	for( int axis=0; axis<3; axis++ )
	{
	  h21(axis)=1./(2.*dx[axis]); 
	  h22(axis)=1./SQR(dx[axis]);

	  h41(axis)=1./(12.*dx[axis]);
	  h42(axis)=1./(12.*SQR(dx[axis]));
	}
      }
    
      switch (derivativeType_) 
      {
      case xDerivative:
	DERIV_OPT(xFDeriv);
	break;
      case yDerivative:
	DERIV_OPT(yFDeriv);
	break;
      case zDerivative:
	DERIV_OPT(zFDeriv);
	break;
      case xxDerivative:
	DERIV_OPT(xxFDeriv);
	break;
      case xyDerivative:
	DERIV_OPT(xyFDeriv);
	break;
      case xzDerivative:
	DERIV_OPT(xzFDeriv);
	break;
      case yyDerivative:
	DERIV_OPT(yyFDeriv);
	break;
      case yzDerivative:
	DERIV_OPT(yzFDeriv);
	break;
      case zzDerivative:
	DERIV_OPT(zzFDeriv);
	break;
      case laplacianOperator:
	DERIV_OPT(laplacianFDeriv);
	break;
      case vorticityOperator:  // *wdh* 090317 
	DERIVATIVE_OPT(vorticityFiniteDiffDeriv);
	break;
      default:
       printF("MappedGridOperators::derivative:ERROR: This operator not implemented yet. "
 	     " derivativeType=%i\n",(int)derivativeType_);
	return 1;
      }
      
      
    }
    else if( orderOfAccuracy==6 || orderOfAccuracy==8 )
    {
      // 6th or 8th order approximations


      switch (derivativeType_) 
      {
      case xDerivative:
  	DERIVATIVE_OPT(xFiniteDiffDeriv);
	break;
      case yDerivative:
        DERIVATIVE_OPT(yFiniteDiffDeriv);
	break;
      case zDerivative:
	DERIVATIVE_OPT(zFiniteDiffDeriv);
	break;
      case xxDerivative:
  	DERIVATIVE_OPT(xxFiniteDiffDeriv);
	break;
      case xyDerivative:
	DERIVATIVE_OPT(xyFiniteDiffDeriv);
	break;
      case xzDerivative:
	DERIVATIVE_OPT(xzFiniteDiffDeriv);
	break;
      case yyDerivative:
	DERIVATIVE_OPT(yyFiniteDiffDeriv);
	break;
      case yzDerivative:
	DERIVATIVE_OPT(yzFiniteDiffDeriv);
	break;
      case zzDerivative:
	DERIVATIVE_OPT(zzFiniteDiffDeriv);
	break;
      case laplacianOperator:
	DERIVATIVE_OPT(laplacianFiniteDiffDeriv);
	break;
      case divergence:
	DERIVATIVE_OPT(divFiniteDiffDeriv);
	break;
      case gradient:
	DERIVATIVE_OPT(gradFiniteDiffDeriv);
	break;
      case vorticityOperator:
	DERIVATIVE_OPT(vorticityFiniteDiffDeriv);
	break;
      default:
//       printf("MappedGridOperators::derivative:ERROR: This operator not implemented yet. "
// 	     " derivativeType=%i\n",(int)derivativeType_);
	return 1;
      }
      
    }
    else
    {
      printF("***MappedGridOperators::derivative:ERROR: invalid order of accuracy=%i\n",orderOfAccuracy);
      return 1;
    }
    
  }
  else
  {
    // here are conservative approximations
    // **NOTE: The function xi call this function and keeps track of which derivatives were not evaluated
    if( false )
    {
      printF("MappedGridOperators::derivative:ERROR: opt version of derivativeType=%i not implemented for "
	     "conservative yet.\n",derivativeType_);
    }
    return 1;
  }
  

//    #ifdef USE_PPP
//      printf("MGOP:...leaving derivative node=%i\n",Communication_Manager::My_Process_Number);
//    #endif
  return 0;
}

