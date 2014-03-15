#include "TriangleClass.h"
#include "Geom.h"
#include "ArraySimple.h"

Triangle::
Triangle()
//===========================================================================
/// \brief  Default Constructor, make a default triangle with 
///  vertices (0,0,0), (1,0,0), (0,1,0)
//===========================================================================
{
  x1[0]=0.; x1[1]=0.; x1[2]=0.; 
  x2[0]=1.; x2[1]=0.; x2[2]=0.; 
  x3[0]=0.; x3[1]=1.; x3[2]=0.; 
  computeNormal();
}

Triangle::
Triangle( const real x1_[3], const real x2_[3], const real x3_[3] )
//===========================================================================
/// \brief  Create a triangle with vertices x1,x2,x3
/// \param x1,x2,x3 (input) : the three vertices of the triangle
//===========================================================================
{
  setVertices(x1,x2,x3);
}

Triangle::
Triangle( const RealArray & x1_, const RealArray & x2_, const RealArray & x3_ )
//===========================================================================
/// \brief  Create a triangle with vertices x1,x2,x3
/// \param x1,x2,x3 (input) : the three vertices of the triangle
//===========================================================================
{
  setVertices( &x1_(0),&x2_(0),&x3_(0) );
}

Triangle::
Triangle(const realArray & grid, 
	 const int & i1, 
	 const int & i2, 
	 const int & i3, 
	 const int & choice /* =0 */,
         const int & axis /* =axis1 */ )
//===========================================================================
/// \brief  
///     Build a triangle from a quadrilateral on the face of a grid grid, 
///   This constructor just calls the corresponding {\tt setVertices} function.
///   See the comments there.
//===========================================================================
{
  setVertices(grid,i1,i2,i3,choice,axis);
}

Triangle::
~Triangle()
{
  
}

void Triangle::
setVertices( const real x1_[3], const real x2_[3], const real x3_[3] )
//===========================================================================
/// \brief  Assign the vertices to a triangle.
/// \param x1,x2,x3 (input) : the three vertices of the triangle
//===========================================================================
{
  for( int i=0; i<3; i++ )
  {
    x1[i]=x1_[i];
    x2[i]=x2_[i];
    x3[i]=x3_[i];
  }
  computeNormal();

}

void Triangle::
setVertices( const RealArray & x1_, const RealArray & x2_, const RealArray & x3_ )
//===========================================================================
/// \brief  Assign the vertices to a triangle.
/// \param x1,x2,x3 (input) : the three vertices of the triangle
//===========================================================================
{
  setVertices( &x1_(0),&x2_(0),&x3_(0) );
}


void Triangle::
setVertices(const realArray & grid, 
	 const int & i1, 
	 const int & i2, 
	 const int & i3, 
	 const int & choice /* =0 */,
         const int & axis /* =axis3 */ )
//===========================================================================
/// \brief  
///     Form a triangle from a quadrilateral on the face of a grid grid, 
///     there are six possible choices.
/// \param grid (input) : and array containing the four points {\tt grid(i1+m,i2+n,i3,0:2)}, {\tt m=0,1}, {\tt n=0,1}.
/// \param i1,i2,i3 (input) : indicates which quadrilateral to use
/// \param choice, axis (input) : These define which of 6 poissible triangles to choose:
///   <ul>
///     <li>[choice=0, axis=axis3(==2)]: use  points (i1,i2,i3), (i1+1,i2,i3), (i1,i2+1,i3). Lower left
///               triangle in the plane i3==constant.
///     <li>[choice=1, axis=axis3(==2)]: use points (i1+1,i2+1,i3), (i1,i2+1,i3), (i1+1,i2,i3). Upper right
///               triangle in the plane i3==constant.
///     <li>[choice=0, axis=axis2(==1)]: use  points (i1,i2,i3), (i1,i2,i3+1), (i1+1,i2,i3).
///     <li>[choice=1, axis=axis2(==1)]: use points (i1+1,i2,i3+1), (i1+1,i2,i3), (i1,i2,i3+1).
///     <li>[choice=0, axis=axis1(==0)]: use  points (i1,i2,i3), (i1,i2+1,i3), (i1,i2,i3+1). 
///     <li>[choice=1, axis=axis1(==0)]: use points (i1,i2+1,i3+1), (i1,i2,i3+1), (i1,i2+1,i3).
///    </ul>
///     The figure below shows the two choices for axis=axis3:
///  \begin{verbatim}
///         x2
///      x3 ----------- x1
///         |\        |
///         |  \   1  |
///         |    \    |
///         | 0    \  |
///         |________\|x3
///        x1        x2
/// 
///  \end{verbatim}
//===========================================================================
{
  assert( choice==0 || choice==1 );
  assert( axis>=0 && axis<=2 );
  
  if( axis==axis3 )
  {
    if( choice==0 )
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2  ,i3,i);
	x2[i]=grid(i1+1,i2  ,i3,i);  
	x3[i]=grid(i1  ,i2+1,i3,i);  
      }
    }
    else
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1+1,i2+1,i3,i);
	x2[i]=grid(i1  ,i2+1,i3,i);  
	x3[i]=grid(i1+1,i2  ,i3,i);  
      }
    }
  }
  else if( axis==axis2 ) 
  {
    if( choice==0 )
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2  ,i3  ,i);
	x2[i]=grid(i1  ,i2  ,i3+1,i);  
	x3[i]=grid(i1+1,i2  ,i3  ,i);  
      }
    }
    else
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1+1,i2  ,i3+1,i);
	x2[i]=grid(i1+1,i2  ,i3  ,i);  
	x3[i]=grid(i1  ,i2  ,i3+1,i);  
      }
    }
  }
  else
  {
    if( choice==0 )
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2  ,i3  ,i);
	x2[i]=grid(i1  ,i2+1,i3  ,i);  
	x3[i]=grid(i1  ,i2+1,i3+1,i);  
      }
    }
    else
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2+1,i3+1,i);
	x2[i]=grid(i1  ,i2  ,i3+1,i);  
	x3[i]=grid(i1  ,i2+1,i3  ,i);  
      }
    }
  }
  
  computeNormal();
}

#ifdef USE_PPP
void Triangle::
setVertices(const realSerialArray & grid, 
	 const int & i1, 
	 const int & i2, 
	 const int & i3, 
	 const int & choice /* =0 */,
         const int & axis /* =axis3 */ )
