//=============================================================================================
//    Motif-OpenGL graphics Interface
//
//
//  /Purpose:
//    Define a graphics interface using Motif and OpenGL
//  /Author: WDH \& AP
//  /Revised by AP to add a separate command window and allow for multiple graphics windows.
//
//  NOTE:
//     The label on pull-down menus does not work on Linux&Lesstif v0.88.1.
//  Therefore, the label has been DISABLED on that platform. This has no
//  effect on the usability, just appearance.  3/13/2000
//
//  -----------------------------------------------------------------------
//    Reorganized 3/13/2000 to group 'external' & 'internal' functionality
//  separately: 'Mogl.C' could be a class ('OvertureGUI' or similar). 
//  There is a clear separation into _internal_ Motif stuff that 
//  shouldn't be visible to other classes (these should be 'private').
//  Routines with the prefix 'mogl' are part of the interface to the class
//  (i.e. 'public' members).
//
//  In principle, one should be able to port Overture to other Widgets & 
//  Windowing systems by rewriting only the 'internal parts' 
//  (=Motif specific bits) on the new platform. ALL Motif specific 
//  code sits in this file (mogl.C). The rest of Overture calls 
//  the service routines (moglXXX) in this file, or uses at most OpenGL.
//
//  This would also be a way to define an alternative user interface
//  to the Overture tools, if such was to be necessary. 
//
//  NOTE: you will need to have OpenGL on any platform you'd like to port
//   Overture onto -- trying to remove OpenGL from Overture would be 
//   painful. 
//               wdh & pf
//============================================================================================

//TO get debug output, define this
//#define PF_DEBUG

//// on the alpha, or 64bit sgi pointers are long int's
//..Replaced directly into the code for portability
//#if (defined(__alpha) || (__mips==4))
//  #define POINTER_TO_INT long int
//#else
//  #define POINTER_TO_INT int
//#endif

// define OV_USE_LESSTIF

#ifndef NO_APP
#include "GenericDataBase.h"
#else
#include "GUIDefine.h"
#include "GUITypes.h"
#endif

#ifndef OV_USE_DOUBLE
typedef float real;
#else
typedef double real;
#endif

/* *wdh* 090704 : for sScanF: */
#include "wdhdefs.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/Command.h>
#include <Xm/DialogS.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/SelectioB.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

//#include "aString.H"
#ifndef NO_APP
#include "aString.H"
#else
#include <string>
#ifndef aString
#define aString std::string
#endif
#endif

#include "mogl.h"
#include "DialogData.h"

//
// *NOTE* we need to add the info from the visual to popup windows --- otherwise this generates an error in XCreateWindow 
//  ????? what is this
//


// -------- VDL INIT -------- 
// *Define VDL interface headers & global variables
// Who to blame: pf
//
#ifdef USE_POWERWALL
extern "C" {
#include <vdl/vdl.h>
}

static void vdlInitWin( vdlTileP tile, void *data );
static void vdlDraw(vdlTileP tile, void *data);

void initGL();
//void draw(Widget w);  //declared below
void new_graphicsInit();
void pw_graphicsInit(Widget w, XtPointer clientData, XtPointer call);
void graphicsInit_2(Widget w, XtPointer clientData, XtPointer call);
//static void pw_args(int argc, char **argv);
static void vdlSetConfigurationFile();
void debug_sniff_for_opengl_errors(char *name);
//
char *vdlFile;
vdlCanvas *canvas;
Bool vdlShareDisplayLists = True;
//Bool vdlShareDisplayLists = False;
Bool vdlFlag = True;
Bool has_vdl = False;
int vdlActiveWindowNumber = 0;

int vdl_width  =0; 
int vdl_height =0;
GLdouble vdl_aspectRatio = 1.;

#endif
/* -------- END VDL INIT ---- */


#ifdef OV_USE_LOCAL_GLW
#include "GLwMDrawA.h" /* use version in Overture/include */
#else
#ifdef OV_USE_MESA
#include <GL/GLwMDrawA.h>   /* Mesa version */
#else
#include <X11/GLw/GLwMDrawA.h>  /* Motif OpenGL drawing area widget */
#endif
#endif
struct WINDOW_REC{
Widget toplevel, mainw, menubar, frame, plotStuff, userCascade,
  rightColumn, rotateFrame, rotateButtons, midarea, userButtonArea, userMenu; // allow for 1 user pulldown menu
Widget userButton[MAX_BUTTONS]; // allow up to MAX_BUTTONS user buttons in each window;
Widget xValue; // editable rotation point widget
Widget lineSF; // editable line scale factor widget
Widget fractionOfScreen;
  
int viewCharOpen; // flag to tell whether the view char dialog is open
int clipPlaneOpen; // same for clip planes dialog
int annotateOpen; // same for the annotate dialog

int numUserBtn;
XVisualInfo *vi;
Colormap     cmap;
GLXContext   cx;
GLboolean postDisplay;
Dimension width, height;
GLboolean doubleBuffer;
};

static WINDOW_REC *OGLWindowList[MAX_WINDOWS];
static int OGLNWindows=0, OGLCurrentWindow=0;
// keep track of if moglInit has been called (would not be the case when running without graphics!)
static int moglInitialized=0; 

static bool showRubberBandBox=true; // by default we draw a rubber band box when zooming
static bool showCommandHistory=true; // echo command history
static bool showPrompt=true;         // echo prompt

// ----------Xt/Motif stuff
static XtAppContext app;

static int screenWidth, screenHeight;

static Widget promptText, textw, command_w, oldCommandList, bulletin, menubar, fsDialog=NULL;
static Widget popupMenu[MAX_WINDOWS+1]={0,0,0,0};

static Arg menuPaneArgs[4], args[1], scrolledTextArgs[20];
static XSetWindowAttributes windowAttributes;
static Cursor cursor, rotateCursor, translateCursor, zoomCursor, boxCursor, 
  boxSpiralCursor, dotCursor, crossCursor;
static XtAppContext & xtAppContext = app;

XGCValues gcValues;
  
static GLboolean preferDirectRendering=GL_TRUE;  // set to 0 to turn off direct rendering

// This points to the function used to display the screen
//..GL
static void 
display(GL_GraphicsInterface *giPointer=NULL, const int & win_number=0);
static void 
resize(GL_GraphicsInterface *giPointer=NULL, const int & win_number=0);
static void
pickCallback( Widget widget, XtPointer client_data, XtPointer call_data );
static void
userToggleButtonCallback( Widget widget, XtPointer client_data, XtPointer call_data );
static void
userOpMenuCallback(Widget widget, XtPointer client_data, XtPointer call_data );
static void
userRadioBoxCallback(Widget widget, XtPointer client_data, XtPointer call_data );
static void
userTextLabelCallback( Widget widget, XtPointer client_data, XtPointer call_data );
static void 
destroyUserDialog( Widget w, XtPointer client_data, XtPointer call_data);
static void
rubberBandPickNew(Display *disp, Window winid, XEvent *event0, int win_number);
void
showHardcopyDialog(Widget pb, XtPointer client_data, XtPointer call_data);
int
checkGLError();
void
toggled(Widget widget, XtPointer client_data, XtPointer call_data );


MOGL_DISPLAY_FUNCTION *displayFunction = display;
MOGL_RESIZE_FUNCTION  *resizeFunction  = resize;
MOGL_VIEW_FUNCTION    *viewFunction    = NULL;

// ----------GL stuff

static int config[] = {
  None, None,           /* Space for multisampling GLX
                           attributes if supported. */
  GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, 16,
  GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,
  None
};

static int *dblBuf = &config[2];
static int *snglBuf = &config[3];
static String fallbackResources[] = {
  (char*)"*title: PlotStuff III", // this should be first since we replace it below
  (char*)"*sgiMode: true",     /* Try to enable Indigo Magic look & feel */
  (char*)"*useSchemes: all",   /* and SGI schemes. */
  (char*)"*filebox_popup*title: Choose a file...",
//  (char*)"*fontList: -*-courier-*-r-*--*-140-*-*-*-*-*-*=STD_FONT,
//                     -*-courier-*-r-*--*-140-*-*-*-*-*-*=BUTTON_FONT",
//    (char*)"*plotStuff*width: 350",
//    (char*)"*plotStuff*height: 350",

  NULL
};

const static int maximumNumberOfMenuEntries = 10000; // *wdh* 060324 500;

static Display     *dpy;

static int OGLActiveWindow=-1, readyToPick=0, pickOption=0, pickWindow = -99;
static real xRubberBandMin, xRubberBandMax, yRubberBandMin, yRubberBandMax;

// ================================================================================
// The user can specify the locations for the command,graphics and dialog window.
//   -1 = use default
static int commandWindowNx=-1, commandWindowNy=-1;
static int graphicsWindowNx=-1, graphicsWindowNy=-1;
static int dialogNx=-1, dialogNy=-1;
static int dialogOffsetNx=10, dialogOffsetNy=0;  // shift new dialogs by this amount 
static int currentDialogNx=0, currentDialogNy=0;  // increment this as dialogs are created

// remember the last directory in the file selection dialog
static XmString fileSelectionDirectory = NULL;

static int exitEventLoop = 0;     // flag to indicate when the event loop should be exited
static int menuItemChosen=-999;   // number of the menu chosen
static char *menuNameChosen = NULL;     // name of the menu chosen


// These next variables are needed to support overlay visuals on the SGI
// The popup and pull-down menus are displayed in the overlay visual and then
// the screen does not need to be re-drawn when the menu goes away. (This
// behaviour is automatic on Sun's !)
static Visual *overlayVisual = NULL; 
static int overlayDepth;
static Colormap overlayColormap;
static  GL_GraphicsInterface *graphicsInterfacePointer=NULL;

//---------------UTILITIES
static int   max( int i1, int i2 ){ return i1>i2 ? i1 : i2; }
static int   min( int i1, int i2 ){ return i1<i2 ? i1 : i2; }
static float max( float i1, float i2 ){ return i1>i2 ? i1 : i2; }
// static float min( float i1, float i2 ){ return i1<i2 ? i1 : i2; }

static double max( double i1, double i2 ){ return i1>i2 ? i1 : i2; }
// static double min( double i1, double i2 ){ return i1<i2 ? i1 : i2; }

static double max( double i1, float i2 ){ return i1>i2 ? i1 : i2; }
// static double min( double i1, float i2 ){ return i1<i2 ? i1 : i2; }

// bitmaps
#define down_width 16
#define down_height 16
static unsigned char down_bits[] = {
   0xff, 0xff, 0x01, 0x80, 0x01, 0xc0, 0x01, 0xa0, 0xf1, 0xcf, 0xf1, 0xaf,
   0xe1, 0xc7, 0xe1, 0xa7, 0xc1, 0xc3, 0xc1, 0xa3, 0x81, 0xc1, 0x81, 0xa1,
   0x01, 0xc0, 0xa9, 0xaa, 0x55, 0xd5, 0xff, 0xff};

#define up_width 16
#define up_height 16
static unsigned char up_bits[] = {
   0xff, 0xff, 0x01, 0x80, 0x01, 0xc0, 0x01, 0xa0, 0x81, 0xc1, 0x81, 0xa1,
   0xc1, 0xc3, 0xc1, 0xa3, 0xe1, 0xc7, 0xe1, 0xa7, 0xf1, 0xcf, 0xf1, 0xaf,
   0x01, 0xc0, 0xa9, 0xaa, 0x55, 0xd5, 0xff, 0xff};

#define right_width 16
#define right_height 16
static unsigned char right_bits[] = {
   0xff, 0xff, 0x01, 0x80, 0x01, 0xc0, 0x01, 0xa0, 0x31, 0xc0, 0xf1, 0xa0,
   0xf1, 0xc3, 0xf1, 0xaf, 0xf1, 0xcf, 0xf1, 0xa3, 0xf1, 0xc0, 0x31, 0xa0,
   0x01, 0xc0, 0xa9, 0xaa, 0x55, 0xd5, 0xff, 0xff};

#define left_width 16
#define left_height 16
static unsigned char left_bits[] = {
   0xff, 0xff, 0x01, 0x80, 0x01, 0xc0, 0x01, 0xa0, 0x01, 0xcc, 0x01, 0xaf,
   0xc1, 0xcf, 0xf1, 0xaf, 0xf1, 0xcf, 0xc1, 0xaf, 0x01, 0xcf, 0x01, 0xac,
   0x01, 0xc0, 0xa9, 0xaa, 0x55, 0xd5, 0xff, 0xff};

#define zp_width 16
#define zp_height 16
static unsigned char zp_bits[] = {
   0x00, 0x00, 0xc0, 0x07, 0x30, 0x18, 0x18, 0x30, 0x28, 0x28, 0x44, 0x44,
   0x84, 0x42, 0x04, 0x41, 0x84, 0x42, 0x44, 0x44, 0x28, 0x28, 0x18, 0x30,
   0x30, 0x18, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00};

#define zm_width 16
#define zm_height 16
static unsigned char zm_bits[] = {
   0x00, 0x00, 0xc0, 0x07, 0x30, 0x18, 0x08, 0x20, 0x08, 0x20, 0x04, 0x40,
   0x84, 0x43, 0x84, 0x43, 0x84, 0x43, 0x04, 0x40, 0x08, 0x20, 0x08, 0x20,
   0x30, 0x18, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00};

#define yrotp_width 16
#define yrotp_height 16
static unsigned char yrotp_bits[] = {
   0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x10, 0x80, 0x38,
   0x84, 0x7c, 0x84, 0x10, 0x8c, 0x18, 0xf8, 0x0f, 0x80, 0x00, 0x80, 0x00,
   0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00};

#define yrotm_width 16
#define yrotm_height 16
static unsigned char yrotm_bits[] = {
   0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x08, 0x01, 0x1c, 0x01,
   0x3e, 0x21, 0x08, 0x21, 0x18, 0x31, 0xf0, 0x1f, 0x00, 0x01, 0x00, 0x01,
   0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00};

#define xrotp_width 16
#define xrotp_height 16
static unsigned char xrotp_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x03, 0x00, 0x02, 0x00, 0x02,
   0x00, 0x02, 0xfe, 0x7f, 0x00, 0x02, 0x00, 0x02, 0x40, 0x02, 0x60, 0x03,
   0xf0, 0x01, 0x60, 0x00, 0x40, 0x00, 0x00, 0x00};

#define xrotm_width 16
#define xrotm_height 16
static unsigned char xrotm_bits[] = {
   0x00, 0x00, 0x40, 0x00, 0x60, 0x00, 0xf0, 0x01, 0x60, 0x03, 0x40, 0x02,
   0x00, 0x02, 0x00, 0x02, 0xfe, 0x7f, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02,
   0x00, 0x03, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00};

#define zrotp_width 16
#define zrotp_height 16
static unsigned char zrotp_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x07, 0xf8, 0x1f, 0x78, 0x18,
   0x78, 0x30, 0xf8, 0x30, 0x00, 0x30, 0x00, 0x30, 0x10, 0x30, 0x30, 0x18,
   0xe0, 0x1f, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00};

#define zrotm_width 16
#define zrotm_height 16
static unsigned char zrotm_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x27, 0xf0, 0x3f, 0x30, 0x3c,
   0x18, 0x3c, 0x18, 0x3e, 0x18, 0x00, 0x18, 0x00, 0x18, 0x10, 0x30, 0x18,
   0xf0, 0x0f, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00};

#define zoom_in_width 16
#define zoom_in_height 16
static unsigned char zoom_in_bits[] = {
   0x00, 0x00, 0x80, 0x07, 0x60, 0x18, 0x10, 0x20, 0x10, 0x23, 0x08, 0x43,
   0xc8, 0x4f, 0xc8, 0x4f, 0x08, 0x43, 0x10, 0x23, 0x10, 0x20, 0x78, 0x18,
   0x9c, 0x07, 0x0e, 0x00, 0x06, 0x00, 0x00, 0x00};

#define zoom_out_width 16
#define zoom_out_height 16
static unsigned char zoom_out_bits[] = {
   0x00, 0x00, 0x80, 0x07, 0x60, 0x18, 0x10, 0x20, 0x10, 0x20, 0x08, 0x40,
   0xc8, 0x4f, 0xc8, 0x4f, 0x08, 0x40, 0x10, 0x20, 0x10, 0x20, 0x78, 0x18,
   0x9c, 0x07, 0x0e, 0x00, 0x06, 0x00, 0x00, 0x00};

#define reset_width 16
#define reset_height 16
static unsigned char reset_bits[] = {
   0x00, 0x00, 0x80, 0x01, 0xc0, 0x03, 0x60, 0x06, 0x30, 0x0c, 0x18, 0x18,
   0x0c, 0x30, 0xc8, 0x13, 0x48, 0x12, 0x48, 0x12, 0x48, 0x12, 0xc8, 0x13,
   0x08, 0x10, 0x08, 0x10, 0xf8, 0x1f, 0x00, 0x00};

#define rotpnt_width 16
#define rotpnt_height 16
static unsigned char rotpnt_bits[] = {
   0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0xc8, 0x07, 0xf8, 0x1f, 0x78, 0x19,
   0x78, 0x31, 0xf8, 0x31, 0x00, 0x31, 0xfe, 0xff, 0x10, 0x31, 0x30, 0x19,
   0xe0, 0x1f, 0xc0, 0x07, 0x00, 0x01, 0x00, 0x00};

static XImage down_ximage, up_ximage, left_ximage, right_ximage, yrotp_ximage, yrotm_ximage, 
  xrotp_ximage, xrotm_ximage, zrotp_ximage, zrotm_ximage, zoom_in_ximage, zoom_out_ximage,
  zp_ximage, zm_ximage, reset_ximage, rotpnt_ximage;

// toggle button for axes (in the View pulldown menu for each graphics window)
// macro for casting aStrings to char * without getting a memory leak
#define SC (char *)(const char *)

//---------------PROTOTYPES

extern "C" 
{
  void detectOverlaySupport(Display *dpy, Visual *overlayVisual, int *overlayDepth, Colormap *overlayColormap);
};

void
clippingPlanesDialog(Widget pb, XtPointer client_data, XtPointer call_data);
void
annotateDialog(Widget pb, XtPointer client_data, XtPointer call_data);
void
viewCharacteristicsDialog(Widget pb, XtPointer client_data, XtPointer call_data);

void 
draw(Widget w);
void
graphicsInit(Widget w, XtPointer data, XtPointer callData);
void 
exposeOrResize(Widget w, XtPointer data, XtPointer callData);
void 
map_state_changed(Widget w, XtPointer data, XEvent * event, Boolean * cont);
void
popupCallback(Widget menuItem, XtPointer client_data, XtPointer call_data );
void
exec_cmd( Widget widget, XtPointer client_data, XtPointer call_data );
void
inputCommand(Widget cmd_widget, XtPointer client_data, XtPointer call_data );
void
postIt(Widget cmd_widget, XtPointer client_data, XEvent *event, char *dum);
void
buttonCallback(Widget widget, XtPointer client_data, XtPointer call_data );
static void
userButtonCallback(Widget widget, XtPointer client_data, XtPointer call_data );
void  
eventLoop();

void
drawAreaInput(Widget w, XtPointer clientData, XtPointer callData);
static void
select_cmd( Widget widget, XtPointer client_data, XtPointer call_data );
static int
window_exists(Window winid, int *pos);
static int
windowExistsTop(Window winid, int *pos);


// ..VDL:
// ----Wrappers for Viewport changes -- required by the PowerWall version

// graphics_setFrustum( left, right, bottom, top, near, far )
// ----------------------------------------------------------
// ..calls glFrustum on the Motif widget & the Vdl equivalent
//  * powerwall codes are NOT allowed to call glPerspective directly
//
//    NOT USED IN CURRENT VERSION (Feb 10, 2001)
//
// who to blame: pf
void 
graphics_setFrustum( GLdouble left,   GLdouble right,
		     GLdouble bottom, GLdouble top,
		     GLdouble near,   GLdouble far)
{
#ifdef USE_POWERWALL
  //cout << "++PW: graphics_setFrustum called - ";
  //cout << "with VDL.\n";
  vdlCanvasFrustum( canvas, left, right, bottom, top, near, far );
  debug_sniff_for_opengl_errors( "graphics_setFrustum" );
#endif

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum( left, right, bottom, top, near, far );
  glMatrixMode(GL_MODELVIEW);
}

// graphics_setOrtho( left, right, bottom, top, near, far )
// ---------------------------------------------------------
// calls glOrtho on the Motif widget & the Vdl equivalent
//  * powerwall codes are NOT allowed to call glOrtho directly
//
// who to blame: pf
void 
graphics_setOrtho( GLdouble left,   GLdouble right,
		   GLdouble bottom, GLdouble top,
		   GLdouble near,   GLdouble far)
{
#ifdef USE_POWERWALL
  //cout << "++PW: graphics_setOrtho called - ";
  //cout << "with VDL.\n";
  vdlCanvasOrtho( canvas, left, right, bottom, top, near, far );
  //vdlCanvasOrtho( canvas, left*1.875, 1.875*right, bottom, top, near, far ); //debug**pf
  //debug_sniff_for_opengl_errors( "graphics_setOrtho" );
#endif

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( left, right, bottom, top, near, far );
  glMatrixMode(GL_MODELVIEW);
}


// graphics_setOrthoKeepAspectRatio( aRatio, magFactor, left, right, bottom, top, near, far )
// -----------------------------------------------------------------------
// calls glOrtho on the Motif widget & the Vdl equivalent
//  * maintains the correct aspect ratio on the power wall
//    even if the vdl canvas & overture GL canvas have
//    different aspect ratios
//  * powerwall codes are NOT allowed to call glOrtho directly
//
// who to blame: pf
void 
graphics_setOrthoKeepAspectRatio( GLdouble aRatio, GLdouble magFactor,
				  GLdouble left,   GLdouble right,
				  GLdouble bottom, GLdouble top,
				  GLdouble near,   GLdouble far)
{
#ifdef USE_POWERWALL
  //printf("++PW: graphics_setOrthoKeepAspectRatio called:");
  //printf(" a=%f, left=%d, right=%d, bottom=%d, top=%d.\n",
  //	 double(aRatio), double(left), double(right), double(bottom), double(top));

  GLdouble vdl_left, vdl_right, vdl_bottom, vdl_top;
  if ( vdl_aspectRatio > 1.0 )
  {
    vdl_right =  (vdl_aspectRatio/aRatio)*right;
    vdl_left  =  (vdl_aspectRatio/aRatio)*left;
    vdl_bottom = bottom;
    vdl_top    = top;
  }
  else
  {
    vdl_right =  right;
    vdl_left  =  left;
    vdl_bottom = (aRatio/vdl_aspectRatio)*bottom;
    vdl_top    = (aRatio/vdl_aspectRatio)*top;
  }
  vdlCanvasOrtho( canvas, vdl_left, vdl_right, vdl_bottom, vdl_top, near, far );
  //debug_sniff_for_opengl_errors( "graphics_setOrtho" );
#endif

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

// test
//  printf("setOrtho: near=%g, far=%g\n", near, far);
  
//    printf("setOrtho: left/mF=%g, right/mF=%g, bot/mF=%g, top/mF=%g\n", left/magFactor, 
//  	 right/magFactor, bottom/magFactor, top/magFactor);
  
  glOrtho( left/magFactor, right/magFactor, bottom/magFactor, top/magFactor, near, far );
  glMatrixMode(GL_MODELVIEW);
}

//  graphics_setPerspective( fovy, aspect, near, far)
//  -------------------------------------------------
//  ..Wrapper for gluPerspective, calls it on the Motif-GL canvas, and on PowerWall
//  * powerwall codes are NOT allowed to call gluPerspective directly
//
//  who to blame: pf
void 
graphics_setPerspective ( GLdouble fovy,   GLdouble aspect,
			  GLdouble near,   GLdouble far)
{
#ifdef USE_POWERWALL
  //cout << "++PW: graphics_setPerspective called - ";
  //cout << "with VDL.\n";
  vdlCanvasPerspective( canvas, fovy, aspect, near, far);
  //debug_sniff_for_opengl_errors( "graphics_setPerspective" );
#endif

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( fovy, aspect, near, far );
  glMatrixMode(GL_MODELVIEW);
}

/*
 * ============ VDL CODE =======================================
 */

#ifdef USE_POWERWALL
extern void vdlInit( GL_GraphicsInterface *giPointer ); // defined in GL_GraphicsInterface.C

// vdlInitWin( vdlTileP tile, void *data)
// --------------------------------------
// ..vdl callback; passed into vdlCanvasCreate, or vdlCanvasCreateShared
// ..guaranteed to be called exactly once for each vdl tile
//
// who to blame: pf
static void
vdlInitWin( vdlTileP tile, void *data)
{
  //cout << "VDL:vdlInitWin  called\n";
  if ( graphicsInterfacePointer == NULL )
  { 
    cout << "ERROR: mogl.C <vdlInitWin>. The graphicsInterfacePointer == NULL\n";
  } else {
    vdlInit( graphicsInterfacePointer ); // must call wrapper in GL_GraphicsInterface
    //debug_sniff_for_opengl_errors( "vdlInitWin" );
  }
}

// pw_args( int argc, char **argv)
// -------------------------------
// ..parses the command line, looks for -vdl <filename>
// ..sets global 'vdlFile' = the argument
// ..not used anymore
//
// who to blame: pf
//
static void pw_args(int argc, char **argv) /* pw=PowerWall */
{
    GLint i;

    for (i = 1; i < argc; i++) {
       if (strcmp(argv[i], "-vdl") == 0) {
           if (i == argc-1) {
              fprintf(stderr, "-vdl argument requires parameter\n");
              exit(1);
           }
           vdlFile = argv[i+1];
	   printf("vdl config file= %s\n",vdlFile);
           i++;
       }
    }
}

// char *vdlGetConfigurationFile()
// -------------------------------
// ..if the environmental variable $VDLCONFIG is defined,
//   copies it's value into a string & returns a pointer
// ..VDLCONFIG should contain the name of the vdl config file
//
// who to blame: pf
//
static
char * vdlGetConfigurationFile()
{
  char *vdlFile00 = getenv("VDLCONFIG");
  if ( vdlFile00 != NULL ) 
  {
    printf("++ VDL configuration file= %s.\n",vdlFile00 );
  } else {
    printf("++ VDL configuration file= <NULL>, using defaults.\n");
  }
  return (vdlFile00);
}


// vdlDraw( vdlTileP tile, void *data)
// --------------------------------------
// ..vdl callback; passed into vdlCanvasCreate, or vdlCanvasCreateShared
// ..called for each vdl tile to render it
//
// who to blame: pf
static void 
vdlDraw(vdlTileP tile, void *data)
{
#ifdef PF_DEBUG
  printf(" <draw> ");
#endif
  displayFunction(graphicsInterfacePointer, vdlActiveWindowNumber);
  debug_sniff_for_opengl_errors( "vdlDraw" );
}

// debug_sniff_for_opengl_errors(char *name)
// -----------------------------------------
// ..looks for GL error messages & displays them
// (C) Mark Kilgaard, from his book
//
void debug_sniff_for_opengl_errors(char *name)
{
  int error;
  
  while( (error = glGetError()) != GL_NO_ERROR ) 
    {
      //    fprintf(stderr, "*** VDL:  GL error in <%s>: %s\n\n", 
      //	    name, gluErrorString( error ));
    fprintf(stderr, "*** VDL:  GL error in <%s>: # %i\n\n", 
	    name, error );
    }
}
#endif

// =============== END VDL code ====================================

//..GL
static void 
display(GL_GraphicsInterface *giPointer /* =NULL */, const int & win_number /* =0 */)
// default display function
{
  printf("Mogl::ERROR: default display function called. This should not happen!\n");
}

//..GL
static void 
resize(GL_GraphicsInterface *giPointer /* =NULL */, const int & win_number /* =0 */)
// default resize function
{
  printf("Mogl::ERROR: default resize function called. This should not happen!\n");
}


//..GL
void
moglDisplay(int win)
// redraw the OpenGL window
{
  if (0 <= win && win < OGLNWindows)
    draw(OGLWindowList[win]->plotStuff);
}


//..GL
void
setMenuNameChosen( const char* answer )
// Assign the global variable menuNameChosen to equal answer
{
  int length = strlen(answer);
  if( !menuNameChosen || length > strlen(menuNameChosen) )
  {
    if (menuNameChosen) delete [] menuNameChosen;
    menuNameChosen= new char[length+1];
  } 
  strcpy(menuNameChosen,answer);
}  

//..GL
void
setMenuChosen( const int & menuItem, char* answer )
// Assign the global variable menuNameChosen to equal answer
{
  menuItemChosen = menuItem;
  setMenuNameChosen(answer);
  exitEventLoop=TRUE;
}  



//..Motif
void 
getCursor( real & x, real & y )
// Wait for the user to choose a cursor position
{
  eventLoop();

  x=xRubberBandMax;
  y=yRubberBandMax;
}

//============================================MOTIF-EXTERNAL

static void
initXimage( XImage & ximage, int width, int height, const unsigned char *bitmap, const char *name )
{
  ximage.width = width;
  ximage.height = height;
  ximage.data = (char*)bitmap;
  ximage.xoffset = 0;
  ximage.format = XYBitmap;
  ximage.byte_order = MSBFirst;
  ximage.bitmap_pad = 8;
  ximage.bitmap_bit_order = LSBFirst;
  ximage.bitmap_unit = 8;
  ximage.depth = 1;
  ximage.bytes_per_line = 2;
  ximage.obdata = NULL;
  XmInstallImage(&ximage, (char*)name);
}


//..MOTIF --- mixes Motif & GL initialization
void 
moglInit(int & argc, char *argv[], 
	 const aString &windowTitle, 
	 aString fileMenuItems[],
	 aString helpMenuItems[],
	 WindowProperties &wProp)
