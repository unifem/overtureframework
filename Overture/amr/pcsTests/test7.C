// created by Bobby Philip 09212001
#include "Overture.h"
#include "Square.h"
#include "ParentChildSiblingInfo.h"

// TEST 6: tests getSiblingGhostBoxes() function

// can just test some very specific examples


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

  // Add a refinement, specify position in the coarse grid index space
  IntegerArray rangeA(2,3), factorA(3), rangeB(2,3), factorB(3);
  IntegerArray rangeC(2,3), factorC(3);
  // CASE 1: seperation between child grids is greater than gbw
  Integer level = 1;
  int grid = 0;                              // refine this base grid

  factorA(0) = 3;                            // refinement factor along axis1 for grid A (level 1)
  factorA(1) = 3;                            // refinement factor along axis2 for grid A (level 1)
  factorA(2) = 1;                            // refinement factor along axis3 for grid A (level 1)
  factorB(0) = 3;                            // refinement factor along axis1 for grid B (level 1)
  factorB(1) = 3;                            // refinement factor along axis2 for grid B (level 1)
  factorB(2) = 1;                            // refinement factor along axis3 for grid B (level 1)

  // ranges for first patch, A
  rangeA(0,0) = 2; rangeA(1,0) = 6;
  rangeA(0,1) = 2; rangeA(1,1) = 6;
  rangeA(0,2) = 0; rangeA(1,2) =  0;
  // ranges for second patch, B
  rangeB(0,0) = 6; rangeB(1,0) = 10;
  rangeB(0,1) = 2; rangeB(1,1) = 6;
  rangeB(0,2) = 0; rangeB(1,2) =  0;

  // add first refinement patch
  gc.addRefinement(rangeA, factorA, level, grid);    // add a refinement grid to level 1
  // add second refinement patch
  gc.addRefinement(rangeB, factorB, level, grid);    // add a refinement grid to level 1	    

  gc.update(GridCollection::THErefinementLevel); // build the refinement level collections
  assert(gc.numberOfGrids()==3);
  assert(gc.numberOfRefinementLevels()==2);

  int side, axis;  // set the number of ghost points equal to the refinement factor
  for( axis=0; axis<=2; axis++)
    for( side=0; side<=1; side++)
      {
	gc[1].setNumberOfGhostPoints(side, axis, factorA(axis));
	gc[2].setNumberOfGhostPoints(side, axis, factorB(axis));
      }

  ListOfParentChildSiblingInfo listOfPCSInfo;
  assert( listOfPCSInfo.listLength() == 0 );	    
  ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, listOfPCSInfo );
  assert( listOfPCSInfo.listLength() == 3 ); // one for parent, two for children

  intSerialArray gridIndices;
  BoxList parentGhostBoxes, siblingBoxes, ghostBoxesOnCurrentGrid;
  Range ghostLines;
  bool excludeSiblingPoints = TRUE;


  // ****************************************************************************************** //
  //                      CASE I : boundary and first two ghost lines
  ghostLines = Range(0,2);  // all ghost lines
  int gridIndex = 0;

#if 0
  IndexType iType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
#else
  IndexType iType(D_DECL(IndexType::CELL, IndexType::CELL, IndexType::CELL));
