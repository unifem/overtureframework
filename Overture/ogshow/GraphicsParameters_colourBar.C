#include "GraphicsParameters.h"
#include "GenericGraphicsInterface.h"

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
  plotBoundsChanged=FALSE;
  relativeChangeForPlotBounds=.1;  // use with usePlotBoundsOrLarger

  colourTable=rainbow;
  plotBound.redim(2,3); plotBound=0.; plotBound(Start,0)=1; plotBound(End,0)=-1.;
  backGroundGridDimension.redim(3); backGroundGridDimension=0;
  topLabel="";
  topLabel1="";
  topLabel2="";
  topLabel3="";
  bottomLabel="";
  bottomLabel1="";
  bottomLabel2="";
  bottomLabel3="";
  plotTitleLabels=TRUE;   // plot topLabel etc.
  
  plotObject=TRUE;         // TRUE means we immediately plot the object on entering a plotting routine
  plotObjectAndExit=FALSE; // immediately plot the object then exit
  usePlotBounds=FALSE;               // use plot bounds found in the plotBound array
  usePlotBoundsOrLarger  =FALSE;      // plot bounds should include bounds from plotBound array
  mappingColour="red";
  lineOffset=0.;
  pointOffset=0.;
  surfaceOffset=3.;             // offset mappings 3 units behind grid lines
  plotShadedMappingBoundaries=TRUE;  // show shaded surfaces when plotting Mappings.
  plotMappingEdges=true;
  
  numberOfGhostLinesToPlot=0;
  labelGridsAndBoundaries=FALSE;
  plotInterpolationPoints=FALSE;
  plotInterpolationCells=TRUE;  
  plotBackupInterpolationPoints=FALSE;
  plotNonPhysicalBoundaries=FALSE;
  labelBoundaries        =FALSE;
  plotLinesOnMappingBoundaries=TRUE;
  plotLinesOnGridBoundaries=FALSE;
  plotGridLines=1+0*2;            // bit 1 for 2D , bit 2 for 3D
  plotGridBlockBoundaries=TRUE;
  plotShadedSurfaceGrids=FALSE;
  plotGridPointsOnCurves=TRUE;
  plotBranchCuts         =FALSE;

