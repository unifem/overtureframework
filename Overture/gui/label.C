#include "GL_GraphicsInterface.h"
#include <GL/glu.h>

#ifdef NO_APP
using GUITypes::real;
#endif

void strokeCharacter(int );

/* ---
#include <GL/glx.h>
extern XFontStruct *fontInfo;
extern int fontListBase;
---- */

void GL_GraphicsInterface:: 
labelR(const aString & string,     
      const real xPosition, 
      const real yPosition,
      const real size,       /* =.1 */
      const int centering,   /* =0  */ 
      const real angle,      /* =0. */
      GraphicsParameters & parameters  /* =Overture::defaultGraphicsParameters() */ )
// ================================================================================
// This label uses raster fonts  **** this is not finished *****
// ================================================================================
{
/* -----
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  real left=-1., right=1., bottom=-1., top=1.;
  gluOrtho2D( left,right,bottom,top );
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  real charHeight=105.;  // these are guesses !
  real charWidth=70.;    //
  real xScale=size/charHeight;  // size=2 =(top-bottom) should be entire screen
  real yScale=size/charHeight;

  real xShift=0.;
  if( centering==0 )
    xShift=string.length()*.5*xScale*charWidth;   // centre, shift by half the length
  else if( centering==+1 )                 // flush left
    xShift=string.length()*xScale*charWidth;
  real yShift=.5*yScale*charHeight;

//  glTranslate(xPosition,yPosition,0.);     // translate to correct position

//  glRotate(angle, 0., 0., 1.);                           // rotate about z-axis
//  glTranslate(-xShift,-yShift,0.);                      // translate to center the string 
//  glScale(xScale,yScale,1.);

  glListBase(fontListBase);
  glRasterPos2f(xPosition-xShift, yPosition-yShift);
//  glBitmap(0, 0, 0, 0,
//    winWidth / 2 - width / 2,
//    winHeight / 2 - (fontInfo->ascent + fontInfo->descent) / 2, 0);
  glCallLists(string.length(), GL_UNSIGNED_BYTE, (const char*)string);
  glListBase(0);



  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
------ */
}


//void *font = GLUT_STROKE_ROMAN;
//void *fonts[] =
//{GLUT_STROKE_ROMAN, GLUT_STROKE_MONO_ROMAN};

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{label: plot a aString in normalized coordinates}} 
void GL_GraphicsInterface:: 
label(const aString & string,     
      real xPosition, 
      real yPosition,
      real size,       /* =.1 */
      int centering,   /* =0  */ 
      real angle,      /* =0. */
      GraphicsParameters & parameters  /* =Overture::defaultGraphicsParameters */,
      const aString & colour  /* =nullString */,
      real zOffset /* =.99 */ )
// =======================================================================================
//
//  /Description:
//    This routine plots a label in the normalized coordinate system where the screen
//    has dimensions [-1,1]x[-1,1]. This label does NOT rotate or scale with the plot.
//  /string (input): aString to draw.
//  /xPosition (input): x coordinate of the string in normalized coordinates, [-1,1]. (See the
//    centering argument).
//  /yPosition (input): y coordinate of the string in normalized coordinates, [-1,1]. (See the
//    centering argument).
//  /size (input): Size of the characters in normalized coordinates (size=2.0 would fill the whole view).
//  /centering (input): {\ff centering=0} means put the centre of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=-1} means put the left end of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=+1} means put the right end of the string at {\ff (xPosition,yPosition)}.
//  /angle (input): Angle in degrees to rotate the string.
//  /colour (input): optionally specify a colour for the text. 
//  /zOffset (input): by default raise the label so that it is not covered by the plot 
//                (NOTE: the front clip plane is at z=1)
//  /Errors: none (Ha).
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
// AP: add win_number as an argument
{
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
//    glLoadIdentity();

  // shift labels to edge of possible non-square screen.
//  xPosition*=(rightSide-leftSide)*.5;
//  yPosition*=(top-bottom)*.5;
  

  // printf("label: (x,y)=(%e,%e) size=%8.2e [%s]\n",xPosition,yPosition,size,(const char*)string);
  

  real charHeight=105.;  // these are guesses !
  real charWidth=80.; // *wdh* 021111 70.;   
  real xScale=size/charHeight;  // size=2 =(top-bottom) should be entire screen
  real yScale=size/charHeight;

  real xPos=0.;
  if( centering==0 )
    xPos=string.length()*.5*xScale*charWidth;   // centre, shift by half the length
  else if( centering==+1 )                 // flush left
    xPos=string.length()*xScale*charWidth;
  real yPos=.5*yScale*charHeight;

  //  add a white frame behind the text
  const real delta=.02;
  glBegin(GL_QUAD_STRIP);

  setColour(GenericGraphicsInterface::backGroundColour);  

  real xLeft=xPosition-xPos, xRight=xPosition-xPos+string.length()*xScale*charWidth;
  real yBottom=yPosition-yPos, yTop=yPosition-yPos+yScale*charHeight;
  
//    printf("label: (x,y)=(%e,%e) size=%8.2e [%s] xPos=%e yPos=%e xLeft=%e xRight=%e yBottom=%e yTop=%e xScale=%e yScale=%e\n",
//             xPosition,yPosition,size,(const char*)string,xPos,yPos,xLeft,xRight,yBottom,yTop,xScale,yScale);
  

  glVertex3f(xLeft -delta,yBottom-delta,zOffset);
  glVertex3f(xRight+delta,yBottom-delta,zOffset);

  glVertex3f(xLeft-delta ,yTop+delta,zOffset);
  glVertex3f(xRight+delta,yTop+delta,zOffset);
  glEnd();

  zOffset+=.0001;
  glTranslate(xPosition,yPosition,zOffset);     // translate to correct position

  glRotate(angle, 0., 0., 1.);                           // rotate about z-axis
  glTranslate(-xPos,-yPos,0.);                      // translate to center the string 
  glScale(xScale,yScale,1.);

  if( colour!=nullString )
    setColour(colour);
  else
    setColour(GenericGraphicsInterface::textColour);  

  glLineWidth(parameters.size(lineWidth)*parameters.size(labelLineWidth)*lineWidthScaleFactor[currentWindow]);
  for( int i=0; i<string.length(); i++ )       
    strokeCharacter(string[i]);  
//    glutStrokeCharacter(font,string[i]);  // 



  glPopMatrix();

}

