#include "DomainSolver.h"
#include "ShowFileReader.h"
#include "DataPointMapping.h"
#include "ShowFileReader.h"
#include "interpPoints.h"
#include "ParallelUtility.h"

#include "../moving/src/BeamModel.h"

namespace // make the following local to this file
{
// Here are the possible options for user defined initial conditions. Add new options to this enum.
enum UserDefinedInitialConditionOptions
{
  uniformState,
  bubbles,
  couetteProfile,
  temperatureGradient,
  profileFromADataFile,
  profileFromADataFileWithPerturbation,
  rotatedShock,
  LANLCookOff,
  LANLCookOff2,
  ig,
  conicalShock,
  convergingShock,
  bubbleShock,
  LX17Puck,
  rateStick,
  pdethruster,
  pdethruster3d,
  chev,
  aslam,
  ioan,
  jeffCorner,
  jeffRestart,
  aslamWeak,
  shockPolar,
  twoJump,
  threeJump,
  elliminate,
  planarInterfaceWithShock,
  circularInterfaceWithShock,
  circularSmoothInterfaceWithShock,
  circularSmoothInterfaceWithShock2,
  longEllipticalSmoothInterfaceWithShock,
  tallEllipticalSmoothInterfaceWithShock,
  bubblesShock,
  compliantCorner,
  compliantCornerDES,
  makeFail,
  rotatedJump,
  pencil,
  pencilRestart,
  noh2D,
  sedov2D,
  gravitationallyStratified,
  solidBody,
  rateStick2,
  ablProfile,
  linearBeamExactSolution
};
}

// this next function is for setting values from a 1D profile
int 
initialConditionsFromAProfile(const aString & fileName,
                              realCompositeGridFunction & u,
                              Parameters & parameters,
                              GenericGraphicsInterface & gi,
                              real rpar[]  );

int DomainSolver::
userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u )
//==============================================================================================
/// \brief
///   User defined initial conditions. This function is called to actually assign user 
///   defined initial conditions. The function setupUserDefinedInitialConditions is first 
///   called to assign the option and parameters. Rewrite or add new options to 
///   this function and to setupUserDefinedInitialConditions to supply your own initial conditions.
///
/// \note
/// 
///    -  You must fill in the realCompositeGridFunction u.
///    -   The `parameters' object holds many useful parameters.
///
///  When using adaptive mesh refinement, this function may be called multiple times as the
///  AMR hierarchy is built up.
///
/// \return: 0=success, non-zero=failure.
//==============================================================================================
{

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & rc = parameters.dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");  
  const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  const int & sc = parameters.dbase.get<int >("sc");   //  mass fraction lambda
  const int & pc = parameters.dbase.get<int >("pc");
  
  // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedInitialConditionData") )
  {
    printF("userDefinedInitialConditions:ERROR: sub-directory `userDefinedInitialConditionData' not found!\n");
    Overture::abort("error");
  }
  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedInitialConditionData");

  UserDefinedInitialConditionOptions & option= db.get<UserDefinedInitialConditionOptions>("option");

  RealArray & uniform      = db.get<RealArray>("uniform");
  RealArray & bubbleCentre = db.get<RealArray>("bubbleCentre");
  RealArray & bubbleRadius = db.get<RealArray>("bubbleRadius");
  RealArray & bubbleValues = db.get<RealArray>("bubbleValues");
  RealArray & hotSpotData  = db.get<RealArray>("hotSpotData");
  RealArray & shock        = db.get<RealArray>("shock");
  RealArray & wallValues   = db.get<RealArray>("wallValues");
  RealArray & uLeft        = db.get<RealArray>("uLeft");
  RealArray & uCenter      = db.get<RealArray>("uCenter");
  RealArray & uRight       = db.get<RealArray>("uRight");
  RealArray & perturbData  = db.get<RealArray>("perturbData");

  realCompositeGridFunction *&uSFPointer = db.get<realCompositeGridFunction*>("uSFPointer");
  CompositeGrid *&cgSFPointer = db.get<CompositeGrid*>("cgSFPointer");

  int & numberOfSmooths = db.get<int>("numberOfSmooths");
  int & numberOfBubbles = db.get<int>("numberOfBubbles");
  aString & profileFileName = db.get<aString>("profileFileName");

  real & shockLoc   = db.get<real>("shockLoc");
  real & wallCenter = db.get<real>("wallCenter");
  real & wallThick  = db.get<real>("wallThick");
  real & xJump1     = db.get<real>("xJump1");
  real & xJump2     = db.get<real>("xJump2");
  real & rhoInert   = db.get<real>("rhoInert");
  real & pencilTheta= db.get<real>("pencilTheta");
  real *rpar        = db.get<real[2]>("rpar");
  real &tGradient   = db.get<real>("tGradient");


  if( option==profileFromADataFile || option==profileFromADataFileWithPerturbation )
  {
    GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
    int ierr=initialConditionsFromAProfile( profileFileName,u,parameters,gi,rpar );
    if( ierr!=0 )
    {
      throw "error";
    }
  }

  if( option!=profileFromADataFile )
  {
    // Loop over all grids and assign values to all components.
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      c.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
      #ifndef USE_PPP
        realArray & vertex = c.vertex();  // grid points
        realArray & ug = u[grid];
      #else
        // In parallel, we operate on the arrays local to each processor
        realSerialArray vertex; getLocalArrayWithGhostBoundaries(c.vertex(),vertex);
        realSerialArray ug;     getLocalArrayWithGhostBoundaries(u[grid],ug);
      #endif

      Index I1,I2,I3;
      getIndex( c.dimension(),I1,I2,I3 );          // all points including ghost points.
      // getIndex( c.gridIndexRange(),I1,I2,I3 );  // boundary plus interior points.
      #ifdef USE_PPP
        // restrict bounds to local processor, include ghost
        bool ok = ParallelUtility::getLocalArrayBounds(u[grid],ug,I1,I2,I3,1);   
        if( !ok ) continue;  // no points on this processor
      #endif

      if( option==uniformState )
      {
	ug(I1,I2,I3,rc)=1.;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=1.;   // temperature is 1
	ug(I1,I2,I3,sc)=0.;   // pure fuel => lambda=0

      }
      else if ( option==bubbles )
      {
	// define a set of bubbles -- circular regions with constant properties.
	int n;
	for( n=0; n<numberOfComponents; n++ )
	  ug(I1,I2,I3,n)=uniform(n);

	int b;
	for( b=0; b<numberOfBubbles; b++ )
	{
	  RealArray radius;
	  if( numberOfDimensions==2 )
	    radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-bubbleCentre(b,0))+
			   SQR(vertex(I1,I2,I3,axis2)-bubbleCentre(b,1)) );
	  else
	    radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-bubbleCentre(b,0))+
			   SQR(vertex(I1,I2,I3,axis2)-bubbleCentre(b,1))+
			   SQR(vertex(I1,I2,I3,axis3)-bubbleCentre(b,2)) );
	  where( radius<=bubbleRadius(b) )
	  {
	    for( n=0; n<numberOfComponents; n++ )
	      ug(I1,I2,I3,n)=bubbleValues(b,n);
	  }
	}
      }
      else if( option==couetteProfile )
      {
        // Couette-Poiseuille flow with a divergence free perturbation
        //  u = u0*(y-ya)(yb-y)/[.5*(yb-ya)]^2 + u1*(y-ya)/(yb-ya) 
        //    + u2*      sin(ax*pi*x/(yb-ya))*cos(ay*pi*(y-ya)/(yb-ya))
        //  v =-u2*ax/ay*cos(ax*pi*x/(yb-ya))*sin(ay*pi*(y-ya)/(yb-ya))

	RealArray & couetteData = db.get<RealArray>("couetteData");

        real u0=couetteData(0), u1=couetteData(1), u2=couetteData(2);
        real ax=couetteData(3), ay=couetteData(4);
	real ya=couetteData(5), yb=couetteData(6);
	
	real u0p = u0/SQR(.5*(yb-ya));
	real u1p = u1/(yb-ya);
        real axp=ax*Pi/(yb-ya);
	real ayp=ay*Pi/(yb-ya);
	
	// printF("couetteProfile: u0=%g, u1=%g, u2=%g, ax=%g, ay=%g, ya=%g, yb=%g, axp=%g, ayp=%g\n",
	//       u0,u1,u2,ax,ay,ya,yb,axp,ayp);

	ug=0.;
	ug(I1,I2,I3,uc)=u0p*(vertex(I1,I2,I3,axis2)-ya)*(yb-vertex(I1,I2,I3,axis2)) + 
                        u1p*(vertex(I1,I2,I3,axis2)-ya) +
	                  u2       *sin(axp*vertex(I1,I2,I3,axis1))*cos(ayp*(vertex(I1,I2,I3,axis2)-ya));
	ug(I1,I2,I3,vc)=-(u2*ax/ay)*cos(axp*vertex(I1,I2,I3,axis1))*sin(ayp*(vertex(I1,I2,I3,axis2)-ya));

        // printF("couetteProfile: v : min=%g, max=%g\n",min(ug(I1,I2,I3,vc)),max(ug(I1,I2,I3,vc)));
	
      }
      else if( option==temperatureGradient )
      {
// *        ug(I1,I2,I3,rc)=1.;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
        if( numberOfDimensions==3 )
	  ug(I1,I2,I3,wc)=0.;
	if( sc>0 )
  	ug(I1,I2,I3,sc)=0.;   // pure fuel => lambda=0

        real sigma=hotSpotData(3);
//        RealArray distance;
//        distance=abs(vertex(I1,I2,I3,axis1)-hotSpotData(0));
//        distance=sqrt(SQR(vertex(I1,I2,I3,axis1)-hotSpotData(0)));
//        distance=sqrt(SQR(vertex(I1,I2,I3,axis1)-.5));
//        ug(I1,I2,I3,tc)=1.-sigma*distance;
//        ug(I1,I2,I3,tc)=1.-sigma*vertex(I1,I2,I3,axis1);
// *        ug(I1,I2,I3,tc)=vertex(I1,I2,I3,axis1);

//      ug(I1,I2,I3,tc)=1.-.006*vertex(I1,I2,I3,axis1);
//      ug(I1,I2,I3,tc)=1.-.0228*vertex(I1,I2,I3,axis1);

        printF(">>> assign temperatureGradient sigma=%e center=(%e,%e)\n",sigma,hotSpotData(0),hotSpotData(1));

	RealArray distance;
	if( (c.gridIndexRange(1,1)-c.gridIndexRange(0,1)) <3 )
	{
	  // pseudo 2d case
	  printF(" ****** USE 1D TEMPERATURE GRADIENT *******\n");
	  distance=fabs(vertex(I1,I2,I3,axis1)-hotSpotData(0));
	}
	else
	{
          if( numberOfDimensions==2 )
	  {
	    printF(" ****** USE 2D TEMPERATURE GRADIENT *******\n");
	    distance=sqrt(SQR(vertex(I1,I2,I3,axis1)-hotSpotData(0))
			 +SQR(vertex(I1,I2,I3,axis2)-hotSpotData(1)));
	  }
	  else
	  {
	    printF(" ****** USE 3D TEMPERATURE GRADIENT *******\n");
	    distance=sqrt(SQR(vertex(I1,I2,I3,axis1)-hotSpotData(0))
			 +SQR(vertex(I1,I2,I3,axis2)-hotSpotData(1))
			 +SQR(vertex(I1,I2,I3,axis3)-hotSpotData(2)));
	  }
	}

	ug(I1,I2,I3,tc)=1.-sigma*distance;
	ug(I1,I2,I3,rc)=1./ug(I1,I2,I3,tc);
//        ug(I1,I2,I3,sc+1)=0.;                      // vs=0
//        ug(I1,I2,I3,sc+2)=1./ug(I1,I2,I3,rc); // vg=1/rho
      }
      else if( option==ig )
      {
        ug(I1,I2,I3,rc)=1.;   // density is 1
	ug(I1,I2,I3,uc)=-0.2;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
        if( numberOfDimensions==3 )
	  ug(I1,I2,I3,wc)=0.;
	ug(I1,I2,I3,sc)=0.;   // pure fuel => lambda=0
	ug(I1,I2,I3,tc)=0.;
        ug(I1,I2,I3,sc+1)=1.;                      // vs=vs0
        ug(I1,I2,I3,sc+2)=4.;                      // vg=vg0
      }
      else if( option==convergingShock )
      {
        // set ambient state
        real gam=parameters.dbase.get<real>("gamma");
	
        // Cylindrically converging shock for Veronica

        // set ambient state
        //real gam=1.4;  // air
        real r0, p0, a0;
        r0=gam;              // density (rho)
        p0=1.;               // pressure
        a0=sqrt(gam*p0/r0);  // speed of sound

        ug(I1,I2,I3,rc)=r0;
        ug(I1,I2,I3,uc)=0.;
	ug(I1,I2,I3,vc)=0.;
        if( numberOfDimensions==3 ) ug(I1,I2,I3,wc)=0;
        ug(I1,I2,I3,tc)=p0/r0;

     	RealArray radius, an, bn;
	radius=sqrt(SQR(vertex(I1,I2,I3,axis1))+SQR(vertex(I1,I2,I3,axis2)));
        an=vertex(I1,I2,I3,axis1)/radius;
        bn=vertex(I1,I2,I3,axis2)/radius;
      
        real machNumber=2.4; // ************** change me **********
        real r1, q1, p1;
      
        // compute post-shock state
        r1=r0*(gam+1)*machNumber*machNumber/((gam-1)*machNumber*machNumber+2);
        q1=a0*2*(machNumber*machNumber-1)/((gam+1)*machNumber);
        p1=p0*(1.+2*gam*(machNumber*machNumber-1)/(gam+1));
      
	bool perturbed=numberOfDimensions==3; // fix this 

        real radius0=65.;
	if( !perturbed )
	{
	  where( radius>radius0 )
	  {
	    ug(I1,I2,I3,rc)=r1;
	    ug(I1,I2,I3,uc)=-q1*an;
	    ug(I1,I2,I3,vc)=-q1*bn;
	    ug(I1,I2,I3,tc)=p1/r1;
	  }
	}
        else 
	{
          // here is a perturbed 3d front
          RealArray z(I1,I2,I3);
          real za=min(vertex(I1,I2,I3,axis3));
	  real zb=max(vertex(I1,I2,I3,axis3));
	  z = (vertex(I1,I2,I3,axis3)-za)/(zb-za);  // normalized z 

	  RealArray r0(I1,I2,I3);
          real delta=5.;  // convOct domain is big 
	  r0 = radius0 + delta*(z-.5);  // here is a linear perturbation in the vertical direction
	  
	  where( radius>r0 )
	  {
	    ug(I1,I2,I3,rc)=r1;
	    ug(I1,I2,I3,uc)=-q1*an;
	    ug(I1,I2,I3,vc)=-q1*bn;
	    ug(I1,I2,I3,tc)=p1/r1;
	  }
	}
	
      }
      else if ( option==gravitationallyStratified )
      {
	// Define a gravitationally stratified density in the y direction

        // p_y = r*g,  p=r*R*T, ->  r_y = (g/R*T) * y -> r = r0 * exp( beta*(y-y0) )
	for( int n=0; n<numberOfComponents; n++ )
	  ug(I1,I2,I3,n)=uniform(n);

	const real rho0=bubbleValues(0);
	const real y0  =bubbleValues(1);
        const real g1 = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1];
	
        const real beta = g1/(parameters.dbase.get<real >("Rg")*uniform(tc));
	ug(I1,I2,I3,rc)=rho0*exp( beta*(vertex(I1,I2,I3,1)-y0) );
        

      }
      else if( option == solidBody )
      {
	real pi=3.141592653;
	real r0 = 0.15;
	real x0 = 0.5;
	real y0 = 0.75;
	real p0 = 1e-0;
	ug(I1,I2,I3,rc) = 1.0;
	ug(I1,I2,I3,uc) = 0.5-vertex(I1,I2,I3,axis2);
	ug(I1,I2,I3,vc) =-0.5+vertex(I1,I2,I3,axis1);

	// setup slotted cylinder
	RealArray radius = 1.0/r0*sqrt(SQR(vertex(I1,I2,I3,axis1)-x0)+SQR(vertex(I1,I2,I3,axis2)-y0));
	where( (sqrt(SQR(vertex(I1,I2,I3,axis1)-x0)) >= 0.025 || vertex(I1,I2,I3,axis2) >= 0.85) &&
	       radius <= 1.0 )
	{
	  ug(I1,I2,I3,rc) = 2.0;
	}

	// setup cone
	x0 = 0.5;
	y0 = 0.25;
	radius = 1.0/r0*sqrt(SQR(vertex(I1,I2,I3,axis1)-x0)+SQR(vertex(I1,I2,I3,axis2)-y0));
	where( radius <= 1.0 )
	{
	  ug(I1,I2,I3,rc) = 2.0-1.0/r0*sqrt(SQR(vertex(I1,I2,I3,axis1)-x0)+SQR(vertex(I1,I2,I3,axis2)-y0));
	}

	// setup hump
	x0 = 0.25;
	y0 = 0.5;
	radius = 1.0/r0*sqrt(SQR(vertex(I1,I2,I3,axis1)-x0)+SQR(vertex(I1,I2,I3,axis2)-y0));
	where( radius <= 1.0 )
	{
	  ug(I1,I2,I3,rc) = 1.0+0.25*(1.0+cos(pi*radius));
	}

	ug(I1,I2,I3,tc) = p0/ug(I1,I2,I3,rc); // trick the code ito doing essentially advection

      }
      else if( option==LANLCookOff )
      {
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;

	real r1=.1;
	real r2=.2;
	real f1=r2/(r2-r1);
	real f2=1./(r2-r1);
	real pi=3.141592653;
	RealArray radius, xi, eta;
	radius=sqrt(SQR(vertex(I1,I2,I3,axis1))+SQR(vertex(I1,I2,I3,axis2)));
	xi=vertex(I1,I2,I3,axis1)/radius;
	eta=acos(xi)/pi;
	xi=f1-f2*radius;

	real sigma=.01;
	real chi=12.;
	real aa=.993;
	real bb=.01;
//      ug(I1,I2,I3,tc)=1-sigma*xi;     // radial temperature gradient
//      ug(I1,I2,I3,tc)=aa*(1-sigma*xi)+(1-aa*(1-sigma*xi)-.25*bb*(1-4*xi*(1-xi)))*exp(-chi*eta);  // cookOff4.show
//      ug(I1,I2,I3,tc)=aa*(1-sigma*xi)+(1-aa*(1-sigma*xi)-.25*bb*(1-4*xi*(1-xi)))*exp(-chi*eta*eta);  // cookOff 5.show and cookOff6.show

//    ug(I1,I2,I3,tc)=1-.03*eta;              // cookOff8.show and cookOff10.show
//    ug(I1,I2,I3,tc)=1-.01*eta;              // cookOff11.show
//    ug(I1,I2,I3,tc)=1-.1*eta;              // cookOff12.show
//    ug(I1,I2,I3,tc)=1-.06*eta;              // cookOff13.show

	ug(I1,I2,I3,tc)=1-tGradient*eta;
	ug(I1,I2,I3,rc)=1./ug(I1,I2,I3,tc);

//      real chi=1.;
//      real delta=.125;
//      ug(I1,I2,I3,sc)=delta*(1.-16*SQR(xi*(1.-xi))*exp(-chi*eta));    // distribution of product
	ug(I1,I2,I3,sc)=0.;

      }
      else if( option==LANLCookOff2 )
      {
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;

	real r0=.1;
	real r1=.2;
	real t0=.93; // .922;
	real t1=.95; // .934;
	real t2=.97; // .967;
	real gam=8;
	real alp=(1-t2)/(1-exp(-gam));
	real bet=1-alp;
	real pi=3.141592653;
	RealArray radius, y, tc0, sig, q0, q1, a, b;
	radius=sqrt(SQR(vertex(I1,I2,I3,axis1))+SQR(vertex(I1,I2,I3,axis2)));
	y=acos(vertex(I1,I2,I3,axis1)/radius)/pi;

	real c=(r0+r1)/2;
	tc0=alp*exp(-gam*y*y)+bet;
	sig=40-38*y;

	real p0=r0-c;
	real p1=r1-c;
	q0=c*log(cosh(sig*(r0/c-1)))/sig;
	q1=c*log(cosh(sig*(r1/c-1)))/sig;
	a=((t0-tc0)*q1-(t1-tc0)*q0)/(p0*q1-p1*q0);
	b=(p0*(t1-tc0)-p1*(t0-tc0))/(p0*q1-p1*q0);
	ug(I1,I2,I3,tc)=tc0+a*(radius-c)+b*c*log(cosh(sig*(radius/c-1)))/sig;
	ug(I1,I2,I3,rc)=1./ug(I1,I2,I3,tc);
	ug(I1,I2,I3,sc)=0.;

      }
      else if( option==LX17Puck )
      {
	//real p0=0.0, p2=0.240297, r0=1.0;
	real p0=0.0, p2=0.28, r0=1.0;
	//real p0=0.0, p2=0.8, r0=.4;

        ug(I1,I2,I3,rc)=1.;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	if( numberOfComponents <= 7 )
	{
	  ug(I1,I2,I3,sc)=0.;   // pure fuel
	  ug(I1,I2,I3,sc+1)=1.; // vs0
	  ug(I1,I2,I3,sc+2)=5.; // vg0
	}
	else if( numberOfComponents <= 9 )
	{
	  ug(I1,I2,I3,sc)=1.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	  ug(I1,I2,I3,sc+2)=1.; // vi0
	  ug(I1,I2,I3,sc+3)=1.; // vs0
	  ug(I1,I2,I3,sc+4)=5.; // vg0
	}
	else
	{
	  ug(I1,I2,I3,sc)=1.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	  ug(I1,I2,I3,sc+2)=0.0; // phi (pristine unshocked material)
	  ug(I1,I2,I3,sc+3)=1.; // vi0
	  ug(I1,I2,I3,sc+4)=1.; // vs0
	  ug(I1,I2,I3,sc+5)=5.; // vg0
	}

	RealArray radius;
	radius=sqrt(SQR(vertex(I1,I2,I3,axis1))+SQR(vertex(I1,I2,I3,axis2)));
        where( radius<r0 )
        {
          ug(I1,I2,I3,tc)=p2/1.0;   // Ash: edit this line
	  if( numberOfComponents <= 7 )
	  {
	    ug(I1,I2,I3,sc)=1.;
	    ug(I1,I2,I3,sc+1)=0.;
	    ug(I1,I2,I3,sc+2)=1.;
	  }
	  else if( numberOfComponents <= 9 )
	  {
	    ug(I1,I2,I3,sc)=1.0;   // mu
	    ug(I1,I2,I3,sc+1)=1.0; // lambda
	    ug(I1,I2,I3,sc+2)=1.0; 
	    ug(I1,I2,I3,sc+3)=0.0;
	    ug(I1,I2,I3,sc+4)=1.0;
	  }
	  else
	  {
	    ug(I1,I2,I3,sc)=1.0;    // mu
	    ug(I1,I2,I3,sc+1)=1.0;  // lambda
	    ug(I1,I2,I3,sc+2)=1.0;  // phi
	    ug(I1,I2,I3,sc+3)=1.0;
	    ug(I1,I2,I3,sc+4)=0.0;
	    ug(I1,I2,I3,sc+5)=1.0;
	  }
        }
	if( numberOfComponents > 7 )
	{
	  where( vertex(I1,I2,I3,axis1)<0.0 && vertex(I1,I2,I3,axis2)>-2.48049 )
	  {
	    ug(I1,I2,I3,rc)=1.1;   // density
	    //ug(I1,I2,I3,rc)=2.0;   // density
	    //ug(I1,I2,I3,rc)=2.5;   // density
	    ug(I1,I2,I3,uc)=0.;    // velocity is 0
	    ug(I1,I2,I3,vc)=0.;
	    ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	    if( numberOfComponents <= 9 )
	    {
	      ug(I1,I2,I3,sc)=0.0;   // mu
	      ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	      ug(I1,I2,I3,sc+2)=1.; // vi0
	      ug(I1,I2,I3,sc+3)=1.; // vs0
	      ug(I1,I2,I3,sc+4)=5.; // vg0
	    }
	    else
	    {
	      ug(I1,I2,I3,sc)=0.0;   // mu
	      ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	      ug(I1,I2,I3,sc+2)=0.0; // phi (pristine material)
	      ug(I1,I2,I3,sc+3)=1.; // vi0
	      ug(I1,I2,I3,sc+4)=1.; // vs0
	      ug(I1,I2,I3,sc+5)=5.; // vg0
	    }
	  }
	  where( radius<r0 )
	  {
	    ug(I1,I2,I3,sc+1)=1.0;
	    ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);
	    if( numberOfComponents >= 10 )
	    {
	      ug(I1,I2,I3,sc+2)=1.0;
	    }
	  }
	}

      }
      else if( option==rotatedShock )
      {
//        real sigma=50.;
        real sigma=200.;
        real x0=.1;
//        real cosTheta=.866025;   // Theta=30 degrees
//        real sinTheta=.5;
        real cosTheta=1.;   // Theta=0 degrees
        real sinTheta=0.;
//        real cosTheta=.97014;
//        real sinTheta=.24254;

        RealArray distance;
        distance=tanh(sigma*(cosTheta*(vertex(I1,I2,I3,axis1)-x0)-sinTheta*abs(vertex(I1,I2,I3,axis2))));

        real r1=1.;
        real r2=2.667;
        ug(I1,I2,I3,rc)=.5*(r1+r2)+.5*(r1-r2)*distance;

        real u1=0.;
        real u2=1.25*cosTheta;
        ug(I1,I2,I3,uc)=.5*(u1+u2)+.5*(u1-u2)*distance;

        real v1=0.;
        real v2=-1.25*sinTheta;
        ug(I1,I2,I3,vc)=.5*(v1+v2)+.5*(v1-v2)*distance;
        where (vertex(I1,I2,I3,axis2)<0)
        {
          real v1=0.;
          real v2=1.25*sinTheta;
          ug(I1,I2,I3,vc)=.5*(v1+v2)+.5*(v1-v2)*distance;
        }
        where (vertex(I1,I2,I3,axis2)==0)
        {
          ug(I1,I2,I3,vc)=0.;
        }

        real t1=.3572;
        real t2=.60267;
        ug(I1,I2,I3,tc)=.5*(t1+t2)+.5*(t1-t2)*distance;

      }
      else if( option==conicalShock )
      {
        real sigma=50.;
        real z0=.4;
        real cosTheta=.866025;   // Theta=30 degrees
        real sinTheta=.5;
//        real cosTheta=1.;   // Theta=0 degrees
//        real sinTheta=0.;

        RealArray distance;
        distance=tanh(sigma*(cosTheta*(vertex(I1,I2,I3,axis3)-z0)-sinTheta*sqrt(SQR(vertex(I1,I2,I3,axis1))+SQR(vertex(I1,I2,I3,axis2)))));

        real gamma=parameters.dbase.get<real>("gamma");
        real r2=1.;
        real r1=.125;
        ug(I1,I2,I3,rc)=.5*(r1+r2)+.5*(r1-r2)*distance;

        ug(I1,I2,I3,uc)=0.;
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,wc)=0.;

        real t2=1.;  // upstream state
        real t1=.8; // downstream state
        ug(I1,I2,I3,tc)=.5*(t1+t2)+.5*(t1-t2)*distance;

      }
      else if( option == planarInterfaceWithShock )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
