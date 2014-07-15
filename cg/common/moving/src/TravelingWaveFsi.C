#include "TravelingWaveFsi.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "GenericGraphicsInterface.h"
#include "display.h"
#include "NurbsMapping.h"
#include "ParallelUtility.h"

int 
getTravelingWave( double & wr, double & wi , double *rpar, int *ipar, 
                  const int & nf, double *pr, double *pi, double *yf, double *v1r, double *v1i, double *v2r, double *v2i,  
                  const int & ns, double *ys, double *us1r, double *us1i,  double *us2r, double *us2i,
                  double *us1yr, double *us1yi,  double *us2yr, double *us2yi  );

int 
evalTravelingWave( const double & wr, const double & wi , double *rpar, int *ipar, 
                  const int & nf, double *pr, double *pi, double *yf, double *v1r, double *v1i, double *v2r, double *v2i,  
                  const int & ns, double *ys, double *us1r, double *us1i,  double *us2r, double *us2i,
                  double *us1yr, double *us1yi,  double *us2yr, double *us2yi  );

// ============================================================================
/// \brief Construct an TravelingWaveFsi Object. 
// ============================================================================
TravelingWaveFsi::
TravelingWaveFsi()
{

  if( !dbase.has_key("exactSolution") ) dbase.put<aString>("exactSolution","InsShell");


  if( !dbase.has_key("ec") ) dbase.put<int>("ec",0);  // eta
  if( !dbase.has_key("etc") ) dbase.put<int>("etc",1);  // etat

  // fluid variables:
  if( !dbase.has_key("pfc") ) dbase.put<int>("pfc",0);    // p 
  if( !dbase.has_key("v1fc") ) dbase.put<int>("v1fc",1);  // v1 
  if( !dbase.has_key("v2fc") ) dbase.put<int>("v2fc",2);  // v2

  if (!dbase.has_key("u1c")) dbase.put<int>("u1c",-1);
  if (!dbase.has_key("u2c")) dbase.put<int>("u2c",-1);
  if (!dbase.has_key("u3c")) dbase.put<int>("u3c",-1);

  // some methods need the velocities: 
  if (!dbase.has_key("v1c")) dbase.put<int>("v1c",-1);
  if (!dbase.has_key("v2c")) dbase.put<int>("v2c",-1);
  if (!dbase.has_key("v3c")) dbase.put<int>("v3c",-1);

  // some methods need the stresses 
  if (!dbase.has_key("s11c")) dbase.put<int>("s11c",-1);
  if (!dbase.has_key("s12c")) dbase.put<int>("s12c",-1);
  if (!dbase.has_key("s13c")) dbase.put<int>("s13c",-1);
  if (!dbase.has_key("s21c")) dbase.put<int>("s21c",-1);
  if (!dbase.has_key("s22c")) dbase.put<int>("s22c",-1);
  if (!dbase.has_key("s23c")) dbase.put<int>("s23c",-1);
  if (!dbase.has_key("s31c")) dbase.put<int>("s31c",-1);
  if (!dbase.has_key("s32c")) dbase.put<int>("s32c",-1);
  if (!dbase.has_key("s33c")) dbase.put<int>("s33c",-1);

  if( !dbase.has_key("debug") ) dbase.put<int>("debug",0);
  if( !dbase.has_key("dt") ) dbase.put<real>("dt",-1.); 
  if( !dbase.has_key("dy") ) dbase.put<real>("dy",-1.); 

  if( !dbase.has_key("amp") ) dbase.put<real>("amp",.1);         // initial wave amplitude
  if( !dbase.has_key("kx") ) dbase.put<real>("kx",1.);           // wave number 
  if( !dbase.has_key("x0") ) dbase.put<real>("x0",0.);           // phase in x 
  if( !dbase.has_key("t0") ) dbase.put<real>("t0",0.);           // phase in t 
  if( !dbase.has_key("omega") ) dbase.put<real>("omega",1.);    // reduced frequency (derived quantity)

  if( !dbase.has_key("current") ) dbase.put<int>("current",0);

  if( !dbase.has_key("pde") ) dbase.put<aString>("pde","InsShell"); // pde 
  if( !dbase.has_key("timeSteppingMethod") ) dbase.put<aString>("timeSteppingMethod","Runge-Kutta"); 

  if( !dbase.has_key("height") ) dbase.put<real>("height",1.);
  if( !dbase.has_key("length") ) dbase.put<real>("length",1.);

  if( !dbase.has_key("Hs") ) dbase.put<real>("Hs",.5);    // bulk solid height
  if( !dbase.has_key("Ls") ) dbase.put<real>("Ls",1.);    // bulk solid width

  if( !dbase.has_key("hs") ) dbase.put<real>("hs",1.);    // elastic shell width 
  if( !dbase.has_key("ke") ) dbase.put<real>("ke",0.);    // elastic shell stiffness parameter
  if( !dbase.has_key("te") ) dbase.put<real>("te",1.);    // elastic shell tension parameter  
  if( !dbase.has_key("be") ) dbase.put<real>("be",0.);    // elastic shell damping parameter  
  if( !dbase.has_key("ad") ) dbase.put<real>("ad",.1);    // elastic shell artificial dissipation.
  if( !dbase.has_key("normalMotionOnly") ) dbase.put<bool>("normalMotionOnly",false);
  if( !dbase.has_key("standingWaveSolution") ) dbase.put<bool>("standingWaveSolution",false);

  if( !dbase.has_key("cfl") ) dbase.put<real>("cfl",.9);        // fluid density 

  if( !dbase.has_key("rho") ) dbase.put<real>("rho",1.);        // fluid density 
  if( !dbase.has_key("mu") ) dbase.put<real>("mu",.1);        // fluid density 

  if( !dbase.has_key("rhoe") ) dbase.put<real>("rhoe",1.);        // solid density 
  if( !dbase.has_key("lambdae") ) dbase.put<real>("lambdae",1.);  // solid Lame parameter
  if( !dbase.has_key("mue") ) dbase.put<real>("mue",1.);          // solid Lame parameter

  if( !dbase.has_key("mu") ) dbase.put<real>("mu",.1);            // Fluid viscosity

  if( !dbase.has_key("orderOfAccuracyInSpace") ) dbase.put<int>("orderOfAccuracyInSpace",4);
  if( !dbase.has_key("orderOfAccuracyInTime") ) dbase.put<int>("orderOfAccuracyInTime",4);

  if( !dbase.has_key("numberOfSolidComponents") ) dbase.put<int>("numberOfSolidComponents",2);
  if( !dbase.has_key("numberOfFluidComponents") ) dbase.put<int>("numberOfFluidComponents",3);

  if( !dbase.has_key("numberOfSolidGridPoints") ) dbase.put<int>("numberOfSolidGridPoints",1);
  if( !dbase.has_key("numberOfFluidGridPoints") ) dbase.put<int>("numberOfFluidGridPoints",51);

  if( !dbase.has_key("numberOfSolidGhostPoints") ) dbase.put<int>("numberOfSolidGhostPoints",0);
  if( !dbase.has_key("numberOfFluidGhostPoints") ) dbase.put<int>("numberOfFluidGhostPoints",2);

  if( !dbase.has_key("numberOfTimeLevels") ) dbase.put<int>("numberOfTimeLevels",4);
  if( !dbase.has_key("numberOfWorkSpaceVectors") ) dbase.put<int>("numberOfWorkSpaceVectors",4);

  if( !dbase.has_key("degreeInTime") ) dbase.put<int>("degreeInTime",2);
  if( !dbase.has_key("degreeInSpace") ) dbase.put<int>("degreeInSpace",2);

  // Number of ghost points to include whem computing errors.
  if( !dbase.has_key("numberOfGhostLinesInError") ) dbase.put<int>("numberOfGhostLinesInError",0);

  if( !dbase.has_key("initialConditions") ) dbase.put<aString>("initialConditions","twilightZone");

  if( !dbase.has_key("omegav") ) dbase.put<real[2]>("omegav");   // real and imaginary parts of omega
  real *omegav =  dbase.get<real[2]>("omegav");
  omegav[0]=0.; omegav[1]=0.;
  

  // // Traveling-wave (exact solution) parameters:
  // if( !dbase.has_key("TravelingWaveParameters") ) dbase.put<real[10]>("TravelingWaveParameters"); 
  // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");
  // fwp[0]=.1; // amp
  // fwp[1]=1.; // kx 
  // fwp[2]=1.; // ky 
  // fwp[3]=0.; // x0
  // fwp[4]=0.; // t0 

  if( !dbase.has_key("exactSolution") ) dbase.put<aString>("exactSolution","twilightZone");
  if( !dbase.has_key("exactPointer") ) dbase.put<OGFunction*>("exactPointer",NULL);
  if( !dbase.has_key("twilightZone") ) dbase.put<bool>("twilightZone",false);
  if( !dbase.has_key("twilightZoneOption") ) dbase.put<int>("twilightZoneOption",0);

  // Frequencies for trig TZ: 
  if( !dbase.has_key("trigFreq") ) dbase.put<real[4]>("trigFreq");   // ft, fx, fy, [fz]
  real *trigFreq = dbase.get<real[4]>("trigFreq");
  for( int i=0; i<4; i++ ){ trigFreq[i]=2.;  }


  
  if( !dbase.has_key("yCoord") ) dbase.put<RealArray>("yCoord");
  if( !dbase.has_key("ysCoord") ) dbase.put<RealArray>("ysCoord");
}

//================================================================================================
/// \brief Compute the exact solution in the fluid at time t.
///
/// \param u (output) : exact solution 
/// \param t (input) : evaluate the solution at this time.
/// \param mg (input) : MappedGrid
/// \param I1,I2,I3 (input) : evaluate at these points.
/// \param numberOfTimeDerivatives (input) : evaluate this many time-derivatives of the solution.
///
//================================================================================================
int TravelingWaveFsi::
getExactFluidSolution( RealArray & u, const real t, MappedGrid & mg, const Index & I1, const Index & I2, const Index & I3, 
                       const int numberOfTimeDerivatives /* =0 */ )
{
  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );

  OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);

  return getExactFluidSolution( u, t,  xLocal, I1,I2,I3,numberOfTimeDerivatives );
}

