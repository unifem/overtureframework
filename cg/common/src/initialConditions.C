#include "DomainSolver.h"
#include "ShowFileReader.h"
#include "display.h"
#include "App.h"
#include "ParallelUtility.h"
#include "Ogen.h"
#include "gridFunctionNorms.h"
#include "DialogState.h"
#include "interpPoints.h"
#include "InterpolatePoints.h"
#include "EquationDomain.h"
#include "InterpolatePointsOnAGrid.h"

int updateEquationDomainsForAMR( CompositeGrid & cg,Parameters & parameters );  // in addGrids.bC

#define ForAllComponents(n)  for( n=0; n<numberOfComponents; n++ )
#define ForAllGrids(grid)    for( grid=0; grid<numberOfComponentGrids; grid++ )

int 
newAdaptiveGridBuilt(CompositeGrid & cg, realCompositeGridFunction & u, Parameters & parameters,
                     bool updateSolution);


// int 
// userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u, Parameters & parameters );

// ===================================================================================================================
/// \brief Try to determine if two CompositeGrid's are the same.
/// \return Return true if the grids are definitely different. Return false if
///       the grids are probably the same.
// ===================================================================================================================
bool
isDifferent( const CompositeGrid & cg1, const CompositeGrid & cg2 )
{
  if( cg1.numberOfGrids()!=cg2.numberOfGrids() )
    return true;

  for( int grid=0; grid<cg1.numberOfGrids(); grid++ )
    if( max(abs(cg1[grid].dimension()-cg2[grid].dimension()))!=0 )
      return true;

  return false;
}

// ===================================================================================================================
/// \brief Cleanup routines after the initial conditions have been assigned. 
// ===================================================================================================================
void DomainSolver::
cleanupInitialConditions()
{
  if( parameters.dbase.has_key("puSF") )
  {
    realCompositeGridFunction *& puSF = parameters.dbase.get<realCompositeGridFunction*>("puSF");
    delete puSF; puSF=NULL;
  }
  if( parameters.dbase.has_key("pcgSF") )
  {
    CompositeGrid *& pcgSF = parameters.dbase.get<CompositeGrid*>("pcgSF");
    delete pcgSF; pcgSF=NULL;
  }
      
}

// ===================================================================================================================
/// \brief project initial conditions for moving grids.
/// \details For some problems the initial conditions need to be adjusted for moving grids, e.g. the
///    initial pressure for the INS may be coupled to the initial acceleration of moving modies. 
/// \gfIndex (input) : assign gf[gfIndex] at time gf[gfIndex].t 
// ===================================================================================================================
int DomainSolver::
projectInitialConditionsForMovingGrids(int gfIndex)
{
  return 0;
}


