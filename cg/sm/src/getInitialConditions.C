#include "Cgsm.h"
#include "SmParameters.h"
#include "ShowFileReader.h"
#include "display.h"
#include "App.h"
#include "ParallelUtility.h"
#include "DialogState.h"

double 
getRayleighSpeed( double rho, double mu, double lambda );


// *** we need to merge the options here with those in the base class ***

// ===================================================================================================================
/// \brief Determine the type of initial conditions to assign.
/// \param command (input) : optionally supply a command to execute. Attempt to execute the command
///    and then return. The return value is 0 if the command was executed, 1 otherwise.
/// \param interface (input) : use this dialog. If command=="build dialog", fill in the dialog and return.
/// \param guiState (input) : use this GUIState if provided.
/// \param dialogState (input) : add items found here to the dialog.
// ===================================================================================================================
int Cgsm::
getInitialConditions(const aString & command /* = nullString */,
		     DialogData *interface /* =NULL */,
                     GUIState *guiState /* = NULL */,
                     DialogState *dialogState /* = NULL */ )
{
  int returnValue=0;
  // Parameters::InitialConditionOption & initialConditionOption = parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption");
  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  
  realCompositeGridFunction & u = gf[current].u;
  aString & hempInitialConditionOption = parameters.dbase.get<aString>("hempInitialConditionOption");
  aString & specialInitialConditionOption = parameters.dbase.get<aString>("specialInitialConditionOption");
  
//   // This grid function has not been created yet
//   u.updateToMatchGrid(cg,nullRange,nullRange,nullRange,numberOfComponents);
//   for( int n=0; n<numberOfComponents; n++ )
//   {
//     u.setName(parameters.dbase.get<aString* >("componentName")[n],n);
//   }


  aString prefix = "OBIC:"; // prefix for commands to make them unique.

  const bool executeCommand = command!=nullString;
  if( false &&  // don't check prefix for now
      executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;


  // Use the input GUIState and Dialog data if they are provided.
  GUIState myGui;
  myGui.setWindowTitle("Initial Condition Options");
  myGui.setExitCommand("exit", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)myGui;
  GUIState & gui = guiState!=NULL ? *guiState : myGui;
  
  char buff[100];

  if( interface==NULL || command=="build dialog" )
  {
    // dialog.setWindowTitle("Cgsm forcing options");

    // Put dialog info here to be passed to the base class
    DialogState dialogState;
    

    dialog.setOptionMenuColumns(1);

    const int maxCommands=20;
    aString cmd[maxCommands];

//     // create a new menu with options for choosing a component.
//     if( numberOfComponents>0 )
//     {
//       aString *cmd = new aString[numberOfComponents+1];
//       aString *label = new aString[numberOfComponents+1];
//       for( int n=0; n<numberOfComponents && n<maxCommands-1 ; n++ )
//       {
// 	label[n]=u.getName(n);
// 	cmd[n]="plot:"+u.getName(n);

//       }
//       cmd[numberOfComponents]="";
//       label[numberOfComponents]="";
    
//       dialog.addOptionMenu("plot component:", cmd,label,0);
//       delete [] cmd;
//       delete [] label;
//     }

    aString initialConditionOptionCommands[] = {"defaultInitialCondition", 
						"planeWaveInitialCondition",
						"gaussianPlaneWave",
						"gaussianPulseInitialCondition",
						"squareEigenfunctionInitialCondition",  
						"annulusEigenfunctionInitialCondition",
						"zeroInitialCondition",
						"planeWaveScatteredFieldInitialCondition",
						"planeMaterialInterfaceInitialCondition",
						"gaussianIntegralInitialCondition",
						"twilightZoneInitialCondition",
						"parabolicInitialCondition",
                                                "specialInitialCondition",
                                                "hempInitialCondition",
                                                "knownSolutionInitialCondition",
						"" };

    dialog.addOptionMenu("initial conditions:", initialConditionOptionCommands, initialConditionOptionCommands, 
			 (int)initialConditionOption );

//     aString twilightZoneOptionCommands[] = {"polynomial", 
// 					    "trigonometric",
// 					    "pulse",
// 					    "" };

//     Parameters::TwilightZoneChoice & twilightZoneChoice = 
//       parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice");
//     dialog.addOptionMenu("TZ option:", twilightZoneOptionCommands, twilightZoneOptionCommands, 
// 			 (int)twilightZoneChoice );


    // ----- Text strings ------
    const int numberOfTextStrings=30;
    
    // dialogState.setNumberOfTextStrings(numberOfTextStrings);
    
//     aString textCommands[numberOfTextStrings];
//     aString textLabels[numberOfTextStrings];
//     aString textStrings[numberOfTextStrings];

    aString *& textCommands = dialogState.textCommands;
    aString *& textLabels   = dialogState.textLabels;
    aString *& textStrings  = dialogState.textStrings;

    textCommands = new aString [numberOfTextStrings];  // these will be deleted when dialogState is destroyed
    textLabels   = new aString [numberOfTextStrings];
    textStrings  = new aString [numberOfTextStrings];
    
    int nt=0;

    textCommands[nt] = "kx,ky,kz";  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i,%i,%i",kx,ky,kz);  nt++; 
//  textCommands[nt] = "frequency";  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",frequency); nt++; 

    textCommands[nt] = "Gaussian plane wave:";
    textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g %g %g %g (beta,x0,y0,z0)",
					     betaGaussianPlaneWave,x0GaussianPlaneWave,y0GaussianPlaneWave,z0GaussianPlaneWave); nt++; 

    textCommands[nt] = "Gaussian pulse:";
    textLabels[nt]=textCommands[nt]; 
    sPrintF(textStrings[nt], "%g %g %g %g %g %g (beta,scale,exponent,x0,y0,z0)",
	    gaussianPulseParameters[0][0],gaussianPulseParameters[0][1],gaussianPulseParameters[0][2],
	    gaussianPulseParameters[0][3],gaussianPulseParameters[0][4],gaussianPulseParameters[0][5]); nt++;

    textCommands[nt] = "Special initial condition option:";
    textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s",(const char*)specialInitialConditionOption);
    nt++;

    if( pdeVariation==SmParameters::hemp )
    {
      textCommands[nt] = "Hemp initial condition option:";
      textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s",(const char*)hempInitialConditionOption);
      nt++;
    }
    
//     ArraySimpleFixed<real,4,1,1,1> & omega = parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");
//     textCommands[nt] = "TZ omega:";
//     textLabels[nt]=textCommands[nt]; 
//     sPrintF(textStrings[nt], "%g %g %g %g (fx,fy,fz,ft)",omega[0],omega[1],omega[2],omega[3]); nt++;

//     const int & tzDegreeSpace= parameters.dbase.get<int >("tzDegreeSpace");
//     const int & tzDegreeTime = parameters.dbase.get<int >("tzDegreeTime");
//     textCommands[nt] = "degreeSpace, degreeTime";  
//     textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i, %i",tzDegreeSpace,tzDegreeTime); nt++; 

    // null strings terminal list
    assert( nt<numberOfTextStrings );
    textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
    // dialog.setTextBoxes(textCommands, textLabels, textStrings);


    // we add the textCommands to the the base class options:
    DomainSolver::getInitialConditions(command,interface,guiState,&dialogState);

    if( executeCommand ) return 0;
  }
  

  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  aString answer,line;
  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt(">initial conditions");
  }


  int len=0, found=0;
  for(int it=0; ; it++)
  {
    bool newInitialConditionsChosen=false;
    bool plotSolution=false;
    
    if( !executeCommand )
      gi.getAnswer(answer,"");
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }
  

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);  // strip off the prefix

    if( debug() & 2 ) printF(" *** getInitialConditions: answer=[%s]\n",(const char*)answer);

    // gi.getMenuItem(icMenu,answer,"Make a choice for initial conditions");
    int grid;
    Index I1,I2,I3;
    if( answer=="exit" || answer=="done" || answer=="continue" )
    {
      break;
    }
    else if( answer=="defaultInitialCondition" ||
	     answer=="planeWaveInitialCondition" ||
	     answer=="gaussianPlaneWave" ||
	     answer=="gaussianPulseInitialCondition" ||
	     answer=="squareEigenfunctionInitialCondition" ||  
	     answer=="annulusEigenfunctionInitialCondition" ||
	     answer=="zeroInitialCondition" ||
	     answer=="planeWaveScatteredFieldInitialCondition" ||
	     answer=="planeMaterialInterfaceInitialCondition" ||
	     answer=="gaussianIntegralInitialCondition" ||
	     answer=="twilightZoneInitialCondition" ||
	     answer=="parabolicInitialCondition" ||
	     answer=="specialInitialCondition" ||
	     answer=="hempInitialCondition" ||
             answer=="knownSolutionInitialCondition" )
    {
      initialConditionOption=
	(answer=="planeWaveInitialCondition" ? planeWaveInitialCondition :
	 answer=="gaussianPlaneWave"         ? gaussianPlaneWave : 
	 answer=="gaussianPulseInitialCondition" ? gaussianPulseInitialCondition :
	 answer=="squareEigenfunctionInitialCondition" ? squareEigenfunctionInitialCondition :
	 answer=="annulusEigenfunctionInitialCondition" ? annulusEigenfunctionInitialCondition :
	 answer=="planeWaveScatteredFieldInitialCondition" ? planeWaveScatteredFieldInitialCondition :
	 answer=="zeroInitialCondition" ?  zeroInitialCondition :
	 answer=="planeMaterialInterfaceInitialCondition" ? planeMaterialInterfaceInitialCondition :
	 answer=="gaussianIntegralInitialCondition" ? gaussianIntegralInitialCondition :
	 answer=="twilightZoneInitialCondition" ? twilightZoneInitialCondition :
	 answer=="parabolicInitialCondition" ? parabolicInitialCondition :
	 answer=="specialInitialCondition" ? specialInitialCondition :
	 answer=="hempInitialCondition" ? hempInitialCondition :
         answer=="knownSolutionInitialCondition" ? knownSolutionInitialCondition :
	 defaultInitialCondition);

      if( answer=="squareEigenfunctionInitialCondition" )
      {
	gi.inputString(line,"Enter the relative frequencies omegax,omegay,omegaz (integers)");
	sScanF(line,"%e %e %e ",&initialConditionParameters[0],&initialConditionParameters[1],
	       &initialConditionParameters[2]);
	printF("Using omegax=%f ,omegay=%f ,omegaz=%f\n",initialConditionParameters[0],initialConditionParameters[1],
	       initialConditionParameters[2]);

	forcingOption=noForcing;
      }
      else if( answer=="annulusEigenfunctionInitialCondition" )
      {
        initialConditionParameters[0]=0;
        initialConditionParameters[1]=0;
        initialConditionParameters[2]=0;
        initialConditionParameters[3]=1.;
        initialConditionParameters[4]=2.;
	
        printF(" The annulus eigenfunction is defined by \n"
               "     opt : 0=displacement BC, 1=traction BC,n"
               "     m,n : m=0,1,2,..., n=0,1,2,... denotes the eigenvalue."
	       "     p0,p1 : inner and outer pressure for traction BC,n");
	gi.inputString(line,"Enter the opt,m,n,p0,p1");
	sScanF(line,"%e %e %e %e %e",&initialConditionParameters[0],&initialConditionParameters[1],
                     &initialConditionParameters[2],&initialConditionParameters[3],&initialConditionParameters[4]);
	printF("Using opt=%i, m=%i, n=%i, p0=%8.2e, p1=%8.2e\n",int(initialConditionParameters[0]+.5),
               int(initialConditionParameters[1]+.5),
               int(initialConditionParameters[2]+.5),
               initialConditionParameters[3],
               initialConditionParameters[4]);
	
	forcingOption=noForcing;
	knownSolutionOption=annulusEigenfunctionKnownSolution;
	
      }
      else if( initialConditionOption==knownSolutionInitialCondition )
      {
	specialInitialConditionOption="knownSolutionInitialCondition";   // *wdh* 2017/05/25
      }
      

      dialog.getOptionMenu("initial conditions:").setCurrentChoice((int)initialConditionOption);
      newInitialConditionsChosen=true;
    }
    else if( answer=="polynomial" ||
	     answer=="trigonometric" ||
	     answer=="pulse" )
    {
      Parameters::TwilightZoneChoice & twilightZoneChoice = 
	parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice");
      twilightZoneChoice =  answer=="polynomial" ? Parameters::polynomial :
	answer=="trigonometric" ? Parameters::trigonometric : Parameters::pulse;
      dialog.getOptionMenu("TZ option:").setCurrentChoice((int)twilightZoneChoice);
      newInitialConditionsChosen=true;
    }
    else if( len=answer.matches("kx,ky,kz") )
    {
      printF(" kx,ky,kz are used to define the true solution\n");
	  
      sScanF(answer(len,answer.length()-1),"%i %i %i",&kx,&ky,&kz);
      dialog.setTextLabel("kx,ky,kz",sPrintF(line, "%i,%i,%i",kx,ky,kz));
      newInitialConditionsChosen=true;
    }
    else if( len=answer.matches("degreeSpace, degreeTime") )
    {
      int & tzDegreeSpace= parameters.dbase.get<int >("tzDegreeSpace");
      int & tzDegreeTime = parameters.dbase.get<int >("tzDegreeTime");

      sScanF(answer(len,answer.length()-1),"%i %i",&tzDegreeSpace,&tzDegreeTime);
      dialog.setTextLabel("degreeSpace, degreeTime",sPrintF(line,"%i, %i",tzDegreeSpace,tzDegreeTime));

      // degreeSpaceX=degreeSpaceY=degreeSpaceZ=degreeSpace;
      newInitialConditionsChosen=true;
    }
