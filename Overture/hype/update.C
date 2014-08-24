#include "HyperbolicMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "DataPointMapping.h"
#include "arrayGetIndex.h"
#include <float.h>
#include "EquiDistribute.h"
#include "display.h"
#include "TridiagonalSolver.h"
#include "StretchMapping.h"
#include "GL_GraphicsInterface.h"
#include "PlotIt.h"

#include "CompositeSurface.h"

#include "TrimmedMapping.h"
#include "ReductionMapping.h"
#include "NurbsMapping.h"
#include "ComposeMapping.h"
#include "SplineMapping.h"
#include "ReparameterizationTransform.h"
#include "LineMapping.h"
#include "PlaneMapping.h"

#include "MappingProjectionParameters.h"
#include "UnstructuredMapping.h"

#include "CompositeTopology.h"
#include "GridSmoother.h"
#include "GridStretcher.h"

#include "MatchingCurve.h"

static long int filePosition=-1;


// ** this should be a general utility routine
//! Return the number of multigrid levels that could be built on this grid.
int 
numberOfPossibleMultigridLevels( const IntegerArray & gridIndexRange )
{
  int numberOfLevels=0;
  const int maximumNumberLevels=20;
  int numberOfDimensions=3, axis;
  for( axis=2; axis>0; axis-- )
    if( gridIndexRange(End,axis)-gridIndexRange(Start,axis) == 0 )
      numberOfDimensions--;
      
  for( int m=0; m<maximumNumberLevels; m++ )
  {
    const int pow2 = (int)(pow(2.,double(m+1))+.5);
    bool powerOfTwo=TRUE;
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      if( (((gridIndexRange(End,axis)-gridIndexRange(Start,axis)) % pow2 )!=0)  || // divisible by 2
	  (((gridIndexRange(End,axis)-gridIndexRange(Start,axis))/pow2) <2 ) )// at least 1 points on coarse grid
      {
	powerOfTwo=FALSE;
	break;
      }
    }
    if( !powerOfTwo )
    {
      numberOfLevels=m;
      break;
    }
  }
  return numberOfLevels;
}





int HyperbolicMapping::
buildSurfaceGridParametersDialog(DialogData & surfaceGridParametersDialog )
// ==========================================================================================
// /Description:
//   Build the surfaceGrid parameters dialog.
// ==========================================================================================
{
  const int numLabels=6;
  aString opLabel[numLabels] = {"edges",
				"coordinate line 0",
				"coordinate line 1",
				"points on surface",
                                "boundary curve",
				""}; //
  aString opCmd[numLabels];
  GUIState::addPrefix(opLabel,"initial curve:",opCmd,numLabels);

  int choice = 0;
  if( surface!=NULL && surfaceGrid && surface->getClassName()=="UnstructuredMapping" )
  {
    initialCurveOption=initialCurveFromBoundaryCurves;
  }
  surfaceGridParametersDialog.addOptionMenu("initial curve from:", opCmd,opLabel,(int)initialCurveOption);


  aString pbLabels[] = {"edit initial curve",
			"reparameterize initial curve",
                        "clear initial curve",
			"edit reference surface",
                        "create boundary curve",
                        "clear interior matching curves",
                        "edit an interior matching curve",
                        // "choose boundary condition mappings",
			""};
  // addPrefix(pbLabels,prefix,cmd,maxCommands);
  int numRows=7;
  surfaceGridParametersDialog.setPushButtons( pbLabels, pbLabels, numRows ); 


  aString tbCommands[] = {"project initial curve",
                          "use triangulation of the reference surface",
                          "project points onto reference surface",
                          "adjust for corners when marching",
			  ""};
  int tbState[10];
  tbState[0] = projectInitialCurve==true; 
  tbState[1] = useTriangulation==true; 
  tbState[2] = projectOntoReferenceSurface==true; 
  tbState[3] = surfaceMappingProjectionParameters[0].adjustForCornersWhenMarching()==true;
  int numColumns=1;
  surfaceGridParametersDialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;

  textLabels[nt] = "edge curve tolerance";
  sPrintF(textStrings[nt], "%8.1g", edgeCurveMatchingTolerance); nt++; 

  textLabels[nt] = "Start curve parameter bounds"; 
  sPrintF(textStrings[nt], "%g, %g",startCurveStart, startCurveEnd); nt++; 

  textLabels[nt] = "boundary curve matching tolerance"; 
  sPrintF(textStrings[nt], "%g",distanceToBoundaryCurveTolerance);

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

  // addPrefix(textLabels,prefix,cmd,maxCommands);
  surfaceGridParametersDialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}


bool HyperbolicMapping::
updateSurfaceGridParameters(aString & answer, DialogData & surfaceGridParametersDialog,
                             MappingInformation & mapInfo,
                             GraphicsParameters & referenceSurfaceParameters )
// ==========================================================================================
// /Description:
//     Assign values in the  surfaceGrid parameters dialog to match the current parameter values.
//
// /answer (input) : check this answer to see if it is a marching parameter.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  bool returnValue=true;
  aString line;
  int len;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const bool plotObjectSave=plotObject;
  plotObject=true;  // most things here require a redraw


  if( answer.matches("initial curve:edges") )
  {
    initialCurveOption=initialCurveFromEdges;
    surfaceGridParametersDialog.getOptionMenu("initial curve from:").setCurrentChoice(initialCurveOption);
    plotObject=false;
  }
  else if( answer.matches("initial curve:coordinate line 0") )
  {
    initialCurveOption=initialCurveFromCoordinateLine0;
    surfaceGridParametersDialog.getOptionMenu("initial curve from:").setCurrentChoice(initialCurveOption);
    plotObject=false;
  }
  else if( answer.matches("initial curve:coordinate line 1") )
  {
    initialCurveOption=initialCurveFromCoordinateLine1;
    surfaceGridParametersDialog.getOptionMenu("initial curve from:").setCurrentChoice(initialCurveOption);
    plotObject=false;
  }
  else if( answer.matches("initial curve:points on surface") || 
           answer.matches("initial curve:curve on surface") ) // keep for backward compatibility
  {
    initialCurveOption=initialCurveFromCurveOnSurface;
    surfaceGridParametersDialog.getOptionMenu("initial curve from:").setCurrentChoice(initialCurveOption);
    plotObject=false;
    gi.outputString("Define a curve on the surface by picking points with the mouse.");
    gi.outputString("Note: For triangulated surfaces, pick points on the triangle edges.");
    
  }
  else if( answer.matches("initial curve:boundary curve") )
  {
    initialCurveOption=initialCurveFromBoundaryCurves;
    surfaceGridParametersDialog.getOptionMenu("initial curve from:").setCurrentChoice(initialCurveOption);
    plotObject=false;
  }
  else if( (len=answer.matches("project initial curve")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); projectInitialCurve=value;
    surfaceGridParametersDialog.setToggleState("project initial curve",projectInitialCurve);
    if( projectInitialCurve )
      printf("project initial curve onto the reference surface.\n");
    else
      printf("Do not project initial curve onto the reference surface.\n");

    plotObject=false;
  }
  else if( (len=answer.matches("edit an interior matching curve")) )
  {
    const int numberOfMatchingCurves=matchingCurves.size();
    if( numberOfMatchingCurves<=0 )
    {
      printF("WARNING: There are no matching curves to edit!\n");
      gi.stopReadingCommandFile();
    }
    else
    {
      gi.inputString(answer,sPrintF("Enter the interior matching curve to edit (0,...,%i)",numberOfMatchingCurves-1));
      int matchCurve=-1;
      sScanF(answer,"%i",&matchCurve);
      if( matchCurve>=0 && matchCurve<numberOfMatchingCurves )
      {
	matchingCurves[matchCurve].update(gi);
      }
      else
      {
	printF("ERROR: invalid value for matching curve = %i. Must be in the range 0,...,%i\n",matchCurve,
	       numberOfMatchingCurves-1);
	gi.stopReadingCommandFile();
      }
    }
    
  }
  else if( (len=answer.matches("use triangulation of the reference surface")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); useTriangulation=value;
    surfaceGridParametersDialog.setToggleState("use triangulation of the reference surface",useTriangulation);
    if( useTriangulation )
      printf("use the triangulation of the reference surface for marching.\n");
    else
      printf("Do not use the triangulation of the reference surface for marching.\n");

    referenceSurfaceHasChanged=true;
    plotObject=false;
  }
  else if( (len=answer.matches("project points onto reference surface")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); projectOntoReferenceSurface=value;
    surfaceGridParametersDialog.setToggleState("project points onto reference surface",projectOntoReferenceSurface);
    if( projectOntoReferenceSurface )
      printf("project points onto reference surface when using the triangulation of the surface.\n");
    else
      printf("Do not project points onto reference surface when using the triangulation of the surface.\n");

    plotObject=false;
  }
  else if( (len=answer.matches("adjust for corners when marching")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); 
    if( value )
      printf("Adjust for corners (creases) in the surface when marching\n");
    else
      printf("Do not adjust for corners (creases) in the surface when marching\n");

    for( int m=0; m<3; m++ )
      surfaceMappingProjectionParameters[m].setAdjustForCornersWhenMarching(bool(value));

    surfaceGridParametersDialog.setToggleState("adjust for corners when marching",
                surfaceMappingProjectionParameters[0].adjustForCornersWhenMarching()==true);
  }
  else if( answer=="edit initial curve" )
  {
    printf("*** update the initial curve\n");
    if( startCurve!=0 )
    {
      if( startCurve->getClassName()=="ComposeMapping" )
      {
	ComposeMapping & compose = *(ComposeMapping*)startCurve;
	compose.map1.update(mapInfo);
	compose.setIsPeriodic(axis1,compose.map1.getIsPeriodic(axis1));
	compose.setGridDimensions(axis1,compose.map1.getGridDimensions(axis1));
      }
      else
	startCurve->update(mapInfo);

      // setup();

      updateForInitialCurve();
	
      plotHyperbolicSurface=false;
      plotDirectionArrowsOnInitialCurve=true;
      referenceSurfaceHasChanged=true;
    }
    else
    {
      printf("You must first create an initial curve before you can edit it\n");
    }
  }
  else if( answer=="reparameterize initial curve" )
  {
    if( startCurve!=0 )
    {
      if( startCurve->getClassName()!="ReparameterizationTransform" )
      {
	ReparameterizationTransform *rt = new ReparameterizationTransform(*startCurve);
	startCurve=rt;   // who will delete the old startCurve ?
      }

      startCurve->update(mapInfo);

      // setup();
      updateForInitialCurve();
      plotHyperbolicSurface=false;
      plotDirectionArrowsOnInitialCurve=true;

    }
    else
    {
      printf("You must first create an initial curve before you can edit it\n");
    }
  }
  else if( answer=="clear initial curve" )
  {
    if( startCurve!=NULL )
    {
      if( startCurve->decrementReferenceCount()==0 )
      {
	delete startCurve;
      }
      startCurve=NULL;
      boundaryCondition=domainDimension==3 ? outwardSplay : freeFloating;
      plotHyperbolicSurface=false;

      // reset some parameters
      startCurveStart=0.;
      startCurveEnd=1.;
      surfaceGridParametersDialog.setTextLabel("Start curve parameter bounds",sPrintF(line, "%g, %g",startCurveStart, startCurveEnd));
    }
  }
  else if( answer=="edit reference surface" )
  {
    if( surface!=0 )
    {
      surface->update(mapInfo);
      setup();
      plotHyperbolicSurface=false;
      referenceSurfaceHasChanged=true;
    }
    else
    {
      printf("HyperbolicMapping:ERROR:There is no reference surface to edit\n");
    }
  }
  else if( (len=answer.matches("Start curve parameter bounds")) )
  {
    sScanF(answer(len,answer.length()-1),"%e %e",&startCurveStart,&startCurveEnd);
    if( startCurve!=NULL )
      printf(" **** startCurve->getIsPeriodic(0)=%i\n",(int)startCurve->getIsPeriodic(0));
    
    if( startCurveStart<-.1 || startCurveStart>1.1 || startCurveEnd<-.1 || startCurveEnd>1.1 )
    {
      // User has input funny new parameter bounds -- if the curve is periodic we may allow them.
      bool ok=false;
      if( startCurve!=NULL && (bool)startCurve->getIsPeriodic(axis1) && fabs(startCurveEnd-startCurveStart)<=1. )
      {
        ok=true;
	if( startCurve->getClassName()=="SplineMapping" )
	{
//  	  SplineMapping & spline = (SplineMapping&)(*startCurve);
//            // For periodic splines the interval may lie in [-1,2] so the sub-section can cross the branch cut.
//            real rStart,rEnd;
//  	  spline.getDomainInterval(rStart,rEnd);
	  if( startCurveStart>=-1. && startCurveEnd<=2. )
	  {
//              real rStartNew,rEndNew;
//  	    if( startCurveStart<0. )
//  	    {
//  	      rStartNew=startCurveStart;
//  	      rEndNew=rStartNew+1.;

//    	      startCurveStart-=rStartNew; // we be 0.
//    	      startCurveEnd-=rStartNew;
	      
//  	    }
//  	    else
//  	    {
//                rEndNew=startCurveEnd;
//  	      rStartNew=rEndNew-1.;

//                startCurveStart+=1.-rEndNew;
//                startCurveEnd+=1.-rEndNew;  // will be 1
//  	    }
	      
//  	    printf("WARNING:The start curve is periodic with domain interval [%8.2e,%8.2e]\n"

//                     "  :I am going to change the domain interval to [%8.2e,%8.2e] to allow the new start curve bounds\n"
//                     "  :I will also shift the start curve bounds to [%8.2e,%8.2e]\n",
//  		   rStart,rEnd,rStartNew,rEndNew,startCurveStart,startCurveEnd);

            // *** spline.setDomainInterval(rStartNew,rEndNew);
	  }
	  else
	  {
            ok=false;
	  }
	}
      }
      if( !ok )
      {
        printf("ERROR: invalid start curve parameter bounds: start=%8.2e, end=%8.2e \n"
               "       I am resetting the bounds to [0.,1.]\n",startCurveStart,startCurveEnd);
	startCurveStart=0.;
	startCurveEnd=1.;
      }
    }

    surfaceGridParametersDialog.setTextLabel("Start curve parameter bounds",sPrintF(line, "%g, %g",startCurveStart, startCurveEnd));
    if( startCurve!=NULL )
    { 
      if( startCurve->getClassName()=="SplineMapping" )
      {
        printf("Changing the parameter bounds on the start curve to [%e,%e]\n",startCurveStart, startCurveEnd);
	SplineMapping & spline = (SplineMapping&)(*startCurve);
	real startCurveStartOld,startCurveEndOld;
        spline.getDomainInterval(startCurveStartOld,startCurveEndOld);
        int oldNumberOfPoints=getGridDimensions(axis1);

        spline.setDomainInterval(startCurveStart,startCurveEnd);

        // update the boundary conditions to be consistent with the new start curve bounds.
        real startCurveEndOffset[2]={startCurveStart,startCurveEnd-1.};  //
        for( int side=0; side<=1; side++ )
	{
          if( fabs(startCurveEndOffset[side])>REAL_EPSILON )
	  {
            for( int axis=0; axis<=1; axis++ )
	    {
	      if( boundaryCondition(side,axis)==periodic || 
		  boundaryCondition(side,axis)==matchToMapping || 
		  boundaryCondition(side,axis)==matchToABoundaryCurve )
	      {
                 // set to default since the current BC doesn't make sense.
		boundaryCondition(side,axis)=domainDimension==3 ? outwardSplay : freeFloating; 
                printf("reset the BC to default on side=%i axis=%i\n",side,axis);
		
                if( axis==0 )
		{
		  setBoundaryConditionAndOffset(side,axis,0);
                  // boundaryOffset[side][axis]=1;
		}
	      }
	    }
	  }
	}

        // printf(" Before updateForInitialCurve: bc=%i %i %i %i \n",getBoundaryCondition(0,0),
        //     getBoundaryCondition(1,0),getBoundaryCondition(0,1),getBoundaryCondition(1,1));

	updateForInitialCurve();

        // scale the number of points by the new fraction of the curve
        if( startCurveEndOld-startCurveStartOld>0. )
	{
          printf("old [rStart,rEnd]=[%e,%e]\n",startCurveStartOld,startCurveEndOld);
          printf("new [rStart,rEnd]=%e,%e]\n",startCurveStart,startCurveEnd);

	  numberOfPointsOnStartCurve=int( (oldNumberOfPoints-1)*fabs((startCurveEnd-startCurveStart)/
								     (startCurveEndOld-startCurveStartOld))+1.5 );
         
          spline.setGridDimensions(axis1,numberOfPointsOnStartCurve);
	  setGridDimensions(axis1,numberOfPointsOnStartCurve);
          printf("Setting numberOfPointsOnStartCurve=%i (old=%i)\n",numberOfPointsOnStartCurve,oldNumberOfPoints);
	  
	}
        else
	{
          printf("ERROR: startCurveEndOld=%e startCurveStartOld=%e\n",startCurveEndOld,startCurveStartOld);
	}
	
        //printf(" After updateForInitialCurve: bc=%i %i %i %i \n",getBoundaryCondition(0,0),
        //   getBoundaryCondition(1,0),getBoundaryCondition(0,1),getBoundaryCondition(1,1));

	plotHyperbolicSurface=false;
	plotDirectionArrowsOnInitialCurve=true;
      }
    }
    else
    {
      printf("Sorry, only know how to set the start curve parameter bounds for a spline\n");
    }
  }
  else if( (len=answer.matches("boundary curve matching tolerance")) )
  {
    sScanF(answer(len,answer.length()-1),"%e",&distanceToBoundaryCurveTolerance);
    surfaceGridParametersDialog.setTextLabel("boundary curve matching tolerance",
             sPrintF(line, "%g",distanceToBoundaryCurveTolerance));
  }
  else if( answer=="clear interior matching curves" )
  {
    // destroyInteriorMatchingCurves();
    matchingCurves.resize(0);
  }
  else if( (len=answer.matches("edge curve tolerance")) )
  {
    sScanF(answer(len,answer.length()-1),"%e",&edgeCurveMatchingTolerance);
    surfaceGridParametersDialog.setTextLabel("edge curve tolerance",sPrintF(line, "%g",edgeCurveMatchingTolerance));
  }
//   else if( answer=="choose boundary condition mappings" )
//   {

//     chooseBoundaryConditionMappings(mapInfo);

//   }
  else
  {
    returnValue=false;
    plotObject=plotObjectSave;
  }

  return returnValue;
}




int HyperbolicMapping::
buildPlotOptionsDialog(DialogData & plotOptionsDialog, GraphicsParameters & parameters )
// ==========================================================================================
// /Description:
//   Build the plot options dialog.
// ==========================================================================================
{


  aString tbCommands[] = {"plot reference surface",
			  "plot shaded boundaries on reference surface",
			  "plot boundary lines on reference surface",
			  "plot triangulation",
			  "plot bounds derived from reference surface",
                          "plot boundary curves",
                          "plot grid points on start curve",
                          "plot ghost points",
			  ""};
  int tbState[10];
  tbState[0] = plotReferenceSurface==true; 
  tbState[1] = 1; 
  tbState[2] = 0; 
  tbState[3] = plotTriangulation==true;
  tbState[4] = choosePlotBoundsFromReferenceSurface==true; 
  tbState[5] = plotBoundaryCurves==true;
  tbState[6] = plotGridPointsOnStartCurve==true;
  tbState[7] = plotGhostPoints==true;
  int numColumns=1;
  plotOptionsDialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  const int numberOfTextStrings=5;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "ghost lines to plot:";  sPrintF(textStrings[nt],"%i",numberOfGhostLinesToPlot);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  plotOptionsDialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}


bool HyperbolicMapping::
updatePlotOptions(aString & answer, DialogData & plotOptionsDialog,
		  MappingInformation & mapInfo,
                  GraphicsParameters & parameters,
		  GraphicsParameters & referenceSurfaceParameters )
// ==========================================================================================
// /Description:
//     Assign values in the  surfaceGrid parameters dialog to match the current parameter values.
//
// /answer (input) : check this answer to see if it is a marching parameter.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  bool returnValue=true;
  aString line;
  int len;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const bool plotObjectSave=plotObject;
  plotObject=true;  // most things here require a redraw

  if( (len=answer.matches("plot reference surface")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); plotReferenceSurface=value;
    plotOptionsDialog.setToggleState("plot reference surface",plotReferenceSurface);
    referenceSurfaceHasChanged=true;

    // keep the same plot bounds.
    choosePlotBoundsFromGlobalBounds=true;
	
  }
  else if( (len=answer.matches("plot grid points on start curve")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); plotGridPointsOnStartCurve=value;
    plotOptionsDialog.setToggleState("plot grid points on start curve",plotGridPointsOnStartCurve);
  }
  else if( (len=answer.matches( "plot ghost points" )) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&plotGhostPoints);
    plotOptionsDialog.setToggleState("plot ghost points",plotGhostPoints==true);
    plotObject=true;  // replot 
  }
  else if( (len=answer.matches("plot triangulation")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); plotTriangulation=value;
    plotOptionsDialog.setToggleState("plot triangulation",plotTriangulation);
    referenceSurfaceHasChanged=true;
  }
  else if( (len=answer.matches("plot shaded boundaries on reference surface")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value);
    plotOptionsDialog.setToggleState("plot shaded boundaries on reference surface",value);
    referenceSurfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,value);
    referenceSurfaceParameters.set(GI_PLOT_UNS_FACES,value);
    referenceSurfaceHasChanged=true;
  }
  else if( (len=answer.matches("plot boundary lines on reference surface")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value);
    plotOptionsDialog.setToggleState("plot boundary lines on reference surface",value);
    referenceSurfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,value);
    referenceSurfaceParameters.set(GI_PLOT_UNS_EDGES,value);
    referenceSurfaceHasChanged=true;
  }
  else if( (len=answer.matches("plot bounds derived from reference surface")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value);
    choosePlotBoundsFromReferenceSurface=value;
    plotOptionsDialog.setToggleState("plot bounds derived from reference surface",value);
    referenceSurfaceHasChanged=true;
  }
  else if( (len=answer.matches("plot boundary curves")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); plotBoundaryCurves=value;
    plotOptionsDialog.setToggleState("plot boundaryCurves",plotBoundaryCurves);
  }
  else if( plotOptionsDialog.getTextValue(answer,"ghost lines to plot:","%i",numberOfGhostLinesToPlot) )
  {
    printF("INFO: number of ghost lines to plot = %i. \n",numberOfGhostLinesToPlot);
  }
  else
  {
    returnValue=false;
    plotObject=plotObjectSave;
  }

  return returnValue;
}






int HyperbolicMapping::
buildBoundaryConditionMappingDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the surfaceGrid parameters dialog.
// ==========================================================================================
{

  if( growthOption==1 )
  {
    aString opCmd[] =   {"left BC (forward)",
			 "right BC (forward)",
			 ""}; //
    dialog.addOptionMenu("picking:", opCmd,opCmd,bcOption);
  }
  else if( growthOption==-1 )
  {
    bcOption=leftBackward;
    aString opCmd[] =   {"left BC (backward)",
			 "right BC (backward)",
			 ""}; //
    dialog.addOptionMenu("picking:", opCmd,opCmd,bcOption);
  }
  else
  {
    aString opCmd[] =   {"left BC (forward)",
			 "left BC (backward)",
			 "right BC (forward)",
			 "right BC (backward)",
			 ""}; 
    dialog.addOptionMenu("picking:", opCmd,opCmd,bcOption);
  }
  
  return 0;

}

int HyperbolicMapping::
updateBoundaryConditionMappingDialog(DialogData & dialog )
// =====================================================================================================
// /Description:
// 
// =====================================================================================================
{
  if( growthOption==1 )
  {
    aString opCmd[] =   {"left BC (forward)",
			 "right BC (forward)",
			 ""}; //
    bcOption=leftForward;
    dialog.changeOptionMenu(0,opCmd,opCmd,bcOption);
  }
  else if( growthOption==-1 )
  {
    bcOption=leftBackward;
    aString opCmd[] =   {"left BC (backward)",
			 "right BC (backward)",
			 ""}; //
    bcOption=leftBackward;
    dialog.changeOptionMenu(0,opCmd,opCmd,bcOption);
  }
  else
  {
    aString opCmd[] =   {"left BC (forward)",
			 "left BC (backward)",
			 "right BC (forward)",
			 "right BC (backward)",
			 ""}; 
    bcOption=leftForward;
    dialog.changeOptionMenu(0,opCmd,opCmd,bcOption);
  }
  return 0;
}



bool HyperbolicMapping::
updateBoundaryConditionMappings(aString & answer, 
                                DialogData & dialog, 
                                bool checkSelection, SelectionInfo & select,
                                MappingInformation & mapInfo )
// ==========================================================================================
// /Description:
//     Interactively choose Mapping's to use as boundary conditions when marching.
//  Use this to over-ride the Mapping's that may be chosen automatically.
//
// /answer (input) : check this answer to see if it is a chnage to the BC mapping's.
// /checkSelection (input) : if true, check for selections with the mouse.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  bool returnValue=true;

  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const bool plotObjectSave=plotObject;  // save the input value in case we reset it.
  
  plotObject=true;  // by default we need to replot
  int curveFound=false; // set to true if a BC mapping curve is chosen.
  Mapping *mappingPointer=NULL;
  int  side=-1, direction=-1;
    

  int len=0;
  if( answer=="left BC (forward))" )
  {
    bcOption=leftForward;
  }
  else if( answer=="left BC (backward)" )
  {
    bcOption=leftBackward;
  }
  else if( answer=="right BC (forward)" )
  {
    bcOption=rightForward;
  }
  else if( answer=="right BC (backward)" )
  {
    bcOption=rightBackward;
  }
  else if( answer.matches("boundary condition mapping:") )
  {
    if( (len=answer.matches("boundary condition mapping: select boundary curve")) )
    {
      int b=-1;
      sScanF( answer(len,answer.length()-1),"%i %i %i",&b,&side,&direction);    
      if( b>=0 && b<numberOfBoundaryCurves )
      {
        mappingPointer=boundaryCurves[b]; 
        curveFound=true;
      }
      else
      {
	gi.outputString(sPrintF("ERROR: invalid boundary curve = %i",b));
	gi.stopReadingCommandFile();
      }
    }
    else if( (len=answer.matches("boundary condition mapping: select edge curve")) )
    {
      int e=-1;
      sScanF( answer(len,answer.length()-1),"%i %i %i",&e,&side,&direction);    


      const bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
      if( isCompositeSurface )
      {
	CompositeSurface & cs = (CompositeSurface&)(*surface);
	CompositeTopology & compositeTopology = *cs.getCompositeTopology(); 
	if( e>=0 && e<compositeTopology.getNumberOfEdgeCurves() )
	{

	  mappingPointer= &compositeTopology.getEdgeCurve(e);
	  curveFound=true;
	}
	else
	{
	  gi.outputString(sPrintF("ERROR: invalid edge curve = %i",e));
	  gi.stopReadingCommandFile();
	}
      }
      else
      {
	gi.outputString("ERROR: trying to choose an edge curve but this is not a composite surface!");
	gi.stopReadingCommandFile();
      }
    }
    else
    {
      cout << "Unknown command = [" << answer << "]\n";
      gi.stopReadingCommandFile();
       
    }
	       
  }
  else if( checkSelection && select.nSelect )
  {
    if( bcOption==leftForward )
    {
      side=0, direction=0;
    }
    else if( bcOption==leftBackward )
    {
      side=0, direction=1;
    }
    else if( bcOption==rightForward )
    {
      side=1, direction=0;
    }
    else
    {
      side=1, direction=1;
    }

    for (int i=0; i<select.nSelect && !curveFound; i++)
    {
      // printf("i=%i, ID=%i, minZ=%i, maxZ=%i\n", i,select.selection(i,0),
      //         select.selection(i,1),select.selection(i,2));

      for( int b=0; b<numberOfBoundaryCurves; b++ )
      {
	if( boundaryCurves[b]->getGlobalID()==select.selection(i,0) )
	{

	  printf("Boundary curve %i selected for BC on side=%i and direction=%i\n",b,side,direction);

          // add a command to the command file.
          gi.outputToCommandFile(sPrintF("boundary condition mapping: select boundary curve %i %i %i\n",
                     b,side,direction));

          mappingPointer=boundaryCurves[b]; 

	  curveFound=true;
	  break;
	}
      } // end for b
    }
    if( !curveFound )
    {
      // look for an edge curve if no boundary curve was found

      bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
      bool checkForEdgeCurves=isCompositeSurface && ((CompositeSurface*)surface)->getCompositeTopology()!=NULL;
      if( checkForEdgeCurves )
      {
	int zBuffMin=INT_MAX;
	int selectedCurve=-1;
	CompositeSurface & cs = (CompositeSurface&)(*surface);
	CompositeTopology & compositeTopology = *cs.getCompositeTopology();      
	int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int e=0; e<numberOfEdgeCurves; e++ )
	  {
	    // printf(" edge=%i status=%i\n",e,int(compositeTopology.getEdgeCurveStatus(e)));
	    if( compositeTopology.getEdgeCurve(e).getGlobalID()==select.selection(i,0) &&
		select.selection(i,1)<zBuffMin  )
	    {
	      mappingPointer= &compositeTopology.getEdgeCurve(e);
	      zBuffMin=select.selection(i,1);
	      curveFound=true;
              printf("updateBoundaryConditionMappings:edge curve found for BC, e=%i\n",e);
	      
	      gi.outputToCommandFile(sPrintF("boundary condition mapping: select edge curve %i %i %i\n",
					     e,side,direction));
	    }
	  }
	}
      }
    }
  
  }
  else
  {
    returnValue=false;
    plotObject=plotObjectSave;
  }

  if( curveFound )
  {
    if( side>=0 && side<=1 && direction>=0 && direction<=1 )
    {
      if( boundaryConditionMappingWasNewed[side][direction]!=0 )
	delete boundaryConditionMapping[side][direction];            // what about the reference count ???

      boundaryConditionMapping[side][direction]= mappingPointer;
      boundaryConditionMapping[side][direction]->incrementReferenceCount();
      boundaryConditionMappingWasNewed[side][direction]=false;

      boundaryCondition(side,direction)=matchToABoundaryCurve;  // by default we look for curves to match to
      
      printf("Setting BC mapping on (side=%i,direction=%i)= %i \n",side,direction,
	     boundaryCondition(side,direction));
      
    }
    else
    {
      gi.outputString(sPrintF("ERROR: invalid values for side=%i or direction=%i",side,direction));
      gi.stopReadingCommandFile();

    }
    
  }


  return returnValue;
}



