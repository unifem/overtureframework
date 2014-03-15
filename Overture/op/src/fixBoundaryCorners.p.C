// include "fixBoundaryCorners.proto.h"
#include "GenericMappedGridOperators.h"
#include "SparseRep.h"
#include "GridFunctionParameters.h"

// extern realMappedGridFunction Overture::nullDoubleMappedGridFunction();
// extern realMappedGridFunction Overture::nullFloatMappedGridFunction();
// ifdef OV_USE_DOUBLE
// define NULLRealMappedGridFunction Overture::nullDoubleMappedGridFunction()
// else
// define NULLRealMappedGridFunction Overture::nullFloatMappedGridFunction()
// endif


#define UX1(n1,n2,n3,i1,i2,i3,n)              /*  */ \
          u(i1+  (n1),i2+  (n2),i3+  (n3),n)

#define UX2(n1,n2,n3,i1,i2,i3,n)              /*  */ \
     + 2.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     -    u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n) 


#define UX3(n1,n2,n3,i1,i2,i3,n)              /*  */ \
     + 3.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     - 3.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +    u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)

#define UX4(n1,n2,n3,i1,i2,i3,n)              /*  */ \
     + 4.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     - 6.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     + 4.*u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)  \
     -    u(i1+4*(n1),i2+4*(n2),i3+4*(n3),n)

#define UX5(n1,n2,n3,i1,i2,i3,n)              /*  */ \
     + 5.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     -10.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +10.*u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)  \
     - 5.*u(i1+4*(n1),i2+4*(n2),i3+4*(n3),n)  \
     +    u(i1+5*(n1),i2+5*(n2),i3+5*(n3),n)

#define UX6(n1,n2,n3,i1,i2,i3,n)              /*  */ \
     + 6.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     -15.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +20.*u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)  \
     -15.*u(i1+4*(n1),i2+4*(n2),i3+4*(n3),n)  \
     + 6.*u(i1+5*(n1),i2+5*(n2),i3+5*(n3),n)  \
     -    u(i1+6*(n1),i2+6*(n2),i3+6*(n3),n)


// this macro is used in fixBoundaryCorners to extrapolate to different orders **** not used anymore ****
#define EXTRAP_SWITCH( i1,i2,i3,n, is1,is2,is3,j1,j2,j3,m )  \
        if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==extrapolateCorner ) \
        {  \
	  switch( orderOfExtrapolation ) \
	  { \
	  case 1: \
            u(i1,i2,i3,n)=UX1(is1,is2,is3,j1,j2,j3,m);  \
            break; \
	  case 2: \
            u(i1,i2,i3,n)=UX2(is1,is2,is3,j1,j2,j3,m);    \
            break; \
	  case 3:  \
            u(i1,i2,i3,n)=UX3(is1,is2,is3,j1,j2,j3,m);   \
            break;  \
	  case 4:  \
            u(i1,i2,i3,n)=UX4(is1,is2,is3,j1,j2,j3,m);  \
            break;  \
	  case 5:  \
            u(i1,i2,i3,n)=UX5(is1,is2,is3,j1,j2,j3,m);   \
            break;  \
	  case 6:  \
            u(i1,i2,i3,n)=UX6(is1,is2,is3,j1,j2,j3,m);   \
            break;  \
	  default:  \
	    cout << "fixBoundaryCorners:Error: unable to extrapolate to orderOfExtrapolation= "   \
		 << bcParameters.orderOfExtrapolation << ", can only do orders 1 to 6" << endl;  \
	  } \
        } \
        else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==symmetryCorner ) \
        { \
	    /* symmetry boundary condition */  \
          u(i1,i2,i3,n)=u(j1+2*(is1),j2+2*(is2),j3+2*(is3),n);   \
        } \
        else \
        { \
	    cout << "fixBoundaryCorners:Error: unknown bcParameters.cornerBoundaryCondition="  \
		 << bcParameters.getCornerBoundaryCondition(side1,side2,side3) << endl;  \
	} 



static int
assignCorners( const Index & i1, const Index & i2, const Index & i3, const Index & n, 
               int is1, int is2,int is3,
               const Index & j1, const Index & j2, const Index & j3, const Index & m,
               int side1, int side2, int side3, int orderOfExtrapolation,
               realArray & u, const BoundaryConditionParameters & bcParameters, int numberOfDimensions )
