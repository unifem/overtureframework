#include "Bcmgf.h"

#undef  RX
#define RX(i,j,k,m,n) rx0(i,j,k,m+numberOfDimensions*n)

#define NORMAL   \
        normal[side+2*(axis+numberOfDimensions*(grid))]

// Extrapolation:
#define X3P(n1,n2,i1,i2,i3,axis) ( \
   - 3.*u(i1+  (n1),i2+  (n2),i3,axis)+ 3.*u(i1+2*(n1),i2+2*(n2),i3,axis) \
   -    u(i1+3*(n1),i2+3*(n2),i3,axis) \
                                 )
#define x33P(n1,n2,n3,i1,i2,i3,axis) ( \
        - 3.*u(i1+  (n1),i2+  (n2),i3+  (n3),axis)  \
        + 3.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),axis)  \
        -    u(i1+3*(n1),i2+3*(n2),i3+3*(n3),axis)  \
                                   )

//======================================================================
//
// Purpose:
//   Given u and v at interior points and boundaries determine u and v on
//   one line of fictitious points for iord=2.
//
// To get the unknowns u(-1), v(-1), [w(-1)] use:
//     use    (1)  u.x+v.y=0
//            (2) 2a : if bc=5, set (t.u).n=0
//                2b : extrapolate tangential velocity t.u (-1)
//
// Input
//   t : time
//   u : u at interior nodes, a guess for p at all nodes
//   u2  : work space  (?? don't need)
//
// Output
//   u : u at all nodes
//
//======================================================================
void BCmgf::
divbc( )
{
//      x3(n1,n2,i1,i2,i3,kd)=
//     &        u1(i1     ,i2     ,i3,kd)
//     &   - 3.*u1(i1+  n1,i2+  n2,i3,kd)+ 3.*u1(i1+2*n1,i2+2*n2,i3,kd)
//     &   -    u1(i1+3*n1,i2+3*n2,i3,kd)
//      x3p(n1,n2,i1,i2,i3,kd)=
//     &   - 3.*u1(i1+  n1,i2+  n2,i3,kd)+ 3.*u1(i1+2*n1,i2+2*n2,i3,kd)
//     &   -    u1(i1+3*n1,i2+3*n2,i3,kd)
//      x33p(n1,n2,n3,i1,i2,i3,kd)=
//     &   - 3.*u1(i1+  n1,i2+  n2,i3+  n3,kd)
//     &   + 3.*u1(i1+2*n1,i2+2*n2,i3+2*n3,kd)
//     &   -    u1(i1+3*n1,i2+3*n2,i3+3*n3,kd)
//      x5p(n1,n2,i1,i2,i3,kd)=
//     &   - 5.*u1(i1+  n1,i2+  n2,i3,kd)+10.*u1(i1+2*n1,i2+2*n2,i3,kd)
//     &   -10.*u1(i1+3*n1,i2+3*n2,i3,kd)+ 5.*u1(i1+4*n1,i2+4*n2,i3,kd)
//     &   -    u1(i1+5*n1,i2+5*n2,i3,kd)
//c........end statement functions
//

  if( orderOfAccuracy!=2 )
  {
    cout << " ERROR:divbc: iord!=2 \n";
    exit;
  }

  int m=orderOfAccuracy/2;
  int axis,side,i1,i2,i3,is;
  real g1,g2;

  RealArray & rx0 = c.inverseVertexDerivative;

  int n1=0; // Here are the two components ****
  int n2=1;
  int n2=2;

  int is1,is2,is3,End1,End2,End3;

  RealArray rx(2,2);

  if( numberOfDimensions==2 )
  {
    //       ---2D:
    //       orderOfAccuracy=2:
    //        (1) u_x+v_y=0
    //      =>  rx*u(-1)+ry*v(-1) = rx*u(+1)+ry*v(+1)+2*dr*(sx*u.s+sy*v.s)=g1
    //        (2) tv.D+(4) \uv =0
    //      => t1*u(-1) + t2*v(-1) = -t1*(D+(4)-u(-1)) - t2*(D+(4)-v(-1))=g2

    for( axis=axis1; axis<numberOfDimensions; axis++ )
    {
      real dr = c.gridSpacing(axis);  // 1./(nrs(kd,2,k)-nrs(kd,1,k))
      i3=c.indexRange(Start,axis);
      for( int side=Start; side<=End; side++ )
      {
	if( c.boundaryCondition(side,axis) > 0 )
	{
          End1 = axis==axis1 ? Start : End;
          End2 = axis==axis2 ? Start : End;
          is1  = axis==axis1 ? 1-2*side : 0;
          is2  = axis==axis2 ? 1-2*side : 0;

	  for( i1=c.indexRange(Start,axis1); i1<=c.gridIndexRange(End1,axis1); i1++ )
	  {
	    for( i2=c.indexRange(Start,axis2); i2<=c.gridIndexRange(End2,axis2); i2++ )
	    {
	      if( cg.mask[grid](i1,i2,i3) > 0 )
	      {
		rx(0,0)=RX(i1,i2,i3,axis1,axis1);
		rx(0,1)=RX(i1,i2,i3,axis1,axis2);
		rx(1,0)=RX(i1,i2,i3,axis2,axis1);
		rx(1,1)=RX(i1,i2,i3,axis2,axis2);
		t1= NORMAL(i1,i2,i3,axis2);      // *** needs grid *** an(2,i1,i2,i3,kd,ks,k)
		t2=-NORMAL(i1,i2,i3,axis1);      // -an(1,i1,i2,i3,kd,ks,k)
		g1=rx(axis,0)*u(i1+is1,i2+is2,i3,n1)+rx(axis,1)*u(i1+is1,i2+is2,i3,n2);
		if( axis==axis1 )
		  g1+=is1*2.*dr*(rx(1,0)*us2(i1,i2,i3,n1)+rx(1,1)*us2(i1,i2,i3,n2));
                else
		  g1+=is2*2.*dr*(rx(0,0)*ur2(i1,i2,i3,n1)+rx(0,1)*ur2(i1,i2,i3,n2));  
		if( c.boundaryCondition(side,axis)==5 || c.boundaryCondition(side,axis)==12 )
		{
		  // ..slip wall, set (t.u).n=0
		  g2=t1*u(i1+is1,i2+is2,i3,n1)+t2*u(i1+is1,i2+is2,i3,n2);
		  if( twilightZoneFlow )
		  {
                    g2+=t1*((*e)(i1-is1,i2-is2,i3,n1,t)-(*e)(i1+is1,i2+is2,i3,n1,t))
 		       +t2*((*e)(i1-is1,i2-is2,i3,n2,t)-(*e)(i1+is1,i2+is2,i3,n2,t));
		  }
		  else
		  {
		    g2=-t1*X3P(is1,is2,i1-is1,i2-is2,i3,n1)
		       -t2*X3P(is1,is2,i1-is1,i2-is2,i3,n2);
		  }
		  deti=1./(rx(axis,0)*t2-rx(axis,1)*t1);
		  u(i1-is1,i2-is2,i3,n1)=( t2*g1-rx(axis,1)*g2)*deti;
		  u(i1-is1,i2-is2,i3,n2)=(-t1*g1+rx(axis,0)*g2)*deti;
		}
		else if( cg.mask[grid](i1,i2,i3) < 0 )
		{
		  // ---extrapolate outside interpolation points
		  u(i1-is1,i2-is2,i3,n1)=-X3P(is1,is2,i1-is1,i2-is2,i3,n1);
		  u(i1-is1,i2-is2,i3,n2)=-X3P(is1,is2,i1-is1,i2-is2,i3,n2);
		}
	      }
	    }
	  }
	}
      }
    }
  }
  else
  {
    //       ---3D
    for( axis=axis1; axis<numberOfDimensions; axis++ )
    {
      real dr = c.gridSpacing(axis);  // 1./(nrs(kd,2,k)-nrs(kd,1,k))
      for( int side=Start; side<=End; side++ )
      {
	if( c.boundaryCondition(side,axis) > 0 )
	{
          End1 = axis==axis1 ? Start : End;
          End2 = axis==axis2 ? Start : End;
          End3 = axis==axis3 ? Start : End;
          is1  = axis==axis1 ? 1-2*side : 0;
          is2  = axis==axis2 ? 1-2*side : 0;
          is3  = axis==axis3 ? 1-2*side : 0;

	  for( i3=c.indexRange(Start,axis3); i3<=c.gridIndexRange(End3,axis3); i3++ )
	  for( i2=c.indexRange(Start,axis2); i2<=c.gridIndexRange(End2,axis2); i2++ )
	  for( i1=c.indexRange(Start,axis1); i1<=c.gridIndexRange(End1,axis1); i1++ )
	  {
            if( cg[grid].mask(i1,i2,i3) > 0 )
	    {
	      a11=rx3(axis,0);
	      a12=rx3(axis,1);
	      a13=rx3(axis,2);
              g1=a11*u(i1+is1,i2+is2,i3+is3,n1)
                +a12*u(i1+is1,i2+is2,i3+is3,n2)
		+a13*u(i1+is1,i2+is2,i3+is3,n3);
              if( axis==axis1 )
                g1+=is1*2.*dr*(rx3(1,0)*us2(i1,i2,i3,n1) 
                              +rx3(1,1)*us2(i1,i2,i3,n2)
                              +rx3(1,2)*us2(i1,i2,i3,n3)
                              +rx3(2,0)*ut2(i1,i2,i3,n1)
                              +rx3(2,1)*ut2(i1,i2,i3,n2)
		 	      +rx3(2,2)*ut2(i1,i2,i3,n3));
              else if( axis==axis2 )
                g1+=is2*2.*dr*(rx3(0,0)*ur2(i1,i2,i3,n1) 
                              +rx3(0,1)*ur2(i1,i2,i3,n2)
                              +rx3(0,2)*ur2(i1,i2,i3,n3)
                              +rx3(2,0)*ut2(i1,i2,i3,n1)
                              +rx3(2,1)*ut2(i1,i2,i3,n2)
		  	      +rx3(2,2)*ut2(i1,i2,i3,n3));
              else
                g1+=is3*2.*dr*(rx3(0,0)*ur2(i1,i2,i3,n1) 
                              +rx3(0,1)*ur2(i1,i2,i3,n2)
                              +rx3(0,2)*ur2(i1,i2,i3,n3)
                              +rx3(1,0)*us2(i1,i2,i3,n1)
                              +rx3(1,1)*us2(i1,i2,i3,n2)
			      +rx3(1,2)*us2(i1,i2,i3,n3));
	      
              //  ...Get tangents
              //  ..get tangent vector(s)
	      ap1=(axis+1) % numberOfDimensions;
	      ap2=(axis+2) % numberOfDimensions;
	      ap3=(axis+3) % numberOfDimensions;
	      a21=rx3(ap2,1)*rx3(ap3,2)-rx3(ap2,2)*rx3(ap3,1);
	      a22=rx3(ap2,2)*rx3(ap3,0)-rx3(ap2,0)*rx3(ap3,2);
	      a23=rx3(ap2,0)*rx3(ap3,1)-rx3(ap2,1)*rx3(ap3,0);
	      a31=rx3(ap3,1)*rx3(ap1,2)-rx3(ap3,2)*rx3(ap1,1);
	      a32=rx3(ap3,2)*rx3(ap1,0)-rx3(ap3,0)*rx3(ap1,2);
	      a33=rx3(ap3,0)*rx3(ap1,1)-rx3(ap3,1)*rx3(ap1,0);
              if( c.boundaryCondition(side,axis)==5 || c.boundaryCondition(side,axis)==12 )
	      {
		//..slip wall, set (t.u).n=0
		g2=a21*u(i1+is1,i2+is2,i3+is3,n1)
		  +a22*u(i1+is1,i2+is2,i3+is3,n2)
		  +a23*u(i1+is1,i2+is2,i3+is3,n3);
		
		g3=a31*u(i1+is1,i2+is2,i3+is3,n1)
		  +a32*u(i1+is1,i2+is2,i3+is3,n2)
		  +a33*u(i1+is1,i2+is2,i3+is3,n3);
		
		if( twilightZoneFlow )
		{
		  g2+=a21*((*e)(i1-is1,i2-is2,i3-is3,n1,t)-(*e)(i1+is1,i2+is2,i3+is3,n1,t))
		     +a22*((*e)(i1-is1,i2-is2,i3-is3,n2,t)-(*e)(i1+is1,i2+is2,i3+is3,n2,t))
		     +a23*((*e)(i1-is1,i2-is2,i3-is3,n3,t)-(*e)(i1+is1,i2+is2,i3+is3,n3,t));
		  g3+=a31*((*e)(i1-is1,i2-is2,i3-is3,n1,t)-(*e)(i1+is1,i2+is2,i3+is3,n1,t))
		     +a32*((*e)(i1-is1,i2-is2,i3-is3,n2,t)-(*e)(i1+is1,i2+is2,i3+is3,n2,t))
		     +a33*((*e)(i1-is1,i2-is2,i3-is3,n3,t)-(*e)(i1+is1,i2+is2,i3+is3,n3,t));
		}
		else
		{
		  g2=-a21*X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n1)
		     -a22*X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n2)
		     -a23*X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n3);
		  g3=-a31*X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n1)
		     -a32*X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n2)
		     -a33*X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n3);
		}
		//   ...solve the equations
		deti=1./(
			 a11*(a22*a33-a32*a23)
			 +a12*(a23*a31-a33*a21)
			 +a13*(a21*a32-a31*a22) );
		
		u(i1-is1,i2-is2,i3-is3,n1)=(
					    g1 *(a22*a33-a32*a23)
					    +a12*(a23*g3 -a33*g2 )
					    +a13*(g2 *a32-g3 *a22) )*deti;
		
		u(i1-is1,i2-is2,i3-is3,n2)=(
					    a11*(g2 *a33-g3 *a23)
					    +g1 *(a23*a31-a33*a21)
					    +a13*(a21*g3 -a31*g2 ) )*deti;
		
		u(i1-is1,i2-is2,i3-is3,n3)=(
					    a11*(a22*g3 -a32*g2 )
					    +a12*(g2 *a31-g3 *a21)
					    +g1 *(a21*a32-a31*a22) )*deti;
	      }
              else if( cg[grid].mask(i1,i2,i3) < 0 )
	      {
		// ---extrapolate outside interpolation points
		u(i1-is1,i2-is2,i3-is3,n1)=-X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n1);
		u(i1-is1,i2-is2,i3-is3,n2)=-X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n2);
		u(i1-is1,i2-is2,i3-is3,n3)=-X33P(is1,is2,is3,i1-is1,i2-is2,i3-is3,n3);

	      }
	    }
	  }
	}
      }
    }
  }
  fixBoundaryCorners(u,c);

}