//===========================================================================
// /Purpose: Initialize the graphics interface
//
// /fileMenuItems (input): These items appear in the File menu. This array
//   of strings should be terminated by "".
// /helpMenuItems (input): These items appear in the Help menu. This array
//   of strings should be terminated by "".
//
// /Author: WDH \& AP
//===========================================================================
{
  if (moglInitialized) return; // only do this once!

  
#ifdef USE_POWERWALL
  //VDL initialization code
  //------------------------------
  // * Init X windows to thread safe
  // * Read in the vdl config file (from the file $VDLCONFIG)
  //
  // who to blame: pf
  //
  XInitThreads();
#ifdef __sun
  glXInitThreadsSUN();     // wdh: require thread safe version on the sun
#endif
  vdlFile =  vdlGetConfigurationFile();  // vdlFile=$VDLCONFIG, defined on top of mogl.C
  vdlConfig(vdlFile);
  vdl_width  =  vdlVirtualWidth(0);
  vdl_height =  vdlVirtualHeight(0);
#ifdef VDL_WIDTH
  vdl_width  = atoi(getenv( "VDL_WIDTH" ));
#endif
#ifdef VDL_HEIGHT
  vdl_height = atoi(getenv( "VDL_HEIGHT" ));
#endif
  vdl_aspectRatio = GLdouble(vdl_width)/GLdouble(vdl_height);
  cout << "** Virtual VDL display  size: width= "<< vdl_width
       << ", height= "<< vdl_height <<endl;
#endif    

  int n=0;

  XtSetLanguageProc(NULL, NULL, NULL);

  // **** create a prompt and command area in a separate window *****

  char *title = new char[windowTitle.length()+8];
  strcpy(title,"*title:");
  strcat(title,SC windowTitle.c_str());
  fallbackResources[0]=title;

  textw =  XtAppInitialize(&app, "MotifOpenGLGI", NULL, 0, &argc, argv,
			   fallbackResources, NULL, 0 );

  delete [] title;

// initialize bitmaps
  initXimage(down_ximage, down_width, down_height, down_bits, "Down");
  initXimage(up_ximage, up_width, up_height, up_bits, "Up");
  initXimage(left_ximage, left_width, left_height, left_bits, "Left");
  initXimage(right_ximage, right_width, right_height, right_bits, "Right");
  initXimage(zp_ximage, zp_width, zp_height, zp_bits, "Out");
  initXimage(zm_ximage, zm_width, zm_height, zm_bits, "In");

  initXimage(yrotp_ximage, yrotp_width, yrotp_height, yrotp_bits, "Yrotp");
  initXimage(yrotm_ximage, yrotm_width, yrotm_height, yrotm_bits, "Yrotm");
  initXimage(xrotp_ximage, xrotp_width, xrotp_height, xrotp_bits, "Xrotp");
  initXimage(xrotm_ximage, xrotm_width, xrotm_height, xrotm_bits, "Xrotm");
  initXimage(zrotp_ximage, zrotp_width, zrotp_height, zrotp_bits, "Zrotp");
  initXimage(zrotm_ximage, zrotm_width, zrotm_height, zrotm_bits, "Zrotm");

  initXimage(zoom_in_ximage, zoom_in_width, zoom_in_height, zoom_in_bits, "Zoom_In");
  initXimage(zoom_out_ximage, zoom_out_width, zoom_out_height, zoom_out_bits, "Zoom_Out");
  initXimage(reset_ximage, reset_width, reset_height, reset_bits, "Reset");
  initXimage(rotpnt_ximage, rotpnt_width, rotpnt_height, rotpnt_bits, "Rotpnt");
  
  dpy = XtDisplay(textw);
  Screen *xScreen = DefaultScreenOfDisplay( dpy );

  screenWidth  = WidthOfScreen(xScreen);
  screenHeight = HeightOfScreen(xScreen);

  Dimension borderWidth;
  XtVaGetValues(textw,
		XmNborderWidth, &borderWidth, NULL);

  // printf("Command window border width: %i, screenHeight=%i, commandWindowHeight=%i\n", borderWidth,
  //            screenHeight,wProp.commandWindowHeight );

  // Save some window properties into local variables to this file:
  commandWindowNx =wProp.commandWindowNx;   commandWindowNy =wProp.commandWindowNy;
  graphicsWindowNx=wProp.graphicsWindowNx;  graphicsWindowNy=wProp.graphicsWindowNy;
  dialogNx        =wProp.dialogNx;                 dialogNy =wProp.dialogNy;
  dialogOffsetNx  =wProp.dialogOffsetNx;     dialogOffsetNy =wProp.dialogOffsetNy;


  if (wProp.commandWindowWidth == -1)
    wProp.commandWindowWidth = screenWidth-2*borderWidth;
  else
    wProp.commandWindowWidth = max(500, min(wProp.commandWindowWidth, screenWidth-2*borderWidth));
  
  if (wProp.commandWindowHeight == -1)
    wProp.commandWindowHeight = 200;
  else
    wProp.commandWindowHeight = max(100, min(wProp.commandWindowHeight, screenHeight/2));

 Dimension xOffset=0, yOffset=screenHeight-wProp.commandWindowHeight;
 if( commandWindowNx >=0 )
    xOffset = commandWindowNx;
  if( commandWindowNy >=0 )
    yOffset = commandWindowNy;
  // printF("mogl: commandWindowNy=%d, yOffset=%d\n",commandWindowNy,yOffset);
  
  XtVaSetValues(textw,
		XmNbaseWidth, 1,
		XmNbaseHeight, 1, 
		XmNminHeight, 100, 
		XmNminWidth, 500, 
		XmNheight, wProp.commandWindowHeight, 
		XmNwidth, wProp.commandWindowWidth,
                XmNx, xOffset,
                XmNy, yOffset,
		NULL);

  showRubberBandBox=wProp.showRubberBandBox;    // *wdh* 020704
  showCommandHistory=wProp.showCommandHistory;  // *wdh* 090809
  showPrompt=wProp.showPrompt;                  // *wdh* 090809

  // look for an overlay visual (SGI's) to put menus into
  detectOverlaySupport(dpy, overlayVisual, &overlayDepth,  &overlayColormap);

  bulletin = XtVaCreateWidget("bullet", xmFormWidgetClass, 
			      textw, NULL);

// create children for the form: One menubar, two scrollable text windows, 
// one label and one editable text string.

  n=0;
  int nColumns = wProp.commandWindowWidth/20; // It would be nicer to also change the number of columns after a resize!
  
  XtSetArg(scrolledTextArgs[n], XmNrows, 5); n++;
  XtSetArg(scrolledTextArgs[n], XmNcolumns, nColumns); n++;
  XtSetArg(scrolledTextArgs[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(scrolledTextArgs[n], XmNeditable, False); n++;
  XtSetArg(scrolledTextArgs[n], XmNwordWrap, True); n++;
  XtSetArg(scrolledTextArgs[n], XmNscrollHorizontal, False); n++;
  XtSetArg(scrolledTextArgs[n], XmNcursorPositionVisible, False); n++;
  
  promptText = XmCreateScrolledText(bulletin, (char*)"outputWindow", scrolledTextArgs, n);
#ifndef OV_USE_LESSTIF
  XmTextSetAddMode(promptText, True);
#endif
  
  XtManageChild(promptText);

  Widget commandText = XtVaCreateManagedWidget("Command:",xmLabelWidgetClass, bulletin, 
					       NULL);
  command_w = XtVaCreateManagedWidget("editableText", xmTextWidgetClass, bulletin, 
				      XmNrows, 1, NULL); 
  XtAddCallback(command_w,XmNactivateCallback, exec_cmd, NULL );
  

  n=0;
  XtSetArg(scrolledTextArgs[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED); n++;
  XtSetArg(scrolledTextArgs[n], XmNlistSizePolicy, XmCONSTANT); n++;
  XtSetArg(scrolledTextArgs[n], XmNselectionPolicy, XmSINGLE_SELECT); n++;

  oldCommandList = XmCreateScrolledList(bulletin, (char*)"oldCommands", scrolledTextArgs, n);
  XtAddCallback(oldCommandList, XmNsingleSelectionCallback, select_cmd, NULL);
  XtAddCallback(oldCommandList, XmNdefaultActionCallback, select_cmd, NULL);
  XtManageChild(oldCommandList);
  
//-------------------------------------CREATE MENUBAR--------------------------------

  menubar = XmCreateMenuBar(bulletin, (char*)"menubar", NULL, 0);

// ************** File menu **********************
  n=0;
  if( overlayVisual )
  {
    XtSetArg(menuPaneArgs[n], XmNvisual, overlayVisual); n++;
    XtSetArg(menuPaneArgs[n], XmNdepth,  overlayDepth); n++;
    XtSetArg(menuPaneArgs[n], XmNvisual, overlayColormap); n++;
  }
    
  Widget menupane = XmCreatePulldownMenu(menubar, (char*)"menupane", menuPaneArgs, n);

  int * winInfo_ = (int *) malloc( sizeof(int) );
  XtPointer winInfo = (XtPointer) winInfo_;
  *winInfo_ = -1; // the command window has number -1

  int i;
  Widget btn;
  for( i=0; fileMenuItems[i] != ""; i++ )
  {
    btn = XtVaCreateManagedWidget(SC fileMenuItems[i].c_str(), xmPushButtonWidgetClass, menupane, 
				  XmNuserData, winInfo, NULL);
    intptr_t idata = -(i+1);  // do this to safely cast to void*
    XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)(idata) ); // the position in the menu is the userdata
    // *wdh* 2017/04/21 XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)(-(i+1)) ); // the position in the menu is the userdata
  }
    
  XtSetArg(args[0], XmNsubMenuId, menupane); // the menupane that is attached to the CascadeButton
  Widget cascade = XmCreateCascadeButton(menubar, (char*)"File", args, 1);
  XtManageChild(cascade);

// ***************** Help menu *********************

  menupane = XmCreatePulldownMenu(menubar, (char*)"menupane", menuPaneArgs, n);

  for( i=0; helpMenuItems[i] != ""; i++ )
  {
    btn = XtVaCreateManagedWidget(SC helpMenuItems[i].c_str(), xmPushButtonWidgetClass, menupane, 
				  XmNuserData, winInfo, NULL);
    intptr_t idata = -(i+1);  // do this to safely cast to void*
    XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)(idata) ); // the position in the menu is the userdata
    // *wdh* 2017/04/21  XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)(-(i+1)) ); // the position in the menu is the userdata
  }
    
  XtSetArg(args[0], XmNsubMenuId, menupane); // the menupane that is attached to the CascadeButton
  cascade = XmCreateCascadeButton(menubar, (char*)"Help", args, 1);
  XtManageChild(cascade);

  XtVaSetValues(menubar, 
		XmNmenuHelpWidget, cascade,
		NULL);
// activate the menubar
  XtManageChild(menubar);

// get the sizes of commandText and command_w to properly position them
  Dimension stringHeight, textHeight;
  XtVaGetValues(commandText,
		XmNheight, &stringHeight, NULL);
  XtVaGetValues(command_w,
		XmNheight, &textHeight, NULL);
//  printf("stringHeight: %i, textHeight: %i\n", stringHeight, textHeight);

//
// position the widgets on the bulletin form
//
  

  XtVaSetValues(menubar, 
		XmNtopAttachment, XmATTACH_FORM, 
		XmNrightAttachment, XmATTACH_FORM, 
		XmNleftAttachment, XmATTACH_FORM, 
		NULL);
  XtVaSetValues(XtParent(promptText), 
		XmNleftAttachment, XmATTACH_FORM, 
		XmNtopAttachment, XmATTACH_WIDGET, 
		XmNtopWidget, menubar, 
		XmNbottomAttachment, XmATTACH_WIDGET,
		XmNbottomWidget, commandText, 
// make sure there is enough room for the editable text
		XmNbottomOffset, max(1,textHeight-stringHeight), 
		NULL);
  XtVaSetValues(XtParent(oldCommandList), 
		XmNleftAttachment, XmATTACH_WIDGET, 
		XmNleftWidget, XtParent(promptText),
		XmNrightAttachment, XmATTACH_FORM, 
		XmNtopAttachment, XmATTACH_WIDGET, 
		XmNtopWidget, menubar, 
		XmNbottomAttachment, XmATTACH_FORM, 
		NULL);
  XtVaSetValues(commandText, // this is the text string "Command"
		XmNleftAttachment, XmATTACH_FORM,
  		XmNbottomAttachment, XmATTACH_FORM, 
  		XmNbottomOffset, 1,
		NULL);
  XtVaSetValues(command_w, // this is the editable command text
		XmNrightAttachment, XmATTACH_WIDGET, 
		XmNrightWidget, XtParent(oldCommandList),
		XmNbottomAttachment, XmATTACH_FORM, 
		XmNbottomOffset, 1,
  		XmNleftAttachment, XmATTACH_WIDGET, 
  		XmNleftWidget, commandText,
		NULL);
  
//
// Done making menus
//
  XtManageChild(bulletin);

  XtRealizeWidget(textw);
  
// build a watch cursor
  cursor          = XCreateFontCursor(dpy,XC_watch);
  translateCursor = XCreateFontCursor(dpy,XC_fleur);
  rotateCursor    = XCreateFontCursor(dpy,XC_exchange);
  zoomCursor      = XCreateFontCursor(dpy,XC_sizing);
  boxCursor       = XCreateFontCursor(dpy,XC_tcross);
  boxSpiralCursor = XCreateFontCursor(dpy,XC_box_spiral);
  dotCursor       = XCreateFontCursor(dpy,XC_dot);
  crossCursor     = XCreateFontCursor(dpy,XC_tcross);

  moglInitialized = 1;

}

static void
cancelFileSB(Widget widget, XtPointer client_data, XtPointer call_data)
{
//  char *filename;
//  XmFileSelectionBoxCallbackStruct *cbs =
//    (XmFileSelectionBoxCallbackStruct *) call_data;

//  XtUnrealizeWidget( fsDialog );

  setMenuNameChosen("");
  menuItemChosen=-1;
  exitEventLoop=TRUE;
}

static void
okFileSB(Widget widget, XtPointer client_data, XtPointer call_data)
{
  char *filename;
  XmFileSelectionBoxCallbackStruct *cbs =
    (XmFileSelectionBoxCallbackStruct *) call_data;

//  XtUnrealizeWidget( fsDialog );

  char retString[200];

  if (!XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &filename) ) /* internal error */
    sprintf(retString,"");
  else if (!filename)
  {
    sprintf(retString,"");
    XtFree( filename );
  }
  else
  {
    sprintf(retString,"%s", filename);
    XtFree(filename);
  }
  
  setMenuNameChosen(retString);
  menuItemChosen=-1;
  exitEventLoop=TRUE;
  
}


void
moglOpenFileSB(char *extension /* NULL */)
{
  Arg fsArgs[3];
  int nArgs=0;
  char pattern[80];
  XmString p;
// make the file selection modal, i.e.,
// the dialog box must be closed before any other part of the GUI can be used.
  XtSetArg(fsArgs[nArgs], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); nArgs++;
  if (fileSelectionDirectory)
  {
    XtSetArg(fsArgs[nArgs], XmNdirectory, fileSelectionDirectory); 
    nArgs++;
  }
  sprintf(pattern, "*%s", (extension? extension: ""));
  p = XmStringCreateLocalized(pattern);
  XtSetArg(fsArgs[nArgs], XmNpattern, p); nArgs++;
  
  fsDialog = XmCreateFileSelectionDialog(textw, (char*)"filesb", fsArgs, nArgs);
  XmStringFree(p);
// remove the help button
  Widget helpButton = XmFileSelectionBoxGetChild(fsDialog, XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(helpButton);

// change the directory
//    if (fileSelectionDirectory)
//    {
//      XtVaSetValues(fsDialog, XmNdirectory, fileSelectionDirectory, NULL);
//    }
// change the search filter 
//    if (extension)
//    {
//      char pattern[80];
//      sprintf(pattern, "*%s", extension);
//      XmString p = XmStringCreateLocalized(pattern);
//      XtVaSetValues(fsDialog, XmNpattern, p, NULL);
//      XmStringFree(p);
//    }

  XtAddCallback(fsDialog, XmNokCallback, okFileSB, NULL);
  XtAddCallback(fsDialog, XmNcancelCallback, cancelFileSB, NULL);
  XtManageChild(fsDialog);
}

void
moglCloseFileSB()
{
  if (fsDialog) 
// record the current value of XmNdirectory
    XtVaGetValues(fsDialog, XmNdirectory, &fileSelectionDirectory, NULL);
  
    XtDestroyWidget( fsDialog );
//    XtUnrealizeWidget( fsDialog );
  fsDialog = NULL;
}

void
moglCreateMessageDialog(aString msg, MessageTypeEnum type)
{
  Arg fsArgs[1];
  Widget d;
  XmString m;
  unsigned char dt;
  char * title;
  switch (type)
  {
  case errorDialog:
    dt = XmDIALOG_ERROR;
    title = (char*)"Error";
    break;
  case warningDialog:
    dt = XmDIALOG_WARNING;
    title = (char*)"Warning";
    break;
  case informationDialog:
    dt = XmDIALOG_INFORMATION;
    title = (char*)"Information";
    break;
  case messageDialog:
  default:
    dt = XmDIALOG_MESSAGE;
    title = (char*)"Message";
    break;
  }
  
  d = XmCreateMessageDialog(textw, (char*)"message", NULL, 0);
  m = XmStringCreateLocalized(SC msg.c_str());
  XmString os = XmStringCreateLocalized((char*)"Close");
  XmString ts = XmStringCreateLocalized(title);
  XtVaSetValues(d, 
		XmNmessageString, m, 
		XmNokLabelString, os,
		XmNdialogType, dt,
		XmNdialogTitle, ts,
		NULL);
  XmStringFree(m);
  XmStringFree(os);
  XmStringFree(ts);
// remove the help and cancel buttons
  Widget cancelButton = XmMessageBoxGetChild(d, XmDIALOG_CANCEL_BUTTON);
  XtUnmanageChild(cancelButton);

  Widget helpButton = XmMessageBoxGetChild(d, XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(helpButton);
// display the dialog
  XtManageChild(d);
}


int 
moglSetTitle(int win_number, const aString &windowTitle)
{
  if (win_number >= 0 && win_number < OGLNWindows && windowTitle.length() > 0)
  {
    Widget toplevel = OGLWindowList[win_number]->toplevel;
    XtVaSetValues(toplevel, XtNtitle, SC windowTitle.c_str(), NULL);
  }
  else
  {
    printf("moglSetTitle: ERROR: illegal parameters win_number=%i, windowTitle=%s\n", win_number, SC windowTitle.c_str());
    return ERROR;
  }
  
  return OK;
}


int
makeGraphicsWindow(const aString &windowTitle, 
		   aString fileMenuItems[],
		   aString helpMenuItems[],
		   ClippingPlaneInfo & clippingPlaneInfo,
		   ViewCharacteristics & viewChar,
		   DialogData &hardCopyDialog,
		   PullDownMenu &optionMenu,
		   WindowProperties &wProp, 
                   int directRendering /* = 1 */  )
{
  // encode the window number in viewChar;
  viewChar.win_number = OGLNWindows;

  if( directRendering )
    preferDirectRendering=GL_TRUE;
  else
    preferDirectRendering=GL_FALSE;

  int border=6;
  int xOffset, yOffset;
  int wDim = screenWidth/2 - 2*border;
  int xWindowDim = wDim, yWindowDim = wDim; // **pf 

  //DEBUG CODE --- causes all X errors are reported immediately **pf
  // REMOVE once done debugging, this will slow the code **pf
  //XSynchronize(dpy, True);  //**pf   debug */

  // ---------- NEW window size code = same aspectratio as power wall
  // ---------- not used currently
#if 0
#ifdef USE_POWERWALL
  //..redo xWindowDim & yWindowDim so that aspect ratio
  //  is the same for the canvas & the power wall
  //  Who to blame: pf

  // Geometric constants **pf
  //  *these constants depend on the layout of the buttons
  //  *the following work for Overture version Feb 10, 2001
  //  *IF you change the graphics window code, these need to be recalculated
  const int xExtraSize = 71; // difference xWindowDim - 'width of t->plotStuff'
  const int yExtraSize = 47;
  const int minimumCanvasHeight = wDim-3*yExtraSize; // looks good on a SUN **pf
  //const int minimumCanvasHeight = 300;
  int canvasWidth        = (wDim- xExtraSize);
  int canvasHeight       = canvasWidth;

  if ( (vdl_aspectRatio < .3) || (vdl_aspectRatio > 3.0))
  {
    cout << "WARNING -- mogl, makeGraphicsWindow: "
	 << "extreme vdl_aspectRatio = " << vdl_aspectRatio << endl;
  } 
  else if (vdl_aspectRatio > 1.0)
  {
    //canvasHeight = max( canvasWidth/vdl_aspectRatio, minimumCanvasHeight);
    //canvasWidth  = vdl_aspectRatio*canvasHeight; 
    canvasHeight = canvasHeight/vdl_aspectRatio;
  } 
  else if (vdl_aspectRatio < 1.0 )
  {
    //canvasWidth  = max( vdl_aspectRatio*canvasWidth, minimumCanvasHeight);
    //canvasHeight = canvasWidth/vdl_aspectRatio;  
    canvasWidth = vdl_aspectRatio * canvasWidth;
  }

  xWindowDim = xExtraSize + canvasWidth;
  yWindowDim = yExtraSize + canvasHeight;
#endif
#endif
  
  if (wProp.graphicsWindowWidth == -1)
    wProp.graphicsWindowWidth = xWindowDim;
  else
    wProp.graphicsWindowWidth = max(370, min(screenWidth, wProp.graphicsWindowWidth));
  
  if (wProp.graphicsWindowHeight == -1)
    wProp.graphicsWindowHeight = yWindowDim;
  else
    wProp.graphicsWindowHeight = max(370, min(screenHeight, wProp.graphicsWindowHeight));
  

  //..changed to handle differing x & y dimensions of windows 
  //.. for the power wall **pf
// AP: We are building window # OGLNWindows
  yOffset = 10*OGLNWindows;
  xOffset = 1 + 10*OGLNWindows;
  
  if( graphicsWindowNx >=0 )
    xOffset += graphicsWindowNx;
  if( graphicsWindowNy >=0 )
  {
    yOffset += graphicsWindowNy;
  }
  

  WINDOW_REC *t;
  t = new WINDOW_REC;

  OGLCurrentWindow = OGLNWindows;
  OGLWindowList[OGLNWindows] = t;

  t->toplevel = XtVaAppCreateShell(NULL, "Class", topLevelShellWidgetClass, dpy, 
				   XtNtitle, SC windowTitle.c_str(), 
				   XtNallowShellResize, True,
				   XmNbaseWidth, 1,
				   XmNbaseHeight, 1,
				   XmNminHeight, 370, 
				   XmNminWidth, 370, 
				   XmNheight, wProp.graphicsWindowHeight, //changed from wDim, for VDL **pf  
				   XmNwidth,  wProp.graphicsWindowWidth, //changed from wDim  for VDL **pf
				   XmNx, xOffset,
				   XmNy, yOffset,
				   NULL);
  
//  printf("** XGraphics window %i: width %i, height %i\n",
//	 OGLNWindows, wProp.graphicsWindowWidth, wProp.graphicsWindowHeight); //**pf debug */

//  XSynchronize(dpy,True);  // for debugging

  /* find an OpenGL-capable RGB visual with depth buffer */
  t->vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
  t->doubleBuffer = GL_TRUE;
  if (t->vi == NULL) {
    t->vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
    if (t->vi == NULL)
      XtAppError(app, "no RGB visual with depth buffer");
    t->doubleBuffer = GL_FALSE;
  }

// *AP* the following stuff is done in the graphicsInit callback
//    /* create an OpenGL rendering context */
//    t->cx = glXCreateContext(dpy, t->vi, /* no display list sharing */ NULL,
//  			/* favor direct */ GL_TRUE);
//    if (t->cx == NULL)
//      XtAppError(app, "could not create rendering context");

  XtAddEventHandler(t->toplevel, StructureNotifyMask, False,  map_state_changed, NULL);
  
  t->mainw = XtVaCreateManagedWidget("main_w", xmMainWindowWidgetClass, t->toplevel,
				  XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE, NULL);
  
// we are making window # OGLNWindows. Append this number to all buttons
  int * winInfo_ = (int *) malloc( sizeof(int) );
  XtPointer winInfo = (XtPointer) winInfo_;
  *winInfo_ = OGLNWindows;

  //-------------------------------------CREATE MENUBAR--------------------------------
  // create menu bar
  t->menubar = XmCreateMenuBar(t->mainw, (char*)"menubar", NULL, 0);
  XtManageChild(t->menubar);

  // ************** File menu **********************
    
  int n=0;
  if( overlayVisual )
  {
    XtSetArg(menuPaneArgs[n], XmNvisual, overlayVisual);    n++;
    XtSetArg(menuPaneArgs[n], XmNdepth,  overlayDepth);    n++;
    XtSetArg(menuPaneArgs[n], XmNvisual, overlayColormap);  n++;
  }
    
  Widget menupane = XmCreatePulldownMenu(t->menubar, (char*)"menupane", menuPaneArgs, n);

  int i;
  Widget btn;
// AP: "activate" no longer works because of changes in getMatch
//    btn = XtVaCreateManagedWidget("activate", xmPushButtonWidgetClass, menupane, 
//  				XmNuserData, winInfo, NULL);
//    XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)-1 );

#ifdef USE_POWERWALL
  // add 'power wall' option -- current window is mirrored to vdl **pf
  btn = XtVaCreateManagedWidget("power wall", xmPushButtonWidgetClass, menupane, 
				XmNuserData, winInfo, NULL);
  XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)-1 );
#endif

  btn = XtVaCreateManagedWidget("Hardcopy...", xmPushButtonWidgetClass, menupane, 
				XmNuserData, winInfo, NULL);
// AP: need to add the widget pointer as client_data
  XtAddCallback(btn, XmNactivateCallback, showHardcopyDialog, hardCopyDialog.getWidget() );

// *wdh* removed 100912
//   btn = XtVaCreateManagedWidget("Movie...", xmPushButtonWidgetClass, menupane, 
// 				XmNuserData, winInfo, NULL);
//   // AP: need to add the widget pointer as client_data
//   XtAddCallback(btn, XmNactivateCallback, showHardcopyDialog, movieDialog.getWidget() );

  XtSetArg(args[0], XmNsubMenuId, menupane);
  Widget cascade = XmCreateCascadeButton(t->menubar, (char*)"File", args, 1);
  XtManageChild(cascade);

  // *************** view menu ***************
  menupane = XmCreatePulldownMenu(t->menubar, (char*)"menupane", menuPaneArgs, n);

  btn = XtVaCreateManagedWidget("clipping planes...", xmPushButtonWidgetClass, menupane, 
				   XmNuserData, winInfo, NULL);
  XtAddCallback(btn, XmNactivateCallback, clippingPlanesDialog, (void*) (&clippingPlaneInfo) );

  btn = XtVaCreateManagedWidget("view characteristics...", xmPushButtonWidgetClass, menupane, 
				   XmNuserData, winInfo, NULL);
  XtAddCallback(btn, XmNactivateCallback, viewCharacteristicsDialog, (void *) (&viewChar) );

  btn = XtVaCreateManagedWidget("annotate...", xmPushButtonWidgetClass, menupane, 
				   XmNuserData, winInfo, NULL);
  XtAddCallback(btn, XmNactivateCallback, annotateDialog, (void*) (&clippingPlaneInfo) );

  XtSetArg(args[0], XmNsubMenuId, menupane);
  cascade = XmCreateCascadeButton(t->menubar, (char*)"View", args, 1);
  XtManageChild(cascade);

// ****************** application specific option menu ********************
  if (optionMenu.n_button>0)
  {
    if (optionMenu.type == GI_TOGGLEBUTTON)
    {
      menupane = XmCreatePulldownMenu(t->menubar, (char*)"menupane", menuPaneArgs, n);

      XmString abbrevString;
      int j;
      for( j=0; j < optionMenu.n_button; j++ )
      {
	abbrevString = XmStringCreateLocalized(SC optionMenu.tbList[j].buttonLabel.c_str());
	btn = XtVaCreateManagedWidget(SC optionMenu.tbList[j].buttonCommand.c_str(), 
				      xmToggleButtonWidgetClass, menupane,
				      XmNindicatorType, XmN_OF_MANY,
				      XmNvisibleWhenOff, True, 
				      XmNlabelString, abbrevString,
				      NULL);
	optionMenu.tbList[j].tb=btn;
        XmStringFree (abbrevString); /* always destroy compound strings when done */
	XtSetSensitive(btn, optionMenu.tbList[j].sensitive);
	XtAddCallback(btn, XmNvalueChangedCallback, toggled, &optionMenu.tbList[j]);
// set the initial state of the button
	XmToggleButtonSetState(btn, optionMenu.tbList[j].state, False);
      }
    
      XtSetArg(args[0], XmNsubMenuId, menupane);
      cascade = XmCreateCascadeButton(t->menubar, SC optionMenu.menuTitle.c_str(), args, 1);
      XtManageChild(cascade);
    }
    
// save the handle to the menupane for later use
    optionMenu.menupane = menupane;
  }

  // *************** help menu ***************
  menupane = XmCreatePulldownMenu(t->menubar, (char*)"menupane", menuPaneArgs, n);

  for( i=0; helpMenuItems[i] != ""; i++ )
  {
    btn = XtVaCreateManagedWidget(SC helpMenuItems[i].c_str(), xmPushButtonWidgetClass, menupane, 
				   XmNuserData, winInfo, NULL);
    intptr_t idata = -(i+1);  // do this to safely cast to void*
    XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)(idata) );
    // XtAddCallback(btn, XmNactivateCallback, popupCallback,(void*)(-(i+1)) );
  }
    
  XtSetArg(args[0], XmNsubMenuId, menupane);
  cascade = XmCreateCascadeButton(t->menubar, (char*)"Help", args, 1);
  XtManageChild(cascade);
  XtVaSetValues(t->menubar, XmNmenuHelpWidget, cascade, NULL);
  //---------------------------------END CREATING MENUS----------------------


  // build a form to hold the drawing area and right buttons
  // Thanks to Steffan Neumann for figuring out this fix for the DEC's
  t->midarea = XmCreateForm (t->mainw, (char*)"midarea", NULL,0);
  XtManageChild(t->midarea);

  // rightColumn holds all the buttons etc. down the right side
  t->rightColumn=XtVaCreateManagedWidget("rightColumn",
					     xmRowColumnWidgetClass, t->midarea, 
					     XmNorientation, XmVERTICAL,
					     XmNpacking, XmPACK_TIGHT,   
					     XmNtopAttachment,    XmATTACH_FORM,
					     XmNrightAttachment, XmATTACH_FORM,
					     XmNbottomAttachment, XmATTACH_FORM,
					     NULL);

  // make rotation buttons, put in a frame
  t->rotateFrame = XtVaCreateWidget("rotateFrame",xmFrameWidgetClass, t->rightColumn,
					XmNshadowType, XmSHADOW_ETCHED_IN, NULL );
  t->rotateButtons= XtVaCreateWidget( "rotateButtons",xmRowColumnWidgetClass, t->rotateFrame, 
					  XmNorientation, XmHORIZONTAL,  
					  XmNpacking, XmPACK_COLUMN,
					  XmNnumColumns, 3,
					  NULL);
  