// ================================================================================================
//  /Description:
//     Apply an extrapolation or symmetry boundary condition.
//  /i1,i2,i3,n: Index;'s of points to assign.
// ===============================================================================================
{
  // printf("assignCorners: orderOfExtrapolation=%i\n",orderOfExtrapolation);
  
  

  if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::extrapolateCorner ) 
  {  
    switch( orderOfExtrapolation ) 
    { 
    case 1: 
      //       u(i1,i2,i3,n)=UX1(is1,is2,is3,j1,j2,j3,m);  /* @PA */  
      //  u(i1,i2,i3,n)=u(j1+(is1),j2+(is2),j3+(is3),m);/*@PA*/
      { // begin scope of preprocessor statements 
      real *up = u.Array_Descriptor.Array_View_Pointer3;
      #define INu(i0,i1_,i2_,i3_) ((i0)+uDim0*((i1_)+uDim1*((i2_)+uDim2*((i3_)))))
      const int uBase0 = u.getBase(0)*u.getRawStride(0), uDim0 = u.getRawDataSize(0);
      const int uBase1 = u.getBase(1)*u.getRawStride(1), uDim1 = u.getRawDataSize(1);
      const int uBase2 = u.getBase(2)*u.getRawStride(2), uDim2 = u.getRawDataSize(2);
      const int uBase3 = u.getBase(3)*u.getRawStride(3);
      #define INut(i0,i1_,i2_,i3_) ((i0)+utDim0*((i1_)+utDim1*((i2_)+utDim2*((i3_)))))
      const int utDim0 = i1.getBound()-i1.getBase()+1;
      const int utDim1 = i2.getBound()-i2.getBase()+1;
      const int utDim2 = i3.getBound()-i3.getBase()+1;
      const int utDim3 = n.getBound()-n.getBase()+1;
      real *ut = new real [utDim0*utDim1*utDim2*utDim3]; // temp array 
      // The primary ranges for the loops are i1, i2, i3, n, 
      const int i1Base = i1.getBase(), i1Stride = i1.getStride();
      const int i2Base = i2.getBase(), i2Stride = i2.getStride();
      const int i3Base = i3.getBase(), i3Stride = i3.getStride();
      const int nBase = n.getBase(), nStride = n.getStride();
      const int j1Base = j1.getBase(), j1Stride = j1.getStride();
      const int j2Base = j2.getBase(), j2Stride = j2.getStride();
      const int j3Base = j3.getBase(), j3Stride = j3.getStride();
      const int mBase = m.getBase(), mStride = m.getStride();
      const int i0Count = i1.getLength(), i0Stride = i1.getStride();
      const int i1_Count = i2.getLength(), i1_Stride = i2.getStride();
      const int i2_Count = i3.getLength(), i2_Stride = i3.getStride();
      const int i3_Count = n.getLength(), i3_Stride = n.getStride();
      const int ui1Stride=u.getRawStride(0)*i1Stride;
      const int ui2Stride=u.getRawStride(1)*i2Stride;
      const int ui3Stride=u.getRawStride(2)*i3Stride;
      const int unStride=u.getRawStride(3)*nStride;
      const int uj1Stride=u.getRawStride(0)*j1Stride;
      const int uj2Stride=u.getRawStride(1)*j2Stride;
      const int uj3Stride=u.getRawStride(2)*j3Stride;
      const int umStride=u.getRawStride(3)*mStride;
      int i0; int i1_; int i2_; int i3_; 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]=up[INu(i0*uj1Stride+j1Base+(is1),i1_*uj2Stride+j2Base+(is2),i2_*uj3Stride+j3Base+(is3),i3_*umStride+mBase)];/*@PA*/
      } 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        up[INu(i0*ui1Stride+i1Base,i1_*ui2Stride+i2Base,i2_*ui3Stride+i3Base,i3_*unStride+nBase)] = ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]; // copy temp array into u 

      } 
      delete [] ut;
      #undef INu
      #undef INut
      } // end scope of preprocessor statements 
      break; 
    case 2: 
      //       u(i1,i2,i3,n)=UX2(is1,is2,is3,j1,j2,j3,m);  /* @PA */   
      //  u(i1,i2,i3,n)=+2.*u(j1+(is1),j2+(is2),j3+(is3),m)-u(j1+2*(is1),j2+2*(is2),j3+2*(is3),m);/*@PA*/
      { // begin scope of preprocessor statements 
      real *up = u.Array_Descriptor.Array_View_Pointer3;
      #define INu(i0,i1_,i2_,i3_) ((i0)+uDim0*((i1_)+uDim1*((i2_)+uDim2*((i3_)))))
      const int uBase0 = u.getBase(0)*u.getRawStride(0), uDim0 = u.getRawDataSize(0);
      const int uBase1 = u.getBase(1)*u.getRawStride(1), uDim1 = u.getRawDataSize(1);
      const int uBase2 = u.getBase(2)*u.getRawStride(2), uDim2 = u.getRawDataSize(2);
      const int uBase3 = u.getBase(3)*u.getRawStride(3);
      #define INut(i0,i1_,i2_,i3_) ((i0)+utDim0*((i1_)+utDim1*((i2_)+utDim2*((i3_)))))
      const int utDim0 = i1.getBound()-i1.getBase()+1;
      const int utDim1 = i2.getBound()-i2.getBase()+1;
      const int utDim2 = i3.getBound()-i3.getBase()+1;
      const int utDim3 = n.getBound()-n.getBase()+1;
      real *ut = new real [utDim0*utDim1*utDim2*utDim3]; // temp array 
      // The primary ranges for the loops are i1, i2, i3, n, 
      const int i1Base = i1.getBase(), i1Stride = i1.getStride();
      const int i2Base = i2.getBase(), i2Stride = i2.getStride();
      const int i3Base = i3.getBase(), i3Stride = i3.getStride();
      const int nBase = n.getBase(), nStride = n.getStride();
      const int j1Base = j1.getBase(), j1Stride = j1.getStride();
      const int j2Base = j2.getBase(), j2Stride = j2.getStride();
      const int j3Base = j3.getBase(), j3Stride = j3.getStride();
      const int mBase = m.getBase(), mStride = m.getStride();
      const int i0Count = i1.getLength(), i0Stride = i1.getStride();
      const int i1_Count = i2.getLength(), i1_Stride = i2.getStride();
      const int i2_Count = i3.getLength(), i2_Stride = i3.getStride();
      const int i3_Count = n.getLength(), i3_Stride = n.getStride();
      const int ui1Stride=u.getRawStride(0)*i1Stride;
      const int ui2Stride=u.getRawStride(1)*i2Stride;
      const int ui3Stride=u.getRawStride(2)*i3Stride;
      const int unStride=u.getRawStride(3)*nStride;
      const int uj1Stride=u.getRawStride(0)*j1Stride;
      const int uj2Stride=u.getRawStride(1)*j2Stride;
      const int uj3Stride=u.getRawStride(2)*j3Stride;
      const int umStride=u.getRawStride(3)*mStride;
      int i0; int i1_; int i2_; int i3_; 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]=+2.*up[INu(i0*uj1Stride+j1Base+(is1),i1_*uj2Stride+j2Base+(is2),i2_*uj3Stride+j3Base+(is3),i3_*umStride+mBase)]-up[INu(i0*uj1Stride+j1Base+2*(is1),i1_*uj2Stride+j2Base+2*(is2),i2_*uj3Stride+j3Base+2*(is3),i3_*umStride+mBase)];/*@PA*/
      } 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        up[INu(i0*ui1Stride+i1Base,i1_*ui2Stride+i2Base,i2_*ui3Stride+i3Base,i3_*unStride+nBase)] = ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]; // copy temp array into u 

      } 
      delete [] ut;
      #undef INu
      #undef INut
      } // end scope of preprocessor statements 
      break; 
    case 3:  
      //       u(i1,i2,i3,n)=UX3(is1,is2,is3,j1,j2,j3,m);  /* @PA */  
      //  u(i1,i2,i3,n)=+3.*u(j1+(is1),j2+(is2),j3+(is3),m)-3.*u(j1+2*(is1),j2+2*(is2),j3+2*(is3),m)+u(j1+3*(is1),j2+3*(is2),j3+3*(is3),m);/*@PA*/
      { // begin scope of preprocessor statements 
      real *up = u.Array_Descriptor.Array_View_Pointer3;
      #define INu(i0,i1_,i2_,i3_) ((i0)+uDim0*((i1_)+uDim1*((i2_)+uDim2*((i3_)))))
      const int uBase0 = u.getBase(0)*u.getRawStride(0), uDim0 = u.getRawDataSize(0);
      const int uBase1 = u.getBase(1)*u.getRawStride(1), uDim1 = u.getRawDataSize(1);
      const int uBase2 = u.getBase(2)*u.getRawStride(2), uDim2 = u.getRawDataSize(2);
      const int uBase3 = u.getBase(3)*u.getRawStride(3);
      #define INut(i0,i1_,i2_,i3_) ((i0)+utDim0*((i1_)+utDim1*((i2_)+utDim2*((i3_)))))
      const int utDim0 = i1.getBound()-i1.getBase()+1;
      const int utDim1 = i2.getBound()-i2.getBase()+1;
      const int utDim2 = i3.getBound()-i3.getBase()+1;
      const int utDim3 = n.getBound()-n.getBase()+1;
      real *ut = new real [utDim0*utDim1*utDim2*utDim3]; // temp array 
      // The primary ranges for the loops are i1, i2, i3, n, 
      const int i1Base = i1.getBase(), i1Stride = i1.getStride();
      const int i2Base = i2.getBase(), i2Stride = i2.getStride();
      const int i3Base = i3.getBase(), i3Stride = i3.getStride();
      const int nBase = n.getBase(), nStride = n.getStride();
      const int j1Base = j1.getBase(), j1Stride = j1.getStride();
      const int j2Base = j2.getBase(), j2Stride = j2.getStride();
      const int j3Base = j3.getBase(), j3Stride = j3.getStride();
      const int mBase = m.getBase(), mStride = m.getStride();
      const int i0Count = i1.getLength(), i0Stride = i1.getStride();
      const int i1_Count = i2.getLength(), i1_Stride = i2.getStride();
      const int i2_Count = i3.getLength(), i2_Stride = i3.getStride();
      const int i3_Count = n.getLength(), i3_Stride = n.getStride();
      const int ui1Stride=u.getRawStride(0)*i1Stride;
      const int ui2Stride=u.getRawStride(1)*i2Stride;
      const int ui3Stride=u.getRawStride(2)*i3Stride;
      const int unStride=u.getRawStride(3)*nStride;
      const int uj1Stride=u.getRawStride(0)*j1Stride;
      const int uj2Stride=u.getRawStride(1)*j2Stride;
      const int uj3Stride=u.getRawStride(2)*j3Stride;
      const int umStride=u.getRawStride(3)*mStride;
      int i0; int i1_; int i2_; int i3_; 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]=+3.*up[INu(i0*uj1Stride+j1Base+(is1),i1_*uj2Stride+j2Base+(is2),i2_*uj3Stride+j3Base+(is3),i3_*umStride+mBase)]-3.*up[INu(i0*uj1Stride+j1Base+2*(is1),i1_*uj2Stride+j2Base+2*(is2),i2_*uj3Stride+j3Base+2*(is3),i3_*umStride+mBase)]+up[INu(i0*uj1Stride+j1Base+3*(is1),i1_*uj2Stride+j2Base+3*(is2),i2_*uj3Stride+j3Base+3*(is3),i3_*umStride+mBase)];/*@PA*/
      } 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        up[INu(i0*ui1Stride+i1Base,i1_*ui2Stride+i2Base,i2_*ui3Stride+i3Base,i3_*unStride+nBase)] = ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]; // copy temp array into u 

      } 
      delete [] ut;
      #undef INu
      #undef INut
      } // end scope of preprocessor statements 
      break;  
    case 4:  
      //       u(i1,i2,i3,n)=UX4(is1,is2,is3,j1,j2,j3,m);  /* @PA */  
      //  u(i1,i2,i3,n)=+4.*u(j1+(is1),j2+(is2),j3+(is3),m)-6.*u(j1+2*(is1),j2+2*(is2),j3+2*(is3),m)+4.*u(j1+3*(is1),j2+3*(is2),j3+3*(is3),m)-u(j1+4*(is1),j2+4*(is2),j3+4*(is3),m);/*@PA*/
      { // begin scope of preprocessor statements 
      real *up = u.Array_Descriptor.Array_View_Pointer3;
      #define INu(i0,i1_,i2_,i3_) ((i0)+uDim0*((i1_)+uDim1*((i2_)+uDim2*((i3_)))))
      const int uBase0 = u.getBase(0)*u.getRawStride(0), uDim0 = u.getRawDataSize(0);
      const int uBase1 = u.getBase(1)*u.getRawStride(1), uDim1 = u.getRawDataSize(1);
      const int uBase2 = u.getBase(2)*u.getRawStride(2), uDim2 = u.getRawDataSize(2);
      const int uBase3 = u.getBase(3)*u.getRawStride(3);
      #define INut(i0,i1_,i2_,i3_) ((i0)+utDim0*((i1_)+utDim1*((i2_)+utDim2*((i3_)))))
      const int utDim0 = i1.getBound()-i1.getBase()+1;
      const int utDim1 = i2.getBound()-i2.getBase()+1;
      const int utDim2 = i3.getBound()-i3.getBase()+1;
      const int utDim3 = n.getBound()-n.getBase()+1;
      real *ut = new real [utDim0*utDim1*utDim2*utDim3]; // temp array 
      // The primary ranges for the loops are i1, i2, i3, n, 
      const int i1Base = i1.getBase(), i1Stride = i1.getStride();
      const int i2Base = i2.getBase(), i2Stride = i2.getStride();
      const int i3Base = i3.getBase(), i3Stride = i3.getStride();
      const int nBase = n.getBase(), nStride = n.getStride();
      const int j1Base = j1.getBase(), j1Stride = j1.getStride();
      const int j2Base = j2.getBase(), j2Stride = j2.getStride();
      const int j3Base = j3.getBase(), j3Stride = j3.getStride();
      const int mBase = m.getBase(), mStride = m.getStride();
      const int i0Count = i1.getLength(), i0Stride = i1.getStride();
      const int i1_Count = i2.getLength(), i1_Stride = i2.getStride();
      const int i2_Count = i3.getLength(), i2_Stride = i3.getStride();
      const int i3_Count = n.getLength(), i3_Stride = n.getStride();
      const int ui1Stride=u.getRawStride(0)*i1Stride;
      const int ui2Stride=u.getRawStride(1)*i2Stride;
      const int ui3Stride=u.getRawStride(2)*i3Stride;
      const int unStride=u.getRawStride(3)*nStride;
      const int uj1Stride=u.getRawStride(0)*j1Stride;
      const int uj2Stride=u.getRawStride(1)*j2Stride;
      const int uj3Stride=u.getRawStride(2)*j3Stride;
      const int umStride=u.getRawStride(3)*mStride;
      int i0; int i1_; int i2_; int i3_; 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]=+4.*up[INu(i0*uj1Stride+j1Base+(is1),i1_*uj2Stride+j2Base+(is2),i2_*uj3Stride+j3Base+(is3),i3_*umStride+mBase)]-6.*up[INu(i0*uj1Stride+j1Base+2*(is1),i1_*uj2Stride+j2Base+2*(is2),i2_*uj3Stride+j3Base+2*(is3),i3_*umStride+mBase)]+4.*up[INu(i0*uj1Stride+j1Base+3*(is1),i1_*uj2Stride+j2Base+3*(is2),i2_*uj3Stride+j3Base+3*(is3),i3_*umStride+mBase)]-up[INu(i0*uj1Stride+j1Base+4*(is1),i1_*uj2Stride+j2Base+4*(is2),i2_*uj3Stride+j3Base+4*(is3),i3_*umStride+mBase)];/*@PA*/
      } 
      for( i3_=0; i3_<i3_Count; i3_++ )
      for( i2_=0; i2_<i2_Count; i2_++ )
      for( i1_=0; i1_<i1_Count; i1_++ )
      for( i0=0; i0<i0Count; i0++ )
      { 
        up[INu(i0*ui1Stride+i1Base,i1_*ui2Stride+i2Base,i2_*ui3Stride+i3Base,i3_*unStride+nBase)] = ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]; // copy temp array into u 

      } 
      delete [] ut;
      #undef INu
      #undef INut
      } // end scope of preprocessor statements 
      break;  
    case 5:  
      u(i1,i2,i3,n)=UX5(is1,is2,is3,j1,j2,j3,m); 
      break;  
    case 6:  
      u(i1,i2,i3,n)=UX6(is1,is2,is3,j1,j2,j3,m); 
      break;  
    default:  
      cout << "fixBoundaryCorners:Error: unable to extrapolate to orderOfExtrapolation= "   
	   << bcParameters.orderOfExtrapolation << ", can only do orders 1 to 6" << endl;  
    } 
  } 
  else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::symmetryCorner ) 
  { 
    /* symmetry boundary condition */  
    //  u(i1,i2,i3,n)=u(j1+2*(is1),j2+2*(is2),j3+2*(is3),n);/*@PA*/
    { // begin scope of preprocessor statements 
    real *up = u.Array_Descriptor.Array_View_Pointer3;
    #define INu(i0,i1_,i2_,i3_) ((i0)+uDim0*((i1_)+uDim1*((i2_)+uDim2*((i3_)))))
    const int uBase0 = u.getBase(0)*u.getRawStride(0), uDim0 = u.getRawDataSize(0);
    const int uBase1 = u.getBase(1)*u.getRawStride(1), uDim1 = u.getRawDataSize(1);
    const int uBase2 = u.getBase(2)*u.getRawStride(2), uDim2 = u.getRawDataSize(2);
    const int uBase3 = u.getBase(3)*u.getRawStride(3);
    #define INut(i0,i1_,i2_,i3_) ((i0)+utDim0*((i1_)+utDim1*((i2_)+utDim2*((i3_)))))
    const int utDim0 = i1.getBound()-i1.getBase()+1;
    const int utDim1 = i2.getBound()-i2.getBase()+1;
    const int utDim2 = i3.getBound()-i3.getBase()+1;
    const int utDim3 = n.getBound()-n.getBase()+1;
    real *ut = new real [utDim0*utDim1*utDim2*utDim3]; // temp array 
    // The primary ranges for the loops are i1, i2, i3, n, 
    const int i1Base = i1.getBase(), i1Stride = i1.getStride();
    const int i2Base = i2.getBase(), i2Stride = i2.getStride();
    const int i3Base = i3.getBase(), i3Stride = i3.getStride();
    const int nBase = n.getBase(), nStride = n.getStride();
    const int j1Base = j1.getBase(), j1Stride = j1.getStride();
    const int j2Base = j2.getBase(), j2Stride = j2.getStride();
    const int j3Base = j3.getBase(), j3Stride = j3.getStride();
    const int i0Count = i1.getLength(), i0Stride = i1.getStride();
    const int i1_Count = i2.getLength(), i1_Stride = i2.getStride();
    const int i2_Count = i3.getLength(), i2_Stride = i3.getStride();
    const int i3_Count = n.getLength(), i3_Stride = n.getStride();
    const int ui1Stride=u.getRawStride(0)*i1Stride;
    const int ui2Stride=u.getRawStride(1)*i2Stride;
    const int ui3Stride=u.getRawStride(2)*i3Stride;
    const int unStride=u.getRawStride(3)*nStride;
    const int uj1Stride=u.getRawStride(0)*j1Stride;
    const int uj2Stride=u.getRawStride(1)*j2Stride;
    const int uj3Stride=u.getRawStride(2)*j3Stride;
    int i0; int i1_; int i2_; int i3_; 
    for( i3_=0; i3_<i3_Count; i3_++ )
    for( i2_=0; i2_<i2_Count; i2_++ )
    for( i1_=0; i1_<i1_Count; i1_++ )
    for( i0=0; i0<i0Count; i0++ )
    { 
      ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]=up[INu(i0*uj1Stride+j1Base+2*(is1),i1_*uj2Stride+j2Base+2*(is2),i2_*uj3Stride+j3Base+2*(is3),i3_*unStride+nBase)];/*@PA*/
    } 
    for( i3_=0; i3_<i3_Count; i3_++ )
    for( i2_=0; i2_<i2_Count; i2_++ )
    for( i1_=0; i1_<i1_Count; i1_++ )
    for( i0=0; i0<i0Count; i0++ )
    { 
      up[INu(i0*ui1Stride+i1Base,i1_*ui2Stride+i2Base,i2_*ui3Stride+i3Base,i3_*unStride+nBase)] = ut[INut(i0*i0Stride,i1_*i1_Stride,i2_*i2_Stride,i3_*i3_Stride)]; // copy temp array into u 

    } 
    delete [] ut;
    #undef INu
    #undef INut
    } // end scope of preprocessor statements 
  } 
  else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::taylor2ndOrder ) 
  { 
    // Using a taylor approximation:
    //  u(+1,+1) = u(0,0) +dr*ur + ds*us + dr^2/2 urr + dr*ds*urs + ds^2/2 uss + ...
    //  u(-1,-1) = u(0,0) -dr*ur - ds*us + dr^2/2 urr + ...
    //  u(-1,-1) = u(1,1) -2dr*ur -2*ds*us + O(dr^3+...)
    //  ur = (u(1,0)-u(-1,0))/(2dr)
    // gives
    //   u(-1,-1) = u(1,1) -( u(1,0)-u(-1,0) ) - (u(0,1)-u(0,-1))
    if( numberOfDimensions==2 )
      u(i1,i2,i3,n)=(u(j1+2*(is1),j2+2*(is2),j3,n)-  
                     u(j1+2*(is1),j2+  (is2),j3,n)+
                     u(j1        ,j2+  (is2),j3,n)-
                     u(j1+  (is1),j2+2*(is2),j3,n)+
		     u(j1+  (is1),j2        ,j3,n));
    else if( numberOfDimensions==3 )
      u(i1,i2,i3,n)=(u(j1+2*(is1),j2+2*(is2),j3+2*(is3),n)-  
                     u(j1+2*(is1),j2+  (is2),j3+  (is3),n)+
                     u(j1        ,j2+  (is2),j3+  (is3),n)-
                     u(j1+  (is1),j2+2*(is2),j3+  (is3),n)+
		     u(j1+  (is1),j2        ,j3+  (is3),n)-
                     u(j1+  (is1),j2+  (is2),j3+2*(is3),n)+  
                     u(j1+  (is1),j2+  (is2),j3        ,n));
  }
  else 
  { 
    cout << "fixBoundaryCorners:Error: unknown bcParameters.cornerBoundaryCondition="  
	 << bcParameters.getCornerBoundaryCondition(side1,side2,side3) << endl;  
  } 
  return 0;
}



