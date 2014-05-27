//#define BOUNDS_CHECK
//#define OV_DEBUG
#include "GenericDataBase.h"
#include "OvertureTypes.h"
#include "Geom.h"

#define USE_SARRAY

#ifndef USE_SARRAY
bool intersect2D(realArray const &a, realArray const &b, realArray const &c, realArray const &d, bool &isParallel) {

  double tinynum = 0.00000001;
  //double tinynum = 0.00001; // fraction of a line segment to be excluded
                            // at either vertex 
  //double tinynum = 10*REAL_EPSILON;
  // intersection using parameterizations of the two edges, a la O'Rourke
  double s,t;
  double num1, num2, denom;
  //bool isParallel=false;

  //a.display("a");
  //b.display("b");
  //c.display("c");
  //d.display("d");
  real ax = a(a.getBase(0),0);
  double ay = a(a.getBase(0),1);
  double bx = b(b.getBase(0),0);
  double by = b(b.getBase(0),1);
  double cx = c(c.getBase(0),0);
  double cy = c(c.getBase(0),1);
  double dx = d(d.getBase(0),0);
  double dy = d(d.getBase(0),1);

  denom = ax*(dy-cy) + bx*(cy-dy) + dx*(by-ay) + cx*(ay-by);
  //cout <<"inside intersect "<<denom<<" "<<fabs(denom)<<endl;
  isParallel = false;
  if (fabs(denom)<FLT_EPSILON) isParallel = true;

  if (!isParallel) {
    num1 = ax*(dy-cy) + cx*(ay-dy) + dx*(cy-ay);
    //if ( (num==0.0) || (num==denom) )
    s = num1/denom;

    num2 = -( ax*(cy-by) + bx*(ay-cy) + cx*(by-ay) );
    t = num2/denom;

    //cout<<"s "<<s<<" t  "<<t<<endl;
    //if ( (0.0<s) && (s<1.0) && (0.0<t) && (t<1.0) ) {
    if ( (s>tinynum) && (s<(1.0-tinynum)) &&
	 (t>tinynum) && (t<(1.0-tinynum)) ) {
      //cout<<"caught intersection "<<s<<" "<<t<<endl;
      return true;
    }
  }

  return false;

}

bool checkFaceAngle(const realArray &a, const realArray &b, const realArray &c)
{

  realArray vab = b - a;
  realArray vbc = c - b;
  realArray vac = c - a;
  //  a.display("a");
  // b.display("b");
  //c.display("c");
  real cosang1 = sum(vab*vbc)/(sqrt(sum(pow(vab,2)))*sqrt(sum(pow(vbc,2))));
  //real cosang2 = sum(vab*vac)/(sqrt(sum(pow(vab,2)))*sqrt(sum(pow(vac,2))));
  //cout << "checking face angle : cosang "<<cosang1<<endl;
  //if (acos(cosang1)>=acos(cosang2))
  if (cosang1<=0.0)
    return true;
  else
    return false;

}

#else

