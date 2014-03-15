#include "TFIMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>
#include "LineMapping.h"
#include "display.h"
#include "ParallelUtility.h"


TFIMapping::
TFIMapping(Mapping *left   /* =NULL */, 
	   Mapping *right  /* =NULL */,
	   Mapping *bottom /* =NULL */,
	   Mapping *top    /* =NULL */,
	   Mapping *front  /* =NULL */,
	   Mapping *back   /* =NULL */)
: Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief 
///      Build a TFIMapping and optionally supply curves that define the boundaries.
///   Specify 0, 2, 4 or 6 curves.
///  The Trans-Finite-Interpolation (TFI) Mapping (also known as a Coon's patch)
///  will interpolate between the boundary
///  curves to define a mapping in the space between. See the documentation for further
///  details.
/// \param left, right (input): curves for $r_1=0$ and $r_1=1$.
/// \param bottom, top (input): curves for $r_2=0$ and $r_2=1$.
/// \param front, back (input): curves for $r_3=0$ and $r_3=1$ (3D only).
///  
//===========================================================================
{ 
  assert( (left==NULL && right==NULL) || (left!=NULL && right!=NULL) );
  assert( (bottom==NULL && top==NULL) || (bottom!=NULL && top!=NULL) );
  assert( (front==NULL && back==NULL) || (front!=NULL && back!=NULL) );
  

  TFIMapping::className="TFIMapping";
  setName( Mapping::mappingName,"TFIMapping");
  setGridDimensions( axis1,15 );
  setGridDimensions( axis2,15 );
  blendingFunction=NULL;
  flip.redim(2,6);
  flip=0;

  int i;
  for( i=0;i<3;i++)
    interpolationType[i]=linear;
  
  setSides(left,right,bottom,top,front,back);
  blendingFunction = NULL;
}



// Copy constructor is deep by default
TFIMapping::
TFIMapping( const TFIMapping & map, const CopyType copyType )
{
  TFIMapping::className="TFIMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "TFIMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

TFIMapping::
~TFIMapping()
{ 
  if( debug & 4 )
    cout << " TFIMapping::Desctructor called" << endl;
// release the curves we don't need
  int i, j;
  for (i=0; i<2; i++)
    for (j=0; j<3; j++)
      if (curve[i][j] && curve[i][j]->decrementReferenceCount() == 0)
	delete curve[i][j];

// release the blending function
  if (blendingFunction && blendingFunction->decrementReferenceCount() == 0)
    delete blendingFunction;
  
}

TFIMapping & TFIMapping::
operator=( const TFIMapping & X )
{
  if( TFIMapping::className != X.getClassName() )
  {
    cout << "TFIMapping::operator= ERROR trying to set a TFIMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  int i;
  
  for( i=0;i<3;i++)
  interpolationType[i]=X.interpolationType[i];
  numberOfSidesSpecified=X.numberOfSidesSpecified;
  for( int axis=0; axis<3; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      curve[side][axis]=X.curve[side][axis];                  // this sets the pointer only
      if( curve[side][axis]!=NULL ) 
        curve[side][axis]->incrementReferenceCount();  // *wdh* 030417
    }
  }
  
  blendingFunction=X.blendingFunction;
  if( blendingFunction!=NULL )
    blendingFunction->incrementReferenceCount();  // *wdh* 030417

  flip.redim(0);
  flip=X.flip;
  corner.redim(0);
  corner=X.corner;
  this->Mapping::operator=(X);            // call = for derivee class
  return *this;
}

int TFIMapping::
setInterpolationType(const InterpolationType & direction1, 
		     const InterpolationType & direction2 /* =linear */, 
		     const InterpolationType & direction3 /* =linear */)
//===========================================================================
/// \brief 
///     Set the interpolation type along each axis.
/// \param direction1, direction2, direction3 (input): interpolation type along each axis.
///  
//===========================================================================
{
  interpolationType[0]=direction1;
  interpolationType[1]=direction2;
  interpolationType[2]=direction3;
  return 0;
}


int TFIMapping::
setSides(Mapping *left   /* =NULL */, 
	 Mapping *right  /* =NULL */,
	 Mapping *bottom /* =NULL */,
	 Mapping *top    /* =NULL */,
	 Mapping *front  /* =NULL */,
	 Mapping *back   /* =NULL */)
//===========================================================================
/// \brief 
///      Build a TFIMapping and supply curves that define the boundaries.
///   Specify 0, 2, 4 or 6 curves.
///  The Trans-Finite-Interpolation (TFI) Mapping (also known as a Coon's patch)
///  will interpolate between the boundary
///  curves to define a mapping in the space between. See the documentation for further
///  details.
/// \param left, right (input): curves for $r_1=0$ and $r_1=1$.
/// \param bottom, top (input): curves for $r_2=0$ and $r_2=1$.
/// \param front, back (input): curves for $r_3=0$ and $r_3=1$ (3D only).
///  
//===========================================================================
{ 
  assert( (left==NULL && right==NULL) || (left!=NULL && right!=NULL) );
  assert( (bottom==NULL && top==NULL) || (bottom!=NULL && top!=NULL) );
  assert( (front==NULL && back==NULL) || (front!=NULL && back!=NULL) );
  
  curve[0][0]=left;
  curve[1][0]=right;
  curve[0][1]=bottom;
  curve[1][1]=top;
  curve[0][2]=front;
  curve[1][2]=back;

// make sure the curves stay around for as long as we need them
  int i, j;
  for (i=0; i<2; i++)
    for (j=0; j<3; j++)
      if (curve[i][j]) curve[i][j]->incrementReferenceCount();

  numberOfSidesSpecified=0;
  for( int axis=0; axis<=2; axis++ )
  {
    if( curve[0][axis]!=NULL || curve[1][axis]!=NULL )
    {
      numberOfSidesSpecified+=2;
// AP: Why not allow for 2 or 4 3-D curves to define a surface in 3-D
//        for( int side=0; side<=1; side++ )
//        {
//  	if( curve[side][axis]->getRangeDimension() - curve[side][axis]->getDomainDimension() != 1 )
//  	{
//  	  printf("TFIMapping::ERROR: curve[side=%i][axis=%i] is not a curve in 2D or surface in 3D \n",side,axis);
//  	  printf("                   domainDimension=%i, rangeDimension=%i \n",curve[side][axis]->getDomainDimension(),
//  	         curve[side][axis]->getRangeDimension());
//  	  throw "error";
//  	}
//        }
    }
  }
  if( numberOfSidesSpecified>0 )
  {
    if( numberOfSidesSpecified!=2 && numberOfSidesSpecified!=4 && numberOfSidesSpecified!=6 )
    {
      cout << "TFIMapping::ERROR: The first 2 or 4 or 6 curves must be given \n";
      cout << "  The number of consecutive non-NULL curves specified was = " << numberOfSidesSpecified << endl;
      {throw "error";}
    }
    setMappingProperties();
    // call the initialization routine to check consistency of sides and compute
    // locations of corners
    initialize();
  
    mappingHasChanged();
  }
  return 0;
}


int TFIMapping::
setMappingProperties()
// ===========================================================================
// Set default properties of the tfi mapping from the curves.
// ===========================================================================
{
  assert( numberOfSidesSpecified>0 );
      
  int domainDim=-1;
  int rangeDim=-1;
  int axis;
  for( axis=0; axis<3; axis++ )
  {
    // find the first valid curve and choose grid dimensions for the tangential directions from it.
    if( curve[0][axis]!=NULL )
    {
      Mapping & curve0 = *curve[0][axis];
    
      if( domainDim<0 )
      {
	domainDim=curve0.getDomainDimension()+1;
	setDomainDimension(domainDim);
	rangeDim=curve0.getRangeDimension();
	setRangeDimension(rangeDim);
        if( Mapping::debug & 4 )
          printf("TFIMapping::setting domainDimension=%i, rangeDimension=%i \n",domainDimension,rangeDimension);
      }
      setGridDimensions(axis,11);                     // normal direction
      const int tAxis[3][2] = { 1,2, 0,2, 0,1  };
      for( int dir=0; dir<domainDimension-1; dir++ )
      {
        const int & axisT = tAxis[axis][dir];
	setGridDimensions(axisT,curve0.getGridDimensions(dir));
	setIsPeriodic(axisT,curve0.getIsPeriodic(dir));
      }
      break;
    }
  }
  for( axis=0; axis<domainDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( !getIsPeriodic(axis) )
	setBoundaryCondition(side,axis,1);
      else	
	setBoundaryCondition(side,axis,-1);
    }
  }
  
  return 0;
}
  
int TFIMapping::
checkEdge( const int & sideCurve1, const int & axisCurve1, const int & side1, const int & dir1, 
           const int & sideCurve2, const int & axisCurve2, const int & side2, const int & dir2 )
