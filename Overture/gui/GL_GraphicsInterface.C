#include "GL_GraphicsInterface.h"
#include <string.h>
// #include <GL/gl.h>
// #include <GL/glu.h>
#include "xColours.h"

#ifndef NO_APP
#include "GenericDataBase.h"
#endif

#ifdef NO_APP
using GUITypes::real;
#define getLength size
#define getBound(x) size((x))-1
#define redim resize
using std::cout;
using std::endl;
#endif


#define CS (char *)(const char *)

// PF_DEBUG = if defined, lots of output for power wall debugging
//#define PF_DEBUG 1
#ifdef USE_POWERWALL

void
vdlInit( GL_GraphicsInterface *giPointer )
{
  if ( giPointer == NULL )
  { 
    cout << "vdlInit -- ERROR giPtr == NULL\n";
  }
  const int winzero = 0;
  giPointer->init(winzero);
}

#endif

int
checkGLError()
{
  // =======================================================================
  // this can be called to check and report for any gl errors, currently it is only
  // called in GL_GraphicsInterface::display
  // =======================================================================

  int err = glGetError();
  switch(err)
    {
    case GL_NO_ERROR : break;
    case GL_INVALID_ENUM:
      {
	cout<<"GL ERROR : unacceptable value to enumerated argument"<<endl;
	break;
      }
    case GL_INVALID_VALUE:
      {
	cout<<"GL ERROR : A numeric argument is  out  of range."<<endl;
	break; 
      }
    case GL_INVALID_OPERATION:
      {
	cout<<"GL ERROR : invalid operation "<<endl;
	break;
      }
    case GL_STACK_OVERFLOW:
      {
	cout<<"GL ERROR : stack overflow"<<endl;
	break;
      }
    case GL_STACK_UNDERFLOW:
      {
	cout<<"GL ERROR : stack underflow"<<endl;
	break;
      }
    case GL_OUT_OF_MEMORY:
      {
	cout<<"GL ERROR : out of memory"<<endl;
	break;
      }
    default:
      {
	cout<<"GL ERROR : unknown error"<<endl;
	break;
      }
    }

  return err;
}

extern "C" void 
OV_gluCallback ( GLenum err )
{
  // =======================================================================
  // error callback for glu problems
  // =======================================================================
  aString errst;
  errst = (char *)gluErrorString(err);
  cout<<"GLU ERROR : "<<errst<<endl;
}

static char**
copy( const char **input )
// =======================================================================
//  Copy a char** array into another.
//  It is up to you to call deleteArray when you are done.
// =======================================================================
{
  int i,numberOfEntries=0 ;
  for( i=0; input[i]!=NULL; i++ )
    numberOfEntries++;

  char** output = new char* [numberOfEntries+1];  
  for( i=0; i<numberOfEntries; i++ )
  {
    output[i] = new char [strlen(input[i])+1];
    strcpy(output[i],(const char*)input[i]);  
  }
  output[numberOfEntries]=NULL;
  return output;
}

static int
remove( char **output )
// =======================================================================
//  Delete a null terminated array of char*'s
// =======================================================================
{
  for( int i=0; output[i]!=NULL; i++ )
    delete [] output[i];

  delete [] output;
  return 0;
}

static aString*
copy( const aString *input )
// =======================================================================
//  Copy a aString array into another. "input" should have a null aString, "", as
// a terminator.
//  It is up to you to call deleteArray when you are done.
// =======================================================================
{
  int i,numberOfEntries=0 ;
  for( i=0; input[i]!=""; i++ )
    numberOfEntries++;

  aString *output = new aString [numberOfEntries+1];  
  for( i=0; i<numberOfEntries; i++ )
    output[i] = input[i];

  output[numberOfEntries]="";
  return output;
}

static int
remove( aString *output )
// =======================================================================
//  Delete a null terminated array of char*'s
// =======================================================================
{
  delete [] output;
  return 0;
}


static void
resize(GL_GraphicsInterface *giPointer, const int & win_number)
// this function is called by mogl as a call-back when the screen is resized.
{
  assert( giPointer!=NULL );
  giPointer->init(win_number); // should pass in the win_number here
}



void GL_GraphicsInterface::
init(const int & win_number) 
// this function gets called when the graphics window is opened 
// and every time the reset button is pressed
{
  if( !graphicsWindowIsOpen ) return;
  
  static int calledBefore=0;
  // *** userDefinedRotationPoint=false;
  
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);  
  glPolygonOffset(0.,0.);
  glDisable(GL_POLYGON_OFFSET_FILL);

  glEnable(GL_DEPTH_TEST);
// ***   glFrontFace(GL_CW);  // polygons defined in a clock-wise manner
  glDepthFunc(GL_LESS);       // z-buffer test <
 
#if defined(__sgi) && !defined(OV_USE_MESA)
#else
// *mesa3.0  glEnable(GL_LINE_SMOOTH);  // support fractional line widths
#endif

  glEnable(GL_NORMALIZE); // normalize normals (needed if we scale the figure)

  // Initialize to avoid UMR:
  viewPort[win_number][0]=viewPort[win_number][1]=0;
  viewPort[win_number][2]=viewPort[win_number][3]=1;
  glGetIntegerv( GL_VIEWPORT, viewPort[win_number] );
//  printF("init: viewPort = %i, %i, %i, %i \n",viewPort[win_number][0],viewPort[win_number][1],viewPort[win_number][2],viewPort[win_number][3]);

  // *ap* initialize the bounding box:
  if( !calledBefore) // calledBefore should be an array of size MAX_WINDOWS
  {
    resetGlobalBound(win_number);

    // *ap* check what is in the bounding box...
    //    GL_GraphicsInterface::globalBound.display("globalBound in init");
    calledBefore = 1;
  }

  real denom= viewPort[win_number][3] - viewPort[win_number][1]; // *wdh* 020408 this could be zero if graphics is not open
  if( denom>0. )
    aspectRatio[win_number] = (real) (viewPort[win_number][2] - viewPort[win_number][0])/denom;
  else
    aspectRatio[win_number]=1.;
  
  if (aspectRatio[win_number] > 1.0)
  {
    rightSide[win_number] =  aspectRatio[win_number];
    leftSide[win_number]  = -aspectRatio[win_number];
    top[win_number]       = 1.0;
    bottom[win_number]    = -1.0;
  }
  else
  {
    rightSide[win_number] =  1.0;
    leftSide[win_number]  = -1.0;
    top[win_number]       =  1./aspectRatio[win_number];
    bottom[win_number]    = -1./aspectRatio[win_number];
  }

  //..this updates the screen & the powerwall, maintains correct aspect ratio **pf
  graphics_setOrthoKeepAspectRatio( aspectRatio[win_number], magnificationFactor[win_number],
				    leftSide[win_number], rightSide[win_number], 
				    bottom[win_number], top[win_number],
				    near[win_number], far[win_number] );   

// initialize the name stack
  glInitNames(); 
}

void GL_GraphicsInterface::
setProjection(const int & win_number)
// This routine sets the left, right, top and bottom planes after magnification operations.
{
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);       // z-buffer test <

  int oldCurrentWindow = currentWindow;
  moglMakeCurrent(win_number);
  if( isGraphicsWindowOpen() )
  {
//..this updates the screen & the powerwall, maintains correct aspect ratio **pf
    graphics_setOrthoKeepAspectRatio( aspectRatio[win_number], magnificationFactor[win_number],
				      leftSide[win_number], rightSide[win_number], 
				      bottom[win_number], top[win_number],
				      near[win_number], far[win_number] );   
  }
  moglMakeCurrent(oldCurrentWindow);
}

void 
changeView(GL_GraphicsInterface *giPointer,
	   const int & win_number, 
	   const real & dx,   
	   const real & dy , 
	   const real & dz,
	   const real & dThetaX=0.,
	   const real & dThetaY=0.,
	   const real & dThetaZ=0.,
	   const real & magnify=1. )

{
  assert( giPointer!=NULL );
  giPointer->changeView(win_number,dx,dy,dz,dThetaX,dThetaY,dThetaZ,magnify); 
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setView}} 
void GL_GraphicsInterface::
setView(ViewLocation & loc, int win_number /* = -1 */)
//----------------------------------------------------------------------
// /Description: Set the view parameters. This function is often used together with getView.
// /loc(input): view parameters
// /win\_number(optional input): window number, use current window if absent
// /Return value: None
//  /Author: AP
//
//\end{GL_GraphicsInterfaceInclude.tex} 
{
//  printF("Inside setview\n");
  
  if (win_number == -1)
    win_number = getCurrentWindow();
  
  int i, j;

  xShift[win_number] = loc.shift[0];
  yShift[win_number] = loc.shift[1];
  zShift[win_number] = loc.shift[2];
  magnificationFactor[win_number] = loc.magnificationFactor;
  for (i=0; i<2; i++)
    for (j=0; j<3; j++)
      globalBound[win_number](i,j) = loc.globalBound[i][j];
  for (j=0; j<3; j++)
  {
    shiftCorrection[win_number][j] = loc.shiftCorrection[j];
    rotationCenter[win_number][j]  = loc.rotationCenter[j];
  }
  
  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
    {
      rotationMatrix[win_number](i,j) = loc.rotationMatrix[i][j];
    }
  
  userDefinedRotationPoint[win_number] = loc.userDefinedRotationPoint;

// necessary to redo the projection matrix since the magnification factor might have changed
  setProjection(win_number); 
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getView}} 
void GL_GraphicsInterface::
getView(ViewLocation & loc, int win_number /* = -1 */)
//----------------------------------------------------------------------
// /Description: return the view parameters. These can later be used when calling setView.
// /loc(output): view parameters
// /win\_number(optional input): window number, use current window if absent
// /Return value: None
//  /Author: AP
//
//\end{GL_GraphicsInterfaceInclude.tex} 
{
//  printF("Inside getview\n");
  int i, j;
  
  if (win_number == -1)
    win_number = getCurrentWindow();
  loc.shift[0] = xShift[win_number];
  loc.shift[1] = yShift[win_number];
  loc.shift[2] = zShift[win_number];
  loc.magnificationFactor = magnificationFactor[win_number];
  for (i=0; i<2; i++)
    for (j=0; j<3; j++)
      loc.globalBound[i][j] = globalBound[win_number](i,j);
  for (j=0; j<3; j++)
  {
    loc.shiftCorrection[j] = shiftCorrection[win_number][j];
    loc.rotationCenter[j]  = rotationCenter[win_number][j];
  }
  
  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
    {
      loc.rotationMatrix[i][j] = rotationMatrix[win_number](i,j);
    }
  
  loc.userDefinedRotationPoint = userDefinedRotationPoint[win_number];
}


void GL_GraphicsInterface::
changeView(const int & win_number,
	   const real & dx,   
	   const real & dy , 
	   const real & dz,
	   const real & dThetaX /* =0. */,
	   const real & dThetaY /* =0. */,
	   const real & dThetaZ /* =0. */,
	   const real & magnify /* =1. */ )
// =========================================================================
//this routine is called by mogl to change the view
//
//  dx,dy,dz : relative shift [-1,1]
//
// =========================================================================
{
#ifdef USE_POWERWALL
  //cout << "--GL_GraphicsInterface::changeView  called, win_number = "
  //     << win_number << endl;
#endif
  viewHasChanged[win_number]=TRUE;  // this will be noticed by protectedGetMenuItem

  xShift[win_number] += dx/magnificationFactor[win_number];
  yShift[win_number] += dy/magnificationFactor[win_number];
  zShift[win_number] += dz/magnificationFactor[win_number];
  dtx[win_number] += dThetaX*180./*/magnificationFactor[win_number]*/; // convert to degress
  dty[win_number] += dThetaY*180./*/magnificationFactor[win_number]*/;
  dtz[win_number] += dThetaZ*180./*/magnificationFactor[win_number]*/;
  
  magnificationFactor[win_number] *= magnify;
  if( magnify!=1. )
  {
    setProjection(win_number);
  }

  setRotationTransformation(win_number);
}


// static const int maximumNumberOfTextStrings=25;
// static int textIsOn[maximumNumberOfTextStrings];

// AP: This function is NOT called by anyone!!! REMOVE!!!
void 
GL_GraphicsInterface::
setModelViewMatrix()
// =================================================================================
// /Description:
//   sets the model view to the according to the current global bounds
// /Notes: this was basically copped from display, so there may be extraneous things...?
// =================================================================================
{
#ifdef USE_POWERWALL
  //cout << "--GL_GraphicsInterface::setModelViewMatrix  called,"
  //     << " current window =" << currentWindow <<endl;
#endif
}


static void 
display(GL_GraphicsInterface *giPointer, const int & win_number)
// =================================================================================
// /Description:
//    This function is called as a call-back (from mogl) to display the screen.
// =================================================================================
{
  assert(giPointer!=NULL);
  giPointer->display(win_number);
}


GL_GraphicsInterface::
GL_GraphicsInterface( const aString & windowTitle )
//=====================================================================================
// /Description:
//   Default constructor; the default constructor will open a command and a graphics window.
// /windowTitle (input): Title to appear on the graphics window.
// /Author: WDH
//
//=====================================================================================
{
  graphicsWindowIsOpen=false; 
  constructor();
  defaultWindowTitle=windowTitle;   // remember this 
  createWindow(windowTitle);
}  

GL_GraphicsInterface::
GL_GraphicsInterface(int & argc, 
                     char *argv[], 
                     const aString & windowTitle /* = nullString */ ) : GenericGraphicsInterface(argc,argv)
//=====================================================================================
// /Description:
//   This constructor takes the argc and argv from the main program -- The
//   window manager will strip off any parameters that it recognizes such as the
//   size of the window. See the Motif manual for proper syntax.
//
// /argc (input/output): The argument count to main.
// /argv (input/output): The arguments to main. This function will recognise the command line
//  options "-noplot", "-nopause", "-abortOnEnd", "-nodirect", "-nographics".
// /windowTitle (input): Title to appear on the window
//
//  /Author: WDH
//=====================================================================================
{
  constructor();

  int initialize=true;
  int localIgnorePause=false;
  int abortOnEnd=false;
  
  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="noplot" )
        initialize=false;
      else if( line=="-nopause" || line=="nopause" )
        localIgnorePause=true;
      else if( line=="-abortOnEnd" || line=="abortOnEnd" )
        abortOnEnd=true;
      else if( line=="-nodirect" || line=="nodirect" )
      {
        preferDirectRendering=false;
        printF("GL_GraphicsInterface: preferDirectRendering=false\n");
      }
      else if( line=="-nographics" )
      {
        turnOffGraphics();
      }
#ifndef NO_APP
      else if( line=="-readCollective" )
      {
	printF("***GL_GI: Set DataBase READ mode to collective ***\n");
	GenericDataBase::setParallelReadMode(GenericDataBase::collectiveIO);
      }
      else if( line=="-writeCollective" )
      {
	printF("***GL_GI: Set DataBase WRITE mode to collective ***\n");
	GenericDataBase::setParallelWriteMode(GenericDataBase::collectiveIO);
      }
      else if( line=="-multipleFileIO" )
      {
	printF("***GL_GI: Set DataBase READ/WRITE modes to multipleFileIO ***\n");
	GenericDataBase::setParallelReadMode(GenericDataBase::multipleFileIO);
	GenericDataBase::setParallelWriteMode(GenericDataBase::multipleFileIO);
      }
#endif
      
    }
  }

  
  setIgnorePause(localIgnorePause);
  abortIfCommandFileEnds(abortOnEnd);
  
  if( initialize )
    createWindow(windowTitle,argc,argv);
}

GL_GraphicsInterface::
GL_GraphicsInterface( const bool initialize, const aString & windowTitle /* = nullString */)
//-------------------------------------------------------------------------------------
// /Description:
//    This constructor will only open windows (command and graphics) if the the argument 
//    initialize=TRUE. To create the windows later, call createWindow().
//
// /initialize (input): If TRUE then a command and a graphics window will be created. 
//         If false no window will be created and you will have to call 
//         {\ff createWindow} to make the windows later.
// /windowTitle (input): Title to appear on the window.
//
// /Author: WDH \& AP.  
//-------------------------------------------------------------------------------------
{
  graphicsWindowIsOpen=false; 
  constructor();
  defaultWindowTitle=windowTitle;   // remember this 
  if( initialize )
    createWindow(windowTitle);  // split this routine into two. One for the command window and 
// another for the graphics. The graphics window should only be opened if initialize==TRUE
}  

//\begin{>GL_GraphicsInterfaceInclude.tex}{\subsection{createWindow}} 
int GL_GraphicsInterface::
createWindow(const aString & windowTitle /* = nullString */,
	     int argc /* =0 */, 
	     char *argv[] /* = NULL */ ) 
//----------------------------------------------------------------------
// /Description:
//   On the first call (usually made by the constructor), open a command and a graphics window.
//   On subsequent calls, open another graphics window.
// /windowTitle (input): Title to appear on the graphics window
// /argc (input/output): The argument count to main.
// /argv (input/output): The arguments to main.
//
//  /Return Value: The number of the graphics window that was created. The graphics windows are 
//    numbered 0,1,2,... This number needs to be passed to {\ff setCurrentWindow}, for example. The 
//    window number is also used when typing viewing commands on the command line, such as {\bf x+r:0}.
//
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  char winExt[20];
// moglGetNWindows hold the current number of windows, which equals the window-number we are about to create 
  sprintf(winExt,":%i", moglGetNWindows()); 

  aString actualWindowTitle;
  if( windowTitle==nullString )
  {
    if( defaultWindowTitle==nullString )
      actualWindowTitle = ((aString) "Your Slogan Here")+winExt; 
    else
      actualWindowTitle = defaultWindowTitle+winExt;
  }
  else
  {
    defaultWindowTitle=windowTitle;
    actualWindowTitle = windowTitle+winExt;
  }
  
  interactiveGraphicsIsOn=true; // indicates that the window has been opened (true on all processors)

#ifndef NO_APP
  if( Communication_Manager::localProcessNumber()!=getProcessorForGraphics() )
  { // only open the window if we are on processor zero *************** fix this *******
    graphicsWindowIsOpen=false;   // (only true on the processor doing graphics)
    return -1;
  }
#endif
  int win=-1;
  
  if( !graphicsWindowIsOpen ) // First call: initialize mogl and open the command window
  {
    aString cTitle;
    cTitle = "Commands";

    graphicsWindowIsOpen=TRUE;    // indicates that the window has been opened (only true on the processor doing graphics)
    
    if( argc==0 )
    {
      int argc=1;
      char *argv[] = {(char*)"plot",NULL}; 
      moglInit(argc, argv, cTitle, fileMenuItems, helpMenuItems, wProp );
    }
    else
    {
      moglInit(argc, argv, cTitle, fileMenuItems, helpMenuItems, wProp );
    }
    
    moglSetFunctions(this,::display,::resize);  // set the call backs routine for re-drawing the screen

    moglSetViewFunction(::changeView);        // set the call back routine for mouse driven view changes

  }

// first + subsequent calls: open another graphics window
  DialogData & hcd = hardCopyDialog[moglGetNWindows()];
  setupHardCopy(hcd, moglGetNWindows());

//  DialogData & mov = movieDialog[moglGetNWindows()];
//  setupMovie(mov, moglGetNWindows());

  PullDownMenu & om = optionMenu[moglGetNWindows()];

  win = makeGraphicsWindow(actualWindowTitle, graphicsFileMenuItems, graphicsHelpMenuItems, 
			   clippingPlaneInfo[moglGetNWindows()], viewChar[moglGetNWindows()],
			   hcd, om, wProp, preferDirectRendering );

  wTitle[win] = actualWindowTitle;
  initView(win);
  
// process all events 
  moglPollEvents();

  setCurrentWindow(win);
  
  return win;
}  

