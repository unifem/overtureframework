#ifndef GRAPHICS_PARAMETERS_H
#define GRAPHICS_PARAMETERS_H

#ifndef NO_APP
#include "GenericDataBase.h"
#endif
#include "GUITypes.h"

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

//#include "aString.H"
#ifndef NO_APP
#include "aString.H"
#else
#include <string>
#ifndef aString
#define aString std::string
#endif
#endif

class GenericGraphicsInterface;  // forward declaration
class GL_GraphicsInterface;  // forward declaration
class MappingInformation; // forward declaration
class GraphicsParameters;

extern int 
viewMappings( MappingInformation & mapInfo );
extern int
plotAListOfMappings(MappingInformation & mapInfo,    // get rid of this eventually
		    const int & mapNumberToPlot,
		    int & numberOfMapsPlotted,   
		    IntegerArray & listOfMapsToPlot, 
		    aString *localColourNames,
		    const int & numberOfColourNames,
		    const bool & plotTheAxes,
		    GraphicsParameters & params );
extern int 
readMappings( MappingInformation & mapInfo ); // get rid of this eventually

// Here is the form of a user defined colour table
typedef void (*ColourTableFunctionPointer)(const GUITypes::real & value, GUITypes::real & red, GUITypes::real & green, GUITypes::real & blue);
// /Description: Convert a value from [0,1] into (red,green,blue) values, each in the range [0,1]
// /value (input) :  0 <= value <= 1 
// /red, green, blue (output) : values in the range [0,1]

