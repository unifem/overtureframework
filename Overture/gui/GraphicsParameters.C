#include "GraphicsParameters.h"
#include "GenericGraphicsInterface.h"

#ifdef NO_APP
using GUITypes::real;
#define getLength size
#define redim resize
using std::cout;
using std::endl;
#endif

// scale factors for plotting to change the aspect ratio
GUITypes::real GraphicsParameters::xScaleFactor=1., 
               GraphicsParameters::yScaleFactor=1., 
               GraphicsParameters::zScaleFactor=1.;

// Here is the form of a user defined colour table
void 
defaultColourTableFunction(const real & value, real & red, real & green, real & blue)
// ===================================================================================================
// /Description: Convert a value from [0,1] into (red,green,blue) values, each in the range [0,1]
// /value (input) :  0 <= value <= 1 
// /red, green, blue (output) : values in the range [0,1]
// ===================================================================================================
{ // a sample user defined colour table function: 
  red=0.;
  green=value;
  blue=(1.-value);
}


  
//\begin{>GraphicsParametersInclude.tex}{\subsection{Constructor}} 
GraphicsParameters:: 
GraphicsParameters(bool default0)
//----------------------------------------------------------------------
// /Description:
//   Constructor 
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  defaultObject=default0;
  // assign default values
  plotBoundsChanged=false;
  relativeChangeForPlotBounds=.1;  // use with usePlotBoundsOrLarger

  colourTable=rainbow;
  plotBound.redim(2,3); 
  plotBound=0.; 
#ifndef NO_APP
  plotBound(Start,0)=1; plotBound(End,0)=-1.;
#else
  plotBound(0,0) = 1;   plotBound(1,0) = -1;
#endif
  backGroundGridDimension.redim(3); backGroundGridDimension=0;
  topLabel="";
  topLabel1="";
  topLabel2="";
  topLabel3="";
  bottomLabel="";
  bottomLabel1="";
  bottomLabel2="";
  bottomLabel3="";
  plotTitleLabels=true;   // plot topLabel etc.
  labelColourBar=true;    // plot labels on the colour bar
  labelMinMax=false;      // show min and max values as text on the plot
  labelComponent=true;      // include the component name on the top label (e.g. for contour)
  
  plotObject=true;         // true means we immediately plot the object on entering a plotting routine
  plotObjectAndExit=false; // immediately plot the object then exit
  usePlotBounds=false;               // use plot bounds found in the plotBound array
  usePlotBoundsOrLarger  =false;      // plot bounds should include bounds from plotBound array
  mappingColour="red";
  lineOffset=0.;
  pointOffset=0.;
  //  surfaceOffset=3.;             // offset mappings 3 units behind grid lines
  // surfaceOffset=30.;             // offset mappings this many units behind grid lines
  // surfaceOffset=7.;             // offset mappings 7 units behind grid lines
  surfaceOffset=15.;             // *wdh* increased


  plotShadedMappingBoundaries=true;  // show shaded surfaces when plotting Mappings.
  plotMappingEdges=true;
  
  numberOfGhostLinesToPlot     =0;
  labelGridsAndBoundaries      =false;
  plotInterpolationPoints      =false;
  plotInterpolationCells       =true;  
  plotBackupInterpolationPoints=false;
  plotNonPhysicalBoundaries    =false;
  labelBoundaries              =false;
  plotLinesOnMappingBoundaries =true;
  plotLinesOnGridBoundaries    =false;
  plotGridLines                =1+0*2;            // bit 1 for 2D , bit 2 for 3D
  plotGridBlockBoundaries      =true;
  plotShadedSurfaceGrids       =false;
  plotGridPointsOnCurves       =true;
  plotEndPointsOnCurves        =false;
  plotBranchCuts               =false;
  plotMappingNormals           =false;
  plotInteriorBoundaryPoints   =true;  // set default to be true, they can be turned off 070509
  plotHiddenRefinementPoints   =false;
  plotNurbsCurvesAsSubCurves   =true;  // be default plot sub-curves when plotting a NURBS curve
  
