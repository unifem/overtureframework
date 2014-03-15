// ========================================================================================================
// 
// Cgmx user defined intial conditions 
// 
// ========================================================================================================

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
/// \brief: Evaluate the user defined initial conditions.
/// \details This function is called to actually assign user 
///   defined initial conditions. The function setupUserDefinedInitialConditions is first 
///   called to assign the option and parameters. Rewrite or add new options to 
///   this function and to setupUserDefinedInitialConditions to supply your own initial conditions.
///
/// \Notes:
///  When using adaptive mesh refinement, this function may be called multiple times as the
///  AMR hierarchy is built up.
///
//==============================================================================================
int Maxwell::
userDefinedInitialConditions(int current, real t, real dt )
{

  // Look for the userDefinedForcing sub-directory in the data-base
  if( !dbase.has_key("userDefinedInitialConditionData") )
  {
    // if the directory is not there then assume that there is no user defined forcing
    return 0;
  }
  DataBase & db = dbase.get<DataBase>("userDefinedInitialConditionData");

  aString & option= db.get<aString>("option");

  if( option=="none" ) // No user defined forcing has been specified
   return 0;


  if( true )
  {
    printF("userDefinedInitialConditions: t=%9.3e option=[%s]\n",t,(const char*)option);
  }

  // Here is the CompositeGrid: 
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  // Here is the current and previous solutions: 
  realCompositeGridFunction & u  = cgfields[current];
  const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
  realCompositeGridFunction & um = cgfields[prev];


  int i1,i2,i3;
  Index I1,I2,I3;

  // --- Loop over all grids and assign values to all components. ----
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
    const realArray & x = mg.vertex();
    OV_GET_SERIAL_ARRAY_CONST(real,x,xLocal);
    OV_GET_SERIAL_ARRAY(real,u[grid],uLocal);
    OV_GET_SERIAL_ARRAY(real,um[grid],umLocal);

    getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
    // getIndex( mg.gridIndexRange(),I1,I2,I3 );  // boundary plus interior points.
    // restrict bounds to local processor, include parallel ghost pts:
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);   
    if( !ok ) continue; // no points on this processor (NOTE: no communication should be done after this point)

    if( option=="gaussianPulses" )
    {

      int & numberOfGaussianPulses = db.get<int>("numberOfGaussianPulses");
      RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");

      uLocal=0.;
      umLocal=0.;

      // Add the Gaussian pulse terms:
      for( int m=0; m<numberOfGaussianPulses; m++ )
      {
	real a    = gaussianParameters(0,m);
	real beta = gaussianParameters(1,m); 
	real omega= gaussianParameters(2,m); 
	real p    = gaussianParameters(3,m);
	real x0   = gaussianParameters(4,m); 
	real y0   = gaussianParameters(5,m); 
	real z0   = gaussianParameters(6,m);
	real t0   = gaussianParameters(7,m);

	if( true )
	  printF("UIC: Gaussian pulse %i: setting a=%8.2e, beta=%8.2e, omega=%8.2e, p=%8.2e, x0=%8.2e, y0=%8.2e, "
		 "z0=%8.2e, t0=%8.2e\n", m,a,beta,omega,p,x0,y0,z0,t0);

	const real cost =cos(2.*Pi*omega*(t-t0));
	const real costm=cos(2.*Pi*omega*(t-t0-dt));

	if( method==nfdtd )
	{
	  // -- We are solving Maxwell's as a second-order system ---
          // We must assign u(t) and u(t-dt) as initial conditions
	  printF("UIC: t=%8.2e, cost=%8.3e\n",t,cost);
	  

	  if( mg.numberOfDimensions()==2 )
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1);
	      real g = a*exp( -beta*pow( SQR(x-x0)+SQR(y-y0), p ) );

	      uLocal(i1,i2,i3,ex) += -(y-y0)*cost*g;
	      uLocal(i1,i2,i3,ey) +=  (x-x0)*cost*g;
	      uLocal(i1,i2,i3,hz) +=         cost*g;

	      umLocal(i1,i2,i3,ex) += -(y-y0)*costm*g;
	      umLocal(i1,i2,i3,ey) +=  (x-x0)*costm*g;
	      umLocal(i1,i2,i3,hz) +=         costm*g;
	    }
	  }
	  else
	  {
	    // -- 3D ---
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1), z=xLocal(i1,i2,i3,2);
	      real g = a*exp( -beta*pow( SQR(x-x0)+SQR(y-y0)+SQR(z-z0), p ) );

	      uLocal(i1,i2,i3,ex) += ((z-z0)-(y-y0))*cost*g;
	      uLocal(i1,i2,i3,ey) += ((x-x0)-(z-z0))*cost*g;
	      uLocal(i1,i2,i3,ez) += ((y-y0)-(x-x0))*cost*g;

	      umLocal(i1,i2,i3,ex) += ((z-z0)-(y-y0))*costm*g;
	      umLocal(i1,i2,i3,ey) += ((x-x0)-(z-z0))*costm*g;
	      umLocal(i1,i2,i3,ez) += ((y-y0)-(x-x0))*costm*g;

	    }
	  }
	}
	else // method!=nfdtd
	{
	  printF("userDefinedInitialConditions:ERROR: method!=nfdtd -- not implemented for other methods yet.\n");
	  OV_ABORT("ERROR");
	}


      } // end for m 
    } // end option Gaussian pulses
    else
    {
      printF("Cgmx::userDefinedInitialConditions: Unknown option =[%s]",(const char*)option);
      OV_ABORT("error");
    }

  }

  return 0;
}




