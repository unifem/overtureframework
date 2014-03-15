#include "Maxwell.h"
#include "display.h"
#include "CompositeGridOperators.h"


// // =============================================================================
// //! Advance on a rectangular grid. Yee scheme.
// // =============================================================================
// void Maxwell::
// advanceR( int current, real t, real dt, real *dx, realMappedGridFunction *field )
// {
//   MappedGrid & mg = *(field[0].getMappedGrid());
//   Index I1,I2,I3;
//   getIndex(mg.gridIndexRange(),I1,I2,I3);
  
//   int next = (current+1) % numberOfTimeLevels;
  
//   realArray & u = field[current];
//   realArray & un =field[next];

//   // 2D TEz mode:
//   //   (Ex).t = (1/eps)*[  (Hz).y ]
//   //   (Ey).t = (1/eps)*[ -(Hz).x ]
//   //   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

//   //              
//   //              -->
//   //          X----Ey----X 
//   //          |          |
//   //          |          |  ^
//   //          Ey   Hz    Ey |
//   //          |          |  |
//   //          |          |
//   //          X----Ex-----
//   //              -->
//   //           

//     // u(i1,i2,i3,ex) = Ex(i1+1/2,i2,i3,t-dt/2)
//     // un(i1,i2,i3,ex)= Ex(            ,t+dt/2)
//     // u(i1,i2,i3,hz) = Hz(i1+1/2,i2+1/2,i3+1/2,t)
      
//   un(I1,I2,I3,ex)=u(I1,I2,I3,ex) + (dt/(eps*dx[1]))*( u(I1,I2,I3,hz)-u(I1,I2-1,I3,hz) );
//   un(I1,I2,I3,ey)=u(I1,I2,I3,ey) - (dt/(eps*dx[0]))*( u(I1,I2,I3,hz)-u(I1-1,I2,I3,hz) );
    
//   int option=1;
//   int grid=0;  // **********
//   assignBoundaryConditions( option, grid, t, dt, field[next], field[current],current );
//   field[next].periodicUpdate(Range(ex,ey));

//   un(I1,I2,I3,hz)=u(I1,I2,I3,hz) + ((dt/(mu*dx[1]))*( un(I1,I2+1,I3,ex)-un(I1,I2,I3,ex) )-
// 				    (dt/(mu*dx[0]))*( un(I1+1,I2,I3,ey)-un(I1,I2,I3,ey) ));

//   option=2;
//   assignBoundaryConditions( option, grid, t, dt, field[next], field[current], current );
//   field[next].periodicUpdate(Range(hz,hz));
// }


// =============================================================================
//! Advance on a curvilinear or unstructured grid using the DSI scheme
// =============================================================================
void Maxwell::
advanceDSI( int current, real t, real dt)
{
  realMappedGridFunction grid_fields[5];

  int next = (current+1)%2;

  if ( !mgp )
    {
      CompositeGrid &cg = *cgp;

      for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
	{
	  MappedGrid &mg = cg[grid];
	  
	  if ( cgdissipation )
	    {
	      dissipation = & (*cgdissipation)[grid];
	    }

	  if ( e_cgdissipation )
	    e_dissipation = & (*e_cgdissipation)[grid];
	      
	  if ( mg.getGridType()==MappedGrid::structuredGrid )
	    {
	  
	      grid_fields[current].reference(getCGField(HField,current)[grid]);
	      grid_fields[next].reference(getCGField(HField,next)[grid]);
	      grid_fields[current+2].reference(getCGField(E100,current)[grid]);
	      grid_fields[next+2].reference(getCGField(E100,next)[grid]);
	      grid_fields[current+4].reference(getCGField(E010,current)[grid]);
	      grid_fields[next+4].reference(getCGField(E010,next)[grid]);

	      advanceC(current, t, dt, grid_fields);
	    }
	  else
	    {
	      grid_fields[current].reference(getCGField(HField,current)[grid]);
	      grid_fields[next].reference(getCGField(HField,next)[grid]);
	      grid_fields[current+2].reference(getCGField(EField,current)[grid]);
	      grid_fields[next+2].reference(getCGField(EField,next)[grid]);
	      //	      advanceUnstructuredDSI(current,t,dt,grid_fields);
	      if ( method==dsi )
		advanceUnstructuredDSI(current,t,dt,grid_fields);
	      else if ( method==dsiMatVec )
		advanceUnstructuredDSIMV(current,t,dt,grid_fields);
	    }
	  if  ( cgdissipation )
	    dissipation = 0;
	  if ( e_cgdissipation )
	    e_dissipation = 0;
	}
    }
  else
    {
      if ( mgp->getGridType()==MappedGrid::structuredGrid )
	advanceC(current,t,dt,fields);
      else if ( method==dsi )
	advanceUnstructuredDSI(current,t,dt,fields);
      else if ( method==dsiMatVec )
	advanceUnstructuredDSIMV(current,t,dt,fields);
    }
}