// load pixmaps for translations
  Pixel fg, bg;
  XtVaGetValues(t->rotateButtons, XmNforeground, &fg, 
		XmNbackground, &bg, NULL);

  Pixmap yrotpPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Yrotp", fg, bg);
  Pixmap yrotmPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Yrotm", fg, bg);
  Pixmap xrotpPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Xrotp", fg, bg);
  Pixmap xrotmPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Xrotm", fg, bg);
  Pixmap zrotpPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Zrotp", fg, bg);
  Pixmap zrotmPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Zrotm", fg, bg);

  Widget xpr = XtVaCreateManagedWidget("x+r",xmPushButtonWidgetClass, t->rotateButtons, 
				       XmNlabelType, XmPIXMAP,
				       XmNlabelPixmap, xrotpPixmap,
				       XmNuserData, winInfo, NULL);
  Widget xmr = XtVaCreateManagedWidget("x-r",xmPushButtonWidgetClass, t->rotateButtons, 
				       XmNlabelType, XmPIXMAP,
				       XmNlabelPixmap, xrotmPixmap,
				       XmNuserData, winInfo,NULL);    
  Widget ypr = XtVaCreateManagedWidget("y+r",xmPushButtonWidgetClass, t->rotateButtons, 
				       XmNlabelType, XmPIXMAP,
				       XmNlabelPixmap, yrotpPixmap,
				       XmNuserData, winInfo,NULL);    
  Widget ymr = XtVaCreateManagedWidget("y-r",xmPushButtonWidgetClass, t->rotateButtons, 
				       XmNlabelType, XmPIXMAP,
				       XmNlabelPixmap, yrotmPixmap,
				       XmNuserData, winInfo,NULL);    
  Widget zpr = XtVaCreateManagedWidget("z+r",xmPushButtonWidgetClass, t->rotateButtons, 
				       XmNlabelType, XmPIXMAP,
				       XmNlabelPixmap, zrotpPixmap,
				       XmNuserData, winInfo,NULL);    
  Widget zmr = XtVaCreateManagedWidget("z-r",xmPushButtonWidgetClass, t->rotateButtons, 
				       XmNlabelType, XmPIXMAP,
				       XmNlabelPixmap, zrotmPixmap,
				       XmNuserData, winInfo,NULL);    
  XtAddCallback(xpr, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(xmr, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(ypr, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(ymr, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(zpr, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(zmr, XmNactivateCallback, buttonCallback, NULL);
    

  XtManageChild(t->rotateButtons);
  XtManageChild(t->rotateFrame);

  
  // make shift buttons, put in a frame
  Widget shiftFrame = XtVaCreateManagedWidget("shiftFrame",xmFrameWidgetClass, t->rightColumn,
					      XmNshadowType, XmSHADOW_ETCHED_IN, NULL );
  Widget shiftButtons= XtVaCreateWidget( "shiftButtons",xmRowColumnWidgetClass, shiftFrame, 
					 //    XmNorientation, XmVERTICAL,  
					 //    XmNpacking, XmPACK_COLUMN,
					 //    XmNnumColumns, 2,
					 XmNorientation, XmHORIZONTAL,  
					 XmNpacking, XmPACK_COLUMN,
					 XmNnumColumns, 3,
					 NULL);
  Pixmap rightPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Right", fg, bg);
  Pixmap leftPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Left", fg, bg);
  Pixmap upPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Up", fg, bg);
  Pixmap downPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Down", fg, bg);
  Pixmap zpPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"In", fg, bg);
  Pixmap zmPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Out", fg, bg);

  Widget xp = XtVaCreateManagedWidget("x+",xmPushButtonWidgetClass, shiftButtons,
				      XmNlabelType, XmPIXMAP,
				      XmNlabelPixmap, rightPixmap,
				      XmNuserData, winInfo,NULL);

  Widget xm = XtVaCreateManagedWidget("x-",xmPushButtonWidgetClass, shiftButtons,
				      XmNlabelType, XmPIXMAP,
				      XmNlabelPixmap, leftPixmap,
				      XmNuserData, winInfo,NULL);    
  Widget yp = XtVaCreateManagedWidget("y+",xmPushButtonWidgetClass, shiftButtons,
				      XmNlabelType, XmPIXMAP,
				      XmNlabelPixmap, upPixmap,
				      XmNuserData, winInfo,NULL);    
  Widget ym = XtVaCreateManagedWidget("y-",xmPushButtonWidgetClass, shiftButtons,
				      XmNlabelType, XmPIXMAP,
				      XmNlabelPixmap, downPixmap,
				      XmNuserData, winInfo,NULL);    
  Widget zp = XtVaCreateManagedWidget("z+",xmPushButtonWidgetClass, shiftButtons,
				      XmNlabelType, XmPIXMAP,
				      XmNlabelPixmap, zpPixmap,
				      XmNuserData, winInfo,NULL);    
  Widget zm = XtVaCreateManagedWidget("z-",xmPushButtonWidgetClass, shiftButtons,
				      XmNlabelType, XmPIXMAP,
				      XmNlabelPixmap, zmPixmap,
				      XmNuserData, winInfo,NULL);    
  XtAddCallback(xp, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(xm, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(yp, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(ym, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(zp, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(zm, XmNactivateCallback, buttonCallback, NULL);
  XtManageChild(shiftButtons);

  // make bigger/smaller buttons, put in a frame
  Widget bigSmallFrame = XtVaCreateManagedWidget("bigSmallFrame",xmFrameWidgetClass, t->rightColumn,
						 XmNshadowType, XmSHADOW_ETCHED_IN, NULL );
  Widget bigSmallButtons= XtVaCreateWidget( "bigSmallButtons",xmRowColumnWidgetClass, bigSmallFrame, 
					    XmNorientation, XmHORIZONTAL,  
					    XmNpacking, XmPACK_COLUMN,
					    XmNnumColumns, 2,
					    NULL);

// It is possible to use a different font for the buttons by changing the definition of the font
// tagged "BUTTON_FONT". This can for example be done by changing the XResource fontlist, or by
// changing the default value of fontlist set in the fallback resources.

  XmString biggerLabel  = XmStringCreateLtoR((char*)"bigger",  XmFONTLIST_DEFAULT_TAG); // horizontal XmString
  XmString smallerLabel = XmStringCreateLtoR((char*)"smaller", XmFONTLIST_DEFAULT_TAG); // horizontal XmString
  XmString resetLabel   = XmStringCreateLtoR((char*)"reset",   XmFONTLIST_DEFAULT_TAG); // horizontal XmString
  XmString rotpntLabel  = XmStringCreateLtoR((char*)"rot pnt", XmFONTLIST_DEFAULT_TAG); // horizontal XmString

// bitmaps
  Pixmap zoom_inPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Zoom_In", fg, bg);
  Pixmap zoom_outPixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Zoom_Out", fg, bg);
  Pixmap reset_Pixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Reset", fg, bg);
  Pixmap rotpnt_Pixmap = XmGetPixmap( XtScreen(t->toplevel), (char*)"Rotpnt", fg, bg);

  Widget bigger = XtVaCreateManagedWidget("bigger",xmPushButtonWidgetClass, bigSmallButtons, 
					  XmNlabelType, XmPIXMAP,
					  XmNlabelPixmap, zoom_inPixmap,
//					  XmNlabelString, biggerLabel,
					  XmNuserData, winInfo,NULL);    
  Widget smaller = XtVaCreateManagedWidget("smaller",xmPushButtonWidgetClass, bigSmallButtons, 
					  XmNlabelType, XmPIXMAP,
					  XmNlabelPixmap, zoom_outPixmap,
//					   XmNlabelString, smallerLabel,
					   XmNuserData, winInfo,NULL);    
  Widget reset = XtVaCreateManagedWidget("reset",xmPushButtonWidgetClass, bigSmallButtons, 
					  XmNlabelType, XmPIXMAP,
					  XmNlabelPixmap, reset_Pixmap,
//					 XmNlabelString, resetLabel,
					 XmNuserData, winInfo,NULL);    
  Widget rotate = XtVaCreateManagedWidget("rot pnt",xmPushButtonWidgetClass, bigSmallButtons, 
					  XmNlabelType, XmPIXMAP,
					  XmNlabelPixmap, rotpnt_Pixmap,
//					  XmNlabelString, rotpntLabel,
					  XmNuserData, winInfo,NULL);    

  XmStringFree(biggerLabel);
  XmStringFree(smallerLabel);
  XmStringFree(resetLabel);
  XmStringFree(rotpntLabel);

  XtAddCallback(bigger, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(smaller, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(reset, XmNactivateCallback, buttonCallback, NULL);
  XtAddCallback(rotate, XmNactivateCallback, pickCallback, (XtPointer) &viewChar);

  XtManageChild(bigSmallButtons);

  // make frame for the clear button
  Widget clearFrame = XtVaCreateManagedWidget("clearFrame",xmFrameWidgetClass, t->rightColumn,
						 XmNshadowType, XmSHADOW_ETCHED_IN, NULL );
  Widget clearButtons= XtVaCreateWidget( "clearButtons",xmRowColumnWidgetClass, clearFrame, 
					    XmNorientation, XmHORIZONTAL,  
					    XmNpacking, XmPACK_COLUMN,
					    XmNnumColumns, 5,
					    NULL);

// reset rotation point
  XmString resetRotPntLabel   = XmStringCreateLtoR((char*)"reset", XmFONTLIST_DEFAULT_TAG); // horizontal XmString
  Widget resetRotPnt = XtVaCreateManagedWidget("GLOB:init view",xmPushButtonWidgetClass, clearButtons, 
					 XmNlabelString, resetRotPntLabel,
					 XmNuserData, winInfo, NULL);    
  XmStringFree(resetRotPntLabel);
  XtAddCallback(resetRotPnt, XmNactivateCallback, buttonCallback, NULL);
// clear button
  XmString eraseLabel   = XmStringCreateLtoR((char*)"clear", XmFONTLIST_DEFAULT_TAG); // horizontal XmString
  Widget erase = XtVaCreateManagedWidget("clear all",xmPushButtonWidgetClass, clearButtons, 
					 XmNlabelString, eraseLabel,
					 XmNuserData, winInfo, NULL);    
  XmStringFree(eraseLabel);
  XtAddCallback(erase, XmNactivateCallback, buttonCallback, NULL);

  XtManageChild(clearButtons);

  XtManageChild(t->rightColumn);

  // *********** create framed drawing area for OpenGL rendering ******************

  t->frame = XtVaCreateWidget("frame",xmFrameWidgetClass, t->midarea,
			      XmNtopAttachment,      XmATTACH_FORM,
			      XmNleftAttachment,     XmATTACH_FORM,
			      XmNrightAttachment,    XmATTACH_WIDGET,
			      XmNrightWidget,        t->rightColumn,
			      XmNbottomAttachment,   XmATTACH_FORM,
			      XmNshadowType,         XmSHADOW_IN, /*XmSHADOW_ETCHED_IN,*/
			      NULL );
  XtManageChild(t->frame);

  //..Create 'plotStuff' widget = GL Canvas in Motif
  if( t->vi->depth <=8 )
  {
    // WDH : for visuals with only 8bits of color we seem to need to share the colour map
    // overlayColormap = XCreateColormap(dpy, DefaultRootWindow(dpy),vi, AllocNone);
    
    t->cmap = DefaultColormap(dpy, DefaultScreen(dpy));
    t->plotStuff = XtVaCreateManagedWidget("plotStuff", glwMDrawingAreaWidgetClass,
				      t->frame, GLwNvisualInfo, t->vi, XtNcolormap, t->cmap, NULL);
  }
  else
    t->plotStuff = XtVaCreateManagedWidget("plotStuff", glwMDrawingAreaWidgetClass,
				      t->frame, GLwNvisualInfo, t->vi, NULL);

  XtAddCallback(t->plotStuff, GLwNginitCallback, graphicsInit, (XtPointer) ((intptr_t)OGLCurrentWindow)); 
  XtAddCallback(t->plotStuff, XmNexposeCallback, exposeOrResize, NULL);
  XtAddCallback(t->plotStuff, XmNresizeCallback, exposeOrResize, NULL);
  XtAddCallback(t->plotStuff, XmNinputCallback, drawAreaInput, NULL);

  // create row-column widget for user defined buttons
  short numCol=1;
  t->userButtonArea = XtVaCreateManagedWidget("rowcol", xmRowColumnWidgetClass, t->mainw, 
					      XmNpacking, XmPACK_COLUMN,
					      XmNorientation, XmHORIZONTAL,
					      XmNnumColumns, numCol, // number of rows since it is HORIZONTAL
					      NULL);
  
  // initially, we have no userdefined buttons
  t->numUserBtn = 0;
  for (i=0; i<10; i++)
    t->userButton[i] = NULL;

  // initially, there is no user defined pulldown menu
  t->userCascade = NULL;
  
// ----------------------------------------END CREATING CANVAS & BUTTONS

#ifdef USE_POWERWALL
  //VDL Canvas initialization code
  //-------------------------------
  // who to blame: pf
  //
  if ( (0 == OGLNWindows) && (!has_vdl) ) // initialize once
  {
    has_vdl = True;
    //const int _vdlWidth  = 400;
    //const int _vdlHeight = 400;
    if (vdlShareDisplayLists) // share with window #0
    {
      //canvas = vdlCanvasCreateShared(0, 0, 0,
      //			  vdlVirtualWidth(0), vdlVirtualHeight(0),
      //	       		  vdlDraw, NULL, vdlInitWin, NULL,
      //	       		  t->cx);
      canvas = vdlCanvasCreateShared(0, 0, 0,
      			  vdl_width, vdl_height,
      	       		  vdlDraw, NULL, vdlInitWin, NULL,
      	       		  t->cx);
      
    }
    else
    {
      //cout << "VDL: NOT sharing display lists. VDL might not work properly\n";
      canvas = vdlCanvasCreate(0, 0, 0,
			  vdlVirtualWidth(0), vdlVirtualHeight(0),
			  vdlDraw, NULL, vdlInitWin, NULL);
    }
    //vdl_width  = vdlCanvasWidth( &canvas );
    //vdl_height = vdlCanvasHeight( &canvas );
  } // end if ( 0==OGLNWindows)
#endif // end vdl initialization code.

  XmMainWindowSetAreas(t->mainw, t->menubar, t->userButtonArea, NULL, NULL, t->midarea);

  XtRealizeWidget(t->toplevel);

// *AP* the following stuff is done in the graphicsInit callback
//   // first realize widget, then bind GL context!!
//      if (!glXMakeCurrent(dpy, XtWindow(t->plotStuff), t->cx))
//      {
//        printf("makeGraphicsWindow: glxMakeCURRENT failed\n");
//        checkGLError();
//      }
  

//  windowAttributes.cursor=cursor; // Set the cursor to a watch
  windowAttributes.cursor=None;     // Set the cursor to an arrow
  XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes ); 
  XFlush(dpy);

  t->postDisplay = GL_FALSE;

// the xValue and lineSF Widgets do not get realized until the view char dialog is opened
  t->xValue = NULL;
  t->lineSF = NULL;
  t->fractionOfScreen = NULL;
// the view char dialog is not opened
  t->viewCharOpen = 0;
// the clip plane dialog is not opened
  t->clipPlaneOpen = 0;
// neither is the annotate dialog
  t->annotateOpen = 0;

  XtVaGetValues(t->plotStuff, XmNwidth, &(t->width), XmNheight, &(t->height), NULL);

//  printf("== Canvas size: width %i, height %i\n", t->width, t->height); // pf debug

  OGLNWindows++;
  
  
  return OGLCurrentWindow;
}

int
destroyGraphicsWindow(int win_number)
{
  printf("mogl -- destroy graphics window %i.\n", win_number); // debug **pf

  if ( win_number < 0 || win_number >= OGLNWindows)
  {
    printf("destroyGraphicsWindow: no window such window: %i\n", win_number);
    return ERROR;
  }
  // check if the window is still around
  if (!OGLWindowList[win_number])
  {
    printf("destroyGraphicsWindow: window %i is not active\n", win_number);
    return ERROR;
  }

  WINDOW_REC *t = OGLWindowList[win_number];

#ifdef USE_POWERWALL
  // ..destroy vdl canvas if this is the last window destroyed 
  // ..note that the glXContext is shared, so we should destroy
  //   the vdl canvas before destroying t->cx
  // who to blame: pf
  if ( (OGLNWindows == 0) && has_vdl)
  {
    if (canvas != NULL) 
    {
      vdlCanvasDestroy( canvas );
      canvas = NULL;
    }
    has_vdl = False;
  }
#endif

  // destroy the widget associated with the toplevel widget
  XtUnrealizeWidget( t->toplevel );
  glXDestroyContext( dpy, t->cx );

  // destroy window record
  delete OGLWindowList[win_number];
  OGLWindowList[win_number] = NULL;
  if (win_number == OGLNWindows-1)
    OGLNWindows--;

  return OK;
}

void
moglSetSensitive(int win_number, int trueOrFalse)
{
  WINDOW_REC *t;
  if (0<=win_number && win_number < OGLNWindows)
  {
    t = OGLWindowList[win_number];
  }
  else
  {
    printf("ERROR:moglMakeInSensitive: invalid window number: %i\n", win_number);
    return;
  }

  // first the menu
  if (t->userCascade)
    XtSetSensitive( t->userCascade, trueOrFalse );

  int i;
  for (i=0; i<t->numUserBtn && t->userButton[i]; i++)
    XtSetSensitive( t->userButton[i], trueOrFalse );
}

void
moglSetButtonSensitive(int win_number, int btn, int trueOrFalse)
{
  WINDOW_REC *t;
  if (0<=win_number && win_number < OGLNWindows)
  {
    t = OGLWindowList[win_number];
  }
  else
  {
    printf("ERROR:moglSetButtonSensitive: invalid window number: %i\n", win_number);
    return;
  }

  if (btn >=0 && btn<t->numUserBtn && t->userButton[btn])
    XtSetSensitive( t->userButton[btn], trueOrFalse );
}


void
moglBuildUserButtons(const aString buttonCommand[], const aString buttonLabel[], int win_number)
{
  int i;
  WINDOW_REC *t;
  if (0<=win_number && win_number < OGLNWindows)
  {
    t = OGLWindowList[win_number];
  }
  else
  {
    printf("ERROR:moglBuildUserButtons: invalid window number: %i\n", win_number);
    return;
  }
  
// remove any existing buttons
  for (i=0; i<t->numUserBtn; i++)
  {
    XtUnrealizeWidget(t->userButton[i]);
  }
  t->numUserBtn = 0;

// exit unless there are any new buttons to build
  if (!buttonCommand){
    return;
  }

// count the number of buttons
  for (i=0; i<MAX_BUTTONS && buttonCommand[i] != "" && buttonCommand[i].length()>0; i++);
  t->numUserBtn = i;


// get the width of the user button area
  Dimension baWidth, maxButtonWidth=0;
  // Dimension buttonWidth;
  XtVaGetValues(t->userButtonArea, 
		XmNwidth, &baWidth, 
		NULL);

  maxButtonWidth = 65; // educated guess when the abbrevs are up to 6 characters long
  short numCol; // need to compute this based on the size and number of buttons
  numCol = (int) ceil((maxButtonWidth * t->numUserBtn)/((double) baWidth));
  
// reconfigure the userbuttonarea (change the number of rows) such that the window doesn't have to get wider.
  XtVaSetValues(t->userButtonArea, 
		XmNnumColumns, numCol, // number of rows since it is HORIZONTAL
		NULL);

  XmString abbrevString;
  for (i=0; i<MAX_BUTTONS && buttonCommand[i] != "" && buttonCommand[i].length()>0; i++)
  {
    if (buttonLabel[i]!="" && buttonLabel[i].length()>0) // only use abbreviations if they are there
    {
      abbrevString = XmStringCreateLocalized (SC buttonLabel[i].c_str());
      t->userButton[i] = XtVaCreateManagedWidget(SC buttonCommand[i].c_str(), xmPushButtonWidgetClass, 
						 t->userButtonArea, 
						 XmNlabelString, abbrevString,
						 NULL);    
//        XtVaGetValues(t->userButton[i], 
//  		    XmNwidth, &buttonWidth, 
//  		    NULL);
//        maxButtonWidth = max(maxButtonWidth, buttonWidth);
      
      XmStringFree (abbrevString); /* always destroy compound strings when done */
    }
    else
    {
      t->userButton[i] = XtVaCreateManagedWidget(SC buttonCommand[i].c_str(), xmPushButtonWidgetClass, 
						 t->userButtonArea, 
						 NULL);    
//        XtVaGetValues(t->userButton[i], 
//  		    XmNwidth, &buttonWidth, 
//  		    NULL);
//        maxButtonWidth = max(maxButtonWidth, buttonWidth);
    }
    XtAddCallback(t->userButton[i], XmNactivateCallback, userButtonCallback, NULL);
  }
  t->numUserBtn = i;

}

void
moglBuildUserMenu(const aString menuName[], const aString menuTitle, int win_number)
{
  WINDOW_REC *t;
  if (0<=win_number && win_number < OGLNWindows)
  {
    t = OGLWindowList[win_number];
  }
  else
  {
    printf("ERROR:moglBuildUserMenu: invalid window number: %i\n", win_number);
    return;
  }
  
  if (!menuName) // remove any existing user menu
  {
    if (t->userCascade) XtUnrealizeWidget(t->userCascade);
    return;
  }
  

  int n=0;
  if( overlayVisual )
  {
    XtSetArg(menuPaneArgs[n], XmNvisual, overlayVisual);  n++;
    XtSetArg(menuPaneArgs[n], XmNdepth,  overlayDepth);  n++;
    XtSetArg(menuPaneArgs[n], XmNvisual, overlayColormap);  n++;
  }
    
  Widget menupane = XmCreatePulldownMenu(t->menubar, (char*)"menupane", menuPaneArgs, n);

  int * winInfo_ = (int *) malloc( sizeof(int) );
  XtPointer winInfo = (XtPointer) winInfo_;
  *winInfo_ = win_number; // encode the window number into the user data

  int i;
  Widget btn;
  for (i=0; i<10 && menuName[i].length() && strlen(menuName[i].c_str())>0; i++)
  {
    btn = XtVaCreateManagedWidget(SC menuName[i].c_str(), xmPushButtonWidgetClass, menupane, 
				  XmNuserData, winInfo, NULL);
    intptr_t idata = i+1;  // do this to safely cast to void*
    XtAddCallback(btn, XmNactivateCallback, popupCallback, (void*)(idata) );
    // XtAddCallback(btn, XmNactivateCallback, popupCallback, (void*)((i+1)) );
  }

// delete any previous user defined pulldown menu
  if (t->userCascade) XtUnrealizeWidget(t->userCascade);

  XtSetArg(args[0], XmNsubMenuId, menupane);
  t->userCascade = XmCreateCascadeButton(t->menubar,SC menuTitle.c_str(), args, 1);
  XtManageChild(t->userCascade);

}



//..MOTIF
void
moglSetViewFunction( MOGL_VIEW_FUNCTION viewFunction_ )
{
  viewFunction=viewFunction_;
}



//..MOTIF
void
moglSetFunctions( GL_GraphicsInterface *giPointer,
                  MOGL_DISPLAY_FUNCTION displayFunc, 
                  MOGL_RESIZE_FUNCTION resizeFunc )
// Define the display function callback -- called to redraw the screen
{
  graphicsInterfacePointer=giPointer;
  displayFunction=displayFunc;
  resizeFunction=resizeFunc;
}


int
moglMakeCurrent(int win)
{
  if (0 <= win && win < OGLNWindows)
  {
    if (glXMakeCurrent(dpy, XtWindow(OGLWindowList[win]->plotStuff), OGLWindowList[win]->cx))
    {
      OGLCurrentWindow = win;
      return OK;
    }
    else
    {
      printf("moglMakeCurrent: glXMakeCurrent failed for win=%i\n", win);
      checkGLError();
      return ERROR;
    }
  }
  else
    return ERROR;
  
}

int
moglGetInfo( Display *&dpy_, XVisualInfo *&vi, GLXContext &cx )
{
  int win = OGLCurrentWindow;
  dpy_=dpy;
  vi=OGLWindowList[win]->vi;
  cx=OGLWindowList[win]->cx;
  
  return 0;
}


int
moglGetNWindows()
{
  return OGLNWindows;
}

int
moglGetCurrentWindow()
{
  return OGLCurrentWindow;
}


void
moglResetContext() // this is used by osRender
{
//  glXMakeCurrent(dpy, XtWindow(OGLWindowList[OGLCurrentWindow]->plotStuff), OGLWindowList[OGLCurrentWindow]->cx);
}

void
moglAppendCommandHistory(const aString &answer)
// This routine is usually called from GL_GraphicsInterface
{

  if( showCommandHistory && answer.length() )
  {
// append the command history list here
    XmString answer_string = XmStringCreateLtoR(SC answer.c_str(), XmFONTLIST_DEFAULT_TAG);
    XmListAddItemUnselected(oldCommandList, answer_string, 0);
    int nItems, vItems, listFocus;
    XtVaGetValues(oldCommandList, XmNitemCount, &nItems, XmNvisibleItemCount, &vItems, NULL);
    listFocus = max(1, nItems-vItems+1);
    XmListSetPos(oldCommandList, listFocus); // set the focus of the list at the bottom
    XmStringFree(answer_string);

    XFlush(dpy);
  }
  
  return;
}


//..MOTIF & GL --> separate to Generic & Motif
void 
moglSetPrompt(const aString &prompt)
// ===================================================================================
// /Purpose:
//    Set the prompt.
// ===================================================================================
{
  static XmTextPosition wpr_position=0;
  
  if( showPrompt && prompt.length() )
  {
    XmTextInsert(promptText, wpr_position,SC prompt.c_str());
    wpr_position += prompt.length();
    XtVaSetValues(promptText, XmNcursorPosition, wpr_position, NULL);
    XmTextShowPosition(promptText, wpr_position);
    XmTextInsert(promptText, wpr_position, (char*)"\n");
    wpr_position += 1;
// erase the command line
    XmTextSetString(command_w, (char*)"");
  }
  
}

static int
moglSetupPopup(Widget &baseWin, const aString menu[], Widget &popupMenu)
{
  // *NOTE* we need to add the info from the visual --- otherwise this generates an error in XCreateWindow 
  if(overlayVisual==NULL ) 
// activate the popup menu when mouse button 3 is down and not the shift key, the control key, 
// or any other modifier is pressed. 
// See volume 5, appendix F for the syntax of translation tables.

    // XmNwhichButton, 3,  <- using this (old way) will make right-mouse work with num-lock on (or any other modifier)
    //                        but this means that the mouse-zoom, shift-right-mouse will not work.
    //                        There is apparently a bug in Motif 2.x 
    popupMenu=XmVaCreateSimplePopupMenu(baseWin, (char*)"popup", popupCallback, 
    					XmNmenuPost, (char*)"None <Btn3Down>", 
                                        // XmNwhichButton, 3, // use instead of the above
					NULL); 
  else
    popupMenu=XmVaCreateSimplePopupMenu(baseWin, (char*)"popup", popupCallback,
    					XmNmenuPost, (char*)"None <Btn3Down>", 
					XmNvisual, overlayVisual,
					XmNdepth, overlayDepth, 
					XmNcolormap, overlayColormap, NULL); 
  

  const int maxNumberOfCascadeLevels=10; //.....MAGIC NUMBER, should be defined elsewhere
  Widget widget[maxNumberOfCascadeLevels], gidget;
  int level=0;  // current depth of cascade menu (double check to look for user mistakes)
  int count[maxNumberOfCascadeLevels];
  int i;
  for( i=0; i<maxNumberOfCascadeLevels; i++ )
  {
    widget[i]=popupMenu;
    count[i]=-1;
  }
  
// count the number of menu entries
  int numberOfMenuEntries=0;
  for( i=0; menu[i]!="" && i<maximumNumberOfMenuEntries; i++ )  
  {
    numberOfMenuEntries++;
    if( menu[i][0]=='!' ) // Add title to a cascading menu
    {
      //
      // On Linux + Lesstif 0.88.1, adding titles to cascading
      // menu breaks the cascading menu: On other variants of Motif,
      // labels on cascading menus are ok. TODO: get a fix for Lesstif 2/25/2000 pf
      //
#ifndef OV_USE_LESSTIF
      // ..exclude labels from cascading menus on Lesstif
      // this is a title or sub title
      gidget = XtVaCreateManagedWidget(menu[i].c_str() + 1, xmLabelWidgetClass, widget[level], NULL );
      count[level]++;
      gidget = XtVaCreateManagedWidget("separator", xmSeparatorWidgetClass, widget[level], NULL );
      count[level]++;
#endif
    }
    else if( menu[i][0]=='>' )
    {
      gidget = XtVaCreateManagedWidget(menu[i].c_str() + 1, xmCascadeButtonWidgetClass, widget[level], NULL);
      count[level]++;
      level++;
      if( level>=maxNumberOfCascadeLevels )
	printf("moglSetupPopup: Too many levels of cascading menus, max= %i \n",maxNumberOfCascadeLevels);

      if(overlayVisual==NULL ) {
        widget[level] = XmVaCreateSimplePulldownMenu(widget[level-1], (char*)"pullright",
						     count[level-1],popupCallback, NULL );
      }
      else {
        widget[level] = XmVaCreateSimplePulldownMenu(widget[level-1], (char*)"pullright",count[level-1],
						     popupCallback,
						     XmNvisual, overlayVisual,
						     XmNdepth, overlayDepth, 
						     XmNcolormap, overlayColormap, NULL); 
      }
      intptr_t idata = i;  // do this to safely cast to void*
      XtAddCallback(gidget, XmNactivateCallback, popupCallback, (void*)idata);
    }
    else if( menu[i][0]=='<' )
    {
      count[level]=-1;
      level--;
      if( level<0 )
      {
	printf("moglSetupPopup:ERROR: There seem to be too many '<' characters in the menu\n"
               "I am at menu item %i = %s \n", i, SC menu[i].c_str());
	throw "error";
      }
      if( menu[i].length()>1 && menu[i][1]=='>' )
      { // This case occurs if the menu starts with "<>..." -- when one cascade menu follows another
	gidget = XtVaCreateManagedWidget(menu[i].c_str()+2,xmCascadeButtonWidgetClass,widget[level],NULL);
	count[level]++;
	level++;
        if(overlayVisual==NULL ) 
	  widget[level] = XmVaCreateSimplePulldownMenu(widget[level-1],(char*)"pullright",
						       count[level-1], popupCallback, NULL );
        else
	  widget[level] = XmVaCreateSimplePulldownMenu(widget[level-1], (char*)"pullright",
						       count[level-1], popupCallback,
						       XmNvisual, overlayVisual,
						       XmNdepth, overlayDepth, 
						       XmNcolormap, overlayColormap, NULL); 
      }
      else
      {
        gidget = XtVaCreateManagedWidget(menu[i].c_str() + 1, xmPushButtonGadgetClass, widget[level], NULL);
        count[level]++;
      }
      intptr_t idata = i;  // do this to safely cast to void*
      XtAddCallback(gidget,XmNactivateCallback,popupCallback,(void*)idata);
      // XtAddCallback(gidget,XmNactivateCallback,popupCallback,(void*)i);
    }
    else
    { 
      gidget = XtVaCreateManagedWidget(SC menu[i].c_str(), xmPushButtonGadgetClass, widget[level], NULL);
      count[level]++;
      intptr_t idata = i;  // do this to safely cast to void*
      XtAddCallback(gidget,XmNactivateCallback,popupCallback,(void*)idata);
      // XtAddCallback(gidget,XmNactivateCallback,popupCallback,(void*)i);
    }
  }
  if( numberOfMenuEntries >= maximumNumberOfMenuEntries-1 )
  {
    printf("mogl:ERROR: Too many menu entries! numberOfMenuEntries=%i, maximumNumberOfMenuEntries=%i.\n"
           "     continuing...,",
                numberOfMenuEntries,maximumNumberOfMenuEntries );
    // exit(1);  
  }
  if( level!=0 )
  {
    printf("mogl::FATAL ERROR: There is a mistake in specifying cascading menus. Each `>' must have a matching '<'\n");
    for( i=0; menu[i]!="" && i<maximumNumberOfMenuEntries; i++ )
      printf("menu[%i] = %s \n", i, SC menu[i].c_str());
    throw "error";
  }
  XtAddEventHandler(baseWin, ButtonPressMask, FALSE, postIt, popupMenu );

  return OK;
  
}


void
moglBuildPopup(const aString menu[])
// ===================================================================================
// /Purpose:
//  Get a menu entry from an array of strings, terminated with a null string
// /Notes:
//  To add a label or title (or sub title) to the list start the string with an '!' as in "!my title"
//
//  To create a cascading menu begin the string with an '>'
//  To end the cascade begin the string with an '<' 
//  To end a cascade and start a new cascade begin the string with '<' followed by '>'
//  Here is an example:
//  char *menu1[] = {  "plot",
//                     ">component",
//                                  "u",
//                                  "v",
//                                  "w",
//                     "<erase",
//                     ">stuff",
//                              "s1",
//                              ">more stuff", 
//                                            "more1",
//                                            "more2", 
//                              "<s2", 
//                    "<>apples", 
//                              "apple1", 
//                    "<exit",NULL };  
//
//
// ===================================================================================
{
  WINDOW_REC * w;
  int win;
  
  if (!menu)
  {
// disable the popups in all graphics windows
    for (win=0; win<OGLNWindows; win++)
    {
      w = OGLWindowList[win];
      if (popupMenu[win])
      {
	XtRemoveEventHandler(w->mainw, ButtonPressMask, FALSE, postIt, popupMenu[win] );
      // disable right mouse button (or screen may hang with an arrow pointing up to the right)
	XtVaSetValues(popupMenu[win], XmNpopupEnabled, False, NULL );
      }
      popupMenu[win] = NULL;
    }
// disable the popup in the command window
    if (popupMenu[win])
    {
      XtRemoveEventHandler(/*textw*/ bulletin, ButtonPressMask, FALSE, postIt, popupMenu[OGLNWindows] );
      XtVaSetValues(popupMenu[OGLNWindows], XmNpopupEnabled, False, NULL );
    }
    XFlush(dpy);
    popupMenu[OGLNWindows]=NULL;
  }
  else
  {
// setup the popup in all graphics windows
    for (win=0; win<OGLNWindows; win++)
    {
      w = OGLWindowList[win];
// remove any existing menu
      if (popupMenu[win])
	XtRemoveEventHandler(w->mainw, ButtonPressMask, FALSE, postIt, popupMenu[win] );
      moglSetupPopup(w->mainw, menu, popupMenu[win]);
    }
// remove any existing menu in the command window
    if (popupMenu[OGLNWindows])
      XtRemoveEventHandler( bulletin, ButtonPressMask, FALSE, postIt, popupMenu[OGLNWindows] );
// setup the popup in the command window
    moglSetupPopup( bulletin, menu, popupMenu[OGLNWindows]);
  }
  
}


int 
moglGetAnswer( aString &answer, const aString prompt /* = "" */, 
	       PickInfo *pick_ /* =NULL */, int blocking /* = 1 */ )
{
  // kkc conversion to std::string, how does this work for aString??? if (prompt && prompt.length()>0) moglSetPrompt(prompt);
  if (prompt.length()>0) moglSetPrompt(prompt);

  if( pick_ )
  {
    readyToPick = 1;// reset the global variables used for picking
    pick_->pickType = pickOption = 0;
    pick_->pickWindow = pickWindow = -99;
  }
  else
  {
    readyToPick = 0;
  }
  
  int win;
  WINDOW_REC * w;
// enable the popup in all graphics windows + command window
  for (win=0; win<OGLNWindows; win++)
    {
      w = OGLWindowList[win];
// enable the popup
      if (popupMenu[win])
	XtAddEventHandler(w->mainw, ButtonPressMask, FALSE, postIt, popupMenu[win] );
    }
  if (popupMenu[OGLNWindows])
    XtAddEventHandler( bulletin, ButtonPressMask, FALSE, postIt, popupMenu[OGLNWindows] );

// initialize global variables
  setMenuNameChosen("");
  menuItemChosen = 0;
  
  if (blocking)
// wait in the event loop until the global variable exitEventLoop has been set to true by 
// a callback function
    eventLoop();
  else
// just process all events in the queue
    moglPollEvents();
  
// copy the global variable menuNameChosen. The menu item is already stored in the global variable 
// menuItemChosen
  answer=menuNameChosen; 

// pickOption is set to a non-zero value in drawAreaInput() after a pick event.
  if( pick_ && pickOption != 0 ) 
  {
    pick_->pickBox[0]=xRubberBandMin;
    pick_->pickBox[1]=xRubberBandMax;
    pick_->pickBox[2]=yRubberBandMin;
    pick_->pickBox[3]=yRubberBandMax;

    pick_->pickType = pickOption;
    pick_->pickWindow = pickWindow;

    readyToPick = 0;
  }

    for (win=0; win<OGLNWindows; win++)
    {
      w = OGLWindowList[win];
// disable the popup
      if (popupMenu[win])
	XtRemoveEventHandler(w->mainw, ButtonPressMask, FALSE, postIt, popupMenu[win] );
    }
// remove any existing popup menu in the command window
    if (popupMenu[OGLNWindows])
      XtRemoveEventHandler( bulletin, ButtonPressMask, FALSE, postIt, popupMenu[OGLNWindows] );

  
  return menuItemChosen;
}

//..MOTIF & GL --> separate to Generic & Motif
//....MAIN & Only popup Menu-builder function: 
//....   Reusable code should be separated from the MOTIF menu building business
// ---------------------------moglGetMenuItem
//
// AP: This routine is no longer used
//
//  int 
//  moglGetMenuItem( const aString menu[], aString &answer, 
//                   const aString prompt /* = NULL */, 
//                   float *pickBox /* =0 */,
//  		 int win_number /* =0 */ )
//  // ===================================================================================
//  // /Purpose:
//  //  Get a menu entry from an array of strings, terminated with a null string
//  // /pickBox (input) : optionally supply pickBox[4]. If this argument is given then
//  //         this routine will attempt to pick a region on the screen (in addition to
//  //         choosing menu items). If a point or region is picked then the answer will be
//  //         returned as "mogl-pick", the return value will be -1 and 
//  //             pickBox[0]=xMin, pickBox[1]=xMax, pickBox[2]=yMin, pickBox[3]=yMax, 
//  //         will mark the region that was chosen.
//  // /Notes:
//  //  To add a label or title (or sub title) to the list start the string with an '!' as in "!my title"
//  //
//  //  To create a cascading menu begin the string with an '>'
//  //  To end the cascade begin the string with an '<' 
//  //  To end a cascade and start a new cascade begin the string with '<' followed by '>'
//  //  Here is an example:
//  //  char *menu1[] = {  "plot",
//  //                     ">component",
//  //                                  "u",
//  //                                  "v",
//  //                                  "w",
//  //                     "<erase",
//  //                     ">stuff",
//  //                              "s1",
//  //                              ">more stuff", 
//  //                                            "more1",
//  //                                            "more2", 
//  //                              "<s2", 
//  //                    "<>apples", 
//  //                              "apple1", 
//  //                    "<exit",NULL };  
//  //
//  //
//  // ===================================================================================
//  {
//    int win;

//    moglBuildPopup( menu );
  
//    if (prompt && strlen(prompt)>0) moglSetPrompt(prompt);

//    if( pickBox!=0 )
//    {
//      OGLActiveWindow = win_number; /* save the window number for the picking */
//    }
  
//  // wait in the event loop until the global variable exitEventLoop has been set to true by a callback function
//    eventLoop();
//  // copy the global variable menuNameChosen. The menu item is already stored in the global variable menuItemChosen
//    answer=menuNameChosen; 

//    if( pickBox!=0 )
//    {
//      OGLActiveWindow = -1;
//  //                      123456789
//      if( strncmp(answer,"mogl-pick", 9)==0 ) // zoom events are now different from pick events
//      {
//        pickBox[0]=xRubberBandMin;
//        pickBox[1]=xRubberBandMax;
//        pickBox[2]=yRubberBandMin;
//        pickBox[3]=yRubberBandMax;
//      }
//    }
  
//  // disable the popups
//    moglBuildPopup( NULL );
  
//    return menuItemChosen;
//  }




//..MOTIF --> maybe GL also...
void 
moglGetWindowSize( int & width, int & height, int win /* = 0 */ )
{
  Dimension width0, height0;
  WINDOW_REC * w;
  if (0 <= win && win < OGLNWindows)
  {
    w = OGLWindowList[win];
//    glXMakeCurrent(dpy, XtWindow(w->plotStuff), w->cx);
  }
  else
  {
    width = height = -999;
    return;
  }
  
  XtVaGetValues(w->plotStuff, XmNwidth, &width0, XmNheight, &height0, NULL);
  width=width0;
  height=height0;  
}

// MOTIF & GL?
void 
moglPostDisplay(int win)
{
  if (0 <= win && win < OGLNWindows)
  {
    OGLWindowList[win]->postDisplay=TRUE;
  }
  return;
}

//================================MOTIF INTERNAL

// draw(widget)
// ------------
//  * updates the current window
//  * with PowerWall, renders the tiles
//
//  Who to blame (for the vdl part): pf
//
void 
draw(Widget w)
// draw the OpenGL frame on an expose event
{
  int win_number;
  WINDOW_REC * t=NULL;
  
  if (window_exists(XtWindow(w), &win_number))
  {
    t = OGLWindowList[win_number];
    moglMakeCurrent(win_number);
  }
  else
    return;

  // printf("draw function called, widget name = %s \n",XtName(w));
  displayFunction(graphicsInterfacePointer, win_number);
  if (t->doubleBuffer) glXSwapBuffers(dpy, XtWindow(w));
  if(!glXIsDirect(dpy, t->cx))
    glFinish(); /* avoid indirect rendering latency from queuing */

#ifdef USE_POWERWALL  
  //..Start PowerWall update if we are updating the vdlActiveWindowNumber
  Bool isVdlCanvasRedrawn = False;
  if ( win_number == vdlActiveWindowNumber ) 
  {
    //isVdlCanvasRedrawn = True;
    //vdlCanvasRedrawBegin( canvas, vdlFlag );
    vdlFlag = True;
    vdlCanvasRedraw( canvas, vdlFlag );
    debug_sniff_for_opengl_errors( "VDL -- mogl::draw" );
  }
#endif


#ifdef USE_POWERWALL
  //..Synchronize if necessary
  //....We use a local flag to avoid potential side-effects 
  //    on win_number, vdlActiveWindowNumber **pf
  //if ( isVdlCanvasRedrawn )
  //{
  //  vdlCanvasRedrawEnd( canvas );
  //}
#endif
}


//..MOTIF
void
graphicsInit(Widget w, XtPointer data, XtPointer callData)
{
  int win_number = (long int) data; /*OGLCurrentWindow;*/
  WINDOW_REC * t = OGLWindowList[win_number];
  
  XVisualInfo *visinfo;
//  printf("Inside graphicsInit, win # %i\n", win_number);
  XtVaGetValues(w, GLwNvisualInfo, &visinfo, NULL);
  

  /* create an OpenGL rendering context */
  // ** this needs to be destroyed when done *****************************************************
  // *wdh* t->cx = glXCreateContext(dpy, visinfo /*t->vi*/,  NULL, /* favor direct */ GL_TRUE);
  t->cx = glXCreateContext(dpy, visinfo /*t->vi*/, /* no display list sharing */ NULL, 
                           /* favor direct */ preferDirectRendering);
  if (t->cx == NULL)
    XtAppError(app, "could not create rendering context");
  
 // first realize widget, then bind GL context!!
  if (!glXMakeCurrent(dpy, XtWindow(t->plotStuff), t->cx))
  {
    printf("graphicsInit: glxMakeCURRENT failed\n");
    checkGLError();
  }
//  printf("Leaving graphicsInit\n");

}


void 
exposeOrResize(Widget w, XtPointer data, XtPointer callData)
// The screen has been resized or exposed
{
  XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) callData;
  int win_number=-1;
  WINDOW_REC *t;
  
  if (window_exists(XtWindow(w), &win_number))
  {
    t = OGLWindowList[win_number];
    moglMakeCurrent(win_number);
  }
  else
    return;
  
  if( cbs->reason != XmCR_EXPOSE )
  {
    XtVaGetValues(t->plotStuff, XmNwidth, &(t->width), XmNheight, &(t->height), NULL);
//    printf("resize: new drawing area = %i X %i \n", t->width, t->height);
    glViewport(0, 0, (GLint) t->width, (GLint) t->height); 

    resizeFunction(graphicsInterfacePointer,win_number);

    t->postDisplay=TRUE;
  }
  else
  {
    // multiple expose events can be queued, wait for a count of zero
    if( cbs->event->xexpose.count>0 )
    {
      int count =cbs->event->xexpose.count;
      // skip contiguous expose events
      XEvent event;
      for( int i=0; i<count; i++ )
        XtAppNextEvent(xtAppContext, &event);   // we wait here when no events are pending
    }
    else
    { 
      t->postDisplay=TRUE;
    }
  }
  // **pf debug code
  Dimension xSize, ySize;
  XtVaGetValues(t->toplevel, XmNwidth, &xSize, XmNheight, &ySize, NULL);
#ifdef PF_DEBUG
  cout << "MOGL resizeOrExpose: toplevel.width = "<< xSize
       << " toplevel.height = "<< ySize << endl;
#endif

  //  XFlush(dpy);
}

//..MOTIF
void 
map_state_changed(Widget w, XtPointer data, XEvent * event, Boolean * cont)
// This routine called when window is iconified
{
  // OBSOLETE in v18??
}


static void
select_cmd( Widget widget, XtPointer client_data, XtPointer call_data )
{
  XmListCallbackStruct *listCBS = (XmListCallbackStruct *) call_data;
  
  if (listCBS->reason == XmCR_SINGLE_SELECT)
  {
    char *newtext=NULL;
    XmStringGetLtoR(listCBS->item, XmFONTLIST_DEFAULT_TAG, &newtext);
    XmTextSetString(command_w, newtext);
    XmTextSetInsertionPosition(command_w, strlen(newtext));
  }
  else if (listCBS->reason == XmCR_DEFAULT_ACTION)
  {
    char *newtext=NULL;
    XmStringGetLtoR(listCBS->item, XmFONTLIST_DEFAULT_TAG, &newtext);
// the command is echoed in the list by the moglGetMenuItem routine
// send the command to the interpreter
    menuItemChosen = -1; // ****
    setMenuNameChosen(newtext);
    exitEventLoop=TRUE;
  }
  
  
}

//..MOTIF
void
exec_cmd( Widget widget, XtPointer client_data, XtPointer call_data )
// This widget is called when a command string is typed in
{
  char *message=NULL;
  
  message = XmTextGetString(widget);
  XmTextSetString(widget,(char*)"");       // reset text to blank

  menuItemChosen = -1; // ****
  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}


//..MOTIF
void
inputCommand(Widget cmd_widget, XtPointer client_data, XtPointer call_data )
// use this routine to create popup menu
{
  Widget popup = (Widget) client_data; 
  XmDrawingAreaCallbackStruct *cbs = ( XmDrawingAreaCallbackStruct *) call_data;

  if( cbs->event->xany.type != ButtonPress || cbs->event->xbutton.button != 3 )
    return;

  // position the popup menu where the event occured
  XmMenuPosition(popup, (XButtonPressedEvent *) (cbs->event));
  XtManageChild(popup);
}

//..MOTIF
void
postIt(Widget cmd_widget, XtPointer client_data, XEvent *event, char* dum)
// use this routine to create popup menu
{
  Widget popup = (Widget) client_data; 
  XButtonPressedEvent *bEvent = ( XButtonPressedEvent *) event;
  
//  printf("postit: button number %i\n",bEvent->button);
  
// AP: The following 2 lines are redundant, since this condition is specified by
// the XmNmenuPost option. This routine doesn't even get called unless button3
// is held down WITHOUT any modifiers held down.
  if( bEvent->button != 3 )
    return;

  // position the popup menu where the event occured
  XmMenuPosition(popup, bEvent );
  XtManageChild(popup);
}

//..MOTIF
void
popupCallback(Widget menuItem, XtPointer client_data, XtPointer call_data )
// Here is which menu item was chosen
{
  char answer[120];
  menuItemChosen = (long int) client_data;  //  XtPointer may be a long int on some
                                            //  machines (alpha,sgi)
// only append the window number to the menu name for pulldown menus (and not the popup's).
// Right now, the pulldown menus are fixed and window dependent, while the popup is 
// user-defined. In the future, we might allow for user-defined pulldowns as well.
  if (menuItemChosen < 0)
  {
    int win;
    int * winInfo_;
    XtPointer winInfo=NULL;
  
    XtVaGetValues(menuItem, XmNuserData, &winInfo, NULL);
    winInfo_ = (int *) winInfo;
    if (winInfo_)
    {
      win = *winInfo_;
//      printf("popupCallback: win=%i\n", win);
      if (win>=0 && win < OGLNWindows)
	sprintf(answer,"%s:%i", XtName(menuItem), win);
      else
	sprintf(answer,"%s", XtName(menuItem)); // the command window has win=-1
    }
    else // if for some reason, the XmNUserData is missing, use the currentWindow
    {
      printf("popupCallCack: invalid window: using OGLCurrentWindow\n");
      sprintf(answer,"%s:%i", XtName(menuItem), OGLCurrentWindow);
    }
  
  }
  else
    sprintf(answer,"%s", XtName(menuItem));

  setMenuNameChosen(answer);
//  setMenuNameChosen(XtName(menuItem));
  exitEventLoop=TRUE;
}

//..MOTIF
void
toggled(Widget widget, XtPointer client_data, XtPointer call_data )
// callback for the axes, labels and rotation marker toggle buttons
{
  ToggleButton & tb = *((ToggleButton *)client_data);
  
  XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) call_data;
  
// remember the state of the button
  tb.state = cbs->set;
  
  char message[200];
  sprintf(message, "%s %i", XtName(widget), cbs->set);

// this command is parsed by processSpecialMenuItems
  menuItemChosen = -1;

  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}

//..MOTIF
static void
rubberBand(Display *disp, Window winid, XEvent *event0, int win_number)
// ===============================================================================
// ZOOM
//
// The user can use the MIDDLE (used to be left) mouse button to make a 
// rubber-band box for zooming
// Return the relative positions of the next bounding box on [0,1]x[0,1]
//
// Here the rubber band box will have crosses in the corners
// ===============================================================================
{
  XEvent event =*event0;
  int x0=event.xbutton.x, y0=event.xbutton.y, x1, y1, x1f, y1f;
  x1=x0;
  y1=y0;

  WINDOW_REC *t;
  if (0 <= win_number && win_number < OGLNWindows)
    t = OGLWindowList[win_number];
  else
    return;

  // *wdh* 090531 -- use real instead of float (will this fix issues will zomming many times?)
  xRubberBandMin =min(x0,x1)/real(t->width);
  xRubberBandMax =max(x0,x1)/real(t->width);
  yRubberBandMin =1.-max(y0,y1)/real(t->height);  // make lower left corner (0,0)
  yRubberBandMax =1.-min(y0,y1)/real(t->height);

  int first=TRUE;

  if( !showRubberBandBox )
  {
    // do not plot the ZOOM and "+" (for wdh at home where this is slow)

    if (event.type == ButtonPress) 
    {
      // printf("rubberBand:zoom: button press \n");
      if (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
      {
        // printf("rubberBand:zoom: (x0,y0)=(%i,%i)\n",x0,y0);
	x1=x0; y1=y0;

	while (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
	{
	  while (XCheckWindowEvent(disp, winid, PointerMotionMask, &event)) 
	  {
	    if (event.type == MotionNotify) 
	    { 
	      x1 = event.xmotion.x;
	      y1 = event.xmotion.y;
              // printf("rubberBand:zoom: (x1,y1)=(%i,%i)\n",x1,y1);

	    }
	  }
	}
      }
      // printf("rubberBand:zoom:Button release (x1,y1)=(%i,%i)\n",x1,y1);
    }
  }
  else
  {
    // *********** draw ZOOM and "+" as the cursor moves ***********
    GLubyte zoom[] =
    {
      0x7f, 0x7e, 0x7e, 0x81, 0x60, 0x42, 0x42, 0x81, 0x30, 0x42, 0x42, 0x81,
      0x18, 0x42, 0x42, 0x81, 0x0c, 0x42, 0x42, 0x99, 0x06, 0x42, 0x42, 0xbd,
      0x03, 0x42, 0x42, 0xe7, 0x7f, 0x7e, 0x7e, 0xc3
    };

    if (event.type == ButtonPress) 
    {
      // printf("rb: button press \n");
      if (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
      {

	glDisable(GL_DEPTH_TEST);  // **** do this !
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, t->width, 0, t->height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

#if defined(OV_USE_MESA) && OV_USE_MESA!=2
	// printf("rb: OV_USE_MESA\n");
	glEnable(GL_BLEND);
	glBlendEquation(GL_LOGIC_OP);
	glBlendEquation(GL_COLOR_LOGIC_OP);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);   // ** reset?? 
#else
	glEnable(GL_LOGIC_OP);
	glEnable(GL_COLOR_LOGIC_OP);
#endif
	glLogicOp(GL_XOR);
	// glLogicOp(GL_INVERT);

	glDrawBuffer(GL_FRONT);  // draw into front buffer (default is back for double buffering)

	glColor3f(1.,1.,1.);
	glLineWidth(1.0);
      
	int xa,xb,ya,yb;

	// printf("rb: (x0,y0)=(%i,%i)\n",x0,y0);
	x1=x0; y1=y0;
	x1f=x0; y1f=y0;

// zoom location
	glRasterPos2i(x0,t->height-y0);
// above
	glBitmap(32,8,5.,-10.,0.,0.,zoom);
	glFlush();

	while (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
	{


	  while (XCheckWindowEvent(disp, winid, PointerMotionMask, &event)) 
	  {
	    if (event.type == MotionNotify) 
	    { 
	      x1 = event.xmotion.x;
	      y1 = event.xmotion.y;
	    }
	  }


	  if (x1 != x1f || y1 != y1f) 
	  {
	    // printf("rb: (x1,y1)=(%i,%i)\n",x1,y1);
	    // Draw "+" at the corners of the rubberband box
	    
	    if( first )
	    { 
// the top left corner
	      xa = x0; ya=t->height-y0;
	      glBegin(GL_LINES);
	      glVertex2i(xa-3, ya);
	      glVertex2i(xa+3, ya);
	      glVertex2i(xa, ya-3);
	      glVertex2i(xa, ya+3);
	      glEnd();
	      first=FALSE;
	    }
	    else
	    { // erase old marks by drawing with XOR
	      xa=x0, ya=t->height-y0, xb=x1f, yb=t->height-y1f;
// erase the old line
	      xa=x0, ya=t->height-y0, xb=x1f, yb=t->height-y1f;
	      glBegin(GL_LINES);
	      glVertex2i(xa-3, yb);
	      glVertex2i(xa+3, yb);
	      glVertex2i(xa, yb-3);
	      glVertex2i(xa, yb+3);
	      glVertex2i(xb-3,ya);
	      glVertex2i(xb+3,ya);
	      glVertex2i(xb,ya-3);
	      glVertex2i(xb,ya+3);
	      glEnd();
	      glFlush();   /* Added by Brian Paul */

// erase zoom below
	      glRasterPos2i(xb,yb);
	      glBitmap(32,8,5.,+20.,0.,0.,zoom);
	      glFlush();
	    }
	    xa=x0, ya=t->height-y0, xb=x1, yb=t->height-y1;
// new zoom below
	    glRasterPos2i(xb,yb);
	    glBitmap(32,8,5.,+20.,0.,0.,zoom);
	    glFlush();

// then draw the new line
	    xa=x0, ya=t->height-y0, xb=x1, yb=t->height-y1;
	    glBegin(GL_LINES);
	    glVertex2i(xa-3, yb);
	    glVertex2i(xa+3, yb);
	    glVertex2i(xa, yb-3);
	    glVertex2i(xa, yb+3);
	    glVertex2i(xb-3,ya);
	    glVertex2i(xb+3,ya);
	    glVertex2i(xb,ya-3);
	    glVertex2i(xb,ya+3);
	    glEnd();
	    glFlush();   /* Added by Brian Paul */

	    x1f=x1; y1f=y1;  // save old values
	  }
	} // end while (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 

//  Erase marks
	xa=x0, ya=t->height-y0, xb=x1, yb=t->height-y1;
// zoom above
	glRasterPos2i(xa,ya);
	glBitmap(32,8,5.,-10.,0.,0.,zoom);

// erase the old crosses
	glBegin(GL_LINES);
	glVertex2i(xa-3, yb);
	glVertex2i(xa+3, yb);
	glVertex2i(xa, yb-3);
	glVertex2i(xa, yb+3);
	glVertex2i(xb-3,ya);
	glVertex2i(xb+3,ya);
	glVertex2i(xb,ya-3);
	glVertex2i(xb,ya+3);
	glEnd();

// erase zoom below
	if (!first)
	{
	  glRasterPos2i(xb,yb);
	  glBitmap(32,8,5.,+20.,0.,0.,zoom);
// top left corner
	  glBegin(GL_LINES);
	  glVertex2i(xa-3, ya);
	  glVertex2i(xa+3, ya);
	  glVertex2i(xa, ya-3);
	  glVertex2i(xa, ya+3);
	  glEnd();
	}

	glFlush(); 
	
      //  printf("rb: rectangle = [%e,%e]X[%e,%e] \n",xRubberBandMin,xRubberBandMax,yRubberBandMin,yRubberBandMax);

	glDrawBuffer(GL_BACK);
#if defined(OV_USE_MESA) && OV_USE_MESA!=2
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_LOGIC_OP);
#else
	glDisable(GL_COLOR_LOGIC_OP);
	glDisable(GL_LOGIC_OP);
#endif
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // Restoring the projection matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix(); // Restoring the modelview matrix
	
      }
    }
  }


// Make sure the rectangle is big enough
  xRubberBandMin =min(x0,x1)/real(t->width);
  xRubberBandMax =max(x0,x1)/real(t->width);
  yRubberBandMin =1.-max(y0,y1)/real(t->height);  // make lower left corner (0,0)
  yRubberBandMax =1.-min(y0,y1)/real(t->height);

// the zoom is not valid if the rectangle is outside the window, or if the rectangle is smaller
// than 5 pixels
  if( min(x0,x1) >= 0 && max(x0,x1) <= t->width && 
      min(y0,y1) >= 0 && max(y0,y1) <= t->height &&
      abs(x1-x0)>5 && abs(y1-y0)>5 )
  {
    real xa=xRubberBandMin, xb=xRubberBandMax;
    real ya=yRubberBandMin, yb=yRubberBandMax;
    real aspectRatio=t->width/real(t->height);
    real magnify,xShift,yShift;
    if( aspectRatio>1. )
    {
      xShift=(1.-(xa+xb))*aspectRatio;
      yShift=1.-(ya+yb);
      magnify=1./max(.005,max((xb-xa)*aspectRatio,yb-ya));  // *wdh* 090531 -- this helps fix zooms to be centered
    }
    else
    {
      xShift=1.-(xa+xb);  // shift to center .5(xMin+xMax)
      yShift=(1.-(ya+yb))/aspectRatio;
      magnify=1./max(.005,max(xb-xa,(yb-ya)*aspectRatio));
    }

    // real magnify=1./max(.005,max(xb-xa,yb-ya));
    // printf("rb: zoom: xShift=%e, yShift=%e, magnify=%e aspectRatio=%e\n",xShift,yShift,magnify,aspectRatio);
    if( viewFunction )
      viewFunction( graphicsInterfacePointer, win_number, xShift, yShift, 0., 0., 0., 0., magnify);
// draw from the new view point
    draw(t->plotStuff);
  }
      

  return;
}


static void
rubberBandPickNew(Display *disp, Window winid, XEvent *event0, int win_number)
//=======================================================================================
// PICKING
//
// The user can use the LEFT mouse button to make a rubber-band box for zooming
// Return the relative positions of the next bounding box on [0,1]x[0,1]
//
// Here the rubber band box will be a frame
//=======================================================================================
{
  XEvent event =*event0;
  int x0=event.xbutton.x, y0=event.xbutton.y, x1, y1, x1f, y1f;
  x1=x0;
  y1=y0;

  WINDOW_REC *t;
  if (0 <= win_number && win_number < OGLNWindows)
    t = OGLWindowList[win_number];
  else
    return;

  xRubberBandMin =min(x0,x1)/real(t->width);
  xRubberBandMax =max(x0,x1)/real(t->width);
  yRubberBandMin =1.-max(y0,y1)/real(t->height);  // make lower left corner (0,0)
  yRubberBandMax =1.-min(y0,y1)/real(t->height);

  int first=TRUE;
  if( !showRubberBandBox )
  {

    if (event.type == ButtonPress) 
    {
      // printf("rubberBand:pick: button press \n");
      if (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
      {
	// printf("rubberBand:pick: (x0,y0)=(%i,%i)\n",x0,y0);
	x1=x0; y1=y0;
        x1f=x0; y1f=y0;
	while (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
	{
	  while (XCheckWindowEvent(disp, winid, PointerMotionMask, &event)) 
	  {
	    if (event.type == MotionNotify) 
	    { 
	      x1 = event.xmotion.x;
	      y1 = event.xmotion.y;
              // printf("rubberBand:zoom: (x1,y1)=(%i,%i)\n",x1,y1);
	    }
	  }
          // printf("rubberBand:zoom: no motion (x1,y1)=(%i,%i)\n",x1,y1);
	}
        // printf("rubberBand:zoom: button release motion (x1,y1)=(%i,%i)\n",x1,y1);
      } 
    }
  }
  else
  {

    GLubyte pick[] =
    {
      0x20, 0x8f, 0x4c, 0x00, 0x20, 0x98, 0x5c, 0x00, 0x20, 0x90, 0x70, 0x00,
      0x3e, 0x90, 0x60, 0x00, 0x22, 0x90, 0x70, 0x00, 0x22, 0x90, 0x58, 0x00,
      0x22, 0x98, 0x4c, 0x00, 0x3e, 0x8f, 0x44, 0x00};

    if (event.type == ButtonPress) 
    {
      // printf("rb: button press \n");
      if (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
      {

	glDisable(GL_DEPTH_TEST);  // **** do this !
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, t->width, 0, t->height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

#if defined(OV_USE_MESA) && OV_USE_MESA!=2
	glEnable(GL_BLEND);
	glBlendEquation(GL_LOGIC_OP);
	glBlendEquation(GL_COLOR_LOGIC_OP);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);   // ** reset?? 
#else
	glEnable(GL_LOGIC_OP);
	glEnable(GL_COLOR_LOGIC_OP);
#endif
	glLogicOp(GL_XOR);
	// glLogicOp(GL_INVERT);

	glDrawBuffer(GL_FRONT);  // draw into front buffer (default is back for double buffering)

// line stippling
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xAAA);

	glColor3f(1.,1.,1.);
	glLineWidth(1.0);

	int xa,xb,ya,yb;

	// printf("rb: (x0,y0)=(%i,%i)\n",x0,y0);
	x1=x0; y1=y0;
	x1f=x0; y1f=y0;

// pick above
	glRasterPos2i(x0,t->height-y0);
	glBitmap(32,8,5.,-10.,0.,0.,pick);
	glFlush();

	while (!XCheckMaskEvent(disp, ButtonReleaseMask, &event)) 
	{
	  while (XCheckWindowEvent(disp, winid, PointerMotionMask, &event)) 
	  {
	    if (event.type == MotionNotify) 
	    { 
	      x1 = event.xmotion.x;
	      y1 = event.xmotion.y;
	    }
	  }
	  if (x1 != x1f || y1 != y1f) 
	  {

	    // printf("rb: (x1,y1)=(%i,%i)\n",x1,y1);
	    // Draw "+" at the corners of the rubberband box
	    
//  #ifdef OV_USE_MESA
	    if( first )
	    { 
	      first=FALSE;
	    }
	    else
	    { // erase old marks by drawing with XOR
	      xa=x0, ya=t->height-y0, xb=x1f, yb=t->height-y1f;
// old pick below
	      glRasterPos2i(xb,yb);
	      glBitmap(32,8,5.,+20.,0.,0.,pick);

// erase the old line
	      xa=x0, ya=t->height-y0, xb=x1f, yb=t->height-y1f;
	      glBegin(GL_LINE_LOOP);
	      glVertex2i(xa, ya);
	      glVertex2i(xb, ya);
	      glVertex2i(xb,yb);
	      glVertex2i(xa,yb);
	      glEnd();
	      glFlush();   /* Added by Brian Paul */
	    }
	    xa=x0, ya=t->height-y0, xb=x1, yb=t->height-y1;
// new pick below
	    glRasterPos2i(xb,yb);
	    glBitmap(32,8,5.,+20.,0.,0.,pick);

// then draw the new line
	    xa=x0, ya=t->height-y0, xb=x1, yb=t->height-y1;
	    glBegin(GL_LINE_LOOP);
	    glVertex2i(xa, ya);
	    glVertex2i(xb, ya);
	    glVertex2i(xb,yb);
	    glVertex2i(xa,yb);
	    glEnd();
	    glFlush();   /* Added by Brian Paul */

	    x1f=x1; y1f=y1;  // save old values
	  }
	} // end while !release
      

//  Erase marks
	xa=x0, ya=t->height-y0, xb=x1, yb=t->height-y1;
	glRasterPos2i(xa,ya);
// pick above
	glBitmap(32,8,5.,-10.,0.,0.,pick);
// pick below
	if (!first)
	{
	  glRasterPos2i(xb,yb);
	  glBitmap(32,8,5.,+20.,0.,0.,pick);
	}
      
// erase lines
	glBegin(GL_LINE_LOOP);
	glVertex2i(xa, ya);
	glVertex2i(xb, ya);
	glVertex2i(xb,yb);
	glVertex2i(xa,yb);
	glEnd();
	glFlush();   /* Added by Brian Paul */


	glDisable(GL_LINE_STIPPLE);

	glDrawBuffer(GL_BACK);
#if defined(OV_USE_MESA) && OV_USE_MESA!=2
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_LOGIC_OP);
#else
	glDisable(GL_COLOR_LOGIC_OP);
	glDisable(GL_LOGIC_OP);
#endif
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // Restoring the projection matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix(); // Restoring the modelview matrix
	
      }
      
    }
  }
  
  // Make sure the rectangle is big enough
	
  xRubberBandMin =min(x0,x1)/real(t->width);
  xRubberBandMax =max(x0,x1)/real(t->width);
  yRubberBandMin =1.-max(y0,y1)/real(t->height);  // make lower left corner (0,0)
  yRubberBandMax =1.-min(y0,y1)/real(t->height);

      //  printf("rb: rectangle = [%e,%e]X[%e,%e] \n",xRubberBandMin,xRubberBandMax,yRubberBandMin,yRubberBandMax);
      // the pick is not valid if the rectangle is outside the window.
  if( min(x0,x1) >= 0 && max(x0,x1) <= t->width && 
      min(y0,y1) >= 0 && max(y0,y1) <= t->height )
  {
    exitEventLoop=TRUE;
    setMenuNameChosen("");
    pickOption = 1; // remember the button
    pickWindow = win_number; // remember the window
  }
      
  return;
      
}

//..MOTIF
static void
setView(const int & type, Display *disp, Window winid, XEvent *event0, int win_number)
//
//   Mouse Driven rotate/translate/scale function
//
//   type=0 : translate in x/y
//       =1 : rotate about x/y axes
//       =2 : magnify + rotate about the z axes
//       =3 : translate in z
{
  XEvent event =*event0;
  int x0=event.xbutton.x, y0=event.xbutton.y, x1, y1, x1f, y1f;
  x1=x0;
  y1=y0;
  real xShift=0.,yShift=0.,zShift=0.,dThetaX=0.,dThetaY=0.,dThetaZ=0.,magnify=1.;
  const real rotationFactor = 0.5;
  
  WINDOW_REC *t;
  if (0 <= win_number && win_number < OGLNWindows)
    t = OGLWindowList[win_number];
  else
    return;

//    Dimension width, height;
//    XtVaGetValues(t->plotStuff, XmNwidth, &width, XmNheight, &height, NULL);
//    assert( width>0 && height>0 );

  x1=x0; y1=y0;
  x1f=x0; y1f=y0;

  int dummy;
  Window dummyWindow;  // *wdh* 050719 -- need to use the correct type here for 64bit machines.
  unsigned int button_mask;
    
  do {
// printf("setView: (x0,y0)=(%i,%i)\n",x0,y0);

// when 2 buttons are pressed, the following loop is not always entered
//      while (XCheckWindowEvent(disp, winid, PointerMotionMask, &event)) 
//      {
//      }
    
// instead query X where the pointer is
    XQueryPointer(dpy, XtWindow(t->plotStuff), &dummyWindow, &dummyWindow, 
		  (int *) &dummy, (int *) &dummy, &x1, &y1, &button_mask);
  
    if (x1 != x1f || y1 != y1f) 
    {
      if( type==0 )
      { // translate
// printf("shift: (x1,y1)=(%i,%i)\n",x1,y1);
	xShift= (x1-x1f)/real(t->width);
	yShift=-(y1-y1f)/real(t->height);
      }
      else if( type==1 )
      { // rotate
	// printf("rotate: (x1,y1)=(%i,%i)\n",x1,y1);
	dThetaX= rotationFactor*(y1-y1f)/real(t->height);
	dThetaY= rotationFactor*(x1-x1f)/real(t->width);
      }
      else if( type==2 )
      {
	// printf("magnify (x1,y1)=(%i,%i)\n",x1,y1);
	magnify=max(.001,1.+.9*(y1-y1f)/real(t->height));
	dThetaZ= rotationFactor*(x1-x1f)/real(t->width);
      }
      else if( type==3 )
      {
	// printf("zShift (x1,y1)=(%i,%i)\n",x1,y1);
	zShift=  (x1-x1f)/real(t->width);
      }
//        else if( type==4 )
//        {
//  	dThetaZ= rotationFactor*(x1-x1f)/real(t->width)) +
//  	  (x1-x1f)/real(t->width);
//        }

// change the view point
      if(viewFunction)
	viewFunction( graphicsInterfacePointer, win_number, xShift, yShift, zShift,
		      dThetaX, dThetaY, dThetaZ, magnify);

// draw from the new view point
      draw(t->plotStuff);
	
      x1f=x1; y1f=y1;  // save old values
    }
      
  } while(!XCheckMaskEvent(disp, ButtonReleaseMask, &event));
  
// AP: experimenting
//  printf("End of loop in setview\n");
  moglPostDisplay(win_number);

//  printf("Exiting setView x1=%i, x0=%i, y1=%i, y0=%i\n", x1, x0, y1, y0);
  
  return;
}

bool
moglRotationKeysPressed(int win_number)
{
  bool mouseButtonsDown=false;
  
  WINDOW_REC *t;
  if (0 <= win_number && win_number < OGLNWindows)
    t = OGLWindowList[win_number];
  else
    return false;

/* check which buttons are held down */
  int dummy;
  Window dummyWindow;  // *wdh* 050719 -- need to use the correct type here for 64bit machines.
  unsigned int button_mask;
  Widget w=t->toplevel;

  XQueryPointer(dpy, XtWindow(w), &dummyWindow, &dummyWindow, 
		(int *) &dummy, (int *) &dummy, &dummy, &dummy, &button_mask);

//  /* filter out the interesting buttons */
  button_mask &= (ShiftMask | ControlMask | Button1Mask | Button2Mask | Button3Mask); // AP: Need to add ControlMask for 2 button mouses?
  if ((button_mask & ShiftMask) || (button_mask & ControlMask) )
  {
    mouseButtonsDown = (button_mask & Button1Mask) || (button_mask & Button2Mask) || (button_mask & Button3Mask);
  }
  else
    mouseButtonsDown = false;

//  printf("Rotation keys are %s\n", mouseButtonsDown? "down": "up");
  
  return mouseButtonsDown;
}

//..MOTIF
void
drawAreaInput(Widget w, XtPointer clientData, XtPointer callData)
// detect motion and key presses in the drawing area
{
  XmDrawingAreaCallbackStruct *cd = (XmDrawingAreaCallbackStruct *) callData;
  char            buffer[1];
  KeySym          keysym;
//  Position x,y;
  Window win;

  int win_number;
  WINDOW_REC *t;
  
  if (window_exists(XtWindow(w), &win_number))
  {
    t = OGLWindowList[win_number];
    moglMakeCurrent(win_number);
  }
  else
    return;

  if( true )
  {
    // Here we check if NumLock is on : (I wonder if this always works?) *wdh* 090423
    if( ((XKeyEvent *)cd->event)->state & 0x10 )
    {
      printf("mogl::drawAreaInput:ERROR: NumLock detected! You should turn off NumLock or the mouse buttons "
             "won't work properly.\n");
    }
  }

  switch (cd->event->type) 
  {
  case KeyRelease:
    /*
     * It is necessary to convert the keycode to a keysym before it is
     * possible to check if it is an escape
     */
    XLookupString((XKeyEvent *) cd->event, buffer, 1, &keysym, NULL);
    switch (keysym) 
    {
    case XK_Up:
      // printf("up-arrow\n");
      break;
    case XK_Down:
      // printf("down-arrow\n");
      break;
    case XK_Left:
      // printf("left-arrow\n");
      break;
    case XK_Right:
      // printf("right-arrow\n");
      break;
    case XK_S: case XK_s: /* the S key */
      // printf("s-key\n");
      break;
    case XK_Shift_L:
      // printf("key release: left shift\n");
      break;
    case XK_Control_L:
      // printf("key release: left control\n");
      break;
    case XK_Escape:
      exit(0);
    }
    break;

  case ButtonPress:
    
/* check which buttons are held down */
    int dummy;
    Window dummyWindow;  // *wdh* 050719 -- need to use the correct type here for 64bit machines.
    unsigned int button_mask;
    
    XQueryPointer(dpy, XtWindow(w), &dummyWindow, &dummyWindow, 
		  (int *) &dummy, (int *) &dummy, &dummy, &dummy, &button_mask);
      
//  /* filter out the interesting buttons */
    button_mask &= (ShiftMask | ControlMask | Button1Mask | Button2Mask | Button3Mask); // AP added ControlMask for 2 button mouses

    switch (button_mask)
    {
//      case (Button2Mask): //  z-rotate
//  // set cursor
//        windowAttributes.cursor=rotateCursor;
//        XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes );
//        XFlush(dpy);

//        win =XtWindow(w); // *wdh*
//        setView(4, dpy, win, cd->event, win_number); 
//        break;
    case (ShiftMask |Button1Mask): // <shift>button1 = translate
// set cursor
      windowAttributes.cursor=translateCursor;
      XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes );
      XFlush(dpy);

      win =XtWindow(w);
      setView(0, dpy, win, cd->event, win_number); 
      break;
    case (ControlMask |Button3Mask): // <ctrl>button3 = rotate (2button mouse)
    case (ShiftMask |Button2Mask): // <shift>button2 = rotate
// set cursor
      windowAttributes.cursor=dotCursor;
      XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes );
      XFlush(dpy);

      win =XtWindow(w);
      setView(1, dpy, win, cd->event, win_number); 
      break;
    case (ShiftMask |Button3Mask): // (horizontal): z-rotate,  (vertical): magnify
// set cursor
      windowAttributes.cursor=boxSpiralCursor;
      XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes );
      XFlush(dpy);

      win =XtWindow(w);
      setView(2, dpy, win, cd->event, win_number); //magnify + z-rotate
        
      break;
    case (ControlMask |Button1Mask): // <ctrl>button1 = rubberband zoom (2button mouse)
    case (Button2Mask ): // rubberband zoom
// set cursor
      windowAttributes.cursor=zoomCursor;
      XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes );
      XFlush(dpy);

      win =XtWindow(w);
      rubberBand(dpy, win, cd->event, win_number);
      break;
    case (Button1Mask ): // Selecting objects near a point or in a rubberbanded region
// AP: new stuff
      if (readyToPick)
      {
// set cursor
	windowAttributes.cursor=crossCursor;
	XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes );
	XFlush(dpy);

	win =XtWindow(w);
	rubberBandPickNew(dpy, win, cd->event, win_number);
//	rubberBandPick(dpy, win, cd->event, win_number);
      }
      
      break;
    } // end switch(button_mask)
  
    break;
  case ButtonRelease:
    break;
    
  } // end switch(event_type)
  
  
  // set cursor back to normal
  windowAttributes.cursor=None;
  XChangeWindowAttributes(dpy,XtWindow(t->toplevel),CWCursor,&windowAttributes );
  XFlush(dpy);

}

//..MOTIF
void
buttonCallback(Widget widget, XtPointer client_data, XtPointer call_data )
// call back for right column buttons
{
  menuItemChosen = -1; // ****
// append :"window number" to the button name
  char answer[120];
  int win;
  int * winInfo_;
  XtPointer winInfo=NULL;
  
  XtVaGetValues(widget, XmNuserData, &winInfo, NULL);
  winInfo_ = (int *) winInfo;
  if (winInfo_)
  {
    win = *winInfo_;
//     printf("buttonCallback: win=%i\n", win);
    if (win>=0 && win < OGLNWindows)
      sprintf(answer,"%s:%i", XtName(widget), win);
    else
      sprintf(answer,"%s", XtName(widget)); // the command window has win=-1
  }
  else
  {
    printf("buttonCallCack: invalid window: using OGLCurrentWindow\n");
    sprintf(answer,"%s:%i", XtName(widget), OGLCurrentWindow);
  }

  setMenuNameChosen(answer);
  exitEventLoop=TRUE;
}

//..MOTIF
static void
userOpMenuCallback(Widget widget, XtPointer client_data, XtPointer call_data )
// call back for userdefined buttons
{
  DialogData * dialogSpec_ = NULL;
  XtVaGetValues(widget, XmNuserData, (XtPointer) &dialogSpec_, NULL);

  OptionMenu & om = *((OptionMenu *)client_data);

// remeber the current choice
  om.currentChoice = XtName(widget); // the command is the same as the name of the widget

  if (dialogSpec_)
    menuItemChosen = (dialogSpec_->getBuiltInDialog())? -1 : 1;
  else
    menuItemChosen = 1; 
//  printf("userOpMenuCallback: menuItemChosen = %i\n", menuItemChosen);
  
  setMenuNameChosen(XtName(widget));
  exitEventLoop=TRUE;
}

static void
userRadioBoxCallback(Widget widget, XtPointer client_data, XtPointer call_data )
// call back for userdefined buttons
{
  DialogData * dialogSpec_ = NULL;
  XtVaGetValues(widget, XmNuserData, (XtPointer) &dialogSpec_, NULL);

  RadioBox & rb = *((RadioBox *)client_data);

  XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) call_data;

// only issue a command for activating a toggle button
  if (cbs->set)
  {
    if (dialogSpec_)
      menuItemChosen = (dialogSpec_->getBuiltInDialog())? -1 : 1;
    else
      menuItemChosen = 1; 
//  printf("userOpMenuCallback: menuItemChosen = %i\n", menuItemChosen);
    setMenuNameChosen(XtName(widget));
    exitEventLoop=TRUE;
  }
}

static void
userButtonCallback(Widget widget, XtPointer client_data, XtPointer call_data )
// call back for userdefined buttons
{
// Note that the dialog window is closed by calling closeDialog from the application.
  if (client_data)
  {
    DialogData & dialogSpec = *((DialogData *)client_data);
    menuItemChosen=(dialogSpec.getBuiltInDialog())? -1:1;
  }
  else
    menuItemChosen = 1; 

  setMenuNameChosen(XtName(widget));
  exitEventLoop=TRUE;
}

static void
userToggleButtonCallback( Widget widget, XtPointer client_data, XtPointer call_data )
// call back for userdefined toggle buttons
{
  ToggleButton & tb = *((ToggleButton *)client_data);
  
  XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) call_data;
  
  DialogData * dialogSpec_=NULL;
  XtVaGetValues(widget, XmNuserData, (XtPointer) &dialogSpec_, NULL);

// remember the state of the button
  tb.state = cbs->set;
  
  char message[200];
  sprintf(message, "%s %i", XtName(widget), cbs->set);

  if (dialogSpec_)
    menuItemChosen = (dialogSpec_->getBuiltInDialog())? -1:1;
  else
    menuItemChosen = 1;
//  printf("userToggleButtonCallback: menuItemChosen = %i\n", menuItemChosen);

  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}

static void
userTextLabelCallback( Widget widget, XtPointer client_data, XtPointer call_data )
// call back for userdefined textLabels
{
  TextLabel & tl = *((TextLabel *)client_data);

  DialogData * dialogSpec_=NULL;
  XtVaGetValues(widget, XmNuserData, (XtPointer) &dialogSpec_, NULL);

  char message[200];

// copy the string in the text window
  char *tmpString = XmTextGetString(widget);
  tl.string = tmpString;
  XtFree(tmpString); // free the string allocated by XmTextGetString
  
  sprintf(message, "%s %s", XtName(widget), SC tl.string.c_str()); 

  if (dialogSpec_)
    menuItemChosen = (dialogSpec_->getBuiltInDialog())? -1:1;
  else
    menuItemChosen = 1;
//  printf("userTextLabelCallback: menuItemChosen = %i\n", menuItemChosen);
  
  setMenuNameChosen(message);
  exitEventLoop=TRUE;
  
}

static int
window_exists(Window winid, int *pos){
/* checks if the window `winid' exists and in that case, returns the location */
/* of that window in the window array */
  *pos=0;
  
  if (winid == (Window) NULL)
    return FALSE;
  if (OGLNWindows > 0) {
    while ((*pos < OGLNWindows) && (winid != XtWindow(OGLWindowList[*pos]->plotStuff)) )
      (*pos)++;
    
    return(*pos != OGLNWindows);
  } else
    return(FALSE);
}

static int
windowExistsTop(Window winid, int *pos){
/* checks if the window `winid' exists and in that case, returns the location */
/* of that window in the window array */
/* This function compares the toplevel window instead of the plotStuff window and is */
/* used to identify button callbacks */
  *pos=0;
  
  if (winid == (Window) NULL)
    return FALSE;
  if (OGLNWindows > 0) {
    while ((*pos < OGLNWindows) && (winid != XtWindow(OGLWindowList[*pos]->toplevel)) )
      (*pos)++;
    
    return(*pos != OGLNWindows);
  } else
    return(FALSE);
}

//..MOTIF
void 
eventLoop()
//--------------------------------------------
// /Description:
// Process all current events. Exit when some call-back routine sets the global variable
// exitEventLoop=TRUE. Note that this function
// is very similar to moglPollEvents(), except that this function is blocking. Hence, if there
// are no pending events, the program will wait in this routine until an event occurs.
//
// /Return values: none.
// /Author: WDH \& AP
{
  // first set cursor back to normal from a "watch symbol"
  WINDOW_REC *w;
  int win_number, oldCurrentWindow=OGLCurrentWindow;

// set cursor to the pointer symbol
  for (win_number=0; win_number<OGLNWindows; win_number++)
  {
    w = OGLWindowList[win_number];
    windowAttributes.cursor=None;
    XChangeWindowAttributes(dpy,XtWindow(w->toplevel),CWCursor,&windowAttributes );
  }
// also in the text window
  XChangeWindowAttributes(dpy,XtWindow(textw),CWCursor,&windowAttributes );

  XFlush(dpy);

  XEvent event;
  
  exitEventLoop=FALSE;
  while( !exitEventLoop )
  {
// we need to update all windows before we make our selfs too comfortable...
    for (win_number=0; win_number<OGLNWindows; win_number++)
    {
      w = OGLWindowList[win_number];

      if(w && w->postDisplay )// some w might be destroyed... (w=NULL)
      {
// and draw...
	draw(w->plotStuff);
	w->postDisplay=GL_FALSE;
      }
    }

    XtAppNextEvent(xtAppContext, &event);   // we wait here when no events are pending
    XtDispatchEvent(&event);
    
  }

// set cursor back to "watch symbol"
  for (win_number=0; win_number<OGLNWindows; win_number++)
  {
    w = OGLWindowList[win_number];
    windowAttributes.cursor=cursor;
    XChangeWindowAttributes(dpy,XtWindow(w->toplevel),CWCursor,&windowAttributes );
  }
// also in the text window
  XChangeWindowAttributes(dpy,XtWindow(textw),CWCursor,&windowAttributes );

  XFlush(dpy);

// set back the window focus
  moglMakeCurrent(oldCurrentWindow);
  
}

//..MOTIF
void 
moglPollEvents()
//--------------------------------------------
// /Description:
// Process all current events. Exit when there are no more events. Note that this function
// is very similar to the internal event loop in mogl.C, except that this function is non-blocking.
// Note that this routine can be called from anywhere in an application code to update the
// windows, parse any pending commands, etc. This might for instance be useful during 
// a long computation.
//
// /Return values: none.
// /Author: AP
{
  // first set cursor back to normal from a "watch symbol"
  WINDOW_REC *w;
  int win_number, oldCurrentWindow=OGLCurrentWindow;

// set cursor to the pointer symbol
  for (win_number=0; win_number<OGLNWindows; win_number++)
  {
    w = OGLWindowList[win_number];
    windowAttributes.cursor=None;
    XChangeWindowAttributes(dpy,XtWindow(w->toplevel),CWCursor,&windowAttributes );
  }

// also in the text window
  XChangeWindowAttributes(dpy,XtWindow(textw),CWCursor,&windowAttributes );

  XFlush(dpy);

  XEvent event;
  
// *wdh*  while( XtAppPending(xtAppContext) )
  exitEventLoop=FALSE;
  while( !exitEventLoop && XtAppPending(xtAppContext) )
  {
// we need to update all windows before we make our selfs too comfortable...
    for (win_number=0; win_number<OGLNWindows; win_number++)
    {
      w = OGLWindowList[win_number];

      if(w && w->postDisplay )// some w might be destroyed... (w=NULL)
      {
// and draw...
	draw(w->plotStuff);
	w->postDisplay=GL_FALSE;
      }
    }
    
    XtAppNextEvent(xtAppContext, &event);  

    // printf("process event: event.type=%i menuNameChosen=[%s]\n",event.type,menuNameChosen==NULL ? "" : menuNameChosen);

    XtDispatchEvent(&event);
    
  }

// set cursor back to "watch symbol"
  for (win_number=0; win_number<OGLNWindows; win_number++)
  {
    w = OGLWindowList[win_number];
    windowAttributes.cursor=cursor;
    XChangeWindowAttributes(dpy,XtWindow(w->toplevel),CWCursor,&windowAttributes );
  }

// also in the text window
  XChangeWindowAttributes(dpy,XtWindow(textw),CWCursor,&windowAttributes );

  XFlush(dpy);

// set back the window focus
  moglMakeCurrent(oldCurrentWindow);
  
}

//..MOTIF because viewFunction is changed
void 
getRubberBandBoxCorners( real & xMin, real & xMax, real & yMin, real & yMax )
// Wait for the user to choose a cursor position or a rubber band box
{
  // turn off view:
  MOGL_VIEW_FUNCTION *viewSave = viewFunction;       // ?? Why is this done??
  viewFunction=NULL;

  eventLoop();
  // printf(" exit event loop, menu chosen = %i, name=%s \n",menuItemChosen,menuNameChosen);
  xMin=xRubberBandMin;
  xMax=xRubberBandMax;
  yMin=yRubberBandMin;
  yMax=yRubberBandMax;

  viewFunction=viewSave; // reset
}

    

// ********************** clip callback functions ***************************

//void
//setMenuChosen( const int & menuItem, char* answer );

// this global variable is used by the following macros, and it needs to be assigned 
// to make the macros work
static ClippingPlaneInfo *clippingPlaneInfo; 

#define clipPlane(plane,i) *(clippingPlaneInfo->clippingPlaneEquation+(i) \
                            +(plane)*4)

#define clipPlaneIsOn(plane) *(clippingPlaneInfo->clippingPlaneIsOn+(plane))


static char buff[80];


static void 
destroyClipPlaneDialog( Widget w, XtPointer client_data, XtPointer call_data)
{
// retrieve the window number
  int *win_ = (int *) client_data;

// remember that the view char dialog is closed
  OGLWindowList[*win_]->clipPlaneOpen = 0;
  
// free the memory allocated for win_
  free(win_);

  XtDestroyWidget(w);
}


void
clipPlaneOnOff( Widget widget, XtPointer clinet_data, XtPointer call_data )
{
  XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;
//  printf("%s: %s\n", XtName(widget), state->set ? "on" : "off" );

  sprintf(buff,"%s %s",XtName(widget),state->set ? "on" : "off" );
//  printf("menu: %s \n",buff);
  setMenuChosen(-99,buff);
}


void
scaleCallback(Widget widget, XtPointer clinet_data, XtPointer call_data )
{
  XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *) call_data;
// printf("%s: %d\n",XtName(widget), cbs->value);

  sprintf(buff,"%s %e",XtName(widget),cbs->value/100.);
  // printf("menu: %s \n",buff);
  setMenuChosen(-99,buff);
}

