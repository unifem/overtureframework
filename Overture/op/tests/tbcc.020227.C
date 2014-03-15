#include "Overture.h"
#include "MappedGridOperators.h"
#include "NameList.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "display.h"
#include "SparseRep.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


const int DIRICHLET = 1;
const int NEUMANN = 2;
const int EXTRAPOLATION = 3;

//================================================================================
//  **** Test the Coefficient Matrix boundary conditions *****
//================================================================================

// These macros define how to access the elements in a coefficient matrix. See the example below
#undef C
#undef M123
#define M123(m1,m2,m3) (m1+halfWidth1+width1*(m2+halfWidth2+width2*(m3+halfWidth3)))

#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))

#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))

#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilLength0*(n))


#define COEFF(m1,m2,m3,c,e,I1,I2,I3) coeff(M123CE(m1,m2,m3,c,e),I1,I2,I3)

int
getResidual(MappedGrid & mg, 
            const realArray & coeff, 
	    const realArray & uu,
	    const realArray & ff,
	    realArray & res,
	    const Index & I1,
	    const Index & I2,
	    const Index & I3,
	    const Index & Iu1,
	    const Index & Iu2,
	    const Index & Iu3,
            const int & numberOfComponents,
            const int & stencilSize,
            const int & orderOfAccuracy)
{
  const int width1=orderOfAccuracy+1, halfWidth1=width1/2, width2=width1, halfWidth2=width2/2;
  const int width3= mg.numberOfDimensions()==2 ? 1 : width1, halfWidth3=width3/2;
  const int & numberOfComponentsForCoefficients = numberOfComponents;
  

  for( int e=0; e<numberOfComponents; e++ )
  {
    res(0,I1,I2,I3,e)=ff(0,I1,I2,I3,e);
    if( mg.numberOfDimensions()==2 )
    {
      // The COEFF macro makes the coeff array look like a 8 dimensional array.
      for( int c=0; c<numberOfComponents; c++ )
      {
        if( orderOfAccuracy==2 )
	{
	  res(0,I1,I2,I3,e)-=(    
	     COEFF( 0, 0,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3,c)
	    +COEFF( 1, 0,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3,c)
	    +COEFF( 0, 1,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3,c)
	    +COEFF(-1, 0,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3,c)
	    +COEFF( 0,-1,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3,c)
	    +COEFF( 1, 1,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3,c)
	    +COEFF( 1,-1,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3,c)
	    +COEFF(-1, 1,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3,c)
	    +COEFF(-1,-1,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3,c)
	    );
	}
	else
	{
	  res(0,I1,I2,I3,e)-=(    
	    +COEFF(-2,-2,0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-2,Iu3,c)
	    +COEFF(-1,-2,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-2,Iu3,c)
	    +COEFF( 0,-2,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-2,Iu3,c)
	    +COEFF( 1,-2,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-2,Iu3,c)
	    +COEFF( 2,-2,0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-2,Iu3,c)

	    +COEFF(-2,-1,0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-1,Iu3,c)
	    +COEFF(-1,-1,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3,c)
	    +COEFF( 0,-1,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3,c)
	    +COEFF( 1,-1,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3,c)
	    +COEFF( 2,-1,0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-1,Iu3,c)

	    +COEFF(-2, 0,0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2  ,Iu3,c)
	    +COEFF(-1, 0,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3,c)
	    +COEFF( 0, 0,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3,c)
	    +COEFF( 1, 0,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3,c)
	    +COEFF( 2, 0,0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2  ,Iu3,c)

	    +COEFF(-2, 1,0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+1,Iu3,c)
	    +COEFF(-1, 1,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3,c)
	    +COEFF( 0, 1,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3,c)
	    +COEFF( 1, 1,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3,c)
	    +COEFF( 2, 1,0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+1,Iu3,c)

	    +COEFF(-2, 2,0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+2,Iu3,c)
	    +COEFF(-1, 2,0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+2,Iu3,c)
	    +COEFF( 0, 2,0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+2,Iu3,c)
	    +COEFF( 1, 2,0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+2,Iu3,c)
	    +COEFF( 2, 2,0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+2,Iu3,c)

	    );
	}
      }
    }
    else
    {
      for( int c=0; c<numberOfComponents; c++ )
      {
        if( orderOfAccuracy==2 )
	{
	  res(0,I1,I2,I3,e)-=(
	     COEFF(-1,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3-1,c)
	    +COEFF( 0,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3-1,c)
	    +COEFF( 1,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3-1,c)
	    +COEFF(-1, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3-1,c)
	    +COEFF( 0, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3-1,c)
	    +COEFF( 1, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3-1,c)
	    +COEFF(-1, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3-1,c)
	    +COEFF( 0, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3-1,c)
	    +COEFF( 1, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3-1,c)
				       	    	 
	    +COEFF(-1,-1, 0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3  ,c)
	    +COEFF( 0,-1, 0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3  ,c)
	    +COEFF( 1,-1, 0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3  ,c)
	    +COEFF(-1, 0, 0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3  ,c)
	    +COEFF( 0, 0, 0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3  ,c)
	    +COEFF( 1, 0, 0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3  ,c)
	    +COEFF(-1, 1, 0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3  ,c)
	    +COEFF( 0, 1, 0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3  ,c)
	    +COEFF( 1, 1, 0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3  ,c)
				       	    	 
	    +COEFF(-1,-1, 1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3+1,c)
	    +COEFF( 0,-1, 1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3+1,c)
	    +COEFF( 1,-1, 1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3+1,c)
	    +COEFF(-1, 0, 1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3+1,c)
	    +COEFF( 0, 0, 1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3+1,c)
	    +COEFF( 1, 0, 1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3+1,c)
	    +COEFF(-1, 1, 1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3+1,c)
	    +COEFF( 0, 1, 1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3+1,c)
	    +COEFF( 1, 1, 1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3+1,c)
	    );
	}
	else
	{
	  res(0,I1,I2,I3,e)-=(    
	    +COEFF(-2,-2,-2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-2,Iu3-2,c)
	    +COEFF(-1,-2,-2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-2,Iu3-2,c)
	    +COEFF( 0,-2,-2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-2,Iu3-2,c)
	    +COEFF( 1,-2,-2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-2,Iu3-2,c)
	    +COEFF( 2,-2,-2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-2,Iu3-2,c)

	    +COEFF(-2,-1,-2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-1,Iu3-2,c)
	    +COEFF(-1,-1,-2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3-2,c)
	    +COEFF( 0,-1,-2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3-2,c)
	    +COEFF( 1,-1,-2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3-2,c)
	    +COEFF( 2,-1,-2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-1,Iu3-2,c)

	    +COEFF(-2, 0,-2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2  ,Iu3-2,c)
	    +COEFF(-1, 0,-2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3-2,c)
	    +COEFF( 0, 0,-2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3-2,c)
	    +COEFF( 1, 0,-2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3-2,c)
	    +COEFF( 2, 0,-2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2  ,Iu3-2,c)

	    +COEFF(-2, 1,-2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+1,Iu3-2,c)
	    +COEFF(-1, 1,-2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3-2,c)
	    +COEFF( 0, 1,-2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3-2,c)
	    +COEFF( 1, 1,-2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3-2,c)
	    +COEFF( 2, 1,-2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+1,Iu3-2,c)

	    +COEFF(-2, 2,-2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+2,Iu3-2,c)
	    +COEFF(-1, 2,-2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+2,Iu3-2,c)
	    +COEFF( 0, 2,-2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+2,Iu3-2,c)
	    +COEFF( 1, 2,-2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+2,Iu3-2,c)
	    +COEFF( 2, 2,-2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+2,Iu3-2,c)

	    +COEFF(-2,-2,-1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-2,Iu3-1,c)
	    +COEFF(-1,-2,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-2,Iu3-1,c)
	    +COEFF( 0,-2,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-2,Iu3-1,c)
	    +COEFF( 1,-2,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-2,Iu3-1,c)
	    +COEFF( 2,-2,-1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-2,Iu3-1,c)

	    +COEFF(-2,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-1,Iu3-1,c)
	    +COEFF(-1,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3-1,c)
	    +COEFF( 0,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3-1,c)
	    +COEFF( 1,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3-1,c)
	    +COEFF( 2,-1,-1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-1,Iu3-1,c)

	    +COEFF(-2, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2  ,Iu3-1,c)
	    +COEFF(-1, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3-1,c)
	    +COEFF( 0, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3-1,c)
	    +COEFF( 1, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3-1,c)
	    +COEFF( 2, 0,-1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2  ,Iu3-1,c)

	    +COEFF(-2, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+1,Iu3-1,c)
	    +COEFF(-1, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3-1,c)
	    +COEFF( 0, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3-1,c)
	    +COEFF( 1, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3-1,c)
	    +COEFF( 2, 1,-1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+1,Iu3-1,c)

	    +COEFF(-2, 2,-1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+2,Iu3-1,c)
	    +COEFF(-1, 2,-1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+2,Iu3-1,c)
	    +COEFF( 0, 2,-1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+2,Iu3-1,c)
	    +COEFF( 1, 2,-1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+2,Iu3-1,c)
	    +COEFF( 2, 2,-1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+2,Iu3-1,c)

	    +COEFF(-2,-2,+0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-2,Iu3+0,c)
	    +COEFF(-1,-2,+0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-2,Iu3+0,c)
	    +COEFF( 0,-2,+0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-2,Iu3+0,c)
	    +COEFF( 1,-2,+0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-2,Iu3+0,c)
	    +COEFF( 2,-2,+0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-2,Iu3+0,c)

	    +COEFF(-2,-1,+0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-1,Iu3+0,c)
	    +COEFF(-1,-1,+0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3+0,c)
	    +COEFF( 0,-1,+0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3+0,c)
	    +COEFF( 1,-1,+0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3+0,c)
	    +COEFF( 2,-1,+0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-1,Iu3+0,c)

	    +COEFF(-2, 0,+0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2  ,Iu3+0,c)
	    +COEFF(-1, 0,+0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3+0,c)
	    +COEFF( 0, 0,+0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3+0,c)
	    +COEFF( 1, 0,+0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3+0,c)
	    +COEFF( 2, 0,+0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2  ,Iu3+0,c)

	    +COEFF(-2, 1,+0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+1,Iu3+0,c)
	    +COEFF(-1, 1,+0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3+0,c)
	    +COEFF( 0, 1,+0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3+0,c)
	    +COEFF( 1, 1,+0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3+0,c)
	    +COEFF( 2, 1,+0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+1,Iu3+0,c)

	    +COEFF(-2, 2,+0,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+2,Iu3+0,c)
	    +COEFF(-1, 2,+0,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+2,Iu3+0,c)
	    +COEFF( 0, 2,+0,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+2,Iu3+0,c)
	    +COEFF( 1, 2,+0,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+2,Iu3+0,c)
	    +COEFF( 2, 2,+0,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+2,Iu3+0,c)

	    +COEFF(-2,-2,+1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-2,Iu3+1,c)
	    +COEFF(-1,-2,+1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-2,Iu3+1,c)
	    +COEFF( 0,-2,+1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-2,Iu3+1,c)
	    +COEFF( 1,-2,+1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-2,Iu3+1,c)
	    +COEFF( 2,-2,+1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-2,Iu3+1,c)

	    +COEFF(-2,-1,+1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-1,Iu3+1,c)
	    +COEFF(-1,-1,+1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3+1,c)
	    +COEFF( 0,-1,+1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3+1,c)
	    +COEFF( 1,-1,+1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3+1,c)
	    +COEFF( 2,-1,+1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-1,Iu3+1,c)

	    +COEFF(-2, 0,+1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2  ,Iu3+1,c)
	    +COEFF(-1, 0,+1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3+1,c)
	    +COEFF( 0, 0,+1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3+1,c)
	    +COEFF( 1, 0,+1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3+1,c)
	    +COEFF( 2, 0,+1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2  ,Iu3+1,c)

	    +COEFF(-2, 1,+1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+1,Iu3+1,c)
	    +COEFF(-1, 1,+1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3+1,c)
	    +COEFF( 0, 1,+1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3+1,c)
	    +COEFF( 1, 1,+1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3+1,c)
	    +COEFF( 2, 1,+1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+1,Iu3+1,c)

	    +COEFF(-2, 2,+1,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+2,Iu3+1,c)
	    +COEFF(-1, 2,+1,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+2,Iu3+1,c)
	    +COEFF( 0, 2,+1,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+2,Iu3+1,c)
	    +COEFF( 1, 2,+1,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+2,Iu3+1,c)
	    +COEFF( 2, 2,+1,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+2,Iu3+1,c)

	    +COEFF(-2,-2,+2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-2,Iu3+2,c)
	    +COEFF(-1,-2,+2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-2,Iu3+2,c)
	    +COEFF( 0,-2,+2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-2,Iu3+2,c)
	    +COEFF( 1,-2,+2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-2,Iu3+2,c)
	    +COEFF( 2,-2,+2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-2,Iu3+2,c)

	    +COEFF(-2,-1,+2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2-1,Iu3+2,c)
	    +COEFF(-1,-1,+2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2-1,Iu3+2,c)
	    +COEFF( 0,-1,+2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2-1,Iu3+2,c)
	    +COEFF( 1,-1,+2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2-1,Iu3+2,c)
	    +COEFF( 2,-1,+2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2-1,Iu3+2,c)

	    +COEFF(-2, 0,+2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2  ,Iu3+2,c)
	    +COEFF(-1, 0,+2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2  ,Iu3+2,c)
	    +COEFF( 0, 0,+2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2  ,Iu3+2,c)
	    +COEFF( 1, 0,+2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2  ,Iu3+2,c)
	    +COEFF( 2, 0,+2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2  ,Iu3+2,c)

	    +COEFF(-2, 1,+2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+1,Iu3+2,c)
	    +COEFF(-1, 1,+2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+1,Iu3+2,c)
	    +COEFF( 0, 1,+2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+1,Iu3+2,c)
	    +COEFF( 1, 1,+2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+1,Iu3+2,c)
	    +COEFF( 2, 1,+2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+1,Iu3+2,c)

	    +COEFF(-2, 2,+2,c,e,I1,I2,I3)*uu(0,Iu1-2,Iu2+2,Iu3+2,c)
	    +COEFF(-1, 2,+2,c,e,I1,I2,I3)*uu(0,Iu1-1,Iu2+2,Iu3+2,c)
	    +COEFF( 0, 2,+2,c,e,I1,I2,I3)*uu(0,Iu1  ,Iu2+2,Iu3+2,c)
	    +COEFF( 1, 2,+2,c,e,I1,I2,I3)*uu(0,Iu1+1,Iu2+2,Iu3+2,c)
	    +COEFF( 2, 2,+2,c,e,I1,I2,I3)*uu(0,Iu1+2,Iu2+2,Iu3+2,c)


	    );
	}
	
      }
    }
  }
  return 0;
}