//================================================================================================
/// \brief Compute the exact solution in the fluid at time t.
///
/// \param u (output) : exact solution 
/// \param t (input) : evaluate the solution at this time.
/// \param mg (input) : MappedGrid
/// \param I1,I2,I3 (input) : evaluate at these points.
/// \param numberOfTimeDerivatives (input) : evaluate this many time-derivatives of the solution.
///
//================================================================================================
int TravelingWaveFsi::
getExactFluidSolution( RealArray & u, const real t, const RealArray & xLocal, const Index & I1, const Index & I2, const Index & I3, const int numberOfTimeDerivatives /* =0 */ )
{
  const aString & pde = dbase.get<aString>("pde"); // pde we are solving
  const int & debug = dbase.get<int>("debug");
  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  int & current = dbase.get<int>("current");
  int prev = (current - 1 + numberOfTimeLevels) % numberOfTimeLevels;
  int next = (current + 1 + numberOfTimeLevels) % numberOfTimeLevels;

  RealArray & time = dbase.get<RealArray>("time");
  real tc = time(current);

  int & pc = dbase.get<int >("pfc");
  int & v1c = dbase.get<int >("v1fc");
  int & v2c = dbase.get<int >("v2fc");

  real & cfl = dbase.get<real>("cfl");
  real & rhoe = dbase.get<real>("rhoe");
  real & lambdae = dbase.get<real>("lambdae");
  real & mue = dbase.get<real>("mue");

  real & rho = dbase.get<real>("rho");
  real & mu  = dbase.get<real>("mu");

  real & ke = dbase.get<real>("ke");
  real & te = dbase.get<real>("te");
  real & be = dbase.get<real>("be");
  real & ad = dbase.get<real>("ad");


  const real cp = sqrt( (lambdae+2.0*mue)/rhoe );  
  const real cs = sqrt( mue/rhoe ); 


  real & kx = dbase.get<real>("kx");
  real & height = dbase.get<real>("height");
  real & length = dbase.get<real>("length");

  const real & Hs = dbase.get<real>("Hs"); // solid height
  const real & Ls = dbase.get<real>("Ls"); // solid width
  const real & hs = dbase.get<real>("hs"); // shell height

  real & amp = dbase.get<real>("amp");
  real & x0  = dbase.get<real>("x0");
  real & t0  = dbase.get<real>("t0");

  const bool & standingWaveSolution = dbase.get<bool>("standingWaveSolution");

  // // Traveling wave parameters
  // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");

  // OV_GET_SERIAL_ARRAY(real,u,uLocal);
  RealArray & uLocal =u;
    
  uLocal=0.;
  // printF("++++++TravelingWaveFsi: pde=[%s]\n",(const char*)pde);
  
  if( pde=="InsShell" && mu==0. )
  {
    // --- TravelingWave: inviscid fluid + Elastic SHELL ----

    // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");
    // const real amp = fwp[0];
    // const real kx  = fwp[1];
    // const real ky  = fwp[2];
    // const real x0  = fwp[3];
    // const real t0  = fwp[4];

    const real kxHat=twoPi*kx/length;

    const real am = rho/(kxHat*tanh(kxHat*height));  // added-mass
    const real omegaHat= sqrt( (ke + SQR(kxHat)*te)/(rhoe+am ) );

    // eta = x(Ib1,Ib2,Ib3,1) + amp*sin(kxHat*x(Ib1,Ib2,Ib3,0)+x0)*cos(omegaHat*t+t0);

    if( !standingWaveSolution )
    {
      // -- traveling wave --
      RealArray coskxwt(I1,I2,I3), sinkxwt(I1,I2,I3); 
      coskxwt=cos(kxHat*xLocal(I1,I2,I3,0)-omegaHat*t);
      sinkxwt=sin(kxHat*xLocal(I1,I2,I3,0)-omegaHat*t);

      RealArray coshkxy(I1,I2,I3), sinhkxy(I1,I2,I3); 
      coshkxy=cosh(kxHat*(xLocal(I1,I2,I3,1)));
      sinhkxy=sinh(kxHat*(xLocal(I1,I2,I3,1)));

      // Exact solution for the fluid pressure: (p.y=0 at y=0 )
      real factor = 1./(kxHat*sinh(kxHat*height));

      real pAmp = amp*rho*SQR(omegaHat)/(kxHat*sinh(kxHat*height));

      assert( numberOfTimeDerivatives==0 );
      
      // pressure
      uLocal(I1,I2,I3,pc) = pAmp*coshkxy*coskxwt;


      // Exact solution for the fluid velocity:
      real vAmp = amp*(omegaHat/sinh(kxHat*height));
      uLocal(I1,I2,I3,v1c) = vAmp*coshkxy*coskxwt;
      uLocal(I1,I2,I3,v2c) = vAmp*sinhkxy*sinkxwt;
    }
    else
    {
      // --- standing wave ---
      OV_ABORT("finish me");
    }
    
  }
  else if( true || mu>0. )
  {
    // --- TravelingWave: inviscid OR viscous fluid + Elastic SHELL OR Acoustic Solid  ----

    // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");
    // const real amp = fwp[0];
    // const real kx  = fwp[1];
    // const real ky  = fwp[2];
    // const real x0  = fwp[3];
    // const real t0  = fwp[4];

    const real kxHat=twoPi*kx/length;

    assert( dbase.has_key("twf") );
	
    const RealArray & twf = dbase.get<RealArray>("twf");
    const RealArray & tws = dbase.get<RealArray>("tws");

    real *omegav =  dbase.get<real[2]>("omegav");
    const real wr=omegav[0], wi=omegav[1];
    if( t==0. )
      printF("getExactFluidSolution: w=(%9.3e,%9.3e)\n",wr,wi);

    RealArray coskxwt, sinkxwt; 
    if( !standingWaveSolution )
    {
      coskxwt.redim(I1,I2,I3); sinkxwt.redim(I1,I2,I3); 
      coskxwt=cos(kxHat*xLocal(I1,I2,I3,0)-wr*t);
      sinkxwt=sin(kxHat*xLocal(I1,I2,I3,0)-wr*t);
    }
    
    const int nf=1, ns=1;
    RealArray yf(nf), ys(max(1,ns));
    // printF("***** twf.getLength(1)=%i\n",twf.getLength(1));

    RealArray twfa(2*nf,twf.getLength(1)), twsa(2*ns,tws.getLength(1));
    // RealArray twfa(2*nf,max(10,twf.getLength(1))), twsa(2*ns,tws.getLength(1));
    // RealArray twfa(Range(2*nf),100), twsa(Range(2*ns),100);

    int pdeOption=0;
    if( pde=="InsShell" )
    {
      pdeOption=0;
    }
    else if( pde=="InsAcousticSolid" )
    {
      pdeOption=1;
    }
    else if( pde=="InsElasticSolid" )
    {
      pdeOption=2;
    }
    else
    {
      OV_ABORT("error");
    }
    
    const bool & normalMotionOnly = dbase.get<bool>("normalMotionOnly");
    int option;
    if( normalMotionOnly )
      option=0; // eta1=0,  eta2 varies
    else
      option=2;  // eta1 and eta2 vary

    real solidHeight=hs;  // shell
    int ipar[]={option,pdeOption};  //
    real rpar[]={kxHat,hs,rho,mu, rhoe,hs,ke,te,0.,0.};  //

    if( pde!="InsShell" )
    {
      // --- bulk solid ---
      if( pde=="InsAcousticSolid" ||
	  pde=="InsElasticSolid" )
      {
	rpar[5]=Hs;
	rpar[6]=cp;
	rpar[7]=cs;
	solidHeight=Hs;
      }
      else 
      {
	OV_ABORT("error");
      }
    }
    

    // Real part of vHat*exp( i( kx-w*t) )
    int i3=I3.getBase();
    for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
    {
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	real x=xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1);

	yf(0)=-height + y;
	ys(0)=0.;  // not needed

	const int b0=twfa.getBase(0);
	evalTravelingWave( wr, wi, rpar, ipar, 
			   nf, yf.getDataPointer(), 
			   &twfa(b0,0), &twfa(b0,1), &twfa(b0,2), &twfa(b0,3), &twfa(b0,4), &twfa(b0,5), 
			   ns, ys.getDataPointer(), &twsa(b0,0), &twsa(b0,1), &twsa(b0,2), &twsa(b0,3),
			   &twsa(b0,4), &twsa(b0,5), &twsa(b0,6), &twsa(b0,7) );

	//  printF(" b0=%i, tfa=(%g,%g,%g,%g,%g,%g) \n",b0, twfa(b0,0), twfa(b0,1), twfa(b0,2), twfa(b0,3), twfa(b0,4), twfa(b0,5));
	

	int i2a=b0;
      
	if( !standingWaveSolution )
	{
          if( numberOfTimeDerivatives==0 )
	  {
	    uLocal(i1,i2,i3,pc)  = (amp*exp(wi*t))*( twfa(i2a,0)*coskxwt(i1,i2,i3) - twfa(i2a,1)*sinkxwt(i1,i2,i3) );
	    // printF("-- TWFSI -- t=%g x=%g, y=%g, yf=%g, p=%g\n",t, x,y,yf(0),uLocal(i1,i2,i3,pc));
	    uLocal(i1,i2,i3,v1c) = (amp*exp(wi*t))*( twfa(i2a,2)*coskxwt(i1,i2,i3) - twfa(i2a,3)*sinkxwt(i1,i2,i3) );
	    uLocal(i1,i2,i3,v2c) = (amp*exp(wi*t))*( twfa(i2a,4)*coskxwt(i1,i2,i3) - twfa(i2a,5)*sinkxwt(i1,i2,i3) );
	  }
	  else if( numberOfTimeDerivatives==1 )
	  {
            // -- first time derivative --
            // coskxwt=cos(kxHat*xLocal(I1,I2,I3,0)-wr*t);
            // sinkxwt=sin(kxHat*xLocal(I1,I2,I3,0)-wr*t);
	    uLocal(i1,i2,i3,pc)  = ( (amp*wi*exp(wi*t))*(  twfa(i2a,0)*coskxwt(i1,i2,i3) - twfa(i2a,1)*sinkxwt(i1,i2,i3) )+
				     (amp*wr*exp(wi*t))*( +twfa(i2a,0)*sinkxwt(i1,i2,i3) + twfa(i2a,1)*coskxwt(i1,i2,i3) ) );
	    
	    uLocal(i1,i2,i3,v1c) = ( (amp*wi*exp(wi*t))*(  twfa(i2a,2)*coskxwt(i1,i2,i3) - twfa(i2a,3)*sinkxwt(i1,i2,i3) )+
				     (amp*wr*exp(wi*t))*( +twfa(i2a,2)*sinkxwt(i1,i2,i3) + twfa(i2a,3)*coskxwt(i1,i2,i3) ) );

	    uLocal(i1,i2,i3,v2c) = ( (amp*wi*exp(wi*t))*(  twfa(i2a,4)*coskxwt(i1,i2,i3) - twfa(i2a,5)*sinkxwt(i1,i2,i3) )+
				     (amp*wr*exp(wi*t))*( +twfa(i2a,4)*sinkxwt(i1,i2,i3) + twfa(i2a,5)*coskxwt(i1,i2,i3) ) );
	  }
	  else
	  {
	    OV_ABORT("finish me");
	  }
	}
	else
	{
          // -- standing wave ---  
          //   From one traveling wave solution (p,v1,v2) another solution is obtained by x -> -x and v1 -> -v1
          // Subtract these two (and divide by 2) to get a standing wave 
          //   v1 <- [ v1(x,y,t) + v1(-x,y,t) ) ]/2
          //   v2 <- [ v2(x,y,t) - v2(-x,y,t) ) ]/2
          //   p  <- [ p(x,y,t) - p(-x,y,t) ) ]/2


	  real cwt = cos(wr*t)*exp(wi*t), swt = sin(wr*t)*exp(wi*t);
	  real sinkx=sin(kxHat*x), coskx=cos(kxHat*x);
	  if( numberOfTimeDerivatives==0 )
	  {
	    uLocal(i1,i2,i3,pc)  = amp*( twfa(i2a,0)*swt - twfa(i2a,1)*cwt )*sinkx;
	    uLocal(i1,i2,i3,v1c) = amp*( twfa(i2a,2)*cwt + twfa(i2a,3)*swt )*coskx;  
	    uLocal(i1,i2,i3,v2c) = amp*( twfa(i2a,4)*swt - twfa(i2a,5)*cwt )*sinkx;
	    // uLocal(i1,i2,i3,pc)  = amp*( twfa(i2a,0)*cwt + twfa(i2a,1)*swt )*sinkx;
	    // uLocal(i1,i2,i3,v1c) = amp*( twfa(i2a,2)*swt - twfa(i2a,3)*cwt )*coskx;  
	    // uLocal(i1,i2,i3,v2c) = amp*( twfa(i2a,4)*swt - twfa(i2a,5)*cwt )*sinkx;
	  }
	  else if( numberOfTimeDerivatives==1 )
	  {
            // -- first time derivative --
            real cwtp=  -wr*swt+wi*cwt, swtp=wr*cwt+wi*swt;
	    uLocal(i1,i2,i3,pc)  = amp*( twfa(i2a,0)*swtp - twfa(i2a,1)*cwtp )*sinkx;
	    uLocal(i1,i2,i3,v1c) = amp*( twfa(i2a,2)*cwtp + twfa(i2a,3)*swtp )*coskx;  
	    uLocal(i1,i2,i3,v2c) = amp*( twfa(i2a,4)*swtp - twfa(i2a,5)*cwtp )*sinkx;

	    // uLocal(i1,i2,i3,pc)  = amp*( twfa(i2a,0)*cwtp + twfa(i2a,1)*swtp )*sinkx;
	    // uLocal(i1,i2,i3,v1c) = amp*( twfa(i2a,2)*swtp - twfa(i2a,3)*cwtp )*coskx;  
	    // uLocal(i1,i2,i3,v2c) = amp*( twfa(i2a,4)*swtp - twfa(i2a,5)*cwtp )*sinkx;
	  }
	  else
	  {
	    OV_ABORT("finish me");
	  }
	  
	}
	
	// printF(" twfa(i2a,4)=%g, twfa(i2a,5)=%g, coskxwt(i1,i2,i3)=%g sinkxwt(i1,i2,i3)=%g\n",
	//  twfa(i2a,4),twfa(i2a,5),coskxwt(i1,i2,i3),sinkxwt(i1,i2,i3));
	// printF(" (x,y)=(%g,%g) (p,v1,v2)=(%g,%g,%g)\n",x,y,uLocal(i1,i2,i3,pc) ,uLocal(i1,i2,i3,v1c),uLocal(i1,i2,i3,v2c) );

	  
	  
      }
    }
    
  }
  else
  {
    printF("TravelingWaveFsi:: Unknown pde=[%s]\n",(const char*)pde);
    OV_ABORT("finish me");
  }
  
  return 0;
}


