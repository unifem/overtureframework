#include "InsParameters.h"
#include "FlowSolutions.h"
#include "GenericGraphicsInterface.h"
#include "FluidPiston.h"
#include "PistonMotion.h"
#include "ParallelUtility.h"
#include "DeformingBodyMotion.h"
#include "RigidBodyMotion.h"
#include "BeamModel.h"
#include "BoundaryLayerProfile.h"
#include "TimeFunction.h"

#define rotatingDiskSVK EXTERN_C_NAME(rotatingdisksvk)
#define zeroin EXTERN_C_NAME(zeroin)
#define cBesselJ EXTERN_C_NAME(cbesselj)
#define cBesselY EXTERN_C_NAME(cbessely)
#define evalOscillatingBubble EXTERN_C_NAME(evaloscillatingbubble)
#define evalCapillaryFlow EXTERN_C_NAME(evalcapillaryflow)
#define evalFibShearFluid EXTERN_C_NAME(evalfibshearfluid)

extern "C"
{
  // rotating disk (SVK) exact solution:
  void rotatingDiskSVK( const real & t, const int & numberOfGridPoints, real & uDisk, real & param,
			const int & nrwk, real & rwk );

  void cBesselJ( const real& nu, const real&zr, const real &zi, real &jr,real &ji );
  void cBesselY( const real& nu, const real&zr, const real &zi, real &yr,real &yi );
  void evalOscillatingBubble(const real& r, const real&R, 
			     const int& n, const real& mu,
			     const real& lr, const real& li,
			     const real& cr, const real& ci,
			     const real& dr, const real& di,
			     real& vrr, real& vri,
			     real& vtr, real& vti,
			     real& pr, real& pi);
  void evalCapillaryFlow(const real& k, const real& y, const real& mu, 
			 const real& alphar, const real& alphai, 
			 const real& ar, const real& ai, const real& br, const real& bi,
			 const real& cr, const real& ci, const real& dr, const real& di,
			 real& uhr, real& uhi, real& vhr, real& vhi, real& phr, real& phi);

  // exact fsi solution for shear flow
  void evalFibShearFluid( const real & kfr, const real & kfi,
			  const real & omegar, const real & omegai,
			  const real & cr, const real & ci,
			  const real & dr, const real & di,
			  const real & y, 
			  real & vr, real & vi);
}

namespace
{
// These are for passing to the rotating disk function
real innerRadius, outerRadius, inertiaTerm, massTerm;
}


extern "C"
{
// zeroin: function to compute the zero of a function on an interval -- f is assumed to change sign
real zeroin(real & ax,real & bx, real (*f) (const real&), real & tol);

// A positive zero of this function defines the parameter "k" in the rotating disk FSI exact solution
real rotatingDiskFunction(const real & k )
{ 
   
  real a=innerRadius, b=outerRadius;

  const int n=1;
  // jn(n,x) = Bessel function J of order n
  // yn(n,x) = Bessel function Y of order n
  real Ja = jn(n,k*a);  
  real Jb = jn(n,k*b);  
  real Jap = jn(0,k*a)-Ja/(k*a); // J1' = J0 - J1/x 
   

  real Ya = yn(n,k*a); 
  real Yb = yn(n,k*b); 
  real Yap = yn(0,k*a)-Ya/(k*a); // Y1' = Y0 - Y1/x 

  real Ra = Ja*Yb-Jb*Ya;
  real Rap = Jap*Yb - Jb*Yap; // R' 
   
  real value = k*Rap - (1./a - inertiaTerm*k*k)*Ra;  // look for a zero of this function

  return value;
   
}

// A positive zero of this function defines the parameter "k" in the shear-block FSI solution
real shearBlockFunction(const real & kH )
{ 
  // massTerm = bodyMass/(fluidDensity*height*length);   
  real value = tan(kH)+massTerm*kH;

  return value;
   
}

// A positive zero of this function defines the parameter "k" in the translating disk FSI exact solution
// See stokesRB-notes.pdf by DWS. 
real translatingDiskFunction(const real & k )
{ 
   
  real delta=massTerm; // massBody/(rho*Pi*a^2)

  real a=innerRadius, b=outerRadius;
  real alam=k, alam2=alam*alam;
  real za=alam*a, za2=za*za;
  real zb=alam*b;
 
  real j0a =jn(0,za),         y0a =yn(0,za);
  real j1a =jn(1,za),         y1a =yn(1,za);
  real j1pa=j0a-j1a/za,       y1pa=y0a-y1a/za;

  real j1ppa = -j1a - j0a/za + 2.*j1a/(za*za);   // J1''
  real y1ppa = -y1a - y0a/za + 2.*y1a/(za*za);   // Y1''
   

  real j0b=jn(0,zb),          y0b=yn(0,zb);
  real j1b=jn(1,zb),          y1b=yn(1,zb);

  real a11=(Pi*b/(2*a))*(2*y1b-zb*y0b),   a12=(Pi*a/(2*b))*zb*y0b;
  real a21=(Pi*b/(2*a))*(2*j1b-zb*j0b),   a22=(Pi*a/(2*b))*zb*j0b;

  real c11=(a11*j1a-a21*y1a+1)/za2,         c12=(a12*j1a-a22*y1a-1)/za2;
  real c21=((a11*j1pa-a21*y1pa)*za+1)/za2,  c22=((a12*j1pa-a22*y1pa)*za+1)/za2;

  real determ=c11*c22-c12*c21;
  real ahat=(c22-c12)/(determ*a*a);
  real bhat=(c11-c21)/(determ);

  real k1hat= (Pi*b/2)*((2*y1b-zb*y0b)*ahat/alam2+y0b*bhat/zb);
  real k2hat=-(Pi*b/2)*((2*j1b-zb*j0b)*ahat/alam2+j0b*bhat/zb);

  real coeff1 = -1 + a11*j1ppa - a21*y1ppa;
  real coeff2 = -1 + a12*j1ppa - a22*y1ppa - 2/za2;
  real value = delta*za2 + coeff1*ahat*a*a + coeff2*bhat; // look for a zero of this function
   
  return value;
   
}

}



#define FOR_3D(i1,i2,i3,I1,I2,I3)					\
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(); \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++)					\
  for(i2=I2Base; i2<=I2Bound; i2++)					\
    for(i1=I1Base; i1<=I1Bound; i1++)


// Macro to get the vertex array
#define GET_VERTEX_ARRAY(x)                                     \
mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);       \
OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);                  \
if( !thisProcessorHasPoints )                                   \
  return 0; // no points on this processor


int InsParameters::
getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, RealArray & ua, 
			    const Index & I1_, const Index &I2_, const Index &I3_, 
                            int numberOfTimeDerivatives /* = 0 */ )
