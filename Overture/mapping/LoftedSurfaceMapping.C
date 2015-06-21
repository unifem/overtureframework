//-------------------------------------------------------------
// 
//  This Class defined Lofted Surfaces
//  
//-------------------------------------------------------------

#include "LoftedSurfaceMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "StretchMapping.h"
#include "NurbsMapping.h"
#include "SmoothedPolygon.h"

namespace
{
enum ProfileEnum
{
  noProfile,
  roundedTipProfile,
  flatTipProfile,
  flatDoubleTipProfile,
  windTurbineProfile
};

enum SectionEnum
{
  cylinderToJoukowskySections,
  twistedJoukowskySections,
  smoothedPolygonSections,
  shipHullSections
};


}



static StretchMapping *stretch=NULL;
static Mapping *pprofile=NULL;
static Mapping *crossSection=NULL;

LoftedSurfaceMapping::
LoftedSurfaceMapping( )
  : Mapping(2,2,parameterSpace,cartesianSpace)  
//===========================================================================
/// \brief  Build a user defined Mapping
//===========================================================================
{ 
  LoftedSurfaceMapping::className="LoftedSurfaceMapping";
  setName( Mapping::mappingName,"loftedSurfaceMapping");

  ip.redim(0);  // means that mapping is not initialized

  // mapping is from R^2 -> R^3 by default:
  setDomainDimension(2);
  setRangeDimension(3);

  setGridDimensions( axis1,31 );  // axial
  setGridDimensions( axis2,61 );  // angular 


  setBasicInverseOption(canDoNothing);  // we do not provide an inverse

  mappingHasChanged();

}

// Copy constructor is deep by default
LoftedSurfaceMapping::
LoftedSurfaceMapping( const LoftedSurfaceMapping & map, const CopyType copyType )
{
  LoftedSurfaceMapping::className="LoftedSurfaceMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    printF("LoftedSurfaceMapping:: sorry no shallow copy constructor, doing a deep! \n");
    *this=map;
  }
}

LoftedSurfaceMapping::
~LoftedSurfaceMapping()
{ 
  if( debug & 4 )
     printF("LoftedSurfaceMapping::Destructor called\n");

  delete stretch;
  delete pprofile;
  delete [] crossSection;
}

LoftedSurfaceMapping & LoftedSurfaceMapping::
operator=( const LoftedSurfaceMapping & x )
{
  if( LoftedSurfaceMapping::className != x.getClassName() )
  {
    cout << "LoftedSurfaceMapping::operator= ERROR trying to set a LoftedSurfaceMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class
  
  rp.redim(0); rp=x.rp;
  ip.redim(0); ip=x.ip;
  
  return *this;
}

void LoftedSurfaceMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
#ifdef USE_PPP
  Overture::abort("LoftedSurfaceMapping::map: ERROR: you should call mapS in parallel!");
#else
  mapS(r,x,xr,params);
#endif
}

// ==================================================================================================
/// \brief Evaluate the cross-section for the ship hull lofted surface
///
/// \param (s,t) (input) : axial and tangetial parameters, 0<= s <=1, and 0<= t <=1.
/// \param z (input) :  position in the "z" direction
/// \param zs,zt (input) :  d(z)/ds and d(z)/dt
///
/// \param xc[2] (output) : (x,y) values of the cross-section
//  \param xcs[2], xct[2] (output) : derivatives of (x,y) w.r.t. s and t.
// ==================================================================================================
int LoftedSurfaceMapping::
getShipHullCrossSection( const real s, const real t, const real z, const real & zs, const real & zt, 
                         real xc[2], real xcs[2], real xct[2] )
{
  real shipLength=rp(13); // length of the ship (from profile)  
  if( shipLength<0. ) shipLength=3.;
  real shipDepth=rp(14);
  if( shipDepth<0. ) shipDepth=.25;
  real shipBreadth=rp(15);
  if( shipBreadth<0. ) shipBreadth=.5;
  real hullSharpness=rp(16);
  if( hullSharpness<0. ) hullSharpness=.025;

  // real endOffset=.05;              // breadth at ends is .05*(1-.05)
  // real endOffset=.025;             // breadth at ends is .025*(1-.025)
  // real endOffset=.01;              // breadth at ends is .01*(1-.01)
  real endOffset=hullSharpness;       // breadth at ends is endOffset*(1-endOffset)

  const int shipBreadthOption       =ip(13);
  const int shipCrossSectionOption  =ip(14);

  // -------------------------
  // --- Ship Hull Breadth ---
  // -------------------------

  real breadth,breadths,shipDepths;
  
  if( shipBreadthOption==0 )
  {
    // Wigley Hull: parabolic breadth:
    //     b(z) = zn*(1-zn)
    //        0 <= zn  <= 1 ,   zn=z/length
    //
    // We want a rounded prow/stern so we make a finite width at the front and back.
    // Here is a simple way to do this:
    //     b(z) = zz*( 1 - zz )
    //        delta <= zz <= 1-2*delta,   zz=a*z+b 

    real offset=1.-2.*endOffset;
      
    real zz = offset*(z/shipLength) + endOffset;   
    breadth = 2.*shipBreadth * zz*(1.-zz);  // breadth of hull 

    real zzs = offset*zs/shipLength;
    
    breadths = 2.*shipBreadth * zzs*(1.-2.*zz);  // d(breadth)/ds
  }
  else
  {
    printF("getShipHullCrossSection:ERROR: unknown shipBreadthOption=%i\n",shipBreadthOption);
    OV_ABORT("ERROR");
  }
  
  
  // --------------------------------------------
  // --- Define the cross-section of the hull ---
  // --------------------------------------------
  if( shipCrossSectionOption==0 )
  {
    // 
    //  Here we choose an ellipsoid of constant depth
    //
    real theta=twoPi*t;
    real cost=cos(theta), sint=sin(theta);
    
    xc[0] = breadth*cost;
    xc[1] = shipDepth*sint;

    shipDepths=0.;            // d(depth)/ds  : depth is constant in s

    xcs[0] = breadths*cost;   // d(xc[0])/ds
    xcs[1] = shipDepths*sint;

    xct[0] = -twoPi*sint;     // d(xc[0])/dt 
    xct[1] = twoPi*cost;
      
  }
  else
  {
    printF("getShipHullCrossSection:ERROR: unknown shipCrossSectionOption=%i\n",shipBreadthOption);
    OV_ABORT("ERROR");
  }
      
      
  return 0;
      
}

void LoftedSurfaceMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
// ========================================================================================
// /Description:
//     Define the mapping here.
// /r (input) : parameter space coordinates on the unit line or unit square or unit cube.
// ========================================================================================
{
  if( params.coordinateType != cartesian )
  {
    printF("LoftedSurfaceMapping::map - coordinateType != cartesian : \n"
           "   NOTE: The derivatives of the mapping will be wrong if you are creating an orthographic patch\n");
  }
  
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );


  // sectionType:
  //    0 : cylinder to Joukowsky
  //    1 : twisted Joukowsky
  //    2 : smoothedPolygon
  const int sectionType=ip(0);
  const int profileType=ip(1);


  // -- First evaluate the profile --
  RealArray rProfile(I,1), xTop(I,2), xrTop(I,2,1), xBot(I,2), xrBot(I,2,1);
  
  const real sTip = rp(0);
  if( profileType!=noProfile )
  {
    assert( pprofile!=NULL );
    Mapping & profile = *pprofile;

    // Eval the top profile = profile(0..sTip)
    rProfile = r(I,0)*sTip;
    profile.mapS(rProfile,xTop,xrTop);
    // Eval the bottom profile
    rProfile = 1.- (1.-sTip)*r(I,0);
    profile.mapS(rProfile,xBot,xrBot);   
  }
  else
  {
    // no profile:
    xTop(I,0)=r(I,0);
    xTop(I,1)=1.;
    xrTop(I,0,0)=1.;
    xrTop(I,1,0)=0.;
    

    xBot(I,0)=r(I,0);
    xBot(I,1)=0.;
    xrBot(I,0,0)=1.;
    xrBot(I,1,0)=0.;
  }
  

  // ---- Here is a wind turbine blade ---
  if( sectionType==cylinderToJoukowskySections )
  {
    // Cylinder to Joukowsky sections

    // Joukowsky parameters
    real a=.85, d=.15, d0=d, delta=(-15.)*Pi/180; 
    real amRe=-d*sin(delta), amIm= d*cos(delta);

    real height=rp(4);
    real xMin = rp(5);
    real xMax = rp(6);
    real yMin = rp(7);
    real yMax = rp(8);
    real xAve=.5*(xMin+xMax);
    real yAve=.5*(yMin+yMax);
    real xScale=xMax-xMin;


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
      real cScale = .5*.75;
      real xc0 = cScale*(cost+1.), yc0=cScale*sint;


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
      int profileOption=1; // 0=transition Joukowsky to an ellipse at the end
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
      real yShift=profileOption==0 ? .025 : 0;
      real xc1=(zRe-zReMin)*zScale, yc1=(zIm-yAve)*zScale+yShift; 

      // Blend the initial circular cross-section (xc0,yc0) with the Joukowsky: 
      real sa=.15;      // transition position
      real beta=20.;    // transition exponent
      real phi0 = .5*(1.+tanh(beta*(s-sa)));  // phi0 : goes from 0 to 1 at s=sa

      // Here is the cross-section before scaling by the profile: 
      real xc = xc0*(1.-phi0) + xc1*phi0;
      real yc = yc0*(1.-phi0) + yc1*phi0;
	

      // --- Evaluate the profile and it's derivatives  ---

      // First eval the top profile = profile(0..sTip)
      // real s1 = sTip*s; rv(0,0)=s1;    
      // profile.mapS(rv,xv,xrv);
      real z    = xTop(i,0);               // actual axial position for this value of s 
      real zs   = xrTop(i,0,0)*sTip;
      real top  = xTop(i,1);
      real tops = xrTop(i,1,0)*sTip;

      // Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
      // real s2=1.- (1.-sTip)*s; rv(0,0)=s2;  // bot = profile(1..sTip)
      // profile.mapS(rv,xv,xrv);
      real bot  = xBot(i,1);
      real bots = xrBot(i,1,0)*(-(1.-sTip));

      real rad=top-bot;
      if( rad<0. )
      {
	printF("LoftedSurfaceMapping::WARNING: rad=%8.2e, s=%10.4e, top=%10.4e, bot=%10.4e\n",rad,r(i,0),top,bot);
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
      

	// xc0 = cScale*(cost+1.), yc0=cScale*sint;
	real xc0t = -twoPi*cScale*sint, yc0t=twoPi*cScale*cost;   // xc0t = d(xc0)/d(s1)
	real xct = xc0t*(1.-phi0) + xc1t*phi0;
	real yct = yc0t*(1.-phi0) + yc1t*phi0;
	real zt=0.;

	switch (params.coordinateType)
	{
	case cartesian: 
	  xr(i,0,0)=xcs*rad+xc*rads+bots;
	  xr(i,1,0)=ycs*rad+yc*rads;
	  xr(i,2,0)=zs;

	  xr(i,0,1)=xct*rad;
	  xr(i,1,1)=yct*rad;
	  xr(i,2,1)=zt;
	  break;

	case cylindrical:  // return -rho*d()/ds and (1/rho)*d()/d(theta)

	  // -- this needs to be worked out ---
	  // Normally "s" is the axial variable like "z"


	  //  zeta= sScale*rS-(1.-2.*startS)    // 2*( (endS-startS)*r+startS ) -1
	  //  rho=  SQRT(fabs(1.-SQR(zeta))) 
	  // real rho =sqrt( fabs(1.-s*s) );  // is this correct ? 
	  // real rho = rad; // is this correct ? 

	  xr(i,0,0)=xcs*rad+xc*rads+bots;
	  xr(i,1,0)=ycs*rad+yc*rads;
	  xr(i,2,0)=zs;

	  xr(i,0,1)=xct*rad;
	  xr(i,1,1)=yct*rad;
	  xr(i,2,1)=zt;

	  break;

	default:
	  printF("LoftedSurfaceMapping::map: ERROR not implemented for coordinateType = %i\n",
		 (int)params.coordinateType);
	}
	

      }
    }
  }
  else if( sectionType==twistedJoukowskySections )
  {
    // --- Twisted Joukowsky sections ---

    // -- compressor blade --
      
    // Joukowsky parameters
    real a=.85, d=.15, d0=d, delta=(-15.)*Pi/180; 
    real sind=sin(delta), cosd=cos(delta);
    real amRe=-d*sind, amIm= d*cosd;

    real height=rp(4);
    real xMin = rp(5);
    real xMax = rp(6);
    real yMin = rp(7);
    real yMax = rp(8);
    real xAve=.5*(xMin+xMax);
    real yAve=.5*(yMin+yMax);
    real xScale=xMax-xMin;

    RealArray rv(1,1), xv(1,2), xrv(1,2,1);
    real s0,s1,s2;
    for( int i=I.getBase(); i<=I.getBound(); i++ )
    {
      real s=r(i,0);
      real theta=twoPi*r(i,1);    // angular

      real cost = cos(theta), sint = sin(theta); 

      // First eval the top profile = profile(0..sTip)
      real z    = xTop(i,0);               // actual axial position for this value of s 
      real zs   = xrTop(i,0,0)*sTip;
      real top  = xTop(i,1);
      real tops = xrTop(i,1,0)*sTip;

      // Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
      real bot  = xBot(i,1);
      real bots = xrBot(i,1,0)*(-(1.-sTip));
      real zt=0.;
      
//       // Eval the top profile = profile(0..sTip)
//       s1 = sTip*s; rv(0,0)=s1;    
//       profile.mapS(rv,xv,xrv);
//       real z=xv(0,0);               // actual axial position for this value of s 
//       real zs = xrv(0,0,0)*sTip;
//       real zt=0.;
//       real top = xv(0,1);
//       real tops = xrv(0,1,0)*sTip;

//       // Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
//       s2=1.- (1.-sTip)*s; rv(0,0)=s2;  // bot = profile(1..sTip)
//       profile.mapS(rv,xv,xrv);
//       real bot = xv(0,1);
//       real bots = xrv(0,1,0)*(-(1.-sTip));

      real rad=top-bot;
      real rads=tops-bots;
      if( rad<0. )
      {
	printF("WARNING: rad=%8.2e, s1=%10.4e, s2=%10.4e, top=%10.4e, bot=%10.4e\n",rad,s1,s2,top,bot);
      }

	
      // -- evaluate the surface (cross section)

      // evalCompressorBlade( s,z, xc,yc,zc );

      // Here is the scaled Joukowsky cross-section: 

      real wRe=a*cost+amRe, wIm=(-a)*sint+amIm;
      real wNormI=1./(wRe*wRe+wIm*wIm);
      real zRe=wRe+wRe*wNormI,  zIm=wIm-wIm*wNormI;

      const real scale=1./xScale;
      real x0=(zRe-xAve)*scale, y0=(zIm-yAve)*scale; // Joukowsky : (x0,y0)
      real x0s, y0s, x0t, y0t;

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
      x0t=ztRe*scale, y0t=ztIm*scale;    // Joukowsky derivatives wrt tangential variable 

      // Step I: multiply the sections by the profile to form the tip
      real xc,yc;
      xc = x0*rad;
      yc = y0*rad;

      real xcs = x0*rads;  // d(x)/ds
      real ycs = y0*rads;  
      real xct = x0t*rad;
      real yct = y0t*rad;
      

      // Step II: 
      //    - Adjust the chord as a function of zeta
      //    - add a twist to the wing
      //    - flex the wing

      real zeta=z/height;  // zeta is the normalized z coordinate
      real zeta2=zeta*zeta;
      real zeta3=zeta*zeta2;
      real zetas = zs/height;  // d(zeta)/ds

      // -- evaluate the chord as a function of zeta ---
      const real chorda=1., chordb=.7;
      real chord = chorda*(1.-zeta2)+ chordb*zeta2;
      real chords = (chorda*(-2.*zeta)+ chordb*2.*zeta)*zetas;  // derivative wrt s
      // for now scale x and y in the same way:
      x0=xc, y0=yc, x0s=xcs, y0s=ycs; 
      xc = x0*chord;  
      yc = y0*chord;  

      xcs =x0s*chord+x0*chords, ycs =y0s*chord+y0*chords;
      xct*=chord, yct*=chord;

      // --- evaluate the twist as a function of zeta ---
      real anglea=0., angleb=-30.*Pi/180.;
      real angle = anglea*(1.-zeta2)+ angleb*zeta2;  // twist function
      real angles = (anglea*(-2.*zeta)+ angleb*2.*zeta)*zetas;
      real cosa=cos(angle), sina=sin(angle);
      real cosas=-sina*angles, sinas=cosa*angles;

      x0=xc, y0=yc, x0s=xcs, y0s=ycs, x0t=xct, y0t=yct; 
      // NOTE: here we rotate about (x,y)=(0,0)
      xc = x0*cosa - y0*sina;   
      yc = x0*sina + y0*cosa;
      xcs = x0s*cosa - y0s*sina + x0*cosas - y0*sinas; 
      ycs = x0s*sina + y0s*cosa + x0*sinas + y0*cosas;

      xct = x0t*cosa - y0t*sina;
      yct = x0t*sina + y0t*cosa;

      // -- add the flex in the wing --
      real yFlexa=0., yFlexb=.25;
      real yFlex = yFlexa*(1.-zeta3)+ yFlexb*zeta3;  // flex function
      real yFlexs = (yFlexa*(-3.*zeta2)+ yFlexb*3.*zeta2)*zetas;  
       
      yc += yFlex;
      ycs += yFlexs;

      if( computeMap )
      {
	// scale (xc,yc) by the profile 
	x(i,0)=xc;
	x(i,1)=yc;
	x(i,2)=z;
      }

      if( computeMapDerivative )
      {
	xr(i,0,0)=xcs;
	xr(i,1,0)=ycs;
	xr(i,2,0)=zs;

	xr(i,0,1)=xct;
	xr(i,1,1)=yct;
	xr(i,2,1)=zt;
      }
	
    }
      

  }
  else if( sectionType==smoothedPolygonSections )
  {
    // -- Smoothed polygon cross section  --

    assert( crossSection!=NULL );
      
    real sectionExponent = rp(12);
    if( sectionExponent<=0 ) sectionExponent=50.;


    RealArray rt(I,1),x0(I,3),xr0(I,3,1);

    rt(I,0)=r(I,1);  // angular direction
    crossSection[0].mapS(rt,x0,xr0);
      
//       RealArray x1(I,3),xr1(I,3,1);
//       crossSection[1].map(rt,x1,xr1);

//       RealArray x2(I,3),xr2(I,3,1);
//       crossSection[2].map(rt,x2,xr2);



    RealArray rv(1,1), xv(1,2), xrv(1,2,1);
    real s0,s1,s2;
    for( int i=I.getBase(); i<=I.getBound(); i++ )
    {
      real s=r(i,0);

      // First eval the top profile = profile(0..sTip)
      real z    = xTop(i,0);               // actual axial position for this value of s 
      real zs   = xrTop(i,0,0)*sTip;
      real top  = xTop(i,1);
      real tops = xrTop(i,1,0)*sTip;

      // Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
      real bot  = xBot(i,1);
      real bots = xrBot(i,1,0)*(-(1.-sTip));

//       // Eval the top profile = profile(0..sTip)
//       s1 = sTip*s; rv(0,0)=s1;    
//       profile.mapS(rv,xv,xrv);
//       real z=xv(0,0);               // actual axial position for this value of s 
//       real zs = xrv(0,0,0)*sTip;
//       real top = xv(0,1);
//       real tops = xrv(0,1,0)*sTip;

//       // Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
//       s2=1.- (1.-sTip)*s; rv(0,0)=s2;  // bot = profile(1..sTip)
//       profile.mapS(rv,xv,xrv);
//       real bot = xv(0,1);
//       real bots = xrv(0,1,0)*(-(1.-sTip));

      real rad=top-bot, rads=tops-bots;
      if( rad<0. )
      {
	printF("WARNING: rad=%8.2e, s1=%10.4e, s2=%10.4e, top=%10.4e, bot=%10.4e\n",rad,s1,s2,top,bot);
      }

	
      real xp = x0(i,0), xpt=xr0(i,0,0);
      real yp = x0(i,1), ypt=xr0(i,1,0);
      // real zp = x0(i,2), zpt=xr0(i,2,0);
      
      
      // transition the smoothed polygon to a circle
      real theta=twoPi*(r(i,1)-.25);  // angular : note smoothed polygon starts at middle bottom
      real cost = cos(theta), sint = sin(theta);  
      
      real cScale = .5;
      real xc0 = cScale*cost, yc0=cScale*sint;
      real xc0t = -cScale*twoPi*sint, yc0t=cScale*twoPi*cost;
      real zt=0.;
      
      // transition the smoothed polygon to a circle at one or both ends 

      // Here is the cross-section before scaling by the profile: 
      real xc,yc, xcs,ycs, xct,yct, s0,phi0,phi0s, s1,phi1,phi1s, beta0,beta1;
      if( profileType==flatDoubleTipProfile )
      {
        // transition to a circle at both singular ends
	s0=.1, s1=.9;        // transition positions -- this should be around the corner of the flat face profile
	beta0=sectionExponent, beta1=sectionExponent;    // transition exponents
	phi0 = .5*( 1. +tanh(beta0*(s-s0)));  // phi0 : goes from 0 to 1 at s=s0
	real sech0 = 1./cosh(beta0*(s-s0));
	phi0s = .5*beta0*sech0*sech0;

	phi1 = .5*( 1. +tanh(beta1*(s-s1)));  // phi1 : goes from 0 to 1 at s=s1
	real sech1 = 1./cosh(beta1*(s-s1));
	phi1s = .5*beta1*sech1*sech1;

	xc = (1.-phi0)*xc0 + xp*phi0*(1.-phi1) + xc0*phi1;
	yc = (1.-phi0)*yc0 + yp*phi0*(1.-phi1) + yc0*phi1;

	xcs = (-phi0s)*xc0 + xp*phi0s*(1.-phi1) + xp*phi0*(-phi1s) + xc0*phi1s;
	ycs = (-phi0s)*yc0 + yp*phi0s*(1.-phi1) + yp*phi0*(-phi1s) + yc0*phi1s;

	xct = (1.-phi0)*xc0t + xpt*phi0*(1.-phi1) + xc0t*phi1;
	yct = (1.-phi0)*yc0t + ypt*phi0*(1.-phi1) + yc0t*phi1;

      }
      else
      {
        // transition to a circle at the right singular end
	s1=.9;        // transition position -- this should be around the corner of the flat face profile
	beta1=sectionExponent;    // transition exponent
	phi1 = .5*( 1. +tanh(beta1*(s-s1)));  // phi1   goes from 0 to 1 at s=s1
	real sech1 = 1./cosh(beta1*(s-s1));
	phi1s = .5*beta1*sech1*sech1;
        
	xc = xp*(1.-phi1) + xc0*phi1;
	yc = yp*(1.-phi1) + yc0*phi1;

	xcs = xp*(-phi1s) + xc0*phi1s;
	ycs = yp*(-phi1s) + yc0*phi1s;

	xct = xpt*(1.-phi1) + xc0t*phi1;
	yct = ypt*(1.-phi1) + yc0t*phi1;
      }
      
      if( computeMap )
      {
	// scale (xc,yc) by the profile 
	x(i,0)=xc*rad;
	x(i,1)=yc*rad;
	x(i,2)=z;
      }

      if( computeMapDerivative )
      {
	xr(i,0,0)=xcs*rad+xc*rads;
	xr(i,1,0)=ycs*rad+yc*rads;
	xr(i,2,0)=zs;

	xr(i,0,1)=xct*rad;
	xr(i,1,1)=yct*rad;
	xr(i,2,1)=zt;
      }
	
    }
      

  }
  else if( sectionType==shipHullSections )
  {
    // ------------------------------------
    // -- cross sections for a ship hull --
    // ------------------------------------

    RealArray rv(1,1), xv(1,2), xrv(1,2,1);
    real xc[2], xcs[2], xct[2];
    
    for( int i=I.getBase(); i<=I.getBound(); i++ )
    {
      // real s=r(i,0);             // axial parameter
      // real theta=twoPi*r(i,1);    // angular
      // real cost = cos(theta), sint = sin(theta); 

      // --- Evaluate the profile and it's derivatives  ---

      // First eval the top profile = profile(0..sTip)
      // real s1 = sTip*s; rv(0,0)=s1;    
      // profile.mapS(rv,xv,xrv);
      real z    = xTop(i,0);               // actual axial position for this value of s 
      real zs   = xrTop(i,0,0)*sTip;
      real top  = xTop(i,1);
      real tops = xrTop(i,1,0)*sTip;
      real zt=0.;
      
      // Bottom profile: for now we assume that xv(0,0) on bot is approx. z on top ** fix me **
      // real s2=1.- (1.-sTip)*s; rv(0,0)=s2;  // bot = profile(1..sTip)
      // profile.mapS(rv,xv,xrv);
      real bot  = xBot(i,1);
      real bots = xrBot(i,1,0)*(-(1.-sTip));

      real rad=top-bot;
      real rads = tops-bots;
      if( rad<0. )
      {
	printF("LoftedSurfaceMapping::WARNING: rad=%8.2e, s=%10.4e, top=%10.4e, bot=%10.4e\n",rad,r(i,0),top,bot);
      }

      getShipHullCrossSection( r(i,0), r(i,1), z,zs,zt, xc,xcs,xct );

      if( computeMap )
      {
	x(i,0)=xc[0]*rad;
	x(i,1)=xc[1]*rad;
	x(i,2)=z;
      }
      if( computeMapDerivative )
      {
	switch (params.coordinateType)
	{
	case cartesian: 
	  xr(i,0,0)=xcs[0]*rad+xc[0]*rads;
	  xr(i,1,0)=xcs[1]*rad+xc[1]*rads;
	  xr(i,2,0)=zs;

	  xr(i,0,1)=xct[0]*rad;
	  xr(i,1,1)=xct[1]*rad;
	  xr(i,2,1)=zt;
	  break;

	case cylindrical:  // return -rho*d()/ds and (1/rho)*d()/d(theta)

          // ** FINISH ME **
          // *** THIS IS WRONG BUT CURRENTLY NOT USED ***

	  xr(i,0,0)=xcs[0]*rad+xc[0]*rads;
	  xr(i,1,0)=xcs[1]*rad+xc[1]*rads;
	  xr(i,2,0)=zs;

	  xr(i,0,1)=xct[0]*rad;
	  xr(i,1,1)=xct[1]*rad;
	  xr(i,2,1)=zt;

	  break;

	default:
	  printF("LoftedSurfaceMapping::map: ERROR not implemented for coordinateType = %i\n",
		 (int)params.coordinateType);
	}
	

      }
    }
  }

  else
  {
    printF("LoftedSurfaceMapping::mapS: ERROR: unknown sectionType=%i\n",sectionType);
    OV_ABORT("error");
  }
  
}

