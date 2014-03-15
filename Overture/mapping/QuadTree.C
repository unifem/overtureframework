#include "QuadTree.h"

int TMquad::maxsquares(0);
real TMquad::dxMinNormal(0.0625);
real TMquad::dxMin2Curve(0.015625);
real TMquad::smallestSquareWidth(1);
int TMquad::nextID(0);

TMquad::TMquad() :
  centerX(0), centerY(0), dx(1), inside(0), curves(), children(NULL)
//===========================================================================
/// \brief  Default Constructor for the quadtree mesh class TMquad
//===========================================================================
{
  ++maxsquares;
};

TMquad::TMquad( TrimmedMapping& tm, const real& centerX_, const real& centerY_,
		const real dx_ ) :
  centerX(centerX_), centerY(centerY_), dx(dx_), inside(-123), children(NULL)
//===========================================================================
/// \brief  Create a quadtree mesh for use in trimmed surface computations.
///  The constructor makes a single-level mesh (just a square), which can later
///  be subdivided by calling the function TMquad::divide.
/// \param tm (input) : the TrimmedMapping object this is for
/// \param centerX_ (input) : the x-coordinate of the center of the square
/// \param centerY_ (input) : the y-coordinate of the center of the square
/// \param dx_ (input): half the width of the square
//===========================================================================
{
  ++maxsquares;
  TMcurves curves_(tm);
  remake( tm, centerX, centerY, dx, curves_ );
};

void TMquad::remake( TrimmedMapping& tm, const real& centerX_,
		     const real& centerY_, const real& dx_, TMcurves& curves_ )
  // centerX_,centerY_ and dx_ are the center point and half the width
  // of this square.
{
  centerX = centerX_;
  centerY = centerY_;
  dx = dx_;
  inside = -123;
  curves = curves_;
  children = NULL;
  realArray center_array(1,2);
  center_array(0,0) = centerX;
  center_array(0,1) = centerY;

  // Check the effect of each curve on whether this square is inside the region.
  // Even after that's determined, we still want to check every active curve so
  // as to reduce its segment range for this square.
  // We are assuming some things about the topology: all curves are nonintersecting,
  // closed curves; the region is bounded by one "outer" curve with holes defined by
  // the remaining "inner" curves, which lie inside the outer curve.
  int curvemin = curves.curvestop();
  int curvemax;
  for ( int c=curves.curvestart(); c<curves.curvestop(); ++c ) {
    if ( tm.curveGoesThrough( *this, c, curves.nodestart(c), curves.nodestop(c) ) ) {
      // A curve passes through the square; part is in the region and part out
      inside = 0;
      curvemin = curvemin>c ? c : curvemin ;
      curvemax = c;
    }
    else if ( inside==-123 && tm.insideOrOutside(center_array,c)==-1 ) {
      // outside the region due to curve c
      // Because of the topological assumptions we can be outside due to only one curve.
      // Save it in curves.curvestart(), and set the distance to it, curveDist.
      inside = -1;
      real cd = distanceToCurve( c, tm );
      if ( cd>curves.curveDist() ) curves.curveDist() = cd;
      curves.curvestart() = c;
      curves.curvestop() = curves.curvestart()+1;
    }
  };
  if ( inside==0 ) {
    curves.curvestart() = curvemin;
    curves.curvestop() = curvemax+1;
  };
  if ( inside==-123 ) {
    // No curve goes through, and no curve makes this square outside the region.
    // We don't need to worry about distance to the nearest curve, as that is
    // not used for inside squares.
    inside = 1;
    curves.curvestop() = curves.curvestart();
  };
};