//================================================================================================
/// \brief Compute the exact solution in the (bulk) solid at time t.
///
/// \param u (output) : exact solution 
/// \param t (input) : evaluate the solution at this time.
/// \param mg (input) : MappedGrid
/// \param I1,I2,I3 (input) : evaluate at these points.
/// \param numberOfTimeDerivatives (input) : evaluate this many time-derivatives of the solution.
///
//================================================================================================
int TravelingWaveFsi::
getExactSolidSolution( RealArray & u, real t, MappedGrid & mg, const Index & I1, const Index & I2, const Index & I3, int numberOfTimeDerivatives /* =0 */ )
{
  const aString & pde = dbase.get<aString>("pde"); // pde we are solving
  const int & debug = dbase.get<int>("debug");
  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  int & current = dbase.get<int>("current");
  int prev = (current - 1 + numberOfTimeLevels) % numberOfTimeLevels;
  int next = (current + 1 + numberOfTimeLevels) % numberOfTimeLevels;

  RealArray & time = dbase.get<RealArray>("time");
  real tc = time(current);

  // OV_GET_SERIAL_ARRAY(real,u,uLocal);
  RealArray & uLocal =u;
  OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);


  int & u1c = dbase.get<int >("u1c");
  int & u2c = dbase.get<int >("u2c");

  int & v1c = dbase.get<int >("v1c");
  int & v2c = dbase.get<int >("v2c");

  int & s11c = dbase.get<int >("s11c");
  int & s12c = dbase.get<int >("s12c");
  int & s21c = dbase.get<int >("s21c");
  int & s22c = dbase.get<int >("s22c");

  // assert( u1c>=0 && u2c>=0 && v1c>=0 && v2c>=0 );

  real & cfl = dbase.get<real>("cfl");
  real & rhoe = dbase.get<real>("rhoe");
  real & lambdae = dbase.get<real>("lambdae");
  real & mue = dbase.get<real>("mue");

  real & rho = dbase.get<real>("rho");
  real & mu  = dbase.get<real>("mu");

  real & ke = dbase.get<real>("ke");
  real & te = dbase.get<real>("te");
  real & be = dbase.get<real>("be");
  real & ad = dbase.get<real>("ad");

  real & kx = dbase.get<real>("kx");
  real & height = dbase.get<real>("height");
  real & length = dbase.get<real>("length");

  real & amp = dbase.get<real>("amp");
  real & x0  = dbase.get<real>("x0");
  real & t0  = dbase.get<real>("t0");

  const bool & standingWaveSolution = dbase.get<bool>("standingWaveSolution");

  // // Traveling wave parameters
  // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");

    
  const real csq = (lambdae+2.0*mue)/rhoe;  // solid sound speed squared
  const real lamp2mu = lambdae+2.0*mue;

  assert( numberOfTimeDerivatives==0 ); // finish me 

  uLocal=0.;
  // printF("++++++TravelingWaveFsi: pde=[%s]\n",(const char*)pde);
  

  if( true || mu>0. )
  {
    // --- TravelingWave: viscous fluid + Elastic SHELL OR Acoustic Solid  ----

    // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");
    // const real amp = fwp[0];
    // const real kx  = fwp[1];
    // const real ky  = fwp[2];
    // const real x0  = fwp[3];
    // const real t0  = fwp[4];

    const real kxHat=twoPi*kx/length;

    assert( dbase.has_key("tws") );
	
    RealArray & tws = dbase.get<RealArray>("tws");

    real *omegav =  dbase.get<real[2]>("omegav");
    const real wr=omegav[0], wi=omegav[1];

    if( t==0. )
      printF("-TW-- getExactSolidSolution: w=(%9.3e,%9.3e), u1c=%i, u2c=%i, v1c=%i, v2c=%i\n",wr,wi,u1c,u2c,v1c,v2c);

    if( !standingWaveSolution )
    {
      // --- traveling wave solution ---
      RealArray coskxwt(I1,I2,I3), sinkxwt(I1,I2,I3); 
      // printF(" TravelingWave: kxHat=%e, wr=%e, t=%e\n",kxHat,wr,t);
    
      coskxwt=cos(kxHat*xLocal(I1,I2,I3,0)-wr*t);
      sinkxwt=sin(kxHat*xLocal(I1,I2,I3,0)-wr*t);

      // Real part of vHat*exp( i( kx-w*t) )
      bool acoustic=pde=="InsAcousticSolid";
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	if( u1c>=0 )
	  uLocal(I1,i2,I3,u1c) = amp*( tws(i2,0)*coskxwt(I1,i2,I3) - tws(i2,1)*sinkxwt(I1,i2,I3) )*exp(wi*t);

        if( u2c>=0 )
   	  uLocal(I1,i2,I3,u2c) = amp*( tws(i2,2)*coskxwt(I1,i2,I3) - tws(i2,3)*sinkxwt(I1,i2,I3) )*exp(wi*t);

	// 
	if( v1c>=0 )
	  uLocal(I1,i2,I3,v1c) = ( (amp*wr)*( tws(i2,0)*sinkxwt(I1,i2,I3) + tws(i2,1)*coskxwt(I1,i2,I3) )+
				   (wi*amp)*( tws(i2,0)*coskxwt(I1,i2,I3) - tws(i2,1)*sinkxwt(I1,i2,I3) ))*exp(wi*t);
      
        if( v2c>=0 )
	  uLocal(I1,i2,I3,v2c) = ( (amp*wr)*( tws(i2,2)*sinkxwt(I1,i2,I3) + tws(i2,3)*coskxwt(I1,i2,I3) )+
	   			   (wi*amp)*( tws(i2,2)*coskxwt(I1,i2,I3) - tws(i2,3)*sinkxwt(I1,i2,I3) ))*exp(wi*t);

	if( s21c>=0 && s22c>=0 )
	{
	  // s21 = rhoe*csq* u2_x    
	  // s22 = rhoe*csq* u2_y
	  if( acoustic )
	  {
	    uLocal(I1,i2,I3,s21c) = (rhoe*csq*kxHat*amp)*((-tws(i2,2))*sinkxwt(I1,i2,I3) - tws(i2,3)*coskxwt(I1,i2,I3) )*exp(wi*t);
	    // u2y appears in tws(i,6:7)
	    uLocal(I1,i2,I3,s22c) = (rhoe*csq*amp)*( tws(i2,6)*coskxwt(I1,i2,I3) - tws(i2,7)*sinkxwt(I1,i2,I3) )*exp(wi*t);
	  }
	  else
	  {
	    // Elastic wave equation
	    // s11 = lamp2mu*u1x + lam*u2y
	    // s12=s21 = mu*( u1y+u2x )
	    // s22 = lamp2mu*u2y + lam*u1x
	    int i3=I3.getBase();
	    for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      real u1x=(amp*kxHat)*((-tws(i2,0))*sinkxwt(i1,i2,i3) - tws(i2,1)*coskxwt(i1,i2,i3) )*exp(wi*t);
	      real u2x=(amp*kxHat)*((-tws(i2,2))*sinkxwt(i1,i2,i3) - tws(i2,3)*coskxwt(i1,i2,i3) )*exp(wi*t);

	      real u1y=amp*( tws(i2,4)*coskxwt(i1,i2,i3) - tws(i2,5)*sinkxwt(i1,i2,i3) )*exp(wi*t);
	      real u2y=amp*( tws(i2,6)*coskxwt(i1,i2,i3) - tws(i2,7)*sinkxwt(i1,i2,i3) )*exp(wi*t);
	  

	      uLocal(i1,i2,i3,s11c) =lamp2mu*u1x + lambdae*u2y;
	      uLocal(i1,i2,i3,s12c) =mue*( u1y+u2x );
	      uLocal(i1,i2,i3,s21c) =uLocal(i1,i2,i3,s12c);
	      uLocal(i1,i2,i3,s22c) =lamp2mu*u2y + lambdae*u1x;
	    }
	  
	  }
	

	}
      }
    }
    else
    {
      // --- standing wave ---

      RealArray coskx(I1,I2,I3), sinkx(I1,I2,I3); 
      coskx=cos(kxHat*xLocal(I1,I2,I3,0));
      sinkx=sin(kxHat*xLocal(I1,I2,I3,0));

      real cwt = cos(wr*t)*exp(wi*t), swt = sin(wr*t)*exp(wi*t);
      real cwtp=  -wr*swt+wi*cwt, swtp=wr*cwt+wi*swt;

      // Real part of vHat*exp( i( kx-w*t) )
      bool acoustic=pde=="InsAcousticSolid";
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	if( u1c>=0 )
	  uLocal(I1,i2,I3,u1c) = amp*( tws(i2,0)*cwt + tws(i2,1)*swt )*coskx(I1,i2,I3);

        if( u2c>=0 )
   	  uLocal(I1,i2,I3,u2c) = amp*( tws(i2,2)*swt - tws(i2,3)*cwt )*sinkx(I1,i2,I3);

	// 
	if( v1c>=0 )
	  uLocal(I1,i2,I3,v1c) = amp*( tws(i2,0)*cwtp + tws(i2,1)*swtp )*coskx(I1,i2,I3);
        if( v2c>=0 )
	  uLocal(I1,i2,I3,v2c) = amp*( tws(i2,2)*swtp - tws(i2,3)*cwtp )*sinkx(I1,i2,I3);

	if( s21c>=0 && s22c>=0 )
	{
	  // s21 = rhoe*csq* u2_x    
	  // s22 = rhoe*csq* u2_y
	  if( acoustic )
	  {
	    uLocal(I1,i2,I3,s21c) = (rhoe*csq*kxHat*amp)*( tws(i2,2)*swt - tws(i2,3)*cwt )*coskx(I1,i2,I3);
	    // u2y appears in tws(i,6:7)
	    uLocal(I1,i2,I3,s22c) =(rhoe*csq*amp)*( tws(i2,6)*swt - tws(i2,7)*cwt )*sinkx(I1,i2,I3);
	  }
	  else
	  {
	    // Elastic wave equation
	    // s11 = lamp2mu*u1x + lam*u2y
	    // s12=s21 = mu*( u1y+u2x )
	    // s22 = lamp2mu*u2y + lam*u1x
	    int i3=I3.getBase();
	    for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      real u1x=(-amp*kxHat)*( tws(i2,0)*cwt + tws(i2,1)*swt )*sinkx(i1,i2,i3);
	      real u2x=( amp*kxHat)*( tws(i2,2)*swt - tws(i2,3)*cwt )*coskx(i1,i2,i3);

	      real u1y=amp*( tws(i2,4)*cwt + tws(i2,5)*swt )*coskx(i1,i2,i3);
	      real u2y=amp*( tws(i2,6)*swt - tws(i2,7)*cwt )*sinkx(i1,i2,i3);
	  

	      uLocal(i1,i2,i3,s11c) =lamp2mu*u1x + lambdae*u2y;
	      uLocal(i1,i2,i3,s12c) =mue*( u1y+u2x );
	      uLocal(i1,i2,i3,s21c) =uLocal(i1,i2,i3,s12c);
	      uLocal(i1,i2,i3,s22c) =lamp2mu*u2y + lambdae*u1x;
	    }
	  
	  }
	

	}
      }


    }
    
  }
  else
  {
    printF("TravelingWaveFsi:: Unknown pde=[%s]\n",(const char*)pde);
    OV_ABORT("finish me");
  }
  

  return 0;
}

//================================================================================================
/// \brief Compute the exact solution in the solid shell at time t.
///
/// \param x (input) : coordinates
/// \param ue (output) : exact solution for the displacement
/// \param ve (output) : exact solution for the velocity     
/// \param ve (output) : exact solution for the acceleration
/// \param t (input) : evaluate the solution at this time.
/// \param I1,I2,I3 (input) : evaluate at these points.
///
//================================================================================================
int TravelingWaveFsi::
getExactShellSolution( const RealArray & x, RealArray & ue, RealArray & ve, RealArray & ae, real t, 
		       const Index & I1, const Index & I2, const Index & I3 )
{
  const aString & pde = dbase.get<aString>("pde"); // pde we are solving
  const int & debug = dbase.get<int>("debug");
  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  int & current = dbase.get<int>("current");
  int prev = (current - 1 + numberOfTimeLevels) % numberOfTimeLevels;
  int next = (current + 1 + numberOfTimeLevels) % numberOfTimeLevels;

  RealArray & time = dbase.get<RealArray>("time");
  real tc = time(current);

  int & u1c = dbase.get<int >("u1c");
  int & u2c = dbase.get<int >("u2c");

  int & v1c = dbase.get<int >("v1c");
  int & v2c = dbase.get<int >("v2c");

  int & s11c = dbase.get<int >("s11c");
  int & s12c = dbase.get<int >("s12c");
  int & s21c = dbase.get<int >("s21c");
  int & s22c = dbase.get<int >("s22c");

  real & cfl = dbase.get<real>("cfl");
  real & rhoe = dbase.get<real>("rhoe");
  real & lambdae = dbase.get<real>("lambdae");
  real & mue = dbase.get<real>("mue");

  real & rho = dbase.get<real>("rho");
  real & mu  = dbase.get<real>("mu");

  real & ke = dbase.get<real>("ke");
  real & te = dbase.get<real>("te");
  real & be = dbase.get<real>("be");
  real & ad = dbase.get<real>("ad");

  real & kx = dbase.get<real>("kx");
  real & height = dbase.get<real>("height");
  real & length = dbase.get<real>("length");

  real & amp = dbase.get<real>("amp");
  real & x0  = dbase.get<real>("x0");
  real & t0  = dbase.get<real>("t0");

  const bool & standingWaveSolution = dbase.get<bool>("standingWaveSolution");

  // // Traveling wave parameters
  // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");

    
  if( pde=="InsShell" && mu==0. )
  {
    // --- inviscid Traveling Wave Solution, Elastic Shell Solid ---
    // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");
    // const real amp = fwp[0];
    // const real kx  = fwp[1];
    // const real ky  = fwp[2];
    // const real x0  = fwp[3];
    // const real t0  = fwp[4];

    const real kxHat=twoPi*kx/length;

    const real am = rho/(kxHat*tanh(kxHat*height));  // added-mass
    const real omegaHat= sqrt( (ke + SQR(kxHat)*te)/(rhoe+am) );

    if( t==0. )
      printF("\n++getExactShellSolution : kxHat=%e, rhos=%8.2e ke=%8.2e te=%8.2e rho=%8.2e "
	     "mu=%8.2e H=%8.2e omegaHat=%18.14e\n",kxHat,rhoe,ke,te,rho,mu,height,omegaHat);

    if( !standingWaveSolution )
    {
      // traveling wave 
      RealArray coskxwt(I1,I2,I3), sinkxwt(I1,I2,I3); 
      coskxwt=cos(kxHat*x(I1,I2,I3,0)-omegaHat*t);
      sinkxwt=sin(kxHat*x(I1,I2,I3,0)-omegaHat*t);

      // Shell displacement and velocity:
      ue(I1,I2,I3,0)= x(I1,I2,I3,0);
      ue(I1,I2,I3,1)= x(I1,I2,I3,1) + amp*coskxwt;
    
      ve(I1,I2,I3,0)= 0.;
      ve(I1,I2,I3,1)= (amp*omegaHat)*sinkxwt;

      ae(I1,I2,I3,0)= 0.;
      ae(I1,I2,I3,1)= (-amp*omegaHat*omegaHat)*coskxwt;


    }
    else
    {
      // standing wave
      RealArray sinkx;
      sinkx=sin(kxHat*x(I1,I2,I3,0)+x0);

      ue(I1,I2,I3,0)= x(I1,I2,I3,0);
      ue(I1,I2,I3,1)= x(I1,I2,I3,1) + (amp*cos(omegaHat*t+t0))*sinkx;
    
      ve(I1,I2,I3,0)= 0.;
      ve(I1,I2,I3,1)= (-amp*omegaHat*sin(omegaHat*t+t0))*sinkx;

      ae(I1,I2,I3,0)= 0.;
      ae(I1,I2,I3,1)= (-amp*omegaHat*omegaHat*cos(omegaHat*t+t0))*sinkx;
    }
    
  }
  else if( pde=="InsShell" && mu>0. )
  {
    // --- Viscous Traveling Wave Solution, Elastic Shell Solid ---
    // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");
    // const real amp = fwp[0];
    // const real kx  = fwp[1];
    // const real ky  = fwp[2];
    // const real x0  = fwp[3];
    // const real t0  = fwp[4];

    const real kxHat=twoPi*kx/length;

    assert( dbase.has_key("twf") );
	
    RealArray & twf = dbase.get<RealArray>("twf");
    RealArray & tws = dbase.get<RealArray>("tws");

    const real omegaHat=1.;

    real *omegav =  dbase.get<real[2]>("omegav");
    const real wr=omegav[0], wi=omegav[1];

    if( !standingWaveSolution )
    {
      RealArray coskxwt(I1,I2,I3), sinkxwt(I1,I2,I3); 
      coskxwt=cos(kxHat*x(I1,I2,I3,0)-wr*t);
      sinkxwt=sin(kxHat*x(I1,I2,I3,0)-wr*t);

      const real ewit = exp(wi*t);
      const real a1=amp*tws(0,0)*ewit, a2=-amp*tws(0,1)*ewit;
      const real b1=amp*tws(0,2)*ewit, b2=-amp*tws(0,3)*ewit;
      
      const real a1t = wi*a1, a2t=wi*a2;
      const real b1t = wi*b1, b2t=wi*b2;

      const real a1tt = wi*wi*a1, a2tt=wi*wi*a2;
      const real b1tt = wi*wi*b1, b2tt=wi*wi*b2;

      // eta1, and eta2:
      ue(I1,I2,I3,0)= x(I1,I2,I3,0) + a1*coskxwt + a2*sinkxwt;
      ue(I1,I2,I3,1)= x(I1,I2,I3,1) + b1*coskxwt + b2*sinkxwt;
    
      // shell velocities:
      ve(I1,I2,I3,0)= (-wr*a2 +a1t)*coskxwt + (wr*a1 + a2t)*sinkxwt;
      ve(I1,I2,I3,1)= (-wr*b2 +b1t)*coskxwt + (wr*b1 + b2t)*sinkxwt;

      // shell acceleration:
      ae(I1,I2,I3,0)= (-wr*wr*a1 -2.*wr*a2t +a1tt)*coskxwt + (-wr*wr*a2 + 2.*wr*a1t + a2tt)*sinkxwt;
      ae(I1,I2,I3,1)= (-wr*wr*b1 -2.*wr*b2t +b1tt)*coskxwt + (-wr*wr*b2 + 2.*wr*b1t + b2tt)*sinkxwt;
    }
    else
    {
      // --- standing wave ---   ***THIS IS WRONG ***
      //OV_ABORT("finish me");
      // printF("TravelingWaveFsi::getExactShellSolution t=%8.2e, amp=%8.2e\n",t,amp);
      
      real cwt = cos(wr*t)*exp(wi*t), swt = sin(wr*t)*exp(wi*t);
      real cwtp = -wr*swt + wi*cwt;  // d(cwt)/dt 
      real swtp =  wr*cwt + wi*swt;  // d(swt)/dt 

      real cwtpp = -wr*wr*cwt - 2.*wr*wi*swt + wi*wi*cwt;  // d^2(cwt)/dt^2 
      real swtpp = -wr*wr*swt + 2.*wr*wi*cwt + wi*wi*swt;  // d^2(swt)/dt^2 

      RealArray sinkx(I1,I2,I3); 
      sinkx=sin(kxHat*x(I1,I2,I3,0));

      // eta1, and eta2:
      ue(I1,I2,I3,0)= x(I1,I2,I3,0) + amp*( tws(0,0)*swt - tws(0,1)*cwt )*sinkx;
      ue(I1,I2,I3,1)= x(I1,I2,I3,1) + amp*( tws(0,2)*swt - tws(0,3)*cwt )*sinkx;
    
      // shell velocities:
      ve(I1,I2,I3,0)= amp*( tws(0,0)*swtp - tws(0,1)*cwtp )*sinkx;
      ve(I1,I2,I3,1)= amp*( tws(0,2)*swtp - tws(0,3)*cwtp )*sinkx;

      // shell acceleration:
      ae(I1,I2,I3,0)= amp*( tws(0,0)*swtpp - tws(0,1)*cwtpp )*sinkx;
      ae(I1,I2,I3,1)= amp*( tws(0,2)*swtpp - tws(0,3)*cwtpp )*sinkx;


    }
    
  }
  else
  {
    printF("TravelingWaveFsi:: Unknown pde=[%s]\n",(const char*)pde);
    OV_ABORT("finish me");
  }
  

  return 0;
}



