#include "Cgcns.h"
#include "GenericGraphicsInterface.h"
#include "MaterialProperties.h"
#include "Chemkin.h"
#include "CnsParameters.h"
#include "EquationDomain.h"
#include "SurfaceEquation.h"


int readRestartFile(GridFunction & cgf, Parameters & parameters,
                    const aString & restartFileName =nullString );



/////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Setup the PDE to be solved.
/// \param reactionName name of the reaction.
/// \param restartChosen true if this is a restart.
/// \param originalBoundaryCondition
/// \author wdh.
/////////////////////////////////////////////////////////////////////////////////////////////////
int Cgcns::
setupPde(aString & reactionName,bool restartChosen, IntegerArray & originalBoundaryCondition)
{
  real cpu0 = getCPU();
  const int buffSize=100;
  char buff[buffSize];

  realCompositeGridFunction & u = gf[current].u;


  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  GUIState setupDialog;

  setupDialog.setWindowTitle("Cgcns Setup");
  setupDialog.setExitCommand("continue", "continue");

  aString answer, turbulenceModel="";

  aString equationDomainName="domain0";
  aString label = "Active Equation Domain : "+equationDomainName;
  setupDialog.addInfoLabel(label); 

  DialogData & surfaceEquationDialog = setupDialog.getDialogSibling();
  surfaceEquationDialog.setWindowTitle("Surface Equation Options");
  surfaceEquationDialog.setExitCommand("close surface equation options", "close");
  if( parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")!=NULL )
    parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")->update(cg,originalBoundaryCondition,gi,"build dialog",&surfaceEquationDialog);


  aString tbLabel[] = {"axisymmetric flow with swirl",
		       ""};
  int tbState[5];
  tbState[0] = (int)parameters.dbase.get<bool >("axisymmetricWithSwirl"); 

  int numColumns=1;
  setupDialog.setToggleButtons(tbLabel, tbLabel, tbState, numColumns); 

  // push buttons
  aString pbCommands[] = {"choose a grid",
			  "read a restart file",
			  // "passive scalar advection",
			  "add advected scalars",
			  "add extra variables",
			  "new equation domain...", 
			  "surface equations...",
			  ""};

  const int numRows=3;
  setupDialog.setPushButtons( pbCommands, pbCommands, numRows ); 


  setupDialog.setOptionMenuColumns(1);

  aString pdeCommands[] =  {"compressible Navier Stokes (Jameson)",
			    "compressible Navier Stokes (Godunov)",
			    "compressible Navier Stokes (multi-component)",
			    "compressible Navier Stokes (multi-fluid)",
			    "compressible Navier Stokes (non-conservative)",
			    "compressible multiphase",
			    "compressible multiphase (multi-fluid)",
			    "compressible Navier Stokes (implicit)",
			    "steady-state compressible Navier Stokes (newton)",
			    //   "all speed Navier Stokes",
			    ""     };
  setupDialog.addOptionMenu("pde", pdeCommands, pdeCommands, (int)parameters.dbase.get<CnsParameters::PDE >("pde"));

//   aString pdeModelCommands[] = {"standard model",
// 			        "Boussinesq model",
// 			        "visco-plastic model",
// 			        ""     };
//   setupDialog.addOptionMenu("model", pdeModelCommands, pdeModelCommands, (int)parameters.dbase.get<Parameters::PDEModel >("pdeModel"));

  aString reactionCommands[] =  { "no reactions",
				  "one step",
				  "branching",
				  "ignition and growth",
				  "ignition and growth desensitization",
				  "one equation mixture fraction",
				  "two equation mixture fraction and extent of reaction",
				  "one step pressure law",
				  "specify CHEMKIN reaction",
                                  "ignition-pressure reaction rate",
				  ""     };
      
  setupDialog.addOptionMenu("reaction", reactionCommands, reactionCommands, (int)parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType"));

//   aString tmCommands[] =  { "noTurbulenceModel",
// 			    "Baldwin-Lomax",
// 			    "k-epsilon",
// 			    "k-omega",
// 			    "SpalartAllmaras",
// 			    ""     };
      
//   setupDialog.addOptionMenu("turbulence model", tmCommands, tmCommands, (int)parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"));

  aString eosCommands[] =  {"ideal gas law",
			    "JWL equation of state",
			    "Mie-Gruneisen equation of state",
                            "user defined equation of state",
                            "stiffened gas equation of state",
                            "tait equation of state",
			    ""     };
      
  setupDialog.addOptionMenu("equation of state", eosCommands, eosCommands, (int)parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState"));


  aString mvCommands[] =  { "solve and move grids",
			    "move and regenerate grids only",
			    "move grids only",
			    ""     };
      
  setupDialog.addOptionMenu("motion option:", mvCommands, mvCommands, parameters.dbase.get<int >("simulateGridMotion"));

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "define real parameter"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "<name> <value>"); nt++; 

  textCommands[nt] = "define integer parameter"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "<name> <value>"); nt++; 

  textCommands[nt] = "define string parameter"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "<name> <value>"); nt++; 

  textCommands[nt] = "solver name:";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)equationDomainName);  nt++; 

