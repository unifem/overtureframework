#include <A++.h>

#include <string.h>

#include "realPrecision.h"     // define real to be float or double

#include "wdhdefs.h"           // some useful defines and constants

#include "mathutil.h"          // define max, min,  etc

#include <assert.h>

#include "Mapping.h"

#include "maputil.h" 

//int domainDimension;
//int rangeDimension;
//Index xAxes;

/* *******************
//===================================================================================
//              intersectLine
//
//  Find all points
//             xI=x+s*vector,   s an integer,
//   that lie inside the box.
//
// Input -
//  x : target point (find periodic images of this point)
//  vector : look for integers, s, with  x+s*vector inside the box
//  nI : save points in xI(.,nI), xI(.,nI+1), ...
// Output -
//  xI : image points that we found
//  nI : points to the last point in the list
//====================================================================================
//void ApproximateGlobalInverse::
void intersectLine( const RealArray & x, int & nI, RealArray & xI, 
               const RealArray & vector, const RealArray xOrigin, const RealArray xTangent )
{

  RealArray xa(3),xba(3),xca(3),sp(3),xma(3);
  real det,s1,s2,s;  

  Index xAxes(axis1,rangeDimension);  // ******

  // Get intersections of the line y(s) = x + s*periodicityVector with
  // the faces (edges) of the boundingBox
  int intersection=0;
  for( int dir=axis1; dir<domainDimension; dir++ )
  {
    for( int side=Start; side<=End; side++ )
    { 
      // intersect the line with the plane (line) defined by
      //    xa+s1*xba+s2*xca
      xa(xAxes)=xOrigin(xAxes,side,dir);
      xba(xAxes)=xTangent(xAxes,axis1,side,dir);

      // xa.display("Here is xa");
      // xba.display("Here is xba");
    
      switch (rangeDimension)
      {
      case 2:
        // Solve :    [ -vector(0) xba(0) ][ s ] = [ x(0)-xa(0) ]
        //            [ -vector(1) xba(1) ][ s1] = [ x(1)-xa(1) ]
        det=-vector(axis1)*xba(axis2)+vector(axis2)*xba(axis1);
        // cout << " det =" << det << endl;
        if( det!=0. )
        {  // for now assume parallel lines don't intersect
        
          s1=(-vector(axis1)*(x(axis2)-xa(axis2))+vector(axis2)*(x(axis1)-xa(axis1)))/det;
          // cout << " dir = " << dir << ", side=" << side << ", s1=" << s1 << endl;
       
          if( s1>=0. && s1<=1. )
          {
            s=((x(axis1)-xa(axis1))*xba(axis2)-(x(axis2)-xa(axis2))*xba(axis1))/det;
            sp(intersection++)=s;
          }
	}
	break;
      case 3:
        // Solve :    [ -vector(0) xba(0) xca(0) ][ s ] = [ x(0)-xa(0) ]
        //            [ -vector(1) xba(1) xca(1) ][ s1] = [ x(1)-xa(1) ]
        //            [ -vector(2) xba(2) xca(2) ][ s2] = [ x(2)-xa(2) ]
        xca(xAxes)=xTangent(xAxes,axis2,side,dir);
        det=-vector(axis1)*( xba(axis2)*xca(axis3)-xba(axis3)*xca(axis2) )
            +vector(axis2)*( xba(axis1)*xca(axis3)-xba(axis3)*xca(axis1) )
            -vector(axis3)*( xba(axis1)*xca(axis2)-xba(axis2)*xca(axis1) );
        // cout << " det =" << det << endl;
        if( det!=0. )
        {  // for now assume parallel lines don't intersect
        
          xma(xAxes)=x(xAxes)-xa(xAxes);
          s1=(-vector(axis1)*( xma(axis2)*xca(axis3)-xma(axis3)*xca(axis2) )
              +vector(axis2)*( xma(axis1)*xca(axis3)-xma(axis3)*xca(axis1) )
              -vector(axis3)*( xma(axis1)*xca(axis2)-xma(axis2)*xca(axis1) ))/det;
          // cout << " dir = " << dir << ", side=" << side << ", s1=" << s1 << endl;
       
          if( s1>=0. && s1<=1. )
          {
          s2=(-vector(axis1)*( xba(axis2)*xma(axis3)-xba(axis3)*xma(axis2) )
              +vector(axis2)*( xba(axis1)*xma(axis3)-xba(axis3)*xma(axis1) )
              -vector(axis3)*( xba(axis1)*xma(axis2)-xba(axis2)*xma(axis1) ))/det;
            // cout << " dir = " << dir << ", side=" << side << ", s2=" << s2 << endl;
            if( s2>=0. && s2<=1. )
            {
              s=(xma(axis1)*( xba(axis2)*xca(axis3)-xba(axis3)*xca(axis2) )
                -xma(axis2)*( xba(axis1)*xca(axis3)-xba(axis3)*xca(axis1) )
                +xma(axis3)*( xba(axis1)*xca(axis2)-xba(axis2)*xca(axis1) ))/det;
              sp(intersection++)=s;
	    }
          }
	}
	
	
        break;
      default:
        cerr << " intersectLine::ERROR rangeDimension =" << rangeDimension << endl;
      } // end switch
    }
  }

  int xIBound = xI.getBound(axis2);  // maximum entry in xI
  
  if( intersection > 0 )
  { // find all integral mutiples of p that lie between the points
    // of intersection ** Here we assume the bounding box is slightly larger ***
    Index I(axis1,intersection);  // there may be 2,4 or 6 intersections
    // cout << "intersection=" << intersection 
    //     << " sp(0) = " << sp(0) << ", sp(1) =" << sp(1) << endl;
    for( int i = int(ceil(min(sp(I)))); i<= int(floor(max(sp(I)))); i++ )
    {
      if( nI+1 > xIBound )
      {
	cerr << "intersectLine:Error not enough space in xI ! " << endl;
	exit(1);
      }
      xI(xAxes,nI++)=x(xAxes)+i*vector(xAxes);
    }
  }
  
}



//===================================================================================
//              intersectPlane
//
//  Use this routine when the periodicityOfSpace==2 and rangeDimension==2
//  in order to compute the perioidc image points of the point x that lie
//  in the bounding box
//  
//
// Method:
//  Image points are of the form
//        x+ alpha1*vector1 + alpha2*vector2
//
//  (1) Determine the range of possible alpha2 by computing the alpha2 corresponding
//      to all corners of the box, giving  alpha2Min <= alpha2 <= alpha2Max
//  (2) For each integer i2 with alpha2Min <= i2 <= alpha2MAx, intersect the line
//           y(alpha1) = x + alpha1*vector1 + i2*vector2 
//      with the bounding box.
//
//==================================================================================
// void ApproximateGlobalInverse::
void intersectPlane( const RealArray & x, int & nI, RealArray & xI, 
               const RealArray & vector, const RealArray xOrigin, const RealArray xTangent )
{
  // Get period vector coordinates of all corners (xOrigin)
  // Solve  x+alpha1*vector1+alpha2*vector2 = xCorner
  //  -> a*alpha = xCorner-x
  //  ...we really on use alpha2

  Index xAxes(axis1,rangeDimension);  // ******

  RealArray a(2,2);

  a(axis1,axis1)=vector(axis1,axis1)*vector(axis1,axis1)   // vector1^T vector1
                +vector(axis2,axis1)*vector(axis2,axis1);
  a(axis1,axis2)=vector(axis1,axis1)*vector(axis1,axis2)   // vector1^T vector2
                +vector(axis2,axis1)*vector(axis2,axis2);
  a(axis2,axis1)=a(axis1,axis2);                           // vector2^T vector1
  a(axis2,axis2)=vector(axis1,axis2)*vector(axis1,axis2)   // vector1^T vector1
                +vector(axis2,axis2)*vector(axis2,axis2);
  real det=a(axis1,axis1)*a(axis2,axis2)-a(axis1,axis2)*a(axis2,axis1);
  if( det==0. )
  {
    cerr << "intersectPlane::Error det(vector1,vector2)=0 !" << endl;
    exit(1);
  }
  
  real alpha2Min=1.e10;      // ****
  real alpha2Max=-alpha2Min;
  RealArray x0(3);
  
  for( int dir=axis1; dir<domainDimension; dir++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      x0(xAxes)=xOrigin(xAxes,side,dir)-x(xAxes);
      //      alpha(axis1,side,dir)=
      //	(x0(axis1)*a(axis2,axis2)-x0(axis2)*a(axis1,axis2))/det;
      real alpha2=(a(axis1,axis1)*x0(axis2)-a(axis2,axis1)*x0(axis1))/det;
      alpha2Min=min(alpha2Min,alpha2);
      alpha2Max=max(alpha2Max,alpha2);
    }
  }

  // for lines parallel to vector(.,axis2) find points where they
  // intersect the bounding box
  RealArray x2(3);
  
  for( int i2=int(ceil(alpha2Min)); i2<=int(floor(alpha2Max)); i2++ )
  {
    x2(xAxes)=x(xAxes)+i2*vector(xAxes,axis2);
    intersectLine( x2, nI, xI, vector, xOrigin, xTangent );
  }
}

//===================================================================================
//              intersectCube
//
//  Use this routine when the periodicityOfSpace==3 and rangeDimension==3
//  in order to compute the perioidic image points of the point x that lie
//  in the bounding box
//  
//
// Method:
//  Image points are of the form
//        x+ alpha1*vector1 + alpha2*vector2 + alpha3*vector3
//
//  (1) Determine the range of possible alpha2[3] by computing the alpha2[3] corresponding
//      to all corners of the box, giving  alpha2[3]Min <= alpha2[3] <= alpha2[3]Max
//  (2) For each integer i2[3] with alpha2[3]Min <= i2[3] <= alpha2[3]MAx, intersect the line
//           y(alpha1) = x + alpha1*vector1 + i2*vector2 + i3*vector3
//      with the bounding box.
//
//==================================================================================
// void ApproximateGlobalInverse::
void intersectCube( const RealArray & x, int & nI, RealArray & xI, 
               const RealArray & vector, const RealArray xOrigin, const RealArray xTangent )
{
  // Get period vector coordinates of all corners (xOrigin)
  // Solve  x+alpha1*vector1+alpha2*vector2 = xCorner
  //  -> a*alpha = xCorner-x
  //  ...we really on use alpha2

  Index xAxes(axis1,rangeDimension);  // ******

  RealArray a(3,3);

  for( int i2=axis1; i2<=axis3; i2++ )
  {
    for( int i1=axis1; i1<=axis3; i1++ )
    {
      a(i1,i2)=vector(axis1,i1)*vector(axis1,i2)   // vector(i1)^T vector(i2)
              +vector(axis2,i1)*vector(axis2,i2)
              +vector(axis3,i1)*vector(axis3,i2);
    }
  }
  
  real det=a(axis1,axis1)*(a(axis2,axis2)*a(axis3,axis3)-a(axis3,axis2)*a(axis2,axis3))
          +a(axis2,axis1)*(a(axis3,axis2)*a(axis1,axis3)-a(axis1,axis2)*a(axis3,axis3))
	  +a(axis3,axis1)*(a(axis1,axis2)*a(axis2,axis3)-a(axis2,axis2)*a(axis1,axis3));
  
  if( det==0. )
  {
    cerr << "intersectCube::Error det(vector1,vector2)=0 !" << endl;
    exit(1);
  }
  
  real alpha2,alpha3;
  
  real alpha2Min=1.e10;      // ****
  real alpha2Max=-alpha2Min;
  real alpha3Min=1.e10;      // ****
  real alpha3Max=-alpha3Min;
  RealArray x0(3);
  
  for( int dir=axis1; dir<domainDimension; dir++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      x0(xAxes)=xOrigin(xAxes,side,dir)-x(xAxes);
      //      alpha(axis1,side,dir)=
      //	(x0(axis1)*a(axis2,axis2)-x0(axis2)*a(axis1,axis2))/det;
      alpha2=(a(axis1,axis1)*(x0(axis2)*a(axis3,axis3)-x0(axis3)*a(axis2,axis3))
             +a(axis2,axis1)*(x0(axis3)*a(axis1,axis3)-x0(axis1)*a(axis3,axis3))
	     +a(axis3,axis1)*(x0(axis1)*a(axis2,axis3)-x0(axis2)*a(axis1,axis3)))/det;
      alpha3=(a(axis1,axis1)*(a(axis2,axis2)*x0(axis3)-a(axis3,axis2)*x0(axis2))
             +a(axis2,axis1)*(a(axis3,axis2)*x0(axis1)-a(axis1,axis2)*x0(axis3))
             +a(axis3,axis1)*(a(axis1,axis2)*x0(axis2)-a(axis2,axis2)*x0(axis1)))/det;
      alpha2Min=min(alpha2Min,alpha2);
      alpha2Max=max(alpha2Max,alpha2);
      alpha3Min=min(alpha3Min,alpha3);
      alpha3Max=max(alpha3Max,alpha3);
    }
  }

  RealArray x2(3);
  
//  Intersect the line  x2(alpha1)=x+alpha1*vector1+i2*vector2+i3*vector3
//  with the bounding box

  for( int i3=int(ceil(alpha3Min)); i3<=int(floor(alpha3Max)); i3++ )
  {
    for( int i2=int(ceil(alpha2Min)); i2<=int(floor(alpha2Max)); i2++ )
    {
      x2(xAxes)=x(xAxes)+i2*vector(xAxes,axis2)+i3*vector(xAxes,axis3);
      intersectLine( x2, nI, xI, vector, xOrigin, xTangent );
    }
  }
}

 ******************* */