// =============================================================================
//! Advance on a curvilinear grid using the DSI scheme
// =============================================================================
void Maxwell::
advanceC( int current, real t, real dt, realMappedGridFunction *field )
{
  const int debug=0;

  realMappedGridFunction & uhf = field[current];
  realMappedGridFunction & ue0f = field[current+2];
  realMappedGridFunction & ue1f = field[current+4];
  int next = (current+1) % numberOfTimeLevels;
  realMappedGridFunction & uhnf =field[next];
  realMappedGridFunction & ue0nf = field[next+2];
  realMappedGridFunction & ue1nf = field[next+4];

  realArray & uh = uhf;
  realArray & ue0 = ue0f;
  realArray & ue1 = ue1f;

  realArray & uhn =uhnf;
  realArray & ue0n = ue0nf;
  realArray & ue1n = ue1nf;

  MappedGrid & mg = *(field[0].getMappedGrid());
  const realArray & center = mg.center();
  
  Range all;
  Index I1,I2,I3;
  Index J1,J2,J3;
  Index K1,K2,K3;

  getIndex(mg.gridIndexRange(),I1,I2,I3);
  getIndex(mg.gridIndexRange(),J1,J2,J3,1);  // include 1 ghost line
  getIndex(mg.gridIndexRange(),K1,K2,K3,2);  // include 2 ghost line
  K1=Range(K1.getBase(),K1.getBound()-1);
  K2=Range(K2.getBase(),K2.getBound()-1);
  
  

  const realArray & x = center(all,all,all,0);
  const realArray & y = center(all,all,all,1);

  // cell centres:
  realArray xc(K1,K2,K3),yc(K1,K2,K3);
  xc(K1,K2,K3)=.25*(x(K1,K2,K3)+x(K1+1,K2,K3)+x(K1,K2+1,K3)+x(K1+1,K2+1,K3));
  yc(K1,K2,K3)=.25*(y(K1,K2,K3)+y(K1+1,K2,K3)+y(K1,K2+1,K3)+y(K1+1,K2+1,K3));
  

  // 2D TEz mode:
  //   (Ex).t = (1/eps)*[  (Hz).y ]
  //   (Ey).t = (1/eps)*[ -(Hz).x ]
  //   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

  //              
  //              -->
  //          X----Ex----X 
  //          |          |
  //          |          |  ^
  //          Ey   Hz    Ey |
  //          |          |  |
  //          |          |
  //          X----Ex-----
  //              -->
  //           

    // u(i1,i2,i3,ex) = Ex(i1,i2+1/2,i3,t-dt/2)
    // un(i1,i2,i3,ex)= Ex(            ,t+dt/2)
    // u(i1,i2,i3,hz) = Hz(i1+1/2,i2+1/2,i3+1/2,t)
      

  // E.n : horizontal face of the primary cell is a vertical face of the dual
  //           Hz x(i1+1/2,i2+1/2)
  //           |
  //           |
  //           E-->n (Ex)
  //           |
  //           |
  //           Hz x(i1+1/2,i2-1/2)

//   realArray ds01,ds10;
//   ds01=SQRT( SQR(x(I1,I2+1,I3)-x(I1,I2,I3))+SQR(y(I1,I2+1,I3)-y(I1,I2,I3)) );
//   ds10=SQRT( SQR(x(I1+1,I2,I3)-x(I1,I2,I3))+SQR(y(I1+1,I2,I3)-y(I1,I2,I3)) );

  // en10 : component of E normal to face, normal is (dy,-dx)
  realArray nx10(J1,J2,J3),ny10(J1,J2,J3);
  nx10=yc(J1,J2)-yc(J1,J2-1);  //  dy    nx10(i1+1/2,i2)
  ny10=xc(J1,J2-1)-xc(J1,J2);  // -dx    ny10(i1+1/2,i2)
  
  realArray en10(J1,J2,J3), diss;
  en10(J1,J2,J3) = ue0(J1,J2,J3,0)*nx10+ue0(J1,J2,J3,1)*ny10;   // en10(i1+1/2,i2)

  if( debug ) 
  {
    
    display(ue0(J1,J2,J3,0),"u(J1,J2,J3,ex10) before advance","%8.5f");
    display(ue0(J1,J2,J3,1),"u(J1,J2,J3,ey10) before advance","%8.5f");
    display(ue1(J1,J2,J3,0),"u(J1,J2,J3,ex01) before advance","%8.5f");
    display(ue1(J1,J2,J3,1),"u(J1,J2,J3,ey01) before advance","%8.5f");
    display(en10(J1,J2,J3),"en10(J1,J2,J3) before advance","%8.5f");
  }
  
  // advance the normal component (the "face" for Stokes theorem extends along the z-axis)
  bool useNew=true;
  if( !useNew && artificialDissipation>0. )
  {
    diss.redim(I1,I2,I3);
    diss(I1,I2,I3)=-4.*en10(I1,I2,I3)+en10(I1+1,I2,I3)+en10(I1-1,I2,I3)+en10(I1,I2+1,I3)+en10(I1,I2-1,I3);
  }
  
  if( orderOfAccuracyInTime==2 )
  {
    en10(J1,J2,J3)=en10(J1,J2,J3) + (dt/(eps))*(uh(J1,J2,J3)-uh(J1,J2-1,J3));  // note scaling
  }
  else if( orderOfAccuracyInTime==3 )
  {
    // Third order ABS3 scheme.

    const real c0=(25./24.)*dt/eps, c1=(-1./12.)*dt/eps, c2=(1./24.)*dt/eps;
    const int m0=currentFn, m1=(m0+1)%numberOfFunctions;
    
    // printF("3rd order ABS method: t=%8.2e, m0,m1=%i,%i\n",t,m0,m1);    

    en10(J1,J2,J3)=en10(J1,J2,J3) + 
      ( c0*(     uh(J1,J2,J3)-     uh(J1,J2-1,J3))+
	c1*(fn[m0](J1,J2,J3,hz11)-fn[m0](J1,J2-1,J3,hz11))+
	c2*(fn[m1](J1,J2,J3,hz11)-fn[m1](J1,J2-1,J3,hz11)) );

    // currentFn=m1; // done below
  }
  else
  {
    printF("ERROR: orderOfAccuracyInTime=%i\n",orderOfAccuracyInTime);
    Overture::abort();
  }
  
  if( !useNew && artificialDissipation>0. )
    en10(I1,I2,I3)+=(artificialDissipation*dt)*diss(I1,I2,I3);
 
  if( debug ) display(en10(J1,J2,J3),"en10(J1,J2,J3) after advance","%8.5f");

  // E.n : vertical face of the primary cell is a horizontal face of the dual
  // 
  //                ^n (Ey)
  //                |
  //     Hz---------E----------Hz
  // x(i1-1/2,i2+1/2)       x(i1+1/2,i2+1/2)
  // 

  realArray nx01(J1,J2,J3),ny01(J1,J2,J3);
  nx01=yc(J1-1,J2)-yc(J1  ,J2);  // -dy  nx01(i1,i2+1/2)
  ny01=xc(J1  ,J2)-xc(J1-1,J2);  // dx   ny01(i1,i2+1/2)

  realArray en01(J1,J2,J3);
  en01(J1,J2,J3) = ue1(J1,J2,J3,0)*nx01+ue1(J1,J2,J3,1)*ny01;  //  en01(i1,i2+1/2)


  if( debug ) display(en01(J1,J2,J3),"en01(J1,J2,J3) before advance","%7.4f");

  if( !useNew  && artificialDissipation>0. )
  {
    diss(I1,I2,I3)=-4.*en01(I1,I2,I3)+en01(I1+1,I2,I3)+en01(I1-1,I2,I3)+en01(I1,I2+1,I3)+en01(I1,I2-1,I3);
  }

  // advance the normal component.
  if( orderOfAccuracyInTime==2 )
  {
    en01(J1,J2,J3)=en01(J1,J2,J3) - (dt/(eps))*(uh(J1,J2,J3)-uh(J1-1,J2,J3));
  }
  else if( orderOfAccuracyInTime==3 )
  {
    // Third order ABS3 scheme.

    const real c0=(25./24.)*dt/eps, c1=(-1./12.)*dt/eps, c2=(1./24.)*dt/eps;
    const int m0=currentFn, m1=(m0+1)%numberOfFunctions;
    
    en01(J1,J2,J3)=en01(J1,J2,J3) - 
      ( c0*(     uh(J1,J2,J3)-     uh(J1-1,J2,J3))+
	c1*(fn[m0](J1,J2,J3,hz11)-fn[m0](J1-1,J2,J3,hz11))+
	c2*(fn[m1](J1,J2,J3,hz11)-fn[m1](J1-1,J2,J3,hz11)) );

    // currentFn=m1; // done below
  }
  else
  {
    printF("ERROR: orderOfAccuracyInTime=%i\n",orderOfAccuracyInTime);
    Overture::abort();
  }


  if( !useNew && artificialDissipation>0. )
    en01(I1,I2,I3)+=(artificialDissipation*dt)*diss(I1,I2,I3);

  if( debug ) display(en01(J1,J2,J3),"en01(J1,J2,J3) after advance","%7.4f");

  // Given E.n on the horizontal and vertical faces compute the vector E

  // Solve:
  //    [ nx10(I1  ,I2  ) ny10(I1  ,I2  ) ] [ Ex10 ] = [ en10(I1,I2) ] 
  //    [ nx01(I1+m,I2+n) ny01(I1+m,I2+n) ] [ Ey10 ]   [ en01(I1+m,I2+n) ]
  //
  // for m=0,1  n=-1,0

  realArray exa(J1,J2), eya(J1,J2), exb(J1,J2), eyb(J1,J2), det(I1,I2);
  
  exb(I1,I2)=0.;
  eyb(I1,I2)=0.;
  int m1,m2;
  for( m1=0; m1<=1; m1++ )
  {
    for( m2=-1; m2<=0; m2++ )
    {
    
      det=nx10(I1,I2)*ny01(I1+m1,I2+m2)-ny10(I1,I2)*nx01(I1+m1,I2+m2);

      exa(I1,I2)=(en10(I1,I2)      *ny01(I1+m1,I2+m2)-en01(I1+m1,I2+m2)*ny10(I1,I2))/det;
      eya(I1,I2)=(en01(I1+m1,I2+m2)*nx10(I1,I2)      -en10(I1,I2)      *nx01(I1+m1,I2+m2))/det;
  
      exb(I1,I2)+=exa(I1,I2);     // simple average for now
      eyb(I1,I2)+=eya(I1,I2);
    }
  }
  ue0n(I1,I2,I3,0)=exb(I1,I2)*.25;  // simple average
  ue0n(I1,I2,I3,1)=eyb(I1,I2)*.25;

  if( debug ) display(ue0n(I1,I2,I3,0),"un(I1,I2,I3,ex10)","%7.4f");
  if( debug ) display(ue0n(I1,I2,I3,1),"un(I1,I2,I3,ey10)","%7.4f");

  exb(I1,I2)=0.;
  eyb(I1,I2)=0.;
  for( m1=-1; m1<=0; m1++ )
  {
    for( m2=0; m2<=1; m2++ )
    {
    
      det=nx01(I1,I2)*ny10(I1+m1,I2+m2)-ny01(I1,I2)*nx10(I1+m1,I2+m2);

      exa(I1,I2)=(en01(I1,I2)      *ny10(I1+m1,I2+m2)-en10(I1+m1,I2+m2)*ny01(I1,I2))/det;
      eya(I1,I2)=(en10(I1+m1,I2+m2)*nx01(I1,I2)      -en01(I1,I2)      *nx10(I1+m1,I2+m2))/det;
  
      exb(I1,I2)+=exa(I1,I2);
      eyb(I1,I2)+=eya(I1,I2);
    }
  }
  ue1n(I1,I2,I3,0)=exb(I1,I2)*.25;
  ue1n(I1,I2,I3,1)=eyb(I1,I2)*.25;

    
  if( false && useNew && artificialDissipation>0. )
  {
    // kkc ex10 and ey01 are now in different arrays    addDissipation( current, t,dt,field, Range(ex10,ey01) );
    addDissipation( current+2, t,dt,&field[current+2], Range(0,1) );
    addDissipation( current+4, t,dt,&field[current+4], Range(0,1) );
  }

  int grid=0;
  //assignBoundaryConditions( option, grid, t, dt, field[next] );
  //  field[next].periodicUpdate(Range(ex10,ey01));
  int option=1;
  assignBoundaryConditions( option, grid, t, dt, ue0nf, ue0nf, current );
  option=0;
  assignBoundaryConditions( option, grid, t, dt, ue1nf, ue1nf, current );
  ue0nf.periodicUpdate(Range(ex,ey));
  ue1nf.periodicUpdate(Range(ex,ey));

  if( debug ) display(ue1n(I1,I2,I3,0),"un(I1,I2,I3,ex01)","%7.4f");
  if( debug ) display(ue1n(I1,I2,I3,1),"un(I1,I2,I3,ey01)","%7.4f");


//   un(I1,I2,I3,hz)=u(I1,I2,I3,hz) + ((dt/(mu*dx[1]))*( un(I1,I2+1,I3,ex)-un(I1,I2,I3,ex) )-
// 				    (dt/(mu*dx[0]))*( un(I1+1,I2,I3,ey)-un(I1,I2,I3,ey) ));

  // volume of the primary cell:
  // area of a polygon = +/- (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i }
  realArray & pArea = exb;
  pArea(I1,I2,I3)=( x(I1  ,I2  ,I3)*y(I1+1,I2  ,I3) - x(I1+1,I2  ,I3)*y(I1  ,I2  ,I3)+ 
                    x(I1+1,I2  ,I3)*y(I1+1,I2+1,I3) - x(I1+1,I2+1,I3)*y(I1+1,I2  ,I3)+ 
                    x(I1+1,I2+1,I3)*y(I1  ,I2+1,I3) - x(I1  ,I2+1,I3)*y(I1+1,I2+1,I3)+
                    x(I1  ,I2+1,I3)*y(I1  ,I2  ,I3) - x(I1  ,I2  ,I3)*y(I1  ,I2+1,I3)  )*(.5);

  if( orderOfAccuracyInTime==2 )
  {
    // E.s : vertical face:
    realArray & dh01 = exa, &dh10 = eya;
    dh01(I1,I2,I3)= (ue1n(I1,I2,I3,0)*(x(I1,I2+1,I3)-x(I1,I2,I3))+
		     ue1n(I1,I2,I3,1)*(y(I1,I2+1,I3)-y(I1,I2,I3)));
  
    // E.s : horizontal face
    dh10(I1,I2,I3)= (ue0n(I1,I2,I3,0)*(x(I1+1,I2,I3)-x(I1,I2,I3))+
		     ue0n(I1,I2,I3,1)*(y(I1+1,I2,I3)-y(I1,I2,I3)));

    uhn(I1,I2,I3)=uh(I1,I2,I3) + 
      (dt/mu)*( dh01(I1,I2,I3)-dh01(I1+1,I2,I3)+ dh10(I1,I2+1,I3)-dh10(I1,I2,I3))/pArea(I1,I2,I3);
  }
  else if( orderOfAccuracyInTime==3 )
  {
    // Third order ABS3 scheme.

    const real c0=(25./24.)*dt/mu, c1=(-1./12.)*dt/mu, c2=(1./24.)*dt/mu;
    const int m0=currentFn, m1=(m0+1)%numberOfFunctions;
    
    // E.s : vertical face:
    realArray & dh01 = exa, &dh10 = eya;
    dh01(I1,I2,I3)= ( (c0*ue1n(I1,I2,I3,ex)+c1*fn[m0](I1,I2,I3,ex01)+c2*fn[m1](I1,I2,I3,ex01))*(x(I1,I2+1,I3)-x(I1,I2,I3))+
		      (c0*ue1n(I1,I2,I3,ey)+c1*fn[m0](I1,I2,I3,ey01)+c2*fn[m1](I1,I2,I3,ey01))*(y(I1,I2+1,I3)-y(I1,I2,I3)));
  
    // E.s : horizontal face
    dh10(I1,I2,I3)= ( (c0*ue0n(I1,I2,I3,ex)+c1*fn[m0](I1,I2,I3,ex10)+c2*fn[m1](I1,I2,I3,ex10))*(x(I1+1,I2,I3)-x(I1,I2,I3))+
		      (c0*ue0n(I1,I2,I3,ey)+c1*fn[m0](I1,I2,I3,ey10)+c2*fn[m1](I1,I2,I3,ey10))*(y(I1+1,I2,I3)-y(I1,I2,I3)));


    uhn(I1,I2,I3)=uh(I1,I2,I3) + ( dh01(I1,I2,I3)-dh01(I1+1,I2,I3)+ dh10(I1,I2+1,I3)-dh10(I1,I2,I3))/pArea(I1,I2,I3);

    currentFn=m1;
  }
  else
  {
    printF("ERROR: orderOfAccuracyInTime=%i\n",orderOfAccuracyInTime);
    Overture::abort();
  }


  if( !useNew && artificialDissipation>0. )
  {
    uhn(I1,I2,I3)+= (artificialDissipation*dt)*(
      -4.*uhn(I1,I2,I3)+uhn(I1+1,I2,I3)+uhn(I1-1,I2,I3)+uhn(I1,I2+1,I3)+uhn(I1,I2-1,I3) );
  }
  
//   display(pArea(I1,I2,I3),"pArea(I1,I2,I3)","%7.4f");
//   display(dh01(I1,I2,I3),"dh01(I1,I2,I3) before periodic update","%7.4f");
//   display(dh10(I1,I2,I3),"dh10(I1,I2,I3) before periodic update","%7.4f");
  
  if( debug ) display(uhn(J1,J2,J3),"un(J1,J2,J3,hz11) before periodic update","%7.4f");
  

  if( useNew && artificialDissipation>0. )
  {
    // kkc 031231 hz is no longer in the same gf as e    addDissipation( current, t,dt,field,Range(hz11,hz11) );
    addDissipation( current, t,dt,field,Range(0,0) );
  }

  //  assignBoundaryConditions( option, grid, t, dt, field[next] );
  //  field[next].periodicUpdate(Range(hz11,hz11));
  option=2;
  assignBoundaryConditions( option, grid, t, dt, uhnf, uhnf, current );
  uhnf.periodicUpdate(Range(hz,hz));
  
  if( orderOfAccuracyInTime==3 )
  {
    fn[currentFn].reference(field[current]);  // save current values.
  }
  
//  display(un(J1,J2,J3,hz11),"un(J1,J2,J3,hz11) After periodic update","%7.4f");
}



