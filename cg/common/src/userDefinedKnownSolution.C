#include "Parameters.h"
#include "FlowSolutions.h"
#include "GenericGraphicsInterface.h"
#include "FluidPiston.h"
#include "PistonMotion.h"
#include "ParallelUtility.h"
#include "TravelingWaveFsi.h"
#include "TimeFunction.h"

#include "../moving/src/BeamModel.h"

// Sometimes we need a single and persistent FlowSolutions object: (e.g. FSI)
static FlowSolutions *pFLowSolutions=NULL;

static int rc=-1, uc=-1, vc=-1, tc=-1;
static int u1c=-1, u2c=-1, v1c=-1, v2c=-1, s11c=-1, s12c=-1, s21c=-1, s22c=-1;
     


#define rotatingDiskSVK EXTERN_C_NAME(rotatingdisksvk)
#define evalFibShearSolid EXTERN_C_NAME(evalfibshearsolid)
#define evalFibShearSolidFull EXTERN_C_NAME(evalfibshearsolidfull)

extern "C"
{
  // rotating disk (SVK) exact solution:
  void rotatingDiskSVK( const real & t, const int & numberOfGridPoints, real & uDisk, real & param,
                        const int & nrwk, real & rwk );

  // fsi shear motion exact solution (implemented in cBessel.f)
  void evalFibShearSolid( const real & ksr, const real & ksi,
			  const real & ar, const real & ai,
			  const real & br, const real & bi,
			  const real & y, 
			  real & ur, real & ui, real & uyr, real & uyi);
  void evalFibShearSolidFull( const real & ksr, const real & ksi,
                              const real & ar, const real & ai,
                              const real & br, const real & bi,
                              const real & y, const real & t,
                              real & ur, real & ui, 
                              real & vr, real & vi, 
                              real & uyr, real & uyi,
                              const real & omegar, const real & omegai);
}


#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

