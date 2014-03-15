#include "DistributedInverse.h"
#include "Inverse.h"
#include "ParallelUtility.h"

// This class is used to compute bounding boxes and inverses in parallel

DistributedInverse::
DistributedInverse( Mapping& map_ ) : map(map_)
{
  boundingBoxesComputed=false;
}

DistributedInverse::
~DistributedInverse()
{
}


int DistributedInverse::
get( const GenericDataBase & dir, const aString & name)    // get from a database file
{
  Overture::abort("DistributedInverse::get:ERROR: finish me...");
  return 0;
}

const RealArray & DistributedInverse::
getBoundingBox() const
// =====================================================================================
//  /Description:
//     Return the boundingBox for the entire domain.
// 
// =====================================================================================
{
  return boundingBox;
}

const BoundingBox& DistributedInverse::
getBoundingBoxTree(int side, int axis)
// =====================================================================================
//  /Description:
//     Return the BoundingBox tree for the boundary (side,axis). For now this is 
//  not a tree since it has no children. It does hold the bounding box for the entire 
//  boundary (side,axis).
// 
// =====================================================================================
{
  if( side<0 || side>1 || axis<0 || axis>=map.getRangeDimension() )
  {
    printf("DistributedInverse::getBoundingBoxTree:ERROR: invalid arguments: side=%i axis=%i "
           " map.getRangeDimension()=%i\n",side,axis,map.getRangeDimension());
    Overture::abort("error");
  }
  return boundingBoxTree[side][axis];
}

int DistributedInverse::
put( GenericDataBase & dir, const aString & name) const    // put to a database file
{
  Overture::abort("DistributedInverse::put:ERROR: finish me...");

  return 0;
}

real DistributedInverse::
sizeOf(FILE *file /* = NULL */ ) const
// ==================================================================================
// /Description:
//   Return size of this object  
// ==================================================================================
{
  return 0.;
}

int DistributedInverse::
computeBoundingBoxes()
// ==================================================================================
// /Description:
//    Compute the bounding boxes
// 
//  boundingBox(side,axis) : global bounding box for the grid that goes with the Mapping.
//
//  boundingBoxTree[2][3]  : The bounding box (tree) for each face of the grid. For now this is 
//  not a tree since it has no children. It does hold the bounding box for the entire 
//  boundary (side,axis).
//
//  
// ==================================================================================
{
  if( boundingBoxesComputed ) return 0;
  
  int debug=0; // 3 

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int domainDimension=map.getDomainDimension();
  const int rangeDimension=map.getRangeDimension();
  const realArray & x = map.getGrid();  
  

  // First make sure the bounding boxes are built for each local array

  assert( map.approximateGlobalInverse!=0 );
  map.approximateGlobalInverse->initialize();


  // Now build the bounding boxes for the distributed array by using the local array bounds

  // NOTE: There is a GAP in the local serial arrays for the grid since processor 0
  // might own indicies 0,1,2,..., 5 and processor 1 owns 6,7,...,10
  // Thus the bounding boxes from the local arrays DO NOT COVER THE DOMAIN


  // bbl(2,3) : Bounds on the local array
  const RealArray & bbl = map.approximateGlobalInverse->getBoundingBox();
  
  real xMinLocal[3], xMin[3], xMaxLocal[3], xMax[3];
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    xMinLocal[axis]=bbl(0,axis);
    xMaxLocal[axis]=bbl(1,axis);
  }

  ParallelUtility::getMinValues(xMinLocal,xMin,rangeDimension);
  ParallelUtility::getMaxValues(xMaxLocal,xMax,rangeDimension);

  boundingBox.redim(2,3);    // bounding box for the entire distributed grid
  boundingBox=0.;

  for( int axis=0; axis<rangeDimension; axis++ )
  {
    boundingBox(0,axis)=xMin[axis];
    boundingBox(1,axis)=xMax[axis];

  }
  if( debug & 1 )
  {
    printF("DistributedInverse:boundingBox = [%g,%g][%g,%g][%g,%g]\n",
	   boundingBox(0,0),boundingBox(1,0),
	   boundingBox(0,1),boundingBox(1,1),
	   boundingBox(0,2),boundingBox(1,2));
  }
  
  // **** Compute the BoundingBox for the boundary (side,axis) ****
  //      from the BoundingBox for the the local serial arrays

  RealArray bb(2,3);
  bb=0.;
  for( int axis=0; axis<domainDimension; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      if( map.getIsPeriodic(axis) != Mapping::functionPeriodic )
      { // we do not assign bounding boxes for periodic faces
	
	const BoundingBox & faceBoundingBox = map.approximateGlobalInverse->getBoundingBoxTree(side,axis);
	const RealArray & bbFace = faceBoundingBox.getRangeBound();
  
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  xMinLocal[dir]=REAL_MAX/10.;
	  xMaxLocal[dir]=-xMinLocal[dir];
	}
	if( debug & 2 )
	  printf("DistributedInverse:myid=%i bbFace for (side,axis)=(%i,%i) = [%g,%g][%g,%g][%g,%g]\n",
		 myid,side,axis,
		 bbFace(0,0),bbFace(1,0),
		 bbFace(0,1),bbFace(1,1),
		 bbFace(0,2),bbFace(1,2));

	// For bounding boxes on boundaries, only consider processors where the local array
	// falls on the boundary.
	if( faceBoundingBox.domainBound(side,axis)==map.getGridIndexRange(side,axis)  )
	{
	  // this local face corresponds to a boundary of the distributed array
	  for( int dir=0; dir<rangeDimension; dir++ )
	  {
	    xMinLocal[dir]=bbFace(0,dir);
	    xMaxLocal[dir]=bbFace(1,dir);
	  }
	}

        ParallelUtility::getMinValues(xMinLocal,xMin,rangeDimension);
        ParallelUtility::getMaxValues(xMaxLocal,xMax,rangeDimension);

	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0,dir)=xMin[dir];
	  bb(1,dir)=xMax[dir];
    
	}
	boundingBoxTree[side][axis].setRangeBound(bb);
	if( debug & 1 )
	  printF("DistributedInverse:boundingBox for (side,axis)=(%i,%i) = [%g,%g][%g,%g][%g,%g]\n",
		 side,axis,
		 bb(0,0),bb(1,0),
		 bb(0,1),bb(1,1),
		 bb(0,2),bb(1,2));

      }
    }
  }
  
  boundingBoxesComputed=true;
  return 0;
}