void
changeNormal(Widget widget, XtPointer clinet_data, XtPointer call_data )
// This widget is called when a normal value is changed
{
  char *message;
  message = XmTextGetString(widget);
//XmTextSetString(widget,"");       // reset text to blank

//  printf("change %s: new value = %s\n",XtName(widget),message);
  
  real n0=0.,n1=0.,n2=-1.;
//  printf("changeNormal message: %s \n",message);
  sScanF( message,"%e,%e,%e",&n0,&n1,&n2 ); 
  // printf("set normal=(%e,%e,%e) for plane %i \n",n0,n1,n2,plane);

  // kkc cast to real so that the sgi does not complain
  // wdh cast to double
  real norm=pow(double(n0*n0+n1*n1+n2*n2),double(.5));
  if( norm == 0. )
  {
    printf("changeNormal: invalid clipping plane normal, cannot be all zeroes! \n");
    norm = 1.;
    n2=-1;
  }
  sprintf(buff,"%3.2f, %3.2f, %3.2f",n0/norm,n1/norm,n2/norm);
  XmTextSetString(widget,buff);

  sprintf(buff,"%s %s",XtName(widget),message);
  // printf("menu: %s \n",buff);
  setMenuChosen(-99,buff);


}

// =====================================================================================
/// \brief call-back function to position a dialog on the scree
/// \details The location of the dialog can be set from the .overturerc file.
// =====================================================================================
void
positionDialog(Widget dialog, XtPointer client_data, XtPointer call_data )
{

  int positioned; // Keeps track if this dialog has already been positioned
  
  Dimension w, h;
  XtVaGetValues(dialog, XmNuserData, &positioned, XmNwidth, &w, XmNheight, &h, NULL );
  
  //  printf("positioned: %i\n", positioned);

  if (!positioned)
  {
    int width = WidthOfScreen(XtScreen(dialog));
    int height = HeightOfScreen(XtScreen(dialog));
  
    Dimension xPosition, yPosition;
    // put the dialog in the top right corner of the screen
    xPosition=width-w; yPosition=0;  // top right corner

    // Could place dialog next to main graphics window? 
    if( dialogNx>=0  )
      xPosition=dialogNx;
    if( dialogNx>=0 )
      yPosition=dialogNy;
    
    // -- new dialogs are shifted in the x and y directions
    xPosition+=currentDialogNx;
    currentDialogNx += dialogOffsetNx;  
    if( currentDialogNx>width-100 ) currentDialogNx=0;  // wrap if there are too many 

    yPosition+=currentDialogNy;
    currentDialogNy += dialogOffsetNy;
    if( currentDialogNy>height-300 ) currentDialogNy=0;
    
    XtVaSetValues(dialog,
		  XmNx, xPosition,
		  XmNy, yPosition,
		  XmNuserData, (XtPointer) 1, // only position the dialog the first time
		  NULL);
  }
  

}

