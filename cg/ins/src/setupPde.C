#include "Cgins.h"
#include "GenericGraphicsInterface.h"
#include "viscoPlasticMacrosCpp.h"
#include "EquationDomain.h"
#include "SurfaceEquation.h"
#include "GridMaterialProperties.h"

int readRestartFile(GridFunction & cgf, Parameters & parameters,
                    const aString & restartFileName =nullString );



// ===================================================================================================================
/// \brief Setup the PDE to be solved.
/// \param reactionName (input) : 
/// \param restartChosen (input) : 
/// \param originalBoundaryCondition (input) : 
///
// ===================================================================================================================
int Cgins::
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

  setupDialog.setWindowTitle("Cgins Setup");
  setupDialog.setExitCommand("continue", "continue");

  aString answer, turbulenceModel="";

  aString equationDomainName="domain0";
  aString label = "Active Equation Domain : "+equationDomainName;
  setupDialog.addInfoLabel(label); 

  DialogData & surfaceEquationDialog = setupDialog.getDialogSibling();
  surfaceEquationDialog.setWindowTitle("Surface Eqyation Options");
  surfaceEquationDialog.setExitCommand("close surface equation options", "close");
  if( parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")!=NULL )
    parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")->update(cg,originalBoundaryCondition,gi,"build dialog",&surfaceEquationDialog);


//   aString tbLabel[] = {"axisymmetric flow with swirl",
// 		       ""};
//   int tbState[5];
//   tbState[0] = (int)parameters.dbase.get<bool >("axisymmetricWithSwirl"); 

//   int numColumns=1;
//   setupDialog.setToggleButtons(tbLabel, tbLabel, tbState, numColumns); 

  // push buttons
  aString pbCommands[] = {"choose a grid",
			  "read a restart file",
			  "passive scalar advection",
			  "add extra variables",
			  "new equation domain...", 
			  "surface equations...",
			  ""};

  const int numRows=3;
  setupDialog.setPushButtons( pbCommands, pbCommands, numRows ); 


  setupDialog.setOptionMenuColumns(1);

  aString pdeCommands[] =  {"incompressible Navier Stokes",
			    ""     };
  setupDialog.addOptionMenu("pde", pdeCommands, pdeCommands, 0);

  aString pdeModelCommands[] = {"standard model",
			        "Boussinesq model",
			        "visco-plastic model",
			        "two-phase flow model",
			        ""     };
  setupDialog.addOptionMenu("model", pdeModelCommands, pdeModelCommands, (int)parameters.dbase.get<InsParameters::PDEModel >("pdeModel"));

//   aString reactionCommands[] =  { "no reactions",
// 				  ""     };
      
//   setupDialog.addOptionMenu("reaction", reactionCommands, reactionCommands, (int)parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType"));


  aString tmCommands[] =  { "noTurbulenceModel",
			    "Baldwin-Lomax",
			    "k-epsilon",
			    "k-omega",
			    "SpalartAllmaras",
                            "LargeEddySimulation",
			    ""     };
      
  setupDialog.addOptionMenu("turbulence model", tmCommands, tmCommands, (int)parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"));

//   aString eosCommands[] =  {"ideal gas law",
//                             "user defined equation of state",
// 			    ""     };
//   setupDialog.addOptionMenu("equation of state", eosCommands, eosCommands, (int)parameters.dbase.get<Parameters::EquationOfStateEnum >("equationOfState"));


  aString mvCommands[] =  { "solve and move grids",
			    "move and regenerate grids only",
			    "move grids only",
			    ""     };
      
  setupDialog.addOptionMenu("motion option:", mvCommands, mvCommands, parameters.dbase.get<int >("simulateGridMotion"));


  // --- Toggle buttons ---
  aString tbLabel[] = {"variable material properties",
 		       ""};
  int tbState[5];
  tbState[0] = (int)parameters.dbase.get<int>("variableMaterialPropertiesOption");

  int numColumns=1;
  setupDialog.setToggleButtons(tbLabel, tbLabel, tbState, numColumns); 

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
    
  bool pdeChosen=false;

  // For the INS we set a default
  pdeChosen=true;
  pdeName="incompressible Navier Stokes";


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
    else if( answer=="incompressible Navier Stokes" )
    {
      pdeChosen=true;
      pdeName=answer;
    }
    else if( answer=="standard model" || 
	     answer=="Boussinesq model" ||
	     answer=="visco-plastic model"||
	     answer=="two-phase flow model" )
    {
      if( answer=="standard model" )
        parameters.dbase.get<InsParameters::PDEModel >("pdeModel")=InsParameters::standardModel;
      else if( answer=="Boussinesq model" )
        parameters.dbase.get<InsParameters::PDEModel >("pdeModel")=InsParameters::BoussinesqModel;
      else if( answer=="visco-plastic model" )
        parameters.dbase.get<InsParameters::PDEModel >("pdeModel")=InsParameters::viscoPlasticModel;
      else if( answer=="two-phase flow model" )
        parameters.dbase.get<InsParameters::PDEModel >("pdeModel")=InsParameters::twoPhaseFlowModel;
      else
      {
	Overture::abort("ERROR: unexpected option, this should not happen.");
      }
      setupDialog.getOptionMenu("model").setCurrentChoice(answer);
    }
    else if( answer=="k-epsilon" || answer=="k-omega" || answer=="SpalartAllmaras" || answer=="Baldwin-Lomax" ||
             answer=="LargeEddySimulation" )
    {
      if( answer=="k-epsilon" )
        parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")=Parameters::kEpsilon;
      else if( answer=="k-omega" )
        parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")=Parameters::kOmega;
      else if ( answer=="Baldwin-Lomax" )
	parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")=Parameters::BaldwinLomax;
      else if( answer=="SpalartAllmaras" )
        parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")=Parameters::SpalartAllmaras;
      else if( answer=="LargeEddySimulation" )
        parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")=Parameters::LargeEddySimulation;
      else
      {
        OV_ABORT("ERROR: unexpected option, this should not happen.");
      }
      setupDialog.getOptionMenu("turbulence model").setCurrentChoice(answer);
    }
//     else if( answer=="ideal gas law" )
//     {
//       parameters.dbase.get<Parameters::EquationOfStateEnum >("equationOfState")=Parameters::idealGasEOS;
//     }
//     else if( answer=="user defined equation of state" )
//     {
//       parameters.dbase.get<Parameters::EquationOfStateEnum >("equationOfState")=Parameters::userDefinedEOS;
//     }
    else if( setupDialog.getToggleValue(answer,"variable material properties",
				   parameters.dbase.get<int>("variableMaterialPropertiesOption")) )
    {
      if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
      {
        printF("Cgins:INFO: turn ON variable material properties.\n");
	// By default the material properties vary from grid point to grid point (e.g. for TZ)
	parameters.dbase.get<int>("variableMaterialPropertiesOption")=GridMaterialProperties::variableMaterialProperties;
      }
      else
      {
        printF("Cgins:INFO: turn OFF variable material properties.\n");
        parameters.dbase.get<int>("variableMaterialPropertiesOption")=GridMaterialProperties::constantMaterialProperties;
      }
      
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

          // new way:
          pdeParameters.put<real>((const char*)name,value);
	  
	}
	else if( answer.matches("define integer parameter") )
	{
	  int value;
	  sScanF(answer(iEnd+1,answer.length()),"%i",&value);
	  printF(" Adding the integer parameter [%s] with value [%i]\n",(const char*)name,value);
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").push_back(ShowFileParameter(name,value));
          // new way:
          pdeParameters.put<int>((const char*)name,value);
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

          // new way:
          pdeParameters.put<aString>((const char*)name,value);
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
    else if( setupDialog.getTextValue(answer,"domain name:","%s",equationDomainName) ) // ** old *** 
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
      else
      {
	printF("Unknown response: [%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
      if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
      {
	(*parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"))[activeEquationDomain].setPDE(&parameters);
      }
    }
    else if( answer=="passive scalar advection" )
    {
      parameters.dbase.get<bool>("advectPassiveScalar")=true;
    }
//     else if( setupDialog.getToggleValue(answer,"simulate grid motion only",
// 				   parameters.dbase.get<bool>("simulateGridMotionOnly")) )
//     {
//       if( parameters.dbase.get<bool>("simulateGridMotionOnly") )
// 	printF("INFO: simulate grid motion only is ON : move grids but solve no equations,\n"
//                "      use dtMax as the time step.\n");
//     }
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

  if  ( !parameters.dbase.has_key("pdeNameModifier") ) 
    parameters.dbase.put<aString>("pdeNameModifier",InsParameters::PDEModelName[(int)parameters.dbase.get<InsParameters::PDEModel >("pdeModel")]) ;
  else
    parameters.dbase.get<aString>("pdeNameModifier") = InsParameters::PDEModelName[(int)parameters.dbase.get<InsParameters::PDEModel >("pdeModel")] ;

  return 0;
}

// ===================================================================================================================
/// \brief Set the plot titles for interactive plotting.
/// \param t (input) : current time
/// \param dt (input) : current time step
///
// ===================================================================================================================
int Cgins::
setPlotTitle(const real &t, const real &dt)
{
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
  const Parameters::TurbulenceModel & turbulenceModel = 
    parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  const int orderOfAccuracy = parameters.dbase.get<int >("orderOfAccuracy");
  const int orderOfTimeAccuracy  = parameters.dbase.get<int>("orderOfTimeAccuracy");

  Parameters::TimeSteppingMethod &timeSteppingMethod = 
             parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");
  const Parameters::ImplicitMethod implicitMethod = 
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");  

  aString name="INS";
  if( timeSteppingMethod==Parameters::implicit )
  {
    if( implicitMethod==Parameters::approximateFactorization )
    {
      name = name + "-AF";
      const InsParameters::DiscretizationOptions & discretizationOption =
	parameters.dbase.get<InsParameters::DiscretizationOptions>("discretizationOption") = InsParameters::compactDifference;
      if( discretizationOption==InsParameters::compactDifference )
	name = name + "C";
      else
	name = name + "S"; // standard finite difference
    }
    else
      name = name + "-IM";

  }
  

  aString buff;
  if( !parameters.isSteadyStateSolver() )
    psp.set(GI_TOP_LABEL,sPrintF(buff,"%s%i%i: t=%6.2e,",(const char*)name,orderOfTimeAccuracy,orderOfAccuracy,t));
  else
    psp.set(GI_TOP_LABEL,sPrintF(buff,"%s%i: it=%i,",(const char*)name,orderOfAccuracy,parameters.dbase.get<int >("globalStepNumber")+1));
  aString label2;

  if( pdeModel==InsParameters::viscoPlasticModel )
    label2+="VP: ";
  else if( pdeModel!=InsParameters::standardModel )
    label2+=InsParameters::PDEModelName[(int)pdeModel]+" ";
      
  if( turbulenceModel!=Parameters::noTurbulenceModel )
  {
    label2+= turbulenceModel==Parameters::BaldwinLomax ? "TM=BL, " :
      turbulenceModel==Parameters::kEpsilon     ? "TM=KE, " :
      turbulenceModel==Parameters::kOmega       ? "TM=KW, " :
      turbulenceModel==Parameters::SpalartAllmaras ? "TM=SA, " : 
      turbulenceModel==Parameters::LargeEddySimulation ? "TM=LES, " : "TM=??, ";
  }
  if( parameters.isAxisymmetric() )
    label2+=sPrintF(buff,"axisymmetric, ");

  if( pdeModel==InsParameters::viscoPlasticModel )
  {
    // declare and lookup visco-plastic parameters (macro)
    declareViscoPlasticParameters;

    // real nuViscoPlastic=1., etaViscoPlastic=1., yieldStressViscoPlastic=10., exponentViscoPlastic=10.; 

    label2+=sPrintF(buff,"dt=%4.1e, (eta,yield,exp)=(%.4g,%.4g,%.4g)",dt,etaViscoPlastic,yieldStressViscoPlastic,
                    exponentViscoPlastic);
    
  }
  else
  {
    label2+=sPrintF(buff,"dt=%4.1e, nu=%6.1e",dt,parameters.dbase.get<real >("nu"));
  }
  
  if( parameters.dbase.get<int >("tc")>=0 )
  {
    label2+=sPrintF(buff,", k=%6.1e",parameters.dbase.get<real >("kThermal"));
  }
      
  if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
    label2+=sPrintF(buff,", ad2=(%3.1f,%3.1f)",parameters.dbase.get<real >("ad21"),parameters.dbase.get<real >("ad22"));
  // label2+=sPrintF(buff,", ad21=%3.1f ad22=%3.1f",parameters.dbase.get<real >("ad21"),parameters.dbase.get<real >("ad22"));
  if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
    label2+=sPrintF(buff,", ad4=(%3.1f,%3.1f)",parameters.dbase.get<real >("ad41"),parameters.dbase.get<real >("ad42"));
  // label2+=sPrintF(buff,", ad41=%3.1f ad42=%3.1f",parameters.dbase.get<real >("ad41"),parameters.dbase.get<real >("ad42"));
      
  psp.set(GI_TOP_LABEL_SUB_1,(const char*)label2);
  if( turbulenceModel==Parameters::SpalartAllmaras )
  {
    aString label3;
    label3=sPrintF(buff," ad21n=%3.1f ad22n=%3.1f",parameters.dbase.get<real >("ad21n"),parameters.dbase.get<real >("ad22n"));
    psp.set(GI_TOP_LABEL_SUB_2,(const char*)label3);
  }
      
  return 0;
}
