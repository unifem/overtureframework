// -----------------------------------------------------------------------------------------------------------
// This file contains the functions:
// 
// buildRunTimeDialog()
// setSensitivity( GUIState & dialog, bool trueOrFalse );
// setTopLabel(std::vector<realCompositeGridFunction*> u, real t)
// plot(const real & t, const int & optionIn, real & tFinal )
// 
// 
// -----------------------------------------------------------------------------------------------------------


#include "Cgmp.h"
#include "AsfParameters.h"
#include "InsParameters.h"
#include "CnsParameters.h"
#include "AdParameters.h"
#include "GenericGraphicsInterface.h"
#include "App.h"
#include "ParallelUtility.h"
#include "FileOutput.h"
#include "MpParameters.h"

#include "EquationDomain.h"


static int chooseAComponentMenuItem=-1;  // this will go away

static int numberOfPushButtons=-1;  // these are for setting the sensitivity.
static int numberOfTextBoxes=-1;

// int Cgmp::
// buildPlotOptionsDialog(DialogData & dialog )
// // ==========================================================================================
// // /Description:
// //   Build the plot options dialog.
// // ==========================================================================================
// {

//   dialog.setOptionMenuColumns(1);

//   return 0;
// }


// //================================================================================
// /// \brief: Look for a plot option in the string "answer"
// ///
// /// \param answer (input) : check this command 
// ///
// /// \return return 1 if the command was found, 0 otherwise.
// //====================================================================
// int Cgmp::
// getPlotOption(const aString & answer,
// 		 DialogData & dialog )
// {

//   GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
//   GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

//   int found=false; 
//   int len=0;

//   return found;

// }