bool intersect2D(const ArraySimpleFixed<real,2,1,1,1> &a_, const ArraySimpleFixed<real,2,1,1,1> &b_, 
		 const ArraySimpleFixed<real,2,1,1,1> &c_, const ArraySimpleFixed<real,2,1,1,1> &d_, bool &isParallel)
{

  ArraySimpleFixed<real,2,1,1,1> &a = (ArraySimpleFixed<real,2,1,1,1> &)a_;
  ArraySimpleFixed<real,2,1,1,1> &b = (ArraySimpleFixed<real,2,1,1,1> &)b_;
  ArraySimpleFixed<real,2,1,1,1> &c = (ArraySimpleFixed<real,2,1,1,1> &)c_;
  ArraySimpleFixed<real,2,1,1,1> &d = (ArraySimpleFixed<real,2,1,1,1> &)d_;

  //double tinynum = 0.00000001;
  double tinynum = 10*FLT_EPSILON;
  double s,t;
  double num1, num2, denom;

  real a1,a2;
  a1 = orient2d(a.ptr(),b.ptr(),c.ptr());
  a2 = orient2d(a.ptr(),b.ptr(),d.ptr());

  isParallel = a1==0.0 && a2==0.0;

  if ( (a1>0.0 && a2>0.0) || (a1<0.0 && a2<0.0) ) return false;

  real aa1, aa2;
  if ( a1>0.0 && a2<=0.0 )
    {
      aa1 = orient2d(a.ptr(),d.ptr(),c.ptr());
      aa2 = orient2d(b.ptr(),c.ptr(),d.ptr());
    }
  else
    {
      aa1 = orient2d(a.ptr(),c.ptr(),d.ptr());
      aa2 = orient2d(b.ptr(),d.ptr(),c.ptr());
    }

  return ( aa1>0.0 && aa2>0.0 );

#if 0
  const real &ax = a[0];
  const real &ay = a[1];
  const real &bx = b[0];
  const real &by = b[1];
  const real &cx = c[0];
  const real &cy = c[1];
  const real &dx = d[0];
  const real &dy = d[1];

  denom = ax*(dy-cy) + bx*(cy-dy) + dx*(by-ay) + cx*(ay-by);

  //isParallel = false;
  //if (fabs(denom)<FLT_EPSILON) isParallel = true;

  if (!isParallel) {
    num1 = ax*(dy-cy) + cx*(ay-dy) + dx*(cy-ay);
    s = num1/denom;

    num2 = -( ax*(cy-by) + bx*(ay-cy) + cx*(by-ay) );
    t = num2/denom;

    if ( (s>tinynum) && (s<(1.0-tinynum)) &&
	 (t>tinynum) && (t<(1.0-tinynum)) ) {
      // cout<<"caught intersection "<<s<<" "<<t<<endl;
      return true;
    }
  }

  return false;
#endif

}

bool intersect2D(const ArraySimple<real> &a_, const ArraySimple<real> &b_, 
		 const ArraySimple<real> &c_, const ArraySimple<real> &d_, bool &isParallel)
{
  ArraySimple<real> &a = (ArraySimple<real> &)a_;
  ArraySimple<real> &b = (ArraySimple<real> &)b_;
  ArraySimple<real> &c = (ArraySimple<real> &)c_;
  ArraySimple<real> &d = (ArraySimple<real> &)d_;

  //double tinynum = 0.00000001;
  double tinynum = 10*FLT_EPSILON;
  double s,t;
  double num1, num2, denom;

  real a1,a2;
  a1 = orient2d(a.ptr(),b.ptr(),c.ptr());
  a2 = orient2d(a.ptr(),b.ptr(),d.ptr());

  isParallel = a1==0.0 && a2==0.0;

  if ( (a1>0.0 && a2>0.0) || (a1<0.0 && a2<0.0) ) return false;

  real aa1, aa2;
  if ( a1>0.0 && a2<=0.0 )
    {
      aa1 = orient2d(a.ptr(),d.ptr(),c.ptr());
      aa2 = orient2d(b.ptr(),c.ptr(),d.ptr());
    }
  else
    {
      aa1 = orient2d(a.ptr(),c.ptr(),d.ptr());
      aa2 = orient2d(b.ptr(),d.ptr(),c.ptr());
    }

  //  cout<<aa1<<" "<<aa2<<endl;
  return ( aa1>0.0 && aa2>0.0 );

#if 0
  //double tinynum = 0.00000001;
  double tinynum = 10*FLT_EPSILON;
  double s,t;
  double num1, num2, denom;

  const real &ax = a[0];
  const real &ay = a[1];
  const real &bx = b[0];
  const real &by = b[1];
  const real &cx = c[0];
  const real &cy = c[1];
  const real &dx = d[0];
  const real &dy = d[1];

  denom = ax*(dy-cy) + bx*(cy-dy) + dx*(by-ay) + cx*(ay-by);

  isParallel = false;
  if (fabs(denom)<FLT_EPSILON) isParallel = true;

  if (!isParallel) {
    num1 = ax*(dy-cy) + cx*(ay-dy) + dx*(cy-ay);
    s = num1/denom;

    num2 = -( ax*(cy-by) + bx*(ay-cy) + cx*(by-ay) );
    t = num2/denom;

    if ( (s>tinynum) && (s<(1.0-tinynum)) &&
	 (t>tinynum) && (t<(1.0-tinynum)) ) {
      // cout<<"caught intersection "<<s<<" "<<t<<endl;
      return true;
    }
  }

  return false;
#endif

}

