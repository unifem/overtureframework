#include "PlotStuff.h"
#include "GL_GraphicsInterface.h"
#include <GL/glu.h>
#include <GL/glut.h>

PlotStuffParameters defaultPlotStuffParameters(TRUE);

aString GL_GraphicsInterface::colourNames[GL_GraphicsInterface::numberOfColourNames]
     = { "blue",
         "green",
         "red",
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
	 };

void setXColour( const aString & xColourName );
void getXColour( const aString & xColourName, RealArray & rgb);

void 
label(const aString & string,     
      const real xPosition, 
      const real yPosition,
      const real size=.1,
      const int centering=0, 
      const real angle=0. );     // angle in degrees

extern "C" 
{
  void wdhMainLoop(int *doneWithGlut );
}

static const int maximumNumberOfDisplayLists=200;

static real xEyeCoordinate=0., yEyeCoordinate=0., zEyeCoordinate=5.; // 5.  // position of the eye
static real xAngle=0., yAngle=0., zAngle=0.;
static real dtx=0., dty=0., dtz=0.;
static real xShift=0., yShift=0., zShift=0.;
static real magnificationFactor=1., magnificationIncrement=1.25;
static real deltaX=.05, deltaY=.05, deltaZ=.05, deltaAngle=10.;

static int xZoomStart, yZoomStart, xZoomEnd, yZoomEnd;  // for rubber band zoom
 
/* GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0}; */
static GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

static int win1, win2, submenu1, submenu2;
 
static int menu1,menu2,menuContour,menuOld;

static int list = 1;

static RealArray rotationMatrix(4,4), matrix(4,4);
static Index I4(0,4);

void
display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // glTranslatef(0.0, 0.0, -1.0);    // is this needed?

  gluLookAt(xEyeCoordinate,yEyeCoordinate,zEyeCoordinate,  /* eye is at (0,-5,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  glScalef(magnificationFactor,magnificationFactor,magnificationFactor);
  // Translate the origin --- this changes the point we rotate about
  glTranslatef(xShift,yShift,zShift);   


  // Here are the rotations -- In order to incrementally rotate about
  // the FIXED axes we keep a rotation matrix that we pre-multiply
  // with any incremental rotation

  if( dtx!=0. )
  {
    real cx=cos(dtx*Pi/180.), sx=sin(dtx*Pi/180.);
    matrix(1,I4)=cx*rotationMatrix(1,I4)-sx*rotationMatrix(2,I4);
    matrix(2,I4)=sx*rotationMatrix(1,I4)+cx*rotationMatrix(2,I4);
    rotationMatrix(1,I4)=matrix(1,I4); rotationMatrix(2,I4)=matrix(2,I4);
  }
  else if( dty!=0. )
  {
    real cy=cos(-dty*Pi/180.), sy=sin(-dty*Pi/180.);   // NOTE minus sign
    matrix(0,I4)=cy*rotationMatrix(0,I4)-sy*rotationMatrix(2,I4);
    matrix(2,I4)=sy*rotationMatrix(0,I4)+cy*rotationMatrix(2,I4);
    rotationMatrix(0,I4)=matrix(0,I4); rotationMatrix(2,I4)=matrix(2,I4);
  }
  else if( dtz!=0. )
  {
    real cz=cos(dtz*Pi/180.), sz=sin(dtz*Pi/180.);
    matrix(0,I4)=cz*rotationMatrix(0,I4)-sz*rotationMatrix(1,I4);
    matrix(1,I4)=sz*rotationMatrix(0,I4)+cz*rotationMatrix(1,I4);
    rotationMatrix(0,I4)=matrix(0,I4); rotationMatrix(1,I4)=matrix(1,I4);
  }
  glMultMatrixf( &rotationMatrix(0,0) );
  dtx=dty=dtz=0.;

  glClearColor(1.,1.,1.,1.);   // back ground colour
  
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLbitfield arg = GLbitfield(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(arg);
  // ****** render these display lists *******      ************** fix this **********
  for( int i=1; i<maximumNumberOfDisplayLists; i++ )
   glCallList(i);      // render contour display list 

  glutSwapBuffers();
}

void
idle(void)
{
}

void
delayed_stop(int value)
{
  glutIdleFunc(NULL);
}


static int rubberBandList;
void
init(void)
{

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  real left=-1., right=1., bottom=-1., top=1., near=1., far=10.;   // here is the screen size
  glOrtho( left,right,bottom,top,near,far );       

  glMatrixMode(GL_MODELVIEW);

  rotationMatrix=0.;
  rotationMatrix(0,0)=1.;   rotationMatrix(1,1)=1.;   
  rotationMatrix(2,2)=1.;   rotationMatrix(3,3)=1.; 

//  GLfloat light_ambient[] =  {1.0, 1.0, 1.0, .25};
  GLfloat light_ambient[] =  {.25,.25,.25,1.};  // {.5,.5,.5,1.};
  GLfloat light_diffuse[] =  {1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
  
  // here is light 0
  GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  // last value zero means light is at infinity
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // Here is light 1
//  GLfloat light_position1[] = {-1.0, -1.0, -1.0, 0.0}; 
  GLfloat light_position1[] = {-1.0, -1.0, +1.0, 0.0}; 
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

/* ---
  // Here is light 2
  GLfloat light_position2[] = {-1.0, -1.0, +1.0, 0.0}; 
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT2, GL_POSITION, light_position1);

  // Here is light 3  
  GLfloat light_position3[] = {1.0, 1.0, -1.0, 0.0}; 
  glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT3, GL_POSITION, light_position1);
---- */

}

