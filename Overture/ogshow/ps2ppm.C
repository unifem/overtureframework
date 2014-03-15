#include "GL_GraphicsInterface.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  aString fileName, ppmFileName;

  if( argc > 2 )
  {
    fileName=argv[1];
    ppmFileName=argv[2];
  }
  else
  {
    cout << "Type: `ps2ppm <file.ps> <file.ppm> to convert the plotStuff generated file.ps to a .ppm file\n";
    return 1;
  }

  GL_GraphicsInterface gi(FALSE);

  int returnValue=gi.psToRaster(fileName,ppmFileName);

  Overture::finish();          

  return returnValue;
}