void TMquad::divide( TrimmedMapping& tm, int& sizeOfMesh, real& minWidth )
//===========================================================================
/// \brief  (maybe) refines the quadtree mesh of which this square is root.
///  This square may be divided into four, and each newly created square may
///  itself be divided, recursively.
/// \param tm (input) : the TrimmedMapping object for which the quadtree mesh is made
/// \param sizeOfMesh (input/output) : the number of squares in the quadtree mesh
///  (for diagnostic use)
/// \param minWidth (input/output) : the width of the smallest square in the mesh
///  (for diagnostic use)
/// \param General Notes: A square will not be divided unless it meets one of the
///  following criteria: <ul>
///  <li> the square is cut by a trimming curve, and $dx \geq dxMinNormal$
///  <li> the square is cut by two or more trimming curves, and $ dx \geq dxMin2Curve$
///  <li> the square is outside the region, but its distance to the nearest trimming
///  curve is less than tm.farthestDistanceNearCurve and $dx \geq dxMinNormal$
///  </ul>
///  where $dx$ is the square's half-width, and
///  $dxMinNormal$ and $dxMin2Curve$ are public static members of the TMquad class.
///  If you reset them, ensure that $dxMin2Curve \leq dxMinNormal$.
//===========================================================================
{
  // >>>> The quadtree mesh should be divided until, for every curve <<<<
  // >>>> there is a square entirely outside that curve.  It would be good to <<<<
  // >>>> check for that. <<<<
  // Otherwise, "holes" become invisible in plots because they get filled in during
  // the graphics projection process.
  ++sizeOfMesh;
  assert( sizeOfMesh > 0 );
  assert( minWidth > 0 );
  if ( children==NULL &&
       (
	( inside==0 &&
	  ( dx>=dxMinNormal ||
	    ( dx>=dxMin2Curve && curves.curvestop()>curves.curvestart()+1 ) )
	  )
	||
	( inside==-1 && dx>=dxMinNormal &&
	  curves.curveDist() < tm.farthestDistanceNearCurve
	  )
	)
       )
    {
      smallestSquareWidth = dx<smallestSquareWidth ? dx : smallestSquareWidth ;
      minWidth = dx<minWidth ? dx : minWidth ;
      children = new TMquad[4];
      real newdx = 0.5 * dx;
      real newcenterX, newcenterY;
      newcenterX = centerX - newdx;
      newcenterY = centerY + newdx;
      children[0].remake( tm, newcenterX, newcenterY, newdx, curves );
      newcenterX = centerX + newdx;
      children[1].remake( tm, newcenterX, newcenterY, newdx, curves );
      newcenterX = centerX - newdx;
      newcenterY = centerY - newdx;
      children[2].remake( tm, newcenterX,newcenterY, newdx, curves );
      newcenterX = centerX + newdx;
      children[3].remake( tm, newcenterX, newcenterY, newdx, curves );
    };

  if ( children != NULL ) {
    for ( int i=0; i<4; ++i ) children[i].divide( tm, sizeOfMesh, minWidth );
  }
};

bool TMquad::inThisSquare( real pointX, real pointY ) const
//===========================================================================
/// \brief  Returns true is the given point lies in this square, false otherwise.
///  Points lying on or very near the boundary are considered to be inside.
/// \param pointX (input) : x-coordinate of the point
/// \param pointY (input) : y-coordinate of the point
//===========================================================================
{
  // dx+REAL_EPSILON == dx when real dx = 0.5 !
  // *wdh* 010301 real dxeps = dx + 4.*REAL_EPSILON;
  real dxeps = dx + 8.*REAL_EPSILON;
  return fabs(pointX-centerX)<=dxeps && fabs(pointY-centerY)<=dxeps;
/* -- wdh
  if ( pointX>=(centerX-dxeps) && pointX<=(centerX+dxeps) &&
       pointY>=(centerY-dxeps) && pointY<=(centerY+dxeps) ) {
    return true;
  }
  else {
    return false;
  }
-- */
}

const TMquad* TMquad::squareItsIn( real pointX, real pointY ) const
//===========================================================================
/// \brief   Finds and returns a leaf square in the quadtree which contains the
///  given point; the point should be in this square.
/// \param pointX (input) : x-coordinate of the point
/// \param pointY (input) : y-coordinate of the point
//===========================================================================
{
  assert( inThisSquare( pointX, pointY ) );
  if ( children==NULL ) return this;
  if ( pointY>centerY ) {   // child square 0 or 1
    if ( pointX>centerX ) return children[1].squareItsIn( pointX, pointY );
    else return children[0].squareItsIn( pointX, pointY );
  }
  else {                    // child square 2 or 3
    if ( pointX>centerX ) return children[3].squareItsIn( pointX, pointY );
    else return children[2].squareItsIn( pointX, pointY );
  }
}

