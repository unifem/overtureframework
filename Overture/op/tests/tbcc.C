// This file automatically generated from tbcc.bC with bpp.
//================================================================================
//  **** Test the Coefficient Matrix operators and boundary conditions *****
//================================================================================


#include "Overture.h"
#include "MappedGridOperators.h"
#include "NameList.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "display.h"
#include "SparseRep.h"
#include "Checker.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )


const int DIRICHLET = 1;
const int NEUMANN = 2;
const int EXTRAPOLATION = 3;

// ** new macro versions: 
int
setCoefficients( MappedGridOperators::derivativeTypes deriv, 
                                  realMappedGridFunction & coeff,
                                  realMappedGridFunction *s = NULL,
                                  int orderOfAccuracy=2,
                                  int conservative=0 );


// These macros define how to access the elements in a coefficient matrix. See the example below
#undef C
#undef M123
#define M123(m1,m2,m3) (m1+halfWidth1+width1*(m2+halfWidth2+width2*(m3+halfWidth3)))

#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))

#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))

#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilLength0*(n))

#define COEFF(m1,m2,m3,c,e,I1,I2,I3) CC(M123CE(m1,m2,m3,c,e),I1,I2,I3)

static real timeForGetResidual=0.;

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
    real time=getCPU();

    const int width1=orderOfAccuracy+1, halfWidth1=width1/2, width2=width1, halfWidth2=width2/2;
    const int width3= mg.numberOfDimensions()==2 ? 1 : width1, halfWidth3=width3/2;
    const int & numberOfComponentsForCoefficients = numberOfComponents;
    
    const real *uup = uu.Array_Descriptor.Array_View_Pointer3;
    const int uuDim0=uu.getRawDataSize(0);
    const int uuDim1=uu.getRawDataSize(1);
    const int uuDim2=uu.getRawDataSize(2);
#define UU(i0,i1,i2,i3) uup[i0+uuDim0*(i1+uuDim1*(i2+uuDim2*(i3)))]
    const real *ffp = ff.Array_Descriptor.Array_View_Pointer3;
    const int ffDim0=ff.getRawDataSize(0);
    const int ffDim1=ff.getRawDataSize(1);
    const int ffDim2=ff.getRawDataSize(2);
#define FF(i0,i1,i2,i3) ffp[i0+ffDim0*(i1+ffDim1*(i2+ffDim2*(i3)))]
    real *resp = res.Array_Descriptor.Array_View_Pointer3;
    const int resDim0=res.getRawDataSize(0);
    const int resDim1=res.getRawDataSize(1);
    const int resDim2=res.getRawDataSize(2);
#define RES(i0,i1,i2,i3) resp[i0+resDim0*(i1+resDim1*(i2+resDim2*(i3)))]
    const real *coeffp = coeff.Array_Descriptor.Array_View_Pointer3;
    const int coeffDim0=coeff.getRawDataSize(0);
    const int coeffDim1=coeff.getRawDataSize(1);
    const int coeffDim2=coeff.getRawDataSize(2);
#define CC(i0,i1,i2,i3) coeffp[i0+coeffDim0*(i1+coeffDim1*(i2+coeffDim2*(i3)))]

    int i1,i2,i3,iu1,iu2,iu3,I1Bound,I2Bound,I3Bound;
    
#define FOR_3(i1,i2,i3,I1,I2,I3)I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound(); for( i3=I3.getBase(); i3<=I3Bound; i3++ ) for( i2=I2.getBase(); i2<=I2Bound; i2++ ) for( i1=I1.getBase(); i1<=I1Bound; i1++ )

