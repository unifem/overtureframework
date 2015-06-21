#include "AdParameters.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "Ogshow.h"
#include "PlotStuff.h"


int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);


//===================================================================================
//\begin{>ParametersInclude.tex}{\subsection{Variables in AdParameters}} 
//\no function header:
//
// /int numberOfDimensions: number of spacial dimensions.
//\end{ParametersInclude.tex}
//===================================================================================


// ==================================================================================
/// \brief Constructor.
//===================================================================================
AdParameters::
AdParameters(const int & numberOfDimensions0) : Parameters(numberOfDimensions0)
{
  int & numberOfComponents = dbase.get<int>("numberOfComponents"); 
  numberOfComponents=1;  // default
  
  if (!dbase.has_key("kappa")) dbase.put<std::vector<real> >("kappa");
  if (!dbase.has_key("a")) dbase.put<std::vector<real> >("a");
  if (!dbase.has_key("b")) dbase.put<std::vector<real> >("b");
  if (!dbase.has_key("c")) dbase.put<std::vector<real> >("c");


  // the thermalConductivity is used in boundary conditions at domain interfaces: 
  if (!dbase.has_key("thermalConductivity")) dbase.put<real>("thermalConductivity",-1.);

  if (!dbase.has_key("variableDiffusivity")) dbase.put<bool>("variableDiffusivity",false);
  if (!dbase.has_key("diffusivityIsTimeDependent")) dbase.put<bool>("diffusivityIsTimeDependent",false);
  if (!dbase.has_key("kappaVar"))
  {  // save variable diffusion coefficients here:
     dbase.put<realCompositeGridFunction*>("kappaVar");
     dbase.get<realCompositeGridFunction*>("kappaVar")=NULL;
  }

  if (!dbase.has_key("variableAdvection")) dbase.put<bool>("variableAdvection",false);
  if (!dbase.has_key("advectionIsTimeDependent")) dbase.put<bool>("advectionIsTimeDependent",false);
  if (!dbase.has_key("advectVar"))
  {  // save variable advection coefficients here:
     dbase.put<realCompositeGridFunction*>("advectVar");
     dbase.get<realCompositeGridFunction*>("advectVar")=NULL;
  }
  registerBC((int)mixedBoundaryCondition,"mixedBoundaryCondition");

 if( !dbase.has_key("implicitAdvection") ) dbase.put<bool>("implicitAdvection")=false;

  // initialize the items that we time: 
  initializeTimings();
}

// ==================================================================================
/// \brief Destructor. 
//===================================================================================
AdParameters::
~AdParameters()
{
  // -- delete work space created by the implicit time-stepping ---
  if( dbase.has_key("varCoeff") )
    delete dbase.get<realCompositeGridFunction*>("varCoeff"); 
  if( dbase.has_key("advectionCoeff") )
    delete dbase.get<realCompositeGridFunction*>("advectionCoeff"); 
}

