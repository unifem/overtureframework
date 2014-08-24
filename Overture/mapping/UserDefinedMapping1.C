#include "UserDefinedMapping1.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "StretchMapping.h"
#include "NurbsMapping.h"
#include "SmoothedPolygon.h"


static StretchMapping *stretch=NULL;
static Mapping *pprofile=NULL;
static NurbsMapping *crossSection=NULL;

UserDefinedMapping1::
UserDefinedMapping1( )
  : Mapping(2,2,parameterSpace,cartesianSpace)  
//===========================================================================
/// \brief  Build a user defined Mapping
//===========================================================================
{ 
  UserDefinedMapping1::className="UserDefinedMapping1";
  setName( Mapping::mappingName,"userDefinedMapping1");

  mappingType=unitSquare;

  // mapping is from R^2 -> R^2 by default:
  setDomainDimension(2);
  setRangeDimension(2);

  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );


  setBasicInverseOption(canInvert);  // uncomment if inverse is defined in this file

  mappingHasChanged();

}

// Copy constructor is deep by default
UserDefinedMapping1::
UserDefinedMapping1( const UserDefinedMapping1 & map, const CopyType copyType )
{
  UserDefinedMapping1::className="UserDefinedMapping1";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "UserDefinedMapping1:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

UserDefinedMapping1::
~UserDefinedMapping1()
{ 
  if( debug & 4 )
    cout << " UserDefinedMapping1::Destructor called" << endl;
  delete stretch;

  delete pprofile;
  delete [] crossSection;
}

UserDefinedMapping1 & UserDefinedMapping1::
operator=( const UserDefinedMapping1 & x )
{
  if( UserDefinedMapping1::className != x.getClassName() )
  {
    cout << "UserDefinedMapping1::operator= ERROR trying to set a UserDefinedMapping1 = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class
  mappingType=x.mappingType;
  
  rp.redim(0); rp=x.rp;
  ip.redim(0); ip=x.ip;
  
  return *this;
}

void UserDefinedMapping1::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
#ifdef USE_PPP
  Overture::abort("UserDefinedMapping1::map: ERROR: fix me Bill!");
#else
  mapS(r,x,xr,params);
#endif
}


void UserDefinedMapping1::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
// ========================================================================================
// /Description:
//     Define the mapping here.
// /r (input) : parameter space coordinates on the unit line or unit square or unit cube.
// ========================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "UserDefinedMapping1::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( mappingType==unitSquare )
  {
    if( computeMap )
    {
      x(I,axis1)=r(I,axis1); 
      x(I,axis2)=r(I,axis2); 
    }
    if( computeMapDerivative )
    {
      xr(I,axis1,axis1)=1.;
      xr(I,axis1,axis2)=0.;
      xr(I,axis2,axis1)=0.;
      xr(I,axis2,axis2)=1.;
    
    }
  }
  else if( mappingType==helicalWire )
  {
    assert( rp.getLength(0)==10 );
    const real ra=   rp(0);
    const real rb=   rp(1);
    const real za=   rp(2);
    const real zb=   rp(3);
    const real x0=   rp(4);
    const real y0=   rp(5);
    const real rh=   rp(6);
    const real hfreq=rp(7);
    const real arg0= rp(8);

//       printF("The helical write is defined by \n"
//              "   x = xh(t) + rr*cos(theta)\n"
//              "   y = yh(t) + rr*sin(theta)\n"
//              "   y = yh(t) + rr*sin(theta)\n"
//              "   z = za + (zb-za)*t\n"
//              " where\n"
//              "   theta = 2*pi*(r(1)) \n"
//              "   rr = ra+(rb-ra)*(r(2))   (ra,rb are the inner and outer radii of the wire grid)\n"
//              "  t= r(3) \n"
//              "   (xh(t),yh(t)) = (x0,y0) + rh*(cos(arg),sin(arg))  (helix: center of the wire))\n"
//              "  arg = 2*pi*hfreq*(t-arg0) \n"
//              "  hfreq = helix frequency, arg0=helix angle offset\n"
//              "  rh= radius of the helix, phi0=offset for helix angle\n");

    real startAngle=0., endAngle=1.;
     
    const real scale=twoPi*(endAngle-startAngle);
    const real rad=rb-ra;
    const real length = zb-za;

    RealArray angle(I);
    angle=scale*r(I,axis1)+startAngle*twoPi;

#define RADIUS(x) (rad*(x)+ra)

    RealArray phi(I),radius(I);
    radius=RADIUS(r(I,axis2));
    phi=twoPi*hfreq*(r(I,axis3)-arg0);

    if( computeMap )
    {
      RealArray xh(I),yh(I); // helix

      xh=x0+rh*cos(phi);
      yh=y0+rh*sin(phi);

      x(I,0)=radius*cos(angle)+xh;
      x(I,1)=radius*sin(angle)+yh;
      x(I,2)=length*r(I,axis3)+za;
    }
    if( computeMapDerivative )
    {

      xr(I,axis1,axis1)=-radius*scale*sin(angle);
      xr(I,axis2,axis1)= radius*scale*cos(angle);
      xr(I,axis3,axis1)=0;

      xr(I,axis1,axis2)= rad*cos(angle);
      xr(I,axis2,axis2)= rad*sin(angle);
      xr(I,axis3,axis2)=0.;

      xr(I,axis1,axis3)=(-rh*twoPi*hfreq)*sin(phi);
      xr(I,axis2,axis3)=( rh*twoPi*hfreq)*cos(phi);
      xr(I,axis3,axis3)=length;


    }
  }
  else if( mappingType==filletForTwoCylinders )
  {
    // Here is a surface-grid  fillet grid for joining two intersecting cylinders.  
    // 
    // cyl1 is parallel to the x-axis :  y^2 + z^2 = a^2
    // cyl2 is parallel to the y-axis :  x^2 + y^2 = b^2 
    // 
    // The curve of intersection is
    //      x=b*cos(t), z=b*sin(t), y = sqrt( a^2 - (b*sin(t))^2 )

    real a = rp(0);
    real b = rp(1);
    real width=rp(2);

    assert( a>=b-width );

    real aSq=a*a, bSq=b*b;
    
    // RealArray xh(I),yh(I); // helix

    // xh=x0+rh*cos(phi);
    // yh=y0+rh*sin(phi);
    RealArray theta(I), st(I), ct(I), x1(I), y1(I), z1(I), x2(I), y2(I), z2(I), bb(I), temp;
    RealArray s(I), alpha(I), alphas(I), beta(I), betas(I);

    theta(I)=2.*Pi*r(I,0);
    st= sin(theta);
    ct= cos(theta);

    // ****************************************************************************
    //   The fillet is defined as S(r,s) = (1-alpha(s))*S1(r,1-s) + alpha(s)*S2(r,s)
    // ****************************************************************************

    // To make sharper use : 
    //      S(r,s) = (1-alpha(s))*S1(r,beta(1-s)) + alpha(s)*S2(r,beta(s))


    // Here is a function that goes from 0 to 1 and has zero derivatives at 0 and 1: 
    // alpha(I) = 6.*SQR(r(I,1))*( .5 - r(I,1)/3. ); // blending function goes from 0 to 1. 

    assert( stretch!=NULL );
    s(I)=r(I,1);
    stretch->mapS( s,alpha,alphas );  // blending function

    beta = alpha*s;
    betas = alpha + alphas*s;

    RealArray sm(I),alpham(I),alphams(I),betam(I),betams(I);
    sm=1.-s(I);
    stretch->mapS( sm,alpham,alphams ); 
    betam= alpham*sm;
    betams = -alpham - alphams*sm;
    

    // Surface 1:  S1(r,1-s) 
    temp = sqrt( aSq - bSq*SQR(st) );
    x1=b*ct;
    y1=temp + betam*width; 
    z1=b*st;

    // Surface 2: S2(r,s)
    real db=width;
    bb(I) = b + db*beta;
    x2=bb*ct;
    y2=sqrt( aSq - SQR(bb*st) );
    z2=bb*st;

      
    if( computeMap )
    {

      x(I,0)=(1.-alpha)*x1 + alpha*x2;
      x(I,1)=(1.-alpha)*y1 + alpha*y2;
      x(I,2)=(1.-alpha)*z1 + alpha*z2;
    }
    if( computeMapDerivative )
    {
      // check these : 
      xr(I,axis1,0)=(1.-alpha)*(-b*2.*Pi*st)            + alpha*(-bb*2.*Pi*st);
      xr(I,axis2,0)=(1.-alpha)*(-bSq*2.*Pi*st*ct/temp)  + alpha*(-bb*bb*2.*Pi*st*ct/y2);
      xr(I,axis3,0)=(1.-alpha)*( b*2.*Pi*ct)            + alpha*( bb*2.*Pi*ct);

      xr(I,axis1,1)=-alphas*x1 + alphas*x2                           + alpha*db*betas*ct;
      xr(I,axis2,1)=-alphas*y1 + alphas*y2  +(1.-alpha)*betams*width - alpha*bb*betas*st*st*db/y2;
      xr(I,axis3,1)=-alphas*z1 + alphas*z2                           + alpha*db*betas*st;

    }


  }
  else if( mappingType==blade )
  {
    const int profileOption= ip(0) ; 

    // Profile for the blade 
    assert( pprofile!=NULL );
    Mapping & profile = *pprofile;
    
    const real sTip = rp(0);


    // ---- Here is a wind turbine blade ---
    if( profileOption==0 )
    {
      // Joukowsky parameters
      real a=.85, d=.15, d0=d, delta=(-15.)*Pi/180; 
      real amRe=-d*sin(delta), amIm= d*cos(delta);

      // Compute the "min" value of the profile 
      real cost=-1., sint=0;
      real wRe=a*cost+amRe, wIm=(-a)*sint+amIm;
      real wNormI=1./(wRe*wRe+wIm*wIm);
      real zReMin=wRe+wRe*wNormI,  zImMin=wIm-wIm*wNormI;

      // Compute the "max" value of the profile
      cost=1., sint=0.;
      wRe=a*cost+amRe, wIm=(-a)*sint+amIm;
      wNormI=1./(wRe*wRe+wIm*wIm);
      real zReMax=wRe+wRe*wNormI,  zImMax=wIm-wIm*wNormI;
      real zScale=1./(zReMax-zReMin);
      const real twoPi=2.*Pi;

      RealArray rv(1,1), xv(1,2), xrv(1,2,1);
      for( int i=I.getBase(); i<=I.getBound(); i++ )
      {
	real s=r(i,0);             // axial parameter
	real theta=twoPi*r(i,1);    // angular

	cost = cos(theta), sint = sin(theta); 

	// Basic cross-section: (x,y) = (.5*cos(theta)+.5,.5*sin(theta))
	//  Assume: 0<= xc <=1
	real xc0 = .5*(cost+1.), yc0=.5*sint;


	// Joukowsky:
	//   z = x + i y = w + 1/w
	//   w = a exp(-i theta) + i d*exp(i delta)
	//     = a*cost - a*i*sint - d*sind + i*d*cosd  
	//   z = w + wBar/|w|^2 
	//   x = [ a*cos(theta) - d*sin(delta) ]*[ 1+1/wNorm]
	//   y = [-a*sin(theta) + d*cos(delta) ]*[ 1-1/wNorm]

	// At the tip we transition the Joukowsky to an ellipse by making "d" go to zero at s=1
	// real beta1=10.;
	real beta1=10.;
	real phi1, phi1s;
	if( profileOption==0 )
	{
	  real sech1 = 1./cosh(beta1*(1.-s));
	  phi1= tanh(beta1*(1.-s));
	  phi1s = -beta1*sech1*sech1;
	}
	else
	{
	  phi1=1.;
	  phi1s=0.;
	}
      

	d = d0*phi1;  // transition to an ellipse at the end

	real sind=sin(delta), cosd=cos(delta);
	real amRe=-d*sind, amIm= d*cosd;
	wRe=a*cost+amRe, wIm=(-a)*sint+amIm;
	wNormI=1./(wRe*wRe+wIm*wIm);
	real zRe=wRe+wRe*wNormI,  zIm=wIm-wIm*wNormI;

	// Here is the scaled Joukowsky cross-section: 
	real yShift=profileOption==0 ? 0 : -.1;
	real xc1=(zRe-zReMin)*zScale, yc1=zIm*zScale+yShift; 

	// Blend the initial circular cross-section (xc0,yc0) with the Joukowsky: 
	real sa=.15;      // transition position
	real beta=20.;    // transition exponent
	real phi0 = .5*(1.+tanh(beta*(s-sa)));  // phi0 : goes from 0 to 1 at s=sa

	// Here is the cross-section before scaling by the profile: 
	real xc = xc0*(1.-phi0) + xc1*phi0;
	real yc = yc0*(1.-phi0) + yc1*phi0;
	

	// --- Evaluate the profile and it's derivatives  ---

	// First eval the top profile = profile(0..sTip)
	real s1 = sTip*s; rv(0,0)=s1;    
	profile.mapS(rv,xv,xrv);
	real z=xv(0,0);               // actual axial position for this value of s 
	real zs = xrv(0,0,0)*sTip;
	real top = xv(0,1);
	real tops = xrv(0,1,0)*sTip;

	// Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
	real s2=1.- (1.-sTip)*s; rv(0,0)=s2;  // bot = profile(1..sTip)
	profile.mapS(rv,xv,xrv);
	real bot = xv(0,1);
	real bots = xrv(0,1,0)*(-(1.-sTip));

	real rad=top-bot;
	if( rad<0. )
	{
	  printF("WARNING: rad=%8.2e, s1=%10.4e, s2=%10.4e, top=%10.4e, bot=%10.4e\n",rad,s1,s2,top,bot);
	}
      
	if( computeMap )
	{
	  x(i,0)=xc*rad+bot;
	  x(i,1)=yc*rad;
	  x(i,2)=z;
	}
	if( computeMapDerivative )
	{
	  // d= d0*phi1 = d0*tanh(beta1*(1.-s))
	  // d(d)/ds = 
	  real ds =d0*phi1s;   // d(d)/ds

	  // derivatives of the Joukowsky profile : 
	  // z' = w' ( 1-1/w^2 ) 
	  //    = w' ( 1 - wBar^2/|w|^4 )
	  //    = w' ( 1 - (wr - i wi)^2/|w|^4 )
	  //    = w' ( 1 - (wr^2-wi^2)/|w|^4 + 2i*wr*wi/|w|^4 )
	  real dzRe=1.-( wRe*wRe-wIm*wIm )*wNormI*wNormI;
	  real dzIm=   ( 2.*wRe*wIm      )*wNormI*wNormI;

	  // w = a*cost - a*i*sint - d*sind + i*d*cosd  
	  // dw/d(theta) = a*2*pi*(  -sint - i*cost )
	  real wtRe = -a*twoPi*sint, wtIm= -a*twoPi*cost;
	  real ztRe = wtRe*dzRe - wtIm*dzIm;
	  real ztIm = wtIm*dzRe + wtRe*dzIm;
	  // xc1=(zRe-zReMin)*zScale, yc1=zIm*zScale; 
	  real xc1t=ztRe*zScale, yc1t=ztIm*zScale; 

	  // dw/ds = i*ds*exp(i delta) = -ds*sind + i*ds*cosd
	  real wsRe = -ds*sind, wsIm=ds*cosd;
	  real zsRe = wsRe*dzRe - wsIm*dzIm;
	  real zsIm = wsIm*dzRe + wsRe*dzIm;
	  real xc1s = zsRe*zScale, yc1s=zsIm*zScale;

	  // phi0 = .5*(1.+tanh(beta*(s-sa)))
	  real sechs = 1./cosh(beta*(s-sa));
	  real phi0s = .5*beta*sechs*sechs;
	  // xc = xc0*(1.-phi0) + xc1*phi0;
	  // yc = yc0*(1.-phi0) + yc1*phi0;
	  real xcs = xc0*(-phi0s) + xc1*phi0s + xc1s*phi0;
	  real ycs = yc0*(-phi0s) + yc1*phi0s + yc1s*phi0;

	  real rads = tops-bots;

	  // x(i,0)=xc*rad+bot;
	  // x(i,1)=yc*rad;
	  // x(i,2)=z;
      

	  // xc0 = .5*(cost+1.), yc0=.5*sint;
	  real xc0t = -Pi*sint, yc0t=Pi*cost;   // xc0t = d(xc0)/d(s1)
	  real xct = xc0t*(1.-phi0) + xc1t*phi0;
	  real yct = yc0t*(1.-phi0) + yc1t*phi0;
	  real zt=0.;

	  switch (params.coordinateType)
	  {
	  case cartesian: 
	    xr(I,0,0)=xcs*rad+xc*rads+bots;
	    xr(I,1,0)=ycs*rad+yc*rads;
	    xr(I,2,0)=zs;

	    xr(I,0,1)=xct*rad;
	    xr(I,1,1)=yct*rad;
	    xr(I,2,1)=zt;
	    break;

	  case cylindrical:  // return -rho*d()/ds and (1/rho)*d()/d(theta)

	    // -- this needs to be worked out ---
	    // Normally "s" is the axial variable like "z"


	    //  zeta= sScale*rS-(1.-2.*startS)    // 2*( (endS-startS)*r+startS ) -1
	    //  rho=  SQRT(fabs(1.-SQR(zeta))) 
	    // real rho =sqrt( fabs(1.-s*s) );  // is this correct ? 
	    // real rho = rad; // is this correct ? 

	    xr(I,0,0)=xcs*rad+xc*rads+bots;
	    xr(I,1,0)=ycs*rad+yc*rads;
	    xr(I,2,0)=zs;

	    xr(I,0,1)=xct*rad;
	    xr(I,1,1)=yct*rad;
	    xr(I,2,1)=zt;

	    break;

	  default:
	    printF("UserDefinedMapping1::map: ERROR not implemented for coordinateType = %i\n",
		   (int)params.coordinateType);
	  }
	

	}
      }
    }
    else if( profileOption==1 )
    {
      // -- compressor blade --
      assert( crossSection!=NULL );
      
      RealArray rt(I,1),x0(I,3),xr0(I,3,1);

      rt(I,0)=r(I,1);  // angular direction
      crossSection[0].mapS(rt,x0,xr0);
      
      RealArray x1(I,3),xr1(I,3,1);
      crossSection[1].mapS(rt,x1,xr1);

      RealArray x2(I,3),xr2(I,3,1);
      crossSection[2].mapS(rt,x2,xr2);



      RealArray rv(1,1), xv(1,2), xrv(1,2,1);
      real s0,s1,s2;
      for( int i=I.getBase(); i<=I.getBound(); i++ )
      {
	real s=r(i,0);

	// Eval the top profile = profile(0..sTip)
	s1 = sTip*s; rv(0,0)=s1;    
	profile.mapS(rv,xv,xrv);
	real z=xv(0,0);               // actual axial position for this value of s 
	real zs = xrv(0,0,0)*sTip;
	real top = xv(0,1);
	real tops = xrv(0,1,0)*sTip;

	// Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
	s2=1.- (1.-sTip)*s; rv(0,0)=s2;  // bot = profile(1..sTip)
	profile.mapS(rv,xv,xrv);
	real bot = xv(0,1);
	real bots = xrv(0,1,0)*(-(1.-sTip));

	real rad=top-bot;
	if( rad<0. )
	{
	  printF("WARNING: rad=%8.2e, s1=%10.4e, s2=%10.4e, top=%10.4e, bot=%10.4e\n",rad,s1,s2,top,bot);
	}

	real xc,yc,zc;
	
        // Use quadratic interpolation between the cross-sections
        real sc0=0., sc1=.33, sc2=.612; // s positions of the cross sections
	 
	real q0=(sc1-s)*(sc2-s)/((sc1-sc0)*(sc2-sc0));
	real q1=(sc2-s)*(sc0-s)/((sc2-sc1)*(sc0-sc1));
	real q2=(sc0-s)*(sc1-s)/((sc0-sc2)*(sc1-sc2));
	 
	xc = x0(i,0)*q0+x1(i,0)*q1+x2(i,0)*q2;
	yc = x0(i,1)*q0+x1(i,1)*q1+x2(i,1)*q2;
	zc = x0(i,2)*q0+x1(i,2)*q1+x2(i,2)*q2;
	
	if( computeMap )
	{
          // scale (xc,yc) by the profile 
	  x(i,0)=xc*rad;
	  x(i,1)=yc*rad;
	  x(i,2)=z;
	}

	if( computeMapDerivative )
	{
	}
	
      }
      

    }
    else
    {
      printF("ERROR: unknown profileOption=%i\n",profileOption);
      OV_ABORT("error");
    }
    



  }
  else
  {
    printF("UserDefinedMapping1:ERROR: unknown mappingType=%i\n",mappingType);
    Overture::abort();
  }
  
}