/* -----
void
label(const aString & string,     
      const real xPosition, 
      const real yPosition,
      const real size=.1,
      const int centering=0, 
      const real angle=0. )     // angle in degrees
{

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  real left=-1., right=1., bottom=-1., top=1.;
  gluOrtho2D( left,right,bottom,top );
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  real charHeight=105.;  // these are guesses !
  real charWidth=70.;    //
  real xScale=size/charHeight*(right-left);  // size=1 should be entire screen
  real yScale=size/charHeight*(top-bottom);

  real xShift=0.;
  if( centering==0 )
    xShift=string.length()*.5*xScale*charWidth;   // centre, shift by half the length
  else if( centering==+1 )                 // flush left
    xShift=string.length()*xScale*charWidth;
  real yShift=.5*yScale*charHeight;

  glTranslate(xPosition,yPosition,0.);     // translate to correct position

  glRotate(angle, 0., 0., 1.);                           // rotate about z-axis
  glTranslate(-xShift,-yShift,0.);                      // translate to center the string 
  glScale(xScale,yScale,1.);

  for( int i=0; i<string.length(); i++ )       
    strokeCharacter(string[i]);  
//    glutStrokeCharacter(font,string[i]);  // 

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
}
---- */

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{xlabel: plot a aString in 2D world coordinates}} 
void GL_GraphicsInterface:: 
xLabel(const aString & string,     
       const real xPosition, 
       const real yPosition,
       const real size,      /* =.1 */
       const int centering,  /* =0  */ 
       const real angle,     /* =0. */
       GraphicsParameters & parameters  /* =Overture::defaultGraphicsParameters() */,
       int win_number /* = -1 */ )
// =======================================================================================
//  /Description:
//    This routine plots a label with position and size in World coordinates, 
//    This label DOES rotate and scale with the plot. This version of xLabel plots
//    the label in the $z=0$ plane.
//  /string (input): aString to draw.
//  /xPosition (input): x coordinate of the string in world coordinates. (See the
//    centering argument).
//  /yPosition (input): y coordinate of the string in world coordinates. (See the
//     centering argument).
//  /size (input): Size of the characters in NORMALIZED coordinates.
//  /centering (input): {\ff centering=0} means put the centre of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=-1} means put the left end of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=+1} means put the right end of the string at {\ff (xPosition,yPosition)}.
//  /angle (input): Angle in degrees to rotate the string.
//  /Errors: none.
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  if (win_number == -1)
    win_number = currentWindow;
  
  RealArray x(3), rightVector(3), upVector(3);
  x(0)=xPosition;
  x(1)=yPosition;
  x(2)=0.;
  real ct=cos(angle*Pi/180.);
  real st=sin(angle*Pi/180.);
  rightVector(0)=ct;
  rightVector(1)=st;
  rightVector(2)=0.;
  upVector(0)=-st;
  upVector(1)= ct;
  upVector(2)=0.;
  
  xLabel(string,x,size,centering,rightVector,upVector,parameters,win_number);  

