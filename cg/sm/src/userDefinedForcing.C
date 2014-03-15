#include "Cgsm.h"
#include "ShowFileReader.h"
#include "DataPointMapping.h"
#include "ShowFileReader.h"
#include "ParallelUtility.h"

namespace // make the following local to this file
{
// Here are the possible options for user defined forcings. Add new options to this enum.
enum UserDefinedForcingOptions
{
  noForcing,
  constantForcing,
  gaussianForcing,
  translationAndRotationForcing
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


//\begin{>>CgsmInclude.tex}{\subsection{userDefinedForcing}}  
int Cgsm:: 
userDefinedForcing( realArray & f, const realMappedGridFunction & u, int iparam[], real rparam[] )
//==============================================================================================
// /Description:
//   User defined forcing. This function is called to actually evaluate the user defined forcing
//   The function setupUserDefinedForcing is first 
//   called to assign the option and parameters. Rewrite or add new options to 
//   this function and to setupUserDefinedForcing to supply your own forcing option.
//
// /u (input) : current solution
// /f (input/output) : add to this forcing function
// /iparam[] (input) : holds some integer parameters
// /rparam[] (input) : holds some real parameters
// 
// /Notes:
//  \begin{itemize}
//    \item You must fill in the realMappedGridFunction f.
//    \item The `parameters' object holds many useful parameters.
//    \item The current time "t" is extracted below from rparam.  
//  \end{itemize}
//
// /Return values: 0=success, non-zero=failure.
//\end{CgsmInclude.tex}  
//==============================================================================================
{
  // Look for the userDefinedForcing sub-directory in the data-base
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedForcingData") )
  {
    // if the directory is not there then assume that there is no user defined forcing
    return 0;
  }
  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedForcingData");

  UserDefinedForcingOptions & option= db.get<UserDefinedForcingOptions>("option");

  if( option==noForcing ) return 0;

  const real t =rparam[1];       // ** here is the current time ***  
  const int & grid = iparam[0];

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & rc = parameters.dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");  
  const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  const int & sc = parameters.dbase.get<int >("sc");   //  mass fraction lambda
  const int & pc = parameters.dbase.get<int >("pc");

  const real & rho=parameters.dbase.get<real>("rho");
  const real & mu = parameters.dbase.get<real>("mu");
  const real & lambda = parameters.dbase.get<real>("lambda");
  const RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
  const RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
  
  MappedGrid & mg = *u.getMappedGrid();
  
  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
  #ifndef USE_PPP
    realArray & vertex = mg.vertex();  // grid points
    const realArray & ug = u;
    realArray & fg = f;
  #else
    // In parallel, we operate on the arrays local to each processor
    realSerialArray vertex; getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
    realSerialArray ug;     getLocalArrayWithGhostBoundaries(u,ug);
    realSerialArray fg;     getLocalArrayWithGhostBoundaries(f,fg);
  #endif

  Index I1,I2,I3;
  getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
  // getIndex( mg.gridIndexRange(),I1,I2,I3 );  // boundary plus interior points.
  #ifdef USE_PPP
    // restrict bounds to local processor, include ghost
    bool ok = ParallelUtility::getLocalArrayBounds(u,ug,I1,I2,I3,1);   
    if( !ok ) return 0;  // no points on this processor
  #endif

  if( option==constantForcing )
  {
    const RealArray & constantForcingParameters = db.get<RealArray>("constantForcingParameters");
    for( int n=0; n<numberOfComponents; n++ )
    {
      if( constantForcingParameters(n)!=0. )
      {
	fg(I1,I2,I3,n)+=constantForcingParameters(n);
      }
    }
  }
  else if( option==gaussianForcing )
  {
    int & numberOfSources = db.get<int>("numberOfSources");
    RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");

    // Add a Gaussian source term
    int n;
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
	  RealArray rad2;
	  if( numberOfDimensions==2 )
	    rad2 = ( SQR(vertex(I1,I2,I3,axis1)-xn)+
		     SQR(vertex(I1,I2,I3,axis2)-yn) );
	  else
	    rad2 = ( SQR(vertex(I1,I2,I3,axis1)-xn)+
		     SQR(vertex(I1,I2,I3,axis2)-yn)+
		     SQR(vertex(I1,I2,I3,axis3)-zn) );
	  fg(I1,I2,I3,n)+=an*exp( -bn*pow(rad2,pn*.5) );
	}
      }
    }
  }
  else if( option==translationAndRotationForcing )
  {
    // assign the forcing that makes a translation and rotation an exact solution

    // Rotation solution is 
    //       u = (R(t)-I)*( X - x0 )
    //   u = displacement
    //   X = reference position
    //   x0 = origin of the rotation
    //   R(t) = rotation matrix
    // 
    //  Forcing:
    //    f = rho*u_tt = R'' (X-x0)

    std::vector<real> & trd = parameters.dbase.get<std::vector<real> >("translationAndRotationForcingData");
    real omega = trd[0];
    real x0=trd[1];
    real x1=trd[2];
    real x2=trd[3];
    real v0=trd[4];
    real v1=trd[5];
    real v2=trd[6];

    real omeg2=omega*omega/rho;
    
    fg=0.;
    
    int i1,i2,i3;
    real x,y,cost,sint;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      x = vertex(i1,i2,i3,0);
      y = vertex(i1,i2,i3,1);
      cost =cos(omega*t);
      sint= sin(omega*t);

      fg(i1,i2,i3,uc)=( -cost*(x-x0) - sint*(y-x1) )*omeg2;
      fg(i1,i2,i3,vc)=(  sint*(x-x0) - cost*(y-x1) )*omeg2;
    }
    
  }
  else
  {
    cout << "userDefinedForcing: Unknown option =" << option << endl;
  }

  return 0;
}