int AdParameters::
setParameters(const int & numberOfDimensions0 /* =2 */,const aString & reactionName )
// ==================================================================================================
//  /reactionName (input) : optional name of a reaction or a reaction 
//     file that defines the chemical reactions, such as a Chemkin binary file. 
// ==================================================================================================
{
  int & numberOfComponents     = dbase.get<int>("numberOfComponents");

  // printF("AdParameters::setParameters: number of components=%i\n",numberOfComponents);

  int & numberOfExtraVariables = dbase.get<int>("numberOfExtraVariables");
  int & tc = dbase.get<int >("tc");
  aString *& componentName = dbase.get<aString* >("componentName");
   
  dbase.get<int >("numberOfDimensions")=numberOfDimensions0;
  //  dbase.get<Parameters::PDE >("pde")=pde0;
  dbase.get<int >("rc")= dbase.get<int >("uc")= dbase.get<int >("vc")= dbase.get<int >("wc")= 
    dbase.get<int >("pc")= tc= dbase.get<int >("sc")= dbase.get<int >("kc")= 
    dbase.get<int >("epsc")= dbase.get<int >("sec")=-1;
  
  dbase.get<bool >("computeReactions")=false;
  dbase.get<Reactions* >("reactions")=NULL;
  dbase.get<int >("numberOfSpecies")=0;
  
  int s, i;
  //...set component index'es, showVariables, etc. that are equation-specific

  printF("--AD-- AdParameters::setParameters: pdeName=[%s]\n",(const char*)pdeName);
  if( pdeName=="thinFilmEquations" )
  {
    // --- thin film equations ---
    //  Unknowns are
    //        h : height
    //        q : flux
    //        s : flourenscence concentration (optional)
    dbase.get<Range >("Rt")= numberOfComponents;  // "time dependent" components 
    delete  componentName;
    componentName= new aString [numberOfComponents];
    tc=0;
    componentName[tc  ]="h";
    if( numberOfComponents>=2 )
      componentName[tc+1]="p";
    if( numberOfComponents>=3 )
      componentName[tc+2]="s";

    addShowVariable( "h",tc );
    if( numberOfComponents>=2 )
      addShowVariable( "p",tc+1 );
    if( numberOfComponents>=3 )
      addShowVariable( "s",tc+2 );
  }
  else
  {
    // --- advection diffusion ---
    dbase.get<Range >("Rt")= numberOfComponents;

    if( numberOfExtraVariables>0 )
      numberOfComponents+=numberOfExtraVariables;

    tc=0;    
    dbase.get<Range >("Rt")=Range( tc,tc);              // time dependent components

    addShowVariable( "T",tc );


    delete  componentName;
    componentName= new aString [numberOfComponents];

    if( tc>=0 )  componentName[tc]="T";

    if( numberOfExtraVariables>0 )
    {
      aString buff;
      for( int e=0; e< numberOfExtraVariables; e++ )
      {
	int n= numberOfComponents- numberOfExtraVariables+e;
	componentName[n]=sPrintF(buff,"Var%i",e);
	addShowVariable(  componentName[n],n );
      }

    }
  }
  
  std::vector<real> & kappa = dbase.get<std::vector<real> >("kappa");
  std::vector<real> & a = dbase.get<std::vector<real> >("a");
  std::vector<real> & b = dbase.get<std::vector<real> >("b");
  std::vector<real> & c = dbase.get<std::vector<real> >("c");

  kappa.resize(numberOfComponents,.1);
  a.resize(numberOfComponents,0.);
  b.resize(numberOfComponents,0.);
  c.resize(numberOfComponents,0.);

  dbase.get<int >("stencilWidthForExposedPoints")=3;
  dbase.get<int >("extrapolateInterpolationNeighbours")=false;

  dbase.get<RealArray >("initialConditions").redim( numberOfComponents);  
  dbase.get<RealArray >("initialConditions")=defaultValue;
  
  dbase.get<RealArray >("checkFileCutoff").redim( numberOfComponents+1);  // cutoff's for errors in checkfile
  dbase.get<RealArray >("checkFileCutoff")=REAL_EPSILON*500.;
  
  return 0;
}


//\begin{>>AdParametersInclude.tex}{\subsection{setTwilightZoneFunction}} 
int AdParameters::
setTwilightZoneFunction(const TwilightZoneChoice & choice_,
                        const int & degreeSpace /* =2 */ , 
                        const int & degreeTime /* =1 */ )