int Cgmp::
buildRunTimeDialog()
// =============================================================================================
// =============================================================================================
{
  if( parameters.dbase.get<int >("plotMode")==1 && parameters.dbase.get<int >("plotOption")==0 ) return 0;  // plotting is disabled with "no plotting"

  GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  if( parameters.dbase.get<GUIState* >("runTimeDialog")==NULL )
  {
    parameters.dbase.get<GUIState* >("runTimeDialog") = new GUIState;
    GUIState & dialog = *parameters.dbase.get<GUIState* >("runTimeDialog");
    

    dialog.setWindowTitle("cgmp");
    dialog.setExitCommand("finish", "finish");

    aString cmds[] = {"break",
                      "continue",
                      "movie mode",
                      "movie and save",
                      "contour", 
                      "streamlines",
                      "grid", 
                      "displacement",
                      "plot all",
                      "erase",
                      "change the grid...",
                      "adaptive grids...", 
                      "show file options...",
                      "file output...",
                      "pde parameters...",
                      "solver parameters...",
                      // "plot options...",
                      "",  // place holder for extra push buttons
                      ""};
    numberOfPushButtons=15;  // number of entries in cmds
    if( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL )
    {
      cmds[numberOfPushButtons]="plot distance to walls";
      numberOfPushButtons++;
    }
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    dialog.setOptionMenuColumns(1);

    // get any extra components such as errors for tz flow or the pressure for CNS.
    const int numberOfDomains = domainSolver.size();
    

    std::vector<realCompositeGridFunction> v(numberOfDomains);
    std::vector<realCompositeGridFunction*> u(numberOfDomains);
    int numberOfComponents = 0;
    ForDomain(d)
    {
      GridFunction & ud = domainSolver[d]->gf[domainSolver[d]->current];
      u[d] = &( domainSolver[d]->getAugmentedSolution(ud,v[d]) );
      numberOfComponents+= u[d]->getComponentBound(0)-u[d]->getComponentBase(0)+1;
    }

    aString *cmd = new aString[numberOfComponents+1];
    aString *label = new aString[numberOfComponents+1];
    int n=0;
    ForDomain(d)
    {
      realCompositeGridFunction & ud = *(u[d]);
      const aString & name = domainSolver[d]->getName();
      for( int m=ud.getComponentBase(0); m<=ud.getComponentBound(0); m++ )
      {
//	label[n]=ud.getName(m);
//	cmd[n]="plot:"+ud.getName(m);

        sPrintF(label[n],"%s : %s",(const char*)name,(const char*)ud.getName(m));
        sPrintF(cmd[n],"plot:%s : %s",(const char*)name,(const char*)ud.getName(m));
        n++;
      }
      
    }
    cmd[numberOfComponents]="";
    label[numberOfComponents]="";
    
    dialog.addOptionMenu("plot component:", cmd,label,0);
    delete [] cmd;
    delete [] label;


    // -- By default we plot all domains when the user chooses "grid" or "contour" etc ---
    // -- The use can also choose to only plot one domain at a time ----
    cmd = new aString[numberOfDomains+2];
    label = new aString[numberOfDomains+2];
    n=0;
    label[n]="all";  cmd[n]="plot domain: all"; n++;
    ForDomain(d)
    {
      const aString & name = domainSolver[d]->getName();

      label[n]=name; cmd[n]="plot domain: "+name; n++;
    }
    label[n]=""; cmd[n]="";
    dialog.addOptionMenu("plot domain:", cmd,label,0);
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

    const int numberOfTextStrings=6;
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

    textLabels[nt] = "dtMax";  sPrintF(textStrings[nt], "%8.2e",parameters.dbase.get<real>("dtMax"));  nt++; 
 
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
                      ""};
    int numberOfRows=4;
    fileOutputDialog.setPushButtons( cmdf, cmdf, numberOfRows );

    nt=0;
    textLabels[nt] = "output file name";  sPrintF(textStrings[nt], "%s","cgmp.out");  nt++; 
    textLabels[nt]= "restart file name";  sPrintF(textStrings[nt], "%s",(const char*)parameters.dbase.get<aString >("restartFileName"));nt++; 
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    fileOutputDialog.setTextBoxes(textLabels, textLabels, textStrings);


    // ****** pde parameters *************
    DialogData &pdeDialog = dialog.getDialogSibling();
    pdeDialog.setExitCommand("close pde options", "close");
    parameters.setPdeParameters(gf[current].cg,"build dialog",&pdeDialog);

    // ****** solver parameters *************
    DialogData &solverDialog = dialog.getDialogSibling();
    solverDialog.setExitCommand("close solver options", "close");
    setSolverParameters("build dialog",&solverDialog);


    // ****** plot options *************
    DialogData &plotOptionsDialog = dialog.getDialogSibling();
    plotOptionsDialog.setExitCommand("close plot options", "close");
    buildPlotOptionsDialog(plotOptionsDialog);


    // ****** old popup *****

    aString answer;
    const int numberOfMenuItems=25;
    aString menu0[numberOfMenuItems]=
    {
      "!cgmp",
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


//     char buffer[100];

//     // create a new menu with options for choosing a component.
//     aString *menu = new aString[numberOfMenuItems+numberOfComponents+1];
//     chooseAComponentMenuItem=-1;
//     int j=0;
//     for( int i=0; ; i++ )
//     {
//       menu[j]=menu0[i];
//       if( menu[j]=="" )
// 	break;
//       if( menu[j]==">choose a component" )
//       {
// 	chooseAComponentMenuItem=j;
// 	for( int n=0; n<numberOfComponents; n++ )
// 	{
// 	  menu[++j]=u.getName(n);
// 	  if( menu[j]=="" )
// 	    menu[j]="unknown";
	  
// 	}
//       }
//       j++;
//     }
    
//     dialog.buildPopup(menu);

//     delete [] menu;

    ps.pushGUI(dialog);
  }
  return 0;

}


void
setSensitivity( GUIState & dialog, bool trueOrFalse );


