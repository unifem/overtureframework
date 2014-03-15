#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <Xm/BulletinB.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeB.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <Xm/Command.h>
#include <Xm/Text.h>
#include <Xm/LabelG.h>
#include <Xm/Separator.h>
#include <Xm/ToggleBG.h>
#include <Xm/PushBG.h>
#include <Xm/List.h>
#include <X11/cursorfont.h>
 
#include <assert.h>

typedef float real;

#include "aString.H"
#include "mogl.h"    
#include <iostream.h>

#define MAX_PLANES 15

class GL_GraphicsInterface
{
};


// static int moving = 0;

struct {
    float           speed;	/* zero speed means not flying */
    GLfloat         red, green, blue;
    float           theta;
    float           x, y, z, angle;
} planes[MAX_PLANES];

#define v3f glVertex3f /* v3f was the short IRIS GL name for glVertex3f */

static float xShift[MAX_WINDOWS], yShift[MAX_WINDOWS], zShift[MAX_WINDOWS];
static float dtx[MAX_WINDOWS], dty[MAX_WINDOWS], dtz[MAX_WINDOWS];
static float magnificationFactor[MAX_WINDOWS];

//..Data for HardCopyDialogs
//....types from GraphicsParameters--> copied here.
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

HardCopyType hardCopyType[MAX_WINDOWS];
OutputFormat outputFormat[MAX_WINDOWS];

aString hardCopyFile[MAX_WINDOWS];
int rasterResolution[MAX_WINDOWS];
DialogData hardCopyDialog[MAX_WINDOWS];

static void
setupDialog(DialogData &dialogSpec);

extern 
aString& sPrintF(aString & s, const char *format, ...);



// from GL_GraphicsInterface::
static void setupHardCopy(DialogData &hcd, int win)
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
  sPrintF(opFormatCommand[2], "hardcopy format:%i Raster PS", win);
  sPrintF(opFormatCommand[3], "hardcopy format:%i ppm", win);
  opFormatCommand[4] = "";
  aString opFormatLabel[] = {"PS", 
			     "EPS", 
			     "PS raster", 
			     "ppm", 
			     ""};
  int stdFormat = hardCopyType[win];
// initial choice: element stdFormat
  hcd.addOptionMenu( "Format", opFormatCommand, opFormatLabel, stdFormat); 

  aString opColourCommand[5];
  sPrintF(opColourCommand[0], "hardcopy colour:%i 8 bit", win);
  sPrintF(opColourCommand[1], "hardcopy colour:%i 24 bit", win);
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

// text labels
  aString textCommands[3];
  sPrintF(textCommands[0], "hardcopy resolution:%i", win);
  sPrintF(textCommands[1], "hardcopy file name:%i", win);
  textCommands[2] = "";
  aString textLabels[] = {"Resolution", "File name", ""};
  aString textStrings[3];
  sPrintF(textStrings[0], "%i", rasterResolution[win]);
  textStrings[1] = hardCopyFile[win];
  textStrings[2] = "";
  hcd.setTextBoxes(textCommands, textLabels, textStrings);

  hcd.setBuiltInDialog(); // indicate that this dialog is parsed in processSpecialMenuItems
  hcd.openDialog(0); // create the Motif widgets
}


void 
changeView(GL_GraphicsInterface *giPointer,
	   const int & win_number,
           const float & dx,   
	   const float & dy , 
	   const float & dz,
	   const float & dThetaX=0.,
	   const float & dThetaY=0.,
	   const float & dThetaZ=0.,
	   const float & magnify=1. )
// =========================================================================
//this routine is called by mogl to change the view
//
//  dx,dy,dz : relative shift [-1,1]
//
// =========================================================================
{
//  printf("changeView called with dx=%f, dy=%f, dz=%f\n", dx, dy, dz);
  
  xShift[win_number] += dx;
  yShift[win_number] += dy;
  zShift[win_number] += dz;
  dtx[win_number] += -10*dThetaX;
  dty[win_number] += -10*dThetaY;
  dtz[win_number] += -10*dThetaZ;
  
  magnificationFactor[win_number] *= magnify;
// no need to redraw here since that is done from the calling routine
}