void GenericMappedGridOperators::
fixBoundaryCorners(realMappedGridFunction & u,
                   const BoundaryConditionParameters & bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		   const Range & C0 /* =nullRange */ )
//======================================================================
//
// /Description:
// This is a fix-up routine to get the solution
// at corners, including the ghost points outside corners.
//
// /bcParameters.lineToAssign (input) : if zero assign all ghost corner points. If 1 only assign
//    ghost corner points on ghost line 2 or greater. If 2 only assign on ghost corner points on
//   ghost line3 or greater, etc.
//======================================================================
{

  MappedGrid & c = *u.getMappedGrid();
  //     ---Fix periodic edges
  u.periodicUpdate(C0);
  
  const int orderOfExtrapolation = bcParameters.orderOfExtrapolation<0 ? orderOfAccuracy+1
                                                                       : bcParameters.orderOfExtrapolation;

  int indexRange[2][3];
  indexRange[0][0]=c.indexRange(Start,axis1);
  indexRange[1][0]=c.indexRange(End  ,axis1);
  indexRange[0][1]=c.indexRange(Start,axis2);
  indexRange[1][1]=c.indexRange(End  ,axis2);
  indexRange[0][2]=c.indexRange(Start,axis3);
  indexRange[1][2]=c.indexRange(End  ,axis3);

  if( bcParameters.lineToAssign!=0 ) // *wdh* added 010825 to fix AMR problem 
  {
    // increase the size of the indexRange so we adjust fewer corner points 

    // printf("\n\n ************ INFO: fixBoundaryCorners only adjust ghost lines corner values > %i ***********\n\n",
    //    bcParameters.lineToAssign);
    
    for( int axis=0; axis<c.numberOfDimensions(); axis++ )
    {
      indexRange[0][axis]-=bcParameters.lineToAssign;
      indexRange[1][axis]+=bcParameters.lineToAssign;
    }
  }

  //     ---when two (or more) adjacent faces have boundary conditions
  //        we set the values on the fictitous line (or vertex)
  //        that is outside both faces ( points marked + below)
  //        We set values on all ghost points that lie outside the corner
  //
  //                + +                + +
  //                + +                + +
  //                    --------------
  //                    |            |
  //                    |            |
  //

  int side1,side2,side3,is1,is2,is3,i1,i3;
  

  Index I1=Range(indexRange[Start][axis1],indexRange[End][axis1]);
  Index I2=Range(indexRange[Start][axis2],indexRange[End][axis2]);
  Index I3=Range(indexRange[Start][axis3],indexRange[End][axis3]);
  Index N =C0!=nullRange ? C0 : Range(u.getComponentBase(0),u.getComponentBound(0));   // ********* Is this ok ?? *************

  //         ---extrapolate edges---
  Index I1m,I2m,I3m;
  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis2) )
  {
    //       ...Do the four edges parallel to i3
    side3=-1; 
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      for( side2=Start; side2<=End; side2++ )
      {
	is2=1-2*side2;
	if( c.boundaryCondition(side1,axis1)>0 || c.boundaryCondition(side2,axis2)>0 )
	{
	  I2m= side2==Start ? Range(c.dimension(side2,axis2)+1-is2,indexRange[side2][axis2]-is2) :
	    Range(indexRange[side2][axis2]-is2,c.dimension(side2,axis2)-1-is2);
          // We have to loop over i1 from inside to outside since later points depend on previous ones.
	  for( i1=indexRange[side1][axis1]; i1!=c.dimension(side1,axis1); i1-=is1 )
	  {
            I1m=i1-is1;
	    assignCorners(I1m,I2m,I3,N, is1,is2,0,I1m,I2m,I3,N, side1,side2,side3,orderOfExtrapolation,
			  u,bcParameters,c.numberOfDimensions());
	    // EXTRAP_SWITCH(i1-is1,i2-is2,I3,N, is1,is2,0,i1-is1,i2-is2,I3,N); 
	  }
	}
      }
    }
  }
 
  if( c.numberOfDimensions()<=2 ) return;

  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis3) )
  {
    //       ...Do the four edges parallel to i2
    side2=-1;
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      for( side3=Start; side3<=End; side3++ )
      {
        is3=1-2*side3;
        if( c.boundaryCondition(side1,axis1)>0 || c.boundaryCondition(side3,axis3)>0 )
	{
	   I3m= side3==Start ? Range(c.dimension(side3,axis3)+1-is3,indexRange[side3][axis3]-is3) :
	    Range(indexRange[side3][axis3]-is3,c.dimension(side3,axis3)-1-is3);

          // We have to loop over i1 from inside to outside since later points depend on previous ones.
	  for( i1=indexRange[side1][axis1]; i1!=c.dimension(side1,axis1); i1-=is1 )
	  {
            I1m=i1-is1;
	    assignCorners(I1m,I2,I3m,N,is1,0,is3,I1m,I2,I3m,N, side1,side2,side3,orderOfExtrapolation,
			  u,bcParameters,c.numberOfDimensions());
    	    // EXTRAP_SWITCH(i1-is1,I2,i3-is3,N,is1,0,is3,i1-is1,I2,i3-is3,N);
	  
	  }
	}
      }
    }
  }
  if( !c.isPeriodic(axis2) && !c.isPeriodic(axis3) )
  {
    //       ...Do the four edges parallel to i1
    side1=-1;
    for( side2=Start; side2<=End; side2++ )
    {
      is2=1-2*side2;
      I2m= side2==Start ? Range(c.dimension(side2,axis2)+1-is2,indexRange[side2][axis2]-is2) :
	Range(indexRange[side2][axis2]-is2,c.dimension(side2,axis2)-1-is2);
      for( side3=Start; side3<=End; side3++ )
      {
        is3=1-2*side3;
        if( c.boundaryCondition(side2,axis2)>0 || c.boundaryCondition(side3,axis3)>0 )
	{
          // We have to loop over i3 from inside to outside since later points depend on previous ones.
          for( i3=indexRange[side3][axis3]; i3!=c.dimension(side3,axis3); i3-=is3 )
	  {
	    I3m=i3-is3;
            assignCorners(I1,I2m,I3m,N, 0,is2,is3,I1,I2m,I3m,N, side1,side2,side3,orderOfExtrapolation,
			  u,bcParameters,c.numberOfDimensions());
            // EXTRAP_SWITCH(I1,i2-is2,i3-is3,N, 0,is2,is3,I1,i2-is2,i3-is3,N);
	  }
	}
      }
    }
  }
  
  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis2) && !c.isPeriodic(axis3) )
  {
    //    ...Do the points outside vertices in 3D
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      I1m= side1==Start ? Range(c.dimension(side1,axis1)+1-is1,indexRange[side1][axis1]-is1) :
	Range(indexRange[side1][axis1]-is1,c.dimension(side1,axis1)-1-is1);
      for( side2=Start; side2<=End; side2++ )
      {
        is2=1-2*side2;
	I2m= side2==Start ? Range(c.dimension(side2,axis2)+1-is2,indexRange[side2][axis2]-is2) :
	  Range(indexRange[side2][axis2]-is2,c.dimension(side2,axis2)-1-is2);
        for( side3=Start; side3<=End; side3++ )
        {
          is3=1-2*side3;
          if( c.boundaryCondition(side1,axis1)>0 || 
              c.boundaryCondition(side2,axis2)>0 || 
              c.boundaryCondition(side3,axis3)>0 )
	  {
            for( i3=indexRange[side3][axis3]; i3!=c.dimension(side3,axis3); i3-=is3 )
	    {
	      I3m=i3-is3;
              assignCorners(I1m,I2m,I3m,N, is1,is2,is3,I1m,I2m,I3m,N, side1,side2,side3,
			    orderOfExtrapolation,u,bcParameters,c.numberOfDimensions());
	      // EXTRAP_SWITCH(i1-is1,i2-is2,i3-is3,N, is1,is2,is3,i1-is1,i2-is2,i3-is3,N);
	    }
	    
	  }
	}
      }
    }
  }

}

#undef UX1
#undef UX2
#undef UX3
#undef UX4
#undef UX5
#undef UX6
#undef EXTRAP_SWITCH


