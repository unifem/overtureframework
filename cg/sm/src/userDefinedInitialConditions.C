// ========================================================================================================
// 
// Cgsm user defined initial conditions 
// 
// ========================================================================================================

#include "Cgsm.h"
#include "SmParameters.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"


//\begin{>>CgsmInclude.tex}{\subsection{userDefinedInitialConditions}}
int Cgsm::
userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u )
//==============================================================================================
// /Description:
//   User defined initial conditions. This function is called to actually assign user 
//   defined initial conditions. The function setupUserDefinedInitialConditions is first 
//   called to assign the option and parameters. Rewrite or add new options to 
//   this function and to setupUserDefinedInitialConditions to supply your own initial conditions.
//
// /Notes:
//  \begin{itemize}
//    \item You must fill in the realCompositeGridFunction u.
//    \item The `parameters' object holds many useful parameters.
//  \end{itemize}
//  When using adaptive mesh refinement, this function may be called multiple times as the
//  AMR hierarchy is built up.
//
// /Return values: 0=success, non-zero=failure.
//\end{CgsmInclude.tex}
//==============================================================================================
{


  // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("CgsmUserDefinedInitialConditionData") )
  {
    printF("userDefinedInitialConditions:ERROR: sub-directory `CgsmUserDefinedInitialConditionData' not found!\n");
    Overture::abort("error");
  }
  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgsmUserDefinedInitialConditionData");

  const aString & option= db.get<aString>("option");
  if( option=="none" )
    return 0;


  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  // Do this for now:
  // pdeTypeForGodunovMethod==2 : SVK
  const int pdeTypeForGodunovMethod = parameters.dbase.get<int >("pdeTypeForGodunovMethod");

  real & rho=parameters.dbase.get<real>("rho");
  real & mu = parameters.dbase.get<real>("mu");
  real & lambda = parameters.dbase.get<real>("lambda");

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");

  const int v1c = parameters.dbase.get<int >("v1c");
  const int v2c = parameters.dbase.get<int >("v2c");
  const int v3c = parameters.dbase.get<int >("v3c");

  bool assignVelocities= v1c>=0 ;
  const int s11c = parameters.dbase.get<int >("s11c");
  const int s12c = parameters.dbase.get<int >("s12c");
  const int s13c = parameters.dbase.get<int >("s13c");
  const int s21c = parameters.dbase.get<int >("s21c");
  const int s22c = parameters.dbase.get<int >("s22c");
  const int s23c = parameters.dbase.get<int >("s23c");
  const int s31c = parameters.dbase.get<int >("s31c");
  const int s32c = parameters.dbase.get<int >("s32c");
  const int s33c = parameters.dbase.get<int >("s33c");

  bool assignStress = s11c >=0 ;


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
#define U2DT(x,y,z,t) ( (-2.*c*alpha)*( (x)-(xPulse-c*t) )*U2D(x,y,z,t) )
#define U2DX(x,y,z,t) ( (  -2.*alpha)*( (x)-(xPulse-c*t) )*U2D(x,y,z,t) )
#define U2DY(x,y,z,t) ( (  -2.*alpha)*( (y)-(yPulse    ) )*U2D(x,y,z,t) )

#define U3D(x,y,z,t) (amp*exp( - alpha*( SQR((x)-(xPulse-c*t)) + SQR((y)-yPulse) + SQR((z)-zPulse) ) ))
#define U3DT(x,y,z,t) ( (-2.*c*alpha)*( (x)-(xPulse-c*t) )*U3D(x,y,z,t) )
#define U3DX(x,y,z,t) ( (  -2.*alpha)*( (x)-(xPulse-c*t) )*U3D(x,y,z,t) )
#define U3DY(x,y,z,t) ( (  -2.*alpha)*( (y)-(yPulse    ) )*U3D(x,y,z,t) )
#define U3DZ(x,y,z,t) ( (  -2.*alpha)*( (z)-(zPulse    ) )*U3D(x,y,z,t) )


      if( pdeTypeForGodunovMethod==0 )
      {
	// -- linear elasticity ---
        printF(">>> Cgsm:userDefinedInitialConditions: assignGaussianPulseIC (linear-elasticity)...\n");
	if( numberOfDimensions==2 )
	{
	  // Displacements:
	  ug(I1,I2,I3,uc)=U2D(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);   
	  ug(I1,I2,I3,vc)=0.;

	  if( assignVelocities )
	  { // Some solvers need the velocity:
	    ug(I1,I2,I3,v1c) =U2DT(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);   
	    ug(I1,I2,I3,v2c) =0.;
	  }
	  if( assignStress )
	  {  // Some solvers need the stress:
	    ug(I1,I2,I3,s11c) =(lambda+2.*mu)*U2DX(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);
	    ug(I1,I2,I3,s12c) =mu*U2DY(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);
	    ug(I1,I2,I3,s21c) =ug(I1,I2,I3,s12c);
	    ug(I1,I2,I3,s22c) =0.;
	  }
	}
	else if( numberOfDimensions==3 )
	{
	  // Displacements:
	  ug(I1,I2,I3,uc)=U3D(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);   
	  ug(I1,I2,I3,vc)=0.;
	  ug(I1,I2,I3,wc)=0.;

	  if( assignVelocities )
	  {
	    ug(I1,I2,I3,v1c) =U3DT(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);   
	    ug(I1,I2,I3,v2c) =0.;
	    ug(I1,I2,I3,v3c) =0.;
	  }
	  if( assignStress )
	  {
	    ug(I1,I2,I3,s11c) =(lambda+2.*mu)*U3DX(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);
	    ug(I1,I2,I3,s12c) =mu*U3DY(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);
	    ug(I1,I2,I3,s13c) =mu*U3DZ(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),vertex(I1,I2,I3,2),t);
	    ug(I1,I2,I3,s21c) =ug(I1,I2,I3,s12c);
	    ug(I1,I2,I3,s22c) =0.;
	    ug(I1,I2,I3,s23c) =0.;
	    ug(I1,I2,I3,s31c) =ug(I1,I2,I3,s13c);
	    ug(I1,I2,I3,s32c) =ug(I1,I2,I3,s23c);
	    ug(I1,I2,I3,s33c) =0.;
	  }
	}

      }
      else
      {
	// --- SVK model ---
        printF(">>> Cgsm:userDefinedInitialConditions: assignGaussianPulseIC (SVK)...\n");

	RealArray ux(I1,I2,I3),uy(I1,I2,I3),vx(I1,I2,I3),vy(I1,I2,I3),
	  f11(I1,I2,I3),f12(I1,I2,I3),f21(I1,I2,I3),f22(I1,I2,I3),
	  s11(I1,I2,I3),s12(I1,I2,I3),s21(I1,I2,I3),s22(I1,I2,I3),trace(I1,I2,I3);

	ug(I1,I2,I3,uc)=amp*exp(-alpha*(SQR(vertex(I1,I2,I3,0)-xPulse-c*t)+SQR(vertex(I1,I2,I3,1)-yPulse)));
	ug(I1,I2,I3,vc)=0.;
	ug(I1,I2,I3,v1c)=2.*alpha*c*(vertex(I1,I2,I3,0)-xPulse-c*t)*ug(I1,I2,I3,uc);
	ug(I1,I2,I3,v2c)=0.;

	ux=-2.*alpha*(vertex(I1,I2,I3,0)-xPulse-c*t)*ug(I1,I2,I3,uc);
	uy=-2.*alpha*(vertex(I1,I2,I3,1)-yPulse)*ug(I1,I2,I3,uc);
	vx=0.;
	vy=0.;

	f11=1.+ux;
	f12=   uy;
	f21=   vx;
	f22=1.+vy;
	s11=.5*(f11*f11+f21*f21-1.);          // this is E(i,j), for now
	s12=.5*(f11*f12+f21*f22   );
	s21=s12;
	s22=.5*(f12*f12+f22*f22-1.);
	trace=s11+s22;                        // this is Tr(E)
	s11=lambda*trace+2.*mu*s11;           // this is S(i,j)
	s12=             2.*mu*s12;
	s21=s12;
	s22=lambda*trace+2.*mu*s22;

	ug(I1,I2,I3,s11c)=s11*f11+s12*f12;    // this is P(i,j) based on the current F(i,j)
	ug(I1,I2,I3,s12c)=s11*f21+s12*f22;
	ug(I1,I2,I3,s21c)=s21*f11+s22*f12;
	ug(I1,I2,I3,s22c)=s21*f21+s22*f22;

      }
    } // end pulse option
    else if( option=="rotation" )
    {
      RealArray & rotationParameters = db.get<RealArray>("rotationParameters");

      // Rotation parameters:
      real xRotation=rotationParameters(0);
      real yRotation=rotationParameters(1);
      real rotationRate=rotationParameters(2);

      printF(">>> Cgsm:userDefinedInitialConditions: assignRotationIC...\n");
      if( numberOfDimensions==2 )
      {
        // Displacements:
        ug(I1,I2,I3,uc)=0.;   
        ug(I1,I2,I3,vc)=0.;

        if( assignVelocities )
        { // Some solvers need the velocity:
          ug(I1,I2,I3,v1c)=-rotationRate*(vertex(I1,I2,I3,1)-yRotation);   
          ug(I1,I2,I3,v2c)= rotationRate*(vertex(I1,I2,I3,0)-xRotation);
        }
        if( assignStress )
        {  // Some solvers need the stress:
          ug(I1,I2,I3,s11c)=0.;
          ug(I1,I2,I3,s12c)=0.;
          ug(I1,I2,I3,s21c)=0.;
          ug(I1,I2,I3,s22c)=0.;
        }
      }
      else if( numberOfDimensions==3 )
      {
        // Displacements:
        ug(I1,I2,I3,uc)=0.;   
        ug(I1,I2,I3,vc)=0.;
        ug(I1,I2,I3,wc)=0.;

        if( assignVelocities )
        {
          ug(I1,I2,I3,v1c)=-rotationRate*(vertex(I1,I2,I3,1)-yRotation);   
          ug(I1,I2,I3,v2c)= rotationRate*(vertex(I1,I2,I3,0)-xRotation);
          ug(I1,I2,I3,v3c)= 0.;
        }
        if( assignStress )
        {
          ug(I1,I2,I3,s11c)=0.;
          ug(I1,I2,I3,s12c)=0.;
          ug(I1,I2,I3,s13c)=0.;
          ug(I1,I2,I3,s21c)=0.;
          ug(I1,I2,I3,s22c)=0.;
          ug(I1,I2,I3,s23c)=0.;
          ug(I1,I2,I3,s31c)=0.;
          ug(I1,I2,I3,s32c)=0.;
          ug(I1,I2,I3,s33c)=0.;
        }
      }
    } // end rotation option
    else
    {
      printF("Cgsm::userDefinedInitialConditions: Unknown option =[%s]",(const char*)option);
      OV_ABORT("error");
    }

  }

  return 0;
}




