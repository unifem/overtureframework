// created by Bobby Philip 09052001
#include "Overture.h"
#include "Square.h"
#include "ParentChildSiblingInfo.h"

// TEST 3: tests children on non-periodic MappedGrid
// create a base grid 

// vary refinement factor between 2..4 and test for siblings when:
// a) seperation is greater than ghost boundary width (gbw)
// b) both grids have same gbw and are seperated by gbw
// c) grids have different gbw and are seperated by lesser gbw
// d) grids have different gbw and are seperated by greater gbw
// e) grids touch at point
// f) grids are close at corner and seperated by gbw

// check that correct ParentChildSiblingInfo
// relations are detected.

bool passedCase( GridCollection &gc, int i, int j, int caseNo)
{
  bool retVal = FALSE;
  // Add a refinement, specify position in the coarse grid index space
  IntegerArray rangeA(2,3), factorA(3), rangeB(2,3), factorB(3);
  // CASE 1: seperation between child grids is greater than gbw
  Integer level = 1;
  int grid = 0;                              // refine this base grid

  factorA(0) = i;                            // refinement factor along axis1
  factorA(1) = j;                            // refinement factor along axis2
  factorA(2) = 1;                            // refinement factor along axis3
  factorB(0) = i;                            // refinement factor along axis1
  factorB(1) = j;                            // refinement factor along axis2
  factorB(2) = 1;                            // refinement factor along axis3

  switch( caseNo ){
  case 1:  // grids align along an edge
    // ranges for first patch
    rangeA(0,0) = 2; rangeA(1,0) = 5;
    rangeA(0,1) = 2; rangeA(1,1) = 6;
    rangeA(0,2) = 0; rangeA(1,2) =  0;
    // ranges for second patch
    rangeB(0,0) = 5; rangeB(1,0) = 10;
    rangeB(0,1) = 2; rangeB(1,1) = 6;
    rangeB(0,2) = 0; rangeB(1,2) =  0;
    break;
  case 2:  // grids touch along an edge but are not aligned
    // ranges for first patch
    rangeA(0,0) = 2; rangeA(1,0) = 5;
    rangeA(0,1) = 2; rangeA(1,1) = 6;
    rangeA(0,2) = 0; rangeA(1,2) =  0;
    // ranges for second patch
    rangeB(0,0) = 5; rangeB(1,0) = 10;
    rangeB(0,1) = 4; rangeB(1,1) = 7;
    rangeB(0,2) = 0; rangeB(1,2) =  0;
    break;
  case 3:     // seperation between grids is greater than GBW
    // ranges for first patch
    rangeA(0,0) = 2; rangeA(1,0) = 5;
    rangeA(0,1) = 2; rangeA(1,1) = 6;
    rangeA(0,2) = 0; rangeA(1,2) =  0;
    // ranges for second patch
    rangeB(0,0) = 7; rangeB(1,0) = 10;
    rangeB(0,1) = 2; rangeB(1,1) = 6;
    rangeB(0,2) = 0; rangeB(1,2) =  0;
    break;
  case 4:  // seperation between grids is exactly GBW
    // ranges for first patch
    rangeA(0,0) = 2; rangeA(1,0) = 5;
    rangeA(0,1) = 2; rangeA(1,1) = 6;
    rangeA(0,2) = 0; rangeA(1,2) =  0;
    // ranges for second patch
    rangeB(0,0) = 6; rangeB(1,0) = 10;
    rangeB(0,1) = 2; rangeB(1,1) = 6;
    rangeB(0,2) = 0; rangeB(1,2) =  0;    
    break;
  case 5:    // grids touch at a point
    // ranges for first patch
    rangeA(0,0) = 2; rangeA(1,0) = 5;
    rangeA(0,1) = 2; rangeA(1,1) = 5;
    rangeA(0,2) = 0; rangeA(1,2) =  0;
    // ranges for second patch
    rangeB(0,0) = 5; rangeB(1,0) = 10;
    rangeB(0,1) = 5; rangeB(1,1) = 10;
    rangeB(0,2) = 0; rangeB(1,2) =  0;
    break;
  case 6:    // grids meet at a corner seperated by GBW
    // ranges for first patch
    rangeA(0,0) = 2; rangeA(1,0) = 5;
    rangeA(0,1) = 2; rangeA(1,1) = 5;
    rangeA(0,2) = 0; rangeA(1,2) =  0;
    // ranges for second patch
    rangeB(0,0) = 6; rangeB(1,0) = 10;
    rangeB(0,1) = 6; rangeB(1,1) = 10;
    rangeB(0,2) = 0; rangeB(1,2) =  0;
    break;
  case 7: // grids have different GBW and are seperated by lesser GBW
  case 8: // grids have different GBW and are seperated by larger GBW
    // ranges for first patch
    rangeA(0,0) = 2; rangeA(1,0) = 5;
    rangeA(0,1) = 2; rangeA(1,1) = 6;
    rangeA(0,2) = 0; rangeA(1,2) =  0;
    // ranges for second patch
    rangeB(0,0) = 6; rangeB(1,0) = 9;
    rangeB(0,1) = 2; rangeB(1,1) = 6;
    rangeB(0,2) = 0; rangeB(1,2) =  0;
    break;
  }
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
	if((caseNo==7)||(caseNo==8))
	  {
	    if(caseNo==7)  // grids are seperated by lesser of GBW's
	      {
		gc[1].setNumberOfGhostPoints(side, axis, 2*factorA(axis));
		gc[2].setNumberOfGhostPoints(side, axis, factorB(axis));
	      }
	    else  // grids are seperated by greater of GBW's
	      {
		gc[1].setNumberOfGhostPoints(side, axis, factorA(axis)/2);
		gc[2].setNumberOfGhostPoints(side, axis, factorB(axis));
	      }
	  }
	else
	  {
	    gc[1].setNumberOfGhostPoints(side, axis, factorA(axis));
	    gc[2].setNumberOfGhostPoints(side, axis, factorB(axis));
	  }
      }

  ListOfParentChildSiblingInfo listOfPCSInfo;
  assert( listOfPCSInfo.listLength() == 0 );	    
  ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, listOfPCSInfo );
  assert( listOfPCSInfo.listLength() == 3 ); // one for parent, two for children

  switch( caseNo )
    {
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
      if( ( listOfPCSInfo[0].getParents().size() ==0) &&
	  ( listOfPCSInfo[0].getChildren().size()==2) &&
	  ( listOfPCSInfo[0].getSiblings().size()==0) &&
	  ( listOfPCSInfo[1].getParents().size() ==1) &&
	  ( listOfPCSInfo[1].getChildren().size()==0) &&
	  ( listOfPCSInfo[1].getSiblings().size()==1) &&
	  ( listOfPCSInfo[2].getParents().size() ==1) &&
	  ( listOfPCSInfo[2].getChildren().size()==0) &&
	  ( listOfPCSInfo[2].getSiblings().size()==1) )     
	retVal = TRUE;
	break;
    case 3:
      if( ( listOfPCSInfo[0].getParents().size() ==0) &&
	  ( listOfPCSInfo[0].getChildren().size()==2) &&
	  ( listOfPCSInfo[0].getSiblings().size()==0) &&
	  ( listOfPCSInfo[1].getParents().size() ==1) &&
	  ( listOfPCSInfo[1].getChildren().size()==0) &&
	  ( listOfPCSInfo[1].getSiblings().size()==0) &&
	  ( listOfPCSInfo[2].getParents().size() ==1) &&
	  ( listOfPCSInfo[2].getChildren().size()==0) &&
	  ( listOfPCSInfo[2].getSiblings().size()==0) )     
	retVal = TRUE;
      break;
    case 8:
      if( ( listOfPCSInfo[0].getParents().size() ==0) &&
	  ( listOfPCSInfo[0].getChildren().size()==2) &&
	  ( listOfPCSInfo[0].getSiblings().size()==0) &&
	  ( listOfPCSInfo[1].getParents().size() ==1) &&
	  ( listOfPCSInfo[1].getChildren().size()==0) &&
	  ( listOfPCSInfo[1].getSiblings().size()==0) &&
	  ( listOfPCSInfo[2].getParents().size() ==1) &&
	  ( listOfPCSInfo[2].getChildren().size()==0) &&
	  ( listOfPCSInfo[2].getSiblings().size()==1) )     
	retVal = TRUE;
      break;
    }

  if( !retVal )
    {
      cout << "TEST 3: ERROR:: did not pass case " << caseNo  << " with i= " << i << ", j = " << j << endl; 
    }

