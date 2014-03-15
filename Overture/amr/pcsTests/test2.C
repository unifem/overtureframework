// created by Bobby Philip 09052001
#include "Overture.h"
#include "Square.h"
#include "ParentChildSiblingInfo.h"

// create a MappedGrid with one refinement patch
// vary the refinement factor and ensure that the
// Parent-Child relations are detected

int main( int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

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
  for( int i = 1; i<=4; i++)
    for( int j = 1; j<=4; j++)
      {
	if( !((i==1)&&(j==1)))  // we don't want to consider no refinement at all
	  {
	    // Add a refinement, specify position in the coarse grid index space
	    IntegerArray range(2,3), factor(3);
	    range(0,0) = 2; range(1,0) = 6;
	    range(0,1) = 2; range(1,1) = 6;
	    range(0,2) = 0; range(1,2) =  0;
	    factor(0) = i;                            // refinement factor along axis1
	    factor(1) = j;                            // refinement factor along axis2
	    factor(2) = 1;                            // refinement factor along axis3
	    Integer level = 1;
	    int grid = 0;                              // refine this base grid
	    gc.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1
	    gc.update(GridCollection::THErefinementLevel); // build the refinement level collections
	    assert(gc.numberOfGrids()==2);
	    assert(gc.numberOfRefinementLevels()==2);

	    assert( listOfPCSInfo.listLength() == 0 );	    
	    ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, listOfPCSInfo );

	    assert( listOfPCSInfo.listLength() == 2 ); // one for parent, one for child
	    if( ( listOfPCSInfo[0].getParents().size() ==0) &&
		( listOfPCSInfo[0].getChildren().size()==1) &&
		( listOfPCSInfo[0].getSiblings().size()==0) &&
	        ( listOfPCSInfo[1].getParents().size() ==1) &&
		( listOfPCSInfo[1].getChildren().size()==0) &&
		( listOfPCSInfo[1].getSiblings().size()==0) )
	      {
		// thought this is not really an indication that the test has been passed
		// we say it has been passed. We rely on the user to inspect the debug info
		// output to verify validaity of the claim for now.
		cout << "__________________________________________________________" << endl;
		cout << "Refinement factor along x-axis = " << factor(0) << endl;
		cout << "Refinement factor along y-axis = " << factor(1) << endl;
		cout << "Refinement factor along z-axis = " << factor(2) << endl;
		cout << "__________________________________________________________" << endl;
		cout << "Debug information for parent:" << endl;
		cout << listOfPCSInfo[0] << endl;
		cout << "__________________________________________________________" << endl;
		cout << "Debug information for child:" << endl;
		cout << listOfPCSInfo[1] << endl;
		cout << "__________________________________________________________" << endl;
	      }

	    gc.deleteRefinementLevels(0); // delete the new refinement level to reinitialize
	    listOfPCSInfo.destroy();
	    gc.update(GridCollection::THErefinementLevel); // build the refinement level collections
	  }
      }
  Overture::finish();
  return 0;
}
