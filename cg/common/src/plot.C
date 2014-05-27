#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "App.h"
#include "ParallelUtility.h"
#include "FileOutput.h"
#include "Ogen.h"
#include "EquationDomain.h"
#include "BodyForce.h"

// =============================================================================================
/// \brief Build the run time dialog. This dialog appears while a Domain solver is time stepping.
// =============================================================================================
int DomainSolver::
buildRunTimeDialog()
{
  int & plotOption = parameters.dbase.get<int >("plotOption");
  int & plotMode = parameters.dbase.get<int >("plotMode");

  // *wdh* 070529 if( plotMode==1 && plotOption==0 ) return 0;  // plotting is disabled with "no plotting"

  GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  if( parameters.dbase.get<GUIState* >("runTimeDialog")==NULL )
  {
    parameters.dbase.get<GUIState* >("runTimeDialog") = new GUIState;
    GUIState & dialog = *parameters.dbase.get<GUIState* >("runTimeDialog");
    

    dialog.setWindowTitle((const char*)getClassName());
    dialog.setExitCommand("finish", "finish");

    // --- push buttons ----
    aString cmds[] = {"break",
                      "continue",
                      "movie mode",
                      "movie and save",
                      "contour", 
                      "streamlines",
                      "grid", 
                      // "plot parallel dist.",
                      // "plot material properties",
                      "erase",
                      "plot options...",
                      "change the grid...",
                      "adaptive grids...", 
                      "show file options...",
                      "file output...",
                      "pde parameters...",
                      "time stepping params...",
                      "general options...",
                      "moving grid options...", 
                      ""};
    numberOfPushButtons=17;  // number of entries in cmds
    if( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL )
    {
      assert( cmds[numberOfPushButtons]="" );
      cmds[numberOfPushButtons]="plot distance to walls";
      numberOfPushButtons++;
    }
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    // get any extra components such as errors for tz flow or the pressure for CNS.
    realCompositeGridFunction *pu=NULL;
    realCompositeGridFunction v;
    if( plotMode==1 && plotOption==0 )
    {
      // no plotting (do not build the augmented solution to save space).
      pu = & gf[current].u;
    }
    else
    {
      pu = &getAugmentedSolution(gf[current],v);

    }
    realCompositeGridFunction & u = *pu;

    const int numberOfComponents = u.getComponentBound(0)-u.getComponentBase(0)+1;
    // create a new menu with options for choosing a component.
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

    
//     aString tbLabels[] = {"contour","streamlines","grid",""};
//     int tbState[4];
//     tbState[0] = 1;
//     tbState[1] = 0;
//     tbState[2] = 0;
//     tbState[3] = 0;
//     int numColumns=1;
//     dialog.setToggleButtons(tbLabels, tbLabels, tbState, numColumns); 

    if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
    {
      aString tbCommands[] = {"use local time stepping",""};
      int tbState[10];
      tbState[0] = parameters.dbase.get<int >("useLocalTimeStepping");
      int numColumns=1;
      dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);
    }

    const int numberOfTextStrings=5;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    if( !parameters.isSteadyStateSolver() )
    {
      textLabels[nt] = "final time";     sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("tFinal"));  nt++; 
      textLabels[nt] = "times to plot";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("tPrint"));  nt++; 
    }
    else
    {
      textLabels[nt] = "max iterations";  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("maxIterations"));  nt++; 
      textLabels[nt] = "plot iterations"; sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("plotIterations"));  nt++; 
    }
    if ( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::steadyStateNewton )
      {
	textLabels[nt] = "cfl";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("cfl"));  nt++; 
      }
    else
      {
	textLabels[nt] = "implicit factor";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("implicitFactor"));  nt++; 
      }


    textLabels[nt] = "debug";  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("debug"));  nt++; 
 
       // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);
    numberOfTextBoxes=nt;
    

    // *************** adaptive grids ****************************
    DialogData & adaptiveGridDialog = dialog.getDialogSibling();

    adaptiveGridDialog.setWindowTitle("Adaptive Grid Parameters");
    adaptiveGridDialog.setExitCommand("close adaptive grid dialog", "close");

    aString tbLabels[] = {"use adaptive grids",""};
    int tbState[2];
    tbState[0] = (int)parameters.dbase.get<bool >("adaptiveGridProblem");
    tbState[1] = 0;
    int numColumns=1;
    adaptiveGridDialog.setToggleButtons(tbLabels, tbLabels, tbState, numColumns); 

    nt=0;
    textLabels[nt] = "error threshold";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("errorThreshold"));  nt++; 
    textLabels[nt] = "regrid frequency";  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("amrRegridFrequency"));  nt++; 
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    adaptiveGridDialog.setTextBoxes(textLabels, textLabels, textStrings);

    // ******************* file output *************************
    DialogData & fileOutputDialog = dialog.getDialogSibling();

    fileOutputDialog.setWindowTitle("File Output Parameters");
    fileOutputDialog.setExitCommand("close file output dialog", "close");

    aString cmdf[] = {"file output",
                      "output periodically to a file",
                      "close an output file",
                      "save restart file",
                      "save current grid to a file",
                      ""};
    int numberOfRows=5;
    fileOutputDialog.setPushButtons( cmdf, cmdf, numberOfRows );

    nt=0;
    textLabels[nt] = "output file name";  sPrintF(textStrings[nt], "%s","cg.out");  nt++; 
    textLabels[nt]= "restart file name";  sPrintF(textStrings[nt], "%s",(const char*)parameters.dbase.get<aString >("restartFileName"));nt++; 
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    fileOutputDialog.setTextBoxes(textLabels, textLabels, textStrings);


    // ****** pde parameters *************
    DialogData &pdeDialog = dialog.getDialogSibling();
    pdeDialog.setExitCommand("close pde options", "close");
    parameters.setPdeParameters(gf[current].cg,"build dialog",&pdeDialog);

    // ********************* time stepping options **************************
    DialogData &timeSteppingDialog = dialog.getDialogSibling();
    timeSteppingDialog.setWindowTitle("Time Stepping Parameters");
    timeSteppingDialog.setExitCommand("close time stepping", "close");
    buildTimeSteppingDialog(timeSteppingDialog );