void Cgmp::
setTopLabel(std::vector<realCompositeGridFunction*> u, real t)
// ===================================================================================
// /Description:
//     Assign the top label for plots
// ===================================================================================
{
  aString topLabel="Cgmp";
  char buff[100];
//   const int numberOfDomains = domainSolver.size();
//   ForDomain(d)
//   {
//     Parameters & parameters = domainSolver[d]->parameters;
//     topLabel+= cg.getDomainName(d) + ": ";

//     // we could add a function to DomainSolver to return a good name, do this for now:
//     topLabel+= domainSolver[d]->getClassName() + " ";

//     realCompositeGridFunction & ud = *(u[d]);
//     GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
//     int component=psp.get(GI_COMPONENT_FOR_CONTOURS,component);
//     component=max(ud.getComponentBase(0),min(ud.getComponentBound(0),component));
//     topLabel+=ud.getName(component) + ", ";

//   }
  topLabel+= sPrintF(buff," t=%6.2e ",t);
  ForDomain(d)
  {
    GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
    psp.set(GI_TOP_LABEL,topLabel);
  }

}

// =============================================================================================
/// \brief Utility routine to plot contours, streamlines, grids etc. in the different domains.
// =============================================================================================
int Cgmp::
plotDomainQuantities( std::vector<realCompositeGridFunction*> u, real t )
{
  GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  typedef std::vector<int> intVector;
  intVector & plotOptions = parameters.dbase.get<DataBase >("modelData").get<intVector>("plotOptions");

  ps.erase();
  setTopLabel(u,t);
  // We build up a label by joining the labels generated by contour and streamlines which append their
  // label to the current label since we have set : psp.set(GI_LABEL_MIN_MAX,2)
  aString label="";
  int numDomains=0;
  ForDomain(d)
  {
    numDomains++;
    if( label != "" ) label+= ", ";
    
    // label+= domainSolver[d]->getClassName() + sPrintF("%i",numDomains);
    label+= domainSolver[d]->getName() + ":";

    GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
    psp.set(GI_TOP_LABEL_SUB_1,label);

    if( plotOptions[d] & 1 )
      PlotIt::plot(ps,*(u[d]->getCompositeGrid()),psp);

    if( plotOptions[d] & 2 )
      PlotIt::contour(ps,*(u[d]),psp);

    if( plotOptions[d] & 4 )
      PlotIt::streamLines(ps,*(u[d]),psp);

    if( plotOptions[d] & 8 )
      PlotIt::displacement(ps,*(u[d]),psp);

    psp.get(GI_TOP_LABEL_SUB_1,label);
  }
  return 0;
}


//  static int plotOptions = 2;   // should be in the class

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{plot}} 
int Cgmp::
plot(const real & t, 
     const int & optionIn,
     real & tFinal )