void GL_GraphicsInterface::
setRotationTransformation(int win_number)
{

  // Here are the rotations -- In order to incrementally rotate about
  // the FIXED axes we keep a rotation matrix that we pre-multiply
  // with any incremental rotation. These rotations are user requested (Xr+)

  if( dtx[win_number]!=0. )
  {
    real cx=cos(dtx[win_number]*Pi/180.), sx=sin(dtx[win_number]*Pi/180.);
#ifndef NO_APP    
    matrix[win_number](1,I4)=cx*rotationMatrix[win_number](1,I4)-sx*rotationMatrix[win_number](2,I4);
    matrix[win_number](2,I4)=sx*rotationMatrix[win_number](1,I4)+cx*rotationMatrix[win_number](2,I4);
    rotationMatrix[win_number](1,I4)=matrix[win_number](1,I4); 
    rotationMatrix[win_number](2,I4)=matrix[win_number](2,I4);
#else
    for (int i=0; i<4; i++)
    {
      matrix[win_number](1,i)=cx*rotationMatrix[win_number](1,i)-sx*rotationMatrix[win_number](2,i);
      matrix[win_number](2,i)=sx*rotationMatrix[win_number](1,i)+cx*rotationMatrix[win_number](2,i);
      rotationMatrix[win_number](1,i)=matrix[win_number](1,i); 
      rotationMatrix[win_number](2,i)=matrix[win_number](2,i);
    }
#endif
  }
  if( dty[win_number]!=0. )
  {
    real cy=cos(-dty[win_number]*Pi/180.), sy=sin(-dty[win_number]*Pi/180.);   // NOTE minus sign
#ifndef NO_APP    
    matrix[win_number](0,I4)=cy*rotationMatrix[win_number](0,I4)-sy*rotationMatrix[win_number](2,I4);
    matrix[win_number](2,I4)=sy*rotationMatrix[win_number](0,I4)+cy*rotationMatrix[win_number](2,I4);
    rotationMatrix[win_number](0,I4)=matrix[win_number](0,I4); 
    rotationMatrix[win_number](2,I4)=matrix[win_number](2,I4);
#else
    for (int i=0; i<4; i++)
    {
      matrix[win_number](0,i)=cy*rotationMatrix[win_number](0,i)-sy*rotationMatrix[win_number](2,i);
      matrix[win_number](2,i)=sy*rotationMatrix[win_number](0,i)+cy*rotationMatrix[win_number](2,i);
      rotationMatrix[win_number](0,i)=matrix[win_number](0,i); 
      rotationMatrix[win_number](2,i)=matrix[win_number](2,i);
    }
#endif
  }
  if( dtz[win_number]!=0. )
  {
    real cz=cos(dtz[win_number]*Pi/180.), sz=sin(dtz[win_number]*Pi/180.);
#ifndef NO_APP    
    matrix[win_number](0,I4)=cz*rotationMatrix[win_number](0,I4)-sz*rotationMatrix[win_number](1,I4);
    matrix[win_number](1,I4)=sz*rotationMatrix[win_number](0,I4)+cz*rotationMatrix[win_number](1,I4);
    rotationMatrix[win_number](0,I4)=matrix[win_number](0,I4); 
    rotationMatrix[win_number](1,I4)=matrix[win_number](1,I4);
#else
    for (int i=0; i<4; i++)
    {
      matrix[win_number](0,i)=cz*rotationMatrix[win_number](0,i)-sz*rotationMatrix[win_number](1,i);
      matrix[win_number](1,i)=sz*rotationMatrix[win_number](0,i)+cz*rotationMatrix[win_number](1,i);
      rotationMatrix[win_number](0,i)=matrix[win_number](0,i); 
      rotationMatrix[win_number](1,i)=matrix[win_number](1,i);
    }
#endif
  }
  dtx[win_number] = dty[win_number] = dtz[win_number] = 0.;

  
}


void GL_GraphicsInterface::
display(const int & win_number) 
// =================================================================================
// /Description:
//  This function renders all display lists associated with window number win\_number. 
//  This routine should normally not be called directly by the application. It
//  is called by the display call-back function and by the routine select(). 
//
// =================================================================================
{
#ifdef USE_POWERWALL 
  //cout << "GL_GraphicsInterface::display called, win ="<<win_number<<endl;
#endif //USE_POWERWALL
  const GLubyte anchor[] = {
    0xe0, 0x07, 0xf8, 0x1f, 0xbc, 0x3d, 0x8e, 0x71, 0x8f, 0xf1, 0x87, 0xe1,
    0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xf0, 0x0f, 0x80, 0x01, 0x80, 0x01,
    0x80, 0x01, 0xc0, 0x03, 0x40, 0x02, 0xc0, 0x03};

  // the non-rotable lists are not lit
  lightsOff(win_number);   

  // printF("GL_G: display called!\n");

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(leftSide[win_number], rightSide[win_number], bottom[win_number], top[win_number]);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // glTranslatef(0.0, 0.0, -1.0);    // is this needed?

  // back ground colour
  glClearColor(backGround[win_number][0],backGround[win_number][1],
	       backGround[win_number][2],backGround[win_number][3]);
  
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLbitfield arg = GLbitfield(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(arg);

  // first disable the clipping planes so we can render text etc.
  int i;
  for( i=0; i<maximumNumberOfClippingPlanes; i++)
      glDisable(clip[win_number][i]);

#ifdef PF_DEBUG
  printF("display> NON CLIPPED\n");
#endif

// Sorry, Petri, but I had to change this a bit... /AP
// first render the labels if plotLabel[w] is true
  if (plotTheLabels[win_number])
  {
    glPushMatrix();
    glTranslate(0., 0.925*(top[win_number] - bottom[win_number])*.5, 0.);
    for( i = getTopLabelDL(win_number);
	 i<= getTopLabel3DL(win_number); i++ )
    {
      if (glIsList(i))
	glCallList(i);      // render contour display list 
    }
    glPopMatrix();

// the bottom lists need a different shift
    glPushMatrix();
    glTranslate(0., -0.925*(top[win_number] - bottom[win_number])*.5, 0.);
    for( i = getBottomLabelDL(win_number);
	 i<= getBottomLabel3DL(win_number); i++ )
    {
      if (glIsList(i))
	glCallList(i);      // render contour display list 
    }
    glPopMatrix();
  }
  
  if (plotTheColouredSquares[win_number])
  {
    i = getColouredSquaresDL(win_number);
    if (glIsList(i))
    {
      glPushMatrix();
      glTranslate(rightSide[currentWindow], bottom[currentWindow], 0.);
      glCallList(i);      // render contour display list 
      glPopMatrix();
    }
    
  }
  
  if (plotTheColourBar[win_number])
  {
    i = getColourBarDL(win_number);
    if (glIsList(i))
    {
      glPushMatrix();
      colourBar.positionInWindow(leftSide[win_number], rightSide[win_number], bottom[win_number], top[win_number]);
      // glTranslate(rightSide[win_number]-0.225, 0., 0.);
      glCallList(i);      // render contour display list 
      glPopMatrix();
    }
    
  }
  
// then render all other non-rotatable lists
  for( i = getFirstUserLabelDL(win_number);
       i<= getLastUserLabelDL(win_number); i++ )
  {
    if (glIsList(i))
      glCallList(i);      // render contour display list 
  }

//
// Now restore the projection matrix and start manipulating the modelview matrix
// to view the rotating display lists
//

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);

#ifdef PF_DEBUG
  checkGLError();
#endif

  // scale the global bound by the current scaling factors *wdh* 090425
  real pgBound[6];
  #define gBound(side,axis) pgBound[(side)+2*(axis)]
  for( int side=0; side<=1; side++ )
  {
    gBound(side,0)=globalBound[win_number](side,0)*GraphicsParameters::xScaleFactor;
    gBound(side,1)=globalBound[win_number](side,1)*GraphicsParameters::yScaleFactor;
    gBound(side,2)=globalBound[win_number](side,2)*GraphicsParameters::zScaleFactor;
  }

  // AP fixing thread problem
  bool globalBoundSet=max(gBound(End,0)-gBound(Start,0),
			  gBound(End,1)-gBound(Start,1),
			  gBound(End,2)-gBound(Start,2))>0.;
  
  if( !globalBoundSet )
  {
    return;
  }

//  globalBound[win_number].display(" display: **** globalBound **** ");

  gluLookAt(xEyeCoordinate,yEyeCoordinate,zEyeCoordinate,  /* eye position */
	    0.0, 0.0, 0.0,      /* the focal point is at (0,0,0) */
  	    0.0, 1.0, 0.);      /* up is in positive Y direction */

  // turn on the clipping planes
  for( i=0; i<maximumNumberOfClippingPlanes; i++)
  {
    if( clippingPlaneIsOn[win_number][i] )
    {
      glClipPlane(clip[win_number][i],clippingPlaneEquation[win_number][i]);
      glEnable(clip[win_number][i]);
    }
    else
      glDisable(clip[win_number][i]);
  }

  real objectScale0,objectScale1,objectScale2;

//
// NOTE on the MODELVIEW matrix:
// The routines glTranslate, glMultMatrix and glScale all modify the existing MODELVIEW matrix
// by POST multiplication. This means that the calls are applied to transform the object coordinates
// in reversed order from how they appear in the code, i.e., the last call (glScale) is applied first,
// and so on. Also note that the MODELVIEW matrix is set to the identity matrix at the top of this routine.
//

// AP: When zooming by changing the projection matrix through glOrtho, it is not necessary to
// make objectScale depend on the aspect ratio!
  if( keepAspectRatio[win_number] )
  {
    objectScale0=fractionOfScreen[win_number]*
      2./max((gBound(End,0)-gBound(Start,0)),
	     (gBound(End,1)-gBound(Start,1)),
	     (gBound(End,2)-gBound(Start,2)) );
    objectScale1=objectScale2=objectScale0;
  }
  else
  {
    objectScale0=fractionOfScreen[win_number]*
      2./((gBound(End,0)-gBound(Start,0)));
    objectScale1=fractionOfScreen[win_number]*
      2./((gBound(End,1)-gBound(Start,1)));
    real zDiff=gBound(End,2)-gBound(Start,2);
    if( zDiff>0. )
      objectScale2=fractionOfScreen[win_number]*2./zDiff;
    else
      objectScale2=min(objectScale0,objectScale1);
  }
  // printF(" objectScales: %e, %e, %e, keepAspectRatio=%i \n",objectScale0,objectScale1,objectScale2,keepAspectRatio);
  
// these next scale factors are used by xlabel
  windowScaleFactor[win_number][0]=objectScale0*magnificationFactor[win_number];                
  windowScaleFactor[win_number][1]=objectScale1*magnificationFactor[win_number];                 
  windowScaleFactor[win_number][2]=objectScale2*magnificationFactor[win_number];
  
  // Translate the origin according to user modifications of the view (x+, y+, z+, etc...)
  glTranslate(xShift[win_number], yShift[win_number], zShift[win_number]);   

// Center the object
  glTranslate(-(gBound(Start,0)+gBound(End,0))*.5*objectScale0,
	      -(gBound(Start,1)+gBound(End,1))*.5*objectScale1,
	      -(gBound(Start,2)+gBound(End,2))*.5*objectScale2);

  // Correction for changing rotation center
  glTranslate(-shiftCorrection[win_number][0]*objectScale0, 
	      -shiftCorrection[win_number][1]*objectScale1, 
	      -shiftCorrection[win_number][2]*objectScale2);

  if( !userDefinedRotationPoint[win_number] )
  {
    for (int axis=0; axis<3; axis++ )
      rotationCenter[win_number][axis] = 0.5*(gBound(Start,axis) + gBound(End,axis));
  }

//  setRotationTransformation(win_number);

  // *ap* Translate forwards to move rotation center
  glTranslate(rotationCenter[win_number][0]*objectScale0, 
	      rotationCenter[win_number][1]*objectScale1, 
	      rotationCenter[win_number][2]*objectScale2);

// in order to use glRotate to rotate around axes that are fixed with respect to the screen,
// we would need to rotate the rotation axes as well, i.e., (1., 0., 0.), (0., 1., 0.) 
// and (0.,0.,1.) would have to be multiplied by the rotation matrix after the
// rotation is done.
//  glRotate(xRotAngle[win_number], 1., 0., 0.);
//  glRotate(yRotAngle[win_number], 0., 1., 0.);
//  glRotate(zRotAngle[win_number], 0., 0., 1.);
  
  glMultMatrix( &rotationMatrix[win_number](0,0) ); // AP: This call sets the rotation...

  // *ap* Translate backwards to move translation center
  glTranslate(-rotationCenter[win_number][0]*objectScale0, 
	      -rotationCenter[win_number][1]*objectScale1, 
	      -rotationCenter[win_number][2]*objectScale2);

  // Scale to make the object fit in a [-1,1]^3 cube
  glScale(objectScale0 , objectScale1, objectScale2 );                  


  lightsOff(win_number);
  if( plotTheAxes[win_number] && globalBoundSet )
  {
//    int numberOfDimensions=max(1,min(3,plotTheAxes[win_number]));
    int numberOfDimensions=max(1,min(3,axesDimension[win_number]));
// the following call generates the display lists with the axes
    plotAxes(globalBound[win_number], numberOfDimensions, Overture::defaultGraphicsParameters(), 
	     win_number);
  }
  else
  {
    eraseAxes(win_number);
  }

// plot the rotation point
  if (plotTheRotationPoint[win_number])
  {

// first save the current packing method
    GLint unpack;
    GLboolean lsbFirst;
    
    glGetBooleanv(GL_UNPACK_LSB_FIRST,&lsbFirst);
    glGetIntegerv(GL_UNPACK_ALIGNMENT,&unpack);

// now change the packing method to suit the anchor bitmap

    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    setColour("black");
    glRasterPos3f(rotationCenter[win_number][0], rotationCenter[win_number][1], 
		  rotationCenter[win_number][2]);
    glBitmap(16, 16, 8.0, 8.0, 0.0, 0.0, anchor);


// reset the packing method *wdh* 020617
    glPixelStorei(GL_UNPACK_LSB_FIRST, lsbFirst);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
  }
  

  // initialize the name stack
  glInitNames(); 
#ifdef PF_DEBUG
  checkGLError();
#endif

  // render all the used unlit lists
#ifdef PF_DEBUG
  printF("display> UNLIT\n"); 
#endif
  lightsOff(win_number);
  for( i=getFirstRotableDL(win_number); i<=getLastRotableDL(win_number); i++ ) 
  {
    if( glIsList(i) && !(plotInfo[win_number](i) & lightDL) && (plotInfo[win_number](i) & plotDL))
    {
#ifdef PF_DEBUG
      printF("<list %i> ",i); fflush(NULL); //debug **pf
#endif
      glCallList(i);      // render display lists 
    } 
#ifdef PF_DEBUG
    checkGLError();
#endif
  }

// set the lights
#ifdef PF_DEBUG
  printF("display> LIT\n");
#endif

// draw all lit lists
  if (lighting[win_number]) 
    lightsOn(win_number);

// AP: Experimenting with conditional displaying during rotation
  if( simplifyPlotWhenRotating && moglRotationKeysPressed(win_number) )
  {
// only draw lists tagged as interactively displayable
    for( i=getFirstRotableDL(win_number); i<=getLastRotableDL(win_number); i++ )
    {
      if( glIsList(i) && (plotInfo[win_number](i) & lightDL) && (plotInfo[win_number](i) & plotDL)
	  && (plotInfo[win_number](i) & interactiveDL))
      {
	glCallList(i);      // render display lists 
      } // end if 
    } //end for i
  }
  else
  {
// render all the used lit lists
    for( i=getFirstRotableDL(win_number); i<=getLastRotableDL(win_number); i++ )
    {
      if( glIsList(i) && (plotInfo[win_number](i) & lightDL) && (plotInfo[win_number](i) & plotDL))
      {
#ifdef PF_DEBUG
	printF("<list %i> ",i); fflush(NULL); //debug **pf
#endif
	glCallList(i);      // render display lists 
      } // end if 
#ifdef PF_DEBUG
      checkGLError();
#endif
    } //end for i
  }
  
  if( TRUE || saveTransformationInfo )
  {
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix[win_number]);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix[win_number]);
    glGetIntegerv(GL_VIEWPORT, viewPort[win_number]);
  }

  // turn off the clipping planes so that the rubber-band zoom box will show
  for( i=0; i<maximumNumberOfClippingPlanes; i++)
  {
    glDisable(clip[win_number][i]);
  }

//
// Restore the polygon attributes of the GL state variables
//
//  glPopAttrib();
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setCurrentWindow}} 
void GL_GraphicsInterface::
setCurrentWindow(const int & w)
//-------------------------------------------------------
// /Description:
// Set the active graphics window to `w'. Subsequent plots will appear in this window.
//
// /w (input) : the number of the window to activate
//
// /Return value: none.
//
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//-------------------------------------------------------
//
// update user defined buttons and menus:
// buttons and menus in inactive windows should be grayed out,
// while buttons and menus in the active window should be made active.
{
// add some checks here to make sure w is ok
  if (moglMakeCurrent(w))
  {
    if (currentWindow >=0 && currentWindow < moglGetNWindows())
    {
      moglSetTitle(currentWindow, wTitle[currentWindow]);
// make any user defined buttons or menus insensitive
      moglSetSensitive(currentWindow, 0);
    }
    
    aString activeTitle = "*** " + wTitle[w] + " ***";
    moglSetTitle(w, activeTitle);
// make any existing user defined buttons or menu sensitive
    moglSetSensitive(w, 1);
    currentWindow = w; 
  }
  else
    printF("setCurrentWindow failed for win=%i\n", w);
  
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getCurrentWindow}} 
int GL_GraphicsInterface::
getCurrentWindow()
//-------------------------------------------------------
// /Description:
// Return the number of the active graphics window.
//
// /Return value: The number of the active graphics window.
//
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//-------------------------------------------------------
{ 
  return currentWindow; 
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{displayHelp}} 
bool GL_GraphicsInterface::
displayHelp( const aString & topic )
//
// /Description:
// display help on a topic that appears in the "help" pulldown menu
// /topic (input) : the topic that help is requested for.
// /Return value: TRUE if help was found for the topic, false if no help found
// /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
                   //01234567890123456789
#ifndef NO_APP
  if (topic(0,8) == "PlotStuff")
#else
  if (topic.substr(0,9) == "PlotStuff")
#endif
  {
    if (helpOverture("PS", "PlotStuff") != 0)
    {
      printF("------------------------------Help PlotStuff-----------------------------------\n"
	     " See the PlotStuff documentation, available from the Overture home page. \n"
	     "-------------------------------------------------------------------------------\n");
    }
  }
#ifndef NO_APP
  else if( topic(0,14)=="mouse functions" )
#else
  else if( topic.substr(0,15)=="mouse functions" )
#endif
                       //012345678901234567890123456789

  {
    if( helpOverture("PS","mouse button!translate, rotate and zoom") !=0 )
    {
      printF("---------------------Help on mouse functions ----------------------------\n"
	     " right button        : click to see popup menu  \n"
	     " left  button        : click (and drag) for picking a point (or a region)\n"
	     "                       or selecting an object.\n"
	     " middle button       : click and drag for a rubber band zoom \n"
	     " <SHIFT> left        : click and drag to translate the view \n"
	     " <SHIFT> middle      : click and drag to rotate the view \n"
	     " <SHIFT> right       : click and drag horizontally to rotate in the plane, \n"
	     " <SHIFT> right       : click and drag vertically to zoom in or out\n"
//  	     " <CTRL>  left        : click (and drag) for picking a point (or a region)\n"
//  	     "                       or selecting an object.\n"
	     "-------------------------------------------------------------------------\n");
    }
    
  }
#ifndef NO_APP
  else if( topic(0,13)=="window buttons" )
#else
  else if( topic.substr(0,14)=="window buttons" )
