#include "Maxwell.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"


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
/// \brief Evaluate the user defined forcing.
/// \details This function is called to actually evaluate the user defined forcing
///   The function setupUserDefinedForcing is first 
///   called to assign the option and parameters. Rewrite or add new options to 
///   this function and to setupUserDefinedForcing to supply your own forcing option.
///
/// \param f (input/output) : add to this forcing function
/// \param iparam[] (input) : holds some integer parameters
/// \param rparam[] (input) : holds some real parameters
/// 
// NOTES:
//   (1) The forcing function is added to the right-hand side of Maxwell's equations
//        in second order form:
//               E_tt = c^2 ( E_xx + E_yy + E_zz ) + F(x,y,z,t) 
//==============================================================================================

int Maxwell:: 
userDefinedForcing( realArray & f, int iparam[], real rparam[] )
{
  // Look for the userDefinedForcing sub-directory in the data-base
  if( !dbase.has_key("userDefinedForcingData") )
  {
    // if the directory is not there then assume that there is no user defined forcing
    return 0;
  }
  DataBase & db = dbase.get<DataBase>("userDefinedForcingData");

  aString & option= db.get<aString>("option");

  if( option=="none" ) // No user defined forcing has been specified
   return 0;

  if( method!=nfdtd )
  {
    printF("userDefinedForcing:ERROR: method!=nfdtd -- not implemented for other methods yet\n");
    OV_ABORT("ERROR");
  }

  const real t =rparam[0];       // Add the forcing at this time.
  const real dt =rparam[1];      // Current time step.
  const int & grid = iparam[0];  // Here is the grid we are on 
  const int & current = iparam[1];
  
  if( false )
  {
    printF("userDefinedForcing: t=%9.3e option=[%s]\n",t,(const char*)option);
  }

  // Here is the CompositeGrid: 
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  assert( grid>=0 && grid<numberOfComponentGrids );
  MappedGrid & mg = cg[grid];
  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
  const realArray & x = mg.vertex();

  // Here is the current solution: 
  realCompositeGridFunction & ucg = cgfields[current];
  realMappedGridFunction & u = ucg[grid];
  

  // Access the local arrays on this processor:
  OV_GET_SERIAL_ARRAY_CONST(real,x,xLocal);
  OV_GET_SERIAL_ARRAY(real,u,uLocal);
  OV_GET_SERIAL_ARRAY(real,f,fLocal);

  int i1,i2,i3;
  Index I1,I2,I3;
  getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
  // getIndex( mg.gridIndexRange(),I1,I2,I3 );  // boundary plus interior points.
  // restrict bounds to local processor, include parallel ghost pts:
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,1);   
  if( !ok ) return 0;  // no points on this processor (NOTE: no communication should be done after this point)

  // fLocal=0.;

  if( option=="gaussianSources" )
  {
    int & numberOfGaussianSources = db.get<int>("numberOfGaussianSources");
    RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");

    // Add the Gaussian source terms to fLocal
    for( int m=0; m<numberOfGaussianSources; m++ )
    {
      real a    = gaussianParameters(0,m);
      real beta = gaussianParameters(1,m); 
      real omega= gaussianParameters(2,m); 
      real p    = gaussianParameters(3,m);
      real x0   = gaussianParameters(4,m); 
      real y0   = gaussianParameters(5,m); 
      real z0   = gaussianParameters(6,m);
      real t0   = gaussianParameters(7,m);

      if( false )
	printF("Gaussian source %i: setting a=%8.2e, beta=%8.2e, omega=%8.2e, p=%8.2e, x0=%8.2e, y0=%8.2e, "
               "z0=%8.2e, t0=%8.2e\n", m,a,beta,omega,p,x0,y0,z0,t0);

      const real cost=cos(2.*Pi*omega*(t-t0));
      const real sint=sin(2.*Pi*omega*(t-t0));

      if( mg.numberOfDimensions()==2 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1);
	  real g = a*cost*exp( -beta*pow( SQR(x-x0)+SQR(y-y0), p ) );

	  fLocal(i1,i2,i3,ex)+= -(y-y0)*g;
	  fLocal(i1,i2,i3,ey)+=  (x-x0)*g;
	  fLocal(i1,i2,i3,hz)+=         g;

	  fLocal(i1,i2,i3,ey)=0.; // *****************
	  
	}
      }
      else
      {
	// -- 3D ---
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1), z=xLocal(i1,i2,i3,2);
	  real g = a*cost*exp( -beta*pow( SQR(x-x0)+SQR(y-y0)+SQR(z-z0), p ) );

	  fLocal(i1,i2,i3,ex)+= ((z-z0)-(y-y0))*g;
	  fLocal(i1,i2,i3,ey)+= ((x-x0)-(z-z0))*g;
	  fLocal(i1,i2,i3,ez) =0.;  // *****************
	  // fLocal(i1,i2,i3,ez)+= ((y-y0)-(x-x0))*g;
	}
      }
    }
  }
  else
  {
    printF("Maxwell::userDefinedForcing:ERROR: unknown option =[%s]\n",(const char*)option);
    OV_ABORT("error");
  }

  return 0;
}




