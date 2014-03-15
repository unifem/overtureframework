// define the next variable to get timings
#define STENCIL_DEBUG(x) 
#define STENCIL_DEBUG2(x) 
// #define STENCIL_DEBUG(x) x
// #define STENCIL_DEBUG2(x) x


#include "Inverse.h"
#include "BoundingBox.h"
#include "TriangleClass.h"
#include "display.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
OV_USINGNAMESPACE(std);
#else
#include <list.h>
#endif

static int initializeStencilWalk2D=TRUE;
static int initializeStencilWalk3D=TRUE;
// extern IntegerArray numberOfStencilDirections2D,
//                 stencilDirection2D1,
//                 stencilDirection2D2,
//                 numberOfStencilDirections3D,
//                 stencilDirection3D1,
//                 stencilDirection3D2,
//                 stencilDirection3D3;

int ApproximateGlobalInverse::numberOfStencilDir2D[9];  // (3,3);
int ApproximateGlobalInverse::stencilDir2D1[8*3*3];    // (8,3,3)
int ApproximateGlobalInverse::stencilDir2D2[8*3*3];
int ApproximateGlobalInverse::stencilDir2D3[8*3*3];
int ApproximateGlobalInverse::numberOfStencilDir3D[27];  // (3,3,3);
int ApproximateGlobalInverse::stencilDir3D1[27*3*3*3];    // (27,3,3,3)
int ApproximateGlobalInverse::stencilDir3D2[27*3*3*3];
int ApproximateGlobalInverse::stencilDir3D3[27*3*3*3];

#define numberOfStencilDirections2D(dir1,dir2) numberOfStencilDir2D[(dir1)+1+3*((dir2)+1)]
#define stencilDirection2D1(dir,dir1,dir2) stencilDir2D1[(dir)+8*((dir1)+1+3*((dir2)+1))]
#define stencilDirection2D2(dir,dir1,dir2) stencilDir2D2[(dir)+8*((dir1)+1+3*((dir2)+1))]
#define numberOfStencilDirections3D(dir1,dir2,dir3) numberOfStencilDir3D[(dir1)+1+3*((dir2)+1+3*((dir3)+1))]
#define stencilDirection3D1(dir,dir1,dir2,dir3) stencilDir3D1[(dir)+27*((dir1)+1+3*((dir2)+1+3*((dir3)+1)))]
#define stencilDirection3D2(dir,dir1,dir2,dir3) stencilDir3D2[(dir)+27*((dir1)+1+3*((dir2)+1+3*((dir3)+1)))]
#define stencilDirection3D3(dir,dir1,dir2,dir3) stencilDir3D3[(dir)+27*((dir1)+1+3*((dir2)+1+3*((dir3)+1)))]


real ApproximateGlobalInverse::timeForFindNearestGridPoint=0.;
real ApproximateGlobalInverse::timeForBinarySearchOverBoundary=0.;
real ApproximateGlobalInverse::timeForBinarySearchOnLeaves=0.;

int ApproximateGlobalInverse::numberOfStencilWalks=0;
int ApproximateGlobalInverse::numberOfStencilSearches=0;
int ApproximateGlobalInverse::numberOfBinarySearches=0;
int ApproximateGlobalInverse::numberOfBoxesChecked=0;
int ApproximateGlobalInverse::numberOfBoundingBoxes=0;

void ApproximateGlobalInverse::
initializeBoundingBoxTrees()
//==========================================================================
/// \details 
///    Assign the binary tree's of Bounding Boxes 
/// 
///  For domainDimension==1 bounding boxes are made for the whole mapping (curve in 1,2 or 3d)
///  For domainDimension<rangeDimension boxes are made for the whole mapping (3D surface) 
/// 
///  For domainDimension>1 a binary tree is created for each side 
///  Any box that has too many grid points in it is subdivided into two
/// 
///  \begin{verbatim}
///            boundingBoxTree[side][axis]
///                      |
///               +------+-------+
///            child1          child2
///              |               |
///          +---+----+     +----+------+
///       child1   child2 child1     child2
///          |        |     |           |
/// 
///  Each box contains:
///     domainBound(2,domainDimension) : index bounds for box
///     rangeBound(2,rangeDimension)   : bounds of box in physical space
/// 
///  \end{verbatim}
/// 
///  Note that these Bounding Box trees will be automatically (recursively) deleted when
///  the  destructor is called for boundingBoxTree[2][3]
//==========================================================================
{
  // Mapping::debug=7; // ********* TEMP
  

  int maximumNumberOfPointsInSmallestBox=domainDimension==1 ? 4 : (int)pow(4,domainDimension);
  int side,axis,dir,dir1,dir2,width,boxSize;
  Index I[3];  

  boundingBox.redim(2,3);    // bounding Box for entire grid
  boundingBox=0.;

  for( axis=0; axis<3; axis++ )
    for( side=0; side<=1; side++ )
      boundingBoxTree[side][axis].deleteChildren();

  // ** in parallel the local array may be empty. In this case we should set 
  //    boundingBox(Start,axis) = REAL_MAX/10. 
  //    boundingBox(End  ,axis) = - boundingBox(Start,axis)
  //  We then need to set 
  //             boundingBoxTree[side][axis]
  if( indexRange(0,0)>indexRange(1,0) )
  {
    // there are no points on the grid -- this must be a parallel computation and there
    // are no grid points on the local grid to this proc.
    // -> set the boundaing boxes so the lower bound is greater then the upper

    for( axis=0; axis<3; axis++ )
    {
      boundingBox(Start,axis) = REAL_MAX/10.;
      boundingBox(End  ,axis) = - boundingBox(Start,axis);
    }
    for( side=Start; side<=End; side++ )
    {
      for( axis=axis1; axis<domainDimension; axis++ )  
      {
        // we only set (side,axis)=(0,0) for domainDimension<rangeDimension
	if( domainDimension<rangeDimension && (side>Start || axis>axis1) ) 
	  break;

	numberOfBoundingBoxes++;
        
	BoundingBox & box0= boundingBoxTree[side][axis];
	
	box0.setDimensions( domainDimension,rangeDimension );
	box0.setDomainBound(indexRange(Range(0,1),Axes));      
	for( dir=axis1; dir<rangeDimension; dir++ )
	{
	  box0.rangeBound(Start,dir)=boundingBox(Start,axis);
	  box0.rangeBound(End  ,dir)=boundingBox(End  ,axis);
	}
        box0.setIsDefined(); // the box is now defined
      }
    
    }
    return;
  }

  assert( indexRange(0,0)<=indexRange(1,0) && indexRange(0,1)<=indexRange(1,1) && indexRange(0,2)<=indexRange(1,2) );


  for( axis=axis1; axis<rangeDimension; axis++ )
    boundingBox(Start,axis)=grid(indexRange(Start,axis1),    // initial values
                                 indexRange(Start,axis2),
                                 indexRange(Start,axis3),axis);
  boundingBox(End,xAxes)=boundingBox(Start,xAxes);

  if( Mapping::debug & 64 ) // & 64
  {
    Mapping::openDebugFiles();
    const int myid=max(0,Communication_Manager::My_Process_Number);
    printf("myid=%i : intializeBoundingBoxTrees \n",myid);
    fprintf(Mapping::pDebugFile,"intializeBoundingBoxTrees \n");
    ::display(grid,"Here is the grid",Mapping::pDebugFile,"%5.2f ");
  }
  if( Mapping::debug & 4 )
  {
    Mapping::openDebugFiles();

    const int myid=max(0,Communication_Manager::My_Process_Number);
    fprintf(Mapping::pDebugFile,
           "initializeBoundingBoxTrees: name=%s\n"
           "  myid=%i grid=[%i,%i][%i,%i][%i,%i][%i,%i]\n"
           "    indexRange=[%i,%i][%i,%i][%i,%i]\n",
           (const char*)map->getName(Mapping::mappingName),
           myid,
	   grid.getBase(0),grid.getBound(0),   
	   grid.getBase(1),grid.getBound(1),   
	   grid.getBase(2),grid.getBound(2),   
	   grid.getBase(3),grid.getBound(3),   
           indexRange(0,0),indexRange(1,0),
           indexRange(0,1),indexRange(1,1),
           indexRange(0,2),indexRange(1,2));
  }
  
  BoundingBoxStack boxStack;

  if( domainDimension < rangeDimension )
  {
    // *********************************************************************
    //        domainDimension==1 : make bounding boxes for entire Mapping
    //   OR:  Surface in 3D : make bounding boxes for entire surface
    // *********************************************************************
    side=0;  // save the bounding boxes here
    axis=0;

    // set domain dimensions of the root box:
    boundingBoxTree[side][axis].setDimensions( domainDimension,rangeDimension );
    boundingBoxTree[side][axis].setDomainBound(indexRange(Range(0,1),Axes));                 

    boxStack.push( boundingBoxTree[side][axis] );
    while( !boxStack.isEmpty() )
    {
      numberOfBoundingBoxes++;
      BoundingBox & box0=boxStack.pop();       // get a box off the stack

      if( Mapping::debug & 64 )
        box0.display("intializeBoundingBoxTrees, box from stack..");

      boxSize= domainDimension==1 ? box0.domainBound(End,axis1)-box0.domainBound(Start,axis1)+1 :
              (box0.domainBound(End,axis1)-box0.domainBound(Start,axis1)+1) 
             *(box0.domainBound(End,axis2)-box0.domainBound(Start,axis2)+1);
      
      if( boxSize > maximumNumberOfPointsInSmallestBox )
      {  
	box0.addChildren();  
        // bisect the box along the longest edge
        if( domainDimension==1 )
          dir=axis1;
	else
          dir = (box0.domainBound(End,axis1)-box0.domainBound(Start,axis1) >= 
                 box0.domainBound(End,axis2)-box0.domainBound(Start,axis2) ) ? axis1 : axis2;
	
        width = box0.domainBound(End,dir)-box0.domainBound(Start,dir);
        // box0.child1->domainBound=box0.domainBound;
        box0.child1->domainBoundEquals(box0);
	
        box0.child1->domainBound(Start,dir)=box0.domainBound(Start,dir);
        box0.child1->domainBound(End  ,dir)=box0.domainBound(Start,dir)+width/2;

        // box0.child2->domainBound=box0.domainBound;
        box0.child2->domainBoundEquals(box0);

        box0.child2->domainBound(Start,dir)=box0.child1->domainBound(End,dir);
        box0.child2->domainBound(End  ,dir)=box0.domainBound(End  ,dir);
        boxStack.push( *box0.child1 );	
        boxStack.push( *box0.child2 );	
      }
      else
      { // the box is small enough, find range bounds for this box
        I[axis1]=Range(box0.domainBound(Start,axis1),box0.domainBound(End,axis1));
        if( domainDimension==2 )
          I[axis2]=Range(box0.domainBound(Start,axis2),box0.domainBound(End,axis2));
        else
          I[axis2]=Range(indexRange(side,axis2),indexRange(side,axis2));
        I[axis3]=Range(indexRange(side,axis3),indexRange(side,axis3));

        for( dir=axis1; dir<rangeDimension; dir++ )
	{
  	  box0.rangeBound(Start,dir)=min(grid(I[0],I[1],I[2],dir));
	  box0.rangeBound(End  ,dir)=max(grid(I[0],I[1],I[2],dir));
	}

        // adjust the bounding box to be slightly larger  
        // real delta = max(fabs(box0.rangeBound)) * .01;
        real delta = fabs(box0.rangeBound(End,axis1)-box0.rangeBound(Start,axis1));
        for( dir=1; dir<rangeDimension; dir++ )
          delta=max(delta,fabs(box0.rangeBound(End,dir)-box0.rangeBound(Start,dir)));

	delta*=boundingBoxExtensionFactor;
        for( dir=axis1; dir<rangeDimension; dir++ )
	{
  	  box0.rangeBound(Start,dir)-=delta;
	  box0.rangeBound(End  ,dir)+=delta;
	}


        
        box0.setIsDefined(); // the box is now defined
      }
    }
    // --- Now assign the rangeBound for all boxes.
    //   up till this point only the outer-most leaves in the tree have been assigned.
    if( boundingBoxTree[side][axis].child1!=NULL )
      boxStack.push( boundingBoxTree[side][axis] );
    while( !boxStack.isEmpty() )
    {
      BoundingBox & box0=boxStack.pop();                // get a box off the stack
      if( box0.child1->isDefined() && box0.child2->isDefined() ) // if both children are defined, set this box
      {
	for( dir=axis1; dir<rangeDimension; dir++ )
	{
	  box0.rangeBound(Start,dir)=min(box0.child1->rangeBound(Start,dir),
					 box0.child2->rangeBound(Start,dir));
	  box0.rangeBound(End  ,dir)=max(box0.child1->rangeBound(End  ,dir),
					 box0.child2->rangeBound(End  ,dir));
	}
        box0.setIsDefined();
      }
      else
      {
	boxStack.push( box0 );           // push this box back on, plus any undefined children
        if( !(box0.child1->isDefined()) )
	  boxStack.push( *box0.child1 );
        if( !(box0.child2->isDefined()) )
	  boxStack.push( *box0.child2 );
      }
    }
    if( Mapping::debug & 32 )
    {
      cout << "intializeBoundingBoxTrees side,axis = " << side << "," << axis << endl;
      cout << "Enter 1 display " << endl;
      cin >> dir1;
      if( dir1==1 ) boundingBoxTree[side][axis].display( " " );
    }
    // compute the bounding box for the whole grid:
    for( dir=axis1; dir<rangeDimension; dir++ )
    {
      boundingBox(Start,dir)=min(boundingBox(Start,dir),
				   boundingBoxTree[side][axis].rangeBound(Start,dir));
      boundingBox(End  ,dir)=max(boundingBox(End  ,dir),
				   boundingBoxTree[side][axis].rangeBound(End  ,dir));
    }
    
    return;
  }

  // *********************************************************************
  //    domainDimension==2 or 3 : make bounding boxes for each side
  // *********************************************************************

  for( side=Start; side<=End; side++ )
  {
    for( axis=axis1; axis<domainDimension; axis++ )  
    {
      bool notFunctionPeriodic = map->getIsPeriodic(axis) != Mapping::functionPeriodic; // *wdh* 070403
      #ifdef USE_PPP
      // in parallel we check the face if the periodic direction is distributed
      notFunctionPeriodic = (notFunctionPeriodic || 
			     indexRange(0,axis)!=map->gridIndexRange(0,axis) ||
			     indexRange(1,axis)!=map->gridIndexRange(1,axis));
      #endif
      if( notFunctionPeriodic )
      {
	// set domain dimensions of the root box:
	boundingBoxTree[side][axis].setDimensions( domainDimension,rangeDimension );
	boundingBoxTree[side][axis].setDomainBound(indexRange(Range(0,1),Axes));                 
	boundingBoxTree[side][axis].domainBound(Start,axis)=indexRange(side,axis);
	boundingBoxTree[side][axis].domainBound(End  ,axis)=indexRange(side,axis);
	dir1 = (axis+1) % domainDimension;
	dir2 = (axis+2) % domainDimension;

	boxStack.push( boundingBoxTree[side][axis] );
	while( !boxStack.isEmpty() )
	{
	  numberOfBoundingBoxes++;
	  BoundingBox & box0=boxStack.pop();       // get a box off the stack

	  if( Mapping::debug & 64 )
	    box0.display("intializeBoundingBoxTrees, box from stack..");

	  boxSize=(box0.domainBound(End,dir1)-box0.domainBound(Start,dir1)+1)
	    *(box0.domainBound(End,dir2)-box0.domainBound(Start,dir2)+1);
      
	  if( boxSize > maximumNumberOfPointsInSmallestBox )
	  {  // bisect the box (along longest axis in 3D)
	    box0.addChildren();  
	    if( domainDimension==2 )
	      dir = (axis +1) % domainDimension;
	    else 
	      dir = (box0.domainBound(End,dir1)-box0.domainBound(Start,dir1) >=
		     box0.domainBound(End,dir2)-box0.domainBound(Start,dir2) ) ? dir1 : dir2;
	    width = box0.domainBound(End,dir)-box0.domainBound(Start,dir);
	    box0.child1->domainBoundEquals(box0);
	    box0.child1->domainBound(Start,dir)=box0.domainBound(Start,dir);
	    box0.child1->domainBound(End  ,dir)=box0.domainBound(Start,dir)+width/2;
	    box0.child2->domainBoundEquals(box0);
	    box0.child2->domainBound(Start,dir)=box0.child1->domainBound(End,dir);
	    box0.child2->domainBound(End  ,dir)=box0.domainBound(End  ,dir);
	    boxStack.push( *box0.child1 );	
	    boxStack.push( *box0.child2 );	
	  }
	  else
	  { // the box is small enough, find range bounds for this box
	    for( dir=axis1; dir<domainDimension; dir++ )
	      I[dir]=Range(box0.domainBound(Start,dir),box0.domainBound(End,dir));
	    if( domainDimension==2 )
	      I[2]=Range(indexRange(side,axis3),indexRange(side,axis3));
	    // evaluate vertex...
	    for( dir=axis1; dir<rangeDimension; dir++ )
	    {
	      box0.rangeBound(Start,dir)=min(grid(I[0],I[1],I[2],dir));
	      box0.rangeBound(End  ,dir)=max(grid(I[0],I[1],I[2],dir));
	    }

	    if( false ) // *******************
	    {
              const int myid=max(0,Communication_Manager::My_Process_Number);
	      printf("myid=%i map=%s define box %i bounds=[%8.2e,%8.2e][%8.2e,%8.2e][%8.2e,%8.2e]\n",
		     myid,(const char*)map->getName(Mapping::mappingName),numberOfBoundingBoxes, 
                     box0.rangeBound(0,0),box0.rangeBound(1,0),
                     box0.rangeBound(0,0),box0.rangeBound(1,0),
                     box0.rangeBound(0,0),box0.rangeBound(1,0));
	    }
		
	    box0.setIsDefined(); // the box is now defined
	  }
	}
	// --- Now assign the rangeBound for all boxes.
	//   up till this point only the outer-most leaves in the tree have been assigned.
	if( boundingBoxTree[side][axis].child1!=NULL )
	  boxStack.push( boundingBoxTree[side][axis] );
	while( !boxStack.isEmpty() )
	{
	  BoundingBox & box0=boxStack.pop();                // get a box off the stack
	  if( box0.child1->isDefined() && box0.child2->isDefined() ) // if both children are defined, set this box
	  {
	    for( dir=axis1; dir<rangeDimension; dir++ )
	    {
	      box0.rangeBound(Start,dir)=min(box0.child1->rangeBound(Start,dir),
					     box0.child2->rangeBound(Start,dir));
	      box0.rangeBound(End  ,dir)=max(box0.child1->rangeBound(End  ,dir),
					     box0.child2->rangeBound(End  ,dir));
	    }
	    box0.setIsDefined(); // the box is now defined
	  }
	  else
	  {
	    boxStack.push( box0 );           // push this box back on, plus any undefined children
	    if( !(box0.child1->isDefined()) )
	      boxStack.push( *box0.child1 );
	    if( !(box0.child2->isDefined()) )
	      boxStack.push( *box0.child2 );
	  }
	}
	if( Mapping::debug & 32 )
	{
	  cout << "intializeBoundingBoxTrees side,axis = " << side << "," << axis << endl;
	  cout << "Enter 1 display " << endl;
	  cin >> dir1;
	  if( dir1==1 ) boundingBoxTree[side][axis].display( " " );
	}
	// compute the bounding box for the whole grid:
	for( dir=axis1; dir<rangeDimension; dir++ )
	{
	  boundingBox(Start,dir)=min(boundingBox(Start,dir),
				     boundingBoxTree[side][axis].rangeBound(Start,dir));
	  boundingBox(End  ,dir)=max(boundingBox(End  ,dir),
				     boundingBoxTree[side][axis].rangeBound(End  ,dir));
	}
      }
    } // end for axis
  } // end for side
  
  if( Mapping::debug & 4 )
  {
    printF("initializeBoundingBoxTrees: boundingBox=[%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e] (mapping=%s)\n",
	   boundingBox(0,0),boundingBox(1,0), boundingBox(0,1),boundingBox(1,1),  
           boundingBox(0,2),boundingBox(1,2),(const char*)map->getName(Mapping::mappingName));

    // Index I1=grid.dimension(0), I2=grid.dimension(1), I3=grid.dimension(2);
    // printF(" -> grid==[%9.3e,%9.3e][%9.3e,%9.3e]\n",
    // 	   min(grid(I1,I2,I3,0)),max(grid(I1,I2,I3,0)),min(grid(I1,I2,I3,1)),max(grid(I1,I2,I3,1)));
    
  }
  
}