//\begin{>>CgsmInclude.tex}{\subsection{setupUserDefinedForcing}}  
int Cgsm::
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
//\end{CgsmInclude.tex}  
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
  //  const int & pc = parameters.dbase.get<int >("pc");
  
  // here is a menu of possible forcing options
  aString menu[]=  
  {
    "no forcing",
    "constant forcing",
    "gaussian forcing",
    "translation and rotation forcing",
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
    UserDefinedForcingOptions & option= db.put<UserDefinedForcingOptions>("option",::noForcing);

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
      option=::noForcing;
    }
    else if( answer=="constant forcing" )
    {
      // define a constant forcing
      option=constantForcing;

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
          printF("Cgsm::setupUserDefinedForcing:ERROR: invalid input: n=%i \n",n);
	  gi.stopReadingCommandFile();
	  break;
	}
      }
    }
    else if( answer=="gaussian forcing" )
    {
      // define a Gaussian forcing
      option=gaussianForcing;

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
          printF("Cgsm::setupUserDefinedForcing:ERROR: invalid input: m=%i and n=%i \n",m,n);
	  gi.stopReadingCommandFile();
	  break;
	}
      }
    }
    else if( answer=="translation and rotation forcing" )
    {
      // define a forcing that goes with the translation-rotation special solution
      option=translationAndRotationForcing;


      // Rotation solution is 
      //       u = (R(t)-I)*( X - x0 )
      //   u = displacement
      //   X = reference position
      //   x0 = origin of the rotation
      //   R(t) = rotation matrix

      printF("INFO: translation and rotation forcing parameters: \n"
	     "  omega = rotation rate (omega=1 corresponds to 1 rotation per unit time)\n"
	     "  (x0,x1,x2) = center of rotation\n"
	     "  (v0,v1,v2) = velocity\n");
      if( !parameters.dbase.has_key("translationAndRotationForcingData") ) 
	parameters.dbase.put<std::vector<real> >("translationAndRotationForcingData");

      std::vector<real> & trd = parameters.dbase.get<std::vector<real> >("translationAndRotationForcingData");
      trd.resize(10);
      real omega=1., x0=0.,x1=0.,x2=0., v0=0.,v1=0.,v2=0.;
      aString answer2;
      gi.inputString(answer2,"Enter omega,x0,x1,x2,v0,v1,v2");
      sScanF(answer2,"%e %e %e %e %e %e %e",&omega,&x0,&x1,&x2,&v0,&v1,&v2);

      printF("Translation and rotation forcing: using omega=%8.2e, (x0,x1,x2)=(%8.2e,%8.2e,%8.2e), "
	     " (v0,v1,v2)=(%8.2e,%8.2e,%8.2e)\n",omega,x0,x1,x2,v0,v1,v2);

      trd[0]=omega*twoPi;  // *note*
      trd[1]=x0;
      trd[2]=x1;
      trd[3]=x2;
      trd[4]=v0;
      trd[5]=v1;
      trd[6]=v2;

    }
    else 
    {
      cout << "Unknown option =" << answer << endl;
      gi.stopReadingCommandFile();
    }
    
  }
  
  gi.unAppendTheDefaultPrompt();
  return 0;
}



//! This routine is called when Cgsm is finished and can be used to clean up memory.
void Cgsm::
userDefinedForcingCleanup()
{
  if( parameters.dbase.get<int >("myid")==0 ) 
    printf("***userDefinedForcingCleanup: delete arrays\n");

  if( parameters.dbase.get<DataBase >("modelData").has_key("userDefinedForcingData") )
  {
    DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedForcingData");
  }
  
}