// ==========================================================================
//  Initialization for the blade
// ==========================================================================
int UserDefinedMapping1::
bladeSetup(MappingInformation & mapInfo)
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  // Profile for the blade 
  assert( pprofile==NULL );
  
  const real pi=4.*atan2(1.,1.);

  int profileOption= ip(0) ;  // =0

  int numPts=-1;
  if( profileOption==0 )
    numPts=5+2+11+11-4+11;
  else
    numPts=5*11 - 4 -4;

  RealArray xp(numPts,2);
 


  int n=0; //  counts points
  real r,x,y,theta;

  
  if( profileOption==0 )
  {
    // ---- Here is a profile for a wind turbine blade ---
    

    real xa=0., ya=.8, xb=.8, yb=.8; int nn=5;
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    x=1.4; y=1.1;   xp(n,0)=x; xp(n,1)=y; n++;
    x=1.8; y=1.45;  xp(n,0)=x; xp(n,1)=y; n++;
    xa=3.; ya=1.6; xb=9.; yb=.8; nn=11; 
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    // tip: ellipse
    real x0=9., y0=.4, rada=.8, radb=.4, theta0=pi*.5, theta1=-.5*pi; nn=11;
    for( int i=2; i<nn-2; i++ ){ r=i/(nn-1.); theta=theta0+(theta1-theta0)*r; x=x0+rada*cos(theta); y=y0+radb*sin(theta); xp(n,0)=x; xp(n,1)=y; n++; }
    xa=9.; ya=0.; xb=0.; yb=.0; nn=11; 
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    
    assert( numPts==n );

    NurbsMapping & profile = *new NurbsMapping;

    pprofile = &profile;
    profile.setDomainDimension(1);
    profile.setRangeDimension(2);
    profile.interpolate(xp);

    profile.setGridDimensions(0,101);


  }
  else if( profileOption==1 )
  {

    // -- compressor turbine: straight profile with flat tip --

    //  s=0        s -> 
    //  X---------------------------+
    //                                 +
    //                                  +
    //                                  |
    //  z -> ...........................|s=sTip ........  
    //                                  |
    //                                  +
    //                                 +
    //  X---------------------------+
    //  s=1

    if( false )
    {
      real height=3.;

      real xa1=0., ya1=.5, xb1=height, yb1=ya1; int nn=11;
      for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa1+(xb1-xa1)*r; y=ya1 + (yb1-ya1)*r; xp(n,0)=x; xp(n,1)=y; n++; }
      // rounded corner at top
      real rada=.15, radb=.15;
      real x0=xb1, y0=ya1-radb,  theta0=pi*.5, theta1=0; nn=11;
      for( int i=2; i<nn-2; i++ ){ r=i/(nn-1.); theta=theta0+(theta1-theta0)*r; x=x0+rada*cos(theta); y=y0+radb*sin(theta); xp(n,0)=x; xp(n,1)=y; n++; }

      // flat tip
      real xa2=xb1+rada, ya2=yb1-radb, xb2=xa2, yb2=-ya1+radb; nn=11;
      for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa2+(xb2-xa2)*r; y=ya2 + (yb2-ya2)*r; xp(n,0)=x; xp(n,1)=y; n++; }

      // rounded corner at bottom
      x0=xb1, y0=-ya1+radb,  theta0=0., theta1=-pi*.5; nn=11;
      for( int i=2; i<nn-2; i++ ){ r=i/(nn-1.); theta=theta0+(theta1-theta0)*r; x=x0+rada*cos(theta); y=y0+radb*sin(theta); xp(n,0)=x; xp(n,1)=y; n++; }

      // straight bottom
      xa2=xb1, ya2=-ya1, xb2=xa1, yb2=ya2; nn=11;
      for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa2+(xb2-xa2)*r; y=ya2 + (yb2-ya2)*r; xp(n,0)=x; xp(n,1)=y; n++; }

    
      NurbsMapping & profile = *new NurbsMapping;

      pprofile = &profile;
      profile.setDomainDimension(1);
      profile.setRangeDimension(2);
      profile.interpolate(xp);

      profile.setGridDimensions(0,101);
    }
    else
    {
      // Use a profile from the mapping list:
      // pprofile = mapInfo.mappingList[0].mapPointer;

     SmoothedPolygon & smoothedPolygon = *new SmoothedPolygon;
     pprofile = &smoothedPolygon;
     
     // set the verticies of the smoothed polygon profile
     RealArray xv(4,2);
     real height=3.;
     xv(0,0)=0.;     xv(0,1)= .5;
     xv(1,0)=height; xv(1,1)= .5;
     xv(2,0)=height; xv(2,1)=-.5;
     xv(3,0)=0.;     xv(3,1)=-.5;
     smoothedPolygon.setPolygon(xv);
     smoothedPolygon.setDomainDimension(1); // make a curve 

     if( false )
       smoothedPolygon.update(mapInfo);

    }

  }

  Mapping & profile = *pprofile;
  
  // View the profile: 
  if( false )
    profile.update(mapInfo);

  // We need to define curves for the top and bottom of the profile:

  // first find the tip of the profile:  (where x'=0)
  real sTip=.5;
  if( profileOption==0 )
  {
    RealArray rv(1,1), xv(1,2), xrv(1,2,1);
    real ra=.3, rb=.7;
    rv=ra;  profile.mapS(rv,xv,xrv);
    real xra=xrv(0,0,0);
    rv=rb; profile.mapS(rv,xv,xrv);
    real xrb=xrv(0,0,0);
    assert( xra*xrb<0. );
    const real eps=REAL_EPSILON*10.;
    const int maxit=100;
    int it=0;
    while( fabs(rb-ra)>eps )
    {
      real rm = .5*(ra+rb);
      rv=rm;
      profile.mapS(rv,xv,xrv);
      real xrm = xrv(0,0,0);
      if( xrm<0. )
      {
	xrb=xrm; rb=rm;
      }
      else
      {
	xra=xrm; ra=rm;
      }
      it++;
      if( it>maxit ) break;
    }
    sTip=.5*(ra+rb);
    printF(" tip of profile: s=%14.8e (rb-ra=%8.2e, eps=%8.2e)\n",sTip,rb-ra,eps);
  }
  else
  {
    sTip=.5;
  }
  
  rp(0)=sTip;

  // Here we define cross-sections for the compressor blade

  if( profileOption==1 )
  {
    // Notes: 
    //  - rotation and chord should depend on z and not s
    //  -- increase number of cross-sections or define through a formula  ***


    const int nc = 3; // number of cross-sections 
    crossSection = new NurbsMapping [nc];
    
    // cross-section 0 :

    // Cross section : Joukowsky parameters
    real a=.85, d=.15, d0=d, delta=(-15.)*Pi/180; 
    real amRe=-d*sin(delta), amIm= d*cos(delta);
    real sind=sin(delta), cosd=cos(delta);

//     // Compute the "min" value of the profile 
//     real cost=-1., sint=0;
//     real wRe=a*cost+amRe, wIm=(-a)*sint+amIm;
//     real wNormI=1./(wRe*wRe+wIm*wIm);
//     real zReMin=wRe+wRe*wNormI,  zImMin=wIm-wIm*wNormI;

//     // Compute the "max" value of the profile
//     cost=1., sint=0.;
//     wRe=a*cost+amRe, wIm=(-a)*sint+amIm;
//     wNormI=1./(wRe*wRe+wIm*wIm);
//     real zReMax=wRe+wRe*wNormI,  zImMax=wIm-wIm*wNormI;
//     real zScale=1./(zReMax-zReMin);
//     real yShift=0.;

    const int nt = 101;
    Range I=nt;
    RealArray xc(nt,3);
    real xMin=REAL_MAX, xMax=-xMin, yMin=xMin, yMax=xMax;
    for( int i=0; i<nt; i++ )
    {
      real theta= twoPi*i/(nt-1.);
      real cost = cos(theta), sint = sin(theta);

      real wRe=a*cost+amRe, wIm=(-a)*sint+amIm;
      real wNormI=1./(wRe*wRe+wIm*wIm);
      real zRe=wRe+wRe*wNormI,  zIm=wIm-wIm*wNormI;

      // Here is the Joukowsky cross-section: 
      real xc1=zRe, yc1=zIm;

      xc(i,0)=xc1;
      xc(i,1)=yc1;
      xc(i,2)=0.;
      
      xMin=min(xMin,xc1);
      xMax=max(xMax,xc1);
      yMin=min(yMin,yc1);
      yMax=max(yMax,yc1);
    }
    // scale profile to lie on [-.5,.5] in the x direction, and be centered about 0 in the y direction
    real xAve = .5*(xMin+xMax);
    xc(I,0) = (xc(I,0)-xAve)/(xMax-xMin);
    real yAve = .5*(yMax+yMin);
    xc(I,1) = (xc(I,1)-yAve)/(xMax-xMin);
    crossSection[0].setIsPeriodic(0,Mapping::functionPeriodic);
    crossSection[0].interpolate(xc);
    crossSection[0].setGridDimensions(0,101);    
    crossSection[0].setName(Mapping::mappingName,"crossSection0");
    
    if( false )
    { // display the cross section
      crossSection[0].update(mapInfo);
    }
    
    // Cross-section 1 is scaled and rotated 
    // compute the "s" value for this cross-section
    real zc = 1.5;
    
    RealArray rv(1,1), xv(1,2);
    xv(0,0)=zc; xv(0,1)=.5;
    rv=-1.;
    profile.inverseMapS(xv,rv);
    real sc=rv(0,0)/sTip;
    
    printF(" Cross-section 1 : z=%8.2e, s=%8.2e\n",zc,sc);


    real cScale=.9;
    xc *= cScale;
    xc(I,2)=zc;       // z value 
    crossSection[1].setIsPeriodic(0,Mapping::functionPeriodic);
    crossSection[1].interpolate(xc);
    crossSection[1].setGridDimensions(0,101);    
    real rotationAngle= 20.*Pi/180;    // rotation angle
    crossSection[1].rotate( 2,rotationAngle );   // rotate about (0,0,0)
    crossSection[1].setName(Mapping::mappingName,"crossSection1");
    

    // Cross-section 2 is scaled and rotated 

    // NOTE: the last cross section will be shrunk to (x,y)=(0,0) so we assume that (0,0) is inside the last
    //       cross-section or else the surface near the tip will be bad. 
    zc=2.75;
    xv(0,0)=zc; xv(0,1)=.5;
    rv=-1.;
    profile.inverseMapS(xv,rv);
    sc=rv(0,0)/sTip;
    
    printF(" Cross-section 2 : z=%8.2e, s=%8.2e\n",zc,sc);
    

    cScale=1.;  // scaled from previous
    xc *= cScale;
    xc(I,2)=2.;       // z value 
    crossSection[2].setIsPeriodic(0,Mapping::functionPeriodic);
    crossSection[2].interpolate(xc);
    crossSection[2].setGridDimensions(0,101);    
    rotationAngle= 25.*Pi/180;    // rotation angle
    crossSection[2].rotate( 2,rotationAngle );   // rotate about (0,0,0)
    crossSection[2].setName(Mapping::mappingName,"crossSection2");

//     // Cross-section 3 is a rotated ellipse for the tip region

//     real ae=.3*cScale, be=.075*cScale;
//     real xe=.0, ye=.0;
//     for( int i=0; i<nt; i++ )
//     {
//       real theta= twoPi*i/(nt-1.);
//       real cost = cos(theta), sint = sin(theta);

//       xc(i,0)=ae*cost+xe;
//       xc(i,1)=be*sint+ye;
//       xc(i,2)=0.;
//     }
//     crossSection[2].setIsPeriodic(0,Mapping::functionPeriodic);
//     crossSection[2].interpolate(xc);
//     crossSection[2].setGridDimensions(0,101);    
//     crossSection[2].rotate( 2,rotationAngle );
//     crossSection[2].setName(Mapping::mappingName,"crossSection2");


    if( true )
    {
      // Add cross sections to the mapping list so we can plot them all
      mapInfo.mappingList.addElement(profile);
      mapInfo.mappingList.addElement(crossSection[0]);
      mapInfo.mappingList.addElement(crossSection[1]);
      mapInfo.mappingList.addElement(crossSection[2]);
      gi.erase();
      viewMappings(mapInfo);
    }
    
  }
  else
  {
    printF("blade:ERROR: unknown option=%i\n");
    OV_ABORT("error");
  }
  
  

  
  return 0;
  
}