// unstructured stuff
  plotUnsNodes = FALSE;
  plotUnsFaces = TRUE;
  plotUnsEdges = FALSE;
  plotUnsBoundaryEdges = FALSE;

  boundaryColourOption=defaultColour;
  gridLineColourOption=defaultColour;
  blockBoundaryColourOption=colourByGrid;

  boundaryColourValue=0;       // colour number to use if boundaryColourOption==colourByValue
  gridLineColourValue=0;
  blockBoundaryColourValue=0;

  colourInterpolationPoints=FALSE;
  plotTheAxes            =TRUE;
  plotContourLines       =TRUE;
  plotShadedSurface      =TRUE;
  numberOfContourLevels  =11;
  plotWireFrame=FALSE;
  colourLineContours=FALSE;
  plotColourBar=TRUE;
  plotGridBoundariesOnContourPlots=TRUE;
  contourSurfaceVerticalScaleFactor=.75;
  
  numberOfContourPlanes=-1;

  uComponentForStreamLines=-999;
  vComponentForStreamLines=-999;
  wComponentForStreamLines=-999;
  componentForContours=0;
  componentForSurfaceContours=0;
  objectWasPlotted=TRUE;
  yLevelFor1DGrids=0.;
  zLevelFor2DGrids=0.;
  multigridLevelToPlot=0;
  refinementLevelToPlot=0;
  keepAspectRatio=true;

  outputFormat=colour8Bit;
  rasterResolution=0;
  hardCopyType=postScript;

  minAndMaxContourLevelsSpecified.redim(10); 
  minAndMaxContourLevelsSpecified=FALSE;
  
  minAndMaxContourLevels.redim(2,10);            // for user specified min and max contour levels
  minAndMaxContourLevels=0.;
  minimumContourSpacing=0.;
  axesOrigin.redim(3);
  axesOrigin=GenericGraphicsInterface::defaultOrigin;
  plotDashedLinesForNegativeContours=TRUE;

  plot2DContoursOnCoordinatePlanes=FALSE;
  normalAxisFor2DContoursOnCoordinatePlanes=2;
  numberOfIsoSurfaces=0;
  numberOfCoordinatePlanes=0;
  numberOfGridCoordinatePlanes=0;

  // for ColourBars: default is a vertical bar on the right
  //.. all lengths are in normalized units, where screen is [-1,1]
  //colourBarPosition                 = rightColourBar;
  colourBarPosition                 = useOldColourBar;
  colourBarWidth                    = 0.05;
  colourBarLength                   = 1.5;

  colourBarCenter.redim(2);
  colourBarCenter(0)                = 0.8; 
  colourBarCenter(1)                = 0.;
  
  colourBarAngle                    = 0.; //in degrees
  colourBarCurvature                = 0.; 
  colourBarOffsetFromPlot           = 0.;

  colourBarLabelOption              = colourBarLabelsOn;  //..colourbar labels
  colourBarLabelOnRight             = TRUE;
  colourBarLabelAngle               = 0.;
  colourBarRelativeAngle            = FALSE;
  colourBarLabelNormalOffset        = 0.05;
  colourBarLabelTangentialOffset    = 0.;
  colourBarNumberOfIntervals        = 50;
  colourBarMaximumNumberOfLabels    = 20;
  colourBarLabelScaling             = 0.75;
  colourBarThickLineInterval        = 5;
  
  // **** add these to setParameters ****
  linePlots=FALSE;                // plot solution on lines that intersect the range
  numberOfLines=0;
  numberOfPointsPerLine=0;

  minStreamLine=0.;
  maxStreamLine=0.;
  streamLineStoppingTolerance=1.e-3;
  minAndMaxStreamLinesSpecified=FALSE;

  numberOfStreamLineStartingPoints=0;

  // points:
  pointSize=3;  // size in pixels
  pointSymbol=0;
  pointColour="black";

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

  colourTableFunction=&defaultColourTableFunction;
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
//   Return TRUE if this object is a default object. 
// This routine can be used to tell whether a GraphicsParameter object is
// equal to the static object {\tt Overture::defaultGraphicsParameters()} which can
// be used as a default argument in a function call.
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  return defaultObject;
}


