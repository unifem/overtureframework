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

extern "C"
{
// rotating disk (SVK) exact solution:
void rotatingDiskSVK( const real & t, const int & numberOfGridPoints, real & uDisk, real & param,
		      const int & nrwk, real & rwk );
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
#define GET_VERTEX_ARRAY(x)\
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);\
    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);\
    if( !thisProcessorHasPoints )\
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

    // **** CHECK ME ****
    assert( numberOfTimeDerivatives==0 );

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    const real & amp      = rpar[0];
    const real & k        = rpar[1];
    const real & phase    = rpar[2];
    const real & H        = rpar[3];
    const real & Hbar     = rpar[4];
    const real & rho      = rpar[5];
    const real & rhoBar   = rpar[6];
    const real & lambdaBar= rpar[7];
    const real & muBar    = rpar[8];



    const real cp = sqrt((lambdaBar+2.*muBar)/rhoBar);
  
    const real sint = sin(cp*k*t+twoPi*phase);
    const real cost = cos(cp*k*t+twoPi*phase);
    const real coskHbar = cos(k*Hbar);
    const real sinkHbar = sin(k*Hbar);

    
    const real pI = -amp*(lambdaBar+2.*muBar)*cp*k*coskHbar*cost;
    
    const real yI =  amp*         sinkHbar*cost;    // interface position
    const real vI = -amp*cp*k*    sinkHbar*sint;    // interface velocity
    const real aI = -amp*SQR(cp*k)*sinkHbar*cost;   // interface acceleration
    const real p0 = pI - rho*H*aI*(1.-yI/H);        // fluid pressure at y=H 
    const real pAmp = (p0-pI)/(H-yI);

    if( t <= 2.*dt )
    {
      // Traction rate is - d(pAmp)/dt
      real yIt = vI;
      real pIt = amp*(lambdaBar+2.*muBar)*SQR(cp*k)*coskHbar*sint;
      real aIt = amp*SQR(cp*k)*cp*k*sinkHbar*sint;
      real p0t = pIt -rho*H*( aIt*(1.-yI/H) + aI*(-yIt/H) );
      
      real pAmpt = (p0t-pIt)/(H-yI) + yIt*pAmp/(H-yI);
      real s22t = -( p0t - pAmpt*H ); // s22_t = - p_t 

      printF("--INS-- userDefinedKnownSolution: bulkSolidPiston, amp=%g, k=%g, t=%9.3e\n",amp,k,t);
      printF("--INS-- vI=%g, p0=%g, pAmp=%g, pAmpt=%g, s22t=%12.6e\n",vI,p0,pAmp,pAmpt,s22t);
    }
    

    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      // const real x = xLocal(i1,i2,i3,0);
      const real y = xLocal(i1,i2,i3,1);

      ua(i1,i2,i3,uc) = 0.;
      ua(i1,i2,i3,vc) = vI;
      ua(i1,i2,i3,pc) = p0 - pAmp*(H-y);

    }
      
  }

  else 
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
// ==========================================================================================
int InsParameters::
getUserDefinedDeformingBodyKnownSolution( 
  int body,
  DeformingBodyStateOptionEnum stateOption, 
  const real t, const int grid, MappedGrid & mg, const Index &I1_, const Index &I2_, const Index &I3_, 
  realSerialArray & state )
{
  const int numberOfDimensions = mg.numberOfDimensions();
  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedKnownSolutionData") )
  {
    printf("getUserDefinedKnownSolution:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
    OV_ABORT("error");
  }
  DataBase & db =  dbase.get<DataBase >("modelData").get<DataBase>("userDefinedKnownSolutionData");

  const aString & userKnownSolution = db.get<aString>("userKnownSolution");
  const real dt = dbase.get<real>("dt");
  
  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");

  // Adjust index bounds for parallel *wdh* 2017/05/31 
  OV_GET_SERIAL_ARRAY_CONST(int,mg.mask(),mask);
  Index I1=I1_, I2=I2_, I3=I3_;
  int includeGhost=1;
  bool thisProcessorHasPoints=ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);

  if( userKnownSolution=="bulkSolidPiston" )
  {
    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    const real & amp      = rpar[0];
    const real & k        = rpar[1];
    const real & phase    = rpar[2];
    const real & H        = rpar[3];
    const real & Hbar     = rpar[4];
    const real & rho      = rpar[5];
    const real & rhoBar   = rpar[6];
    const real & lambdaBar= rpar[7];
    const real & muBar    = rpar[8];

    const real cp = sqrt((lambdaBar+2.*muBar)/rhoBar);
  
    const real sint = sin(cp*k*t+twoPi*phase);
    const real cost = cos(cp*k*t+twoPi*phase);
    const real coskHbar = cos(k*Hbar);
    const real sinkHbar = sin(k*Hbar);
    
    const real pI = -amp*(lambdaBar+2.*muBar)*cp*k*coskHbar*cost;
    
    const real yI =  amp*         sinkHbar*cost;    // interface position
    const real vI = -amp*cp*k*    sinkHbar*sint;    // interface velocity
    const real aI = -amp*SQR(cp*k)*sinkHbar*cost;   // interface acceleration
    const real p0 = pI - rho*H*aI*(1.-yI/H);        // fluid pressure at y=H 
    const real pAmp = (p0-pI)/(H-yI);

    if( t <= 2.*dt )
    {
      printF("--INS-- getUserDefinedDeformingBodyKnownSolution: bulkSolidPiston, amp=%g, k=%g, t=%9.3e\n",amp,k,t);
      printF("--INS-- vI=%g, p0=%g, pAmp=%g\n",vI,p0,pAmp);
    }
    


    if( stateOption==boundaryPosition )
    {
      state(I1,I2,I3,0)=xLocal(I1,I2,I3,0);
      state(I1,I2,I3,1)=yI;
    }
    else if( stateOption==boundaryVelocity )
    {
      state(I1,I2,I3,0)=0.;
      state(I1,I2,I3,1)=vI;
    }
    else if( stateOption==boundaryAcceleration )
    {
      state(I1,I2,I3,0)=0.;
      state(I1,I2,I3,1)=aI;
    }
    else
    {
      OV_ABORT("Unknown state option");
    }

  }
  else
  {
    printF("InsParameters::getUserDefinedDeformingBodyKnownSolution:ERROR: unknown userKnownSolution=[%s]\n",
	   (const char*)userKnownSolution);
    OV_ABORT("error");
  }
  
  return 0;
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
      "bulk solid piston", // FSI solution for a bulk solid adjacent to a fluid channel
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
    else if( answer=="bulk solid piston" )
    {
      userKnownSolution="bulkSolidPiston";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & amp      = rpar[0];
      real & k        = rpar[1];
      real & phase    = rpar[2];
      real & H        = rpar[3];
      real & Hbar     = rpar[4];
      real & rho      = rpar[5];
      real & rhoBar   = rpar[6];
      real & lambdaBar= rpar[7];
      real & muBar    = rpar[8];

      printF("--- Exact solution for a bulk elastic solid adjacent to a fluid chamber ---\n"
             "   y_I(t) = A sin(k Hbar) * cos( cp*k*t + 2*pi*phase )\n"
             " Parameters: \n"
             " amp : amplitude of the interface motion \n"
             " k: wave number in solid domain (y-direction)\n"
             " H,Hbar: Height of fluid and solid domains\n"
             " phase: phase for time dependence\n"
             " rhoBar,lambaBar,muBar : solid density and Lame parameters\n"
	);
      gi.inputString(answer,"Enter amp, k,phase,H,Hbar,rho,lambdaBar,muBar,rhoBar");
      sScanF(answer,"%e %e %e %e %e %e %e %e %e",&amp,&k,&phase,&H,&Hbar,&rho,&rhoBar,&lambdaBar,&muBar);

      // The waveform for the exact solution is defined through a TimeFunction:
      if( !db.has_key("timeFunctionBSP") )
        db.put<TimeFunction>("timeFunctionBSP");

      TimeFunction & timeFunction = db.get<TimeFunction>("timeFunctionBSP");
      real rampStart=0., rampEnd=1.; // Ramp from 0 to 1,
      real rampStartTime=.1, rampEndTime=.6;
      int rampOrder=3;
      timeFunction.setRampFunction( rampStart,rampEnd,rampStartTime,rampEndTime,rampOrder );

      // ** FINISH ME**      

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