// ==========================================================================
//  Setup routine for the profiles.
// ==========================================================================
int LoftedSurfaceMapping::
profileSetup(MappingInformation & mapInfo)
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const int sectionType=ip(0);
  const int profileType=ip(1);

  // The blade is singular at the end
  if( profileType!=noProfile )
  {
    setTypeOfCoordinateSingularity( End  ,axis1,polarSingularity );
  }
  else
  { // there is no singular at the end with no profile.
    setTypeOfCoordinateSingularity( End  ,axis1,noCoordinateSingularity );
  }
  
  if( profileType==flatDoubleTipProfile )
  { // double ended flat tip has a singularity at the start too
    setTypeOfCoordinateSingularity( Start,axis1,polarSingularity );
  }
  else
  {
    setTypeOfCoordinateSingularity( Start,axis1,noCoordinateSingularity );
  }
  

  int numPts=-1;
  if( profileType==roundedTipProfile )
    numPts=11+11-4+11;
  else if( profileType==noProfile || profileType==flatTipProfile || profileType==flatDoubleTipProfile )
    numPts=1; // note used
  else if( profileType==windTurbineProfile )
    numPts=5+2+11+11-4+11;
  else
  {
    OV_ABORT("ERROR: Unexpected profileType");
  }
  

  RealArray xp(numPts,2);


  int n=0; //  counts points
  real r,x,y,theta;

  
  // profile: 
  //   0 = rounded tip
  //   1 = flat tip
  //   2 = double flat tip
  //   3 = wind turbine

  delete pprofile; pprofile=NULL;

  if( profileType==roundedTipProfile )
  {
    // ---- Here is a flat profile with a rounded tip -----
    
    real height=3.;

    // tip: ellipse
    real rada=1., radb=.5;

    real xa=0., ya=1., xb=height-rada, yb=ya; int nn=11; 
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    // tip: ellipse
    real x0=xb, y0=ya-radb,  theta0=Pi*.5, theta1=-.5*Pi; nn=11;
    for( int i=2; i<nn-2; i++ ){ r=i/(nn-1.); theta=theta0+(theta1-theta0)*r; x=x0+rada*cos(theta); y=y0+radb*sin(theta); xp(n,0)=x; xp(n,1)=y; n++; }
    xa=xb; ya=0.; xb=0.; yb=.0; nn=11; 
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    
    rp(4)=height; // +rada;

    assert( numPts==n );

    assert( pprofile==NULL );

    NurbsMapping & profile = *new NurbsMapping;

    pprofile = &profile;
    profile.setDomainDimension(1);
    profile.setRangeDimension(2);
    profile.interpolate(xp);

    profile.setGridDimensions(0,101);


  }
  else if( profileType==flatTipProfile  || profileType==flatDoubleTipProfile )
  {

    // -- straight profile with flat tip --

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


    // Use a profile from the mapping list:
    // pprofile = mapInfo.mappingList[0].mapPointer;

    real height;
    if( sectionType==shipHullSections )
      height = rp(13);  // shipLength
    else
      height = rp(10);  // bladeLength

    if( height<=0 ) height=3.;
    rp(4)=height;

    real chord = rp(11);
    if( chord<=0 ) chord=1.;
    const real hw= chord*.5;  // chord half-width

    assert( pprofile==NULL );

    SmoothedPolygon & smoothedPolygon = *new SmoothedPolygon;
    pprofile = &smoothedPolygon;
     
    // set the verticies of the smoothed polygon profile
    RealArray xv, sharpness, tStretch;
    if( profileType==flatTipProfile )
    {
      // flat tip on one end 
      int nv=4;
      xv.redim(nv,2);  sharpness.redim(nv); tStretch.redim(2,nv);
      sharpness=40.;
      xv(0,0)=0.;     xv(0,1)= hw;  tStretch(0,0)= 0.; tStretch(1,0)=20.;
      xv(1,0)=height; xv(1,1)= hw;  tStretch(0,1)=.10; tStretch(1,1)=20.;
      xv(2,0)=height; xv(2,1)=-hw;  tStretch(0,2)=.10; tStretch(1,2)=20.;
      xv(3,0)=0.;     xv(3,1)=-hw;  tStretch(0,3)= 0.; tStretch(1,3)=20.;
    }
    else
    {
      // flat tip on both ends
      int nv=6;
      xv.redim(nv,2); sharpness.redim(nv); tStretch.redim(2,nv);
      sharpness=40.;
      xv(0,0)=0.;     xv(0,1)=0.;   tStretch(0,0)= 0.; tStretch(1,0)=20.;
      xv(1,0)=0.;     xv(1,1)= hw;  tStretch(0,1)=.10; tStretch(1,1)=20.;
      xv(2,0)=height; xv(2,1)= hw;  tStretch(0,2)=.10; tStretch(1,2)=20.;
      xv(3,0)=height; xv(3,1)=-hw;  tStretch(0,3)=.10; tStretch(1,3)=20.;
      xv(4,0)=0.;     xv(4,1)=-hw;  tStretch(0,4)=.10; tStretch(1,4)=20.;
      xv(5,0)=0.;     xv(5,1)=0.;   tStretch(0,5)= 0.; tStretch(1,5)=20.;
    }
    
// setPolygon( const RealArray & xv,
//             const RealArray & sharpness /* = Overture::nullRealArray() */,
//             const real normalDist /* = 0. */,
//             const RealArray & variableNormalDist /* = Overture::nullRealArray() */,
//             const RealArray & tStretch /* = Overture::nullRealArray() */,
//             const RealArray & rStretch /* = Overture::nullRealArray() */,
//             const bool correctForCorners /* = false */ )

    smoothedPolygon.setPolygon(xv,sharpness,0.,Overture::nullRealArray(),tStretch);
    smoothedPolygon.setDomainDimension(1); // make a curve 

    if( false )
      smoothedPolygon.update(mapInfo);

  }
  else if( profileType==windTurbineProfile )
  {
    // ---- Here is a profile for a wind turbine blade ---

    //                     + --------
    //                  +             -------------
    //                +                              -----------------
    //              +                                                  +
    //   --------+                                                       +
    //                                                                 +
    //   ------------------------------------------------------------+

    real bladeLength = rp(10);
    if( bladeLength<=0 ) bladeLength=9.;

    real xa=0., ya=.8, xb=.8, yb=.8; int nn=5;
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    x=1.4; y=1.1;   xp(n,0)=x; xp(n,1)=y; n++;
    x=1.8; y=1.45;  xp(n,0)=x; xp(n,1)=y; n++;
    xa=3.; ya=1.6; xb=bladeLength; yb=.8; nn=11; 
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    // tip: ellipse
    real x0=bladeLength, y0=.4, rada=.8, radb=.4, theta0=Pi*.5, theta1=-.5*Pi; nn=11;
    for( int i=2; i<nn-2; i++ ){ r=i/(nn-1.); theta=theta0+(theta1-theta0)*r; x=x0+rada*cos(theta); y=y0+radb*sin(theta); xp(n,0)=x; xp(n,1)=y; n++; }
    xa=bladeLength; ya=0.; xb=0.; yb=.0; nn=11; 
    for( int i=0; i<nn; i++ ){ r=i/(nn-1.); x=xa+(xb-xa)*r; y=ya + (yb-ya)*r; xp(n,0)=x; xp(n,1)=y; n++; }
    
    rp(4)=bladeLength+rada;

    assert( numPts==n );

    assert( pprofile==NULL );
    
    NurbsMapping & profile = *new NurbsMapping;

    pprofile = &profile;
    profile.setDomainDimension(1);
    profile.setRangeDimension(2);
    profile.interpolate(xp);

    profile.setGridDimensions(0,101);


  }

  
  // View the profile: 
  if( false && pprofile!=NULL )
  {
    Mapping & profile = *pprofile;
    profile.update(mapInfo);
  }
  
  // We need to define curves for the top and bottom of the profile:

  // first find the tip of the profile:  (where x'=0)
  real sTip=.5;
  if( profileType==noProfile )
  {
    sTip=1.;  // with no profile x=s
  }
  else if( profileType==roundedTipProfile || profileType==windTurbineProfile )
  {
    assert( pprofile!=NULL );
    Mapping & profile = *pprofile;


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
  
  return 0;
  
}