int HyperbolicMapping::
buildMarchingParametersDialog(DialogData & marchingParametersDialog, aString bcChoices[] )
// ==========================================================================================
// /Description:
//   Build the marching parameters dialog.
// ==========================================================================================
{
  marchingParametersDialog.setOptionMenuColumns(1);

//    aString opLabel[] = {"uniform","geometric","inverse hyperbolic","stretch Mapping",
//  		       "user defined",""}; //
//    aString opCmd[] =   {"spacing: constant",
//  		       "spacing: geometric",
//  		       "spacing: inverse hyperbolic",
//  		       "spacing: stretch Mapping",
//  		       "spacing: user defined",
//  		       ""}; //
//    // addPrefix(label,prefix,cmd,maxCommands);
//    marchingParametersDialog.addOptionMenu("spacing:", opCmd,opLabel,spacingType);

  int choice=0;
 
  if( boundaryCondition.getLength(0)<2 )
  {
    boundaryCondition.redim(2,3);
    boundaryCondition=domainDimension==3 ? outwardSplay :freeFloating;
  }

  const int maxCommands=numberOfBoundaryConditions+1;
  aString bcCmd[maxCommands];
   
  if( surfaceGrid )
  {
    GUIState::addPrefix(bcChoices,"BC: left (forward) ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: left (forward)[green] ", bcCmd,bcChoices,boundaryCondition(0,0)-1);
    GUIState::addPrefix(bcChoices,"BC: right (forward) ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: right (forward)[red]  ", bcCmd,bcChoices,boundaryCondition(1,0)-1);

    GUIState::addPrefix(bcChoices,"BC: left (backward) ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: left (backward)[green]", bcCmd,bcChoices,boundaryCondition(0,1)-1);
    GUIState::addPrefix(bcChoices,"BC: right (backward) ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: right (backward)[red] ", bcCmd,bcChoices,boundaryCondition(1,1)-1);
  }
  else
  {
    GUIState::addPrefix(bcChoices,"BC: left ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: left [green]", bcCmd,bcChoices,boundaryCondition(0,0)-1);
    GUIState::addPrefix(bcChoices,"BC: right ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: right [red] ", bcCmd,bcChoices,boundaryCondition(1,0)-1);

    GUIState::addPrefix(bcChoices,"BC: bottom ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: bottom [blue]", bcCmd,bcChoices,boundaryCondition(0,1)-1);
    GUIState::addPrefix(bcChoices,"BC: top ",bcCmd,maxCommands);
    marchingParametersDialog.addOptionMenu("BC: top [yellow] ", bcCmd,bcChoices,boundaryCondition(1,1)-1);
  }
  
//   // "project ghost points" : for each side
//   aString opLabel2[] = {"extrapolate an extra ghost line",
//                         "use last line as ghost line",
// 		        ""}; 
//   // addPrefix(label,prefix,cmd,maxCommands);
//   marchingParametersDialog.addOptionMenu("ghost line option:", opLabel2,opLabel2,ghostLineOption);

  aString tbCommands[] = {"stop on negative cells",
                          "march along normals",
                          "project normals on matching boundaries",
                          "correct projection of initial curve",
                          "apply boundary conditions to start curve",
                          "project points onto reference surface",
			  ""};
  int tbState[8];
  if( projectGhostPoints.getLength(0)<2 )
  {
    projectGhostPoints.redim(2,3);   // created by setup
    projectGhostPoints=true;
  }
  tbState[0] = (int)stopOnNegativeCells;
  tbState[1] = 0;
  tbState[2] = (int)projectNormalsOnMatchingBoundaries;
  tbState[3] = (int)correctProjectionOfInitialCurves;
  tbState[4] = (int)applyBoundaryConditionsToStartCurve;
  tbState[5] = (int)projectOntoReferenceSurface==true; 

  int numColumns=1;
  marchingParametersDialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "uniform dissipation"; 
  sPrintF(textStrings[nt], "%g", uniformDissipationCoefficient); nt++; 

  textLabels[nt] = "boundary dissipation"; 
  sPrintF(textStrings[nt], "%g", boundaryUniformDissipationCoefficient); nt++; 

  textLabels[nt] = "dissipation transition"; 
  sPrintF(textStrings[nt], "%i (>0 : use boundary dissipation)", dissipationTransition); nt++; 

  textLabels[nt] = "volume smooths"; 
  sPrintF(textStrings[nt], "%i", numberOfVolumeSmoothingIterations);  nt++; 

//    textLabels[nt] = "implicit coefficient"; 
//    sPrintF(textStrings[nt], "%g", implicitCoefficient); nt++; 

  textLabels[nt] = "equidistribution"; 
  sPrintF(textStrings[nt], "%g (in [0,1])",equidistributionWeight); nt++; 

  textLabels[nt] = "arclength weight"; 
  sPrintF(textStrings[nt], "%g (for equidistribution)",arcLengthWeight); nt++; 

  textLabels[nt] = "curvature weight"; 
  sPrintF(textStrings[nt], "%g (for equidistribution)",curvatureWeight); nt++; 

//    textLabels[nt] = "curvature speed"; 
//    sPrintF(textStrings[nt], "%g (in [0,1])",curvatureSpeedCoefficient); nt++; 

//    textLabels[nt] = "geometric stretch factor"; 
//    sPrintF(textStrings[nt], "%g ",geometricFactor); nt++; 

//    real stretchFactor=10.;
//    textLabels[nt] = "inv hyp stretch factor"; 
//    sPrintF(textStrings[nt], "%g ",stretchFactor); nt++; 

  textLabels[nt] = "normal blending"; 
  if( domainDimension==2 )
    sPrintF(textStrings[nt],"%i, %i (lines: left, right)",
	    numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0]);
  else
    sPrintF(textStrings[nt],"%i, %i, %i, %i (lines, left,right,bottom,top)",
	    numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0],
	    numberOfLinesForNormalBlend[0][1],numberOfLinesForNormalBlend[1][1]);
  nt++; 

  textLabels[nt] = "outward splay"; 
  if( domainDimension==2 )
    sPrintF(textStrings[nt],"%g, %g (left, right for outward splay BC)",
	    splayFactor[0][0],splayFactor[1][0]);
  else
    sPrintF(textStrings[nt],"%g, %g, %g, %g (left,right,bottom,top for outward splay BC)",
	    splayFactor[0][0],splayFactor[1][0],
	    splayFactor[0][1],splayFactor[1][1]);
  nt++; 

  textLabels[nt] = "boundary offset"; 
  if( domainDimension==2 )
    sPrintF(textStrings[nt],"%i, %i, %i, %i (l r b t)",
	    boundaryOffset[0][0],boundaryOffset[1][0],boundaryOffset[0][1],boundaryOffset[1][1]);
  else
    sPrintF(textStrings[nt],"%i, %i, %i, %i, %i, %i (l r b t b f)",
	    boundaryOffset[0][0],boundaryOffset[1][0],boundaryOffset[0][1],boundaryOffset[1][1],
            boundaryOffset[0][2],boundaryOffset[1][2]);

  nt++; 
  assert( nt < numberOfTextStrings-1 );
  
    // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

  // addPrefix(textLabels,prefix,cmd,maxCommands);
  marchingParametersDialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}


int HyperbolicMapping::
assignMarchingParametersDialog(DialogData & marchingParametersDialog, aString bcChoices[] )
// ==========================================================================================
// /Description:
//     Assign values in the  marching parameters dialog to match the current parameter values.
// ==========================================================================================
{
  int choice = spacingType==constantSpacing ? 0 : spacingType==geometricSpacing ? 1 : 2;
  // marchingParametersDialog.getOptionMenu(0).setCurrentChoice(choice); // Spacing

  if( boundaryCondition.getLength(0)<2 )
  {
    boundaryCondition.redim(2,3);
    boundaryCondition=domainDimension==3 ? outwardSplay :freeFloating;
  }

  // for a surface grid we keep track of BC's for forward and reverse  
  marchingParametersDialog.getOptionMenu(0).setCurrentChoice(boundaryCondition(0,0)-1);  // BC: left
  marchingParametersDialog.getOptionMenu(1).setCurrentChoice(boundaryCondition(1,0)-1);  // BC: right
  marchingParametersDialog.getOptionMenu(2).setCurrentChoice(boundaryCondition(0,1)-1);  // BC: bottom
  marchingParametersDialog.getOptionMenu(3).setCurrentChoice(boundaryCondition(1,1)-1);  // BC: top 

//   if( (domainDimension==2 || surfaceGrid) && growthOption==1 )
//   {
//     marchingParametersDialog.getOptionMenu(3).setSensitive(false);
//     marchingParametersDialog.getOptionMenu(4).setSensitive(false);
//   }
//   else
//   {
//     marchingParametersDialog.getOptionMenu(3).setSensitive(true);
//     marchingParametersDialog.getOptionMenu(4).setSensitive(true);
//   }
  
  if( projectGhostPoints.getLength(0)<2 )
  {
    projectGhostPoints.redim(2,3);   // created by setup
    projectGhostPoints=true;
  }
  marchingParametersDialog.setToggleState("project ghost left",projectGhostPoints(0,0));
  marchingParametersDialog.setToggleState("project ghost bottom",projectGhostPoints(0,1));
  marchingParametersDialog.setToggleState("plot BC mappings",plotBoundaryConditionMappings);
  marchingParametersDialog.setToggleState("project ghost right",projectGhostPoints(1,0));
  marchingParametersDialog.setToggleState("project ghost top",projectGhostPoints(1,1));

  aString line;

  marchingParametersDialog.setTextLabel("uniform dissipation",sPrintF(line, "%g", uniformDissipationCoefficient));  
  marchingParametersDialog.setTextLabel("boundary dissipation",sPrintF(line, "%g", boundaryUniformDissipationCoefficient));  
  marchingParametersDialog.setTextLabel("dissipation transition",sPrintF(line, "%i  (>0 : use boundary dissipation)",
            dissipationTransition));  
  marchingParametersDialog.setTextLabel("volume smooths",sPrintF(line, "%i", numberOfVolumeSmoothingIterations));   
//  marchingParametersDialog.setTextLabel("implicit coefficient",sPrintF(line, "%g", implicitCoefficient));  
  marchingParametersDialog.setTextLabel("equidistribution",sPrintF(line, "%g (in [0,1])",equidistributionWeight));  
  marchingParametersDialog.setTextLabel("arclength weight",sPrintF(line, "%g (for equidistribution)",arcLengthWeight));  
  marchingParametersDialog.setTextLabel("curvature weight",sPrintF(line, "%g (for equidistribution)",curvatureWeight));  
//  marchingParametersDialog.setTextLabel("curvature speed",sPrintF(line, "%g (in [0,1])",curvatureSpeedCoefficient));  
//    marchingParametersDialog.setTextLabel("geometric stretch factor",sPrintF(line, "%g ",geometricFactor));  

//    real stretchFactor=10.;  // ** fix **
//    marchingParametersDialog.setTextLabel("inv hyp stretch factor",sPrintF(line, "%g ",stretchFactor));  
  if( domainDimension==2 )
    marchingParametersDialog.setTextLabel("normal blending",sPrintF(line,"%i, %i (lines: left, right)",
	    numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0]));
  else
    marchingParametersDialog.setTextLabel("normal blending",sPrintF(line,"%i, %i, %i, %i (lines, left,right,bottom,top)",
	    numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0],
	    numberOfLinesForNormalBlend[0][1],numberOfLinesForNormalBlend[1][1]));
  

  if( domainDimension==2 )
    marchingParametersDialog.setTextLabel("outward splay",sPrintF(line,"%g, %g (left, right for outward splay BC)",
	    splayFactor[0][0],splayFactor[1][0]));
  else
    marchingParametersDialog.setTextLabel("outward splay",sPrintF(line,"%g, %g, %g, %g (left,right,bottom,top for outward splay BC)",
	    splayFactor[0][0],splayFactor[1][0],
	    splayFactor[0][1],splayFactor[1][1]));

  

  if( domainDimension==2 )
    marchingParametersDialog.setTextLabel("boundary offset",sPrintF(line,"%i, %i, %i, %i (l r b t)",
	    boundaryOffset[0][0],boundaryOffset[1][0],boundaryOffset[0][1],boundaryOffset[1][1]));
  else
    marchingParametersDialog.setTextLabel("boundary offset",sPrintF(line,"%i, %i, %i, %i, %i, %i (l r b t b f)",
	    boundaryOffset[0][0],boundaryOffset[1][0],boundaryOffset[0][1],boundaryOffset[1][1],
            boundaryOffset[0][2],boundaryOffset[1][2]));

  return 0;
}

int HyperbolicMapping::
findNormalsToStartCurve(RealArray & r, RealArray & normal, int directionToMarch)
// ========================================================================================
// /Description:
//    Determine the marching direction vector (normal) to the start curve.
// /r (input) : find normals at these points on the unit 
// /normal (output): 
// /directionToMarch (input) : 0=forward, 1=backward
// ========================================================================================
{
  
  assert( startCurve!=NULL );
  
  Range xAxes(0,rangeDimension-1);
  Range I=r.dimension(0);
  int n=r.getLength(0);

  assert( n>1 );  // project needs n>1 in order to fix corners.
  
  RealArray x(I,3), xr(I,3,2);
  xr=0.;  // needed for dec
  
  startCurve->mapS(r,x,xr);


  x.reshape(n,1,1,3);
  xr.reshape(n,1,1,3,2);

  // The indexRange is used by the project function
  indexRange.redim(2,3);
  indexRange=0;
  indexRange(1,0)=n-1;
  
  // project curve and get normals
  bool setBoundaryConditions=false;
  const int marchingDirection=directionToMarch==0  ? 1 : -1; 
  bool initialStep=true;
  project( x,marchingDirection,xr,setBoundaryConditions,initialStep );

  x.reshape(n,3);
  xr.reshape(n,3,2);

  normal.redim(I,3);
  normal(I,0)=(xr(I,1,1)*xr(I,2,0)-xr(I,2,1)*xr(I,1,0));
  normal(I,1)=(xr(I,2,1)*xr(I,0,0)-xr(I,0,1)*xr(I,2,0));
  normal(I,2)=(xr(I,0,1)*xr(I,1,0)-xr(I,1,1)*xr(I,0,0));

  // scale by the average length
  RealArray norm; 
  norm= (-marchingDirection)/max( REAL_MIN, SQRT( SQR(normal(I,0))+SQR(normal(I,1))+SQR(normal(I,2)) ));
  
  normal(I,0)*=norm;
  normal(I,1)*=norm;
  normal(I,2)*=norm;

  // normal.display("normal");
  
  return 0;
}


int HyperbolicMapping::
findMatchingBoundaryCurve(int side, int axis, int directionToMarch, 
                          GenericGraphicsInterface & gi, bool promptForChanges /* =false */ )
// ============================================================================================
//  /Description:
//     Find boundary condition curves to match to. When we grow a surface grid starting from
// a curve on the edge we normally want to grow the grid so it matches another curve at the
// boundary -- in this routine we look for a curve that connects to the given starting curve at
// the end specified by (side,axis).
// /directionToMarch (input) : 0=forward, 1=backward
// /promptForChanges (input) : if true, query for changes to the matching boundary curves.
// ===========================================================================================
{
  // debug=7;  // ************
  
  // match the grid to an existing boundary curve.
  if( startCurve==NULL )
  {
    printf("You must define a start curve first \n");
    return 1;
  }
  if( debug & 2 )
  {
    printf("\n-------Entering findMatchingBoundaryCurve: look for matching curves for the boundary conditions.\n");
    printf(" > side=%i, directionToMarch=%i, there are currently %i boundary curves. You may define more.\n",
	   side,directionToMarch,numberOfBoundaryCurves);
  }
  
  const int direction = directionToMarch; // !surfaceGrid || growthOption==1 ? axis : axis+1;

  // look for the boundary curve to match to.
  RealArray x(2,3);
  const RealArray & startCurveGrid = startCurve->getGridSerial();
  if( side==0 )
  {
    for( int dir=0; dir<3; dir++ )
    {
      x(0,dir)=startCurveGrid(0,0,0,dir);
      x(1,dir)=startCurveGrid(1,0,0,dir);  // project a nearby point too
    }
  }
  else
  {
    for( int dir=0; dir<3; dir++ )
    {
      x(0,dir)=startCurveGrid(startCurveGrid.getBound(0),0,0,dir);
      x(1,dir)=startCurveGrid(startCurveGrid.getBound(0)-1,0,0,dir);// project a nearby point too
    }
	  
  }
	
  // find the normal to the start curve 
  // (we need to evaluate at two points for the projection+corner correction to work)
  RealArray normal;
  RealArray rn(2,1);
  if( side==0 )
  {
    rn(0,0)=0.;
    rn(1,0)=.01;  // nearby point
  }
  else
  {
    rn(0,0)=1.-.01;  //  put this point first to keep parameterization correct.
    rn(1,0)=1.;
  }
  
  findNormalsToStartCurve(rn, normal, directionToMarch);

  // we need a scale so we can tell if two points are close.
  assert( surface!=NULL );
  real xScale=0.;
  for( int dir=0; dir<rangeDimension; dir++ )
    xScale=max(xScale,(real)surface->getRangeBound(End,dir)-(real)surface->getRangeBound(Start,dir));

  // real epsx = SQR(xScale*1.e-5);
  // const real distanceToBoundaryCurveTolerance=1.e-3; // now in class

  int curve=-1;
  RealArray r(2,2), rr(1,1), x2(2,3), xx(1,3), xr1(1,3,1), xr2(1,3,1);
  real distMin=REAL_MAX;  // distance to closest boundary curve
  real marchingDistanceAlongBoundaryCurve=0.;  // estimated distance we can march along the closest boundary curve.
   
  RealArray distanceToBoundaryCurve(numberOfBoundaryCurves);
  distanceToBoundaryCurve=distMin;

  RealArray normalDotTangent(numberOfBoundaryCurves);
  normalDotTangent=-2.;  // -2 means invalid; valid values are in [-1,1]
 
  const bool useStrictTolerances=!promptForChanges;
  const real scaleFactor= useStrictTolerances ? 1. : 10.;
  const real dotTolerance=useStrictTolerances ? .7 : 1.1;
  const real normalDotTangentTolerance=useStrictTolerances ? .2 : 0.;

  // project onto the closest boundary curve, avoiding boundary curves that may form the start curve
  for (int b=0; b<numberOfBoundaryCurves; b++ )
  {
    assert( boundaryCurves[b]!=NULL );
    Mapping & map = *boundaryCurves[b];
            
    bool nearby=true;
    for( int dir=0; dir<3; dir++ )
    {
      real xa=map.getRangeBound(Start,dir);
      real xb=map.getRangeBound(End,dir);
      if( xa>REAL_MAX*.001 || xb>REAL_MAX*.001 )
      {
	map.getGridSerial();
	xa=map.getRangeBound(Start,dir);
	xb=map.getRangeBound(End,dir);
      }
      assert( xa<REAL_MAX*.001 && xb<REAL_MAX*.001 );

      real tol = xScale*.1;
      if( debug & 2 ) 
          printf(" Check boundary curve %i xScale=%8.2e dir=%i xa=%8.2e xb=%8.2e\n",b,xScale,dir,xa,xb);
      
      if( x(0,dir)< xa-tol || x(0,dir) > xb+tol )
      {
	nearby=false;
	break;
      }
    }
    if( nearby )
    {
      r=-1;
      map.inverseMapS(x,r);
      map.mapS(r,x2);

      real dist1 = SQRT( SQR(x(0,0)-x2(0,0))+SQR(x(0,1)-x2(0,1))+SQR(x(0,2)-x2(0,2)) );
      real dist2 = SQRT( SQR(x(1,0)-x2(1,0))+SQR(x(1,1)-x2(1,1))+SQR(x(1,2)-x2(1,2)) );
      real dist0 = SQRT( SQR(x(0,0)-x(1,0))+SQR(x(0,1)-x(1,1))+SQR(x(0,2)-x(1,2)) );

      distanceToBoundaryCurve(b)=dist1;

      // if dist2 is small then ignore this boundary curve since the starting curve
      // appears to be coincident with it.
      //                                      |
      //                            <-dist0-> | boundary curve
      //     start curve -----------x1-----x0-|
      //                                   <->| dist1
      //                           <--dist2-->
      //                      
      //

      // printf(" side=%i axis=%i boundaryCurve=%i dist1=%e, dist2=%e, dist0=%e\n",side,axis,b,dist1,dist2,dist0);
      // printf(" r=%e and r=%e  x2(1,.)=(%8.2e,%8.2e,%8.2e,) \n",r(0,0),r(1,0), x2(1,0),x2(1,1),x2(1,2) );
      
      if( debug & 2 ) printf(" > boundaryCurve=%i : distance=%e (dist2=%e, dist0=%e) \n",b,dist1,dist2,dist0);
      if( dist1<distMin ||   // new curve is closer than best so far
          dist1<xScale*distanceToBoundaryCurveTolerance*scaleFactor )  // new curve is close enough to double check
      {

         // Check the tangents to the curves instead -- do not choose if nearly parallel ---
        // 
        //   xr1(0,0:2,0:1) : tangent to the start curve
        //   xr2(0,0:2,0:1) : tangent to the boundary curve
        //
        //                         <--xr2--->
        //             ----------------X--------------------boundary curve
        //                             |     /\
        //                      normal | xr1 |
        //                        < -- |     \/
        //                             |
        //                             |
        //                             |
   
        rr = side==0 ? 0. : 1.;
	startCurve->mapS(rr,xx,xr1);
        // evaluate tangent a bit off the ends to avoid problems with NURBS that may have been split at
        // internal corners.
        rr(0,0)=min(.975,max(.025,r(0,0)));  
        xr2=-1.;
	map.mapS(rr,xx,xr2);
	
	real norm1=sqrt(xr1(0,0,0)*xr1(0,0,0)+xr1(0,1,0)*xr1(0,1,0)+xr1(0,2,0)*xr1(0,2,0));
	real norm2=sqrt(xr2(0,0,0)*xr2(0,0,0)+xr2(0,1,0)*xr2(0,1,0)+xr2(0,2,0)*xr2(0,2,0));

	if( norm1>0. && norm2>0. )
	{
          // Here is the tangent of the start curve dotted with the tangent to the boundary curve
          //   the absolute value of dot should be near zero.
	  real dot = (xr1(0,0,0)*xr2(0,0,0)+xr1(0,1,0)*xr2(0,1,0)+xr1(0,2,0)*xr2(0,2,0))/(norm1*norm2);
          if( debug & 2 )
	  {
	    printf(" > potential new closest curve: b=%i, dist1=%8.2e, distMin=%8.2e, dot=%8.2e \n",
                   b,dist1,distMin,dot);
	    printf(" > start curve : tangent=(%8.2e,%8.2e,%8.2e) , bCurve tangent=(%8.2e,%8.2e,%8.2e) (at r=%e)\n",
		   xr1(0,0,0)/norm1,xr1(0,1,0)/norm1,xr1(0,2,0)/norm1,
		   xr2(0,0,0)/norm2,xr2(0,1,0)/norm2,xr2(0,2,0)/norm2,rr(0,0));
	    
	  }
          // Here is the normal to the start curve dotted with the tangent to the boundary curve
          //   the abs() of this value should be near 1.
	  real normalDotCurveTangent=(normal(0,0)*xr2(0,0,0)+normal(0,1)*xr2(0,1,0)+normal(0,2)*xr2(0,2,0))/norm2;
	  normalDotTangent(b)=normalDotCurveTangent;
	  
          if( debug & 2 ) 
            printf(" > Close curve found: r=%e, normal=(%8.2e,%8.2e,%8.2e) normalDotCurveTangent=%e\n",r(0,0),
                      normal(0,0),normal(0,1),normal(0,2),normalDotCurveTangent);

	  if( fabs(dot) < dotTolerance ) // the BC curve should not be parallel to the start curve.
	  {
            // fabs(normalDotCurveTangent) should be near 1 -- but also in the right direction

            real dr = normalDotCurveTangent>=0. ? 1.-r(0,0) : r(0,0);
            real distanceAlongBoundaryCurve=sqrt( SQR(xr2(0,0,0))+SQR(xr2(0,1,0))+SQR(xr2(0,2,0)) )*dr;
	    
//  	    if( (r(0,0)<.1 && normalDotCurveTangent<0.2) || 
//                  (r(0,0)>.9 && normalDotCurveTangent>0.2) ||
//  		fabs(normalDotCurveTangent)<.2 )
	    if( fabs(normalDotCurveTangent)<normalDotTangentTolerance )
 	    {
	      if( debug & 2 )
                 printf(" > XXX reject the curve %i since it seems to be going in the wrong direction. "
                         " |normalDotCurveTangent|=%e < .2  XXX\n",b,normalDotCurveTangent);
 	    }
	    else if( distanceAlongBoundaryCurve<=0. )
	    {
	      if( debug & 2 )
                 printf(" > XXX reject the curve %i since it goes in the wrong direction, "
                        "est. dist we can march along the curve is %9.3e (dr=%8.2e, r=%8.2e, rr=%8.2e, normalDotCurveTangent=%8.2e ). XXX\n",b,
                         distanceAlongBoundaryCurve,dr,r(0,0),rr(0,0),normalDotCurveTangent);
	    }
	    else if( distanceAlongBoundaryCurve<marchingDistanceAlongBoundaryCurve &&
                     ( dist1>distMin || distMin< xScale*distanceToBoundaryCurveTolerance*.1  ) )
	    {
	      if( debug & 2 )
                 printf(" > XXX reject the curve %i since we cannot march as far along it as curve %i. XXX\n",b,curve);
	    }
	    else
	    {
	      if( debug & 2 ) printf(" > curve %i dist=%e is the chosen as the current closest curve\n"
                                     "    estimated distance we can march along the curve is %9.3e (dr=%8.2e)\n",
                           b,dist1,distanceAlongBoundaryCurve,dr);
	      distMin=dist1;
              marchingDistanceAlongBoundaryCurve=distanceAlongBoundaryCurve;
	      curve=b;
	    }
	  }
          else
	  {
	    if( debug & 2 ) printf(" > XXX reject the curve %i since fabs(dot)>.7 dot=%e\n",b,dot);
	  }
	}
	else
	{
	  printf("findMatchingBoundaryCurve:WARNING: tangent of edge curve has norm==0!\n");
	}

      }
    }
  }

  if( curve>=0 && distMin < xScale*distanceToBoundaryCurveTolerance )
  {
    printf("\n > *** Start curve in %s direction (%s side) will try to follow boundary curve %i\n",
           (direction==0 ? "forward" : "backward"),(side==0 ? "left" : "right"),curve);
    printf(" >     Change the boundary condition if you do not want this behaviour\n");
    if( true || sum(abs(distanceToBoundaryCurve<xScale*distanceToBoundaryCurveTolerance))>1 )
    {
      printf(" >  Potential boundary curves: [angle=angle between start curve and boundary curve should be "
              "near +-90 (degrees)]\n     ");
    
      for( int b=0; b<numberOfBoundaryCurves; b++ )
      {
	if( distanceToBoundaryCurve(b)<xScale*distanceToBoundaryCurveTolerance )
	{
          if( fabs(normalDotTangent(b))<=1. )
	  {
            real angle=acos(normalDotTangent(b))*180./Pi-90.;
            printf(" %i (dist=%7.1e,angle=%4.1f), ",b,distanceToBoundaryCurve(b),angle);
	  }
          else
            printf(" %i (dist=%7.1e), ",b,distanceToBoundaryCurve(b));
	}
      }
      printf(" *** <\n");
    }

    if( boundaryConditionMappingWasNewed[side][direction] )
      delete boundaryConditionMapping[side][direction];
    boundaryConditionMapping[side][direction]= boundaryCurves[curve]; 
    boundaryConditionMapping[side][direction]->incrementReferenceCount();
    boundaryConditionMappingWasNewed[side][direction]=false;

    
  }
  else
  {
    if( !promptForChanges )
    {
      printf(" > xxx Start curve in %s direction (%s side) : there are no nearby boundary curves to follow.\n"
	     " > xxx You may want to create a new boundary curve and try again.'\n"
	     " > xxx You could also try increasing 'tol', the `boundary curve matching tolerance'.\n",
	     (direction==0 ? "forward" : "backward"),(side==0 ? "left" : "right"));
    }
    
    if( curve>=0 )
      printf(" > xxx The nearest potential curve was %i distMin=%e >= xScale*tol=%e (tol=%e).\n",
	     curve,distMin,xScale*distanceToBoundaryCurveTolerance,distanceToBoundaryCurveTolerance);
    
    if( sum(abs(distanceToBoundaryCurve<xScale*distanceToBoundaryCurveTolerance))>0 )
    { 
      printf(" >  Potential boundary curves: [angle=angle between start curve and boundary curve should be "
              "near +-90 (degrees)]\n     ");
    
      for( int b=0; b<numberOfBoundaryCurves; b++ )
      {
	if( true || distanceToBoundaryCurve(b)<xScale*distanceToBoundaryCurveTolerance )
	{
          if( fabs(normalDotTangent(b))<1.1 )
	  {
            real angle=acos(normalDotTangent(b))*180./Pi-90.;
            printf(" %i (dist=%7.1e,angle=%4.1f), ",b,distanceToBoundaryCurve(b),angle);
	  }
          else
            printf(" %i (dist=%7.1e), ",b,distanceToBoundaryCurve(b));
	}
      }
      printf(" *** <\n");
    }

    boundaryCondition(side,direction)=1;   // reset to default if we don't find a matching BC mapping
    
  }

// --------------------------------------
  if( promptForChanges )
  {
    if( curve>=0 && distMin < xScale*distanceToBoundaryCurveTolerance )
    {
      printF("\nINFO:The start curve in the %s direction (%s side) currently matches to"
	     " boundary curve %i\n",
	     (direction==0 ? "forward" : "backward"),(side==0 ? "left" : "right"),curve);
    }
    else
    {
      printF("\nINFO:No matching boundary curve was automatically found for the \n"
             "       start curve in the %s direction (%s side)\n",
	     (direction==0 ? "forward" : "backward"),(side==0 ? "left" : "right"));
    }
    if( numberOfBoundaryCurves<=0 )
    {
      printF("ERROR: there are no boundary curves defined yet. You can create boundary curves by joining\n" 
             "       edges together\n");
    }
    else
    {
      printF("Available boundary curves are numbered 0...%i. Enter the number of a new boundary curve.\n",
	     numberOfBoundaryCurves-1 );
          
      for( ;; )
      {
	aString answer2;
	gi.inputString(answer2,"Enter the number of a new boundary curve to use "
		       "(or `done' to remain unchanged).");
	if( answer2=="done" )
	{
	  break;
	}
	else 
	{
	  int newCurve=-1;
	  sScanF(answer2,"%i",&newCurve);
	  if( newCurve>=0 && newCurve<numberOfBoundaryCurves )
	  {
	    curve=newCurve;
	    if( boundaryConditionMappingWasNewed[side][direction] )
	      delete boundaryConditionMapping[side][direction];
	    else
	    {
	      boundaryConditionMapping[side][direction]->decrementReferenceCount(); 
	    }
		  
	    boundaryConditionMapping[side][direction]=boundaryCurves[curve];
	    boundaryConditionMapping[side][direction]->incrementReferenceCount();
	    boundaryConditionMappingWasNewed[side][direction]=false;

	    break;
	  }
	  else
	  {
	    gi.outputString(sPrintF("You have entered an invalid boundary curve=%i. Try again",newCurve));
	    gi.stopReadingCommandFile();
	  }
	}
      }

    }
  }

  // estimate the normal blending
  // We need to increase this value if the matching boundary curve is not normal to the start curve.
  if( boundaryConditionMapping[side][direction]!=NULL &&
      curve>=0 && fabs(normalDotTangent(curve))<=1.001 )
  {
    real scaledAngle=1.-fabs(normalDotTangent(curve));  // 0 <= scaledAngle <=1 
    
    numberOfLinesForNormalBlend[side][0]= int(3 + max(0.5,scaledAngle*90));  // default number is 3
    if( numberOfLinesForNormalBlend[side][0] > 3 )
    {
      printf("\nINFO:I am setting the normal blending (%s side) to %i lines since the start curve\n"
	     "    :is not orthogonal to the boundary curve. You may want to change this further.\n\n",
	     (side==0 ? "left" : "right"),numberOfLinesForNormalBlend[side][0]);
    }
  }

  // -----------------------------------------

  if( debug & 2 ) printf("-------Leaving findMatchingBoundaryCurve.\n\n");
  return 0;
}