void
clippingPlanesDialog(Widget pb, XtPointer client_data, XtPointer call_data)
{
  Widget dialog;
  XmString t;
  Arg args[7];
  int n = 0;
  int win;
  int * winInfo_;
  XtPointer winInfo=NULL;
  
  XtVaGetValues(pb, XmNuserData, &winInfo, NULL);
  winInfo_ = (int *) winInfo;
  if (winInfo_)
  {
    win = *winInfo_;
//    printf("clippingPlanesDialog: win = %i\n", win);
  }
  else
  {
    win = OGLCurrentWindow;
    printf("clippingPlanesDialog: invalid window: using OGLCurrentWindow\n");
  }

// check if this dialog already is opened
  if (OGLWindowList[win]->clipPlaneOpen)
    return;

  // retrieve the pointer to the clipping plane info
  clippingPlaneInfo=(ClippingPlaneInfo *)client_data;

  // Create the dialog -- the PushButton acts as the DialogShell's
  // parent (not the parent of the PromptDialog).  The "userData"
  // is used to store the value 
  
  char clipTitle[100];
  sprintf(clipTitle, "clipping planes:%i", win);

  t = XmStringCreateLocalized ((char*)"Close");
  XtSetArg (args[n], XmNcancelLabelString, t); n++;
  XtSetArg (args[n], XmNautoUnmanage, False); n++;
  XtSetArg (args[n], XtNtitle, clipTitle); n++;
  if( overlayVisual )
  {
    // ** more here ***
    XtSetArg (args[n], XmNvisual, overlayVisual ); n++;
  }
  
  dialog = XmCreateTemplateDialog( pb, clipTitle, args, n );
  XmStringFree (t); /* always destroy compound strings when done */

  int * local_win_number_ = (int *) malloc(sizeof(int));
  *local_win_number_ = win;
  
  XtAddCallback (dialog, XmNcancelCallback, destroyClipPlaneDialog, (void *)local_win_number_);

// these did not work:

//  XtAddCallback (dialog, XmNmapCallback, positionDialog, NULL);
//  XtAddCallback (dialog, XmNpopupCallback, positionDialog, NULL);
	
//  XtVaSetValues(dialog,XmNx,200,XmNy,200, NULL );

    // arg list for scales
    XtVarArgsList arglist = XtVaCreateArgsList (NULL,
        XmNshowValue, True,
        XmNminimum, -175,
        XmNmaximum, 175,
        XmNscaleMultiple, 1,
	XmNorientation,XmHORIZONTAL,
	XmNvalue,0,
	XmNdecimalPoints, 2,
        NULL);

    // this rowColumn holds all the clipping planes
    Widget baseRC = XtVaCreateWidget ("rowcol", xmRowColumnWidgetClass, dialog,
				      XmNorientation, XmVERTICAL,
				      NULL);

    // **************************************************
    // *********  Make some cipping planes **************
    // **************************************************
    Widget rowcol, toggle, scale, normalRowCol, normal;
    for( int clip=0; clip<3; clip++ )
    {
      Widget frame0 = XtVaCreateManagedWidget("frame",
					      xmFrameWidgetClass, baseRC,
					      XmNshadowType, XmSHADOW_ETCHED_IN, NULL );

      rowcol = XtVaCreateWidget ("rowcol", xmRowColumnWidgetClass, frame0,
				 XmNorientation, XmVERTICAL,
				 NULL);

      // Create a toggle button 
      sprintf(buff,"CLIP:%i %i", win, clip);
      toggle = XtVaCreateManagedWidget(
        buff,
	xmToggleButtonWidgetClass, rowcol,
        XmNindicatorType, XmN_OF_MANY,
        NULL);
      XmToggleButtonSetState(toggle,clipPlaneIsOn(clip),0);
      XtAddCallback(toggle, XmNvalueChangedCallback, clipPlaneOnOff, NULL  );


      sprintf(buff,"CLIP:%i %i distance", win, clip);
      scale = XtVaCreateManagedWidget(buff,
				      xmScaleWidgetClass, rowcol,
				      XtVaNestedList, arglist,
				      XtVaTypedArg, XmNtitleString, XmRString, "distance", 8,
				      NULL);
      XtAddCallback(scale,XmNvalueChangedCallback, scaleCallback, NULL);
      // set scale value for the distance
      XmScaleSetValue(scale,(int)(clipPlane(clip,3)*100.+.5));

      // add ability to input the normal
      normalRowCol = XtVaCreateWidget("normalRowCol",
				       xmRowColumnWidgetClass, rowcol,
				       XmNorientation, XmHORIZONTAL, NULL);
      XtVaCreateManagedWidget("normal:", xmLabelWidgetClass, normalRowCol, NULL);

      sprintf(buff,"CLIP:%i %i normal", win, clip);
      normal = XtVaCreateManagedWidget(buff,xmTextFieldWidgetClass, 
				       normalRowCol, XmNcolumns, 20, XmNmarginHeight, 2, NULL );
      XtAddCallback(normal, XmNactivateCallback, changeNormal, NULL );
      sprintf(buff,"%3.2f, %3.2f, %3.2f",clipPlane(clip,0),clipPlane(clip,1),clipPlane(clip,2));
      XmTextSetString(normal,buff);

      XtManageChild(normalRowCol);
      XtManageChild (rowcol);
      
    }

    XtManageChild (baseRC);

    XtManageChild (dialog);
    XtPopup (XtParent (dialog), XtGrabNone);

// remember that the clip plane dialog is opened
    OGLWindowList[win]->clipPlaneOpen = 1;
    
}

