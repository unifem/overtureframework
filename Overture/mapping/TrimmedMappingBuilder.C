//
// Tests:
//    mbuilder trimPlane.cmd
//    mbuilder trimCylinder.cmd
//    mbuilder createTrimmed.cmd
//    mbuilder trimDonut
//    mbuilder trimWing
// 
#include "TrimmedMappingBuilder.h"

#include "TrimmedMapping.h"
#include "GUIState.h"
#include "MappingInformation.h"
#include "NurbsMapping.h"
#include "GenericGraphicsInterface.h"
#include "CompositeSurface.h"
#include "MappingRC.h"
#include "IntersectionMapping.h"
#include "ReparameterizationTransform.h"

// ==================================================================================
/// \brief Constructor for the class is used to build trimmed mappings for CAD geometries 
///        from the intersection of surfaces.
// ==================================================================================
TrimmedMappingBuilder::
TrimmedMappingBuilder()
{
  pSurface=NULL;

  // List of all possible trim curves:
  numberOfTrimCurves=0;
  trimCurve=NULL;
  trimParametricCurve=NULL;

  // Outer trim curve (NULL=use surface boundary)
  outerTrimCurve=NULL;

  // Inner trim curves
  numberOfInnerTrimCurves=0;
  innerTrimCurve=NULL;


  trimmedMapping=NULL;


  plotReferenceSurface=true;
  plotTrimCurves=true;
  plotTrimmedMappings=true;

  newSurface=false;

  plotCuttingSurface=true;
  cuttingSurface=NULL; // surface we cut with 

}
// ==================================================================================
/// \brief Destructor.
// ==================================================================================
TrimmedMappingBuilder::
~TrimmedMappingBuilder()
{
}


// ==================================================================================
// \brief Add a curve to the list of possible trim curves
/// \param curve : curve in physical space
/// \param iCurve : curve in parameter space
// ==================================================================================
int TrimmedMappingBuilder::
addCurve( NurbsMapping & curve, NurbsMapping & pCurve )
{
  if( trimCurve==NULL )
  {
    const int maximumNumberOfTrimCurves=100;  // fix me 
    trimCurve = new NurbsMapping* [maximumNumberOfTrimCurves];
    trimParametricCurve = new NurbsMapping* [maximumNumberOfTrimCurves];
  }

  trimCurve[numberOfTrimCurves] = &curve;
  trimCurve[numberOfTrimCurves]->incrementReferenceCount();
  // we also save the trim curve in the parameter space of the reference surface
  trimParametricCurve[numberOfTrimCurves] = &pCurve;
  trimParametricCurve[numberOfTrimCurves]->incrementReferenceCount();
        
  numberOfTrimCurves++;

  return 0;
}


// ==================================================================================
/// \brief Delete all curves that have been created to be potential trim curves.
// ==================================================================================
int TrimmedMappingBuilder::
deleteCurves()
{
  if( trimCurve!=NULL )
  {
    for( int i=0; i<numberOfTrimCurves; i++ )
    {
      if( trimCurve[i]->decrementReferenceCount()==0 )
	delete trimCurve[i];
      if( trimParametricCurve[i]->decrementReferenceCount()==0 )
	delete trimParametricCurve[i];
    }
    delete [] trimCurve;             trimCurve=NULL;
    delete [] trimParametricCurve;   trimParametricCurve=NULL;
    numberOfTrimCurves=0;
  }
  return 0;
}