const TMquad* TMquad::squareItsIn( real pointX, real pointY, TMquad*& parent ) const
  // (this function is not presently used, 8mar99)
  // Finds a leaf square in the quadtree which contains the given point;
  // the point should be in this square.  The third argument, on input, should
  // be this square's parent square.  On output, it is the parent of the returned
  // leaf square.
{
  assert( inThisSquare( pointX, pointY ) );
  if ( children==NULL ) return this;
  parent = (TMquad*) this; // ok to cast away const: we're changing what parent points to, no more
  if ( pointY>centerY ) {   // child square 0 or 1
    if ( pointX>centerX ) return children[1].squareItsIn( pointX, pointY, parent );
    else return children[0].squareItsIn( pointX, pointY, parent );
  }
  else {                    // child square 2 or 3
    if ( pointX>centerX ) return children[3].squareItsIn( pointX, pointY, parent );
    else return children[2].squareItsIn( pointX, pointY, parent );
  }
};


real TMquad::distancePointToSegment( real x, real y,
				     real x0, real y0, real u0, real v0 ) const
  // returns the distance from a point X = (x,y) to a line segment
  // S = X - U = (x0,y0)-(u0,v0)
{
  // This won't do very well in some near-boundary situations.
  real answer1, answer2;
  real s = x0 - u0;
  real t = y0 - v0;
  real PdotS = (x-u0)*s + (y-v0)*t;
  real SdotS = s*s + t*t;
  // *wdh*  assert( SdotS > 16*REAL_EPSILON*REAL_EPSILON );
  if( SdotS <= 16.5*REAL_EPSILON*REAL_EPSILON  ) //subject to roundoff
    return SdotS;
  
  real scalfac = PdotS / SdotS;
  real xPonS = scalfac*s + u0;  // projection of P to S
  real yPonS = scalfac*t + v0;
  if ( fabs(s) >= fabs(t) )
    {  // best to parameterize S by x-coordinate
//      assert( fabs(s) > 4*REAL_EPSILON );
      if ( ( x0>u0 && xPonS >= u0 && xPonS <= x0 ) ||
	   ( x0<u0 && xPonS <= u0 && xPonS >= x0 )
	   ) {         // PonS is within S
	// return distance of P from its projection onto S
	answer1 = sqrt( (x-xPonS)*(x-xPonS) + (y-yPonS)*(y-yPonS) );
	return answer1;   // return |P-PonS|
      } else {  // return distance of P from nearest endpoint of S
	answer1 = sqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) );
	answer2 = sqrt( (x-u0)*(x-u0) + (y-v0)*(y-v0) );
	if ( answer1<answer2 ) { return answer1; }
	else { return answer2; };
      }
    }
  else
    {  // best to parameterize S by y-coordinate
//      assert( fabs(t) > 4*REAL_EPSILON );
      if ( ( y0>v0 && yPonS >= v0 && yPonS <= y0 ) ||
	   ( y0<v0 && yPonS <= v0 && yPonS >= y0 )
	   ) {         // PonS is within S
	answer1 = sqrt( (x-xPonS)*(x-xPonS) + (y-yPonS)*(y-yPonS) );
	return answer1;   // return |P-PonS|
      }
      else {  // return distance of P from nearest endpoint of S
	answer1 = sqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) );
	answer2 = sqrt( (x-u0)*(x-u0) + (y-v0)*(y-v0) );
	if ( answer1<answer2 ) { return answer1; }
	else { return answer2; };
      }
    }
};