// ============================================================================
// \Description:
// check two edges for consistency
// ============================================================================
{
  assert( curve[sideCurve1][axisCurve1]!=NULL && curve[sideCurve2][axisCurve2]!=NULL );

  const int n=5;
  Range I(0,n-1);
  realArray r(I,2);
  realArray edge1(I,rangeDimension), edge2(I,rangeDimension);

  r(I,dir1)=side1;
  r(I,(dir1+1)%2).seqAdd(0.,1./n);
  curve[sideCurve1][axisCurve1]->map(r,edge1);

  r(I,dir2)=side2;
  r(I,(dir2+1)%2).seqAdd(0.,1./n);
  curve[sideCurve2][axisCurve2]->map(r,edge2);
    
  const real eps=.001;
  if( max(fabs(edge2-edge1)) > max(fabs(edge1))*eps )
  {
    printf("TFIMapping:ERROR: checkEdge: edges do not match for curve[%i][%i], (side,axis)=(%i,%i) \n"
           "                                                and curve[%i][%i], (side,axis)=(%i,%i) \n",
           sideCurve1,axisCurve1,side1,dir1, sideCurve2,axisCurve2,side2,dir2);
    ::display(edge1,"Here is the edge 1");
    ::display(edge2,"Here is the edge 2");
  }
  else
  {
    // printf("TFIMapping: checkEdge: edges DO MATCH for curve=%i, (side,axis)=(%i,%i) \n"
    //        "                                      and curve=%i, (side,axis)=(%i,%i) \n",
    //        curve1,side1,dir1,curve2,side2,dir2);
  }
  return 0;
}




void TFIMapping::
initialize()
  //
  // initialize will set up the corner array to be correct size, and
  // will then fill in the locations of the corners
  //
{

  int axis;
  Range I2(0,1);
  Range IR=rangeDimension;
  
  realArray r,x0,x1,x2,x3,x4,x5;
  
  if( domainDimension==2 )
    corner.redim(rangeDimension/*domainDimension*/,2,2);
  else
    corner.redim(rangeDimension/*domainDimension*/,2,2,2);

  
#define SQR(x) ((x)*(x))
  if( numberOfSidesSpecified==2  && domainDimension==2 )
  {
// check the distance between the end points
    if (curve[0][0] && curve[1][0])
    {
      r.redim(I2);
      r(0)=0.0;
      r(1)=1.0;
        
      x0.redim(I2,IR),x1.redim(I2,IR),x2.redim(I2,IR),x3.redim(I2,IR);

      curve[0][0]->map(r,x0);
      curve[1][0]->map(r,x1);
      real dist[4]={0.,0.,0.,0.};
      for (axis=0; axis<rangeDimension; axis++)
      {
	dist[0] += SQR(x0(0,axis)-x1(0,axis));
	dist[1] += SQR(x0(0,axis)-x1(1,axis));
	dist[2] += SQR(x0(1,axis)-x1(0,axis));
	dist[3] += SQR(x0(1,axis)-x1(1,axis));
      }
      if (dist[1] < dist[0] && dist[2] < dist[3])
      {
	flip(1,0) = !flip(1,0);
	printf("Reversing curve[1][0]\n");
      }
    }
  }
  else if( numberOfSidesSpecified==4 && domainDimension==2 )
  {
    //
    // two dimensional domain, four sides specified,
    //

        // r will be the lower and upper range of the domain space
    r.redim(I2);
    r(0)=0.0;
    r(1)=1.0;
        
    x0.redim(I2,IR),x1.redim(I2,IR),x2.redim(I2,IR),x3.redim(I2,IR);

// check if some curves need to be reversed or swapped
    bool reOrder=false;
    if( debug & 1 ) printf("Checking corner #0\n");
    do
    {
      if (flip(0,0) == 0)
	curve[0][0]->map(r,x0);
      else
	curve[0][0]->map(evaluate(1.-r),x0);

      if (flip(1,0) == 0)
	curve[1][0]->map(r,x1);
      else
	curve[1][0]->map(evaluate(1.-r),x1);

      if (flip(0,1) == 0)
	curve[0][1]->map(r,x2);
      else
	curve[0][1]->map(evaluate(1.-r),x2);

      if (flip(1,1) == 0)
	curve[1][1]->map(r,x3);
      else
	curve[1][1]->map(evaluate(1.-r),x3);

      real dist0[4]={0.,0.,0.,0.};
      for (axis=0; axis<rangeDimension; axis++)
      {
	dist0[0] += SQR(x0(0,axis)-x2(0,axis));
	dist0[1] += SQR(x0(0,axis)-x2(1,axis));
	dist0[2] += SQR(x0(0,axis)-x3(0,axis));
	dist0[3] += SQR(x0(0,axis)-x3(1,axis));
      }
      if( debug & 1 ) 
          printf("Corner gaps: dist00=%e, dist01=%e, dist02=%e, dist03=%e\n", dist0[0], dist0[1], dist0[2], dist0[3]);
      int iMin=0;
      real dMin=dist0[0];
      for (int i=1; i<4; i++)
	if (dist0[i] < dMin)
	{
	  dMin = dist0[i];
	  iMin = i;
	}
    
// reordering needed?
      if (iMin == 0)
      {
	if( debug & 1 )  printf("No reordering needed for corner 0\n");
	reOrder = false;
      }
      else
      {
	reOrder = true;
	Mapping *tmpMap;
	if (iMin == 1)
	{
	  // reverse curve[0][1]
	  flip(0,1) = !flip(0,1);
	  if( debug & 1 ) printf("Reversing curve[0][1]\n");
	}
	else if (iMin == 2 || iMin == 3 )
	{
	  // swap curve[0][1] and curve[1][1]
	  if( debug & 1 ) printf("Swapping curve[0][1] and [1][1]\n");
	  tmpMap = curve[0][1];
	  curve[0][1] = curve[1][1];
	  curve[1][1] = tmpMap;
	  if (iMin == 3)
	  {
	    // reverse curve[1][1]
	    flip(1,1) = !flip(1,1);
	    if( debug & 1 ) printf("Reversing curve[1][1]\n");
	  }
	}
      }
    } while(reOrder);

// corner 2
    if( debug & 1 ) printf("Checking corner #2\n");
    do
    {
      if (flip(0,0) == 0)
	curve[0][0]->map(r,x0);
      else
	curve[0][0]->map(evaluate(1.-r),x0);

      if (flip(1,0) == 0)
	curve[1][0]->map(r,x1);
      else
	curve[1][0]->map(evaluate(1.-r),x1);

      if (flip(0,1) == 0)
	curve[0][1]->map(r,x2);
      else
	curve[0][1]->map(evaluate(1.-r),x2);

      if (flip(1,1) == 0)
	curve[1][1]->map(r,x3);
      else
	curve[1][1]->map(evaluate(1.-r),x3);

      real dist2[4]={0.,0.,0.,0.};
      for (axis=0; axis<rangeDimension; axis++)
      {
	dist2[0] += SQR(x0(1,axis)-x2(0,axis));
	dist2[1] += SQR(x0(1,axis)-x2(1,axis));
	dist2[2] += SQR(x0(1,axis)-x3(0,axis));
	dist2[3] += SQR(x0(1,axis)-x3(1,axis));
      }
      if( debug & 1 ) 
        printf("Corner gaps: dist20=%e, dist21=%e, dist22=%e, dist23=%e\n", dist2[0], dist2[1], dist2[2], dist2[3]);
      int iMin=0;
      real dMin=dist2[0];
      for (int i=1; i<4; i++)
	if (dist2[i] < dMin)
	{
	  dMin = dist2[i];
	  iMin = i;
	}
    
// reordering needed?
      if (iMin == 2)
      {
	if( debug & 1 ) printf("No reordering needed for corner 2\n");
	reOrder = false;
      }
      else
      {
	reOrder = true;
	Mapping *tmpMap;
	if (iMin == 3)
	{
	  // reverse curve[1][1]
	  flip(1,1) = !flip(1,1);
	  if( debug & 1 ) printf("Reversing curve[1][1]\n");
	}
	else if (iMin == 0 || iMin == 1 )
	{
	  // swap curve[0][1] and curve[1][1]
	  printf("Swapping curve[0][1] and [1][1]\n");
	  tmpMap = curve[0][1];
	  curve[0][1] = curve[1][1];
	  curve[1][1] = tmpMap;
	  if (iMin == 1)
	  {
	    // reverse curve[0][1]
	    flip(0,1) = !flip(0,1);
	    if( debug & 1 ) printf("Reversing curve[0][1]\n");
	  }
	}
      }
    } while(reOrder);
//
// necessary to swap curve[1][0]?    
//    
    if( debug & 1 ) printf("Checking corner #1\n");
    do
    {
      if (flip(0,0) == 0)
	curve[0][0]->map(r,x0);
      else
	curve[0][0]->map(evaluate(1.-r),x0);

      if (flip(1,0) == 0)
	curve[1][0]->map(r,x1);
      else
	curve[1][0]->map(evaluate(1.-r),x1);

      if (flip(0,1) == 0)
	curve[0][1]->map(r,x2);
      else
	curve[0][1]->map(evaluate(1.-r),x2);

      if (flip(1,1) == 0)
	curve[1][1]->map(r,x3);
      else
	curve[1][1]->map(evaluate(1.-r),x3);

      real dist1[4]={1.e7,0.,1.e7,0.};
      for (axis=0; axis<rangeDimension; axis++)
      {
	dist1[1] += SQR(x1(0,axis)-x2(1,axis));
	dist1[3] += SQR(x1(0,axis)-x3(1,axis));
      }
      if( debug & 1 ) printf("Corner gaps: dist11=%e, dist13=%e\n", dist1[1], dist1[3]);
      int iMin=0;
      real dMin=dist1[0];
      for (int i=1; i<4; i++)
	if (dist1[i] < dMin)
	{
	  dMin = dist1[i];
	  iMin = i;
	}
    
// reordering needed?
      if (iMin == 1)
      {
	if( debug & 1 ) printf("No reordering needed for corner 1\n");
	reOrder = false;
      }
      else
      {
	reOrder = true;
	Mapping *tmpMap;
	if (iMin == 3)
	{
	  // reverse curve[1][0]
	  flip(1,0) = !flip(1,0);
	  if( debug & 1 ) printf("Reversing curve[1][0]\n");
	}
      }
    } while(reOrder);

//
// check consistency (do corners match?)
//
// AP: Wouldn't it be better to reorder (or reverse parametrization of) the curves to make 
// sure the corners are consistent?
//      assert( SQR(x0(0,0)-x2(0,0))+SQR(x0(0,1)-x2(0,1)) < 1.e-6 );
//      assert( SQR(x1(0,0)-x2(1,0))+SQR(x1(0,1)-x2(1,1)) < 1.e-6 );
//      assert( SQR(x0(1,0)-x3(0,0))+SQR(x0(1,1)-x3(0,1)) < 1.e-6 );
//      assert( SQR(x1(1,0)-x3(1,0))+SQR(x1(1,1)-x3(1,1)) < 1.e-6 );
    if ( SQR(x0(0,0)-x2(0,0))+SQR(x0(0,1)-x2(0,1)) >= 1.e-6 )
      printf("Warning: missmatch at corner 0\n");
    if ( SQR(x1(0,0)-x2(1,0))+SQR(x1(0,1)-x2(1,1)) >= 1.e-6 )
      printf("Warning: missmatch at corner 1\n");
    if ( SQR(x0(1,0)-x3(0,0))+SQR(x0(1,1)-x3(0,1)) >= 1.e-6 )
      printf("Warning: missmatch at corner 2\n");
    if ( SQR(x1(1,0)-x3(1,0))+SQR(x1(1,1)-x3(1,1)) >= 1.e-6 )
      printf("Warning: missmatch at corner 3\n");

    for( axis=0/*axis1*/; axis<rangeDimension; axis++ )
    {
      corner(axis,0,0) = x0(0,axis);
      corner(axis,1,0) = x1(0,axis);
      corner(axis,0,1) = x0(1,axis);
      corner(axis,1,1) = x1(1,axis);
    }
    
    if( Mapping::debug & 16 )
     ::display(corner,"TFIMapping::Here is the corner array");
  }
  else if( numberOfSidesSpecified==6 || domainDimension==3 )
  {
    //
    // three dimensional domain, 4 or 6  sides specified
    //
    if( curve[0][0]!=NULL && curve[0][1]!=NULL )
    {
      // check common edges between r_1={0,1} and r_2={0,1}

      checkEdge( 0,0,Start,0, 0,1,Start,0 );
      checkEdge( 0,0,End  ,0, 1,1,Start,0 );
      checkEdge( 1,0,Start,0, 0,1,End  ,0 );
      checkEdge( 1,0,End  ,0, 1,1,End  ,0 );
    }
    if( curve[0][1]!=NULL && curve[0][2]!=NULL )
    {
      // check common edges between r_2={0,1} and r_3={0,1}

      checkEdge( 0,1,Start,1, 0,2,Start,1 );
      checkEdge( 0,1,End  ,1, 1,2,Start,1 );
      checkEdge( 1,1,Start,1, 0,2,End  ,1 );
      checkEdge( 1,1,End  ,1, 1,2,End  ,1 );
    }
    if( curve[0][0]!=NULL && curve[0][2]!=NULL )
    {
      // check common edges between r_1={0,1} and r_3={0,1}

      checkEdge( 0,0,Start,1, 0,2,Start,0 );
      checkEdge( 0,0,End  ,1, 1,2,Start,0 );
      checkEdge( 1,0,Start,1, 0,2,End  ,0 );
      checkEdge( 1,0,End  ,1, 1,2,End  ,0 );
    }
    if( numberOfSidesSpecified==6 )
    {
      // evaluate the 8 vertices
      r.redim(4,domainDimension-1); x0.redim(4,rangeDimension); x1.redim(4,rangeDimension);
      r(0,0)=0.; r(0,1)=0.;
      r(1,0)=1.; r(1,1)=0.;
      r(2,0)=0.; r(2,1)=1.;
      r(3,0)=1.; r(3,1)=1.;
	
      curve[0][2]->map(r,x0);
      curve[1][2]->map(r,x1);
      for( axis=axis1; axis<rangeDimension; axis++ )
      {
	corner(axis,0,0,0)=x0(0,axis);
	corner(axis,1,0,0)=x0(1,axis);
	corner(axis,0,1,0)=x0(2,axis);
	corner(axis,1,1,0)=x0(3,axis);
	
	corner(axis,0,0,1)=x1(0,axis);
	corner(axis,1,0,1)=x1(1,axis);
	corner(axis,0,1,1)=x1(2,axis);
	corner(axis,1,1,1)=x1(3,axis);
      }
      if( Mapping::debug & 16 )
	::display(corner,"TFIMapping::Here is the corner array");
    }
  }
}