// =============================================================================================
// /Description:
//
// /choice (input): AdParameters::polynomial or AdParameters::trigonometric
//\end{AdParametersInclude.tex}
// =============================================================================================
{
  TwilightZoneChoice choice=choice_;
  int & numberOfComponents     = dbase.get<int>("numberOfComponents");
  const int numberOfDimensions = dbase.get<int >("numberOfDimensions");
  
  //TODO: add TZ for passive scalar=passivec
  if( choice!=polynomial && choice!=trigonometric && choice!=pulse )
  {
    printF("Parameters:: setTwilightZoneFunction: TwilightZoneChoice=%i not recognized\n"
           "  TwilightZoneChoice=trigonometric will be used instead\n",choice);
  }

  delete  dbase.get<OGFunction* >("exactSolution");
  if( choice==polynomial )
  {
    // ******* polynomial twilight zone function ******
     dbase.get<OGFunction* >("exactSolution") = new OGPolyFunction(degreeSpace, numberOfDimensions, numberOfComponents,degreeTime);

    Range R5(0,4);
    RealArray spatialCoefficientsForTZ(5,5,5, numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5, numberOfComponents);      
    timeCoefficientsForTZ=0.;


    // default case:
    for( int n=0; n< numberOfComponents; n++ )
    {
      real ni =1./(n+1);
    
      spatialCoefficientsForTZ(0,0,0,n)=2.+n;      
      if( degreeSpace>0 )
      {
	spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
	spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,0,1,n)=  numberOfDimensions==3 ? .25*ni : 0.;
      }
      if( degreeSpace>1 )
      {
	spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	spatialCoefficientsForTZ(0,0,2,n)=  numberOfDimensions==3 ? .125*ni : 0.;

	if( false ) // *wdh* 050610
	{
	  // add cross terms
	  printF("\n\n ************* add cross terms to TZ ************** \n\n");
	    

	  spatialCoefficientsForTZ(1,1,0,n)=.125*ni;
	  if(  numberOfDimensions>2 )
	  {
	    spatialCoefficientsForTZ(1,0,1,n)=.1*ni;
	    spatialCoefficientsForTZ(0,1,1,n)=-.15*ni;
	  }
	}
      }
      if( degreeSpace>2 )
      {
	const int degreeSpace3 = numberOfDimensions==3 ? degreeSpace : 0;
        for( int m1=0; m1<=degreeSpace; m1++ )for( int m2=0; m2<=degreeSpace; m2++ )for( int m3=0; m3<=degreeSpace3; m3++ )
	{
	  if( (m1+m2+m3)==degreeSpace )
	  { // choose "random" coefficients
	    spatialCoefficientsForTZ(m1,m2,m3,n)=ni/(m1+2.*m2+1.5*m3);
	  }
	}
      }

    }

    for( int n=0; n< numberOfComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
      {
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
      }
	  
    }
  
    // ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
    
    ((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  // for u
  
  }
  else if( choice==trigonometric ) // ******* Trigonometric function chosen ******
  {
    RealArray fx( numberOfComponents),fy( numberOfComponents),fz( numberOfComponents),ft( numberOfComponents);
    RealArray gx( numberOfComponents),gy( numberOfComponents),gz( numberOfComponents),gt( numberOfComponents);
    gx=0.;
    gy=0.;
    gz=0.;
    gt=0.;
    RealArray amplitude( numberOfComponents), cc( numberOfComponents);
    amplitude=1.;
    cc=0.;

    fx= dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0];
    fy =  numberOfDimensions>1 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1] : 0.;
    fz =  numberOfDimensions>2 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2] : 0.;
    ft =  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3];

     dbase.get<OGFunction* >("exactSolution") = new OGTrigFunction(fx,fy,fz,ft);
    
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setShifts(gx,gy,gz,gt);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setAmplitudes(amplitude);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setConstants(cc);
      
  }
  else if( choice==pulse ) 
  {
    // ******* Pulse function chosen ******
     dbase.get<OGFunction* >("exactSolution") =  new OGPulseFunction( numberOfDimensions, numberOfComponents); 

    // ******* Pulse function chosen ******
     ArraySimpleFixed<real,9,1,1,1> & pulseData = dbase.get<ArraySimpleFixed<real,9,1,1,1> >("pulseData");

     printF("CgAd: setTwilightZoneFunction:INFO: create the OGPulseFunction pulseData="
            "[%.2g,%.2g,%.2g,%.2g,%.2g,%.2g,%.2g,%.2g,%.2g\n",pulseData[0],pulseData[1],pulseData[2],
            pulseData[3],pulseData[4],pulseData[5],pulseData[6],pulseData[7],pulseData[8]);

     dbase.get<OGFunction* >("exactSolution") =  new OGPulseFunction( numberOfDimensions, numberOfComponents, pulseData[0],pulseData[1],pulseData[2],pulseData[3],pulseData[4],pulseData[5],
         pulseData[6],pulseData[7],pulseData[8]); 

  }
    
  
  
  return 0;
}

//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

// ============================================================================================
/// \brief Assign the default values for the data required by the boundary conditions.
// ============================================================================================
int AdParameters::
setDefaultDataForABoundaryCondition(const int & side,
				    const int & axis,
				    const int & grid,
				    CompositeGrid & cg)
{
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  RealArray & bcData = dbase.get<RealArray>("bcData");

  Range all;
  const Range & Ru = dbase.get<Range >("Ru");
  const int & pc = dbase.get<int >("pc");
  const int & tc = dbase.get<int >("tc");
  
  switch( cg[grid].boundaryCondition(side,axis) ) 
  {
  case Parameters::neumannBoundaryCondition:
    for( int c=0; c<numberOfComponents; c++ )
    {
      mixedRHS(c,side,axis,grid)=0.;
      mixedCoeff(c,side,axis,grid)=0.;
      mixedNormalCoeff(c,side,axis,grid)=1.;
      // printF("*** AdParameters::setDefaultDataForABC: set default neumannBC ****\n");
    }
    break;

  case AdParameters::mixedBoundaryCondition:
    for( int c=0; c<numberOfComponents; c++ )
    {
      mixedRHS(c,side,axis,grid)=0.;
      mixedCoeff(c,side,axis,grid)=1.;
      mixedNormalCoeff(c,side,axis,grid)=1.;
      // printF("*** AdParameters::setDefaultDataForABC: set default mixedBoundaryCondition ****\n");
    }
    break ;

  }
  return 0;
  
}