bool intersect3D(const ArraySimpleFixed<real,3,3,1,1> &triVertices, 
		 const ArraySimpleFixed<real,3,1,1,1> &p1_, 
		 const ArraySimpleFixed<real,3,1,1,1> &p2_, 
		 bool &isParallel, real &angle, real ftol /*=0.0*/ )
{

  // this will return true if :
  //    1. the line p2-p1 is in the plane defined by triVertices and intersects the triangle
  //    2. the line p2-p1 is in the plane defined by triVertices and lies inside the triangle
  //    3. the line p2-p1 intersects the plane inside the triangle

  // maybe this method should return some kind of feedback classifying the interaction

  ArraySimpleFixed<real,3,1,1,1> &p1 = (ArraySimpleFixed<real,3,1,1,1> &)p1_;
  ArraySimpleFixed<real,3,1,1,1> &p2 = (ArraySimpleFixed<real,3,1,1,1> &)p2_;

  isParallel = false;
  bool intersects = false;

  ArraySimpleFixed<real,3,1,1,1> t1,t2,t3;
  int a;

  real pmag=0;
  
  for ( a=0; a<3; a++ )
    {
      t1[a] = triVertices(0,a);
      t2[a] = triVertices(1,a);
      t3[a] = triVertices(2,a);
 //      p1[a] = p1_[a];
//       p2[a] = p2_[a];
      pmag += (p1[a]-p2[a])*(p1[a]-p2[a]);
    }

  real at = sqrt(ASmag2(areaNormal3D(t1,t2,t3)));
  real vol1 = orient3d(t1.ptr(),t2.ptr(),t3.ptr(), p1.ptr());
  real vol2 = orient3d(t1.ptr(),t2.ptr(),t3.ptr(), p2.ptr());

  real partol = (fabs(vol1)+fabs(vol2))/(at*sqrt(pmag));
  real flatTolerance = max(ftol,FLT_EPSILON); // cos(90-smallAngle) = flatTolerance

  //cout<<"base vols "<<vol1<<" "<<vol2<<endl;
  //if ( fabs(vol1)<1e-15 && fabs(vol2)<1e-15 ) cout<<"suspect base vols "<<vol1<<" "<<vol2<<" "<<partol<<endl;

  //  if ( partol<flatTolerance && (vol1!=0.0 || vol2!=0.0) ) 
  //    cout<<"partol, flatTolerance "<<partol<<" "<<flatTolerance<<endl;

  angle=REAL_MAX;
  if ( (vol1==0.0 && vol2!=0.0) || (vol1!=0.0 && vol2==0.0) ) angle = partol;
  
  if ( (vol1>0.0 && vol2>0.0) || ( vol1<0.0 && vol2<0.0 ) )
    return false;
  else if ( (vol1==0.0 && vol2==0.0) || (vol1==0.0 && partol<flatTolerance) || (partol<flatTolerance &&vol2==0.0) )
    {
      ArraySimpleFixed<real,3,1,1,1> tnorm = areaNormal3D(t1,t2,t3);
      real tarea = sqrt(ASmag2(tnorm));
      
      real pdotn = 0;
      real pmag = 0;
      real maxdist = -REAL_MAX;
      for ( a=0; a<3; a++ )
	{
	  tnorm[a]/=tarea;
	  pdotn += tnorm[a]*(p2[a]-p1[a]);
	  maxdist = max(maxdist,fabs(p2[a]-p1[a]));
	  pmag += (p2[a]-p1[a])*(p2[a]-p1[a]);
	}

      isParallel = true;
      //cout<<"isParallel true, performing 2D check "<<endl;
      ArraySimpleFixed<real,2,1,1,1> e1,e2, tt1,tt2,tt3;

      // project to 2D and determine the intersection
      if ( fabs(tnorm[2]) > FLT_EPSILON ) // ok to project onto z plane
	{
	  //cout<<"project onto z"<<endl;
	  e1[0] = p1[0];
	  e1[1] = p1[1];
	  e2[0] = p2[0];
	  e2[1] = p2[1];
	  tt1[0] = t1[0];
	  tt1[1] = t1[1];
	  tt2[0] = t2[0];
	  tt2[1] = t2[1];
	  tt3[0] = t3[0];
	  tt3[1] = t3[1];
	}
      else if ( fabs(tnorm[1])>FLT_EPSILON ) // ok to project onto y plane
	{
	  //cout<<"project onto y"<<endl;
	  e1[0] = p1[0];
	  e1[1] = p1[2];
	  e2[0] = p2[0];
	  e2[1] = p2[2];
	  tt1[0] = t1[0];
	  tt1[1] = t1[2];
	  tt2[0] = t2[0];
	  tt2[1] = t2[2];
	  tt3[0] = t3[0];
	  tt3[1] = t3[2];
	}
      else
	{ 
	  //cout<<"project onto x"<<endl;
	  e1[0] = p1[1];
	  e1[1] = p1[2];
	  e2[0] = p2[1];
	  e2[1] = p2[2];
	  tt1[0] = t1[1];
	  tt1[1] = t1[2];
	  tt2[0] = t2[1];
	  tt2[1] = t2[2];
	  tt3[0] = t3[1];
	  tt3[1] = t3[2];
	}

#if 0
      cout<<"performing 2D intersection "<<endl;

      cout<<"e1 "<<e1[0]<<" "<<e1[1]<<endl;
      cout<<"e2 "<<e2[0]<<" "<<e2[1]<<endl;
      cout<<"tt1 "<<tt1[0]<<" "<<tt1[1]<<endl;
      cout<<"tt2 "<<tt2[0]<<" "<<tt2[1]<<endl;
      cout<<"tt3 "<<tt3[0]<<" "<<tt3[1]<<endl;
#endif
      bool edgeParallel = false;

      if ( !(intersects = intersect2D(e1,e2,tt1,tt2, edgeParallel)) )
	if ( !(intersects = intersect2D(e1,e2,tt2,tt3, edgeParallel)) )
	  intersects = intersect2D(e1,e2,tt3,tt1, edgeParallel);
      
      // if it does not intersect, does it lie entirely inside the triangle?
      if ( !intersects )
	{
	  bool isInside = false;
	  //cout<<"checking inside "<<endl;
	  real torient = orient2d(tt1.ptr(),tt2.ptr(),tt3.ptr())>0 ? 1 : -1;

	  bool inside1=false, inside2=false;
#if 0
	  inside1 = ( torient*triangleArea2D(tt1,tt2,e1)>100*FLT_MIN && 
		      torient*triangleArea2D(tt2,tt3,e1)>100*FLT_MIN && 
		      torient*triangleArea2D(tt3,tt1,e1)>100*FLT_MIN );
	  
	  inside2 = ( torient*triangleArea2D(tt1,tt2,e2)>100*FLT_MIN && 
		      torient*triangleArea2D(tt2,tt3,e2)>100*FLT_MIN && 
		      torient*triangleArea2D(tt3,tt1,e2)>100*FLT_MIN );

#endif
	  inside1 = ( torient*orient2d(tt1.ptr(),tt2.ptr(),e1.ptr())>0. &&
		      torient*orient2d(tt2.ptr(),tt3.ptr(),e1.ptr())>0. &&
		      torient*orient2d(tt3.ptr(),tt1.ptr(),e1.ptr())>0. );
	  inside2 = ( torient*orient2d(tt1.ptr(),tt2.ptr(),e2.ptr())>0. &&
		      torient*orient2d(tt2.ptr(),tt3.ptr(),e2.ptr())>0. &&
		      torient*orient2d(tt3.ptr(),tt1.ptr(),e2.ptr())>0. );

	  isInside = ( inside1 || inside2 );

	  if ( !isInside )
	    {
	      if ( orient2d(tt1.ptr(),tt2.ptr(),e1.ptr())==0. ||
		   orient2d(tt2.ptr(),tt3.ptr(),e1.ptr())==0. ||
		   orient2d(tt3.ptr(),tt1.ptr(),e1.ptr())==0. )
		isInside = inside2;
	      else if ( orient2d(tt1.ptr(),tt2.ptr(),e2.ptr())==0. ||
			orient2d(tt2.ptr(),tt3.ptr(),e2.ptr())==0. ||
			orient2d(tt3.ptr(),tt1.ptr(),e2.ptr())==0. )
		isInside = inside1;
	    }

	  intersects = isInside;

	}
    }
  else
    {
      // The line p2-p1 intersects the plane, does it intersect inside the triangle?
      
      // classify by volumes a la O'Rourke "Computational Geometry in C"

      real vol12,vol23,vol31;
      if ( vol1>0 && vol2<=0 )
	{
	  vol12 = orient3d(t1.ptr(),t2.ptr(),p2.ptr(),p1.ptr());
	  vol23 = orient3d(t2.ptr(),t3.ptr(),p2.ptr(),p1.ptr());
	  vol31 = orient3d(t3.ptr(),t1.ptr(),p2.ptr(),p1.ptr());
	}
      else
	{
	  vol12 = orient3d(t1.ptr(),p2.ptr(),t2.ptr(),p1.ptr());
	  vol23 = orient3d(t2.ptr(),p2.ptr(),t3.ptr(),p1.ptr());
	  vol31 = orient3d(t3.ptr(),p2.ptr(),t1.ptr(),p1.ptr());
	}

      bool oneVertexOnFace = ( (fabs(vol1)>0 && vol2==0) || (fabs(vol2)>0 && vol1==0) );
 
      //cout<<"vols "<<vol12<<" "<<vol23<<" "<<vol31<<endl;
      int npos = 0;
      int nzero = 0;

      if ( vol12>0.0 ) npos++;
      if ( vol23>0.0 ) npos++;
      if ( vol31>0.0 ) npos++;
      if ( vol12 == 0.0 ) nzero++;
      if ( vol23 == 0.0 ) nzero++;
      if ( vol31 == 0.0 ) nzero++;

      intersects = (npos==3) || ( npos==2 && nzero==1) ;
    }
  
  return intersects;
}