#define FOR_3U(i1,i2,i3)I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound(); for( i3=I3.getBase(), iu3=Iu3.getBase(); i3<=I3Bound; i3++,iu3++ ) for( i2=I2.getBase(), iu2=Iu2.getBase(); i2<=I2Bound; i2++,iu2++ ) for( i1=I1.getBase(), iu1=Iu1.getBase(); i1<=I1Bound; i1++,iu1++ ) 

    for( int e=0; e<numberOfComponents; e++ )
    {
        FOR_3(i1,i2,i3,I1,I2,I3)
        {
            RES(i1,i2,i3,e)=FF(i1,i2,i3,e);
        }
        if( mg.numberOfDimensions()==2 )
        {
      // The COEFF macro makes the coeff array look like a 8 dimensional array.
            for( int c=0; c<numberOfComponents; c++ )
            {
                if( orderOfAccuracy==2 )
      	{
                    FOR_3U(i1,i2,i3)
                    {
        	  RES(i1,i2,i3,e)-=(    
           	     COEFF( 0, 0,0,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3,c)
          	    +COEFF( 1, 0,0,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3,c)
          	    +COEFF( 0, 1,0,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3,c)
          	    +COEFF(-1, 0,0,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3,c)
          	    +COEFF( 0,-1,0,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3,c)
          	    +COEFF( 1, 1,0,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3,c)
          	    +COEFF( 1,-1,0,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3,c)
          	    +COEFF(-1, 1,0,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3,c)
          	    +COEFF(-1,-1,0,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3,c)
          	    );
        	  }
      	}
      	else
      	{
                    FOR_3U(i1,i2,i3)
                    {
        	  RES(i1,i2,i3,e)-=(    
          	    +COEFF(-2,-2,0,c,e,i1,i2,i3)*UU(iu1-2,iu2-2,iu3,c)
          	    +COEFF(-1,-2,0,c,e,i1,i2,i3)*UU(iu1-1,iu2-2,iu3,c)
          	    +COEFF( 0,-2,0,c,e,i1,i2,i3)*UU(iu1  ,iu2-2,iu3,c)
          	    +COEFF( 1,-2,0,c,e,i1,i2,i3)*UU(iu1+1,iu2-2,iu3,c)
          	    +COEFF( 2,-2,0,c,e,i1,i2,i3)*UU(iu1+2,iu2-2,iu3,c)

          	    +COEFF(-2,-1,0,c,e,i1,i2,i3)*UU(iu1-2,iu2-1,iu3,c)
          	    +COEFF(-1,-1,0,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3,c)
          	    +COEFF( 0,-1,0,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3,c)
          	    +COEFF( 1,-1,0,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3,c)
          	    +COEFF( 2,-1,0,c,e,i1,i2,i3)*UU(iu1+2,iu2-1,iu3,c)

          	    +COEFF(-2, 0,0,c,e,i1,i2,i3)*UU(iu1-2,iu2  ,iu3,c)
          	    +COEFF(-1, 0,0,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3,c)
          	    +COEFF( 0, 0,0,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3,c)
          	    +COEFF( 1, 0,0,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3,c)
          	    +COEFF( 2, 0,0,c,e,i1,i2,i3)*UU(iu1+2,iu2  ,iu3,c)

          	    +COEFF(-2, 1,0,c,e,i1,i2,i3)*UU(iu1-2,iu2+1,iu3,c)
          	    +COEFF(-1, 1,0,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3,c)
          	    +COEFF( 0, 1,0,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3,c)
          	    +COEFF( 1, 1,0,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3,c)
          	    +COEFF( 2, 1,0,c,e,i1,i2,i3)*UU(iu1+2,iu2+1,iu3,c)

          	    +COEFF(-2, 2,0,c,e,i1,i2,i3)*UU(iu1-2,iu2+2,iu3,c)
          	    +COEFF(-1, 2,0,c,e,i1,i2,i3)*UU(iu1-1,iu2+2,iu3,c)
          	    +COEFF( 0, 2,0,c,e,i1,i2,i3)*UU(iu1  ,iu2+2,iu3,c)
          	    +COEFF( 1, 2,0,c,e,i1,i2,i3)*UU(iu1+1,iu2+2,iu3,c)
          	    +COEFF( 2, 2,0,c,e,i1,i2,i3)*UU(iu1+2,iu2+2,iu3,c)

          	    );
        	  }
        	  
      	}
            }
        }
        else
        {
            for( int c=0; c<numberOfComponents; c++ )
            {
                if( orderOfAccuracy==2 )
      	{
                    FOR_3U(i1,i2,i3)
                    {
        	  RES(i1,i2,i3,e)-=(
           	     COEFF(-1,-1,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3-1,c)
          	    +COEFF( 0,-1,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3-1,c)
          	    +COEFF( 1,-1,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3-1,c)
          	    +COEFF(-1, 0,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3-1,c)
          	    +COEFF( 0, 0,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3-1,c)
          	    +COEFF( 1, 0,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3-1,c)
          	    +COEFF(-1, 1,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3-1,c)
          	    +COEFF( 0, 1,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3-1,c)
          	    +COEFF( 1, 1,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3-1,c)
                                                				       	    	 
          	    +COEFF(-1,-1, 0,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3  ,c)
          	    +COEFF( 0,-1, 0,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3  ,c)
          	    +COEFF( 1,-1, 0,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3  ,c)
          	    +COEFF(-1, 0, 0,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3  ,c)
          	    +COEFF( 0, 0, 0,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3  ,c)
          	    +COEFF( 1, 0, 0,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3  ,c)
          	    +COEFF(-1, 1, 0,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3  ,c)
          	    +COEFF( 0, 1, 0,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3  ,c)
          	    +COEFF( 1, 1, 0,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3  ,c)
                                                				       	    	 
          	    +COEFF(-1,-1, 1,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3+1,c)
          	    +COEFF( 0,-1, 1,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3+1,c)
          	    +COEFF( 1,-1, 1,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3+1,c)
          	    +COEFF(-1, 0, 1,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3+1,c)
          	    +COEFF( 0, 0, 1,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3+1,c)
          	    +COEFF( 1, 0, 1,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3+1,c)
          	    +COEFF(-1, 1, 1,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3+1,c)
          	    +COEFF( 0, 1, 1,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3+1,c)
          	    +COEFF( 1, 1, 1,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3+1,c)
          	    );
        	  }
        	  
      	}
      	else
      	{
                    FOR_3U(i1,i2,i3)
                    {
        	  RES(i1,i2,i3,e)-=(    
          	    +COEFF(-2,-2,-2,c,e,i1,i2,i3)*UU(iu1-2,iu2-2,iu3-2,c)
          	    +COEFF(-1,-2,-2,c,e,i1,i2,i3)*UU(iu1-1,iu2-2,iu3-2,c)
          	    +COEFF( 0,-2,-2,c,e,i1,i2,i3)*UU(iu1  ,iu2-2,iu3-2,c)
          	    +COEFF( 1,-2,-2,c,e,i1,i2,i3)*UU(iu1+1,iu2-2,iu3-2,c)
          	    +COEFF( 2,-2,-2,c,e,i1,i2,i3)*UU(iu1+2,iu2-2,iu3-2,c)

          	    +COEFF(-2,-1,-2,c,e,i1,i2,i3)*UU(iu1-2,iu2-1,iu3-2,c)
          	    +COEFF(-1,-1,-2,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3-2,c)
          	    +COEFF( 0,-1,-2,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3-2,c)
          	    +COEFF( 1,-1,-2,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3-2,c)
          	    +COEFF( 2,-1,-2,c,e,i1,i2,i3)*UU(iu1+2,iu2-1,iu3-2,c)

          	    +COEFF(-2, 0,-2,c,e,i1,i2,i3)*UU(iu1-2,iu2  ,iu3-2,c)
          	    +COEFF(-1, 0,-2,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3-2,c)
          	    +COEFF( 0, 0,-2,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3-2,c)
          	    +COEFF( 1, 0,-2,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3-2,c)
          	    +COEFF( 2, 0,-2,c,e,i1,i2,i3)*UU(iu1+2,iu2  ,iu3-2,c)

          	    +COEFF(-2, 1,-2,c,e,i1,i2,i3)*UU(iu1-2,iu2+1,iu3-2,c)
          	    +COEFF(-1, 1,-2,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3-2,c)
          	    +COEFF( 0, 1,-2,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3-2,c)
          	    +COEFF( 1, 1,-2,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3-2,c)
          	    +COEFF( 2, 1,-2,c,e,i1,i2,i3)*UU(iu1+2,iu2+1,iu3-2,c)

          	    +COEFF(-2, 2,-2,c,e,i1,i2,i3)*UU(iu1-2,iu2+2,iu3-2,c)
          	    +COEFF(-1, 2,-2,c,e,i1,i2,i3)*UU(iu1-1,iu2+2,iu3-2,c)
          	    +COEFF( 0, 2,-2,c,e,i1,i2,i3)*UU(iu1  ,iu2+2,iu3-2,c)
          	    +COEFF( 1, 2,-2,c,e,i1,i2,i3)*UU(iu1+1,iu2+2,iu3-2,c)
          	    +COEFF( 2, 2,-2,c,e,i1,i2,i3)*UU(iu1+2,iu2+2,iu3-2,c)

          	    +COEFF(-2,-2,-1,c,e,i1,i2,i3)*UU(iu1-2,iu2-2,iu3-1,c)
          	    +COEFF(-1,-2,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2-2,iu3-1,c)
          	    +COEFF( 0,-2,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2-2,iu3-1,c)
          	    +COEFF( 1,-2,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2-2,iu3-1,c)
          	    +COEFF( 2,-2,-1,c,e,i1,i2,i3)*UU(iu1+2,iu2-2,iu3-1,c)

          	    +COEFF(-2,-1,-1,c,e,i1,i2,i3)*UU(iu1-2,iu2-1,iu3-1,c)
          	    +COEFF(-1,-1,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3-1,c)
          	    +COEFF( 0,-1,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3-1,c)
          	    +COEFF( 1,-1,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3-1,c)
          	    +COEFF( 2,-1,-1,c,e,i1,i2,i3)*UU(iu1+2,iu2-1,iu3-1,c)

          	    +COEFF(-2, 0,-1,c,e,i1,i2,i3)*UU(iu1-2,iu2  ,iu3-1,c)
          	    +COEFF(-1, 0,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3-1,c)
          	    +COEFF( 0, 0,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3-1,c)
          	    +COEFF( 1, 0,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3-1,c)
          	    +COEFF( 2, 0,-1,c,e,i1,i2,i3)*UU(iu1+2,iu2  ,iu3-1,c)

          	    +COEFF(-2, 1,-1,c,e,i1,i2,i3)*UU(iu1-2,iu2+1,iu3-1,c)
          	    +COEFF(-1, 1,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3-1,c)
          	    +COEFF( 0, 1,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3-1,c)
          	    +COEFF( 1, 1,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3-1,c)
          	    +COEFF( 2, 1,-1,c,e,i1,i2,i3)*UU(iu1+2,iu2+1,iu3-1,c)

          	    +COEFF(-2, 2,-1,c,e,i1,i2,i3)*UU(iu1-2,iu2+2,iu3-1,c)
          	    +COEFF(-1, 2,-1,c,e,i1,i2,i3)*UU(iu1-1,iu2+2,iu3-1,c)
          	    +COEFF( 0, 2,-1,c,e,i1,i2,i3)*UU(iu1  ,iu2+2,iu3-1,c)
          	    +COEFF( 1, 2,-1,c,e,i1,i2,i3)*UU(iu1+1,iu2+2,iu3-1,c)
          	    +COEFF( 2, 2,-1,c,e,i1,i2,i3)*UU(iu1+2,iu2+2,iu3-1,c)

          	    +COEFF(-2,-2,+0,c,e,i1,i2,i3)*UU(iu1-2,iu2-2,iu3+0,c)
          	    +COEFF(-1,-2,+0,c,e,i1,i2,i3)*UU(iu1-1,iu2-2,iu3+0,c)
          	    +COEFF( 0,-2,+0,c,e,i1,i2,i3)*UU(iu1  ,iu2-2,iu3+0,c)
          	    +COEFF( 1,-2,+0,c,e,i1,i2,i3)*UU(iu1+1,iu2-2,iu3+0,c)
          	    +COEFF( 2,-2,+0,c,e,i1,i2,i3)*UU(iu1+2,iu2-2,iu3+0,c)

          	    +COEFF(-2,-1,+0,c,e,i1,i2,i3)*UU(iu1-2,iu2-1,iu3+0,c)
          	    +COEFF(-1,-1,+0,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3+0,c)
          	    +COEFF( 0,-1,+0,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3+0,c)
          	    +COEFF( 1,-1,+0,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3+0,c)
          	    +COEFF( 2,-1,+0,c,e,i1,i2,i3)*UU(iu1+2,iu2-1,iu3+0,c)

          	    +COEFF(-2, 0,+0,c,e,i1,i2,i3)*UU(iu1-2,iu2  ,iu3+0,c)
          	    +COEFF(-1, 0,+0,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3+0,c)
          	    +COEFF( 0, 0,+0,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3+0,c)
          	    +COEFF( 1, 0,+0,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3+0,c)
          	    +COEFF( 2, 0,+0,c,e,i1,i2,i3)*UU(iu1+2,iu2  ,iu3+0,c)

          	    +COEFF(-2, 1,+0,c,e,i1,i2,i3)*UU(iu1-2,iu2+1,iu3+0,c)
          	    +COEFF(-1, 1,+0,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3+0,c)
          	    +COEFF( 0, 1,+0,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3+0,c)
          	    +COEFF( 1, 1,+0,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3+0,c)
          	    +COEFF( 2, 1,+0,c,e,i1,i2,i3)*UU(iu1+2,iu2+1,iu3+0,c)

          	    +COEFF(-2, 2,+0,c,e,i1,i2,i3)*UU(iu1-2,iu2+2,iu3+0,c)
          	    +COEFF(-1, 2,+0,c,e,i1,i2,i3)*UU(iu1-1,iu2+2,iu3+0,c)
          	    +COEFF( 0, 2,+0,c,e,i1,i2,i3)*UU(iu1  ,iu2+2,iu3+0,c)
          	    +COEFF( 1, 2,+0,c,e,i1,i2,i3)*UU(iu1+1,iu2+2,iu3+0,c)
          	    +COEFF( 2, 2,+0,c,e,i1,i2,i3)*UU(iu1+2,iu2+2,iu3+0,c)

          	    +COEFF(-2,-2,+1,c,e,i1,i2,i3)*UU(iu1-2,iu2-2,iu3+1,c)
          	    +COEFF(-1,-2,+1,c,e,i1,i2,i3)*UU(iu1-1,iu2-2,iu3+1,c)
          	    +COEFF( 0,-2,+1,c,e,i1,i2,i3)*UU(iu1  ,iu2-2,iu3+1,c)
          	    +COEFF( 1,-2,+1,c,e,i1,i2,i3)*UU(iu1+1,iu2-2,iu3+1,c)
          	    +COEFF( 2,-2,+1,c,e,i1,i2,i3)*UU(iu1+2,iu2-2,iu3+1,c)

          	    +COEFF(-2,-1,+1,c,e,i1,i2,i3)*UU(iu1-2,iu2-1,iu3+1,c)
          	    +COEFF(-1,-1,+1,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3+1,c)
          	    +COEFF( 0,-1,+1,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3+1,c)
          	    +COEFF( 1,-1,+1,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3+1,c)
          	    +COEFF( 2,-1,+1,c,e,i1,i2,i3)*UU(iu1+2,iu2-1,iu3+1,c)

          	    +COEFF(-2, 0,+1,c,e,i1,i2,i3)*UU(iu1-2,iu2  ,iu3+1,c)
          	    +COEFF(-1, 0,+1,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3+1,c)
          	    +COEFF( 0, 0,+1,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3+1,c)
          	    +COEFF( 1, 0,+1,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3+1,c)
          	    +COEFF( 2, 0,+1,c,e,i1,i2,i3)*UU(iu1+2,iu2  ,iu3+1,c)

          	    +COEFF(-2, 1,+1,c,e,i1,i2,i3)*UU(iu1-2,iu2+1,iu3+1,c)
          	    +COEFF(-1, 1,+1,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3+1,c)
          	    +COEFF( 0, 1,+1,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3+1,c)
          	    +COEFF( 1, 1,+1,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3+1,c)
          	    +COEFF( 2, 1,+1,c,e,i1,i2,i3)*UU(iu1+2,iu2+1,iu3+1,c)

          	    +COEFF(-2, 2,+1,c,e,i1,i2,i3)*UU(iu1-2,iu2+2,iu3+1,c)
          	    +COEFF(-1, 2,+1,c,e,i1,i2,i3)*UU(iu1-1,iu2+2,iu3+1,c)
          	    +COEFF( 0, 2,+1,c,e,i1,i2,i3)*UU(iu1  ,iu2+2,iu3+1,c)
          	    +COEFF( 1, 2,+1,c,e,i1,i2,i3)*UU(iu1+1,iu2+2,iu3+1,c)
          	    +COEFF( 2, 2,+1,c,e,i1,i2,i3)*UU(iu1+2,iu2+2,iu3+1,c)

          	    +COEFF(-2,-2,+2,c,e,i1,i2,i3)*UU(iu1-2,iu2-2,iu3+2,c)
          	    +COEFF(-1,-2,+2,c,e,i1,i2,i3)*UU(iu1-1,iu2-2,iu3+2,c)
          	    +COEFF( 0,-2,+2,c,e,i1,i2,i3)*UU(iu1  ,iu2-2,iu3+2,c)
          	    +COEFF( 1,-2,+2,c,e,i1,i2,i3)*UU(iu1+1,iu2-2,iu3+2,c)
          	    +COEFF( 2,-2,+2,c,e,i1,i2,i3)*UU(iu1+2,iu2-2,iu3+2,c)

          	    +COEFF(-2,-1,+2,c,e,i1,i2,i3)*UU(iu1-2,iu2-1,iu3+2,c)
          	    +COEFF(-1,-1,+2,c,e,i1,i2,i3)*UU(iu1-1,iu2-1,iu3+2,c)
          	    +COEFF( 0,-1,+2,c,e,i1,i2,i3)*UU(iu1  ,iu2-1,iu3+2,c)
          	    +COEFF( 1,-1,+2,c,e,i1,i2,i3)*UU(iu1+1,iu2-1,iu3+2,c)
          	    +COEFF( 2,-1,+2,c,e,i1,i2,i3)*UU(iu1+2,iu2-1,iu3+2,c)

          	    +COEFF(-2, 0,+2,c,e,i1,i2,i3)*UU(iu1-2,iu2  ,iu3+2,c)
          	    +COEFF(-1, 0,+2,c,e,i1,i2,i3)*UU(iu1-1,iu2  ,iu3+2,c)
          	    +COEFF( 0, 0,+2,c,e,i1,i2,i3)*UU(iu1  ,iu2  ,iu3+2,c)
          	    +COEFF( 1, 0,+2,c,e,i1,i2,i3)*UU(iu1+1,iu2  ,iu3+2,c)
          	    +COEFF( 2, 0,+2,c,e,i1,i2,i3)*UU(iu1+2,iu2  ,iu3+2,c)

          	    +COEFF(-2, 1,+2,c,e,i1,i2,i3)*UU(iu1-2,iu2+1,iu3+2,c)
          	    +COEFF(-1, 1,+2,c,e,i1,i2,i3)*UU(iu1-1,iu2+1,iu3+2,c)
          	    +COEFF( 0, 1,+2,c,e,i1,i2,i3)*UU(iu1  ,iu2+1,iu3+2,c)
          	    +COEFF( 1, 1,+2,c,e,i1,i2,i3)*UU(iu1+1,iu2+1,iu3+2,c)
          	    +COEFF( 2, 1,+2,c,e,i1,i2,i3)*UU(iu1+2,iu2+1,iu3+2,c)

          	    +COEFF(-2, 2,+2,c,e,i1,i2,i3)*UU(iu1-2,iu2+2,iu3+2,c)
          	    +COEFF(-1, 2,+2,c,e,i1,i2,i3)*UU(iu1-1,iu2+2,iu3+2,c)
          	    +COEFF( 0, 2,+2,c,e,i1,i2,i3)*UU(iu1  ,iu2+2,iu3+2,c)
          	    +COEFF( 1, 2,+2,c,e,i1,i2,i3)*UU(iu1+1,iu2+2,iu3+2,c)
          	    +COEFF( 2, 2,+2,c,e,i1,i2,i3)*UU(iu1+2,iu2+2,iu3+2,c)


           	     );
        	  }
      	}
      	
            }
        }
    }

    timeForGetResidual+=getCPU()-time;

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

    int i1,i2,i3,I1Bound,I2Bound,I3Bound;

    const real *uup = uu.Array_Descriptor.Array_View_Pointer3;
    const int uuDim0=uu.getRawDataSize(0);
    const int uuDim1=uu.getRawDataSize(1);
    const int uuDim2=uu.getRawDataSize(2);