/* ----
inline real 
distancel1( RealArray & xv, RealArray & grid, const int i1, const int i2,
                        const int i3, const int rangeDimension )
// l1 distance
{
  if( rangeDimension==2 )
    return fabs(xv(0)-grid(i1,i2,i3,0))+fabs(xv(1)-grid(i1,i2,i3,1));
  else if( rangeDimension==1 )
    return fabs(xv(0)-grid(i1,i2,i3,0));
  else 
    return fabs(xv(0)-grid(i1,i2,i3,0))+fabs(xv(1)-grid(i1,i2,i3,1))+fabs(xv(2)-grid(i1,i2,i3,2));
}
---- */

inline real 
distancel2( real xv[3], RealArray & grid, const int i1, const int i2,
                        const int i3, const int rangeDimension )
// ***** Square of the l2 distance  ****
{
  if( rangeDimension==2 )
    return SQR(xv[0]-grid(i1,i2,i3,0))+SQR(xv[1]-grid(i1,i2,i3,1));
  else if( rangeDimension==3 )
    return SQR(xv[0]-grid(i1,i2,i3,0))+SQR(xv[1]-grid(i1,i2,i3,1))+SQR(xv[2]-grid(i1,i2,i3,2));
  else 
    return SQR(xv[0]-grid(i1,i2,i3,0));
}
    
// define DISTANCE_L2_1(xv,i1,i2,i3) \
//        fabs(xv[0]-grid(i1,i2,i3,0))
// define DISTANCE_L2_2(xv,i1,i2,i3) \
//        SQR(xv[0]-grid(i1,i2,i3,0))+SQR(xv[1]-grid(i1,i2,i3,1))
// define DISTANCE_L2_3(xv,i1,i2,i3) \
//        SQR(xv[0]-grid(i1,i2,i3,0))+SQR(xv[1]-grid(i1,i2,i3,1))+SQR(xv[2]-grid(i1,i2,i3,2))

#define DISTANCE_L2_1(xv,i1,i2,i3) \
       fabs(xv[0]-GRID1(i1,0))
#define DISTANCE_L2_2(xv,i1,i2,i3) \
       SQR(xv[0]-GRID2(i1,i2,0))+SQR(xv[1]-GRID2(i1,i2,1))
#define DISTANCE_L2_3(xv,i1,i2,i3) \
       SQR(xv[0]-GRID3(i1,i2,i3,0))+SQR(xv[1]-GRID3(i1,i2,i3,1))+SQR(xv[2]-GRID3(i1,i2,i3,2))

#define GET_DISTANCE1(d,i1) \
      switch( rangeDimension )  \
      {  \
      case 1:  \
        d=SQR(xv[0]-GRID1(i1,0));  \
	break;  \
      case 2:  \
        d=SQR(xv[0]-GRID1(i1,0))+SQR(xv[1]-GRID1(i1,1));  \
	break;  \
      case 3:  \
        d=SQR(xv[0]-GRID1(i1,0))+SQR(xv[1]-GRID1(i1,1))+SQR(xv[2]-GRID1(i1,2));  \
	break;  \
      }


