// created by Bobby Philip 09052001
#include "Overture.h"
#include "Square.h"
#include "ParentChildSiblingInfo.h"

// TEST 4: tests gridsOverlap() function
// vary refinement factor between 2..4
// create a base grid and two siblings that touch
// create a refinement patch that is a child
// of both sibling grids at level 1 ( try varying refinement factor again! )
// vary overlap on each grid
// check that correct ParentChildSiblingInfo relations
// are detected


// add comment to explain what this function does
bool passedTest(GridCollection &gc, int i, int j, int k, int l, int caseNo)
{
  bool retVal = FALSE;
  // Add a refinement, specify position in the coarse grid index space
  IntegerArray rangeA(2,3), factorA(3), rangeB(2,3), factorB(3);
  IntegerArray rangeC(2,3), factorC(3);
  // CASE 1: seperation between child grids is greater than gbw
  Integer level = 1;
  int grid = 0;                              // refine this base grid

  factorA(0) = i;                            // refinement factor along axis1 for grid A (level 1)
  factorA(1) = j;                            // refinement factor along axis2 for grid A (level 1)
  factorA(2) = 1;                            // refinement factor along axis3 for grid A (level 1)
  factorB(0) = i;                            // refinement factor along axis1 for grid B (level 1)
  factorB(1) = j;                            // refinement factor along axis2 for grid B (level 1)
  factorB(2) = 1;                            // refinement factor along axis3 for grid B (level 1)
  factorC(0) = k;                            // refinement factor along axis1 for grid C (level 2)
  factorC(1) = l;                            // refinement factor along axis2 for grid C (level 2)
  factorC(2) = 1;                            // refinement factor along axis3 for grid C (level 2)

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

  level = 2; grid = 1;
  switch(caseNo)
    {
    case 1:   // grid C lies entirely within A, with distance to the boundary of A being positive
      // ranges for C
      rangeC(0,0) = 3*factorA(0); rangeC(1,0) = 5*factorA(0);
      rangeC(0,1) = 3*factorA(1); rangeC(1,1) = 5*factorA(1);
      rangeC(0,2) = 0; rangeC(1,2) =  0;
      break;
    case 2:   // grid C lies entirely within A, aligned along boundary with B
      // ranges for C
      rangeC(0,0) = 3*factorA(0); rangeC(1,0) = 6*factorA(0);
      rangeC(0,1) = 3*factorA(1); rangeC(1,1) = 5*factorA(1);
      rangeC(0,2) = 0; rangeC(1,2) =  0;
      break;
    case 3:   // grid C lies over A and B
      // ranges for C
      rangeC(0,0) = 4*factorA(0); rangeC(1,0) = 8*factorA(0);
      rangeC(0,1) = 3*factorA(1); rangeC(1,1) = 5*factorA(1);
      rangeC(0,2) = 0; rangeC(1,2) =  0;
      break;
    }

  // add second refinement patch
  gc.addRefinement(rangeC, factorC, level, grid);    // add a refinement grid to level 1	    
  gc.update(GridCollection::THErefinementLevel); // build the refinement level collections
  assert(gc.numberOfGrids()==4);
  assert(gc.numberOfRefinementLevels()==3);

  int side, axis;  // set the number of ghost points equal to the refinement factor
  for( axis=0; axis<=2; axis++)
    for( side=0; side<=1; side++)
      {
	gc[1].setNumberOfGhostPoints(side, axis, factorA(axis));
	gc[2].setNumberOfGhostPoints(side, axis, factorB(axis));
	gc[3].setNumberOfGhostPoints(side, axis, factorC(axis));
      }

  ListOfParentChildSiblingInfo listOfPCSInfo;
  assert( listOfPCSInfo.listLength() == 0 );	    
  ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, listOfPCSInfo );
  assert( listOfPCSInfo.listLength() == 4 ); // one for parent, two for children

  switch(caseNo)
    {
    case 1:
    case 2:
      if( ( listOfPCSInfo[0].getParents().size() ==0) &&
	  ( listOfPCSInfo[0].getChildren().size()==2) &&
	  ( listOfPCSInfo[0].getSiblings().size()==0) &&
	  ( listOfPCSInfo[1].getParents().size() ==1) &&
	  ( listOfPCSInfo[1].getChildren().size()==1) &&
	  ( listOfPCSInfo[1].getSiblings().size()==1) &&
	  ( listOfPCSInfo[2].getParents().size() ==1) &&
	  ( listOfPCSInfo[2].getChildren().size()==0) &&
	  ( listOfPCSInfo[2].getSiblings().size()==1) &&
	  ( listOfPCSInfo[3].getParents().size() ==1) &&
	  ( listOfPCSInfo[3].getChildren().size()==0) &&
	  ( listOfPCSInfo[3].getSiblings().size()==0) )     
	retVal = TRUE;
      break;
    case 3:
      if( ( listOfPCSInfo[0].getParents().size() ==0) &&
	  ( listOfPCSInfo[0].getChildren().size()==2) &&
	  ( listOfPCSInfo[0].getSiblings().size()==0) &&
	  ( listOfPCSInfo[1].getParents().size() ==1) &&
	  ( listOfPCSInfo[1].getChildren().size()==1) &&
	  ( listOfPCSInfo[1].getSiblings().size()==1) &&
	  ( listOfPCSInfo[2].getParents().size() ==1) &&
	  ( listOfPCSInfo[2].getChildren().size()==1) &&
	  ( listOfPCSInfo[2].getSiblings().size()==1) &&
	  ( listOfPCSInfo[3].getParents().size() ==2) &&
	  ( listOfPCSInfo[3].getChildren().size()==0) &&
	  ( listOfPCSInfo[3].getSiblings().size()==0) )     
	retVal = TRUE;
      break;      
    }

  if( !retVal )
    cout << "TEST 3: ERROR:: did not pass case " << caseNo  << " with i= " << i << ", j = " << j 
	 << ", k = " << k << ", l = " << l << endl; 
