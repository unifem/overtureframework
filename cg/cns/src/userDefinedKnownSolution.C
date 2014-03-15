#include "CnsParameters.h"
#include "FlowSolutions.h"
#include "GenericGraphicsInterface.h"
#include "FluidPiston.h"
#include "PistonMotion.h"
#include "ParallelUtility.h"


#include "BeamModel.h"

// #define rotatingDiskSVK EXTERN_C_NAME(rotatingdisksvk)

// extern "C"
// {
//   // rotating disk (SVK) exact solution:
//   void rotatingDiskSVK( const real & t, const int & numberOfGridPoints, real & uDisk, real & param,
//                         const int & nrwk, real & rwk );
// }

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

int CnsParameters::
getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, realArray & ua, 
			    const Index & I1, const Index &I2, const Index &I3 )
// ==========================================================================================
///  \brief Evaluate a user defined known solution.
// ==========================================================================================
{
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

    FlowSolutions flowSolutions;

    int ipar[]={  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("wc"), dbase.get<int >("tc"),-1,-1 }; // 

    rpar[19]=t;
    // evaluate the known solution for an oblique shock flow
    flowSolutions.getFlowSolution(FlowSolutions::obliqueShockFlow, cg,grid,ua,ipar,rpar,I1,I2,I3);
  }
  else if( userKnownSolution=="superSonicExpandingFlow" )
  {
    // evaluate the known solution for the supersonic expanding flow

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
//   else if( userKnownSolution=="rotatingDisk" )
//   {
//     // ---- return the exact solution for the rotating disk ---
//     printF(" userDefinedKnownSolution: rotatingDisk: t=%9.3e\n",t);

//     // Here are the comopnents for displacement velocity and stress
//     int v1c = dbase.get<int >("v1c");
//     int v2c = dbase.get<int >("v2c");

//     int u1c = dbase.get<int >("uc");
//     int u2c = dbase.get<int >("vc");

//     int s11c = dbase.get<int >("s11c");
//     int s12c = dbase.get<int >("s12c");
//     int s21c = dbase.get<int >("s21c");
//     int s22c = dbase.get<int >("s22c");

//     assert( v1c>=0 && u1c>=0 && s22c >=0 );

//     MappedGrid & mg = cg[grid];
//     mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

//     const realArray & center = mg.center();
//     realArray & u = ua;

    
//     // tDisk : solution has been computed at this time
//     real & tDisk = db.get<real>("tDisk");

//     const int & numberOfGridPoints = db.get<int>("numberOfGridPointsDisk");

//     const real & omega = db.get<real>("omegaDisk");

//     const real & innerRadius = db.get<real>("innerRadiusDisk");
//     const real & outerRadius = db.get<real>("outerRadiusDisk");


//     const real dr = (outerRadius-innerRadius)/(numberOfGridPoints-1.);

//     RealArray & uDisk = db.get<RealArray>("uDisk"); // exact solution is stored here

//     if( t!=tDisk )
//     {
//       // Compute the exact solution at time t
//       tDisk=t;


//       int numberOfComponents=8;  // number of components needed in 1D solution: we compute u, v, and sigma 
//       if( uDisk.getLength(0)!=numberOfGridPoints )
//       {
// 	uDisk.redim(numberOfGridPoints,numberOfComponents);
//       }
      
//       // call the fortran routine here to evaluate the solution at time t and return the result in uDisk     

//       int nrwk=10*numberOfGridPoints+6;
//       RealArray rwk(nrwk);
//       int npar=10;
//       RealArray param(npar);

//       param(0)=innerRadius;
//       param(1)=outerRadius;
//       param(2)=omega;
//       param(3)=1.;   // this is lambda
//       param(4)=1.;   // this mu

//       rotatingDiskSVK( t, numberOfGridPoints, *uDisk.getDataPointer(), *param.getDataPointer(), nrwk, *rwk.getDataPointer() );

//     }
    

//     const real twopi=6.283185307179586;
//     const real x0=0., y0=0.;  // center of the disk 

//     int i1,i2,i3;
//     FOR_3D(i1,i2,i3,I1,I2,I3)
//     {
//       // Reference coordinates:
//       real x= center(i1,i2,i3,0);
//       real y= center(i1,i2,i3,1);

//       real r = sqrt( SQR(x-x0) + SQR(y-y0) );

//       // closest point in uDisk less than or equal to r:
//       int i = int( (r-innerRadius)/dr );  
//       i = max(0,min(i,numberOfGridPoints-2));
      
//       // linear interpolation
//       real alpha = (r-innerRadius)/dr - i;
//       real c1=1.-alpha, c2=alpha;

//       // radial and angular displacements
//       real w=c1*uDisk(i,0)+c2*uDisk(i+1,0);
//       real p=c1*uDisk(i,1)+c2*uDisk(i+1,1);

//       // angular position
//       real theta=0.;
//       if (r>0.)
//       {
//         if (y<0.)
//         {
//           theta=twopi-acos((x-x0)/r);
//         }
//         else
//         {
//           theta=acos((x-x0)/r);
//         }
//       }

// //      printF("rotatingDisk: i=%i, uDisk(i,0)=%9.3e, uDisk(i+1,0)=%9.3e, uDisk(i,1)=%9.3e, uDisk(i+1,1)=%9.3e\n",i,uDisk(i,0),uDisk(i+1,0),uDisk(i,1),uDisk(i+1,1));
// //      printF("rotatingDisk: i=%i, alpha=%9.3e, c1=%9.3e, c2=%9.3e, w=%9.3e, p=%9.3e, theta=%9.3e\n",i,alpha,c1,c2,w,p,theta);

//       // displacements
//       real thbar=theta+p;
//       u(i1,i2,i3,u1c)=(r+w)*cos(thbar)-x;
//       u(i1,i2,i3,u2c)=(r+w)*sin(thbar)-y;

//       // velocities
//       real wt=c1*uDisk(i,2)+c2*uDisk(i+1,2);
//       real pt=c1*uDisk(i,3)+c2*uDisk(i+1,3);
//       u(i1,i2,i3,v1c)=wt*cos(thbar)-pt*(r+w)*sin(thbar);
//       u(i1,i2,i3,v2c)=wt*sin(thbar)+pt*(r+w)*cos(thbar);

//       // stresses
//       real pbar11=c1*uDisk(i,4)+c2*uDisk(i+1,4);
//       real pbar12=c1*uDisk(i,5)+c2*uDisk(i+1,5);
//       real pbar21=c1*uDisk(i,6)+c2*uDisk(i+1,6);
//       real pbar22=c1*uDisk(i,7)+c2*uDisk(i+1,7);
//       real pt11=pbar11*cos(thbar)-pbar12*sin(thbar);
//       real pt12=pbar11*sin(thbar)+pbar12*cos(thbar);
//       real pt21=pbar21*cos(thbar)-pbar22*sin(thbar);
//       real pt22=pbar21*sin(thbar)+pbar22*cos(thbar);
//       u(i1,i2,i3,s11c)=cos(theta)*pt11-sin(theta)*pt21;
//       u(i1,i2,i3,s12c)=cos(theta)*pt12-sin(theta)*pt22;
//       u(i1,i2,i3,s21c)=sin(theta)*pt11+cos(theta)*pt21;
//       u(i1,i2,i3,s22c)=sin(theta)*pt12+cos(theta)*pt22;

//     }


//   }
  else if( userKnownSolution=="uniformFlowINS" )
  {
    // for testing with INS
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
  else if( userKnownSolution=="linearBeamExactSolution" )
  {
 
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),vertex);

    realArray & u = ua;

    const int & uc = dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
    const int & vc = dbase.get<int >("vc");  
    const int & pc = dbase.get<int >("pc");  
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
    double what = 0.00001;
    //double beta=1.0/nu*sqrt(E*Ioverb/(rhos*h));
    //std::complex<double> omegatilde(1.065048891,-5.642079778e-4);
    std::cout << "t = " << t << std::endl;
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



//\begin{>>MovingGridsSolverInclude.tex}{\subsection{updateUserDefinedMotion}} 
int CnsParameters::
updateUserDefinedKnownSolution(GenericGraphicsInterface & gi)
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
      // "rotating disk",  // for cgsm SVK model
      "uniform flow INS", // for testing INS    
      "linear beam exact solution",
      "choose a common known solution",  // choose a known solution from the base class
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
      Parameters::updateUserDefinedKnownSolution(gi);
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
//     else if( answer=="rotating disk" )
//     {
//       userKnownSolution="rotatingDisk";
//       dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent

//       if( !db.has_key("tDisk") )
//       { // Create parameters for the rotating disk solution
// 	db.put<real>("tDisk");
// 	db.put<int>("numberOfGridPointsDisk");
// 	db.put<real>("omegaDisk");
// 	db.put<real>("innerRadiusDisk");
// 	db.put<real>("outerRadiusDisk");
//         db.put<RealArray>("uDisk"); // exact solution is stored here
//       }
      

//       real & tDisk = db.get<real>("tDisk");
//       int & numberOfGridPoints = db.get<int>("numberOfGridPointsDisk");
//       real & omega = db.get<real>("omegaDisk");
//       real & innerRadius = db.get<real>("innerRadiusDisk");
//       real & outerRadius = db.get<real>("outerRadiusDisk");
//       RealArray & uDisk = db.get<RealArray>("uDisk"); // exact solution is stored here

//       // Defaults:
//       tDisk=-1.;  // this will cause the solution to be computed 
//       numberOfGridPoints=101;
//       omega=.5;
//       innerRadius=0.;
//       outerRadius=1.;

//       // Prompt for input:
//       printF("--- The rotating disk exact solution requires: ---\n"
//              " n : number of points to use when computing the exact solution\n"
//              " omega : rotation rate\n"
//              " ra,rb : radial bounds\n");
//       gi.inputString(answer,"Enter n,omega,ra,rb for the exact solution");
//       sScanF(answer,"%i %e %e %e",&numberOfGridPoints,&omega,&innerRadius,&outerRadius);

//       printF("rotatingDisk: setting n=%i, omega=%9.3e, ra=%9.3e, rb=%9.3e\n",
// 	     numberOfGridPoints,omega,innerRadius,outerRadius);

// //       // We need to keep a FlowSolutions object around
// //       db.put<FlowSolutions*>("flowSolutions",NULL);

// //       db.get<FlowSolutions*>("flowSolutions")=new FlowSolutions;

// //       FlowSolutions & flowSolutions = *db.get<FlowSolutions*>("flowSolutions");

//     }
    
    else if( answer=="uniform flow INS" )
    {
      userKnownSolution="uniformFlowINS";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent
    }
    
    else if( answer=="linear beam exact solution" ) {

      userKnownSolution="linearBeamExactSolution";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent 
      double omega;
      gi.inputString(answer,"Enter omega");
      sScanF(answer,"%e",&omega);
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