int AdParameters::
setPdeParameters(CompositeGrid & cg, const aString & command /* = nullString */,
                 DialogData *interface /* =NULL */ )
// =====================================================================================
// /Description:
//   Prompt for changes in the PDE parameters.
// =====================================================================================
{
  int returnValue=0;

  assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");

  aString prefix = "OBPDE:"; // prefix for commands to make them unique.

  // ** Here we only look for commands that have the proper prefix ****
  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;

  int & numberOfComponents = dbase.get<int>("numberOfComponents");
  bool & implicitAdvection = dbase.get<bool >("implicitAdvection");
  

  std::vector<real> & kappa = dbase.get<std::vector<real> >("kappa");
  std::vector<real> & a = dbase.get<std::vector<real> >("a");
  std::vector<real> & b = dbase.get<std::vector<real> >("b");
  std::vector<real> & c = dbase.get<std::vector<real> >("c");

  real & thermalConductivity = dbase.get<real>("thermalConductivity");
  
  aString answer,line;
  char buff[100];
//  const int numberOfDimensions = cg.numberOfDimensions();
  

  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=40;
    aString cmd[maxCommands];
    dialog.setWindowTitle("Advection-diffusion parameters");

    // push buttons
    aString pbCommands[] = {"user defined coefficients",
			    ""};

    const int numRows=2;
    addPrefix(pbCommands,prefix,cmd,maxCommands);
    dialog.setPushButtons( cmd, pbCommands, numRows ); 


    const int numberOfTextStrings=5+1;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    for( int n=0; n<4; n++ )
    {
      std::vector<real> & par = n==0 ? kappa : n==1 ? a : n==2 ? b : c;
      aString name = n==0 ? "kappa" : n==1 ? "a" : n==2 ? "b" : "c";
      textLabels[nt] = name; 
      line="";
      for( int m=0; m<numberOfComponents; m++ )
	line += sPrintF(buff,"%g ",par[m]);
      textStrings[nt]=line;  nt++;
    }
    
    textLabels[nt] = "thermal conductivity";  sPrintF(textStrings[nt], "%g",thermalConductivity);  nt++; 
 
    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);

    // -- toggle buttons --
    aString tbLabels[] = {"variable diffusivity",
                          "variable advection", 
                          "treat advection implicitly",
			  ""};
    int tbState[3];
    tbState[0] = dbase.get<bool >("variableDiffusivity");
    tbState[1] = dbase.get<bool >("variableAdvection");
    tbState[2] = implicitAdvection;

    int numColumns=1;
    addPrefix(tbLabels,prefix,cmd,maxCommands);
    dialog.setToggleButtons(cmd, tbLabels, tbState, numColumns); 


//     gui.buildPopup(pdeParametersMenu);
//     delete [] pdeParametersMenu;
    
    if( false && gi.graphicsIsOn() )
      dialog.openDialog(0);   // open the dialog here so we can reset the parameter values below

    updatePDEparameters();

    if( executeCommand ) return 0;
  }
  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("pde parameters>");  
  }
  int len;
  for(int it=0; ; it++)
  {
    if( !executeCommand )
    {
      gi.getAnswer(answer,"");
    }
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

    // printf("setPdeParameters: answer=[%s]\n",(const char*)answer);
    

    if( answer=="done" )
      break;
    else if( answer.matches("kappa") ||
             answer.matches("a") ||
             answer.matches("b") ||
             answer.matches("c") )
    {
      aString name = answer.matches("kappa") ? "kappa" : answer.matches("a") ? "a" : answer.matches("b") ? "b" : "c";
      int n = name=="kappa" ? 0 : name=="a" ? 1 : name=="b" ? 2 : 3;
      len = name.length();
      std::vector<real> & par = n==0 ? kappa : n==1 ? a : n==2 ? b : c;

      assert( numberOfComponents<10 );
      real val[10]={1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,};
      
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e %e %e %e %e ",&val[0],&val[1],&val[2],&val[3],&val[4],
                 &val[5],&val[6],&val[7],&val[8],&val[9] );

      line="";
      for( int m=0; m<numberOfComponents; m++ )
      {
	if( numberOfComponents==1 )
	  printF(" set %s=%g \n",(const char*)name,val[m]);
        else
	  printF(" set %s[%i]=%g \n",(const char*)name,m,val[m]);
        par[m]=val[m];
	line += sPrintF(buff,"%g ",par[m]);
      }
      
      dialog.setTextLabel(name,line);
    }
    else if( dialog.getTextValue(answer,"thermal conductivity","%e",thermalConductivity) )
    {
      printF("INFO: The thermalConductivity=%g is used for flux interfaces between domains\n",thermalConductivity);
    }
    else if( dialog.getToggleValue(answer,"variable diffusivity", dbase.get<bool >("variableDiffusivity")) ){}//
    else if( dialog.getToggleValue(answer,"variable advection", dbase.get<bool >("variableAdvection")) ){}//
    else if( dialog.getToggleValue(answer,"treat advection implicitly",implicitAdvection) )
    {
      if( implicitAdvection )
	printF("--AD-- INFO: advection terms will be treated IMPLICITLY when using implicit time stepping.\n");
      else
	printF("--AD-- INFO: advection terms will be treated EXPLICITLY when using implicit time stepping.\n");
    }
    

    else if( answer=="user defined coefficients" ) 
    {
      updateUserDefinedCoefficients(gi);
    }

    else if(  dbase.get<ListOfShowFileParameters >("pdeParameters").matchAndSetValue( answer ) )
    {
      printF("*** answer=[%s] was found as a user defined parameter\n",(const char*)answer);
      
    }

    else
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing  dbase.get<real >("a") single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	printF("Unknown response=[%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
       
    }

  }

  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

 return returnValue;

}



