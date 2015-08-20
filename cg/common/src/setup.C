#include "DomainSolver.h"
#include "NameList.h"
#include "GenericGraphicsInterface.h"
#include "MaterialProperties.h"
#include "Chemkin.h"
#include "Ogshow.h"
#include "Oges.h"
#include "OgesParameters.h"
#include "SparseRep.h"
#include "MovingGrids.h"
#include "FileOutput.h"
#include "Regrid.h"
#include "ErrorEstimator.h"
#include "InterpolateRefinements.h"
#include "GridStatistics.h"

#include "EquationDomain.h"
#include "SurfaceEquation.h"
#include "MatrixMotion.h"
#include "RigidBodyMotion.h"
#include "DeformingBodyMotion.h"

#include "ProbeInfo.h"

#include "Controller.h"

int readRestartFile(GridFunction & cgf, Parameters & parameters,
                    const aString & restartFileName =nullString );

// ===================================================================================================================
/// \brief Display DomainSolver parameters.
/// \param file (input) : output info to this file.
///
// ===================================================================================================================
int DomainSolver::
displayParameters(FILE *file /* = stdout */ )
{
  
  const char *offOn[2] = {  "off","on" };
  char buff[100];
  
  fprintf(file,
          "twilight zone: twilight zone is %s.\n"
	  "               polynomial is %s. degree in space=%i, degree in time=%i\n"
	  "               trigonometric is %s. frequencies (fx,fy,fz,ft)=(%f,%f,%f,%f)\n"
          "               use 2D function in 3D is %s\n",
          offOn[int(parameters.dbase.get<bool >("twilightZoneFlow")==true)],
          offOn[int(parameters.dbase.get<bool >("twilightZoneFlow")==true && parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==Parameters::polynomial)],
	  parameters.dbase.get<int >("tzDegreeSpace"),parameters.dbase.get<int >("tzDegreeTime"),
          offOn[int(parameters.dbase.get<bool >("twilightZoneFlow")==true && parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==Parameters::trigonometric)],
	  parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0],parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1],parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2],parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3],
	  offOn[int(parameters.dbase.get<int >("dimensionOfTZFunction")==2)]);
  
  fprintf(file,
          "time stepping options: final time=%e, cfl=%f, dtMax=%e, recompute dt interval=%i,\n"
          "    implicit factor=%f, (.5=Crank-Nicolson, 1.=Backward Euler)\n"
          "    slow start time interval=%f, slow start cfl=%f\n"
          "    time stepping method=%s \n",
	  parameters.dbase.get<real >("tFinal"),parameters.dbase.get<real >("cfl"),parameters.dbase.get<real >("dtMax"), parameters.dbase.get<int >("maximumStepsBetweenComputingDt"),
          parameters.dbase.get<real >("implicitFactor"),parameters.dbase.get<real >("slowStartTime"),parameters.dbase.get<real >("slowStartCFL"),
	  (const char*)parameters.getTimeSteppingName());

  real tolerance;
  int maximumNumberOfIterations;
  implicitTimeStepSolverParameters.get(OgesParameters::THEtolerance,tolerance);
  implicitTimeStepSolverParameters.get(OgesParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
	
  fprintf(file,"  If implicit time stepping is on, here is how each grid will be treated:\n");
  fprintf(file,"    implicit solver =%s, tolerance=%e, max number of iterations=%s \n",
	  (const char*)implicitTimeStepSolverParameters.getSolverName(),tolerance,
	  maximumNumberOfIterations==0 ? "default" : 
	  sPrintF(buff,"%i",maximumNumberOfIterations));

  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
// *** scLC
    fprintf(file,"    %20s is time integrated %s \n",(const char *)cg[grid].getName(),
	    (parameters.getGridIsImplicit(grid)==1 ? "implicitly" : 
	     (parameters.getGridIsImplicit(grid)==2 ? "semi-implicitly" :
	      "explicitly")));
// *** ecLC
  }

  displayBoundaryConditions(file);

  fprintf(file,
          "output options: times to plot=%e, plotOption=%s\n" 
          "    save a restart file is %s (A show file can also be used as a restart file)\n",
          parameters.dbase.get<real >("tPrint"),
          parameters.dbase.get<int >("plotOption")==0 ? "no plotting" :  
          parameters.dbase.get<int >("plotOption")==1 ? "plot and always wait" :
          parameters.dbase.get<int >("plotOption")==2 ? "plot with no waiting" : "plot and wait first time",
	  offOn[int(parameters.dbase.get<bool >("saveRestartFile")==true)]);

  fprintf(file,
         "initial conditions: (need to be specified when not using twilight zone flow)\n"
         "  project initial conditions is %s\n",
         offOn[int(parameters.dbase.get<bool >("projectInitialConditions"))]);
  
  parameters.displayPdeParameters(file);
  
  fprintf(file,
          "axisymmetric flow is %s. cyindrical axis is the %s axis\n",
          offOn[int(parameters.dbase.get<bool >("axisymmetricProblem")==true)],
          (parameters.dbase.get<int >("radialAxis")==0 ? "x" : "y"));
  fprintf(file,
          "Adaptive mesh refinement is %s\n",
          offOn[int(parameters.dbase.get<bool >("adaptiveGridProblem")==true)]);
  fprintf(file,
          "AMR error function option = %i (1=use top-hat function)\n",
          parameters.dbase.get<int>("amrErrorFunctionOption"));

  
  if( parameters.dbase.get<bool >("adaptiveGridProblem") )
  {
    if( parameters.dbase.get<bool >("useDefaultErrorEstimator") )
      fprintf(file,"Use default error estimator. ");
    if( parameters.dbase.get<bool >("useUserDefinedErrorEstimator") )
      fprintf(file,"Use the user defined error estimator.");
    fprintf(file,"\n");
    
    fprintf(file,"  plotting of the amr error function is %s\n",
	    offOn[int(parameters.dbase.get<int >("showAmrErrorFunction")==true)]);
    
    if( parameters.dbase.get<Regrid* >("regrid")==NULL )
      parameters.dbase.get<Regrid* >("regrid") = new Regrid();
 
    parameters.dbase.get<Regrid* >("regrid")->displayParameters(file);  // *************** finish

    if( parameters.dbase.get<ErrorEstimator* >("errorEstimator")==NULL )
      parameters.buildErrorEstimator();
      
    parameters.dbase.get<ErrorEstimator* >("errorEstimator")->displayParameters(file);  // *************** finish

  }
  fprintf(file,
	  "Debugging: \n"
	  "  debug=%i   (bit flag)\n"
	  "  info=%i    (bit flag)\n"
	  "  Oges::debug=%i (for sparse solvers)\n"
	  "  Reactions::debug=%i (for chemical reactions)\n"
          "  checkForFloatingPointErrors=%i\n"
	  "  compare 3D run to 2D is %s\n",
	  parameters.dbase.get<int >("debug"),parameters.dbase.get<int >("info"),Oges::debug,Reactions::debug,
          Parameters::checkForFloatingPointErrors,
          offOn[int(parameters.dbase.get<int >("compare3Dto2D")==true)]);
  
  if( cg.rcData->interpolant!=NULL )
  {
    fprintf(file,
	    "use iterative implicit interpolation is %s.\n",
	    offOn[int(cg.rcData->interpolant->getImplicitInterpolationMethod()==Interpolant::iterateToInterpolate)]);
  }
  
  fprintf(file,
          "reduced interpolation width is %s, reduced width is %i. \n",
	  offOn[int(parameters.dbase.get<int >("reducedInterpolationWidth")>0)],
          parameters.dbase.get<int >("reducedInterpolationWidth"));

  fprintf(file," Moving grids are turned %s. \n",offOn[int(parameters.isMovingGridProblem()==true)]);
  fprintf(file,
	  "  collision detection is %s.\n"
	  "  minimum separation for collisions is %f (grid lines).\n",
	  offOn[int(parameters.dbase.get<bool >("detectCollisions"))],
	  parameters.dbase.get<real >("collisionDistance") );
  if( parameters.isMovingGridProblem() )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      if( parameters.gridIsMoving(grid) )
      {
	fprintf(file,"  Grid %15s is moving : %s\n",(const char*)cg[grid].getName(),
		(const char*)parameters.dbase.get<MovingGrids >("movingGrids").movingGridOptionName(
		  parameters.dbase.get<MovingGrids >("movingGrids").movingGridOption(grid)));
      }
  }


  return 0;
}






// ===================================================================================================================
/// \brief Save the original boundary conditions from the CompositeGrid.
/// \param cg (input) : CompositeGrid.
/// \param originalBoundaryCondition (input) : save boundary conditions in this array, 
///    originalBoundaryCondition(0:1,0:2,0:ngd-1), ngd= number of grids.
// ===================================================================================================================
int DomainSolver::    
getOriginalBoundaryConditions(CompositeGrid & cg, IntegerArray & originalBoundaryCondition )
{
  originalBoundaryCondition.redim(2,3,cg.numberOfComponentGrids());
  const int numberOfDimensions = cg.numberOfDimensions();

  int grid, side,axis;
  char buff[80];
  Range all;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    originalBoundaryCondition(all,all,grid)=cg[grid].boundaryCondition();
    
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( cg[grid].boundaryCondition()(side,axis) != 0 )
	{
	  if( cg[grid].numberOfGhostPoints(side,axis)<2 )
	  {
	    printF("DomainSolver:ERROR: The grid must be made with numberOfGhostPoints>=2 on all boundaries\n");
            display(cg[grid].boundaryCondition(),sPrintF(buff,"boundaryCondition on grid %i\n",grid));
            display(cg[grid].dimension(),sPrintF(buff,"dimension on grid %i\n",grid));
            display(cg[grid].indexRange(),sPrintF(buff,"indexRange on grid %i\n",grid));
            display(cg[grid].gridIndexRange(),sPrintF(buff,"gridIndexRange on grid %i\n",grid));
	    Overture::abort("error");
	  }
          if( abs(cg[grid].dimension(side,axis)-cg[grid].indexRange(side,axis))<2 )
	  {
	    printF("DomainSolver:ERROR: The grid must be made with numberOfGhostPoints>=2 on all boundaries\n");
            cg[grid].dimension().display(sPrintF(buff,"dimension on grid %i\n",grid));
            cg[grid].indexRange().display(sPrintF(buff,"indexRange on grid %i\n",grid));
            cg[grid].gridIndexRange().display(sPrintF(buff,"gridIndexRange on grid %i\n",grid));
	    
	    Overture::abort("error");
	  }
	}
      }
    }
  }
}

// ===================================================================================================================
/// \brief Build the dialog that shows the various forcing options
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DomainSolver::
buildForcingOptionsDialog(DialogData & dialog )
{
  aString pbCommands[] = {"body forcing...",
                          "user defined forcing...",
                          "user defined material properties...",
                          "controls...",
			  ""};
  aString *pbLabels = pbCommands;
  int numRows=2;
  dialog.setPushButtons( pbCommands, pbLabels, numRows ); 

  return 0;
}

//================================================================================
/// \brief: Look for a forcing option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DomainSolver::
getForcingOption(const aString & answer,
		 DialogData & dialog )
{
  return false;
}

// ===================================================================================================================
/// \brief Build the plot options dialog.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DomainSolver::
buildPlotOptionsDialog(DialogData & dialog )
{

  aString cmd[] = {"plot and wait first time", "plot with no waiting",
		   "plot and always wait","no plotting","" };

  int & plotOption = parameters.dbase.get<int>("plotOption");
  int & plotMode   = parameters.dbase.get<int>("plotMode");
  dialog.addOptionMenu("plot option", cmd, cmd, plotOption);

  aString pbCommands[] = {"plot parallel dist.",
			  "plot material properties",
                          "plot body force mask",
                          "forcing regions plot options",
			  ""};
  aString *pbLabels = pbCommands;
  int numRows=4;
  dialog.setPushButtons( pbCommands, pbLabels, numRows ); 


  aString tbCommands[] = {"disable plotting",
			  "plot residuals",
                          "adjust grid for displacement",
                          "plot grid velocity", 
                          "plot body force mask surface",
                          "plot structures",  // plot beams and shells
                          "plot body force",
                          "output yPlus",
			  ""};
  int tbState[10];
  tbState[0] = plotMode==1;
  tbState[1] = parameters.dbase.get<int>("showResiduals"); 
  tbState[2] = parameters.dbase.get<int>("adjustGridForDisplacement"); 
  tbState[3] = parameters.dbase.get<int>("plotGridVelocity"); 
  tbState[4] = parameters.dbase.get<bool>("plotBodyForceMaskSurface");
  tbState[5] = parameters.dbase.get<bool>("plotStructures");
  tbState[6] = parameters.dbase.get<bool>("plotBodyForce");
  tbState[7] = parameters.dbase.get<bool>("outputYplus");
  

  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  if( !parameters.isSteadyStateSolver() )
  {
    textCommands[nt] = "times to plot";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%g", parameters.dbase.get<real >("tPrint"));  nt++; 
  }
  else
  {
    textCommands[nt] = "plot iterations";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%i", parameters.dbase.get<int >("plotIterations"));  nt++; 
  }
    
  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textLabels, textStrings);


  return 0;
}