int
getExtrapolationResidual(MappedGrid & mg, 
            const realArray & coeff, 
	    const realArray & uu,
	    const realArray & ff,
	    realArray & res,
	    const Index & I1,
	    const Index & I2,
	    const Index & I3,
	    const int & side,
	    const int & axis,
            const int & numberOfComponents,
            const int & stencilSize)
{
  const int & numberOfComponentsForCoefficients = numberOfComponents;
  
  int d[3], &d1=d[0], &d2=d[1], &d3=d[2];
  d1=d2=d3=0;
  d[axis]=1-2*side;

  for( int e=0; e<numberOfComponents; e++ )
  {
    res(0,I1,I2,I3,e)=ff(0,I1,I2,I3,e);
    // The COEFF macro makes the coeff array look like a 8 dimensional array.
    for( int c=0; c<numberOfComponents; c++ )
    {
      res(0,I1,I2,I3,e)-=(    
	 coeff(0+CE(c,e),I1,I2,I3)*uu(0,I1     ,I2     ,I3     ,c)
	+coeff(1+CE(c,e),I1,I2,I3)*uu(0,I1+d1  ,I2+d2  ,I3+d3  ,c)
	+coeff(2+CE(c,e),I1,I2,I3)*uu(0,I1+d1*2,I2+d2*2,I3+d3*2,c)
	+coeff(3+CE(c,e),I1,I2,I3)*uu(0,I1+d1*3,I2+d2*3,I3+d3*3,c)
	+coeff(4+CE(c,e),I1,I2,I3)*uu(0,I1+d1*4,I2+d2*4,I3+d3*4,c)
	+coeff(5+CE(c,e),I1,I2,I3)*uu(0,I1+d1*5,I2+d2*5,I3+d3*5,c)
	);
    }
  }
  return 0;
}