void
annotateOnOff( Widget widget, XtPointer clinet_data, XtPointer call_data )
{
  XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;
  printf("%s: %s\n", XtName(widget), state->set ? "on" : "off" );

  sprintf(buff,"%s %s",XtName(widget),state->set ? "on" : "off" );
  printf("menu: %s \n",buff);
  setMenuChosen(-99,buff);
}


void
annotateText( Widget widget, XtPointer clinet_data, XtPointer call_data )
{
  char *message;
  message = XmTextGetString(widget);

  sprintf(buff,"%s %s",XtName(widget),message); // annotate # text ...
//  printf("menu: %s \n",buff);
  setMenuChosen(-99,buff);
}

static void 
destroyAnnotateDialog(Widget w, XtPointer client_data, XtPointer call_data)
{
// retrieve the window number
  int *win_ = (int *) client_data;

// remember that the view char dialog is closed
  OGLWindowList[*win_]->annotateOpen = 0;
  
// free the memory allocated for win_
  free(win_);

  XtDestroyWidget(w);
}

//
// AP: Annotations are not fully implemented yet
//
void
annotateDialog(Widget pb, XtPointer client_data, XtPointer call_data)
//
//  Create a dialog that can be used to annotate the plot by placing 
//  text strings at specified positions
{
//  WINDOW_REC *w=OGLWindowList[OGLCurrentWindow];
  
  Widget dialog;
  XmString t;
  Arg args[7];
  int n = 0, win;
  XtPointer winInfo=NULL;
  int *winInfo_;

  XtVaGetValues(pb, XmNuserData, &winInfo, NULL);
  winInfo_ = (int *) winInfo;
  if (winInfo_)
  {
    win = *winInfo_;
//    printf("annotateDialog: win = %i\n", win);
  }
  else
  {
    win = OGLCurrentWindow;
    printf("annotateDialog: invalid window: using OGLCurrentWindow\n");
  }

// check if this dialog already is opened
  if (OGLWindowList[win]->annotateOpen)
    return;

  // Create the dialog -- the PushButton acts as the DialogShell's
  // parent (not the parent of the PromptDialog).  The "userData"
  // is used to store the value 

  t = XmStringCreateLocalized ((char*)"Close");
  XtSetArg (args[n], XmNcancelLabelString, t); n++;
  XtSetArg (args[n], XmNautoUnmanage, False); n++;
  XtSetArg (args[n], XtNtitle, "annotate"); n++;
  if( overlayVisual )
  {
    // ** more here ***
//    XtSetArg (args[n], XmNvisual, w->vi->visual ); n++;
    XtSetArg (args[n], XmNvisual, overlayVisual ); n++;
  }
  
// #ifdef noGLwidget
//   XtSetArg (args[n], XmNvisual, vi->visual ); n++;
// #endif
//XtSetArg (args[n], XmNy, 200); n++;

  dialog = XmCreateTemplateDialog (pb, (char*)"anotate",args,n );

  XmStringFree (t); /* always destroy compound strings when done */

  int * local_win_number_ = (int *) malloc(sizeof(int));
  *local_win_number_ = win;

  XtAddCallback (dialog, XmNcancelCallback, destroyAnnotateDialog, (void *)local_win_number_);


  // arg list for scales
  XtVarArgsList arglist = XtVaCreateArgsList (NULL,
					      XmNshowValue, True,
					      XmNminimum, -175,
					      XmNmaximum, 175,
					      XmNscaleMultiple, 1,
					      XmNorientation,XmHORIZONTAL,
					      XmNvalue,0,
					      XmNdecimalPoints, 2,
					      NULL);

  // this rowColumn holds all the anotations
  Widget baseRC = XtVaCreateWidget ("rowcol", xmRowColumnWidgetClass, dialog,
				    XmNorientation, XmVERTICAL,
				    NULL);

    // **************************************************
    // *********  Make some annotations    **************
    // **************************************************
    Widget rowcol, toggle, normalRowCol, normal;
    for( int text=0; text<3; text++ )
    {
      Widget frame0 = XtVaCreateManagedWidget("frame",
					      xmFrameWidgetClass, baseRC,
					      XmNshadowType, XmSHADOW_ETCHED_IN, NULL );

      rowcol = XtVaCreateWidget ("rowcol", xmRowColumnWidgetClass, frame0,
				 XmNorientation, XmVERTICAL,
				 NULL);

      // Create a toggle button 
      sprintf(buff,"annotate %i",text);
      toggle = XtVaCreateManagedWidget(
        buff,
	xmToggleButtonWidgetClass, rowcol,
        XmNindicatorType, XmN_OF_MANY,
        NULL);
      XmToggleButtonSetState(toggle,FALSE,0);
      XtAddCallback(toggle, XmNvalueChangedCallback, annotateOnOff, NULL  );


      // Here is the text
      normalRowCol = XtVaCreateWidget("normalRowCol",
				       xmRowColumnWidgetClass, rowcol,
				       XmNorientation, XmHORIZONTAL, NULL);
      XtVaCreateManagedWidget("text:", xmLabelWidgetClass, normalRowCol, NULL);

      sprintf(buff,"annotate %i text",text);
      normal = XtVaCreateManagedWidget(buff,xmTextFieldWidgetClass, 
				       normalRowCol, XmNcolumns, 20, XmNmarginHeight, 2, NULL );
      XtAddCallback(normal, XmNactivateCallback, annotateText, NULL );


      XtManageChild(normalRowCol);


      XtManageChild (rowcol);
      
    }

    XtManageChild (baseRC);

    XtManageChild (dialog);
    XtPopup (XtParent (dialog), XtGrabNone);

    OGLWindowList[win]->annotateOpen=1;
}

void 
moglPrintRotPnt(real x, real y, real z, int win_number)
{
  if (!moglInitialized) return; 
  char message[200];
  
  if (win_number < 0 || win_number >= OGLNWindows)
  {
    printf("moglPrintRotPnt: ERROR: win_number=%i out of bounds\n", win_number);
    return;
  }

  if (OGLWindowList[win_number]->xValue) // make sure the widget is created
  {
// reset current value
    sprintf(message,"%3.2f  %3.2f  %3.2f", x, y, z);
    XmTextSetString(OGLWindowList[win_number]->xValue, message);
  }
  
}

void 
moglPrintLineWidth(real lw, int win_number)
{
  if (!moglInitialized) return; 
  char message[200];
  
  if (win_number < 0 || win_number >= OGLNWindows)
  {
    printf("moglPrintLineWidth: ERROR: win_number=%i out of bounds\n", win_number);
    return;
  }

  if (OGLWindowList[win_number]->lineSF) // make sure the widget is created
  {
// reset current value
    sprintf(message,"%3.2f", lw);
    XmTextSetString(OGLWindowList[win_number]->lineSF, message);
  }
  
}

void 
moglPrintFractionOfScreen(real fraction, int win_number)
{
  if (!moglInitialized) return; 
  char message[200];
  
  if (win_number < 0 || win_number >= OGLNWindows)
  {
    printf("moglPrintFractionOfScreen: ERROR: win_number=%i out of bounds\n", win_number);
    return;
  }

  if (OGLWindowList[win_number]->fractionOfScreen) // make sure the widget is created
  {
// reset current value
    sprintf(message,"%3.2f", fraction);
    XmTextSetString(OGLWindowList[win_number]->fractionOfScreen, message);
  }
  
}

static void 
destroyViewCharDialog( Widget w, XtPointer client_data, XtPointer call_data)
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

// remember that the view char dialog is closed
  OGLWindowList[viewChar_->win_number]->viewCharOpen = 0;
  
  XtDestroyWidget(w);

  OGLWindowList[viewChar_->win_number]->xValue = NULL; // reset the pointer to the rotation point text widget
  OGLWindowList[viewChar_->win_number]->lineSF = NULL; // reset the pointer to the line scale factor widget
  OGLWindowList[viewChar_->win_number]->fractionOfScreen = NULL; 
}


static void
setRotPnt( Widget widget, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;
  real x, y, z;
  char message[200];

  if (sScanF(XmTextGetString(widget), "%e %e %e", &x, &y, &z) == 3)
  {
    sprintf(message, "ROTATIONPOINT:%i %g %g %g", viewChar_->win_number, x, y, z); 

    menuItemChosen = -1; // ****
    setMenuNameChosen(message);
    exitEventLoop=TRUE;
  }
  else
  {
    moglSetPrompt("setRotPnt: ERROR: Expecting 3 numbers!");
// reset current value
    sprintf(message,"%3.2f  %3.2f  %3.2f", 
	    (real) viewChar_->rotationCenter[0], 
	    (real) viewChar_->rotationCenter[1], 
	    (real) viewChar_->rotationCenter[2]);
    XmTextSetString(widget, message);
  }
  
}

static void
setLineWidth( Widget widget, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;
  real ls;
  char message[200];

  if (sScanF(XmTextGetString(widget), "%e", &ls) == 1)
  {
    sprintf(message, "line width scale factor:%i %g", viewChar_->win_number, ls); 

    menuItemChosen = -1; // ****
    setMenuNameChosen(message);
    exitEventLoop=TRUE;
  }
  else
  {
    moglSetPrompt("setLineWidth: ERROR: Expecting a number!");
// reset current value
    sprintf( message, "%3.2f", (real) *(viewChar_->lineScaleFactor_) );
    XmTextSetString(widget, message);
  }
}

static void
setFractionOfScreen( Widget widget, XtPointer client_data, XtPointer call_data )
{
  // retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;
  real fraction;
  char message[200];

  if (sScanF(XmTextGetString(widget), "%e", &fraction) == 1)
  {
    sprintf(message, "fraction of screen:%i %g", viewChar_->win_number, fraction); 

    menuItemChosen = -1; // ****
    setMenuNameChosen(message);
    exitEventLoop=TRUE;
  }
  else
  {
    moglSetPrompt("setFractionOfScreen: ERROR: Expecting a number!");
    // reset current value
    sprintf( message, "%3.2f", (real) *(viewChar_->fractionOfScreen_) );
    XmTextSetString(widget, message);
  }
}

static void
setPosition( Widget widget, XtPointer client_data, XtPointer call_data )
// This widget is called when a value is typed in to the position field
{
  char message[200];
  int *light_number_, lightN;
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;
  
  XtVaGetValues(widget, XmNuserData, (XtPointer) &light_number_, NULL);
  if (light_number_)
  {
    lightN = *light_number_;
  }
  else
  {
    lightN = 0;
    printf("setPosition: invalid light number: using 0\n");
  }

  real x, y, z;
  if (sScanF(XmTextGetString(widget), "%e %e %e", &x, &y, &z) == 3)
  {
    sprintf(message, "POSITION#%i:%i %g %g %g", lightN, viewChar_->win_number, x, y, z);

    menuItemChosen = -1; // ****
    setMenuNameChosen(message);
    exitEventLoop=TRUE;
  }
  else
  {
// restore the old values
    moglSetPrompt("ERROR: Expecting 3 numbers");
    sprintf(message,"%3.2f  %3.2f  %3.2f", viewChar_->position[lightN][0], viewChar_->position[lightN][1], 
	    viewChar_->position[lightN][2]);
    XmTextSetString(widget, message);
  }
}

static void
setAmbient( Widget widget, XtPointer client_data, XtPointer call_data )
// This widget is called when a value is typed in to the ambient field
{
  char message[200];
  int *light_number_, lightN;
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

//  XtPointer lightInfo;
  
  XtVaGetValues(widget, XmNuserData, (XtPointer) &light_number_, NULL);
  if (light_number_)
  {
    lightN = *light_number_;
  }
  else
  {
    lightN = 0;
    printf("setAmbient: invalid light number: using 0\n");
  }
  
  real r, g, b, a=1;
  
  if (sScanF(XmTextGetString(widget), "%e %e %e %e", &r, &g, &b, &a) == 4)
  {
    sprintf(message, "%s:%i %g %g %g %g", XtName(widget), viewChar_->win_number, r, g, b, a);
// the name is of the form AMBIENT#0, where 0 is the light number
    menuItemChosen = -1; // ****
    setMenuNameChosen(message);
    exitEventLoop=TRUE;
  }
  else
  {
// restore the old values
    moglSetPrompt("ERROR: Expecting 4 numbers");
    sprintf(message,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->ambient[lightN][0], viewChar_->ambient[lightN][1], 
	    viewChar_->ambient[lightN][2], viewChar_->ambient[lightN][3]);
    XmTextSetString(widget, message);
  }
}

static void
setDiffuse( Widget widget, XtPointer client_data, XtPointer call_data )
// This widget is called when a value is typed in to the diffuse field
{
  char message[200];
  int *light_number_, lightN;
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

//  XtPointer lightInfo;
  
  XtVaGetValues(widget, XmNuserData, (XtPointer) &light_number_, NULL);
  if (light_number_)
  {
    lightN = *light_number_;
  }
  else
  {
    lightN = 0;
    printf("setDiffuse: invalid light number: using 0\n");
  }
  
  real r, g, b, a=1;
  
  if (sScanF(XmTextGetString(widget), "%e %e %e %e", &r, &g, &b, &a) == 4)
  {
    sprintf(message, "%s:%i %g %g %g %g", XtName(widget), viewChar_->win_number, r, g, b, a);
// the name is of the form DIFFUSE#0, where 0 is the light number
    menuItemChosen = -1; // ****
    setMenuNameChosen(message);
    exitEventLoop=TRUE;
  }
  else
  {
// restore the old values
    moglSetPrompt("ERROR: Expecting 4 numbers");
    sprintf(message,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->diffuse[lightN][0], viewChar_->diffuse[lightN][1], 
	    viewChar_->diffuse[lightN][2], viewChar_->diffuse[lightN][3]);
    XmTextSetString(widget, message);
  }
}

static void
setSpecular( Widget widget, XtPointer client_data, XtPointer call_data )
// This widget is called when a value is typed in to the specular field
{
  char message[200];
  int *light_number_, lightN;
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

//  XtPointer lightInfo;
  
  XtVaGetValues(widget, XmNuserData, (XtPointer) &light_number_, NULL);
  if (light_number_)
  {
    lightN = *light_number_;
  }
  else
  {
    lightN = 0;
    printf("setSpecular: invalid light number: using 0\n");
  }
  
  real r, g, b, a=1;
  
  if (sScanF(XmTextGetString(widget), "%e %e %e %e", &r, &g, &b, &a) == 4)
  {
    sprintf(message, "%s:%i %g %g %g %g", XtName(widget), viewChar_->win_number, r, g, b, a);
// the name is of the form SPECULAR#0, where 0 is the light number
    menuItemChosen = -1; // ****
    setMenuNameChosen(message);
    exitEventLoop=TRUE;
  }
  else
  {
// restore the old values
    moglSetPrompt("ERROR: Expecting 4 numbers");
    sprintf(message,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->specular[lightN][0], viewChar_->specular[lightN][1], 
	    viewChar_->specular[lightN][2], viewChar_->specular[lightN][3]);
    XmTextSetString(widget, message);
  }
}


void
backgroundCallback(Widget menuItem, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

  char answer[120];
  sprintf(answer,"Background colour:%i %s", viewChar_->win_number, XtName(menuItem));

  menuItemChosen = -1; // ****
  setMenuNameChosen(answer);
  exitEventLoop=TRUE;
}

void
foregroundCallback(Widget menuItem, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

  char answer[120];
  sprintf(answer,"Foreground colour:%i %s", viewChar_->win_number, XtName(menuItem));

  menuItemChosen = -1; // ****
  setMenuNameChosen(answer);
  exitEventLoop=TRUE;
}

static void
setShininess( Widget widget, XtPointer client_data, XtPointer call_data )
// This routine is called when the value of the shininess slider is changed
{
  XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *) call_data;
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;
  
  char message[200];
  sprintf(message, "SHININESS:%i %i", viewChar_->win_number, cbs->value);

  menuItemChosen = -1; // ****
  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}

static void
setScalefactor( Widget widget, XtPointer client_data, XtPointer call_data )
// This widget is called when the value of the scalefactor slider is changed
{
  XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *) call_data;
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;
  
  char message[200];
// the value is an integer and needs to be scaled to account for the precision.
  sprintf(message, "SCALEFACTOR:%i %.2f", viewChar_->win_number, 0.01*cbs->value); 

  menuItemChosen = -1; // ****
  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}

static void
axesOriginCallback( Widget widget, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

  // XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) call_data;
  
  char message[200];
  sprintf(message, "%s:%i", XtName(widget), viewChar_->win_number);

  menuItemChosen = -1; // ****
  setMenuNameChosen(message);

  exitEventLoop=TRUE;
}

static void
onOffCallback( Widget widget, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

  XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) call_data;
  
// read the light number
  int lightN;
  char message[200];
  sprintf(message, "%s", XtName(widget));
// start reading from character 8 in the name (Light #0)
  if (sScanF(&message[7], "%i", &lightN) != 1)
  {
    printf("onOffCallback: ERROR: Can't read the light number from the widget name `%s'\n"
	   "Using light number 0\n", message);
    lightN = 0;
  }
  
  sprintf(message, "LIGHT#%i:%i %s", lightN, viewChar_->win_number, (cbs->set ==1)? "ON": "OFF");

  menuItemChosen = -1; // ****
  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}


static void
lightingCallback( Widget widget, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

  XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) call_data;
  
  char message[200];
  sprintf(message, "LIGHTING:%i %s", viewChar_->win_number, (cbs->set ==1)? "ON": "OFF");

  menuItemChosen = -1; // ****
  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}

static void
pickCallback( Widget widget, XtPointer client_data, XtPointer call_data )
{
// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

  char message[200];
  sprintf(message, "Pick rotation point:%i", viewChar_->win_number);

  menuItemChosen = -1; // means that the reply will be parsed by processSpecialMenuItems in GL_GI
  setMenuNameChosen(message);
  exitEventLoop=TRUE;
}

void
viewCharacteristicsDialog(Widget pb, XtPointer client_data, XtPointer call_data)
//
//  /Purpose:
//  Create a dialog that can be used to set the view characteristics
//  /Author: AP
{
  // Create the dialog -- the PushButton `pb' acts as the DialogShell's
  // parent.

  Widget dialog;
  XmString t;
  Arg args[7];
  int n = 0;

// retrieve the pointer to the viewCharacteristics info
  ViewCharacteristics * viewChar_ = (ViewCharacteristics *) client_data;

// check if the dialog already is opened
  if (OGLWindowList[viewChar_->win_number]->viewCharOpen)
    return;

  char buff[200];
// setup the colour names
  const char *colour[]={"black",  "white",  "red",  "blue",  "green",  "orange",  "yellow",  "darkgreen",
		  "seagreen", "skyblue",  "navyblue",  "violet",  "pink",  "turquoise",  "gold", 
		  "coral",  "violetred",  "darkturquoise",  "steelblue",  "orchid",  "salmon",
		  "aquamarine",  "mediumgoldenrod",  "wheat",  "khaki",  "maroon",  "slateblue",
		  "darkorchid", "plum",  NULL};   // null string terminates the menu


  t = XmStringCreateLocalized ((char*)"Close");
  XtSetArg (args[n], XmNcancelLabelString, t); n++;
  XtSetArg (args[n], XmNautoUnmanage, False); n++;
  sprintf(buff, "Set View Characteristics:%i", viewChar_->win_number);
  XtSetArg (args[n], XtNtitle, buff); n++; 
  if( overlayVisual )
  {
    // ** more here ***
    XtSetArg (args[n], XmNvisual, overlayVisual ); n++;
  }
  
  dialog = XmCreateTemplateDialog (pb, (char*)"viewCharacteristics",args,n );

  XmStringFree (t); /* always destroy compound strings when done */

  XtAddCallback (dialog, XmNcancelCallback, destroyViewCharDialog, (XtPointer) viewChar_);

  // this rowColumn holds all the buttons, menus, etc.
  Widget baseRC = XtVaCreateWidget ("rowcol", xmRowColumnWidgetClass, dialog,
				    XmNorientation, XmVERTICAL,
				    XmNpacking, XmPACK_TIGHT, 
				    NULL);

// put all the buttons and other widget here

  Widget smallButtonFrame = XtVaCreateWidget ("smallButtonFrame", 
					      xmRowColumnWidgetClass, baseRC,
					      XmNpacking, XmPACK_TIGHT,
					      XmNorientation, XmVERTICAL,
					      NULL);

// Frame for all colours
  Widget colorFrame = XtVaCreateWidget ("colorFrame", 
					xmRowColumnWidgetClass, smallButtonFrame,
					XmNpacking, XmPACK_TIGHT,
					XmNorientation, XmHORIZONTAL,
					NULL);
// option menu for the background color
  Widget menupane = XmCreatePulldownMenu(colorFrame, (char*)"menuPane", NULL, 0);
  XmString bgText = XmStringCreateLocalized((char*)"Background colour:");
  n=0;
  XtSetArg(args[n], XmNsubMenuId, menupane); n++;
  XtSetArg(args[n], XmNlabelString, bgText); n++;
  Widget bgmenu = XmCreateOptionMenu(colorFrame, (char*)"optionMenu", args, n);
  
  XmStringFree(bgText);

// fill in all the colors
  Widget btn;
  Widget btn1=NULL;
  int i;
  
  for (i=0; colour[i] != NULL; i++)
  {
    btn = XtVaCreateManagedWidget(colour[i],xmPushButtonWidgetClass, menupane, NULL);
    XtAddCallback(btn, XmNactivateCallback, backgroundCallback, (XtPointer) viewChar_ );
// get a handle to the button with the backgroundName[w] instead.
    if (!strcmp(colour[i], viewChar_->backGroundName)) btn1=btn; 
  }
  if (btn1) XtVaSetValues(bgmenu, XmNmenuHistory, btn1, NULL);

  XtManageChild(bgmenu);
// done with the options menu for the background colour

// make an option menu for the text color
  menupane = XmCreatePulldownMenu(colorFrame, (char*)"menuPane", NULL, 0);
  bgText = XmStringCreateLocalized((char*)"Text colour:");
  n=0;
  XtSetArg(args[n], XmNsubMenuId, menupane); n++;
  XtSetArg(args[n], XmNlabelString, bgText); n++;
  Widget fgmenu = XmCreateOptionMenu(colorFrame, (char*)"optionMenu", args, n);
  
  XmStringFree(bgText);

// fill in all the colors
  for (i=0; colour[i] != NULL; i++)
  {
    btn = XtVaCreateManagedWidget((char*)colour[i],xmPushButtonWidgetClass, menupane, NULL);
    XtAddCallback(btn, XmNactivateCallback, foregroundCallback, (XtPointer) viewChar_);
// get a handle to the button with the backgroundName[w] instead.
    if (!strcmp(colour[i], viewChar_->foreGroundName)) btn1=btn; 
  }
  if (btn1) XtVaSetValues(fgmenu, XmNmenuHistory, btn1, NULL);

  XtManageChild(fgmenu);
// done with the options menu for the text colour

// done with the color frame
  XtManageChild(colorFrame);

// axes location radiobox
  Widget radioBox = XmCreateRadioBox(smallButtonFrame, (char*)"radioBox", NULL, 0);

  Widget defaultAX = XtVaCreateManagedWidget("Axes origin at default point", 
					     xmToggleButtonGadgetClass, radioBox, NULL);
  XtAddCallback(defaultAX, XmNvalueChangedCallback, axesOriginCallback, (XtPointer) viewChar_);

  Widget rotAX = XtVaCreateManagedWidget("Axes origin at rotation point", 
					 xmToggleButtonGadgetClass, radioBox, NULL);
  XtAddCallback(rotAX, XmNvalueChangedCallback, axesOriginCallback, (XtPointer) viewChar_);

// set current value
  if (*viewChar_->axesOriginOption_ == 0)
    XmToggleButtonSetState(defaultAX, True, False); // turn the default case on
  else
    XmToggleButtonSetState(rotAX, True, False); // turn the rotation point case on
  
  XtManageChild(radioBox);

// make a horizontal row-col frame to put the rotation label and editable text in
  Widget lineScaleForm = XtVaCreateWidget ("lineScaleForm", xmRowColumnWidgetClass, smallButtonFrame,
				      XmNorientation, XmHORIZONTAL,
				      NULL);

  // -- line width scale factor ----
  Widget lineSFText = XtVaCreateManagedWidget("Line width scale factor:",xmLabelWidgetClass, lineScaleForm, 
					       NULL);
  // save the pointer to the editable text so that we can access it later on...
  Widget lineSF =  OGLWindowList[viewChar_->win_number]->lineSF = 
    XtVaCreateManagedWidget("editableX", xmTextFieldWidgetClass, lineScaleForm, XmNrows, 1, NULL); 

  XtAddCallback(lineSF, XmNactivateCallback, setLineWidth, (XtPointer) viewChar_ );
  // set current value
  sprintf(buff,"%3.2f", *(viewChar_->lineScaleFactor_));
  XmTextSetString(lineSF, buff);

  // -- fraction of screen ----
  Widget fractionOfScreenText = XtVaCreateManagedWidget("Fraction of screen:",xmLabelWidgetClass, lineScaleForm, 
					       NULL);
  // save the pointer to the editable text so that we can access it later on...
  Widget fractionOfScreen =  OGLWindowList[viewChar_->win_number]->fractionOfScreen = 
    XtVaCreateManagedWidget("fractionOfScreen", xmTextFieldWidgetClass, lineScaleForm, XmNrows, 1, NULL); 

  XtAddCallback(fractionOfScreen, XmNactivateCallback, setFractionOfScreen, (XtPointer) viewChar_ );
  // set current value
  sprintf(buff,"%3.2f", *(viewChar_->fractionOfScreen_));
  XmTextSetString(fractionOfScreen, buff);




  XtManageChild (lineScaleForm);

// rotation point input
  Widget rotOptionFrame = XtVaCreateWidget ("rotOptionFrame", xmRowColumnWidgetClass, smallButtonFrame,
					    XmNorientation, XmHORIZONTAL,
					    XmNpacking, XmPACK_TIGHT,
					    NULL);
// make a horizontal row-col frame to put the rotation label and editable text in
  Widget rotFrame = XtVaCreateWidget ("rotFrame", xmRowColumnWidgetClass, rotOptionFrame,
				      XmNorientation, XmHORIZONTAL,
				      NULL);
  Widget commandText = XtVaCreateManagedWidget("Rotation point (x y z):",xmLabelWidgetClass, rotFrame, 
					       NULL);
// save the pointer to the editable text so that we can access it later on...
  Widget xValue = OGLWindowList[viewChar_->win_number]->xValue = 
    XtVaCreateManagedWidget("editableX", xmTextFieldWidgetClass, rotFrame, XmNrows, 1, NULL); 

  XtAddCallback(xValue, XmNactivateCallback, setRotPnt, (XtPointer) viewChar_ );
// set current value
  sprintf(buff,"%3.2f  %3.2f  %3.2f", viewChar_->rotationCenter[0], viewChar_->rotationCenter[1], 
	  viewChar_->rotationCenter[2]);
  XmTextSetString(xValue, buff);


  XtManageChild (rotFrame);

// pick rotation point button
  Widget rotpt = XtVaCreateManagedWidget("Pick rotation point", xmPushButtonWidgetClass, rotOptionFrame, 
					 NULL);
  XtAddCallback(rotpt, XmNactivateCallback, pickCallback, (XtPointer) viewChar_);

// done defining the rotation point frame
  XtManageChild(rotOptionFrame);

// done defining the samllButtonFrame
  XtManageChild (smallButtonFrame);


// make a frame around the lighting stuff
  Dimension borderW=2;
  Widget lightBorder = XtVaCreateWidget("lightBorder", xmFrameWidgetClass, baseRC,
					XmNmarginWidth, borderW,
					XmNmarginHeight, borderW,
					NULL);
// make a frame around all the light input boxes
  Widget lightCommentFrame = XtVaCreateWidget("commentFrame", xmRowColumnWidgetClass, lightBorder,
					   XmNpacking, XmPACK_TIGHT,
					   XmNorientation, XmVERTICAL,
					   NULL);
// lighting properties
  Widget lightFrame = XtVaCreateWidget ("lightFrame", 
					xmRowColumnWidgetClass, lightCommentFrame,
					XmNpacking, XmPACK_COLUMN,
					XmNnumColumns, 4,
					XmNorientation, XmVERTICAL,
					NULL);
  
// Toggle lighting
  Widget lightOnOff = XtVaCreateManagedWidget("Lighting", xmToggleButtonWidgetClass, 
					      lightFrame,
					      XmNindicatorType, XmN_OF_MANY,
					      NULL);
// add callback function here
  XtAddCallback(lightOnOff, XmNvalueChangedCallback, lightingCallback, (XtPointer) viewChar_);
// set the state of the button
  if (*(viewChar_->lighting_))
    XmToggleButtonSetState(lightOnOff, True, False); // check the current state

  Widget positionText = XtVaCreateManagedWidget("Position (X Y Z):",xmLabelWidgetClass, lightFrame, 
						NULL);
  Widget amientText = XtVaCreateManagedWidget("Ambient (R G B A):",xmLabelWidgetClass, lightFrame, 
					      NULL);
  Widget diffuseText = XtVaCreateManagedWidget("Diffusive (R G B A):",xmLabelWidgetClass, lightFrame, 
					       NULL);
  Widget specularText = XtVaCreateManagedWidget("Specular (R G B A):",xmLabelWidgetClass, lightFrame, 
						NULL);
  
// light 0
  int * lightInfo0 = (int *) malloc( sizeof(int) );
  *lightInfo0 = 0; // encode the light number into the user data

// on/off 0
  Widget light0_on = XtVaCreateManagedWidget("Light #0", xmToggleButtonWidgetClass, lightFrame,
					     XmNindicatorType, XmONE_OF_MANY,
					     NULL);
  XtAddCallback(light0_on, XmNvalueChangedCallback, onOffCallback, (XtPointer) viewChar_);
// set the indicator
  if (viewChar_->lightIsOn[0]) 
    XmToggleButtonSetState(light0_on, True, False);

// position 0
  Widget position0 = XtVaCreateManagedWidget("POSITION#0", xmTextFieldWidgetClass, lightFrame, 
					     XmNrows, 1, 
					     XmNuserData, (XtPointer) lightInfo0,
					     NULL); 
  XtAddCallback(position0, XmNactivateCallback, setPosition, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f", viewChar_->position[0][0], viewChar_->position[0][1], 
	  viewChar_->position[0][2]);
  XmTextSetString(position0, buff);
// ambient 0
  Widget ambient0 = XtVaCreateManagedWidget("AMBIENT#0", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo0,
					    NULL); 
  XtAddCallback(ambient0, XmNactivateCallback, setAmbient, (XtPointer) viewChar_  );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->ambient[0][0], viewChar_->ambient[0][1], 
	  viewChar_->ambient[0][2], viewChar_->ambient[0][3]);
  XmTextSetString(ambient0, buff);