// ============================================================================
/// \brief Destructor.
// ============================================================================
TravelingWaveFsi::
~TravelingWaveFsi()
{
  if( dbase.has_key("usa" ) )
  {
    delete [] dbase.get<RealArray*>("usa");   // solid solution vectors 
    delete [] dbase.get<RealArray*>("fsa");   // solid forcing functions 
    delete [] dbase.get<RealArray*>("wsa");   // solid work-space vectors

    delete [] dbase.get<RealArray*>("ufa");   // fluid solution vectors 
    delete [] dbase.get<RealArray*>("ffa");   // forcing functions 
    delete [] dbase.get<RealArray*>("wfa");   // fluid work-space vectors
  }
}


// =========================================================================================
/// \brief Setup routine. Allocate arrays etc.
// =========================================================================================
int TravelingWaveFsi::
setup( CompositeGrid & cg, CompositeGrid & cgSolid )
{
  int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  // -- for now the number of fluid grid points matches those in cg ---
  MappedGrid & mg = cg[0];
  const IntegerArray & gid = mg.gridIndexRange();
  numberOfFluidGridPoints = gid(1,axis2)-gid(0,axis2)+1;

  MappedGrid & mgSolid = cgSolid[0];
  const IntegerArray & gidSolid = mgSolid.gridIndexRange();
  numberOfSolidGridPoints = gidSolid(1,axis2)-gidSolid(0,axis2)+1;

  return setup( numberOfFluidGridPoints, numberOfSolidGridPoints );
}