bool HyperbolicMapping::
updateMarchingParameters(aString & answer, DialogData & marchingParametersDialog, aString bcChoices[],
                         MappingInformation & mapInfo )
// ==========================================================================================
// /Description:
//     Assign values in the  marching parameters dialog to match the current parameter values.
//
// /answer (input) : check this answer to see if it is a marching parameter.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  bool returnValue=true;

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const bool plotObjectSave=plotObject;
  plotObject=false;    // by default no need to redraw
  
  aString line;
  int len;

  if( answer.matches("BC:") )
  {
    int side=0,axis=0;
    if( (len=answer.matches("BC: left (backward) ")) || 
        (len=answer.matches("BC: left (forward) ")) || 
        (len=answer.matches("BC: left ")) )
    {
      side=0;  axis=0;
      if( answer.matches("BC: left (backward) ") )
      {
	axis=1;
      }
	  
    }
    else if( (len=answer.matches("BC: right (backward) ")) || // look for longest match first
	     (len=answer.matches("BC: right (forward) ")) || 
	     (len=answer.matches("BC: right ")) )
    {
      side=1;  axis=0;
      if( answer.matches("BC: right (backward) ") )
      {
	axis=1;
      }
    }
    else if( (len=answer.matches("BC: bottom" )) )
    {
      side=0;  axis=1;
    }
    else if( (len=answer.matches("BC: top" )) )
    {
      side=1;  axis=1;
    }

    const int length=answer.length();
    while( len<length && answer[len]==' ' ) // skip blanks
      len++;
    

    line=answer(len,answer.length()-1);

    // search for the BC name in the list of choices.
    int bcChosen=-1; 
    for( int i=0; bcChoices[i]!=""; i++ )
    {
      if( line==bcChoices[i] )
      {
	bcChosen=i;
	break;
      }
    }
    if( bcChosen==-1 )
    {
      printf("ERROR: unknown BC: answer=[%s], line=[%s]\n",(const char*)answer,(const char*)line);
      printf(" Available boundary conditions are:\n");
      for( int i=0; bcChoices[i]!=""; i++ )
      {
	printf("[%s]\n",(const char*)bcChoices[i]);
      }
    }
    else if( bcChosen>=0 )
    {
      boundaryCondition(side,axis)=bcChosen+1;  // BC's start at 1

      const int menuNumber=side+2*axis;

      if( line=="trailing edge" )
      {

      }
      else if( line=="periodic" )
      {
        setIsPeriodic(axis,functionPeriodic);
	
	for( int side1=Start; side1<=End; side1++ )
	  setBoundaryCondition(side1,axis,-1);
      }
      else if( line=="match to a plane" )
      {
	    
	printf("A plane (or rhombus) is defined by 3 points. Choose the points in the order\n"
	       "  x1,y1,z1 : lower left corner       3-----X \n"
	       "  x2,y2,z2 : lower right corner      |     | \n"
	       "  x3,y3,z3 : upper left corner       1-----2 \n");
	real x1=0.,y1=0.,z1=0., x2=1.,y2=0.,z2=0., x3=0.,y3=1.,z3=0.;
	gi.inputString(line,sPrintF(line,"Enter x1,y1,z1, x2,y2,z2, x3,y3,z3 (default=(%6.2e,%6.2e,%6.2e)"
				    " ,(%6.2e,%6.2e,%6.2e),(%6.2e,%6.2e,%6.2e) ): ",
				    x1,y1,z1, x2,y2,z2, x3,y3,z3));
	if( line!="" )
	{
	  sScanF(line,"%e %e %e %e %e %e %e %e %e",&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3);
	}	    
            
        boundaryCondition(side,axis)=matchToMapping;

	if( boundaryConditionMappingWasNewed[side][axis] )
	  delete boundaryConditionMapping[side][axis];

	boundaryConditionMapping[side][axis]= new PlaneMapping(x1,y1,z1, x2,y2,z2, x3,y3,z3 );
        boundaryConditionMapping[side][axis]->incrementReferenceCount();   // ******************** add this
	boundaryConditionMappingWasNewed[side][axis]=true;

	setBoundaryConditionAndOffset(side,axis,(domainDimension==3 ? (int)outwardSplay : (int)freeFloating));
        // boundaryOffset[side][axis]=0;
	
        plotObject=true;
      }
      else if( line=="match to a boundary curve" )
      {
        // NOTE axis should equal direction
	int direction= (answer.matches("BC: left (backward) ") ||
			       answer.matches("BC: right (backward) ")) ? 1 : 0; // note 0=forward, 1=backward
	// look again for matching boundary curves.
        const bool promptForChanges=true;
	findMatchingBoundaryCurve( side,axis, direction, gi, promptForChanges );

	if( boundaryConditionMapping[side][axis]!=NULL )
	{
	  setBoundaryConditionAndOffset(side,axis,bcChosen+1);
  	  projectGhostPoints(side,0)=0;  // don't project the ghost line since it will probably be off the surface

	  if( surfaceGrid )
	  {
	    marchingParametersDialog.setTextLabel("normal blending",sPrintF(line,"%i, %i (lines: left, right)",
			  	    numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0]));
	  }
	}
	else if( boundaryConditionMapping[side][axis]==NULL )
	{
	  printf("Choose BC:INFO:No matching boundary curve has been found automatically.\n");
	  printf("Choose BC:INFO:You may want to first create a new boundary curve to use\n"
		 "by choosing 'Picking: create boundary curve' and selecting a curve or curves to use\n");
	  bcChosen=0;
	  boundaryCondition(side,axis)=bcChosen+1;
	  printf("Choose BC:INFO: Resetting bc(%i,%i)=%i\n",side,axis,boundaryCondition(side,axis));
	  
	  setBoundaryConditionAndOffset(side,axis,bcChosen+1);   // *wdh* 011121: default BC with no match is 1	  
	}
      }
      else if( line=="match to a mapping" )
      {
	// Make a menu with the Mapping names (only curves or surfaces!)
	int numberOfMaps=mapInfo.mappingList.getLength();
	int numberOfFaces=numberOfMaps*(6+1);  // up to 6 sides per grid plus grid itself
	aString *menu2 = new aString[numberOfFaces+2];
	IntegerArray subListNumbering(numberOfFaces);
        int mappingListStart=0; 
	int i, j=0;
	for( i=0; i<numberOfMaps; i++ )
	{
	  MappingRC & map = mapInfo.mappingList[i];
	  if( ( map.getDomainDimension()== (map.getRangeDimension()-1)  ||
		(map.getDomainDimension()==map.getRangeDimension() && map.getDomainDimension()>1 ) )
	      && map.mapPointer!=this )
	  {
	    if( map.getDomainDimension()==map.getRangeDimension()-1 )
	    {
	      subListNumbering(j)=i;
	      menu2[j++]=map.getName(mappingName);
	    }
	    else
	    {
	      subListNumbering(j)=i;
	      menu2[j++]=map.getName(mappingName);  
	      // include all sides that are physical boundaries.
	      for( int dir=axis1; dir<map.getDomainDimension(); dir++ )
	      {
		for( int side2=Start; side2<=End; side2++ )
		{
		  if( map.getBoundaryCondition(side2,dir)>0 )
		  {
		    subListNumbering(j)=i;
		    menu2[j++]=sPrintF(line,"%s (side2=%i,axis=%i)",(const char *)map.getName(mappingName),
				       side2,dir);
		  }
		}
	      }
	    }
	  }
	}
	int mappingListEnd=j;
	if( j==0 )
	{
	  gi.outputString("HyperbolicMapping::WARNING: There are no appropriate curves/surfaces to choose from");
	}
	menu2[j++]="none"; 
	menu2[j]="";   // null string terminates the menu
	  
       // replace menu with a new cascading menu if there are too many items. (see viewMappings.C)
       gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

	int mapNumber = gi.getMenuItem(menu2,line,sPrintF("Match side=%i,axis=%i to which mapping?",
							  side,axis));
        gi.indexInCascadingMenu( mapNumber,mappingListStart,mappingListEnd);

	delete [] menu2;
		
	mapNumber=subListNumbering(mapNumber);  // map number in the original list

        // *wdh* 081101 : use the untrimmed surface for projecting 
	// *wdh* 081101 Mapping & map =* mapInfo.mappingList[mapNumber].mapPointer;
        Mapping *mapPointer = mapInfo.mappingList[mapNumber].mapPointer;
	if( mapPointer->getClassName()=="TrimmedMapping" )
	{
	  mapPointer = ((TrimmedMapping*)mapInfo.mappingList[mapNumber].mapPointer)->untrimmedSurface();
	  printF("*** HyperbolicMapping::INFO: using the untrimmed surface for matching to\n");
	}
         
	Mapping & map = *mapPointer;

	if( map.getDomainDimension()==map.getRangeDimension()-1 )
	{
	  if( boundaryConditionMappingWasNewed[side][axis] )
	    delete boundaryConditionMapping[side][axis];
	  boundaryConditionMapping[side][axis]=&map;
	  boundaryConditionMappingWasNewed[side][axis]=false;
	}
	else if( map.getDomainDimension()==map.getRangeDimension() )
	{
	  // we may need to build a Mapping that corresponds to a side of a volume grid.
	  int side2=-1, dir=-1;
	  int length=line.length();
	  for( int j=0; j<length-6; j++ )
	  {
	    if( line(j,j+5)=="(side=" ) 
	    {
	      sScanF(line(j,length-1),"(side=%i axis=%i",&side2,&dir); // remember that commas are removed
	      if( side2<0 || dir<0 )
	      {
		cout << "Error getting (side,axis) from choice!\n";
		Overture::abort("error");
	      }
	      if( boundaryConditionMappingWasNewed[side2][dir] )
		delete boundaryConditionMapping[side2][dir];
	      boundaryConditionMapping[side2][dir]= new ReductionMapping(map,dir,side2);
	      boundaryConditionMappingWasNewed[side2][dir]=true;
		      
	      // printf(" create a mapping for (side,axis)=(%i,%i) for curve[%i] \n",side2,dir,i);
	      break;
	    }
	  }
	  if( side2<0 || dir<0 )
	  {
	    // printf("Setting curve[%i] \n",i);
	    if( boundaryConditionMappingWasNewed[side2][dir] )
	      delete boundaryConditionMapping[side2][dir];
	    boundaryConditionMapping[side2][dir]=mapInfo.mappingList[mapNumber].mapPointer; 
	    boundaryConditionMappingWasNewed[side2][dir]=false;
	  }
	}
	else
	{
	  Overture::abort("error");
	}
	  

	setBoundaryConditionAndOffset(side,axis,(domainDimension==3 ? (int)outwardSplay : (int)freeFloating));
        // boundaryOffset[side][axis]=0;
        plotObject=true;
	
      }
      
      marchingParametersDialog.getOptionMenu(menuNumber).setCurrentChoice(boundaryCondition(side,axis)-1);
    }
    
  }
//   else if( answer=="extrapolate an extra ghost line" )
//   {
//     ghostLineOption=extrapolateAnExtraGhostLine;
//   }
//   else if( answer=="use last line as ghost line" )
//   {
//     ghostLineOption=useLastLineAsGhostLine;
//   }
  else if( (len=answer.matches("project ghost left")) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&projectGhostPoints(0,0));
    marchingParametersDialog.setToggleState("project ghost left",projectGhostPoints(0,0)==1);
  }
  else if( (len=answer.matches("project ghost right")) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&projectGhostPoints(1,0));
    marchingParametersDialog.setToggleState("project ghost right",projectGhostPoints(1,0)==1);  // these are in a funny order
  }
  else if( (len=answer.matches("project ghost bottom")) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&projectGhostPoints(0,1));
    marchingParametersDialog.setToggleState("project ghost bottom",projectGhostPoints(0,1)==1);
  }
  else if( (len=answer.matches("project ghost top")) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&projectGhostPoints(1,1));
    marchingParametersDialog.setToggleState("project ghost top",projectGhostPoints(1,1)==1);
  }
  else if( (len=answer.matches("plot BC mappings")) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&plotBoundaryConditionMappings);
    marchingParametersDialog.setToggleState("plot BC mappings",plotBoundaryConditionMappings);
    plotObject=true;// replot
  }
  else if( (len=answer.matches( "stop on negative cells" )) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&stopOnNegativeCells);
    marchingParametersDialog.setToggleState("stop on negative cells",stopOnNegativeCells==true);
  }
  else if( marchingParametersDialog.getToggleValue( answer,"project normals on matching boundaries",
                                                    projectNormalsOnMatchingBoundaries) ){} // 
  else if( marchingParametersDialog.getToggleValue( answer,"correct projection of initial curve",
                                                    correctProjectionOfInitialCurves) ){} // 
  else if( marchingParametersDialog.getToggleValue( answer,"apply boundary conditions to start curve",
                                                    applyBoundaryConditionsToStartCurve) ){} // 
  else if( (len=answer.matches("project points onto reference surface")) )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); projectOntoReferenceSurface=value;
    marchingParametersDialog.setToggleState("project points onto reference surface",projectOntoReferenceSurface);
    if( projectOntoReferenceSurface )
      printF("project points onto reference surface when using the triangulation of the surface.\n");
    
    else
      printF("Do not project points onto reference surface when using the triangulation of the surface.\n");

    printF(" (this option is used when a matching boundary surface is a composite surface.)\n");
  }
  else if( (len=answer.matches("march along normals" )) )
  {
    int marchAlongNormals;
    sScanF( answer(len,answer.length()-1),"%i",&marchAlongNormals);
    marchingParametersDialog.setToggleState("march along normals",marchAlongNormals==true);
    if( marchAlongNormals )
    {
      uniformDissipationCoefficient=0.;
      numberOfVolumeSmoothingIterations=0;
      printf("Setting uniform dissipation=0 and volume smooths=0 so that marching will be in the normal direction\n");
    }
    else
    {
      uniformDissipationCoefficient=0.1;
      numberOfVolumeSmoothingIterations=20;
      printf("Resetting uniform dissipation and volume smooths to default.\n");
    }
    marchingParametersDialog.setTextLabel("uniform dissipation",sPrintF(line, "%g", uniformDissipationCoefficient)); 
    marchingParametersDialog.setTextLabel("volume smooths",sPrintF(line, "%i", numberOfVolumeSmoothingIterations));
    
  }
  else if( (len=answer.matches("uniform dissipation")) )
  {
    sScanF( answer(len,answer.length()-1),"%e",&uniformDissipationCoefficient);
    marchingParametersDialog.setTextLabel("uniform dissipation",sPrintF(line, "%g", uniformDissipationCoefficient));
  }
  else if( (len=answer.matches("boundary dissipation")) )
  {
    sScanF( answer(len,answer.length()-1),"%e",&boundaryUniformDissipationCoefficient);
    marchingParametersDialog.setTextLabel("boundary dissipation",sPrintF(line, "%g", boundaryUniformDissipationCoefficient));
  }
  else if( (len=answer.matches("dissipation transition")) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&dissipationTransition);
    marchingParametersDialog.setTextLabel("dissipation transition",sPrintF(line, "%i (>0 : use boundary dissipation)", dissipationTransition));
  }
  else if( (len=answer.matches("volume smooths")) )
  {
    sScanF( answer(len,answer.length()-1),"%i",&numberOfVolumeSmoothingIterations);
    marchingParametersDialog.setTextLabel("volume smooths",sPrintF(line, "%i", numberOfVolumeSmoothingIterations));
  }
  else if( (len=answer.matches("implicit coefficient"))  )
  {
    sScanF( answer(len,answer.length()-1),"%e",&implicitCoefficient);
    marchingParametersDialog.setTextLabel("implicit coefficient",sPrintF(line, "%g", implicitCoefficient));
  }
  else if( (len=answer.matches("equidistribution"))  )
  {
    sScanF( answer(len,answer.length()-1),"%e",&equidistributionWeight);
    if( arcLengthWeight==0. && curvatureWeight==0. )
    {
      arcLengthWeight=1.;
      if( surfaceGrid )
	curvatureWeight=1.;

      marchingParametersDialog.setTextLabel("arclength weight",sPrintF(line, "%g (for equidistribution)",arcLengthWeight)); 
      marchingParametersDialog.setTextLabel("curvature weight",sPrintF(line, "%g (for equidistribution)",curvatureWeight)); 
    }
    marchingParametersDialog.setTextLabel("equidistribution",sPrintF(line, "%g (in [0,1])",equidistributionWeight));

  }
  else if( (len=answer.matches("arclength weight")) )
  {
    sScanF( answer(len,answer.length()-1),"%e",&arcLengthWeight);
    marchingParametersDialog.setTextLabel("arclength weight",sPrintF(line, "%g (for equidistribution)",arcLengthWeight)); 
  }
  else if( (len=answer.matches("curvature weight")) )
  {
    sScanF( answer(len,answer.length()-1),"%e",&curvatureWeight);
    marchingParametersDialog.setTextLabel("curvature weight",sPrintF(line, "%g (for equidistribution)",curvatureWeight)); 
  }
  else if( (len=answer.matches("curvature speed coefficient")) )
  {
    sScanF( answer(len,answer.length()-1),"%e",&curvatureSpeedCoefficient);
//    marchingParametersDialog.setTextLabel("curvature speed coefficient",sPrintF(line, "%g (in [0,1])",curvatureSpeedCoefficient));
  }
//    else if( (len=answer.matches("geometric stretch factor")) )
//    {
//      sScanF( answer(len,answer.length()-1),"%e",&geometricFactor);
//      if( geometricFactor==1. )
//      {
//        printf("WARNING: The geometric stretch factor cannot be 1. setting to 1.00001\n");
//        geometricFactor=1.00001;
//      }
//      marchingParametersDialog.setTextLabel("geometric stretch factor",sPrintF(line, "%g ",geometricFactor));

//      spacingType=geometricSpacing;
//      marchingParametersDialog.getOptionMenu(0).setCurrentChoice(spacingType);

//    }
//    else if( (len=answer.matches("inv hyp stretch factor")) )
//    {
//      real exponent=10.;
//      sScanF( answer(len,answer.length()-1),"%e",&exponent);
//      marchingParametersDialog.setTextLabel("inv hyp stretch factor",sPrintF(line, "%g ",exponent));

//      spacingType=oneDimensionalMappingSpacing;  // ***** set option dialog.
//      marchingParametersDialog.getOptionMenu(0).setCurrentChoice(spacingType);

//      StretchMapping & stretch = *new StretchMapping;
//      if( normalDistribution!=NULL && normalDistribution->decrementReferenceCount()==0 )
//        delete normalDistribution;
//      normalDistribution=&stretch;
//      normalDistribution->incrementReferenceCount();

//      stretch.setStretchingType( StretchMapping::inverseHyperbolicTangent );
//      stretch.setNumberOfLayers(1);
//      stretch.setLayerParameters(0,1.,exponent,0.);

//    }
  else if( (len=answer.matches("normal blending")) )
  {
    if( domainDimension==2 )
    {
      sScanF( answer(len,answer.length()-1),"%i %i",&numberOfLinesForNormalBlend[0][0],
                &numberOfLinesForNormalBlend[1][0]);
      marchingParametersDialog.setTextLabel("normal blending",sPrintF(line,"%i, %i (lines: left, right)",
	    numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0]));
    }
    else
    {
      sScanF( answer(len,answer.length()-1),"%i %i %i %i",
	      &numberOfLinesForNormalBlend[0][0],&numberOfLinesForNormalBlend[1][0],
	      &numberOfLinesForNormalBlend[0][1],&numberOfLinesForNormalBlend[1][1]);
      
      marchingParametersDialog.setTextLabel("normal blending",
                                      sPrintF(line,"%i, %i, %i, %i (lines, left,right,bottom,top)",
				      numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0],
				      numberOfLinesForNormalBlend[0][1],numberOfLinesForNormalBlend[1][1]));
    }
  }
  else if( (len=answer.matches("outward splay")) )
  {
    if( domainDimension==2 )
    {
      sScanF( answer(len,answer.length()-1),"%e %e",&splayFactor[0][0],&splayFactor[1][0]);
      marchingParametersDialog.setTextLabel("outward splay",sPrintF(line,"%g, %g (left, right for outward splay BC)",
	    splayFactor[0][0],splayFactor[1][0]));
    }
    else
    {
      sScanF( answer(len,answer.length()-1),"%e %e %e %e",
	      &splayFactor[0][0],&splayFactor[1][0],
	      &splayFactor[0][1],&splayFactor[1][1]);
      
      marchingParametersDialog.setTextLabel("outward splay",sPrintF(line,"%g, %g, %g, %g "
                                 "(left,right,bottom,top for outward splay BC)",
				      splayFactor[0][0],splayFactor[1][0],
				      splayFactor[0][1],splayFactor[1][1]));
    }
    
  }
  else if( (len=answer.matches("boundary offset")) )
  {
    if( domainDimension==2 )
    {
      sScanF( answer(len,answer.length()-1),"%i %i %i %i",&boundaryOffset[0][0],&boundaryOffset[1][0],
                    &boundaryOffset[0][1],&boundaryOffset[1][1]);

      if( boundaryOffsetOption==0 && boundaryOffset[1][1] < 1 ) // old way 
        printf("INFO: boundaryOffset[1][1] must be at least 1\n");
      
      boundaryOffset[0][0]=max(0,boundaryOffset[0][0]);
      boundaryOffset[1][0]=max(0,boundaryOffset[1][0]);
      boundaryOffset[0][1]=max(0,boundaryOffset[0][1]);
      if( boundaryOffsetOption==0 )
        boundaryOffset[1][1]=max(1,boundaryOffset[1][1]); // old way: *note* this value at least 1
      else
	boundaryOffset[1][1]=max(0,boundaryOffset[1][1]);
      

      marchingParametersDialog.setTextLabel("boundary offset",sPrintF(line,"%i, %i, %i, %i (l r b t)",
		     boundaryOffset[0][0],boundaryOffset[1][0],boundaryOffset[0][1],boundaryOffset[1][1]));
    }
    else
    {
      sScanF( answer(len,answer.length()-1),"%i %i %i %i %i %i",&boundaryOffset[0][0],&boundaryOffset[1][0],
                    &boundaryOffset[0][1],&boundaryOffset[1][1],&boundaryOffset[0][2],&boundaryOffset[1][2]);

      if( boundaryOffsetOption==0 && boundaryOffset[1][2] < 1 ) // old way 
        printf("INFO: boundaryOffset[1][1] must be at least 1\n");

      boundaryOffset[0][0]=max(0,boundaryOffset[0][0]);
      boundaryOffset[1][0]=max(0,boundaryOffset[1][0]);
      boundaryOffset[0][1]=max(0,boundaryOffset[0][1]);
      boundaryOffset[1][1]=max(0,boundaryOffset[1][1]);
      boundaryOffset[0][2]=max(0,boundaryOffset[0][2]);
      if( boundaryOffsetOption==0 )
        boundaryOffset[1][2]=max(1,boundaryOffset[1][2]); // old way: *note* this value at least 1
      else
        boundaryOffset[1][2]=max(0,boundaryOffset[1][2]); // new way

      marchingParametersDialog.setTextLabel("boundary offset",sPrintF(line,"%i, %i, %i, %i, %i, %i (l r b t b f)",
	           boundaryOffset[0][0],boundaryOffset[1][0],boundaryOffset[0][1],boundaryOffset[1][1],
    	           boundaryOffset[0][2],boundaryOffset[1][2]));
    }

    // change the current grid if it has been built
    if( dpm!=NULL )
    {
      RealArray & x = xHyper;

      IntegerArray gid(2,3);
      gid=0;
      for( int axis=0; axis<domainDimension; axis++ )
      {
        int dir = domainDimension==2 && axis==1 ? axis3 : axis;  // marching direction is always axis3
        
	gid(Start,axis)=max(x.getBase(dir),min(x.getBound(dir),gridIndexRange(Start,dir)+boundaryOffset[Start][axis]));
	gid(End  ,axis)=max(x.getBase(dir),min(x.getBound(dir),gridIndexRange(End  ,dir)-boundaryOffset[End  ][axis]));

        setGridDimensions(axis,gid(End,axis)-gid(Start,axis)+1); 
      }
      
      Range xAxes(0,rangeDimension-1);
    
      #ifndef USE_PPP
      if( domainDimension==2 )
      {
	x.reshape(x.dimension(0),x.dimension(2),1,xAxes);

	dpm->setDataPoints(x,3,domainDimension,0,gid);

	x.reshape(x.dimension(0),1,x.dimension(1),xAxes);
      }
      else
	dpm->setDataPoints(x,3,domainDimension,0,gid);
      #else
        OV_ABORT("finish me for parallel");
      #endif

    }
    plotObject=true;
    mappingHasChanged();

  }
  else
  {
    plotObject=plotObjectSave;
    returnValue=false;
  }

  return returnValue;
}


int HyperbolicMapping::
update( MappingInformation & mapInfo )
{
  return update(mapInfo,nullString);
}