int TFIMapping::
flipper()
//=====================================================================================
/// \brief 
///   Try to flip the curve parameterizations to make the mapping non-singular.
/// \param Notes:
///    Fix up a TFIMapping that turns inside out because the bounding curves
///  are not parameterized in compatible ways.
//=====================================================================================
{
  // first check the Jacobian at the corners. They should all have the same sign.

  const int n = domainDimension==2 ? 4 : 8;
  realArray r(n,domainDimension),det(n),
    x(n,rangeDimension),xr(n,rangeDimension,domainDimension);

  r=0.;
  r(0,0)=0.; r(0,1)=0.;
  r(1,0)=1.; r(1,1)=0.;
  r(2,0)=0.; r(2,1)=1.;
  r(3,0)=1.; r(3,1)=1.;
  if( domainDimension==3 )
  {
    r(4,0)=0.; r(4,1)=0.; r(4,2)=1.;
    r(5,0)=1.; r(5,1)=0.; r(5,2)=1.;
    r(6,0)=0.; r(6,1)=1.; r(6,2)=1.;
    r(7,0)=1.; r(7,1)=1.; r(7,2)=1.;
  }
  assert( curve[0][0]!=NULL && curve[1][0]!=NULL );
  map(r,x,xr);

  int ok=TRUE;
  if( domainDimension==2 )
  {
    for( int i=0; i<n; i++ )
      det(i)=xr(i,0,0)*xr(i,1,1)-xr(i,0,1)*xr(i,1,0);
      
    if( det(0)*det(2) <= 0. || det(1)*det(3) <=0. )
    {
      ok=FALSE;
      printf("TFIMapping::flipper: Mapping is flipped, reverse orientation of curve[1][0] to fix\n");
      flip(0,1)=1;
    }
  }
  if( ok )  
      printf("TFIMapping::flipper: Mapping appears to be non-singular. no flipping needed\n");
  else
    mappingHasChanged();
  
  return 0;
}