// unstructured stuff
  plotUnsNodes = false;
  plotUnsFaces = true;
  plotUnsEdges = false;
  plotUnsBoundaryEdges = false;

  boundaryColourOption=defaultColour;
  gridLineColourOption=defaultColour;
  blockBoundaryColourOption=colourByGrid;

  boundaryColourValue=0;       // colour number to use if boundaryColourOption==colourByValue
  gridLineColourValue=0;
  blockBoundaryColourValue=0;

  colourInterpolationPoints=false;
  plotTheAxes            =true;
  plotContourLines       =true;
  plotShadedSurface      =true;
  numberOfContourLevels  =11;
  plotWireFrame=false;
  colourLineContours=false;
  plotColourBar=true;
  plotGridBoundariesOnContourPlots=true;
  contourSurfaceVerticalScaleFactor=.75;
  contourSurfaceSpatialBound=-1.;  // by default use bounds from the grid
  flatShading=false;
  
  numberOfContourPlanes=-1;

  uComponentForStreamLines=-999;
  vComponentForStreamLines=-999;
  wComponentForStreamLines=-999;
  streamLineArrowSize=.015;
  componentForContours=0;
  componentForSurfaceContours=0;
  objectWasPlotted=true;
  yLevelFor1DGrids=0.;
  zLevelFor2DGrids=0.;
  multigridLevelToPlot=0;
  refinementLevelToPlot=0;
  plotRefinementGrids=true;
  keepAspectRatio=true;
  computeCoarseningFactor=true;
  
  outputFormat=colour8Bit;
  rasterResolution=0;
  hardCopyType=postScript;

  minAndMaxContourLevelsSpecified.redim(10); // AP: Needs access function
  minAndMaxContourLevelsSpecified=false;
  
  minAndMaxContourLevels.redim(2,10);            // for user specified min and max contour levels
  minAndMaxContourLevels=0.;
  minimumContourSpacing=0.;
  axesOrigin.redim(3);
  axesOrigin=GenericGraphicsInterface::defaultOrigin;
  plotDashedLinesForNegativeContours=true;

  plot2DContoursOnCoordinatePlanes=false;
  normalAxisFor2DContoursOnCoordinatePlanes=2;
  numberOfIsoSurfaces=0;
  numberOfCoordinatePlanes=0;
  numberOfGridCoordinatePlanes=0;
  contour3dMinMaxOption=baseMinMaxOnGlobalValues;
  
  
  // **** add these to setParameters ****
  linePlots=false;                // plot solution on lines that intersect the range
  numberOfLines=0;
  numberOfPointsPerLine=0;

  minStreamLine=0.;
  maxStreamLine=0.;
  streamLineStoppingTolerance=1.e-3;
  minAndMaxStreamLinesSpecified=false;

  numberOfStreamLineStartingPoints=0;

  // points:
  pointSize=3;  // size in pixels
  pointSymbol=0;
  pointColour="black";
  lineColour="black";   // for plotLines
  
  // set default sizes
  size.redim(numberOfSizes);         // holds sizes for items in the Sizes enum
  size=0.;
  size(lineWidth)=1.;         // size in pixels! for a general line
  size(axisNumberSize)=.03; // .025;  // size of number labels on the axes (normalized coordinates)
  size(axisLabelSize)=.03; // .025;   // size of axis label ("x-axis")
  size(axisMinorTickSize)=.01;
  size(axisMajorTickSize)=.02;
  size(topLabelSize)=.04;     
  size(topSubLabelSize)=.035;    
  size(bottomLabelSize)=.04;
  size(bottomSubLabelSize)=.035;
  size(minorContourWidth)=1.;
  size(majorContourWidth)=1.;
  size(streamLineWidth)=1.;
  size(labelLineWidth)=1.;
  size(curveLineWidth)=1.;         // size in pixels! for a curve drawn

  // unstructured mapping stuff
  useUnsCutplane = false;
  useUnsFlatShading = false;
  unsCutplaneVertex.redim(3);
  unsCutplaneVertex = 0;
  unsCutplaneNormal.redim(3);
  unsCutplaneNormal = 0;
  unsCutplaneNormal(0) = 1;

  // displacement 
  adjustGridForDisplacement=0;  // plot contours etc. with grid + displacement instead of the grid 
  displacementScaleFactor=1.;
  displacementComponent[0]=0;
  displacementComponent[1]=1;
  displacementComponent[2]=2;

  colourTableFunction=&defaultColourTableFunction;

  showFileReader=NULL;  // pointer to an active ShowFileReader (if any)
  showFileSolutionNumber=-1; // current solution number in the show file
  showFileParameters=NULL; // pointer to an active ListOfShowFileParameters (if any)
  
}

GraphicsParameters:: 
~GraphicsParameters()
{
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{isDefault}} 
bool GraphicsParameters:: 
isDefault()
//----------------------------------------------------------------------
// /Description:
//   Return true if this object is a default object. 
// This routine can be used to tell whether a GraphicsParameter object is
// equal to the static object {\tt Overture::defaultGraphicsParameters()} which can
// be used as a default argument in a function call.
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  return defaultObject;
}


//\begin{>>GraphicsParametersInclude.tex}{\subsection{getObjectWasPlotted}} 
int GraphicsParameters::
getObjectWasPlotted() const
//----------------------------------------------------------------------
// /Description:
//    Determine if the object was plotted in the last plotting routine
//    that was called.
// /Return value: true if an object was plotted, false otherwise.
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  return objectWasPlotted;
}


//\begin{>>GraphicsParametersInclude.tex}{\subsection{get(aString)}} 
aString & GraphicsParameters::
get(const GraphicsOptions & option, aString & label) const
//----------------------------------------------------------------------
// /Description:
//   Return the aString associated with a GraphicsParameter option.
// /option (input) : Return the aString associated with this option (if any).
// /label (output) : Return the string in this variable
// /Return value: the return value is also equal to label.
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  switch (option)
  {
  case GI_TOP_LABEL:
    label=topLabel;
    break;
  case GI_TOP_LABEL_SUB_1:
    label=topLabel1;
    break;
  case GI_TOP_LABEL_SUB_2:
    label=topLabel2;
    break;
  case GI_TOP_LABEL_SUB_3:
    label=topLabel3;
    break;
  case GI_BOTTOM_LABEL:
    label=bottomLabel;
    break;
  case GI_BOTTOM_LABEL_SUP_1:
    label=bottomLabel1;
    break;
  case GI_BOTTOM_LABEL_SUP_2:
    label=bottomLabel2;
    break;
  case GI_BOTTOM_LABEL_SUP_3:
    label=bottomLabel3;
    break;
  case GI_POINT_COLOUR:
    label=pointColour;
    break;
  case GI_LINE_COLOUR:
    label=lineColour;
    break;
  case GI_MAPPING_COLOUR:
    label=mappingColour;
    break;
  default :
    cout << "GraphicsParameters::get(aString): ERROR: unknown option = " << option << endl;
  }
  return label;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{get(int)}} 