int
residual(realMappedGridFunction & coeff, 
         realMappedGridFunction & u,
         realMappedGridFunction & f,
         realMappedGridFunction & residual)
// ==================================================================================
// /Description:
//   Compute the residual for a coefficient matrix, res = f - coeff * u.
// /coeff (input) : coefficient matrix.
// /u, f (input)  : solution and right hand side.
// /residual (output) : residual.
//
// =================================================================================
{
  realArray & c = coeff;
  realArray & uu= u;
  realArray & ff= f;
  realArray & res = residual;

  int numberOfComponents=coeff.sparse->numberOfComponents;
  const int & stencilSize=coeff.sparse->stencilSize;
  const int orderOfAccuracy = coeff.getOperators()->orderOfAccuracy;
  
  // printf(" getResidual: numberOfComponents=%i, stencilSize=%i \n",numberOfComponents,stencilSize);
  
  // We must first reshape the arrays so that we can multiply by the coefficient matrix
  uu.reshape(1,uu.dimension(0),uu.dimension(1),uu.dimension(2),uu.dimension(3));
  ff.reshape(1,ff.dimension(0),ff.dimension(1),ff.dimension(2),ff.dimension(3));
  res.reshape(1,res.dimension(0),res.dimension(1),res.dimension(2),res.dimension(3));
      
  MappedGrid & mg = *coeff.getMappedGrid();
  Index I1,I2,I3;
  getIndex(mg.indexRange(),I1,I2,I3);  

  getResidual(mg,c,uu,ff,res,I1,I2,I3,I1,I2,I3,numberOfComponents,stencilSize,orderOfAccuracy);


  int side,axis;
  Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
  for( axis=0; axis<mg.numberOfDimensions(); axis++ ) 
  {
    for( side=0; side<=1; side++ )
    {
      if( mg.boundaryCondition(side,axis)==NEUMANN )
      {
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
        getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
        getResidual(mg,c,uu,ff,res,Ig1,Ig2,Ig3,Ib1,Ib2,Ib3,numberOfComponents,stencilSize,orderOfAccuracy);
      }
      else if( mg.boundaryCondition(side,axis)==EXTRAPOLATION )
      {
        getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
        getExtrapolationResidual(mg,c,uu,ff,res,Ig1,Ig2,Ig3,side,axis,numberOfComponents,stencilSize);
      }
    }
  }

  // reshape the arrays back to their original shape -- this would
  // be essential if the GridFunctions u,f or res were used again
  uu.reshape(uu.dimension(1),uu.dimension(2),uu.dimension(3),uu.dimension(4));
  ff.reshape(ff.dimension(1),ff.dimension(2),ff.dimension(3),ff.dimension(4));
  res.reshape(res.dimension(1),res.dimension(2),res.dimension(3),res.dimension(4));
  return 0;
}