//===========================================================================
// /Purpose: 
//    Form a triangle from a quadrilateral on the face of a grid grid, 
//    there are six possible choices.
//  /grid (input) : and array containing the four points {\tt grid(i1+m,i2+n,i3,0:2)}, {\tt m=0,1}, {\tt n=0,1}.
//  /i1,i2,i3 (input) : indicates which quadrilateral to use
//  /choice, axis (input) : These define which of 6 poissible triangles to choose:
//  \begin{description}
//    \item[choice=0, axis=axis3(==2)]: use  points (i1,i2,i3), (i1+1,i2,i3), (i1,i2+1,i3). Lower left
//              triangle in the plane i3==constant.
//    \item[choice=1, axis=axis3(==2)]: use points (i1+1,i2+1,i3), (i1,i2+1,i3), (i1+1,i2,i3). Upper right
//              triangle in the plane i3==constant.
//    \item[choice=0, axis=axis2(==1)]: use  points (i1,i2,i3), (i1,i2,i3+1), (i1+1,i2,i3).
//    \item[choice=1, axis=axis2(==1)]: use points (i1+1,i2,i3+1), (i1+1,i2,i3), (i1,i2,i3+1).
//    \item[choice=0, axis=axis1(==0)]: use  points (i1,i2,i3), (i1,i2+1,i3), (i1,i2,i3+1). 
//    \item[choice=1, axis=axis1(==0)]: use points (i1,i2+1,i3+1), (i1,i2,i3+1), (i1,i2+1,i3).
//   \end{description}
//    The figure below shows the two choices for axis=axis3:
// \begin{verbatim}
//        x2
//     x3 ----------- x1
//        |\        |
//        |  \   1  |
//        |    \    |
//        | 0    \  |
//        |________\|x3
//       x1        x2
//
// \end{verbatim}
//===========================================================================
{
  assert( choice==0 || choice==1 );
  assert( axis>=0 && axis<=2 );
  
  if( axis==axis3 )
  {
    if( choice==0 )
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2  ,i3,i);
	x2[i]=grid(i1+1,i2  ,i3,i);  
	x3[i]=grid(i1  ,i2+1,i3,i);  
      }
    }
    else
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1+1,i2+1,i3,i);
	x2[i]=grid(i1  ,i2+1,i3,i);  
	x3[i]=grid(i1+1,i2  ,i3,i);  
      }
    }
  }
  else if( axis==axis2 ) 
  {
    if( choice==0 )
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2  ,i3  ,i);
	x2[i]=grid(i1  ,i2  ,i3+1,i);  
	x3[i]=grid(i1+1,i2  ,i3  ,i);  
      }
    }
    else
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1+1,i2  ,i3+1,i);
	x2[i]=grid(i1+1,i2  ,i3  ,i);  
	x3[i]=grid(i1  ,i2  ,i3+1,i);  
      }
    }
  }
  else
  {
    if( choice==0 )
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2  ,i3  ,i);
	x2[i]=grid(i1  ,i2+1,i3  ,i);  
	x3[i]=grid(i1  ,i2+1,i3+1,i);  
      }
    }
    else
    {
      for( int i=0; i<3; i++ )
      {
	x1[i]=grid(i1  ,i2+1,i3+1,i);
	x2[i]=grid(i1  ,i2  ,i3+1,i);  
	x3[i]=grid(i1  ,i2+1,i3  ,i);  
      }
    }
  }
  
  computeNormal();
}
#endif

void Triangle::
computeNormal()
{
  
  normal[0]=(x2[1]-x1[1])*(x3[2]-x1[2])-(x2[2]-x1[2])*(x3[1]-x1[1]);
  normal[1]=(x2[2]-x1[2])*(x3[0]-x1[0])-(x2[0]-x1[0])*(x3[2]-x1[2]);
  normal[2]=(x2[0]-x1[0])*(x3[1]-x1[1])-(x2[1]-x1[1])*(x3[0]-x1[0]);
  
  real norm = SQR(normal[0])+SQR(normal[1])+SQR(normal[2]);
  if( norm!=0. )
  {
    norm=1./SQRT(norm);
    normal[0]*=norm;
    normal[1]*=norm;
    normal[2]*=norm;
  }
  normal[3] = -( normal[0]*x1[0]+normal[1]*x1[1]+normal[2]*x1[2] );  // const coeff for plane n*x+c=0

}

real Triangle::
area() const
//===========================================================================
/// \brief  
///     return the area of the triangle
//===========================================================================
{
  
  real a1=(x2[1]-x1[1])*(x3[2]-x1[2])-(x2[2]-x1[2])*(x3[1]-x1[1]);
  real a2=(x2[2]-x1[2])*(x3[0]-x1[0])-(x2[0]-x1[0])*(x3[2]-x1[2]);
  real a3=(x2[0]-x1[0])*(x3[1]-x1[1])-(x2[1]-x1[1])*(x3[0]-x1[0]);
  
  return .5*SQRT( a1*a1+a2*a2+a3*a3 );
}


void Triangle::
display(const aString & label /* =blankString */) const
//===========================================================================
/// \brief  
///     print out the vertices and the normal.
//===========================================================================
{
  printf("Triangle %s: x1=(%6.2g,%6.2g,%6.2g), x2=(%6.2g,%6.2g,%6.2g), x3=(%6.2g,%6.2g,%6.2g), "
         "normal=(%6.2g,%6.2g,%6.2g)\n",(const char*)label,
	 x1[0],x1[1],x1[2],x2[0],x2[1],x2[2],x3[0],x3[1],x3[2],normal[0],normal[1],normal[2]);
}


#define TETRAHEDRALVOLUME(vol,a,b,c,d)\
{\
  double ad1 = a[1] - d[1];\
  double bd1 = b[1] - d[1];\
  double cd1 = c[1] - d[1];\
  double ad2 = a[2] - d[2];\
  double bd2 = b[2] - d[2];\
  double cd2 = c[2] - d[2];\
  vol=   (a[0]-d[0])*(bd1*cd2 - bd2*cd1)\
       + (b[0]-d[0])*(cd1*ad2 - cd2*ad1)\
       + (c[0]-d[0])*(ad1*bd2 - ad2*bd1); \
}


double Triangle::
tetrahedralVolume(const real a[], const real b[], const real c[], const real d[]) const
//===========================================================================
/// \brief  
///     Return the approximate volume (actually 6 times the volume) of the
///       tretrahedra formed by the points (a,b,c,d)
//===========================================================================
{
  double ad1 = a[1] - d[1];
  double bd1 = b[1] - d[1];
  double cd1 = c[1] - d[1];
  double ad2 = a[2] - d[2];
  double bd2 = b[2] - d[2];
  double cd2 = c[2] - d[2];

  return (a[0]-d[0])*(bd1*cd2 - bd2*cd1)
       + (b[0]-d[0])*(cd1*ad2 - cd2*ad1)
       + (c[0]-d[0])*(ad1*bd2 - ad2*bd1);
}


