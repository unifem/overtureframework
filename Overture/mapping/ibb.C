#include "Inverse.h"
#include "BoundingBox.h"

//==========================================================================
//   Assign the binary tree's of Bounding Boxes 
//
// No boxes are created for domainDimension==1
//
// A binary tree is created for each side 
// Any box that has too many grid points in it is subdivided into two
//
//           boundingBoxTree[side][axis]
//                     |
//              +------+-------+
//           child1          child2
//             |               |
//         +---+----+     +----+------+
//      child1   child2 child1     child2
//         |        |     |           |
//
//==========================================================================
void ApproximateGlobalInverse::initializeBoundingBoxTrees()
{
  if( domainDimension==1 ) return;

  const int maximumNumberOfPointsInSmallestBox=16;

  int side,axis,dir,dir1,dir2,width,boxSize;
  Index dAxes = Range(0,domainDimension-1);
  Index rAxes = Range(0,rangeDimension-1);
  Index I[3];  
  IntegerArray dimension(2,3);
  for( axis=axis1; axis<3; axis++ )
  {
    dimension(Start,axis)=grid.getBase(axis);
    dimension(End  ,axis)=grid.getBound(axis);
  }
  
  
  for( side=Start; side<=End; side++ )
  for( axis=axis1; axis<domainDimension; axis++ )  
  {
    // set domain dimensions of the root box:
    boundingBoxTree[side][axis].domainBound=dimension(Range(0,1),dAxes);                 
    boundingBoxTree[side][axis].domainBound(Start,axis)=dimension(side,axis);
    boundingBoxTree[side][axis].domainBound(End  ,axis)=dimension(side,axis);
    dir1 = (axis+1) % domainDimension;
    dir2 = (axis+2) % domainDimension;

    boxStack.push( boundingBoxTree[side][axis] );
    while( !boxStack.isEmpty() )
    {
      BoundingBox & box=boxStack.pop();       // get a box off the stack
      boxSize=(box.domainBound(End,dir1)-box.domainBound(Start,dir1)+1)
	     *(box.domainBound(End,dir2)-box.domainBound(Start,dir2)+1);
      
      if( boxSize > maximumNumberOfPointsInSmallestBox )
      {  // bisect the box (along longest axis in 3D)
	box.addChildren();  
        if( domainDimension==2 )
          dir = axis +1 % domainDimension;
	else 
          dir = (box.domainBound(End,dir1)-box.domainBound(Start,dir1) >=
                 box.domainBound(End,dir2)-box.domainBound(Start,dir2) ) ? dir1 : dir2;
        width = box.domainBound(End,dir)-box.domainBound(Start,dir);
        box.child1->domainBound=box.domainBound;
        box.child1->domainBound(Start,dir)=box.domainBound(Start,dir);
        box.child1->domainBound(End  ,dir)=box.domainBound(Start,dir)+width/2;
        box.child2->domainBound=box.domainBound;
        box.child2->domainBound(Start,dir)=box.child1->domainBound(End,dir);
        box.child2->domainBound(End  ,dir)=box.domainBound(End  ,dir);
        boxStack.push( *box.child1 );	
        boxStack.push( *box.child2 );	
      }
      else
      { // the box is small enough, find range bounds for this box
        for( dir=axis1; dir<domainDimension; dir++ )
          I[dir]=Range(box.domainBound(Start,dir),box.domainBound(End,dir));
        if( domainDimension==2 )
          I[2]=Range(dimension(side,axis3),dimension(side,axis3));
        // evaluate vertex...
        for( dir=axis1; dir<rangeDimension; dir++ )
	{
  	  box.rangeBound(Start,dir)=min(grid(I[0],I[1],I[2],dir));
	  box.rangeBound(End  ,dir)=max(grid(I[0],I[1],I[2],dir));
	}
        box.defined=TRUE; // the box is now defined
      }
    }
    // --- Now assign the rangeBound for all boxes.
    //   up till this point only the outer-most leaves in the tree have been assigned.
    if( boundingBoxTree[side][axis].child1!=NULL )
      boxStack.push( boundingBoxTree[side][axis] );
    while( !boxStack.isEmpty() )
    {
      BoundingBox & box=boxStack.pop();                // get a box off the stack
      if( box.child1->defined && box.child2->defined ) // if both children are defined, set this box
      {
	box.rangeBound(Start,rAxes)=min(box.child1->rangeBound(Start,rAxes),
                                        box.child2->rangeBound(Start,rAxes));
	box.rangeBound(End  ,rAxes)=max(box.child1->rangeBound(End  ,rAxes),
                                        box.child2->rangeBound(End  ,rAxes));
        box.defined=TRUE;
      }
      else
      {
	boxStack.push( box );           // push this box back on, plus any undefined children
        if( !(box.child1->defined) )
	  boxStack.push( *box.child1 );
        if( !(box.child2->defined) )
	  boxStack.push( *box.child2 );
      }
    }
    if( Mapping::debug & 16 )
    {
      cout << "intializeBoundingBoxTrees side,axis = " << side << "," << axis << endl;
      cout << "Enter 1 display " << endl;
      cin >> dir1;
      if( dir1==1 ) boundingBoxTree[side][axis].display( " " );
    }
  }
}