// ===================================================================================================================
/// \brief Assign initial conditions.
/// \details This routine actually assigns the initial conditions into gf[current].u based on the parameters setup
/// in getInitialConditions.
/// \gfIndex (input) : assign gf[gfIndex] at time gf[gfIndex].t 
// ===================================================================================================================
int DomainSolver::
assignInitialConditions(int gfIndex)
{

  realCompositeGridFunction & u = gf[gfIndex].u;
  const real t = gf[gfIndex].t;

  CompositeGrid & cg = gf[gfIndex].cg;  // *wdh* 2013/10/02 
  
  Parameters::InitialConditionOption & initialConditionOption = parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption");

  if( debug() & 2 )
    printF("DomainSolver::assignInitialConditions: gfIndex=%i t=%9.3e initialConditionOption=%i\n",
	   gfIndex,t,initialConditionOption);


  // Look for the sub-directory in the data-base to store variables used here and in assignInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("initialConditionData") )
  {
    printF("DomainSolver::assignInitialConditions:ERROR: sub-directory `initialConditionData' not found!\n");
    Overture::abort("error");
  }
  DataBase & icdb = parameters.dbase.get<DataBase >("modelData").get<DataBase>("initialConditionData");

  RealArray & uLeft    = icdb.get<RealArray>("uLeft");
  RealArray & uRight   = icdb.get<RealArray>("uRight");
  real & stepSharpness = icdb.get<real>("stepSharpness");
  real & stepNormalx   = icdb.get<real>("stepNormalx");
  real & stepNormaly   = icdb.get<real>("stepNormaly");
  real & stepNormalz   = icdb.get<real>("stepNormalz");
  real & stepNormalEquationValue = icdb.get<real>("stepNormalEquationValue");

  RealArray & initialConditions = parameters.dbase.get<RealArray>("initialConditions");

  if( initialConditionOption==Parameters::userDefinedInitialCondition )
  {
    userDefinedInitialConditions(cg,u);
  }
  else if( initialConditionOption==Parameters::knownSolutionInitialCondition )
  {
    printF("\n*** assignInitialConditions from knownSolutionInitialCondition t=%9.3e\n",t);
    
    // realCompositeGridFunction & uKnown = parameters.getKnownSolution( cg, parameters.dbase.get<real >("tInitial") );
    realCompositeGridFunction & uKnown = parameters.getKnownSolution( cg, t );
    u.dataCopy(uKnown);
  }
  else if( initialConditionOption==Parameters::readInitialConditionFromShowFile )
  {
    // Here we interpolate from the solution in the show file.
    printF("assignInitialConditions: interpolating the solution from the show file to the "
           "existing grid, t=%9.3e (gfIndex=%i, current=%i)...\n",t,gfIndex,current);

    if( gfIndex!=current )
    {
      printF("assignInitialConditions:INFO: assigning IC's at t=%10.4e (gfIndex=%i, past time?) from solution at t=%10.4e (current=%i)\n",
	     t,gfIndex,gf[current].t,current);
      
      u.dataCopy(gf[current].u);
    }
    else
    {
      realCompositeGridFunction *puSF = parameters.dbase.get<realCompositeGridFunction*>("puSF");
      assert( puSF!=NULL );
      realCompositeGridFunction & uSF = *puSF;
    

      // cg.update(MappedGrid::THEcenter); // *wdh* 110730 - turn this off

      if( true ) // Here is an even newer way which should handle all cases and parallel *wdh* 110321
      {
	const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
	Range C=numberOfComponents;

	InterpolatePointsOnAGrid interp;

	interp.setAssignAllPoints(true);  // assign all points -- extrap if necessary
	// interp.setInterpolationWidth( width ); // we could change the interp width here
	  
	interp.interpolateAllPoints( uSF,u, C, C );  // interpolate u from uSF
      }
      else
      {
	  
	interpolateAllPoints( uSF,u );  // interpolate u from uSF
      }
    }
    
  }
  else
  {
    const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
    int grid,n;
    Index I1,I2,I3;
    
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      getIndex( c.dimension(),I1,I2,I3 );
      #ifdef USE_PPP
        realSerialArray ug;  getLocalArrayWithGhostBoundaries(u[grid],ug);
        ug=0.;
      #else
       realSerialArray & ug = u[grid];
      #endif
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],ug,I1,I2,I3,1);

      if( initialConditionOption==Parameters::uniformInitialCondition )
      {
	ForAllComponents( n )
	{
	  if( ok )
	  {
	    if( initialConditions(n)!=(real)Parameters::defaultValue )
	      ug(I1,I2,I3,n)=initialConditions(n);
	    else
	      ug(I1,I2,I3,n)=0.;
	  }
	}
      }
      else if( initialConditionOption==Parameters::twilightZoneFunctionInitialCondition )
      {
	gf[gfIndex].cg.update(MappedGrid::THEcenter);
	u.updateToMatchGrid(gf[gfIndex].cg,nullRange,nullRange,nullRange,parameters.dbase.get<int >("numberOfComponents")); 
        OGFunction *& exactSolution = parameters.dbase.get<OGFunction* >("exactSolution");
	assert( exactSolution!=NULL );
	// exactSolution->assignGridFunction( u,parameters.dbase.get<real >("tInitial") );   //  Twilight-zone flow
	exactSolution->assignGridFunction( u,t );   //  Twilight-zone flow

        // u.display("assignInitialConditions: u after assign TZ IC's");
      }

      else if( initialConditionOption==Parameters::stepFunctionInitialCondition )
      {
	real *up = ug.Array_Descriptor.Array_View_Pointer3;
	const int uDim0=ug.getRawDataSize(0);
	const int uDim1=ug.getRawDataSize(1);
	const int uDim2=ug.getRawDataSize(2);
        #define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

        if( c.isRectangular() )
	{
          if( !ok ) continue;  // no points on this processor

	  real dx[3],xab[2][3];
	  c.getRectangularGridParameters( dx, xab );
            
	  const int i1a=c.gridIndexRange(0,0);
	  const int i2a=c.gridIndexRange(0,1);
	  const int i3a=c.gridIndexRange(0,2);
            
	  const real xa=xab[0][0], dx0=dx[0];
	  const real ya=xab[0][1], dy0=dx[1];
	  const real za=xab[0][2], dz0=dx[2];
                  	
#define VERTEX0(i1,i2,i3) (xa+dx0*(i1-i1a))
#define VERTEX1(i1,i2,i3) (ya+dy0*(i2-i2a))
#define VERTEX2(i1,i2,i3) (za+dz0*(i3-i3a))
#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

#define XSTEP(i1,i2,i3) (stepNormalx*VERTEX0(i1,i2,i3)+stepNormaly*VERTEX1(i1,i2,i3)+stepNormalz*VERTEX2(i1,i2,i3)-stepNormalEquationValue)

          printF("assignIC: step function: grid=%i stepNormal=(%9.2e,%9.2e,%9.2e), stepNormalEquationValue=%9.2e\n",
		 grid,stepNormalx,stepNormaly,stepNormalz,stepNormalEquationValue);
	  

          int i1,i2,i3;
	  ForAllComponents( n )
	  {
            real uLeftn=uLeft(n), uRightn=uRight(n);
	    if( stepSharpness>0. )
	    {
              FOR_3(i1,i2,i3,I1,I2,I3)
	        U(i1,i2,i3,n)=uLeftn+(uRightn-uLeftn)*(.5+.5*tanh(stepSharpness*(XSTEP(i1,i2,i3))));
	    }
	    else
	    {
              FOR_3(i1,i2,i3,I1,I2,I3)
              {
		
		if( XSTEP(i1,i2,i3) <= 0. )
		{
		  U(i1,i2,i3,n)=uLeftn;
		}
		else
		{
		  U(i1,i2,i3,n)=uRightn;
		}
	      }
	    }
	  }
	}
	else
	{
	  c.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
          if( !ok ) continue;  // no points on this processor
	  
          #ifndef USE_PPP
            const realArray & vertex = c.center();
          #else
            // In parallel, we operate on the arrays local to each processor
            realSerialArray vertex; getLocalArrayWithGhostBoundaries(c.center(),vertex);
          #endif

	  realSerialArray x(I1,I2,I3);
          if( c.numberOfDimensions()==2 )
  	    x=stepNormalx*vertex(I1,I2,I3,0)+stepNormaly*vertex(I1,I2,I3,1)-stepNormalEquationValue;
          else
            x=stepNormalx*vertex(I1,I2,I3,0)+
              stepNormaly*vertex(I1,I2,I3,1)+
              stepNormalz*vertex(I1,I2,I3,2)-stepNormalEquationValue;

	  ForAllComponents( n )
	  {
	    if( stepSharpness>0. )
	      ug(I1,I2,I3,n)=uLeft(n)+(uRight(n)-uLeft(n))*(.5+.5*tanh(stepSharpness*(x)));
	    
	    else
	    {
// 	      where( x <= 0. )
// 	      {
// 		ug(I1,I2,I3,n)=uLeft(n);
// 	      }
// 	      otherwise()  // trouble with P++ ??
// 	      {
// 		ug(I1,I2,I3,n)=uRight(n);
// 	      }
              ug(I1,I2,I3,n)=uRight(n);
              where( x <= 0. )
 	      {
 		ug(I1,I2,I3,n)=uLeft(n);

 	      }
	    }
	  }
	}
        if( debug() & 32 )
	{
	  ::display(ug,"initial conditions: ug (local array)",parameters.dbase.get<FILE* >("pDebugFile"));
	}
	if( debug() & 32 )
	{
	  ::display(u[grid],"Here are the initial conditions, u[grid]",parameters.dbase.get<FILE* >("debugFile"));
	}
      }
    
    }
  }
  
  return 0;
}