real
getBoundaryError(MappedGrid & mg,
		 const realArray & res)
{
  const int numberOfComponents=res.getLength(4);
  real error=0.;
  int side,axis;
  Index Ib1,Ib2,Ib3;
  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition(side,axis) > 0 )
    {
      getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3);
      where( mg.mask()(Ib1,Ib2,Ib3)>0 )
      {
	for( int c=0; c<numberOfComponents; c++ )
  	  error=max(error,max(abs(res(Ib1,Ib2,Ib3,c))));    
      }
    }
  }
  return error;
}

real
getGhostError(MappedGrid & mg,
              const realArray & res,
              const int extra=0)
{
  const int numberOfComponents=res.getLength(4);
  real error=0.;
  int side,axis;
  Index Ig1,Ig2,Ig3;
  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition()(side,axis) > 0 )
    {
      getGhostIndex(mg.indexRange(),side,axis,Ig1,Ig2,Ig3,extra);
      where( mg.mask()(Ig1,Ig2,Ig3)>0 )
      {
	for( int c=0; c<numberOfComponents; c++ )
	  error=max(error,max(abs(res(Ig1,Ig2,Ig3,c))));    
      }
    }
  }
  return error;
}

bool measureCPU=TRUE;

real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU();
  else
    return 0;
}


int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  int debug=3;

  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
    
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
        measureCPU=FALSE;
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[1];
      }
    }
  }
  else
    cout << "Usage: `tbcc [<gridName>] [-noTiming]' \n";

  // make some shorter names for readability
  BCTypes::BCNames 
                   dirichlet                  = BCTypes::dirichlet,
                   neumann                    = BCTypes::neumann,
                   extrapolate                = BCTypes::extrapolate,
                   normalComponent            = BCTypes::normalComponent,
                   extrapolateNormalComponent = BCTypes::extrapolateNormalComponent,
              extrapolateTangentialComponent0 = BCTypes::extrapolateTangentialComponent0,
              extrapolateTangentialComponent1 = BCTypes::extrapolateTangentialComponent1,
                   aDotU                      = BCTypes::aDotU,
                   generalizedDivergence      = BCTypes::generalizedDivergence,
                   generalMixedDerivative     = BCTypes::generalMixedDerivative,
                   aDotGradU                  = BCTypes::aDotGradU,
                   normalDotScalarGrad        = BCTypes::normalDotScalarGrad,
                   evenSymmetry               = BCTypes::evenSymmetry,
                   vectorSymmetry             = BCTypes::vectorSymmetry,
                   tangentialComponent        = BCTypes::tangentialComponent,
                   tangentialComponent0       = BCTypes::tangentialComponent0,
                   tangentialComponent1       = BCTypes::tangentialComponent1,
            normalDerivativeOfNormalComponent = BCTypes::normalDerivativeOfNormalComponent,
       normalDerivativeOfTangentialComponent0 = BCTypes::normalDerivativeOfTangentialComponent0,
       normalDerivativeOfTangentialComponent1 = BCTypes::normalDerivativeOfTangentialComponent1,
                   allBoundaries              = BCTypes::allBoundaries,
                   boundary1                  = BCTypes::boundary1; 

  real overallWorstError=0.;
  real time,time1,time2;
  

  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];

    cout << "\n *****************************************************************\n";
    cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
    cout << " *****************************************************************\n\n";



    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);


    // define an exact solution for testing
    // each component is a different polynomial of degree 2
    int degreeSpace = 2;
    int degreeTime = 1;
    int numberOfComponents = cg.numberOfDimensions();
    OGPolyFunction exact(degreeSpace,cg.numberOfDimensions(),numberOfComponents,degreeTime);

    RealArray spatialCoefficientsForTZ(5,5,5,numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5,numberOfComponents);      
    timeCoefficientsForTZ=0.;
    int n;
    for( n=0; n<numberOfComponents; n++ )
    {
      real ni =1./(n+1);
      spatialCoefficientsForTZ(0,0,0,n)=1.;      
      if( degreeSpace>0 )
      {
	spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
	spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,0,1,n)= cg.numberOfDimensions()==3 ? .25*ni : 0.;
      }
      if( degreeSpace>1 )
      {
	spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	spatialCoefficientsForTZ(0,0,2,n)= cg.numberOfDimensions()==3 ? .125*ni : 0.;
      }
    }
    for( n=0; n<numberOfComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
    }
    exact.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 

    real error=0., worstError=0.;
    // loop over all component grids
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid]; 

      cout << "\n+++++++Checking component grid = " << grid 
           << " (" << mg.mapping().getName(Mapping::mappingName) << ") +++++++" << endl;

      mg.update(MappedGrid::THEcenter | MappedGrid::THEmask);
      mg.update(MappedGrid::THEcenterBoundaryNormal | MappedGrid::THEcenterBoundaryTangent);

