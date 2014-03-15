#include "MappedGridOperators.h"
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


//------------------------------------------------------------------------------------
// This macro defines the general GET_DERIVATIVE statement for all derivative types
//   derivativeType : which derivative to compute
//   ux  : array to put the results in
//   order: order of accuracy 2 or 4
//   nd   : number of dimensions 2 or 3
// Notes:
//  This macro calls one of the macros UX22, UX23, UX42, UX43 etc,
//  (defined in the include file cgux2a.h or cgux4a.h) to compute a 
//  derivative to first or second order accuracy
//------------------------------------------------------------------------------------
#define GET_DERIVATIVE( derivativeType,ux,order,nd,I1,I2,I3,N )          \
{                                                                        \
     switch( derivativeType )                                            \
     {                                                                   \
      case xDerivative:                                                  \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UX ## order ## nd (I1,I2,I3,n);                  \
       break;                                                            \
      case yDerivative:                                                  \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UY ## order ## nd(I1,I2,I3,n);                   \
       break;                                                            \
      case zDerivative:                                                  \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UZ ## order ## nd(I1,I2,I3,n);                   \
       break;                                                            \
      case xxDerivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UXX ## order ## nd(I1,I2,I3,n);                  \
       break;                                                            \
      case xyDerivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UXY ## order ## nd(I1,I2,I3,n);                  \
       break;                                                            \
      case xzDerivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UXZ ## order ## nd(I1,I2,I3,n);                  \
       break;                                                            \
      case yyDerivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UYY ## order ## nd(I1,I2,I3,n);                  \
       break;                                                            \
      case yzDerivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UYZ ## order ## nd(I1,I2,I3,n);                  \
       break;                                                            \
      case zzDerivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UZZ ## order ## nd(I1,I2,I3,n);                  \
       break;                                                            \
                                                                         \
      case r1Derivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UR ## order (I1,I2,I3,n);                        \
       break;                                                            \
      case r2Derivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=US ## order (I1,I2,I3,n);                        \
       break;                                                            \
      case r3Derivative:                                                 \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UT ## order (I1,I2,I3,n);                        \
       break;                                                            \
                                                                         \
      case r1r1Derivative:                                               \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=URR ## order (I1,I2,I3,n);                       \
       break;                                                            \
      case r1r2Derivative:                                               \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=URS ## order (I1,I2,I3,n);                       \
       break;                                                            \
      case r1r3Derivative:                                               \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=URT ## order (I1,I2,I3,n);                       \
       break;                                                            \
      case r2r2Derivative:                                               \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=USS ## order (I1,I2,I3,n);                       \
       break;                                                            \
      case r2r3Derivative:                                               \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UST ## order (I1,I2,I3,n);                       \
       break;                                                            \
      case r3r3Derivative:                                               \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=UTT ## order (I1,I2,I3,n);                       \
       break;                                                            \
                                                                         \
      case laplacianOperator:                                            \
       for( n=N.getBase(); n<=N.getBound(); n++ )                        \
         ux(I1,I2,I3,n)=LAPLACIAN ## order ## nd(I1,I2,I3,n);           \
       break;                                                            \
      case divergence:                                                   \
       ux(I1,I2,I3)=UX ## order ## nd(I1,I2,I3,0)                       \
                   +UY ## order ## nd(I1,I2,I3,1);                      \
       if( numberOfDimensions==3 )                                       \
         ux(I1,I2,I3)+=UZ ## order ## nd(I1,I2,I3,2);                    \
       break;                                                            \
      case gradient:                                                     \
       ux(I1,I2,I3,0)=UX ## order ## nd(I1,I2,I3,0);                    \
       ux(I1,I2,I3,1)=UY ## order ## nd(I1,I2,I3,0);                    \
       if( numberOfDimensions==3 )                                       \
         ux(I1,I2,I3,2)=UZ ## order ## nd(I1,I2,I3,0);                   \
       break;                                                            \
      default:                                                           \
        cout << "MappedGridDerivatives:ERROR unknown derivative!\n";     \
     }                                                                   \
}



c=======================================================================
c=======================================================================
      subroutine deriv( type,u,ux,ndra,ndrb,ndsa,ndsb,ndta,ndtb,nv,
     &   dim )
      integer xderiv,yderiv
      parameter( xderiv=0,yderiv=1 )
      real u(ndra0:ndrb0,ndsa0:ndsb0,ndta0:ndtb0,nv), 
     &    ux(ndra1:ndrb1,ndsa1:ndsb1,ndta1:ndtb1,nv)
      integer dim(2,3)

      
      if( type.eq.xderiv )then

      else if( type.eq.yderiv )then

      end if

      end
