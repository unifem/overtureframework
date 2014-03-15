#include "DomainSolver.h"
#include "ShowFileReader.h"
#include "DataPointMapping.h"
#include "ShowFileReader.h"
#include "ParallelUtility.h"
#include "BodyForce.h"

namespace // make the following local to this file
{
// Here are the possible options for user defined forcings. Add new options to this enum.
enum UserDefinedForcingOptions
{
  noForcing,
  constantForcing,
  gaussianForcing,
  dragForcing,
  soapfilmForcing,  // for Alessandro
  trigonometricForcing,  // for heat transfer test
  polynomialForcing,     // for heat transfer test
  blastWaveSource
};
}

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

//==============================================================================================
/// \brief User defined forcing. 
/// \deatils Compute a user defined forcing that will be added to the right-hand side of the equations.
///   This function is called to actually evaluate the user defined forcing
///   The function setupUserDefinedForcing is first 
///   called to assign the option and parameters. Rewrite or add new options to 
///   this function and to setupUserDefinedForcing to supply your own forcing option.
///
/// /f (input/output) : add to this forcing function
/// /gf (input) : current solution
/// \param tForce : evaluate the forcing at this time.
/// 
///
/// /Return values: 0=success, non-zero=failure.
///
/// /NOTE: 2011/08/04 - in the *new* version we just set f rather than adding to it.
//
//==============================================================================================
int DomainSolver:: 
userDefinedForcing( realCompositeGridFunction & f, GridFunction & gf, const real & tForce )
{
  // Look for the userDefinedForcing sub-directory in the data-base
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedForcingData") )
  {
    // if the directory is not there then assume that there is no user defined forcing
    return 0;
  }
  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedForcingData");

  UserDefinedForcingOptions & option= db.get<UserDefinedForcingOptions>("option");
  const bool & userDefinedForcingIsTimeDependent = parameters.dbase.get<bool >("userDefinedForcingIsTimeDependent");

  CompositeGrid & cg = gf.cg;
  realCompositeGridFunction & u = gf.u;

  const real t0 = gf.t;     // ** here is the current time ***  

  // There is no forcing to compute if none was specified or if the forcing is not time dependent and t>0
  // For moving or AMR we always evaluate the forcing (for time independet forcing and AMR, we really 
  //    only to to re-eval when the grids change.) *wdh* 2013/08/31
  if( option==noForcing || ( !userDefinedForcingIsTimeDependent && t0>0.
			     && !parameters.isAdaptiveGridProblem()
                             && !parameters.isMovingGridProblem()) ) 
    return 0;

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & rc = parameters.dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");  
  const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  const int & sc = parameters.dbase.get<int >("sc");   //  mass fraction lambda
  const int & pc = parameters.dbase.get<int >("pc");
  
  real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  int iv0[3]={0,0,0}; //
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];  // NOTE: iv[0]==i1, iv[1]==i2, iv[2]==i3
  real xv[3]={0.,0.,0.};

  // --- loop over component grids ---
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {

    MappedGrid & mg = cg[grid];
  
    // -- To save space we do not create the array of grid vertices on rectangular grids --
    const bool isRectangular = mg.isRectangular();
    if( isRectangular )
    {
      mg.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
	iv0[dir]=mg.gridIndexRange(0,dir);
	if( mg.isAllCellCentered() )
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
    }
    else
    { // for curvilinear grids we need the array of grid vertices
      mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
    }
  
    // This macro defines the grid points for rectangular grids:
    #undef XC
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

    // Extract local serial arrays:
    OV_GET_SERIAL_ARRAY_CONDITIONAL(real,mg.vertex(),vertexLocal,!isRectangular);
    OV_GET_SERIAL_ARRAY_CONST(real,u[grid],uLocal);
    OV_GET_SERIAL_ARRAY(real,f[grid],fLocal);

    Index I1,I2,I3;
    getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
    // getIndex( mg.gridIndexRange(),I1,I2,I3 );  // boundary plus interior points.
    // restrict bounds to local processor, include ghost
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);   
    if( !ok ) return 0;  // no points on this processor

    if( option==constantForcing )
    {
      const RealArray & constantForcingParameters = db.get<RealArray>("constantForcingParameters");
      for( int n=0; n<numberOfComponents; n++ )
      {
	if( constantForcingParameters(n)!=0. )
	{
	  fLocal(I1,I2,I3,n) = constantForcingParameters(n);
	}
      }
    }
    else if( option==gaussianForcing )
    {
      int & numberOfSources = db.get<int>("numberOfSources");
      RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");

      if( false )
	printF("userDefinedForcing: assign gaussianForcing at t=%9.3e\n",t0);
      
      // Add a Gaussian source term
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	if( isRectangular )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=XC(iv,axis);
	}
	else
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=vertexLocal(i1,i2,i3,axis);
	}
	for( int n=0; n<numberOfComponents; n++ )
	  fLocal(i1,i2,i3,n)=0.;

	for( int m=0; m<numberOfSources; m++ )
	{
	  for( int n=0; n<numberOfComponents; n++ )
	  {
	    real an,bn,pn,xn,yn,zn;

	    an=gaussianParameters(0,n,m);
	    bn=gaussianParameters(1,n,m);
	    pn=gaussianParameters(2,n,m);
	    xn=gaussianParameters(3,n,m);
	    yn=gaussianParameters(4,n,m);
	    zn=gaussianParameters(5,n,m);
	    if( an!=0. )
	    {
	      real rad2;
	      if( numberOfDimensions==2 )
		rad2 = ( SQR(xv[0]-xn)+
			 SQR(xv[1]-yn) );
	      else
		rad2 = ( SQR(xv[0]-xn)+
			 SQR(xv[1]-yn)+
			 SQR(xv[2]-zn) );
	      fLocal(i1,i2,i3,n) += an*exp( -bn*pow(rad2,pn*.5) );
	    }
	  }
	}
      }
    }
    else if( option==dragForcing )
    {
      const real & dt = parameters.dbase.get<real >("dt");  // here is the current dt

      if( grid==0 && debug() & 1 )
        printF("userDefinedForcing:INFO: compute a drag forcing at time tForce=%9.3e (dt=%8.2e)\n",tForce,dt);

      // add a drag forcing:
      //    Du/Dt + ... = -beta*u
      assert( uc>=0 );
      assert( vc>=0 );
    
      // For now add a drag forcing over a circle of radius .2 centered at (.5,.5)
      real damp=50., beta=30.;
      real x0=.5, y0=.5, z0=.5, rad0=SQR(.2);
 
      // try this
      assert( dt>0. );

      damp=.5/dt;
      // damp=min(1.,5.*tForce)/dt;  // slow start


      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	if( isRectangular )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=XC(iv,axis);
	}
	else
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=vertexLocal(i1,i2,i3,axis);
	}
      
	real rad;
	if( numberOfDimensions==2 )
	{
	  real xa = xv[0]-x0;
	  real ya = xv[1]-y0;
	  rad = xa*xa+ya*ya;
	}
	else
	{
	  real xa = xv[0]-x0;
	  real ya = xv[1]-y0;
	  real za = xv[2]-z0;
	  rad = xa*xa+ya*ya+za*za;
	}
      