//        realArray & vertex = c.vertex();  // grid points

        real p0=1., M0=2.;
        real xShock=.2, xInterface=.3;
        real gammaGas=1.4, rhoGas=1.4;
        real gammaSolid=5., piSolid=6857.8, rhoSolid=2296.68705;   // these are the new values
//        real gammaSolid=5., piSolid=6843.3, rhoSolid=2290.;  These are old values

        real rShock, uShock, pShock, cSolid;
        cSolid=sqrt(gammaSolid*(p0+piSolid)/rhoSolid);
        uShock=2.0*cSolid*(M0*M0-1.0)/((gammaSolid+1.0)*M0);
        pShock=p0+2.0*gammaSolid*(p0+piSolid)*(M0*M0-1.0)/(gammaSolid+1.0);
        rShock=rhoSolid*(gammaSolid+1.0)*M0*M0/((gammaSolid-1.0)*M0*M0+2.0);

        ug(I1,I2,I3,rc)=rhoGas;
        ug(I1,I2,I3,uc)=0.;
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,tc)=p0/rhoGas;
        ug(I1,I2,I3,sc)=1./(gammaGas-1.);
        ug(I1,I2,I3,sc+1)=0.;

        where(vertex(I1,I2,I3,axis1) < xInterface)
        {
          ug(I1,I2,I3,rc)=rhoSolid;
          ug(I1,I2,I3,tc)=p0/rhoSolid;
          ug(I1,I2,I3,sc)=1./(gammaSolid-1.);
          ug(I1,I2,I3,sc+1)=gammaSolid*piSolid/(gammaSolid-1.);
        }

        where(vertex(I1,I2,I3,axis1) < xShock)
        {
          ug(I1,I2,I3,rc)=rShock;
          ug(I1,I2,I3,uc)=uShock;
          ug(I1,I2,I3,tc)=pShock/rShock;
        }

      }
      else if( option == circularInterfaceWithShock )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
//        realArray & vertex = c.vertex();  // grid points

        real p0=1., M0=2.;
        real xShock=.2, xCenter=.5, rInterface=.2;
        real gammaGas=1.4, rhoGas=1.4;
        real gammaSolid=5., piSolid=6857.8, rhoSolid=2296.68705;   // these are the new values
//        real gammaSolid=5., piSolid=6843.3, rhoSolid=2290.;  These are old values

        real rShock, uShock, pShock, cSolid;
        cSolid=sqrt(gammaSolid*(p0+piSolid)/rhoSolid);
        uShock=2.0*cSolid*(M0*M0-1.0)/((gammaSolid+1.0)*M0);
        pShock=p0+2.0*gammaSolid*(p0+piSolid)*(M0*M0-1.0)/(gammaSolid+1.0);
        rShock=rhoSolid*(gammaSolid+1.0)*M0*M0/((gammaSolid-1.0)*M0*M0+2.0);

        ug(I1,I2,I3,rc)=rhoGas;
        ug(I1,I2,I3,uc)=0.;
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,tc)=p0/rhoGas;
        ug(I1,I2,I3,sc)=1./(gammaGas-1.);
        ug(I1,I2,I3,sc+1)=0.;

	RealArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-xCenter) + SQR(vertex(I1,I2,I3,axis2)) );
        where(radius > rInterface)
        {
          ug(I1,I2,I3,rc)=rhoSolid;
          ug(I1,I2,I3,tc)=p0/rhoSolid;
          ug(I1,I2,I3,sc)=1./(gammaSolid-1.);
          ug(I1,I2,I3,sc+1)=gammaSolid*piSolid/(gammaSolid-1.);
        }

        where(vertex(I1,I2,I3,axis1) < xShock)
        {
          ug(I1,I2,I3,rc)=rShock;
          ug(I1,I2,I3,uc)=uShock;
          ug(I1,I2,I3,tc)=pShock/rShock;
        }

      }
      else if( option == circularSmoothInterfaceWithShock2 )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
//        realArray & vertex = c.vertex();  // grid points

// rectangular grid is assumed
        real dx[3];
        c.getDeltaX(dx);

        real p0=1., M0=2.;
        real xShock=.2, xCenter=.5, rInterface=.2;
        real gammaGas=1.4, rhoGas=1.4;
        real gammaSolid=5., piSolid=6857.8, rhoSolid=2296.68705;   // these are the new values