int AdParameters::
displayPdeParameters(FILE *file /* = stdout */ )
// =====================================================================================
// /Description:
//   Display PDE parameters
// =====================================================================================
{
  const char *offOn[2] = { "off","on" };
  int & numberOfComponents     = dbase.get<int>("numberOfComponents");

  fprintf(file,
	  "PDE parameters: equation is `advection diffusion'.\n");

  // The  dbase.get<DataBase >("modelParameters") will be displayed here:
  Parameters::displayPdeParameters(file);

  fprintf(file,
	  "  number of components is %i\n",
	  numberOfComponents);

  std::vector<real> & kappa = dbase.get<std::vector<real> >("kappa");
  std::vector<real> & a = dbase.get<std::vector<real> >("a");
  std::vector<real> & b = dbase.get<std::vector<real> >("b");
  std::vector<real> & c = dbase.get<std::vector<real> >("c");
  for( int n=0; n<4; n++ )
  {
    std::vector<real> & par = n==0 ? kappa : n==1 ? a : n==2 ? b : c;
    aString name = n==0 ? "kappa" : n==1 ? "a" : n==2 ? "b" : "c";
    for( int m=0; m<numberOfComponents; m++ )
    {
      if( numberOfComponents==1 )
	fprintf(file," %s=%g",(const char*)name,par[m]);
      else
	fprintf(file," %s[%i]=%g,",(const char*)name,m,par[m]);
    }
    fprintf(file,"\n");
  }

  return 0;
}




//\begin{>>AdParametersInclude.tex}{\subsection{updateShowFile}} 
int AdParameters::
saveParametersToShowFile()
// =================================================================================================
// /Description:
//     Save PDE specific parameters in the show file.
//     These parameters can be used for a restart. They can also be used, for example,
//     by the user defined derived functions (when viewing the show file with plotStuff).
// 
//\end{AdParametersInclude.tex}  
// =================================================================================================
{
  assert(  dbase.get<Ogshow* >("show")!=NULL );

  ListOfShowFileParameters & showFileParams = dbase.get<ListOfShowFileParameters>("showFileParams");

  int & numberOfComponents     = dbase.get<int>("numberOfComponents");

  // save parameters
  showFileParams.push_back(ShowFileParameter("advectionDiffusion","pde"));
    
  std::vector<real> & kappa = dbase.get<std::vector<real> >("kappa");
  std::vector<real> & a = dbase.get<std::vector<real> >("a");
  std::vector<real> & b = dbase.get<std::vector<real> >("b");
  std::vector<real> & c = dbase.get<std::vector<real> >("c");
  for( int m=0; m<numberOfComponents; m++)
  {
    showFileParams.push_back(ShowFileParameter(sPrintF("kappa[%i]",m),kappa[m]));
    showFileParams.push_back(ShowFileParameter(sPrintF("a[%i]",m),a[m]));
    showFileParams.push_back(ShowFileParameter(sPrintF("b[%i]",m),b[m]));
    showFileParams.push_back(ShowFileParameter(sPrintF("c[%i]",m),c[m]));
  }
  showFileParams.push_back(ShowFileParameter("thermalConductivity",dbase.get<real>("thermalConductivity")));
    
  // Now save parameters common to all solvers:
  Parameters::saveParametersToShowFile();   

  return 0;
}


