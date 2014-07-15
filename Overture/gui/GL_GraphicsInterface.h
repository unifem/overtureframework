#ifndef GL_GRAPHICS_INTERFACE_H 
#define GL_GRAPHICS_INTERFACE_H 

//============================================================================================
//     -----------------------------------
//     ------GL_GraphicsInterface---------
//     -----------------------------------
//
//  This class is a graphics interface based on OpenGL. It uses OpenGL for graphics
//  and MOTIF to manage windows and menues. Command files are
//  supported through the base class, GenericGraphicsInterface
//
//
//   This class can plot 
//     o points and lines
//   All high-level Overture plotting is done by the class PlotIt.
//
//  Optional plotting parameters are passed using an object of the GraphicsParameters class.
//  An object of this class can be used to set many parameters such as the labels on the
//  plots, whether to plot axes, whether to plot line contours etc.
//
// 
//===========================================================================================

#include "GenericGraphicsInterface.h"
#include "GraphicsParameters.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#ifdef OV_USE_GL
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include "nullgl.h"
#include "nullglu.h"
#endif

#include "mogl.h"
#include "GUIState.h"
#include "ColourBar.h"

// extern intArray Overture::nullIntegerDistributedArray();

// define these for the sgi
#if defined(__sgi) && defined(GL_EXT_polygon_offset)
#define glPolygonOffset glPolygonOffsetEXT
#ifndef GL_POLYGON_OFFSET_FILL
#define GL_POLYGON_OFFSET_FILL GL_POLYGON_OFFSET_EXT
#endif
#define OFFSET_FACTOR .00001
#else
#define OFFSET_FACTOR 1.
#endif

// define single and double precision versions of the OpenGL functions
#ifndef OV_USE_DOUBLE
// here are single precision definitions:
#define glColor3     glColor3f
#define glMultMatrix glMultMatrixf
#define glNormal3v   glNormal3fv
#define glNormal3    glNormal3f 
#define glRotate     glRotatef
#define glScale      glScalef
#define glTranslate  glTranslatef
#define glVertex2    glVertex2f
#define glVertex3    glVertex3f
#define glVertex2v   glVertex2fv
#define glVertex3v   glVertex3fv
#else
#define glColor3     glColor3d
#define glMultMatrix glMultMatrixd
#define glNormal3v   glNormal3dv
#define glNormal3    glNormal3d
#define glRotate     glRotated
#define glScale      glScaled
#define glTranslate  glTranslated
#define glVertex2    glVertex2d
#define glVertex3    glVertex3d
#define glVertex2v   glVertex2dv
#define glVertex3v   glVertex3dv
#endif

// More feed back tokens for pass through tokens:
enum GLenum2
{
  GL_DISABLE_TOKEN              = 0x0701,
  GL_ENABLE_TOKEN               = 0x0702,
  GL_LINE_WIDTH_TOKEN		= 0x0703,
  GL_POINT_SIZE_TOKEN		= 0x0704,
  GL_LINE_STIPPLE_TOKEN         = 0x0705 
};

// forward declarations
class UnstructuredMapping;
class AdvancingFront;  
class TrimmedMapping;
class NurbsMapping;
class HyperbolicMapping;

