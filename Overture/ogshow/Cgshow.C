#include "Cgshow.h"
#include <GL/glut.h>


Cgshow::
Cgshow()
{
}

Cgshow:: 
~Cgshow()
{
}

  
/* -----
void Cgshow::
contour()
{
  cout << "Inside Cgshow:contour\n";
  
  int list=3;
  glNewList(list,GL_COMPILE);
  glColor3f(1.,0.,0.);
  glBegin(GL_POLYGON);
  for( int i=0; i<50; i++)
    glVertex2f(cos(twoPi*i/100.),sin(twoPi*i/100.));
  glEnd();

  glColor3f(0.,1.,0.);
  glBegin(GL_POLYGON);
  for( i=50; i<100; i++)
    glVertex2f(1.5*cos(twoPi*i/100.),1.5*sin(twoPi*i/100.));
  glEnd();

  glColor3f(0.,0.,1.);
  glBegin(GL_POLYGON);
  for( i=25; i<75; i++)
    glVertex2f(2.0*cos(twoPi*i/100.),2.0*sin(twoPi*i/100.));
  glEnd();

  glEndList();
}
----- */



void Cgshow::
surface()
{
  cout << "Inside Cgshow:surface\n";
}

