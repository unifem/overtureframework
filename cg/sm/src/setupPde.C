#include "Cgsm.h"
#include "SmParameters.h"
#include "GenericGraphicsInterface.h"
#include "GridMaterialProperties.h"

//#include "EquationDomain.h"
// #include "SurfaceEquation.h"


int readRestartFile(GridFunction & cgf, Parameters & parameters,
                    const aString & restartFileName =nullString );



//===================================================================================
/// \brief Setup the PDE to be solved.
/// \details This function is called at the very start in order to setup the equations
///   to be solved etc. 
//===================================================================================
int Cgsm::
setupPde(aString & reactionName, bool restartChosen, IntegerArray & originalBoundaryCondition)
{
  real cpu0 = getCPU();

  SmParameters::PDEModel & pdeModel = parameters.dbase.get<SmParameters::PDEModel>("pdeModel");
  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

  const int buffSize=100;
  char buff[buffSize];

  realCompositeGridFunction & u = gf[current].u;

  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  GUIState setupDialog;

  setupDialog.setWindowTitle("Cgsm Setup");
  setupDialog.setExitCommand("continue", "continue");

  aString answer;

//   aString equationDomainName="domain0";
//   aString label = "Active Equation Domain : "+equationDomainName;
//   setupDialog.addInfoLabel(label); 

//   DialogData & surfaceEquationDialog = setupDialog.getDialogSibling();
//   surfaceEquationDialog.setWindowTitle("Surface Eqyation Options");
//   surfaceEquationDialog.setExitCommand("close surface equation options", "close");
//   if( parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")!=NULL )
//     parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")->update(cg,originalBoundaryCondition,gi,"build dialog",&surfaceEquationDialog);


   aString tbLabel[] = {"variable material properties",
 		       ""};
   int tbState[5];
   tbState[0] = (int)parameters.dbase.get<int>("variableMaterialPropertiesOption");

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

  aString *pdeCommands = SmParameters::PDEModelName;
  setupDialog.addOptionMenu("pde", pdeCommands, pdeCommands, (int)pdeModel);

  aString *pdeVariationCommands = SmParameters::PDEVariationName;
  setupDialog.addOptionMenu("pde variation", pdeVariationCommands, pdeVariationCommands, (int)pdeVariation);

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
    
//   int & numberOfComponents = parameters.dbase.get<int>("numberOfComponents"); 
//   textCommands[nt] = "number of components"; textLabels[nt]=textCommands[nt];
//   sPrintF(textStrings[nt],"%i",numberOfComponents); nt++; 

  textCommands[nt] = "define real parameter"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "<name> <value>"); nt++; 

  textCommands[nt] = "define integer parameter"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "<name> <value>"); nt++; 

  textCommands[nt] = "define string parameter"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "<name> <value>"); nt++; 

  textCommands[nt] = "solver name:";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)name);  nt++; 

//   textCommands[nt] = "domain name:";  textLabels[nt]=textCommands[nt];
//   sPrintF(textStrings[nt], "%s",(const char*)equationDomainName);  nt++; 

//   textCommands[nt] = "add grid:";   textLabels[nt]=textCommands[nt];
//   sPrintF(textStrings[nt], "%s","none");  nt++; 

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  setupDialog.setTextBoxes(textCommands, textLabels, textStrings);


  gi.pushGUI(setupDialog);
    

  bool pdeChosen=false;
  // Set defaults
  pdeChosen=true;
  pdeName=SmParameters::PDEModelName[pdeModel];
  parameters.pdeName = "solidMechanics";
  
  bool gridChosen=cg.numberOfComponentGrids()>0;
  int len=0;
  