void
menustate(int inuse)
{
  // printf("menu is %s\n", inuse ? "INUSE" : "not in use");
}

void
keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'x':
     dtx=deltaAngle; // xAngle+=deltaAngle;
     glutPostRedisplay();
    break;
  case 'y':
     dty=deltaAngle; // yAngle+=deltaAngle;
     glutPostRedisplay();
    break;
  case 'z':
     dtz=deltaAngle; // zAngle+=deltaAngle;
     glutPostRedisplay();
    break;
  case 'X':
     dtx=-deltaAngle; // xAngle-=deltaAngle;
     glutPostRedisplay();
    break;
  case 'Y':
     dty=-deltaAngle; // yAngle-=deltaAngle;
     glutPostRedisplay();
    break;
  case 'Z':
     dtz=-deltaAngle; // zAngle-=deltaAngle;
     glutPostRedisplay();
    break;
  case 'l':
     xShift-=deltaX;
     glutPostRedisplay();
    break;
  case 'r':
     xShift+=deltaX;
     glutPostRedisplay();
    break;
  case 'u':
     yShift+=deltaY;
     glutPostRedisplay();
    break;
  case 'd':
     yShift-=deltaY;
     glutPostRedisplay();
    break;
  case 'f':
     zShift+=deltaZ;
     glutPostRedisplay();
    break;
  case 'b':
     zShift-=deltaZ;
     glutPostRedisplay();
    break;
  case 'M':
     magnificationFactor*=magnificationIncrement;
     glutPostRedisplay();
    break;
  case 'm':
     magnificationFactor/=magnificationIncrement;
     glutPostRedisplay();
    break;
  case 'R':   //  ==== reset ======
     xShift=0.; 
     xAngle=0.; yAngle=0.; zAngle=0.;
     dtx=0.; dty=0.; dtz=0.;
     xShift=0.; yShift=0.; zShift=0.;
     magnificationFactor=1.; magnificationIncrement=1.25;
     deltaX=.05; deltaY=.05; deltaZ=.05; deltaAngle=10.;
     rotationMatrix=0.;
     rotationMatrix(0,0)=1.;   rotationMatrix(1,1)=1.;   
     rotationMatrix(2,2)=1.;   rotationMatrix(3,3)=1.; 
     glutPostRedisplay();
    break;
  }    
//  if(isprint(key)) {
//    printf("key: `%c' %d,%d\n", key, x, y);
//  } else {
//    printf("key: 0x%x %d,%d\n", key, x, y);
//  }
}


