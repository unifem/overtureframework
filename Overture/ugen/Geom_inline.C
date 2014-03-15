
#ifndef USE_SARRAY
inline 
//realArray
int
get_circle_center(realArray const &p1, realArray const &p2, realArray const &p3, realArray &center) {

  // get the center of a circle given three points, coding adapted
  // from a Mathematica equation to C dump.

//   cout<<"geom_inline"<<endl;
//   p1.display();
//   p2.display();
//   p3.display();
//   cout<<"geom_inline end"<<endl;

  //realArray c(Range(0,0),Range(0, p1.getLength(1)-1));
  /*
  realArray  p1pow = pow(p1,2);
  realArray  p2pow = pow(p2,2);
  realArray  p3pow = pow(p3,2);

  p1pow.reshape(Range(p1.getBase(1), p1.getBound(1)));
  p2pow.reshape(Range(p2.getBase(1), p2.getBound(1)));
  p3pow.reshape(Range(p3.getBase(1), p3.getBound(1)));
  */
  real x1 = p1(p1.getBase(0),0); 
  real y1 = p1(p1.getBase(0),1); 
  real x2 = p2(p2.getBase(0),0); 
  real y2 = p2(p2.getBase(0),1); 
  real x3 = p3(p3.getBase(0),0); 
  real y3 = p3(p3.getBase(0),1); 

  real denom = 2.0*(x3*y1-x2*y1+x1*y2-x3*y2-x1*y3+x2*y3);

  //cout <<"denom is "<<denom<<" "<<" "<<2.0*(x3*y1-x2*y1+x1*y2-x3*y2-x1*y3+x2*y3)<<" "<<2.*((x3-x2)*(y3-y1) - (x3-x1)*(y3-y2))<<endl;
  real x;
  real y;
  //if (fabs(denom)>0.0) {
  if (fabs(denom)>10.*REAL_EPSILON) {
    x = (pow(x3,2)*(y1-y2)+pow(x2,2)*(y3-y1)+(y2-y3)*(pow(x1,2)+pow(y1,2)-y1*y2-y1*y3+y2*y3))/denom;

    y =-((x1-x3)*(pow(x1,2)-pow(x2,2)+pow(y1,2)-pow(y2,2))-(x1-x2)*(pow(x1,2)-pow(x3,2)+pow(y1,2)-pow(y3,2)))/denom;
  } else {
    //p1.display("p1");
    //p2.display("p2"); 
    //p3.display("p3"); 
    return -1;
  }

  center(center.getBase(0),0) = x;
  center(center.getBase(0),1) = y;
  
  //cout <<" x = "<<x<<"  y = "<<y<<" denom =  "<<denom<<endl;
  //center.display("circle center");
  //return c;
  return 0;
}

inline 
double
triangleArea2D(const realArray &p1, const realArray &p2, const realArray &p3)
{
  
  return 0.5*((p2(0)-p1(0))*(p3(1) - p1(1)) - (p3(0)-p1(0))*(p2(1)-p1(1)));

}

inline
bool
isBetween2D(const realArray &p1, const realArray p2, const realArray &p3)
{
  // is p3 between p1 and p2 ?

  if (p1(0)!=p3(0)) 
    return ((p1(0)<=p3(0) && p3(0)<=p2(0)) ||
	    (p2(0)<=p3(0) && p3(0)<=p1(0)));
  else
    return ((p1(1)<=p3(1) && p3(1)<=p2(1)) ||
	    (p2(1)<=p3(1) && p3(1)<=p1(1)));
}

inline
bool
isBetweenOpenInterval2D(const realArray &p1, const realArray p2, const realArray &p3)
{
  // is p3 between p1 and p2 ?
  if ( fabs(p1(0)-p3(0))>10.*REAL_EPSILON ) 
    return ((p1(0)<p3(0) && p3(0)<p2(0)) ||
	    (p2(0)<p3(0) && p3(0)<p1(0)));
  else
    return ((p1(1)<p3(1) && p3(1)<p2(1)) ||
	    (p2(1)<p3(1) && p3(1)<p1(1)));
}

inline
bool 
angleLessThan(const realArray &a, const realArray &b, real angle)
{

  return angle>acos(sum(a*b)/(sqrt(sum(a*a)*sum(b*b))));
    
}

#else
inline
real
ASdot( const ArraySimple<real> &a, const ArraySimple<real> &b )
{
  real absum = 0.0;
    
  for ( int axis=0; axis<a.size(); axis++ )
    absum += a[axis]*b[axis];
  
  return absum;
}

