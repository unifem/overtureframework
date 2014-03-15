#include "Overture.h"  
#include "GL_GraphicsInterface.h"
#include "Annulus.h"
#include "Square.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  GL_GraphicsInterface ps(TRUE,"test");      // create a GL_GraphicsInterface object
  GraphicsParameters psp;                       // This object is used to change plotting parameters
  char buffer[80];

  SquareMapping square;
  AnnulusMapping annulus;
  
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  ps.plot(annulus,(GraphicsParameters&)psp);
  psp.set(GI_USE_PLOT_BOUNDS,TRUE);
  ps.plot(square,(GraphicsParameters&)psp);
  
  aString answer,menu[]=
  {
    "exit",
    ""
  };
  for( ;; )
  {
    ps.getMenuItem(menu,answer,"c>");
    if( answer=="exit" )
      break;
  }
  
  
  return 0;
}