void UserDefinedMapping1::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
#ifdef USE_PPP
  Overture::abort("UserDefinedMapping1::basicInverse: ERROR: fix me Bill!");
#else
  basicInverseS(x,r,rx,params);
#endif
}



void UserDefinedMapping1::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
//==================================================================================
// /Description:
//   Define the inverse here if you know it. You must also set the bascInverseOption in the
//  constructor or else this function will not be called.
//=================================================================================
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );
  if( mappingType==unitSquare )
  {
    if( computeMap )
    {
      r(I,axis1)=x(I,axis1);
      r(I,axis2)=x(I,axis2);
      periodicShift(r,I);   // shift r in any periodic directions
    }
    if( computeMapDerivative )
    {
      rx(I,axis1,axis1)=1.;
      rx(I,axis1,axis2)=0.;
      rx(I,axis2,axis1)=0.;
      rx(I,axis2,axis2)=1.;
    }
  }
  else if( mappingType==helicalWire )
  {
    assert( rp.getLength(0)==10 );
    const real ra=   rp(0);
    const real rb=   rp(1);
    const real za=   rp(2);
    const real zb=   rp(3);
    const real x0=   rp(4);
    const real y0=   rp(5);
    const real rh=   rp(6);
    const real hfreq=rp(7);
    const real arg0= rp(8);

    real startAngle=0., endAngle=1.;
     
    const real scale=twoPi*(endAngle-startAngle), inverseScale=1./scale;
    const real rad=rb-ra, inverseRad=1./rad;
    const real length = zb-za, inverseLength=1./length;


//       printF("The helical write is defined by \n"
//              "   x = xh(t) + rr*cos(theta)\n"
//              "   y = yh(t) + rr*sin(theta)\n"
//              "   y = yh(t) + rr*sin(theta)\n"
//              "   z = za + (zb-za)*t\n"
//              " where\n"
//              "   theta = 2*pi*(r(1)) \n"
//              "   rr = ra+(rb-ra)*(r(2))   (ra,rb are the inner and outer radii of the wire grid)\n"
//              "  t= r(3) \n"
//              "   (xh(t),yh(t)) = (x0,y0) + rh*(cos(arg),sin(arg))  (helix: center of the wire))\n"
//              "  arg = 2*pi*hfreq*(t-arg0) \n"
//              "  hfreq = helix frequency, arg0=helix angle offset\n"
//              "  rh= radius of the helix, phi0=offset for helix angle\n");


//       phi=twoPi*hfreq*(r(I,axis3)-arg0);
//       xh=x0+rh*cos(phi);
//       yh=y0+rh*sin(phi);

//       x(I,0)=radius*cos(angle)+xh;
//       x(I,1)=radius*sin(angle)+yh;
//       x(I,2)=length*r(I,axis3)+za;

    RealArray r3(I), phi(I),xh(I),yh(I); // helix

    r3=(x(I,axis3)-za)*inverseLength;

    phi=twoPi*hfreq*(r3-arg0);
    xh=x0+rh*cos(phi);
    yh=y0+rh*sin(phi);
    
    if( computeMap )
    {
      real theta0=twoPi*startAngle; //  theta1=twoPi*endAngle;

      // **NOTE** atan2(-y,-x) : result in [-pi,pi]
      r(I,axis3)=r3;

      r(I,axis1)=atan2(evaluate(yh-x(I,axis2)),evaluate(xh-x(I,axis1))); 
      r(I,axis1)=( r(I,axis1)+(Pi-theta0) )*inverseScale;
      r(I,axis1)=fmod(r(I,axis1)+1.,1.);  // map back to [0,1]

      r(I,axis2)=(sqrt(SQR(x(I,axis1)-xh)+SQR(x(I,axis2)-yh))-ra)*inverseRad;

      periodicShift(r,I);   // shift r in any periodic directions

    }
    if( computeMapDerivative )
    {
      RealArray radius(I);
      radius=sqrt(SQR(x(I,axis1)-xh)+SQR(x(I,axis2)-yh));

      rx(I,axis2,axis1)= (x(I,axis1)-xh)/(rad*radius);
      rx(I,axis2,axis2)= (x(I,axis2)-yh)/(rad*radius);
      rx(I,axis2,axis3)= ( (x(I,axis1)-xh)*( sin(phi)) + 
			   (x(I,axis2)-yh)*(-cos(phi)) )*(rh*twoPi*hfreq/(length*rad))/radius;

      radius=inverseScale/(SQR(radius));    // ** change defn of radius! **
      rx(I,axis1,axis1)=-(x(I,axis2)-yh)*radius;
      rx(I,axis1,axis2)= (x(I,axis1)-xh)*radius;
      rx(I,axis1,axis3)=((x(I,axis2)-yh)*(sin(phi))+
			 (x(I,axis1)-xh)*(cos(phi)) )*(-rh*twoPi*hfreq/length)*radius;

      rx(I,axis3,axis1)=0.;
      rx(I,axis3,axis2)=0.;
      rx(I,axis3,axis3)=inverseLength;

//       radius=sqrt(SQR(x(I,cylAxis1)-xc[cylAxis1])+SQR(x(I,cylAxis2)-xc[cylAxis2]));
//       // radius=pow(pow(x(I,cylAxis1)-xc[cylAxis1],2)+pow(x(I,cylAxis2)-xc[cylAxis2],2),.5);
//       rx(I,axis3,cylAxis1)= (x(I,cylAxis1)-xc[cylAxis1])/(rad*radius);
//       rx(I,axis3,cylAxis2)= (x(I,cylAxis2)-xc[cylAxis2])/(rad*radius);
//       rx(I,axis3,cylAxis3)=0.;
//       radius=inverseScale/(SQR(radius));    // ** change defn of radius! **
//       rx(I,axis1,cylAxis1)=-(x(I,cylAxis2)-xc[cylAxis2])*radius;
//       rx(I,axis1,cylAxis2)= (x(I,cylAxis1)-xc[cylAxis1])*radius;
//       rx(I,axis1,cylAxis3)=0.;

    }
  }
  else
  {
    printF("UserDefinedMapping1:ERROR: unknown mappingType=%i\n",mappingType);
    Overture::abort();
  }
  
}
  