#endif
                       //012345678901234567890123456789
  {
    if( helpOverture("PS","bigger,smaller,clear,reset") !=0 )
    {
      printF("--------------------------------Help on window buttons --------------------------------------\n"
	     " The rotations and translation functions operate on x-y-z axes that are \n"
	     " fixed to the screen. The x-axis is horizontal on the screen the y-axis is vertical \n"
	     " and the z-axis is out of the screen. \n"
	     "        \n"
	     " Any window button can also be executed by typing the command (e.g. `x+r') on the command line  \n"
	     "        \n"
	     " x+r      : rotate about the x-axis in a positive sense \n"
	     "            type `x+r angle' on the comamd line to rotate angle degrees \n"
	     " x-r      : rotate about the x-axis in a negative sense \n"
	     "            type `x-r angle' on the comamd line to rotate angle degrees \n"
	     " y+r, y-r, z+r, z-r : rotate about the y or z-axis \n"
	     " x+       : shift the plotted objects to the right by a fraction of the screen width \n"
	     "            type `x+ value' to shift right by an amount `value', a fraction of the screen width\n"
	     " x-       : shift the plotted objects to the left. \n"
	     " y+, y-, z+, z- : shift up or down, forward or back. \n"
	     " bigger   : magnify the window by the factor 1.25 \n"
// not yet          "          type `bigger factor' on the command line to magnify by a different factor\n"
	     " smaller  : decrease the size of the window by the factor 1/1.25 \n"
	     " clear all: erase all objects in the view \n"
	     " reset    : reset the view \n"
	     "--------------------------------------------------------------------------------------------\n");
    }
    
  }
  else if( topic=="help index" )
  {
    if( helpOverture("","master index") !=0 )
    {
      printF("------------------------------Help index --------------------------------------\n"
	     " See the master index, available from the Overture home page. \n"
	     "-------------------------------------------------------------------------------\n");
    }
  }
  else if( topic=="help grid" )
  {
    if( helpOverture("GR","grids") !=0 )
    {
      printF("------------------------------Help on grids -----------------------------------\n"
	     " See the grid documentation, available from the Overture home page. \n"
	     "-------------------------------------------------------------------------------\n");
    }
  }
  else if( topic=="help grid functions" )
  {
    if( helpOverture("GF","grid functions") !=0 )
    {
      printF("------------------------------Help on grid functions -----------------------------------\n"
	     " See the grid function documentation, available from the Overture home page. \n"
	     "------------------------------------------- -----------------------------------\n");
    }
  }
  else if( topic=="help operators" )
  {
    if( helpOverture("OP","operators") !=0 )
    {
      printF("------------------------------Help on operators -----------------------------------\n"
	     " See the operator documentation, available from the Overture home page. \n"
	     "------------------------------------------- -----------------------------------\n");
    }
  }
  else if( topic=="help grid generation" )
  {
    if( helpOverture("GG","grid generation") !=0 )
    {
      printF("------------------------------Help on grid generation ---------------------------------------\n"
	     " See the grid generator and the Mapping documentation, available from the Overture home page. \n"
	     "--------------------------------------------------------- -----------------------------------\n");
    }
  }
#ifndef NO_APP
  else if( topic(0,8)=="hard copy" )
#else
  else if( topic.substr(0,9)=="hard copy" )
#endif
                      //012345678901234567890123456789
  {
    if( helpOverture("PS","saving postscript") !=0 )
    {
      printF("-------------------------Help on hard copy output------------------------------\n"
	     " Normally you just use the 'save postscript' option from the 'file' menu. \n"
	     " The image is rendered in software at a default resolution of 1024x1024. \n"
	     "\n"
	     " If you are using Mesa, you can obtain a higher resolution postscript file by:\n"
	     "      1. Change the 'output resolution' in the 'file' menu (2048 is a good value).  \n"
	     "      2. Change the 'line width scale factor' in the 'options' menu (2 or 3 is good)  \n"
	     "      3. Redraw the plot. The lines may look thick but it should print properly.  \n"
	     "      4. Save the postscript file with 'save postscript' from the file menu. \n"
	     "      5. NOTE: Mesa may have to be recompiled with a higher resolution than default. \n"
	     "         See the PlotStuff documentation for how to do this.                         \n"
	     " \n"
	     "-------------------------------------------------------------------------------\n");
    }
  }
  else
    return 0;
  return 1;
}



//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{destroyWindow}} 
int GL_GraphicsInterface::
destroyWindow(int win_number)
//----------------------------------------------------------------------
// /Description:
//   Destroy one graphics window.
// /Note:
//      NOT implemented yet.
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( !graphicsWindowIsOpen )
    return 0;
  cout << "GL_GraphicsInterface::destroyWindow: this function is not implemented yet, sorry\n";
  return 0;
}  


bool GL_GraphicsInterface::
readOvertureRC()
{
  char *home = getenv("HOME");
  if (home)
  {
//    printF("HOME=`%s'\n", home);
    char fileName[200];
    sprintf(fileName, "%s/.overturerc", home);
//    printf("Filename: `%s'\n", fileName);
    FILE *overtureRC = fopen(fileName, "r");
    // if we don't find .overturec in the home directory then look for one in the Overture directory
    if( !overtureRC )
    {
      char *ov = getenv("Overture");
      sprintf(fileName, "%s/.overturerc",ov);
      overtureRC = fopen(fileName, "r");
    }
    if (overtureRC)
    {
      printF("Successfully opened %s for reading\n", fileName);
      char buffert[201];
      char keyword[201];
      char *crest, * colour;
      while (fgets(buffert, 200, overtureRC))
      {
// get the keyword from the buffert (the keyword is ended with :)
	int colon=strcspn(buffert,":"), firstAlpha=0, lastAlpha=0;
	strncpy(keyword, buffert, colon); keyword[colon]='\0';
	crest = &buffert[colon+1];
//	printF("keyword=`%s', crest=`%s'\n", keyword, crest);
	if (!strcmp(keyword, "commandwindow*width"))
	{
	  sScanF(crest, "%d", &wProp.commandWindowWidth);
	}
	else if (!strcmp(keyword, "commandwindow*height"))
	{
	  sScanF(crest, "%d", &wProp.commandWindowHeight);
	}
	else if (!strcmp(keyword, "graphicswindow*width"))
	{
	  sScanF(crest, "%d", &wProp.graphicsWindowWidth);
	}
	else if (!strcmp(keyword, "graphicswindow*height"))
	{
	  sScanF(crest, "%d", &wProp.graphicsWindowHeight);
	}
	else if (!strcmp(keyword, "foregroundcolour"))
	{
	  firstAlpha = strspn(crest, " \t");
	  colour = &crest[firstAlpha];
	  lastAlpha = strcspn(colour, " \t\n");
	  colour[lastAlpha] = '\0';
//	  printF("foregroundcolour: firstAlpha=%i, lastAlpha=%i, colour=`%s'\n", firstAlpha, lastAlpha, colour);
	  wProp.foregroundColour = colour;
	}
	else if (!strcmp(keyword, "backgroundcolour"))
	{
	  firstAlpha = strspn(crest, " \t");
	  colour = &crest[firstAlpha];
	  lastAlpha = strcspn(colour, " \t\n");
	  colour[lastAlpha] = '\0';
//	  printF("backgroundcolour: firstAlpha=%i, lastAlpha=%i, colour=`%s'\n", firstAlpha, lastAlpha, colour);
	  wProp.backgroundColour = colour;
	}
	else if( !strcmp(keyword, "showRubberBandBox") )
	{
          int value=1;
	  sScanF(crest, "%d", &value);
	  wProp.showRubberBandBox=(bool)value;
          // printF(" showRubberBandBox=%i\n",(int)wProp.showRubberBandBox);
	}
	else if( !strcmp(keyword, "showCommandHistory") )
	{
          int value=1;
	  sScanF(crest, "%d", &value);
	  wProp.showCommandHistory=(bool)value;
	}
	else if( !strcmp(keyword, "showPrompt") )
	{
          int value=1;
	  sScanF(crest, "%d", &value);
	  wProp.showPrompt=(bool)value;
	}
	else
	{
	  printF("Unknown keyword `%s' in the .overturerc file.\n", keyword);
	}
	
      } // end while...
      
      fclose(overtureRC);
    }
    else
    {
      printF("Did not find or could not open the file %s\n", fileName);
      return false;
    }
    
    
    if( false )
    {
      printF("commandWindowHeight=%i, commandWindowWidth=%i\n", wProp.commandWindowHeight, 
	     wProp.commandWindowWidth);
      printF("graphicsWindowHeight=%i, graphicsWindowWidth=%i\n", wProp.graphicsWindowHeight, 
	     wProp.graphicsWindowWidth);
      printF("foregroundColour=%s, backgroundColour=%s\n", CS wProp.foregroundColour.c_str(), 
	     CS wProp.backgroundColour.c_str());
    }
    
    return true;
  }
  return false;
}


void GL_GraphicsInterface:: 
constructor() // int & argc, char *argv[], const aString & windowTitle)
{
  int i;
  
// read this info from ${HOME}/.overturerc
  readOvertureRC();

  wProp.foregroundColour = (wProp.foregroundColour == "")? (aString) "black": wProp.foregroundColour;
  wProp.backgroundColour = (wProp.backgroundColour == "")? (aString) "white": wProp.backgroundColour;
  
  graphicsWindowIsOpen=false;
  saveTransformationInfo=false;  // if true save model, projection and viewPort info in display()
  
  xEyeCoordinate=0., yEyeCoordinate=0., zEyeCoordinate=2.0; // 5.0

  aString optionHeading = "Options";
  aString optionLabels[] = {"Axes", "Labels", "Rotation Point", "Colour Bar", "Squares", 
			    "Background Grid", "Wire frame rotation", "" };
  aString optionCommands[8];
  int optionState[8];

  for (int w=0; w<MAX_WINDOWS; w++) 
  {
    moglMakeCurrent(w);

// since display lists are not share between windows (GCs), it is ok to use the same number for 

    viewHasChanged[w]=false;

    dtx[w]=0., dty[w]=0., dtz[w]=0.;
    xShift[w]=0., yShift[w]=0., zShift[w]=0.;

    topLabelList[w] = 1; // display list number for the top label (numbers must be positive)
    topLabel1List[w] = topLabelList[w] + 1 ;
    topLabel2List[w] = topLabel1List[w] + 1;
    topLabel3List[w] = topLabel2List[w] + 1;
    bottomLabelList[w] = topLabel3List[w] + 1;
    bottomLabel1List[w] = bottomLabelList[w] + 1;
    bottomLabel2List[w] = bottomLabel1List[w] + 1;
    bottomLabel3List[w] = bottomLabel2List[w] + 1;
    colouredSquaresList[w] = bottomLabel3List[w] + 1;
    colourBarList[w] = colouredSquaresList[w] + 1;
    firstUserLabelList[w] = colourBarList[w] + 1;                        
    lastUserLabelList[w] = firstUserLabelList[w] +100;

    axesList[w] = lastUserLabelList[w] + 1;       // axes are rotated
    firstUserRotableList[w] = axesList[w] + 1;
    lastUserRotableList[w] = axesList[w]; // initially, there are no user defined lists
    
// this is the actual number that we initially have space for
    maximumNumberOfDisplayLists[w] = firstUserRotableList[w] + 10;

// lighting is off, plotting is on, and hiding is off, interactive is on by default
    plotInfo[w].resize(maximumNumberOfDisplayLists[w]);
    for (i=0; i<maximumNumberOfDisplayLists[w]; i++)
    {
      plotInfo[w](i) = plotDL | interactiveDL;
    }
    
    globalBound[w].redim(2,3);
    resetGlobalBound(w);
    magnificationFactor[w]=1.;
    magnificationIncrement[w]=1.25;
    fractionOfScreen[w]=.75; // .85 : *wdh* 010616 reset to old value for compatibility. Can change from view char.

    aspectRatio[w]=1.;
    keepAspectRatio[w]=true; // make this window dependent

    userDefinedRotationPoint[w]=false;  
    deltaX[w]=.05, deltaY[w]=.05, deltaZ[w]=.05, deltaAngle[w]=10.;

    for( i=0; i<3; i++ )
    {
      rotationCenter[w][i]=0.;
      shiftCorrection[w][i]=0.; // default correction for changing the rotation center
    }
    
    rotationMatrix[w].redim(4,4); matrix[w].redim(4,4);
    rotationMatrix[w]=0.;
    matrix[w]=0.;
    for (i=0; i<4; i++)
    {
      rotationMatrix[w](i,i)=1.;   
      matrix[w](i,i)=1.;
    }
//    defaultNear[w]=-zEyeCoordinate*1., defaultFar[w]=zEyeCoordinate*1.; // 10.0
// the fractionOfScreen < 1 extends the efficient viewing box,
// but we have to take the length of the space diagonal of the [-1,1] cube into account
    defaultNear[w]=-1.7+zEyeCoordinate, defaultFar[w]=1.7+zEyeCoordinate; // 10*zEyeCoordinate

    userDefinedRotationPoint[w]=false;

// near : negative value => plane is behind the viewer
    leftSide[w] = -1., rightSide[w] = 1., bottom[w] = -1., top[w] = 1.;
    near[w] = defaultNear[w], far[w] = defaultFar[w];

    windowScaleFactor[w][0]=windowScaleFactor[w][1]=windowScaleFactor[w][2]=1.;

// clipping plane stuff
    clippingPlaneInfo[w].maximumNumberOfClippingPlanes=maximumNumberOfClippingPlanes;
    clippingPlaneInfo[w].clippingPlaneIsOn=&(clippingPlaneIsOn[w][0]);
    clippingPlaneInfo[w].clippingPlaneEquation=&(clippingPlaneEquation[w][0][0]);

  // clipping planes (clip behind the normal)
    numberOfClippingPlanes[w]=0;          // no planes defined by default
    for( i=0; i<maximumNumberOfClippingPlanes; i++ )
      clippingPlaneIsOn[w][i]=false;

  // store the clipping plane names in a array (for convenience)
    clip[w][0] = GL_CLIP_PLANE0;
    clip[w][1] = GL_CLIP_PLANE1;
    clip[w][2] = GL_CLIP_PLANE2;
    clip[w][3] = GL_CLIP_PLANE3;
    clip[w][4] = GL_CLIP_PLANE4;
    clip[w][5] = GL_CLIP_PLANE5;

  // clipping plane 0 default values: (clips front half) 
    clippingPlaneEquation[w][0][0]=0.;   // normal(0)
    clippingPlaneEquation[w][0][1]=0.;   // normal(1)
    clippingPlaneEquation[w][0][2]=-1.;  // normal(2)
    clippingPlaneEquation[w][0][3]=0.;   // constant ( n0*x+n1*y+n2*z+c=0)
    // clipping plane 1 default values:   (clips left-front)
    clippingPlaneEquation[w][1][0]=1./SQRT(2.);
    clippingPlaneEquation[w][1][1]=0.;
    clippingPlaneEquation[w][1][2]=-1./SQRT(2.);
    clippingPlaneEquation[w][1][3]=0.;
    // clipping plane 1 default values:   (clips right-front)
    clippingPlaneEquation[w][2][0]=-1./SQRT(2.);
    clippingPlaneEquation[w][2][1]=0.;
    clippingPlaneEquation[w][2][2]=-1./SQRT(2.);
    clippingPlaneEquation[w][2][3]=0.;
    for( i=3; i<maximumNumberOfClippingPlanes; i++ )
    {
      clippingPlaneEquation[w][i][0]=0.;   
      clippingPlaneEquation[w][i][1]=0.;   
      clippingPlaneEquation[w][i][2]=-1.;  
      clippingPlaneEquation[w][i][3]=0.;   
    }

// set the view characteristics pointers
    viewChar[w].rotationCenter = rotationCenter[w];
    viewChar[w].axesOriginOption_ = &axesOriginOption[w];

    foreGroundName[w] = wProp.foregroundColour;
    backGroundName[w] = wProp.backgroundColour;

//      for( i=0; i<4; i++ )
//      {
//        backGround[w][i] = 1.;  // white background colour
//        foreGround[w][i] = 0.;  // black foreground colour
//      }
    real rgb[3];
    
    getXColour( foreGroundName[w], rgb);
    for (i=0; i<3; i++)
      foreGround[w][i] = rgb[i];
    foreGround[w][3] = 1.; // alpha value should be one

    getXColour( backGroundName[w], rgb);
    for (i=0; i<3; i++)
      backGround[w][i] = rgb[i];
    backGround[w][3] = 1.; // alpha value should be one
    
// copy pointers to the viewChar data structure
    viewChar[w].backGround = backGround[w];
    viewChar[w].foreGround = foreGround[w];

    strcpy(viewChar[w].backGroundName, (const char *)backGroundName[w].c_str());
    strcpy(viewChar[w].foreGroundName, (const char *)foreGroundName[w].c_str());

    viewChar[w].lighting_ = &lighting[w];
    viewChar[w].lightIsOn = lightIsOn[w];
    for (i=0; i<numberOfLights; i++)
    {
      viewChar[w].ambient[i] = ambient[w][i];
      viewChar[w].diffuse[i] = diffuse[w][i];
      viewChar[w].specular[i] = specular[w][i];
      viewChar[w].position[i] = position[w][i];
    }
    
    viewChar[w].globalAmbient = globalAmbient[w];
    
    viewChar[w].materialAmbient = materialAmbient[w];
    viewChar[w].materialDiffuse = materialDiffuse[w];
    viewChar[w].materialSpecular = materialSpecular[w];
    viewChar[w].materialShininess_ = &materialShininess[w];
    viewChar[w].materialScaleFactor_ = &materialScaleFactor[w];
    
    viewChar[w].lineScaleFactor_ = &lineWidthScaleFactor[w]; /* make this windows dependent */
    viewChar[w].fractionOfScreen_= &fractionOfScreen[w]; /* make this windows dependent */

    // We can change the default "home" view
    for( i=0; i<14; i++ )
      homeViewParameters[w][i]=-1.;

    // here are default values for lights

    for( i=0; i<numberOfLights; i++ )
    {
      lightIsOn[w][i] = i<2 ? 1 : 0;  // which lights are on?

      ambient[w][i][0] = 0.1;
      ambient[w][i][1] = 0.1;
      ambient[w][i][2] = 0.1;
      ambient[w][i][3] = 1.0;
      diffuse[w][i][0] = 0.9;
      diffuse[w][i][1] = 0.9;
      diffuse[w][i][2] = 0.9;
      diffuse[w][i][3] = 1.0;
      specular[w][i][0] =  1.0;
      specular[w][i][1] =  1.0;
      specular[w][i][2] =  1.0;
      specular[w][i][3] =  1.0;
    }
    // position[][3] : last zero -> directional light
    position[w][0][0] = 10.; position[w][0][1] = 10.; position[w][0][2] = 10.; position[w][0][3] = 0.;
    position[w][1][0] =-10.; position[w][1][1] =-10.; position[w][1][2] = 10.; position[w][1][3] = 0.;
    position[w][2][0] =-10.; position[w][2][1] = 10.; position[w][2][2] = 10.; position[w][2][3] = 0.;

    globalAmbient[w][0]=.2;  // global ambient light
    globalAmbient[w][1]=.2;
    globalAmbient[w][2]=.2;
    globalAmbient[w][3]=1.0;

    // set default material properties, these values are used for surfaces in 3D when we give them
    // different colours
    materialAmbient[w][0] = .1;
    materialAmbient[w][1] = .1; 
    materialAmbient[w][2] = .1; 
    materialAmbient[w][3] = 1.; 

    materialDiffuse[w][0] = .8;
    materialDiffuse[w][1] = .8;
    materialDiffuse[w][2] = .8;
    materialDiffuse[w][3] = 1.;

    materialSpecular[w][0]=1.;
    materialSpecular[w][1]=1.;
    materialSpecular[w][2]=1.;
    materialSpecular[w][3]=1.;
    materialShininess[w]=50.;  // in [0,128] , bigger = sharper
    materialScaleFactor[w]=1.0;  // scale factor in [0,1]

    axesOriginOption[w]=0;

    axesOrigin[w].redim(3);
    axesOrigin[w]=GenericGraphicsInterface::defaultOrigin;

    lighting[w]=TRUE;  // lighting is on initially
    plotTheAxes[w]=true;  // axes are plotted by default
    axesDimension[w] = 2; // two-dimensional axes
    plotTheRotationPoint[w] = false;
    plotTheLabels[w] = true;
    plotTheColouredSquares[w] = true; // make true by default *wdh* 080423
    plotTheColourBar[w] = true;
    plotBackGroundGrid[w] = false;

// option menu in the graphics menu
    int q=0;
    optionState[q++] = plotTheAxes[w];
    optionState[q++] = plotTheLabels[w];
    optionState[q++] = plotTheRotationPoint[w];
    optionState[q++] = plotTheColourBar[w];
    optionState[q++] = plotTheColouredSquares[w];
    optionState[q++] = plotBackGroundGrid[w];
    optionState[q++] = simplifyPlotWhenRotating;
    
    q=0;
    sPrintF(optionCommands[q++], "DISPLAY AXES:%i", w);
    sPrintF(optionCommands[q++], "DISPLAY LABELS:%i", w);
    sPrintF(optionCommands[q++], "DISPLAY ROTATION POINT:%i", w);
    sPrintF(optionCommands[q++], "DISPLAY COLOUR BAR:%i", w);
    sPrintF(optionCommands[q++], "DISPLAY SQUARES:%i", w);
    sPrintF(optionCommands[q++], "DISPLAY BACKGROUND GRID:%i", w);
    sPrintF(optionCommands[q++], "DISPLAY WIRE FRAME ROTATION:%i", w);
    optionCommands[q++] = "";
    
    optionMenu[w].setPullDownMenu(optionHeading, optionCommands, optionLabels, GI_TOGGLEBUTTON, optionState);

// label stuff
    userLabel[w]=0;              // current number of userLabels;
    labelsPlotted[w]=false;
    xAxisLabel[w]=blankString;
    yAxisLabel[w]=blankString;
    zAxisLabel[w]=blankString;
  
    rasterResolution[w]=1024;           // vertical resolution for off screen renderer
    horizontalRasterResolution[w]=1024; // horizontal resolution for off screen renderer

  // these values are used by hardCopy if no GraphicsParameters object is given
    hardCopyType[w] = GraphicsParameters::postScript; // *wdh* 000109 change default, GraphicsParameters::ppm;
    outputFormat[w] = GraphicsParameters::colour8Bit;

    hardCopyFile[w] = "hardcopy.ps";   // *wdh* "hardcopy.ppm";
  
//     movieBaseName[w] = "movie";
//     saveMovie[w] = true;
//     numberOfMovieFrames[w] = 10;
//     movieFirstFrame[w] = 0;
//     movieDxRot[w] = 0.03;
//     movieDyRot[w] = 0.03;
//     movieDzRot[w] = 0.03;
//     movieDxTrans[w] = 0.0;
//     movieDyTrans[w] = 0.0;
//     movieDzTrans[w] = 0.0;
//     movieRelZoom[w] = 1.0;

    plotBackGroundGrid[w] = 0; // no background grid by default

    lineWidthScaleFactor[w] = 1.0; // unit line width
    
  } // end for w...
  
// we always start with window 0
  currentWindow = 0;

// initially, the GUI stack is empty
  currentGUI = NULL;

#ifndef NO_APP
  I4=Index(0,4);
#endif 
/* ----
  if( Communication_Manager::localProcessNumber()!=0 )
  { // only open the window if we are on processor zero *************** fix this *******
    graphicsWindowIsOpen=false; 
    return;
  }
  graphicsWindowIsOpen=TRUE;    // indicates that the window has been opened
--- */

  readFile=NULL;
  saveFile=NULL;

//
// NOTE: When adding a new pull-down menu, be sure to increase the size of the appropriate array of aStrings
//


  fileMenuItems = new aString[12];
  fileMenuItems[ 0] = "read command file";
  fileMenuItems[ 1] = "log commands to file";
  fileMenuItems[ 2] = "pause";
  fileMenuItems[ 3] = "new window";
  fileMenuItems[ 4] = "figure";
  fileMenuItems[ 5] = "turn off parser";
  fileMenuItems[ 6] = "turn on parser";
  fileMenuItems[ 7] = "turn on graphics";
  fileMenuItems[ 8] = "turn off graphics";
  fileMenuItems[ 9] = "set home";  // put this here for now 
  fileMenuItems[10] = "abort";
  fileMenuItems[11] = "";
  
  graphicsFileMenuItems = new aString[1];
// AP "activate" used to be here, but it doesn't work anymore due to changes in getMatch
  graphicsFileMenuItems[0] = "";

//  Here are the menu items that appear in the help menu
  helpMenuItems = new aString[6];
  helpMenuItems[0] = "help index";
  helpMenuItems[1] = "help grid";
  helpMenuItems[2] = "help grid functions";
  helpMenuItems[3] = "help operators";
  helpMenuItems[4] = "help grid generation";
  helpMenuItems[5] = "";
  
  graphicsHelpMenuItems = new aString[5];
  graphicsHelpMenuItems[0] = "PlotStuff";
  graphicsHelpMenuItems[1] = "mouse functions";
  graphicsHelpMenuItems[2] = "window buttons";
  graphicsHelpMenuItems[3] = "hard copy";
  graphicsHelpMenuItems[4] = "";
  
// Here is the list of all menuBar items, file menu plus help menu, for both the command and the graphics windows
// AP: write a function that appends all the above strings and puts the result into menuBarItems directly

// first count the number of entries
  int ne=0;
  for (i=0; fileMenuItems[i] != ""; i++, ne++);
  for (i=0; graphicsFileMenuItems[i] != ""; i++, ne++);
  for (i=0; helpMenuItems[i] != ""; i++, ne++);
  for (i=0; graphicsHelpMenuItems[i] != ""; i++, ne++);
  
  menuBarItems = new aString[ne+1];
// copy all the strings
  int j=0;
  for (i=0; fileMenuItems[i] != ""; i++)
    menuBarItems[j++] = fileMenuItems[i];
  for (i=0; graphicsFileMenuItems[i] != ""; i++)
    menuBarItems[j++] = graphicsFileMenuItems[i];
  for (i=0; helpMenuItems[i] != ""; i++)
    menuBarItems[j++] = helpMenuItems[i];
  for (i=0; graphicsHelpMenuItems[i] != ""; i++)
    menuBarItems[j++] = graphicsHelpMenuItems[i];
// terminate with a ""
  menuBarItems[j++] = "";
// done copying the menubaritems
  
  const aString colourNames0[numberOfColourNames+1]
    = { "BLUE",
	"GREEN",
	"RED",
	"VIOLETRED",
	"DARKTURQUOISE",
	"STEELBLUE",
	"ORANGE",
	"ORCHID",
	"NAVYBLUE",
	"SALMON",
	"yellow",
	"CORAL",
	"AQUAMARINE",
	"MEDIUMGOLDENROD",
	"DARKGREEN",
	"WHEAT",
	"SEAGREEN",
	"KHAKI",
	"MAROON",
	"SKYBLUE",
	"SLATEBLUE",
	"DARKORCHID",
	"PLUM",
	"VIOLET",
	"PINK",
	"",
    };

// first convert the background name to upper case
  aString bgName = wProp.backgroundColour;
  for( i=0; i<wProp.backgroundColour.length(); i++ )
  {
    bgName[i]=toupper(wProp.backgroundColour[i]);
  }
// copy all colours except the background colour
  int numberOfEntries=0 ;
  for( i=0; colourNames0[i]!=""; i++ )
    if (colourNames0[i] != bgName)
      numberOfEntries++;

//  printF("Background colour `%s'\n", CS bgName);
  colourNames = new aString [numberOfEntries+1];  
  for( i=0, j=0; colourNames0[i]!=""; i++)
    if (colourNames0[i] != bgName)
      colourNames[j++] = colourNames0[i];
    else
      printF("Not using the colour `%s'\n", CS colourNames0[i].c_str());
  
  
  colourNames[numberOfEntries]="";
}