// ========================================================================================
// /Description:
//  optionIn :  0 - wait for a response
//              1 - plot and wait for a response
//              2 - do not wait for response after plotting
// /Return values: 0=normal exit. 1=user has requested "finish".
//\end{CompositeGridSolverInclude.tex}  
// ========================================================================================
{
  if( parameters.dbase.get<int >("plotMode")==1 && parameters.dbase.get<int >("plotOption")==0 ) return 0;  // plotting is disabled with "no plotting"

  const int numberOfDomains = domainSolver.size();
  typedef std::vector<int> intVector;
  if( !parameters.dbase.get<DataBase >("modelData").has_key("plotOptions") )
  {
    parameters.dbase.get<DataBase >("modelData").put<intVector>("plotOptions");
  }
  intVector & plotOptions = parameters.dbase.get<DataBase >("modelData").get<intVector>("plotOptions");
  if( plotOptions.size()!=numberOfDomains )
    plotOptions.resize(numberOfDomains,2);
  

//   std::vector<MpParameters::PlotOptionEnum> & domainPlotOption = 
//                                parameters.dbase.get<std::vector<MpParameters::PlotOptionEnum> >("domainPlotOption");
//  if( domainPlotOption.size()!=numberOfDomains )
//     domainPlotOption.resize(numberOfDomains,MpParameters::plotContour);

  int & domainToPlot = parameters.dbase.get<int>("domainToPlot");

  int option = optionIn;
  if( optionIn==0 ) 
    option=1;

  if( parameters.dbase.get<GenericGraphicsInterface* >("ps")==NULL )
    return 0;

  checkArrayIDs(sPrintF("plot: start") ); 

  int returnValue=0;

  GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  
  assert( parameters.dbase.get<GUIState* >("runTimeDialog")!=NULL );
  GUIState & dialog = *parameters.dbase.get<GUIState* >("runTimeDialog");

  aString answer;

  real cpu0=getCPU();

  char buff[100];

  // get any extra components such as errors for tz flow or the pressure for CNS.

  // **** no need to compute extra components if we are in movie mode and we are not
  //      plotting any extra component ****

  std::vector<realCompositeGridFunction> v(numberOfDomains);
  std::vector<realCompositeGridFunction*> u(numberOfDomains);
  int numberOfComponents = 0;
  ForDomain(d)
  {
    GridFunction & ud = domainSolver[d]->gf[domainSolver[d]->current];
    u[d] = &( domainSolver[d]->getAugmentedSolution(ud,v[d]) );
    numberOfComponents+= u[d]->getComponentBound(0)-u[d]->getComponentBase(0)+1;
  }
  
//   realCompositeGridFunction v;
//   realCompositeGridFunction & u = getAugmentedSolution(solution,v);  // u is either solution or v
//   const int numberOfComponents = u.getComponentBound(0)-u.getComponentBase(0)+1;

  if( movieFrame>=0   )
  { // save a ppm file as part of a movie.
    psp.set(GI_HARD_COPY_TYPE,GraphicsParameters::ppm);
    ps.outputString(sPrintF(buff,"Saving file %s%i.ppm",(const char*)movieFileName,movieFrame));
    ps.hardCopy(    sPrintF(buff,            "%s%i.ppm",(const char*)movieFileName,movieFrame),psp);
    psp.set(GI_HARD_COPY_TYPE,GraphicsParameters::postScript);
    movieFrame++;
  }

  // Get the global spatial scale (used for surface plots so that they match up across domains)
  real spatialBound=0.;
  RealArray xBound(2,3);
  ForDomain(d)
  {
    GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
    PlotIt::getGridBounds(*(u[d]->getCompositeGrid()),psp,xBound);
    spatialBound=max( spatialBound, max(xBound(End,Range(0,1))-xBound(Start,Range(0,1))) );
  }
  if( spatialBound==0. ) spatialBound=1.;
  // printf(" ---> spatialBound=%e\n",spatialBound);
  

  ForDomain(d)
  {
    GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");

    psp.set(GI_LABEL_COMPONENT,false);   // do not label components on top title
    psp.set(GI_LABEL_COLOUR_BAR,false);  // do not label the colour bar
    // psp.set(GI_LABEL_MIN_MAX,(d==0 ? 1 : 2));      // label max/min as a sub-title
    psp.set(GI_LABEL_MIN_MAX,2);      // label max/min as a sub-title, 2= append labels

    // set the global surface scale
    psp.set(GI_CONTOUR_SURFACE_SPATIAL_BOUND,spatialBound);

  }
    

  checkArrayIDs(sPrintF("plot: after getAugmented") ); 

  parameters.dbase.get<GenericGraphicsInterface* >("ps")->erase();
  if( option & 1 )
  {
    // Plot all the the things that the user has previously plotted

    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    plotDomainQuantities( u,t );

//     setTopLabel(u,t);
//     psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
//     aString label="";
//     ForDomain(d)
//     {
//       GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
//       psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
//       psp.set(GI_TOP_LABEL_SUB_1,label);

//       if( plotOptions[d] & 1 )
// 	PlotIt::plot(ps, *(u[d]->getCompositeGrid()),psp);
//       if( plotOptions[d] & 2 )
// 	PlotIt::contour(ps,*(u[d]),psp);
//       if( plotOptions[d] & 4 )
// 	PlotIt::streamLines(ps,*(u[d]),psp);
//       if( plotOptions[d] & 8 )
// 	PlotIt::displacement(ps,*(u[d]),psp);
      
//       psp.get(GI_TOP_LABEL_SUB_1,label);
      
//     }
    

    checkArrayIDs(sPrintF("plot: after initial plots") ); 

    bool programHalted=false;
    int checkForBreak=false;
    const int processorForGraphics = parameters.dbase.get<GenericGraphicsInterface* >("ps")!=NULL ? parameters.dbase.get<GenericGraphicsInterface* >("ps")->getProcessorForGraphics() : 0;
    if( option & 2  && parameters.dbase.get<GenericGraphicsInterface* >("ps")!=NULL && !(ps.readingFromCommandFile()) &&
        parameters.dbase.get<int >("myid")==processorForGraphics && parameters.dbase.get<GenericGraphicsInterface* >("ps")->isGraphicsWindowOpen() )
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
      if( parameters.dbase.get<int >("plotOption")==3 )
      {
	setSensitivity( dialog,true );
      }
      
      parameters.dbase.get<int >("plotOption")=1; // reset movie mode if set.
      movieFrame=-1;
      
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

      DialogData & adaptiveGridDialog = dialog.getDialogSibling(0);
      DialogData & fileOutputDialog = dialog.getDialogSibling(1);
      DialogData & pdeDialog = dialog.getDialogSibling(2);
      DialogData & solverDialog = dialog.getDialogSibling(3);
      DialogData & plotOptionsDialog = dialog.getDialogSibling(4);

      int len;
      for(;;)
      {
        real time1=getCPU();
	
	int menuItem = ps.getAnswer(answer,"");     

        // printF("cgmp::plot: answer=[%s]\n",(const char*)answer);

        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForWaiting"))+=getCPU()-time1;  // do not count waiting in timing

        if( len=answer.matches("plot domain: ") )
	{
	  aString answer2=answer(len,answer.length()-1);
          domainToPlot=-2;
	  if( answer2=="all" )
	  {
	    domainToPlot=-1;  // this means plot all domains
	  }
	  else
	  {
	    ForDomain(d)
	    {
	      const aString & name = domainSolver[d]->getName();
	      if( answer2==name )
	      {
		printF("plot domain: match found: domain=%i [%s]\n",d,(const char*)name);
		domainToPlot=d;

                plotOptions[d]=0;  // turn off all current plot options for this domain

		break;
	      }
	    }	  
	  }
	  if( domainToPlot==-2 )
	  {
	    printF("ERROR: unable to parse answer=[%s]\n",(const char*)answer);
            domainToPlot=-1;  // plot all domains by default
	  }
	}
        else if( answer=="plot all" )
	{
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  plotDomainQuantities( u,t );

//           ps.erase();
// 	  ForDomain(d)
// 	  {
// 	    GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
// 	    psp.set(GI_TOP_LABEL_SUB_1,label);

// 	    if( plotOptions[d] & 1 )
// 	      PlotIt::plot(ps, *(u[d]->getCompositeGrid()),psp);
// 	    if( plotOptions[d] & 2 )
// 	      PlotIt::contour(ps,*(u[d]),psp);
// 	    if( plotOptions[d] & 4 )
// 	      PlotIt::streamLines(ps,*(u[d]),psp);
// 	    if( plotOptions[d] & 8 )
// 	      PlotIt::displacement(ps,*(u[d]),psp);
      
// 	    psp.get(GI_TOP_LABEL_SUB_1,label);
      
// 	  }
	}
	else if( answer=="contour" )
	{
          if( true ) // plotOptions & 2 )
            ps.erase();
	  aString label="";
	  ForDomain(d)
	  {
            if( domainToPlot!=-1 && domainToPlot!=d ) continue;
	    
            GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
	    psp.set(GI_TOP_LABEL_SUB_1,label);            // reset the top sub label 
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	    PlotIt::contour(ps,*(u[d]),psp);

	    if( psp.getObjectWasPlotted() ) 
	      plotOptions[d] |= 2;
	    else
	      plotOptions[d] &= ~2;

            psp.get(GI_TOP_LABEL_SUB_1,label);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }
	  
          // this is not correct:
          // int component;
	  // psp.get(GI_COMPONENT_FOR_CONTOURS,component);
          // dialog.getOptionMenu("plot component:").setCurrentChoice(component);

	}
        else if( answer=="grid" )
	{
	  if( false )
	  {
            // just plot the master grid ??
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	    PlotIt::plot(ps,cg,psp);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }
	  else
	  {
	    ForDomain(d)
	    {
              if( domainToPlot!=-1 && domainToPlot!=d ) continue;

	      GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
	      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

	      PlotIt::plot(ps,*(u[d]->getCompositeGrid()),psp);
	      if( psp.getObjectWasPlotted() ) 
		plotOptions[d] |= 1;
	      else
		plotOptions[d] &= ~1;
	      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

	    }
	  }
	  
	}
	else if( answer=="streamlines" )
	{
          if( true ) // plotOptions & 2 )
            ps.erase();
	  aString label="";
	  ForDomain(d)
	  {
            if( domainToPlot!=-1 && domainToPlot!=d ) continue;

            GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

            psp.set(GI_TOP_LABEL_SUB_1,label);
	    if( domainSolver[d]->parameters.dbase.get<int >("numberOfComponents")>1 )
	    {
              // only plot streamlines if there are at least 2 components --- fix this ---
	      PlotIt::streamLines(ps,*(u[d]),psp);
	      if( psp.getObjectWasPlotted() ) 
		plotOptions[d] |= 4;
	      else
		plotOptions[d] &= ~4;
	    }
	    else
	    {
              PlotIt::contour(ps,*(u[d]),psp);
	      if( psp.getObjectWasPlotted() ) 
		plotOptions[d] |= 2;
	      else
		plotOptions[d] &= ~2;
	    }
            psp.get(GI_TOP_LABEL_SUB_1,label);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }
	  
	}
	else if( answer=="displacement" )
	{
          if( true ) // plotOptions & 2 )
            ps.erase();
	  aString label="";
	  ForDomain(d)
	  {
            if( domainToPlot!=-1 && domainToPlot!=d ) continue;

            Parameters & parameters = domainSolver[d]->parameters;
            GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
	    psp.set(GI_TOP_LABEL_SUB_1,label);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

	    const int & u1c =  parameters.dbase.get<int >("u1c"); // hemp code uses this for the displacement ** fix this somehow **
	    bool & methodComputesDisplacements = parameters.dbase.get<bool>("methodComputesDisplacements");
	    if( u1c>=0  || methodComputesDisplacements )  
	    {
	      if( u1c<0 )
	      {
		const int & uc =  parameters.dbase.get<int >("uc");
		const int & vc =  parameters.dbase.get<int >("vc");
		const int & wc =  parameters.dbase.get<int >("wc");
		psp.set(GI_DISPLACEMENT_U_COMPONENT,uc);
		psp.set(GI_DISPLACEMENT_V_COMPONENT,vc);
		psp.set(GI_DISPLACEMENT_W_COMPONENT,wc);
	      }
	      else
	      {
		const int & u2c =  parameters.dbase.get<int >("u2c");
		const int & u3c =  parameters.dbase.get<int >("u3c");
		psp.set(GI_DISPLACEMENT_U_COMPONENT,u1c);
		psp.set(GI_DISPLACEMENT_V_COMPONENT,u2c);
		psp.set(GI_DISPLACEMENT_W_COMPONENT,u3c);
	      }

	      PlotIt::displacement(ps,*(u[d]),psp);
	      if( psp.getObjectWasPlotted() ) 
		plotOptions[d] |= 8;
	      else
		plotOptions[d] &= ~8;
	    }
            else
	    {
	      PlotIt::plot(ps,*(u[d]->getCompositeGrid()),psp); // plot the grid if method does not compute displacements
	      if( psp.getObjectWasPlotted() ) 
		plotOptions[d] |= 1;
	      else
		plotOptions[d] &= ~1;
	    }

            psp.get(GI_TOP_LABEL_SUB_1,label);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  }

	}
	else if( answer=="erase" )
	{
          ps.erase();
	  ForDomain(d)
          { 
            if( domainToPlot!=-1 && domainToPlot!=d ) continue;
            plotOptions[d]=0; 
	  }
	  
	}
	else if( answer=="continue" )
	{
	  if( !parameters.isSteadyStateSolver() )
	  {
	    if( t >= tFinal-dt/10. )
	    {
	      printf("WARNING: t=tFinal. Choose `finish' if you really want to end\n");
	    }
	    else
	      break;
	  }
	  else
	  {
            if( parameters.dbase.get<int >("globalStepNumber")+1>=parameters.dbase.get<int >("maxIterations") )
	    {
	      printf("WARNING: %i steps (maxIterations=%i) have been taken. Choose `finish' if you really want to end\n",
                parameters.dbase.get<int >("globalStepNumber")+1,parameters.dbase.get<int >("maxIterations") );
	    }
	    else
              break;
	  }
	  
 	}
	else if( answer=="movie mode" )
	{
          parameters.dbase.get<int >("plotOption")= 3;  // don't wait

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
          parameters.dbase.get<int >("plotOption")=3;  // don't wait

  	  setSensitivity( dialog,false );
          break;
	}
	else if( answer=="turn on adaptive grids" )
	{
	  parameters.dbase.get<bool >("adaptiveGridProblem")=true;
	  printf("Using adaptive mesh refinement.\n");
	}
	else if( answer=="turn off adaptive grids" )
	{
	  parameters.dbase.get<bool >("adaptiveGridProblem")=false;
	  printf("Do NOT use adaptive mesh refinement.\n");
	}
        else if( adaptiveGridDialog.getTextValue(answer,"error threshold","%f",parameters.dbase.get<real >("errorThreshold")) )
	{
	  cout << " parameters.errorThreshold=" << parameters.dbase.get<real >("errorThreshold") << endl;
	}
        else if( adaptiveGridDialog.getTextValue(answer,"regrid frequency","%i",parameters.dbase.get<int >("amrRegridFrequency")) )
	{
	  cout << " parameters.amrGridFrequency=" << parameters.dbase.get<int >("amrRegridFrequency") << endl;
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
	    printf("New tFinal=%e \n",tFinal);
	  }
 	}
	else if( answer=="set plot interval" )
	{
          real & tPrint = parameters.dbase.get<real >("tPrint");
          ps.inputString(answer,sPrintF(buff,"Enter plot interval (current=%e)",tPrint));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e",&tPrint);
	    printf("New plot interval=%e \n",tPrint);
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
	else if( dialog.getTextValue(answer,"dtMax","%e", parameters.dbase.get<real >("dtMax")) )
	{
	  printF("Cgmp: Setting dtMax=%8.2e for all domains\n",parameters.dbase.get<real >("dtMax"));
	  ForDomain(d)
	  {
	    domainSolver[d]->parameters.dbase.get<real>("dtMax")=parameters.dbase.get<real >("dtMax");
	  }
	  
	}
	else if( len=answer.matches("plot:") )
	{
          // plot a new component: the answer is of the form 
          //      solverName : componentName
          aString line = answer(len,answer.length()-1);
          int i=0;
	  while( i<line.length() && line[i]!=':' ) i++;  // look for ":"
          if( i==0 )
	  {	    
            printF("plot: unknown plot command=[%s], (expecting a `:')\n",(const char*)answer);
	    ps.stopReadingCommandFile();
	    break;
         
	  }
          int j=i-1;
	  while( j>0 && line[j]==' ') j--;  // skip trailing blanks
          aString solverName = line(0,j);

          // now look for the component name 
          i++;
	  while( i<line.length() && line[i]==' ' ) i++;  // skip initial blanks
	  aString name=line(i,line.length()-1);
       
          // printF("plot: solverName=[%s], name=[%s]\n",(const char*)solverName, (const char*)name);

          int domain=-1;
	  ForDomain(d)
	  {
	    if( domainSolver[d]->getName() == solverName )
	    {
	      domain=d;
	      break;
	    }
	  }
	  if( domain==-1 )
	  {
	    printF("plot: unknown solverName=[%s]\n",(const char*)solverName);
	    ps.stopReadingCommandFile();
	    break;
	  }
	  
	  realCompositeGridFunction & ud = *(u[domain]);
	  
          int component=-1;
	  for( int n=ud.getComponentBase(0); n<=ud.getComponentBound(0); n++ )
	  {
	    if( name==ud.getName(n) )
	    {
	      component=n;
	      break;
	    }
	  }
          if( component==-1 )
	  {
            printf("ERROR: unknown component name =[%s]\n",(const char*)name);
	    ps.stopReadingCommandFile();
	    break;
	  }

          // printf("  >>>plot: domain=%i, component=%i\n",domain,component);

          // count up the number of components in the domains before "domain"
          int cc=0;
	  ForDomain(d)
	  {
	    if( d==domain ) break;
            cc+= u[d]->getComponentBound(0) - u[d]->getComponentBase(0) +1;
	  }
          cc+=component;
          dialog.getOptionMenu("plot component:").setCurrentChoice(cc);

          // here we change the component to be plotted
	  GraphicsParameters & psp = domainSolver[domain]->parameters.dbase.get<GraphicsParameters >("psp");            
	  psp.set(GI_COMPONENT_FOR_CONTOURS,component);

          psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	  plotDomainQuantities( u,t );

//           ps.erase();
// 	  setTopLabel(u,t);
//           label="";
// 	  ForDomain(d)
// 	  {
//             GraphicsParameters & psp = domainSolver[d]->parameters.dbase.get<GraphicsParameters >("psp");
// 	    psp.set(GI_TOP_LABEL_SUB_1,label);

// 	    if( plotOptions[d] & 1 )
// 	      PlotIt::plot(ps,*(u[d]->getCompositeGrid()),psp);

//             if( plotOptions[d] & 2 )
// 	      PlotIt::contour(ps,*(u[d]),psp);

// 	    if( plotOptions[d] & 4 )
// 	      PlotIt::streamLines(ps,*(u[d]),psp);

// 	    if( plotOptions[d] & 8 )
// 	      PlotIt::displacement(ps,*(u[d]),psp);

// 	    psp.get(GI_TOP_LABEL_SUB_1,label);
// 	  }
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

	else if( answer=="plot options..." )
	{
          plotOptionsDialog.showSibling();
	}
        else if( answer=="close plot options dialog" )
	{
          plotOptionsDialog.hideSibling();
	}
	else if( getPlotOption(answer,plotOptionsDialog ) )
	{
	  printF("Cgmp::plot answer=%s found in getPlotOption\n",(const char*)answer);
	}
        else if( adaptiveGridDialog.getToggleValue(answer,"use adaptive grids",parameters.dbase.get<bool >("adaptiveGridProblem")) )
	{
          if( parameters.dbase.get<bool >("adaptiveGridProblem") )
    	    printf("Using adaptive mesh refinement.\n");
          else
    	    printf("Not using adaptive mesh refinement.\n");
	}
	else if( len=answer.matches("error threshold") )
	{
	  ps.outputString("The error threshold should be in the range (0,1). ");
	  sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("errorThreshold"));
          adaptiveGridDialog.setTextLabel(0,sPrintF(answer,"%g", parameters.dbase.get<real >("errorThreshold")));
	  cout << " parameters.errorThreshold=" << parameters.dbase.get<real >("errorThreshold") << endl;
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
	  printf("Answer was found in setPdeParameters\n");
	}
	else if( answer=="solver parameters..." )
	{
	  solverDialog.showSibling();
	}
	else if( answer=="close solver options" )
	{
	  solverDialog.hideSibling();  // pop timeStepping
	}
	else if( setSolverParameters(answer,&solverDialog)==0 )
	{
	  printf("Answer was found in setSolverParameters\n");
	}
	else if( len=answer.matches("use local time stepping") )
	{
	  sScanF(&answer[len],"%i",&parameters.dbase.get<int >("useLocalTimeStepping"));
	  dialog.setToggleState("use local time stepping",parameters.dbase.get<int >("useLocalTimeStepping"));      
	}
        else if( answer=="break" )
	{
	}
	else if ( parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").matchAndSetValue( answer ) )
        {
        }
        else
	{
	  bool found = false;
	  ForDomain(d)
	  { // !!!! kkc 070607 NEED TO FIX THIS
	    if( domainSolver[d]->parameters.setPdeParameters(domainSolver[d]->gf[current].cg,answer,&solverDialog)==0 )
	    {
	      found = true;
	    }
	  }
	  if ( !found ) 
	  {
	    cout << "Unknown response: " << answer << endl;
	    ps.stopReadingCommandFile();
	  }
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