//=================================================================================
// get a mapping from the database
//=================================================================================
int UserDefinedMapping1::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( UserDefinedMapping1::className,"className" ); 
  if( UserDefinedMapping1::className != "UserDefinedMapping1" )
  {
    cout << "UserDefinedMapping1::get ERROR in className!" << endl;
  }
  int temp;
  subDir.get( temp,"mappingType" ); mappingType=(UserDefinedMappingEnum)temp;
  subDir.get( rp,"rp" );
  subDir.get( ip,"ip" );

  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0; 
}

int UserDefinedMapping1::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( UserDefinedMapping1::className,"className" );
  subDir.put( (int)mappingType,"mappingType" ); 
  subDir.put( rp,"rp" );
  subDir.put( ip,"ip" );            
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *UserDefinedMapping1::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==UserDefinedMapping1::className )
    retval = new UserDefinedMapping1();
  return retval;
}

void UserDefinedMapping1::
getParameters(IntegerArray & ipar, RealArray & rpar ) const
//===========================================================================
/// \details  
///    Return the current values for the parameters.
//===========================================================================
{
  ipar.redim(0); ipar=ip;
  rpar.redim(0); rpar=rp;
}



void UserDefinedMapping1::
setParameters(const IntegerArray & ipar, 
              const RealArray & rpar )