/* ---
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  real charHeight=105.;  // these are guesses !
  real charWidth=70.;    //
  real xScale=size/charHeight;
  real yScale=size/charHeight;

  real xShift=0.;
  if( centering==0 )
    xShift=string.length()*.5*xScale*charWidth;   // centre, shift by half the length
  else if( centering==+1 )                 // flush left
    xShift=string.length()*xScale*charWidth;
  real yShift=.5*yScale*charHeight;

  glTranslate(xPosition,yPosition,0.);     // translate to correct position

  glRotate(angle, 0., 0., 1.);                           // rotate about z-axis
  glTranslate(-xShift,-yShift,0.);                      // translate to center the string 
  glScale(xScale,yScale,1.);

  for( int i=0; i<string.length(); i++ )       
    strokeCharacter(string[i]);  

  glPopMatrix();
---- */

}
//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{xlabel: plot a aString in 3D world coordinates}} 
void GL_GraphicsInterface:: 
xLabel(const aString & string,     
       const real x[3], 
       const real size,      /* =.1 */
       const int centering,  /* =0  */ 
       const real angle,     /* =0. */
       GraphicsParameters & parameters  /* =Overture::defaultGraphicsParameters() */,
       int win_number /* = -1 */ )
// =======================================================================================
//  /Description:
//    This routine plots a label with position and size in World coordinates, 
//    This label DOES rotate and scale with the plot. This version of xLabel plots
//    the label in the $z=0$ plane.
//  /string (input): aString to draw.
//  /x (input): x(0:2) 3D coordinates of the string in world coordinates. (See the
//    centering argument).
//  /size (input): Size of the characters in NORMALIZED coordinates.
//  /centering (input): {\ff centering=0} means put the centre of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=-1} means put the left end of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=+1} means put the right end of the string at {\ff (xPosition,yPosition)}.
//  /angle (input): Angle in degrees to rotate the string.
//  /Errors: none.
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  if (win_number == -1)
    win_number = currentWindow;

  real rightVector[3], upVector[3];
  real ct=cos(angle*Pi/180.);
  real st=sin(angle*Pi/180.);
  rightVector[0]=ct;
  rightVector[1]=st;
  rightVector[2]=0.;
  upVector[0]=-st;
  upVector[1]= ct;
  upVector[2]=0.;
  
  xLabel(string,x,size,centering,rightVector,upVector,parameters, win_number);  

}
//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{xlabel: plot a aString in 3D world coordinates}} 
void GL_GraphicsInterface:: 
xLabel(const aString & string,     
       const RealArray & x, 
       const real size,      /* =.1 */
       const int centering,  /* =0  */ 
       const real angle,     /* =0. */
       GraphicsParameters & parameters  /* =Overture::defaultGraphicsParameters() */,
       int win_number /* = -1 */ )
// =======================================================================================
//  /Description:
//    This routine plots a label with position and size in World coordinates, 
//    This label DOES rotate and scale with the plot. This version of xLabel plots
//    the label in the $z=0$ plane.
//  /string (input): aString to draw.
//  /x (input): x(0:2) 3D coordinates of the string in world coordinates. (See the
//    centering argument).
//  /size (input): Size of the characters in NORMALIZED coordinates.
//  /centering (input): {\ff centering=0} means put the centre of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=-1} means put the left end of the string at {\ff (xPosition,yPosition)}.
//    {\ff centering=+1} means put the right end of the string at {\ff (xPosition,yPosition)}.
//  /angle (input): Angle in degrees to rotate the string.
//  /Errors: none.
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  if (win_number == -1)
    win_number = currentWindow;

  RealArray rightVector(3), upVector(3);
  real ct=cos(angle*Pi/180.);
  real st=sin(angle*Pi/180.);
  rightVector(0)=ct;
  rightVector(1)=st;
  rightVector(2)=0.;
  upVector(0)=-st;
  upVector(1)= ct;
  upVector(2)=0.;
  
  xLabel(string,x,size,centering,rightVector,upVector,parameters, win_number);  