real TMquad::distanceBetweenSegments( real x1, real y1, real u1, real v1,
				      real x2, real y2, real u2, real v2 ) const
  // returns the distance between non-intersecting line segments
  // S1=(x1,y1)-(u1,v1) and S2 = (x2,y2)-(u2,v2)
  // Suppose we want to find the distance between two line segments S1,S2.
  // Let P1,P2 be points of them which are closest.  Wnlog we can assume that
  // either P1 is an endpoint of S1 or P2 is an endpoint of S2.  Proof:
  //  Suppose P1,P2 _both_ be interior points.  They are distinct. First suppose that
  //  the line betwen them forms a right angle with both segments.  Then they are
  //  parallel and one of the endpoints is equally close to the other segment.  So
  //  suppose that the line between them forms non-right angles at one point, say with
  //  S1 at P1.  Then by moving along S1 on the acute side of the angle we can find
  //  another point of S1 closer to P2.
  // So we check all 4 endpoints.  For each point P, find the distance to the other
  // segment, S, by projecting onto its line.  If the projection Pr lies within S,
  // then |P-Pr| is the distance.  If not, find the endpoint of S, Ps, closest to Pr.
  // Then |P-Ps| is the distance.  This gives 4 point-segment distances; the
  // minimum is the segment-segment distance.

{
  real d1 = distancePointToSegment( x1,y1,  x2,y2, u2,v2 );
  real d2 = distancePointToSegment( u1,v1,  x2,y2, u2,v2 );
  real d3 = distancePointToSegment( x2,y2,  x1,y1, u1,v1 );
  real d4 = distancePointToSegment( u2,v2,  x1,y1, u1,v1 );
  real d12 = d1<d2 ? d1 : d2;
  real d21 = d3<d4 ? d3 : d4;
  // *wdh* assert( d21>0 );
  // *wdh* assert( d12>0 );
  assert( d21>=0. );
  assert( d12>=0. );
  return d12<d21 ? d12 : d21;
};

real TMquad::distanceToCurve( int c, TrimmedMapping& tm ) const
  // returns the distance from this square to the given curve.
  // Assumes the curve does not cut this square (i.e., the distance
  // is a priori known to be nonzero).
{
  assert( !tm.curveGoesThrough( *this, c ) );
  real dist = 1.0;
  real xL = centerX - dx;
  real xR = centerX + dx;
  real yL = centerY - dx;
  real yU = centerY + dx;
  realArray & rc = tm.rCurve[c];
  int segstart = 0;
  int segstop = -1;
  if ( segstop==-1 ) {
    segstop = rc.getLength(0)-1;
  };

  for( int m=segstart; m<segstop; m++ ) {
    // segment endpoints; if segment illegally leaves the domain (unit square),
    // treat as stopping at border of domain
    real u0=max(0.,min(1.,rc(m  ,0)));
    real v0=max(0.,min(1.,rc(m  ,1)));
    real u1=max(0.,min(1.,rc(m+1,0)));
    real v1=max(0.,min(1.,rc(m+1,1)));

    real dLower = distanceBetweenSegments( xL,yL, xR,yL,  u0,v0, u1,v1 );
    real dUpper = distanceBetweenSegments( xL,yU, xR,yU,  u0,v0, u1,v1 );
    real dLeft  = distanceBetweenSegments( xL,yL, xL,yU,  u0,v0, u1,v1 );
    real dRight = distanceBetweenSegments( xR,yL, xR,yU,  u0,v0, u1,v1 );
    real dHoriz = dLower<dUpper ? dLower : dUpper;
    real dVert  = dLeft<dRight  ? dLeft  : dRight;
    real d = dHoriz<dVert ? dHoriz : dVert;
    // *wdh*    assert( d>0 );
    assert( d>=0. );
    dist = dist<d ? dist : d;
  };

  return dist;
};