void
special(int key, int x, int y)
{
   char *name;

   switch(key) {
   case GLUT_KEY_F1: name = "F1"; break;
   case GLUT_KEY_F2: name = "F2"; break;
   case GLUT_KEY_F3: name = "F3"; break;
   case GLUT_KEY_F4: name = "F4"; break;
   case GLUT_KEY_F5: name = "F5"; break;
   case GLUT_KEY_F6: name = "F6"; break;
   case GLUT_KEY_F7: name = "F7"; break;
   case GLUT_KEY_F8: name = "F8"; break;
   case GLUT_KEY_F9: name = "F9"; break;
   case GLUT_KEY_F10: name = "F11"; break;
   case GLUT_KEY_F11: name = "F12"; break;
   case GLUT_KEY_LEFT: 
     name = "Left"; break;
   case GLUT_KEY_UP: 
     name = "Up"; 
     break;
   case GLUT_KEY_RIGHT: 
     name = "Right"; break;
   case GLUT_KEY_DOWN: 
     name = "Down";
     break;
   case GLUT_KEY_PAGE_UP: name = "Page up"; break;
   case GLUT_KEY_PAGE_DOWN: name = "Page down"; break;
   case GLUT_KEY_HOME: name = "Home"; break;
   case GLUT_KEY_END: name = "End"; break;
   case GLUT_KEY_INSERT: name = "Insert"; break;
   default: name = "UNKNOWN"; break;
   }
   // printf("special: %s %d,%d\n", name, x, y);
}

static int rubberBand=FALSE;

void
mouse(int button, int state, int x, int y)
{
 // printf("button: %d %s %d,%d\n", button, state == GLUT_UP ? "UP" : "down", x, y);
 if( state!=GLUT_UP )
 {
   rubberBand=1;  // tells motion function to draw a rubber band
   rubberBandList=glGenLists(1);  // get a new display list to use for the rubber band box
   xZoomStart=x;
   yZoomStart=y;
   // printf(" start at (%i,%i), x0=%e, y0=%e\n",x,y,x0,y0);
 }
 else
 {
   rubberBand=0;
   xZoomEnd=x;
   yZoomEnd=y;
   if( abs(xZoomEnd-xZoomStart)+abs(yZoomEnd-yZoomStart) > 50 )
   { // zoom if the window is large enough
     // printf(" end at (%i,%i)\n",x,y);
     // shift by 2*( average position ) -1
     real w=glutGet((GLenum)GLUT_WINDOW_WIDTH);
     real h=glutGet((GLenum)GLUT_WINDOW_HEIGHT);
     xShift-=(real(xZoomStart+xZoomEnd)/w-1.)/magnificationFactor;  // window is [-1,1]
     yShift-=(1.-real(yZoomStart+yZoomEnd)/h)/magnificationFactor;
     magnificationFactor/=max(.005,max(real(abs(xZoomEnd-xZoomStart))/w,
				       real(abs(yZoomEnd-yZoomStart))/h));
     
   }
   glDeleteLists(rubberBandList,1); // delete rubber band list
   glutPostRedisplay();
 }
}

static real xBand,yBand;