void BCmgf::
fixBoundaryCorners( realMappedGridFunction & u, MappedGrid & c )
{
  //======================================================================
  //
  // This is a fix-up routine to swap periodic edges and get the solution
  // at corners.
  //
  //======================================================================
  //c.......start statement functions
  //c     ...extrapolate velocity 3rd order
  //      ux3(n1,n2,n3,i1,i2,i3,kd)=
  //     &   + 3.*u(i1+  n1,i2+  n2,i3+  n3,kd)
  //     &   - 3.*u(i1+2*n1,i2+2*n2,i3+2*n3,kd)
  //     &   +    u(i1+3*n1,i2+3*n2,i3+3*n3,kd)
  //c.......end statement functions

  //     ---Fix periodic edges
  u.periodicUpdate();
  

  //     ---when two (or more) adjacent faces have boundary conditions
  //        we set the values on the fictitous line (or vertex)
  //        that is outside both faces ( points marked + below)
  //
  //                  +                +
  //                    --------------
  //                    |            |
  //                    |            |
  //

  int side1,side2,side3,is1,is2,is3,i1,i2,i3,n;
  

  Index I1=Range(c.indexRange(Start,axis1),c.indexRange(End,axis1));
  Index I2=Range(c.indexRange(Start,axis2),c.indexRange(End,axis2));
  Index I3=Range(c.indexRange(Start,axis3),c.indexRange(End,axis3));
  Index N(0,numberOfComponents);

  //         ---extrapolate edges---
  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis2) )
  {
    //       ...Do the four edges parallel to i3
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      i1=c.indexRange(side1,axis1);
      for( side2=Start; side2<=End; side2++ )
      {
        is2=1-2*side2;
        i2=c.indexRange(side2,axis2);
// ***        u(i1-is1,i2-is2,I3,N)=UX3(is1,is2,0,i1-is1,i2-is2,I3,N);
        for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
        for( n=N.getBase(); n<=N.getBound(); n++ )
          u(i1-is1,i2-is2,i3,n)=UX3(is1,is2,0,i1-is1,i2-is2,i3,n);
      }
    }
  }
 
  if( numberOfDimensions==2 ) return;

  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis3) )
  {
    //       ...Do the four edges parallel to i2
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      i1=c.indexRange(side1,axis1);
      for( side3=Start; side2<=End; side3++ )
      {
        is3=1-2*side3;
        i3=c.indexRange(side3,axis3);
        u(i1-is1,I2,i3-is3,N)=UX3(is1,0,is3,i1-is1,I2,i3-is3,N);
      }
    }
  }
  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis3) )
  {
    //       ...Do the four edges parallel to i1
    for( side2=Start; side2=End; side2++ )
    {
      is2=1-2*side2;
      i2=c.indexRange(side2,axis2);
      for( side3=Start; side2<=End; side3++ )
      {
        is3=1-2*side3;
        i3=c.indexRange(side3,axis3);
        u(I1,i2-is2,i3-is3,N)=UX3(0,is2,is3,I1,i2-is2,i3-is3,N);
      }
    }
  }

  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis2) )
  {
    //       ...Do the four edges parallel to i3
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      i1=c.indexRange(side1,axis1);
      for( side2=Start; side2=End; side2++ )
      {
        is2=1-2*side2;
        i2=c.indexRange(side2,axis2);
        for( side3=Start; side2<=End; side3++ )
        {
          is3=1-2*side3;
          i3=c.indexRange(side3,axis3);
          u(i1-is1,i2-is2,i3-is3,N)=UX3(is1,is2,is3,i1-is1,i2-is2,i3-is3,N);
	}
      }
    }
  }

}