class GL_GraphicsInterface : public GenericGraphicsInterface
{
friend class ColourBar; // *AP* at this point, making ColourBar a friend seems to be the easiest solution. 
// It might be better to absorb class ColourBar into GL_GraphicsInterface

public:
  
// Default constructor (will open a window)
GL_GraphicsInterface(const aString & windowTitle = "Your Slogan Here");

// Constructor that takes argc and argv from main program and strips away Motif parameters
GL_GraphicsInterface(int & argc, char *argv[], const aString & windowTitle = "Your Slogan Here" );

//  This Constructor will only create a window if the the argument initialize=TRUE
//  To create a window later, call createWindow()
GL_GraphicsInterface( const bool initialize, const aString & windowTitle = "Your Slogan Here" );

virtual ~GL_GraphicsInterface();
  
// Open another graphics window (and also a command window if it isn't already opened)
virtual int 
createWindow(const aString & windowTitle = nullString,
	     int argc=0, 
	     char *argv[] = NULL );
  
// display help on a topic in the help pull-down  menu
virtual bool
displayHelp( const aString & topic );

// destroy the window 
virtual int
destroyWindow(int win_number);

// Put up a menu and wait for a response, return the menu item chosen as answer,
// The return value is the item number (0,1,2,... ) (return -1 if there is an error)
// An error can occur if we are reading a command file
virtual int
getMenuItem(const aString *menu, aString & answer, const aString & prompt=nullString);
  
// Erase all graphics display lists and delete the ones that aren't hidable
virtual void 
erase(); 

// erases all display lists in window win_number.
virtual void 
erase(const int win_number, bool forceDelete = false);

// erase a list of display lists
virtual void
erase(const IntegerArray & displayList);

// input a filename through a file selection dialog
virtual void
inputFileName(aString &answer, const aString & prompt=nullString, const aString &extension=nullString);

// Input a string after displaying an optional prompt
virtual void
inputString(aString &answer, const aString & prompt=nullString);

// output a string in the message window
virtual void
outputString(const aString & message, int messageLevel =2 );

// Redraw all graphics display list
virtual void
redraw( bool immediate=FALSE );

// Initialize the view to default
virtual void
initView(int win_number=-1);

virtual void
resetView(int win_number=-1); 

//  // Stop reading the command file (and close the file)
virtual void 
stopReadingCommandFile();

// convert normalized coordinates [-1,+1] to world (globalBound) coordinates
virtual int
normalizedToWorldCoordinates(const RealArray & r, RealArray & x ) const;

// convert world to normalized coordinates [-1,+1] 
virtual int 
worldToNormalizedCoordinates(const RealArray & x, RealArray & r ) const;

// functions for setting the bounding box and the rotation center
virtual void
setGlobalBound(const RealArray &xBound);

virtual RealArray
getGlobalBound() const;

virtual void
resetGlobalBound(const int win_number); 

virtual int
setKeepAspectRatio( bool trueOrFalse=true );

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getKeepAspectRatio}} 
virtual bool
getKeepAspectRatio(){return keepAspectRatio[currentWindow];};
//----------------------------------------------------------------------
// /Description:
// Return true if the aspect ratio is preserved in the current window, otherwise return false.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------

// plot points
virtual void
plotPoints(const realArray & points, 
	   GraphicsParameters & parameters =Overture::defaultGraphicsParameters(),
	   int dList = 0 );

#ifdef USE_PPP
// Version to use in parallel that takes serial arrays as input and forms the aggregate
virtual void
plotPoints(const RealArray & points, 
	   GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	   int dList = 0);
#endif

virtual void
plotPoints(const realArray & points, 
	   const realArray & value,
	   GraphicsParameters & parameters =Overture::defaultGraphicsParameters(),
	   int dList = 0 );

#ifdef USE_PPP
// Version to use in parallel that takes serial arrays as input and forms the aggregate
virtual void 
plotPoints(const RealArray & points, 
	   const RealArray & value,
	   GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	   int dList = 0);
#endif

virtual void
plotLines(const realArray & arrows, 
	  GraphicsParameters & parameters = Overture::defaultGraphicsParameters(),
	  int dList = 0 );

//  choose a colour
virtual aString
chooseAColour();

// get the name of the colour for backGroundColour, textColour, ...
virtual aString
getColour( ItemColourEnum item );  

  //  set the colour for subsequent objects that are plotted
virtual int
setColour( const aString & colourName );

 // set colour to default for a given type of item
virtual int
setColour( ItemColourEnum item ); 

virtual void
setColourFromTable( const GUITypes::real value, GraphicsParameters & parameters =Overture::defaultGraphicsParameters() );

virtual void
setColourName( int i, aString newColourName ) const;

virtual aString
getColourName( int i ) const;