int Triangle::
intersects(Triangle & tri, real xi1[3], real xi2[3] ) const
//===========================================================================
/// \brief  
///    Determine if this triangle intersect another.
/// \param tri (input) : intersect with this triangle.
/// \param xi1, xi2 (output) : if the return value is true then these are the endpoints
///     of the line of intersection between the two triangles.
/// \param Return value : TRUE if the triangles intersect, false otherwise.
//===========================================================================
{

  // first check that the bounding boxes around the triangles intersect
  int intersects=TRUE;
  real xMin1,xMax1, xMin2,xMax2;
  for( int axis=0; axis<3; axis++ )
  {
    xMin1=min(    x1[axis],min(    x2[axis],    x3[axis]));
    xMax1=max(    x1[axis],max(    x2[axis],    x3[axis]));
    xMin2=min(tri.x1[axis],min(tri.x2[axis],tri.x3[axis]));
    xMax2=max(tri.x1[axis],max(tri.x2[axis],tri.x3[axis]));
    intersects=intersects && xMax1>=xMin2 && xMax2>=xMin1;
    if( !intersects )
      return FALSE;
  }

//  Triangle ABC : A=x1[.], B=x2[.], C=x3[.]
//            C 
//
//
//       A         B
//
//
// Triangle DEF, D=t2.x1[.], E=t2.x2[.], F
// 

//  (1) line DE crosses this triangle if: (AD).N * (AE).N <= 0
//  Check DE, EF and FD

  double adn,aen,t, v1,v2,v3;
  
  int d1,d2,d3;
  const real *d,*e;
  real *p;
  int numberOfIntersections=0;
  
  for( int n=0; n<2; n++ )
  { 
     // n==0 : check intersections with ABC
     // n==1 : check intersections with DEF

    const Triangle & t1 = n==0 ? *this : tri;
    const Triangle & t2 = n==0 ? tri : *this;
    
    for( int m=0; m<3; m++ )  // check possible intersections with 3 line segments
    {
      // look for intersection of segment DE with triangle ABC
      d= m==0 ? t2.x1 : (m==1 ? t2.x2 : t2.x3);
      e= m==0 ? t2.x2 : (m==1 ? t2.x3 : t2.x1);

      // adn = (AD).N   aen = (AE).N
      adn=(t1.x1[0]-d[0])*t1.normal[0]+(t1.x1[1]-d[1])*t1.normal[1]+(t1.x1[2]-d[2])*t1.normal[2];
      aen=(t1.x1[0]-e[0])*t1.normal[0]+(t1.x1[1]-e[1])*t1.normal[1]+(t1.x1[2]-e[2])*t1.normal[2];

      if( adn*aen <=0. )
      {
	// line segment DE crosses the plane of ABC, check for inside 
        v1 = tetrahedralVolume(t1.x1,t1.x2, d,e );
        v2 = tetrahedralVolume(t1.x2,t1.x3, d,e );
        v3 = tetrahedralVolume(t1.x3,t1.x1, d,e );

        d1 = sign( v1 );
        d2 = sign( v2 );
        d3 = sign( v3 );
      

	if (  (d1*d2*d3) == 0 )
	{ 
          // At least one of the tetrahedra volumes is zero
          if( d1*d2 <0 || d1*d3<0 || d2*d3<0 )
	  {
	    intersects=0;
	  }
	  else
	  {
	    intersects=2;
            if( Mapping::debug & 4 )
	    {
	      printf("Triangle:INFO: coplanar intersection d=(%e,%e,%e), e=(%e,%e,%e), d1=%i, d2=%i, d3=%i \n",
		     d[0],d[1],d[2],e[1],e[2],e[3],d1,d2,d3);
	      t1.display("triangle t1");
	      t2.display("triangle t2");
	    }
	    
	    if(     area()<REAL_EPSILON*max(fabs(xMax1),fabs(xMin1)) ||
		    tri.area()<REAL_EPSILON*max(fabs(xMax2),fabs(xMin2)) )
	    {
	      printf("Triangle:INFO: one of the triangles has zero area");
	      return FALSE;
	    }
	  }
	}
	else
	{
	  if ( (d1 == d2) && ( d2==d3) && ( d1==d3) )
	  { 
            intersects=1; // proper intersection 
	  }
          else
            intersects=0; 
	}

	if ( intersects )
	  { // kkc make darn sure using robust predicates
	    ArraySimpleFixed<real,3,1,1,1> p1,p2;
	    ArraySimpleFixed<real,3,3,1,1> abc;
	    
	    for ( int i=0; i<3; i++ )
	      {
		p1[i] = d[i];
		p2[i] = e[i];
		abc(0,i) = t1.x1[i];
		abc(1,i) = t1.x2[i];
		abc(2,i) = t1.x3[i];
	      }

	    bool isParallel = false;
	    real angle=0.;
	    intersects = intersect3D(abc,p1,p2,isParallel,angle);

	  } 
 
        if( intersects )
	{
	  numberOfIntersections++;

	  // here is the point of intersection:
	  if( adn-aen !=0. )
	  {
	    t=adn/(adn-aen); // this is error prone
	    if( t<0. || t>1. )
	    {
	      printf("Triangle:INFO: t=%e, should be in [0,1] \n",t);
	      t=max(0.,min(1.,t));
	    }
	    p= numberOfIntersections==1 ? xi1 : xi2;
	    // point of intersection is P=tE+(1-t)D
	    p[0]=t*e[0]+(1.-t)*d[0];
	    p[1]=t*e[1]+(1.-t)*d[1];
	    p[2]=t*e[2]+(1.-t)*d[2];
	  }
	  else
	  {
            intersects=2;
	    printf("%%%%%%%%Triangle::INFO: d and e both lie in the plane! \n");
	    printf("%%%%%%%%Triangle::ERROR: fix this bill! \n");
            // *** this is not correct *** find if inside the triangle! *********************
	    // real alpha1,alpha2;
	    // getRelativeCoordinates( x,alpha1,alpha2);

	    xi1[0]=d[0];
	    xi1[1]=d[1];
	    xi1[2]=d[2];
	    numberOfIntersections++;	
	    xi2[0]=e[0];
	    xi2[1]=e[1];
	    xi2[2]=e[2];
	  }
	  if( intersects==2 && numberOfIntersections>=2 )
	  {
	    // coplanar intersection, check for the same point of intersection
	    if( fabs(xi1[0]-xi2[0])+fabs(xi1[1]-xi2[1])+fabs(xi1[2]-xi2[2]) 
		<REAL_EPSILON*max(fabs(xMax1),fabs(xMin1)) )
	    {
	      printf("Triangle:INFO: skipping an intersection that has already been found \n");
	      numberOfIntersections--;
	    }
            if( numberOfIntersections>2 )
	    {
	      printf("&&&&&&&&& Triangle:ERROR: too many intersections!! \n");
	      numberOfIntersections=2;
	    }
	  }
          if( numberOfIntersections==2  )
	    break;
	}
      }
    }  // for m
    if( numberOfIntersections==2  )
      break;
  }
  if( numberOfIntersections ==1 )
  {
    printf("Triangle::intersect:INFO: only 1 point of intersection bewteen two triangles \n");
    xi2[0]=xi1[0];
    xi2[1]=xi1[1];
    xi2[2]=xi1[2];
    // throw "error";
    // kkc instead of throwing an error, prevent this from being an intersection!
    numberOfIntersections=0;
  }

  return numberOfIntersections>0;
}