#define UU(i0,i1,i2,i3) uup[i0+uuDim0*(i1+uuDim1*(i2+uuDim2*(i3)))]
    const real *ffp = ff.Array_Descriptor.Array_View_Pointer3;
    const int ffDim0=ff.getRawDataSize(0);
    const int ffDim1=ff.getRawDataSize(1);
    const int ffDim2=ff.getRawDataSize(2);
#define FF(i0,i1,i2,i3) ffp[i0+ffDim0*(i1+ffDim1*(i2+ffDim2*(i3)))]
    real *resp = res.Array_Descriptor.Array_View_Pointer3;
    const int resDim0=res.getRawDataSize(0);
    const int resDim1=res.getRawDataSize(1);
    const int resDim2=res.getRawDataSize(2);
#define RES(i0,i1,i2,i3) resp[i0+resDim0*(i1+resDim1*(i2+resDim2*(i3)))]
    const real *coeffp = coeff.Array_Descriptor.Array_View_Pointer3;
    const int coeffDim0=coeff.getRawDataSize(0);
    const int coeffDim1=coeff.getRawDataSize(1);
    const int coeffDim2=coeff.getRawDataSize(2);
#define CC(i0,i1,i2,i3) coeffp[i0+coeffDim0*(i1+coeffDim1*(i2+coeffDim2*(i3)))]

    for( int e=0; e<numberOfComponents; e++ )
    {
        FOR_3(i1,i2,i3,I1,I2,I3)
        {
            RES(i1,i2,i3,e)=FF(i1,i2,i3,e);
        }
    // The COEFF macro makes the coeff array look like a 8 dimensional array.
        FOR_3(i1,i2,i3,I1,I2,I3)
        {
            for( int c=0; c<numberOfComponents; c++ )
            {
      	RES(i1,i2,i3,e)-=(    
         	   CC(0+CE(c,e),i1,i2,i3)*UU(i1     ,i2     ,i3     ,c)
        	  +CC(1+CE(c,e),i1,i2,i3)*UU(i1+d1  ,i2+d2  ,i3+d3  ,c)
        	  +CC(2+CE(c,e),i1,i2,i3)*UU(i1+d1*2,i2+d2*2,i3+d3*2,c)
        	  +CC(3+CE(c,e),i1,i2,i3)*UU(i1+d1*3,i2+d2*3,i3+d3*3,c)
        	  +CC(4+CE(c,e),i1,i2,i3)*UU(i1+d1*4,i2+d2*4,i3+d3*4,c)
        	  +CC(5+CE(c,e),i1,i2,i3)*UU(i1+d1*5,i2+d2*5,i3+d3*5,c)
        	  );
            }
        }
        
    }
    return 0;
}



