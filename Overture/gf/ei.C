#include "MappedGridOperators.o"


void MappedGridOperators::
extrapolateInterpolationPointNeighbours( realMappedGridFunction & u )
//=========================================================================
// /Description:
//   Extrapolate the values on the unused points that lie next to
// interpolation points.
//=========================================================================
{
  if( !extrapolateInterpolationNeighboursIsInitialized )
    findInterpolationNeighbours();

    for( m=0; m<numberOfInterpolationNeighbours[0]; m++ )
      u(ipn[0](m,0),ipn[0](1,m),ipn[0](2,m),C)=
                    3.*u(ipn[0](m,0)+  ipd[0](m),ipn[0](m,1),ipn[0](m,2),C)
                   -3.*u(ipn[0](m,0)+2*ipd[0](m),ipn[0](m,1),ipn[0](m,2),C)
		      +u(ipn[0](m,0)+3*ipd[0](m),ipn[0](m,1),ipn[0](m,2),C);
  
  if( mg.numberOfDimensions>1 )
  {
    for( m=0; m<numberOfInterpolationNeighbours[1]; m++ )
      u(ipn[1](m,0),ipn[1](1,m),ipn[1](2,m),C)=
                    3.*u(ipn[1](m,0),ipn[1](m,1)+  ipd[1](m),ipn[1](m,2),C)
                   -3.*u(ipn[1](m,0),ipn[1](m,1)+2*ipd[1](m),ipn[1](m,2),C)
		      +u(ipn[1](m,0),ipn[1](m,1)+3*ipd[1](m),ipn[1](m,2),C);
  }
  if( mg.numberOfDimensions>2 )
  {
    for( m=0; m<numberOfInterpolationNeighbours[2]; m++ )
      u(ipn[2](m,0),ipn[2](1,m),ipn[2](2,m),C)=
                    3.*u(ipn[2](m,0),ipn[2](m,1),ipn[2](m,2)+  ipd[2](m),C)
                   -3.*u(ipn[2](m,0),ipn[2](m,1),ipn[2](m,2)+2*ipd[2](m),C)
		      +u(ipn[2](m,0),ipn[2](m,1),ipn[2](m,2)+3*ipd[2](m),C);
  }
  


void MappedGridOperators:: 
findInterpolationPointNeighbours()
// ================================================================
//  Extrapolate points used by the Fourth-Order Artificial Viscosity
//             Initialization routine
// 
//   Purpose-
//     Extrapolate points next to interpolation points that are
//    used by the fourth-order artificial viscosity.
//    This is the initialization routine
// ================================================================
{
  extrapolateInterpolationNeighboursIsInitialized=TRUE;

  numberOfInterpolationNeighbours[0]=0;
  numberOfInterpolationNeighbours[1]=0;
  numberOfInterpolationNeighbours[2]=0;

  int & m0 = numberOfInterpolationNeighbours(0);
  int & m1 = numberOfInterpolationNeighbours(1);
  int & m2 = numberOfInterpolationNeighbours(2);

  for( int i3=mg.indexRange(Start,axis3); i3<=mg.indexRange(End,axis3); i3++ )
  for( int i2=mg.indexRange(Start,axis2); i2<=mg.indexRange(End,axis2); i2++ )
  for( int i1=mg.indexRange(Start,axis1); i1<=mg.indexRange(End,axis1); i1++ )
  {
    if( mg.mask(i1,i2,i3) < 0 )
    {
      
      for( int axis=0; axis<mg.numberOfDimensions; axis++ )
      {
	if( numberOfInterpolationNeighbours(axis) >= ipn[axis].getLength(0)-1 )
	{
	  // allocate space for arrays ipn and ipd
	  int newSize = ipn[axis].getLength(0)==0 ? cg.numberOfInterpolationPoints(grid)*cg.numberOfDimensions +20 
                                                  : ipn[axis].getLength(0) + cg.numberOfInterpolationPoints(grid) ;
	  
	  ipn[axis].resize(newSize,3);
	  ipd[axis].resize(newSize);
	}
      }
      // find points to extrapolate along axis1
      if( i1==mg.indexRange(Start,axis1) || mg.mask(i1-1,i2,i3)==0 )
      {
	m0++;
	ipn[0](m0,0)=i1-1;
	ipn[0](m0,1)=i2;
	ipn[0](m0,2)=i3;
	ipd[0](m0)=+1;
      }
      else if( i1==mg.indexRange(End,axis1) || mg.mask(i1+1,i2,i3)==0 )
      {
	m0++;
	ipn[0](m0,0)=i1+1;
	ipn[0](m0,1)=i2;
	ipn[0](m0,2)=i3;
	ipd[0](m0)=-1;
      }
      if( mg.numberOfDimensions > 1 )
      {
	// find points to extrapolate along axis2
	if( i2==mg.indexRange(Start,axis2) || mg.mask(i1,i2-1,i3)==0 )
	{
	  m1++;
	  ipn[1](m1,0)=i1; 
	  ipn[1](m1,1)=i2-1;
	  ipn[1](m1,2)=i3;
	  ipd[1](m1)=+1;
	}
	else if( i2==mg.indexRange(End,axis2) || mg.mask(i1,i2+1,i3)==0 )
	{
	  m1++;
	  ipn[1](m1,0)=i1;
	  ipn[1](m1,1)=i2+1;
	  ipn[1](m1,2)=i3;
	  ipd[1](m1)=-1;
	}
      }
      if( mg.numberOfDimensions > 2 )
      {
	
	// find points to extrapolate along axis1
	if( i3==mg.indexRange(Start,axis3) || mg.mask(i1,i2,i3-1)==0 )
	{
	  m2++;
	  ipn[2](m2,0)=i1;
	  ipn[2](m2,1)=i2;
	  ipn[2](m2,2)=i3-1;
	  ipd[2](m2)=+1;
	}
	else if( i3==mg.indexRange(End,axis3) || mg.mask(i1,i2,i3+1)==0 )
	{
	  m2++;
	  ipn[2](m2,0)=i1;
	  ipn[2](m2,1)=i2;
	  ipn[2](m2,2)=i3+1;
	  ipd[2](m2)=-1;
	}
      }
    }
  }
  
}   

