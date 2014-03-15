// created by Bobby Philip 09052001
#include "Overture.h"
#include "Square.h"
#include "ParentChildSiblingInfo.h"

// TEST 1 reads in an overlapping grid with no refinement levels,
// creates ParentChildSiblingInfo objects for the base grids, and
// checks that no parents, children, or siblings are created

int main( int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  if( argc == 2 )
    {
      aString nameOfOGFile = argv[1]; // initialize with name      
      CompositeGrid cg;
      getFromADataBase(cg,nameOfOGFile);
      cg.update();
      cg.update(GridCollection::THErefinementLevel); // build the refinement level collections
      ListOfParentChildSiblingInfo listOfPCSInfo;
      assert( listOfPCSInfo.listLength() == 0 );
      ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( (GridCollection &) cg, listOfPCSInfo );
      cout << "Number of ParentChildSiblingInfo objects created is " << listOfPCSInfo.getLength() << endl;

      for( int i = 0; i < cg.numberOfGrids(); i++)
	{
	  assert( listOfPCSInfo[i].getParents().size()  == 0 );
	  assert( listOfPCSInfo[i].getChildren().size() == 0 );
	  assert( listOfPCSInfo[i].getSiblings().size() == 0 );
	}
      cout << "TEST 1: Passed!" << endl;
    }
  else
    {
      cout << "ERROR: specify an overlapping grid as argument" << endl;
      cout << "Usage: test1 gridname.hdf" << endl;	
    }

  Overture::finish();
  return 0;
}
