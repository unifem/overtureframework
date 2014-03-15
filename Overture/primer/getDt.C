//===========================================================================================
//  
// This function is used by mappedGridExample6
// 
//===========================================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "ParallelUtility.h"

real
getDt(const real & cfl, 
      const real & a, 
      const real & b, 
      const real & nu, 
      MappedGrid & mg, 
      MappedGridOperators & op,
      const real alpha0 = -2.,
      const real beta0  = 1. )
//======================================================================================
// /Description:
//  Determine the time step for the convection diffusion equation in 2D
//      u_t + a u_x + b u_y = nu( u_xx + u_yy )
//  discretized with the mapping method. Scale the maximum allowable time
//  step by the factor cfl. the stability region is assumed to lie within the 
//  ellipse (x/alpha0)^2 + (y/beta0)^2 = 1
// /cfl (input): Scale the time step by this factor (cfl=1 should normally be stable)
// /a (input) : coefficient of u_x
// /b (input) : coefficient of u_y
// /nu (input) : coefficient of (u_xx+u_yy)
// /alpha0, beta0 (input) : parameters defining the ellipse for the stability region  
// ====================================================================================
{
  real dt=REAL_MAX;
  if( mg.isRectangular() )
  {
    // ***** rectangular grid *****
    real dx[3];
    mg.getDeltaX(dx);
    
    dt = cfl * pow(
	pow( fabs(a)*(1./(beta0*dx[0]))+fabs(b)*(1./beta0*dx[1]) , 2.)
	+pow( nu *(4./(alpha0*dx[0]*dx[0])+4./(alpha0*dx[1]*dx[1])) , 2.)
              ,-.5);
  }
  else
  {
    // ***** non-rectangular grid *****

    mg.update(MappedGrid::THEinverseVertexDerivative);  // make sure the jacobian derivatives are built
    // define an alias:
    realMappedGridFunction & rxd = mg.inverseVertexDerivative();
  
    Index I1,I2,I3;
    getIndex( mg.indexRange(),I1,I2,I3); // Get Index's for the interior+boundary points

#ifdef USE_PPP
    /// In parallel, get the serial array local to this processor
    realSerialArray rx; getLocalArrayWithGhostBoundaries(rxd,rx);
    intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
    bool ok=ParallelUtility::getLocalArrayBounds(rxd,rx,I1,I2,I3); // get bounds local to this processor
#else
    realSerialArray & rx = rxd;
    const intSerialArray & mask = mg.mask();
    bool ok=true;
#endif
    const int nd=mg.numberOfDimensions();
    #define MN(m,n) ((m)+nd*(n))
    #define RX(m,n) rx(I1,I2,I3,MN(m,n))
    if( ok ) // there are points on this processor
    {
      
      realSerialArray a1(I1,I2,I3), b1(I1,I2,I3);

      // Grid spacings on unit square:
      real dr1 = mg.gridSpacing(axis1);
      real dr2 = mg.gridSpacing(axis2);
      if( nu>0. )
      {
	realSerialArray nu11(I1,I2,I3), nu12(I1,I2,I3), nu22(I1,I2,I3);

	op.derivative(MappedGridOperators::xDerivative,rx,nu11,I1,I2,I3,MN(0,0));  // rxx
	// printf("getDt: max err in rxx=%8.2e\n",max(fabs(nu11-rx.x(I1,I2,I3,0,0)(I1,I2,I3,0,0))));
	op.derivative(MappedGridOperators::yDerivative,rx,nu22,I1,I2,I3,MN(0,1));  // ryy
	// printf("getDt: max err in rxy=%8.2e\n",max(fabs(nu22-rx.y(I1,I2,I3,0,1)(I1,I2,I3,0,1))));
	// a1   = a*RX(0,0) + b*RX(0,1) - nu*( nu11+nu22 );
	a1  = a*RX(0,0) + b*RX(0,1);
  
	op.derivative(MappedGridOperators::xDerivative,rx,nu11,I1,I2,I3,MN(1,0));  // sxx
	// printf("getDt: max err in sxx=%8.2e\n",max(fabs(nu11-rx.x(I1,I2,I3,1,0)(I1,I2,I3,1,0))));
	op.derivative(MappedGridOperators::yDerivative,rx,nu22,I1,I2,I3,MN(1,1));  // syy
	// printf("getDt: max err in syy=%8.2e\n",max(fabs(nu22-rx.y(I1,I2,I3,1,1)(I1,I2,I3,1,1))));
	// b1   = a*RX(1,0) + b*RX(1,1) - nu*( nu11+nu22 );
	b1   = a*RX(1,0) + b*RX(1,1);

	// nu11 = nu*( r1.x*r1.x + r1.y*r1.y )
	// nu12 = nu*( r1.x*r2.x + r1.y*r2.y )*2 
	// nu22 = nu*( r2.x*r2.x + r2.y*r2.y ) 
	nu11 = nu*( RX(0,0)*RX(0,0) + RX(0,1)*RX(0,1) );
	nu12 = nu*( RX(0,0)*RX(1,0) + RX(0,1)*RX(1,1) )*2.;
	nu22 = nu*( RX(1,0)*RX(1,0) + RX(1,1)*RX(1,1) );

	where( mask(I1,I2,I3)>0 ) // *wdh* 070730 
	{
	  dt = cfl * min( 
	    pow(
	      pow( abs(a1)*(1./(beta0*dr1))+abs(b1)*(1./beta0*dr2) , 2.)
	      +pow(   nu11 *(4./(alpha0*dr1*dr1)) 
		      +abs(nu12)*(1./(alpha0*dr1*dr2))
		      +nu22 *(4./(alpha0*dr2*dr2)) , 2.)
	      ,-.5) 
	    );
	}
	
      }
      else
      {
	a1   = a*RX(0,0) + b*RX(0,1);
	b1   = a*RX(1,0) + b*RX(1,1);

	where( mask(I1,I2,I3)>0 )// *wdh* 070730 
	{
	  dt = cfl * min( 
	    pow(
	      pow( abs(a1)*(1./(beta0*dr1))+abs(b1)*(1./beta0*dr2) , 2.)
	      ,-.5) 
	    );
	}
	
      }
    } // end if ok
  } // end curvilinear grid

  dt = ParallelUtility::getMinValue(dt);  // min value over all processors

  return dt;
}