//       mg.update(MappedGrid::THEinverseVertexDerivative); // *****
//       display(mg.inverseVertexDerivative(),"rsxy","%8.2e ");
      

      const intArray & mask = mg.mask();
      const realArray & center = mg.center();

      Index I1,I2,I3;
      Index Ib1,Ib2,Ib3;
      Index Ig1,Ig2,Ig3;    

      for(int orderOfAccuracy=2; orderOfAccuracy<=4; orderOfAccuracy+=2 )
      {

        printf("\n ******************* orderOfAccuracy=%i ****************************\n",orderOfAccuracy);
	
      // make a grid function to hold the coefficients
	Range all;
	int stencilSize=int( pow(orderOfAccuracy+1,mg.numberOfDimensions()) );
	realMappedGridFunction coeff(mg,stencilSize,all,all,all); 
	coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    
      // create grid functions: 
	realMappedGridFunction u(mg),f(mg),res(mg), scalar(mg);
	res=0.;

	MappedGridOperators op(mg);                            // create some differential operators
	op.setStencilSize(stencilSize);
	op.setOrderOfAccuracy(orderOfAccuracy);

	coeff.setOperators(op);
	u.setOperators(op);
  
      // ****************************************************************
      //       Test laplacian
      // ****************************************************************
	where( mg.boundaryCondition() > 0 )
	  mg.boundaryCondition()(all,all)=100;

	for( int c=0; c<=1; c++ ) // ***** conservative or non-conservative
	{
          if( c==0 && orderOfAccuracy==4 )
            continue;

	  op.useConservativeApproximations(c==0);
	  if( c==0 )
	    printf("**Using conservative approximations:\n");
	  else
	    printf("**Using standard approximations:\n");

#beginMacro testCoefficients(operator,operatorName)    
	  getIndex(mg.dimension(),I1,I2,I3);
	  time=CPU();
	  coeff=op.operator ## Coefficients();
	  time=CPU()-time;

	  // display(coeff,"laplacianCoefficients: coeff","%6.1e ");

	  u=exact(mg,I1,I2,I3,0);
	  f=u.operator();
      
	  residual(coeff,u,f,res);
	  getIndex(mg.indexRange(),I1,I2,I3);
	  where( mask(I1,I2,I3)>0 )
	    error=max(abs(res(I1,I2,I3)));
	  worstError=max(worstError,error);
	  printf("Maximum error in operator                         = %e, cpu=%e \n",error,time);  

        // display(res,"laplacianCoefficients: res","%6.1e ");
        // display(f,"laplacianCoefficients: f","%6.1e ");

	  if( c==1 )
	  {
	    // *** efficient version ****
	    coeff=0.;
	    time=CPU();
	    op.coefficients(MappedGridOperators::operatorName,coeff);
	    time=CPU()-time;

	    // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");

	    residual(coeff,u,f,res);
	    getIndex(mg.indexRange(),I1,I2,I3);
	    where( mask(I1,I2,I3)>0 )
	      error=max(abs(res(I1,I2,I3)));
	    worstError=max(worstError,error);
	    printf("Maximum error in operator (efficient version)     = %e, cpu=%e \n",error,time);  
	  
	  }  
#endMacro
          testCoefficients(laplacian,laplacianOperator)


          if( orderOfAccuracy==4 )  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            continue;
	
	  // ****************************************************************
	  //       Test div( s grad u )
	  // ****************************************************************

	  getIndex(mg.dimension(),I1,I2,I3);
	  // scalar=1.+center(I1,I2,I3,0)+2.*center(I1,I2,I3,1)+.25*center(I1,I2,I3,0)*center(I1,I2,I3,1);

        // for harmonic averaging we do not want a negative scalar
	  realArray xy; xy = 1.+center(I1,I2,I3,0)+2.*center(I1,I2,I3,1)+.25*center(I1,I2,I3,0)*center(I1,I2,I3,1);
	  real xyMax=max(fabs(xy));
	  scalar=1.+xy*(.5/xyMax);
	  scalar=1.;

	  aString averageType;
	  for( int a=0; a<=1; a++ )
	  {
	    if( a==1 )
	    {
	      op.setAveragingType(MappedGridOperators::arithmeticAverage);
	      averageType="(arithmetic)";
	      // printf("  --Using arithmetic average\n");
	    }
	    else
	    {
	      op.setAveragingType(MappedGridOperators::harmonicAverage);
	      averageType="(harmonic)  ";
	      // printf("  --Using harmonic average\n");
	    }
	    getIndex(mg.dimension(),I1,I2,I3);

	    coeff=0.;
      
	    time=CPU();
	    coeff=op.divScalarGradCoefficients(scalar);
	    time=CPU()-time;
	    u=exact(mg,I1,I2,I3,0);

	    f=u.divScalarGrad(scalar);
      
	    residual(coeff,u,f,res);
	    getIndex(mg.indexRange(),I1,I2,I3);
	    where( mask(I1,I2,I3)>0 )
	      error=max(abs(res(I1,I2,I3)));
	    worstError=max(worstError,error);
	    printf("Maximum error in divScalarGrad  %s       = %e, cpu=%e \n",(const char*)averageType,
		   error,time);  

	  }
	
	  // display(coeff,"divScalarGradCoefficients: coeff","%6.1e ");
	  // display(res,"divScalarGradCoefficients: res","%6.1e ");
	  // display(f,"divScalarGradCoefficients: f","%6.1e ");
	  
	} // end for conservative
	
      
	if( orderOfAccuracy==4 )  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	  continue;

	coeff=op.identityCoefficients();   
  
	getIndex(mg.dimension(),I1,I2,I3);

	// ****************************************************************
	//       dirichlet 
	// ****************************************************************
	where( mg.boundaryCondition() > 0 )
	  mg.boundaryCondition()(all,all)=DIRICHLET;
    
	time=CPU();
	coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,allBoundaries);
	time=CPU()-time;
//    coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
	coeff.finishBoundaryConditions();

	int side,axis;

      // compute the residual
	u=exact(mg,I1,I2,I3,0);
	f=exact(mg,I1,I2,I3,0);
    
	res=123456789.;
	residual(coeff,u,f,res);

	if( FALSE && debug & 2 )
	{
	  display(coeff,"coeff");
	  display(u,"u");
	  display(res,"res");
	}
      
	error=getBoundaryError(mg,res);
	worstError=max(worstError,error);
	printf("Maximum error in dirichlet                         = %e, cpu=%e \n",error,time);  


      // ****************************************************************
      //       neumann
      // ****************************************************************
	where( mg.boundaryCondition() > 0 )
	  mg.boundaryCondition()(all,all)=NEUMANN;
	time=CPU();