/* ---
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  real charHeight=105.;  // these are guesses !
  real charWidth=70.;    //
  real xScale=size/charHeight;
  real yScale=size/charHeight;

  real xShift=0.;
  if( centering==0 )
    xShift=string.length()*.5*xScale*charWidth;   // centre, shift by half the length
  else if( centering==+1 )                 // flush left
    xShift=string.length()*xScale*charWidth;
  real yShift=.5*yScale*charHeight;

  glTranslate(xPosition,yPosition,0.);     // translate to correct position

  glRotate(angle, 0., 0., 1.);                           // rotate about z-axis
  glTranslate(-xShift,-yShift,0.);                      // translate to center the string 
  glScale(xScale,yScale,1.);

  for( int i=0; i<string.length(); i++ )       
    strokeCharacter(string[i]);  

  glPopMatrix();
---- */

}

//=========================================================================================
//  Compute c = (a X b)/|a X b|  NORMALIZED Cross product
//  and return the angle between them (in radians, always 0<= angle <=Pi)
//=========================================================================================
void
crossProductAndAngle(const RealArray & a, const RealArray & b, RealArray & c, real & theta)
{
  c(0)=a(1)*b(2)-a(2)*b(1);
  c(1)=a(2)*b(0)-a(0)*b(2);
  c(2)=a(0)*b(1)-a(1)*b(0);

  real norm = SQRT((SQR(c(0))+SQR(c(1))+SQR(c(2))));
  if( norm!=0. )
  {
    c(0)/=norm; c(1)/=norm; c(2)/=norm;
  }
  norm = SQRT((SQR(a(0))+SQR(a(1))+SQR(a(2)))*(SQR(b(0))+SQR(b(1))+SQR(b(2))));
  if( norm!=0. )
    theta=acos((a(0)*b(0)+a(1)*b(1)+a(2)*b(2))/norm);
  else
    theta=0.;
}

void
crossProductAndAngle(const real a[3], const real b[3], real c[3], real & theta)
{
  c[0]=a[1]*b[2]-a[2]*b[1];
  c[1]=a[2]*b[0]-a[0]*b[2];
  c[2]=a[0]*b[1]-a[1]*b[0];

  real norm = SQRT((SQR(c[0])+SQR(c[1])+SQR(c[2])));
  if( norm!=0. )
  {
    c[0]/=norm; c[1]/=norm; c[2]/=norm;
  }
  norm = SQRT((SQR(a[0])+SQR(a[1])+SQR(a[2]))*(SQR(b[0])+SQR(b[1])+SQR(b[2])));
  if( norm!=0. )
    theta=acos((a[0]*b[0]+a[1]*b[1]+a[2]*b[2])/norm);
  else
    theta=0.;
}

  
//==============================================================
//   Rotate the vector x defined by the rotation of the
//   unit vector u into the unit vector v
//==============================================================
void
rot3d( const RealArray & u, const RealArray & v, RealArray & x )
{
  RealArray a(3),b(3),w(3);

  //....... a = u X v
  a(0)=u(1)*v(2)-u(2)*v(1);
  a(1)=u(2)*v(0)-u(0)*v(2);
  a(2)=u(0)*v(1)-u(1)*v(0);
  if( a(0)==0. && a(1)==0. && a(2)==0. )
  {
    //       u and v are parallel: choose a vector perpendicular
    if( u(0)!=0. || u(1)!=0. )
    {
      a(0)=-u(1);
      a(1)= u(0);
      a(2)= 0.;
    }
    else
    {
      a(0)= 0.;
      a(1)= u(2);
      a(2)=-u(1);
    }
  }
  //....... b = u X a
  b(0)=u(1)*a(2)-u(2)*a(1);
  b(1)=u(2)*a(0)-u(0)*a(2);
  b(2)=u(0)*a(1)-u(1)*a(0);
  //.......now (axisa,b,u) form an orthonormal set
  w(0)=x(0)*a(0)+x(1)*a(1)+x(2)*a(2);
  w(1)=x(0)*b(0)+x(1)*b(1)+x(2)*b(2);
  w(2)=x(0)*u(0)+x(1)*u(1)+x(2)*u(2);
  //........rotate in the plane (axisb,u)
  //        cos(axis angle ) = u dot v
  real c=u(0)*v(0)+u(1)*v(1)+u(2)*v(2);
  real s=sqrt(1.-SQR(c));
  real c1=w(1)*c-w(2)*s;
  real c2=w(1)*s+w(2)*c;
  x(0)=w(0)*a(0)+c1*b(0)+c2*u(0);
  x(1)=w(0)*a(1)+c1*b(1)+c2*u(1);
  x(2)=w(0)*a(2)+c1*b(2)+c2*u(2);
}

