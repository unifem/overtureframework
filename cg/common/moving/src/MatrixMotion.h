#ifndef MATRIX_MOTION_H
#define MATRIX_MOTION_H

#include "TimeFunction.h"


// ===========================================================================================
/// \brief This class knows how to rotate around a line in space or translate along a line
// ===========================================================================================
class MatrixMotion : public ReferenceCounting
{
public:

enum MotionTypeEnum
{
  rotateAroundALine,
  translateAlongALine
};


MatrixMotion();

// copy constructor
MatrixMotion( const MatrixMotion & mm, const CopyType ct  = DEEP );

~MatrixMotion();

// operator = 
MatrixMotion & operator =( const MatrixMotion & mm );

// evaluate the motion matrix 
int getMotion( const real & t, RealArray & r );
// evaluate the motion matrix and a time derivative
int getMotion( const real & t, RealArray & r, RealArray & rp, int derivative, bool computeComposed=true );

// set the line of rotation or line of translation
int setLine( const real *x0, const real *v, const MotionTypeEnum motion = rotateAroundALine  );

int setMotionType( const MotionTypeEnum motion );

// compose this MatrixMotion with another which is applied first (set to NULL for none)
int compose( MatrixMotion *motion );

// interactively update parameters:
int update(GenericGraphicsInterface & gi );

// get from a data base file
int get( const GenericDataBase & dir, const aString & name);

// put to a data base file
int put( GenericDataBase & dir, const aString & name) const;

// Write information about the moving grids
void writeParameterSummary( FILE *file= stdout );

private:

  virtual ReferenceCounting& operator=( const ReferenceCounting & x)
    { return operator=( *(MatrixMotion*) & x ); }
  virtual void reference( const ReferenceCounting & x)
    { reference( (MatrixMotion &) x ); }
  virtual ReferenceCounting* virtualConstructor(const CopyType ct  = DEEP) const
    { return ::new MatrixMotion(*this, ct); }


MotionTypeEnum motionType;

real x0[3];  // a point on the line of rotation
real v[3];   // tangent to the line of rotation

TimeFunction timeFunction;  // a function of time (can define the angle as a function of time, for e.g.)

MatrixMotion *preMotion;  // if non NULL then the current motion follows this motion (i.e. we compose the motions)


};

#endif