// ==========================================================================================
///  \brief Evaluate a user defined known solution.
/// \param numberOfTimeDerivatives (input) : number of time derivatives to evaluate (0 means evaluate the solution).
// ==========================================================================================
{
  MappedGrid & mg = cg[grid];
  const int numberOfDimensions = mg.numberOfDimensions();
    
  // Adjust index bounds for parallel *wdh* 2017/05/31 
  Index I1=I1_, I2=I2_, I3=I3_;
  
  OV_GET_SERIAL_ARRAY_CONST(int,mg.mask(),mask);
  int includeGhost=1;
  bool thisProcessorHasPoints=ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);
  
  // printF("--INSPAR-- getUserDefinedKnownSolution: START---\n");

  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedKnownSolutionData") )
  {
    printf("getUserDefinedKnownSolution:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
    OV_ABORT("error");
  }
  DataBase & db =  dbase.get<DataBase >("modelData").get<DataBase>("userDefinedKnownSolutionData");

  const aString & userKnownSolution = db.get<aString>("userKnownSolution");

  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");
  
  const real nu = dbase.get<real>("nu");
  const int pc = dbase.get<int >("pc");
  const int uc = dbase.get<int >("uc");
  const int vc = dbase.get<int >("vc");
  const int wc = dbase.get<int >("wc");
  const real dt = dbase.get<real>("dt");
  
  if( userKnownSolution=="pipeFlow" )
  {
    // --- Circular pipe flow (Hagen-Poiseuille flow) ---
    // 	  u(r) = -(1/(4*nu)* dp/dx *( R^2 - r^2 )
    // 	  dp/dx = (pInflow-pOutflow)/length = const

    const real & pInflow  = rpar[0];
    const real & pOutflow = rpar[1];
    const real & radius   = rpar[2];
    const real & s0       = rpar[3];
    const real & length   = rpar[4];
    const real & Ua       = rpar[5];
    const real & Ub       = rpar[6];
    const int  & axialAxis= ipar[0];
    if( t<=dt )
      printF("circular pipe flow: pInflow=%g, pOutflow=%g, radius=%g, s0=%g, length=%g, ua=%g, ub=%g, axialAxis=%i, t=%9.3e\n",
	     pInflow,pOutflow,radius,s0,length,axialAxis,t);

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(x);

    const int axisp1 = (axialAxis+1)%3, axisp2=(axialAxis+2)%3;
    const int uca=uc+axialAxis, ucb=uc + (axialAxis+1)%3, ucc=uc + (axialAxis+2)%3;

    ua(I1,I2,I3,pc)=pInflow + ((pOutflow-pInflow)/length)*(x(I1,I2,I3,axialAxis)-s0);
    if( mg.numberOfDimensions()==2 )
    {
      // Poiseulle-Couette flow: 
      ua(I1,I2,I3,uca)=( (-(pOutflow-pInflow)/length/(2.*nu))*( radius*radius - SQR(x(I1,I2,I3,axisp1)) ) 
			 + Ua + ((Ub-Ua)/(2.*radius))*(x(I1,I2,I3,axisp1)+radius) );
      ua(I1,I2,I3,ucb)=0.;
    }
    else
    {
      // Hagen-Poiseuille flow: 
      ua(I1,I2,I3,uca)= (-(pOutflow-pInflow)/length/(4.*nu))*( radius*radius - SQR(x(I1,I2,I3,axisp1)) - SQR(x(I1,I2,I3,axisp2)) );
      ua(I1,I2,I3,ucb)=0.;
      ua(I1,I2,I3,ucc)=0.;
    }

  }
  else if( userKnownSolution=="rotatingCouetteflow" )
  {
    const real & rInner     = rpar[0];
    const real & rOuter     = rpar[1];
    const real & omegaInner = rpar[2];
    const real & omegaOuter = rpar[3];

    const int & axialAxis = ipar[3];
    
    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(x);

    // ua(I1,I2,I3,pc)=pInflow + ((pOutflow-pInflow)/length)*(x(I1,I2,I3,0)-x0);
    RealArray r(I1,I2,I3), uTheta(I1,I2,I3);
    
    const int axisp1 = numberOfDimensions==2 ? 0 : (axialAxis+1) % numberOfDimensions;
    const int axisp2 = numberOfDimensions==2 ? 1 : (axialAxis+2) % numberOfDimensions;

    if( t<=dt )
      printF("++++rotatingCouetteflow : axialAxis=%i axisp1=%i axisp2=%i \n",axialAxis,axisp1,axisp2);

    r = sqrt( SQR(x(I1,I2,I3,axisp1)) + SQR(x(I1,I2,I3,axisp2)) );

    const real r1=rInner, r2=rOuter, w1=omegaInner, w2=omegaOuter;
    const real A =-(w2-w1)*r1*r1*r2*r2/(r2*r2-r1*r1);
    const real B = (w2*r2*r2-w1*r1*r1)/(r2*r2-r1*r1);
  
    uTheta=A/r + B*r;

    // dp/dr = rho*uTheta^2/r 
    //       =  A^2/r^3 + 2*A*B/r + B^2*r 
    // thetaHat = (-sin(theta),cos(theta))
    ua(I1,I2,I3,uc+axisp1)=-uTheta*x(I1,I2,I3,axisp2)/r;
    ua(I1,I2,I3,uc+axisp2)= uTheta*x(I1,I2,I3,axisp1)/r;
    ua(I1,I2,I3,pc)= ( (-A*A/2.)*( 1./(SQR(r)) - 1./(SQR(r1)) ) + 
		       (2.*A*B)*( log(r)-log(r1) ) + (.5*B*B)*( SQR(r)-SQR(r1) ) );

    if( numberOfDimensions==3 )
      ua(I1,I2,I3,uc+axialAxis)=0.;
    
  }
  else if( userKnownSolution=="TaylorGreenVortex" )
  {
    const real & kp = rpar[0];
    const int & axialAxis = ipar[0];
//       printF("--- Taylor Green vortex is an exact solution ---\n"
//              " u = sin(k x) cos(k y) F(t)\n"
//              " v =-cos(k x) sin(k y) F(t)\n"
//              " p = (rho/4)*( cos(2 k x) + cos(2 k y) ) F(t)*F(t)\n"
//              "   where F(t) = exp( -2 nu k^2 t )\n");
    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(x);

    const real k= kp*twoPi;
    const real f = exp(-2.*nu*k*k*t);

    if( numberOfDimensions==2 )
    {
      ua(I1,I2,I3,uc) = sin(k*x(I1,I2,I3,0))*cos(k*x(I1,I2,I3,1))*f; 
      ua(I1,I2,I3,vc) =-cos(k*x(I1,I2,I3,0))*sin(k*x(I1,I2,I3,1))*f; 
      ua(I1,I2,I3,pc) = (f*f/4.)*( cos((2.*k)*x(I1,I2,I3,0)) + cos( (2.*k)*x(I1,I2,I3,1)) ); 
    }
    else  if( numberOfDimensions==3 )
    {
      // --- The 2D solution is cyclically permutted in 3D ---
      int axisp1 = (axialAxis+1) % 3;
      int axisp2 = (axialAxis+2) % 3;
    
      ua(I1,I2,I3,uc+axisp1) = sin(k*x(I1,I2,I3,axisp1))*cos(k*x(I1,I2,I3,axisp2))*f; 
      ua(I1,I2,I3,uc+axisp2) =-cos(k*x(I1,I2,I3,axisp1))*sin(k*x(I1,I2,I3,axisp2))*f; 
      ua(I1,I2,I3,uc+axialAxis)=0.;

      ua(I1,I2,I3,pc) = (f*f/4.)*( cos((2.*k)*x(I1,I2,I3,axisp1)) + cos( (2.*k)*x(I1,I2,I3,axisp2)) ); 

      
    }
    else
    {
      OV_ABORT("error");
    }

  }
  else if( userKnownSolution=="uniformFlowINS" )
  {
    // for testing with INS
    const int pc = dbase.get<int >("pc");
    const int uc = dbase.get<int >("uc");
    const int vc = dbase.get<int >("vc");
    const int wc = dbase.get<int >("wc");

    assert( pc>=0 && uc>=0 && vc>=0 );
    if( !thisProcessorHasPoints )
      return 0; // no points on this processor
    
    ua(I1,I2,I3,pc)=0.;
    ua(I1,I2,I3,uc)=1.+t;
    ua(I1,I2,I3,vc)=2.+t;
    if( mg.numberOfDimensions()==3 )
      ua(I1,I2,I3,wc)=3.+t;
     
  }

  else if( userKnownSolution=="linearBeamExactSolution" )
  {
 
    MappedGrid & mg = cg[grid];
    GET_VERTEX_ARRAY(vertex);

    RealArray & u = ua;

    const int & uc = dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
    const int & vc = dbase.get<int >("vc");  
    const int & pc = dbase.get<int >("pc");  

    // --- these parameters must match those in BeamModel.C -- *fix me*
    double E=1.4e6;
    double rhos=10000.0;
    double h=0.02;
    double Ioverb=6.6667e-7;
    double rhof=1000;
    double nu=0.001;
    double L=0.3;
    double H=0.3;
    double k=2.0*3.141592653589/L;
    double omega0=sqrt(E*Ioverb*k*k*k*k/(rhos*h));
    double what = 0.00001;  // not used 

    assert( numberOfTimeDerivatives==0 );

    //double beta=1.0/nu*sqrt(E*Ioverb/(rhos*h));
    //std::complex<double> omegatilde(1.065048891,-5.642079778e-4);
    // std::cout << "t = " << t << std::endl;
    double omegar = 0.8907148069, omegai = -0.9135887123e-2;
    for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ ) {
      for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ ) {
	for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ ) {
	  
	  double y = vertex(i1,i2,i3,1);
	  double x = vertex(i1,i2,i3,0);
	  
	  BeamModel::exactSolutionVelocity(x,y,t,k,H,
					   omegar,omegai, 
					   omega0,nu,
					   what,u(i1,i2,i3,uc),
					   u(i1,i2,i3,vc));

	  BeamModel::exactSolutionPressure(x,y,t,k,H,
					   omegar,omegai, 
					   omega0,nu,
					   what,u(i1,i2,i3,pc));

	  //std::cout << x << " " << y << " " << u(i1,i2,i3,uc) << " " << u(i1,i2,i3,vc) << std::endl;
	}
      }
    }   
  }
 
  else if( userKnownSolution=="flatPlateBoundaryLayer" )
  {

    const real & U = rpar[0];   
    const real & xOffset = rpar[1];
    const real & nuBL = rpar[2];
    
    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    if( !db.has_key("BoundaryLayerProfile") )
    {
      db.put<BoundaryLayerProfile*>("BoundaryLayerProfile");
      db.get<BoundaryLayerProfile*>("BoundaryLayerProfile") = new BoundaryLayerProfile();  // who will delete ???

      BoundaryLayerProfile & profile = *db.get<BoundaryLayerProfile*>("BoundaryLayerProfile");
      // const real nu = dbase.get<real>("nu");
      profile.setParameters( nuBL,U );  // 

    }
    BoundaryLayerProfile & profile = *db.get<BoundaryLayerProfile*>("BoundaryLayerProfile");


    // --- evaluate the boundary layer solution ----
    assert( numberOfTimeDerivatives==0 );
    
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      ua(i1,i2,i3,pc)=0.;
      profile.eval( xLocal(i1,i2,i3,0)+xOffset,xLocal(i1,i2,i3,1), ua(i1,i2,i3,uc), ua(i1,i2,i3,vc) );
      if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
    }
  }
  else if( userKnownSolution=="beamPiston" )
  {
    // ---- Beam Piston : beam adjacent to one or two fluid domains ----

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    // ** NOTE this solution appears in userDefinedKnownSolution.C and beamInitialConditions.C ***
    const real & ya          = rpar[0];    
    const real & yb          = rpar[1];    
    const real & pa          = rpar[2];   
    const real & pb          = rpar[3];   
    const real & rhos        = rpar[4];    
    const real & hs          = rpar[5];    
    const real & fluidHeight = rpar[6];    
    const real & K0          = rpar[7];    
    const real & Kt          = rpar[8];    

    const real dp = pa-pb;
    // -- hard code these for now: **FIX ME**
    const real rho=1;

    
    const real omega=sqrt(K0/(rhos*hs+rho*fluidHeight));
    const real w    = (dp/K0)*(1. - cos(omega*t) );   // beam position y=w(t) 
    const real wt   = (dp/K0)*omega*sin(omega*t);
    const real wtt  = (dp/K0)*omega*omega*cos(omega*t);
    const real dpdy =-rho*wtt;  // dp/dy = -rho*w_tt
    
    if( t <= dt )
      printF("--UDKS-- eval beam piston solution at t=%9.3e, rhos*hs=%8.2e, rho*H=%8.2e, K0=%8.2e, pa=%g pb=%g w=%g wt=%g wtt=%g "
             "dpdy=%g\n",t,rhos*hs,rho*fluidHeight,K0,pa,pb,w,wt,wtt,dpdy);

    // --- evaluate the solution ----

    // Note: When checking which side of the beam we are on do not use the y-location of
    // ghost points since these may go to the opposite side. Instead use the closest
    // boundary point (i1a,i2a) computed below.
    const IntegerArray & gid = cg[grid].gridIndexRange();
    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        const int i1a = max(gid(0,0),min(gid(0,1),i1));
	const int i2a = max(gid(0,1),min(gid(1,1),i2));
	
	const real y0 = xLocal(i1a,i2a,i3,1);
	const real y = xLocal(i1,i2,i3,1);
	ua(i1,i2,i3,uc)=0.;
	ua(i1,i2,i3,vc)=wt;
	if( y0>w )
	  ua(i1,i2,i3,pc)=pb + dpdy*( y-yb);   // upper fluid 
	else 
	  ua(i1,i2,i3,pc)=pa + dpdy*( y-ya);   // lower fluid 
      
	if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
      }
    }
    else if( numberOfTimeDerivatives==1 )
    {
      // return the time derivative of the solution
      const real wttt  = -(dp/K0)*omega*omega*omega*sin(omega*t);
      const real dpdyt =-rho*wttt;  // p_yyt = -rho*w_ttt

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        const int i1a = max(gid(0,0),min(gid(0,1),i1));
	const int i2a = max(gid(0,1),min(gid(1,1),i2));

	const real y0 = xLocal(i1a,i2a,i3,1);
	const real y = xLocal(i1,i2,i3,1);
	ua(i1,i2,i3,uc)=0.;
	ua(i1,i2,i3,vc)=wtt;
	if( y0>w )
	  ua(i1,i2,i3,pc)=dpdyt*( y-yb);   // upper fluid 
	else 
	  ua(i1,i2,i3,pc)=dpdyt*( y-ya);   // lower fluid 
      
	if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
      }
    }
    else
    {
      OV_ABORT("finish me");
    }
    
  }

  else if( userKnownSolution=="rigidBodyPiston" )
  {
    // ---- Rigid Body Piston Solution ---
    // FSI solution for a rigid body next to a fluid channel.
    // The pressure on the open boundary is chosen to give a specified body motion.
    //    xBody(t) = amp*sin(freq*2*pi*t)  : body displacement from initial position.

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    // ** NOTE this solution ALSO appears in userDefinedBoundaryValues.

    const real & amp   = rpar[0];
    const real & freq  = rpar[1];
    const real & depth = rpar[2];
    const real & bodyMass = rpar[3];

    if( t <= dt )
      printF("--UDKS-- Rigid Body Piston Solution at t=%9.3e, amp=%8.2e, freq=%8.2e, depth=%8.2e, bodyMass=%8.2e"
             " numberOfTimeDerivatives=%i\n",
	     t,amp,freq,depth,bodyMass,numberOfTimeDerivatives);

    // --- evaluate the solution ----
    real fluidDensity=1.;
    real length = 1.5;   // initial length of fluid domain *FIX ME*

    const real xBody =  amp*sin(freq*twoPi*t);                 // body offset 
    const real vBody =  amp*freq*twoPi*cos(freq*twoPi*t);      // body velocity
    const real aBody = -amp*SQR(freq*twoPi)*sin(freq*twoPi*t); // body acceleration

    // pressure force at right-hand-side of the channel:
    real addedMass =  fluidDensity*depth*(length - xBody);
    real gravity=0.;  // *check me* 
    const real pressureBC = (-1./depth)*( (bodyMass + addedMass )*aBody - gravity );

    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	const real x = xLocal(i1,i2,i3,0);

	ua(i1,i2,i3,uc)=vBody; // fluid velocity is constant in space
	ua(i1,i2,i3,vc)=0.;

        ua(i1,i2,i3,pc)= (-fluidDensity*aBody)*(x-length) + pressureBC; // pressure is a linear function

	if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
      }
      
    }
    else if( numberOfTimeDerivatives==1 )
    {
      // return the time derivative of the solution

      const real xBodyttt = -amp*pow(freq*twoPi,3)*cos(freq*twoPi*t); // 3 time derivatives of body motion
      const real addedMasst = fluidDensity*depth*( -vBody);
      const real pressureBCt = (-1./depth)*( (bodyMass + addedMass )*xBodyttt + addedMasst*aBody );

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {

	const real x = xLocal(i1,i2,i3,0);

	ua(i1,i2,i3,uc)=aBody; // fluid velocity is constant in space
	ua(i1,i2,i3,vc)=0.;

        ua(i1,i2,i3,pc)= (-fluidDensity*xBodyttt)*(x-length) + pressureBCt;

	if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;

      }
    }
    else
    {
      OV_ABORT("finish me");
    }
    
  }

  else if( userKnownSolution=="shearBlock" )
  {
    // --- Shear block exact solution ---
    //  FSI solution for a rigid body moving tangentially to a fluid channel.
    //      u = amp*[cos(y-H)/cos(H)]*exp(-nu t)
    //   Interface:  u(x,0,t) = ub(t) = amp*exp(-nu t) 
    //   BC:   u.y(x,H,t)=0 
    //  The fluid depth H must be chosen to satisfy:
    //      tan(H)=-mass/(rho*L)


    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    const real & amp      = rpar[0];
    const real & bodyMass = rpar[1];
    const real & length   = rpar[2];
    const real & height   = rpar[3];
    const real & k        = rpar[4];

    if( t <= dt )
      printF("--UDKS-- Shear-block exact INS-RB solution at t=%9.3e, amp=%8.2e, H=%8.2e, bodyMass=%8.2e, k=%g"
             " ntd=%i\n",
	     t,amp,height,bodyMass,k,numberOfTimeDerivatives);

    // --- evaluate the solution ----
    // const real & fluidDensity = dbase.get<real >("fluidDensity");
    const real nu = dbase.get<real >("nu");    

    const real expnut=exp(-nu*k*k*t);
   

    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 )
    {
      const real aFactor = (amp/cos(k*height))*expnut;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	const real y = xLocal(i1,i2,i3,1);

	ua(i1,i2,i3,uc)=aFactor*cos(k*(y-height));
	ua(i1,i2,i3,vc)=0.;

        ua(i1,i2,i3,pc)=0.;

	if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
      }
      
    }
    else if( numberOfTimeDerivatives==1 )
    {
      // return the time derivative of the solution
      const real aFactor = (-nu*k*k)*(amp/cos(k*height))*expnut;

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {

	const real y = xLocal(i1,i2,i3,1);

	ua(i1,i2,i3,uc)=aFactor*cos(k*(y-height));
	ua(i1,i2,i3,vc)=0.;

        ua(i1,i2,i3,pc)=0.;

	if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;

      }
    }
    else
    {
      OV_ABORT("finish me");
    }
    
  }
  else if( userKnownSolution=="rotatingDiskInDisk" )
  {
    // --- Exact solution for a rigid-disk rotating in a disk of fluid -----


    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    const real & amp         = rpar[0];
    const real & a           = rpar[1];
    const real & b           = rpar[2];
    const real & k           = rpar[3];
    const real & bodyDensity = rpar[4];
    const real & fluidDensity = dbase.get<real >("fluidDensity");  

    if( t <= 3.*dt )
    {
      printF("--UDKS-- rotating-disk-in-disk exact INS-RB solution at t=%9.3e, amp=%8.2e, a=%8.2e, b=%8.2e, k=%12.4e"
             " numberOfTimeDerivatives=%i\n",
	     t,amp,a,b,k,numberOfTimeDerivatives);
      printF("    grid=%i I1=[%i,%i] I2=[%i,%i]\n",grid,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
      
    }
    
    if( false )
    {
      int i1=0, i2=0, i3=0;
      printF("--UDKS--  grid=%i: t=%12.5e, point (i1=0,i2=0) (x,y)=(%20.12e,%20.12e)\n",
	     grid,t,xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1));
    }

    // --- evaluate the solution ----
    // const real & fluidDensity = dbase.get<real >("fluidDensity");
    const real nu = dbase.get<real >("nu");    
    const real expnukt=exp(-nu*k*k*t);
    const int n=1;
    const real Jna=jn(n,k*a), Yna=yn(n,k*a);
    const real Jnb=jn(n,k*b), Ynb=yn(n,k*b);
    const real scale = Jna*Ynb-Jnb*Yna;  // scale to make uTheta(r=a) = amp at t=0
   

    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 )
    {
      const real aFactor = amp*expnukt/scale;
      const real pFactor = fluidDensity*(amp*expnukt)*(amp*expnukt);
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( mask(i1,i2,i3)!=0 ) // We need to avoid evaluating Yn at r=0 
	{
	  const real x = xLocal(i1,i2,i3,0);
	  const real y = xLocal(i1,i2,i3,1);
	  const real r = sqrt(x*x+y*y);
        
	  const real uTheta = aFactor*( jn(n,k*r)*Ynb - Jnb*yn(n,k*r) ); // angular velocity 

	  // thetaHat = (-sin(theta),cos(theta)
	  ua(i1,i2,i3,uc)=-uTheta*y/r;
	  ua(i1,i2,i3,vc)= uTheta*x/r; 

	  // Pressure satisfies p_r = rho*uTheta^2/r -- but assume amp is scaled to be small so p=0

	  // Pressure is defined in a maple generated file:
#include "rotatingDiskPressure.h"

	  ua(i1,i2,i3,pc)=pFactor*pv;
	  if( ua(i1,i2,i3,pc) != ua(i1,i2,i3,pc) )
	  {
	    printF("--UDKS-- ERROR: p=nan for (i1,i2,i3)=(%i,%i,%i) r=%g \n",i1,i2,i3,r);
	    OV_ABORT("error");
	  }
	
	}
	else
	{
	  ua(i1,i2,i3,uc)=0.;
	  ua(i1,i2,i3,vc)=0.;
	  ua(i1,i2,i3,pc)=0.;
	}
        if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
	
      }
      
    }
    else if( numberOfTimeDerivatives==1 )
    {
      // return the time derivative of the solution
      const real aFactor = -nu*k*k*amp*expnukt/scale;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( mask(i1,i2,i3)!=0 ) // We need to avoid evaluating Yn at r=0 
	{
	  const real x = xLocal(i1,i2,i3,0);
	  const real y = xLocal(i1,i2,i3,1);
	  const real r = sqrt(x*x+y*y);
        
	  const real uTheta = aFactor*( jn(n,k*r)*Ynb - Jnb*yn(n,k*r) ); // angular velocity 

	  // thetaHat = (-sin(theta),cos(theta)
	  ua(i1,i2,i3,uc)=-uTheta*y/r;
	  ua(i1,i2,i3,vc)= uTheta*x/r; 

	  ua(i1,i2,i3,pc)=0.;
	}
	else
	{
	  ua(i1,i2,i3,uc)=0.;
	  ua(i1,i2,i3,vc)=0.;
	  ua(i1,i2,i3,pc)=0.;
	}
	
	if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
      }
    }
    else
    {
      OV_ABORT("finish me");
    }
    
  }

  else if( userKnownSolution=="translatingDiskInDisk" )
  {
    // --- Exact solution for a rigid-disk translating in a disk of fluid -----

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);


    const real & amp         = rpar[0];
    const real & a           = rpar[1];
    const real & b           = rpar[2];
    const real & k           = rpar[3];
    const real & bodyDensity = rpar[4];
    const real & fluidDensity = dbase.get<real >("fluidDensity");  

    if( t <= dt )
      printF("--UDKS-- translating-disk-in-disk exact INS-RB solution at t=%9.3e, amp=%8.2e, a=%8.2e, b=%8.2e, k=%12.4e"
             " numberOfTimeDerivatives=%i\n",
	     t,amp,a,b,k,numberOfTimeDerivatives);

    // --- evaluate the solution ----
    // const real & fluidDensity = dbase.get<real >("fluidDensity");
    const real nu = dbase.get<real >("nu");    
    const real mu = nu*fluidDensity;
    const real expnukt=exp(-nu*k*k*t);

    const real alam=k, alam2=alam*alam;
    const real za=alam*a, za2=za*za;
    const real zb=alam*b;
 
    const real j0a =jn(0,za),         y0a =yn(0,za);
    const real j1a =jn(1,za),         y1a =yn(1,za);
    const real j1pa=j0a-j1a/za,       y1pa=y0a-y1a/za;

    const real j0b=jn(0,zb),          y0b=yn(0,zb);
    const real j1b=jn(1,zb),          y1b=yn(1,zb);

    const real a11=(Pi*b/(2*a))*(2*y1b-zb*y0b),   a12=(Pi*a/(2*b))*zb*y0b;
    const real a21=(Pi*b/(2*a))*(2*j1b-zb*j0b),   a22=(Pi*a/(2*b))*zb*j0b;

    const real c11=(a11*j1a-a21*y1a+1)/za2,         c12=(a12*j1a-a22*y1a-1)/za2;
    const real c21=((a11*j1pa-a21*y1pa)*za+1)/za2,  c22=((a12*j1pa-a22*y1pa)*za+1)/za2;

    const real determ=c11*c22-c12*c21;
    const real ahat=(c22-c12)/(determ*a*a);
    const real bhat=(c11-c21)/(determ);

    const real k1hat= (Pi*b/2)*((2*y1b-zb*y0b)*ahat/alam2+y0b*bhat/zb);
    const real k2hat=-(Pi*b/2)*((2*j1b-zb*j0b)*ahat/alam2+j0b*bhat/zb);

    const real scale = 1.; // scale factor
   

    // -- scale the solution depending on which time derivative is needed ---
    real aFactor=0.;
    if( numberOfTimeDerivatives==0 )
      aFactor = amp*expnukt/scale;
    else if( numberOfTimeDerivatives==1 )
      aFactor = -nu*k*k*amp*expnukt/scale;
    else
    {
      OV_ABORT("error");
    }
    
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      if( mask(i1,i2,i3)!=0 ) // We need to avoid evaluating Yn at r=0 
      {
	const real x = xLocal(i1,i2,i3,0);
	const real y = xLocal(i1,i2,i3,1);
	const real r = sqrt(x*x+y*y);
	const real cosTheta=x/r, sinTheta=y/r;
        
	const real z=alam*r, z2=z*z;
	const real j1=jn(1,z),         y1=yn(1,z);
	const real j1p=jn(0,z)-j1/z,  y1p=yn(0,z)-y1/z;

	const real uhat=(k1hat*j1+k2hat*y1)/r+ahat/alam2-bhat/z2;         // Q(r)/r
	const real vhat=alam*(k1hat*j1p+k2hat*y1p)+ahat/alam2+bhat/z2;    //  Q'(r)
	const real phat=ahat*r+bhat/r;

        const real uRadial = uhat*aFactor*cosTheta;
	const real uTheta  =-vhat*aFactor*sinTheta;

	ua(i1,i2,i3,uc)= uRadial*cosTheta - uTheta*sinTheta;
	ua(i1,i2,i3,vc)= uRadial*sinTheta + uTheta*cosTheta;

	ua(i1,i2,i3,pc)= aFactor*mu*phat*cosTheta;  // note mu
      }
      else
      {
	ua(i1,i2,i3,uc)=0.;
	ua(i1,i2,i3,vc)=0.;
	ua(i1,i2,i3,pc)=0.;
      }
      if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
    }
    
  }

  else if( userKnownSolution=="bulkSolidPiston" )
  {
    // ---- return the exact solution for the FSI INS+elastic piston ---
    //     y_I(t) = F(t + Hbar/cp) - F(t - Hbar/cp)
    //        F(z) = amp * R(z) * sin( 2*Pi*k(t-t0) )
    //        R(z) = ramp function that smoothly transitions from 0 to 1 

    // **** CHECK ME ****
    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    const real & H        = rpar[0];
    const real & Hbar     = rpar[1];
    const real & rho      = rpar[2];
    const real & rhoBar   = rpar[3];
    const real & lambdaBar= rpar[4];
    const real & muBar    = rpar[5];

    const real & fluidDensity = dbase.get<real >("fluidDensity");
    assert( rho==fluidDensity );

    const real cp = sqrt((lambdaBar+2.*muBar)/rhoBar);
  
    TimeFunction & bsp = db.get<TimeFunction>("timeFunctionBSP");

    const real ys=0; // interface 
    real xim,xip, fm,fp, fmd,fpd, fmdd,fpdd;
    xim=t-(ys+Hbar)/cp;
    xip=t+(ys+Hbar)/cp;  
        
    // eval F and F' at xim and xip:
    bsp.eval(xim, fm,fmd );  // fmd = d(fm(xi))/d(xi)
    bsp.eval(xip, fp,fpd );

    // eval F''
    bsp.evalDerivative(xim, fmdd, 2 ); // 2 derivatives 
    bsp.evalDerivative(xip, fpdd, 2 );

    
    const real yI =  fp - fm;        // interface position
    const real vI =  fpd - fmd;      // interface velocity
    const real aI =  fpdd - fmdd;    // interface acceleration
    const real pI = -(lambdaBar+2.*muBar)*(fpd + fmd)/cp;  // interface p
    const real pIt= -(lambdaBar+2.*muBar)*(fpdd + fmdd)/cp;  // interface p_t

    const real p0 = pI - rho*H*aI*(1.-yI/H);        // fluid pressure at y=H 
    const real pAmp = (p0-pI)/(H-yI);

    // const real pI = -amp*(lambdaBar+2.*muBar)*cp*k*coskHbar*cost;
    
    // const real yI =  amp*         sinkHbar*cost;    // interface position
    // const real vI = -amp*cp*k*    sinkHbar*sint;    // interface velocity
    // const real aI = -amp*SQR(cp*k)*sinkHbar*cost;   // interface acceleration
    // const real p0 = pI - rho*H*aI*(1.-yI/H);        // fluid pressure at y=H 
    // const real pAmp = (p0-pI)/(H-yI);

    if( t <= 2.*dt )
    {
      printF("--INS-- userDefinedKnownSolution: bulkSolidPiston, t=%9.3e, yI=%9.3e vI=%9.3e,\n",
             t,yI,vI);
      printF("        rho=%8.3e aI=%9.3e, p0=%9.3e, pI=%9.3e, pAmp=%9.3e, pIt=%9.3e\n",rho,aI,p0,pI,pAmp,pIt);

      // // Traction rate is - d(pAmp)/dt
      // real yIt = vI;
      // real pIt = amp*(lambdaBar+2.*muBar)*SQR(cp*k)*coskHbar*sint;
      // real aIt = amp*SQR(cp*k)*cp*k*sinkHbar*sint;
      // real p0t = pIt -rho*H*( aIt*(1.-yI/H) + aI*(-yIt/H) );
      
      // real pAmpt = (p0t-pIt)/(H-yI) + yIt*pAmp/(H-yI);
      // real s22t = -( p0t - pAmpt*H ); // s22_t = - p_t 

      // printF("--INS-- vI=%g, p0=%g, pAmp=%g, pAmpt=%g, s22t=%12.6e\n",vI,p0,pAmp,pAmpt,s22t);
    }
    
    if( true && grid==1 )
    {
      real yIerr = xLocal(0,0,0,1)-yI;
      printF(" --UDKS--  ERROR IN INTERFACE POSITION = %9.3e at t=%9.3e\n",yIerr,t);
    }
    
    int i1,i2,i3;

    if( numberOfTimeDerivatives==0 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        // const real x = xLocal(i1,i2,i3,0);
        const real y = xLocal(i1,i2,i3,1);

        ua(i1,i2,i3,uc) = 0.;
        ua(i1,i2,i3,vc) = vI;
        ua(i1,i2,i3,pc) = p0 - pAmp*(H-y);

      }
    }
    else if( numberOfTimeDerivatives==1 )
    {
      // This is currently only needed for debugging, computing errors in du/dt

      // **check me**

      // eval F'''
      real fmddd,fpddd;
      bsp.evalDerivative(xim, fmddd, 3 ); // 3 derivatives 
      bsp.evalDerivative(xip, fpddd, 3 );

      const real aIt =  fpddd - fmddd;    // 3 time-deriatives of the interface
      const real p0t = pIt - rho*H*aIt*(1.-yI/H) - rho*H*aI*(1.-vI/H);      
      const real pAmpt =  (p0t-pIt)/(H-yI) - (p0-pI)*(-vI)/SQR(H-yI);

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        // const real x = xLocal(i1,i2,i3,0);
        const real y = xLocal(i1,i2,i3,1);

        ua(i1,i2,i3,uc) = 0.;
        ua(i1,i2,i3,vc) = aI;
        ua(i1,i2,i3,pc) = p0t - pAmpt*(H-y);

      }
    }
    else
    {
      printF("--UDKS-- Error, not implemented for numberOfTimeDerivatives=%i \n",numberOfTimeDerivatives);
    }
    
  }

  else if( userKnownSolution=="radialElasticPiston" )
  {
    // ---- return the exact solution for radial elastic piston ----

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);
    const real & R        = rpar[0];
    const real & Rbar     = rpar[1];
    const real & rho      = rpar[2];
    const real & rhoBar   = rpar[3];
    const real & lambdaBar= rpar[4];
    const real & muBar    = rpar[5];
    const real & k        = rpar[6];

    const real cp = sqrt((lambdaBar+2.*muBar)/rhoBar);
  
    const real mu = rho*nu;

    // uI = uI(t) =  interface displacement in the radial direction 
    // eval uI and vI = uI_t 
    real uI,vI,aI;
    TimeFunction & bsp = db.get<TimeFunction>("timeFunctionREP");
    bsp.eval(t, uI,vI );
    bsp.evalDerivative(t, aI, 2 ); // 2 derivatives 

    if( t <= 2.*dt )
    {
      printF("--INS-- UD DB Known radialElasticPiston, t=%9.3e uI=%9.3e vI=%9.3e R=%6.3f Rbar=%6.3f\n"
             "  grid=%i I1=[%i,%i] I2=[%i,%i]\n",
             t,uI,vI,R,Rbar,grid,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
      // if( grid==1 )
      // {
      //   int i1=0, i2=0, i3=0;
      //   printF(" --> grid=%i: (x,y)(0,0) = (%9.3e,%9.3e)\n",grid,xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1));
      // }

    }

    // FIrst eval the solid solution on the interface ar rs=rBar 
    real kr=k*Rbar;
    real jnkr = jn(1,kr);
    real jnkrp = .5*k*(jn(0,kr)-jn(2,kr));  // Jn' = .5*( J(n-1) - J(n+1) )
        
    real ur = uI*jnkr;       // radial displacement in solid 
    real urr= uI*jnkrp;      // r-derivative of the radial displacement
    real vr = vI*jnkr;       // radial velocity 
    real ar = aI*jnkr;       // radial acceleration

    real RI = Rbar + ur;  // inner radius of the fluid region as a function of time 
    real RIt = vr;        // d(RI)/dt
    
    // fluid radial velocity is Vr = f(t)/r 
    // Interface condition is Vr(RI) = f(t)/RI = vr 
    real f = RI*vr;                // f in notes 
    real ft = RI*ar + RIt*vr;      // df/dt 

    // Here is g(t) from notes 
    real g = (.5*rho*f*f - 2.*mu*f)/(RI*RI) - (lambdaBar+2.*muBar)*urr - lambdaBar*ur/Rbar;

    RealArray & u = ua;
    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        real x= xLocal(i1,i2,i3,0);
        real y= xLocal(i1,i2,i3,1);
        real r = sqrt( SQR(x) + SQR(y) );
        real cosTheta=x/r, sinTheta=y/r;
    
        real vR = f/r;        // radial velocity : vr = vI at r=RI 

        ua(i1,i2,i3,uc) = vR*cosTheta;
        ua(i1,i2,i3,vc) = vR*sinTheta;
        ua(i1,i2,i3,pc) = g - rho*f*f/(2.*r*r) - rho*ft*log(r/RI);

      }
    }
    else
    {
      OV_ABORT("FINISH ME");
    }
    


  }
  else if( userKnownSolution=="oscillatingBubble" )
  {
    // *** Bubble oscillating under surface tension *******
    // DS here is the solution for velocity and pressure

    real & amp = rpar[0];
    real & n = rpar[1];
    real & R = rpar[2];
    real & mu = rpar[3];
    real & omegar = rpar[4];
    real & omegai = rpar[5];
    real & lr = rpar[6];
    real & li = rpar[7];
    real & cr = rpar[8];
    real & ci = rpar[9];
    real & dr = rpar[10];
    real & di = rpar[11];
    real & gamma = rpar[12];
    real k = n;

    if( true )
    {
      printF("--UDKS-- evaluate oscillatingBubble at t=%9.3e, amp=%9.3e \n",t,amp);
    }
    
    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);
    const real eps = 10.*REAL_EPSILON;

    int i1,i2,i3;

    if (numberOfDimensions == 2) { // --- BEGIN 2D --- //
      if( numberOfTimeDerivatives==0 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      // **** JUST MAKE SOMETHING UP FOR NOW -- FINISH ME ***

	      real x= xLocal(i1,i2,i3,0);
	      real y= xLocal(i1,i2,i3,1);
	      real r = sqrt( SQR(x) + SQR(y) );
	      real cosTheta, sinTheta;
	      real vrr, vri, vtr, vti, pr, pi;
	      real vr, vt, p;

	      // compute trig functions
	      real theta = atan2(y,x);
	      if( abs(r)>eps )
		{
		  cosTheta=x/r; sinTheta=y/r;
		}
	      else
		{
		  cosTheta=1.; sinTheta=0.;
		}


	      // get vrhat and vthetahat
	      evalOscillatingBubble( r, R, (int)round(n), mu, lr,li,
				     cr,ci,dr,di,
				     vrr,vri,vtr,vti,pr,pi);

	      // compute phase
	      real phi = n*theta-omegar*t;

	      // compute decay factor
	      real A = exp(omegai*t)*amp; // *wdh* scale by amp too

	      // evaluate real part
	      vr = (vrr*cos(phi)-vri*sin(phi))*A;
	      vt = (vtr*cos(phi)-vti*sin(phi))*A;
	      p  = ((pr)*cos(phi)-(pi)*sin(phi))*A  +gamma/R;

	      ua(i1,i2,i3,uc) = vr*cosTheta-vt*sinTheta;
	      ua(i1,i2,i3,vc) = vr*sinTheta+vt*cosTheta;
	      ua(i1,i2,i3,pc) = p;

	      // ua(i1,i2,i3,uc) = 0.0;
	      // ua(i1,i2,i3,vc) = 0.0;
	      // ua(i1,i2,i3,pc) = 0.0;

	      // real kr = k*r;
        
	      // real j1 = jn(1,kr);  // Bessel function J_1


	      // ua(i1,i2,i3,uc) = amp*j1*cosTheta;
	      // ua(i1,i2,i3,vc) = amp*j1*sinTheta;
	      // ua(i1,i2,i3,pc) = amp*j1;


	    }
	} else {
	// some options may need a time derivative ...
	OV_ABORT("FINISH ME");
      }
    } else { // --- BEGIN 3D --- //
      if( numberOfTimeDerivatives==0 ) {
	FOR_3D(i1,i2,i3,I1,I2,I3) {
	  real x= xLocal(i1,i2,i3,0);
	  real y= xLocal(i1,i2,i3,1);
	  real r = sqrt( SQR(x) + SQR(y) );
	  real z= xLocal(i1,i2,i3,2);

	  const int wc = dbase.get<int >("wc");

	  real cosTheta, sinTheta;
	  real vrr, vri, vtr, vti, pr, pi;
	  real vr, vt, p;

	  // compute trig functions
	  real theta = atan2(y,x);
	  if( abs(r)>eps )
	    {
	      cosTheta=x/r; sinTheta=y/r;
	    }
	  else
	    {
	      cosTheta=1.; sinTheta=0.;
	    }

	  // get vrhat and vthetahat
	  evalOscillatingBubble( r, R, (int)round(n), mu, lr,li,
				 cr,ci,dr,di,
				 vrr,vri,vtr,vti,pr,pi);

	  // compute phase
	  real phi = n*theta-omegar*t;

	  // compute decay factor
	  real A = exp(omegai*t)*amp; // *wdh* scale by amp too

	  // evaluate real part
	  vr = (vrr*cos(phi)-vri*sin(phi))*A;
	  vt = (vtr*cos(phi)-vti*sin(phi))*A;
	  p  = ((pr)*cos(phi)-(pi)*sin(phi))*A  +gamma/R;

	  ua(i1,i2,i3,uc) = vr*cosTheta-vt*sinTheta;
	  ua(i1,i2,i3,vc) = vr*sinTheta+vt*cosTheta;
	  ua(i1,i2,i3,wc) = 0.;
	  ua(i1,i2,i3,pc) = p;

	}

      } else {
	OV_ABORT("FINISH ME");
      }

    }


  } else if ( userKnownSolution=="capillaryFlow") {

    // define our needed parameters and get amp and casenumber 
    real & amp = rpar[0];
    real & k = rpar[1];
    real & R = rpar[2];
    real & mu = rpar[3];
    real & omegar = rpar[4];
    real & omegai = rpar[5];
    real & alphar = rpar[6];
    real & alphai = rpar[7];
    real & ar = rpar[8];
    real & ai = rpar[9];
    real & br = rpar[10];
    real & bi = rpar[11];
    real & cr = rpar[12];
    real & ci = rpar[13];
    real & dr = rpar[14];
    real & di = rpar[15];
    real & gamma = rpar[16];
    
    if( true )
    {
      printF("--UDKS-- evaluate capillaryFlow at t=%9.3e, amp=%9.3e \n",t,amp);
    }

    GET_VERTEX_ARRAY(xLocal);
    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 ) {
      FOR_3D(i1,i2,i3,I1,I2,I3) {
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);

	real uhr,uhi,vhr,vhi,phr,phi;
	real v1,v2,p;

	// get real and imag parts of hat variables
	evalCapillaryFlow(k, y, mu, 
			  alphar, alphai, 
			  ar, ai, br, bi,
			  cr, ci, dr, di,
			  uhr, uhi, vhr, vhi, phr, phi);

	// compute phase
	real phase = k*x-omegar*t;

	// compute decay factor
	real A = exp(omegai*t)*amp; // *wdh* scale by amp too

	// evaluate real part
	v1 = (uhr*cos(phase)-uhi*sin(phase))*A;
	v2 = (vhr*cos(phase)-vhi*sin(phase))*A;
	p  = (phr*cos(phase)-phi*sin(phase))*A;

	// combine
	ua(i1,i2,i3,uc) = v1;
	ua(i1,i2,i3,vc) = v2;
	ua(i1,i2,i3,pc) =  p;
      }
    } else {
      // some options may need a time derivative ...
      OV_ABORT("FINISH ME");
    }


  } else if ( userKnownSolution=="freeSurfacePiston") {
	
    real & amp = rpar[0];
    real & k   = rpar[1];
    real & nu  = rpar[2];
    real & H   = rpar[3];
    real & amp1 = rpar[4];
    real & amp2 = rpar[4];

    if( true ) {
      printF("--UDKS-- evaluate freeSurfacePiston at t=%9.3e, amp=%9.3e \n",t,amp);
    }

    GET_VERTEX_ARRAY(xLocal);
    int i1,i2,i3;

    if (numberOfDimensions == 2) { // --- BEGIN 2D --- //
      if( numberOfTimeDerivatives==0 ) {
	FOR_3D(i1,i2,i3,I1,I2,I3) {
	  real x= xLocal(i1,i2,i3,0);
	  real y= xLocal(i1,i2,i3,1);
	
	  real yI = amp*sin(k*t) + amp1*t*t + amp2*t;
	  real v2 = k*amp*cos(k*t) + 2*amp1*t + amp2;
	  real v2p = -k*k*amp*sin(k*t) + 2*amp1;
	
	  real p = -(y-yI)*(v2p);

	  ua(i1,i2,i3,uc) = 0.;
	  ua(i1,i2,i3,vc) = v2;
	  ua(i1,i2,i3,pc) = p;

	}
      } else {
	// some options may need a time derivative ...
	printF("\n\n >>>>> --UDKS-- ERROR: FINISH ME -- time derivative <<<<<\n\n ");
	// OV_ABORT("FINISH ME");
      }
    } else { // --- BEGIN 3D --- //
      if( numberOfTimeDerivatives==0 ) {
	FOR_3D(i1,i2,i3,I1,I2,I3) {
	  real x= xLocal(i1,i2,i3,0);
	  real y= xLocal(i1,i2,i3,1);
	  real z= xLocal(i1,i2,i3,2);

	  const int wc = dbase.get<int >("wc");

	  real zI = amp*sin(k*t) + amp1*t*t + amp2*t;
	  real v3 = k*amp*cos(k*t) + 2*amp1*t + amp2;
	  real v3p = -k*k*amp*sin(k*t) + 2*amp1;
	
	  real p = -(z-zI)*(v3p);

	  ua(i1,i2,i3,uc) = 0.;
	  ua(i1,i2,i3,vc) = 0.;
	  ua(i1,i2,i3,wc) = v3;
	  ua(i1,i2,i3,pc) = p;

	}
      } else {
	// some options may need a time derivative ...
	printF("\n\n >>>>> --UDKS-- ERROR: FINISH ME -- time derivative <<<<<\n\n ");
	// OV_ABORT("FINISH ME");
      }

    }


  } else if ( userKnownSolution=="parallelFlow") {
	
    real & amp = rpar[0];
    real & n   = rpar[1];
    real & nu  = rpar[2];
    real & H   = rpar[3];
    if( true ) {
      printF("--UDKS-- evaluate parallelFlow at t=%9.3e, amp=%9.3e \n",t,amp);
    }

    GET_VERTEX_ARRAY(xLocal);
    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 ) {
      FOR_3D(i1,i2,i3,I1,I2,I3) {
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);

	ua(i1,i2,i3,uc) = amp
	  *sin((2.*n-1.) * Pi * (y+1.) / (2.*H))
	  *exp(-nu * pow((2.*n-1.)*Pi/(2.*H),2) * t);
	ua(i1,i2,i3,vc) = 0.;
	ua(i1,i2,i3,pc) = 0.;
      }
    } else {
      // some options may need a time derivative ...
      OV_ABORT("FINISH ME");
    }

  } else if( userKnownSolution=="fibShear" ) {
    // --------------------------------------------------------------------------------
    // ------ Exact solution for a parallel flow shearing a bulk elastic solid --------
    //  \bar{u}_1(y,t) = amp         exp(i omega t) (A cos(ks y) + B sin( ks y))
    //      {v}_1(y,t) = amp i omega exp(i omega t) (C exp(kf y) + D exp(-kf y))
    //              ks = omega / cs
    //              kf = sqrt(i rho omega / mu)
    //  Parameters:
    //  amp    : maximum amplitude of the displacement 
    //  omega  : time frequency of solution 
    //  H,Hbar : Height of fluid and solid domains
    //  rhoBar,lambaBar,muBar : solid density and Lame parameters
    // --------------------------------------------------------------------------------

    const real & omegar = rpar[0];
    const real & omegai = rpar[1];
    const real & ar     = rpar[2];
    const real & ai     = rpar[3];
    const real & br     = rpar[4];
    const real & bi     = rpar[5];
    const real & cr     = rpar[6];
    const real & ci     = rpar[7];
    const real & dr     = rpar[8];
    const real & di     = rpar[9];
    const real & ksr    = rpar[10];
    const real & ksi    = rpar[11];
    const real & kfr    = rpar[12];
    const real & kfi    = rpar[13];
    const real & amp    = rpar[14];

    if( true ) {
      printF("--INS-- evaluate fibShear at t=%9.3e \n",t,amp);
    }

    GET_VERTEX_ARRAY(xLocal);

    real u0_r = amp*exp(-omegai*t)*cos(omegar*t);
    real u0_i = amp*exp(-omegai*t)*sin(omegar*t);

    int i1,i2,i3;
    if( numberOfTimeDerivatives==0 ) {
      FOR_3D(i1,i2,i3,I1,I2,I3) {
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);
	
	real vr, vi;
	evalFibShearFluid(kfr,kfi,omegar,omegai,cr,ci,dr,di,y,vr,vi);
	ua(i1,i2,i3,uc) = vr*u0_r-vi*u0_i;
	ua(i1,i2,i3,vc) = 0.;
	ua(i1,i2,i3,pc) = 0.;
      }
    } else {
      // some options may need a time derivative ...
      OV_ABORT("FINISH ME");
    }

  } else 
  {
    // look for a solution in the base class
    Parameters::getUserDefinedKnownSolution( t, cg, grid, ua, I1,I2,I3 );
  }
  

  // else
  // {
  //   printF("getUserDefinedKnownSolution:ERROR: unknown value for userDefinedKnownSolution=%s\n",
  // 	   (const char*)userKnownSolution);
  //   OV_ABORT("ERROR");
  // }
  
  return 0;
}