// ===================================================================================================================
/// \brief Determine the type of initial conditions to assign.
/// \param command (input) : optionally supply a command to execute. Attempt to execute the command
///    and then return. The return value is 0 if the command was executed, 1 otherwise.
/// \param interface (input) : use this dialog. If command=="build dialog", fill in the dialog and return.
/// \param guiState (input) : use this GUIState if provided.
/// \param dialogState (input) : add items found here to the dialog.
// ===================================================================================================================
int DomainSolver::
getInitialConditions(const aString & command /* = nullString */,
		     DialogData *interface /* =NULL */,
                     GUIState *guiState /* = NULL */,
                     DialogState *dialogState /* = NULL */ )
{
  int returnValue=0;
  Parameters::InitialConditionOption & initialConditionOption = parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption");

  real & tInitial = parameters.dbase.get<real >("tInitial");
  

  realCompositeGridFunction & u = gf[current].u;
  
  // Make a sub-directory in the data-base to store variables used here and in assignInitialConditions
  if( !parameters.dbase.get<DataBase >("modelData").has_key("initialConditionData") )
    parameters.dbase.get<DataBase >("modelData").put<DataBase>("initialConditionData");

  DataBase & icdb = parameters.dbase.get<DataBase >("modelData").get<DataBase>("initialConditionData");
  // allocate variables 
  if( !icdb.has_key("uLeft") )
  {
    icdb.put<RealArray>("uLeft");
    icdb.put<RealArray>("uRight");

    icdb.put<real>("stepSharpness",-1.);
    icdb.put<real>("stepNormalx",1.);
    icdb.put<real>("stepNormaly",0.);
    icdb.put<real>("stepNormalz",0.);
    icdb.put<real>("stepNormalEquationValue",0.);
  }
  RealArray & uLeft    = icdb.get<RealArray>("uLeft");
  RealArray & uRight   = icdb.get<RealArray>("uRight");
  real & stepSharpness = icdb.get<real>("stepSharpness");
  real & stepNormalx   = icdb.get<real>("stepNormalx");
  real & stepNormaly   = icdb.get<real>("stepNormaly");
  real & stepNormalz   = icdb.get<real>("stepNormalz");
  real & stepNormalEquationValue = icdb.get<real>("stepNormalEquationValue");
  RealArray & initialConditions = parameters.dbase.get<RealArray>("initialConditions");


  bool & useGridFromShowFile=parameters.dbase.get<bool>("useGridFromShowFile");
  // By default AMR or moving grid problems use the grid from the show file *wdh* 090818 
  if( parameters.isAdaptiveGridProblem() || parameters.isMovingGridProblem() )
    useGridFromShowFile=true;

  bool alwaysInterpolateFromShowFile=false; // If true always interpolate from the show file.
    

  aString nameOfShowFile;
  ShowFileReader showFileReader;
  int numberOfSolutions=-1, solutionNumber=-1;

  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  if( u.numberOfComponentGrids()==0 )
  {
    u.updateToMatchGrid(cg,nullRange,nullRange,nullRange,parameters.dbase.get<int >("numberOfComponents")); 
    gf[current].t=0.;
  }
  
  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");

  int n;
  ForAllComponents( n )
  {
    u.setName(parameters.dbase.get<aString* >("componentName")[n],n);
  }

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

  // ----- sibling dialogs ------
  
  if( pUniformFlowDialog==NULL )
  { // create sibling dialogs the first time thru.
    pUniformFlowDialog =&gui.getDialogSibling();
    pStepFunctionDialog=&gui.getDialogSibling();
    pShowFileDialog    =&gui.getDialogSibling();
  }
  assert( pTzOptionsDialog!=NULL );  // this should have been set
  //  pTzOptionsDialog   =&gui.getDialogSibling();
  
  DialogData & uniformFlowDialog  = *pUniformFlowDialog;
  DialogData & stepFunctionDialog = *pStepFunctionDialog;
  DialogData & showFileDialog     = *pShowFileDialog;
  DialogData &tzOptionsDialog     = *pTzOptionsDialog;

  if( interface==NULL || command=="build dialog" )
  {
    dialog.setOptionMenuColumns(1);

    const int maxCommands=20;
    aString cmd[maxCommands];

    // create a new menu with options for choosing a component.
    if( numberOfComponents>0 )
    {
      aString *cmd = new aString[numberOfComponents+1];
      aString *label = new aString[numberOfComponents+1];
      for( int n=0; n<numberOfComponents; n++ )
      {
	label[n]=u.getName(n);
	cmd[n]="plot:"+u.getName(n);

      }
      cmd[numberOfComponents]="";
      label[numberOfComponents]="";
    
      dialog.addOptionMenu("plot component:", cmd,label,0);
      delete [] cmd;
      delete [] label;
    }
    
    aString pbCommands[] = {"uniform flow...", 
			    "step function...",
			    "read from a show file...",
                            "twilight zone...",
                            "known solution",
			    "user defined...",
                            "change contour plot",
			    ""};
    int numRows=7;
    addPrefix(pbCommands,prefix,cmd,maxCommands);
    dialog.setPushButtons( cmd, pbCommands, numRows );


    const int numberOfTextStrings=40;            // we should count up how many we have 
    aString textCommands[numberOfTextStrings];
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    if( dialogState!=NULL && dialogState->textCommands!=NULL )
    {
      aString *& inputTextCommands = dialogState->textCommands;
      aString *& inputTextLabels   = dialogState->textLabels;
      aString *& inputTextStrings  = dialogState->textStrings;

      while( nt<numberOfTextStrings && inputTextCommands[nt]!="" )
      {
	textCommands[nt]=inputTextCommands[nt];
	textLabels[nt]  =inputTextLabels[nt];
	textStrings[nt] =inputTextStrings[nt];
        nt++;
      }
    }
    

    textCommands[nt]="initial time"; textLabels[nt] = textCommands[nt];  
    sPrintF(textStrings[nt], "%g", tInitial); nt++; 
    
    // null strings terminal list
    textCommands[nt]="";   textLabels[nt] = textCommands[nt]; textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textCommands,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);

 
    // **** here are the old (and currently available options)
    aString icMenu[]=  
    {
      "uniform flow",
      "read from a show file",
      "read from a restart file",
      "step function",
      "rotated step function",   // temporary
      "user defined",
      "spin down",
      ""
    };
//     gui.buildPopup(icMenu); // we can't do this since it over-rides the main popup

    aString pushButtonCommands[maxCommands];
    

    // ----------------------- uniform flow ------------------------------------------
    uniformFlowDialog.setWindowTitle("Uniform Flow Parameters");
    uniformFlowDialog.setExitCommand("close uniform flow", "close uniform flow");

    n=0;
    pushButtonCommands[n]="assign uniform state"; n++;
    pushButtonCommands[n]=""; n++;
    assert( n<maxCommands );

    numRows=n;
    addPrefix(pushButtonCommands,prefix,cmd,maxCommands);
    uniformFlowDialog.setPushButtons( cmd, pushButtonCommands, numRows );

    nt=0;
    textLabels[nt] = "uniform state";  
    textStrings[nt]="";
    assert( initialConditions.getLength(0)==numberOfComponents );
    ForAllComponents( n )
    {
      textStrings[nt]+=
	sPrintF(buff,"%s=%g",(const char *)parameters.dbase.get<aString* >("componentName")[n],
		initialConditions(n)!=(real)Parameters::defaultValue ? 
		initialConditions(n) :0.);

      if( n!=numberOfComponents ) textStrings[nt]+=", ";
      
    }
    nt++;
    
    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textLabels,prefix,cmd,maxCommands);
    uniformFlowDialog.setTextBoxes(cmd, textLabels, textStrings);

    // ----------------------- step function ------------------------------------------
    stepFunctionDialog.setWindowTitle("Step Function Parameters");
    stepFunctionDialog.setExitCommand("close step function", "close step function");

    n=0;
    pushButtonCommands[n]="assign step function"; n++;
    pushButtonCommands[n]=""; n++;
    assert( n<maxCommands );

    numRows=n;
    addPrefix(pushButtonCommands,prefix,cmd,maxCommands);
    stepFunctionDialog.setPushButtons( cmd, pushButtonCommands, numRows );

    nt=0;
    for( int side=0; side<=1; side++ )
    {
      textLabels[nt] = side==0 ? "state behind" : "state ahead";  
      textStrings[nt]="";
      ForAllComponents( n )
      {
	textStrings[nt]+=
	  sPrintF(buff,"%s=%g",(const char *)parameters.dbase.get<aString* >("componentName")[n],
		  initialConditions(n)!=(real)Parameters::defaultValue ? 
		  initialConditions(n) :0.);

	if( n<numberOfComponents-1 ) textStrings[nt]+=", ";
      
      }
      nt++;
    }
    
    textLabels[nt] = "step: a*x+b*y+c*z=d"; 
    textStrings[nt]=sPrintF("%g, %g, %g, %g, (a,b,c,d)",stepNormalx,stepNormaly,stepNormalz,stepNormalEquationValue);
    nt++;

    textLabels[nt] = "step sharpness"; textStrings[nt]=sPrintF("%g (-1=step)",stepSharpness);  nt++;

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textLabels,prefix,cmd,maxCommands);
    stepFunctionDialog.setTextBoxes(cmd, textLabels, textStrings);

    // ----------------------- read from a show file dialog ------------------------------------------
    showFileDialog.setWindowTitle("Read From a Show File");

    n=0;
    pushButtonCommands[n]="assign solution from show file"; n++;
    pushButtonCommands[n]="choose file from menu..."; n++;
    pushButtonCommands[n]=""; n++;
    assert( n<maxCommands );

    numRows=n;
    addPrefix(pushButtonCommands,prefix,cmd,maxCommands);
    showFileDialog.setPushButtons( cmd, pushButtonCommands, numRows );


    // toggle button: "use grid from show file"   <- set to true by default if moving or adaptive
    aString tbCommands[] = {"use grid from show file",
                            "always interpolate from show file",
                            ""};

    int tbState[10];
    tbState[0] = useGridFromShowFile; 
    tbState[1] = alwaysInterpolateFromShowFile;
    int numColumns=1;
    showFileDialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    nt=0;
    textLabels[nt] = "show file name"; textStrings[nt]="myShowFile.show";
    nt++;
    
    textLabels[nt] = "solution number"; textStrings[nt]="-1 (-1=last)";
    nt++;
   
    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textLabels,prefix,cmd,maxCommands);
    showFileDialog.setTextBoxes(cmd, textLabels, textStrings);

    showFileDialog.setExitCommand("close read show file", "close read show file");


    aString forcingOB[] = {"no forcing", "showfile forcing",""};
    dialog.addOptionMenu("Forcing Options:", forcingOB, forcingOB, parameters.dbase.get<Parameters::ForcingType >("forcingType"));


    if( executeCommand ) return 0;
  }
  