//        real gammaSolid=5., piSolid=6843.3, rhoSolid=2290.;  These are old values

        real cvSolid=1.5, cvGas=0.717625;
        real mu1Solid=1./(gammaSolid-1.);
        real mu1Gas=1./(gammaGas-1.);
        real T0=p0*mu1Gas/(rhoGas*cvGas);

        real rShock, uShock, pShock, cSolid;
        cSolid=sqrt(gammaSolid*(p0+piSolid)/rhoSolid);
        uShock=2.0*cSolid*(M0*M0-1.0)/((gammaSolid+1.0)*M0);
        pShock=p0+2.0*gammaSolid*(p0+piSolid)*(M0*M0-1.0)/(gammaSolid+1.0);
        rShock=rhoSolid*(gammaSolid+1.0)*M0*M0/((gammaSolid-1.0)*M0*M0+2.0);

        ug(I1,I2,I3,rc)=rhoGas;
        ug(I1,I2,I3,uc)=0.;
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,tc)=p0/rhoGas;
        ug(I1,I2,I3,sc)=1./(gammaGas-1.);
        ug(I1,I2,I3,sc+1)=0.;

        int n1=20;
        int n2=20;

        int i1,i2,i3,k1,k2,sum;
        real xc,yc,xck,yck,frac,cvMix;
        for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
        {
          for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
          {
            for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
            {
              sum=0;
              xc=vertex(i1,i2,i3,axis1);
              yc=vertex(i1,i2,i3,axis2);

              if( sqrt( SQR(xc-xCenter+.5*dx[0]) + SQR(yc+.5*dx[1]) ) > rInterface )
                sum=sum+1;
              else
                sum=sum-1;

              if( sqrt( SQR(xc-xCenter+.5*dx[0]) + SQR(yc-.5*dx[1]) ) > rInterface )
                sum=sum+1;
              else
                sum=sum-1;

              if( sqrt( SQR(xc-xCenter-.5*dx[0]) + SQR(yc+.5*dx[1]) ) > rInterface )
                sum=sum+1;
              else
                sum=sum-1;

              if( sqrt( SQR(xc-xCenter-.5*dx[0]) + SQR(yc-.5*dx[1]) ) > rInterface )
                sum=sum+1;
              else
                sum=sum-1;

       // printF("sum=%d\n",sum);

              if( sum==4 || sum==-4 )
              {
                if( sum==4 )
                {
                  ug(i1,i2,i3,rc)=rhoSolid;
                  ug(i1,i2,i3,tc)=p0/rhoSolid;
                  ug(i1,i2,i3,sc)=1./(gammaSolid-1.);
                  ug(i1,i2,i3,sc+1)=gammaSolid*piSolid/(gammaSolid-1.);
                }
              }
              else
              {
                sum=0;
                for( k2=1; k2<=n2; k2++ )
                {
                  yck=yc+((k2-.5)/n2-.5)*dx[1];
                  for( k1=1; k1<=n1; k1++ )
                  {
                    xck=xc+((k1-.5)/n1-.5)*dx[0];
                    if( sqrt( SQR(xck-xCenter) + SQR(yck) ) > rInterface )
                      sum=sum+1;
                  }
                }
                frac=(1.*sum)/(n1*n2);
      //  printF("frac=%e\n",frac);
//                ug(i1,i2,i3,rc)=(1.-frac)*ug(i1,i2,i3,rc)+frac*rhoSolid;
//                ug(i1,i2,i3,tc)=(1.-frac)*ug(i1,i2,i3,tc)+frac*p0/rhoSolid;
//                ug(i1,i2,i3,sc)=(1.-frac)*ug(i1,i2,i3,sc)+frac/(gammaSolid-1.);
//                ug(i1,i2,i3,sc+1)=frac*gammaSolid*piSolid/(gammaSolid-1.);

                ug(i1,i2,i3,sc)=(1.-frac)*ug(i1,i2,i3,sc)+frac/(gammaSolid-1.);         // set mu1 and mu2 based of volume fraction (frac)
                ug(i1,i2,i3,sc+1)=frac*gammaSolid*piSolid/(gammaSolid-1.);

                cvMix=(1.-frac)*cvGas+frac*cvSolid;                                     // mixture Cv
                ug(i1,i2,i3,rc)=(ug(i1,i2,i3,sc)*p0+ug(i1,i2,i3,sc+1))/(T0*cvMix);      // set rho so that mixture temperature=T0
                ug(i1,i2,i3,tc)=p0/ug(i1,i2,i3,rc);
              }
            }
          }
        }

        where(vertex(I1,I2,I3,axis1) < xShock)
        {
          ug(I1,I2,I3,rc)=rShock;
          ug(I1,I2,I3,uc)=uShock;
          ug(I1,I2,I3,tc)=pShock/rShock;
        }

      }
      else if( option == circularSmoothInterfaceWithShock )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
//        realArray & vertex = c.vertex();  // grid points

        real p0=1., M0=2.;
        real xShock=.2, xCenter=.5, rInterface=.2;
        real gammaGas=1.4, rhoGas=1.4;
        real gammaSolid=5., piSolid=6857.8, rhoSolid=2296.68705;   // these are the new values
//        real gammaSolid=5., piSolid=6843.3, rhoSolid=2290.;  These are old values

        real cvSolid=1.5, cvGas=0.717625;
        real mu1Solid=1./(gammaSolid-1.);
        real mu2Solid=gammaSolid*piSolid*mu1Solid;
        real mu1Gas=1./(gammaGas-1.);
        real T0=p0*mu1Gas/(rhoGas*cvGas);

        real rShock, uShock, pShock, cSolid;
        cSolid=sqrt(gammaSolid*(p0+piSolid)/rhoSolid);
        uShock=2.0*cSolid*(M0*M0-1.0)/((gammaSolid+1.0)*M0);
        pShock=p0+2.0*gammaSolid*(p0+piSolid)*(M0*M0-1.0)/(gammaSolid+1.0);
        rShock=rhoSolid*(gammaSolid+1.0)*M0*M0/((gammaSolid-1.0)*M0*M0+2.0);

	RealArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-xCenter) + SQR(vertex(I1,I2,I3,axis2)) );

//        real sig=24000.;  // this value is grid dependent for h=(1/4000)/4
        real sig=6000.;  // this value is grid dependent for h=(1/4000)
	RealArray phi, cvMix;
	phi = .5 + .5*tanh(sig*(radius-rInterface));

//        ug(I1,I2,I3,rc)=rhoSolid*phi+rhoGas*(1.-phi);
//        ug(I1,I2,I3,uc)=0.;
//        ug(I1,I2,I3,vc)=0.;
//        ug(I1,I2,I3,tc)=(p0/rhoSolid)*phi+(p0/rhoGas)*(1.-phi);
//        ug(I1,I2,I3,sc)=(1./(gammaSolid-1.))*phi+(1./(gammaGas-1.))*(1.-phi);
//        ug(I1,I2,I3,sc+1)=(gammaSolid*piSolid/(gammaSolid-1.))*phi;

        ug(I1,I2,I3,uc)=0.;                                                     // velocity is zero
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,sc)=mu1Solid*phi+mu1Gas*(1.-phi);                           // set mu1 and mu2 according to phi
        ug(I1,I2,I3,sc+1)=mu2Solid*phi;

        cvMix=(1.-phi)*cvGas+phi*cvSolid;                                       // mixture Cv
        ug(I1,I2,I3,rc)=(ug(I1,I2,I3,sc)*p0+ug(I1,I2,I3,sc+1))/(T0*cvMix);      // set rho so that mixture temperature=T0
        ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);

        where(vertex(I1,I2,I3,axis1) < xShock)
        {
          ug(I1,I2,I3,rc)=rShock;
          ug(I1,I2,I3,uc)=uShock;
          ug(I1,I2,I3,tc)=pShock/rShock;
        }

      }
      else if( option == longEllipticalSmoothInterfaceWithShock )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
//        realArray & vertex = c.vertex();  // grid points

        real p0=1., M0=2.;
        real xShock=.2, xCenter=.5, rInterface=.2;
        real gammaGas=1.4, rhoGas=1.4;
        real gammaSolid=5., piSolid=6857.8, rhoSolid=2296.68705;   // these are the new values
//        real gammaSolid=5., piSolid=6843.3, rhoSolid=2290.;  These are old values

        real cvSolid=1.5, cvGas=0.717625;
        real mu1Solid=1./(gammaSolid-1.);
        real mu2Solid=gammaSolid*piSolid*mu1Solid;
        real mu1Gas=1./(gammaGas-1.);
        real T0=p0*mu1Gas/(rhoGas*cvGas);

        real rShock, uShock, pShock, cSolid;
        cSolid=sqrt(gammaSolid*(p0+piSolid)/rhoSolid);
        uShock=2.0*cSolid*(M0*M0-1.0)/((gammaSolid+1.0)*M0);
        pShock=p0+2.0*gammaSolid*(p0+piSolid)*(M0*M0-1.0)/(gammaSolid+1.0);
        rShock=rhoSolid*(gammaSolid+1.0)*M0*M0/((gammaSolid-1.0)*M0*M0+2.0);

	RealArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-xCenter) + 4.0*SQR(vertex(I1,I2,I3,axis2)) );

//        real sig=24000.;  // this value is grid dependent for h=(1/4000)/4
        real sig=6000.;  // this value is grid dependent for h=(1/4000)
	RealArray phi, cvMix;
	phi = .5 + .5*tanh(sig*(radius-rInterface));

//        ug(I1,I2,I3,rc)=rhoSolid*phi+rhoGas*(1.-phi);
//        ug(I1,I2,I3,uc)=0.;
//        ug(I1,I2,I3,vc)=0.;
//        ug(I1,I2,I3,tc)=(p0/rhoSolid)*phi+(p0/rhoGas)*(1.-phi);
//        ug(I1,I2,I3,sc)=(1./(gammaSolid-1.))*phi+(1./(gammaGas-1.))*(1.-phi);
//        ug(I1,I2,I3,sc+1)=(gammaSolid*piSolid/(gammaSolid-1.))*phi;

        ug(I1,I2,I3,uc)=0.;                                                     // velocity is zero
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,sc)=mu1Solid*phi+mu1Gas*(1.-phi);                           // set mu1 and mu2 according to phi
        ug(I1,I2,I3,sc+1)=mu2Solid*phi;

        cvMix=(1.-phi)*cvGas+phi*cvSolid;                                       // mixture Cv
        ug(I1,I2,I3,rc)=(ug(I1,I2,I3,sc)*p0+ug(I1,I2,I3,sc+1))/(T0*cvMix);      // set rho so that mixture temperature=T0
        ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);

        where(vertex(I1,I2,I3,axis1) < xShock)
        {
          ug(I1,I2,I3,rc)=rShock;
          ug(I1,I2,I3,uc)=uShock;
          ug(I1,I2,I3,tc)=pShock/rShock;
        }

      }
      else if( option == tallEllipticalSmoothInterfaceWithShock )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
//        realArray & vertex = c.vertex();  // grid points

        real p0=1., M0=2.;
        real xShock=.2, xCenter=.5, rInterface=.2;
        real gammaGas=1.4, rhoGas=1.4;
        real gammaSolid=5., piSolid=6857.8, rhoSolid=2296.68705;   // these are the new values
//        real gammaSolid=5., piSolid=6843.3, rhoSolid=2290.;  These are old values

        real cvSolid=1.5, cvGas=0.717625;
        real mu1Solid=1./(gammaSolid-1.);
        real mu2Solid=gammaSolid*piSolid*mu1Solid;
        real mu1Gas=1./(gammaGas-1.);
        real T0=p0*mu1Gas/(rhoGas*cvGas);

        real rShock, uShock, pShock, cSolid;
        cSolid=sqrt(gammaSolid*(p0+piSolid)/rhoSolid);
        uShock=2.0*cSolid*(M0*M0-1.0)/((gammaSolid+1.0)*M0);
        pShock=p0+2.0*gammaSolid*(p0+piSolid)*(M0*M0-1.0)/(gammaSolid+1.0);
        rShock=rhoSolid*(gammaSolid+1.0)*M0*M0/((gammaSolid-1.0)*M0*M0+2.0);

	RealArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-xCenter) + 0.25*SQR(vertex(I1,I2,I3,axis2)) );

//        real sig=48000.;  // this value is grid dependent for h=(1/4000)/4
        real sig=12000.;  // this value is grid dependent for h=(1/4000)
	RealArray phi, cvMix;
	phi = .5 + .5*tanh(sig*(radius-rInterface));