//! Add in an artificial dissipation
/*!
  \param C (input) : apply to these components.
 */
int Maxwell::
addDissipation( int current, real t, real dt, realMappedGridFunction *field, const Range & C )
{
  if( artificialDissipation<=0. ) 
    return 0;
  

  realArray & u = field[current];
  int next = (current+1) % numberOfTimeLevels;
  realArray & un =field[next];

  MappedGrid & mg = *(field[0].getMappedGrid());
  
  Index I1,I2,I3;
  getIndex(mg.gridIndexRange(),I1,I2,I3);

  assert( dissipation!=NULL );
  realMappedGridFunction & diss = *dissipation;
  realArray & d = diss;

  int n;
  for( n=C.getBase(); n<=C.getBound(); n++ )
  {
    //    d(I1,I2,I3)=-4.*u(I1,I2,I3,n)+u(I1+1,I2,I3,n)+u(I1-1,I2,I3,n)+u(I1,I2+1,I3,n)+u(I1,I2-1,I3,n);

    d(I1,I2,I3)=(-8./3.)*u(I1,I2,I3,n)+
      (1./3.)*(u(I1+1,I2,I3,n)+u(I1-1,I2,I3,n)+u(I1,I2+1,I3,n)+u(I1,I2-1,I3,n)+
	       u(I1-1,I2-1,I3,n)+u(I1+1,I2-1,I3,n)+u(I1-1,I2+1,I3,n)+u(I1+1,I2+1,I3,n));

  
    if( orderOfArtificialDissipation==2 )
    {
      un(I1,I2,I3,n)+=(artificialDissipation*dt)*d(I1,I2,I3);
    }
    else
    {
      diss.periodicUpdate();

      //       d(I1,I2,I3)=-4.*d(I1,I2,I3)+d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3);
      d(I1,I2,I3)=(-8./3.)*d(I1,I2,I3)+
	(1./3.)*(d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3)+
		 d(I1-1,I2-1,I3)+d(I1+1,I2-1,I3)+d(I1-1,I2+1,I3)+d(I1+1,I2+1,I3));
      if( orderOfArtificialDissipation==4 )
      {
	// fourth-order dissipation
	un(I1,I2,I3,n)+=(-artificialDissipation*dt)*d(I1,I2,I3);
      }
      else if( orderOfArtificialDissipation==6 )
      {
	diss.periodicUpdate();

	// d(I1,I2,I3)=-4.*d(I1,I2,I3)+d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3);
	d(I1,I2,I3)=(-8./3.)*d(I1,I2,I3)+
	  (1./3.)*(d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3)+
		   d(I1-1,I2-1,I3)+d(I1+1,I2-1,I3)+d(I1-1,I2+1,I3)+d(I1+1,I2+1,I3));

	// sixth-order dissipation
	un(I1,I2,I3,n)+=(artificialDissipation*dt)*d(I1,I2,I3);
      }
      else
      {
	Overture::abort();
      }

      //      cout<<"component "<<n<<" min/max diss "<<min(d)<<"  "<<max(d)<<endl;
    }
    
  }
  
