#ifndef CGSHOW_H 
#define CGSHOW_H  "cgshow.h"

#include "Overture.h"
#include <GL/glut.h>

void label(const aString & string,     
	   const real xPosition, 
	   const real yPosition,
	   const real size=.1,
	   const int centering=0, 
	   const real angle=0. );

void xLabel(const aString & string,     
	    const real xPosition, 
	    const real yPosition,
	    const real size=.1,
	    const int centering=0, 
	    const real angle=0. );

class Cgshow
{
 public:

  CompositeGrid cg;

  int solution;
  int component;

  Cgshow();
  ~Cgshow();
  
  void contour( realCompositeGridFunction & u );
  void grid();
  void surface();
};
  
#endif
