// ========================================================================================================
// 
// Cgad user defined intial conditions 
// 
// ========================================================================================================

#include "Cgad.h"
#include "AdParameters.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"
#include "ShowFileReader.h"
#include "interpPoints.h"


// static real S = 1.*pow(10.,-2);
// static real h0= 1.5;

//==============================================================================================
/// \brief Assign user defined initial conditions.
/// \details This function is called to actually assign user 
///   defined initial conditions. The function setupUserDefinedInitialConditions is first 
///   called to assign the option and parameters. Rewrite or add new options to 
///   this function and to setupUserDefinedInitialConditions to supply your own initial conditions.
///
/// \notes:
///  \begin{itemize}
///    \item You must fill in the realCompositeGridFunction u.
///    \item The `parameters' object holds many useful parameters.
///  \end{itemize}
///  When using adaptive mesh refinement, this function may be called multiple times as the
///  AMR hierarchy is built up.
///
/// \Return values: 0=success, non-zero=failure.
//==============================================================================================
int Cgad::
userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u )
{
  
  // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("CgadUserDefinedInitialConditionData") )
  {
    printF("userDefinedInitialConditions:ERROR: sub-directory `CgadUserDefinedInitialConditionData' not found!\n");
    Overture::abort("error");
  }
  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgadUserDefinedInitialConditionData");

  const aString & option= db.get<aString>("option");
  if( option=="none" )
    return 0;


  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & tc = parameters.dbase.get<int >("tc");
    
  const real & S  = parameters.dbase.get<real>("inverseCapillaryNumber");
  const real & G  = parameters.dbase.get<real>("scaledStokesNumber");
  const real & h0 = parameters.dbase.get<real>("thinFilmBoundaryThickness");
  const real & he = parameters.dbase.get<real>("thinFilmLidThickness");

  // Added by Kara to generate initial pressure profile
  CompositeGridOperators op(cg);
  u.setOperators(op);
    

  // Loop over all grids and assign values to all components.
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created

    OV_GET_SERIAL_ARRAY(real,mg.vertex(),vertex);
    OV_GET_SERIAL_ARRAY(real,u[grid],ug);
    
    Index I1,I2,I3;
    getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
    #ifdef USE_PPP
      // restrict bounds to local processor, include ghost
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],ug,I1,I2,I3,1);   
      if( !ok ) continue;  // no points on this processor
    #endif

    if( option=="pulse" )
    {
      RealArray & pulseParameters = db.get<RealArray>("pulseParameters");

      // Pulse parameters:
      real c= 0.;
      real t=0.;
      
      real xPulse=pulseParameters(0);
      real yPulse=pulseParameters(1);
      real zPulse=pulseParameters(2);
      real amp   =pulseParameters(3); // amplitude     
      real alpha =pulseParameters(4); // 50.; // 200.;

      #define U2D(x,y,z,t) ( amp*exp( - alpha*( SQR((x)-(xPulse-c*t)) + SQR((y)-yPulse) ) ) )
      #define U3D(x,y,z,t) (amp*exp( - alpha*( SQR((x)-(xPulse-c*t)) + SQR((y)-yPulse) + SQR((z)-zPulse) ) ))

      printF(">>> Cgad:userDefinedInitialConditions: assign pulse initial conditions...\n");
      if( numberOfDimensions==2 )
      {
          ug(I1,I2,I3,tc)=U2D(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);
          //Add by Kara to establish initial pressure, note that tc = 0
          ug(I1,I2,I3,tc+1)= -S*u[grid].laplacian()(I1,I2,I3,t);
          

          ug(I1,I2,I3,tc)=h0;
          ug(I1,I2,I3,tc+1) = 0;
          
      }
      else if( numberOfDimensions==3 )
      {
	// Displacements:
	ug(I1,I2,I3,tc)=U3D(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);
            //Add by Kara to establish initial pressure, note that tc = 0
    ug(I1,I2,I3,tc+1)= -S*u[grid].laplacian()(I1,I2,I3,t);
      }
    } // end pulse
    else if( option=="tearFilm" )
    {
      // Formula : H0 * exp(-beta*(y-yl)) + Hm
      //         H0 = height at lower lid, y=yl
      //         Hm = height at the centre of the eye
      RealArray & tearFilmParameters = db.get<RealArray>("tearFilmParameters");
      const real & H0   = tearFilmParameters(0);
      const real & beta = tearFilmParameters(1);
      const real & yl   = tearFilmParameters(2);
      const real & Hm   = tearFilmParameters(3);
      
      ug(I1,I2,I3,tc)=H0*exp(-beta*(vertex(I1,I2,I3,1)-yl)) + Hm;
      ug(I1,I2,I3,tc+1)= -S*beta*beta*(ug(I1,I2,I3,tc)-Hm);
    }
    else
    {
      printF("Cgad::userDefinedInitialConditions: Unknown option =[%s]",(const char*)option);
      OV_ABORT("error");
    }
      
  }
    
    //Code to read in solution from a show file
    // Ask Longfei

    /*
    int sol=10;
    CompositeGrid cghold;
    realCompositeGridFunction unew;
    ShowFileReader showFileReader("initialEye.show");
    showFileReader.getASolution(sol,cghold,unew);
    interpolateAllPoints(unew,u);
    */
    
    
    
  return 0;
}




