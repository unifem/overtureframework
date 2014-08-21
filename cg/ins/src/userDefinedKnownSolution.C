#include "InsParameters.h"
#include "FlowSolutions.h"
#include "GenericGraphicsInterface.h"
#include "FluidPiston.h"
#include "PistonMotion.h"
#include "ParallelUtility.h"
#include "DeformingBodyMotion.h"

#include "BeamModel.h"
#include "BoundaryLayerProfile.h"

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

int InsParameters::
getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, RealArray & ua, 
			    const Index & I1, const Index &I2, const Index &I3 )
// ==========================================================================================
///  \brief Evaluate a user defined known solution.
// ==========================================================================================
{
  MappedGrid & mg = cg[grid];
  const int numberOfDimensions = mg.numberOfDimensions();
    

  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedKnownSolutionData") )
  {
    printf("getUserDefinedKnownSolution:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
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
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);

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
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);

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
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);

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
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),xLocal);

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
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      ua(i1,i2,i3,pc)=0.;
      profile.eval( xLocal(i1,i2,i3,0)+xOffset,xLocal(i1,i2,i3,1), ua(i1,i2,i3,uc), ua(i1,i2,i3,vc) );
      if( numberOfDimensions==3 ) ua(i1,i2,i3,wc)=0.;
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



int InsParameters::
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
      "choose a common known solution",
      "pipe flow",
      "rotating Couette flow",
      "Taylor Green vortex",
      "exact solution from a file",
      "uniform flow INS", // for testing INS    
      "linear beam exact solution",
      "flat plate boundary layer",
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