inline
real
ASdot( const ArraySimpleFixed<real,3,1,1,1> &a, const ArraySimpleFixed<real,3,1,1,1> &b )
{
  real absum = 0.0;
    
  for ( int axis=0; axis<a.size(); axis++ )
    absum += a[axis]*b[axis];
  
  return absum;
}

inline 
real 
ASmag2( const ArraySimple<real> &a )
{
  real mag2=0.0;
  for ( int axis=0; axis<a.size(); axis++ )
    mag2+= a[axis]*a[axis];

  return mag2;
}

//template<int dim>
inline 
real 
ASmag2( const ArraySimpleFixed<real,3,1,1,1> &a )
{
  int dim=3;
  real mag2=0.0;
  for ( int axis=0; axis<dim; axis++ )
    mag2+= a[axis]*a[axis];

  return mag2;
}


inline 
//realArray
int
get_circle_center(ArraySimple<real> const &p1, ArraySimple<real> const &p2, ArraySimple<real> const &p3, ArraySimple<real> &center) {

  // get the center of a circle given three points, coding adapted
  // from a Mathematica equation to C dump.

  real x1 = p1(0);
  real y1 = p1(1);
  real x2 = p2(0);
  real y2 = p2(1);
  real x3 = p3(0);
  real y3 = p3(1);

  real denom = 2.0*(x3*y1-x2*y1+x1*y2-x3*y2-x1*y3+x2*y3);

  real x = center(0) = 0;
  real y = center(1) = 0;

  if (fabs(denom)>10.*REAL_EPSILON) {
    x = (pow((double)x3,2.)*(y1-y2)+pow((double)x2,2.)*(y3-y1)+(y2-y3)*((double)pow(x1,2.)+
         pow((double)y1,2.)-y1*y2-y1*y3+y2*y3))/denom;

    y =-((x1-x3)*(pow((double)x1,2.)-pow((double)x2,2.)+pow((double)y1,2.)-pow((double)y2,2.))-
         (x1-x2)*(pow((double)x1,2.)-pow((double)x3,2.)+pow((double)y1,2.)-pow((double)y3,2.)))/denom;
  } else {

    return -1;
  }

  return 0;
}

inline 
int
get_sphere_center(ArraySimple<real> const &p1, ArraySimple<real> const &p2, ArraySimple<real> const &p3, ArraySimple<real> const &p4, ArraySimple<real> &center) 
{
  // get the center for a sphere given a tetrahedron defined by p1,p2,p3,p4, 
  //   as seen in http://www.faqs.org/faqs/graphics/algorithms-faq/

  if ( orient3d(((ArraySimple<real> &)p1).ptr(), ((ArraySimple<real> &)p2).ptr(), 
		((ArraySimple<real> &)p3).ptr(), ((ArraySimple<real> &)p4).ptr())==0.0 ) return -1;

  ArraySimpleFixed<real,3,1,1,1> dir,ba,ca,da;

  for ( int a=0; a<3; a++ )
    {
      ba[a] = p2[a] - p1[a];
      ca[a] = p3[a] - p1[a];
      da[a] = p4[a] - p1[a];
    }

  real det = ( (ba[0])*( (ca[1])*(da[2])-(ca[2])*(da[1]) ) -
	       (ba[1])*( (ca[0])*(da[2])-(ca[2])*(da[0]) ) +
	       (ba[2])*( (ca[0])*(da[1])-(ca[1])*(da[0]) ) );

  //  if ( fabs(det)<100*FLT_MIN ) 
  //    return -1;

  det *= 2.0;
  real bamag = ASmag2(ba);
  real camag = ASmag2(ca);
  real damag = ASmag2(da);

  dir[0] = (damag*(ba[1]*ca[2]-ba[2]*ca[1]) + camag*(da[1]*ba[2]-da[2]*ba[1]) + bamag*(ca[1]*da[2]-ca[2]*da[1]));
  dir[1] = -(damag*(ba[0]*ca[2]-ba[2]*ca[0]) + camag*(da[0]*ba[2]-da[2]*ba[0]) + bamag*(ca[0]*da[2]-ca[2]*da[0]));
  dir[2] = (damag*(ba[0]*ca[1]-ba[1]*ca[0]) + camag*(da[0]*ba[1]-da[1]*ba[0]) + bamag*(ca[0]*da[1]-ca[1]*da[0]));

  center[0] = p1[0] + dir[0]/det;
  center[1] = p1[1] + dir[1]/det;
  center[2] = p1[2] + dir[2]/det;

  return 0;
}