// Draw coloured squares with a number inside them to label colours on the plot
virtual void
drawColouredSquares(const IntegerArray & numberList,
		    GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
		    const int & numberOfColourNames = -1 , // use default colours by default
		    aString *colourNames = NULL);

virtual void 
displayColourBar(const int & numberOfContourLevels,
		 RealArray & contourLevels,
		 GUITypes::real uMin,
		 GUITypes::real uMax,
		 GraphicsParameters & parameters);

// erase the colour bar.
virtual void
eraseColourBar();

//  Save the graphics window in hard-copy form
virtual int
hardCopy(const aString & fileName=nullString, 
	 GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	 int win_number=-1);

aString *colourNames;
aString *fileMenuItems;
aString *helpMenuItems;
aString *graphicsFileMenuItems;
aString *graphicsHelpMenuItems;
aString *menuBarItems;

virtual void
setView(ViewLocation & loc, int win_number = -1); // win_number = -1 == currentWindow

virtual void
getView(ViewLocation & loc, int win_number = -1);

virtual int
generateNewDisplayList(bool lit = false, bool plotIt = true, bool hideable = false, 
		       bool interactive = true);
  
virtual int 
getNewLabelList(int win = -1 );

virtual void
deleteList(int dList);

virtual void
setLighting(int list, bool lit);

virtual void
setPlotDL(int list, bool plot);

virtual void
setInteractiveDL(int list, bool interactive);

// access functions for display lists
virtual int 
getFirstDL(const int win){ return topLabelList[win]; }

virtual int 
getFirstFixedDL(const int win){ return topLabelList[win]; }

virtual int 
getTopLabelDL(const int win){ return topLabelList[win]; }

virtual int 
getTopLabel1DL(const int win){ return topLabel1List[win]; }

virtual int 
getTopLabel2DL(const int win){ return topLabel2List[win]; }

virtual int
getTopLabel3DL(const int win){ return topLabel3List[win]; }

virtual int 
getBottomLabelDL(const int win){ return bottomLabelList[win]; }

virtual int 
getBottomLabel1DL(const int win){ return bottomLabel1List[win]; }

virtual int 
getBottomLabel2DL(const int win){ return bottomLabel2List[win]; }

virtual int 
getBottomLabel3DL(const int win){ return bottomLabel3List[win]; }

virtual int 
getColouredSquaresDL(const int win){ return colouredSquaresList[win]; }

virtual int 
getColourBarDL(const int win){ return colourBarList[win]; }

virtual int 
getFirstUserLabelDL(const int win){ return firstUserLabelList[win]; }

virtual int 
getLastUserLabelDL(const int win){ return lastUserLabelList[win]; }

virtual int 
getLastFixedDL(const int win){ return lastUserLabelList[win]; }

virtual int 
getFirstRotableDL(const int win){ return axesList[win]; }

virtual int 
getAxesDL(const int win){ return axesList[win]; }

virtual int 
getFirstUserRotableDL(const int win){ return firstUserRotableList[win]; }

virtual int
getLastUserRotableDL(const int win){ return lastUserRotableList[win]; }

virtual int 
getLastRotableDL(const int win){ return lastUserRotableList[win]; }

virtual int 
getLastDL(const int win){ return lastUserRotableList[win]; }

virtual int 
getMaxNOfDL(const int win){ return maximumNumberOfDisplayLists[win]; }

virtual int 
getCurrentWindow();

virtual void
setCurrentWindow(const int & w);

// New way of setting plotTheAxes.
virtual void
setPlotTheAxes(bool newState, int win_number=-1);

virtual bool
getPlotTheAxes(int win_number=-1);

virtual void
setAxesDimension(int dim, int win_number=-1);

virtual void
setPlotTheLabels(bool newState, int win_number = -1);

virtual bool
getPlotTheLabels(int win_number = -1);

virtual void
setPlotTheRotationPoint(bool newState, int win_number = -1);

virtual bool
getPlotTheRotationPoint(int win_number = -1);

virtual void
setPlotTheColourBar(bool newState, int win_number = -1);

