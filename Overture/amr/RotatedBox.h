#ifndef ROTATED_BOX_H
#define ROTATED_BOX_H

#include "Overture.h"

// The RotatedBox class is used to define a refinement grid for AMR
// Given a set of points in space (tagged error points) a rotated
// box with be determined that contains all the points.

class RotatedBox
{
 public:
  RotatedBox(int numberOfDimensions);
  RotatedBox(int numberOfDimensions, realArray & x );
  ~RotatedBox();
  
  int display() const;
  int fitBox();
  real getEfficiency() const;
  int numberOfPoints() const;

  bool intersects( RotatedBox & box, real distance=0. ) const;  // does this box intersect another
  
  int setPoints( realArray & x );

  real centre[3];   // centre
  real axisVector[3][3];    // vectors in the directions of the axes
  real halfAxesLength[3];       // half the length of the axes.
   
  int numberOfDimensions;
  realArray xa;     // positions of tagged points inside this box
};

#endif

  