inline 
double
triangleArea2D(const ArraySimple<real> &p1, const ArraySimple<real> &p2, const ArraySimple<real> &p3)
{
  
  return 0.5*((p2(0)-p1(0))*(p3(1) - p1(1)) - (p3(0)-p1(0))*(p2(1)-p1(1)));

}

inline 
double
triangleArea2D(const ArraySimpleFixed<real,2,1,1,1> &p1, const ArraySimpleFixed<real,2,1,1,1> &p2, const ArraySimpleFixed<real,2,1,1,1> &p3)
{
  
  return 0.5*((p2(0)-p1(0))*(p3(1) - p1(1)) - (p3(0)-p1(0))*(p2(1)-p1(1)));

}

inline 
ArraySimple<real> 
areaNormal3D(const ArraySimple<real> &p1, const ArraySimple<real> &p2, 
	     const ArraySimple<real> &p3)
{
  ArraySimple<real> norm(3);
  norm[0] = norm[1] = norm[2] = 0;

  // divide by 2 so the area is the triangle area
  norm[0] = ( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) )/real(2);
  norm[1] =-( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) )/real(2);
  norm[2] = ( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) )/real(2);

  return norm;
}

inline 
ArraySimpleFixed<real,3,1,1,1> 
areaNormal3D(const ArraySimpleFixed<real,3,1,1,1> &p1, const ArraySimpleFixed<real,3,1,1,1> &p2, 
	     const ArraySimpleFixed<real,3,1,1,1> &p3)
{
  ArraySimpleFixed<real,3,1,1,1> norm;
  norm[0] = norm[1] = norm[2] = 0;

  // divide by 2 so the area is the triangle area
  norm[0] = ( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) )/real(2);
  norm[1] =-( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) )/real(2);
  norm[2] = ( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) )/real(2);

  return norm;
}

inline 
double
tetVolume(const ArraySimple<real> &p1, const ArraySimple<real> &p2, 
	  const ArraySimple<real> &p3, const ArraySimple<real> &p4)
{
  // (p2-p1)x(p3-p1) points in the direction of p4 ( p1,p2,p3 are counter clockwise viewed from p4 )
  // 6 vol = (p4-p1) . ( (p2-p1)x(p3-p1) )
  return  ( (p4[0]-p1[0])*( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) ) -
	    (p4[1]-p1[1])*( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) ) +
	    (p4[2]-p1[2])*( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) ) ) / 6. ;
	  
}

inline 
double
tetVolume(const ArraySimpleFixed<real,3,1,1,1> &p1, const ArraySimpleFixed<real,3,1,1,1> &p2, 
	  const ArraySimpleFixed<real,3,1,1,1> &p3, const ArraySimpleFixed<real,3,1,1,1> &p4)
{
  // (p2-p1)x(p3-p1) points in the direction of p4 ( p1,p2,p3 are counter clockwise viewed from p4 )
  // 6 vol = (p4-p1) . ( (p2-p1)x(p3-p1) )
  return  ( (p4[0]-p1[0])*( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) ) -
	    (p4[1]-p1[1])*( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) ) +
	    (p4[2]-p1[2])*( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) ) ) / 6. ;
	  
}

inline
bool
isBetween2D(const ArraySimple<real> &p1, const ArraySimple<real> p2, const ArraySimple<real> &p3)
{
  // is p3 between p1 and p2 ?

  if ( triangleArea2D(p1,p2,p3)!=0. )
    return false;

  if (p1(0)!=p3(0)) 
    return ((p1(0)<=p3(0) && p3(0)<=p2(0)) ||
	    (p2(0)<=p3(0) && p3(0)<=p1(0)));
  else
    return ((p1(1)<=p3(1) && p3(1)<=p2(1)) ||
	    (p2(1)<=p3(1) && p3(1)<=p1(1)));
}

inline
bool
isBetweenOpenInterval2D(const ArraySimple<real> &p1, const ArraySimple<real> p2, const ArraySimple<real> &p3)
{
  // is p3 between p1 and p2 ?
  if ( triangleArea2D(p1,p2,p3)!=0. )
    return false;

  if ( fabs(p1(0)-p3(0))>10.*REAL_EPSILON ) 
    return ((p1(0)<p3(0) && p3(0)<p2(0)) ||
	    (p2(0)<p3(0) && p3(0)<p1(0)));
  else
    return ((p1(1)<p3(1) && p3(1)<p2(1)) ||
	    (p2(1)<p3(1) && p3(1)<p1(1)));
}


#endif