//     // ****** solver parameters *************
//     DialogData &solverDialog = dialog.getDialogSibling();
//     solverDialog.setExitCommand("close solver options", "close");
//     setSolverParameters("build dialog",&solverDialog);

    // ********************* general options ********************************
    DialogData &generalOptionsDialog = dialog.getDialogSibling();
    generalOptionsDialog.setWindowTitle("General Options");
    generalOptionsDialog.setExitCommand("close general options", "close");
    buildGeneralOptionsDialog(generalOptionsDialog );


    // ********************* moving grid options ********************************
    DialogData &movingGridOptionsDialog = dialog.getDialogSibling();
    movingGridOptionsDialog.setWindowTitle("Moving Grid Options");
    movingGridOptionsDialog.setExitCommand("close moving grid options", "close");
    buildMovingGridOptionsDialog(movingGridOptionsDialog );


    // ********************* plot options ********************************
    DialogData &plotOptionsDialog = dialog.getDialogSibling();
    plotOptionsDialog.setWindowTitle("Plot Options");
    plotOptionsDialog.setExitCommand("close plot options", "close");
    buildPlotOptionsDialog(plotOptionsDialog );



    // ****** old popup *****

    aString answer;
    const int numberOfMenuItems=25;
    aString menu0[numberOfMenuItems]=
    {
      "continue",
      "contour",
      ">choose a component",
      "<grid",
      "streamlines",
      "movie mode",
      "movie and save",
      "set final time",
      "set plot interval",
      "debug",
      "save a restart file",
      ">file output",
      "output to a file",
      "output periodically to a file",
      "close an output file",
      "<change the grid",
      ">adaptive grids",
      "turn on adaptive grids",
      "turn off adaptive grids",
      "error threshold",
      "<show file options",
      "erase",
      "finish",
      "" 
    };