void 
displayM(GL_GraphicsInterface *gi, const int & win_number)
{
    GLfloat         red, green, blue;
    int             i;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(xShift[win_number], yShift[win_number], zShift[win_number]);
    glRotatef(dtx[win_number], 1.,0.,0.);
    glRotatef(dty[win_number], 0.,1.,0.);
    glRotatef(dtz[win_number], 0.,0.,1.);
    glScalef(magnificationFactor[win_number],magnificationFactor[win_number],magnificationFactor[win_number]);

    glClear(GL_DEPTH_BUFFER_BIT);
    /* paint black to blue smooth shaded polygon for background */
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0); // black
    v3f(-100, 100, -19); v3f(100, 100, -19);
    v3f(100, 20, -19);   v3f(-100, 20, -19); 
    v3f(-100, 20, -19); v3f(100, 20, -19);
    glColor3f(0.0, 0.0, 1.0); //blue
    v3f(100, -20, -19); v3f(-100, -20, -19);
    v3f(-100, -20, -19); v3f(100, -20, -19); 
    v3f(100, -100, -19); v3f(-100, -100, -19);
    glEnd();
    /* paint planes */
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_FLAT);
    for (i = 0; i < MAX_PLANES; i++)
	if (planes[i].speed != 0.0) {
	    glPushMatrix();
	        glTranslatef(planes[i].x, planes[i].y, planes[i].z);
	        glRotatef(290.0, 1.0, 0.0, 0.0);
	        glRotatef(planes[i].angle, 0.0, 0.0, 1.0);
	        glScalef(1.0 / 3.0, 1.0 / 4.0, 1.0 / 4.0);
	        glTranslatef(0.0, -4.0, -1.5);
	        glBegin(GL_TRIANGLE_STRIP);
	            /* left wing */
	            v3f(-7.0, 0.0, 2.0); v3f(-1.0, 0.0, 3.0);
	            glColor3f(red = planes[i].red, green = planes[i].green,
	                      blue = planes[i].blue);
	            v3f(-1.0, 7.0, 3.0);
	            /* left side */
	            glColor3f(0.6 * red, 0.6 * green, 0.6 * blue);
	            v3f(0.0, 0.0, 0.0); v3f(0.0, 8.0, 0.0);
	            /* right side */
	            v3f(1.0, 0.0, 3.0); v3f(1.0, 7.0, 3.0);
	            /* final tip of right wing */
	            glColor3f(red, green, blue);
	            v3f(7.0, 0.0, 2.0);
	        glEnd();
	    glPopMatrix();
	}
// wdh    if (doubleBuffer) glXSwapBuffers(dpy, XtWindow(w));
// wdh    if(!glXIsDirect(dpy, cx))
// wdh        glFinish(); /* avoid indirect rendering latency from queuing */

// *wdh*    displayString("Here-is-a-better-font");

#ifdef DEBUG
 { /* for help debugging, report any OpenGL errors that occur per frame */
   GLenum error;
   while((error = glGetError()) != GL_NO_ERROR)
     fprintf(stderr, "GL error: %s\n", gluErrorString(error));
 }
#endif
}

void
resize(GL_GraphicsInterface *gi, const int & win_number)
{
}


void 
tick_per_plane(int i)
{
    float theta = planes[i].theta += planes[i].speed;
    planes[i].z = -9 + 4 * cos(theta);
    planes[i].x = 4 * sin(2 * theta);
    planes[i].y = sin(theta / 3.4) * 3;
    planes[i].angle = ((atan(2.0) + M_PI_2) * sin(theta) - M_PI_2) * 180 / M_PI;
    if (planes[i].speed < 0.0) planes[i].angle += 180;
}