//        ug(I1,I2,I3,rc)=rhoSolid*phi+rhoGas*(1.-phi);
//        ug(I1,I2,I3,uc)=0.;
//        ug(I1,I2,I3,vc)=0.;
//        ug(I1,I2,I3,tc)=(p0/rhoSolid)*phi+(p0/rhoGas)*(1.-phi);
//        ug(I1,I2,I3,sc)=(1./(gammaSolid-1.))*phi+(1./(gammaGas-1.))*(1.-phi);
//        ug(I1,I2,I3,sc+1)=(gammaSolid*piSolid/(gammaSolid-1.))*phi;

        ug(I1,I2,I3,uc)=0.;                                                     // velocity is zero
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,sc)=mu1Solid*phi+mu1Gas*(1.-phi);                           // set mu1 and mu2 according to phi
        ug(I1,I2,I3,sc+1)=mu2Solid*phi;

        cvMix=(1.-phi)*cvGas+phi*cvSolid;                                       // mixture Cv
        ug(I1,I2,I3,rc)=(ug(I1,I2,I3,sc)*p0+ug(I1,I2,I3,sc+1))/(T0*cvMix);      // set rho so that mixture temperature=T0
        ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);

        where(vertex(I1,I2,I3,axis1) < xShock)
        {
          ug(I1,I2,I3,rc)=rShock;
          ug(I1,I2,I3,uc)=uShock;
          ug(I1,I2,I3,tc)=pShock/rShock;
        }

      }
      else if( option==bubbleShock)
      {
	// Set uniform state
	int n;
	for( n=0; n<numberOfComponents; n++ )
	{
	  ug(I1,I2,I3,n)=uniform(n);
	}
	
	// Set bubble
	RealArray radius;
        if( cg.numberOfDimensions()==2 )
	{
	  radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-bubbleCentre(0,0))+
			 SQR(vertex(I1,I2,I3,axis2)-bubbleCentre(0,1)) );
	}
	else
	{
	  radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-bubbleCentre(0,0))+
			 SQR(vertex(I1,I2,I3,axis2)-bubbleCentre(0,1))+
			 SQR(vertex(I1,I2,I3,axis3)-bubbleCentre(0,2)) );
	}
	
	where( radius<=bubbleRadius(0) )
	{
	  for( n=0; n<numberOfComponents; n++ )
	  {
	    ug(I1,I2,I3,n)=bubbleValues(0,n);
	  }
	}
	
	// Set Shock values
	where ( vertex(I1,I2,I3,axis1)<=shockLoc )
	{
	  for( n=0; n<numberOfComponents; n++ )
	  {
	    ug(I1,I2,I3,n)=shock(n);
	  }
	}
      }
      else if ( option==profileFromADataFileWithPerturbation )
      {
	// apply a perturbation to the profile read from a data file 
        real a0=perturbData(0);
        real f0=perturbData(1);
        real x0=perturbData(2);
        real beta=perturbData(3);
	
	ug(I1,I2,I3,rc)+=a0*sin(twoPi*f0*vertex(I1,I2,I3,1))*exp(-beta*SQR(vertex(I1,I2,I3,0)-x0));

      }
      else if (option == rateStick)
      {
	int n;
	// Set uniform background state
	for( n=0; n<numberOfComponents; n++ )
	  {
	    ug(I1,I2,I3,n)=uniform(n);
	  }

	// Set the wall (rate stick walls) values
	where ( vertex(I1,I2,I3,axis2)>wallCenter || 
		vertex(I1,I2,I3,axis2)<=-wallCenter )
	{
	  for( n=0; n<numberOfComponents; n++)
	  {
	    ug(I1,I2,I3,n)=wallValues(n);
	  }
	}

	// Set shock values
	//where ( vertex(I1,I2,I3,axis1)<=shockLoc )
	where ( vertex(I1,I2,I3,axis1)<=shockLoc && 
		(vertex(I1,I2,I3,axis2)<=wallCenter && 
		 vertex(I1,I2,I3,axis2)>-wallCenter) )
	{
	  for( n=0; n<numberOfComponents; n++ )
	  {
	    ug(I1,I2,I3,n)=shock(n);
	  }
	}
      }
      else if( option == rateStick2 )
      {
	real h = 2.0;
	//real rhoR=2.0, rhoI=9.0, press0=0.000001, press1=.5;
	//real rhoR=2.0, rhoI=2.0, press0=0.000001, press1=.5;
	real rhoR=2.0, rhoI=rhoInert, press0=0.000001, press1=.5;

	// unburt HE state
	ug(I1,I2,I3,rc)=rhoR;
	ug(I1,I2,I3,uc)=0.0;
	ug(I1,I2,I3,vc)=0.0;
	ug(I1,I2,I3,tc)=press0/rhoR;
	ug(I1,I2,I3,sc)=1.0;
	ug(I1,I2,I3,sc+1)=0.0;
	
	// inert confinement
	where( vertex(I1,I2,I3,axis1) > h )
	{
	  ug(I1,I2,I3,rc)=rhoI;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press0/ug(I1,I2,I3,rc);
	  ug(I1,I2,I3,sc)=0.0;
	  ug(I1,I2,I3,sc+1)=0.0;
	}
	where( vertex(I1,I2,I3,axis2) < h )
	{
	  ug(I1,I2,I3,rc)=rhoI;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press0/ug(I1,I2,I3,rc);
	  ug(I1,I2,I3,sc)=0.0;
	  ug(I1,I2,I3,sc+1)=0.0;
	}

	// "booster" state
	where( vertex(I1,I2,I3,axis2) <= 2*h && vertex(I1,I2,I3,axis2) >= h &&
	       vertex(I1,I2,I3,axis1) <= h )
	{
	  ug(I1,I2,I3,rc)=rhoR;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press1/ug(I1,I2,I3,rc);
	  ug(I1,I2,I3,sc)=1.0;
	  ug(I1,I2,I3,sc+1)=0.0;
	}

	// boundary values of the rate stick
	where( (abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && vertex(I1,I2,I3,axis2) > h )
	       || (abs(h-vertex(I1,I2,I3,axis2)) <= .00000001 && abs(vertex(I1,I2,I3,axis1)) < h) )
	{
	  real rhoB=0.5*(rhoI+rhoR);
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press0/rhoB;
	  ug(I1,I2,I3,rc)=rhoB;
	  ug(I1,I2,I3,sc)=0.5;
	  ug(I1,I2,I3,sc+1)=0.0;
	}
	// fix pressure along upper boundary of pressure slug
	where( abs(2*h-vertex(I1,I2,I3,axis2)) <= .00000001 && abs(vertex(I1,I2,I3,axis1)) < h)
	{
	  ug(I1,I2,I3,tc) = 0.5*(press0+press1)/ug(I1,I2,I3,rc);
	}
	// corner values of inert
	where( abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && abs(h-vertex(I1,I2,I3,axis2)) <= .00000001 )
	{
	  real rhoB=0.75*rhoI+0.25*rhoR;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=(0.75*press0+0.25*press1)/rhoB;
	  ug(I1,I2,I3,rc)=rhoB;
	  ug(I1,I2,I3,sc)=0.25;
	  ug(I1,I2,I3,sc+1)=0.0;
	}
	// fix pressure along the boundary near the pressure slug
	where( (abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && vertex(I1,I2,I3,axis2) > h
	       && vertex(I1,I2,I3,axis2) < 2*h) 
	       || (abs(h-vertex(I1,I2,I3,axis2)) <= .00000001 && abs(vertex(I1,I2,I3,axis1)) < h) )
	{
	  ug(I1,I2,I3,tc) = 0.5*(press0+press1)/ug(I1,I2,I3,rc);
	}
	// fix corner point where pressure slug ends
	where( abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && abs(2*h-vertex(I1,I2,I3,axis2)) <= .00000001 )
	{
	  ug(I1,I2,I3,tc) = (0.75*press0+0.25*press1)/ug(I1,I2,I3,rc);
	}
      }
      else if (option == pdethruster)
      {
        // ambient state
        real alph=10.;
        real a=4.44;  // 85% of the tube length
        real beta=50.;
        real b=-0.6;
	ug(I1,I2,I3,rc)=1.2015;
	ug(I1,I2,I3,uc)=0.0;
	ug(I1,I2,I3,vc)=0.0;
	ug(I1,I2,I3,tc)=0.8323;
	ug(I1,I2,I3,sc)=1.-.25*(1.-tanh(alph*(vertex(I1,I2,I3,axis1)-a)))*(1.+tanh(beta*(vertex(I1,I2,I3,axis2)-b)));

	// booster state
	where ( vertex(I1,I2,I3,axis1)<0.4999 &&
		vertex(I1,I2,I3,axis2)>-0.6 )
	{
	  ug(I1,I2,I3,tc)=8.4012;
	  ug(I1,I2,I3,sc)=1.0;
	}
      }
      else if (option == pdethruster3d)
      {
        // ambient state
        real alph=10.;
        real a=4.44;  // 85% of the tube length
        real beta=50.;
        real b=0.6;
	ug(I1,I2,I3,rc)=1.2015;
	ug(I1,I2,I3,uc)=0.0;
	ug(I1,I2,I3,vc)=0.0;
	ug(I1,I2,I3,wc)=0.0;
	ug(I1,I2,I3,tc)=0.8323;

	RealArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis2)) + SQR(vertex(I1,I2,I3,axis3)) );
	ug(I1,I2,I3,sc)=1.-.25*(1.-tanh(alph*(vertex(I1,I2,I3,axis1)-a)))*(1.-tanh(beta*(radius-b)));

	// booster state
	where ( vertex(I1,I2,I3,axis1)<0.4999 && radius<0.6 )
	{
	  ug(I1,I2,I3,tc)=8.4012;
	  ug(I1,I2,I3,sc)=1.0;
	}
      }
      else if (option == chev)
      {
	// Background stuff
	ug(I1,I2,I3,rc)=1.0;
	ug(I1,I2,I3,uc)=0.0;
	ug(I1,I2,I3,vc)=0.0;
	ug(I1,I2,I3,tc)=1.0;
	ug(I1,I2,I3,sc)=0.0;
	
	real x0 = 0.1;
	real x1 = 0.15;
	real y0 = 0.05;
	real slope1 = 1.;
	real slope2 = 1.;
	real shockLoc = .075;
	
	where( ((vertex(I1,I2,I3,axis1) >= x0 
		 && vertex(I1,I2,I3,axis2) <= slope1*(vertex(I1,I2,I3,axis1)-x0)+y0
		 && vertex(I1,I2,I3,axis2) >= slope2*(vertex(I1,I2,I3,axis1)-x1)+y0
		 && vertex(I1,I2,I3,axis2) >= y0)
		|| (vertex(I1,I2,I3,axis1) >= x0 
		    && vertex(I1,I2,I3,axis2) >= -slope1*(vertex(I1,I2,I3,axis1)-x0)+y0
		    && vertex(I1,I2,I3,axis2) <= -slope2*(vertex(I1,I2,I3,axis1)-x1)+y0
		    && vertex(I1,I2,I3,axis2) <= y0)) 
	       && vertex(I1,I2,I3,axis2) <= 0.1 && vertex(I1,I2,I3,axis2) >= 0.0)
	  {
	    // solid stuff
	    ug(I1,I2,I3,rc)=6.0; // density
	    ug(I1,I2,I3,uc)=0.0; // velocity=0
	    ug(I1,I2,I3,vc)=0.0;
	    ug(I1,I2,I3,tc)=1.0/ug(I1,I2,I3,rc); // pressure=1 ... mechanical equilibrium
	    ug(I1,I2,I3,sc)=1.; // solid
	  }
	
	real RHOS,US,VS,PS;
	RHOS = (1.4+1.0)*1.22*1.22/((1.4-1.0)*1.22*1.22+2.0);
	US = (sqrt(1.4)*2.0*(1.22*1.22-1.0)/((1.4+1.0)*1.22));
	VS = 0.;
	PS = 1.0+(2.0*1.4*(1.22*1.22-1.0))/(1.4+1.0);
	where (vertex(I1,I2,I3,axis1) <= shockLoc)
	  {
	    // shock stuff
	    ug(I1,I2,I3,rc) = RHOS;
	    ug(I1,I2,I3,uc) = US;
	    ug(I1,I2,I3,vc) = VS;
	    ug(I1,I2,I3,tc) = PS/(RHOS);
	    ug(I1,I2,I3,sc) = 0.0; // air => lambda=0
	  }
      }
      else if( option == aslam )
      {
	// Background ... just to be safe. This should all get overwritten
	ug(I1,I2,I3,rc)=1.0;
	ug(I1,I2,I3,uc)=0.0;
	ug(I1,I2,I3,vc)=0.0;
	ug(I1,I2,I3,tc)=1.0;
	ug(I1,I2,I3,sc)=0.0;
	
	// Region 1
	where( vertex(I1,I2,I3,axis1) <= 0 
	       && vertex(I1,I2,I3,axis2) > -0.267949*vertex(I1,I2,I3,axis1))
	{
	  ug(I1,I2,I3,rc)=2.0;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=.000001/2.0;
	  ug(I1,I2,I3,sc)=1.0;
	}

	// Region 2
	where( vertex(I1,I2,I3,axis1) > 0 
	       && vertex(I1,I2,I3,axis2) > -1.212217*vertex(I1,I2,I3,axis1))
	{
	  ug(I1,I2,I3,rc)=14.2130;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=2.90712;
	  ug(I1,I2,I3,tc)=.000001/14.2130;
	  ug(I1,I2,I3,sc)=0.0;
	}

	// Region 3
	where( vertex(I1,I2,I3,axis1) <= (-1.0/4.26794)*vertex(I1,I2,I3,axis2) 
	       && vertex(I1,I2,I3,axis2) <= -0.267949*vertex(I1,I2,I3,axis1))
	{
	  ug(I1,I2,I3,rc)=4.0;
	  ug(I1,I2,I3,uc)=1.0;
	  ug(I1,I2,I3,vc)=3.73205;
	  ug(I1,I2,I3,tc)=59.7128/4.0;
	  ug(I1,I2,I3,sc)=1.0;
	}

	// Region 4
	where( vertex(I1,I2,I3,axis1) > (-1.0/4.26794)*vertex(I1,I2,I3,axis2) 
	       && vertex(I1,I2,I3,axis2) <= -1.212217*vertex(I1,I2,I3,axis1))
	{
	  ug(I1,I2,I3,rc)=23.6883;
	  ug(I1,I2,I3,uc)=1.0;
	  ug(I1,I2,I3,vc)=3.73205;
	  ug(I1,I2,I3,tc)=59.7128/23.6883;
	  ug(I1,I2,I3,sc)=0.0;
	}
      }
      else if( option==ioan )
      {
	// Set inside state
        ug(I1,I2,I3,rc) = 1.4;
	ug(I1,I2,I3,uc) = 0.0;
	ug(I1,I2,I3,vc) = 0.0;
	ug(I1,I2,I3,tc) = 1.0/1.4;
	ug(I1,I2,I3,sc) = 1.0;
        
	// Set outside state
	RealArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-0.)+
		       SQR(vertex(I1,I2,I3,axis2)-0.) );
	where( SQR(radius)-0.09 > .000000000001 )
	{
	  ug(I1,I2,I3,rc) = 0.887565;
	  ug(I1,I2,I3,uc) = -.57735*(vertex(I1,I2,I3,axis1)/radius);
	  ug(I1,I2,I3,vc) = -.57735*(vertex(I1,I2,I3,axis2)/radius);
	  ug(I1,I2,I3,tc) = 0.191709/.887565;
	  ug(I1,I2,I3,sc) = 0.0;
	}
      }
      else if( option == jeffCorner )
      {
        assert( uSFPointer!=NULL && cgSFPointer!=NULL );
	realCompositeGridFunction & uSF = *uSFPointer;
	CompositeGrid & cgSF            = *cgSFPointer;
	printf ("**** Interpolating on grid %i\n", grid);

        if( true )
	{
	  interpolateAllPoints(uSF,u[grid]);  // *wdh* 050514
	}
	else
	{
	  
	  RealArray interpPosition(1,2);
	  RealArray uInterp(1,numberOfComponents+2);  

	  // Background ... just to be safe. This should all get overwritten
	  ug(I1,I2,I3,rc)=1.0;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=1.0;
	  ug(I1,I2,I3,sc)=0.0;
	  ug(I1,I2,I3,sc+1)=0.0;

	  int i1,i2,i3,num=-1,extrapolated=0;
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ ) 
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {       
		interpPosition(0,0) = vertex(i1,i2,i3,axis1);
		interpPosition(0,1) = vertex(i1,i2,i3,axis2);
		if( interpPosition(0,0) <= -.75 && (interpPosition(0,1) < 0.5 && interpPosition(0,1) > -0.5) )
		{
                  #ifndef USE_PPP
		    num=interpolatePoints(interpPosition,uSF,uInterp);
                  #else
		    Overture::abort("Fix this for parallel");
		  #endif
		  for( int n=0; n<numberOfComponents; n++)
		    ug(i1,i2,i3,n)=uInterp(0,n);
		  ug(i1,i2,i3,3)=uInterp(0,7)/uInterp(0,0);  // interpolate pressure instead of temperature
		  if( num < 0 )
		  {
		    /*ug(i1,i2,i3,rc)=2.0;
		      ug(i1,i2,i3,uc)=0.0;
		      ug(i1,i2,i3,vc)=0.0;
		      ug(i1,i2,i3,tc)=.0000005;
		      ug(i1,i2,i3,sc)=1.0;
		      ug(i1,i2,i3,sc+1)=1.0;*/
		    extrapolated++;
		  }
		  else if( num > 0 )
		  {
		    printf ("Error: interpolation/extrapolation was unsucessful!!\n");
		    exit(0);
		  }	     
		}
	      }
	}
	
	where( vertex(I1,I2,I3,axis1) > -.75 )
	{
	  ug(I1,I2,I3,rc) = 2.0;
	  ug(I1,I2,I3,uc) = 0.0;
	  ug(I1,I2,I3,vc) = 0.0;
	  ug(I1,I2,I3,tc) = .0000005;
	  ug(I1,I2,I3,4) = 1.0;
	  ug(I1,I2,I3,5) = 0.0;
	}
	where( vertex(I1,I2,I3,axis1) > -.75 && 
	       (vertex(I1,I2,I3,axis1) <= 0.2 && 
		(vertex(I1,I2,I3,axis2) >= .2 || vertex(I1,I2,I3,axis2) < -.2 )))
	{
	  /*ug(I1,I2,I3,rc) = 9.0;
	    ug(I1,I2,I3,uc) = 0.0;
	    ug(I1,I2,I3,vc) = 0.0;
	    ug(I1,I2,I3,tc) = .000001/9.0;
	    ug(I1,I2,I3,4) = 0.0;
	    ug(I1,I2,I3,5) = 1.0;*/
	  
	  ug(I1,I2,I3,rc) = 2.0;
	  ug(I1,I2,I3,uc) = 0.0;
	  ug(I1,I2,I3,vc) = 0.0;
	  ug(I1,I2,I3,tc) = .0000005;
	  ug(I1,I2,I3,4) = 0.0;
	  ug(I1,I2,I3,5) = 1.0;
	  	  
	}
	// printf ("**** Done with grid %i. There were %i points extrapolated\n", grid, extrapolated);
	printf ("**** Done with grid %i.\n", grid);
      }
      else if( option == jeffRestart )
      {
        assert( uSFPointer!=NULL && cgSFPointer!=NULL );
	realCompositeGridFunction & uSF = *uSFPointer;
	CompositeGrid & cgSF            = *cgSFPointer;
	printf ("**** Interpolating on grid %i\n", grid);

        if( true )
	{
	  interpolateAllPoints(uSF,u[grid]);  // *wdh* 050514
	}

	printf ("**** Done with grid %i.\n", grid);
      }
      else if( option == aslamWeak )
      {
	//real h=2, rhoR=2.0, rhoI=2.0;
	real h=2, rhoR=2.0, rhoI=rhoInert, press0=.000001, press1=.5;
	// unburt HE state
	ug(I1,I2,I3,rc)=rhoR;
	ug(I1,I2,I3,uc)=0.0;
	ug(I1,I2,I3,vc)=0.0;
	ug(I1,I2,I3,tc)=press0/rhoR;
	ug(I1,I2,I3,sc)=1.0;
	ug(I1,I2,I3,sc+1)=0.0;
	
	// inert confinement
	where( vertex(I1,I2,I3,axis2) <= 80 &&
	       ( vertex(I1,I2,I3,axis1) > h || vertex(I1,I2,I3,axis1) < -h ))
	{
	  ug(I1,I2,I3,rc)=rhoI;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press0/ug(I1,I2,I3,rc);
	  ug(I1,I2,I3,sc)=0.0;
	  ug(I1,I2,I3,sc+1)=0.0;
	}
	where( vertex(I1,I2,I3,axis2) < h )
	{
	  ug(I1,I2,I3,rc)=rhoI;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press0/ug(I1,I2,I3,rc);
	  ug(I1,I2,I3,sc)=0.0;
	  ug(I1,I2,I3,sc+1)=0.0;
	}

	// "booster" state
	where( vertex(I1,I2,I3,axis2) <= 2*h && vertex(I1,I2,I3,axis2) >= h &&
	       vertex(I1,I2,I3,axis1) <= h && vertex(I1,I2,I3,axis1) >= -h )
	{
	  ug(I1,I2,I3,rc)=rhoR;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press1/ug(I1,I2,I3,rc);
	  ug(I1,I2,I3,sc)=1.0;
	  ug(I1,I2,I3,sc+1)=0.0;
	}

	// boundary values of the rate stick
	where( (abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && vertex(I1,I2,I3,axis2) > h )
	       || (abs(h-vertex(I1,I2,I3,axis2)) <= .00000001 && abs(vertex(I1,I2,I3,axis1)) < h) )
	  /*where( ((vertex(I1,I2,I3,axis1) <= h+.00000001 && vertex(I1,I2,I3,axis1) >= h-.00000001 ) 
	    || (vertex(I1,I2,I3,axis1) >= -h-.00000001 && vertex(I1,I2,I3,axis1) <= -h+.00000001))
	    &&  vertex(I1,I2,I3,axis2) >= h )*/
	{
	  real rhoB=0.5*(rhoI+rhoR);
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=press0/rhoB;
	  ug(I1,I2,I3,rc)=rhoB;
	  ug(I1,I2,I3,sc)=0.5;
	  ug(I1,I2,I3,sc+1)=0.0;
	}
	// fix pressure along upper boundary of pressure slug
	where( abs(2*h-vertex(I1,I2,I3,axis2)) <= .00000001 && abs(vertex(I1,I2,I3,axis1)) < h)
	{
	  ug(I1,I2,I3,tc) = 0.5*(press0+press1)/ug(I1,I2,I3,rc);
	}
	// corner values of inert
	where( abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && abs(h-vertex(I1,I2,I3,axis2)) <= .00000001 )
	{
	  real rhoB=0.75*rhoI+0.25*rhoR;
	  ug(I1,I2,I3,uc)=0.0;
	  ug(I1,I2,I3,vc)=0.0;
	  ug(I1,I2,I3,tc)=(0.75*press0+0.25*press1)/rhoB;
	  ug(I1,I2,I3,rc)=rhoB;
	  ug(I1,I2,I3,sc)=0.25;
	  ug(I1,I2,I3,sc+1)=0.0;
	}
	// fix pressure along the boundary near the pressure slug
	where( (abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && vertex(I1,I2,I3,axis2) > h
	       && vertex(I1,I2,I3,axis2) < 2*h) 
	       || (abs(h-vertex(I1,I2,I3,axis2)) <= .00000001 && abs(vertex(I1,I2,I3,axis1)) < h) )
	{
	  ug(I1,I2,I3,tc) = 0.5*(press0+press1)/ug(I1,I2,I3,rc);
	}
	// fix corner point where pressure slug ends
	where( abs(h-vertex(I1,I2,I3,axis1)) <= .00000001 && abs(2*h-vertex(I1,I2,I3,axis2)) <= .00000001 )
	{
	  ug(I1,I2,I3,tc) = (0.75*press0+0.25*press1)/ug(I1,I2,I3,rc);
	}
      }
      else if( option == shockPolar )
      {
	// Here is the shock polar initial conditions

	c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
	realArray & vertex = c.vertex();  // grid points
	
	// parameters theta, rhoa1, rhob1, P1, Dtotal
	real theta, rhoa1, rhob1, P1, Dtotal;
	theta  = 0.1;
	rhoa1  = 0.9;
	rhob1  = 1.1;
	P1     = 0.02;
	//	Dtotal = 0.3;
	Dtotal = 0.5;

	// now the shock polar solution with no post shock slip
	// phi1, phi2, D1, D2, rhoa2, rhob2, P2, S
	real phi1, phi2, D1, D2, rhoa2, rhob2, P2, S;
	phi1  = .2141538;
	phi2  = .2485619;
	D1    = .8516656;
	D2    = .8131895;
	rhoa2 = 1.344539;
	rhob2 = 1.574952;
	P2    = .2260847;
	S     = .5857269;

	// fluid "a" (confiner), pre-shock state
	where( vertex(I1,I2,I3,axis2) > 0.0 && 
	       vertex(I1,I2,I3,axis1) <= vertex(I1,I2,I3,axis2)*tan(phi1) )
	{
	  ug(I1,I2,I3,rc)   = rhoa1;
	  ug(I1,I2,I3,uc)   = D1-Dtotal;
	  ug(I1,I2,I3,vc)   = 0.0;
	  ug(I1,I2,I3,tc)   = P1/rhoa1;
	  if( numberOfComponents == 9 )
	  {
	    ug(I1,I2,I3,sc)   = 0.0;
	    ug(I1,I2,I3,sc+1) = 1.0;
	    ug(I1,I2,I3,sc+2) = 1./rhoa1;
	    ug(I1,I2,I3,sc+3) = 1.0;
	    ug(I1,I2,I3,sc+4) = 1.0;
	  }
	  else
	  {
	    ug(I1,I2,I3,sc)   = 1.0;
	    ug(I1,I2,I3,sc+1) = 1./rhoa1;
	    ug(I1,I2,I3,sc+2) = 1.0;
	  }
	}

	// fluid "a" (confiner), post-shock state
	where( vertex(I1,I2,I3,axis2) > vertex(I1,I2,I3,axis1)*tan(theta) &&
	       vertex(I1,I2,I3,axis1) > vertex(I1,I2,I3,axis2)*tan(phi1) )
	{
	  ug(I1,I2,I3,rc)   = rhoa2;
	  ug(I1,I2,I3,uc)   = S*cos(theta)-Dtotal;
	  ug(I1,I2,I3,vc)   = S*sin(theta);
	  ug(I1,I2,I3,tc)   = P2/rhoa2;
	  if( numberOfComponents == 9 )
	  {
	    ug(I1,I2,I3,sc)   = 0.0;
	    ug(I1,I2,I3,sc+1) = 1.0;
	    ug(I1,I2,I3,sc+2) = 1./rhoa2;
	    ug(I1,I2,I3,sc+3) = 1.0;
	    ug(I1,I2,I3,sc+4) = 1.0;
	  }
	  else
	  {
	    ug(I1,I2,I3,sc)   = 1.0;
	    ug(I1,I2,I3,sc+1) = 1./rhoa2;
	    ug(I1,I2,I3,sc+2) = 1.0;
	  }
	}

	// fluid "b" (HE) pre-shock state
	where( vertex(I1,I2,I3,axis2) <= 0.0 && 
	       vertex(I1,I2,I3,axis1) <= vertex(I1,I2,I3,axis2)*tan(phi2) )
	{
	  ug(I1,I2,I3,rc)   = rhob1;
	  ug(I1,I2,I3,uc)   = D2-Dtotal;
	  ug(I1,I2,I3,vc)   = 0.0;
	  ug(I1,I2,I3,tc)   = P1/rhob1;
	  if( numberOfComponents == 9 )
	  {
	    ug(I1,I2,I3,sc)   = 1.0;
	    ug(I1,I2,I3,sc+1) = 1.0;
	    ug(I1,I2,I3,sc+2) = 1.0;
	    ug(I1,I2,I3,sc+3) = 1./rhob1;
	    ug(I1,I2,I3,sc+4) = 1.0;
	  }
	  else
	  {
	    ug(I1,I2,I3,sc)   = 0.0;
	    ug(I1,I2,I3,sc+1) = 1.0;
	    ug(I1,I2,I3,sc+2) = 1.0/rhob1;
	  }
	}

	//fluid "b" (HE) post-shock state
	where( vertex(I1,I2,I3,axis2) <= vertex(I1,I2,I3,axis1)*tan(theta) &&
	       vertex(I1,I2,I3,axis1) > vertex(I1,I2,I3,axis2)*tan(phi2) )
	{
	  ug(I1,I2,I3,rc)   = rhob2;
	  ug(I1,I2,I3,uc)   = S*cos(theta)-Dtotal;
	  ug(I1,I2,I3,vc)   = S*sin(theta);
	  ug(I1,I2,I3,tc)   = P2/rhob2;
	  if( numberOfComponents == 9 )
	  {
	    ug(I1,I2,I3,sc)   = 1.0;
	    ug(I1,I2,I3,sc+1) = 1.0;
	    ug(I1,I2,I3,sc+2) = 1.0;
	    ug(I1,I2,I3,sc+3) = 1./rhob2;
	    ug(I1,I2,I3,sc+4) = 1.0;
	  }
	  else
	  {
	    ug(I1,I2,I3,sc)   = 0.0;
	    ug(I1,I2,I3,sc+1) = 1.0;
	    ug(I1,I2,I3,sc+2) = 1.0/rhob2;
	  }
	}
      }
      else if( option == twoJump )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
        realArray & vertex = c.vertex();  // grid points

        int n;
        // left state
        where( vertex(I1,I2,I3,axis1) <= xJump1 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uLeft(n);
          }
        }

        // middle state
        where( vertex(I1,I2,I3,axis1) <= xJump2 && vertex(I1,I2,I3,axis1) > xJump1 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uCenter(n);
          }
        }

        // right state
        where( vertex(I1,I2,I3,axis1) > xJump2 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uRight(n);
          }
        }      
      }

      else if( option == threeJump )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
        realArray & vertex = c.vertex();  // grid points

        int n;
        // left state
        where( vertex(I1,I2,I3,axis1) <= -xJump2 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uRight(n);
          }
          ug(I1,I2,I3,1)=-uRight(1);
        }

        // middle state (left)
        where( vertex(I1,I2,I3,axis1) <= xJump1 && vertex(I1,I2,I3,axis1) > -xJump2 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uCenter(n);
          }
	  ug(I1,I2,I3,1) = -uCenter(1);
        }

        // middle state (right)
        where( vertex(I1,I2,I3,axis1) <= xJump2 && vertex(I1,I2,I3,axis1) > xJump1 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uCenter(n);
          }
        }

        // right state
        where( vertex(I1,I2,I3,axis1) > xJump2 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uRight(n);
          }
        }      
      }

      else if( option == elliminate )
      {
	assert( uSFPointer!=NULL && cgSFPointer!=NULL );
	realCompositeGridFunction & uSF = *uSFPointer;
	CompositeGrid & cgSF            = *cgSFPointer;

	realArray interpPosition(1,2);
	realArray uTemp(1,numberOfComponents+2); 

	//interpolateAllPoints(uSF,ug);
	int i1,i2,i3,num=-1,extrapolated=0;
	for( i3=I3.getBase(); i3<=I3.getBound(); i3++ ) 
	  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {       
	      interpPosition(0,0) = vertex(i1,i2,i3,axis1);
	      interpPosition(0,1) = vertex(i1,i2,i3,axis2);
	      if( interpPosition(0,0) > xJump1 )
	      {
		num=interpolatePoints(interpPosition,uSF,uTemp);
		for( int n=0; n<numberOfComponents; n++)
		  ug(i1,i2,i3,n)=uTemp(0,n);
		//ug(i1,i2,i3,3)=uTemp(0,7)/uTemp(0,0);  // interpolate pressure instead of temperature
		if( num < 0 )
		{
		  extrapolated++;
		}
		else if( num > 0 )
		{
		  printf ("Error: interpolation/extrapolation was unsucessful!!\n");
		  exit(0);
		}	     
	      }
	    }

	interpPosition(0,0)=xJump1;
	interpPosition(0,1)=0.0;
	interpolatePoints(interpPosition,uSF,uTemp);

	where(vertex(I1,I2,I3,axis1) <= xJump1 )
	{
	  for( int n=0; n<numberOfComponents; n++)
	    ug(I1,I2,I3,n)=uTemp(0,n);
	  //ug(I1,I2,I3,3)=uTemp(0,7)/uTemp(0,0); // interpolate pressure instead of temperature
	}
	
      }
      else if ( option==bubblesShock )
      {
	// define a set of bubbles -- circular regions with constant properties, and a shock
	c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
	realArray & vertex = c.vertex();  // grid points

	int n;
	for( n=0; n<numberOfComponents; n++ )
	  ug(I1,I2,I3,n)=uniform(n);

	int b;
	for( b=0; b<numberOfBubbles; b++ )
	{
	  realArray radius;
	  radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-bubbleCentre(b,0))+
			 SQR(vertex(I1,I2,I3,axis2)-bubbleCentre(b,1)) );
	  where( radius<=bubbleRadius(b) )
	  {
	    for( n=0; n<numberOfComponents; n++ )
	      ug(I1,I2,I3,n)=bubbleValues(b,n);
	  }
	}
	where ( vertex(I1,I2,I3,axis1)<=shockLoc )
	{
	  for( n=0; n<numberOfComponents; n++ )
	  {
	    ug(I1,I2,I3,n)=shock(n);
	  }
	}
      }
      else if ( option==compliantCorner )
      {
	c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
	realArray & vertex = c.vertex();  // grid points

	//real xb=0.0, yb=-.65;  //xb,yb boundaries of confinement
	//real xb=10.0, yb=-.2;  //xb,yb boundaries of confinement
	////real xb=0.0, yb=-1.0;  //xb,yb boundaries of confinement
//	real xb=0.0, yb=-0.15;  //xb,yb boundaries of confinement
//	real x0=-3.0, y0=yb; // x0,y0 center of circle
//	real xs=-2.75;  // initial pressure slug

	real xb=0.0, yb=-1.5625;  //xb,yb boundaries of confinement, this gives a 12mm radius donor stick
	real x0=-6.51, y0=yb; // x0,y0 center of circle; x0 is the left boundary of donor stick which is at 50mm here
	real xs=-6.25;  // initial pressure slug

	real p0=0.0, p2=0.240297;
	real rhoI = 1.0;
	//real m=-1.5/2.0;
	real m=-1000000, eps=.000006;

	assert( numberOfComponents == 9 );  // make sure multi-component JWL computation

	// set background values
	ug(I1,I2,I3,rc)=1.;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	ug(I1,I2,I3,sc)=1.0;   // mu
	ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	ug(I1,I2,I3,sc+2)=1.; // vi0
	ug(I1,I2,I3,sc+3)=1.; // vs0
	ug(I1,I2,I3,sc+4)=5.; // vg0

	// set conditions for inert
	where( (vertex(I1,I2,I3,axis1) <= xb && vertex(I1,I2,I3,axis2) <= yb)
	    || (vertex(I1,I2,I3,axis2) <= yb && vertex(I1,I2,I3,axis1) <= (vertex(I1,I2,I3,axis2)-yb)/m ) )
	{
	  ug(I1,I2,I3,rc)=rhoI;   // density
	  ug(I1,I2,I3,uc)=0.;    // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=0.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda
	  ug(I1,I2,I3,sc+2)=1.; // vi0
	  ug(I1,I2,I3,sc+3)=1.; // vs0
	  ug(I1,I2,I3,sc+4)=5.; // vg0
	}

	// set conditions along inert/HE boundary for accurate geometry representation
//	where( (vertex(I1,I2,I3,axis1) <= xb && vertex(I1,I2,I3,axis2) <= yb+eps && vertex(I1,I2,I3,axis2) >  yb-eps)
//	       || (vertex(I1,I2,I3,axis2) <= yb && vertex(I1,I2,I3,axis1) <= (vertex(I1,I2,I3,axis2)-yb)/m+eps && vertex(I1,I2,I3,axis1) > (vertex(I1,I2,I3,axis2)-yb)/m-eps) )
//	{
//	  ug(I1,I2,I3,sc)=0.5;
//	}

	// set conditions for pressure slug
	where( vertex(I1,I2,I3,axis1) <= xs && vertex(I1,I2,I3,axis2) > yb )
	{
	  ug(I1,I2,I3,rc)=1.;   // density is 1
	  ug(I1,I2,I3,uc)=0.;   // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=1.0;   // mu
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	  ug(I1,I2,I3,sc+2)=1.0; // vi0
	  ug(I1,I2,I3,sc+3)=0.0; // vs0
	  ug(I1,I2,I3,sc+4)=1.0; // vg0
	}

	// set conditions for pressure cylinder in inert to avoid too much diffraction from IC
	realArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-x0)+
		       SQR(vertex(I1,I2,I3,axis2)-y0) );
	where( radius<=xs-x0 )
	{
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	}
      }
      else if ( option==compliantCornerDES )
      {
	c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
	realArray & vertex = c.vertex();  // grid points

	//real xb=0.0, yb=-.65;  //xb,yb boundaries of confinement
	//real xb=10.0, yb=-.66;  //xb,yb boundaries of confinement
	//real xb=0.0, yb=-1.0;  //xb,yb boundaries of confinement
	//real x0=-3.0, y0=yb; // x0,y0 center of circle
	//real xs=-2.75;  // initial pressure slug
	real xb=0.0, yb=-1.5625;  //xb,yb boundaries of confinement, this gives a 12mm radius donor stick
	real x0=-6.51, y0=yb; // x0,y0 center of circle; x0 is the left boundary of donor stick which is at 50mm here
	real xs=-6.25;  // initial pressure slug
 
	real p0=0.0, p2=0.240297;
	real rhoI = 1.0;
	//real m=-1.5/2.0;
	real m=-1000000, eps=.000006;

	assert( numberOfComponents == 10 );  // make sure multi-component desensitized JWL computation

	// set background values
	ug(I1,I2,I3,rc)=1.;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	ug(I1,I2,I3,sc)=1.0;   // mu
	ug(I1,I2,I3,sc)=1.0;   // 
	ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	ug(I1,I2,I3,sc+2)=0.0; // phi (pristine, unshocked fuel)
	ug(I1,I2,I3,sc+3)=1.; // vi0
	ug(I1,I2,I3,sc+4)=1.; // vs0
	ug(I1,I2,I3,sc+5)=5.; // vg0

	// set conditions for inert
	where( (vertex(I1,I2,I3,axis1) <= xb && vertex(I1,I2,I3,axis2) <= yb)
	    || (vertex(I1,I2,I3,axis2) <= yb && vertex(I1,I2,I3,axis1) <= (vertex(I1,I2,I3,axis2)-yb)/m ) )
	{
	  ug(I1,I2,I3,rc)=rhoI;   // density
	  ug(I1,I2,I3,uc)=0.;    // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=0.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda
	  ug(I1,I2,I3,sc+2)=0.0; // phi
	  ug(I1,I2,I3,sc+3)=1.; // vi0
	  ug(I1,I2,I3,sc+4)=1.; // vs0
	  ug(I1,I2,I3,sc+5)=5.; // vg0
	}

	// set conditions along inert/HE boundary for accurate geometry representation
//	where( (vertex(I1,I2,I3,axis1) <= xb && vertex(I1,I2,I3,axis2) <= yb+eps && vertex(I1,I2,I3,axis2) >  yb-eps)
//	       || (vertex(I1,I2,I3,axis2) <= yb && vertex(I1,I2,I3,axis1) <= (vertex(I1,I2,I3,axis2)-yb)/m+eps && vertex(I1,I2,I3,axis1) > (vertex(I1,I2,I3,axis2)-yb)/m-eps) )
//	{
//	  ug(I1,I2,I3,sc)=0.5;
//	}

	// set conditions for pressure slug
	where( vertex(I1,I2,I3,axis1) <= xs && vertex(I1,I2,I3,axis2) > yb )
	{
	  ug(I1,I2,I3,rc)=1.;   // density is 1
	  ug(I1,I2,I3,uc)=0.;   // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=1.0;   // mu
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	  ug(I1,I2,I3,sc+2)=1.0; // phi (desensitized)
	  ug(I1,I2,I3,sc+3)=1.0; // vi0
	  ug(I1,I2,I3,sc+4)=0.0; // vs0
	  ug(I1,I2,I3,sc+5)=1.0; // vg0
	}

	// set conditions for pressure cylinder in inert to avoid too much diffraction from IC
	realArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-x0)+
		       SQR(vertex(I1,I2,I3,axis2)-y0) );
	where( radius<=xs-x0 )
	{
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	  ug(I1,I2,I3,sc+2)=1.0; // phi
	}
      }
      else if ( option==makeFail )
      {
	c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
	realArray & vertex = c.vertex();  // grid points

	real xb=10.0, yb=-.2;  //xb,yb boundaries of confinement
	real x0=-3.0, y0=yb; // x0,y0 center of circle
	real xs=-2.75;  // initial pressure slug
	//real p0=0.0, p2=0.240297;
	real p0=0.0, p2=0.240297;
	real rhoI = 1.0;
	//real m=-1.5/2.0;
	real m=-1000000, eps=.000006;

	assert( numberOfComponents == 9 );  // make sure multi-component JWL computation

	// set background values
	ug(I1,I2,I3,rc)=1.;   // density is 1
	ug(I1,I2,I3,uc)=0.1;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	ug(I1,I2,I3,sc)=1.0;   // mu
	ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	ug(I1,I2,I3,sc+2)=1.; // vi0
	ug(I1,I2,I3,sc+3)=1.; // vs0
	ug(I1,I2,I3,sc+4)=5.; // vg0

	// set conditions for inert
	where( (vertex(I1,I2,I3,axis1) <= xb && vertex(I1,I2,I3,axis2) <= yb)
	    || (vertex(I1,I2,I3,axis2) <= yb && vertex(I1,I2,I3,axis1) <= (vertex(I1,I2,I3,axis2)-yb)/m ) )
	{
	  ug(I1,I2,I3,rc)=rhoI;   // density
	  ug(I1,I2,I3,uc)=0.;    // velocity is 0
	  ug(I1,I2,I3,vc)=-0.1;
	  ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=0.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda
	  ug(I1,I2,I3,sc+2)=1.; // vi0
	  ug(I1,I2,I3,sc+3)=1.; // vs0
	  ug(I1,I2,I3,sc+4)=5.; // vg0
	}

	// set conditions for pressure slug
	where( vertex(I1,I2,I3,axis1) <= xs && vertex(I1,I2,I3,axis2) > yb )
	{
	  ug(I1,I2,I3,rc)=1.;   // density is 1
	  ug(I1,I2,I3,uc)=-0.1;   // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=1.0;   // mu
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	  ug(I1,I2,I3,sc+2)=1.0; // vi0
	  ug(I1,I2,I3,sc+3)=0.0; // vs0
	  ug(I1,I2,I3,sc+4)=1.0; // vg0
	}

	// set conditions for pressure cylinder in inert to avoid too much diffraction from IC
	realArray radius;
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-x0)+
		       SQR(vertex(I1,I2,I3,axis2)-y0) );
	where( radius<=xs-x0 )
	{
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	}
      }
      else if( option == rotatedJump )
      {
        c.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
        realArray & vertex = c.vertex();  // grid points
	real theta;
	real v1,v1x,v2x,v1y,v2y,v2;
	real offSet=1.0/6.0;
	//real offSet=-2.0;

	theta=60;
	//theta=30;
	//theta=45;
	//theta=80;
	theta=theta*3.14159265358979323846/180;

	v1=sqrt(uLeft(uc)*uLeft(uc)+uLeft(vc)*uLeft(vc));
	v2=sqrt(uRight(uc)*uRight(uc)+uRight(vc)*uRight(vc));
	v1x=v1*sin(theta);
	v1y=-v1*cos(theta);
	v2x=v2*sin(theta);
	v2y=-v2*cos(theta);
        int n;
        // left state
        where( vertex(I1,I2,I3,axis2) >= tan(theta)*(vertex(I1,I2,I3,axis1)-offSet) )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uLeft(n);
          }
	  ug(I1,I2,I3,uc)=v1x;
	  ug(I1,I2,I3,vc)=v1y;
        }

        // right state
        where( vertex(I1,I2,I3,axis2) < tan(theta)*(vertex(I1,I2,I3,axis1)-offSet)+.00000001 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uRight(n);
          }
	  ug(I1,I2,I3,uc)=v2x;
	  ug(I1,I2,I3,vc)=v2y;
	}
	
	/*theta=-theta;
	v1y=-v1y;
        // left state
        where( vertex(I1,I2,I3,axis2) < 0 && vertex(I1,I2,I3,axis2) <= tan(theta)*(vertex(I1,I2,I3,axis1)-offSet) )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uLeft(n);
          }
	  ug(I1,I2,I3,uc)=v1x;
	  ug(I1,I2,I3,vc)=v1y;
        }

        // right state
        where( vertex(I1,I2,I3,axis2) < 0 && vertex(I1,I2,I3,axis2) > tan(theta)*(vertex(I1,I2,I3,axis1)-offSet)-.00000001 )
        {
          for( n=0; n<numberOfComponents; n++ )
          {
            ug(I1,I2,I3,n)=uRight(n);
          }
	  ug(I1,I2,I3,uc)=v2x;
	  ug(I1,I2,I3,vc)=v2y;
	  }*/
      }
      else if( option==pencil)
      {
	real p0=0.0, p2=0.240297;
	real rhoI=1.0, rhoR=1.0;
	real yb=-.975353307;
	real x0=-8.0, y0=yb;
	real xs=-7.75;
	real theta=30;

	// convert to radians
	theta=theta*3.14159265358979323846/180;

	assert( numberOfComponents == 9 );  // make sure multi-component JWL computation

	// set background values
	ug(I1,I2,I3,rc)=rhoR;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	ug(I1,I2,I3,sc)=1.0;   // mu
	ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	ug(I1,I2,I3,sc+2)=1.; // vi0
	ug(I1,I2,I3,sc+3)=1.; // vs0
	ug(I1,I2,I3,sc+4)=5.; // vg0

	// set conditions for inert
	where( vertex(I1,I2,I3,axis2) <= yb || 
	       (vertex(I1,I2,I3,axis2) <=  tan(theta)*vertex(I1,I2,I3,axis1)+yb && vertex(I1,I2,I3,axis1) > 0.0) )
        {
	  ug(I1,I2,I3,rc)=rhoI;   // density
	  ug(I1,I2,I3,uc)=0.;    // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=0.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda
	  ug(I1,I2,I3,sc+2)=1.; // vi0
	  ug(I1,I2,I3,sc+3)=1.; // vs0
	  ug(I1,I2,I3,sc+4)=5.; // vg0
	}

	// set conditions for pressure slug
	where( vertex(I1,I2,I3,axis1) <= xs && vertex(I1,I2,I3,axis2) > yb )
	{
	  ug(I1,I2,I3,rc)=1.;   // density is 1
	  ug(I1,I2,I3,uc)=0.;   // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=1.0;   // mu
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	  ug(I1,I2,I3,sc+2)=1.0; // vi0
	  ug(I1,I2,I3,sc+3)=0.0; // vs0
	  ug(I1,I2,I3,sc+4)=1.0; // vg0
	}

	// set conditions for pressure cylinder in inert to avoid too much diffraction from IC
	RealArray radius(I1,I2,I3);
	radius = sqrt( SQR(vertex(I1,I2,I3,axis1)-x0)+
		       SQR(vertex(I1,I2,I3,axis2)-y0) );
	where( radius<=xs-x0 )
	{
	  ug(I1,I2,I3,tc)=p2/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc+1)=1.0; // lambda (pure product)
	}
      }
      else if( option==pencilRestart)
      {
        assert( uSFPointer!=NULL && cgSFPointer!=NULL );
	realCompositeGridFunction & uSF = *uSFPointer;
	CompositeGrid & cgSF            = *cgSFPointer;
	printf ("**** Interpolating on grid %i\n", grid);

	interpolateAllPoints(uSF,u[grid]);  // *wdh* 050514

	real p0=0.0, p2=0.240297;
	real rhoI=1.0, rhoR=1.0;
	real yb=-.975353307;
	real x0=-8.0, y0=yb;
	real xs=-7.75;
	//real theta=15;
	real theta=pencilTheta; // in degrees

	// convert to radians
	theta=theta*3.14159265358979323846/180;

	assert( numberOfComponents == 9 );  // make sure multi-component JWL computation

	// set conditions for inert
	where( vertex(I1,I2,I3,axis1) > 3.0 )
	{
	  // set background values
	  ug(I1,I2,I3,rc)=rhoR;   // density is 1
	  ug(I1,I2,I3,uc)=0.;   // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=1.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda (pure fuel)
	  ug(I1,I2,I3,sc+2)=1.; // vi0
	  ug(I1,I2,I3,sc+3)=1.; // vs0
	  ug(I1,I2,I3,sc+4)=5.; // vg0
	}

	where( vertex(I1,I2,I3,axis1) > 3.0 &&
               (vertex(I1,I2,I3,axis2) <= yb || 
	       (vertex(I1,I2,I3,axis2) <=  tan(theta)*vertex(I1,I2,I3,axis1)+yb && vertex(I1,I2,I3,axis1) > 0.0)) )
        {
	  ug(I1,I2,I3,rc)=rhoI;   // density
	  ug(I1,I2,I3,uc)=0.;    // velocity is 0
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0
	  ug(I1,I2,I3,sc)=0.0;   // mu
	  ug(I1,I2,I3,sc+1)=0.0; // lambda
	  ug(I1,I2,I3,sc+2)=1.; // vi0
	  ug(I1,I2,I3,sc+3)=1.; // vs0
	  ug(I1,I2,I3,sc+4)=5.; // vg0
	}
      }
      else if( option==noh2D )
      {
	real p0=1.0e-14, theta,rad;
	int i1,i2,i3;

	// set background values
	ug(I1,I2,I3,rc)=1.0;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);   // pressure is 0

	for( i3=I3.getBase(); i3<=I3.getBound(); i3++ ) 
	  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      rad=vertex(i1,i2,i3,axis1)*vertex(i1,i2,i3,axis1)+vertex(i1,i2,i3,axis2)*vertex(i1,i2,i3,axis2);
	      rad=sqrt(rad);
	      //theta=atan(vertex(i1,i2,i3,axis2)/vertex(i1,i2,i3,axis1));
	      //ug(i1,i2,i3,uc)=-1.0*cos(theta);
	      //ug(i1,i2,i3,vc)=-1.0*sin(theta);
	      if( rad > 1.0e-6 )
	      {
		ug(i1,i2,i3,uc)=-1.0*vertex(i1,i2,i3,axis1)/rad;
		ug(i1,i2,i3,vc)=-1.0*vertex(i1,i2,i3,axis2)/rad;
	      }
	      else
	      {
		ug(i1,i2,i3,uc)=0.0;
		ug(i1,i2,i3,vc)=0.0;
	      }
	    }
      }
      else if( option==sedov2D )
      {
	real p0=1.0e-14;
        real gamma=parameters.dbase.get<real>("gamma");

	// set background values
	ug(I1,I2,I3,rc)=1.0;   // density is 1
	ug(I1,I2,I3,uc)=0.;   // velocity is 0
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,tc)=p0/ug(I1,I2,I3,rc);
	
	where( SQR(vertex(I1,I2,I3,axis1))+SQR(vertex(I1,I2,I3,axis2)) <= 0.00001 )
	{
	  //ug(I1,I2,I3,tc)=16.0*4.0*(gamma-1.0)*409.7;  // for dx=1.1 / (45*4)
	  ug(I1,I2,I3,tc)=4.0*(gamma-1.0)*409.7; // for dx=1.1 / 45
	}
	/*where( SQR(vertex(I1,I2,I3,axis1))+SQR(vertex(I1,I2,I3,axis2)) <= .0016 )
	  {
	  ug(I1,I2,I3,tc)=4.0/9.0*(gamma-1.0)*409.7; // for dx=1.1 / 45
	  }*/
      }
      else if ( option==ablProfile )
	{
	  
	  RealArray &values = db.get<RealArray>("ablValues");

	  real u_ref, z_ref, alpha,d;
	  u_ref = values(0);
	  z_ref = values(1);
	  alpha = values(2);
	  d     = values(3);
	  printf("***userDefinedInitialCondition grid %i, abl profile = %f %f %f %e\n",
		 grid,u_ref,z_ref,alpha,d);
	  
	  Range C(uc,uc+numberOfDimensions-1);
	  ug(I1,I2,I3,C) = 0.0;

#define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define X2(i0,i1,i2) (za+dz0*(i2-i2a))

	  //	  real d=1e-3;
	  if( c.isRectangular() )
	    {
	      real dx[3], xab[2][3];
	      c.getRectangularGridParameters(dx,xab);
	      const int i0a = c.gridIndexRange(0,0);
	      const int i1a = c.gridIndexRange(0,1);
	      const int i2a = c.gridIndexRange(0,2);
	      
	      const real xa = xab[0][0], dx0 = dx[0];
	      const real ya = xab[0][1], dy0 = dx[1];
	      const real za = xab[0][2], dz0 = dx[2];

	      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    {
		      real scale = X1(i1,i2,i3)>d ? 1 : X1(i1,i2,i3)*(2*d-X1(i1,i2,i3))/(d*d);
		      ug(i1,i2,i3,uc) = scale*u_ref * pow(X1(i1,i2,i3)/z_ref,alpha);
		    }
	    }
	  else
	    {	
	      c.update(MappedGrid::THEvertex);
	      const RealArray & vertex = c.vertex().getLocalArray();
	      //	      RealArray scale(Ib1,Ib2,Ib3);
	      
	      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    {
		      real scale = vertex(i1,i2,i3,1)>d ? 1 : vertex(i1,i2,i3,1)*(2*d-vertex(i1,i2,i3,1))/(d*d);
		      ug(i1,i2,i3,uc) = scale*u_ref * pow(abs(vertex(i1,i2,i3,1))/z_ref,alpha);
		    }
	      
	    }
	  
	} else if (option==linearBeamExactSolution) {

	const RealArray & vertex = c.vertex().getLocalArray();
	//	      RealArray scale(Ib1,Ib2,Ib3);
	
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
	double omegar = 0.8907148069, omegai = -0.9135887123e-2;
	for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ ) {
	  for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ ) {
	    for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ ) {
	 
	      double y = vertex(i1,i2,i3,1);
	      double x = vertex(i1,i2,i3,0);
	      
	      BeamModel::exactSolutionVelocity(x,y,0.0,k,H,
					       omegar,omegai, 
					       omega0,nu,
					       what,ug(i1,i2,i3,uc),
					       ug(i1,i2,i3,vc));

	      BeamModel::exactSolutionPressure(x,y,0.0,k,H,
					       omegar,omegai, 
					       omega0,nu,
					       what,ug(i1,i2,i3,pc));
	    }
	  }
	}

      }
      else
      {
	cout << "userDefinedInitialConditions: Unknown option =" << option << endl;
      }

    }
  }

  // These next values determine the pressureLevel constant for ASF:
  parameters.dbase.get<RealArray >("initialConditions")=uniform;

  return 0;
}