virtual bool
getPlotTheColourBar(int win_number = -1);

virtual void
setPlotTheColouredSquares(bool newState, int win_number = -1);

virtual bool
getPlotTheColouredSquares(int win_number = -1);

virtual void
setPlotTheBackgroundGrid(bool newState, int win_number = -1);

virtual bool
getPlotTheBackgroundGrid(int win_number = -1);

virtual void
pollEvents();

virtual int
getAnswer(aString & answer, const aString & prompt);

virtual int
getAnswer(aString & answer, const aString & prompt,
	  SelectionInfo &selection);

virtual int
getAnswerNoBlock(aString & answer, const aString & prompt);


virtual int
pickPoints( RealArray & x, 
	    bool plotPoints = TRUE,
	    int win_number = -1 );

virtual void
setUserButtonSensitive( int btn, int trueOrFalse );

virtual void
pushGUI( GUIState &newState );

virtual void
popGUI();

virtual int
beginRecordDisplayLists( IntegerArray & displayLists);

virtual int
endRecordDisplayLists( IntegerArray & displayLists);

virtual void
createMessageDialog(aString msg, MessageTypeEnum type);

virtual int
pause();

virtual void
appendCommandHistory(const aString &answer);

virtual int
processSpecialMenuItems(aString & answer);

// plot and erase title labels
virtual void
plotLabels(GraphicsParameters & parameters, 
	   const GUITypes::real & labelSize=-1.,  // <0 means use default in parameters
	   const GUITypes::real & topLabelHeight=.925,
	   const GUITypes::real & bottomLabelHeight=-.925, 
	   int win_number = -1);

virtual void
eraseLabels(GraphicsParameters & parameters, int win_number = -1); 

virtual int
setAxesLabels( const aString & xAxisLabel=blankString,
	       const aString & yAxisLabel=blankString,
	       const aString & zAxisLabel=blankString );

virtual aString
getXAxisLabel(){return xAxisLabel[currentWindow];};

virtual aString
getYAxisLabel(){return yAxisLabel[currentWindow];};

virtual aString
getZAxisLabel(){return zAxisLabel[currentWindow];};
  
virtual void
setXAxisLabel(const aString & xAxisLabel_=blankString){xAxisLabel[currentWindow]=xAxisLabel_;};

virtual void
setYAxisLabel(const aString & yAxisLabel_=blankString){yAxisLabel[currentWindow]=yAxisLabel_;};

virtual void
setZAxisLabel(const aString & zAxisLabel_=blankString){zAxisLabel[currentWindow]=zAxisLabel_;};

virtual int
setAxesOrigin( const GUITypes::real x0=defaultOrigin , const GUITypes::real y0=defaultOrigin , const GUITypes::real z0=defaultOrigin );

virtual void
getAxesOrigin( GUITypes::real & x0, GUITypes::real & y0, GUITypes::real & z0 ){
  x0=axesOrigin[currentWindow](0); y0=axesOrigin[currentWindow](1); z0=axesOrigin[currentWindow](2);
};

// return the aspect ratio of a window
virtual GUITypes::real
getAspectRatio(const int win=0){ return aspectRatio[win];}

// Set scale factor for line widths (this can be used to increase the line widths for
// high-res off screen rendering.
virtual void 
setLineWidthScaleFactor(const GUITypes::real & lineWidthScaleFactor = 1, int win_number = -1 );

virtual GUITypes::real 
getLineWidthScaleFactor(int window = -1 );

virtual int
psToRaster(const aString & fileName,
	   const aString & ppmFileName );
  

// This utility routines plots a label, this label does NOT rotate or scale with the plot
virtual void
label(const aString & string,     
      GUITypes::real xPosition, 
      GUITypes::real yPosition,
      GUITypes::real size=.1,
      int centering=0, 
      GUITypes::real angle=0.,
      GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
      const aString & colour = nullString,
      GUITypes::real zOffset =.99 );
  
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
       int win_number = -1);
  
