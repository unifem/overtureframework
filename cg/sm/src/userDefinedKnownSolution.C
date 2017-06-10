#include "SmParameters.h"
#include "FlowSolutions.h"
#include "GenericGraphicsInterface.h"
#include "FluidPiston.h"
#include "PistonMotion.h"
#include "ParallelUtility.h"
#include "TimeFunction.h"

#include "BeamModel.h"

#define rotatingDiskSVK EXTERN_C_NAME(rotatingdisksvk)

extern "C"
{
  // rotating disk (SVK) exact solution:
  void rotatingDiskSVK( const real & t, const int & numberOfGridPoints, real & uDisk, real & param,
                        const int & nrwk, real & rwk );
}

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

int SmParameters::
getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, RealArray & ua, 
			    const Index & I1, const Index &I2, const Index &I3, 
                            int numberOfTimeDerivatives /* = 0 */  )
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

  const real dt = dbase.get<real>("dt");
  const real & rho    = dbase.get<real>("rho");
  const real & mu     = dbase.get<real>("mu");
  const real & lambda = dbase.get<real>("lambda");

  const real cp=sqrt((2.*mu+lambda)/rho);
  const real cs =sqrt(mu/rho);

  // Here are the comopnents for displacement velocity and stress
  const int v1c = dbase.get<int >("v1c");
  const int v2c = dbase.get<int >("v2c");

  const int u1c = dbase.get<int >("uc");
  const int u2c = dbase.get<int >("vc");

  const int s11c = dbase.get<int >("s11c");
  const int s12c = dbase.get<int >("s12c");
  const int s21c = dbase.get<int >("s21c");
  const int s22c = dbase.get<int >("s22c");

  const bool assignVelocity = v1c>=0 ;
  const bool assignStress   = s11c>=0 ;

  assert( numberOfTimeDerivatives==0 );  // for now we don't use this in Cgsm

  if( userKnownSolution=="rotatingDisk" )
  {
    // ---- return the exact solution for the rotating disk ---
    printF(" userDefinedKnownSolution: rotatingDisk: t=%9.3e\n",t);

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
  else if( userKnownSolution=="linearBeamExactSolution" )
  {
 
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),vertex);

    RealArray & u = ua;

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

  else if( userKnownSolution=="bulkSolidPiston" )
  {
    // ---- return the exact solution for the FSI INS+elastic piston ---

    // assert( v1c>=0 && u1c>=0 && s22c >=0 );

    const real & amp      = rpar[0];
    const real & k        = rpar[1];
    const real & phase    = rpar[2];
    const real & H        = rpar[3];
    const real & Hbar     = rpar[4];
    const real & rhoFluid = rpar[5];
    const real & rhoBar   = rpar[6];
    const real & lambdaBar= rpar[7];
    const real & muBar    = rpar[8];

    const real cp2 = sqrt((lambdaBar+2.*muBar)/rhoBar);
    assert( cp==cp2 );
    
    const real sint = sin(cp*k*t + twoPi*phase);
    const real cost = cos(cp*k*t + twoPi*phase);

    if( t<= 2.*dt )
    {
      printF("--SM-- userDefinedKnownSolution: bulkSolidPiston, amp=%g, k=%g, phase=%g, t=%9.3e\n",amp,k,phase,t);
      printF("--SM-- cp=%g, k*hBar=%g, amp*cp*k*sin(k*hBar)=%g\n",cp,k*Hbar,amp*cp*k*sin(k*Hbar));
    }
    
   
    assert( lambda==lambdaBar && mu==muBar && rho==rhoBar );

    assert( numberOfTimeDerivatives==0 );

    // for fluid 
    // const real sinkHbar = sin(k*Hbar);
    // const real pI = -amp*(lambdaBar+2.*muBar)*cp*k*sinkHbar*cost;
    // const real yI =  amp*         sinkHbar*cost;    // interface position
    // const real vI = -amp*cp*k*    sinkHbar*sint;    // interface velocity
    // const real aI = -amp*SQR(cp*k)*sinkHbar*cost;   // interface acceleration
    // const real p0 = pI - rhoFluid*H*aI*(1.-yI)/H;        // fluid pressure at y=H 
    // const real pAmp = (p0-pI)/(H-yI);

    // *new way*
    //   u2(y,t) = F(y+H) * F( cp*t ) (which satisfies the wave eqaution w_tt = cp^2 w_yy )
    bool useNew=true;
    TimeFunction & bsp = db.get<TimeFunction>("timeFunctionBSP");
    real ft,ftp;
    bsp.eval( cp*t, ft,ftp );  // ft = F(cp*t), ftp = d(ft)/dt
    ftp *=cp;

    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

    const realArray & center = mg.center();
    RealArray & u = ua;

    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      // Reference coordinates:
      // real x= center(i1,i2,i3,0);
      const real y= center(i1,i2,i3,1);

      const real sinky = sin(k*(y+Hbar));

      real fs,fsp;
      bsp.eval( y+Hbar, fs,fsp );  // fs = F(y+H), fsp = d(fs)/dy

      // displacement
      if( useNew )
      {
        u(i1,i2,i3,u1c)=0.;
        u(i1,i2,i3,u2c)= fs*ft;
      }
      else
      {
        u(i1,i2,i3,u1c)=0.;
        u(i1,i2,i3,u2c)= amp * sinky * cost;
      }
      

      // velocities
      if( assignVelocity )
      {
        if( useNew )
        {
          u(i1,i2,i3,v1c)=0.;
          u(i1,i2,i3,v2c)=fs*ftp;
        }
        else
        {
          u(i1,i2,i3,v1c)=0.;
          u(i1,i2,i3,v2c)= -amp*cp*k*sinky*sint;
        }
        
      }
      
      // stresses
      real u2y;
      if( assignStress )
      {
        if( useNew )
        {
          u2y = fsp*ft;
        }
        else
        {
          u2y = amp * k*cos(k*(y+Hbar)) * cost;
        }
        
	u(i1,i2,i3,s11c)=lambda*u2y;
	u(i1,i2,i3,s12c)=0.;
	u(i1,i2,i3,s21c)=0.;
	u(i1,i2,i3,s22c)=(lambda+2.*mu)*u2y;
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



int SmParameters::
updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg)
// ==========================================================================================
/// \brief This function is called to set the user defined know solution.
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
      "rotating disk",  // for cgsm SVK model
      "linear beam exact solution",
      "bulk solid piston",  // for INS+SM exact solution
      "choose a common known solution",
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
    else if( answer=="choose a common known solution" )
    {
      // Look for a known solution from the base class (in common/src)
      Parameters::updateUserDefinedKnownSolution(gi,cg);
    }
    else if( answer=="no known solution" )
    {
      userKnownSolution="unknownSolution";
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
    
    else if( answer=="linear beam exact solution" ) 
    {

      userKnownSolution="linearBeamExactSolution";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution IS time dependent 
      double omega;
      gi.inputString(answer,"Enter omega");
      sScanF(answer,"%e",&omega);
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
      printF("Setting amp=%g, k=%g,phase=%g,H=%g,Hbar=%g,rho=%g,lambdaBar=%g,muBar=%g,rhoBar=%g\n",
                  amp,k,phase,H,Hbar,rho,rhoBar,lambdaBar,muBar);

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