//\begin{>>CgsmInclude.tex}{\subsection{setupUserDefinedInitialConditions}}  
int Cgsm::
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

  // here is a menu of possible initial conditions
  aString menu[]=
  {
    "pulse",
    "rotation",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user defined");

  // Here is where parameters can be put to be saved in the show file:
  ListOfShowFileParameters & showFileParams = parameters.dbase.get<ListOfShowFileParameters>("showFileParams");

  // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("CgsmUserDefinedInitialConditionData") )
    parameters.dbase.get<DataBase >("modelData").put<DataBase>("CgsmUserDefinedInitialConditionData");

  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgsmUserDefinedInitialConditionData");
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

      // Save parameters in the show file: (one can save real, int or a string)
      // These parameters will be displayed when the show file is read with plotStuff.
      // One can also access the parameters fro userDefinedDerivedFunctions.

      // Just for example, we save a real, int and string
      real amp=pulseParameters(3);
      int numPulse=1;
      aString pulseName="myPulse";
      showFileParams.push_back(ShowFileParameter("pulseAmp",amp));
      showFileParams.push_back(ShowFileParameter("numPulse",1));
      showFileParams.push_back(ShowFileParameter("pulseName",pulseName));

    }
    else if( answer=="rotation" )
    {
      // Define a "rotation"
      option="rotation";

      RealArray & rotationParameters = db.put<RealArray>("rotationParameters");
      rotationParameters.redim(10);
      rotationParameters=0.;

      gi.inputString(answer2,"Enter the rotation center (x,y) and rotation rate (rate) :");

      sScanF(answer2,"%e %e %e",&rotationParameters(0),&rotationParameters(1),&rotationParameters(2));
      printF("Rotation center = (%8.2e,%8.2e), rate=%8.2e\n",rotationParameters(0),rotationParameters(1),rotationParameters(2));

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
void Cgsm::
userDefinedInitialConditionsCleanup()
{
  if( parameters.dbase.get<int >("myid")==0 ) 
    printP("***userDefinedInitialConditionsCleanup: delete arrays\n");

  if( parameters.dbase.get<DataBase >("modelData").has_key("CgsmUserDefinedInitialConditionData") )
  {
    DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgsmUserDefinedInitialConditionData");



  }

  // call the base class cleanup 
  DomainSolver::userDefinedInitialConditionsCleanup();
}