// Plot a label in 3D world coordinates
// This label DOES rotate and scale with the plot
virtual void
xLabel(const aString & string,     
       const RealArray & x,    // supply 3 position coordinates
       const GUITypes::real size=.1,    
       const int centering=0, 
       const GUITypes::real angle=0.,   
       GraphicsParameters & parameters =Overture::defaultGraphicsParameters(),
       int win_number = -1);

virtual void
xLabel(const aString & string,     
       const GUITypes::real x[3], 
       const GUITypes::real size =.1,
       const int centering = 0,
       const GUITypes::real angle = 0.,
       GraphicsParameters & parameters  =Overture::defaultGraphicsParameters(),
       int win_number = -1 );

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
       int win_number = -1);

virtual void
xLabel(const aString & string,     
       const GUITypes::real x[3],  
       const GUITypes::real size,      
       const int centering,
       const GUITypes::real rightVector[3],  
       const GUITypes::real upVector[3],
       GraphicsParameters & parameters  =Overture::defaultGraphicsParameters(),
       int win_number = -1);

virtual void
drawColourBar(const int & numberOfContourLevels,
	      RealArray & contourLevels,
	      GUITypes::real uMin=0., 
	      GUITypes::real uMax=1.,
	      GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	      GUITypes::real xLeft=.775,  // .8
	      GUITypes::real xRight=.825, // .85
	      GUITypes::real yBottom=-.75,
	      GUITypes::real yTop=.75);

// update the colour bar.
virtual void
updateColourBar(GraphicsParameters & parameters, int window=0);

// the following three routines need to be here so they are accessible by the callbacks
// initialize the screen, not normally called by a user.
void 
init(const int & win_number);    
  
void
changeView(const int & win_number,
	   const real & dx,   
	   const real & dy , 
	   const real & dz,
	   const real & dThetaX=0.,
	   const real & dThetaY=0.,
	   const real & dThetaZ=0.,
	   const real & magnify=1. );
  
// display the screen, not normally called by a user. 
void 
display(const int & win_number); 

protected:

// reset rotation point to default
void
resetRotationPoint(int win_number=-1);

// This routine sets the projection and modelview to a normalized system on [-1,1]x[-1,1]
void
setNormalizedCoordinates();

// This routine un-does the previous function
void
unsetNormalizedCoordinates();

// convert pick [0,1] + zBuffer [0,2**m] coordinates to world coordinates. 
int
pickToWorldCoordinates(const RealArray & r, RealArray & x, const int & win_number ) const;

void
setRotationCenter(GUITypes::real rotationPoint[3], int win_number=-1);

// This utility routines plot Axes
void
plotAxes(const RealArray & xBound, 
	 const int numberOfDimensions,
	 GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	 int win_number = -1);

// erase the axes
void
eraseAxes(int win_number);  

// This label uses raster fonts  **** this is not finished *****
void
labelR(const aString & string,     
       const GUITypes::real xPosition, 
       const GUITypes::real yPosition,
       const GUITypes::real size=.1,
       const int centering=0, 
       const GUITypes::real angle=0.,
       GraphicsParameters & parameters=Overture::defaultGraphicsParameters());
  
// render directly to post-script without a raster
int
renderPS(const char * fileName, 
	 GraphicsParameters & parameters =Overture::defaultGraphicsParameters() );
  
void
setRotationTransformation(int win_number);

virtual void 
setView(const ViewParameters & viewParameter, const GUITypes::real & value);

void 
getWindowShape( int window, GUITypes::real & leftSide,GUITypes::real & rightSide,GUITypes::real & top,GUITypes::real & bottom) const;

GUIState * 
getCurrentGUI(){ return currentGUI;}

// set the model view matrix for the current bounds
void 
setModelViewMatrix();

void 
setFractionOfScreen(const GUITypes::real & fraction = .75, int win_number = -1 );

void
lightsOn(int win_number);

void
lightsOff(int win_number);

WindowProperties wProp;

int currentWindow;

