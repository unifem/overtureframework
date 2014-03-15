#include "Overture.h"
#include "MappingInformation.h"
#include "ShowFileReader.h"


int
readMappingsFromAnOverlappingGridFile( MappingInformation & mapInfo, const aString & ogFileName=nullString )
// /Description:
//    Import the Mappings that belong to an overlapping grid.
// /ogFileName (input) : optional name of the overlapping grid file.
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & graphicsInterface = *mapInfo.graphXInterface;
    

  // FILE *fp=NULL;
  aString fileName;

  if( ogFileName!="" && ogFileName!=" " )
    fileName=ogFileName;
  else
    graphicsInterface.inputString(fileName,"Enter the name of the overlapping grid or show file");

  ShowFileReader showFileReader(fileName);

  CompositeGrid cg;

  int numberOfFrames=showFileReader.getNumberOfFrames();
  if( numberOfFrames<=0 )
  {
    getFromADataBase(cg,fileName);
  }
  else
  {

    int solutionNumber=1;
    if( numberOfFrames>1 )
    {
      printf("There were %i solutions found in this show file. Choose one to use.\n",numberOfFrames);
      aString line;
      char buff[100];
      graphicsInterface.inputString(line,sPrintF(buff,"Enter a solution to use from 1..%i\n",numberOfFrames));
      if( line!="" )
      {
	sScanF(line,"%i",&solutionNumber);
	solutionNumber=max(1,min(solutionNumber,numberOfFrames));
	printf("Reading solution %i...\n",solutionNumber);
      }
    }
    realCompositeGridFunction u;
    showFileReader.getASolution(solutionNumber,cg,u);
  }
  
  const int numberOfDimensions = cg.numberOfDimensions();

  printf("There are %i grids in this overlapping grid \n",cg.numberOfGrids());
  for( int grid=0; grid<cg.numberOfGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const IntegerArray & bc = mg.boundaryCondition();
    Mapping & map = mg.mapping().getMapping();
    map.getGrid();   // do this so grid in the mapping is created

    // *wdh* 2012/03/05 -- make sure the boundary conditions in the mapping match those in the grid
    for( int axis=0; axis<numberOfDimensions; axis++ )for( int side=0; side<=1; side++ )
    {
      // printF("grid=%i: bc(%i,%i)=%i\n",grid,side,axis,bc(side,axis));
      map.setBoundaryCondition(side,axis,bc(side,axis));
    }

    mapInfo.mappingList.addElement(cg[grid].mapping());
    printf("Mapping %i is named %s\n",grid,(const char*)cg[grid].mapping().getName(Mapping::mappingName));
  }
  return 0;  

}