static inline void eval_F(ArraySimpleFixed<real,3,1,1,1> &F, 
			  const real &x, const real &y, const real &z,
			  const real &x1, const real &y1, const real &z1,
			  const real &x2, const real &y2, const real &z2,
			  const real &x3, const real &y3, const real &z3,
			  const ArraySimple<real> &n)
{
  F[0] = (x1-x)*n[0] + (y1-y)*n[1] + (z1-z)*n[2];
  F[1] = ( (x1-x)*(x1-x) + (y1-y)*(y1-y) + (z1-z)*(z1-z) -
	   (x2-x)*(x2-x) + (y2-y)*(y2-y) + (z2-z)*(z2-z) );
  F[2] = ( (x1-x)*(x1-x) + (y1-y)*(y1-y) + (z1-z)*(z1-z) -
	   (x3-x)*(x3-x) + (y3-y)*(y3-y) + (z3-z)*(z3-z) );
}

static inline void eval_dFdXc(ArraySimpleFixed<real,3,3,1,1> &wr,
			      const real &x, const real &y, const real &z,
			      const real &x1, const real &y1, const real &z1,
			      const real &x2, const real &y2, const real &z2,
			      const real &x3, const real &y3, const real &z3,
			      const ArraySimple<real> &n)
{

  wr(0,0) = -n[0];
  wr(0,1) = -n[1];
  wr(0,2) = -n[2];

  wr(1,0) = 2*( x2-x1 );
  wr(1,1) = 2*( y2-y1 );
  wr(1,2) = 2*( z2-z1 );

  wr(2,0) = 2*( x3-x1 );
  wr(2,1) = 2*( y3-y1 );
  wr(2,2) = 2*( z3-z1 );

}