// plotting routines should only use display lists up to numberOfAvailableDisplayLists.
// a few more are reserved for labels and other stuff
int topLabelList[MAX_WINDOWS],                     // display list number for the top label
  topLabel1List[MAX_WINDOWS],             // display list number for the top label sub 1
  topLabel2List[MAX_WINDOWS],
  topLabel3List[MAX_WINDOWS],
  bottomLabelList[MAX_WINDOWS],
  bottomLabel1List[MAX_WINDOWS],
  bottomLabel2List[MAX_WINDOWS],
  bottomLabel3List[MAX_WINDOWS],
  colouredSquaresList[MAX_WINDOWS],
  colourBarList[MAX_WINDOWS],
  firstUserLabelList[MAX_WINDOWS],                        
  lastUserLabelList[MAX_WINDOWS], /* = firstUserLabelList+100 */ 
  axesList[MAX_WINDOWS],             // axes are rotated
  firstUserRotableList[MAX_WINDOWS],                        
  lastUserRotableList[MAX_WINDOWS],                        
  maximumNumberOfDisplayLists[MAX_WINDOWS]; // this is the actual number that we have

// is the lighting on in the list?
// should the list be plotted?
// should the list be erased or just not plotted?
// should the list be display during interactive rotation?
IntegerArray plotInfo[MAX_WINDOWS]; 

public: // do this for now *wdh* 080123

bool keepAspectRatio[MAX_WINDOWS];   // usually we keep the aspect ratio for plots.
  
bool viewHasChanged[MAX_WINDOWS];

// global bounding box and rotation center
RealArray globalBound[MAX_WINDOWS];
GUITypes::real rotationCenter[MAX_WINDOWS][3];

GUITypes::real magnificationFactor[MAX_WINDOWS], magnificationIncrement[MAX_WINDOWS];
GUITypes::real aspectRatio[MAX_WINDOWS], fractionOfScreen[MAX_WINDOWS];
  
aString wTitle[MAX_WINDOWS];

aString defaultWindowTitle;
GraphicsParameters::HardCopyType hardCopyType[MAX_WINDOWS];
GraphicsParameters::OutputFormat outputFormat[MAX_WINDOWS];

aString hardCopyFile[MAX_WINDOWS];
int rasterResolution[MAX_WINDOWS];       // resolution for off screen renderer
int horizontalRasterResolution[MAX_WINDOWS];       // resolution for off screen renderer

// *wdh* removed 100912
// aString movieBaseName[MAX_WINDOWS];
// bool saveMovie[MAX_WINDOWS];
// int numberOfMovieFrames[MAX_WINDOWS], movieFirstFrame[MAX_WINDOWS];
// GUITypes::real movieDxRot[MAX_WINDOWS], movieDyRot[MAX_WINDOWS], movieDzRot[MAX_WINDOWS];
// GUITypes::real movieDxTrans[MAX_WINDOWS], movieDyTrans[MAX_WINDOWS], movieDzTrans[MAX_WINDOWS];
// GUITypes::real movieRelZoom[MAX_WINDOWS];

// hardcopy dialog window: 
DialogData hardCopyDialog[MAX_WINDOWS];

// *wdh* removed 100912 
// DialogData movieDialog[MAX_WINDOWS];

// option pulldown menu
PullDownMenu optionMenu[MAX_WINDOWS];

int userLabel[MAX_WINDOWS];              // current number of userLabels;
int labelsPlotted[MAX_WINDOWS];          // true if the top/bottom labels have been plotted.

GUITypes::real xEyeCoordinate, yEyeCoordinate, zEyeCoordinate;  // position of the eye
GUITypes::real dtx[MAX_WINDOWS], dty[MAX_WINDOWS], dtz[MAX_WINDOWS];
GUITypes::real xShift[MAX_WINDOWS], yShift[MAX_WINDOWS], zShift[MAX_WINDOWS];

GUITypes::real deltaX[MAX_WINDOWS], deltaY[MAX_WINDOWS], deltaZ[MAX_WINDOWS], deltaAngle[MAX_WINDOWS];
GUITypes::real defaultNear[MAX_WINDOWS], defaultFar[MAX_WINDOWS];