//===========================================================================
/// \details 
///     Set any parameters. These are up to you to define and use as appropriate.
//===========================================================================
{
  ip.redim(0); ip=ipar;
  rp.redim(0); rp=rpar;
}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int UserDefinedMapping1::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
//   aString menu[] = 
//     {
//       "!UserDefinedMapping1",
//       "unit square",
//       "helical wire",
//       " ",
//       "lines",
//       "boundary conditions",
//       "share",
//       "mappingName",
//       "periodicity",
//       "show parameters",
//       "plot",
//       "help",
//       "exit", 
//       "" 
//      };

  // defaults for helical wire: 
  real ra=.2, rb=.3;
  real za=0., zb=5.;
  real x0=0., y0=0., rh=.5;
  real hfreq=1., arg0=0.;
  if( mappingType==helicalWire && rp.getLength(0)==10 )
  {
    ra=   rp(0);
    rb=   rp(1);
    za=   rp(2);
    zb=   rp(3);
    x0=   rp(4);
    y0=   rp(5);
    rh=   rp(6);
    hfreq=rp(7);
    arg0= rp(8);
  }
  

  GUIState dialog;
  bool buildDialog=true;
  if( buildDialog )
  {
    dialog.setWindowTitle("UserDefinedMapping");
    dialog.setExitCommand("exit", "exit");

    // option menus
    dialog.setOptionMenuColumns(1);

    aString opCommand1[] = {"unit square",
			    "helical wire",
                            "fillet for two cylinders",
                            "blade",
			    ""};
    
    dialog.addOptionMenu( "type:", opCommand1, opCommand1, mappingType); 


    aString cmds[] = {"lines",
		      "boundary conditions",
		      "share",
		      "mappingName",
		      "periodicity",
		      "show parameters",
		      "plot",
                      "check",
                      "check inverse",
		      "help",
		      ""};
    int numberOfPushButtons=10;  // number of entries in cmds
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

//   bool plotVolumeGrid=false;
//   bool plotSurfaceGrid=true;
//   bool plotStitching=true;
//   bool interactiveStitcher=true;
//   aString tbCommands[] = {"plot volume grid",
//                           "plot surface grid",
// 			  "plot stitching",
//                           "interactive stitcher",
// 			  ""};
//   int tbState[10];
//   tbState[0] = plotVolumeGrid; 
//   tbState[1] = plotSurfaceGrid; 
//   tbState[2] = plotStitching;
//   tbState[3] = interactiveStitcher;
//   int numColumns=1;
//   dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

    const int numberOfTextStrings=7;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "ra,rb:";  sPrintF(textStrings[nt],"%g,%g",ra,rb);  nt++; 
    textLabels[nt] = "za,zb:";  sPrintF(textStrings[nt],"%g,%g",za,zb);  nt++; 
    textLabels[nt] = "x0,y0,rh:";  sPrintF(textStrings[nt],"%g,%g,%g",x0,y0,rh);  nt++; 
    textLabels[nt] = "hfreq,arg0:";  sPrintF(textStrings[nt],"%g,%g",hfreq,arg0);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

    // dialog.buildPopup(menu);
    gi.pushGUI(dialog);
  }
  


  aString help[] = 
    {
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line; 

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Square>"); // set the default prompt
  int len=0;
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getAnswer(answer,"");
 

    if( answer=="unit square" )
    {
      mappingType=unitSquare;

      setDomainDimension(2);
      setRangeDimension(2);

      setGridDimensions( axis1,11 );
      setGridDimensions( axis2,11 );

      mappingHasChanged();

    }
    else if( answer=="helical wire" )
    {
      mappingType=helicalWire;

      setDomainDimension(3);
      setRangeDimension(3);

      setGridDimensions( axis1,41 ); // theta
      setGridDimensions( axis2,7  ); // r 
      setGridDimensions( axis3,101); // z 

      signForJacobian=-1.;            // mapping is left handed

      setIsPeriodic(axis1,functionPeriodic );       
      setBoundaryCondition( Start,axis1,-1 );
      setBoundaryCondition(   End,axis1,-1 );

      setBoundaryCondition( Start,axis2, 1 );
      setBoundaryCondition(   End,axis2, 2 );
      setBoundaryCondition( Start,axis3, 3 );
      setBoundaryCondition(   End,axis3, 4 );

      if( !gi.readingFromCommandFile() )
      {
	printF("The helical wire is defined by \n"
	       "   x = xh(t) + rr*cos(theta)\n"
	       "   y = yh(t) + rr*sin(theta)\n"
	       "   y = yh(t) + rr*sin(theta)\n"
	       "   z = za + (zb-za)*t\n"
	       " where\n"
	       "   theta = 2*pi*(r(1)) \n"
	       "   rr = ra+(rb-ra)*(r(2))   (ra,rb are the inner and outer radii of the wire grid)\n"
	       "  t= r(3) \n"
	       "   (xh(t),yh(t)) = (x0,y0) + rh*(cos(arg),sin(arg))  (helix: center of the wire))\n"
	       "  arg = 2*pi*hfreq*(t-arg0) \n"
	       "  hfreq = helix frequency, arg0=helix angle offset\n"
	       "  rh= radius of the helix, phi0=offset for helix angle\n");
      }
      
      if( rp.getLength(0)!=10 )
      {
        rp.redim(10);
      }
      
      rp(0)=ra;
      rp(1)=rb;
      rp(2)=za;
      rp(3)=zb;
      rp(4)=x0;
      rp(5)=y0;
      rp(6)=rh;
      rp(7)=hfreq;
      rp(8)=arg0;

      mappingHasChanged();
      
    }
    else if( (len=answer.matches("ra,rb:")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&ra,&rb);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("ra,rb:",sPrintF(answer,"%g,%g",ra,rb));
      rp(0)=ra;
      rp(1)=rb;
      mappingHasChanged();
    }
    else if( (len=answer.matches("za,zb:")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&za,&zb);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("za,zb:",sPrintF(answer,"%g,%g",za,zb));
      rp(2)=za;
      rp(3)=zb;
      mappingHasChanged();
    }
    else if( (len=answer.matches("x0,y0,rh:")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&x0,&y0,&rh);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("x0,y0,rh:",sPrintF(answer,"%g,%g,%g",x0,y0,rh));
      rp(4)=x0;
      rp(5)=y0;
      rp(6)=rh;
      mappingHasChanged();
    }
    else if( (len=answer.matches("hfreq,arg0:")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&hfreq,&arg0);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("hfreq,arg0:",sPrintF(answer,"%g,%g",hfreq,arg0));
      rp(7)=hfreq;
      rp(8)=arg0;
      mappingHasChanged();
    }
    else if( answer=="fillet for two cylinders" )
    {
      mappingType=filletForTwoCylinders;
      setBasicInverseOption(canDoNothing); 

      if( rp.getLength(0)!=10 )
      {
        rp.redim(10); rp=0.;
      }
      rp(0)=1.;  // radius a
      rp(1)=.5;  // radius b

      gi.inputString(answer,"Enter a,b,d  (a=cyl1-radius, b=cyl2-radius, d=width width a>= b-width)");
      sScanF(answer(len,answer.length()-1),"%e %e %e",&rp(0),&rp(1),&rp(2));
      printF("Setting a=%e, b=%e, d=%e\n",rp(0),rp(1),rp(2));

      // if( stretch==NULL ) stretch = new StretchMapping(StretchMapping::exponentialBlend);

      if( stretch==NULL ) stretch = new StretchMapping(StretchMapping::hyperbolicTangent);
      stretch->setIsNormalized(false);
      real beta=5.; // 10.;
      stretch->setHyperbolicTangentParameters(.5,0.,.5,beta,.5);  


      setDomainDimension(2);
      setRangeDimension(3);

      setGridDimensions( axis1,41 ); // theta
      setGridDimensions( axis2,7  ); // r 

      setIsPeriodic(axis1,functionPeriodic );       
      setBoundaryCondition( Start,axis1,-1 );
      setBoundaryCondition(   End,axis1,-1 );

      setBoundaryCondition( Start,axis2, 1 );
      setBoundaryCondition(   End,axis2, 2 );
//       setBoundaryCondition( Start,axis3, 3 );
//       setBoundaryCondition(   End,axis3, 4 );

      mappingHasChanged();
    }
    else if( answer=="blade" )
    {
      mappingType=blade;
      setBasicInverseOption(canDoNothing); 

      if( rp.getLength(0)!=10 )
      {
        rp.redim(10); rp=0.;
        ip.redim(10); ip=0;
      }


      gi.inputString(answer,"Enter option (0=wind-turbine, 1=compressor-turbine)");
      sScanF(answer(len,answer.length()-1),"%i",&ip(0));
      printF("Setting option=%i\n",ip(0));

      // rp(0)=1.;  // radius a
      // rp(1)=.5;  // radius b

      // gi.inputString(answer,"Enter a,b,d  (a=cyl1-radius, b=cyl2-radius, d=width width a>= b-width)");
      // sScanF(answer(len,answer.length()-1),"%e %e %e",&rp(0),&rp(1),&rp(2));
      // printF("Setting a=%e, b=%e, d=%e\n",rp(0),rp(1),rp(2));

      // if( stretch==NULL ) stretch = new StretchMapping(StretchMapping::exponentialBlend);

      // if( stretch==NULL ) stretch = new StretchMapping(StretchMapping::hyperbolicTangent);
      // stretch->setIsNormalized(false);
      // real beta=5.; // 10.;
      // stretch->setHyperbolicTangentParameters(.5,0.,.5,beta,.5);  


      setDomainDimension(2);
      setRangeDimension(3);

      setGridDimensions( axis1,81 ); // axial
      setGridDimensions( axis2,51  ); // theta
      setBoundaryCondition( Start,axis1, 1 );
      setBoundaryCondition(   End,axis1, 2 );

      setIsPeriodic(axis2,functionPeriodic );       
      setBoundaryCondition( Start,axis2,-1 );
      setBoundaryCondition(   End,axis2,-1 );


      // The blade is singular at the end
      setTypeOfCoordinateSingularity( End  ,axis1,polarSingularity );
      setCoordinateEvaluationType( spherical,true );  // Mapping can be evaluated in spherical coordinates

      bladeSetup(mapInfo);

      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      rp.display("real parameters");
      ip.display("int parameters");
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" ||
             answer=="check" ||
             answer=="check inverse" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset

  if( buildDialog )
  {
    gi.popGUI(); // restore the previous GUI
  }
  return 0;
}