int & GraphicsParameters::
get(const GraphicsOptions & option, int & value) const
//----------------------------------------------------------------------
// /Description:
//   Return the int associated with a GraphicsParameter option.
// /option (input) : Return the int value associated with this option (if any).
// /value (output) : Return the value in this variable
// /Return value: the return value is also equal to value.
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  switch (option)
  {
  case GI_ADJUST_GRID_FOR_DISPLACEMENT:
    value=adjustGridForDisplacement;
    break;
  case GI_BLOCK_BOUNDARY_COLOUR_OPTION:
      value=blockBoundaryColourOption;
    break;
  case GI_BOUNDARY_COLOUR_OPTION:
      value=boundaryColourOption;
    break;
  case GI_COLOUR_INTERPOLATION_POINTS:
    value=colourInterpolationPoints;
    break;
  case GI_COLOUR_LINE_CONTOURS:
    value=colourLineContours;
    break;
  case GI_DISPLACEMENT_U_COMPONENT:
    value=displacementComponent[0];
    break;
  case GI_DISPLACEMENT_V_COMPONENT:
    value=displacementComponent[1];
    break;
  case GI_DISPLACEMENT_W_COMPONENT:
    value=displacementComponent[2];
    break;
  case GI_FLAT_SHADING:
    value=flatShading;
    break;
  case GI_GRID_LINE_COLOUR_OPTION:
      value=gridLineColourOption;
    break;
  case GI_KEEP_ASPECT_RATIO:
    value=keepAspectRatio;
    break;
  case GI_COMPUTE_COARSENING_FACTOR:
    value=computeCoarseningFactor;
    break;
  case GI_CONTOUR3D_MIN_MAX_OPTION:
    value=contour3dMinMaxOption;
    break;
  case GI_LABEL_COMPONENT:
    value=labelComponent;
    break;
  case GI_LABEL_COLOUR_BAR:
    value=labelColourBar;
    break;
  case GI_LABEL_GRIDS_AND_BOUNDARIES:
    value=labelGridsAndBoundaries;
    break;
  case GI_LABEL_MIN_MAX:
    value=labelMinMax;
    break;
  case GI_PLOT_BLOCK_BOUNDARIES:
      value=plotGridBlockBoundaries;
    break;
  case GI_PLOT_COLOUR_BAR:
    value=plotColourBar;
    break;
  case GI_PLOT_CONTOUR_LINES:
    value=plotContourLines;
    break;
  case GI_PLOT_GRID_LINES:
    value=plotGridLines;
    break;
  case GI_PLOT_LABELS:
    value=plotTitleLabels;
    break;
  case GI_PLOT_LINES_ON_MAPPING_BOUNDARIES:
    value=plotLinesOnMappingBoundaries;
    break;
  case GI_PLOT_MAPPING_EDGES:
    value=plotMappingEdges;
    break;
  case GI_PLOT_MAPPING_NORMALS:
    value=plotMappingNormals;
    break;
  case GI_PLOT_GRID_BOUNDARIES_ON_CONTOUR_PLOTS:
    value=plotGridBoundariesOnContourPlots;
    break;
  case GI_PLOT_LINES_ON_GRID_BOUNDARIES:
    value=plotLinesOnGridBoundaries;
    break;
  case GI_PLOT_GRID_POINTS_ON_CURVES:
    value=plotGridPointsOnCurves;
    break;
  case GI_PLOT_END_POINTS_ON_CURVES:
    value=plotEndPointsOnCurves;
    break;
  case GI_PLOT_HIDDEN_REFINEMENT_POINTS:
    value=plotHiddenRefinementPoints;
    break;
  case GI_PLOT_INTERIOR_BOUNDARY_POINTS:
    value=plotInterpolationPoints;
    break;
  case GI_PLOT_INTERPOLATION_POINTS:
    value=plotInterpolationPoints;
    break;
  case GI_PLOT_INTERPOLATION_CELLS:
    value=plotInterpolationCells;
    break;
  case GI_PLOT_NON_PHYSICAL_BOUNDARIES:
    value=plotNonPhysicalBoundaries;
    break;
  case GI_PLOT_NURBS_CURVES_AS_SUBCURVES:
    value=plotNurbsCurvesAsSubCurves;
    break;
  case GI_PLOT_BACKUP_INTERPOLATION_POINTS:
    value=plotBackupInterpolationPoints;
    break;
  case GI_PLOT_SHADED_MAPPING_BOUNDARIES:
    value=plotShadedMappingBoundaries;
    break;
  case GI_PLOT_REFINEMENT_GRIDS:
    value=plotRefinementGrids;
    break;
  case GI_PLOT_SHADED_SURFACE:
    value=plotShadedSurface;
    break;
  case GI_PLOT_SHADED_SURFACE_GRIDS:
    value=plotShadedSurfaceGrids;
    break;
  case GI_PLOT_THE_OBJECT:
    value=plotObject;
    break;
  case GI_PLOT_THE_OBJECT_AND_EXIT:
    value=plotObjectAndExit;
    break;
  case GI_PLOT_WIRE_FRAME:
    value=plotWireFrame;
    break;
  case GI_PLOT_2D_CONTOURS_ON_COORDINATE_PLANES:
    value=plot2DContoursOnCoordinatePlanes;
    break;
  case GI_PLOT_UNS_NODES:
    value=plotUnsNodes;
    break;
  case GI_PLOT_UNS_FACES:
    value=plotUnsFaces;
    break;
  case GI_PLOT_UNS_EDGES:
    value=plotUnsEdges;
    break;
  case GI_PLOT_UNS_BOUNDARY_EDGES:
    value=plotUnsBoundaryEdges;
    break;
  case GI_COLOUR_TABLE:
    value=colourTable;
    break;
  case GI_COMPONENT_FOR_CONTOURS:
    value=componentForContours;
    if( componentsToPlot.getLength(0) > 0 )
      value=componentsToPlot(0);
    break;
  case GI_COMPONENT_FOR_SURFACE_CONTOURS:
    value=componentForSurfaceContours;
    break;
  case GI_HARD_COPY_TYPE:
    value=hardCopyType;
    break;
  case GI_MULTIGRID_LEVEL_TO_PLOT:
    value=multigridLevelToPlot;
    break;
  case GI_NORMAL_AXIS_FOR_2D_CONTOURS_ON_COORDINATE_PLANES:
    value=normalAxisFor2DContoursOnCoordinatePlanes;
    break;
  case GI_NUMBER_OF_CONTOUR_LEVELS:
    value=numberOfContourLevels;
    break;
  case GI_NUMBER_OF_GHOST_LINES_TO_PLOT:
    value=numberOfGhostLinesToPlot;
    break;
  case GI_OUTPUT_FORMAT:
    value=outputFormat;
    break;
  case GI_POINT_SYMBOL:
    value=pointSymbol;
    break;
  case GI_RASTER_RESOLUTION:
    value=rasterResolution;
    break;
  case GI_REFINEMENT_LEVEL_TO_PLOT:
    value=refinementLevelToPlot;
    break;
  case GI_U_COMPONENT_FOR_STREAM_LINES:
    value=uComponentForStreamLines;
    break;
  case GI_V_COMPONENT_FOR_STREAM_LINES:
    value=vComponentForStreamLines;
    break;
  case GI_W_COMPONENT_FOR_STREAM_LINES:
    value=wComponentForStreamLines;
    break;
  case GI_USE_PLOT_BOUNDS:
    value=usePlotBounds;
    break;
  case GI_USE_PLOT_BOUNDS_OR_LARGER:
    value=usePlotBoundsOrLarger;  
    break;
  case GI_UNS_USE_CUT_PLANE:
    value = useUnsCutplane;
    break;
  case GI_UNS_FLAT_SHADING:
    value = useUnsFlatShading;
    break;
  default :
    cout << "GraphicsParameters::get(int): ERROR: unknown option = " << option << endl;
    throw "error";
  }
  return value;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{get(real)}} 