//\begin{>runTimeMenuInclude.tex}{}
//\no function header:
//  \begin{description}
//    \item[plot component:] choose the solution component to plot.
//    \item[break] : If running in movie mode this command will cause the program to halt at the next
//                   time to plot.
//    \item[continue] : compute the solution to the next time to plot.
//    \item[movie mode] : compute the solution to the final time without waiting. The solution will be
//         plotted at each output time interval. 
//    \item[movie and save] : movie mode plus save each frame as a ppm file.
//    \item[contour] : enter the contour plotting function in {\tt PlotStuff}. Here you will more options
//       to change the plot.  
//    \item[streamlines] : enter the streamlines plotting function from {\tt PlotStuff}.
//    \item[grid] : enter the grid plotting function from {\tt PlotStuff}. If you don't first erase
//        the contour plot then both the contours and the grid will be shown.
//    \item[erase] : erase the screen.
//    \item[change the grid] : add, remove or change existing grids. (poor man's adaptive mesh refinement).
//    \item[adaptive grids...] : open up a new dialog to show parameters adaptive grids.
//    \begin{description}
//       \item[use adaptive grids] : turn adaptive grids on or off.
//       \item[error threshold] : specify the error threshold.
//    \end{description}
//    \item[show file options...] : choose show file options; e.g. open or close a show file.
//    \item[file output...] : specify options for saving solutions to an ascii file (for plotting with matlab for example).
//        There are a number of options available as to what data should be saved. See also the userDefinedOutput routine
//        where you can customize output.
//      \begin{description}
//         \item[output periodically to a file] : Open a file for output; specify how often to save data in the
//          file (every step, every second step...); specify what data to save in the file (only grid 1, only
//           values on some boundaries etc).  Each time this menu item is selected a new file is opened, allowing
//           one, for example, to save certain information every step and other information every tenth step.
//        \item[close an output file] : Close a file opened by the command `output periodically to a file'.
//       \item[save a restart file] : save the current solution as a restart file; usually I just use the
//         show file for restarts.
//    \end{description}
//    \item[pde parameters...] change PDE parameters at run time.
//    \item[final time] : change the value for the final time to integrate to.
//    \item[times to plot] : change the time interval between plotting (and output).
//    \item[debug] : enter an integer to turn on debugging info. This is a bit flag with debug=1 turning on just
//       a bit of info, debug=3 (1+2) showing more, debug=7 (1+2+4) even more etc.
//    \item[finish] : do not compute any further, exit and save the show files etc.
//  \end{description}
//\end{runTimeMenuInclude.tex} 


    char buffer[100];

    // create a new pop-up menu with options for choosing a component.
    aString *menu = new aString[numberOfMenuItems+numberOfComponents+1];
    chooseAComponentMenuItem=-1;
    int j=0;
    for( int i=0; ; i++ )
    {
      menu[j]=menu0[i];
      if( menu[j]=="" )
	break;
      if( menu[j]==">choose a component" )
      {
	chooseAComponentMenuItem=j;
	for( int n=0; n<numberOfComponents; n++ )
	{
	  menu[++j]=u.getName(n);
	  if( menu[j]=="" )
	    menu[j]="unknown";
	  
	}
      }
      j++;
    }
    
    dialog.buildPopup(menu);

    delete [] menu;

    ps.pushGUI(dialog);
  }
  return 0;

}


void DomainSolver::
setSensitivity( GUIState & dialog, 
                bool trueOrFalse )
{
  dialog.getOptionMenu(0).setSensitive(trueOrFalse);
  int n;
  for( n=1; n<numberOfPushButtons; n++ ) // leave first push button sensitive (=="break")
    dialog.setSensitive(trueOrFalse,DialogData::pushButtonWidget,n);
  
  for( n=0; n<numberOfTextBoxes; n++ )
    dialog.setSensitive(trueOrFalse,DialogData::textBoxWidget,n);
  
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{plot}} 
int DomainSolver::
plot(const real & t, 
     const int & optionIn,
     real & tFinal )