void
motion(int x, int y)
{
  if( rubberBand>0 )
  {
    // printf("motion:rubberBand: %d,%d\n", x, y);
    real w=glutGet((GLenum)GLUT_WINDOW_WIDTH);
    real h=glutGet((GLenum)GLUT_WINDOW_HEIGHT);

    glDeleteLists(rubberBandList,1);
    glNewList(rubberBandList,GL_COMPILE);

    glColor3f(.1,.1,.1);
    glBegin(GL_LINE_LOOP);
    xBand=x;  yBand=y;
    real & mag = magnificationFactor;
    glVertex3f((2.*xZoomStart/w-1.)/mag-xShift,(1.-2.*yZoomStart/h)/mag-yShift,1.1);
    glVertex3f((2.*xBand     /w-1.)/mag-xShift,(1.-2.*yZoomStart/h)/mag-yShift,1.1);
    glVertex3f((2.*xBand     /w-1.)/mag-xShift,(1.-2.*yBand     /h)/mag-yShift,1.1);
    glVertex3f((2.*xZoomStart/w-1.)/mag-xShift,(1.-2.*yBand     /h)/mag-yShift,1.1);
    glEnd();

/* -----  
    // this didn't seem to work very well
    glBlendEquationEXT(GL_LOGIC_OP);
    glEnable(GL_LOGIC_OP);
    glEnable(GL_BLEND);
    glLogicOp(GL_XOR);
    glColor3f(.2,.2,.2);

    if( rubberBand>1 )  // redraw to erase
    {
      glBegin(GL_LINE_LOOP);
      glVertex2f(2.*xZoomStart/w-1.,1.-2.*yZoomStart/h);
      glVertex2f(2.*xBand     /w-1.,1.-2.*yZoomStart/h);
      glVertex2f(2.*xBand     /w-1.,1.-2.*yBand     /h);
      glVertex2f(2.*xZoomStart/w-1.,1.-2.*yBand     /h);
      glEnd();
    }
    rubberBand++;
    xBand=x;  yBand=y;

    glBegin(GL_LINE_LOOP);
    glVertex2f(2.*xZoomStart/w-1.,1.-2.*yZoomStart/h);
    glVertex2f(2.*xBand     /w-1.,1.-2.*yZoomStart/h);
    glVertex2f(2.*xBand     /w-1.,1.-2.*yBand     /h);
    glVertex2f(2.*xZoomStart/w-1.,1.-2.*yBand     /h);
    glEnd();

    glFlush();
    glDisable(GL_BLEND);
    glDisable(GL_LOGIC_OP);
----- */

    glEndList(); 
    glutPostRedisplay();
  }
}

void
visible(int status)
{
  // printf("visible: %s\n", status == GLUT_VISIBLE ? "YES" : "no");
}




bool doneWithGlut=FALSE;
int menuSelected;

extern "C"
{
  void 
    PSMenuCallBack(int value)
    {
      menuSelected=value;
      doneWithGlut=TRUE;   // set this so we exit the event loop
    }
}



//\begin{>GL_GraphicsInterfaceInclude.tex}{\subsection{Constructors}} 
GL_GraphicsInterface::
GL_GraphicsInterface()
//=====================================================================================
// /Description:
//   Default constructor; the default constructor will open a window
// /Author: WDH
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//=====================================================================================
{
  int argc=1;
  char *argv[] = {"plot",NULL}; 
  constructor(argc,argv);
}  

//\begin{>>GL_GraphicsInterfaceInclude.tex}{} 
GL_GraphicsInterface::
GL_GraphicsInterface(int & argc, char *argv[])
//=====================================================================================
// /Description:
//   This Constructor takes the argc and argv from the main program -- The GLUT 
//   window manager will strip off any parameters that it recognizes such as the
//   size of the window. See the GLUT manual for further details.
//
// /argc (input/output): The argument count to main.
// /argv (input/output): The arguments to main.
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//=====================================================================================
{
  constructor(argc,argv);
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{} 
GL_GraphicsInterface::
GL_GraphicsInterface( const bool initialize )
//-------------------------------------------------------------------------------------
// /Description:
//    This Constructor will only create a window if the the argument initialize=TRUE
//    To create a window later call createWindow()
//
// /initialize (input): If TRUE then a window will be created. If FALSE no window will
//         be created and you will have to call {\ff createWindow} to make the window.
//
// /Author: WDH.  
//\end{GL_GraphicsInterfaceInclude.tex} 
//-------------------------------------------------------------------------------------
{
  if( initialize )
    createWindow();
  else
    graphicsWindowIsOpen=FALSE; 
}  

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{createWindow}} 
int GL_GraphicsInterface::
createWindow()
//----------------------------------------------------------------------
// /Description:
//   Open the window (but only if one is not already open)
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( !graphicsWindowIsOpen )
  {
    int argc=1;
    char *argv[] = {"plot",NULL}; 
    constructor(argc,argv);
  }
  return 0;
}  

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{destroyWindow}} 
int GL_GraphicsInterface::
destroyWindow()
//----------------------------------------------------------------------
// /Description:
//   destroy the window 
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  return 0;
}  