real & GraphicsParameters:: 
get(const GraphicsOptions & option, real & value) const
//----------------------------------------------------------------------
// /Description:
//   Return the real associated with a GraphicsParameter option.
// /option (input) : Return the real value associated with this option (if any).
// /value (output) : Return the value in this variable
// /Return value: the return value is also equal to value.
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  switch (option)
  {
  case GI_X_SCALE_FACTOR:
    value=xScaleFactor;
    break;
  case GI_Y_SCALE_FACTOR:
    value=yScaleFactor;
    break;
  case GI_Z_SCALE_FACTOR:
    value=zScaleFactor;
    break;
  case GI_CONTOUR_SURFACE_VERTICAL_SCALE_FACTOR:
    value=contourSurfaceVerticalScaleFactor;
    break;
  case GI_CONTOUR_SURFACE_SPATIAL_BOUND:
    value=contourSurfaceSpatialBound;
    break;
  case GI_DISPLACEMENT_SCALE_FACTOR:
    value=displacementScaleFactor;
    break;
  case GI_LINE_OFFSET:
    value=lineOffset;
    break;
  case GI_POINT_OFFSET:
    value=pointOffset;
    break;
  case GI_SURFACE_OFFSET:
    value=surfaceOffset;
    break;
  case GI_MINIMUM_CONTOUR_SPACING:
    value=minimumContourSpacing;
    break;
  case GI_POINT_SIZE:
    value=pointSize;
    break;
  case GI_STREAM_LINE_TOLERANCE:
    value=streamLineStoppingTolerance;
    break;
  case GI_STREAM_LINE_ARROW_SIZE:
    value=streamLineArrowSize;
    break;
  case GI_Y_LEVEL_FOR_1D_GRIDS:
    value=yLevelFor1DGrids;
    break;
  case GI_Z_LEVEL_FOR_2D_GRIDS:
    value=zLevelFor2DGrids;
    break;
  default :
    cout << "GraphicsParameters::get(real): ERROR: unknown option = " << option << endl;
  }
  return value;

}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{get(IntegerArray)}} 
IntegerArray & GraphicsParameters::
get(const GraphicsOptions & option, IntegerArray & values) const
//----------------------------------------------------------------------
// /Description:
//   Return the IntegerArray associated with a GraphicsParameter option.
// /option (input) : Return the IntegerArray value associated with this option (if any).
// /value (output) : Return the value in this variable
// /Return value: the return value is also equal to value.
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  switch (option)
  {
  case GI_GRID_BOUNDARY_CONDITION_OPTIONS:
    values=gridBoundaryConditionOptions;
    break;
  case GI_GRID_OPTIONS:
    values=gridOptions;
    break;
  case GI_GRIDS_TO_PLOT:
    values=gridsToPlot;
    break;
  case GI_BACK_GROUND_GRID_FOR_STREAM_LINES:
    values=backGroundGridDimension;
    break;
  case GI_COMPONENTS_TO_PLOT:
    values=componentsToPlot;
    break;
  case GI_CONTOUR_ON_GRID_FACE:
    values=plotContourOnGridFace;
    break;
  default :
    cout << "GraphicsParameters::get(IntegerArray): ERROR: unknown option = " << option << endl;
  }
  return values;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{get(RealArray)}} 