//    coeff.updateToMatchGrid(mg,MappedGridFunction::updateCoefficientMatrix);
//    realMappedGridFunction coeff(mg,stencilSize,all,all,all); 
//  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
	coeff=op.identityCoefficients();
	coeff.applyBoundaryConditionCoefficients(0,0,neumann,allBoundaries);
	coeff.finishBoundaryConditions();
	time=CPU()-time;

	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & normal = mg.centerBoundaryNormal(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    if( mg.numberOfDimensions()==2 )
	      f(Ig1,Ig2,Ig3)=normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0);
	    else
	      f(Ig1,Ig2,Ig3)=normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0);
	  }
	}

	res=123456789.;
	residual(coeff,u,f,res);
	// display(res,"res after neumann");

	error=getGhostError(mg,res);
	worstError=max(worstError,error);
	// display(coeff,"coeff");
	printf("Maximum error in neumann                           = %e, cpu=%e \n",error,time); 


	BoundaryConditionParameters bcParams;


	// ****************************************************************
	//       normalDotScalarGrad
	// ****************************************************************

	time=CPU();
	bcParams.setVariableCoefficients(scalar);
      
	coeff=op.identityCoefficients();
	coeff.applyBoundaryConditionCoefficients(0,0,normalDotScalarGrad,allBoundaries,bcParams);
	coeff.finishBoundaryConditions();
	time=CPU()-time;

	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & normal = mg.centerBoundaryNormal(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    if( mg.numberOfDimensions()==2 )
	      f(Ig1,Ig2,Ig3)=(normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)+
			      normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0))*scalar(Ib1,Ib2,Ib3);
	    else
	      f(Ig1,Ig2,Ig3)=(normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)+
			      normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)+
			      normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0))*scalar(Ib1,Ib2,Ib3);
	  }
	}

	res=123456789.;
	residual(coeff,u,f,res);

	error=getGhostError(mg,res);
	worstError=max(worstError,error);
	printf("Maximum error in normalDotScalarGrad               = %e, cpu=%e \n",error,time); 



      // ****************************************************************
      //    generalMixedDerivative
      // ****************************************************************
	bcParams.a.redim(4);
	bcParams.a(0)=1.;
	bcParams.a(1)=2.;
	bcParams.a(2)=3.;
	bcParams.a(3)=4.;
	const RealArray & a = bcParams.a;

	coeff.applyBoundaryConditionCoefficients(0,0,generalMixedDerivative,allBoundaries,bcParams);

	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & normal = mg.centerBoundaryNormal(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    f(Ig1,Ig2,Ig3)=a(0)*exact  (mg,Ib1,Ib2,Ib3,0)
	      +a(1)*exact.x(mg,Ib1,Ib2,Ib3,0)
	      +a(2)*exact.y(mg,Ib1,Ib2,Ib3,0);
	    if( mg.numberOfDimensions()>2 )
	      f(Ig1,Ig2,Ig3)+=a(3)*exact.z(mg,Ib1,Ib2,Ib3,0);
	  }
	}
	res=123456789.;
	residual(coeff,u,f,res);

	error=getGhostError(mg,res);
	worstError=max(worstError,error);
	// display(coeff,"coeff");
	printf("Maximum error in generalMixedDerivative            = %e, cpu=%e \n",error,time); 



	// +++++++++ Now test BC's that apply to a system of equations ++++++++++++++

	const int numberOfComponents=mg.numberOfDimensions();
	u.updateToMatchGrid(mg,all,all,all,numberOfComponents);
	f.updateToMatchGrid(mg,all,all,all,numberOfComponents);
	res.updateToMatchGrid(mg,all,all,all,numberOfComponents);

	getIndex(mg.dimension(),I1,I2,I3);
	Range R(0,mg.numberOfDimensions()-1);
	u(I1,I2,I3,R)=exact(mg,I1,I2,I3,R);
	f=u;
    

    
	const int & numberOfComponentsForCoefficients=numberOfComponents;
	int stencilDimension=stencilSize*SQR(numberOfComponentsForCoefficients);
	coeff.updateToMatchGrid(mg,stencilDimension,all,all,all); 
	int numberOfGhostLines=1;  // we will solve for values including the first ghostline
	coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponentsForCoefficients);

	op.setStencilSize(stencilSize);
	op.setNumberOfComponentsForCoefficients(numberOfComponentsForCoefficients);


	Range e0(0,0), e1(1,1), e2(2,2);  // e0 = first equation, e1=second equation
	Range c0(0,0), c1(1,1), c2(2,2);  // c0 = first component, c1 = second component

	coeff=op.identityCoefficients(I1,I2,I3,e0,c0)+op.identityCoefficients(I1,I2,I3,e1,c1);
	if( mg.numberOfDimensions()==3 )
	  coeff+=op.identityCoefficients(I1,I2,I3,e2,c2);

      // ****************************************************************
      //       normalComponent
      // ****************************************************************
	where( mg.boundaryCondition() > 0 )
	  mg.boundaryCondition()(all,all)=DIRICHLET;
       
	residual(coeff,u,f,res);
	// display(coeff,"coeff for identity operator");
	// display(res,"residual for identity operator");


	Range Rx(0,mg.numberOfDimensions()-1);
	time=CPU();
	coeff.applyBoundaryConditionCoefficients(e0,Rx,normalComponent,allBoundaries);
	coeff.applyBoundaryConditionCoefficients(e1,c1,dirichlet      ,allBoundaries);
	if( mg.numberOfDimensions()==3 )
	  coeff.applyBoundaryConditionCoefficients(e2,c2,dirichlet      ,allBoundaries);
	time=CPU()-time;
	coeff.finishBoundaryConditions();

	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & normal = mg.centerBoundaryNormal(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    if( mg.numberOfDimensions()==2 )
	      f(Ib1,Ib2,Ib3,0)=normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1);
	    else
	      f(Ib1,Ib2,Ib3,0)=normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1)
		+normal(Ib1,Ib2,Ib3,2)*exact(mg,Ib1,Ib2,Ib3,2);
	  }
	}
	res=123456789.;
	residual(coeff,u,f,res);

	error=getBoundaryError(mg,res);
	worstError=max(worstError,error);
	printf("Maximum error in normalComponent                   = %e, cpu=%e \n",error,time); 

      // ****************************************************************
      //       tangentialComponent
      // ****************************************************************
       
	time=CPU();
	coeff.applyBoundaryConditionCoefficients(e0,Rx,tangentialComponent0,allBoundaries);
	coeff.applyBoundaryConditionCoefficients(e1,c1,dirichlet           ,allBoundaries);
	if( mg.numberOfDimensions()==3 )
	  coeff.applyBoundaryConditionCoefficients(e2,Rx,tangentialComponent1,allBoundaries);

	time=CPU()-time;
	coeff.finishBoundaryConditions();

	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    if( mg.numberOfDimensions()==2 )
	      f(Ib1,Ib2,Ib3,0)=tangent(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0)
		+tangent(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1);
	    if( mg.numberOfDimensions()==3 )
	    {
	      f(Ib1,Ib2,Ib3,0)=tangent(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0)
		+tangent(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1)
		+tangent(Ib1,Ib2,Ib3,2)*exact(mg,Ib1,Ib2,Ib3,2);
	      const int ndt=3;
	      f(Ib1,Ib2,Ib3,2)=tangent(Ib1,Ib2,Ib3,0+ndt)*exact(mg,Ib1,Ib2,Ib3,0)
		+tangent(Ib1,Ib2,Ib3,1+ndt)*exact(mg,Ib1,Ib2,Ib3,1)
		+tangent(Ib1,Ib2,Ib3,2+ndt)*exact(mg,Ib1,Ib2,Ib3,2);
	    }
	  }
	}

	res=123456789.;
	residual(coeff,u,f,res);
	// display(res,"res after neumann");

	error=getBoundaryError(mg,res);
	worstError=max(worstError,error);
	// display(coeff,"coeff");
	printf("Maximum error in tangentialComponent               = %e, cpu=%e \n",error,time); 


      // ****************************************************************
      //    generalizedDivergence
      // ****************************************************************
	coeff=op.identityCoefficients(I1,I2,I3,e0,c0)+op.identityCoefficients(I1,I2,I3,e1,c1);
	if( mg.numberOfDimensions()==3 )
	  coeff+=op.identityCoefficients(I1,I2,I3,e2,c2);

	where( mg.boundaryCondition() > 0 )
	  mg.boundaryCondition()(all,all)=NEUMANN;
	bcParams.a.redim(3);
	bcParams.a(0)=2.;
	bcParams.a(1)=3.;
	bcParams.a(2)=4.;

	time=CPU();
	coeff.applyBoundaryConditionCoefficients(e0,Rx,generalizedDivergence,allBoundaries,bcParams);
	coeff.applyBoundaryConditionCoefficients(e1,c1,dirichlet           ,allBoundaries);
	if( mg.numberOfDimensions()==3 )
	  coeff.applyBoundaryConditionCoefficients(e2,c2,dirichlet,allBoundaries);
	time=CPU()-time;
	coeff.finishBoundaryConditions();

	f=u;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    if( mg.numberOfDimensions()==2 )
	      f(Ig1,Ig2,Ig3,0)=a(0)*exact.x(mg,Ib1,Ib2,Ib3,0)
		+a(1)*exact.y(mg,Ib1,Ib2,Ib3,1);
	    if( mg.numberOfDimensions()==3 )
	    {
	      f(Ig1,Ig2,Ig3,0)=a(0)*exact.x(mg,Ib1,Ib2,Ib3,0)
		+a(1)*exact.y(mg,Ib1,Ib2,Ib3,1)
		+a(2)*exact.z(mg,Ib1,Ib2,Ib3,2);
	    }
	  }
	}

	res=123456789.;
	residual(coeff,u,f,res);
	error=getGhostError(mg,res);
	worstError=max(worstError,error);
	printf("Maximum error in generalizedDivergence             = %e, cpu=%e \n",error,time); 

      // ****************************************************************
      //    normalDerivativeOf...
      // ****************************************************************
	time=CPU();
	coeff.applyBoundaryConditionCoefficients(e0,Rx,normalDerivativeOfNormalComponent,allBoundaries);
	coeff.applyBoundaryConditionCoefficients(e1,Rx,normalDerivativeOfTangentialComponent0,allBoundaries);
	if( mg.numberOfDimensions()==3 )
	  coeff.applyBoundaryConditionCoefficients(e2,Rx,normalDerivativeOfTangentialComponent1,allBoundaries);
	time=CPU()-time;
	coeff.finishBoundaryConditions();

	f=u;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & normal  = mg.centerBoundaryNormal(side,axis);
	    const realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    for( int n=0; n<mg.numberOfDimensions(); n++ )
	    {
	      const realArray & vector = n==0 ? normal : tangent;
	      const int ndt = n==2 ? 3 : 0;

	      if( mg.numberOfDimensions()==2 )
		f(Ig1,Ig2,Ig3,n)=
		  vector(Ib1,Ib2,Ib3,0+ndt)*(
		    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
		    +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
		    )
		  +vector(Ib1,Ib2,Ib3,1+ndt)*(
		    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,1)
		    +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,1)
		    );
	      else
		f(Ig1,Ig2,Ig3,n)=
		  vector(Ib1,Ib2,Ib3,0+ndt)*(
		    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
		    +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
		    +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0)
		    )
		  +vector(Ib1,Ib2,Ib3,1+ndt)*(
		    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,1)
		    +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,1)
		    +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,1)
		    )
		  +vector(Ib1,Ib2,Ib3,2+ndt)*(
		    normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,2)
		    +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,2)
		    +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,2)
		    );
	    }
	  }
	}

	res=123456789.;
	residual(coeff,u,f,res);
	error=getGhostError(mg,res);
	worstError=max(worstError,error);
	printf("Maximum error in normalDerivativeOf[Normal/Tangent]= %e, cpu=%e \n",error,time); 


      // ****************************************************************
      //    vector Symmetry
      // ****************************************************************
      // coeff=op.identityCoefficients(I1,I2,I3,e0,c0)+op.identityCoefficients(I1,I2,I3,e1,c1);

	time=CPU();
	coeff.applyBoundaryConditionCoefficients(Rx,Rx,vectorSymmetry,allBoundaries);
	time=CPU()-time;
	coeff.finishBoundaryConditions();

	f=u;
	Index Ip1,Ip2,Ip3;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & normal  = mg.centerBoundaryNormal(side,axis);
	    const realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line in
	    if( mg.numberOfDimensions()==2 )
	    {
	      f(Ig1,Ig2,Ig3,0)=   
		normal(Ib1,Ib2,Ib3,0)*(exact(mg,Ig1,Ig2,Ig3,0)+exact(mg,Ip1,Ip2,Ip3,0))
		+normal(Ib1,Ib2,Ib3,1)*(exact(mg,Ig1,Ig2,Ig3,1)+exact(mg,Ip1,Ip2,Ip3,1));
	      f(Ig1,Ig2,Ig3,1)=  
		tangent(Ib1,Ib2,Ib3,0)*(exact(mg,Ig1,Ig2,Ig3,0)-exact(mg,Ip1,Ip2,Ip3,0))
		+tangent(Ib1,Ib2,Ib3,1)*(exact(mg,Ig1,Ig2,Ig3,1)-exact(mg,Ip1,Ip2,Ip3,1));
	    }
	    if( mg.numberOfDimensions()==3 )
	    {
	      f(Ig1,Ig2,Ig3,0)=   
		normal(Ib1,Ib2,Ib3,0)*(exact(mg,Ig1,Ig2,Ig3,0)+exact(mg,Ip1,Ip2,Ip3,0))
		+normal(Ib1,Ib2,Ib3,1)*(exact(mg,Ig1,Ig2,Ig3,1)+exact(mg,Ip1,Ip2,Ip3,1))
		+normal(Ib1,Ib2,Ib3,2)*(exact(mg,Ig1,Ig2,Ig3,2)+exact(mg,Ip1,Ip2,Ip3,2));
	      f(Ig1,Ig2,Ig3,1)=  
		tangent(Ib1,Ib2,Ib3,0)*(exact(mg,Ig1,Ig2,Ig3,0)-exact(mg,Ip1,Ip2,Ip3,0))
		+tangent(Ib1,Ib2,Ib3,1)*(exact(mg,Ig1,Ig2,Ig3,1)-exact(mg,Ip1,Ip2,Ip3,1))
		+tangent(Ib1,Ib2,Ib3,2)*(exact(mg,Ig1,Ig2,Ig3,2)-exact(mg,Ip1,Ip2,Ip3,2));
	      f(Ig1,Ig2,Ig3,2)=  
		tangent(Ib1,Ib2,Ib3,3)*(exact(mg,Ig1,Ig2,Ig3,0)-exact(mg,Ip1,Ip2,Ip3,0))
		+tangent(Ib1,Ib2,Ib3,4)*(exact(mg,Ig1,Ig2,Ig3,1)-exact(mg,Ip1,Ip2,Ip3,1))
		+tangent(Ib1,Ib2,Ib3,5)*(exact(mg,Ig1,Ig2,Ig3,2)-exact(mg,Ip1,Ip2,Ip3,2));
	    }
	  }
	}
	res=123456789.;
	residual(coeff,u,f,res);
	error=getGhostError(mg,res,-1);
	worstError=max(worstError,error);
	printf("Maximum error in vectorSymmetry                    = %e, cpu=%e \n",error,time); 

      // ****************************************************************
      //    evenSymmetry
      // ****************************************************************
	time=CPU();
	coeff.applyBoundaryConditionCoefficients(e0,c0,evenSymmetry,allBoundaries);
	coeff.applyBoundaryConditionCoefficients(e1,c1,evenSymmetry,allBoundaries);
	if( mg.numberOfDimensions()==3 )
	  coeff.applyBoundaryConditionCoefficients(e2,c2,evenSymmetry,allBoundaries);
	time=CPU()-time;
	coeff.finishBoundaryConditions();

      //display(coeff,"coeff evenSymmetry");

	f=u;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line in
	    f(Ig1,Ig2,Ig3,Rx)=exact(mg,Ig1,Ig2,Ig3,Rx)-exact(mg,Ip1,Ip2,Ip3,Rx);
	  }
	}
	res=123456789.;
	residual(coeff,u,f,res);
	// display(coeff,"coeff");
	// display(res,"residual");
    
	error=getGhostError(mg,res);
	worstError=max(worstError,error);
	printf("Maximum error in evenSymmetry                      = %e, cpu=%e \n",error,time); 


      // ****************************************************************
      //    extrapolate[Normal/Tangential]Component
      // ****************************************************************
	where( mg.boundaryCondition() > 0 )
	  mg.boundaryCondition()(all,all)=EXTRAPOLATION;

	time=CPU();
	coeff.applyBoundaryConditionCoefficients(e0,Rx,extrapolateNormalComponent,allBoundaries);
	coeff.applyBoundaryConditionCoefficients(e1,Rx,extrapolateTangentialComponent0,allBoundaries);
	if( mg.numberOfDimensions()==3 )
	  coeff.applyBoundaryConditionCoefficients(e2,Rx,extrapolateTangentialComponent1,allBoundaries);
	time=CPU()-time;
	coeff.finishBoundaryConditions();
	f=u;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    const realArray & normal  = mg.centerBoundaryNormal(side,axis);
	    const realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

	    f(Ig1,Ig2,Ig3,Rx)=0.;
