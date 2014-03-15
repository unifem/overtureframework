#include "Overture.h"
#include "GL_GraphicsInterface.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  GL_GraphicsInterface ps(TRUE,"Surface");                  // create a GL_GraphicsInterface object
  GraphicsParameters psp;                      // This object is used to change plotting parameters

  int n=41;
  int nv=21;
  realArray x(n), u(n,nv), t(nv);
  
  real h=1./(n-1);
  real dt=1./(nv-1);
  for( int j=0; j<nv; j++ )
    t(j)=j*dt;
  for( int i=0; i<n; i++ )
  {
    x(i)=i*h;
    for( int j=0; j<nv; j++ )
      u(i,j)=sin(2.*Pi*(x(i)-t(j)));
  }
  
  ps.setAxesLabels("x","t","u");
  psp.set(GI_TOP_LABEL,"MySurface");
  ps.plot(x,t,u,psp);
  
  return 0;
}
