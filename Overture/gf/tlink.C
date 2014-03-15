#include <Overture.h>
main()
{


  Index::setBoundsCheck (on);
  ios::sync_with_stdio ();

  GridFunctionParameters cellCentered;
  cellCentered.inputType = GridFunctionParameters::cellCentered; 


// ... Create composite grid; read in from database (HDF) file

  CompositeGrid cg;
  getFromADataBase (cg, "../cgsh/square5cc.hdf");
  cg.update ();

// ... Create grid function

  int numberOfVelocityComponents = 2;
  floatCompositeGridFunction q    (cg, cellCentered, numberOfVelocityComponents);

  q.positionOfComponent.display("q.positionOfComponent");
  q.positionOfCoordinate.display("q.positionOfCoordinate");

// ... link the components

  floatCompositeGridFunction xVelocity;  xVelocity.link (q, Range(0,0));         
  floatCompositeGridFunction yVelocity;  yVelocity.link (q, Range(1,1));

  xVelocity = 1.;
  yVelocity = 0.;
    
  cout << "all done" << endl;
}

