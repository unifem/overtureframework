#ifndef TIME_FUNCTION_H
#define TIME_FUNCTION_H

#include "Overture.h"


// ============================================================================
/// \brief This class defines a function of time that can be used to rotate 
///    or translate a body
// ===========================================================================
class TimeFunction : public ReferenceCounting
{
public:

enum FunctionTypeEnum
{
  linearFunction,
  sinusoidalFunction,
  rampFunction,
  mappingFunction,
  userDefinedFunction
};


enum ComposeTypeEnum
{
  composeWillMultiply,
  composeWillAdd
};



TimeFunction();

// copy constructor
TimeFunction( const TimeFunction & tf, const CopyType ct  = DEEP );

~TimeFunction();

// operator = 
TimeFunction & operator =( const TimeFunction & tf );

int compose( TimeFunction *preFunc, const ComposeTypeEnum ct=composeWillMultiply );

int eval( const real t, real & f );
int eval( const real t, real & f, real & ft );
// int eval( const real t, real & f, real & ft , real & ftt );
int evalDerivative( const real t, real & fp, int derivative, bool computeComposed=true );

int setLinearFunction( const real a0, const real a1 );

int setRampFunction( const real rampStart, const real rampEnd,
		     const real rampStartTime, const real rampEndTime,
		     const int rampOrder );

int setSinusoidFunction( const real b0, const real f0, const real t0 );

// interactively update parameters:
int update(GenericGraphicsInterface & gi );

// get from a data base file
int get( const GenericDataBase & dir, const aString & name);

// put to a data base file
int put( GenericDataBase & dir, const aString & name) const;

private:

  virtual ReferenceCounting& operator=( const ReferenceCounting & x)
    { return operator=( *(TimeFunction*) & x ); }
  virtual void reference( const ReferenceCounting & x)
    { reference( (TimeFunction &) x ); }
  virtual ReferenceCounting* virtualConstructor(const CopyType ct  = DEEP) const
    { return ::new TimeFunction(*this, ct); }


FunctionTypeEnum functionType;
ComposeTypeEnum composeType;

real a0,a1;     // for linear function
real b0,f0,t0;  // for sinusoid
real rampStart,rampEnd,rampStartTime,rampEndTime;  // for ramp
int rampOrder;  // for ramp

IntegerArray ipar;  // put user defined integer parameters here
RealArray rpar;     // put user defined real parameters here

TimeFunction *preFunction;  // optionally specify a function to compose with 

MappingRC mapFunction;   // for a time function defined from a Mapping
real timeParameterizationScaleFactor; // factor to scale time to the unit interval, used for the mappingFunction

};

#endif
