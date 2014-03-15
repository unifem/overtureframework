// created by Bobby Philip 09052001
#include <Overture.h>
#include "Square.h"
#include "ParentChildSiblingInfo.h"
#include "ListOfParentChildSiblingInfo.h"
#include "ParentInfo.h"
#include "ChildInfo.h"
#include "SiblingInfo.h"
 
// TEST 0 simply creates a GridCollection with one MappedGrid and checks that
// a ParentChildSiblingInfo object is correctly created
int main( int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture
  
  cout << "TEST 0: create a GridCollection with a single nonPeriodic MappedGrid " << endl;
  cout << "TEST 0: and build a ParentChildSiblingInfo object" << endl;

  SquareMapping mapping(-1., 1., -1., 1.);            // Create a SquareMapping
  mapping.setGridDimensions(axis1,11); mapping.setGridDimensions(axis2,11);

  MappedGrid mg(mapping);      // grid for a mapping
  mg.update();

  //  Create a two-dimensional GridCollection with one grid.
  GridCollection gc(2,1);
  gc[0].reference(mg);   
  gc.updateReferences();
  gc.update(MappedGrid::THEvertex);
  gc.update(GridCollection::THErefinementLevel); // build the refinement level collections

  ListOfParentChildSiblingInfo listOfPCSInfo;
  assert( listOfPCSInfo.listLength() == 0 );
#ifdef TESTING
  cout << "entering ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, listOfPCSInfo )" << endl;
#endif
  ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, listOfPCSInfo );
  if(listOfPCSInfo.listLength()==1)
    {
      assert( listOfPCSInfo[0].getParents().size()  == 0 );
      assert( listOfPCSInfo[0].getChildren().size() == 0 );
      assert( listOfPCSInfo[0].getSiblings().size() == 0 );
    }
  else
    {
      cout << "TEST 0 FAILED!!!" << endl;
      cout << "Incorrect number of ParentChildSiblingInfo objects created" << endl;
      cout << "Debug info for the ParentChildSiblingInfo object created!" << endl;
      cout << listOfPCSInfo[0] << endl;
      exit(1);
    }

  cout << "Debug info for the ParentChildSiblingInfo object created!" << endl;
  cout << listOfPCSInfo[0] << endl;

  Overture::finish();          
  return 0;
}