/* --------
   if( mg.numberOfDimensions()==2 )
   {
   f(Ig1,Ig2,Ig3,0)=   normal(Ib1,Ib2,Ib3,0)*exact(mg,Ig1,Ig2,Ig3,0)
   +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ig1,Ig2,Ig3,1);
   f(Ig1,Ig2,Ig3,1)=  tangent(Ib1,Ib2,Ib3,0)*exact(mg,Ig1,Ig2,Ig3,0)
   +tangent(Ib1,Ib2,Ib3,1)*exact(mg,Ig1,Ig2,Ig3,1);
   }
   if( mg.numberOfDimensions()==3 )
   {
   f(Ig1,Ig2,Ig3,0)=   normal(Ib1,Ib2,Ib3,0)*exact(mg,Ig1,Ig2,Ig3,0)
   +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ig1,Ig2,Ig3,1)
   +normal(Ib1,Ib2,Ib3,2)*exact(mg,Ig1,Ig2,Ig3,2);
   f(Ig1,Ig2,Ig3,1)=  tangent(Ib1,Ib2,Ib3,0)*exact(mg,Ig1,Ig2,Ig3,0)
   +tangent(Ib1,Ib2,Ib3,1)*exact(mg,Ig1,Ig2,Ig3,1)
   +tangent(Ib1,Ib2,Ib3,2)*exact(mg,Ig1,Ig2,Ig3,2);
   f(Ig1,Ig2,Ig3,2)=  tangent(Ib1,Ib2,Ib3,3)*exact(mg,Ig1,Ig2,Ig3,0)
   +tangent(Ib1,Ib2,Ib3,4)*exact(mg,Ig1,Ig2,Ig3,1)
   +tangent(Ib1,Ib2,Ib3,5)*exact(mg,Ig1,Ig2,Ig3,2);
   }
   ----- */
	  }
	}
	res=123456789.;
	residual(coeff,u,f,res);
	// display(coeff,"coeff");
	// display(res,"residual");
    
	error=getGhostError(mg,res);
	worstError=max(worstError,error);
	printf("Maximum error in extrapolate[Normal/Tangential]    = %e, cpu=%e \n",error,time); 

      }

    }  // end loop over order of accuracy
    
    overallWorstError=max(overallWorstError,worstError);
    
    if( worstError > .01 )
      printf("\n ******* Warning, there is a large error somewhere on this grid, worst error =%e ************\n",
	     worstError);
    else
      printf("\n *********** This grid is apparently ok, worst error =%e ***************\n",worstError);

  } // end loop over grids
  
  printf("\n\n **************************************************************************************************\n");
  if( overallWorstError > .01 )
    printf(" *********** Warning, there was a large error somewhere, overall worst error =%e ***************\n",
           overallWorstError);
  else
    printf(" *********** Test apparently successful, overall worst error =%e ***************\n",overallWorstError);
  printf(" **************************************************************************************************\n\n");
    
  cout << "Program Terminated Normally! \n";
  Overture::finish();          
  return 0;
}