void TFIMapping::
map( const realArray & r_, realArray & x_, realArray & xr_, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the TFI and/or derivatives. 
//=====================================================================================
{
  #ifdef USE_PPP
    const realSerialArray & r = r_.getLocalArray();
    realSerialArray x; getLocalArrayWithGhostBoundaries(x_,x);
    realSerialArray xr; getLocalArrayWithGhostBoundaries(xr_,xr);
    
  #else
    const realSerialArray & r = r_;
    realSerialArray & x = x_;
    realSerialArray & xr = xr_;
  #endif

  if( numberOfSidesSpecified==0 || numberOfSidesSpecified % 2 ==1 )
  {
    cout << "TFIMapping::map: Error: There must be 2,4 or 6 sides specified!\n";
    exit(1);    
  }
  
  if( params.coordinateType != cartesian )
  {
    cerr << "TFIMapping::map - coordinateType != cartesian " << endl;
  }

  //Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  
  if( numberOfSidesSpecified==2 )                             
  {
    mapNumberOfSides2(r,x,xr,params);
  }
  else if(numberOfSidesSpecified == 4 && domainDimension==2 )  
  {
    mapNumberOfSides4AndDim2(r,x,xr,params);
  }
  else if(numberOfSidesSpecified == 4 && domainDimension==3 )
  {
    mapNumberOfSides4AndDim3(r,x,xr,params);
  }
  else if(numberOfSidesSpecified == 6 )
  {
    mapNumberOfSides6(r,x,xr,params);
  }
}


void TFIMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
//=====================================================================================
// /Purpose: Evaluate the TFI and/or derivatives. 
//=====================================================================================
{
  if( numberOfSidesSpecified==0 || numberOfSidesSpecified % 2 ==1 )
  {
    cout << "TFIMapping::map: Error: There must be 2,4 or 6 sides specified!\n";
    exit(1);    
  }
  
  if( params.coordinateType != cartesian )
  {
    cerr << "TFIMapping::map - coordinateType != cartesian " << endl;
  }

  //Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  
  if( numberOfSidesSpecified==2 )                             
  {
    mapNumberOfSides2(r,x,xr,params);
  }
  else if(numberOfSidesSpecified == 4 && domainDimension==2 )  
  {
    mapNumberOfSides4AndDim2(r,x,xr,params);
  }
  else if(numberOfSidesSpecified == 4 && domainDimension==3 )
  {
    mapNumberOfSides4AndDim3(r,x,xr,params);
  }
  else if(numberOfSidesSpecified == 6 )
  {
    mapNumberOfSides6(r,x,xr,params);
  }
}


//-----SPLIT UP TFIMapping::map to get around an SGI compiler bug **pf 5/16/00
void TFIMapping::
mapNumberOfSides2( const RealArray & r, RealArray & x, 
	    RealArray & xr, MappingParameters & params )
{
  const int tAxis[3][2] = { 1,2, 0,2, 0,1  };

  if( numberOfSidesSpecified==0 || numberOfSidesSpecified % 2 ==1 )
  {
    cout << "TFIMapping::map: Error: There must be 2,4 or 6 sides specified!\n";
    exit(1);    
  }
  
  if( params.coordinateType != cartesian )
  {
    cerr << "TFIMapping::map - coordinateType != cartesian " << endl;
  }
  
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  
  //IF numberOfSidesSpecified==2
  {
    // --------------------------------------------------------------------------------------------------
    //     2 sides specified, 2d or 3d
    // --------------------------------------------------------------------------------------------------
    // evaluate the curves on top and bottom
    RealArray c[2], cr[2], b, br;
    c[0].redim(I,rangeDimension); 
    c[1].redim(I,rangeDimension); 
    if( interpolationType[0]==blend )
    {
      b.redim(I);
    }
    
    if( computeMapDerivative || interpolationType[0]==hermite )
    {
      cr[0].redim(I,rangeDimension,domainDimension-1);
      cr[1].redim(I,rangeDimension,domainDimension-1);
      if( interpolationType[0]==blend )
      {
        br.redim(I);
      }
    }
    int axisN;            // normal direction (interpolate in this direction)
    if( curve[0][0]!=NULL )
      axisN=0;  
    else if( curve[0][1]!=NULL )
      axisN=1;  
    else 
      axisN=2;  
    
    RealArray rI(I,1);                  // interpolate along this axis
    if( getIsPeriodic(axisN)==functionPeriodic )
      rI(I,0)=fmod(r(I,axisN)+1.,1.);
    else
      rI(I,0)=r(I,axisN);
    
    RealArray rT(I,domainDimension-1);                 // evaluate curve at these values.
    for( int axis=0; axis<domainDimension-1; axis++ )
    {
      if( getIsPeriodic(tAxis[axisN][axis])==functionPeriodic )
        rT(I,axis)=fmod(r(I,tAxis[axisN][axis])+1.,1.);
      else
        rT(I,axis)=r(I,tAxis[axisN][axis]);
   }
    
    
    curve[0][axisN]->mapS(rT,c[0],cr[0]);
    if( !flip(1,0) ) // ***AP***
      curve[1][axisN]->mapS(rT,c[1],cr[1]);
    else
    {
      curve[1][axisN]->mapS(evaluate(1.-rT),c[1],cr[1]);
      if( computeMapDerivative )
        cr[1]=-cr[1];
    }

    if( interpolationType[0]==blend )
    { // use a blending function
      assert( blendingFunction!=NULL );
      blendingFunction->mapS(rI,b,br);
    }
    
    // cDot[i] equals the r_1 derivative of the patch at the boundary i, i=0,1
    RealArray cDot[2];
    if( interpolationType[0]==hermite )
    { // scale derivatives to an appropriate length
      RealArray scale(I);
      for( int s=0; s<numberOfSidesSpecified; s++ )
      {
        cDot[s].redim(I,domainDimension);
        if( domainDimension==2 )
	{
          scale(I,0)=SQRT( 
            (SQR(c[1](I,0)-c[0](I,0))+SQR(c[1](I,1)-c[0](I,1)))/
            (SQR(cr[s](I,0))+SQR(cr[s](I,1)))
            );
          cDot[s](I,0)=-cr[s](I,1)*scale(I,0); 
          cDot[s](I,1)=+cr[s](I,0)*scale(I,0);
        }
	else
	{
          {throw "error";}
        }
      }
    }
    // interpolate between the curves
    if( computeMap )
    {
      if( interpolationType[0]==linear )
      {
        for( int axis=0; axis<rangeDimension /*domainDimension*/; axis++ ) // rangedim
          x(I,axis)=c[0](I,axis)*(1.-rI)+c[1](I,axis)*rI;
      }
      else if( interpolationType[0]==hermite )
      {
        for( int axis=0; axis<rangeDimension/*domainDimension*/; axis++ ) // rangedim
	{
          x(I,axis)=
             (c[0](I,axis)*(1.+2.*rI)+cDot[0](I,axis)*rI)*SQR(1.-rI)
            +(c[1](I,axis)*(3.-2.*rI)+cDot[1](I,axis)*(rI-1.))*SQR(rI);
        }
      }
      else if( interpolationType[0]==blend )
      { // use a blending function
        for( int axis=0; axis<rangeDimension/*domainDimension*/; axis++ ) // rangedim
          x(I,axis)=c[0](I,axis)*(1.-b(I))+c[1](I,axis)*b(I);
      }
    }//end of if(computeMap)
    
    if( computeMapDerivative )
    {
      if( interpolationType[0]==linear )
      {
        for( int axis=0; axis<rangeDimension/*domainDimension*/; axis++ )
	{
          for( int dir=0; dir<domainDimension-1; dir++ )
            xr(I,axis,tAxis[axisN][dir])=cr[0](I,axis,dir)*(1.-rI)+cr[1](I,axis,dir)*rI; // tangential derivatives
          xr(I,axis,axisN)=c[1](I,axis,0)-c[0](I,axis,0);
        }
      }
      else if( interpolationType[0]==hermite )
      {
        RealArray cDotr[2];
        cDotr[0].redim(I,1,1);
        cDotr[1].redim(I,1,1);

        RealArray crr[2];
        crr[0].redim(I,2);
        crr[1].redim(I,2);
	
        for( int axis=0; axis<rangeDimension/*domainDimension*/; axis++ )
	{
          xr(I,axis,axisN)=
             (c[0](I,axis,0)*(  -6.*rI)+cDot[0](I,axis,0)*(1.-3.*rI))*(1.-rI)
            +(c[1](I,axis,0)*(6.-6.*rI)+cDot[1](I,axis,0)*(   3.*rI-2.))* rI;

          // compute r-derivative of the cDot along "axis"
          // compute the second derivatives by differences
          int i,dir;
          for( dir=0; dir<2; dir++ )
	  {
            curve[0][axisN]->secondOrderDerivative(I, r,crr[0],dir,0);
            curve[1][axisN]->secondOrderDerivative(I, r,crr[1],dir,0);
          }
	  
          for( i=0; i<2; i++ )
	  {
            cDotr[i](I,0,0)= 
              ( (c[1](I,0,0)-c[0](I,0,0))*(cr[1](I,0,0)-cr[0](I,0,0))
                +(c[1](I,1,0)-c[0](I,1,0))*(cr[1](I,1,0)-cr[0](I,1,0)) )*cDot[i](I,axis,0)
              /(SQR(c[1](I,0,0)-c[0](I,0,0))+SQR(c[1](I,1,0)-c[0](I,1,0)))

              -( cDot[i](I,1,0)*crr[i](I,1,0) + cDot[i](I,0,0)*crr[i](I,0,0) )*cr[i](I,axis,0)
              / (SQR(cr[i](I,0,0))+SQR(cr[i](I,1,0))) ;
          }

          if( (debug & 8) && I.length()>1 )
	  {
            // printf(" *** axis=%i ******",axis);
            r(I,0).display(" r(I,0)");
            RealArray cDot0; cDot0=(1.-r(I,0)+SQR(r(I,0)))*(2.*r(I,0)-1.)/SQRT(SQR(1.-2.*r(I,0))+1.);
            cDot[0](I,0,0).display("Here is cDot[0](I,0,0)");
            (cDot[0](I,0,0)-cDot0).display("Here is the error in cDot[0]");

            RealArray cDot1; cDot1=(1.-r(I,0)+SQR(r(I,0)))/SQRT(SQR(1.-2.*r(I,0))+1.);
            cDot[0](I,1,0).display("Here is cDot[0](I,1,0)");
            (cDot[0](I,1,0)-cDot1).display("Here is the error in cDot[0](I,1,0)");
            // *** for axis==0 ***
            real h=pow(REAL_EPSILON/20.,1./3.); 
            cDot0= (
              (1.-(r(I,0)+h)+SQR(r(I,0)+h))*(2.*(r(I,0)+h)-1.)/SQRT(SQR(1.-2.*(r(I,0)+h))+1.)
              - (1.-(r(I,0)-h)+SQR(r(I,0)-h))*(2.*(r(I,0)-h)-1.)/SQRT(SQR(1.-2.*(r(I,0)-h))+1.)
              )/(2.*h);
            cDotr[0].display("Here is cDotr[0]");
            cDot0.display("Here is the true cDotr");
            (cDotr[0](I,0,0)-cDot0).display("Here is the error in cDotr[0]");
          }

          for( dir=0; dir<domainDimension-1; dir++ )
	  {
            xr(I,axis,tAxis[axisN][dir])=
              (cr[0](I,axis,dir)*(1.+2.*rI)+cDotr[0](I,0,dir)*rI)*SQR(1.-rI) +
              (cr[1](I,axis,dir)*(3.-2.*rI)+cDotr[1](I,0,dir)*(rI-1.))*SQR(rI);
          }

        }
      }
      else if( interpolationType[0]==blend )
      {
        for( int axis=0; axis<rangeDimension/*domainDimension*/; axis++ )
	{
          for( int dir=0; dir<domainDimension-1; dir++ )
	  {
            xr(I,axis,tAxis[axisN][dir])=cr[0](I,axis,dir)*(1.-b(I))+cr[1](I,axis,dir)*b(I);
          }
          xr(I,axis,axisN)=(c[1](I,axis,0)-c[0](I,axis,0))*br(I);
        }
      }
    }
  }
}

void TFIMapping::
mapNumberOfSides4AndDim2( const RealArray & r, RealArray & x, 
	    RealArray & xr, MappingParameters & params )
{
  int axis,dir;

  if( numberOfSidesSpecified==0 || numberOfSidesSpecified % 2 ==1 )
  {
    cout << "TFIMapping::map: Error: There must be 2,4 or 6 sides specified!\n";
    exit(1);    
  }
  
  if( params.coordinateType != cartesian )
  {
    cerr << "TFIMapping::map - coordinateType != cartesian " << endl;
  }
  
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  {
    //    RealArray c[2][2], cr[2][2]; *wdh* sgi compiler chokes on multi-d arrays of A++ arrays

#define C(side,axis) c[(side)+2*(axis)]
#define CR(side,axis) cr[(side)+2*(axis)]


    // --------------------------------------------------------------------------------------------------
    // 2-d TFI with all four sides of domain specified
    // --------------------------------------------------------------------------------------------------
    //
    // define arrays to hold the values of the boundary curves and blending functions.
    // c[i] is a VECTOR defining the tranformation along side i with rangeDimension components.
    // cr[i] is the matrix of derivatives of the mappings with respect to the parameter space variables.
    RealArray c[4], cr[4], b[2], br[2];

    // evaluate the mapping and its derivatives on the boundaries
    for(axis=0; axis<domainDimension; axis++)
    {
      if( interpolationType[axis] == blend)
        b[axis].redim(I);

      RealArray rI = r(I,1-axis); // tangential direction.
      rI.setBase(0,1);
      for( int side=0; side<=1; side++ )
      {
        C(side,axis).redim(I,rangeDimension);     // set size for array holding boundary curve points
        if( computeMapDerivative || (interpolationType[axis]==hermite) )
          CR(side,axis).redim(I,rangeDimension,domainDimension-1);

	if (flip(side,axis) == 0)
	  curve[side][axis]->mapS(rI,C(side,axis),CR(side,axis));
	else
	{
	  curve[side][axis]->mapS(evaluate(1.-rI),C(side,axis),CR(side,axis));
	  if (computeMapDerivative)
	    CR(side,axis) = -CR(side,axis);
	}
	
      }
    }    

    //
    // fill in blending function (not supported yet.)
    //
//     for(i=0;i<2;i++){
//       if( interpolationType[i]==blend ){ // use a blending function
//         assert( blendingFunction!=NULL );
//         RealArray rI;
//         rI.reference(r(I,axisI));  // need base 0
//         rI.setBase(0,axis2);
//         blendingFunction->mapS(rI,b,br);
//       }
//     }

    //
    // do the interpolation
    //
    if(computeMap)
    {
      // loop over each direction, computing the interpolation along the normalDir[dir]
      // axis between the boundary curves
      x = 0.0;
      for( dir = 0 ; dir < domainDimension ; dir++)
      {
        // the boundary curves bounding the domain in this direction are c[2*dir] and c[2*dir+1]
        // the axis along which the interpolation occurs (perpindicular to c[i] and c[i+1]) 
        const RealArray rI = r(I,dir); // normal direction (interpolation direction)

        if(interpolationType[dir]==linear)
	{
          for( axis=0; axis<rangeDimension/*domainDimension*/; axis++ )
            x(I,axis) += C(1,dir)(I,axis)*rI + C(0,dir)(I,axis)*(1.0 - rI);
        }
	else if( interpolationType[dir] == hermite)
	{
          // first compute the derivatives of the boundary maps (in the same direction as the interpolation, 
          // or normal to the boundary curve)
          //
          // The funny multiplication by pow(-1.0,(double(dir))) is because the normal to the bottom and top curves is
          // (- cr[i](I,axis2), cr[i](I,axis1) ) while the normal to the left and right curves is
          // ( cr[i](I,axis2), -cr[i](I,axis1) )
          //
          RealArray dCdn[2];
          dCdn[0].redim(I,domainDimension);
          dCdn[1].redim(I,domainDimension);
            
          if(domainDimension == 2)
	  {
            dCdn[axis1] = pow(-1,dir+1) * CR(0,dir)(I,axis2);
            dCdn[axis2] = pow(-1,dir  ) * CR(0,dir)(I,axis1);
          }
          // now compute the contribution to the intpolation from the
          // hermite interpolation in this direction (without the
          // correction term added below)
          for( axis=0; axis<rangeDimension/*domainDimension*/; axis++ )
            x(I,axis) +=
              (1.0 + 2.0*rI)*C(0,dir)(I,axis) + (3.0 + 2.0*rI)*C(1,dir)(I,axis)
              +SQR(1.0-rI)*rI*dCdn[0](I,axis)+SQR(rI)*(rI-1.0)*dCdn[1](I,axis);
        }
	else
	{
          cout<<"Unsupported interpolation."<<endl;
          {throw("error");}
        }
      }//end of loop over directions
      // now compute the correction term needed to enforce compatability at the corners
      // in linear, blending, spline, or hermite interpolation, the following
      // term is subtracted from x.  hermite interpolation has additional
      // terms that will be computed later
      for(axis=0; axis<rangeDimension/*domainDimension*/; axis++)
      {
        if( domainDimension == 2 )
	{
          if( (interpolationType[0]==linear)&&(interpolationType[1]==linear) )
	  {
	    x(I,axis) -=
	      (1.-r(I,axis1))*( (1.-r(I,axis2))*corner(axis,0,0) + r(I,axis2)*corner(axis,0,1) ) +
	          r(I,axis1) *( (1.-r(I,axis2))*corner(axis,1,0) + r(I,axis2)*corner(axis,1,1) );
          }
	  else if( (interpolationType[0]==linear)&&(interpolationType[1]==hermite) )
	  {
            // interpolation between bottom and top = linear
            // interpolation between left and right = hermite
          }
	  else if( (interpolationType[0]==hermite)&&(interpolationType[1]==linear) )
	  {
          }
	  else if( (interpolationType[0]==hermite)&&(interpolationType[1]==hermite) )
	  {
          }//end of various mixtures of interpolation types
        }//end of 2-d correction
      }//end of loop over components
    }//end of if(computeMap)
    
    //
    // compute the derivative of the mapping.
    //
    if(computeMapDerivative)
    {
      for( axis=0; axis<rangeDimension/*domainDimension*/; axis++ )
      {
        //dx/dr(1)
        xr(I,axis,axis1) =    C(1,0)(I,axis)  - C(0,0)(I,axis)
          + (1.0 - r(I,axis2))*CR(0,1)(I,axis,axis1) + r(I,axis2)*CR(1,1)(I,axis,axis1)
          + (1.0-r(I,axis2))*( corner(axis,0,0)-corner(axis,1,0) )
          +      r(I,axis2) *( corner(axis,0,1)-corner(axis,1,1) );
        // dx/dr(2)
        xr(I,axis,axis2) = C(1,1)(I,axis) - C(0,1)(I,axis)
          +(1.0-r(I,axis1))*CR(0,0)(I,axis,axis1) + r(I,axis1)*CR(1,0)(I,axis,axis1)
          +(1.0-r(I,axis1))*( corner(axis,0,0)-corner(axis,0,1) )
          +     r(I,axis1) *( corner(axis,1,0)-corner(axis,1,1) );
          
      }//end of loop over components
    }//end of if(computeMapDerivative)
  }// end of if(numberOfSidesSpecified==4)

}



void TFIMapping::
mapNumberOfSides4AndDim3( const RealArray & r, RealArray & x, 
	    RealArray & xr, MappingParameters & params )
{

  int axis;
  const int tAxis[3][2] = { 1,2, 0,2, 0,1  };

  if( numberOfSidesSpecified==0 || numberOfSidesSpecified % 2 ==1 )
  {
    cout << "TFIMapping::map: Error: There must be 2,4 or 6 sides specified!\n";
    exit(1);    
  }
  
  if( params.coordinateType != cartesian )
  {
    cerr << "TFIMapping::map - coordinateType != cartesian " << endl;
  }
  
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  
    {
    // --------------------------------------------------------------------------------------------------
    // 3D TFI with four sides of domain specified
    // --------------------------------------------------------------------------------------------------
//    RealArray edge[2][2], edger[2][2];
#define EDGE(side,axis) edge[(side)+2*(axis)]
#define EDGER(side,axis) edger[(side)+2*(axis)]

    RealArray c[4], cr[4];
    RealArray edge[4], edger[4];  // arrays to hold edge corrections
    RealArray rI(I,2);
    int cAxis[3];
    // evaluate the mapping and its derivatives on the boundaries
    // curves 0,1 : are functions of r_2, r_3   (r_1=0 or r_1=1)
    // curves 2,3 : are functions of r_1, r_3
    // curves 4,5 : are functions of r_1, r_2
    int i=0;
    for( axis=0; axis<3; axis++ )
    {
      if( curve[0][axis]!=NULL )
      {
        cAxis[i]=axis;               // save normal directions
        for( int dir=0; dir<=1; dir++ )
	{
	  if( getIsPeriodic(tAxis[axis][dir])==functionPeriodic )
  	    rI(I,dir)=fmod(r(I,tAxis[axis][dir])+1.,1.);
          else 
  	    rI(I,dir)=r(I,tAxis[axis][dir]);
	}
        for( int side=0; side<=1; side++ )
	{
	  C(side,i).redim(I,rangeDimension);
	  if( computeMapDerivative || (interpolationType[i]==hermite) )
	    CR(side,i).redim(I,rangeDimension,domainDimension-1);
	  curve[side][axis]->mapS(rI,C(side,i),CR(side,i));
	}
        i++;
        if( i==2 )
	{ // compute the edge curves 
          int axisE= 
            cAxis[0]==0 && cAxis[1]==1 ? 0 :
	    cAxis[0]==1 && cAxis[1]==2 ? 1 :  
 	                                 0 ;    // cAxis[0]=0 && cAxis[1]=2
	  for( int side=0; side<=1; side++ )
	  {
            rI(I,axisE)=(real)side;
	    EDGE(0,side).redim(I,rangeDimension);
	    EDGE(1,side).redim(I,rangeDimension);
	    if( computeMapDerivative )
	    {
	      EDGER(0,side).redim(I,rangeDimension,domainDimension-1);
	      EDGER(1,side).redim(I,rangeDimension,domainDimension-1);
	    }
	    curve[0][axis]->mapS(rI,EDGE(0,side),EDGER(0,side));
	    curve[1][axis]->mapS(rI,EDGE(1,side),EDGER(1,side));
	  }
	}
      }
    }    

    const RealArray & n1 = r(I,cAxis[0]);               // first normal direction
    const RealArray & oneMinusN1 = evaluate(1.-n1);
    const RealArray & n2 = r(I,cAxis[1]);               // second normal direction
    const RealArray & oneMinusN2 = evaluate(1.-n2);

    if( computeMap )
    {
      if( interpolationType[0]==linear && interpolationType[1]==linear && interpolationType[2]==linear )
      {
	for( int axis=0; axis<domainDimension; axis++ )
	{
	  // Basic Interpolation -( edge corrections ) + (vertex corrections)
	  x(I,axis)=
             C(0,0)(I,axis)*oneMinusN1+C(1,0)(I,axis)*n1
	    +C(0,1)(I,axis)*oneMinusN2+C(1,1)(I,axis)*n2
	    -(
	      +oneMinusN2*( oneMinusN1*EDGE(0,0)(I,axis) + n1*EDGE(0,1)(I,axis))      
	      +        n2*( oneMinusN1*EDGE(1,0)(I,axis) + n1*EDGE(1,1)(I,axis))
	      );
        }
      }
      else
      {
	cout<< "TFIMapping::ERROR: Unsupported interpolation."<<endl;
	throw("error");
      }
    }
    
    if(computeMapDerivative)
    {
      int axisT= cAxis[0]==0 && cAxis[1]==1 ? 2 : 
                 cAxis[0]==1 && cAxis[1]==2 ? 0 :
                                              1;
      int dir1 = tAxis[cAxis[0]][0]==cAxis[1] ? 0 : 1;
      int dir2 = tAxis[cAxis[1]][0]==cAxis[0] ? 0 : 1;
      
      if( interpolationType[0]==linear && interpolationType[1]==linear && interpolationType[2]==linear )
      {
	for( int axis=0; axis<domainDimension; axis++ )
	{
//   const int tAxis[3][2] = { 1,2, 0,2, 0,1  };
	  // d/d(n1)
	  xr(I,axis,cAxis[0])=
                        C(1,0)(I,axis)              -C(0,0)(I,axis)       
                      +CR(0,1)(I,axis,dir2)*oneMinusN2+CR(1,1)(I,axis,dir2)*n2 
               -(
	        oneMinusN2*( EDGE(0,1)(I,axis)-EDGE(0,0)(I,axis) )
		       +n2*( EDGE(1,1)(I,axis)-EDGE(1,0)(I,axis) )
		);
	  xr(I,axis,axisT)=
	               CR(0,0)(I,axis,1-dir1)*oneMinusN1+CR(1,0)(I,axis,1-dir1)*n1 
                      +CR(0,1)(I,axis,1-dir2)*oneMinusN2+CR(1,1)(I,axis,1-dir2)*n2 
               -(
	       +oneMinusN2*( oneMinusN1*EDGER(0,0)(I,axis,0) + n1*EDGER(0,1)(I,axis,0))
		       +n2*( oneMinusN1*EDGER(1,0)(I,axis,0) + n1*EDGER(1,1)(I,axis,0))
		);
	  // d/d(n2)
	  xr(I,axis,cAxis[1])=
	               CR(0,0)(I,axis,dir1)*oneMinusN1+CR(1,0)(I,axis,dir1)*n1  
                      + C(1,1)(I,axis)              -C(0,1)(I,axis)
               -(
	        oneMinusN1*( EDGE(1,0)(I,axis)-EDGE(0,0)(I,axis) )
                      + n1*( EDGE(1,1)(I,axis)-EDGE(0,1)(I,axis) )
		);
	}
      }
      else
      {
	cout<< "TFIMapping::ERROR: Unsupported interpolation."<<endl;
	throw("error");
      }
    }   // end of if(computeMapDerivative)
  }
}



void TFIMapping::
mapNumberOfSides6( const RealArray & r, RealArray & x, 
	    RealArray & xr, MappingParameters & params )
{

  int axis;
  const int tAxis[3][2] = { 1,2, 0,2, 0,1  };

  if( numberOfSidesSpecified==0 || numberOfSidesSpecified % 2 ==1 )
  {
    cout << "TFIMapping::map: Error: There must be 2,4 or 6 sides specified!\n";
    exit(1);    
  }
  
  if( params.coordinateType != cartesian )
  {
    cerr << "TFIMapping::map - coordinateType != cartesian " << endl;
  }
  
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  {
    // --------------------------------------------------------------------------------------------------
    // 3D TFI with all six sides of domain specified
    // --------------------------------------------------------------------------------------------------

//    RealArray edge[2][3][2], edger[2][3][2];
#undef EDGE
#undef EDGER
#define EDGE(side,axis,dir) edge[(side)+2*(axis+3*(dir))]
#define EDGER(side,axis,dir) edger[(side)+2*(axis+3*(dir))]

    RealArray c[6], cr[6];
    RealArray edge[12], edger[12];  // arrays to hold edge corrections
    // evaluate the mapping and its derivatives on the boundaries
    // curves 0,1 : are functions of r_2, r_3
    // curves 2,3 : are functions of r_1, r_3
    // curves 4,5 : are functions of r_1, r_2
    RealArray rI(I,2);
    for( axis=0; axis<3; axis++ )
    {
      for( int dir=0; dir<=1; dir++ )
      {
	if( getIsPeriodic(tAxis[axis][dir])==functionPeriodic )
	  rI(I,dir)=fmod(r(I,tAxis[axis][dir])+1.,1.);
	else 
	  rI(I,dir)=r(I,tAxis[axis][dir]);
      }
      int side;
      for( side=0; side<=1; side++ )
      {
	C(side,axis).redim(I,rangeDimension);
	if( computeMapDerivative || (interpolationType[axis]==hermite) )
	  CR(side,axis).redim(I,rangeDimension,domainDimension-1);

	curve[side][axis]->mapS(rI,C(side,axis),CR(side,axis));
      }
      // Evaluate the edge corrections. There are 12 edges.
      for( side=0; side<=1; side++ )
      {
        for( int dir=0; dir<=1; dir++ )
	{
          if( axis==0 )
      	    rI(I,0)=(real)dir;
          else if( axis==1 )
      	    rI(I,1)=(real)dir;
          else
      	    rI(I,0)=(real)dir;
	  EDGE(side,axis,dir).redim(I,rangeDimension);
	  if( computeMapDerivative )
	    EDGER(side,axis,dir).redim(I,rangeDimension,domainDimension-1);
	  curve[side][axis]->mapS(rI,EDGE(side,axis,dir),EDGER(side,axis,dir));
	}
      }
    }    

    const RealArray & r1 = r(I,axis1);
    const RealArray & oneMinusR1 = evaluate(1.-r1);
    const RealArray & r2 = r(I,axis2);
    const RealArray & oneMinusR2 = evaluate(1.-r2);
    const RealArray & r3 = r(I,axis3);
    const RealArray & oneMinusR3 = evaluate(1.-r3);

    if( computeMap )
    {
      if( interpolationType[0]==linear && interpolationType[1]==linear && interpolationType[2]==linear )
      {
	for( int axis=0; axis<domainDimension; axis++ )
	{
	  // Basic Interpolation -( edge corrections ) + (vertex corrections)
	  x(I,axis)=
	    +C(0,0)(I,axis)*oneMinusR1+C(1,0)(I,axis)*r1    // c(r_2,r_3)
	    +C(0,1)(I,axis)*oneMinusR2+C(1,1)(I,axis)*r2    // c(r_1,r_3)
            +C(0,2)(I,axis)*oneMinusR3+C(1,2)(I,axis)*r3    // c(r_1,r_2)
	    -(
	       oneMinusR1*( r2*EDGE(0,0,1)(I,axis) + oneMinusR2*EDGE(0,0,0)(I,axis) )   //  edge(r3)
	      +        r1*( r2*EDGE(1,0,1)(I,axis) + oneMinusR2*EDGE(1,0,0)(I,axis))   //  
	      +oneMinusR2*( r3*EDGE(0,1,1)(I,axis) + oneMinusR3*EDGE(0,1,0)(I,axis))   //  edge(r1)
	      +        r2*( r3*EDGE(1,1,1)(I,axis) + oneMinusR3*EDGE(1,1,0)(I,axis))   //  
	      +oneMinusR3*( r1*EDGE(0,2,1)(I,axis) + oneMinusR1*EDGE(0,2,0)(I,axis))   //  edge(r2)
	      +        r3*( r1*EDGE(1,2,1)(I,axis) + oneMinusR1*EDGE(1,2,0)(I,axis))   //  
	      )
	    +(
	       oneMinusR1*(oneMinusR2*( oneMinusR3*corner(axis,0,0,0)+r3*corner(axis,0,0,1))
	      +                    r2*( oneMinusR3*corner(axis,0,1,0)+r3*corner(axis,0,1,1)))
	      +        r1*(oneMinusR2*( oneMinusR3*corner(axis,1,0,0)+r3*corner(axis,1,0,1))
	      +                    r2*( oneMinusR3*corner(axis,1,1,0)+r3*corner(axis,1,1,1)))
	      );
        }
      }
      else
      {
	cout<< "TFIMapping::ERROR: Unsupported interpolation."<<endl;
	throw("error");
      }
    }
    
    if(computeMapDerivative)
    {
      if( interpolationType[0]==linear && interpolationType[1]==linear && interpolationType[2]==linear )
      {
	for( int axis=0; axis<domainDimension; axis++ )
	{
	  xr(I,axis,axis1)=
                        C(1,0)(I,axis)      -  C(0,0)(I,axis)        
  	              +CR(1,1)(I,axis,0)*r2 + CR(0,1)(I,axis,0)*oneMinusR2
                      +CR(1,2)(I,axis,0)*r3 + CR(0,2)(I,axis,0)*oneMinusR3
               -(
                oneMinusR2*(     EDGE(1,0,0)(I,axis)-EDGE(0,0,0)(I,axis) )
                       +r2*(     EDGE(1,0,1)(I,axis)-EDGE(0,0,1)(I,axis) )
	       +oneMinusR2*( r3*EDGER(0,1,1)(I,axis,0) + oneMinusR3*EDGER(0,1,0)(I,axis,0) )
		       +r2*( r3*EDGER(1,1,1)(I,axis,0) + oneMinusR3*EDGER(1,1,0)(I,axis,0) )
	       +oneMinusR3*(     EDGE(0,2,1)(I,axis)-EDGE(0,2,0)(I,axis) )
		       +r3*(     EDGE(1,2,1)(I,axis)-EDGE(1,2,0)(I,axis) )
		)
              +(
                 oneMinusR2*( oneMinusR3*(corner(axis,1,0,0)-corner(axis,0,0,0))
                                     +r3*(corner(axis,1,0,1)-corner(axis,0,0,1)) )
                        +r2*( oneMinusR3*(corner(axis,1,1,0)-corner(axis,0,1,0))
                                     +r3*(corner(axis,1,1,1)-corner(axis,0,1,1)) )
		);
	    xr(I,axis,axis2)=
	               CR(1,0)(I,axis,0)*r1 + CR(0,0)(I,axis,0)*oneMinusR1
  	              + C(1,1)(I,axis)      -  C(0,1)(I,axis) 	    
                      +CR(1,2)(I,axis,1)*r3 + CR(0,2)(I,axis,1)*oneMinusR3
               -(
	        oneMinusR1*( EDGE(0,0,1)(I,axis)-EDGE(0,0,0)(I,axis) )
		       +r1*( EDGE(1,0,1)(I,axis)-EDGE(1,0,0)(I,axis) )
	       +oneMinusR3*( EDGE(1,1,0)(I,axis)-EDGE(0,1,0)(I,axis) ) 
                       +r3*( EDGE(1,1,1)(I,axis)-EDGE(0,1,1)(I,axis) )
	       +oneMinusR3*( r1*EDGER(0,2,1)(I,axis,1) + oneMinusR1*EDGER(0,2,0)(I,axis,1) )
		       +r3*( r1*EDGER(1,2,1)(I,axis,1) + oneMinusR1*EDGER(1,2,0)(I,axis,1) )
		)
              +(
                oneMinusR1*(oneMinusR3*(corner(axis,0,1,0)-corner(axis,0,0,0))
                                   +r3*(corner(axis,0,1,1)-corner(axis,0,0,1)))
                       +r1*(oneMinusR3*(corner(axis,1,1,0)-corner(axis,1,0,0))
                                   +r3*(corner(axis,1,1,1)-corner(axis,1,0,1)))
		);
	    xr(I,axis,axis3)=
	               CR(1,0)(I,axis,1)*r1 + CR(0,0)(I,axis,1)*oneMinusR1
  	              +CR(1,1)(I,axis,1)*r2 + CR(0,1)(I,axis,1)*oneMinusR2
                      + C(1,2)(I,axis)      -  C(0,2)(I,axis)
               -(
		oneMinusR1*( r2*EDGER(0,0,1)(I,axis,1) + oneMinusR2*EDGER(0,0,0)(I,axis,1) )
		       +r1*( r2*EDGER(1,0,1)(I,axis,1) + oneMinusR2*EDGER(1,0,0)(I,axis,1) )
	       +oneMinusR2*( EDGE(0,1,1)(I,axis)-EDGE(0,1,0)(I,axis) )
		       +r2*( EDGE(1,1,1)(I,axis)-EDGE(1,1,0)(I,axis) )
	       +oneMinusR1*( EDGE(1,2,0)(I,axis)-EDGE(0,2,0)(I,axis) )
                      + r1*( EDGE(1,2,1)(I,axis)-EDGE(0,2,1)(I,axis) )
		)
              +(
                oneMinusR1*(oneMinusR2*( corner(axis,0,0,1)-corner(axis,0,0,0) )
                                   +r2*( corner(axis,0,1,1)-corner(axis,0,1,0) ))
                       +r1*(oneMinusR2*( corner(axis,1,0,1)-corner(axis,1,0,0) )
                                   +r2*( corner(axis,1,1,1)-corner(axis,1,1,0) ))
		);
	  
	}
      }
      else
      {
	cout<< "TFIMapping::ERROR: Unsupported interpolation."<<endl;
	throw("error");
      }
    }   // end of if(computeMapDerivative)
  }
  

#undef C
#undef CR
#undef EDGE
#undef EDGER
    

}


//=================================================================================
// get a mapping from the database
//=================================================================================
int TFIMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering TFIMapping::get" << endl;
  subDir.setMode(GenericDataBase::streamInputMode);
  int i;
  subDir.get( TFIMapping::className,"className" ); 
  if( TFIMapping::className != "TFIMapping" )
  {
    cout << "TFIMapping::get ERROR in className!" << endl;
  }
  //  int temp;
  subDir.get((int*)interpolationType,"interpolationType",3);
  //  for( i=0;i<3;i++)
  //    subDir.get(temp,"interpolationType"); interpolationType[i]=(InterpolationType)temp;
  
  subDir.get(numberOfSidesSpecified,"numberOfSidesSpecified");
    // get the curves that we use 
  char buff[40];
  aString curveClassName;

  i=0;
  for( int axis=0; axis<3; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      curve[side][axis]=NULL;

      int curveExists;
      subDir.get(curveExists,sPrintF(buff,"curve%i%iExists",side,axis));
      assert( curveExists==0 || curveExists==1 );
      if( curveExists )
      { 
        i++;
	subDir.get(curveClassName,sPrintF(buff,"curve%i%iClassName",side,axis));
// makeMapping does new a Mapping -- reference count incremented below, object deleted in destructor
	curve[side][axis] = Mapping::makeMapping( curveClassName ); 
	if( curve[side][axis]==NULL )
	{
	  cout << "TFIMapping::get:ERROR unable to make the mapping with className = " 
	       << (const char *)curveClassName << endl;
          {throw "error";}
	}
	else
	{
	  curve[side][axis]->incrementReferenceCount();
	}
	
	curve[side][axis]->get( subDir,sPrintF(buff,"curve%i%i",side,axis) );
      }
    }
  }
  if( i!=numberOfSidesSpecified )
  {
    cout << "TFIMapping::get:ERROR unable to find all the curves associated with this Mapping!\n";
    {throw "error";}
  }

  int blendingFunctionExists;
  subDir.get(blendingFunctionExists,sPrintF(buff,"blendingFunctionExists"));
  assert( blendingFunctionExists==0 || blendingFunctionExists==1 );
  if( blendingFunctionExists )
  { 
    subDir.get(curveClassName,"curveClassName");
// makeMapping does new a Mapping -- reference count incremented below, object deleted in destructor
    blendingFunction = Mapping::makeMapping( curveClassName ); 
    if( blendingFunction==NULL )
    {
      cout << "TFIMapping::get:ERROR unable to make the blending function mapping with className = " 
           << (const char *)curveClassName << endl;
      return 1;
    }
    else
    {
      blendingFunction->incrementReferenceCount();
    }
    blendingFunction->get( subDir,sPrintF(buff,"blendingFunction") );
  }
  subDir.get(flip,"flip");
  subDir.get(corner,"corner");

  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();

  delete &subDir;

  return 0;
}

int TFIMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  
    // save the mapping as a stream of data by default, this is more efficient
  subDir.setMode(GenericDataBase::streamOutputMode);
  int i;
  subDir.put( TFIMapping::className,"className" );
  subDir.put( (int*)interpolationType, "interpolationType",3);
  //  for( i=0;i<3;i++){
  //    subDir.put((int)interpolationType[i],"interpolationType");
  //  }
  
  subDir.put(numberOfSidesSpecified,"numberOfSidesSpecified");

    // save the curves that we use *** this could be wasteful is they are already saved ****
  char buff[40];

  for( int axis=0; axis<3; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      int curveExists=   curve[side][axis]!=NULL ? 1 : 0;
      subDir.put(curveExists,sPrintF(buff,"curve%i%iExists",side,axis));
      if( curveExists )
      {
	subDir.put(curve[side][axis]->getClassName(),sPrintF(buff,"curve%i%iClassName",side,axis));
	curve[side][axis]->put( subDir,sPrintF(buff,"curve%i%i",side,axis) );
      }
    }
  }
  
  int blendingFunctionExists=   blendingFunction!=NULL ? 1 : 0;
  subDir.put(blendingFunctionExists,sPrintF(buff,"blendingFunctionExists"));
  if( blendingFunctionExists )
  {
    subDir.put(blendingFunction->getClassName(),"curveClassName");
    blendingFunction->put( subDir,sPrintF(buff,"blendingFunction") );
  }
  subDir.put(flip,"flip");
  subDir.put(corner,"corner");

  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