// ========================================================================================
// /Description:
//
//     Plot the solution at user requested times. Query for changes to the parameters.
// 
//  optionIn :  0 - wait for a response
//              1 - plot and wait for a response
//              2 - do not wait for response after plotting
// /Notes:
//    plotMode=1 : changes to plotting are disabled. This normally means we are running in "noplot" mode.
// /Return values: 0=normal exit. 1=user has requested "finish".
//\end{CompositeGridSolverInclude.tex}  
// ========================================================================================
{
  // *wdh* 030916 if( parameters.dbase.get<GenericGraphicsInterface* >("ps")==NULL || option==0 )
//   if( parameters.dbase.get<int >("myid")==0 ) 
//     printf(" OB_CGS:INFO: plotMode=%i plotOption=%i\n",parameters.dbase.get<int >("plotMode"),parameters.dbase.get<int >("plotOption"));
  
  int & plotOption = parameters.dbase.get<int >("plotOption");
  int & plotMode = parameters.dbase.get<int >("plotMode");

  // Here we ignored command input if were were running without plotting: 
  // *wdh* 070529 if( plotMode==1 && plotOption==0 ) return 0;       // plotting is disabled with "no plotting"

  int option = optionIn;
  if( optionIn==0 ) 
    option=1;

  // printf("XXX:plot: myid=%i parameters.dbase.get<GenericGraphicsInterface* >("ps")->isGraphicsWindowOpen()=%i\n",parameters.dbase.get<int >("myid"),parameters.dbase.get<GenericGraphicsInterface* >("ps")->isGraphicsWindowOpen());

  GenericGraphicsInterface *& pps = parameters.dbase.get<GenericGraphicsInterface* >("ps");
  if( pps==NULL )
    return 0;

  checkArrayIDs(sPrintF("plot: start") ); 

  int returnValue=0;

  GenericGraphicsInterface & ps = *pps;
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  
  assert( parameters.dbase.get<GUIState* >("runTimeDialog")!=NULL );
  GUIState & dialog = *parameters.dbase.get<GUIState* >("runTimeDialog");

  aString answer;

  real cpu0=getCPU();

  char buff[100];

  GridFunction & solution = gf[current];
  // get any extra components such as errors for tz flow or the pressure for CNS.

  // **** no need to compute extra components if we are in movie mode and we are not
  //      plotting any extra component ****
  realCompositeGridFunction v;
  // realCompositeGridFunction & u = getAugmentedSolution(solution,v);  // u is either solution or v
  realCompositeGridFunction *pu=NULL;
  if( plotMode==1 && plotOption==0 )
  {
    // no plotting (do not build the augmented solution to save space).
    pu = & solution.u;
  }
  else
  {
    pu = &getAugmentedSolution(solution,v);

  }
  realCompositeGridFunction & u = *pu;

  const int numberOfComponents = u.getComponentBound(0)-u.getComponentBase(0)+1;

  if( movieFrame>=0   )
  { // save a ppm file as part of a movie.
    psp.set(GI_HARD_COPY_TYPE,GraphicsParameters::ppm);
    ps.outputString(sPrintF(buff,"Saving file %s%i.ppm",(const char*)movieFileName,movieFrame));
    ps.hardCopy(    sPrintF(buff,            "%s%i.ppm",(const char*)movieFileName,movieFrame),psp);
    psp.set(GI_HARD_COPY_TYPE,GraphicsParameters::postScript);
    movieFrame++;
  }


  checkArrayIDs(sPrintF("plot: after getAugmented") ); 

  // The graphics parameters keeps a pointer to the pde parameters for use by some graphics functions.
  // (such as the contour line plotter)
  ListOfShowFileParameters & sfp = parameters.dbase.get<ListOfShowFileParameters>("pdeParameters");
  sfp.setParameter("time",t);  // set the current time 
  psp.showFileParameters = &sfp;
  

  ps.erase();
  if( option & 1 )
  {
    setPlotTitle(t,dt);

    bool plotObjects=true;  // this means plot the objects that have been previously plotted

    if( (option & 2) )
    {
      // Plot all the the things that the user has previously plotted
      // printF("plotObjects: itemsToPlot bits=[%i%i%i%i]\n",(itemsToPlot & 1),(itemsToPlot & 2),(itemsToPlot & 4),(itemsToPlot & 8));
	  
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      if( itemsToPlot & 8 ) // plot this first since title is wrong in body force graphics parameters
      {
        // Plot body/boundary forcing regions and immersed boundaries. 
	BodyForce::plotForcingRegions(ps, parameters.dbase, solution.cg, psp); 
      }
      if( itemsToPlot & 1 )
	PlotIt::plot(ps, solution.cg,psp);
      if( itemsToPlot & 2 )
	PlotIt::contour(ps,u,psp);
      if( itemsToPlot & 4 )
	PlotIt::streamLines(ps,u,psp);

      if( parameters.dbase.get<bool >("plotStructures") )
      {
	parameters.dbase.get<MovingGrids >("movingGrids").plot(ps,solution,psp);
      }
    }

    // printf("XXX myid=%i pps->isGraphicsWindowOpen()=%i\n",parameters.dbase.get<int >("myid"),pps->isGraphicsWindowOpen());
    checkArrayIDs(sPrintF("plot: after initial plots") ); 

    bool programHalted=false;
    int checkForBreak=false;
    const int processorForGraphics = pps!=NULL ? pps->getProcessorForGraphics() : 0;
    if( option & 2  && pps!=NULL && !(ps.readingFromCommandFile()) &&
        parameters.dbase.get<int >("myid")==processorForGraphics && ps.isGraphicsWindowOpen() )
    { // we are running interactively and we should check for a "break" command:
      checkForBreak=true; 
    }
    broadCast(checkForBreak,processorForGraphics); // broadcast to all from the processor for graphics

    if( checkForBreak )
    {
      // movie mode ** check here if the user has hit break ***
      // ps.outputString(sPrintF(buff,"Check for break at t=%e\n",t));
      answer="";
      
      int menuItem = ps.getAnswerNoBlock(answer,"monitor>");
      // printf("answer = [%s]\n",(const char*)answer);
      
      if( answer=="break" )
      {
	programHalted=true;

      }
    }


    

    checkArrayIDs(sPrintF("plot: begin loops") ); 

    if( ! (option & 2) || programHalted )
    {
      if( plotOption==3 )
      {
	setSensitivity( dialog,true );
      }
      
      plotOption=1; // reset movie mode if set.
      movieFrame=-1;
      
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

      DialogData & adaptiveGridDialog      = dialog.getDialogSibling(0);
      DialogData & fileOutputDialog        = dialog.getDialogSibling(1);
      DialogData & pdeDialog               = dialog.getDialogSibling(2);
      DialogData & timeSteppingDialog      = dialog.getDialogSibling(3);
      DialogData & generalOptionsDialog    = dialog.getDialogSibling(4);
      DialogData & movingGridOptionsDialog = dialog.getDialogSibling(5);
      DialogData & plotOptionsDialog       = dialog.getDialogSibling(6);

      itemsToPlot |= 8;  // for now always turn on body force regions 

      int len;
      for(;;)
      {
        real time1=getCPU();
	
	if( plotObjects )
	{
	  // Plot all the the things that the user has previously plotted
	  // printF("plotObjects: itemsToPlot bits=[%i%i%i%i]\n",(itemsToPlot & 1),(itemsToPlot & 2),(itemsToPlot & 4),(itemsToPlot & 8));
	  
          ps.erase();
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  if( itemsToPlot & 8 ) 
	  {
            // plot this first since title is wrong in body force graphics parameters
	    BodyForce::plotForcingRegions(ps, parameters.dbase, solution.cg, psp); 
	  }
	  if( itemsToPlot & 1 )
	    PlotIt::plot(ps, solution.cg,psp);
	  if( itemsToPlot & 2 )
	    PlotIt::contour(ps,u,psp);
	  if( itemsToPlot & 4 )
	    PlotIt::streamLines(ps,u,psp);

          if( parameters.dbase.get<bool >("plotStructures") )
	  {
	    parameters.dbase.get<MovingGrids >("movingGrids").plot(ps,solution,psp);
	  }

	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
          plotObjects=false;
	}


	int menuItem = ps.getAnswer(answer,"choose answer");       // query for an input 

        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForWaiting"))+=getCPU()-time1;  // do not count waiting in timing

        if( answer=="" )
	{ // *wdh* 110319 
	  printF("DomainSolver::plot:WARNING: answer is null!\n");
	}
	else if( answer=="contour" )
	{
          if( itemsToPlot & 2 )
            ps.erase();

          PlotIt::contour(ps,u,psp);

	  if( psp.getObjectWasPlotted() ) 
	    itemsToPlot |= 2;
          else
            itemsToPlot &= ~2;

	  plotObjects=true; // replot objects
	  
          int component;
	  psp.get(GI_COMPONENT_FOR_CONTOURS,component);
          dialog.getOptionMenu("plot component:").setCurrentChoice(component);

	}
	else if( menuItem > chooseAComponentMenuItem && 
                 menuItem <= chooseAComponentMenuItem+numberOfComponents )
	{
          // plot a new component
	  int component=menuItem-chooseAComponentMenuItem-1;
 
          dialog.getOptionMenu("plot component:").setCurrentChoice(component);

          if( itemsToPlot & 2 )
	  {
   	    plotObjects=true; // replot objects
	  }
	}
        else if( answer=="grid" )
	{

          PlotIt::plot(ps,solution.cg,psp);

	  if( psp.getObjectWasPlotted() ) 
	    itemsToPlot |= 1;
          else
            itemsToPlot &= ~1;

	  plotObjects=true; // replot objects
	}
	else if( answer=="streamlines" )
	{
          PlotIt::streamLines(ps,u,psp);
	  if( psp.getObjectWasPlotted() ) 
	    itemsToPlot |= 4;
          else
            itemsToPlot &= ~4;

          plotObjects=true; // replot objects
	}
        else if( answer=="plot distance to walls" )
	{
          if( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL )
	  {
	    ps.erase();
	    psp.set(GI_TOP_LABEL,"distance to walls");
	    PlotIt::contour(ps, *parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"), psp);

	    plotObjects=true; // replot objects

	  }
	  else
	  {
	    printF("Sorry: the distance to the boundary has not been computed yet.\n");
	  }
	}
	else if( answer=="plot parallel dist." )
	{
	  ps.erase();
	  psp.set(GI_TOP_LABEL,"Parallel distribution");
	  PlotIt::plotParallelGridDistribution(solution.cg,ps,psp);
	  ps.erase();

	  plotObjects=true; // replot objects

	}
        else if( answer=="forcing regions plot options" )
	{
          // change options for plotting body/boundary forcing regions
	  ps.erase();
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
          // Plot body/boundary forcing regions and immersed boundaries. 
          BodyForce::plotForcingRegions(ps, parameters.dbase,solution.cg,psp);
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  plotObjects=true; // replot objects
	}
	

        else if( answer=="plot material properties" )
	{
          // --- plot material properties ---
          realCompositeGridFunction matPropValues;

	  int rt = getMaterialProperties( solution, matPropValues );
	  if( rt==-1 )
	  {
	    printF("Sorry: there are no variable material properties defined.\n");
	    continue;
	  }
	  
	  ps.erase();
	  psp.set(GI_TOP_LABEL,"Material properties");

	  PlotIt::contour(ps,matPropValues, psp);

	  plotObjects=true; // replot objects
	}
        else if( answer=="plot body force mask" )
	{
	  if( !parameters.dbase.get<bool >("turnOnBodyForcing") )
	  {
	    printF("plot body force mask: there are no body forcings defined!\n");
	    continue;
	  }

	  if( !parameters.dbase.has_key("bodyForceMaskGridFunction") )
	  {
	    printF("plot body force mask: there is no body force mask defined!\n");
	    continue;
	  }
	  realCompositeGridFunction & bodyForceMask = 
                          *parameters.dbase.get<realCompositeGridFunction*>("bodyForceMaskGridFunction");

          // The body force mask iso surface has its own graphics parameter:
          if( !parameters.dbase.has_key("bodyForceMaskGraphicsParameters") )
	  {
            parameters.dbase.put<GraphicsParameters>("bodyForceMaskGraphicsParameters");
	  }
	      
          GraphicsParameters & gp = parameters.dbase.get<GraphicsParameters>("bodyForceMaskGraphicsParameters");
	  

	  ps.erase();
	  gp.set(GI_TOP_LABEL,"Body force mask");

	  PlotIt::contour(ps,bodyForceMask, gp);

	  plotObjects=true; // replot objects
	}

 	else if( answer=="erase" )
	{
          ps.erase();
	  itemsToPlot=0;

          itemsToPlot |= 8;  // turn on body force regions 
	}
        else if( answer=="save a restart file" )
	{
	  ps.inputFileName(answer,sPrintF(buff,"Enter the restart file name (default value=%s)",
					  (const char *)parameters.dbase.get<aString >("restartFileName")));
	  if( answer!="" )
	    parameters.dbase.get<aString >("restartFileName")=answer;

	  saveRestartFile(solution,parameters.dbase.get<aString >("restartFileName"));
	}
        else if( answer=="save restart file" ) // new way, do not prompt for restart file name
	{
	  saveRestartFile(solution,parameters.dbase.get<aString >("restartFileName"));
	}
        else if( answer=="output to a file" )
	{
	  FileOutput fileOutput;
	  fileOutput.update(u,ps);
	}
	else if( answer=="output periodically to a file" || answer=="output periodically to a file..." )
	{
          if( parameters.dbase.get<int >("numberOfOutputFiles")>=Parameters::maximumNumberOfOutputFiles )
	  {
	    printF("ERROR: too many files open\n");
	    continue;
	  }
          parameters.dbase.get<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency")[parameters.dbase.get<int >("numberOfOutputFiles")]=1;
          ps.inputString(answer,"Save to the file every how many steps? (default=1)");
          sScanF(answer,"%i",&parameters.dbase.get<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency")[parameters.dbase.get<int >("numberOfOutputFiles")]);
	  
          FileOutput & fileOutput = * new FileOutput;
	  parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[parameters.dbase.get<int >("numberOfOutputFiles")] = &fileOutput;
	  parameters.dbase.get<int >("numberOfOutputFiles")++;
          
          fileOutput.update(u,ps);

	  
	}
	else if( answer=="close an output file" )
	{
          aString *fileMenu = new aString [parameters.dbase.get<int >("numberOfOutputFiles")+2];
          int n;
	  for( n=0; n<parameters.dbase.get<int >("numberOfOutputFiles"); n++ )
	  {
	    fileMenu[n]=parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[n]->getFileName();
	  }
          fileMenu[parameters.dbase.get<int >("numberOfOutputFiles")]="none";
          fileMenu[parameters.dbase.get<int >("numberOfOutputFiles")+1]="";
	  int fileChosen = ps.getMenuItem(fileMenu,answer,"Choose a file to close");
	  if( fileChosen>=0 && fileChosen<parameters.dbase.get<int >("numberOfOutputFiles") )
	  {
            printF("close file %s\n",(const char*)fileMenu[fileChosen]);
	    delete parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[fileChosen];
            parameters.dbase.get<int >("numberOfOutputFiles")--;
	    for( n=fileChosen; n<parameters.dbase.get<int >("numberOfOutputFiles"); n++ )
	      parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[n]=parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[n+1];
	    parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[parameters.dbase.get<int >("numberOfOutputFiles")]=NULL;
	  }
	}
	else if( answer=="file output" )
	{
          fileOutput(ps,u);  // old way
	}
	else if( answer=="continue" )
	{
	  if( !parameters.isSteadyStateSolver() )
	  {
	    if( t >= tFinal-dt/10. )
	    {
	      printF("WARNING: t=tFinal. Choose `finish' if you really want to end\n");
	    }
	    else
	      break;
	  }
	  else
	  {
            if( parameters.dbase.get<int >("globalStepNumber")+1>=parameters.dbase.get<int >("maxIterations") )
	    {
	      printF("WARNING: %i steps (maxIterations=%i) have been taken. Choose `finish' if you really want to end\n",
                parameters.dbase.get<int >("globalStepNumber")+1,parameters.dbase.get<int >("maxIterations") );
	    }
	    else
              break;
	  }
	  
 	}
	else if( answer=="save current grid to a file" )
	{
	  aString gridFileName="cgGrid.hdf", gridName="currentGrid";
	  printF("Saving the current grid in the file %s\n",(const char*)gridFileName);
	  printF("You can use ogen to re-generate this grid using the commands: \n"
		 "ogen\ngenerate an overlapping grid\n read in an old grid\n  %s\n  reset grid"
		 "\n display intermediate results\n compute overlap\n",
		 (const char*)gridFileName); 
	  Ogen::saveGridToAFile( solution.cg, gridFileName,gridName );
	}
	else if( answer=="movie mode" )
	{
          plotOption= 3;  // don't wait

  	  setSensitivity( dialog,false );
          break;
 	}
        else if( answer=="movie and save" )
	{
	  ps.inputString(answer,"Enter basic name for the ppm files (default=plot)");
	  if( answer !="" && answer!=" ")
	    movieFileName=answer;
          else
	    movieFileName="plot";
          ps.outputString(sPrintF(buff,"pictures will be named %s0.ppm, %s1.ppm, ...",
            (const char*)movieFileName,(const char*)movieFileName));
	  movieFrame=0;
          plotOption=3;  // don't wait

  	  setSensitivity( dialog,false );
          break;
	}
        else if( answer=="change the grid" || answer=="change the grid..." )
	{
          addGrids();

	  getAugmentedSolution(solution,v);  /// recompute augmented solution

	  plotObjects=true; // replot objects
	}
	else if( answer=="turn on adaptive grids" )
	{
	  parameters.dbase.get<bool >("adaptiveGridProblem")=true;
	  printF("Using adaptive mesh refinement.\n");
	}
	else if( answer=="turn off adaptive grids" )
	{
	  parameters.dbase.get<bool >("adaptiveGridProblem")=false;
	  printF("Do NOT use adaptive mesh refinement.\n");
	}
        else if( adaptiveGridDialog.getTextValue(answer,"error threshold","%e",parameters.dbase.get<real >("errorThreshold")) )
	{
	  printF(" Setting errorThreshold=%9.3e\n",parameters.dbase.get<real >("errorThreshold"));
	}
        else if( adaptiveGridDialog.getTextValue(answer,"regrid frequency","%i",parameters.dbase.get<int >("amrRegridFrequency")) )
	{
	  printF(" Setting amrGridFrequency=%i\n",parameters.dbase.get<int >("amrRegridFrequency"));
	}
        else if( answer=="show file options" || answer=="show file options..." )
	{
           parameters.updateShowFile();
	}
	else if( answer=="finish" )
	{
          tFinal=t;
          parameters.dbase.get<int >("maxIterations")=parameters.dbase.get<int >("globalStepNumber")+1;
	  
          returnValue=1;
          break;
 	}
	else if( answer=="set final time" )
	{
          ps.inputString(answer,sPrintF(buff,"Enter the final time (current=%e)",tFinal));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e",&tFinal);
	    printF("New tFinal=%e \n",tFinal);
	  }
 	}
	else if( answer=="set plot interval" )
	{
          real & tPrint = parameters.dbase.get<real >("tPrint");
          ps.inputString(answer,sPrintF(buff,"Enter plot interval (current=%e)",tPrint));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e",&tPrint);
	    printF("New plot interval=%e \n",tPrint);
	  }
 	}
	else if( answer=="debug" )
	{
	  ps.inputString(answer,sPrintF(buff,"Enter debug (default value=%i)",parameters.dbase.get<int >("debug")));
	  if( answer!="" )
	    sScanF(answer,"%i",&parameters.dbase.get<int >("debug"));
	  cout << " debug=" << parameters.dbase.get<int >("debug") << endl;
	}
        else if( len=answer.matches("final time") )
	{
	  sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("tFinal"));
          dialog.setTextLabel("final time",sPrintF(answer,"%g", parameters.dbase.get<real >("tFinal"))); 
	}
        else if( len=answer.matches("times to plot") )
	{
	  sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("tPrint"));
          dialog.setTextLabel("times to plot",sPrintF(answer,"%g", parameters.dbase.get<real >("tPrint"))); 
	}
        else if( len=answer.matches("max iterations") )
	{
	  sScanF(answer(len,answer.length()-1),"%i",&parameters.dbase.get<int >("maxIterations"));
          dialog.setTextLabel("max iterations",sPrintF(answer,"%i", parameters.dbase.get<int >("maxIterations"))); 
	}
        else if( len=answer.matches("plot iterations") )
	{
	  sScanF(answer(len,answer.length()-1),"%i",&parameters.dbase.get<int >("plotIterations"));
          dialog.setTextLabel("plot iterations",sPrintF(answer,"%i", parameters.dbase.get<int >("plotIterations"))); 
	}
        else if( len=answer.matches("cfl") )
	{
	  sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("cfl"));
          dialog.setTextLabel("cfl",sPrintF(answer,"%g", parameters.dbase.get<real >("cfl"))); 
	}
        else if( len=answer.matches("implicit factor") )
	{
	  sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("implicitFactor"));
          dialog.setTextLabel("implicit factor",sPrintF(answer,"%g", parameters.dbase.get<real >("implicitFactor"))); 
	}
        else if( len=answer.matches("debug") )
	{
	  sScanF(answer(len,answer.length()-1),"%i",&parameters.dbase.get<int >("debug"));
          dialog.setTextLabel("debug",sPrintF(answer,"%i", parameters.dbase.get<int >("debug"))); 
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
          dialog.getOptionMenu(0).setCurrentChoice(component);
	  psp.set(GI_COMPONENT_FOR_CONTOURS,component);

          if( itemsToPlot & 2 )
	  {
	    plotObjects=true;
	  }
	}
	else if( answer=="adaptive grids..." )
	{
          adaptiveGridDialog.showSibling();
	}
        else if( answer=="close adaptive grid dialog" )
	{
          adaptiveGridDialog.hideSibling();
	}
	else if( answer=="file output..." )
	{
          fileOutputDialog.showSibling();
	}
        else if( answer=="close file output dialog" )
	{
          fileOutputDialog.hideSibling();
	}
        else if( adaptiveGridDialog.getToggleValue(answer,"use adaptive grids",parameters.dbase.get<bool >("adaptiveGridProblem")) )
	{
          if( parameters.dbase.get<bool >("adaptiveGridProblem") )
    	    printF("Using adaptive mesh refinement.\n");
          else
    	    printF("Not using adaptive mesh refinement.\n");
	}
	else if( len=answer.matches("error threshold") )
	{
	  ps.outputString("The error threshold should be in the range (0,1). ");
	  sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("errorThreshold"));
          adaptiveGridDialog.setTextLabel(0,sPrintF(answer,"%g", parameters.dbase.get<real >("errorThreshold")));

	  printF(" Setting errorThreshold=%9.3e\n",parameters.dbase.get<real >("errorThreshold"));
	}
	else if( answer=="pde parameters..." )
	{
	  pdeDialog.showSibling();
	}
	else if( answer=="close pde options" )
	{
	  pdeDialog.hideSibling();  // pop timeStepping
	}
	else if( parameters.setPdeParameters(gf[current].cg,answer,&pdeDialog)==0 )
	{
	  printF("Answer was found in setPdeParameters\n");
	}
	else if( answer=="time stepping params..." || answer=="time stepping parameters..." || 
                 answer=="time stepping parameters" )
	{
	  timeSteppingDialog.showSibling();
	}
	else if( answer=="close time stepping" )
	{
	  timeSteppingDialog.hideSibling();  // pop timeStepping
	}
	else if( getTimeSteppingOption(answer,timeSteppingDialog ) )
	{
	  printF("plot: answer=%s found in getTimeSteppingOption\n",(const char*)answer);
	}