GUITypes::real leftSide[MAX_WINDOWS], rightSide[MAX_WINDOWS];
GUITypes::real bottom[MAX_WINDOWS], top[MAX_WINDOWS], near[MAX_WINDOWS], far[MAX_WINDOWS]; 

RealArray rotationMatrix[MAX_WINDOWS], matrix[MAX_WINDOWS];
#ifndef NO_APP
Index I4;
#endif

GUITypes::real windowScaleFactor[MAX_WINDOWS][3];      // current scale factors for the plotting window.
bool userDefinedRotationPoint[MAX_WINDOWS];  // TRUE if the user has defined the rotation center

int axesOriginOption[MAX_WINDOWS];           // indicates where to place the axes origin
RealArray axesOrigin[MAX_WINDOWS];

aString xAxisLabel[MAX_WINDOWS];
aString yAxisLabel[MAX_WINDOWS];
aString zAxisLabel[MAX_WINDOWS];

GUITypes::real shiftCorrection[MAX_WINDOWS][3]; // default correction for changing the rotation center

GUITypes::real backGround[MAX_WINDOWS][4];  // back ground colour (RGBA)
GUITypes::real foreGround[MAX_WINDOWS][4];  // text colour (not presently used) (RGBA)

// we can change the back-ground and text colour
// replace this by
aString backGroundName[MAX_WINDOWS], foreGroundName[MAX_WINDOWS];

bool saveTransformationInfo;
GLdouble modelMatrix[MAX_WINDOWS][16], projectionMatrix[MAX_WINDOWS][16];
GLint viewPort[MAX_WINDOWS][4];

GUITypes::real homeViewParameters[MAX_WINDOWS][14];  // holds the view parameters for the "home" view.

// here are default values for lights
int lightIsOn[MAX_WINDOWS][numberOfLights];  // which lights are on
GLfloat ambient[MAX_WINDOWS][numberOfLights][4];
GLfloat diffuse[MAX_WINDOWS][numberOfLights][4];
GLfloat specular[MAX_WINDOWS][numberOfLights][4];
GLfloat position[MAX_WINDOWS][numberOfLights][4];
GLfloat globalAmbient[MAX_WINDOWS][4];

// default material properties, these values are used for surfaces in 3D when we give them
// different colours
GLfloat materialAmbient[MAX_WINDOWS][4]; // AP: this variable is no longer used
GLfloat materialDiffuse[MAX_WINDOWS][4]; // AP: this variable is no longer used
GLfloat materialSpecular[MAX_WINDOWS][4];
GLfloat materialShininess[MAX_WINDOWS];
GLfloat materialScaleFactor[MAX_WINDOWS]; // AP: this variable is no longer used

// view characteristics structure, used to communicate with mogl
ViewCharacteristics viewChar[MAX_WINDOWS];

int lighting[MAX_WINDOWS];
bool plotTheAxes[MAX_WINDOWS]; // false=no axes
bool plotBackGroundGrid[MAX_WINDOWS]; // false =no grid

int axesDimension[MAX_WINDOWS]; // 1,2,3 = 1D, 2D, 3D axes
bool plotTheRotationPoint[MAX_WINDOWS];
bool plotTheLabels[MAX_WINDOWS];
bool plotTheColouredSquares[MAX_WINDOWS];
bool plotTheColourBar[MAX_WINDOWS];

  // Scale line widths/ point sizes by this amount (for high-res off-screen render)
GUITypes::real lineWidthScaleFactor[MAX_WINDOWS];

// clipping planes
enum
{
  maximumNumberOfClippingPlanes=6,
  maximumNumberOfTextStrings=25
};
  
// number of clipping planes that have been turned on
int numberOfClippingPlanes[MAX_WINDOWS];               