//================================================================================
/// \brief: Look for a plot option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DomainSolver::
getPlotOption(const aString & answer,
		 DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int & plotOption = parameters.dbase.get<int>("plotOption");
  int & plotMode   = parameters.dbase.get<int>("plotMode");

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  if( answer=="plot and wait first time" )
  {
    if( plotMode==0 )
      plotOption=3;
    else
      printF("Not changing the plot option since plotting is disabled (plotMode=%i).\n",plotMode);

   dialog.getOptionMenu("plot option").setCurrentChoice(0);
  }
  else if( answer=="plot with no waiting" )
  {
    if( plotMode==0 ) 
      plotOption=2;
    else
      printF("Not changing the plot option since plotting is disabled (plotMode=%i).\n",plotMode);
   dialog.getOptionMenu("plot option").setCurrentChoice(1);
  }
  else if( answer=="plot and always wait" )
  {
    if( plotMode==0 ) 
      plotOption=1;
    else
      printF("Not changing the plot option since plotting is disabled (plotMode=%i).\n",plotMode);
   dialog.getOptionMenu("plot option").setCurrentChoice(2);
  }
  else if( answer=="no plotting" )
  {
    plotOption=0;
   dialog.getOptionMenu("plot option").setCurrentChoice(3);
  }
  else if( dialog.getToggleValue(answer,"disable plotting",plotMode) ){} //
  else if( dialog.getToggleValue(answer,"plot residuals",parameters.dbase.get<int >("showResiduals")) ){} //
  else if( dialog.getToggleValue(answer,"plot grid velocity",parameters.dbase.get<int >("plotGridVelocity")) ){} //
  else if( answer=="times to plot (tp=)" ) // for backward compatibility
  {
    gi.inputString(answer2,sPrintF(buff,"Enter the time between plotting (default value=%e)",parameters.dbase.get<real >("tPrint")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("tPrint"));
    printF(" tPrint=%9.3e\n",parameters.dbase.get<real >("tPrint"));
  }
  else if( dialog.getTextValue(answer,"times to plot","%e",parameters.dbase.get<real >("tPrint")) ){} //
  else if( dialog.getTextValue(answer,"plot iterations","%i",parameters.dbase.get<int >("plotIterations")) ){} //
  else if( dialog.getToggleValue(answer,"adjust grid for displacement",parameters.dbase.get<int >("adjustGridForDisplacement")) )
  {
    psp.set(GI_ADJUST_GRID_FOR_DISPLACEMENT,parameters.dbase.get<int >("adjustGridForDisplacement"));
  }
  else if( dialog.getToggleValue(answer,"plot body force mask surface",parameters.dbase.get<bool >("plotBodyForceMaskSurface")) )
  {
    printF("plotBodyForceMaskSurface=%i : You should choose `plot body force mask' to initialize the isosurface.\n",
           (int)parameters.dbase.get<bool >("plotBodyForceMaskSurface"));
  }
  else if( dialog.getToggleValue(answer,"plot structures",parameters.dbase.get<bool >("plotStructures")) )
  {
    if( parameters.dbase.get<bool >("plotStructures") )
      printF("Plotting the center lines of any beams or shells.\n");
  }
  else if( dialog.getToggleValue(answer,"plot body force",parameters.dbase.get<bool >("plotBodyForce")) )
  {
    if( parameters.dbase.get<bool >("plotBodyForce") )
      printF("Plotting the body force (if it is turned on).\n");
  }
  else if( dialog.getToggleValue(answer,"output yPlus",parameters.dbase.get<bool >("outputYplus")) )
  {
    if( parameters.dbase.get<bool >("outputYplus") )
      printF("Output information on y+ (for turbulence computations).\n");
  }
  

  // -- these next options are handled in plot(..)
  //   else if( answer=="plot parallel dist." )
  //   }
  //   else if( answer=="plot material properties" )
  //   {
  //   }

  else
  {
    found=false;
  }
  

  return found;
}

// ===================================================================================================================
/// \brief Build the output options dialog.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DomainSolver::
buildOutputOptionsDialog(DialogData & dialog )
{

  aString tbCommands[] = {"save a restart file",
			  "allow user defined output",
			  ""};
  int tbState[10];
  tbState[0] = parameters.dbase.get<bool >("saveRestartFile"); 
  tbState[1] = parameters.dbase.get<int >("allowUserDefinedOutput"); 
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  aString pbCommands[] = {"show file options...",
                          "create a probe...",
                          "check probes...",
                          "output periodically to a file",
			  ""};
  aString *pbLabels = pbCommands;
  int numRows=3;
  dialog.setPushButtons( pbCommands, pbLabels, numRows ); 

  bool buildDialog=true;
  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "check file cutoffs";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i %8.1e",0,(parameters.dbase.get<RealArray>("checkFileCutoff"))(0));  nt++;  

  textCommands[nt] = "debug";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("debug"));  nt++;

  textCommands[nt] = "info flag";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("info"));  nt++;

  textCommands[nt] = "output format";  textLabels[nt]=textCommands[nt];
  textStrings[nt]=parameters.dbase.get<aString >("outputFormat");  nt++;

  textCommands[nt] = "frequency to save probes";  textLabels[nt]=textCommands[nt];
  textStrings[nt]=sPrintF(textStrings[nt], "%i",parameters.dbase.get<int>("probeFileFrequency")); nt++;

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}

//================================================================================
/// \brief: Look for an output option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DomainSolver::
getOutputOption(const aString & answer,
		 DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int & plotOption = parameters.dbase.get<int>("plotOption");
  int & plotMode   = parameters.dbase.get<int>("plotMode");

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  if( dialog.getToggleValue(answer,"save a restart file",parameters.dbase.get<bool >("saveRestartFile") ) )
  {
    if( parameters.dbase.get<bool >("saveRestartFile") )
      printF("A restart file will be saved. Actually, two files will be saved, `ob1.restart' and `ob2.restart',\n"
              "just in case the program crashes while writing the restart file. \n"                  
              "At least one of these files should be valid for restarting. \n"
	     "You can run cg using a restart file for initial conditions\n");
  }
  else if( dialog.getToggleValue(answer,"allow user defined output",parameters.dbase.get<int >("allowUserDefinedOutput") ) ){} //
  // else if( answer=="output periodically to a file" ) do not trap this here
  else if( answer=="show file options..." )
  {
    parameters.updateShowFile();
  }
  else if( answer=="create a probe..." || answer=="create a probe" )
  {
    // create a new probe : these are points or regions on the grid whose info is saved periodically to a file

   if(!parameters.dbase.has_key("probeList") ) parameters.dbase.put<std::vector<ProbeInfo*> >("probeList");

    std::vector<ProbeInfo* > & probeList = parameters.dbase.get<std::vector<ProbeInfo*> >("probeList");
    ProbeInfo & probe = *( new ProbeInfo(parameters) );
    probeList.push_back(&probe);
    probe.update( cg,gi );
    
  }
  else if( answer=="check probes..." )
  {
    // this routine performs regression tests on the probe evaluation routines.
    checkProbes();
  }
  else if( answer=="debug" ) // old way
  {
    gi.inputString(answer2,sPrintF(buff,"Enter debug (default value=%i)",parameters.dbase.get<int >("debug")));
    if( answer2!="" )
      sScanF(answer2,"%i",&parameters.dbase.get<int >("debug"));
    printF(" debug=%i\n",parameters.dbase.get<int >("debug"));
  }
  else if( dialog.getTextValue(answer,"debug","%i",parameters.dbase.get<int >("debug")) ){} //
  else if( dialog.getTextValue(answer,"info flag","%i",parameters.dbase.get<int >("info")) ){} //
  else if( dialog.getTextValue(answer,"output format","%s",parameters.dbase.get<aString >("outputFormat")) ){}//
  else if( dialog.getTextValue(answer,"frequency to save probes","%i",parameters.dbase.get<int>("probeFileFrequency")) ){}//
  else if( len=answer.matches("check file cutoffs") )
  {
    gi.outputString("Specify cutoffs for the check file. Enter a component number and value");
    int n=0;
    sScanF(answer(len,answer.length()-1),"%i",&n);
    sScanF(answer(len,answer.length()-1),"%i %e",&n,&parameters.dbase.get<RealArray>("checkFileCutoff")(n));
    dialog.setTextLabel("check file cutoffs",sPrintF(buff,"%i %8.1e ",n,parameters.dbase.get<RealArray>("checkFileCutoff")(n)));
    gi.outputString(sPrintF(buff,"Setting cutoff for component %i to %e\n",n,parameters.dbase.get<RealArray>("checkFileCutoff")(n)));
  }
  else
  {
    found=false;
  }
  

  return found;
}


// ===================================================================================================================
/// \brief Build the dialog that shows the various general options
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DomainSolver::
buildGeneralOptionsDialog(DialogData & dialog )
{
  aString pbCommands[] = {"pressure solver options",
			  "implicit time step solver options",
			  ""};
  aString *pbLabels = pbCommands;
  int numRows=2;
  dialog.setPushButtons( pbCommands, pbLabels, numRows ); 


  aString tbCommands[] = {"axisymmetric flow",
                          "iterative implicit interpolation",
                          "check for floating point errors",
                          "use interactive grid generator",
                          "use new time-stepping startup",
			  ""};
  int tbState[10];
  tbState[0] = parameters.dbase.get<bool >("axisymmetricProblem"); 
  tbState[1] = cg.rcData->interpolant!=NULL ? 
               cg.rcData->interpolant->getImplicitInterpolationMethod()==Interpolant::iterateToInterpolate  : 0;
  tbState[2] = Parameters::checkForFloatingPointErrors;
  tbState[3] = parameters.dbase.get<bool >("useInteractiveGridGenerator");
  tbState[4] = parameters.dbase.get<bool>("useNewTimeSteppingStartup");
  
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  // ----- Text strings ------
  const int numberOfTextStrings=10;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "maximum iterations for implicit interpolation";
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation"));  nt++;

  int width=max(cg.interpolationWidth);
  textCommands[nt] = "reduce interpolation width";  sPrintF(textStrings[nt], "%i",width);  nt++;

  textCommands[nt] = "velocity scale";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real>("velocityScale"));  nt++;
  textCommands[nt] = "target grid spacing";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real>("targetGridSpacing")); nt++;

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);
}

//================================================================================
/// \brief: Look for a general option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DomainSolver::
getGeneralOption(const aString & answer,
		 DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  bool iterativeImplicitInterpolation=true;
  int width=-1;

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  if( answer=="pressure solver options" )
  {
    pressureSolverParameters.update(gi,cg);
  }
  else if( answer=="implicit time step solver options" )
  {
    implicitTimeStepSolverParameters.update(gi,cg);
  }
  else if( dialog.getToggleValue(answer,"axisymmetric flow",parameters.dbase.get<bool >("axisymmetricProblem") ) ){}//
  else if( dialog.getToggleValue(answer,"iterative implicit interpolation",iterativeImplicitInterpolation) )
  {
    if( cg.rcData->interpolant!=NULL )
    {
      if( iterativeImplicitInterpolation )
	cg.rcData->interpolant->setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
      else
	cg.rcData->interpolant->setImplicitInterpolationMethod(Interpolant::directSolve);
    }
  }
  else if( dialog.getToggleValue(answer,"check for floating point errors",Parameters::checkForFloatingPointErrors ) ){}//
  else if( dialog.getToggleValue(answer,"use interactive grid generator",
                                 parameters.dbase.get<bool >("useInteractiveGridGenerator") ) )
  {
    if(  parameters.dbase.get<bool >("useInteractiveGridGenerator") )
    printF("For moving grids I will use the interactive Ogen grid generator so that you\n"
           "can interactively step through the grid generation steps.\n");
  }
  else if( dialog.getToggleValue(answer,"use new time-stepping startup",
           parameters.dbase.get<bool>("useNewTimeSteppingStartup")) ){} //

  else if( dialog.getTextValue(answer,"maximum iterations for implicit interpolation","%i",
                               parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation")) ){}//
  else if( dialog.getTextValue(answer,"velocity scale","%e",parameters.dbase.get<real>("velocityScale")) )
  {
    printF("Setting velocityScale=%9.3e. This is used to compute the output performance measure TTS.\n",
            parameters.dbase.get<real>("velocityScale"));
  }
  else if( dialog.getTextValue(answer,"target grid spacing","%e",parameters.dbase.get<real>("targetGridSpacing")) )
  {
    printF("Setting target grid spacing=%9.3e. This is used to compute the output performance measure TTS.\n",
            parameters.dbase.get<real>("targetGridSpacing"));
  }
  else if( answer=="reduce interpolation width" ) // for backward compat.
  {
    int width, oldWidth=max(cg.interpolationWidth);
    gi.inputString(answer2,sPrintF(buff,"Enter new interpolation width (should <= %i)",oldWidth));
    if( answer2!="" )
    {
      sScanF(answer2,"%i",&width);
      if( width<oldWidth )
      {
	printF("Changing width to %i\n",width);
	cg.changeInterpolationWidth(width);
	parameters.dbase.get<int >("reducedInterpolationWidth")=width;
      }
      else
      {
	printF("Sorry, the requested width=%i should be <= %i\n",width,oldWidth);
      }
    }
  }
  else if( dialog.getTextValue(answer,"reduce interpolation width","%i",width) )
  {
    int oldWidth=max(cg.interpolationWidth);
    if( width<oldWidth && width>0 )
    {
      printF("Changing the interpolation width to %i\n",width);
      cg.changeInterpolationWidth(width);
      parameters.dbase.get<int >("reducedInterpolationWidth")=width;
    }
    else
    {
      printF("Sorry, the requested width=%i should be <= %i and > 0 \n"
             "  No change was made.\n", width,oldWidth);
    }
  }
  else
  {
    found=false;
  }
  

  return found;
}


// ===================================================================================================================
/// \brief Build the dialog that shows the various general options
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DomainSolver::
buildAdaptiveGridOptionsDialog(DialogData & dialog )
{
  aString pbCommands[] = {"change adaptive grid parameters",
                          "change error estimator parameters",
                          "top hat parameters",
			  ""};
  aString *pbLabels = pbCommands;
  int numRows=3;
  dialog.setPushButtons( pbCommands, pbLabels, numRows ); 

  aString opCommands[] = {"use default error function",
		          "use top-hat for error function",
                          "" };

  dialog.addOptionMenu("error function", opCommands, opCommands, parameters.dbase.get<int>("amrErrorFunctionOption"));


  aString tbCommands[] = {"use adaptive grids",
                          "show amr error function",
                          "use user defined error estimator",
                          "use default error estimator",
                          "use front tracking",
			  ""};
  int tbState[10];
  tbState[0] = parameters.dbase.get<bool >("adaptiveGridProblem");
  tbState[1] = parameters.dbase.get<int >("showAmrErrorFunction");
  tbState[2] = parameters.dbase.get<bool >("useUserDefinedErrorEstimator");
  tbState[3] = parameters.dbase.get<bool >("useDefaultErrorEstimator");
  tbState[4] = parameters.dbase.get<int >("trackingIsOn");

  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  // ----- Text strings ------
  const int numberOfTextStrings=10;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "error threshold";
  sPrintF(textStrings[nt], "%e",parameters.dbase.get<real >("errorThreshold"));  nt++;

  textCommands[nt] = "regrid frequency";
  sPrintF(textStrings[nt], "%i (-1=use default)",parameters.dbase.get<int >("amrRegridFrequency"));  nt++;

  textCommands[nt] = "truncation error coefficient";
  sPrintF(textStrings[nt], "%e",parameters.dbase.get<real >("truncationErrorCoefficient"));  nt++;


  textCommands[nt] = "order of AMR interpolation";
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation"));  nt++;

  textCommands[nt] = "tracking frequency";
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("trackingFrequency"));  nt++;

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);
}