void ApproximateGlobalInverse::
findNearestGridPoint( const int base1, const int bound1, RealArray & x, RealArray & r )
//==================================================================================
/// \details 
///    Find the nearest grid point by a `stencil walk' and possibly a global
///    search over the boundary
/// 
///  For each point x(i,.), i=base1,...,bound1, find the index of the closest point on 
///  the boundary r(i,.).
/// 
///  <ol>
///    <li>  For a 1D grid start at the initial guess and look to the left
///           or to the right depending on whether the distance decreases
///           to the left or right.
///    <li>  For a 2D grid first do a local search, use the index arrays
///           to indicate which points in the square
///           to check (not all points need be searched as they would have
///           been done on the previous checks). If the local search ends
///           on a boundary then do a global search of all boundary points,
///           followed by another local search.  
///    <li>  For a 3D grid proceed as in 2 but use different index arrays
///  </ul>
//==================================================================================
{
  STENCIL_DEBUG2(real time0=getCPU();)

  assert( map!=NULL );
  // initialize here because interpolatePoints call this routine
  if( uninitialized || domainDimension != map->getDomainDimension()
                    || rangeDimension  != map->getRangeDimension() )
  {
    // cout << "findNearestGridPoint: call initialize...\n";
    initialize();  // Initialize first time (do here to allow for calls to setGrid)
  }
  if( Mapping::debug>0 )
    Mapping::openDebugFiles();

  FILE *&pDebugFile = Mapping::pDebugFile;
      
  if( Mapping::debug & 8 )
    fprintf(pDebugFile,"findNearestGridPoint... \n");

  real minimumDistance,distance;
  int axis,direction,dir,dir1,dir2,dir3,i1Old,i2Old,i3Old,i10,i20,i30,walk;
  int onBoundary;

  int iv[3], ivNew[3];
  int & i1 = iv[axis1];
  int & i2 = iv[axis2];
  int & i3 = iv[axis3];
  real xv[3];
  real & x1 = xv[axis1];
  real & x2 = xv[axis2];
  real & x3 = xv[axis3];
  
  int *index = &indexRange(0,0);
#define indexRange(side,axis) index[(side)+2*(axis)]

  // gid(side,axis) = gridIndexRange for the global (distributed) grid
  // dr[axis] : grid spacing on global grid
  real dr[3] = {1.,1.,1.}; // 
  bool isPeriodic[3]={false,false,false}; //
  int pgid[6] = {0,0,0,0,0,0};  //
  #define gid(side,axis) pgid[(side)+2*(axis)]
  for( axis=0; axis<domainDimension; axis++ )
  {
    for( int side=0; side<=1; side++ )
      gid(side,axis)=map->gridIndexRange(side,axis);
    dr[axis]=1./(max(1,gid(1,axis)-gid(0,axis)));
    // *wdh* 2013/08/02 -- only wrap for functionPeriodic 
    #ifdef USE_PPP
    isPeriodic[axis] = map->getIsPeriodic(axis)==Mapping::functionPeriodic && gid(0,axis)==indexRange(0,axis) && gid(1,axis)==indexRange(1,axis);
    #else
      isPeriodic[axis] = map->getIsPeriodic(axis)==Mapping::functionPeriodic;
    #endif 
//     #ifdef USE_PPP
//       isPeriodic[axis] = map->getIsPeriodic(axis) && gid(0,axis)==indexRange(0,axis) && gid(1,axis)==indexRange(1,axis);
//     #else
//       isPeriodic[axis] = map->getIsPeriodic(axis);
//     #endif 
  }
  
//   int *dimensionp = &dimension(0,0);
// #define dim(side,axis) dimensionp[(side)+2*(axis)]

  const int gDim0=grid.getRawDataSize(0);
  const int gDim1=grid.getRawDataSize(1);
  const int gDim2=grid.getRawDataSize(2);
  real *gridp1 = grid.Array_Descriptor.Array_View_Pointer1;
  real *gridp2 = grid.Array_Descriptor.Array_View_Pointer2;
  real *gridp3 = grid.Array_Descriptor.Array_View_Pointer3;
#define GRID1(i1,axis) gridp1[i1+gDim0*(axis)]
#define GRID2(i1,i2,axis) gridp2[i1+gDim0*(i2+gDim1*(axis))]
#define GRID3(i1,i2,i3,axis) gridp3[i1+gDim0*(i2+gDim1*(i3+gDim2*(axis)))]

  for( int i=base1; i<=bound1; i++ )
  {
    if( Mapping::debug & 8 )
      fprintf(pDebugFile,"findNearestGridPoint: point i =%i\n",i);

    for( axis=0; axis<rangeDimension; axis++ )
      xv[axis]=x(i,axis); 

    if( domainDimension==rangeDimension )
    {
      // First check if the point is in the bounding box (plus a bit)
      if( !findBestGuess )
      {
	bool ok=TRUE;
	for( axis=axis1; axis<rangeDimension; axis++ )
	{
	  distance=stencilWalkBoundingBoxExtensionFactor*(boundingBox(End,axis)-boundingBox(Start,axis));
	  if( xv[axis] < (boundingBox(Start,axis)-distance) ||
	      xv[axis] > (boundingBox(End  ,axis)+distance) )
	  {
	    ok=FALSE;
	    r(i,Axes)=bogus;
	    if( Mapping::debug & 8 )
	    {
	      fprintf(pDebugFile,"findNearestGridPoint: pt i=%i xv=(%8.2e,%8.2e) is outside"
                     " bounding box= [%8.2e,%8.2e][%8.2e,%8.2e]\n",
		     i,xv[0],xv[1],boundingBox(0,0),boundingBox(1,0),boundingBox(0,1),boundingBox(1,1));
	    }
	    
	    break;
	  }
	}
	if( !ok ) continue;
      }
      
/* ----
      // now check if the point is near a point that is outside the grid
      for( j=0; j<numberOfOutsidePoints; j++ )
      {
        if( distancel2(xv[xAxes],outsidePoint(xAxes,j)) < outsidePointDistance(j) )
        { 
	  ok=FALSE;
          r(i,Axes)=bogus;
	  break;
        }
      }
      if( !ok ) continue;
      ----- */	
    }
    
    numberOfStencilWalks++;

    // get initial guess, use given value if valid ******
    bool noInitialGuess=false;
    for( axis=axis1; axis<domainDimension; axis++ )
    {
      if( r(i,axis)>=0. && r(i,axis)<=1. )
  	iv[axis]=int(r(i,axis)/dr[axis] + gid(0,axis)+.5);
      else if( i>base1 && r(i-1,axis)>=0. && r(i-1,axis)<=1. )  // use previous point
	iv[axis]=int(r(i-1,axis)/dr[axis]+gid(0,axis)+.5);
      else
      {
	noInitialGuess=true;
  	iv[axis]=int(.5*(indexRange(1,axis)+indexRange(0,axis))+.5);     // mid point
      }
      
      #ifdef USE_PPP
        // in parallel -- restrict initial guess to the local grid dimensions
        iv[axis] = max(indexRange(0,axis),min(indexRange(1,axis),iv[axis]));
      #endif
    }
    
    for( axis=domainDimension; axis<3; axis++ )
      iv[axis]=indexRange(Start,axis);

    if( domainDimension==1 )  // 1D stencil search
    {
      minimumDistance=distancel2(xv,grid,i1,i2,i3,rangeDimension);
      GET_DISTANCE1(minimumDistance,i1);
      // first determine whether to look to the left or right
      // distance=(i1+1) <= indexRange(End,axis1) ? distancel2(xv,grid,i1+1,i2,i3,rangeDimension) 
      //                                  : minimumDistance+1.;
      if( (i1+1) <= indexRange(End,axis1) )
      {
	GET_DISTANCE1(distance,i1+1);
      }
      else
      {
	distance=minimumDistance+1.;
      }
      
      if( distance < minimumDistance )
      {
        i1++; minimumDistance=distance;
        direction=+1;  
      }
      else
      {
        direction=-1; 
      }
      // move in the chosen direction until the distance starts to increase
      for( axis=axis1; axis<2; axis++ )  // loop for periodic boundaries
      {
        for( i10=i1+direction; i10<=indexRange(End,axis1) && i10>=indexRange(Start,axis1);
             i10+=direction )
        {
          numberOfStencilSearches++;
          // distance=distancel2(xv,grid,i10,i2,i3,rangeDimension);
          GET_DISTANCE1(distance,i10);
          if( distance<minimumDistance )
  	  {
             minimumDistance=distance;
             i1=i10;
	   }
	  else
            break;
        }
        if( isPeriodic[axis1] && (i1==indexRange(Start,axis1) || i1==indexRange(End,axis1)) )
        {
	  i1= i1==indexRange(Start,axis1) ? indexRange(End,axis1) : indexRange(Start,axis1);
	}
	else
          break;
      }
      if( rangeDimension>1 )
      { // find the global minimum (this uses the local minimum to speed up the search)
        binarySearchOverBoundary( xv,minimumDistance,iv ); 
      }
    }
    else  // 2D or 3D stencil walk
    {

      bool boundarySearch=false;  // set to true if a boundary search has been performed
      for( walk=0; walk<domainDimension+1; walk++ ) // we may have to more than one stencil walk - if
      {                                             // we hit a periodic side or a boundary

	if( noInitialGuess && domainDimension==2 && rangeDimension==3 ) // *wdh* 081101
	{ // For surfaces we save keep a bounding box tree for the entire surface
	  if( Mapping::debug & 8 )
	    printF("findNearestGridPoint: check surface for nearest pt: i=(%i,%i) **noInitialGuess** "
		   "r=(%e,%e)\n",i1,i2,r(i,0),r(i,1));
	  if( rangeDimension==2 )
	    minimumDistance=DISTANCE_L2_2(xv,i1,i2,i3);
	  else
	    minimumDistance=DISTANCE_L2_3(xv,i1,i2,i3);
	  binarySearchOverBoundary( xv,minimumDistance,iv );
	  break;  // we have found the globally closest point 
	}

        if( domainDimension==2 )
	{
          // minmumDistance=distancel2(xv,grid,i1,i2,i3,rangeDimension); // use l2 distance! needed
	  if( rangeDimension==2 )
	    minimumDistance=DISTANCE_L2_2(xv,i1,i2,i3);
	  else
	    minimumDistance=DISTANCE_L2_3(xv,i1,i2,i3);
 	  i1Old=i1;  i2Old=i2;
	  for( ;; )
	  {
            numberOfStencilSearches++;
	    dir1=i1-i1Old;  dir2=i2-i2Old;
  	    i1Old=i1;  i2Old=i2;
            // now look in some Directions
	    for( dir=0; dir<numberOfStencilDirections2D(dir1,dir2); dir++ )
	    {
	      i20=i2Old+stencilDirection2D2(dir,dir1,dir2);
	      if( i20>=indexRange(Start,axis2) && i20<=indexRange(End,axis2) )
	      {
 	        i10=i1Old+stencilDirection2D1(dir,dir1,dir2);
	        if( i10>=indexRange(Start,axis1) && i10<=indexRange(End,axis1) )
	        {
                  if( rangeDimension==2 )
                    distance=DISTANCE_L2_2(xv,i10,i20,i3);
                  else
                    distance=DISTANCE_L2_3(xv,i10,i20,i3);
	          if( distance<minimumDistance )
		  {
                    minimumDistance=distance;
                    i1=i10; i2=i20;
	  	  }
	        }
	      }
	    }
	    if( i1==i1Old && i2==i2Old ) break;
	  }
          onBoundary= i1==indexRange(Start,axis1) || i1==indexRange(End,axis1) ||
                      i2==indexRange(Start,axis2) || i2==indexRange(End,axis2);

          // Optionally look for the closest cell rather than the closest point. A cell based
          // search is more robust but more expensive.
          if( useRobustApproximateInverse )
	  {
	    onBoundary=findNearestCell(xv,iv,minimumDistance );
	  }
	  
	}
	else  // domainDimension==3
	{
          minimumDistance=DISTANCE_L2_3(xv,i1,i2,i3); // use l2 distance! needed
	  i1Old=i1; i2Old=i2; i3Old=i3;
	  for( ;; )
	  {
	    numberOfStencilSearches++;
	    dir1=i1-i1Old; dir2=i2-i2Old; dir3=i3-i3Old;
	    i1Old=i1;  i2Old=i2; i3Old=i3;
	    // now look in some Directions
	    for( dir=0; dir<numberOfStencilDirections3D(dir1,dir2,dir3); dir++ )
	    {
	      i30=i3Old+stencilDirection3D3(dir,dir1,dir2,dir3);
	      if( i30>=indexRange(Start,axis3) && i30<=indexRange(End,axis3) )
	      {
		i20=i2Old+stencilDirection3D2(dir,dir1,dir2,dir3);
		if( i20>=indexRange(Start,axis2) && i20<=indexRange(End,axis2) )
		{
		  i10=i1Old+stencilDirection3D1(dir,dir1,dir2,dir3);
		  if( i10>=indexRange(Start,axis1) && i10<=indexRange(End,axis1) )
		  {
                    distance=DISTANCE_L2_3(xv,i10,i20,i30);
		    if( distance<minimumDistance )
		    {
		      minimumDistance=distance;
		      i1=i10; i2=i20; i3=i30;
		    }
		  }
		}
	      }
	    }
	    if( i1==i1Old && i2==i2Old && i3==i3Old ) break;
	  }
	  onBoundary= i1==indexRange(Start,axis1) || i1==indexRange(End,axis1) ||
	              i2==indexRange(Start,axis2) || i2==indexRange(End,axis2) ||
	              i3==indexRange(Start,axis3) || i3==indexRange(End,axis3);

          // Optionally look for the closest cell rather than the closest point. A cell based
          // search is more robust but more expensive.
          if( useRobustApproximateInverse )
	    onBoundary=findNearestCell(xv,iv,minimumDistance );
	}


        if( !boundarySearch &&  // this is the first boundary search
             onBoundary )      // ** don't do if minimumDistance is small enough!!
	{ // first check to see if we are on a periodic boundary
          // for a periodic boundary and not a true boundary, we step across and do a local search
          // for a true boundary and/or periodic we do a boundary search, then a local search
          // only one boundary search is done.
          onBoundary=false;  // assume we are on a periodic side only
	  
          for( axis=axis1; axis<domainDimension; axis++ )
	  {
	    if( isPeriodic[axis] && ( iv[axis]==indexRange(Start,axis) || iv[axis]==indexRange(End,axis) ) )
	    {
              if( Mapping::debug & 4 ) fprintf(pDebugFile,"stencil walk: cross periodic boundary..\n");
	      iv[axis] = iv[axis]==indexRange(Start,axis) ? indexRange(End,axis)
		                                          : indexRange(Start,axis);
	    }
	    else if( iv[axis]==indexRange(Start,axis) || iv[axis]==indexRange(End,axis) )
	    {
	      onBoundary=true;  // we are really on a true boundary
              break;
	    }
	  }
          if( onBoundary ) // on a true boundary
	  {
            ivNew[0]=iv[0]; ivNew[1]=iv[1]; ivNew[2]=iv[2];
            binarySearchOverBoundary( xv,minimumDistance,ivNew ); 

            boundarySearch=true;
            // redo local search only if the point is different:
            if(  max(abs(iv[0]-ivNew[0]),abs(iv[1]-ivNew[1]),abs(iv[2]-ivNew[2]))==0 ) 
              break;    // no more local searches
            iv[0]=ivNew[0]; iv[1]=ivNew[1]; iv[2]=ivNew[2];
	  }
	}
        else
          break;    // no more local searches
      }
    }
    
    if( Mapping::debug & 16 ) 
    {
      if( rangeDimension==3 )
        fprintf(pDebugFile,"StencilWalk: i=%i x=(%e,%e,%e), closest point : (i1,i2,i3)=(%i,%i,%i), x(i1,i2,i3)=(%e,%e,%e)\n",
	       i,x1,x2,x3,i1,i2,i3,grid(i1,i2,i3,0),grid(i1,i2,i3,1),grid(i1,i2,i3,2));
      else if( rangeDimension==2 )
        fprintf(pDebugFile,"StencilWalk: i=%i x=(%e,%e), closest point : (i1,i2)=(%i,%i), x(i1,i2,i3)=(%e,%e)\n",
	       i,x1,x2,i1,i2,grid(i1,i2,i3,0),grid(i1,i2,i3,1));
      else 
        fprintf(pDebugFile,"StencilWalk: x=(%e), closest point : (i1)=(%i), x(i1)=(%e)\n",x1,i1,grid(i1,i2,i3,0));
    }
    
    if( Mapping::debug & 4 && domainDimension==2 )
    { // check that this is really a closest point ** 2D only ***
      ivNew[0]=iv[0]; ivNew[1]=iv[1]; ivNew[2]=iv[2];
      for( i20=i2-1; i20<=i2+1; i20++ )
      {
	if( i20>=indexRange(Start,axis2) && i20<=indexRange(End,axis2) )
          for( i10=i1-1; i10<=i1+1; i10++ )
	    if( i10>=indexRange(Start,axis1) && i10<=indexRange(End,axis1) )
	    {
              distance=distancel2(xv,grid,i10,i20,i3,rangeDimension);
	      if( distance<minimumDistance )
	      {
                minimumDistance=distance;
                ivNew[0]=i10; ivNew[1]=i20;
	      }
	    }
      }	
      if( max(abs(iv[0]-ivNew[0]),abs(iv[1]-ivNew[1]),abs(iv[2]-ivNew[2]))!=0 ) 
      {
	fprintf(pDebugFile,"StencilWalk:WARNING: double check local search failed! (i1,i2)=(%i,%i) but (%i,%i) is closer!\n",
	       i1,i2,ivNew[0],ivNew[1]);
        iv[0]=ivNew[0]; iv[1]=ivNew[1]; iv[2]=ivNew[2];
      }
    }
    if( Mapping::debug & 2 ) fprintf(pDebugFile,"stencilWalk: i=%i closest pt: rr=[",i);
    for( axis=axis1; axis<domainDimension; axis++ )
    {
      // *wdh* 020526 real rr = (iv[axis]-indexRange(Start,axis))/real(indexRange(End,axis)-indexRange(Start,axis));
      real rr = (iv[axis]-gid(0,axis))*dr[axis];
      // use the initial guess rather than the nearest grid point if we are within .75*(grid spacing)
      if( fabs(rr-r(i,axis)) > .75*dr[axis] )
      {
	r(i,axis)=rr;
	if( Mapping::debug & 2 ) fprintf(pDebugFile," %9.3e (iv=%i,ir=%i,gid=%i),",rr,iv[axis],indexRange(0,axis),gid(0,axis));
      }
      else
	if( Mapping::debug & 2 ) fprintf(pDebugFile," %9.3e (use init guess,iv=%i),",r(i,axis),iv[axis]);
    }
    if( Mapping::debug & 2 ) fprintf(pDebugFile,"]\n");
      
  }
  STENCIL_DEBUG2(timeForFindNearestGridPoint+=getCPU()-time0;)
}
#undef indexRange
#undef GRID1
#undef GRID2
#undef GRID3

static list<BoundingBox*> boxStack;



void ApproximateGlobalInverse::
binarySearchOverBoundary( real x[3], 
			  real & minimumDistance, 
			  int iv[3],
                          int side /* = -1 */,
                          int axis /* = -1 */ )
