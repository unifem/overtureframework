#include "Oges.h"
#include "SparseRep.h"
#include <float.h>

#define RSXY(i1,i2,i3,m,n) rsxy(i1,i2,i3,m+numberOfDimensions*(n))
static inline real 
weightedArea( const int numberOfDimensions, const int i1, const int i2, const int i3, RealDistributedArray & u, 
  const RealDistributedArray & rsxy, const RealArray & gridSpacing )
{
  return numberOfDimensions==2 ? 
     ( (RSXY(i1,i2,i3,axis1,axis1)*RSXY(i1,i2,i3,axis2,axis2)-
        RSXY(i1,i2,i3,axis1,axis2)*RSXY(i1,i2,i3,axis2,axis1))
       *u(i1,i2,i3)/(gridSpacing(axis1)*gridSpacing(axis2)))
   : ( numberOfDimensions==3 ?
     ( RSXY(i1,i2,i3,axis1,axis1)*
          (RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis2))
     -RSXY(i1,i2,i3,axis1,axis2)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis1))
     +RSXY(i1,i2,i3,axis1,axis3)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis2)
          -RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis1)) )
       *u(i1,i2,i3)/(gridSpacing(axis1)*gridSpacing(axis2)*gridSpacing(axis3))
      : 
         RSXY(i1,i2,i3,axis1,axis1)*u(i1,i2,i3)/gridSpacing(axis1)
      );
}	 

inline real 
jacobian( const int numberOfDimensions, const int i1, const int i2, const int i3, const RealDistributedArray & rsxy )
{
  return numberOfDimensions==2 ? 
     ( (RSXY(i1,i2,i3,axis1,axis1)*RSXY(i1,i2,i3,axis2,axis2)-
        RSXY(i1,i2,i3,axis1,axis2)*RSXY(i1,i2,i3,axis2,axis1)))
   :
     ( RSXY(i1,i2,i3,axis1,axis1)*
          (RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis2))
     -RSXY(i1,i2,i3,axis1,axis2)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis3)
          -RSXY(i1,i2,i3,axis2,axis3)*RSXY(i1,i2,i3,axis3,axis1))
     +RSXY(i1,i2,i3,axis1,axis3)*
          (RSXY(i1,i2,i3,axis2,axis1)*RSXY(i1,i2,i3,axis3,axis2)
	  -RSXY(i1,i2,i3,axis2,axis2)*RSXY(i1,i2,i3,axis3,axis1)) );
}

//===========================================================================
// Compute weighted area =  jacobian/dr*u
//===========================================================================