// =========================================================================================
/// \brief Setup routine. Allocate arrays etc.
// =========================================================================================
int TravelingWaveFsi::
setup( int numberOfFluidGridPointsY, int numberOfSolidGridPointsY )
{
  int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  numberOfFluidGridPoints=numberOfFluidGridPointsY;
  numberOfSolidGridPoints=numberOfSolidGridPointsY;

  const int & debug = dbase.get<int>("debug");
  const aString & pde = dbase.get<aString>("pde");

  bool & normalMotionOnly = dbase.get<bool>("normalMotionOnly");

  const int & numberOfSolidComponents = dbase.get<int>("numberOfSolidComponents");
  const int & numberOfFluidComponents = dbase.get<int>("numberOfFluidComponents");

  const int & numberOfSolidGhostPoints = dbase.get<int>("numberOfSolidGhostPoints");
  const int & numberOfFluidGhostPoints = dbase.get<int>("numberOfFluidGhostPoints");

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  assert( numberOfTimeLevels>0 );
  
  const int & numberOfWorkSpaceVectors = dbase.get<int>("numberOfWorkSpaceVectors");
  assert( numberOfWorkSpaceVectors>0 );
  
  // // -- for now the number of fluid grid points matches those in cg ---
  // MappedGrid & mg = cg[0];
  // const IntegerArray & gid = mg.gridIndexRange();
  // numberOfFluidGridPoints = gid(1,axis2)-gid(0,axis2)+1;

  // MappedGrid & mgSolid = cgSolid[0];
  // const IntegerArray & gidSolid = mgSolid.gridIndexRange();
  // numberOfSolidGridPoints = gidSolid(1,axis2)-gidSolid(0,axis2)+1;

  
  printF("****FW setting numberOfFluidGridPoints=%i, numberOfSolidGridPoints=%i (y-direction) ***\n",
           numberOfFluidGridPoints,numberOfSolidGridPoints);

  // ---- Twilight Zone Functions ----
  const int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  const int & degreeInTime = dbase.get<int>("degreeInTime");
  const int & degreeInSpace = dbase.get<int>("degreeInSpace");
  const bool & twilightZone = dbase.get<bool>("twilightZone");

  OGFunction *& exactPointer = dbase.get<OGFunction*>("exactPointer");

  const int numberOfComponents=numberOfFluidComponents;
  const int numberOfTZComponents= numberOfFluidComponents;
  const int numberOfDimensions=1;
  if( twilightZoneOption==polynomial )
  {
    // --------------------------------------------
    // --------- Polynomial TZ --------------------
    // --------------------------------------------

    const int tzDegreeSpace = degreeInSpace;
    const int degreeTime = degreeInTime;

    exactPointer = new OGPolyFunction(tzDegreeSpace,numberOfDimensions,numberOfTZComponents,degreeTime);

    const int ndp=max(max(5,tzDegreeSpace+1),degreeTime+1);
    
    printF("\n $$$$$$$ FW::setup: setTwilightZoneFunction: tzDegreeSpace=%i, degreeTime=%i ndp=%i $$$$\n",
	   tzDegreeSpace,degreeTime,ndp);

    RealArray spatialCoefficientsForTZ(ndp,ndp,ndp,numberOfTZComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(ndp,numberOfTZComponents);      
    timeCoefficientsForTZ=0.;

    for( int n=0; n< numberOfComponents; n++ )
    {
      const int tzDegreeSpace2 = numberOfDimensions>=2 ? tzDegreeSpace : 0;
      const int tzDegreeSpace3 = numberOfDimensions==3 ? tzDegreeSpace : 0;

      for( int m1=0; m1<=tzDegreeSpace; m1++ )for( int m2=0; m2<=tzDegreeSpace2; m2++ )for( int m3=0; m3<=tzDegreeSpace3; m3++ )
      {
	if( (m1+m2+m3)<=tzDegreeSpace )
	{ // choose "random" coefficients
	  spatialCoefficientsForTZ(m1,m2,m3,n)=(pow(-1.,m1+2*m2+3*m3+n) )/(1.+ (.25+n)*m1+m2+(1.5+n)*m3);
	}
      }
    }
    
    for( int n=0; n<numberOfComponents; n++ )
    {
      for( int i=0; i<ndp; i++ )
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
    }
    
    ((OGPolyFunction*)exactPointer)->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 

  }
  // -------------------------------------------------------------------------------------------
  else if( twilightZoneOption==trigonometric )
  {
    const int nc = numberOfTZComponents; 

    RealArray fx(nc),fy(nc),fz(nc),ft(nc);
    RealArray gx(nc),gy(nc),gz(nc),gt(nc);
    gx=0.;
    gy=0.;
    gz=0.;
    gt=0.;
    RealArray amplitude(nc), cc(nc);
    amplitude=1.;
    cc=0.;

    real *trigFreq = dbase.get<real[4]>("trigFreq");  // ft, fx, fy, [fz]
    const real omegav[4]={trigFreq[1],trigFreq[2],trigFreq[3],trigFreq[0]};

    fx = omegav[0];
    fy = numberOfDimensions>1 ? omegav[1] : 0.;
    fz = numberOfDimensions>2 ? omegav[2] : 0.;
    ft = omegav[3];

    exactPointer = new OGTrigFunction(fx,fy,fz,ft);
    
    ((OGTrigFunction*)exactPointer)->setShifts(gx,gy,gz,gt);
    ((OGTrigFunction*)exactPointer)->setAmplitudes(amplitude);
    ((OGTrigFunction*)exactPointer)->setConstants(cc);

  }
  else
  {
    printF("FW::setup:ERROR:unknown value for twilightZoneOption=%i\n",(int)twilightZoneOption);
    OV_ABORT("FW::setup:ERROR");
  }
    


  OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

  Range Is(-numberOfSolidGhostPoints,numberOfSolidGridPoints-1+numberOfSolidGhostPoints);
  Range If(-numberOfFluidGhostPoints,numberOfFluidGridPoints-1+numberOfFluidGhostPoints);
  

  // allocate arrays to hold the solution
  if( !dbase.has_key("usa" ) )
  {
    dbase.put<RealArray*>("usa");   // solid solution vectors 
    dbase.put<RealArray*>("fsa");   // solid forcing functions 

    dbase.put<RealArray*>("wsa");   // solid work-space vectors

    dbase.put<RealArray*>("ufa");   // fluid solution vectors 
    dbase.put<RealArray*>("ffa");   // forcing functions 

    dbase.put<RealArray*>("wfa");   // fluid work-space vectors 
  }


  RealArray *& usa = dbase.get<RealArray*>("usa");
  RealArray *& fsa = dbase.get<RealArray*>("fsa");
  RealArray *& wsa = dbase.get<RealArray*>("wsa");

  RealArray *& ufa = dbase.get<RealArray*>("ufa");
  RealArray *& ffa = dbase.get<RealArray*>("ffa");
  RealArray *& wfa = dbase.get<RealArray*>("wfa");
    
  usa = new RealArray [numberOfTimeLevels];
  fsa = new RealArray [numberOfTimeLevels];

  wsa = new RealArray [numberOfWorkSpaceVectors];

  ufa = new RealArray [numberOfTimeLevels];
  ffa = new RealArray [numberOfTimeLevels];

  wfa = new RealArray [numberOfWorkSpaceVectors];
    
  for( int m=0; m<numberOfTimeLevels; m++ )
  {
    usa[m].redim(Is,numberOfSolidComponents);
    fsa[m].redim(Is,numberOfSolidComponents);

    ufa[m].redim(If,numberOfFluidComponents);
    ffa[m].redim(If,numberOfFluidComponents);

    usa[m]=0.;
    fsa[m]=0.;
    ufa[m]=0.;
    ffa[m]=0.;
  }

  for( int m=0; m<numberOfWorkSpaceVectors; m++ )
  {
    wsa[m].redim(Is,numberOfSolidComponents);
    wfa[m].redim(If,numberOfFluidComponents);
  }
  

  const real & height = dbase.get<real>("height");
  const real & length = dbase.get<real>("length");

  // Here is an array of the y coordinates for the fluid
  RealArray & yCoord = dbase.get<RealArray>("yCoord");
  yCoord.redim(If,1,1);

  real & dy =  dbase.get<real>("dy");
  dy = height/(numberOfFluidGridPoints-1.);
  for( int i=If.getBase(); i<=If.getBound(); i++ )
  {
    yCoord(i,0,0)=i*dy;
  }
 
  // Here is an array of the y coordinates for the fluid
  RealArray & ysCoord = dbase.get<RealArray>("ysCoord");
  ysCoord.redim(Is,1,1);
  for( int i=Is.getBase(); i<=Is.getBound(); i++ )
  {
    ysCoord(i,0,0)=height + i*dy;
  }
  // ::display(xa[2],"ElasticShell:: xa[2]");
    
  // times are stored in the array time:
  dbase.put<RealArray>("time");
  RealArray & time = dbase.get<RealArray>("time");
  time.redim(numberOfTimeLevels);
  time=0.;


  const real & rho = dbase.get<real>("rho");
  const real & mu  = dbase.get<real>("mu");
  const real & kx = dbase.get<real>("kx");

  const real & rhoe = dbase.get<real>("rhoe");
  const real & ke = dbase.get<real>("ke");
  const real & te = dbase.get<real>("te");
  const real & be = dbase.get<real>("be");

  const real & hs = dbase.get<real>("hs"); // shell height

  const real & lambdae = dbase.get<real>("lambdae");
  const real & mue = dbase.get<real>("mue");
  const real & Hs = dbase.get<real>("Hs"); // bulk solid height

  const real cp = sqrt( (lambdae+2.0*mue)/rhoe );  
  const real cs = sqrt( mue/rhoe ); 

  // --- evaluate the surface reduced frequency ---
  real & omega = dbase.get<real>("omega");
  
  real kxHat = twoPi*kx/length;
  real Ma = rho/( kxHat*tanh(kxHat*height) );

  omega = (ke+kxHat*kxHat*te)/( rhoe*hs + Ma);

  if( !( pde=="InsShell" && mu==0 ) ) // No need to call for InsShell and mu==0
  {
    if( !dbase.has_key("twf") )
    {
      // Evaluate the time-independent parts of the traveling wave solution
      dbase.put<RealArray>("twf");
      dbase.put<RealArray>("tws");
    }
	
    int numGhost=2;
    Range Rf(-numGhost,numberOfFluidGridPoints+numGhost);
    Range Rs(-numGhost,numberOfSolidGridPoints+numGhost);

    RealArray & twf = dbase.get<RealArray>("twf");
    twf.redim(Rf,2*numberOfFluidComponents);           // we store the real and imag parts
    RealArray & tws = dbase.get<RealArray>("tws");
    int numSolid= 8;  // return us1, us2, [us1y, us2y]
    tws.redim(Rs,max(2*numberOfSolidComponents,numSolid ));    // store u1,u2 even for simple shell
    tws=0.;
      
      

    int pdeOption=0;
    if( pde=="InsShell" )
    {
      pdeOption=0;
    }
    else if( pde=="InsAcousticSolid" )
    {
      pdeOption=1;
    }
    else if( pde=="InsElasticSolid" )
    {
      pdeOption=2;
    }
    else
    {
      OV_ABORT("error");
    }
    
    int option;
    if( normalMotionOnly )
      option=0; // eta1=0,  eta2 varies
    else
      option=2;  // eta1 and eta2 vary

    real solidHeight=hs;  // shell
    int ipar[]={option,pdeOption};  //
    real rpar[]={kxHat,hs,rho,mu, rhoe,hs,ke,te,0.,0.};  //

    if( pde!="InsShell" )
    {
      // --- bulk solid ---
      if( pde=="InsAcousticSolid" ||
	  pde=="InsElasticSolid" )
      {
	rpar[5]=Hs;
	rpar[6]=cp;
	rpar[7]=cs;
	solidHeight=Hs;
      }
      else 
      {
	OV_ABORT("error");
      }
    }
    

    RealArray yf(Rf);
    for( int i=Rf.getBase(); i<=Rf.getBound(); i++ )
      yf(i)=-height + i*dy;
    
    const real dys=solidHeight/max(numberOfSolidGridPoints-1.,1);
    RealArray ys(Rs);
    for( int i=Rs.getBase(); i<=Rs.getBound(); i++ )
      ys(i)= i*dys;
    

    int b0=twf.getBase(0);
    real wr, wi;
    getTravelingWave( wr, wi, rpar, ipar, 
		      Rf.getLength(), yf.getDataPointer(), 
                      &twf(b0,0), &twf(b0,1), &twf(b0,2), &twf(b0,3), &twf(b0,4), &twf(b0,5), 
		      Rs.getLength(), ys.getDataPointer(), &tws(b0,0), &tws(b0,1), &tws(b0,2), &tws(b0,3),
                      &tws(b0,4), &tws(b0,5), &tws(b0,6), &tws(b0,7) );

	
    real *omegav =  dbase.get<real[2]>("omegav");
    omegav[0]=wr; omegav[1]=wi;

    if( debug & 4 )
    {
      ::display(twf,"traveling wave: (p,v1,v2) in y ","%5.2f ");
      ::display(tws,"traveling wave: (us1,us2) in y ","%8.2e ");
    }
    
  }
	


  return 0;
}


// ===========================================================================================
/// \brief Compute the maximum allowable time step.
///
/// \return value : maximum dt 
/// 
// ==========================================================================================
real TravelingWaveFsi::
getTimeStep( real t ) 
{

  const int & numberOfSolidComponents = dbase.get<int>("numberOfSolidComponents");
  const int & numberOfFluidComponents = dbase.get<int>("numberOfFluidComponents");

  const int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  const int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  Range Is = numberOfSolidGridPoints;
  Range If = numberOfFluidGridPoints;

  const real & cfl = dbase.get<real>("cfl");
  const real & rhoe = dbase.get<real>("rhoe");

  const real & rho = dbase.get<real>("rho");
  const real & mu  = dbase.get<real>("mu");

  const real & ke = dbase.get<real>("ke");
  const real & te = dbase.get<real>("te");
  const real & be = dbase.get<real>("be");
  const real & ad = dbase.get<real>("ad");

  const real & height = dbase.get<real>("height");
  const real & length = dbase.get<real>("length");

  const real & dy =  dbase.get<real>("dy");

  const real & omega = dbase.get<real>("omega");

  real dtSolid = .5*cfl/(omega*omega);
  
  // real and imaginary parts of the time-stepping eigenvalue: 
  real realPart = (mu/rho)*4./(dy*dy);  // factor 4 from 2nd-order scheme *FIX ME*
  real imPart = 0.;   // *FIX ME*

  // -- guess: *FIX ME*
  // Approximate the stability region by an ellipse
  //      (dt*Re/alpha)^2 + (dt*Im/beta) < cfl^2  
  // dt^2*(  (Re/alpha)^2 + (Im/beta)^2 ) < cfl 

  real alpha=2.7, beta=.8; // RK4 ??

  real dtFluid = cfl/sqrt( SQR(realPart/alpha) + SQR(imPart/beta) );

  real dt = min( dtSolid, dtFluid );

  printF("TravelingWaveFsi: cfl=%g, dt=%9.3e\n",cfl,dt);

  return dt;
}




// =================================================================================================
/// \brief Compute du/dt for the the InsShell solution.
// =================================================================================================
int TravelingWaveFsi::
assignInitialConditions( const real t, const real dt )
{
  const int & numberOfSolidComponents = dbase.get<int>("numberOfSolidComponents");
  const int & numberOfFluidComponents = dbase.get<int>("numberOfFluidComponents");

  const int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  const int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  Range Is = numberOfSolidGridPoints;
  Range If = numberOfFluidGridPoints;

  const real & cfl = dbase.get<real>("cfl");
  const real & rhoe = dbase.get<real>("rhoe");

  const real & rho = dbase.get<real>("rho");
  const real & mu  = dbase.get<real>("mu");

  const real & ke = dbase.get<real>("ke");
  const real & te = dbase.get<real>("te");
  const real & be = dbase.get<real>("be");
  const real & ad = dbase.get<real>("ad");

  // -- components:
  const int ec = dbase.get<int>("ec");
  const int etc = dbase.get<int>("etc");

  const int v1c = dbase.get<int>("v1fc");
  const int v2c = dbase.get<int>("v2fc");
  const int pc  = dbase.get<int>("pfc");

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  int & current = dbase.get<int>("current");
  int prev = (current - 1 + numberOfTimeLevels) % numberOfTimeLevels;
  int next = (current + 1 + numberOfTimeLevels) % numberOfTimeLevels;

  RealArray & time = dbase.get<RealArray>("time");
  // real t0 = time(current);

  RealArray *& usa = dbase.get<RealArray*>("usa");
  RealArray *& fsa = dbase.get<RealArray*>("fsa");

  RealArray *& ufa = dbase.get<RealArray*>("ufa");
  RealArray *& ffa = dbase.get<RealArray*>("ffa");

  RealArray & us = usa[current];
  RealArray & uf = ufa[current];

  assert( numberOfSolidComponents==2 );
  assert( numberOfFluidComponents==3 );
  

  Range Js=us.dimension(0);
  Range Jf=uf.dimension(0);
  
  RealArray & yCoord = dbase.get<RealArray>("yCoord");

  // -- test : ---
  const real & amp = dbase.get<real>("amp");
  const real & omega = dbase.get<real>("omega");
  const real & t0 = dbase.get<real>("t0"); // phase in t

  us(Js,ec ) =  amp*cos(omega*t+t0);        // eta 
  us(Js,etc) = -amp*omega*sin(omega*t+t0);  // etat 
  
  uf(Jf,v1c) = 0.;
  uf(Jf,v2c) = 0.;
  uf(Jf,pc ) = 0.;

  printF("---- FW: assignInitialConditions ---\n");
  

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  if( twilightZone )
  {
    // --- TZ initial condition ---
    OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

    Index Jf1=Jf, Jf2=0, Jf3=0;
    RealArray v2e(Jf1,1,1);
    //traction.display();
    int numberOfDimensions=1, rectangularForTZ=0;
    exact.gd( v2e,yCoord,numberOfDimensions,rectangularForTZ,0,0,0,0,Jf1,Jf2,Jf3,v2c,t);  // v2 exact 

    // ::display(v2e,"Initial conditions v2e","%g ");
    
    uf(Jf,v2c)=v2e;
  }
  else
  {
    // -- Initial conditions from inviscid solution ---
/* =======
      RealArray coskx(I1,I2,I3), sinkx(I1,I2,I3); 
      coskx=cos(kxHat*xLocal(I1,I2,I3,0)+x0);
      sinkx=sin(kxHat*xLocal(I1,I2,I3,0)+x0);

      RealArray coshkxy(I1,I2,I3), sinhkxy(I1,I2,I3); 
      coshkxy=cosh(kxHat*(xLocal(I1,I2,I3,1)));
      sinhkxy=sinh(kxHat*(xLocal(I1,I2,I3,1)));

      // Exact solution for the fluid pressure: (p.y=0 at y=0 )
      real factor = 1./(kxHat*sinh(kxHat*height));

      real pAmp = amp*rho*SQR(omegaHat)/(kxHat*sinh(kxHat*height));

      uLocal(I1,I2,I3,pc) = pAmp*sinkx*coshkxy*cos(omegaHat*t+t0);


      // Exact solution for the fluid velocity:
      // v1e = v1e(x,y,0) + (-1/rho) INT ( p_x ) dt
      // v2e = v2e(x,y,0) + (-1/rho) INT ( p_y ) dt

      // *CHECK ME*
      real vAmp = -amp*(omegaHat/sinh(kxHat*height))*sin(omegaHat*t+t0);

      uLocal(I1,I2,I3,v1c) = vAmp*coskx*coshkxy;

      uLocal(I1,I2,I3,v2c) = vAmp*sinkx*sinhkxy;
   ======= */


    const real & kx = dbase.get<real>("kx");
    const real & height = dbase.get<real>("height");
    const real & length = dbase.get<real>("length");
    const real kxHat = twoPi*kx/length;

    real vAmp = -amp*(omega/sinh(kxHat*height))*sin(omega*t+t0);
    uf(Jf,v2c) = vAmp*sinh(kxHat*yCoord(Jf));

    OV_ABORT("finish me");

  }
  

  dbase.get<real>("dt")=dt; // ** FIX ME***

  return 0;
}




// int TravelingWaveFsi::
// rungeKutta2(real & t, 
// 	    real dt,
// 	    realCompositeGridFunction & u1, 
// 	    realCompositeGridFunction & u2, 
// 	    realCompositeGridFunction & u3 );

int TravelingWaveFsi::
rungeKutta4(real t, 
	    real dt,
	    RealArray & us1, RealArray & uf1,
	    RealArray & usNew, RealArray & ufNew )
// ================================================================================
/// \brief Advance a time step using Fourth Order Runge-Kutta
///
///       y(n+1) = yn + 1/6( k1 + 2*k2 + 2*k3 + k4 )
///           k1 = dt*f(t,yn)
///           k2 = dt*f(t+.5*h,yn+.5*k1)
///           k3 = dt*f(t+.5*h,yn+.5*k2)
///           k4 = dt*f(t+h,yn+k3)
///
/// \param t (input) : current time
/// \param dt (input) : time step.
/// \param us1, uf1 (input) : solution at time t on input, solution at time t+dt at output
/// \param usNew, ufNew (output) : solution at time t+dt at output
//======================================================================
{
  // -- work space --
  RealArray *& wsa = dbase.get<RealArray*>("wsa");
  RealArray *& wfa = dbase.get<RealArray*>("wfa");

  const int & numberOfWorkSpaceVectors = dbase.get<int>("numberOfWorkSpaceVectors");
  assert( numberOfWorkSpaceVectors>=3 );

  RealArray & us2 = wsa[0];
  RealArray & us3 = wsa[1];
  RealArray & us4 = wsa[2];   // NOTEL us4,uf4 can probably be replaced with usNew, ufNew
  
  RealArray & uf2 = wfa[0];
  RealArray & uf3 = wfa[1];
  RealArray & uf4 = wfa[2];
  

  real dtb2=dt*.5;
  real dtb3=dt/3.;
  real dtb6=dt/6.;
  
  // -- STAGE I ---  
  getUt( us1,us2, uf1,uf2, t );  // ... us2 <- k1=d(us1)/dt(t), uf2 <- k1=d(uf1)/dt(t)
  us3=us1+dtb2*us2;   // ...u3 <- yn+.5*k1
  uf3=uf1+dtb2*uf2;   // ...u3 <- yn+.5*k1

  us4=us1+dtb6*us2;   // ...u4 <- yn+1/6( k1 )   keep a running sum of the result (saves space)
  uf4=uf1+dtb6*uf2;   // ...u4 <- yn+1/6( k1 )   keep a running sum of the result (saves space)
  
  applyBoundaryConditions( us3,uf3, t+dtb2 );

  // --- STAGE II ---
  getUt( us3,us2, uf3,uf2, t+dtb2 );  //  ...u2 <- k2 = f(u3)
  
  us3=us1+dtb2*us2;   // ...yn+.5*k2
  uf3=uf1+dtb2*uf2;   // ...yn+.5*k2

  us4+=dtb3*us2;       // ...yn+1/6( k1 +2*k2 )
  uf4+=dtb3*uf2;       // ...yn+1/6( k1 +2*k2 )
  
  applyBoundaryConditions( us3,uf3,t+dtb2 );

  // --- STAGE III ---
  getUt( us3,us2, uf3,uf2, t+dtb2 ); // ...u2 <- k3 = f(u3)

  us3=us1+dt*us2;    // ...yn+k3
  uf3=uf1+dt*uf2;    // ...yn+k3
    
  us4+=dtb3*us2;    // ...yn+1/6( k1 +2*k2 +2*k3 )
  uf4+=dtb3*uf2;    // ...yn+1/6( k1 +2*k2 +2*k3 )
 
  applyBoundaryConditions( us3,uf3, t+dt );

  // --- STAGE IV ---
  getUt( us3,us2, uf3,uf2,t+dt ); //  ...u2 <- k4 = f(u3)

  usNew=us4+dtb6*us2;
  ufNew=uf4+dtb6*uf2;
  
  applyBoundaryConditions( usNew,ufNew, t+dt );

  return 0;
}




// =================================================================================================
/// \brief Advance the solution to time t.
// =================================================================================================
int TravelingWaveFsi::
advance( real t )
{
  
  const aString & pde = dbase.get<aString>("pde");
  const aString & exactSolution = dbase.get<aString>("exactSolution");


  // -- Advance the viscous INS-Shell equations ---
  if( pde=="InsShell" )
  {
    advanceInsShell( t );
  }
  else
  {
    OV_ABORT("error");
  }



  return 0;
}

// =================================================================================================
/// \brief Advance the InsShell solution to time tFinal.
// =================================================================================================
int TravelingWaveFsi::
advanceInsShell( real tFinal )
{

  // InsShell Solution:
  //
  //  us(x,y,t) = sin(kxHat x ) usHat(t)
  //  vs(x,y,t) = sin(kxHat x ) vsHat(t)
  //
  //  p(x,y,t)  = sin(kxHat x ) pHat(y,t)
  //  v1(x,y,t) = cos(kxHat x ) v1Hat(y,t)
  //  v2(x,y,t) = sin(kxHat x ) v2Hat(y,t)


  // InsShell Equations: (for "hat" variables)
  //
  //         us_t = vs 
  //  rhoe * vs_t = - L us + p - 2*mu*v2_y 
  //  rho * v2_t = - p_y + mu*( -k^2 v2 + v2_yy )
  //   p = P(us,v2) 
  //
  // Bc:
  //    v2(H,t) = vs(t)
  //    v2(0,t) = 0 


  // current : index into the usa,ua,... arrays for the current solution    
  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  int & current = dbase.get<int>("current");
  int prev = (current - 1 + numberOfTimeLevels) % numberOfTimeLevels;
  int next = (current + 1 + numberOfTimeLevels) % numberOfTimeLevels;

  RealArray & time = dbase.get<RealArray>("time");
  real tInitial = time(current);

  RealArray *& usa = dbase.get<RealArray*>("usa");
  RealArray *& fsa = dbase.get<RealArray*>("fsa");

  RealArray *& ufa = dbase.get<RealArray*>("ufa");
  RealArray *& ffa = dbase.get<RealArray*>("ffa");


  // -- Method of lines solve: ---

  const real & dt = dbase.get<real>("dt");
  
  int numberOfSteps = int( (tFinal-tInitial)/dt + 1.5 );
  int maxNumberOfSteps = numberOfSteps + 100;

  const real eps = 100.*REAL_EPSILON*(tFinal+1.);
  real t=tInitial;
  for( int step=0; step<maxNumberOfSteps; step++ )
  {

    RealArray & usCurrent = usa[current];
    RealArray & ufCurrent = ufa[current];
    
    RealArray & usNext = usa[next];
    RealArray & ufNext = ufa[next];
    

    rungeKutta4( t ,dt, usCurrent, ufCurrent, usNext,ufNext );
    
    current = (current+1 + numberOfTimeLevels) % numberOfTimeLevels;
    t+= dt;

    time(current)=t;

    if( t>= tFinal - eps ) break;
  }
  



  return 0;
}


// =================================================================================================
/// \brief Compute du/dt (generic version)
// =================================================================================================
int TravelingWaveFsi::
getUt( RealArray & us, RealArray & ust, 
       RealArray & u, RealArray & ut, real t )
{

  const aString & pde = dbase.get<aString>("pde");

  if( pde=="InsShell" )
  {
    getUtInsShell( us,ust, u,ut, t );

  }
  else
  {
    OV_ABORT("ERROR");
  }

  return 0;
}


// =================================================================================================
/// \brief Compute du/dt for the the InsShell solution.
// =================================================================================================
int TravelingWaveFsi::
getUtInsShell( RealArray & us, RealArray & ust, 
               RealArray & uf, RealArray & uft, real t )
{

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  const int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  const int & orderOfAccuracyInSpace = dbase.get<int>("orderOfAccuracyInSpace");

  const int & degreeInTime = dbase.get<int>("degreeInTime");
//   const int & degreeInSpace = dbase.get<int>("degreeInSpace");

  const int & numberOfSolidComponents = dbase.get<int>("numberOfSolidComponents");
  const int & numberOfFluidComponents = dbase.get<int>("numberOfFluidComponents");

  const int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  const int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  Range Is = numberOfSolidGridPoints;
  Range If = numberOfFluidGridPoints;

  const real & cfl = dbase.get<real>("cfl");
  const real & rhoe = dbase.get<real>("rhoe");

  const real & rho = dbase.get<real>("rho");
  const real & mu  = dbase.get<real>("mu");
  const real nu = mu/rho;

  const real & ke = dbase.get<real>("ke");
  const real & te = dbase.get<real>("te");
  const real & be = dbase.get<real>("be");
  const real & ad = dbase.get<real>("ad");

  // -- components:
  const int ec = dbase.get<int>("ec");
  const int etc = dbase.get<int>("etc");

  const int v1c = dbase.get<int>("v1fc");
  const int v2c = dbase.get<int>("v2fc");
  const int pc  = dbase.get<int>("pfc");

  assert( numberOfSolidComponents==2 );
  assert( numberOfFluidComponents==3 );
  
  const real & height = dbase.get<real>("height");
  const real & length = dbase.get<real>("length");

  RealArray & yCoord = dbase.get<RealArray>("yCoord");

  const real & dy =  dbase.get<real>("dy");

  Range Js=us.dimension(0);
  Range Jf=uf.dimension(0);
  

  // -- test : ---
  real tp = degreeInTime*pow(t,degreeInTime-1); // test solution is t^p 

  const real & omega = dbase.get<real>("omega");
  const real & kx = dbase.get<real>("kx");
  const real kxHat = twoPi*kx/length;

  real sinhkH=sinh(kxHat*height), coshkH=cosh(kxHat*height), tanhkH=sinhkH/coshkH;

  real v2yya =0.;  
  real v2yyb =0.;
  int i1a=If.getBase(), i1b=If.getBound();
  if( orderOfAccuracyInSpace==2 )
  {
    v2yya = (uf(i1a+1,v2c) -2.*uf(i1a,v2c) + uf(i1a-1,v2c))/(dy*dy);
    v2yyb = (uf(i1b+1,v2c) -2.*uf(i1b,v2c) + uf(i1b-1,v2c))/(dy*dy);
  }
  else if( orderOfAccuracyInSpace==4 )
  {
    v2yya = (-30.*uf(i1a,v2c)+16.*(uf(i1a+1,v2c)+uf(i1a-1,v2c)) 
                 -(uf(i1a+2,v2c)+uf(i1a-2,v2c))  )/(12.*dy*dy);
    v2yyb = (-30.*uf(i1b,v2c)+16.*(uf(i1b+1,v2c)+uf(i1b-1,v2c)) 
                 -(uf(i1b+2,v2c)+uf(i1b-2,v2c))  )/(12.*dy*dy);

    // d24(kd) = 1./(12.*dr(kd)**2)
    //         urr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+u(
    //      & i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(0)

  }
  else
  {
    OV_ABORT("ERROR");
  }
  
  // --- compute eta_tt ---
  real q = ( mu*(coshkH-1.)/(kxHat*sinhkH) )*v2yyb;
  real etab = mu*SQR(kxHat)/(kxHat*tanhkH);
  
  if( twilightZone )
  {  // -- do this for now:
    q=0.;
    etab=0.;
  }
  

  real eta  = us(0,ec);
  real etat = us(0,etc);
  real etatt = -omega*omega*eta - etab*etat + q;  // d(etat)/dt

  

  // Compute p_y
  RealArray py(If);

  real Ap = mu*v2yya;
  real Bp = -rho*etatt + mu*( -SQR(kxHat)*eta + v2yyb);
  if( twilightZone )
  {  // -- do this for now:
    Ap=0.;
    Bp=0.;
  }

  py = Ap*cosh(kxHat*yCoord(If)) + (Bp/sinhkH - Ap/tanhkH)*sinh(kxHat*yCoord(If));
  
  
  // -- surface: ---
  ust(Js,ec) = us(Js,etc);                  // d(eta)/dt  = etat 
  ust(Js,etc) = etatt ;     
  
  // --- fluid ---
  uft(Jf,v1c) = tp;
  // uft(Jf,v2c) = tp;
  uft(Jf,pc) = tp;
  

  // Fluid momentum: 
  //   v2_t = -py/rho + (mu/rho)*( v2_yy )
  if( orderOfAccuracyInSpace==2 )
  {
    uft(If,v2c) = (-1./rho)*py + (nu/(dy*dy))*( uf(If+1,v2c)-2.*uf(If,v2c)+uf(If-1,v2c) );
  }
  else if( orderOfAccuracyInSpace==4 )
  {
    // d24(kd) = 1./(12.*dr(kd)**2)
    //         urr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+u(
    //      & i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(0)

    uft(If,v2c) = (-1./rho)*py + (nu/(12.*dy*dy))*( -30.*uf(If,v2c) + 16.*(uf(If+1,v2c)+uf(If-1,v2c)) 
						    -(uf(If+2,v2c) + uf(If-2,v2c)) );
  }
  else
  {
    OV_ABORT("error");
  }
  

  if( twilightZone )
  {
    // --- Add TZ forcing ---
    OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

    Index If1=If, If2=0, If3=0;
    RealArray v2t(If1,1,1), v2yy(If1,1,1);

    int numberOfDimensions=1, rectangularForTZ=0;
    exact.gd( v2t ,yCoord,numberOfDimensions,rectangularForTZ,1,0,0,0,If1,If2,If3,v2c,t);
    exact.gd( v2yy,yCoord,numberOfDimensions,rectangularForTZ,0,2,0,0,If1,If2,If3,v2c,t); // note - use "x-derivatives"


    // ::display(uft(If,v2c),"nu*v2_yy - computed","%4.2f ");
    // ::display(nu*v2yy    ,"nu*v2_yy - exact   ","%4.2f ");
    
    uft(If,v2c) += v2t - nu*v2yy;

  }
  

/* ---
  ust(0,us2c) = us(0,vs2c);
  ust(0,vs2c) =  (1./rhoe*)*( (-ke + te*kHat*kKat)*us(0,vs2c) + u(n1b,pc) - 2.*mu*v2y );
  
  ut(I1,v2c) = (-1./rho)*(  -py(I1) + mu*( (-kHat*kHat)*u(I1,v2c) + v2yy(I1)  );
  
  --- */


  return 0;
}



// =================================================================================================
/// \brief assign boundary conditions.
// =================================================================================================
int TravelingWaveFsi::
applyBoundaryConditions( RealArray & us, RealArray & uf, const real t )
{
  const aString & pde = dbase.get<aString>("pde");

  const int & numberOfSolidComponents = dbase.get<int>("numberOfSolidComponents");
  const int & numberOfFluidComponents = dbase.get<int>("numberOfFluidComponents");

  const int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  const int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  Range Is = numberOfSolidGridPoints;
  Range If = numberOfFluidGridPoints;

  // -- components:
  const int ec = dbase.get<int>("ec");
  const int etc = dbase.get<int>("etc");

  const int v1c = dbase.get<int>("v1fc");
  const int v2c = dbase.get<int>("v2fc");
  const int pc  = dbase.get<int>("pfc");

  const int & orderOfAccuracyInSpace = dbase.get<int>("orderOfAccuracyInSpace");
  const real & dy =  dbase.get<real>("dy");

  const real & rho = dbase.get<real>("rho");
  const real & mu  = dbase.get<real>("mu");
  const real nu = mu/rho;

  if( pde=="InsShell" )
  {

    const int i1a=If.getBase(), i1b=If.getBound();
    
    // dirichlet BC's for v2=g:
    real ga=0., gb=0.;
    
    // v2_y = gy :
    real gya=0., gyb=0.;

    const bool & twilightZone = dbase.get<bool>("twilightZone");
    if( twilightZone )
    {
      OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");
      RealArray & yCoord = dbase.get<RealArray>("yCoord");

      real v2e = exact(yCoord(i1a),0.,0.,v2c,t );
      real v2ye = exact.x(yCoord(i1a),0.,0.,v2c,t );  // note: use "x" derivative 
      ga += v2e;
      gya += v2ye;
      
      v2e = exact(yCoord(i1b),0.,0.,v2c,t );
      v2ye = exact.x(yCoord(i1b),0.,0.,v2c,t );      // note: use "x" derivative 

      gb  += v2e;
      gyb += v2ye;

    }

    uf(i1a,v2c)=ga;
    uf(i1b,v2c)=gb;

    if( mu>0. )
    {
      // v2_y=0 at no-slip walls since v1=0
      if( orderOfAccuracyInSpace==2 )
      {
	uf(i1a-1,v2c) = uf(i1a+1,v2c)- gya*(2.*dy);
	uf(i1b+1,v2c) = uf(i1b-1,v2c)+ gyb*(2.*dy);
      }
      else if( orderOfAccuracyInSpace==4 )
      {
//       d14(kd) = 1./(12.*dr(kd))
//         ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+
//      & 2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)

	// We set v2_y=0 and extrap to order 5:
	//  v(-2) -8*v(-1) = -8*v(1) + v(+2) + 12*dy*gya         = f1
	//  v(-2) -5*v(-1) = - 10*v(0) + 10*v(1) -5*v(2) + v(3)  = f2
      
	// side=0 : 
	real f1 = -8.*uf(i1a+1,v2c)+uf(i1a+2,v2c) + 12.*dy*gya;
	real f2 = -10.*uf(i1a,v2c) +10.*uf(i1a+1,v2c) -5.*uf(i1a+2,v2c) + uf(i1a+3,v2c);
      
	uf(i1a-1,v2c) = (f2-f1)/3.;
	uf(i1a-2,v2c) = (8.*f2-5.*f1)/3.;
    
     
	// side=1 : 
	f1 = -8.*uf(i1b-1,v2c)+uf(i1b-2,v2c) - 12.*dy*gyb;
	f2 = -10.*uf(i1b,v2c) +10.*uf(i1b-1,v2c) -5.*uf(i1b-2,v2c) + uf(i1b-3,v2c);
      
	uf(i1b+1,v2c) = (f2-f1)/3.;
	uf(i1b+2,v2c) = (8.*f2-5.*f1)/3.;
    
      }
      else
      {
	OV_ABORT("error");
      }
    }
    else
    {
      // -- mu=0 : inviscid : extrpolate
      if( orderOfAccuracyInSpace==2 )
      {
	uf(i1a-1,v2c) = 3.*uf(i1a,v2c)-3.*uf(i1a+1,v2c)+uf(i1a+2,v2c);
	uf(i1b+1,v2c) = 3.*uf(i1b,v2c)-3.*uf(i1b-1,v2c)+uf(i1b-2,v2c);
      }
      else if( orderOfAccuracyInSpace==4 )
      {
	uf(i1a-1,v2c) = 5.*uf(i1a,v2c)-10.*uf(i1a+1,v2c)+10.*uf(i1a+2,v2c)-5.*uf(i1a+3,v2c)+uf(i1a+4,v2c);
	uf(i1b+1,v2c) = 5.*uf(i1b,v2c)-10.*uf(i1b-1,v2c)+10.*uf(i1b-2,v2c)-5.*uf(i1b-3,v2c)+uf(i1b-4,v2c);
      }
	    
    }
    

    

   // printF("FINISH ME");

  }
  else
  {
    OV_ABORT("ERROR");
  }

  return 0;
}




// =================================================================================================
/// \brief Determine the maximum errors.
// =================================================================================================
int TravelingWaveFsi::
computeErrors( real t )
{
  
  const bool & twilightZone = dbase.get<bool>("twilightZone");
  if( !twilightZone )
  {
    return 0;
  }
  


  const aString & pde = dbase.get<aString>("pde");
  const aString & exactSolution = dbase.get<aString>("exactSolution");

  const int & twilightZoneOption = dbase.get<int>("twilightZoneOption");

  const int & numberOfSolidComponents = dbase.get<int>("numberOfSolidComponents");
  const int & numberOfFluidComponents = dbase.get<int>("numberOfFluidComponents");

  const int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  const int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  Range Is = numberOfSolidGridPoints;
  Range If = numberOfFluidGridPoints;

  const real & cfl = dbase.get<real>("cfl");
  const real & dt = dbase.get<real>("dt");

  const real & rhoe = dbase.get<real>("rhoe");

  const real & rho = dbase.get<real>("rho");
  const real & mu  = dbase.get<real>("mu");

  const real & ke = dbase.get<real>("ke");
  const real & te = dbase.get<real>("te");
  const real & be = dbase.get<real>("be");
  const real & ad = dbase.get<real>("ad");

  // components:
  const int ec = dbase.get<int>("ec");
  const int etc = dbase.get<int>("etc");
  const int v1c = dbase.get<int>("v1fc");
  const int v2c = dbase.get<int>("v2fc");
  const int pc  = dbase.get<int>("pfc");

  assert( numberOfSolidComponents==2 );
  assert( numberOfFluidComponents==3 );
  

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  int & current = dbase.get<int>("current");

  RealArray & time = dbase.get<RealArray>("time");
  real tc = time(current);
  assert( tc==t );

  RealArray *& usa = dbase.get<RealArray*>("usa");
  RealArray *& ufa = dbase.get<RealArray*>("ufa");

  RealArray & us = usa[current];
  RealArray & uf = ufa[current];

  Range Js=us.dimension(0);
  Range Jf=uf.dimension(0);
  

  // -- Compute errors ---
  if( pde=="InsShell" )
  {


    const real & amp = dbase.get<real>("amp");
    const real & omega = dbase.get<real>("omega");

    real etae  = amp*sin(omega*t);
    real etate = amp*omega*cos(omega*t);

    const int & degreeInTime = dbase.get<int>("degreeInTime");
    real ue = pow(t,degreeInTime); // solution is t^p 

    real maxErrSolid[2]={0.,0.}, maxErrFluid[3]={0.,0.,0.};
    
    maxErrSolid[ec] = max(maxErrSolid[ec], max(fabs(us(Is,ec)-etae)));
    maxErrSolid[etc] = max(maxErrSolid[etc], max(fabs(us(Is,etc)-etate)));

    maxErrFluid[v1c] = max(maxErrFluid[v1c], max(fabs(uf(If,v1c)-ue)));
    // maxErrFluid[v2c] = max(maxErrFluid[v2c], max(fabs(uf(If,v2c)-ue)));
    maxErrFluid[pc]  = max(maxErrFluid[pc] , max(fabs(uf(If,pc )-ue)));

    if( twilightZone )
    {
      // --- Compute errors for TZ ---
      OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");
      RealArray & yCoord = dbase.get<RealArray>("yCoord");

      Index If1=If, If2=0, If3=0;
      RealArray v2e(If1,1,1);
      //traction.display();
      int numberOfDimensions=1, rectangularForTZ=0;
      exact.gd( v2e,yCoord,numberOfDimensions,rectangularForTZ,0,0,0,0,If1,If2,If3,v2c,t);  // v2 exact 

      // ::display(v2e,"computeErrors v2e","%g ");
      // ::display(uf(If1,v2c),"computeErrors uf(v2c)","%g ");

      maxErrFluid[v2c] = max(maxErrFluid[v2c], max(fabs(uf(If1,v2c)-v2e)));

    }


    printF("TravelingWaveFsi:: errors at t=%9.3e (dt=%8.2e) : eta=%8.2e, etat=%8.2e, v1=%8.2e, v2=%8.2e, p=%8.2e\n",t,
	   dt,maxErrSolid[ec],maxErrSolid[etc],maxErrFluid[v1c],maxErrFluid[v2c], maxErrFluid[pc]);


  }
  else
  {
    OV_ABORT("error");
  }



  return 0;
}



// =================================================================================================
/// \brief Plot the solution.
// =================================================================================================
int TravelingWaveFsi::
plot( GenericGraphicsInterface & gi, realCompositeGridFunction & uPlot, GraphicsParameters & psp )
{

  // components:
  const int ec = dbase.get<int>("ec");
  const int etc = dbase.get<int>("etc");
  const int v1c = dbase.get<int>("v1fc");
  const int v2c = dbase.get<int>("v2fc");
  const int pc  = dbase.get<int>("pfc");

  CompositeGrid & cg = *uPlot.getCompositeGrid();
  MappedGrid & mg = cg[0];
  
  OV_GET_SERIAL_ARRAY(real,uPlot[0],uLocal);
  OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);

  const int & numberOfSolidGridPoints = dbase.get<int>("numberOfSolidGridPoints");
  const int & numberOfFluidGridPoints = dbase.get<int>("numberOfFluidGridPoints");

  int & current = dbase.get<int>("current");

  RealArray & time = dbase.get<RealArray>("time");
  const real t = time(current);

  RealArray *& usa = dbase.get<RealArray*>("usa");
  RealArray *& fsa = dbase.get<RealArray*>("fsa");

  RealArray *& ufa = dbase.get<RealArray*>("ufa");
  RealArray *& ffa = dbase.get<RealArray*>("ffa");

  RealArray & us = usa[current];
  RealArray & uf = ufa[current];

  const real & x0 = dbase.get<real>("x0"); // phase in x
  const real & t0 = dbase.get<real>("t0"); // phase in t
  const real & length = dbase.get<real>("length");
  const real & kx = dbase.get<real>("kx");
  const real kxHat = twoPi*kx/length;

  Index I1,I2,I3;
  int numGhost=2;
  getIndex(mg.gridIndexRange(),I1,I2,I3,numGhost);

  assert( I2.getLength()==(numberOfFluidGridPoints+2*numGhost) );
  
  const bool & twilightZone = dbase.get<bool>("twilightZone");
  RealArray v2e;  // holds exact solution for TZ
  if( twilightZone )
  {
    OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");
    RealArray & yCoord = dbase.get<RealArray>("yCoord");

    Index If1=I1, If2=0, If3=0;
    v2e.redim(If1,1,1);
    //traction.display();
    int numberOfDimensions=1, rectangularForTZ=0;
    exact.gd( v2e,yCoord,numberOfDimensions,rectangularForTZ,0,0,0,0,If1,If2,If3,v2c,t);  // v2 exact 
  }
  

  for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
  for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
  for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
  {
    uLocal(i1,i2,i3,pc)  = 0.;
    uLocal(i1,i2,i3,v1c) = 0.;
    uLocal(i1,i2,i3,v2c) = sin(kxHat*xLocal(i1,i2,i3,0)+x0)*uf(i2,v2c);
    
    if( twilightZone )
    {
      // plot errors
      uLocal(i1,i2,i3,pc+3)  = 0.;
      uLocal(i1,i2,i3,v1c+3) = 0.;  
      uLocal(i1,i2,i3,v2c+3) = sin(kxHat*xLocal(i1,i2,i3,0)+x0)*(uf(i2,v2c)-v2e(i2));
    }
    
  }
  
  aString buff;
  psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at time %9.2e",t));

  // gi.erase();
  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
  PlotIt::contour(gi,uPlot,psp);
  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  const int grid=0, side=1, axis=1;  // do this for now  *FIX ME*
  Index Ib1,Ib2,Ib3;
  getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3,numGhost);

  Range Rx=2;
  RealArray x(Ib1,Ib2,Ib3,Rx);

  x(Ib1,Ib2,Ib3,0)=xLocal(Ib1,Ib2,Ib3,0);
  x(Ib1,Ib2,Ib3,1)=xLocal(Ib1,Ib2,Ib3,1) + sin(kxHat*xLocal(Ib1,Ib2,Ib3,0)+x0)*us(0,ec);  // surface height 
 
  x.reshape(Ib1.length()*Ib2.length()*Ib3.length(),Rx);

  // printF("ElasticShell::plot: current=%i\n",current);
  // ::display(x1(Ib1,Ib2,Ib3,Rx),"x1","%6.1f ");

  NurbsMapping map;
  int option=0, degree=3;
  map.interpolate(x,option,Overture::nullRealArray(),degree,NurbsMapping::parameterizeByChordLength,numGhost);

  PlotIt::plot(gi,map,psp);


  return 0;
}


