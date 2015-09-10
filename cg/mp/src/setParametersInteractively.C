// overture includes
#include "Cgins.h"
#include "Cgasf.h"
#include "Cgad.h"
#include "GenericGraphicsInterface.h"
#include "ArraySimple.h"
#include <strstream>

#include "Cgmp.h"
#include "MpParameters.h"

using namespace std;

// ===================================================================================================================
/// \brief Setup the solver and parameters for a given domain.
/// \param domain (input) : domain number to setup.
/// \param modelNames (input) : names of available models (PDE solvers).
///
// ===================================================================================================================
int Cgmp::
setupDomainSolverParameters( int domain, std::vector<aString> & modelNames )
{
  GenericGraphicsInterface &gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  GUIState setupDialog;

  aString buff;
  setupDialog.setWindowTitle(sPrintF(buff,"Set %s parameters",(const char*)cg.getDomainName(domain)));
  setupDialog.setExitCommand("done", "done");
    
  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  aString solverName = cg.getDomainName(domain); // default solver name 
  if( domainSolver[domain]!=NULL ) solverName=domainSolver[domain]->getName();
  textCommands[nt] = "solver name";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)solverName);  nt++; 

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  setupDialog.setTextBoxes(textCommands, textLabels, textStrings);

  const int nModels = modelNames.size();
  aString radioTitle = "Select "+cg.getDomainName(domain)+" Solver";
  aString *radioLbl = new aString[nModels+2];
  aString *radioCmd = new aString[nModels+2];
  radioLbl[0] = "none";
  radioCmd[0] = "set solver none";  // check this 
  for ( int i=0; i<nModels; i++ )
  {
    radioLbl[i+1] = modelNames[i];
    radioCmd[i+1] = "set solver "+radioLbl[i+1];
  }
  radioLbl[nModels+1]="";  radioCmd[nModels+1]=""; 
  setupDialog.addRadioBox(radioTitle,radioCmd,radioLbl,0,nModels);
  
  delete [] radioLbl;
  delete [] radioCmd;

  ArraySimple<aString> pbCommands(2);
  pbCommands[0]="solver parameters";
  pbCommands[1]="";
  const int numRows = 1;
  setupDialog.setPushButtons( pbCommands.ptr(), pbCommands.ptr(), numRows ); 

  gi.pushGUI(setupDialog);
  aString answer,line;
  int len=0;
  for(;;) 
  {
    gi.getAnswer(answer,"");      
    if( answer=="done" )
    {
      break;
    }
    else if ( (len=answer.matches("set solver")) )
    {
      aString solverType;
      aString s = substring(answer,len+1,answer.length()-1);
      //	  sScanF(s.c_str(),"%s %s",dName, solverType);
      istrstream is(s);
      is>>solverType;

      if( domainSolver[domain] ) 
      {
	delete domainSolver[domain];
	domainSolver[domain] = 0;
      }
      CompositeGrid &cgd = cg.domain[domain];
      if( solverType!="none" )
      {
        const int &plotOption = parameters.dbase.get<int>("plotOption");	
        domainSolver[domain] = buildModel(solverType,cg.domain[domain],&gi,
                                          parameters.dbase.get<Ogshow*>("show"),plotOption);
	// Note: the name of debug files are based on the solverName.
	domainSolver[domain]->setName(solverName);

	// Set the grid file name for all domains to be the same file as that supplied to Cgmp
	domainSolver[domain]->setNameOfGridFile(parameters.dbase.get<aString>("nameOfGridFile"));
      }
    }
    else if( answer=="solver parameters" )
    {
      if ( domainSolver[domain] )
      {
	// Assign solver parameters:
	domainSolver[domain]->setParametersInteractively(false);
      }
  
      else
      {
	gi.outputString("a domain solver has not been chosen yet");
      }
    }
    else if( setupDialog.getTextValue(answer,"solver name","%s",solverName) )
    {
      // printF("answer=[%s]\n",(const char*)answer);
      // removing leading blanks
      int i=0;
      while( i<solverName.length() && solverName[i]==' ' ) i++;
      solverName=solverName(i,solverName.length()-1);
	// Note: the name of debug files are based on the solverName.
      if( domainSolver[domain] )
	domainSolver[domain]->setName(solverName);
    }
    else
    {
      printF("Unknown command = [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
       
    }

  }
  gi.popGUI();  // pop dialog


  return 0;
}


