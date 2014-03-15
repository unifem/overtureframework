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

  // local copies of geometry data near interfaces needed in parallel:
  pmask1=NULL; prsxy1=NULL; pxy1=NULL;
  pmask2=NULL; prsxy2=NULL; pxy2=NULL;

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

  // local copies of geometry data near interfaces needed in parallel:
  pmask1=NULL; prsxy1=NULL; pxy1=NULL;
  pmask2=NULL; prsxy2=NULL; pxy2=NULL;
}

InterfaceInfo::
InterfaceInfo(const InterfaceInfo& x)
// copy constructor
{
  *this = x;
}


// ====================================================================================
/// \brief equal operator.
/// \note This operator is called by stl when adding elements to a list
// ====================================================================================
InterfaceInfo& InterfaceInfo::
operator=( const InterfaceInfo& x)
{
  grid1=x.grid1;
  side1=x.side1;
  dir1 =x.dir1;
  grid2=x.grid2;
  side2=x.side2;
  dir2 =x.dir2;
  ndf1=x.ndf1;
  ndf2=x.ndf2;
  initialized=x.initialized;
  rwk=x.rwk;
  iwk=x.iwk;
  assert( rwk==NULL && iwk==NULL );  // enforce this since we don't reference count

  pmask1=x.pmask1;
  prsxy1=x.prsxy1;
  pxy1=x.pxy1;
  pmask2=x.pmask2;
  prsxy2=x.prsxy2;
  pxy2=x.pxy2;
  
  assert( pmask1==NULL && prsxy1==NULL && pxy1==NULL ); // enforce this since we don't reference count
  assert( pmask2==NULL && prsxy2==NULL && pxy2==NULL ); // enforce this since we don't reference count
}


InterfaceInfo::
~InterfaceInfo()
{
  delete [] rwk;
  delete [] iwk;

 delete pmask1; 
 delete prsxy1;
 delete pxy1;
 delete pmask2; 
 delete prsxy2; 
 delete pxy2;

}