void GL_GraphicsInterface:: 
constructor(int & argc, char *argv[])
{
  graphicsWindowIsOpen=TRUE;    // indicates that the window has been opened

  readFile=NULL;
  saveFile=NULL;
  lighting=FALSE;  // lighting is off initially

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // ------create a window-----
  int win1 = glutCreateWindow("GL_GraphicsInterface");
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutVisibilityFunc(visible);
  init();

  glutDisplayFunc(display);  // set the call back routine for re-drawing the screen
  glutMenuStateFunc(menustate);
}

GL_GraphicsInterface::
~GL_GraphicsInterface()
{
}

int getMatch(const aString *menu, aString & answer);  // from GenericGraphicsInterface.C

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getMenu}} 
int GL_GraphicsInterface::
getMenuItem(const aString *menu,
            aString & answer, 
            const aString & prompt /* =nullString */ )
//----------------------------------------------------------------------
// /Description:
//    Create a menu and return with a menu item. Display an optional prompt.
//
// /menu (input):
//    The {\ff menu} is
//    an array of aString's (the menu choices) with a empty aString
//    indicating the end of the menu choices. For example
//    \begin{verbatim}
//       PlotStuff ps;
//       aString menu[] = { "plot",
//                         "erase",
//                         "exit",
//                         "" };
//       aString menuItem;
//       int i=ps.getMenuItem(menu,menuItem);
//    \end{verbatim}
// /answer (output): Return the chosen item.
//
// /prompt (input): display the optional prompt message
//
//  /Errors:  Some...
//
//  /Return Values: On return "menuItem" is set equal to the 
//    menu item chosen. The function return value is set equal to
//    the number of the item chosen, starting from zero.
//    Thus, for example, in the above menu if the user picked "erase"
//    the return value would be 1, if the user picked "plot" the
//    return value would be 0.
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  // cout << "getMenuItem...\n";
  if( readFile || stringCommands )
  {
    readLineFromCommandFile(answer);   // this will save in a command file if open
    // strip off leading blanks
    aString line=answer;
    for( int i=0; i<line.length() && line[i]==' '; i++) {}
    if( i>0 )
      answer=line(i,line.length()-1);
    menuSelected=getMatch(menu,answer);   // find the best match, if any
  
    if( menuSelected==-1 )
    {
      if( readFile )
      {
        fclose(readFile);
        readFile=NULL;    // fall through to the interactive menu
      }
    }
    else
      return menuSelected;
  }

  if( prompt!=nullString )
    outputString(prompt);
  glutCreateMenu(PSMenuCallBack);
  for( int i=0; menu[i]!=""; i++ )
  {
    // cout << "Add menu " << menu[i] << endl;
    glutAddMenuEntry((char *)menu[i],i);
  }
  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutPostRedisplay();   // ***

  // --------sit in the event loop until a menu item is chosen----------------
  doneWithGlut=FALSE;
  wdhMainLoop(&doneWithGlut);    // *** this sets the variable menuSelected ****
//  glutMainLoop();
  doneWithGlut=FALSE;   // reset this value
  answer=menu[menuSelected];

  if( saveFile )
    fprintf(saveFile,"%s\n",(const char*)answer);
 
  return menuSelected;
}

  
void GL_GraphicsInterface::
erase()
{
  glDeleteLists(1,maximumNumberOfDisplayLists);
  glutPostRedisplay();
}

//
// Input a string after displaying an optional prompt
//
void GL_GraphicsInterface::
inputString(aString & answer, const aString & prompt)
{
  GenericGraphicsInterface::inputString(answer,prompt); 
}

//
// Redraw  all graphics display lists
//
void GL_GraphicsInterface::
redraw(bool immediate)
{
  glutPostRedisplay();  
/* ---
  doneWithGlut=TRUE;
  for( int i=0; i<100; i++ )
    wdhMainLoop(&doneWithGlut);    // Enter event loop so the screen is redrawn
  doneWithGlut=FALSE;   // reset this value
--- */
}