// ****** NOTE: wdh: 2012/01/26 -- changed return values on these calls to int *******

int Triangle::
intersects(Triangle & triangle, RealArray & xi1, RealArray & xi2 ) const
//===========================================================================
/// \brief  
///    Determine if this triangle intersect another.
/// \param tri (input) : intersect with this triangle.
/// \param xi1, xi2 (output) : if the return vaule is true then these are the endpoints
///     of the line of intersection between the two triangles.
/// \param Return value : 0=no intersecttion, >0 if the ray intersects the triangle,
///     1=proper intersection, 2=intersects on the boundary.
//===========================================================================
{
  return intersects( triangle,&xi1(0),&xi2(0));
}


int Triangle::
intersects(real x[3], real xi[3] ) const
//===========================================================================
/// \brief  
///    Determine if this triangle intersects a ray starting at the point x[] and
///      extending to y=+infinity.
/// \param x (input) : find the intersection with a vertical ray starting at this point.
/// \param xi (output) : if the return value is true then this is the intersection point./
/// \param Return value : 0=no intersecttion, >0 if the ray intersects the triangle,
///     1=proper intersection, 2=intersects on the boundary.
//===========================================================================
{
  int debug=0; // 3;

  real x0=x[0], y0=x[1], z0=x[2];

  // first check that the bounding boxes around the triangle contains the ray.
  int intersects=TRUE;
  real yMin,yMax, xMin,xMax,zMin,zMax;
  yMax=max(x1[1],x2[1],x3[1]);

  intersects=intersects && y0 <=yMax;
  if( !intersects )
    return 0;

  xMin=min(x1[0],x2[0],x3[0]);
  xMax=max(x1[0],x2[0],x3[0]);

  intersects=intersects && x0 >= xMin && x0 <=xMax;
  if( !intersects )
    return 0;

  zMin=min(x1[2],x2[2],x3[2]);
  zMax=max(x1[2],x2[2],x3[2]);

  intersects=intersects && z0 >= zMin && z0 <=zMax;
  if( !intersects )
    return 0;

  yMin=min(x1[1],x2[1],x3[1]);

//  Triangle ABC : A=x1[.], B=x2[.], C=x3[.]
//            C 
//
//
//       A         B
//
//
// Line  DE, D=(x0,y0,z0),+infinity,z0)
// 

//  (1) line DE crosses this triangle if: (AD).N * (AE).N <= 0

  double adn,aen,t, v1,v2,v3;
  
  int d1,d2,d3;

  real e[3];
  e[0]=x0;
  e[1]=yMax+ (yMax-yMin)+(xMax-xMin) + (zMax-zMin);   // this point is above the triangle
  e[2]=z0;

  // adn = (AD).N   aen = (AE).N   D=x
  adn=(x1[0]-x0)*normal[0]+(x1[1]-y0  )*normal[1]+(x1[2]-z0)*normal[2];
  aen=(x1[0]-x0)*normal[0]+(x1[1]-e[1])*normal[1]+(x1[2]-z0)*normal[2];

  printF("Triangle::intersects: adn=%e aen=%e \n",adn,aen);

  if( adn*aen <=0. )
  {
    // line segment DE crosses the plane of ABC, check for an intersection point
    // that is inside the triangle
    // instead of using x, which could be a long way away from the triangle,
    // find a point below the triangle but probably closer
    real d[3];
    d[0]=x0;
    d[1]=yMin - ( (yMax-yMin)+(xMax-xMin) + (zMax-zMin) );   // this point is below the triangle
    d[2]=z0;
    
    v1 = tetrahedralVolume(x1,x2, d,e );
    v2 = tetrahedralVolume(x2,x3, d,e );
    v3 = tetrahedralVolume(x3,x1, d,e );

    if( debug & 1 )
      printf("Triangle::intersects: (v1,v2,v3)=(%e,%e,%e) \n",v1,v2,v3);
    

    d1 = sign( v1 );     // the intersection point is inside the triangle if all the signs are the same
    d2 = sign( v2 );
    d3 = sign( v3 );

    if (  (d1*d2*d3) == 0 )
    { 
      // At least one of the tetrahedra volumes is zero
      // ** This means that the ray intersects an edge. ***
      if( d1*d2 <0 || d1*d3<0 || d2*d3<0 )
      {
	intersects=2;    // ***wdh*** count as a degenerate case
      }
      else
      {
	intersects=2;
	if( debug & 1 )
	{
	  printf("Triangle:INFO: coplanar intersection d=(%e,%e,%e), e=(%e,%e,%e), d1=%i, d2=%i, d3=%i \n",
		 d[0],d[1],d[2],e[0],e[1],e[2],d1,d2,d3);
	  display("triangle t1");
	}
	    
	if( area()<REAL_EPSILON*max(fabs(xMax),fabs(xMin)) )
	{
	  printf("Triangle:INFO: the triangle has zero area");
	  return FALSE;
	}
      }
    }
    else
    {
      if ( (d1 == d2) && ( d2==d3) && ( d1==d3) )
      { 
	intersects=1; // proper intersection 
      }
      else
      {
        // the 3 tetrahedra do not have the same sign
        // check for close:
/* ------
        real epsVol =max(fabs(v1),fabs(v2),fabs(v3))*REAL_EPSILON*10.;
	
        if( (d1==d2 && fabs(v3)<epsVol) || (d2==d3 && fabs(v1)<epsVol) || (d1==d3 && fabs(v2)<epsVol ) )
	  intersects=1;
	else
	{
	  printf("no intersection: volumes should be the same sign: (v1,v2,v3)=(%e,%e,%e) \n",v1,v2,v3);
    	  intersects=0; 
	}
--------- */
        intersects=0;
      }
    }
    if( intersects )
    {
      // here is the point of intersection:
      if( adn-aen !=0. )
      {
	t=adn/(adn-aen); // this is error prone
	if( t<0. || t>1. )
	{
	  printf("Triangle:INFO: t=%e, should be in [0,1] \n",t);
	  t=max(0.,min(1.,t));
	}
	// point of intersection is P=tE+(1-t)D
	xi[0]=t*e[0]+(1.-t)*x[0];  // ***  Note: d=x for computation of adn, aen
	xi[1]=t*e[1]+(1.-t)*x[1];
	xi[2]=t*e[2]+(1.-t)*x[2];
      }
      else
      {
        // we proclaim this case to be no intersection. Is this correct?
	printf("%%%%%%%%Triangle::INFO: ray lines in the plane of the triangle! no intersection assumed.\n");
        intersects=0;
      }
    }
  }
  else
    intersects=0;

  return intersects;
}