RealArray & GraphicsParameters::
get(const GraphicsOptions & option, RealArray & values) const
//----------------------------------------------------------------------
// /Description:
//   Return the RealArray associated with a GraphicsParameter option.
// /option (input) : Return the RealArray value associated with this option (if any).
// /value (output) : Return the value in this variable
// /Return value: the return value is also equal to value.
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  switch (option)
  {
  case GI_AXES_ORIGIN:
    values=axesOrigin;
    break;
  case GI_PLOT_BOUNDS:
    values=plotBound;
    break;
  case GI_ISO_SURFACE_VALUES:
    values.redim(0);
    values=isoSurfaceValue;
    break;
  case GI_MINIMUM_CONTOUR_SPACING:
    values=minAndMaxContourLevels;
    break;
  case GI_CONTOUR_LEVELS:
    values.redim(0);
    values=values=contourLevels;
    break;
  case GI_MIN_AND_MAX_CONTOUR_LEVELS:
    values.redim(0);
    values=minAndMaxContourLevels;
    break;
  case GI_MIN_AND_MAX_STREAM_LINES:
    values(0)=minStreamLine;
    values(1)=maxStreamLine;
    break;
  case GI_UNS_CUT_PLANE_VERTEX:
    values = unsCutplaneVertex;
    break;
  case GI_UNS_CUT_PLANE_NORMAL:
    values = unsCutplaneNormal;
    break;
  default :
    cout << "GraphicsParameters::get(RealArray): ERROR: unknown option = " << option << endl;
  }
  return values;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{get(Sizes)}} 
real & GraphicsParameters::
get(const Sizes & option, real & value) const
//----------------------------------------------------------------------
// /Description:
//   Deterimine the value of a {\it size} parameter
// /option (input) : determine the value for this size.
// /value (output) : the value.
// /Return value: the return value is also equal to value.
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  value=size(option);
  return value;
}