//void TMquad::plot( GenericGraphicsInterface & gi ) const
//void TMquad::plot( PlotStuff & gi, PlotStuffParameters parameters ) const
void TMquad::plot( GenericGraphicsInterface & gi, GraphicsParameters parameters ) const
{
  // This function isn't presently used because it doesn't work with the
  // current (0299) graphics package.
  // It seems to work better not to graphics square-by-square; instead
  // accumulate data into a RealArray, and graph it all in one place.
  // When called this way, which overlays plots from different function calls,
  // the plot package is erratic, probably has a few bugs.

// This plotting uses grid functions through the plot function and can therefore not be used until
// a special plot function for realArrays has been implemented
//    realArray t(5);
//    realArray x(5,1);

//    t(0) = centerX - dx;   x(0,0) = centerY - dx;
//    t(1) = centerX - dx;   x(1,0) = centerY + dx;
//    t(2) = centerX + dx;   x(2,0) = centerY + dx;
//    t(3) = centerX + dx;   x(3,0) = centerY - dx;
//    t(4) = t(0);   x(4,0) = x(0,0);
//    //  cerr << "plot t,x " << t(0) << ',' << x(0,0) << ' ' << t(2) << ',' << x(2,0) << endl;

//    parameters.set(GI_LABEL_GRIDS_AND_BOUNDARIES,FALSE);
//    parameters.set(GI_USE_PLOT_BOUNDS,TRUE);

//    PlotIt::plot(gi,t,x,nullString,nullString,NULL,parameters);

//    if ( children != NULL ) {
//      for ( int i=0; i<4; ++i ) children[i].plot(gi,parameters);
//    }
};

void TMquad::accumulateCenterPoints( realArray& points,
				     const int startID  ) const
  // (this function is not presently used, 8mar99)
  // Fills the array "points" with the center points of all squares of this quadtree.
  // The array indices begin with "startID" for this square.
{
  assert( points.numberOfDimensions() == 2 );
  assert( points.getBase(0) <= startID );
  assert( points.getBase(1) == 0 );
  assert( points.getBound(0) >= startID );
  assert( points.getBound(1) >= 2 );   assert( points.getBound(1) <= 3 );
  assert( nextID >= 0 );

  nextID = startID;
  int index = nextID;
  ++nextID;

  points( index, 0 ) = centerX;
  points( index, 1 ) = centerY;
  if ( points.getBound(1)==3 ) points( index, 2 ) = 0.0;

  if ( children != NULL ) {
    for ( int i=0; i<4; ++i ) children[i].accumulateCenterPoints( points );
  }
};

void TMquad::accumulateCenterPoints( realArray& points, realArray& inout,
				     const int startID ) const
//===========================================================================
/// \brief  fills an array with  the center points of all squares of this quadtree
///  and fills sets another array to indicate whether the squares are inside the
///  trimmed surface.
/// \param points(R,.) (output) : array of x,y values of center points.  If the second
///  index has a range size of 3, {\bf points} will also be given a z value of 0.
///  To ensure that this array is large enough, save and use the sizeOfMesh argument
///  of the last call of TMquad::divide on this quadtree square
///  (c.f. TrimmedMapping::sizeOfQuadTreeMesh).
/// \param inout(R) (output) : inside/outside array.  It may take on the following values:
///  <ul>
///  <li> -1: the square (including borders) is outside the trimmed area
///  <li> 0: the square is cut by one trimming curve, and is a leaf node of the quadtree
///  <li> 0.5: the square is cut by two or more trimming curves, and is a leaf node
///  of the quadtree
///  <li> 1: the square (including borders) is inside the trimmed area
///  <li> 2: the square is not a leaf node of the quadtree; hence is cut by a trimming
///  curve or is outside but near a trimming curve
///  </ul>
/// \param startID (input) : starting index value into the arrays points and inout
//===========================================================================
{
  assert( points.numberOfDimensions() == 2 );
  assert( points.getBase(0) <= startID );
  assert( points.getBase(1) == 0 );
  assert( points.getBound(0) >= startID );
  assert( points.getBound(1) >= 2 );   assert( points.getBound(1) <= 3 );
  assert( inout.numberOfDimensions() == 1 );
  assert( inout.getBase(0) <= startID );
  assert( inout.getBound(0) >= startID );
  assert( nextID >= 0 );

  nextID = startID;
  int index = nextID;
  ++nextID;

  points( index, 0 ) = centerX;
  points( index, 1 ) = centerY;
  if ( points.getBound(1)==3 ) points( index, 2 ) = 0.0;
  inout( index ) = inside ;

  if ( inside==0 && curves.curvestop()>curves.curvestart()+1 ) {
    inout( index ) = 0.5; // special designation for >=2 curves cutting a square
  };
  if ( children != NULL ) {
    assert( inside == 0 || inside==-1 );
    inout( index ) = 2 ;  // special designation for non-leaf squares
    for ( int i=0; i<4; ++i ) children[i].accumulateCenterPoints( points, inout );
  }
};