int Triangle::
intersects(RealArray & x, RealArray &  xi ) const
//===========================================================================
/// \brief  
///    Determine if this triangle intersects a line starting at the point x and
///      extending to y=+infinity.
/// \param x (input) : find the intersection with a vertical ray starting at this point.
/// \param xi (output) : if the return value is true then this is the intersection point.
/// \param Return value : 0=no intersection, >0 if the ray intersects the triangle,
///     1=proper intersection, 2=intersects on the boundary.
//===========================================================================
{
  return intersects(&x(0),&xi(0));
}

int Triangle::
intersects(real x[3], real xi[3], real b0[3], real b1[3], real b2[3]  ) const
//===========================================================================
/// \brief  
///    Determine if this triangle intersects a ray starting at the point x[] and
///      extending in the direction b1
/// \param x (input) : find the intersection with a vertical ray starting at this point.
/// \param xi (output) : if the return value is true then this is the intersection point.
/// \param b0,b1,b2 : these vectors form an ortho-normal set
/// \param Return value : 0=no intersection, >0 if the ray intersects the triangle,
///     1=proper intersection, 2=intersects on the boundary.
//===========================================================================
{

  int debug=0; // 3;

  // We convert the vectors x1-x, x2-x, x3-x to the basis (b0,b1,b2)
  // In this basis the ray starts at "(x,y,z)=(0,0,0)" and extends to y=+infinity 

  const real x0=0., y0=0., z0=0.;  // ray starts here in transformed coordinates

  real dx1[3]={x1[0]-x[0],x1[1]-x[1],x1[2]-x[2]}; //
  real dx2[3]={x2[0]-x[0],x2[1]-x[1],x2[2]-x[2]}; //
  real dx3[3]={x3[0]-x[0],x3[1]-x[1],x3[2]-x[2]}; //

  real w1[3]={b0[0]*dx1[0]+b0[1]*dx1[1]+b0[2]*dx1[2],    // b0.dx1
              b1[0]*dx1[0]+b1[1]*dx1[1]+b1[2]*dx1[2],    // b1.dx1
              b2[0]*dx1[0]+b2[1]*dx1[1]+b2[2]*dx1[2]};   // b2.dx1

  real w2[3]={b0[0]*dx2[0]+b0[1]*dx2[1]+b0[2]*dx2[2],    // b0.dx2
              b1[0]*dx2[0]+b1[1]*dx2[1]+b1[2]*dx2[2],    // b1.dx2
              b2[0]*dx2[0]+b2[1]*dx2[1]+b2[2]*dx2[2]};   // b2.dx2

  real w3[3]={b0[0]*dx3[0]+b0[1]*dx3[1]+b0[2]*dx3[2],
              b1[0]*dx3[0]+b1[1]*dx3[1]+b1[2]*dx3[2],
              b2[0]*dx3[0]+b2[1]*dx3[1]+b2[2]*dx3[2]}; //

  // first check that the bounding boxes around the triangle contains the ray.
  int intersects=true;
  real yMin,yMax, xMin,xMax,zMin,zMax;
  yMax=max(w1[1],w2[1],w3[1]);

  intersects=intersects && y0 <=yMax;
  if( !intersects )
    return false;


  xMin=min(w1[0],w2[0],w3[0]);
  xMax=max(w1[0],w2[0],w3[0]);

  intersects=intersects && x0 >= xMin && x0 <=xMax;
  if( !intersects )
    return false;

  zMin=min(w1[2],w2[2],w3[2]);
  zMax=max(w1[2],w2[2],w3[2]);

  intersects=intersects && z0 >= zMin && z0 <=zMax;
  if( !intersects )
    return false;

  yMin=min(w1[1],w2[1],w3[1]);

//  Triangle ABC : A=x1[.], B=x2[.], C=x3[.]
//            C 
//
//
//       A         B
//
//
// Line  DE, D=(x0,y0,z0),+infinity,z0)
// 

//  (1) line DE crosses this triangle if: (AD).N * (AE).N <= 0

  double adn,aen,t, v1,v2,v3;
  
  int d1,d2,d3;

  real e[3];
  e[0]=x0;
  e[1]=yMax+ (yMax-yMin)+(xMax-xMin) + (zMax-zMin);   // this point is above the triangle
  e[2]=z0;

  // transform the normal to the [b0,b1,b2] basis
  real nw[3]={b0[0]*normal[0]+b0[1]*normal[1]+b0[2]*normal[2],
              b1[0]*normal[0]+b1[1]*normal[1]+b1[2]*normal[2],
              b2[0]*normal[0]+b2[1]*normal[1]+b2[2]*normal[2]}; //

  // adn = (AD).N   aen = (AE).N   D=x
  adn=(w1[0]-x0)*nw[0]+(w1[1]-y0  )*nw[1]+(w1[2]-z0)*nw[2];
  aen=(w1[0]-x0)*nw[0]+(w1[1]-e[1])*nw[1]+(w1[2]-z0)*nw[2];

  if( adn*aen <=0. )
  {
    // line segment DE crosses the plane of ABC, check for an intersection point
    // that is inside the triangle
    // instead of using x, which could be a long way away from the triangle,
    // find a point below the triangle but probably closer
    real d[3];
    d[0]=x0;
    d[1]=yMin - ( (yMax-yMin)+(xMax-xMin) + (zMax-zMin) );   // this point is below the triangle
    d[2]=z0;
    
//      v1 = tetrahedralVolume(w1,w2, d,e );
//      v2 = tetrahedralVolume(w2,w3, d,e );
//      v3 = tetrahedralVolume(w3,w1, d,e );

    TETRAHEDRALVOLUME(v1,w1,w2, d,e );
    TETRAHEDRALVOLUME(v2,w2,w3, d,e );
    TETRAHEDRALVOLUME(v3,w3,w1, d,e );


    d1 = sign( v1 );     // the intersection point is inside the triangle if all the signs are the same
    d2 = sign( v2 );
    d3 = sign( v3 );

    if( debug & 1 )
      printf("Triangle::intersects: (v1,v2,v3)=(%e,%e,%e) signs=(%i,%i,%i) \n",v1,v2,v3,d1,d2,d3);
    
    if (  (d1*d2*d3) == 0 )
    { 
      // At least one of the tetrahedra volumes is zero
      // ** This means that the ray intersects an edge. ***
      if( d1*d2 <0 || d1*d3<0 || d2*d3<0 )
      {
	intersects=2;    // ***wdh*** count as a degenerate case
      }
      else
      {
	intersects=2;
	if( debug & 1 )
	{
	  printF("Triangle:INFO: coplanar intersection d=(%e,%e,%e), e=(%e,%e,%e), d1=%i, d2=%i, d3=%i \n",
		 d[0],d[1],d[2],e[0],e[1],e[2],d1,d2,d3);
	  display("triangle t1");
	}
	    
	if( area()<REAL_EPSILON*max(fabs(xMax),fabs(xMin)) )
	{
	  printF("Triangle:INFO: the triangle has zero area");
	  return FALSE;
	}
      }
    }
    else
    {
      if ( (d1 == d2) && ( d2==d3) && ( d1==d3) )
      { 
	intersects=1; // proper intersection 
      }
      else
      {
        // the 3 tetrahedra do not have the same sign
        // check for close:
/* ------
        real epsVol =max(fabs(v1),fabs(v2),fabs(v3))*REAL_EPSILON*10.;
	
        if( (d1==d2 && fabs(v3)<epsVol) || (d2==d3 && fabs(v1)<epsVol) || (d1==d3 && fabs(v2)<epsVol ) )
	  intersects=1;
	else
	{
	  printf("no intersection: volumes should be the same sign: (v1,v2,v3)=(%e,%e,%e) \n",v1,v2,v3);
    	  intersects=0; 
	}
--------- */
        intersects=0;
      }
    }
    if( intersects )
    {
      // here is the point of intersection:
      if( adn-aen !=0. )
      {
	t=adn/(adn-aen); // this is error prone
	if( t<0. || t>1. )
	{
	  printf("Triangle:INFO: t=%e, should be in [0,1] \n",t);
	  t=max(0.,min(1.,t));
	}
	// point of intersection is P=tE+(1-t)D
	d[0]=t*e[0];  //   ***  Note: d=x for computation of adn, aen
	d[1]=t*e[1];
	d[2]=t*e[2];

        // transform the point back to (x,y,z) space:
        xi[0] = d[0]*b0[0] + d[1]*b1[0] + d[2]*b2[0] +x[0];
        xi[1] = d[0]*b0[1] + d[1]*b1[1] + d[2]*b2[1] +x[1];
        xi[2] = d[0]*b0[2] + d[1]*b1[2] + d[2]*b2[2] +x[2];

      }
      else
      {
        // we proclaim this case to be no intersection. Is this correct?
	printf("%%%%%%%%Triangle::INFO: ray lines in the plane of the triangle! no intersection assumed.\n");
        intersects=0;
      }
    }
  }
  else
    intersects=0;

  // printf("Triangle:return intersects=%i\n",intersects);
  return intersects;
}

