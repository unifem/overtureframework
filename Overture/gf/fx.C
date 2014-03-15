



#define UX3(n1,n2,n3,i1,i2,i3,n)              \
     + 3.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     - 3.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +    u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)


//======================================================================
//
// This is a fix-up routine to swap periodic edges and get the solution
// at corners, including the ghost points outside corners.
//
//======================================================================
void MappedGridFiniteVolumeOperators::
fixBoundaryCorners( realMappedGridFunction & u )
{

  MappedGrid & c = mappedGrid;

  //     ---Fix periodic edges
  u.periodicUpdate();
  

  //     ---when two (or more) adjacent faces have boundary conditions
  //        we set the values on the fictitous line (or vertex)
  //        that is outside both faces ( points marked + below)
  //        We set values on all ghost points that lie outside the corner
  //
  //                + +                + +
  //                + +                + +
  //                    --------------
  //                    |            |
  //                    |            |
  //

  int side1,side2,side3,is1,is2,is3,i1,i2,i3,n;
  

  Index I1=Range(c.indexRange(Start,axis1),c.indexRange(End,axis1));
  Index I2=Range(c.indexRange(Start,axis2),c.indexRange(End,axis2));
  Index I3=Range(c.indexRange(Start,axis3),c.indexRange(End,axis3));
  Index N =Range(u.getComponentBase(0),u.getComponentBound(0));   // ********* Is this ok ?? *************

  //         ---extrapolate edges---
  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis2) )
  {
    //       ...Do the four edges parallel to i3
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      // * i1=c.indexRange(side1,axis1);
      // loop over all ghost points along i1:
      for( i1=c.indexRange(side1,axis1); i1!=c.dimension(side1,axis1); i1-=is1 )
      for( side2=Start; side2<=End; side2++ )
      {
        is2=1-2*side2;
        // * i2=c.indexRange(side2,axis2);
        // loop over all ghost points along i2:
        for( i2=c.indexRange(side2,axis2); i2!=c.dimension(side2,axis2); i2-=is2 )
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
      // * i1=c.indexRange(side1,axis1);
      for( i1=c.indexRange(side1,axis1); i1!=c.dimension(side1,axis1); i1-=is1 )
      for( side3=Start; side3<=End; side3++ )
      {
        is3=1-2*side3;
        // * i3=c.indexRange(side3,axis3);
        for( i3=c.indexRange(side3,axis3); i3!=c.dimension(side3,axis3); i3-=is3 )
          u(i1-is1,I2,i3-is3,N)=UX3(is1,0,is3,i1-is1,I2,i3-is3,N);
      }
    }
  }
  if( !c.isPeriodic(axis1) && !c.isPeriodic(axis3) )
  {
    //       ...Do the four edges parallel to i1
    for( side2=Start; side2<=End; side2++ )
    {
      is2=1-2*side2;
      // * i2=c.indexRange(side2,axis2);
      for( i2=c.indexRange(side2,axis2); i2!=c.dimension(side2,axis2); i2-=is2 )
      for( side3=Start; side3<=End; side3++ )
      {
        is3=1-2*side3;
        // * i3=c.indexRange(side3,axis3);
        for( i3=c.indexRange(side3,axis3); i3!=c.dimension(side3,axis3); i3-=is3 )
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
      // * i1=c.indexRange(side1,axis1);
      for( i1=c.indexRange(side1,axis1); i1!=c.dimension(side1,axis1); i1-=is1 )
      for( side2=Start; side2<=End; side2++ )
      {
        is2=1-2*side2;
        // * i2=c.indexRange(side2,axis2);
        for( i2=c.indexRange(side2,axis2); i2!=c.dimension(side2,axis2); i2-=is2 )
        for( side3=Start; side3<=End; side3++ )
        {
          is3=1-2*side3;
          // * i3=c.indexRange(side3,axis3);
          for( i3=c.indexRange(side3,axis3); i3!=c.dimension(side3,axis3); i3-=is3 )
            u(i1-is1,i2-is2,i3-is3,N)=UX3(is1,is2,is3,i1-is1,i2-is2,i3-is3,N);
	}
      }
    }
  }

}