//
// Reset the view to default
//
void GL_GraphicsInterface::
resetView()
{
 xEyeCoordinate=0., yEyeCoordinate=0., zEyeCoordinate=5.; // 5.  // position of the eye
 xAngle=0., yAngle=0., zAngle=0.;
 dtx=0., dty=0., dtz=0.;
 xShift=0., yShift=0., zShift=0.;
 magnificationFactor=1., magnificationIncrement=1.25;
 deltaX=.05, deltaY=.05, deltaZ=.05, deltaAngle=10.;
 init();
}

//
// This routine sets the projection and modelview to a normalized system on [-1,1]x[-1,1]
//
void GL_GraphicsInterface:: 
setNormalizedCoordinates()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  real left=-1., right=1., bottom=-1., top=1.;
  gluOrtho2D( left,right,bottom,top );
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
}

//
// This routine un-does the previous function
//
void GL_GraphicsInterface:: 
unsetNormalizedCoordinates()
{
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}


// 
//  choose a colour
//
aString GL_GraphicsInterface::
chooseAColour()
{
  aString answer;
  aString *menu = new aString[39+1];
  int i=0;
  menu[i++]="no change";
  menu[i++]="emerald";
  menu[i++]="jade";
  menu[i++]="obsidian";
  menu[i++]="pearl";
  menu[i++]="ruby";
  menu[i++]="turquoise";
  menu[i++]="brass";
  menu[i++]="bronze";
  menu[i++]="chrome";
  menu[i++]="copper";
  menu[i++]="gold";
  menu[i++]="silver";
  menu[i++]="blue",
  menu[i++]="green",
  menu[i++]="red",
  menu[i++]="coral",
  menu[i++]="violetred",
  menu[i++]="darkturquoise",
  menu[i++]="steelblue",
  menu[i++]="orange",
  menu[i++]="orchid",
  menu[i++]="navyblue",
  menu[i++]="salmon",
  menu[i++]="yellow",
  menu[i++]="aquamarine",
  menu[i++]="mediumgoldenrod",
  menu[i++]="darkgreen",
  menu[i++]="wheat",
  menu[i++]="seagreen",
  menu[i++]="khaki",
  menu[i++]="maroon",
  menu[i++]="skyblue",
  menu[i++]="slateblue",
  menu[i++]="darkorchid",
  menu[i++]="plum",
  menu[i++]="violet",
  menu[i++]="pink",
  menu[i++]="clear";
  menu[i++]="";   // null string terminates the menu
  getMenuItem(menu,answer);
  delete [] menu;
  return answer;
}

// utility routine
void
setMaterialProperties(
  float ambr, float ambg, float ambb,
  float difr, float difg, float difb,
  float specr, float specg, float specb, float shine)
{
  glDisable(GL_COLOR_MATERIAL);

  float mat[4];
  float factor=1.1;  // 1.25;  // wdh

  mat[0] = ambr*factor;
  mat[1] = ambg*factor;
  mat[2] = ambb*factor;
  mat[3] = 1.0;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat);
  mat[0] = difr*factor;
  mat[1] = difg*factor;
  mat[2] = difb*factor;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat);
  mat[0] = specr*factor;
  mat[1] = specg*factor;
  mat[2] = specb*factor;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine * 128.0);
}