GL_GraphicsInterface::
~GL_GraphicsInterface()
{
  remove( fileMenuItems );
  remove( helpMenuItems );
  remove( graphicsFileMenuItems );
  remove( graphicsHelpMenuItems );
  remove( menuBarItems );
  remove( colourNames );
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{generateNewDisplayList}} 
int GL_GraphicsInterface::
generateNewDisplayList(bool lit /* = false */, bool plotIt /* = true */, bool hideable /* = false */,
		       bool interactive /* = true */) 
// ========================================================================================
// /Description:
//   Use this function to allocate an available display list to use in the currentWindow.
//   The display lists allocated here will be rotated and scaled when buttons like x+r, "bigger", etc.
//   are chosen.
//
// /lit (input):
//    The lighting is turned OFF if lit == 0 and ON if lit != 0. This setting can be changed by calling 
//    the function setLighting after the list is allocated.
//
//  /Return Value: The number of the new display list.
//
// /Remark:
//   Lighting is OFF by default.

//  /Author: WDH \& AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  #ifndef OV_USE_GL
    return 123;
  #endif

// first look among all the allocated lists
  int i;
  for( i=getFirstUserRotableDL(currentWindow); i<getMaxNOfDL(currentWindow); i++ )
  {
    if( !glIsList(i) ) break;
  }

// do we need to allocate more lists?
  if (i>=getMaxNOfDL(currentWindow))
  {
    maximumNumberOfDisplayLists[currentWindow] += 1000;
    plotInfo[currentWindow].resize(maximumNumberOfDisplayLists[currentWindow]);
#ifndef NO_APP
    plotInfo[currentWindow](Range(maximumNumberOfDisplayLists[currentWindow]-1000,
                                  maximumNumberOfDisplayLists[currentWindow]-1))=plotDL | interactiveDL; // *ap*
#else
    for (int q=0; q<1000; q++)
      plotInfo[currentWindow](maximumNumberOfDisplayLists[currentWindow]-1000+q) = plotDL | interactiveDL; // *ap*
#endif
    
    printF("INFO: Allocating more display lists in generateNewDisplayList. New max=%i\n",
	   maximumNumberOfDisplayLists[currentWindow]);
  }
  
  plotInfo[currentWindow](i) = 0;
  if (lit)
    plotInfo[currentWindow](i) |= lightDL;
  if (plotIt)
    plotInfo[currentWindow](i) |= plotDL;
  if (hideable)
    plotInfo[currentWindow](i) |= hideableDL;
  if (interactive)
    plotInfo[currentWindow](i) |= interactiveDL;
  
  lastUserRotableList[currentWindow] = max(i, lastUserRotableList[currentWindow]);

  glNewList(i, GL_COMPILE); // create an empty display list with number i. Then consecutive calls
  glEndList();              // to this routine will deliver different list numbers.

  if( recordDisplayLists!=NULL )
  {
    // save this display list number if we are recording them
    IntegerArray & record = *recordDisplayLists;
#ifndef NO_APP
    if( numberRecorded > record.getBound(0) )
      record.resize(Range(record.getBase(),record.getBound(0)+10));
#else
    if( numberRecorded > record.getBound(0) )
    {
      record.resize(record.getLength(0)+10); // ArraySimple always has base 0
    }    
#endif
    record(numberRecorded)=i;
    numberRecorded++;
  }
  return i;
    
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getNewLabelList}} 
int GL_GraphicsInterface::
getNewLabelList(int win /* = -1 */ )
// ========================================================================================
// /Description:
//   Use this function to get an unused list for labels (non-rotatable).
//
//  /Return Value: The number of the new display list.
//
//  /Author: WDH 
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
// first look among all the allocated lists
  if( win==-1 )
    win=currentWindow;
  
  int i, list=-1;
  for( i=getFirstUserLabelDL(win); i<getLastUserLabelDL(win); i++ )
  {
    if( !glIsList(i) ) 
    {
      list=i;
      break;
    }
  }
  if( list==-1 )
  {
    printF("GL_GraphicsInterface::ERROR:getNewLabelList: Unable to find an unused display list number!\n");
    list=getFirstUserLabelDL(win);
  }
  return list;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{deleteList}} 
void GL_GraphicsInterface::
deleteList(int dList)
// ========================================================================================
// /Description:
//   Delete one display list in the current window
//
//  /Return Value: None.
//
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  if (glIsList(dList))
    glDeleteLists(dList, 1);
}



//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getGlobalBound}} 
RealArray GL_GraphicsInterface:: 
getGlobalBound() const
//----------------------------------------------------------------------
// /Description:
//   return a copy of the global bounds in the current graphics window.
// /return value: globalBound(0:1,0:2): current global bounds.
//
// /author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  return globalBound[currentWindow];
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{hardCopy (save a Postscript File)}} 
int GL_GraphicsInterface::
hardCopy(const aString & fileName, /* =nullString */
         GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	 int win_number /* =-1 */)
//----------------------------------------------------------------------
// /Description:
//    This routine saves the contents of one graphics window in hard-copy form.
//    If off-screen rendering is available (MESA) then use it, unless the hardCopyType is
//    set to GraphicsParameters::postScriptRaster.
//  /fileName (input): Optional name for the file to save the plot in. If no name is given then
//     the user is prompted for a name.
//  /hardCopyType (input): GraphicsParameters::postScriptRaster, or GraphicsParameters::postScript.
//  /win\_number (input): The number of the window to save. If that argument is omitted, the 
//  contents of the current window is saved .
//  /Return Values: 1: unable to open the file.
//
//  /Author: WDH \& AP
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  #ifdef USE_PPP
  if( Communication_Manager::localProcessNumber()!=processorForGraphics )
    return 0;
  #endif

  if (win_number==-1)
    win_number=currentWindow;
  
  int oldCurrentWindow = currentWindow;
  
  GraphicsParameters::HardCopyType localHardCopyType;
  if( parameters.isDefault() )
    localHardCopyType=hardCopyType[win_number];
  else
    localHardCopyType=parameters.hardCopyType;

// get a file name if one isn't supplied...
  aString localFileName;
  if( fileName!="" && fileName!=" " )
    localFileName=fileName;
  else
  {
    if( localHardCopyType==GraphicsParameters::postScript ||
	localHardCopyType==GraphicsParameters::encapsulatedPostScript ||
	localHardCopyType==GraphicsParameters::postScriptRaster )
      inputFileName(localFileName,"Enter name of postscript file");
    else
      inputFileName(localFileName,"Enter name of ppm file");
  }
  if( localFileName=="" || localFileName==blankString )
    return 0;

  // set the current window to be win_number during the hardcopy
  moglMakeCurrent(win_number);
  
  // save the screen as a bitmap image
  if( hardCopyRenderingType==GenericGraphicsInterface::offScreenRender )
  {
    offScreenRender((const char *)localFileName.c_str(),parameters);
  }
  else
  {
    // read the frame buffer, resolution determined by the window size.
    GLint x=0,y=0;  // lower corner of image
    GLsizei width, height;
    moglGetWindowSize(width,height,win_number);

    float *xBuffer = new float [width*height*3+1000];
    for( int n=0; n<width*height*3+1000; n++ )
      xBuffer[n]=0.;
  
    printF("hardCopy:Reading the frame buffer, resolution determined by the window size..\n");
  
    glReadBuffer(GL_FRONT);
    glReadPixels( x,y,width,height,GL_RGB,GL_FLOAT,xBuffer);  // read the frame buffer
    glReadBuffer(GL_BACK);
 
    int rgbType=0;
    saveRasterInAFile(localFileName,xBuffer,width,height,rgbType,parameters);
    delete [] xBuffer;
  }
  
  int len = localFileName.length();
  if( len>4 && localFileName(len-4,len-1)==".pdf" )
  {
    // printF("Convert file [%s] to pdf using `convert'\n",(const char*)localFileName);
    printF("Convert file [%s] to pdf using `ps2pdf -dEPSCrop'\n",(const char*)localFileName);

    aString prefix = localFileName(0,len-5);
    aString buff;
    // printF("File name prefix=[%s]\n",(const char*)prefix);
    system(sPrintF(buff,"mv %s overtureTempFile.ps",(const char*)localFileName));
    // system(sPrintF(buff,"convert overtureTempFile.ps %s.pdf",(const char*)prefix,(const char*)prefix));
    system(sPrintF(buff,"ps2pdf -dEPSCrop overtureTempFile.ps %s.pdf",(const char*)prefix,(const char*)prefix));
    system("rm overtureTempFile.ps");
  }
  


  moglMakeCurrent(oldCurrentWindow);
  return 0;

}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{outputString}} 
void GL_GraphicsInterface::
outputString(const aString & message, int messageLevel /* =2 */ )
//----------------------------------------------------------------------
// /Description:
//   Output a string in the prompt sub-window in the command window.
//   If the echo file is open, also output the string in that file.
// /message (input): the string to be output.
// /messageLevel (input) : output the string if messageLevel is less than or equal
//    to the current value for infoLevel. Values for infoLevel are 0=expert, 1=intermediate, 2=novice.
//
// /Return Value: none.
//
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( messageLevel<=infoLevel )
  {
    if( echoFile )
      fPrintF(echoFile,"%s\n",(const char*)message.c_str());

    if (graphicsWindowIsOpen)
      moglSetPrompt((char *)(const char *)message.c_str());

    // *wdh* always output the message to standard out.
    printF("%s\n",(const char*)message.c_str());
  }
  
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{erase}} 
void GL_GraphicsInterface::
erase()
//----------------------------------------------------------------------------------
//  /Description:
//    Erase the current graphics window. Shorthand for erase(getCurrentWindow(), false);
//
//  /Author: AP
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  erase(currentWindow, false);
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{erase (win\_number)}} 
void GL_GraphicsInterface::
erase(const int win_number, bool forceDelete /* = false */)
//----------------------------------------------------------------------------------
//  /Description:
//    Erase the contents in one graphics window.
// win\_number(input): window number
// forceDelete(input): If true, delete all display lists associated with this window. If false,
// delete the display lists that are not hidable and don't plot the hidable lists.
//
//  /Author: WDH \& AP
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( !graphicsWindowIsOpen )
    return;
  glDeleteLists(getFirstDL(win_number),getLastFixedDL(win_number));
  int i;
  for (i=getFirstUserRotableDL(win_number); i<= getLastUserRotableDL(win_number); i++)
  {
// only delete lists that are NOT hideable
    if (forceDelete || !(plotInfo[win_number](i) & hideableDL))
    {
//#define AP_DEBUG

#ifdef AP_DEBUG
      printF("Deleting display list #%i\n", i);
#endif
      glDeleteLists(i,1);
    }
    else
    {
#ifdef AP_DEBUG
      printF("Hiding display list #%i\n", i);
#endif
#undef AP_DEBUG
      setPlotDL(i, false);
    }
    
  }

  userLabel[win_number]     = 0;

  resetGlobalBound(win_number);
  
  moglPostDisplay(win_number); 
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{erase (IntegerArray)}} 
void GL_GraphicsInterface::
erase(const IntegerArray & displayList)
//----------------------------------------------------------------------------------
//  /Description:
//    Erase some display lists in the current graphics window.
//
// /displayList (input) : an array of display lists to delete, all values should be non-negative.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( !graphicsWindowIsOpen )
    return;

#ifndef NO_APP
  for( int i=displayList.getBase(0); i<=displayList.getBound(0); i++ )
#else
  for( int i=0; i<=displayList.getBound(0); i++ )
#endif
  {
    glDeleteLists(displayList(i),1);
  }
  
  moglPostDisplay(currentWindow); 
}

//
// Input a filename through a file selection dialog
//
//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{inputFileName}} 
void GL_GraphicsInterface::
inputFileName(aString & fileName, const aString & prompt, const aString &extension /* =nullString */)
//----------------------------------------------------------------------
// /Description:
//   Open a Motif file selection dialog window and prompt for a file name. 
// /fileName (output): an aString with the selected file name.
// /prompt (input): an aString with the prompt that will be displayed in the text 
//    sub-window of the command window.
// /extension (input): an aString with the extension of the files that will be displayed when 
//    the file selection dialog is opened.
//
// /Return Value: none.
//
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  // show a null menu and take a response from the file selection dialog
  aString menu[] = { "!Enter filename",
		    "" };
  bool openDialog=graphicsWindowIsOpen && !readFile; // *wdh* do not open dialog if reading a command file
  if( openDialog )
  {
    moglOpenFileSB(CS extension.c_str());
  }
  getMenuItem(menu, fileName, prompt);
  if( openDialog )
  {
    moglCloseFileSB();
  }
//  cout << "inputFileName=" << answer << endl;
}

//
// Input a string after displaying an optional prompt
//
//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{inputString}} 
void GL_GraphicsInterface::
inputString(aString & answer, const aString & prompt)
//----------------------------------------------------------------------
// /Description:
//   Output a prompt and wait for an answer.
// /answer (output): a aString with the answer.
// /prompt (input): a aString with the prompt that will be displayed in the text 
//    sub-window of the command window (if the GUI is active).
//
// /Return Value: none.
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  // show a null menu and take a response from the command window
  aString menu[] = { "!Enter string",
		    "" };
  getMenuItem(menu,answer,prompt);  