//\begin{>>setParametersInclude.tex}{\subsubsection{Initial condition options}\label{sec:icMenu}}
//\no function header:
//
// Here are the options for specifying initial conditions.
// This menu appears when {\tt `initial conditions'} is chosen from main parameter menu.
//\begin{description}
//  \item[uniform flow] : specify a uniform flow. Enter values in the form {\tt `p=1., u=2., ...'}.
//     Variables not specified will get default values (usually zero).
//  \item[step function] : specify two uniform conditions separted by a step
//  \item[read from a show file] : read the initial conditions from a solution in a show file.
//  \item[read from a restart file] : read the initial conditions from a solution in a restart file.
// \end{description}
//
//\end{setParametersInclude.tex}

  aString answer;
  
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
    else if( answer=="uniform flow..." )
    {
       uniformFlowDialog.showSibling();
    }
    else if( len=answer.matches("uniform state") )
    {
      answer = answer(len,answer.length()-1);

      parameters.dbase.get<RealArray >("initialConditions")=(real)Parameters::defaultValue;
      parameters.inputParameterValues(answer,"initial conditions", parameters.dbase.get<RealArray >("initialConditions") );
    
/* -----
      // **** -- this may not be needed:
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];
	getIndex( c.dimension(),I1,I2,I3 );
	ForAllComponents( n )
	  if( initialConditions(n)!=(real)Parameters::defaultValue )
	    u[grid](I1,I2,I3,n)=initialConditions(n);
	  else
	    u[grid](I1,I2,I3,n)=0.;
      }
    ------ */

      aString textString;
      ForAllComponents( n )
      {
	textString+=
	  sPrintF(buff,"%s=%g",(const char *)parameters.dbase.get<aString* >("componentName")[n],
		  initialConditions(n)!=(real)Parameters::defaultValue ? 
		  initialConditions(n) :0.);
	if( n<numberOfComponents-1 ) textString+=", ";
      }
      uniformFlowDialog.setTextLabel("uniform state",textString);

    }
    else if( answer=="assign uniform state" )
    {
      initialConditionOption=Parameters::uniformInitialCondition; newInitialConditionsChosen=true;
    }
    else if( answer=="close uniform flow" )
    {
      uniformFlowDialog.hideSibling();
    }
    else if( answer=="known solution" )
    {
      printF("--IC--Setting the initial condition to the known solution.\n");
      initialConditionOption=Parameters::knownSolutionInitialCondition;
      newInitialConditionsChosen=true;
    }
    
    else if( answer=="twilight zone..." )
    {
      tzOptionsDialog.showSibling();
    }
    else if( answer=="close twilight zone options" )
    {
      tzOptionsDialog.hideSibling(); 
    }
    else if( found=parameters.setTwilightZoneParameters( cg, answer,&tzOptionsDialog )==0 )
    {
      printF("getInitialConditions: answer found in parameters.setTwilightZoneParameters\n");

      if( (parameters.dbase.get<bool >("twilightZoneFlow") || parameters.dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow") ) &&
          !parameters.dbase.get<bool >("userDefinedTwilightZoneCoefficients") )
      {
        parameters.setTwilightZoneFunction(parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"),parameters.dbase.get<int >("tzDegreeSpace"),
                                           parameters.dbase.get<int >("tzDegreeTime"));
      }
      if( // ** wdh* 060416 parameters.dbase.get<bool >("twilightZoneFlow") && 
	 parameters.dbase.get<bool >("twilightZoneFlow") && 
          parameters.dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow") ) 
      {
	initialConditionOption=Parameters::twilightZoneFunctionInitialCondition;
        newInitialConditionsChosen=true;
      }
      else if( initialConditionOption==Parameters::knownSolutionInitialCondition )
      {
        newInitialConditionsChosen=true;
      }
      
    }
    else if( answer=="step function..." )
    {
      stepFunctionDialog.showSibling();
      
      // is this right to do here?
      uLeft.redim(numberOfComponents);    uLeft=0.;
      uRight.redim(numberOfComponents);   uRight=0.;

    }
    else if( len=answer.matches("state behind") )
    {
      if( uLeft.getLength(0)!=numberOfComponents )
      {
	uLeft.redim(numberOfComponents);    uLeft=0.;
	uRight.redim(numberOfComponents);   uRight=0.;
      }
      aString answer2 = answer(len,answer.length()-1);
      parameters.inputParameterValues(answer2,"state behind", uLeft);
      aString textString;
      ForAllComponents( n )
      {
	textString+= sPrintF(buff,"%s=%g",(const char *)parameters.dbase.get<aString* >("componentName")[n],uLeft(n));
	if( n<numberOfComponents-1 ) textString+=", ";
      }
      stepFunctionDialog.setTextLabel("state behind",textString);
    }
    else if( len=answer.matches("state ahead") )
    {
      if( uLeft.getLength(0)!=numberOfComponents )
      {
	uLeft.redim(numberOfComponents);    uLeft=0.;
	uRight.redim(numberOfComponents);   uRight=0.;
      }
      aString answer2 = answer(len,answer.length()-1);
      parameters.inputParameterValues(answer2,"state ahead", uRight);
      aString textString;
      ForAllComponents( n )
      {
	textString+= sPrintF(buff,"%s=%g",(const char *)parameters.dbase.get<aString* >("componentName")[n],uRight(n));
	if( n<numberOfComponents-1 ) textString+=", ";
      }
      stepFunctionDialog.setTextLabel("state ahead",textString);
    }
    else if( len=answer.matches("step: a*x+b*y+c*z=d") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&stepNormalx,&stepNormaly,&stepNormalz,
             &stepNormalEquationValue);
      stepFunctionDialog.setTextLabel("step: a*x+b*y+c*z=d",sPrintF("%g, %g, %g, %g (a,b,c,d)",stepNormalx,
								   stepNormaly,stepNormalz,stepNormalEquationValue));
    }
    else if( len=answer.matches("step sharpness") )
    {
      printF("The step sharpness, beta, determines the smoothness of the smooth step function.\n"
             "   The smooth step function is of the form tanh(beta*(x-x0)) \n"
             "   Increase beta to make the step sharper. \n");
      
      sScanF(answer(len,answer.length()-1),"%e",&stepSharpness);
      stepFunctionDialog.setTextLabel("step sharpness",sPrintF("%g (-1=step)",stepSharpness));
    }
    else if( answer=="assign step function" )
    {
      initialConditionOption=Parameters::stepFunctionInitialCondition; newInitialConditionsChosen=true;
    }
    else if( answer=="close step function" )
    {
      stepFunctionDialog.hideSibling();
    }

    else if( answer=="read from a show file..." )
    {
      showFileDialog.showSibling();
    }
    else if( answer=="close read show file" )
    {
      showFileDialog.hideSibling();
    }
    else if( showFileDialog.getToggleValue(answer,"use grid from show file",useGridFromShowFile) ){}//
    else if( showFileDialog.getToggleValue(answer,"always interpolate from show file",alwaysInterpolateFromShowFile) ){}//
    else if( (len=answer.matches("show file name")) ||
             answer=="choose file from menu..." )
    {
      if( answer=="choose file from menu..." )
      {
	gi.inputFileName(nameOfShowFile, ">> Enter the name of the (old) show file", ".show");
      }
      else
      {
	nameOfShowFile=answer(len+1,answer.length()-1);
      }
      
      printF("nameOfShowFile=[%s]\n",(const char*)nameOfShowFile);
      if( nameOfShowFile=="" || nameOfShowFile==" " )
        continue;
      
      showFileReader.open(nameOfShowFile);
      int numberOfFrames=showFileReader.getNumberOfFrames();
      numberOfSolutions = max(1,numberOfFrames);
      solutionNumber=numberOfSolutions;  

      showFileDialog.setTextLabel("show file name",nameOfShowFile);
      
      showFileDialog.setTextLabel("solution number",sPrintF("%i  (from %i to %i, -1=last)",
                             solutionNumber,1,numberOfSolutions));
      
    }
    else if( len=answer.matches("solution number") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&solutionNumber);
      if( numberOfSolutions>0 )
      {
	showFileDialog.setTextLabel("solution number",sPrintF("%i  (from %i to %i, -1=last)",
							      solutionNumber,1,numberOfSolutions));
      }
      else
      {
	showFileDialog.setTextLabel("solution number",sPrintF("%i  (-1=last)",solutionNumber));
      }
    }
    else if( answer=="read from a show file" || answer=="assign solution from show file" )
    {
      const bool oldWay= answer=="read from a show file";
      if( !oldWay && nameOfShowFile=="" )
      {
	gi.outputString("You must choose the name of a show file before you can assign a solution");
	continue;
      }

      initialConditionOption=Parameters::readInitialConditionFromShowFile; newInitialConditionsChosen=true;
      
      parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")=Parameters::readInitialConditionFromShowFile;
      printF("getInitialConditions: readFromShowFile\n");
      // Read in a solution from a show file
      // This only works if the first components of the grid functions match


      if( oldWay )
      {
	gi.inputFileName(nameOfShowFile,"Enter the name of the (old) show file:");
	if( nameOfShowFile=="" )
	  continue;

        showFileReader.open(nameOfShowFile);

	const int numberOfFrameSeries = showFileReader.getNumberOfFrameSeries();
	if( numberOfFrameSeries> 1 )
	{
	  printF("INFO: There are %i frame series in this show file\n",numberOfFrameSeries);
          printF("The domain name for the current grid is [%s]\n",(const char*)cg.getDomainName(0));
          int frameSeriesToUse=-1;
	  for( int fs=0; fs<numberOfFrameSeries; fs++ )
	  {
	    printF(" frame series %i : [%s]\n",fs,(const char*)showFileReader.getFrameSeriesName(fs));
	    if( showFileReader.getFrameSeriesName(fs)==cg.getDomainName(0) )
	    {
              frameSeriesToUse=fs;
	    }
	  }
          if( frameSeriesToUse>=0 )
	  {
	    printF("INFO: I will read the solution from frame series %i (%s) since the name matches this domain.\n",
		   frameSeriesToUse,(const char*)showFileReader.getFrameSeriesName(frameSeriesToUse));
	    showFileReader.setCurrentFrameSeries(frameSeriesToUse);
	  }
          else
	  {
            printF("WARNING: There are multiple frame series but I cannot find a name that matches the domain %s.\n"
                   " I will just read from the first frame series.\n", 
                   (const char*)cg.getDomainName(0));
	  }
	  
	}

	int numberOfFrames=showFileReader.getNumberOfFrames();
	numberOfSolutions = max(1,numberOfFrames);
	solutionNumber=numberOfSolutions;  // use last

        gi.inputString(answer,sPrintF(buff,"Enter the solution to use, from 1 to %i (-1=use last)",
				      numberOfSolutions));
	if( answer!="" )
	{
	  sScanF(answer,"%i",&solutionNumber);
	}
      } // end if oldWay
      

      if( solutionNumber<0 || solutionNumber>numberOfSolutions )
      {
	solutionNumber=numberOfSolutions;
      }
      
      // Save the show file grid and solution in the data base in case we need to build AMR levels
      parameters.dbase.put<CompositeGrid*>("pcgSF",NULL);
      CompositeGrid *& pcgSF = parameters.dbase.get<CompositeGrid*>("pcgSF");
      pcgSF = new CompositeGrid;
      CompositeGrid & cgSF = *pcgSF;

      parameters.dbase.put<realCompositeGridFunction*>("puSF",NULL);
      realCompositeGridFunction *& puSF = parameters.dbase.get<realCompositeGridFunction*>("puSF");
      puSF = new realCompositeGridFunction;
      realCompositeGridFunction & uSF = *puSF;
      
      showFileReader.getASolution(solutionNumber,cgSF,uSF);        // read in a grid and solution

      if( false && cgSF.numberOfRefinementLevels()>1 )
      {
	PlotStuffParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
        printF(" cgSF.numberOfRefinementLevels()=%i \n",cgSF.numberOfRefinementLevels());
	
        psp.set(GI_TOP_LABEL,"getInitialConditions: cgSF.refinementLevel[1]");
	PlotIt::plot(gi,cgSF.refinementLevel[1],psp);

        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      }

      // look for extra data that will appear if this show file was saved by this class
      HDF_DataBase & db = *showFileReader.getFrame();
      int found;
      found = db.get(tInitial,"t");
      if( found==0 )
      {
	printF("getInitialConditions: time taken from file =%9.3e\n", tInitial);

        dialog.setTextLabel("initial time",sPrintF("%g",tInitial));

        gf[current].t=tInitial;
	
      }
    
      // read any header comments that go with this solution
      int numberOfHeaderComments;
      const aString *headerComment=showFileReader.getHeaderComments(numberOfHeaderComments);
      for( int i=0; i<numberOfHeaderComments; i++ )
	printF("Header comment: %s \n",(const char *)headerComment[i]);

      
      printF(" getInitialConditions:cg.numberOfGrids()=%i, cgSF.numberOfGrids()=%i,\n",cg.numberOfGrids(),
	     cgSF.numberOfGrids());
      printF(" getInitialConditions:cg.numberOfComponentGrids()=%i, cgSF.numberOfComponentGrids()=%i,\n",
	     cg.numberOfComponentGrids(),
	     cgSF.numberOfComponentGrids());
    
      int showFileGridIsMoving=false;
      db.get(showFileGridIsMoving,"isMovingGridProblem");
      if( useGridFromShowFile && showFileGridIsMoving )
      {
	printF("getInitialConditions:The show file holds a moving grid problem. "
               "I will use the grid from the show file.\n");

	if( parameters.isMovingGridProblem() ) // 2011/07/13 -- only read moving grid data if we are really moving
	{
	  printF("getInitialConditions:INFO reading in movingGrids info from the show file.\n");
	  parameters.dbase.get<MovingGrids >("movingGrids").get(db,"movingGrids");
	}
	
      }

      bool gridWasTakenFromTheShowFile=false;
      if( useGridFromShowFile ) // *wdh* 090819 || cgSF.numberOfRefinementLevels()>1 || showFileGridIsMoving )
      {
        // use the grid from the show file in the case of AMR or moving grids
	// use the grid from the show file if it has refinement levels -- fix this -- should be an option

	gridWasTakenFromTheShowFile=true;
	
	if( cgSF.numberOfRefinementLevels()>1 )
	  printF("Grid in showfile has AMR grids. I will use this grid instead\n");
	else if( showFileGridIsMoving )
	  printF("Grid in showfile is moving. I will use this grid instead\n");
	
        // useGridFromShowFile=true;
        const int oldNumberOfBaseGrids=cg.numberOfBaseGrids();
	
	if( cg->interpolant!=NULL )
	{
	  printF("getInitialConditions:Interpolant is there. (showFileGridIsMoving=%i)\n",showFileGridIsMoving);
	}
	
        Interpolant *interpolant=cg->interpolant;
	cg.reference(cgSF);
	if( true )
	{
	  // 100501 -- new way : fixes reference counting 
	  if( interpolant!=NULL )
	  {
	    printF("update the interpolant..\n");
            interpolant->updateToMatchGrid(cg);
	  }
	}
	else
	{

	  cg->interpolant=interpolant;
	  if( cg->interpolant!=NULL )
	  {
	    printF("update the interpolant..\n");
	    cg->interpolant->updateToMatchGrid(cg);
	  }
	}
	
	gf[current].cg.reference(cg);  // *wdh* 061211
	
	u.updateToMatchGrid(cg,nullRange,nullRange,nullRange,parameters.dbase.get<int >("numberOfComponents")); 
	parameters.updateToMatchGrid(cg);

	// For now the AMR overlapping grid interp points are not saved in the show file -- so we regenerate them here
	// The problem is that these interp points need to live on the appropriate processor and thus it is
	// easier to re-create them 
#ifdef USE_PPP
	printF("getInitialConditions::updateRefinements for AMR grid to build AMR interpolation points\n");
	  
	if(  parameters.dbase.get<Ogen* >("gridGenerator")==NULL )
	  parameters.dbase.get<Ogen* >("gridGenerator") = new Ogen(*parameters.dbase.get<GenericGraphicsInterface* >("ps"));
	parameters.dbase.get<Ogen* >("gridGenerator")->updateRefinement(cg);
#endif
	

        // update equation domain lists here?
        if( cg.numberOfBaseGrids()>oldNumberOfBaseGrids && 
            parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
	{
          ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));
          const int numberOfEquationDomains=equationDomainList.size();
          if( numberOfEquationDomains>1 )
	  {
	    printF("WARNING:read from show file: The grid in the show file has more base grids\n"
		   " I am setting the EquationDomain for these extra base grids to be domain 0\n");
	  }
	  equationDomainList.gridDomainNumberList.resize(cg.numberOfComponentGrids());
	  for( int grid=oldNumberOfBaseGrids; grid<cg.numberOfBaseGrids(); grid++ )
	  { // Assume new base grids use equation domain 0 :
	    equationDomainList.gridDomainNumberList[grid]=0;
	  }
	}
	
        // assign equation domain numbers for any refinement grids:
        updateEquationDomainsForAMR( cg,parameters );
	
      } // end if useGridFromShowFile
      