// 
//  set the colour for subsequent objects that are plotted
//
int  GL_GraphicsInterface::
setColour( const aString & name )
{
  int returnValue=0;
  
  if( lighting )
  {
    glShadeModel(GL_SMOOTH);     // smooth shading
    // set default material properties
    float ambient[4] = { .4,.4,.4, 1. };  // { .2,.2,.2, 1. }; 
    float diffuse[4] = { .8,.8,.8, 1. }; 
    float specular[4]= { 1.,1.,1., 1. }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.);  // 50. bigger = sharper high-lite, [0,128]
    // set defaults for non fancy colours
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT);   // this causes the object to reflect 
                                                        // according to its colour
    glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE); 
    glEnable(GL_COLOR_MATERIAL);
  }
  else
  {
    glShadeModel(GL_FLAT);     // flat shading
    glDisable(GL_COLOR_MATERIAL);
  }
  
  if( name=="emerald" )
    setMaterialProperties(0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6);
  else if( name=="jade" )
    setMaterialProperties(0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 0.1);
  else if( name=="obsidian" )
    setMaterialProperties(0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 0.3);
  else if( name=="pearl" )
    setMaterialProperties(0.25, 0.20725, 0.20725, 1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 0.088);
  else if( name=="ruby" )
    setMaterialProperties(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 
                          0.626959, 0.6);
  else if( name=="turquoise" )
    setMaterialProperties(0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 0.1);
  else if( name=="brass" )
    setMaterialProperties(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 
                          0.941176, 0.807843, 0.21794872);
  else if( name=="bronze" )
    setMaterialProperties(0.2125, 0.1275, 0.054,
			  0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 0.2);
  else if( name=="chrome" )
    setMaterialProperties(0.25, 0.25, 0.25,
			  0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6);
  else if( name=="copper" )
    setMaterialProperties(0.19125, 0.0735, 0.0225,
			  0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 0.1);
  else if( name=="gold" )
    setMaterialProperties(0.24725, 0.1995, 0.0745,
			  0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4);
  else if( name=="silver" )
    setMaterialProperties(0.19225, 0.19225, 0.19225,
			  0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);
  else if( name=="blackPlastic" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.01, 0.01, 0.01,
			  0.50, 0.50, 0.50, .25);
  else if( name=="cyanPlastic" )
    setMaterialProperties(0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392,
			  0.50196078, 0.50196078, 0.50196078, .25);
  else if( name=="greenPlastic" )
    setMaterialProperties(0.0, 0.0, 0.0,
			  0.1, 0.35, 0.1, 0.45, 0.55, 0.45, .25);
  else if( name=="redPlastic" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.5, 0.0, 0.0,
			  0.7, 0.6, 0.6, .25);
  else if( name=="whitePlastic" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.55, 0.55, 0.55,
			  0.70, 0.70, 0.70, .25);
  else if( name=="yellowPlastic" )
    setMaterialProperties(0.0, 0.0, 0.0, 0.5, 0.5, 0.0,
			  0.60, 0.60, 0.50, .25);
  else if( name=="blackRubber" )
    setMaterialProperties(0.02, 0.02, 0.02, 0.01, 0.01, 0.01,
			  0.4, 0.4, 0.4, .078125);
  else if( name=="cyanRubber" )
    setMaterialProperties(0.0, 0.05, 0.05, 0.4, 0.5, 0.5,
			  0.04, 0.7, 0.7, .078125);
  else if( name=="greenRubber" )
    setMaterialProperties(0.0, 0.05, 0.0, 0.4, 0.5, 0.4,
			  0.04, 0.7, 0.04, .078125);
  else if( name=="redRubber" )
    setMaterialProperties(0.05, 0.0, 0.0, 0.5, 0.4, 0.4,
			  0.7, 0.04, 0.04, .078125);
  else if( name=="whiteRubber" )
    setMaterialProperties(0.05, 0.05, 0.05, 0.5, 0.5, 0.5,
			  0.7, 0.7, 0.7, .078125);
  else if( name=="yellowRubber" )
    setMaterialProperties(0.05, 0.05, 0.0, 0.5, 0.5, 0.4,
			  0.7, 0.7, 0.04, .078125);
  else
  {
    setXColour(name);   // assign some standard x-colours
  }
//  else
//  {
//    cout << "setColour:ERROR unknown colour = " << (const char*) name << endl;
//    returnValue=1;
//  }

  return returnValue;
}

void GL_GraphicsInterface:: 
turnOnLighting()
{
  lighting=TRUE;


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
//  glEnable(GL_LIGHT2);
//  glEnable(GL_LIGHT3);
  
  GLfloat lmodel_ambient[] = {.25, .25, .25, 1.0};  // global ambient light
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );  // treat front and back facing the same

}