//\begin{>>GraphicsParametersInclude.tex}{\subsection{set(GraphicsOptions, int/real)}} 
int GraphicsParameters:: 
set(const GraphicsOptions & option, real value)
//----------------------------------------------------------------------
// /Description:
//   Assign a parameter with an int or real value
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  int returnValue=0;
  int grid;

  int ivalue = value>0. ? int(value+.5) : int( value-.5);  // round to the nearest integer
  
  switch (option)
  {
  case GI_ADJUST_GRID_FOR_DISPLACEMENT:
    adjustGridForDisplacement=ivalue;
    break;
  case GI_PLOT_BACKUP_INTERPOLATION_POINTS:
    plotBackupInterpolationPoints=ivalue;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotBackupInterpolationPoints )
	gridOptions(grid) |=plotBackupInterpolation;
      else
	gridOptions(grid) &= !plotBackupInterpolation;
    }
    break;
  case GI_BLOCK_BOUNDARY_COLOUR_OPTION:
      blockBoundaryColourOption=ivalue;
    break;
  case GI_BOUNDARY_COLOUR_OPTION:
      boundaryColourOption=ivalue;
    break;
  case GI_GRID_LINE_COLOUR_OPTION:
      gridLineColourOption=ivalue;
    break;
  case GI_KEEP_ASPECT_RATIO:
    keepAspectRatio=ivalue;
    break;
  case GI_COMPUTE_COARSENING_FACTOR:
    computeCoarseningFactor=value;
    break;
  case GI_COLOUR_INTERPOLATION_POINTS:
    colourInterpolationPoints=ivalue;
    break;
  case GI_COLOUR_LINE_CONTOURS:
    colourLineContours=ivalue;
    break;
  case GI_CONTOUR3D_MIN_MAX_OPTION:
    contour3dMinMaxOption=ivalue;
    break;
  case GI_LABEL_COMPONENT:
    labelComponent=ivalue;
    break;
  case GI_LABEL_COLOUR_BAR:
    labelColourBar=ivalue;
    break;
  case GI_LABEL_GRIDS_AND_BOUNDARIES:
    labelGridsAndBoundaries=ivalue;
    break;
  case GI_LABEL_MIN_MAX:
    labelMinMax=ivalue;
    break;
  case GI_PLOT_BLOCK_BOUNDARIES:
    plotGridBlockBoundaries=ivalue;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotGridBlockBoundaries )
	gridOptions(grid) |=plotBlockBoundaries;
      else
	gridOptions(grid) &= !plotBlockBoundaries;
    }
    break;
  case GI_PLOT_COLOUR_BAR:
    plotColourBar=ivalue;
    break;
  case GI_PLOT_CONTOUR_LINES:
    plotContourLines=ivalue;
    break;
  case GI_PLOT_GRID_LINES:
    plotGridLines=ivalue;
    break;
  case GI_PLOT_LABELS:
    plotTitleLabels=ivalue;
    break;
  case GI_PLOT_GRID_BOUNDARIES_ON_CONTOUR_PLOTS:
    plotGridBoundariesOnContourPlots=ivalue;
    break;
  case GI_PLOT_LINES_ON_GRID_BOUNDARIES:
    plotLinesOnGridBoundaries=ivalue;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotLinesOnGridBoundaries )
	gridOptions(grid) |=plotBoundaryGridLines;
      else
	gridOptions(grid) &= !plotBoundaryGridLines;
    }
    break;
  case GI_PLOT_LINES_ON_MAPPING_BOUNDARIES:
    plotLinesOnMappingBoundaries=ivalue;
    break;
  case GI_PLOT_MAPPING_EDGES:
    plotMappingEdges=ivalue;
    break;
  case GI_PLOT_MAPPING_NORMALS:
    plotMappingNormals=ivalue;
    break;
  case GI_PLOT_GRID_POINTS_ON_CURVES:
    plotGridPointsOnCurves=ivalue;
    break;
  case GI_PLOT_END_POINTS_ON_CURVES:
    plotEndPointsOnCurves=ivalue;
    break;
  case GI_PLOT_HIDDEN_REFINEMENT_POINTS:
    plotHiddenRefinementPoints=ivalue;
    break;
  case GI_PLOT_INTERIOR_BOUNDARY_POINTS:
    plotInterpolationPoints=ivalue;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
      gridOptions(grid)^=GraphicsParameters::plotInteriorBoundary;
    break;
  case GI_PLOT_INTERPOLATION_POINTS:
    plotInterpolationPoints=ivalue;
    break;
  case GI_PLOT_INTERPOLATION_CELLS:
    plotInterpolationCells=ivalue;
    break;
  case GI_PLOT_NON_PHYSICAL_BOUNDARIES:
    plotNonPhysicalBoundaries=ivalue;
    break;
  case GI_PLOT_NURBS_CURVES_AS_SUBCURVES:
    plotNurbsCurvesAsSubCurves=ivalue;
    break;
  case GI_PLOT_SHADED_MAPPING_BOUNDARIES:
    plotShadedMappingBoundaries=ivalue;
    break;
  case GI_PLOT_SHADED_SURFACE:
    plotShadedSurface=ivalue;
    break;
  case GI_PLOT_SHADED_SURFACE_GRIDS:
    plotShadedSurfaceGrids=ivalue;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotShadedSurfaceGrids )
	gridOptions(grid) |=plotShadedSurfaces;
      else
	gridOptions(grid) &= !plotShadedSurfaces;
    }
    break;
  case GI_PLOT_THE_OBJECT:
    plotObject=ivalue;
    break;
  case GI_PLOT_THE_OBJECT_AND_EXIT:
    plotObjectAndExit=ivalue;
    break;
  case GI_PLOT_WIRE_FRAME:
    plotWireFrame=ivalue;
    break;
  case GI_PLOT_2D_CONTOURS_ON_COORDINATE_PLANES:
    plot2DContoursOnCoordinatePlanes=ivalue;
    break;
  case GI_PLOT_UNS_NODES:
    plotUnsNodes=ivalue;
    break;
  case GI_PLOT_UNS_FACES:
    plotUnsFaces=ivalue;
    break;
  case GI_PLOT_UNS_EDGES:
    plotUnsEdges=ivalue;
    break;
  case GI_PLOT_UNS_BOUNDARY_EDGES:
    plotUnsBoundaryEdges=ivalue;
    break;
  case GI_COLOUR_TABLE:
    colourTable=ColourTables(max(min(ivalue,numberOfColourTables),0));
    break;
  case GI_COMPONENT_FOR_SURFACE_CONTOURS:
    componentForSurfaceContours=ivalue;
    break;
  case GI_COMPONENT_FOR_CONTOURS:
    componentForContours=ivalue;
    if( componentsToPlot.getLength(0) > 0 )
      componentsToPlot(0)=ivalue;
    break;
  case GI_DISPLACEMENT_U_COMPONENT:
    displacementComponent[0]=ivalue;
    break;
  case GI_DISPLACEMENT_V_COMPONENT:
    displacementComponent[1]=ivalue;
    break;
  case GI_DISPLACEMENT_W_COMPONENT:
    displacementComponent[2]=ivalue;
    break;
  case GI_FLAT_SHADING:
    flatShading=ivalue;
    break;
  case GI_HARD_COPY_TYPE:
    hardCopyType=(HardCopyType)ivalue;
    break;
  case GI_MULTIGRID_LEVEL_TO_PLOT:
    multigridLevelToPlot=ivalue;
    break;
  case GI_NORMAL_AXIS_FOR_2D_CONTOURS_ON_COORDINATE_PLANES:
    normalAxisFor2DContoursOnCoordinatePlanes=ivalue;
    break;
  case GI_NUMBER_OF_CONTOUR_LEVELS:
    numberOfContourLevels=ivalue;
    break;
  case GI_NUMBER_OF_GHOST_LINES_TO_PLOT:
    numberOfGhostLinesToPlot=ivalue;
    break;
  case GI_OUTPUT_FORMAT:
    outputFormat=(OutputFormat)ivalue;
    break;
  case GI_POINT_SYMBOL:
    pointSymbol=ivalue;
    break;
  case GI_RASTER_RESOLUTION:
    rasterResolution=ivalue;
    break;
  case GI_PLOT_REFINEMENT_GRIDS:
    plotRefinementGrids=ivalue;
    break;
  case GI_REFINEMENT_LEVEL_TO_PLOT:
    refinementLevelToPlot=ivalue;
    break;
  case GI_U_COMPONENT_FOR_STREAM_LINES:
    uComponentForStreamLines=ivalue;
    break;
  case GI_V_COMPONENT_FOR_STREAM_LINES:
    vComponentForStreamLines=ivalue;
    break;
  case GI_W_COMPONENT_FOR_STREAM_LINES:
    wComponentForStreamLines=ivalue;
    break;
  case GI_USE_PLOT_BOUNDS:
    usePlotBounds=ivalue;
    break;
  case GI_USE_PLOT_BOUNDS_OR_LARGER:
    usePlotBoundsOrLarger=ivalue;  
    break;
  case GI_UNS_USE_CUT_PLANE:
    useUnsCutplane = ivalue;
    break;
  case GI_UNS_FLAT_SHADING:
    useUnsFlatShading = ivalue;
    break;

    // ---------------  options taking reals ------------------------------

  case GI_X_SCALE_FACTOR:
    xScaleFactor=value;
    break;
  case GI_Y_SCALE_FACTOR:
    yScaleFactor=value;
    break;
  case GI_Z_SCALE_FACTOR:
    zScaleFactor=value;
    break;
  case GI_DISPLACEMENT_SCALE_FACTOR:
    displacementScaleFactor=value;
    break;
  case GI_CONTOUR_SURFACE_VERTICAL_SCALE_FACTOR:
    contourSurfaceVerticalScaleFactor=value;
    break;
  case GI_CONTOUR_SURFACE_SPATIAL_BOUND:
    contourSurfaceSpatialBound=value;
    break;
  case GI_LINE_OFFSET:
    lineOffset=value;
    break;
  case GI_POINT_OFFSET:
    pointOffset=value;
    break;
  case GI_SURFACE_OFFSET:
    surfaceOffset=value;
    break;
  case GI_MINIMUM_CONTOUR_SPACING:
    minimumContourSpacing=value;
    break;
  case GI_POINT_SIZE:
    pointSize=value;
    break;
  case GI_STREAM_LINE_TOLERANCE:
    streamLineStoppingTolerance=value;
    break;
  case GI_STREAM_LINE_ARROW_SIZE:
    streamLineArrowSize=value;
    break;
  case GI_Y_LEVEL_FOR_1D_GRIDS:
    yLevelFor1DGrids=value;
    break;
  case GI_Z_LEVEL_FOR_2D_GRIDS:
    zLevelFor2DGrids=value;
    break;
  default :
    cout << "GraphicsParameters::set(real): ERROR: unknown option = " << option << endl;
    returnValue=1;
  }
  return returnValue;

}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{set(GraphicsOptions, IntegerArray)}} 
int GraphicsParameters::
set(const GraphicsOptions & option, const IntegerArray & values)
//----------------------------------------------------------------------
// /Description:
//   Assign a parameter with that requires an array of int's
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  int returnValue=0;
  switch (option)
  {
  case GI_CONTOUR_ON_GRID_FACE:
    plotContourOnGridFace=values;
    break;
  case GI_GRID_BOUNDARY_CONDITION_OPTIONS:
    gridBoundaryConditionOptions=values;
    break;
  case GI_GRID_OPTIONS:
    gridOptions=values;
    break;
  case GI_GRIDS_TO_PLOT:
    gridsToPlot=values;
    break;
  case GI_BACK_GROUND_GRID_FOR_STREAM_LINES:
    backGroundGridDimension=values;
    break;
  case GI_COMPONENTS_TO_PLOT:
    componentsToPlot=values;
    break;
  case GI_COORDINATE_PLANES:
    coordinatePlane.redim(0); // Will this work with ArraySimple???
    coordinatePlane=values;
    numberOfCoordinatePlanes=coordinatePlane.getLength(1);
    break;
  case GI_GRID_COORDINATE_PLANES:
    gridCoordinatePlane.redim(0); // Will this work with ArraySimple???
    gridCoordinatePlane=values;
    numberOfGridCoordinatePlanes=coordinatePlane.getLength(1);
    break;
  default :
    cout << "GraphicsParameters::set(IntegerArray): ERROR: unknown option = " << option << endl;
    returnValue=1;
  }
  return returnValue;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{set(GraphicsOptions, RealArray)}} 