void
rot3d( const real u[3], const real v[3], real x[3] )
{
  real a[3],b[3],w[3];

  //....... a = u X v
  a[0]=u[1]*v[2]-u[2]*v[1];
  a[1]=u[2]*v[0]-u[0]*v[2];
  a[2]=u[0]*v[1]-u[1]*v[0];
  if( a[0]==0. && a[1]==0. && a[2]==0. )
  {
    //       u and v are parallel: choose a vector perpendicular
    if( u[0]!=0. || u[1]!=0. )
    {
      a[0]=-u[1];
      a[1]= u[0];
      a[2]= 0.;
    }
    else
    {
      a[0]= 0.;
      a[1]= u[2];
      a[2]=-u[1];
    }
  }
  //....... b = u X a
  b[0]=u[1]*a[2]-u[2]*a[1];
  b[1]=u[2]*a[0]-u[0]*a[2];
  b[2]=u[0]*a[1]-u[1]*a[0];
  //.......now (axisa,b,u) form an orthonormal set
  w[0]=x[0]*a[0]+x[1]*a[1]+x[2]*a[2];
  w[1]=x[0]*b[0]+x[1]*b[1]+x[2]*b[2];
  w[2]=x[0]*u[0]+x[1]*u[1]+x[2]*u[2];
  //........rotate in the plane (axisb,u)
  //        cos(axis angle ) = u dot v
  real c=u[0]*v[0]+u[1]*v[1]+u[2]*v[2];
  real s=sqrt(1.-SQR(c));
  real c1=w[1]*c-w[2]*s;
  real c2=w[1]*s+w[2]*c;
  x[0]=w[0]*a[0]+c1*b[0]+c2*u[0];
  x[1]=w[0]*a[1]+c1*b[1]+c2*u[1];
  x[2]=w[0]*a[2]+c1*b[2]+c2*u[2];
}



//----------------------------------------------------------------------------------------------
//  Plot a label in 3D World coordinates
//
// Input -
//  string      : label to plot
//  x(0:2)      : position of label in world coordinates
//  size        : size in world coordinates
//  centering   : centering option
//                centering = -1 : left justify
//                centering =  0 : centre
//                centering = +1 : right
//  rightVector : the label is plotted parallel to this vector
//  upVector    : this vector defines the "up" direction for the label
//----------------------------------------------------------------------------------------------

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{xlabel: plot a aString in 3D world coordinates}} 
void GL_GraphicsInterface:: 
xLabel(const aString & string,     
       const RealArray & x,  
       const real size,      
       const int centering,
       const RealArray & rightVector,  
       const RealArray & upVector,
       GraphicsParameters & parameters  /* =Overture::defaultGraphicsParameters() */,
       int win_number /* = -1 */)
// =======================================================================================
//  /Description:
//    This routine plots a label with position and size in World coordinates, 
//    This label DOES rotate and scale with the plot. This version of xLabel 
//    plots the string in the plane formed by the vectors {\ff rightVector}
//    and {\ff upVector}.
//  /string (input): aString to draw.
//  /x(0:2) (input): x,y,z coordinates of the string in world coordinates.
//    (see the centering entering argument).
//  /size (input): Size of the characters in NORMALIZED coordinates.
//  /centering (input): {\ff centering=0} means put the centre of the string at {\ff x},
//    {\ff centering=-1} means put the left end of the string at {\ff x}.
//    {\ff centering=+1} means put the right end of the string at {\ff x}.
//  /rightVector(0:2) (input): The string is drawn to lie parallel to this vector.
//  /upVector(0:2) (input): This vector defines the ``up'' direction for the characters. The
//     characters are drawn in the plane defined by the rightVector and the upVector.
//  /Errors: none.
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  if (win_number == -1)
    win_number = currentWindow;

  // These don't work in compile mode!
  // float m[16];
  // glGetFloatv(GL_MODELVIEW_MATRIX,m);
  // printf("modelview matrix: %f,%f,%f,%f, %f,%f,%f,%f, %f,%f,%f,%f, %f,%f,%f,%f\n",
  //   m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
  //   glGetFloatv(GL_PROJECTION_MATRIX,m);
  //   printf("projection matrix: %f,%f,%f,%f, %f,%f,%f,%f, %f,%f,%f,%f, %f,%f,%f,%f\n",
  // 	 m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);


  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
//  glLoadIdentity(); // ***** wdh961201

  real charHeight=105.;  // these are guesses !