static void
getArcLength( Mapping &curve, real & arcLength )
// ==================================================================================
// /Description:
//    Compute the arclength of a curve (approx).
// ==================================================================================
{
  // estimate the area, and arclength, and length scales
  assert( curve.getDomainDimension()==1 && curve.getRangeDimension()==3 );

  int n= 100;
  RealArray r(n,1),x(n,3);
  real dr=1./(n-1);
  r.seqAdd(0.,dr);
  curve.mapS(r,x);

  arcLength=0.;
  for ( int nn=0; nn<n-1; nn++ )
    arcLength+=SQRT( SQR(x(nn+1,0)-x(nn,0))+SQR(x(nn+1,1)-x(nn,1))+SQR(x(nn+1,2)-x(nn,2)) );
}

void HyperbolicMapping::
estimateMarchingParameters( real & estimatedDistanceToMarch, int & estimatedLinesToMarch, int directionToMarch,
                            GenericGraphicsInterface & gi )
// ==========================================================================================
// /Description: 
//   Estimate marching parameters for surface grids, call this routine after a new starting
// curve has been chosen. This function will also look for adjacent boundary curves to use
// as boundary conditions.
// /directionToMarch (input) : 0=forward, 1=backward
// =========================================================================================
{
  // Here are defaults
  if( spacingOption==spacingFromDistanceAndLines )
  {
    estimatedLinesToMarch=11;
    estimatedDistanceToMarch=targetGridSpacing <=0. ? 1. : estimatedLinesToMarch*targetGridSpacing;
    
  }
  else if( spacingOption==distanceFromLinesAndSpacing )
  {
    estimatedLinesToMarch=11;
    if( initialSpacing>0. )
    {
      if( constantSpacing==geometricSpacing && geometricFactor>1. )
      {
        // dist = a*(r^n-1)/(r-1)
        estimatedDistanceToMarch=initialSpacing*(pow(geometricFactor,estimatedLinesToMarch-1.)-1.)/(geometricFactor-1.);
      }
      else
      {
	estimatedDistanceToMarch=estimatedLinesToMarch*initialSpacing;
      }
    }
    else
    {
      estimatedDistanceToMarch=targetGridSpacing <=0. ? 1. : estimatedLinesToMarch*targetGridSpacing;
    }
    
  }
  else if( spacingOption==linesFromDistanceAndSpacing )
  {
    estimatedDistanceToMarch=1.;
    estimatedLinesToMarch=11;
    if( initialSpacing>0. )
    {
      estimatedLinesToMarch=int(estimatedDistanceToMarch/initialSpacing+1.5);
    }
    else if( targetGridSpacing>0. )
    {
      estimatedLinesToMarch=int(estimatedDistanceToMarch/targetGridSpacing+1.5);
    }
  }
  else
  {
    printf("ERROR unknown value for spacingOption=%i\n",(int)spacingOption);
    Overture::abort();
  }
  
  if( !surfaceGrid || startCurve==NULL )
    return;
  
  
  if( startCurve==NULL )
  {
    return;
  }
  

  real arcLen,startCurveArcLength;

  assert( directionToMarch==0 || directionToMarch==1 );
  
  const int direction = directionToMarch; // !surfaceGrid || growthOption>0 ? axis1 : axis1+1;

  if( boundaryCondition(Start,direction)==matchToABoundaryCurve ||
      boundaryCondition(End  ,direction)==matchToABoundaryCurve )
  {
    // estimate marching distance: base on the length of matching boundary curves

    arcLen=-1.;
    for( int side=Start; side<=End; side++ )
    {
      if( boundaryCondition(side,direction)==matchToABoundaryCurve )
      {
	findMatchingBoundaryCurve( side, axis1, directionToMarch, gi  );

	if( boundaryConditionMapping[side][direction]!=NULL )
	{
          // A matching boundary curve was found
          setBoundaryConditionAndOffset(side,axis1,(domainDimension==3 ? (int)outwardSplay : (int)freeFloating));  
          // boundaryOffset[side][axis1]=0;

	  real boundaryArcLength=0.;
	  ::getArcLength( *boundaryConditionMapping[side][direction], boundaryArcLength );
	  if( arcLen<0. )
	    arcLen=boundaryArcLength;
	  else
	    arcLen=min(arcLen,boundaryArcLength);
	}
        else
	{
          setBoundaryConditionAndOffset(side,axis1,0);     // *wdh* 011121: default BC with no match is 0
          // boundaryOffset[side][axis1]=1;
	}
      }
      
    }
    ::getArcLength( *startCurve, startCurveArcLength );

    if( arcLen<0. )
    { // use start curve arclength if there are no matching boundaries.
      arcLen=startCurveArcLength*.5;
    }
  }
  else
  {
    ::getArcLength( *startCurve, startCurveArcLength );
    arcLen=startCurveArcLength*.5;
  }
  
  if( arcLen>0. && spacingOption==spacingFromDistanceAndLines )
  {
    estimatedDistanceToMarch = arcLen*.5; // march this fraction of the length
    // choose lines to march so explicit time stepping is stable
    // ** should look for min(ds)
    real cfl=.75;
    real ds = targetGridSpacing>0. ? targetGridSpacing : cfl*startCurveArcLength/getGridDimensions(0);
    estimatedLinesToMarch = int( max(real(5),estimatedDistanceToMarch/(ds)) );

    if( targetGridSpacing>0. )
      printf(">>>>> using targetGridSpacing=%8.2e : estimatedLinesToMarch=%i\n",targetGridSpacing,estimatedLinesToMarch);
  } 
}

void HyperbolicMapping::
display( const aString & label /* =blankString */ ) const
{
  printF(" ************* Hyperbolic Mapping **************** \n");
  printF("This ia a %s.\n", surfaceGrid ? "surface grid" : "volume grid");
      
  if( growthOption==1 )
  {
    printF(" distance to march = %g (forward direction)\n",distance[0]);
    printF(" linesToMarch      = %i\n",linesToMarch[0]);
  }
  else if( growthOption==-1 )
  {
    printF(" distance to march = %g (backward direction)\n",distance[1]);
    printF(" linesToMarch      = %i\n",linesToMarch[1]);
  }
  else
  {
    printF(" distance to march: forward= %g, backward= %g \n",distance[0],distance[1]);
    printF(" linesToMarch     : forward= %i, backward= %i \n",linesToMarch[0],linesToMarch[1]);
  }
  printF("upwindDissipationCoefficient = %g \n",upwindDissipationCoefficient);
  printF("upwindDissipationCoefficient = %g \n",upwindDissipationCoefficient);
  printF("numberOfVolumeSmoothingIterations = %i \n",numberOfVolumeSmoothingIterations);
  printF("curvatureSpeedCoefficient = %g\n",curvatureSpeedCoefficient);
  printF("implicitCoefficient = %g \n",implicitCoefficient);
  printF("spacingOption = %i\n",(int)spacingOption);
  printF("spacingType   = %i\n",(int)spacingType);

  int axis,side;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    for( side=Start; side<=End; side++ )
      printF("boundaryCondition(%i,%i) = %s,   %s\n",side,axis,
	     (const char *)boundaryConditionName[max(0,min(boundaryCondition(side,axis),numberOfBoundaryConditions))],
	     (!surfaceGrid? " ": projectGhostPoints(side,axis)? "project ghost points" : "do not project ghost points"));
  }
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    for( side=Start; side<=End; side++ )
      printF("ghostBoundaryCondition(%i,%i) = %s\n",side,axis,
	     (const char *)ghostBoundaryConditionName[max(0,min(ghostBoundaryCondition(side,axis),
                           numberOfGhostBoundaryConditions))]);
  }
  for( axis=0; axis<2; axis++ )
  {
    for( side=Start; side<=End; side++ )
    {
      printF("numberOfLinesForNormalBlend[%i][%i]=%i \n",side,axis,numberOfLinesForNormalBlend[side][axis]);
    }
  }
  if( dpm!=NULL )
  {
    printF(" DataPointeMapping: order of interpolation: %i\n",dpm->getOrderOfInterpolation());
    if( usingRobustInverse() )
      printF(" Using robust inverse\n");
    else
      printF(" Not using robust inverse.\n");
  }
  

  Mapping::display();
}


int HyperbolicMapping::
deleteBoundaryCurves()
// ==================================================================================
// /Description:
//    Delete all boundary curves.
// ==================================================================================
{
  int b;
  for( b=0; b<numberOfBoundaryCurves; b++ )
  {
    if( boundaryCurves[b]->decrementReferenceCount()==0 )
      delete boundaryCurves[b];
  }
  delete [] boundaryCurves;
  boundaryCurves=NULL;
  numberOfBoundaryCurves=0;
  
  return 0;
}

int HyperbolicMapping::
deleteBoundaryCurves(IntegerArray & curvesToDelete )
// ==================================================================================
// /Description:
//    Delete a list of boundary curves.
// ==================================================================================
{
  int i;
  for( i=curvesToDelete.getBase(0); i<=curvesToDelete.getBound(0); i++ )
  {
    int b=curvesToDelete(i);
    if( b>=0 && b<numberOfBoundaryCurves && boundaryCurves[b]!=NULL )
    {
      if( boundaryCurves[b]->decrementReferenceCount()==0 )
	delete boundaryCurves[b];

      boundaryCurves[b]=NULL;
    }
    else
    {
      curvesToDelete.display("ERROR: deleteBoundaryCurves: here are the curves to delete");
    }
  }
  int b=0;
  for( i=0; i<numberOfBoundaryCurves; i++ )
  {
    if( boundaryCurves[i]!=NULL )
    {
      boundaryCurves[b]=boundaryCurves[i];
      b++;
    }
  }
  numberOfBoundaryCurves=b;

  return 0;
}


int HyperbolicMapping::
getBoundaryCurves( int & numberOfBoundaryCurves_, Mapping **&boundaryCurves_ )
// ===============================================================================================
// /Description:
//   Return the curves that can be used for starting curves.
// ===============================================================================================
{
  if( surface==NULL )
    return 0;
  
  if( numberOfBoundaryCurves==0 && surface->getClassName()=="UnstructuredMapping" )
  {
    ((UnstructuredMapping*)surface)->findBoundaryCurves(numberOfBoundaryCurves, boundaryCurves);
    printf(" hype:getBoundaryCurves %i boundary curves found from triangulation. \n",numberOfBoundaryCurves);
  }
  else if( numberOfBoundaryCurves==0 && surface->getClassName()=="CompositeSurface")
  {
    ((CompositeSurface*)surface)->findBoundaryCurves(numberOfBoundaryCurves, boundaryCurves);
    printf(" hype:getBoundaryCurves %i boundary curves found from CompositeSurface\n",numberOfBoundaryCurves);
  }
  
  numberOfBoundaryCurves_=numberOfBoundaryCurves;
  boundaryCurves_=boundaryCurves;
  return 0;
}

int HyperbolicMapping::
setBoundaryCurves( const int & numberOfBoundaryCurves_, Mapping **boundaryCurves_ )
// ===============================================================================================
// /Description:
//   Set the curves that can be used for starting curves.
// ===============================================================================================
{
  deleteBoundaryCurves();
  
  numberOfBoundaryCurves=numberOfBoundaryCurves_;
  if( numberOfBoundaryCurves>0 )
  {
    boundaryCurves = new Mapping *[numberOfBoundaryCurves];
    for( int b=0; b<numberOfBoundaryCurves; b++ )
    {
      boundaryCurves[b]= boundaryCurves_[b];
      boundaryCurves[b]->incrementReferenceCount();
    }
  }
  
  return 0;
}


int HyperbolicMapping::
addBoundaryCurves( const int & numberOfExtraBoundaryCurves, Mapping **extraBoundaryCurves )
// ===============================================================================================
// /Description:
//   Add extra boundary curves to the current set.
// ===============================================================================================
{
  if( numberOfExtraBoundaryCurves>0 )
  {
    // make sure we have the basic ones (this will no nothing if they already are there).
    getBoundaryCurves(numberOfBoundaryCurves,boundaryCurves);

    Mapping **temp = new Mapping *[numberOfBoundaryCurves+numberOfExtraBoundaryCurves];
    int b, bb=0;
    for( b=0; b<numberOfBoundaryCurves; b++ )
      temp[bb++]=boundaryCurves[b];

    for( b=0; b<numberOfExtraBoundaryCurves; b++ )
    {
      temp[bb]=extraBoundaryCurves[b];
      temp[bb]->incrementReferenceCount();
      bb++;
    }
    
    delete [] boundaryCurves;
    boundaryCurves=temp;
    numberOfBoundaryCurves+=numberOfExtraBoundaryCurves;
  }
  
  return 0;
}


