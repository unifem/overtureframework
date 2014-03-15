#ifndef X_DERIVATIVE_H
#define X_DERIVATIVE_H

//===============================================================================
// Define the standard finite difference derivative functions
//===============================================================================

#undef DERIVATIVE
#define DERIVATIVE(name)                  \
void                                      \
name (const realMappedGridFunction & u,   \
      RealDistributedArray & derivative,             \
      const Index & I1,                   \
      const Index & I2,                   \
      const Index & I3,                   \
      const Index & N,                    \
      MappedGridOperators & mgop );


DERIVATIVE(rDerivative)
DERIVATIVE(sDerivative)
DERIVATIVE(tDerivative)
DERIVATIVE(rrDerivative)
DERIVATIVE(rsDerivative)
DERIVATIVE(rtDerivative)
DERIVATIVE(ssDerivative)
DERIVATIVE(stDerivative)
DERIVATIVE(ttDerivative)

DERIVATIVE(xFDerivative)
DERIVATIVE(yFDerivative)
DERIVATIVE(zFDerivative)
DERIVATIVE(xxFDerivative)
DERIVATIVE(xyFDerivative)
DERIVATIVE(xzFDerivative)
DERIVATIVE(yyFDerivative)
DERIVATIVE(yzFDerivative)
DERIVATIVE(zzFDerivative)
DERIVATIVE(laplaceFDerivative)
DERIVATIVE(divFDerivative)
DERIVATIVE(gradFDerivative)
DERIVATIVE(identityFDerivative)
DERIVATIVE(vorticityFDerivative)


#undef DERIVATIVE
#define DERIVATIVE(name)                  \
void                                      \
name (RealDistributedArray & derivative,             \
      const Index & I1,                   \
      const Index & I2,                   \
      const Index & I3,                   \
      const Index & E,                    \
      const Index & C,                    \
      MappedGridOperators & mgop );

DERIVATIVE(rDerivCoefficients)
DERIVATIVE(sDerivCoefficients)
DERIVATIVE(tDerivCoefficients)
DERIVATIVE(rrDerivCoefficients)
DERIVATIVE(rsDerivCoefficients)
DERIVATIVE(rtDerivCoefficients)
DERIVATIVE(ssDerivCoefficients)
DERIVATIVE(stDerivCoefficients)
DERIVATIVE(ttDerivCoefficients)

DERIVATIVE(xFDerivCoefficients)
DERIVATIVE(yFDerivCoefficients)
DERIVATIVE(zFDerivCoefficients)
DERIVATIVE(xxFDerivCoefficients)
DERIVATIVE(xyFDerivCoefficients)
DERIVATIVE(xzFDerivCoefficients)
DERIVATIVE(yyFDerivCoefficients)
DERIVATIVE(yzFDerivCoefficients)
DERIVATIVE(zzFDerivCoefficients)
DERIVATIVE(laplaceFDerivCoefficients)
DERIVATIVE(divFDerivCoefficients)
DERIVATIVE(gradFDerivCoefficients)
DERIVATIVE(identityFDerivCoefficients)

#undef DERIVATIVE
#endif