//  real charWidth=80.; // 70.;    //
  real xScale=size/charHeight;
  real yScale=size/charHeight;

  // get the length of the label for centering, ignore leading blanks
  int labelLength=string.length();
  int length=labelLength;
  int firstChar=0;
  int n;
  for( n=0; n<length && string[n]==' ' ; n++ )  // ignore leading blanks
    firstChar++;
  labelLength-=firstChar;
  // a '.' does not count in the length beacuse it is so short
  for( n=firstChar; n<length; n++ )
  {
    if( string[n]=='.' )
      labelLength--;
  }
  
  real xPos=0.;
  if( centering==0 )
    xPos=labelLength*.5*size; // xScale*charWidth;   // centre, shift by half the length
  else if( centering==+1 )                        // flush left
    xPos=labelLength*size; // xScale*charWidth;
  real yPos=.5*size; // yScale*charHeight;

  glTranslate(x(0),x(1),x(2));                               // translate to correct position

//  glScale(1./m[0],1./m[5],1./m[10]);  // rescale so letters are correct aspect ratio
   // rescale so letters are correct aspect ratio:
  glScale(1./windowScaleFactor[win_number][0], 1./windowScaleFactor[win_number][1],
	  1./windowScaleFactor[win_number][2]); 
  // printf("xLabel: windowScaleFactor=%e,%e,%e\n",windowScaleFactor[0],windowScaleFactor[1],windowScaleFactor[2]);
  

  real angle,angle4,angle5;
  RealArray v3(3),xVector(3),zVector(3),v4(3),rv1(3),v5(3);

  crossProductAndAngle(rightVector,upVector,v3,angle);

  xVector=0.; xVector(0)=1.; zVector=0.; zVector(2)=1.; 
  crossProductAndAngle(zVector,v3,v4,angle4);
  // rotate the x-axis and v1 in the same way as we rotate the z-axis below
  rot3d(zVector,v3,xVector);  // rotate the x-axis in the same way as we rotate the z-axis below
  rv1=rightVector;            // do not rotate the rightVector because it is already in the plane we want
  // rot3d(v3,zVector,rv1);      // rot3d(zVector,v3,rv1);
  crossProductAndAngle(xVector,rv1,v5,angle5);  // here is how we rotate the rotated x-axis into 
                                                // rv1

  glRotate(angle5*180./Pi,v5(0),v5(1),v5(2));
  glRotate(angle4*180./Pi,v4(0),v4(1),v4(2));   // rotate z-axis into the normal to v1,v2

  glTranslate(-xPos,-yPos,0.);                      // translate to center the string 
  glScale(xScale,yScale,1.);

  glLineWidth(parameters.size(lineWidth)*parameters.size(labelLineWidth)*lineWidthScaleFactor[win_number]);
  for( int i=firstChar; i<string.length(); i++ )       
    strokeCharacter(string[i]);  
//    glutStrokeCharacter(font,string[i]);  // 

  glPopMatrix();

}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{xlabel: plot a aString in 3D world coordinates}} 
void GL_GraphicsInterface:: 
xLabel(const aString & string,     
       const real x[3],  
       const real size,      
       const int centering,
       const real rightVector[3],  
       const real upVector[3],
       GraphicsParameters & parameters  /* =Overture::defaultGraphicsParameters() */,
       int win_number /* = -1 */)
// =======================================================================================
//  /Description:
//    This routine plots a label with position and size in World coordinates, 
//    This label DOES rotate and scale with the plot. This version of xLabel 
//    plots the string in the plane formed by the vectors {\ff rightVector}
//    and {\ff upVector}.
//  /string (input): aString to draw.
//  /x(0:2) (input): x,y,z coordinates of the string in world coordinates.
//    (see the centering entering argument).
//  /size (input): Size of the characters in NORMALIZED coordinates.
//  /centering (input): {\ff centering=0} means put the centre of the string at {\ff x},
//    {\ff centering=-1} means put the left end of the string at {\ff x}.
//    {\ff centering=+1} means put the right end of the string at {\ff x}.
//  /rightVector(0:2) (input): The string is drawn to lie parallel to this vector.
//  /upVector(0:2) (input): This vector defines the ``up'' direction for the characters. The
//     characters are drawn in the plane defined by the rightVector and the upVector.
//  /Errors: none.
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  if (win_number == -1)
    win_number = currentWindow;

  int ii;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  real charHeight=105.;  // these are guesses !
//  real charWidth=80.; // 70.;    //
  real xScale=size/charHeight;
  real yScale=size/charHeight;

  // get the length of the label for centering, ignore leading blanks
  int labelLength=string.length();
  int length=labelLength;
  int firstChar=0;
  int n;
  for( n=0; n<length && string[n]==' ' ; n++ )  // ignore leading blanks
    firstChar++;
  labelLength-=firstChar;
  // a '.' does not count in the length beacuse it is so short
  for( n=firstChar; n<length; n++ )
  {
    if( string[n]=='.' )
      labelLength--;
  }
  
  real xPos=0.;
  if( centering==0 )
    xPos=labelLength*.5*size; // xScale*charWidth;   // centre, shift by half the length
  else if( centering==+1 )                        // flush left
    xPos=labelLength*size; // xScale*charWidth;
  real yPos=.5*size; // yScale*charHeight;

  glTranslate(x[0],x[1],x[2]);                               // translate to correct position