void
measureQuality( Mapping & map, int numberOfGhostLines=1 )
{
  const int domainDimension=map.getDomainDimension();
  const int rangeDimension=map.getRangeDimension();
  

  // we compute the grid points including ghost points.

  Range Ig1(0,0),Ig2(0,0),Ig3(0,0);
  real dr[3]={1.,1.,1.};//

  int n1=map.getGridDimensions(0);
  Ig1=Range(-numberOfGhostLines,n1+numberOfGhostLines-1);
  dr[0]=1./max(1,n1-1);
  
  if( domainDimension>1 )
  {
    int n2=map.getGridDimensions(0);
    Ig2=Range(-numberOfGhostLines,n2+numberOfGhostLines-1);
    dr[1]=1./max(1,n2-1);
    
    if( domainDimension>2 )
    {
      int n3=map.getGridDimensions(0);
      Ig3=Range(-numberOfGhostLines,n3+numberOfGhostLines-1);
      dr[2]=1./max(1,n3-1);
    
    }
  }
  
  RealArray x(Ig1,Ig2,Ig3,rangeDimension);
  RealArray r(Ig1,Ig2,Ig3,domainDimension);
  int i1,i2,i3;
  for( i3=Ig3.getBase(); i3<=Ig3.getBound(); i3++ )
  {
    for( i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
      r(Ig1,i2,i3,0).seqAdd(dr[0]*Ig1.getBase(),dr[0]);
    if( domainDimension>1 )
    {
      for( i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
	r(i1,Ig2,i3,1).seqAdd(dr[1]*Ig2.getBase(),dr[1]);
    }
  }
  if( domainDimension>2 )
  {
    for( i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
      for( i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
	r(i1,i2,Ig3,2).seqAdd(dr[2]*Ig3.getBase(),dr[2]);
  }
  map.mapGridS(r,x);


  if( map.getDomainDimension()==3 && map.getRangeDimension()==3 )
  {
    // measure cell volumes, include ghost cells.


    for( i3=Ig3.getBase(); i3<=Ig3.getBound(); i3++ )
    {
      for( i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
      {
	for( i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
	{
	}
      }
    }
    
  }
  


}


int HyperbolicMapping::
updateForInitialCurve(bool updateNumberOfGridLines /*= true */)
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Define properties of this mapping that depend on the initial curve.
//===========================================================================
{ 
  assert( surface!=NULL );

  if( surfaceGrid )
    assert( startCurve!=NULL );
  Mapping & map = surfaceGrid ? *startCurve : *surface;
  
  int axis;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    // *** fix this for targetGridSpacing ***

    if( updateNumberOfGridLines )
    {
      if( axis==0 && surfaceGrid && targetGridSpacing>0. )
      {
	real startCurveArcLength;
	::getArcLength( *startCurve, startCurveArcLength );
	numberOfPointsOnStartCurve= int(startCurveArcLength/targetGridSpacing+.5);
	setGridDimensions(axis,numberOfPointsOnStartCurve);
      }
      else
      {
	if( surfaceGrid )
	  setGridDimensions(axis,numberOfPointsOnStartCurve);
        else
	  setGridDimensions(axis,map.getGridDimensions(axis));  // ** set initial number of points on the curve **
      }
    }
    
    if( getIsPeriodic(axis) != map.getIsPeriodic(axis) )
    { 
      // periodicity has changed:
      setIsPeriodic(axis,map.getIsPeriodic(axis));
      if( (bool)getIsPeriodic(axis) )
      {
	boundaryCondition(Range(0,1),axis)=periodic;
	for( int side=Start; side<=End; side++ )
	{
	  setBoundaryConditionAndOffset(side,axis,-1);
          // boundaryOffset[side][axis]=0;
	}
      }
      else 
      {
	// boundaryCondition(Range(0,1),axis)=freeFloating; // *wdh* 001206
	for( int side=Start; side<=End; side++ )
	{
	  if( boundaryCondition(side,axis)==0 || boundaryCondition(side,axis)==periodic )
	  {
	    boundaryCondition(side,axis)=domainDimension==3 ? outwardSplay :freeFloating;
	  }
	  setBoundaryConditionAndOffset(side,axis,0);
          // boundaryOffset[side][axis]=1;
	}
      }
    }
    if( (bool)getIsPeriodic(axis) )
    {
      boundaryCondition(Range(0,1),axis)=periodic;
      for( int side=Start; side<=End; side++ )
      {
	setBoundaryConditionAndOffset(side,axis,-1);
	// boundaryOffset[side][axis]=0;
      }
      if( surfaceGrid )
      {
        // for a surface grid both foward and backward are periodic BC's
	boundaryCondition(Range(0,1),axis2)=periodic;
// 	for( int side=Start; side<=End; side++ )
// 	  setBoundaryCondition(side,axis2,-1);
      }
    }


    for( int side=Start; side<=End; side++ )
      setTypeOfCoordinateSingularity(side,axis,map.getTypeOfCoordinateSingularity(side,axis));

  }

  if( !surfaceGrid )
    referenceSurfaceHasChanged=true;  // this is the "start curve"
  
  evaluateTheSurface=true;  // re-evaluate the initial curve for marching
  mappingHasChanged();

  printF("updateForInitialCurve:At end: numberOfPointsOnStartCurve=%i\n",numberOfPointsOnStartCurve);

  return 0;
}


void HyperbolicMapping::
setLinesAndDistanceLabels(DialogData & dialog )
// =============================================================================================
//   /Description:
//      Set the labels for lines to march and distance to march.
// =============================================================================================
{
  dialog.getOptionMenu(1).setCurrentChoice(growthOption==1 ? 0 : growthOption==-1 ? 1 : 2); 

  aString msg1=" ", msg2=" ";
  if( spacingOption==distanceFromLinesAndSpacing) 
    msg1="(=lines*initialSpacing)";
  if( spacingOption==linesFromDistanceAndSpacing )
    msg2="(=distance/initialSpacing)";
  
  aString line;
  if( abs(growthOption)==1 )
    dialog.setTextLabel("lines to march",sPrintF(line,"%i %s",linesToMarch[(1-growthOption)/2],(const char*)msg2));
  else
    dialog.setTextLabel("lines to march",sPrintF(line,"%i, %i (forward,backward) %s",linesToMarch[0],linesToMarch[1],(const char*)msg2));

  if( abs(growthOption)==1 )
    dialog.setTextLabel("distance to march",sPrintF(line,"%g %s",distance[(1-growthOption)/2],(const char*)msg1));
  else
    dialog.setTextLabel("distance to march",sPrintF(line,"%g, %g (forward,backward) %s",distance[0],distance[1],(const char*)msg1));

  // set sensitivity
  dialog.setSensitive(spacingOption!=distanceFromLinesAndSpacing,DialogData::textBoxWidget,0);
  dialog.setSensitive(spacingOption!=linesFromDistanceAndSpacing,DialogData::textBoxWidget,1);
}


void HyperbolicMapping::
updateLinesAndDistanceToMarch()
// ========================================================================================
// /Description:
//    Update lines to march or distance to march. 
// ========================================================================================
{
  printf(" ***updateLinesAndDistanceToMarch: spacingOption=%i initialSpacing=%8.2e targetGridSpacing=%8.2e\n",(int)spacingOption,initialSpacing,targetGridSpacing);
  
  if( spacingOption==linesFromDistanceAndSpacing )
  {
    real ds=initialSpacing>0. ? initialSpacing : targetGridSpacing>0. ? targetGridSpacing : -1.;
    if( ds>0. )
    {
      printf("Assign lines to march from spacing %8.2e and distance\n",ds);
      if( spacingType==constantSpacing )
      {
	linesToMarch[0]=int(distance[0]/ds+1.5);
	linesToMarch[1]=int(distance[1]/ds+1.5);
      }
      else if( spacingType==geometricSpacing )
      {
        linesToMarch[0]=int(log(distance[0]/initialSpacing)/log(geometricFactor)+1.5);
        linesToMarch[1]=int(log(distance[1]/initialSpacing)/log(geometricFactor)+1.5);
      }
      else
      {
        Overture::abort("updateLinesAndDistanceToMarch:inish this");
      }
    }
  }
  else if( spacingOption==distanceFromLinesAndSpacing )
  {
    real ds=initialSpacing>0. ? initialSpacing : targetGridSpacing>0. ? targetGridSpacing : -1.;
    if( ds>0. )
    {
      printf("Assign distance to march from spacing %8.2e and number of lines\n",ds);
      if( spacingType==constantSpacing )
      {
	distance[0]=linesToMarch[0]*ds;
	distance[1]=linesToMarch[1]*ds;
      }
      else if( spacingType==geometricSpacing )
      {
        distance[0]=initialSpacing*(pow(geometricFactor,linesToMarch[0]-1.)-1.)/(geometricFactor-1.);
        distance[1]=initialSpacing*(pow(geometricFactor,linesToMarch[1]-1.)-1.)/(geometricFactor-1.);
      }
      else
      {
        Overture::abort("updateLinesAndDistanceToMarch:finish this");
      }
    }
  }

}


int HyperbolicMapping::
update( MappingInformation & mapInfo,
	const aString & command /* = nullString */,
	DialogData *interface /* =NULL */ ) 
//=============================================================================
/// \details 
///    Prompt for changes to parameters
///    
//=============================================================================
{
  int returnValue=0;

  info =1;  // output basic info when running interactively
  
  if( info & 1 ) printf("***************update:START: initialSpacing=%e, spacingOption=%i\n",
                         initialSpacing,spacingOption);
  
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  aString prefix = "HYPER:"; // prefix for commands to make them unique.

//  DialogData *interface=NULL;
//  aString command=nullString; // *** fix ***

  const bool executeCommand = command!=nullString;
  if( false &&   // don't check prefix for now
      executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;

  int linesToStep=5;  // default lines to step

  int plotNonPhysicalBoundaries=true;
  bool plotQualityOfCells=false;

  plotReferenceSurface=true;
  //  bool plotShadedMappingBoundaries =false;
  choosePlotBoundsFromReferenceSurface=false;
  plotGridPointsOnStartCurve=true;
  
  
  plotDirectionArrowsOnInitialCurve=false;
  plotHyperbolicSurface=dpm!=NULL;
  plotBoundaryConditionMappings=true;
  plotBoundaryCurves=true;
  plotTriangulation=false;
  plotNegativeCells=false;   // set to true in generate if there are negative cells detected

  numberOfPointsOnStartCurve=21;

  aString answer,line,answer2; 

  plotObject=true;
  const bool mappingChosenOnInput = (surface!=NULL);
  bool mappingChosen = mappingChosenOnInput;

  // By default transform the last mapping in the list (if this mapping is uninitialized, mappingChosen==false)
  if( !mappingChosen )
  {
    mappingHasChanged();
    int number= mapInfo.mappingList.getLength();
    for( int i=number-1; i>=0; i-- ) // AP: used to start at number-2
    {
      Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
      if( mapPointer!=this &&
          ((mapPointer->getDomainDimension()==2 && mapPointer->getRangeDimension()==3) ||
           (mapPointer->getDomainDimension()==1 && mapPointer->getRangeDimension()==2)) )
      {
        surface=mapPointer;   // use this one
	surface->uncountedReferencesMayExist();
	surface->incrementReferenceCount();

        mappingChosen=true;
      
        if( surface->getClassName()=="CompositeSurface" || surface->getClassName()=="UnstructuredMapping" )
          surfaceGrid=true;

        setup();  // this will set initial BC's
	initializeHyperbolicGridParameters();
	deleteBoundaryCurves();
	mappingHasChanged();
	
	//printf("*** After define (1): bc=%i %i %i %i \n",getBoundaryCondition(0,0),getBoundaryCondition(1,0),
	//       getBoundaryCondition(0,1),getBoundaryCondition(1,1));

	break; 
      }
    }
  }
  if( !mappingChosen  )
  {
    if( dpm==NULL )
    {
      printF("HyperbolicMapping:ERROR: there are no mappings that can be used!! \n");
      printF("A mapping should have domainDimension==rangeDimension-1   \n");
      return 1;
    }
    else
    {
      gi.outputString("There are no Mapping's to use for building a hyperbolic mapping but the underlying");
      gi.outputString("DataPointMapping does exist.");
    }
    
  }

  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  const aString boundaryColour[]={"green","red","blue","yellow","orange","violet"}; //

  // parameters.set(GI_MAPPING_COLOUR,"cornFlowerBlue");
  // *note* use upper case names so we can compare to surface colours and boundary curve colours.
  const aString boundaryConditionMappingColour="BLUE";
  const aString buildCurveColour = "YELLOW";
  const aString edgeCurveColour = "GREEN";

  aString referenceSurfaceColour ="MEDIUMAQUAMARINE";
  aString hyperbolicMappingColour ="LIGHTSTEELBLUE"; // "MEDIUMSPRINGGREEN"; // "midnightBlue"; // "aquamarine";
  if( rangeDimension==2 )
  {
    referenceSurfaceColour = "blue";
    hyperbolicMappingColour="red";
  }
  
  initialCurveOption=initialCurveFromEdges;

  aString menu[] = 
    {
      "!HyperbolicMapping",

//      "start from which curve/surface?",
//      "generate",
//      "step n",
      ">surface grid options",
//        "surface grid",
//        "volume grid",
//        "choose the initial curve",
//        "edit initial curve",
//        "project initial curve",
//        "do not project initial curve",
//        "edit reference surface",
//        "reparameterize initial curve",
        ">stopping criteria",
          "minimum grid spacing", 
        "<",
//        "<plot reference surface (toggle)",
//        "plot shaded boundaries on reference surface (toggle)",
//        "plot boundary lines on reference surface (toggle)",
//        "change reference surface plot parameters",
//        "plot bounds derived reference surface (toggle)",
        "plot boundary condition mappings (toggle)",
      "<",
//      "<grow grid in opposite direction",
//      "grow grid in both directions (toggle)",
//      "distance to march",
//      "lines to march",
      "initial grid spacing",
      ">marching boundary conditions",
//        "boundary conditions for marching",
//        "project ghost points",
//      "<>stretching",
//        "constant spacing",
//        "geometric stretching, specified ratio",
//        "inverse hyperbolic stretching",
//        "stretching function",
//        "user defined stretching function",
      "<>parameters",
//        ">grid density",
//          "arc-length weight",
//          "curvature weight",
//          "normal curvature weight",
//        "<remove normal smoothing",
//        "hypgen values",
//        "curvature values",
//        "curvature speed coefficient",
//        "uniform dissipation coefficient",
//        "upwind dissipation coefficient",
//        "volume smoothing iterations",
//        "implicit coefficient",
//        "equidistribution weight",
        "orthogonal factor for mapping BC",
//        "number of lines for normal blending",
      "<choose a sub-interval of lines",
      "plot cell quality",
//      "save reference surface when put (toggle)",
      "save reference surface when put",
      "do not save reference surface when put",
      " ",
      "smooth",
      "smooth and project",
      "debug",
      "plot ghost lines",
      "change plot parameters",
      " ",
//      "lines",
//      "boundary conditions",
//      "share",
//      "check",
//      "check inverse",
//      "mappingName",
//      "periodicity",
      "c-grid",
      "use robust inverse",
      "do not use robust inverse",
      "use old boundary offset",
      "show parameters",
//      "plot axes (toggle)",
      "plot the back ground grid (toggle)",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "start from which curve/surface? ",
      "start from which curve/surface?",
      "generate",
      "step n    : take n more steps"
      "surface grid options",
      "  surface grid",
      "  volume grid",
      "  choose the initial curve",
      "  edit initial curve",
      "  project initial curve",
      "  do not project initial curve",
      "  reparameterize initial curve",
      "  stopping criteria",
      "  minimum grid spacing", 
      "  plot reference surface (toggle)",
      "  plot shaded boundaries on reference surface (toggle)",
      "  plot boundary lines on reference surface (toggle)",
      "  plot bounds derived reference surface (toggle)",
      "grow grid in opposite direction",
      "grow grid in both directions (toggle)",
      "distance to march",
      "lines to march",
      "boundary conditions for marching",
      "project ghost points",
      "stretching",
      "  constant spacing",
      "  geometric stretching, specified ratio",
      "  geometric stretching, specified h",
      "  inverse hyperbolic stretching",
      "  stretching function",
      "  user defined stretching function",
      "parameters",
      "  grid density",
      "  arc-length weight",
      "  curvature weight",
      "  curvature speed coefficient",
      "  uniform dissipation coefficient",
      "  upwind dissipation coefficient",
      "  volume smoothing iterations",
      "  implicit coefficient",
      "  equidistrubition weight",
      "choose a sub-interval of lines : restrict the set of marching lines to a sub-interval",
      "hypgen             : use hypgen to create a grid",
      "smooth             : smooth grid (after generation)",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
//      "plot axes (toggle)",
      "plot the back ground grid (toggle)",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString bcChoices[] = 
  {
    "free floating",
    "outward splay",  
    "fix x, float y and z",
    "fix y, float x and z",
    "fix z, float x and y",
    "float x, fix y and z",
    "float y, fix x and z",
    "float z, fix x and y",
    "float a collapsed edge",
    "periodic",
    "x symmetry plane",
    "y symmetry plane",
    "z symmetry plane",
    "singular axis point",
    "match to a mapping",
    "match to a plane",
    "trailing edge",
    "match to a boundary curve",
    ""
  };


  GUIState gui;
  gui.setWindowTitle("Hyperbolic Grid Generator");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

//   /// initialize the Mapping parameters dialog box
//   DialogData * oldInterface = mapInfo.interface; // could be null, right?
//   DialogData & mappingInterface = gui.getDialogSibling();
//   mapInfo.interface = & mappingInterface;
//   Mapping::updateWithCommand(mapInfo,"build mapping dialog");

  if( interface==NULL || command=="build dialog" )
  {
    const int num=mapInfo.mappingList.getLength();
    aString *label = new aString[num+2];

    dialog.setOptionMenuColumns(1);

    const int maxCommands= max(20,num+2);
    aString *cmd = new aString [maxCommands];
    int startingCurve=-1;
    int j=0;
    for( int i=0; i<num; i++ )
    {
      MappingRC & map = mapInfo.mappingList[i];
      if( ( (map.getDomainDimension()==2 && map.getRangeDimension()==3) ||
	    (map.getDomainDimension()==1 && map.getRangeDimension()==2) ) && map.mapPointer!=this )
      {
	label[j]=map.getName(mappingName);
        cmd[j]="Start curve:"+label[j];
        if( surface!=NULL && surface->getName(Mapping::mappingName)==label[j] )
          startingCurve=j;
        j++;
      }
    }
    label[j]=""; cmd[j]="";   // null string terminates the menu
    const int numberOfPossibleStartingCurves=j;
    if( startingCurve==-1 )
      startingCurve=numberOfPossibleStartingCurves-1;

    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("Start from:", cmd,label,startingCurve);
    delete [] label;

    aString opLabel[] = {"forward","backward","forward and backward",""}; //
    // addPrefix(label,prefix,cmd,maxCommands);
    int grow = growthOption==1 ? 0 : growthOption==-1 ? 1 : 2;
    dialog.addOptionMenu("Direction:", opLabel,opLabel,grow);

    aString opLabel2[] = {"volume grid","surface grid",""}; //
    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("Type:", opLabel2,opLabel2,surfaceGrid==true);

    aString opLabel3[] = {"choose initial curve","choose BC mapping","edit a curve","create boundary curve",
                "delete boundary curve", "hide sub-surface","choose interior matching curve",
                "query a point", "off",""}; //
    GUIState::addPrefix(opLabel3,"picking:",cmd,maxCommands);
    pickingOption=pickToChooseInitialCurve;
    dialog.addOptionMenu("Picking:", cmd,opLabel3,pickingOption);
    dialog.getOptionMenu(3).setSensitive(surfaceGrid==true);

    aString opLabel4[] = {"second order","fourth order",""}; //
    // addPrefix(label,prefix,cmd,maxCommands);
    int orderOfInterpolation = dpm==NULL ? 0 : dpm->getOrderOfInterpolation()==2 ? 0 : 1;
    dialog.addOptionMenu("Interpolation:", opLabel4,opLabel4,orderOfInterpolation);


    aString pbLabels[] = {"generate",
                          "step",
                          "marching options...",
                          "surface grid options...",
                          "boundary condition options...",
                          "plot options...",
                          "mapping parameters", 
                          "change plot parameters",
                          "edit data point mapping...",
                          "smoothing...",
                          "marching spacing...",
                          "post stretching...",
                          "start curve spacing...",
                          "print grid statistics",
			  "plot grid quality",
                          "reset grid",
			  ""};
    // addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=8;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

    dialog.setSensitive(surfaceGrid==true,DialogData::pushButtonWidget,3);
    

    aString tbCommands[] = {"evaluate as nurbs",
			    ""
                           };
    

    int tbState[5];
    tbState[0] = (int)evalAsNurbs;
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);


    const int numberOfTextStrings=8;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

    int growthDirection = growthOption==-1 ? 1 : 0;
    int nt=0;
    textLabels[nt] = "distance to march"; 
    if( abs(growthOption)==1 )
      sPrintF(textStrings[nt], "%g", distance[growthDirection]); 
    else
      sPrintF(textStrings[nt], "%g, %g",distance[0],distance[1] );
    nt++;
    
    textLabels[nt] = "lines to march"; 
    if( abs(growthOption)==1 )
      sPrintF(textStrings[nt], "%i", linesToMarch[growthDirection]);
    else
      sPrintF(textStrings[nt], "%i, %i", linesToMarch[0],linesToMarch[1] );
    nt++; 

    textLabels[nt] = "lines to step"; 
    sPrintF(textStrings[nt], "%i", linesToStep); nt++; 

    textLabels[nt] = "points on initial curve"; 
    if( domainDimension==2 )
      sPrintF(textStrings[nt], "%i", getGridDimensions(0));
    else
      sPrintF(textStrings[nt], "%i, %i", getGridDimensions(0), getGridDimensions(1));
    nt++; 

    textLabels[nt] = "target grid spacing"; 
    sPrintF(textStrings[nt], "%g, %g (tang,normal, <0 : use default)",targetGridSpacing,initialSpacing); 
    nt++; 

    textLabels[nt] = "degree of nurbs"; 
    sPrintF(textStrings[nt], "%i",nurbsDegree); 
    nt++; 

    textLabels[nt] = "name"; 
    sPrintF(textStrings[nt], "%s", (const char*)getName(mappingName)); 
    nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

    // addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

    gui.buildPopup(menu);

    delete [] cmd;
  }

  // --- Build the sibling dialog for marching parameters ---
  DialogData &marchingParametersDialog = gui.getDialogSibling();
  marchingParametersDialog.setWindowTitle("Hype Marching Options");
  marchingParametersDialog.setExitCommand("close marching options", "close");

  buildMarchingParametersDialog(marchingParametersDialog,bcChoices );
  

  // --- Build the sibling dialog for plot options parameters ---
  DialogData &plotOptionsDialog = gui.getDialogSibling();
  plotOptionsDialog.setWindowTitle("Hype Plot Options");
  plotOptionsDialog.setExitCommand("close plot options", "close");
  buildPlotOptionsDialog(plotOptionsDialog,parameters);


  // --- Build the sibling dialog for surface parameters ---
  DialogData &surfaceGridParametersDialog = gui.getDialogSibling();
  surfaceGridParametersDialog.setWindowTitle("Hype Surface Grid Options");
  surfaceGridParametersDialog.setExitCommand("close surface grid options", "close");

  buildSurfaceGridParametersDialog(surfaceGridParametersDialog);

  // --- Build the sibling dialog for setting boundary condition mappings ---
  bcOption=leftForward;
  DialogData & boundaryConditionMappingDialog = gui.getDialogSibling();
  boundaryConditionMappingDialog.setWindowTitle("Boundary Condition Mappings");
  boundaryConditionMappingDialog.setExitCommand("close boundary condition mappings options", "close");

  buildBoundaryConditionMappingDialog(boundaryConditionMappingDialog);
  
  // --- Build the sibling dialog for smooth dialog ---
  DialogData & smoothDialog = gui.getDialogSibling();
  smoothDialog.setWindowTitle("Smoothing");
  smoothDialog.setExitCommand("close smoothing options", "close");

  GridSmoother gridSmoother(domainDimension,rangeDimension);
  IntegerArray bc(2,3);
  bc = (int) GridSmoother::pointsFixed; // pointsSlide;
  gridSmoother.setBoundaryConditions( bc );
  gridSmoother.buildDialog(smoothDialog);

  // --- Build the sibling dialog for setting post stretching parameters ---
  GridStretcher gridStretcher(domainDimension,rangeDimension);
  DialogData & stretchDialog = gui.getDialogSibling();
  stretchDialog.setWindowTitle("Post Stretching of Grid Lines");
  stretchDialog.setExitCommand("close post stretching options", "close");
  gridStretcher.buildDialog(stretchDialog);

  // --- Build the sibling dialog for setting marching spacing/stretching parameters ---
  DialogData & marchingSpacingDialog = gui.getDialogSibling();
  marchingSpacingDialog.setWindowTitle("Marching Spacing and Stretching of Grid Lines");
  marchingSpacingDialog.setExitCommand("close marching spacing options", "close");
  buildMarchingSpacingDialog(marchingSpacingDialog);

  // --- Build the sibling dialog for setting marching spacing/stretching parameters ---
  DialogData & startCurveSpacingDialog = gui.getDialogSibling();
  startCurveSpacingDialog.setWindowTitle("Start Curve Grid Spacing");
  startCurveSpacingDialog.setExitCommand("close start curve spacing options", "close");
  buildStartCurveSpacingDialog(startCurveSpacingDialog);

  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("Hyperbolic>");  
  }

  if( !executeCommand  )
  {
    getBoundaryCurves(numberOfBoundaryCurves, boundaryCurves);

    // add on any potential curves in the mapping list  *wdh* 040225
    int j=0;
    const int num=mapInfo.mappingList.getLength();
    Mapping **newBoundaryCurve = new Mapping *[num];
    for( int i=0; i<num; i++ )
    {
      MappingRC & map = mapInfo.mappingList[i];
      if( map.getDomainDimension()==1 && map.getRangeDimension()==getDomainDimension() )
      {
        newBoundaryCurve[j]=&(map.getMapping());
        j++;
      }
    }
    if( j>0 )
    {
      // a new boundary curve was created
      addBoundaryCurves( j, newBoundaryCurve );
      printf("New boundary curve(s) added. Current number of boundary curves = %i\n",numberOfBoundaryCurves);
    }
    delete [] newBoundaryCurve;

  }
  


  GraphicsParameters referenceSurfaceParameters;
  referenceSurfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  referenceSurfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,true);
  referenceSurfaceParameters.set(GI_PLOT_UNS_FACES,true);

  referenceSurfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,false);
  referenceSurfaceParameters.set(GI_PLOT_UNS_EDGES,false);

  real initialOffset, offSet;
  referenceSurfaceParameters.get(GI_SURFACE_OFFSET, initialOffset);  

  IntegerArray hyperbolicGridDisplayList;
  referenceSurfaceHasChanged=true;
  RealArray xBoundForReferenceSurface(2,3);
  xBoundForReferenceSurface=0.;

  real estimatedDistanceToMarch;
  int estimatedLinesToMarch;


//  const real surfaceOffset=5.;  // =20. offset surfaces so we can see curves drawn on them
  const real surfaceOffset=initialOffset;
  
  int stretchReturnValue;
  IntegerArray gid(2,3), projectIndexRange(2,3);
  gid=0; projectIndexRange=0;
  
  // *wdh* 081211 -- add these two: 
  updateLinesAndDistanceToMarch();
  setLinesAndDistanceLabels(dialog);


  SelectionInfo select; select.nSelect=0;
//  PickInfo3D pick;  pick.active=0;
  int len;
  
  FILE *saveFile = gi.getSaveCommandFile(); // command file we are currently saving to

  // -------------------------------------------------------------------------------------------
  for(int it=0; ; it++)
  {
    if( !executeCommand )
    {
      if( it==0 && plotObject )
	answer="plotObject";
      else
      {
	plotObject=false;  // by default no need to redraw.

        gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

        if( saveFile!=NULL )
	  filePosition=ftell(saveFile);  // save the position in the file so we can over-write the last line if needed.

	gi.getAnswer(answer,"", select);
         
        gi.savePickCommands(true); // turn back on

      }
    }
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }

    // printf("answer=[%s]\n",(const char*)answer);

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

    if( dpm!=NULL )
    {
      // compute gid for the gridStretcher
      RealArray & x = xHyper;
      Range Rx=rangeDimension;
      if( domainDimension==2 )
	x.reshape(x.dimension(0),x.dimension(2),1,Rx);
      gid=gridIndexRange;
      if( domainDimension==2 )
      {
	gid(Range(0,1),axis2)=gridIndexRange(Range(0,1),axis3);
	gid(Range(0,1),axis3)=0;
      }
      projectIndexRange=gid;
      // adjust for the boundary offset
      int axis;
      for( axis=0; axis<domainDimension; axis++ )
      {
	if( ! (bool)getIsPeriodic(axis) )
	{
	  gid(Start,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(Start,axis)+boundaryOffset[Start][axis]));
	  gid(End  ,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(End  ,axis)-boundaryOffset[End  ][axis]));
	}
      } 
      if( domainDimension==2 )
	x.reshape(x.dimension(0),1,x.dimension(1),Rx);
    }
    
    bool curveWasChosen=false;
    if( (len=answer.matches("Start curve:")) )
    {
      aString name=answer(len,answer.length()-1);
      const int num=mapInfo.mappingList.getLength();
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( ( (map.getDomainDimension()==2 && map.getRangeDimension()==3) ||
	      (map.getDomainDimension()==1 && map.getRangeDimension()==2) ) && map.mapPointer!=this )
	{
	  if( name==map.getName(mappingName) )
	  {
	    if( surface!=0 && surface->decrementReferenceCount()==0 ) 
	      delete surface;
	    surface=mapInfo.mappingList[i].mapPointer;

	    surface->incrementReferenceCount();

            setup();
    	    deleteBoundaryCurves();
	    mappingHasChanged();
            referenceSurfaceHasChanged=true;
            plotObject=true;
            break;
	  }
	}
      }
      
    }
    else if( answer=="start from which curve/surface?" )
    { 
      // Make a menu with the Mapping names 
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
        if( ( (map.getDomainDimension()==2 && map.getRangeDimension()==3) ||
              (map.getDomainDimension()==1 && map.getRangeDimension()==2) ) && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      if( answer2=="none" )
        continue;
      if( mapNumber<0 )
      {
        gi.outputString("Error: unknown mapping to start from!");
        gi.stopReadingCommandFile();
      }
      else
      {
        mapNumber=subListNumbering(mapNumber);  // map number in the original list
	if( surface!=0 && surface->decrementReferenceCount()==0 ) 
	  delete surface;
	surface=mapInfo.mappingList[mapNumber].mapPointer;

        surface->incrementReferenceCount();

	mappingHasChanged(); plotObject=true; referenceSurfaceHasChanged=true;

      }
    }

    if( (!mappingChosenOnInput && it==0 && dpm==NULL ) || 
        answer=="start from which curve/surface?" || 
        answer(0,11)=="Start curve:" )
    {
      // Define properties of this mapping
      if( dpm==NULL )
      {
	setup();
	initializeHyperbolicGridParameters();
	mappingHasChanged();
      }
      // assign current values into the marchingParametersDialog
      assignMarchingParametersDialog(marchingParametersDialog,bcChoices );
      updateLinesAndDistanceToMarch();
      setLinesAndDistanceLabels(dialog);
      
      //printf("*** After define (2): bc=%i %i %i %i \n",getBoundaryCondition(0,0),getBoundaryCondition(1,0),
      //     getBoundaryCondition(0,1),getBoundaryCondition(1,1));


      plotHyperbolicSurface=false;
      plotReferenceSurface=true;
      mappingChosen=true;
      plotObject=true;
    }
    else if( answer=="print grid statistics" )
    {
      if( dpm!=NULL )
      {
        // old printGridStatistics(*dpm);
	RealArray gridStats;
	gridStatistics( *dpm,gridStats,stdout );
      }
      else
      {
	printf("print grid statistics:WARNING:There is no grid made yet\n");
      }
    }
    else if( dialog.getToggleValue( answer,"evaluate as nurbs",evalAsNurbs) )
    {
      useNurbsToEvaluate(evalAsNurbs);
    }
    else if( dialog.getTextValue(answer,"degree of nurbs","%i",nurbsDegree) )
    {
      setDegreeOfNurbs(nurbsDegree);
    }
    else if( answer=="plot grid quality" )
    {
      if( dpm!=NULL )
      {
        PlotIt::plotMappingQuality(gi,*dpm);
      }
      else
      {
	printf("plot grid quality:WARNING:There is no grid made yet.\n");
      }
      gi.erase();
      plotObject=true;

    }
    else if( updateOld(answer,mapInfo,referenceSurfaceParameters) )  // old style commands
    {
      printf("answer processed by updateOld\n");
    }
    else if( answer=="marching options..." )
    {
      marchingParametersDialog.showSibling();
    }
    else if( answer=="close marching options" )
    {
      marchingParametersDialog.hideSibling();
    }
    else if( answer=="boundary condition options..." )
    {
      // **new** do this for now -- we need a separate dialog for boundary conditions and ghost BC's
      marchingParametersDialog.showSibling();
    }
    else if( answer=="close boundary condition mappings options" )
    {
      boundaryConditionMappingDialog.hideSibling();
    }

    else if( answer=="surface grid options..." )
    {
      surfaceGridParametersDialog.showSibling();
    }
    else if( answer=="close surface grid options" )
    {
      surfaceGridParametersDialog.hideSibling();
    }

    else if( answer=="plot options..." )
    {
      plotOptionsDialog.showSibling();
    }
    else if( answer=="close plot options" )
    {
      plotOptionsDialog.hideSibling();
    }
    else if( answer=="smoothing..." )
    {
      smoothDialog.showSibling();
    }
    else if( answer=="close smoothing options" )
    {
      smoothDialog.hideSibling();
    }
    else if( gridSmoother.updateOptions( answer,smoothDialog,mapInfo ) )
    {
      printf("answer=%s was processed by the gridSmoother\n",(const char*)answer);
      
      if( answer.matches("GSM:smooth grid") ||
          answer.matches("smooth grid") )
      {
	assert( surface!=NULL );
	assert( dpm!=NULL );
      
        int projectGhost[2][3];
	for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<3; axis++ )
	  {
	    projectGhost[side][axis]=boundaryOffset[side][axis]>0;
	  }
	}

        // **** supply Mappings to the GridSmoother for projecting boundaries ****
        Mapping *boundaryMappings[2][3]={NULL,NULL,NULL,NULL,NULL,NULL}; //
        if( domainDimension==rangeDimension )
	{
	  boundaryMappings[0][domainDimension-1]=surface;  // curve/surface we start from
          for( int axis=0; axis<domainDimension-1; axis++ )
	  {
	    for( int side=0; side<=1; side++ )
	    {
	      boundaryMappings[side][axis]=boundaryConditionMapping[side][axis];  // for match to mapping BC
	    }
	  }
	}
        else 
	{
          assert( domainDimension==2 && rangeDimension==3 );
	  boundaryMappings[0][1]=startCurve;    // curve we start from

          // growthOption: 1=forward, -1=backward +-2=both
          bool growBothDirections = fabs(growthOption) > 1;
	  int direction = (growthOption==1 || growBothDirections) ? 0 : 1;
          for( int side=0; side<=1; side++ )
	  {
            // we have a problem if we are going in both directions but match to different curves
            //  -- this case is not yet supported by the GridSmoother. If we leave NULL then the
            // GridSmoother will just project onto the boundary defined by the dpm
            if( !growBothDirections || 
                (boundaryConditionMapping[0][direction]==boundaryConditionMapping[1][direction]) )
	    {
	      boundaryMappings[side][0]=boundaryConditionMapping[side][direction];
	    }
	  }
	}
	
	gridSmoother.setBoundaryMappings( boundaryMappings );
        gridSmoother.setMatchingCurves( matchingCurves );
	
	gridSmoother.smooth(*surface,*dpm,gi,parameters,projectGhost );

	setBasicInverseOption(dpm->getBasicInverseOption());
	reinitialize();  // *wdh* 000503
      
        const realArray & xdpm = dpm->getDataPoints();
        RealArray & x = xHyper;
	

        Index I1,I2,I3;
	I1=x.dimension(0);
	I2=x.dimension(1);
	I3=x.dimension(2);
        Range xAxes=rangeDimension;
        // xdpm may be smaller along I1 in the periodic case
        Index J1 =Range( max(xdpm.getBase(0),I1.getBase()),min(xdpm.getBound(0),I1.getBound()));

	if( I3.getBase()< xdpm.getBase(domainDimension-1) || I3.getBound()>xdpm.getBound(domainDimension-1) )
	{
	  printf("After smooth: dpm bounds [%i,%i][%i,%i][%i,%i], x bounds: [%i,%i][%i,%i][%i,%i]"
                 " gridIndexRange=[%i,%i][%i,%i][%i,%i] \n",
		 xdpm.getBase(0),xdpm.getBound(0),xdpm.getBase(1),xdpm.getBound(1),xdpm.getBase(2),xdpm.getBound(2),
		 x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),x.getBase(2),x.getBound(2),
                 gridIndexRange(0,0),gridIndexRange(1,0),gridIndexRange(0,1),gridIndexRange(1,1),
                 gridIndexRange(0,2),gridIndexRange(1,2));

	  printf("ERROR: after smoothing: The smoothed grid is smaller than the hyperbolic grid\n"
		 "    It could be that the grid generator ended in an error with a negative cell\n");
	  gi.stopReadingCommandFile();
	}
	else
	{
          #ifndef USE_PPP
	  if( domainDimension==2 )
	  {
	    x.reshape(I1,I3,1,x.dimension(3));
	    x(J1,I3,0,xAxes)=xdpm(J1,I3,0,xAxes);
	    x.reshape(I1,I2,I3,x.dimension(3));
	  }
	  else	  
	    x(J1,I2,I3,xAxes)=xdpm(J1,I2,I3,xAxes);
          #else
	  OV_ABORT("finish me");
          #endif
	}
	

	mappingHasChanged();
	plotObject=true; 
	plotHyperbolicSurface=true;

      }
    }
    else if( answer=="marching spacing..." )
    {
      marchingSpacingDialog.showSibling();
    }
    else if( answer=="close marching spacing options" )
    {
      marchingSpacingDialog.hideSibling();
    }
    else if( updateMarchingSpacingOptions( answer,marchingSpacingDialog,mapInfo ) )
    {
      printf("answer=%s was processed by updateSpacingOptions.\n",(const char*)answer);
      if( answer.matches("initial spacing") || answer.matches("geometric stretch factor") ||
          answer.matches("spacing:") )
      {
        updateLinesAndDistanceToMarch();
      }
      setLinesAndDistanceLabels(dialog);
    }

    else if( answer=="post stretching..." )
    {
      stretchDialog.showSibling();
    }
    else if( answer=="close post stretching options" )
    {
      stretchDialog.hideSibling();
    }
    else if( dpm!=NULL && 
             (stretchReturnValue=gridStretcher.update(answer,stretchDialog,mapInfo,xHyper,
                                                      gid,projectIndexRange,*dpm,surface)) )
    {
      // NOTE: in the call to gridStretcher.update: 
      //               gid: marks the actual boundaries
      //               gridIndexRange: marks the points to be projected on a surface grid
      printf("answer=%s was processed by gridStretcher.update, returnValue=%i\n",(const char*)answer,
             stretchReturnValue);
      if( stretchReturnValue==GridStretcher::gridWasChanged )
      {
	setBasicInverseOption(dpm->getBasicInverseOption());
	reinitialize();  // *wdh* 000503
	mappingHasChanged();
	plotObject=true; 
	plotHyperbolicSurface=true;
      }
    }
    else if( answer=="start curve spacing..." )
    {
      startCurveSpacingDialog.showSibling();
    }
    else if( answer=="close start curve spacing options" )
    {
      startCurveSpacingDialog.hideSibling();
    }
    else if( updateStartCurveSpacingOptions( answer,startCurveSpacingDialog,mapInfo ) )
    {
      printf("answer=%s was processed by updateStartCurveSpacingOptions.\n",(const char*)answer);
      // we may have changed the equidistribution weight
      marchingParametersDialog.setTextLabel("equidistribution",sPrintF(line, "%g (in [0,1])",equidistributionWeight));
      marchingParametersDialog.setTextLabel("volume smooths",sPrintF(line, "%i", numberOfVolumeSmoothingIterations));   
    }
    else if( pickingOption==pickToQueryAPoint && 
                  (select.active || select.nSelect || 
                   answer.matches("query a point")) )
    {
      // *********** Query a Point *******************
      real xSelect[3];
      if( (len=answer.matches("query a point")) )
      {
	sScanF(answer(len,answer.length()-1),"%e %e %e",&xSelect[0],&xSelect[1],&xSelect[2]);
      }
      else
      {
        xSelect[0]=select.x[0]; xSelect[1]=select.x[1]; xSelect[2]=select.x[2];
      }
      gi.outputToCommandFile(sPrintF(line,"query a point %e %e %e\n",xSelect[0],xSelect[1],xSelect[2]));

      printf("\n"
             "**********************************************************************************************\n"
             "   Selected point: x=(%e,%e,%e)\n", xSelect[0],xSelect[1],xSelect[2]);

      MappingProjectionParameters mpParams;
      RealArray x(1,3),r(1,3);
      int iv[3]={0,0,0}; //
      if( dpm!=NULL )
      {
        x(0,0)=xSelect[0]; x(0,1)=xSelect[1]; x(0,2)=xSelect[2]; 
	r=-1.; 
	dpm->inverseMapS(x,r);
	dpm->mapS(r,x);
        for( int axis=0; axis<domainDimension; axis++ )
	{
	  real dr=1./max(1,dpm->getGridDimensions(axis)-1);
          iv[axis]=int(r(0,axis)/dr+.5);  // closest point
	}
        if( domainDimension==2 )
  	  printf(" ...closest point on grid: x=(%e,%e,%e) r=(%e,%e) i=(%i,%i)\n",x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),iv[0],iv[1]);
        else
          printf(" ...closest point on grid: x=(%e,%e,%e) r=(%e,%e,%e) i=(%i,%i,%i)\n",x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),r(0,2),
                   iv[0],iv[1],iv[2]);
      }
      const bool isCompositeSurface = surface!=NULL ? surface->getClassName()=="CompositeSurface" : false;
      if( surface!=NULL )
      {
	x(0,0)=xSelect[0]; x(0,1)=xSelect[1]; x(0,2)=xSelect[2]; 
	r=-1.;
	surface->project(x,mpParams);
	printf(" ...closest point on reference surface: x=(%e,%e,%e)\n",x(0,0),x(0,1),x(0,2));
      }
	
      if( surfaceGrid && startCurve!=NULL )
      { 
	x(0,0)=xSelect[0]; x(0,1)=xSelect[1]; x(0,2)=xSelect[2]; 
	r=-1.;
	startCurve->inverseMapS(x,r);
	startCurve->mapS(r,x);
        iv[0]=int(r(0,0)*(numberOfPointsOnStartCurve-1)+.5);  // closest point
	printf(" ...closest point on start curve: x=(%e,%e,%e) r=(%e) i=(%i)\n",x(0,0),x(0,1),x(0,2),r(0,0),iv[0]);
      }
      if( surfaceGrid && surface!=NULL )
      {
	if( isCompositeSurface )
	{
	  CompositeSurface & cs = (CompositeSurface&)(*surface);
	  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    if( select.globalID == cs[s].getGlobalID() )
	    {
	      printf(" ...point is close to sub-surface %i of the reference surface\n",s);
	    }
	  }
	  CompositeTopology & compositeTopology = *cs.getCompositeTopology();      
	  int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
	  for (int i=0; i<select.nSelect; i++)
	  {
	    for( int e=0; e<numberOfEdgeCurves; e++ )
	    {
	      // printf(" edge=%i status=%i\n",e,int(compositeTopology.getEdgeCurveStatus(e)));
	      if( compositeTopology.getEdgeCurve(e).getGlobalID()==select.selection(i,0) &&
		  compositeTopology.getEdgeCurveStatus(e)!=CompositeTopology::edgeCurveIsRemoved )
	      {
		Mapping & edge = compositeTopology.getEdgeCurve(e);
		x(0,0)=xSelect[0]; x(0,1)=xSelect[1]; x(0,2)=xSelect[2]; 
		r=-1.;
		edge.inverseMapS(x,r);
		edge.mapS(r,x);
		printf(" ...point is close to edge curve %i, x=(%e,%e,%e) r=(%e)\n",e,x(0,0),x(0,1),x(0,2),r(0,0));
	      }
	    }
	  }
	}

	for (int i=0; i<select.nSelect; i++)
	{
	  for( int b=0; b<numberOfBoundaryCurves; b++ )
	  {
	    if( boundaryCurves[b]->getGlobalID()==select.selection(i,0) )
	    {
	      x(0,0)=xSelect[0]; x(0,1)=xSelect[1]; x(0,2)=xSelect[2]; 
	      r=-1.;
	      boundaryCurves[b]->inverseMapS(x,r);
	      boundaryCurves[b]->mapS(r,x);
	      printf(" ...point is close to boundary curve %i, x=(%e,%e,%e) r=(%e)\n",b,x(0,0),x(0,1),x(0,2),r(0,0));
	    }
	  } // end for b
	}        
      } // end if surfaceGrid && surface!=NULL

      printf("**********************************************************************************************\n");
    }
    else if( pickingOption==pickToChooseInitialCurve && 
                  (select.active || select.nSelect || 
                   answer.matches("choose boundary curve") || 
                   answer.matches("choose edge curve")     ||
                   answer.matches("choose point on surface")       ))
    {
      // We can choose the initial curve by picking curves or points on the surface.
      printf(" pickToChooseInitialCurve: select.active=%i, select.nSelect=%i\n",select.active,select.nSelect);
      
      if( surfaceGrid )
      {
	if( startCurve!=NULL )
	{
	  if( startCurve->decrementReferenceCount()==0 )
	  {
	    delete startCurve;
	  }
	  startCurve=NULL;
	  boundaryCondition=domainDimension==3 ? outwardSplay : freeFloating;
	  plotHyperbolicSurface=false;

	  // reset some parameters
	  startCurveStart=0.;
	  startCurveEnd=1.;
	  surfaceGridParametersDialog.setTextLabel("Start curve parameter bounds",sPrintF(line, "%g, %g",startCurveStart, startCurveEnd));
	}
  	// *wdh* 011121: by default BC is now 0
        for( int axis=0; axis<domainDimension; axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
            setBoundaryConditionAndOffset(side,axis,0);  // this will be changed if we match to a boundary curve
            // boundaryOffset[side][axis]=1;       // this means the last line computed will be the ghost line
	  }
	}
	
	buildCurve( gi,parameters,dialog, answer,select,startCurve,buildCurveColour );
        if( startCurve!=NULL )
	{
          curveWasChosen=true;  // this will cause stuff to be done below.
	  plotHyperbolicSurface=false;  // do not plot grid if the initial curve has changed.
	  mappingHasChanged();
	  plotObject=true;

	}
        //printf(" After buildCurve: bc=%i %i %i %i \n",getBoundaryCondition(0,0),getBoundaryCondition(1,0),
	// getBoundaryCondition(0,1),getBoundaryCondition(1,1));

      }
      else
      {
        gi.stopReadingCommandFile();
        gi.outputString("ERROR: cannot create an initial curve unless you are building a surface grid.");
        gi.outputString("If you want to define an initial curve for a surface grid you should first\n"
                        "select `surface grid'.");
      }

    }
    else if( pickingOption==pickToCreateBoundaryCurve && (select.active || select.nSelect 
             || answer.matches("choose boundary curve") 
             || answer.matches("choose edge curve")
             || answer.matches("choose point on surface") ) )
    {
      // We can create a boundary curve by picking curves and concatenating them
      printf(" pickingOption==pickToCreateBoundaryCurve  select.active=%i, select.nSelect=%i\n",
              select.active,select.nSelect);
      
      if( surfaceGrid )
      {
	Mapping *newBoundaryCurve=NULL;
        // choose a sequence of curves until finished:
        bool resetBoundaryConditions=false; // do not reset BC's *wdh* 020925
	buildCurve( gi,parameters,dialog, answer,select,newBoundaryCurve,buildCurveColour,resetBoundaryConditions );

        cout << "pickToCreateBoundaryCurve: after buildCurve: newBoundaryCurve=" << newBoundaryCurve << endl;
	
        if( newBoundaryCurve!=NULL )
	{
	  // a new boundary curve was created
	  addBoundaryCurves( 1, &newBoundaryCurve );
	  printf("New boundary curve added. Current number of boundary curves = %i\n",numberOfBoundaryCurves);
	}
        plotObject=true;
      }
      else
      {
        gi.stopReadingCommandFile();
        gi.outputString("ERROR: cannot create boundary curve unless you are building a surface grid.");
      }
    }
    else if( pickingOption==pickToDeleteBoundaryCurve && (select.active || select.nSelect 
             || answer.matches("delete boundary curve") ))
    {
      IntegerArray curveToDelete(1); curveToDelete=-1;
      if( (len=answer.matches("delete boundary curve")) )
      {
	sScanF(answer(len,answer.length()-1),"%i",&curveToDelete(0));
	if( curveToDelete(0)<0 || curveToDelete(0)>numberOfBoundaryCurves )
	{
	  gi.outputString(sPrintF("ERROR: invalid boundary curve to delete = %i\n",curveToDelete(0)));
	  gi.stopReadingCommandFile();
	  break;
	}
      }
      else
      {
        bool curveFound=false;
	for (int i=0; i<select.nSelect && !curveFound; i++)
	{
	  for( int b=0; b<numberOfBoundaryCurves; b++ )
	  {
	    if( boundaryCurves[b]->getGlobalID()==select.selection(i,0) )
	    {
	      printf("Boundary curve %i selected\n",b);
	      gi.outputToCommandFile(sPrintF(line,"delete boundary curve %i\n",b));
	      curveToDelete(0)=b;
              curveFound=true;
	      break;
	    }
	  } // end for b
	}        
      }
      if( curveToDelete(0)>=0 )
      {
        printf("delete curve %i\n",curveToDelete(0));
	deleteBoundaryCurves(curveToDelete);
        plotObject=true;
      }
    }
    else if( pickingOption==pickToChooseInteriorMatchingCurve && (select.active || select.nSelect 
             || answer.matches("choose boundary curve") || 
                answer.matches("choose edge curve") ) )
    {
      // Create a curve that will be used to match an interior grid line to
//        printf("Before build interior matching curve: numberOfPointsOnStartCurve=%i gridDimension=%i dpm=%i\n",
//  	     numberOfPointsOnStartCurve,getGridDimensions(0));

      if( startCurve==NULL )
      {
	printF("\n **** INFO: You should choose the initial curve before choosing interior matching curves ****\n");
	continue;
      }

      printF(" pickToChooseInteriorMatchingCurve: select.active=%i, select.nSelect=%i\n",select.active,select.nSelect);
      if( surfaceGrid )
      {

	Mapping *newMatchingCurve=NULL;
        // choose a sequence of curves until finished:
        bool resetBoundaryConditions=false;
	buildCurve( gi,parameters,dialog, answer,select,newMatchingCurve,buildCurveColour,resetBoundaryConditions );

        if( newMatchingCurve!=NULL )
	{
	  // a new matching curve was created
	  printF("******* Looking for the intersection of the start curve with the interior matching curve *****\n");
	  
          // *** intersect the matching curve with the initial curve to determine which grid point to project. ***

          const RealArray & matchingCurveGrid = newMatchingCurve->getGridSerial();
          const RealArray & startCurveGrid = startCurve->getGridSerial();

	  assert( rangeDimension==3 );
	  RealArray x(1,3),r(1,1), xp(1,3), xr(1,3,1);

          // *wdh* new way 081102
          // --  find the closest point between the start curve and matching curve : use brute force for now --
          real xm[3], xs[3]; // holds current pt on matching curve and start curve

          real rp, xi[3];
	  int matchingSide=0;
          real minDist = REAL_MAX;
	  
          int m1=0, m2=0;  // holds index's of closest points
          for( int n2=matchingCurveGrid.getBase(0); n2<=matchingCurveGrid.getBound(0); n2++ )
	  {
	    for( int axis=0; axis<rangeDimension; axis++ )
	      xm[axis]=matchingCurveGrid(n2,0,0,axis); 
	    
	    for( int n1=startCurveGrid.getBase(0); n1<=startCurveGrid.getBound(0); n1++ )
	    {
	      for( int axis=0; axis<rangeDimension; axis++ )
		xs[axis]=startCurveGrid(n1,0,0,axis); 
              real dist = SQR( xs[0]-xm[0]) + SQR( xs[1]-xm[1] ) + SQR( xs[2]-xm[2] );
	      if( dist<minDist )
	      {
                minDist=dist;
		xi[0]=xs[0]; xi[1]=xs[1]; xi[2]=xs[2];
                m2=n2; m1=n1;
	      }
	      
	    }
	  }
          // rs : startCurve(rs) = point-of-intersection
          // rm : matchingCurve(rm) = point-of-intersection
	  real rs = ( m1-startCurveGrid.getBase(0))/real(max(1,startCurveGrid.getBound(0)-startCurveGrid.getBase(0)));
          const real rm = ( m2-matchingCurveGrid.getBase(0))/
                         real(max(1,matchingCurveGrid.getBound(0)-matchingCurveGrid.getBase(0)));
          minDist=sqrt(minDist);
	  printF("match: The minimum dist. between interior matching curve and start curve grid-pts = %8.2e "
                 "(rs=%8.2e, rm=%8.2e)\n",minDist,rs,rm);
	  
	  for( int axis=0; axis<3; axis++ )
	    x(0,axis)=xi[axis];
	  
	  r=rs; // initial guess
	  startCurve->inverseMapS(x,r);

	  if( fabs(r(0,0)-.5)>.55 )
	  {
	    printf("*** match: ERROR: matching curve does not intersect the start curve!\n");
	    gi.stopReadingCommandFile();
	    continue;
	  }
	  else
	  {
	    xp=0.;
	    startCurve->mapS(r,xp);
	    rp=r(0,0);
	    xi[0]=xp(0,0); xi[1]=xp(0,1); xi[2]=xp(0,2); 

	    rs = rp;
	    printF("match: The interior matching curve intersects the start curve at x=(%8.2e,%8.2e,%8.2e) "
		   " startCurve : r=%8.2e\n",xi[0],xi[1],xi[2],rs);
	  }
	  

          // Note that the gridLine here may have to be adjusted by the boundary offset
	  int gridLine=int( rs*(numberOfPointsOnStartCurve-1)+.5 );
          if( (bool)getIsPeriodic(axis1) && gridLine==(numberOfPointsOnStartCurve-1) )
	  {
            // if periodic prefer to project the first pt instead of the last
	    gridLine=0;  
            rp=0.;  // but leave rs as it is for use below
	  }

	  const int numberOfMatchingCurves=matchingCurves.size();
          // We keep the matching curves sorted by the point of intersection (curvePosition)
          vector<MatchingCurve>::iterator mi;
          if( numberOfMatchingCurves==0 )
	  {
	    matchingCurves.resize(numberOfMatchingCurves+1);
	    mi=matchingCurves.begin();
	  }
	  else
	  {
            MatchingCurve temp;
	    for( mi=matchingCurves.begin(); mi!=matchingCurves.end(); mi++ )
	    {
	      if( rp < (*mi).curvePosition )
	      {
		mi=matchingCurves.insert(mi,temp); // insert before
                break;
	      }
	    }
            if( mi==matchingCurves.end() )
	    {
	      mi=matchingCurves.insert(mi,temp); // insert before
	    }
            // mi--;  // points to the newly inserted element
	  }
	  
	  MatchingCurve & match =*mi; // matchingCurves[imatch];
	  
          match.setCurve(*newMatchingCurve);

	  match.curvePosition=rp;
          match.x[0]=xi[0];
          match.x[1]=xi[1];
          match.x[2]=xi[2];
	  
	  
          printf("match: after insertion:\n");
	  for( int i=0; i<=numberOfMatchingCurves; i++ )
	  {
	    printF(" matchingCurves[%i] : curvePosition=%8.2e\n",i,matchingCurves[i].curvePosition);
	  }

          printF("match: matching curve intersects start curve at r=%8.2e, near grid point=%i, rp=%8.2e\n",
                 rs,gridLine,rp);

          if( startCurve->getClassName()=="SplineMapping" )
	  {
	    SplineMapping & spline = (SplineMapping&) (*startCurve);
	    int ns = spline.getNumberOfKnots();
	    // RealArray s(ns);

	    printF("match: start curve has %i points, %i knots\n",numberOfPointsOnStartCurve,ns);
	  }
	  

          // *********************************************************************
          // **** Now determine which direction the matching curve points in  ****
          // *********************************************************************

          // get tangent to startCurve at intersection point
          r=rs;
	  startCurve->mapS(r,xp,xr);

          // project the first point inward on the matching curve in order to get the normal to
          // the surface (we don't project the end point since we may be on a corner where
          // the normal is not well defined).
          real xrm[3];
          // rtol: if the start curve intersects the matching curve this close to an end 
          // of the matching curve then we only project onto the matching curve in one direction.
          const real rTol=.05;  // fix me or allow user to over-ride
          bool marchInBothDirections=false;
	  if( false )
	  {
	    int n0=matchingSide==0 ? 0 : matchingCurveGrid.getBound(0);
	    int n1=matchingSide==0 ? 1 : matchingCurveGrid.getBound(0)-1;
	    for( int axis=0; axis<3; axis++ )
	    {
	      x(0,axis)=matchingCurveGrid(n1,0,0,axis);
	      xrm[axis]=x(0,axis)-matchingCurveGrid(n0,0,0,axis);
	    }
	  }
	  else
	  {
            // *new* *wdh* 090718 
	    if( rm<rTol )
	    {
              // pt of intersection is near the start of the matching curve
	      for( int axis=0; axis<3; axis++ )
	      {
		x(0,axis)=matchingCurveGrid(m2+1,0,0,axis);
		xrm[axis]=x(0,axis)-matchingCurveGrid(m2,0,0,axis);
	      }
	    }
	    else if( rm>1.-rTol )
	    {
              // pt of intersection is near the end of the matching curve
	      for( int axis=0; axis<3; axis++ )
	      {
		x(0,axis)=matchingCurveGrid(m2-1,0,0,axis);
		xrm[axis]=x(0,axis)-matchingCurveGrid(m2,0,0,axis);
	      }
	    }
	    else
	    {
	      marchInBothDirections=true;
	    }
	  }
	  
	  if( !marchInBothDirections )
	  {
	    
	    MappingProjectionParameters mpParams;
            #ifndef USE_PPP
	    RealArray & surfaceNormal= mpParams.getRealArray(MappingProjectionParameters::normal);
            #else
  	      RealArray surfaceNormal;
              OV_ABORT("finishe me");
            #endif
	    surfaceNormal.redim(1,3);

	    surface->project(x,mpParams);
	  
	    // mv: marching vector = (start curve tangent X surface Normal) 
	    real mv[3];
	    mv[0]=xr(0,1,0)*surfaceNormal(0,2)-xr(0,2,0)*surfaceNormal(0,1);
	    mv[1]=xr(0,2,0)*surfaceNormal(0,0)-xr(0,0,0)*surfaceNormal(0,2);
	    mv[2]=xr(0,0,0)*surfaceNormal(0,1)-xr(0,1,0)*surfaceNormal(0,0);
	  
	    real dot = mv[0]*xrm[0]+mv[1]*xrm[1]+mv[2]*xrm[2];
          
	    // The interior matching curve will be used to match when marching in the
	    // forward or backward direction:
	    match.curveDirection= dot>0.? 1 : -1;

	    printF("match: startCurve xr=(%8.2e,%8.2e,%8.2e) match xrm=(%8.2e,%8.2e,%8.2e) "
		   "surf n=(%8.2e,%8.2e,%8.2e) )\n",
		   xr(0,0,0),xr(0,1,0),xr(0,2,0),xrm[0],xrm[1],xrm[2],
		   surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2));
	    printF("match: est march direction mv=(%8.2e,%8.2e,%8.2e), mv o xrm=%8.2e, ****direction=%i\n",
		   mv[0],mv[1],mv[2],dot,match.curveDirection);

	  }
	  else
	  {
            // use matching curve in both directions
            match.curveDirection=0; 
	  }
	  
	  printF("INFO: The interior matching curve will be used in %s direction(s)\n",
		 (match.curveDirection== 1 ? "the forward" : 
                  match.curveDirection==-1 ? "the backward" : "both"));



          //          *** allow this to be changed elsewhere ****
	  if( false ) // *wdh* 090718 -- we can edit matching curve parameters
	  {
	    gi.inputString(answer2,sPrintF(line,"Enter the number of lines for normal blending (default=%i)",
					   match.numberOfLinesForNormalBlend));
	    if( answer2!="" )
	    {
	      sScanF(answer2,"%i",&match.numberOfLinesForNormalBlend);
	    }
	  }
	  
	  // numberOfMatchingCurves++;
	  printF("match: New matching curve added. Current number of matching curves = %i\n",numberOfMatchingCurves+1);


	}
        plotObject=true;
      }
      else
      {
        gi.stopReadingCommandFile();
        gi.outputString("ERROR: cannot create an interior matching curve unless you are building a surface grid.");
      }