// ==========================================================================
//  Setup routine for the sections
// ==========================================================================
int LoftedSurfaceMapping::
sectionSetup(MappingInformation & mapInfo)
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const int sectionType=ip(0);
  const int profileType=ip(1);

  delete crossSection; crossSection=NULL;

  // -- Here we the section curves for some options --

  if( sectionType==cylinderToJoukowskySections || sectionType==twistedJoukowskySections )
  {
    // --- Initialize the Joukowsky sections ---

    // Notes: 
    //  - rotation and chord should depend on z and not s
    //  -- increase number of cross-sections or define through a formula  ***


    // Cross section : Joukowsky parameters
    real a=.85, d=.15, d0=d, delta=(-15.)*Pi/180; 
    real amRe=-d*sin(delta), amIm= d*cos(delta);
    real sind=sin(delta), cosd=cos(delta);

    // Compute the "min" and max values from the profile 

    const int nt = 101;
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

      xMin=min(xMin,xc1);
      xMax=max(xMax,xc1);
      yMin=min(yMin,yc1);
      yMax=max(yMax,yc1);
    }
    // scale profile to lie on [-.5,.5] in the x direction, and be centered about 0 in the y direction
    // real xAve = .5*(xMin+xMax);
    // real yAve = .5*(yMax+yMin);

    // *** save these:
    rp(5)=xMin;
    rp(6)=xMax;
    rp(7)=yMin;
    rp(8)=yMax;

