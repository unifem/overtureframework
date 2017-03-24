#ifndef GENERIC_GRAPHICS_INTERFACE_H
#define GENERIC_GRAPHICS_INTERFACE_H

//============================================================================================
//     -----------------------------------------
//     ------Generic Graphics Interface---------
//     -----------------------------------------
//
//   This is the generic graphics interface. It supports menus, command files and
//   some generic plotting. This Class does not know how to do any plotting. The
//   derived Class GL_GraphicsInterface does know how to plot.
// 
//===========================================================================================

#ifndef NO_APP
#include "GenericDataBase.h"
#endif

#include "GUITypes.h"

#include "wdhdefs.h"

#include "mathutil.h"          // define max, min,  etc

#ifndef NO_APP
#include "OvertureInit.h"
#else
#include "GUIInit.h"
#endif

#include "GraphicsParameters.h"

#include <assert.h>

#ifndef NO_APP
#ifndef OV_USE_OLD_STL_HEADERS
#include <stack>
#include <queue>
OV_USINGNAMESPACE(std);
#else
#include <stack.h>
#include <queue.h>
#endif
#else
#include <stack>
#include <queue>
#endif

// extern GraphicsParameters Overture::defaultGraphicsParameters();  // use these as a default argument
extern const aString nullString;

// forward declarations
class CompositeSurface;
class UnstructuredMapping;
class AdvancingFront;  

class MappedGrid;
class floatMappedGridFunction;
class doubleMappedGridFunction;

class GridCollection;
class floatGridCollectionFunction;
class doubleGridCollectionFunction;

class OvertureParser;

#ifdef OV_USE_DOUBLE
  typedef doubleMappedGridFunction realMappedGridFunction;
  typedef doubleGridCollectionFunction realGridCollectionFunction;
#else
  typedef floatMappedGridFunction realMappedGridFunction;
  typedef floatGridCollectionFunction realGridCollectionFunction;
#endif

#include "GUIState.h"
//  class SelectionInfo;
//  class PickInfo3D;
//  class GUIState;

class ViewLocation
{
public:
// data member are public for now...  
GUITypes::real shift[3], magnificationFactor, globalBound[2][3], shiftCorrection[3];
GUITypes::real rotationCenter[3], rotationMatrix[4][4];
bool userDefinedRotationPoint;
};