//   int activeEquationDomain=0;
//   if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
//   {
//     ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));
//     // add an EquationDomain with default parameters
//     equationDomainList.push_back(EquationDomain(&parameters,equationDomainName));  
//     // By default all grids initially belong to the first equationDomain.
//     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     {
//       equationDomainList.gridDomainNumberList.push_back(activeEquationDomain);
//     }
//   }
  
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
    else if( answer=="linear elasticity" )
    {
      pdeChosen=true;
      pdeName="elasticity"; // answer;
      parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm")=
                          SmParameters::modifiedEquationTimeStepping;
    }
    else if( answer=="non-linear mechanics" )
    {
      pdeChosen=true;
      //      parameters.dbase.get<Parameters::PDE >("pde")=Parameters::convectionDiffusion;
      pdeName=answer;
      parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm")=
                          SmParameters::modifiedEquationTimeStepping;
    }
    else if( answer==pdeVariationCommands[0] ||
             answer==pdeVariationCommands[1] ||
             answer==pdeVariationCommands[2] ||
             answer==pdeVariationCommands[3] )
    {
      pdeVariation = ( answer==pdeVariationCommands[0] ? SmParameters::nonConservative :
                       answer==pdeVariationCommands[1] ? SmParameters::conservative :
                       answer==pdeVariationCommands[2] ? SmParameters::godunov : 
                       answer==pdeVariationCommands[3] ? SmParameters::hemp : SmParameters::nonConservative );
      if( pdeVariation==SmParameters::conservative )
      {
	useConservative=true;  // we could eliminate this variable 
      }
      // The hemp code computes the full deformed state, others compute displacements from the ref. state
      bool & methodComputesDisplacements = parameters.dbase.get<bool>("methodComputesDisplacements");
      if( pdeVariation==SmParameters::hemp )
	methodComputesDisplacements=false;
      else
	methodComputesDisplacements=true;

      if( pdeVariation==SmParameters::hemp )
      { // default time stepper for Hemp is improvedEuler
	parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm")=SmParameters::improvedEuler;
	
      }
      
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
    else if( setupDialog.getToggleValue(answer,"variable material properties",
				   parameters.dbase.get<int>("variableMaterialPropertiesOption")) )
    {
      if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
      {
        printF("Cgsm:INFO: turn ON variable material properties.\n");
	// By default the material properties vary from grid point to grid point (e.g. for TZ)
	parameters.dbase.get<int>("variableMaterialPropertiesOption")=GridMaterialProperties::variableMaterialProperties;
      }
      else
      {
        printF("Cgsm:INFO: turn OFF variable material properties.\n");
        parameters.dbase.get<int>("variableMaterialPropertiesOption")=GridMaterialProperties::constantMaterialProperties;
      }
      
    }
    else if( answer=="add extra variables" )
    {
      gi.inputString(answer,"Enter the number of extra variables");
      sScanF(answer,"%i",&parameters.dbase.get<int >("numberOfExtraVariables"));
      printF(" *** Adding %i extra variables *****\n",parameters.dbase.get<int >("numberOfExtraVariables"));
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
      else
      {
	printf("ERROR parsing the define parameter statement: answer=[%s]\n",(const char*) answer);
      }
      
      setupDialog.setTextLabel(answer(0,len-1),"<name> <value>");
    }
    // else if( setupDialog.getTextValue(answer,"number of components","%u",numberOfComponents) ){} //
    else if( setupDialog.getTextValue(answer,"solver name:","%s",name) )
    {
      // removing leading blanks
      int i=0;
      while( i<name.length() && name[i]==' ' ) i++;
      name=name(i,name.length()-1);
    }
  }
  
  gi.popGUI();  // pop setup

  return 0;
}

int Cgsm::
setPlotTitle(const real &t, const real &dt)
{
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  aString buff;


  psp.set(GI_TOP_LABEL,sPrintF(buff,"linear elasticity: t=%6.2e ",t));
//   if( parameters.dbase.get<int>("numberOfDimensions")==2 )
//     psp.set(GI_TOP_LABEL_SUB_1,sPrintF(buff,"a=%4.1g, b=%4.1g, kappa=%6.1g, dt=%4.1g",
// 				       a[0],b[0],kappa[0],dt));
//   else
//     psp.set(GI_TOP_LABEL_SUB_1,sPrintF(buff,"a=%4.1g, b=%4.1g, c=%4.1g, kappa=%6.1g, dt=%4.1g",
// 				       a[0],b[0],c[0],kappa[0],dt));
  
  return 0;
}
