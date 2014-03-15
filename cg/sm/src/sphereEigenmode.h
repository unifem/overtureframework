// ==================================================================================
// Macro to evaluate eigen modes of the sphere
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================
#beginMacro getSphereEigenmode(evalSolution,U,ERR,X,t,I1,I2,I3)
{
  const int v1c = parameters.dbase.get<int >("v1c");
  const int v2c = parameters.dbase.get<int >("v2c");
  const int v3c = parameters.dbase.get<int >("v3c");

  bool assignVelocities= v1c>=0 ;
  const int s11c = parameters.dbase.get<int >("s11c");
  const int s12c = parameters.dbase.get<int >("s12c");
  const int s13c = parameters.dbase.get<int >("s13c");
  const int s21c = parameters.dbase.get<int >("s21c");
  const int s22c = parameters.dbase.get<int >("s22c");
  const int s23c = parameters.dbase.get<int >("s23c");
  const int s31c = parameters.dbase.get<int >("s31c");
  const int s32c = parameters.dbase.get<int >("s32c");
  const int s33c = parameters.dbase.get<int >("s33c");

  const int pc = parameters.dbase.get<int >("pc");
    
  const real & rho=parameters.dbase.get<real>("rho");
  const real & mu = parameters.dbase.get<real>("mu");
  const real & lambda = parameters.dbase.get<real>("lambda");
  const RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
  const RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");


  bool assignStress = s11c >=0 ;

  if( pdeVariation == SmParameters::hemp )
  {
    printF("\n\n **************** FIX ME: getSphereEigenmode: finish me for HEMP **********\n\n");
    // OV_ABORT("error");
  }

  assert( mg.numberOfDimensions()==3 );

  // parameters are stored here:
  std::vector<real> & data = parameters.dbase.get<std::vector<real> >("sphereEigenmodeData");

  const int vibrationClass = (int)data[0];
  const int n = (int)data[1];
  const int m = (int)data[2];
  const real rad   = data[3];

  const real cp =sqrt( (lambda+2.*mu)/rho );
  const real cs = sqrt( mu/rho );

  // The eigenvalues z = kappa*a satisfy (n=mode)
  //   (n-1)*psi_n(z) + z*psi_n'(z) = 0 

  // cgDoc/sm/sphere.maple: 
  //  n=1 : m=1  x/Pi = 1.83456604098843e+00
  //  n=1 : m=2  x/Pi = 2.89503202144422e+00

  // n=2 : m=1  x/Pi = 7.96135239733083e-01
  // n=2 : m=2  x/Pi = 2.27146214644857e+00

  // kappa = omega /cs
  // h = omega/cp

  real omega;
  real cPhi=1., cOmega=1.;
  if( vibrationClass==1 )
  {
    // eigenvalues are kappa*a
    real radialEigenvalue;
    if( n==1 )
    {
      if( m==1 )
	radialEigenvalue = 1.83456604098843*Pi;
      else if( m==2 )
	radialEigenvalue = 2.89503202144422*Pi;
      else
      {
	OV_ABORT("getSphereEigenmode: invalid value for m");
      }
    }
    else if( n==2 )
    {
      assert( m==1 );
      
      if( m==1 )
	radialEigenvalue = 7.96135239733083e-01*Pi;
      else if( m==2 )
	radialEigenvalue = 2.27146214644857*Pi;
      else
      {
	OV_ABORT("getSphereEigenmode: invalid value for m");
      }
    }
    omega = (radialEigenvalue/rad)*cs;
    
  }
  else if( vibrationClass==2 )
  {
    // See cgDoc/sm/sphere2.maple
    // % sphere1.maple: 
    // %  Class II : n=0 : root m=1  x=4.43999821235653e+00, x/Pi = 1.41329532563144e+00, h*a/Pi=8.15966436697752e-01
    // %  Class II : n=0 : root m=2  x=1.04939244112140e+01, x/Pi = 3.34031988495483e+00, h*a/Pi=1.92853458475813e+00
    // %  Class II : n=0 : root m=3  x=1.60731909374045e+01, x/Pi = 5.11625557789555e+00, h*a/Pi=2.95387153514092e+00
    // %  Class II : n=0 : root m=4  x=2.15793450854558e+01, x/Pi = 6.86891887807218e+00, h*a/Pi=3.96577216329668e+00
    real radialEigenvalue;
    if( n==0 )
    { // radial vibrations
      // h*a
      const real eig[4] ={ // 
	8.15966436697752e-01,
	1.92853458475813e+00,
	2.95387153514092e+00,
	3.96577216329668e+00
      };
      
      if( m>=1 && m<=4 )
      {
        radialEigenvalue  = eig[m-1]*Pi;  // Love: m=0 : .8160
      }
      else 
      {
	printF("sphereEigenmode: ERROR: n=%i m=%i not available\n",n,m);
	OV_ABORT("ERROR");
      }
      
      omega = (radialEigenvalue/rad)*cp; 
    }
    else if( n==2 )
    {
      // %  n=2 : root m=1  x=2.63986927790186e+00, x/Pi = 8.40296489389027e-01, kappa*a/Pi=8.40296489389027e-01
      // %  an =-1.13478082789981e-02, cn=-3.02305786589451e-02, -an/cn=-3.75375159272393e-01
      // %  bn =-2.04041203329361e-02, dn=-5.43566078599509e-02, -bn/dn=-3.75375159272393e-01
      // % 
      // %  n=2 : root m=2  x=4.86527284993742e+00, x/Pi = 1.54866444711667e+00, kappa*a/Pi=1.54866444711667e+00
      // %  an =1.50294520720980e-02, cn=1.88125428532884e-01, -an/cn=-7.98905931500425e-02
      // %  bn =-1.22064204309416e-02, dn=-1.52789207710809e-01, -bn/dn=-7.98905931500425e-02
      // % 
      // %  n=2 : root m=3  x=8.32919545905501e+00, x/Pi = 2.65126525857435e+00, kappa*a/Pi=2.65126525857435e+00
      // %  an =4.57627801659678e-03, cn=-9.86226192484144e-02, -an/cn=4.64019111586348e-02
      // %  bn =-9.34189056911829e-04, dn=2.01325556121623e-02, -bn/dn=4.64019111586348e-02
      // % 
      // %  n=2 : root m=4  x=9.78016346034290e+00, x/Pi = 3.11312271792062e+00, kappa*a/Pi=3.11312271792062e+00
      // %  an =7.28789419129781e-04, cn=4.50066777991950e-02, -an/cn=-1.61929174684121e-02
      // %  bn =1.21547211256669e-03, dn=7.50619593373295e-02, -bn/dn=-1.61929174684121e-02

      // kappa*a
      const real eig[4] ={ // 
	8.40296489389027e-01,
	1.54866444711667e+00,
	2.65126525857435e+00,
        3.11312271792062e+00
      };
      const real cwp[4] ={ // 
	-3.75375159272393e-01,
	-7.98905931500425e-02,
	4.64019111586348e-02,
	-1.61929174684121e-02
      };
      // radialEigenvalue= 2.63986927790039;
      // radialEigenvalue = .840*Pi;  // Love p286
      if( m>=1 && m<=4 )
      {
	radialEigenvalue=eig[m-1]*Pi;
	cPhi=cwp[m-1];
      }
      else 
      {
	printF("sphereEigenmode: ERROR: n=%i m=%i not available\n",n,m);
	OV_ABORT("ERROR");
      }
      omega = (radialEigenvalue/rad)*cs;

    }
    else
    {

      OV_ABORT("finish me for vibrationClass==2");
    }

  }
  else
  {
    OV_ABORT("ERROR: vibrationClass");
  }
  
  
  real kappa,h;
  // kappa = omega/cs
  kappa = omega/cs;
  // h = omega/cp
  h = omega/cp;
  
  const real h2= h*h, h3=h*h*h, h4=h*h*h*h, kappa2=kappa*kappa, kappa3=kappa*kappa*kappa, kappa4=kappa*kappa*kappa*kappa;
  
  

  if( t==0. )
    printF("**** getSphereEigenmode: t=%8.2e class=%i n=%i m=%i radius=%9.3e omega=%9.3e cp=%8.2e cs=%8.2e"
           " h*a/pi=%9.3e kappa*a/pi=%9.3e\n",
            t,vibrationClass,n,m,rad,omega,cp,cs,h*rad/Pi,kappa*rad/Pi);

  // printF("**** getSphereEigenmode: t=%8.2e class=%i evalSolution=%i \n",t,vibrationClass,evalSolution);

  real cost = cos(omega*t);
  real sint = sin(omega*t);

  // vibrationClass==1 solution: 
  //     

  if( vibrationClass==1 )
  {
    real amp=10./rad; // amplitude
    if( n==2 )
      amp=100./rad;

    const real a0xy =1.;  // shear in the x-y plane
    const real a0yz =.8;  // shear in the y-z plane
    const real a0zx =.6;  // shear in the z-x plane
#define U1(x,y,z) (amp*cost*psi*( a0xy*(y)         -a0zx*(z) ))
#define V1(x,y,z) (amp*cost*psi*(-a0xy*(x)+a0yz*(z) ))
#define W1(x,y,z) (amp*cost*psi*(         -a0yz*(y)+a0zx*(x) ))
  
    // psi has an asymptotic series for small argument: 
    real psi0, psi1= 1./(2.*(2.*n+3.));
    if( n==1 )
    {
      psi0 = -1./(1.*3.);
    }
    else if( n==2 )
    {
      psi0 = 1./(1.*3.*5.);
    }
    else
    {
      OV_ABORT("finish me for n>2");
    }
  

    const real rEps = pow(REAL_EPSILON,.25); // use small r approximation when kappa*r < rEps
    // const real rEps = pow(REAL_EPSILON,.5); // use small r approximation when kappa*r < rEps

    int i1,i2,i3;
    real x0,y0,z0,r,kr,psi,u1,u2,u3;

    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      x0 = X(i1,i2,i3,0);
      y0 = X(i1,i2,i3,1);
      z0 = X(i1,i2,i3,2);

      r = sqrt( x0*x0 + y0*y0 + z0*z0 );
      kr = kappa*r;

      if( n==1 )
      {
	// Here is psi_1 
	if( kr < rEps ) // use taylor series: (Love p279)
	  psi = psi0*( 1. - psi1*kr*kr );  // + O( kr^4 )
	else
	  psi = (kr*cos(kr)-sin(kr))/(kr*kr*kr);

	u1 = U1(x0,y0,z0);
	u2 = V1(x0,y0,z0);
	u3 = W1(x0,y0,z0);
      }
      else if( n==2 )
      {
#define U2(x,y,z) (amp*cost*psi*( a0xy*(y)*(z)              -a0zx*(z)*(y) ))
#define V2(x,y,z) (amp*cost*psi*(-a0xy*(x)*(z)+a0yz*(z)*(x)               ))
#define W2(x,y,z) (amp*cost*psi*(             -a0yz*(y)*(x) +a0zx*(x)*(y) ))
  
	// Here is psi_2 
	if( kr < rEps ) // use taylor series: (Love p279)
	  psi = psi0*( 1. - psi1*kr*kr );  // + O( kr^4 )
	else
	  psi = ( 3.*kr*cos(kr)+ (kr*kr-3.)*sin(kr) )/(kr*kr*kr*kr*kr);

	u1 = U2(x0,y0,z0);
	u2 = V2(x0,y0,z0);
	u3 = W2(x0,y0,z0);
      }
      else
      {
	OV_ABORT("un-implemented value for n");
      }
      
      
      if( evalSolution )
      {
	U(i1,i2,i3,uc) =u1;
	U(i1,i2,i3,vc) =u2;
	U(i1,i2,i3,wc) =u3;
      }
      else
      {
	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
	ERR(i1,i2,i3,wc) =U(i1,i2,i3,wc) - u3;
      }

    }

    if( assignVelocities )
    {

      OV_ABORT("getSphereEigenmode: finish me");

      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
      {
	if( evalSolution )
	{
	}
	else
	{
	}

      }
    }
    if( assignStress )
    {

      OV_ABORT("getSphereEigenmode: finish me");

      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
      {
	if( evalSolution )
	{
	}
	else
	{
	}
      }
    }
  }
  else if( vibrationClass==2 )
  {
    const real amp=50./rad; // amplitude -- this makes |u| about 1

    const real rEps = pow(REAL_EPSILON,.25); // use small r approximation when kappa*r < rEps
    // const real rEps = pow(REAL_EPSILON,.5); // use small r approximation when kappa*r < rEps

    // psi has an asymptotic series for small argument: 
    // psi_n(x) = psin0*( 1. - psin1*x^2 + ... )
    const real psi10 = -1./(1.*3.);
    const real psi20 =  1./(1.*3.*5.);
    const real psi30 = -1./(1.*3.*5.*7.);
    const real psi40 =  1./(1.*3.*5.*7.*9.);

    const real psi11 = 1./(2.*(2.*1.+3.));
    const real psi21 = 1./(2.*(2.*2.+3.));
    const real psi31 = 1./(2.*(2.*3.+3.));
    const real psi41 = 1./(2.*(2.*4.+3.));

    int m1=0;  // m value for first solid harmonic "wn"
    int m2=0;  // m value for second solid harmonic "phi"

    const real n2p1 = 2.*n+1.;
    
    int i1,i2,i3;
    real x0,y0,z0,r;
    real kr,kr2,kr3,kr4,kr5,kr7,kr9;
    real hr,hr2,hr3,hr4,hr5,hr7,hr9;
    real u1,u2,u3;
    real v1,v2,v3;
    real u1x,u1y,u1z, u2x,u2y,u2z, u3x,u3y,u3z,div;
    real s11,s12,s13,s21,s22,s23,s31,s32,s33;
    real psi1hr, psi2hr, psi3hr, psi4hr;
    real psi1kr, psi2kr, psi3kr, psi4kr;

    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      x0 = X(i1,i2,i3,0);
      y0 = X(i1,i2,i3,1);
      z0 = X(i1,i2,i3,2);

      r = sqrt( x0*x0 + y0*y0 + z0*z0 );

      kr =kappa*r;
      kr2=kr*kr;
      kr3=kr2*kr;
      kr5=kr3*kr2;
      kr7=kr5*kr2;
      real coskr = cos(kr);
      real sinkr = sin(kr);

      hr = h*r;
      hr2=hr*hr;
      hr3=hr2*hr;
      hr5=hr3*hr2;
      hr7=hr5*hr2;
      real coshr = cos(hr);
      real sinhr = sin(hr);

      if( n==0 )
      {
        // radial vibrations

	if( hr < rEps ) // use taylor series: (Love p279)
	{
	  psi1hr = psi10*( 1. - psi11*hr2 );  // + O( hr^4 )
	}
	else
	{
	  psi1hr =   ( hr *coshr - sinhr )/hr3;
	}

        u1 = amp*cost*( x0*psi1hr );
        u2 = amp*cost*( y0*psi1hr );
        u3 = amp*cost*( z0*psi1hr );
	if( assignVelocities )
	{
	  v1 = -amp*omega*sint*( x0*psi1hr );
	  v2 = -amp*omega*sint*( y0*psi1hr );
	  v3 = -amp*omega*sint*( z0*psi1hr );
	}
	if( assignStress )
	{
	  if( hr < rEps ) // use taylor series: (Love p279)
	    psi2hr = psi20*( 1. - psi21*hr2 );  // + O( hr^4 )
	  else
	    psi2hr = - ( hr2*sinhr + 3.*hr*coshr - 3.*sinhr)/hr5;

          // u1 = x*psi1(hr) 
          // u1x = psi1(hr) + x*h*x/r*psi1'(hr) = psi1r + (h*x)^2 psi2(hr) 
          u1x = amp*cost*( psi1hr + h*h*x0*x0*psi2hr );
	  u1y = amp*cost*(          h*h*x0*y0*psi2hr );
	  u1z = amp*cost*(          h*h*x0*z0*psi2hr );

          u2x = amp*cost*(          h*h*y0*x0*psi2hr );
	  u2y = amp*cost*( psi1hr + h*h*y0*y0*psi2hr );
	  u2z = amp*cost*(          h*h*y0*z0*psi2hr );

          u3x = amp*cost*(          h*h*z0*x0*psi2hr );
	  u3y = amp*cost*(          h*h*z0*y0*psi2hr );
	  u3z = amp*cost*( psi1hr + h*h*z0*z0*psi2hr );
	}
	
      }
      else if( n==2 )
      {
        // cgDoc/sm/sphericalHarmonics.maple
        // > lprint(psi1);
        //1/x^3*(cos(x)*x-sin(x))
        //> lprint(psi2);
        //-(3*cos(x)*x-3*sin(x)+sin(x)*x^2)/x^5
        //> lprint(psi3);
        //-(x^3*cos(x)-6*sin(x)*x^2-15*cos(x)*x+15*sin(x))/x^7

	// We need 
	//   psi2(hr), psi3(hr)
	//   psi1(kr), psi3(kr)

	if( hr < rEps ) // use taylor series: (Love p279)
	{
	  psi2hr = psi20*( 1. - psi21*hr2 );  // + O( hr^4 )
	  psi3hr = psi30*( 1. - psi31*hr2 );  // + O( hr^4 )
	}
	else
	{
	  psi2hr = - ( hr2*sinhr + 3.*hr*coshr - 3.*sinhr)/hr5;
	  psi3hr = - ( hr3*coshr - 6.*hr2*sinhr - 15.*hr*coshr +15.*sinhr)/hr7;
	}
	
	if( kr < rEps ) // use taylor series: (Love p279)
	{
	  psi1kr = psi10*( 1. - psi11*kr2 );  // + O( kr^4 )
	  psi3kr = psi30*( 1. - psi31*kr2 );  // + O( kr^4 )
	}
	else
	{
	  psi1kr =   ( kr *coskr - sinkr )/kr3;
	  psi3kr = - ( kr3*coskr - 6.*kr2*sinkr - 15.*kr*coskr +15.*sinkr )/kr7;
	}
	
        // wn = r^2*Y_2^0 = ( 2*z^2 - x^2 - y^2 )
        // wn = r^2*Y_2^1 = ( z*(a*x+b*y) )
        // wn = r^2*Y_2^2 = ( a*(x^2-y^2) + b*x*y )
        real w,wx,wy,wz;
        real wxx,wxy,wxz, wyy,wyz,wzz;
	  
        if( m1==0 )
	{
          w = 2.*z0*z0 - x0*x0 - y0*y0;
	  wx = -2.*x0;
	  wy = -2.*y0;
	  wz =  4.*z0;
          wxx = -2.; wxy=0.; wxz=0.;
          wyy = -2.; wyz=0.;
          wzz =  4.; 
	}
        else
	{
          OV_ABORT("ERROR: unexpected m1");
	}
	
        // Question: do we need to have the correct leading coeff for the solid harmonics?

	// NOTE:
        //     an*wn+cn*pn = 0 
        //     bn*wn+dn*pn = 0 
        // 
        //  p/w = -an/cn =-bn/dn 
        // sphere2.maple:
	//  an =-1.13478082790558e-02, cn=-3.02305786593820e-02, -an/cn=-3.75375159268876e-01
        //  bn =-2.04041203329458e-02, dn=-5.43566078598750e-02, -bn/dn=-3.75375159273096e-01
        
//% % ---Digits 25: 
//%  n=2 : root m=1  x=2.63986927790186e+00, x/Pi = 8.40296489389027e-01, kappa*a/Pi=8.40296489389027e-01
//%  an =-1.13478082789981e-02, cn=-3.02305786589451e-02, -an/cn=-3.75375159272393e-01
//%  bn =-2.04041203329361e-02, dn=-5.43566078599509e-02, -bn/dn=-3.75375159272393e-01
//% 
//%  n=2 : root m=2  x=4.86527284993742e+00, x/Pi = 1.54866444711667e+00, kappa*a/Pi=1.54866444711667e+00
//%  an =1.50294520720980e-02, cn=1.88125428532884e-01, -an/cn=-7.98905931500425e-02
//%  bn =-1.22064204309416e-02, dn=-1.52789207710809e-01, -bn/dn=-7.98905931500425e-02
//% 
//%  n=2 : root m=3  x=8.32919545905501e+00, x/Pi = 2.65126525857435e+00, kappa*a/Pi=2.65126525857435e+00
//%  an =4.57627801659678e-03, cn=-9.86226192484144e-02, -an/cn=4.64019111586348e-02
//%  bn =-9.34189056911829e-04, dn=2.01325556121623e-02, -bn/dn=4.64019111586348e-02
//% 
//%  n=2 : root m=4  x=9.78016346034290e+00, x/Pi = 3.11312271792062e+00, kappa*a/Pi=3.11312271792062e+00
//%  an =7.28789419129781e-04, cn=4.50066777991950e-02, -an/cn=-1.61929174684121e-02
//%  bn =1.21547211256669e-03, dn=7.50619593373295e-02, -bn/dn=-1.61929174684121e-02

//         const real cw=1.;
//         const real cPhi=-3.75375159268876e-01;  // ********* 
	 

        real p,px,py,pz;
        real pxx,pxy,pxz, pyy,pyz,pzz; 
	if( m2==0 )
	{
          p  = cPhi*(2.*z0*z0 - x0*x0 - y0*y0);
	  px = cPhi*( -2.*x0 );
	  py = cPhi*( -2.*y0 );
	  pz = cPhi*(  4.*z0 );

          pxx = cPhi*(-2. ); pxy=0.; pxz=0.;
          pyy = cPhi*(-2. ); pyz=0.;
          pzz = cPhi*( 4. ); 
	}
	else if( m2==1 )
	{
          // here is a linear combination of the real and im parts
          // p  = cPhi*( z0*( x0+ y0) );
	  // px = cPhi*( z0 );
	  // py = cPhi*( z0 );
	  // pz=  cPhi*( x0+y0 );
	}
        else
	{
          OV_ABORT("ERROR: unexpected m2");
	}
	
#define VB2A(xa,wxa,pxa) \
            ( (-1./(h*h))*( psi2hr + (hr2/n2p1)*psi3hr )*(wxa)   \
  	    + (1./n2p1)*psi3hr*( r*r*(wxa) - n2p1*(xa)*w )  \
            + psi1kr*(pxa) - (n/(n+1.))*psi3kr*kappa*kappa*( r*r*(pxa) - n2p1*(xa)*p ) )

// simpler: 
#define VB2(xa,wxa,pxa) \
            ( (-1./(h*h))*( psi2hr*(wxa) + h*h*(xa)*psi3hr*w )   \
            + psi1kr*(pxa) - (n/(n+1.))*psi3kr*kappa*kappa*( r*r*(pxa) - n2p1*(xa)*p ) )

        real vb2x = VB2(x0,wx,px);
	real vb2y = VB2(y0,wy,py);
	real vb2z = VB2(z0,wz,pz);

        u1 = amp*cost*vb2x;
        u2 = amp*cost*vb2y;
        u3 = amp*cost*vb2z;

	if( assignVelocities )
	{
	  v1 = -amp*omega*sint*vb2x;
	  v2 = -amp*omega*sint*vb2y;
	  v3 = -amp*omega*sint*vb2z;
	}
	if( assignStress )
	{
          // we need psi4(hr) and psi4(kr)
          // > lprint(psi4);
          //  1/x^9*(10*x^3*cos(x) -45*sin(x)*x^2+ x^4*sin(x) -105*cos(x)*x+105*sin(x))
          kr4=kr2*kr2;
	  kr9=kr7*kr2;

          hr4=hr2*hr2;
	  hr9=hr7*hr2;

	  if( hr < rEps ) // use taylor series: (Love p279)
	    psi4hr = psi40*( 1. - psi41*hr2 );  // + O( hr^4 )
	  else
	    psi4hr = ( (hr4-45.*hr2+105.)*sinhr + (10.*hr3-105.*hr)*coshr )/hr9;
	
	  if( kr < rEps ) // use taylor series: (Love p279)
	  {
	    psi2kr = psi20*( 1. - psi21*kr2 );  // + O( kr^4 )
	    psi4kr = psi40*( 1. - psi41*kr2 );  // + O( kr^4 )
	  }
	  else
	  {
	    psi2kr = - ( kr2*sinkr + 3.*kr*coskr - 3.*sinkr )/kr5;
	    psi4kr = ( (kr4-45.*kr2+105.)*sinkr + (10.*kr3-105.*kr)*coskr )/kr9;
	  }
	  
          // D( u_j )/D( x_a ) **check me **
#define VB2X(xj,wj,pj,  xa,wa,pa, wja,pja, deltaja )\
          ( (-1./(h2))*( h2*xa*psi3hr*wj + psi2hr*wja + h2*deltaja*psi3hr*w + h4*xj*xa*psi4hr*w + h2*xj*psi3hr*wa )\
	    + kappa2*xa*psi2kr*pj + psi1kr*pja \
	    - (n/(n+1.))*kappa4*xa*psi4kr*( r*r*pj - n2p1*xj*p )\
            - (n/(n+1.))*kappa2*psi3kr*( 2.*xa*pj - n2p1*(deltaja)*p + r*r*pja - n2p1*xj*pa ) )

	  u1x = amp*cost*( VB2X(x0,wx,px, x0,wx,px, wxx,pxx, 1. ) );
	  u1y = amp*cost*( VB2X(x0,wx,px, y0,wy,py, wxy,pxy, 0. ) );
	  u1z = amp*cost*( VB2X(x0,wx,px, z0,wz,pz, wxz,pxz, 0. ) );
	  
	  u2x = amp*cost*( VB2X(y0,wy,py, x0,wx,px, wxy,pxy, 0. ) );
	  u2y = amp*cost*( VB2X(y0,wy,py, y0,wy,py, wyy,pyy, 1. ) );
	  u2z = amp*cost*( VB2X(y0,wy,py, z0,wz,pz, wyz,pyz, 0. ) );
	  
	  u3x = amp*cost*( VB2X(z0,wz,pz, x0,wx,px, wxz,pxz, 0. ) );
	  u3y = amp*cost*( VB2X(z0,wz,pz, y0,wy,py, wyz,pyz, 0. ) );
	  u3z = amp*cost*( VB2X(z0,wz,pz, z0,wz,pz, wzz,pzz, 1. ) );
	  
	}

#undef VB2

      }
      else
      {
        OV_ABORT("ERROR: unexpected n");
      }


      if( evalSolution )
      {
	U(i1,i2,i3,uc) =u1;
	U(i1,i2,i3,vc) =u2;
	U(i1,i2,i3,wc) =u3;
      }
      else
      {
	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
	ERR(i1,i2,i3,wc) =U(i1,i2,i3,wc) - u3;
      }

      if( assignVelocities )
      {
        if( evalSolution )
	{
	  U(i1,i2,i3,v1c) =v1;
	  U(i1,i2,i3,v2c) =v2;
	  U(i1,i2,i3,v3c) =v3;
	}
	else
	{
	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
	  ERR(i1,i2,i3,v3c) =U(i1,i2,i3,v3c) - v3;
	}
	

      }
      if( assignStress )
      {
	div = u1x+u2y+u3z;
	s11 = lambda*div + 2.*mu*u1x;
	s12 = mu*( u1y+u2x );
	s13 = mu*( u1z+u3x );
	s21 = s12;
	s22 = lambda*div + 2.*mu*u2y;
	s23 = mu*( u2z + u3y );
	s31 = s13;
	s32 = s23;
	s33 = lambda*div + 2.*mu*u3z;

        if( evalSolution )
	{
	  
	  U(i1,i2,i3,s11c) =s11;
	  U(i1,i2,i3,s12c) =s12;
	  U(i1,i2,i3,s13c) =s13;
	  U(i1,i2,i3,s21c) =s21;
	  U(i1,i2,i3,s22c) =s22;
	  U(i1,i2,i3,s23c) =s23;
	  U(i1,i2,i3,s31c) =s31;
	  U(i1,i2,i3,s32c) =s32;
	  U(i1,i2,i3,s33c) =s33;
	}
	else
	{
	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) - s11;
	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) - s12;
	  ERR(i1,i2,i3,s13c) =U(i1,i2,i3,s13c) - s13;
	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) - s21;
	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) - s22;
	  ERR(i1,i2,i3,s23c) =U(i1,i2,i3,s23c) - s23;
	  ERR(i1,i2,i3,s31c) =U(i1,i2,i3,s31c) - s31;
	  ERR(i1,i2,i3,s32c) =U(i1,i2,i3,s32c) - s32;
	  ERR(i1,i2,i3,s33c) =U(i1,i2,i3,s33c) - s33;
	}
      }
      
      
    }  // end for3d
    

  }
  else
  {
    OV_ABORT("ERROR: unexpected vibrationClass");
  }
  

#undef U2
#undef V2
#undef W2

  
}

#endMacro