//   getAnswer(answer,prompt);
//  cout << "inputString=" << answer << endl;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{redraw}} 
void GL_GraphicsInterface::
redraw(bool immediate)
// --------------------------------------------------------
// /Description:
// Redraw all graphics display lists in the current window.
// /immediate(input): If true, force an immediate redraw. Otherwise, post a redraw event, in which
// case the window will be redrawn next time the application asks for a user input.
//\end{GL_GraphicsInterfaceInclude.tex} 
//  /Author: WDH \& AP
// --------------------------------------------------------
{
  if( !graphicsWindowIsOpen )
    return;
  if( immediate )
    moglDisplay(currentWindow);    
  else
    moglPostDisplay(currentWindow);

}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{resetGlobalBound}} 
void GL_GraphicsInterface::
resetGlobalBound(const int win_number)
//----------------------------------------------------------------------
// /Description:
//   Reset the global bounds to represent no bounds at all.
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
#ifndef NO_APP
  Range Axes(0,2);
  globalBound[win_number](Start,Axes)= REAL_MAX*.1;
  globalBound[win_number](End  ,Axes)=-REAL_MAX*.1;
#else
  for (int ax=0; ax<3; ax++)
  {
    globalBound[win_number](Start,ax)= REAL_MAX*.1;
    globalBound[win_number](End  ,ax)=-REAL_MAX*.1;
  }
#endif
  // globalBound.display("resetGlobalBound: globalBound");
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setGlobalBound}} 
void GL_GraphicsInterface::
setGlobalBound(const RealArray & xBound)
//----------------------------------------------------------------------
// /Description:
//   Set the global bounds for plotting. These values will only increase the 
// size of the current bounds.
// /xBound(0:1,0:2) (input) : global bounds should be at least this large.
//
//\end{GL_GraphicsInterfaceInclude.tex} 
// AP: Who calls this routine???
// AP: This routine changes the bounds in window "currentWindow"
//----------------------------------------------------------------------
{
#ifndef NO_APP
  Range Axes(0,2);
  globalBound[currentWindow](Start,Axes) = min(globalBound[currentWindow](Start,Axes),xBound(Start,Axes)); // elementwise min
  globalBound[currentWindow](End  ,Axes) = max(globalBound[currentWindow](End  ,Axes),xBound(End  ,Axes));
#else
  int ax;
  for (ax=0; ax<3; ax++)
  {
    globalBound[currentWindow](Start,ax) = min(globalBound[currentWindow](Start,ax),xBound(Start,ax));
    globalBound[currentWindow](End  ,ax) = max(globalBound[currentWindow](End  ,ax),xBound(End  ,ax));
  }
#endif
  // determine scale factors for xLabel:
  //   real objectScale=fractionOfScreen[currentWindow]*
  //     2./max((globalBound[currentWindow](End,0)-globalBound[currentWindow](Start,0)) ,
  // 	   (globalBound[currentWindow](End,1)-globalBound[currentWindow](Start,1)) ,
  // 	   (globalBound[currentWindow](End,2)-globalBound[currentWindow](Start,2)) );

  // kkc a single point has a very small bounding box, objectScale->INF in this case
  //     if the bounds are all 0, make the objectScale 1
  real objectScale=1.;
#ifndef NO_APP
  if ( max(globalBound[currentWindow](End,Axes)-globalBound[currentWindow](Start,Axes))>REAL_MIN )
#else
  real maxBoundDiff=0;
  for (ax=0; ax<3; ax++)
    maxBoundDiff = max(maxBoundDiff, globalBound[currentWindow](End,ax)-globalBound[currentWindow](Start,ax));
  
  if ( maxBoundDiff>REAL_MIN )
#endif
    {
      objectScale=fractionOfScreen[currentWindow]*
	2./max((globalBound[currentWindow](End,0)-globalBound[currentWindow](Start,0)) ,
	       (globalBound[currentWindow](End,1)-globalBound[currentWindow](Start,1)) ,
	       (globalBound[currentWindow](End,2)-globalBound[currentWindow](Start,2)) );
    }

  // these next scale factors are used by xlabel
  windowScaleFactor[currentWindow][0]=objectScale*magnificationFactor[currentWindow];                
  windowScaleFactor[currentWindow][1]=objectScale*magnificationFactor[currentWindow];                 
  windowScaleFactor[currentWindow][2]=objectScale*magnificationFactor[currentWindow];
  // globalBound.display("setGlobalBound: globalBound");
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setKeepAspectRatio}} 
int GL_GraphicsInterface:: 
setKeepAspectRatio( bool trueOrFalse /* =true */ )
//----------------------------------------------------------------------------------
//  /Description:
//     If "true", keep the aspect ratio of plots.
//  /Author: WDH \& AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  keepAspectRatio[currentWindow]=trueOrFalse;
  return 0;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getWindowShape}} 
void GL_GraphicsInterface::
getWindowShape( int window, real & leftSide_ , real & rightSide_ , real & top_ , real & bottom_) const
//----------------------------------------------------------------------------------
//  /Description:
//    Return the shape of the window.
//\end{GL_GraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  rightSide_ = rightSide[window];
  leftSide_  = leftSide[window];
  top_ = top[window];
  bottom_ = bottom[window];
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getLineWidthScaleFactor}} 
real GL_GraphicsInterface:: 
getLineWidthScaleFactor(int window /* = -1 */)
//----------------------------------------------------------------------------------
//  /Description:
//    Return the scale factor for line widths.
//\end{GL_GraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if (window == -1) window = currentWindow;
  return lineWidthScaleFactor[window];
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{displayColourBar}}
void GL_GraphicsInterface:: 
displayColourBar(const int & numberOfContourLevels,
		 RealArray & contourLevels,
		 real uMin,
		 real uMax,
  	         GraphicsParameters & parameters)
//----------------------------------------------------------------------------------
//  /Description:
// Display the colour bar.
//\end{GL_GraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  colourBar.setGraphicsInterface(this);
  colourBar.setGraphicsParameters(&parameters);
  colourBar.draw(numberOfContourLevels,contourLevels,uMin,uMax);
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{updateColourBar}} 
void GL_GraphicsInterface:: 
updateColourBar(GraphicsParameters & parameters, int window /* =0 */)
// /Description:
// update the colour bar.
//\end{GL_GraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  colourBar.setGraphicsInterface(this);
  colourBar.setGraphicsParameters(&parameters);
  colourBar.update();
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setLighting}} 
void GL_GraphicsInterface::
setLighting(int list, bool lit)
// ========================================================================================
// /Description:
//    Use this function to turn on or off lighting in a display list in the currentWindow. Note that
//    each display list can only be completely lit or unlit. If you need to display both lit and unlit
//    objects, you need to split the plotting into two display lists.
//
// /list (input):
//    The number of the existing display list. 1 $<=$ list $<$ getMaxNOfDL(currentWindow). 
//    This number is for example returned by the function generateNewDisplayList.
// /lit (input):
//    The lighting is turned OFF if lit == 0 and ON if lit $!=$ 0.
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  if (1 <= list && list < getMaxNOfDL(currentWindow))
  {
    if (lit)
      plotInfo[currentWindow](list) |= lightDL;
    else
      plotInfo[currentWindow](list) &= ~lightDL;
  }
  
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setPlotDL}} 
void GL_GraphicsInterface::
setPlotDL(int list, bool plot)
// ========================================================================================
// /Description:
//    Use this function to turn on or off plotting of a display list in the currentWindow.
//
// /list (input):
//    The number of the existing display list. getFirstUserRotableDL(currentWindow) $<=$ list $<$ 
//    getMaxNOfDL(currentWindow). The number of the display list is for example returned by the function
//    generateNewDisplayList.
// /plot (input):
//    The plotting of the display list is turned OFF if plot == false and ON if plot $==$ true .
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  if (getFirstUserRotableDL(currentWindow) <= list && list < getMaxNOfDL(currentWindow) 
      && glIsList(list))
  {
    if (plot)
      plotInfo[currentWindow](list) |= plotDL;
    else
      plotInfo[currentWindow](list) &= ~plotDL;
  }
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setInteractiveDL}} 
void GL_GraphicsInterface::
setInteractiveDL(int list, bool interactive)
// ========================================================================================
// /Description:
//    Use this function to turn on or off interactive plotting of this display list during rotations 
//    in the currentWindow.
//
// /list (input):
//    The number of the existing display list. getFirstUserRotableDL(currentWindow) $<=$ list $<$ 
//    getMaxNOfDL(currentWindow). The number of the display list is for example returned by the function
//    generateNewDisplayList.
// /interactive (input):
//    Interactive plotting of the display list is turned OFF if plot == false and ON if plot $==$ true .
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  if (getFirstUserRotableDL(currentWindow) <= list && list < getMaxNOfDL(currentWindow))
  {
    if (interactive)
      plotInfo[currentWindow](list) |= interactiveDL;
    else
      plotInfo[currentWindow](list) &= ~interactiveDL;
  }
}

void  GL_GraphicsInterface::
annotate(const aString & answer)
// for displaying user labels (defined interactively)
{
  printF("annotate: answer=%s \n",(const char *)answer.c_str());

  // answers of the form:
  //   annotate %i text %s
  //   annotate %i size %e
  //   annotate %i position %e %e
  //   annotate %i centering %i
  //   annotate %i angle %e

  int length=answer.length();
  int numberOfTokens =0;
  int startOfToken[10];
  for( int i=4; i<length-1; i++ )
  {
    if( answer[i]==' ' && answer[i+1]!=' ' )
    {
      startOfToken[numberOfTokens++]=i+1;
      if( numberOfTokens > 4 )
	break;
    }
  }
  startOfToken[numberOfTokens]=length;
    
  aString text;
  real x=0., y=0., size=.05, angle=0.;
  int centering;

  int number=-1;
  // cout << "token 0 =[" << answer(startOfToken[0],startOfToken[1]-1) << "]\n";
#ifndef NO_APP
  sScanF( answer(startOfToken[0],startOfToken[1]-1),"%i",&number ); 
#else
  sScanF( answer.substr(startOfToken[0],startOfToken[1]-startOfToken[0]),"%i",&number ); 
#endif

  if( numberOfTokens < 2 || number <0 || number>=maximumNumberOfTextStrings )
  {
    cout << "invalid annotate command, input string = " << answer << endl;
  }
  else
  {
    // cout << "token 1 =[" << answer(startOfToken[1],startOfToken[2]-1) << "]\n";
#ifndef NO_APP
    if( answer(startOfToken[1],startOfToken[1]+1)=="on" )
#else
    if( answer.substr(startOfToken[1],2)=="on" )
#endif
    {
      if( !textIsOn[number] )
      { // turn this text if it is not already on
	userLabel[currentWindow]=max(userLabel[currentWindow],number);
	textIsOn[number]=TRUE;
	printF("turn on text %i \n",number);
      }
    }
#ifndef NO_APP
    else if( answer(startOfToken[1],startOfToken[1]+2)=="off" )
#else
    else if( answer.substr(startOfToken[1],3)=="off" )
#endif
    {
      if( textIsOn[number] )
      { // turn off this text 
        if( number>=userLabel[currentWindow] )
  	  userLabel[currentWindow]--;
	textIsOn[number]=false;
	printF("turn off text %i \n",number);
      }
    }
#ifndef NO_APP
    else if( answer(startOfToken[1],startOfToken[1]+3)=="text" )
#else
    else if( answer.substr(startOfToken[1],4)=="text" )
#endif
    {
#ifndef NO_APP
      text=answer(startOfToken[2],length-1);
#else
      text=answer.substr(startOfToken[2],length-startOfToken[2]);
#endif
      printF("text %i =%s\n",number,(const char *)text.c_str());
    }
#ifndef NO_APP
    else if( answer(startOfToken[1],startOfToken[1]+3)=="size" )
#else
    else if( answer.substr(startOfToken[1],4)=="size" )
#endif
    {
#ifndef NO_APP
      sScanF( answer(startOfToken[2],length-1),"%e",&size); 
#else
      sScanF( answer.substr(startOfToken[2],length-startOfToken[2]),"%e",&size); 
#endif
    }
#ifndef NO_APP
    else if( answer(startOfToken[1],startOfToken[1]+8)=="centering" )
#else
    else if( answer.substr(startOfToken[1],9)=="centering" )
#endif
    {
      //      sScanF( answer(startOfToken[2],length-1),"%i",&centering); 
#ifndef NO_APP
      sScanF( answer(startOfToken[2],length-1),"%i",&centering); 
#else
      sScanF( answer.substr(startOfToken[2],length-startOfToken[2]),"%e",&centering); 
#endif
    }
#ifndef NO_APP
    else if( answer(startOfToken[1],startOfToken[1]+7)=="position" )
#else
    else if( answer.substr(startOfToken[1],8)=="position" )
#endif
    {
      //      sScanF( answer(startOfToken[2],length-1),"%e %e",&x,&y); 
#ifndef NO_APP
      sScanF( answer(startOfToken[2],length-1),"%e %e",&x,&y); 
#else
      sScanF( answer.substr(startOfToken[2],length-startOfToken[2]),"%e %e",&x,&y); 
#endif
    }
#ifndef NO_APP
    else if( answer(startOfToken[1],startOfToken[1]+4)=="angle" )
#else
    else if( answer.substr(startOfToken[1],5)=="angle" )
#endif
    {
      //      sScanF( answer(startOfToken[2],length-1),"%e",&angle); 
#ifndef NO_APP
      sScanF( answer(startOfToken[2],length-1),"%e",&angle); 
#else
      sScanF( answer.substr(startOfToken[2],length-startOfToken[2]),"%e",&angle); 
#endif
    }
    else
    {
      cout << "invalid annotate command(2), input string = " << answer << endl;
    }
  }

  if( textIsOn[number] )
  {
    glDeleteLists(firstUserLabelList[currentWindow]+number,1);  // clear the title
    glNewList(firstUserLabelList[currentWindow]+number,GL_COMPILE);
    setColour(textColour);  // label colour
    label(text,x,y,size,centering,angle);
    glEndList();
  }
  
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{initView}} 
void GL_GraphicsInterface::
initView(int win_number/*=-1*/)
// /Description:
// Initialize the view and rotation point to default values
// /win\_number(optional input): window number
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  if (win_number == -1)
    win_number = getCurrentWindow();
  
  userDefinedRotationPoint[win_number]=false;
  resetView(win_number);
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{resetView}} 
void GL_GraphicsInterface::
resetView(int win_number /*=-1*/)
// /Description:
// Reset the view point (but not the rotation point)
// /win\_number(optional input): window number
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  if (win_number == -1)
    win_number = getCurrentWindow();
  
// xEyeCoordinate=0., yEyeCoordinate=0., zEyeCoordinate=5.; // 5.  // position of the eye
 dtx[win_number]=0., dty[win_number]=0., dtz[win_number]=0.;
 xShift[win_number]=0., yShift[win_number]=0., zShift[win_number]=0.; // replace by actual window number
 
 magnificationFactor[win_number]=1., magnificationIncrement[win_number]=1.25;
 deltaX[win_number]=.05, deltaY[win_number]=.05, deltaZ[win_number]=.05, deltaAngle[win_number]=10.;
 rotationMatrix[win_number]=0.;
 rotationMatrix[win_number](0,0)=1.;   
 rotationMatrix[win_number](1,1)=1.;   
 rotationMatrix[win_number](2,2)=1.;   
 rotationMatrix[win_number](3,3)=1.; 
 
 near[win_number] = defaultNear[win_number];
 far[win_number]  = defaultFar[win_number];
 
 for (int i=0; i<3; i++)
   shiftCorrection[win_number][i] = 0.;

 init(win_number);

 setRotationTransformation(win_number); // AP: Is this call necessary? (i.e. does it do anything?)
}

void GL_GraphicsInterface::
resetRotationPoint(int win_number /*=-1*/)
// /Description:
// Reset the rotation point to the default value
// /win\_number(optional input): window number
// /Return value: None
//  /Author: AP
{
  int w=(win_number==-1)? getCurrentWindow(): win_number;
  
  real rotPnt[3];
  int axis;
  for (axis=0; axis<3; axis++ )
    rotPnt[axis] = 0.5*(globalBound[win_number](Start,axis) + globalBound[win_number](End,axis));
  
  setRotationCenter(rotPnt, w);
}

// AP: This function is obsolete and does nothing. 
void GL_GraphicsInterface:: 
setNormalizedCoordinates()
{
}

// AP: This function is obsolete and does nothing. 
void GL_GraphicsInterface:: 
unsetNormalizedCoordinates()
{
}


// 
//  choose a colour
//
//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{chooseAColour}} 
aString GL_GraphicsInterface::
chooseAColour()
// ========================================================================================
// /Description:
//  Choose a colour from a menu.
// /Return value: The name of the colour.
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// ========================================================================================
{
  aString answer;
  aString menu[] = 
  {"!Choose a colour",
   "no change",
   ">Special materials",
   "emerald", "jade", "obsidian", "pearl", "ruby", "turquoise", 
   "brass", "bronze", "chrome", "copper", "gold", "silver", 
   "blackPlastic", "cyanPlastic", "greenPlastic", "redPlastic", "whitePlastic", "yellowPlastic",
   "blackRubber", "cyanRubber", "greenRubber", "redRubber", "whiteRubber", "yellowRubber",
//
   "<>Basic X-colours",
   "black","white", "red", "blue", "green", "orange", "yellow",  
//
   "<>Red X-colours",
   "pink", "sienna", "firebrick", "tan", 
   "goldenrod", "mediumgoldenrod",
   "magenta", "indianred",
   "violet", "violetred", "mediumvioletred", 
   "orangered", "brown", "sandybrown", "coral", "salmon", 
//
   "<>Green X-colours",
   "khaki", "darkgreen", "darkolivegreen", "forestgreen", "limegreen",
   "mediumforestgreen", "seagreen", "mediumseagreen", "springgreen", "mediumspringgreen", 
   "aquamarine", "mediumaquamarine", "palegreen", "yellowgreen", "greenyellow",
//
   "<>Blue X-colours",
   "maroon", "plum", "orchid", "mediumorchid", "darkorchid",
   "lightblue", "mediumblue", "midnightblue", "cyan", "skyblue", "navyblue", 
   "mediumturquoise", "darkturquoise", "steelblue", "lightsteelblue", 
   "slateblue", "mediumslateblue", "darkslateblue", 
   "cadetblue", "cornflowerblue", "blueviolet", 
//
   "<>Gray X-colours",
   "wheat", "thistle", "darkslategray", "dimgray", "gray", "lightgray", 
   "gray10", "gray20", "gray30", "gray40", "gray50",
   "gray60", "gray70", "gray80", "gray90",
   "<",
   ""};   // null string terminates the menu
  getMenuItem(menu,answer);
  return answer;
}

// utility routine
void GL_GraphicsInterface::
setMaterialProperties(
  float ambr, float ambg, float ambb,
  float difr, float difg, float difb,
  float specr, float specg, float specb, float shine)
{
// modifies the material properties for objects in the currentWindow
  glDisable(GL_COLOR_MATERIAL);

  GLfloat mat[4];

  mat[3] = 1.0;
//    mat[0] = ambr*materialScaleFactor[currentWindow];
//    mat[1] = ambg*materialScaleFactor[currentWindow];
//    mat[2] = ambb*materialScaleFactor[currentWindow];
  mat[0] = ambr;
  mat[1] = ambg;
  mat[2] = ambb;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat);
//    mat[0] = difr*materialScaleFactor[currentWindow];
//    mat[1] = difg*materialScaleFactor[currentWindow];
//    mat[2] = difb*materialScaleFactor[currentWindow];
  mat[0] = difr;
  mat[1] = difg;
  mat[2] = difb;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat);