//==============================================================================================
/// \brief This function is used to choose a user defined forcing and input parameters etc.
/// \details This function is used to setup and define the forcing to use.
/// The function userDefinedForcing (above) is called to actually assign the forcing.
///  Rewrite or add new options to  this routine to supply your own forcing.
/// Choose the "user defined forcing" option to have this routine called.
///
//==============================================================================================
int Maxwell::
setupUserDefinedForcing()
{
  GenericGraphicsInterface & gi = *gip;

  // here is a menu of possible forcing options
  aString menu[]=  
  {
    "no forcing",
    "gaussian sources",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user defined forcing");

  // Make a sub-directory in the data-base to store variables used in userDefinedForcing
  if( !dbase.has_key("userDefinedForcingData") )
    dbase.put<DataBase>("userDefinedForcingData");

  DataBase & db = dbase.get<DataBase>("userDefinedForcingData");

  // option = the name of the user defined forcing.
  if( !db.has_key("option") )
  { // create option variable in the data base.
    db.put<aString>("option");
    db.get<aString>("option")="none"; // default option
  }
  aString & option = db.get<aString>("option");

  // Here is the CompositeGrid: 
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="no forcing" )
    {
      option="none";
    }
    else if( answer=="gaussian sources" )
    {
      // define a Gaussian forcing
      option="gaussianSources";

      // We save parameters in the data base 

      if( !db.has_key("numberOfGaussianSources") ) db.put<int>("numberOfGaussianSources");
      int & numberOfGaussianSources = db.get<int>("numberOfGaussianSources");

      gi.inputString(answer2,sPrintF("Enter the number of Gaussian sources (default = 1)"));
      sScanF(answer2,"%i",&numberOfGaussianSources);

      if( numberOfDimensions==2 )
      {
	printF("The Gaussian source in 2D is of the form:\n"
               " g(x,y,t) = a*sin(2*pi*omega*(t-t0) )*exp( -beta*[ (x-x0)^2 + (y-y0)^2 ]^p )\n"
	       " F(Ex) = -(y-y0)*g(x,y,t) \n"
	       " F(Ey) =  (x-x0)*g(x,y,t) \n"
	       " F(Hz) =         g(x,y,t) \n"
	  );
      }
      else
      {
	printF("The Gaussian source in 3D is of the form:\n"
               " g(x,y,z,t) = a*sin(2*pi*omega*(t-t0) )*exp( -beta*[ (x-x0)^2 + (y-y0)^2 + (z-z0)^2 ]^p )\n"
	       " F(Ex) = [(z-z0)-(y-y0)]*g(x,y,z,t) \n"
	       " F(Ey) = [(x-x0)-(z-z0)]*g(x,y,z,t) \n"
	       " F(Ez) = [(y-xy)-(x-x0)]*g(x,y,z,t) \n"
	  );
      }
      
      if( !db.has_key("gaussianParameters") )
	db.put<RealArray>("gaussianParameters");

      RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");
      gaussianParameters.redim(8,numberOfGaussianSources);
      gaussianParameters=0.;
      
      for( int m=0; m<numberOfGaussianSources; m++ )
      {
        real a=1., beta=10., omega=1., p=1., x0=0., y0=0., z0=0., t0=0.;
	gi.inputString(answer2,sPrintF("Source %i: Enter a,beta,omega,p,x0,y0,z0,t0",m));
        sScanF(answer2,"%e %e %e %e %e %e %e %e",&a,&beta,&omega,&p,&x0,&y0,&z0,&t0);

        printF("Gaussian source %i: setting a=%8.2e, beta=%8.2e, omega=%8.2e, p=%8.2e, x0=%8.2e, y0=%8.2e, "
               "z0=%8.2e, t0=%8.2e\n", m,a,beta,omega,p,x0,y0,z0,t0);

        gaussianParameters(0,m)=a; 
        gaussianParameters(1,m)=beta; 
        gaussianParameters(2,m)=omega; 
        gaussianParameters(3,m)=p;
        gaussianParameters(4,m)=x0; 
        gaussianParameters(5,m)=y0; 
        gaussianParameters(6,m)=z0;
        gaussianParameters(7,m)=t0;
      }

    }
    else 
    {
      printF("Maxwell::setupUserDefinedForcing:ERROR: unknown option =[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
  
  gi.unAppendTheDefaultPrompt();
  return 0;
}



//! This routine is called when cgmx is finished and can be used to clean up memory.
void Maxwell::
userDefinedForcingCleanup()
{
  printF("***userDefinedForcingCleanup: delete arrays\n");

  if( dbase.has_key("userDefinedForcingData") )
  {
    DataBase & db = dbase.get<DataBase>("userDefinedForcingData");

    // For now we do thing here 

  }
  
}