//        printf("After build interior matching curve: numberOfPointsOnStartCurve=%i gridDimension=%i dpm=%i\n",
//  	     numberOfPointsOnStartCurve,getGridDimensions(0));
      

    }
    else if( pickingOption==pickToEditAMapping && 
               (select.active || select.nSelect ||
                answer.matches("edit curve") ))
    {
      // We can edit a curve by picking it.
      // printf(" select.active=%i, select.nSelect=%i\n",select.active,select.nSelect);
      bool curveFound=false;
      Mapping *mapPointer;
      if( (len=answer.matches("edit curve")) )
      {
	int b=-1;
	sScanF(answer(len,answer.length()-1),"%i",&b);
        if( b>=0 && b<numberOfBoundaryCurves )
	{
	  mapPointer=boundaryCurves[b];
	}
	else
	{
 	  printf("ERROR: Invalid boundary curve %i\n",b);
 	  gi.stopReadingCommandFile();
 	  continue;
	}
      }
      else
      {
	for (int i=0; i<select.nSelect && !curveFound; i++)
	{
	  // printf("i=%i, ID=%i, minZ=%i, maxZ=%i\n", i,select.selection(i,0),
	  //         select.selection(i,1),select.selection(i,2));

	  for( int b=0; b<numberOfBoundaryCurves; b++ )
	  {
	    if( boundaryCurves[b]->getGlobalID()==select.selection(i,0) )
	    {
	      printf("Boundary curve %i selected\n",b);
	      mapPointer=boundaryCurves[b];
	      curveFound=true;

	      gi.outputToCommandFile(sPrintF(line,"edit curve %i\n",b));
	      break;
	    }
	  } // end for b
	}
      }
      if( mapPointer!=NULL )
      {
	mapPointer->interactiveUpdate(gi);
      }
    }
    else if( pickingOption==pickToHideSubSurface && 
              (select.active || select.nSelect ||
               answer.matches("hide surface")  ))
    {
      // Hide sub-surfaces and rebuild triangulation. 

      bool compositeSurface = surface->getClassName()=="CompositeSurface";
      if( !compositeSurface )
      {
	printf("pickToHideSubSurface: this is not a composite surface\n");
	continue;
      }

      int subSurface=-1;
      if( (len=answer.matches("hide surface")) )
      {
        sScanF(answer(len,answer.length()-1),"%i",&subSurface);
      }
      else
      {
	// printf("World coordinates: %e, %e, %e\n", select.x[0], select.x[1], select.x[2]);

	if( select.active != 1 )
	{
	  printf("pickToHideSubSurface: selected point was not on the surface\n");
	  continue;
	}
	MappingProjectionParameters mpParams;
	const IntegerDistributedArray & subSurfaceIndex = 
	  mpParams.getIntArray(MappingProjectionParameters::subSurfaceIndex);

	RealArray x(1,3);
	x(0,0)=select.x[0]; x(0,1)=select.x[1]; x(0,2)=select.x[2];

	// project the points onto the surface 
	surface->project(x,mpParams);

	subSurface=subSurfaceIndex.getLength(0)>0 ? subSurfaceIndex(0) : 0;
        gi.outputToCommandFile(sPrintF(line,"hide surface %i\n",subSurface));
      }
      
      CompositeSurface & cs = (CompositeSurface &)(*surface);
      if( subSurface>=0 && subSurface<cs.numberOfSubSurfaces() )
      {
        printf("Hiding sub-surface %i\n",subSurface);
	cs.setIsVisible(subSurface,false);

	CompositeTopology *compositeTopology = cs.getCompositeTopology();
	if( compositeTopology!=NULL )
	{
	  compositeTopology->buildTriangulationForVisibleSurfaces();  

          if( Mapping::debug & 4 )
	  {
            // plot trinagulation for debugging
	    gi.erase();
	    MappingInformation mapInfo;
	    mapInfo.graphXInterface=&gi;
	    compositeTopology->getTriangulation()->update(mapInfo);
	  }
	  

	  referenceSurfaceHasChanged=true;
	  plotObject=true;

	}
      }
      else
      {
	printf("ERROR: Invalid sub-surface number: %i\n",subSurface);
	gi.stopReadingCommandFile();
	continue;
      }
      
    }
    else if( answer=="forward" )
    {
      if( growthOption!=1 )
      {
       plotHyperbolicSurface=false;
	if( domainDimension==3 )
	  referenceSurfaceHasChanged=true;  // in 3D we need to replot the ref surface if we change directions
      }
	

      growthOption=1;
      plotDirectionArrowsOnInitialCurve=true;
      plotObject=true;

      if( surfaceGrid )
      {
        // estimate dist to march, lines, find matching boundary curves
	estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,0,gi );
	distance[0] = estimatedDistanceToMarch; // march this fraction of the length
	linesToMarch[0] = estimatedLinesToMarch;
	assignMarchingParametersDialog(marchingParametersDialog,bcChoices );

      }
      setLinesAndDistanceLabels(dialog);
      updateBoundaryConditionMappingDialog(boundaryConditionMappingDialog);
      
    }
    else if( answer=="backward" )
    {
      if( growthOption!=-1 )
      {
	plotHyperbolicSurface=false;
	if( domainDimension==3 )
	  referenceSurfaceHasChanged=true;  // in 3D we need to replot the ref surface if we change directions
      }

      growthOption=-1;
      plotDirectionArrowsOnInitialCurve=true; 
      plotObject=true;

      if( surfaceGrid )
      {
        // estimate dist to march, lines, find matching boundary curves
	estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,1,gi );
	distance[1] = estimatedDistanceToMarch; // march this fraction of the length
	linesToMarch[1] = estimatedLinesToMarch;
	assignMarchingParametersDialog(marchingParametersDialog,bcChoices );

      }
      setLinesAndDistanceLabels(dialog);
      updateBoundaryConditionMappingDialog(boundaryConditionMappingDialog);
      
    }
    else if( answer=="forward and backward" )
    {
      if( growthOption!=2 )
      {
	plotHyperbolicSurface=false;
	if( domainDimension==3 )
	  referenceSurfaceHasChanged=true;  // in 3D we need to replot the ref surface if we change directions
      }
      
      growthOption=2;
      plotDirectionArrowsOnInitialCurve=true;
      plotObject=true;

      if( surfaceGrid )
      {
        // forward
        // estimate dist to march, lines, find matching boundary curves
	estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,0,gi );
	distance[0] = estimatedDistanceToMarch; // march this fraction of the length
	linesToMarch[0] = estimatedLinesToMarch;
        // backward
        // estimate dist to march, lines, find matching boundary curves
	estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,1,gi );
	distance[1] = estimatedDistanceToMarch; // march this fraction of the length
	linesToMarch[1] = estimatedLinesToMarch;

	assignMarchingParametersDialog(marchingParametersDialog,bcChoices );
      }
      setLinesAndDistanceLabels(dialog);
      updateBoundaryConditionMappingDialog(boundaryConditionMappingDialog);
    }
    else if( answer=="reset grid" )
    {
      plotHyperbolicSurface=false;
      plotDirectionArrowsOnInitialCurve=true;
      plotObject=true;
    }
    else if( answer=="surface grid" )
    {
      if( surface!=NULL && surface->getDomainDimension()!=2 )
      {
	printf("Sorry: A surface grid can only be defined on a 3D surface\n");
	continue;
      }
      surfaceGrid=true;
      setup();

      initializeHyperbolicGridParameters();   // *** this may change the values in the dialogs
      printf("surfaceGrid: set implicitCoefficient=%e, domainD=%i, rangeD=%i\n",implicitCoefficient,
          domainDimension,rangeDimension);

      assignMarchingParametersDialog(marchingParametersDialog,bcChoices );
	  
      
      dialog.setSensitive(surfaceGrid==true,DialogData::pushButtonWidget,3);
      dialog.getOptionMenu(2).setCurrentChoice(1);  
      dialog.getOptionMenu(3).setSensitive(surfaceGrid==true);
      // surfaceGridParametersDialog.showSibling();

      int n1=getGridDimensions(0); 
      if( domainDimension==2 )
	dialog.setTextLabel("points on initial curve",sPrintF(line, "%i", getGridDimensions(0)));
      else
      {
        int n2=getGridDimensions(1);
	dialog.setTextLabel("points on initial curve",sPrintF(line, "%i, %i", getGridDimensions(0), getGridDimensions(1)));
      }

    }
    else if( answer=="volume grid" )
    {
      surfaceGrid=false;
      setup();
      assignMarchingParametersDialog(marchingParametersDialog,bcChoices );

      dialog.getOptionMenu(2).setCurrentChoice(0);  
      dialog.setSensitive(surfaceGrid==true,DialogData::pushButtonWidget,3);
      dialog.getOptionMenu(3).setSensitive(surfaceGrid==true);
    }
    else if( answer=="second order" || answer=="fourth order" )
    {
      if( dpm!=NULL )
      {
        if( answer=="second order" )
  	  dpm->setOrderOfInterpolation(2);
        else
  	  dpm->setOrderOfInterpolation(4);
      }
      else
      {
	printF("HyperbolicMapping:ERROR: cannot set order of interpolation before a DataPointMapping has been created.\n"
               " You can wait until the grid is generated before setting this parameter\n");
        gi.stopReadingCommandFile();
      }
      int choice= answer=="second order" ? 0 :1;
      dialog.getOptionMenu("Interpolation:").setCurrentChoice(choice);  
    }
    else if( answer(0,31)=="plot boundary condition mappings" )
    {
      int value;
      sScanF(answer(32,answer.length()-1),"%i",&value);
      plotBoundaryConditionMappings=value;
      plotObject=true;
    }
    else if( answer.matches("picking:choose initial curve") )
    {
      pickingOption=pickToChooseInitialCurve;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
    }
    else if( answer.matches("picking:choose BC mapping") )
    {
      pickingOption=pickToChooseBoundaryConditionMapping;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
      // open up the BC dialog
      boundaryConditionMappingDialog.showSibling();
    }
    else if( answer.matches("picking:edit a curve") )
    {
      pickingOption=pickToEditAMapping;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
    }
    else if( answer=="picking:create boundary curve" )
    {
      printf("Pick a sequence of edges to form a new boundary curve\n");
      printf("This boundary curve can be used as a boundary condition when building a surface grid\n");
      pickingOption=pickToCreateBoundaryCurve;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
    }
    else if( answer=="picking:delete boundary curve" )
    {
      printf("Pick a boundary curve to delete it\n");
      pickingOption=pickToDeleteBoundaryCurve;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
    }
    else if( answer=="picking:hide sub-surface" )
    {
      gi.outputString("Hide sub-surfaces to prevent surface grids from growing there");
      pickingOption=pickToHideSubSurface;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);

    }
    else if( answer.matches("picking:choose interior matching curve") )
    {
      pickingOption=pickToChooseInteriorMatchingCurve;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
    }
    else if( answer.matches("picking:query a point") )
    {
      pickingOption=pickToQueryAPoint;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
    }
    else if( answer.matches("picking:off") )
    {
      pickingOption=pickOff;
      dialog.getOptionMenu(3).setCurrentChoice(pickingOption);
    }
    else if( answer=="plot cell quality" )
    {
      plotQualityOfCells=true;
    }
    else if( answer=="save reference surface when put (toggle)" )
    {
      if( saveReferenceSurface )
      {
        saveReferenceSurface=0;
        printf("Reference surface and start curve will NOT be saved when the mapping is 'put' to a data base\n");
      }
      else      
      {
        saveReferenceSurface=2;
        printf("Reference surface and start curve will be saved when the mapping is 'put' to a data base\n");
      }
    }
    else if( answer=="save reference surface when put" )
    {
      saveReferenceSurface=2;
      printf("Reference surface and start curve will be saved when the mapping is 'put' to a data base\n");
    }
    else if( answer=="do not save reference surface when put" )
    {
      saveReferenceSurface=0;
      printf("Reference surface and start curve will NOT be saved when the mapping is 'put' to a data base\n");
    }
    else if( answer=="generate" )
    {
      printf("*** Before generate: bc=%i %i %i %i \n",getBoundaryCondition(0,0),getBoundaryCondition(1,0),
	     getBoundaryCondition(0,1),getBoundaryCondition(1,1));


      if( surfaceGrid && startCurve==NULL )
      {
	gi.outputString("You must choose an initial curve before growing a surface grid.");
	continue;
      }
      int returnValue = generate();
      if( returnValue!=0 || (stopOnNegativeCells && plotNegativeCells) )
      {
	gi.stopReadingCommandFile();
      }
      
      plotObject=true;
      plotHyperbolicSurface=true;
      plotDirectionArrowsOnInitialCurve=false;  

      gridSmoother.reset();  // reset grid smoother when the grid changes

      // printStatistics();
      
    }
    else if( (len=answer.matches("lines to step")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&linesToStep);
      printf("linesToStep=%i\n",linesToStep);
      dialog.setTextLabel("lines to step",sPrintF(line,"%i ",linesToStep));
    }
    else if( (len=answer.matches("step")) )
    {
      int growthDirection = growthOption==-1 ? 1 : 0;
      int numberOfAdditionalSteps=0;
      int numRead=sScanF(answer(len,answer.length()-1),"%i",&numberOfAdditionalSteps);
      if( numberOfAdditionalSteps==0 )
        numberOfAdditionalSteps=linesToStep;
      if( numRead==0 )
      {
	numberOfAdditionalSteps=1;
      }
      else if( numberOfAdditionalSteps<0 )
      {
        numberOfAdditionalSteps=max(-linesToMarch[growthDirection]+1,numberOfAdditionalSteps);
	printf("Undo %i steps...\n", numberOfAdditionalSteps);
      }
      if( numberOfAdditionalSteps>0 )
	printf("Take an additional %i steps...\n", numberOfAdditionalSteps);
      if( numberOfAdditionalSteps!=0 )
      {
	if( (dpm==NULL || !plotHyperbolicSurface ) && numberOfAdditionalSteps>0 )
	{
  	  // if no steps have been taken yet, set the linesToMarch variable instead.
	  linesToMarch[0]=linesToMarch[1]=numberOfAdditionalSteps+1;
          // distance[0]=distance[1]=1.;
	  numberOfAdditionalSteps=0;
	}
	int returnValue = generate(numberOfAdditionalSteps);
	if( returnValue!=0 || (stopOnNegativeCells && plotNegativeCells) )
	{
	  gi.stopReadingCommandFile();
	}

        plotObject=true;
	plotHyperbolicSurface=true;
      }

      printf(" Set: distance to march %s \n",(const char*)sPrintF(line,"%g ",distance[(1-growthOption)/2]));
      printf("    : distance[0]=%e distance[1]=%e\n",distance[0],distance[1]);

      setLinesAndDistanceLabels(dialog);

      gridSmoother.reset();  // reset grid smoother when the grid changes
      // printStatistics();

    }
    else if( (len=answer.matches("distance to march")) )
    {
      line=answer(len,answer.length()-1);
      if( abs(growthOption)==1 )
      {
        int growthDirection = growthOption==-1 ? 1 : 0;
        sScanF(line,"%e",&distance[growthDirection]);
        distance[1-growthDirection]=distance[growthDirection];
	printf(" distance =%e\n",distance[growthDirection]);

      }
      else
      {
        sScanF( line,"%e %e",&distance[0],&distance[1]);
	printf(" distance (forward)=%e, distance (backward)=%e\n",distance[0],distance[1]);
      }
      updateLinesAndDistanceToMarch();
      setLinesAndDistanceLabels(dialog);
    }
    else if( (len=answer.matches("lines to march")) )
    {
       line=answer(14,answer.length()-1);
      if( abs(growthOption)==1 )
      {
        int growthDirection = growthOption==-1 ? 1 : 0;
        sScanF( line,"%i",&linesToMarch[growthDirection]);
        linesToMarch[1-growthDirection]=linesToMarch[growthDirection];
      }
      else
      {
        sScanF( line,"%i %i",&linesToMarch[0],&linesToMarch[1]);
      }
      updateLinesAndDistanceToMarch();
      setLinesAndDistanceLabels(dialog);
    }
    else if( (len=answer.matches("points on initial curve")) )
    {
      int n1=getGridDimensions(0); 

      if( domainDimension==2 )
      {
	sScanF(answer(len,answer.length()-1),"%i",&n1);
        setGridDimensions(0,n1); 
	numberOfPointsOnStartCurve=n1;
        printf("***Setting numberOfPointsOnStartCurve=%i\n",numberOfPointsOnStartCurve);
	
	dialog.setTextLabel("points on initial curve",sPrintF(line, "%i", getGridDimensions(0)));
      }
      else
      {
        int n2=getGridDimensions(1);
	sScanF(answer(len,answer.length()-1),"%i %i",&n1,&n2);
        setGridDimensions(0,n1);  setGridDimensions(1,n2);
	dialog.setTextLabel("points on initial curve",sPrintF(line, "%i, %i", getGridDimensions(0), getGridDimensions(1)));
      }
      if( startCurve!=NULL )
      {
	startCurve->setGridDimensions(axis1,numberOfPointsOnStartCurve);
        bool updateNumberOfGridLines=false;
	updateForInitialCurve(updateNumberOfGridLines);
	plotHyperbolicSurface=false;
        plotDirectionArrowsOnInitialCurve=true;  // replot direction arrows
        plotObject=true;
      }
    }
    else if( (len=answer.matches("target grid spacing")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&targetGridSpacing,&initialSpacing);
      dialog.setTextLabel("target grid spacing",sPrintF(line,"%g, %g (tang,normal, <0 : use default)",
							targetGridSpacing,initialSpacing));
      
      if( initialSpacing>0. && spacingOption==spacingFromDistanceAndLines )
      {
	spacingOption=distanceFromLinesAndSpacing;
	dialog.getOptionMenu(1).setCurrentChoice((int)spacingOption);
	gi.outputString("INFO: setting spacingOption equal to `distance from lines and initial spacing'\n"
			"      You could also set the spacingOption to `lines from distance and initial spacing'");
      }
      else if( targetGridSpacing<0. && initialSpacing<0. && spacingOption==distanceFromLinesAndSpacing )
      {
	spacingOption=spacingFromDistanceAndLines;
	dialog.getOptionMenu(1).setCurrentChoice((int)spacingOption);
	gi.outputString("INFO: setting spacingOption equal to `spacing from distance and lines'\n");
      }
      
      updateLinesAndDistanceToMarch();
      setLinesAndDistanceLabels(dialog);

    }
    else if( (len=answer.matches("name ")) )
    {
      setName(mappingName,answer(len,answer.length()-1));
      dialog.setTextLabel("name",sPrintF(line, "%s", (const char*)getName(mappingName))); 
    }
    else if( (len=answer.matches("edit data point mapping...")) )
    {
      gi.outputString("The hyperbolic mapping is represented as a DataPointMapping");
      if( dpm!=NULL )
      {
        for( int axis=0; axis<domainDimension; axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
	    dpm->setBoundaryCondition(side,axis,getBoundaryCondition(side,axis));
	  }
	}
	dpm->update(mapInfo);
      }
      plotObject=true;
    }
    else if( (len=answer.matches("change plot parameters")) )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(gi,*this,parameters);
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      referenceSurfaceHasChanged=true;
      plotObject=true;
    }
    else if( (len=answer.matches("check grid quality")) )
    {
      if( dpm!=NULL )
      {
	measureQuality(*dpm);
      }
      else
      {
	gi.outputString("You must first generate the grid before you can check the quality.");
      }
    }
    else if( answer=="show parameters" )
    {
      display();
    }
    else if( answer=="debug" )
    {
      gi.inputString(line,sPrintF("Enter Mapping::debug (current= %i)",Mapping::debug));
      if( line!="" ) sScanF( line,"%i",&Mapping::debug);
      printf("Setting Mapping::debug=%i\n",Mapping::debug);
      if( (Mapping::debug !=0) && (debugFile==NULL) )
	debugFile = fopen("hype.debug","w" );

    }
    else if( answer=="plot ghost lines" )
    {
      gi.inputString(answer,sPrintF("Enter the number of ghost lines (or cells) to plot (current=%i)",
				  numberOfGhostLinesToPlot)); 
      if( answer!="" )
      {
	sScanF(answer,"%i ",&numberOfGhostLinesToPlot);
        gi.outputString(sPrintF("Plot %i ghost lines\n",numberOfGhostLinesToPlot));
        // parameters.getNumberOfGhostLinesToPlot()=numberOfGhostLinesToPlot;
        referenceSurfaceParameters.getNumberOfGhostLinesToPlot()=numberOfGhostLinesToPlot;
        plotObject=true;
	referenceSurfaceHasChanged=true;
	
      }
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      plotObject=true;
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
      continue;
    }
    else if( answer=="use robust inverse" ) // **NOTE* This must come before updateWithCommand to over-ride
    {
      useRobustInverse(true);
    }
    else if( answer=="do not use robust inverse" )// **NOTE* This must come before updateWithCommand to over-ride
    {
      useRobustInverse(false);
    }
    else if( answer=="use old boundary offset" )
    {
      printF("Using the old boundary offset option: boundary offset must be at least 1 in the marching direction\n");
      boundaryOffsetOption=0;
    }
    
    else if( updateWithCommand(mapInfo, answer) )
    {
      printf("updating boundaryOffset to match any changes in the boundary conditions..\n");
      int axis;
      for( axis=0; axis<domainDimension; axis++ )
      {
        for( int side=Start; side<=End; side++ )
	{
          int bcValue=getBoundaryCondition(side,axis);
          setBoundaryConditionAndOffset(side,axis,bcValue);
//           if( bcValue==0 )
// 	  {
// 	    boundaryOffset[side][axis]=1;
// 	  }
// 	  else if( bcValue>0 )
// 	  {
//             boundaryOffset[side][axis]=0;
// 	  }
	}
      }

      IntegerArray gid(2,3);
      gid=0;
      for( axis=0; axis<domainDimension; axis++ )
	gid(1,axis)=getGridDimensions(axis)-1;
      printf("gridIndexRange=[%i,%i]x[%i,%i]x[%i,%i], Possible multigrid levels=%i\n",gid(0,0),
	     gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),numberOfPossibleMultigridLevels(gid));