void 
add_plane(void)
{
    int i;

    for (i = 0; i < MAX_PLANES; i++)
	if (planes[i].speed == 0) {

#define SET_COLOR(r,g,b) \
	planes[i].red=r; planes[i].green=g; planes[i].blue=b; break;

            switch (rand() % 6) {
            case 0: SET_COLOR(1.0, 0.0, 0.0); /* red */
            case 1: SET_COLOR(1.0, 1.0, 1.0); /* white */
            case 2: SET_COLOR(0.0, 1.0, 0.0); /* green */
            case 3: SET_COLOR(1.0, 0.0, 1.0); /* magenta */
            case 4: SET_COLOR(1.0, 1.0, 0.0); /* yellow */
            case 5: SET_COLOR(0.0, 1.0, 1.0); /* cyan */
            }
            planes[i].speed = (rand() % 20) * 0.001 + 0.02;
            if (rand() & 0x1) planes[i].speed *= -1;
	    planes[i].theta = ((float) (rand() % 257)) * 0.1111;
 	    tick_per_plane(i);
//	    if (!moving) draw(glxarea);
	    return;
	}
//    XBell(dpy, 100); /* can't add any more planes */
}


void 
remove_plane(void)
{
    int             i;

    for (i = MAX_PLANES - 1; i >= 0; i--)
	if (planes[i].speed != 0) {
	    planes[i].speed = 0;
//	    if (!moving) draw(glxarea);
	    return;
	}
//    XBell(dpy, 100); /* no more planes to remove */
}


static const int maximumNumberOfClippingPlanes=6;
// static int numberOfClippingPlanes=0;               // number of clipping planes that have been turned on
static int clippingPlaneIsOn[maximumNumberOfClippingPlanes];   // TRUE if a clipping plane is turned on
// each clipping plane is defined by four constants c0*x+c1*y+c2*z+c3
static double clippingPlaneEquation[maximumNumberOfClippingPlanes][4];

int
main(int argc, char *argv[])
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  int i, j;

  aString fileMenuItems[] = {"read command file", 
			     "save command file",
			     "abort",
			     "" };      // terminated by ""
  aString helpMenuItems[] = {"other help", 
			     "" };      // terminated by ""

  aString graphicsFileMenuItems[] = {"save postscript",
				     "exit",
				     "" };      // terminated by a ""
  aString graphicsHelpMenuItems[] = {"mouse buttons", 
				   "" };      // terminated by a ""

  ClippingPlaneInfo clippingPlaneInfo[MAX_WINDOWS];
  ViewCharacteristics viewChar[MAX_WINDOWS];
  enum
  {
    numberOfLights=3
  };
  
  float rotationCenter[MAX_WINDOWS][3];
  int axesOriginOption[MAX_WINDOWS]; // indicates where to place the axes origin

  float backGround[MAX_WINDOWS][4];  // back ground colour
  float foreGround[MAX_WINDOWS][4];  // text colour

  int lighting[MAX_WINDOWS];
  int lightIsOn[MAX_WINDOWS][numberOfLights];  // which lights are on
  GLfloat ambient[MAX_WINDOWS][numberOfLights][4];
  GLfloat diffuse[MAX_WINDOWS][numberOfLights][4];
  GLfloat specular[MAX_WINDOWS][numberOfLights][4];
  GLfloat position[MAX_WINDOWS][numberOfLights][4];
  GLfloat globalAmbient[MAX_WINDOWS][4];

  // default material properties, these values are used for surfaces in 3D when we give them
  // different colours
  GLfloat materialAmbient[MAX_WINDOWS][4];
  GLfloat materialDiffuse[MAX_WINDOWS][4];
  GLfloat materialSpecular[MAX_WINDOWS][4];
  GLfloat materialShininess[MAX_WINDOWS];
  GLfloat materialScaleFactor[MAX_WINDOWS];

  real lineScaleFactor[MAX_WINDOWS];

  GL_GraphicsInterface *gi=NULL;  // only used to pass to moglSetFunctions

  moglInit( argc, argv, "my interface", fileMenuItems, helpMenuItems );

  for (int w=0; w<MAX_WINDOWS; w++)
  {
    clippingPlaneInfo[w].maximumNumberOfClippingPlanes=maximumNumberOfClippingPlanes;
    clippingPlaneInfo[w].clippingPlaneIsOn=&(clippingPlaneIsOn[0]);
    clippingPlaneInfo[w].clippingPlaneEquation=&(clippingPlaneEquation[0][0]);

// view characteristics
    viewChar[w].rotationCenter = rotationCenter[w];
    viewChar[w].axesOriginOption_ = &axesOriginOption[w];

    viewChar[w].backGround = backGround[w];
    viewChar[w].foreGround = foreGround[w];

    lighting[w] = 1;
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

    lineScaleFactor[w] = 1.0;
    viewChar[w].lineScaleFactor_ = &lineScaleFactor[w];

    xShift[w] = yShift[w] = zShift[w] = 0.;
    dtx[w] = dty[w] = dtz[w] = 0.;
    magnificationFactor[w] = 1.;

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
    position[w][0][0] = 10.; position[w][0][1] = 10.; position[w][0][2] = 10.; 
    position[w][0][3] = 0.;
    position[w][1][0] =-10.; position[w][1][1] =-10.; position[w][1][2] = 10.; 
    position[w][1][3] = 0.;
    position[w][2][0] =-10.; position[w][2][1] = 10.; position[w][2][2] = 10.; 
    position[w][2][3] = 0.;

    globalAmbient[w][0]=.2;  // global ambient light
    globalAmbient[w][1]=.2;
    globalAmbient[w][2]=.2;
    globalAmbient[w][3]=1.0;

    // set default material properties, these values are used for surfaces in 3D when we give them
    // different colours
    materialAmbient[w][0] = .4; 
    materialAmbient[w][1] = .4; 
    materialAmbient[w][2] = .4; 
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
    materialScaleFactor[w]=1.0;  // scale factor

    axesOriginOption[w]=0;

    for( i=0; i<3; i++ )
    {
      rotationCenter[w][i]=0.;
    }
  }
  

