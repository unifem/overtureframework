#include "Overture.h"
#include "Square.h"

int 
main(int argc, char* argv[]) 
{
  ios::sync_with_stdio();
  Index::setBoundsCheck(On);

  SquareMapping mapping(-1., 1., -1., 1.);            // Create a SquareMapping
  
  MappedGrid mg(mapping);      // grid for a mapping
  mg.consistencyCheck();
  mg.update();
  mg.consistencyCheck();
      
//  mg.vertex.display("Here are the vertices");
  printf(" **** done*** \n");
  
  return 0;
}