// 	else if( answer=="solver parameters..." )
// 	{
// 	  solverDialog.showSibling();
// 	}
// 	else if( answer=="close solver options" )
// 	{
// 	  solverDialog.hideSibling();  // pop timeStepping
// 	}
// 	else if( setSolverParameters(answer,&solverDialog)==0 )
// 	{
// 	  printF("Answer was found in setSolverParameters\n");
// 	}

	else if( answer=="general options..." )
	{
	  generalOptionsDialog.showSibling();
	}
	else if( answer=="close general options" )
	{
	  generalOptionsDialog.hideSibling(); 
	}
	else if( getGeneralOption(answer,generalOptionsDialog ) )
	{
	  printF("Answer=%s found in getGeneralOption\n",(const char*)answer);
	}

	else if( answer=="plot options..." )
	{
	  plotOptionsDialog.showSibling();
	}
	else if( answer=="close plot options" )
	{
	  plotOptionsDialog.hideSibling(); 
	}
	else if( getPlotOption(answer,plotOptionsDialog ) )
	{
	  printF("Answer=%s found in getPlotOption\n",(const char*)answer);
	}

	else if( answer=="moving grid options..." )
	{
	  movingGridOptionsDialog.showSibling();
	}
	else if( answer=="close moving grid options" )
	{
	  movingGridOptionsDialog.hideSibling(); 
	}
	else if( getMovingGridOption(answer,movingGridOptionsDialog ) )
	{
	  printF("Answer=%s found in getMovingGridOption\n",(const char*)answer);
	}

	else if( len=answer.matches("use local time stepping") )
	{
	  sScanF(&answer[len],"%i",&parameters.dbase.get<int >("useLocalTimeStepping"));
	  dialog.setToggleState("use local time stepping",parameters.dbase.get<int >("useLocalTimeStepping"));      
	}
        else if( answer=="break" )
	{
	}
        else
	{
	  printF("DomainSolver::plot: unknown response=[%s]\n",(const char*)answer);
          ps.stopReadingCommandFile();
	}
      }
    }
  }

  if( option & 2  )
  {
    ps.redraw(true);
    
  }

  checkArrayIDs(sPrintF("plot: end") ); 

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForPlotting"))+=getCPU()-cpu0;
  return returnValue;
}
