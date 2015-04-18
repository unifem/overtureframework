#include "Cgad.h"
#include "GenericGraphicsInterface.h"
#include "MaterialProperties.h"
#include "Chemkin.h"

#include "EquationDomain.h"
#include "SurfaceEquation.h"


int readRestartFile(GridFunction & cgf, Parameters & parameters,
                    const aString & restartFileName =nullString );



//\begin{>>Cgad.tex}{\subsection{setParametersInteractively}} 
int Cgad::
setupPde(aString & reactionName,bool restartChosen, IntegerArray & originalBoundaryCondition)
//===================================================================================
// /Description:
//    Setup the PDE to be solved
// /Author: WDH
//
//\end{CgadInclude.tex}  
// =======================================================================================================
{
  real cpu0 = getCPU();
  const int buffSize=100;
  char buff[buffSize];

  realCompositeGridFunction & u = gf[current].u;


  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  GUIState setupDialog;

  setupDialog.setWindowTitle("Cgad Setup");
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


  aString tbLabel[] = {"axisymmetric flow with swirl",
		       ""};
  int tbState[5];
  tbState[0] = (int)parameters.dbase.get<bool >("axisymmetricWithSwirl"); 

  int numColumns=1;
  setupDialog.setToggleButtons(tbLabel, tbLabel, tbState, numColumns); 

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

  aString pdeCommands[] =  {"advection diffusion", 
                            "thin film equations",
			    ""     };
  setupDialog.addOptionMenu("pde", pdeCommands, pdeCommands, 0);//(int)parameters.dbase.get<Parameters::PDE >("pde"));

  aString referenceFrameCommands[] =  {"fixed reference frame", 
                                       "rigid body reference frame",
                                       "specified reference frame",
                                       ""     };
  setupDialog.addOptionMenu("reference frame", referenceFrameCommands, referenceFrameCommands, 0);


//   aString reactionCommands[] =  { "no reactions",
// 				  "one step",
// 				  "branching",
// 				  "ignition and growth",
// 				  "ignition and growth desensitization",
// 				  "one equation mixture fraction",
// 				  "two equation mixture fraction and extent of reaction",
// 				  "one step pressure law",
// 				  "specify CHEMKIN reaction",
// 				  ""     };
      
//   setupDialog.addOptionMenu("reaction", reactionCommands, reactionCommands, (int)parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType"));


  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
    
  int & numberOfComponents = parameters.dbase.get<int>("numberOfComponents"); 
  textCommands[nt] = "number of components"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt],"%i",numberOfComponents); nt++; 

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
  // For the AD we set a default
  pdeChosen=true;
  pdeName="advection diffusion";

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
    else if( answer=="advection diffusion" || answer=="convection diffusion" )
    {
      pdeChosen=true;
      //      parameters.dbase.get<Parameters::PDE >("pde")=Parameters::convectionDiffusion;
      pdeName=answer;
    }
    else if( answer=="thin film equations"  )
    {
      pdeChosen=true;
      //      parameters.dbase.get<Parameters::PDE >("pde")=Parameters::convectionDiffusion;
      pdeName="thinFilmEquations";
    }
    else if( answer=="fixed reference frame" ||
             answer=="rigid body reference frame" ||
             answer=="specified reference frame" )
    {
      printF("The frame of reference is needed so that we know how to transform the PDE when the grids move.\n"
	     "Often the PDEs are defined in a fixed reference frame (even if some boundaries are moving).\n" 
	     "If we are solving for a PDE inside a moving rigid body, then the PDE (e.g. the heat equation)\n"
	     "may be defined in the frame of reference of the rigid body.\n");

      Parameters::ReferenceFrameEnum & referenceFrame = 
                    parameters.dbase.get<Parameters::ReferenceFrameEnum>("referenceFrame");
      
      referenceFrame= (answer=="fixed reference frame" ? Parameters::fixedReferenceFrame :
                       answer=="rigid body reference frame" ? Parameters::rigidBodyReferenceFrame :
                       Parameters::specifiedReferenceFrame);
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
      // EquationDomain & equationDomain = equationDomainList[activeEquationDomain];  // The active domain

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
	  if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
            (*parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"))[activeEquationDomain].pdeParameters.push_back(ShowFileParameter(name,value));
	}
	else if( answer.matches("define integer parameter") )
	{
	  int value;
	  sScanF(answer(iEnd+1,answer.length()),"%i",&value);
	  printF(" Adding the integer parameter [%s] with value [%i]\n",(const char*)name,value);
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").push_back(ShowFileParameter(name,value));
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
    else if( setupDialog.getTextValue(answer,"number of components","%u",numberOfComponents) ){} //
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
  }
  
  gi.popGUI();  // pop setup

  return 0;
}