int TMquad::TMget( const GenericDataBase & dir, const aString & name,
		   TrimmedMapping& tm, TMcurves * curves_ )
  // curves_ is optional, defaults to NULL
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  // *wdh* subDir.setMode(GenericDataBase::streamInputMode);

  aString className;
  subDir.get( className,"className" ); 
  if( className != "TMquad" ) {
    cout << "TMquad::get ERROR in className!, got=[" << className << "]" << endl;
  };
  subDir.get( centerX, "centerX" );
  subDir.get( centerY, "centerY" );
  subDir.get( dx, "dx" );
  // *wdh assert( centerX > 0 );  assert( centerX < 1 );
  // *wdh assert( centerY > 0 );  assert( centerY < 1 );
  assert( dx > 0 );  assert( dx < 1 );

  if ( curves_ == NULL ) curves_ = new TMcurves(tm);
  remake( tm, centerX, centerY, dx, *curves_ );

  int childrenExist;
  subDir.get( childrenExist, "childrenExist" );
  assert( childrenExist==0 || childrenExist==1 );
  if ( childrenExist==0 ) {
    children = NULL;
  }
  else {
    children = new TMquad[4];
    char buff[80];
    for ( int i=0; i<4; ++i ) {
      sprintf(buff,"child%1.1i",i);
      children[i].TMget( subDir, buff, tm, &curves );
    }
  };

  delete &subDir;
  return 0;
};
  

int TMquad::put( GenericDataBase & dir, const aString & name) const
{
  GenericDataBase & subDir = *dir.virtualConstructor();   // derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  //  *wdh*   subDir.setMode(GenericDataBase::streamOutputMode);

  aString className="TMquad";
  subDir.put( className, "className" );

  subDir.put( centerX, "centerX" );
  subDir.put( centerY, "centerY" );
  subDir.put( dx, "dx" );
  int childrenExist = children==NULL ? 0 : 1;
  subDir.put( childrenExist, "childrenExist" );
  if ( children != NULL ) {
    char buff[80];
    for ( int i=0; i<4; ++i ) {
      sprintf(buff,"child%1.1i",i);
      children[i].put(subDir,buff);
    }
  };

  delete &subDir;
  return 0;
};

void TMquad::getStatics( GenericDataBase & dir ) const
{
  dir.get( maxsquares, "maxsquares" );
  dir.get( nextID, "nextID" );
  dir.get( smallestSquareWidth, "smallestSquareWidth" );
  dir.get( dxMinNormal, "dxMinNormal" );
  dir.get( dxMin2Curve, "dxMin2Curve" );
};

void TMquad::putStatics( GenericDataBase & dir ) const
{
  dir.put( maxsquares, "maxsquares" );
  dir.put( nextID, "nextID" );
  dir.put( smallestSquareWidth, "smallestSquareWidth" );
  dir.put( dxMinNormal, "dxMinNormal" );
  dir.put( dxMin2Curve, "dxMin2Curve" );
};

int TMquadRoot::TMget( const GenericDataBase & dir, const aString & name,
		       TrimmedMapping& tm, TMcurves * curves_ )
{
  dir.get( sizeOfQuadTreeMesh, "sizeOfQuadTreeMesh" );
  dir.get( minQuadTreeMeshDx, "minQuadTreeMeshDx" );
  return TMquad::TMget(dir,name,tm,curves_);
};

int TMquadRoot::put( GenericDataBase & dir, const aString & name) const
{
  dir.put( sizeOfQuadTreeMesh, "sizeOfQuadTreeMesh" );
  dir.put( minQuadTreeMeshDx, "minQuadTreeMeshDx" );
  return TMquad::put(dir,name);
};

  // >>>> to do: pass curves on to children in TMget