// ==================================================================================
/// \brief Delete the curves that have been chosen to be trim curves.
// ==================================================================================
int TrimmedMappingBuilder::
resetTrimCurves()
{
  if( outerTrimCurve!=NULL &&  outerTrimCurve->decrementReferenceCount()==0 )
    delete outerTrimCurve;
  outerTrimCurve=NULL;

  if( innerTrimCurve!=NULL )
  {
    for( int i=0; i<numberOfInnerTrimCurves; i++ )
    {
      if( innerTrimCurve[i]->decrementReferenceCount()==0 )
	delete innerTrimCurve[i];
    }
    delete [] innerTrimCurve;    innerTrimCurve=NULL;
    numberOfInnerTrimCurves=0;
  }
  return 0;
}

	  
// ========================================================================================================
/// \brief Construct boundary curves on the edges of a surface
/// \param surface (input) : surface in 3D.
/// \param curve (output) : edge curves in physical space
/// \param pCurve (output) : edge curves in parameter space
//
// This is modified from a routine in TrimmedMapping.C
// ========================================================================================================
int TrimmedMappingBuilder::
constructOuterBoundaryCurves(Mapping & surface, NurbsMapping *curve, NurbsMapping *pCurve)
{
  // -- Parametric Curve --
  int m,n,p;
  p = 1; // linear segments
  n = 1; // 2 control points (0,1)
  m = n+p+1; // 4 knots (0,1,2,3)

  RealArray cPoints(n+1,3);
  RealArray knots(m+1);
  
  // set the control point weights
  cPoints(0,2) = cPoints(1,2) = 1.;
  
  cPoints(0,0) = cPoints(0,1) = 0.;
  cPoints(1,0) = 1.; cPoints(1,1) = 0.;

  knots(0) = knots(1) = 0;
  knots(2) = knots(3) = 1.;

  // first segment  (bottom)
  cPoints(0,0) = 0.; cPoints(0,1) = 0.;
  cPoints(1,0) = 1.; cPoints(1,1) = 0.;

  pCurve->specify(m,n,p,knots,cPoints,2);  // parameter space curve 

  int numPoints =101;
  Range I=numPoints;
  RealArray r(I,2), x(I,3);
  real dr=1./(numPoints-1.);
  for( int i=0; i<numPoints; i++ )
    r(i,0)=i*dr;
  r(I,1)=0.;
  surface.mapS(r,x);
  curve->interpolate(x);                   // physical space curve

  // add in subcurves
  NurbsMapping & subCurve = *new NurbsMapping; subCurve.incrementReferenceCount();
  
  // second segment  (right side)
  cPoints(0,0) = 1.; cPoints(0,1) = 0.;
  cPoints(1,0) = 1.; cPoints(1,1) = 1.;

  subCurve.specify(m,n,p,knots,cPoints,2);

  //add in the subcurve
  pCurve->merge(subCurve);

  r(I,0)=1.;
  for( int i=0; i<numPoints; i++ )
    r(i,1)=i*dr;
  surface.mapS(r,x);
  subCurve.interpolate(x);
  curve->merge(subCurve);


  // third segment (top edge from right to left)
  cPoints(0,0) = 1.; cPoints(0,1) = 1.;
  cPoints(1,0) = 0.; cPoints(1,1) = 1.;

  subCurve.specify(m,n,p,knots,cPoints,2);

  //add in the subcurve
  pCurve->merge(subCurve);

  for( int i=0; i<numPoints; i++ )
    r(i,0)=1.-i*dr;
  r(I,1)=1.;
  surface.mapS(r,x);
  subCurve.interpolate(x);
  curve->merge(subCurve);

  // fourth segment (left edge from top to bottom)
  cPoints(0,0) = 0.; cPoints(0,1) = 1.;
  cPoints(1,0) = 0.; cPoints(1,1) = 0.;

  subCurve.specify(m,n,p,knots,cPoints,2);

  //add in the subcurve
  pCurve->merge(subCurve);
  r(I,0)=0.;
  for( int i=0; i<numPoints; i++ )
    r(i,1)=1.-i*dr;
  surface.mapS(r,x);
  subCurve.interpolate(x);
  curve->merge(subCurve);

  pCurve->setGridDimensions(axis1,81);  

  if( subCurve.decrementReferenceCount()==0 )
    delete &subCurve;
  
  return 0;
}


// =============================================================================================
/// \brief  Plot the various surfaces and curves associated with building the trimmed mapping.
// =============================================================================================
int TrimmedMappingBuilder::
plotCurvesAndSurfaces( MappingInformation & mapInfo )
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  aString answer,line;
  
  //kkc   assert( pSurface!=NULL );
  Mapping *surface = pSurface;
  
  if ( surface )
    gi.setAxesDimension(surface->getRangeDimension());
  else
    gi.setAxesDimension(3);
  
  gi.erase();   // This will only hide the things in a CompositeSurface

  if( surface!=NULL && plotReferenceSurface )
  {
    referenceSurfaceParameters.set(GI_MAPPING_COLOUR,"blue");
    real oldCurveLineWidth;
    referenceSurfaceParameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,2.);

    PlotIt::plot(gi,*surface,referenceSurfaceParameters);  

    referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  }
      
  if( cuttingSurface!=NULL && plotCuttingSurface )
  {
    referenceSurfaceParameters.set(GI_MAPPING_COLOUR,"green");
    real oldCurveLineWidth;
    referenceSurfaceParameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,2.);

    PlotIt::plot(gi,*cuttingSurface,referenceSurfaceParameters);  

    referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  }
      
  if( plotTrimCurves )
  {
    real oldCurveLineWidth;
    parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    parameters.set(GraphicsParameters::curveLineWidth,3.);
    parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,false );

    int bc=0;
    for( int b=0; b<numberOfTrimCurves; b++ )
    {
      bc++;
      aString colour = gi.getColourName(bc);
      if( colour=="BLUE"  ) // skip this colour
      {
	bc++;
	colour = gi.getColourName((bc % GenericGraphicsInterface::numberOfColourNames));
      }
      if( colour=="GREEN" ) // skip this colour
      {
	bc++;
	colour = gi.getColourName((bc % GenericGraphicsInterface::numberOfColourNames));
      }
      parameters.set(GI_MAPPING_COLOUR,colour);
	
      PlotIt::plot(gi,*trimCurve[b],parameters);
    }
    parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    parameters.set(GI_MAPPING_COLOUR,"red");
    parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	
  }

  if( plotTrimmedMappings )
  {
    const int numberOfMappings= mapInfo.mappingList.getLength();
    int bc=1;
    for( int i=0; i<numberOfMappings; i++ )
    {
      Mapping & map = *mapInfo.mappingList[i].mapPointer;
      if( map.getClassName()=="TrimmedMapping" )
      {
	bc++;
        aString colour = gi.getColourName( (bc % GenericGraphicsInterface::numberOfColourNames) );
	parameters.set(GI_MAPPING_COLOUR,colour);
	PlotIt::plot(gi,map,parameters);

      }
      parameters.set(GI_MAPPING_COLOUR,"red");
    }

  }
  

  return 0;
}