int Triangle::
intersects(RealArray & x, RealArray &  xi, real b0[3], real b1[3], real b2[3]  ) const
//===========================================================================
/// \brief  
///    Determine if this triangle intersects a ray starting at the point x and
///      extending in the direction b1
/// \param x (input) : find the intersection with a vertical ray starting at this point.
/// \param xi (output) : if the return value is true then this is the intersection point.
/// \param b0,b1,b2 : these vectors form an ortho-normal set
/// \param Return value : 0=no intersection, >0 if the ray intersects the triangle,
///     1=proper intersection, 2=intersects on the boundary.
//===========================================================================
{
  if( b1[1]!=1. )
    return intersects(&x(0),&xi(0),b0,b1,b2);
  else
    return intersects(&x(0),&xi(0));   // use the more efficient version for a ray in the +y-direction
}



/* ----
struct Point
{
  int id;
  double x,y,z;
};


  
bool Triangle::
intersects(Triangle & t2, real xi1[3], real xi2[3] ) const
// does this triangle intersect another? 
// if true : return segment of intersection: point xi1 to point xi2
{

  real tol=REAL_EPSILON;  // *** needs to be relative ****
  
  double ad[3], bd[3], td;
  double a12, b12, ab1, ab2;
  Point a1,a2,b1,b2;
  int debug=0;

  ad[0] = t2.normal[0]*x1[0]+t2.normal[1]*x1[1]+t2.normal[2]*x1[2]+t2.normal[3];
  ad[1] = t2.normal[0]*x2[0]+t2.normal[1]*x2[1]+t2.normal[2]*x2[2]+t2.normal[3];
  ad[2] = t2.normal[0]*x3[0]+t2.normal[1]*x3[1]+t2.normal[2]*x3[2]+t2.normal[3];

  bd[0] = normal[0]*t2.x1[0]+normal[1]*t2.x1[1]+normal[2]*t2.x1[2]+normal[3];
  bd[1] = normal[0]*t2.x2[0]+normal[1]*t2.x2[1]+normal[2]*t2.x2[2]+normal[3];
  bd[2] = normal[0]*t2.x3[0]+normal[1]*t2.x3[1]+normal[2]*t2.x3[2]+normal[3];

  if (debug>3) printf("ad[*]=%f %f %f   bd[*]=%f %f %f\n",
	              ad[0],ad[1],ad[2],bd[0],bd[1],bd[2]);	

  a1.id=a2.id=b1.id=b2.id=0;

  //  1 -- 1 -- 2    
  //  |        /     
  //  |       /      
  //                 
  //  3     2        
  //                 
  //  |   /          
  //  |  /           
  //  | /            
  //  3              
  
  if (fabs(ad[0])<tol && fabs(ad[1])<tol) {
     a1.x=x1[0];
     a1.y=x1[1];
     a1.z=x1[2];
     a1.id=1;
     a2.x=x2[0];
     a2.y=x2[1];
     a2.z=x2[2];
     a2.id=1;
  } 
  else if (fabs(ad[0])<tol && fabs(ad[2])<tol) {
     a1.x=x1[0];
     a1.y=x1[1];
     a1.z=x1[2];
     a1.id=1;
     a2.x=x3[0];
     a2.y=x3[1];
     a2.z=x3[2];
     a2.id=1;
  }
  else if (fabs(ad[1])<tol && fabs(ad[2])<tol) {
     a1.x=x2[0];
     a1.y=x2[1];
     a1.z=x2[2];
     a1.id=1;
     a2.x=x3[0];
     a2.y=x3[1];
     a2.z=x3[2];
     a2.id=1;
  }

  if (ad[0]*ad[1]<0. && a1.id==0) {
     td = fabs(ad[1]) + fabs(ad[0]);
     a1.x = (fabs(ad[1])*x1[0] +fabs(ad[0])*x2[0])/td;
     a1.y = (fabs(ad[1])*x1[1] +fabs(ad[0])*x2[1])/td;
     a1.z = (fabs(ad[1])*x1[2] +fabs(ad[0])*x2[2])/td;
     a1.id=1;
  }
  if (ad[0]*ad[2]<0.) {
     td = fabs(ad[2]) + fabs(ad[0]);
     if (a1.id==0) {   
       a1.x = (fabs(ad[2])*x1[0] +fabs(ad[0])*x3[0])/td;
       a1.y = (fabs(ad[2])*x1[1] +fabs(ad[0])*x3[1])/td;
       a1.z = (fabs(ad[2])*x1[2] +fabs(ad[0])*x3[2])/td;
       a1.id=1;
     } 
     else if (a2.id==0) {
       a2.x = (fabs(ad[2])*x1[0] +fabs(ad[0])*x3[0])/td;
       a2.y = (fabs(ad[2])*x1[1] +fabs(ad[0])*x3[1])/td;
       a2.z = (fabs(ad[2])*x1[2] +fabs(ad[0])*x3[2])/td;
       a2.id=1;
     } 
  }
  if (ad[2]*ad[1]<0. && a2.id==0 ) {
     td = fabs(ad[1]) + fabs(ad[2]);
     a2.x = (fabs(ad[1])*x3[0] +fabs(ad[2])*x2[0])/td;
     a2.y = (fabs(ad[1])*x3[1] +fabs(ad[2])*x2[1])/td;
     a2.z = (fabs(ad[1])*x3[2] +fabs(ad[2])*x2[2])/td;
     a2.id=1;
  }
  
 // for second triangular   
  if (fabs(bd[0])<tol && fabs(bd[1])<tol) {
     b1.x=t2.x1[0];
     b1.y=t2.x1[1];
     b1.z=t2.x1[2];
     b1.id=1;
     b2.x=t2.x3[0];
     b2.y=t2.x3[1];
     b2.z=t2.x3[2];
     b2.id=1;
  }
  else if (fabs(bd[0])<tol && fabs(bd[2])<tol) {
     b1.x=t2.x1[0];
     b1.y=t2.x1[1];
     b1.z=t2.x1[2];
     b1.id=1;
     b2.x=t2.x3[0];
     b2.y=t2.x3[1];
     b2.z=t2.x3[2];
     b2.id=1;
  }
  else if (fabs(bd[1])<tol && fabs(bd[2])<tol) {
     b1.x=t2.x2[0];
     b1.y=t2.x2[1];
     b1.z=t2.x2[2];
     b1.id=1;
     b2.x=t2.x3[0];
     b2.y=t2.x3[1];
     b2.z=t2.x3[2];
     b2.id=1;
  }

  if (bd[0]*bd[1]<0. && b1.id==0) {
     td = fabs(bd[1]) + fabs(bd[0]);
     b1.x = (fabs(bd[1])*t2.x1[0] +fabs(bd[0])*t2.x2[0])/td;
     b1.y = (fabs(bd[1])*t2.x1[1] +fabs(bd[0])*t2.x2[1])/td;
     b1.z = (fabs(bd[1])*t2.x1[2] +fabs(bd[0])*t2.x2[2])/td;
     b1.id=1;
  }
  if (bd[0]*bd[2]<0.) {
     td = fabs(bd[2]) + fabs(bd[0]);
     if (b1.id==0) {   
       b1.x = (fabs(bd[2])*t2.x1[0] +fabs(bd[0])*t2.x3[0])/td;
       b1.y = (fabs(bd[2])*t2.x1[1] +fabs(bd[0])*t2.x3[1])/td;
       b1.z = (fabs(bd[2])*t2.x1[2] +fabs(bd[0])*t2.x3[2])/td;
       b1.id=1;
     } 
     else if (b2.id==0) {
       b2.x = (fabs(bd[2])*t2.x1[0] +fabs(bd[0])*t2.x3[0])/td;
       b2.y = (fabs(bd[2])*t2.x1[1] +fabs(bd[0])*t2.x3[1])/td;
       b2.z = (fabs(bd[2])*t2.x1[2] +fabs(bd[0])*t2.x3[2])/td;
       b2.id=1;
     } 
  }
  if (bd[2]*bd[1]<0. && b2.id==0 ) {
     td = fabs(bd[1]) + fabs(bd[2]);
     b2.x = (fabs(bd[1])*t2.x3[0] +fabs(bd[2])*t2.x2[0])/td;
     b2.y = (fabs(bd[1])*t2.x3[1] +fabs(bd[2])*t2.x2[1])/td;
     b2.z = (fabs(bd[1])*t2.x3[2] +fabs(bd[2])*t2.x2[2])/td;
     b2.id=1;
  }
  if (debug> 1) printf("id 1234= %d %d %d %d \n",a1.id,a2.id,b1.id,b2.id);  

  if (a1.id !=1 || a2.id !=1 || b1.id !=1 || b2.id !=1 ) return FALSE;

  if (debug>3) printf("a1= %f %f %f a2= %f %f %f b1= %f %f %f b2= %f %f %f \n",
                       a1.x,a1.y,a1.z,a2.x,a2.y,a2.z, 
                       b1.x,b1.y,b1.z,b2.x,b2.y,b2.z); 
  // compare the distance to find the the two intersection points 

  a12 = (a2.x-a1.x)*(a2.x-a1.x) + (a2.y-a1.y)*(a2.y-a1.y) +
        (a2.z-a1.z)*(a2.z-a1.z);

  b12 = (b2.x-b1.x)*(b2.x-b1.x) + (b2.y-b1.y)*(b2.y-b1.y) +
        (b2.z-b1.z)*(b2.z-b1.z);

  ab1 = (b1.x-a1.x)*(a2.x-a1.x) + (b1.y-a1.y)*(a2.y-a1.y) +
        (b1.z-a1.z)*(a2.z-a1.z);

  ab2 = (b2.x-a1.x)*(a2.x-a1.x) + (b2.y-a1.y)*(a2.y-a1.y) +
        (b2.z-a1.z)*(a2.z-a1.z);

  // case 0:  a1 =a2 or b1 = b2    
  // case 1:  a1 -- b1 -- b2 -- a2 
  //                 or            
  //          a1 -- b2 -- b1 -- a2 
  // case 2:  b1 -- a1 -- a2 -- b2 
  //                 or            
  //          b1 -- a2 -- a1 -- b2 
  // case 3:  a1 -- b1 -- a2 -- b2 
  // case 4:  a1 -- b2 -- a2 -- b1 
  // case 5:  b1 -- a1 -- b2 -- a2 
  // case 6:  b2 -- a1 -- b1 -- a2 
  if (debug>3) printf("a12=%f ab1 =%f ab2=%f\n",a12,ab1,ab2);

  if (a12 < tol || b12 < tol) return FALSE;
  if (a12 >= ab1 && a12 >= ab2 && ab1 >=0 && ab2 >=0) {
     xi1[0]=b1.x; xi1[1]=b1.y; xi1[2]=b1.z;
     xi2[0]=b2.x; xi2[1]=b2.y; xi2[2]=b2.z;
  } 
  else if ((a12 <= ab1 && ab2 <=0 ) || (a12 <= ab2 && ab1 <=0 )) {
     xi1[0]=a1.x; xi1[1]=a1.y; xi1[2]=a1.z;
     xi2[0]=a2.x; xi2[1]=a2.y; xi2[2]=a2.z;
  } 
  else if (ab1>=0 && ab2>=0 && a12>= ab1 && a12 <= ab2 ) {
     xi1[0]=b1.x; xi1[1]=b1.y; xi1[2]=b1.z;
     xi2[0]=a2.x; xi2[1]=a2.y; xi2[2]=a2.z;
  } 
  else if (ab1>=0 && ab2>=0 && a12>= ab2 && a12 <= ab1 ) {
     xi1[0]=b2.x; xi1[1]=b2.y; xi1[2]=b2.z;
     xi2[0]=a2.x; xi2[1]=a2.y; xi2[2]=a2.z;
  }
  else if (ab1<=0 && ab2>=0 && a12>= ab2 ) {
     xi1[0]=a1.x; xi1[1]=a1.y; xi1[2]=a1.z;
     xi2[0]=b2.x; xi2[1]=b2.y; xi2[2]=b2.z;
  }
  else if (ab1>=0 && ab2<=0 && a12>= ab1 ) {
     xi1[0]=a1.x; xi1[1]=a1.y; xi1[2]=a1.z;
     xi2[0]=b1.x; xi2[1]=b1.y; xi2[2]=b1.z;
  }
  else return FALSE;

  if (debug>1)
  {
    printf("a1= %f %f %f a2= %f %f %f b1= %f %f %f b2= %f %f %f \n",
            a1.x,a1.y,a1.z,a2.x,a2.y,a2.z, 
            b1.x,b1.y,b1.z,b2.x,b2.y,b2.z); 
    printf("a12=%f ab1 =%f ab2=%f\n",a12,ab1,ab2);
    printf("p1= %f %f %f, p2=%f %f %f, p3=%f %f %f \n",
	       x1[0],x1[1],x1[2],x2[0],x2[1],xi2[2],x3[0],x3[1],x3[2]);
    printf("q1= %f %f %f, q2=%f %f %f, q3=%f %f %f \n",
	       t2.x1[0],t2.x1[1],t2.x1[2],t2.x2[0],t2.x2[1],t2.x2[1],t2.x3[0],t2.x3[1],t2.x3[2]);
    printf("P1= %f %f %f, P2=%f %f %f \n",
	       xi1[0],xi1[1],xi1[2],xi2[0],xi2[1],xi2[2]);
  }

  return TRUE;

}

------ */