//    mat[0] = specr*materialScaleFactor[currentWindow];
//    mat[1] = specg*materialScaleFactor[currentWindow];
//    mat[2] = specb*materialScaleFactor[currentWindow];
  mat[0] = specr;
  mat[1] = specg;
  mat[2] = specb;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine * 128.0);
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setColour}} 
int  GL_GraphicsInterface::
setColour( const aString & nameIn )
// /Description
//  set the colour for subsequent objects that are plotted
// /Return value: 0 means success, 1 means failure
//\end{GL_GraphicsInterfaceInclude.tex} 
// ========================================================================================
{
// sets the colur in the currentWindow
  int returnValue=0;
  
//    if( lighting[currentWindow] )
//    {
    glShadeModel(GL_SMOOTH);     // smooth shading
    
// this causes the object to reflect according to its colour
//    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT);   // AP: This call is overridden by the next call...
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE); 
//    }
//    else
//    {
//      glShadeModel(GL_FLAT);     // flat shading
//      glDisable(GL_COLOR_MATERIAL);
//    }

  // convert to upper case and remove blanks
  aString name = nameIn;
  int i,j=0;
  for( i=0; i<nameIn.length(); i++ )
  {
    if( nameIn[i]!=' ' )
      name[j++]=toupper(nameIn[i]);
  } 
  if( j<(nameIn.length()-1) )
    name[j]='\0';
 
  // printF("setColour: nameIn=[%s] name=[%s] \n",(const char*)nameIn,(const char*)name);
  

  if( name=="EMERALD" )
    setMaterialProperties(0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6);
  else if( name=="JADE" )
    setMaterialProperties(0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 0.1);
  else if( name=="OBSIDIAN" )
    setMaterialProperties(0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 0.3);
  else if( name=="PEARL" )
    setMaterialProperties(0.25, 0.20725, 0.20725, 1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 0.088);
  else if( name=="RUBY" )
    setMaterialProperties(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 
                          0.626959, 0.6);
  else if( name=="TURQUOISE" )
    setMaterialProperties(0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 0.1);
  else if( name=="BRASS" )
    setMaterialProperties(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 
                          0.941176, 0.807843, 0.21794872);
  else if( name=="BRONZE" )
    setMaterialProperties(0.2125, 0.1275, 0.054,
			  0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 0.2);
  else if( name=="CHROME" )
    setMaterialProperties(0.25, 0.25, 0.25,
			  0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6);
  else if( name=="COPPER" )
    setMaterialProperties(0.19125, 0.0735, 0.0225,
			  0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 0.1);
  else if( name=="GOLD" )
    setMaterialProperties(0.24725, 0.1995, 0.0745,
			  0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4);
  else if( name=="SILVER" )
    setMaterialProperties(0.19225, 0.19225, 0.19225,
			  0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);
  else if( name=="BLACKPLASTIC" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.01, 0.01, 0.01,
			  0.50, 0.50, 0.50, .25);
  else if( name=="CYANPLASTIC" )
    setMaterialProperties(0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392,
			  0.50196078, 0.50196078, 0.50196078, .25);
  else if( name=="GREENPLASTIC" )
    setMaterialProperties(0.0, 0.0, 0.0,
			  0.1, 0.35, 0.1, 0.45, 0.55, 0.45, .25);
  else if( name=="REDPLASTIC" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.5, 0.0, 0.0,
			  0.7, 0.6, 0.6, .25);
  else if( name=="WHITEPLASTIC" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.55, 0.55, 0.55,
			  0.70, 0.70, 0.70, .25);
  else if( name=="YELLOWPLASTIC" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.5, 0.5, 0.0,
			  0.60, 0.60, 0.50, .25);
  else if( name=="BLACKRUBBER" )
    setMaterialProperties(0.02, 0.02, 0.02, 0.01, 0.01, 0.01,
			  0.4, 0.4, 0.4, .078125);
  else if( name=="CYANRUBBER" )
    setMaterialProperties(0.0, 0.05, 0.05, 0.4, 0.5, 0.5,
			  0.04, 0.7, 0.7, .078125);
  else if( name=="GREENRUBBER" )
    setMaterialProperties(0.0, 0.05, 0.0, 0.4, 0.5, 0.4,
			  0.04, 0.7, 0.04, .078125);
  else if( name=="REDRUBBER" )
    setMaterialProperties(0.05, 0.0, 0.0, 0.5, 0.4, 0.4,
			  0.7, 0.04, 0.04, .078125);
  else if( name=="WHITERUBBER" )
    setMaterialProperties(0.05, 0.05, 0.05, 0.5, 0.5, 0.5,
			  0.7, 0.7, 0.7, .078125);
  else if( name=="YELLOWRUBBER" )
    setMaterialProperties(0.05, 0.05, 0.0, 0.5, 0.5, 0.4,
			  0.7, 0.7, 0.04, .078125);
  else
  {
//    outputString("Using the default material");
    glEnable(GL_COLOR_MATERIAL);
// AP: The ambient and diffuse properties are overridden by setXColour since we have called
// glColorMaterial with the AMBIENT_AND_DIFFUSE argument
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient[currentWindow]);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse[currentWindow]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,materialSpecular[currentWindow]);
     // 50. bigger = sharper high-lite, [0,128] :
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess[currentWindow]); 
    setXColour(name);   // assign some standard x-colours
  }
//  else
//  {
//    cout << "setColour:ERROR unknown colour = " << (const char*) name << endl;
//    returnValue=1;
//  }

  return returnValue;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setColour}} 
int GL_GraphicsInterface::
setColour( ItemColourEnum item )  
// ========================================================================================
// /Description:
//   Set colour to default for a given type of item
//\end{GL_GraphicsInterfaceInclude.tex}
// ========================================================================================
{
  real rgb[3];
  if (item == textColour)
    getXColour(foreGroundName[currentWindow],rgb);
  else
    getXColour(backGroundName[currentWindow],rgb);

  glColor3(rgb[0],rgb[1],rgb[2]);
  return 0;
  // return setColour(itemColour[item]);
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getColour}} 
aString GL_GraphicsInterface::
getColour( ItemColourEnum item )
// ========================================================================================
// /Description:  
//   Get the name of the colour for backGroundColour, textColour, ...
//\end{GL_GraphicsInterfaceInclude.tex}
// ========================================================================================
{
  if (item == textColour)
    return foreGroundName[currentWindow];
  else if( item==backGroundColour )
    return backGroundName[currentWindow];
  else
  {
    printF("GL_GraphicsInterface::getColour:ERROR: unknown ItemColourEnum=%i\n",item);
    return "black";
  }
  
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setColourName}} 
void GL_GraphicsInterface::
setColourName( int i, aString newColourName ) const
// ========================================================================================
// /Description:  
//     Assign the name of colour i in the list of colours. 
//\end{GL_GraphicsInterfaceInclude.tex}
// ========================================================================================
{
  colourNames[i % numberOfColourNames] = newColourName;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getColourName}} 
aString GL_GraphicsInterface::
getColourName( int i ) const
// ========================================================================================
// /Description:  
//     Return the name of colour i in the list of colours. 
//\end{GL_GraphicsInterfaceInclude.tex}
// ========================================================================================
{
  return colourNames[i % numberOfColourNames];
}

void GL_GraphicsInterface:: 
lightsOn(int win_number)
//----------------------------------------------------------------------
// /Description:
//   Turn ON lighting in window number win\_number before executing display lists. This function
//   is called by the internal display call-back routine before executing the lit display lists. This
//   routine is normally NOT called from an application code.
// /win\_number: The number of the graphics window.
//
// /Return values: none.
//
//  /Author: AP
//----------------------------------------------------------------------
{
  glEnable(GL_LIGHTING);

  // specify the properties of the lights
  for( int light=0; light<numberOfLights; light++ )
  {
    GLenum glLight =  light==0 ? GL_LIGHT0 :
                     (light==1 ? GL_LIGHT1 :
                     (light==2 ? GL_LIGHT2 :
                     (light==3 ? GL_LIGHT3 :
                     (light==4 ? GL_LIGHT4 :
                     (light==5 ? GL_LIGHT5 :
                     (light==6 ? GL_LIGHT6 : GL_LIGHT7 ))))));
    if( lightIsOn[win_number][light] )
    {
      glLightfv(glLight, GL_DIFFUSE, diffuse[win_number][light]);
      glLightfv(glLight, GL_SPECULAR,specular[win_number][light]);
      glLightfv(glLight, GL_AMBIENT, ambient[win_number][light]);
      // position the light in a fixed frame ****** is this right ??? *******
      glPushMatrix();
      glLoadIdentity();
      glLightfv(glLight, GL_POSITION,position[win_number][light]);
      glPopMatrix();
      glEnable(glLight);
    }
    else
      glDisable(glLight);
  }

  if( numberOfLights>8 )
    cout << "GL_GraphicsInterface::lightsOn:ERROR fix this routine for more lights\n";
    
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient[win_number] );

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );  // treat front and back facing the same

// AP: these calls are made in setColour, but only for X-colours!!!
// this causes the grids to reflect according to their colour
//    glColorMaterial(GL_FRONT,GL_DIFFUSE);   
//    glColorMaterial(GL_FRONT,GL_AMBIENT); 
//    glEnable(GL_COLOR_MATERIAL);
}

void GL_GraphicsInterface:: 
lightsOff(int win_number)
//----------------------------------------------------------------------
// /Description:
//   Turn OFF lighting in window number win\_number before executing display lists. This function
//   is called by the internal display call-back routine before executing the unlit display lists. This
//   routine is normally NOT called from an application code.
// /win\_number: The number of the graphics window.
// /Return values: none.
//
//  /Author: AP
//----------------------------------------------------------------------
{
// Turn off lighting in window win_number
  glDisable(GL_LIGHTING);
  if( lightIsOn[win_number][0] )
    glDisable(GL_LIGHT0);
  if( lightIsOn[win_number][1] )
    glDisable(GL_LIGHT1);
  if( lightIsOn[win_number][2] )
    glDisable(GL_LIGHT2);
}

void GL_GraphicsInterface::
rleCompress( const int num, GLubyte *xBuffer, FILE *outFile, const int numPerLine /* = 30 */ )
// ==============================================================================================
// /Description:
//    Compress a byte stream using a run length encoding scheme. See the postscript manual
//  for details of RLE files.
// ==============================================================================================
{

  real time=getCPU();

  // printF("\n\n\n ***** rleCompress ****** n\n\n");

  int r;         // repetition count

  int maxR=128;  // largest repetition count allowed is 128

  int i=0;           // current char
  int count=0;       // number of chars printed on current lline
  while( i<num )
  {
    // count the number of similar chars
    r=1;
    while( r<maxR && i+r<num && xBuffer[i+r]==xBuffer[i] )
    {
      r++;
    }
    if( r>1 )
    {
      // printF("repeat: r=%i, char=%2.2X \n",r,xBuffer[i]);
      fPrintF(outFile,"%2.2X",257-r);   // length = 257-r
      fPrintF(outFile,"%2.2X",xBuffer[i]);
      i+=r;
      count+=2;
    }
    else
    { // : b[i+1]!=b[i]
      // count number of dis-similiar chars
      r=1;
      while( r<maxR && i+r+1 < num && xBuffer[i+r+1]!=xBuffer[i+r] )
      {
	r++;
      }
      // printF("dis-similar: r=%i, start-1=%2.2X start=%2.2X , end=%2.2X, end+1=%2.2X \n",r,xBuffer[i-1],
      //       xBuffer[i],xBuffer[i+r-1],xBuffer[i+r]);
      fPrintF(outFile,"%2.2X",r-1);   // length = r-1  [0,127]
      for( int j=i; j<i+r; j++ )
      {
	fPrintF(outFile,"%2.2X",xBuffer[j]);
        count++;
	if( count > numPerLine )
	{
	  fPrintF(outFile,"\n");
	  count=0;
	}
      }
      i+=r;
    }
    if( count > numPerLine )
    {
      fPrintF(outFile,"\n");
      count=0;
    }
  }
  // write EOD    
  fPrintF(outFile,"%2.2X",128);

  // add some zeroes to the end of the data -- needed by printer for some reason
  for( ;count<=numPerLine; count++ )  
    fPrintF(outFile,"%2.2X",0);        
  fPrintF(outFile,"\n");
  for( i=0; i<2; i++ )
  {
    for( count=0; count<numPerLine; count++ ) 
      fPrintF(outFile,"%2.2X",0);        
    fPrintF(outFile,"\n");
  }
  printF("time for rle = %e \n",getCPU()-time);
}

// this macro converts from [0,1] to [0,255]
#define C(x) ( int((x*255)+.5)  )


int GL_GraphicsInterface::
saveRasterInAFile(const aString & fileName, 
                  void *buffer, 
                  const GLint & width, 
                  const GLint & height,
                  const int & rgbType /* =0 */,
                  GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */)
// ================================================================================
// /rbgType (input) : 0=buffer is an array of floats (r,g,b)
//                    1=buffer is an array of bytes (r,g,b,a)
// ================================================================================
{

  GraphicsParameters::OutputFormat localOutputFormat;
  GraphicsParameters::HardCopyType localHardCopyType;
  if( parameters.isDefault() )
  {
    localOutputFormat=outputFormat[currentWindow];
    localHardCopyType=hardCopyType[currentWindow];
  }
  else
  {
    localOutputFormat=parameters.outputFormat;
    localHardCopyType=parameters.hardCopyType;
  }

  FILE *file;
  file = fopen((const char*)fileName.c_str(),"w" );         
  if( file==NULL )
  {
    cout << "GL_GraphicsInterface::saveRasterInAFile:ERROR: unable to open the file: " << fileName << endl;
    return 1;
  }

  // GLint x=0,y=0;  // lower corner of image
  int stride = rgbType==0 ? 3 : 4;
  int num = width*height*stride;    // total number of entries in the buffer


  if( localHardCopyType==GraphicsParameters::ppm )
  {
    // output a ppm file 
    int i, x, y;
    fPrintF(file,"P6\n");
    fPrintF(file,"# ppm-file created by %s\n", "plotStuff");
    fPrintF(file,"%i %i\n", width,height);
    fPrintF(file,"255\n");  // max colour value 
    fclose(file);
    file = fopen( (const char *)fileName.c_str(), "ab" );  /* reopen in binary append mode */
    if( rgbType==0 )
    {
      float *ptr=(float*)buffer;
      for (y=height-1; y>=0; y--) {
	for (x=0; x<width; x++) {
	  i = (y*width + x) * 3;
	  fputc(C(ptr[i]), file);   /* write red */
	  fputc(C(ptr[i+1]), file); /* write green */
	  fputc(C(ptr[i+2]), file); /* write blue */
	}
      }
    }
    else
    {
      GLubyte *ptr=(GLubyte*)buffer;
      for (y=height-1; y>=0; y--) {
	for (x=0; x<width; x++) {
	  i = (y*width + x) * 4;
	  fputc(ptr[i], file);   /* write red */
	  fputc(ptr[i+1], file); /* write green */
	  fputc(ptr[i+2], file); /* write blue */
	}
      }
    }
    fclose(file);
    return 0;
  }
  else
  {  // output a post script file
    
    int length = fileName.length();

#ifndef NO_APP
    bool saveEPS= localHardCopyType==GraphicsParameters::encapsulatedPostScript
                  || fileName(length-4,length-1)==".eps";
#else
    bool saveEPS= localHardCopyType==GraphicsParameters::encapsulatedPostScript
                  || fileName.substr(length-4,4)==".eps";
#endif
    if( saveEPS )
      printF("saving an encapsulated postcript file\n");

    int numPerLine=30;           // print this many colours per line  

    real llbx,llby,urbx,urby,scale,scaleFactor;
    real scaleWidth, scaleHeight;
  // 
  // Scale the picture:
  //  Note we assume that the user meant to have width=height, thus we scale the
  // result to have a square aspect ratio
  //
    if( saveEPS )
    {
      // save an encapsulated post script file
      llbx=0;
      llby=0;
      urbx=width;
      urby=height;
      scaleFactor=scale=1.;
      scaleWidth=width;
      scaleHeight=height;
    }
    else
    {
      // save a postScript file --- scale to the size of a printed page
      real pageWidth=8.5;  // page width in inches

      real pageHeight=11.;  
      real leftMargin=.5;   
      real rightMargin=.5;
      real bottomMargin=.5;
      real topMargin=.5;
      real w=(pageWidth-leftMargin-rightMargin)*72.;  // width of space for figure (in pts. 1/72 inch)
      real h=(pageHeight-bottomMargin-topMargin)*72.;
      if( height/width <= h/w )
      { // the width of the figure determines how it should be scaled
	llbx=leftMargin*72.;
	llby=bottomMargin*72.;
	scaleFactor=w/width;
	scale=w;
        scaleWidth=w;
	scaleHeight=w*height/width;
      }
      else
      {
	scaleFactor=h/height;
	scale=h;
	llbx=leftMargin*72.+.5*(w-width*scaleFactor);   // figure is narrow, centre it in the x-direction
	llby=bottomMargin*72.;

        scaleWidth=h*width/height;
	scaleHeight=h;
      }
      urbx=llbx+width*scaleFactor;
      urby=llby+height*scaleFactor;
      // ** urby=llby+max(width,height)*scaleFactor;
      // ** scaleWidth=scaleHeight=scale;
    }
    
    fPrintF(file,"%%!PS-Adobe-2.0 EPSF-2.0\n");
    fPrintF(file,"%%%%Creator: PlotStuff v1.0\n");
    fPrintF(file,"%%%%Title: What a concept! \n");
    fPrintF(file,"%%%%CreationDate: Fri Feb 29 12:34:56 1999 \n");
    fPrintF(file,"%%%%Pages: 1 \n");
    fPrintF(file,"%%%%Requirements: colorprinter \n");
    fPrintF(file,"%%%%BoundingBox: %i %i %i %i \n",int(llbx+.5),int(llby+.5),int(urbx+.5),int(urby+.5));
    fPrintF(file,"%%%%EndComments \n");
    fPrintF(file,"%%%%EndProlog \n");
    fPrintF(file,"%%%%Page: 1 1 \n");
    fPrintF(file,"gsave \n");


    if( localOutputFormat != GraphicsParameters::colour24Bit )
    {

      const int ctSize=256;
      short ct[ctSize][3];
      int nt;                 // number of colour table entries
      short r,g,b;
      if( localOutputFormat==GraphicsParameters::colour8Bit )
      {
	// there are really 5*9*5=225 possible colours
	// red:   0x00,       0x3F,       0x7F,       0xBF,       0xFF
	// green: 0x00, 0x1F, 0x3F, 0x5F, 0x7F, 0x9F, 0xBF, 0xDF, 0xFF
        nt=225;
	int ir,ig,ib;
	for( int i=0; i<nt; i++ )
	{
	  ir= i     % 5;
	  ig=(i/5)  % 9;
	  ib=(i/45) % 5;
	  ct[i][0]=max(0,64*ir-1);
	  ct[i][1]=max(0,32*ig-1);
	  ct[i][2]=max(0,64*ib-1);
	  // printF(" i=%i, ct=(%2X,%2X,%2X) \n",i,ct[i][0],ct[i][1],ct[i][2]);
	}
      }
      else if( localOutputFormat==GraphicsParameters::grayScale )
      {
        nt=256;
	for( int i=0; i<nt; i++ )
	{
	  ct[i][0]=i;
	  ct[i][1]=i;
	  ct[i][2]=i;
	}
      }
      else
      {
        nt=2;
	ct[0][0]=0;
	ct[0][1]=0;
	ct[0][2]=0;
	ct[1][0]=255;
	ct[1][1]=255;
	ct[1][2]=255;
	
      }
      
      GLubyte *iBuff = new GLubyte [width*height];  // holds color table index value for each pixel

      float *xBuffer  =(float*)buffer;
      GLubyte *cBuffer=(GLubyte*)buffer;


      printF("fill index buffer...\n");
      real time=getCPU();
      int count=0;  
      int ii=0, jj=0;
      int w3=width*stride;

      int i;
      for( i=0; i<num; i+=stride )
      {
        if( rgbType==0 )
	{
    	  r=C(xBuffer[i]);      
	  g=C(xBuffer[i+1]);    
	  b=C(xBuffer[i+2]);    
	}
        else
	{
    	  r=cBuffer[i];      
	  g=cBuffer[i+1];    
	  b=cBuffer[i+2];    
	}
        if( localOutputFormat==GraphicsParameters::colour8Bit )
	{
	  const short int
	    ditherRed[2][16]=
	  {
	    {-16,  4, -1, 11,-14,  6, -3,  9,-15,  5, -2, 10,-13,  7, -4,  8},
	    { 15, -5,  0,-12, 13, -7,  2,-10, 14, -6,  1,-11, 12, -8,  3, -9}
	  },
	    ditherGreen[2][16]=
	    {
	      { 11,-15,  7, -3,  8,-14,  4, -2, 10,-16,  6, -4,  9,-13,  5, -1},
	      {-12, 14, -8,  2, -9, 13, -5,  1,-11, 15, -7,  3,-10, 12, -6,  0}
	    },
	      ditherBlue[2][16]=
	      {
		{ -3,  9,-13,  7, -1, 11,-15,  5, -4,  8,-14,  6, -2, 10,-16,  4},
		{  2,-10, 12, -8,  0,-12, 14, -6,  3, -9, 13, -7,  1,-11, 15, -5}
	      };

	      // dither:
          if( r!=0 && r!=255 )
	  {
	    r += (ditherRed  [ii][jj]) << 1;
	    r=min(255,max(0,(((r+32) >> 6) << 6) -1)); // ((r+32)/64)*64-1)
	    // r=min(255,max(0,((r+32) & 0xc0) -1)); // ((r+32)/64)*64-1)
	  }
	  if( g!=0 && g!=255 )
	  {
	    g += ditherGreen[ii][jj];
	    g=min(255,max(0,(((g+16) >> 5) << 5) -1)); // ((g+16)/32)*32-1)
	    // g=min(255,max(0,((g+16) & 0xe0) -1)); // ((g+16)/32)*32-1)
	  }
	  if( b!=0 && b!=255 )
	  {
	    b += (ditherBlue [ii][jj]) << 1;
	    b=min(255,max(0,(((b+32) >> 6) << 6) -1)); // ((b+32)/64)*64-1)
  	    // b=min(255,max(0,((b+32) & 0xc0) -1)); // ((b+32)/64)*64-1)
	  }
	  // reduce to 5 red by 9 green by 5 blue colours
 
	  jj++;
          if( jj==16 )
	    jj=0;
	    
	  if( (i+stride) % w3 == 0 )
	  {
	    jj=0;
	    ii= (ii+1) %2;  // alternate scan lines
	  }
	  // iBuff[count++]=(r+1) /64 + 5*( (g+1)/32 + 9*( (b+1)/64 ) );
	  iBuff[count++]=( (r+1) >> 6) + 5*( ((g+1) >> 5) + 9*( ((b+1) >> 6) ) );
	}
	else if( localOutputFormat==GraphicsParameters::grayScale )
	{
	  iBuff[count++] = (unsigned char)(.114*b+.299*r+.587*g + .5); // convert to a gray scale
	}
	else
	{ // convert to black and white
	  iBuff[count++] = (r!=255 || g!=255 || b!=255 ) ? 0 : 1; // 0=black, 1=white
	}
      }
      printF("...done, time=%e \n",getCPU()-time);
    
      fPrintF(file,"%f %f translate \n",llbx,llby);
      fPrintF(file,"%f %f scale \n",scaleWidth,scaleHeight);

      // Save the colour table
      fPrintF(file,"[/Indexed/DeviceRGB %i\n<",nt-1);
      i=0;
      while( i<nt )
      {
	for( int j=0; j<10 && i<nt ; j++,i++ )
	{
	  fPrintF(file,"%2.2X%2.2X%2.2X",ct[i][0],ct[i][1],ct[i][2]);
	}
	fPrintF(file,"\n");
      }
      fPrintF(file,">\n]setcolorspace\n\n");


      fPrintF(file,"<<                    %% start image dictionary \n");
      fPrintF(file,"/ImageType 1 \n");
      fPrintF(file,"/Width %i   \n",width);
      fPrintF(file,"/Height %i  \n",height);
      fPrintF(file,"/BitsPerComponent 8 \n");
      fPrintF(file,"/Decode [0 255]     \n");
      fPrintF(file,"/ImageMatrix [%i 0 0 %i 0 0] \n",width,height);
      fPrintF(file,"/DataSource currentfile /ASCIIHexDecode filter /RunLengthDecode filter \n");
      fPrintF(file,">>   \n");
      fPrintF(file,"image \n");

      // output the index colour RLE file
      rleCompress( width*height, iBuff, file, numPerLine );

      delete [] iBuff;

    }
    else if( localOutputFormat == GraphicsParameters::colour24Bit )
    {
      // printF("saveRasterInAFile:Warning: There are more than 256 colours, number of colours = %i \n",nt);  
      // printF("     The .ps file is thus being saved in a way that requires more space\n");

      fPrintF(file,"/bufstr %i string def \n",3*width); 
      fPrintF(file,"%f %f translate \n",llbx,llby);
      fPrintF(file,"%f %f scale \n",scaleWidth,scaleHeight);  // *wdh* changed from scale,scale -- need for eps
      fPrintF(file,"%i %i 8 \n",width,height);
      fPrintF(file,"[%i 0 0 %i 0 0]\n",width,height);
      fPrintF(file,"{currentfile bufstr readhexstring pop} bind \n");
      fPrintF(file,"false 3 colorimage \n\n");


      int i=0;
      if( rgbType==0 )
      {
        float *xBuffer  =(float*)buffer;
	for( int k=0; k<(num+numPerLine-1)/numPerLine; k++)   // print this many lines 
	{
	  for( int j=0; j<numPerLine && i<num ; j++)  // print colours on a line 
	    fPrintF(file,"%2.2X",C(xBuffer[i++]));
	  fPrintF(file,"\n");
	}
      }
      else
      {
	
        GLubyte *cBuffer=(GLubyte*)buffer;
	for( int k=0; k<(num+numPerLine-1)/numPerLine; k++)   // print this many lines 
	{
	  for( int j=0; j<numPerLine && i<num ; j++)  // print colours on a line 
	  { // *wdh* 070712 -- fixed : need to skip every 4th entry
	    fPrintF(file,"%2.2X",cBuffer[i++]);
	    fPrintF(file,"%2.2X",cBuffer[i++]);
	    fPrintF(file,"%2.2X",cBuffer[i++]);
            i++;
	  }
	  
	  fPrintF(file,"\n");
	}
      }
    }

    fPrintF(file,"\n %% grestore\n"); // don't do this
    if( saveEPS )
      fPrintF(file,"%% to print this file add the command showpage here.\n");
    else
      fPrintF(file,"showpage\n");
    fPrintF(file,"%%%%Trailer\n");
    fclose(file);
  }
  
  return 0;

}