// -----------------make two graphics windows----------------------
  moglSetFunctions(gi,displayM,resize);
  moglSetViewFunction( changeView );

  //..create HardCopy dialogs=required input to makeGraphicsWindow **pf
  DialogData & hcd = hardCopyDialog[moglGetNWindows()];
  setupHardCopy(hcd, moglGetNWindows());
  //..end creating the hardcopy dialog

  int winList[MAX_WINDOWS], win=0;
  
  winList[win++] = makeGraphicsWindow("graphics0", 
				      graphicsFileMenuItems, 
				      graphicsHelpMenuItems, 
				      clippingPlaneInfo[0], 
				      viewChar[0], 
				      hcd);

  winList[win++] = makeGraphicsWindow("graphics1", 
				      graphicsFileMenuItems, 
				      graphicsHelpMenuItems, 
				      clippingPlaneInfo[1], 
				      viewChar[1], 
				      hcd);
  
  winList[win++] = makeGraphicsWindow("graphics2", 
				      graphicsFileMenuItems, 
				      graphicsHelpMenuItems, 
				      clippingPlaneInfo[2], 
				      viewChar[2],
				      hcd);

  /* add three initial random planes */
  srand(12345);
  add_plane(); add_plane(); add_plane();

// setup the projection matrix in each window
  for (i=0; i<win; i++)
    {
      moglMakeCurrent(i);

      glMatrixMode(GL_PROJECTION);
      glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 20);
      glMatrixMode(GL_MODELVIEW);

  /* set background color */
      glClearColor( 0.84, 0.82, 0.65, 0.0 );
    }

  aString menu1[] = { "!mogl test program", "read command file", "menu", "buttons",
		    "dialog",
                    ">component", "u", "v", "w", 
                    "<erase",
                    ">stuff", "s1", ">more stuff", "more1", "more2", 
                    "<s2", 
                    "<>apples", "apple1", 
                    "<exit", "" };  
  aString menu2[] = { "!my title", "contour","draw","wait","exit", "" };  
  aString menu3[] = { "!Enter Filename", "" };  
  moglBuildPopup( menu1 );

  int item;
  aString answer;
  for(; ; )
  {
    item = moglGetAnswer( answer, "A prompt>" );
    cout << "answer=[" << answer << "]" << endl;
    if (answer == "read command file")
    {
      moglOpenFileSB();
// call with a null menu to block other input
      moglGetAnswer( answer, "Select FileName" ); 
      cout << "filename=[" << answer << "]" << endl;
      printf("%s\n", (const char *) answer);
// explicitly bring down the dialog
      moglCloseFileSB();
    }
    
    if (answer == "menu")
    {
// add a user defined pulldown menu to window #2
      aString menuName[] = { "m1", "m2", "menu3", "Remove this menu", "" };
      moglBuildUserMenu(menuName, "My menu", 2);
    }
    else if (answer == "Remove this menu")
    {
// remove the user defined pulldown menu to window #2
      moglBuildUserMenu( NULL, NULL, 2);
    }
    else if (answer == "buttons")
    {
// add user defined buttons to window #0
      aString buttonCommands[] = {"button 1", "button 2", "remove buttons", ""};
      aString buttonLabels[]   = {"btn1", "btn2", "rmb", ""};
      moglBuildUserButtons(buttonCommands, buttonLabels, 0);
    }
    else if (answer == "remove buttons")
// remove the user defined buttons from window #0
    {
      moglBuildUserButtons(NULL, NULL, 0);
    }
    else if (answer == "dialog")
    {
      aString dialogMenu[] = { "!Dialog menu", "do stuff", "" };  
    
      DialogData dialogSpec;
      setupDialog(dialogSpec);
      dialogSpec.openDialog();
// specify tha second dialog box
      DialogData moreOptions;
      moreOptions.setWindowTitle("Expert options");
      aString moreButtonC[] ={"one", "two", "three", ""};
      aString moreButtonL[] ={"One", "Two", "Three", ""};
      moreOptions.setPushButtons(moreButtonC, moreButtonL);
      moreOptions.setExitCommand("close expert dialog", "Close");

// setup the modified popup menu
      moglBuildPopup( dialogMenu );
      PickInfo pick;
      do
      {
	moglGetAnswer( answer, "", &pick); 
	if (pick.pickType)
	{
	  printf("pickType: %i\n", pick.pickType);
	  printf("pickWindow: %i\n", pick.pickWindow);
	  printf("pickBox: %e, %e, %e, %e\n", pick.pickBox[0], pick.pickBox[1], 
		 pick.pickBox[2], pick.pickBox[3]);
	}
	
	cout << "dialog answer=[" << answer << "]" << endl;
//                          0123456789
	if (answer(0,5) == "expert")
	{
	  int state;
	  sscanf(&answer[6],"%i", &state);
	  //bring up another dialog with more options
	  if (state == 1)
	    moreOptions.openDialog();
	  else
	    moreOptions.closeDialog();
	}
	else if (answer == "close expert dialog")
	{
	  dialogSpec.setToggleState(3,0); // explicitly turn off toggle button 3: "Expert options"
	  moreOptions.closeDialog();
	}
//                                012345678901234567
	else if (answer(0,16) == "distance to march")
	{
// parse the answer
	  float d=1.0, dum;
	  int n_read = sscanf(&answer[17], "%g %g", &d, &dum);
	  
	  if (n_read < 1)
	  {
	    // couldn't read a real number
	    printf("ERROR: Non-numeric input\n");
	    // write back the default value in the text string
	    aString buff;
	    sPrintF(buff, "%g", d);
	    dialogSpec.setTextLabel(0, buff);
	  }
	  else
	  {
	    // write back the value in the text string using the standard format
	    aString buff;
	    sPrintF(buff, "%g", d);
	    dialogSpec.setTextLabel(0, buff);
	    printf("Read distance to march: %g\n", d);
	  }
	}
	else if (answer == "project")
	{
	  dialogSpec.setSensitive(0); // make the dialog insensitive
	  // do some stuff
	  aString projectMenu[] = { "!Project menu", "do stuff", "exit", "" };  
// setup the modified popup
	  moglBuildPopup( projectMenu );
	  do
	  {
	    moglGetAnswer( answer ); 
	  }
	  while (answer != "exit");
// restore the dialog popup
	  moglBuildPopup( dialogMenu );
	  dialogSpec.setSensitive(1); // make the dialog sensitive again
	}
	
      }
      //while (answer != dialogSpec.exitCommand); //exitCommand is protected **pf
      while (answer != dialogSpec.getExitCommand() ); //**pf
      dialogSpec.closeDialog();
      moreOptions.closeDialog(); // this additional dialog window might be open
// restore the main popup menu
      moglBuildPopup( menu1 );
    }
    else if (answer == "exit") 
      break;
    else if (answer == "abort") 
      break;
  }
  moglBuildPopup( NULL ); // remove the main popup menu

  
  for (i = win-1; i>=0; i--)
    destroyGraphicsWindow(i);
  
  return 0;
}

