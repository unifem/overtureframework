// 100525 macros and other stuff Kyle likes to use

// in common/src/getBounds.C : (should use new version in ParallelGridUtility.h)
// 101102, now using ParallelGridUtility
//extern void
//getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                     IntegerArray & gidLocal, 
//                                     IntegerArray & dimensionLocal, 
//                                     IntegerArray & bcLocal );


// Macros that hide the use of PPP when getting local data
#ifdef USE_PPP

#define OV_GET_LOCAL_ARRAY(TYPE,UU) TYPE ## SerialArray UU ## Local; getLocalArrayWithGhostBoundaries(UU,UU ## Local);
#define OV_GET_LOCAL_ARRAY_CONST(TYPE,UU) TYPE ## SerialArray UU ## Local; getLocalArrayWithGhostBoundaries(UU,UU ## Local);
#define OV_GET_LOCAL_ARRAY_FROM(TYPE,UU,FROM) TYPE ## SerialArray UU ## Local; getLocalArrayWithGhostBoundaries(FROM,UU ## Local);
#define OV_GET_LOCAL_ARRAY_CONDITIONAL(TYPE,UU,boolExp,isTrue, isFalse) TYPE ## SerialArray UU ## Local; \
                                                             if ( boolExp ) { isTrue; } \
                                                             else { getLocalArrayWithGhostBoundaries( isFalse, UU ## Local ); }

#else

#define OV_GET_LOCAL_ARRAY(TYPE,UU) TYPE ## SerialArray & UU ## Local = UU;
#define OV_GET_LOCAL_ARRAY_CONST(TYPE,UU) const TYPE ## SerialArray & UU ## Local = UU;
#define OV_GET_LOCAL_ARRAY_FROM(TYPE,UU,FROM) TYPE ## SerialArray & UU ## Local = FROM;
#define OV_GET_LOCAL_ARRAY_CONDITIONAL(TYPE,UU,boolExp,isTrue, isFalse) TYPE ## SerialArray & UU ## Local = ( boolExp ) ? isTrue : isFalse;

#endif

// Macros that help extract and use data pointers from A++P++ arrays
#define OV_APP_TO_PTR_3D(TYPE,ARRAY, PNAME) TYPE *PNAME = ARRAY.Array_Descriptor.Array_View_Pointer2; \
  const int PNAME ## _d0 = ARRAY.getRawDataSize(0); \
  const int PNAME ## _d1 = ARRAY.getRawDataSize(1); 
  
#define A_3D(PNAME,i0,i1,i2) PNAME[i0+(PNAME ## _d0)*(i1+(PNAME ## _d1)*(i2))]

#define OV_APP_TO_PTR_4D(TYPE,ARRAY, PNAME) TYPE *PNAME = ARRAY.Array_Descriptor.Array_View_Pointer3; \
  const int PNAME ## _d0 = ARRAY.getRawDataSize(0); \
  const int PNAME ## _d1 = ARRAY.getRawDataSize(1); \
  const int PNAME ## _d2 = ARRAY.getRawDataSize(2); 

#define A_4D(PNAME,i0,i1,i2,i3) PNAME[i0+(PNAME ## _d0)*(i1+(PNAME ## _d1)*(i2+(PNAME ## _d2)*(i3)))]

#define OV_APP_TO_PTR_5D(TYPE,ARRAY, PNAME) TYPE *PNAME = ARRAY.Array_Descriptor.Array_View_Pointer4; \
  const int PNAME ## _d0 = ARRAY.getRawDataSize(0); \
  const int PNAME ## _d1 = ARRAY.getRawDataSize(1); \
  const int PNAME ## _d2 = ARRAY.getRawDataSize(2); \
  const int PNAME ## _d3 = ARRAY.getRawDataSize(3);

#define A_5D(PNAME,i0,i1,i2,i3,i4) PNAME[i0+(PNAME ## _d0)*(i1+(PNAME ## _d1)*(i2+(PNAME ## _d2)*(i3+(PNAME ## _d3)*(i4))))]

#define OV_RGF_TO_PTR_5D(TYPE,ARRAY, PNAME, ND) TYPE *PNAME = ARRAY.Array_Descriptor.Array_View_Pointer3; \
  const int PNAME ## _d0 = ARRAY.getRawDataSize(0); \
  const int PNAME ## _d1 = ARRAY.getRawDataSize(1); \
  const int PNAME ## _d2 = ARRAY.getRawDataSize(2); \
  const int PNAME ## _d3 = ND;

// use A_5D to access RGF array pointers


inline void solveSmallSystem(const int &nd,
			      const ArraySimpleFixed<real,3,3,1,1> &A,
			      const ArraySimpleFixed<real,3,1,1,1> &f,
			      ArraySimpleFixed<real,3,1,1,1> &u)
{
  // solve Au=f for 2 and 3D systems.  Because how many times do I have to implement this?

  real det = nd==3 ? 
    ( A(0,0)*(A(1,1)*A(2,2)-A(1,2)*A(2,1)) - // 3D
      A(0,1)*(A(1,0)*A(2,2)-A(1,2)*A(2,0)) +
      A(0,2)*(A(1,0)*A(2,1)-A(2,0)*A(1,2)) ) :
    ( A(0,0)*A(1,1) - A(0,1)*A(1,0) ); // 2D

  assert(fabs(det)>10*REAL_EPSILON);

  real deti=1./det;

  if ( nd==2 )
    {
      u[0] = ( A(1,1)*f[0] - A(0,1)*f[1])*deti;
      u[1] = (-A(1,0)*f[0] + A(0,0)*f[1])*deti;
      u[2] = 0.;
    }
  else
    {
      u[0] = ( (A(1,1)*A(2,2)-A(1,2)*A(2,1))*f(0) -
	       (A(0,1)*A(2,2)-A(0,2)*A(2,1))*f(1) +
	       (A(0,1)*A(1,2)-A(0,2)*A(1,1))*f(2) )*deti;

      u[1] = (-(A(1,0)*A(2,2)-A(1,2)*A(2,0))*f(0)+
  	       (A(0,0)*A(2,2)-A(2,0)*A(0,2))*f(1)-
 	       (A(0,0)*A(1,2)-A(0,2)*A(1,0))*f(2) )*deti;

      u[2] = ( (A(1,0)*A(2,1)-A(1,1)*A(2,0))*f(0) -
	       (A(0,0)*A(2,1)-A(0,1)*A(2,0))*f(1) +
	       (A(0,0)*A(1,1)-A(0,1)*A(1,0))*f(2) )*deti;
    }
}