#if 1
  // thought this is not really an indication that the test has been passed
  // we say it has been passed. We rely on the user to inspect the debug info
  // output to verify validity of the claim for now.
  cout << "__________________________________________________________" << endl;
  cout << "Debug information for parent:" << endl;
  cout << listOfPCSInfo[0] << endl;
  cout << "__________________________________________________________" << endl;
  cout << "Debug information for first child:" << endl;
  cout << "Refinement factor along x-axis = " << factorA(0) << endl;
  cout << "Refinement factor along y-axis = " << factorA(1) << endl;
  cout << "Refinement factor along z-axis = " << factorA(2) << endl;
  cout << listOfPCSInfo[1] << endl;
  cout << "__________________________________________________________" << endl;
  cout << "Debug information for second child:" << endl;
  cout << "Refinement factor along x-axis = " << factorB(0) << endl;
  cout << "Refinement factor along y-axis = " << factorB(1) << endl;
  cout << "Refinement factor along z-axis = " << factorB(2) << endl;
  cout << listOfPCSInfo[2] << endl;
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
      for( int caseNo = 1; caseNo<=8; caseNo++)
	{
	  if( (i>1)||(j>1))  // we don't want to consider no refinement at all
	    {
	      cout << "*****************************************************************" << endl;
	      cout << "i = " << i << ", j = " << j << ", caseNo = " << caseNo << endl;  
	      bool bTest = passedCase (gc, i, j, caseNo);
	      retVal = retVal && bTest;
	      cout << "*****************************************************************" << endl;
	    }
	}

  if(retVal)
    cout << "TEST 3: successfully passed all cases" << endl;

  Overture::finish();
  return 0;
}