//       if( surfaceGrid && answer=="boundary conditions" )
//       {
//         for( int side=Start; side<=End; side++ )
//           boundaryCondition(side,axis2)=getBoundaryCondition(side,axis2);  // *** what is this ??
//       }
      plotObject=true;
      if( answer=="mappingName" )
        plotObject=false;

      // in case the use has changed the robust inverse option:
      if( dpm!=NULL )
        useRobustInverse(usingRobustInverse());

    }
//     else if( answer=="lines"  ||
//              answer=="boundary conditions"  ||
//              answer=="share"  ||
//              answer=="mappingName"  ||
//              answer=="periodicity" )
//     { // call the base class to change these parameters:  // ********* fix this -- assign dpm too
//       mapInfo.commandOption=MappingInformation::readOneCommand;
//       mapInfo.command=&answer;
//       Mapping::update(mapInfo); 
//       mapInfo.commandOption=MappingInformation::interactive;
//       if( surfaceGrid && answer=="boundary conditions" )
//       {
//         for( int side=Start; side<=End; side++ )
//           boundaryCondition(side,axis2)=getBoundaryCondition(side,axis2);
//       }
//       plotObject=true;
//       if( answer=="mappingName" )
//         plotObject=false;
//     }
//     else if( (len=answer.matches("lines" ))>0 )
//     {
//       int n1,n2;
//       sScanF(answer(len,answer.length()-1),"%i %i",&n1,&n2);
      
//       if( domainDimension==2 )
//       {
// 	dialog.setTextLabel(4,sPrintF(line, "%i", getGridDimensions(0)));
//       }
//       else
// 	dialog.setTextLabel(4,sPrintF(line, "%i, %i", getGridDimensions(0), getGridDimensions(1)));

//     }
    else if( answer=="c-grid" )
    {
      if( dpm!=0 )
        dpm->specifyTopology(gi,parameters);
/* ---
      for( int axis=0; axis<domainDimension; axis++ )
	for( int side=0; side<=1; side++ )
	       setTopology(side,axis,dpm->getTopology(side,axis));
--- */
    }
    else if( answer=="check" )
    {
      Mapping::debug=7;
      RealArray x(1,3),r(1,3),xr(1,3,3);
      x=0.;
      Range Rx(0,2);
      for( int i=0;;i++ )
      {
	gi.inputString(answer,"Enter a point (r0,r1) to map (null string to terminate)");
        if( answer!="" )
	{
          sScanF(answer,"%e %e %e",&r(0,0),&r(0,1),&r(0,2));
	  mapS(r,x,xr);
          printf(" x=(%6.2e,%6.2e,%6.2e), r=(%6.2e,%6.2e), xr=(%6.2e,%6.2e,%6.2e,%6.2e)\n",
		 x(0,0),x(0,1),x(0,2), r(0,0),r(0,1), xr(0,0,0),xr(0,1,0),xr(0,0,1),xr(0,1,1));
	  gi.erase();
	  PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***
          parameters.set(GI_USE_PLOT_BOUNDS,true);
          parameters.set(GI_POINT_SIZE,(real)4.);
	  gi.plotPoints(x,parameters);
          parameters.set(GI_USE_PLOT_BOUNDS,false);
	}
	else
          break;
      }
    }
    else if( answer=="check inverse" )
    {
      Mapping::debug=7;

      RealArray x(2,3),r(1,3),xx(1,3);
      x=0.;
      Range Rx(0,rangeDimension-1);
      for( int i=0;;i++ )
      {
	gi.inputString(answer,"Enter a point (x,y,z) to invert (null string to terminate)");
        if( answer!="" )
	{
          sScanF(answer,"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
          r=-1.;
          inverseMapS(x(0,Rx),r);
	  mapS(r,xx);
          x(1,Rx)=xx(0,Rx);
          printf(" x=(%6.2e,%6.2e,%6.2e), r=(%6.2e,%6.2e), projected x=(%6.2e,%6.2e,%6.2e)\n",
		 x(0,0),x(0,1),x(0,2), r(0,0),r(0,1), x(1,0),x(1,1),x(1,2));
	  gi.erase();
	  PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***
          // parameters.set(GI_USE_PLOT_BOUNDS,true);
          parameters.set(GI_POINT_SIZE,(real)4.);
	  gi.plotPoints(x,parameters);
          // parameters.set(GI_USE_PLOT_BOUNDS,false);
	}
	else
          break;
      }
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=true;
    else if( answer=="hypgen" )
    {
      // ************************************
      // *** generate the hyperbolic grid ***
      // ************************************
      hypgen(gi,parameters);
      mappingHasChanged();
    }
    else if( answer=="smooth" )
    { // smooth the grid
      smoothAndProject=false;
      smooth(gi,parameters);
    }
    else if( answer=="smooth and project" )
    { // smooth the grid
      smoothAndProject=true;
      smooth(gi,parameters);
    }
    else if( answer=="change plot parameters" )
    {
      // re-plot and allow changes to the plot parameters.
      if( dpm!=NULL )
      {
        gi.erase();
        // plot hyperbolic surface
        parameters.set(GI_TOP_LABEL,getName(mappingName));
        if( plotNonPhysicalBoundaries )
          parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,true);
	parameters.set(GI_MAPPING_COLOUR,"red");
        parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
        PlotIt::plot(gi,*this,parameters); 
        parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
        parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,false);
        // See if the option below has been turned off or on
        parameters.get(GI_PLOT_NON_PHYSICAL_BOUNDARIES,plotNonPhysicalBoundaries);
      }
      plotObject=true;
    }
    else if( updateMarchingParameters(answer,marchingParametersDialog,bcChoices,mapInfo) )
    {
      printf("answer processed by updateMarchingParameters\n");
      marchingParametersDialog.setToggleState("project ghost left",projectGhostPoints(0,0)==1);
      marchingParametersDialog.setToggleState("project ghost right",projectGhostPoints(1,0)==1);      
      plotObject=true;
      
    }
    else if( updatePlotOptions(answer,plotOptionsDialog,mapInfo,parameters,referenceSurfaceParameters) )
    {
       printf("answer processed by updatePlotOptions.\n");
    }
    else if( updateSurfaceGridParameters(answer,surfaceGridParametersDialog,mapInfo,referenceSurfaceParameters) )
    {
      printf("answer processed by updateSurfaceGridParameters\n");
      // assign current values into the marchingParametersDialog
      assignMarchingParametersDialog(marchingParametersDialog,bcChoices );

      // update points on initial curve
      if( domainDimension==2 )
	dialog.setTextLabel("points on initial curve",sPrintF( "%i", getGridDimensions(0)));
      else
	dialog.setTextLabel("points on initial curve",sPrintF( "%i, %i",getGridDimensions(0), getGridDimensions(1)));
    }
    else if( updateBoundaryConditionMappings(answer,boundaryConditionMappingDialog,
					     pickingOption==pickToChooseBoundaryConditionMapping, select, mapInfo ) )
    {
      printf("answer processed by updateBoundaryConditionMappings\n");
      // BC dialog values need to be set here
      assignMarchingParametersDialog(marchingParametersDialog,bcChoices );
    }
    else 
    {
      gi.outputString( sPrintF(line,"Unknown response=%s",(const char*)answer) );
      printf("Unknown response=%s \n",(const char*)answer);
      gi.stopReadingCommandFile();
    }


    // ******************************************************************
    // ************ A new curve was chosen above ***********************
    // ******************************************************************
    if( curveWasChosen && pickingOption==pickToChooseInitialCurve )
    {
      if( surfaceGrid )
      {
	// setup();
        printf(" curveWasChosen: start:    bc=%i %i %i %i \n",getBoundaryCondition(0,0),getBoundaryCondition(1,0),
	       getBoundaryCondition(0,1),getBoundaryCondition(1,1));        

	updateForInitialCurve();
	plotDirectionArrowsOnInitialCurve=true;

          // update BC's on dialog -- they may have changed.
	int axis=0;
	for( int side=0; side<=1; side++ )
	{
	  int menuNumber=side+2*axis;
	  marchingParametersDialog.getOptionMenu(menuNumber).setCurrentChoice(boundaryCondition(side,axis)-1);
	  // printf("Set BC dialog: bc=%i \n",boundaryCondition(side,axis));
	    
	  for( int direction=0; direction<=1; direction++ )
	  {
	    if( boundaryConditionMapping[side][direction]!=NULL && 
		boundaryConditionMapping[side][direction]->decrementReferenceCount()==0 )
	      delete boundaryConditionMapping[side][direction];
	    boundaryConditionMapping[side][direction]=NULL;
	  }
	    
	}
	marchingParametersDialog.setToggleState("project ghost left",projectGhostPoints(0,0)==1);
	marchingParametersDialog.setToggleState("project ghost right",projectGhostPoints(1,0)==1);

	dialog.getOptionMenu(1).setCurrentChoice(growthOption==1 ? 0 : growthOption==-1 ? 1 : 2); 


	if( growthOption==-1 || growthOption==1 )
	{
          int directionToMarch=(1-growthOption)/2;
          // estimate dist to march, lines, find matching boundary curves
   	  estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,directionToMarch,gi );

	  distance[directionToMarch] = estimatedDistanceToMarch; // march this fraction of the length
	  linesToMarch[directionToMarch] = estimatedLinesToMarch;
	}
	else
	{
          // forward
          // estimate dist to march, lines, find matching boundary curves
   	  estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,0,gi );
	  distance[0]= estimatedDistanceToMarch; // march this fraction of the length
	  linesToMarch[0]=estimatedLinesToMarch;
          // backward
          // estimate dist to march, lines, find matching boundary curves
   	  estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,1,gi );
	  distance[1] = estimatedDistanceToMarch; // march this fraction of the length
	  linesToMarch[1] = estimatedLinesToMarch;
	}
	updateLinesAndDistanceToMarch();
        setLinesAndDistanceLabels(dialog);
	  

	printf(" Choosing distance to march=%8.2e and lines to march = %i\n",estimatedDistanceToMarch,
	       estimatedLinesToMarch);
        //printf(" After choose start curve:    bc=%i %i %i %i \n",getBoundaryCondition(0,0),getBoundaryCondition(1,0),
	//     getBoundaryCondition(0,1),getBoundaryCondition(1,1));        

	// assign current values into the marchingParametersDialog
	assignMarchingParametersDialog(marchingParametersDialog,bcChoices );

	updateBoundaryConditionMappingDialog(boundaryConditionMappingDialog);

        dialog.setTextLabel("points on initial curve",sPrintF(line, "%i", getGridDimensions(0)));

        plotHyperbolicSurface=false;  // do not plot grid if the initial curve has changed.
        mappingHasChanged();
        plotObject=true;

	// we should check to see if the ghost points lie on the surface, otherwise we should *********************
        // turn off "project ghost points"

      }
      else if( !surfaceGrid )
      {
        printf("A curve was chosen. If you want to define an initial curve for a surface grid you should first\n"
               "select `surface grid'.\n");
      }
      
    }
    


    if( plotObject )
    {
      // printf("hypeUpdate: Plotting objects\n");
      
      if( surface!=NULL )
        gi.setAxesDimension(surface->getRangeDimension());
  
      if( referenceSurfaceHasChanged || 
          (plotHyperbolicSurface && getDomainDimension()==3) )  // do not plot ref surface in 3D when grid is made
      {
        const RealArray & xBound = gi.getGlobalBound();
	
	gi.erase();   // erase everything
        // optionally use the current bounds when drawing -- this allows the calling program to determine the
        // bounds.
        if( choosePlotBoundsFromGlobalBounds )
	{
	  gi.setGlobalBound(xBound); 
	  // choosePlotBoundsFromGlobalBounds=false;
	}
      }
      else
      {
        gi.erase(hyperbolicGridDisplayList);  // erase grid and start curve but not reference surface.
        // We need to reset the global bounds
        gi.resetGlobalBound(gi.getCurrentWindow());
	gi.setGlobalBound(xBoundForReferenceSurface);
      }
      
      if( referenceSurfaceHasChanged && plotReferenceSurface && surface!=NULL &&
	  ( !plotHyperbolicSurface || surfaceGrid || abs(growthOption)==2 || dpm==NULL ) )
      {
        // ***** plot reference surface ******
	printf("Plot reference surface\n");
	drawReferenceSurface(  gi, referenceSurfaceParameters, surfaceOffset,
			      referenceSurfaceColour, edgeCurveColour );
	 
        xBoundForReferenceSurface=gi.getGlobalBound();  // save these
      }

	// plot the edges of the referenceSurface so we can tell which boundaries are left/right/bottom/top
      drawReferenceSurfaceEdges(gi,parameters,boundaryColour);
      
      // -- begin recording the display lists so we can delete them later --
      hyperbolicGridDisplayList.redim(0);
      gi.beginRecordDisplayLists(hyperbolicGridDisplayList);

      // plot the hyperbolic grid.
      drawHyperbolicGrid( gi, parameters, plotNonPhysicalBoundaries, initialOffset, hyperbolicMappingColour );


      // draw starting curve, interior edges, boundary edges, boundary condition mappings.
      drawBoundariesAndCurves(gi, parameters, referenceSurfaceParameters, surfaceOffset, initialOffset,
			      boundaryConditionMappingColour, referenceSurfaceColour, edgeCurveColour,
                              buildCurveColour, boundaryColour );

      gi.endRecordDisplayLists(hyperbolicGridDisplayList);
      // ::display(hyperbolicGridDisplayList,"hyperbolicGridDisplayList");

	// Mark cells will negative volumes
      if( plotQualityOfCells && plotHyperbolicSurface && dpm!=NULL )
      {
        plotCellQuality(gi,parameters);
        plotQualityOfCells=false;
      }

      // if( !choosePlotBoundsFromReferenceSurface )
      //  parameters.set(GI_USE_PLOT_BOUNDS,false); 

    }
  }
  if( dpm!=NULL )
  {
    for( int axis=0; axis<domainDimension; axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	dpm->setBoundaryCondition(side,axis,getBoundaryCondition(side,axis));
      }
    }
  }
      

  gi.erase();

  referenceSurfaceParameters.set(GI_SURFACE_OFFSET, initialOffset);  

  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

  printStatistics();

  return returnValue;
}

int HyperbolicMapping::
buildCurve( GenericGraphicsInterface & gi,  
            GraphicsParameters & parameters,
            DialogData & dialog,
            aString & answer,
	    SelectionInfo & select, 
            Mapping *&newCurve, 
            const aString & buildCurveColour,
            bool resetBoundaryConditions /* = true */ )
// ======================================================================================================
// /Description:
//    Interactively build a new boundary curve by concatenating curves together as they are
//  chosen by the user.
//
// /newCurve (output) : Here is the curve that was built. 
// /resetBoundaryConditions (input) : by default we assume we are choosing an initial curve or new
//   boundary curve so we rest the boundary conditions. 
// =====================================================================================================
{
  GUIState doneDialog;

  // numberOfPointsOnStartCurve is changed by createCurveOnSurface so we save the original ** should fix this ****
  const int numberOfPointsOnStartCurveSave=numberOfPointsOnStartCurve;  // save this
  

  if( pickingOption==pickToCreateBoundaryCurve )
  {
    printf("Choose `done' menu item to finish boundary curve\n");
    doneDialog.setWindowTitle("Build a boundary curve");
  }
  else if( pickingOption==pickToChooseInitialCurve )
  {
    printf("Choose `done' menu item to finish initial curve\n");
    doneDialog.setWindowTitle("Build an initial curve");
  }
  else
    doneDialog.setWindowTitle("Build a curve");

  doneDialog.setExitCommand("done","done");

  if( initialCurveOption!=initialCurveFromCurveOnSurface )
  {
    doneDialog.addInfoLabel("Choose edges to join together.");
  }
  else
  {
    doneDialog.addInfoLabel("Choose points to form a curve.");
  }
  
  aString doneMenu[] = {"done",""};  //
  doneDialog.buildPopup(doneMenu);
  gi.pushGUI(doneDialog);

  IntegerArray curveList; // use this so we can plot and erase the new boundary curve

  parameters.set(GI_MAPPING_COLOUR,buildCurveColour);
  real oldCurveLineWidth;
  parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  parameters.set(GraphicsParameters::curveLineWidth,5.);
  int len=0;
  IntegerArray boundaryCurvesChosen(max(1,numberOfBoundaryCurves));
  boundaryCurvesChosen=-1;
  int numberOfBoundaryCurvesChosen=0;

  FILE *saveFile = gi.getSaveCommandFile();

  for( int it=0;; it++ )
  {
    gi.erase(curveList);  // erase boundary curve

    if( newCurve!=NULL )
    {
      printf("INFO: buildCurve: plot the chosen curve\n");
      newCurve->getGrid();
      curveList.redim(0);
      gi.beginRecordDisplayLists(curveList); // remember which display list numbers were used to plot the curve
      PlotIt::plot(gi,*newCurve,parameters);
      gi.endRecordDisplayLists(curveList);
    }
    if( it>0 )
    {
      if( saveFile!=NULL )
	filePosition=ftell(saveFile);  // save the position in the file so we can over-write the last line if needed.
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     
      gi.getAnswer(answer,"",select);
      gi.savePickCommands(true); 
    }
    
    if( answer=="done" )
    {
      break;
    }
    else if( select.nSelect )
    {
      // curve was selected interactively, the next routine will attempt to append the new curve
      // to the existing one, or start a new curve.
      int boundaryCurve=-1;
      createCurveOnSurface(gi,select,newCurve,NULL,NULL,NULL,&boundaryCurve,resetBoundaryConditions);
      if( boundaryCurve>=0 && boundaryCurve<numberOfBoundaryCurves )
      {
	boundaryCurvesChosen(numberOfBoundaryCurvesChosen)=boundaryCurve;
	numberOfBoundaryCurvesChosen++;
      }

    }
    else if( (len=answer.matches("choose boundary curve")) ||
             (len=answer.matches("choose edge curve")) )
    {
      // curve was chosen through a command.
      int curve=-1;
      sScanF(answer(len,answer.length()-1),"%i",&curve);

      Mapping *mapPointer;  // holds new curve.
      initialCurveIsABoundaryCurve=answer.matches("choose boundary curve"); // this is used in createCurveOnSurface
      if( initialCurveIsABoundaryCurve )
      {
	if( curve>=0 && curve<numberOfBoundaryCurves )
	{
	  mapPointer=boundaryCurves[curve];
	  boundaryCurvesChosen(numberOfBoundaryCurvesChosen)=curve;
          numberOfBoundaryCurvesChosen++;

	  if( saveFile!=NULL )
	  {
	    // here is a fudge so we can over-write the last line in the command file we are saving
	    // filePosition holds the position in the file before the last command
	    assert( filePosition!=-1 );
	    fseek(saveFile, filePosition, SEEK_SET);
	    // we also save a point near the middle as a backup
	    // (this is probably better than saving the endpoints)
	    const RealDistributedArray & xg = mapPointer->getGrid();
	    const int n=(xg.getBound(0)+xg.getBase(0))/2;
	    aString line;
	    gi.outputToCommandFile(sPrintF(line,"choose boundary curve %i %e %e %e \n",curve,
					   xg(n,0,0,0),xg(n,0,0,1),xg(n,0,0,2) ));
	      
	  }

	}
	else
	{
	  printf("ERROR: Invalid boundary curve number %i\n",curve);
	  gi.stopReadingCommandFile();
	  continue;
	}
      
      }
      else
      { // choose an edge curve
	const bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
	if( isCompositeSurface )
	{
	  CompositeSurface & cs = (CompositeSurface&)(*surface);
	  CompositeTopology & compositeTopology = *cs.getCompositeTopology();

          if( true || curve<0 )
	  {
	    // look for backup coordinates (some point near the middle of the curve)
            const real bogus=REAL_MAX;
	    real xm[3]={bogus,bogus,bogus};
	    
            sScanF(answer(len,answer.length()-1),"%i %e %e %e",&curve,
		   &xm[0],&xm[1],&xm[2]);

	    // curve=compositeTopology.getEdgeFromEndPoints(x0,x1);
            int curveFromCoords=curve;
            if( xm[2]!=bogus )
	    {
	      curveFromCoords=compositeTopology.getNearestEdge(xm);
	      printf(" *** Look for the closest edge to the point xm=(%8.2e,%8.2e,%8.2e)... found curve=%i\n",
		     xm[0],xm[1],xm[2],curveFromCoords);
	    }
	    if( curveFromCoords>=0 && curveFromCoords!=curve )
	    {
	      printf("choose edge curve:WARNING: curve in file=%i but curve from coordinates =%i\n"
                     "                           ...using curve=%i\n",curve,curveFromCoords,curveFromCoords);
	      curve=curveFromCoords;
	    }
	  }
	  
          if( curve>=0 && curve<compositeTopology.getNumberOfEdgeCurves() )
	  {
  	    mapPointer=&compositeTopology.getEdgeCurve(curve);


	    if( saveFile!=NULL )
	    {
              // here is a fudge so we can over-write the last line in the command file we are saving
              // filePosition holds the position in the file before the last command
              assert( filePosition!=-1 );
	      fseek(saveFile, filePosition, SEEK_SET);
	      // we also save a point near the middle as a backup
	      // (this is probably better than saving the endpoints)
	      const RealDistributedArray & xg = mapPointer->getGrid();
	      const int n=(xg.getBound(0)+xg.getBase(0))/2;
              aString line;
	      gi.outputToCommandFile(sPrintF(line,"choose edge curve %i %e %e %e \n",curve,
					     xg(n,0,0,0),xg(n,0,0,1),xg(n,0,0,2) ));
	      
	    }
	  }
	  else
	  {
	    printf("ERROR: Invalid edge curve number %i\n",curve);
	    gi.stopReadingCommandFile();
	    continue;
	  }
	}
	else
	{
	  printf("ERROR: attempt to choose an edge curve but this is not a CompositeSurface!\n");
	  gi.stopReadingCommandFile();
	  continue;
	}
      }
      
      // now attempt to append curve "mapPointer" to the current curve "newCurve"
      createCurveOnSurface(gi,select,newCurve,mapPointer,NULL,NULL,NULL,resetBoundaryConditions);
      
      // update points on initial curve
      if( domainDimension==2 )
	dialog.setTextLabel("points on initial curve",sPrintF( "%i", getGridDimensions(0)));
      else
	dialog.setTextLabel("points on initial curve",sPrintF( "%i, %i",getGridDimensions(0), getGridDimensions(1)));

    }
    else if( (len=answer.matches("choose point on surface")) )
    {
      int subSurface=-1;
      real xSelected[3], rSelected[2];
      sScanF(answer(len,answer.length()-1),"%i %e %e %e %e %e",&subSurface,&xSelected[0],&xSelected[1],&xSelected[2],
                &rSelected[0],&rSelected[1]);
      const bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
      Mapping *mapPointer;
      if( isCompositeSurface )
      {
	CompositeSurface & cs = *((CompositeSurface*) surface);
	if( subSurface>=0 && subSurface<cs.numberOfSubSurfaces() )
	{
	  mapPointer= &cs[subSurface];
	}
	else
	{
	  printf("ERROR: Invalid sub-surface number %i\n",subSurface);
	  gi.stopReadingCommandFile();
	  continue;
	}
      }
      else
      {
	mapPointer=surface;
      }
      initialCurveIsABoundaryCurve=false;
      // *wdh* 021101 createCurveOnSurface(gi,select,startCurve,mapPointer,xSelected,rSelected,NULL,resetBoundaryConditions);
      createCurveOnSurface(gi,select,newCurve,mapPointer,xSelected,rSelected,NULL,resetBoundaryConditions);
    }
    else
    {
      printf("Choose an edge to add to the new curve or choose `done'\n");
    }


  }
  gi.erase(curveList);  // erase new curve

	  
  if( pickingOption==pickToCreateBoundaryCurve )
  {
    boundaryCurvesChosen.resize(numberOfBoundaryCurvesChosen);
    deleteBoundaryCurves(boundaryCurvesChosen);
  }
  
  if( surfaceGrid && pickingOption==pickToChooseInitialCurve && numberOfBoundaryCurvesChosen>0 )
  {
    setBoundaryConditionAndOffset(Start,axis2,(domainDimension==3 ? (int)outwardSplay : (int)freeFloating));
    // boundaryOffset[Start][axis2]=0;

  }
  if( pickingOption!=pickToChooseInitialCurve  )
  {
    numberOfPointsOnStartCurve=numberOfPointsOnStartCurveSave; // reset this value since we did not choose an initial curve
  }
  
  gi.popGUI();
  printf("***buildCurve: at end: numberOfPointsOnStartCurve=%i\n",numberOfPointsOnStartCurve);
  
  return 0;
}



  bool HyperbolicMapping::