// diffuse 0
  Widget diffuse0 = XtVaCreateManagedWidget("DIFFUSE#0", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo0,
					    NULL); 
  XtAddCallback(diffuse0, XmNactivateCallback, setDiffuse, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->diffuse[0][0], viewChar_->diffuse[0][1], 
	  viewChar_->diffuse[0][2], viewChar_->diffuse[0][3]);
  XmTextSetString(diffuse0, buff);
// specular 0
  Widget specular0 = XtVaCreateManagedWidget("SPECULAR#0", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo0,
					    NULL); 
  XtAddCallback(specular0, XmNactivateCallback, setAmbient, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->specular[0][0], viewChar_->specular[0][1], 
	  viewChar_->specular[0][2], viewChar_->specular[0][3]);
  XmTextSetString(specular0, buff);

// light 1
  int * lightInfo1 = (int *) malloc( sizeof(int) );
  *lightInfo1 = 1; // encode the light number into the user data

// on/off 1
  Widget light1_on = XtVaCreateManagedWidget("Light #1", xmToggleButtonWidgetClass, lightFrame,
					     XmNindicatorType, XmONE_OF_MANY,
					     NULL);
  XtAddCallback(light1_on, XmNvalueChangedCallback, onOffCallback, (XtPointer) viewChar_);
// set the indicator
  if (viewChar_->lightIsOn[1]) 
    XmToggleButtonSetState(light1_on, True, False);

// position 1
  Widget position1 = XtVaCreateManagedWidget("POSITION#1", xmTextFieldWidgetClass, lightFrame, 
					     XmNrows, 1, 
					     XmNuserData, (XtPointer) lightInfo1,
					     NULL); 
  XtAddCallback(position1, XmNactivateCallback, setPosition, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f", viewChar_->position[1][0], viewChar_->position[1][1], 
	  viewChar_->position[1][2]);
  XmTextSetString(position1, buff);
// ambient 1
  Widget ambient1 = XtVaCreateManagedWidget("AMBIENT#1", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo1,
					    NULL); 
  XtAddCallback(ambient1, XmNactivateCallback, setAmbient, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->ambient[1][0], viewChar_->ambient[1][1], 
	  viewChar_->ambient[1][2], viewChar_->ambient[1][3]);
  XmTextSetString(ambient1, buff);
// diffuse 1
  Widget diffuse1 = XtVaCreateManagedWidget("DIFFUSE#1", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo1,
					    NULL); 
  XtAddCallback(diffuse1, XmNactivateCallback, setDiffuse, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->diffuse[1][0], viewChar_->diffuse[1][1], 
	  viewChar_->diffuse[1][2], viewChar_->diffuse[1][3]);
  XmTextSetString(diffuse1, buff);
// specular 1
  Widget specular1 = XtVaCreateManagedWidget("SPECULAR#1", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo1,
					    NULL); 
  XtAddCallback(specular1, XmNactivateCallback, setSpecular, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->specular[1][0], viewChar_->specular[1][1], 
	  viewChar_->specular[1][2], viewChar_->specular[1][3]);
  XmTextSetString(specular1, buff);

// light 2
  int * lightInfo2 = (int *) malloc( sizeof(int) );
  *lightInfo2 = 2; // encode the light number into the user data

// on/off 2
  Widget light2_on = XtVaCreateManagedWidget("Light #2", xmToggleButtonWidgetClass, lightFrame,
					     XmNindicatorType, XmONE_OF_MANY,
					     NULL);
  XtAddCallback(light2_on, XmNvalueChangedCallback, onOffCallback, (XtPointer) viewChar_);
// set the indicator
  if (viewChar_->lightIsOn[2]) 
    XmToggleButtonSetState(light2_on, True, False);

// position 2
  Widget position2 = XtVaCreateManagedWidget("POSITION#2", xmTextFieldWidgetClass, lightFrame, 
					     XmNrows, 1, 
					     XmNuserData, (XtPointer) lightInfo2,
					     NULL); 
  XtAddCallback(position2, XmNactivateCallback, setPosition, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f", viewChar_->position[2][0], viewChar_->position[2][1], 
	  viewChar_->position[2][2]);
  XmTextSetString(position2, buff);
// ambient 2
  Widget ambient2 = XtVaCreateManagedWidget("AMBIENT#2", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo2,
					    NULL); 
  XtAddCallback(ambient2, XmNactivateCallback, setAmbient, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->ambient[2][0], viewChar_->ambient[2][1], 
	  viewChar_->ambient[2][2], viewChar_->ambient[2][3]);
  XmTextSetString(ambient2, buff);
// diffuse 2
  Widget diffuse2 = XtVaCreateManagedWidget("DIFFUSE#2", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo2,
					    NULL); 
  XtAddCallback(diffuse2, XmNactivateCallback, setDiffuse, (XtPointer) viewChar_  );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->diffuse[2][0], viewChar_->diffuse[2][1], 
	  viewChar_->diffuse[2][2], viewChar_->diffuse[2][3]);
  XmTextSetString(diffuse2, buff);
// specular 2
  Widget specular2 = XtVaCreateManagedWidget("SPECULAR#2", xmTextFieldWidgetClass, lightFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) lightInfo2,
					    NULL); 
  XtAddCallback(specular2, XmNactivateCallback, setSpecular, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->specular[2][0], viewChar_->specular[2][1], 
	  viewChar_->specular[2][2], viewChar_->specular[2][3]);
  XmTextSetString(specular2, buff);

  XtManageChild (lightFrame);
  Widget lightComment = XtVaCreateManagedWidget("NOTE: Changing the lighting only alters "
						"lit objects", xmLabelWidgetClass, 
						lightCommentFrame, 
						NULL);
// change the foreground to red
  XtVaSetValues(lightComment, XtVaTypedArg, XmNforeground, XmRString, "red", 4, NULL);

  XtManageChild(lightCommentFrame);
  XtManageChild (lightBorder);
// end lights

// material stuff
// make a border around the material frame
  Widget materialBorder = XtVaCreateWidget("materialBorder", xmFrameWidgetClass, baseRC,
					   XmNmarginWidth, borderW,
					   XmNmarginHeight, borderW,
					   NULL);

// make a frame around the material input text boxes and the sliders
  Widget commentFrame = XtVaCreateWidget("commentFrame", xmRowColumnWidgetClass, materialBorder,
					   XmNpacking, XmPACK_TIGHT,
					   XmNorientation, XmVERTICAL,
					   NULL);
// make a frame around the material input text boxes and the sliders
  Widget outerMatFrame = XtVaCreateWidget("outerMatFrame", xmRowColumnWidgetClass, commentFrame,
					  XmNpacking, XmPACK_TIGHT, /*COLUMN*/
					   XmNorientation, XmVERTICAL,
//					   XmNorientation, XmHORIZONTAL,
					   NULL);
// material properties
  Widget materialFrame = XtVaCreateWidget ("materialFrame", 
					   xmRowColumnWidgetClass, outerMatFrame,
					   XmNpacking, XmPACK_COLUMN,
					   XmNnumColumns, 2,
					   XmNorientation, XmVERTICAL,
					   NULL);
  
// the toggle buttons need no explanation
  Widget material = XtVaCreateManagedWidget("X-colour material properties",xmLabelWidgetClass, materialFrame,
					    NULL);
  Widget specularTextm = XtVaCreateManagedWidget("Specular (R G B A):",xmLabelWidgetClass, materialFrame, 
						 NULL);
  
// material properties
  int * materialInfo = (int *) malloc( sizeof(int) );
  *materialInfo = -1; // -1 means material properties

  Widget nothing = XtVaCreateManagedWidget("",xmLabelWidgetClass, materialFrame, NULL);

// specular
  Widget specularm = XtVaCreateManagedWidget("MATERIALSPECULAR", xmTextFieldWidgetClass, materialFrame, 
					    XmNrows, 1, 
					    XmNuserData, (XtPointer) materialInfo,
					    NULL); 
  XtAddCallback(specularm, XmNactivateCallback, setSpecular, (XtPointer) viewChar_ );
// fill in the value
  sprintf(buff,"%3.2f  %3.2f  %3.2f %3.2f", viewChar_->materialSpecular[0], viewChar_->materialSpecular[1], 
	  viewChar_->materialSpecular[2], viewChar_->materialSpecular[3]);
  XmTextSetString(specularm, buff);

// shininess
  Widget shininess = XtVaCreateManagedWidget("Shininess exponent", xmScaleWidgetClass, outerMatFrame,
//                                            12345678901234567890
					     XtVaTypedArg, XmNtitleString, 
// AP: doesn't make a difference	     XmNwidth, 100,
					     XmRString, "Shininess exponent", 19,
					     XmNorientation, XmHORIZONTAL,
					     XmNprocessingDirection, XmMAX_ON_RIGHT,
					     XmNmaximum, 128,
					     XmNminimum, 0,
					     XmNvalue, (int) *(viewChar_->materialShininess_),
					     XmNshowValue, True,
					     XmNscaleHeight, 10,
					     NULL);
  XtAddCallback(shininess, XmNvalueChangedCallback, setShininess, (XtPointer) viewChar_ );

  XtManageChild(materialFrame);

  XtManageChild(outerMatFrame);

  Widget materialComment = XtVaCreateManagedWidget("NOTE: Changing the material properties only takes effect "
						   "after replotting the object",xmLabelWidgetClass, 
						   commentFrame, 
						   NULL);
// change the foreground to red
  XtVaSetValues(materialComment, XtVaTypedArg, XmNforeground, XmRString, "red", 4, NULL);

  XtManageChild(commentFrame);
  XtManageChild(materialBorder);
// end material

// end buttons...

  XtManageChild (baseRC);

  XtManageChild (dialog);
  XtPopup (XtParent (dialog), XtGrabNone);

// remember that the view char is opened for this win_number
  OGLWindowList[viewChar_->win_number]->viewCharOpen = 1;
}

static void 
destroyUserDialog( Widget w, XtPointer client_data, XtPointer call_data)
{
// Note that the dialog window is closed by calling closeDialog from the application.
  DialogData & dialogSpec = *((DialogData *)client_data);
  
// break out of the eventloop. 
  setMenuNameChosen(SC dialogSpec.getExitCommand().c_str());
  menuItemChosen=(dialogSpec.getBuiltInDialog())? -1:1;
  exitEventLoop=TRUE;
}


// default constructor
PickInfo::
PickInfo()
{
  pickType = 0; // 1 means Button1, 2 means Button2, and 3 means Button3
  pickWindow = -99;
  pickBox[0] = 0; // xmin
  pickBox[1] = -1;// xmax
  pickBox[2] = 0; // ymin
  pickBox[3] = -1;// ymax
};



// Some member functions in the dialogData class (these functions use static variables 
// from this file, which prevents them from beeing in DialogData.C


//\begin{>PushButtonInclude.tex}{\subsection{setSensitive}} 
void PushButton::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a PushButton object.
// /trueOrFalse: The new state of the PushButton widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{PushButtonInclude.tex}
//------------------------
{ 
  sensitive=trueOrFalse;
  if( pb!=NULL ) XtSetSensitive((Widget)pb, trueOrFalse);
}  

//\begin{>ToggleButtonInclude.tex}{\subsection{setSensitive}} 
void ToggleButton::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a ToggleButton object.
// /trueOrFalse: The new state of the ToggleButton widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{ToggleButtonInclude.tex}
//------------------------
{ 
  sensitive=trueOrFalse;
  if( tb!=NULL ) XtSetSensitive((Widget)tb, trueOrFalse);
}  

//\begin{>TextLabelInclude.tex}{\subsection{setSensitive}} 
void TextLabel::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a TextLabel object.
// /trueOrFalse: The new state of the textlabel widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{TextLabelInclude.tex}
//------------------------
{ 
  sensitive=trueOrFalse;
  if( textWidget!=NULL ) XtSetSensitive((Widget)textWidget, trueOrFalse);
  if( labelWidget!=NULL) XtSetSensitive((Widget)labelWidget, trueOrFalse);  
}  

//\begin{>OptionMenuInclude.tex}{\subsection{setSensitive}} 
void OptionMenu::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a OptionMenu object.
// /trueOrFalse: The new state of the OptionMenu widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{OptionMenuInclude.tex}
//------------------------
{ 
  sensitive=trueOrFalse;
  if( menupane!=NULL ) XtSetSensitive((Widget)menupane, trueOrFalse);
// do all the buttons
  int btn;
  for (btn=0; btn<n_options; btn++)
  {
    PushButton & pb = optionList[btn];
    pb.setSensitive(trueOrFalse);
  }

}  