// ================================================================================================================
/// \brief Set the title for plotting the solution.
// ================================================================================================================
int Cgad::
setPlotTitle(const real &t, const real &dt)
{
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  aString buff;

  std::vector<real> & kappa = parameters.dbase.get<std::vector<real> >("kappa");
  std::vector<real> & a = parameters.dbase.get<std::vector<real> >("a");
  std::vector<real> & b = parameters.dbase.get<std::vector<real> >("b");
  std::vector<real> & c = parameters.dbase.get<std::vector<real> >("c");   

  Parameters::TimeSteppingMethod &timeSteppingMethod = 
             parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");
  const Parameters::ImplicitMethod implicitMethod = 
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");  

  const int orderOfAccuracy = parameters.dbase.get<int >("orderOfAccuracy");
  const int orderOfTimeAccuracy  = parameters.dbase.get<int>("orderOfTimeAccuracy");

  aString name="AD";
  if( pdeName=="thinFilmEquations" )
  {
    name="ThinFilm";
  }
  
  if( timeSteppingMethod==Parameters::implicit )
  {
    // if( implicitMethod==Parameters::approximateFactorization )
    // {
    //   name = name + "-AF";
    //   const InsParameters::DiscretizationOptions & discretizationOption =
    // 	parameters.dbase.get<Parameters::DiscretizationOptions>("discretizationOption") = Parameters::compactDifference;
    //   if( discretizationOption==Parameters::compactDifference )
    // 	name = name + "C";
    //   else
    // 	name = name + "S"; // standard finite difference
    // }
    if( implicitMethod==Parameters::backwardDifferentiationFormula )
    {
      const int & orderOfBDF = parameters.dbase.get<int>("orderOfBDF");      
      name = name + sPrintF("-BDF%i",orderOfBDF);
    }
    else
      name = name + "-IM";
  }
  else if( timeSteppingMethod==Parameters::adamsBashforth2 || 
	   timeSteppingMethod==Parameters::adamsPredictorCorrector2 || 
	   timeSteppingMethod==Parameters::adamsPredictorCorrector4 || 
	   timeSteppingMethod==Parameters::variableTimeStepAdamsPredictorCorrector )
  {
    name = name + sPrintF("-PC%i%i",orderOfTimeAccuracy,orderOfAccuracy);
  }
  

  psp.set(GI_TOP_LABEL,sPrintF(buff,"%s: t=%6.2e,",(const char*)name,t));
  // psp.set(GI_TOP_LABEL,sPrintF(buff,"%s: t=%6.2e,",(const char*)name,orderOfAccuracyInTime,orderOfAccuracy,t));

  // psp.set(GI_TOP_LABEL,sPrintF(buff,"convectionDiffusion: t=%6.2e ",t));
  if( parameters.dbase.get<int>("numberOfDimensions")==2 )
    psp.set(GI_TOP_LABEL_SUB_1,sPrintF(buff,"a=%4.1g, b=%4.1g, kappa=%6.1g, dt=%4.1g",
				       a[0],b[0],kappa[0],dt));
  else
    psp.set(GI_TOP_LABEL_SUB_1,sPrintF(buff,"a=%4.1g, b=%4.1g, c=%4.1g, kappa=%6.1g, dt=%4.1g",
				       a[0],b[0],c[0],kappa[0],dt));
  
  return 0;
}