//==================================================================================
/// \details 
///    Binary Search over the boundary.
/// 
///  For point x find the index of the closest point on the boundary iv
///  iv should be given a value on input, current closest point
/// 
///  For curves or surfaces we search the entire surface for the closest point
/// 
/// \param x (input) : point to search for.
/// \param minimumDistance (input/output) : NOTE that this "distance" is the SQUARE of the L2 norm.
///      On input : find a point with minimum distance less
///      than this value. On output, if minimumDistance is less than the input value then
///      this will be the new minimum distance and iv will hold the point on the boundary
///     that is closest.
/// \param iv (output) : Closest boundary ONLY IF a point is found that is closer than the input 
///    value of minimumDistance.
/// \param side,axis (input) : optionally specify to only search this face.
//==================================================================================
{
  STENCIL_DEBUG(real time0=getCPU();)
  if( Mapping::debug & 8 )
  {
    printF("binarySearchOverBoundary: x=(%9.3e,%9.3e,%9.3e)\n",x[0],x[1],x[2]);
  }

  int iiv[3], &i1=iiv[0], &i2=iiv[1], &i3=iiv[2];
  
  int *index = &indexRange(0,0);
#define indexRange(side,axis) index[(side)+2*(axis)]


  const int gDim0=grid.getRawDataSize(0);
  const int gDim1=grid.getRawDataSize(1);
  const int gDim2=grid.getRawDataSize(2);
  real *gridp1 = grid.Array_Descriptor.Array_View_Pointer1;
  real *gridp2 = grid.Array_Descriptor.Array_View_Pointer2;
  real *gridp3 = grid.Array_Descriptor.Array_View_Pointer3;
#define GRID1(i1,axis) gridp1[i1+gDim0*(axis)]
#define GRID2(i1,i2,axis) gridp2[i1+gDim0*(i2+gDim1*(axis))]
#define GRID3(i1,i2,i3,axis) gridp3[i1+gDim0*(i2+gDim1*(i3+gDim2*(axis)))]

  real distance;
  
  numberOfBinarySearches++;

  if( domainDimension < rangeDimension )
  {
    // *********** Find the closest point on this curve or surface ********
    side=0;
    axis=0;
    if( domainDimension<2 ) i2=indexRange(Start,axis2);  
    i3=indexRange(Start,axis3);
    
    boxStack.push_front( &boundingBoxTree[side][axis] );  
    while( !boxStack.empty() )
    {
      numberOfBoxesChecked++;
      BoundingBox & box0= *boxStack.front();             // get a box off the stack
      boxStack.pop_front();
      distance=0.;
      for( int dir=0; dir<rangeDimension; dir++ )
      {
	real dist= max(max(box0.rangeBound(Start,dir)-x[dir],x[dir]-box0.rangeBound(End,dir)),0.);
	distance+=SQR(dist);
      }
      if( distance<minimumDistance )  // check this box
      { 
        if( box0.child1!=NULL )
	{ // push children onto the stack
          boxStack.push_front( box0.child1 );
          boxStack.push_front( box0.child2 );
	}
	else
	{
          const int i1Bound=box0.domainBound(End,axis1);
          if( domainDimension==1 )
	  {
            switch (rangeDimension)
	    {
	    case 2:
	      for( i1=box0.domainBound(Start,axis1); i1<=i1Bound; i1++ )
	      {
		distance=SQR(x[0]-GRID1(i1,0))+SQR(x[1]-GRID1(i1,1));
		if( distance<minimumDistance )
		{
		  minimumDistance=distance;
		  iv[axis1]=i1;
		}
	      }
	      break;
	    case 3:
	      for( i1=box0.domainBound(Start,axis1); i1<=i1Bound; i1++ )
	      {
		distance=SQR(x[0]-GRID1(i1,0))+SQR(x[1]-GRID1(i1,1))+SQR(x[2]-GRID1(i1,2));
		if( distance<minimumDistance )
		{
		  minimumDistance=distance;
		  iv[axis1]=i1;
		}
	      }
	    }
	  }
	  else
	  {
	    
            const int i1End=box0.domainBound(End,axis1); 
	    int i2Start= domainDimension==1 ? indexRange(Start,axis2) : box0.domainBound(Start,axis2);
	    int i2End  = domainDimension==1 ? indexRange(Start,axis2) : box0.domainBound(End  ,axis2);
	    for( i2=i2Start; i2<=i2End; i2++ )
	    {
	      for( i1=box0.domainBound(Start,axis1); i1<=i1End; i1++ )
	      {
		distance=SQR(x[0]-GRID2(i1,i2,0))+SQR(x[1]-GRID2(i1,i2,1))+SQR(x[2]-GRID2(i1,i2,2));
		if( distance<minimumDistance )
		{
		  minimumDistance=distance;
		  iv[axis1]=i1; iv[axis2]=i2;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  else
  {
    // domainDimension==rangeDimension
    // Loop over the boxes for each side of the grid
    assert( side==-1 || side==0 || side==1 );
    assert( axis==-1 || (axis>=0 && axis<domainDimension) );


    int axisStart = axis==-1 ? axis1 : axis;
    int axisEnd   = axis==-1 ? domainDimension-1 : axis;
    int sideStart = side==-1 ? Start : side;
    int sideEnd   = side==-1 ? End : side;
    for( axis=axisStart; axis<=axisEnd; axis++ )
    {
      bool notFunctionPeriodic = map->getIsPeriodic(axis) != Mapping::functionPeriodic;
      #ifdef USE_PPP
      // in parallel we check the face if the periodic direction is distributed
      notFunctionPeriodic = (notFunctionPeriodic || 
			     indexRange(0,axis)!=map->gridIndexRange(0,axis) ||
			     indexRange(1,axis)!=map->gridIndexRange(1,axis));
      #endif

      // if( map->getIsPeriodic(axis) != Mapping::functionPeriodic )
      if( notFunctionPeriodic )
      {
	for( side=sideStart; side<=sideEnd; side++ )
	{
          assert( boundingBoxTree[side][axis].isDefined() );
	  
	  boxStack.push_front( &boundingBoxTree[side][axis] );  
	  while( !boxStack.empty() )
	  {

	    numberOfBoxesChecked++;
	    BoundingBox & box0=*boxStack.front();             // get a box off the stack
	    boxStack.pop_front();             // get a box off the stack
	    distance=0.;
	    for( int dir=0; dir<rangeDimension; dir++ )
	    {
	      real dist= max(max(box0.rangeBound(Start,dir)-x[dir],x[dir]-box0.rangeBound(End,dir)),0.);
	      distance+=SQR(dist);
	    }
	    if( distance<minimumDistance )  // check this box
	    { 
	      if( box0.child1!=NULL )
	      { // push children onto the stack
		boxStack.push_front( box0.child1 );
		boxStack.push_front( box0.child2 );
	      }
	      else
	      {
                if( !useRobustApproximateInverse )
		{
                  STENCIL_DEBUG(real tm0=getCPU();)
		  int i3Start= domainDimension==2 ? indexRange(Start,axis3) : box0.domainBound(Start,axis3);
		  const int i1Start=box0.domainBound(Start,axis1), i1End=box0.domainBound(End,axis1);
		  const int i2Start=box0.domainBound(Start,axis2), i2End=box0.domainBound(End,axis2);
                  // only check leaf box if we are not already in it! *wdh* 000928
                  // if( iv[0]<i1Start || iv[0]>i1End || iv[1]<i2Start || iv[1]>i2End ||
                  //     iv[2]<i3Start || iv[2]>i3End )
		  switch (rangeDimension)
		  {
		  case 2:
		    iv[axis3]=i3Start;
		    for( i2=i2Start; i2<=i2End; i2++ )
		      for( i1=i1Start; i1<=i1End; i1++ )
		      {
// 			real x0=x[0]-GRID2(i1,i2,0);
// 			real x1=x[1]-GRID2(i1,i2,1);
// 			distance=x0*x0+x1*x1;
			distance=SQR(x[0]-GRID2(i1,i2,0))+SQR(x[1]-GRID2(i1,i2,1));
			if( distance<minimumDistance )
			{
			  minimumDistance=distance;
			  iv[axis1]=i1; iv[axis2]=i2; 
			}
		      }
		    break;
		  case 3:
		  {
  		    int i3End  = box0.domainBound(End  ,axis3);
		    for( i3=i3Start; i3<=i3End; i3++ )
		      for( i2=i2Start; i2<=i2End; i2++ )
			for( i1=i1Start; i1<=i1End; i1++ )
			{
//                           real x0=x[0]-GRID3(i1,i2,i3,0);
// 			  real x1=x[1]-GRID3(i1,i2,i3,1);
// 			  real x2=x[2]-GRID3(i1,i2,i3,2);
// 			  distance=x0*x0+x1*x1+x2*x2;
			  distance=SQR(x[0]-GRID3(i1,i2,i3,0))+SQR(x[1]-GRID3(i1,i2,i3,1))+SQR(x[2]-GRID3(i1,i2,i3,2));
			  if( distance<minimumDistance )
			  {
			    minimumDistance=distance;
			    iv[axis1]=i1; iv[axis2]=i2; iv[axis3]=i3;
			  }
			}
		    break;
		  }
		  case 1:
		    iv[axis3]=i3Start;
		    iv[axis2]=i2Start;
		    for( i1=i1Start; i1<=i1End; i1++ )
		    {
// 		      real x0 = x[0]-GRID1(i1,0);
// 		      distance=x0*x0;
		      distance=SQR(x[0]-GRID1(i1,0));
		      if( distance<minimumDistance )
		      {
			minimumDistance=distance;
			iv[axis1]=i1; 
		      }
		    }
		    break;
		  }
		  
		  // printf("bin search: min: iv=(%i,%i,%i) \n",iv[0],iv[1],iv[2]);

                  STENCIL_DEBUG(timeForBinarySearchOnLeaves+=getCPU()-tm0;)
		}
		else
		{
                  // use robust method
                  // printf("use robust binary search...\n");
		  
                  // we check the distance to the cells along the boundary
                  int iStart[3], &i1Start = iStart[0], &i2Start=iStart[1], &i3Start=iStart[2];
                  int iEnd  [3], &i1End   = iEnd  [0], &i2End  =iEnd  [1], &i3End  =iEnd  [2];
		  
                  i1Start=box0.domainBound(Start,axis1);
		  i1End = min(box0.domainBound(End  ,axis1),indexRange(End,axis1)-1);
                  i2Start=box0.domainBound(Start,axis2);
		  i2End = min(box0.domainBound(End  ,axis2),indexRange(End,axis2)-1);
		  
		  i3Start= domainDimension==2 ? indexRange(Start,axis3) : box0.domainBound(Start,axis3);
		  i3End  = domainDimension==2 ? indexRange(Start,axis3) : 
                           min(box0.domainBound(End  ,axis3),indexRange(End,axis3)-1);

                  if( i1End>=grid.getBound(0) ) // *wdh* 091130 
		  {
		    printF("ApproximateGlobalInverse::binarySearchOverBoundary:ERROR: box=[%i,%i][%i,%i] but grid has "
                           "bounds [%i,%i]x[%i,%i], indexRange=[%i,%i]x[%i,%i]\n",
			   i1Start,i1End,i2Start,i2End,grid.getBase(0),grid.getBound(0),grid.getBase(1),grid.getBound(1),
                            indexRange(0,0),indexRange(1,0),indexRange(0,1),indexRange(1,1));
		    OV_ABORT("ERROR");
		  }
		  


                  // Since we check cells we must shift the index value for points on the End
                  if( side==End )
		  {
                    iStart[axis] = indexRange(End,axis)-1;
                    iEnd  [axis] = indexRange(End,axis)-1;
		  }
		  for( i3=i3Start; i3<=i3End; i3++ )
		    for( i2=i2Start; i2<=i2End; i2++ )
		      for( i1=i1Start; i1<=i1End; i1++ )
		      {
			distanceToCell( x, iiv, distance, minimumDistance );
                        // printf("binary: new distance=%e, old minimumDistance=%e \n",distance,minimumDistance );
			
			if( distance<minimumDistance )
			{
			  iv[axis1]=i1; iv[axis2]=i2; iv[axis3]=i3;
                          if( distance<=0. )
			  {
                            // we are done!
                            // printf("binary: enclosing cell have been found!\n");
			    minimumDistance=0.;
                            // ** should break out here
//                             while( !boxStack.empty() )
// 			      boxStack.pop_front();
                            boxStack.clear();
			    
                            i2=i2End+1;
			    i3=i3End+1;
			    break;
			  }
			  minimumDistance=distance;
			}
		      }
                
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  if( Mapping::debug & 8 )
    printF("binarySearchOverBoundary iv=(%i,%i,%i) numberOfBoxesChecked=%i (current total)\n",iv[0],iv[1],iv[2],
              numberOfBoxesChecked);

  STENCIL_DEBUG(timeForBinarySearchOverBoundary+=getCPU()-time0;)
}

#undef indexRange
#undef GRID1
#undef GRID2
#undef GRID3

//* ------ not used anymore ---

//* begin{>>ApproximateGlobalInverseInclude.tex}{\subsubsection{binarySearchOverBoundary}}
//* void ApproximateGlobalInverse::
//* robustBinarySearchOverBoundary( real x[3], 
//* 			  real & minimumDistance, 
//* 			  int iv[3],
//*                           int side,
//*                           int axis  )
//* //==================================================================================
//* // /Description:
//* //   Robust Binary Search over the boundary. ** use this for a thin wing or  c-grid ***
//* //
//* // /Method:
//* //   Search for local minima in the top two bounding boxes.
//* //
//* // For point x find the index of the closest point on the boundary iv
//* // iv should be given a value on input, current closest point
//* //
//* // For curves or surfaces we search the entire surface for the closest point
//* //
//* //  /x (input) : point to search for.
//* // /minimumDistance (input/output) : NOTE that this "distance" is the SQUARE of the L2 norm.
//* //     On input : find a point with minimum distance less
//* //     than this value. On output, if minimumDistance is less than the input value then
//* //     this will be the new minimum distance and iv will hold the point on the boundary
//* //    that is closest.
//* // /iv (output) : Closest boundary ONLY IF a point is found that is closer than the input 
//* //   value of minimumDistance.
//* // /side,axis (input) : optionally specify to only search this face.
//* //\end{ApproximateGlobalInverseInclude.tex}
//* //==================================================================================
//* {
//*   STENCIL_DEBUG(real time0=getCPU();)
//*   if( Mapping::debug & 4 )
//*     cout << "robustBinarySearchOverBoundary x=(" << x[0] << "," 
//*          << ( domainDimension > 1 ? x[1] : 0.) << "," 
//*          << ( domainDimension > 2 ? x[2] : 0.) << ")" << endl;
//* 
//*   int i1,i2,i3;
//*   int *index = &indexRange(0,0);
//* #define indexRange(side,axis) index[(side)+2*(axis)]
//*   real distance;
//*   
//*   numberOfBinarySearches++;
//* 
//*   assert( domainDimension==rangeDimension );
//*   
//*   // Loop over the boxes for each side of the grid
//*   assert( side==0 || side==1 );
//*   assert( axis>=0 && axis<domainDimension );
//* 
//*   bool inside=FALSE;
//*   real dot[2]={0.,0.};
//*   
//*   if( map->getIsPeriodic(axis) != Mapping::functionPeriodic )
//*   {
//*     int jv1[3],jv2[3];
//*     real minDistance[2];
//*     for( int m=0; m<=1; m++ )
//*     {
//*       real & minDist = minDistance[m];
//*       minDist=REAL_MAX;
//*       
//*       int *jv = m==0 ? jv1 : jv2;
//*       jv[0]=iv[0], jv[1]=iv[1], jv[2]=iv[2];
//*       
//*       if( m==0 )
//*       {
//* 	assert( boundingBoxTree[side][axis].child1!=0 );
//*         boxStack.push( *(boundingBoxTree[side][axis].child1) );  
//*       }
//*       else
//*       {
//* 	assert( boundingBoxTree[side][axis].child2!=0 );
//*         boxStack.push( *(boundingBoxTree[side][axis].child2) );  
//*       }
//*       
//*       while( !boxStack.isEmpty() )
//*       {
//* 	numberOfBoxesChecked++;
//* 	BoundingBox & box0=boxStack.pop();             // get a box off the stack
//* 	distance=0.;
//* 	for( int dir=0; dir<rangeDimension; dir++ )
//* 	{
//* 	  real dist= max(max(box0.rangeBound(Start,dir)-x[dir],x[dir]-box0.rangeBound(End,dir)),0.);
//* 	  distance+=SQR(dist);
//* 	}
//* 	if( distance<minDist )  // check this box
//* 	{ 
//* 	  if( box0.child1!=NULL )
//* 	  { // push children onto the stack
//* 	    boxStack.push( *box0.child1 );
//* 	    boxStack.push( *box0.child2 );
//* 	  }
//* 	  else
//* 	  {
//* 	    int i3Start= domainDimension==2 ? indexRange(Start,axis3) : box0.domainBound(Start,axis3);
//* 	    int i3End  = domainDimension==2 ? indexRange(Start,axis3) : box0.domainBound(End  ,axis3);
//* 	    for( i3=i3Start; i3<=i3End; i3++ )
//* 	      for( i2=box0.domainBound(Start,axis2); i2<=box0.domainBound(End,axis2); i2++ )
//* 		for( i1=box0.domainBound(Start,axis1); i1<=box0.domainBound(End,axis1); i1++ )
//* 		{
//* 		  distance=distancel2(x,grid,i1,i2,i3,rangeDimension); 
//* 		  if( distance<minDist )
//* 		  {
//* 		    minDist=distance;
//* 		    jv[axis1]=i1; jv[axis2]=i2; jv[axis3]=i3;
//* 		  }
//* 		}
//* 	  }
//* 	}
//*       }
//*       // check if we are inside the grid near the point jv
//*       if( insideGrid( side,axis,x,jv,dot[m] ) )
//*       {
//*         inside=TRUE;
//* 	iv[0]=jv[0], iv[1]=jv[1], iv[2]=jv[2];
//* 	minimumDistance=minDist;
//* 	break;
//*       }
//*     }  // end for m
//* 
//*   }
//*   if( !inside )
//*   {
//*     // for now we do nothing if not inside either --- could choose the 
//*     // side with min fabs(dot[m])
//*   }
//*   
//*   if( Mapping::debug & 4 )
//*     cout << "robustBinarySearchOverBoundary iv=(" << iv[0] << "," << iv[1] << "," << iv[2] << ")" 
//*       " numberOfBoxesChecked =" << numberOfBoxesChecked << endl;
//* 
//*   STENCIL_DEBUG(timeForBinarySearchOverBoundary+=getCPU()-time0;)
//* }
//* 

#undef indexRange

int ApproximateGlobalInverse::
findNearestCell(real x[3], 
		int iv[3],
		real & minimumDistance )
//==================================================================================
/// \details 
///    Find the nearest grid cell by a `stencil walk' . This search technique may be
///    needed for highly stretched grids since the closest grid point to x may be
///    many cells away. 
/// 
/// \param iv (input/output) : on input, the initial guess for the closest cell. On output the 
///      nearest cell (locally, may end on a boundary).
/// 
/// \param minimumDistance (output): return 0 if x is inside the cell iv. Otherwise the minimum distance
///    is NOT computed by this routine (for efficiency) since the algorithm does not really require it.
/// \return 
///  <ul>
///    <li> <B>0</B> point x is inside the cell. 
///    <li> <B>1</B> stencil walk has reached a boundary and the point is apparently not inside the cell.
///  </ul>
//==================================================================================
{
  if( Mapping::debug & 2 )
    printf("findNearestCell \n");

  assert( domainDimension==rangeDimension );
  
  // Two dimensional case:
  // Check whether x is below line AB or above CD or left of AC or right of BD
  // Adjust the closest pt appropriately
  // 
  //              C----------D
  //              |          |
  //          x   |   x      |
  //              |          |
  //              |          |
  //              A----------B
  // 
  int & i1=iv[0], & i2=iv[1], & i3=iv[2];
  const int i10=i1, i20=i2, i30=i3;  // save initial position
    
  const real signForJacobian=map->getSignForJacobian();
  const int maximumNumberOfTries=50;
  const int warningStep=maximumNumberOfTries/2;
  
  int previousMove=-1;  // previousMove = side+2*axis of the direction moved.
  
  if( domainDimension==2 )
  {
    int iva[3], &ia1 = iva[0], &ia2=iva[1]; //  &ia3=iva[2];
    int ivb[3], &ib1 = ivb[0], &ib2=ivb[1]; //  &ib3=ivb[2];

    //              left   right bot  top
    int ia[2][2][2]={0,1,  1,0,  0,0, 1,1}; 
    int ib[2][2][2]={0,0,  1,1,  1,0, 0,1}; 

    if( i1<indexRange(Start,axis1) || i1>indexRange(End,axis1) ||
        i2<indexRange(Start,axis2) || i2>indexRange(End,axis2) )
    {
      return 1;  // point is outside
    }

    bool changed=TRUE;
    for( int m=0; m<maximumNumberOfTries && changed ; m++ )
    {
      // prevent possible cycles by making sure the point is increasing in distance
      // from the initial point.
      int iDist = abs(i1-i10)+abs(i2-i20);
      if( iDist < m-3 )  // add -3 for safety, point could sometimes move a bit closer.
        break;

      if( m==warningStep )
	printf("findNearestCell:WARNING: the stencil walk is taking more than %i steps! x=(%e,%e) i=(%i,%i)\n",
              warningStep,x[0],x[1],i1,i2);
      
      changed=FALSE;
      // check all faces except the face that we may have crossed on the previous step
      for( int axis=0; axis<domainDimension; axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
          if( 1-side+2*axis == previousMove ) // note side -> (1-side)
            continue; // no need to check this face since we just came in this direction.

          iva[0]=iv[0]+ia[axis][side][0];
          iva[1]=iv[1]+ia[axis][side][1];

          ivb[0]=iv[0]+ib[axis][side][0];
          ivb[1]=iv[1]+ib[axis][side][1];
	  
          if( max(iva[0],ivb[0]) > indexRange(End,axis1) ||
              max(iva[1],ivb[1]) > indexRange(End,axis2) )
            continue;

	  // take the dot product of (x-A) with the normal to segment AB
	  //            x
	  //           /
	  //          /    
	  // --------A----------B-------o----
	  // 
    
	  real dot;
	  dot=( (x[1]-grid(ia1,ia2,i3,1))*(grid(ib1,ib2,i3,0)-grid(ia1,ia2,i3,0))-
		(x[0]-grid(ia1,ia2,i3,0))*(grid(ib1,ib2,i3,1)-grid(ia1,ia2,i3,1)) )*signForJacobian;

            
	  // printf("findNearestCell: ia=(%i,%i) ib=(%i,%i) side,axis=(%i,%i) dot=%e \n",ia1,ia2,ib1,ib2,side,axis,dot);
	  if( dot<0 )
	  {
	    iv[axis]+=2*side-1;
	    // printf("findNearestCell: changing to i=(%i,%i) \n",i1,i2);
	    if( iv[axis]<indexRange(Start,axis) || iv[axis]>=indexRange(End,axis) )
	    {
	      iv[axis]=max(iv[axis],indexRange(Start,axis));   // on the boundary
	      return 1; // point is outside
	    }
            previousMove=side+2*axis;
	    changed=TRUE;
	  }
	}
      }
    }
    
  }
  else if( domainDimension==3 )
  {
    // printf("findNearestCell 3D \n");

    // cell based stencil walk in 3D.

    // We specify two planes for each face

    //                left1 left2   right1 right2   bot1   bot2     top1   top2    back          front
    int ia[3][4][3]={0,0,0, 0,1,1,  1,0,0, 1,1,1,  0,0,0, 1,0,1,   0,1,0, 1,1,1,  0,0,0, 1,1,0,  0,0,1, 1,1,1 };
    int ib[3][4][3]={0,1,0, 0,0,1,  1,0,1, 1,1,0,  0,0,1, 1,0,0,   1,1,0, 0,1,1,  1,0,0, 0,1,0,  0,1,1, 1,0,1 };
    int ic[3][4][3]={0,0,1, 0,1,0,  1,1,0, 1,0,1,  1,0,0, 0,0,1,   0,1,1, 1,1,0,  0,1,0, 1,0,0,  1,0,1, 0,1,1 };

    int j1,j2,j3, k1,k2,k3, l1,l2,l3;
    real xx[3], a[3],b[3], n[3];

    if( i1<indexRange(Start,axis1) || i1>indexRange(End,axis1) ||
        i2<indexRange(Start,axis2) || i2>indexRange(End,axis2)  ||
        i3<indexRange(Start,axis3) || i3>indexRange(End,axis3) )
    {
      return 1;  // point is outside
    }

    bool changed=TRUE;
    for( int m=0; m<maximumNumberOfTries && changed ; m++ )
    {
      // prevent possible cycles by making sure the point is increasing in distance
      // from the initial point.
      int iDist = abs(i1-i10)+abs(i2-i20)+abs(i3-i30);
      if( iDist < m-3 )  // add -3 for safety, point could sometimes move a bit closer.
        break;
      
      if( m==warningStep )
	printf("findNearestCell:WARNING: the stencil walk is taking more than %i steps! x=(%e,%e,%e) i=(%i,%i,%i)\n",
              warningStep,x[0],x[1],x[2],i1,i2,i3);

      changed=FALSE;
      // check all faces
      for( int axis=0; axis<domainDimension; axis++ )
      {
	for( int bside=0; bside<=3; bside++ )  // 4 bside's per axis
	{
          int side=bside/2;
	  
          if( 1-side+2*axis == previousMove ) // note side -> (1-side)
            continue; // no need to check this face since we just came in this direction.

	  j1=iv[0]+ia[axis][bside][0];
	  j2=iv[1]+ia[axis][bside][1];
	  j3=iv[2]+ia[axis][bside][2];

	  k1=iv[0]+ib[axis][bside][0];
	  k2=iv[1]+ib[axis][bside][1];
	  k3=iv[2]+ib[axis][bside][2];

	  l1=iv[0]+ic[axis][bside][0];
	  l2=iv[1]+ic[axis][bside][1];
	  l3=iv[2]+ic[axis][bside][2];

          if( max(j1,k1,l1) > indexRange(End,axis1) ||
              max(j2,k2,l2) > indexRange(End,axis2) ||
              max(j3,k3,l3) > indexRange(End,axis3) )
            continue;

	  xx[0]=x[0]-grid(j1,j2,j3,0);
	  xx[1]=x[1]-grid(j1,j2,j3,1);
	  xx[2]=x[2]-grid(j1,j2,j3,2);

	  a[0]=grid(k1,k2,k3,0)-grid(j1,j2,j3,0);
	  a[1]=grid(k1,k2,k3,1)-grid(j1,j2,j3,1);
	  a[2]=grid(k1,k2,k3,2)-grid(j1,j2,j3,2);

	  b[0]=grid(l1,l2,l3,0)-grid(j1,j2,j3,0);
	  b[1]=grid(l1,l2,l3,1)-grid(j1,j2,j3,1);
	  b[2]=grid(l1,l2,l3,2)-grid(j1,j2,j3,2);

	  n[0]=a[1]*b[2]-a[2]*b[1];
	  n[1]=a[2]*b[0]-a[0]*b[2];
	  n[2]=a[0]*b[1]-a[1]*b[0];
	
	  real dot =signForJacobian*(xx[0]*n[0]+xx[1]*n[1]+xx[2]*n[2]);
	  // ** temp real dot =-signForJacobian*(xx[0]*n[0]+xx[1]*n[1]+xx[2]*n[2]);

	  // printf("findNearestCell: i=(%i,%i,%i) side,axis=(%i,%i) dot=%e jac=%f\n",iv[0],iv[1],iv[2],side,axis,dot,
          //     signForJacobian );
	  if( dot<0 )
	  {
	    iv[axis]+=2*side-1;
	    // printf("findNearestCell: changing to i=(%i,%i) \n",i1,i2);
	    if( iv[axis]<indexRange(Start,axis) || iv[axis]>=indexRange(End,axis) )
	    {
	      iv[axis]=max(iv[axis],indexRange(Start,axis));   // on the boundary
	      return 1; // point is outside
	    }
            previousMove=side+2*axis;
	    changed=TRUE;
	  }
	}
      }
    }

  }
  else
  {
    {throw "error";}
  }
  if( i1<indexRange(End,axis1) && i2<indexRange(End,axis2) && (domainDimension==2 || i3<indexRange(End,axis3)) )
  {
    if( Mapping::debug & 2 )
    {
      printf("findNearestCell: point x=(%7.1e,%7.1e,%7.1e) is INSIDE  cell i=(%i,%i,%i)\n",
	     x[0],x[1],x[2],iv[0],iv[1],iv[2]);
    }
    minimumDistance=0.;
    return 0; // point is inside the cell
  }
  else
    return 1; // point is outside
}


#define GET_NDIST(ia,ib,ic,distance) \
	j1=iv[0]+ia[axis][side][0]; \
	j2=iv[1]+ia[axis][side][1]; \
	j3=iv[2]+ia[axis][side][2]; \
 \
	k1=iv[0]+ib[axis][side][0]; \
	k2=iv[1]+ib[axis][side][1]; \
	k3=iv[2]+ib[axis][side][2]; \
 \
	l1=iv[0]+ic[axis][side][0]; \
	l2=iv[1]+ic[axis][side][1]; \
	l3=iv[2]+ic[axis][side][2]; \
 \
 \
	xx[0]=x[0]-GRID(j1,j2,j3,0); \
	xx[1]=x[1]-GRID(j1,j2,j3,1); \
	xx[2]=x[2]-GRID(j1,j2,j3,2); \
 \
	a[0]=GRID(k1,k2,k3,0)-GRID(j1,j2,j3,0); \
	a[1]=GRID(k1,k2,k3,1)-GRID(j1,j2,j3,1); \
	a[2]=GRID(k1,k2,k3,2)-GRID(j1,j2,j3,2); \
 \
	b[0]=GRID(l1,l2,l3,0)-GRID(j1,j2,j3,0); \
	b[1]=GRID(l1,l2,l3,1)-GRID(j1,j2,j3,1); \
	b[2]=GRID(l1,l2,l3,2)-GRID(j1,j2,j3,2); \
 \
        n[0]=a[1]*b[2]-a[2]*b[1]; \
        n[1]=a[2]*b[0]-a[0]*b[2]; \
        n[2]=a[0]*b[1]-a[1]*b[0]; \
	distance =-signForJacobian*(xx[0]*n[0]+xx[1]*n[1]+xx[2]*n[2]);


#define GET_TRIANGLE_COORDS(r,s)  \
  aDotA  =  a[0]*a[0]+ a[1]*a[1]+ a[2]*a[2];  \
  aDotB  =  a[0]*b[0]+ a[1]*b[1]+ a[2]*b[2];  \
  bDotB  =  b[0]*b[0]+ b[1]*b[1]+ b[2]*b[2];  \
  xxDotA = xx[0]*a[0]+xx[1]*a[1]+xx[2]*a[2];  \
  xxDotB = xx[0]*b[0]+xx[1]*b[1]+xx[2]*b[2];  \
    \
  detInverse = 1./max( REAL_MIN, aDotA*bDotB-aDotB*aDotB);  \
  r  = (xxDotA*bDotB-xxDotB*aDotB)*detInverse; \
  s  = (xxDotB*aDotA-xxDotA*aDotB)*detInverse;

int ApproximateGlobalInverse::
distanceToCell( real x[], int iv[], real & signedDistance, const real minimumDistance )
// ===============================================================================
// /Description:
//   Compute the distance-squared (L2 distance squared) to the boundary of the cell with lower left corner iv, but
//    only compute an accurate distance if it will be less than the current minimumDistance.
// /Method:
//    We are outside the cell if we are on the outside side of any face. If we are inside
// all faces then we are inside the cell. The only trick is to define the face in 3D since
// the four corners of the face may not be coplanar.
//
// /x (input) : point to check
// /iv (input) : cell to check (lower left corner).
// /signedDistance (output) : signed distance-squared to the cell boundary (positive=outside, negative=inside)
//    If negative then abs(signedDistance) is the distance to the boundary of the cell so that
//   small values of abs(signedDistance) means the point is close to the boundary.
// /minimumDistance (input)
// /Return value: 1 if inside, 0 is outside
// ===============================================================================
{
  assert( domainDimension==rangeDimension );

  signedDistance=-REAL_MAX*.5;
  real signForJacobian=map->getSignForJacobian();

  if( rangeDimension==2 )
  {
    const int gDim0=grid.getRawDataSize(0);
    const int gDim1=grid.getRawDataSize(1);
    const int gDim2=grid.getRawDataSize(2);
    real *gridp2 = grid.Array_Descriptor.Array_View_Pointer2;
#define GRID(i1,i2,i3,axis) gridp2[i1+gDim0*(i2+gDim1*(axis))]
      
    // assert( iv[0]<grid.getBound(0)-1indexRange(End,axis1) &&  iv[1]<indexRange(End,axis2) );
    // assert( iv[0]<grid.getBound(0) &&  iv[1]<grid.getBound(1) );
    if( iv[0]>=grid.getBound(0) || iv[1]>=grid.getBound(1) )
    {
      printF("ApproximateGlobalInverse::distanceToCell:ERROR: iv=[%i,%i] but grid has bounds [%i,%i]x[%i,%i]\n",
	     iv[0],iv[1],grid.getBase(0),grid.getBound(0),grid.getBase(1),grid.getBound(1));
      OV_ABORT("ERROR");
    }
    
 
    // traverse the cell in a counter-clockwise direction
    //          <- 3
    //        +------+
    //        |      |
    //     4  |      |2
    //        +------+
    //          1->   

    //  ia[0][0]=(0,0) --> ib[0][0] = (1,0)    // bottom
    //  ia[0][0]=(1,0) --> ib[0][0] = (1,1)    // right side
    //  ia[0][0]=(1,1) --> ib[0][0] = (0,1)    // top    
    //  ia[0][0]=(0,1) --> ib[0][0] = (0,0)    // left
    const int ia[2][2][2]={0,0, 1,0, 1,1, 0,1};  //
    const int ib[2][2][2]={1,0, 1,1, 0,1, 0,0};  //
    int j1,j2,k1,k2;
    int i3=iv[2];

    real xx[2],a[2],dot,distance;

    for( int axis=0; axis<=1; axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
        // face end pts are (j1,j2) ---> (k1,k2)
	j1=iv[0]+ia[axis][side][0];
	j2=iv[1]+ia[axis][side][1];

	k1=iv[0]+ib[axis][side][0];
	k2=iv[1]+ib[axis][side][1];


	xx[0]=x[0]-GRID(j1,j2,i3,0);
	xx[1]=x[1]-GRID(j1,j2,i3,1);
    
        // tangent is (a0,a1), outward normal is (a1,-a0)
	a[0]=GRID(k1,k2,i3,0)-GRID(j1,j2,i3,0);
	a[1]=GRID(k1,k2,i3,1)-GRID(j1,j2,i3,1);

    
	distance =signForJacobian*(xx[0]*a[1]-xx[1]*a[0]);
        if( distance>0. )
	{
	  // point is outside the cell -- the distance to the cell may be the distance to one of the vertices
          // and not the distance to the line defining the face.
   	  dot=xx[0]*a[0]+xx[1]*a[1];
          if( dot<0. )
            signedDistance=xx[0]*xx[0]+xx[1]*xx[1];
          else 
	  {
            real aNorm=a[0]*a[0]+a[1]*a[1];
	    
	    if( dot>aNorm )
	      signedDistance=SQR(x[0]-GRID(k1,k2,i3,0))+SQR(x[1]-GRID(k1,k2,i3,1));
	    else
	      signedDistance=distance*distance/max(REAL_MIN,aNorm);
	  }
          // printf("distanceToCell: pt x=(%7.1e,%7.1e) is outside cell i=(%i,%i),  signedDistance=%8.1e dot=%8.1e "
          //        " aNorm=%8.1e distance=%8.1e \n",
          //   x[0],x[1],iv[0],iv[1],signedDistance,dot,a[0]*a[0]+a[1]*a[1],distance);
	  // printf("distanceToCell: j=(%i,%i) k=(%i,%i) side=%i axis=%i \n",j1,j2,k1,k2,side,axis);
	  
	  return 0;
	}
	else
	{
          // choose the smallest distance in abs value.
	  signedDistance=max(signedDistance,-distance*distance/max(REAL_MIN,a[0]*a[0]+a[1]*a[1])); 
	}
      }
    }
  }
  else
  {
    const int gDim0=grid.getRawDataSize(0);
    const int gDim1=grid.getRawDataSize(1);
    const int gDim2=grid.getRawDataSize(2);
    real *gridp3 = grid.Array_Descriptor.Array_View_Pointer3;
#undef GRID
#define GRID(i1,i2,i3,axis) gridp3[i1+gDim0*(i2+gDim1*(i3+gDim2*(axis)))]


    // ia,ib,ic : 3 points to define a triangle on a face
    //                      left  right   bot    top    front back 
    const int ia[3][2][3]={0,0,0, 1,0,0, 0,0,0, 0,1,0, 0,0,0, 0,0,1};  //
    const int ib[3][2][3]={0,1,0, 1,0,1, 0,0,1, 1,1,0, 1,0,0, 0,1,1};  //
    const int ic[3][2][3]={0,0,1, 1,1,0, 1,0,0, 0,1,1, 0,1,0, 1,0,1};  //

    // ia2,ib2,ic2 : here is the 2nd triangle on the face
    //                      left  right   bot    top    front back 
    const int ia2[3][2][3]={0,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,0, 1,1,1};  //
    const int ib2[3][2][3]={0,0,1, 1,1,0, 1,0,0, 0,1,1, 0,1,0, 1,0,1};  //
    const int ic2[3][2][3]={0,1,0, 1,0,1, 0,0,1, 1,1,0, 1,0,0, 0,1,1};  //


    int j1,j2,j3, k1,k2,k3, l1,l2,l3;

    real xx[3], a[3],b[3], n[3], y[3], distance;
    real r,s,aDotA,aDotB,bDotB,xxDotA,xxDotB,detInverse;
    
    for( int axis=0; axis<=2; axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
        GET_NDIST(ia,ib,ic,distance);

	// solve for the triangle coordinates
	//      xx - n.xx n  = r*a + s*b 
	// to obtain the closest pt on the plane
	GET_TRIANGLE_COORDS(r,s);
        if( distance>0. )
	{
	  // point is outside this plane -- the distance to the cell may be the distance to one of the edges
          // and not the distance to the plane defining the face.
	  

          if( r+s >1. )
	  {
	    // printf("distCell: (s,a)=(%i,%i) outside but r+s=%8.2e ..check other triangle\n",side,axis,r+s);
	    
	    // projected point is outside the diagonal. We need to check the other triangle on this face
            GET_NDIST(ia2,ib2,ic2,distance);

	    if( distance>0. )
	    {
              // point is outside both planes
              GET_TRIANGLE_COORDS(r,s);
   	      // printf("distCell: (s,a)=(%i,%i) 2nd triangle r+s=%8.2e\n",side,axis,r+s);

//                if( r+s >1.01 )
//  	      {
//  		printf("distanceToCell:ERROR: point is outside diagonal in both directions! Something wrong here\n");
//                  printf("   : r+s=%8.2e \n",r+s);
//  	      }
	      
	    }
	    else
	    {
	      // point is inside t
	      // choose the smallest distance in abs value.
	      signedDistance=max(signedDistance,-distance*distance/max(REAL_MIN,n[0]*n[0]+n[1]*n[1]+n[2]*n[2])); 
              // printf("distCell: (s,a)=(%i,%i) inside 2nd triangle signedDistance=%8.1e\n",
              //      side,axis,signedDistance);

              continue;
	    }
	  }
	  
	  r  = max(0.,min(1., r));  /* restrict to [0,1] - the approximate face */ 
	  s  = max(0.,min(1., s));
	  if( r+s > 1. )
	  {
	    r/=(r+s);  s/=(r+s);
	  }
	  y[0]=xx[0]-(r*a[0]+s*b[0]);
	  y[1]=xx[1]-(r*a[1]+s*b[1]);
	  y[2]=xx[2]-(r*a[2]+s*b[2]);
	  
          signedDistance=y[0]*y[0]+y[1]*y[1]+y[2]*y[2];

	  // printf("distCell: (s,a)=(%i,%i) pt x=(%7.1e,%7.1e,%7.1e) is outside cell i=(%i,%i,%i),"
          //       " signedDistance=%8.1e "
	  // " distance=%8.1e \n",side,axis,x[0],x[1],x[2],iv[0],iv[1],iv[2],signedDistance,distance);
	  // printf("        : j=(%i,%i,%i) k=(%i,%i,%i) l=(%i,%i,%i) \n",
	  //   j1,j2,j3,k1,k2,k3,l1,l2,l3);
	  
	  return 0;
	}
	else
	{
	  // point is inside this plane.
	  if( r+s >1. )
	  {
	    // printf("distCell: (s,a)=(%i,%i) inside 1st tri but r+s=%8.2e ..check other triangle\n",
            //            side,axis,r+s);

	    // projected point is outside the diagonal. We need to check the other triangle on this face
            GET_NDIST(ia2,ib2,ic2,distance);

	    if( distance<=0. )
	    {
              // inside 2nd tri
	    }
	    else
	    {
	      // outside 2nd tri
              GET_TRIANGLE_COORDS(r,s);

	      r  = max(0.,min(1., r));  /* restrict to [0,1] - the approximate face */ 
	      s  = max(0.,min(1., s));
	      if( r+s > 1. )
	      {
		r/=(r+s);  s/=(r+s);
	      }

	      y[0]=xx[0]-(r*a[0]+s*b[0]);
	      y[1]=xx[1]-(r*a[1]+s*b[1]);
	      y[2]=xx[2]-(r*a[2]+s*b[2]);
	  
	      signedDistance=y[0]*y[0]+y[1]*y[1]+y[2]*y[2];
	      // printf("distCell: (s,a)=(%i,%i) OUTSIDE 2nd tri\n",side,axis);
	      // printf("distCell: (s,a)=(%i,%i) pt x=(%7.1e,%7.1e,%7.1e) is outside cell i=(%i,%i,%i),"
		//      " signedDistance=%8.1e "
		//      " distance=%8.1e \n",side,axis,x[0],x[1],x[2],iv[0],iv[1],iv[2],signedDistance,distance);
	      // printf("        : j=(%i,%i,%i) k=(%i,%i,%i) l=(%i,%i,%i) \n",
		//      j1,j2,j3,k1,k2,k3,l1,l2,l3);
	  
	      return 0;
	    }
	  }
	    // choose the smallest distance in abs value.
	  signedDistance=max(signedDistance,-distance*distance/max(REAL_MIN,n[0]*n[0]+n[1]*n[1]+n[2]*n[2])); 
          // printf("distCell: (s,a)=(%i,%i) inside first triangle signedDistance=%8.1e\n",side,axis,signedDistance);
	}
      }
    }
  }
  if( Mapping::debug & 2 )
  {
    printf("distanceToCell: x=(%7.1e,%7.1e,%7.1e) is INSIDE cell i=(%i,%i,%i)," 
              "signedDistance=%8.1e\n",
	   x[0],x[1],x[2],iv[0],iv[1],iv[2],signedDistance);
  }
  
  return 1;   // we must be inside
}

#undef GRID


int ApproximateGlobalInverse::
insideGrid( int side, int axis, real x[], int iv[], real & signedDistance )
// ===============================================================================
// /side,axis (input) : 
// /signedDistance (output) : return the approximate (signed) distance from the point x to the 
//  boundary (inside if >=0 , outside if <=0 ).
// /Return value: true if "inside"
// ===============================================================================
{
  int i1=iv[0], i2=iv[1], i3=iv[2];

  real signForJacobian=map->getSignForJacobian();
  if( domainDimension==2 )
  {
    int i1p= axis==0 ? i1 : i1<indexRange(End,axis1) ? i1+1 : i1-1 ;
    int i2p= axis==1 ? i2 : i2<indexRange(End,axis2) ? i2+1 : i2-1 ;
    
    // take the dot product of (x-A) with the normal to segment AB
    //            x
    //           /
    //          /    
    // --------A----------B-------o----
    // 
    real nn[2];
    nn[0]=grid(i1,i2,i3,1)-grid(i1p,i2p,i3,1);
    nn[1]=grid(i1p,i2p,i3,0)-grid(i1,i2,i3,0);
    
    signedDistance=(x[0]-grid(i1,i2,i3,0))*nn[0]+(x[1]-grid(i1,i2,i3,1))*nn[1];
    
    if( i1p<i1 || i2p<i2 )
      signedDistance*=(2*side-1)*signForJacobian/max(REAL_MIN,SQRT( nn[0]*nn[0]+nn[1]*nn[1] ));
    else
      signedDistance*=(1-2*side)*signForJacobian/max(REAL_MIN,SQRT( nn[0]*nn[0]+nn[1]*nn[1] ));

    // printf("insideGrid: x=(%e,%e) i=(%i,%i) signedDistance=%e signForJacobian=%6.1e\n",x[0],x[1],i1,i2,signedDistance,signForJacobian);
    
  }
  else
  {
    int i1p=i1, i2p=i2;  // , i3p=i3;
    int j1p=i1, j2p=i2, j3p=i3;
    
    if( axis==0 )
    {
      i2p=i2<indexRange(End,axis2) ? i2+1 : i2-1 ;
      j3p=i3<indexRange(End,axis3) ? i3+1 : i3-1 ;
      signForJacobian*=(i2p-i2)*(j3p-i3);
    }
    else if( axis==1 )
    {
      i1p=i1<indexRange(End,axis1) ? i1+1 : i1-1 ;
      j3p=i3<indexRange(End,axis3) ? i3+1 : i3-1 ;
      signForJacobian*=(i1p-i1)*(j3p-i3);
    }
    else
    {
      i1p=i1<indexRange(End,axis1) ? i1+1 : i1-1 ;
      j2p=i2<indexRange(End,axis2) ? i2+1 : i2-1 ;
      signForJacobian*=(i1p-i1)*(j2p-i2);
    }
    

    // take the dot product of (x-A) with the normal to the point A
    //            x
    //           /
    //          /    
    // --------A--------
    // 
    real nn[3];
    nn[0]=(grid(i1p,i2p,i3,1)-grid(i1,i2,i3,2))*(grid(j1p,j2p,j3p,2)-grid(i1,i2,i3,1));
    nn[1]=(grid(i1p,i2p,i3,2)-grid(i1,i2,i3,0))*(grid(j1p,j2p,j3p,0)-grid(i1,i2,i3,2));
    nn[2]=(grid(i1p,i2p,i3,0)-grid(i1,i2,i3,1))*(grid(j1p,j2p,j3p,1)-grid(i1,i2,i3,0));
    
    signedDistance=(x[0]-grid(i1,i2,i3,0))*nn[0]+(x[1]-grid(i1,i2,i3,1))*nn[1]+(x[2]-grid(i1,i2,i3,2))*nn[2];
    
    signedDistance*=(1-2*side)*signForJacobian/max(REAL_MIN,SQRT( nn[0]*nn[0]+nn[1]*nn[1]+nn[2]*nn[2] ));
  
  }
  

  return signedDistance>=0.;
}




void ApproximateGlobalInverse::
initializeStencilWalk()
//==============================================================================
// /Description:
//   Compute the search direction arrays for 2 and 3D local searches
//
//    
//  Only do this once for the Class
//==============================================================================
{
  
  int dir;
  if( domainDimension==2 && initializeStencilWalk2D )
  {
    initializeStencilWalk2D=FALSE;
    
//     numberOfStencilDirections2D.redim(3,3);
//     numberOfStencilDirections2D.setBase(-1);

//     stencilDirection2D1.redim(8,3,3);   
//     stencilDirection2D1.setBase(-1); 
//     stencilDirection2D1.setBase(0,0); 
//     stencilDirection2D1=0;

//     stencilDirection2D2.redim(8,3,3);   
//     stencilDirection2D2.setBase(-1); 
//     stencilDirection2D2.setBase(0,0); 
//     stencilDirection2D2=0;

    // (id1,id2) : direction of the last step in the stencil walk, id1=-1,0,+1
    for( int id2=-1; id2<=1; id2++ )
    for( int id1=-1; id1<=1; id1++ )
    {
      for( dir=0; dir<8; dir++ )
      {
        stencilDirection2D1(dir,id1,id2)=0;
        stencilDirection2D2(dir,id1,id2)=0;
      }
      
      dir=0;
      for( int i2=-1; i2<=1; i2++ )
      for( int i1=-1; i1<=1; i1++ )
      {
        // if id1==0 and id2==0 check all directions (starting point)
        // if id1=-1: don't check directions with i1==0 or i1=+1
        // if id1=+1: don't check directions with i1==0 or i1=-1
        //     +-X-X   X-X-+
        //     +-X-X   X-X-+
        //     +-X-X   X-X-+
        //    id1=-1  id1=+1
        if( ( !(
         ((id1==-1 && (i1==0 || i1==+1)) || id1==0 || (id1==+1 && (i1==0 || i1==-1))) && 
         ((id2==-1 && (i2==0 || i2==+1)) || id2==0 || (id2==+1 && (i2==0 || i2==-1)))) ||
          (id1==0 && id2==0) )
                  && (i1 != 0 || i2 != 0 ) )
        {
          stencilDirection2D1(dir,id1,id2)=i1;
          stencilDirection2D2(dir,id1,id2)=i2;
          dir++;
        }
      }
      numberOfStencilDirections2D(id1,id2)=dir;

    }
//     if( Mapping::debug & 32 )
//     {
//       numberOfStencilDirections2D.display("numberOfStencilDirections2D:");
//       stencilDirection2D1.display("stencilDirection2D1:");
//       stencilDirection2D2.display("stencilDirection2D2:");
//     }
  }
  else if( domainDimension==3 && initializeStencilWalk3D )
  {
    initializeStencilWalk3D=FALSE;
    
//     numberOfStencilDirections3D.redim(3,3,3);
//     numberOfStencilDirections3D.setBase(-1);

//     stencilDirection3D1.redim(27,3,3,3);   
//     stencilDirection3D1.setBase(-1); 
//     stencilDirection3D1.setBase(0,0); 
//     stencilDirection3D1=0;

//     stencilDirection3D2.redim(27,3,3,3);   
//     stencilDirection3D2.setBase(-1); 
//     stencilDirection3D2.setBase(0,0); 
//     stencilDirection3D2=0;

//     stencilDirection3D3.redim(27,3,3,3);   
//     stencilDirection3D3.setBase(-1); 
//     stencilDirection3D3.setBase(0,0); 
//     stencilDirection3D3=0;
    
    for( int id3=-1; id3<=1; id3++ )
    for( int id2=-1; id2<=1; id2++ )
    for( int id1=-1; id1<=1; id1++ )
    {
      for( dir=0; dir<27; dir++ )
      {
        stencilDirection3D1(dir,id1,id2,id3)=0;
        stencilDirection3D2(dir,id1,id2,id3)=0;
        stencilDirection3D3(dir,id1,id2,id3)=0;
      }

      dir=0;
      for( int i3=-1; i3<=1; i3++ )
      for( int i2=-1; i2<=1; i2++ )
      for( int i1=-1; i1<=1; i1++ )
      {
        if( ( !(
         ((id1==-1 && (i1==0 || i1==+1)) || id1==0 || (id1==+1 && (i1==0 || i1==-1))) && 
         ((id2==-1 && (i2==0 || i2==+1)) || id2==0 || (id2==+1 && (i2==0 || i2==-1))) && 
         ((id3==-1 && (i3==0 || i3==+1)) || id3==0 || (id3==+1 && (i3==0 || i3==-1)))) ||
          (id1==0 && id2==0 && id3==0) )
               && (i1 != 0 || i2 != 0 || i3 != 0) )
        {
          stencilDirection3D1(dir,id1,id2,id3)=i1;
          stencilDirection3D2(dir,id1,id2,id3)=i2;
          stencilDirection3D3(dir,id1,id2,id3)=i3;
          dir++;
        }
      }
      numberOfStencilDirections3D(id1,id2,id3)=dir;
    }
//     if( Mapping::debug & 32 )
//     {
//       numberOfStencilDirections3D.display("numberOfStencilDirections3D:");
//       stencilDirection3D1.display("stencilDirection3D1:");
//       stencilDirection3D2.display("stencilDirection3D2:");
//       stencilDirection3D3.display("stencilDirection3D3:");
//     }
  }
}







void ApproximateGlobalInverse::
countCrossingsWithPolygon(const RealArray & x, 
                          IntegerArray & crossings,
                          const int & side_ /* =Start */, 
                          const int & axis_ /* =axis1 */,
                          RealArray & xCross /* = Overture::nullRealDistributedArray() */,
                          const IntegerArray & mask /* = Overture::nullIntArray() */,
                          const unsigned int & maskBit /* = UINT_MAX */,
                          const int & maskRatio1  /* =1 */ ,
                          const int & maskRatio2  /* =1 */ ,
                          const int & maskRatio3  /* =1 */ )
//==========================================================================================
/// \details 
///    Count the number of times that the ray starting from position xv=(x,y) and extending
///   to y=+ infinity, crosses the polygon approximation to the curve (domainDimension<=1)
///   or the triangulated approximation to the face of the mapping (domainDimension==3).
///  
/// 
/// \param x(I,0:r-1) (input):  set of points to check
/// \param crossings(I) (input/ouput): number of crossings for each point. **NOTE** this function will
///    add on to the current values in this array, thus you should set this to zero on the first call,
///    or subsequent calls, depending on your application.
/// \param side,axis_ (input): For domainDimension>1 these will indicate the side (domainDimension==2)
///     or the face (domainDimension==3) to check. For domainDimension==1 these values are ignored.
/// \param xCross (input/output) : If this argument is supplied then we store each crossing point in ths
///      array as xCross(i,0:2r,cross) where cross=0,1,...,crossings(i)-1, r=rangeDimension.
///      We save [x,y,i1,i2] if rangeDimension==2 and [x,y,z,i1,i2,i3] if rangeDimension==3.
///      (i1,i2,i3) denotes the lower left corner of the cell that holds the intersection.
/// \param mask (input): optional arg that is used to mask out certain parts of the boundary. If this
///    arg is given then ALL corners of a cell must have "mask(i1,i2,i3) \& maskBit" in order
///    that a ray crossing that cell to count as an actual crossing. In other words the
///    valid points on the boundary are marked with "mask(i1,i2,i3) \& maskBit".
/// \param maskBit (input) :  by default the mask bit is UINT_MAX == $2^m$ -1 (all bits on) so that invalid
///     points would have mask(i1,i2,i3)==0  
/// \param maskRatio1, maskratio2,maskRatio3 (input) : parameters from multigrid. These are the ratios of the
///     current grid spacing to the finest grid spacing. (assuming that the grid associated with this
///     mapping is the finest grid!).
/// \param Return value : number of times the ray crosses the polygon. For a closed curve there will
///    be an odd number of crossings if the point is inside the polygon and and even number
///  of crossings if the point is outside the polygon.
/// 
/// \param NOTE: If a point lies exactly on a vertical line segment then this routine will give 
///    zero crossings for this segment (it may cross other segments in which case the crossing
///    count may be non-zero)
//===========================================================================================
{
  if( uninitialized )
    initialize();      // this will initialize the bounding boxes
  
  const int axis=domainDimension==1 ? axis1 : axis_;
  const int side=domainDimension==1 ? Start : side_;
    

  assert( side>=0 && side<=1 );
  assert( axis>=0 && axis<domainDimension );
  
  bool saveCrossings = xCross.getLength(0) > 0;  // if TRUE then we save the crossing points
  bool maskPoints = mask.getLength(0) > 0;
  bool doNotUseMaskRatio = maskRatio1==1 && maskRatio2==1 && maskRatio3==1;
/* ----
  if( !doNotUseMaskRatio )
    printf("countCrossingsWithPolygon: maskRatio1=%i, maskRatio2=%i \n",maskRatio1,maskRatio2);

  if( maskPoints )
  {
    // The grid and mask array should be the same dimensions!
    if( mask.dimension(0)!=grid.dimension(0) || 
        mask.dimension(1)!=grid.dimension(1) ||
        mask.dimension(2)!=grid.dimension(2) )
    {
      // with multigrid the mask may be only a subset of the points that are in the grid array
      maskIsTheSameSize=FALSE;
      maskRatio1=(grid.getBound(0)-grid.getBase(0))/max(1,mask.getBound(0)-mask.getBase(0));
      maskRatio2=(grid.getBound(1)-grid.getBase(1))/max(1,mask.getBound(1)-mask.getBase(1));
      maskRatio3=(grid.getBound(2)-grid.getBase(2))/max(1,mask.getBound(2)-mask.getBase(2));
      printf("countCrossingsWithPolygon: maskRatio1=%i, maskRatio2=%i \n",maskRatio1,maskRatio2);
      // printf("countCrossingsWithPolygon:ERROR the mask and grid arrays are different dimensions\n");
      // throw "error";
    }
  }
------ */  
  BoundingBoxStack boxStack;

  int cross;
  real epsX=(boundingBox(End,axis1)-boundingBox(Start,axis1))*REAL_EPSILON*10.;

  real gx0,gx1;

  if( (domainDimension==1 && rangeDimension==2) || (domainDimension==2 && rangeDimension==2) )
  {

    int i3=indexRange(Start,axis3);  // default value

    int is1 = domainDimension==1 ? 1 : (axis==axis1 ? 0 : 1 );
    int is2 = domainDimension==1 ? 0 : (axis==axis2 ? 0 : 1 );
    
    for( int i=x.getBase(0); i<=x.getBound(0); i++ )
    {
      cross=crossings(i);
      real x0=x(i,0), x1=x(i,1);

      boxStack.push( boundingBoxTree[side][axis] );  
      while( !boxStack.isEmpty() )
      {
	numberOfBoxesChecked++;
	BoundingBox & box0=boxStack.pop();             // get a box off the stack

        if( x1 <= box0.rangeBound(End,axis2) &&  
	    x0 >= box0.rangeBound(Start,axis1) && x0 <= box0.rangeBound(End,axis1) )
	{ // check this box for crossings
	  if( box0.child1!=NULL )
	  { // push children onto the stack
	    boxStack.push( *box0.child1 );
	    boxStack.push( *box0.child2 );
	  }
	  else
	  {
            const int i2Start= domainDimension==1 ? indexRange(Start,axis2) : box0.domainBound(Start,axis2)+is2;
            const int i2End  = domainDimension==1 ? indexRange(End  ,axis2) : box0.domainBound(End,axis2);
	    
            const int i1Start= box0.domainBound(Start,axis1)+is1;
            const int i1End  = box0.domainBound(End  ,axis1);
	    for( int i2=i2Start; i2<=i2End; i2++ )
	    for( int i1=i1Start; i1<=i1End; i1++ )
	    {
              // count crossings with all line segments in this box:
	      if( (x0-grid(i1-is1,i2-is2,i3,0))*(x0-grid(i1,i2,i3,0))<= 0. )
//                 &&    (!maskPoints || 
//                       ( (mask(i1-is1,i2-is2,i3) & maskBit) && (mask(i1,i2,i3) & maskBit) ) ) )
	      { 
                if( maskPoints )
		{
		  if( doNotUseMaskRatio )
		  {
                    if( !(mask(i1-is1,i2-is2,i3) & maskBit) || !(mask(i1,i2,i3) & maskBit) )
                      continue;  // skip this crossing
		  }
		  else
		  { 
                    // with multigrid the mask may be only a subset of the points that are in the grid array
		    int j1=(i1+maskRatio1-1)/maskRatio1;   // closest point in the mask >= (i1,i2)
		    int j2=(i2+maskRatio2-1)/maskRatio2;
		    if( !(mask(j1-is1,j2-is2,i3) & maskBit) || !(mask(j1,j2,i3) & maskBit) )
                      continue; // skip this crossing
		  }
		}
                // the vertical line through x0 is in between the end points of the line segment.
                //  now check if the point is really above or below the line segment
                //
                //       O | 
                //         \
                //         | \
                //         |   \
                //         |     O 
                //         x0

                gx0=grid(i1-is1,i2-is2,i3,0);
                gx1=grid(i1    ,i2,i3,0);
                // check to see if we line up with an end point of the segment, x0=g0 or x0=g1
                // perturb the points to remove this degeneracy
                if( x0==gx0 )
                  gx0-= epsX;
                if( x0==gx1 )
                  gx1-= epsX;
                // now x0!=gx0 and x0!=gx1
		if( (x0-gx0)*(x0-gx1)< 0. ) // do we still cross?
		{
		  if(  ( grid(i1-is1,i2-is2,i3,1) > x1 && grid(i1,i2,i3,1) > x1 )       // line segment is above
		       ||          
		       (  
			 !(grid(i1-is1,i2-is2,i3,1) < x1 && grid(i1,i2,i3,1) < x1 )    // line seg is not below
			 &&
			 ( ((x0-gx0)*grid(i1  ,i2,i3,1)
			    +(gx1-x0)*grid(i1-is1,i2-is2,i3,1))/(gx1-gx0) > x1)  ) ) // point is close, check carefully
                 
		  {
                    // we have a crossing
                    if( saveCrossings )
		    {
                      if( xCross.getLength(2) < cross )
		      {
			// allocate more space for crossings
                        printf("countCrossingsWithPolygon:INFO:Too many crossings: allocating more space\n");
                        xCross.resize(xCross.dimension(0),xCross.dimension(1),xCross.getLength(2)*2);
		      }
		      xCross(i,0,cross)=x0;   // line is vertical
                      assert( gx1-gx0 !=0. );
		      xCross(i,1,cross)=grid(i1-is1,i2-is2,i3,1)
                        +(grid(i1,i2,i3,1)-grid(i1-is1,i2-is2,i3,1))*(x0-gx0)/(gx1-gx0);
                      xCross(i,2,cross)=i1-is1;
                      xCross(i,3,cross)=i2-is2;
		      
		    }
		    cross++;
		    // printf("countCrossings: cross=%i i=%i, x0=%e,x(i-1)=%e,x(i)=%e, x1=%e \n",cross,i1,x0,
		    //   grid(i1-1,i2,i3,0),grid(i1,i2,i3,0),x1);
		  }
		}
	      }
	    }
	  }
	}
      }
      crossings(i)=cross;
    }
  }
  else if( domainDimension==3 && rangeDimension==3 )
  {
    // first do a global check on all the points
    Range I(x.getBase(0),x.getBound(0));
    real xMin[3], xMax[3];
    for( int dir=0; dir<rangeDimension; dir++ )
    {
      xMin[dir]=min(x(I,dir));
      xMax[dir]=max(x(I,dir));
    }
    if( xMin[1] > boundingBox(End,axis2) ||
	xMax[0] < boundingBox(Start,axis1) || xMin[0] > boundingBox(End,axis1) ||
	xMax[2] < boundingBox(Start,axis3) || xMin[2] > boundingBox(End,axis3) )
    {
      return;
    }


    // In 3D we always save the points of crossing
    RealArray xCrossLocal;
    if( !saveCrossings )
      xCrossLocal.redim(x.dimension(0),3,5);   // save intersections here if the user doesn't want them

    RealArray & xx = saveCrossings ? xCross : xCrossLocal;
    
    real xv[3], xi[3];
    real & x0 = xv[0];
    real & x1 = xv[1];
    real & x2 = xv[2];
    int is1 = axis==axis1 ? 0 : 1;
    int is2 = axis==axis2 ? 0 : 1;
    int is3 = axis==axis3 ? 0 : 1;

    const real epsEdge = REAL_EPSILON*100.;  // 1.e-4;   // double check points that are this close to the boundary of a triangle,
                                  // the distance is in normalized triangle coordinates

    const real epsY=(boundingBox(End,axis2)-boundingBox(Start,axis2))*REAL_EPSILON*10.;

    real epsX=(boundingBox(End,axis1)-boundingBox(Start,axis1))*epsEdge;
    real epsZ=(boundingBox(End,axis3)-boundingBox(Start,axis3))*epsEdge;
    epsZ*=1.23456789; // add a random perturbation so the virtual perurbation is unlikely to
                      // shift along the direction of a grid line
    

    Triangle tri;
    bool virtualPerturbation=FALSE;
    for( int i=x.getBase(0); i<=x.getBound(0); i++ )
    {
      redo:
      cross=crossings(i);
      int initialCross=cross;  // the list may already contain crossing info from previous calls, keep the
                               // initial value for this "i"
      x0=x(i,0), x1=x(i,1), x2=x(i,2);

      // use bounding boxes for the given face
      boxStack.push( boundingBoxTree[side][axis] );  
      while( !boxStack.isEmpty() )
      {
	numberOfBoxesChecked++;
	BoundingBox & box0=boxStack.pop();             // get a box off the stack

        // really can shift by +epsX if we set these to zero when not perturbing
        if( x1 <= box0.rangeBound(End,axis2) &&
	    x0 >= box0.rangeBound(Start,axis1)-epsX && x0 <= box0.rangeBound(End,axis1)+epsX &&
	    x2 >= box0.rangeBound(Start,axis3)-epsZ && x2 <= box0.rangeBound(End,axis3)+epsZ )
	{ // check this box for crossings
	  if( box0.child1!=NULL )
	  { // push children onto the stack
	    boxStack.push( *box0.child1 );
	    boxStack.push( *box0.child2 );
	  }
	  else
	  {
            // Check the grid cell faces in this box for intersections
	    for( int i3=box0.domainBound(Start,axis3); i3<=box0.domainBound(End,axis3)-is3; i3++ )
	    for( int i2=box0.domainBound(Start,axis2); i2<=box0.domainBound(End,axis2)-is2; i2++ )
	    for( int i1=box0.domainBound(Start,axis1); i1<=box0.domainBound(End,axis1)-is1; i1++ )
	    {
              for( int dir=0; dir<3; dir++ )    // *** could do better here **** check as you go
	      {
                xMin[dir]=min(grid(i1,i2,i3,dir),grid(i1+is1,i2,i3,dir),grid(i1,i2+is2,i3,dir),
                            grid(i1,i2,i3+is3,dir),grid(i1+is1,i2+is2,i3+is3,dir));
                xMax[dir]=max(grid(i1,i2,i3,dir),grid(i1+is1,i2,i3,dir),grid(i1,i2+is2,i3,dir),
			      grid(i1,i2,i3+is3,dir),grid(i1+is1,i2+is2,i3+is3,dir));
	      }
	      
	      
              if( x1 <= xMax[1] && x0>=xMin[0]-epsX && x0<=xMax[0]+epsX && x2>=xMin[2]-epsZ && x2<=xMax[2]+epsZ )
//                && ( !maskPoints || 
//                   ((mask(i1,i2,i3)&maskBit) && (mask(i1+is1,i2,i3)&maskBit) && (mask(i1,i2+is2,i3)&maskBit) &&
//                    (mask(i1,i2,i3+is3)&maskBit) && (mask(i1+is1,i2+is2,i3+is3)&maskBit) ) ) )
	      {
                if( maskPoints )
		{
		  if( doNotUseMaskRatio )
		  {
                    if( !(mask(i1    ,i2    ,i3    )&maskBit) || !(mask(i1+is1,i2,i3    )&maskBit) ||
                        !(mask(i1    ,i2+is2,i3    )&maskBit) || !(mask(i1    ,i2,i3+is3)&maskBit) ||
                        !(mask(i1+is1,i2+is2,i3+is3)&maskBit) )
                      continue;  // skip this crossing
		  }
		  else
		  { 
                    // with multigrid the mask may be only be a subset of the points that are in the grid array
		    int j1=(i1+maskRatio1-1)/maskRatio1;   // closest point in the mask >= (i1,i2.i3)
		    int j2=(i2+maskRatio2-1)/maskRatio2;
		    int j3=(i3+maskRatio3-1)/maskRatio3;
                    if( !(mask(j1    ,j2    ,j3    )&maskBit) || !(mask(j1+is1,j2,j3    )&maskBit) ||
                        !(mask(j1    ,j2+is2,j3    )&maskBit) || !(mask(j1    ,j2,j3+is3)&maskBit) ||
                        !(mask(j1+is1,j2+is2,j3+is3)&maskBit) )
                      continue;  // skip this crossing
		  }
		}

		// form 2 triangles from the face of the grid cell       
                for( int j=0; j<=1; j++ )
		{
                  bool intersectionFound=FALSE;

		  tri.setVertices(grid,i1,i2,i3,j,axis );
                  if( virtualPerturbation )
		  {
		    tri.x1[0]+=epsX;  tri.x1[2]+=epsZ;
		    tri.x2[0]+=epsX;  tri.x2[2]+=epsZ;
		    tri.x3[0]+=epsX;  tri.x3[2]+=epsZ;
		  }
		  
                  if( Mapping::debug & 4 )
                    tri.display("check this triangle");
		  // find any crossings with the triangles
                  // xi[0]=-999.; xi[1]=-999.; xi[2]=-999.;
		  
                  int intersection=tri.intersects(xv,xi);
		  if( intersection )   // xi[3] holds the point of intersection
		  {
                    if( Mapping::debug & 4 )
                      printf(" intersects! cross=%i, xi=(%e,%e,%e)\n",cross,xi[0],xi[1],xi[2]);
		    
                    if( intersection<=1 )
		    { // non-degenerate intersection
		      intersectionFound=TRUE;  // remove multiples later
		    }
                    else
		    { // degenerate intersection, we need to redo with a perturbed geometry : 
                      
                      if( !virtualPerturbation )
		      {
                        if( Mapping::debug & 4 )
                          printf("REDO point %i with a virtual perturbation, epsX=%e, epsZ=%e\n",i,epsX,epsZ);
                        // go back and redo this point with a perturbed geometry
			virtualPerturbation=TRUE;
			while( !boxStack.isEmpty() )
			{
                          boxStack.pop(); 
			}
                        goto redo;
		      }
		      else if( virtualPerturbation )
		      {
                        {throw "error";}
		      }
		      else 
		      {
			real alpha1,alpha2;
			int ok = tri.getRelativeCoordinates(xi,alpha1,alpha2);   // get "triangle" parameter coordinates
			if( ok!=0 )
			  printf(" ERROR from getRelativeCoordinates, x=(%e,%e,%e), xi=(%e,%e,%e) \n",
				 x0,x1,x2,xi[0],xi[1],xi[2]);
			
			if( fabs(alpha2)<epsEdge || fabs(alpha1)<epsEdge || fabs(alpha1+alpha2-1.)<epsEdge )
			{
			  // we are on the boundary of a triangle, perturb the triangle and recheck.
			  // Note that all triangles are perturbed in the same way so that if the ray
			  // hits the intersection of two triangles both triangles will be perturbed. **check this **

			  // ** potential problem: what if the other possible intersection is not detected? ***

			  if( Mapping::debug & 4 )
			  {
			    printf("INFO:countCrossings: intersection of the edge of a triangle"
				   " - perturbing to recheck\n");
			    printf(" epsX=%e, epsZ=%e, alpha1=%e, alpha2=%e, alpha1+alpha2=%e \n",epsX,epsZ,
				   alpha1,alpha2,alpha1+alpha2);
			  }
			  tri.x1[0]+=epsX;  tri.x1[2]+=epsZ;
			  tri.x2[0]+=epsX;  tri.x2[2]+=epsZ;
			  tri.x3[0]+=epsX;  tri.x3[2]+=epsZ;
			  if( tri.intersects(xv,xi) )
			  {
			    tri.getRelativeCoordinates(xi,alpha1,alpha2);
			    if( fabs(alpha1)<epsEdge || fabs(alpha2)<epsEdge || fabs(alpha1+alpha2-1.)<epsEdge )
			    {
			      printf("ERROR:countCrossings: still on an edge after perturbing!. ****** \n");
			      printf(" epsX=%e, epsZ=%e, alpha1=%e, alpha2=%e, alpha1+alpha2=%e \n",epsX,epsZ,
				     alpha1,alpha2,alpha1+alpha2);
			    
			      printf(" ****** For now count the intersection, fix this case Bill ! ****** \n");
			      intersectionFound=TRUE;
			    }
			    else
			    {
			      if( Mapping::debug & 4 )
				printf("INFO:countCrossings: intersection found after perturbing\n");
			      intersectionFound=TRUE;
			    }
			  }
			  else
			  {
			    if( Mapping::debug & 4 )
			      printf("INFO:countCrossings: no intersection found after perturbation.\n");
			  }
			}
			else
			  intersectionFound=TRUE;
		      }
		    }
		  }
		  if( intersectionFound )
		  {
		    // save the point of intersection
		    if( xx.getLength(2) <= cross )
		    {
		      // allocate more space for crossings
		      printf("countCrossingsWithPolygon:INFO:Too many crossings: allocating more space\n");
		      xx.resize(xx.dimension(0),xx.dimension(1),xx.getLength(2)+5);
		    }
		    xx(i,0,cross)=x0;   // line is vertical
		    xx(i,1,cross)=xi[1];
		    xx(i,2,cross)=x2;
                    xx(i,3,cross)=i1;
                    xx(i,4,cross)=i2;
                    xx(i,5,cross)=i3;
		    
		    for( int k=initialCross; k<cross; k++ )
		    {
		      // double check for multiple counts of the same point
                      //  A double crossing may occur at a corner of a grid
                      //          ---------------+ <- ray crosses this point twice
                      //                         |
                      //                         |
                      //                         X <- point to check
		      if( xx(i,1,k)==xi[1] )  // note (x0,x2) values will already be the same so need to check
		      {
			printf("INFO:countCrossings: EXACTLY the same point appears more"
			       " than once. I am NOT going to remove it, x=(%e,%e,%e)\n",x0,x1,x2);
			// cross--;
		      }
		      else if( fabs(xx(i,1,k)-xi[1])< epsY )  // note (x0,x2) values will be the same
		      {
			printf("INFO:countCrossings: NEARLY the same point appears more"
			       " than once. I am NOT going to remove it, x=(%e,%e,%e)\n",x0,x1,x2);
			// cross--;
			break;
		      }
		    }
		    cross++;
		  } // if intersection found
		}  // for j
	      }
	    }
	  }
	}
      }
      crossings(i)=cross;
      if( Mapping::debug & 4 )
        printf("countCrossing: checked point i=%i, x=(%e,%e,%e), (side,axis)=(%i,%i), crossings=%i \n\n",
          i,x0,x1,x2,side,axis,crossings(i));
      virtualPerturbation=FALSE;
    }
  }
  else 
  {
    cout << "ApproximateGlobalInverse::ERROR: countRayCrossings is not implemented for domainDimension=="
         << domainDimension << " and rangeDimension==" << rangeDimension << endl;
    {throw "error";}
  }
  
}


#undef numberOfStencilDirections2D
#undef stencilDirection2D1
#undef stencilDirection2D2
#undef numberOfStencilDirections3D
#undef stencilDirection3D1
#undef stencilDirection3D2
#undef stencilDirection3D3

#undef STENCIL_DEBUG