// ==========================================================================================
/// \brief  Evaluate a user defined known solution.
///
/// \param numberOfTimeDerivatives (input) : evaluate this many time-derivatives of the solution.
///     Normally  numberOfTimeDerivatives=0, but it can be 1 when the known solution is used
//      to define boundary conditions for cgins.
// ==========================================================================================
int Parameters::
getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, RealArray & ua, 
			    const Index & I1, const Index &I2, const Index &I3, int numberOfTimeDerivatives /* = 0 */ )
{

  // printF("--PAR-- getUserDefinedKnownSolution: START---\n");

  MappedGrid & mg = cg[grid];

  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedKnownSolutionData") )
  {
    printf("getUserDefinedKnownSolution:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
    Overture::abort("error");
  }
  DataBase & db =  dbase.get<DataBase >("modelData").get<DataBase>("userDefinedKnownSolutionData");

  const aString & userKnownSolution = db.get<aString>("userKnownSolution");

  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");
  
  if( userKnownSolution=="pistonMotion" ) // *NEW WAY*
  {
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    FlowSolutions flowSolutions;
    int ipar[]={  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("tc"),-1,-1 };
    real rpar2[]={ dbase.get<real >("gamma"), dbase.get<real >("Rg"),t  };  // 

    assert( dbase.has_key("pistonMotion") );
    PistonMotion & pistonMotion = dbase.get<PistonMotion>("pistonMotion");
    flowSolutions.getFlowSolution(pistonMotion, cg,grid,ua,ipar,rpar2,I1,I2,I3);

  }
  else if( userKnownSolution=="specifiedPiston" )  // *OLD WAY*
  {

//   const int rc=ipar[0];
//   const int uc=ipar[1];
//   const int vc=ipar[2];
//   const int tc=ipar[3];
//   const int pc=ipar[4];  // fill in if >=0 
//   const int sc=ipar[5];  // fill in if >=0 
  
//   const real gamma   =rpar[0];
//   const real Rg      =rpar[1];
//   const real rho0    =rpar[2];
//   const real u0      =rpar[3];
//   const real v0      =rpar[4];
//   const real p0      =rpar[5];
//   const real t       =rpar[6];
//   const real ag      =rpar[7];  // g(t)= ap*t^pg  (piston with specfied motion)
//   const real pg      =rpar[8];
//   const real mass    =rpar[9];  // for hydrodynamically forced piston
//   const real height  =rpar[10]; // for hydrodynamically forced piston

    FlowSolutions flowSolutions;

    int ipar[]={  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("tc"),-1,-1 }; // 

    const real rho0=rpar[2]; // 1.4;  *wdh* 101020
    const real v0=0.;
    const real p0=rpar[3];   // 1;
    const real angle=rpar[4];  // angle (in degrees0 for a rotated piston

    const real te0=p0/rho0;
    const real a0 = sqrt( dbase.get<real >("gamma")*p0/rho0);
    const real u0 = 1.5*a0;

    real mass=1., height=1.;

    real ag=rpar[0], pg=rpar[1];
    
    real rpar2[]={ dbase.get<real >("gamma"), dbase.get<real >("Rg"), rho0,u0,v0, p0,t,ag,pg,mass,height,angle}; //

    // evaluate the known solution for flow in an expanding channel
    flowSolutions.getFlowSolution(FlowSolutions::specifiedPistonMotion, cg,grid,ua,ipar,rpar2,I1,I2,I3);
  }
  else if( userKnownSolution=="forcedPiston" )  // *OLD WAY*
  {
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    FlowSolutions flowSolutions;

    int ipar[]={  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("tc"),-1,-1 }; // 

    // do this for now -- fix this ----------------------------------------- ****************************
    const real rho0=1.4; 
    const real v0=0.;
    const real p0=1;
    const real te0=p0/rho0;
    const real a0 = sqrt( dbase.get<real >("gamma")*p0/rho0);
    const real u0 = 1.5*a0;

    real pg=4., ag=-.5/pg;  // not used
    
    real mass=1., height=1.;
    mass = rpar[0], height=rpar[1];
     
    real rpar2[]={ dbase.get<real >("gamma"), dbase.get<real >("Rg"), rho0, u0,v0, p0,t,ag,pg,mass,height}; //

    // evaluate the known solution for forcedPistonMotion
    flowSolutions.getFlowSolution(FlowSolutions::forcedPistonMotion, cg,grid,ua,ipar,rpar2,I1,I2,I3);
  }
  else if( userKnownSolution=="obliqueShockFlow" )
  {
    // *** Oblique Shock Solution ***
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    FlowSolutions flowSolutions;

    int ipar[]={  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("wc"), dbase.get<int >("tc"),-1,-1 }; // 

    rpar[19]=t;
    // evaluate the known solution for an oblique shock flow
    flowSolutions.getFlowSolution(FlowSolutions::obliqueShockFlow, cg,grid,ua,ipar,rpar,I1,I2,I3);
  }
  else if( userKnownSolution=="superSonicExpandingFlow" )
  {
    // evaluate the known solution for the supersonic expanding flow
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    FlowSolutions flowSolutions;

    // int ipar[]={  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("tc"),-1,-1 }; // 
    ipar[0]=dbase.get<int >("rc");
    ipar[1]=dbase.get<int >("uc");
    ipar[2]=dbase.get<int >("vc");
    ipar[3]=dbase.get<int >("wc");
    ipar[4]=dbase.get<int >("tc");

    // evaluate the known solution for flow in an expanding channel
    flowSolutions.getFlowSolution(FlowSolutions::superSonicExpandingFlow, cg,grid,ua,ipar,rpar,I1,I2,I3);

  }
//   else if( userKnownSolution==deformingDiffuser )
//   {
//     // *** Deforming Diffuser (solid machanics) ***

//     FlowSolutions flowSolutions;

//     OV_ABORT("finish me");

//     int ipar[]={  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("wc"), dbase.get<int >("tc"),-1,-1 }; // 

//     rpar[19]=t;
//     // evaluate the known solution for the deforming diffuser
//     flowSolutions.getFlowSolution(FlowSolutions::deformingDiffuser, cg,grid,ua,ipar,rpar,I1,I2,I3);

//   }
  else if( userKnownSolution=="shockElasticPiston" )
  {
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    int debug = t<=0.;

    FlowSolutions flowSolutions;

    int domain = ipar[0];   // 1=solid, 2=fluid

    real xShock = rpar[0];
    real Mshock = rpar[1];
    real gamma  = rpar[2];
    real rs0    = rpar[3];  // rhoSolid 
    real as0    = rpar[4];  // cp 
    real lambda = rpar[5];  
    real mu     = rpar[6]; 
    
    
    // here are defaults -- these are changed below
    int rc=0, uc=1, vc=2, tc=3;
    int v1c=0, v2c=1, s11c=2, s12c=3, s21c=4, s22c=5, u1c=6, u2c=7;
    real Rg=1.;

    if( domain==1 )
    {
      // get answer for the solid domain
      v1c = dbase.get<int >("v1c");
      v2c = dbase.get<int >("v2c");
      // u1c = dbase.get<int >("u1c"); // fix me 
      // u2c = dbase.get<int >("u2c");
      u1c = dbase.get<int >("uc");
      u2c = dbase.get<int >("vc");

      s11c = dbase.get<int >("s11c");
      s12c = dbase.get<int >("s12c");
      s21c = dbase.get<int >("s21c");
      s22c = dbase.get<int >("s22c");

    }
    else
    {
      // get answer for the fluid domain
      rc = dbase.get<int >("rc");
      uc = dbase.get<int >("uc");
      vc = dbase.get<int >("vc");
      tc = dbase.get<int >("tc");
      gamma = dbase.get<real>("gamma");
      Rg = dbase.get<real>("Rg");
    }
    

    real gam=gamma;  
     
    //real  Shock=.25;  //% location of the shock
    //real Mshock=2.;   //% shock Mach number = |S|/a1;

    //% state ahead of the shock (left)
    real r1=1., p1=r1/gam, a1=1., v1=0., e1=p1/(gam-1.)+.5*r1*v1*v1; 

    // set the pressure offset
    real pOffset = p1; 
    if( dbase.has_key("boundaryForcePressureOffset") ) 
      dbase.get<real>("boundaryForcePressureOffset")=p1;

    real M=Mshock;   //% shock Mach number   M = |S|/a1;
    real S = -M*a1;  //% shock speed
    real shockSpeed=S; 

    //% Here are the normal shock relations: 
    real Mr = (gam+1.)*M*M/( (gam-1.)*M*M + 2.);
    real r2=r1*Mr;
    real p2 = p1*( 1.+ (2.*gam/(gam+1))*(M*M-1.) );
    real v2 = S + (v1-S)/Mr; 
    real a2=sqrt(gam*p2/r2); 
    real e2=p2/(gam-1.)+.5*r2*v2*v2;


    if( debug )
    {
      printf("SEP:fluid:IC: [r1,v1,p1,a1]=[%12.5e,%12.5e,%12.5e,%12.5e]\n",r1,v1,p1,a1);
      printf("SEP:fluid:IC: [r2,v2,p2,a2]=[%12.5e,%12.5e,%12.5e,%12.5e]\n",r2,v2,p2,a2);

      printf("SEP:fluid:IC: S=%4.2f\n",S);
      printf("          [r*v]/[r] - S = %8.2e\n",(r2*v2-r1*v1)/(r2-r1)-S);
      printf("          [r*v^2+p]/[r*v] - S = %8.2e\n",(r2*v2*v2+p2-(r1*v1*v1+p1))/(r2*v2-r1*v1)-S);
      printf("          [E*v+p*v]/[E] - S = %8.2e\n",(e2*v2+p2*v2-(e1*v1+p1*v1))/(e2-e1)-S);
    }


    // Now compute the solution to the fluid-solid Riemann problem (see fsi/notes.pdf)
    real us0=0., vs0=0., sigmas0=0.;  // initial solid state

    real rfsr,vfsr,pfsr, efsr, afsr, vsfsr, sigmasfsr, Mfsr, Sfsr;


    FluidPiston fp;
    real solid[4]={rs0,vs0,sigmas0,as0};  // 
    real fluid[5]={r2,v2,p2,gamma,pOffset};  // 
    real fsr[10];
      
    fp.fluidSolidRiemannProblem( solid, fluid, fsr );

    rfsr=fsr[0];
    vfsr=fsr[1];
    pfsr=fsr[2];
    Mfsr=fsr[3];   // FSR Mach number 
    Sfsr=fsr[4];   // reflected shock speed
    
    efsr = pfsr/(gam-1.)+.5*rfsr*vfsr*vfsr; 

    afsr=sqrt(gam*pfsr/rfsr);
    vsfsr=vfsr;
    sigmasfsr=-(pfsr-pOffset);

    const int ndip=20;
    int ip[ndip];

    int m=0;
    ip[m]=domain; m++;  // 1=solid, 2=fluid
    ip[m]=rc;     m++;
    ip[m]=uc;     m++;
    ip[m]=vc;     m++;
    ip[m]=tc;     m++;

    ip[m]=v1c;    m++; 
    ip[m]=v2c;    m++; 

    ip[m]=s11c;   m++; 
    ip[m]=s12c;   m++;  
    ip[m]=s21c;   m++;  
    ip[m]=s22c;   m++;  

    ip[m]=u1c;    m++; 
    ip[m]=u2c;    m++;  
  
    assert( m<=ndip );

    const int ndrp=30;
    real rp[ndrp];

    m=0;
    rp[m]=t;     m++;

    rp[m]=gamma; m++;
    rp[m]=Rg;    m++;

    rp[m]=r1;    m++;
    rp[m]=v1;    m++;
    rp[m]=p1;    m++;

    rp[m]=r2;    m++;
    rp[m]=v2;    m++;
    rp[m]=p2;    m++;


    rp[m]=xShock; m++;
    rp[m]=shockSpeed; m++;  // incident shock speed

    rp[m]=pOffset; m++;

    rp[m]=rs0;   m++;  // rhoSolid 
    rp[m]=vs0;   m++; 
    rp[m]=sigmas0; m++; 
    rp[m]=as0;   m++;   // cp 

    rp[m]=rfsr;  m++;    // state from fluid-solid-Riemann (FSR) problem 
    rp[m]=vfsr;  m++; 
    rp[m]=pfsr;  m++; 
    rp[m]=vsfsr; m++; 
    rp[m]=sigmasfsr;  m++;
    rp[m]=Mfsr;  m++;  
    rp[m]=Sfsr;  m++;  // reflected shock speed
    rp[m]=lambda;m++;  
    rp[m]=mu;    m++;  

    assert( m<=ndrp );

    // evaluate the known solution for flow in an expanding channel
    flowSolutions.getFlowSolution(FlowSolutions::shockElasticPiston, cg,grid,ua,ip,rp,I1,I2,I3);
  }
  else if( userKnownSolution=="rotatingDisk" )
  {
    // ---- return the exact solution for the rotating disk ---
    printF(" userDefinedKnownSolution: rotatingDisk: t=%9.3e\n",t);
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    // Here are the comopnents for displacement velocity and stress
    int v1c = dbase.get<int >("v1c");
    int v2c = dbase.get<int >("v2c");

    int u1c = dbase.get<int >("uc");
    int u2c = dbase.get<int >("vc");

    int s11c = dbase.get<int >("s11c");
    int s12c = dbase.get<int >("s12c");
    int s21c = dbase.get<int >("s21c");
    int s22c = dbase.get<int >("s22c");

    assert( v1c>=0 && u1c>=0 && s22c >=0 );

    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

    const realArray & center = mg.center();
    RealArray & u = ua;

    
    // tDisk : solution has been computed at this time
    real & tDisk = db.get<real>("tDisk");

    const int & numberOfGridPoints = db.get<int>("numberOfGridPointsDisk");

    const real & omega = db.get<real>("omegaDisk");

    const real & innerRadius = db.get<real>("innerRadiusDisk");
    const real & outerRadius = db.get<real>("outerRadiusDisk");


    const real dr = (outerRadius-innerRadius)/(numberOfGridPoints-1.);

    RealArray & uDisk = db.get<RealArray>("uDisk"); // exact solution is stored here

    if( t!=tDisk )
    {
      // Compute the exact solution at time t
      tDisk=t;


      int numberOfComponents=8;  // number of components needed in 1D solution: we compute u, v, and sigma 
      if( uDisk.getLength(0)!=numberOfGridPoints )
      {
	uDisk.redim(numberOfGridPoints,numberOfComponents);
      }
      
      // call the fortran routine here to evaluate the solution at time t and return the result in uDisk     

      int nrwk=10*numberOfGridPoints+6;
      RealArray rwk(nrwk);
      int npar=10;
      RealArray param(npar);

      param(0)=innerRadius;
      param(1)=outerRadius;
      param(2)=omega;
      param(3)=1.;   // this is lambda
      param(4)=1.;   // this mu

      rotatingDiskSVK( t, numberOfGridPoints, *uDisk.getDataPointer(), *param.getDataPointer(), nrwk, *rwk.getDataPointer() );

    }
    

    const real twopi=6.283185307179586;
    const real x0=0., y0=0.;  // center of the disk 

    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      // Reference coordinates:
      real x= center(i1,i2,i3,0);
      real y= center(i1,i2,i3,1);

      real r = sqrt( SQR(x-x0) + SQR(y-y0) );

      // closest point in uDisk less than or equal to r:
      int i = int( (r-innerRadius)/dr );  
      i = max(0,min(i,numberOfGridPoints-2));
      
      // linear interpolation
      real alpha = (r-innerRadius)/dr - i;
      real c1=1.-alpha, c2=alpha;

      // radial and angular displacements
      real w=c1*uDisk(i,0)+c2*uDisk(i+1,0);
      real p=c1*uDisk(i,1)+c2*uDisk(i+1,1);

      // angular position
      real theta=0.;
      if (r>0.)
      {
        if (y<0.)
        {
          theta=twopi-acos((x-x0)/r);
        }
        else
        {
          theta=acos((x-x0)/r);
        }
      }

//      printF("rotatingDisk: i=%i, uDisk(i,0)=%9.3e, uDisk(i+1,0)=%9.3e, uDisk(i,1)=%9.3e, uDisk(i+1,1)=%9.3e\n",i,uDisk(i,0),uDisk(i+1,0),uDisk(i,1),uDisk(i+1,1));
//      printF("rotatingDisk: i=%i, alpha=%9.3e, c1=%9.3e, c2=%9.3e, w=%9.3e, p=%9.3e, theta=%9.3e\n",i,alpha,c1,c2,w,p,theta);

      // displacements
      real thbar=theta+p;
      u(i1,i2,i3,u1c)=(r+w)*cos(thbar)-x;
      u(i1,i2,i3,u2c)=(r+w)*sin(thbar)-y;

      // velocities
      real wt=c1*uDisk(i,2)+c2*uDisk(i+1,2);
      real pt=c1*uDisk(i,3)+c2*uDisk(i+1,3);
      u(i1,i2,i3,v1c)=wt*cos(thbar)-pt*(r+w)*sin(thbar);
      u(i1,i2,i3,v2c)=wt*sin(thbar)+pt*(r+w)*cos(thbar);

      // stresses
      real pbar11=c1*uDisk(i,4)+c2*uDisk(i+1,4);
      real pbar12=c1*uDisk(i,5)+c2*uDisk(i+1,5);
      real pbar21=c1*uDisk(i,6)+c2*uDisk(i+1,6);
      real pbar22=c1*uDisk(i,7)+c2*uDisk(i+1,7);
      real pt11=pbar11*cos(thbar)-pbar12*sin(thbar);
      real pt12=pbar11*sin(thbar)+pbar12*cos(thbar);
      real pt21=pbar21*cos(thbar)-pbar22*sin(thbar);
      real pt22=pbar21*sin(thbar)+pbar22*cos(thbar);
      u(i1,i2,i3,s11c)=cos(theta)*pt11-sin(theta)*pt21;
      u(i1,i2,i3,s12c)=cos(theta)*pt12-sin(theta)*pt22;
      u(i1,i2,i3,s21c)=sin(theta)*pt11+cos(theta)*pt21;
      u(i1,i2,i3,s22c)=sin(theta)*pt12+cos(theta)*pt22;

    }


  }

  else if( userKnownSolution=="rotatingElasticDiskInFluid" )
  {
    // ---------------------------------------------------------------------------
    // ---- return the exact solution for the rotating elastic disk in a fluid ---
    // ---------------------------------------------------------------------------
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    const int domain = ipar[0];   // 1=solid, 2=fluid

    if( false )
      printF("-- userDefinedKnownSolution: rotatingElasticDiskInFluid: t=%9.3e, domain=%i, grid=%i.\n",t,
	     domain,grid);

    // -- pass parameters to FlowSolutions: 
    const int ndip=15, ndrp=15;
    int ip[ndip];
    real rp[ndrp];
    
    // component numbers for fluid and solid domains are stored as global variables (to this file)

    // assert( rc>=0 && uc>=0 && vc>=0 && tc>=0 );
    // assert( u1c>=0 && u2c>=0 && v1c>=0 && v2c>=0 && s11c>=0 && s12c>=0 && s21c>=0 && s22c>=0 );

    int mi=0;
    ip[mi]=domain; mi++;
    ip[mi]=rc; mi++;
    ip[mi]=uc; mi++;
    ip[mi]=vc; mi++;
    ip[mi]=tc; mi++;

    ip[mi]=u1c; mi++;
    ip[mi]=u2c; mi++;
    ip[mi]=v1c; mi++;
    ip[mi]=v2c; mi++;
    ip[mi]=s11c; mi++;
    ip[mi]=s12c; mi++;
    ip[mi]=s21c; mi++;
    ip[mi]=s22c; mi++;
    assert( mi<=ndip );
     
    int mr=0;
    rp[mr]=t;   mr++;
    rp[mr]=db.get<real>("innerRadiusDisk");   mr++;
    rp[mr]=db.get<real>("outerRadiusDisk");   mr++;
    rp[mr]=db.get<real>("omegaDisk");   mr++;
    rp[mr]=db.get<real>("lambdas");   mr++;
    rp[mr]=db.get<real>("mus");   mr++;
    rp[mr]=db.get<real>("outerRadiusFluid");   mr++;
    rp[mr]=db.get<real>("rho0");   mr++;
    rp[mr]=db.get<real>("pOffset");   mr++;
    rp[mr]=db.get<real>("gamma");   mr++;
    rp[mr]=db.get<real>("Rg");   mr++;
    rp[mr]=db.get<real>("gridRatio");   mr++;
    assert( mr<=ndrp );
     
    // We need a single and persistent FlowSolutions object for this flow solution
    if( pFLowSolutions==NULL )
    { 
      pFLowSolutions = new FlowSolutions;  // who will delete this ??
    }
    
    assert( pFLowSolutions!=NULL );
    FlowSolutions & flowSolutions = *pFLowSolutions;
    
    flowSolutions.getRotatingElasticDiskInFluid( FlowSolutions::rotatingElasticDiskInFluid,
						 cg, grid, ua, ip,rp,I1,I2,I3);

  }

  else if( userKnownSolution=="uniformFlowINS" )
  {
    // for testing with INS
    assert( numberOfTimeDerivatives==0 );  // only this case implemented so far

    const int pc = dbase.get<int >("pc");
    const int uc = dbase.get<int >("uc");
    const int vc = dbase.get<int >("vc");
    const int wc = dbase.get<int >("wc");

    assert( pc>=0 && uc>=0 && vc>=0 );
    
    ua(I1,I2,I3,pc)=0.;
    ua(I1,I2,I3,uc)=1.+t;
    ua(I1,I2,I3,vc)=2.+t;
    if( mg.numberOfDimensions()==3 )
      ua(I1,I2,I3,wc)=3.+t;
     

  }
  else if( userKnownSolution=="travelingWaveFSIfluid" ||
           userKnownSolution=="travelingWaveFSIsolid" )
  {
    // -- evaluate the FSI traveling wave solution ---
    TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

    if( userKnownSolution=="travelingWaveFSIfluid" )
    {
      travelingWaveFsi.getExactFluidSolution( ua, t, mg, I1, I2, I3, numberOfTimeDerivatives );
    }
    else
    {
      travelingWaveFsi.getExactSolidSolution( ua, t, mg, I1, I2, I3, numberOfTimeDerivatives );
    }
    
  }

  else
  {
    printF("getUserDefinedKnownSolution:ERROR: unknown value for userDefinedKnownSolution=%s\n",
	   (const char*)userKnownSolution);
    OV_ABORT("ERROR");
  }
  
  return 0;
}