//================================================================================
/// \brief: Look for a general option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DomainSolver::
getAdaptiveGridOption(const aString & answer,
		      DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  bool iterativeImplicitInterpolation=true;
  int width=-1;

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  if( answer=="change adaptive grid parameters" )
  {
    if( parameters.dbase.get<Regrid* >("regrid")==NULL )
      parameters.dbase.get<Regrid* >("regrid") = new Regrid();

    parameters.dbase.get<Regrid* >("regrid")->update(gi);
  }
  else if( answer=="change error estimator parameters" )
  {
    if( parameters.dbase.get<ErrorEstimator* >("errorEstimator")==NULL )
      parameters.buildErrorEstimator();

    parameters.dbase.get<ErrorEstimator* >("errorEstimator")->update(gi);
  }
  else if( answer=="top hat parameters" )
  {
    real topHatCentre[3]={0.,0.,0.}, topHatVelocity[3]={1.,1.,1.}, topHatRadius=.25;
    gi.inputString(answer2,"Enter the centre");
    sScanF(answer2,"%e %e %e",&topHatCentre[0],&topHatCentre[1],&topHatCentre[2]);
    printF("centre = (%e,%e,%e)\n",topHatCentre[0],topHatCentre[1],topHatCentre[2]);
    gi.inputString(answer2,"Enter the radius");
    sScanF(answer2,"%e",&topHatRadius);
    printF("radius = %e\n",topHatRadius);
    gi.inputString(answer2,"Enter the top hat velocity vector");
    sScanF(answer2,"%e %e %e",&topHatVelocity[0],&topHatVelocity[1],&topHatVelocity[2]);
    printF("velocity = %e %e %e\n",topHatVelocity[0],topHatVelocity[1],topHatVelocity[2]);

    if( parameters.dbase.get<ErrorEstimator* >("errorEstimator")==NULL )
      parameters.buildErrorEstimator();
      
    parameters.dbase.get<ErrorEstimator* >("errorEstimator")->setTopHatParameters( topHatCentre, topHatVelocity,topHatRadius);         
  }
  else if( dialog.getToggleValue(answer,"use adaptive grids",parameters.dbase.get<bool >("adaptiveGridProblem") ) )
  {
    printF("Using adaptive mesh refinement.\n");
    if( parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")==NULL )
      parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")= 
                                            new InterpolateRefinements( cg.numberOfDimensions() );
    cg.getInterpolant()->setInterpolateRefinements( 
                            *parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements") );
  }
  else if( dialog.getToggleValue(answer,"show amr error function",
                                 parameters.dbase.get<int >("showAmrErrorFunction") ) )
  {   
    printP("***getAdaptiveGridOption: showAmrErrorFunction=%i\n",parameters.dbase.get<int >("showAmrErrorFunction"));
  }//
  else if( dialog.getToggleValue(answer,"use user defined error estimator",
                                 parameters.dbase.get<bool >("useUserDefinedErrorEstimator") ) ){}//
  else if( dialog.getToggleValue(answer,"use default error estimator",
                                   parameters.dbase.get<bool >("useDefaultErrorEstimator") ) ){}//
  else if( dialog.getToggleValue(answer,"use front tracking",parameters.dbase.get<int >("trackingIsOn") ) ){}//
  else if( answer=="use default error function" ||
           answer=="use top-hat for error function" )
  {
    if( answer=="use default error function" )
    {
      parameters.dbase.get<int>("amrErrorFunctionOption")=0;
    }
    else if( answer=="use top-hat for error function" )
    {
      parameters.dbase.get<int>("amrErrorFunctionOption")=1;
      printP("INFO:The error function will be computed from the top-hat function\n"
             "     rather than from the usual error estimate. Use 'top hat parameters' to \n"
             "     adjust the top-hat function");
    }
    else
    {
      OV_ABORT("error");
    }
  }
  else if( answer=="error threshold" ) // old
  {
    // gi.outputString("The error threshold should be in the range (0,1). ");
    gi.inputString(answer2,sPrintF(buff,"Enter the AMR error threshold (default value=%e)",
				   parameters.dbase.get<real >("errorThreshold")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("errorThreshold"));
    printF(" parameters.errorThreshold=%9.3e\n",parameters.dbase.get<real >("errorThreshold"));
  }
  else if( dialog.getTextValue(answer,"error threshold","%e",parameters.dbase.get<real >("errorThreshold")) ){}//
  else if( answer=="regrid frequency" ) // old
  {
    gi.outputString("By default the regrid frequency is equal to the refinement ratio");
    gi.inputString(answer2,sPrintF(buff,"Enter the regrid frequency, -1=default (current=%i)",
				   parameters.dbase.get<int >("amrRegridFrequency")));
    if( answer2!="" )
      sScanF(answer2,"%i",&parameters.dbase.get<int >("amrRegridFrequency"));
    printF(" parameters.amrRegridFrequency=%i\n",parameters.dbase.get<int >("amrRegridFrequency"));
  }
  else if( dialog.getTextValue(answer,"regrid frequency","%i",parameters.dbase.get<int >("amrRegridFrequency")) ){}//
  else if( answer=="truncation error coefficient" ) // old
  {
    gi.inputString(answer2,sPrintF(buff,"Enter the AMR truncation error coefficient (default value=%e)",
				   parameters.dbase.get<real >("truncationErrorCoefficient")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("truncationErrorCoefficient"));
    printF(" parameters.truncationErrorCoefficient=%9.3e\n",parameters.dbase.get<real >("truncationErrorCoefficient"));
  }
  else if( dialog.getTextValue(answer,"truncation error coefficient","%e",
                               parameters.dbase.get<real >("truncationErrorCoefficient")) ){}//
  else if( answer=="order of AMR interpolation" )// old
  {
    gi.outputString("The order of interpolation should be 2 or 3");
    gi.inputString(answer2,sPrintF(buff,"Enter the order of interpolation for AMR grids (current=%i)",
				   parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation")));
    if( answer2!="" )
      sScanF(answer2,"%i",&parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation"));
    printF(" parameters.orderOfAdaptiveGridInterpolation=%i\n",parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation"));
    if( parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")==NULL )
      parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")= new InterpolateRefinements( cg.numberOfDimensions() );
    parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")->setOrderOfInterpolation(parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation"));
  }
  else if( dialog.getTextValue(answer,"order of AMR interpolation","%i",
                               parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation")) )
  {
    if( parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")==NULL )
      parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")= 
                                                 new InterpolateRefinements( cg.numberOfDimensions() );
    parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")->
                          setOrderOfInterpolation(parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation"));
  }
  else if( dialog.getTextValue(answer,"tracking frequency","%i",
                               parameters.dbase.get<int >("trackingFrequency")) ){}//
  else
  {
    found=false;
  }
  

  return found;
}


// ===================================================================================================================
/// \brief Build the dialog that shows the various general options
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DomainSolver::
buildMovingGridOptionsDialog(DialogData & dialog )
{
  MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
  dialog.addInfoLabel(sPrintF("Bodies: %i matrix, %i rigid, %i deforming",
			      movingGrids.getNumberOfMatrixMotionBodies(),
                              movingGrids.getNumberOfRigidBodies(),
                              movingGrids.getNumberOfDeformingBodies()));
  

  aString pbCommands[] = {"specify grids to move",
			  ""};
  aString *pbLabels = pbCommands;
  int numRows=3;
  dialog.setPushButtons( pbCommands, pbLabels, numRows );

  aString tbCommands[] = {"use moving grids",
                          "detect collisions",
			  ""};
  int tbState[10];
  tbState[0] = parameters.dbase.get<MovingGrids >("movingGrids").isMovingGridProblem();
  tbState[1] = parameters.dbase.get<bool >("detectCollisions");
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  // ----- Text strings ------
  const int numberOfTextStrings=10;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "edit rigid body:";
  sPrintF(textStrings[nt], "%i",0);  nt++;

  textCommands[nt] = "edit matrix motion body:";
  sPrintF(textStrings[nt], "%i",0);  nt++;

  textCommands[nt] = "edit deforming body:";
  sPrintF(textStrings[nt], "%i",0);  nt++;

  textCommands[nt] = "minimum separation for collisions";
  sPrintF(textStrings[nt], "%e",parameters.dbase.get<real >("collisionDistance"));  nt++;

  textCommands[nt] = "frequency for full grid gen update";
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"));  nt++;

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);
}

//================================================================================
/// \brief: Look for a general option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DomainSolver::
getMovingGridOption(const aString & answer,
		    DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  bool iterativeImplicitInterpolation=true;
  bool useMovingGrids=false;

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;
  int bodyToEdit=0;
  
  if( answer=="specify grids to move" )
  {
    parameters.dbase.get<MovingGrids >("movingGrids").update(cg,gi);
  }
  else if( dialog.getToggleValue(answer,"use moving grids",useMovingGrids ) )
  {
    parameters.dbase.get<MovingGrids >("movingGrids").setIsMovingGridProblem(useMovingGrids);
  }
  else if( dialog.getToggleValue(answer,"detect collisions",parameters.dbase.get<bool >("detectCollisions") )){} //
  else if( dialog.getTextValue(answer,"minimum separation for collisions","%e",
                                       parameters.dbase.get<real >("collisionDistance")) ){}//
  else if( dialog.getTextValue(answer,"frequency for full grid gen update","%i",
			       parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration")) )
  {
          printF("INFO: For moving grids, the overlapping grid generator is called at every time step.\n"
             "      An optimized grid generation algorithm is used which will not minimize the overlap.\n"
             "      Once in a while the full algorithm is used -- you can change the frequency \n"
		 "      this occurs here. Choosing a value of 1 will mean the full update is always called.\n");
  }
  else if( dialog.getTextValue(answer,"edit rigid body:","%i",bodyToEdit) )
  {
    // Edit an existing rigid body
    MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
    const int numRigidBodies = movingGrids.getNumberOfRigidBodies();
    if( bodyToEdit>=0 && bodyToEdit<numRigidBodies )
    {
      RigidBodyMotion & rigidBody = movingGrids.getRigidBody(bodyToEdit);
      rigidBody.update(gi);
    }
    else
    {
      printF("getMovingGridOption:ERROR: rigid body number %i is invalid. Should be in the range [0,%i]\n",
	     bodyToEdit,numRigidBodies-1);
    }
    
  }
  else if( dialog.getTextValue(answer,"edit matrix motion body:","%i",bodyToEdit) )
  {
    // Edit an existing rigid body
    MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
    const int numMatrixMotionBodies = movingGrids.getNumberOfMatrixMotionBodies();
    if( bodyToEdit>=0 && bodyToEdit<numMatrixMotionBodies )
    {
      MatrixMotion & matrixMotionBody = movingGrids.getMatrixMotionBody(bodyToEdit);
      matrixMotionBody.update(gi);
    }
    else
    {
      printF("getMovingGridOption:ERROR: rigid body number %i is invalid. Should be in the range [0,%i]\n",
	     bodyToEdit,numMatrixMotionBodies-1);
    }
    
  }
  else if( dialog.getTextValue(answer,"edit deforming body:","%i",bodyToEdit) )
  {
    // Edit an existing deforming body
    MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
    const int numDeformingBodies = movingGrids.getNumberOfDeformingBodies();

    CompositeGrid & cg = *(gf[current].u.getCompositeGrid());

    if( bodyToEdit>=0 && bodyToEdit<numDeformingBodies )
    {
      DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(bodyToEdit);
      deformingBody.update(cg,gi);
    }
    else
    {
      printF("getMovingGridOption:ERROR: deforming body number %i is invalid. Should be in the range [0,%i]\n",
	     bodyToEdit,numDeformingBodies-1);
    }
    
  }
  else
  {
    found=false;
  }
  

  return found;
}


// ===================================================================================================================
/// \brief Assign run-time parameters for the DomainSolver.
/// \details This is the main function for assigning PDE parameters during the initialization stage.
/// \param runSetupOnExit (input) : if true, run the setup function on exit.
// ===================================================================================================================
int DomainSolver::
setParametersInteractively(bool runSetupOnExit/*=true*/)
{
  real cpu0 = getCPU();
  const int buffSize=100;
  char buff[buffSize];

  if( false )
  {
    printF("**** setParametersInteractively START:\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }

  int & plotOption = parameters.dbase.get<int>("plotOption");
  int & plotMode = parameters.dbase.get<int >("plotMode");

  // open log files for debug, check, logging etc.
  if ( parameters.dbase.get<FILE* >("debugFile")==NULL &&  
       parameters.dbase.get<FILE* >("checkFile")==NULL &&  
       parameters.dbase.get<FILE* >("logFile")==NULL &&  
       parameters.dbase.get<FILE* >("moveFile")==NULL )
    parameters.openLogFiles(getName());

  realCompositeGridFunction & u = gf[current].u;
  gf[current].cg.reference(cg); // *wdh* 060919

  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  // we keep a copy of the original boundary conditions
  IntegerArray & originalBoundaryCondition = parameters.dbase.get<IntegerArray >("originalBoundaryCondition");
  if( cg.numberOfComponentGrids()>0 )
  {
    getOriginalBoundaryConditions(cg,originalBoundaryCondition);
  }
  

  gi.appendToTheDefaultPrompt("setParameters>");
  
  if( plotOption!=0 && !gi.graphicsIsOn() )
    gi.createWindow();   // open up a graphics window


  aString reactionName="";
  bool restartChosen=false;
  setupPde(reactionName,restartChosen,originalBoundaryCondition);
  


  // Fill in the list of grids associated with each equationDomain. We have already determined the
  // mapping from grid number -> equationDomainNumber
  if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
  {
    ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));

    const int numberOfEquationDomains = equationDomainList.size();
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const int domainNumber = equationDomainList.gridDomainNumberList[grid];
      assert( domainNumber>=0 && domainNumber<numberOfEquationDomains );
      equationDomainList[domainNumber].gridList.push_back(grid);
    }
  }
  if( parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")!=NULL )
  {
    SurfaceEquation & surfaceEquation = *(parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation"));
    const int numberOfSurfaceEquationFaces=surfaceEquation.faceList.size();
    if( numberOfSurfaceEquationFaces>0 )
    {
      // we are solve surface equations

      parameters.dbase.get<int >("numberOfSurfaceEquationVariables")=surfaceEquation.numberOfSurfaceEquationVariables;  
    }
  }
  

  const int numberOfDimensions = cg.numberOfDimensions();
  int grid,side,axis;

  // ***********************************************************************
  // ***** initialize stuff now that we know which pde we are solving ******
  // ***********************************************************************

  const Parameters::Stuff & defaultValue =Parameters::defaultValue;
  real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
  Parameters::TimeSteppingMethod & timeSteppingMethod= parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  RealArray & printArray = parameters.dbase.get<RealArray >("printArray");

  // In parallel we require explicit interpolation (for now) 
  #ifdef USE_PPP
  if( cg.getInterpolant()!=NULL && !cg.getInterpolant()->interpolationIsExplicit() )
  {
    printF("DomainSolver:INFO: This grid has implicit interpolation. Explicit interpolation is faster.\n");
    // printf("DomainSolver:ERROR: The parallel composite grid interpolator needs explicit interpolation ****\n");
    // Overture::abort();
  }
  #endif

  if( !restartChosen )
  {

    /// kkc 0701025 first set generic data
    //              btw, why isn't this stuff done in Parameters::updateToMatchGrid ??

    // variableBoundaryData(grid) is true if variable BC data is required; such as 
    // a parabolic inflow profile
    parameters.dbase.get<IntegerArray >("variableBoundaryData").redim(cg.numberOfComponentGrids());
    parameters.dbase.get<IntegerArray >("variableBoundaryData")=false;

    parameters.dbase.get<aString >("restartFileName")="ob1.restart";
  
    // *** scLC
    parameters.dbase.get<IntegerArray >("timeStepType").redim(cg.numberOfComponentGrids());
    parameters.dbase.get<IntegerArray >("timeStepType")=0;  // All set to explicit
    // *** ecLC

    printArray.redim(10);
    printArray=defaultValue;

    parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours")=3;
    parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine")=3;
    
    /// kkc 0701025 now call functions that may be virtual
    //              XXXX BILL : this section and the one just after it used to be swapped
    parameters.setUserDefinedParameters();

    parameters.setParameters(numberOfDimensions,reactionName);
    pressureLevel=defaultValue;
    parameters.dbase.get<real >("kThermal")=defaultValue;

    parameters.updateToMatchGrid(cg);

    timeSteppingMethod= parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");

  //kkc 080708 do not do this here since boundary conditions have not been assigned yet and the ones in the grid may not be relevant???
  //      setDefaultDataForBoundaryConditions();
  
  }
  NameList nl;
  
  bool initialConditionsSpecified=false;
  bool bcDataSpecified=true;              // set to false if certain BC's are used

  aString name;
  int gridWasPlotted=false;
  // psp.set(GI_COLOUR_BOUNDARIES_BY_BOUNDARY_CONDITION,true);

  aString mainMenu[]=
  {
    "!change parameters",
    "continue",
    "display parameters",
//    ">twilight zone",
//      ">polynomial",
//        "turn on polynomial",    
//        "degree in space",
//        "degree in time",
//      "<>trigonometric",
//        "turn on trigonometric",
//        "frequencies",
//      "<turn on twilight zone",
//      "turn off twilight zone",
//      "use 2D function in 3D",
    ">time stepping options",
      "use new advanceSteps versions",
//    "final time (tf=)",
//    "cfl (cfl=)",
//    "dtMax",
//    "recompute dt interval",
//    "implicit factor",
//    "slow start time interval",
//    "slow start cfl",
//    ">time stepping method",
//    "forwardEuler",
//    "adamsBashforth2",
//    "adamsPredictorCorrector2",
//    "midpoint",
    // *** scLC
//    "Runge-Kutta",
    // *** ecLC
    "implicit",  // We need to keep this here for now -> conflict with "implicit time step solver options"
//    "all speed implicit",
//    "linearized all speed implicit", 
//    "<choose grids for implicit",
//     "<boundary conditions",
//     "data for boundary conditions",
//     ">output options",
//       "times to plot (tp=)",
//       ">plot option (po=)",
//         "plot and wait first time",
//         "plot with no waiting",
//         "plot and always wait",
//         "no plotting",
//       "<show file options",
//      "show file variables",
//      "save uncompressed show file",
//      "save compressed show file",
//       "save a restart file",
//       "do not save a restart file",
//      "frequency to save in show file (fsf=)",
//      "frequency to flush the show file",
//       "turn on user defined output",
//       "turn off user defined output",
//       "output periodically to a file",
//    "<initial conditions",
//     ">project initial conditions",
//       "project initial conditions",
//       "do not project initial conditions",
//    "<pde parameters",
//     ">axisymmetric flow",
//       "turn on axisymmetric flow",
//       "turn off axisymmetric flow",
//       "cylindrical axis is x axis",
//       "cylindrical axis is y axis",
//    "<>adaptive grids",
//      "turn on adaptive grids",
//      "turn off adaptive grids",
//      "error threshold",
//      "truncation error coefficient",
//      "order of AMR interpolation",
//      "regrid frequency",
//      "top hat parameters",
//      "change adaptive grid parameters",
//      "change error estimator parameters",
//      "show amr error function",
//      "turn on user defined error estimator",
//      "turn off user defined error estimator",
//      "turn on default error estimator",
//      "turn off default error estimator",
//      "turn on front tracking",
//      "turn off front tracking",
//      "tracking frequency",
    "<>Debugging",
      ">debug-file options",
         "print solution/errors",     
         "check error on ghost",
         "print classify array",
         "print sparse matrix",
         "turn on memory checking",
      "<debug",
      "Oges::debug (od=)",
      "Reactions::debug (rd=)",
//      "compare 3D run to 2D",
//      "check for floating point errors",
      "corner extrapolation option",
//    "order of extrapolation for interpolation neighbours",
//      "use iterative implicit interpolation",
//      "do not use iterative implicit interpolation",
//      "reduce interpolation width",
//      "maximum number of iterations for implicit interpolation",
    "always use curvilinear BC version", 
//    "<>sparse solver options",
//      "pressure solver options",
//      "implicit time step solver options",
//    "<>moving grids",
//      "turn on moving grids",
//      "turn off moving grids",
//      "specify grids to move",
//      "detect collisions",
//      "do not detect collisions",
//      "minimum separation for collisions",
//      "frequency for full grid gen update",
//    "<plot the grid",
    "<erase",
    "exit",
    ""
  };
//\begin{>setParametersInclude.tex}{}
//\no function header:
//
// Here is a description of the menu options available for changing parameters. This main parameter menu appears
// when \DomainSolver is run and is found in the {\tt DomainSolver::\-set\-Parameters\-Interactively()} function.
//\begin{description}
//  \item[continue] choose this item to exit this menu and continue on to the run-time dialog.
//  \item[time stepping parameters...] : open the time stepping parameters dialog
//    \begin{description}
//      \item[time stepping method] : Not all schemes work for all PDEs. 
//      \item[final time] : Integrate to this time.
//      \item[cfl] : Set the {\tt cfl} parameter. 
//         The maximum time step based on stability is scaled by this factor.
//         By default {\tt cfl=.9}. 
//      \item[dtMax] : Restrict the time step to be no larger than this value.
//      \item[implicit factor] :This value in $[0.,1.]$ is used with the implicit time-stepping. A value
//         of $.5$ will correspond to a 2nd-order Crank-Nicolson approach for the viscous terms,
//        a value of $1.$ will be backward-Euler and a value of $0.$ will be forward-Euler. See 
//        the the reference manual for more details.
//      \item[recompute dt every] : The time step, dt,  is recomputed every time the solution is plotted/saved.
//              In addition you may specify the maximum number of steps that will be taken
//              before dt is recomputed. Use this if the solution is not plotted very often.
//      \item[slow start time] : Ramp the time step $\Delta t$ from a small value (determined by slow start cfl)
//         to it maximum value (as  determined by the {\tt cfl} parameter over this time interval. 
//      \item[slow start cfl]: The initial time step for the slow start option is determined by this cfl value, default$=.25$..
//    \end{description}
//  
// \item[pde options...] open the pde options menu. The dialog which opens depends on which PDE was chosen and is described below.
// \item[initial conditions options...] open the initial conditions dialog. This dialog is under
//      construction.
//    \begin{description}
//      \item[read from a show file]: read initial conditions from a show file. This can either be
//        a show file generated from DomainSolver or one that you have built yourself.
//      \item[read from a restart file]: read initial conditions from a restart file.  
//      \item[uniform state] : specify a uniform state.
//     \end{description}
// \item[show file options...] open the showfile dialog.
//    \begin{description}
//        \item[show variables] : toggle on/off variables that should be saved in the show file.
//        \item[mode] : specify the mode as compressed or uncompressed. A compressed file will be
//         smaller (especially for AMR runs that create many grids) but a compressed file will not
//         be readable by future versions of DomainSolver. 
//        \item[open] : open a show file. You will be prompted for the name.
//        \item[close] : close the show file.
//      \item[frequency to save] : By default the solution is saved in the show file
//          as often as it is plotted according to {\tt 'times to plot'}. To save the solution less
//          often set this integer value to be greater than 1. A value of 2 for example will save solutions
//          every 2nd time the solution is plotted.
//      \item[frequency to flush] : Save this many solutions in each show file so that multiple
//        show files will be created (these are automatically handled by plotStuff). See section~(\ref{sec:flush})
//        for why you might do this.  
//     \end{description}
// \item[display parameters] : print current values for parameters.
// \item[output options...] open the output options dialog.
//  \item[output options] : Here are the output options.
//    \begin{description}
//      \item[plot option-menu] :
//        \begin{description}
//          \item[plot and wait first time] :
//          \item[plot with no waiting] :
//          \item[plot and always wait] : 
//          \item[no plotting] : do not plot. If you want to turn off all graphics you must choose
//            this option and also run cg with the noplot option.
//        \end{description}
//      \item[output periodically to a file] : output data to a file at each time step
//      \item[times to plot] : Specify the time interval between plotting (and saving in a show file).
//      \item[show file options...] open the show file options dialog.
//      \item[save a restart file] : save or do not save a restart file.
//      \item[allow user defined output] : call the userDefinedOutput routine at every step.
//      \item[times to plot] : change the time interval between plotting (and output).
//      \item[check file cutoffs] : used internally for regression tests.
// %     \item[read restart file] : read from a restart file.
// %     \item[restart file name (rsf=)] : name of the restart file to read from.
//    \end{description}
// \item[boundary conditions...] open the boundary condition options dialog.
//        This dialog is under construction
//  \item[twilight zone options...] : open the twilight zone (method of analytic solutions) dialog.
//    \begin{description}
//     \item[type] : specify the type of analytic solution
//     \begin{description}
//       \item[polynomial] : 
//         \begin{description}
//           \item[turn on polynomial] : Make the twilight-zone function be a polynomial.
//           \item[degree in space] : 0,1, or 2
//           \item[degree in time] : 0,1, or 2
//         \end{description}
//       \item[trigonometric]
//         \begin{description}
//           \item[turn on trigonometric] : Make the twilight-zone function be a trigonometric polymoinal.
//           \item[frequencies] : arguments to the trig functions are $\Pi$ ? times the frequency
//                specified here. 
//         \end{description}
//      \end{description}
//    \item[twilight zone flow] : toggle on or off. When this option is on the equations are forced so that the true solution is
//         equal to some analytically defined function. This is used to test the accuracy of the code.
//     \item[use 2D function in 3D] : use a 2D analytic function in 3D .
//     \item[compare 3D run to 2D] : make adjustments so that an extruded 3D geoemtry can be compared to a 3D computation.
//     \item[degree in space] : degree of the spatial polynomial
//     \item[degree in time] : degree of the temporal polynomial
//     \item[frequencies (x,y,z,t)] : frequencies to use with the trigonometric analytic solution.
//    \end{description}
//  \item[plot the grid] : plot the grid.
//  \item[project initial conditions] : (popup menu)
//    \begin{description}
//      \item[project initial conditions] : Project initial conditions to nearly satisfy $\grad\cdot\uv=0$. This
//        option applies to INS and ASF.
//      \item[do not project initial conditions] : (popup menu)
//    \end{description}
//
//  \item[time stepping options] : Here are options that affect the time step.
//    \begin{description}
//      \item[choose grids for implicit] : For use with the {\tt implicit} time stepping option. Choose 
//         which grids to integrate implicitly and which to integrate explicitly. Normally one should choose
//         thoses grids with fine grid spacing (such as in boundary layers) to be implicit while a back-ground
//       grid could be explicit. See section~(\ref{sec:implicitMenu}).
//    \end{description}
//  \item[boundary conditions] : Brings up a new menu described in section~(\ref{sec:bcMenu}).
//  \item[data for boundary conditions] : Brings up the sub-menu described in section~(\ref{sec:bcDataMenu}).
//  \item[initial conditions] : Brings up a new menu described in section~(\ref{sec:icMenu}).
//  \item[pde parameters] : This brings up a new menu described below in section~(\ref{sec:pdeParams}).
//  \item[axisymmetric flow] : solve an axisymmetric problem with cylindrical symmetry.
//    \begin{description}
//      \item[turn on axisymmetric flow] : The solution is assumed to have cylindrical symmetry about
//                 the axis $y=0$ with the grid defined only in the region $y\ge 0$.
//      \item[turn off axisymmetric flow] :
//      \item[cylindrical axis is x axis] : axis of symmetry is x=0
//      \item[cylindrical axis is y axis] : axis of symmetry is y=0
//    \end{description}
//   \item[adaptive grids] : use adaptive mesh refinement.
//     \begin{description}
//       \item[turn on adaptive grids] 
//       \item[turn off adaptive grids] 
//       \item[error threshold]
//       \item[truncation error coefficient] : 
//       \item[order of AMR interpolation] : 
//       \item[regrid frequency] : 
//       \item[change adaptive grid parameters] : change AMR regridding parameters (class Regrid).
//       \item[change error estimator parameters] : change parameters in the error estimator (class ErrorEstimator).
//       \item[show amr error function] : add the error function used for AMR regridding to the items that
//         can be plotted.
//     \end{description}
//  \item[Debugging] :
//    \begin{description}
//       \item[debug file options] : turn out various output to the debug file, ob.debug.
//         \begin{description}
//           \item[print solution/errors] : print solution (or errors if known) at each time.
//           \item[check error on ghost] : also check errors of ghost points.
//           \item[print classify array] : print the classify array for sparse coefficient matrixes.
//           \item[print sparse matrix] : print the sparse matrix generated by Oges (big).
//        \end{description}
//      \item[debug (debug=)] : This is a bit flag that turns on various messages. The more bits turned
//        on, the more detailed the messages that appear. Thus a value of debug=3 (1+2) would have the first
//        2 bits turned on and would display few messages. A value of debug=63 (1+2+4+8+16+32) would have 6
//        bits turned on and would results in a lot of information.
//      \item[Oges::debug (od=)] : bit flag debug variable for Oges.
//      \item[Reactions::debug (rd=)] :
//      \item[compare 3D run to 2D] : this option will adjust the equations and forcing so
//         that a 3D run on an extruded 2D grid can be compared to the 2D computation. This includes setting
//         the twilight-zone function to be 2D and changing the divergence damping (INS) to be two-dimensional 
//         (otherwise it is scaled in the wrong way).
//    \end{description}
//  \item[reduce interpolation width] : specify a new interpolation width. For example, when solving the
//      inviscid Navier-Stokes equations one may want to use linear interpolation (width=2) instead of
//      of quadratic interpolation (width=3) since this may reduce wiggles. 
//      If the grid was built with width=3 interpolation you can
//      reduce the order of interpolation with this option.
//  \item[sparse solver options] :
//    \begin{description}
//      \item[pressure solver options] : Choosing this item will allow you to change any {\tt Oges} related
//         parameters as they apply to the elliptic equation for the pressure. See the {\tt Oges} documentation
//         for a description of these parameters~\cite{OGES}.
//      \item[implicit time step solver options] :Choosing this item will allow you to change any {\tt Oges} related
//         parameters as they apply to the mplicit time stepping equations. See the {\tt Oges} documentation
//         for a description of these parameters~\cite{OGES}.
//    \end{description}
//  \item[moving grids] : Options related to moving grids.
//    \begin{description}
//      \item[turn on moving grids] : Allow grids to move.
//      \item[turn off moving grids] : do not allow grids to move.
//      \item[specify grids to move] : indicate which grids move and how. You must also choose 
//            {\tt`turn on moving grids'} if you really want these grids to move. See section~(\ref{sec:moveMenu}).
//      \item[detect collisions] : detect collisions for some types of rigid bodies (wip)
//      \item[do not detect collisions] : turn off collision detection.
//      \item[minimum separation for collisions] : minimum allowed distance between colliding bodies. 
//          This distance is in grid lines and should be chosen large enough so that a valid grid
//          can still be generated. Usually value will be from 2 to 3 but may need to be more for
//          some grids.
//    \end{description}
//  \item[plot the grid] : plot the grid. Useful to see if boundary conditions have been plotted correctly.
//  \item[erase] : erase the graphics screen.
//  \item[exit] : exit this menu and continue on (same as 'continue').
// \end{description}
//
// \subsubsection{Show file options}
//   Here are the options related to show files, these options are from the {\tt updateShowFile} function
//   in the {\tt Parameters} class.
// 
// \input ShowFileOptionsInclude.tex
//
//\end{setParametersInclude.tex} 



  Range Rx(0,numberOfDimensions-1);
  aString answer2,answer3;
  aString *gridMenu = new aString [cg.numberOfComponentGrids()+5];
  gridMenu[0]="!grids";
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    gridMenu[grid+1]=cg[grid].mapping().getName(Mapping::mappingName);
  gridMenu[cg.numberOfComponentGrids()+1]="all";
  gridMenu[cg.numberOfComponentGrids()+2]="none";
  gridMenu[cg.numberOfComponentGrids()+3]="done";
  gridMenu[cg.numberOfComponentGrids()+4]="";

  GUIState mainMenuInterface;

  mainMenuInterface.setWindowTitle(sPrintF(buff,"%s Parameters",(const char*)className));
  mainMenuInterface.setExitCommand("continue", "continue");

  mainMenuInterface.buildPopup(mainMenu);

  bool buildDialog=true;
  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  if( buildDialog )
  {
  // push buttons
    aString pbCommands[] = {"time stepping parameters...",
                            "plot options...",
                            "output options...",
                            "pde options...",
                            "boundary conditions...",
                            "initial conditions options...",
                            "forcing options...",
			    "twilight zone options...",
			    "showfile options...",
                            "general options...",
                            "adaptive grid options...",
                            "moving grid options...",
                            "plot the grid",
                            "display parameters",
			    ""};
    aString *pbLabels = pbCommands;
    int numRows=7;
    mainMenuInterface.setPushButtons( pbCommands, pbLabels,numRows ); // default is 2 rows

  }
  
  // make dialog siblings


  // ********************* time stepping options **************************
  DialogData &timeSteppingDialog = mainMenuInterface.getDialogSibling();

  timeSteppingDialog.setWindowTitle("Time Stepping Parameters");
  timeSteppingDialog.setExitCommand("close time stepping", "close");
  timeSteppingDialog.setOptionMenuColumns(1);

  buildTimeSteppingDialog(timeSteppingDialog );
  
  
  // ********************* forcing options ********************************
  DialogData &forcingOptionsDialog = mainMenuInterface.getDialogSibling();
  forcingOptionsDialog.setWindowTitle("Forcing Options");
  forcingOptionsDialog.setExitCommand("close forcing options", "close");
  if( buildDialog )
  {
    buildForcingOptionsDialog(forcingOptionsDialog );
  }
 

  // ********************* output options ********************************
  DialogData &outputOptionsDialog = mainMenuInterface.getDialogSibling();

  outputOptionsDialog.setWindowTitle("Output Options");
  outputOptionsDialog.setExitCommand("close output options", "close");
  if( buildDialog )
  {
    buildOutputOptionsDialog(outputOptionsDialog);
    
  }

  // ********************* plot options ********************************
  DialogData &plotOptionsDialog = mainMenuInterface.getDialogSibling();

  plotOptionsDialog.setWindowTitle("Plot Options");
  plotOptionsDialog.setExitCommand("close plot options", "close");
  if( buildDialog )
  {
    buildPlotOptionsDialog(plotOptionsDialog);
    
  }

  DialogData &tzOptionsDialog = mainMenuInterface.getDialogSibling();
  pTzOptionsDialog = &tzOptionsDialog;  // used in get initial conditions
  tzOptionsDialog.setWindowTitle("Twilight Zone Options");
  tzOptionsDialog.setExitCommand("close twilight zone options", "close");
  parameters.setTwilightZoneParameters(cg,"build dialog",&tzOptionsDialog);
  
  DialogData &showInterface = mainMenuInterface.getDialogSibling();

  showInterface.setWindowTitle("Show File Options");
  showInterface.setExitCommand("close show file options", "close");
  parameters.updateShowFile("build dialog",&showInterface);

  DialogData &pdeDialog = mainMenuInterface.getDialogSibling();
  pdeDialog.setExitCommand("close pde options", "close");
  parameters.setPdeParameters(cg,"build dialog",&pdeDialog);

//    DialogData &bcDialog = mainMenuInterface.getDialogSibling();
//    bcDialog.setExitCommand("close boundary conditions", "close");
//    parameters.defineBoundaryConditions(cg,originalBoundaryCondition,"build dialog",&bcDialog);

  DialogData &initialConditionsDialog = mainMenuInterface.getDialogSibling();
  initialConditionsDialog.setWindowTitle("Initial Conditions Options");
  initialConditionsDialog.setExitCommand("close initial conditions options", "close");
  getInitialConditions("build dialog",&initialConditionsDialog,&mainMenuInterface);

  // ********************* general options ********************************
  DialogData &generalOptionsDialog = mainMenuInterface.getDialogSibling();
  generalOptionsDialog.setWindowTitle("General Options");
  generalOptionsDialog.setExitCommand("close general options", "close");
  if( buildDialog )
  {
    buildGeneralOptionsDialog(generalOptionsDialog );
  }

  // ********************* adaptive grid options ********************************
  DialogData &adaptiveGridOptionsDialog = mainMenuInterface.getDialogSibling();
  adaptiveGridOptionsDialog.setWindowTitle("Adaptive Grid Options");
  adaptiveGridOptionsDialog.setExitCommand("close adaptive grid options", "close");
  if( buildDialog )
  {
    buildAdaptiveGridOptionsDialog( adaptiveGridOptionsDialog );
  }

  // ********************* moving grid options ********************************
  DialogData &movingGridOptionsDialog = mainMenuInterface.getDialogSibling();
  movingGridOptionsDialog.setWindowTitle("Moving Grid Options");
  movingGridOptionsDialog.setExitCommand("close moving grid options", "close");
  if( buildDialog )
  {
    buildMovingGridOptionsDialog(movingGridOptionsDialog );
  }



  aString answer;
  int len=0;
  
  gi.pushGUI(mainMenuInterface);
    
  for(;;)
  {
    int item = gi.getAnswer(answer,"");

    // cout << "answer=[" << answer << "], item=" << item <<"\n";

    bool found=false;

    if( answer=="continue" || answer=="exit" )
    {
      if( !parameters.dbase.get<bool >("twilightZoneFlow") && !restartChosen && (!initialConditionsSpecified  || !bcDataSpecified) )
      {
	if( !initialConditionsSpecified )
	  printF("DomainSolver:WARNING: You should specify initial conditions before you can run\n");
        if( !bcDataSpecified )
	  printF("DomainSolver:WARNING: There are some boundary conditions for which you should specify"
                 " data before you can run\n");
      }
      break;
    }
    else if( found= (parameters.updateShowFile(answer,&showInterface)==0) )
    {
      if( parameters.dbase.get<int >("debug") & 2 ) printF("Answer was found in parameters.updateShowFile\n");
    }
    else if( found= (parameters.setPdeParameters(cg,answer,&pdeDialog)==0) )
    {
      if( parameters.dbase.get<int >("debug") & 2 ) printF("Answer was found in setPdeParameters\n");
    }
//      else if( found= (parameters.defineBoundaryConditions(cg,originalBoundaryCondition,answer,&bcDialog)==0) )
//      {
//        // ** this is the new way to define boundary conditions **
//        printF("Answer was found in parameters.dbase.get< >("defineBoundaryConditions")\n");
//      }
    else if( found=getInitialConditions(answer,&initialConditionsDialog)==0 )
    {
      // The above call will also assign the initial conditions!
      if( parameters.dbase.get<int >("debug") & 2 ) printF("Answer was found in getInitialConditions\n");
      initialConditionsSpecified=true;
    }
    // These next parameters will be found in getInitialConditions:
//     else if( found=parameters.setTwilightZoneParameters( answer,&tzOptionsDialog )==0 )
//       printF("answer found in parameters.dbase.get< >("setTwilightZoneParameters")\n");

    if( found )
    {
      continue;
    }
//     else if( answer=="continue" || answer=="exit" )
//     {
//       if( !parameters.dbase.get<bool >("twilightZoneFlow") && !restartChosen && (!initialConditionsSpecified  || !bcDataSpecified) )
//       {
// 	if( !initialConditionsSpecified )
// 	  printF("DomainSolver:WARNING: You should specify initial conditions before you can run\n");
//         if( !bcDataSpecified )
// 	  printF("DomainSolver:WARNING: There are some boundary conditions for which you should specify"
//                  " data before you can run\n");
//       }
//       break;
//     }
    else if( answer=="time stepping parameters..." || answer=="time stepping parameters" )
    {
      // gi.pushGUI(timeSteppingDialog);
      timeSteppingDialog.showSibling();
    }
    else if( answer=="close time stepping" )
    {
      timeSteppingDialog.hideSibling();  // pop timeStepping
    }
    else if( getTimeSteppingOption(answer,timeSteppingDialog ) )
    {
      printF("Answer=%s found in getTimeSteppingOption\n",(const char*)answer);
    }
    else if( getForcingOption(answer,forcingOptionsDialog ) )
    {
      printF("Answer=%s found in getForcingOption\n",(const char*)answer);
    }

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

    else if( answer=="adaptive grid options..." )
    {
      adaptiveGridOptionsDialog.showSibling();
    }
    else if( answer=="close adaptive grid options" )
    {
      adaptiveGridOptionsDialog.hideSibling(); 
    }
    else if( getAdaptiveGridOption(answer,adaptiveGridOptionsDialog ) )
    {
      printF("Answer=%s found in getAdaptiveOption\n",(const char*)answer);
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

    else if( getOutputOption(answer,outputOptionsDialog ) )
    {
      printF("Answer=%s found in getOutputOption\n",(const char*)answer);
    }
    else if( answer=="output options..." || answer=="output options" )
    {
      outputOptionsDialog.showSibling();
    }
    else if( answer=="close output options" )
    {
      outputOptionsDialog.hideSibling(); 
    }

    else if( getPlotOption(answer,plotOptionsDialog ) )
    {
      printF("Answer=%s found in getPlotOption\n",(const char*)answer);
    }
    else if( answer=="plot options..." )
    {
      plotOptionsDialog.showSibling();
    }
    else if( answer=="close plot options" )
    {
      plotOptionsDialog.hideSibling(); 
    }

    else if( answer=="forcing options..." || answer=="forcing options" )
    {
      forcingOptionsDialog.showSibling();
    }
    else if( answer=="close forcing options" )
    {
      forcingOptionsDialog.hideSibling(); 
    }
    else if( answer=="twilight zone options..." || answer=="twilight zone options" )
    {
      tzOptionsDialog.showSibling();
    }
    else if( answer=="close twilight zone options" )
    {
      printF("*** close twilight zone options...***\n");
      tzOptionsDialog.hideSibling(); 
    }
    else if( answer=="showfile options..." || answer=="showfile options" )
    {
      showInterface.showSibling();
    }
    else if( answer=="close show file options" )
    {
      showInterface.hideSibling();  
    }
    else if( answer=="pde options..." || answer=="pde options" )
    {
      pdeDialog.showSibling();
    }
    else if( answer=="close pde options" )
    {
      pdeDialog.hideSibling();  
    }
    else if( answer=="boundary conditions..." )
    {
      // bcDialog.showSibling();
      parameters.defineBoundaryConditions(cg,originalBoundaryCondition);
      
    }
//      else if( answer=="close boundary conditions" )
//      {
//        bcDialog.hideSibling();  
//      }
    else if( answer=="initial conditions options..." || answer=="initial conditions options" )
    {
      initialConditionsDialog.showSibling();
    }
    else if( answer=="close initial conditions options" )
    {
      initialConditionsDialog.hideSibling();  
    }
    else if( answer=="display parameters" )
    {
      displayParameters();
    }
    else if( answer=="use iterative implicit interpolation" )
    {
      cg.rcData->interpolant->setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
    }
    else if( answer=="do not use iterative implicit interpolation" )
    {
      cg.rcData->interpolant->setImplicitInterpolationMethod(Interpolant::directSolve);
    }
    else if( answer=="maximum number of iterations for implicit interpolation" )
    {
      gi.inputString(answer2,"Enter the maximum number of iterations for implicit interpolation (-1=use default)");
      sScanF(answer2,"%i",&parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation"));
      printF("Setting maximumNumberOfIterationsForImplicitInterpolation=%i\n",
              parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation"));
    }
    else if( answer=="corner extrapolation option" )
    {
      printF("corner extrapolation option:\n"
             " 0 : corners are extrapolated along diagonals (default)\n"
             " 1 : do not extrapolate in axis1 direction\n"
             " 2 : do not extrapolate in axis2 direction\n"
             " 3 : do not extrapolate in axis3 direction\n");
      gi.inputString(answer2,sPrintF(buff,"Enter the corner extrapolation option (current=%i)",
                    parameters.dbase.get<int >("cornerExtrapolationOption")));
      sScanF(answer2,"%i",&parameters.dbase.get<int >("cornerExtrapolationOption"));
      printF("parameters.cornerExtrapolationOption=%i\n",parameters.dbase.get<int >("cornerExtrapolationOption"));
      
    }
    else if( answer=="order of extrapolation for interpolation neighbours" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the order of extrapolation for interpolation neighbours (current=%i)",
                    parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours")));
      sScanF(answer2,"%i",&parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours"));
      printF("parameters.orderOfExtrapolationForInterpolationNeighbours=%i\n",
             parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours"));
    }
    else if( answer=="order of extrapolation for second ghost line" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the order of extrapolation for the second ghost line (current=%i)",
                    parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine")));
      sScanF(answer2,"%i",&parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine"));
      printF("parameters.orderOfExtrapolationForSecondGhostLine=%i\n",
             parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine"));
    }

    else if( answer=="use new advanceSteps versions" )
    {
      parameters.dbase.get<int >("useNewAdvanceStepsVersions")=true;
    }
//     else if( len=answer.matches("times to plot" ) )
//     {
//       sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("tPrint"));
//       outputOptionsDialog.setTextLabel("times to plot",sPrintF(answer2,"%g", parameters.dbase.get<real >("tPrint"))); 
//       printF(" tPrint=%9.3e\n",parameters.dbase.get<real >("tPrint"));
//     }

//     else if( outputOptionsDialog.getTextValue(answer,"debug","%i",parameters.dbase.get<int >("debug")) ){}//
//     else if( outputOptionsDialog.getTextValue(answer,"info flag","%i",parameters.dbase.get<int >("info")) ){}//
//     else if( outputOptionsDialog.getTextValue(answer,"output format","%s",parameters.dbase.get<aString >("outputFormat")) ){}//
//     else if( len=answer.matches("check file cutoffs") )
//     {
//       gi.outputString("Specify cutoffs for the check file. Enter a component number and value");
//       int n=0;
//       sScanF(answer(len,answer.length()-1),"%i",&n);
//       sScanF(answer(len,answer.length()-1),"%i %e",&n,&parameters.dbase.get<RealArray>("checkFileCutoff")(n));
//       outputOptionsDialog.setTextLabel("check file cutoffs",sPrintF(buff,"%i %8.1e ",n,parameters.dbase.get<RealArray>("checkFileCutoff")(n)));
//       gi.outputString(sPrintF(buff,"Setting cutoff for component %i to %e\n",n,parameters.dbase.get<RealArray>("checkFileCutoff")(n)));
//     }
    else if( answer=="plot option (po=)" )
    {
      if( plotMode==0 )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the plot option (default value=%i)",plotOption));
	if( answer2!="" )
	  sScanF(answer2,"%i",&plotOption);
	printF(" plotOption=%i\n",plotOption);
      }
      else
        printF("Not changing the plot option since plotting is disabled (plotMode=%i).\n",plotMode);
    }
//     else if( answer=="plot and wait first time" )
//     {
//       if( plotMode==0 )
//         plotOption=3;
//       else
//         printF("Not changing the plot option since plotting is disabled (plotMode=%i).\n",plotMode);
//     }
//     else if( answer=="plot with no waiting" )
//     {
//       if( plotMode==0 ) 
//         plotOption=2;
//       else
//         printF("Not changing the plot option since plotting is disabled (plotMode=%i).\n",plotMode);
//     }
//     else if( answer=="plot and always wait" )
//     {
//       if( plotMode==0 ) 
//         plotOption=1;
//       else
//         printF("Not changing the plot option since plotting is disabled (plotMode=%i).\n",plotMode);
//     }
//     else if( answer=="no plotting" )
//     {
//       plotOption=0;
//     }
    else if( answer=="turn on user defined output" )
    {
      parameters.dbase.get<int >("allowUserDefinedOutput")=true;
    }
    else if( answer=="turn off user defined output" )
    {
      parameters.dbase.get<int >("allowUserDefinedOutput")=false;
    }
    else if( answer=="always use curvilinear BC version" )
    {
      parameters.dbase.get<bool >("alwaysUseCurvilinearBoundaryConditions")=true; 
      printF("Always use the curvilinear version of the boundary conditions\n"); 
    }
    else if( answer=="output periodically to a file" )
    {
      if( parameters.dbase.get<int >("numberOfOutputFiles")>=Parameters::maximumNumberOfOutputFiles )
      {
	printF("ERROR: too many files open\n");
	continue;
      }
      parameters.dbase.get<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency")[parameters.dbase.get<int >("numberOfOutputFiles")]=1;
      gi.inputString(answer,"Save to the file every how many steps? (default=1)");
      sScanF(answer,"%i",&parameters.dbase.get<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency")[parameters.dbase.get<int >("numberOfOutputFiles")]);
	  
      FileOutput & fileOutput = * new FileOutput;
      parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[parameters.dbase.get<int >("numberOfOutputFiles")] = &fileOutput;
      parameters.dbase.get<int >("numberOfOutputFiles")++;
          
      fileOutput.update(u,gi);

	  
    }
    else if( answer=="frequency to save in show file (fsf=)" )
    {
      printF("*** WARNING: This option is OBSOLETE. use `show file' menu instead. option=%s\n",(const char*)answer);
      gi.inputString(answer2,sPrintF(buff,"Enter the frequencyToSaveInShowFile (default value=%i)",
             parameters.dbase.get<int >("frequencyToSaveInShowFile")));
      if( answer2!="" )
	sScanF(answer2,"%i",&parameters.dbase.get<int >("frequencyToSaveInShowFile"));
      printF(" frequencyToSaveInShowFile=%i\n",parameters.dbase.get<int >("frequencyToSaveInShowFile"));
    }
    else if( answer=="frequency to flush the show file" )
    {
      printF("*** WARNING: This option is OBSOLETE. use `show file' menu instead. option=%s\n",(const char*)answer);
      int flushFrequency;
      gi.inputString(answer2,"Enter the frequency to flush the show file");
      if( answer2!="" )
	sScanF(answer2,"%i",&flushFrequency);
      flushFrequency=max(1,flushFrequency);
      if( parameters.dbase.get<Ogshow* >("show")!=NULL )
        parameters.dbase.get<Ogshow* >("show")->setFlushFrequency( flushFrequency );
      printF(" flushFrequency=%i\n",flushFrequency);
    }
    else if( answer=="save a restart file" )
    {
      parameters.dbase.get<bool >("saveRestartFile")=true;
      printF("A restart file will be saved. Actually, two files will be saved, `ob1.restart' and `ob2.restart',\n"
              "just in case the program crashes while writing the restart file. \n"                  
              "At least one of these files should be valid for restarting. \n"
              "You can run cg using a restart file for initial conditions\n");
    }
//     else if( len=answer.matches("save a restart file") )
//     {
//       int value;
//       sScanF(answer(len,answer.length()-1),"%i",&value);
//       outputOptionsDialog.setToggleState("save a restart file",value);      
//       parameters.dbase.get<bool >("saveRestartFile")=value;
//       if( parameters.dbase.get<bool >("saveRestartFile") )
//       {
// 	printF("A restart file will be saved. Actually, two files will be saved, `ob1.restart' and `ob2.restart',\n"
// 	  "just in case the program crashes while writing the restart file. \n"                  
// 	  "At least one of these files should be valid for restarting. \n"
// 	  "You can run cg using a restart file for initial conditions\n");
//       }
//     }
//     else if( len=answer.matches("allow user defined output") )
//     {
//       int value;
//       sScanF(answer(len,answer.length()-1),"%i",&value);
//       outputOptionsDialog.setToggleState("allow user defined output",value);      
//       parameters.dbase.get<int >("allowUserDefinedOutput")=value;
//       if( parameters.dbase.get<int >("allowUserDefinedOutput") )
// 	printf("allow user defined output\n");
//       else
// 	printf("Do not allow user defined output\n");
//     }
//     else if( len=answer.matches("disable plotting") )
//     {
//       int value;
//       sScanF(answer(len,answer.length()-1),"%i",&value);
//       if( value==1 )
//         plotMode= value==1 ? 1 : 0;

//       outputOptionsDialog.setToggleState("disable plotting",plotMode==1);      
//     }
//    else if( outputOptionsDialog.getToggleValue(answer,"plot residuals",parameters.dbase.get<int >("showResiduals")) ){}//
    else if( len=answer.matches("turn on front tracking") )
    {
      parameters.dbase.get<int >("trackingIsOn")=true;
      printF("Turn on front tracking.\n");
    }
    else if( len=answer.matches("turn off front tracking") )
    {
      parameters.dbase.get<int >("trackingIsOn")=false;
      printF("Turn off front tracking.\n");
    }
    else if( len=answer.matches("tracking frequency") )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the tracking frequency (default value=%e)",
				     parameters.dbase.get<int >("trackingFrequency")));
      if( answer2!="" )
	sScanF(answer2,"%i",&parameters.dbase.get<int >("trackingFrequency"));
      printF(" parameters.trackingFrequency=%i\n",parameters.dbase.get<int >("trackingFrequency"));

    }
    else if( answer=="do not a save restart file" )
    {
      parameters.dbase.get<bool >("saveRestartFile")=false;
      printF(" saveRestartFile=%i\n",parameters.dbase.get<bool >("saveRestartFile"));
    }
    else if( answer=="pde parameters" )
    {
      parameters.setPdeParameters(cg);
    }
    else if( answer=="turn on axisymmetric flow" )
    {
      if( parameters.dbase.get<int >("numberOfDimensions")!=2 )
      {
	printf("Sorry: axisymmetric flow option is only valid for a 2D grid\n");
      }
      else
      {
        parameters.dbase.get<bool >("axisymmetricProblem")=true;
        printF("axisymmetric flow is now on. You should also set the boundary condition on sides that \n"
               "match the axis of symmetry to `axisymmetric'\n");
      }
    }
    else if( answer=="turn off axisymmetric flow" )
    {
      parameters.dbase.get<bool >("axisymmetricProblem")=false;
      printF("axisymmetric flow is now off\n");
    }
    else if( answer=="cylindrical axis is x axis" )
    {
	parameters.dbase.get<int >("radialAxis")=axis2;
	printf("cylindrical axis is now the x axis, y=0 (radialAxis=%i)\n",parameters.dbase.get<int >("radialAxis"));
    }
    else if( answer=="cylindrical axis is y axis" )
    {
	parameters.dbase.get<int >("radialAxis")=axis1;
	printf("cylindrical axis is now the y axis, x=0 (radialAxis=%i)\n",parameters.dbase.get<int >("radialAxis"));
    }
    else if( answer=="turn on adaptive grids" )
    {
      parameters.dbase.get<bool >("adaptiveGridProblem")=true;
      printF("Using adaptive mesh refinement.\n");
      if( parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")==NULL )
	parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements")= new InterpolateRefinements( cg.numberOfDimensions() );
      cg.getInterpolant()->setInterpolateRefinements( *parameters.dbase.get<InterpolateRefinements* >("interpolateRefinements") );
    }
    else if( answer=="turn off adaptive grids" )
    {
      parameters.dbase.get<bool >("adaptiveGridProblem")=false;
      printF("Do NOT use adaptive mesh refinement.\n");
    }
//     else if( answer=="top hat parameters" )
//     {
//       real topHatCentre[3]={0.,0.,0.}, topHatVelocity[3]={1.,1.,1.}, topHatRadius=.25;
//       gi.inputString(answer,"Enter the centre");
//       sScanF(answer,"%e %e %e",&topHatCentre[0],&topHatCentre[1],&topHatCentre[2]);
//       printF("centre = (%e,%e,%e)\n",topHatCentre[0],topHatCentre[1],topHatCentre[2]);
//       gi.inputString(answer,"Enter the radius");
//       sScanF(answer,"%e",&topHatRadius);
//       printF("radius = %e\n",topHatRadius);
//       gi.inputString(answer,"Enter the top hat velocity vector");
//       sScanF(answer,"%e %e %e",&topHatVelocity[0],&topHatVelocity[1],&topHatVelocity[2]);
//       printF("velocity = %e %e %e\n",topHatVelocity[0],topHatVelocity[1],topHatVelocity[2]);

//       if( parameters.dbase.get<ErrorEstimator* >("errorEstimator")==NULL )
//         parameters.buildErrorEstimator();
      
//       parameters.dbase.get<ErrorEstimator* >("errorEstimator")->setTopHatParameters( topHatCentre, topHatVelocity,topHatRadius);         
//     }
//     else if( answer=="change adaptive grid parameters" )
//     {
//       if( parameters.dbase.get<Regrid* >("regrid")==NULL )
// 	parameters.dbase.get<Regrid* >("regrid") = new Regrid();

//       parameters.dbase.get<Regrid* >("regrid")->update(gi);
//     }
//     else if( answer=="change error estimator parameters" )
//     {
//       if( parameters.dbase.get<ErrorEstimator* >("errorEstimator")==NULL )
// 	parameters.buildErrorEstimator();

//       parameters.dbase.get<ErrorEstimator* >("errorEstimator")->update(gi);
//     }
    else if( answer=="show amr error function" )
    {
      parameters.dbase.get<int >("showAmrErrorFunction")=true;
    }
    else if( answer==  "turn on user defined error estimator" )
    {
      parameters.dbase.get<bool >("useUserDefinedErrorEstimator")=true;
    }
    else if( answer=="turn off user defined error estimator" )
    {
      parameters.dbase.get<bool >("useUserDefinedErrorEstimator")=false;
    }
    else if( answer=="turn on default error estimator" )
    {
      parameters.dbase.get<bool >("useDefaultErrorEstimator")=true;
    }
    else if( answer==  "turn off default error estimator" )
    {
      parameters.dbase.get<bool >("useDefaultErrorEstimator")=false;
    }
    else if( answer=="advectionCoefficient (ac=)" ) // old way 
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the advection coefficientReynolds (default value=%e)",
				     parameters.dbase.get<real >("advectionCoefficient")));
      if( answer2!="" )
	sScanF(answer2,"%e",&parameters.dbase.get<real >("advectionCoefficient"));
      printF(" parameters.advectionCoefficient=%9.3e\n",parameters.dbase.get<real >("advectionCoefficient"));
    }
    else if ( len=answer.matches("advectionCoefficient") ) // new way *** this should go in a menu *****
    {
      sScanF(answer(len,answer.length()-1),"%e",&parameters.dbase.get<real >("advectionCoefficient"));
    }
    else if( answer=="show file options..." || 
             answer=="show file options" )
    {
      parameters.updateShowFile();
    }
    else if( answer=="show file variables" )
    {
      printF("*** WARNING: This option is OBSOLETE. use `show file' menu instead. option=%s\n",(const char*)answer);

      const aString *showVariableName = parameters.dbase.get<aString* >("showVariableName");
      const int maximumNumberOfNames=parameters.dbase.get<int >("numberOfComponents")+20;
      aString *showMenu= new aString[maximumNumberOfNames];
      for( ;; )
      {
	int i=0;
	for( int n=0; showVariableName[n]!=""; n++ )
	{
	  showMenu[i]=showVariableName[n] + (parameters.dbase.get<IntegerArray>("showVariable")(i)>0 ? " (on)" : " (off)");
          i++;
          assert( i+2 < maximumNumberOfNames );
	}
	showMenu[i++]="done";
	showMenu[i]="";

	int response=gi.getMenuItem(showMenu,answer2,"toggle variables to save in the show file");
        if( answer2=="done" || answer2=="exit" )
	  break;
	else if( response>=0 && response<i-1 )
	  parameters.dbase.get<IntegerArray>("showVariable")(response)=-parameters.dbase.get<IntegerArray>("showVariable")(response);
	else
	{
	  printF("Unknown response: [%s]\n",(const char*)answer2);
	  gi.stopReadingCommandFile();
	}
	
      }
    }
//     else if( answer=="pressure solver options" )
//     {
//       pressureSolverParameters.update(gi,cg);
//     }
//     else if( answer=="implicit time step solver options" )
//     {
//       implicitTimeStepSolverParameters.update(gi,cg);
//     }
    else if( answer=="check for floating point errors" )
    {
      Parameters::checkForFloatingPointErrors=1;
      printF(" Parameters::checkForFloatingPointErrors=%i\n",Parameters::checkForFloatingPointErrors);
    }
    else if( answer=="print solution/errors" )
    {
      parameters.dbase.get<int >("debug") |= 7;
      printF(" debug=%i\n",parameters.dbase.get<int >("debug"));
    }
    else if( answer=="print sparse matrix" )
    {
      Oges::debug |=31;
      printF(" print sparse matrix (setting Oges::debug = %i)\n",Oges::debug);
    }
    else if( answer=="print classify array" )
    {
      SparseRepForMGF::debug |=3;
      printF(" print sparse matrix classify (setting SparseRepForMGF::debug = %i)\n",SparseRepForMGF::debug);
    }
    else if( answer=="check error on ghost" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter 0 or 1 (default value=%i)",parameters.dbase.get<int >("checkErrorsAtGhostPoints")));
      if( answer2!="" )
	sScanF(answer2,"%i",&parameters.dbase.get<int >("checkErrorsAtGhostPoints"));
      printF(" checkErrorsAtGhostPoints=%i\n",parameters.dbase.get<int >("checkErrorsAtGhostPoints"));
    }
    else if( answer=="turn on memory checking" )
    {
      Overture::turnOnMemoryChecking(true);
    }
    else if ( answer.matches("refactor frequency") )
    {
	sScanF(answer,"refactor frequency %i",&parameters.dbase.get<int >("refactorFrequency"));
    }
    else if( answer=="Oges::debug (od=)")
    {
      gi.inputString(answer2,sPrintF(buff,"Enter Oges::debug (default value=%i)",Oges::debug));
      if( answer2!="" )
	sScanF(answer2,"%i",&Oges::debug);
      printF(" Oges::debug=%i\n",Oges::debug);
    }
    else if( answer=="Reactions::debug (rd=)" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter Reactions::debug (default value=%i)",Reactions::debug));
      if( answer2!="" )
	sScanF(answer2,"%i",&Reactions::debug);
      printF(" Reactions::debug=%i\n",Reactions::debug);
    }

    else if( answer=="initial conditions"  )
    {

      initialConditionsSpecified=true;

      parameters.setUserDefinedParameters(); // update user defined parameters

      // **************************************************
      // *** Here is where we assign initial conditions ***
      // **************************************************
      getInitialConditions();

      // printF("&&&&&&&& After getInitialConditions: gf[current=%i].t=%8.2e\n",current,gf[current].t);
    }
    else if( answer=="project initial conditions" )
    {
      parameters.dbase.get<bool >("projectInitialConditions")=true;
      printF(" projectIntialConditions=%i\n",parameters.dbase.get<bool >("projectInitialConditions"));
    }
    else if( answer=="do not project initial conditions" )
    {
      parameters.dbase.get<bool >("projectInitialConditions")=false;
      printF(" projectIntialConditions=%i\n",parameters.dbase.get<bool >("projectInitialConditions"));
    }
    else if( answer(0,26)=="project initial conditions" )
    {
      int ival=parameters.dbase.get<bool >("projectInitialConditions");
      sScanF(answer,"project initial conditions %i",&ival);
      parameters.dbase.get<bool >("projectInitialConditions")=ival;
      printF(" projectIntialConditions=%i\n",(int)parameters.dbase.get<bool >("projectInitialConditions"));
    }
    else if( answer=="boundary conditions" )
    {
      if( !gridWasPlotted )
        printF("INFO: If you want to see the boundary conditions you should first plot the grid\n");
      
      setBoundaryConditionsInteractively(answer,originalBoundaryCondition);
    }
    else if( answer=="data for boundaryConditions" )
    {
      bcDataSpecified=true;
      setBoundaryConditionsInteractively(answer,originalBoundaryCondition);
    }
    else if( answer=="turn on moving grids" )
    {
      // parameters.dbase.get< >("movingGridProblem")=true;

      parameters.dbase.get<MovingGrids >("movingGrids").setIsMovingGridProblem(true);

      printF(" movingGridProblem=%i\n",parameters.isMovingGridProblem());

    }
    else if( answer=="turn off moving grids" )
    {
      // parameters.dbase.get< >("movingGridProblem")=false;
      parameters.dbase.get<MovingGrids >("movingGrids").setIsMovingGridProblem(false);

      printF(" movingGridProblem=%i\n",parameters.isMovingGridProblem());

    }
    else if( answer=="specify grids to move" )
    {
      // parameters.dbase.get<MovingGrids >("movingGrids").setIsMovingGridProblem(parameters.dbase.get< >("movingGridProblem"));

      parameters.dbase.get<MovingGrids >("movingGrids").update(cg,gi);

    }
    else if( answer=="detect collisions" )
    {
      parameters.dbase.get<bool >("detectCollisions")=true;
    }
    else if( answer=="do not detect collisions" )
    {
      parameters.dbase.get<bool >("detectCollisions")=false;
    }
    else if( answer=="minimum separation for collisions" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the minimum separation in grid lines (default=%e)",
				     parameters.dbase.get<real >("collisionDistance")));
      if( answer2!="" )
	sScanF(answer2,"%e",&parameters.dbase.get<real >("collisionDistance"));
      printF("  minimum separation=%9.3e\n",parameters.dbase.get<real >("collisionDistance"));
    }
    else if( answer=="frequency for full grid gen update" )
    {
      printF("INFO: For moving grids, the overlapping grid generator is called at every time step.\n"
             "      An optimized grid generation algorithm is used which will not minimize the overlap.\n"
             "      Once in a while the full algorithm is used -- you can change the frequency \n"
             "      this occurs here. Choosing a value of 1 will mean the full update is always called.\n");

      gi.inputString(answer2,sPrintF(buff,"Enter frequency for full grid gen update for moving grids (current=%i)",
				     parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration")));
      if( answer2!="" )
	sScanF(answer2,"%i",&parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"));

      parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration")=
                 max(1,parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"));
      
      printF("  frequencyToUseFullUpdateForMovingGridGeneration=%i\n",
             parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"));
    }
    else if( answer=="body forcing..." )
    {
      parameters.setupBodyForcing(cg);
    }
    else if( answer=="controls..." )
    {
      // --- Define controls ---

      // Create a Controller object if it does not already exist.
      if( !parameters.dbase.has_key("Controller") )
      {
	Controller controller(parameters);
	parameters.dbase.put<Controller>("Controller",controller);
      }
      Controller & controller = parameters.dbase.get<Controller>("Controller");

      // Make changes to the controller:
      controller.update(cg,gi);

    }
    else if( answer=="user defined forcing..." ||
             answer=="user defined forcing" ) // for backward compatibility
    {
      setupUserDefinedForcing();
    }
    else if( answer=="user defined material properties..." )
    {
      setupUserDefinedMaterialProperties();
    }
    else if( answer=="plot the grid" )
    {
      PlotIt::plot(gi,cg,psp);
      if( psp.getObjectWasPlotted() ) 
	gridWasPlotted=true;
    }
    else if( answer=="erase" )
    {
      gi.erase();
      gridWasPlotted=false;
    }
    else 
    {
      // answer is not in the menu -- assume it is a NameList stype answer
      if( answer=="" || answer=="exit" ) break;
      if( answer[0]=='*' ) continue;  // comment
    
      nl.getVariableName( answer, name );   // parse the answer
    
      if( name=="printArray" )
      {
	int i0;
	nl.getRealArray(answer,printArray,i0);  
	printf(" printArray(%i)=%g\n",i0,printArray(i0));
	if( i0 >= printArray.getBound(0) )  // add more entries if this index is the last
	{
	  int n=printArray.getLength(0);
	  printArray.resize(n+10);
	  printArray(Range(n,n+9))=defaultValue;
	}
      }
      else
      {
	printF("unknown response: name=[%s], answer=[%s]\n",(const char*)name,(const char*)answer);
	gi.stopReadingCommandFile();
      }
      
    }

  } // end mainMenu 

  gi.popGUI();  // pop main

  showInterface.hideSibling();
  pdeDialog.hideSibling();
  initialConditionsDialog.hideSibling();
  tzOptionsDialog.hideSibling();

  delete [] gridMenu;

  gi.unAppendTheDefaultPrompt();
  if( plotOption > 0 && !gi.graphicsIsOn() )
    gi.createWindow(); // make sure the window is now open if initially it was closed

  real cpu1=getCPU();
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("totalTime"))+=cpu1-cpu0;
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInitialize"))=cpu1-cpu0;


  if( runSetupOnExit ) 
    setup(parameters.dbase.get<real >("tInitial"));  // now we know enough that we can create the grid functions etc.


  return 0;
}




// ===================================================================================================================
/// \brief Build the AMR hierarchy of grids for the initial conditions.
/// \details This is done after the DomainSolvers have been created since we may need truncation error info
/// and boundary condition info.
// ===================================================================================================================
int DomainSolver::
buildAmrGridsForInitialConditions()
{
  const Parameters::InitialConditionOption & initialConditionOption = 
                  parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption");
  if( initialConditionOption==Parameters::readInitialConditionFromShowFile &&
      parameters.dbase.get<bool>("useGridFromShowFile") )
  {
    // The AMR grid has already been read in from the show file.
    return 0;
  }
  

  if( parameters.dbase.get<bool >("adaptiveGridProblem") )
  {
    printF(" **** Build AMR levels for the initial conditions... *****\n");

    GridFunction & gf0 = gf[current];

    if( movingGridProblem() )  // *wdh* 040312
    {
      printF("**** buildAmrGridsForInitialConditions:BEFORE gf0.t = %e, gf0.gridVelocityTime=%e \n",gf0.t,gf0.gridVelocityTime);
      getGridVelocity( gf0, gf0.t );

      printF("**** buildAmrGridsForInitialConditions:AFTER gf0.t = %e, gf0.gridVelocityTime=%e \n",gf0.t,gf0.gridVelocityTime);

//        CompositeGrid & cg = gf0.cg;
      
//        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//        {
//  	if(  parameters.gridIsMoving(grid) )
//  	{
//  	  gf0.getGridVelocity(grid);
//  	}
//        }
    }
    

    // we should first apply the BC's since there could be a discontinuity at the boundary.
    interpolateAndApplyBoundaryConditions( gf0 );
//     gf0.u.reference(u);
//     gf0.cg.reference(cg);
//     gf0.t=parameters.dbase.get<real >("tInitial");

    
    if( parameters.dbase.get<Regrid* >("regrid")==NULL )
      parameters.dbase.get<Regrid* >("regrid") = new Regrid();
    const int maxNumberOfRefinementLevels=parameters.dbase.get<Regrid* >("regrid")->getDefaultNumberOfRefinementLevels();
    bool done=false;
    while( !done )
    {
      printF(" *****Build AMR level = %i \n",gf0.cg.numberOfRefinementLevels());

      if( parameters.dbase.has_key("amrNeedsTimeStepInfo") &&
	  parameters.dbase.get<bool>("amrNeedsTimeStepInfo") ) 
      {
        // for some methods we need to compute a truncation error and/or time-stepping eigenvalues
        //  so we call the dudt routine
	Range all;
	realCompositeGridFunction fn(gf0.cg,all,all,all,parameters.dbase.get<int >("numberOfComponents"));
    
	parameters.dbase.get<real >("dt")=1.e-5;  // we need an initial value
	getUt( gf0,gf0.t,fn,gf0.t);
	gf0.conservativeToPrimitive();
	parameters.dbase.get<real >("dt") = getTimeStep( gf0 ); 

        // we need to call again since now we have the correct dt -- the error estimate depends on dt
	getUt( gf0,gf0.t,fn,gf0.t);   
	gf0.conservativeToPrimitive();
      }
      
      adaptGrids( gf0 );

      // *wdh* 070705 -- we need to update the Interpolant ---
      // moved to adaptGrids 070706
      //       real time1=getCPU();
      //       gf0.cg.rcData->interpolant->updateToMatchGrid( gf0.cg ); 
      //       parameters.dbase.get<RealArray>("timing")(Parameters::timeForUpdateInterpolant)+=getCPU()-time1;

      // *wdh* 030808 interpolateAndApplyBoundaryConditions( gf0 ); // this is not needed

      // printF("Solution after adaptGrids and assignInitialConditions\n");
      // gi.contour(gf0.u);
      
      printF("cg.numberOfRefinementLevels()=%i gf0.cg.numberOfRefinementLevels()=%i\n",
	     cg.numberOfRefinementLevels(),gf0.cg.numberOfRefinementLevels());

      if( parameters.isMovingGridProblem() )
      {
  	// update cgf0.gridVelocity arrays
  	gf0.updateGridVelocityArrays();
        gf0.gridVelocityTime=gf0.t -1.e10;  // force a recomputation of the grid velocity
      }

      if( cg.numberOfRefinementLevels()!=gf0.cg.numberOfRefinementLevels() )
      {
        printF("New levels were added -- recompute the initial conditions\n");
	
        realCompositeGridFunction & u = gf[current].u;

        // new levels were added
        cg.reference(gf0.cg);

        assignInitialConditions(current);
        assert( gf0.form==GridFunction::primitiveVariables );
	
	if( movingGridProblem() )  // *wdh* 040312
	{
          getGridVelocity( gf0, gf0.t );

          if( parameters.dbase.get<int >("debug") & 8 )
	  {
	    if( cg.numberOfComponentGrids()==2 )
	      display(gf0.getGridVelocity(1),"buildAMR: gridVelocity(grid=1)","%8.2e ");
	  }
	  
//            // *** really on necessary to do new grids *** -- check refinement level?? ---
//  	  CompositeGrid & cg = gf0.cg;
//  	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//  	  {
//  	    if(  parameters.gridIsMoving(grid) )
//  	    {
//  	      gf0.getGridVelocity(grid);
//  	    }
//  	  }
	}

        interpolateAndApplyBoundaryConditions( gf0 );

        if( gf0.cg.numberOfRefinementLevels()>=maxNumberOfRefinementLevels )
          done=true;
      }
      else // if( gf0.cg.numberOfRefinementLevels()>=maxNumberOfRefinementLevels )
      {
	done=true;
      }

      
    }

    // update the work-space functions gf0[i] and fn[i]
    updateForAdaptiveGrids(cg);
    
  }
  return 0;
}


// ===================================================================================================================
/// \brief Prompt for changes in the solver parameters.
/// \details This dialog is available at start-up and also during run-time.
/// \param command (input) : optionally supply a command to execute. Attempt to execute the command
///    and then return. The return value is 0 if the command was executed, 1 otherwise.
/// \param interface (input) : use this dialog. If command=="build dialog", fill in the dialog and return.
/// 
// ===================================================================================================================
int DomainSolver::
setSolverParameters(const aString & command /* = nullString */,
                    DialogData *interface /* =NULL */ )
{
  printF("Inside DomainSolver::setSolverParameters\n");

  int returnValue=0;

  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  aString prefix = "CGSOL:"; // prefix for commands to make them unique.

  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;


  aString answer;
  char buff[100];
//  const int numberOfDimensions = cg.numberOfDimensions();
  
  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=20;
    aString cmd[maxCommands];

    dialog.setWindowTitle("Solver parameters");

    aString pbLabels[] = {"display parameters",
			  "" };

    addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=1;
    dialog.setPushButtons( cmd, pbLabels, numRows );
      
    
    if( executeCommand ) return 0;
  }
  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("solver parameters>");  
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

    if( answer=="done" )
      break;
    else if( answer=="display parameters" )
    {
      displayParameters();
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
