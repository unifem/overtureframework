#include "Mapping.h"
//#include "GL_GraphicsInterface.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"

int 
SaveMappingInFile( MappingInformation & mapInfo )
//===============================================================================================
//  Save a Mapping in a file 
//
// supported file types:
//   plot3d format
//
//===============================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & graphicsInterface = *mapInfo.graphXInterface;
    

  FILE *fp=NULL;
  aString fileName;
  graphicsInterface.inputString(fileName,"Enter the name of the IGES file");

  fp=fopen((const char*)fileName,"r");


***** to finish ******

  
}
