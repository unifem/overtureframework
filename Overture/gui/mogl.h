#ifndef MOGL_H 
#define MOGL_H

#include "DialogData.h"

class GL_GraphicsInterface;

#define MAX_WINDOWS 3
#define MAX_BUTTONS 15
#define TRUE  1
#define FALSE 0
#define OK 1
#define ERROR 0

enum
{
  numberOfLights=3 
};

struct ClippingPlaneInfo
{
  int maximumNumberOfClippingPlanes;
  int *clippingPlaneIsOn;
  double *clippingPlaneEquation;
};

struct ViewCharacteristics
{
  int win_number;

  GUITypes::real *rotationCenter;
  int *axesOriginOption_;           // indicates where to place the axes origin

  GUITypes::real *backGround, *foreGround;
  char backGroundName[100], foreGroundName[100];

  // here are default values for lights
  int *lighting_;
  int *lightIsOn;  // which lights are on
  GLfloat *ambient[numberOfLights];
  GLfloat *diffuse[numberOfLights];
  GLfloat *specular[numberOfLights];
  GLfloat *position[numberOfLights];
  GLfloat *globalAmbient;

  // default material properties, these values are used for surfaces in 3D when we give them
  // different colours
  GLfloat *materialAmbient;
  GLfloat *materialDiffuse;
  GLfloat *materialSpecular;
  GLfloat *materialShininess_;
  GLfloat *materialScaleFactor_;

  GUITypes::real *lineScaleFactor_;
  GUITypes::real *fractionOfScreen_;
};

class WindowProperties
{
public:
// constructor
WindowProperties(){
  commandWindowWidth=-1; commandWindowHeight=-1; commandWindowNx=-1; commandWindowNy=-1;
  graphicsWindowWidth=-1; graphicsWindowHeight=-1; graphicsWindowNx=-1; graphicsWindowNy=-1;
  dialogNx=-1; dialogNy=-1;
  dialogOffsetNx=10; dialogOffsetNy=0;  // shift new dialogs by this amount 
  foregroundColour=""; backgroundColour=""; showRubberBandBox=true;
  preferDirectRendering=true; showCommandHistory=true; showPrompt=true; }
// data
int commandWindowWidth;
int commandWindowHeight;
int commandWindowNx, commandWindowNy;  // x,y location for upper left corner of command window

int graphicsWindowWidth;
int graphicsWindowHeight;
int graphicsWindowNx, graphicsWindowNy;  // x,y location for upper left corner of graphics window

int dialogNx, dialogNy;  // x,y location for upper left corner of dialog window
int dialogOffsetNx, dialogOffsetNy;  // shift new dialogs by this amount 

aString foregroundColour;
aString backgroundColour;
bool showRubberBandBox;
int preferDirectRendering;
bool showCommandHistory;    // echo command history
bool showPrompt;            // echo command prompt

};


// Display the screen again immediately
// (see also moglPostDisplay)
void 
moglDisplay(int win=0);

void 
moglGetWindowSize(int & width, int & height, int win=0 );

void
moglOpenFileSB(char *pattern = NULL);

void
moglCloseFileSB();

void
moglCreateMessageDialog(aString msg, MessageTypeEnum type);

void
moglBuildUserButtons(const aString buttonCommand[], const aString buttonLabel[], int win_number);

void
moglBuildUserMenu(const aString menuName[], const aString menuTitle, int win_number);

void
moglSetSensitive(int win_number, int trueOrFalse);

void
moglSetButtonSensitive(int win_number, int btn, int trueOrFalse);

void
moglBuildPopup(const aString menu[]);

void 
moglInit(int & argc, 
	 char *argv[], 
	 const aString &windowTitle, 
	 aString fileMenuItems[],
	 aString helpMenuItems[],
	 WindowProperties &wProp);

int 
moglGetAnswer( aString &answer, const aString prompt = "", 
	       PickInfo *pick_ =NULL, int blocking = 1 );


int 
moglGetMenuItem(const aString menu[], aString &answer, const aString prompt="", 
		real *pickBox=0, int win_number=0 );

// Display the screen the next time the event loop is entered
void 
moglPostDisplay(int win=0);

//kkc xltypedef void MOGL_DISPLAY_FUNCTION(GL_GraphicsInterface *giPointer=NULL, const int & win_number=0);
//kkc xltypedef void MOGL_RESIZE_FUNCTION(GL_GraphicsInterface *giPointer=NULL,  const int & win_number=0);
typedef void MOGL_DISPLAY_FUNCTION(GL_GraphicsInterface *giPointer, const int & win_number);
typedef void MOGL_RESIZE_FUNCTION(GL_GraphicsInterface *giPointer,  const int & win_number);
// Define the functions that will display and resize the screen 
// (same function for all windows)
void
moglSetFunctions( GL_GraphicsInterface *giPointer,
                  MOGL_DISPLAY_FUNCTION displayFunc, 
                  MOGL_RESIZE_FUNCTION resizeFunc );
// set the prompt in the command window
void 
moglSetPrompt(const aString &prompt);

void
moglAppendCommandHistory(const aString &item);

typedef void MOGL_VIEW_FUNCTION(GL_GraphicsInterface *giPointer,
				const int & win,
                                const real & dx,   
				const real & dy, 
				const real & dz,
				const real & dThetaX,
				const real & dThetaY,
				const real & dThetaZ,
				const real & magnify );
// define the function that will be called when the rubber-band box
// is used to zoom in
void 
moglSetViewFunction( MOGL_VIEW_FUNCTION viewFunction );
int
makeGraphicsWindow(const aString &windowTitle, 
		   aString fileMenuItems[],
		   aString helpMenuItems[],
		   ClippingPlaneInfo & clippingPlaneInfo,
		   ViewCharacteristics & viewChar,
		   DialogData & hardCopyDialog,
		   PullDownMenu &optionMenu,
		   WindowProperties &wProp,
                   int directRendering = 1 );
int
destroyGraphicsWindow(int win_number);
int
moglMakeCurrent(int win);
int
moglGetNWindows();
int
moglGetCurrentWindow();
int 
moglSetTitle(int win_number, const aString &windowTitle);
void
moglPollEvents();
void 
moglPrintRotPnt(real x, real y, real z, int win_number);
void 
moglPrintLineWidth(real lw, int win_number);
void 
moglPrintFractionOfScreen(real fraction, int win_number);
bool
moglRotationKeysPressed(int win_number);


// ----------- VIEWPORT interface -------------------------- **pf

void graphics_setFrustum      ( GLdouble left,   GLdouble right,
				GLdouble bottom, GLdouble top,
				GLdouble near,   GLdouble far);

void graphics_setOrtho        ( GLdouble left,   GLdouble right,
			        GLdouble bottom, GLdouble top,
				GLdouble near,   GLdouble far);

void graphics_setOrthoKeepAspectRatio( GLdouble aspectRatio, GLdouble magFactor,
				       GLdouble left,   GLdouble right,
				       GLdouble bottom, GLdouble top,
				       GLdouble near,   GLdouble far);

void graphics_setPerspective ( GLdouble fovy,   GLdouble aspect,
			       GLdouble near,   GLdouble far);

//void graphics_setLookAt (GLdouble eyeX,    GLdouble eyeY,    GLdouble eyeZ, 
//			 GLdouble centerX, GLdouble centerY, GLdouble centerZ,// 
//			 GLdouble upX,     GLdouble upY,     GLdouble upZ);

#endif