int Triangle::
getRelativeCoordinates( const real x[3], 
			real & alpha1, 
			real & alpha2, 
			const bool & shouldBeInside /* =TRUE */ ) const
//===========================================================================
/// \details 
/// 
///   Determine the coordinates of the point x with respect to this triangle. I.e. solve for alpha1,alpha2 where 
///            x-x1 = alpha1 * v1 + alpha2 * v2
/// 
///   where v1=x2-x1 and v2=x3-x1 are two vectors from the sides of the triangle, (x1,x2,x3)
///     Solve
///  \begin{verbatim}
///          [ v1.v1 v1.v2 ] [ alpha1 ] = [ v1.x ]
///          [ v1.v2 v2.v2 ] [ alpha2 ] = [ v2.x ]
///    alpha1 = ( v1.x * v2.v2 - v2.x * v1.v2 ) /( v1.v1 * v2.v2 - v1.v2 * v1.v2 )
///    alpha2 = ( v1.x * v2.v2 - v2.x * v1.v2 ) /( v1.v1 * v2.v2 - v1.v2 * v1.v2 )
///  \end{verbatim}
/// 
/// \param x (input) : find coordinates of this point.
/// \param alpha1, alpha2 (output) : relative coordinates.
/// \param shouldBeInside (input) : if true, this routine will print out a message if alpha1 or alpha
///    are not in the range [0,1]  ( +/- epsilon), AND return a value of 1
/// \param Return value : 0 on sucess, 1 if shouldBeInside==TRUE and the point is not inside.
/// 
//===========================================================================
{
  real v1[3],v2[3];
  
  v1[0]=x2[0]-x1[0]; 
  v1[1]=x2[1]-x1[1];
  v1[2]=x2[2]-x1[2];

  v2[0]=x3[0]-x1[0]; 
  v2[1]=x3[1]-x1[1];
  v2[2]=x3[2]-x1[2];

  real v1DotV1=SQR(v1[0])+SQR(v1[1])+SQR(v1[2]);
  real v2DotV2=SQR(v2[0])+SQR(v2[1])+SQR(v2[2]);

  real v1DotV2=v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];


  real v1DotX = v1[0]*(x[0]-x1[0])+v1[1]*(x[1]-x1[1])+v1[2]*(x[2]-x1[2]);
  real v2DotX = v2[0]*(x[0]-x1[0])+v2[1]*(x[1]-x1[1])+v2[2]*(x[2]-x1[2]);

  real det = v1DotV1 * v2DotV2 - v1DotV2 * v1DotV2;
  
  if( det==0. )
  {
    if( v1DotV1 > 0. )
      alpha1=v1DotX/v1DotV1;
    else
      alpha1=.5;  // v1 is zero, alpha1 is arbitrary

    if( v2DotV2 > 0. )
      alpha2=v2DotX/v2DotV2;
    else
      alpha2=.5;  // v2 is zero, alpha2 is arbitrary
  }
  else
  {
    alpha1 = ( v1DotX * v2DotV2 - v2DotX * v1DotV2 ) / det;
    alpha2 = ( v2DotX * v1DotV1 - v1DotX * v1DotV2 ) / det;
  }
  
  if( shouldBeInside )
  {
    real eps=REAL_EPSILON*50.;  
    if( fabs(alpha1-.5)>.5+eps || fabs(alpha2-.5)>.5+eps )
    {
      printf("Triangle::getRelativeCoordinates:WARNING point should be inside but alpha1=%e, alpha2=%e,\n"
          " point : x=(%e,%e,%e), triangle : x1=(%e,%e,%e), x2=(%e,%e,%e), x3=(%e,%e,%e) \n",
	   alpha1,alpha2,x[0],x[1],x[2],
	   x1[0],x1[1],x1[2],
	   x2[0],x2[1],x2[2],
	   x3[0],x3[1],x3[2]);
      return 1;
    }
  }
  return 0;
  
}