// ===================================================================================================================
/// \brief Setup the solvers and parameters for different domains.
/// \param callSetup (input) : if true call setup. 
///
// ===================================================================================================================
int
Cgmp::
setParametersInteractively(bool callSetup)
{
  //  KK::AssertAlways<CgTH_Err>(parameters.dbase.get<GenericGraphicsInterface*>("ps"),
  //  			     "interactive interface not initialized for Cgmp");
  
  const int &plotOption = parameters.dbase.get<int>("plotOption");
    
  GenericGraphicsInterface &gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters &gp = parameters.dbase.get<GraphicsParameters >("psp");

  int nDomains = cg.numberOfDomains();
  //  INFO_LOG(className<<" : number of domains = "<<nDomains);
  //  for ( int d=0; d<nDomains; d++ )
  //    {
  //      INFO_LOG("    domain "<<d<<" name : "<<cg.getDomainName(d));
  //    }

  interpolant = new Interpolant[nDomains];

  
  // build interpolants for each domain
  for( int d=0; d<nDomains; d++ )
  {
    CompositeGrid & cgd = cg.domain[d];
    // Interpolant & interp = *new Interpolant(cgd);
    interpolant[d].incrementReferenceCount();
    interpolant[d].updateToMatchGrid(cgd);
    if( cgd.numberOfDimensions()==3 )
    {
      // interpolant[d].setImplicitInterpolationMethod(Interpolant::directSolve);
      interpolant[d].setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
      // parameters.maximumNumberOfIterationsForImplicitInterpolation=20;
    }
    
  }

  GUIState gui;
  gui.setWindowTitle("Cgmp Setup");
  gui.setExitCommand("continue","Continue");

  // --- get a list of the available PDE models ---
  std::vector<aString> modelNames;
  getModelInfo(modelNames);
  const int nModels = modelNames.size();

  aString buff;
  ArraySimple<aString> pbCommands(nDomains+2);
  pbCommands[0]="setup general parameters";
  for( int d=0; d<nDomains; d++ )
  {
    if ( cg.getDomainName(d).length()==0 )
    {
      sPrintF(buff,"domain %i",d);
      cg.setDomainName(d,buff);
    }

    pbCommands[d+1] = "setup "+cg.getDomainName(d);
  }
  pbCommands[nDomains+1]="";
  // Number of columns: 
  const int numColumns=nDomains<5  ? 1 : nDomains<10 ? 2 : 3;
  const int numRows = (nDomains+numColumns)/numColumns;  
  gui.setPushButtons( pbCommands.ptr(), pbCommands.ptr(), numRows ); 

  // set the label field with names of the domains and corresponding pde model
  for( int d=0; d<nDomains; d++ )
  {
    aString modelLabel=sPrintF(buff,"%s : %s ",(const char*)cg.getDomainName(d),
		       (domainSolver[d] ? (const char*)domainSolver[d]->getClassName() : "none"));
    gui.addInfoLabel(modelLabel);
  }

  aString tbLabels[] = {"use preferred order of domains",
                        "match interfaces geometrically",
			""};
  int tbState[10];
  tbState[0] = parameters.dbase.get<bool>("usePreferredOrderOfDomains");
  tbState[1] = parameters.dbase.get<bool>("matchInterfacesGeometrically");
  const int numToggleColumns=1;
  gui.setToggleButtons(tbLabels, tbLabels, tbState, numToggleColumns); 


  gi.pushGUI(gui);
  aString answer="";
  while ( true )
  {
    int len;

    gi.getAnswer(answer,"");

    if (answer=="continue")
      break;
    else if ( (len=answer.matches("setup")) )
    {
      aString dName = substring(answer,len+1,answer.length()-1);
      //	  DEBUG_LOG1("setting up parameters for "<<dName);
      int domain=-1;
      for( int d=0; d<nDomains; d++ )
      {
	if( cg.getDomainName(d)==dName )
	{
	  domain=d;
	  break;
	}
      }
      if( domain>=0 )
      {
	// domainSetupDialog[domain]->showSibling();
        setupDomainSolverParameters( domain,modelNames );
      }
      else
      {
        printF("ERROR: unknown setup command [%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
      // set the label field with names of the domains and corresponding pde model
      for( int d=0; d<nDomains; d++ )
      {
	aString modelLabel=sPrintF(buff,"%s : %s ",(const char*)cg.getDomainName(d),
			   (domainSolver[d] ? (const char*)domainSolver[d]->getClassName() : "none"));
	gui.setInfoLabel(d,modelLabel);
      }
    }
    else if( answer=="setup general parameters" )
    {
      printF("INFO: There are currently no general parameters to set\n");
    }
    else if( gui.getToggleValue(answer,"use preferred order of domains",
                                   parameters.dbase.get<bool>("usePreferredOrderOfDomains")) ){} //
    else if( gui.getToggleValue(answer,"match interfaces geometrically",
                                   parameters.dbase.get<bool>("matchInterfacesGeometrically")) ){} //
    else if( (len=answer.matches("define real parameter"))     ||
	     (len=answer.matches("define integer parameter"))  ||
	     (len=answer.matches("define string parameter")) )
    {
	  
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
	}
	else if( answer.matches("define integer parameter") )
	{
	  int value;
	  sScanF(answer(iEnd+1,answer.length()),"%i",&value);
	  printF(" Adding the integer parameter [%s] with value [%i]\n",(const char*)name,value);
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").push_back(ShowFileParameter(name,value));
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
	}
      }
    }
  }

  gi.popGUI();  
  


  // **** Initialize the interfaces here so that they are known for changing interface parameters ****
  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  if( interfaceList.size()==0 )
  {
    // -- Initialize the list of interfaces --
    const int numberOfDomains = domainSolver.size();
    std::vector<int> gfIndex(numberOfDomains,current); 
    ForDomain(d)
    {
      gfIndex[d]=domainSolver[d]->current;  // is this right ? 
    }

    initializeInterfaces(gfIndex);

    if( interfaceList.size()>0 ) gridHasMaterialInterfaces=true;

    if( interfaceList.size()>0 )
    {
      printF("***************************************************************\n"
             "**** Cgmp::initialize interfaces: number of interfaces =%i\n"
             "***************************************************************\n" 
              ,interfaceList.size());
    }
    else
    {
      printF("***************************************************************\n"
             "**** Cgmp::initialize interfaces: There are NO interfaces. ****\n"
             "***************************************************************\n" );
    }
    
  }

  // Look for an initial time that is different from zero, this may occur on a restart
  real & tInitial = parameters.dbase.get<real>("tInitial");
  tInitial=0.;
  ForDomain(d)
  {
    real t0 = domainSolver[d]->parameters.dbase.get<real>("tInitial");
    if( t0!=tInitial )
    {
      if( tInitial==0. )
      {
	tInitial=t0;
        printF("INFO: taking initial time =%9.3e from domain d=%i\n",tInitial,d);
      }
      else
      {
        printF("WARNING: domain d=%i has an initial time =%9.3e which is not the same! \n"
               " I will still keep the initial time as %9.e3\n",t0,tInitial);
      }
    }
    
  }

  if( parameters.dbase.get<bool>("usePreferredOrderOfDomains") )
  {
    std::vector<int> & domainOrder = parameters.dbase.get<std::vector<int> >("domainOrder");
    // For fluid structure problems we should advance the solid domains first
    int domain=0;
    for( int d=0; d<domainSolver.size(); d++ )
    {
      if( domainSolver[d]!=NULL && domainSolver[d]->getClassName()=="Cgsm" )   // *** fix me : need a better way to check for solids
      {
	domainOrder[d]=domain;
	domain++;
      }
    }
    for( int d=0; d<domainSolver.size(); d++ )
    {
      if( domainSolver[d]==NULL || (domainSolver[d]!=NULL && domainSolver[d]->getClassName()!="Cgsm") )
      {
	domainOrder[d]=domain;
	domain++;
      }
    }
    if( true || debug() & 1 )
    {
      printF("Cgmp:INFO: The preferred domain order is ");
      for( int d=0; d<domainSolver.size(); d++ )
	printF("%i ",domainOrder[d]);
      printF("\n");
      printF("Cgmp:INFO: The domains are re-ordered to put solid domains before fluid domains\n.");
      printF("Cgmp:INFO: To use the default ordering turn off the toggle `use preferred order of domains'\n");
      printF("Cgmp:INFO: You can also specify a different domain ordering directly when setting Cgmp parameters.\n");
    }
  }
  
  const real dtMaxDefault = parameters.dbase.get<real >("dtMax"); // remember initial value
  // *****************************************************************************
  // **** Get run time parameters for Cgmp (timeStepping method, cfl, etc.)   **** 
  // **** this will call setup if requested                                   ****
  // *****************************************************************************
  DomainSolver::setParametersInteractively(callSetup);

  if( parameters.dbase.get<real >("dtMax")!=dtMaxDefault )
  {
    printF("Cgmp: Setting dtMax=%8.2e for all domains\n",parameters.dbase.get<real >("dtMax"));
    ForDomain(d)
    {
      domainSolver[d]->parameters.dbase.get<real>("dtMax")=parameters.dbase.get<real >("dtMax");
    }
	  
  }

}