int
get_circle_center_on_plane(ArraySimple<real> const &p1, 
			   ArraySimple<real> const &p2, 
			   ArraySimple<real> const &p3, ArraySimple<real> &center) {

  const real &x1 = p1(0);
  const real &y1 = p1(1);
  const real &z1 = p1(2);
  const real &x2 = p2(0);
  const real &y2 = p2(1);
  const real &z2 = p2(2);
  const real &x3 = p3(0);
  const real &y3 = p3(1);
  const real &z3 = p3(2);

  real lineCheck = ( ((x2-x1)*(x3-x1)+(y2-y1)*(y3-y1)+(z2-z1)*(z3-z1))/
		     sqrt( ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1))*
			   ((x3-x1)*(x3-x1)+(y3-y1)*(y3-y1)+(z3-z1)*(z3-z1)) ) );

  real &x = center(0);
  real &y = center(1);
  real &z = center(2);

  if ( fabs(lineCheck)>10*REAL_EPSILON )
    {
      ArraySimple<real> n = areaNormal3D(p1,p2,p3);

      ArraySimpleFixed<real,3,1,1,1> F,h;
      ArraySimpleFixed<real,3,3,1,1> dFdXc,inv_dFdXc;

      // create the initial guess
      x = (x1+x2+x3)/3.;
      y = (y1+y2+y3)/3.;
      z = (z1+z2+z3)/3.;
	  
      eval_F(F,x,y,z,
	     x1,y1,z1,
	     x2,y2,z2,
	     x3,y3,z3,
	     n);

      eval_dFdXc(dFdXc,x,y,z,
		 x1,y1,z1,
		 x2,y2,z2,
		 x3,y3,z3,
		 n);
      
      double det = dFdXc(0,0)*(dFdXc(1,1)*dFdXc(2,2)-dFdXc(1,2)*dFdXc(2,1)) - 
	dFdXc(0,1)*(dFdXc(1,0)*dFdXc(2,2)-dFdXc(1,2)*dFdXc(2,0)) +
	dFdXc(0,2)*(dFdXc(1,0)*dFdXc(2,1)-dFdXc(1,1)*dFdXc(2,0));
      
      assert(det!=0);
      
      inv_dFdXc(0,0) = (dFdXc(1,1)*dFdXc(2,2)-dFdXc(1,2)*dFdXc(2,1))/det;
      inv_dFdXc(1,0) = -(dFdXc(1,0)*dFdXc(2,2)-dFdXc(1,2)*dFdXc(2,0))/det;
      inv_dFdXc(2,0) = (dFdXc(1,0)*dFdXc(2,1)-dFdXc(1,1)*dFdXc(2,0))/det;
      inv_dFdXc(0,1) = -(dFdXc(0,1)*dFdXc(2,2)-dFdXc(0,2)*dFdXc(2,1))/det;
      inv_dFdXc(1,1) = (dFdXc(0,0)*dFdXc(2,2)-dFdXc(0,2)*dFdXc(2,0))/det;
      inv_dFdXc(2,1) = -(dFdXc(0,0)*dFdXc(2,1)-dFdXc(0,1)*dFdXc(2,0))/det;
      inv_dFdXc(0,2) = (dFdXc(0,1)*dFdXc(1,2)-dFdXc(0,2)*dFdXc(1,1))/det;
      inv_dFdXc(1,2) = -(dFdXc(0,0)*dFdXc(1,2)-dFdXc(0,2)*dFdXc(1,0))/det;
      inv_dFdXc(2,2) = (dFdXc(0,0)*dFdXc(1,1)-dFdXc(0,1)*dFdXc(1,0))/det;
      
      // one step of a newton iteration, note that the Jacobian, dFdXc, is a constant
      for ( int a=0; a<3; a++ ) h[a] = 0;

      for ( int a1=0; a1<3; a1++ )
	for ( int a2=0; a2<3; a2++ )
	  {
	    h[a1] -= F[a2]*inv_dFdXc(a1,a2);
	  }
      
      x += h[0];
      y += h[1];
      z += h[2];
      
      //      eval_F(F,x,y,z,
      //	     x1,y1,z1,
      //	     x2,y2,z2,
      //	     x3,y3,z3,
      //	     n);

      //cout<< min(fabs(F[0]),fabs(F[1]),fabs(F[2]))<<endl;
      
    }
  else
    return -1;

  return 0;
}

#endif




      
      

    

    

    

  