// ===================================================================================================================
/// \brief Return properties of a known solution for rigid-body motions
/// \param body (input) : body number
/// \param t (input) : time
// ===================================================================================================================
int InsParameters::
getUserDefinedKnownSolutionRigidBody( int body, real t, 
				      RealArray & xCM      /* = Overture::nullRealArray() */, 
				      RealArray & vCM      /* = Overture::nullRealArray() */,
				      RealArray & aCM      /* = Overture::nullRealArray() */,
				      RealArray & omega    /* = Overture::nullRealArray() */, 
				      RealArray & omegaDot /* = Overture::nullRealArray() */ )
{

  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedKnownSolutionData") )
  {
    printf("getUserDefinedKnownSolutionRigidBody:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
    Overture::abort("error");
  }
  DataBase & db =  dbase.get<DataBase >("modelData").get<DataBase>("userDefinedKnownSolutionData");

  const aString & userKnownSolution = db.get<aString>("userKnownSolution");

  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");
  
  const real nu = dbase.get<real>("nu");
  const int pc = dbase.get<int >("pc");
  const int uc = dbase.get<int >("uc");
  const int vc = dbase.get<int >("vc");
  const int wc = dbase.get<int >("wc");
  const real dt = dbase.get<real>("dt");
  
  if( userKnownSolution=="rigidBodyPiston" )
  {
    // ---- Rigid Body Piston Solution ---
    // FSI solution for a rigid body next to a fluid channel.
    // The pressure on the open boundary is chosen to give a specified body motion.
    //    xBody(t) = amp*sin(freq*2*pi*t)  : body displacement from initial position.

    // ** NOTE this solution ALSO appears in userDefinedBoundaryValues.

    const real & amp   = rpar[0];
    const real & freq  = rpar[1];
    const real & depth = rpar[2];
    const real & bodyMass = rpar[3];

    // --- evaluate the solution ----
    real fluidDensity=1.;
    real length = 1.5;   // initial length of fluid domain *FIX ME*

    const real xBody =  amp*sin(freq*twoPi*t);                 // body offset 
    const real vBody =  amp*freq*twoPi*cos(freq*twoPi*t);      // body velocity
    const real aBody = -amp*SQR(freq*twoPi)*sin(freq*twoPi*t); // body acceleration

    if( xCM.getLength(0)>=3 )
    {
      xCM=0.; xCM(0)= -1. + xBody;  // assume xCM(0)=-1 
    }
    if( vCM.getLength(0)>=3 )
    {
      vCM=0.; vCM(0)= vBody;
    }
    if( aCM.getLength(0)>=3 )
    {
      aCM=0.; aCM(0)= aBody; 
    }
    if( omega.getLength(0)>=3 )
    {
       omega=0.; 
    }
    if( omegaDot.getLength(0)>=3 )
    {
       omegaDot=0.; 
    }

  }
  
  else if( userKnownSolution=="shearBlock" )
  {
    // --- Shear block exact solution ---
    //  FSI solution for a rigid body moving tangentially to a fluid channel.
    //      u = amp*[cos(y-H)/cos(H)]*exp(-nu t)
    //   Interface:  u(x,0,t) = ub(t) = amp*exp(-nu t) 
    //   BC:   u.y(x,H,t)=0 
    //  The fluid depth H must be chosen to satisfy:
    //      tan(H)=-mass/(rho*L)


    const real & amp      = rpar[0];
    const real & bodyMass = rpar[1];
    const real & length   = rpar[2];
    const real & height   = rpar[3];
    const real & k        = rpar[4];

    if( t <= dt )
      printF("--getUserDefinedKnownSolutionRigidBody-- Shear-block exact INS-RB solution at t=%9.3e, amp=%8.2e, "
             "H=%8.2e, bodyMass=%8.2e, k=%g \n",t,amp,height,bodyMass,k);

    // --- evaluate the solution ----
    // const real & fluidDensity = dbase.get<real >("fluidDensity");
    const real nu = dbase.get<real >("nu");    

    const real nuk2 = nu*k*k;
    const real expnut=exp(-nuk2*t);
    if( xCM.getLength(0)>=3 )
    {
      xCM=0.; xCM(0)= (amp/nuk2)*(1.-expnut);  // offset from initial center of mass
    }
    if( vCM.getLength(0)>=3 )
    {
       vCM=0.; vCM(0)= amp*expnut;
    }
    if( aCM.getLength(0)>=3 )
    {
       aCM=0.; aCM(0)= -nuk2*amp*expnut;
    }
    if( omega.getLength(0)>=3 )
    {
       omega=0.; 
    }
    if( omegaDot.getLength(0)>=3 )
    {
       omegaDot=0.; 
    }

  }
  else if( userKnownSolution=="rotatingDiskInDisk" )
  {
    // --- Exact solution for a rigid-disk rotating in a disk of fluid -----

    const real & amp      = rpar[0];
    const real & a        = rpar[1];
    const real & b        = rpar[2];
    const real & k        = rpar[3];

    const real nu = dbase.get<real >("nu");    
    const real expnukt=exp(-nu*k*k*t);
    const int n=1;
    const real Jna=jn(n,k*a), Yna=yn(n,k*a);
    const real Jnb=jn(n,k*b), Ynb=yn(n,k*b);
    const real scale = Jna*Ynb-Jnb*Yna;  // scale to make uTheta(r=a) = amp at t=0

    const real uTheta = amp*expnukt; // angular velocity 

    if( xCM.getLength(0)>=3 )
    {
      xCM=0.; 
    }
    if( vCM.getLength(0)>=3 )
    {
       vCM=0.; 
    }
    if( aCM.getLength(0)>=3 )
    {
       aCM=0.; 
    }
    if( omega.getLength(0)>=3 )
    {
      omega=0.;
      omega(2)=uTheta; 
    }
    if( omegaDot.getLength(0)>=3 )
    {
      omegaDot=0.;
      omegaDot(2)=(-nu*k*k)*uTheta; 
    }
  }
  else if( userKnownSolution=="translatingDiskInDisk" )
  {
    // --- Exact solution for a rigid-disk translating in a disk of fluid -----

    const real & amp      = rpar[0];
    const real & a        = rpar[1];
    const real & b        = rpar[2];
    const real & k        = rpar[3];

    const real nu = dbase.get<real >("nu");    
    const real expnukt=exp(-nu*k*k*t);

    const real scale = 1.;  

    const real aFactor = amp*expnukt/scale;

    if( xCM.getLength(0)>=3 )
    {
      xCM=0.;
      xCM(0)=(amp/scale)*(expnukt-1.)/(-nu*k*k);  // assume: xCM = 0 at t=0 
    }
    if( vCM.getLength(0)>=3 )
    {
      vCM=0.;
      vCM(0)=aFactor;
    }
    if( aCM.getLength(0)>=3 )
    {
      aCM=0.;
      aCM(0)=(-nu*k*k)*aFactor;
    }
    if( omega.getLength(0)>=3 )
    {
      omega=0.;
    }
    if( omegaDot.getLength(0)>=3 )
    {
      omegaDot=0.;
    }
  } // end  translatingDiskinDisk

  else
  {

    printF("InsParameters::getKnownSolutionRigidBody:ERROR: unknown userKnownSolution=%s\n",
           (const char*)userKnownSolution);
    OV_ABORT("ERROR");
  }

  return 0;
}



// ==========================================================================================
/// \brief Return the state of a known solution for a deforming body
/// 
/// \param body (input) : body number
/// \param stateOption (input) : specify which information to return
/// \param t (input) : time to evaluate the solution at 
/// \param grid, mg, I1,I2,I3 (input) : 
/// \param state (output) : return results here
/// \return (output) : 1=solution was found, 0=solution was not found 
// ==========================================================================================
int InsParameters::
getUserDefinedDeformingBodyKnownSolution( 
  int body,
  DeformingBodyStateOptionEnum stateOption, 
  const real t, const int grid, MappedGrid & mg, const Index &I1_, const Index &I2_, const Index &I3_, const Range & C, 
  realSerialArray & state )
{

  // Look for a solution in the base class function (cg/common/userDefinedKnownSolution.C):
  // returns found=1 : solution was found, 0=solution was not found 
  int found = Parameters::getUserDefinedDeformingBodyKnownSolution(body,stateOption,t,grid,mg,I1_,I2_,I3_,C,state );

  if( found ) return found;
  

  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedKnownSolutionData") )
  {
    printf("getUserDefinedKnownSolution:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
    OV_ABORT("error");
  }
  DataBase & db =  dbase.get<DataBase >("modelData").get<DataBase>("userDefinedKnownSolutionData");
  const aString & userKnownSolution = db.get<aString>("userKnownSolution");


  const int numberOfDimensions = mg.numberOfDimensions();
  const real dt = dbase.get<real>("dt");
  
  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");

  // Adjust index bounds for parallel *wdh* 2017/05/31 
  OV_GET_SERIAL_ARRAY_CONST(int,mg.mask(),mask);
  Index I1=I1_, I2=I2_, I3=I3_;
  int includeGhost=1;
  bool thisProcessorHasPoints=ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);

  found=1;
  if( userKnownSolution=="oscillatingBubble" )
  {

    // deforming body solution

    // ---  Bubble oscillating under surface tension ---
    //   amp = amplitude of the motion  (solution only valid for small amp, eg. amp=1.e-4)
    //   R = radius of the undeformed bubble
    //   k = wave number

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);
    real & amp = rpar[0];
    real & n = rpar[1];
    real & R = rpar[2];
    real & mu = rpar[3];
    real & omegar = rpar[4];
    real & omegai = rpar[5];
    real & lr = rpar[6];
    real & li = rpar[7];
    real & cr = rpar[8];
    real & ci = rpar[9];
    real & dr = rpar[10];
    real & di = rpar[11];
    real & gamma = rpar[12];
    real k = 0.;

    // amp = 0;
    if( t <= 2.*dt )
    {
      printF("--INS-- getUserDefinedDeformingBodyKnownSolution: oscillatingBubble, t=%9.3e amp=%9.3e R=%9.3e k=%5.3f\n",
             t,amp,R,k );
    }

    // *** MAKE UP SOMETHING FOR NOW *****
    // x = (R + amp*sin(2*pi*kt))*cos(theta)
    // y = (R + amp*cos(2*pi*kt))*sin(theta)
    real a_ = R + amp*sin(2.*Pi*k*t);
    real b_ = R + amp*cos(2.*Pi*k*t);

    real at_ = R + amp*2.*Pi*k*cos(2.*Pi*k*t);
    real bt_ = R - amp*2.*Pi*k*sin(2.*Pi*k*t);

    real att_ = R - amp*SQR(2.*Pi*k)*sin(2.*Pi*k*t);
    real btt_ = R - amp*SQR(2.*Pi*k)*sin(2.*Pi*k*t);

    const real eps = 10.*REAL_EPSILON;
    
    const int c0=C.getBase(), c1=c0+1;
    int i1,i2,i3;
    if ( numberOfDimensions==2 ) {
      /// --- loop over the grid points on the interface ---
      FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  // Reference coordinates for solid or grid positions for the fluid -- we only need angle theta
	  real x= xLocal(i1,i2,i3,0);
	  real y= xLocal(i1,i2,i3,1);
	  real r = sqrt( SQR(x) + SQR(y) );
	  real vrr, vri, vtr, vti, pr, pi;
	  real ct,st;
	  // compute trig functions
	  real theta = atan2(y,x);
	  if( r>eps )
	    {
	      ct=x/r; st=y/r;
	    }
	  else
	    {
	      ct=1.; st=0.;  // at the origin we just pick an angle, should not matter
	    }

	  // get vrhat and vthetahat
	  evalOscillatingBubble( r, R, (int)round(n), mu, lr,li,
				 cr,ci,dr,di,
				 vrr,vri,vtr,vti,pr,pi);

	  // compute phase
	  real phi = n*theta-omegar*t;

	  // compute decay factor
	  real A = exp(omegai*t)*amp; // *wdh* scale by amp too

	  if( stateOption==boundaryPosition )
	    {
	      // compute displacement
	      real ur = (A/(omegar*omegar+omegai*omegai)) 
		*(  omegai*(vrr*cos(phi)-vri*sin(phi)) 
		    - omegar*(vrr*sin(phi)+vri*cos(phi)));
	      real ut = (A/(omegar*omegar+omegai*omegai)) 
		*(  omegai*(vtr*cos(phi)-vti*sin(phi)) 
		    - omegar*(vtr*sin(phi)+vti*cos(phi)));

	      // position of the interface:
	      state(i1,i2,i3,c0)=(ur+R)*ct-ut*st;
	      state(i1,i2,i3,c1)=(ur+R)*st+ut*ct;

	    }
	  else if( stateOption==boundaryVelocity )
	    {
	      // compute velocity
	      real vr = (vrr*cos(phi)-vri*sin(phi))*A;
	      real vt = (vtr*cos(phi)-vri*sin(phi))*A;

	      // velocity of the interface:
	      state(i1,i2,i3,c0)=vr*ct-vt*st; 
	      state(i1,i2,i3,c1)=vr*st+vt*ct;
	    }
	  else if( stateOption==boundaryAcceleration )
	    {
	      // compute acceleration
	      real ar = (A/(omegar*omegar+omegai*omegai)) 
		*(  omegai*(-vrr*cos(phi)+vri*sin(phi)) 
		    - omegar*( vrr*sin(phi)+vri*cos(phi)));
	      real at = (A/(omegar*omegar+omegai*omegai)) 
		*(  omegai*(-vtr*cos(phi)+vti*sin(phi)) 
		    - omegar*( vtr*sin(phi)+vti*cos(phi)));

	      // acceleration of the interface:
	      state(i1,i2,i3,c0)=ar*ct-at*st; 
	      state(i1,i2,i3,c1)=ar*st+at*ct;
	    }

	  else
	    {
	      OV_ABORT("--INS-- UDKS: Unknown state option");
	    }
	} 
    } else {
      /// --- loop over the grid points on the interface ---
      FOR_3D(i1,i2,i3,I1,I2,I3) {
	// Reference coordinates for solid or grid positions for the fluid -- we only need angle theta
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);
	real r = sqrt( SQR(x) + SQR(y) );
	real z= xLocal(i1,i2,i3,2);
	real vrr, vri, vtr, vti, pr, pi;
	real ct,st;
	// compute trig functions
	real theta = atan2(y,x);
	if( r>eps ) {
	  ct=x/r; st=y/r;
	} else {
	  ct=1.; st=0.;  // at the origin we just pick an angle, should not matter
	}

	// get vrhat and vthetahat
	evalOscillatingBubble( r, R, (int)round(n), mu, lr,li,
			       cr,ci,dr,di,
			       vrr,vri,vtr,vti,pr,pi);
	// compute phase
	real phi = n*theta-omegar*t;

	// compute decay factor
	real A = exp(omegai*t)*amp; // *wdh* scale by amp too

	const int c2=c1+1;
	if( stateOption==boundaryPosition )
	  {
	    // compute displacement
	    real ur = (A/(omegar*omegar+omegai*omegai)) 
	      *(  omegai*(vrr*cos(phi)-vri*sin(phi)) 
		  - omegar*(vrr*sin(phi)+vri*cos(phi)));
	    real ut = (A/(omegar*omegar+omegai*omegai)) 
	      *(  omegai*(vtr*cos(phi)-vti*sin(phi)) 
		  - omegar*(vtr*sin(phi)+vti*cos(phi)));

	    // position of the interface:
	    state(i1,i2,i3,c0)=(ur+R)*ct-ut*st;
	    state(i1,i2,i3,c1)=(ur+R)*st+ut*ct;
	    state(i1,i2,i3,c2)=z;
	  }
	else if( stateOption==boundaryVelocity )
	  {
	    // compute velocity
	    real vr = (vrr*cos(phi)-vri*sin(phi))*A;
	    real vt = (vtr*cos(phi)-vri*sin(phi))*A;

	    // velocity of the interface:
	    state(i1,i2,i3,c0)=vr*ct-vt*st; 
	    state(i1,i2,i3,c1)=vr*st+vt*ct;
	    state(i1,i2,i3,c2)=0.;
	  }
	else if( stateOption==boundaryAcceleration )
	  {
	    // compute acceleration
	    real ar = (A/(omegar*omegar+omegai*omegai)) 
	      *(  omegai*(-vrr*cos(phi)+vri*sin(phi)) 
		  - omegar*( vrr*sin(phi)+vri*cos(phi)));
	    real at = (A/(omegar*omegar+omegai*omegai)) 
	      *(  omegai*(-vtr*cos(phi)+vti*sin(phi)) 
		  - omegar*( vtr*sin(phi)+vti*cos(phi)));

	    // acceleration of the interface:
	    state(i1,i2,i3,c0)=ar*ct-at*st; 
	    state(i1,i2,i3,c1)=ar*st+at*ct;
	    state(i1,i2,i3,c2)=0.;
	  }

	else
	  {
	    OV_ABORT("--INS-- UDKS: Unknown state option");
	  }

      } 
    }
    
    if( (stateOption==boundaryPosition) && (false))
    {
      const int c0=C.getBase(), c1=c0+1;
      real & R = rpar[2];
      real maxDisplacement = sqrt(max(fabs( SQR(state(I1,I2,I3,c0))+SQR(state(I1,I2,I3,c1)) -R*R )));
      RealArray du;
      du= sqrt(fabs(SQR(state(I1,I2,I3,c0))+SQR(state(I1,I2,I3,c1)) -R*R));
      
      ::display(du,"Boundary offset distance","%8.2e ");
      // ::display(state(I1,I2,I3,c0),"Boundary position x","%5.2f ");
      // ::display(state(I1,I2,I3,c1),"Boundary position y","%5.2f ");
      
      printF("\n >>> -UD-BD-KS-- boundaryPosition: R=%9.3e, max-displacement=%9.3e at t=%9.3e\n",R,maxDisplacement,t);
      
    }
      
  } else if (userKnownSolution=="freeSurfacePiston") {
    
    // --- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);
    real & amp = rpar[0];
    real & k   = rpar[1];
    real & nu  = rpar[2];
    real & H   = rpar[3];
    real & amp1 = rpar[4];
    real & amp2 = rpar[4];

    if( t <= 2.*dt )
    {

      printF("--INS-- getUserDefinedDeformingBodyKnownSolution: freeSurfacePiston, t=%9.3e amp=%9.3e\n",
             t,amp );
    }

    const int c0=C.getBase(), c1=c0+1, c2=c1+1;

    if (numberOfDimensions == 2) { // --- BEGIN 2D --- //
      /// --- loop over the grid points on the interface ---
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3) {
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);

	real yI = amp*sin(k*t) + amp1*t*t + amp2*t;
	real v2 = k*amp*cos(k*t) + 2*amp1*t + amp2;
	real v2p = -k*k*amp*sin(k*t) + 2*amp1;

	if( stateOption==boundaryPosition )
	  {
	    // position of the interface:
	    state(i1,i2,i3,c0)=x;
	    state(i1,i2,i3,c1)=yI;
	  }
	else if( stateOption==boundaryVelocity )
	  {
	    // velocity of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=v2;
	  }
	else if( stateOption==boundaryAcceleration )
	  {
	    // acceleration of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=v2p;
	  }

	else
	  {
	    OV_ABORT("--INS-- UDKS: Unknown state option");
	  }
      }
    } else { // --- BEGIN 3D --- //
      /// --- loop over the grid points on the interface ---
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3) {
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);
	real z= xLocal(i1,i2,i3,2);

	real zI = amp*sin(k*t) + amp1*t*t + amp2*t;
	real v3 = k*amp*cos(k*t) + 2*amp1*t + amp2;
	real v3p = -k*k*amp*sin(k*t) + 2*amp1;
	
	// printF("---UDKS--- compute 3D deformation of fluid\n");

	if( stateOption==boundaryPosition )
	  {
	    // position of the interface:
	    state(i1,i2,i3,c0)=x;
	    state(i1,i2,i3,c1)=y;
	    state(i1,i2,i3,c2)=zI;
	  }
	else if( stateOption==boundaryVelocity )
	  {
	    // velocity of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=0.;
	    state(i1,i2,i3,c2)=v3;
	  }
	else if( stateOption==boundaryAcceleration )
	  {
	    // acceleration of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=0.;
	    state(i1,i2,i3,c2)=v3p;
	  }

	else
	  {
	    OV_ABORT("--INS-- UDKS: Unknown state option");
	  }
	
      }
    }




  } else if (userKnownSolution=="capillaryFlow") {
    
    // --- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);
    real & amp = rpar[0];
    real & k   = rpar[1];
    real & R = rpar[2];
    real & mu = rpar[3];
    real & omegar = rpar[4];
    real & omegai = rpar[5];
    real & alphar = rpar[6];
    real & alphai = rpar[7];
    real & ar = rpar[8];
    real & ai = rpar[9];
    real & br = rpar[10];
    real & bi = rpar[11];
    real & cr = rpar[12];
    real & ci = rpar[13];
    real & dr = rpar[14];
    real & di = rpar[15];
    real & gamma = rpar[16];

    if( t <= 2.*dt )
    {

      printF("--INS-- getUserDefinedDeformingBodyKnownSolution: capillaryFlow, t=%9.3e amp=%9.3e\n",
             t,amp );
    }

    const int c0=C.getBase(), c1=c0+1;
    int i1,i2,i3;
    /// --- loop over the grid points on the interface ---
    FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	  
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);

	real uhr,uhi,vhr,vhi,phr,phi;
	real v1,v2,p;

	// get real and imag parts of hat variables
	evalCapillaryFlow(k, y, mu, 
			  alphar, alphai, 
			  ar, ai, br, bi,
			  cr, ci, dr, di,
			  uhr, uhi, vhr, vhi, phr, phi);

	// compute phase
	real phase = k*x-omegar*t;

	// compute decay factor
	real A = exp(omegai*t)*amp; 


	// left off here!
	if( stateOption==boundaryPosition )
	  {
	    // compute displacement
	    real u1 = (A/(omegar*omegar+omegai*omegai)) 
	      *(  omegai*(uhr*cos(phase)-uhi*sin(phase)) 
		  - omegar*(uhr*sin(phase)+uhi*cos(phase)));
	    real u2 = (A/(omegar*omegar+omegai*omegai)) 
	      *(  omegai*(vhr*cos(phase)-vhi*sin(phase)) 
		  - omegar*(vhr*sin(phase)+vhi*cos(phase)));

	    // position of the interface:
	    state(i1,i2,i3,c0)=x +u1;
	    state(i1,i2,i3,c1)=0.+u2;

	  }
	else if( stateOption==boundaryVelocity )
	  {

	    // velocity of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=0.;
	  }
	else if( stateOption==boundaryAcceleration )
	  {

	    // acceleration of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=0.;
	  }

	else
	  {
	    OV_ABORT("--INS-- UDKS: Unknown state option");
	  }
      }
  } else if (userKnownSolution=="parallelFlow") {
    
    // --- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);
    real & amp = rpar[0];
    real & n = rpar[1];

    if( t <= 2.*dt )
    {

      // printF("------------------------------\n");
      // printF("------------------------------\n");
      printF("--INS-- getUserDefinedDeformingBodyKnownSolution: parallelFlow, t=%9.3e amp=%9.3e\n",
             t,amp );
      // printF("------------------------------\n");
      // printF("------------------------------\n");

      // printF("(%d,%d)\n",stateOption,boundaryPosition);
    }

    const int c0=C.getBase(), c1=c0+1;
    int i1,i2,i3;
    /// --- loop over the grid points on the interface ---
    FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	  
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);


	if( stateOption==boundaryPosition )
	  {

	    // position of the interface:
	    state(i1,i2,i3,c0)=x;
	    state(i1,i2,i3,c1)=0.;
	    // printF("(x,y)=(%f,%f)\n",x,y);

	  }
	else if( stateOption==boundaryVelocity )
	  {

	    // velocity of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=0.;
	    // printF("boundary velocity\n");
	  }
	else if( stateOption==boundaryAcceleration )
	  {

	    // acceleration of the interface:
	    state(i1,i2,i3,c0)=0.;
	    state(i1,i2,i3,c1)=0.;
	    // printF("boundary acceleration\n");
	  }

	else
	  {
	    OV_ABORT("--INS-- UDKS: Unknown state option");
	  }
      }




  } else {
    found =0 ;  // Not found
  }


  if( !found )
  {
    printF("--INS-- UDKS: getUserDefinedDeformingBodyKnownSolution:ERROR: unknown userKnownSolution=[%s]\n",
	   (const char*)userKnownSolution);
    OV_ABORT("error");
  }
  
  return found;
}