class GenericGraphicsInterface
{
public:
  
enum HardCopyRenderingEnum
{
  offScreenRender,      // better but may not be supported when OpenGl does direct rendering to the hardware
  frameBuffer     // grab the frame buffer, resolution is determined by the window size
};


enum
{
  defaultOrigin=-(INT_MAX/2)
};

enum ItemColourEnum
{
  backGroundColour=0,
  textColour,
  numberOfItemColours
};

enum
{
  numberOfColourNames=25
};

// Set some view parameters
enum ViewParameters
{
  xAxisAngle,      // angle to rotate about x-axis (absolute value, not incremental)
  yAxisAngle,
  zAxisAngle,
  xTranslation,
  yTranslation,
  zTranslation,
  magnification
};

enum displayListProperty{ // bitwise flags for handling display lists.
  lightDL = 1, // if this bit is set, the display list should be plotted with lighting, otherwise not.
  plotDL  = 2, // if this bit is set, the display list should be plotted, otherwise not
  hideableDL = 4,  // if this bit is set, the display list can be hidden (i.e., not plotted) instead of
// getting erased.
  interactiveDL = 8 // if set, this display list will be drawn during interactive rotations
};

//
// Default constructor
//
GenericGraphicsInterface();
//
// Constructor that takes argc and argv from main program and strips away GLUT parameters
//
GenericGraphicsInterface(int & argc, char *argv[]);

virtual 
~GenericGraphicsInterface();
  
// abort the program if we stop reading a command file.
void
abortIfCommandFileEnds(bool trueOrFalse=true);

virtual void
appendCommandHistory(const aString &answer)=0;

// make a new default prompt by appending to the current, and push onto the stack
int 
appendToTheDefaultPrompt(const aString & appendage );

virtual int
beginRecordDisplayLists( IntegerArray & displayLists)=0;


// add cascading entries to a long menu.
int 
buildCascadingMenu( aString *&menu,
		    int startCascade, 
		    int endCascade ) const ;
// choose a colour
virtual aString 
chooseAColour()=0;

virtual void
createMessageDialog(aString msg, MessageTypeEnum type)=0;

// Open the window (but only if one is not already open)
virtual int 
createWindow(const aString & windowTitle = nullString,
	     int argc=0, 
	     char *argv[] = NULL )=0;

virtual void
deleteList(int dList)=0;

// destroy the window 
virtual int
destroyWindow(int win_number)=0;

virtual void 
displayColourBar(const int & numberOfContourLevels,
		 RealArray & contourLevels,
		 GUITypes::real uMin,
		 GUITypes::real uMax,
		 GraphicsParameters & parameters)=0;

// display help on a topic in the help pull-down  menu
virtual bool
displayHelp( const aString & topic )=0;

// Draw coloured squares with a number inside them to label colours on the plot
virtual void
drawColouredSquares(const IntegerArray & numberList,
		    GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
		    const int & numberOfColourNames = -1 , // use default colours by default
		    aString *colourNames = NULL)=0;


virtual void
drawColourBar(const int & numberOfContourLevels,
	      RealArray & contourLevels,
	      GUITypes::real uMin=0., 
	      GUITypes::real uMax=1.,
	      GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	      GUITypes::real xLeft=.775,  // .8
	      GUITypes::real xRight=.825, // .85
	      GUITypes::real yBottom=-.75,
	      GUITypes::real yTop=.75)=0;


virtual int
endRecordDisplayLists( IntegerArray & displayLists)=0;

// Erase all graphics display lists
virtual void 
erase()=0;

// erases all display lists in window win_number.
virtual void 
erase(const int win_number, bool forceDelete = false)=0;

// erase a list of display lists
virtual void
erase(const IntegerArray & displayList)=0;

virtual void
eraseColourBar()=0;

virtual void
eraseLabels(GraphicsParameters & parameters, int win_number = -1)=0; 

virtual int
generateNewDisplayList(bool lit = false, bool plotIt = true, bool erasable = false,
		       bool interactive = true)=0;
  
virtual int
getMenuItem(const aString *menu, aString & answer, const aString & prompt=nullString)=0;

virtual int
getAnswer(aString & answer, const aString & prompt)=0;

virtual int
getAnswer(aString & answer, const aString & prompt,
	  SelectionInfo &selection)=0;

virtual int
getAnswerNoBlock(aString & answer, const aString & prompt)=0;

// return the aspect ratio of a window
virtual GUITypes::real
getAspectRatio(const int win=0)=0;

virtual void
getAxesOrigin( GUITypes::real & x0, GUITypes::real & y0, GUITypes::real & z0 )=0;

// Set the info level which determines the level of information that is output. 
// 0=expert, 1=intermediate, 2=novice
int
getInfoLevel() const;

// access functions for display lists
virtual int
getFirstDL(const int win)=0;

virtual int
getFirstFixedDL(const int win)=0;

virtual int
getTopLabelDL(const int win)=0;

virtual int
getTopLabel1DL(const int win)=0;

virtual int
getTopLabel2DL(const int win)=0;

virtual int 
getTopLabel3DL(const int win)=0;

virtual int 
getBottomLabelDL(const int win)=0;

virtual int 
getBottomLabel1DL(const int win)=0;

virtual int 
getBottomLabel2DL(const int win)=0;

virtual int 
getBottomLabel3DL(const int win)=0;

virtual int 
getCurrentWindow()=0;

// get the name of the colour for backGroundColour, textColour, ...
virtual aString
getColour( ItemColourEnum item )=0;  

// return the name of a colour for i=0,1,2,...
virtual aString 
getColourName( int i ) const=0;

virtual int 
getColouredSquaresDL(const int win)=0;

virtual int
getColourBarDL(const int win)=0;

const aString & 
getDefaultPrompt();

virtual RealArray 
getGlobalBound() const=0;

virtual bool
getKeepAspectRatio()=0;

virtual bool
getPlotTheAxes(int win_number=-1)=0;

virtual bool
getPlotTheColourBar(int win_number = -1)=0;

virtual bool
getPlotTheColouredSquares(int win_number = -1)=0;

virtual bool
getPlotTheLabels(int win_number = -1)=0;

virtual int 
getFirstUserLabelDL(const int win)=0;

virtual int 
getLastUserLabelDL(const int win)=0;

virtual int 
getLastFixedDL(const int win)=0;

virtual int
getFirstRotableDL(const int win)=0;

virtual int
getAxesDL(const int win)=0;

virtual int
getFirstUserRotableDL(const int win)=0;

virtual int
getLastUserRotableDL(const int win)=0;

virtual int
getLastRotableDL(const int win)=0;

virtual int
getLastDL(const int win)=0;

virtual GUITypes::real
getLineWidthScaleFactor(int window = -1 )=0;

virtual int
getMaxNOfDL(const int win)=0;

int 
getMatch(const aString *menu, aString & answer);
  
virtual int 
getNewLabelList(int win = -1 )=0;

int
getProcessorForGraphics(){return processorForGraphics;};

// return a file pointer to the current command file we are reading
FILE*
getReadCommandFile() const;

// return a file pointer to the current command file we are saving
FILE*
getSaveCommandFile() const;

int 
getValues(const aString & prompt, 
	  IntegerArray & values,
	  const int minimunValue= INT_MIN, 
	  const int maximumValue= INT_MAX,
	  const int sort = 0 );

int 
getValues(const aString & prompt, 
	  RealArray & values,
	  const GUITypes::real minimunValue= -REAL_MAX, 
	  const GUITypes::real maximumValue= REAL_MAX,
	  const int sort = 0 );

virtual void
getView(ViewLocation & loc, int win_number = -1)=0;

virtual aString
getXAxisLabel()=0;

virtual aString
getYAxisLabel()=0;

virtual aString
getZAxisLabel()=0;

virtual bool
getPlotTheRotationPoint(int win_number = -1)=0;

virtual bool
getPlotTheBackgroundGrid(int win_number = -1)=0;

// return true if graphics plotting turned on (Note: this routine is non-virtual).
bool 
graphicsIsOn();

//  Save the graphics window in hard-copy form
virtual int
hardCopy(const aString & fileName=nullString, 
	 GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	 int win_number=-1)=0;

int 
indexInCascadingMenu( int & index,
		      const int startCascade,
		      const int endCascade ) const;
  
virtual void
initView(int win_number=-1)=0;

// input a filename
virtual void
inputFileName(aString &answer, const aString & prompt=nullString, const aString & extension=nullString)=0;



// Input a string after displaying an optional prompt
virtual void 
inputString(aString &answer, const aString & prompt=nullString);
  
// return true if the graphics windows are open (on this processor)
bool 
isGraphicsWindowOpen();

// return true if the graphics windows are open
bool 
isInteractiveGraphicsOn();

virtual void
label(const aString & string,     
      GUITypes::real xPosition, 
      GUITypes::real yPosition,
      GUITypes::real size=.1,
      int centering=0, 
      GUITypes::real angle=0.,
      GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
      const aString & colour = nullString,
      GUITypes::real zOffset =.99 )=0;

// convert normalized coordinates [-1,+1] to world (globalBound) coordinates
virtual int 
normalizedToWorldCoordinates(const RealArray & r, RealArray & x ) const=0;

// Output a string 
virtual void 
outputString(const aString & message, int messageLevel =2 );
  
// output a line to the command file if there is one open.
void
outputToCommandFile( const aString & line );

virtual int 
pause()=0;

virtual int
pickPoints( RealArray & x, 
	    bool plotPoints = TRUE,
	    int win_number = -1 )=0;

// plot and erase title labels
virtual void
plotLabels(GraphicsParameters & parameters, 
	   const GUITypes::real & labelSize=-1.,  // <0 means use default in parameters
	   const GUITypes::real & topLabelHeight=.925,
	   const GUITypes::real & bottomLabelHeight=-.925, 
	   int win_number = -1)=0;

// plot points
virtual void 
plotPoints(const realArray & points, 
	   GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	   int dList = 0)=0;

#ifdef USE_PPP
// Version to use in parallel that takes serial arrays as input and forms the aggregate
virtual void 
plotPoints(const RealArray & points, 
	   GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	   int dList = 0)=0;
#endif

// plot points with different colour
virtual void
plotPoints(const realArray & points, 
	   const realArray & value,
	   GraphicsParameters & parameters =Overture::defaultGraphicsParameters(),
	   int dList = 0 )=0;

#ifdef USE_PPP
// Version to use in parallel that takes serial arrays as input and forms the aggregate
virtual void 
plotPoints(const RealArray & points, 
	   const RealArray & value,
	   GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	   int dList = 0)=0;
#endif

virtual void
plotLines(const realArray & arrows, 
	  GraphicsParameters & parameters = Overture::defaultGraphicsParameters(),
	  int dList = 0 )=0;


// pop a default prompt off the stack and make the next prompt the new default
int 
popDefaultPrompt();

virtual void
pollEvents()=0;

virtual void
pushGUI( GUIState &newState )=0;

virtual void
popGUI()=0;

virtual int
psToRaster(const aString & fileName,
	   const aString & ppmFileName)=0;

// push a default prompt onto a stack and make the current prompt
int 
pushDefaultPrompt(const aString & prompt );


//  Start reading a command file (if no file name is given, prompt for one)
FILE* 
readCommandFile(const aString & commandFileName=nullString);
  
//  Start reading commands from an array of Strings, commands terminated by the aString=""
int 
readCommandsFromStrings(const aString *commands);

// return true of we are reading from a command file.
bool
readingFromCommandFile() const;

// Redraw all graphics display lists
virtual void 
redraw( bool immediate=FALSE)=0;

virtual void 
resetGlobalBound(const int win_number)=0;

virtual void
resetView(int win_number=-1)=0;

// Start saving a command file (if no file name is given, prompt for one)
FILE* 
saveCommandFile(const aString & commandFileName=nullString);
  
int 
savePickCommands(bool trueOrFalse=TRUE );

virtual void
setCurrentWindow(const int & w)=0;

// set the deafult prompt and clear the stack of deafult prompts
int 
setDefaultPrompt(const aString & prompt);

// Start saving an echo file (if no file name is given, prompt for one)
FILE* 
saveEchoFile(const aString & fileName=nullString);
  
virtual int
setAxesLabels( const aString & xAxisLabel=blankString,
	       const aString & yAxisLabel=blankString,
	       const aString & zAxisLabel=blankString )=0;

virtual int
setAxesOrigin( const GUITypes::real x0=defaultOrigin , const GUITypes::real y0=defaultOrigin , const GUITypes::real z0=defaultOrigin )=0;

//  set the colour for subsequent objects that are plotted
virtual int 
setColour( const aString & colourName )=0;

// set colour to default for a given type of item
virtual int 
setColour( ItemColourEnum item )=0;  

virtual void
setColourFromTable( const GUITypes::real value, GraphicsParameters & parameters =Overture::defaultGraphicsParameters() )=0;

// assign a name for colour i
virtual void
setColourName( int i, aString newColourName ) const=0;

// Functions for setting the bounding box and the rotation center
virtual void 
setGlobalBound(const RealArray &xBound)=0;

virtual int
setKeepAspectRatio( bool trueOrFalse=true )=0;

void 
setInfoLevel(int value);

// Set scale factor for line widths (this can be used to increase the line widths for
// high-res off screen rendering.
virtual void 
setLineWidthScaleFactor(const GUITypes::real & lineWidthScaleFactor = 1, int win_number = -1 )=0;

// if true, ignore the "pause" statement
void
setIgnorePause( bool trueOrFalse=true );

virtual void
setInteractiveDL(int list, bool interactive)=0;

virtual void
setLighting(int list, bool lit)=0;

// toggle whether a display list should be plotted or not
virtual void
setPlotDL(int list, bool lit)=0;

// Access functions for plotTheAxes.
virtual void
setPlotTheAxes(bool newState, int win_number=-1)=0;

void
setSingleProcessorGraphicsMode(bool mode){singleProcessorGraphicsMode = mode;};

virtual void
setXAxisLabel(const aString & xAxisLabel_=blankString)=0;

virtual void
setYAxisLabel(const aString & yAxisLabel_=blankString)=0;

virtual void
setZAxisLabel(const aString & zAxisLabel_=blankString)=0;

virtual void
setAxesDimension(int dim, int win_number=-1)=0;

// Access functions for plotTheLabels
virtual void
setPlotTheLabels(bool newState, int win_number = -1)=0;

virtual void
setPlotTheRotationPoint(bool newState, int win_number = -1)=0;

virtual void
setPlotTheColourBar(bool newState, int win_number = -1)=0;

virtual void
setPlotTheColouredSquares(bool newState, int win_number = -1)=0;

virtual void
setPlotTheBackgroundGrid(bool newState, int win_number = -1)=0;

virtual void
setUserButtonSensitive( int btn, int trueOrFalse )=0;

virtual void
setView(ViewLocation & loc, int win_number = -1)=0;

virtual void 
setView(const ViewParameters & viewParameter, const GUITypes::real & value)=0;

// Stop reading the command file (and close the file)
virtual void 
stopReadingCommandFile();

// Stop saving the command file (and close the file)
void 
stopSavingCommandFile();

// Stop saving the echo file (and close the file)
void 
stopSavingEchoFile();

// turn on graphics (grid, contour, streamline... plots)
void
turnOnGraphics();

// turn off graphics  (grid, contour, streamline... plots)
void 
turnOffGraphics();

// remove the last thing appended to the default prompt (just pop's the stack)
int 
unAppendTheDefaultPrompt();

// update the colour bar.
virtual void
updateColourBar(GraphicsParameters & parameters, int window=0)=0;

// convert world to normalized coordinates [-1,+1] 
virtual int 
worldToNormalizedCoordinates(const RealArray & x, RealArray & r ) const=0;

// Plot a label in 2D world coordinates
// This label DOES rotate and scale with the plot
virtual void
xLabel(const aString & string,     
       const GUITypes::real xPosition, 
       const GUITypes::real yPosition,
       const GUITypes::real size=.1,
       const int centering=0,    // -1=left justify, 0=center, 1=right justify
       const GUITypes::real angle=0.,
       GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
       int win_number = -1)=0;
  
// Plot a label in 3D world coordinates
// This label DOES rotate and scale with the plot
virtual void
xLabel(const aString & string,     
       const RealArray & x,    // supply 3 position coordinates
       const GUITypes::real size=.1,    
       const int centering=0, 
       const GUITypes::real angle=0.,   
       GraphicsParameters & parameters =Overture::defaultGraphicsParameters(),
       int win_number = -1)=0;

virtual void
xLabel(const aString & string,     
       const GUITypes::real x[3], 
       const GUITypes::real size =.1,
       const int centering = 0,
       const GUITypes::real angle = 0.,
       GraphicsParameters & parameters  =Overture::defaultGraphicsParameters(),
       int win_number = -1 )=0;

// Plot a label with position and size in World coordinates, 
// This label DOES rotate and scale with the plot. This version of xLabel 
// plots the string in the plane formed by the vectors {\ff rightVector}
// and {\ff upVector}.
virtual void
xLabel(const aString & string,     
       const RealArray & x,     // supply 3 position coordinates
       const GUITypes::real size,         // size in world coordinates
       const int centering,
       const RealArray & rightVector,   // string lies parallel to this vector
       const RealArray & upVector,      // in the plane of these two vectors
       GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
       int win_number = -1)=0;

virtual void
xLabel(const aString & string,     
       const GUITypes::real x[3],  
       const GUITypes::real size,      
       const int centering,
       const GUITypes::real rightVector[3],  
       const GUITypes::real upVector[3],
       GraphicsParameters & parameters  =Overture::defaultGraphicsParameters(),
       int win_number = -1)=0;



protected:
// These must be the same as in GraphicsParameters, put here for convenience
enum Sizes
{
  lineWidth           =GraphicsParameters::lineWidth,       
  axisNumberSize      =GraphicsParameters::axisNumberSize,  
  axisLabelSize       =GraphicsParameters::axisLabelSize,   
  axisMinorTickSize   =GraphicsParameters::axisMinorTickSize,
  axisMajorTickSize   =GraphicsParameters::axisMajorTickSize,
  topLabelSize        =GraphicsParameters::topLabelSize  ,     
  topSubLabelSize     =GraphicsParameters::topSubLabelSize,    
  bottomLabelSize     =GraphicsParameters::bottomLabelSize,
  bottomSubLabelSize  =GraphicsParameters::bottomSubLabelSize,
  minorContourWidth   =GraphicsParameters::minorContourWidth,
  majorContourWidth   =GraphicsParameters::majorContourWidth,
  streamLineWidth     =GraphicsParameters::streamLineWidth,  
  labelLineWidth      =GraphicsParameters::labelLineWidth,   
  curveLineWidth      =GraphicsParameters::curveLineWidth, 
  extraSize1,         
  extraSize2,
  extraSize3,
  numberOfSizes    // counts number of entries in this list
};

aString readFileName, saveFileName, echoFileName;
FILE *readFile, *saveFile, *echoFile;          // command files
std::stack<FILE*> readFileStack;
std::queue<aString> stringCommands;  // this is a FIFO queue, first in, first out

bool getInteractiveResponse;    // if true, do not get next command from a command file

bool readCommands, saveCommands;
bool savePick;             // if false, do not save pick related stuff in the command file.

OvertureParser *parser;    // for parsing commands (with perl for e.g.)
bool useParser;            // if true, parse commands

bool singleProcessorGraphicsMode;  // true if we are only plotting on one processor
int processorForGraphics;          // use this processor for plotting graphics on
bool graphicsPlottingIsOn;         // set to false to skip graphics (for batch runs for e.g.)

GUIState *currentGUI; // the GUIState needs to be here so it is available from fileAnswer

aString defaultPrompt, indentBlanks;
aString *defaultPromptStack; // stack of default prompts
int maxNumberOfDefaultPrompts,topOfDefaultPromptStack;
int saveFileCount;  // counts lines written to log file -- for flushing the file
bool graphicsWindowIsOpen;  // true if the graphics window is open on this processor. 
bool interactiveGraphicsIsOn;  // true if the graphics window is open
bool ignorePause;  // if true, ignore the "pause" statement

bool preferDirectRendering; // if true, prefer direct rendering of OpenGL to the graphics card
HardCopyRenderingEnum hardCopyRenderingType;

int numberRecorded;
IntegerArray *recordDisplayLists;

int simplifyPlotWhenRotating;  // if true then draw a wire frame when rotating (if implemented)

public:
// make this public for now
int gridCoarseningFactor; // coarsen contour/grid plots by this factor (usually for very fine grids) (if implemented) 
// kkc moved this into the public interface so we can add perl commands in the main program to execute before the graphics interface goes to work
int 
parseAnswer(aString & answer );


protected:

int infoLevel;
int echoToTerminal;

int abortProgramIfCommandFileEnds;

void 
constructor(int & argc, char *argv[]);

int 
readLineFromCommandFile(aString & answer );

virtual int 
processSpecialMenuItems(aString & answer);

int
fileAnswer(aString & answer,
	   const aString & prompt,
	   SelectionInfo * selection_);

int 
promptAnswerSelectPick(aString & answer, 
		       const aString & prompt, 
		       SelectionInfo * selection_);

int
promptAnswer(aString & answer, 
	     const aString & prompt = nullString);

};
  


#endif
