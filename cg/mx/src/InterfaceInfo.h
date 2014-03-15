#ifndef INTERFACE_INFO_H
#define INTERFACE_INFO_H

#include "Overture.h"

// This class holds info about an interface

// grid1,side1,dir1 : defines face 1
// grid2,side2,dir2 : defines matching face 2

class InterfaceInfo
{
public:

InterfaceInfo();
InterfaceInfo(int grid1, int side1, int dir1, 
              int grid2, int side2, int dir2);
InterfaceInfo(const InterfaceInfo& x);

~InterfaceInfo();

InterfaceInfo& operator=( const InterfaceInfo& x);

int grid1,side1,dir1;
int grid2,side2,dir2;

int ndf1,ndf2;  // number of points on face1 and face2 

bool initialized; // set to true when interface has been initialized

// work-space
real *rwk;
int *iwk;

// local copies of geometry data near interfaces needed in parallel:
intSerialArray *pmask1, *pmask2;
realSerialArray *prsxy1, *pxy1, *prsxy2, *pxy2;

};


#endif