//==============================================================================================
/// \brief choose a user defined initial condition.
/// \details This function is used to setup and define the initial conditions.
/// The function userDefinedInitialConditions (above) is called to actually evaluate the initial conditions.
///  Rewrite or add new options to  this routine to supply your own initial conditions.
/// Choose the "user defined" option from the initial conditions options to have this routine
///
//==============================================================================================
int Maxwell::
setupUserDefinedInitialConditions()
{
  GenericGraphicsInterface & gi = *gip;

  // here is a menu of possible forcing options
  aString menu[]=  
  {
    "no user defined initial condition",
    "gaussian pulses",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user defined IC");

  // Make a sub-directory in the data-base to store variables used in userDefinedForcing
  if( !dbase.has_key("userDefinedInitialConditionData") )
    dbase.put<DataBase>("userDefinedInitialConditionData");

  DataBase & db = dbase.get<DataBase>("userDefinedInitialConditionData");

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
    else if( answer=="gaussian pulses" )
    {
      // Define a "pulse"
      option="gaussianPulses";  // This name is used in the userDefinedInitialConditions function above.

      if( !db.has_key("numberOfGaussianPulses") ) db.put<int>("numberOfGaussianPulses");
      int & numberOfGaussianPulses = db.get<int>("numberOfGaussianPulses");

      gi.inputString(answer2,sPrintF("Enter the number of Gaussian pulses (default = 1)"));
      sScanF(answer2,"%i",&numberOfGaussianPulses);

      if( numberOfDimensions==2 )
      {
	printF("The Gaussian pulse initial condition in 2D is of the form:\n"
               " g(x,y,t) = a*cos(2*pi*omega*(t-t0) )*exp( -beta*[ (x-x0)^2 + (y-y0)^2 ]^p )\n"
	       " F(Ex) = -(y-y0)*g(x,y,t) \n"
	       " F(Ey) =  (x-x0)*g(x,y,t) \n"
	       " F(Hz) =         g(x,y,t) \n"
	  );
      }
      else
      {
	printF("The Gaussian pulse initial condition in 3D is of the form:\n"
               " g(x,y,z,t) = a*cos(2*pi*omega*(t-t0) )*exp( -beta*[ (x-x0)^2 + (y-y0)^2 + (z-z0)^2 ]^p )\n"
	       " F(Ex) = [(z-z0)-(y-y0)]*g(x,y,z,t) \n"
	       " F(Ey) = [(x-x0)-(z-z0)]*g(x,y,z,t) \n"
	       " F(Ez) = [(y-xy)-(x-x0)]*g(x,y,z,t) \n"
	  );
      }
      
      if( !db.has_key("gaussianParameters") )
	db.put<RealArray>("gaussianParameters");

      RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");
      gaussianParameters.redim(8,numberOfGaussianPulses);
      gaussianParameters=0.;
      
      for( int m=0; m<numberOfGaussianPulses; m++ )
      {
        real a=1., beta=10., omega=1., p=1., x0=0., y0=0., z0=0., t0=0.;
	gi.inputString(answer2,sPrintF("Pulse %i: Enter a,beta,omega,p,x0,y0,z0,t0",m));
        sScanF(answer2,"%e %e %e %e %e %e %e %e",&a,&beta,&omega,&p,&x0,&y0,&z0,&t0);

        printF("Gaussian pulse %i: setting a=%8.2e, beta=%8.2e, omega=%8.2e, p=%8.2e, x0=%8.2e, y0=%8.2e, "
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
      printF("Unknown option =[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
  
  gi.unAppendTheDefaultPrompt();
  return 0;
}



//! This routine is called when Cgsm is finished with the initial conditions and can 
//!  be used to clean up memory.
void Maxwell::
userDefinedInitialConditionsCleanup()
{
  printF("***userDefinedInitialConditionsCleanup: delete arrays\n");

  if( dbase.has_key("userDefinedInitialConditionData") )
  {
    DataBase & db = dbase.get<DataBase>("userDefinedInitialConditionData");

    // Do nothing for now 

  }
}