updateOld(aString & answer, 
	  MappingInformation & mapInfo,
	  GraphicsParameters & referenceSurfaceParameters )
// ================================================================================================================
//
// ================================================================================================================
{
  bool returnValue=true;

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  aString line,answer2;

  if( answer=="grow grid in opposite direction" )
  {
    growthOption=-growthOption;
    plotDirectionArrowsOnInitialCurve=true;
    plotObject=true;
  }
  else if( answer=="grow grid in both directions (toggle)" )
  {
    if( abs(growthOption)==2 )
      growthOption= growthOption > 0 ? 1 : -1;
    else
      growthOption= growthOption > 0 ? 2 : -2;

    // plotHyperbolicSurface=false;
    plotObject=true;
  }
  else if( answer=="distance to march" )
  {
    if( abs(growthOption)==1 )
    {
      int growthDirection = growthOption==-1 ? 1 : 0;
      gi.inputString(line,sPrintF("Enter distance to march (current=%e)",distance[growthDirection]));
      if( line!="" ) sScanF( line,"%e",&distance[growthDirection]);
      distance[1-growthDirection]=distance[growthDirection];
    }
    else
    {
      gi.inputString(line,sPrintF("Enter distance to march (forward, current=%e)",distance[0]));
      if( line!="" ) sScanF( line,"%e",&distance[0]);
      gi.inputString(line,sPrintF("Enter distance to march (backward, current=%e)",distance[1]));
      if( line!="" ) sScanF( line,"%e",&distance[1]);
    }
    if( initialSpacing>0. )
    {
      printf("Using `distace to march' to determine the initial spacing, ignoring `initial spacing' parameter\n");
      initialSpacing=-1.;
    }
  }
  else if( answer=="lines to march" )
  {
    if( abs(growthOption)==1 )
    {
      int growthDirection = growthOption==-1 ? 1 : 0;
      gi.inputString(line,sPrintF("Enter the number of lines to march (current=%i)",
				  linesToMarch[growthDirection]));
      if( line!="" ) sScanF( line,"%i",&linesToMarch[growthDirection]);
      linesToMarch[1-growthDirection]=linesToMarch[growthDirection];
    }
    else
    {
      gi.inputString(line,sPrintF("Enter the number of lines to march (forward, current=%i)",linesToMarch[0]));
      if( line!="" ) sScanF( line,"%i",&linesToMarch[0]);
      gi.inputString(line,sPrintF("Enter the number of lines to march (backward, current=%i)",linesToMarch[1]));
      if( line!="" ) sScanF( line,"%i",&linesToMarch[1]);
    }
  }
  else if( answer=="remove normal smoothing" )
  {
    removeNormalSmoothing=!removeNormalSmoothing;
    cout << "removeNormalSmoothing = " << removeNormalSmoothing << endl;
  }
  else if( answer=="arc-length weight" )
  {
    gi.inputString(line,sPrintF("Enter the arc-length weight (default=%e)",arcLengthWeight));
    if( line!="" )
      sScanF( line,"%e",&arcLengthWeight);
  }
  else if( answer=="curvature weight" )
  {
    gi.inputString(line,sPrintF("Enter the curvature weight (default=%e)",curvatureWeight));
    if( line!="" )
      sScanF( line,"%e",&curvatureWeight);
  }
  else if( answer=="normal curvature weight" )
  {
    gi.inputString(line,sPrintF("Enter the normal curvature weight (default=%e)",normalCurvatureWeight));
    if( line!="" )
      sScanF( line,"%e",&normalCurvatureWeight);
  }
  else if( answer=="constant spacing" )
  {
    spacingType=constantSpacing;
  }
  else if( answer=="geometric stretching, specified ratio" )
  {
    gi.inputString(line,sPrintF("Enter the geometric factor (default=%e)",geometricFactor));
    if( line!="" )
      sScanF( line,"%e",&geometricFactor);
    if( geometricFactor==1. )
    {
      printf("WARNING: The geometric stretch factor cannot be 1. setting to 1.00001\n");
      geometricFactor=1.00001;
    }
    spacingType=geometricSpacing;
  }
  else if( answer=="initial grid spacing" )
  {
    if( initialSpacing<=0. )
      initialSpacing=.1;
    gi.inputString(line,sPrintF("Enter the initial grid spacing (default=%e)",initialSpacing));
    if( line!="" )
    {
      sScanF( line,"%e",&initialSpacing);
      printf("Using the initial grid spacing of %g. Ignoring the `distance to march'\n",initialSpacing);
    }
  }
  else if( answer=="inverse hyperbolic stretching" )
  {
    real exponent=10.;
    gi.inputString(line,sPrintF("Enter the inverse hyperbolic exponent (default=%e)",exponent));
    if( line!="" )
      sScanF( line,"%e",&exponent);

    spacingType=oneDimensionalMappingSpacing;
    StretchMapping & stretch = *new StretchMapping;
    if( normalDistribution!=NULL && normalDistribution->decrementReferenceCount()==0 )
      delete normalDistribution;
    normalDistribution=&stretch;
    normalDistribution->incrementReferenceCount();

    stretch.setStretchingType( StretchMapping::inverseHyperbolicTangent );
    stretch.setNumberOfLayers(1);
    stretch.setLayerParameters(0,1.,exponent,0.);
    // stretch.update(mapInfo);
  }
  else if( answer=="stretching function" )
  {
    spacingType=oneDimensionalMappingSpacing;
    StretchMapping & stretch = *new StretchMapping;
    if( normalDistribution!=NULL && normalDistribution->decrementReferenceCount()==0 )
      delete normalDistribution;
    normalDistribution=&stretch;
    normalDistribution->incrementReferenceCount();
    stretch.update(mapInfo);
  }
  else if( answer=="user defined stretching function" )
  {
  }
  else if( answer=="curvature speed coefficient" )
  {
    gi.inputString(line,sPrintF("Enter the curvature speed coefficient (default= %g)",
				curvatureSpeedCoefficient));
    if( line!="" ) sScanF( line,"%e",&curvatureSpeedCoefficient);
  }
  else if( answer=="uniform dissipation coefficient" )
  {
    gi.inputString(line,sPrintF("Enter the uniform dissipation coefficient (default= %g)",
				uniformDissipationCoefficient));
    if( line!="" ) sScanF( line,"%e",&uniformDissipationCoefficient);
  }
  else if( answer=="upwind dissipation coefficient" )
  {
    gi.inputString(line,sPrintF("Enter upwindDissipationCoefficient (default= %g)",
				upwindDissipationCoefficient));
    if( line!="" ) sScanF( line,"%e",&upwindDissipationCoefficient);
  }
  else if( answer=="volume smoothing iterations" )
  {
    gi.inputString(line,sPrintF("Enter the number of volume smoothing iterations (default= %i)",
				numberOfVolumeSmoothingIterations));
    if( line!="" ) sScanF( line,"%i",&numberOfVolumeSmoothingIterations);
  }
  else if( answer=="implicit coefficient" )
  {
    gi.inputString(line,sPrintF("Enter the implicit coefficient (default= %g)",
				implicitCoefficient));
    if( line!="" ) sScanF( line,"%e",&implicitCoefficient);
  }
  else if( answer=="equidistribution weight" )
  {
    gi.inputString(line,sPrintF("Enter the equidistribution weight (default= %g)",
				equidistributionWeight));
    if( line!="" )
    {
      sScanF( line,"%e",&equidistributionWeight);
      if( arcLengthWeight==0. && curvatureWeight==0. )
      {
	arcLengthWeight=1.;
	if( surfaceGrid )
	  curvatureWeight=1.;
      }
      printf("equidistributionWeight=%g, arcLengthWeight=%g, curvatureWeight=%g\n",
             equidistributionWeight,arcLengthWeight,curvatureWeight); 
    }
  }
  else if( answer=="orthogonal factor for mapping BC" )
  {
    printf("orthogonal factor: 1.=orthogonal projection of line 1, 0.=project boundary\n");
    gi.inputString(line,sPrintF("Enter the orthogonal factor (default= %g)",
				matchToMappingOrthogonalFactor));
    if( line!="" )
    {
      sScanF( line,"%e",&matchToMappingOrthogonalFactor);
    }
  }
  else if( answer=="number of lines for normal blending" )
  {
    if( domainDimension==2 )
      gi.inputString(line,sPrintF("Enter number of lines for normal blending (default= %i,%i)",
				  numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0]));
    else
      gi.inputString(line,sPrintF("Enter number of lines for normal blending (default= %i,%i, %i,%i)",
				  numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[1][0],
				  numberOfLinesForNormalBlend[0][1],numberOfLinesForNormalBlend[1][1]));
    if( line!="" )
    {
      sScanF( line,"%i %i %i %i ",
	      &numberOfLinesForNormalBlend[0][0],
	      &numberOfLinesForNormalBlend[1][0],
	      &numberOfLinesForNormalBlend[0][1],
	      &numberOfLinesForNormalBlend[1][1]);
      for( int axis=0; axis<2; axis++ )
	for( int side=Start; side<=End; side++ )
	  numberOfLinesForNormalBlend[side][axis]=max(0,numberOfLinesForNormalBlend[side][axis]);
    }
  }
  else if( answer=="do not project initial curve" )
    projectInitialCurve=false;
  else if( answer=="minimum grid spacing" )
  {
    gi.inputString(line,sPrintF("Stop marching when the grid spacing is less than ? (current=%e)",
				minimumGridSpacing));
    if( line!="" ) sScanF( line,"%e",&minimumGridSpacing);
  }
  else if( answer=="plot reference surface (toggle)" )
  {
    plotReferenceSurface=!plotReferenceSurface;
    plotObject=true;
  }
  else if( answer=="plot shaded boundaries on reference surface (toggle)" )
  {
    int value;
    referenceSurfaceParameters.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES,value);
    referenceSurfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,!value);
    plotObject=true;
  }
  else if( answer=="plot boundary lines on reference surface (toggle)" )
  {
    int value;
    referenceSurfaceParameters.get(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,value);
    referenceSurfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,!value);
    plotObject=true;
  }
  else if( answer=="plot bounds derived from reference surface (toggle)" )
  {
    choosePlotBoundsFromReferenceSurface=!choosePlotBoundsFromReferenceSurface;
    plotObject=true;
  }
  else if( answer=="change reference surface plot parameters" )
  {
    gi.erase();
    referenceSurfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    PlotIt::plot(gi, *surface,referenceSurfaceParameters );
    referenceSurfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    plotObject=true;
  }
  else if( answer=="plot boundary condition mappings (toggle)" )
  {
    plotBoundaryConditionMappings=!plotBoundaryConditionMappings;
    plotObject=true;
  }
//   else if( answer=="choose the initial curve" )
//   { 
//     gi.appendToTheDefaultPrompt("initial curve>"); // set the default prompt
//     // Make a menu with the Mapping names 
//     int num=mapInfo.mappingList.getLength();
//     aString *menu2 = new aString[num+3];
//     IntegerArray subListNumbering(num);
//     int j=0;
//     for( int i=0; i<num; i++ )
//     {
//       MappingRC & map = mapInfo.mappingList[i];
//       if( map.getDomainDimension()==1 && map.getRangeDimension()==3 )
//       {
// 	subListNumbering(j)=i;
// 	menu2[j++]=map.getName(mappingName);
//       }
//     }
//     menu2[j++]="create a curve from the surface";
//     menu2[j++]="none"; 
//     menu2[j]="";   // null string terminates the menu
//     int mapNumber = gi.getMenuItem(menu2,answer2);
//     delete [] menu2;
//     if( answer2!="none" )
//     {
//       if( answer2=="create a curve from the surface" )
//       {
// 	assert( surface!=0 );
// 	createCurveFromASurface(gi,*surface,startCurve);
//       }
//       else
//       {
// 	if( mapNumber<0 )
// 	  gi.outputString("Error: unknown mapping to start from!");
// 	else
// 	{
// 	  mapNumber=subListNumbering(mapNumber);  // map number in the original list
// 	  if( mapInfo.mappingList[mapNumber].mapPointer==this )
// 	  {
// 	    cout << "HyperbolicMapping::ERROR: you cannot use this mapping, this would be recursive!\n";
//             throw "error";
// 	  }
// 	}
// 	startCurve=mapInfo.mappingList[mapNumber].mapPointer;
// 	startCurve->incrementReferenceCount();
//       }
//       if( startCurve!=NULL )
//       {
// 	if( !surfaceGrid )
// 	{
// 	  surfaceGrid=true;
// 	}
// 	setup();
// 	initializeHyperbolicGridParameters();

// 	plotObject=true;
// 	mappingHasChanged();
// 	plotDirectionArrowsOnInitialCurve=true;
//       }
//     }
//     gi.unAppendTheDefaultPrompt();  // reset
//   }
//   else if( answer=="boundary conditions for marching" )
//   {

//     gi.appendToTheDefaultPrompt("bc>"); 
//     aString bcMenu[] =
//     {
//       "left   (side=0,axis=0)",
//       "right  (side=1,axis=0)",
//       "bottom (side=0,axis=1)",
//       "top    (side=1,axis=1)",
//       "set all sides",
//       "exit",
//       ""
//     };
//     aString bcChoices[] = 
//     {
//       "no change",
//       "free floating",
//       "outward splay",  
//       "fix x, float y and z",
//       "fix y, float x and z",
//       "fix z, float x and y",
//       "float x, fix y and z",
//       "float y, fix x and z",
//       "float z, fix x and y",
//       "float a collapsed edge",
//       "periodic",
//       "x symmetry plane",
//       "y symmetry plane",
//       "z symmetry plane",
//       "singular axis point",
//       "match to a mapping",
//       "match to a plane",
//       "trailing edge",
//       "match to a boundary curve",
//       ""
//     };

//     for( ;; )
//     {
//       int sideChosen = gi.getMenuItem(bcMenu,answer,"choose a menu item");
//       printf("sideChosen = %i\n",sideChosen);
      
//       if( answer=="exit" )
//       {
// 	break;
//       }
//       else if( sideChosen>=0 )
//       {
// 	Range A,S;
// 	if( answer=="set all sides" )
// 	{
// 	  A=Range(0,domainDimension-1);
// 	  S=Range(0,1);
// 	}
// 	else
// 	{
// 	  int side=sideChosen %2;
// 	  int axis=sideChosen/2;
// 	  S=Range(side,side);
// 	  A=Range(axis,axis);
// 	}
// 	int itemChosen = gi.getMenuItem(bcChoices,answer2,"choose a boundary condition");
// 	if( itemChosen>0 )
// 	{
// 	  boundaryCondition(S,A)=itemChosen;
// 	}
// 	else
// 	{
// 	  gi.outputString( sPrintF("Unknown response=%s",(const char*)answer2) );
// 	  gi.stopReadingCommandFile();
// 	  break;
// 	}
// 	int side,axis;
	  
	    
// 	if( answer2=="trailing edge" )
// 	{

// 	}
// 	else if( answer2=="outward splay" )
// 	{
// 	  printf("The outward splay boundary condition causes grid lines to splay outward (or inward)\n"
// 		 "in proportion to the distance marched. The splayFactor determines the degree\n"
// 		 " of the splay. Choose a value of \n"
// 		 "     0. = no splay \n"
// 		 "     .1 = small amount or splay \n"
// 		 "     1. = a large splay (generates a nearly circular boundary) \n"
// 		 "    -.2 = negative for inward splay \n");
// 	  for( axis=A.getBase(); axis<=A.getBound(); axis++ )
// 	  {
// 	    for( side=S.getBase(); side<=S.getBound(); side++ )
// 	    {
// 	      gi.inputString(line,sPrintF("Enter the splayFactor for the %s (side=%i,axis=%i)"
// 					  " (default=%e)", 
// 					  ((side==0 && axis==0) ? "left" : 
// 					   (side==1 && axis==0) ? "right" : 
// 					   (side==0 && axis==1) ? "bottom" : "top" ),side,axis,splayFactor[axis][side]));
// 	      if( line!="" )
// 		sScanF(line,"%e ",&splayFactor[axis][side]);
// 	    }
// 	  }
// 	}
// 	else if( answer2=="match to a plane" )
// 	{
// 	  boundaryCondition(S,A)=matchToMapping; // treat this as a matchToMapping in the rest of the code.
	    
// 	  printf("A plane (or rhombus) is defined by 3 points. Choose the points in the order\n"
// 		 "  x1,y1,z1 : lower left corner       3-----X \n"
// 		 "  x2,y2,z2 : lower right corner      |     | \n"
// 		 "  x3,y3,z3 : upper left corner       1-----2 \n");
// 	  real x1=0.,y1=0.,z1=0., x2=1.,y2=0.,z2=0., x3=0.,y3=1.,z3=0.;
// 	  gi.inputString(line,sPrintF("Enter x1,y1,z1, x2,y2,z2, x3,y3,z3 (default=(%6.2e,%6.2e,%6.2e)"
// 				      " ,(%6.2e,%6.2e,%6.2e),(%6.2e,%6.2e,%6.2e) ): ",
// 				      x1,y1,z1, x2,y2,z2, x3,y3,z3));
// 	  if( line!="" )
// 	  {
// 	    sScanF(line,"%e %e %e %e %e %e %e %e %e",&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3);
// 	  }	    
            
// 	  for( axis=A.getBase(); axis<=A.getBound(); axis++ )
// 	  {
// 	    for( side=S.getBase(); side<=S.getBound(); side++ )
// 	    {
// 	      if( boundaryConditionMappingWasNewed[side][axis] )
// 		delete boundaryConditionMapping[side][axis];

// 	      boundaryConditionMapping[side][axis]= new PlaneMapping(x1,y1,z1, x2,y2,z2, x3,y3,z3 );
// 	      boundaryConditionMappingWasNewed[side][axis]=true;
// 	    }
// 	  }
// 	}
// 	else if( answer2=="match to a mapping" )
// 	{
// 	  for( axis=A.getBase(); axis<=A.getBound(); axis++ )
// 	  {
// 	    for( side=S.getBase(); side<=S.getBound(); side++ )
// 	    {
                
// 	      // Make a menu with the Mapping names (only curves or surfaces!)
// 	      int numberOfMaps=mapInfo.mappingList.getLength();
// 	      int numberOfFaces=numberOfMaps*(6+1);  // up to 6 sides per grid plus grid itself
// 	      aString *menu2 = new aString[numberOfFaces+2];
// 	      IntegerArray subListNumbering(numberOfFaces);
// 	      int i, j=0;
// 	      for( i=0; i<numberOfMaps; i++ )
// 	      {
// 		MappingRC & map = mapInfo.mappingList[i];
// 		if( ( map.getDomainDimension()== (map.getRangeDimension()-1)  ||
// 		      (map.getDomainDimension()==map.getRangeDimension() && map.getDomainDimension()>1 ) )
// 		    && map.mapPointer!=this )
// 		{
// 		  if( map.getDomainDimension()==map.getRangeDimension()-1 )
// 		  {
// 		    subListNumbering(j)=i;
// 		    menu2[j++]=map.getName(mappingName);
// 		  }
// 		  else
// 		  {
// 		    subListNumbering(j)=i;
// 		    menu2[j++]=map.getName(mappingName);  
// 		    // include all sides that are physical boundaries.
// 		    for( int dir=axis1; dir<map.getDomainDimension(); dir++ )
// 		    {
// 		      for( int side=Start; side<=End; side++ )
// 		      {
// 			if( map.getBoundaryCondition(side,dir)>0 )
// 			{
// 			  subListNumbering(j)=i;
// 			  menu2[j++]=sPrintF("%s (side=%i,axis=%i)",(const char *)map.getName(mappingName),
// 					     side,dir);
// 			}
// 		      }
// 		    }
// 		  }
// 		}
// 	      }
// 	      if( j==0 )
// 	      {
// 		gi.outputString("HyperbolicMapping::WARNING: There are no appropriate curves/surfaces to choose from");
// 		continue;
// 	      }
// 	      menu2[j++]="none"; 
// 	      menu2[j]="";   // null string terminates the menu
	
// 	      int mapNumber = gi.getMenuItem(menu2,answer2,sPrintF("Match side=%i,axis=%i to which mapping?",
// 								   side,axis));

// 	      delete [] menu2;
		
// 	      mapNumber=subListNumbering(mapNumber);  // map number in the original list
// 	      if( mapInfo.mappingList[mapNumber].mapPointer==this )
// 	      {
// 		cout << "HyperbolicMappingg::ERROR: you cannot use this mapping, this would be recursive!\n";
// 		continue;
// 	      }

// 	      Mapping & map =* mapInfo.mappingList[mapNumber].mapPointer;
// 	      if( map.getDomainDimension()==map.getRangeDimension()-1 )
// 	      {
// 		if( boundaryConditionMappingWasNewed[side][axis] )
// 		  delete boundaryConditionMapping[side][axis];
// 		boundaryConditionMapping[side][axis]=&map;
// 		boundaryConditionMappingWasNewed[side][axis]=false;
// 	      }
// 	      else if( map.getDomainDimension()==map.getRangeDimension() )
// 	      {
// 		// we may need to build a Mapping that corresponds to a side of a volume grid.
// 		int side=-1, axis=-1;
// 		int length=answer2.length();
// 		for( int j=0; j<length-6; j++ )
// 		{
// 		  if( answer2(j,j+5)=="(side=" ) 
// 		  {
// 		    sScanF(answer2(j,length-1),"(side=%i axis=%i",&side,&axis); // remember that commas are removed
// 		    if( side<0 || axis<0 )
// 		    {
// 		      cout << "Error getting (side,axis) from choice!\n";
// 		      throw "error";
// 		    }
// 		    if( boundaryConditionMappingWasNewed[side][axis] )
// 		      delete boundaryConditionMapping[side][axis];
// 		    boundaryConditionMapping[side][axis]= new ReductionMapping(map,axis,side);
// 		    boundaryConditionMappingWasNewed[side][axis]=true;
		      
// 		    // printf(" create a mapping for (side,axis)=(%i,%i) for curve[%i] \n",side,axis,i);
// 		    break;
// 		  }
// 		}
// 		if( side<0 || axis<0 )
// 		{
// 		  // printf("Setting curve[%i] \n",i);
// 		  if( boundaryConditionMappingWasNewed[side][axis] )
// 		    delete boundaryConditionMapping[side][axis];
// 		  boundaryConditionMapping[side][axis]=mapInfo.mappingList[mapNumber].mapPointer; 
// 		  boundaryConditionMappingWasNewed[side][axis]=false;
// 		}
// 	      }
// 	      else
// 	      {
// 		throw "error";
// 	      }

// 	    }
// 	  }
// 	}

// 	for( axis=A.getBase(); axis<=A.getBound(); axis++ )
// 	{
// 	  for( side=S.getBase(); side<=S.getBound(); side++ )
// 	  {
// 	    if( boundaryCondition(side,axis)==matchToMapping )
// 	    {
// 	      setBoundaryCondition(side,axis,1);
// 	      // setShare(side,axis,boundaryConditionMapping[side][axis]->??? );
// 	    }
// 	  }
// 	}

//       }
//       else
//       {
// 	gi.outputString( sPrintF("Choose BC>Unknown response=[%s]",(const char*)answer) );
// 	printf("Choose BC>Unknown response=[%s]\n",(const char*)answer);
// 	gi.stopReadingCommandFile();
// 	// break;
//       }
//     }
//     gi.unAppendTheDefaultPrompt();  // reset
//   }
  else if( answer=="project ghost points" )
  {

    gi.appendToTheDefaultPrompt("ghost>"); 
    aString bcMenu[] =
    {
      "left   (side=0,axis=0)",
      "right  (side=1,axis=0)",
      "bottom (side=0,axis=1)",
      "top    (side=1,axis=1)",
      "set all sides",
      "exit",
      ""
    };
    aString bcChoices[] = 
    {
      "no change",
      "project ghost points",
      "do not project ghost points",
      ""
    };

    for( ;; )
    {
      int sideChosen = gi.getMenuItem(bcMenu,answer,"choose a menu item");
      if( answer=="exit" )
      {
	break;
      }
      else if( sideChosen>=0 )
      {
	Range A,S;
	if( answer=="set all sides" )
	{
	  A=Range(0,domainDimension-1);
	  S=Range(0,1);
	}
	else
	{
	  int side=sideChosen %2;
	  int axis=sideChosen/2;
	  S=Range(side,side);
	  A=Range(axis,axis);
	}
	int itemChosen = gi.getMenuItem(bcChoices,answer2,"choose an option");
	if( answer2=="project ghost points" )
	  projectGhostPoints(S,A)=true;
	else if( answer2=="do not project ghost points" )
	  projectGhostPoints(S,A)=false;

	::display(projectGhostPoints,"projectGhostPoints");
      }
    }
    gi.unAppendTheDefaultPrompt();  // reset
  }
  else if( answer=="choose a sub-interval of lines" )
  {
    // Restrict the set of marching lines to a sub-interval
    if( dpm==NULL )
    {
      printf("HyperbolicMapping::WARNING:You need to generate the grid first!\n");
    }
    else
    {
      int startLine =gridIndexRange(Start,axis3);
      int endLine   =gridIndexRange(End  ,axis3);
      gi.inputString(line,sPrintF("Enter startLine,endLine (current=%i,%i)",startLine,endLine));
      if( line!="" )
      {
	sScanF( line,"%i %i",&startLine,&endLine);
	IntegerArray gid;
	gid=gridIndexRange;
	gid(Start,axis3)=startLine;
	gid(End  ,axis3)=endLine;
	Index I1,I2,I3;
	::getIndex(gid,I1,I2,I3);
	Range xAxes(0,rangeDimension-1);
    
	RealArray & x = xHyper;
        #ifndef USE_PPP
	if( domainDimension==2 )
	{
	  gid(Range(0,1),axis2)=gid(Range(0,1),axis3);
	  gid(Range(0,1),axis3)=0;
	  x.reshape(x.dimension(0),x.dimension(2),1,xAxes);

	    dpm->setDataPoints(x(I1,I3,0,xAxes),3,domainDimension,0,gid);

	  x.reshape(x.dimension(0),1,x.dimension(1),xAxes);
	  gid(Range(0,1),axis3)=gid(Range(0,1),axis2);
	  gid(Range(0,1),axis2)=0;
	}
	else
	  dpm->setDataPoints(x(I1,I2,I3,xAxes),3,domainDimension,0,gid);
        #else
          OV_ABORT("finish me");
        #endif

      }
      plotObject=true;
      mappingHasChanged();
    }
  }
  else
  {
    returnValue=false;
  }
  

  return returnValue;
}