// 	GridCollection & rl = cg.refinementLevel[0];
// 	printF(" number of grids on level=0 is %i\n",rl.numberOfGrids());
// 	GridCollection & rl0 = (*u.getCompositeGrid()).refinementLevel[0];
// 	printF(" number of grids on level=0 is %i\n",rl0.numberOfGrids());
      cgSF.update(MappedGrid::THEmask);
      cg.update(MappedGrid::THEmask);
      if( alwaysInterpolateFromShowFile || (!gridWasTakenFromTheShowFile && isDifferent(cg,cgSF)) )
      {
	// kkc 090330 added the check for the number of components just in case we store more in the solution than needed
	//            much of the code is copied from the stuff in the "else" section of the if block
	Range all;
        const int numberOfComponentsSF=uSF.getComponentBound(0)-uSF.getComponentBase(0)+1;
	const int nc=min(numberOfComponents,numberOfComponentsSF);
        if( nc!=numberOfComponents )
	{
	  printF("getInitialConditions:WARNING:numberOfComponents in show file =%i is not equal "
                 "to numberOfComponents=%i\n",
                 numberOfComponentsSF,numberOfComponents);
	  if( numberOfComponents>nc )
            printF(" getInitialConditions:I am setting the values for the extra variables to zero.\n");
	}
	Range C(0,nc-1);
        printF("Interpolating the solution from the show file to the existing grid...\n");
	// cg.update(MappedGrid::THEcenter); // *wdh* 110730 -- turn this off

	if( true ) // Here is an even newer way which should handle all cases and parallel *wdh* 110321
	{
          InterpolatePointsOnAGrid interp;

          interp.setAssignAllPoints(true);  // assign all points -- extrap if necessary
          // interp.setInterpolationWidth( width ); // we could change the interp width here
	  
	  interp.interpolateAllPoints( uSF,u, C, C );  // interpolate u from uSF
	}
	else if ( numberOfComponentsSF==numberOfComponents )
	  { // kkc 090330 original way
	    int status = interpolateAllPoints( uSF,u);
	    if ( !status )
	      printF("getInitialConditions:WARNING:interpolateAllPoints returned %i\n",status);
	  }
	else
	{ // kkc 090330 only if there are a different number of components
	  InterpolatePoints interp;
	  interp.interpolateAllPoints( uSF,u, C, C );  // interpolate u from uSF
	}
      }
      else
      {
        printF("readFromShow: Just copy values since the grids look the same. (Choose 'always interpolate' to force the"
               " solution to be interpolated.)\n");

	Range all;
        const int numberOfComponentsSF=uSF.getComponentBound(0)-uSF.getComponentBase(0)+1;
	const int nc=min(numberOfComponents,numberOfComponentsSF);
        if( nc!=numberOfComponents )
	{
	  printF("getInitialConditions:WARNING:numberOfComponents in show file =%i is not equal "
                 "to numberOfComponents=%i\n",
                 numberOfComponentsSF,numberOfComponents);
	  if( numberOfComponents>nc )
            printF(" getInitialConditions:I am setting the values for the extra variables to zero.\n");
	}
	Range C(0,nc-1);
	for( int grid=0; grid<cg.numberOfGrids(); grid++ )
	{
          if( nc<numberOfComponents )
	  {
            // u[grid](all,all,all,Range(nc,numberOfComponents-1))=0.;  // ----------------- fix this ------------
            assign(u[grid],0.,all,all,all,Range(nc,numberOfComponents-1));
	  }
	  
	  // ::display(uSF[grid],"uSF","%5.2f ");
	  

	  // u[grid](all,all,all,C)=uSF[grid](all,all,all,C);
          assign( u[grid],all,all,all,C, uSF[grid],all,all,all,C);
	  
          RealArray uMin(numberOfComponents),uMax(numberOfComponents);
	  
          GridFunctionNorms::getBounds(u[grid],uMin,uMax);  

// 	  for( int c=0; c<numberOfComponents; c++ )
// 	  {
//             real minU,maxU;
// 	    where( cg[grid].mask()!=0 )
// 	    {
// 	      minU=min(u[grid](all,all,all,c));
// 	      maxU=max(u[grid](all,all,all,c));
// 	    }
	  for( int c=0; c<numberOfComponents; c++ )
	  {
	    printF("Values from show file: grid=%i: component=%i: min=%e, max=%e \n",grid,c,uMin(c),uMax(c));
	  }
	}
        if( Parameters::checkForFloatingPointErrors )
          checkSolution(u,"getInitialCond");
      }
      
      if( false && cg.numberOfRefinementLevels()>1 )
      {
	PlotStuffParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	// psp.set(GI_TOP_LABEL,"getInitialConditions: Solution from the show file");
	// PlotIt::contour(gi,u,psp);

        printF(" gf[current].cg.numberOfRefinementLevels()=%i \n",cg.numberOfRefinementLevels());
	
        psp.set(GI_TOP_LABEL,"getInitialConditions: cg.refinementLevel[1]");
	PlotIt::plot(gi,cg.refinementLevel[1],psp);

        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      }
      

      if( useGridFromShowFile )
      { // delete the show file solution since it is no longer needed.
	delete puSF;  puSF=NULL;
        delete pcgSF; pcgSF=NULL;
      }
      