//\begin{>>GraphicsParametersInclude.tex}{\subsection{getObjectWasPlotted}} 
bool GraphicsParameters::
getObjectWasPlotted() const
//----------------------------------------------------------------------
// /Description:
//    Determine if the object was plotted in the last plotting routine
//    that was called.
// /Return value: TRUE if an object was plotted, FALSE otherwise.
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
  case GI_BLOCK_BOUNDARY_COLOUR_OPTION:
    value=blockBoundaryColourOption;
    break;
  case GI_BOUNDARY_COLOUR_OPTION:
    value=boundaryColourOption;
    break;
  case GI_COLOURBAR_POSITION:
    value=colourBarPosition;
    break;
  case GI_COLOURBAR_LABEL_OPTION:
    value=colourBarLabelOption;
    break;
  case GI_COLOURBAR_LABEL_ON_RIGHT:
    value=colourBarLabelOnRight;
    break;
  case GI_COLOURBAR_RELATIVE_ANGLE:
    value=colourBarRelativeAngle;
    break;
  case GI_COLOURBAR_NUMBER_OF_INTERVALS:
    value=colourBarNumberOfIntervals;
    break;
  case GI_COLOURBAR_MAXIMUM_NUMBER_OF_LABELS:
    value=colourBarMaximumNumberOfLabels;
    break;
  case GI_COLOURBAR_THICK_LINE_INTERVAL:
    value=colourBarThickLineInterval;
    break;
  case GI_COLOUR_INTERPOLATION_POINTS:
    value=colourInterpolationPoints;
    break;
  case GI_COLOUR_LINE_CONTOURS:
    value=colourLineContours;
    break;
  case GI_GRID_LINE_COLOUR_OPTION:
    value=gridLineColourOption;
    break;
  case GI_KEEP_ASPECT_RATIO:
    value=keepAspectRatio;
    break;
  case GI_LABEL_GRIDS_AND_BOUNDARIES:
    value=labelGridsAndBoundaries;
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
  case GI_PLOT_GRID_BOUNDARIES_ON_CONTOUR_PLOTS:
    value=plotGridBoundariesOnContourPlots;
    break;
  case GI_PLOT_LINES_ON_GRID_BOUNDARIES:
    value=plotLinesOnGridBoundaries;
    break;
  case GI_PLOT_GRID_POINTS_ON_CURVES:
    value=plotGridPointsOnCurves;
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
  case GI_PLOT_BACKUP_INTERPOLATION_POINTS:
    value=plotBackupInterpolationPoints;
    break;
  case GI_PLOT_SHADED_MAPPING_BOUNDARIES:
    value=plotShadedMappingBoundaries;
    break;
  case GI_PLOT_SHADED_SURFACE:
    value=plotShadedSurface;
    break;
  case GI_PLOT_SHADED_SURFACE_GRIDS:
    value=plotShadedSurfaceGrids;
    break;
  case GI_PLOT_THE_AXES:
    value=plotTheAxes;
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
  case GI_COLOURBAR_WIDTH:
    value=colourBarWidth;
    break;
  case GI_COLOURBAR_LENGTH:
    value=colourBarLength;
    break;
  case GI_COLOURBAR_ANGLE:
    value=colourBarAngle;
    break;
  case GI_COLOURBAR_CURVATURE:
    value=colourBarCurvature;
    break;
  case GI_COLOURBAR_OFFSET_FROM_PLOT:
    value=colourBarOffsetFromPlot;
    break;
  case GI_COLOURBAR_LABEL_ANGLE:
    value=colourBarLabelAngle;
    break;
  case GI_COLOURBAR_LABEL_NORMAL_OFFSET:
    value=colourBarLabelNormalOffset;
    break;
  case GI_COLOURBAR_LABEL_TANGENTIAL_OFFSET:
    value=colourBarLabelTangentialOffset;
    break;
  case GI_COLOURBAR_LABEL_SCALING:
    value=colourBarLabelScaling;
    break;
  case GI_CONTOUR_SURFACE_VERTICAL_SCALE_FACTOR:
    value=contourSurfaceVerticalScaleFactor;
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
  case GI_COLOURBAR_CENTER:
    values.redim(2);
    values(0)=colourBarCenter(0);
    values(1)=colourBarCenter(1);
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
  case GI_MIN_AND_MAX_STREAM_LINES:
    values(0)=minStreamLine;
    values(1)=maxStreamLine;
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
//   Determine the value of a {\it size} parameter
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



