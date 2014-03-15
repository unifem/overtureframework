//===============================================================================
//  Test the ShowFileReader
//    o read a solution and grid from a show file
//    o interpolate the solution from one grid to another grid
//==============================================================================
#include "Overture.h"
#include "Ogshow.h"  
#include "ShowFileReader.h"
#include "interpPoints.h"
#include "GL_GraphicsInterface.h"

#include <sys/resource.h>

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  

  aString nameOfShowFile;
  cout << ">> Enter the name of the (old) show file:" << endl;
  cin >> nameOfShowFile;
  ShowFileReader showFileReader(nameOfShowFile);

  int numberOfFrames=showFileReader.getNumberOfFrames();
  int numberOfSolutions = max(1,numberOfFrames);
  int solutionNumber;

  CompositeGrid cg;
  realCompositeGridFunction u;

  GL_GraphicsInterface ps;          // create a GL_GraphicsInterface object
  GraphicsParameters psp;           // create an object that is used to pass parameters
    
  aString answer,answer2;
  aString menu[] = { "get a solution",
                    "interpolate to new grid",
		    "exit",
                    "" };
  const aString *headerComment;
  int numberOfHeaderComments;
  char buff[80];
  
  for(;;)
  {
    ps.getMenuItem(menu,answer);
    if( answer=="get a solution" )
    {
      // In this case the user is asked to choose a solution to read in
      // Choosing a number that is too large will cause the last solution to be read 

      aString line;
      ps.inputString(line,sPrintF(buff,"Enter the solution number, [1,%i] \n",numberOfSolutions));
      sscanf(line,"%i",&solutionNumber);

      showFileReader.getASolution(solutionNumber,cg,u);        // read in a grid and solution

      // read any header comments that go with this solution
      headerComment=showFileReader.getHeaderComments(numberOfHeaderComments);

      for( int i=0; i<numberOfHeaderComments; i++ )
        printf("Header comment: %s \n",(const char *)headerComment[i]);

      psp.set(GI_TOP_LABEL,sPrintF(buff,"solution number %i",solutionNumber));
      ps.erase();
      PlotIt::contour(ps,u,psp);  
    }
    else if( answer=="interpolate to new grid" )
    {
      // In this case we read a solution from the showFile and save it in the grid cg and grid function u.
      // We then read in a different CompositeGrid, cgTo, and create a grid function uTo. We get values
      // on uTo by interpolating from u
      solutionNumber=1;
      showFileReader.getASolution(solutionNumber,cg,u);

      aString nameOfOGFile;
      CompositeGrid cgTo;
      ps.inputString(nameOfOGFile,">> Enter the name of the CompositeGrid file (to interpolate to):");

      getFromADataBase(cgTo,nameOfOGFile);          // read from a data base file
      cgTo.update();
      Range all;
      realCompositeGridFunction uTo(cgTo,all,all,all,10);

      interpolateAllPoints( u,uTo );  // interpolate uTo from u

      psp.set(GI_TOP_LABEL,sPrintF(buff,"interpolated solution at number %i",solutionNumber));
      ps.erase();
      PlotIt::contour(ps,uTo,psp); 
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  Overture::finish();
  return 0;
}