int
getGeneralResidual(MappedGrid & mg, 
                                      const realMappedGridFunction & coeff, 
               		   const realArray & uu,
               		   const realArray & ff,
               		   realArray & res,
               		   const Index & I1,
               		   const Index & I2,
               		   const Index & I3 )
// ============================================================================================
//   Compute the residual in the general case -- he we need to use info fromt the SparseRep
// ============================================================================================
{
    const int numberOfComponents=coeff.sparse->numberOfComponents;
    const int & stencilSize=coeff.sparse->stencilSize;
    const int orderOfAccuracy = coeff.getOperators()->orderOfAccuracy;

    const int & numberOfComponentsForCoefficients = numberOfComponents;
    
    int i1,i2,i3,I1Bound,I2Bound,I3Bound,j1,j2,j3;

    const real *uup = uu.Array_Descriptor.Array_View_Pointer3;
    const int uuDim0=uu.getRawDataSize(0);
    const int uuDim1=uu.getRawDataSize(1);
    const int uuDim2=uu.getRawDataSize(2);
#define UU(i0,i1,i2,i3) uup[i0+uuDim0*(i1+uuDim1*(i2+uuDim2*(i3)))]
    const real *ffp = ff.Array_Descriptor.Array_View_Pointer3;
    const int ffDim0=ff.getRawDataSize(0);
    const int ffDim1=ff.getRawDataSize(1);
    const int ffDim2=ff.getRawDataSize(2);
#define FF(i0,i1,i2,i3) ffp[i0+ffDim0*(i1+ffDim1*(i2+ffDim2*(i3)))]
    real *resp = res.Array_Descriptor.Array_View_Pointer3;
    const int resDim0=res.getRawDataSize(0);
    const int resDim1=res.getRawDataSize(1);
    const int resDim2=res.getRawDataSize(2);
#define RES(i0,i1,i2,i3) resp[i0+resDim0*(i1+resDim1*(i2+resDim2*(i3)))]
    const real *coeffp = coeff.Array_Descriptor.Array_View_Pointer3;
    const int coeffDim0=coeff.getRawDataSize(0);
    const int coeffDim1=coeff.getRawDataSize(1);
    const int coeffDim2=coeff.getRawDataSize(2);
#define CC(i0,i1,i2,i3) coeffp[i0+coeffDim0*(i1+coeffDim1*(i2+coeffDim2*(i3)))]

    const intArray & equationNumber = coeff.sparse->equationNumber;
    const intArray & classify       = coeff.sparse->classify;

  // printf(" >>>>> coeff.sparse->equationOffset=%i \n",coeff.sparse->equationOffset);

    int n;
    for( int e=0; e<numberOfComponents; e++ )
    {
    // The COEFF macro makes the coeff array look like a 8 dimensional array.
        FOR_3(i1,i2,i3,I1,I2,I3)
        {
            if( classify(i1,i2,i3,e)==SparseRepForMGF::extrapolation )
            {
                RES(i1,i2,i3,e)=FF(i1,i2,i3,e);
      	for( int c=0; c<numberOfComponents; c++ )
      	{
        	  for( int m=0; m<stencilSize; m++ )
        	  {
          	    coeff.sparse->equationToIndex( equationNumber(m+CE(c,e),i1,i2,i3)-1, n,j1,j2,j3);

                        if( false )
          	    {
            	      printf(" i=(%i,%i,%i) m=%i CE(c,e)=%i eqnNo=%i (n,j1,j2,j3)=(%i,%i,%i,%i) CC=%8.4f UU=%8.4f\n",
                 		     i1,i2,i3,m,CE(c,e),equationNumber(m+CE(c,e),i1,i2,i3),
                 		     n,j1,j2,j3,CC(m+CE(c,e),i1,i2,i3),UU(j1,j2,j3,n));
          	    }
          	    
          	    RES(i1,i2,i3,e)-=CC(m+CE(c,e),i1,i2,i3)*UU(j1,j2,j3,n);
        	  }
      	}
            }
        }
        
    }

    return 0;
}




#undef UU
#undef RES
#undef FF
#undef COEFF

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
//    uu.reshape(1,uu.dimension(0),uu.dimension(1),uu.dimension(2),uu.dimension(3));
//    ff.reshape(1,ff.dimension(0),ff.dimension(1),ff.dimension(2),ff.dimension(3));
//    res.reshape(1,res.dimension(0),res.dimension(1),res.dimension(2),res.dimension(3));
            
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
//    uu.reshape(uu.dimension(1),uu.dimension(2),uu.dimension(3),uu.dimension(4));
//    ff.reshape(ff.dimension(1),ff.dimension(2),ff.dimension(3),ff.dimension(4));
//    res.reshape(res.dimension(1),res.dimension(2),res.dimension(3),res.dimension(4));
    return 0;
}


int
ghostResidual(realMappedGridFunction & coeff, 
            	      realMappedGridFunction & u,
            	      realMappedGridFunction & f,
            	      realMappedGridFunction & residual,
                            real & resMax,
                            int maskOption = 0  )
// ==================================================================================
// /Description:
//   Compute the residual at all ghost points (including corners) for a coefficient matrix, res = f - coeff * u.
// /coeff (input) : coefficient matrix.
// /u, f (input)  : solution and right hand side.
// /residual (output) : residual.
// /resMax (output) : maximum residual at ghost points
// /maskOption (input) : maskOption=0 : compute residual at all ghost points.
//                       maskOption=1 : only compute residual at ghost points where the boundary mask!=0 
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
//    uu.reshape(1,uu.dimension(0),uu.dimension(1),uu.dimension(2),uu.dimension(3));
//    ff.reshape(1,ff.dimension(0),ff.dimension(1),ff.dimension(2),ff.dimension(3));
//    res.reshape(1,res.dimension(0),res.dimension(1),res.dimension(2),res.dimension(3));
            
    MappedGrid & mg = *coeff.getMappedGrid();
    const IntegerArray & dimension = mg.dimension();
    const IntegerArray & gridIndexRange = mg.gridIndexRange();
    const intArray & mask = mg.mask();
    
    resMax=0.;
    res=0.;

    int side,axis;
    Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
    for( axis=0; axis<mg.numberOfDimensions(); axis++ ) 
    {
        for( side=0; side<=1; side++ )
        {
            if( mg.boundaryCondition(side,axis)>0 )
            {
                getIndex(dimension,Ig1,Ig2,Ig3);
                Igv[axis] = side==0 ? Range(dimension(0,axis),gridIndexRange(0,axis)-1) :
                            	                      Range(gridIndexRange(1,axis)+1,dimension(1,axis));
      	
                getGeneralResidual(mg,coeff,uu,ff,res,Ig1,Ig2,Ig3);
                  
      	if( maskOption==0 )
      	{
        	  resMax=max(resMax,max(fabs(res(Ig1,Ig2,Ig3))));
      	}
      	else
      	{
        	  getBoundaryIndex(dimension,side,axis,Ib1,Ib2,Ib3);
                    Ibv[axis]=gridIndexRange(side,axis);
      	
                    J1=Ig1; J2=Ig2; J3=Ig3; 
                    where( mask(Ib1,Ib2,Ib3)!=0 )
        	  {
                        for( int g=Igv[axis].getBase(); g<Igv[axis].getBound(); g++ )
          	    {
            	      Jv[axis]=g;
            	      resMax=max(resMax,max(fabs(res(J1,J2,J3))));
          	    }
        	  }
      	}

            }
        }
    }
  // display(res,"res after ghostResidual","%6.4f ");  

    return 0;
}