//\begin{>OptionMenuInclude.tex}{\subsection{setSensitive}} 
void OptionMenu::
setSensitive(int btn, bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of one button in a OptionMenu object.
// /btn: The button number in the option menu.
// /trueOrFalse: The new state of the push button widget
// /Return valuse: None
// /Author: AP
//\end{OptionMenuInclude.tex}
//------------------------
{ 
  if (menupane!=NULL && btn>=0 && btn<n_options)
  {
    PushButton & pb = optionList[btn];
    pb.setSensitive(trueOrFalse);
  }
}  

//\begin{>PullDownMenuInclude.tex}{\subsection{setSensitive}} 
void PullDownMenu::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a PullDownMenu object.
// /trueOrFalse: The new state of the PullDownMenu widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{PullDownMenuInclude.tex}
//------------------------
{ 
  sensitive=trueOrFalse;
  if( menupane!=NULL ) XtSetSensitive((Widget)menupane, trueOrFalse);
}  

//\begin{>>DialogDataInclude.tex}{\subsection{setSensitive}} 
void DialogData::
setSensitive(int trueFalse)
//------------------------
// /Description: Set the sensitivity of a DialogData object.
// /trueOrFalse: The new state of the DialogData widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{DialogDataInclude.tex}
//------------------------
{
  if (!moglInitialized) return; 

// while reading a command file, dialogWindow is not always constructed when this function is
// called
  if (!dialogWindow) return;

// need to explicitly set the sensitivity of the pulldown menus, since they might be torn off
  int i;
  for (i=0; i<n_pullDownMenu; i++)
    XtSetSensitive((Widget)pdMenuData[i].menupane, trueFalse); 

  XtSetSensitive((Widget)dialogWindow, trueFalse);

// set the cursor to a watch if not sensitive, and a pointer i sensitive
  setCursor(trueFalse? pointerCursor: watchCursor);
  
}


//\begin{>>DialogDataInclude.tex}{\subsection{setSensitive}} 
void DialogData::
setSensitive(bool trueOrFalse, WidgetTypeEnum widgetType, int number )
// ======================================================================================================
// /Description:
//    Set the sensitivity of a widget in the DialogData
// /trueOrFalse (input): set senstive or not
// /widgetType (input): choose a widget type to assign. One of 
// \begin{verbatim}
//  enum WidgetTypeEnum  
//  {
//    optionMenuWidget,
//    pushButtonWidget,
//    pullDownWidget,
//    toggleButtonWidget,
//    textBoxWidget,
//    radioBoxWidget
// };
// \end{verbatim}
// /number (input) : set sensitivity for this widget. 
//\end{DialogDataInclude.tex}
// ======================================================================================================
{
  if( widgetType==optionMenuWidget )
  {
    if( number>=0 && number<n_optionMenu  && opMenuData!=NULL )
    {
      opMenuData[number].setSensitive(trueOrFalse);
//        opMenuData[number].sensitive=trueOrFalse;
//        if( opMenuData[number].menupane!=NULL )
//  	XtSetSensitive((Widget)opMenuData[number].menupane, trueOrFalse);
    }
  }
  else if( widgetType==pullDownWidget )
  {
    if( number>=0 && number<n_pullDownMenu  && pdMenuData!=NULL )
    {
      pdMenuData[number].sensitive=trueOrFalse;
      if( pdMenuData[number].menupane!=NULL )
	XtSetSensitive((Widget)pdMenuData[number].menupane, trueOrFalse);
    }
  }
  else if(  widgetType==pushButtonWidget )
  {
    if( number>=0 && number<n_pButtons  && pButtons!=NULL )
    {
      pButtons[number].sensitive=trueOrFalse;
      if( pButtons[number].pb!=NULL )
	XtSetSensitive((Widget)pButtons[number].pb, trueOrFalse);
    }
  }
  else if(  widgetType==textBoxWidget )
  {
    if( number>=0 && number<n_text  && textBoxes!=NULL )
    {
      textBoxes[number].sensitive=trueOrFalse;
      if( textBoxes[number].textWidget!=NULL )
	XtSetSensitive((Widget)textBoxes[number].textWidget, trueOrFalse);
      if( textBoxes[number].labelWidget!=NULL )
	XtSetSensitive((Widget)textBoxes[number].labelWidget, trueOrFalse);  
    }
  }
  else if(  widgetType==toggleButtonWidget )
  {
    if( number>=0 && number<n_toggle  && tButtons!=NULL )
    {
      tButtons[number].sensitive=trueOrFalse;
      if( tButtons[number].tb!=NULL )
	XtSetSensitive((Widget)tButtons[number].tb, trueOrFalse);
    }
  }
  else if( widgetType==radioBoxWidget )
  {
    if( number>=0 && number<n_radioBoxes  && radioBoxData!=NULL )
    {
      radioBoxData[number].setSensitive(trueOrFalse);
    }
  }
  else
  {
    printf("DialogData::setSensitive:ERROR: unknown widgetType=%i\n",(int)widgetType);
  }
}

//\begin{>>DialogDataInclude.tex}{\subsection{setSensitive}}
void DialogData::
setSensitive(bool trueOrFalse, WidgetTypeEnum widgetType, const aString & label)
// ======================================================================================================
// /Description:
//    Set the sensitivity of a widget in the DialogData
// /trueOrFalse (input): set senstive or not
// /widgetType (input): choose a widget type to assign. One of 
// \begin{verbatim}
//  enum WidgetTypeEnum  
//  {
//    optionMenuWidget,
//    pushButtonWidget,
//    pullDownWidget,
//    toggleButtonWidget,
//    textBoxWidget,
//    radioBoxWidget
// };
// \end{verbatim}
// /label (input) : set sensitivity for the widget with this label
//\end{DialogDataInclude.tex}
// ======================================================================================================
{
  int number=-1;
  int i;
  if( widgetType==optionMenuWidget )
  {
    if( opMenuData!=NULL )
    {
      for( i=0; i<n_optionMenu; i++ )
      {
	if( label==opMenuData[i].optionLabel )
	{
	  number=i;
	  break;
	}
      }
      if( number<0 )
      {
	printf("ERROR:setSensitive:optionMenuWidget: label [%s] not found!\n"
	       "   Valid labels are\n",(const char*)label.c_str());
	for( i=0; i<n_optionMenu; i++ )
	  printf("[%s]\n",(const char*)opMenuData[i].optionLabel.c_str());
      }
      
      if( number>=0 && number<n_optionMenu )
      {
	opMenuData[number].setSensitive(trueOrFalse);
      }
    }
    
  }
  else if( widgetType==pullDownWidget )
  {
    if( pdMenuData!=NULL )
    {
      for( i=0; i<n_pullDownMenu; i++ )
      {
	if( label==pdMenuData[i].menuTitle )
	{
	  number=i;
	  break;
	}
      }
      if( number<0 )
      {
	printf("ERROR:setSensitive:pullDownWidget: label [%s] not found!\n"
	       "   Valid labels are\n",(const char*)label.c_str());
	for( i=0; i<n_pullDownMenu; i++ )
	  printf("[%s]\n",(const char*)pdMenuData[i].menuTitle.c_str());
      }
      if( number>=0 && number<n_pullDownMenu  && pdMenuData!=NULL )
      {
	pdMenuData[number].sensitive=trueOrFalse;
	if( pdMenuData[number].menupane!=NULL )
	  XtSetSensitive((Widget)pdMenuData[number].menupane, trueOrFalse);
      }
    }
  }
  else if(  widgetType==pushButtonWidget )
  {
    if( pButtons!=NULL )
    {
      for( i=0; i<n_pButtons; i++ )
      {
	if( label==pButtons[i].buttonLabel )
	{
	  number=i;
	  break;
	}
      }
      if( number<0 )
      {
	printf("ERROR:setSensitive:pushButtonWidget: label [%s] not found!\n"
	       "   Valid labels are\n",(const char*)label.c_str());
	for( i=0; i<n_pButtons; i++ )
	  printf("[%s]\n",(const char*)pButtons[i].buttonLabel.c_str());
	
      }
      if( number>=0 && number<n_pButtons  && pButtons!=NULL )
      {
	pButtons[number].sensitive=trueOrFalse;
	if( pButtons[number].pb!=NULL )
	  XtSetSensitive((Widget)pButtons[number].pb, trueOrFalse);
      }
    }
  }
  else if(  widgetType==textBoxWidget )
  {
    if( textBoxes!=NULL )
    {
      for( i=0; i<n_text; i++ )
      {
	if( label==textBoxes[i].textLabel )
	{
	  number=i;
	  break;
	}
      }
      if( number<0 )
      {
	printf("ERROR:setSensitive:textBoxWidget: label [%s] not found!\n"
	       "   Valid labels are\n",(const char*)label.c_str());
	for( i=0; i<n_text; i++ )
	  printf("[%s]\n",(const char*)textBoxes[i].textLabel.c_str());
      }
      
      if( number>=0 && number<n_text  && textBoxes!=NULL )
      {
	textBoxes[number].sensitive=trueOrFalse;
	if( textBoxes[number].textWidget!=NULL )
	  XtSetSensitive((Widget)textBoxes[number].textWidget, trueOrFalse);
	if( textBoxes[number].labelWidget!=NULL )
	  XtSetSensitive((Widget)textBoxes[number].labelWidget, trueOrFalse);  
      }
    }
  }
  else if(  widgetType==toggleButtonWidget )
  {
    if( tButtons!=NULL )
    {
      for( i=0; i<n_toggle; i++ )
      {
	if( label==tButtons[i].buttonLabel )
	{
	  number=i;
	  break;
	}
      }
      if( number<0 )
      {
	printf("ERROR:setSensitive:toggleButtonWidget: label [%s] not found!\n"
	       "   Valid labels are\n",(const char*)label.c_str());
	for( i=0; i<n_toggle; i++ )
	  printf("[%s]\n",(const char*)tButtons[i].buttonLabel.c_str());
      }
      
      if( number>=0 && number<n_toggle )
      {
	tButtons[number].sensitive=trueOrFalse;
	if( tButtons[number].tb!=NULL )
	  XtSetSensitive((Widget)tButtons[number].tb, trueOrFalse);
      }
    }
    
  }
  else if( widgetType==radioBoxWidget )
  {
    if( radioBoxData!=NULL )
    {
      for( i=0; i<n_radioBoxes; i++ )
      {
	if( label==radioBoxData[i].radioLabel )
	{
	  number=i;
	  break;
	}
      }
      if( number<0 )
      {
	printf("ERROR:setSensitive:radioBoxWidget: label [%s] not found!\n"
	       "   Valid labels are\n",(const char*)label.c_str());
	for( i=0; i<n_radioBoxes; i++ )
	  printf("[%s]\n",(const char*)radioBoxData[i].radioLabel.c_str());
      }
      if( number>=0 && number<n_radioBoxes  && radioBoxData!=NULL )
      {
	radioBoxData[number].setSensitive(trueOrFalse);
      }
    }
  }
  else
  {
    printf("DialogData::setSensitive:ERROR: unknown widgetType=%i\n",(int)widgetType);
  }
}


void DialogData::
closeDialog()
{
  int i;
  
  if (dialogWindow)
  {

    // *wdh* 100712 -- clean up these other objects: 
    deleteOptionMenus();    
    deleteToggleButtons();
    deleteInfoLabels();

    for (i=0; i<n_text; i++)
    {
      XtDestroyWidget((Widget)textBoxes[i].textWidget);
      textBoxes[i].textWidget = NULL;
    }
    XtDestroyWidget((Widget)dialogWindow); // make sure it isn't closed twice!
  }
  
  dialogWindow = NULL; // remember that the dialog window now is closed
}

void DialogData::
openDialog(int managed /* = 1*/)
//
//  /Purpose:
//  Call MOTIF to create a generic dialog window
//  /Author: AP
//-----------------------------------------------------
{
  // Create the dialog -- the main text window acts as the DialogShell's parent.

  XmString t;
  Arg args[7];
  int n = 0;
  int i, j;

  char buff[200];

// check if the dialog already is opened
  if (dialogWindow)
    return;

// check if the dialog is empty.
// NOTE: just having an infoLabel is NOT enough, since there is no way of
// taking down the dialog window without any buttons. If you just want to output a message, call 
// moglCreateMessageDialog() or GenericGraphicsInterface::createMessageDialog().
  if (!exitCommandSet && n_pButtons == 0 && n_toggle == 0 && n_text == 0 && n_optionMenu == 0 && 
      n_pullDownMenu == 0)
    return;

  t = XmStringCreateLocalized (SC exitLabel.c_str());
  
  XtSetArg (args[n], XmNcancelLabelString, t); n++;
  XtSetArg (args[n], XmNautoUnmanage, False); n++;
  XtSetArg (args[n], XtNtitle, SC windowTitle.c_str()); n++; 
  XtSetArg (args[n], XmNdefaultPosition, False); n++; 
// only position the dialog window once!
  XtSetArg (args[n], XmNuserData, (XtPointer) 0); n++; 
  if( overlayVisual )
  {
    // ** more here ***
    XtSetArg (args[n], XmNvisual, overlayVisual ); n++;
  }
  
  Widget dialog = XmCreateTemplateDialog(textw, (char*)"generic dialog", args, n );
  dialogWindow = dialog;
  
  XmStringFree (t); /* always destroy compound strings when done */

  XtAddCallback (dialog, XmNcancelCallback, destroyUserDialog, this);
  XtAddCallback (dialog, XmNmapCallback, positionDialog, NULL);

// this rowColumn holds all the buttons, labels, texts, etc.
  Widget baseRC = XtVaCreateWidget ("rowcol", xmRowColumnWidgetClass, dialog,
				    XmNorientation, XmVERTICAL,
				    XmNpacking, XmPACK_TIGHT, 
				    NULL);

  Widget menupane, btn, cascade;
  XmString abbrevString;

//-------------------------------------CREATE PULLDOWN MENUBAR--------------------------------
  int nMenuPaneArgs=0;
  if (n_pullDownMenu > 0)
  {
    Widget topMenuBar = XmCreateMenuBar(dialog, (char*)"menubar", NULL, 0);

    nMenuPaneArgs = 0;
    // On the DEC the next line causes nMenuPaneArgs to be increased twice!
    // ---> yes because XtSetArg is a macro! as defined in the X manuals.
    //XtSetArg(menuPaneArgs[nMenuPaneArgs++], XmNtearOffModel, XmTEAR_OFF_ENABLED); 
    // printf(" nMenuPaneArgs=%i *** should be 1\n",nMenuPaneArgs);

    XtSetArg(menuPaneArgs[nMenuPaneArgs], XmNtearOffModel, XmTEAR_OFF_ENABLED); nMenuPaneArgs++;
    if( overlayVisual )
    {
      XtSetArg(menuPaneArgs[nMenuPaneArgs], XmNvisual, overlayVisual);  nMenuPaneArgs++;
      XtSetArg(menuPaneArgs[nMenuPaneArgs], XmNdepth,  overlayDepth); nMenuPaneArgs++;
      XtSetArg(menuPaneArgs[nMenuPaneArgs], XmNvisual, overlayColormap); nMenuPaneArgs++;
    }
    

    for (i=0; i<n_pullDownMenu; i++)
    {
      if (pdMenuData[i].type == GI_PUSHBUTTON)
      {
	menupane = XmCreatePulldownMenu(topMenuBar, (char*)"menupane", menuPaneArgs, nMenuPaneArgs);

	for( j=0; j < pdMenuData[i].n_button; j++ )
	{
	  abbrevString = XmStringCreateLocalized(SC pdMenuData[i].pbList[j].buttonLabel.c_str());
	  btn = XtVaCreateManagedWidget(SC pdMenuData[i].pbList[j].buttonCommand.c_str(),
					xmPushButtonWidgetClass, menupane, 
					XmNlabelString, abbrevString,
					NULL);
	  pdMenuData[i].pbList[j].pb = btn;
	  XmStringFree (abbrevString); /* always destroy compound strings when done */
	  XtSetSensitive(btn, pdMenuData[i].pbList[j].sensitive);
	  XtAddCallback(btn, XmNactivateCallback, userButtonCallback, this);
	}
    
	XtSetArg(args[0], XmNsubMenuId, menupane);
	cascade = XmCreateCascadeButton(topMenuBar, SC pdMenuData[i].menuTitle.c_str(), args, 1);
	XtManageChild(cascade);
      }
      else if (pdMenuData[i].type == GI_TOGGLEBUTTON)
      {
	menupane = XmCreatePulldownMenu(topMenuBar, (char*)"menupane", menuPaneArgs, nMenuPaneArgs);

	for( j=0; j < pdMenuData[i].n_button; j++ )
	{
	  abbrevString = XmStringCreateLocalized(SC pdMenuData[i].tbList[j].buttonLabel.c_str());
	  btn = XtVaCreateManagedWidget(SC pdMenuData[i].tbList[j].buttonCommand.c_str(), 
					xmToggleButtonWidgetClass, menupane,
					XmNindicatorType, XmN_OF_MANY,
					/* XmNvisibleWhenOff makes the indicator visible, even when it is off */
					XmNvisibleWhenOff, True, 
					XmNlabelString, abbrevString,
					XmNuserData, this,
					NULL);
          pdMenuData[i].tbList[j].tb=btn;  // *wdh*
	  XmStringFree (abbrevString); /* always destroy compound strings when done */
	  XtSetSensitive(btn, pdMenuData[i].tbList[j].sensitive);
	  XtAddCallback(btn, XmNvalueChangedCallback, userToggleButtonCallback, 
			&pdMenuData[i].tbList[j]);
// set the initial state of the button
	  XmToggleButtonSetState(btn, pdMenuData[i].tbList[j].state, False);
	}
    
	XtSetArg(args[0], XmNsubMenuId, menupane);
	cascade = XmCreateCascadeButton(topMenuBar, SC pdMenuData[i].menuTitle.c_str(), args, 1);
	XtManageChild(cascade);
      }
    
// save the handle to the menupane for later use
      pdMenuData[i].menupane = menupane;

      if (i==n_pullDownMenu-1 && pdLastIsHelp == 1)
	XtVaSetValues(topMenuBar, XmNmenuHelpWidget, cascade, NULL); // only do this if pdLastIsHelp == 1.

      XtSetSensitive((Widget)pdMenuData[i].menupane, pdMenuData[i].sensitive);  // *wdh* 
    }
    XtManageChild(topMenuBar);
//---------------------------------END CREATING PULLDOWN MENUS----------------------
  }

//
// info labels
//
  if (n_infoLabels>0)
  {
// border around all labels
    Dimension borderW=2;
    Widget infoLabelBorder = XtVaCreateWidget("radioBorder", xmFrameWidgetClass, baseRC,
					    XmNmarginWidth, borderW,
					    XmNmarginHeight, borderW,
					    NULL);
// frame for all radio buttons
    Widget infoLabelFrame = XtVaCreateWidget ("infoLabelFrame", 
					      xmRowColumnWidgetClass, infoLabelBorder,
					      XmNpacking, /* XmPACK_COLUMN */ XmPACK_TIGHT,
					      XmNorientation, XmVERTICAL,
					      XmNnumColumns, 1,
					      NULL);
    for (i=0; i<n_infoLabels; i++)
    {
      abbrevString = XmStringCreateLocalized(SC infoLabelData[i].textLabel.c_str());
      infoLabelData[i].labelWidget = 
	XtVaCreateManagedWidget("infoLabel", xmLabelWidgetClass, infoLabelFrame, 
				XmNlabelString, abbrevString, NULL);
      XmStringFree (abbrevString); /* always destroy compound strings when done */
    }
// done with the info label frame
    XtManageChild(infoLabelFrame);
    XtManageChild(infoLabelBorder);
  }
  
  
//
// Radio Buttons
//

  if (n_radioBoxes>0)
  {
// frame for all radio buttons
    Widget radioBoxFrame = XtVaCreateWidget ("radioBoxFrame", 
					     xmRowColumnWidgetClass, baseRC,
					     XmNpacking, /* XmPACK_COLUMN */ XmPACK_TIGHT,
					     XmNorientation, XmVERTICAL,
					     XmNnumColumns, 1,
					     NULL);
    for (i=0; i<n_radioBoxes; i++)
    {
      Dimension borderW=2;
      Widget radioBorder = XtVaCreateWidget("radioBorder", xmFrameWidgetClass, radioBoxFrame,
					    XmNmarginWidth, borderW,
					    XmNmarginHeight, borderW,
					    NULL);
      Widget radioCommentFrame = XtVaCreateWidget("commentFrame", xmRowColumnWidgetClass, 
						  radioBorder,
						  XmNpacking, XmPACK_TIGHT,
						  XmNorientation, XmVERTICAL,
						  NULL);
      Widget radioComment = XtVaCreateManagedWidget(SC radioBoxData[i].radioLabel.c_str(), 
						    xmLabelWidgetClass, 
						    radioCommentFrame, 
						    NULL);

      int n_radioArgs=0;
      Arg radioArgs[7];

      XtSetArg(radioArgs[n_radioArgs], XmNnumColumns, radioBoxData[i].columns); 
      n_radioArgs++;
      
// user defined radiobox
      Widget radioBox = XmCreateRadioBox(radioCommentFrame, (char*)"radioBox", 
					 radioArgs, n_radioArgs);

// save the handle to the menu frame for later use
      radioBoxData[i].radioBox = radioBox;
// set the initial sensitivity for the entire radiobox
      XtSetSensitive((Widget)radioBoxData[i].radioBox, radioBoxData[i].sensitive);

// fill in all the menus
      Widget btn;
      Widget btn1=NULL;
  
      for (j=0; j<radioBoxData[i].n_options; j++)
      {
	abbrevString = XmStringCreateLocalized (SC radioBoxData[i].optionList[j].buttonLabel.c_str());
	btn = XtVaCreateManagedWidget(SC radioBoxData[i].optionList[j].buttonCommand.c_str(), 
				      xmToggleButtonGadgetClass, radioBox, 
				      XmNlabelString, abbrevString, 
				      XmNuserData, this,
				      NULL);
	radioBoxData[i].optionList[j].tb=btn; // save a pointer to the toggle button widget
	XmStringFree (abbrevString); /* always destroy compound strings when done */
// initial sensitivity for this togglebutton
	XtSetSensitive(btn, radioBoxData[i].optionList[j].sensitive); 
	XtAddCallback(btn, XmNvalueChangedCallback, userRadioBoxCallback, &radioBoxData[i] );
// get a handle to the button with the currentChoice
	if (radioBoxData[i].optionList[j].buttonCommand == radioBoxData[i].currentChoice)
	{
	  radioBoxData[i].currentIndex=j;
	  btn1=btn; 
	}
	
      }
// set current value
      if (btn1) 
	XmToggleButtonSetState(btn1, True, False); // turn the default case on

      XtManageChild((Widget) radioBoxData[i].radioBox);
      XtManageChild( radioCommentFrame );
      XtManageChild( radioBorder );
    }
  
// done with the radio button frame
    XtManageChild(radioBoxFrame);
  }

// option menus
  if (n_optionMenu>0)
  {
// frame for all option menus
    Widget optionMenuFrame = XtVaCreateWidget ("optionMenuFrame", 
					       xmRowColumnWidgetClass, baseRC,
					       XmNpacking, XmPACK_COLUMN,
					       XmNorientation, XmVERTICAL,
					       XmNnumColumns, optionMenuColumns,
					       NULL);
    for (i=0; i<n_optionMenu; i++)
    {
// make an option menu
      menupane = XmCreatePulldownMenu(optionMenuFrame, (char*)"menuPane", NULL, 0);
// save the handle to the menupane for later use
      opMenuData[i].menupane = menupane;
      XtSetSensitive((Widget)opMenuData[i].menupane, opMenuData[i].sensitive);  // *wdh* 

      XmString bgText = XmStringCreateLocalized(SC opMenuData[i].optionLabel.c_str());
      
      // try this: XmString bgText   = XmStringCreateLtoR(SC opMenuData[i].optionLabel.c_str(), XmFONTLIST_DEFAULT_TAG); // horizontal XmString

// make argument list for the option menu
      n=0;

      XtSetArg(args[n], XmNsubMenuId, menupane); n++;

// *wdh* 100324 -- trouble with next line and invalid read; 
      XtSetArg(args[n], XmNlabelString, bgText); n++;

      Widget bgmenu = XmCreateOptionMenu(optionMenuFrame, (char*)"optionMenu", args, n);

      XmStringFree(bgText); // always free strings after we are done with them

// save the handle to the menu frame for later use
      opMenuData[i].menuframe = bgmenu;

// fill in all the menus
      Widget btn;
      Widget btn1=NULL;
  
      for (j=0; j<opMenuData[i].n_options; j++)
      {
	abbrevString = XmStringCreateLocalized (SC opMenuData[i].optionList[j].buttonLabel.c_str());
	btn = XtVaCreateManagedWidget(SC opMenuData[i].optionList[j].buttonCommand.c_str(), 
				      xmPushButtonWidgetClass, menupane, 
				      XmNlabelString, abbrevString, 
				      XmNuserData, this /*(XtPointer) j*/, // save the position in the array
				      NULL);
	opMenuData[i].optionList[j].pb=btn; // save a pointer to the push button widget to set sensitivity
	XmStringFree (abbrevString); /* always destroy compound strings when done */
	XtSetSensitive(btn, opMenuData[i].optionList[j].sensitive); // initial sensitivity
	XtAddCallback(btn, XmNactivateCallback, userOpMenuCallback, &opMenuData[i] );
        // get a handle to the button with the currentChoice
	if (opMenuData[i].optionList[j].buttonCommand == opMenuData[i].currentChoice)
	  btn1=btn; 
      }
      if (btn1) XtVaSetValues((Widget) opMenuData[i].menuframe, XmNmenuHistory, btn1, NULL);

      XtManageChild((Widget) opMenuData[i].menuframe);
    }
  
    // done with the option menu frame
    XtManageChild(optionMenuFrame);
  }
  

  if (n_pButtons > 0)
  {
    // frame for all push buttons
    Widget pushButtonFrame = XtVaCreateWidget ("pushButtonFrame", 
					       xmRowColumnWidgetClass, baseRC,
					       XmNpacking, XmPACK_COLUMN,
					       XmNorientation, XmHORIZONTAL,
					       XmNnumColumns, pButtonRows,
					       NULL);
// insert push buttons
    Widget button;
    for (i=0; i<n_pButtons; i++)
    {
      abbrevString = XmStringCreateLocalized (SC pButtons[i].buttonLabel.c_str());
      button = XtVaCreateManagedWidget(SC pButtons[i].buttonCommand.c_str(), 
				       xmPushButtonWidgetClass, 
				       pushButtonFrame, 
				       XmNlabelString, abbrevString,
				       NULL);
      pButtons[i].pb = button;
      XtSetSensitive((Widget)pButtons[i].pb, pButtons[i].sensitive);  // *wdh* 
      XtAddCallback(button, XmNactivateCallback, userButtonCallback, this);
      XmStringFree (abbrevString); /* always destroy compound strings when done */
    }
  
// done defining the smallButtonFrame
    XtManageChild (pushButtonFrame);
  }
  

  if (n_toggle > 0)
  {
// frame for all toggle buttons
    Widget toggleButtonFrame = XtVaCreateWidget ("toggleButtonFrame", 
						 xmRowColumnWidgetClass, baseRC,
						 XmNpacking, XmPACK_COLUMN,
						 XmNorientation, XmVERTICAL,
						 XmNnumColumns, toggleButtonColumns,
						 NULL);

// Toggle buttons
    for (i=0; i<n_toggle; i++)
    {
      abbrevString = XmStringCreateLocalized (SC tButtons[i].buttonLabel.c_str());
      Widget button = XtVaCreateManagedWidget(SC tButtons[i].buttonCommand.c_str(), 
					      xmToggleButtonWidgetClass, 
					      toggleButtonFrame,
					      XmNindicatorType, XmN_OF_MANY,
					      XmNlabelString, abbrevString,
					      XmNuserData, this,
					      NULL);
      tButtons[i].tb = button;
      XtSetSensitive((Widget)tButtons[i].tb, tButtons[i].sensitive);  // *wdh* 
// add callback function here
      XtAddCallback(button, XmNvalueChangedCallback, userToggleButtonCallback, &tButtons[i]);
// set the initial state of the button
      XmToggleButtonSetState(button, tButtons[i].state, False); // set the current state
      XmStringFree (abbrevString); /* always destroy compound strings when done */
    }
  
// done defining the toggleButtonFrame
    XtManageChild (toggleButtonFrame);
  }
  

  if (n_text>0)
  {
// frame for all textLabels
    Widget outerTextLabelFrame = XtVaCreateWidget ("outerTextLabelFrame", 
						   xmRowColumnWidgetClass, baseRC,
						   XmNpacking, XmPACK_COLUMN, 
						   XmNnumColumns, 1,
						   XmNorientation, XmVERTICAL,
						   XmNisAligned, True,
						   XmNentryAlignment, XmALIGNMENT_END,
						   NULL);

    Widget oneFrame;
    Widget commandText;
    Widget xValue;
    for (i=0; i<n_text; i++)
    {
//        oneFrame = XtVaCreateWidget ("textLabelFrame", 
//  				   xmRowColumnWidgetClass, outerTextLabelFrame,
//  				   XmNpacking, XmPACK_TIGHT, /*XmPACK_COLUMN,*/
//  //					      XmNnumColumns, n_text,
//  				   XmNorientation, XmHORIZONTAL,
//  				   NULL);
      oneFrame = XtVaCreateWidget ("textLabelFrame", 
				   xmFormWidgetClass, outerTextLabelFrame,
				   XmNtopAttachment, i ? XmATTACH_WIDGET : XmATTACH_FORM,
				   XmNtopWidget, oneFrame,
				   XmNleftAttachment, XmATTACH_FORM,
				   XmNrightAttachment, XmATTACH_FORM,
				   NULL);

      textBoxes[i].labelWidget = commandText = XtVaCreateManagedWidget(SC textBoxes[i].textLabel.c_str(), 
					    xmLabelWidgetClass, oneFrame, 
					    XmNtopAttachment, XmATTACH_FORM,
					    XmNbottomAttachment, XmATTACH_FORM,
					    XmNleftAttachment, XmATTACH_FORM,
					    XmNalignment, XmALIGNMENT_BEGINNING,
					    NULL);
// save the pointer to the editable text so that we can access it later on...
      textBoxes[i].textWidget = xValue = 
	XtVaCreateManagedWidget(SC textBoxes[i].textCommand.c_str(), 
				xmTextFieldWidgetClass, oneFrame, 
				XmNrows, 1, 
				XmNuserData, this,
				XmNtopAttachment,    XmATTACH_FORM,
				XmNbottomAttachment, XmATTACH_FORM,
				XmNrightAttachment,  XmATTACH_FORM,
				XmNleftAttachment,   XmATTACH_WIDGET,
				XmNleftWidget,       commandText,
				NULL); 

      XtAddCallback(xValue, XmNactivateCallback, userTextLabelCallback, &textBoxes[i] );

// set current value
      XmTextSetString(xValue, SC textBoxes[i].string.c_str());
      XtManageChild(oneFrame);

      XtSetSensitive((Widget)textBoxes[i].textWidget, textBoxes[i].sensitive);  // *wdh* 
      XtSetSensitive((Widget)textBoxes[i].labelWidget, textBoxes[i].sensitive);  

    }
  
// done defining the textLabelFrame
    XtManageChild (outerTextLabelFrame);
  }
  
  XtManageChild (baseRC);

  if (managed) XtManageChild (dialog);

}

//\begin{>>DialogDataInclude.tex}{\subsection{changeOptionMenu}} 
bool DialogData::
changeOptionMenu(int nOption, const aString opCommands[], const aString opLabels[], int initCommand)
//-----------------------------------------------------
// /Description: Change the menu items in an option menu after it has been created (by pushGUI)
//
// /nOption(input): Change option menu \# nOption.
// /opCommands(input): An array of strings with the command that will be issued when each menu
//  item is selected. The array must be terminated by an empty string ("").
// /opLabels(input): An array of strings with the label that will be put on each menu
//  item. The array must be terminated by an empty string ("").
// /initCommand(input): The index of the initial selection in the opLabels array. This label will
//  appear on top of the option menu to indicate the initial setting.
// /Return values: The function returns true on a successful completion and false if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  if (nOption < 0 || nOption >= n_optionMenu)
  {
    printf("changeOptionMenu: ERROR: nOption=%i out of bounds=[0,%i)\n", nOption, n_optionMenu);
    return false;
  }
  
  int j, i = nOption;
// Only deactivate the present buttons if dialogWindow has been created! When reading a command file,
// pushGUI does not create any widgets, so this case actually occurs!
  if (dialogWindow)
  {
// first deactivate the present menu buttons
    XtUnmanageChild((Widget)opMenuData[i].menuframe);
    for (j=0; j<opMenuData[i].n_options; j++)
    {
      XtDestroyWidget((Widget) opMenuData[i].optionList[j].pb ); 
    }
  }

// free up the old data structure
  delete [] opMenuData[i].optionList;
  
// opCommands and opLabels are supposed to be "" terminated arrays of aString.
// Start by counting the number of elements
  int ne;
  for (ne=0; opCommands[ne] != "" && opLabels[ne] != ""; ne++);
  if (ne == 0)
  {
    printf("changeOptionMenu: WARNING: Empty list of option commands. Not building an empty options menu.\n");
    return false;
  }

// fill in the new values
  opMenuData[i].n_options = ne;
  opMenuData[i].optionList = new PushButton [ne];

// copy all the strings
  for (j=0; j<ne; j++)
  {
    opMenuData[i].optionList[j].buttonCommand = opCommands[j];
    opMenuData[i].optionList[j].buttonLabel = opLabels[j];
  }
  opMenuData[i].currentChoice = opMenuData[i].optionList[initCommand].buttonCommand; // *wdh* 090528
  

// Only build the new motif widgets for the menu buttons if the dialog window has been created
  if (dialogWindow)
  {
    Widget btn;
    Widget btn1=NULL;
    XmString abbrevString;
  
    for (j=0; j<opMenuData[i].n_options; j++)
    {
      abbrevString = XmStringCreateLocalized (SC opMenuData[i].optionList[j].buttonLabel.c_str());
      btn = XtVaCreateManagedWidget(SC opMenuData[i].optionList[j].buttonCommand.c_str(), 
				    xmPushButtonWidgetClass, (Widget) opMenuData[i].menupane, 
				    XmNlabelString, abbrevString, 
				    XmNuserData, this /*(XtPointer) j*/, // save the position in the array
				    NULL);
      opMenuData[i].optionList[j].pb=btn; // save a pointer to the push button widget to set sensitivity
      XmStringFree (abbrevString); /* always destroy compound strings when done */
      XtSetSensitive(btn, opMenuData[i].optionList[j].sensitive); // initial sensitivity
      XtAddCallback(btn, XmNactivateCallback, userOpMenuCallback, &opMenuData[i] );
// get a handle to the button with the currentChoice
      if (opMenuData[i].optionList[j].buttonCommand == opMenuData[i].currentChoice)
	btn1=btn; 
    }
    if (btn1) XtVaSetValues((Widget) opMenuData[i].menuframe, XmNmenuHistory, btn1, NULL);

// remanage the menuframe
    XtManageChild((Widget) opMenuData[i].menuframe);
  }

  return true;
}

//\begin{>>DialogDataInclude.tex}{\subsection{showSibling}} 
int DialogData::
showSibling()
//----------------------------------------------
// /Description: Show a sibling (dialog) window that previously was allocated with 
// getDialogSibling() and created with pushGUI().
//
// /Returnvalues: The function returns 1 if the sibling could be shown, otherwise 0 
// (in which case it doesn't exist or already is shown).
// /Author: AP
//\end{DialogDataInclude.tex}
//----------------------------------------------
{
  Widget d = (Widget) dialogWindow;
  if (d && !XtIsManaged(d))
  {
    XtManageChild(d);
    return 1;
  }
  else
    return 0;
}


//\begin{>>DialogDataInclude.tex}{\subsection{hideSibling}} 
int DialogData::
hideSibling() 
//----------------------------------------------
// /Description: Hide a sibling (dialog) window that previously was allocated with 
// getDialogSibling(), created with pushGUI() and shown with showSibling().
//
// /Returnvalues: The function returns 1 if the sibling could be hidden, otherwise 0 
// (in which case it doesn't exist or already is hidden).
// /Author: AP
//\end{DialogDataInclude.tex}
//----------------------------------------------
{
  Widget d = (Widget) dialogWindow;
  if (d && XtIsManaged(d))
  {
    XtUnmanageChild(d);
    return 1;
  }
  else
    return 0;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setTextLabel}} 
int DialogData::
setTextLabel(int n, const aString &buff)
//----------------------------------------------------
// /Description: Set the text string in textlabel \# n in the currently
// active GUIState.
//
// /n(input): The index of the text label in the array given to setTextBoxes during setup.
// /buff(input): The new text string.
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  if (n >= n_text)
    return 0;
  
  TextLabel & tl = textBoxes[n];

// copy buff to tl.string
  tl.string = buff;

// copy tl.string to the text widget
  if( dialogWindow!=NULL && textBoxes!=NULL && textBoxes[n].textWidget!=NULL )
    XmTextSetString((Widget)textBoxes[n].textWidget, SC tl.string.c_str());

  return 1;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setInfoLabel}} 
bool DialogData::
setInfoLabel(int n, const aString &buff)
//----------------------------------------------------
// /Description: Set the text string in info label \# n in the currently
// active GUIState.
//
// /n(input): The index of the text label returned by addInfoLabel during the setup.
// /buff(input): The new text string.
//
// /Return code: true if the label could be changed successfully, otherwise false
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  if (n < 0 || n >= n_infoLabels)
    return false;
  
// always change the text
  infoLabelData[n].textLabel = buff;

// only change the widget if the GUI is open
  if (infoLabelData[n].labelWidget != NULL)
  {
    XmString abbrevString = XmStringCreateLocalized(SC buff.c_str());
    XtVaSetValues((Widget) infoLabelData[n].labelWidget, XmNlabelString, abbrevString, NULL);
    XmStringFree (abbrevString); /* always destroy compound strings when done */
  }
  
  return true;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setToggleState}} 
int DialogData::
setToggleState(int n, int trueFalse)
//----------------------------------------------------
// /Description: Set the state of toggle button \# n in the currently
// active GUIState.
//
// /n(input): The index of the toggle button in the array given to setToggleButtons during setup.
// /trueFalse(input): trueFalse==1 turns the toggle button on, all other values turn it off.
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  if (n >= n_toggle)
    return 0;
  
// copy the state to the toggleButton
  
  tButtons[n].state = (trueFalse == 1)? 1:0;

// set the state of the toggle button widget
  if( tButtons[n].tb!=NULL )
    XmToggleButtonSetState((Widget)tButtons[n].tb, tButtons[n].state, False); // set the current state

  return 1;
}
// end of DialogData Class functions

//\begin{>>DialogDataInclude.tex}{\subsection{setToggleState}} 
int DialogData::
setToggleState( const aString & toggleButtonLabel,  int trueOrFalse)
//----------------------------------------------------
// /Description: Set the toggle state for the toggle button with the given label.
//
// /toggleButtonLabel(input): The label of the toggle button to set.
// /trueOrFalse(input): The new state.
//
// /Author: wdh
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  for( int i=0; i<n_toggle; i++ )
  {
    if( toggleButtonLabel==tButtons[i].buttonLabel )
    {
      setToggleState(i,trueOrFalse);
      break;
    }
  }

  return 0;
}



void DialogData::
setCursor(cursorTypeEnum c)
{
  Cursor crsr;
  
  if (dialogWindow && XtIsManaged((Widget) dialogWindow))
  {
    switch(c)
    {
    case pointerCursor:
      crsr = None;
      break;
    case watchCursor:
      crsr = cursor;
      break;
    default:
      crsr = None;
      break;
    }
    
    XSetWindowAttributes wAttrib;
    wAttrib.cursor=crsr;
    XChangeWindowAttributes(dpy,XtWindow((Widget)dialogWindow),CWCursor,&wAttrib );
  }
}




//\begin{>>ToggleButtonInclude.tex}{\subsection{setState}} 
int ToggleButton::
setState(bool trueOrFalse )
// =================================================================================
// /Description:
//   Set the state of a Toggle button
// /trueOrFalse:
//\end{ToggleButtonInclude.tex}
// =================================================================================
{
  state = trueOrFalse;
  if( tb!=NULL )
    XmToggleButtonSetState((Widget)tb, state, False); // set the current state

  return 0;
}


//\begin{>>PullDownMenuInclude.tex}{\subsection{setToggleState}} 
int PullDownMenu::
setToggleState(int n, bool trueOrFalse )
// =================================================================================
// /Description:
//   Set the state of a Toggle button in a pulldown menu.
// /trueOrFalse:
//\end{PullDownMenuInclude.tex}
// =================================================================================
{
  if( type == GI_PUSHBUTTON || n<0 || n>=n_button )
    return 1;
  
  return tbList[n].setState(trueOrFalse);
}

void
showHardcopyDialog(Widget pb, XtPointer client_data, XtPointer call_data)
{
  Widget d = (Widget) client_data;
  if (d && !XtIsManaged(d))
  {
    XtManageChild(d);
  }
}


//\begin{>>OptionMenuInclude.tex}{\subsection{setCurrentChoice}} 
int OptionMenu::
setCurrentChoice(int command)
// ====================================================================================
// /Description:
//    Set the current choice for an option menu. 
//\end{OptionMenuInclude.tex}
// ===================================================================================
{
  if (command >= 0 && command < n_options)
    currentChoice = optionList[command].buttonCommand;
  else
// use the first choice if command is not valid
  {
    currentChoice = optionList[0].buttonCommand; 
    printf(" OptionMenu::setCurrentChoice: WARNING: command=%i out of bounds, using the first "
	   "item as initial choise\n", command);

    printf(" optionLabel=%s, Valid labels are:\n",(const char*)optionLabel.c_str());
    for( int j=0; j<n_options; j++ )
      printf("[%s]\n",(const char*)optionList[j].buttonLabel.c_str());  
  }

  if( menuframe!=NULL )
  {
    Widget btn1=NULL;
    for( int j=0; j<n_options; j++)
    {
      if (optionList[j].buttonCommand == currentChoice)
	btn1=(Widget)optionList[j].pb;
    }
    if( btn1 ) XtVaSetValues((Widget) menuframe, XmNmenuHistory, btn1, NULL);
  }
  

  return 0;
}

//\begin{>>OptionMenuInclude.tex}{\subsection{setCurrentChoice}} 
int OptionMenu::
setCurrentChoice(const aString & label)
// ====================================================================================
// /Description:
//    Set the current choice for an option menu. 
//\end{OptionMenuInclude.tex}
// ===================================================================================
{
  int j;
  for( j=0; j<n_options; j++)
  {
    if( optionList[j].buttonLabel==label )
    {
      return setCurrentChoice(j);
    }
  }
  printf("OptionMenu::setCurrentChoice:ERROR: label [%s] not found in option menu=%s\n"
         "   Valid labels are\n",(const char*)label.c_str(),(const char*)optionLabel.c_str());
  for( j=0; j<n_options; j++ )
    printf("[%s]\n",(const char*)optionList[j].buttonLabel.c_str());  
  return 1;
}

  
//\begin{>RadioBoxInclude.tex}{\subsection{setCurrentChoice}} 
bool RadioBox::
setCurrentChoice(int command)
// ====================================================================================
// /Description:
//    Set the current choice for a radio box. 
// /command(input): The command to be chosen
// /Return value: true if the command could be chosen, otherwise false. A command cannot
// be chosen if it is insensitive or out of bounds.
// /Author: AP
//\end{RadioBoxInclude.tex}
// ===================================================================================
{
  if (command >= 0 && command < n_options)
  {
// only give the current choice to an active toggle button
    if ( optionList[command].sensitive )
      currentChoice = optionList[command].buttonCommand;
    else
      return false;
  }
  else
// use the first choice if command is not valid
  {
    currentChoice = optionList[0].buttonCommand; 
    printf(" RadioBox::setCurrentChoice: WARNING: command=%i out of bounds, using the first "
	   "item as initial choise\n", command);
    command = 0;
  }

  optionList[currentIndex].setState(false);
  ToggleButton & tb=optionList[command];
  tb.setState(true);
  currentIndex = command;
  
  return true;
}

//\begin{>>RadioBoxInclude.tex}{\subsection{setSensitive}} 
void RadioBox::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a RadioBox object.
// /trueOrFalse: The new state of the RadioBox widget
// /Return valuse: None
// /Author: AP
//\end{RadioBoxInclude.tex}
//------------------------
{ 
  sensitive=trueOrFalse;
  if( radioBox!=NULL ) XtSetSensitive((Widget)radioBox, trueOrFalse);
// do all the buttons
  int btn;
  for (btn=0; btn<n_options; btn++)
  {
    ToggleButton & tb = optionList[btn];
    tb.setSensitive(trueOrFalse);
  }
}  

//\begin{>>RadioBoxInclude.tex}{\subsection{setSensitive}} 
void RadioBox::
setSensitive(int btn, bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of one button in a RadioBox object.
// /btn: The button number in the radio box.
// /trueOrFalse: The new state of the toggle button widget
// /Return valuse: None
// /Author: AP
//\end{RadioBoxInclude.tex}
//------------------------
{ 
  if (btn>=0 && btn<n_options)
  {
    ToggleButton & tb = optionList[btn];
    tb.setSensitive(trueOrFalse);
  }
}  