/* ---
   // try to match up components with the same name
   for( int c0=u.getComponentBase(0); c0<=u.getComponentBound(0); c0++ )
   {
   aString uName = u.getName(c0);
   for( int c1=uSF.getComponentBase(0); c1<=uSF.getComponentBound(0); c1++ )
   {
   if( uSF.getName(c1)==uName )
   {
   }
   }
   }

   -- */    

    }
    else if( answer=="read from a restart file" )
    {
      initialConditionOption=Parameters::readInitialConditionFromRestartFile; newInitialConditionsChosen=true;

      gi.inputString(answer,sPrintF(buff,"Enter the restart file name (default value=%s)",
				    (const char *)parameters.dbase.get<aString >("restartFileName")));
      if( answer!="" )
	parameters.dbase.get<aString >("restartFileName")=answer;
      printF(" attempt to read the restart file\n");

      readRestartFile(u,tInitial,parameters.dbase.get<aString >("restartFileName"));  

    }
    else if( answer=="uniform flow" )
    {
      // ***** old way *****
      initialConditionOption=Parameters::uniformInitialCondition; newInitialConditionsChosen=true;
      
      gi.inputString(answer,"Enter initial conditions as: `p=1., u=2., ...'");

      parameters.dbase.get<RealArray >("initialConditions")=(real)Parameters::defaultValue;
      parameters.inputParameterValues(answer,"initial conditions", parameters.dbase.get<RealArray >("initialConditions") );
    
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];
        #ifdef USE_PPP
         realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        #else
         realSerialArray & uLocal = u[grid]; 
        #endif

	getIndex( c.dimension(),I1,I2,I3 );
        
        int includeGhost=1;
	bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);
	if( !ok ) continue;
	ForAllComponents( n )
	{
	  if( initialConditions(n)!=(real)Parameters::defaultValue )
	  {
	    // u[grid](I1,I2,I3,n)=initialConditions(n);
            // assign( u[grid],initialConditions(n),I1,I2,I3,n);
            uLocal(I1,I2,I3,n)=initialConditions(n);
	  }
	  else
	  {
	    // u[grid](I1,I2,I3,n)=0.;
            // assign( u[grid],0.,I1,I2,I3,n);
            uLocal(I1,I2,I3,n)=0.;
	  }
	}
      }
    }
    else if( answer=="step function" || answer=="rotated step function" || answer=="smooth step function" )
    {
      // ***** old way *****
      initialConditionOption=Parameters::stepFunctionInitialCondition; newInitialConditionsChosen=true;

      real stepPosition;
      int stepAxis;
      uLeft.redim(numberOfComponents);    uLeft=0.;
      uRight.redim(numberOfComponents);   uRight=0.;

      stepPosition=0.;
      stepSharpness=0.;
      stepAxis=0;
      aString answer2;
      if( answer=="step function" || answer=="smooth step function" )
      {
        gi.inputString(answer2,"Enter the step function position: `x=value' or `y=value' or `z=value'");
	// also allow a*x+b*y=c
	char buff[80];
	sScanF(answer2,"%1s=%e",buff,&stepPosition);
	stepNormalEquationValue=stepPosition;
	aString axisName;
	axisName=buff;
        stepNormalx=stepNormaly=stepNormalz=0.;
	if( axisName=="x" )
	{
	  stepAxis=0;
	  stepNormalx=1.;
	}
	else if( axisName=="y" )
	{
	  stepAxis=1;
	  stepNormaly=1.;
	}
	else if( axisName=="z" )
	{
	  stepAxis=2;
	  stepNormalz=1.;
	}
	else
	{
	  printF("ERROR: Expecting an answer of the form `x=value' or `y=value' or `z=value', answer=[%s]\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	
        if( answer=="smooth step function" )
	{
	  stepSharpness=20.;
	  gi.inputString(answer2,"Enter the sharpness exponent beta, tanh(beta*(x-x0))");
	  sScanF(answer2,"%e",&stepSharpness);
          printF(" step sharpness = %e\n",stepSharpness);
	}
	

      }
      else
      {
        // look for a*x+b*y=d
        gi.inputString(answer2,"Enter step equation parameters a,b,c,d (ax+by+cz=d)");
        stepNormalx=1.;
	stepNormaly=-.2;
	stepNormalz=0.;
	stepNormalEquationValue=.15;

        sScanF(answer2,"%e %e %e %e",&stepNormalx,&stepNormaly,&stepNormalz,&stepNormalEquationValue);
/* ---
        int length=answer2.length();
        int i=0;
        while( answer[i]!='*' && i<length ) i++;
        sScanF(answer2(0,i-1),"%e",&a);
        i+=3;
        int i0=i;
	while( answer[i]!='*' && i<length ) i++;
        sScanF(answer2(i0,i-1),"%e",&b);
        i0=i+3;
	sScanF(answer2(i0,length-1),"%e",&d);
--- */
	

      }
      stepPosition=stepNormalEquationValue;

      gi.inputString(answer2,"Enter the state behind the step as: `p=1., u=2., ...'");
      parameters.inputParameterValues(answer2,"state behind", uLeft);

      gi.inputString(answer2,"Enter the state in front of the step as: `p=1., u=2., ...'");
      parameters.inputParameterValues(answer2,"state in front", uRight);

//    cout << " Layer: Enter stepPosition,stepSharpness,kdjump" << endl;
//    cout << " layer : ul+(ur-ul)*(1+tanh(stepSharpness*(x-stepPosition))/2" << endl;
//    cout << " stepAxis=0,1,2 : jump in x,y, or z direction" << endl;
//    cin >> stepPosition >> stepSharpness >> stepAxis;    // infile

      printF(" a=%8.2e, b=%8.2e, c=%8.2e, d=%8.2e, stepPosition = %e, stepSharpness=%e \n",
           stepNormalx,stepNormaly,stepNormalz,stepNormalEquationValue,stepPosition,stepSharpness,stepAxis);
      assert( stepAxis>=0 && stepAxis<cg.numberOfDimensions() );

      printF("uLeft  = [");
      for( int i=uLeft.getBase(0); i<=uLeft.getBound(0); i++ ) printF("%9.3e,",uLeft(i));
      printF("]\n");
      printF("uRight = [");
      for( int i=uRight.getBase(0); i<=uRight.getBound(0); i++ ) printF("%9.3e,",uRight(i));
      printF("]\n");
      
      // uLeft.display("Here is uLeft");
      // uRight.display("Here is uRight");

    
    }
    else if( answer=="spin down" )
    {
      initialConditionOption=Parameters::spinDownInitialCondition; newInitialConditionsChosen=true;

      cg.update(MappedGrid::THEcenter);

      const int & pc = parameters.dbase.get<int >("pc");
      const int & uc = parameters.dbase.get<int >("uc");
      const int & vc = parameters.dbase.get<int >("vc");
      const int & wc = parameters.dbase.get<int >("wc");
    
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];
        const realArray & center = c.center();
	getIndex( c.dimension(),I1,I2,I3 );
	u[grid](I1,I2,I3,pc)=0.;
	if( cg.numberOfDimensions()==2 )
	{
	  u[grid](I1,I2,I3,uc)=(-sin(twoPi*center(I1,I2,I3,axis2))
				*sin(   Pi*center(I1,I2,I3,axis1))
				*sin(   Pi*center(I1,I2,I3,axis1)));
	  u[grid](I1,I2,I3,vc)=(+sin(twoPi*center(I1,I2,I3,axis1))
				*sin(   Pi*center(I1,I2,I3,axis2))
				*sin(   Pi*center(I1,I2,I3,axis2)));
	}
	else
	{
	  const int symmetryAxis=axis1;
	  if( symmetryAxis==axis3 )
	  {
	    u[grid](I1,I2,I3,uc)=(-sin(twoPi*center(I1,I2,I3,axis2))
				  *sin(   Pi*center(I1,I2,I3,axis1))
				  *sin(   Pi*center(I1,I2,I3,axis1)));
	    u[grid](I1,I2,I3,vc)=(+sin(twoPi*center(I1,I2,I3,axis1))
				  *sin(   Pi*center(I1,I2,I3,axis2))
				  *sin(   Pi*center(I1,I2,I3,axis2)));
	    u[grid](I1,I2,I3,wc)=0.;
	  }
	  else if( symmetryAxis==axis1 ) 
	  {
	    u[grid](I1,I2,I3,uc)=0.;
	    u[grid](I1,I2,I3,vc)=(-sin(twoPi*center(I1,I2,I3,axis3))
				  *sin(   Pi*center(I1,I2,I3,axis2))
				  *sin(   Pi*center(I1,I2,I3,axis2)));
	    u[grid](I1,I2,I3,wc)=(+sin(twoPi*center(I1,I2,I3,axis2))
				  *sin(   Pi*center(I1,I2,I3,axis3))
				  *sin(   Pi*center(I1,I2,I3,axis3)));
	  }
	  else 
	  {
	    u[grid](I1,I2,I3,uc)=(-sin(twoPi*center(I1,I2,I3,axis3))
				  *sin(   Pi*center(I1,I2,I3,axis1))
				  *sin(   Pi*center(I1,I2,I3,axis1)));
	    u[grid](I1,I2,I3,vc)=0.;
	    u[grid](I1,I2,I3,wc)=(+sin(twoPi*center(I1,I2,I3,axis1))
				  *sin(   Pi*center(I1,I2,I3,axis3))
				  *sin(   Pi*center(I1,I2,I3,axis3)));
	  }
	}
      }
    }
  
    else if( answer=="user defined..." || answer=="user defined" )
    {
      initialConditionOption=Parameters::userDefinedInitialCondition; newInitialConditionsChosen=true;
      
      setupUserDefinedInitialConditions();

      // old way: userDefinedInitialConditions();

    }
    else if ( answer=="no forcing" )
      {
	if ( parameters.dbase.get<realCompositeGridFunction* >("forcingFunction") ) 
	  {
	    delete parameters.dbase.get<realCompositeGridFunction* >("forcingFunction");
	    parameters.dbase.get<realCompositeGridFunction* >("forcingFunction") = 0;
	  }
      }
    else if ( answer=="showfile forcing" )
      {
	aString fileName;
	gi.inputFileName(fileName,"Enter the name of the forcing function show file (e.g. forcing.show)");
	if ( fileName != "" )
	  {
	    ShowFileReader showFileReader;
	    showFileReader.open(fileName);
	    int numberOfFrames=showFileReader.getNumberOfFrames();
	    int numberOfSolutions = max(1,numberOfFrames);
	    solutionNumber=numberOfSolutions;  

	    CompositeGrid cgSF;
	    realCompositeGridFunction uSF(cg);
	    showFileReader.getASolution(solutionNumber,cgSF,uSF);
	    parameters.dbase.get<realCompositeGridFunction* >("forcingFunction") = new realCompositeGridFunction;
	    parameters.dbase.get<realCompositeGridFunction* >("forcingFunction")->updateToMatchGridFunction(u);
	    cg.update(MappedGrid::THEcenter);
	    cgSF.update(MappedGrid::THEmask);
	    cg.update(MappedGrid::THEmask);
	    interpolateAllPoints( uSF,*parameters.dbase.get<realCompositeGridFunction* >("forcingFunction") );  // interpolate u from uSF
	    parameters.dbase.get<Parameters::ForcingType >("forcingType") = Parameters::showfileForcing;
	    //	    PlotIt::contour(gi,*parameters.dbase.get<realCompositeGridFunction* >("forcingFunction"));
	  }
      }
    else if( dialog.getTextValue(answer,"initial time","%g",tInitial) )
    {
      gf[current].t=tInitial;
    }//
    else if( answer=="change contour plot" )
    {
      // plot the solution
      PlotStuffParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
      psp.set(GI_TOP_LABEL,"initial conditions");
      gi.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::contour(gi,u,parameters.dbase.get<GraphicsParameters >("psp"));
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
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
      if(  newInitialConditionsChosen &&
	   (initialConditionOption==Parameters::uniformInitialCondition || 
	    initialConditionOption==Parameters::stepFunctionInitialCondition ||
            initialConditionOption==Parameters::twilightZoneFunctionInitialCondition ||
	    initialConditionOption==Parameters::userDefinedInitialCondition ||
            initialConditionOption==Parameters::knownSolutionInitialCondition ) )
      {
	assignInitialConditions(current);

      }
      
    }
    if( (newInitialConditionsChosen || plotSolution) &&
        parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")!=Parameters::noInitialConditionChosen &&
        gi.isInteractiveGraphicsOn() )  // *wdh* 2011/08/21
    {
      // plot the solution
      PlotStuffParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
      psp.set(GI_TOP_LABEL,"initial conditions");
      gi.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      PlotIt::contour(gi,u,psp);
    }
    
  }  // end for it
  
// don't print here as the values may not have been set
//   if( debug() & 4 )
//     gf[current].u.display("Here is gf[current].u in getInitialConditions",parameters.dbase.get<FILE* >("debugFile"),"%7.1e ");

  
  showFileReader.close();
  
  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

  return returnValue;

}
