#include "MatrixTransformMotionFunction.h"

MatrixTransformMotionFunction::
MatrixTransformMotionFunction ()
{
  cout << "MatrixTransformMotionFunction::default constructor called" << endl;
  coefficientsReDimd = LogicalFalse;
  translationalCoefficientsInitialized = LogicalFalse;
  angularCoefficientsInitialized = LogicalFalse;
}

MatrixTransformMotionFunction::
MatrixTransformMotionFunction( MatrixTransformMotionFunction& function_)
  :
  numberOfDimensions (function_.numberOfDimensions),
  coefficientsReDimd (function_.coefficientsReDimd),
  translationalCoefficientsInitialized (function_.translationalCoefficientsInitialized),
  angularCoefficientsInitialized (function_.angularCoefficientsInitialized),
  
  trCoeff  (function_.trCoeff),
  trFreq   (function_.trFreq),
  trOffset (function_.trOffset),
  angCoeff (function_.angCoeff),
  angFreq  (function_.angFreq),
  angOffset(function_.angOffset)
//
// /Purpose: copy constructor
//
{
  //...no assignments in copy constructor
  cout << "MatrixTransformMotionFunction::copy constructor called" << endl;
}

MatrixTransformMotionFunction::
MatrixTransformMotionFunction (const int& numberOfDimensions_)
{
  real ZERO=static_cast<real>(0.0);
  
  numberOfDimensions = numberOfDimensions_;

  trCoeff.redim (numberOfDimensions);
  trFreq.redim  (numberOfDimensions);
  trOffset.redim(numberOfDimensions);

  trCoeff = ZERO;
  trFreq  = ZERO;
  trOffset= ZERO;

  coefficientsReDimd                   = LogicalTrue;
  translationalCoefficientsInitialized = LogicalTrue;
  angularCoefficientsInitialized       = LogicalTrue;
  
}

MatrixTransformMotionFunction::
~MatrixTransformMotionFunction ()
{
}


void MatrixTransformMotionFunction::
setTranslationalParameters (const RealArray& trCoeff_,
			    const RealArray& trFreq_,
			    const RealArray& trOffset_)
{
  assert (coefficientsReDimd);
  assert (trCoeff_.getBase(0)<1 && trCoeff_.getBound(0)>numberOfDimensions-2);
  assert (trFreq_.getBase(0)<1 && trFreq_.getBound(0)>numberOfDimensions-2);

//  real ZERO=static_cast<real>(0.0);

  for (int i=0; i<numberOfDimensions; i++)
  {
    trCoeff(i) = trCoeff_(i);
    trFreq(i)  = trFreq_(i);
    trOffset(i)= trOffset_(i);
  }
  translationalCoefficientsInitialized = LogicalTrue;
}

void MatrixTransformMotionFunction::
setAngularParameters (const real& angCoeff_,
		      const real& angFreq_,
		      const real& angOffset_)
{
  angCoeff = angCoeff_;
  angFreq  = angFreq_;
  angOffset= angOffset_;

  angularCoefficientsInitialized = LogicalTrue;
  
}

real MatrixTransformMotionFunction::
rotationalMotion (const real& time)
//
// angle is normalized to 1., so multiply by 2.*Pi. before calling rotate()
//
{
  real angle, TWO=static_cast<real>(2.0);
//  real degConv = 360.;

  assert (angularCoefficientsInitialized);

  angle = angCoeff * sin (TWO*Pi*angFreq * (time - angOffset));
  return angle;
}

real MatrixTransformMotionFunction::
rotationalVelocity (const real& time)
{
  real angVelocity, TWO=static_cast<real>(2.0);
//  real degConv = 360.;

  assert (angularCoefficientsInitialized);

  angVelocity = angCoeff*TWO*Pi*angFreq * cos (TWO*Pi*angFreq * (time - angOffset));
  return angVelocity;
}

real MatrixTransformMotionFunction::
translationalMotion (const real& time, const int& axis)
{
  assert (axis>-1 && axis<3);
  assert (translationalCoefficientsInitialized);
  
  real x, TWO=static_cast<real>(2.0);

  x = trCoeff(axis) * sin (TWO*Pi*trFreq(axis) * (time - trOffset(axis)));
  return x;
}

real MatrixTransformMotionFunction::
translationalVelocity (const real& time, const int& axis)
{
  assert (axis>-1 && axis<3);
  assert (translationalCoefficientsInitialized);

  real v, TWO=static_cast<real>(2.0);

  v = TWO*Pi*trFreq(axis)*trCoeff(axis) * 
    cos (TWO*Pi*trFreq(axis) * (time - trOffset(axis)));
  
  return v;
}

					       
  
  