//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setAxesLabels}} 
int GL_GraphicsInterface::
setAxesLabels( const aString & xAxisLabel_ /* = blankString */,
	       const aString & yAxisLabel_ /* = blankString */,
	       const aString & zAxisLabel_ /* = blankString */ )
//----------------------------------------------------------------------
// /Description:
//   Set labels on the coordinate axes. The labels will be plotted in the currentWindow 
//   next time the screen is updated.
// /xAxisLabel\_: The label on the x-axis.
// /yAxisLabel\_: The label on the y-axis.
// /zAxisLabel\_: The label on the z-axis.
// /Return values: none.
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  xAxisLabel[currentWindow] = xAxisLabel_;
  yAxisLabel[currentWindow] = yAxisLabel_;
  zAxisLabel[currentWindow] = zAxisLabel_;
  return 0;
}

int GL_GraphicsInterface::
setAxesOrigin( const real x0 /* =defaultOrigin */ , 
	       const real y0 /* =defaultOrigin */ , 
	       const real z0 /* =defaultOrigin */ )
{
  axesOrigin[currentWindow](0)=x0;
  axesOrigin[currentWindow](1)=y0;
  axesOrigin[currentWindow](2)=z0;
  return 0;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setLineWidthScaleFactor}} 
void GL_GraphicsInterface::
setLineWidthScaleFactor(const real & scaleFactor /* =1. */, int win_number /* = -1 */ )
//----------------------------------------------------------------------------------
//  /Description:
// Set scale factor for line widths (this can be used to increase the line widths for
// high-res off screen rendering.
//  /Author: WDH \& AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if (win_number == -1)
    win_number = currentWindow;
  
  lineWidthScaleFactor[win_number]=scaleFactor;
}

void GL_GraphicsInterface::
setFractionOfScreen(const real & fraction /* =.75 */, int win_number /* = -1 */ )
//----------------------------------------------------------------------------------
//  /Description:
//     Set the fraction of the screen to use.
//  /Author: WDH
//-----------------------------------------------------------------------------------
{
  if (win_number == -1)
    win_number = currentWindow;
  
  fractionOfScreen[win_number]=fraction;
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{normalizedToWorldCoordinates}} 
int GL_GraphicsInterface::
normalizedToWorldCoordinates(const RealArray & r, RealArray & x ) const
//----------------------------------------------------------------------
// /Description:
// Convert normalized coordinates [-1,+1] to world coordinates
// /r(i,0:2) (input) : points to convert.
// /x(i,0:2) (output) : converted points. (x and r can be the same array).
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  const int winNumber=currentWindow;
#ifndef NO_APP
  assert( r.getBase(1)<=0 && r.getBound(1)>=2 ); 
  Range R=r.dimension(0);
  if( x.getBase(0)>R.getBase() || x.getBound(0)<R.getBound() || x.getBase(1) >0 || x.getBound(1)<2 )
    x.redim(R,3);
#else // ArraySimple's always has base 0
  if( x.getBound(0)<r.getBound(0) || x.getBound(1)<2 )
    x.redim(r.getLength(0),3);
#endif  

#ifndef NO_APP
  for( int i=r.getBase(0); i<=r.getBound(0); i++ )
#else
  for( int i=0; i<=r.getBound(0); i++ )
#endif
  {
    GLdouble winx,winy,winz;
    winx=(r(i,0)+1.)*.5*(viewPort[winNumber][2]-viewPort[winNumber][0])+viewPort[winNumber][0];
    winy=(r(i,1)+1.)*.5*(viewPort[winNumber][3]-viewPort[winNumber][1])+viewPort[winNumber][1];
    winz=r(i,2);
	  
    GLdouble x1,x2,x3;
    gluUnProject( winx,winy,winz,modelMatrix[winNumber],projectionMatrix[winNumber],viewPort[winNumber],
		  &x1,&x2,&x3);
    x(i,0)=x1;
    x(i,1)=x2;
    x(i,2)=x3;
    
  }
  return 0;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{worldToNormalizedCoordinates}} 
int GL_GraphicsInterface::
worldToNormalizedCoordinates(const RealArray & x, RealArray & r ) const
//----------------------------------------------------------------------
// /Description:
// Convert world coordinate to normalized coordinates [-1,+1] 
// /x(i,0:2) (input) : points to convert.
// /r(i,0:1) (output) : converted points. (x and r can be the same array).
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  const int winNumber=currentWindow;
#ifndef NO_APP
  assert( x.getBase(1)<=0 && x.getBound(1)>=2 );
  Range R=x.dimension(0);
  if( r.getBase(0)>R.getBase() || r.getBound(0)<R.getBound() || r.getBase(1) >0 || r.getBound(1)<2 )
    r.redim(R,3);
#else
  if( r.getBound(0)<x.getBound(0) || r.getBound(1)<2 )
    r.redim(x.getLength(0),3);
#endif

#ifndef NO_APP
  for( int i=x.getBase(0); i<=x.getBound(0); i++ )
#else
  for( int i=0; i<=x.getBound(0); i++ )
#endif
  {
    GLdouble winx,winy,winz;
    GLdouble x1,x2,x3;
    
    x1=x(i,0);
    x2=x(i,1);
    x3=x(i,2);
    
    gluProject( x1,x2,x3,modelMatrix[winNumber],projectionMatrix[winNumber],viewPort[winNumber],
		  &winx,&winy,&winz);
    r(i,0)=winx;
    r(i,1)=winy;
    r(i,2)=winz;
    
  }
  real scale1=max(REAL_MIN*100.,.5*(viewPort[winNumber][2]-viewPort[winNumber][0])+viewPort[winNumber][0]);
  real scale2=max(REAL_MIN*100.,.5*(viewPort[winNumber][3]-viewPort[winNumber][1])+viewPort[winNumber][1]);
#ifndef NO_APP
  r(R,0)=r(R,0)*(1./scale1)-1.;
  r(R,1)=r(R,1)*(1./scale2)-1.;
#else
  for (int q=0; q<x.getLength(0); q++)
  {
    r(q,0)=r(q,0)*(1./scale1)-1.;
    r(q,1)=r(q,1)*(1./scale2)-1.;
  }
#endif
  return 0;
}


void GL_GraphicsInterface::
setRotationCenter(real rotationPoint[3], int win_number /* = -1 */ )
//----------------------------------------------------------------------
// /Description:
//   Set the centre for rotations.
// /rotationPoint (input) : rotation centre in world coordinates (i.e. the same dimensions
// as the {\tt globalBound}.
// /win\_number (input): The window number. If omitted, the current window.
//
// /author: WDH \& AP
//----------------------------------------------------------------------
{
  if (win_number == -1) win_number=currentWindow;
  
  if( !userDefinedRotationPoint[win_number] )
  {
    for (int axis=0; axis<3; axis++ )
      rotationCenter[win_number][axis] = 0.5*(globalBound[win_number](Start,axis) + 
					      globalBound[win_number](End,axis));
  }

  // we must compute an shift adjustment when the centre of rotation is changed.
  // Keep track of the difference
  real dc[3], rotDc[3];

// test
//    printF("setRotCtr: rotPnt = %e, %e, %e\n", rotationPoint[0], 
//  	 rotationPoint[1], rotationPoint[2]);
//    printF("setRotCtr: rotCtr = %e, %e, %e\n", rotationCenter[win_number][0], 
//  	 rotationCenter[win_number][1], rotationCenter[win_number][2]);

  dc[0] = rotationPoint[0] - rotationCenter[win_number][0];
  dc[1] = rotationPoint[1] - rotationCenter[win_number][1];
  dc[2] = rotationPoint[2] - rotationCenter[win_number][2];
  int i,j;
  for (i=0; i<3; i++)
  {
    rotDc[i] = 0;
    for (j=0; j<3; j++)
      rotDc[i] += rotationMatrix[win_number](i,j) * dc[j];
  }
  // These corrections accumulate
  for (i=0; i<3; i++)
    shiftCorrection[win_number][i] += dc[i] - rotDc[i];


// test
//    printF("setRotCtr: ShiftCorrection = %e, %e, %e\n", shiftCorrection[win_number][0], 
//  	 shiftCorrection[win_number][1], shiftCorrection[win_number][2]);

  rotationCenter[win_number][0] = rotationPoint[0];
  rotationCenter[win_number][1] = rotationPoint[1];
  rotationCenter[win_number][2] = rotationPoint[2];

  userDefinedRotationPoint[win_number]=TRUE;

}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setView}} 
void GL_GraphicsInterface::
setView(const ViewParameters & viewParameter, const real & value)
//----------------------------------------------------------------------
//
// /Description: set some view parameters. The change will take effect
//   the next time the view is updated (with a call to redraw for example).
//
// /viewParmeter (input): indicate which parameter to change:
//   \begin{verbatim}
//  enum ViewParameters
//    {
//      xAxisAngle,      // angle to rotate about x-axis (absolute value, not incremental)
//      yAxisAngle,
//      zAxisAngle,
//      xTranslation,
//      yTranslation,
//      zTranslation,
//      magnification
//    }
//   \end{verbatim}
// /value (input) : change the parameter to this value.
// /Note: setting one of the angle parameters (xAxisAngle, yAxisAngle or zAxisAngle)
//   will cause the current rotation matrix to be reset to the identity. One or more
//   angle parameters can be changed and the changes will take effect the next time the
//   view is updated.
//  /Author: WDH \& AP
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
//  
{
  switch (viewParameter)
  {
  case xAxisAngle:
    dtx[currentWindow]=value;
    break;
  case yAxisAngle:
    dty[currentWindow]=value;
    break;
  case zAxisAngle:
    dtz[currentWindow]=value;
    break;
  case xTranslation:
    xShift[currentWindow]=value;
    break;
  case yTranslation:
    yShift[currentWindow]=value;
    break;
  case zTranslation:
    zShift[currentWindow]=value;
    break;
  case magnification:
    magnificationFactor[currentWindow]=value;
    setProjection(currentWindow);
    break;
  default:
    cout << "GL_GraphicsInterface::setView: ERROR, unknown option! \n";
  }
// AP: Why reset the rotation matrix?
  if( viewParameter==xAxisAngle || viewParameter==yAxisAngle || viewParameter==zAxisAngle )
  {
     // reset the rotation matrix
     rotationMatrix[currentWindow]=0.;
     rotationMatrix[currentWindow](0,0)=1.;   rotationMatrix[currentWindow](1,1)=1.;   
     rotationMatrix[currentWindow](2,2)=1.;   rotationMatrix[currentWindow](3,3)=1.; 
  }
// AP: Do we need to call setRotationTransformation here?  
}


int GL_GraphicsInterface::
pickToWorldCoordinates(const RealArray & r, RealArray & x, const int & win_number ) const
// ============================================================================================
// /Description:
// convert screen [0,1] + zBuffer [0,$2^{31}$] coordinates (from getMenuItem) to world coordinates.
//
// /r (input) : (r(i,0),r(i,1)) = (r,s) screen coordinates returned in the pickRegion array from getMenuItem.\\
//              r(i,2) = zBuffer coordinates returned in the selection array from getMenuItem.
// /x (output) : x(i,0:2) : world coordinates
// /win\_number: The window number where the picking occured.
//
//  /Return Value: 0
//
//  /Author: WDH \& AP
// ============================================================================================
{
	  
#ifndef NO_APP
  assert( r.getBase(1)<=0 && r.getBound(1)>=2 );
  Range R=r.dimension(0);
  if( x.getBase(0)>R.getBase() || x.getBound(0)<R.getBound() || x.getBase(1) >0 || x.getBound(1)<2 )
    x.redim(R,3);
#else
  if( x.getBound(0)<r.getBound(0) || x.getBound(1)<2 )
    x.redim(r.getLength(0),3);
#endif
  
  const real zBufferResolution=pow(2.,31.); // where does 31 come from ?
#ifndef NO_APP
  for( int i=r.getBase(0); i<=r.getBound(0); i++ )
#else
  for( int i=0; i<=r.getBound(0); i++ )
#endif
  {
    GLdouble winx,winy,winz;
    winx=r(i,0)*(viewPort[win_number][2]-viewPort[win_number][0])+viewPort[win_number][0];
    winy=r(i,1)*(viewPort[win_number][3]-viewPort[win_number][1])+viewPort[win_number][1];
    winz=r(i,2)/zBufferResolution;
	  
    GLdouble x1,x2,x3;
    gluUnProject( winx,winy,winz,modelMatrix[win_number],projectionMatrix[win_number],viewPort[win_number],
		  &x1,&x2,&x3);
    x(i,0)=x1;
    x(i,1)=x2;
    x(i,2)=x3;
    
  }
  
  return 0;
}



//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{pollEvents}} 
void GL_GraphicsInterface::
pollEvents()
//======================================================================
// /Description:
// Process all current events. Exit when there are no more events. Note that this function
// is very similar to the internal event loop in mogl.C, except that this function is non-blocking.
// Note that this routine can be called from anywhere in an application code to update the
// windows, parse any pending commands, etc. This might for instance be useful during 
// a long computation.
//
// /Return values: none.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// =====================================================================
{
  if( !graphicsWindowIsOpen )
    return;
  moglPollEvents();
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setUserButtonSensitive}} 
void GL_GraphicsInterface::
setUserButtonSensitive( int btn, int trueOrFalse )
// /Description:
// Set the sensitivity (on/off) of push button number "btn" on the bottom of the graphics window.
// /btn(input): Set the sensitivity of this button
// /trueOrFalse(input): Turn the button on or off (grayed out).
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex}
//---------------------------------------------------------------
{
// we should store the sensitivity of each button, so that it is properly set after reading 
// a command file, or opening the GUI.
  if( isGraphicsWindowOpen() && !readFile )
  {
    moglSetButtonSensitive(getCurrentWindow(), btn, trueOrFalse);
  }
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{pushGUI}} 
void GL_GraphicsInterface::
pushGUI( GUIState &newState )
//---------------------------------------------------------------
// /Description:
// Push newState onto the top of the internal GUIState stack and change the menus, buttons 
// and dialog window according to newState.
//
// /newState(input): The description of the context and layout of the new menus, buttons 
// and dialog window. See the GUIState function descriptions for an explaination of how to
// set the context of a GUIState object.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex}
//---------------------------------------------------------------
{
  int i;
//
// make the dialog in the current GUI insensitive
//
//  printF("pushGUI called\n");
  
  if( isGraphicsWindowOpen() && !readFile )
  {
// make the previous dialog window insensitive
    if (currentGUI)
    {
      currentGUI->setSensitive( 0 );
// make all current dialog siblings insensitive too
      for (i=0; i<currentGUI->nDialogSiblings; i++)
	currentGUI->dialogSiblings[i].setSensitive(0);
    }

// do we need to make a local copy of newState? Everything is copied into Motif anyway,
// so maybe it is not necessary...

// dialog
    newState.openDialog();

// make all dialog siblings, but don't display them yet
    for (i=0; i<newState.nDialogSiblings; i++)
      newState.dialogSiblings[i].openDialog(0);

// loop over all graphics windows to setup buttons and pulldown menus
    for (int win=0; win<moglGetNWindows(); win++)
    {
// setup the buttons.
      moglBuildUserButtons(newState.windowButtonCommands, newState.windowButtonLabels, win);
// set the pulldown menu. Make it sensitive only in the current window
      moglBuildUserMenu(newState.pulldownCommand, newState.pulldownTitle, win);
// Only make the buttons and pulldown menus sensitive on the active window
      moglSetSensitive(win, (win == currentWindow) );
    }

// build the popup menu in all windows
    moglBuildPopup( newState.popupMenu );
  }  

// setup the array with the union of all commands
  newState.setAllCommands();

// save a pointer to the current GUI
  newState.prev = currentGUI;
    
// save a pointer to the GL_GI
  newState.gl = this; // replace with an access function
  
// switch the pointer to the current GUI
  currentGUI = &newState;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{popGUI}} 
