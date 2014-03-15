#include "InterfaceInfo.h"

// This class holds info about an interface

// grid1,side1,dir1 : defines face 1
// grid2,side2,dir2 : defines matching face 2


InterfaceInfo::InterfaceInfo()
{
  grid1=-1; side1=-1; dir1=-1;
  grid2=-1; side2=-1; dir2=-1;
  ndf1=0; ndf2=0;
  initialized=false;
  
  rwk=NULL;
  iwk=NULL;
}

InterfaceInfo::InterfaceInfo(int grid1_, int side1_, int dir1_, 
                             int grid2_, int side2_, int dir2_)
{
  grid1=grid1_; side1=side1_; dir1=dir1_;
  grid2=grid2_; side2=side2_; dir2=dir2_;
  ndf1=0; ndf2=0;
  initialized=false;
   
  rwk=NULL;
  iwk=NULL;
}



InterfaceInfo::
~InterfaceInfo()
{
  delete [] rwk;
  delete [] iwk;
}