// TRUE if a clipping plane is turned on
// each clipping plane is defined by four constants c0*x+c1*y+c2*z+c3
int clippingPlaneIsOn[MAX_WINDOWS][maximumNumberOfClippingPlanes];   
double clippingPlaneEquation[MAX_WINDOWS][maximumNumberOfClippingPlanes][4];
// store the clipping plane names in a array (for convenience)
GLenum clip[MAX_WINDOWS][maximumNumberOfClippingPlanes];
ClippingPlaneInfo clippingPlaneInfo[MAX_WINDOWS];

int textIsOn[maximumNumberOfTextStrings];

ColourBar colourBar;

void setProjection(const int & win_number); 
  
// for displaying user labels (defined interactively)
void
annotate(const aString & answer);  
  
void
constructor(); // int & argc, char *argv[], const aString & windowTitle = "Your Slogan Here");

bool
readOvertureRC();

int
getAnswerSelectPick(aString & answer, const aString & prompt,
		    SelectionInfo *selection_ = NULL,
		    int blocking = 1);

int
interactiveAnswer(aString & answer,
		  const aString & prompt,
		  SelectionInfo * selection_,
		  int blocking = 1);

//  int
//  select(const float xPick[], IntegerArray & selection, const int & selectOption=0 );
  
void
parseCommandLine( const aString & line, aString & command, int & windowNumber, aString & arg ) const;

void
pickNew(PickInfo &xPick, SelectionInfo & select);

int
selectNew(PickInfo & xPick, SelectionInfo *select);

int
offScreenRender(const char * fileName, 
		GraphicsParameters & parameters=Overture::defaultGraphicsParameters() );

// This version uses X pixmaps
int
offScreenRenderX(const char * fileName, 
		GraphicsParameters & parameters=Overture::defaultGraphicsParameters() );

// This version uses the OSMesa library
int
offScreenRenderMesa(const char * fileName, 
		GraphicsParameters & parameters=Overture::defaultGraphicsParameters() );

int
saveRasterInAFile(const aString & fileName, 
		  void *buffer, 
		  const GLint & width, 
		  const GLint & height,
		  const int & rgbType  =0,
		  GraphicsParameters & parameters=Overture::defaultGraphicsParameters() );

void
rleCompress( const int num, GLubyte *xBuffer, FILE *outFile, const int numPerLine = 30 );

void
setMaterialProperties(float ambr, float ambg, float ambb,
		      float difr, float difg, float difb,
		      float specr, float specg, float specb, float shine);

void
setupHardCopy(DialogData &hcd, int win);

// void
// setupMovie(DialogData &mov, int win);

void
hardcopyCommands(aString &longAnswer, int win_number);

// void
// movieCommands(aString &longAnswer, int win_number);

void
optionCommands(aString &answer, int win_number);

void
openGUI();

void
disableGUI();

// void
// getNormal( const MappedGrid & mg, const IntegerArray & iv, const int axis, RealArray & normal);


// these variables used to be in the class PlotStuff
// for grid plots
//IntegerArray boundaryConditionList;
//int numberOfBoundaryConditions;

// for streamlines:
// IntegerArray maskForStreamLines;
// IntegerArray componentsToInterpolate;

};
  
// define some macros to add extra info to the feed-back array
// Each macro will replace a single OpenGL call by 3 calls, adding extra info to the feedback array.

#define glEnable(stuff)    glEnable(stuff),   glPassThrough(GL_ENABLE_TOKEN),     glPassThrough(stuff)
#define glDisable(stuff)   glDisable(stuff),  glPassThrough(GL_DISABLE_TOKEN),    glPassThrough(stuff)
#define glLineWidth(size)  glLineWidth(size), glPassThrough(GL_LINE_WIDTH_TOKEN), glPassThrough(size)
#define glLineStipple(num,pattern) glLineStipple(num,pattern), \
              glPassThrough(GL_LINE_STIPPLE_TOKEN), glPassThrough(num), glPassThrough(pattern)
#define glPointSize(size)  glPointSize(size), glPassThrough(GL_POINT_SIZE_TOKEN), glPassThrough(size)

#endif