#endif
  // first check that the base grid returns empty lists
  listOfPCSInfo[0].getSiblingGhostBoxes( gridIndices, siblingBoxes,
					 ghostBoxesOnCurrentGrid, 
					 ghostLines, gc, 0, iType);

  assert( siblingBoxes.isEmpty() && ghostBoxesOnCurrentGrid.isEmpty() );
  gridIndices.redim(0);

  // don't exclude sibling points
  listOfPCSInfo[0].getParentGhostBoxes( gridIndices, parentGhostBoxes, ghostLines, gc, 0, iType );
  assert( parentGhostBoxes.isEmpty() );
  gridIndices.redim(0);

  listOfPCSInfo[0].getParentGhostBoxes( gridIndices, parentGhostBoxes, ghostLines, gc, 0, iType, excludeSiblingPoints );
  assert( parentGhostBoxes.isEmpty() );
  gridIndices.redim(0);

  for( gridIndex = 1; gridIndex <=2; gridIndex++)
    {
      // check 1st refinement grid patch
      listOfPCSInfo[gridIndex].getSiblingGhostBoxes( gridIndices, siblingBoxes,
						     ghostBoxesOnCurrentGrid, 
						     ghostLines, gc, gridIndex, iType );

      assert( siblingBoxes.isNotEmpty() && ghostBoxesOnCurrentGrid.isNotEmpty() );
      assert( siblingBoxes.length() == ghostBoxesOnCurrentGrid.length() );
      assert( siblingBoxes == ghostBoxesOnCurrentGrid );  // the lists should contain the same elements
      gridIndices.display("the grid indices for the sibling boxes");
      cout << "ghostBoxesOnCurrentGrid = " << endl;
      cout << ghostBoxesOnCurrentGrid << endl;
      gridIndices.redim(0);
      siblingBoxes.clear();
      ghostBoxesOnCurrentGrid.clear();
	
      // don't exclude sibling points
      listOfPCSInfo[gridIndex].getParentGhostBoxes( gridIndices, parentGhostBoxes, ghostLines, gc, gridIndex, iType );
      assert( parentGhostBoxes.isNotEmpty() );
      gridIndices.display("the grid indices for the parent boxes");
      cout << "Parent Ghost Boxes not excluding Siblings = " << endl;
      cout << parentGhostBoxes << endl;
      parentGhostBoxes.clear();
      gridIndices.redim(0);

      listOfPCSInfo[gridIndex].getParentGhostBoxes( gridIndices, 
						    parentGhostBoxes, 
						    ghostLines, 
						    gc, gridIndex, 
						    iType, excludeSiblingPoints );
      assert( parentGhostBoxes.isNotEmpty() );
      gridIndices.display("the grid indices for the parent boxes");
      cout << "Parent Ghost Boxes excluding Siblings = " << endl;
      cout << parentGhostBoxes << endl;
      parentGhostBoxes.clear();
      gridIndices.redim(0);
    }

  // just a temporary thing, there's nothing wrong with the code
#if 1
  //                                    END CASE I
  // ****************************************************************************************** //
  //                                    CASE 2: consider boundary and first ghost line only
  ghostLines = Range(0, 1);
  for( gridIndex = 1; gridIndex <=2; gridIndex++)
    {
      // check 1st refinement grid patch
      listOfPCSInfo[gridIndex].getSiblingGhostBoxes( gridIndices, siblingBoxes,
						     ghostBoxesOnCurrentGrid, 
						     ghostLines, gc, gridIndex, iType );

      assert( siblingBoxes.isNotEmpty() && ghostBoxesOnCurrentGrid.isNotEmpty() );
      assert( siblingBoxes.length() == ghostBoxesOnCurrentGrid.length() );
      assert( siblingBoxes == ghostBoxesOnCurrentGrid );  // the lists should contain the same elements
      gridIndices.display("the grid indices for the sibling boxes");
      cout << "ghostBoxesOnCurrentGrid = " << endl;
      cout << ghostBoxesOnCurrentGrid << endl;
      gridIndices.redim(0);
      siblingBoxes.clear();
      ghostBoxesOnCurrentGrid.clear();
	
      // don't exclude sibling points
      listOfPCSInfo[gridIndex].getParentGhostBoxes( gridIndices, parentGhostBoxes, ghostLines, gc, gridIndex, iType );
      assert( parentGhostBoxes.isNotEmpty() );
      gridIndices.display("the grid indices for the parent boxes");
      cout << "Parent Ghost Boxes not excluding Siblings = " << endl;
      cout << parentGhostBoxes << endl;
      parentGhostBoxes.clear();
      gridIndices.redim(0);

      listOfPCSInfo[gridIndex].getParentGhostBoxes( gridIndices, 
						    parentGhostBoxes, 
						    ghostLines, 
						    gc, gridIndex, 
						    iType, excludeSiblingPoints );
      assert( parentGhostBoxes.isNotEmpty() );
      gridIndices.display("the grid indices for the parent boxes");
      cout << "Parent Ghost Boxes excluding Siblings = " << endl;
      cout << parentGhostBoxes << endl;
      parentGhostBoxes.clear();
      gridIndices.redim(0);
    }
#endif
  Overture::finish();
  return 0;
}