//   textCommands[nt] = "domain name:";  textLabels[nt]=textCommands[nt];
//   sPrintF(textStrings[nt], "%s",(const char*)equationDomainName);  nt++; 

  textCommands[nt] = "add grid:";   textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s","none");  nt++; 

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  setupDialog.setTextBoxes(textCommands, textLabels, textStrings);

  gi.pushGUI(setupDialog);
    
  CnsParameters::PDE & pde = parameters.dbase.get<CnsParameters::PDE >("pde");
  CnsParameters::GodunovVariation & conservativeGodunovMethod = 
                          parameters.dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");
  CnsParameters::PDEVariation & pdeVariation = 
                 parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation");

  Parameters::TimeSteppingMethod & timeSteppingMethod = 
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");

  bool pdeChosen=false;
  bool gridChosen=cg.numberOfComponentGrids()>0;
  int len=0;
  
  int activeEquationDomain=0;
  if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
  {
    ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));
    // add an EquationDomain with default parameters
    equationDomainList.push_back(EquationDomain(&parameters,equationDomainName));  
    // By default all grids initially belong to the first equationDomain.
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      equationDomainList.gridDomainNumberList.push_back(activeEquationDomain);
    }
  }
  
  bool found;
  for(;;)
  {
    // gi.getMenuItem(pdeMenu,answer,"choose a PDE");
    gi.getAnswer(answer,"choose a PDE");

    if( answer=="exit" || answer=="done" || answer=="continue" )
    {
      if( pdeChosen && gridChosen )
        break;
      if( !pdeChosen )
        printF("You must choose a PDE before continuing\n");
      if( !gridChosen )
        printF("You must choose a grid before continuing\n");
        
    }
    else if( answer=="choose a grid" )
    {
      aString nameOfOGFile;
      gi.inputFileName(nameOfOGFile, ">> Enter the name of the (old) composite grid file:");
      gridChosen= getFromADataBase(cg,nameOfOGFile)==0;

      assert( cg.rcData->interpolant != NULL );
      cg.rcData->interpolant->updateToMatchGrid( cg );

      getOriginalBoundaryConditions(cg,originalBoundaryCondition);
    }
    else if( answer=="read a restart file" )
    {
      gi.inputFileName(answer,sPrintF(buff,"Enter the restart file name (default value=%s)",
				    (const char *)parameters.dbase.get<aString >("restartFileName")));
      if( answer!="" )
	parameters.dbase.get<aString >("restartFileName")=answer;

      
      GridFunction gf;
      // restartChosen=readRestartFile(gf,initialTime,parameters.dbase.get<aString >("restartFileName"))==0;
      restartChosen=::readRestartFile(gf,parameters,parameters.dbase.get<aString >("restartFileName"))==0;
      if( restartChosen )
      {
        parameters.dbase.get<real >("tInitial")=gf.t;
	u.reference(gf.u);
        gf.cg.rcData->interpolant=cg.rcData->interpolant;
        cg.reference(gf.cg);
        assert( cg.rcData->interpolant != NULL );
        cg.rcData->interpolant->updateToMatchGrid( cg );
        u.updateToMatchGrid(cg); // **
        // interpolant->setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
      }
      pdeChosen=pdeChosen || restartChosen;
      gridChosen=gridChosen || restartChosen;
      // gi.contour(u);
      
    }
    //                      01234567890123456789012345
    else if( answer(0,25)=="compressible Navier Stokes" )
    {
      pdeChosen=true;
      pde=CnsParameters::compressibleNavierStokes;
      parameters.pdeName = "compressibleNavierStokes";
      pdeName=answer;

      // Choose the default value for the Gas  constant
      parameters.dbase.get<real >("Rg")=1.;  // Don expects Rg==1

      if( answer=="compressible Navier Stokes (Jameson)" )
      {
	pdeVariation=CnsParameters::conservativeWithArtificialDissipation;
      }
      else if( answer=="compressible Navier Stokes (Godunov)" )
      {
        if( REAL_EPSILON != DBL_EPSILON )
	{
	  gi.outputString("SORRY: compressible Navier Stokes (Godunov) only available in double precision");
	  gi.stopReadingCommandFile();
	}
	else
	{
	  pdeVariation=CnsParameters::conservativeGodunov;
	  timeSteppingMethod=Parameters::forwardEuler;
	  conservativeGodunovMethod=CnsParameters::fortranVersion;
	}
      }
      else if( answer=="compressible Navier Stokes (multi-component)" )
      {
        if( REAL_EPSILON != DBL_EPSILON )
	{
	  gi.outputString("SORRY: compressible Navier Stokes (multi-component) only available in double precision");
	  gi.stopReadingCommandFile();
	}
	else
	{
	  pdeVariation=CnsParameters::conservativeGodunov;
	  timeSteppingMethod=Parameters::forwardEuler;
	  conservativeGodunovMethod=CnsParameters::multiComponentVersion; // Jeff Banks' code
	}
      }
      else if( answer=="compressible Navier Stokes (multi-fluid)" )
      {
        if( REAL_EPSILON != DBL_EPSILON )
	{
	  gi.outputString("SORRY: compressible Navier Stokes (multi-fluid) only available in double precision");
	  gi.stopReadingCommandFile();
	}
	else
	{
	  pdeVariation=CnsParameters::conservativeGodunov;
	  timeSteppingMethod=Parameters::forwardEuler;

	  conservativeGodunovMethod=CnsParameters::multiFluidVersion; // Don's multifluid code

	  parameters.dbase.get<Parameters::InterpolationTypeEnum >("interpolationType")=Parameters::interpolatePrimitiveAndPressure;
	  
	}
      }
      else if( answer=="compressible Navier Stokes (non-conservative)" )
	pdeVariation=CnsParameters::nonConservative;
      else if ( answer=="compressible Navier Stokes (implicit)" )
	{
	  if( REAL_EPSILON != DBL_EPSILON )
	    {
	      gi.outputString("SORRY: compressible Navier Stokes (implicit) only available in double precision");
	      gi.stopReadingCommandFile();
	    }
	  else
	    {
	      pdeVariation=CnsParameters::nonConservative;
	      timeSteppingMethod = Parameters::implicit;
	      parameters.dbase.get<real >("implicitFactor") = 1.;
	      parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::trapezoidal;
	      parameters.dbase.get<int >("refactorFrequency")=1;
	      parameters.dbase.get<bool >("useDimensionalParameters") = false;
	      parameters.dbase.get<int >("extrapolateInterpolationNeighbours") = false;
	    }
	}
      else
	{
	  printF("unknown choice for PDE, answer=[%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
	}
    }
    else if ( answer=="steady-state compressible Navier Stokes (newton)" )
      {
	pdeChosen=true;
	pde=CnsParameters::compressibleNavierStokes;
	parameters.pdeName = "compressibleNavierStokes";

	pdeName=answer;
	
	// Choose the default value for the Gas  constant
	parameters.dbase.get<real >("Rg")=1.;  // Don expects Rg==1
	if( REAL_EPSILON != DBL_EPSILON )
	  {
	    gi.outputString("SORRY: compressible Navier Stokes (implicit) only available in double precision");
	    gi.stopReadingCommandFile();
	  }
	else
	  {
	    pdeVariation=CnsParameters::nonConservative;
	    timeSteppingMethod = Parameters::steadyStateNewton;
	    parameters.dbase.get<real >("implicitFactor") = .1;
	    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::trapezoidal;
	    parameters.dbase.get<bool >("useDimensionalParameters") = false;
	    parameters.dbase.get<int >("extrapolateInterpolationNeighbours") = false;
	    parameters.dbase.get<int >("refactorFrequency")=1;
	  }
      }
    else if( answer=="compressible multiphase" ||
             answer=="compressible multiphase (multi-fluid)" )
    {
      pdeChosen=true;
      // Choose the default value for the Gas  constant
      parameters.dbase.get<real >("Rg")=1.;  // Don expects Rg==1

      pde=CnsParameters::compressibleMultiphase;
      parameters.pdeName = "compressibleMultiphase";

      pdeName=answer;
      pdeVariation=CnsParameters::conservativeGodunov;
      timeSteppingMethod=Parameters::forwardEuler;      

      if( answer=="compressible multiphase" )
        conservativeGodunovMethod=CnsParameters::fortranVersion;
      else
        conservativeGodunovMethod=CnsParameters::multiFluidVersion;

    }
//     else if( answer=="all speed Navier Stokes" )
//     {
//       pdeChosen=true;
//       parameters.dbase.get<Parameters::PDE >("pde")=Parameters::allSpeedNavierStokes;
//       pdeName=answer;
//     }
    else if( answer=="one step" )
    {
      reactionName="one step";
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::oneStep;
    }
    else if( answer=="branching" )
    {
      reactionName="branching";
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::branching;
    }
    else if( answer=="passive scalar advection" )
    {
      reactionName="passive scalar advection"; 
      printF("Passive scalar advection is ON. Only supported with incompressible Navier-Stokes.\n");
    }
    else if( answer=="one equation mixture fraction" )
    {
      reactionName="one equation mixture fraction"; // parameters.dbase.get<bool >("advectPassiveScalar") = TRUE;
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::oneEquationMixtureFraction;

      printF("One equation mixture fraction is ON. Only supported with all-speed Navier-Stokes.\n");
    }
    else if( answer=="two equation mixture fraction and extent of reaction" )
    {
      reactionName="two equation mixture fraction and extent of reaction";
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::twoEquationMixtureFractionAndExtentOfReaction;

      printF("Two equation mixture fraction and extent of reaction is ON. "
             "Only supported with all-speed Navier-Stokes.\n");
    }
    else if( answer=="ignition and growth" )
    {
      reactionName="ignition and growth";
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::ignitionAndGrowth;
    }
    else if( answer=="ignition and growth desensitization" )
    {
      reactionName="ignition and growth desensitization";
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::igDesensitization;
    }
    else if( answer=="specify CHEMKIN reaction" )
    {
      gi.inputString(reactionName,"Enter the name of a CHEMKIN reaction");
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::chemkinReaction;
      printF("reaction =[%s]\n",(const char*)reactionName);
    }
    else if( answer=="one step pressure law" )
    {
      reactionName="one step";
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::oneStepPress;
    }
    else if( answer=="ignition-pressure reaction rate" )
    {
      reactionName="ignition-pressure reaction rate";
      parameters.dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")=CnsParameters::ignitionPressureReactionRate;
    }

    else if( answer=="ideal gas law" )
    {
      parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=CnsParameters::idealGasEOS;
    }
    else if( answer=="JWL equation of state" )
    {
      parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=CnsParameters::jwlEOS;
    }
    else if( answer=="Mie-Gruneisen equation of state" )
    {
      parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=CnsParameters::mieGruneisenEOS;
    }
    else if( answer=="user defined equation of state" )
    {
      parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=CnsParameters::userDefinedEOS;
      // -- choose the user defined equation of state: 
      parameters.updateUserDefinedEOS(gi);

    }
    else if( answer=="stiffened gas equation of state" )
    {
      parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=CnsParameters::stiffenedGasEOS;
    }
    else if( answer=="tait equation of state" )
    {
      parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=CnsParameters::taitEOS;
    }
    else if( setupDialog.getToggleValue(answer,"axisymmetric flow with swirl",parameters.dbase.get<bool >("axisymmetricWithSwirl")) )
    {
      parameters.dbase.get<bool >("axisymmetricProblem")=parameters.dbase.get<bool >("axisymmetricWithSwirl");
    }
    else if( answer=="add advected scalars" )
    {
      gi.inputString(answer,"Enter the number of advected scalars");
      sScanF(answer,"%i",&parameters.dbase.get<int >("numberOfAdvectedScalars"));
      printF(" *** Adding %i advected scalars *****\n",parameters.dbase.get<int >("numberOfAdvectedScalars"));
    }
    else if( answer=="add extra variables" )
    {
      gi.inputString(answer,"Enter the number of extra variables");
      sScanF(answer,"%i",&parameters.dbase.get<int >("numberOfExtraVariables"));
      printF(" *** Adding %i extra variables *****\n",parameters.dbase.get<int >("numberOfExtraVariables"));
    }
    else if( answer=="new equation domain..." )
    {
      printF("---------------------------------------------------------------------------------------\n");
      printF("Define a new `equation domain' where a different set of equations are solved.\n");
      printF("  For example, the Navier-Stokes could be solved on one domain (corresponding to some\n"
             "  set of grids) and the heat equation could be solved on a second domain (corresponding\n"
             "  to another set of grids\n"
             "  Note that a `domain' is defined by which equations are solved on it. A single 'domain'\n"
             "  may correspond to multiple disjoint spatial domains\n");
      printF(" A domain can be given a name. One should specify which grids belong to a domain.\n");
      printF(" After exiting this menu, this new domain will become the active domain. Choose the PDE and \n"
             " parameters that you want to use.\n");
      printF("---------------------------------------------------------------------------------------\n");


      // add a new domain
      if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
      {
	ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));
	activeEquationDomain++;
	aString equationDomainName = sPrintF("domain%i",activeEquationDomain);
	equationDomainList.push_back(EquationDomain(&parameters,equationDomainName));
	
      }
      //  EquationDomain & equationDomain = equationDomainList[activeEquationDomain];  // The active domain
 
      // New popup:
      //   Domain Name 
      //   Grids to include (by number or by name with wild-cards)
      // DialogData domainDialog; //  = gui.getDialogSibling();
      GUIState domainDialog; //  = gui.getDialogSibling();
      domainDialog.setWindowTitle("Equation Domain Options");
      domainDialog.setExitCommand("done", "done");

      const int numberOfTextStrings=10;
      aString textLabels[numberOfTextStrings];
      aString textStrings[numberOfTextStrings];

      
//       int nt=0;
//       textLabels[nt] = "domain name:";  sPrintF(textStrings[nt], "%s",(const char*)equationDomainName);  nt++; 
//       textLabels[nt] = "add grid:";  // sPrintF(textStrings[nt], "%s"," (e.g. none, all, square, annulus*)");  nt++; 
//       sPrintF(textStrings[nt], "%s","none");  nt++; 
 
//        // null strings terminal list
//       textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
//       domainDialog.setTextBoxes(textLabels, textLabels, textStrings);


      gi.pushGUI(domainDialog);
      aString answer,line;
      int len=0;
      for(;;) 
      {
	gi.getAnswer(answer,"");      
	if( answer=="done" || answer=="exit" )
	{
	  break;
	}
	else
	{
	  printF("Unknown command = [%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
       
	}

      }
      gi.popGUI();  // pop dialog
      
      if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
      {
	ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));
	aString label = "Active Equation Domain : "+equationDomainList[activeEquationDomain].getName();
	setupDialog.setInfoLabel(0,label);
      }

	// Now go back and choose a PDE etc.
    }
    else if( answer=="surface equations..." )
    {
      surfaceEquationDialog.showSibling();
    }
    else if( answer=="close surface equation options" )
    {
      surfaceEquationDialog.hideSibling();  
    }
    else if( parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")!=NULL &&
             parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")->update(cg,originalBoundaryCondition,gi,answer,&surfaceEquationDialog)==0 )
    {
      // The above call will also assign the initial conditions!
      if( parameters.dbase.get<int >("debug") & 2 ) printF("Answer was found in surfaceEquation.update()\n");
    }
    else if( (len=answer.matches("define real parameter"))     ||
             (len=answer.matches("define integer parameter"))  ||
             (len=answer.matches("define string parameter")) )
    {
      DataBase & pdeParameters = parameters.dbase.get<DataBase>("PdeParameters");

      const int length=answer.length();
      int iStart=len;
      while(  iStart<length && answer[iStart]==' ' ) iStart++;  // skip leading blanks
      int iEnd=iStart;
      while( iEnd<length && answer[iEnd]!=' ' ) iEnd++;       // now look for a blank to end the name
      iEnd--;
      if( iStart<=iEnd )
      {
	aString name = answer(iStart,iEnd);
        if( answer.matches("define real parameter") )
	{
	  real value;
	  sScanF(answer(iEnd+1,answer.length()),"%e",&value);
	  printF(" Adding the real parameter [%s] with value [%e]\n",(const char*)name,value);
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").push_back(ShowFileParameter(name,value));
	  pdeParameters.put<real>((const char*)name,value); // new way

	  if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
            (*parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"))[activeEquationDomain].pdeParameters.push_back(ShowFileParameter(name,value));
	}
	else if( answer.matches("define integer parameter") )
	{
	  int value;
	  sScanF(answer(iEnd+1,answer.length()),"%i",&value);
	  printF(" Adding the integer parameter [%s] with value [%i]\n",(const char*)name,value);
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").push_back(ShowFileParameter(name,value));
          pdeParameters.put<int>((const char*)name,value); // new way

          if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
            (*parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"))[activeEquationDomain].pdeParameters.push_back(ShowFileParameter(name,value));
	}
	else
	{
          iStart=iEnd+1;
	  iEnd=length-1;
	  while( iStart<iEnd && answer[iStart]==' ' ) iStart++;
	  while( iEnd>iStart && answer[iEnd]==' ' ) iEnd--;
          aString value=answer(iStart,iEnd);
	  
	  printF(" Adding the string parameter [%s] with value [%s]\n",(const char*)name,(const char*)value);
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").push_back(ShowFileParameter(name,value));
          pdeParameters.put<aString>((const char*)name,value); // new way

          if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
            (*parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"))[activeEquationDomain].pdeParameters.push_back(ShowFileParameter(name,value));
	}
      }
      else
      {
	printf("ERROR parsing the define parameter statement: answer=[%s]\n",(const char*) answer);
      }
      
      setupDialog.setTextLabel(answer(0,len-1),"<name> <value>");
    }
    else if( setupDialog.getTextValue(answer,"solver name:","%s",name) )
    {
      // removing leading blanks
      int i=0;
      while( i<name.length() && name[i]==' ' ) i++;
      name=name(i,name.length()-1);
    }
    else if( setupDialog.getTextValue(answer,"domain name:","%s",equationDomainName) ) // *old* 
    { // Specify the name of the active EquationDomain
      printF("INFO:Specify the name of the active EquationDomain\n");

      // skip leading blanks:
      int i=0;
      if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
      {
	while( i<equationDomainName.length() && equationDomainName[i]==' ' ) i++;
  	  (*parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"))[activeEquationDomain].setName(equationDomainName(i,equationDomainName.length()-1));
      }
      
    }
    else if( len=answer.matches("add grid:") )
    {
      if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
      {
	ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));

	printF("INFO:add grid to an EquationDomain: examples: 'add grid:all', 'add grid:square', "
	       "'add grid:annulus*'\n");

	aString gridName = answer(len,answer.length()-1);

	EquationDomain & equationDomain = equationDomainList[activeEquationDomain];
	
	if( gridName=="all" )
	{
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    equationDomainList.gridDomainNumberList[grid]=activeEquationDomain;
	  }
	}
	else if( gridName[gridName.length()-1]=='*' )
	{
	  // wild card: final char is a '*'
	  printf("add grid:INFO: looking for a wild card match since the final character is a '*' ...\n");
	  bool found=false;
	  gridName=gridName(0,gridName.length()-2); // remove trailing '*'

	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    // printF(" Check [%s] matches [%s] \n",(const char*)gridName,(const char*)cg[grid].getName());
	    if( cg[grid].getName().matches(gridName) )
	    {

	      equationDomainList.gridDomainNumberList[grid]=activeEquationDomain;

	      printF(" -- (wild card match) Add grid=%i (%s) to equationDomain %s\n",grid,
		     (const char*)cg[grid].getName(),(const char*)equationDomain.getName());
	    
	      found=true;
	    }
	  }
	  if( !found )
	  {
	    printF("add grid:WARNING: No match for the wildcard name [%s*]\n",(const char*)gridName);
	    continue;
	  }
	}
	else
	{
	  bool found=false;
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    if( cg[grid].getName()==gridName )
	    {
	      equationDomainList.gridDomainNumberList[grid]=activeEquationDomain;
	      printF(" -- Add grid=%i (%s) to equationDomain %s\n",grid,
		     (const char*)cg[grid].getName(),(const char*)equationDomain.getName());
	      found=true;
	      break;
	    }
	  }
	  if( !found )
	  {
	    printF("add grid:ERROR looking for the grid named [%s]\n",(const char*)gridName);
	    gi.stopReadingCommandFile();
	    continue;
	  }
	}

	setupDialog.setTextLabel(answer(0,len-1),"none");
      }
      if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
      {
	(*parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"))[activeEquationDomain].setPDE(&parameters);
      }
      
    }
    else if( answer=="solve and move grids" ||
             answer=="move and regenerate grids only" ||
             answer=="move grids only" )
    {
      int & simulateGridMotion = parameters.dbase.get<int>("simulateGridMotion");
      simulateGridMotion = (answer=="solve and move grids" ? 0 :
			    answer=="move and regenerate grids only" ? 1 :
			    answer=="move grids only" ? 2 : 3 );
	printF("Setting simulateGridMotion=%i \n" 
               "  0 = move grid and solve PDE in the normal way,\n"
               "  1 = move grids and generate overlapping grids (Ogen) but do not solve PDE,\n"
               "  2 = move grids but do not generate overlapping grids (Ogen) and do not solve PDE.\n",
	       simulateGridMotion);
    }
    else
    {
	printF("Unknown response: [%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
    }
    
  }
  
  gi.popGUI();  // pop setup

  parameters.dbase.get<bool> ("recomputeDTEveryStep") = 
    pdeVariation==CnsParameters::conservativeGodunov &&
    conservativeGodunovMethod==0;
  parameters.dbase.get<bool> ("timeStepDataIsPrecomputed") =  
    pdeVariation==CnsParameters::conservativeGodunov &&
    conservativeGodunovMethod==0;

  return 0;
}