// NOTE: remember to change the documentation in GraphicsDoc.tex when you change this next table
enum GraphicsOptions
{
  GI_ADJUST_GRID_FOR_DISPLACEMENT,
  GI_AXES_ORIGIN,  
  GI_BOUNDARY_COLOUR_OPTION,
  GI_BACK_GROUND_GRID_FOR_STREAM_LINES,
  GI_BLOCK_BOUNDARY_COLOUR_OPTION,
  GI_BOTTOM_LABEL,
  GI_BOTTOM_LABEL_SUP_1,
  GI_BOTTOM_LABEL_SUP_2,
  GI_BOTTOM_LABEL_SUP_3,
  GI_COLOUR_INTERPOLATION_POINTS,
  GI_COLOUR_LINE_CONTOURS,
  GI_COMPUTE_COARSENING_FACTOR,
  GI_CONTOUR_ON_GRID_FACE,
  GI_CONTOUR_SURFACE_VERTICAL_SCALE_FACTOR,
  GI_CONTOUR_SURFACE_SPATIAL_BOUND,
  GI_CONTOUR3D_MIN_MAX_OPTION,
  GI_COLOUR_TABLE,
  GI_COMPONENT_FOR_CONTOURS,
  GI_COMPONENT_FOR_SURFACE_CONTOURS,
  GI_COMPONENTS_TO_PLOT,
  GI_CONTOUR_LEVELS,
  GI_COORDINATE_PLANES,
  GI_DISPLACEMENT_SCALE_FACTOR,
  GI_DISPLACEMENT_U_COMPONENT,
  GI_DISPLACEMENT_V_COMPONENT,
  GI_DISPLACEMENT_W_COMPONENT,
  GI_FLAT_SHADING,
  GI_GRID_COORDINATE_PLANES,
  GI_GRID_BOUNDARY_CONDITION_OPTIONS,
  GI_GRID_OPTIONS,
  GI_GRID_LINES,
  GI_GRID_LINE_COLOUR_OPTION,
  GI_GRIDS_TO_PLOT,
  GI_HARD_COPY_TYPE,
  GI_ISO_SURFACE_VALUES,
  GI_KEEP_ASPECT_RATIO,
  GI_LABEL_COMPONENT,
  GI_LABEL_COLOUR_BAR,
  GI_LABEL_GRIDS_AND_BOUNDARIES,
  GI_LABEL_MIN_MAX,
  GI_LINE_OFFSET,
  GI_LINE_COLOUR,
  GI_MAPPING_COLOUR,
  GI_MINIMUM_CONTOUR_SPACING,
  GI_MIN_AND_MAX_CONTOUR_LEVELS,
  GI_MIN_AND_MAX_STREAM_LINES,
  GI_MULTIGRID_LEVEL_TO_PLOT,
  GI_NUMBER_OF_CONTOUR_LEVELS,
  GI_NUMBER_OF_GHOST_LINES_TO_PLOT,
  GI_OUTPUT_FORMAT,
  GI_PLOT_BOUNDS,
  GI_PLOT_BACK_GROUND_GRID,
  GI_PLOT_BACKUP_INTERPOLATION_POINTS,
  GI_PLOT_BLOCK_BOUNDARIES,
  GI_PLOT_COLOUR_BAR,
  GI_PLOT_CONTOUR_LINES,
  GI_PLOT_LABELS,
  GI_PLOT_LINES_ON_GRID_BOUNDARIES,
  GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,
  GI_PLOT_MAPPING_EDGES,
  GI_PLOT_MAPPING_NORMALS,
  GI_PLOT_GRID_BOUNDARIES_ON_CONTOUR_PLOTS,
  GI_PLOT_GRID_LINES,
  GI_PLOT_GRID_POINTS_ON_CURVES,
  GI_PLOT_END_POINTS_ON_CURVES,
  GI_PLOT_HIDDEN_REFINEMENT_POINTS,
  GI_PLOT_INTERIOR_BOUNDARY_POINTS,
  GI_PLOT_INTERPOLATION_POINTS,
  GI_PLOT_INTERPOLATION_CELLS,
  GI_PLOT_NON_PHYSICAL_BOUNDARIES,
  GI_PLOT_NURBS_CURVES_AS_SUBCURVES,
  GI_PLOT_REFINEMENT_GRIDS,
  GI_PLOT_SHADED_MAPPING_BOUNDARIES,
  GI_PLOT_SHADED_SURFACE,
  GI_PLOT_SHADED_SURFACE_GRIDS,
  GI_PLOT_THE_OBJECT,
  GI_PLOT_THE_OBJECT_AND_EXIT,
  GI_PLOT_WIRE_FRAME,
  GI_POINT_COLOUR,
  GI_POINT_OFFSET,
  GI_POINT_SIZE,
  GI_POINT_SYMBOL,
  GI_RASTER_RESOLUTION,
  GI_REFINEMENT_LEVEL_TO_PLOT,
  GI_SURFACE_OFFSET,
  GI_STREAM_LINE_TOLERANCE,
  GI_STREAM_LINE_ARROW_SIZE,
  GI_U_COMPONENT_FOR_STREAM_LINES,
  GI_V_COMPONENT_FOR_STREAM_LINES,
  GI_W_COMPONENT_FOR_STREAM_LINES,
  GI_TOP_LABEL,
  GI_TOP_LABEL_SUB_1,
  GI_TOP_LABEL_SUB_2,
  GI_TOP_LABEL_SUB_3,
  GI_USE_PLOT_BOUNDS,
  GI_USE_PLOT_BOUNDS_OR_LARGER,
  GI_PLOT_2D_CONTOURS_ON_COORDINATE_PLANES,
  GI_NORMAL_AXIS_FOR_2D_CONTOURS_ON_COORDINATE_PLANES,
  GI_X_SCALE_FACTOR,
  GI_Y_SCALE_FACTOR,
  GI_Z_SCALE_FACTOR,
  GI_Y_LEVEL_FOR_1D_GRIDS,
  GI_Z_LEVEL_FOR_2D_GRIDS,
  GI_PLOT_UNS_NODES,
  GI_PLOT_UNS_FACES,
  GI_PLOT_UNS_EDGES,
  GI_PLOT_UNS_BOUNDARY_EDGES,
  GI_UNS_USE_CUT_PLANE,
  GI_UNS_CUT_PLANE_NORMAL,
  GI_UNS_CUT_PLANE_VERTEX,
  GI_UNS_FLAT_SHADING
};