int InsParameters::
updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg)
// ==========================================================================================
/// \brief This function is called to set the user defined known solution.
/// 
/// \return   >0 : known solution was chosen, 0 : no known solution was chosen
///
// ==========================================================================================
{
  // Make  dbase.get<real >("a") sub-directory in the data-base to store variables used here
  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedKnownSolutionData") )
     dbase.get<DataBase >("modelData").put<DataBase>("userDefinedKnownSolutionData");
  DataBase & db =  dbase.get<DataBase >("modelData").get<DataBase>("userDefinedKnownSolutionData");

  if( !db.has_key("userKnownSolution") )
  {
    db.put<aString>("userKnownSolution");
    db.get<aString>("userKnownSolution")="unknownSolution";
    
    db.put<real[20]>("rpar");
    db.put<int[20]>("ipar");
  }
  aString & userKnownSolution = db.get<aString>("userKnownSolution");
  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");


  const aString menu[]=
    {
      "no known solution",
      "choose a common known solution",
      "pipe flow",
      "rotating Couette flow",
      "Taylor Green vortex",
      "exact solution from a file",
      "uniform flow INS", // for testing INS    
      "linear beam exact solution",
      "flat plate boundary layer",
      "beam piston", // FSI solution for a beam and one or two adjacent fluid domains.
      "rigid body piston",  // FSI solution for a rigid-body next to a fluid channel
      "shear block",   // INS-Rigid body FSI solution for a shearing block
      "rotating disk in disk", // INS-Rigid body FSI solution for a rotating disk in a disk
      "oscillating bubble",  // oscillating bubble with a free surface and surface tension
      "capillary flow", // rectangular geometry under surface tension on top boundary with no slip bottom boundary
      "parallel flow", // parallel flow free surface on top and no slip wall on bottom
      "free surface piston", // deforming piston with free surface on top and pressure forcing on bottom
      "done",
      ""
    }; 

  gi.appendToTheDefaultPrompt("userDefinedKnownSolution>");
  aString answer;
  for( ;; ) 
  {

    int response=gi.getMenuItem(menu,answer,"Choose a known solution");
    
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="no known solution" )
    {
      userKnownSolution="unknownSolution";
    }

    else if( answer=="choose a common known solution" )
    {
      // Look for a known solution from the base class (in common/src)
      Parameters::updateUserDefinedKnownSolution(gi,cg);
    }

    else if( answer=="pipe flow" )
    {
      userKnownSolution="pipeFlow";
      dbase.get<bool>("knownSolutionIsTimeDependent")=false;  // known solution is NOT time dependent

      real & pInflow  = rpar[0];
      real & pOutflow = rpar[1];
      real & radius   = rpar[2];
      real & s0       = rpar[3];
      real & length   = rpar[4];
      real & ua       = rpar[5];
      real & ub       = rpar[6];

      int  & axialAxis= ipar[0];
      axialAxis=0;
       
      ua=0.; ub=0.;
      printF("--- Pipe flow ---\n"
             " p  = pInflow + (s-s0)*(pOutflow-pInflow)/length\n"
             " 2D: Poiseuille-Couette flow:\n"
             "   u(r) = -(1/(2*nu)* dp/dx *( radius^2 - r^2 )  + ua +(ub-ua)*(r+R)/(2R) \n"
             " 3D: Hagen-Poiseuille flow \n"
             "   u(r) = -(1/(4*nu)* dp/dx *( radius^2 - r^2 ) \n"
             " s = axial coordinate (x,y, or z)\n"
             " u = axial velocity (x,y, or z)\n"
             " axialAxis=0,1,2 denotes the axial axis x, y, or z\n"
	);
      
      gi.inputString(answer,"Enter radius, pInflow, pOutflow, s0, length, ua,ub, axialAxis for the exact solution");
      sScanF(answer,"%e %e %e %e %e %e %e %i",&radius,&pInflow,&pOutflow,&s0,&length,&ua,&ub,&axialAxis);

      printF("Pipe flow: radius=%g, s0=%g, pInflow=%g, pOutflow=%g, length=%g, ua=%g, ub=%g, axialAxis=%i\n",
	     radius,s0,pInflow,pOutflow,length,ua,ub,axialAxis);

    }
    else if( answer=="rotating Couette flow" )
    {
      userKnownSolution="rotatingCouetteflow";
      dbase.get<bool>("knownSolutionIsTimeDependent")=false;  // known solution is NOT time dependent

      real & rInner     = rpar[0];
      real & rOuter     = rpar[1];
      real & omegaInner = rpar[2];
      real & omegaOuter = rpar[3];

      int & axialAxis = ipar[3];

      printF("--- Rotating Couette flow (flow between rotating cylinders, Taylor-Couette) ---\n"
             " Parameters:\n"
             "    rInner, rOuter : inner and outer radii.\n"
             "    omegaInner, omegaOuter : inner and outer angular velocities.\n"
             "    axialAxis : 0=x, 1=y, 2=z - orientation of the cylindrical axis.\n"
             );
      gi.inputString(answer,"Enter rInner, rOuter, omegaInner, omegaOuter, axialAxis");
      sScanF(answer,"%e %e %e %e %i",&rInner,&rOuter,&omegaInner,&omegaOuter,&axialAxis);

      printF("Rotating Couette flow: rInner=%g, rOuter=%g, omegaInner=%g, omegaOuter=%g, axialAxis=%i\n",
	     rInner,rOuter,omegaInner,omegaOuter,axialAxis);

    }

    else if( answer=="Taylor Green vortex" )
    {
      userKnownSolution="TaylorGreenVortex";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & kp = rpar[0];
      int & axialAxis = ipar[0];
      axialAxis=2;

      printF("--- Taylor Green vortex is an exact solution ---\n"
             " u = sin(k x) cos(k y) F(t)\n"
             " v =-cos(k x) sin(k y) F(t)\n"
             " p = (rho/4)*( cos(2 k x) + cos(2 k y) ) F(t)*F(t)\n"
             "   where F(t) = exp( -2 nu k^2 t )\n"
             " In 3D the solution can be cyclically permuted by setting the axialAxis=0,1,2:\n"
             "   (u,v) (x,y) : axialAxis=2 \n"
             "   (v,w) (y,z) : axialAxis=0 \n"
             "   (w,u) (z,x) : axialAxis=1 \n"
             );
      gi.inputString(answer,"Enter kp, axialAxis (kp = k/(2 pi), wave number of the exact solution)");
      sScanF(answer,"%e %i",&kp,&axialAxis);

      printF("Taylor Green vortex: kp=%g, axialAxis=%i\n",kp,axialAxis);

    }

    else if( answer=="exact solution from a file" )
    {
      userKnownSolution="exactSolutionFromAFile";
      dbase.get<bool>("knownSolutionIsTimeDependent")=false;  // known solution does NOT depend on time

      printF("The exact solution can be defined by a solution in a show file (e.g. from a fine grid solution)\n");
      
      gi.inputString(answer,"Enter the the name of the file holding the exact solution");

      // sScanF(answer,"%e %e",&rpar[0],&rpar[1]);
      // printF("forced piston: mass=%e, height=%e\n",rpar[0],rpar[1]);

    }
    
    else if( answer=="uniform flow INS" )
    {
      userKnownSolution="uniformFlowINS";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent
    }
    
    else if( answer=="linear beam exact solution" ) 
    {
      // ** OLD WAY ***
      userKnownSolution="linearBeamExactSolution";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent 
      double omega;
      gi.inputString(answer,"Enter omega");
      sScanF(answer,"%e",&omega);


      // double scale=.00001;
      // gi.inputString(answer,"Enter the scale factor for the exact solution (0=use default)");
      // sScanF(answer,"%e",&scale);
      // if( scale==0 ){ scale=.00001; }  // 
      // if( !db.has_key("exactSolutionScaleFactorFSI") ) 
      // { 
      // 	dbase.put<real>("exactSolutionScaleFactorFSI");
      // 	dbase.get<real>("exactSolutionScaleFactorFSI")=scale; // scale FSI solution so linearized approximation is valid 
      // }
      // printF("INFO:Setting the exact FSI solution scale factor to %9.3e\n",scale);
      // MovingGrids & move = dbase.get<MovingGrids >("movingGrids");
      // assert( move.getNumberOfDeformingBodies()==1 );
      // DeformingBodyMotion & deform = move.getDeformingBody(0);
      // assert( deform.pBeamModel!=NULL );
      // deform.pBeamModel->dbase.get<real>("exactSolutionScaleFactorFSI")=scale;

    }

    else if( answer=="flat plate boundary layer" )
    {

      userKnownSolution="flatPlateBoundaryLayer";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true; // false;  // known solution is NOT time dependent 

      real & U        = rpar[0];
      real & xOffset  = rpar[1];
      real & nuBL     = rpar[2];
      U=1.;
      xOffset=1.;
      nuBL = dbase.get<real>("nu");

      printF("The flat plate boundary layer solution (Blasius) is a similiarity solution with\n"
             "the flat plate starting at x=0, y=0. The free stream velocity is U.\n"
             "To have a smooth inflow profile, enter an offset in x so the similiarity solution starts at this value\n"
             "Note: the vertical velocity v only makes sense if sqrt(nu*U/x) is small.\n"
             "NOTE: nu for the BL profile can be different than the actual nu.\n");
      gi.inputString(answer,sPrintF("Enter U, xOffset and nu (defaults U=%8.2e, xOffset=%8.2e, nu=%8.2e)",U,xOffset,nuBL));
      sScanF(answer,"%e %e %e",&U,&xOffset,&nuBL);
      printF("Setting U=%9.3e, xOffset=%9.3e, nu=%8.2e\n",U,xOffset,nuBL);
      
    }

    else if( answer=="beam piston" )
    {
      // ---- Beam Piston : beam adjacent to one or two fluid domains ----
      userKnownSolution="beamPiston";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true; // known solution IS time dependent 
      printF("The beam piston solution is an FSI solution for a horizontal beam adjacent to one or two fluid domains.\n"
             " The vertical motion of the beam is of the form:\n"
             "      y(t) = (dp/K0)*( 1 - cos(omega*t) )   (for an undamped beam, Kt=0)\n");
      real & ya          = rpar[0];    
      real & yb          = rpar[1];    
      real & pa          = rpar[2];   
      real & pb          = rpar[3];   
      real & rhos        = rpar[4];    
      real & hs          = rpar[5];    
      real & fluidHeight = rpar[6];    
      real & K0          = rpar[7];    
      real & Kt          = rpar[8];    

      gi.inputString(answer,sPrintF("Enter ya,yb,pa,pb,rhos,hs,fluidHeight,K0,Kt"));
      sScanF(answer,"%e %e %e %e %e  %e %e %e %e %e %e %e %e",&ya,&yb,&pa,&pb,&rhos,&hs,&fluidHeight,&K0,&Kt);
      printF("Setting ya=%g, yb=%g, pa=%g, pb=%g, rhos=%g, hs=%g, fluidHeight=%g, K0=%g, Kt=%g \n",
               ya,yb,pa,pb,rhos,hs,fluidHeight,K0,Kt);   

    }
    
    else if( answer=="rigid body piston" )
    {
      userKnownSolution="rigidBodyPiston";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & amp      = rpar[0];
      real & freq     = rpar[1];
      real & depth    = rpar[2];
      real & bodyMass = rpar[3];

      printF("--- Rigid Body Piston Solution ---\n"
             " FSI solution for a rigid body next to a fluid channel.\n"
             " The pressure on the open boundary is chosen to give a specified body motion.\n"
             "     xBody(t) = amp*sin(freq*2*pi*t)  : body displacement from initial position.\n"
             );
      gi.inputString(answer,"Enter amp, freq, and depth (height of fluid channel).");
      sScanF(answer,"%e %e %e",&amp,&freq,&depth);

      //-- get the mass of the rigid body
      MovingGrids & movingGrids = dbase.get<MovingGrids >("movingGrids");
      const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();
      assert( numberOfRigidBodies==1 );
      const int b=0;
      RigidBodyMotion & body = movingGrids.getRigidBody(b);
      bodyMass = body.getMass();

      printF("Rigid Body Piston: setting amp=%g, freq=%g, depth=%g, bodyMass=%g.\n",amp,freq,depth,bodyMass);

    }
    else if( answer=="shear block" )
    {
      userKnownSolution="shearBlock";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & amp         = rpar[0];
      real & bodyMass    = rpar[1];
      real & length      = rpar[2];
      real & height      = rpar[3];
      real & k           = rpar[4];

      printF("--- Shear block exact solution ---\n"
             " FSI solution for a rigid body moving tangentially to a fluid channel.\n"
             "     u = amp* [cos(k*(y-H))/cos(k*H)] *exp(-nu*k^2* t)\n"
             "  Interface:  u(x,0,t) = ub(t) = amp*exp(-nu t) \n"
             "  BC:   u.y(x,H,t)=0 \n"
             "  Parameter k is chosen from an eigenvalue condition:\n"
             "     tan(k*H)/(k*H) = -mass/(rho*L*H)\n"
             );
      gi.inputString(answer,"Enter amp, L, H (L, H = length and height of fluid-domain)");
      sScanF(answer,"%e %e %e",&amp,&length,&height);

      //-- get the mass of the rigid body
      MovingGrids & movingGrids = dbase.get<MovingGrids >("movingGrids");
      const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();
      assert( numberOfRigidBodies==1 );
      const int b=0;
      RigidBodyMotion & body = movingGrids.getRigidBody(b);
      bodyMass = body.getMass();

      const real & fluidDensity = dbase.get<real >("fluidDensity");
      assert( fluidDensity>0. );
      
      // Solve for kH=k*H: 
      // Look for the first root
      // 
      real kH0=.5*Pi+.05, kH1=1.5*Pi-.01;
      real f0=shearBlockFunction(kH0), f1=shearBlockFunction(kH1);
      if( f0*f1 > 0 )
      {
	printF("ERROR locating root for shear-block exact solution!\n");
	OV_ABORT("error");
      }
     
      printF("...look for a root on the interval k*H in [%g,%g], [f0,f1]=[%g,%g]\n",kH0,kH1,f0,f1);
      real tol=1.e-14; // 
      massTerm = bodyMass/(fluidDensity*height*length); // global variable passed to shearBlockFunction
      real kH = zeroin(kH0,kH1,shearBlockFunction,tol);
      k=kH/height;
      
      printF(" .. root found k=%20.14e\n",k);

      // // Here is the height -- we could check the grid to see if it matches this height and length.
      // height=Pi - atan2(bodyMass,fluidDensity*length);

      printF("Shear block: setting amp=%g, L=%g, H=%g, k=%16.12e, bodyMass=%g fluidDensity=%g.\n",
             amp,length,height,k, bodyMass,fluidDensity);
    }
    else if( answer=="rotating disk in disk" )
    {
      userKnownSolution="rotatingDiskInDisk";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & amp      = rpar[0];
      real & a        = rpar[1];
      real & b        = rpar[2];
      real & k        = rpar[3];
      real & bodyDensity = rpar[4];

      printF("--- Rotating disk exact solution ---\n"
             " FSI solution for a rotating rigid disk in a disk of an incompressible fluid.\n"
             "    v_theta = amp*( J1(k*r)*Y1(k*a) - J1(k*a)*Y1(k*r) )*exp( -nu*k^2 t ) \n"
             " Parameters: \n"
             " a,b: inner and outer radius\n"
             );
      gi.inputString(answer,"Enter amp, a, b (amplitude, inner and outer radius)");
      sScanF(answer,"%e %e %e",&amp,&a,&b);

      //-- get the moment of inertial of the rigid body
      MovingGrids & movingGrids = dbase.get<MovingGrids >("movingGrids");
      const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();
      assert( numberOfRigidBodies==1 );
      const int bodyNumber=0;
      RigidBodyMotion & body = movingGrids.getRigidBody(bodyNumber);
      bodyDensity = body.getDensity();

      // Moment of inertia: 
      RealArray mI; mI= body.getMomentsOfInertia();
      const real Iz = mI(2);

      const real & fluidDensity = dbase.get<real >("fluidDensity");
      assert( fluidDensity>0. );
      
      // Compute the parameter k that must satisfy the equation
      // 
      real bodyFluidMass = fluidDensity*Pi*a*a; // mass of displaced fluid

      // These next 4 parameters are global variables passed to the function "rotatingDiskFunction"
      innerRadius=a; outerRadius=b;
      inertiaTerm= Iz/(2*bodyFluidMass*a); 
      massTerm   = 1./a; 

      // Look for the first root
      real k0=.1, f0 = rotatingDiskFunction(k0);
      real dk=.5;
      real k1=k0+dk, f1=rotatingDiskFunction(k1);
      const int maxIt=20;
      for( int it=0; it<maxIt; it++ )
      {
	if( f0*f1 <= 0. ) break; // root is bracketed between [k0,k1]
        k0=k1; f0=f1;
        k1=k0+dk;
        f1=rotatingDiskFunction(k1);
      }
      if( f0*f1 > 0 )
      {
	printF("ERROR locating root for exact solution!\n");
	OV_ABORT("error");
      }
      printF("...look for a root on the interval [k0,k1]=[%g,%g] [f0,f1]=[%g,%g]\n",k0,k1,f0,f1);

      real tol=1.e-14; // 
      k = zeroin(k0,k1,rotatingDiskFunction,tol);

      printF("Rotating disk in disk: setting amp=%g, a=%g, b=%g, Iz=%g, k=%20.12e, fluidDensity=%g.\n",
              amp,a,b,Iz,k,fluidDensity);

    }

    else if( answer=="translating disk in disk" )
    {
      // **** FINISH ME *****
      userKnownSolution="translatingDiskInDisk";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & amp      = rpar[0];
      real & a        = rpar[1];
      real & b        = rpar[2];
      real & k        = rpar[3];

      printF("--- Translating solid disk inside a disk of fluid exact solution ---\n"
             " Parameters: \n"
             " amp : amplitude of the motion (solution only valid for small amp, e.g. amp=1e-4)\n"
             " a,b: inner and outer radius\n"
             );
      gi.inputString(answer,"Enter amp, a, b (amplitude, inner and outer radius)");
      sScanF(answer,"%e %e %e",&amp,&a,&b);

      //  --- get the mass of the body: ---
      MovingGrids & movingGrids = dbase.get<MovingGrids >("movingGrids");
      const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();
      assert( numberOfRigidBodies==1 );
      const int bodyNumber=0;
      RigidBodyMotion & body = movingGrids.getRigidBody(bodyNumber);
      real bodyMass = body.getMass();

      const real & fluidDensity = dbase.get<real >("fluidDensity");
      assert( fluidDensity>0. );
      
      // Compute the parameter k that must satisfy the equation
      // 
      real bodyFluidMass = fluidDensity*Pi*a*a; // mass of displaced fluid

      // These next 3 parameters are global variables passed to the function "translatingDiskFunction"
      innerRadius=a; outerRadius=b;
      massTerm=bodyMass/bodyFluidMass;

      // Look for the first root
      real k0=.1, f0 = translatingDiskFunction(k0);
      real dk=.5;
      real k1=k0+dk, f1=translatingDiskFunction(k1);
      const int maxIt=20;
      for( int it=0; it<maxIt; it++ )
      {
	if( f0*f1 <= 0. ) break; // root is bracketed between [k0,k1]
        k0=k1; f0=f1;
        k1=k0+dk;
        f1=translatingDiskFunction(k1);
      }
      if( f0*f1 > 0 )
      {
	printF("ERROR locating root for exact solution!\n");
	OV_ABORT("error");
      }
      printF("...look for a root on the interval [k0,k1]=[%g,%g] [f0,f1]=[%g,%g]\n",k0,k1,f0,f1);

      real tol=1.e-14; // 
      k = zeroin(k0,k1,translatingDiskFunction,tol);

      // k=k*1.1;
      
      printF("Translating disk in disk: setting amp=%g, a=%g, b=%g, delta=mb/(rho*pi*a^2)=%g, k=%20.12e.\n",
              amp,a,b,massTerm,k);

    }
 
    else if( answer=="oscillating bubble" )
    {
      userKnownSolution="oscillatingBubble";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      // get parameters from database
      const real nu  = dbase.get<real>("nu");
      // const real rho = dbase.get<real>("fluidDensity");
      real surfaceTension;
      dbase.get<ListOfShowFileParameters >("pdeParameters")
	.getParameter("surfaceTension",surfaceTension);

      // define our needed parameters and get amp and casenumber 
      real & amp = rpar[0];
      int casenumber;

      printF("---  Bubble oscillating under surface tension ---\n"
             "   amp        = amplitude of the motion  (solution only valid \n"
	     "                for small amp, eg. amp=1.e-4)\n"
             "   casenumber = choose parameter set to use (n, rho, R, mu, gamma)\n"
	     "                where n   = number of periods, \n"
	     "                      rho = fluid density, \n"
	     "                      R   = outer radius, \n"
	     "                      mu  = viscosity, \n"
	     "                      gamma = surface tension. \n"
             "                1: (n, rho, R, mu, gamma) = (1, 1.0, 1.0, .01, .1)\n"
	     "                2: (n, rho, R, mu, gamma) = (2, 1.0, 1.0, .01, .1)\n"
	     "                3: (n, rho, R, mu, gamma) = (3, 1.0, 1.0, .01, .1)\n"
	     "                4: (n, rho, R, mu, gamma) = (4, 1.0, 1.0, .01, .1)\n"
	     "                5: (n, rho, R, mu, gamma) = (5, 1.0, 1.0, .01, .1)\n"
	     "                6: (n, rho, R, mu, gamma) = (6, 1.0, 1.0, .01, .1)\n"); 

      gi.inputString(answer,"Enter amp, casenumber (amplitude, casenumber)");
      sScanF(answer,"%e %d",&amp,&casenumber);
      
      // now that we have casenumber, set the rest of the needed parameters
      real & n = rpar[1];
      real & R = rpar[2];

      real rho;

      real & mu = rpar[3];
      real & omegar = rpar[4];
      real & omegai = rpar[5];
      real & lr = rpar[6];
      real & li = rpar[7];
      real & cr = rpar[8];
      real & ci = rpar[9];
      real & dr = rpar[10];
      real & di = rpar[11];
      real & gamma = rpar[12];

      if (casenumber == 3) {
	// parameters
	n     = 3.0;
	R     = 1.0;
	rho   = 1.0;
	mu    = 0.1;
	gamma = 1.0;
	
	// solution to dispersion relation
	omegar =  4.6468563337943198e+00;
	omegai = -9.1284130939664321e-01;
	lr = 5.3143717795319771e+00;
	li = 4.3719714451399900e+00;

	cr =  6.1496830686969157e-05;
	ci =  2.5445807007528146e-04;
	dr = -9.2591940315355399e-03;
	di = -4.4862330257864754e-03;

      } else {
	printF("***WARNING*** invalid casenumber entered. Using default values instead\n");
	n     = 3.0;
	R     = 1.0;
	rho   = 1.0;
	mu    = 0.01;
	gamma = 0.1;

	// solution to dispersion relation
	omegar = .234;
	omegai = .345;
	lr = .123;
	li = .567;

	// compute constants
	cr = .153;
	ci = .634;
	dr = .637;
	di = .337;
      }

      // check if parameters agree with set parameters
      real tol = 1.0e-12;
      if (abs(nu*rho-mu) > tol) {
	printF("***WARNING*** input viscosity does not agree with parameters in casenumber\n");
      } 
      if (abs(surfaceTension-gamma) > tol) {
	printF("***WARNING*** input surface tension does not agree with parameters in casenumber\n");
      }

      // print all parameters
      printF("oscillatingBubble parameters: \n"
	     "  amp        = %9.3e \n"
	     "  casenumber = %d \n"
	     "  R          = %9.3e \n"
	     "  n          = %9.3e \n"
	     "  mu         = %9.3e \n"
	     "  gamma      = %9.3e \n"
	     "  rho        = %9.3e \n",amp,casenumber,R,n,mu,gamma,rho);

      
      // *TEST THE BESSEL FUNCTIONS OF COMPLEX ARG ***
      real fnu=1., zr,zi,jr,ji,yr,yi;
      
      zr=1.; zi=1.;
      cBesselJ(  fnu, zr, zi, jr, ji );  // eval J_fnu(z)
      printf(" *** BESSEL: fnu=%3.1f, z=(%.16e,%.16e) J=(%.16e,%.16e)\n",fnu,zr,zi,jr,ji);

      zr=1.; zi=1.;
      cBesselY(  fnu, zr, zi, yr, yi );  // eval Y_fnu(z)
      printf(" *** BESSEL: fnu=%3.1f, z=(%.16e,%.16e) Y=(%.16e,%.16e)\n",fnu,zr,zi,yr,yi);

      // * test evalOscillatingBubble solution * //
      real r = .133;

      real vrr, vri, vtr, vti, pr, pi;
      evalOscillatingBubble( r, R, (int)round(n), mu, lr,li,
			     cr,ci,dr,di,
			     vrr,vri,vtr,vti,pr,pi);

      printf(" *** OSCILLATINGBUBBLE: \n");
      printf(" vr = %23.16e + %23.16e i\n",vrr,vri);
      printf(" vt = %23.16e + %23.16e i\n",vtr,vti);
      printf(" p  = %23.16e + %23.16e i\n",pr ,pi);
      printf("--------------------------------\n");
      printf("--------------------------------\n");
      printf("--------------------------------\n\n");

      // OV_ABORT("stop for now");
      
    }

    //******************************************************************//
    else if (answer == "capillary flow")
    {
      userKnownSolution="capillaryFlow";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      // get parameters from database
      const real nu  = dbase.get<real>("nu");

      real surfaceTension;
      dbase.get<ListOfShowFileParameters >("pdeParameters")
	.getParameter("surfaceTension",surfaceTension);

      // define our needed parameters and get amp and casenumber 
      real & amp = rpar[0];
      real & k = rpar[1];
      real & R = rpar[2];
      real & mu = rpar[3];
      real & omegar = rpar[4];
      real & omegai = rpar[5];
      real & alphar = rpar[6];
      real & alphai = rpar[7];
      real & ar = rpar[8];
      real & ai = rpar[9];
      real & br = rpar[10];
      real & bi = rpar[11];
      real & cr = rpar[12];
      real & ci = rpar[13];
      real & dr = rpar[14];
      real & di = rpar[15];
      real & gamma = rpar[16];


      int casenumber;

      printF("---  capillary waves in rectangular geometry ---\n"
             "   amp        = amplitude of the motion  (solution only valid \n"
	     "                for small amp, eg. amp=1.e-4)\n"
             "   casenumber = choose parameter set to use (n, rho, H, mu, gamma)\n"
	     "                where n   = number of periods, \n"
	     "                      rho = fluid density, \n"
	     "                      H   = height of domain, \n"
	     "                      mu  = viscosity, \n"
	     "                      gamma = surface tension. \n"
             "                1: (n, rho, H, mu, gamma) = (1, 1.0, 1.0, .1, .1)\n");

      gi.inputString(answer,"Enter amp, casenumber (amplitude, casenumber)");
      sScanF(answer,"%e %d",&amp,&casenumber);
      
      // now that we have casenumber, set the rest of the needed parameters

      real rho;

      real n;
      real H = 1.;
      if (casenumber == 1) {
	omegar =  2.6327730371658089e+00;
	omegai = -2.5705601835278751e+00;
	alphar =  4.6629087846975636e+00;
	alphai = -2.8231015860806408e+00;
	ar = ( 1.6408189489495698e-01);
	ai = ( 5.7724681448045732e-02);
	br = (-7.2006869928791420e-06);
	bi = ( 1.3988177151996081e-05);
	cr = (-3.9473503009392136e-01);
	ci = ( 1.3347862201341571e-04);
	dr = (-4.9145949653947911e-05);
	di = (-1.3347862201368001e-04);
	gamma = .1;
	n = 1.;
	mu = .1;
	k = 2*Pi*n/H;
      } else {
	printF("***WARNING*** invalid casenumber entered. \n");
	OV_ABORT("stop for now");
      }

      // print all parameters
      printF("capillaryFlow parameters: \n"
	     "  amp        = %9.3e \n"
	     "  casenumber = %d \n"
	     "  H          = %9.3e \n"
	     "  k          = %9.3e \n"
	     "  mu         = %9.3e \n"
	     "  gamma      = %9.3e \n"
	     "  rho        = %9.3e \n",amp,casenumber,R,k,mu,gamma,rho);


      }
 
    else if (answer == "free surface piston") 
      {

      //
      userKnownSolution="freeSurfacePiston";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      // get parameters from database
      real & amp = rpar[0];
      real & k   = rpar[1];
      real & nu  = rpar[2];
      real & H   = rpar[3];
      real & amp1 = rpar[4];
      real & amp2 = rpar[4];

      nu = dbase.get<real>("nu");
      H  = 1.;

      printF("--- free surface piston ---\n"
	     "Interface position = yI(t) = amp sin(k t) + amp1 t^2 + amp2 t"
	     "   amp = amplitude of interface displacement "
	     "   amp1 = amplitude of t^2"
	     "   amp2 = amplitude of t"
	     "   k   = frequency of interface displacement");
      
      gi.inputString(answer,"Enter (amp,amp1,amp2,k)");
      sScanF(answer,"%e %e %e %e",&amp,&amp1,&amp2,&k);

      printF("***freeSurfacePiston: recieved (amp,k)=(%e,%e)\n",amp,k);
      
      // OV_ABORT("stop for now");
    }

    else if (answer == "parallel flow") 
    {
      userKnownSolution="parallelFlow";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      // get parameters from database
      real & amp = rpar[0];
      real & n   = rpar[1];
      real & nu  = rpar[2];
      real & H   = rpar[3];

      nu = dbase.get<real>("nu");
      H  = 1.;

      printF("--- Parallel flow with free surface ---\n"
	     "   amp = amplitude of initial "
	     "   n   = number of periods");
      
      gi.inputString(answer,"Enter amp and n (amp,n)");
      sScanF(answer,"%e %e",&amp,&n);

      printF("***parallelFlow: recieved (amp,n)=(%e,%e)\n",amp,n);
      
      // OV_ABORT("stop for now");
    }

    else
    {
      printF("unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }

  gi.unAppendTheDefaultPrompt();
  bool knownSolutionChosen = userKnownSolution!="unknownSolution";
  return knownSolutionChosen;
}