Mapping *TFIMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==TFIMapping::className )
    retval = new TFIMapping();
  return retval;
}

    

int TFIMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the TFI mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
  {
      "!TFIMapping",
    ">choose curves for sides",
      "choose left curve   (r_1=0)",
      "choose right curve  (r_1=1)",
      "choose bottom curve (r_2=0)",
      "choose top curve    (r_2=1)",
      "choose back curve   (r_3=0)",
      "choose front curve  (r_3=1)",
    "<>interpolation type",
       "hermite interpolation",
       "linear interpolation",     
       "blending function interpolation",
       "specify type in each direction",
    "<flip to make non-singular",
    "axes orientation",
    "lines",
    "boundary conditions",
    "share",
    "mappingName",
    "periodicity",
    "show parameters",
    "plot",
    "help",
    "exit", 
    "" 
  };
  aString help[] = 
  {
    "choose curves for sides: choose curves for the sides. Choose 2, 4 or 6 curves.",
    "interpolationType  : use linear or hermite interpolation, or a blending function",
    "lines              : specify number of grid lines",
    "boundary conditions: specify boundary conditions",
    "share              : specify share values for sides",
    "mappingName        : specify the name of this mapping",
    "periodicity        : specify periodicity in each direction",
    "show parameters    : print current values for parameters",
    "plot               : enter plot menu (for changing ploting options)",
    "help               : Print this list",
    "exit               : Finished with changes",
    "" 
  };

  aString answer,line,answer2; 

  bool plotObject=FALSE;
  numberOfSidesSpecified=0;
  for( int axis=0; axis<3; axis++ )
    for( int side=0; side<=1; side++ )
      if( curve[side][axis]!=NULL )
	numberOfSidesSpecified++;

  if( numberOfSidesSpecified >0 && numberOfSidesSpecified % 2 == 0 )
  {
    plotObject=TRUE;
  }

  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("TFIMapping>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer(0,5)=="choose" )
    { 
      gi.appendToTheDefaultPrompt("choose side>"); // set the default prompt
      // Make a menu with the Mapping names (only curves or surfaces!)
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+3];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
        MappingRC & map = mapInfo.mappingList[i];
	//kkc 110525        if( map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this )        
	if( map.getDomainDimension()<= (map.getRangeDimension()-1) && map.mapPointer!=this )
        {
          subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
        }
      }
      menu2[j++]="none"; 
      menu2[j++]="done"; 
      menu2[j]="";   // null string terminates the menu
	
      int mapNumber = gi.getMenuItem(menu2,answer2,answer);
      if( answer2=="done" )
      {
      }
      else
      {
	mapNumber=subListNumbering(mapNumber);  // map number in the original list
	int axis,side;
	if( answer=="choose left curve   (r_1=0)" )
	{
	  axis=0; side=0;
	}
	else if( answer=="choose right curve  (r_1=1)" )
	{
	  axis=0;  side=1;
	}
	else if( answer=="choose bottom curve (r_2=0)" )
	{
	  axis=1;  side=0;
	}
	else if( answer=="choose top curve    (r_2=1)" )
	{
	  axis=1;  side=1;
	}
	else if( answer=="choose back curve   (r_3=0)" )
	{
	  axis=2;  side=0;
	}
	else if( answer=="choose front curve  (r_3=1)" )
	{
	  axis=2;  side=1;
	}
	else
	{
	  printf("TFIMapping:ERROR: unknown response: [%s]\n",(const char*)answer);
          gi.stopReadingCommandFile();
	  break;
	}
        if( answer2=="none" )
	{ // remove this curve if there is one there
	  if( curve[side][axis]!=NULL )
	  {
	    numberOfSidesSpecified--;   // adding a new curve.
  	    curve[side][axis]=NULL;
	  }
	}
	else
	{
	  if( curve[side][axis]==NULL )
	    numberOfSidesSpecified++;   // adding a new curve.
          printf("Assigning a curve for (side,axis)=(%i,%i)\n",side,axis);
	  curve[side][axis]=mapInfo.mappingList[mapNumber].mapPointer;
	  curve[side][axis]->incrementReferenceCount();
	}
	delete [] menu2;
	if( numberOfSidesSpecified % 2 == 0 )
	{ // We can plot the tfi mapping now, even if it isn't finished.
	  // Define properties of this mapping
	  // setName(mappingName,"TFI-"+surface->getName(mappingName));
	  setMappingProperties();
	  // call the initialization routine to check consistency of sides and compute
	  // locations of corners
	  initialize();
	  mappingHasChanged(); 
	  plotObject=TRUE;
	}
	else
	{
	  setGridIsValid();
	  plotObject=FALSE;
	}
	gi.unAppendTheDefaultPrompt();  // reset
      }
    }
    else if( answer=="hermite interpolation" )
    {
      for( int dir=0; dir<3; dir++ )
        interpolationType[dir]=hermite;
      mappingHasChanged();
    }
    else if( answer=="linear interpolation" )
    {
      for( int dir=0; dir<3; dir++ )
        interpolationType[dir]=linear;
      mappingHasChanged();
    }
    else if( answer=="blending function interpolation" )
    {
      for( int dir=0; dir<3; dir++ )
      {
        interpolationType[dir]=blend;
      }
      delete blendingFunction;
      blendingFunction = new LineMapping(0.,1.,5);   // blending function is a line for now
      mappingHasChanged();
    }
    else if( answer=="specify type in each direction" )
    {
      //make a menu with all possible directions
      int num=domainDimension;
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int i;
      char buff[80];
      
      for( i=0; i<num; i++ )
      {
        sprintf(buff,"type along direction %i",i);
        menu2[i]+=buff;
      }
      menu2[i++]="none"; 
      menu2[i]="";   // null string terminates the menu
	
      for( i=0; i<num; i++ )
      {
        int dir = gi.getMenuItem(menu2,answer2);
        // for this choice of direction, specify interpolation type
        aString menu[] = {  
          "linear",
          "hermite",
          "blend",
          "no change",
          ""
        }; 
        int response = gi.getMenuItem(menu,answer,"Enter the type of interpolation");   
        if( answer!="" && response >= 0 && response <= 2 )
        {
          interpolationType[dir]=(InterpolationType)response;
          if( interpolationType[dir]==blend )
          {
            delete blendingFunction;
            blendingFunction = new LineMapping(0.,1.,5);   // blending function is a line for now
          }
          mappingHasChanged(); 
        }
      }
      
      delete [] menu2;
      
    }//end of interpolation type 
    else if( answer=="flip to make non-singular" )
    {
      flipper();
    }
    else if( answer=="show parameters" )
    {
      int i;
      for(i=0;i<3;i++){
        printf(" interpolationType[%i] = %s \n",i,(interpolationType[i]==linear ? "linear" : "hermite"));
      }
      
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  if( !gridIsValid() )
    getGrid();
  
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}




