//       // amp = 1 inside the circle and 0 outside
//       // -- here is a smooth transition from 0 to damp at "radius" rad0
//       real amp = .5*damp*(tanh( -beta*(rad-rad0) )+1.);
//       fg(i1,i2,i3,uc) =  -amp*ug(i1,i2,i3,uc);
//       fg(i1,i2,i3,vc) =  -amp*ug(i1,i2,i3,vc);
       
	// here we turn on the drag as a step function at rad=rad0
	if( rad < rad0 )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	  {
	    fLocal(i1,i2,i3,uc+axis) =  -damp*uLocal(i1,i2,i3,uc+axis);  // add damping terms to velocity equations
	  }
	}
	  
      }    
    }
    else if( option==soapfilmForcing)
    {

    
      RealArray & soapfilmParameters = db.get<RealArray>("soapfilmParameters");  //address to the the gravity and airviscosity array (gx,gy,gz,av)

      for( int n=0; n<numberOfDimensions; n++ ) //should work both in 2d and 3d? Is numberOfComponents the same?
      {
	real gn;

	gn=soapfilmParameters(n); //choose gravity components one by one (2 or 3)
	
	  
	if(n==0)
	  fLocal(I1,I2,I3,uc)=gn-soapfilmParameters(3)*uLocal(I1,I2,I3,uc);

	else if(n==1)
	  fLocal(I1,I2,I3,vc)=gn-soapfilmParameters(3)*uLocal(I1,I2,I3,vc);

	else if(n==2)
	  fLocal(I1,I2,I3,wc)=gn-soapfilmParameters(3)*uLocal(I1,I2,I3,wc);
	
         //cout << endl << endl << "ehi, I'm in userdefinedfunctions. gravity along " << n << " is " << gn << "and air viscosity friction is " << soapfilmParameters(3) << endl << endl;

      }

    }
    else if( option==trigonometricForcing )
    {
      // -- Add a trigonmetric source term --

      RealArray & trigonometricParameters = db.get<RealArray>("trigonometricParameters");

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	if( isRectangular )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=XC(iv,axis);
	}
	else
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=vertexLocal(i1,i2,i3,axis);
	}
	for( int n=0; n<numberOfComponents; n++ )
	{
          // this is not so efficient but forcing is not time dependent
	  real amp = trigonometricParameters(0,n);
	  if( amp!=0. )
	  {
            real mx = trigonometricParameters(1,n);
            real my = trigonometricParameters(2,n);
            real x0 = trigonometricParameters(4,n);
            real y0 = trigonometricParameters(5,n);

	    fLocal(i1,i2,i3,n) = sin(mx*Pi*(xv[0]-x0)) * sin(my*Pi*(xv[1]-y0));

	    if( numberOfDimensions==3 )
	    {
	      real mz = trigonometricParameters(3,n);
              real z0 = trigonometricParameters(6,n);

	      fLocal(i1,i2,i3,n) *= sin(mz*Pi*(xv[2]-z0));
	    }
	    fLocal(i1,i2,i3,n) *= amp;
	  }
	  else
	  {
	    fLocal(i1,i2,i3,n)=0.;
	  }
	}
      }
    }
    else if( option==polynomialForcing )
    {
      // -- Add a polynomial source term --
      //  The source term is added to make the exact solution:
      //      T = x(1-x) y(1-y) z(1-z) 
      //   f = 2.*amp*(  y(1-y) z(1-z) +  x(1-x) z(1-z) + x(1-x) y(1-y) ]
      

      RealArray & polynomialParameters = db.get<RealArray>("polynomialParameters");

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	if( isRectangular )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=XC(iv,axis);
	}
	else
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=vertexLocal(i1,i2,i3,axis);
	}
	for( int n=0; n<numberOfComponents; n++ )
	{
          // this is not so efficient but forcing is not time dependent
	  real amp = polynomialParameters(0,n);
	  // if( amp!=0. )
	  //   printF("*** Setting polynomialForcing: amp=%e for n=%i\n",amp,n);
	  
	  if( amp!=0. )
	  {
	    if( numberOfDimensions==2 )
	    {
	      fLocal(i1,i2,i3,n) = amp*( xv[1]*(1.-xv[1]) + xv[0]*(1.-xv[0]) );
	    }
	    else 
	    {
	      fLocal(i1,i2,i3,n) = amp*( xv[1]*(1.-xv[1])*xv[2]*(1.-xv[2]) + 
                                         xv[2]*(1.-xv[2])*xv[0]*(1.-xv[0]) + 
                                         xv[0]*(1.-xv[0])*xv[1]*(1.-xv[1]) );
	    }
	  }
	  else
	  {
	    fLocal(i1,i2,i3,n)=0.;
	  }
	}
      }
    }

    else if( option==blastWaveSource )
    {

      RealArray & blastWaveParameters = db.get<RealArray>("blastWaveParameters");

      const real alpha=blastWaveParameters(0);
      const real beta =blastWaveParameters(1);
      const real En   =blastWaveParameters(2);
      const real xn   =blastWaveParameters(3);
      const real yn   =blastWaveParameters(4);
      const real zn   =blastWaveParameters(5);
      const real tn   =blastWaveParameters(6);
      

      if( t0<3*dt )
	printF("userDefinedForcing: assign blast wave source at t=%9.3e\n",t0);
      
      // Add the blast wave source term
      const real expt = exp(-alpha*(t0-tn)*(t0-tn));
 
      // --- choose the amplitude so that the total energy release is approximately E0 ---
      real rho0=1.;  // background density -- fix me --
      const real amp = En/(rho0*(Pi/beta)*sqrt(Pi/alpha)) * expt;
      
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	if( isRectangular )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=XC(iv,axis);
	}
	else
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=vertexLocal(i1,i2,i3,axis);
	}
	for( int n=0; n<numberOfComponents; n++ )
	  fLocal(i1,i2,i3,n)=0.;

	real rad2;
	if( numberOfDimensions==2 )
	  rad2 = ( SQR(xv[0]-xn)+
		   SQR(xv[1]-yn) );
	else
	  rad2 = ( SQR(xv[0]-xn)+
		   SQR(xv[1]-yn)+
		   SQR(xv[2]-zn) );

	fLocal(i1,i2,i3,tc) = amp*exp( -beta*rad2  );  // source term in total energy

      }
    }

    else
    {
      printF("userDefinedForcing:ERROR: Unknown option =%i\n",option);
    }
 
  }  // end for( grid )
  
  return 0;
}