int GraphicsParameters::
set(const GraphicsOptions & option, const RealArray & values)
//----------------------------------------------------------------------
// /Description:
//   Assign a parameter with that requires an array of real's
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  int returnValue=0;
  switch (option)
  {
  case GI_AXES_ORIGIN:
    axesOrigin=values;
    break;
  case GI_PLOT_BOUNDS:
    plotBound=values;
    break;
  case GI_ISO_SURFACE_VALUES:
    numberOfIsoSurfaces=values.getLength(0);
    isoSurfaceValue=values;
    break;
  case GI_MINIMUM_CONTOUR_SPACING:
    minAndMaxContourLevels=values;
    break;
  case GI_CONTOUR_LEVELS:
    if( values.getLength(0) > 0 )
    {
      numberOfContourLevels=values.getLength(0);
      contourLevels=values;
    }
    else
    {
      numberOfContourLevels=11; // default 
    }
    break;
  case GI_MIN_AND_MAX_CONTOUR_LEVELS:
    printF("GraphicsParameters::set:ERROR: Use the function GraphicsParameters::setMinAndMaxContourLevels\n"
           "     to set the min and max values for components\n");
    break;
  case GI_MIN_AND_MAX_STREAM_LINES:
    minStreamLine=values(0);
    maxStreamLine=values(1);
    minAndMaxStreamLinesSpecified= minStreamLine < maxStreamLine;
    break;
  case GI_UNS_CUT_PLANE_VERTEX:
    unsCutplaneVertex = values;
    break;
  case GI_UNS_CUT_PLANE_NORMAL:
    unsCutplaneNormal = values;
    break;
  default :
    cout << "GraphicsParameters::set(RealArray): ERROR: unknown option = " << option << endl;
    returnValue=1;
  }
  return returnValue;
}

