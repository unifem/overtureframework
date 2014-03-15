// created by Bobby Philip 09052001
#include "Overture.h"
#include "Square.h"
#include "ParentChildSiblingInfo.h"

// TEST 6: create a base periodic grid, periodic in two directions,
// create a refinement patch that covers the whole base grid, and
// check to see that the periodic siblings are created correctly
// we vary the refinement factor in each direction just for additional
// completeness
int main( int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture


  SquareMapping mapping;            // Create a square
  mapping.setGridDimensions(axis1,21); mapping.setGridDimensions(axis2,11);

  MappedGrid mg(mapping);      // grid for a mapping
  for( int i = 0; i < mg.numberOfDimensions(); i++)
    mg.setIsPeriodic(i, Mapping::derivativePeriodic );

  mg.update();

  GridCollection gc(2,1);
  gc[0].reference(mg);   
  gc.updateReferences();
  gc.update(MappedGrid::THEvertex);
  gc.update(GridCollection::THErefinementLevel); // build the refinement level collections

  IntegerArray rangeA(2,3), factorA(3);
  Integer level = 1;
  int grid = 0;                              // refine this base grid

  bool retVal = TRUE;
  for(i=1; i<=4; i++)
    for(int j=1;j<=4; j++)
      {
	if((i!=1)||(j!=1))
	  {
	    factorA(0) = i;
	    factorA(1) = j;
	    factorA(2) = 1;
	    rangeA(0,0) = 0; rangeA(1,0) = 11;
	    rangeA(0,1) = 0; rangeA(1,1) = 11;
	    rangeA(0,2) = 0; rangeA(1,2) =  0;
	    // add first refinement patch
	    gc.addRefinement(rangeA, factorA, level, grid);    // add a refinement grid to level 1
	    gc.update(GridCollection::THErefinementLevel); // build the refinement level collections
	    assert(gc.numberOfGrids()==2);
	    assert(gc.numberOfRefinementLevels()==2);
	    int side, axis;  // set the number of ghost points equal to the refinement factor
	    for( axis=0; axis<=2; axis++)
	      for( side=0; side<=1; side++)
		gc[1].setNumberOfGhostPoints( side, axis, factorA(axis));

	    ListOfParentChildSiblingInfo listOfPCSInfo;
	    assert( listOfPCSInfo.listLength() == 0 );	    
	    ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, listOfPCSInfo );
	    assert( listOfPCSInfo.listLength() == 2 ); // one for parent, one for child
	    if( ( listOfPCSInfo[0].getParents().size() ==0) &&
		( listOfPCSInfo[0].getChildren().size()==1) &&
		( listOfPCSInfo[0].getSiblings().size()==0) &&
		( listOfPCSInfo[1].getParents().size() ==1) &&
		( listOfPCSInfo[1].getChildren().size()==0) &&
		( listOfPCSInfo[1].getSiblings().size()==2))
	      retVal = retVal && TRUE;
	    else
	      retVal = retVal && FALSE;

	    gc.deleteRefinementLevels(0); // delete the new refinement level to reinitialize
	    listOfPCSInfo.destroy();
	    gc.update(GridCollection::THErefinementLevel); // build the refinement level collections
	    assert(gc.numberOfGrids()==1);
	    assert(gc.numberOfRefinementLevels()==1);	    
	  }
      }

  if(retVal)
    cout << "TEST 6: passed all cases successfully!" << endl;
  else
    cout << "TEST 6: DIDN'T PASS TESTS SUCCESSFULLY!!" << endl;
  Overture::finish();
}