// ====================================================================================
/// \brief Create or update the option menus that depend on the list of Mapping's
/// \param createOrUpdate (input) : 0=create, 1=update
// ====================================================================================
int TrimmedMappingBuilder::
setOptionMenus( MappingInformation & mapInfo, DialogData & dialog, int createOrUpdate )
{
  
  const int num=mapInfo.mappingList.getLength();
  aString *label = new aString[num+2];

  dialog.setOptionMenuColumns(1);

  const int maxCommands= max(20,num+2);
  aString *cmd = new aString [maxCommands];
  int j=0;
  int currentStartingCurve = 0;
  for( int i=0; i<num; i++ )
  {
    MappingRC & map = mapInfo.mappingList[i];
    if( (map.getDomainDimension()==2 && map.getRangeDimension()==3) )
    {
      label[j]=map.getName(Mapping::mappingName);
      cmd[j]="Surface to trim:"+label[j];
      if (&(map.getMapping()) == pSurface)
	currentStartingCurve = j;
      
      j++;
    }
  }
  if ( j==0 )
  {
    label[j] = cmd[j] = "-- none --";
    j++;
  }
  label[j]=""; cmd[j]="";   // null string terminates the menu
  const int numberOfPossibleStartingCurves=j;

  // addPrefix(label,prefix,cmd,maxCommands);
  if( createOrUpdate==0 )
    dialog.addOptionMenu("Surface to trim:", cmd,label,currentStartingCurve);
  else
    dialog.changeOptionMenu("Surface to trim:", cmd,label,currentStartingCurve);

  // -- menu for surfaces that can be used to create a trim curve
  j=0;
  for( int i=0; i<num; i++ )
  {
    MappingRC & map = mapInfo.mappingList[i];
    if( (map.getDomainDimension()==2 && map.getRangeDimension()==3) )
    {
      label[j]=map.getName(Mapping::mappingName);
      cmd[j]="Trim with:"+label[j];
      j++;
    }
  }
  label[j] = cmd[j] = "none";  j++;
  label[j]=""; cmd[j]="";   // null string terminates the menu

  // addPrefix(label,prefix,cmd,maxCommands);
  if( createOrUpdate==0 )
    dialog.addOptionMenu("Trim with:", cmd,label,j-1);
  else
    dialog.changeOptionMenu("Trim with:", cmd,label,j-1);

  delete [] cmd;
  delete [] label;

  return 0;
}