real
getDt(const real & cfl, 
	const real & a, 
	const real & b, 
	const real & c, 
	const real & nu, 
	MappedGrid & mg, 
	MappedGridOperators & op,
	const real alpha0 = -2.,
	const real beta0  = 1. )
//======================================================================================
// /Description:
//  Determine the time step for the convection diffusion equation in ** 3D ** (or 2D)
//      u_t + a u_x + b u_y + c u_z = nu( u_xx + u_yy )
//  discretized with the mapping method. Scale the maximum allowable time
//  step by the factor cfl. the stability region is assumed to lie within the 
//  ellipse (x/alpha0)^2 + (y/beta0)^2 = 1
// /cfl (input): Scale the time step by this factor (cfl=1 should normally be stable)
// /a (input) : coefficient of u_x
// /b (input) : coefficient of u_y
// /c (input) : coefficient of u_z
// /nu (input) : coefficient of (u_xx+u_yy+u_zz)
// /alpha0, beta0 (input) : parameters defining the ellipse for the stability region  
// ====================================================================================
{
  if( mg.numberOfDimensions()==2 )
    return getDt(cfl,a,b,nu,mg,op,alpha0,beta0);

  real dt=REAL_MAX;
  if( mg.isRectangular() )
  {
    // ***** rectangular grid *****
    real dx[3];
    mg.getDeltaX(dx);
    
    dt = cfl * pow(
	pow( fabs(a)*(1./(beta0*dx[0])) +fabs(b)*(1./beta0*dx[1]) +fabs(c)*(1./beta0*dx[2]) , 2.)
	+pow( nu *(4./(alpha0*dx[0]*dx[0])+4./(alpha0*dx[1]*dx[1])+4./(alpha0*dx[2]*dx[2])) , 2.)
              ,-.5);
  }
  else
  {
    // ***** non-rectangular grid *****
    mg.update(MappedGrid::THEinverseVertexDerivative);  // make sure the jacobian derivatives are built
    // define an alias:
    realMappedGridFunction & rxd = mg.inverseVertexDerivative();

    const int nd=mg.numberOfDimensions();
    #undef MN
    #define MN(m,n) ((m)+nd*(n))
    #undef RX
    #define RX(m,n) rx(I1,I2,I3,MN(m,n))

    // Get Index's for the interior+boundary points
    Index I1,I2,I3;
    getIndex( mg.indexRange(),I1,I2,I3);
#ifdef USE_PPP
    /// In parallel, get the serial array local to this processor
    realSerialArray rx; getLocalArrayWithGhostBoundaries(rxd,rx);
    intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
    bool ok=ParallelUtility::getLocalArrayBounds(rxd,rx,I1,I2,I3); // get bounds local to this processor
#else
    realSerialArray & rx = rxd;
    const intSerialArray & mask = mg.mask();
    bool ok=true;
#endif
    if( ok )
    {
      // Grid spacings on unit square:
      real dr1 = mg.gridSpacing(axis1);
      real dr2 = mg.gridSpacing(axis2);
      real dr3 = mg.gridSpacing(axis3);

      realSerialArray rxx(I1,I2,I3), ryy(I1,I2,I3), rzz(I1,I2,I3);
      realSerialArray sxx(I1,I2,I3), syy(I1,I2,I3), szz(I1,I2,I3);
      realSerialArray txx(I1,I2,I3), tyy(I1,I2,I3), tzz(I1,I2,I3);

      op.derivative(MappedGridOperators::xDerivative,rx,rxx,I1,I2,I3,MN(0,0));
      op.derivative(MappedGridOperators::yDerivative,rx,ryy,I1,I2,I3,MN(0,1));
      op.derivative(MappedGridOperators::zDerivative,rx,rzz,I1,I2,I3,MN(0,2));

      op.derivative(MappedGridOperators::xDerivative,rx,sxx,I1,I2,I3,MN(1,0));
      op.derivative(MappedGridOperators::yDerivative,rx,syy,I1,I2,I3,MN(1,1));
      op.derivative(MappedGridOperators::zDerivative,rx,szz,I1,I2,I3,MN(1,2));

      op.derivative(MappedGridOperators::xDerivative,rx,txx,I1,I2,I3,MN(2,0));
      op.derivative(MappedGridOperators::yDerivative,rx,tyy,I1,I2,I3,MN(2,1));
      op.derivative(MappedGridOperators::zDerivative,rx,tzz,I1,I2,I3,MN(2,2));


      realSerialArray imLambda,reLambda;
      imLambda=(abs(a*RX(0,0) + 
		    b*RX(0,1) + 
		    c*RX(0,2)
		    - nu*( rxx+ryy+rzz ) )*(1./(beta0*dr1)) +
		abs(a*RX(1,0) + 
		    b*RX(1,1) + 
		    c*RX(1,2)
		    - nu*( sxx+syy+szz ) )*(1./(beta0*dr2)) +
		abs(a*RX(2,0) + 
		    b*RX(2,1) + 
		    c*RX(2,2)
		    - nu*( txx+tyy+tzz ) )*(1./(beta0*dr3)) );

      reLambda=( ( RX(0,0)*RX(0,0)+
		   RX(0,1)*RX(0,1)+
		   RX(0,2)*RX(0,2) )*(nu*4./(alpha0*dr1*dr1)) +
		 ( RX(1,0)*RX(1,0)+
		   RX(1,1)*RX(1,1)+
		   RX(1,2)*RX(1,2) )*(nu*4./(alpha0*dr2*dr2)) +
		 ( RX(2,0)*RX(2,0)+
		   RX(2,1)*RX(2,1)+
		   RX(2,2)*RX(2,2) )*(nu*4./(alpha0*dr3*dr3)) +
		 abs( RX(0,0)*RX(1,0)+
		      RX(0,1)*RX(1,1)+
		      RX(0,2)*RX(1,2) )*(nu*2.*(1./(alpha0*dr1*dr2)))+
		 abs( RX(0,0)*RX(2,0)+
		      RX(0,1)*RX(2,1)+
		      RX(0,2)*RX(2,2) )*(nu*2.*(1./(alpha0*dr1*dr3))) +
		 abs( RX(1,0)*RX(2,0)+
		      RX(1,1)*RX(2,1)+
		      RX(1,2)*RX(2,2) )*(nu*2.*(1./(alpha0*dr2*dr3))) );

      where( mask(I1,I2,I3)>0 )// *wdh* 070730 
      {
	dt = cfl * min( pow( imLambda*imLambda + reLambda*reLambda , -.5 ) );
      }
      
    } // end if ok
    
  } // end curvilinear grid

  dt = ParallelUtility::getMinValue(dt);  // min value over all processors
  return dt;
}



