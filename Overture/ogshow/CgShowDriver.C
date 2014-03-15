#include "Cgshow.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <GL/glut.h>

Cgshow cgshow;   // cgshow class object
realCompositeGridFunction u;

real xEyeCoordinate=0., yEyeCoordinate=0., zEyeCoordinate=5.; // 5.  // position of the eye
real xAngle=0., yAngle=0., zAngle=0.;
real dtx=0., dty=0., dtz=0.;
real xShift=0., yShift=0., zShift=0.;
real magnificationFactor=1., magnificationIncrement=1.25;
real deltaX=.05, deltaY=.05, deltaZ=.05, deltaAngle=10.;

/* GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0}; */
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

GLfloat light_position[] =
{1.0, 1.0, 1.0, 0.0};
GLUquadricObj *qobj;

int win1, win2, submenu1, submenu2;
 
int menu1,menu2,menuContour,menuOld;

int list = 1;

float thetime = 0.0;

RealArray rotationMatrix(4,4), matrix(4,4);
Index I4(0,4);

void
display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // glTranslatef(0.0, 0.0, -1.0);    // is this needed?

  gluLookAt(xEyeCoordinate,yEyeCoordinate,zEyeCoordinate,  /* eye is at (0,-5,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */


  real cy=cos(yAngle*Pi/180.), sy=-sin(yAngle*Pi/180.);
  real cz=cos(zAngle*Pi/180.), sz=-sin(zAngle*Pi/180.);


  glTranslatef(xShift,yShift,zShift);
  glScalef(magnificationFactor,magnificationFactor,magnificationFactor);

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
    real cy=cos(dty*Pi/180.), sy=sin(dty*Pi/180.);
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

//  glRotatef(zAngle,0.,0.,1.);          // rotate about the z-axis
//  glRotatef(yAngle,0.,1.,0.);          // rotate about the FIXED y-axis
//  glRotatef(xAngle,1.,0.,0);  // rotate about the FIXED x-axis

  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLbitfield arg = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  glClear(arg);
//  glCallList(1);      /* render sphere display list */
  for( int i=1; i<10; i++ )
   glCallList(i);      /* render contour display list */

  glutSwapBuffers();
}

void
display_win1(void)
{
  glPushMatrix();
  glTranslatef(0.0, 0.0, -1 - 2 * sin(thetime));
  display();
  glPopMatrix();
}

void
idle(void)
{
/* ---
  GLfloat light_position[] =
  {1.0, 1.0, 1.0, 0.0};

  glutSetWindow(win1);
  thetime += 0.05;
  light_position[1] = 1 + sin(thetime);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  display_win1();
 --- */
}

void
delayed_stop(int value)
{
  glutIdleFunc(NULL);
}

void
CBshow(int value)
{
  printf("menu selection: win=%d, menu=%d\n", glutGetWindow(), glutGetMenu());
  switch (value) {
  case 1:
    menuOld=glutGetMenu();
    glutSetMenu(menuContour);                // put up menu for contour
    glutAttachMenu(GLUT_LEFT_BUTTON);
    break;
  case 2:
    cgshow.surface();
    break;
  case 3:
    cout << "streamline plot...\n";
    glutPostRedisplay();
    break;
  case 4:
    cout << "grid plot...\n";
    cgshow.grid();
    glutPostRedisplay();
    break;
  case 98:  // erase all display lists
    glDeleteLists(1,100);
    glutPostRedisplay();
    break;
  case 99:
    exit(0);
  default:
    printf("value = %d\n", value);
  }
}

void
CBcontour(int val)
{
  int par;
  switch (val)
  {
  case 1:
    cout << "Cgshow:contourCallBack: help\n";
    break;
  case 2:
    cout << "Cgshow:contourCallBack: parameters\n";
    cout << "Enter an int value...\n";
    // cin >> par;
    break;
  case 3:
    cout << "Cgshow:contourCallBack: plot\n";
    PlotIt::contour(cgshow, u );
    glutPostRedisplay();
    break;
  case 99:
    cout << "Cgshow:contourCallBack: exit\n";
    glutSetMenu(menuOld);                        // restore old menu
    glutAttachMenu(GLUT_LEFT_BUTTON);
    
    break;
  }
}

