#include "Overture.h"
#include "Square.h"

int
main()
{

  SquareMapping map;
  MappedGrid mgGlobal;

  map.setGridDimensions ( axis1, 17 );
  map.setGridDimensions ( axis2, 17 );

//  mgGlobal = MappedGrid( map );  //this works
  mgGlobal.reference(map);        // but this is faster
  mgGlobal.update();              // update "usual stuff"
  mgGlobal.update(MappedGrid::THEboundingBox);  

  mgGlobal.boundingBox().display("Here is the bounding box");
  return 0;
}