void Oges::
scaleIntegrationCoefficients( realCompositeGridFunction & u )
{

  //======================================================================
  // Composite Grid Utility Routine:
  //
  //      Scale the Integration Coefficients
  //      ----------------------------------
  //
  //  Purpose :
  //     Determine the integration weights from the left null vector
  //     to the matrix formed by discretizing
  //          A: {     Del u =
  //             {   + du/dn =
  //
  //     This routine determines how to scale the left null vector
  //     so that it's entries will become the weights needed to
  //     integrate a function on the entire domain or on the boundary
  //     of the domain. Use routine CGIF, for example, to integrate
  //     a grid function.
  //
  //     NOTE: First call CGES to determine the left null vector of
  //           the matrix A. Pass this left null vector to this
  //           routine in order to scale the null vector.
  //
  // Input
  //   u  : the solution to the Neumann problem computed from oges
  //
  // Output -
  //   u  : u now contains the weights needed to integrate a function
  //         over the entire domain or over the boundary of the domain
  //         Use this as the input, qogic, to CGIF to integrate a
  //         function (for example).
  //
  //  Remarks:
  //   Scale the elements of the null vector so that they become
  //        the integration coefficients
  //
  //   Method -
  //    To scale the null vector we need one extra piece of information.
  //    We try two approaches:
  //      (I) Look for a region on the grid where the ratio of
  //          u(i1,i2,i3,grid) to the cell area is constant. In such a
  //          region we expect that u(i1,i2,i3,grid) should be equal
  //          to the cell area - this determines the scaling.
  //      (II) Look for a side of a single grid  which forms the
  //           entire side of the physical domain (ie without any
  //           interpolation points.) For such a side we can compute
  //           the surface area accurately (beacuse there are no
  //           overlapping areas) and thus we can determine
  //           the scaling factor.
  //
  // ======================================================================

  if( Oges::debug & 16 )
    cout << "Entering scaleIntegrationCoefficients..." << endl;
  
  
  // set default values to impossible
  int undefinedValue=INT_MIN;
  int i10=undefinedValue, i20=i10, i30=i10, grid0=0;
  int ok=FALSE;
  
  IntegerArray iv(3);   
  IntegerArray ivb(3);
  IntegerArray ma(3,3);  
  IntegerArray ivm(3);   
  IntegerArray iv1m(3);  

  IntegerArray saok(2,3,numberOfGrids);
  RealArray area(numberOfGrids);   
  RealArray sarea(2,3,numberOfGrids);   
  RealArray sarea1(2,3,numberOfGrids);  
  RealArray sareae(2,3,numberOfGrids);    

  area=0.;
  sarea=0.;
  sarea1=0.;
  sareae=0.;

  real dwjmin=1.;
  real wj0=0.;
  int grid,axis,side,axisb,sideb;
  real wj,dwj,wjm,ds,dse;
  
  numberOfDimensions=cg.numberOfDimensions();

  for( grid=0; grid<numberOfGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    IntegerDistributedArray & classifyX =  coeff[grid].sparse->classify;

    for( axis=axis1; axis<numberOfDimensions; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
        //           ... saok = .true. if all points on a BC side are
        //               used for the BC (and not interpolated)
        //               These are the sides that we can get a good
        //               approximation to the surface area
        //           ...initialize saok
        saok(side,axis,grid)= c.boundaryCondition()(side,axis) > 0;       
      }
    }

    for( int i3=c.dimension()(Start,axis3); i3<=c.dimension()(End,axis3); i3++ )
    {
      iv(axis3)=i3;
      for( int i2=c.dimension()(Start,axis2); i2<=c.dimension()(End,axis2); i2++ )
      {
        iv(axis2)=i2;
        for( int i1=c.dimension()(Start,axis1); i1<=c.dimension()(End,axis1); i1++ )
        {
          iv(axis1)=i1;
          if( classifyX(i1,i2,i3)==SparseRepForMGF::interior || classifyX(i1,i2,i3)==SparseRepForMGF::boundary )
	  {
	    //   ...interior point
            area(grid)+=u[grid](i1,i2,i3);

	    // Look for the point where wj is nearly constant
            // wj = det|dr/dx|/dr * u

            wj = weightedArea(numberOfDimensions, i1,i2,i3,u[grid],c.inverseVertexDerivative(), c.gridSpacing() );  
            // if( wj <= 0. )
	    //  cout << "CGICSC:ERROR jac*weight<0 " << " :weight=" << u[grid](i1,i2,i3) << endl;

            ok= wj != 0.;
	    dwj=0.;
            int m3a = numberOfDimensions<3 ? 0 : -1;
            int m3b = numberOfDimensions<3 ? 0 : +1;
            int m2a = numberOfDimensions<2 ? 0 : -1;
            int m2b = numberOfDimensions<2 ? 0 : +1;
	    for( int m3=m3a; m3<=m3b && ok; m3++ )
	    {
	      for( int m2=m2a; m2<=m2b && ok; m2++ )
	      {
		for( int m1=-1; m1<=1 && ok ; m1++ )
		{
		  if( cg[grid].mask()(i1+m1,i2+m2,i3+m3) > 0 )
		  {
                    wjm = weightedArea(numberOfDimensions, i1+m1,i2+m2,i3+m3,u[grid],c.inverseVertexDerivative(), 
                                      c.gridSpacing() );  
                    dwj=max(dwj,fabs(wjm/wj-1.));
		  }
		  else
		  {
		    ok=FALSE;
		  }
		}
	      }
	    }
	  }
  	  else if( classifyX(i1,i2,i3)==SparseRepForMGF::ghost1 )
	  {
	    //  ...Boundary point
            // find ivb(axis) : point on nearest boundary
            // (sideb,axisb) : this is the boundary we are on             

            // change the sign of boundary weights because we solved with the BC u.n= instead of -u.n
            u[grid](i1,i2,i3)=-u[grid](i1,i2,i3);
	    
            for( axis=axis1; axis<=axis3; axis++)
	    {
              ivb(axis)=min(c.indexRange(End,axis),max(c.indexRange(Start,axis),iv(axis)));
              if( ivb(axis)!=iv(axis) )
	      {
		axisb=axis;
		sideb= ivb(axis)-iv(axis) > 0 ? Start : End;
	      }
	    }

	    sarea(sideb,axisb,grid)=sarea(sideb,axisb,grid)+u[grid](i1,i2,i3);
	    // Look for the point where wj=jacobian*u
            // is most nearly constant; jacobian=jacobian
            // on the boundary

            //  ...get arclength (or surface area in 3D)
            surfaceArea( ds,dse,ivb,axisb,c.inverseVertexDerivative(),
		        c.gridSpacing(),c.dimension() );
	    if( saok(sideb,axisb,grid) )
	    {
	      //      ...
              if( cg[grid].mask()(ivb(axis1),ivb(axis2),ivb(axis3)) <= 0 )
	      {
		// ...this side has some interpolation pts
                saok(sideb,axisb,grid)=FALSE;
	      }
	      else
	      {
		//  ...Compute an approximation to the surface area
                //     on this side; watch out for edges
                //     sfact = 1 (interior) 1/2 (edge) 1/4 (vertex)
                real sfact=1.;
		for( int axisTangent=0; axisTangent<=numberOfDimensions-2; axisTangent++ )
		{
		  int axisp=(axisb+axisTangent+1) % numberOfDimensions;
                  if( (ivb(axisp)==c.indexRange()(Start,axisp)  &&
                           c.boundaryCondition()(Start,axisp) > 0)  ||
                      (ivb(axisp)==c.indexRange()(End,axisp)  &&
                           c.boundaryCondition()(End,axisp) > 0) )
		  {
		    sfact*=.5;
		  }
		}
                // printf(" (i1,i2)=(%i,%i) sideb=%i, axisb=%i, axisp=%i ds=%e sfact=%e \n",
		//       i1,i2,sideb,axisb,(axisb+axisTangent+1) % numberOfDimensions,ds,sfact);
		
                sarea1(sideb,axisb,grid)=sarea1(sideb,axisb,grid)+ds*sfact;
                sareae(sideb,axisb,grid)=sareae(sideb,axisb,grid)+dse*sfact;
	      }
	    }
            if( ds != 0. )
	      wj=u[grid](iv(axis1),iv(axis2),iv(axis3))/ds;
	    else
	    {
	      cout << "CGICSC: WARNING ds=0" << endl;
              ok=FALSE;
	      continue;
	    }
            // *       write(1,9999) kd1,ks1,i1,i2,i3,k,wj
            // *  9999 format(' kd1,ks1,i1,i2,i3,k,wj =',2i3,3i5,i2,e12.4)
            ma(Start,axis1)=-1;  ma(End,  axis1)=+1;
            ma(Start,axis2)=numberOfDimensions>1 ? -1 : 0;
            ma(End,  axis2)=numberOfDimensions>1 ? +1 : 0;
            ma(Start,axis3)=numberOfDimensions>2 ? -1 : 0;
            ma(End,  axis3)=numberOfDimensions>2 ? +1 : 0;
            ma(Start,axisb)=0;   ma(End,  axisb)=0;
            ok=TRUE;
            dwj=0.;
	    
            for( int m3=ma(Start,axis3); m3<=ma(End,axis3) && ok; m3++ )
            for( int m2=ma(Start,axis2); m2<=ma(End,axis2) && ok; m2++ )
            for( int m1=ma(Start,axis1); m1<=ma(End,axis1) && ok; m1++ )
            {
              //   ** this is wrong at corners
              ivm(axis1)=iv(axis1)+m1;
              ivm(axis2)=iv(axis2)+m2;
              ivm(axis3)=iv(axis3)+m3;
              iv1m(axis1)=ivb(axis1)+m1;
              iv1m(axis2)=ivb(axis2)+m2;
              iv1m(axis3)=ivb(axis3)+m3;
              if( cg[grid].mask()(iv1m(axis1),iv1m(axis2),iv1m(axis3)) > 0 )
              {
                surfaceArea( ds,dse,iv1m,axisb,c.inverseVertexDerivative(),
		   	    c.gridSpacing(),c.dimension() );
		if( ds != 0. )
		{
		  wjm=u[grid](ivm(axis1),ivm(axis2),ivm(axis3))/ds;
                  dwj=max(dwj,fabs(wjm/wj-1.));
		}
		else
		{
		  cout << " CGICSC: WARNING ds=0 " << endl;
                }
              }
              else
              {
		if( cg[grid].mask()(iv1m(axis1),iv1m(axis2),iv1m(axis3)) < 0 )
                  saok(sideb,axisb,grid)=FALSE;
                ok=FALSE;
	      }
            }
	  }
          // *       if( ok ) write(1,9300) itype,ds,1./wj,dwj
          // *  9300 format(' CGICSC: itype=',i2,' ds,scale,dwj=',3e12.4)
	  if( ok &&  dwj < dwjmin )
	  {
	    dwjmin=dwj;
            wj0=wj;
            i10=ivb(axis1);
            i20=ivb(axis2);
            i30=ivb(axis3);
            grid0=grid;
	  }
	}
      }
    }
  }

  if( i10 !=undefinedValue ) // this means we have found a point to use for scaling
    ok=TRUE;

  if( ok )
  {
    //       ..Scale equations based on the point (i10,i20,i30,grid0)
    //
    //   If the point is an interior point then
    //
    //                            dr*ds*dt
    //             scale =  ----------------------
    //                      |r.x|*u(i10,i20,i30,grid0)
    //
    //  The scale is chosen so point (i10,i20,i30,grid0) has a weight
    //  equal to the cell area
    //
    real scale;
    if( wj0 != 0. )
      scale=1./wj0;
    else
    {
      cerr << "CGICSC: ERROR wj0=0 " << endl;
      exit(1);
    }
    
    if( Oges::debug & 16 )
      printf(" The best local scale factor is =%12.5e\n"
         " Found at point (i1,i2,i3,grid)=(%6i,%6i,%6i,%3i)\n"
         " estimated accuracy, dwjmin = %12.5e \n"
         "     ---Surface Area Estimates--- \n"
         " sarea*scale= (Sum of null vector weights)*scale \n"
         " estimated = approximate integral of surface area \n"
         " error = estimated error in `estimated' \n"
         " saok = Surface Area OK: this side has no interp. pts \n",
	   scale,i10,i20,i30,grid0,dwjmin);
    if( Oges::debug & 8 )
    {
      for( grid=0; grid<numberOfGrids; grid++ )
      {
        printf(" Grid=%3i,  area =%12.4e\n"
          "side axis   saok?  sarea*scale   estimated   est. error\n",grid,scale*area(grid));
        for( axis=axis1; axis<numberOfDimensions; axis++ )
	for( side=Start; side<=End; side++ )
	{
	  printf( " %3i%3i       %1i %12.4e %12.4e %12.4e\n",
                 side,axis,saok(side,axis,grid),scale*sarea(side,axis,grid),sarea1(side,axis,grid),
		 sareae(side,axis,grid));
	}
      }
    }
    
	  
    real saemin=1.;
    int axisMin,sideMin,gridMin;
    for( grid=0; grid<numberOfGrids; grid++ )
    for( axis=axis1; axis<numberOfDimensions; axis++ )
    for( side=Start; side<=End; side++ )
    {
      if( saok(side,axis,grid) &&  sareae(side,axis,grid) < saemin )
      {
	saemin=sareae(side,axis,grid);
        axisMin=axis;
        sideMin=side;
        gridMin=grid;
      }
    }
    
    if( saemin < 10.*dwjmin )
    {
      //  ...base scaling on this side
      scale=sarea1(sideMin,axisMin,gridMin)/sarea(sideMin,axisMin,gridMin);
      if( Oges::debug & 8 )
        printf(" ---Basing scale factor on the area of side side=%2i,axis=%2i,grid=%3i\n"
               "   New scale factor = %14.8e \n",sideMin,axisMin,gridMin,scale);
      if( ! TRUE )
      {
	cout << "Enter a different side to base scale on" << endl;
	cin >> sideMin >> axisMin >> gridMin;
	scale=sarea1(sideMin,axisMin,gridMin)/sarea(sideMin,axisMin,gridMin);
      }
    
      // output results again after the scaling has changed
      if( Oges::debug & 8 )
      {
        for( grid=0; grid<numberOfGrids; grid++ )
        {
          printf(" Grid=%3i,  area =%12.4e\n"
            "side axis   saok?  sarea*scale   estimated   est. error\n",grid,scale*area(grid));
          for( axis=axis1; axis<numberOfDimensions; axis++ )
	  for( side=Start; side<=End; side++ )
  	  {
	    printf( " %3i%3i       %1i %12.4e %12.4e %12.4e\n",
                 side,axis,saok(side,axis,grid),scale*sarea(side,axis,grid),sarea1(side,axis,grid),
		 sareae(side,axis,grid));
	  }
	}
      }
    }
    
      // *         write(*,*) 'Enter a better value k,kd,ks,val'
      // *         write(*,*) ' (k=0 none, kd=ks=0 enter area)'
      // *         read(*,*) k,kd,ks,val
      // *         if( k.ne.0 )then
      // *           if( kd.eq.0 )then
      // *             scale=val/area(grid)
      // *           else
      // *             scale=val/sarea(kd,ks,k)
      // *           end if
      // *           write(*,*) 'New scale value =',scale
      // *         end if

      //       --- now scale the coefficients ---
    real area0=0.;
    real sarea0=0.;
    
    Index I1,I2,I3;
    for( grid=0; grid<numberOfGrids; grid++ )
    {
      MappedGrid & c = cg[grid];
      IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;

      I1=Range(c.dimension()(Start,axis1),c.dimension()(End,axis1));
      I2=Range(c.dimension()(Start,axis2),c.dimension()(End,axis2));
      I3=Range(c.dimension()(Start,axis3),c.dimension()(End,axis3));
      u[grid]*=scale;   // scale weights
      where( classifyX(I1,I2,I3)==SparseRepForMGF::interior || classifyX(I1,I2,I3)==SparseRepForMGF::boundary )
        area0+=sum(u[grid](I1,I2,I3));
      where( classifyX(I1,I2,I3)==SparseRepForMGF::ghost1 )
        sarea0+=sum(u[grid](I1,I2,I3));
    }
    if( Oges::debug & 4 )
      printf(" CGICSC: total area(2D) or volume(3D) = %14.8e\n"
             " boundary length(2D) or surface area(3D) = %14.8e \n",area0,sarea0);
  }
  else
  {
    cout << "CGICSC: ERROR: Unable to scale equations" << endl;
  }

}




