#include "Cgsh.h"
#include "Square.h"
#include "HDF_DataBase.h"

//
// Generate overlapping grids.
//
int 
main(int argc, char* argv[]) 
{
  ios::sync_with_stdio();
  Index::setBoundsCheck(On);

  aString nameOfOGFile;
  cout << " Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  //
  //  Create a SquareMapping.
  //
  PlotStuff plotStuff;
  cout << "Plot initial grid \n";
  PlotIt::plot(plotStuff,cg);

  Mapping & map = *(cg[0].mapping().mapPointer);
  ReparameterizationTransform mapping(map,ReparameterizationTransform::restriction);
  mapping.setGridDimensions(axis1,mapping.getGridDimensions(axis1)*2);
  mapping.setGridDimensions(axis2,mapping.getGridDimensions(axis2)*2);

  cg[cg.numberOfGrids-1].reference(mapping);

  cout << "main():  mapping.setBounds(0.,1.,0.,1.,0.,0.);" << endl;
  mapping.setBounds(0.,1.,0.,1.,0.,0.); 

  cout << "Plot ReparameterizationTransform \n";
  PlotIt::plot(plotStuff,mapping);

  cg.destroy(CompositeGrid::EVERYTHING);
  cg.update(CompositeGrid::EVERYTHING);
  cout << "Plot cg \n";
  PlotIt::plot(plotStuff,cg);

  cout << "main():  mapping.setBounds(.25,.75,.25,.75,0.,0.);" << endl;
  mapping.setBounds(.25,.75,.25,.75,0.,0.); 
  cout << "Plot ReparameterizationTransform \n";
  PlotIt::plot(plotStuff,mapping);

  cg.destroy(CompositeGrid::EVERYTHING);
  cg.update(CompositeGrid::EVERYTHING);
  cout << "Plot cg \n";
  PlotIt::plot(plotStuff,cg);

  if( TRUE )
  {
    // recursive level of transformation
    Mapping & map = *(cg[0].mapping().mapPointer);
    ReparameterizationTransform mapping(map,ReparameterizationTransform::restriction);

    cg[cg.numberOfGrids-1].reference(mapping);

    cout << "main(): (recursive) setBounds(.25,.75,.25,.75,0.,0.);" << endl;
    mapping.setBounds(.25,.75,.25,.75,0.,0.); 

    cg.destroy(CompositeGrid::EVERYTHING);
    cg.update(CompositeGrid::EVERYTHING);
    cout << "Plot cg \n";
    PlotIt::plot(plotStuff,cg);

    cout << "main(): (recursive) scaleBounds(.25,.75,.25,.75,0.,0.);" << endl;
    mapping.scaleBounds(.25,.75,.25,.75,0.,0.); 

    cg.destroy(CompositeGrid::EVERYTHING);
    cg.update(CompositeGrid::EVERYTHING);
    cout << "Plot cg \n";
    PlotIt::plot(plotStuff,cg);

  }
  
  return 0;
}