//     if( false )
//     {
//       // Add cross sections to the mapping list so we can plot them all
//       mapInfo.mappingList.addElement(profile);
//       mapInfo.mappingList.addElement(crossSection[0]);
//       mapInfo.mappingList.addElement(crossSection[1]);
//       mapInfo.mappingList.addElement(crossSection[2]);
//       gi.erase();
//       viewMappings(mapInfo);
//     }
    
  }


  if( sectionType==smoothedPolygonSections )
  {
    // smoothed polygon section

    assert( crossSection==NULL );
    SmoothedPolygon & smoothedPolygon = *new SmoothedPolygon;
    crossSection = &smoothedPolygon;

    // set the vertices of the smoothed polygon profile
    const int nv=6;     // number of vertices
    RealArray xv(nv,2), sharpness(nv), tStretch(2,nv);;
    sharpness=40.;
    xv(0,0)= .0;   xv(0,1)=-.5;  tStretch(0,0)=.0 ; tStretch(1,0)=20.;
    xv(1,0)= .5;   xv(1,1)=-.5;	 tStretch(0,1)=.10; tStretch(1,1)=20.;
    xv(2,0)= .5;   xv(2,1)= .5;	 tStretch(0,2)=.10; tStretch(1,2)=20.;
    xv(3,0)=-.5;   xv(3,1)= .5;	 tStretch(0,3)=.10; tStretch(1,3)=20.;
    xv(4,0)=-.5;   xv(4,1)=-.5;	 tStretch(0,4)=.10; tStretch(1,4)=20.;
    xv(5,0)=  0;   xv(5,1)=-.5;	 tStretch(0,5)=.0 ; tStretch(1,5)=20.;
    smoothedPolygon.setPolygon(xv,sharpness,0.,Overture::nullRealArray(),tStretch);
    smoothedPolygon.setDomainDimension(1); // make a curve 

    if( false )
      smoothedPolygon.update(mapInfo);


  }
  

  
  return 0;
  
}



  