//\begin{>>DomainSolverInclude.tex}{\subsection{setupUserDefinedInitialConditions}}  
int DomainSolver::
setupUserDefinedInitialConditions()
//==============================================================================================
// /Description:
//    User defined initial conditions. This function is used to setup and define the initial conditions.
// The function userDefinedInitialConditions (above) is called to actually evaluate the initial conditions.
//  Rewrite or add new options to  this routine to supply your own initial conditions.
// Choose the "user defined" option from the initial conditions options to have this routine
// called.
// /Notes:
//  \begin{itemize}
//    \item You must fill in the realCompositeGridFunction u.
//    \item The `parameters' object holds many useful parameters.
//  \end{itemize}
//
// /Return values: 0=success, non-zero=failure.
//\end{DomainSolverInclude.tex}  
//==============================================================================================
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & rc = parameters.dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");  
  // const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  const int & sc = parameters.dbase.get<int >("sc");   //  mass fraction lambda
  //const int & pc = parameters.dbase.get<int >("pc");
  
  // Here is where parameters can be put to be saved in the show file:
  ListOfShowFileParameters & showFileParams = parameters.dbase.get<ListOfShowFileParameters>("showFileParams");

  // here is a menu of possible initial conditions
  aString menu[]=  
  {
    "bubbles",
    "couette profile",
    "uniform state",
    "LANL Cook-Off",
    "LANL Cook-Off 2",
    "temperature gradient",
    "temperature gradient centre",
    "temperature gradient sigma",
    "1d profile from a data file",
    "1d profile from a data file (smoothed)",
    "1d profile from a data file perturbed",
    "1d profile from a data file with changes",
    "rotated shock",
    "IG",
    "LX-17 puck",
    "conical shock",
    "converging shock",
    "planar interface with shock",
    "circular interface with shock",
    "circular (smooth) interface with shock",
    "circular (smooth) interface with shock 2",
    "long elliptical (smooth) interface with shock",
    "tall elliptical (smooth) interface with shock",
    "bubble with shock",
    "rate stick",
    "PDE thruster",
    "PDE thruster 3d",
    "chevron",
    "Tariq Aslam shock polar",
    "Ioan cylinder",
    "Jeffs Corner",
    "Jeff restart",
    "Aslam weak det",
    "2-D shock polar",
    "two jumps",
    "three jumps",
    "elliminate",
    "bubbles with shock",
    "compliant corner",
    "compliant corner, desensitized",
    "make fail",
    "compliant rate stick",
    "rotated jump",
    "pencil",
    "pencil restart",
    "Noh 2D",
    "Sedov 2D",
    "gravitationally stratified",
    "solid body rotation",
    "rate stick (reaction)",
    "abl profile",
    "linear beam exact solution",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user defined");

  // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedInitialConditionData") )
    parameters.dbase.get<DataBase >("modelData").put<DataBase>("userDefinedInitialConditionData");

  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedInitialConditionData");
  // first time through allocate variables 
  if( !db.has_key("option") )
  {
    UserDefinedInitialConditionOptions & option= db.put<UserDefinedInitialConditionOptions>("option",uniformState);

    db.put<RealArray>("uniform");
    db.put<RealArray>("bubbleCentre");
    db.put<RealArray>("bubbleRadius");
    db.put<RealArray>("bubbleValues");
    db.put<RealArray>("hotSpotData");
    db.put<RealArray>("shock");
    db.put<RealArray>("wallValues");
    db.put<RealArray>("uLeft");
    db.put<RealArray>("uCenter");
    db.put<RealArray>("uRight");
    db.put<RealArray>("perturbData"); 
    db.put<realCompositeGridFunction*>("uSFPointer");
    db.put<CompositeGrid*>("cgSFPointer");
    db.put<RealArray>("ablValues");
    realCompositeGridFunction *& uSFPointer = db.get<realCompositeGridFunction*>("uSFPointer");
    uSFPointer=NULL;
    CompositeGrid *& cgSFPointer = db.get<CompositeGrid*>("cgSFPointer");
    cgSFPointer=NULL;

    db.put<int>("numberOfSmooths",20);
    int & numberOfSmooths = db.get<int>("numberOfSmooths");
    assert( numberOfSmooths==20 );

    db.put<int>("numberOfBubbles",0);
    db.put<aString>("profileFileName");

    db.put<real>("shockLoc",0.);
    db.put<real>("wallCenter",0.);
    db.put<real>("wallThick",0.);
    db.put<real>("xJump1",0.);
    db.put<real>("xJump2",0.);

    db.put<real>("rhoInert",1.);
    db.put<real>("pencilTheta",0.);

    db.put<real[2]>("rpar");
    real *rpar = db.get<real[2]>("rpar");
    rpar[0]=0.; rpar[1]=0.;
  
    db.put<real>("tGradient",0.);
    
  }

  UserDefinedInitialConditionOptions & option= db.get<UserDefinedInitialConditionOptions>("option");

  RealArray & uniform      = db.get<RealArray>("uniform");
  RealArray & bubbleCentre = db.get<RealArray>("bubbleCentre");
  RealArray & bubbleRadius = db.get<RealArray>("bubbleRadius");
  RealArray & bubbleValues = db.get<RealArray>("bubbleValues");
  RealArray & hotSpotData  = db.get<RealArray>("hotSpotData");
  RealArray & shock        = db.get<RealArray>("shock");
  RealArray & wallValues   = db.get<RealArray>("wallValues");
  RealArray & uLeft        = db.get<RealArray>("uLeft");
  RealArray & uCenter      = db.get<RealArray>("uCenter");
  RealArray & uRight       = db.get<RealArray>("uRight");
  RealArray & perturbData  = db.get<RealArray>("perturbData");
  realCompositeGridFunction *& uSFPointer = db.get<realCompositeGridFunction*>("uSFPointer");
  CompositeGrid *& cgSFPointer = db.get<CompositeGrid*>("cgSFPointer");
  int & numberOfSmooths = db.get<int>("numberOfSmooths");
  int & numberOfBubbles = db.get<int>("numberOfBubbles");
  aString & profileFileName = db.get<aString>("profileFileName");

  real & shockLoc   = db.get<real>("shockLoc");
  real & wallCenter = db.get<real>("wallCenter");
  real & wallThick  = db.get<real>("wallThick");
  real & xJump1     = db.get<real>("xJump1");
  real & xJump2     = db.get<real>("xJump2");
  real & rhoInert   = db.get<real>("rhoInert");
  real & pencilTheta= db.get<real>("pencilTheta");
  real *rpar        = db.get<real[2]>("rpar");
  real &tGradient   = db.get<real>("tGradient");

  // default values for a background state:
  uniform.redim(numberOfComponents);
  uniform=0.;
  if( rc>=0 && rc<numberOfComponents )
    uniform(rc)=1.;
  if( tc>=0 && tc<numberOfComponents )
    uniform(tc)=1.;

 
  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="bubbles" )
    {
      // define a set of bubbles -- circular regions with constant properties.
      option=bubbles;

      gi.inputString(answer2,"Enter the number of bubbles");
      sScanF(answer2,"%i",&numberOfBubbles);  
      printF("numberOfBubbles = %i \n",numberOfBubbles);

      bubbleCentre.redim(numberOfBubbles,3); bubbleCentre=0.;
      bubbleRadius.redim(numberOfBubbles); bubbleRadius=1.;
      bubbleValues.redim(numberOfBubbles,numberOfComponents);bubbleValues=1.;

      gi.inputString(answer2,"Enter uniform background values as `r=2.,p=1., ...' ");
      parameters.inputParameterValues(answer2,"background values",uniform );

      int n,b;
      for( b=0; b<numberOfBubbles; b++ )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter radius and centre of bubble %i",b));
	sScanF(answer2,"%e %e %e %e",&bubbleRadius(b),&bubbleCentre(b,0),&bubbleCentre(b,1),&bubbleCentre(b,2));
	gi.inputString(answer2,sPrintF(buff,"Enter values for bubble %i as `r=2. p=1., ...'",b));

        RealArray values(numberOfComponents); values=uniform;
        parameters.inputParameterValues(answer2,"bubble values",values );
        for( n=0; n<numberOfComponents; n++ )
	{
	  if( values(n)!=(real)Parameters::defaultValue )
	    bubbleValues(b,n)=values(n);
	  else
	    bubbleValues(b,n)=0.;
	}
      }

      // Save parameters in the show file: (one can save real, int or a string)
      // These parameters will be displayed when the show file is read with plotStuff.
      // One can also access the parameters fro userDefinedDerivedFunctions.

      // Just for example, we save a real, int and string
      real bubbleRadius0=bubbleRadius(0);
      int numPulse=1;
      aString bubbleName="myBubble";
      showFileParams.push_back(ShowFileParameter("bubbleRadius0",bubbleRadius0));
      showFileParams.push_back(ShowFileParameter("numberOfBubbles",numberOfBubbles));
      showFileParams.push_back(ShowFileParameter("bubbleName",bubbleName));

    }
    else if( answer=="uniform state" )
    {
      option=uniformState;
    }
    else if( answer=="rotated shock" )
    {
      option=rotatedShock;
    }
    else if( answer=="gravitationally stratified" )
    {
      
      option=gravitationallyStratified;

      printF(" The gravitationally stratified density is rho(y) = rho0*exp( gravity[1]/(Rg*T0) ( y - y0 ))\n");
      
      gi.inputString(answer2,"Enter rho0 and y0");
      real rho0=1., y0=0.;
      sScanF(answer2,"%e %e",&rho0,&y0);
      bubbleValues.redim(2);  // just save the values in this array
      bubbleValues(0)=rho0;
      bubbleValues(1)=y0;

      gi.inputString(answer2,"Enter uniform background values as `r=2.,p=1., ...' ");
      parameters.inputParameterValues(answer2,"background values",uniform );
    }
    else if( answer == "solid body rotation" )
    {
      option = solidBody;
    }
    else if( answer=="IG" )
    {
      option=ig;
    }
    else if( answer=="LX-17 puck" )
    {
      option=LX17Puck;
    }
    else if( answer=="conical shock" )
    {
      option=conicalShock;
    }
    else if( answer=="converging shock" )
    {
      option=convergingShock;
    }
    else if( answer=="couette profile" )
    {
      // Couette-Poiseuille flow with a divergence free perturbation
      //  u = u0*(y-ya)(yb-y)/[.5*(yb-ya)]^2 + u1*(y-ya)/(yb-ya) 
      //    + u2*      sin(ax*pi*x/(yb-ya))*cos(ay*pi*(y-ya)/(yb-ya))
      //  v =-u2*ax/ay*cos(ax*pi*x/(yb-ya))*sin(ay*pi*(y-ya)/(yb-ya))


      option=couetteProfile;

      if( !db.has_key("couetteData") )
	db.put<RealArray>("couetteData");

      RealArray & couetteData = db.get<RealArray>("couetteData");
      printF(" Couette profile with perturbations\n"
             "     u = u0*(y-ya)(yb-y)/[.5*(yb-ya)]^2 + u1*(y-ya)/(yb-ya) \n"
             "       + u2*      sin(ax*pi*x/(yb-ya))*cos(ay*pi*(y-ya)/(yb-ya))\n"
             "     v =-u2*ax/ay*cos(ax*pi*x/(yb-ya))*sin(ay*pi*(y-ya)/(yb-ya))\n");

      couetteData.redim(7);
      couetteData=0.;
      
      gi.inputString(answer,sPrintF("Enter u0,u1,u2, ax,ay, ya,yb"));

      sScanF(answer,"%e %e %e %e %e %e %e %e",&couetteData(0),&couetteData(1),&couetteData(2),&couetteData(3),
	     &couetteData(4),&couetteData(5),&couetteData(6)); 

    }
    else if( answer=="LANL Cook-Off" )
    {
      option=LANLCookOff;
      gi.inputString(answer,"Enter temperature gradient :");
      sScanF(answer,"%e",&tGradient);  
    }
    else if( answer=="LANL Cook-Off 2" )
    {
      option=LANLCookOff2;
//      gi.inputString(answer,"Enter temperature gradient :");
//      sScanF(answer,"%e",&tGradient);  
    }
    else if( answer=="temperature gradient" ||
             answer=="temperature gradient centre" ||
             answer=="temperature gradient sigma" )
    {
      // define hot spot center and magnitude of the gradient
      if( hotSpotData.getLength(0)<4 )
      {
	hotSpotData.redim(4);
	hotSpotData(0)=.2;  hotSpotData(1)=0.;  hotSpotData(2)=0.; hotSpotData(3)=.075;
      }
      if( answer=="temperature gradient" )
        option=temperatureGradient;
      else if( answer=="temperature gradient centre" )
      {
	gi.inputString(answer,"Enter the centre for the hot spot, x,y,z");
	sScanF(answer,"%e %e %e ",&hotSpotData(0),&hotSpotData(1),&hotSpotData(2));  

	printF("Setting the hot spot centre to be (%e,%e,%e)\n",hotSpotData(0),hotSpotData(1),hotSpotData(2));  
      }
      else if( answer=="temperature gradient sigma" )
      {
	gi.inputString(answer,"Enter sigma for the temperature gradient, T=1-sigma*||x-x0||");
	sScanF(answer,"%e ",&hotSpotData(3));  

	printF("Setting  sigma for the temperature gradient, to be sigma=%e\n",hotSpotData(3));
      }
    }
    else if( answer=="1d profile from a data file" )
    {
      option=profileFromADataFile;
      gi.inputString(profileFileName,"Enter the name of the file with the data");
    }
    else if( answer=="1d profile from a data file (smoothed)" )
    {
      option=profileFromADataFile;
      gi.inputString(profileFileName,"Enter the name of the file with the data");
      gi.inputString(answer,"Enter the number of smooths");
      sScanF(answer,"%i",&numberOfSmooths); 
    }
    else if( answer=="1d profile from a data file perturbed" )
    {
      option=profileFromADataFileWithPerturbation;
      gi.inputString(profileFileName,"Enter the name of the file with the data");

      perturbData.redim(4);

      perturbData(0)=.1;
      perturbData(1)=5.;
      perturbData(2)=.2;
      perturbData(3)=10;
      
      printF(" The perturbation to the density is a0*sin(2*Pi*f0*y)*exp(-beta*(x-x0)^2) \n");
      
      gi.inputString(answer,sPrintF("Enter a0,f0,x0,beta (amplitude,freq, position, "
                    "decay-strength of the perturbation"));
      sScanF(answer,"%e %e %e %e",&perturbData(0),&perturbData(1),&perturbData(2),&perturbData(3));  

      printF("Setting amplitude=%9.2e, frequency=%9.3e, position=%9.3e, beta=%9.2e\n",
	     perturbData(0),perturbData(1),perturbData(2),perturbData(3));
    }
    else if( answer=="1d profile from a data file with changes" )
    {
      option=profileFromADataFile;
      gi.inputString(profileFileName,"Enter the name of the file with the data");
      gi.inputString(answer,"Enter xShift, uShift (shifts to the position and velocity");
      sScanF(answer,"%e %e ",&rpar[0],&rpar[1]);        

    }
    else if( answer=="planar interface with shock" )
    {
      option=planarInterfaceWithShock;
    }
    else if( answer=="circular interface with shock" )
    {
      option=circularInterfaceWithShock;
    }
    else if( answer=="circular (smooth) interface with shock" )
    {
      option=circularSmoothInterfaceWithShock;
    }
    else if( answer=="long elliptical (smooth) interface with shock" )
    {
      option=longEllipticalSmoothInterfaceWithShock;
    }
    else if( answer=="tall elliptical (smooth) interface with shock" )
    {
      option=tallEllipticalSmoothInterfaceWithShock;
    }
    else if( answer=="circular (smooth) interface with shock 2" )
    {
      option=circularSmoothInterfaceWithShock2;
    }
    else if( answer=="bubble with shock" )
    {
      option=bubbleShock;
      numberOfBubbles=1;
      
      bubbleCentre.redim(numberOfBubbles,3); bubbleCentre=0.;
      bubbleRadius.redim(numberOfBubbles); bubbleRadius=1.;
      bubbleValues.redim(numberOfBubbles,numberOfComponents);bubbleValues=1.;

      gi.inputString(answer2,"Enter uniform background values as `r=2.,p=1., ...' ");
      parameters.inputParameterValues(answer2,"background values",uniform );

      int n;
      gi.inputString(answer2,sPrintF(buff,"Enter radius and centre of bubble"));
      if( numberOfDimensions==2 )
        sScanF(answer2,"%e %e %e",&bubbleRadius(0),&bubbleCentre(0,0),&bubbleCentre(0,1));
      else
        sScanF(answer2,"%e %e %e %e",&bubbleRadius(0),&bubbleCentre(0,0),&bubbleCentre(0,1),&bubbleCentre(0,2));
      gi.inputString(answer2,sPrintF(buff,"Enter values for bubble as `r=2. p=1., ...'"));
      RealArray values(numberOfComponents); values=uniform;
      parameters.inputParameterValues(answer2,"bubble values",values );
      for( n=0; n<numberOfComponents; n++ )
      {
	if( values(n)!=(real)Parameters::defaultValue )
	  bubbleValues(0,n)=values(n);
	else
	  bubbleValues(0,n)=0.;
      }
      gi.inputString(answer2,sPrintF(buff,"Enter x location shock"));
      sScanF(answer2,"%e",&shockLoc); 
      shock.redim(numberOfComponents);
      gi.inputString(answer2,sPrintF(buff,"Enter values for shock as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",shock );
    }
    else if( answer == "rate stick (no reaction)" )
    {
      option=rateStick;

      gi.inputString(answer2,"Enter uniform background values as `r=2.,p=1., ...' ");
      parameters.inputParameterValues(answer2,"background values",uniform );

      gi.inputString(answer2,sPrintF(buff,"Enter x location shock"));
      sScanF(answer2,"%e",&shockLoc); 
      shock.redim(numberOfComponents);
      gi.inputString(answer2,sPrintF(buff,"Enter values for shock as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",shock );

      gi.inputString(answer2,sPrintF(buff,"Enter location for center of wall and thickness"));
      sScanF(answer2,"%e %e", &wallCenter, &wallThick);
      
      wallValues.redim(numberOfComponents);
      gi.inputString(answer2,sPrintF(buff,"Enter values for wall as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"wall values",wallValues );
    }
    else if( answer == "compliant rate stick" )
    {
      option = aslamWeak;
      gi.inputString(answer2,sPrintF(buff,"Enter density for inert material"));
      sScanF(answer2,"%e",&rhoInert); 
    }
    else if( answer == "rate stick (reaction)" )
    {
      option = rateStick2;
      gi.inputString(answer2,sPrintF(buff,"Enter density for inert material"));
      sScanF(answer2,"%e",&rhoInert); 
    }
    else if( answer == "PDE thruster" )
    {
      option = pdethruster;
    }
    else if( answer == "PDE thruster 3d" )
    {
      option = pdethruster3d;
    }
    else if( answer == "chevron" )
    {
      option = chev;
    }
    else if( answer == "Tariq Aslam shock polar" )
    {
      option = aslam;
    }
    else if( answer == "Ioan cylinder" )
    {
      option = ioan;
    }
    else if( answer == "Jeffs Corner" )
    {
      if( uSFPointer==NULL )
	uSFPointer =  new realCompositeGridFunction;
      if( cgSFPointer==NULL )
	cgSFPointer = new CompositeGrid;
      
      realCompositeGridFunction & uSF = *uSFPointer;
      CompositeGrid & cgSF            = *cgSFPointer;

      int solutionNumber;
      ShowFileReader showFileReader;
      aString showFileName;

      option = jeffCorner;
      gi.inputString(showFileName,"Enter the name of the show file.");
      showFileReader.open(showFileName);
      gi.inputString(answer2,"Enter the solution number (-1 for last).");
      sScanF(answer2,"%i",&solutionNumber);
      if( solutionNumber < 0 || solutionNumber > showFileReader.getNumberOfFrames() )
      {
	solutionNumber=max(1,showFileReader.getNumberOfFrames());
      }
      showFileReader.getASolution(solutionNumber,cgSF,uSF);
      showFileReader.close();
    }
    else if( answer == "Jeff restart" )
    {
      if( uSFPointer==NULL )
	uSFPointer =  new realCompositeGridFunction;
      if( cgSFPointer==NULL )
	cgSFPointer = new CompositeGrid;
      
      realCompositeGridFunction & uSF = *uSFPointer;
      CompositeGrid & cgSF            = *cgSFPointer;

      int solutionNumber;
      ShowFileReader showFileReader;
      aString showFileName;

      option = jeffRestart;
      gi.inputString(showFileName,"Enter the name of the show file.");
      showFileReader.open(showFileName);
      gi.inputString(answer2,"Enter the solution number (-1 for last).");
      sScanF(answer2,"%i",&solutionNumber);
      if( solutionNumber < 0 || solutionNumber > showFileReader.getNumberOfFrames() )
      {
	solutionNumber=max(1,showFileReader.getNumberOfFrames());
      }
      showFileReader.getASolution(solutionNumber,cgSF,uSF);
      showFileReader.close();
    }
    else if( answer == "Aslam weak det" )
    {
      option = aslamWeak;
    }
    else if( answer == "2-D shock polar" )
    {
      option = shockPolar;
    }
    else if( answer == "two jumps" )
    {
      option = twoJump;

      uLeft.redim(numberOfComponents);
      uCenter.redim(numberOfComponents);
      uRight.redim(numberOfComponents);

      gi.inputString(answer2,sPrintF(buff,"Enter discontinuity locations as 'x1, x2'"));
      sScanF(answer2,"%e %e",&xJump1, &xJump2); 

      gi.inputString(answer2,sPrintF(buff,"Enter values for left state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uLeft );

      gi.inputString(answer2,sPrintF(buff,"Enter values for center state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uCenter );

      gi.inputString(answer2,sPrintF(buff,"Enter values for right state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uRight );
    }

    else if( answer == "three jumps" )
    {
      option = threeJump;

      uLeft.redim(numberOfComponents);
      uCenter.redim(numberOfComponents);
      uRight.redim(numberOfComponents);

      gi.inputString(answer2,sPrintF(buff,"Enter discontinuity locations as 'x1, x2'"));
      sScanF(answer2,"%e %e",&xJump1, &xJump2); 

      gi.inputString(answer2,sPrintF(buff,"Enter values for left state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uLeft );

      gi.inputString(answer2,sPrintF(buff,"Enter values for center state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uCenter );

      gi.inputString(answer2,sPrintF(buff,"Enter values for right state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uRight );
    }
    else if( answer == "elliminate" )
    {
      if( uSFPointer==NULL )
	uSFPointer =  new realCompositeGridFunction;
      if( cgSFPointer==NULL )
	cgSFPointer = new CompositeGrid;
      
      realCompositeGridFunction & uSF = *uSFPointer;
      CompositeGrid & cgSF            = *cgSFPointer;

      int solutionNumber;
      ShowFileReader showFileReader;
      aString showFileName;

      option = elliminate;
      gi.inputString(showFileName,"Enter the name of the show file.");
      showFileReader.open(showFileName);
      gi.inputString(answer2,"Enter the solution number (-1 for last).");
      sScanF(answer2,"%i",&solutionNumber);
      if( solutionNumber < 0 || solutionNumber > showFileReader.getNumberOfFrames() )
      {
	solutionNumber=max(1,showFileReader.getNumberOfFrames());
      }
      showFileReader.getASolution(solutionNumber,cgSF,uSF);
      showFileReader.close();

      gi.inputString(answer2,sPrintF(buff,"Enter cut-off location x1"));
      sScanF(answer2,"%e",&xJump1); 
    }
    else if( answer=="bubbles with shock" )
    {
      // define a set of bubbles -- circular regions with constant properties.
      option=bubblesShock;

      gi.inputString(answer2,"Enter the number of bubbles");
      sScanF(answer2,"%i",&numberOfBubbles);  
      printF("numberOfBubbles = %i \n",numberOfBubbles);

      bubbleCentre.redim(numberOfBubbles,2); bubbleCentre=0.;
      bubbleRadius.redim(numberOfBubbles); bubbleRadius=1.;
      bubbleValues.redim(numberOfBubbles,numberOfComponents);bubbleValues=1.;

      gi.inputString(answer2,"Enter uniform background values as `r=2.,p=1., ...' ");
      parameters.inputParameterValues(answer2,"background values",uniform );

      int n,b;
      for( b=0; b<numberOfBubbles; b++ )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter radius and centre of bubble %i",b));
	sScanF(answer2,"%e %e %e",&bubbleRadius(b),&bubbleCentre(b,0),&bubbleCentre(b,1));
	gi.inputString(answer2,sPrintF(buff,"Enter values for bubble %i as `r=2. p=1., ...'",b));

        RealArray values(numberOfComponents); values=uniform;
        parameters.inputParameterValues(answer2,"bubble values",values );
        for( n=0; n<numberOfComponents; n++ )
	{
	  if( values(n)!=(real)Parameters::defaultValue )
	    bubbleValues(b,n)=values(n);
	  else
	    bubbleValues(b,n)=0.;
	}
      }
      gi.inputString(answer2,sPrintF(buff,"Enter x location for shock"));
      sScanF(answer2,"%e",&shockLoc); 
      shock.redim(numberOfComponents);
      gi.inputString(answer2,sPrintF(buff,"Enter values for post shock state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",shock );
    }
    else if( answer=="compliant corner" )
    {
      option = compliantCorner;
    }
    else if( answer=="compliant corner, desensitized" )
    {
      option = compliantCornerDES;
    }
    else if( answer=="make fail" )
    {
      option = makeFail;
    }
    else if( answer == "compliant rate stick" )
    {
      option = aslamWeak;
      gi.inputString(answer2,sPrintF(buff,"Enter density for inert material"));
      sScanF(answer2,"%e",&rhoInert); 
    }
    else if( answer == "rotated jump" )
    {
      option=rotatedJump;

      uLeft.redim(numberOfComponents);
      uRight.redim(numberOfComponents);

      gi.inputString(answer2,sPrintF(buff,"Enter values for left state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uLeft );

      gi.inputString(answer2,sPrintF(buff,"Enter values for right state as `r=2. p=1., ...'"));
      parameters.inputParameterValues(answer2,"shock values",uRight );
    }
    else if( answer == "pencil" )
    {
      option=pencil;
    }
    else if( answer == "pencil restart" )
    {
      option=pencilRestart;
      if( uSFPointer==NULL )
	uSFPointer =  new realCompositeGridFunction;
      if( cgSFPointer==NULL )
	cgSFPointer = new CompositeGrid;
      
      realCompositeGridFunction & uSF = *uSFPointer;
      CompositeGrid & cgSF            = *cgSFPointer;

      int solutionNumber;
      ShowFileReader showFileReader;
      aString showFileName;
      gi.inputString(showFileName,"Enter the name of the show file.");
      showFileReader.open(showFileName);
      gi.inputString(answer2,"Enter the solution number (-1 for last).");
      sScanF(answer2,"%i",&solutionNumber);
      gi.inputString(answer2,"Enter the pencil half angle (in degrees).");
      sScanF(answer2,"%e",&pencilTheta);
      if( solutionNumber < 0 || solutionNumber > showFileReader.getNumberOfFrames() )
      {
	solutionNumber=max(1,showFileReader.getNumberOfFrames());
      }
      showFileReader.getASolution(solutionNumber,cgSF,uSF);
      showFileReader.close();
    }
    else if( answer == "Noh 2D" )
    {
      option=noh2D;
    }
    else if( answer == "Sedov 2D" )
    {
      option= sedov2D;
    }
    else if( answer=="abl profile" )
      {
	option=ablProfile;

	gi.inputString(answer2,"Enter u_ref, z_ref, alpha, d");
	real u_ref=1., z_ref=1., alpha=1., d=0;
	if( answer2!="" )
	  {
	    sScanF(answer2,"%e %e %e %e",&u_ref,&z_ref,&alpha,&d);
	  }

	RealArray &values = db.get<RealArray>("ablValues");
	values.resize(4);
	values(0) = u_ref;
	values(1) = z_ref;
	values(2) = alpha;
	values(3) = d;

      }
    else if( answer == "linear beam exact solution" )
    {
      option= linearBeamExactSolution;
    }
    else 
    {
      cout << "Unknown option =" << answer << endl;
      gi.stopReadingCommandFile();
    }
    
  }
  
  // These next values determine the pressureLevel constant for ASF:
  parameters.dbase.get<RealArray >("initialConditions")=uniform;

  gi.unAppendTheDefaultPrompt();
  return 0;
}



//! This routine is called when DomainSolver is finished with the initial conditions and can 
//!  be used to clean up memory.
void DomainSolver::
userDefinedInitialConditionsCleanup()
{
  if( parameters.dbase.get<int >("myid")==0 ) 
    printF("***userDefinedInitialConditionsCleanup: delete arrays\n");

  if( parameters.dbase.get<DataBase >("modelData").has_key("userDefinedInitialConditionData") )
  {
    DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedInitialConditionData");
    realCompositeGridFunction *&uSFPointer = db.get<realCompositeGridFunction*>("uSFPointer");
    CompositeGrid *&cgSFPointer = db.get<CompositeGrid*>("cgSFPointer");
    delete uSFPointer;           uSFPointer=NULL;
    delete cgSFPointer;          cgSFPointer=NULL;
  }

}

//! This routine is called when DomainSolver is finished and can be used to clean up memory.
void DomainSolver::
userDefinedCleanup()
{
  if( parameters.dbase.get<int >("myid")==0 ) 
    printF("***userDefinedCleanup: delete arrays\n");

  userDefinedForcingCleanup();
  
  userDefinedMaterialPropertiesCleanup();

}