#define RSXYR(i1,i2,i3,r,x) (RSXY(i1+1,i2  ,i3  ,r,x)-RSXY(i1,i2,i3,r,x))
#define RSXYS(i1,i2,i3,r,x) (RSXY(i1  ,i2+1,i3  ,r,x)-RSXY(i1,i2,i3,r,x))
#define RSXYT(i1,i2,i3,r,x) (RSXY(i1  ,i2  ,i3+1,r,x)-RSXY(i1,i2,i3,r,x))

#define RX(m,n) rsxy(i1,i2,i3,m+numberOfDimensions*(n))

void Oges::
surfaceArea( real & ds, real & dse, IntegerArray & iv, int & axis, 
    const RealDistributedArray & rsxy, const RealArray & gridSpacing, const IntegerArray & dimension )
{
  
  //====================================================================
  // Compute the local surface area on side (kd,ks)
  //
  // Input
  //  iv(3),k : point
  //  kd,ks   : side
  // Output
  //  ds      : arclength (nd=2) or the surface area
  //  dse     : estimate for the error in the local arclength
  //====================================================================

  int i1=iv(axis1);
  int i2=iv(axis2);
  int i3=iv(axis3);

  RealArray xr(3,3);

  if( numberOfDimensions==1 ) // ***** fix this
  {
    ds=1.;
    dse=0.;
  }
  else if( numberOfDimensions==2 )
  {
    //       ...ds = arclength on the side
    // **** fix kd,ks
    ds=SQRT(SQR(RSXY(i1,i2,i3,axis,axis1))+
            SQR(RSXY(i1,i2,i3,axis,axis2)))/fabs(jacobian(numberOfDimensions, i1,i2,i3,rsxy))
           *gridSpacing(1-axis);
    
    dse=0.;
    
    if( axis==axis1 && i2 < dimension(End,axis2) )
    {
      //         ...very poor error estimate:
      dse=( fabs(RSXYS(i1,i2,i3,axis,axis1))
           +fabs(RSXYS(i1,i2,i3,axis,axis2)) )*SQR(gridSpacing(axis2));
    }
    else if( axis==axis2 && i1 < dimension(End,axis1) )
    {
      dse=( fabs(RSXYR(i1,i2,i3,axis,axis1))
           +fabs(RSXYR(i1,i2,i3,axis,axis2)) )*SQR(gridSpacing(axis1));
    }
  }
  else if( numberOfDimensions==3 )
  {
    //       ...ds = surface area
    int axisp1=(axis+1  ) % numberOfDimensions;
    int axisp2=(axisp1+1) % numberOfDimensions;
    int axisp3=(axisp2+1) % numberOfDimensions;
    //  ...get xr from rx
    xr(axis1,axisp1)=RX(axisp2,axis2)*RX(axisp3,axis3)-RX(axisp2,axis3)*RX(axisp3,axis2);
    xr(axis2,axisp1)=RX(axisp2,axis3)*RX(axisp3,axis1)-RX(axisp2,axis1)*RX(axisp3,axis3);
    xr(axis3,axisp1)=RX(axisp2,axis1)*RX(axisp3,axis2)-RX(axisp2,axis2)*RX(axisp3,axis1);
    xr(axis1,axisp2)=RX(axisp3,axis2)*RX(axisp1,axis3)-RX(axisp3,axis3)*RX(axisp1,axis2);
    xr(axis2,axisp2)=RX(axisp3,axis3)*RX(axisp1,axis1)-RX(axisp3,axis1)*RX(axisp1,axis3);
    xr(axis3,axisp2)=RX(axisp3,axis1)*RX(axisp1,axis2)-RX(axisp3,axis2)*RX(axisp1,axis1);

    // ...get normal by a cross product
    real an1=xr(axis2,axisp1)*xr(axis3,axisp2)-xr(axis3,axisp1)*xr(axis2,axisp2);
    real an2=xr(axis3,axisp1)*xr(axis1,axisp2)-xr(axis1,axisp1)*xr(axis3,axisp2);
    real an3=xr(axis1,axisp1)*xr(axis2,axisp2)-xr(axis2,axisp1)*xr(axis1,axisp2);
    // ...surface area:
    ds=SQRT( SQR(an1)+SQR(an2)+SQR(an3) )/SQR(jacobian(numberOfDimensions, i1,i2,i3,rsxy))
      *gridSpacing(axisp1)*gridSpacing(axisp2);
      
    dse=0.;
    real dse1, dse2;
  
    if( axis==axis1 && i2 < dimension(End,axis2) && i3 < dimension(End,axis3) )
    {
      //  ...very poor error estimate:
      dse1=0.;
      dse2=0.;
      for( int dir1=0; dir1<numberOfDimensions; dir1++)
      {
	for( int dir2=0; dir2<numberOfDimensions; dir2++)
	{
          dse1=dse1+fabs(RSXYS(i1,i2,i3,dir1,dir2));
	  dse2=dse2+fabs(RSXYT(i1,i2,i3,dir1,dir2));
	}
      }
      dse=dse1*SQR(gridSpacing(axis2))+dse2*SQR(gridSpacing(axis3));
    }
    else if( axis==axis2 && i1 < dimension(End,axis1) 
                         && i3 < dimension(End,axis3) )
    {
      //         ...very poor error estimate:
      dse1=0.;
      dse2=0.;
      for( int dir1=0; dir1<numberOfDimensions; dir1++)
      {
	for( int dir2=0; dir2<numberOfDimensions; dir2++)
	{
          dse1=dse1+fabs(RSXYR(i1,i2,i3,dir1,dir2));
          dse2=dse2+fabs(RSXYT(i1,i2,i3,dir1,dir2));
	}
      }
      dse=dse1*SQR(gridSpacing(axis1))+dse2*SQR(gridSpacing(axis3));
    }
    else if( axis==axis3 && i1 < dimension(End,axis1)
                         && i2 < dimension(End,axis2) )
    {
      //         ...very poor error estimate:
      dse1=0.;
      dse2=0.;
      for( int dir1=0; dir1<numberOfDimensions; dir1++)
      {
	for( int dir2=0; dir2<numberOfDimensions; dir2++)
	{
          dse1=dse1+fabs(RSXYR(i1,i2,i3,dir1,dir2));
          dse2=dse2+fabs(RSXYS(i1,i2,i3,dir1,dir2));
	}
      }
      dse=dse1*SQR(gridSpacing(axis1))+dse2*SQR(gridSpacing(axis2));
    }
  }
  else
  {
    cerr << "DS23: Invalid value for numberOfDimensions! " << endl;
    exit(1);
  }
}

#undef RSXY