static void
setupDialog(DialogData &dialogSpec)
{
  int i,j;
  
  dialogSpec.setWindowTitle("GI test dialog");
  dialogSpec.setExitCommand("kill the dialog", "Close dialog");

// define toggle buttons
  aString pbCommands[] = {"generate", "smooth", "project", ""};
  aString pbLabels[] = {"Generate", "Smooth grid", "Project grid", ""};
  
  dialogSpec.setPushButtons( pbCommands, pbLabels, 1 );
  
// define toggle buttons
  aString tbCommands[] = {"grow grid in both directions", "plot cell quality", 
			  "save reference surface when put", "expert", ""};
  aString tbLabels[] = {"Both directions", "Plot cell quality", "Save reference surface", 
			"More options", ""};
  int tbState[] = {1, 0, 1, 0};
  
  dialogSpec.setToggleButtons(tbCommands, tbLabels, tbState, 2);
  
// text labels
  aString textCommands[] = {"distance to march", "step n", "lines to march", 
			    "initial grid spacing", ""};
  aString textLabels[] = {"Distance", "Number of steps", "Number of lines", 
			  "First grid size", ""};
  aString textStrings[] = {"1.0", "14", "10", "0.1", ""};

  dialogSpec.setTextBoxes( textCommands, textLabels, textStrings );

// define option menus
  dialogSpec.setOptionMenuColumns(2);
  
// first option menu
  aString opCommand0[] = {"Start from Square", "Start from Sphere", "Start from Cylinder", ""};
  aString opLabel0[] = {"Square", "Sphere", "Cylinder", ""};
  dialogSpec.addOptionMenu( "Start from", opCommand0, opLabel0, 1); // initial choice: element 1

// second option menu
  aString opCommand1[] = {"Project onto Left", "Project onto Right", "Project onto Upper", 
			 "Project onto Lower", ""};
  aString  opLabel1[] = {"Left", "Right", "Upper", "Lower", ""};
  dialogSpec.addOptionMenu( "Project onto", opCommand1, opLabel1, 2); // initial choice: element 2

// third option menu
  aString opCommand2[] = {"Choose color blue", "Choose color red", "Choose color green", 
			 "Choose color yellow", ""};
  aString opLabel2[] = {"Blue", "Red", "Green", "Yellow", ""};
  dialogSpec.addOptionMenu( "Project onto", opCommand2, opLabel2, 3); // initial choice: element 3

// done defining option menus

// define pulldown menus
  aString pdCommand0[] = {"New command", "Open command", "Save command", "Exit command", ""};
  aString pdLabel0[] = {"New", "Open", "Save", "Exit", ""};
  dialogSpec.addPulldownMenu("File", pdCommand0, pdLabel0, GI_PUSHBUTTON);
  
  aString pdCommand1[] = {"Square command", "Box command", "Save command", 
			 "Exit command", ""};
  aString pdLabel1[] = {"Square", "Box", ""};
  int initState[] = {0, 1};
  dialogSpec.addPulldownMenu("View", pdCommand1, pdLabel1, GI_TOGGLEBUTTON, initState);
  
  aString pdCommand2[] = {"Stuffed help", "Other stuffed help", "Button help", 
			 "Command file help", ""};
  aString pdLabel2[] = {"Stuff", "Other Stuff", "Buttons", "Command files", ""};
  dialogSpec.addPulldownMenu( "Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  dialogSpec.setLastPullDownIsHelp(1);
  
// done defining pulldown menus  
}