// =============================================================================================
/// \brief Build a trimmed mapping by intersecting mapping's.
/// 
/// \param mapInfo (input/output) : on input holds lists of Mapping's that we can use. On output holds the
///       additional trimmed mappings that were constructed.
/// \param surface (input/output) : if provided, this is a pointer to the Mapping to trim.
///
///  NOTE: this could be a static function in MappingBuilder
// =============================================================================================
int TrimmedMappingBuilder::
buildTrimmedMapping( MappingInformation & mapInfo, Mapping *surface /* = NULL */ )
{


  int returnValue=0;
  
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;


  aString answer,line,answer2; 

  bool plotObject=true;

  bool mappingChosen = surface!=NULL;


  if( true )
  {
    printF(" ------------------------------ Build a Trimmed Mapping --------------------------------------\n"
           "\n"
           " Trimmed mappings are used to build CAD geometries (with the CompositeSurface).\n"
           " A trimmed mapping consists of an untrimmed reference surface together with\n"
           " a collection of trimming curves. There is an outer trimming curve which defines the outer\n"
           " boundary of the trimmed surface and one or more inner trimming curves that cut holes in\n"
           " the surface. Each trim curve should be a closed curve.\n"
           "\n"
           " The steps to build a trimmed mapping are\n"
           "    1. Choose a reference surface to be trimmed.\n"
           "    2. Construct curves that can be used to trim the surface.\n"
           "    3. Choose curves that will belong to the outer and inner trim curves.\n"
           "    4. Create the trimmed mapping and edit the trim curves to ensure that they form closed curves.\n"
           "\n"
           " Trimming curves can be defined here by\n"
           "    1. Intersecting another surface with the reference surface.\n"
           "    2. From the outer edges of the reference surface.\n"
           "    3. Defining a curve from a Mapping such as a Nurbs\n"
           "\n"
           " NOTES:\n"
           "    1. When building trimmed mappings for CAD surfaces you should avoid having a periodic\n"
           "       surface such a cylinder or sphere since the topology routine that builds a global \n"
           "       triangulation cannot handle these. To get around this you can split a periodic surface into\n"
           "       two parts. For example, a cylinder can be split in two halves, one half with 0<= theta =<pi\n"
           "       and a second half with pi<= theta <2 pi.\n"
           " --------------------------------------------------------------------------------------------------\n");
  }
  
    
      // --- Create an inner or outer trim curve ---
      //
      //  A trim curve may consist of multiple sub-curves. At this stage we do not enforce that
      //  the sub-curves form a contiguous closed curve. This is done later when the trimmed mapping is formed.    



  // By default trim the last surface mapping in the list (if this mapping is uninitialized, mappingChosen==FALSE)
  if( !mappingChosen )
  {
    int number= mapInfo.mappingList.getLength();
    for( int i=number-1; i>=0; i-- )
    {
      Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
      if( mapPointer->getDomainDimension()==2 && mapPointer->getRangeDimension()==3 )
      {
        surface=mapPointer;   // use this one
	surface->uncountedReferencesMayExist();
	surface->incrementReferenceCount();

        mappingChosen=true;
        newSurface=true;
      
	break; 
      }
    }
  }
  if( !mappingChosen )
  {
    printF("buildTrimmedMapping:ERROR: there are no mappings that can be used!! \n");
    printF("A mapping should be a 3d surface : domainDimension=2 and rangeDimension=3  \n");
    return 1;
  }

  pSurface=surface;

  GUIState gui;
  gui.setWindowTitle("Build a Trimmed Mapping");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = gui;


  //  dialog.addInfoLabel("Total points on surface grids: 0.");

  // build the option menus that depend on the list of mappings
  setOptionMenus(mapInfo,dialog,0);


  enum PickOptionsEnum
  {
    pickToBuildTheOuterTrimCurve=0,
    pickToBuildAnInnerTrimCurve,
    pickToSplitPeriodicSurface,
    pickToDoNothing
  }  pickOption=pickToBuildTheOuterTrimCurve;
  

  aString opLabel[] = {"build the outer trim curve",
		       "build an inner trim curve",
                       "split periodic surface",
		       "do nothing",""}; //
  aString opCmd[]   = {"pick to build the outer trim curve",
		       "pick to build an inner trim curve",
		       "pick to split periodic surface",
		       "pick to do nothing",""}; //
  // addPrefix(label,prefix,cmd,maxCommands);
  // dialog.addOptionMenu("Pick to:", opCmd,opLabel,0);
  int numberOfRadioColumns=1;
  dialog.addRadioBox("Pick to:", opCmd,opLabel,(int)pickOption,numberOfRadioColumns);

//    aString opLabel1[] = {"none",""}; //
//    /// addPrefix(label,prefix,cmd,maxCommands);
//    dialog.addOptionMenu("active grid:", opLabel1,opLabel1,0);


  aString pbLabels[] = {"compute intersection",
                        "edit trimmed mapping...",
                        "edit trim curve",
                        "create trimmed mapping...",
                        "reset trim curves",
                        "delete all trim curves",
                        "build trim curve on boundary",
                        "reparameterize reference surface",
			""};
  // addPrefix(pbLabels,prefix,cmd,maxCommands);
  int numRows=7;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  aString tbCommands[] = {"plot reference surface",
			  "plot shaded on reference surface",
			  "plot lines on reference surface",
			  "plot trim curves",
			  "plot cutting surface",
                          "plot trimmed mappings",
                          ""};
  int tbState[9];
  tbState[0] = plotReferenceSurface==true; 
  tbState[1] = 1; 
  tbState[2] = 1; 
  tbState[3] = plotTrimCurves;
  tbState[4] = plotCuttingSurface;
  tbState[5] = plotTrimmedMappings;


  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

//     dialog.setSensitive(surfaceGrid==true,DialogData::pushButtonWidget,3);

//   bcPlotOption=colourBoundariesByGridNumber;
  
//   aString bcPlotOptionCommand[] = { "colour boundaries by grid number",
// 				    "colour boundaries by BC number",
// 				    "colour boundaries by share number",
// 				    "" };
//   dialog.addRadioBox("volume grids boundary colour:",bcPlotOptionCommand, bcPlotOptionCommand, 
//                      (int)colourBoundariesByGridNumber);
    

  const int numberOfTextStrings=5;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  aString referenceSurfaceColour="default";

  int nt=0;
  textLabels[nt] = "surface colour"; 
  sPrintF(textStrings[nt], "%s",(const char*)referenceSurfaceColour); nt++; 

  // addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("trim>");  

  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  referenceSurfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  referenceSurfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,true);
  referenceSurfaceParameters.set(GI_PLOT_UNS_FACES,true);

  referenceSurfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,true);
  referenceSurfaceParameters.set(GI_PLOT_UNS_EDGES,true);

  SelectionInfo select; select.nSelect=0;
   //  PickInfo3D pick;  pick.active=0;
  int len;
  
//   numberOfBoundaryCurves=0;
//   boundaryCurves=NULL;
//   int activeSurfaceGrid=0;
//   int activeVolumeGrid=0;