// ===================================================================================================================
/// \brief Setup routine.
/// \param time (input) : current time.
///
// ===================================================================================================================
void
Cgmp::
setup(const real & time)
{
  // // // code copied from DomainSolver::setup starts here
  real cpu0 = getCPU();
  const int buffSize=100;
  char buff[buffSize];

  // printF("********* Cgmp: setup: time=%8.2e tInitial=%8.2e\n",time,parameters.dbase.get<real>("tInitial"));

  int grid,axis,side;
  const int numberOfDimensions = cg.numberOfDimensions();
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
  Parameters::TimeSteppingMethod & timeSteppingMethod= parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  realCompositeGridFunction & u = gf[current].u;

  gf[current].t=time;  // *wdh* 080814 

  // setup the twlightzone function:
  if( parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")==Parameters::noInitialConditionChosen )
    parameters.setTwilightZoneFunction(parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"),parameters.dbase.get<int >("tzDegreeSpace"),parameters.dbase.get<int >("tzDegreeTime"));
  
  
  parameters.updatePDEparameters();
  
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflow )
	  parameters.dbase.get<IntegerArray>("variableBoundaryData")(grid)=TRUE;  // **** why can't this be in setBoundaryConditions ?
	  
	// **** set the data to default for any BC's that were not set 
	/* -------
	   if( bc=cg[grid].boundaryCondition()(side,axis) > 0 )
	   {
	   for( n=0; n<parameters.dbase.get<int >("numberOfComponents"); n++ )
	   {
	   if( bcData(n,side,axis,grid)==defaultValue )
	   {
	   }
	   }
	   }
	   ------ */
      }
    }
  }
  // // // code copied from DomainSolver::setup ends here


  // --- call the setup routine for all the active domains now ---
  int nDomains = cg.numberOfDomains();
  for ( int d=0; d<nDomains; d++ )
  {
    if ( domainSolver[d] )
    {
      // assign this parameter for projecting the interface values:
      domainSolver[d]->parameters.dbase.get<bool>("projectInterface") = parameters.dbase.get<bool>("projectInterface");

      domainSolver[d]->setup(time);
    }
  }
  
  MpParameters::MultiDomainAlgorithmEnum multiDomainAlgorithm=
                 parameters.dbase.get<MpParameters::MultiDomainAlgorithmEnum>("multiDomainAlgorithm");
  
  // output header info for Cgmp
  for( int output=0; output<=1; output++ )
  {
    if( parameters.dbase.get<int >("myid")!=0 ) continue;

    FILE *file = output==0 ? stdout : parameters.dbase.get<FILE* >("logFile");

    
    fprintf(file,"\n"
	    "******************************************************************\n");
    fprintf(file,
	    "             %s Version 0.1                                 \n"
	    "             -----------------                              \n",
	    (const char*)getClassName()   );
    

    fprintf(file,"\n"
	    " cfl = %f, tFinal=%e, tPrint = %e \n"
	    " Time stepping method: %s.\n"
	    " Solve coupled interface equations = %i.\n"
            " Use %s interface transfer.\n"
            " Relax correction steps = %i.\n"
	    " Multi-domain algorithm = %s.\n"
            " Project interface = %s. (interfaceProjectionOption=%i, interface-ghost=%s)\n"
	    ,
	    parameters.dbase.get<real >("cfl"),
	    parameters.dbase.get<real >("tFinal"),
	    parameters.dbase.get<real >("tPrint"),
	    (const char*)Parameters::timeSteppingName[timeSteppingMethod],
            (int)parameters.dbase.get<bool>("solveCoupledInterfaceEquations"),
	    (parameters.dbase.get<bool>("useNewInterfaceTransfer") ? "new" : "old"),
	    (int)parameters.dbase.get<bool>("relaxCorrectionSteps"),
            (multiDomainAlgorithm==MpParameters::defaultMultiDomainAlgorithm ? "default" :
             multiDomainAlgorithm== MpParameters::stepAllThenMatchMultiDomainAlgorithm ? "step all then match" : 
             "unknown"),
            (parameters.dbase.get<bool>("projectInterface")? "true" : "false"),
            parameters.dbase.get<int>("interfaceProjectionOption"),
            (parameters.dbase.get<int>("interfaceProjectionGhostOption")==0 ? "extrapolate" : 
             parameters.dbase.get<int>("interfaceProjectionGhostOption")==1 ? "compatibility" : 
             parameters.dbase.get<int>("interfaceProjectionGhostOption")==2 ? "exact" : "domain BC" )
             );


    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      fprintf(file," Twilight zone flow\n");
    }

    fprintf(file,"******************************************************************\n\n");
  }
  
  real cpu1=getCPU();
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("totalTime"))+=cpu1-cpu0;
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInitialize"))=cpu1-cpu0;

  return;
}