//   else if( len=answer.matches("degreeSpaceX, degreeSpaceY, degreeSpaceZ") )
//   {
//     sScanF(answer(len,answer.length()-1),"%i %i %i %i",&degreeSpaceX,&degreeSpaceY,&degreeSpaceZ);
//     dialog.setTextLabel("degreeSpaceX, degreeSpaceY, degreeSpaceZ",sPrintF(line,"%i, %i %i",
// 											 degreeSpaceX,degreeSpaceY,degreeSpaceZ));
//   }
    else if( len=answer.matches("Gaussian plane wave:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e",&betaGaussianPlaneWave,&x0GaussianPlaneWave,&y0GaussianPlaneWave,&z0GaussianPlaneWave);
      
      dialog.setTextLabel("Gaussian plane wave:",sPrintF(line,"%g %g %g %g (beta,x0,y0,z0)",
							 betaGaussianPlaneWave,x0GaussianPlaneWave,y0GaussianPlaneWave,z0GaussianPlaneWave));
  
      newInitialConditionsChosen=true;
    }
    else if( len=answer.matches("Gaussian pulse:") )
    {
      if( numberOfGaussianPulses>=maxNumberOfGaussianPulses )
      {
	printf(" ERROR: there are too many Gaussian pulses. At most %i are allowed\n",maxNumberOfGaussianPulses);
      }
      real *gpp = gaussianPulseParameters[numberOfGaussianPulses];
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e",&gpp[0],&gpp[1],&gpp[2],&gpp[3],&gpp[4],&gpp[5]);
      
      dialog.setTextLabel("Gaussian pulse:",sPrintF(line,"%g %g %g %g %g %g (beta,scale,exponent,x0,y0,z0)",
						    gpp[0],gpp[1],gpp[2],gpp[3],gpp[4],gpp[5]));  

      printF(" Setting pulse %i parameters:  beta=%g scale=%g exponent=%g x0=%g y0=%g z0=%g\n",
	     numberOfGaussianPulses,gpp[0],gpp[1],gpp[2],gpp[3],gpp[4],gpp[5]);
      numberOfGaussianPulses++;

      newInitialConditionsChosen=true;
    }
    else if( answer=="user defined..." || answer=="user defined" )
    {
      initialConditionOption=userDefinedInitialCondition; 
      newInitialConditionsChosen=true;
      
      setupUserDefinedInitialConditions();

    }
    else if( len=answer.matches("TZ omega:") )
    {
      ArraySimpleFixed<real,4,1,1,1> & omega = parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&omega[0],&omega[1],&omega[2],&omega[3]);
      
      dialog.setTextLabel("TZ omega:",sPrintF(line,"%g %g %g %g (fx,fy,fz,ft)",omega[0],omega[1],omega[2],omega[3]));
      newInitialConditionsChosen=true;
    }

    else if( dialog.getTextValue(answer,"Special initial condition option:","%s",specialInitialConditionOption) )
    {
      printF("Options for special solutions: \n"
             "  eigenmode1d : eigen-mode in 1d.)\n"
             "  invariant : invariant of the equations of linear elasticity. \n"
             "  travelingWave : define traveling p and s (shock) waves.\n"
             "  planeTravelingWave : define traveling p and s sinusoidal waves.\n"
             "  translationAndRotation : a large translation and rotation solution.\n"
             "  sphereEigenmode : eigenmode of a sphere.\n"
             "  RayleighWave : Rayleigh surface wave.\n"
             "  pistonMotion : motion of an elastic piston.\n");
      
      newInitialConditionsChosen=true;
      if( specialInitialConditionOption=="default" || 
          specialInitialConditionOption=="eigenmode1d" || 
          specialInitialConditionOption=="invariant" )
      {
      }
      else if( specialInitialConditionOption=="travelingWave" )
      {
	aString answer2;
	printF("INFO: The (shock-wave) traveling wave solutions are combinations of p and s solutions:\n"
               "  [u1,u2] = ap* [ k1,k2] * G( k1*(x-x0)+k2*(y-y0) - cp*t )  (p-wave)\n"
               "  [u1,u2] = as* [-k2,k1] * G( k1*(x-x0)+k2*(y-y0) - cs*t )  (s-wave\n",
               "    where  G(xi)=0 for xi>0 and G(xi)=-1 for xi<0 \n");
        int np=0, ns=0;
	gi.inputString(answer2,"Enter np,ns (the number of p and s traveling waves)");
	sScanF(answer2,"%i %i",&np,&ns);

        if( !parameters.dbase.has_key("travelingWaveData") ) 
          parameters.dbase.put<std::vector<real> >("travelingWaveData");

        std::vector<real> & twd = parameters.dbase.get<std::vector<real> >("travelingWaveData");
        twd.resize(5+(np+ns)*7);

        int m=0;
        twd[m++]=np+.5;
        twd[m++]=ns+.5;

	aString buff;
        real ap,k1,k2,k3=0.,x0,y0,z0=0.;
	for( int wave=0; wave<=1; wave++ ) // p-wave, s-wave
	{
	  const int numWaves = wave==0 ? np : ns;
	  for( int n=0; n<numWaves; n++ ) 
	  {
	    gi.inputString(answer2,sPrintF(buff,"Enter ap,k1,k2,k3,x0,y0,z0 for %s-wave %i (k1,k2,k3 will be normalized)\n",
                  (wave==0 ? "p" : "s"),n));
	    sScanF(answer2,"%e %e %e %e %e %e %e",&ap,&k1,&k2,&k3,&x0,&y0,&z0);
	    real kNorm = sqrt( k1*k1 + k2*k2 + k3*k3 );
	    k1/=kNorm; k2/=kNorm; k3/=kNorm;

	    twd[m++]=ap;
	    twd[m++]=k1;
	    twd[m++]=k2;
	    twd[m++]=k3;
	    twd[m++]=x0;
	    twd[m++]=y0;
	    twd[m++]=z0;
	  
	  }
	}

      }
      else if( specialInitialConditionOption=="planeTravelingWave" )
      {
	aString answer2;
	printF("INFO: The (sine-wave) traveling wave solutions are combinations of p and s solutions:\n"
               "  [u1,u2] = ap* [ k1,k2] * G( k1*(x-x0)+k2*(y-y0) - cp*t )  (p-wave)\n"
               "  [u1,u2] = as* [-k2,k1] * G( k1*(x-x0)+k2*(y-y0) - cs*t )  (s-wave\n",
               "    where  G(xi)=sin(freq*2*pi*xi)\n");
        int np=0, ns=0;
	gi.inputString(answer2,"Enter np,ns (the number of p and s traveling waves)");
	sScanF(answer2,"%i %i",&np,&ns);

        if( !parameters.dbase.has_key("travelingWaveData") ) 
          parameters.dbase.put<std::vector<real> >("travelingWaveData");

        std::vector<real> & twd = parameters.dbase.get<std::vector<real> >("travelingWaveData");
        twd.resize(5+(np+ns)*8);

        int m=0;
        twd[m++]=np+.5;
        twd[m++]=ns+.5;

	aString buff;
        real ap,k1,k2,k3=0.,x0,y0,z0=0.,freq=1.;
	for( int wave=0; wave<=1; wave++ ) // p-wave, s-wave
	{
	  const int numWaves = wave==0 ? np : ns;
	  for( int n=0; n<numWaves; n++ ) 
	  {
	    gi.inputString(answer2,sPrintF(buff,"Enter ap,k1,k2,k3,x0,y0,z0,freq for %s-wave %i (k1,k2,k3 will be normalized)\n",
                  (wave==0 ? "p" : "s"),n));
	    sScanF(answer2,"%e %e %e %e %e %e %e %e",&ap,&k1,&k2,&k3,&x0,&y0,&z0,&freq);
	    real kNorm = sqrt( k1*k1 + k2*k2 + k3*k3 );
	    k1/=kNorm; k2/=kNorm; k3/=kNorm;
            freq*= twoPi;

	    twd[m++]=ap;
	    twd[m++]=k1;
	    twd[m++]=k2;
	    twd[m++]=k3;
	    twd[m++]=x0;
	    twd[m++]=y0;
	    twd[m++]=z0;
	    twd[m++]=freq;
	  
	  }
	}

      }
      else if( specialInitialConditionOption=="translationAndRotation" )
      {
	// Rotation solution is 
	//       u = (R(t)-I)*( X - x0 )
	//   u = displacement
	//   X = reference position
	//   x0 = origin of the rotation
	//   R(t) = rotation matrix

	printF("INFO: translation and rotation solution parameters: \n"
               "  omega = rotation rate (omega=1 corresponds to 1 rotation per unit time)\n"
               "  (x0,x1,x2) = center of rotation\n"
               "  (v0,v1,v2) = velocity\n");
        if( !parameters.dbase.has_key("translationAndRotationSolutionData") ) 
          parameters.dbase.put<std::vector<real> >("translationAndRotationSolutionData");

        std::vector<real> & trd = parameters.dbase.get<std::vector<real> >("translationAndRotationSolutionData");
        trd.resize(10);
	real omega=1., x0=0.,x1=0.,x2=0., v0=0.,v1=0.,v2=0.;
	aString answer2;
	gi.inputString(answer2,"Enter omega,x0,x1,x2,v0,v1,v2");
	sScanF(answer2,"%e %e %e %e %e %e %e",&omega,&x0,&x1,&x2,&v0,&v1,&v2);

	printF("Using omega=%8.2e, (x0,x1,x2)=(%8.2e,%8.2e,%8.2e), "
	       " (v0,v1,v2)=(%8.2e,%8.2e,%8.2e)\n",omega,x0,x1,x2,v0,v1,v2);

	trd[0]=omega*twoPi;  // *note*
	trd[1]=x0;
	trd[2]=x1;
	trd[3]=x2;
	trd[4]=v0;
	trd[5]=v1;
	trd[6]=v2;
      }
      else if( specialInitialConditionOption=="sphereEigenmode" )
      {
	// define an eigen mode of a sphere -- see cgDoc/sm/notes.pdf for more details --

        // Option: "rotary vibrations"


	printF("INFO: There are a number of eigenmodes available: \n"
               "  class=1 : Vibrations of the first class ('rotary vibration') \n"
               "      parameters: n,m : radial and angular indexes (integer) n=1,2,3,...,  m=0,1,2,...\n"
               "  class=2 : Vibrations of the second class. \n"
               "      parameters: n,m : radial and angular indexes (integer) n=1,2,3,...,  m=0,1,2,...\n");

        if( !parameters.dbase.has_key("sphereEigenmodeData") ) 
          parameters.dbase.put<std::vector<real> >("sphereEigenmodeData");

        std::vector<real> & data = parameters.dbase.get<std::vector<real> >("sphereEigenmodeData");
        data.resize(10);
	int vibrationClass=1, n=1, m=0;
	aString answer2;
	gi.inputString(answer2,"Enter class (1 or 2)");
	sScanF(answer2,"%i",&vibrationClass);
	if( vibrationClass==1 || vibrationClass==2 )
	{
          printF(" The vibrations of the first or second class are of the form \n"
                 "     u = A cos( omega_m*t ) * psi_n( k_m r ) F( r^n P_n^m )\n"
                 "     n=1,2,..., m=0,1,2,... \n");
          real rad=1.;
	  gi.inputString(answer2,"Enter n,m, and the radius of the sphere");
          sScanF(answer2,"%i %i %e",&n,&m,&rad);
          printF(" sphereEigenmode: 'vibrations of the first or second class' n=%i m=%i, radius=%e\n",n,m,rad);

	  data[0]=real(vibrationClass+.5);  // save as a real 
	  data[1]=real(n+.5);  // save as a real 
	  data[2]=real(m+.5);  // save as a real 
	  data[3]=rad;
	  
	}
        else
	{
	  printF("sphereEigenmode:ERROR: invalid value for class=%i\n",vibrationClass);
	  OV_ABORT("error");
	}
	

	
      }
      else if( specialInitialConditionOption=="RayleighWave" )
      {
        // compute the Rayleigh wave speed cr (see cgDoc/sm/notes.pdf)
	real & rho=parameters.dbase.get<real>("rho");
	real & mu = parameters.dbase.get<real>("mu");
	real & lambda = parameters.dbase.get<real>("lambda");

        real cr = getRayleighSpeed( rho,mu,lambda );
	
//         real gamma = mu/(lambda+2.*mu);
// 	complex R = (2./27.)*( 27. + gamma*(-90. + gamma*( 99. + gamma*(-32.))));
// 	complex D = (4./27.)*SQR(1.-gamma)*( 11. + gamma*(-62. + gamma*(107. + gamma*(-64.))));
	
//         complex cb = 4.*(1.-gamma)/( 2.- 4.*gamma/3. + pow( R+sqrt(D), 1./3.) + pow( R-sqrt(D), 1./3.) );


//         complex crc = sqrt( (mu/rho)*cb );

//         printF(" mu=%e, lambda=%e, rho=%e, gamma=%e, R=%e, D=%e, cb=%e, cr=%e\n",mu,lambda,rho,gamma,R,D,cb,cr);

	aString answer2;
	printF("INFO: The Rayleigh wave is defined by a set of wave numbers and amplitudes:\n"
               "  k0,a0,b0,  k1,a1,b1 ... \n"
               " The amplitudes aj,bj can be thought of coefficients in the Fourier series for the surface shape\n" 
               "  The surface shape at y=ySurf will be:\n"
               "      u2 = sum_j [ aj*cos( 2*pi*kj*(x-xShift -c*t)) + bj*sin( 2*pi*kj*(x-xShift -c*t)) ]\n"
               "  The Rayleigh wave decays exponentially into the bulk (y<ySurf).\n"
	"  The Rayleigh wave speed is cr=%9.3e for the current material parameters.\n",cr);

	real y0=0., period=1., xShift=0.;
      	gi.inputString(answer2,"Enter ySurf, period, xShift : y value of upper surface, period, shift in x and z");
	sScanF(answer2,"%e %e %e %e",&y0,&period,&xShift);
        int nk;
	gi.inputString(answer2,"Enter nk (the total number of different wave-numbers)");
	sScanF(answer2,"%i",&nk);

        if( !parameters.dbase.has_key("RayleighWaveData") ) 
          parameters.dbase.put<std::vector<real> >("RayleighWaveData");

        std::vector<real> & rwd = parameters.dbase.get<std::vector<real> >("RayleighWaveData");
        rwd.resize(4 + nk*(3));

        int m=0;
	rwd[m++]=nk+.5; 
	rwd[m++]=cr;      // save the wave speed
	rwd[m++]=y0;      // y value of top surface
	rwd[m++]=period;  // length of periodic interval
	rwd[m++]=xShift;  // shift in x
	
	real k=0., a=0., b=0.;
	for( int n=0; n<nk; n++ ) 
	{
	  gi.inputString(answer2,sPrintF(buff,"Enter k%i, a%i, b%i(wave number, amplitude for mode %i)\n",n,n,n,n));
	  sScanF(answer2,"%e %e %e",&k,&a,&b);

	  rwd[m++]=k;
	  rwd[m++]=a;
	  rwd[m++]=b;
	}

      }
      else if( specialInitialConditionOption=="pistonMotion" )
      {
        // Motion of an elastic piston (to be used for a FSI computation) (see cgDoc/mp/fluidStructure/fsm.tex)
	real & rho=parameters.dbase.get<real>("rho");
	real & mu = parameters.dbase.get<real>("mu");
	real & lambda = parameters.dbase.get<real>("lambda");

	aString answer2;
	printF("INFO: The pistonMotion solution defines an exact solution for an elastic piston (to be used in an FSI problem).\n"
               "  The motion of the right boundary at x=0 is given by F(t) = -(a/p)*t^p \n"
               "  The initial velocity is defined in such a way that the traction on the boundary at x=0 matches \n" 
               "  the pressure force that would be exerted by a compressible gas.\n");

	real a=1., p=3.;
      	gi.inputString(answer2,"Enter a,p  (Motion of the right end is : F(t)=-(a/p)^p)");
	sScanF(answer2,"%e %e",&a,&p);
        real rhog,pg,gamma=1.4,angle=0.;
	gi.inputString(answer2,"Enter rho, p, gamma, angle (density, pressure and gamma for the adjacent gas, angle for rotated piston)");
	sScanF(answer2,"%e %e %e %e",&rhog,&pg,&gamma,&angle);

        if( !parameters.dbase.has_key("pistonMotionData") ) 
          parameters.dbase.put<std::vector<real> >("pistonMotionData");

        std::vector<real> & data = parameters.dbase.get<std::vector<real> >("pistonMotionData");
        data.resize(6);

        int m=0;
        data[m++]=a;
        data[m++]=p;
        data[m++]=rhog;
        data[m++]=pg;
        data[m++]=gamma;
        data[m++]=angle;
	

      }
      else
      {
	printF("ERROR: unknown special initial condition option: %s\n",(const char*)specialInitialConditionOption);
	OV_ABORT("Cgsm::ERROR");
      }
    }
    else if( dialog.getTextValue(answer,"Hemp initial condition option:","%s",hempInitialConditionOption) ){}//

    else if( len=answer.matches("plot:") )
    {
      // plot a new component
      aString name = answer(len,answer.length()-1);
      int component=-1;
      for( int n=0; n<numberOfComponents; n++ )
      {
	if( name==u.getName(n) )
	{
	  component=n;
	  break;
	}
      }
      if( component==-1 )
      {
	printF("ERROR: unknown component name =[%s]\n",(const char*)name);
	component=0;
      }
      dialog.getOptionMenu("plot component:").setCurrentChoice(component);
      parameters.dbase.get<GraphicsParameters >("psp").set(GI_COMPONENT_FOR_CONTOURS,component);
      plotSolution=true;
    }
    else if( DomainSolver::getInitialConditions(answer,interface,guiState)==0 )
    {
      printF("Cgsm:getInitialConditions:INFO: answer found in DomainSolver::getInitialConditions\n");
      if( parameters.dbase.get<bool >("twilightZoneFlow") )
      {
	initialConditionOption=twilightZoneInitialCondition;
	forcingOption=twilightZoneForcing;
      }
      
    }
    else
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing a single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	printF("Unknown response: [%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
       
    }

    if( newInitialConditionsChosen )
    {
      // Assign the initial conditions (AMR hierachy is built later in buildAmrGridsForInitialConditions)
      assignInitialConditions(current);

    }
    if( (newInitialConditionsChosen || plotSolution) &&
        initialConditionOption!=Parameters::noInitialConditionChosen )
    {
      // plot the solution
      PlotStuffParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
      psp.set(GI_TOP_LABEL,"initial conditions");
      gi.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      PlotIt::contour(gi,u,psp);
    }
    
  }  // end for it
  
  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

  return returnValue;

}
