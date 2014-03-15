#include "BoundingBox.h"
#include "ListOfBoundingBox.h"

BoundingBox::
BoundingBox( const int domainDimension0, const int rangeDimension0 )
{
  domainDimension=domainDimension0;
  rangeDimension=rangeDimension0;
  parent=child1=child2=NULL;
  defined=FALSE;
}

BoundingBox::
~BoundingBox()
{
  // cout << "~BoundingBox called" << endl;
  delete child1;
  delete child2;
}

//=================================================================
//  Set the domainDimension and rangeDimension
//=================================================================
void BoundingBox::
setDimensions( const int domainDimension0, const int rangeDimension0 )
{
  domainDimension=domainDimension0;
  rangeDimension=rangeDimension0;
}

  
IntegerArray BoundingBox::
getDomainBound() const
// ====================================================================================
// /Description:
// Return an IntegerArray holding the domain bounds.
// /Return value:
//   An IntegerArray of size (0:1,0:domainDimension-1)
// ====================================================================================
{
  IntegerArray d(2,domainDimension);
  for( int axis=0; axis<domainDimension; axis++ )
  {
    d(0,axis)=domain[0][axis];
    d(1,axis)=domain[1][axis];
  }
  return d;
}

RealArray BoundingBox::
getRangeBound() const
// ====================================================================================
// /Description:
// Return an RealArray holding the range bounds.
// /Return value:
//   An RealArray of size (0:1,0:rangeDimension-1)
// ====================================================================================
{
  RealArray r(2,rangeDimension);
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    r(0,axis)=range[0][axis];
    r(1,axis)=range[1][axis];
  }
  return r;
}

  
int BoundingBox::
setDomainBound( const IntegerArray & d)
{
  for( int axis=0; axis<domainDimension; axis++ )
  {
    domain[0][axis]=d(0,axis);
    domain[1][axis]=d(1,axis);
  }
  return 0;
}

int 
BoundingBox::
setRangeBound( const RealArray & r)
{
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    range[0][axis]=r(0,axis);
    range[1][axis]=r(1,axis);
  }
  return 0;
}


int BoundingBox::
domainBoundEquals(const BoundingBox & b)
{
  for( int axis=0; axis<domainDimension; axis++ )
  {
    domain[0][axis]=b.domain[0][axis];
    domain[1][axis]=b.domain[1][axis];
  }
  return 0;
  
}

int BoundingBox::
rangeBoundEquals(const BoundingBox & b)
{
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    range[0][axis]=b.range[0][axis];
    range[1][axis]=b.range[1][axis];
  }
  return 0;
}



void BoundingBox::
addChildren()
{
  delete child1;
  delete child2;
  child1=new BoundingBox( domainDimension,rangeDimension );  child1->parent=this;
  child2=new BoundingBox( domainDimension,rangeDimension );  child2->parent=this;
}

void BoundingBox::
deleteChildren()
{
  delete child1; 
  delete child2;
  child1=child2=0;
}


//=================================================================
//  Display a tree of bounding boxes
//=================================================================
void BoundingBox::
display( const aString & comment ) const
{
  cout << comment << endl;
  printf("domainBound : [%i,%i]x[%i,%i]x[%i,%i] rangeBound=[%8.2e,%8.2e]x[%8.2e,%8.2e]x[%8.2e,%8.2e] \n",
	 domain[0][0],domain[1][0],
	 domain[0][1],domain[1][1],
	 domain[0][2],domain[1][2],
	 range[0][0],range[1][0],
	 range[0][1],range[1][1],
	 range[0][2],range[1][2]);

  if( child1!=NULL ) child1->display(comment+"/child1");  
  if( child2!=NULL ) child2->display(comment+"/child2");  
}


bool BoundingBox:: 
intersects( const BoundingBox & box ) const
// does this box intersect another (in the range space).
{
  assert( rangeDimension==box.rangeDimension );
  
  switch (rangeDimension)
  {
  case 1:
    return range[End][axis1]>=box.range[Start][axis1] && box.range[End][axis1]>=range[Start][axis1];
  case 2:
    return range[End][axis1]>=box.range[Start][axis1] && box.range[End][axis1]>=range[Start][axis1] &&
           range[End][axis2]>=box.range[Start][axis2] && box.range[End][axis2]>=range[Start][axis2];
  case 3:
    return range[End][axis1]>=box.range[Start][axis1] && box.range[End][axis1]>=range[Start][axis1] &&
           range[End][axis2]>=box.range[Start][axis2] && box.range[End][axis2]>=range[Start][axis2] &&
           range[End][axis3]>=box.range[Start][axis3] && box.range[End][axis3]>=range[Start][axis3];
  default:
    cout << "BoundingBox::intersects: invalid rangeDimension = " << rangeDimension << endl;
  }
  return FALSE;
  
}


static BoundingBox nullBoundingBox;

BoundingBoxStack::
BoundingBoxStack()
{
  stack= new ListOfBoundingBox;
  bottom=0;
  top=bottom-1;
}

BoundingBoxStack::
~BoundingBoxStack()
{
  delete stack;
}

void BoundingBoxStack::
push( BoundingBox & bb )
{
  top++;
  if( stack->getLength() > top )
    stack->setElementPtr( &bb,top );
  else
    stack->addElement( bb,top );
}


//=====================================================================================
// pop returns a null box if the stack is empty
//=====================================================================================
BoundingBox& BoundingBoxStack::
pop()
{
  if( top < 0 )
  {
    cout << "BoundingBoxStack:ERROR attempt to pop an empty stack!" << endl;
    return nullBoundingBox;
  }
  else
    return (*stack)[top--];
}

int BoundingBoxStack::isEmpty() const
{
  return top<0;
}