void GL_GraphicsInterface:: 
turnOffLighting()
{
  lighting=FALSE;

  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{hardCopy (save a Postscript File)}} 
int GL_GraphicsInterface::
hardCopy(const aString & fileName, /* =nullString */
         const hardCopyTypes hardCopyType /* =postScript */ )
//----------------------------------------------------------------------
// /Description:
//    This routines saves the graphics window in hard-copy form.
//  /fileName (input): Optional name for the file to save the plot in. If no name is given then
//     the user is prompted for a name.
//  /hardCopyType (input): Only post-script is currently supported.
//  /Return Values: 1: unable to open the file
//
//  /Author: WDH
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  aString localFileName;


  if( fileName!="" && fileName!=" " )
    localFileName=fileName;
  else
  {
    outputString("Enter the name for the postscript file (ex. plot.ps)");
    inputString(localFileName);
  }
  FILE *file;
  file = fopen(localFileName,"w" );         
  if( file==NULL )
  {
    cout << "GL_GraphicsInterface::hardCopy:ERROR: unable to open the file: " << localFileName << endl;
    return 1;
  }

  GLint x=0,y=0;  // lower corner of image
  GLsizei width=glutGet((GLenum)GLUT_WINDOW_WIDTH), height=glutGet((GLenum)GLUT_WINDOW_HEIGHT);

  float *xBuffer = new float [width*height*3+1000];
  glReadPixels( x,y,width,height,GL_RGB,GL_FLOAT,xBuffer);  // read the frame buffer

  real llbx,llby,urbx,urby,scale,scaleFactor;

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
  }
  else
  {
    scaleFactor=h/height;
    scale=h;
    llbx=leftMargin*72.+.5*(w-width*scaleFactor);   // figure is narrow, centre it in the x-direction
    llby=bottomMargin*72.;
  }
  urbx=llbx+width*scaleFactor;
  urby=llby+height*scaleFactor;

  fprintf(file,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(file,"%%%%Creator: PlotStuff v1.0\n");
  fprintf(file,"%%%%Title: What a concept! \n");
  fprintf(file,"%%%%CreationDate: Fri Feb 29 12:34:56 1999 \n");
  fprintf(file,"%%%%Pages: 1 \n");
  fprintf(file,"%%%%Requirements: colorprinter \n");
  fprintf(file,"%%%%BoundingBox: %i %i %i %i \n",int(llbx+.5),int(llby+.5),int(urbx+.5),int(urby+.5));
  fprintf(file,"%%%%EndComments \n");
  fprintf(file,"/bufstr %i string def \n",3*width); 
  fprintf(file,"%%%%EndProlog \n");
  fprintf(file,"%%%%Page: 1 1 \n");
  fprintf(file,"gsave \n");
  fprintf(file,"%f %f translate \n",llbx,llby);
  fprintf(file,"%f %f scale \n",scale,scale);
  fprintf(file,"%i %i 8 \n",width,height);
  fprintf(file,"[%i 0 0 -%i 0 %i]\n",width,height,height);
  fprintf(file,"{currentfile bufstr readhexstring pop} bind \n");
  fprintf(file,"false 3 colorimage \n\n");

#define C(x) ( int((x*255)+.5)  )
  int numPerLine=30;           // print this many colours per line  
  int num = width*height*3;    // total number of colours
  int i=num-width*3;           // print colours from top to bottom!
  for( int k=0; k<(num+numPerLine-1)/numPerLine; k++)   // print this many lines 
  {
    for( int j=0; j<numPerLine; j++)  // print colours on a line 
    {
      fprintf(file,"%2.2X",C(xBuffer[i++]));
       if( (i % (width*3)) == 0 )
       {
          i-=2*(width*3);  // shift down a row
       }
    }
    fprintf(file,"\n");
  }
  delete [] xBuffer;
  fprintf(file,"grestore\n");
  fprintf(file,"showpage\n");
  fprintf(file,"%%%%Trailer\n");
  fclose(file);

  return 0;

}

#undef C
