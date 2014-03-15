#include "BoundaryAdjustment.h"
#include "GridCollection.h"

BoundaryAdjustment::
BoundaryAdjustment() 
// ====================================================================================
// /Description:
//    Default constructor.
// ====================================================================================
{
  ba=ag=ob=NULL;
  ss=NULL;
  computedGeometry=GridCollection::NOTHING;
  sharedSidesExist=unknown;  // are there any shared sides
};

BoundaryAdjustment::
~BoundaryAdjustment()
{
  destroy();
}


BoundaryAdjustment::
BoundaryAdjustment(const BoundaryAdjustment& x)
// =======================================================================
// /Description:
//   Copy constructor.
// =======================================================================
{
  *this=x;
}


BoundaryAdjustment& BoundaryAdjustment::
operator=(const BoundaryAdjustment& x) 
// =======================================================================
// /Description:
//    Equals operator. *** this is shallow ****
// =======================================================================
{
  computedGeometry=x.computedGeometry;
  
  ba               = x.ba;
  ag               = x.ag;
  ob               = x.ob;
  ss               = x.ss;
  sharedSidesExist =x.sharedSidesExist;
  return *this;
}


int BoundaryAdjustment::
reference( const BoundaryAdjustment& x)
// =======================================================================
// /Description:
//   Reference.
// =======================================================================
{
  computedGeometry=x.computedGeometry;
  sharedSidesExist =x.sharedSidesExist;
  if( x.ba!=NULL )
  {
    if( ba==NULL )
      create();
    ba->reference(*x.ba);
    ag->reference(*x.ag);
    ob->reference(*x.ob);
    ss->reference(*x.ss);
  }
  else if( ba!=NULL )
  { // x is null so destroy this.
    destroy();
  }
  return 0;
}



//! return the size in bytes of this object.
real BoundaryAdjustment::
sizeOf(FILE *file /* = NULL */ ) const
{
  real size=sizeof(*this);
  if( ba!=NULL )  size+=ba->elementCount()*sizeof(real);
  if( ag!=NULL )  size+=ag->elementCount()*sizeof(real);
  if( ob!=NULL )  size+=ob->elementCount()*sizeof(real);
  if( ss!=NULL )  size+=ss->elementCount()*sizeof(int);

  return size;
}



int BoundaryAdjustment::
create()
// =======================================================================
// /Description:
//   Create the arrays found in this object.
// =======================================================================
{
  if( ba==NULL ) ba=new RealArray;
  if( ag==NULL ) ag=new RealArray;
  if( ob==NULL ) ob=new RealArray;
  if( ss==NULL )
  {
    ss=new IntegerArray(2,3);
    *ss=unknown;
  }
  return 0;
}

int BoundaryAdjustment::
destroy()
// =======================================================================
// /Description:
//   Destroy the arrays found in this object.
// =======================================================================
{
  computedGeometry=GridCollection::NOTHING;
  
  delete ba; ba=NULL;
  delete ag; ag=NULL;
  delete ob; ob=NULL;
  delete ss; ss=NULL;
  return 0;
}


BoundaryAdjustment::SidesShareEnum BoundaryAdjustment::
sidesShare(int side, int axis) const
// =======================================================================
// /Description:
//    Indicate whether the current face of the grid being adjusted  shares boundary points
// with the face (side,axis) of the "other grid" we are adjusting for.
// =======================================================================
{
   assert(ss!=NULL && side>=0 && side<=1 && axis>=0 && axis<=2);

  return (SidesShareEnum) (*ss)(side,axis);
}