void
init(void)
{

  //  gluQuadricDrawStyle(qobj, GLU_FILL);
  //  glNewList(1, GL_COMPILE);  /* create sphere display list */
  //
  //  
  //  gluSphere(qobj, /* radius */ 1.0, /* slices */ 20,  /* stacks 
  //                                                       */ 20);
  //  glEndList();
  //  gluQuadricDrawStyle(qobj, GLU_LINE);
  //  glNewList(2, GL_COMPILE);  /* create sphere display list */
  //  gluSphere(qobj, /* radius */ 1.0, /* slices */ 20,  /* stacks 
  //                                                       */ 20);
  //  glEndList();

  //  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  //  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  //  glEnable(GL_LIGHTING);
  //  glEnable(GL_LIGHT0);

  glEnable(GL_DEPTH_TEST);

//  glMatrixMode(GL_PROJECTION);
//  gluPerspective( /* field of view in degree */ 40.0,  /* aspect ratio */ 1.0,
//    /* Z near */ 1., /* Z far */ 5.0);   // 10.

  glMatrixMode(GL_PROJECTION);
  real left=-1., right=1., bottom=-1., top=1., near=1., far=10.;   // here is the screen size
  glOrtho( left,right,bottom,top,near,far );       

  glMatrixMode(GL_MODELVIEW);

//  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
//    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
//    0.0, 1.0, 0.);      /* up is in positive Y direction */
//  glTranslatef(0.0, 0.0, -1.0);

  rotationMatrix=0.;
  rotationMatrix(0,0)=1.;   rotationMatrix(1,1)=1.;   
  rotationMatrix(2,2)=1.;   rotationMatrix(3,3)=1.; 

}

void
menustate(int inuse)
{
  printf("menu is %s\n", inuse ? "INUSE" : "not in use");
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
  }    
  if(isprint(key)) {
    printf("key: `%c' %d,%d\n", key, x, y);
  } else {
    printf("key: 0x%x %d,%d\n", key, x, y);
  }
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
   default: name = "UNKONW"; break;
   }
   printf("special: %s %d,%d\n", name, x, y);
}

void
mouse(int button, int state, int x, int y)
{
  printf("button: %d %s %d,%d\n", button, state == GLUT_UP ? "UP" : "down", x, y);
}

void
motion(int x, int y)
{
  printf("motion: %d,%d\n", x, y);
}

void
visible(int status)
{
  printf("visible: %s\n", status == GLUT_VISIBLE ? "YES" : "no");
}

int 
main(int argc, char **argv)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  aString nameOfOGFile(80), nameOfShowFile(80), nameOfDirectory(80);

  cout << "cgshow>> Enter the name of the show file:" << endl;
  cin >> nameOfShowFile;
  nameOfShowFile = "/n/c3servet/henshaw/cgap/cguser/" + nameOfShowFile;

  nameOfDirectory=".";
  MultigridCompositeGrid mgcg(nameOfShowFile,nameOfDirectory);   // read from a data base file
  CompositeGrid & cg=mgcg[0];                                  // use multigrid level 0

  cgshow.cg.reference(cg);    // reference to version in cgshow

  // set up a function for contour plotting:
  u.updateToMatchGrid(cg);
  Index I1,I2,I3;                                       // A++ Index object
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    getIndex(cg[grid].dimension(),I1,I2,I3);                  // assign I1,I2,I3 from indexRange
    u[grid](I1,I2,I3)=sin(Pi*cg[grid].vertex()(I1,I2,I3,axis1))   // assign all interior points on this
                     *cos(Pi*cg[grid].vertex()(I1,I2,I3,axis2));  // component grid
  }    
    



  qobj = gluNewQuadric();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // ------create a window-----
  win1 = glutCreateWindow("cgshow");
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutVisibilityFunc(visible);
  init();

  glutDisplayFunc(display_win1);  // set the call back routine for re-drawing the screen

  // create a menu for the left mouse button
  menu1 = glutCreateMenu(CBshow);                   // set the call back for the menu
  glutAddMenuEntry("contour plot", 1);
  glutAddMenuEntry("surface plot", 2);
  glutAddMenuEntry("streamline", 3);
  glutAddMenuEntry("grid", 4);
  glutAddMenuEntry("erase",98);
  glutAddMenuEntry("exit",99);
  glutAttachMenu(GLUT_LEFT_BUTTON);

  // menu for contour
  menuContour = glutCreateMenu(CBcontour);             // set the call back for the menu
  glutAddMenuEntry("help", 1);
  glutAddMenuEntry("parameters", 2);
  glutAddMenuEntry("plot", 3);
  glutAddMenuEntry("exit", 99);


  // create a menu for the right mouse button
//  glutCreateMenu(it);
//  glutAddMenuEntry("yes", 1);
//  glutAddMenuEntry("no", 2);
//  glutAttachMenu(GLUT_RIGHT_BUTTON);



/* ----

  init();
  light_diffuse[0] = 0;         // change the colour of the second sphere
  light_diffuse[1] = 0; 
  light_diffuse[2] = 1;
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glutDisplayFunc(display);
  submenu1 = glutCreateMenu(it);
  glutAddMenuEntry("submenu a", 666);
  glutAddMenuEntry("submenu b", 777);
  submenu2 = glutCreateMenu(it);
  glutAddMenuEntry("submenu 1", 25);
  glutAddMenuEntry("submenu 2", 26);
  glutAddSubMenu("submenuXXX", submenu1);
  glutCreateMenu(it);
  glutAddSubMenu("submenu", submenu2);
  glutAddMenuEntry("stop motion", 5);
  glutAddMenuEntry("delayed stop motion", 6);
  glutAddSubMenu("submenu", submenu2);
  glutAttachMenu(GLUT_LEFT_BUTTON);

---- */

  glutMenuStateFunc(menustate);
  glutMainLoop();
}