int Cgcns::
setPlotTitle(const real &t, const real &dt)
{
  const CnsParameters::PDE & pde = parameters.dbase.get<CnsParameters::PDE >("pde");
  const CnsParameters::GodunovVariation & conservativeGodunovMethod = 
                          parameters.dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");
  const CnsParameters::PDEVariation & pdeVariation = 
                 parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation");
  const Parameters::TimeSteppingMethod & timeSteppingMethod = 
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  const real mu = parameters.dbase.get<real >("mu");
  const real kThermal = parameters.dbase.get<real >("kThermal");

  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  aString buff;
  if( pde==CnsParameters::compressibleNavierStokes )
  {
    aString title1,title2;
    title2="";
    if( pdeVariation==CnsParameters::conservativeGodunov )
    {
      if( parameters.dbase.get<int >("numberOfSpecies")==0 )
      {
	if( conservativeGodunovMethod==CnsParameters::multiComponentVersion )
	  title1=sPrintF(buff,"Multicomponent-Euler: t=%6.2e",t);
	else if( conservativeGodunovMethod==CnsParameters::multiFluidVersion )
	  title1=sPrintF(buff,"Multi-Fluid-Euler: t=%6.2e",t);
	else if( mu>0 || kThermal>0 )
          title1=sPrintF(buff,"N-S mu=%7.1e k=%7.1e: t=%6.2e",mu,kThermal,t);
        else 
	  title1=sPrintF(buff,"Euler: t=%6.2e",t);
	title2=sPrintF(buff,"dt=%4.1e",dt);
      }
      else 
      {
	if( conservativeGodunovMethod==CnsParameters::multiComponentVersion )
	  title1=sPrintF(buff,"Multicomponent Reactive Euler: t=%6.2e",t);
	else if( conservativeGodunovMethod==CnsParameters::multiFluidVersion )
	  title1=sPrintF(buff,"Multi-Fluid Euler: t=%6.2e",t);
	else
	  title1=sPrintF(buff,"Reactive Euler (%s), t=%6.2e",(const char*)parameters.dbase.get<aString >("reactionName"),t);
	title2=sPrintF(buff,"  dt=%4.1e",dt);
      }
    }
    else if( timeSteppingMethod==Parameters::steadyStateNewton)
    {
      title1=sPrintF(buff,"Steady-State Compressible NS: iteration=%i",(parameters.dbase.get<int >("globalStepNumber")+1));
      title2=sPrintF(buff,"1/Re=%6.1e, M=%6.2e, Pr=%6.2e",1./parameters.dbase.get<real >("reynoldsNumber"),parameters.dbase.get<real >("machNumber"),parameters.dbase.get<real >("prandtlNumber"));
	
    }	
    else if( parameters.dbase.get<real >("reynoldsNumber")<1.e20 )
    {
      title1=sPrintF(buff,"Compressible NS: t=%6.2e",t);
      title2=sPrintF(buff,"dt=%4.1e, 1/Re=%6.1e",dt,1./parameters.dbase.get<real >("reynoldsNumber"));
    }
    else if( parameters.dbase.get<real >("mu")>0. || parameters.dbase.get<real >("kThermal")>0. )
    {
      title1=sPrintF(buff,"Compressible NS: t=%6.2e",t);
      title2=sPrintF(buff,"  dt=%4.1e, mu=%3.1f, k=%3.1f",dt,parameters.dbase.get<real >("mu"),parameters.dbase.get<real >("kThermal"));
    }
    else
    {
      title1=sPrintF(buff,"Euler: t=%6.2e, dt=%4.1e",t,dt);
    }

    psp.set(GI_TOP_LABEL,title1);
    psp.set(GI_TOP_LABEL_SUB_1,title2);
  }
  else if( pde==CnsParameters::compressibleMultiphase )
  {
    aString title1;
    if( conservativeGodunovMethod==CnsParameters::multiFluidVersion )
     title1=sPrintF(buff,"Compressible Multiphase (multi-fluid) : t=%6.2e",t);
    else
      title1=sPrintF(buff,"Compressible Multiphase: t=%6.2e",t);
    psp.set(GI_TOP_LABEL,title1);
  }

  return 0;
}