//\begin{>>GraphicsParametersInclude.tex}{\subsection{set(GraphicsOptions, int)}} 
int GraphicsParameters::
set(const GraphicsOptions & option, int value)
//----------------------------------------------------------------------
// /Description:
//   Assign a parameter with an int
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  int returnValue=0;
  int grid;
  switch (option)
  {
  case GI_PLOT_BACKUP_INTERPOLATION_POINTS:
    plotBackupInterpolationPoints=value;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotBackupInterpolationPoints )
	gridOptions(grid) |=plotBackupInterpolation;
      else
	gridOptions(grid) &= !plotBackupInterpolation;
    }
    break;
  case GI_BLOCK_BOUNDARY_COLOUR_OPTION:
      blockBoundaryColourOption=value;
    break;
  case GI_BOUNDARY_COLOUR_OPTION:
      boundaryColourOption=value;
    break;
  case GI_COLOURBAR_POSITION:
    colourBarPosition=(ColourBarPosition)value;
    break;
  case GI_COLOURBAR_LABEL_OPTION:
    colourBarLabelOption=(ColourBarLabelOption)value;
    break;
  case GI_COLOURBAR_LABEL_ON_RIGHT:
    colourBarLabelOnRight=(bool)value;
    break;
  case GI_COLOURBAR_RELATIVE_ANGLE:
    colourBarRelativeAngle=(bool)value;
    break;
  case GI_COLOURBAR_NUMBER_OF_INTERVALS:
    colourBarNumberOfIntervals=value;  
    break;
  case GI_COLOURBAR_MAXIMUM_NUMBER_OF_LABELS:
    colourBarMaximumNumberOfLabels = value; 
    break;
  case GI_COLOURBAR_THICK_LINE_INTERVAL:
    colourBarThickLineInterval = value;
    break;
  case GI_GRID_LINE_COLOUR_OPTION:
    gridLineColourOption=value;
    break;
  case GI_KEEP_ASPECT_RATIO:
    keepAspectRatio=value;
    break;
  case GI_COLOUR_INTERPOLATION_POINTS:
    colourInterpolationPoints=value;
    break;
  case GI_COLOUR_LINE_CONTOURS:
    colourLineContours=value;
    break;
  case GI_LABEL_GRIDS_AND_BOUNDARIES:
    labelGridsAndBoundaries=value;
    break;
  case GI_PLOT_BLOCK_BOUNDARIES:
    plotGridBlockBoundaries=value;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotGridBlockBoundaries )
	gridOptions(grid) |=plotBlockBoundaries;
      else
	gridOptions(grid) &= !plotBlockBoundaries;
    }
    break;
  case GI_PLOT_COLOUR_BAR:
    plotColourBar=value;
    break;
  case GI_PLOT_CONTOUR_LINES:
    plotContourLines=value;
    break;
  case GI_PLOT_GRID_LINES:
    plotGridLines=value;
    break;
  case GI_PLOT_LABELS:
    plotTitleLabels=value;
    break;
  case GI_PLOT_GRID_BOUNDARIES_ON_CONTOUR_PLOTS:
    plotGridBoundariesOnContourPlots=value;
    break;
  case GI_PLOT_LINES_ON_GRID_BOUNDARIES:
    plotLinesOnGridBoundaries=value;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotLinesOnGridBoundaries )
	gridOptions(grid) |=plotBoundaryGridLines;
      else
	gridOptions(grid) &= !plotBoundaryGridLines;
    }
    break;
  case GI_PLOT_LINES_ON_MAPPING_BOUNDARIES:
    plotLinesOnMappingBoundaries=value;
    break;
  case GI_PLOT_MAPPING_EDGES:
    plotMappingEdges=value;
    break;
  case GI_PLOT_GRID_POINTS_ON_CURVES:
    plotGridPointsOnCurves=value;
    break;
  case GI_PLOT_INTERPOLATION_POINTS:
    plotInterpolationPoints=value;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
      gridOptions(grid)^=GraphicsParameters::plotInterpolation;
    break;
  case GI_PLOT_INTERPOLATION_CELLS:
    plotInterpolationCells=value;
    break;
  case GI_PLOT_NON_PHYSICAL_BOUNDARIES:
    plotNonPhysicalBoundaries=value;
    break;
  case GI_PLOT_SHADED_MAPPING_BOUNDARIES:
    plotShadedMappingBoundaries=value;
    break;
  case GI_PLOT_SHADED_SURFACE:
    plotShadedSurface=value;
    break;
  case GI_PLOT_SHADED_SURFACE_GRIDS:
    plotShadedSurfaceGrids=value;
    for( grid=0; grid<gridOptions.getLength(0); grid++ )
    {
      if( plotShadedSurfaceGrids )
	gridOptions(grid) |=plotShadedSurfaces;
      else
	gridOptions(grid) &= !plotShadedSurfaces;
    }
    break;
  case GI_PLOT_THE_AXES:
    plotTheAxes=value;
    break;
  case GI_PLOT_THE_OBJECT:
    plotObject=value;
    break;
  case GI_PLOT_THE_OBJECT_AND_EXIT:
    plotObjectAndExit=value;
    break;
  case GI_PLOT_WIRE_FRAME:
    plotWireFrame=value;
    break;
  case GI_PLOT_2D_CONTOURS_ON_COORDINATE_PLANES:
    plot2DContoursOnCoordinatePlanes=value;
    break;
  case GI_PLOT_UNS_NODES:
    plotUnsNodes=value;
    break;
  case GI_PLOT_UNS_FACES:
    plotUnsFaces=value;
    break;
  case GI_PLOT_UNS_EDGES:
    plotUnsEdges=value;
    break;
  case GI_PLOT_UNS_BOUNDARY_EDGES:
    plotUnsBoundaryEdges=value;
    break;
  case GI_COLOUR_TABLE:
    colourTable=ColourTables(max(min(value,numberOfColourTables),0));
    break;
  case GI_COMPONENT_FOR_SURFACE_CONTOURS:
    componentForSurfaceContours=value;
    break;
  case GI_COMPONENT_FOR_CONTOURS:
    componentForContours=value;
    if( componentsToPlot.getLength(0) > 0 )
      componentsToPlot(0)=value;
    break;
  case GI_HARD_COPY_TYPE:
    hardCopyType=(HardCopyType)value;
    break;
  case GI_MULTIGRID_LEVEL_TO_PLOT:
    multigridLevelToPlot=value;
    break;
  case GI_NORMAL_AXIS_FOR_2D_CONTOURS_ON_COORDINATE_PLANES:
    normalAxisFor2DContoursOnCoordinatePlanes=value;
    break;
  case GI_NUMBER_OF_CONTOUR_LEVELS:
    numberOfContourLevels=value;
    break;
  case GI_NUMBER_OF_GHOST_LINES_TO_PLOT:
    numberOfGhostLinesToPlot=value;
    break;
  case GI_OUTPUT_FORMAT:
    outputFormat=(OutputFormat)value;
    break;
  case GI_POINT_SYMBOL:
    pointSymbol=value;
    break;
  case GI_RASTER_RESOLUTION:
    rasterResolution=value;
    break;
  case GI_REFINEMENT_LEVEL_TO_PLOT:
    refinementLevelToPlot=value;
    break;
  case GI_U_COMPONENT_FOR_STREAM_LINES:
    uComponentForStreamLines=value;
    break;
  case GI_V_COMPONENT_FOR_STREAM_LINES:
    vComponentForStreamLines=value;
    break;
  case GI_W_COMPONENT_FOR_STREAM_LINES:
    wComponentForStreamLines=value;
    break;
  case GI_USE_PLOT_BOUNDS:
    usePlotBounds=value;
    break;
  case GI_USE_PLOT_BOUNDS_OR_LARGER:
    usePlotBoundsOrLarger=value;  
    break;
  default :
    cout << "GraphicsParameters::set(int): ERROR: unknown option = " << option << endl;
    returnValue=1;
  }
  return returnValue;
}