// ===================================================================================
/// \brief Set the min and max contour levels for a given component. 
/// \param minValue (input) : minimum value for the contour level.
/// \param maxValue (input) : maximum value for the contour level.
/// \param component (input) : apply these values to this component.
/// \note To reset the min and max values to be chosen in the default way set minValue > maxValue. 
// ===================================================================================
int GraphicsParameters::
setMinAndMaxContourLevels( const real minValue, const real maxValue, const int component /* =0 */  )
{
  int oldSize = minAndMaxContourLevelsSpecified.getLength(0);
  if( component >= oldSize )
  {
    int newSize=component+10; // make space for extra components
    Range R(minAndMaxContourLevelsSpecified.getBound(0)+1,newSize-1);
    minAndMaxContourLevelsSpecified.resize(newSize);

    minAndMaxContourLevels.resize(2,newSize);
    minAndMaxContourLevelsSpecified(R)=false;   // give values to new entries
    minAndMaxContourLevels(Range(0,1),R)=0.;
  }
  if( minValue <= maxValue )
  { // the min and max for this 
    minAndMaxContourLevelsSpecified(component)=true;
    minAndMaxContourLevels(0,component)=minValue;
    minAndMaxContourLevels(1,component)=maxValue;
  }
  else
  { // reset to default choice for min and max
    minAndMaxContourLevelsSpecified(component)=false;
    minAndMaxContourLevels(0,component)=0.;
    minAndMaxContourLevels(1,component)=0.;
  }

  return 0;
}


//\begin{>>GraphicsParametersInclude.tex}{\subsection{set(GraphicsOptions, aString)}} 
int GraphicsParameters::
set(const GraphicsOptions & option, const aString & label)
//----------------------------------------------------------------------
// /Description:
//   Assign a parameter with a aString
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  int returnValue=0;
  switch (option)
  {
  case GI_BOTTOM_LABEL:
    bottomLabel=label;
    break;
  case GI_BOTTOM_LABEL_SUP_1:
    bottomLabel1=label;
    break;
  case GI_BOTTOM_LABEL_SUP_2:
    bottomLabel2=label;
    break;
  case GI_BOTTOM_LABEL_SUP_3:
    bottomLabel3=label;
    break;
  case GI_MAPPING_COLOUR:
    mappingColour=label;
    break;
  case GI_POINT_COLOUR:
    pointColour=label;
    break;
  case GI_LINE_COLOUR:
    lineColour=label;
    break;
  case GI_TOP_LABEL:
    topLabel=label;
    break;
  case GI_TOP_LABEL_SUB_1:
    topLabel1=label;
    break;
  case GI_TOP_LABEL_SUB_2:
    topLabel2=label;
    break;
  case GI_TOP_LABEL_SUB_3:
    topLabel3=label;
    break;
  default :
    cout << "GraphicsParameters::set(aString): ERROR: unknown option = " << option << endl;
    returnValue=1;
  }
  return returnValue;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{set(Sizes)}} 
int GraphicsParameters::
set(const Sizes & option, real value)  // set a size
//----------------------------------------------------------------------
// /Description:
//   Assign a {\it size} parameter
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  size(option)=value;
  return 0;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{setColourTable}} 
int GraphicsParameters::
setColourTable(ColourTableFunctionPointer ctf)
//----------------------------------------------------------------------
// /Description:
//   Provide a function to use for a colour table. This function will then be subsequently used for
//   the colour table. The colour table can be reset to one of the provided colour tables
//   using the  {\tt GI\_SET\_COLOUR\_TABLE} option. (The function provided here corresponds
//   to the {\tt userDefined} colour table).
// /ctf (input) : a pointer to a function of the form shown below. 
//
//  Here is an example of a function that defines a colour table
// 
// {\footnotesize
// \begin{verbatim}
// void 
// defaultColourTableFunction(const real & value, real & red, real & green, real & blue)
// // =============================================================================================
// // Description: Convert a value from [0,1] into (red,green,blue) values, each in the range [0,1]
// // value (input) :  0 <= value <= 1 
// // red, green, blue (output) : values in the range [0,1]
// // =============================================================================================
// { // a sample user defined colour table function: 
//   red=0.;
//   green=value;
//   blue=(1.-value);
// }
// \end{verbatim}
// }
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  if( ctf!=NULL )
  {
    colourTableFunction= ctf;
    colourTable=userDefined;
  }
  return 0;
}