//  glScale(1./m[0],1./m[5],1./m[10]);  // rescale so letters are correct aspect ratio
   // rescale so letters are correct aspect ratio:

// AP: This scaling makes the letters smaller when we zoom in and bigger when we zoom out. Is that what we want?
  glScale(1./windowScaleFactor[win_number][0], 1./windowScaleFactor[win_number][1],
	  1./windowScaleFactor[win_number][2]); 

  // printf("windowScaleFactor=%e,%e,%e\n",windowScaleFactor[0],windowScaleFactor[1],windowScaleFactor[2]);
  

  real angle,angle4,angle5;
  real v3[3],xVector[3],zVector[3],v4[3],rv1[3],v5[3];

  crossProductAndAngle(rightVector,upVector,v3,angle); // AP update!!!

  for (ii=0; ii<3; ii++)
  {
    xVector[ii]=0.; 
    zVector[ii]=0.;
  }
  
  xVector[0]=1.;  zVector[2]=1.; 
  crossProductAndAngle(zVector,v3,v4,angle4);
  // rotate the x-axis and v1 in the same way as we rotate the z-axis below
  rot3d(zVector,v3,xVector);  // rotate the x-axis in the same way as we rotate the z-axis below
  for (ii=0; ii<3; ii++)
    rv1[ii]=rightVector[ii];     // do not rotate the rightVector because it is already in the plane we want
  // rot3d(v3,zVector,rv1);      // rot3d(zVector,v3,rv1);
  crossProductAndAngle(xVector,rv1,v5,angle5);  // here is how we rotate the rotated x-axis into 
                                                // rv1

  glRotate(angle5*180./Pi,v5[0],v5[1],v5[2]);
  glRotate(angle4*180./Pi,v4[0],v4[1],v4[2]);   // rotate z-axis into the normal to v1,v2

  glTranslate(-xPos,-yPos,0.);                      // translate to center the string 
  glScale(xScale,yScale,1.);

  glLineWidth(parameters.size(lineWidth)*parameters.size(labelLineWidth)*lineWidthScaleFactor[win_number]);
  for( int i=firstChar; i<string.length(); i++ )       
    strokeCharacter(string[i]);  
//    glutStrokeCharacter(font,string[i]);  // 

  glPopMatrix();

}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{plotLabels}} 
void GL_GraphicsInterface::
plotLabels(GraphicsParameters & parameters,
           const real & labelSize /* =-1. */,
           const real & topLabelHeight /* =.925 */,
           const real & bottomLabelHeight /* =-.925 */,
	   int win_number /* = -1 */)
// =================================================================
// /Description:
//    Plot labels.
// /labelSize (input) : if <= 0 use default in parameters
// Utility routine used to plot labels from a GraphicParameters 
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
//  glEnable(GL_LINE_SMOOTH);
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  if (win_number == -1)
    win_number = currentWindow;

  real size    = labelSize<= 0. ? parameters.size(topLabelSize)    : labelSize;
  real subSize = labelSize<= 0. ? parameters.size(topSubLabelSize) : labelSize*.9;
  const real spacingFactor=1.5; // relative spacing between lines 
  const int maxCharsPerLine=65;

  // shift the position if the scrren is not square
  real yb=0.; // shift in the display function instead!
  real ya=0.;

//  GLenum errCode;
//  const GLubyte *errString;
  