//   if( extraBoundaryCurve==NULL )
//   {
//     numberOfExtraBoundaryCurves=0;
//     maxNumberOfExtraBoundaryCurves=100;
//     extraBoundaryCurve = new Mapping *[maxNumberOfExtraBoundaryCurves];
//   }
//   else
//   {
//     assert( maxNumberOfExtraBoundaryCurves>0 );
//   }
  
  FILE *saveFile = gi.getSaveCommandFile();
  long int filePosition=-1;

  for(int it=0; ; it++)
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

      if( saveFile!=NULL )
	filePosition=ftell(saveFile);  // save the position in the file so we can over-write the last line if needed.

      gi.getAnswer(answer,"", select);

      gi.savePickCommands(true); // turn back on
    }

    printf("buildTrimmedMapping: answer=[%s]\n",(const char*)answer);


    if( answer=="pick to build the outer trim curve" ||
        answer=="pick to build an inner trim curve" ||
        answer=="pick to split periodic surface" ||
        answer=="pick to do nothing" )
    {
      pickOption = (answer=="pick to build the outer trim curve" ? pickToBuildTheOuterTrimCurve :
		    answer=="pick to build an inner trim curve"  ? pickToBuildAnInnerTrimCurve : 
                    answer=="pick to split periodic surface" ? pickToSplitPeriodicSurface : pickToDoNothing );

      dialog.getRadioBox(0).setCurrentChoice(pickOption);
    }
    else if( answer.matches("split periodic surface") ||
	     ( (select.active || select.nSelect) && 
              (pickOption==pickToSplitPeriodicSurface )) )
    {
      // --- Split a periodic surface ---
      const bool splitPeriodic = pickOption==pickToSplitPeriodicSurface || answer.matches("split periodic surface");

      Mapping *mapPointer=NULL;
      if( (len=answer.matches("split periodic surface")) )
      {
	aString name;
        name = answer(len+1,answer.length()-1);
        for( int i=0; i<mapInfo.mappingList.getLength(); i++ )
	{
	  if( mapInfo.mappingList[i].getName(Mapping::mappingName)==name )
	  {
            mapPointer = mapInfo.mappingList[i].mapPointer;
	    break;
	  }
	}
	if( mapPointer==NULL )
	{
	  printF("Error: There is no mapping named [%s]\n",(const char*)name);
	  gi.stopReadingCommandFile();
	  continue;
	}
	
      }
      else
      { // check pick values: 
	for (int i=0; i<select.nSelect && mapPointer==NULL; i++)
	{
	  for( int m=0; m<mapInfo.mappingList.getLength(); m++ )
	  {
	    Mapping & map = *mapInfo.mappingList[m].mapPointer;
	    if( map.getDomainDimension()==2 && map.getRangeDimension()==3 &&
                map.getGlobalID()==select.selection(i,0) )
	    {
	      // mapPointer=map.mapPointer;
	      mapPointer=&map;
	      printF("Mapping %i, name=%s selected.\n",m,(const char*)map.getName(Mapping::mappingName));
	      if( splitPeriodic )
                gi.outputToCommandFile(sPrintF(line,"split periodic surface %s\n",
                     (const char*)map.getName(Mapping::mappingName)));
	      break;
	    }
	  }
	  if( mapPointer==NULL )
	  {
	    printF("Error: There was no surface selected. Choose a Mapping that is a 3D surface.\n");
	    continue;
	  }

	}
      }
      assert( mapPointer!=NULL );
      
      Mapping & mapToSplit = *mapPointer;
      int axisToSplit=-1;
      for( int axis=0; axis<mapToSplit.getDomainDimension(); axis++ )
      {
	if( mapToSplit.getIsPeriodic(axis)==Mapping::functionPeriodic )
	{
	  axisToSplit=axis;
	  break;
	}
      }
      if( axisToSplit==-1 )
      {
	printF("ERROR: The mapping %s is not periodic in any direction!\n",
                    (const char*)mapToSplit.getName(Mapping::mappingName));
	continue;
      }

      real prab[4];
      #define rab(side,axis) prab[(side)+2*(axis)]
      rab(0,0)=0.; rab(1,0)=1.; rab(0,1)=0.; rab(1,1)=1.; 

      // Build two new mapping's that split the original mapping into two parts
      for( int side=0; side<=1; side++ )
      {
	ReparameterizationTransform & map = 
                 *new ReparameterizationTransform( mapToSplit,ReparameterizationTransform::restriction ); 
	map.incrementReferenceCount();
	if( side==0 )
	{
	  rab(0,axisToSplit)=0.0; rab(1,axisToSplit)=0.5;
	}
	else
	{
	  rab(0,axisToSplit)=0.5; rab(1,axisToSplit)=1.;
	}
	map.setBounds(rab(0,0),rab(1,0),rab(0,1),rab(1,1));
	aString name = mapToSplit.getName(Mapping::mappingName) + sPrintF("Side%i",side);
	map.setName(Mapping::mappingName,name);
	mapInfo.mappingList.addElement(map);  map.decrementReferenceCount();

        printF("Created mapping %s for r_%i = [%g,%g]\n",(const char*)map.getName(Mapping::mappingName),
	       axisToSplit,rab(0,axisToSplit),rab(1,axisToSplit));
	
      }
      printF("You may edit these new Mapping's to change their names or to change other properities\n");

      // regenerate option menu's to include the new mappings
      setOptionMenus(mapInfo,dialog,1);

    }
    else if( answer.matches("outer trim curve") ||
             answer.matches("inner trim curve") ||
	     ( (select.active || select.nSelect)  && 
              (pickOption==pickToBuildTheOuterTrimCurve || pickOption==pickToBuildAnInnerTrimCurve )) )
    {
      // --- Create an inner or outer trim curve ---
      //
      //  A trim curve may consist of multiple sub-curves. At this stage we do not enforce that
      //  the sub-curves form a contiguous closed curve. This is done later when the trimmed mapping is formed.


      const bool createOuter = pickOption==pickToBuildTheOuterTrimCurve || answer.matches("outer trim curve");
      const bool createInner = pickOption==pickToBuildAnInnerTrimCurve  || answer.matches("inner trim curve");

      int curveFound=-1;
      if( (len=answer.matches("outer trim curve")) )
      {
        sScanF(answer(len,answer.length()-1),"%i",&curveFound);
      }
      else if( (len=answer.matches("inner trim curve")) )
      {
        sScanF(answer(len,answer.length()-1),"%i",&curveFound);
      }
      else
      { // check pick values: 
        int zBuffMin=INT_MAX;
	for (int i=0; i<select.nSelect && curveFound<0; i++)
	{
	  // printf("i=%i, ID=%i, minZ=%i, maxZ=%i\n", i,select.selection(i,0),
	  //        select.selection(i,1),select.selection(i,2));
	  for( int b=0; b<numberOfTrimCurves; b++ )
	  {
	    if( trimCurve[b]->getGlobalID()==select.selection(i,0)  && 
                select.selection(i,1)<zBuffMin ) // look for closest curve picked
	    {
	      curveFound=b;
              zBuffMin=select.selection(i,1);
	    }
	  }
	}
	if( curveFound>=0 )
	{
	  printf("Trim curve %i selected\n",curveFound);
	  if( pickOption==pickToBuildTheOuterTrimCurve )
	    gi.outputToCommandFile(sPrintF(line,"outer trim curve %i\n",curveFound));
	  else if( pickOption==pickToBuildAnInnerTrimCurve )
	    gi.outputToCommandFile(sPrintF(line,"inner trim curve %i\n",curveFound));
	}
      }
      
      if( curveFound>=0 )
      {
	if( createOuter )
	{
	  printF("Setting the outer boundary curve to curve %i. You may add additional curves.\n",curveFound);
            
	  if( outerTrimCurve!=NULL &&  outerTrimCurve->decrementReferenceCount()==0 )
	    delete outerTrimCurve;
	  outerTrimCurve = trimParametricCurve[curveFound];
	  outerTrimCurve->incrementReferenceCount();
	}
	else if( createInner )	
	{
	  printF("Setting the inner trim curve %i to curve %i. You may add additional curves.\n",
		 numberOfInnerTrimCurves,curveFound);

	  if( innerTrimCurve==NULL )
	  {
	    const int maximumNumberOfInnerTrimCurves=50;  // *fix me*
            innerTrimCurve = new NurbsMapping *[maximumNumberOfInnerTrimCurves];
            for( int i=0; i<maximumNumberOfInnerTrimCurves; i++ )
	      innerTrimCurve[i]=NULL;
	  }
          NurbsMapping *& inner = innerTrimCurve[numberOfInnerTrimCurves]; // make a short name

	  if( inner!=NULL &&  inner->decrementReferenceCount()==0 )
            delete inner;
	  inner = trimParametricCurve[curveFound]; // note: use the parametric curve
	  inner->incrementReferenceCount();
          numberOfInnerTrimCurves++;
	}

        // --- Prompt for further curves to be appended to the current trim curve ---
	GUIState doneDialog;
	if( createOuter )
	  doneDialog.setWindowTitle("Build the outer trim curve");
	else
	  doneDialog.setWindowTitle("Build an inner trim curve");
	doneDialog.setExitCommand("done","done");
	doneDialog.addInfoLabel("Append more curves.");
	aString doneMenu[] = {"done",""};  //
	doneDialog.buildPopup(doneMenu);
	gi.pushGUI(doneDialog);

	for( int it=0;; it++ )
	{
	  gi.savePickCommands(false); // temporarily turn off saving of pick commands.     
	  gi.getAnswer(answer,"",select);
	  gi.savePickCommands(true); 
	  if( answer=="done" )
	  {
	    break;
	  }
	  else if( (select.nSelect ||
		    (createOuter && answer.matches("add outer trim curve")) || 
		    (createInner && answer.matches("add inner trim curve")) ) )
	  {
	    int curveFound=-1;
	    if( (len=answer.matches("add outer trim curve")) )
	    {
	      sScanF(answer(len,answer.length()-1),"%i",&curveFound);
	    }
	    else if( (len=answer.matches("add inner trim curve")) )
	    {
	      sScanF(answer(len,answer.length()-1),"%i",&curveFound);
	    }
	    else
	    { // check pick values and find the closest curve picked: 
              int zBuffMin=INT_MAX;
	      for (int i=0; i<select.nSelect && curveFound<0; i++)
	      {
		// printf("i=%i, ID=%i, minZ=%i, maxZ=%i\n", i,select.selection(i,0),
		//        select.selection(i,1),select.selection(i,2));
		for( int b=0; b<numberOfTrimCurves; b++ )
		{
		  if( trimCurve[b]->getGlobalID()==select.selection(i,0) && 
                      select.selection(i,1)<zBuffMin ) // look for closest curve picked
		  {
		    curveFound=b;
                    zBuffMin=select.selection(i,1);
		    
		  }
		}
	      }
	      if(  curveFound>=0 )
	      {
		printf("Trim curve %i selected\n",curveFound);
		if( createOuter )
		  gi.outputToCommandFile(sPrintF(line,"add outer trim curve %i\n",curveFound));
		else if( createInner )
		  gi.outputToCommandFile(sPrintF(line,"add inner trim curve %i\n",curveFound));
	      }
	    }
	    if( curveFound>=0 )
	    {
	      if( createOuter )
	      {
		printF("Adding curve %i to the outer trim curve. You can edit the trim curves later"
		       " when the trimmed mapping is created.\n", curveFound);
		outerTrimCurve->addSubCurve(*trimParametricCurve[curveFound]);
	      }
	      else
	      {
		printF("Adding curve %i to the inner trim curve %i. You can edit the trim curves later"
		       " when the trimmed mapping is created.\n", curveFound,numberOfInnerTrimCurves-1);
		assert( numberOfInnerTrimCurves>0 );
		innerTrimCurve[numberOfInnerTrimCurves-1]->addSubCurve(*trimParametricCurve[curveFound]);
	      }
	      
	    }
	    else
	    {
	      printF("No curve was selected. Try again or choose 'done'\n");
	    }
	  }
	  else 
	  {
	    printF("Unexpected answer=[%s]\n",(const char*)answer);
	    gi.stopReadingCommandFile();
	  }
	    
	}
	  
	gi.popGUI();
	
      } // end if curveFound>=0
    }
    else if( (len=answer.matches("Surface to trim:")) )
    {
      aString name=answer(len,answer.length()-1);
      const int num=mapInfo.mappingList.getLength();
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( (map.getDomainDimension()==2 && map.getRangeDimension()==3) )
	{
	  if( name==map.getName(Mapping::mappingName) )
	  {
	    if( surface!=0 && surface->decrementReferenceCount()==0 ) 
	      delete surface;
	    surface=mapInfo.mappingList[i].mapPointer;

            printf(" New reference surface is %s\n",(const char*)surface->getName(Mapping::mappingName));
	    
	    surface->incrementReferenceCount();
	    pSurface=surface;
	    
            plotObject=true;
            newSurface=true;
            break;
	  }
	}
      }
    }
    else if( (len=answer.matches("Trim with:")) )
    {
      // A surface has been chosen to trim the reference surface with ...
      aString name=answer(len,answer.length()-1);
      const int num=mapInfo.mappingList.getLength();
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( (map.getDomainDimension()==2 && map.getRangeDimension()==3) )
	{
	  if( name==map.getName(Mapping::mappingName) )
	  {
	    if( cuttingSurface!=0 && cuttingSurface->decrementReferenceCount()==0 ) 
	      delete cuttingSurface;
	    cuttingSurface=mapInfo.mappingList[i].mapPointer;

            printf(" Cut with surface %s\n",(const char*)cuttingSurface->getName(Mapping::mappingName));
	    
	    cuttingSurface->incrementReferenceCount();
	    
            plotObject=true;
            break;
	  }
	}
      }
      
    }
    else if( answer=="build trim curve on boundary" )
    {
      NurbsMapping *curve =  new NurbsMapping; curve->incrementReferenceCount();     
      NurbsMapping *pCurve = new NurbsMapping; pCurve->incrementReferenceCount();

      // construct the physical space and parameteric curves for the outer boundary
      constructOuterBoundaryCurves( *pSurface, curve,pCurve );

      addCurve( *curve,*pCurve );

      pCurve->decrementReferenceCount();
      curve->decrementReferenceCount();
    }
    else if( (len=answer.matches("surface colour")) )
    {
      referenceSurfaceColour=answer(len+1,answer.length()-1);
      cout << "answer=[" << answer << "]" << endl;
      cout << "referenceSurfaceColour=[" << referenceSurfaceColour << "]" << endl;
      
      if( pSurface!=NULL )
      {
	if( referenceSurfaceColour=="default" )
	{
	  // The default is multi-coloured
	  if( pSurface->getClassName()=="CompositeSurface" )
	  {
	    CompositeSurface & cs = (CompositeSurface&)(*pSurface);
	    for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	    {
	      cs.setColour(s,gi.getColourName(s));
	    }
	    cs.eraseCompositeSurface(gi);
	  }
	}
	else
	{
	  if( pSurface->getClassName()=="CompositeSurface" )
	  {
	    CompositeSurface & cs = (CompositeSurface&)(*pSurface);
	    for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	    {
	      cs.setColour(s,referenceSurfaceColour);
	    }
	    cs.eraseCompositeSurface(gi);
	  }
	}
	plotObject=true;      
      }
      else
      {
        printf("INFO:Sorry, there is no reference surface to set the colour for\n");
      }
    }
    else if( (len=answer.matches("plot reference surface")) )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotReferenceSurface=value;
      dialog.setToggleState("plot reference surface",plotReferenceSurface);
    }
    else if( (len=answer.matches("plot shaded on reference surface")) )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("plot shaded on reference surface",value);
      referenceSurfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,value);
      referenceSurfaceParameters.set(GI_PLOT_UNS_FACES,value);
    }
    else if( (len=answer.matches("plot lines on reference surface")) )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); 
      dialog.setToggleState("plot lines on reference surface",value);
      referenceSurfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,value);
      referenceSurfaceParameters.set(GI_PLOT_UNS_EDGES,value);
    }
    else if( dialog.getToggleValue(answer,"plot trim curves",plotTrimCurves) ){}//
    else if( dialog.getToggleValue(answer,"plot cutting surface",plotCuttingSurface) ){}//
    else if( dialog.getToggleValue(answer,"plot trimmed mappings",plotTrimmedMappings) ){}//
    else if( answer=="edit reference surface" )
    {
      if( surface!=0 )
      {
	surface->update(mapInfo);
	plotObject=true;
      }
      else
      {
	printf("MappingBuilder:ERROR:There is no reference surface to edit\n");
      }
    }
    else if( answer=="build curve on surface" )
    {
      printF(" Finish me: use routine from MappingBuilder...\n");
      // buildCurveOnSurface(mapInfo);
      plotObject=true;

    }
    else if( answer=="edit trim curve" )
    {
      if( numberOfTrimCurves>0 )
      {
        for( int n=0; n<numberOfTrimCurves; n++ )
	{
	  printf("edit trim curve %i\n",n);
	  gi.erase();
	  trimCurve[n]->update(mapInfo);
	}
	
      }
    }
    else if( answer=="reparameterize reference surface" )
    {
      printF("The reference surface is being replaced by a reparmeterized surface. \n"
             "In this way you can split a periodic surface for example.\n");

      ReparameterizationTransform & map = 
	*new ReparameterizationTransform( *pSurface,ReparameterizationTransform::restriction ); 
      map.incrementReferenceCount();
      map.update(mapInfo);
      
      if( pSurface->decrementReferenceCount()==0 )
	delete pSurface;

      pSurface = &map;

      // add to the list of mapping's
      mapInfo.mappingList.addElement(map);  map.decrementReferenceCount();

      // regenerate option menu's to include the new mappings
      setOptionMenus(mapInfo,dialog,1);

      // dialog.getOptionMenu("type:").setCurrentChoice((int)filletType);
      

    }
    else if( answer=="compute intersection" )
    {
      // compute the intersection between the surface surface and the cutting surface
      assert( pSurface!=NULL && pSurface!=cuttingSurface );
      assert( cuttingSurface!=NULL );

      IntersectionMapping inter;
      bool intersectionFound=false;
      
      // Mapping::debug=7;
      // NOTE: computed intersection is a NurbsMapping with possible disjoint sub-curves for 
      //       the different intersection curves 
      int result = inter.intersect(*pSurface,*cuttingSurface,&gi);
      if( result!=0 )
      {
	printF("buildTrimmedMapping:ERROR in computing the intersection curve \n");
        printF("INFO: the intersection computation can have trouble if the grid lines on the one surface\n"
               " lie exactly on the other surface. Changing the number of grid lines may fix this.\n");
        intersectionFound=false;
      }
      else
      {
	printF("buildTrimmedMapping:INFO: An intersection curve was constructed.\n");
        intersectionFound=true;
        assert( inter.curve->getClassName()=="NurbsMapping" );
        NurbsMapping & iNurbs = *((NurbsMapping*)inter.curve);
        printF("INFO: there were %i intersection curves found.\n",iNurbs.numberOfSubCurves());
        assert( inter.rCurve1->getClassName()=="NurbsMapping" );
        NurbsMapping & pNurbs = *((NurbsMapping*)inter.rCurve1);
        printF("INFO: Parameter space : there were %i intersection curves found.\n",pNurbs.numberOfSubCurves());
        assert( iNurbs.numberOfSubCurves()==pNurbs.numberOfSubCurves() );



 	for( int sc=0; sc<iNurbs.numberOfSubCurves(); sc++ )
 	{
 	  NurbsMapping & iCurve = iNurbs.numberOfSubCurves()==1 ? iNurbs : iNurbs.subCurve(sc);
 	  NurbsMapping & pCurve = iNurbs.numberOfSubCurves()==1 ? pNurbs : pNurbs.subCurve(sc);

          addCurve( iCurve,pCurve );
	}
	
      }
      
    }
    else if( answer=="create trimmed mapping..." )
    {
      if( trimmedMapping!=NULL && trimmedMapping->decrementReferenceCount()==0 )
	delete trimmedMapping;
      
      // trimmedMapping= new TrimmedMapping(*pSurface,numberOfTrimCurves,trimParametricCurve );
      trimmedMapping= new TrimmedMapping(*pSurface,(Mapping*)outerTrimCurve,numberOfInnerTrimCurves,(Mapping**)innerTrimCurve );
      trimmedMapping->incrementReferenceCount();
      trimmedMapping->update(mapInfo);

      mapInfo.mappingList.addElement(*trimmedMapping);  

      trimmedMapping->decrementReferenceCount();
      trimmedMapping=NULL;
    }
    else if( answer=="delete mapping" )
    {
      int num=mapInfo.mappingList.getLength();
      if( num<=0 )
      {
	printf("There are no mappings available! \n");
	continue;
      }

      aString *menu2 = new aString[num+3];
      int i=0;
      menu2[i++]="!delete mapping";
      int mappingListStart=i;
      for( int j=0; j<num; j++ )
	menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
      int mappingListEnd=i-1;
      menu2[i++]="none"; 
      menu2[i]="";   // null string terminates the menu

      gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

      int map = gi.getMenuItem(menu2,answer2,"delete which mapping?");
      delete [] menu2;

      gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

      if( map>=0 && answer2!="none" )
      {
	Mapping *mapPointer=mapInfo.mappingList[map].mapPointer;
        printf("delete mapping %s \n",(const char *) mapPointer->getName(Mapping::mappingName));
        mapInfo.mappingList.deleteElement(map);  // delete the mapping

      }
      else if( map<0 )
      {
        gi.outputString("Error: unknown mapping name!");
        gi.stopReadingCommandFile();
      }
    }
    else if( answer=="reset trim curves" )
    {
      resetTrimCurves();
    }
    else if( answer=="delete all trim curves" )
    {
      resetTrimCurves();
      deleteCurves();
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
    {
    }
    else 
    {
      gi.outputString( sPrintF(line,"Unknown response=%s",(const char*)answer) );
      printF("Unknown response=[%s] \n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    if( newSurface )
    {
      newSurface=false;
      
    }

    if( plotObject )
    {
      plotCurvesAndSurfaces(mapInfo);
      
    }
  }
  gi.erase(); 

  // clean up 

  resetTrimCurves();
  deleteCurves();
  

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return returnValue;
}