real
getBoundaryError(MappedGrid & mg,
             		 const realArray & res)
{
    const int numberOfComponents=res.getLength(3);
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
                            const int extra=0,
                            const int nc = -1 )
// /nc (input): if >0 then check this many components
{
    const int numberOfComponents= nc==-1 ? res.getLength(3) : nc;
    real error=0.;
    int side,axis;
    Index Ig1,Ig2,Ig3;
    Index Ib1,Ib2,Ib3;
    ForBoundary(side,axis)
    {
        if( mg.boundaryCondition(side,axis) > 0 )
        {
            const int ghost=1;
            getGhostIndex(mg.indexRange(),side,axis,Ig1,Ig2,Ig3,ghost,extra); // *wdh* 061010 -- added ghost line=1 !
            getBoundaryIndex(mg.indexRange(),side,axis,Ib1,Ib2,Ib3,extra);
//       for( int c=0; c<numberOfComponents; c++ )
//         ::display(res(Ig1,Ig2,Ig3,c),"getGhostError: res(Ig1,Ig2,Ig3,c)","%8.1e ");
//       ::display(mg.mask()(Ig1,Ig2,Ig3),"getGhostError: mask","%i ");
            
            where( mg.mask()(Ib1,Ib2,Ib3)>0 ) // check the mask on the boundary
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

    real time0=getCPU();

    int debug=3;

    aString checkFileName;
    if( REAL_EPSILON == DBL_EPSILON )
        checkFileName="tbcc.dp.check.new";  // double precision
    else  
        checkFileName="tbcc.sp.check.new";

    Checker checker(checkFileName);  // for saving a check file.

    bool checkAll=true;  // check all derivatives -- for debugging , change code below to check one derivative

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
            else if( arg=="-checkOne" )
                checkAll=false; 
            else
            {
      	numberOfGridsToTest=1;
      	gridName[0]=argv[1];
            }
        }
    }
    else
    {
        printF("Usage: `tbcc [<gridName>] [-noTiming][-checkOne]' \n");
    }
    
  // make some shorter names for readability
    BCTypes::BCNames 
                                      dirichlet                  = BCTypes::dirichlet,
                                      neumann                    = BCTypes::neumann,
                                      mixed                      = BCTypes::mixed,
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
    
    real cutOff = REAL_EPSILON == DBL_EPSILON ? 1.e-11 : 6.e-4;
    checker.setCutOff(cutOff);
      
    printf(" **** tbcc: Test the Coefficient Matrix Operators and Boundary conditions ***** \n");

    aString label,buff;
    
    for( int it=0; it<numberOfGridsToTest; it++ )
    {
        aString nameOfOGFile=gridName[it];
        checker.setLabel(nameOfOGFile,0);

        cout << "\n *****************************************************************\n";
        cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
        cout << " *****************************************************************\n\n";



        CompositeGrid cg;
        getFromADataBase(cg,nameOfOGFile);

    // cg[0].mapping().getMapping().setMappingCoordinateSystem( Mapping::general );   // ****************

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
            checker.setLabel(mg.getName(),1);

            cout << "\n+++++++Checking component grid = " << grid 
                      << " (" << mg.getName() << ") +++++++" << endl;

            mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
            mg.update(MappedGrid::THEcenterBoundaryNormal | MappedGrid::THEcenterBoundaryTangent);

      // for derivativeScalarDerivative : **** fix this in the MGOP's
            mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEcenterJacobian );

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
      	checker.setLabel(sPrintF(buff,"order=%i",orderOfAccuracy),2);
      	checker.setLabel("std",3);


      	
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
                f.setOperators(op);
      	
      	BoundaryConditionParameters bcParams;


      	if( orderOfAccuracy==2 || orderOfAccuracy==4 )
      	{
	  // ***** check boundary conditions at edges and corners : determined by finishBoundaryConditions
        	  const int numberOfCornerBC=4 + int(orderOfAccuracy==4); 
        	  const int cornerBC[]={ BoundaryConditionParameters::extrapolateCorner,
                         				 BoundaryConditionParameters::evenSymmetryCorner,
                         				 BoundaryConditionParameters::oddSymmetryCorner,
                         				 BoundaryConditionParameters::taylor2ndOrderEvenCorner,
                         				 BoundaryConditionParameters::taylor4thOrderEvenCorner};  //
        	  aString cornerBCName[]={"(extrapolate)       ",
                          				  "(evenSymmetry)      ",
                          				  "(oddSymmetry)       ", 
                          				  "(taylor2ndOrderEven)",
                          				  "(taylor4thOrderEven)"}; //

                    if( checkAll )
        	  {
          	    for( int ibc=0; ibc<numberOfCornerBC; ibc++ )
          	    {
            	      bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::
                                      						  CornerBoundaryConditionEnum(cornerBC[ibc]));
            	      coeff=0.;

            	      getIndex(mg.dimension(),I1,I2,I3);
                        
            	      time=CPU();
            	      coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
            	      coeff.finishBoundaryConditions(bcParams);
            	      time=CPU()-time;
            
	      // use "direct" BC's on u to compare to the BC's put in the coeff matrix
            	      u=exact(mg,I1,I2,I3,0);
            	      u.applyBoundaryCondition(0,BCTypes::extrapolate,allBoundaries,0);
            	      u.finishBoundaryConditions(bcParams);

            	      const intArray & equationNumber = coeff.sparse->equationNumber;
	      // display(equationNumber,"equationNumber","%3i");

            	      f=0.;  // no RHS in this case
                            int maskOption=1; // only compute residual at ghost points where the boundary mask!=0
            	      ghostResidual(coeff,u,f,res,error,maskOption);
            	      checker.printMessage(sPrintF(buff,"finishBC %s",(const char*)cornerBCName[ibc]), 
                           				   error, time );
	      // display(u,"u after finishBoundaryConditions","%6.2f ");
          	    
          	    }
          	    bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::extrapolateCorner);
          	    
        	  }
        	  
      	}



      // ****************************************************************
      //       Test x,y,xx,xy,...,laplacian,...
      // ****************************************************************
      	where( mg.boundaryCondition() > 0 )
        	  mg.boundaryCondition()(all,all)=100;

      	for( int c=0; c<=1; c++ ) // ***** conservative or non-conservative
      	{
                    if( c==0 && orderOfAccuracy==4 )
                        continue;
                    bool conservative= c==0;
        	  
        	  op.useConservativeApproximations(conservative);
        	  if( c==0 )
          	    checker.setLabel("cons",3);
        	  else
          	    checker.setLabel("std",3);

        	  if( checkAll )
          // 	    testCoefficients(identity,identityOperator);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.identityCoefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.identity();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in identity                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("identity",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::identityOperator,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in identity (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("identity (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "identity" eq "laplacian"
                    #endif
                    }

                    if( checkAll )
            // testCoefficients(laplacian,laplacianOperator);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.laplacianCoefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.laplacian();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in laplacian                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("laplacian",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::laplacianOperator,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in laplacian (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("laplacian (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "laplacian" eq "laplacian"
                            {
                                coeff=0.;
                                time=CPU();
                                setCoefficients(MappedGridOperators::laplacianOperator,coeff,&scalar,orderOfAccuracy,conservative);
                                time=CPU()-time;
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
                                checker.printMessage("laplacian (mac)",  error, time );
                            }
                        #endif
                        }

        	  if( checkAll ) 
            // testCoefficients(x,xDerivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.xCoefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.x();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in x                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("x",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::xDerivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in x (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("x (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "x" eq "laplacian"
                        #endif
                        }
                    if( checkAll )
            // testCoefficients(xx,xxDerivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.xxCoefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.xx();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in xx                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("xx",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::xxDerivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in xx (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("xx (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "xx" eq "laplacian"
                        #endif
                        }
                    if( checkAll && mg.numberOfDimensions()>1 )
                    {
          // 	    testCoefficients(y,yDerivative);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.yCoefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.y();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in y                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("y",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::yDerivative,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in y (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("y (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "y" eq "laplacian"
                    #endif
                    }
            // testCoefficients(xy,xyDerivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.xyCoefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.xy();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in xy                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("xy",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::xyDerivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in xy (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("xy (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "xy" eq "laplacian"
                        #endif
                        }
            // testCoefficients(yy,yyDerivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.yyCoefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.yy();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in yy                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("yy",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::yyDerivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in yy (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("yy (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "yy" eq "laplacian"
                        #endif
                        }
        	  }
                    if( checkAll && mg.numberOfDimensions()>2 )
                    {
          // 	    testCoefficients(z,zDerivative);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.zCoefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.z();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in z                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("z",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::zDerivative,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in z (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("z (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "z" eq "laplacian"
                    #endif
                    }
          // 	    testCoefficients(xz,xzDerivative);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.xzCoefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.xz();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in xz                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("xz",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::xzDerivative,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in xz (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("xz (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "xz" eq "laplacian"
                    #endif
                    }
          // 	    testCoefficients(yz,yzDerivative);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.yzCoefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.yz();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in yz                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("yz",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::yzDerivative,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in yz (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("yz (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "yz" eq "laplacian"
                    #endif
                    }
          // 	    testCoefficients(zz,zzDerivative);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.zzCoefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.zz();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in zz                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("zz",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::zzDerivative,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in zz (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("zz (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "zz" eq "laplacian"
                    #endif
                    }
          	    
        	  }
        	  if( checkAll )
        	  {
          // 	    testCoefficients(r1,r1Derivative);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.r1Coefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.r1();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in r1                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("r1",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::r1Derivative,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in r1 (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r1 (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "r1" eq "laplacian"
                    #endif
                    }
          // 	    testCoefficients(r1r1,r1r1Derivative);  // this is a macro
                    {
                        getIndex(mg.dimension(),I1,I2,I3);
                        time=CPU();
                        coeff=op.r1r1Coefficients();
                        time=CPU()-time;
          // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                        u=exact(mg,I1,I2,I3,0);
                        f=u.r1r1();
                        residual(coeff,u,f,res);
                        getIndex(mg.indexRange(),I1,I2,I3);
                        where( mask(I1,I2,I3)>0 )
                            error=max(abs(res(I1,I2,I3)));
                        worstError=max(worstError,error);  
          // label="Maximum error in r1r1                                               ";
          // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                        checker.printMessage("r1r1",  error, time );
          // display(res,"laplacianCoefficients: res","%6.1e ");
          // display(f,"laplacianCoefficients: f","%6.1e ");
                        if( true && c==1 )
                        {
              // *** efficient version ****
                            coeff=0.;
                            time=CPU();
                            op.coefficients(MappedGridOperators::r1r1Derivative,coeff);
                            time=CPU()-time;
              // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);
          //  label="Maximum error in r1r1 (optimized version)                            ";
          //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r1r1 (opt)",  error, time );
                        }  
                        int conservative = 0; // *** c==0 ? 1 : 0;
                    #if 0 
          // test new coeff routines
          //           #If "r1r1" eq "laplacian"
                    #endif
                    }
          	    if( mg.numberOfDimensions()>1 )
          	    {
            // 	      testCoefficients(r2,r2Derivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.r2Coefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.r2();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in r2                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r2",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::r2Derivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in r2 (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("r2 (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "r2" eq "laplacian"
                        #endif
                        }
            // 	      testCoefficients(r1r2,r1r2Derivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.r1r2Coefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.r1r2();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in r1r2                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r1r2",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::r1r2Derivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in r1r2 (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("r1r2 (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "r1r2" eq "laplacian"
                        #endif
                        }
            // 	      testCoefficients(r2r2,r2r2Derivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.r2r2Coefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.r2r2();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in r2r2                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r2r2",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::r2r2Derivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in r2r2 (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("r2r2 (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "r2r2" eq "laplacian"
                        #endif
                        }
          	    }
          	    if( mg.numberOfDimensions()>2 )
          	    {
            // 	      testCoefficients(r3,r3Derivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.r3Coefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.r3();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in r3                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r3",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::r3Derivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in r3 (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("r3 (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "r3" eq "laplacian"
                        #endif
                        }
            // 	      testCoefficients(r1r3,r1r3Derivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.r1r3Coefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.r1r3();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in r1r3                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r1r3",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::r1r3Derivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in r1r3 (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("r1r3 (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "r1r3" eq "laplacian"
                        #endif
                        }
            // 	      testCoefficients(r2r3,r2r3Derivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.r2r3Coefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.r2r3();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in r2r3                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r2r3",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::r2r3Derivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in r2r3 (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("r2r3 (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "r2r3" eq "laplacian"
                        #endif
                        }
            // 	      testCoefficients(r3r3,r3r3Derivative);  // this is a macro
                        {
                            getIndex(mg.dimension(),I1,I2,I3);
                            time=CPU();
                            coeff=op.r3r3Coefficients();
                            time=CPU()-time;
            // display(coeff,"laplacianCoefficients: coeff","%6.1e ");
                            u=exact(mg,I1,I2,I3,0);
                            f=u.r3r3();
                            residual(coeff,u,f,res);
                            getIndex(mg.indexRange(),I1,I2,I3);
                            where( mask(I1,I2,I3)>0 )
                                error=max(abs(res(I1,I2,I3)));
                            worstError=max(worstError,error);  
            // label="Maximum error in r3r3                                               ";
            // printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                            checker.printMessage("r3r3",  error, time );
            // display(res,"laplacianCoefficients: res","%6.1e ");
            // display(f,"laplacianCoefficients: f","%6.1e ");
                            if( true && c==1 )
                            {
                // *** efficient version ****
                                coeff=0.;
                                time=CPU();
                                op.coefficients(MappedGridOperators::r3r3Derivative,coeff);
                                time=CPU()-time;
                // display(coeff,"op.coefficients(MappedGridOperators::laplacianOperator,coeff)","%6.1e ");
                                residual(coeff,u,f,res);
                                getIndex(mg.indexRange(),I1,I2,I3);
                                where( mask(I1,I2,I3)>0 )
                                    error=max(abs(res(I1,I2,I3)));
                                worstError=max(worstError,error);
            //  label="Maximum error in r3r3 (optimized version)                            ";
            //  printf("%s = %e, cpu=%e \n",(const char*)label(0,50),error,time);  
                                checker.printMessage("r3r3 (opt)",  error, time );
                            }  
                            int conservative = 0; // *** c==0 ? 1 : 0;
                        #if 0 
            // test new coeff routines
            //             #If "r3r3" eq "laplacian"
                        #endif
                        }
          	    
          	    }
        	  }
        	  

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

        	  scalar=1.;  // ************************************

        	  aString averageType;
                    int na = conservative ? 1 : 0; // averaging only applies to conservative
        	  for( int a=0; a<=na; a++ )
        	  {
          	    if( a==0 )
          	    {
            	      op.setAveragingType(MappedGridOperators::arithmeticAverage);
                            if( conservative )
                    	        averageType="arith";
                            else
                                averageType=" ";
	      // printf("  --Using arithmetic average\n");
          	    }
          	    else
          	    {
            	      op.setAveragingType(MappedGridOperators::harmonicAverage);
            	      averageType="harmonic";
	      // printf("  --Using harmonic average\n");
          	    }
          	    getIndex(mg.dimension(),I1,I2,I3);

                        if( checkAll )
          	    {
            	      coeff=0.;
            	      time=CPU();
	      //  printf(" tbcc: call op.divScalarGradCoefficients(scalar):\n");
          	    
            	      const realMappedGridFunction & uX = op.divScalarGradCoefficients(scalar);
	      // const realMappedGridFunction & uX = op.laplacianCoefficients();
	      // uX=op.divScalarGradCoefficients(scalar);
//     printf(" tbcc: op.divScalarGradCoefficients(scalar):  [%i,%i][%i,%i][%i,%i][%i,%i]\n",
// 	   uX.getBase(0),uX.getBound(0),
// 	   uX.getBase(1),uX.getBound(1),
// 	   uX.getBase(2),uX.getBound(2),
// 	   uX.getBase(3),uX.getBound(3));

            	      coeff=op.divScalarGradCoefficients(scalar);

            	      time=CPU()-time;
            	      u=exact(mg,I1,I2,I3,0);

            	      f=u.divScalarGrad(scalar);
            
            	      residual(coeff,u,f,res);
            	      getIndex(mg.indexRange(),I1,I2,I3);
            	      where( mask(I1,I2,I3)>0 )
            		error=max(abs(res(I1,I2,I3)));
            	      worstError=max(worstError,error);
            	      checker.printMessage(sPrintF(buff,"divScalarGrad %s",(const char*)averageType), error, time );
          	    }
          	    
          	    if( checkAll )
          	    {
	      // optimised version
            	      coeff=0.;
            	      time=CPU();
            	      op.coefficients(MappedGridOperators::divergenceScalarGradient,coeff,scalar);
            	      time=CPU()-time;
            	      residual(coeff,u,f,res);
            	      getIndex(mg.indexRange(),I1,I2,I3);
            	      where( mask(I1,I2,I3)>0 )
            		error=max(abs(res(I1,I2,I3)));
            	      worstError=max(worstError,error);
            	      checker.printMessage(sPrintF(buff,"divScalarGrad %s (opt)",(const char*)averageType), error, time );
          	    }
          	    

            // **************** DXSDX **************** 
          	    if( (true && !conservative) || checkAll )
          	    {
            	      int dir1, dir2;
            	      for( dir1=0; dir1<mg.numberOfDimensions(); dir1++ )
            	      {
            		for( dir2=0; dir2<mg.numberOfDimensions(); dir2++ )
            		{
              		  MappedGridOperators::derivativeTypes derivType;
              		  if( dir1==0 && dir2==0 )
                		    derivType= MappedGridOperators::xDerivativeScalarXDerivative;
              		  else if( dir1==0 && dir2==1 )
                		    derivType= MappedGridOperators::xDerivativeScalarYDerivative;
              		  else if( dir1==1 && dir2==1 )
                		    derivType= MappedGridOperators::yDerivativeScalarYDerivative;
              		  else if( dir1==1 && dir2==0 )
                		    derivType= MappedGridOperators::yDerivativeScalarXDerivative;
              		  else if( dir1==0 && dir2==2 )
                		    derivType= MappedGridOperators::xDerivativeScalarZDerivative;
              		  else if( dir1==1 && dir2==2 )
                		    derivType= MappedGridOperators::yDerivativeScalarZDerivative;
              		  else if( dir1==2 && dir2==2 )
                		    derivType= MappedGridOperators::zDerivativeScalarZDerivative;
              		  else if( dir1==2 && dir2==0 )
                		    derivType= MappedGridOperators::zDerivativeScalarXDerivative;
              		  else if( dir1==2 && dir2==1 )
                		    derivType= MappedGridOperators::zDerivativeScalarYDerivative;



              		  getIndex(mg.dimension(),I1,I2,I3);
              		  time=CPU();
              		  coeff=op.derivativeScalarDerivativeCoefficients(scalar,dir1,dir2);
              		  time=CPU()-time;
              		  if( false && dir1==0 && dir2==0 && a==1 )
                		    display(coeff,"op.derivativeScalarDerivativeCoefficients","%6.1e ");

              		  u=exact(mg,I1,I2,I3,0);
              		  f=u.derivativeScalarDerivative(scalar,dir1,dir2);
            
                                    realArray f2(I1,I2,I3);
                                    op.derivative(derivType,u,scalar,f2); // optimised way

              		  residual(coeff,u,f,res);
              		  getIndex(mg.indexRange(),I1,I2,I3);

                                    real fDiff;
                                    where( mask(I1,I2,I3)>0 )
              		  {
                		    fDiff=max(fabs(f(I1,I2,I3)-f2(I1,I2,I3)));
              		  }
              		  printf(" ++++ derivScalarDeriv: diff(f) = %9.2e\n",fDiff);
              		  

              		  where( mask(I1,I2,I3)>0 )
                		    error=max(abs(res(I1,I2,I3)));
              		  worstError=max(worstError,error);
              		  checker.printMessage(sPrintF(buff,"Dx%i(sDx%i) %s",dir1,dir2,(const char*)averageType), 
                               				       error, time );

              		  if( conservative )
              		  {
		    // *** efficient version, conservative only  ****

                		    coeff=0.;
                		    time=CPU();
                		    op.coefficients(derivType,coeff,scalar);
                		    time=CPU()-time;

                		    if( false && dir1==0 && dir2==0 && a==1 )
                		    {
                  		      display(f,"**** f=u.derivativeScalarDerivative(scalar,dir1,dir2) ***","%6.1e");
                  		      display(coeff,"op.coefficients(xDerivativeScalarXDerivative,coeff,scalar)","%6.1e ");
                		    }
                		    residual(coeff,u,f,res);
                		    getIndex(mg.indexRange(),I1,I2,I3);
                		    where( mask(I1,I2,I3)>0 )
                  		      error=max(abs(res(I1,I2,I3)));
                		    worstError=max(worstError,error);
                		    label="Maximum error in  Dx(sDx) (efficient version)                            ";
                		    checker.printMessage(sPrintF(buff,"Dx%i(sDx%i) opt, %s",dir1,dir2,(const char*)averageType), 
                               					 error, time );
              		  }
            		
            		
            		}
            	      }

          	    } // end for a
        	  }
        	  
                    op.setAveragingType(MappedGridOperators::arithmeticAverage);
      	
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
            	      
      	if( checkAll )
      	{
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
	  // printf("Maximum error in dirichlet                         = %e, cpu=%e \n",error,time);  
        	  checker.printMessage("dirichlet", error, time );
      	}
      	

        // ****************************************************************
        //       neumann
        // ****************************************************************
                int side,axis; 
      	
      	if( checkAll )
      	{
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
	  // printf("Maximum error in neumann                           = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("neumann", error, time );
        	  
      	}
      	
        // ****************************************************************
        //       mixed - variable coefficients
        // ****************************************************************
      	if( checkAll )
      	{
        	  where( mg.boundaryCondition() > 0 )
          	    mg.boundaryCondition()(all,all)=NEUMANN;
        	  time=CPU();

        	  bcParams.setVariableCoefficientOption(  BoundaryConditionParameters::spatiallyVaryingCoefficients );

        	  getIndex(mg.gridIndexRange(),I1,I2,I3);
	  // varCoeff only needs to be allocated on the boundary but do this so we can assign all boundaries:
        	  RealArray varCoeff(I1,I2,I3,2);  // holds variable coefficients
        	  bcParams.setVariableCoefficientsArray( &varCoeff );        

        	  OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);
        	  varCoeff(I1,I2,I3,0)=1.+ .1*x(I1,I2,I3,0) - .1*x(I1,I2,I3,1);
        	  varCoeff(I1,I2,I3,1)=2. + .1*SQR(x(I1,I2,I3,0)) + .05*SQR(x(I1,I2,I3,1));  // this value must not be zero


        	  coeff=op.identityCoefficients();
        	  coeff.applyBoundaryConditionCoefficients(0,0,mixed,allBoundaries,bcParams);
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
            		f(Ig1,Ig2,Ig3)=varCoeff(Ib1,Ib2,Ib3,1)*(
              		  normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
              		  +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0) )
                                + varCoeff(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0);
            	      else
            		f(Ig1,Ig2,Ig3)=varCoeff(Ib1,Ib2,Ib3,1)*(
              		  normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
              		  +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
              		  +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0) )
                                  + varCoeff(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0) ;
          	    }
        	  }

        	  res=123456789.;
        	  residual(coeff,u,f,res);
	  // display(res,"res after neumann");

        	  error=getGhostError(mg,res);
        	  worstError=max(worstError,error);
	  // display(coeff,"coeff");
	  // printf("Maximum error in neumann                           = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("mixed (var-coeff)", error, time );
        	  
	  // reset:
        	  bcParams.setVariableCoefficientsArray( NULL ); 
        	  bcParams.setVariableCoefficientOption( BoundaryConditionParameters::spatiallyConstantCoefficients );

      	}
      	

	// ****************************************************************
	//       normalDotScalarGrad
	// ****************************************************************

      	if( checkAll )
      	{
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
	  // printf("Maximum error in normalDotScalarGrad               = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("normalDotScalarGrad", error, time );
        	  
      	}
      	

      // ****************************************************************
      //    generalMixedDerivative
      // ****************************************************************
      	bcParams.a.redim(4);
      	bcParams.a(0)=1.;
      	bcParams.a(1)=2.;
      	bcParams.a(2)=3.;
      	bcParams.a(3)=4.;
      	const RealArray & a = bcParams.a;

      	if( checkAll )
      	{

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
	  // printf("Maximum error in generalMixedDerivative            = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("generalMixedDerivative", error, time );

        	  
      	}
      	


	// +++++++++ Now test BC's that apply to a system of equations ++++++++++++++
      	if( checkAll )
      	{
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
	  // printf("Maximum error in normalComponent                   = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("normalComponent", error, time );

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
	  // printf("Maximum error in tangentialComponent               = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("tangentialComponent", error, time );


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
        	  error=getGhostError(mg,res,0,1); // extra=0, nc=1 -- only check error in first equation
        	  worstError=max(worstError,error);
	  // printf("Maximum error in generalizedDivergence             = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("generalizedDivergence", error, time );

        	  if( false )
        	  {
          	    ForBoundary(side,axis)
          	    {
            	      if( mg.boundaryCondition(side,axis) > 0 )
            	      {
            		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
            		getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
            		int d=0;
            		::display(f(Ig1,Ig2,Ig3,d),sPrintF("genDiv: f(Ig1,Ig2,Ig3,%i)",d),"%3.1f ");
            		::display(res(Ig1,Ig2,Ig3,d),sPrintF("genDiv: res(Ig1,Ig2,Ig3,%i)",d),"%8.1e ");
            	      }
          	    }
        	  }
	  // Overture::abort("done");

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
	  // printf("Maximum error in normalDerivativeOf[Normal/Tangent]= %e, cpu=%e \n",error,time); 
        	  checker.printMessage("normalDerivativeOf[Normal/Tangent]", error, time );


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
          	    if( mg.boundaryCondition(side,axis) > 0 )
          	    {
            	      const realArray & normal  = mg.centerBoundaryNormal(side,axis);
            	      const realArray & tangent = mg.centerBoundaryTangent(side,axis);
            	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
            	      getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
            	      getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line in
            	      if( false )
            	      {
		// old way
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
            	      else
            	      {
		// new way
                // equation d:  u_d(-1) + (n.u)(1) n_d - (t^1.u)(1) t^1_d - (t^2.u)(1) t^2_d = 0 
                                realArray nDotU(Ib1,Ib2,Ib3);
                                realArray tau1DotU(Ib1,Ib2,Ib3);
            		realArray up(Ip1,Ip2,Ip3,mg.numberOfDimensions());
            		up = exact(mg,Ip1,Ip2,Ip3,Range(mg.numberOfDimensions())); // exact solution on first line in

		// ::display(tangent,"tangent");
            		
            		if( mg.numberOfDimensions()==2 )
            		{
                                    nDotU=(normal(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,0)+
                                                  normal(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,1));
                                    tau1DotU=(tangent(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,0)+
                                                        tangent(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,1));
                                    for( int d=0; d<mg.numberOfDimensions(); d++ )
              		  {
                		    f(Ig1,Ig2,Ig3,d)=(exact(mg,Ig1,Ig2,Ig3,d)+ 
                              				      nDotU*normal(Ib1,Ib2,Ib3,d) -
                              				      tau1DotU*tangent(Ib1,Ib2,Ib3,d));
              		  }
            		}
            		if( mg.numberOfDimensions()==3 )
            		{
              		  realArray tau2DotU(Ib1,Ib2,Ib3);
                                    nDotU=(normal(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,0)+
                                                  normal(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,1)+
                                                  normal(Ib1,Ib2,Ib3,2)*up(Ip1,Ip2,Ip3,2));
              		  
                                    tau1DotU=(tangent(Ib1,Ib2,Ib3,0)*up(Ip1,Ip2,Ip3,0)+
                                                        tangent(Ib1,Ib2,Ib3,1)*up(Ip1,Ip2,Ip3,1)+
                                                        tangent(Ib1,Ib2,Ib3,2)*up(Ip1,Ip2,Ip3,2));
              		  
                                    tau2DotU=(tangent(Ib1,Ib2,Ib3,3)*up(Ip1,Ip2,Ip3,0)+
                                                        tangent(Ib1,Ib2,Ib3,4)*up(Ip1,Ip2,Ip3,1)+
                                                        tangent(Ib1,Ib2,Ib3,5)*up(Ip1,Ip2,Ip3,2));

                                    for( int d=0; d<mg.numberOfDimensions(); d++ )
              		  {
                		    f(Ig1,Ig2,Ig3,d)=(exact(mg,Ig1,Ig2,Ig3,d)+ 
                              				      nDotU*normal(Ib1,Ib2,Ib3,d) -
                              				      tau1DotU*tangent(Ib1,Ib2,Ib3,d)- 
                              				      tau2DotU*tangent(Ib1,Ib2,Ib3,3+d));
              		  }
            		}
            	      }
            	      
          	    }
        	  }
        	  res=123456789.;
        	  residual(coeff,u,f,res);
        	  error=getGhostError(mg,res,-1);
        	  if( false )
        	  {
          	    ForBoundary(side,axis)
          	    {
            	      if( mg.boundaryCondition(side,axis) > 0 )
            	      {
            		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
            		getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
            		getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line in
            		for( int d=0; d<mg.numberOfDimensions(); d++ )
            		{
              		  ::display(f(Ig1,Ig2,Ig3,d),sPrintF("vectorSymmetry: f(Ig1,Ig2,Ig3,%i)",d),"%3.1f ");
              		  ::display(res(Ig1,Ig2,Ig3,d),sPrintF("vectorSymmetry: res(Ig1,Ig2,Ig3,%i)",d),"%8.1e ");
            		}
            		
            	      }
          	    }
        	  }

        	  worstError=max(worstError,error);
	  // printf("Maximum error in vectorSymmetry                    = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("vectorSymmetry", error, time );

	  // Overture::abort("done");

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

//  	f=u;
//  	ForBoundary(side,axis)
//  	{
//  	  if( mg.boundaryCondition()(side,axis) > 0 )
//  	  {
//  	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
//  	    getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line in
//  	    f(Ig1,Ig2,Ig3,Rx)=exact(mg,Ig1,Ig2,Ig3,Rx)-exact(mg,Ip1,Ip2,Ip3,Rx);
//  	  }
//  	}
//  	res=123456789.;
	  // *** residual(coeff,u,f,res);
	  // error=getGhostError(mg,res);

        	  u.applyBoundaryCondition(Rx,BCTypes::evenSymmetry,allBoundaries,0);
        	  u.finishBoundaryConditions(); // to be consistent with above
            
        	  f=0.;
        	  ghostResidual(coeff,u,f,res,error);
        	  error=getGhostError(mg,res); // we need to avoid unused points

	  // display(coeff,"coeff");
	  // display(res,"residual");
        
        	  worstError=max(worstError,error);
	  // printf("Maximum error in evenSymmetry                      = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("evenSymmetry", error, time );
        	  u(I1,I2,I3,R)=exact(mg,I1,I2,I3,R);

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
          	    }
        	  }
        	  res=123456789.;
        	  residual(coeff,u,f,res);
	  // display(coeff,"coeff");
	  // display(res,"residual");
        
        	  error=getGhostError(mg,res);
        	  worstError=max(worstError,error);
	  // printf("Maximum error in extrapolate[Normal/Tangential]    = %e, cpu=%e \n",error,time); 
        	  checker.printMessage("extrapolate[Normal/Tangent]", error, time );
        	  
      	}
            
      	
      	}// end loop over order of accuracy

        }  // end loop over component grids




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
        
    real totalTime=getCPU()-time0;
    printf(" >>>> total time for tbcc.............%8.2e \n",totalTime);
    printf(" >>>> time for getResidual............%8.2e   %5.2f %% of total time \n",timeForGetResidual,
                                                                                                                                                  100.*timeForGetResidual/totalTime);
    

    cout << "Program Terminated Normally! \n";
    Overture::finish();          
    return 0;
}