void main()
{
  
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

  Mapping::debug=15;

  const int max=100;
  RealArray xI(3,max);
  RealArray x(3);
  int nI;
  RealArray periodicityVector(3,3);
  RealArray xOrigin(3,2,3);
  RealArray xTangent(3,2,2,3);
  

  real xmin, xmax, ymin, ymax, zmin, zmax;
  xmin=0.; xmax=1.; ymin=0.; ymax=1.; zmin=0.; zmax=1.;
  
  // faces:  axis1=Start,End:
  int dir=axis1;
  for( int side=Start; side<=End; side++ )
  {
    xOrigin(axis1,side,dir)=(side==Start? xmin : xmax);
    xOrigin(axis2,side,dir)=ymin;
    xOrigin(axis3,side,dir)=zmin;

    xTangent(axis1,axis1,side,dir)=0.;          // Tangent vector 1
    xTangent(axis2,axis1,side,dir)=ymax-ymin;
    xTangent(axis3,axis1,side,dir)=0.;

    xTangent(axis1,axis2,side,dir)=0.;          // Tangent vector 2
    xTangent(axis2,axis2,side,dir)=0.;
    xTangent(axis3,axis2,side,dir)=zmax-zmin;
  }
  // faces:  axis2=Start,End:
  dir=axis2;
  for( side=Start; side<=End; side++ )
  {
    xOrigin(axis1,side,dir)=xmin;
    xOrigin(axis2,side,dir)=(side==Start? ymin : ymax);
    xOrigin(axis3,side,dir)=zmin;

    xTangent(axis1,axis1,side,dir)=xmax-xmin;          // Tangent vector 1
    xTangent(axis2,axis1,side,dir)=0.;
    xTangent(axis3,axis1,side,dir)=0.;

    xTangent(axis1,axis2,side,dir)=0.;          // Tangent vector 2
    xTangent(axis2,axis2,side,dir)=0.;
    xTangent(axis3,axis2,side,dir)=zmax-zmin;
  }
  // faces:  axis3=Start,End:
  dir=axis3;
  for( side=Start; side<=End; side++ )
  {
    xOrigin(axis1,side,dir)=xmin;
    xOrigin(axis2,side,dir)=ymin;
    xOrigin(axis3,side,dir)=(side==Start? zmin : zmax);

    xTangent(axis1,axis1,side,dir)=xmax-xmin;          // Tangent vector 1
    xTangent(axis2,axis1,side,dir)=0.;
    xTangent(axis3,axis1,side,dir)=0.;

    xTangent(axis1,axis2,side,dir)=0.;          // Tangent vector 2
    xTangent(axis2,axis2,side,dir)=ymax-ymin;
    xTangent(axis3,axis2,side,dir)=0.;
  }
  
  
  RealArray vector(3,3);
  
  vector(axis1,axis1)=.4;  // periodicity vector 1
  vector(axis2,axis1)=.0;
  vector(axis3,axis1)=0.;

  vector(axis1,axis2)=.0;  // periodicity vector 2
  vector(axis2,axis2)=.4;
  vector(axis3,axis2)=0.;
  
  vector(axis1,axis3)=.0;  // periodicity vector 2
  vector(axis2,axis3)=.0;
  vector(axis3,axis3)=.4;
  

//  domainDimension=3;
//  rangeDimension=3;
  BoxMapping map(0.,1.,0.,1.,0.,1.);
  RealArray r(3),rx(3,3);
  r=0.;
  x=0.;
  map.inverseMap( x,r,rx );
    
  
  nI=0;
  x(axis1)=.5;  // look for the image of this point
  x(axis2)=.5;
  x(axis3)=.5;
  
  cout << ">>>>>>>>>>>>>>Call intersectLine..." << endl;
  map.approximateGlobalInverse->intersectLine( x, nI, xI, vector, xOrigin, xTangent );
  cout << " nI = " << nI << endl;
  

  for( int i=0; i<nI; i++ )
  {
    cout << " i=" << i << " xI = (" << xI(axis1,i) << "," << xI(axis2,i) << ")" << endl;
  }


  nI=0;
  cout << "***********Call intersectPlane.." << endl;
  map.approximateGlobalInverse->intersectPlane( x, nI, xI, vector, xOrigin, xTangent );
  cout << " nI = " << nI << endl;
  

  for( i=0; i<nI; i++ )
  {
    cout << " i=" << i << " xI = (" << xI(axis1,i) << "," << xI(axis2,i) << ")" << endl;
  }


//  domainDimension=3;
//  rangeDimension=3;
  BoxMapping map2(0.,1.,0.,1.,0.,1.);
  map2.inverseMap( x,r,rx );
    
  nI=0;
  cout << "++++++++Call intersectCube.." << endl;
  map2.approximateGlobalInverse->intersectCube( x, nI, xI, vector, xOrigin, xTangent );
  cout << " nI = " << nI << endl;
  

  for( i=0; i<nI; i++ )
  {
    cout << " i=" << i << " xI = (" << xI(axis1,i) << "," << xI(axis2,i) << ","
         << xI(axis3,i) << ")" << endl;
  }

}