//=================================================================================
// get a mapping from the database
//=================================================================================
int LoftedSurfaceMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( LoftedSurfaceMapping::className,"className" ); 
  if( LoftedSurfaceMapping::className != "LoftedSurfaceMapping" )
  {
    cout << "LoftedSurfaceMapping::get ERROR in className!" << endl;
  }
  int temp;
  subDir.get( rp,"rp" );
  subDir.get( ip,"ip" );

  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0; 
}

int LoftedSurfaceMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( LoftedSurfaceMapping::className,"className" );
  subDir.put( rp,"rp" );
  subDir.put( ip,"ip" );            
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *LoftedSurfaceMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==LoftedSurfaceMapping::className )
    retval = new LoftedSurfaceMapping();
  return retval;
}

void LoftedSurfaceMapping::
getParameters(IntegerArray & ipar, RealArray & rpar ) const
//===========================================================================
/// \details  
///    Return the current values for the parameters.
//===========================================================================
{
  ipar.redim(0); ipar=ip;
  rpar.redim(0); rpar=rp;
}



void LoftedSurfaceMapping::
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
int LoftedSurfaceMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  // -- If the lofted surface has not been created yet then initialize the mapping 
  //    to use the default lofted surface --
  real bladeLength=-1.;   // <0 means use default value
  real chord=-1.;
  real sectionExponent=-1.;  // used to transition between cross sections
  real shipLength=-1., shipDepth=-1., shipBreadth=-1.;
  real hullSharpness=.025;  // determines the sharpness of the Wigley Hull. 

  int shipCrossSectionOption=0, shipBreadthOption=0;
  
  if( rp.getLength(0)==0  )
  {
    rp.redim(20); rp=0.;
    ip.redim(20); ip=0;

    // set defaults:
    ip(0)=cylinderToJoukowskySections;
    ip(1)=roundedTipProfile;
    
    ip(13)=shipBreadthOption;
    ip(14)=shipCrossSectionOption;

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

    profileSetup(mapInfo);
    sectionSetup(mapInfo);

    mappingHasChanged();
  }
  else
  {
    bladeLength=rp(10)>0 ? rp(10) : -1.;
    chord=rp(11)>0 ? rp(11) : -1.;
    sectionExponent=rp(12)>0 ? rp(12) : -1.;

    shipLength   = rp(13)>0. ? rp(13) :  3.;
    shipDepth    = rp(14)>0. ? rp(14) : .25;
    shipBreadth  = rp(15)>0. ? rp(15) : .5;
    hullSharpness= rp(16)>0. ? rp(16) : .025;
    
  }
  
      

  char buff[180];  // buffer for sprintf

  GUIState dialog;
  bool buildDialog=true;
  if( buildDialog )
  {
    dialog.setWindowTitle("LoftedSurfaceMapping");
    dialog.setExitCommand("exit", "exit");

    // option menus
    dialog.setOptionMenuColumns(1);

    aString opCommand1[] = {"cylinder to Joukowsky sections",
                            "twisted Joukowsky sections",
                            "smoothed polygon sections",
                            "ship hull sections",
			    ""};
    


    int sectionType = ip(0);
    dialog.addOptionMenu( "sections:", opCommand1, opCommand1, sectionType); 


    aString opCommand2[] = {"no profile",
                            "rounded tip profile",
                            "flat tip profile",
                            "flat double tip profile",
                            "wind turbine profile",
			    ""};
    
    int profileType = ip(1);
    dialog.addOptionMenu( "profile:", opCommand2, opCommand2, profileType); 


    aString cmds[] = {"edit profile",
                      "edit section",
                      "lines",
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
    int numberOfPushButtons=11;  // number of entries in cmds
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

    const int numberOfTextStrings=20;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "blade length:";  sPrintF(textStrings[nt],"%g",bladeLength);  nt++; 
    textLabels[nt] = "chord:";  sPrintF(textStrings[nt],"%g",chord);  nt++; 
    textLabels[nt] = "section exponent:";  sPrintF(textStrings[nt],"%g",sectionExponent);  nt++; 
    textLabels[nt] = "ship length:";  sPrintF(textStrings[nt],"%g",shipLength);  nt++; 
    textLabels[nt] = "ship depth:";  sPrintF(textStrings[nt],"%g,",shipDepth);  nt++; 
    textLabels[nt] = "ship breadth:";  sPrintF(textStrings[nt],"%g,",shipBreadth);  nt++; 
    textLabels[nt] = "hull sharpness:";  sPrintF(textStrings[nt],"%g,",hullSharpness);  nt++; 

    // null strings terminal list
    assert( nt<numberOfTextStrings );
    textLabels[nt]="";   textStrings[nt]="";  
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

  bool plotObject=ip.getLength(0)>0;

  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  gi.appendToTheDefaultPrompt("LoftedSurface>"); // set the default prompt
  int len=0;
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getAnswer(answer,"");

    if( answer=="cylinder to Joukowsky sections" ||
        answer=="twisted Joukowsky sections" ||
        answer=="smoothed polygon sections" || 
        answer=="ship hull sections" )
    {

      assert( ip.getLength(0)>0 && rp.getLength(0)>0 );

      if( answer=="cylinder to Joukowsky sections" )
        ip(0)=0;
      else if( answer=="twisted Joukowsky sections" )
	ip(0)=1;
      else if( answer=="smoothed polygon sections" )
	ip(0)=2;
      else if( answer=="ship hull sections" )
	ip(0)=3;
      else
      {
        printF("ERROR: unexpected answer=%s\n",(const char*)answer);
	OV_ABORT("error");
      }
      
      if( answer=="ship hull sections" )
      {
        // *** ADD MORE BREADTH AND CROSS-SECTION OPTIONS HERE***
	printF("Here are the current options for defining the ship hull:\n"
               "  Ship breadth option:\n"
               "    0 = Wigley Hull : b(z) = shipBreadth * 4* zs*( 1 - zs ) , zs=z/shipLength\n"
               "  Ship cross-section option:\n"
               "    1 : elliptical cross section with constant depth\n");
	
	gi.inputString(line,"Enter breadthOption, crossSectionOption");
	sScanF(line,"%i %i",&shipBreadthOption,&shipCrossSectionOption);
	printF(" Setting shipBreadthOption=%i, shipCrossSectionOption=%i\n",shipBreadthOption,shipCrossSectionOption);
	
	ip(13)=shipBreadthOption;   
	ip(14)=shipCrossSectionOption;
      }
      dialog.getOptionMenu("sections:").setCurrentChoice(ip(0));


      sectionSetup(mapInfo);

      mappingHasChanged();
 
    }
    else if( answer=="no profile" ||
             answer=="rounded tip profile" ||
	     answer=="flat tip profile" ||
             answer=="flat double tip profile" ||

	     answer=="wind turbine profile" )
    {

      assert( ip.getLength(0)>0 && rp.getLength(0)>0 );

      if( answer=="no profile" )
        ip(1)=noProfile;  
      else if( answer=="rounded tip profile" )
        ip(1)=roundedTipProfile;  
      else if( answer=="flat tip profile" )
        ip(1)=flatTipProfile;
      else if( answer=="flat double tip profile" )
        ip(1)=flatDoubleTipProfile;
      else if( answer=="wind turbine profile" ) 
	ip(1)=windTurbineProfile;
      else
      {
        printF("ERROR: unexpected answer=%s\n",(const char*)answer);
	OV_ABORT("error");
      }
      dialog.getOptionMenu("profile:").setCurrentChoice(ip(1));
      
      profileSetup(mapInfo);

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
    else if( answer=="edit profile" )
    {
      if( pprofile==NULL )
      {
	printF("WARNING: You must choose a profile type before you can edit it\n");
	continue;
      }
      
      Mapping & profile = *pprofile;
      profile.update(mapInfo);
      mappingHasChanged();
    }
    else if( answer=="edit section" )
    {
      if( crossSection==NULL )
      {
	printF("WARNING: Currently you can only edit the 'smoothed polygon sections'\n");
	continue;
      }
      
      crossSection->update(mapInfo);
      mappingHasChanged();
    }
    else if( dialog.getTextValue(answer,"blade length:","%e",bladeLength) )
    {
      printF("Setting the blade length =%9.3e \n",bladeLength);
      printF("You should re-choose the profile and/or section\n");
      rp(10)=bladeLength;
    }
    else if( dialog.getTextValue(answer,"chord:","%e",chord) )
    {
      printF("Setting the chord =%9.3e \n",chord);
      printF("You should re-choose the profile and/or section\n");
      rp(11)=chord;
    }
    else if( dialog.getTextValue(answer,"section exponent:","%e",sectionExponent) )
    {
      printF("INFO: The section exponent is used when transitioning between different cross sections\n");
      printF("Setting the section exponent =%9.3e \n",sectionExponent);
      printF("You should re-choose the profile and/or section\n");
      rp(12)=sectionExponent;
    }
    else if( dialog.getTextValue(answer,"ship length:","%e",shipLength) )
    {
      printF("Setting ship length =%9.3e \n",shipLength);
      printF("You should re-choose the profile and/or section\n");
      rp(13)=shipLength;
    }
    else if( dialog.getTextValue(answer,"ship depth:","%e",shipDepth) )
    {
      printF("Setting ship depth =%9.3e \n",shipDepth);
      printF("You should re-choose the profile and/or section\n");
      rp(14)=shipDepth;
    }
    else if( dialog.getTextValue(answer,"ship breadth:","%e",shipBreadth) )
    {
      printF("Setting ship breadth =%9.3e \n",shipBreadth);
      printF("You should re-choose the profile and/or section\n");
      rp(15)=shipBreadth;
    }
    else if( dialog.getTextValue(answer,"hull sharpness:","%e",hullSharpness) )
    {
      printF("Setting hull sharpness parameter to %9.3e. \n",hullSharpness);
      printF("For the Wigley Hull, choosing a value closer to zero will make the bow and stern sharper.\n"
             "  But note that choosing too small a value may cause trouble in the grid generation\n");
      printF("You should re-choose the profile and/or section\n");
      rp(16)=hullSharpness;
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
      plotObject=true;
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