// ===================================================================================================================
/// \brief Return proerties of a known solution for rigid-body motions
/// \param body (input) : body number
/// \param t (input) : time
/// \return value (output) : 1=solution was found, 0=solution was not found 
// ===================================================================================================================
int Parameters::
getUserDefinedKnownSolutionRigidBody( int body, real t, 
				      RealArray & xCM      /* = Overture::nullRealArray() */, 
				      RealArray & vCM      /* = Overture::nullRealArray() */,
				      RealArray & aCM      /* = Overture::nullRealArray() */,
				      RealArray & omega    /* = Overture::nullRealArray() */, 
				      RealArray & omegaDot /* = Overture::nullRealArray() */ )
{

  const KnownSolutionsEnum & knownSolution = dbase.get<KnownSolutionsEnum >("knownSolution");

  printF("Parameters::getKnownSolutionRigidBody:ERROR: unknown knownSolution=%i\n", (int)knownSolution);
  OV_ABORT("ERROR");

  return 0;
}

// Macro to get the vertex array
#define GET_VERTEX_ARRAY(x)                                     \
mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);       \
OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);                  \
if( !thisProcessorHasPoints )                                   \
  return 0; // no points on this processor

// ==========================================================================================
/// \brief Return the state of a known solution for a deforming body
/// 
/// \param body (input) : body number
/// \param stateOption (input) : specify which information to return
/// \param t (input) : time to evaluate the solution at 
/// \param grid, mg, I1,I2,I3,C (input) :  assign values to state(I1,I2,I3,C)
/// \param state (output) : return results here
/// \return (output) : 1=solution was found, 0=solution was not found 
// ==========================================================================================
int Parameters::
getUserDefinedDeformingBodyKnownSolution( 
  int body,
  DeformingBodyStateOptionEnum stateOption, 
  const real t, const int grid, MappedGrid & mg, const Index &I1_, const Index &I2_, const Index &I3_, 
  const Range & C, realSerialArray & state )
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
    // ---- return the exact solution for the FSI INS+elastic piston ---
    //     y_I(t) = F(t + Hbar/cp) - F(t - Hbar/cp)
    //        F(z) = amp * R(z) * sin( 2*Pi*k(t-t0) )
    //        R(z) = ramp function that smoothly transitions from 0 to 1 

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);
    const real & H        = rpar[0];
    const real & Hbar     = rpar[1];
    const real & rho      = rpar[2];
    const real & rhoBar   = rpar[3];
    const real & lambdaBar= rpar[4];
    const real & muBar    = rpar[5];

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

    
    if( t <= 2.*dt )
    {
      printF("--INS-- getUserDefinedDeformingBodyKnownSolution: bulkSolidPiston, t=%9.3e fm=%9.3e fp=%9.3e\n",
             t,fm,fp);
    }

    const int c0=C.getBase(), c1=c0+1;
    if( stateOption==boundaryPosition )
    {
      state(I1,I2,I3,c0)=xLocal(I1,I2,I3,0);
      state(I1,I2,I3,c1)=yI;
    }
    else if( stateOption==boundaryVelocity )
    {
      state(I1,I2,I3,c0)=0.;
      state(I1,I2,I3,c1)=vI;
    }
    else if( stateOption==boundaryAcceleration )
    {
      state(I1,I2,I3,c0)=0.;
      state(I1,I2,I3,c1)=aI;
    }
    else if( stateOption==boundaryTraction )
    {
      state(I1,I2,I3,c0)=0.;
      state(I1,I2,I3,c1)=-pI;
    }
    else if( stateOption==boundaryTractionRate )
    {
      state(I1,I2,I3,c0)=0.;
      state(I1,I2,I3,c1)=-pIt;
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
    
    // uI = uI(t) =  interface displacement in the radial direction 
    // eval uI and vI = uI_t 
    real uI,vI,aI;
    TimeFunction & bsp = db.get<TimeFunction>("timeFunctionREP");
    bsp.eval(t, uI,vI );  // fmd = d(fm(xi))/d(xi)
    bsp.evalDerivative(t, aI, 2 ); // 2 derivatives 

    if( t <= 2.*dt )
    {
      printF("--DS-- getUserDefinedDeformingBodyKnownSolution: radialElasticPiston, t=%9.3e uI=%9.3e vI=%9.3e Rbar=%5.3f\n",
             t,uI,vI,Rbar);
    }

    // *** CHECK ME *****

    real kr=k*Rbar;
    real jnkr = jn(1,kr);
    real jnkrp = .5*k*(jn(0,kr)-jn(2,kr));  // Jn' = .5*( J(n-1) - J(n+1) )

    real ur = uI*jnkr;        // radial displacment in solid 
    real vr = vI*jnkr;        // radial velocity in solid 
    real ar = aI*jnkr;        // radial acceleration in solid 

    real RI = Rbar+ur; // interface radius 
    real sigmarr,sigmart,sigmatt;
    if( stateOption==boundaryTraction )
    {
      real urr = uI*jnkrp;      // r-derivative of the radial displacement
        
      sigmarr = (lambdaBar+2.*muBar)*urr + lambdaBar*ur/Rbar;
      sigmart=0.;
      sigmatt = lambdaBar*urr  + (lambdaBar+2.*muBar)*ur/Rbar;
    }
    else if( stateOption==boundaryTractionRate )
    {
      real vrr = vI*jnkrp;      // r-derivative of the radial velocity
        
      sigmarr = (lambdaBar+2.*muBar)*vrr + lambdaBar*vr/Rbar;
      sigmart=0.;
      sigmatt = lambdaBar*vrr  + (lambdaBar+2.*muBar)*vr/Rbar;
    }
    

    const int c0=C.getBase(), c1=c0+1;
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      // Reference coordinates for solid or grid positions for the fluid -- we only need angle theta
      real x= xLocal(i1,i2,i3,0);
      real y= xLocal(i1,i2,i3,1);
      real r = sqrt( SQR(x) + SQR(y) );
      real ct=x/r, st=y/r;

      // Normal is [cos(theta), sin(theta)]

      if( stateOption==boundaryPosition )
      {
        state(i1,i2,i3,c0)=RI*ct;
        state(i1,i2,i3,c1)=RI*st;
      }
      else if( stateOption==boundaryVelocity )
      {
        state(i1,i2,i3,c0)=vr*ct;
        state(i1,i2,i3,c1)=vr*st;
      }
      else if( stateOption==boundaryAcceleration )
      {
        state(i1,i2,i3,c0)=ar*ct;
        state(i1,i2,i3,c1)=ar*st;
      }
      else if( stateOption==boundaryTraction )
      {
        // traction: 
        //       Sigma(r,theta). nv = traction(r,theta) = [tr, tt] , nv=[cos,sin]
        //  traction(x,y) = tr*rHat + tt*thetaHat
        //                = tr*[cos,sin] + tt*[-sin,cos]
        // real tr = sigmarr*ct + sigmart*st;
        // real tt = sigmart*ct + sigmatt*st;
	real s11 = sigmarr*ct*ct - 2.*sigmart*ct*st + sigmatt*st*st;
	real s12 = sigmarr*ct*st + sigmart*(ct*ct-st*st) - sigmatt*ct*st ;
	real s21 = s12;
	real s22 = sigmarr*st*st + 2.*sigmart*ct*st + sigmatt*ct*ct;
        state(i1,i2,i3,c0)= s11*ct + s12*st;
        state(i1,i2,i3,c1)= s21*ct + s22*st;

      }
      else if( stateOption==boundaryTractionRate )
      {
        // traction-rate: 
	real s11 = sigmarr*ct*ct - 2.*sigmart*ct*st + sigmatt*st*st;
	real s12 = sigmarr*ct*st + sigmart*(ct*ct-st*st) - sigmatt*ct*st ;
	real s21 = s12;
	real s22 = sigmarr*st*st + 2.*sigmart*ct*st + sigmatt*ct*ct;
        state(i1,i2,i3,c0)= s11*ct + s12*st;
        state(i1,i2,i3,c1)= s21*ct + s22*st;

        // real tr = sigmarr*ct + sigmart*st;
        // real tt = sigmart*ct + sigmatt*st;
        // state(i1,i2,i3,c0)= tr*ct - tt*st;
        // state(i1,i2,i3,c1)= tr*st + tt*ct;
        // state(i1,i2,i3,c0)= (sigmarr*ct + sigmart*st);
        // state(i1,i2,i3,c1)= (sigmart*ct + sigmatt*st);

      }
      else
      {
        OV_ABORT("Unknown state option");
      }
    }
    
  }
  else if ( userKnownSolution=="fibShear" ) {
    // ---- return the exact solution for fib shear solution ----

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
    real & mu = rpar[15];

    // -- we could avoid building the vertex array on Cartesian grids ---
    GET_VERTEX_ARRAY(xLocal);

    printF("-- getUserDefinedDeformingBodyKnownSolution: fibShear, t=%9.3e\n",t);

    const int c0=C.getBase(), c1=c0+1;
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Reference coordinates for solid or grid positions for the fluid -- we only need angle theta
	real x= xLocal(i1,i2,i3,0);
	real y= xLocal(i1,i2,i3,1);

        real ur,ui,vr,vi,uyr,uyi;
	// evalFibShearSolid(ksr,ksi,ar,ai,br,bi,y,ur,ui,uyr,uyi);
        // get the time dependent solution
        evalFibShearSolidFull(ksr,ksi,ar,ai,br,bi,y,t,ur,ui,vr,vi,uyr,uyi,omegar,omegai);

	if( stateOption==boundaryPosition )
	  {
	    state(i1,i2,i3,c0)=x+amp*ur;
	    state(i1,i2,i3,c1)=0.;
	  }
	else if( stateOption==boundaryVelocity )
	  {
	    state(i1,i2,i3,c0)=amp*vr;
	    state(i1,i2,i3,c1)=0.;
	  }
	else if( stateOption==boundaryAcceleration )
	  {
	    state(i1,i2,i3,c0)=amp*(omegai*vr+omegar*vi);
	    state(i1,i2,i3,c1)=0.;
	  }
	else if( stateOption==boundaryTraction )
	  {
	    state(i1,i2,i3,c0)= amp*mu*uyr;
	    state(i1,i2,i3,c1)= 0.;
	  }
	else if( stateOption==boundaryTractionRate )
	  {
	    // traction-rate: 
	    state(i1,i2,i3,c0)= amp*mu*(omegai*uyr+omegar*uyi);
	    state(i1,i2,i3,c1)= 0.;
	  }
	else
	  {
	    OV_ABORT("Unknown state option");
	  }
      }


  }
  else
  {
    return 0;  // Not found
  }
  

  return 1;   // solution was found
}