#if 0
  // thought this is not really an indication that the test has been passed
  // we say it has been passed. We rely on the user to inspect the debug info
  // output to verify validity of the claim for now.
  cout << "__________________________________________________________" << endl;
  cout << "Debug information for PCSInfo at level 0:" << endl;
  cout << listOfPCSInfo[0] << endl;
  cout << "__________________________________________________________" << endl;
  cout << "Debug information for first child at level 1:" << endl;
  cout << "Refinement factor along x-axis = " << factorA(0) << endl;
  cout << "Refinement factor along y-axis = " << factorA(1) << endl;
  cout << "Refinement factor along z-axis = " << factorA(2) << endl;
  cout << listOfPCSInfo[1] << endl;
  cout << "__________________________________________________________" << endl;
  cout << "Debug information for second child:at level 1" << endl;
  cout << "Refinement factor along x-axis = " << factorB(0) << endl;
  cout << "Refinement factor along y-axis = " << factorB(1) << endl;
  cout << "Refinement factor along z-axis = " << factorB(2) << endl;
  cout << listOfPCSInfo[2] << endl;
  cout << "__________________________________________________________" << endl;
  cout << "Debug information for child:at level 2" << endl;
  cout << "Refinement factor along x-axis = " << factorC(0) << endl;
  cout << "Refinement factor along y-axis = " << factorC(1) << endl;
  cout << "Refinement factor along z-axis = " << factorC(2) << endl;
  cout << listOfPCSInfo[3] << endl;
  cout << "__________________________________________________________" << endl;
#endif

  gc.deleteRefinementLevels(0); // delete the new refinement level to reinitialize
  listOfPCSInfo.destroy();
  gc.update(GridCollection::THErefinementLevel); // build the refinement level collections
  //  cout << "gc.numberOfGrids() = " << gc.numberOfGrids() << endl;
  assert(gc.numberOfGrids()==1);
  assert(gc.numberOfRefinementLevels()==1);
  return retVal;
}

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
  
  bool retVal = TRUE;
  for( int i = 1; i<=4; i++)     // i specifies refinement factor in x direction
    for( int j = 1; j<=4; j++)   // j specifies refinement factor in y direction
      for( int k = 1; k<=4; k++)     // k specifies refinement factor in x direction for finest grid
	for( int l = 1; l<=4; l++)   // l specifies refinement factor in y direction for finest grid
	  {
	    if( ((i>1)||(j>1))&&((k>1)||(l>1)))
	      {
		for( int caseNo = 1; caseNo<=3; caseNo++)
		  {
		    cout << "*****************************************************************" << endl;
		    cout << "i = " << i << ", j = " << j << endl;
		    cout << "k = " << k << ", l = " << l << endl;
		    cout << " caseNo = " << caseNo << endl;  
		    bool bTest = passedTest (gc, i, j, k, l, caseNo);
		    retVal = retVal && bTest;
		    cout << "*****************************************************************" << endl;
		  }
	      }
	  }

  if(retVal)
    cout << "TEST 4: successfully passed all cases" << endl;

  Overture::finish();
  return 0;
}