// =================================================================================================
/// \brief Set parameters for the elastic solid
// =================================================================================================
int TravelingWaveFsi::
update( GenericGraphicsInterface & gi )
{

  int & debug = dbase.get<int>("debug");
  aString & pde = dbase.get<aString>("pde"); // pde we are solving
  aString & initialConditions = dbase.get<aString>("initialConditions");
  aString & exactSolution = dbase.get<aString>("exactSolution");
  
  aString & timeSteppingMethod = dbase.get<aString>("timeSteppingMethod");
  bool & twilightZone = dbase.get<bool>("twilightZone");
  int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  real *trigFreq = dbase.get<real[4]>("trigFreq");
  bool & normalMotionOnly = dbase.get<bool>("normalMotionOnly");
  bool & standingWaveSolution = dbase.get<bool>("standingWaveSolution");

  int & degreeInTime = dbase.get<int>("degreeInTime");
  int & degreeInSpace = dbase.get<int>("degreeInSpace");
  int & orderOfAccuracyInSpace = dbase.get<int>("orderOfAccuracyInSpace");
  int & orderOfAccuracyInTime = dbase.get<int>("orderOfAccuracyInTime");
  int & numberOfGhostLinesInError = dbase.get<int>("numberOfGhostLinesInError");

  real & cfl = dbase.get<real>("cfl");
  real & rhoe = dbase.get<real>("rhoe");
  real & lambdae = dbase.get<real>("lambdae");
  real & mue = dbase.get<real>("mue");

  real & rho = dbase.get<real>("rho");
  real & mu  = dbase.get<real>("mu");

  real & ke = dbase.get<real>("ke");
  real & te = dbase.get<real>("te");
  real & be = dbase.get<real>("be");
  real & ad = dbase.get<real>("ad");

  real & kx = dbase.get<real>("kx");
  real & height = dbase.get<real>("height");
  real & length = dbase.get<real>("length");

  real & Hs = dbase.get<real>("Hs"); // solid height

  real & amp = dbase.get<real>("amp");
  real & x0  = dbase.get<real>("x0");
  real & t0  = dbase.get<real>("t0");

  // // Traveling wave parameters
  // real *fwp =  dbase.get<real[10]>("TravelingWaveParameters");

//   // -- Boundary conditions ---
//   if( !dbase.has_key("boundaryCondition") )
//   {
//     dbase.put<BcArray>("boundaryCondition");
//     BcArray & boundaryCondition = dbase.get<BcArray>("boundaryCondition");
//     for( int side=0; side<=1; side++ )for( int axis=0; axis<2; axis++ )
//     {
//       boundaryCondition(side,axis)=dirichletBoundaryCondition;
//     }
//   }
//   BcArray & boundaryCondition = dbase.get<BcArray>("boundaryCondition");

  GUIState dialog;
  dialog.setWindowTitle("Traveling Wave Solution");
  dialog.setExitCommand("exit", "exit");

  dialog.setOptionMenuColumns(1);

  aString pdeNames[] = {"InsShell",
                        "InsAcousticSolid",
			"InsElasticSolid",
			""};
  int n=0;
  while( pdeNames[n]!=pde && pdeNames[n+1]!="" ){ n++; } // look for current choice

  const int maxOptCommands=21;
  assert( n<maxOptCommands );
  aString optionCmd[maxOptCommands];
  GUIState::addPrefix(pdeNames,"PDE: ",optionCmd,maxOptCommands);
  dialog.addOptionMenu( "PDE:", optionCmd, pdeNames, n );

  aString timeSteppingNames[] = {"Runge-Kutta",
				 ""};
  n=0;
  while( timeSteppingNames[n]!=timeSteppingMethod && timeSteppingNames[n+1]!="" ){ n++; } // find current choice
  assert( n<maxOptCommands );

  GUIState::addPrefix(timeSteppingNames,"Time-stepping: ",optionCmd,maxOptCommands);
  dialog.addOptionMenu( "Time-stepping:", optionCmd, timeSteppingNames, n );

  aString twilightZoneOptions[] = {"polynomial",
				   "trigonometric",
				   ""};
  GUIState::addPrefix(twilightZoneOptions,"Twilight-zone: ",optionCmd,maxOptCommands);
  dialog.addOptionMenu( "Twilight-zone:", optionCmd, twilightZoneOptions, (int)twilightZoneOption );

  // ---- initial condition options ----
  aString initialConditionNames[] = {"twilightZone",
				     "exactSolution",
                                     "zero",
				     ""};
  n=0;
  while( initialConditionNames[n]!=initialConditions && initialConditionNames[n+1]!="" ){ n++; } 
  assert( n<maxOptCommands );
  GUIState::addPrefix(initialConditionNames,"Initial conditions: ",optionCmd,maxOptCommands);
  dialog.addOptionMenu( "Initial conditions:", optionCmd, initialConditionNames, n );

  // ---- exact solution options ----
  aString exactSolutionNames[] = {"none",
                                  "twilightZone",
				  ""};
  n=0;
  while( exactSolutionNames[n]!=exactSolution && exactSolutionNames[n+1]!="" ){ n++; } 
  assert( n<maxOptCommands );
  GUIState::addPrefix(exactSolutionNames,"Exact solution: ",optionCmd,maxOptCommands);
  dialog.addOptionMenu( "Exact solution:", optionCmd, exactSolutionNames, n );


//   const aString bcNames[] = { "periodic",
// 			      "Dirichlet",
// 			      "Neumann",
//                               "slide",
// 			      "ElasticWall",
// 			      "" };  
//   const int maxCommands=numberOfBoundaryConditions+1;
//   aString bcCmd[maxCommands];

//   GUIState::addPrefix(bcNames,"BC left: ",bcCmd,maxCommands);
//   dialog.addOptionMenu("BC left:",bcCmd,bcNames,boundaryCondition(0,0) );

//   GUIState::addPrefix(bcNames,"BC right: ",bcCmd,maxCommands);
//   dialog.addOptionMenu("BC right:",bcCmd,bcNames,boundaryCondition(1,0) );

//   GUIState::addPrefix(bcNames,"BC bottom: ",bcCmd,maxCommands);
//   dialog.addOptionMenu("BC bottom:",bcCmd,bcNames,boundaryCondition(0,1) );

//   GUIState::addPrefix(bcNames,"BC top: ",bcCmd,maxCommands);
//   dialog.addOptionMenu("BC top:",bcCmd,bcNames,boundaryCondition(1,1) );


  aString tbCommands[] = {"normal motion only",
                          "standing wave solution",
                          "twilight-zone",
                          ""};
  int tbState[10];
  tbState[0] = normalMotionOnly;
  tbState[1] = standingWaveSolution;
  tbState[2] = twilightZone;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


//   aString pbCommands[] = {"show file options...",
//                           "create a probe",
//                           "output periodically to a file",
// 			  ""};
//   aString *pbLabels = pbCommands;
//   int numRows=2;
//   dialog.setPushButtons( pbCommands, pbLabels, numRows ); 

  // ----- Text strings ------
  const int numberOfTextStrings=40;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "cfl:";  sPrintF(textStrings[nt],"%g",cfl); nt++;
//  textLabels[nt] = "Traveling wave:";  sPrintF(textStrings[nt],"%g, %g, %g, %g, %g (amp,kx,ky,x0,t0)",fwp[0],fwp[1],fwp[2],fwp[3],fwp[4]); nt++;
// 
  textLabels[nt] = "height:";  sPrintF(textStrings[nt],"%g",height); nt++;
  textLabels[nt] = "length:";  sPrintF(textStrings[nt],"%g",length); nt++;
  textLabels[nt] = "kx:";  sPrintF(textStrings[nt],"%g",kx); nt++;
  textLabels[nt] = "amp, x0, t0:";  sPrintF(textStrings[nt],"%g, %g, %g",amp,x0,t0); nt++;

  textLabels[nt] = "elastic shell density:";  sPrintF(textStrings[nt],"%g",rhoe); nt++;
  textLabels[nt] = "elastic shell tension:";  sPrintF(textStrings[nt],"%g",te); nt++;
  textLabels[nt] = "elastic shell stiffness:";  sPrintF(textStrings[nt],"%g",ke); nt++;
  textLabels[nt] = "elastic shell damping:";  sPrintF(textStrings[nt],"%g",be); nt++;
  textLabels[nt] = "elastic shell artificial dissipation:";  sPrintF(textStrings[nt],"%g",ad); nt++;
//textLabels[nt] = "volume penalty parameter:";  sPrintF(textStrings[nt],"%g",volumePenalty); nt++;
//
  textLabels[nt] = "elastic solid density:";  sPrintF(textStrings[nt],"%g",rhoe); nt++;
  textLabels[nt] = "elastic solid lambda:";  sPrintF(textStrings[nt],"%g",lambdae); nt++;
  textLabels[nt] = "elastic solid mu:";  sPrintF(textStrings[nt],"%g",mue); nt++;
  textLabels[nt] = "elastic solid height:";  sPrintF(textStrings[nt],"%g",Hs); nt++;

  textLabels[nt] = "fluid density:";  sPrintF(textStrings[nt],"%g",rho); nt++;
  textLabels[nt] = "fluid viscosity:";  sPrintF(textStrings[nt],"%g",mu); nt++;
  textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",debug); nt++;
  textLabels[nt] = "numberOfGhostLinesInError:";  sPrintF(textStrings[nt],"%i",numberOfGhostLinesInError); nt++;
  textLabels[nt] = "order of accuracy in space:";  sPrintF(textStrings[nt],"%i",orderOfAccuracyInSpace); nt++;
  textLabels[nt] = "order of accuracy in time:";  sPrintF(textStrings[nt],"%i",orderOfAccuracyInTime); nt++;
  textLabels[nt] = "degree in space:";  sPrintF(textStrings[nt],"%i",degreeInSpace);  nt++; 
  textLabels[nt] = "degree in time:";  sPrintF(textStrings[nt],"%i",degreeInTime);  nt++; 
  textLabels[nt] = "trig frequencies:";  sPrintF(textStrings[nt],"%g, %g, %g, %g (ft,fx,fy,fz)",
						 trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]); nt++;

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(dialog);
  gi.appendToTheDefaultPrompt("ESP>");


  int len=0;
  aString answer,line,buff;
  for( ;; )
  {

    gi.getAnswer(answer,"");  
 
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( dialog.getToggleValue(answer,"twilight-zone",twilightZone) ){}//
    else if( dialog.getToggleValue(answer,"normal motion only",normalMotionOnly) ){}//
    else if( dialog.getToggleValue(answer,"standing wave solution",standingWaveSolution) ){}//

    else if( dialog.getTextValue(answer,"cfl:","%g",cfl) ){} // 
    else if( dialog.getTextValue(answer,"height:","%g",height) ){} // 
    else if( dialog.getTextValue(answer,"length:","%g",length) ){} // 
    else if( dialog.getTextValue(answer,"kx:","%g",kx) ){} // 

    else if( dialog.getTextValue(answer,"elastic shell density:","%g",rhoe) ){} // 
    else if( dialog.getTextValue(answer,"elastic shell tension:","%g",te) ){}// 
    else if( dialog.getTextValue(answer,"elastic shell stiffness:","%g",ke) ){}// 
    else if( dialog.getTextValue(answer,"elastic shell damping:","%g",be) ){}// 
    else if( dialog.getTextValue(answer,"elastic shell artificial dissipation:","%g",ad) ){}// 


    else if( dialog.getTextValue(answer,"elastic solid density:","%g",rhoe) ){} // 
    else if( dialog.getTextValue(answer,"elastic solid lambda:","%g",lambdae) ){}// 
    else if( dialog.getTextValue(answer,"elastic solid mu:","%g",mue) ){}// 
    else if( dialog.getTextValue(answer,"elastic solid height:","%g",Hs) ){}// 

    else if( dialog.getTextValue(answer,"fluid density:","%g",rho) ){}// 
    else if( dialog.getTextValue(answer,"fluid viscosity:","%g",mu) ){}// 

    else if( dialog.getTextValue(answer,"degree in space:","%i",degreeInSpace) ){} //
    else if( dialog.getTextValue(answer,"degree in time:","%i",degreeInTime) ){} //
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){}// 
    else if( dialog.getTextValue(answer,"order of accuracy in space:","%i",orderOfAccuracyInSpace) ){}// 
    else if( dialog.getTextValue(answer,"order of accuracy in time:","%i",orderOfAccuracyInTime) ){}// 
    else if( dialog.getTextValue(answer,"numberOfGhostLinesInError:","%i",numberOfGhostLinesInError) )
    {
      printF("Setting: numberOfGhostLinesInError=%i : this many ghost lines will be included when computing errors\n",
             numberOfGhostLinesInError);
    }

    else if( len=answer.matches("amp, x0, t0:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&amp,&x0,&t0);
      printF("Setting amp=%g, x0=%g, t0=%g\n",amp,x0,t0);
    }
    

    else if( len=answer.matches("trig frequencies:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&trigFreq[0],&trigFreq[1],&trigFreq[2],&trigFreq[3]);
      printF("Setting trigonometric TZ frequencies to ft=%g, fx=%g, fy=%g, fz=%g.\n",
              trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]);

      dialog.setTextLabel("trig frequencies:",sPrintF(textStrings[nt],"%g, %g, %g, %g (ft,fx,fy,fz)",
                                                      trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]));
    }

    // else if( len=answer.matches("Traveling wave:") )
    // {
    //   sScanF(answer(len,answer.length()-1),"%e %e %e %e %e",&fwp[0],&fwp[1],&fwp[2],&fwp[3],&fwp[4]);
    //   // ** FIX ME **
    //   printF("**FIX ME* The Traveling wave solution is:\n",
    //          " solid: us(x,y,t) = amp sin(kxHat*x+x0) [ a sin(kyHat(y-H)) + b cos(kyHat(y-H)) ] cos(omegaHat*t+t0)\n"
    //          "           where: kxHat = 2*pi*kx/L, kyHat=2*pi*ky\n, omegaHat = 2*pi*omega\n" );
      
    //   printF("Setting Traveling-wave parameters to amp=%g, kx=%g, ky=%g, x0=%g, t0=%g\n",
    //           fwp[0],fwp[1],fwp[2],fwp[3],fwp[4]);

    //   dialog.setTextLabel("Traveling wave:",sPrintF("%g, %g, %g, %g, %g (amp,kx,ky,x0,t0)",
    //                       fwp[0],fwp[1],fwp[2],fwp[3],fwp[4]));
    // }
    else if( len=answer.matches("PDE: ") )
    {
      aString name=answer(len,answer.length()-1);
      
      if( name!="InsShell" && name!="InsAcousticSolid" &&
          name!="InsElasticSolid" )
      {
	printF("ERROR: invalid PDE, name=[%s].\n",(const char*)name);
	gi.stopReadingCommandFile();
      }
      else
      {
	pde=name;
	printF("Setting the PDE to to [%s].\n",(const char*)pde);
      }
      dialog.getOptionMenu("PDE:").setCurrentChoice(pde);
      
    }
    else if( len=answer.matches("Twilight-zone: ") )
    {
      aString name=answer(len,answer.length()-1);
      if( name=="polynomial" )
      {
	twilightZoneOption=polynomial;
      }
      else if( name=="trigonometric" )
      {
	twilightZoneOption=trigonometric;
      }
      else
      {
	printF("Error: unexpected TZ=[%s]\n",(const char*)name);
	gi.stopReadingCommandFile();
        continue;
      }
      dialog.getOptionMenu("Twilight-zone:").setCurrentChoice(name);
    }

    else if( len=answer.matches("Time-stepping: ") )
    {
      aString name=answer(len,answer.length()-1);
      
      if( name!="Runge-Kutta" )
      {
	printF("ElasticSolid::update:ERROR: invalid time-stepping name=[%s].\n",(const char*)name);
	gi.stopReadingCommandFile();
      }
      else
      {
	timeSteppingMethod=name;
	printF("Setting the time-stepping method to [%s].\n",(const char*)timeSteppingMethod);
      }
      dialog.getOptionMenu("Time-stepping:").setCurrentChoice(name);
    }
    else if( len=answer.matches("Initial conditions: ") )
    {
      aString name=answer(len,answer.length()-1);
      
      initialConditions=name;
      printF("Setting initialConditions to [%s].\n",(const char*)initialConditions);

      dialog.getOptionMenu("Initial conditions:").setCurrentChoice(name);
    }
    else if( len=answer.matches("Exact solution: ") )
    {
      aString name=answer(len,answer.length()-1);
      
      exactSolution=name;
      printF("Setting the exact solution to be [%s].\n",(const char*)exactSolution);

      dialog.getOptionMenu("Exact solution:").setCurrentChoice(name);
    }

