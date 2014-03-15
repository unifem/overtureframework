#include "Overture.h"
#include "Square.h"
#include "Box.H"
#include "BoxAssoc.H"

int
main()
{

  SquareMapping square;
  MappedGrid mg(square);
  mg.update();
  cout << "here is mg.box = " << mg.box << endl;
  
  INTVECT lo(mg.indexRange(Start,axis1),mg.indexRange(Start,axis2),mg.indexRange(Start,axis3));
  INTVECT hi(mg.indexRange(End  ,axis1),mg.indexRange(End  ,axis2),mg.indexRange(End  ,axis3));
  INTVECT centering(IndexType::NODE,IndexType::NODE,IndexType::NODE);
  
  BOX b(lo,hi,centering);
  
  cout << "here is b = " << b << endl;

  BoxAssoc ba;

  return 0;
}