//  field[next].periodicUpdate();
//  PlotIt::contour(*Overture::getGraphicsInterface(), diss);

  return 0;
}


// =============================================================================
//! Advance on a curvilinear grid using the *new* DSI scheme
/*!
     Here we assume the grid is nearly orthogonal

 */
// =============================================================================
  void Maxwell::
    advanceNew( int current, real t, real dt, realMappedGridFunction *field )
  {
    // printF(" advanceNew: t=%e\n",t);
  
    const int debug=0;

    realArray & u = field[current];
    int next = (current+1) % numberOfTimeLevels;
    realArray & un =field[next];

    MappedGrid & mg = *(field[0].getMappedGrid());
    const realArray & center = mg.center();
  
    Range all;
    Index I1,I2,I3;
    Index J1,J2,J3;
    Index K1,K2,K3;

    getIndex(mg.gridIndexRange(),I1,I2,I3);
    getIndex(mg.gridIndexRange(),J1,J2,J3,1);  // include 1 ghost line
    getIndex(mg.gridIndexRange(),K1,K2,K3,2);  // include 2 ghost line
    K1=Range(K1.getBase(),K1.getBound()-1);
    K2=Range(K2.getBase(),K2.getBound()-1);
  
  

    const realArray & x = center(all,all,all,0);
    const realArray & y = center(all,all,all,1);

  // cell centres:
    realArray xc(K1,K2,K3),yc(K1,K2,K3);
    xc(K1,K2,K3)=.25*(x(K1,K2,K3)+x(K1+1,K2,K3)+x(K1,K2+1,K3)+x(K1+1,K2+1,K3));
    yc(K1,K2,K3)=.25*(y(K1,K2,K3)+y(K1+1,K2,K3)+y(K1,K2+1,K3)+y(K1+1,K2+1,K3));
  

    // 2D TEz mode:
    //   (Ex).t = (1/eps)*[  (Hz).y ]
    //   (Ey).t = (1/eps)*[ -(Hz).x ]
    //   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

    //              
    //              -->
    //          X----Ex----X 
    //          |          |
    //          |          |  ^
    //          Ey   Hz    Ey |
    //          |          |  |
    //          |          |
    //          X----Ex-----
    //              -->
    //           


  // E.n : horizontal face of the primary cell is a vertical face of the dual
  //           Hz x(i1+1/2,i2+1/2)
  //           |
  //           |
  //           E-->n (Ex)
  //           |
  //           |
  //           Hz x(i1+1/2,i2-1/2)

//   realArray ds01,ds10;
//   ds01=SQRT( SQR(x(I1,I2+1,I3)-x(I1,I2,I3))+SQR(y(I1,I2+1,I3)-y(I1,I2,I3)) );
//   ds10=SQRT( SQR(x(I1+1,I2,I3)-x(I1,I2,I3))+SQR(y(I1+1,I2,I3)-y(I1,I2,I3)) );

  // en10 : component of E normal to face, normal is (dy,-dx)
    realArray nx10(J1,J2,J3),ny10(J1,J2,J3);
    nx10=yc(J1,J2)-yc(J1,J2-1);  //  dy    nx10(i1+1/2,i2)
    ny10=xc(J1,J2-1)-xc(J1,J2);  // -dx    ny10(i1+1/2,i2)
  
    // use pv instead of nv
    realArray px10(J1,J2,J3),py10(J1,J2,J3);
    px10=x(J1+1,J2)-x(J1,J2);
    py10=y(J1+1,J2)-y(J1,J2);

    realArray p10Norm, nNorm;
    p10Norm=SQRT( SQR(px10(J1,J2))+SQR(py10(J1,J2)) );
    nNorm=SQRT( SQR(nx10(J1,J2))+SQR(ny10(J1,J2)) );

    realArray en10(J1,J2,J3);
    en10(J1,J2,J3) = u(J1,J2,J3,ex10)*px10+u(J1,J2,J3,ey10)*py10;   // en10(i1+1/2,i2)

    if( debug ) display(en10(J1,J2,J3),"en10(J1,J2,J3) before advance","%7.4f");

    // advance the normal component (the "face" for Stokes theorem extends along the z-axis)
  
    en10(J1,J2,J3)=en10(J1,J2,J3) + (dt/(eps))*(u(J1,J2,J3,hz11)-u(J1,J2-1,J3,hz11))*p10Norm/nNorm;

    if( debug ) display(en10(J1,J2,J3),"en10(J1,J2,J3) after advance","%7.4f");

  // E.n : vertical face of the primary cell is a horizontal face of the dual
  // 
  //                ^n (Ey)
  //                |
  //     Hz---------E----------Hz
  // x(i1-1/2,i2+1/2)       x(i1+1/2,i2+1/2)
  // 

    realArray nx01(J1,J2,J3),ny01(J1,J2,J3);
    nx01=yc(J1-1,J2)-yc(J1  ,J2);  // -dy  nx01(i1,i2+1/2)
    ny01=xc(J1  ,J2)-xc(J1-1,J2);  // dx   ny01(i1,i2+1/2)

    realArray px01(J1,J2,J3),py01(J1,J2,J3);
    px01=x(J1,J2+1)-x(J1,J2);
    py01=y(J1,J2+1)-y(J1,J2);

    realArray p01Norm;
    p01Norm=SQRT( SQR(px01(J1,J2))+SQR(py01(J1,J2)) );
    nNorm=SQRT( SQR(nx01(J1,J2))+SQR(ny01(J1,J2)) );

    realArray en01(J1,J2,J3);
    en01(J1,J2,J3) = u(J1,J2,J3,ex01)*px01+u(J1,J2,J3,ey01)*py01;  //  en01(i1,i2+1/2)

    if( debug ) display(en01(J1,J2,J3),"en01(J1,J2,J3) before advance","%7.4f");

  // advance the normal component.
    en01(J1,J2,J3)=en01(J1,J2,J3) - (dt/(eps))*(u(J1,J2,J3,hz11)-u(J1-1,J2,J3,hz11))*p01Norm/nNorm;
  
    if( debug ) display(en01(J1,J2,J3),"en01(J1,J2,J3) after advance","%7.4f");

  // Given E.n on the horizontal and vertical faces compute the vector E

  // Solve:
  //    [ nx10(I1  ,I2  ) ny10(I1  ,I2  ) ] [ Ex10 ] = [ en10(I1,I2) ] 
  //    [ nx01(I1+m,I2+n) ny01(I1+m,I2+n) ] [ Ey10 ]   [ en01(I1+m,I2+n) ]
  //
  // for m=0,1  n=-1,0

    realArray exa(J1,J2), eya(J1,J2), exb(J1,J2), eyb(J1,J2), det(I1,I2);
  
    exb(I1,I2)=0.;
    eyb(I1,I2)=0.;
    int m1,m2;
    for( m1=0; m1<=1; m1++ )
    {
      for( m2=-1; m2<=0; m2++ )
      {
    
	det=px10(I1,I2)*py01(I1+m1,I2+m2)-py10(I1,I2)*px01(I1+m1,I2+m2);

	exa(I1,I2)=(en10(I1,I2)      *py01(I1+m1,I2+m2)-en01(I1+m1,I2+m2)*py10(I1,I2))/det;
	eya(I1,I2)=(en01(I1+m1,I2+m2)*px10(I1,I2)      -en10(I1,I2)      *px01(I1+m1,I2+m2))/det;
  
	exb(I1,I2)+=exa(I1,I2);     // simple average for now
	eyb(I1,I2)+=eya(I1,I2);
      }
    }
    exb(I1,I2)*=.25;
    eyb(I1,I2)*=.25;
  
    // normalize p10
    realArray pxHat(I1,I2,I3),pyHat(I1,I2,I3);

    realArray dot;
    pxHat=px10(I1,I2)/p10Norm(I1,I2);
    pyHat=py10(I1,I2)/p10Norm(I1,I2);

  // subtract off the component in the direction of p10
    dot=exb(I1,I2)*pxHat(I1,I2)+eyb(I1,I2)*pyHat(I1,I2);
    exb(I1,I2)-=dot(I1,I2)*pxHat(I1,I2);
    eyb(I1,I2)-=dot(I1,I2)*pyHat(I1,I2);
  

    un(I1,I2,I3,ex10)=en10(I1,I2,I3)*pxHat(I1,I2)/p10Norm(I1,I2)+exb(I1,I2);
    un(I1,I2,I3,ey10)=en10(I1,I2,I3)*pyHat(I1,I2)/p10Norm(I1,I2)+eyb(I1,I2);

    exb(I1,I2)=0.;
    eyb(I1,I2)=0.;
    for( m1=-1; m1<=0; m1++ )
    {
      for( m2=0; m2<=1; m2++ )
      {
    
	det=px01(I1,I2)*py10(I1+m1,I2+m2)-py01(I1,I2)*px10(I1+m1,I2+m2);

	exa(I1,I2)=(en01(I1,I2)      *py10(I1+m1,I2+m2)-en10(I1+m1,I2+m2)*py01(I1,I2))/det;
	eya(I1,I2)=(en10(I1+m1,I2+m2)*px01(I1,I2)      -en01(I1,I2)      *px10(I1+m1,I2+m2))/det;
  
	exb(I1,I2)+=exa(I1,I2);
	eyb(I1,I2)+=eya(I1,I2);
      }
    }
    exb(I1,I2)*=.25;
  eyb(I1,I2)*=.25;
  
  // normalize p01
  pxHat=px01(I1,I2)/p01Norm(I1,I2);
  pyHat=py01(I1,I2)/p01Norm(I1,I2);

  // subtract off the component in the direction of p01
  dot=exb(I1,I2)*pxHat(I1,I2)+eyb(I1,I2)*pyHat(I1,I2);
  exb(I1,I2)-=dot(I1,I2)*pxHat(I1,I2);
  eyb(I1,I2)-=dot(I1,I2)*pyHat(I1,I2);
  

  un(I1,I2,I3,ex01)=en01(I1,I2,I3)*pxHat(I1,I2)/p01Norm(I1,I2)+exb(I1,I2);
  un(I1,I2,I3,ey01)=en01(I1,I2,I3)*pyHat(I1,I2)/p01Norm(I1,I2)+eyb(I1,I2);

  field[next].periodicUpdate(Range(ex10,ey01));

  if( debug ) display(un(I1,I2,I3,ex01),"un(I1,I2,I3,ex01)","%7.4f");
  if( debug ) display(un(I1,I2,I3,ey01),"un(I1,I2,I3,ey01)","%7.4f");


//   un(I1,I2,I3,hz)=u(I1,I2,I3,hz) + ((dt/(mu*dx[1]))*( un(I1,I2+1,I3,ex)-un(I1,I2,I3,ex) )-
// 				    (dt/(mu*dx[0]))*( un(I1+1,I2,I3,ey)-un(I1,I2,I3,ey) ));

  // E.s : vertical face:
  realArray & dh01 = exa, &dh10 = eya;
  dh01(I1,I2,I3)= (un(I1,I2,I3,ex01)*(x(I1,I2+1,I3)-x(I1,I2,I3))+
	           un(I1,I2,I3,ey01)*(y(I1,I2+1,I3)-y(I1,I2,I3)));
  
  // E.s : horizontal face
  dh10(I1,I2,I3)= (un(I1,I2,I3,ex10)*(x(I1+1,I2,I3)-x(I1,I2,I3))+
	           un(I1,I2,I3,ey10)*(y(I1+1,I2,I3)-y(I1,I2,I3)));

  // volume of the primary cell:
  // area of a polygon = +/- (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i }
  realArray & pArea = exb;
  pArea(I1,I2,I3)=( x(I1  ,I2  ,I3)*y(I1+1,I2  ,I3) - x(I1+1,I2  ,I3)*y(I1  ,I2  ,I3)+ 
                    x(I1+1,I2  ,I3)*y(I1+1,I2+1,I3) - x(I1+1,I2+1,I3)*y(I1+1,I2  ,I3)+ 
                    x(I1+1,I2+1,I3)*y(I1  ,I2+1,I3) - x(I1  ,I2+1,I3)*y(I1+1,I2+1,I3)+
                    x(I1  ,I2+1,I3)*y(I1  ,I2  ,I3) - x(I1  ,I2  ,I3)*y(I1  ,I2+1,I3)  )*(.5);

  un(I1,I2,I3,hz11)=u(I1,I2,I3,hz11) + 
    (dt/mu)*( dh01(I1,I2,I3)-dh01(I1+1,I2,I3)+ dh10(I1,I2+1,I3)-dh10(I1,I2,I3))/pArea(I1,I2,I3);
  
  
//   display(pArea(I1,I2,I3),"pArea(I1,I2,I3)","%7.4f");
//   display(dh01(I1,I2,I3),"dh01(I1,I2,I3) before periodic update","%7.4f");
//   display(dh10(I1,I2,I3),"dh10(I1,I2,I3) before periodic update","%7.4f");
  
//   display(un(J1,J2,J3,hz11),"un(J1,J2,J3,hz11) before periodic update","%7.4f");
  

  if( false && artificialDissipation>0. )
  {
    addDissipation( current, t,dt,field,Range(hz11,hz11) );
  }
  field[next].periodicUpdate(Range(hz11,hz11));
  

//  display(un(J1,J2,J3,hz11),"un(J1,J2,J3,hz11) After periodic update","%7.4f");
}



