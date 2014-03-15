#include "Cgsh.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "Square.h"
#include "HDF_DataBase.h"

//
// Check for memoery leaks
//
int 
main() 
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile;
  cout << "Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // Create two CompositeGrid objects, cg[0] and cg[1]
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);             // read cg[0] from a data-base file
  cg.update();
  cg.update(CompositeGrid::THEinverseVertexDerivative); // make sure this array is there
  cg.update(CompositeGrid::THEvertexBoundaryNormal); 

  HDF_DataBase db;   // make a data base
  db.mount("junk.hdf","I");    // open the data base, I=initialize
  char buff[80];
  
  for( int j=0; j<50; j++ )
  {
    // cout << "Ogshow: put the GridCollection in frame = " << frameNumber << endl;

    CompositeGrid cg2 = cg;
    // first destroy any big geometry arrays: (but not the mask)
    cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
    cg.put(db,sPrintF(buff,"CompositeGrid%i",j));
    if( j % 2 == 0 )
      printf("**** cg2=cg: number of A++ arrays = %i \n",Array_Descriptor_Type::getMaxNumberOfArrays());
  }
  db.unmount();


/* -----

  Mapping::debug=7; 
//  SquareMapping square,square2;
  
  SquareMapping & square  = *(new SquareMapping);
  square.incrementReferenceCount();
  SquareMapping & square2 = *(new SquareMapping);
  square2.incrementReferenceCount();

  square2.setName( Mapping::mappingName,"square2");

  MappingRC map1(square), map2(square2), map3, map4, map5;

  map4.reference(map1);
// ok  map2=map4;
  map2.reference(map4);
  
  map3=map1;  
  map3.getMapping().setName( Mapping::mappingName,"square3");
//  map3.display("Here is map3");
  
  if( square.decrementReferenceCount()==0 )
    delete &square;
  
  cout << "square2.name = " << square2.getName(Mapping::mappingName) << endl;
  if( square2.decrementReferenceCount()== 0 )
    delete &square2;

----- */

  return 0;

}
