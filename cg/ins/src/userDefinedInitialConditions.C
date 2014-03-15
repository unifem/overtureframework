#include "Cgins.h"
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
  profileFromADataFile,
  profileFromADataFileWithPerturbation,
  gravitationallyStratified,
  solidBody,
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

int Cgins::
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
//  RealArray & hotSpotData  = db.get<RealArray>("hotSpotData");
  RealArray & perturbData  = db.get<RealArray>("perturbData");

  realCompositeGridFunction *&uSFPointer = db.get<realCompositeGridFunction*>("uSFPointer");
  CompositeGrid *&cgSFPointer = db.get<CompositeGrid*>("cgSFPointer");

  int & numberOfSmooths = db.get<int>("numberOfSmooths");
  int & numberOfBubbles = db.get<int>("numberOfBubbles");
  aString & profileFileName = db.get<aString>("profileFileName");

  real *rpar        = db.get<real[2]>("rpar");

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
      else if ( option==profileFromADataFileWithPerturbation )
      {
	// apply a perturbation to the profile read from a data file 
        real a0=perturbData(0);
        real f0=perturbData(1);
        real x0=perturbData(2);
        real beta=perturbData(3);
	
	ug(I1,I2,I3,rc)+=a0*sin(twoPi*f0*vertex(I1,I2,I3,1))*exp(-beta*SQR(vertex(I1,I2,I3,0)-x0));

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
    "1d profile from a data file",
    "1d profile from a data file (smoothed)",
    "1d profile from a data file perturbed",
    "1d profile from a data file with changes",
    "gravitationally stratified",
    "solid body rotation",
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

    db.put<real[2]>("rpar");
    real *rpar = db.get<real[2]>("rpar");
    rpar[0]=0.; rpar[1]=0.;
  
  }

  UserDefinedInitialConditionOptions & option= db.get<UserDefinedInitialConditionOptions>("option");

  RealArray & uniform      = db.get<RealArray>("uniform");
  RealArray & bubbleCentre = db.get<RealArray>("bubbleCentre");
  RealArray & bubbleRadius = db.get<RealArray>("bubbleRadius");
  RealArray & bubbleValues = db.get<RealArray>("bubbleValues");
  RealArray & perturbData  = db.get<RealArray>("perturbData");
  realCompositeGridFunction *& uSFPointer = db.get<realCompositeGridFunction*>("uSFPointer");
  CompositeGrid *& cgSFPointer = db.get<CompositeGrid*>("cgSFPointer");
  int & numberOfSmooths = db.get<int>("numberOfSmooths");
  int & numberOfBubbles = db.get<int>("numberOfBubbles");
  aString & profileFileName = db.get<aString>("profileFileName");

  real *rpar        = db.get<real[2]>("rpar");
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
void Cgins::
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