//    if ((errCode=glGetError()) != GL_NO_ERROR)
//    {
//      errString = gluErrorString(errCode);
//      printf("plotLabel: OpenGL Error: %s\n", errString);
//    }
  real y0=yb;
  if( parameters.topLabel!="" && parameters.topLabel!=" ")
  {
    glNewList(getTopLabelDL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 
    label(parameters.topLabel,.0,y0,size,0,0.,parameters);
    glEndList();
    labelsPlotted[win_number]=TRUE;
    y0 -= size*spacingFactor;
  }
  if( parameters.topLabel1!="" && parameters.topLabel1!=" " )
  {
    glNewList(getTopLabel1DL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 

    // If the label is too long, split into multiple lines:
    if( parameters.topLabel1.length() < maxCharsPerLine )
    {
      label(parameters.topLabel1,.0,y0,subSize,0,0.,parameters);
      y0-= subSize*spacingFactor;
    }
    else
    {
      const real zOffset= -.99; // don't keep these labels on top since there may be too many
      const aString & text = parameters.topLabel1;
      int iStart=0, iEnd=maxCharsPerLine-1;
      while( iStart < text.length() )
      {
	if( iEnd<text.length()-1 )
	{
	  // look for a previous blank to split the line at (if we don't make the line too short)
	  const int minCharsPerLine = maxCharsPerLine-10;
	  while( text[iEnd] != ' ' && (iEnd-iStart > minCharsPerLine) ) iEnd--;
	}
	//kkc 081217	aString line = text(iStart,iEnd);
	aString line = text.substr(iStart, iEnd-iStart+1);
	label(line,.0,y0,subSize,0,0.,parameters,nullString,zOffset);
        y0-= subSize*spacingFactor;
	iStart=iEnd+1;
	iEnd=min(text.length()-1, iStart+maxCharsPerLine-1);
      }
    }
    glEndList();
    labelsPlotted[win_number]=TRUE;
  }
  if( parameters.topLabel2!="" && parameters.topLabel2!=" " )
  {
    glNewList(getTopLabel2DL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 
    label(parameters.topLabel2,.0,y0,subSize,0,0.,parameters);
    y0-= subSize*spacingFactor;
    glEndList();
    labelsPlotted[win_number]=TRUE;
  }
  if( parameters.topLabel3!="" && parameters.topLabel3!=" " )
  {
    glNewList(getTopLabel3DL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 
    label(parameters.topLabel3,.0,y0,subSize,0,0.,parameters);
    y0-= subSize*spacingFactor;
    glEndList();
    labelsPlotted[win_number]=TRUE;
  }

  size    = labelSize<= 0. ? parameters.size(bottomLabelSize)    : labelSize;
  subSize = labelSize<= 0. ? parameters.size(bottomSubLabelSize) : labelSize*.9;
  if( parameters.bottomLabel!="" )
  {
    glNewList(getBottomLabelDL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 
    label(parameters.bottomLabel,.0,ya,size,0,0.,parameters);
    glEndList();
    labelsPlotted[win_number]=TRUE;
  }
  if( parameters.bottomLabel1!="" )
  {
    glNewList(getBottomLabel1DL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 
    label(parameters.bottomLabel1,.0,ya+size*spacingFactor,subSize,0,0.,parameters);
    glEndList();
    labelsPlotted[win_number]=TRUE;
  }
  if( parameters.bottomLabel2!="" )
  {
    glNewList(getBottomLabel2DL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 
    label(parameters.bottomLabel2,.0,ya+(size+subSize)*spacingFactor,subSize,0,0.,parameters);
    glEndList();
    labelsPlotted[win_number]=TRUE;
  }
  if( parameters.bottomLabel3!="" )
  {
    glNewList(getBottomLabel3DL(win_number),GL_COMPILE);
    setColour(textColour); //  label colour 
    label(parameters.bottomLabel3,.0,ya+(size+2*subSize)*spacingFactor,subSize,0,0.,parameters);
    glEndList();
    labelsPlotted[win_number]=TRUE;
  }

//  glDisable(GL_LINE_SMOOTH);
//  glDisable(GL_BLEND);

}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{eraseLabels}} 
void GL_GraphicsInterface::
eraseLabels(GraphicsParameters & parameters, int win_number /* = -1*/)
// =================================================================
// /Description:
//    Erase the labels.
//    Utility routine used to erase title labels.
//  /Return Values: none.
//  
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  if (win_number == -1)
    win_number = currentWindow;

//  if( parameters.topLabel!="" )
    glDeleteLists(getTopLabelDL(win_number),1);  // clear the title
//  if( parameters.topLabel1!="" )
    glDeleteLists(getTopLabel1DL(win_number),1);  // clear the title
//  if( parameters.topLabel2!="" )
    glDeleteLists(getTopLabel2DL(win_number),1);  // clear the title
//  if( parameters.topLabel3!="" )
    glDeleteLists(getTopLabel3DL(win_number),1);  // clear the title

//  if( parameters.bottomLabel!="" )
    glDeleteLists(getBottomLabelDL(win_number),1);  // clear the title
//  if( parameters.bottomLabel1!="" )
    glDeleteLists(getBottomLabel1DL(win_number),1);  // clear the title
//  if( parameters.bottomLabel2!="" )
    glDeleteLists(getBottomLabel2DL(win_number),1);  // clear the title
//  if( parameters.bottomLabel3!="" )
    glDeleteLists(getBottomLabel3DL(win_number),1);  // clear the title

  labelsPlotted[win_number]=FALSE;
}