void GL_GraphicsInterface::
popGUI()
//---------------------------------------------------------------
// /Description:
// Pop the internal GUIState stack and restore menus, buttons 
// and the dialog window according to the previous state.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex}
//---------------------------------------------------------------
{
//  printF("popGUI was called\n");
  int i;
  
  if (!currentGUI)
  {
    printF("WARNING: popGUI called with an empty GUI stack\n");
    return;
  }
  
// remove the dialog window
  if( isGraphicsWindowOpen() )
  {
    currentGUI->closeDialog();

// remove all dialog siblings
    for (i=0; i<currentGUI->nDialogSiblings; i++)
      currentGUI->dialogSiblings[i].closeDialog();
  }
  

  GUIState *prevGUI;
  prevGUI = currentGUI->prev;
  
// how should the memory taken by currentGUI be released?
// release the memory taken by the allCommands array
//  printF("popGUI: freeing allcommands %x\n", currentGUI->allCommands);
  
  delete [] currentGUI->allCommands;
  currentGUI->allCommands = NULL;
  currentGUI->nAllCommands = 0;

// set the pointer to the previous GUIState
  currentGUI = prevGUI;

// make any dialogs active again
  if (currentGUI)
  {
    GUIState &newState = *currentGUI;
    
    if( isGraphicsWindowOpen() && !readFile )
    {
// make the old dialog window sensitive again      
      newState.setSensitive( 1 );

// make all old dialog siblings sensitive too
      int i;
      for (i=0; i<newState.nDialogSiblings; i++)
	newState.dialogSiblings[i].setSensitive(1);

// loop over all graphics windows to setup buttons and pulldown menus
      for (int win=0; win<moglGetNWindows(); win++)
      {
// setup the buttons.
	moglBuildUserButtons(newState.windowButtonCommands, newState.windowButtonLabels, win);
// set the pulldown menu. Make it sensitive only in the current window
	moglBuildUserMenu(newState.pulldownCommand, newState.pulldownTitle, win);
// Only make the buttons and pulldown menus sensitive on the active window
	moglSetSensitive(win, (win == currentWindow) );
      }
// build the popup menu in all windows
// if newState.nPopup > 0, we need to build the popup. If newState doesn't have a
// popup, we need to disable the previous one
      moglBuildPopup( newState.popupMenu );
    }
    
  }
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{createMessageDialog}} 
void GL_GraphicsInterface::
createMessageDialog(aString msg, MessageTypeEnum type)
//---------------------------------------------------------------
// /Description:
// Open a dialog window with a message and a close button. The dialog window only appears
// if the graphical user interface is opened and commands are NOT beeing read from a command file.
//
// /msg(input): The text string with the message. Newline characters `$\backslash$n' indicate line
// breaks.
// /type(input): The type of dialog window to open. The type determines the symbol and the title
// of the dialog window. Can have the following values:
//   \begin{verbatim}
//  enum MessageTypeEnum 
//  {
//    errorDialog,
//    warningDialog,
//    informationDialog,
//    messageDialog // No symbol
//  };
//   \end{verbatim}
//
// /Return values: None
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex}
//---------------------------------------------------------------
{
  if( isGraphicsWindowOpen() && !readFile )
    moglCreateMessageDialog(msg, type);
  else
    outputString(msg);
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{appendCommandHistory}} 
void GL_GraphicsInterface::
appendCommandHistory(const aString &answer)
//---------------------------------------------------------------
// /Description:
// Write a string in the command history window.
// /answer(input): String to be written.
// /Return values: None
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex}
//---------------------------------------------------------------
{
  if( isGraphicsWindowOpen() )
    moglAppendCommandHistory(answer);
}

// *wdh* removed 100912 
// void GL_GraphicsInterface::
// setupMovie(DialogData &mov, int win)
// {
//   aString cc;
//   sPrintF(cc,"MOVIE close dialog:%i", win);
//   mov.setExitCommand(cc, "Close");
//   aString dTitle;
//   sPrintF(dTitle, "Movie:%i", win);
//   mov.setWindowTitle(dTitle);

//   aString pbCommands[3];
//   sPrintF(pbCommands[0], "MOVIE action:%i", win);
//   sPrintF(pbCommands[1], "MOVIE browse:%i", win);
//   pbCommands[2] = "";
  
//   aString pbLabels[] = {"Action!", "Browse...", ""};
//   mov.setPushButtons( pbCommands, pbLabels, 1);

// // toggle buttons
//   aString tbLabels[] = {"Save frames on file", ""};
//   aString tbCommands[2];
//   sPrintF(tbCommands[0], "MOVIE save:%i", win);
//   tbCommands[1] = "";

//   int tbState[] = {saveMovie[win]};
    
//   mov.setToggleButtons(tbCommands, tbLabels, tbState, 1); // organize in 1 column
// // done defining toggle buttons


// // define layout of option menus
//   mov.setOptionMenuColumns(1);

//   aString opFormatCommand[5];
//   sPrintF(opFormatCommand[0], "MOVIE format:%i PS", win);
//   sPrintF(opFormatCommand[1], "MOVIE format:%i EPS", win);
//   sPrintF(opFormatCommand[2], "MOVIE format:%i RasterPS", win);
//   sPrintF(opFormatCommand[3], "MOVIE format:%i ppm", win);
//   opFormatCommand[4] = "";
//   aString opFormatLabel[] = {"PS", 
// 			     "EPS", 
// 			     "PS raster", 
// 			     "ppm", 
// 			     ""};
//   int stdFormat = hardCopyType[win];
// // initial choice: element stdFormat
//   mov.addRadioBox( "Format", opFormatCommand, opFormatLabel, stdFormat, 2); 

//   aString opColourCommand[5];
//   sPrintF(opColourCommand[0], "MOVIE colour:%i 8bit", win);     // commands should not have spaces due to parser
//   sPrintF(opColourCommand[1], "MOVIE colour:%i 24bit", win);
//   sPrintF(opColourCommand[2], "MOVIE colour:%i BW", win);
//   sPrintF(opColourCommand[3], "MOVIE colour:%i Gray", win);
//   opColourCommand[4] = "";
//   aString opColourLabel[] = {"8 bit colour", 
// 			     "24 bit colour", 
// 			     "Black & White", 
// 			     "Gray scale", 
// 			     ""};
//   int stdColour = outputFormat[win];
//   mov.addOptionMenu( "Colour", opColourCommand, opColourLabel, stdColour); 

// // text labels
//   aString textCommands[13];
//   int q=0;
//   sPrintF(textCommands[q++], "MOVIE resolution:%i", win);
//   sPrintF(textCommands[q++], "MOVIE horizontal resolution:%i", win);
//   sPrintF(textCommands[q++], "MOVIE base name:%i", win);
//   sPrintF(textCommands[q++], "MOVIE number of frames:%i", win);
//   sPrintF(textCommands[q++], "MOVIE starting frame:%i", win);
//   sPrintF(textCommands[q++], "MOVIE delta xr:%i", win);
//   sPrintF(textCommands[q++], "MOVIE delta yr:%i", win);
//   sPrintF(textCommands[q++], "MOVIE delta zr:%i", win);
//   sPrintF(textCommands[q++], "MOVIE delta x:%i", win);
//   sPrintF(textCommands[q++], "MOVIE delta y:%i", win);
//   sPrintF(textCommands[q++], "MOVIE delta z:%i", win);
//   sPrintF(textCommands[q++], "MOVIE relative zoom:%i", win);
//   textCommands[q++] = "";
//   aString textLabels[] = {"Vertical Resolution", "Horizontal Resolution", "Base name", "# of Frames", 
// 			  "First frame", "Delta x-rot", "Delta y-rot", "Delta z-rot", 
// 			  "Delta x-trans", "Delta y-trans", "Delta z-trans", 
// 			  "Relative zoom", ""};
//   aString textStrings[13];
//   q=0;
  
//   sPrintF(textStrings[q++], "%i", rasterResolution[win]);
//   sPrintF(textStrings[q++], "%i", horizontalRasterResolution[win]);
//   textStrings[q++] = movieBaseName[win];
//   sPrintF(textStrings[q++], "%i", numberOfMovieFrames[win]);
//   sPrintF(textStrings[q++], "%i", movieFirstFrame[win]);
//   sPrintF(textStrings[q++], "%g", movieDxRot[win]);
//   sPrintF(textStrings[q++], "%g", movieDyRot[win]);
//   sPrintF(textStrings[q++], "%g", movieDzRot[win]);
//   sPrintF(textStrings[q++], "%g", movieDxTrans[win]);
//   sPrintF(textStrings[q++], "%g", movieDyTrans[win]);
//   sPrintF(textStrings[q++], "%g", movieDzTrans[win]);
//   sPrintF(textStrings[q++],"%g", movieRelZoom[win]);
  
//   textStrings[q++] = "";
//   mov.setTextBoxes(textCommands, textLabels, textStrings);

//   mov.setBuiltInDialog(); // indicate that the commands in this dialog are parsed in processSpecialMenuItems
//   mov.openDialog(0); // create the Motif widgets, but don't display the dialog yet.
// }

void GL_GraphicsInterface::
setupHardCopy(DialogData &hcd, int win)
{
  aString cc;
  sPrintF(cc,"hardcopy close dialog:%i", win);
  hcd.setExitCommand(cc, "Close");
  aString dTitle;
  sPrintF(dTitle, "Hardcopy:%i", win);
  hcd.setWindowTitle(dTitle);

  aString pbCommands[3];
  sPrintF(pbCommands[0], "hardcopy save:%i", win);
  sPrintF(pbCommands[1], "hardcopy browse:%i", win);
  pbCommands[2] = "";
  
  aString pbLabels[] = {"Save", "Browse...", ""};
  hcd.setPushButtons( pbCommands, pbLabels, 1);

// define layout of option menus
  hcd.setOptionMenuColumns(1);

  aString opFormatCommand[5];
  sPrintF(opFormatCommand[0], "hardcopy format:%i PS", win);
  sPrintF(opFormatCommand[1], "hardcopy format:%i EPS", win);
  sPrintF(opFormatCommand[2], "hardcopy format:%i RasterPS", win);
  sPrintF(opFormatCommand[3], "hardcopy format:%i ppm", win);
  opFormatCommand[4] = "";
  aString opFormatLabel[] = {"PS", 
			     "EPS", 
			     "PS raster", 
			     "ppm", 
			     ""};
  int stdFormat = hardCopyType[win];
// initial choice: element stdFormat
  hcd.addRadioBox( "Format", opFormatCommand, opFormatLabel, stdFormat, 2); 

  aString opColourCommand[5];
  sPrintF(opColourCommand[0], "hardcopy colour:%i 8bit", win);
  sPrintF(opColourCommand[1], "hardcopy colour:%i 24bit", win);
  sPrintF(opColourCommand[2], "hardcopy colour:%i BW", win);
  sPrintF(opColourCommand[3], "hardcopy colour:%i Gray", win);
  opColourCommand[4] = "";
  aString opColourLabel[] = {"8 bit colour", 
			     "24 bit colour", 
			     "Black & White", 
			     "Gray scale", 
			     ""};
  int stdColour = outputFormat[win];
  hcd.addOptionMenu( "Colour", opColourCommand, opColourLabel, stdColour); 

  // these are currently independent of the window number:
  aString renderingCommand[]={"hardcopy rendering:0 offScreen", 
                              "hardcopy rendering:0 frameBuffer",
                              ""};
  aString renderingLabel[] = {"offScreen",
                              "frameBuffer",
                              "" };
  hcd.addOptionMenu( "rendering", renderingCommand, renderingLabel, (int)hardCopyRenderingType); 

// text labels
  aString textCommands[4];
  int q=0;
  sPrintF(textCommands[q++], "hardcopy vertical resolution:%i", win);
  sPrintF(textCommands[q++], "hardcopy horizontal resolution:%i", win);
  sPrintF(textCommands[q++], "hardcopy file name:%i", win);
  textCommands[q++] = "";
  aString textLabels[] = {"Vertical Resolution", "Horizontal Resolution", "File name", ""};

  aString textStrings[4];
  q=0;
  sPrintF(textStrings[q++], "%i", rasterResolution[win]);
  sPrintF(textStrings[q++], "%i", horizontalRasterResolution[win]);
  textStrings[q++] = hardCopyFile[win];
  textStrings[q++] = "";
  hcd.setTextBoxes(textCommands, textLabels, textStrings);

  hcd.setBuiltInDialog(); // indicate that the commands in this dialog are parsed in processSpecialMenuItems
  hcd.openDialog(0); // create the Motif widgets
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{beginRecordDisplayLists}} 
int GL_GraphicsInterface::
beginRecordDisplayLists( IntegerArray & displayLists)
// ================================================================================
// /Description:
//    Record the display list numbers that are allocated from now until
// a call to endRecordDisplayLists. This list could be used, for example, to selectively
// delete items that were drawn.
//
// /displayLists (input) : save display list numbers in this array. The array will
//  be automatically redimensioned to hold the numbers.
//\end{GL_GraphicsInterfaceInclude.tex} 
// ================================================================================
{
  if( recordDisplayLists==NULL )
  {
    numberRecorded=0;
    recordDisplayLists=&displayLists;
  }
  return 0;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{endRecordDisplayLists}} 
int GL_GraphicsInterface::
endRecordDisplayLists( IntegerArray & displayLists)
// ====================================================================================
// /Description:
//    Stop recording the display list numbers. 
// 
// /displayLists (output) : The same array passed when calling beginRecordDisplayLists.
// On output this array will hold the display list numbers, it will be exactly the correct size.
// 
//\end{GL_GraphicsInterfaceInclude.tex} 
// ====================================================================================
{
  if( recordDisplayLists==&displayLists )
  {
    (*recordDisplayLists).resize(numberRecorded);
    recordDisplayLists=NULL;
  }
  return 0;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{pause}} 
int GL_GraphicsInterface::
pause()
// ---------------------------------------------------------------------------------
// /Description:
//   Pause and wait for a response: "continue" or "break"
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// --------------------------------------------------------------------------------
{
  if( ignorePause ) 
    return 0;
  
  aString answer;

  GUIState interface;
  aString commands[] = { "break", "" };
  aString labels[]   = { "Break", "" };
  interface.setPushButtons( commands, labels, 1 ); 
  interface.setWindowTitle("Pause");
  interface.setExitCommand("continue", "Continue");
  pushGUI(interface);

  // We need to stop reading from the command file
  getInteractiveResponse=true;  // force an interactive response
  
  getAnswer(answer,"pause");

  getInteractiveResponse=false;  // reset
  
  popGUI();
  
  if( answer=="break" )
  {
    stopReadingCommandFile();

    // only open the GUI if the graphics window is up
    if (graphicsWindowIsOpen)
      openGUI();
  }
  return 0;
}




#undef C


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{drawColouredSquares}} 
void GL_GraphicsInterface::
drawColouredSquares(const IntegerArray & numberList,   // AP changed to a reference
                    GraphicsParameters & parameters,
                    const int & numberOfColourNames_ /* = -1 */ ,
                    aString *colourNames_ /* = NULL */)
//--------------------------------------------------------------------------
// /Description:
// Draw a coloured square with the number inside it for each of the colours
// shown on the plot 
//
// Input -
//   numberList : a list of numbers that should be labeled. The numbers
//                may appear more than once in the list and they need not
//                be ordered
// /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//--------------------------------------------------------------------------
{
  if( !isGraphicsWindowOpen() ) return;
  
  const int maxNumberOfSquaresPerLine=24;  // this cannot be increased without decreasing the box size
  const int maxNumberOfLines=2;

  glNewList(getColouredSquaresDL(currentWindow),GL_COMPILE);

  glShadeModel(GL_FLAT);     // flat shading

  real size=.025;  // .015;
  int number=min(numberList.getBound(0),maxNumberOfSquaresPerLine*maxNumberOfLines-1);
  char buff[100];
  int numberOfColours;
  aString *cNames;
  if( numberOfColourNames_ > 0 )
  {
    numberOfColours=numberOfColourNames_;
  }
  else
  { // use default from the class
    numberOfColours=numberOfColourNames;
  }
  if( colourNames_!=NULL )
  {
    cNames=colourNames_;
  }
  else
  { // use default from the class
    cNames=colourNames;
  }
  real width=.075;   // width of the squares
#ifndef NO_APP
  int maxNum=max(numberList);
#else
  int maxNum=-1000000;
  for (int q=0; q<numberList.getLength(0); q++)
    maxNum = max(maxNum, numberList(q));
#endif
  if( maxNum>1000 )
  {
    cout << "GL_GraphicsInterface::drawColouredSquares: ERROR? max(numberList>1000, setting=1000 \n";
    maxNum=1000;
  }
// translate in the display function instead!
  real yStart= maxNum<19 ? 
    0.25 : width;
  real xStart= maxNum<maxNumberOfSquaresPerLine-1 ? 
    - 1.5*width : - 2.5*width;

  const real zpos = .99-.01;   // labels are at .99, move squares a bit below ** fix this **
  
//    real yStart= maxNum<19 ? 
//      bottom[currentWindow] + 0.25 : bottom[currentWindow] + width;
//    real xStart= maxNum<maxNumberOfSquaresPerLine-1 ? 
//      rightSide[currentWindow] - 1.5*width : rightSide[currentWindow] - 2.5*width;
  int count=0;
  for( int i=0; i<=maxNum; i++ )  // this is inefficient, should sort the list
  {
    for( int n=0; n<=number; n++ )
    {
      if( i==numberList(n) )  // display colour i if it is in the list
      { // start a new column of numbers
        if( count==maxNumberOfSquaresPerLine-1 )
	{
	  xStart+=width*1.1;
          count=0;
	}
	setXColour(cNames[(i%numberOfColours)]);
	glBegin(GL_POLYGON);
	real x0=xStart,                  x1=x0+width;
	real y0=yStart+count*width*1.1,  y1=y0+width;
	glVertex3(x0,y0,zpos);   // lower square so label appears on top
	glVertex3(x1,y0,zpos);
	glVertex3(x1,y1,zpos);
	glVertex3(x0,y1,zpos);
	glEnd();	
	glColor3(1.,1.,1.);   // set label to white
	label(sPrintF(buff,"%i",i),.5*(x0+x1),.5*(y0+y1),size,0,0.,parameters);
	count++;
	break;
      }
    }
  }
  glEndList();
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{stopReadingCommandFile}} 
void GL_GraphicsInterface::
stopReadingCommandFile()
// ---------------------------------------------------------------------------------------------
// /Description:
// Stop reading the command file (and close the file). Open the GUI if the graphics
// window is open
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( readFile )
  {
    fclose(readFile);
    while( !readFileStack.empty() )
    {
      fclose(readFileStack.top());
      readFileStack.pop();
    }    
  }

  readFile=NULL;

  // kkc 080106 also clear the string command queue
  while ( !stringCommands.empty() ) stringCommands.pop();

  if( abortProgramIfCommandFileEnds )
  {
    printF("The command file has ended and abortProgramIfCommandFileEnds==true.\n"
           "The program will now abort on purpose.\n");
    OV_ABORT("abort");
  }

// need to open the GUI if the file ended, but
// only if the graphics window is up
  if (graphicsWindowIsOpen)
    openGUI();
  
}