//\begin{>>DomainSolverInclude.tex}{\subsection{setupUserDefinedForcing}}  
int DomainSolver::
setupUserDefinedForcing()
//==============================================================================================
// /Description:
//    Setup User defined forcing. This function is used to setup and define the forcing to use.
// The function userDefinedForcing (above) is called to actually assign the forcing.
//  Rewrite or add new options to  this routine to supply your own forcing.
// Choose the "user defined forcing" option from the initial conditions options to have this routine
// called.
//
// /Return values: 0=success, non-zero=failure.
//\end{DomainSolverInclude.tex}  
//==============================================================================================
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  bool & userDefinedForcingIsTimeDependent = parameters.dbase.get<bool >("userDefinedForcingIsTimeDependent");
  
  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & rc = parameters.dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");  
  // const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  const int & sc = parameters.dbase.get<int >("sc");   //  mass fraction lambda
  //  const int & pc = parameters.dbase.get<int >("pc");
  
  // here is a menu of possible forcing options
  aString menu[]=  
  {
    "no forcing",
    "constant forcing",
    "gaussian forcing",
    "drag forcing",
    "soapfilm forcing",
    "trigonmetric forcing",
    "polynomial forcing",
    "blast wave source",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user defined");

  // Make a sub-directory in the data-base to store variables used here and in userDefinedForcing
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedForcingData") )
    parameters.dbase.get<DataBase >("modelData").put<DataBase>("userDefinedForcingData");

  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedForcingData");
  // first time through allocate variables 
  if( !db.has_key("option") )
  {
    UserDefinedForcingOptions & option= db.put<UserDefinedForcingOptions>("option",noForcing);

    db.put<int>("numberOfSources");
    db.put<RealArray>("gaussianParameters");
  }

  UserDefinedForcingOptions & option= db.get<UserDefinedForcingOptions>("option");

  int & numberOfSources = db.get<int>("numberOfSources");

 
  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="no forcing" )
    {
      option=noForcing;
    }
    else if( answer=="constant forcing" )
    {
      // define a constant forcing
      option=constantForcing;
      userDefinedForcingIsTimeDependent=false;  // this forcing is not time dependent

      if( !db.has_key("constantForcingParameters") )
	db.put<RealArray>("constantForcingParameters");
      RealArray & constantForcingParameters = db.get<RealArray>("constantForcingParameters");

      constantForcingParameters.redim(numberOfComponents);
      constantForcingParameters=0.;
      
      printF("The constant forcing is of the form\n"
             "   f(x,t; n) = g(n)\n"
             " n = component number, n=0,1,..,%i \n",numberOfComponents-1 );

      // query for changes:
      for( int nn=0; ; nn++ )
      {
        gi.inputString(answer2,sPrintF("Enter n, g(n) (Enter `done' to finish)"));
        if( answer2=="done" ) break;
	
        int n=-1;
        real gn=0.;
        sScanF(answer2,"%i %e",&n,&gn);  
        if( n>=0 && n<numberOfComponents )
	{
          constantForcingParameters(n)=gn;
	  printF(" Setting constant forcing g(%i)=%g\n",n,gn);
	}
	else
	{
          printF("DomainSolver::setupUserDefinedForcing:ERROR: invalid input: n=%i \n",n);
	  gi.stopReadingCommandFile();
	  break;
	}
      }
    }
    else if( answer=="gaussian forcing" )
    {
      // define a Gaussian forcing
      option=gaussianForcing;
      userDefinedForcingIsTimeDependent=false;  // this forcing is not time dependent

      if( !db.has_key("gaussianParameters") )
	db.put<RealArray>("gaussianParameters");
      RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");


      gi.inputString(answer2,sPrintF("Enter the number of Gaussian sources (default = 1 per component)"));
      sScanF(answer2,"%i",&numberOfSources);
      printF(" numberOfSources = %i\n",numberOfSources);
      
      printF("The Gaussian forcing is of the form\n"
             "   f(x,t; n) = Sum_m { a(n,m)*exp( -b(n,m)*| x - x0(n,m) |^p(n,m) ) }\n"
             " m = source number, m=0,1,..,%i \n"
             " n = component number, n=0,1,..,%i \n"
             " a(n,m) = amplitude \n"
             " b(n,m) = exponent \n" 
             " x0(n,m) = centre \n" 
             " p(n,m) = power \n",numberOfSources-1,numberOfComponents-1 );
      real an=0.,bn=10.,pn=2.,xn=0.,yn=0.,zn=0.;
      gaussianParameters.redim(6,numberOfComponents,numberOfSources);
      // assign defaults
      for( int m=0; m<numberOfSources; m++ )
      {
	for( int n=0; n<numberOfComponents; n++ )
	{
	  gaussianParameters(0,n,m)=an;
	  gaussianParameters(1,n,m)=bn;
	  gaussianParameters(2,n,m)=pn;
	  gaussianParameters(3,n,m)=xn;
	  gaussianParameters(4,n,m)=yn;
	  gaussianParameters(5,n,m)=zn;
	}
      }
      for( int nn=0; ; nn++ )
      {
        gi.inputString(answer2,sPrintF("Enter m, n, a, b,p, x,y,z (Enter `done' to finish)"));
        if( answer2=="done" ) break;
	
        int m=-1, n=-1;
        sScanF(answer2,"%i %i %e %e %e %e %e %e ",&m,&n,&an,&bn,&pn,&xn,&yn,&zn);  
        if( m>=0 && m<numberOfSources && n>=0 && n<numberOfComponents )
	{
          gaussianParameters(0,n,m)=an;
          gaussianParameters(1,n,m)=bn;
          gaussianParameters(2,n,m)=pn;
          gaussianParameters(3,n,m)=xn;
          gaussianParameters(4,n,m)=yn;
          gaussianParameters(5,n,m)=zn;
	  printF(" Setting Gaussian parameters for m=%i n=%i: a=%g, b=%g, p=%g, x=%g,y=%g,z=%g\n",
		 m,n,an,bn,pn,xn,yn,zn);
	}
	else
	{
          printF("DomainSolver::setupUserDefinedForcing:ERROR: invalid input: m=%i and n=%i \n",m,n);
	  gi.stopReadingCommandFile();
	  break;
	}
      }
    }
    else if( answer=="drag forcing" )
    {
      // define a drag forcing ... finish me ...
      option=dragForcing;
      userDefinedForcingIsTimeDependent=true;  // this forcing is time dependent

    }
    else if( answer=="soapfilm forcing" )
    {
      option=soapfilmForcing;
      userDefinedForcingIsTimeDependent=true;  // this forcing is time dependent *wdh* 

      if( !db.has_key("soapfilmParameters") )
	db.put<RealArray>("soapfilmParameters");

      RealArray & soapfilmParameters = db.get<RealArray>("soapfilmParameters");

      soapfilmParameters.redim(4);
      soapfilmParameters=0.;
      real gx=0.0, gy=-9.81, gz=0., av=4.905;
     
      //  assign defaults
      soapfilmParameters(0)=gx;
      soapfilmParameters(1)=gy;
      soapfilmParameters(2)=gz;
      soapfilmParameters(3)=av;

      printF("The forcing is of the form\n"
             "   f(x,y,t; n) = g(n) - lambda/rho_2d u(x,y,t; n)\n"
             " n = dimensions number, n=0,1,..,%i \n"
             " u(x,y,t; n) instant local velocity in n direction\n"
             " NOTE: g(n) is positive, so put the correct signs! If gravity is in left-right x-direction it's positive, if it's in up-down y-direction 		       is negative; lambda has to be positive to be a friction! \n",numberOfDimensions-1);

      // query for changes:
      for( int nn=0; ; nn++ )
      {
        gi.inputString(answer2,sPrintF("Enter gravity vector (gx, gy, gz) and the air viscosity coefficient=lambda/rho_2d (av).\n"
                                       "Default values are  gx=%8.2e, gy=%8.2e, gz=%8.2e, av=%8.2e, recall that av=lambda/rho_2d=g/v \n"
                                       " (Enter `done' to finish)\n",gx, gy, gz, av));
        if( answer2=="done" ) break;
	

        sScanF(answer2,"%e %e %e %e",&gx, &gy, &gz, &av);  

//reassign for changes

     	soapfilmParameters(0)=gx;
	soapfilmParameters(1)=gy;
	soapfilmParameters(2)=gz;
	soapfilmParameters(3)=av;

	printF(" Setting Soapfilm parameters for gx=%g, gy=%g, gz=%g, av=%g\n",
	       gx, gy, gz, av);
	
	
      }
    }

    else if( answer=="trigonmetric forcing" )
    {
      // define a trigonometric forcing
      option=trigonometricForcing;
      userDefinedForcingIsTimeDependent=false;  // this forcing is not time dependent

      if( !db.has_key("trigonometricParameters") )
	db.put<RealArray>("trigonometricParameters");
      RealArray & trigonometricParameters = db.get<RealArray>("trigonometricParameters");

      trigonometricParameters.redim(7,numberOfComponents);
      trigonometricParameters=0.;
      
      printF("The trigonmetric forcing is of the form\n"
             "   f(x,t; n) = amp(n) * sin(mx(n)*pi*(x-x0(n))) * sin(my(n)*pi*(y-y0(n))) [* sin(mz(n)*pi*(z-z0(n)))]\n"
             " n = component number, n=0,1,..,%i, \n"
             " amp(n) = amplitude,\n"
             " mx(n), my(n), mz(n) = frequencies,\n"
             " x0(n), y0(n), z0(n) = phase shifts,\n",
             numberOfComponents-1 );

      for( int nn=0; ; nn++ )
      {
        gi.inputString(answer2,sPrintF("Enter n, amp, mx, my, mz, x0, y0, z0 (Enter `done' to finish)"));
        if( answer2=="done" ) break;
	
	int n=-1;
	real amp=1., mx=1.,my=1.,mz=1.,x0=0.,y0=0.,z0=0.;
	 
        sScanF(answer2,"%i %e %e %e %e %e %e %e ",&n,&amp,&mx,&my,&mz,&x0,&y0,&z0);  
        if( n>=0 && n<numberOfComponents )
	{
          trigonometricParameters(0,n)=amp;
          trigonometricParameters(1,n)=mx; 
          trigonometricParameters(2,n)=my; 
          trigonometricParameters(3,n)=mz; 
          trigonometricParameters(4,n)=x0; 
          trigonometricParameters(5,n)=y0; 
          trigonometricParameters(6,n)=z0; 
	  printF(" Setting trigonometric parameters for n=%i: amp=%g, mx=%g, my=%g, mz=%g, x0=%g, y0=%g, z0=%g\n",
		 n,amp,mx,my,mz,x0,y0,z0);
	}
	else
	{
          printF("DomainSolver::setupUserDefinedForcing:ERROR: invalid input:n=%i \n",n);
	  gi.stopReadingCommandFile();
	  break;
	}
      }
    }
    else if( answer=="polynomial forcing" )
    {
      // define a polynomial forcing
      option=polynomialForcing;
      userDefinedForcingIsTimeDependent=false;  // this forcing is not time dependent

      if( !db.has_key("polynomialParameters") )
	db.put<RealArray>("polynomialParameters");
      RealArray & polynomialParameters = db.get<RealArray>("polynomialParameters");

      polynomialParameters.redim(1,numberOfComponents);
      polynomialParameters=0.;
      
      printF("The polynomial forcing is of the form\n"
             "   f(x,t; n) = amp(n)*(  y(1-y) z(1-z) +  x(1-x) z(1-z) + x(1-x) y(1-y) ]\n"
             " n = component number, n=0,1,..,%i, \n"
             " amp(n) = amplitude,\n",
             numberOfComponents-1 );

      for( int nn=0; ; nn++ )
      {
        gi.inputString(answer2,sPrintF("Enter n, amp (Enter `done' to finish)"));
        if( answer2=="done" ) break;
	
	int n=-1;
	real amp=1.;
	 
        sScanF(answer2,"%i %e ",&n,&amp);  
        if( n>=0 && n<numberOfComponents )
	{
          polynomialParameters(0,n)=amp;
	  printF(" Setting polynomial parameters for n=%i: amp=%g\n",n,amp);
	}
	else
	{
          printF("DomainSolver::setupUserDefinedForcing:ERROR: invalid input:n=%i \n",n);
	  gi.stopReadingCommandFile();
	  break;
	}
      }
    }

    else if( answer=="blast wave source" )
    {
      // define a source term for a Sedov-Taylor blast wave

      option=blastWaveSource;
      userDefinedForcingIsTimeDependent=true;  // this forcing IS time dependent

      if( !db.has_key("blastWaveParameters") )
	db.put<RealArray>("blastWaveParameters");
      RealArray & blastWaveParameters = db.get<RealArray>("blastWaveParameters");


      real alpha=10., beta=10., En=10., xn=.5, yn=.5, zn=0., tn=.25;
      
      gi.inputString(answer2,"Enter alpha,beta,En, xn,yn,zn,tn");
      sScanF(answer2,"%e %e %e %e %e %e  %e",&alpha,&beta,&En,&xn,&yn,&zn,&tn);   
      printF("Setting: alpha=%g, beta=%g, En=%g, (xn,yn,zn)=(%g,%g,%g), tn=%g\n",
               alpha,beta,En,xn,yn,zn,tn);

      blastWaveParameters.redim(7);
      blastWaveParameters(0)=alpha;
      blastWaveParameters(1)=beta;
      blastWaveParameters(2)=En;
      blastWaveParameters(3)=xn;
      blastWaveParameters(4)=yn;
      blastWaveParameters(5)=zn;
      blastWaveParameters(6)=tn;
      
    }

    else 
    {
      printF("userDefinedForcing:Unknown option =%s\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
  

  // Indicate whether user defined forcing has been turned on:
  if( option!=noForcing )
    parameters.dbase.get<bool >("turnOnUserDefinedForcing")=true;
  else
    parameters.dbase.get<bool >("turnOnUserDefinedForcing")=false;
  
  // 120203 added by kkc because it body forcing was no longer turned on if user defined forcings were activated but no other body forcings were
  bool & turnOnBodyForcing = parameters.dbase.get<bool >("turnOnBodyForcing"); 
  turnOnBodyForcing = turnOnBodyForcing || parameters.dbase.get<bool >("turnOnUserDefinedForcing");
  if( turnOnBodyForcing && !parameters.dbase.has_key("bodyForcings") )
  {
    parameters.dbase.put<std::vector<BodyForce*> >("bodyForcings");
  }

  gi.unAppendTheDefaultPrompt();
  return 0;
}



//! This routine is called when DomainSolver is finished and can be used to clean up memory.
void DomainSolver::
userDefinedForcingCleanup()
{
  if( parameters.dbase.get<int >("myid")==0 ) 
    printf("***userDefinedForcingCleanup: delete arrays\n");

  if( parameters.dbase.get<DataBase >("modelData").has_key("userDefinedForcingData") )
  {
    DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedForcingData");
  }
  
}