//==============================================================================================
/// \brief Choose user defined initial condition parameters.
/// \details  This function is used to setup and define the initial conditions.
/// The function userDefinedInitialConditions (above) is called to actually evaluate the initial conditions.
///  Rewrite or add new options to  this routine to supply your own initial conditions.
/// Choose the "user defined" option from the initial conditions options to have this routine
/// called.
/// \Notes:
///  \begin{itemize}
///    \item You must fill in the realCompositeGridFunction u.
///    \item The `parameters' object holds many useful parameters.
///  \end{itemize}
///
/// \Return values: 0=success, non-zero=failure.
//==============================================================================================
int Cgad::
setupUserDefinedInitialConditions()
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  
  // here is a menu of possible initial conditions
  aString menu[]=  
  {
    "pulse",
    "manufactured tear film",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user defined");

  // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("CgadUserDefinedInitialConditionData") )
    parameters.dbase.get<DataBase >("modelData").put<DataBase>("CgadUserDefinedInitialConditionData");

  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgadUserDefinedInitialConditionData");
  // first time through allocate variables 
  if( !db.has_key("option") ) db.put<aString>("option");

  aString & option = db.get<aString>("option");

  option="unknown";
 
  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="pulse" )
    {
      // Define a "pulse"
      option="pulse";  // This name is used in the userDefinedInitialConditions function above when assigning the pulse.

      RealArray & pulseParameters = db.put<RealArray>("pulseParameters");
      pulseParameters.redim(10);
      pulseParameters=0.;
      
      gi.inputString(answer2,"Enter the pulse location, amplitude and exponent: x,y,z,amp,alpha");

      sScanF(answer2,"%e %e %e %e %e",&pulseParameters(0),&pulseParameters(1),&pulseParameters(2),
	     &pulseParameters(3),&pulseParameters(4));  
      printF("Pulse location = (%8.2e,%8.2e,%8.2e), amp=%8.2e, alpha=%8.2e\n",pulseParameters(0),
              pulseParameters(1),pulseParameters(2),pulseParameters(3),pulseParameters(4));
      
    }
    else if( answer=="manufactured tear film" )
    {
      // -- Here is a manfactured tear film
      option="tearFilm";
      parameters.dbase.get<bool >("manufacturedTearFilm")=true;
      
      printF(" Formula : H0 * exp(-beta*(y-yl)) + Hm \n"
             " H0 = height at lower lid, y=yl\n"
             " Hm = height at the centre of the eye\n");

      RealArray & tearFilmParameters = db.put<RealArray>("tearFilmParameters");
      tearFilmParameters.redim(10);
      tearFilmParameters=0.;

      gi.inputString(answer2,"Enter H0,beta,yl,Hm");
      sScanF(answer2,"%e %e %e %e",&tearFilmParameters(0),&tearFilmParameters(1),&tearFilmParameters(2),
	     &tearFilmParameters(3));

      printF(" Setting H0=%g, beta=%g, yl=%g, Hm=%g",tearFilmParameters(0),tearFilmParameters(1),tearFilmParameters(2),
	     tearFilmParameters(3));

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



//! This routine is called when Cgad is finished with the initial conditions and can 
//!  be used to clean up memory.
void Cgad::
userDefinedInitialConditionsCleanup()
{
  if( parameters.dbase.get<int >("myid")==0 ) 
    printP("***userDefinedInitialConditionsCleanup: delete arrays\n");

  if( parameters.dbase.get<DataBase >("modelData").has_key("CgadUserDefinedInitialConditionData") )
  {
    DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgadUserDefinedInitialConditionData");



  }

  // call the base class cleanup 
  DomainSolver::userDefinedInitialConditionsCleanup();
}