//     else if( answer.matches("BC left: ") ||
// 	     answer.matches("BC right: ") ||
// 	     answer.matches("BC bottom: ") ||
// 	     answer.matches("BC top: ") )
//     {
//       int side=0, axis=0;
//       if( len=answer.matches("BC left: ") )
//       {
// 	side=0; axis=0;
//       }
//       else if( len=answer.matches("BC right: ") )
//       {
// 	side=1; axis=0;
//       }
//       else if( len=answer.matches("BC bottom: ") )
//       {
// 	side=0; axis=1;
//       }
//       else if( len=answer.matches("BC top: ") )
//       {
// 	side=1; axis=1;
//       }
//       else
//       {
// 	OV_ABORT("ERROR: unexpected answer!");
//       }
    

//       const aString bcNames[] = { "periodic",
// 				  "Dirichlet",
// 				  "Neumann",
// 				  "slide",
// 				  "ElasticWall",
// 				  "" };  

//       line = answer(len,answer.length()-1);
//       for( int bc=0; bc<numberOfBoundaryConditions; bc++ )
//       {
// 	if( line==bcNames[bc] )
// 	{
// 	  printF("Setting boundaryCondition(%i,%i)=%i (%s).\n",side,axis,bc,(const char*)line);
// 	  boundaryCondition(side,axis)=bc;
// 	}
//       }
    
//     }

    else
    {
      printF("ERROR: unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

  }

  gi.unAppendTheDefaultPrompt();
  gi.popGUI(); // restore the previous GUI

  return 0;

}


