#include "MappedGridFiniteVolumeOperators.h"

// extern realMappedGridFunction Overture::nullDoubleMappedGridFunction();
// extern realMappedGridFunction Overture::nullFloatMappedGridFunction();
// // *wdh* ifdef DOUBLE
// ifdef OV_USE_DOUBLE
// define NULLRealMappedGridFunction Overture::nullDoubleMappedGridFunction()
// else
// define NULLRealMappedGridFunction Overture::nullFloatMappedGridFunction()
// endif

static void
throwErrorMessage( const aString & routineName )
{
  cout << "ERROR:MappedGridFiniteVolumeOperators:: `" << routineName << "' not yet implemented! \n" ; 
  throw "MappedGridFiniteVolumeOperators::ERROR:function not implemented! " ; 
}


// ************************************************
// ***** DIFFERENTIATION CLASS FUNCTIONS **********
// ************************************************


// Macro to define a typical function 
#define FUNCTION(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}                                                              

// Macro to define a typical typed function 

#define TYPED_FUNCTION(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                 \
}

// Macro to define a function with nonstandard arguments

#define TYPED_FUNCTION_2GF(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const realMappedGridFunction & s, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                 \
}

#define NS2GF_TYPED_FUNCTION(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const realMappedGridFunction & v, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3)      \
{                                                   \
  throwErrorMessage("type");                       \
  return Overture::nullRealMappedGridFunction();               \
}

#define NS58_TYPED_FUNCTION(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const int i1, \
			    const int i2, \
			    const int i3, \
			    const int i4, \
			    const int i5, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8       \
			    )      \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                 \
}

#define NS5_TYPED_FUNCTION(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const int i1, \
			    const int i2, \
			    const int i3, \
			    const int i4, \
			    const int i5, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3      \
			    )      \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                 \
}

#define NS6_TYPED_FUNCTION(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const int i1,  \
			    const int i2,  \
			    const int i3,  \
			    const int i4,  \
			    const int i5,  \
			    const int i6,  \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3)      \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                 \
}

#define NS7_TYPED_FUNCTION(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const int i1,  \
			    const int i2,  \
			    const int i3,  \
			    const int i4,  \
			    const int i5,  \
			    const int i6,  \
			    const int i7, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3)      \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                 \
}

// Macro to define a typical function 
#define FUNCTION_COEFFICIENTS(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(                                   \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}   

// Macro to define a typical typed function 

#define TYPED_FUNCTION_COEFFICIENTS(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(                                   \
			    const GridFunctionParameters & gfType,   \
                            const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}

#define TYPED_FUNCTION_COEFFICIENTS_S(type) \
realMappedGridFunction MappedGridFiniteVolumeOperators::            \
                       type(                                   \
			    const GridFunctionParameters & gfType,   \
			    const realMappedGridFunction & gf, \
                            const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage("type");                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}

TYPED_FUNCTION (convectiveDerivative)
NS2GF_TYPED_FUNCTION (convectiveDerivative)
NS6_TYPED_FUNCTION (difference)
//NS5_TYPED_FUNCTION (FCgrad)
NS58_TYPED_FUNCTION (FCgrad)
TYPED_FUNCTION (normalVelocity)
NS7_TYPED_FUNCTION (faceAverage)
TYPED_FUNCTION (laplacian)
NS6_TYPED_FUNCTION (average)
TYPED_FUNCTION (div)
TYPED_FUNCTION_2GF (divScalarGrad)
TYPED_FUNCTION (divNormal)
TYPED_FUNCTION (vorticity)
TYPED_FUNCTION (contravariantVelocity)
TYPED_FUNCTION (cellsToFaces)
TYPED_FUNCTION_2GF (divInverseScalarGrad)
NS6_TYPED_FUNCTION (dZero)

TYPED_FUNCTION_COEFFICIENTS (identityCoefficients)
TYPED_FUNCTION_COEFFICIENTS_S (divInverseScalarGradCoefficients)
TYPED_FUNCTION_COEFFICIENTS (laplacianCoefficients)
TYPED_FUNCTION_COEFFICIENTS_S (divScalarGradCoefficients)


#undef FUNCTION
#undef TYPED_FUNCTION
#undef TYPED_FUNCTION_2GF
#undef NS2GF_TYPED_FUNCTION
#undef NS5_TYPED_FUNCTION
#undef NS58_TYPED_FUNCTION
#undef NS6_TYPED_FUNCTION
#undef NS7_TYPED_FUNCTION
#undef FUNCTION_COEFFICIENTS
#undef TYPED_FUNCTION_COEFFICIENTS
#undef TYPED_FUNCTION_COEFFICIENTS_S