//\begin{>>GraphicsParametersInclude.tex}{\subsection{set(GraphicsOptions, real)}} 
int GraphicsParameters:: 
set(const GraphicsOptions & option, real value)
//----------------------------------------------------------------------
// /Description:
//   Assign a parameter with a real value
//
//\end{GraphicsParametersInclude.tex} 
//----------------------------------------------------------------------
{
  int returnValue=0;
  switch (option)
  {
  case GI_COLOURBAR_WIDTH:
    colourBarWidth =value;
    break;
  case GI_COLOURBAR_LENGTH:
    colourBarLength =value;
    break;
  case GI_COLOURBAR_ANGLE:
    colourBarAngle =value;
    break;
  case GI_COLOURBAR_CURVATURE:
    colourBarCurvature =value;
    break;
  case GI_COLOURBAR_OFFSET_FROM_PLOT:
    colourBarOffsetFromPlot =value;
    break;
  case GI_COLOURBAR_LABEL_ANGLE:
    colourBarLabelAngle =value;
    break;
  case GI_COLOURBAR_LABEL_NORMAL_OFFSET:
    colourBarLabelNormalOffset =value;
    break;
  case GI_COLOURBAR_LABEL_TANGENTIAL_OFFSET:
    colourBarLabelTangentialOffset =value;
    break;
  case GI_COLOURBAR_LABEL_SCALING:
    colourBarLabelScaling =value;  
    break;
  case GI_CONTOUR_SURFACE_VERTICAL_SCALE_FACTOR:
    contourSurfaceVerticalScaleFactor=value;
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
    coordinatePlane.redim(0);
    coordinatePlane=values;
    numberOfCoordinatePlanes=coordinatePlane.getLength(1);
    break;
  case GI_GRID_COORDINATE_PLANES:
    gridCoordinatePlane.redim(0);
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
  case GI_COLOURBAR_CENTER:
    colourBarCenter.redim(2);
    colourBarCenter(0)=values(0);
    colourBarCenter(1)=values(1);
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
  case GI_MIN_AND_MAX_STREAM_LINES:
    minStreamLine=values(0);
    maxStreamLine=values(1);
    minAndMaxStreamLinesSpecified= minStreamLine < maxStreamLine;
    break;
  default :
    cout << "GraphicsParameters::set(RealArray): ERROR: unknown option = " << option << endl;
    returnValue=1;
  }
  return returnValue;
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