class GraphicsParameters
{
public:

enum ColourTables
{
  rainbow,
  gray,
  red,
  green,
  blue,
  userDefined,
  numberOfColourTables
} colourTable;

enum HardCopyType
{
  postScript,       // post script file with drawing commands
  encapsulatedPostScript,
  postScriptRaster, // post script containing a bit map image of the screen
  ppm               // portable pixmap format (P6 binary format)
};


enum OutputFormat   // formats for outputing postscript files
{
  colour8Bit,       // compressed colour file with 225 colours
  colour24Bit,      // 24 bits of colour (2^24 colours)
  blackAndWhite,    // black and white
  grayScale         // 8 bit gray scale (2^8 shades of gray)
};

enum Sizes // **** remember to change the version of this list in GenericGraphicsInterface
{
  lineWidth,       // basic width in pixels for a line
  axisNumberSize,  // size of number labels on the axes
  axisLabelSize,   // size of axis label ("x-axis")
  axisMinorTickSize,
  axisMajorTickSize,
  topLabelSize,     
  topSubLabelSize,    
  bottomLabelSize,
  bottomSubLabelSize,
  minorContourWidth,  // this times lineWidth = actual width in pixels for minor contour lines
  majorContourWidth,  // this times lineWidth = actual width in pixels for major contour lines
  streamLineWidth,    // this times lineWidth = actual width in pixels for stream lines
  labelLineWidth,     // this times lineWidth = actual width in pixels for lines that make up a character
  curveLineWidth,     // this times lineWidth = actual width in pixels for curves drawn
  extraSize2,
  extraSize3,
  extraSize4,
  numberOfSizes    // counts number of entries in this list
};
  
enum GridOptions   // bit-map values for grid plotting options: gridOption(grid)
{
  plotGrid=1,
  plotInterpolation           = plotGrid                   << 1,
  plotShadedSurfaces          = plotInterpolation          << 1,
  plotInteriorGridLines       = plotShadedSurfaces         << 1,
  plotBoundaryGridLines       = plotInteriorGridLines      << 1,
  plotBlockBoundaries         = plotBoundaryGridLines      << 1,
  plotBackupInterpolation     = plotBlockBoundaries        << 1,
  plotInteriorBoundary        = plotBackupInterpolation    << 1,
  doNotPlotFace00             = plotInteriorBoundary       << 1,
  doNotPlotFace10             = doNotPlotFace00            << 1,
  doNotPlotFace01             = doNotPlotFace10            << 1,
  doNotPlotFace11             = doNotPlotFace01            << 1,
  doNotPlotFace02             = doNotPlotFace11            << 1,
  doNotPlotFace12             = doNotPlotFace02            << 1,
  doNotPlotGridLinesOnFace00  = doNotPlotFace12            << 1,
  doNotPlotGridLinesOnFace10  = doNotPlotGridLinesOnFace00 << 1,
  doNotPlotGridLinesOnFace01  = doNotPlotGridLinesOnFace10 << 1,
  doNotPlotGridLinesOnFace11  = doNotPlotGridLinesOnFace01 << 1,
  doNotPlotGridLinesOnFace02  = doNotPlotGridLinesOnFace11 << 1,
  doNotPlotGridLinesOnFace12  = doNotPlotGridLinesOnFace02 << 1
};

enum ColourOptions  // options for colouring boundaries, grids lines and block boundaries
{ 
  defaultColour,   // default 
  colourByGrid,
  colourByRefinementLevel,
  colourByBoundaryCondition,
  colourByShare,
  colourByValue,
  colourBlack,
  colourByIndex,
  colourByDomain
};

// Here are the options for setting the colour bar min/max when plotting 3d contour/coordinate planes
enum Contour3dMinMaxEnum
{
  baseMinMaxOnGlobalValues,
  baseMinMaxOnContourPlaneValues
};
  

GraphicsParameters(bool default0=FALSE); 

~GraphicsParameters(); 

bool 
isDefault();

// use these functions to determine current values for parameters
aString &       
get(const GraphicsOptions & option, aString & label) const;
int &          
get(const GraphicsOptions & option, int & value) const;  
GUITypes::real &         
get(const GraphicsOptions & option, GUITypes::real & value) const;  
IntegerArray & 
get(const GraphicsOptions & option, IntegerArray & values) const;
RealArray &    
get(const GraphicsOptions & option, RealArray & values) const;
GUITypes::real &         
get(const Sizes & option, GUITypes::real & value) const;  

// use these set functions to set a value for a GraphicsOptions parameter
int 
set(const GraphicsOptions & option, const aString & label);
// int 
// set(const GraphicsOptions & option, int value);  
int 
set(const GraphicsOptions & option, GUITypes::real value);  
int 
set(const GraphicsOptions & option, const IntegerArray & values);
int 
set(const GraphicsOptions & option, const RealArray & values);
int 
set(const Sizes & option, GUITypes::real value);  // set a size

int 
setColourTable(ColourTableFunctionPointer ctf); // provide a function to use for a colour table

int
setMinAndMaxContourLevels( const real minValue, const real maxValue, const int component=0 );

//access routines
int
getObjectWasPlotted() const;  // true if an object was plotted

friend
class GenericGraphicsInterface;  
friend 
class GL_GraphicsInterface;

// AP testing
friend 
class PlotIt;

friend int 
viewMappings( MappingInformation & mapInfo );
friend int
plotAListOfMappings(MappingInformation & mapInfo,    // get rid of this eventually
		    const int & mapNumberToPlot,
		    int & numberOfMapsPlotted,   
		    IntegerArray & listOfMapsToPlot, 
		    aString *localColourNames,
		    const int & numberOfColourNames,
		    const bool & plotTheAxes,
		    GraphicsParameters & params );
friend int 
readMappings( MappingInformation & mapInfo ); // get rid of this eventually
  
// some access functions
bool & 
getLabelGridsAndBoundaries(){ return labelGridsAndBoundaries; }
int  & 
getBoundaryColourOption(){ return boundaryColourOption; }
bool & 
getPlotShadedSurface(){ return plotShadedSurface; }
bool & 
getPlotLinesOnMappingBoundaries(){ return plotLinesOnMappingBoundaries; }
bool & 
getPlotNonPhysicalBoundaries(){ return plotNonPhysicalBoundaries; }
bool & 
getPlotGridPointsOnCurves(){ return plotGridPointsOnCurves; }
int  & 
getNumberOfGhostLinesToPlot(){ return numberOfGhostLinesToPlot; }
aString & 
getMappingColour(){ return mappingColour; }



// protected:   // should be protected but GL_GraphicsInterface needs these

enum ToggledItems  // label bit flags used by gridsToPlot
{
  toggleGrids=1,
  toggleContours=2,
  toggleStreamLines=4,
  toggleSum=1+2+4
};

// parameters used generally:
aString topLabel,topLabel1,topLabel2,topLabel3;
aString bottomLabel,bottomLabel1,bottomLabel2,bottomLabel3;
bool plotObject;                  // immediately plot the object
bool plotObjectAndExit;           // immediately plot the object then exit
bool usePlotBounds;               // use plot bounds found in the plotBound array
bool usePlotBoundsOrLarger;      // plot bounds should include bounds from plotBound array
bool plotTheAxes;
RealArray plotBound;              // plotting bounds: plotBound(2,3)
RealArray size;                   // holds sizes for items in the Sizes enum
int objectWasPlotted;            // true on exit if object was plotted (and not erased)
int numberOfGhostLinesToPlot;
bool plotBoundsChanged;  // true if the plot bounds were changed
GUITypes::real relativeChangeForPlotBounds;  // use with usePlotBoundsOrLarger
int multigridLevelToPlot;
int refinementLevelToPlot;
bool plotRefinementGrids;
bool keepAspectRatio;            // default true. Keep the aspect ratio.
bool computeCoarseningFactor;

HardCopyType hardCopyType;
OutputFormat outputFormat;
int rasterResolution;       // resolution for off screen renderer
  
bool plotTitleLabels;     // plot the top title etc. AP: NEEDS ACCESS FUNCTION!!!
bool labelColourBar;
int labelMinMax;          //  o=no-label, 1=set-label, 2=add-to-label
bool labelComponent;      // include the component name on the top label (e.g. for contour)

// parameters used to plot Mappings
aString mappingColour;
bool plotGridPointsOnCurves;
bool plotEndPointsOnCurves;
bool plotLinesOnMappingBoundaries;
bool plotNonPhysicalBoundaries;
bool plotShadedMappingBoundaries;
bool plotMappingEdges;
bool plotMappingNormals;
bool plotNurbsCurvesAsSubCurves;

GUITypes::real lineOffset, pointOffset, surfaceOffset;      // shift (lines/points/polygons) this many "units" before plotting

// parameters for plotting grids
IntegerArray gridsToPlot;                     // bit flag: 1=plot grid, 2=plot contours, 4=plot streamlines.
IntegerArray gridOptions;                     // bit flag holding various options
IntegerArray gridBoundaryConditionOptions;    // another bit flag
IntegerArray gridColours;                     // colours of grids (index into xColours array)

bool colourInterpolationPoints;
bool plotInterpolationPoints;
bool plotInterpolationCells;  
bool plotBackupInterpolationPoints;
bool plotBranchCuts;
bool plotLinesOnGridBoundaries;
int plotGridLines;               // bit 1 for 2d, bit 2 for 3d
bool plotGridBlockBoundaries;
bool plotShadedSurfaceGrids;
bool plotUnsNodes, plotUnsFaces, plotUnsEdges, plotUnsBoundaryEdges;
bool labelBoundaries;
bool plotInteriorBoundaryPoints;
bool plotHiddenRefinementPoints;

int boundaryColourOption;      // 0=default, 1=by grid, 2=by refinement level, 3=BC, 4=by share
int gridLineColourOption;      // 0=default, 1=by grid, 2=by refinement level
int blockBoundaryColourOption; // 0=default, 1=by grid, 2=by refinement level, 3=BC, 4=by share
  
int boundaryColourValue;
int gridLineColourValue;
int blockBoundaryColourValue;
  
//  bool colourBoundariesByBoundaryConditions;
//  bool colourGridByRefinementLevel;
//  bool colourBoundariesByShare;
//  bool colourBlockBoundaries;
GUITypes::real yLevelFor1DGrids;  // for 1d grid plots.
GUITypes::real zLevelFor2DGrids;
bool labelGridsAndBoundaries;
  
static GUITypes::real xScaleFactor, yScaleFactor, zScaleFactor;  // for scaling plots

// These are for the contour plotter:
bool plotContourLines;
bool plotShadedSurface;
int numberOfContourLevels;
bool plotWireFrame;
bool colourLineContours;
bool plotColourBar;
int componentForContours;
int componentForSurfaceContours;  // component for 3D contours on grid boundaries
IntegerArray componentsToPlot;  // for 1d contour plots, multiple components
IntegerArray minAndMaxContourLevelsSpecified;
RealArray minAndMaxContourLevels;            // for user specified min and max contour levels
RealArray contourLevels;
RealArray axesOrigin;  // AP: Can this be removed ?
GUITypes::real minimumContourSpacing;
bool plotDashedLinesForNegativeContours; 
bool plotGridBoundariesOnContourPlots;
GUITypes::real contourSurfaceVerticalScaleFactor;
GUITypes::real contourSurfaceSpatialBound; // by default use bounds from the grid
bool flatShading;
  
bool linePlots;                // plot solution on lines that intersect the range
int numberOfLines;
int numberOfPointsPerLine;
int numberOfContourPlanes;
RealArray linePlotEndPoints;
RealArray contourPlane;          //  contourPlane(0:5,number)   0:2 = normal, 3:5 = point on plane
IntegerArray plotContourOnGridFace;
RealArray isoSurfaceValue;
IntegerArray coordinatePlane;
IntegerArray gridCoordinatePlane;
bool plot2DContoursOnCoordinatePlanes;
int normalAxisFor2DContoursOnCoordinatePlanes;
int numberOfIsoSurfaces;
int numberOfCoordinatePlanes;
int numberOfGridCoordinatePlanes;
int contour3dMinMaxOption;

// These are for streamlines
IntegerArray backGroundGridDimension;   // number of points on back ground grid backGroundGridDimension(3)
int uComponentForStreamLines;
int vComponentForStreamLines;
int wComponentForStreamLines;
GUITypes::real minStreamLine, maxStreamLine;   // determines how colours appear
GUITypes::real streamLineStoppingTolerance;    // stop drawing when velocity decreases by this much
bool minAndMaxStreamLinesSpecified;
GUITypes::real streamLineArrowSize;
  
// for 3d stream lines
int numberOfStreamLineStartingPoints;
realArray streamLineStartingPoint;
IntegerArray numberOfRakePoints;
realArray rakeEndPoint;
IntegerArray numberOfRectanglePoints;
realArray rectanglePoint;

// for plotting points:
GUITypes::real pointSize;
int pointSymbol;
aString pointColour, lineColour;

// for unstructured mappings
bool useUnsCutplane, useUnsFlatShading;
RealArray unsCutplaneVertex; 
RealArray unsCutplaneNormal;

// for plotting the displacement
int adjustGridForDisplacement;  // plot contours etc. with grid + displacement instead of the grid 
GUITypes::real displacementScaleFactor;
int displacementComponent[3];   // (u,v,w) component of the displacement are at these component indicies.

bool defaultObject;   

ColourTableFunctionPointer colourTableFunction;

public:
// Add these for userDefinedOutput functions that are called from contour functions for e.g.
void *showFileReader;  // pointer to an active ShowFileReader (if any)
int showFileSolutionNumber;  // current solution number in the show file.
void *showFileParameters; // pointer to an active ListOfShowFileParameters (if any)

// Here is the new place to store parameters
DataBase dbase;

};

#endif