//\begin{>>MovingGridsSolverInclude.tex}{\subsection{updateUserDefinedMotion}} 
int Parameters::
updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg)
// ==========================================================================================
// /Description: 
//   This function is called to set the user defined know solution.
// 
// /Return value: >0 : known solution was chosen, 0 : no known solution was chosen
//\end{MovingGridsSolverInclude.tex}  
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
      "piston motion",              // *new* way
      "specified piston motion",    // old way
      "forced piston motion",       // old way
      "oblique shock flow",
      "supersonic flow in an expanding channel",
      "exact solution from a file",
      "shock elastic piston",
      "rotating disk",  // for cgsm SVK model
      "uniform flow INS", // for testing INS    
      "rotating elastic disk in a fluid",   // FSI exact solution
      "FSI traveling wave solution fluid",
      "FSI traveling wave solution solid",
      "bulk solid piston",  // for INS+SM exact solution
      "radial elastic piston", // FSI : INS+SM
      "shearing fluid and elastic solid", // FSI : INS+SM
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
    else if( answer=="piston motion" ) // *NEW WAY* using PistonMotion class
    {
      userKnownSolution="pistonMotion";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
      
      if( !dbase.has_key("pistonMotion") )
      {
        dbase.put<PistonMotion>("pistonMotion",PistonMotion());
      }
      PistonMotion & pistonMotion = dbase.get<PistonMotion>("pistonMotion");
      pistonMotion.update(gi);

    }
    else if( answer=="specified piston motion" )  // **OLD WAY**
    {
      userKnownSolution="specifiedPiston"; 
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time

      rpar[1]=4.; rpar[0]=-.5/rpar[1];   // default : p=4, ap=-.5/p

      printF("specified piston motion: x(t) = a*t^p\n");
      gi.inputString(answer,"Enter a,p for x(t) = a*t^p");
      sScanF(answer,"%e %e",&rpar[0],&rpar[1]);
      printf("The specified piston motion parameters are a=%e, p=%e\n",rpar[0],rpar[1]);

      // *wdh* 101020 -- Input flow variables too ---
      gi.inputString(answer,"Enter rho,p, angle   (initial density, initial pressure, rotation angle (degrees))");
      rpar[2]=1.4;  // rho
      rpar[3]=1.;   // p 
      rpar[4]=0.;   // angle in degrees (for a rotated piston)
      sScanF(answer,"%e %e %e",&rpar[2],&rpar[3],&rpar[4]);
      printF("Specified piston : Setting rho=%8.2e, p=%8.2e, angle=%8.2e\n",rpar[2],rpar[3],rpar[4]);

    }
    else if( answer=="forced piston motion" )  // **OLD WAY**
    {
      userKnownSolution="forcedPiston"; 
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
      rpar[0]=1.; rpar[1]=1.; 

      gi.inputString(answer,"Enter mass,height");
      sScanF(answer,"%e %e",&rpar[0],&rpar[1]);
      printf("forced piston: mass=%e, height=%e\n",rpar[0],rpar[1]);
    }
    else if( answer=="shock elastic piston" )
    {
      userKnownSolution="shockElasticPiston";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time

      printF("SEP:INFO: The 'shock elastic piston' option defines an exact solution for a shock hitting an elastic piston\n"
             "  The initial shock is located at xShock, and has Mach number Mshock\n");
      

      int domain=1;  // 1=solid, 2=fluid
      if( !dbase.has_key("v1c") )
      {
        domain=2;  // fluid 
      }
      

      ipar[0]=domain;   // 1=solid, 2=fluid

      real xShock=.25, Mshock=2., gamma=1.4, rhoSolid=1., mu=1., lambda=1.;
      gi.inputString(answer,"Enter xShock, Mshock, gamma, rhoSolid, mu, lambda");
      sScanF(answer,"%e %e %e %e %e %e",&xShock,&Mshock,&gamma,&rhoSolid,&mu,&lambda);
      
      printF("SEP: Using xShock=%8.2e, Mshock=%8.2e, gamma=%5.2f, rhoSolid=%8.2e, mu=%8.2e, lambda=%8.2e\n",
               xShock,Mshock,gamma,rhoSolid,mu,lambda);

      real cp = sqrt( (lambda+2.*mu)/rhoSolid );

      rpar[0]=xShock;
      rpar[1]=Mshock;
      rpar[2]=gamma;
      rpar[3]=rhoSolid;
      rpar[4]=cp;
      rpar[5]=lambda;
      rpar[6]=mu;
      

    }
    else if( answer=="supersonic flow in an expanding channel" )
    {
      userKnownSolution="superSonicExpandingFlow"; 
      dbase.get<bool>("knownSolutionIsTimeDependent")=false;  // known solution does NOT depend on time

      real  rho0=1., v0=0., p0=1;
      real  a0 = sqrt( dbase.get<real >("gamma")*p0/rho0);
      real  u0 = 1.5*a0;
      // rpar[]={ dbase.get<real >("gamma"), dbase.get<real >("Rg"), rho0, u0,v0, p0}; //
      rpar[0]=dbase.get<real >("gamma");
      rpar[1]=dbase.get<real >("Rg");
      rpar[2]=rho0;
      rpar[3]=u0;
      rpar[4]=v0;
      rpar[5]=p0; 

      gi.inputString(answer,sPrintF("Enter the inflow state: rho0,u0,v0,p0 (default: rho0=%g, u0=%g, v0=%g, p0=%g)",
				    rpar[2],rpar[3],rpar[4],rpar[5]));
      sScanF(answer,"%e %e %e %e %e",&rpar[2],&rpar[3],&rpar[4],&rpar[5]);
      printF("supersonic expansion: rho0=%g, u0=%g, v0=%g, p0=%g\n",rpar[2],rpar[3],rpar[4],rpar[5]);

      ipar[0]=dbase.get<int >("rc");
      ipar[1]=dbase.get<int >("uc");
      ipar[2]=dbase.get<int >("vc");
      ipar[3]=dbase.get<int >("wc");
      ipar[4]=dbase.get<int >("tc");
      ipar[5]=-1;
      ipar[6]=-1;

      int side=0,axis=1,grid=0;
      gi.inputString(answer,sPrintF("Enter (side,axis,grid) for the curved wall (default: side=%i, axis=%i, grid=%i)\n",side,axis,grid));
      sScanF(answer,"%i %i %i ",&side,&axis,&grid);
      printF("supersonic expansion: using (side,axis,grid)=(%i,%i,%i)\n",side,axis,grid);

      ipar[7]=side;
      ipar[8]=axis;
      ipar[9]=grid;
      
      
    }
    else if( answer=="oblique shock flow" )
    {
      userKnownSolution="obliqueShockFlow";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
      
      // These parameters are used in FLowSolutions.bC
      //   real  r1 = rpar[0];
      //   real v11 = rpar[1];
      //   real v21 = rpar[2];
      //   real v31 = rpar[3];
      //   real  T1 = rpar[4];
      //   real  p1 = rpar[5];
      // 
      //   real  r2 = rpar[6];
      //   real v12 = rpar[7];
      //   real v22 = rpar[8];
      //   real v32 = rpar[9];
      //   real  T2 = rpar[10];
      //   real  p2 = rpar[11];
      // 
      //   real an1 = rpar[12];  // normal to the shock 
      //   real an2 = rpar[13];
      //   real an3 = rpar[14];
      //   real x0  = rpar[15];  // point on the shock 
      //   real y0  = rpar[16];
      //   real z0  = rpar[17];
      //   real shockSpeed = rpar[18];
      //   real t   = rpar[19];
      gi.inputString(answer,"Enter the state ahead of the shock: rho,v1,v2,v3,T");
      sScanF(answer,"%e %e %e %e %e",&rpar[0],&rpar[1],&rpar[2],&rpar[3],&rpar[4]);

      gi.inputString(answer,"Enter the state behind the shock: rho,v1,v2,v3,T");
      sScanF(answer,"%e %e %e %e %e",&rpar[6],&rpar[7],&rpar[8],&rpar[9],&rpar[10]);

      gi.inputString(answer,"Enter the normal to the shock [n1,n2,n3]");
      sScanF(answer,"%e %e %e",&rpar[12],&rpar[13],&rpar[14]);

      gi.inputString(answer,"Enter a point on the shock [x,y,z]");
      sScanF(answer,"%e %e %e",&rpar[15],&rpar[16],&rpar[17]);

      gi.inputString(answer,"Enter the shock speed");
      sScanF(answer,"%e",&rpar[18]);

      real gamma = dbase.get<real >("gamma");
      real Rg = dbase.get<real >("Rg");

      // compute the pressure 
      rpar[5] =rpar[0]*Rg*rpar[4];
      rpar[11]=rpar[6]*Rg*rpar[10];


//       // compute state 2 from state 1:
//       real  r1 = rpar[0];
//       real v11 = rpar[1];
//       real v21 = rpar[2];
//       real v31 = rpar[3];
//       real  T1 = rpar[4];
//       // real  p1 = rpar[5];
      
//       real  r2 = rpar[6];
//       real v12 = rpar[7];
//       real v22 = rpar[8];
//       real v32 = rpar[9];
//       real  T2 = rpar[10];
//       // real  p2 = rpar[11];
      
//       real an1 = rpar[12];  // normal to the shock 
//       real an2 = rpar[13];
//       real an3 = rpar[14];
//       real aNorm = max( REAL_MIN*100., sqrt( an1*an1+an2*an2+an3*an3));
//       an1/=aNorm;
//       an2/=aNorm;
//       an3/=aNorm;
//       rpar[12]=an1;
//       rpar[13]=an2;
//       rpar[14]=an3;

//       real x0  = rpar[15];  // point on the shock 
//       real y0  = rpar[16];
//       real z0  = rpar[17];
//       real U   = rpar[18];  // shock speed
//       // real t   = rpar[19];

      
//       real a1 = sqrt(gamma*p1/r1);
//       real v1 = U*an1-v11;
//       real v2 = U*an2-v21;
//       real v3 = U*an3-v31;

//       real v1Norm = sqrt( v11*v11 + v21*v21 + v31*v31 );
//       real M1 = v1Norm/a1;
      
//       real sinBeta = ((U*an1-v11)*an1+(U*an2-v21)*an2+(U*an3-v31)*an3)/v1Norm;
      

      
//       OV_ABORT("finish me");



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
    else if( answer=="rotating disk" )
    {
      userKnownSolution="rotatingDisk";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      int domain=1;  // 1=solid, 2=fluid
      if( !dbase.has_key("v1c") )
      {
        domain=2;  // fluid 
      }
      ipar[0]=domain;   // 1=solid, 2=fluid

      if( !db.has_key("tDisk") )
      { // Create parameters for the rotating disk solution
	db.put<real>("tDisk");
	db.put<int>("numberOfGridPointsDisk");
	db.put<real>("omegaDisk");
	db.put<real>("innerRadiusDisk");
	db.put<real>("outerRadiusDisk");
        db.put<RealArray>("uDisk"); // exact solution is stored here
      }
      

      real & tDisk = db.get<real>("tDisk");
      int & numberOfGridPoints = db.get<int>("numberOfGridPointsDisk");
      real & omega = db.get<real>("omegaDisk");
      real & innerRadius = db.get<real>("innerRadiusDisk");
      real & outerRadius = db.get<real>("outerRadiusDisk");
      RealArray & uDisk = db.get<RealArray>("uDisk"); // exact solution is stored here

      // Defaults:
      tDisk=-1.;  // this will cause the solution to be computed 
      numberOfGridPoints=101;
      omega=.5;
      innerRadius=0.;
      outerRadius=1.;

      // Prompt for input:
      printF("--- The rotating disk exact solution requires: ---\n"
             " n : number of points to use when computing the exact solution\n"
             " omega : rotation rate\n"
             " ra,rb : radial bounds\n");
      gi.inputString(answer,"Enter n,omega,ra,rb for the exact solution");
      sScanF(answer,"%i %e %e %e",&numberOfGridPoints,&omega,&innerRadius,&outerRadius);

      printF("rotatingDisk: setting n=%i, omega=%9.3e, ra=%9.3e, rb=%9.3e\n",
	     numberOfGridPoints,omega,innerRadius,outerRadius);

//       // We need to keep a FlowSolutions object around
//       db.put<FlowSolutions*>("flowSolutions",NULL);

//       db.get<FlowSolutions*>("flowSolutions")=new FlowSolutions;

//       FlowSolutions & flowSolutions = *db.get<FlowSolutions*>("flowSolutions");

    }
    
    else if( answer=="rotating elastic disk in a fluid" )
    {
      printF("INFO: This a semi-analytic exact solution for an FSI problem of a rotating disk (elastic solid SVK) in a fluid\n"
             "       This solution is normally used with cgmp\n");

      userKnownSolution="rotatingElasticDiskInFluid";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      int domain=1;  // 1=solid, 2=fluid
      if( !dbase.has_key("v1c") )
        domain=2;  // fluid 
      ipar[0]=domain;   // 1=solid, 2=fluid

      if( !db.has_key("omegaDisk") )
      { // Create parameters for the rotating disk solution
	// db.put<int>("numberOfGridPointsSolid");
	// db.put<int>("numberOfGridPointsFluid");
	db.put<real>("omegaDisk");
	db.put<real>("innerRadiusDisk");
	db.put<real>("outerRadiusDisk");
	db.put<real>("outerRadiusFluid");

	db.put<real>("lambdas");
	db.put<real>("mus");
	db.put<real>("rho0");
	db.put<real>("pOffset");
	db.put<real>("gamma");
	db.put<real>("Rg");
	db.put<real>("gridRatio");
      }

      // -- we need to save component numbers from both domains (saved in file scope global variables)--
      if( dbase.has_key("s11c") )
      { // -- extract solid domain component numbers --
        u1c=dbase.get<int>("u1c");
        u2c=dbase.get<int>("u2c");
        v1c=dbase.get<int>("v1c");
        v2c=dbase.get<int>("v2c");

        s11c=dbase.get<int>("s11c");
        s12c=dbase.get<int>("s12c");
        s21c=dbase.get<int>("s21c");
        s22c=dbase.get<int>("s22c");

        // put defaults in for the fluid: (in case we only are solving for the solid)
        if( rc==-1 )
	{
          rc=0; uc=1; vc=2; tc=3;
	}

      }
      else
      { // --- extract fluid domain component numbers ---
        rc=dbase.get<int>("rc");
        uc=dbase.get<int>("uc");
        vc=dbase.get<int>("vc");
        tc=dbase.get<int>("tc");

        // put defaults in for the solid: (in case we only are solving for the fluid)
        if( v1c==-1 )
	{
          v1c=0; v2c=1; s11c=2; s12c=3; s21c=4; u1c=5; u2c=6;
	}

      }
      

      // int & numberOfGridPointsSolid = db.get<int>("numberOfGridPointsSolid");
      // int & numberOfGridPointsFluid = db.get<int>("numberOfGridPointsFluid");
      real & omegaDisk = db.get<real>("omegaDisk");
      real & innerRadiusDisk = db.get<real>("innerRadiusDisk");
      real & outerRadiusDisk = db.get<real>("outerRadiusDisk");
      real & outerRadiusFluid = db.get<real>("outerRadiusFluid");
      real & lambdas = db.get<real>("lambdas");
      real & mus = db.get<real>("mus");
      real & rho0 = db.get<real>("rho0");
      real & pOffset = db.get<real>("pOffset");
      real & gamma = db.get<real>("gamma");
      real & Rg = db.get<real>("Rg");
      real & gridRatio = db.get<real>("gridRatio");

      // Defaults:
      // numberOfGridPointsSolid=101;
      // numberOfGridPointsFluid=101;
      omegaDisk=.5;
      innerRadiusDisk=0.;
      outerRadiusDisk=1.;
      outerRadiusFluid=2.;
      lambdas=1.;
      mus=1.;
      rho0=1.4;
      pOffset=0.;
      gamma=1.4;
      Rg=1.;
      gridRatio=10.;
      
      // Prompt for input:
      printF("--- The rotating elastic disk in a fluid exact solution requires: ---\n"
             // " ns, nf : number of points to use when computing the exact solution (solid,fluid)\n"
             " omega : rotation rate\n"
             " r0,r1,r2 : radial bounds\n"
             " gridRatio : grid for 1D solution is this many times finer.\n"
             " lambda, mu : solid Lame parameters NOTE: solid density is 1\n"
             " rho0,pOffset,gamma,Rg : fluid parameters\n"
             );
      gi.inputString(answer,"Enter omega,r0,r1,r2, gridRatio for the exact solution");
      sScanF(answer,"%e %e %e %e %e",&omegaDisk,&innerRadiusDisk,&outerRadiusDisk, &outerRadiusFluid,
             &gridRatio);
      gi.inputString(answer,"Enter lambda,mu, rho0,pOffset,gamma,Rg for the exact solution");
      sScanF(answer,"%e %e %e %e %e %e",&lambdas,&mus,&rho0,&pOffset,&gamma,&Rg);

      printF("rotatingElasticDiskInFluid: setting omega=%9.3e, r0=%9.3e, r1=%9.3e, r2=%9.3e gridRatio=%g\n"
             "  lambda=%g, mu=%g, rho0=%g, pOffset=%g, gamma=%g, Rg=%g\n",
	     omegaDisk,innerRadiusDisk,outerRadiusDisk,outerRadiusFluid,gridRatio,
	     lambdas,mus,rho0,pOffset,gamma,Rg );

    }
    
    else if( answer=="uniform flow INS" )
    {
      userKnownSolution="uniformFlowINS";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent
    }
    
    else if( answer=="FSI traveling wave solution fluid" || 
             answer=="FSI traveling wave solution solid" || 
             answer=="FSI traveling wave solution" ) // for backward compatibility
    {
      if( answer=="FSI traveling wave solution fluid" || answer=="FSI traveling wave solution" )
        userKnownSolution="travelingWaveFSIfluid";
      else
        userKnownSolution="travelingWaveFSIsolid";

      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent 

      printF("INFO:The FSI traveling wave solution is an exact solution for a solid (shell or bulk)\n"
	     "coupled to a linearized incompressible fluid\n"
	     "See: `An analysis of a new stable partitioned algorithm for FSI problems.\n"
             "      Part I: Incompressible flow and elastic solids'. \n"
	     "      J.W. Banks, W.D. Henshaw and D.W. Schwendeman, JCP 2014.\n");
    
      
      if( !dbase.has_key("travelingWaveFsi") )
	dbase.put<TravelingWaveFsi*>("travelingWaveFsi")=NULL;

      if( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")==NULL )
	dbase.get<TravelingWaveFsi*>("travelingWaveFsi")= new TravelingWaveFsi; // who will delete ???
      TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

      travelingWaveFsi.update(gi );

      // we also pass the grid for the solid:
      CompositeGrid & cgSolid = cg; // do this for now  -- only used for number of grid points
      travelingWaveFsi.setup(cg,cgSolid);
      if( userKnownSolution=="travelingWaveFSIsolid" )
      {
        // -- set the component numbers ---
	travelingWaveFsi.dbase.get<int>("u1c") =dbase.get<int>("u1c") ;
	travelingWaveFsi.dbase.get<int>("u2c") =dbase.get<int>("u2c") ;
						                      
	travelingWaveFsi.dbase.get<int>("v1c") =dbase.get<int>("v1c") ;
	travelingWaveFsi.dbase.get<int>("v2c") =dbase.get<int>("v2c") ;
						                      
	travelingWaveFsi.dbase.get<int>("s11c")=dbase.get<int>("s11c");
	travelingWaveFsi.dbase.get<int>("s12c")=dbase.get<int>("s12c");
	travelingWaveFsi.dbase.get<int>("s21c")=dbase.get<int>("s21c");
	travelingWaveFsi.dbase.get<int>("s22c")=dbase.get<int>("s22c");
      }
      

    }

    else if( answer=="bulk solid piston" )
    {
      // -- EXACT FSI Solution for INS + SM ---

      // NOTE -- this function is implemented in 
      //     ins/src/userDefinedKnownSolution 
      //     sm/src/userDefinedKnownSolution 
      //     common/src/cBessel.f

      userKnownSolution="bulkSolidPiston";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & H        = rpar[0];
      real & Hbar     = rpar[1];
      real & rho      = rpar[2];
      real & rhoBar   = rpar[3];
      real & lambdaBar= rpar[4];
      real & muBar    = rpar[5];
      
      real amp,k,t0,ra,rb;
      int rampOrder;

      printF("--------------------------------------------------------------------------------\n"
             "------ Exact solution for a bulk elastic solid adjacent to a fluid chamber -----\n\n"
             "   y_I(t) = F(t + Hbar/cp) - F(t - Hbar/cp)\n"
             "      F(z) = amp * R(z) * sin( 2*Pi*k(z-t0) ) \n"
             "      R(z) = ramp function that smoothly transitions from 0 to 1  \n"
             " Parameters: \n"
             " amp : amplitude of the interface motion \n"
             " k: wave number in solid domain (y-direction)\n"
             " H,Hbar: Height of fluid and solid domains\n"
             " t0 : determines the phase for time dependence\n"
             " rhoBar,lambaBar,muBar : solid density and Lame parameters\n"
             " rampOrder, ra,rb : order-of-ramp (1,2,3,4), start and end of ramp transition\n"
             "--------------------------------------------------------------------------------\n"
	);
      gi.inputString(answer,"Enter amp, k,t0,H,Hbar,rho,rhoBar,lambdaBar,muBar");
      sScanF(answer,"%e %e %e %e %e %e %e %e %e",&amp,&k,&t0,&H,&Hbar,&rho,&rhoBar,&lambdaBar,&muBar);

      gi.inputString(answer,"Enter rampOrder,ra,rb");
      sScanF(answer,"%i %e %e",&rampOrder,&ra,&rb);

      printF("Setting amp=%g, k=%g,t0=%g,H=%g,Hbar=%g,rho=%g,lambdaBar=%g,muBar=%g,rhoBar=%g"
               "  rampOrder=%i,ra=%g,rb=%g\n",
             amp,k,t0,H,Hbar,rho,rhoBar,lambdaBar,muBar,rampOrder,ra,rb);


      // The waveform for the exact solution is defined through a TimeFunction:
      if( !db.has_key("timeFunctionBSP") )
      {
        db.put<TimeFunction>("timeFunctionBSP");
        // db.put<TimeFunction>("rampFunctionBSP");
      }
      
      const real cp2 = sqrt((lambdaBar+2.*muBar)/rhoBar);

      TimeFunction & timeFunction = db.get<TimeFunction>("timeFunctionBSP");


      //    f(t)=b0*sin(2.*Pi*f0*(t-t0));
      real b0=amp, f0=k;
      timeFunction.setSinusoidFunction( b0, f0, t0 );

      TimeFunction & rampFunction = * new TimeFunction();  // TimeFunction compose will reference count 
      rampFunction.incrementReferenceCount();
      
      real rampStart=0., rampEnd=1.; // Ramp  from 0 to 1
      // Shift ramp start time to account for form of the solution. 
      // ramp should only start after bar/cp2
      real rampStartTime=Hbar/cp2+ra, rampEndTime=rampStartTime+rb; // Ramp up over [rampStartTime,rampEndTime]
      rampFunction.setRampFunction( rampStart,rampEnd,rampStartTime,rampEndTime,rampOrder );

      // F(t) = Ramp(t) * sin( ... )
      timeFunction.compose(&rampFunction);  // "compose" the two TimeFunction's 
      
      if( rampFunction.decrementReferenceCount()==0 )
        delete &rampFunction;
      
    }

    else if( answer=="radial elastic piston" )
    {
      // -- RADIAL ELASTIC PISTON ----
      //  EXACT FSI Solution for INS + SM ---

      // NOTE -- this function is implemented in 
      //     ins/src/userDefinedKnownSolution 
      //     sm/src/userDefinedKnownSolution 
      //    

      userKnownSolution="radialElasticPiston";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & R        = rpar[0];
      real & Rbar     = rpar[1];
      real & rho      = rpar[2];
      real & rhoBar   = rpar[3];
      real & lambdaBar= rpar[4];
      real & muBar    = rpar[5];
      real & kk       = rpar[6];
      
      real amp,t0,k;
      int rampOrder;

      printF("------------------------------------------------------------------------------------------\n"
             "----------- Exact solution for a radial elastic piston and incompressible fluid ----------\n\n"
             " The radial displacement in the solid is \n"
             "   rHat.us = amp * J1(k*r)/J1(k*rBar) * sin( twoPi*cp*k*(t-t0) ) \n"
             " Parameters: \n"
             " amp : approximate amplitude of the interface motion \n"
             " k: wave number in solid domain (r-direction)\n"
             " R,Rbar: Radial widths (initial) of the fluid and solid domains\n"
             " t0 : determines the phase for time dependence\n"
             " rhoBar,lambaBar,muBar : solid density and Lame parameters\n"
             "--------------------------------------------------------------------------------\n"
	);
      gi.inputString(answer,"Enter amp, k,t0,R,Rbar,rho,rhoBar,lambdaBar,muBar");
      sScanF(answer,"%e %e %e %e %e %e %e %e %e",&amp,&k,&t0,&R,&Rbar,&rho,&rhoBar,&lambdaBar,&muBar);

      printF("--UDKS-- Setting amp=%g, k=%g,t0=%g,R=%g,Rbar=%g,rho=%g,lambdaBar=%g,muBar=%g,rhoBar=%g\n",
             amp,k,t0,R,Rbar,rho,rhoBar,lambdaBar,muBar);


      // The waveform for the exact solution is defined through a TimeFunction:
      if( !db.has_key("timeFunctionEP") )
      {
        db.put<TimeFunction>("timeFunctionREP");
      }
      
      const real cp2 = sqrt((lambdaBar+2.*muBar)/rhoBar);

      TimeFunction & timeFunction = db.get<TimeFunction>("timeFunctionREP");

      // kk = twoPi*k 
      kk = k*twoPi;
      
      //    f(t)=b0*sin(2.*Pi*f0*(t-t0)) --> amp/(J1(k*R)) * sin( (2*Pi*k)*cp*(t-t0 ))
      const real jnRbar = jn(1,kk*Rbar);
      const real b0=amp/jnRbar, f0=cp2*k;
      timeFunction.setSinusoidFunction( b0, f0, t0 );

    }
    else if ( answer=="shearing fluid and elastic solid" ) {
      // -- Shear solution for elastic solid and fluid --
      // Exact FSI solution for INS + SM
      //
      // NOTE -- this function is implemented in 
      //     ins/src/userDefinedKnownSolution 
      //     sm/src/userDefinedKnownSolution 
      //    
      
      userKnownSolution="fibShear";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

      real & omegar = rpar[0];
      real & omegai = rpar[1];
      real & ar     = rpar[2];
      real & ai     = rpar[3];
      real & br     = rpar[4];
      real & bi     = rpar[5];
      real & cr     = rpar[6];
      real & ci     = rpar[7];
      real & dr     = rpar[8];
      real & di     = rpar[9];
      real & ksr    = rpar[10];
      real & ksi    = rpar[11];
      real & kfr    = rpar[12];
      real & kfi    = rpar[13];
      real & amp    = rpar[14];
      real & mu = rpar[15];

      printF("--------------------------------------------------------------------------------\n"
             "------ Exact solution for a parallel flow shearing a bulk elastic solid --------\n\n"
             " \bar{u}_1(y,t) = amp         exp(i omega t) (A cos(ks y) + B sin( ks y))\n"
	     "     {v}_1(y,t) = amp i omega exp(i omega t) (C exp(kf y) + D exp(-kf y))\n"
	     "             ks = omega / cs\n"
	     "             kf = sqrt(i rho omega / mu)\n"
             " Parameters: \n"
             " amp    : maximum amplitude of the displacement \n"
             " omega  : time frequency of solution \n"
             " H,Hbar : Height of fluid and solid domains\n"
             " rhoBar,lambaBar,muBar : solid density and Lame parameters\n"
             "--------------------------------------------------------------------------------\n"
	);
      
      int caseid = 0;
      gi.inputString(answer,"Enter amp, case number\n");
      sScanF(answer,"%e %d",&amp,&caseid);

      real H, Hbar, rho, rhoBar, muBar;
      if (caseid == 0) {
	H      =  1.0;
	Hbar   =  0.5;
	rho    =  1.0;
	rhoBar = 10.0;
	muBar  = 10.0;
	mu     = 10.0;

	ksr =  2.3696802625735396e+00; ksi =  2.7422804696932346e+00; 
	kfr =  2.1000127772389374e-01; kfi =  5.6420615347139857e-01; 
	omegar =  2.3696802625735396e+00; omegai =  2.7422804696932346e+00; 
	ar =  9.9999999999999978e-01; ai =  0.0000000000000000e+00; 
	br =  8.1963580548694320e-02; bi = -9.0822504122626635e-01; 
	cr =  1.7307707579616807e-01; ci =  6.8318696356195785e-01; 
	dr =  8.2692292420383173e-01; di = -6.8318696356195796e-01; 
      } else {
	H      =  1.0;
	Hbar   =  0.5;
	rho    =  1.0;
	rhoBar = 10.0;
	muBar  = 10.0;
	mu     = 10.0;

	ksr =  2.3696802625735396e+00; ksi =  2.7422804696932346e+00; 
	kfr =  2.1000127772389374e-01; kfi =  5.6420615347139857e-01; 
	omegar =  2.3696802625735396e+00; omegai =  2.7422804696932346e+00; 
	ar =  9.9999999999999978e-01; ai =  0.0000000000000000e+00; 
	br =  8.1963580548694320e-02; bi = -9.0822504122626635e-01; 
	cr =  1.7307707579616807e-01; ci =  6.8318696356195785e-01; 
	dr =  8.2692292420383173e-01; di = -6.8318696356195796e-01; 
      }

      printF("Setting amp=%g, H=%g, Hbar=%g, rho=%g, rhoBar=%g, muBar=%g, mu=%g\n",
	     amp,H,Hbar,rho,rhoBar,muBar,mu);
      

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
